
#include <fstream>
#include <algorithm>
#include <list>
#include <direct.h>
#include <errno.h>
#include "Uml.h"
#include "PF_CodeGen.h"
#include "CTypeMap.hpp"

#include "EDPCG.h"
#include "GenBuilder.h"

#include "EsMoL.h"

using namespace std;
using namespace EsMoL;

static bool MakeNode(const Node& node);
static void MakeMain(ofstream& ofs, const Node& node);
static void MakeTSK(ofstream &ttp_build, const Task& tsk, const string& node_nm, 
					stringstream& init_lines, stringstream& global_lines, stringstream& main_lines);

string GetSLPortType( const IOPort& iop )
{
	string dtype("float");

	if ( iop.type() == InputPort::meta )
	{
		const InputPort &ip = InputPort::Cast( iop );
		const set<TypeBaseRef> & tbr_s = ip.TypeBaseRef_kind_children();
		if ( tbr_s.size() > 0 )
		{
			const TypeBaseRef & tbr = *(tbr_s.begin());
			const TypeBase & tb = tbr.ref();
			if (tb)
			{
				if (tb.type() == Matrix::meta) {
					const Matrix& m = Matrix::Cast(tb);
					dtype = CTypeMap::getTypeMap().find( ( m.Type() ) )->second;
				}
			}
		}
	}
	else if ( iop.type() == OutputPort::meta )
	{
		const OutputPort &op = OutputPort::Cast( iop );
		const set<TypeBaseRef> & tbr_s = op.TypeBaseRef_kind_children();
		if ( tbr_s.size() > 0 )
		{
			const TypeBaseRef & tbr = *(tbr_s.begin());
			const TypeBase & tb = tbr.ref();
			if (tb)
			{
				if (tb.type() == Matrix::meta) {
					const Matrix& m = Matrix::Cast(tb);
					dtype = CTypeMap::getTypeMap().find( ( m.Type() ) )->second;
				}
			}
		}
	}
	return dtype;
}




bool MakeAll(const DesignFolder& rootFolder)
{
	bool ret = true;
	
	BEGIN_ITERATE_C(DesignFolder, rootFolder) {
		BEGIN_ITERATE_C(Deployment, DesignFolder_el) {
			if (((const string&)Deployment_el.name()).find("TTPC") != std::string::npos ) {

				string appName((const string&)Deployment_el.name());
				GenBuilder bldr(appName);
				
				BEGIN_ITERATE_C(NodeRef, Deployment_el) {
					ret = MakeNode( NodeRef_el, bldr ) && ret;
				} END_ITERATE_C();		
				
				break;	// Only one (the first) TTPC hardware model is processed
			}
		} END_ITERATE_C();
	} END_ITERATE_C();
	return ret;
}

// Process a Node element
bool MakeNode(const NodeRef& node, GenBuilder & genbuilder)
{
	Node n = node.ref();
	genbuilder.AddAppNode( n );

	//stringstream	global_lines(stringstream::out);
	//stringstream	init_lines(stringstream::out);
	//stringstream	main_lines(stringstream::out);

	//try {
		// create main for the processing node
		//MakeMain(ofs, node);

	std::set< TaskAssignment > tas = node.srcTaskAssignment();
	BEGIN_ITERATE_CSET(TaskAssignment, tas) {
		//MakeTSK(ttp_build, Task_el, node_nm, init_lines, 
		//	global_lines, main_lines, ncfg);
		Task t = TaskAssignment_el.srcTaskAssignment_end();
		MakeTSK(n, t, genbuilder);
	} END_ITERATE_C();

		//ofs << endl << global_lines.str() << endl;
		//ofs << endl << "void init() {" << endl << init_lines.str() << "}" << endl;
		//ofs << endl << main_lines.str() << endl;

	//} catch( udm_exception& e) {
	//	ofs.close();
	//	cerr << "ERROR: " << e.what() << endl;
	//	ret = false;
	//}
	//ofs.close();

	//ttp_build << TTP_BUILD_FOOTER;
	//ttp_build.close();
	//return ret;
}

