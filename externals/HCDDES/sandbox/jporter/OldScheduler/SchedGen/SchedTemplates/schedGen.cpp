#include "SchedGen.h"
#include "google/template.h"

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <ctime>

#include <iostream>
#include <fstream>
#include <algorithm>

#include <direct.h> // Windows-specific
#define LOCAL_OUTPUT_PATH "SchedFiles"

using namespace std;

SchedGen::SchedGen( const std::string & designname ) : _designname( designname ), _max_tick(0.0)
{
	time_t tsecs;
	time( &tsecs );
	struct tm * timeinfo;
	timeinfo = localtime( &tsecs );

	char timestr[100];
	strftime(timestr, 99, "%a_%b_%d_%H_%M_%S", timeinfo);
	_gentime = timestr;
}

void SchedGen::AddNode( const std::string & id, 
					    const std::string & node, const std::string & freq, 
					    const std::string & sndoverhead, 
						const std::string & rcvoverhead )
{
	NodeInfo n;
	n.id = id;
	n.speed = freq;
	n.sendohd = sndoverhead;
	n.recvohd = rcvoverhead;
	_nodeinfo[node] = n;	
}

void SchedGen::AddTask( const std::string & id, 
					    const std::string & node, const std::string & task, 
					    const std::string & freq, const std::string & wcexectime)
{
	string taskname = node + "/" + task;
	TaskInfo t;
	t.id = id;
	t.basename = task;
	t.freq = freq;
	CheckUnits( t.freq, "Hz" );
	t.wcexectime = wcexectime;
	CheckUnits( t.wcexectime, "s" );
	_taskinfo[ taskname ] = t;
	_nodetasks.insert( pair<string,string>(node, taskname) );
}

void SchedGen::AddBus( const std::string & id, 
					   const std::string & name, const std::string & rate, 
					   const std::string & setuptime )
{
	BusInfo b;
	b.id = id;
	b.rate = rate;
	CheckUnits( b.rate, "b" );
	b.setuptime = setuptime;
	CheckUnits( b.setuptime, "s" );
	_businfo[name] = b;
}

void SchedGen::AddMsg( const std::string & id, 
					   const std::string & bus, const std::string & msg, 
					   const std::string & msgsize)
{
	string msgname = bus + "/" + msg;
	MsgInfo m;
	m.id = id;
	m.basename = msg;
	m.msgsize = msgsize;
	CheckUnits( m.msgsize, "B" );
	_msginfo[ msgname ] = m;
	_busmsgs[bus].insert( msgname );
	_logstr += "Add ";
	_logstr += msgname;
	_logstr += ": "; 
	_logstr += m.msgsize;
	_logstr += "\n";
}

void SchedGen::AddMsgSndr( const std::string & bus, const std::string & msg, 
						   const std::string & sndnode, const std::string & sndtask )
{
	string msgname = bus + "/" + msg;
	string taskname = sndnode + "/" + sndtask;
	MsgInfo m = _msginfo[ msgname ];
	m.sender = taskname;
	_msginfo[msgname] = m;

	_logstr += "AddSndr "; 
	_logstr += msgname; 
	_logstr += " ";
	_logstr += _msginfo[msgname].sender;
	_logstr += "\n";
}

void SchedGen::AddMsgRcvr( const std::string & bus, const std::string & msg, 
						   const std::string & rcvnode, const std::string & rcvtask )
{
	string msgname = bus + "/" + msg;
	string taskname = rcvnode + "/" + rcvtask;
	MsgInfo m = _msginfo[ msgname ];
	m.receivers += taskname + " ";
	_msginfo[msgname] = m;

	_logstr += "AddRcvr ";
	_logstr += msgname;
	_logstr += " ";
	_logstr += _msginfo[msgname].receivers;
	_logstr += "\n";
}

void SchedGen::AddLatency( const std::string & sndnode, const std::string & sndtask, 
						   const std::string & rcvnode, const std::string & rcvtask, 
						   const std::string & latency )
{
	LatencyInfo l;
	l.latency = latency;
	CheckUnits( l.latency, "s" );
	l.sender = sndnode + "/" + sndtask;
	l.receiver = rcvnode + "/" + rcvtask;
	_latencyinfo.push_back( l );
}


void SchedGen::GenerateSched()
{
	if ( _mkdir( LOCAL_OUTPUT_PATH ) != 0 )
		if ( errno != EEXIST )
			return;

	google::TemplateDictionary dict( "sched" );

	// The good stuff happens here!
	dict.SetValue( "APPNAME", _designname );
	dict.SetValue( "GENTIME", _gentime );
	dict.SetValue( "RESOLUTION", "1 us" );

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


void SchedGen::GenerateIDs()
{
	if ( _mkdir( LOCAL_OUTPUT_PATH ) != 0 )
		if ( errno != EEXIST )
			return;

	std::string filename(LOCAL_OUTPUT_PATH);
	filename += "\\" + _designname + "_" + _gentime + ".ids";
	ofstream ofile( filename.c_str() );

	// Nodes and tasks
	std::map< std::string, NodeInfo >::iterator pNode;
	for ( pNode = _nodeinfo.begin(); pNode != _nodeinfo.end(); pNode++ )
	{
		ofile << pNode->second.id << " " << pNode->first << endl;

		pair<std::multimap<std::string, std::string>::iterator,std::multimap<std::string, std::string>::iterator > range;
		std::multimap<std::string, std::string>::iterator pPair;

		range = _nodetasks.equal_range( pNode->first ); // All tasks for this node
		for ( pPair = range.first; pPair != range.second; pPair++ )
		{
			TaskInfo t = _taskinfo[ pPair->second ];
			ofile << t.id << " " << pPair->second << endl;
		}

		ofile << endl;
	}

	// Buses and messages
	std::map< std::string, BusInfo >::iterator pBus;
	for ( pBus = _businfo.begin(); pBus != _businfo.end(); pBus++ )
	{
		ofile << pBus->second.id << " " << pBus->first << endl;

		// All msgs for this bus
		std::set< std::string > busMsgs = _busmsgs[pBus->first];
		std::set< std::string >::iterator pMsgs;
		for ( pMsgs = busMsgs.begin(); pMsgs != busMsgs.end(); pMsgs++ )
		{
			MsgInfo m = _msginfo[ *pMsgs ];
			ofile << m.id << " " << *pMsgs << endl;
		}

		ofile << endl;
	}

	ofile.close(); 
}

// Probably not safe to call twice in a row on the same string
void SchedGen::CheckUnits( std::string & str, const std::string & unitstr )
{
	string lunits(unitstr), lstr(str);
	std::transform( lunits.begin(), lunits.end(), lunits.begin(), tolower );
	std::transform( lstr.begin(), lstr.end(), lstr.begin(), tolower );
	if ( lstr.find( lunits ) == string::npos )
		str += unitstr;
}