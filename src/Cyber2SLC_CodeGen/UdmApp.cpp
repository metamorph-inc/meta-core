#include "stdafx.h"
#include "UdmApp.h"
#include "UdmConfig.h"

#include "Uml.h"
#include "UdmUtil.h"

#include <boost/filesystem.hpp>
#include <boost/python.hpp>
#include <queue>

#include "Mga.h"

#include "ModelicaCodeGen.h"
#include "runSL_CodeGen.h"

#include "UdmConsole.h"

//#include "utils.h"

#include "CGLog.h"

#include <algorithm>
#include <UdmUtil.h>

__declspec(noreturn) void ThrowComError(HRESULT hr, LPOLESTR err);

#include <Python.h>

struct PyObject_RAII
{
	PyObject* p;
	PyObject_RAII() : p(NULL) { }
	PyObject_RAII(PyObject* p) : p(p) { }
	operator PyObject*() { return p; }
	~PyObject_RAII() { Py_XDECREF(p); }
};

std::string GetPythonError()
{
	PyObject_RAII type, value, traceback;
	PyErr_Fetch(&type.p, &value.p, &traceback.p);
	PyErr_Clear();
	std::string error;
	if (type)
	{
		PyObject_RAII type_name = PyObject_GetAttrString(type, "__name__");
		if (type_name && PyString_Check(type_name))
		{
			error += PyString_AsString(type_name);
			error += ": ";
		}
	}
	if (value && PyString_Check(value))
	{
		PyObject_RAII str_value = PyObject_Str(value);
		error += PyString_AsString(str_value);
	}
	else
		error += "Unknown exception";
	error += ": ";
	if (traceback)
	{
		PyObject_RAII main = PyImport_ImportModule("__main__");
		PyObject* main_namespace = PyModule_GetDict(main);
		PyObject_RAII dict = PyDict_Copy(main_namespace);
		PyDict_SetItemString(dict, "tb", traceback);
		PyObject_RAII _none = PyRun_StringFlags(
			"import traceback\n"
			"tb = ''.join(traceback.format_tb(tb))\n", Py_file_input, dict, dict, NULL);
		PyObject* formatted_traceback = PyDict_GetItemString(dict, "tb");
		error += PyString_AsString(formatted_traceback);
	}
	else
		error += "Unknown traceback";
	return error;
}


using namespace std;

void showUsage()
{
	CString usage("Cyber2SLC_CodeGen interpreter cannot be invoked. Please launch the interpreter inside a SimulinkWrapper model.\r\n");
	AfxMessageBox(usage,MB_ICONINFORMATION);				
}

std::string CUdmApp::createWrapperFolder(const std::string &foldername)
{
	boost::filesystem::current_path(CUdmApp::currPath);
		
	if ( !boost::filesystem::exists(foldername) )
	{
		boost::filesystem::create_directory(foldername);
		printLog("create folder: "+foldername);
	}
	boost::filesystem::path work_dir(CUdmApp::currPath+"\\"+foldername);
	boost::filesystem::current_path(work_dir);
	boost::system::error_code err;
	boost::filesystem::create_directory("Simulink", err);
	printLog("create folder: SignalFlow");	

	return work_dir.string()+"\\Simulink";
}

void CUdmApp::removeWrapperFolder(const std::string &foldername)
{
	boost::filesystem::path work_dir(CUdmApp::currPath);
	boost::filesystem::current_path(work_dir);
	if(!boost::filesystem::remove_all(foldername))
	{
		std::string err = "boost::filesystem::remove(\""+foldername+"\") fails";
		throw exception(err.c_str());
	}
}

//void __declspec(dllimport) runCyPhySF_CodeGen_dllimported(const CyPhyML::SignalFlowModel &sfmodel, std::string cyphy_file, Uml::Diagram &cyphy_meta_diagram, std::string directory="");
void CUdmApp::updateInputPort(CyberComposition::Simulink::SFData &sfdata)
{
	set<CyberComposition::Simulink::ConnToSFStates> srcConns = sfdata.srcConnToSFStates();
	for(auto it_src=srcConns.begin();it_src!=srcConns.end();++it_src)
	{
		CyberComposition::ConnToSFStates_Members_Base sport = (*it_src).srcConnToSFStates_end();
		if(Uml::IsDerivedFrom(sport.type(), CyberComposition::Simulink::InputPort::meta))
		{
			CyberComposition::Simulink::InputPort iport = CyberComposition::Simulink::InputPort::Cast(sport);
			iport.Number() = sfdata.Port();
			iport.name() = sfdata.name();
		}
	}
}

