#include <iostream>

#include "TopoGraph.h"
#include "Procs.h"
#include "Schedule.h"

/**
 * Main application entry point
 * 
 * @param argc The number of elements in the argv array
 * @param argv The command line arguments passed to the
 *             application
 * 
 * @return Completion code, typically 0 for success
 */

using namespace std;
using namespace boost;

int main (int argc, char *argv[])
{
    // Create a topo graph
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

    ProcList proclist( tg );
    ProcMap  procmap( tg );
    Schedule sched( 0, 0 );

    proclist.sortProcs( vertex(0, tg), procmap, sched );

    proclist.print();


    return 0;
}

