
#include "ConfigKeeper.h"

#include "defaultSchedModel.h"
#include "solverAdapter.h"
#include "cpSolver.h"

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

using namespace std;

DefaultSchedModel::DefaultSchedModel( void ): _foundresults( false ) {
    std::string solvername( "FD" );
    _solver = SolverAdapter::GetSolver( solvername );
}

DefaultSchedModel::~DefaultSchedModel( void ) {
}

// Figure out initial hyperperiod and resolution
void DefaultSchedModel::CalcHyperperiod(  ) {
    /* Iterate over tasks
       gather periods and strictness
       TODO: add a refinement step -- run needs a # of iterations arg, bool return to collectResults,
       and add a refinement call to adjust the sched model before re-running the solver
       -how do we figure out the best arrangement?
       try to schedule it as-is, then iterate! -- how can we figure out which task
       most needs to be adjusted?
     */

    // Figure out resolution
    //   Start with the first proc
    /* unsigned long long resolution;
       Proc p1((_procmap.begin())->second);
       resolution = Ticks( 1.0 / p1.Speed(), 1e-15 ); // Use femtoseconds

       std::map< std::string, Proc >::iterator pProc;
       for ( pProc = _procmap.begin(); pProc != _procmap.end(); pProc++ )
       {
       resolution = GCD( resolution, Ticks( 1.0 / (pProc->second).Speed(), 1e-15 ));
       }

       std::map< std::string, Bus >::iterator pBus;
       for ( pBus = _busmap.begin(); pBus != _busmap.end(); pBus++ )
       {
       resolution = GCD( resolution, Ticks( 1.0 / (pBus->second).Speed(), 1e-15 ));
       }

       _resolution = (double) resolution * 1e-15;

     */

    // Figure out hyperperiod
    unsigned long long hyperperiod = 1;
    std::map < std::string, Task >::iterator pTask;
    for( pTask = _taskmap.begin(  ); pTask != _taskmap.end(  ); pTask++ ) {
        hyperperiod =
            LCM( hyperperiod,
                 Ticks( 1.0 / ( pTask->second ).Freq(  ), _resolution ) );
    }

	// hyperperiod *= 2;  Good for debugging the constraints

    _ticks = hyperperiod;       // In system ticks, total hyperperiod
    _hyperperiod = _resolution * ( double ) hyperperiod;        // Total hypeperiod time
    cout << "Resolution " << _resolution << " (sec), hp " << _hyperperiod << " (sec) ticks:" << _ticks << endl;

    ( ( CPSolver * ) _solver )->SetDomain( 0, _ticks );
}

