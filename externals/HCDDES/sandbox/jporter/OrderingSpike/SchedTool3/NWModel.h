#ifndef NWModel_h__
#define NWModel_h__

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/make_shared.hpp>

#include <set>

#include "NamedElement.h"
#include "PrintableElement.h"

// ====== Utility Classes and Types ==========================

typedef unsigned long long Bytes;


// ====== Network Model Classes ==============================

class Problem;
class Proc;
class Task;
class Bus;
class Msg;
class Latency;
class ProcMsg;
class SyncMsg;
class SoftwareElement;

typedef boost::shared_ptr< Problem > ProblemPtr;
typedef boost::shared_ptr< Proc > ProcPtr;
typedef std::set< ProcPtr > ProcSet;
typedef boost::shared_ptr< Task > TaskPtr;
typedef std::set< TaskPtr > TaskSet;
typedef boost::shared_ptr< Bus > BusPtr;
typedef std::set< BusPtr > BusSet;
typedef boost::shared_ptr< ProcMsg > ProcMsgPtr;
typedef std::set< ProcMsgPtr > ProcMsgSet;
typedef boost::shared_ptr< SyncMsg > SyncMsgPtr;
typedef std::set< SyncMsgPtr > SyncMsgSet;
typedef boost::shared_ptr< Msg > MsgPtr;
typedef std::set< MsgPtr > MsgSet;
typedef boost::shared_ptr< Latency > LatencyPtr;
typedef std::set< LatencyPtr > LatencySet;

typedef boost::shared_ptr< SoftwareElement > SoftwareElementPtr;

class Problem : public NamedElement, public PrintableElement
{
public:
	Problem() {  }
	Problem( const std::string & name ) : NamedElement( name ), _initialized(false) { }
	~Problem() { }

	void Setup();

	void AddProc( ProcPtr p ) { _Procs.insert(p); }
	void AddBus( BusPtr p ) { _Buses.insert(p); }
	void AddLatency( LatencyPtr p ) { _Latencies.insert(p); }

	ProcSet getProcs() { return _Procs; }
	BusSet getBuses() { return _Buses; }
	LatencySet getLatencies() { return _Latencies; }
	
	void Run();
	void CalcFundamentalResolution();
	
	void CreateExpandedTaskGraph();
	void InitializeLatencyMgrs();

	double getSystemResolution() { assert( _initialized); return _sysResolution; }
	void setMinimumResolution( double resolution ) { _minResolution = resolution; }

	ProcPtr getProcByName( const std::string & procname );
	BusPtr getBusByName( const std::string & busname );
	TaskPtr getTaskByName( const std::string & procname, const std::string & taskname );
	MsgPtr  getMsgByName( const std::string & busname, const std::string & msgname );

	virtual void Print();

protected:

	virtual void PrintOtherParameters() { }

	double getMinimumResolution() { return _minResolution; }
	void setSystemResolution( double resolution ) { _sysResolution = resolution; }

	ProcSet _Procs;
	BusSet _Buses;
	LatencySet _Latencies;

	double _minResolution;
	double _sysResolution;
	bool _initialized;

private:
};

class HardwareElement : public NamedElement
{
public:
	HardwareElement() { }
	HardwareElement( const std::string & name ) : 
		NamedElement( name ), _resolutionSecs(0.0), _systemResolutionSecs(0.0) { }
	~HardwareElement() { }

	virtual double getResolution() { 
			return std::max( _resolutionSecs, _systemResolutionSecs );
	}
	virtual void setResolution( double  resolutionSecs ) { _resolutionSecs = resolutionSecs; }
	virtual void setSystemResolution( double resolutionSecs ) { _systemResolutionSecs = resolutionSecs; }

protected:

	double _resolutionSecs;
	double _systemResolutionSecs;
	
private:
};

class Proc : public HardwareElement, public PrintableElement, 
				public boost::enable_shared_from_this< Proc >
{
public:

	Proc();
	Proc( const std::string & name, ProblemPtr parent ) :
		HardwareElement( name ), _parent( parent ) { }
	virtual ~Proc() { }

	virtual const ProblemPtr parent() { return _parent; }
	virtual void SetupParent() { _parent->AddProc( shared_from_this() ); }
	void AddTask( TaskPtr p ) { _Tasks.insert(p); }
	void AddProcMsg( ProcMsgPtr p ) { _ProcMsgs.insert(p); }
	
	TaskSet getTasks() { return _Tasks; }
	ProcMsgSet getProcMsgs() { return _ProcMsgs; }

	virtual void Print();

	void setSchedType( const std::string & schedtype ) { _schedType = schedtype; }
	TaskPtr getTaskByName( const std::string & taskname );
	void AddSyncMsg( SyncMsgPtr syncmsg );
	const std::string getSchedType() { return _schedType; }

protected:

	virtual void PrintOtherParameters() { }

	ProblemPtr _parent;
	TaskSet _Tasks;
	ProcMsgSet _ProcMsgs;
	SyncMsgSet _SyncMsgs;
	std::string _schedType;

private:
};