void CUdmApp::updateOutputPort(CyberComposition::Simulink::SFData &sfdata)
{
	set<CyberComposition::Simulink::ConnToSFStates> dstConns = sfdata.dstConnToSFStates();
	for(auto it_dst=dstConns.begin();it_dst!=dstConns.end();++it_dst)
	{
		CyberComposition::ConnToSFStates_Members_Base dport = (*it_dst).dstConnToSFStates_end();
		//CyPhyML::ConnToSFStates_Members_Base dport = (*it_dst).dstConnToSFStates_end();
		if(Uml::IsDerivedFrom(dport.type(), CyberComposition::Simulink::OutputPort::meta))
		{
			CyberComposition::Simulink::OutputPort oport = CyberComposition::Simulink::OutputPort::Cast(dport);
			oport.Number() = sfdata.Port();
			oport.name() = sfdata.name();
		}
	}
}

void CUdmApp::checkSFState(CyberComposition::Simulink::SFState &sfstate)
{
	string statename = sfstate.name();
	printLog( "Visiting state " + statename + "." );
	sfstate.Name() = sfstate.name();
	CyberComposition::Simulink::SFState state = sfstate;
	CyberComposition::Simulink::Block sub_sfstate = sfstate.Block_parent();
	sub_sfstate.name() = sfstate.name();
	if(sfstate.isInstance())
		state = sfstate.Archetype();
	if(checkedSFStates.find(state)!=checkedSFStates.end())
		return;
	checkedSFStates.insert(state);
	CyberComposition::Simulink::Block sub = state.Block_parent();
	sub.name() = state.name();

	int in=1;
	int out=1;
	set<CyberComposition::Simulink::SFData> datas = state.SFData_kind_children();
	for(auto it=datas.begin();it!=datas.end();++it)
	{
		CyberComposition::Simulink::SFData data = *it;
		std::string scope = data.Scope();
		data.Name() = data.name();
		set<CyberComposition::Simulink::SFData> data_insts = data.Instances();
		if(scope=="INPUT_DATA")
		{
			data.Port() = in++;
			updateInputPort(data);
			for(auto it_inst=data_insts.begin();it_inst!=data_insts.end();++it_inst)
			{
				CyberComposition::Simulink::SFData data_inst = *it_inst;
				updateInputPort(data_inst);
			}
		}
		else if(scope=="OUTPUT_DATA")
		{
			data.Port() = out++;
			updateOutputPort(data);
			for(auto it_inst=data_insts.begin();it_inst!=data_insts.end();++it_inst)
			{
				CyberComposition::Simulink::SFData data_inst = *it_inst;
				updateOutputPort(data_inst);
			}
		}
		
		for(auto it_inst=data_insts.begin();it_inst!=data_insts.end();++it_inst)
		{
			(*it_inst).Port() = data.Port();
		}
	}
}

