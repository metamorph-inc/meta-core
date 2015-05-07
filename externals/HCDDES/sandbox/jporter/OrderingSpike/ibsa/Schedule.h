//
// Datatype definitions 
//

#ifndef _SCHEDULE_H_
#define _SCHEDULE_H_

#include <vector>
#include <set>
#include <map>

#include "IBSATypes.h"
#include "Procs.h"


typedef struct 
{
    idx_t task;
    idx_t start;
    idx_t finish;    
    idx_t dat;
    idx_t vip;

} TaskEntry;
typedef std::vector< TaskEntry > SchedVectorType;

typedef struct 
{
    idx_t msg;
    idx_t start;
    idx_t finish;

} MsgEntry;
typedef std::vector< MsgEntry > MsgVectorType;

class TaskEntryStartSorter {
public:

    bool operator()( const TaskEntry & e1, const TaskEntry & e2 ) {
        return e1.start < e2.start;
    }
};

class TaskEntryFinishSorter {
public:

    bool operator()( const TaskEntry & e1, const TaskEntry & e2 ) {
        return e1.finish < e2.finish;
    }
};

class MsgEntryStartSorter {
public:

    bool operator()( const MsgEntry & e1, const MsgEntry & e2 ) {
        return e1.start < e2.start;
    }
};

class MsgEntryFinishSorter {
public:

    bool operator()( const MsgEntry & e1, const MsgEntry & e2 ) {
        return e1.finish < e2.finish;
    }
};

typedef std::set< TaskEntry, TaskEntryStartSorter > TaskSet_SortedByStart;
typedef std::set< TaskEntry, TaskEntryFinishSorter > TaskSet_SortedByFinish;
typedef std::set< MsgEntry, MsgEntryStartSorter > MsgSet_SortedByStart;
typedef std::set< MsgEntry, MsgEntryFinishSorter > MsgSet_SortedByFinish;

class Schedule 
{
public:
    Schedule() { }
    Schedule( idx_t ntasks, idx_t nmsgs ) :
        _tasks( ntasks, TaskEntry() ), 
        _msgs( nmsgs, MsgEntry() ) { }
    Schedule( const Schedule & sched ) : 
        _tasks( sched.getTasks() ), _msgs( sched.getMsgs() ) { }
    virtual ~Schedule() { }

    SchedVectorType getTasks() const { return _tasks; }
    MsgVectorType getMsgs() const { return _msgs; }
    TaskEntry  getTaskSched( idx_t v ) { return _tasks[v]; }
    MsgEntry    getMsgSched( idx_t e ) { return _msgs[e]; }

    template < class CollectionT >
    TaskSet_SortedByStart getStartOrderedTasks( CollectionT & tasks ) {
        TaskSet_SortedByStart schSet;
        for ( typename CollectionT::iterator vIdx = tasks.begin(); vIdx != tasks.end(); vIdx++ )
        {
            schSet.insert( _tasks[*vIdx] );
        }
        return schSet;
    } 

    template < class CollectionT >
    TaskSet_SortedByFinish getFinishOrderedTasks( CollectionT & tasks ) {
        TaskSet_SortedByFinish schSet;
        for ( typename CollectionT::iterator vIdx = tasks.begin(); vIdx != tasks.end(); vIdx++ )
        {
            schSet.insert( _tasks[*vIdx] );
        }
        return schSet;
    } 

    template < class CollectionT >
    MsgSet_SortedByStart getStartOrderedMsgs( CollectionT & msgs ) {
        MsgSet_SortedByStart schSet;
        for ( typename CollectionT::iterator vIdx = msgs.begin(); vIdx != msgs.end(); vIdx++ )
        {
            schSet.insert( _msgs[*vIdx] );
        }
        return schSet;
    } 

    template < class CollectionT >
    MsgSet_SortedByFinish getFinishOrderedMsgs( CollectionT & msgs ) {
        MsgSet_SortedByFinish schSet;
        for ( typename CollectionT::iterator vIdx = msgs.begin(); vIdx != msgs.end(); vIdx++ )
        {
            schSet.insert( _msgs[*vIdx] );
        }
        return schSet;
    } 

