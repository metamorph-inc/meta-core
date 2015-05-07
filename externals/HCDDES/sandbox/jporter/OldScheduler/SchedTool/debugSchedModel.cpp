
#include "ConfigKeeper.h"

#include "debugSchedModel.h"
#include "solverAdapter.h"
#include "cpSolver.h"

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

using namespace std;

DebugSchedModel::DebugSchedModel() : _debugAll(false), DefaultSchedModel()
{
	std::string solvername( "FDCO" );
	_solver = SolverAdapter::GetSolver( solvername );

	vector< string > elts = ConfigKeeper::inst()->getDebugOptions();
	_debugList.insert(elts.begin(), elts.end()); // Copy into something we can use to look it up
	if ( _debugList.find(string("all")) != _debugList.end() )
	{
		_debugAll = true;
	}
	else
	{
		// Add extra processors to complete the topology
		// First look up any buses specified in the debug list,:
		for( vector< string >::iterator pD = elts.begin(); pD != elts.end(); pD++ )
		{
			vector<string> msgs = _bus_msgmap[*pD]; // This will be empty if *pD is a processor
			for (vector<string>::iterator pM = msgs.begin(); pM != msgs.end(); pM++)
			{
				//  then get a list of tasks from their message list:  _msgmap[].SndTask(), .RcvTasks()
				Msg m = _msgmap[*pM];

				//  add all processors listed in those tasks:  parse the task name at the split '/'
				string proc = getRootName(m.SndTask()); // should give us the part with the proc
				if ( proc.size() > 0 )
					_debugList.insert(proc);

				vector<string> rtasks = m.RcvTasks();
				for (vector<string>::iterator pR = rtasks.begin(); pR != rtasks.end(); pR++ )
				{
					proc = getRootName(*pR); // should give us the part with the proc
					if ( proc.size() > 0 )
						_debugList.insert(proc);
				}
			}
		}
	}
}

DebugSchedModel::~DebugSchedModel()
{
}

