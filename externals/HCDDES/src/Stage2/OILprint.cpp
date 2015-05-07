#include "Uml.h"
#include "OILprint.h"
#include <fstream>

using namespace OIL;
static void Print(OIL_VERSION);
static void Print(Include);
static void Print(CPU);
static void Print(OIL_OS);
static void Print(OIL_COM);
static void Print(TASK);
static void Print(Procedure);
static void Print(ALARM);
static void Print(EVENT);

static ofstream ofs;
static int il = 0;
static char *indents[] = {
	"",
	"  ",
	"    ",
	"      ",
	"        ",
	"          ",
	0
};



void Print(OILFile oil)
{
	ofs.open( string(oil.filename()).c_str() );
	il = 0;

	Print( oil.OIL_VERSION_child() );

	set<Include> inc_s = oil.Include_children();
	for(set<Include>::iterator inc_i=inc_s.begin(); inc_i!=inc_s.end(); inc_i++)
		Print( *inc_i );

	Print( oil.CPU_child() );

	ofs.close();
}

void Print(OIL_VERSION ver)
{
	ofs << indents[il] << "OIL_VERSION = \"" << string(ver.text()).c_str() << "\";" << endl << endl;
}

void Print(Include inc)
{
	ofs << indents[il] << "#include <" << inc.filename() << ">" << endl;
}

void Print(CPU cpu)
{
	ofs << indents[il++] << "CPU " << string(cpu.name()).c_str() << " {" << endl;

	set<Include> inc_s = cpu.Include_children();
	for(set<Include>::iterator inc_i=inc_s.begin(); inc_i!=inc_s.end(); inc_i++)
		Print( *inc_i );

	Print( cpu.OIL_OS_child() );
	ofs << endl;
	Print( cpu.OIL_COM_child() );
	ofs << endl;

	set<EVENT> evt_s = cpu.EVENT_children();
	for(set<EVENT>::iterator evt_i=evt_s.begin(); evt_i!=evt_s.end(); evt_i++)
		Print( *evt_i );

	set<ALARM> alm_s = cpu.ALARM_children();
	for(set<ALARM>::iterator alm_i=alm_s.begin(); alm_i!=alm_s.end(); alm_i++)
		Print( *alm_i );

	set<TASK> tsk_s = cpu.TASK_children();
	for(set<TASK>::iterator tsk_i=tsk_s.begin(); tsk_i!=tsk_s.end(); tsk_i++)
		Print( *tsk_i );

	ofs << indents[--il] << "};" << endl;
}


#define print_str_attr(attr,obj) \
	{ ofs << indents[il] << #attr " = " << string(obj.attr()).c_str() << ";" << endl; }
#define print_bool_attr(attr,obj) \
	{ ofs << indents[il] << #attr " = " << ( ((bool)obj.attr()) ? "TRUE" : "FALSE" ) << ";" << endl; }
#define print_int_attr(attr,obj) \
	{ ofs << indents[il] << #attr " = " << (int)obj.attr() << ";" << endl; }

void Print(OIL_OS os)
{
	ofs << indents[il++] << "OS " << string(os.name()).c_str() << " {" << endl;
	print_str_attr(CC,os);
	print_str_attr(STATUS,os);
	print_str_attr(SCHEDULE,os);
	print_bool_attr(STARTUPHOOK,os);
	print_bool_attr(ERRORHOOK,os);
	print_bool_attr(SHUTDOWNHOOK,os);
	print_bool_attr(PRETASKHOOK,os);
	print_bool_attr(POSTTASKHOOK,os);
	print_bool_attr(USEGETSERVICEID,os);
	print_bool_attr(USEPARAMETERACCESS,os);
	print_int_attr(TickTime,os);
	ofs << indents[--il] << "}: \"" << os.comment() << "\";" << endl;
}

void Print(OIL_COM com)
{
	ofs << indents[il++] << "COM " << string(com.name()).c_str() << " {" << endl;
	print_bool_attr(USEMESSAGERESOURCE,com);
	print_bool_attr(USEMESSAGESTATUS,com);
	ofs << indents[--il] << "}: \"" << com.comment() << "\";" << endl;
}

void Print(EVENT evt)
{
	ofs << indents[il++] << "EVENT " << string(evt.name()).c_str() << " {" << endl;
	if (string(evt.Component()) != "")
		print_str_attr(Component,evt);
	print_str_attr(MASK,evt);
	ofs << indents[--il] << "};" << endl;
}

void Print(ALARM alm)
{
	ofs << indents[il++] << "ALARM " << string(alm.name()).c_str() << " {" << endl;
	if (string(alm.Component()) != "")
		print_str_attr(Component,alm);
	print_str_attr(COUNTER,alm);
	EVENT evt = alm.event();
	if (evt != Udm::null)
	{
		ofs << indents[il] << "ACTION = " << string(alm.ACTION()).c_str() << endl;
		ofs << indents[il++] << "{" << endl;

		TASK tsk = evt.ownerTask();
		ofs << indents[il] << "TASK = " << tsk.name() << ";" << endl;
		ofs << indents[il] << "EVENT = " << evt.name() << ";" << endl;

		ofs << indents[--il] << "};" << endl;
	}
	{
		ofs << indents[il] << "AUTOSTART = " << ((bool)alm.AUTOSTART() ? "TRUE" : "FALSE") << endl;
		ofs << indents[il++] << "{" << endl;

		print_str_attr(APPMODE,alm);
		print_int_attr(ALARMTIME,alm);
		print_int_attr(CYCLETIME,alm);
		print_str_attr(AlarmUnit,alm);
		print_bool_attr(StaticAlarm,alm);

		ofs << indents[--il] << "};" << endl;
	}
	ofs << indents[--il] << "};" << endl;
}

void Print(TASK tsk)
{
	ofs << indents[il++] << "TASK " << string(tsk.name()).c_str() << " {" << endl;
	if (string(tsk.Component()) != "")
		print_str_attr(Component,tsk);
	print_str_attr(TYPE,tsk);
	print_str_attr(SCHEDULE,tsk);
	print_int_attr(PRIORITY,tsk);
	print_int_attr(ACTIVATION,tsk);
	{
		ofs << indents[il] << "AUTOSTART = " << ((bool)tsk.AUTOSTART() ? "TRUE" : "FALSE") << endl;
		ofs << indents[il++] << "{" << endl;

		print_str_attr(APPMODE,tsk);

		ofs << indents[--il] << "};" << endl;
	}
	set<EVENT> evt_s = tsk.events();
	for(set<EVENT>::iterator evt_i=evt_s.begin(); evt_i!=evt_s.end(); evt_i++)
		ofs << indents[il] << "EVENT = " << (*evt_i).name() << ";" << endl;
	Procedure proc = tsk.Procedure_child();
	if (proc != Udm::null)
	{
		ofs << indents[il] << "Procedure = EventProcedure" << endl;
		ofs << indents[il++] << "{" << endl;

		EVENT evt = proc.event();
		ofs << indents[il] << "EVENT = " << evt.name() << ";" << endl;
		ofs << indents[il] << "Name = \"" << proc.name() << "\";" << endl;

		ofs << indents[--il] << "};" << endl;
	}

	ofs << indents[--il] << "};" << endl;
}