	// TODO: This needs to take into account the communication costs as well
    idx_t calcStartTime( const DGVertex & t, const TGVertex & p, DepGraph & g, ProcMap & procs ) {

		TaskEntry new_te;

        // Take the tasks from processor p, sorted in start order
        // Search the list until we find the first starting point
        idx_t tdur = g[t].cost;

        IndexSet tasks = procs.getProcTasks( p );
        TaskSet_SortedByStart stasks = getStartOrderedTasks(tasks);

        // Check before the first vertex
        TaskEntry te, te_prev;
        te = *(stasks.begin());
        if ( stasks.empty() || te.start >= tdur )
        {
            return 0; // start at the beginning
        }

        TaskSet_SortedByStart::iterator ti = stasks.begin();
        for ( ti++; ti != stasks.end(); ti++ )
        {
            te_prev = te;
            te = *ti;
            if ( te.start - te_prev.finish > tdur )
            {
                return te_prev.finish; // start between two tasks
            }
        }

		return te.finish;  // start after the end
    }

	TaskEntry calcTaskUpdate( idx_t starttime, const DGVertex & t, const TGVertex & p, 
							  DepGraph & g, TopoGraph & tg, ProcMap & procs ) {
		TaskEntry te;
		te.task = t;
		te.start = starttime;
		te.finish = starttime + procs.scaleRuntime(p, g[t].cost);
		std::pair< idx_t, idx_t > dat_vip = calcDATandVIP( t, g, procs );
		te.dat = dat_vip.first;
		te.vip = dat_vip.second;
		
		return te;
	}
	
	std::pair< idx_t, idx_t > calcDATandVIP( const DGVertex & t, DepGraph & g, ProcMap & procs ) {
	
		IndexPair dat_vip;
		
		// Create variables to track the latest possible finish+comm time,
		//   the task that belongs to it, and the processor it runs on
		
		// Record which processor currently holds t
		// Iterate over all of the tasks which precede t
		    // Find the latest, where latest is by finish time + comm time (if on different procs)
		
		// Write the latest time and proc into dat_vip
		
		return dat_vip;
	}


    // TODO: This version should work for plain vanilla serialInject and not much else
    //void update( IndexVector seq, DepGraph & g, ProcMap & procs, TopoGraph & h ) {
    void update( const DGVertex & t, DepGraph & g, const TGVertex & p, ProcMap & procs, TopoGraph & h ) {

        //TGVertex p_prev = procs.getTaskProc( *(seq.begin()));
        //for ( IndexVector::iterator si = seq.begin(); si != seq.end(); si++ )
        //{
        //    TGVertex p = procs.getTaskProc( *si );

        idx_t first_avail_time = calcStartTime( t, p, g, procs );

        //if ( p != p_prev ) // we moved off this proc, so include a message in the schedule
        //{
            // TODO: decide whether to merge into CalcStartTime (with a moved flag?)
            // find the message from the last seq element to this one,
            // & assign it to the next available connected bus slot which gives
            // the best finish time.

            // set the first available time to the end of the message (if later)
        //}

        // TODO: fix these to get the real updates -- it may be better to have
        // CalcStartTime return a TaskEntry with all of the right fields filled in
        TaskEntry te = _tasks[ t ];
        te.task = t;
        te.start = first_avail_time;
        te.finish = first_avail_time + g[t].cost; // TODO: use proc info to scale by speed
        te.dat = first_avail_time;
        te.vip = 0; // (si == seq.begin()) ? 0 : (*(t-1));
        _tasks[t] = te;

        //p_prev = p;
        //}

        // TODO: Figure out how to flag and move dependent vertices as we schedule.
        // We should probably set up a start-time-sorted task set, and add things to it
        // as we move through -- always process the next one in the set, and always add
        // updated dependent vertices before moving forward
    }


    void print() {
        std::cout << "Printing Schedule: " << std::endl;

        // TODO: These have to be broken down by hardware list, or else
        // the start times collide with each other and overwrite
        //TaskSet_SortedByStart tset;
        //MsgSet_SortedByStart mset;

        //tset.insert( _tasks.begin(), _tasks.end() );
        //mset.insert( _msgs.begin(), _msgs.end() );

        //for (TaskSet_SortedByStart::iterator ti = tset.begin(); ti != tset.end(); ti++ )
        for ( SchedVectorType::iterator ti = _tasks.begin(); ti != _tasks.end(); ti++ )
        {
            std::cout << "Task " << ti->task << ": st " << ti->start << " fi " << ti->finish;
            std::cout << " da " << ti->dat << " vip " << ti->vip << std::endl;
        }

        //for (MsgSet_SortedByStart::iterator mi = mset.begin(); mi != mset.end(); mi++ )
        for ( MsgVectorType::iterator mi = _msgs.begin(); mi != _msgs.end(); mi++ )
        {
            std::cout << "Msg " << mi->msg << ": st " << mi->start << " fi " << mi->finish << std::endl;;
        }
    }

protected:
    SchedVectorType _tasks;
    MsgVectorType   _msgs;
};

#endif // _SCHEDULE_H_
