
#include "schedModel.h"
#include "defaultSchedModel.h"
#include "debugSchedModel.h"

#include <iostream>
#include <sstream>

using namespace std;

// GCD/LCM Utility functions
unsigned long long GCD( unsigned long long a, unsigned long long b )
{
    unsigned long long r, atmp = a, btmp = b;
	// First find the GCD
	while( btmp > 0 )
	{
		r = atmp;
        atmp = btmp;
		btmp = r % btmp;
	}

	return atmp;
}

unsigned long long LCM( unsigned long long a, unsigned long long b )
{
    unsigned long long r, atmp = a, btmp = b;
	// First find the GCD
	while( btmp > 0 )
	{
		r = atmp;
        atmp = btmp;
		btmp = r % btmp;
	}

	// Then use it to get the LCM
	return ( a * b ) / atmp;
}

unsigned long long Ticks( double val, double res )
{
	return (unsigned long long) ((val / res) + 0.9999);
}

long long STicks( double val, double res )
{
	return (long long) ( (val < 0.0) ? ((val/res) - 0.9999) : ((val / res) + 0.9999) );
}

double AbsTime( unsigned long long ticks, double res )
{
	return (double) ((double)ticks) * res;
}

SchedModel::SchedModel() : _resolution( -1.0 )
{
}

bool SchedModel::GetBusByName( const std::string & busname, Bus & b )
{
	std::map< std::string, Bus >::iterator pBus = _busmap.find( busname );
	if ( pBus == _busmap.end() )
		return false;

	b = pBus->second;
	return true;
}

bool SchedModel::GetTaskByName( const std::string & taskname, Task & t )
{
	std::map< std::string, Task >::iterator pTask = _taskmap.find( taskname );
	if ( pTask == _taskmap.end() )
		return false;
	
	t = pTask->second;
	return true;
}

bool SchedModel::GetMsgByName( const std::string & msgname, Msg & m )
{
	std::map< std::string, Msg >::iterator pMsg = _msgmap.find( msgname );
	if ( pMsg == _msgmap.end() )
		return false;

	m = pMsg->second;
	return true;
}

bool SchedModel::GetSyncMsgByName( const std::string & msgname, SyncMsg & m )
{
	std::map< std::string, SyncMsg >::iterator pMsg = _syncmsgmap.find( msgname );
	if ( pMsg == _syncmsgmap.end() )
		return false;

	m = pMsg->second;
	return true;
}

void SchedModel::SetResolution( double t )
{
	_resolution = t;
}

void SchedModel::AddProc( const Proc & p )
{
	_procmap[ p.Name() ] = p;
}

void SchedModel::AddTask( std::string procname, const Task & t )
{
	ostringstream oss;
	oss << procname << "/" << t.Name();
	_taskmap[ oss.str() ] = t;
	_proc_taskmap[ procname ].push_back( oss.str() );
}

void SchedModel::AddLocalMsg( std::string procname, const Msg & m )
{
	ostringstream oss;
	oss << procname << "/" << m.Name();
	_msgmap[ oss.str() ] = m;
	_proc_msgmap[ procname ].push_back( oss.str() );

  
}

void SchedModel::AddBus( const Bus & b )
{
	_busmap[ string(b.Name()) ] = b;

}

void SchedModel::AddMsg( std::string busname, const Msg & m )
{
	ostringstream oss;
	oss << busname << "/" << m.Name();
	_msgmap[ oss.str() ] = m;
	_bus_msgmap[ busname ].push_back( oss.str() );
}

void SchedModel::AddSyncMsg( std::string busname, const SyncMsg & sm )
{
	ostringstream oss;
	oss << busname << "/" << sm.Name();
	_syncmsgmap[ oss.str() ] = sm;
	_bus_syncmsgmap[ busname ].push_back( oss.str() );
}

void SchedModel::AddLatency( const Latency & l )
{
	_latencies.push_back( l );
}

// Factory method (or null if no such model)
SchedModel * SchedModel::GetSchedModel( std::string & ModName )
{
	if ( ModName == "Default" || ModName == "default" || ModName == "DEFAULT" )
		return (SchedModel * ) new DefaultSchedModel();
//	else if ( ModName == "Debug" || ModName == "debug" || ModName == "DEBUG" )
//		return (SchedModel *) new DebugSchedModel();
	else
		return (SchedModel * ) NULL;
}

// Template Pattern Executor
int SchedModel::Run()
{
	CalcHyperperiod();
	BuildConstraints();
	InvokeSolver();
	if ( CheckResults() )
	{
		//std::map< std::string, std::map< double, std::string > > schedule; 
		SchedResults * rslt = this->GetResultsObject();
		CollectResults(rslt);
		OutputResults(rslt);
		return 0;
	}
	else
	{
		// No results -- tell the user
		cerr << "Invalid results -- please check your spec and try again." << endl;
		return 1;
	}
}

std::string SchedModel::getRootName(const std::string & name)
{
	string rt = name.substr(0, name.find('/'));
	return rt;
}