void CUdmApp::checkPortNumber(const CyberComposition::Simulink::Block &block)
{
	string blockname = block.name();
	printLog( "Visiting block " + blockname + "." );
	set<unsigned int> numbers;
	set<CyberComposition::Simulink::InputPort> inports = block.InputPort_kind_children();
	for(unsigned int i=1; i<=(unsigned int)inports.size();++i)
		numbers.insert(i);
	set<CyberComposition::Simulink::InputPort> ip_zeros;
	set<unsigned int> used_ints;
	for(auto it_in=inports.begin();it_in!=inports.end();++it_in)
	{
		CyberComposition::Simulink::InputPort inport = *it_in;
		long long n = inport.Number();
		if(n==0) 
			ip_zeros.insert(inport);
		else
		{
			set<unsigned int>::iterator in_pos = numbers.find((unsigned int) n);
			if(in_pos!=numbers.end())
				numbers.erase(in_pos);
			else
			{
				in_pos = used_ints.find((unsigned int) n);
				if(in_pos!=used_ints.end())
					ip_zeros.insert(inport);
			}
			used_ints.insert((unsigned int) n);
		}
	}
	for(auto it_z=ip_zeros.begin();it_z!=ip_zeros.end();++it_z)
	{
		CyberComposition::Simulink::InputPort ip = *it_z;
		int n = *(numbers.begin());
		numbers.erase(numbers.begin());
		ip.Number() = n;
		if(ip.isInstance())
			CyberComposition::Simulink::InputPort::Cast(ip.Archetype()).Number() = n;
	}

	numbers.clear();
	used_ints.clear();
	
	set<CyberComposition::Simulink::OutputPort> outports = block.OutputPort_kind_children();
	for(unsigned int i=1; i<=(unsigned int) outports.size();++i)
		numbers.insert(i);
	set<CyberComposition::Simulink::OutputPort> op_zeros;
	for(auto it_o=outports.begin();it_o!=outports.end();++it_o)
	{
		CyberComposition::Simulink::OutputPort outport = *it_o;
		long long n = outport.Number();
		if(n==0) 
			op_zeros.insert(outport);
		else
		{
			set<unsigned int>::iterator out_pos = numbers.find((unsigned int) n);
			if(out_pos!=numbers.end())
				numbers.erase(out_pos);
			else
			{
				out_pos = used_ints.find((unsigned int) n);
				if(out_pos!=used_ints.end())
					op_zeros.insert(outport);
			}
			used_ints.insert((unsigned int) n);
		}
	}

	for(auto it_zo=op_zeros.begin();it_zo!=op_zeros.end();++it_zo)
	{
		CyberComposition::Simulink::OutputPort op = *it_zo;
		unsigned int n = *(numbers.begin());
		numbers.erase(numbers.begin());
		op.Number() = n;
		if(op.isInstance())
			CyberComposition::Simulink::OutputPort::Cast(op.Archetype()).Number() = n;
	}
	
	if(Uml::IsDerivedFrom(block.type(), CyberComposition::Simulink::Subsystem::meta))
	{
		CyberComposition::Simulink::Subsystem subsys = CyberComposition::Simulink::Subsystem::Cast(block);
		set<CyberComposition::Simulink::Block> blocks = subsys.Block_kind_children();
		for(auto it_block=blocks.begin();it_block!=blocks.end();++it_block)
			checkPortNumber(*it_block);
	}
}

void CUdmApp::checkSimulinkWrapper(CyberComposition::SimulinkWrapper &sl)
{
	set<CyberComposition::Simulink::Subsystem> subs = sl.Simulink_Subsystem_kind_children();
	for(auto it_sub=subs.begin();it_sub!=subs.end();++it_sub)
	{
		printLog( "Checking for invalid object names..." );
		checkBadNames(*it_sub);
		printLog( "Checking for port numbers..." );
		checkPortNumber(*it_sub);
		printLog( "Checking for port data type objects..." );
		checkBadPorts(*it_sub);
	}
		
	if ( !badNameObjs.empty() )
	{
		string outmsg( "Found the following invalid names in SimulinkWrapper model: "+std::string(sl.name())+"\n" );
		GMEConsole::Console::writeLine(outmsg, MSG_ERROR);
		printLog( outmsg );
		while(!badNameObjs.empty())
		{
			Udm::Object obj = badNameObjs.front();
			badNameObjs.pop_front();

			std::string obj_type = obj.type().name();
			std::string obj_name = UdmUtil::ExtractName(obj);
			std::string err = obj_type +":\t<A HREF=\"mga:"+ UdmGme::UdmId2GmeId(obj.uniqueId()) + "\">" + obj_name + "</A>";
			GMEConsole::Console::writeLine("\t"+err, MSG_ERROR);
			printLog(obj_type+":\t"+obj_name);
		}
	}

	if ( !badPortObjs.empty() )
	{
		string outmsg( "Found the following ports with empty data type references in SimulinkWrapper model: "+std::string(sl.name())+"\n" );
		GMEConsole::Console::writeLine(outmsg, MSG_ERROR);
		printLog( outmsg );
		while(!badPortObjs.empty())
		{
			Udm::Object obj = badPortObjs.front();
			badPortObjs.pop_front();

			std::string obj_type = obj.type().name();
			std::string obj_name = UdmUtil::ExtractName(obj);
			std::string err = obj_type +":\t<A HREF=\"mga:"+ UdmGme::UdmId2GmeId(obj.uniqueId()) + "\">" + obj_name + "</A>";
			GMEConsole::Console::writeLine("\t"+err, MSG_ERROR);
			printLog(obj_type+":\t"+obj_name);
		}
	}
}

