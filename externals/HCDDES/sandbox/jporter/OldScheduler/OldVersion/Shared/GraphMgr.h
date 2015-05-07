#ifndef __GRAPHMGR_H_INCL__
#define __GRAPHMGR_H_INCL__

#include <utility>
#include <algorithm>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

class SchedulableResource
{
public:
	SchedulableResource( std::string & name, long length ) :
		_name(name), _length(length) { }

	SchedulableResource() {}
	SchedulableResource( const SchedulableResource & sr ) :
		_name(sr._name), _length(sr._length) { }

	SchedulableResource & operator= ( const SchedulableResource & sr )
	{
		_name = sr._name;
		_length = sr._length;
		return *this;
	}

	// These should be hidden, but we're using them as glorified
	// structs inside the graph vertices.
	std::string _name;
	long		_length;
};

class SchedulableTask : public virtual SchedulableResource
{
public:
	SchedulableTask( std::string & name, long length ) :
		SchedulableResource( name, length ) { }
	SchedulableTask() { }
	SchedulableTask( const SchedulableTask & st ) :
		SchedulableResource( st ) { }

	SchedulableTask & operator= ( const SchedulableTask & st )
	{
		SchedulableResource::operator=(st);
	}
};

class SchedulableMsg : public virtual SchedulableResource
{
public:
	SchedulableMsg( std::string & name, long length ) :
		SchedulableResource( name, length ) { }

	SchedulableMsg() { }
	SchedulableMsg( const SchedulableMsg & sm ) :
		SchedulableResource( sm ) { }

	SchedulableMsg & operator= (const SchedulableMsg & sm )
	{
		SchedulableResource::operator=(sm);
	}
};

typedef boost::adjacency_list< boost::vecS, boost::vecS, 
							   boost::directedS, 
							   SchedulableResource> TaskGraph;

typedef std::map< std::string, TaskGraph::vertex_descriptor > VertexMap;

class GraphMgr
{
public:
	GraphMgr();

	void AddTask( std::string & name, long length );
	void AddMsg( std::string & name, long length );
	void AddSender( std::string & sndr, std::string & msg );
	void AddReceiver( std::string & msg, std::string & rcvr );
	
private:
	TaskGraph _tg; 
	VertexMap _tasks;
	VertexMap _msgs;
};

#endif // __GRAPHMGR_H_INCL__

