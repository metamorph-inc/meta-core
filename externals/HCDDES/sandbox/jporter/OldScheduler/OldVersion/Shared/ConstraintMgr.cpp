
#include "ConstraintMgr.h"
#include "ConstraintSys.h"
#include "MiscUtil.h"

#include <sstream>
#include <fstream>
#include <iomanip>

// TODO: your implementation here

// Build class from scratch
ConstraintMgr::ConstraintMgr( Udm::DataNetwork* p_backend, TTSystem & sys ) : _varcount(0)
{
    /// Create the proper constraints
    RootFolder rootObj = RootFolder::Cast(p_backend->GetRootObject());

    // First make a constraint model
    _cp = ConstraintProblem::Create( rootObj );
    _cp.name() = string(sys.name()) + "Constraints";
}

// Initialize class from existing ConstraintProblem
ConstraintMgr::ConstraintMgr( ConstraintProblem & cp ) : _varcount(0)
{
    _cp = cp;

    std::set< FDVar > vars = _cp.FDVar_children();
    std::set< FDVar >::iterator pVar;
    for ( pVar = vars.begin(); pVar != vars.end(); pVar++ )
    {
        _inst_varmap[ pVar->name() ] = *pVar;
        _inst_nummap[ pVar->name() ] = _varcount++;
    }

	UpdateSolutions();
}

// I hope we made sure all the instance names were unique ;-)
void ConstraintMgr::AddTaskInstance( const std::string & tskName, int wcet, int upper_bound )
{
    FDVar fdv = FDVar::Create( _cp );
    fdv.name() = tskName;
    _inst_varmap[ tskName ] = fdv;
    _inst_nummap[ tskName ] = _varcount++;
    fdv.LowerBound() = 1;
    fdv.UpperBound() = upper_bound;
    fdv.Size() = wcet;             
}

void ConstraintMgr::AddMsgInstance( const std::string & msgName, int dur, int upper_bound )
{
    FDVar fdv = FDVar::Create( _cp );
    fdv.name() = msgName;
    _inst_varmap[ msgName ] = fdv;
    _inst_nummap[ msgName ] = _varcount++;
    fdv.LowerBound() = 1;
    fdv.UpperBound() = upper_bound;
    fdv.Size() = dur;
}

void ConstraintMgr::AddEqualityConstraint( const std::string & firstInst, const std::string & secondInst, const int offset )
{
    // Create a new constraint that's appropriate for the pair
    EqualityConstraint eq = EqualityConstraint::Create( _cp );
    //ostringstream eqcname;
    //eqcname << nextInst " = " << pInstance->first << " + " << dist;
    //eq.name() = eqcname.str();
    eq.name() = secondInst + " = " + firstInst + " + " + MiscUtil::itos(offset);
    XPort x = XPort::Create( eq );
    YPort y = YPort::Create( eq );
    Member xm = Member::Create( _cp );
    Member ym = Member::Create( _cp );
    
    //xm.srcMember_end() = pInstance->second; // later instance -- this was wrong! Joe - 5/7/2007
    xm.srcMember_end() = _inst_varmap[ secondInst ];
    xm.dstMember_end() = x;
    //ym.srcMember_end() = tsk_inst_varmap[ pdlist[ pInstance->first].first ]; // earlier instance
    ym.srcMember_end() = _inst_varmap[ firstInst ];
    ym.dstMember_end() = y;
    eq.Offset() = offset;  // period

}

