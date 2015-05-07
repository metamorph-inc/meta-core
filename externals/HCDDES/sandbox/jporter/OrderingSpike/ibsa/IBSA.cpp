#include "IBSA.h"

#include <exception>

using namespace std;
using namespace boost;

class TaskKeeperEmpty : public std::exception
{
public:
	virtual const char * what() const throw () { return "Requested next task from an empty task keeper."; }
};

class TaskKeeper {
	
public:
	TaskKeeper( TGVertex & p, ProcMap & procs, Schedule & sch ) {
		
		IndexSet tasks = procs.getProcTasks( p );
		TaskSet_SortedByStart sorted_tasks = sch.getStartOrderedTasks( tasks );
		
		_task_list.clear();
		
		for ( TaskSet_SortedByStart::iterator taskIter = sorted_tasks.begin(); taskIter != sorted_tasks.end(); 
				taskIter++ )
		{
			_task_list.push_back( (*taskIter).task );
			
		}
	}
	
	bool empty() { return _task_list.empty(); }
	
	idx_t getNextTask() throw (TaskKeeperEmpty) {
		
		if ( empty() )
		{
			throw TaskKeeperEmpty();
		}
		
		idx_t next_task = _task_list.front();
		_task_list.pop_front();
		return next_task;
	}
	
protected:
	
	IndexList _task_list;
};

IndexVector IBSA::serialInject( const TGVertex & p, DepGraph & g, 
                                const IndexVector & cp, TopoGraph & h, 
                                ProcMap & procs, Schedule & sch ) 
{
    DGVertexBlevelSorter::Setup( g );
    IndexVector cds;
    //idx_t cpidx;
    
    //cds.push_back( cp[0] );

    //cpidx = 1;

    DGUtils::clearDistances( g );

    // Mark all cp vertices that have no IB nodes
    /*DGVertexIter vi, vi_end;
    for ( tie(vi, vi_end) = vertices(g);
          vi != vi_end; vi++ )
    {
        if ( g[*vi].vtype == CP && in_degree(*vi, g) < 2 )
        {
            put( vertex_distance, g, *vi, 1 ); // mark vertex
        }
    }

    while ( cpidx < cp.size() )
    {
        if ( get( vertex_distance, g, *vi ) != 1 )
        {
            IndexVector dummyseq;
            IndexVector ibseq = orderPred( *vi, g, dummyseq );
            cds.insert( cds.end(), ibseq.begin(), ibseq.end() );

            for ( IndexVector::iterator ibIter = ibseq.begin(); ibIter != ibseq.end(); ibIter++ )
            {
                put( vertex_distance, g, *ibIter, 1 ); // mark sequence
            }
        }

        cds.push_back( cp[cpidx++] );
    } */

    // TODO: collect all of the unmarked vertices sorted by 
    // descending blevel, and add them to cds in that order


    // This is actually just a DFS visit of the graph g, recording nodes 
    // backwards in decreasing blevel order.  That gives first priority
    // to edge dependencies, and second priority to blevels.

    // First collect all of the starting vertices in blevel order
    DGVertexSet_SortedByBlevel starters;

    DGVertexIter vi, vi_end;
    for ( tie( vi, vi_end ) = vertices(g); vi != vi_end; vi++ )
    {
        if ( in_degree( *vi, g ) == 0 )
        {
            starters.insert(*vi);
        }
    }

    for ( DGVertexSet_SortedByBlevel::iterator svi = starters.begin(); svi != starters.end(); svi++ )
    {
        if ( get( vertex_distance, g, *svi ) == 0 ) // unmarked
        {
            //cout << "Visiting start vertex " << *svi << endl;
            orderPred( *svi, g, cds );
        }
    }

    //cout << "Done visiting." << endl;

    IndexVector rcds;
    // Schedule and assign to processor
    for ( IndexVector::reverse_iterator ivi = cds.rbegin(); ivi != cds.rend(); ivi++ )
    { 
        sch.update( *ivi, g, p, procs, h ); // assign times
        procs.assignTask( p, *ivi ); // add sequence to proc
        rcds.push_back( *ivi );
    }

    return rcds;
}

