#ifndef DiscNWModel_h__
#define DiscNWModel_h__

// Discretized version of the network model

// construct from a network model
#include "NWModel.h"

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/make_shared.hpp>

#include <set>
#include <map>
#include <string>

// Data Types

typedef unsigned long long Ticks;

class DiscProblem;
class DiscProc;
class DiscBus;
class DiscTask;
class DiscProcMsg;
class DiscSyncMsg;
class DiscMsg;
class DiscLatency;

typedef boost::shared_ptr< DiscProblem > DiscProblemPtr;
typedef boost::shared_ptr< DiscProc > DiscProcPtr;
typedef std::set< DiscProcPtr > DiscProcSet;
typedef boost::shared_ptr< DiscBus > DiscBusPtr;
typedef std::set< DiscBusPtr > DiscBusSet;
typedef boost::shared_ptr< DiscTask > DiscTaskPtr;
typedef std::set< DiscTaskPtr > DiscTaskSet;
typedef boost::shared_ptr< DiscProcMsg > DiscProcMsgPtr;
typedef std::set< DiscProcMsgPtr > DiscProcMsgSet;
typedef boost::shared_ptr< DiscSyncMsg > DiscSyncMsgPtr;
typedef std::set< DiscSyncMsgPtr > DiscSyncMsgSet;
typedef boost::shared_ptr< DiscMsg > DiscMsgPtr;
typedef std::set< DiscMsgPtr > DiscMsgSet;
typedef boost::shared_ptr< DiscLatency > DiscLatencyPtr;
typedef std::set< DiscLatencyPtr > DiscLatencySet;

class DiscProblem : public PrintableElement,
	public boost::enable_shared_from_this< DiscProblem >
{
public:
	DiscProblem() : _initialized(false) { }
	DiscProblem( ProblemPtr p )  : _ref( p ), _initialized(false) { }
	~DiscProblem() { }

	void Setup();  // We had to move the tree copy from the constructor,
	               // since shared_from_this is not initialized
	               // until the end of the constructor call
	ProblemPtr getRef() { assert( _initialized ); return _ref; }

	void AddDiscProc( ProcPtr p, std::map< std::string, DiscTaskPtr > & discTasks );
	void AddDiscBus( BusPtr p, std::map< std::string, DiscTaskPtr > & discTasks );
	void AddDiscLatency( LatencyPtr p );

	DiscProcSet getDiscProcs() { return _Procs; }
	DiscBusSet getDiscBuses() { return _Buses; }
	DiscLatencySet getDiscLatencies() { return _Latencies; }

	static Ticks Time2Ticks( double tm, double res, Ticks granularity = 1 );

	void DiscretizeModel();
	void CalcHyperperiod();

	Ticks getSystemHyperperiodTicks() { return _hyperperiodTicks; }
	//void CreateExpandedTaskGraph();

	virtual void Print();

protected:

	void PrintOtherParameters() { }

	Ticks calculateLCM( const std::set< Ticks > & periods );
	Ticks calculateLCM( Ticks a, Ticks b  );

	void setSystemHyperperiod( Ticks hyperperiodticks );
	
	DiscProcSet _Procs;
	DiscBusSet _Buses;
	DiscLatencySet _Latencies;

	Ticks _hyperperiodTicks;

	ProblemPtr _ref;

	bool _initialized;

private:
};

class DiscGranularity
{
public:
	DiscGranularity() { }

	void setGranularityTicks( Ticks granularity ) { _granularityTicks = granularity; }
	Ticks getGranularityTicks() { return _granularityTicks; }

protected:
	Ticks _granularityTicks;

};

class DiscProc : public boost::enable_shared_from_this< DiscProc >,
	public DiscGranularity
{
public:

	DiscProc();
	DiscProc( ProcPtr proc, DiscProblemPtr parent ) : _parent( parent ), _ref( proc ) { }
	virtual ~DiscProc() { }

	void Setup( std::map< std::string, DiscTaskPtr > & discTasks );
	ProcPtr getRef() { return _ref; }

	virtual const DiscProblemPtr parent() { return _parent; }

	DiscTaskPtr AddDiscTask( TaskPtr p );
	void AddDiscProcMsg( ProcMsgPtr p, DiscTaskPtr dt );

	DiscTaskSet getDiscTasks() { return _Tasks; }
	DiscProcMsgSet getDiscProcMsgs() { return _ProcMsgs; }

	virtual void Print();

	void AddSyncMsg( SyncMsgPtr syncmsg );
	void DiscretizeProc( double res );

protected:

	DiscProblemPtr _parent;
	DiscTaskSet _Tasks;
	DiscProcMsgSet _ProcMsgs;
	DiscSyncMsgSet _SyncMsgs;

	ProcPtr _ref;

};

