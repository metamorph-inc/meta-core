#include "ModelicaCodeGen.h"
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp> 

#include "runSL_CodeGen.h"

#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <set>

#include <UmlExt.h>

#include <ctemplate\template.h>
#include "Rpc.h"
#include "CommandExecutor.h"
#include "ModelicaTemplate.h"

#include "CGLog.h"

#include "utils.h"


#include <cstdlib>

MoCodeGen::MoCodeGen(CyberComposition::SimulinkWrapper &slmodel, const std::string &outputDirectory, const std::string logFile)
	:_sl(slmodel), _directory(outputDirectory), _logFileName(logFile)
{
	_slName = _sl.name();
	_wrapperName = _slName + "_wrapper";
}

void MoCodeGen::setPackageName(const std::string &pkg)
{
	_pkgName = pkg;
}

void MoCodeGen::gen()
{
	if(_sl == Udm::null)
		return;
	
	SFUtils::obj2VarNameMap.clear();


	//if(((std::string)_sl.Class()).empty())
	//{
	//	_sl.Class() = string("SignalFlow.")+_slName+string("_type");
	//}

	set<CyberComposition::Simulink::Subsystem> subs  = _sl.Simulink_Subsystem_kind_children();
		
	if(subs.empty()) return;
		
	CyberComposition::Simulink::Subsystem topsub = *(subs.begin());


	//try {
		SFUtils::TopLevelTopologicalSort( topsub );
	//}
	//catch ( udm_exception & err )
	//{
	//	printLog( err.what() );
		//ostringstream oss;
		//oss << "Subsystem " << topsub.name() << " failed the cycle check. See the code generator log file for details.";
		//throw err;
	//}

	_topSubSystems.push_back(topsub);


	set<CyberComposition::Simulink::InputPort, InputPortSorter> inports = topsub.InputPort_kind_children_sorted(InputPortSorter());
	for(set<CyberComposition::Simulink::InputPort, InputPortSorter>::iterator it_in=inports.begin();it_in!=inports.end();++it_in)
	{
		CyberComposition::Simulink::InputPort inport = *it_in;
		_inputs.push_back(inport);
		CyberComposition::Simulink::TypeBaseRef inref = inport.TypeBaseRef_child();
		_inputMap[inport] = inref;
	}

	set<CyberComposition::Simulink::OutputPort, OutputPortSorter> outports = topsub.OutputPort_kind_children_sorted(OutputPortSorter());
	for(set<CyberComposition::Simulink::OutputPort, OutputPortSorter>::iterator it_out=outports.begin();it_out!=outports.end();++it_out)
	{
		CyberComposition::Simulink::OutputPort outport = *it_out;
		_outputs.push_back(outport);
		CyberComposition::Simulink::TypeBaseRef outref = outport.TypeBaseRef_child();
		_outputMap[outport] = outref;
	}

	set<CyberComposition::ParameterRef> paramRefs = _sl.ParameterRef_kind_children();
	for(auto it_ref=paramRefs.begin(); it_ref!=paramRefs.end(); ++it_ref)
	{
		CyberComposition::ParameterRef pref = *it_ref;
		std::string ref_name = pref.name();
		std::string data_type;
		CyberComposition::Simulink::TypeBaseRef typeref;
		CyberComposition::ParameterBase pbase = pref.ref();

		if(pbase==Udm::null) 
		{
			data_type = pref.Class();
		}
		else if(pbase.type()==CyberComposition::Simulink::SFData::meta)
		{
			CyberComposition::Simulink::SFData sfdata = CyberComposition::Simulink::SFData::Cast(pbase);
			data_type = sfdata.DataType();
			if(data_type.empty())
				typeref = sfdata.TypeBaseRef_child();
		}
		else if(pbase.type()==CyberComposition::Simulink::Data::meta)
		{
			CyberComposition::Simulink::Data data = CyberComposition::Simulink::Data::Cast(pbase);
			data_type = data.DataType();
			if(data_type.empty())
				typeref = data.TypeBaseRef_child();
		}
		else if(pbase.type()==CyberComposition::SignalFlow::ParameterDataPort::meta)
		{
			CyberComposition::SignalFlow::ParameterDataPort pdataport = CyberComposition::SignalFlow::ParameterDataPort::Cast(pbase);
			data_type = pdataport.DataType();
		}
		else //SF_Parameter, no DataType attribute
			data_type = "double";

		if(data_type.empty() && typeref!=Udm::null)
		{
			CyberComposition::Simulink::SF_TypeBase tbase = typeref.ref();
			data_type = tbase.name();
		}
		std::string gen_datatype;
		if (data_type=="Float" || data_type == "Real")
			gen_datatype = "double";
		else if (data_type=="Integer")
			gen_datatype = "long";
		else if (data_type=="Boolean")
			gen_datatype = "boolean";
		else
			gen_datatype = "double";

		if(pbase == Udm::null || pbase.type()==CyberComposition::Simulink::SF_Parameter::meta)
			_parameterRefMap[pref] = gen_datatype;
		else
			_sfdataRefMap[pref] = gen_datatype;
	}

	ComponentCodeGen cg(_sl, _directory, "");
	cg.runCodeGen(_logFileName);
	if(cg.getError())
	{
		printLog(">>>>>>>>>>>>>>>>No modelica files generated due to the SL_CodeGen error.<<<<<<<<<<<<<<<<<<<<<<");
		return;
	}

	_hfiles = cg.getGenerated_H_Files();
	_cfiles = cg.getGenerated_C_Files();

	if(_topSubSystems.empty()) return;

	_topSubSystemName = (std::string)((*(_topSubSystems.begin())).name());
	_contextName = _topSubSystemName + "_context";

	printLog("generated Modelica files: ");
	genWrapper_source();
	genWrapper_proj();
	printLog("generated Modelica Package files: ");
	genModelica();
	genPackageMo();
}