// TODO: Go back and add uncertainties for tasks and messages:
// TODO:   tasks -- should be discretized with respect to processor speed and global resolution
// TODO:   msgs -- should be discretized wrt bus speed and global resolution
// TODO:           add overhead for send/rcv and gap
void DefaultSchedModel::BuildConstraints(  ) {

    unsigned long long total_instances = 0;

    // TODO: Process Sync messages here
    // data is in
    // std::map< std::string, SyncMsg > _syncmsgmap; // qualified msg names->msg objects
    // std::map< std::string, std::vector< std::string > > _bus_syncmsgmap; // busname->msg names
    // What we should probably do:
    //   Get the sync messages for this bus
    //   Give them unique instance names (one instance per sync msg)
    //   Register them with the solver (including any bus overhead)
    //   Add them to the bus list

    // Here's the loop:
    // Iterate over buses
    //   Iterate over sync messages
    //     Create an instancekeeper variable for the sync message
    //     Add each sync message to the hw map under the bus name
    //     Iterate over procs attached to the bus
    //        Add each sync message to the hw map under the proc name
    map< string, Bus >::iterator pBus;
    for ( pBus = _busmap.begin(); pBus != _busmap.end(); pBus++ )
    {
		Bus b( pBus->second );
		vector< string > SyncMsgs = _bus_syncmsgmap[ b.Name() ];
        vector< string >::iterator pSyncMsg;
        for ( pSyncMsg = SyncMsgs.begin();
              pSyncMsg != SyncMsgs.end(); pSyncMsg++ ) {

            SyncMsg sm( _syncmsgmap[ *pSyncMsg ] );
            _solver->AddVariable( sm.Name() );
            _solver->AddSyncMsg( sm.Name(),
                                 Ticks( sm.SyncTime(), _resolution ),
                                 Ticks( sm.StartPosMin(), _resolution ),
                                 Ticks( sm.StartPosMax(), _resolution ));
            pair< std::string, unsigned long long > syncvals
                 = make_pair( sm.Name(),
                              Ticks( sm.SyncTime(), _resolution ) );
			cout << "Adding syncmap entry for " << string(b.Name()) << endl;
            _hw_syncmap[ b.Name() ].push_back( syncvals );

            vector< string > BusProcs = b.BusProcs();
            vector< string >::iterator pBusProc;
            for ( pBusProc = BusProcs.begin();
                  pBusProc != BusProcs.end(); pBusProc++ )
            {
				 cout << "Adding syncmap entry for " << string(*pBusProc) << endl;
                 _hw_syncmap[ *pBusProc ].push_back( syncvals );
            }
        }
    }

    // Make solver variables for each instance
    // Create ordering for each task
    // Create ordering for each processor
    std::map< std::string, std::vector< std::string > >::iterator pTaskList;
    //std::map< std::string, std::vector< std::string > > task_instmap;
    for( pTaskList = _proc_taskmap.begin(  );
         pTaskList != _proc_taskmap.end(  ); pTaskList++ ) {

        // Keep list of all instances on this processor
        std::vector< std::string > procList;

	// Keep list of all task instance durations
        std::vector< unsigned long long > lengthList;

        std::vector < std::string >::iterator pTask;
        for( pTask = pTaskList->second.begin(  );
             pTask != pTaskList->second.end(  ); pTask++ ) {

            // Get the task in question
            Task t( _taskmap[*pTask] );
            std::vector< std::string > taskInstList;

            unsigned long long period =
                Ticks( 1.0 / t.Freq(  ), _resolution );
            unsigned long long ninst = _ticks / period;
            unsigned long long dur = Ticks( t.WCET(  ), _resolution );

            //cout << "Task " << *pTask << " " << ninst << " instances " << dur << " ticks long" << endl;
            for( unsigned long long count = 0; count < ninst; count++ ) {

                // Make the name for this instance
                ostringstream oss;
                oss << *pTask << "_" << count;

                _solver->AddVariable( oss.str(  ) );
                taskInstList.push_back( oss.str(  ) );
                procList.push_back( oss.str(  ) );
                lengthList.push_back( dur );
            }

            //task_instmap[*pTask] = taskInstList;
            _taskmap[ *pTask ].AddInstances( taskInstList );
            _num_inst[*pTask] = taskInstList.size(  );
            total_instances += taskInstList.size(  );

            //if ( taskInstList.size() > 1 )
            _solver->OrderInstances( taskInstList,
                                     Ticks( 1.0 / t.Freq(  ), _resolution ),
                                     Ticks( t.WCET(  ), _resolution ) );

			long long off = STicks( t.Offset(), _resolution );
			cout << "!!! Offset ticks " << off << endl;
			if ( off >= 0 ) // We have a fixed offset
			{
				string tname = (*pTask) + string( "_0" ); // only set the offset of the first task				
				_solver->AddTaskOffset( tname, dur, (unsigned long long) off, (unsigned long long) off );
			}
        }

		// Look up the sync messages for this processor and add them to the proclist
		vector< pair< string, unsigned long long > > syncmsgs
			= _hw_syncmap[ pTaskList->first ];

		vector< pair< string, unsigned long long > >::iterator pSyncMsg;
		for ( pSyncMsg = syncmsgs.begin();
			  pSyncMsg != syncmsgs.end(); pSyncMsg++ ) {

			cout << "Adding sync msg " << pSyncMsg->first << " to list for processor " << pTaskList->first << endl;
            procList.push_back( pSyncMsg->first );
            lengthList.push_back( pSyncMsg->second );
        }

        if( procList.size(  ) > 1 )
            _solver->SerializeInstances( procList, lengthList );

        std::map < std::string, std::vector < std::string > >::iterator
		pMsgList = _proc_msgmap.find( pTaskList->first );

        if( pMsgList != _proc_msgmap.end(  ) ) {

            for( std::vector < std::string >::iterator pMsg =
                 	( pMsgList->second ).begin(  );
                 pMsg != ( pMsgList->second ).end(  ); pMsg++ ) {

                // Get the message
                Msg m( _msgmap[*pMsg] );

                vector < std::string > rcvrs = m.RcvTasks(  );
                for( vector < std::string >::iterator pRcvr = rcvrs.begin(  );
                     pRcvr != rcvrs.end(  ); pRcvr++ ) {
                    Task st( _taskmap[m.SndTask(  )] );
                    unsigned long long period =
                        Ticks( 1.0 / st.Freq(  ), _resolution );
                    unsigned long long ninst = _ticks / period;
                    unsigned long long dur =
                        Ticks( st.WCET(  ), _resolution );

                    ostringstream sname,
                        rname;
                    sname << st.LongName(  ) << "_0";   // pick the first instance
                    rname << *pRcvr << "_0";
                    _solver->AddLocalDependency( sname.str(  ), rname.str(  ),
                                                 dur );
                }
            }
        }
    }

    // Do the same procedure for buses
    std::map < std::string, std::vector < std::string > >::iterator pMsgList;   // Iterate by bus
    for( pMsgList = _bus_msgmap.begin(  ); pMsgList != _bus_msgmap.end(  );
         pMsgList++ ) {

        // Standard bus messages
        Bus b( _busmap[pMsgList->first] );
        double busrate = b.Speed(  );

	// Keep list of all instances on this bus
        std::vector < std::string > busList;

	// Keep list of all msg instance durations
        std::vector < unsigned long long > lengthList;


        std::vector < std::string >::iterator pMsg;
        for( pMsg = pMsgList->second.begin(  );
             pMsg != pMsgList->second.end(  ); pMsg++ ) {
            // Get the message
            Msg m( _msgmap[*pMsg] );
            std::vector < std::string > msgInstList;

            // Now the tricky part -- # msgs = min( # sender instances, max (over tasks) ( # rcvr instances ) )
            std::vector < std::string > rcvrList( m.RcvTasks(  ) );
            unsigned long long maxInstances = 1;
            std::vector < std::string >::iterator pRcvr;
            for( pRcvr = rcvrList.begin(  ); pRcvr != rcvrList.end(  );
                 pRcvr++ ) {
                unsigned long long ninst = _num_inst[*pRcvr];

                maxInstances = ( ninst > maxInstances ) ? ninst : maxInstances; // min
                //cout << "Rcvr maxInst: " << maxInstances << " ninst " << ninst << endl;
            }

            Task s( _taskmap[m.SndTask(  )] );  // Get the sender
            unsigned long long speriod =
                Ticks( 1.0 / s.Freq(  ), _resolution );
            //unsigned long long sinst = _ticks / speriod;
            unsigned long long sinst = _num_inst[m.SndTask(  )];
            unsigned long long minst =
                ( sinst < maxInstances ) ? sinst : maxInstances;

            //cout << "Sndr sinst " << sinst << " minst " << minst << endl;

            //_num_inst[ m.Name() ] = minst;
            _num_inst[*pMsg] = ( unsigned int ) minst;
            total_instances += minst;

            // Scale msg length by bus rate
            unsigned long long dur =
                Ticks( m.Length(  ) / busrate,
                       _resolution ) + Ticks( b.Gap(  ), _resolution );

            cout << "Msg " << *pMsg << " " << minst << " instances " << dur << " ticks long" << endl;

            // Create msg instances & order them
            for( unsigned long long count = 0; count < minst; count++ ) {
                ostringstream oss;
                oss << *pMsg << "_" << count;

                _solver->AddVariable( oss.str(  ) );
                msgInstList.push_back( oss.str(  ) );
                busList.push_back( oss.str(  ) );
                lengthList.push_back( dur );

            }

            _msgmap[ *pMsg ].AddInstances( msgInstList );

            // order the instances -- use the sender duration, with the period rate difference ( #senders / #minst )-1 * period
            if( msgInstList.size(  ) > 1 ) {
                _solver->OrderInstances( msgInstList,
                                         Ticks( s.WCET(  ),
                                                _resolution ) +
                                         ( ( ( ( unsigned long long ) ( sinst
                                                                        /
                                                                        minst ) )
                                             - 1 ) * speriod ), dur, false );
                //Ticks( m.Length(), _resolution ), false);

			}
			else if ( msgInstList.size() == 1 )
			{
				_solver->EnforceDuration( msgInstList, dur );
			}
			else
			{
				cout << "Error: message " << *pMsg << " has no instances!" << endl;
			}

            // Take the msg instances & serialize with the senders
            msgInstList.insert( msgInstList.end(  ),
                                s.Instances().begin(),
                                s.Instances().end() );

            std::vector <
                unsigned long long >msgInstSizes( msgInstList.size(  ),
                                                    dur );
            msgInstSizes.insert( msgInstSizes.end(  ),
                                    s.Instances().size(),
                                    Ticks( s.WCET(  ), _resolution ) );
			//cout << "Called serialize!!! " << endl;
            _solver->SerializeInstances( msgInstList, msgInstSizes );

            //}

			long long off = STicks( m.Offset(), _resolution );
			if ( off >= 0 ) // We have a fixed offset
			{
				string mname = (*pMsg) + string( "_0" ); // only set the offset of the first task				
				_solver->AddTaskOffset( mname, dur, (unsigned long long) off, (unsigned long long) off );
			}

        }

		// Look up the sync messages for this bus, and add them to the bus list
		vector< pair< string, unsigned long long > > syncmsgs
			= _hw_syncmap[ pMsgList->first ];

		vector< pair< string, unsigned long long > >::iterator pSyncMsg;
        for ( pSyncMsg = syncmsgs.begin();
              pSyncMsg != syncmsgs.end(); pSyncMsg++ ) {

			cout << "Adding sync msg " << pSyncMsg->first << " to list for bus " << pMsgList->first << endl;
            busList.push_back( pSyncMsg->first );
            lengthList.push_back( pSyncMsg->second );

        }

        // Serialize all of the instances on the bus
        if( busList.size() > 1 )
            _solver->SerializeInstances( busList, lengthList );


    }

    // Handle synchronous buses -- the constraints are stronger
    for( pMsgList = _bus_msgmap.begin(  ); pMsgList != _bus_msgmap.end(  );
         pMsgList++ ) {

        // Standard bus messages
        Bus b( _busmap[pMsgList->first] );

        if( b.Synchronous() )
        {
            double busrate = b.Speed(  );
            std::vector < std::string > busListSync; // This one is for all messages and tasks on a synchronous bus
            std::vector < unsigned long long > lengthListSync;

            // This one is a bit simpler conceptually
            //  add all of the messages to the list
            for ( vector< string >::iterator pmsg = (pMsgList->second).begin();
                  pmsg != (pMsgList->second).end(); pmsg++ )
            {
                Msg m( _msgmap[*pmsg] );
                unsigned long long dur =
                    Ticks( m.Length(  ) / busrate,
                       _resolution ) + Ticks( b.Gap(  ), _resolution );

                busListSync.insert( busListSync.end(), m.Instances().begin(),
                                    m.Instances().end() );
                lengthListSync.insert( lengthListSync.end(), m.Instances().size(), dur );

            }

            // Look up the sync messages for this bus, and add them to the bus list
            vector< pair< string, unsigned long long > > syncmsgs
                = _hw_syncmap[ pMsgList->first ];

            vector< pair< string, unsigned long long > >::iterator pSyncMsg;
            for ( pSyncMsg = syncmsgs.begin();
                  pSyncMsg != syncmsgs.end(); pSyncMsg++ ) {

                cout << "Adding sync msg " << pSyncMsg->first << " to sync list for bus " << pMsgList->first << endl;
                busListSync.push_back( pSyncMsg->first );
                lengthListSync.push_back( pSyncMsg->second );

            }

            // We need one serialization constraint for each processor

            vector< string > procs = b.BusProcs();
            vector< string >::iterator pproc;
            for ( pproc = procs.begin(); pproc != procs.end(); pproc++ )
            {
                vector< string > procBusListSync = busListSync;
                vector< unsigned long long > procLengthListSync = lengthListSync;

                vector< string > tasks = _proc_taskmap[ *pproc ];
                for ( vector< string >::iterator ptask = tasks.begin(); ptask != tasks.end(); ptask++ )
                {
                    Task t( _taskmap[ *ptask ] );

                    vector< string > tinsts = t.Instances();
                    for ( vector< string >::iterator pinst = tinsts.begin(); pinst != tinsts.end(); pinst++ )
                    {
                        procBusListSync.push_back( *pinst );
                        procLengthListSync.push_back( Ticks( t.WCET(  ), _resolution ) );
                    }
                }

                if( procBusListSync.size() > 1 )
                    _solver->SerializeInstances( procBusListSync, procLengthListSync );

            }
        }
    }


    // Now, what to do about latencies?
    // Latencies have to be a relationship between two task instances, so which two?
    std::vector < Latency >::iterator pLat;
    for( pLat = _latencies.begin(  ); pLat != _latencies.end(  ); pLat++ ) {
        Task s( _taskmap[pLat->SndTask(  )] );
        Task r( _taskmap[pLat->RcvTask(  )] );

        double Speriod = 1.0 / s.Freq(  );
        double Rperiod = 1.0 / r.Freq(  );
        double lat = pLat->LatencyVal(  );

        // Get rid of redundant latency constraints -- if the requested latency is
        // larger than the smaller period of the two tasks, then it will be trivially
        // satisfied by the specification.  Don't make n^2 constraints unless we
        // really have to!
        // TODO:  This assessment is too simplistic -- we have to consider the rate
        // TODO:  relationship between the two tasks
        if( ( ( Speriod < Rperiod ) && ( lat < Speriod ) )
            || ( ( Speriod > Rperiod ) && ( lat < Rperiod ) ) ) {
            unsigned int minRcvrIdx = ( unsigned int ) floor( lat / Rperiod );
            unsigned int maxRcvrIdx =
                ( unsigned int ) floor( ( lat + Speriod ) / Rperiod );
            unsigned int minSndrIdx = ( unsigned int ) floor( lat / Speriod );
            unsigned int maxSndrIdx =
                ( unsigned int ) floor( ( lat + Rperiod ) / Speriod );

            // Make sure we're not out of bounds
            unsigned int rcvrsize = _num_inst[pLat->RcvTask(  )] - 1;
            unsigned int sndrsize = _num_inst[pLat->SndTask(  )] - 1;
            minRcvrIdx = ( minRcvrIdx < 0 ) ? 0 : minRcvrIdx;
            maxRcvrIdx = ( maxRcvrIdx > rcvrsize ) ? rcvrsize : maxRcvrIdx;
            minSndrIdx = ( minSndrIdx < 0 ) ? 0 : minSndrIdx;
            maxSndrIdx = ( maxSndrIdx > sndrsize ) ? sndrsize : maxSndrIdx;

            std::vector < std::string > sndrnames;
            std::vector < std::string > rcvrnames;
            // Yes, there are n^2 constraints here, but hopefully n is very small
            for( unsigned int ridx = minRcvrIdx; ridx <= maxRcvrIdx; ridx++ )
                for( unsigned int sidx = minSndrIdx; sidx <= maxSndrIdx;
                     sidx++ ) {
                    ostringstream sinst,
                        rinst;
                    sinst << s.Proc(  ) << "/" << s.Name(  ) << "_" << sidx;
                    rinst << r.Proc(  ) << "/" << r.Name(  ) << "_" << ridx;
                    sndrnames.push_back( sinst.str(  ) );
                    rcvrnames.push_back( rinst.str(  ) );
                }

            _solver->AddLatencyBound( sndrnames, rcvrnames,
                                      Ticks( lat - r.WCET(  ),
                                             _resolution ) );
        }
        else {
            cout << "Throwing away redundant latency constraint: ";
            cout << s.Name(  ) << " " << r.Name(  ) << " " << lat << endl;
        }
    }

    cout << "Total number of task/message instances: " << total_instances <<
        endl;
}

