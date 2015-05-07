#ifndef ExtractSubgraph_h__
#define ExtractSubgraph_h__

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

typedef boost::adjacency_list< boost::vecS, boost::vecS, boost::directedS, boost::property<boost::vertex_color_t, int>, 
boost::property< boost::edge_index_t, int> > DepGraphBase;
typedef boost::subgraph< DepGraphBase > DepGraph;
typedef boost::graph_traits<DepGraphBase>::edge_descriptor DepEdge;
typedef boost::graph_traits<DepGraphBase>::edge_iterator DepEdgeIter;
typedef boost::graph_traits<DepGraphBase>::vertex_descriptor DepVertex;
typedef boost::graph_traits<DepGraphBase>::vertex_iterator DepVertexIter;
typedef boost::graph_traits<DepGraphBase>::adjacency_iterator DepAdjIter;

class ExtractSubgraph {
protected:
	DepGraph & _depgr;
	DepGraph & _depsubgr;
	typedef std::vector< DepVertex > DepPath;
	typedef std::set<DepVertex> DepCollection;
	typedef std::map<DepVertex, DepVertex > DepSubMap;
	DepCollection * _depcollector;
	DepSubMap _depSubMap;


public:

	ExtractSubgraph(DepGraph & G, const DepVertex & firstvert, const DepVertex & lastvert);
	void findPath( DepVertex current, DepVertex target, DepPath & path);
};

	/* Working graph example code */
    //DepGraph G(6);
    //add_edge(0,1,G);
    //add_edge(2,4,G);
    //add_edge(2,5,G);
    //add_edge(0,3,G);
    //add_edge(1,4,G);
    //add_edge(4,3,G);

//ExtractSubgraph es(G,0,3);

  /* Topological sort example */
  /*
  typedef std::vector< Vertex > container;
  container c;
  topological_sort(G, std::back_inserter(c));

  cout << "A topological ordering: ";
  for ( container::reverse_iterator ii=c.rbegin(); ii!=c.rend(); ++ii)
    cout << *ii << " ";
  cout << endl; */


#endif // ExtractSubgraph_h__