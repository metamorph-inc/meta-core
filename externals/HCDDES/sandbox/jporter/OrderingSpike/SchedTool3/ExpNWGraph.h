#ifndef ExpNWGraph_h__
#define ExpNWGraph_h__

// Expanded Task Graph Model

// construct from a discretized network model
#include "NWModel.h"
#include "DiscNWModel.h"

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/make_shared.hpp>

#include <boost/property_map/property_map.hpp>
#include <boost/graph/subgraph.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/graph_utility.hpp>

#include <vector>
#include <set>
#include <map>

// Data Types

class ExpProblem;
class ExpProc;
class ExpBus;
class ExpTask;
class ExpProcMsg;
class ExpSyncMsg;
class ExpMsg;
class ExpLatency;

typedef boost::shared_ptr< ExpProblem > ExpProblemPtr;
typedef boost::shared_ptr< ExpProc > ExpProcPtr;
typedef std::set< ExpProcPtr > ExpProcSet;
typedef boost::shared_ptr< ExpBus > ExpBusPtr;
typedef std::set< ExpBusPtr > ExpBusSet;
typedef boost::shared_ptr< ExpTask > ExpTaskPtr;
typedef std::set< ExpTaskPtr > ExpTaskSet;
typedef boost::shared_ptr< ExpProcMsg > ExpProcMsgPtr;
typedef std::set< ExpProcMsgPtr > ExpProcMsgSet;
typedef boost::shared_ptr< ExpSyncMsg > ExpSyncMsgPtr;
typedef std::set< ExpSyncMsgPtr > ExpSyncMsgSet;
typedef boost::shared_ptr< ExpMsg > ExpMsgPtr;
typedef std::set< ExpMsgPtr > ExpMsgSet;
typedef boost::shared_ptr< ExpLatency > ExpLatencyPtr;
typedef std::set< ExpLatencyPtr > ExpLatencySet;

// Superclass for things that can be used as a vertex in the task graph (tasks and messages)
class ExpNWVertex
{
public:
	ExpNWVertex() { }
	virtual ~ExpNWVertex() { }

	virtual void CalculateInstances( Ticks hpd ) = 0;
	virtual Ticks getNumInstances() { return _num_inst; }

protected:
	unsigned long long _num_inst;

};

typedef boost::shared_ptr< ExpNWVertex > ExpNWVertexPtr;

typedef struct 
{
	ExpNWVertexPtr pobj;

} ExpNWVertexStruct;

// Custom property to hold a network object in the graph
typedef boost::property< boost::vertex_index_t, unsigned long long, ExpNWVertexStruct > vert_prop_t;

typedef boost::property< boost::edge_index_t, unsigned long long > edge_prop_t;

typedef boost::adjacency_list< boost::vecS, boost::vecS, boost::directedS, vert_prop_t, edge_prop_t > TaskGraphBase;
typedef boost::subgraph< TaskGraphBase > TaskGraph;
typedef boost::graph_traits< TaskGraphBase >::edge_descriptor TGEdge;
typedef boost::graph_traits< TaskGraphBase >::edge_iterator TGEdgeIter;
typedef boost::graph_traits< TaskGraphBase >::vertex_descriptor TGVertex;
typedef boost::graph_traits< TaskGraphBase >::vertex_iterator TGVertexIter;
typedef boost::graph_traits< TaskGraphBase >::adjacency_iterator TGAdjIter;

// Superclass for things that can contain subsets of the task graph (i.e. procs, buses, etc...)
class ExpNWSubgraph
{
public:

	ExpNWSubgraph( TaskGraph & root_tg ) : 
		_root_tg( root_tg ), _local_tg( _root_tg.create_subgraph() ) { }
	virtual ~ExpNWSubgraph() { }

	void AddExpTGVertex( ExpNWVertexPtr vp );

	// TODO: Figure out interface for adding vertices and edges
	//  (only used inside the tree) - it should add the right stuff, and 
	//  then store the ExpNWVertex object

	virtual void CalculateInstances( Ticks hpd ) = 0;

protected:

	TaskGraph & _root_tg;
	TaskGraph & _local_tg;

	std::map< unsigned long long, unsigned long long > _index_map; // < local idx, global idx >
};


class ExpProblem : public PrintableElement,
	public boost::enable_shared_from_this< ExpProblem >
{
public:
	ExpProblem() : _initialized(false) { }
	ExpProblem( DiscProblemPtr p )  : _ref( p->getRef() ), _disc_ref( p ), 
		_initialized( false ) { }
	virtual ~ExpProblem() { }

	void Setup();  // We had to move the tree copy from the constructor,
	// since shared_from_this is not initialized
	// until the end of the constructor call

	void AddExpProc( DiscProcPtr p );
	void AddExpBus( DiscBusPtr p );
	void AddExpLatency( DiscLatencyPtr p );

	ExpProcSet getExpProcs() { return _Procs; }
	ExpBusSet getExpBuses() { return _Buses; }
	ExpLatencySet getExpLatencies() { return _Latencies; }

	void CreateExpandedTaskGraph();
	TaskGraph & getGraphRef() { return _tg; }

	virtual void Print();
	virtual void PrintOtherParameters() { }

protected:

	TaskGraph _tg;

	ExpProcSet _Procs;
	ExpBusSet _Buses;
	ExpLatencySet _Latencies;

	ProblemPtr _ref;
	DiscProblemPtr _disc_ref;

	bool _initialized;

private:
};