void DefaultSchedModel::InvokeSolver(  ) {
    _foundresults = _solver->Solve(  );
}

bool DefaultSchedModel::CheckResults(  ) {
    // We'll need something fairly sophisticated here!
    return _foundresults;
}

void DefaultSchedModel::CollectResults( SchedResults * rslt )
//void DefaultSchedModel::CollectResults( std::map< std::string, std::map< double, std::string > > & schedule )
{
    DefaultSchedResults *dsrslt =
        dynamic_cast < DefaultSchedResults * >( rslt );
    std::map < std::string, unsigned long long > results;
    _solver->CollectInstances( results );

    // What needs to be done to format the instances?
    //   Organize into processors and buses
    //   Look up each instance for each processor and bus
    //   Order the instances by start time
    //   Convert start times to absolute
    //   Store a fixed schedule for each processor and bus
    //   Make schedule available to output processing

    std::map < std::string,
        std::vector < std::string > >::iterator pProcTasks;
    for( pProcTasks = _proc_taskmap.begin(  );
         pProcTasks != _proc_taskmap.end(  ); pProcTasks++ ) {

        std::map < double, std::string > procschedule;
        vector< pair< string, unsigned long long > > syncmsgs
	    = _hw_syncmap[ pProcTasks->first ];
        vector< pair< string, unsigned long long > >::iterator pSyncMsg;
        for ( pSyncMsg = syncmsgs.begin();
              pSyncMsg != syncmsgs.end(); pSyncMsg++ ) {

            procschedule[AbsTime( results[pSyncMsg->first], _resolution )]=
                pSyncMsg->first;
        }

        std::vector < std::string >::iterator pTask;
        for( pTask = pProcTasks->second.begin(  );
             pTask != pProcTasks->second.end(  ); pTask++ ) {


            for( unsigned int idx = 0; idx < _num_inst[*pTask]; idx++ ) {

                ostringstream name;
                name << *pTask << "_" << idx;

                procschedule[AbsTime( results[name.str(  )], _resolution )] =
                    name.str(  );
            }
        }

        dsrslt->schedule[pProcTasks->first] = procschedule;
    }

    std::map < std::string, std::vector < std::string > >::iterator pBusMsgs;
    for( pBusMsgs = _bus_msgmap.begin(  ); pBusMsgs != _bus_msgmap.end(  );
         pBusMsgs++ ) {

        std::map < double, std::string > busschedule;
        vector< pair< string, unsigned long long > > syncmsgs
    	    = _hw_syncmap[ pBusMsgs->first ];
        vector< pair< string, unsigned long long > >::iterator pSyncMsg;
        for ( pSyncMsg = syncmsgs.begin();
              pSyncMsg != syncmsgs.end(); pSyncMsg++ ) {

            busschedule[AbsTime( results[pSyncMsg->first], _resolution )]=
                pSyncMsg->first;
        }

        std::vector < std::string >::iterator pMsg;
        for( pMsg = pBusMsgs->second.begin(  );
             pMsg != pBusMsgs->second.end(  ); pMsg++ ) {

            for( unsigned int idx = 0; idx < _num_inst[*pMsg]; idx++ ) {

                ostringstream name;
                name << *pMsg << "_" << idx;

                busschedule[AbsTime( results[name.str(  )], _resolution )] =
                    name.str(  );
            }
        }

        dsrslt->schedule[pBusMsgs->first] = busschedule;
    }
}