// TODO: it might be necessary to adjust the sorter so that it
// puts CP vertices last, and IB vertices first
//IndexVector IBSA::orderPred( const DGVertex & v, DepGraph & g, 
// TODO:  More testing to make sure the CP sequence is preserved
void IBSA::orderPred( const DGVertex & v, DepGraph & g, 
                      IndexVector & seq ) {

    put( vertex_distance, g, v, 1 ); // visited, but not processed
    //cout << "Marked " << v << " as 1" << endl;

    DGVertexSet_SortedByBlevel neighbors;

    //cout << "Collected ";
    DGAdjIter dai, dai_end;
    for ( tie( dai, dai_end ) = adjacent_vertices(v, g); dai != dai_end; dai++ )
    {
        //cout << *dai << " ";
        neighbors.insert( *dai );
    }
    //cout << endl;

    for ( DGVertexSet_SortedByBlevel::iterator svi = neighbors.begin(); 
          svi != neighbors.end(); svi++ )
    {
        //cout << "Checking vertex " << *svi << endl;
        if ( get( vertex_distance, g, *svi ) == 0 ) // unmarked
        {
            //cout << "Visiting vertex " << *svi << endl;
            orderPred( *svi, g, seq );
        }
    }

    //cout << "Marking " << v << " as 2" << endl;
    put( vertex_distance, g, v, 2 ); // processed
    //cout << "Sequencing " << v << endl;
    seq.push_back( v );
    //cout << "Seq so far: ";
    //for ( idx_t idx = 0; idx < seq.size(); idx++ )
    //{
    //    cout << seq[idx] << " ";
    //}
    //cout << endl;
}


IndexVector IBSA::serialInjectBFS( const TGVertex & p, DepGraph & g, 
                                   const IndexVector & cp, TopoGraph & h, 
                                   ProcMap & procs, Schedule & sch ) 
{
    DGVertexBlevelSorter::Setup( g );
    IndexVector cds;
    //idx_t cpidx;
    
    //cds.push_back( cp[0] );

    //cpidx = 1;

    DGUtils::clearDistances( g );

    // Mark all cp vertices that have no IB nodes
    /*DGVertexIter vi, vi_end;
    for ( tie(vi, vi_end) = vertices(g);
          vi != vi_end; vi++ )
    {
        if ( g[*vi].vtype == CP && in_degree(*vi, g) < 2 )
        {
            put( vertex_distance, g, *vi, 1 ); // mark vertex
        }
    }

    while ( cpidx < cp.size() )
    {
        if ( get( vertex_distance, g, *vi ) != 1 )
        {
            IndexVector dummyseq;
            IndexVector ibseq = orderPred( *vi, g, dummyseq );
            cds.insert( cds.end(), ibseq.begin(), ibseq.end() );

            for ( IndexVector::iterator ibIter = ibseq.begin(); ibIter != ibseq.end(); ibIter++ )
            {
                put( vertex_distance, g, *ibIter, 1 ); // mark sequence
            }
        }

        cds.push_back( cp[cpidx++] );
    } */

    // TODO: collect all of the unmarked vertices sorted by 
    // descending blevel, and add them to cds in that order


    // This is actually just a DFS visit of the graph g, recording nodes 
    // backwards in decreasing blevel order.  That gives first priority
    // to edge dependencies, and second priority to blevels.

    // First collect all of the starting vertices in blevel order
    DGVertexMultiset_SortedByBlevel starters;

    DGVertexIter vi, vi_end;
    for ( tie( vi, vi_end ) = vertices(g); vi != vi_end; vi++ )
    {
        //if ( in_degree( *vi, g ) == 0 )
        //{
            starters.insert(*vi);
        //}
    }

    //for ( DGVertexSet_SortedByBlevel::iterator svi = starters.begin(); svi != starters.end(); svi++ )
    //{
    //    cds.push_back( *svi );
    //}

    //IndexList q;
    //for ( DGVertexSet_SortedByBlevel::iterator svi = starters.begin(); svi != starters.end(); svi++ )
    //{
    //    if ( get( vertex_distance, g, *svi ) == 0 ) // unmarked
    //    {
    //        //cout << "Visiting start vertex " << *svi << endl;
    //        put( vertex_distance, g, *svi, 1);
    //        q.push_back( *svi );
    //    }
    //}

    //orderPredBFS( q, g, cds );

    //cout << "Done visiting." << endl;

    // Schedule and assign to processor
    //for ( IndexVector::reverse_iterator ivi = cds.rbegin(); ivi != cds.rend(); ivi++ )
    for ( DGVertexMultiset_SortedByBlevel::reverse_iterator ivi = starters.rbegin(); ivi != starters.rend(); ivi++ )
    { 
        sch.update( *ivi, g, p, procs, h ); // assign times
        procs.assignTask( p, *ivi ); // add sequence to proc
        cds.push_back( *ivi );
    }

    return cds;
}