#define BAD_NAME_CHARS " ;.,#:'\"?<>-+=*\n\t"

void CUdmApp::checkBadNames(const CyberComposition::Simulink::Block &block)
{
	string blockname = block.name();
	printLog( "Visiting block " + blockname + "." );
	if(Uml::IsDerivedFrom(block.type(), CyberComposition::Simulink::Subsystem::meta))
	{
		const CyberComposition::Simulink::Subsystem &subsys = CyberComposition::Simulink::Subsystem::Cast(block);
		string subName = subsys.name();
		if ( subName.find_first_of( BAD_NAME_CHARS ) != std::string::npos ) 
		{
			badNameObjs.push_back(subsys);
		}
		
		set<CyberComposition::Simulink::Block> blocks = subsys.Block_kind_children();
		for(set<CyberComposition::Simulink::Block>::iterator bit=blocks.begin();bit!=blocks.end();++bit)
		{
			CyberComposition::Simulink::Block currBlock = *bit;
			checkBadNames(currBlock);
		}
	}
	else if(Uml::IsDerivedFrom(block.type(), CyberComposition::Simulink::Primitive::meta))
	{
		string primitiveName = block.name();
		if ( primitiveName.find_first_of( BAD_NAME_CHARS ) != std::string::npos ) {
			badNameObjs.push_back(block);
		}
	}
	
	set<CyberComposition::Simulink::State> states = block.State_kind_children();
	for (set<CyberComposition::Simulink::State>::iterator stateIter = states.begin(); stateIter != states.end(); stateIter++ )
	{
		string stateName = (*stateIter).name();
		if ( stateName.find_first_of( BAD_NAME_CHARS ) != std::string::npos ) {
			badNameObjs.push_back(*stateIter);
		}

		set<CyberComposition::Simulink::Transition> transitions = (*stateIter).Transition_kind_children();
		for ( set<CyberComposition::Simulink::Transition>::iterator transIter = transitions.begin();  transIter != transitions.end();  transIter++ )
		{
			string transName = (*transIter).name();
			if ( transName.find_first_of( BAD_NAME_CHARS ) != std::string::npos ) {
				badNameObjs.push_back(*transIter);
			}
		}

		set<CyberComposition::Simulink::Data> dataelts = (*stateIter).Data_kind_children();
		for (set<CyberComposition::Simulink::Data>::iterator dataIter = dataelts.begin(); dataIter != dataelts.end();  dataIter++ )
		{
			string dataName = (*dataIter).name();
			if ( dataName.find_first_of( BAD_NAME_CHARS ) != std::string::npos ) {
				badNameObjs.push_back(*dataIter);
			}				
		}

		set<CyberComposition::Simulink::Event> events = (*stateIter).Event_kind_children();
		for ( set<CyberComposition::Simulink::Event>::iterator eventIter = events.begin(); eventIter != events.end(); eventIter++ )
		{
			string eventName = (*eventIter).name();
			if ( eventName.find_first_of( BAD_NAME_CHARS ) != std::string::npos ) {
				badNameObjs.push_back(*eventIter);
			}
		}
	}

	set<CyberComposition::Simulink::SFState> sfstates = block.SFState_kind_children();
	for (set<CyberComposition::Simulink::SFState>::iterator sfstateIter = sfstates.begin(); sfstateIter != sfstates.end(); sfstateIter++ )
	{
		CyberComposition::Simulink::SFState sfState = *sfstateIter;
		string sfstateName = (*sfstateIter).name();
		if ( sfstateName.find_first_of( BAD_NAME_CHARS ) != std::string::npos ) {
			badNameObjs.push_back(*sfstateIter);
		}

		set<CyberComposition::Simulink::SFTransition> sftransitions = (*sfstateIter).SFTransition_kind_children();
		for ( set<CyberComposition::Simulink::SFTransition>::iterator sftransIter = sftransitions.begin();  sftransIter != sftransitions.end();  sftransIter++ )
		{
			string sftransName = (*sftransIter).name();
			if ( sftransName.find_first_of( BAD_NAME_CHARS ) != std::string::npos ) {
				badNameObjs.push_back(*sftransIter);
			}
		}

		set<CyberComposition::Simulink::SFData> sfdataelts = (*sfstateIter).SFData_kind_children();
		for ( set<CyberComposition::Simulink::SFData>::iterator sfdataIter = sfdataelts.begin(); sfdataIter != sfdataelts.end();  sfdataIter++ )
		{
			string sfdataName = (*sfdataIter).name();
			if ( sfdataName.find_first_of( BAD_NAME_CHARS ) != std::string::npos ) {
				badNameObjs.push_back(*sfdataIter);
			}
		}

		set<CyberComposition::Simulink::SFEvent> sfevents = (*sfstateIter).SFEvent_kind_children();
		for ( set<CyberComposition::Simulink::SFEvent>::iterator sfeventIter = sfevents.begin(); sfeventIter != sfevents.end();  sfeventIter++ )
		{
			string sfeventName = (*sfeventIter).name();
			if ( sfeventName.find_first_of( BAD_NAME_CHARS ) != std::string::npos ) {
				badNameObjs.push_back(*sfeventIter);
			}				
		}

		//check SFState 
		checkSFState(sfState);
	}

	set<CyberComposition::Simulink::SF_Parameter> parameters = block.SF_Parameter_kind_children();
	for(auto it_parameter=parameters.begin();it_parameter!=parameters.end();++it_parameter)
	{
		string parameterName = (*it_parameter).name();
		if ( parameterName.find_first_of( BAD_NAME_CHARS ) != std::string::npos ) {
			badNameObjs.push_back(*it_parameter);
		}				
	}

	set<CyberComposition::Simulink::SF_Port> ports = block.SF_Port_kind_children();
	for(auto it_port=ports.begin();it_port!=ports.end();++it_port)
	{
		string portName = (*it_port).name();
		if ( portName.find_first_of( BAD_NAME_CHARS ) != std::string::npos ) {
			badNameObjs.push_back(*it_port);
		}				
	}
}