void MakeTSK(const Node & node, const Task& tsk, GenBuilder gb )
{  
	cout << "Generating Task " << ((const string&)tsk.name()).c_str() << " ... ";

	char buff[BUFF_SIZE];

	unsigned long long wcet = 0;

	gb.AddNodeTask( node, tsk );

	//stringstream	decl_lines(stringstream::out);
	//stringstream	get_lines(stringstream::out);
	//stringstream	invoke_lines(stringstream::out);
	//stringstream	put_lines(stringstream::out);
	//int il=1;

	map<IOPort, string> portDT;
	set<TaskAssignment> tskassgn = tsk.dstTaskAssignment();
	//const set<SystemRef>& sref_s = tsk.dstSystemRef();
	
	//for(set<TaskAssignment>::const_iterator t_i=tskassign_s.begin(); t_i!=tskassign.end(); t_i++)
	//{
	//	const SystemRef& sref = t_i.dstTaskAssignment_end();
	//	wcet += (unsigned long long)sref.WCET();
	//}

	//sprintf_s(buff, sizeof(buff), TTP_BUILD_ADDTASK, ((const string&)tsk.name()).c_str(), wcet,
	//	((const string&)tsk.name()).c_str(), ((const string&)tsk.name()).c_str(), ((const string&)tsk.name()).c_str());
	//ttp_build << buff;

	//for(set<TaskAssignment>::const_iterator t_i=tskassign_s.begin(); t_i!=tskassign.end(); t_i++)
	//for(set<SystemRef>::const_iterator sref_i=sref_s.begin(); sref_i!=sref_s.end(); sref_i++)
	BEGIN_ITERATE_C( Component, t );
	{
		//const SystemRef& sref = t_i.dstTaskAssignment_end();
		//const System& comp = sref.ref();
		ComponentBase cb = Component_el.ref();

		// !!!!!!!!!!!!!!!  WE LEFT OFF HERE LAST!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

		// We have to cast it separately to both subtypes, as the base does not 
		// have any ports on it.
		CCode cbc = CCode::Cast( cb ); // What is the right casting syntax?

		// Try to cast to all of these --
		SubsystemRef cbr = SubsystemRef::Cast( cb );  // If this one, de-ref (see below)

		Subsystem cbs;     // final alternative
		if ( cbr )
		{
			cbs = cbr->ref();
		}
		else
		{
			cbs = Subsystem::Cast( cb );
		}

		// declare local variables corresp to inputs
		BEGIN_ITERATE_C(InputPort, (cbs) ? cbs : cbc ) {
			//string dt = type_map[(const string&)InputPort_el.DataType()];
			//portDT[InputPort_el] = dt;
			//const string& cp_cnm = InputPort_el.CName();
			//string cp_nm = string("loc_") + (cp_cnm.empty() ? InputPort_el.name() : cp_cnm);

			// We probably don't need these at all anymore.
			if (is_external) {
				decl_lines << indents[il] << dt.c_str() << " " << cp_nm.c_str() << ";" << endl;
			} 
			else {
				decl_lines << indents[il] << GetSLPortType(InputPort_el) << " " << cp_nm.c_str() << ";" << endl;
			}

			const set<InCommMapping>& icm_s = InputPort_el.srcInCommMapping();
			if (icm_s.size() > 0)
			{
				if (icm_s.size() > 1) {
					cerr << endl 
						<< "Warning: System \"" << ((const string&)comp.name()).c_str()
						<< "\" input port is connected to more then one bus messages.\n"
						<< " Only one will be used.  "
						<< endl;
				}
				const InCommMapping& icm = *(icm_s.begin());
				const CommDst& mp = icm.srcInCommMapping_end();
				const Message& msg = (mp.type() == MessageRef::meta) ? MessageRef::Cast(mp).ref() : Message::Cast(mp);
				get_lines << indents[il] << cp_nm.c_str() << " = " << ((const string&)msg.name()) << ";" << endl;
				
				//string msgName = ((const string&)msg.name());
				//sprintf_s(buff, sizeof(buff), TTP_PLAN_ADDRECEIVEDMSG);
				//ttp_plan << buff;

				//sprintf_s(buff, sizeof(buff), TTP_BUILD_ADDRECEIVEDMSG, ((const string&)tsk.name()).c_str(), msgName.c_str(),
				//	((const string&)tsk.name()).c_str(), msgName.c_str());
				//ttp_build << buff;
			}

		} END_ITERATE_C();

		// declare local variables corresp to outputs - only if the comp is not forwarder
		BEGIN_ITERATE_C(OutputPort, comp) {
			string dt = type_map[(const string&)OutputPort_el.DataType()];
			portDT[OutputPort_el] = dt;
			const string& cp_cnm = OutputPort_el.CName();
			string cp_nm = string("loc_") + (cp_cnm.empty() ? OutputPort_el.name() : cp_cnm);
			
			if (is_external) {
				decl_lines << indents[il] << dt.c_str() << " " << cp_nm.c_str() << ";" << endl;
			}
			else {
				decl_lines << indents[il] << GetSLPortType(OutputPort_el) << " " << cp_nm.c_str() << ";" << endl;
			}


			const set<OutCommMapping>& ocm_s = OutputPort_el.dstOutCommMapping();
			if (ocm_s.size() > 0)
			{
				if (ocm_s.size() > 1) {
					cerr << endl 
						<< "Warning: System \"" << ((const string&)comp.name()).c_str()
						<< "\" output port is connected to more then one bus messages.\n"
						<< " Only one will be used.  "
						<< endl;
				}
				const OutCommMapping& ocm = *(ocm_s.begin());
				const CommDst& mp = ocm.dstOutCommMapping_end();
				const Message& msg = (mp.type() == MessageRef::meta) ? MessageRef::Cast(mp).ref() : Message::Cast(mp);

				put_lines << indents[il] << "tt_Raw_Value(" << ((const string&)msg.name()) << ") = " << cp_nm.c_str() << ";" << endl;

				string msgName = ((const string&)msg.name());
				sprintf_s(buff, sizeof(buff), TTP_PLAN_ADDSENTMSG, msgName.c_str(), msgName.c_str(), ((const string&)OutputPort_el.DataType()).c_str(), 
					((const string&)tsk.name()).c_str(), msgName.c_str(), msgName.c_str(), msgName.c_str(), (int)(msg.CycleTime()));
				ttp_plan << buff;

				sprintf_s(buff, sizeof(buff), TTP_BUILD_ADDSENTMSG, ((const string&)tsk.name()).c_str(), msgName.c_str(),
					((const string&)tsk.name()).c_str(), msgName.c_str());
				ttp_build << buff;
			}
		} END_ITERATE_C();

		vector<OutputPort> outports = comp.OutputPort_kind_children();
		if (outports.size() == 0) {
			// Add dummy IO message...
			sprintf_s(buff, sizeof(buff), TTP_BUILD_ADDIOMSG, ((const string&)tsk.name()).c_str(), ((const string&)tsk.name()).c_str(), 
				((const string&)tsk.name()).c_str(), ((const string&)tsk.name()).c_str(), ((const string&)tsk.name()).c_str(), ((const string&)tsk.name()).c_str());
			ttp_build << buff;
		}

		if (is_external) {
			init_lines << indents[1] << ((const string&)comp.name()) << "_init();" << endl;
			invoke_lines << indents[il++] << ((const string&)comp.name()) << "(";
			int argc=0;
			BEGIN_ITERATE_C(InputPort, comp) {
				const string& cp_cnm = InputPort_el.CName();
				string cp_nm = string("loc_") + (cp_cnm.empty() ? InputPort_el.name() : cp_cnm);
				invoke_lines << ((argc++ == 0) ? "" : ", ") << cp_nm;
			} END_ITERATE_C();
			BEGIN_ITERATE_C(OutputPort, comp) {
				const string& cp_cnm = OutputPort_el.CName();
				string cp_nm = string("loc_") + (cp_cnm.empty() ? OutputPort_el.name() : cp_cnm);
				invoke_lines << ((argc++ == 0) ? "&" : ", &") << cp_nm;
			} END_ITERATE_C();
			invoke_lines << indents[--il] << ");" << endl << endl;
		}
		else
		{
			//const SystemRef& sref = *(sref_s.begin());
			//const System& sys = sref.ref();
			if (comp == Udm::null)
			{
				string e = "System reference " + (const string&)sref.name()
					+ string(" is null!");
				throw udm_exception(e);
			}

			// call MakeSLC to build the simulink code for this system
			//       if ( !MakeSLC(sys) )
			//         throw udm_exception(" MakeSLC failed ");


			string fn_name = (boost::format("%1%_%2%") % (const string&)comp.name() % comp.uniqueId()).str();
			global_lines << "static " << fn_name << "_context " << fn_name << "_ctx;" << endl;
			init_lines << indents[1] << fn_name << "_init(&" << fn_name << "_ctx);" << endl;
			invoke_lines << indents[il++] << fn_name << "_main(&" << fn_name << "_ctx";

			// pass arguments to SL comp function
			vector<InputPort> in_s = comp.InputPort_kind_children();
			//sort(in_s.begin(), in_s.end(), portOrder);
			BEGIN_ITERATE_CCOLL(InputPort, in_s) {
				const set<InPortMapping>& ipm_s = InputPort_el.srcInPortMapping();
				if (ipm_s.size() > 1)
				{
					string e = "System Port " + (const string&)sys.name() + string(".") + (const string&)InputPort_el.name()
						+ string(" has multiple input connections ");
					throw udm_exception(e);
				}
				string cp_nm = "0";
				if (ipm_s.size() == 1)
				{
					const InPortMapping& ipm = *(ipm_s.begin());
					const CPort& cp = ipm.srcInPortMapping_end();
					cp_nm = (const string&)(cp.CName());
					if (cp_nm.empty()) cp_nm = cp.name();
					cp_nm = string("loc_") + cp_nm;
				}
				invoke_lines << ", " << cp_nm.c_str();
			} END_ITERATE_CCOLL();

			vector<OutputPort> out_s = sys.OutputPort_kind_children();
			//sort(out_s.begin(), out_s.end(), portOrder);
			BEGIN_ITERATE_CCOLL(OutputPort, out_s) {
				const set<OutPortMapping>& opm_s = OutputPort_el.dstOutPortMapping();
				if (opm_s.size() > 1)
				{
					string e = "System Port " + (const string&)sys.name() + string(".") + (const string&)OutputPort_el.name()
						+ string(" has multiple input connections ");
					throw udm_exception(e);
				}
				string cp_nm = "0";
				if (opm_s.size() == 1)
				{
					const OutPortMapping& opm = *(opm_s.begin());
					const CPort& cp = opm.dstOutPortMapping_end();
					cp_nm = (const string&)cp.CName();
					if (cp_nm.empty()) cp_nm = cp.name();
					cp_nm = string("loc_") + cp_nm;
				}
				invoke_lines << ", &" << cp_nm.c_str();
			} END_ITERATE_CCOLL();
			invoke_lines << indents[--il] << ");" << endl << endl;
		}
	}
	main_lines << "tt_task( " << ((const string&)tsk.name()).c_str() << "_proc) {" << endl;
	main_lines << decl_lines.str() << endl << get_lines.str() << endl << invoke_lines.str() << endl << put_lines.str() << endl;
	main_lines << "}" << endl;


	cout << " Done!" << endl;
}



/* Sample code for google template engine
int testmain(int argc, char** argv) {
  google::TemplateDictionary dict("example");
  dict.SetValue("NAME", "John Smith");
  int winnings = rand() % 100000;
  dict.SetIntValue("VALUE", winnings);
  dict.SetFormattedValue("TAXED_VALUE", "%.2f", winnings * 0.83);
  // For now, assume everyone lives in CA.
  // (Try running the program with a 0 here instead!)
  if (0) {
    dict.ShowSection("IN_CA");
  } else {
	dict.ShowSection("IN_TX");
  }

  google::Template* tpl = google::Template::GetTemplate("example.tpl",
                                                        google::DO_NOT_STRIP);
  std::string output;
  tpl->Expand(&output, &dict);
  std::cout << output;
  return 0;
} */