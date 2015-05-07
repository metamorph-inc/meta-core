// Scheduling Model Interface

#ifndef _SCHED_MODEL_H_
#define _SCHED_MODEL_H_

#include <map>
#include <vector>
#include <sstream>

class SolverAdapter; // forward reference

class Proc
{
public:
	Proc() { }

	Proc( const std::string & name, double speed, double sndohd, double rcvohd ) :
	  _name( name ), _speed( speed ), _sndohd( sndohd ), _rcvohd( rcvohd ) { }

	Proc( const Proc & p ) :
	  _name( p.Name() ), _speed( p.Speed() ), _sndohd( p.SendOverhead() ), _rcvohd( p.RecvOverhead()) { }

	Proc & operator=( const Proc & p ) {
	  _name = p.Name();
	  _speed = p.Speed();
	  _sndohd = p.SendOverhead();
	  _rcvohd = p.RecvOverhead();
	  return *this;
	}

	const std::string Name() const { return _name; }
	double Speed() const { return _speed; }
	double SendOverhead() const { return _sndohd; }
	double RecvOverhead() const { return _rcvohd; }

private:
	std::string _name;
	double _speed; // Hz
	double _sndohd; // seconds
	double _rcvohd; // seconds
};

class Task
{
public:
	Task() : _offset( -1.0 ) { }
	Task( const std::string & name, const std::string & proc,
		  double freq, double wcet, bool strict=false ) :
		_name( name ), _proc( proc ), _freq( freq ), _wcet( wcet ), 
			_offset( -1.0), _strict( strict ) { }

	Task( const Task & t ) :
		_name(t.Name()), _proc(t.Proc()), _freq(t.Freq()), _wcet(t.WCET()), 
			_strict(t.Strict()), _offset(t.Offset()), _taskinstlist(t.Instances()) { }

	Task & operator=( const Task & t ) {
		_name = t.Name();
		_proc = t.Proc();
		_freq = t.Freq();
		_wcet = t.WCET();
		_strict = t.Strict();
		_offset = t.Offset();
		_taskinstlist = t.Instances();
		return *this;
	}

	void AddOffset( double ot ) { _offset = ot; }
    void AddInstances( const std::vector< std::string > & taskinstlist ) { _taskinstlist = taskinstlist; }

	const std::string Name() const { return _name; }
	const std::string LongName() const { std::ostringstream o; o << _proc << "/" << _name; return o.str(); }
	const std::string Proc() const { return _proc; }
	double Freq() const { return _freq; }
	double WCET() const { return _wcet; }
	double Offset() const { return _offset; }
	bool Strict() const { return _strict; }

	const std::vector< std::string > & Instances() const { return _taskinstlist; }

private:
	std::string _name;
	std::string _proc;
	double _freq; // task execution frequency Hz
	double _wcet; // seconds
	bool _strict; // interpret freq spec as strict
	double _offset;
	std::vector< std::string > _taskinstlist;
};

class Bus
{
public:
	Bus() { }
	Bus( const std::string & name, double speed, double gap, bool sync=true ) :
	  _name( name ), _speed( speed ), _gap( gap ), _synchronous( sync ) { }

	Bus( const Bus & b ) :
	  _name(b.Name()), _speed(b.Speed()), _gap(b.Gap()), _busprocs( b.BusProcs() ), _synchronous(b.Synchronous()) { }

	Bus & operator=( const Bus & b ) {
		_name = b.Name();
		_speed = b.Speed();
		_gap = b.Gap();
		_busprocs.clear();
		_busprocs = b.BusProcs();
		_synchronous = b.Synchronous();
		return *this;
	}

	void AddBusProc ( std::string & procname ) { _busprocs.push_back( procname ); }