// TODO: it might be necessary to adjust the sorter so that it
// puts CP vertices last, and IB vertices first
//IndexVector IBSA::orderPred( const DGVertex & v, DepGraph & g, 
// TODO:  More testing to make sure the CP sequence is preserved
void IBSA::orderPredBFS( IndexList & q, DepGraph & g, IndexVector & seq ) {

    
    while (!q.empty())
    {
        DGVertex v = q.front();
        q.pop_front();
    
        DGVertexSet_SortedByBlevel neighbors;
    
        //cout << "Collected ";
        DGAdjIter dai, dai_end;
        for ( tie( dai, dai_end ) = adjacent_vertices(v, g); dai != dai_end; dai++ )
        {
            //cout << *dai << " ";
            put( vertex_distance, g, v, 1 ); // visited, but not processed
            neighbors.insert( *dai );
        }
        //cout << endl;
    
        for ( DGVertexSet_SortedByBlevel::iterator svi = neighbors.begin(); 
              svi != neighbors.end(); svi++ )
        {
            //cout << "Checking vertex " << *svi << endl;
            if ( get( vertex_distance, g, *svi ) == 0 ) // unmarked
            {
                //cout << "Visiting vertex " << *svi << endl;
                put( vertex_distance, g, *svi, 1 ); // visited, but not processed
                q.push_back( *svi );
            }
        }
    
        //cout << "Marking " << v << " as 2" << endl;
        put( vertex_distance, g, v, 2 ); // processed
        //cout << "Sequencing " << v << endl;
        seq.push_back( v );
        //cout << "Seq so far: ";
        //for ( idx_t idx = 0; idx < seq.size(); idx++ )
        //{
        //    cout << seq[idx] << " ";
        //}
        //cout << endl;
    }
}

void IBSA::bsa_fw( ProcList & pl, ProcMap & pm, Schedule & sch, 
                        DepGraph & g, TopoGraph & tg )
{
    ProcList curProcList( pl );
    TGVertex curProc;

    // Iterate over the processors in priority order
    while ( !curProcList.empty() )
    {
        curProc = curProcList.getFirstProc();
        
        // Get the tasks for this processor, in start time order
        // This iteration has a fly-away problem, as we may move the
        //   current task off the processor during the loop...
 		// TaskKeeper has a local copy of the task list for the current processor.
		TaskKeeper tk( curProc, pm, sch );
		
		// loop construct over t in tasks 
		while ( !tk.empty() )
		{
			idx_t t = tk.getNextTask();
			
			IndexPair rslt = find_best_move_fw( t, curProc, pm, tg, g, sch );
			
			if ( rslt.second != curProc )
			{
				// Perform the update
			}
		}

        pl.removeFirstProc();
    }	
}

IndexPair IBSA::find_best_move_fw( const DGVertex & t, const TGVertex & curProc, ProcMap & pm, 
							  	   TopoGraph & tg, DepGraph & g, Schedule & sch ) {
	
	// best finish time = finishtime(t)
    // bestProc = curProc
	TaskEntry te = sch.getTaskSched( t );
	IndexPair result;	
	idx_t bestFT = result.first = te.finish;
	TGVertex bestProc = result.second = curProc;
	
	// if ( start(t, curProc) > dat(t, curProc) or proc( vip(t) ) != curProc )
	if ( ( te.start > te.dat ) || ( pm.getTaskProc(te.vip) ) != curProc )
	{
        // for all proc in adj( curProc ) - two steps are required, because a vertex can be a bus
		TGAdjacencyIter bai, bai_end;
		for ( tie( bai, bai_end ) = adjacent_vertices( curProc, tg ); bai != bai_end; bai++ )
		{
			IndexSet visited;
			visited.insert(curProc);
			if ( tg[*bai].vtype == Bus ) {
				
				TGAdjacencyIter pai, pai_end;
				for ( tie( pai, pai_end ) = adjacent_vertices( curProc, tg ); pai != pai_end; pai++ )
				{
					if ( (tg[*pai].vtype == Proc) && (visited.find(*pai) == visited.end()) )
					{
						visited.insert(*pai);
						
						// finishtime = calcstart( t,proc) + dur(t,proc)
						idx_t newFT = sch.calcStartTime( t, *pai, g, pm ) + pm.scaleRuntime( *pai, g[te.task].cost );
						
                 		// if (finishtime < bestfinishtime) or (finishtime == bestfinishtime and proc( vip(t) ) == proc )
						if ( ( newFT < bestFT ) || ( newFT == bestFT && pm.getTaskProc(te.vip) == *pai ) )
						{
							// bestfinishtime = finishtime
                         	// bestProc = proc		
							bestFT = newFT;
							bestProc = *pai;
						}   									
					}
				}
			}
		}              

        // if bestProc neq curProc
		if ( bestProc != curProc )
		{
			// proc(t) = bestProc
            // update( g, tg, procList, t ) 
			
			result.first = bestFT;
			result.second = bestProc;
		}
    }

	return result;
}