#include "ExpNWGraph.h"

#include <iostream>

using namespace std;
using namespace boost;

// Parts (in order of implementation)

// x structure setup (build tree)
// graph construction
// x instance calculation
// printing
//    ? print by parts (down the tree)
//       still need to do BMsg
//    print the whole flat graph (from problem)
//       use GraphViz?
//    generate diagrams for graphs down the tree (various subgraphs)
// latencies
//    figure out what is needed
//    implement it


void ExpNWSubgraph::AddExpTGVertex( ExpNWVertexPtr vp )
{
	TGVertex v = add_vertex( _root_tg );
	_root_tg[v].pobj = vp;

	TGVertex lv = add_vertex( v, _local_tg );
}


void ExpProblem::Setup()
{
	_initialized = true;

	DiscProcSet procs = _disc_ref->getDiscProcs();
	for ( DiscProcSet::iterator procIter = procs.begin(); procIter != procs.end(); procIter++ )
	{
		AddExpProc( *procIter );
	}

	DiscBusSet buses = _disc_ref->getDiscBuses();
	for ( DiscBusSet::iterator busIter = buses.begin(); busIter != buses.end(); busIter++ )
	{
		AddExpBus( *busIter );
	}

	DiscLatencySet latencies = _disc_ref->getDiscLatencies();
	for ( DiscLatencySet::iterator latencyIter = latencies.begin(); latencyIter != latencies.end(); 
		latencyIter++ )
	{
		AddExpLatency( *latencyIter );
	}

	CreateExpandedTaskGraph();
}

void ExpProblem::AddExpProc( DiscProcPtr p )
{
	assert( _initialized == true ); // call Setup first
	ExpProcPtr ep = boost::make_shared< ExpProc >( p, shared_from_this() );
	ep->Setup();
	_Procs.insert(ep);
}

void ExpProblem::AddExpBus( DiscBusPtr p )
{
	ExpBusPtr eb = boost::make_shared< ExpBus >( p, shared_from_this() );
	eb->Setup();
	_Buses.insert(eb);
}

void ExpProblem::AddExpLatency( DiscLatencyPtr p )
{
	ExpLatencyPtr el = boost::make_shared< ExpLatency >( p, shared_from_this() );
	_Latencies.insert(el);
}

void ExpProblem::Print()
{
	// Print any global params here

	ExpProcSet procs = getExpProcs();
	for ( ExpProcSet::iterator procIter = procs.begin(); procIter != procs.end(); procIter++ )
	{
		( *procIter )->Print();
	}

	ExpBusSet buses = getExpBuses();
	for ( ExpBusSet::iterator busIter = buses.begin(); busIter != buses.end(); busIter++ )
	{
		( *busIter )->Print();
	}

	ExpLatencySet latencies = getExpLatencies();
	for ( ExpLatencySet::iterator latencyIter = latencies.begin(); latencyIter != latencies.end(); 
		latencyIter++ )
	{
		( *latencyIter )->Print();
	}
}

void ExpProblem::CreateExpandedTaskGraph()
{
	// Do the stuff
	Ticks hpd = _disc_ref->getSystemHyperperiodTicks();

	ExpProcSet procs = getExpProcs();
	for ( ExpProcSet::iterator procIter = procs.begin(); procIter != procs.end(); procIter++ )
	{
		( *procIter )->CalculateInstances( hpd );
	}

	ExpBusSet buses = getExpBuses();
	for ( ExpBusSet::iterator busIter = buses.begin(); busIter != buses.end(); busIter++ )
	{
		( *busIter )->CalculateInstances( hpd );
	}

	ExpLatencySet latencies = getExpLatencies();
	for ( ExpLatencySet::iterator latencyIter = latencies.begin(); latencyIter != latencies.end(); 
		latencyIter++ )
	{
		( *latencyIter )->CalculateInstances( hpd );
	}
}

void ExpProc::Setup()
{
	DiscTaskSet tasks = _disc_ref->getDiscTasks();
	for ( DiscTaskSet::iterator taskIter = tasks.begin(); taskIter != tasks.end(); taskIter++ )
	{
		AddExpTask( *taskIter );
	}

	DiscProcMsgSet procmsgs = _disc_ref->getDiscProcMsgs();
	for ( DiscProcMsgSet::iterator pmIter = procmsgs.begin(); pmIter != procmsgs.end(); pmIter++ )
	{
		AddExpProcMsg( *pmIter );
	}
}

void ExpProc::AddExpTask( DiscTaskPtr p )
{
	ExpTaskPtr et = boost::make_shared< ExpTask >( p, shared_from_this() );

	_Tasks.insert( et );

	AddExpTGVertex( et );
}

void ExpProc::AddExpProcMsg( DiscProcMsgPtr p )
{
	ExpProcMsgPtr epm = boost::make_shared< ExpProcMsg >( p, shared_from_this() );

	_ProcMsgs.insert( epm );

	AddExpTGVertex( epm );
}