// @todo: Factor this back into the default sched model -- we need some kind of
// generic way of enabling debug for different scheduling models.
void DebugSchedModel::BuildConstraints()
{
	unsigned long long total_instances = 0;
	// Make solver variables for each instance
	// Create ordering for each task
	// Create ordering for each processor
	std::map< std::string, std::vector< std::string > >::iterator pTaskList; // Iterate by processor
	std::map< std::string, std::vector< std::string > > task_instmap;
	for ( pTaskList = _proc_taskmap.begin(); pTaskList != _proc_taskmap.end(); pTaskList++ )
	{
		// If it's not in the debug list, then skip it (or else debug all)
		if ( (!_debugAll) && (_debugList.find(pTaskList->first) == _debugList.end()) )
			continue; 

		std::vector< std::string > procList; // Keep list of all instances on this processor
		std::vector< unsigned long long > lengthList; // Keep list of all task instance durations

		std::vector< std::string >::iterator pTask;
		for ( pTask = pTaskList->second.begin(); pTask != pTaskList->second.end(); pTask++ )
		{
			// Get the task in question
			Task t( _taskmap[ *pTask ] );
			std::vector< std::string > taskInstList;

			unsigned long long period = Ticks( 1.0 / t.Freq(), _resolution );
			unsigned long long ninst = _ticks / period;
			unsigned long long dur = Ticks( t.WCET(), _resolution );

			cout << "Task " << *pTask << " " << ninst << " instances " << dur << " ticks long" << endl;
			for ( unsigned long long count = 0; count < ninst; count++ )
			{	
				// Make the name for this instance
				ostringstream oss;
				oss << *pTask << "_" << count;
				
				_solver->AddVariable( oss.str() );
				taskInstList.push_back( oss.str() );
				procList.push_back( oss.str() );
				lengthList.push_back( dur );
			}
			task_instmap[ *pTask ] = taskInstList;
			_num_inst[ *pTask ] = taskInstList.size();
			total_instances += taskInstList.size();

			//if ( taskInstList.size() > 1 )
			_solver->OrderInstances( taskInstList, Ticks( 1.0 / t.Freq(), _resolution ), Ticks( t.WCET(), _resolution ) );
		}

		if ( procList.size() > 1 )
			_solver->SerializeInstances( procList, lengthList );
	}
	
	// Do the same procedure for buses
	std::map< std::string, std::vector< std::string > >::iterator pMsgList; // Iterate by bus
	for ( pMsgList = _bus_msgmap.begin(); pMsgList != _bus_msgmap.end(); pMsgList++ )
	{
		// If it's not in the debug list, then skip it (or debug all)
		if ( (!_debugAll) && ( _debugList.find(pMsgList->first) == _debugList.end()) )
			continue; 

		Bus b( _busmap[ pMsgList->first ] );
		double busrate = b.Speed();

		std::vector< std::string > busList; // Keep list of all instances on this bus
		std::vector< unsigned long long > lengthList; // Keep list of all msg instance durations

		std::vector< std::string >::iterator pMsg;
		for ( pMsg = pMsgList->second.begin(); pMsg != pMsgList->second.end(); pMsg++ )
		{
			// Get the message
			Msg m( _msgmap[ *pMsg ] );
			std::vector< std::string > msgInstList;


			// Now the tricky part -- # msgs = min( # sender instances, max (over tasks) ( # rcvr instances ) )
			std::vector< std::string > rcvrList( m.RcvTasks() );
			unsigned long long maxInstances = 1;
			std::vector< std::string >::iterator pRcvr;
			for ( pRcvr = rcvrList.begin(); pRcvr != rcvrList.end(); pRcvr++ )
			{
				//Task t( _taskmap[ *pRcvr ] );
				//unsigned long long period = Ticks( 1.0 / t.Freq(), _resolution );
				//unsigned long long ninst = _ticks / period;
				unsigned long long ninst = _num_inst[*pRcvr ];

				maxInstances = ( ninst > maxInstances ) ? ninst : maxInstances; // min
			}

			Task s( _taskmap[ m.SndTask() ] ); // Get the sender
			unsigned long long speriod = Ticks( 1.0 / s.Freq(), _resolution );
			//unsigned long long sinst = _ticks / speriod;
			unsigned long long sinst = _num_inst[ m.SndTask() ];
			unsigned long long minst = ( sinst < maxInstances ) ? sinst : maxInstances;
			
			//_num_inst[ m.Name() ] = minst;
			_num_inst[ *pMsg ] = minst;
			total_instances += minst;

			// TODO: Add bus uncertainty to this calculation
			unsigned long long dur = Ticks( m.Length()/busrate, _resolution );  // Scale msg length by bus rate
			

			cout << "Msg " << *pMsg << " " << minst << " instances " << dur << " ticks long" << endl;

			// Create msg instances & order them
			for (unsigned long long count = 0; count < minst; count++ )
			{
				ostringstream oss;
				oss << *pMsg << "_" << count;

				_solver->AddVariable( oss.str() );
				msgInstList.push_back( oss.str() );
				busList.push_back( oss.str() );
				lengthList.push_back( dur );
			}

			// order the instances -- use the sender duration, with the period rate difference ( #senders / #minst )-1 * period
			if ( msgInstList.size() > 1 )
			{
				_solver->OrderInstances( msgInstList, Ticks(s.WCET(),_resolution) + (((unsigned long long(sinst/minst))-1) * speriod ), 
										 Ticks( m.Length(), _resolution ), false);

			// Take the msg instances & serialize with the senders
				msgInstList.insert( msgInstList.end(), task_instmap[ m.SndTask() ].begin(), task_instmap[ m.SndTask() ].end() );
				std::vector< unsigned long long > msgInstSizes( msgInstList.size(), Ticks( m.Length(), _resolution ));
				msgInstSizes.insert( msgInstSizes.end(), _num_inst[ m.SndTask() ], Ticks( s.WCET(), _resolution ) );
				_solver->SerializeInstances( msgInstList, msgInstSizes );
			}

			// TODO: Think about this next one very carefully! 
			// Then serialize independently with each receiver (w/ rate adjustments) -- maybe not at all

		}

		// Serialize all of the instances on the bus
		if ( busList.size() > 1 )
			_solver->SerializeInstances( busList, lengthList );
	}

	// Now, what to do about latencies?
	// Latencies have to be a relationship between two task instances, so which two?
	std::vector< Latency >::iterator pLat;
	for ( pLat = _latencies.begin(); pLat != _latencies.end(); pLat++ )
	{
		// If either end is out of the debug list, then skip this constraint (or debug all)
		if ( (!_debugAll) && (_debugList.find( getRootName(pLat->SndTask())) == _debugList.end() 
			|| _debugList.find( getRootName(pLat->RcvTask())) == _debugList.end()) )
			continue;

		Task s( _taskmap[ pLat->SndTask() ] );
		Task r( _taskmap[ pLat->RcvTask() ] );
		
		double Speriod = 1.0 / s.Freq();
		double Rperiod = 1.0 / r.Freq();
		double lat = pLat->LatencyVal();

		if ( ((Speriod < Rperiod) && (lat < Speriod))
			|| ((Speriod > Rperiod) && (lat < Rperiod)) )
		{
			unsigned int minRcvrIdx = (unsigned int) floor( lat / Rperiod );
			unsigned int maxRcvrIdx = (unsigned int) floor( (lat+Speriod) / Rperiod );
			unsigned int minSndrIdx = (unsigned int) floor( lat / Speriod );
			unsigned int maxSndrIdx = (unsigned int) floor( (lat+Rperiod) / Speriod );

			// Make sure we're not out of bounds
			unsigned int rcvrsize = _num_inst[pLat->RcvTask()] - 1;
			unsigned int sndrsize = _num_inst[pLat->SndTask()] - 1; 
			minRcvrIdx = ( minRcvrIdx < 0) ? 0 : minRcvrIdx;
			maxRcvrIdx = ( maxRcvrIdx > rcvrsize) ? rcvrsize : maxRcvrIdx;
			minSndrIdx = ( minSndrIdx < 0) ? 0 : minSndrIdx;
			maxSndrIdx = ( maxSndrIdx > sndrsize) ? sndrsize : maxSndrIdx;

			std::vector< std::string > sndrnames;
			std::vector< std::string > rcvrnames;
			// Yes, there are n^2 constraints here, but hopefully n is very small
			for ( unsigned int ridx = minRcvrIdx; ridx <= maxRcvrIdx; ridx++ )
				for ( unsigned int sidx = minSndrIdx; sidx <= maxSndrIdx; sidx++ )
				{
					ostringstream sinst, rinst;
					sinst << s.Proc() << "/" << s.Name() << "_" << sidx;
					rinst << r.Proc() << "/" << r.Name() << "_" << ridx;
					sndrnames.push_back( sinst.str() );
					rcvrnames.push_back( rinst.str() );
				}

				_solver->AddLatencyBound( sndrnames, rcvrnames, Ticks( lat - r.WCET(), _resolution ));
		}
		else
		{
			cout << "Throwing away redundant latency constraint: ";
			cout << s.Name() << " " << r.Name() << " " << lat << endl;
		}
	}

	cout << "Total number of task/message instances: " << total_instances << endl;
}

