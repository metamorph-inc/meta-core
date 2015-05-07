#include "Stage2_VisitorFactory.h"
#include "Stage2_SchedVisitor.h"

#include "ConfigKeeperS2.h"
bool Stage2_SchedVisitor::_initialized =
 Stage2_VisitorFactory::registerInstantiator(  string( "Sched" ), &Stage2_SchedVisitor::getSchedVisitor  );

#include "Uml.h"

#include <set>
#include <sstream>

using namespace std;
using namespace Semantics;

std::string Stage2_SchedVisitor::getQualifiedName(const Semantics::Task & t )
{
	ostringstream out;
	Semantics::Node n = t.executedBy();
	out << n.name() << "/" << t.name();
	return out.str();
}

Stage2_SchedVisitor::Stage2_SchedVisitor(Semantics::RootFolder & rf, timeStamp & ts) : 
	Stage2_AbstractVisitor(rf, ts)
{
}

Stage2_AbstractVisitor * getSchedVisitor( Semantics::RootFolder & rf, timeStamp & ts ) { 
	return new Stage2_SchedVisitor( rf, ts );
}

void Stage2_SchedVisitor::Visit_RootFolder(const Semantics::RootFolder & rf)
{
	getMainDictionary().SetValue( "APPNAME", string(rf.name()) );
	getMainDictionary().SetValue( "GENTIME", _datetime.getDateTime() );
	getMainDictionary().SetValue( "RESOLUTION", ConfigKeeperS2::inst().getResolution() );	

	DeploymentSet deps = rf.Deployment_children();
	for ( DeploymentSet::iterator idep = deps.begin(); idep != deps.end(); idep++ )
	{
		(*idep).Accept(*this);
	}

	google::Template *googleTemplate = google::Template::GetTemplate( "sched.tpl", google::DO_NOT_STRIP );
	std::string output;
	googleTemplate->Expand( &output, &getMainDictionary() );

	ofstream outputFile( "sched.scs" );
	outputFile << output;
	outputFile.close();
}

void Stage2_SchedVisitor::Visit_Deployment(const Semantics::Deployment & dep )
{
	NodeSet nodes = dep.Node_children();
	for ( NodeSet::iterator inode = nodes.begin(); inode != nodes.end(); inode++ )
	{
		cout << "Node " << (*inode).name() << endl;
		// Do the stuff
		addSectionDictionary( "HOST_SECTION" );

		getTemplateDictionary().SetValue( "NODENAME", string((*inode).name()) );
		getTemplateDictionary().SetValue( "NODEFREQ", hz2freqStr((*inode).clockspeedhz()) );
		getTemplateDictionary().SetValue( "SENDOHD", secs2timeStr((*inode).sndohdsecs()) );
		getTemplateDictionary().SetValue( "RECVOHD", secs2timeStr((*inode).rcvohdsecs()) );

		// Visit the tasks
		TaskSet tasks = (*inode).executes();
		for ( TaskSet::iterator itask = tasks.begin(); itask != tasks.end(); itask++ )
		{
			if ( (*itask).type() == Semantics::TTTask::meta )
			{
				cout << "Task " << (*itask).name() << endl;
				TTTask ttt = TTTask::Cast( *itask );
				ttt.Accept(*this);
			}	
		}

		popSectionDictionary();
	}


	TTBusSet buses = dep.TTBus_kind_children();
	for ( TTBusSet::iterator ibus = buses.begin(); ibus != buses.end(); ibus++ )
	{
		cout << "Bus " << (*ibus).name() << endl;

		addSectionDictionary( "BUS_SECTION" );
		getTemplateDictionary().SetValue( "BUSNAME", string((*ibus).name()) );
		getTemplateDictionary().SetValue( "BUSRATE", bps2rateStr((*ibus).dataratebps()) );
		getTemplateDictionary().SetValue( "SETUPTIME", secs2timeStr((*ibus).setupsecs()) );

		// Visit the messages and process the TT msgs
		//MsgSet msgs = (*ibus).mediummsgs();
		MsgSet msgs;
		IfaceSet ifaces = (*ibus).interfaces();
		for ( IfaceSet::iterator iiface = ifaces.begin(); iiface != ifaces.end(); iiface++ )
		{
			// Then loop over the channels and accumulate all of the messages
			BChanSet bchans = (*iiface).commChannels();
			for ( BChanSet::iterator bc = bchans.begin(); bc != bchans.end(); bc++ )
			{
				TransmitsSet trans = (*bc).channelTransmitters();
				for ( TransmitsSet::iterator it = trans.begin(); it != trans.end(); it++ )
				{
					msgs.insert((*it).transmittedMsg());
				}
			}
		}
		
		// Finally iterate over the messages and 
		for ( MsgSet::iterator imsg = msgs.begin(); imsg != msgs.end(); imsg++ )
		{
			if ( (*imsg).type() == Semantics::TTMsg::meta )
			{
				cout << "Msg " << (*imsg).name() << endl;
				TTMsg ttm = TTMsg::Cast( *imsg );
				ttm.Accept(*this);
			}
		}
	}
}

