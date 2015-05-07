#include "DepGraph.h"

using namespace std;
using namespace boost;


DepGraph * DGVertexBlevelSorter::_g;

// TODO: Bad hacks -- this needs to be generalized to 
// properly identify CP, IB, and NA vertices
IndexVector DGUtils::findCriticalPath( DepGraph & g ) {

    // assume the first and last vertex are the ends

    // Clear all of the edge weights
    DGEdgeIter ei, ei_end;
    for ( boost::tie(ei, ei_end) = boost::edges(g);
          ei != ei_end; ei++ )
    {
        boost::put( edge_weight, g, *ei, 0 );
    }

    // Clear all of the vertex blevels and distances
    DGUtils::clearDistances( g );
    DGVertexIter vi, vi_end;
    for ( boost::tie(vi, vi_end) = boost::vertices(g);
          vi != vi_end; vi++ )
    {
        g[*vi].blevel = 0;
        g[*vi].vtype = IB;
    }

    // start from the back, traverse backwards, and
    // calculate the b-levels
    IndexSet current, next;
    g[boost::vertex(0,g)].blevel = g[boost::vertex(0,g)].cost;
    current.insert( boost::vertex(boost::num_vertices(g)-1, g) );

    // Until we have marked all of the outbound edges from entry
    while ( boost::get( vertex_distance, g, 0 ) != boost::out_degree(0,g) )
    {
        next.clear();

        // Loop over all the current vertices
        for ( IndexSet::iterator idx = current.begin(); 
              idx != current.end(); idx++ )
        {
            // Propagate back, calculating costs for each 
            // vertex reached
            DGInEdgeIter iei, iei_end;
            for ( boost::tie(iei, iei_end) = boost::in_edges(*idx,g);
                  iei != iei_end; iei++ )
            {
                DGVertex src = boost::source(*iei,g);

                // For each vertex, accumulate the maximum cost
                //  from each in_edge, and mark each in_edge
                idx_t cost = g[*idx].blevel + g[*iei].cost + g[src].cost;
                if ( g[src].blevel < cost )
                {
                    g[src].blevel = cost;
                }

                // For all of the vertices collected, only 
                // propagate back for those where we have already
                // marked all of the out_edges
                if ( boost::get( edge_weight, g, *iei ) == 0 )
                {
                    boost::put( edge_weight, g, *iei, 1 ); // mark edge
                    idx_t marked = boost::get( vertex_distance, g, src );
                    boost::put( vertex_distance, g, src, marked+1 );
                }

                if ( boost::get( vertex_distance, g, src ) == boost::out_degree( src, g ) )
                {
                    next.insert( src );
                }
            }
        }

        current = next;
    } 


    // Now go forward through the graph, following
    // and recording the path which has the maximum blevel
    DGVertex cur = boost::vertex(0,g);
    IndexVector cp;
    cp.push_back( cur );
    while (cur != boost::vertex( boost::num_vertices(g)-1, g ) )
    {
        g[cur].vtype = CP;

        idx_t max_blevel = 0;
        DGVertex max_vertex;
        DGAdjIter ai, ai_end;
        for ( boost::tie( ai, ai_end ) = boost::adjacent_vertices(cur, g);
              ai != ai_end; ai++ )
        {
            idx_t cur_blevel = g[*ai].blevel;
            if ( cur_blevel >= max_blevel )
            {
                max_blevel = cur_blevel;
                max_vertex = *ai;
            }
        }

        cur = max_vertex;
        cp.push_back( cur );
    }
    g[cur].vtype = CP;

    return cp;
}
