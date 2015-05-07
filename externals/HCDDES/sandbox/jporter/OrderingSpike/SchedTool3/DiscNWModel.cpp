#include "DiscNWModel.h"

#include "TaskTypes.h"
#include "DiscTaskTypes.h"

#include "MsgTypes.h"
#include "DiscMsgTypes.h"

#include <iostream>
#include <string>
#include <map>

using namespace std;
using namespace boost;

void DiscProblem::Setup()
{
	_initialized = true;

	map< string, DiscTaskPtr > discTasks;
	ProcSet procs = _ref->getProcs();
	for ( ProcSet::iterator procIter = procs.begin(); procIter != procs.end(); procIter++ )
	{
		AddDiscProc( *procIter, discTasks );
	}

	BusSet buses = _ref->getBuses();
	for ( BusSet::iterator busIter = buses.begin(); busIter != buses.end(); busIter++ )
	{
		AddDiscBus( *busIter, discTasks );
	}

	LatencySet latencies = _ref->getLatencies();
	for ( LatencySet::iterator latencyIter = latencies.begin(); latencyIter != latencies.end(); 
		latencyIter++ )
	{
		AddDiscLatency( *latencyIter );
	}

	DiscretizeModel();
	CalcHyperperiod();
}

void DiscProblem::AddDiscProc( ProcPtr p, map< string, DiscTaskPtr > & discTasks )
{
	assert( _initialized == true ); // call Setup first
	DiscProcPtr dp = boost::make_shared< DiscProc >( p, shared_from_this() );
	dp->Setup( discTasks );
	_Procs.insert(dp);
}

void DiscProblem::AddDiscBus( BusPtr p, map< string, DiscTaskPtr > & discTasks )
{
	DiscBusPtr db = boost::make_shared< DiscBus >( p, shared_from_this() );
	db->Setup( discTasks );
	_Buses.insert(db);
}

void DiscProblem::AddDiscLatency( LatencyPtr p )
{
	DiscLatencyPtr dl = boost::make_shared< DiscLatency >( p, shared_from_this() );
	_Latencies.insert(dl);
}

void DiscProblem::Print()
{
	cout << "Global hyperperiod " << getSystemHyperperiodTicks() << " Ticks" << endl;

	DiscProcSet procs = getDiscProcs();
	for ( DiscProcSet::iterator procIter = procs.begin(); procIter != procs.end(); procIter++ )
	{
		( *procIter )->Print();
	}

	DiscBusSet buses = getDiscBuses();
	for ( DiscBusSet::iterator busIter = buses.begin(); busIter != buses.end(); busIter++ )
	{
		( *busIter )->Print();
	}

	DiscLatencySet latencies = getDiscLatencies();
	for ( DiscLatencySet::iterator latencyIter = latencies.begin(); latencyIter != latencies.end(); 
		latencyIter++ )
	{
		( *latencyIter )->Print();
	}
}

void DiscProblem::setSystemHyperperiod( Ticks hyperperiodticks )
{
	_hyperperiodTicks = hyperperiodticks;
}

void DiscProblem::CalcHyperperiod()
{
	set< Ticks > periods;

	for ( DiscProcSet::iterator procIter = _Procs.begin(); procIter != _Procs.end(); procIter++ )
	{	
		DiscTaskSet tasks = (*procIter)->getDiscTasks();
		for ( DiscTaskSet::iterator taskIter = tasks.begin(); taskIter != tasks.end(); taskIter++ )
		{
			periods.insert( (*taskIter)->getPeriodTicks() );
		}
	}

	Ticks hyperperiodticks = calculateLCM( periods );
	setSystemHyperperiod( hyperperiodticks );
}

Ticks DiscProblem::calculateLCM( const set< Ticks > & periods )
{
	Ticks hp_so_far = 1;
	for ( set< Ticks >::const_iterator periodIter = periods.begin(); 
		periodIter != periods.end(); periodIter++ )
	{
		hp_so_far = calculateLCM( hp_so_far, *periodIter );
	}

	return hp_so_far;
}

Ticks DiscProblem::calculateLCM( Ticks a, Ticks b  )
{
	Ticks r, atmp = a, btmp = b;
	// First find the GCD (Knuth)
	while( btmp > 0 )
	{
		r = atmp;
		atmp = btmp;
		btmp = r % btmp;
	}

	// Then use it to get the LCM (GCD(a,b) * LCM(a,b) == a * b)
	return ( a * b ) / atmp;
}


void DiscProblem::DiscretizeModel()
{
	double res = _ref->getSystemResolution();

	DiscProcSet procs = getDiscProcs();
	for ( DiscProcSet::iterator procIter = procs.begin(); procIter != procs.end(); procIter++ )
	{
		( *procIter )->DiscretizeProc( res );
	}

	DiscBusSet buses = getDiscBuses();
	for ( DiscBusSet::iterator busIter = buses.begin(); busIter != buses.end(); busIter++ )
	{
		( *busIter )->DiscretizeBus( res );
	}

	DiscLatencySet latencies = getDiscLatencies();
	for ( DiscLatencySet::iterator latencyIter = latencies.begin(); latencyIter != latencies.end(); 
		latencyIter++ )
	{
		( *latencyIter )->DiscretizeLatency( res );
	}
}