class DiscBus : public boost::enable_shared_from_this< DiscBus >,
	public DiscGranularity
{
public:

	DiscBus();
	DiscBus( BusPtr bus, DiscProblemPtr parent ) : 
		_parent( parent ), _ref( bus ) { }
	virtual ~DiscBus() { }

	void Setup( std::map<std::string, DiscTaskPtr > & discTasks ); 
	BusPtr getRef() { return _ref; }

	virtual const DiscProblemPtr parent() { return _parent; }

	virtual DiscMsgSet getDiscMsgs() { return _Msgs; }
	virtual DiscSyncMsgSet getDiscSyncMsgs() { return _SyncMsgs; }

	virtual void AddDiscMsg( MsgPtr p, std::map< std::string, DiscTaskPtr > & discTasks );
	virtual void AddDiscSyncMsg( SyncMsgPtr p );

	virtual void Print();
	void DiscretizeBus( double res );
	
protected:

	DiscProblemPtr _parent;
	DiscSyncMsgSet _SyncMsgs;
	DiscMsgSet _Msgs;

	BusPtr _ref;

private:
};

class DiscTask
{
public:

	DiscTask();
	DiscTask( TaskPtr t, DiscProcPtr parent ) : _parent( parent ), _ref( t ) { }
	virtual ~DiscTask() { }

	virtual const DiscProcPtr parent() { return _parent; }
	TaskPtr getRef() { return _ref; }

	virtual Ticks getPeriodTicks() { return _period_ticks; }
	virtual Ticks getDurationTicks() { return _duration_ticks; }

	virtual void Print();
	virtual void DiscretizeTask( double res );
	
protected:

	DiscProcPtr _parent;
	TaskPtr		_ref;
	Ticks		_period_ticks;
	Ticks		_duration_ticks;
};


class DiscProcMsg
{
public:
	DiscProcMsg() { }
	DiscProcMsg( ProcMsgPtr p, DiscProcPtr parent, DiscTaskPtr dt ) : 
		_parent( parent ), _ref( p ), _sending_task( dt ) { }

	virtual ~DiscProcMsg() { }

	virtual DiscProcPtr parent() { return _parent; }
	ProcMsgPtr getRef() { return _ref; }
	DiscTaskPtr getDiscSender() { return _sending_task; }

	virtual void Print();

	Ticks getMsgSizeTicks() { return _msgsize_ticks; }
	void DiscretizeProcMsg( double res );
	
protected:

	DiscProcPtr _parent;
	ProcMsgPtr	_ref;
	DiscTaskPtr	_sending_task;

	Ticks		_msgsize_ticks;

private:
};



class DiscSyncMsg
{
public:
	DiscSyncMsg();
	DiscSyncMsg( SyncMsgPtr s, DiscBusPtr parent ) : 
		_parent( parent ), _ref( s ) { }
	virtual ~DiscSyncMsg() { }

	virtual const DiscBusPtr parent() { return _parent; }
	SyncMsgPtr getRef() { return _ref; }

	virtual void Print();

	Ticks getMsgSizeTicks() { return _msgsize_ticks; }
	Ticks getMaxPosTicks() { return _msg_pos_max_ticks; }
	Ticks getMinPosTicks() { return _msg_pos_min_ticks; }
	void DiscretizeSyncMsg( double res );
	
protected:

	DiscBusPtr _parent;
	SyncMsgPtr _ref;

	Ticks _msgsize_ticks;
	Ticks _msg_pos_min_ticks;
	Ticks _msg_pos_max_ticks;

private:
};

class DiscMsg
{
public:

	DiscMsg();
	DiscMsg( MsgPtr m, DiscBusPtr parent, DiscTaskPtr dt ) :
		_parent( parent ), _ref( m ), _sending_task( dt ) { }
	virtual ~DiscMsg() { }

	virtual const DiscBusPtr parent() { return _parent; }
	MsgPtr getRef() { return _ref; }
	DiscTaskPtr getDiscSender() { return _sending_task; }

	virtual void Print();

	virtual Ticks getMsgSizeTicks() { return _msgsize_ticks; }
	virtual void DiscretizeMsg( double res );
	
protected:

	DiscBusPtr	_parent;
	MsgPtr		_ref;
	DiscTaskPtr	_sending_task;

	Ticks _msgsize_ticks;

private:
	
};



class DiscLatency
{
public:

	DiscLatency();
	DiscLatency( LatencyPtr l, DiscProblemPtr parent ) : 
		_parent( parent ), _ref( l ) { }
	virtual ~DiscLatency() { }

	const DiscProblemPtr parent() { return _parent; }
	LatencyPtr getRef() { return _ref; }

	Ticks getLatencyTicks() { return _latency_ticks; }

	virtual void Print();

	void DiscretizeLatency( double res );
	
protected:

	DiscProblemPtr	_parent;
	LatencyPtr		_ref;

	Ticks _latency_ticks;

private:
};

#endif // DiscNWModel_h__