//std::string MoCodeGen::getDataVarFullPath(const CyPhyML::SF_DataParameterBase &obj)
//{
//	std::string ret = obj.name();
//	Udm::Object subParent;
//	if(Uml::IsDerivedFrom(obj.type(), CyberComposition::Simulink::Data::meta))
//	{
//		CyberComposition::Simulink::Data data = CyberComposition::Simulink::Data::Cast(obj);
//		CyberComposition::Simulink::State state = data.State_parent();
//		ret = (std::string)state.name()+"_"+ret;
//		subParent  = state.GetParent();
//	}
//	else
//	{
//		CyberComposition::Simulink::SFData sfdata = CyberComposition::Simulink::SFData::Cast(obj);
//		CyberComposition::Simulink::SFState sfstate = sfdata.SFState_parent();
//		ret = (std::string)sfstate.name()+"_"+ret;
//		subParent  = sfstate.GetParent();
//	}
//
//	while(Uml::IsDerivedFrom(subParent.type(), CyberComposition::Simulink::Subsystem::meta))
//	{
//		map<int, std::string>::iterator pos = SFUtils::obj2VarNameMap.find(subParent.uniqueId());
//		if(pos!=SFUtils::obj2VarNameMap.end())
//			ret = (*pos).second+"."+ret;
//		else
//		{
//			if(subParent.isInstance())
//			{
//				pos = SFUtils::obj2VarNameMap.find(subParent.archetype().uniqueId());
//				if(pos!=SFUtils::obj2VarNameMap.end())
//					ret = (*pos).second+"."+ret;
//				else
//					return ret;
//			}
//			else
//				return ret;
//		}
//		subParent = subParent.GetParent();
//		std::string subname  = subParent.getPath2();
//	}
//
//	return ret;
//}

std::string MoCodeGen::getParaVarFullPath(const CyberComposition::ParameterBase &param)
{
	std::string ret;
	Udm::Object subParent;
	if(param.type()==CyberComposition::Simulink::SF_Parameter::meta)
	{
		map<int, std::string>::iterator pos = SFUtils::obj2VarNameMap.find(param.uniqueId());
		if(pos!=SFUtils::obj2VarNameMap.end())
			ret = (*pos).second;
		else
		{
			if(param.isInstance())
			{
				pos = SFUtils::obj2VarNameMap.find(param.archetype().uniqueId());
				if(pos!=SFUtils::obj2VarNameMap.end())
					ret = (*pos).second;
				else
					return ret;
			}
			else
				return ret;
		}
		Udm::Object block = param.GetParent();		
		subParent  = block.GetParent();
	}
	else if(param.type()==CyberComposition::Simulink::Data::meta)
	{
		ret = param.name();
		CyberComposition::Simulink::Data data = CyberComposition::Simulink::Data::Cast(param);
		CyberComposition::Simulink::State state = data.State_parent();
		ret = (std::string)state.name()+"_"+ret;

		// Note that State may be down inside a hierarchy -- we need to keep going up until we hit a subsystem
		Udm::Object tmpParent = state.GetParent();
		while ( !Uml::IsDerivedFrom( tmpParent.type(), CyberComposition::Simulink::Subsystem::meta ) )
		{
			Udm::Object tmpParent2 = tmpParent.GetParent();
			tmpParent = tmpParent2;
		}
		subParent  = tmpParent;
	}
	else if(param.type()==CyberComposition::Simulink::SFData::meta)
	{
		ret = param.name();
		CyberComposition::Simulink::SFData sfdata = CyberComposition::Simulink::SFData::Cast(param);
		CyberComposition::Simulink::SFState sfstate = sfdata.SFState_parent();
		ret = (std::string)sfstate.name()+"_"+ret;

		// Note that SFState may be down inside a hierarchy -- we need to keep going up until we hit a subsystem
		Udm::Object tmpParent = sfstate.GetParent();
		while ( !Uml::IsDerivedFrom( tmpParent.type(), CyberComposition::Simulink::Subsystem::meta) )
		{
			Udm::Object tmpParent2 = tmpParent.GetParent();
			tmpParent = tmpParent2;
		}
		subParent  = tmpParent;
	}
		
	while(Uml::IsDerivedFrom(subParent.type(), CyberComposition::Simulink::Subsystem::meta))
	{
		map<int, std::string>::iterator pos = SFUtils::obj2VarNameMap.find(subParent.uniqueId());
		if(pos!=SFUtils::obj2VarNameMap.end())
			ret = (*pos).second+"."+ret;
		else
		{
			if(subParent.isInstance())
			{
				pos = SFUtils::obj2VarNameMap.find(subParent.archetype().uniqueId());
				if(pos!=SFUtils::obj2VarNameMap.end())
					ret = (*pos).second+"."+ret;
				else
					return ret;
			}
			else
				return ret;
		}
		subParent = subParent.GetParent();
		std::string subname  = subParent.getPath2();
	}

	return ret;
}

