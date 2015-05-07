#ifndef FindCyclesBruteForce_h__
#define FindCyclesBruteForce_h__

#include <iostream>
#include <vector>
#include <set>
#include <map>

//#include "boost/property_map/property_map.hpp"
#include "boost/graph/subgraph.hpp"
#include "boost/graph/adjacency_list.hpp"
//#include "boost/graph/properties.hpp"
#include "boost/graph/graph_traits.hpp"
#include "boost/graph/graph_utility.hpp"
//#include "boost/graph/topological_sort.hpp"

typedef boost::adjacency_list< boost::vecS, boost::vecS, boost::directedS, 
	boost::property<boost::vertex_color_t, int>, 
	boost::property< boost::edge_index_t, int> > DepGraphBase;
typedef boost::subgraph< DepGraphBase > DepGraph;
typedef boost::graph_traits<DepGraphBase>::edge_descriptor DepEdge;
typedef boost::graph_traits<DepGraphBase>::edge_iterator DepEdgeIter;
typedef boost::graph_traits<DepGraphBase>::vertex_descriptor DepVertex;
typedef boost::graph_traits<DepGraphBase>::vertex_iterator DepVertexIter;
typedef boost::graph_traits<DepGraphBase>::adjacency_iterator DepAdjIter;

typedef std::vector< DepVertex > DepVertexPath;
typedef std::vector< DepEdge > DepEdgePath;

class DepPath {

public:
	DepGraph & _g;
	DepEdgePath _edges;
	DepVertex	_head;
	DepVertex	_tail;
	unsigned long _starting_edge_index;

	unsigned long long _length;

	DepPath( DepEdge e, DepGraph & g );
	DepPath( const DepPath & dp );
	void AddEdge( DepEdge e );
	std::set< DepEdge > getAdjacentEdges();

	const DepEdgePath & getEdges() { return _edges; }
	bool isCycle() { return ( _head == _tail ); }
	unsigned long long getLength() { return _length; }

private:
	bool _from_tail;
};

class FindCyclesBruteForce {

protected:

	unsigned long long _max_length;

	DepGraph & _depgr;

	typedef DepPath OpenPath;
	typedef DepPath Cycle;
	std::vector< DepPath > _active_paths;
	std::multimap< unsigned long long, OpenPath > _open_paths;
	std::multimap< unsigned long long, Cycle > _cycles;

public:

	FindCyclesBruteForce( DepGraph & G );
	void calculatePathsAndCycles( unsigned long long max_length = 10000ULL );

	void Print();
};

#endif // FindCyclesBruteForce_h__