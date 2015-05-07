//
// Datatype definitions for dependency graphs
//

#ifndef _DEPGRAPH_H_
#define _DEPGRAPH_H_

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

#include <iostream>
#include <set>
#include <string>
#include <sstream>

#include "IBSATypes.h"

typedef enum { CP, IB, NA, DGV_unknown } DGVertexType;

struct DGVertexProps {

	idx_t	cost;
	idx_t	blevel;
	idx_t	tlevel;
	DGVertexType  vtype;
	idx_t	bound_to_proc;	
};

struct DGEdgeProps {

	idx_t	cost;
};

typedef boost::adjacency_list< boost::vecS, 
                               boost::vecS, 
                               boost::bidirectionalS,
                        boost::property< boost::vertex_color_t, idx_t, 
                            boost::property< boost::vertex_distance_t, idx_t, 
                                DGVertexProps > >,
                        boost::property< boost::edge_index_t, idx_t, 
                            boost::property< boost::edge_weight_t, idx_t, 
                                DGEdgeProps > > > DepGraph;
//typedef boost::subgraph< DepGraph > DepGraphSub;
typedef boost::graph_traits< DepGraph >::edge_descriptor DGEdge;
typedef boost::graph_traits< DepGraph >::vertex_descriptor DGVertex;
typedef boost::graph_traits< DepGraph >::vertex_iterator DGVertexIter;
typedef boost::graph_traits< DepGraph >::adjacency_iterator DGAdjIter;
typedef boost::graph_traits< DepGraph >::edge_iterator DGEdgeIter;
typedef boost::graph_traits< DepGraph >::in_edge_iterator DGInEdgeIter;

typedef boost::adjacency_list< boost::vecS, 
                               boost::vecS, 
                               boost::undirectedS,
                    boost::property< boost::vertex_color_t, idx_t >,
                    boost::property< boost::edge_index_t, idx_t > > UnDepGraph;


// b-level sorted vertex set
class DGVertexBlevelSorter {
public:

    bool operator()( const DGVertex & v1, const DGVertex & v2 ) {

        if ( (*_g)[v1].vtype == (*_g)[v1].vtype )
            return (*_g)[v1].blevel < (*_g)[v2].blevel;

        else
            return (*_g)[v1].vtype < (*_g)[v1].vtype;
    }

    static void Setup( DepGraph & g ) { _g = &g; }

    //virtual ~DGVertexBlevelSorter() { std::cout << "Cleanup" << std::endl; }

private:
    static DepGraph * _g;
};

typedef std::set< idx_t, DGVertexBlevelSorter > DGVertexSet_SortedByBlevel;
typedef std::multiset< idx_t, DGVertexBlevelSorter > DGVertexMultiset_SortedByBlevel;

// Visitor helper to detect back edges in a DFS traversal of DepGraph
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


// Shamelessly stolen from G. Hemingway - thanks, Graham!
class DepLabelWriter {
private:
	DepGraph _graph;
public:
	DepLabelWriter( DepGraph & g ) : _graph(g) { }

    void operator()( std::ostream & out, const DGEdge & e ) const {

        out << "[label=\"" << _graph[e].cost << "\"]";
    }

	void operator()( std::ostream & out, const DGVertex & v ) const {

		out << "[label=\"" << _graph[v].cost << "\"]";
	}
};

class DGUtils
{
public:

    static IndexVector findCriticalPath( DepGraph & g );

    static void print( IndexVector vec, DepGraph & g )
    {
        for ( IndexVector::iterator vecIter = vec.begin(); 
              vecIter != vec.end(); vecIter++ )
        {
            std::cout << "    " << *vecIter << " cost: " << g[*vecIter].cost << " blevel: " << g[*vecIter].blevel;
            DGVertexType dgvt = g[*vecIter].vtype;
            std::cout << " vtype: " << ((dgvt == CP) ? "CP" : ((dgvt == IB) ? "IB" : ((dgvt == NA) ? "NA" : "Unknown")));
            idx_t p = g[*vecIter].bound_to_proc;
            std::ostringstream out;
            out << p;
            std::cout << " bound: " << ((p < 50000000) ? out.str() : std::string("No")) << std::endl;
        }
    }

    // Clear all of the vertex distances
    static void clearDistances( DepGraph & g )
    {
    
        DGVertexIter vi, vi_end;
        for ( boost::tie(vi, vi_end) = boost::vertices(g);
              vi != vi_end; vi++ )
        {
            boost::put( boost::vertex_distance, g, *vi, 0 );
        }
    }

};

#endif  // _DEPGRAPH_H_
