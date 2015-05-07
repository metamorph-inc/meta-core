
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

double AbsTime( unsigned long long ticks, double res )
{
	return (double) ((double)ticks) * res;
}

SchedModel::SchedModel() : _resolution( -1.0 )
{
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

void SchedModel::SetResolution( double t )
{
	_resolution = t;
}

void SchedModel::AddProc( Proc & p )
{
	_procmap[ p.Name() ] = p;
}

void SchedModel::AddTask( std::string procname, Task & t )
{
	ostringstream oss;
	oss << procname << "/" << t.Name();
	_taskmap[ oss.str() ] = t;
	_proc_taskmap[ procname ].push_back( oss.str() );
}

void SchedModel::AddBus( Bus & b )
{
	_busmap[ b.Name() ] = b;
}

void SchedModel::AddMsg( std::string busname, Msg & m )
{
	ostringstream oss;
	oss << busname << "/" << m.Name();
	_msgmap[ oss.str() ] = m;
	_bus_msgmap[ busname ].push_back( oss.str() );
}

void SchedModel::AddLatency( Latency & l )
{
	_latencies.push_back( l );
}

// Factory method (or null if no such model)
SchedModel * SchedModel::GetSchedModel( std::string & ModName )
{
	if ( ModName == "Default" || ModName == "default" || ModName == "DEFAULT" )
		return (SchedModel * ) new DefaultSchedModel();
	else if ( ModName == "Debug" || ModName == "debug" || ModName == "DEBUG" )
		return (SchedModel *) new DebugSchedModel();
	else
		return (SchedModel * ) NULL;
}

// Template Pattern Executor
void SchedModel::Run()
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
	}
	else
	{
		// No results -- tell the user
		cout << "Invalid results -- please check your spec and try again." << endl;
	}
}

std::string SchedModel::getRootName(const std::string & name)
{
	string rt = name.substr(0, name.find('/'));
	return rt;
}