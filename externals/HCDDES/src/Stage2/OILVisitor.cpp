/*** Warning Workaround ***/
#pragma warning( disable: 4251 )

#include "VisitorFactory.h"
#include "OILVisitor.h"
#include "ConfigKeeper.h"
#include "Uml.h"
#include "UdmStatic.h"
#include <boost/filesystem/operations.hpp>
#include "OILprint.h"

using namespace Semantics;
using namespace OIL;

bool OILVisitor::_initialized =
VisitorFactory::RegisterInstantiator( std::string( "OIL" ), &OILVisitor::GetOILVisitor  );

void OILVisitor::Visit_RootFolder( const Semantics::RootFolder & rootFolder) {
	bool success = true;

	std::vector<Semantics::Deployment> deployments = rootFolder.Deployment_kind_children();

	for (std::vector<Semantics::Deployment>::iterator it = deployments.begin(); it != deployments.end(); it++) {
		vector<Semantics::Node> nodes = (*it).Node_kind_children();
		for (std::vector<Semantics::Node>::iterator n_it = nodes.begin(); n_it != nodes.end(); n_it++) {
			MakeOIL(*n_it);
		}
	}
}

bool OILVisitor::MakeOIL(const Semantics::Node& ecu) {
	bool success = true;

	const string& ecu_cnm = ecu.name();
	const string& ecu_nm = ecu_cnm.empty() ? ecu.name() : ecu_cnm;

	UdmStatic::StaticDataNetwork oil_dn(OIL::diagram);

	std::string od = ConfigKeeper::inst().GetDirectoryName();
	
	try {
	  oil_dn.CreateNew("", "", OILFile::meta, Udm::CHANGES_LOST_DEFAULT);
	  OILFile oil_file = OILFile::Cast(oil_dn.GetRootObject());
	  oil_file.filename() = od + string("\\") + ecu_nm + string(".oil");

	  cout << "Generating OIL file \n -> " << od << " ... ";

	  OIL_VERSION ver = OIL_VERSION::Create(oil_file);
	  ver.text() = "2.3";

      Include inc_o = Include::Create(oil_file);
      inc_o.filename() = "VecCanol.i23";

	  CPU cpu = CPU::Create(oil_file);
	  cpu.name() = ecu_nm;

	  Include inc_c = Include::Create(cpu);
      inc_c.filename() = "VecCanol.s23";

	  MakeOIL(ecu, cpu);

	  // now do a print
	  Print(oil_file);

	  std::cout << " Done!" << std::endl;
	}
	catch (udm_exception& e)
	{
	  std::cerr << "ERROR: " << e.what();
	  success = false;
	}

	oil_dn.CloseNoUpdate();
	return success;
}

void OILVisitor::MakeOIL(const Semantics::Node& ecu, const CPU& cpu)
{
	// make an OS block
	const vector<Semantics::OSEKParameters> os_s = ecu.OSEKParameters_kind_children();
	if (os_s.size() != 1)
	{
		string str = string("Zero or more than one OS objects in Node: ") + (const string&)ecu.name() + "\n";
		throw udm_exception( str );
	}
	const Semantics::OSEKParameters& os = *(os_s.begin());
	MakeOIL(os, cpu);

	// TODO: Determine the ESMoL abstract concept for OIL::COM
	MakeOILCom(os, cpu);

	Semantics::Deployment deployment = Semantics::Deployment::Cast(ecu.parent());
	std::vector<Semantics::Task> tasks = deployment.Task_kind_children();
	for (vector<Semantics::Task>::iterator it = tasks.begin(); it != tasks.end(); it++) {
		MakeOIL(*it, cpu);
	}
}

