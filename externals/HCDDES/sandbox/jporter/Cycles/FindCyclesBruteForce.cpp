#include "FindCyclesBruteForce.h"

using namespace std;
using namespace boost;

DepPath::DepPath( DepEdge e, DepGraph & g )
	: _g( g ), _length( 1 ), _from_tail( false ),
	_starting_edge_index( 0 )
{
	_head = source( e, _g );
	_tail = target( e, _g );
}

DepPath::DepPath( const DepPath & dp )
	: _g( dp._g ), _length( dp._length ), _edges( dp._edges ),
		_head( dp._head ), _tail( dp._tail ), 
		_starting_edge_index( dp._starting_edge_index )
{

}

void DepPath::AddEdge( DepEdge e )
{
	if ( !_from_tail )
	{

	}
}

std::set< DepEdge > DepPath::getAdjacentEdges()
{
	set< DepEdge > edges;

	if ( !_from_tail )
	{
		// collect edges adjacent to tail which are higher in order than the starting edge
		DepEdgeIter ei, ei_end;
		for ( tie(ei, ei_end) = out_edges( _head, _g ); ei != ei_end; ei++ )
		{
			if ( *ei > _edges[0] )
			{
				edges.insert( *ei );
			}
		}

		// if none, set _from_tail to true
		if ( edges.size() == 0 )
		{
			_from_tail = true;
		}
	}

	if ( _from_tail )
	{
		// collect edges adjacent to head which are higher in order than the starting edge
		
		// if none, we're done!
	}

	return set< DepEdge>();
}

FindCyclesBruteForce::FindCyclesBruteForce( DepGraph & G ) : _depgr( G )
{
	

}

void FindCyclesBruteForce::calculatePathsAndCycles( unsigned long long max_length )
{
	unsigned long long current_max_length = min( max_length, _max_length );
	unsigned long long current_length;

	// All paths of length 1
	DepEdgeIter ei, ei_end;
	for ( tie(ei, ei_end) = edges( _depgr ); ei != ei_end; ++ei )
	{
		DepPath dp( *ei, _depgr );  
		_open_paths.insert( make_pair( dp.getLength(), dp ) );
		_active_paths.push_back( dp );
	}

	// Go get paths
	for ( current_length = 2; current_length < current_max_length; current_length++ )
	{
		vector< DepPath >::iterator pathIter;
		for ( pathIter = _active_paths.begin(); pathIter != _active_paths.end(); pathIter++ )
		{
			// For each edge, get the set of adjacent edges at the active end (head or tail)
			set< DepEdge > edges = (*pathIter).getAdjacentEdges();
		
			// For each adjacent edge with a higher index, create a new copy of the path and add the edge

			// Check to see whether each edge is a cycle, and put it in the appropriate bucket

		}
		
	}

}

void Print()
{

}