void MoCodeGen::genWrapper_source()
{
	ctemplate::TemplateDictionary srcdict( "source" );
	srcdict.SetValue("COMPONENT_NAME", _slName);
	srcdict.SetValue("TOPSUBSYTEM_NAME", _topSubSystemName);
	std::string args, vars;
	for(list<CyberComposition::Simulink::InputPort>::iterator it_in=_inputs.begin();it_in!=_inputs.end();++it_in)
	{
		CyberComposition::Simulink::InputPort in = *it_in;
		map<CyberComposition::Simulink::InputPort, CyberComposition::Simulink::TypeBaseRef>::iterator pos = _inputMap.find(in);
		if(pos!=_inputMap.end())
		{
			CyberComposition::Simulink::TypeBaseRef tref = (*pos).second;
			ctemplate::TemplateDictionary* inDict = srcdict.AddSectionDictionary("INPUT_SIGNAL");
			inDict->SetValue( "SIGNAL_TYPE", (std::string)tref.name());
			inDict->SetValue( "SIGNAL_NAME", (std::string)in.name());
		}
	}
	for(list<CyberComposition::Simulink::OutputPort>::iterator it_out=_outputs.begin();it_out!=_outputs.end();++it_out)
	{
		CyberComposition::Simulink::OutputPort out = *it_out;
		map<CyberComposition::Simulink::OutputPort, CyberComposition::Simulink::TypeBaseRef>::iterator pos = _outputMap.find(out);
		if(pos!=_outputMap.end())
		{
			CyberComposition::Simulink::TypeBaseRef tref = (*pos).second;
			ctemplate::TemplateDictionary* inDict = srcdict.AddSectionDictionary("OUTPUT_SIGNAL");
			inDict->SetValue( "SIGNAL_TYPE", (std::string)tref.name());
			inDict->SetValue( "SIGNAL_NAME", (std::string)out.name());
		}
	}

	if(!_parameterRefMap.empty())
	{
		map<CyberComposition::ParameterRef, std::string >::iterator pos=_parameterRefMap.begin();
		while(pos!=_parameterRefMap.end())
		{
			CyberComposition::ParameterRef pref = (*pos).first;
			std::string sftype = (*pos).second;			
			
			CyberComposition::ParameterBase pbase = pref.ref();
			if(!pbase || pbase.type()!=CyberComposition::Simulink::SF_Parameter::meta)
			{ 
				pos++;
				continue;
			}
			
			std::string refname_base = pref.name();
			std::string refname = refname_base;
			
			int val_size = 1;
			CyberComposition::Simulink::SF_Parameter refObj = CyberComposition::Simulink::SF_Parameter::Cast(pbase);
			val_size = getValueArraySize(refObj);
	
			if(val_size>1)
			{
				char tmp[20];
				_itoa(val_size, tmp, 10);
				refname = refname_base+"["+(std::string)tmp+"]";
			}

			if(pos==_parameterRefMap.begin())
			{
				srcdict.SetValue("SINGLE_VAR_TYPE", sftype);
				srcdict.SetValue("SINGLE_VAR_NAME", refname);
			}
			else
			{				
				ctemplate::TemplateDictionary* argDict = srcdict.AddSectionDictionary("INPUT_ARG");
				argDict->SetValue( "VAR_TYPE", sftype);
				argDict->SetValue( "VAR_NAME", refname);
			}
			
			if(val_size>1)
			{
				for(int i=0;i<val_size;i++)
				{
					char tmp[20];
					_itoa(i, tmp, 10);
					refname = refname_base+"["+(std::string)tmp+"]";
					std::string objPath = getParaVarFullPath(refObj);
					objPath = objPath+"["+(std::string)tmp+"]";
					ctemplate::TemplateDictionary* paramDict = srcdict.AddSectionDictionary("PARAM_INIT");
					paramDict->SetValue("VAR_NAME", refname);
					paramDict->SetValue("PARAM_NAME", objPath);
				}
			}
			else
			{
				ctemplate::TemplateDictionary* paramDict = srcdict.AddSectionDictionary("PARAM_INIT");
				paramDict->SetValue("VAR_NAME", refname);
				paramDict->SetValue("PARAM_NAME", getParaVarFullPath(refObj));
			}
			pos++;
		}
	}

	if(!_sfdataRefMap.empty())
	{		
		map<CyberComposition::ParameterRef, std::string >::iterator pos=_sfdataRefMap.begin();
	
		while(pos!=_sfdataRefMap.end())
		{
			CyberComposition::ParameterRef sfdref = (*pos).first;
			CyberComposition::ParameterBase refObj = sfdref.ref();
			if(!refObj || refObj.type()==CyberComposition::Simulink::SF_Parameter::meta)
			{
				pos++;
				continue;
			}
			
			std::string refname = sfdref.name();

			if(pos==_sfdataRefMap.begin() && _parameterRefMap.empty())
			{
				srcdict.SetValue("SINGLE_VAR_TYPE", (*pos).second);
				srcdict.SetValue("SINGLE_VAR_NAME",  refname);
			}		
			else
			{
				ctemplate::TemplateDictionary* argDict = srcdict.AddSectionDictionary("INPUT_ARG");
				argDict->SetValue("VAR_TYPE", (*pos).second);
				argDict->SetValue("VAR_NAME", refname);
			}

			ctemplate::TemplateDictionary* paramDict = srcdict.AddSectionDictionary("PARAM_INIT");
			paramDict->SetValue("VAR_NAME", refname);
			paramDict->SetValue("PARAM_NAME", getParaVarFullPath(refObj));
			pos++;
		}	
	}

	std::string output;
	//h file
	boost::filesystem::current_path(_directory);
	std::string hFileName = _wrapperName +".h";
	ctemplate::StringToTemplateCache("htpl", Modelica_Template::get_h_tpl(), ctemplate::DO_NOT_STRIP);
	ctemplate::ExpandTemplate("htpl", ctemplate::DO_NOT_STRIP, &srcdict, &output);
	ofstream h_file(hFileName.c_str() );
	h_file << output ;
	h_file.close();
	_hfiles.insert(_directory+"\\"+hFileName);

	printLog("\t\t"+hFileName);
	
	output.clear();

	//c file
	boost::filesystem::current_path(_directory+"\\"+_slName);
	std::string cFileName = _wrapperName +".c";
	ctemplate::StringToTemplateCache("ctpl", Modelica_Template::get_c_tpl(), ctemplate::DO_NOT_STRIP);
	ctemplate::ExpandTemplate("ctpl", ctemplate::DO_NOT_STRIP, &srcdict, &output);
	ofstream c_file(cFileName.c_str() );
	c_file << output;
	c_file.close();
	_cfiles.insert(cFileName);

	printLog("\t\t"+cFileName);
}