	const std::string Name() const { return _name; }
	bool Synchronous() const { return _synchronous; }
	double Speed() const { return _speed; }
	double Gap() const { return _gap; }
	const std::vector< std::string > & BusProcs() const { return _busprocs; }
private:
	std::string _name;
	double _speed; // bus data rate in bits/second
	double _gap;   // time gap in seconds
	bool _synchronous;
	std::vector< std::string > _busprocs;
};

class Msg
{
public:
	Msg() : _offset(-1.0) { }
	Msg( const std::string & name, const std::string & bus,
		double length, const std::string & sndtask ) :
		_name( name ), _bus( bus ), _length( length ), _sndtask( sndtask ), _offset(-1.0) { }

	Msg( const Msg & m ) :
		_name(m.Name()), _bus(m.Bus()), _length(m.Length()), _sndtask(m.SndTask()), 
			_rcvtasks(m.RcvTasks()), _offset(m.Offset()), _msginstlist(m.Instances()) { }

	Msg & operator= ( const Msg & m ) {
		_name = m.Name();
		_bus = m.Bus();
		_length = m.Length();
		_sndtask = m.SndTask();
		_rcvtasks = m.RcvTasks();
		_offset = m.Offset();
		_msginstlist = m.Instances();
		return *this;
	}

	void AddRcvr( const std::string & rcvtask ) { _rcvtasks.push_back( rcvtask ); }
	void AddOffset( double ot ) { _offset = ot; }
	void AddInstances( const std::vector< std::string > & msginstlist ) { _msginstlist = msginstlist; }

	const std::string Name() const { return _name; }
	const std::string LongName() const { std::ostringstream o; o << _bus << "/" << _name; return o.str(); }
	const std::string Bus() const { return _bus; }
	double Length() const { return _length; }
	const std::string SndTask() const { return _sndtask; }
	const std::vector< std::string > & RcvTasks() const { return _rcvtasks; }
	double Offset() const { return _offset; }
	const std::vector< std::string > & Instances() const { return _msginstlist; }
	

private:
	std::string _name;
	std::string _bus;
	double _length;
	std::string _sndtask;
	std::vector< std::string > _rcvtasks;
	double _offset;
	std::vector< std::string > _msginstlist;
};

class SyncMsg
{
public:
	SyncMsg() { }
	SyncMsg( const std::string & name, const std::string & bus,
		double synctime, double posmin, double posmax ) :
		_name( name ), _bus( bus ), _synctime( synctime ),
		_startposmin( posmin ), _startposmax( posmax ) { }

	SyncMsg( const SyncMsg & m ) :
		_name(m.Name()), _bus(m.Bus()), _synctime(m.SyncTime()),
		_startposmin(m.StartPosMin()), _startposmax(m.StartPosMax()) {}

	SyncMsg & operator= ( const SyncMsg & m ) {
		_name = m.Name();
		_bus = m.Bus();
		_synctime = m.SyncTime();
		_startposmin = m.StartPosMin();
		_startposmax = m.StartPosMax();
		return *this;
	}

	void SetPosMax( double posmax ) { _startposmax = posmax; }

	const std::string Name() const { return _name; }
	const std::string LongName() const { std::ostringstream o; o << _bus << "/" << _name; return o.str(); }
	const std::string Bus() const { return _bus; }
	double SyncTime() const { return _synctime; }
	double StartPosMin() const { return _startposmin; }
	double StartPosMax() const { return _startposmax; }

private:
	std::string _name;
	std::string _bus;
	double _synctime;
	double _startposmin;
	double _startposmax;
};

class Latency
{
public:
	Latency() { }
	Latency( const std::string & sndtask, const std::string & rcvtask, double latency ) :
	  _sndtask( sndtask ), _rcvtask( rcvtask ), _latency( latency ) { }

	Latency( const Latency & l ) :
	  _sndtask(l.SndTask()), _rcvtask(l.RcvTask()), _latency(l.LatencyVal()) { }

	Latency & operator=( const Latency & l ) {
		_sndtask = l.SndTask();
		_rcvtask = l.RcvTask();
		_latency = l.LatencyVal();
		return *this;
	}

