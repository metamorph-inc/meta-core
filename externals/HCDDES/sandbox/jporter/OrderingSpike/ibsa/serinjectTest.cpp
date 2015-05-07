//
// Test Serial Injection
//

#include <iostream>

#include "DepGraph.h"
#include "Schedule.h"
#include "IBSA.h"

using namespace std;

int main (int argc, char *argv[])
{
    // HARDWARE TOPOLOGY GRAPH

    TopoGraph tg(5);
    tg[0].vtype = Proc;
    tg[1].vtype = Proc;
    tg[2].vtype = Proc;
    tg[3].vtype = Proc;
    tg[4].vtype = Bus;

    tg[0].relspeed = 4.0;
    tg[1].relspeed = 3.0;
    tg[2].relspeed = 2.0;
    tg[3].relspeed = 1.0;

    tg[0].score = 77.0;
    tg[1].score = 77.0;
    tg[2].score = 103.1;
    tg[3].score = 11.0;

    add_edge( 0, 4, tg );
    add_edge( 1, 4, tg );
    add_edge( 2, 4, tg );
    add_edge( 3, 4, tg );

    // TASK DEPENDENCY GRAPH

    DepGraph g(10);

    g[0].cost = 5;
    g[1].cost = 4;
    g[2].cost = 12;
    g[3].cost = 1;
    g[4].cost = 8;
    g[5].cost = 3;
    g[6].cost = 11;
    g[7].cost = 14;
    g[8].cost = 1;
    g[9].cost = 4;

    g[0].bound_to_proc = 0;
    g[1].bound_to_proc = -1;
    g[2].bound_to_proc = -1;
    g[3].bound_to_proc = -1;
    g[4].bound_to_proc = -1;
    g[5].bound_to_proc = -1;
    g[6].bound_to_proc = -1;
    g[7].bound_to_proc = -1;
    g[8].bound_to_proc = -1;
    g[9].bound_to_proc = 0;
    
    pair< DGEdge, bool > e;
    e = add_edge( 0, 4, g );
    g[e.first].cost = 2;
    e = add_edge( 0, 1, g );
    g[e.first].cost = 2;
    e = add_edge( 1, 2, g );
    g[e.first].cost = 2;
    e = add_edge( 2, 3, g );
    g[e.first].cost = 2;
    e = add_edge( 1, 4, g );
    g[e.first].cost = 2;
    e = add_edge( 2, 4, g );
    g[e.first].cost = 2;
    e = add_edge( 3, 4, g );
    g[e.first].cost = 2;
    e = add_edge( 3, 5, g );
    g[e.first].cost = 2;
    e = add_edge( 5, 8, g );
    g[e.first].cost = 2;
    e = add_edge( 4, 6, g );
    g[e.first].cost = 2;
    e = add_edge( 4, 8, g );
    g[e.first].cost = 2;
    e = add_edge( 6, 7, g );
    g[e.first].cost = 2;
    e = add_edge( 6, 8, g );
    g[e.first].cost = 2;
    e = add_edge( 7, 9, g );
    g[e.first].cost = 2;
    e = add_edge( 8, 9, g );
    g[e.first].cost = 2;

    ProcList proclist( tg );
    ProcMap  procmap( tg );

    Schedule sched( num_vertices(g), num_edges(g) );
    proclist.sortProcs( vertex(0, tg), procmap, sched );

    proclist.print();

    cout << endl;
    cout << "Critical Path: " << endl;
    IndexVector cp = DGUtils::findCriticalPath( g );
    DGUtils::print( cp, g );

    cout << endl;
    cout << "All Vertices: " << endl;
    IndexVector all;
    for ( idx_t idx = 0; idx < num_vertices(g); idx++ )
    {
        all.push_back(idx);
    }
    DGUtils::print( all, g );



    IBSA ibsa;
    IndexVector cds = ibsa.serialInjectBFS( proclist.getFirstProc(), g, cp, tg, procmap, sched );
    cout << endl;
    cout << "CDS Vertices: " << endl;
    DGUtils::print( cds, g );
    
    cout << endl;
    procmap.print();

    cout << endl;
    sched.print();

    return(0);
}