void MoCodeGen::genWrapper_proj()
{
	// Initialize the template system
	ctemplate::TemplateDictionary dict( "vcxproj" );
	
	dict.SetValue("COMPONENT_NAME", _slName);
	dict.SetValue("SOLUTION_ID", generateGUID());
	dict.SetValue("PROJECT_ID", generateGUID());
	dict.SetValue("SOURCE_ID", generateGUID());
	dict.SetValue("HEADER_ID", generateGUID());

	std::string include_path="..\\";
	dict.SetValue("INCLUDE_PATH", include_path);

	for(set<std::string>::iterator it_h=_hfiles.begin();it_h!=_hfiles.end();++it_h)
	{
		ctemplate::TemplateDictionary* inDict = dict.AddSectionDictionary("INCLUDE");	
		inDict->SetValue("INCLUDE_H", *it_h);
	}

	for(set<std::string>::iterator it_c=_cfiles.begin();it_c!=_cfiles.end();++it_c)
	{
		ctemplate::TemplateDictionary* cDict = dict.AddSectionDictionary("COMPILE");				
		cDict->SetValue("COMPILE_C", *it_c);
	}

	std::string output;

	boost::filesystem::current_path(_directory+"\\"+_slName);
	//com.vcxproj
	std::string projFileName = _slName +".vcxproj";
	ctemplate::StringToTemplateCache("projtpl", Modelica_Template::get_vcxproj_tpl(), ctemplate::DO_NOT_STRIP);
    ctemplate::ExpandTemplate("projtpl", ctemplate::DO_NOT_STRIP, &dict, &output);
	ofstream projFile(projFileName.c_str() );
	// Write the generated code out to the file and close
	projFile << output;
	projFile.close();
	
	printLog("\t\t"+projFileName);

	output.clear();
	
	//com..vcxproj.filters
	std::string filterFileName = _slName +".vcxproj.filters";
	ctemplate::StringToTemplateCache("filtertpl", Modelica_Template::get_filter_tpl(), ctemplate::DO_NOT_STRIP);
    ctemplate::ExpandTemplate("filtertpl", ctemplate::DO_NOT_STRIP, &dict, &output);
	ofstream filterFile(filterFileName.c_str());
	filterFile << output;
	filterFile.close();

	printLog("\t\t"+filterFileName);

	output.clear();

	//com.sln
	std::string slnFileName = _slName +".sln";	
	ctemplate::StringToTemplateCache("slntpl", Modelica_Template::get_sln_tpl(), ctemplate::DO_NOT_STRIP);
    ctemplate::ExpandTemplate("slntpl", ctemplate::DO_NOT_STRIP, &dict, &output);
	ofstream slnFile(slnFileName.c_str());
	slnFile << output;
	slnFile.close();

	printLog("\t\t"+slnFileName);

	//compile project file
        char * vs10ToolsEnv = std::getenv("VS100COMNTOOLS");
	std::string vs10Tools;
        if ( vs10ToolsEnv ) { vs10Tools = string( vs10ToolsEnv ); }
	std::string devexe = "devenv.exe";
	bool vsInstalled = false;
	if(!vs10Tools.empty())
	{
		if(!boost::filesystem::exists( vs10Tools +"\\..\\IDE\\"+devexe ))
		{
			devexe =  "VCExpress.exe";
			if(boost::filesystem::exists( vs10Tools +"\\..\\IDE\\"+devexe ))
				vsInstalled = true;
		}
		else
			vsInstalled = true;
	}

	CommandExecutor cmdExec;
	cmdExec.printMsg("Build "+_slName+" static library:", MSG_INFO);

	printLog("Build "+_slName+" static library");

	std::string cmdline =  devexe +" "+ slnFileName +" /build Release /project "+projFileName+" /projectconfig Release";
	std::string makeFile_name = "make_"+_slName+".bat";
	ofstream makeFile(makeFile_name.c_str());
	makeFile << "call \"%VS100COMNTOOLS%vsvars32.bat\"\n";
	makeFile << "\"%DevEnvDir%"+devexe+"\" "+ slnFileName +" /build Release /project "+projFileName+" /projectconfig Release\n";
	makeFile.close();
	
	if(vsInstalled)
	{
		if(!cmdExec.execute(makeFile_name))
		{
			printLog(_slName+".lib fails.");
			throw udm_exception(cmdline+" failed");
		}
		std::string status = _slName+".lib ---> generated successfully.";
		cmdExec.printMsg(status);
		printLog("generated library: ..\\Release\\"+ _slName+".lib");
	}
	else
		cmdExec.printMsg("Visual Studio 2010 or Visual Studio 2010 Express is not installed."); 
	
//	if(!boost::filesystem::remove("make.bat"))
//		throw exception("boost::filesystem::remove(\"make.bat\") fails");
}