	const std::string SndTask() const { return _sndtask; }
	const std::string RcvTask() const { return _rcvtask; }
	double LatencyVal() const { return _latency; }

private:
	std::string _sndtask;
	std::string _rcvtask;
	double _latency;
};

// GCD/LCM/Resolution Utility functions
unsigned long long GCD( unsigned long long a, unsigned long long b );
unsigned long long LCM( unsigned long long a, unsigned long long b );

unsigned long long Ticks( double val, double res );
long long STicks( double val, double res );
double AbsTime( unsigned long long ticks, double res );

class SchedResults
{
	// Nothing to see here!
	virtual void DoSomething() {}
};

class SchedModel
{
public:
	SchedModel();
	virtual ~SchedModel() { }

	bool GetBusByName( const std::string & busname, Bus & b );
	bool GetTaskByName( const std::string & taskname, Task & t ); // Use Proc/Task notation
	bool GetMsgByName( const std::string & msgname, Msg & m );    // Use Bus/Msg notation
	bool GetSyncMsgByName( const std::string & msgname, SyncMsg & m );
	void UpdateTask( Task t ) { _taskmap[ t.LongName() ] = t; } // Overwrite
	void UpdateMsg( const Msg & m ) { _msgmap[ m.LongName() ] = m; }  
	void UpdateSyncMsg( const SyncMsg & m ) { _syncmsgmap[ m.LongName() ] = m; }
	void UpdateBus( const Bus & b ) { _busmap[ b.Name() ] = b; }

	// API for building models
	void SetResolution( double t );
	void AddProc( const Proc & p );
	void AddTask( std::string procname, const Task & t );
	void AddLocalMsg( std::string procname, const Msg & m );
	void AddBus( const Bus & b );
	void AddMsg( std::string busname, const Msg & m );
	void AddSyncMsg( std::string busname, const SyncMsg & sm );
	void AddLatency( const Latency & l );

	// Factory method (or null if no such model)
	static SchedModel * GetSchedModel( std::string & ModName );
	virtual SchedResults * GetResultsObject() = 0;

	int Run();
	// Template Pattern: calling Run() invokes each of these virtual methods in sequence
	virtual void CalcHyperperiod() = 0;
	virtual void BuildConstraints() = 0;
	virtual void InvokeSolver() = 0;
	virtual bool CheckResults() = 0;
	//virtual void CollectResults( std::map< std::string, std::map< double, std::string > > & schedule ) = 0;
	virtual void CollectResults( SchedResults * rslt ) = 0;
	//virtual void OutputResults( std::map< std::string, std::map< double, std::string > > & schedule ) = 0;
	virtual void OutputResults( SchedResults * rslt ) = 0;

protected:
	std::string getRootName(const std::string & name);  // cut a task/msg name at the '/' mark (return left side)
	

	// Generally, these fields all need to be available to any subclasses
	std::map< std::string, Proc > _procmap; // proc name->object
	std::map< std::string, Bus > _busmap; // bus name->object
	std::map< std::string, Task > _taskmap; // qualified task names->task objects
	std::map< std::string, Msg > _msgmap; // qualified msg names->msg objects
	std::map< std::string, SyncMsg > _syncmsgmap; // qualified msg names->msg objects
	std::map< std::string, std::vector< std::string > > _proc_taskmap; // procname->task names
	std::map< std::string, std::vector< std::string > > _proc_msgmap; // procname->local msg names
	std::map< std::string, std::vector< std::string > > _bus_msgmap; // busname->msg names
	std::map< std::string, std::vector< std::string > > _bus_syncmsgmap; // busname->sync msg names

	std::vector< Latency > _latencies;

	double _resolution;
	double _hyperperiod;
	unsigned long long _ticks;

	SolverAdapter * _solver;
};


#endif // _SCHED_MODEL_H_
