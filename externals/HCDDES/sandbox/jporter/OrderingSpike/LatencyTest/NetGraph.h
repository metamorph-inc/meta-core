#ifndef _NET_GRAPH_H
#define _NET_GRAPH_H

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/subgraph.hpp>


#include <iostream>                  // for std::cout
#include <utility>                   // for std::pair
#include <algorithm>                 // for std::for_each
#include <map>
#include <vector>
#include <string>

#include "RandomSelector.h"

class SystemObject
{
public:
	virtual ~SystemObject() { }
	SystemObject( unsigned long idx ) : id( idx ) { }
	unsigned long id;
};

class Task : public SystemObject 
{
public:
	Task( unsigned long idx, unsigned long parent_id ) : SystemObject( idx ), _parent(parent_id) { }
	virtual ~Task() { }

	double _periodSecs;
	double _wcetSecs;
	unsigned long _parent; // Parent node
	std::map< std::string, unsigned long > _msg_lengths; // (bus,target)->length
};

class Node : public SystemObject 
{
public:
	Node( unsigned long idx ) : SystemObject( idx ) { }
	virtual ~Node() { }
	std::vector< Task > _tasks;
};

class Net : public SystemObject 
{
public:
	Net( unsigned long idx ) : SystemObject( idx ) { }
	virtual ~Net() { }
};

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS > NetworkGraph;
typedef boost::graph_traits<NetworkGraph>::vertex_descriptor NetworkVertex;
typedef std::pair< unsigned long, unsigned long > NetworkEdge; // Node->Net

typedef boost::property< boost::vertex_index_t, unsigned long long > TaskVertexProperties;
typedef boost::property< boost::edge_index_t, unsigned long long > TaskEdgeProperties;
typedef boost::property< boost::graph_name_t, std::string > TaskGraphProperties;

typedef boost::subgraph< boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, 
							TaskVertexProperties, TaskEdgeProperties, TaskGraphProperties > > TaskGraph;
typedef boost::graph_traits<TaskGraph>::vertex_descriptor TaskVertex;
//typedef std::pair< unsigned long, unsigned long > Message; // Task->Task

class NetGraph
{
public:
	NetGraph();
	virtual ~NetGraph(void) { }

	bool print();
	bool print_graph();

protected:
	RandomSelector _rnd;

	NetworkGraph _ng;
	TaskGraph * _tg;
	std::vector<TaskGraph> _tg_subs;

	std::vector< SystemObject * > _nets_nodes;
	std::vector< Task > _tasks;
};

#endif // _NET_GRAPH_H