void MoCodeGen::genModelica()
{	
	boost::filesystem::current_path(_directory);
	ctemplate::TemplateDictionary modict( "modelica" );
	modict.SetValue("COMPONENT_NAME", _slName);
	set< string > preventDuplicateNames;
	
//	set<CyPhyML::AggregatePort> aports;
//	set<CyPhyML::SignalFlowBusPortInterface> busports;
	//set<CyPhyML::SignalFlowBusPortInterface> busportInterfaceSet;
	for(list<CyberComposition::Simulink::InputPort>::iterator it_in=_inputs.begin();it_in!=_inputs.end();++it_in)
	{
		CyberComposition::Simulink::InputPort in = *it_in;
		map<CyberComposition::Simulink::InputPort, CyberComposition::Simulink::TypeBaseRef>::iterator pos = _inputMap.find(in);
		if(pos!=_inputMap.end())
		{
			set<CyberComposition::InputSignalInterfaceConnection> iosignal2ins = in.srcInputSignalInterfaceConnection();
		//	set<CyPhyML::IOSignal2InPort> iosignal2ins = in.srcIOSignal2InPort();
			if(iosignal2ins.empty())
				continue;
			CyberComposition::InputSignalInterfaceConnection iosignal2in = *(iosignal2ins.begin());
			//CyPhyML::InSignal insignal = iosignal2in.srcIOSignal2InPort_end();
			CyberComposition::InputSignalInterface insignal = iosignal2in.srcInputSignalInterfaceConnection_end();

			CyberComposition::Simulink::TypeBaseRef tref = (*pos).second;

			// duplicate connections cause problems when we don't keep track of the inner and outer names
			std::string inner_name = in.name();
			std::string outer_name = insignal.name();
			ctemplate::TemplateDictionary* inDict = modict.AddSectionDictionary("INPUT_SIGNAL");	
			inDict->SetValue("DATA_TYPE", convertMoType((std::string)tref.name()));
			inDict->SetValue("DATA_NAME", inner_name);

			std::string in_arg_name;
			//check the parent of the insignal
			Udm::Object signal_parent = insignal.GetParent();
			//if(Uml::IsDerivedFrom(signal_parent.type(), CyPhyML::SignalFlowBusPortInterface::meta))
			if(Uml::IsDerivedFrom(signal_parent.type(), CyberComposition::BusInterface::meta))
			{
				CyberComposition::BusInterface bus = CyberComposition::BusInterface::Cast(signal_parent);
				//CyPhyML::SignalFlowBusPortInterface sfbpi = CyPhyML::SignalFlowBusPortInterface::Cast(signal_parent);
			/*	CyPhyML::BusPort bp = getBusPort(sfbpi);
				if(!bp)
					continue;
				set<CyPhyML::SignalFlowBusPortInterface>::iterator set_it = busportInterfaceSet.find(sfbpi);
				if(set_it==busportInterfaceSet.end())*/
				{
					//busportInterfaceSet.insert(sfbpi);
					string busName = bus.name();

					if ( preventDuplicateNames.find(busName) == preventDuplicateNames.end() )
					{
						ctemplate::TemplateDictionary* portDefDict = modict.AddSectionDictionary("BUSPORT_DEF");	
						portDefDict->SetValue("PORT_TYPE", (std::string)bus.Class());
						portDefDict->SetValue("PORT_NAME", (std::string)bus.name());
						preventDuplicateNames.insert(busName);
					}
				}
				in_arg_name = (std::string)bus.name()+"."+outer_name;
			}
			else
			{
				CyberComposition::Simulink::TypeBaseRef tref = (*pos).second;
				//std::string in_name = insignal.name();
				ctemplate::TemplateDictionary* indefDict = modict.AddSectionDictionary("INPUT_SIGNAL_DEF");	
				indefDict->SetValue("DATA_TYPE", convertMoType((std::string)tref.name()));
				indefDict->SetValue("DATA_NAME", outer_name);
				in_arg_name = outer_name;
			}
			ctemplate::TemplateDictionary* inargDict = modict.AddSectionDictionary("INPUT_ARG");	
			inargDict->SetValue("DATA_NAME", in_arg_name);
		}
	}

	if(_outputs.size()>1)
	{
		modict.ShowSection("LEFT");
		modict.ShowSection("RIGHT");
	}

	for(list<CyberComposition::Simulink::OutputPort>::iterator it_out=_outputs.begin();it_out!=_outputs.end();++it_out)
	{
		CyberComposition::Simulink::OutputPort out = *it_out;
		map<CyberComposition::Simulink::OutputPort, CyberComposition::Simulink::TypeBaseRef>::iterator pos = _outputMap.find(out);
		if(pos!=_outputMap.end())
		{
			set<CyberComposition::OutputSignalInterfaceConnection> out2iosignals = out.dstOutputSignalInterfaceConnection();
		//	set<CyPhyML::OutPort2IOSignal> out2iosignals = out.dstOutPort2IOSignal();
			if(out2iosignals.empty())
				continue;

			//CyPhyML::OutPort2IOSignal out2iosignal = *(out2iosignals.begin());
			CyberComposition::OutputSignalInterfaceConnection out2iosignal = *(out2iosignals.begin()); 
			//CyPhyML::OutSignal outsignal = out2iosignal.dstOutPort2IOSignal_end();
			CyberComposition::OutputSignalInterface outsignal = out2iosignal.dstOutputSignalInterfaceConnection_end();

			CyberComposition::Simulink::TypeBaseRef tref = (*pos).second;
			//std::string out_name = out.name();
			std::string inner_name = out.name();
			std::string outer_name = outsignal.name();
			ctemplate::TemplateDictionary* outDict = modict.AddSectionDictionary("OUTPUT_SIGNAL");
			outDict->SetValue( "DATA_TYPE", convertMoType((std::string)tref.name()));
			outDict->SetValue( "DATA_NAME", inner_name);
			
			std::string out_arg_name;
			Udm::Object signal_parent = outsignal.GetParent();
			//if(Uml::IsDerivedFrom(signal_parent.type(), CyPhyML::SignalFlowBusPortInterface::meta))
			if(Uml::IsDerivedFrom(signal_parent.type(), CyberComposition::BusInterface::meta))
			{	
				//CyPhyML::SignalFlowBusPortInterface sfbpi = CyPhyML::SignalFlowBusPortInterface::Cast(signal_parent);
				CyberComposition::BusInterface bus = CyberComposition::BusInterface::Cast(signal_parent);
				//CyPhyML::BusPort bp = getBusPort(sfbpi);
				//if(!bp)
				//	continue;
				//set<CyPhyML::SignalFlowBusPortInterface>::iterator set_it = busportInterfaceSet.find(sfbpi);
				//if(set_it==busportInterfaceSet.end())
				{
			//		busportInterfaceSet.insert(sfbpi);
					string busName = bus.name();
					if ( preventDuplicateNames.find(busName) == preventDuplicateNames.end() )
					{
							ctemplate::TemplateDictionary* portDefDict = modict.AddSectionDictionary("BUSPORT_DEF");	
							portDefDict->SetValue("PORT_TYPE", (std::string)bus.Class());
							portDefDict->SetValue("PORT_NAME", (std::string)bus.name());
							preventDuplicateNames.insert(busName);
					}
				}
				out_arg_name = (std::string)bus.name()+"."+outer_name;
			}
			else
			{
				ctemplate::TemplateDictionary* outdefDict = modict.AddSectionDictionary("OUTPUT_SIGNAL_DEF");
				outdefDict->SetValue( "DATA_TYPE", convertMoType((std::string)tref.name()));
				outdefDict->SetValue( "DATA_NAME", outer_name);
				ctemplate::TemplateDictionary* outDict_p = modict.AddSectionDictionary("OUTPUT_SIGNAL_P");
				outDict_p->SetValue( "DATA_TYPE", convertMoType((std::string)tref.name()));
				outDict_p->SetValue( "DATA_NAME", outer_name);
				out_arg_name = outer_name;
			}
			if(it_out==_outputs.begin())
				modict.SetValue("FIRST_DATA_NAME", out_arg_name);
			else
			{
				ctemplate::TemplateDictionary* outDict_a = modict.AddSectionDictionary("OUTPUT_SIGNAL_A");
				outDict_a->SetValue( "DATA_NAME", out_arg_name);
			}
		}
	}

	//for SF_ParameterRef
	string sample_period_name;
	if(!_parameterRefMap.empty())
	{
		map<CyberComposition::ParameterRef, std::string >::iterator pos=_parameterRefMap.begin();
		CyberComposition::ParameterRef sfpref = (*pos).first;
		std::string refname_base = sfpref.name();
	
		std::string refname = refname_base;
		modict.SetValue("SINGLE_VAR", refname_base);
	
		while(pos!=_parameterRefMap.end())
		{
			sfpref = (*pos).first;
			refname_base = sfpref.name();

			string lcname = refname_base;
			std::transform(lcname.begin(), lcname.end(), lcname.begin(), ::tolower);

			if ( lcname == "sampleperiod" || lcname == "sample_period" || lcname == "ts" )
			{
				sample_period_name = refname_base;
			}

			CyberComposition::ParameterBase refObj = sfpref.ref();		
			//check the size of array of values
			int val_size = 1;
			if(refObj!=Udm::null && Uml::IsDerivedFrom(refObj.type(), CyberComposition::Simulink::SF_Parameter::meta))
				val_size = getValueArraySize(CyberComposition::Simulink::SF_Parameter::Cast(refObj));
			if(val_size>1)
			{
				char tmp[20];
				_itoa(val_size, tmp, 10);
				refname = refname_base+"["+(std::string)tmp+"]";
			}
			else
				refname = refname_base;

			ctemplate::TemplateDictionary* argDict = modict.AddSectionDictionary("PARAMETER");
			argDict->SetValue( "PARAMETER_TYPE", convertMoType((*pos).second));
			argDict->SetValue( "PARAMETER_NAME", refname);
			
			if(pos!=_parameterRefMap.begin())
			{
				ctemplate::TemplateDictionary* varDict = modict.AddSectionDictionary("VARS");
				varDict->SetValue("VAR_NAME", refname_base);	
			}
			pos++;
		}
	}


	if(!_sfdataRefMap.empty())
	{
		map<CyberComposition::ParameterRef, std::string>::iterator pos=_sfdataRefMap.begin();
		CyberComposition::ParameterRef sfdref = (*pos).first;
		std::string refname = sfdref.name();
		if ( _parameterRefMap.empty() )
			modict.SetValue("SINGLE_VAR", refname);
					
		while(pos!=_sfdataRefMap.end())
		{
			sfdref = (*pos).first;
			refname = sfdref.name();

			string lcname = refname;
			std::transform(lcname.begin(), lcname.end(), lcname.begin(), ::tolower);

			if ( lcname == "sampleperiod" || lcname == "sample_period" || lcname == "ts" )
			{
				sample_period_name = refname;
			}
			ctemplate::TemplateDictionary* argDict = modict.AddSectionDictionary("PARAMETER");
			argDict->SetValue( "PARAMETER_TYPE", convertMoType((*pos).second));
			argDict->SetValue( "PARAMETER_NAME", refname);
			
			if( !_parameterRefMap.empty() || pos!=_sfdataRefMap.begin() )
			{
				ctemplate::TemplateDictionary* varDict = modict.AddSectionDictionary("VARS");
				varDict->SetValue("VAR_NAME", refname);	
			}
			pos++;
		}
	}

	if ( sample_period_name.empty() || sample_period_name.length() == 0 )
	{
			ctemplate::TemplateDictionary* argDict = modict.AddSectionDictionary("PARAMETER");
			argDict->SetValue( "PARAMETER_TYPE", "Real");
			argDict->SetValue( "PARAMETER_NAME", "samplePeriod");
			sample_period_name = "samplePeriod";
	}
	modict.SetValue( "SAMPLE_PERIOD_NAME", sample_period_name );

	std::string output;
	std::string moFileName; // = _slName +".mo";
	//ctemplate::StringToTemplateCache("motpl", Modelica_Template::get_mo_tpl(), ctemplate::DO_NOT_STRIP);
	//ctemplate::ExpandTemplate("motpl", ctemplate::DO_NOT_STRIP, &modict, &output);
	//ofstream moFile(moFileName.c_str() );
	//moFile << output;
	//moFile.close();
	//printLog("\t\t"+moFileName);

	output.clear();
	moFileName = _slName + "_wrapper.mo";
	ctemplate::StringToTemplateCache("mowtpl", Modelica_Template::get_mo_wrapper_tpl(), ctemplate::DO_NOT_STRIP);
    ctemplate::ExpandTemplate("mowtpl", ctemplate::DO_NOT_STRIP, &modict, &output);
	ofstream moWrapperFile(moFileName.c_str() );
	moWrapperFile << output;
	moWrapperFile.close();
	printLog("\t\t"+moFileName);

	output.clear();
	moFileName = _slName + "_wrapper_main.mo";
	ctemplate::StringToTemplateCache("mowmtpl", Modelica_Template::get_mo_wrapper_main_tpl(), ctemplate::DO_NOT_STRIP);
    ctemplate::ExpandTemplate("mowmtpl", ctemplate::DO_NOT_STRIP, &modict, &output);
	ofstream moWrapperMainFile(moFileName.c_str() );
	moWrapperMainFile << output;
	moWrapperMainFile.close();
	printLog("\t\t"+moFileName);

	output.clear();
	moFileName = _slName + "_type.mo";
	ctemplate::StringToTemplateCache("mottpl", Modelica_Template::get_mo_type_tpl(), ctemplate::DO_NOT_STRIP);
    ctemplate::ExpandTemplate("mottpl", ctemplate::DO_NOT_STRIP, &modict, &output);
	ofstream moTypeFile(moFileName.c_str() );
	moTypeFile << output;
	moTypeFile.close();
	printLog("\t\t"+moFileName);

}

