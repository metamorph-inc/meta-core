#ifndef _DEP_GRAPH_H_
#define _DEP_GRAPH_H_

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/subgraph.hpp>

#include <map>

#include "ESMoL.h"
#include "Semantics.h"

#include "IndexMerge.h"

typedef struct
{
	ESMoL::ComponentRef cref;

} CompRefVertexStruct;

typedef boost::property< boost::vertex_index_t, unsigned long long, CompRefVertexStruct > vert_prop_t;

typedef struct
{
	ID_TYPE src_mref_id;
	ID_TYPE dst_mref_id;

} MsgEdgeStruct;

typedef boost::property< boost::edge_index_t, unsigned long long, MsgEdgeStruct > edge_prop_t;

typedef boost::subgraph< boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, vert_prop_t, edge_prop_t > > DepGraph;
typedef boost::graph_traits<DepGraph>::vertex_descriptor DepVertex;
typedef boost::graph_traits<DepGraph>::vertex_iterator VertexIterator;
typedef boost::graph_traits<DepGraph>::edge_descriptor DepEdge;
typedef boost::graph_traits<DepGraph>::edge_iterator EdgeIterator;

class DependencyKeeper 
{
public:
	static DependencyKeeper *Inst();
	virtual ~DependencyKeeper() { }

	void AddDependency( const ESMoL::ComponentRef & srcdep,
						const ESMoL::MessageRef & srcmsgref,
					    const ESMoL::ComponentRef & dstdep, 
						const ESMoL::MessageRef & dstmsgref );
						
	void CloseGraph();
	bool CheckDependency( const ESMoL::ComponentRef & srcdep, 
						  const ESMoL::MessageRef & srcmsgref,
					      const ESMoL::ComponentRef & dstdep, 
						  const ESMoL::MessageRef & dstmsgref );

	bool SelectSubgraph( const std::string & nodename );
	bool GetNextDependency( ESMoL::ComponentRef & srcdep, 
						    ID_TYPE & src_mref_id,
					        ESMoL::ComponentRef & dstdep, 
						    ID_TYPE & dst_mref_id );

	void Dump();

protected:
	DependencyKeeper(void)				{ }
	static DependencyKeeper				*_inst;

	
	typedef std::map< std::string, DepVertex > VertexMapType;
	VertexMapType _crefmap;

	DepGraph _deps;
	DepGraph * _cur_subgraph;
	EdgeIterator _cur_edge, _end_edge;

	typedef std::map< std::string, DepGraph * > NodeDepMap;
	NodeDepMap _node_dep_graphs; // node name -> subgraph
};

#endif // _DEP_GRAPH_H_