// Constraint of the form ltInst <= gtInst + offset
void ConstraintMgr::AddInequalityConstraint( const std::string & ltInst, const std::string & gtInst, const int offset )
{
    InequalityConstraint order_ieq = InequalityConstraint::Create( _cp );
    //ostringstream order_ieqcname;

    XPort order_x = XPort::Create( order_ieq );
    YPort order_y = YPort::Create( order_ieq );
    Member order_xmem = Member::Create( _cp );
    Member order_ymem = Member::Create( _cp );

    order_xmem.srcMember_end() = _inst_varmap[ ltInst ];
    order_xmem.dstMember_end() = order_x;
    order_ymem.srcMember_end() = _inst_varmap[ gtInst ]; 
    order_ymem.dstMember_end() = order_y;
    order_ieq.Offset() = offset;

    order_ieq.name() = ltInst + " <= " + gtInst + " + " + MiscUtil::itos(offset);

}

void ConstraintMgr::AddSerialConstraint( const std::string & usedFor, std::vector< std::string > & instNames )
{
    SerialConstraint sc = SerialConstraint::Create( _cp );
    sc.name() = "Serializing for " + usedFor;

    vector< string >::iterator pInst;
    for ( pInst = instNames.begin(); 
          pInst != instNames.end(); pInst++ )
    {
        Member mem = Member::Create( _cp );
        mem.srcMember_end() = _inst_varmap[ *pInst ];
        mem.dstMember_end() = sc;
    }        
}

bool ConstraintMgr::SolveConstraints( std::string & stats, int maxsln )
{
    // Find the number of variable instances
    set< FDVar > vars = _cp.FDVar_children();
    //map< string, int > varmap;
    int instances = _inst_nummap.size();
    //int count = 0;
    int lbound = 1500000000;
    int ubound = 0;
    for ( set< FDVar >::iterator pVar = vars.begin(); pVar != vars.end(); pVar++ )
    {
        //varmap[ pVar->name() ] = count++; // Map variable names to indices (names had better be unique!)
        lbound = ( lbound < pVar->LowerBound() ) ? lbound : pVar->LowerBound();
        ubound = ( ubound > pVar->UpperBound() ) ? ubound : pVar->UpperBound();
    }
    _cp.LowerBound() = lbound;
    _cp.UpperBound() = ubound;

    // Create the ConstraintSys object
    ConstraintSys * csys = new ConstraintSys( instances, lbound, ubound, _cp, _inst_nummap );

    unsigned int n_p = -1;
    unsigned int n_b = -1;
    if (csys->status() != SS_FAILED) {
      n_p = csys->propagators();
      n_b = csys->branchings();
    }

    // Try to run the search engine like crazy
    Search::Stop* stop = NULL;
    SearchConfig se = _cp.SearchConfig_child();  // _cp.SearchConfig_children();
    int max_solutions = maxsln;
	if ( Udm::Object(se) != Udm::null )
    {	
        max_solutions = (int) se.MaxSolutions();
        stop = FailTimeStop::create( (unsigned int) se.MaxFailures(), (unsigned int) se.MaxRunTimeSec() * 1000 /* Convert to msec */ );
    }

    DFS< ConstraintSys > srch( csys, Search::Config::c_d, Search::Config::a_d, stop ); 
	//LDS< ConstraintSys > srch( csys, 10, stop );
    delete csys;

    ConstraintSys * result = srch.next();

    Search::Statistics stat = srch.statistics();
    ostringstream stat_str;
    stat_str << "Search Stats" << endl << endl;
    stat_str << "Initial" << endl
             << "\tpropagators:   " << n_p << endl
             << "\tbranchings:    " << n_b << endl
             << endl
             << "Summary" << endl
             // << "\truntime:       " << t.stop() << endl
             //<< "\tsolutions:     " << abs(static_cast<int>(o.solutions) - i) << endl
             << "\tpropagations:  " << stat.propagate << endl
             << "\tfailures:      " << stat.fail << endl
             << "\tclones:        " << stat.clone << endl
             << "\tcommits:       " << stat.commit << endl
             << "\tpeak memory:   "
             << static_cast<int>((stat.memory+1023) / 1024) << " KB";
    //AfxMessageBox( stat_str.str().c_str() );
    if ( stats != string("nostats") )
    {
		stats = stat_str.str();
    }

    if ( result == NULL )
    {
        //AfxMessageBox( "No solutions found!" );
        return false; 
    }
    else 
    {
        time_t ct;
        time( &ct );
        tm * t = localtime(&ct);

        ostringstream tmstr;
        tmstr << t->tm_hour << t->tm_min << t->tm_sec;

        int count = 0;
        while ( result && ( count < max_solutions ) )
        {
            vector< int > srch_results;
            result->GetResults( srch_results );

            ostringstream result_str;
            
            
            result_str << "Solution" << ++count << "_" << tmstr.str();
            
            
            Solution sol = Solution::Create( _cp );
            sol.name() = result_str.str();

            //result_str << endl << endl;

            for( set< FDVar >::iterator pVar = vars.begin(); pVar != vars.end(); pVar++ )
            {
                //result_str << pVar->name() << ": " << srch_results[ varmap[ pVar->name() ] ] << endl;
                Assignment a = Assignment::Create( sol );
                a.name() = pVar->name();
                a.Value() = srch_results[ _inst_nummap[ pVar->name() ] ];
            }

            //AfxMessageBox( result_str.str().c_str() );

            result = srch.next();
        }
    }

    return true;
}