void Stage2_SchedVisitor::Visit_TTTask(const Semantics::TTTask & ttt )
{
	addSectionDictionary( "TASK_SECTION" );
	getTemplateDictionary().SetValue( "TASKNAME", string(ttt.name()) );
	double period = ttt.samplesecs();
	double freq = (period < 1.0e-12) ? 0.0 : (1.0/period);
	getTemplateDictionary().SetValue( "FREQUENCY", hz2freqStr(freq) );
	getTemplateDictionary().SetValue( "WCEXECTIME", secs2timeStr(ttt.wcetsecs()) );
	popSectionDictionary();
}

void Stage2_SchedVisitor::Visit_TTMsg(const Semantics::TTMsg & ttm )
{
	addSectionDictionary( "MSG_SECTION" );
	getTemplateDictionary().SetValue( "MSGNAME", string(ttm.name()) );
	getTemplateDictionary().SetValue( "MSGSIZE", bytes2sizeStr(ttm.sizebytes()) );

	Semantics::Transmits trans = ttm.msgTransmitter();
	Semantics::Task tsk = trans.sendingTask();
	getTemplateDictionary().SetValue( "SENDTASK", getQualifiedName(tsk));

	ostringstream rcvrs;
	ReceivesSet rcvrset = ttm.msgListeners();
	

	for ( ReceivesSet::iterator ircvr = rcvrset.begin(); ircvr != rcvrset.end(); ircvr++ )
	{
		tsk = (*ircvr).receivingTask();
		rcvrs << getQualifiedName(tsk) << " ";
	}
	getTemplateDictionary().SetValue( "RECVTASKS", rcvrs.str() );	
	popSectionDictionary();
}


/*  Original code from schedGen

google::TemplateDictionary * d2 = d1->AddSectionDictionary( "MSG_SECTION" );
d2->SetValue( "MSGNAME", m.basename );
d2->SetValue( "MSGSIZE", m.msgsize );
d2->SetValue( "SENDTASK", m.sender );
d2->SetValue( "RECVTASKS", m.receivers );
}
}

// Latencies
if ( _latencyinfo.size() > 0 )
{
std::vector< LatencyInfo >::iterator pLatency;
for ( pLatency = _latencyinfo.begin(); pLatency != _latencyinfo.end(); pLatency++ )
{
google::TemplateDictionary * d1 = dict.AddSectionDictionary( "LATENCY_SECTION" );
d1->SetValue( "LATENCY", pLatency->latency );
d1->SetValue( "SENDTASK", pLatency->sender );
d1->SetValue( "RECVTASK", pLatency->receiver );
}
}

// Now write everything out...
google::Template * tpl = google::Template::GetTemplate( "SchedTemplates\\sched.tpl", google::DO_NOT_STRIP );

std::string output;
tpl->Expand( &output, &dict );

std::string filename(LOCAL_OUTPUT_PATH);
filename += "\\" + _designname + "_" + _gentime + ".scs";
ofstream ofile( filename.c_str() );
ofile << output;
ofile.close();

std::string logfilename(LOCAL_OUTPUT_PATH);
logfilename += "\\SchedGenLog.txt";
ofstream logfile( logfilename.c_str() );
logfile << _logstr;
logfile.close();
}

void SchedGen::GenerateSched()
{
if ( _mkdir( LOCAL_OUTPUT_PATH ) != 0 )
if ( errno != EEXIST )
return;

x google::TemplateDictionary dict( "sched" );

x // The good stuff happens here!
x dict.SetValue( "APPNAME", _designname );
x dict.SetValue( "GENTIME", _gentime );
x dict.SetValue( "RESOLUTION", "1 us" );

// Nodes and tasks
std::map< std::string, NodeInfo >::iterator pNode;
for ( pNode = _nodeinfo.begin(); pNode != _nodeinfo.end(); pNode++ )
{
google::TemplateDictionary * d1 = dict.AddSectionDictionary( "HOST_SECTION" );
d1->SetValue( "NODENAME", pNode->first );
d1->SetValue( "NODEFREQ", (pNode->second).speed );
d1->SetValue( "SENDOHD", (pNode->second).sendohd );
d1->SetValue( "RECVOHD", (pNode->second).recvohd );

pair<std::multimap<std::string, std::string>::iterator,std::multimap<std::string, std::string>::iterator > range;
std::multimap<std::string, std::string>::iterator pPair;

range = _nodetasks.equal_range( pNode->first ); // All tasks for this node

for ( pPair = range.first; pPair != range.second; pPair++ )
{
TaskInfo t = _taskinfo[ pPair->second ];
google::TemplateDictionary * d2 = d1->AddSectionDictionary( "TASK_SECTION" );
d2->SetValue( "TASKNAME", t.basename );
d2->SetValue( "FREQUENCY", t.freq );
d2->SetValue( "WCEXECTIME", t.wcexectime );
}
}

// Buses and messages
std::map< std::string, BusInfo >::iterator pBus;
for ( pBus = _businfo.begin(); pBus != _businfo.end(); pBus++ )
{
google::TemplateDictionary * d1 = dict.AddSectionDictionary( "BUS_SECTION" );
d1->SetValue( "BUSNAME", pBus->first );
d1->SetValue( "BUSRATE", pBus->second.rate );
d1->SetValue( "SETUPTIME", pBus->second.setuptime );

// All msgs for this bus
std::set< std::string > busMsgs = _busmsgs[pBus->first];
std::set< std::string >::iterator pMsgs;
for ( pMsgs = busMsgs.begin(); pMsgs != busMsgs.end(); pMsgs++ )
{
MsgInfo m = _msginfo[ *pMsgs ];

*/
