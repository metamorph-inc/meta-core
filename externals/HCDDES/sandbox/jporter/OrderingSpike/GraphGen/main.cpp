#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <iterator>
#include <vector>
#include <set>
#include <map>
#include <algorithm>

#include "boost/pending/property.hpp"
#include "boost/graph/properties.hpp"
#include "boost/property_map/property_map.hpp"
#include "boost/graph/subgraph.hpp"
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/graph_traits.hpp"
#include "boost/graph/graph_utility.hpp"
#include "boost/graph/graph_concepts.hpp"
#include "boost/graph/depth_first_search.hpp"
#include "boost/graph/topological_sort.hpp"
#include "boost/graph/strong_components.hpp"
#include "boost/graph/transitive_closure.hpp"
#include "boost/graph/exception.hpp"
#include "boost/graph/visitors.hpp"
#include "boost/graph/graphviz.hpp"
#include "boost/graph/graphml.hpp"

#include "boost/random/mersenne_twister.hpp"
#include "boost/random/variate_generator.hpp"
#include "boost/random/uniform_smallint.hpp"
#include "boost/random/binomial_distribution.hpp"

// Define Graph Structures

typedef unsigned long long idx_t;

typedef boost::adjacency_list< boost::vecS, boost::vecS, boost::bidirectionalS,
    boost::property< boost::vertex_color_t, idx_t, boost::property< boost::vertex_distance_t, idx_t > >,
    boost::property< boost::edge_index_t, idx_t, boost::property< boost::edge_weight_t, idx_t > > > DepGraph;
//typedef boost::subgraph< DepGraph > DepGraphSub;
typedef boost::graph_traits< DepGraph >::edge_descriptor DepEdge;
typedef boost::graph_traits< DepGraph >::vertex_descriptor DepVertex;
typedef boost::graph_traits< DepGraph >::vertex_iterator DepVertexIter;
typedef boost::graph_traits< DepGraph >::edge_iterator DepEdgeIter;

typedef boost::adjacency_list< boost::vecS, boost::vecS, boost::undirectedS,
    boost::property< boost::vertex_color_t, idx_t >,
    boost::property< boost::edge_index_t, idx_t > > UnDepGraph;


// Utility Function to Detect Cycle Formation

struct cycle_detector : public boost::dfs_visitor<>
{
    cycle_detector( bool& has_cycle)
        : _has_cycle(has_cycle) { }

    template <class Edge, class Graph>
        void back_edge(Edge, Graph&) {
            _has_cycle = true;
            //std::cout << "Detected cycle..." << std::endl;
        }
protected:
    bool & _has_cycle;
};


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

    if ( N < 3 )
    {
        cout << " N must be larger than 2. " << endl;
        exit( 1 );
    }

    // The algorithm seems to arrive faster if we prime the pump with a
    // connected graph.  We connect the elements 1..N-2 to both the entry
    // and exit vertex to start with a maximally parallel graph.

    DepGraph G(N); // start the graph with a random acyclic path
    UnDepGraph UG(N);  // TODO: Remove this duplicate graph.  Use one undirected graph with an edge property specifying its direction

    vector< idx_t > idxs(N);
    for ( idx_t idx = 0; idx < N; idx++ )
        idxs[idx] = idx;

    for ( idx_t idx = 1; idx < N-1; idx++ )
    {
        add_edge( idxs[0], idxs[idx], G);
        add_edge( idxs[idx], idxs[N-1], G);
        add_edge( idxs[0], idxs[idx], UG);
        add_edge( idxs[idx], idxs[N-1], UG);
    }

    /* Alternative initialization: shuffle the vertex indices and
       create a path through them.  This results in a different graph
       structure. */
    /*
    random_shuffle( idxs.begin(), idxs.end() );

    for ( idx_t idx = 1; idx < N-1; idx++ )
    {
        add_edge( idxs[idx], idxs[idx+1], G );
        add_edge( idxs[idx], idxs[idx+1], UG );
    }
    */


    idx_t factors = 1;
    idx_t iterations = N / 2;

    mt19937 rng;
    rng.seed(time(0));
    uniform_smallint< idx_t > range(0, N-1);
    binomial_distribution< idx_t > durations( 100, 0.05 );
    variate_generator< mt19937 &, uniform_smallint< idx_t > > gen( rng, range );
    variate_generator< mt19937 &, binomial_distribution< idx_t > > dur_gen( rng, durations );

    for ( idx_t count = 0; count < iterations; count++ )
    {
        // Choose (i,j) randomly (i != j)
        idx_t i_idx = gen();
        idx_t j_idx = gen();

        // Check to see whether (i,j) is already in the graph
        DepEdge e1;
        bool found = false;
        tie( e1, found ) = edge( i_idx, j_idx, G );
        if ( found )
        {
            vector< idx_t > compmap(N);

        // if so, remove the edge unless it disconnects the graph
            remove_edge( i_idx, j_idx, G );
            remove_edge( i_idx, j_idx, UG );

            vector< idx_t > compmap2(N);
            unsigned long comps2 = connected_components( UG, &compmap2[0] );
            if ( comps2 > 1 ) // put the reverse edge back in
            {
                //add_edge( j_idx, i_idx, G );
                //add_edge( j_idx, i_idx, UG );
                add_edge( i_idx, j_idx, G );
                add_edge( i_idx, j_idx, UG );
            }
        }
        else
        {
        // if not, add the edge unless it creates a cycle
            add_edge( i_idx, j_idx, G );
            add_edge( i_idx, j_idx, UG );

            bool has_cycle = false;
            cycle_detector vis( has_cycle );
            depth_first_search( G, visitor( vis ) );

            if ( has_cycle )
            {
                remove_edge( i_idx, j_idx, G );
                remove_edge( i_idx, j_idx, UG );
            }
        }
    }

    // To make sure the graph is polar:  wire all of the pure source
    // vertices to another (new) source vertex, and wire all of the pure
    // sink vertices to another (new) sink vertex.
    DepVertexIter v_begin, v_end;
    set< DepVertex > sources, sinks;
    for( tie( v_begin, v_end ) = vertices( G ); v_begin != v_end; v_begin++ )
    {
        idx_t out_degs = out_degree( *v_begin, G );
        idx_t in_degs = in_degree( *v_begin, G );

        if ( in_degs == 0 )
            sources.insert( *v_begin );
        else if ( out_degs == 0 )
            sinks.insert( *v_begin );
    }

    DepVertex source_vertex = add_vertex( G );
    DepVertex sink_vertex = add_vertex( G );

    for ( set< DepVertex >::iterator vert = sources.begin(); vert != sources.end(); vert++ )
    {
        add_edge( source_vertex, *vert, G );
    }

    for ( set< DepVertex>::iterator vert = sinks.begin(); vert != sinks.end(); vert++ )
    {
        add_edge( *vert, sink_vertex, G );
    }

    // add durations
    for( tie( v_begin, v_end ) = vertices( G ); v_begin != v_end; v_begin++ )
    {
        // pick a random duration
        put( vertex_distance, G, *v_begin, dur_gen() );
    }

    DepEdgeIter dei, dei_end;
    for ( tie( dei, dei_end ) = edges( G ); dei != dei_end; dei++ )
    {
        put( edge_weight, G, *dei, dur_gen() );
    }


    dynamic_properties dp;
    dp.property("vertex_distance", get( vertex_distance_t(), G ));
    dp.property("edge_weight", get( edge_weight_t(), G ));
    //write_graphml( cout, G, dp, true );
    write_graphviz( cout, G );

    return 0;
}