void DebugSchedModel::InvokeSolver()
{
	_foundresults = _solver->Solve();
}

bool DebugSchedModel::CheckResults()
{
	return true;
}

void DebugSchedModel::CollectResults( SchedResults * rslt )
{
	DebugSchedResults * dsrslt = dynamic_cast< DebugSchedResults * >( rslt );

	_solver->CollectDebugInfo( (dsrslt->rslts), dsrslt->feasible, dsrslt->empty );
}

// What's in this thing?  
// node name -> { start time, instance name }
void DebugSchedModel::OutputResults( SchedResults * rslt )
{
	DebugSchedResults * dsrslt = dynamic_cast< DebugSchedResults * >( rslt );

	ostringstream out;

	// Three cases: infeasible, feasible w/ full-size core, feasible w/ smaller core
	if ( !dsrslt->feasible )
	{
		out << "Infeasible spec -- no core could be extracted for the timing constraints." << endl;
		out << "The problem lies within the resource or the latency constraints." << endl;
	} else if ( !dsrslt->empty ) 
	{	
		out << "Original specification infeasible -- attempting to extract a minimal infeasible core for timing constraints." << endl;
		out << endl;

		out << "Hyperperiod " << (_hyperperiod * 1000.0) << " ms" << endl;
		out << endl;

		for ( vector< string >::iterator pDbg = dsrslt->rslts.begin(); pDbg != dsrslt->rslts.end(); pDbg++ )
		{
			out << *pDbg << endl;
		}
	}
	else 
	{
		out << "The original specification was probably feasible -- try it again!" << endl;
	} 

	ofstream file( ConfigKeeper::inst()->getOutfileName().c_str() );
	file << out.str() << endl;
	file.close();

	cout << out.str() << endl; // copy to the command line
}