void CUdmApp::checkBadPorts(const CyberComposition::Simulink::Block & block)
{
	string blockname = block.name();
	printLog( "Visiting block " + blockname + "." );
	if(Uml::IsDerivedFrom(block.type(), CyberComposition::Simulink::Subsystem::meta))
	{
		const CyberComposition::Simulink::Subsystem &subsys = CyberComposition::Simulink::Subsystem::Cast(block);

		set< CyberComposition::Simulink::SF_Port > ports = subsys.SF_Port_children();
		for ( set< CyberComposition::Simulink::SF_Port >::iterator portIter = ports.begin(); portIter != ports.end(); portIter++ )
		{
			CyberComposition::Simulink::TypeBaseRef tbr = (*portIter).TypeBaseRef_child();
			if ( tbr == Udm::null )
			{
				badPortObjs.push_back( *portIter );
			}
		}
		
		set<CyberComposition::Simulink::Block> blocks = subsys.Block_kind_children();
		for(set<CyberComposition::Simulink::Block>::iterator bit=blocks.begin();bit!=blocks.end();++bit)
		{
			CyberComposition::Simulink::Block currBlock = *bit;
			checkBadPorts(currBlock);
		}
	}
	else if(Uml::IsDerivedFrom(block.type(), CyberComposition::Simulink::Primitive::meta))
	{
		CyberComposition::Simulink::Primitive prim = CyberComposition::Simulink::Primitive::Cast( block );

		set< CyberComposition::Simulink::SF_Port > ports = prim.SF_Port_children();
		for ( set< CyberComposition::Simulink::SF_Port >::iterator portIter = ports.begin(); portIter != ports.end(); portIter++ )
		{
			CyberComposition::Simulink::TypeBaseRef tbr = (*portIter).TypeBaseRef_child();
			if ( tbr == Udm::null )
			{
				badPortObjs.push_back( *portIter );
			}
		}
	}
	
	set<CyberComposition::Simulink::State> states = block.State_kind_children();
	for (set<CyberComposition::Simulink::State>::iterator stateIter = states.begin(); stateIter != states.end(); stateIter++ )
	{
		checkBadPorts_State_Data( *stateIter );
	}

	set<CyberComposition::Simulink::SFState> sfstates = block.SFState_kind_children();
	for (set<CyberComposition::Simulink::SFState>::iterator sfstateIter = sfstates.begin(); sfstateIter != sfstates.end(); sfstateIter++ )
	{
		checkBadPorts_SFState_Data( *sfstateIter );
	}
}

