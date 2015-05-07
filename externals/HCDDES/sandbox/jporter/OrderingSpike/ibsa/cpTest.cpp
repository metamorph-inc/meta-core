

#include <iostream>

#include "DepGraph.h"
#include "Schedule.h"

using namespace std;

int main (int argc, char *argv[])
{
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
    return(0);
}

