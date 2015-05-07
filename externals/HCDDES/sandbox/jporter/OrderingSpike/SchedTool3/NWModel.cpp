
#include "NWModel.h"

#include <string>
#include <iostream>
#include <sstream>
#include <cmath>

using namespace std;

void Problem::Run()
{
	//CalcFundamentalResolution();
	//CalcHyperperiod();
	CreateExpandedTaskGraph();
	InitializeLatencyMgrs();
}

// The resolution specified in the problem definition is a minimum
// We don't want to use that value unless something in the network has a lower resolution.
// If nothing in the network has a lower resolution, use the minimum resolution from 
// all of the network elements.
void Problem::CalcFundamentalResolution()
{
	set< double > resolutions;

	for ( ProcSet::iterator procIter = _Procs.begin(); procIter != _Procs.end(); procIter++ )
	{
		if ( (*procIter)->getResolution() >= getMinimumResolution() )
			resolutions.insert( (*procIter)->getResolution() );
	}

	for ( BusSet::iterator busIter = _Buses.begin(); busIter != _Buses.end(); busIter++ )
	{
		if ( (*busIter)->getResolution() >= getMinimumResolution() )
			resolutions.insert( (*busIter)->getResolution() );
	}

	if ( resolutions.empty() )
		resolutions.insert( getMinimumResolution() );

	setSystemResolution( *resolutions.begin() ); // should be min
}

void Problem::CreateExpandedTaskGraph()
{
	throw std::exception("The method or operation is not implemented.");
}

void Problem::InitializeLatencyMgrs()
{
	throw std::exception("The method or operation is not implemented.");
}


void Problem::Print()
{
	cout << "Problem " << Name() << ":" << endl;
	cout << "    system minimum resolution " << getSystemResolution() << " seconds" << endl;
	for ( ProcSet::iterator procIter = _Procs.begin(); procIter != _Procs.end(); procIter++ )
	{	
		(*procIter)->Print();
	}
	cout << endl;

	for ( BusSet::iterator busIter = _Buses.begin(); busIter != _Buses.end(); busIter++ )
	{
		(*busIter)->Print();
	}
	cout << endl;

	cout << "    Latencies:" << endl;
	for ( LatencySet::iterator latIter = _Latencies.begin(); latIter != _Latencies.end(); latIter++ )
	{
		(*latIter)->Print();
	}
	cout << endl;
}

ProcPtr Problem::getProcByName( const string & procname )
{
	for ( ProcSet::iterator procIter = _Procs.begin(); procIter != _Procs.end(); procIter++ )
	{
		std::string nm( (*procIter)->Name() );
		if( procname.compare( (*procIter)->Name() ) == 0 )
		{
			return (*procIter);
		}
	}

	return ProcPtr();
}

TaskPtr Problem::getTaskByName( const std::string & procname, const std::string & taskname )
{
	ProcPtr proc = getProcByName( procname );
	TaskPtr task = proc->getTaskByName( taskname );

	return task;	
}

MsgPtr Problem::getMsgByName( const std::string & busname, const std::string & msgname )
{
	BusPtr bus = getBusByName( busname );
	MsgPtr msg = bus->getMsgByName( msgname );

	return msg;
}

BusPtr Problem::getBusByName( const std::string & busname )
{
	for ( BusSet::iterator busIter = _Buses.begin(); busIter != _Buses.end(); busIter++ )
	{
		if ( busname.compare( (*busIter)->Name() ) == 0 )
		{
			return *busIter;
		}
	}

	return BusPtr();
}

void Problem::Setup()
{
	CalcFundamentalResolution(); 

	_initialized = true;

	for ( ProcSet::iterator procIter = _Procs.begin(); procIter != _Procs.end(); procIter++ )
	{	
		(*procIter)->setSystemResolution( getSystemResolution() );
	}

	for ( BusSet::iterator busIter = _Buses.begin(); busIter != _Buses.end(); busIter++ )
	{
		(*busIter)->setSystemResolution( getSystemResolution() );
	}
}

void Proc::Print()
{
	cout << "    " << "Proc " << Name() << ": " << endl;
	cout << "        " << "resolution " << getResolution() << " schedule " << getSchedType();
	PrintOtherParameters();
	cout << endl;

	for ( TaskSet::iterator taskIter = _Tasks.begin(); taskIter != _Tasks.end(); taskIter++ )
	{
		(*taskIter)->Print();
	}

	cout << "      SyncMsgs:" << endl;
	for( SyncMsgSet::iterator smsgIter = _SyncMsgs.begin(); smsgIter != _SyncMsgs.end(); smsgIter++ )
	{
		(*smsgIter)->Print();
	}

	cout << "      Local ProcMsgs:" << endl;
	for ( ProcMsgSet::iterator pmsgIter = _ProcMsgs.begin(); pmsgIter != _ProcMsgs.end(); pmsgIter++ )
	{
		(*pmsgIter)->Print();
	}
}

