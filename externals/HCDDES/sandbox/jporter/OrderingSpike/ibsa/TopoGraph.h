#ifndef _TOPOGRAPH_H_
#define _TOPOGRAPH_H_

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
//
// Datatype definitions for topology graphs
//

#include "boost/graph/visitors.hpp"
#include "boost/graph/graphviz.hpp"
#include "boost/graph/graphml.hpp"

#include "boost/random/mersenne_twister.hpp"
#include "boost/random/variate_generator.hpp"
#include "boost/random/uniform_smallint.hpp"
#include "boost/random/binomial_distribution.hpp"

#include "IBSATypes.h"
#include "DepGraph.h"

typedef enum { Proc, Bus, TGV_unknown } TGVertexType;

struct TGVertexProps {
    
	double		    relspeed;
	double		    score;
    idx_t           hops;
	TGVertexType	vtype;
};

struct TGEdgeProps {

	idx_t		something;
};

typedef boost::adjacency_list< boost::vecS, 
                               boost::vecS, 
                               boost::undirectedS,
                        boost::property< boost::vertex_color_t, idx_t, 
                            boost::property< boost::vertex_distance_t, idx_t, 
                                TGVertexProps > >,
                        boost::property< boost::edge_index_t, idx_t, 
                            boost::property< boost::edge_weight_t, idx_t, 
                                TGEdgeProps > > > TopoGraph;
//typedef boost::subgraph< TopoGraph > TopoGraphSub;
typedef boost::graph_traits< TopoGraph >::edge_descriptor TGEdge;
typedef boost::graph_traits< TopoGraph >::vertex_descriptor TGVertex;
typedef boost::graph_traits< TopoGraph >::vertex_iterator TGVertexIter;
typedef boost::graph_traits< TopoGraph >::edge_iterator TGEdgeIter;
typedef boost::graph_traits< TopoGraph >::out_edge_iterator TGOutEdgeIter;
typedef boost::graph_traits< TopoGraph >::adjacency_iterator TGAdjacencyIter;


#endif // _TOPOGRAPH_H_