class ExpProc : public boost::enable_shared_from_this< ExpProc >, public ExpNWSubgraph
{
public:

	ExpProc();
	ExpProc( DiscProcPtr proc, ExpProblemPtr parent ) : 
		_parent( parent ), _ref( proc->getRef() ), _disc_ref( proc ), 
		ExpNWSubgraph( parent->getGraphRef() ) { }
	virtual ~ExpProc() { }

	void Setup();

	virtual const ExpProblemPtr parent() { return _parent; }

	void AddExpTask( DiscTaskPtr p );
	void AddExpProcMsg( DiscProcMsgPtr p );

	ExpTaskSet getExpTasks() { return _Tasks; }
	ExpProcMsgSet getExpProcMsgs() { return _ProcMsgs; }

	virtual void Print();

	void AddSyncMsg( DiscSyncMsgPtr syncmsg );
	
	virtual void CalculateInstances( Ticks hpd );

protected:

	ExpProblemPtr	_parent;
	ExpTaskSet		_Tasks;
	ExpProcMsgSet	_ProcMsgs;
	ExpSyncMsgSet	_SyncMsgs;

	ProcPtr _ref;
	DiscProcPtr _disc_ref;

};

class ExpBus : public boost::enable_shared_from_this< ExpBus >,
	public ExpNWSubgraph
{
public:

	ExpBus();
	ExpBus( DiscBusPtr bus, ExpProblemPtr parent ) : 
		_parent( parent ), _ref( bus->getRef() ), _disc_ref( bus ), 
		ExpNWSubgraph( parent->getGraphRef() ) { }
	virtual ~ExpBus() { }

	void Setup(); 

	virtual const ExpProblemPtr parent() { return _parent; }

	virtual ExpMsgSet getExpMsgs() { return _Msgs; }
	virtual ExpSyncMsgSet getExpSyncMsgs() { return _SyncMsgs; }

	virtual void AddExpMsg( DiscMsgPtr p );
	virtual void AddExpSyncMsg( DiscSyncMsgPtr p );

	virtual void Print();
	
	virtual void CalculateInstances( Ticks hpd );

protected:

	ExpProblemPtr _parent;

	ExpSyncMsgSet	_SyncMsgs;
	ExpMsgSet		_Msgs;

	BusPtr _ref;
	DiscBusPtr _disc_ref;

private:
};

class ExpTask : public ExpNWVertex
{
public:

	ExpTask();
	ExpTask( DiscTaskPtr t, ExpProcPtr parent ) : 
		_parent( parent ), _ref( t->getRef() ), _disc_ref( t ) { }
	virtual ~ExpTask() { }

	virtual const ExpProcPtr parent() { return _parent; }

	virtual void Print();

	virtual void CalculateInstances( Ticks hpd );

protected:

	ExpProcPtr	_parent;

	TaskPtr		_ref;
	DiscTaskPtr	_disc_ref;
};

class ExpProcMsg : public ExpNWVertex
{
public:
	ExpProcMsg() { }
	ExpProcMsg( DiscProcMsgPtr p, ExpProcPtr parent ) : 
		_parent( parent ), _ref( p->getRef() ), _disc_ref( p ) { }

	virtual ~ExpProcMsg() { }

	virtual ExpProcPtr parent() { return _parent; }

	virtual void Print();

	virtual void CalculateInstances( Ticks hpd );

protected:

	ExpProcPtr _parent;

	ProcMsgPtr		_ref;
	DiscProcMsgPtr	_disc_ref;

private:
};

class ExpSyncMsg : public ExpNWVertex
{
public:
	ExpSyncMsg();
	ExpSyncMsg( DiscSyncMsgPtr s, ExpBusPtr parent ) : 
		_parent( parent ), _ref( s->getRef() ), _disc_ref( s ) { }
	virtual ~ExpSyncMsg() { }

	virtual const ExpBusPtr parent() { return _parent; }

	virtual void Print();

	virtual void CalculateInstances( Ticks hpd );

protected:

	ExpBusPtr		_parent;
	SyncMsgPtr		_ref;
	DiscSyncMsgPtr	_disc_ref;

private:
};

class ExpMsg : public ExpNWVertex
{
public:

	ExpMsg();
	ExpMsg( DiscMsgPtr m, ExpBusPtr parent ) :
		_parent( parent ), _ref( m->getRef() ), _disc_ref( m ) { }
	virtual ~ExpMsg() { }

	virtual const ExpBusPtr parent() { return _parent; }

	virtual void Print();
	
	virtual void CalculateInstances( Ticks hpd );

protected:

	ExpBusPtr	_parent;
	MsgPtr		_ref;
	DiscMsgPtr	_disc_ref;

private:

};

class ExpLatency
{
public:

	ExpLatency();
	ExpLatency( DiscLatencyPtr l, ExpProblemPtr parent ) : 
	_parent( parent ), _ref( l->getRef() ), _disc_ref( l ) { }
	virtual ~ExpLatency() { }

	const ExpProblemPtr parent() { return _parent; }

	virtual void Print();

	virtual void CalculateInstances( Ticks hpd ); // TODO: We may need something more sophisticated for latency

protected:

	ExpProblemPtr	_parent;

	LatencyPtr		_ref;
	DiscLatencyPtr	_disc_ref;

private:
};

#endif // ExpNWGraph_h__