void ExpProc::Print()
{
	cout << "Exp Task Graph Processor Model " << _ref->Name() << ":" << endl;
	ExpTaskSet tasks = getExpTasks();
	for (ExpTaskSet::iterator taskIter = tasks.begin(); taskIter != tasks.end(); taskIter++ )
	{
		( *taskIter )->Print();
	}

	ExpProcMsgSet procmsgs = getExpProcMsgs();
	for ( ExpProcMsgSet::iterator pmIter = procmsgs.begin(); pmIter != procmsgs.end(); pmIter++ )
	{
		( *pmIter )->Print();
	}	
}

void ExpProc::CalculateInstances( Ticks hpd )
{
	ExpTaskSet tasks = getExpTasks();
	for (ExpTaskSet::iterator taskIter = tasks.begin(); taskIter != tasks.end(); taskIter++ )
	{
		( *taskIter )->CalculateInstances( hpd );
	}

	ExpProcMsgSet procmsgs = getExpProcMsgs();
	for ( ExpProcMsgSet::iterator pmIter = procmsgs.begin(); pmIter != procmsgs.end(); pmIter++ )
	{
		( *pmIter )->CalculateInstances( hpd );
	}
}

void ExpBus::Setup()
{
	DiscMsgSet msgs = _disc_ref->getDiscMsgs();
	for ( DiscMsgSet::iterator msgIter = msgs.begin(); msgIter != msgs.end(); msgIter++ )
	{
		AddExpMsg( *msgIter );
	}

	DiscSyncMsgSet syncmsgs = _disc_ref->getDiscSyncMsgs();
	for ( DiscSyncMsgSet::iterator syncmsgIter = syncmsgs.begin(); syncmsgIter != syncmsgs.end(); syncmsgIter++ )
	{
		AddExpSyncMsg( *syncmsgIter );
	}
}

void ExpBus::AddExpMsg( DiscMsgPtr p )
{
	ExpMsgPtr em = boost::make_shared< ExpMsg >( p, shared_from_this() );
	_Msgs.insert( em );
	AddExpTGVertex( em );
}

void ExpBus::AddExpSyncMsg( DiscSyncMsgPtr p )
{
	ExpSyncMsgPtr esm = boost::make_shared< ExpSyncMsg >( p, shared_from_this() );
	_SyncMsgs.insert( esm );
	AddExpTGVertex( esm );
}

void ExpBus::Print()
{
	cout << "Exp Task Graph Bus " << _ref->Name() << ":" << endl;
	ExpMsgSet msgs = getExpMsgs();
	for ( ExpMsgSet::iterator msgIter = msgs.begin(); msgIter != msgs.end(); msgIter++ )
	{
		( *msgIter )->Print();
	}

	ExpSyncMsgSet syncmsgs = getExpSyncMsgs();
	for ( ExpSyncMsgSet::iterator syncmsgIter = syncmsgs.begin(); syncmsgIter != syncmsgs.end(); syncmsgIter++ )
	{
		( *syncmsgIter )->Print();
	}
}

void ExpBus::CalculateInstances( Ticks hpd )
{
	ExpMsgSet msgs = getExpMsgs();
	for ( ExpMsgSet::iterator msgIter = msgs.begin(); msgIter != msgs.end(); msgIter++ )
	{
		( *msgIter )->CalculateInstances( hpd );
	}

	ExpSyncMsgSet syncmsgs = getExpSyncMsgs();
	for ( ExpSyncMsgSet::iterator syncmsgIter = syncmsgs.begin(); syncmsgIter != syncmsgs.end(); syncmsgIter++ )
	{
		( *syncmsgIter )->CalculateInstances( hpd );
	}
}

void ExpTask::Print()
{
	cout << "    Exp Task Graph Task " << _ref->Name() << " instances " << getNumInstances() <<  endl;	
}

void ExpTask::CalculateInstances( Ticks hpd )
{
	_num_inst = hpd / _disc_ref->getPeriodTicks();
}

void ExpProcMsg::Print()
{
	cout << "    Exp Task Graph Proc Msg " << _ref->Name() << " instances " << getNumInstances() << endl;	
}

void ExpProcMsg::CalculateInstances( Ticks hpd )
{
	Ticks sender_period = _disc_ref->getDiscSender()->getPeriodTicks();

	_num_inst = hpd / sender_period;
}

void ExpSyncMsg::Print()
{
	cout << "    Exp Task Graph Sync Msg " << _ref->Name() << " instances " << getNumInstances() <<  endl;	
}

void ExpSyncMsg::CalculateInstances( Ticks hpd )
{
	_num_inst = ( Ticks ) 1;
}

void ExpMsg::Print()
{
	cout << "    Exp Task Graph Msg " << _ref->Name() << " instances " << getNumInstances() <<  endl;
}

void ExpMsg::CalculateInstances( Ticks hpd )
{
	Ticks sender_period = _disc_ref->getDiscSender()->getPeriodTicks();

	_num_inst = hpd / sender_period;
}

void ExpLatency::Print()
{
	cout << "    Exp Task Graph Latency " << endl;
}

void ExpLatency::CalculateInstances( Ticks hpd )
{

}