void CUdmApp::checkBadPorts_State_Data( const CyberComposition::Simulink::State &state)
{
	string statename = state.name();
	printLog( "Visiting state " + statename + "." );
	set< CyberComposition::Simulink::StateDE > stde = state.StateDE_children();
	for ( set < CyberComposition::Simulink::StateDE >::iterator stdeIter = stde.begin(); stdeIter != stde.end(); stdeIter++ )
	{
		CyberComposition::Simulink::TypeBaseRef tbr = (*stdeIter).TypeBaseRef_child();
		if ( tbr == Udm::null )
		{
			badPortObjs.push_back( *stdeIter );
		}
	}

	set< CyberComposition::Simulink::State > states = state.State_kind_children();
	for ( set< CyberComposition::Simulink::State >::iterator stIter = states.begin(); stIter != states.end(); stIter++ )
	{
		checkBadPorts_State_Data( *stIter );
	}
}

void CUdmApp::checkBadPorts_SFState_Data( const CyberComposition::Simulink::SFState &sf_state)
{
	string statename = sf_state.name();
	printLog( "Visiting SF state " + statename + "." );
	set< CyberComposition::Simulink::SFStateDE > stde = sf_state.SFStateDE_children();
	for ( set < CyberComposition::Simulink::SFStateDE >::iterator stdeIter = stde.begin(); stdeIter != stde.end(); stdeIter++ )
	{
		CyberComposition::Simulink::TypeBaseRef tbr = (*stdeIter).TypeBaseRef_child();
		if ( tbr == Udm::null )
		{
			badPortObjs.push_back( *stdeIter );
		}
	}

	set< CyberComposition::Simulink::SFState > states = sf_state.SFState_kind_children();
	for ( set< CyberComposition::Simulink::SFState >::iterator stIter = states.begin(); stIter != states.end(); stIter++ )
	{
		checkBadPorts_SFState_Data( *stIter );
	}
}