// Update local map of solutions (for an existing constraint problem)
void ConstraintMgr::UpdateSolutions()
{
	set< Solution > slns = _cp.Solution_children();
	std::map< std::string, Solution >   new_solns;

	set< Solution >::iterator pSln;
    for ( pSln = slns.begin(); pSln != slns.end(); pSln++ )
    {
		new_solns[ pSln->name() ] = *pSln;	
	}

	_solutions = new_solns;
}

// Programmatically get a list of the current solutions - also updates
bool ConstraintMgr::GetSolutionNames( std::vector< string > & solutions )
{
	bool bSuccess = false;
    set< Solution > slns = _cp.Solution_children();
	std::map< std::string, Solution >   new_solns;
    if ( slns.size() == 0 )
    {
        ; // Do nothing
    }
    else
    {
        set< Solution >::iterator pSln;
        for ( pSln = slns.begin(); pSln != slns.end(); pSln++ )
        {
            solutions.push_back( pSln->name() );
            new_solns[ pSln->name() ] = *pSln;
        }
		bSuccess = true;
    }

	_solutions = new_solns;
    return bSuccess;
}

// One for Matlab output -- prepend prefix to the generated filename
void ConstraintMgr::DrawSchedule( const std::string & prefix, const std::string & sln_name )
{
    //ConstraintProblem cp = ConstraintProblem::Cast( s.parent() );
    InstanceRef ir = _cp.InstanceRef_child();
    InstanceModel im = ir.ref();
    int global_period = (int) im.HyperPeriod();

    // Get the solutions
    Solution s = _solutions[sln_name];

    // First, build an index of all of the answers in the solution
    set< Assignment > anslist = s.Assignment_children();
    set< Assignment >::iterator pAns;
    map< string, int > answers;
    for ( pAns = anslist.begin(); pAns != anslist.end(); pAns++ )
    {
        answers[ pAns->name() ] = (int) pAns->Value();
    }

    // We'll use these a bit
    set< Processor > procs = im.Processor_children();
    set< Processor >::iterator pProc;

    set< Bus > buses = im.Bus_children();
    set< Bus >::iterator pBus;

    // Count colors and store durations
    int colorcount = 0;
    map< string, int > durations;
    map< string, int > colors;
    for ( pProc = procs.begin(); pProc != procs.end(); pProc++ )
    {
        // First get all of the durations (and colors) ready from the tasks on the processor
        set< Task > tasks = pProc->Task_children();
        set< Task >::iterator pTsk;
        for ( pTsk = tasks.begin(); pTsk != tasks.end(); pTsk++ )
        {
            
            durations[ pTsk->name() ] = (int) pTsk->WCExecTime();
            pair< map<string,int>::iterator, bool > r = colors.insert( make_pair( pTsk->name(), colorcount ) ); // No overwrite
            if (r.second == true) colorcount++; // only increment if insert was successful
        }
    }

    for ( pBus = buses.begin(); pBus != buses.end(); pBus++ )
    {
        set< Message > msgs = pBus->Message_children();
        set< Message >::iterator pMsg;
        for ( pMsg = msgs.begin(); pMsg != msgs.end(); pMsg++ )
        {
            durations[ pMsg->name() ] = (int) pMsg->MsgLength();
            pair< map<string,int>::iterator, bool > r = colors.insert( make_pair( pMsg->name(), colorcount ) );
            if ( r.second == true ) colorcount++;
        }
    }


    // Iterate over the processors in the instance model
    //   Create one line for each processor -- 
    //   Put the task boxes on the line, with a different color for
    //   each task type.  Save an index with instance name and rect coords
    int yoffset = 1;
    int ywidth = 5;
    int yskip = 10;
    map< string, vector< string > > instance_grid;
    map< string, vector< int > > taskboxes; // Task id, (x,y,x+width,y+height, color)
    map< string, int > yoffsetmap;  // Processor/Bus name -> y offset of tasks on that line
    for ( pProc = procs.begin(); pProc != procs.end(); pProc++ )
    {
        TaskInstances ti = pProc->TaskInstances_child();

        // For the box, we need a line offset (for processor),
        // fixed width for the line, start time, and duration
        //multimap< int, pair< string, string > > tasksort;  // Sort the tasks according to their offset
        
        vector< string >  instances_per_proc;
        set< TaskInstance > instances = ti.TaskInstance_children();
        set< TaskInstance >::iterator pInst;
        for ( pInst = instances.begin(); pInst != instances.end(); pInst++ )
        {
            instances_per_proc.push_back( pInst->name() );
            vector< int > vals(5);
            vals[0] = (int) answers[ pInst->name() ] + 1;
            vals[1] = yoffset;
            vals[2] = (int) durations[ pInst->RefName() ];
            vals[3] = ywidth;
            vals[4] = colors[ pInst->RefName() ];
            taskboxes[ pInst->name() ] = vals;
        }
        instance_grid[ pProc->name() ] = instances_per_proc;
        yoffsetmap[ pProc->name() ] = yoffset;
        yoffset += ywidth + yskip;
    }

    // Iterate over the buses in the instance model
    //   Create one line for each bus -- 
    //   Put the msg boxes on the line, with a different color for
    //   each msg type.  Save an index with instance name and rect coords
    for ( pBus = buses.begin(); pBus != buses.end(); pBus++ )
    {

        MsgInstances ti = pBus->MsgInstances_child();
        vector< string >  instances_per_proc;
        set< MsgInstance > instances = ti.MsgInstance_children();
        set< MsgInstance >::iterator pInst;
        for ( pInst = instances.begin(); pInst != instances.end(); pInst++ )
        {

            instances_per_proc.push_back( pInst->name() );
            vector< int > vals(5);
            vals[0] = (int) answers[ pInst->name() ] + 1;
            vals[1] = yoffset;
            vals[2] = (int) durations[ pInst->RefName() ];
            vals[3] = ywidth;
            vals[4] = colors[ pInst->RefName() ];
            taskboxes[ pInst->name() ] = vals;
        }
        instance_grid[ pBus->name() ] = instances_per_proc;
        yoffsetmap[ pBus->name() ] = yoffset;
        yoffset += ywidth + yskip;
    }

    // Now get the dependencies, look up the names from the box
    // index sets created above, and draw lines between the boxes.
    // Start with the different connectors in the dependency model.
    // Gather up all of the connectors and plot one arrow per pair.
    vector< vector< double > > lineends;
    for ( pBus = buses.begin(); pBus != buses.end(); pBus++ )
    {
        Dependencies deps = pBus->Dependencies_child();
        //set< Dependencies >::iterator pDep = deps.begin();  // We had better have only one!
        set< SenderInst > senders = deps.SenderInst_children();
        set< ReceiverInst > receivers = deps.ReceiverInst_children();

        set< SenderInst >::iterator pSndr;
        set< ReceiverInst >::iterator pRcvr;

        for ( pSndr = senders.begin(); pSndr != senders.end(); pSndr++ )
        {
            vector< double > sline(4);
            // Look up sending end and message end, use the instance name to get the box, and find the center
            TaskInstRef sref = pSndr->srcSenderInst_end();
            MsgInstRef mref = pSndr->dstSenderInst_end();
            TaskInstance sndtask = sref.ref();
            MsgInstance sndmsg = mref.ref();

            vector< int > sndbox = taskboxes[ sndtask.name() ];
            vector< int > msgbox = taskboxes[ sndmsg.name() ];

            sline[0] = sndbox[0] + ( sndbox[2] / 2.0 ); // send box x-center
            sline[1] = sndbox[1] + ( sndbox[3] / 2.0 ); // send box y-center
            sline[2] = msgbox[0] + ( msgbox[2] / 2.0 ); // msg box x-center
            sline[3] = msgbox[1] + ( msgbox[3] / 2.0 ); // msg box y-center

            lineends.push_back( sline );
        }

        for ( pRcvr = receivers.begin(); pRcvr != receivers.end(); pRcvr++ )
        {
            vector< double > rline(4);
            // Look up sending end and message end, use the instance name to get the box, and find the center
            MsgInstRef mref = pRcvr->srcReceiverInst_end();
            TaskInstRef rref = pRcvr->dstReceiverInst_end();
            MsgInstance sndmsg = mref.ref();
            TaskInstance rcvtask = rref.ref();
            vector< int > msgbox = taskboxes[ sndmsg.name() ];
            vector< int > rcvbox = taskboxes[ rcvtask.name() ];
            
            rline[0] = msgbox[0] + ( msgbox[2] / 2.0 ); // msg box x-center
            rline[1] = msgbox[1] + ( msgbox[3] / 2.0 ); // msg box y-center
            rline[2] = rcvbox[0] + ( rcvbox[2] / 2.0 ); // send box x-center
            rline[3] = rcvbox[1] + ( rcvbox[3] / 2.0 ); // send box y-center

            lineends.push_back( rline );
        }
    }
    
    // Create a file here with the proper name
    string pfname = prefix + string("_") + string(s.name()) + string(".m");

    ofstream fout;
    fout.open( pfname.c_str() );

    fout << "cm = colormap;" << endl;
    fout << "cs = size(cm,1);" << endl << endl;

    map< string, vector< string > >::iterator pInst;
    
    // First the boxes
    for ( pInst = instance_grid.begin(); pInst != instance_grid.end(); pInst++ )
    {
        vector<string>::iterator pList;
        for ( pList = (pInst->second).begin(); pList != (pInst->second).end(); pList++ )
        {	
            fout << "rectangle( 'Position', [";
            fout << taskboxes[ *pList ][0] << ",";
            fout << taskboxes[ *pList ][1] << ",";
            fout << taskboxes[ *pList ][2] << ",";
            fout << taskboxes[ *pList ][3] << "], 'FaceColor', ";
            fout << "cm( floor( " << (double(taskboxes[ *pList ][4])/double(colorcount)) << " * cs )+1, :) );" << endl;
        }
    }
    fout << endl;

    // Iterate over the processors
    //   Get the y offset for this processor
    //   Make a text box for the processor name, place it on the upper part of the line
    //   Iterate over the tasks -- place all of the tasks on the line, in an appropriately colored box
    //   Repeat for buses
    int procxoffset = -10;
    int minxoffset = procxoffset;
    int xskip = 7;
    int xstart = -3;
    int procyoffset = 4;
    int taskyoffset = 1;
    for ( pProc = procs.begin(); pProc != procs.end(); pProc++ )
    {
        fout << "text( " << procxoffset << ", " << yoffsetmap[ pProc->name() ]+procyoffset << ", '" << pProc->name() << "', ";
        //fout << "', 'BackgroundColor', cm( floor( " << (double(color[pProc->name()])/double(colorcount)) << " * cs )+1, :), ";
        fout << "'EdgeColor', [0 0 0], 'HorizontalAlignment', 'center' );" << endl;
        // First get all of the durations (and colors) ready from the tasks on the processor
        int xoffset = xstart;
        set< Task > tasks = pProc->Task_children();
        set< Task >::iterator pTsk;
        for ( pTsk = tasks.begin(); pTsk != tasks.end(); pTsk++ )
        {
            fout << "text( " << xoffset << ", " << yoffsetmap[ pProc->name() ]+taskyoffset << ", '" << pTsk->name() << "', ";
            fout << "'BackgroundColor', cm( floor( " << (double(colors[pTsk->name()])/double(colorcount)) << " * cs )+1, :), ";
            fout << "'EdgeColor', [0 0 0], 'HorizontalAlignment', 'center' );" << endl;
            xoffset -= xskip;
        }
        minxoffset = ( xoffset < minxoffset ) ? xoffset : minxoffset;
    }

    for ( pBus = buses.begin(); pBus != buses.end(); pBus++ )
    {
        fout << "text( " << procxoffset << ", " << yoffsetmap[ pBus->name() ]+procyoffset << ", '" << pBus->name() << "', ";
        //fout << "', 'BackgroundColor', cm( floor( " << (double(color[pBus->name()])/double(colorcount)) << " * cs )+1, :), ";
        fout << "'EdgeColor', [0 0 0], 'HorizontalAlignment', 'center' );" << endl;
        set< Message > msgs = pBus->Message_children();
        set< Message >::iterator pMsg;
        int xoffset = xstart;
        for ( pMsg = msgs.begin(); pMsg != msgs.end(); pMsg++ )
        {
            fout << "text( " << xoffset << ", " << yoffsetmap[ pBus->name() ]+taskyoffset << ", '" << pMsg->name() << "', ";
            fout << "'BackgroundColor', cm( floor( " << (double(colors[pMsg->name()])/double(colorcount)) << " * cs )+1, :), ";
            fout << "'EdgeColor', [0 0 0], 'HorizontalAlignment', 'center' );" << endl;
            xoffset -= xskip;
        }
        minxoffset = ( xoffset < minxoffset ) ? xoffset : minxoffset;
    }

    // Readjust the axis for the new text
    fout << "v = axis();" << endl;
    fout << "axis( [ " << minxoffset << " v(2) v(3) v(4)] );" << endl;

    // Now the lines
    //vector< vector< double > >::iterator pLine;
    /*for ( pLine = lineends.begin(); pLine != lineends.end(); pLine++ )
    {
        fout << "line( [" << (*pLine)[0] << ", " << (*pLine)[2] << "], ";
        fout << "[ " << (*pLine)[1] << ", " << (*pLine)[3] << "], 'Marker', 'x', 'Color', 'k' );" << endl;
    }*/

    fout.close();

}