void OILVisitor::MakeOILCom(const Semantics::OSEKParameters& e_com, const OIL::CPU& cpu)
{
	OIL::OIL_COM com = OIL_COM::Create(cpu);
	com.name() = "StdCOM";
	com.comment() = " no comments ";
	com.USEMESSAGERESOURCE() = true;
	com.USEMESSAGESTATUS() = true;

	if (e_com.GenerateTask()) {
		OIL::TASK tsk = OIL::TASK::Create(cpu);
		tsk.name() = "T_COM";
		tsk.Component() = "\"COM\"";
		tsk.TYPE() = "EXTENDED";
		tsk.SCHEDULE() = "NON";
		tsk.PRIORITY() = 5;
		tsk.ACTIVATION() = 1;
		tsk.AUTOSTART() = true;
		tsk.APPMODE() = "OSDEFAULTAPPMODE";

		#define handle_evt( evt ) \
		if (e_com.Handle##evt()) \
		{ \
			EVENT evt = EVENT::Create(cpu); \
			evt.name() = "evCom" #evt "Task"; \
			evt.Component() = "\"COM\""; \
			evt.MASK() = "AUTO"; \
			evt.ownerTask() = tsk; \
			tsk.events() += evt; \
			ALARM alm = ALARM::Create(cpu); \
			alm.name() = "alCom" #evt "Task"; \
			alm.Component() = "\"COM\""; \
			alm.COUNTER() = "SystemTimer"; \
			alm.ACTION() = "SETEVENT"; \
			alm.event() = evt; \
			evt.alarm() = alm; \
			alm.AUTOSTART() = true; \
			alm.APPMODE() = "OSDEFAULTAPPMODE"; \
			alm.ALARMTIME() = 0; \
			alm.CYCLETIME() = atoi(((const string&)e_com.CycleTime()).c_str());	\
			alm.AlarmUnit() = "MSEC"; \
			alm.StaticAlarm() = true; \
		}

    handle_evt(NM);
    handle_evt(CCL);
    handle_evt(RX);
    handle_evt(TX);
	}
}

// Validates that the exec_info parameter contains all of the children required
// to populate info about a task. Throws a udm_exception if not.
void OILVisitor::ValidateExecInfo(const Semantics::ExecInfo& exec_info)
{
	bool exec_info_ok = true;
	string errmsg = "ExecInfo object " + (std::string)exec_info.name() + " contains ";

	std::vector<Semantics::OsekExecParameters> parameters = exec_info.OsekExecParameters_kind_children();
	if (parameters.size() != 1) {
		exec_info_ok = false;
		errmsg.append(parameters.size() == 0 ? "0 " : "multiple ");
		errmsg.append("OsekExecParameters objects, ");
	}

	std::vector<Semantics::Priority> priorities = exec_info.Priority_kind_children();
	if (priorities.size() != 1) {
		exec_info_ok = false;
		errmsg.append(priorities.size() == 0 ? "0 " : "multiple ");
		errmsg.append("Priority objects, ");
	}

	std::vector<Semantics::TTSchedule> schedules = exec_info.TTSchedule_kind_children();
	if (schedules.size() != 1) {
		exec_info_ok = false;
		errmsg.append(schedules.size() == 0 ? "0 " : "multiple ");
		errmsg.append("TTSchedule objects, ");
	}

	std::vector<Semantics::Preemption> preemptions = exec_info.Preemption_kind_children();
	if (preemptions.size() != 1) {
		exec_info_ok = false;
		errmsg.append(preemptions.size() == 0 ? "0 " : "multiple ");
		errmsg.append("Preemption objects. ");
	}

	if (parameters.size() != 1 || priorities.size() != 1 || preemptions.size() != 1) {
		errmsg.append("Ensure that the TTExecInfo objects in your ESMoL model contain an "
					  "OSEKExecInfo object. ");
	}

	if (!exec_info_ok) {
		throw udm_exception(errmsg);
	}
}

void OILVisitor::MakeOIL(const Semantics::Task& e_tsk, const OIL::CPU& cpu)
{
	std::set<Semantics::ExecInfo> execInfos = e_tsk.info();
	if (execInfos.size() != 1) {
		string st = string("Zero or more than one ExecInfo objects associated with task ") +
					(const string&)e_tsk.name();
		throw udm_exception( st );
	}
	Semantics::ExecInfo execInfo = *(execInfos.begin());
	ValidateExecInfo(execInfo);

	std::vector<Semantics::OsekExecParameters> parameters = execInfo.OsekExecParameters_kind_children();
	Semantics::OsekExecParameters parameter = *(parameters.begin());

	std::vector<Semantics::Priority> priorities = execInfo.Priority_kind_children();
	Semantics::Priority priority = *(priorities.begin());

	std::vector<Semantics::TTSchedule> schedules = execInfo.TTSchedule_kind_children();
	Semantics::TTSchedule schedule = *(schedules.begin());

	std::vector<Semantics::Preemption> preemptions = execInfo.Preemption_kind_children();
	Semantics::Preemption preemption = *(preemptions.begin());

	TASK tsk = TASK::Create(cpu);
	tsk.name() = e_tsk.name();
	tsk.TYPE() = parameter.taskType();
	tsk.SCHEDULE() = schedule.sched(); // changed
	tsk.PRIORITY() = priority.priority();
	tsk.ACTIVATION() = parameter.activation();
	tsk.AUTOSTART() = parameter.autoStart();
	tsk.APPMODE() = "OSDEFAULTAPPMODE";

	set<Semantics::Periodicity> periodicities = execInfo.Periodicity_kind_children();
	if (periodicities.size() == 1) {
		float cycleTime = (*(periodicities).begin()).periodsecs();

		EVENT evt = EVENT::Create(cpu);
		evt.name() = ( (string)e_tsk.name() + string("_event") );
		evt.MASK() = "AUTO";

		evt.ownerTask() = tsk;
		tsk.events() += evt;

		ALARM alm = ALARM::Create(cpu);
		alm.name() = ( (string)e_tsk.name() + string("_alarm") );
		alm.COUNTER() = "SystemTimer";
		alm.ACTION() = "SETEVENT";
		alm.event() = evt;
		evt.alarm() = alm;
		alm.AUTOSTART() = true;
		alm.APPMODE() = "OSDEFAULTAPPMODE";
		alm.ALARMTIME() = 0;
		alm.CYCLETIME() = cycleTime;
		alm.AlarmUnit() = "MSEC";
		alm.StaticAlarm() = true;

		// create an event procedure for the task
		Procedure proc = Procedure::Create(tsk);
		proc.name() = ( (string)e_tsk.name() + string("_proc") );
		proc.event() = evt;
		evt.procs() += proc;
	}
}

void OILVisitor::MakeOIL(const Semantics::OSEKParameters& e_os, const OIL::CPU& cpu)
{
  OIL_OS os = OIL_OS::Create(cpu);
  os.name() = "StdOS";
  os.comment() = " no comments ";
  os.CC() = "AUTO";
  os.STATUS() = e_os.Status();
  os.SCHEDULE() = e_os.Schedule();
  os.STARTUPHOOK() = false;
  os.ERRORHOOK() = true;
  os.SHUTDOWNHOOK() = false;
  os.PRETASKHOOK() = false;
  os.POSTTASKHOOK() = false;
  os.USEGETSERVICEID() = false;
  os.USEPARAMETERACCESS() = false;

  // ticktime is now tickresolutionsecs of the Node
  Semantics::Node nodeParent = Semantics::Node::Cast(e_os.parent());
  os.TickTime() = (int)nodeParent.tickresolutionsecs();
}