bool CUdmApp::processSimulinkWrapper(CyberComposition::SimulinkWrapper &sl,  bool genmo, std::string comname)
{
	boost::filesystem::path curr_path = boost::filesystem::current_path();

	if(comname.empty())
		comname = sl.name();
	
	set<CyberComposition::Simulink::Subsystem> subs = sl.Simulink_Subsystem_kind_children();
	for(auto it_sub=subs.begin();it_sub!=subs.end();++it_sub)
	{
		printLog( "Checking for invalid object names..." );
		checkBadNames(*it_sub);
		printLog( "Checking for port numbers..." );
		checkPortNumber(*it_sub);
		printLog( "Checking for port data type objects..." );
		checkBadPorts(*it_sub);
	}
		
	bool clean = true;
	if ( !badNameObjs.empty() )
	{
		string outmsg( "Found the following invalid names in SimulinkWrapper model: "+std::string(sl.name())+"\n" );
		GMEConsole::Console::writeLine(outmsg, MSG_ERROR);
		printLog( outmsg );
		while(!badNameObjs.empty())
		{
			Udm::Object obj = badNameObjs.front();
			badNameObjs.pop_front();

			std::string obj_type = obj.type().name();
			std::string obj_name = UdmUtil::ExtractName(obj);
			std::string err = obj_type +":\t<A HREF=\"mga:"+ UdmGme::UdmId2GmeId(obj.uniqueId()) + "\">" + obj_name + "</A>";
			GMEConsole::Console::writeLine("\t"+err, MSG_ERROR);
			printLog(obj_type+":\t"+obj_name);
		}
		clean = false;
	}

	if ( !badPortObjs.empty() )
	{
		string outmsg( "Found the following ports with empty data type references in SimulinkWrapper model: "+std::string(sl.name())+"\n" );
		GMEConsole::Console::writeLine(outmsg, MSG_ERROR);
		printLog( outmsg );
		while(!badPortObjs.empty())
		{
			Udm::Object obj = badPortObjs.front();
			badPortObjs.pop_front();

			std::string obj_type = obj.type().name();
			std::string obj_name = UdmUtil::ExtractName(obj);
			std::string err = obj_type +":\t<A HREF=\"mga:"+ UdmGme::UdmId2GmeId(obj.uniqueId()) + "\">" + obj_name + "</A>";
			GMEConsole::Console::writeLine("\t"+err, MSG_ERROR);
			printLog(obj_type+":\t"+obj_name);
		}
		clean = false;
	}

	if ( !clean )
		return false;

	MoCodeGen mocg(sl, curr_path.string(), CUdmApp::logFile);
	mocg.setPackageName(comname);
	mocg.gen();
	return true;

}