//CyPhyML::BusPort MoCodeGen::getBusPort(const CyPhyML::SignalFlowBusPortInterface &busportInterface)
//{
//	map<CyPhyML::SignalFlowBusPortInterface, CyPhyML::BusPort>::iterator pos = _busportMap.find(busportInterface);
//	if(pos!=_busportMap.end())
//		return (*pos).second;
//
//	CyPhyML::BusPort busport;
//	CyPhyML::SignalFlowBusPortMap_Members_Base port_base;
//	set<CyPhyML::SignalFlowBusPortMap> in_flows = busportInterface.srcSignalFlowBusPortMap();
//	if(!in_flows.empty())
//	{
//		port_base = (*(in_flows.begin())).srcSignalFlowBusPortMap_end();
//	}
//	else
//	{
//		set<CyPhyML::SignalFlowBusPortMap> out_flows = busportInterface.dstSignalFlowBusPortMap();
//		if(!out_flows.empty())
//		{
//			port_base = (*(out_flows.begin())).dstSignalFlowBusPortMap_end();
//		}
//		else
//			return busport;
//	}
//				
//	if(Uml::IsDerivedFrom(port_base.type(), CyPhyML::BusPort::meta))
//		busport = CyPhyML::BusPort::Cast(port_base);
//
//	_busportMap[busportInterface] = busport;
//	return busport;
//}
//