// What's in this thing?
// node name -> { start time, instance name }
void DefaultSchedModel::OutputResults( SchedResults * rslt ) {

    DefaultSchedResults *dsrslt =
        dynamic_cast < DefaultSchedResults * >( rslt );
    // Don't forget the hyperperiod at the start!
    string outunits( ConfigKeeper::inst(  )->getOutputUnits(  ) );
    double outscale = ConfigKeeper::inst(  )->getOutputScaleFactor(  );
	ofstream file( ConfigKeeper::inst(  )->getOutfileName(  ).c_str(  ), ios_base::out | ios_base::trunc );

#ifdef WIN32
	if ( !file.good() )
	{
		
		if (GetLastError() != 0) {
			LPTSTR errorText = NULL;
		    FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL, GetLastError(), MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT), 
					(LPTSTR)&errorText, 0, NULL );

			if ( NULL != errorText )
			{
				cout << "Failed to open output file: " << errorText << endl;
				LocalFree( errorText );
				errorText = NULL;
			}
			else
				cout << "Failed to open output file." << endl;
		}
		return;
	}
#endif

    file << "Hyperperiod " << ( _hyperperiod /
                                outscale ) << " " << outunits << endl;
    file << endl;

    std::map < std::string, std::map < double,
    std::string > >::iterator pResource;
    for( pResource = dsrslt->schedule.begin(  );
         pResource != dsrslt->schedule.end(  ); pResource++ ) {
        //file << pResource->first << ":" << endl;  // Section headings in output
        std::map < double,
        std::string >::iterator pSchedEntry;
        for( pSchedEntry = pResource->second.begin(  );
             pSchedEntry != pResource->second.end(  ); pSchedEntry++ ) {
            file << pSchedEntry->second << " " << ( pSchedEntry->first / outscale ) << endl;    // time in output units
        }
        file << endl;
    }

    file.close(  );
}
