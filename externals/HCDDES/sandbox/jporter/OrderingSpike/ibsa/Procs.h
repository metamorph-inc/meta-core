//
// Processor management and data structures
//

#ifndef _PROCS_H_
#define _PROCS_H_

#include <set>
#include <map>
#include <iostream>

#include "IBSATypes.h"
#include "TopoGraph.h"

class Schedule;

class ProcSorter
{
public:
    // Ranked by score -- 
    // Make sure the tie breakers always go to the closest
    // processor the entry node processor (encoded in hops)
    // If that's a tie, use the better relative speed
    bool operator()( const TGVertex & tgv1, const TGVertex & tgv2 )
    {
        if ( (*_tg)[tgv1].score == (*_tg)[tgv2].score )
        {
            if ( (*_tg)[tgv1].hops == (*_tg)[tgv2].hops )
            {
                return (*_tg)[tgv1].relspeed > (*_tg)[tgv2].relspeed;
            }
            else
            {
                return (*_tg)[tgv1].hops < (*_tg)[tgv2].hops;
            }
        }
        else
        {
            return (*_tg)[tgv1].score > (*_tg)[tgv2].score;
        }
    }

    static void Setup( TopoGraph & tg ) { _tg = &tg; }

private:
    static TopoGraph * _tg;
};

typedef std::set< TGVertex, ProcSorter > ProcListType;
typedef std::map< idx_t, IndexSet > ProcMapType;
typedef std::map< idx_t, idx_t > VertexMapType;
typedef std::map< TGVertex, idx_t > TimeMapType;

class ProcMap;

class ProcList
{
public:
    ProcList( TopoGraph & tg ) : _tg( tg ) { ProcSorter::Setup( tg ); }
    ProcList( ProcList & pl ) : 
        _tg( pl.getTopoGraph() ), 
        _procList( pl.getProcListData() ) { ProcSorter::Setup( _tg ); } 

    void sortProcs( const TGVertex & p, ProcMap & procs, Schedule & s );

    TGVertex getFirstProc() { return *_procList.begin(); } 
    bool removeFirstProc() { 
        if (_procList.empty()) return false;
        _procList.erase( _procList.begin() );
        return true;
    }

    bool empty() { return _procList.empty(); }

    void print() {
        std::cout << "Procs:" << std::endl;
        for ( ProcListType::iterator iproc = _procList.begin();
              iproc != _procList.end(); iproc++ )
        {
            std::cout << "    " << *iproc 
                << " score: " << _tg[*iproc].score
                << " hops: " << _tg[*iproc].hops
                << " relspeed: " << _tg[*iproc].relspeed
                << " type: " << ((_tg[*iproc].vtype == Proc) ? "Proc" :((_tg[*iproc].vtype == Bus) ? "Bus" : "unknown"))
                << std::endl;
        }
    }
                                                              
    TopoGraph & getTopoGraph() { return _tg; }
    ProcListType getProcListData() { return _procList; }

protected:

    TopoGraph & _tg;
    ProcListType _procList;
};


class ProcMap
{
public:
    ProcMap( TopoGraph & tg ) : _tg( tg ) { }
    virtual ~ProcMap() { }

	idx_t scaleRuntime( const TGVertex & v, idx_t runtime ) {
		
		double relspeed = _tg[v].relspeed;
		return (idx_t) (((double) runtime) / ((relspeed < 1.0e-6) ? 1.0 : relspeed) + 0.5);
	}

    idx_t getTimeUsed( const TGVertex & v ) {
        return (idx_t) (((double) _times[v]) / _tg[v].relspeed);
    }

    void addTimeUsed( const TGVertex & v, idx_t time ) {

        idx_t t = _times[v];
        _times[v] = t + time;
    }

    void clearTimeUsed() { _times.clear(); }

    IndexSet getVertices( const TGVertex & tg ) { 

        return _procmap[ tg ]; 
    }

    void assignTask( const TGVertex & p, const DGVertex & t) {

        VertexMapType::iterator rslt = _taskmap.find( t );

        if ( rslt != _taskmap.end() ) // if we found it, remove it from other lists
        {
            _procmap[rslt->first].erase(t);
        }
        _taskmap[t] = p;
        _procmap[p].insert(t);
    }

    void assignMsg( const TGVertex & b, DGEdge & e, DepGraph & g ) {

        idx_t edgeindex = boost::get( boost::edge_index, g, e );
        assignMsg( b, edgeindex );
    }

    void assignMsg( const TGVertex & b, idx_t edgeidx ) {

        VertexMapType::iterator rslt = _msgmap.find( edgeidx );

        if ( rslt != _msgmap.end() ) // if we found it, remove it from other lists
        {
            _procmap[rslt->first].erase(edgeidx);
        }
        _msgmap[edgeidx] = b;
        _procmap[b].insert(edgeidx);
    }

    void assignTasks( const TGVertex & p, IndexVector v ) {

        for( IndexVector::iterator vi = v.begin(); vi != v.end(); vi++ )
        {
            assignTask( p, *vi );
        }
    }

    void assignMsgs( const TGVertex & p, IndexVector v ) {

        for( IndexVector::iterator vi = v.begin(); vi != v.end(); vi++ )
        {
            assignMsg( p, *vi );
        }
    }

    const TGVertex getTaskProc( const DGVertex & t ) {

        return _taskmap[t];
    }

    const TGVertex getMsgBus( const DGEdge & m, DepGraph & g ) {

        return _msgmap[ boost::get( boost::edge_index, g, m ) ];
    }

    void print() {

        std::cout << "ProcMap: " << std::endl;
        for ( ProcMapType::iterator pmi = _procmap.begin(); 
              pmi != _procmap.end(); pmi++ )
        {
            if ( _tg[pmi->first].vtype == Proc )
            {
                std::cout << "    P " << pmi->first << " ";
                for ( IndexSet::iterator it = pmi->second.begin();
                      it != pmi->second.end(); it++ )
                {
                    std::cout << *it << "(" << _taskmap[*it] << ") ";
                }
            }
            else
            {
                std::cout << "    B " << pmi->first << " ";
                for ( IndexSet::iterator it = pmi->second.begin();
                      it != pmi->second.end(); it++ )
                {
                    std::cout << *it << "(" << _msgmap[*it] << ") ";
                }
            }
            std::cout << std::endl;
        }
    }

    IndexSet getProcTasks( const TGVertex & p ) {
        return _procmap[p];
    }

    IndexSet getBusMsgs( const TGVertex & b ) {
        return _procmap[b];
    }

protected:

    TopoGraph & _tg;
    TimeMapType _times; // durations ()
    ProcMapType _procmap; // proc -> set of tasks (or bus->set of msgs)
    VertexMapType _taskmap; // task -> proc
    VertexMapType _msgmap;  // msg -> bus

};

#endif // _PROCS_H_