void MoCodeGen::genPackageMo()
{
	boost::filesystem::current_path(_directory);
	// Initialize the template system
	ctemplate::TemplateDictionary pkgdict( "pkgmo" );
	pkgdict.SetValue("COMPONENTPKG_NAME", _pkgName);

	std::string output;
	std::string pkgFileName = "package.mo";
	ctemplate::StringToTemplateCache("pkgtpl", Modelica_Template::get_pkg_tpl(), ctemplate::DO_NOT_STRIP);
    ctemplate::ExpandTemplate("pkgtpl", ctemplate::DO_NOT_STRIP, &pkgdict, &output);
	ofstream pkgFile(pkgFileName.c_str() );
	pkgFile << output;
	pkgFile.close();
	printLog("\t\t"+pkgFileName);
}

std::string MoCodeGen::generateGUID()
{
	UUID uuid;     
	UuidCreate ( &uuid );      
	unsigned char * str;     
	UuidToStringA ( &uuid, &str );      
	std::string guid( ( char* ) str );
	guid = "{"+guid+"}";
	RpcStringFreeA ( &str ); 
	return guid;
}

std::string MoCodeGen::convertMoType(const std::string &slType)
{
	if(slType == "double" || slType == "float" || slType == "single")
		return "Real";
	else if(slType == "long" || slType == "int32" || slType == "int16" || slType == "int8" 
			|| slType == "int" || slType == "uint" || slType == "uint32" || slType == "uint16" 
			|| slType== "uint8" )
		return "Integer";
	else if(slType == "boolean")
		return "Boolean";
	else
		return "String";
}

int MoCodeGen::getValueArraySize(const CyberComposition::Simulink::SF_Parameter &sf_parameter)
{
	int ret=0;
	std::string val = sf_parameter.Value();
	std::string s_tokens = "-0123456789.";
	
	size_t s_pos = val.find_first_of(s_tokens);
	size_t e_pos;
	while (s_pos!=string::npos)
	{
		val = val.substr(s_pos, val.length()-s_pos);
		e_pos = val.find_first_not_of(s_tokens);
		if(e_pos==string::npos)
			break;
		ret++;
		val = val.substr(e_pos+1, val.length()-e_pos-1);
		s_pos = val.find_first_of(s_tokens);
	}

	return ret;
}