Ticks DiscProblem::Time2Ticks( double tm, double res, Ticks granularity )
{
	Ticks ticklength = (Ticks) (( tm / res ) + 0.5); // round up
	Ticks leftovers = ticklength % granularity;
	return ticklength + leftovers;
}

void DiscProc::Setup( map< string, DiscTaskPtr > & discTasks )
{
	TaskSet tasks = _ref->getTasks();
	for (TaskSet::iterator taskIter = tasks.begin(); taskIter != tasks.end(); taskIter++ )
	{
		DiscTaskPtr dt = AddDiscTask( *taskIter );
		discTasks[ (*taskIter)->Name() ] = dt;
	}

	ProcMsgSet procmsgs = _ref->getProcMsgs();
	for ( ProcMsgSet::iterator pmIter = procmsgs.begin(); pmIter != procmsgs.end(); pmIter++ )
	{
		AddDiscProcMsg( *pmIter, discTasks[ (*pmIter)->getSender()->Name() ] );
	}
}

DiscTaskPtr DiscProc::AddDiscTask( TaskPtr p )
{
	TTaskPtr tt = boost::dynamic_pointer_cast< TTask, Task >( p );
	PTaskPtr pt = boost::dynamic_pointer_cast< PTask, Task >( p );
	ATaskPtr at = boost::dynamic_pointer_cast< ATask, Task >( p );

	DiscTaskPtr dt;
	if ( tt != TTaskPtr() ) // was it a TTask?
	{
		DiscTTaskPtr dtt = boost::make_shared< DiscTTask >( tt, shared_from_this() );
		dt = dtt;
	}
	else if ( pt != PTaskPtr() ) // was it a PTask?
	{
		DiscPTaskPtr dpt = boost::make_shared< DiscPTask >( pt, shared_from_this() );
		dt = dpt;
	}
	else if ( at != ATaskPtr() ) // was it an ATask?
	{
		DiscATaskPtr dat = boost::make_shared< DiscATask >( at, shared_from_this() );
		dt = dat;
	}
	else // don't know -- it's some sort of generic task
	{
		dt = boost::make_shared< DiscTask >( p, shared_from_this() );
	}

	_Tasks.insert( dt );

	return dt;
}

void DiscProc::AddDiscProcMsg( ProcMsgPtr p, DiscTaskPtr dt )
{
	DiscProcMsgPtr dpm = boost::make_shared< DiscProcMsg >( p, shared_from_this(), dt );
	_ProcMsgs.insert( dpm );
}

void DiscProc::Print()
{
	cout << "Discrete Processor Model " << _ref->Name() << ":" << endl;
	DiscTaskSet tasks = getDiscTasks();
	for (DiscTaskSet::iterator taskIter = tasks.begin(); taskIter != tasks.end(); taskIter++ )
	{
		( *taskIter )->Print();
	}

	DiscProcMsgSet procmsgs = getDiscProcMsgs();
	for ( DiscProcMsgSet::iterator pmIter = procmsgs.begin(); pmIter != procmsgs.end(); pmIter++ )
	{
		( *pmIter )->Print();
	}	
}

void DiscProc::DiscretizeProc( double sys_res )
{
	// Figure out granularity
	double proc_res = _ref->getResolution();
	if ( proc_res > sys_res )
	{
		setGranularityTicks( (Ticks) ceil( proc_res / sys_res ) );
	}
	else
	{
		setGranularityTicks( 1 );
	}

	DiscTaskSet tasks = getDiscTasks();
	for (DiscTaskSet::iterator taskIter = tasks.begin(); taskIter != tasks.end(); taskIter++ )
	{
		( *taskIter )->DiscretizeTask( sys_res );
	}

	DiscProcMsgSet procmsgs = getDiscProcMsgs();
	for ( DiscProcMsgSet::iterator pmIter = procmsgs.begin(); pmIter != procmsgs.end(); pmIter++ )
	{
		( *pmIter )->DiscretizeProcMsg( sys_res );
	}
}

void DiscBus::Setup( std::map<std::string, DiscTaskPtr > & discTasks )
{
	MsgSet msgs = _ref->getMsgs();
	for ( MsgSet::iterator msgIter = msgs.begin(); msgIter != msgs.end(); msgIter++ )
	{
		AddDiscMsg( *msgIter, discTasks );
	}

	SyncMsgSet syncmsgs = _ref->getSyncMsgs();
	for ( SyncMsgSet::iterator syncmsgIter = syncmsgs.begin(); syncmsgIter != syncmsgs.end(); syncmsgIter++ )
	{
		AddDiscSyncMsg( *syncmsgIter );
	}
}

