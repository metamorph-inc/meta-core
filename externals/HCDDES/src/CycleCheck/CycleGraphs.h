#ifndef CycleGraphs_h__
#define CycleGraphs_h__

#include "boost/config.hpp"
#include "boost/graph/copy.hpp"
#include "boost/graph/properties.hpp"
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/graph_traits.hpp"
#include "boost/graph/graph_utility.hpp"
#include "boost/graph/graph_concepts.hpp"
#include "boost/graph/subgraph.hpp"
#include "boost/graph/depth_first_search.hpp"
//#include "boost/graph/topological_sort.hpp"
#include "boost/graph/named_function_params.hpp"
#include "boost/graph/strong_components.hpp"
//#include "boost/graph/transitive_closure.hpp"
#include "boost/graph/exception.hpp"
#include "boost/graph/visitors.hpp"
#include "boost/graph/graphviz.hpp"

#include "UdmBase.h"


typedef unsigned long long idx_t;

typedef boost::adjacency_list< boost::vecS, boost::vecS, boost::bidirectionalS,
boost::property< boost::vertex_color_t, idx_t >,
boost::property< boost::edge_index_t, idx_t > > PortGraph;
typedef boost::graph_traits< PortGraph >::edge_descriptor PGEdge;
typedef boost::graph_traits< PortGraph >::vertex_descriptor PGVertex;
typedef boost::graph_traits< PortGraph >::vertex_iterator PGVertexIter;
typedef boost::graph_traits< PortGraph >::edge_iterator PGEdgeIter;

typedef enum { prim, inport, outport, delay, subsys_inport, subsys_outport, unknown_prim } cg_primitive_type;
struct CGVertexProps {

	std::string			path;
	cg_primitive_type	primtype;
	Udm::Object			udmobj;
	
};

typedef enum { connection, pathedge, unknown_edge } cg_edge_type;
struct CGEdgeProps {

	cg_edge_type		edgetype;
	Udm::Object			udmobj;
};

typedef boost::subgraph< boost::adjacency_list< boost::setS, boost::vecS, boost::bidirectionalS,
												boost::property< boost::vertex_color_t, idx_t, CGVertexProps >,
												boost::property< boost::edge_index_t, idx_t, CGEdgeProps >,
												boost::property< boost::graph_name_t, std::string > > > CompGraph;
//typedef boost::subgraph< CompGraph > CompSubgraph;
typedef boost::graph_traits< CompGraph >::edge_descriptor CGEdge;
typedef boost::graph_traits< CompGraph >::vertex_descriptor CGVertex;
typedef boost::graph_traits< CompGraph >::vertex_iterator CGVertexIter;
typedef boost::graph_traits< CompGraph >::edge_iterator CGEdgeIter;
typedef boost::graph_traits< CompGraph >::in_edge_iterator CGInEdgeIter;
typedef boost::graph_traits< CompGraph >::out_edge_iterator CGOutEdgeIter;
typedef boost::graph_traits< CompGraph >::adjacency_iterator CGAdjIter;

typedef unsigned long long uid_t;
typedef std::map< uid_t, idx_t > portmap_t;
typedef std::map< uid_t, std::set< uid_t > > port_edges_t;
typedef std::map< uid_t, port_edges_t > compmap_t;
typedef std::vector< idx_t > idxvector_t;
typedef std::set< idx_t > idxset_t;
typedef std::vector< idxvector_t > cyclevector_t;
typedef std::map< idx_t, idx_t > idxmap_t;
typedef std::map< idx_t, idxset_t > idxsetmap_t;

class CGLabelWriter {
private:
	CompGraph & _cg;
public:
	CGLabelWriter( CompGraph & g ) : _cg( g ) {}

	void operator()( std::ostream & out, const CGEdge & e ) const {

		out << "[label=\"" 
			<< ( ( _cg[e].edgetype == connection ) ? 
				std::string( "con" ) : 
				( ( _cg[e].edgetype == pathedge ) ?
					std::string( "pte" ) :
					std::string( "unk" ))) 
			<< "\"]";
	}

	void operator()( std::ostream & out, const CGVertex & v ) const {
		std::string p = _cg[v].path;
		while ( p.find("\n") != std::string::npos ) p.erase( p.find("\n") );  // remove newlines
		out << "[label=\"" << p << "\"]";
	}
};

class CGUtils {

public:
	static void CGResetColor( CompGraph & cg ) {

		CGVertexIter vi, vi_end;
		for ( boost::tie( vi, vi_end ) = vertices( cg ); vi != vi_end; vi++ )
		{
			put( boost::vertex_color, cg, *vi, boost::white_color );
		}
	}

};

#endif // CycleGraphs_h__