class Bus : public HardwareElement, public PrintableElement,
	public boost::enable_shared_from_this< Bus >
{
public:

	Bus();
	Bus( const std::string & name, ProblemPtr parent, double data_rate ) :
		HardwareElement( name ), _parent( parent ), _data_rate( data_rate ) { }
	virtual ~Bus() { }

	virtual void SetupParent() { _parent->AddBus( shared_from_this() ); }
	virtual const ProblemPtr parent() { return _parent; }
	virtual void AddMsg( MsgPtr p ) { _Msgs.insert(p); }

	//virtual void Print();
	virtual void AddBusProc( const std::string & procname );
	virtual void AddSyncMsg( SyncMsgPtr syncmsg );

	virtual MsgSet getMsgs() { return _Msgs; }
	virtual SyncMsgSet getSyncMsgs() { return _SyncMsgs; }
	virtual ProcSet getProcs() { return _Procs; }

	virtual TaskPtr getTaskByName( const std::string & sndproc, 
									const std::string & sndtask );
	virtual MsgPtr getMsgByName( const std::string & msgname );

	virtual double getDataRatebps() { return _data_rate; }
	virtual double getSetupTime() { return 0.0; } // TODO: this is a dummy until we get a more generic way to do this
	virtual double getMsgTransferTime( Bytes msgsize );

	virtual void Print();

protected:

	virtual void PrintOtherParameters() = 0;

	virtual const std::string BusType() = 0;

	ProblemPtr _parent;
	SyncMsgSet _SyncMsgs;
	MsgSet _Msgs;
	ProcSet _Procs;

	double _data_rate;

private:
};

class SoftwareElement : public NamedElement
{
public:

	SoftwareElement() { }
	SoftwareElement( const std::string & name ) : 
		NamedElement( name ), _duration_seconds(0.0) { }
	virtual ~SoftwareElement() { }	

protected:

	double _duration_seconds;

private:
};

class Task : public SoftwareElement, public PrintableElement, 
				public boost::enable_shared_from_this< Task >
{
public:

	Task();
	Task( const std::string & name, ProcPtr parent, double freq, double dur );
	virtual ~Task() { }

	virtual const ProcPtr parent() { return _parent; }
	virtual void SetupParent() { _parent->AddTask( shared_from_this() ); }

	virtual void Print();

	double getFrequency() { return _frequency; }
	double getPeriod() { return _period; }
	double getDuration() { return _duration; }

protected:

	virtual const std::string TaskType()=0;
	virtual void PrintOtherParameters()=0;

	ProcPtr _parent;

	double _frequency;
	double _period;
	double _duration;

private:
};


class ProcMsg : public SoftwareElement, public PrintableElement,
	public boost::enable_shared_from_this< ProcMsg >
{
public:
	ProcMsg() { }
	ProcMsg( const std::string & name, ProcPtr parent, Bytes msgsize ) : 
		SoftwareElement( name ), _parent( parent ), _msgsize( msgsize ) { }
			
	virtual ~ProcMsg() { }

	virtual void Print();
	virtual void SetupParent( const std::string & sndtask );
	void AddReceiver( const std::string & rcvtask );

	TaskPtr getSender() { return _sending_task; }

	Bytes getMsgSize() { return _msgsize; }

protected:

	virtual void PrintOtherParameters() { }

	ProcPtr _parent;

	Bytes _msgsize;

	TaskPtr _sending_task;
	TaskSet _receiving_tasks;

private:
};



class SyncMsg : public SoftwareElement, public PrintableElement,
	public boost::enable_shared_from_this< SyncMsg >
{
public:
	SyncMsg();
	SyncMsg( const std::string & name ) : 
		SoftwareElement( name ), _msg_pos_min( -1.0 ), _msg_pos_max( -1.0 ) { }
	virtual ~SyncMsg() { }

	void SetupParent( BusPtr bus );
	virtual const BusPtr parent() { return _parent; }
	virtual void setSize( Bytes msgsize ) { _msgsize = msgsize; }
	virtual void Print();
	void AddBounds( double msgposmin, double msgposmax ) { _msg_pos_min = msgposmin; _msg_pos_max = msgposmax; }

	Bytes getMsgSize() { return _msgsize; }
	double getMaxPos() { return _msg_pos_max; }
	double getMinPos() { return _msg_pos_min; }

	double getTransferTime();

protected:

	virtual void PrintOtherParameters() { }
	

	BusPtr _parent;

	Bytes _msgsize;
	double _msg_pos_min;
	double _msg_pos_max;

private:
};

class Msg : public SoftwareElement, public PrintableElement,
				public boost::enable_shared_from_this< Msg >
{
public:

	Msg();
	Msg( const std::string & name, BusPtr parent, Bytes msgsize ) :
		SoftwareElement( name ), _parent( parent ), _msg_size( msgsize ) { }
	virtual ~Msg() { }

	virtual const BusPtr parent() { return _parent; }
	virtual void SetupParent( const std::string & sndproc, const std::string & sndtask ) { 
		_parent->AddMsg( shared_from_this() );
		_sending_task = _parent->getTaskByName( sndproc, sndtask );
	}

	virtual void Print();
	
	virtual void AddReceivingTask( const std::string & receiving_proc, 
							const std::string & receiving_task );
	TaskPtr getSender() { return _sending_task; }

	virtual Bytes getMsgSize() { return _msg_size; }
	virtual double getTransferTime();

protected:

	virtual void PrintOtherParameters();
	
	BusPtr _parent;

	Bytes _msg_size;

	TaskPtr _sending_task;
	TaskSet _receiving_tasks;

private:
};

class ConstraintElement
{
public:

	ConstraintElement() { }
	~ConstraintElement() { }

protected:
	
private:
};

class Latency : public ConstraintElement, public PrintableElement, 
					public boost::enable_shared_from_this< Latency >
{
public:

	Latency();
	Latency( ProblemPtr parent, double latency ) : 
		_parent( parent ), _latency( latency ) { }
	virtual ~Latency() { }

	const ProblemPtr parent() { return _parent; }
	virtual void SetupParent() { _parent->AddLatency( shared_from_this() ); }

	virtual void Print() = 0;

	double getLatency() { return _latency; }

protected:

	ProblemPtr _parent;
	
	double _latency;

private:
};

#endif // NWModel_h__