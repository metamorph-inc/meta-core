#ifndef __INDEXMGR_H_INCL__
#define __INDEXMGR_H_INCL__

#include <vector>
#include <map>
#include <algorithm>

#include "Uml.h"

#include "CSched.h"

using namespace CSched;

/**
 * The index manager keeps an odd sort of database regarding 
 * tasks, messages, their instances and all of their 
 * relationships. 
 * 
 * @author   jporter
 */
class IndexMgr
{
    // collect string-type keys from a map of any type
    // or just names from any collection of named objects
    // Stolen shamelessly from discussion on 
    // http://www.thescripts.com/forum/thread60732.html
    // Modified 5/7/2007 jporter
    template <typename T>
    struct collectNames
    {
        std::vector<std::string> & _names;
        collectNames( std::vector<std::string> & names ) : _names( names ) {}
        void operator()( const std::pair< std::string, T > & p ) { _names.push_back( p.first ); }
        void operator()( const T & p ) { _names.push_back( p.name() ); }
    };

public:
    // Constructor
    IndexMgr();

    // Destructor
    virtual ~IndexMgr();

    void MapTasktoTaskInstances( Task & tsk, TaskInstances & ti )
    {
        // taskmap[ *pTsk ] = *pti;
        taskmap[ tsk.name() ] = ti;
        tasksbyname[ tsk.name() ] = tsk;
    }

    TaskInstances & GetTaskInstancesObjectforTask( std::string & tskName )
    {
        return taskmap[ tskName ];
    }

    Task & GetTaskbyName( std::string & tskName )
    {
        return tasksbyname[ tskName ];
    }

    void GetAllTaskNames( std::vector< std::string > & tskNames )
    {
        tskNames.clear();
        for_each( tasksbyname.begin(), tasksbyname.end(), collectNames< Task >( tskNames ) );
    }

	void GetAllTaskInstanceNames( std::vector< std::string > & instNames )
    {
        instNames.clear();
        for_each( inst_tsk_map.begin(), inst_tsk_map.end(), collectNames< Task >( instNames ) );
    }

    void GetProcInstanceNames( const std::string & procName, std::vector< std::string > & instNames )
    {
        instNames.clear();
		instNames = proclist[procName];
    }

    void GetBusInstanceNames( const std::string & busName, std::vector< std::string > & instNames )
    {
        instNames.clear();
		instNames = buslist[busName];
    }

    void StoreExactInstanceDistance( const std::string & firstInst, std::string & secondInst, int distance )
    {
        pdlist[ firstInst ] = make_pair( secondInst, distance );
    }

    bool GetNextInstanceAndDist( const std::string & firstInst, std::string & secondInst, int & distance )
    {
        std::map< std::string, pair< std::string, int > >::iterator foundIt;
        foundIt = pdlist.find( firstInst );

		if ( foundIt == pdlist.end() )
        {
            return false;
        }
        else
        {
            secondInst = (foundIt->second).first;
            distance = (foundIt->second).second;
        }
        return true;
    }

    void StoreInstanceforTask( Task & t, TaskInstance & ti )
    {
        task_inst_map[ t.name() ].push_back( ti ); // Also save this for later.
        inst_tsk_map[ ti.name() ] = t;
    }

    void StoreInstanceforMsg( Message & m, MsgInstance & mi )
    {
        msg_inst_map[ m.name() ].push_back( mi ); // Save these for later
        inst_msg_map[ mi.name() ] = m;
    }

    bool GetInstancesforTask( const std::string & tskName, std::vector< TaskInstance > & instances )
    {
        std::map< std::string, vector< TaskInstance > >::iterator pTskNInst;
        pTskNInst = task_inst_map.find( tskName );
		if ( pTskNInst == task_inst_map.end() )
        {
            return false;
        }
        else
        {
            instances.clear(); // really shouldn't need this, but just to be safe...
            instances = pTskNInst->second;
        }
        return true;
    }

    void GetTaskInstanceNames( const std::string & tskName, std::vector< std::string > & instNames )
    {
        instNames.clear();
        for_each( task_inst_map[tskName].begin(), task_inst_map[tskName].end(), collectNames< TaskInstance >(instNames) );
    }

    void GetMsgInstanceNames( const std::string & msgName, std::vector< std::string > & instNames )
    {
        instNames.clear();
        for_each( msg_inst_map[msgName].begin(), msg_inst_map[msgName].end(), collectNames< MsgInstance >(instNames) );
    }

    void StoreInstanceforProc( Processor & p, TaskInstance & ti )
    {
        proclist[ p.name() ].push_back( ti.name() );
    }

    void StoreInstanceforBus( Bus & b, MsgInstance & m )
    {
        buslist[ b.name() ].push_back( m.name() );
    }

protected:
    
    std::map< std::string , TaskInstances > taskmap; // task name -> TaskInstances object
    std::map< std::string, Task > inst_tsk_map;  // Task instance name -> Task object
    std::map< std::string, Task > tasksbyname;   // task name -> Task object

    std::set<Processor> newprocs;
    std::map< std::string, pair< std::string, int > > pdlist; // Keep list of task relationships (task inst -> next task inst)
    std::map< std::string, vector< std::string > > proclist; // Keep processor -> instances map for scheduling
    std::map< std::string, vector< TaskInstance > > task_inst_map; // Keep Task->Instances map for later

    std::set< Bus > newbuses;
    std::map< std::string, vector< std::string > > buslist; // Keep bus -> instances map for scheduling
    std::map< std::string, vector< MsgInstance > > msg_inst_map; // Keep Task->Instances map for later
    std::map< std::string, Message > inst_msg_map; // Keep reverse map
};

// Constructor implementation
inline IndexMgr::IndexMgr()
{
}

// Destructor implementation
inline IndexMgr::~IndexMgr()
{
}


#endif // __INDEXMGR_H_INCL__