void CUdmApp::UdmMain(
					 Udm::DataNetwork* p_backend,		// Backend pointer(already open!)
					 Udm::Object focusObject,			// Focus object
					 std::set<Udm::Object> selectedObjects,	// Selected objects
					 long param)						// Parameters
{	
	boost::filesystem::path orig_path = boost::filesystem::current_path();

	if(!outputDir.empty())
	{
		if(!boost::filesystem::exists(outputDir))
		{
			std::string err = outputDir+" doesn't exist.";
			AfxMessageBox(err.c_str());
			return;
		}			
		currPath = outputDir;
		boost::filesystem::current_path(currPath);
	}

	boost::filesystem::path path((LPCTSTR)mgaPath);
	logFile = path.stem().string()+"_cg.log";
	
	if(currPath.empty())
	{
		currPath = path.remove_filename().string();
		boost::filesystem::current_path(currPath);
		boost::filesystem::create_directory("results");
		currPath = currPath + "\\results";
	}
	

	boost::filesystem::current_path(currPath);
	boost::filesystem::create_directory("log");
	boost::filesystem::current_path(currPath+"\\log");
	openLogFile(logFile);
	logFile = boost::filesystem::current_path().string()+"\\"+logFile;   //full path of logfile

	boost::filesystem::current_path(currPath);
	
	if(outputDir.empty())
		printLog("create folder: results");
	//else
	{
		boost::filesystem::create_directory("Simulink");
		outputDir = outputDir + "\\Simulink";
		currPath = currPath + "\\Simulink";
		boost::filesystem::current_path(currPath);
		printLog("create folder: Simulink");
	}
	
	try
	{		
		if (!master && (param == GME_SILENT_MODE || ((bool)(GetKeyState(VK_CONTROL) & 0x8000))))
		{
			CyberComposition::RootFolder root = CyberComposition::RootFolder::Cast(p_backend->GetRootObject());
		
			set<CyberComposition::Components> comsFdrs = root.Components_kind_children();	
			for (auto it = comsFdrs.begin(); it != comsFdrs.end(); it++)
			{
				set<CyberComposition::SimulinkWrapper> sls = (*it).SimulinkWrapper_kind_children();
				for(auto sl_it=sls.begin();sl_it!=sls.end();sl_it++)
				{
					boost::filesystem::current_path(currPath);
					CyberComposition::SimulinkWrapper sl = *sl_it;
					std::string info = std::string(sl.name())+"...";
					printLog("==============================================================================");
					printLog("process SimulinkWrapper: "+(std::string)sl.name()+" ("+(std::string)sl.getPath2()+"):");
					processSimulinkWrapper(sl, false);
				}
			}
		
			
			//prgDlg.OnFinished();

			boost::filesystem::current_path( orig_path );
			closeLogFile();
			return;
		}
		
		if(!selectedObjects.empty() && !master)
		{
			std::string info;
			CyberComposition::SimulinkWrapper slmodel;
			for(std::set<Udm::Object>::iterator it=selectedObjects.begin();it!=selectedObjects.end();++it)
			{
				boost::filesystem::current_path(currPath);
				Udm::Object selectObj = *it;
				if(Uml::IsDerivedFrom(selectObj.type(), CyberComposition::SimulinkWrapper::meta))
				{
					slmodel = CyberComposition::SimulinkWrapper::Cast(selectObj);
					std::string sfname = slmodel.name();
					boost::filesystem::create_directory(sfname);
					
					std::string currDir = currPath + "\\" + sfname;
					boost::filesystem::current_path(currDir);
					info = "run SF_CodeGen for SimulinkWrapper model: "+sfname+"...";

					closeLogFile();
					runCyberSF_CodeGen_dllimported(slmodel, (LPCTSTR)mgaPath, CyberComposition::meta, CUdmApp::logFile, currDir);
					
					openLogFile(CUdmApp::logFile, true);
					info = "run SL_CodeGen for SimulinkWrapper Model: "+sfname+"...";
				//	prgDlg.SetProgress(info.c_str());
					runCyberSL_CodeGen(slmodel, (LPCTSTR)mgaPath, p_backend, currDir);
				
					std::string log = "SL_Code has been generated for the signalflow model: "+sfname;
					//AfxMessageBox(log.c_str(),MB_ICONINFORMATION);
					GMEConsole::Console::writeLine(log, MSG_INFO);
				}
			}
			if(slmodel==Udm::null)
				showUsage();
		}
		else if(Uml::IsDerivedFrom(focusObject.type(), CyberComposition::SimulinkWrapper::meta))
		{
			CyberComposition::SimulinkWrapper slmodel = CyberComposition::SimulinkWrapper::Cast(focusObject);
			std::string sfname = slmodel.name();

			printLog( "Processing SimulinkWrapper model: " + sfname + "..." );
			boost::filesystem::create_directory(sfname);
	
			std::string currDir = currPath;
			boost::filesystem::current_path(currDir);

			bool pswresult = processSimulinkWrapper(slmodel, true);
	
			if( pswresult )
			{
				std::string logmsg = "SL_Code has been generated for the SimulinkWrapper model: "+(std::string)slmodel.name();
				//GMEConsole::Console::writeLine(log, MSG_INFO);
				printLog( logmsg );

			}
			else // There were errors
			{
				string outmsg = "Errors processing cyber component " + string(slmodel.name()) + ".";
				outmsg += " See <a href=\" " + logFile + "\">log file</a> for details.";
				boost::filesystem::current_path( orig_path );
				std::exception exc( outmsg.c_str() );
				throw exc;
				//ThrowComError(E_FAIL, _bstr_t(outmsg.c_str()));
			}
		}
		else
		{
			showUsage();
			boost::filesystem::current_path( orig_path );
			return;
		}

		boost::filesystem::current_path( orig_path );
	}
	catch(udm_exception &exc)
	{
		printLog(exc.what());
		boost::filesystem::current_path( orig_path );
		closeLogFile();
		GMEConsole::Console::Error::writeLine(exc.what());
		ThrowComError(E_FAIL, _bstr_t(exc.what()));
	}
	catch(exception &e)
	{
		printLog(e.what());
		boost::filesystem::current_path( orig_path );
		closeLogFile();
		ThrowComError(E_FAIL, _bstr_t(e.what()));
	}
	catch(const boost::python::error_already_set& )
	{
		std::string error = GetPythonError();
		printLog(error);
		OutputDebugStringA(error.c_str());
		cerr << error << endl;
		GMEConsole::Console::Error::writeLine(error);
		boost::filesystem::current_path( orig_path );
		// do we need to close the log? no, this exception is recoverable
		// do we want to return a failure? no
	}
	catch(...)
	{
		printLog("Exception throw.");
		boost::filesystem::current_path( orig_path );
		closeLogFile();
			throw;
	}	
	closeLogFile();
}