void DiscBus::AddDiscMsg( MsgPtr p, std::map< std::string, DiscTaskPtr > & discTasks )
{
	BMsgPtr bm = boost::dynamic_pointer_cast< BMsg, Msg >( p ); // figure out which kind of message we have

	DiscMsgPtr dm;
	DiscTaskPtr dt = discTasks[ p->getSender()->Name() ];
	if ( bm == BMsgPtr() ) // null pointer
	{
		
		dm = boost::make_shared< DiscMsg >( p, shared_from_this(), dt );
	}
	else
	{
		DiscTaskPtr rdt = discTasks[ bm->getReturnSender()->Name() ];
		DiscBMsgPtr dbm = boost::make_shared< DiscBMsg >( bm, shared_from_this(), dt, rdt );
		dm = dbm;
	}
	 
	_Msgs.insert( dm );
}

void DiscBus::AddDiscSyncMsg( SyncMsgPtr p )
{
	DiscSyncMsgPtr dsm = boost::make_shared< DiscSyncMsg >( p, shared_from_this() );
	_SyncMsgs.insert( dsm );
}

void DiscBus::Print()
{
	cout << "Discrete Bus " << _ref->Name() << ":" << endl;
	DiscMsgSet msgs = getDiscMsgs();
	for ( DiscMsgSet::iterator msgIter = msgs.begin(); msgIter != msgs.end(); msgIter++ )
	{
		( *msgIter )->Print();
	}

	DiscSyncMsgSet syncmsgs = getDiscSyncMsgs();
	for ( DiscSyncMsgSet::iterator syncmsgIter = syncmsgs.begin(); syncmsgIter != syncmsgs.end(); syncmsgIter++ )
	{
		( *syncmsgIter )->Print();
	}
}

void DiscBus::DiscretizeBus( double sys_res )
{
	// Figure out granularity
	double bus_res = _ref->getResolution();
	if ( bus_res > sys_res )
	{
		setGranularityTicks( (Ticks) ceil( bus_res / sys_res ) );
	}
	else
	{
		setGranularityTicks( 1 );
	}

	DiscMsgSet msgs = getDiscMsgs();
	for ( DiscMsgSet::iterator msgIter = msgs.begin(); msgIter != msgs.end(); msgIter++ )
	{
		( *msgIter )->DiscretizeMsg( sys_res );
	}

	DiscSyncMsgSet syncmsgs = getDiscSyncMsgs();
	for ( DiscSyncMsgSet::iterator syncmsgIter = syncmsgs.begin(); syncmsgIter != syncmsgs.end(); syncmsgIter++ )
	{
		( *syncmsgIter )->DiscretizeSyncMsg( sys_res );
	}
}

void DiscTask::Print()
{
	cout << "    Discrete Task " << _ref->Name() << " period " << _period_ticks << " ticks" 
		<< " duration " << _duration_ticks << " ticks" << endl;
}

void DiscTask::DiscretizeTask( double res )
{
	Ticks proc_granularity = _parent->getGranularityTicks();
	double task_period = _ref->getPeriod();

	_period_ticks = DiscProblem::Time2Ticks( task_period, res, proc_granularity );

	double task_duration = _ref->getDuration();
	_duration_ticks = DiscProblem::Time2Ticks( task_duration, res, proc_granularity );
}

void DiscProcMsg::Print()
{
	cout << "    Discrete Processor Message " << _ref->Name() << endl;
}

void DiscProcMsg::DiscretizeProcMsg( double res )
{
	_msgsize_ticks = 0; // transfer time is shared memory write time (folded into sending task duration)
}

void DiscSyncMsg::Print()
{
	cout << "    Discrete Sync Message " << _ref->Name() << " length " << _msgsize_ticks << " ticks" << endl;
}

void DiscSyncMsg::DiscretizeSyncMsg( double res )
{
	Ticks bus_granularity = _parent->getGranularityTicks();
	double msg_time = _ref->getTransferTime();

	_msgsize_ticks = DiscProblem::Time2Ticks( msg_time, res, bus_granularity );

	double msg_pos_min = _ref->getMinPos();
	_msg_pos_min_ticks = DiscProblem::Time2Ticks( msg_pos_min, res, bus_granularity );

	double msg_pos_max = _ref->getMaxPos();
	_msg_pos_max_ticks = DiscProblem::Time2Ticks( msg_pos_max, res, bus_granularity );
}

void DiscMsg::Print()
{
	cout << "    Discrete Message " << _ref->Name() << " length " << _msgsize_ticks << " ticks" << endl;
}

void DiscMsg::DiscretizeMsg( double res )
{
	Ticks bus_granularity = _parent->getGranularityTicks();
	double msg_time = _ref->getTransferTime();

	_msgsize_ticks = DiscProblem::Time2Ticks( msg_time, res, bus_granularity );
	_msgsize_ticks = std::max( _msgsize_ticks, (Ticks) 1 );
}

void DiscLatency::Print()
{
	cout << "Discrete Latency " << _latency_ticks << " ticks" << endl;
}

void DiscLatency::DiscretizeLatency( double res )
{
	double latency = _ref->getLatency();

	_latency_ticks = DiscProblem::Time2Ticks(latency, res);
}
