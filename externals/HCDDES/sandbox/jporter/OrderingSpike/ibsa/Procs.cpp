//
// Declaration of static member instance for ProcSorter


#include "TopoGraph.h"
#include "Procs.h"
#include "Schedule.h"

TopoGraph * ProcSorter::_tg;

using namespace std;
using namespace boost;

void ProcList::sortProcs( const TGVertex & entry_proc, 
                          ProcMap & procs, Schedule & s )
{
    // Things to do for each proc:
    //   Update all scores
    //   Update the used time

    // Walk the schedule:
    //    For each proc, get the sorted vertex list
    //    Add the time consumed to the used space for that proc
    //    For each bus, get the sorted vertex list
    //    Add the time consumed to the used space for that bus
    
    procs.clearTimeUsed();

    // TODO: get rid of this hack - it won't work for a decent network
    idx_t bustime = 0;
    TGVertexIter tgv, tgv_end;
    for ( tie( tgv, tgv_end ) = vertices( _tg ); tgv != tgv_end; tgv++ )
    {
        if ( _tg[*tgv].vtype == Proc )
        {
            IndexSet taskvertices = procs.getVertices( *tgv );
            TaskSet_SortedByStart tset = s.getStartOrderedTasks( taskvertices );
            TaskSet_SortedByStart::iterator tidx;
            for ( tidx = tset.begin(); tidx != tset.end(); tidx++ )
            {
                procs.addTimeUsed( *tgv, (*tidx).finish - (*tidx).start );
                bustime = (*tidx).finish - (*tidx).start;
            }
        }

        if ( _tg[*tgv].vtype == Bus )
        {
            IndexSet msgvertices = procs.getVertices( *tgv );
            MsgSet_SortedByStart mset = s.getStartOrderedMsgs( msgvertices );
            MsgSet_SortedByStart::iterator midx;
            for ( midx = mset.begin(); midx != mset.end(); midx++ )
            {
                procs.addTimeUsed( *tgv, (*midx).finish - (*midx).start );
            }
        }
    }

    // Create the weighting function
    // Assume the network is a single connected graph
    // TODO: This is a cheap imitation.  Replace with a real hop distance search.
    _tg[ entry_proc ].hops = 0;
    for ( tie( tgv, tgv_end ) = vertices( _tg ); tgv != tgv_end; tgv++ )
    {
        if ( (_tg[*tgv].vtype == Proc) && (*tgv != entry_proc ))
        {
            _tg[ *tgv ].hops = 1;
        }
    }

    // For each proc, update its score
    // TODO: The ideal is to weight up linearly by distance
    // from entry_proc, by building a level set of cost:
    //  Accumulate total message cost as we go through a bus vertex,
    //  Accumulate total task cost as we go through a proc
    //  Always traverse depth-first through the lowest-time objects first
    
    // TODO: Other cool ideas
    // Work out the topology -- if we have multi-core processors, they shouldn't 
    // necessarily have an intervening bus object (or maybe a fast one).
    
    for ( tie( tgv, tgv_end ) = vertices( _tg ); tgv != tgv_end; tgv++ )
    {
        if ( _tg[*tgv].vtype == Proc )
        {
            _tg[*tgv].score = _tg[ *tgv ].hops * (procs.getTimeUsed( entry_proc ) 
                                                  + bustime) + procs.getTimeUsed(*tgv);
            _procList.insert( *tgv );
        }
    }
}