// One for Text files
void ConstraintMgr::WriteSchedule( const std::string & prefix, const std::string & sln_name )
{
	int rescale = 1000000;

    //ConstraintProblem cp = ConstraintProblem::Cast( s.parent() );
    InstanceRef ir = _cp.InstanceRef_child();
    InstanceModel im = ir.ref();
    long long int global_period = im.HyperPeriod() * rescale;

    // Get the solution
    Solution s = _solutions[sln_name];

    // First, build an index of all of the answers in the solution
    set< Assignment > anslist = s.Assignment_children();
    set< Assignment >::iterator pAns;
    map< string, int > answers;
    for ( pAns = anslist.begin(); pAns != anslist.end(); pAns++ )
    {
        answers[ pAns->name() ] = (int) pAns->Value();
    }

    // Iterate over the processors in the instance model
    //   Create one file for each processor -- tasks, period, offset
    // Iterate over the buses in the instance model
    //   Create one file for each bus -- msgs, period, offset
    //   For filenames, use solution name + processor or bus name.scd
	map< string, string > taskproc; // Store processor for each task
    set< Processor > procs = im.Processor_children();
    set< Processor >::iterator pProc;
    for ( pProc = procs.begin(); pProc != procs.end(); pProc++ )
    {	
        multimap< int, string > tasksort;  // Sort the tasks according to their offset
        TaskInstances ti = pProc->TaskInstances_child();
        set< TaskInstance > instances = ti.TaskInstance_children();
        set< TaskInstance >::iterator pInst;
        for ( pInst = instances.begin(); pInst != instances.end(); pInst++ )
        {
            // Make an offset-sorted list of tasks
            tasksort.insert( make_pair( answers[ pInst->name() ], pInst->RefName() ) ); 
			taskproc[ pInst->RefName() ] = pProc->name();
        }

        // Create a file here with the proper name for the processor
        string pfname = prefix + string("_") + string(s.name()) + string("_p") + string(pProc->name()) + string(".scd");

        ofstream fout;
        fout.open( pfname.c_str() );

        // Write them out to the file
        multimap< int, string >::iterator pTS;
		fout << left << global_period<< endl;
        for ( pTS = tasksort.begin(); pTS != tasksort.end(); pTS++ )
        {
            fout << left << setw(20) << pTS->second << setw(20) << pTS->first * rescale << endl;
        }

        fout.close();
    }

    set< Bus > buses = im.Bus_children();
    set< Bus >::iterator pBus;
    for ( pBus = buses.begin(); pBus != buses.end(); pBus++ )
    {
		Dependencies dep = pBus->Dependencies_child();
		set< SenderInst > sndPairs = dep.SenderInst_children();
		map< string, string > senders;
		set< SenderInst >::iterator pSndPair;
		for ( pSndPair = sndPairs.begin(); pSndPair != sndPairs.end(); pSndPair++ )
		{
			// Get sender end name (instance->RefName())
			TaskInstRef tir = pSndPair->srcSenderInst_end();
			TaskInstance ti = tir.ref();

			// Get message end name (instance->RefName())
			MsgInstRef mir = pSndPair->dstSenderInst_end();
			MsgInstance mi = mir.ref();

			senders[ mi.RefName() ] = ti.RefName();
		}

        multimap< int, string > msgsort;  // Sort the tasks according to their offset
        MsgInstances ti = pBus->MsgInstances_child();
        set< MsgInstance > instances = ti.MsgInstance_children();
        set< MsgInstance >::iterator pInst;
        for ( pInst = instances.begin(); pInst != instances.end(); pInst++ )
        {
            // Make an offset-sorted list of tasks
            msgsort.insert( make_pair( answers[ pInst->name() ], pInst->RefName() ) ); 
        }

        // Create a file here with the proper name for the bus
        string pfname = prefix + string("_") + string(s.name()) + string("_b") + string(pBus->name()) + string(".scd");

        ofstream fout;
        fout.open( pfname.c_str() );

        // Write them out to the file
		fout << left << global_period << endl;
        multimap< int, string >::iterator pTS;
        for ( pTS = msgsort.begin(); pTS != msgsort.end(); pTS++ )
        {
            fout << left << setw(20) << pTS->second << setw(20) << taskproc[senders[pTS->second]] << setw(20) << pTS->first * rescale << endl;
        }

        fout.close();
    }
}

// One for instance model
void ConstraintMgr::UpdateInstanceModel( const std::string & sln_name )
{
}
