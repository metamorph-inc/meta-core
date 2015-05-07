#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <iterator>
#include <vector>
#include <set>
#include <map>
#include <algorithm>

#include "DepGraph.h"

using namespace boost;
using namespace std;

int main( int argc, char *argv[] )
{
    idx_t N = 10;  // Number of vertices
    if ( argc > 1 )
    {
        string num( argv[1]);
        istringstream nin( num );
        nin >> N;
    }

    double ccr = 0.1;
    if ( argc > 2 )
    {
	string num( argv[2] );
	istringstream nin( num );
	nin >> ccr;
    }

    if ( N < 5 )
    {
        cout << " N must be larger than 4. " << endl;
        exit( 1 );
    }

    DepGraph G(N); // start the graph with a random acyclic path
    //UnDepGraph UG(N);  // TODO: Remove this duplicate graph.  Use one undirected graph with an edge property specifying its direction
    //

    mt19937 rng;
    rng.seed(time(0));
    uniform_smallint< idx_t > vertex_cost_range(2, 78);
    variate_generator< mt19937 &, uniform_smallint< idx_t > > gen_vertex_cost( rng, vertex_cost_range );
    uniform_smallint< idx_t > vertex_children_range(1, (N/5));
    variate_generator< mt19937 &, uniform_smallint< idx_t > > gen_vertex_children( rng, vertex_children_range );
    uniform_smallint< idx_t > edge_cost_range(2, (2*((int) (ccr * 40.0)))-2);
    variate_generator< mt19937 &, uniform_smallint< idx_t > > gen_edge_cost( rng, edge_cost_range );
    
    for ( idx_t vidx = 0; vidx < N; vidx++ )
    {
        G[vidx].cost = gen_vertex_cost();
    }

    idx_t counter = 0;
    while ( counter < (N-1) )
    {
	idx_t nchildren = gen_vertex_children();
        nchildren = min( nchildren, (N-(counter+1)) ); //don't go over the limit
	for ( idx_t cidx = counter+1; cidx < (counter+1)+nchildren; cidx++ )
        {
	    pair< DepEdge, bool > e;
            e = add_edge( counter, cidx, G );
            //cout << "Creating edge " << counter << " " << cidx << endl;
            G[e.first].cost = gen_edge_cost();
        }
        counter++;
    }

    dynamic_properties dp;
    dp.property("vertex_distance", get( vertex_distance_t(), G ));
    dp.property("edge_weight", get( edge_weight_t(), G ));
    //write_graphml( cout, G, dp, true );
    write_graphviz( cout, G, DepLabelWriter(G), DepLabelWriter(G)  );

    return 0;
}

    // add durations
    //for( tie( v_begin, v_end ) = vertices( G ); v_begin != v_end; v_begin++ )
    //{
    //    put( vertex_distance, G, *v_begin, dur_gen() );
    //}

    //DepEdgeIter dei, dei_end;
    //for ( tie( dei, dei_end ) = edges( G ); dei != dei_end; dei++ )
    //{
    //    put( edge_weight, G, *dei, dur_gen() );
    //}


