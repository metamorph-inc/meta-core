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
    tg[2].vtype = Bus;

    tg[0].relspeed = 1.0;
    tg[1].relspeed = 1.0;
    tg[2].relspeed = 1.0;

    tg[0].score = 1.0;
    tg[1].score = 1.0;
    tg[2].score = 1.0;

    add_edge( 0, 2, tg );
    add_edge( 1, 2, tg );

    // TASK DEPENDENCY GRAPH

    DepGraph g(4);

    g[0].cost = 1;
    g[1].cost = 1;
    g[2].cost = 1;
    g[3].cost = 1;

    g[0].bound_to_proc = -1;
    g[1].bound_to_proc = -1;
    g[2].bound_to_proc = -1;
    g[3].bound_to_proc = -1;
    
    pair< DGEdge, bool > e;
    e = add_edge( 0, 1, g );
    g[e.first].cost = 1;
    e = add_edge( 1, 2, g );
    g[e.first].cost = 1;
    e = add_edge( 2, 3, g );
    g[e.first].cost = 1;

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

	IndexPair ip1 = ibsa.find_best_move_fw( 0, 0, procmap, tg, g, sched );
	cout << "Task 1 FT: " << ip1.first << " Proc: " << ip1.second << endl;
	IndexPair ip2 = ibsa.find_best_move_fw( 1, 0, procmap, tg, g, sched );
	cout << "Task 2 FT: " << ip2.first << " Proc: " << ip2.second << endl;
	IndexPair ip3 = ibsa.find_best_move_fw( 2, 0, procmap, tg, g, sched );
	cout << "Task 3 FT: " << ip3.first << " Proc: " << ip3.second << endl;
	IndexPair ip4 = ibsa.find_best_move_fw( 3, 0, procmap, tg, g, sched );
	cout << "Task 4 FT: " << ip4.first << " Proc: " << ip4.second << endl;

    return(0);
}

