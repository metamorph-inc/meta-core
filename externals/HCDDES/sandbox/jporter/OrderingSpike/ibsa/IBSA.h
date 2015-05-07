#ifndef __IBSA_H_INCL__
#define __IBSA_H_INCL__

#include <list>

#include "DepGraph.h"
#include "TopoGraph.h"
#include "Procs.h"
#include "Schedule.h"


typedef std::list< idx_t > IndexList;

/**
 * TODO: Add class description
 * 
 * @author   jporter
 */
class IBSA
{
public:
    // Constructor
    IBSA();

    // Destructor
    virtual ~IBSA();


    IndexVector serialInject( const TGVertex & p, DepGraph & g, 
                              const IndexVector & cp, TopoGraph & tg, 
                              ProcMap & procs, Schedule & sch );

    IndexVector serialInjectBFS( const TGVertex & p, DepGraph & g, 
                                 const IndexVector & cp, TopoGraph & h, 
                                 ProcMap & procs, Schedule & sch );

    // Copy constructor
    // TODO: Uncomment the copy constructor when you need it.
    //IBSA(const IBSA& src);

    // Assignment operator
    // TODO: Uncomment the assignment operator when you need it.
    //IBSA& operator=(const IBSA& src);

    void bsa_fw( ProcList & pl, ProcMap & pm, Schedule & sch, DepGraph & g, TopoGraph & tg );

	IndexPair find_best_move_fw( const DGVertex & t, const TGVertex & curProc, ProcMap & pm, 
								 TopoGraph & tg, DepGraph & g, Schedule & sch );

protected:

    //IndexVector orderPred( const DGVertex & v, DepGraph & g, 
    void orderPred( const DGVertex & v, DepGraph & g, 
                           IndexVector & seq );

    void orderPredBFS( IndexList & q, DepGraph & g, 
                       IndexVector & seq );

    DepGraph _original;
    DepGraph _addon;
};

// Constructor implementation
inline IBSA::IBSA()
{
}

// Destructor implementation
inline IBSA::~IBSA()
{
}

// TODO: Uncomment the copy constructor when you need it.
//inline IBSA::IBSA(const IBSA& src)
//{
//   // TODO: copy
//}

// TODO: Uncomment the assignment operator when you need it.
//inline IBSA& IBSA::operator=(const IBSA& rhs)
//{
//   if (this == &rhs) {
//      return *this;
//   }
//
//   // TODO: assignment
//
//   return *this;
//}

#endif // __IBSA_H_INCL__