TaskPtr Proc::getTaskByName( const string & taskname )
{
	for ( TaskSet::iterator taskIter = _Tasks.begin(); taskIter != _Tasks.end(); taskIter++ )
	{
		if ( taskname.compare( (*taskIter)->Name() ) == 0 )
		{
			return *taskIter;
		}
	}

	return TaskPtr(); // uninitialized <--> null
}

void Proc::AddSyncMsg( SyncMsgPtr syncmsg )
{
	_SyncMsgs.insert( syncmsg );
}

Task::Task( const std::string & name, ProcPtr parent, double freq, double dur ) : 
	SoftwareElement( name ), _parent( parent ), _frequency( freq ), _duration( dur )
{ 
	
	if ( fabsl(freq) < 1e-15 ) 
	{
		ostringstream oss;
		oss << "Frequency for task " << name << " effectively zero (f < 1e-15).";
		throw std::exception(oss.str().c_str());  
	}

	_period = ( 1.0 / _frequency );
}

void Task::Print()
{
	cout << "        " << TaskType() << " " << Name();
	cout << "            " << "frequency " << getFrequency() << " period " << getPeriod() << " duration " << getDuration();
	PrintOtherParameters();
	cout << endl;

}

void Bus::AddBusProc( const string & procname )
{
	_Procs.insert( _parent->getProcByName( procname ) );
}

void Bus::AddSyncMsg( SyncMsgPtr syncmsg )
{
	_SyncMsgs.insert( syncmsg );
}

TaskPtr Bus::getTaskByName( const std::string & sndproc, const std::string & sndtask )
{
	return _parent->getTaskByName( sndproc, sndtask );
}

MsgPtr Bus::getMsgByName( const std::string & msgname )
{
	for ( MsgSet::iterator msgIter = _Msgs.begin(); msgIter != _Msgs.end(); msgIter++ )
	{
		if ( msgname.compare( (*msgIter)->Name() ) == 0 )
		{
			return *msgIter;
		}
	}

	return MsgPtr();
}

void Bus::Print()
{
	cout << "    " << BusType() << " " << Name() << endl;
	cout << "        " << "resolution " << getResolution() << " data rate " << getDataRatebps();
	PrintOtherParameters(); 
	cout << endl;
	for ( MsgSet::iterator msgIter = _Msgs.begin(); msgIter != _Msgs.end(); msgIter++ )
	{
		(*msgIter)->Print();
	}
}

double Bus::getMsgTransferTime( Bytes msgsize )
{
	double trans = ( (double) msgsize ) / ( getDataRatebps() * 8.0 );
	return getSetupTime() + trans;
}

void Msg::Print()
{
	cout << "        " << "Msg " << Name() << endl;
	cout << "            " << "sending task " << _sending_task->Name() << " msg size " << getMsgSize() << endl;
	cout << "            " << "receiving tasks ";
	for ( TaskSet::iterator taskIter = _receiving_tasks.begin(); taskIter != _receiving_tasks.end(); taskIter++ )
	{
		cout << (*taskIter)->Name() << " ";
	}
	cout << endl;
}

void Msg::AddReceivingTask( const std::string & receiving_proc, 
							const std::string & receiving_task )
{
	TaskPtr task = _parent->getTaskByName( receiving_proc, receiving_task );
	_receiving_tasks.insert( task );
}

void Msg::PrintOtherParameters()
{
	cout << " message size " << getMsgSize() << " transfer time " << getTransferTime();
}

double Msg::getTransferTime()
{
	double trans = _parent->getMsgTransferTime( getMsgSize() );
	return trans;
}

void ProcMsg::Print()
{
	cout << "        " << "ProcMsg " << Name() << endl;
	cout << "            " << "sending task " << _sending_task->Name() << " message size " << getMsgSize();
	cout << "            " << "receiving tasks ";
	for ( TaskSet::iterator taskIter = _receiving_tasks.begin(); taskIter != _receiving_tasks.end(); taskIter++ )
	{
		cout << (*taskIter)->Name() << " ";
	}
	cout << endl;
}

void ProcMsg::AddReceiver( const string & rcvtask )
{
	
	_receiving_tasks.insert( _parent->getTaskByName( rcvtask ) );
}

void ProcMsg::SetupParent( const std::string & sndtask )
{
	_parent->AddProcMsg( shared_from_this() ); 
	_sending_task = _parent->getTaskByName( sndtask );
}

void SyncMsg::Print()
{
	cout << "        " << "SyncMsg " << Name() << endl;
	cout << "            " << "bus " << _parent->Name();
	cout << " message size " << getMsgSize();
	cout << " min pos bound " << getMinPos() << " max pos bound " << getMaxPos() << endl;
}

void SyncMsg::SetupParent( BusPtr bus )
{
	_parent = bus; 
	if ( _parent.get() == NULL )
	{
		cout << "Error: Bus ptr is null!!!!";
	}
	_parent->AddSyncMsg( shared_from_this() );
}

double SyncMsg::getTransferTime()
{
	double trans = (double) getMsgSize() / ( _parent->getDataRatebps() * 8 );
	return _parent->getSetupTime() + trans;
}

