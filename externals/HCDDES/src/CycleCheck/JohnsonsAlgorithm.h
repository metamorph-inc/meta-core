#ifndef JohnsonsAlgorithm_h__
#define JohnsonsAlgorithm_h__

#include "CycleGraphs.h"

#include "boost/graph/named_function_params.hpp"

#include <vector>
#include <fstream>

class JohnsonsAlgorithm
{
public:

	virtual ~JohnsonsAlgorithm() { }

	void Unblock( CGVertex v )
	{
		_blocked[ boost::get( boost::vertex_index, _a_sub, v) ] = false;
		CGAdjIter cvi, cvi_end;
		std::vector< CGVertex > adjvec;
		for ( boost::tie( cvi, cvi_end) = boost::adjacent_vertices( v, *_b ); cvi != cvi_end; cvi++ )
		{
			adjvec.push_back( *cvi );
		}

		for ( std::vector< CGVertex >::iterator vIter = adjvec.begin(); vIter != adjvec.end(); vIter++ )
		{
			//boost::remove_edge( v, *cvi, *_b );
			boost::remove_edge( v, *vIter, *_b );  
			//if ( _blocked[ boost::get( boost::vertex_index, _a_sub, *cvi) ] )
			if ( _blocked[ boost::get( boost::vertex_index, _a_sub, *vIter) ] )
			{
				Unblock( *vIter );
			}
		}
	}

	bool Circuit( CGVertex v, CGVertex _cur_vertex )
	{
		bool flag = false;

		_vertex_stack.push_back( get( boost::vertex_index, _a_sub, v ) );
		_blocked[ get( boost::vertex_index, _a_sub, v ) ] = true;
		CGAdjIter cvi, cvi_end;
		for( boost::tie( cvi, cvi_end ) = boost::adjacent_vertices( v, _a_sub ); cvi != cvi_end; cvi++ )
		{
			if ( boost::get( boost::vertex_index, _a_sub, *cvi ) == boost::get( boost::vertex_index, _a_sub, _cur_vertex ) )
			{
				idxvector_t cycle;
				for( idxvector_t::iterator vIter = _vertex_stack.begin(); vIter != _vertex_stack.end(); vIter++ )
				{
					cycle.push_back( _a_sub.local_to_global( *vIter ) );
				}
				cycle.push_back( _a_sub.local_to_global(_cur_vertex) );
				_cycles.push_back( cycle );
				flag = true;
			}
			else if ( !_blocked[ boost::get( boost::vertex_index, _a_sub, *cvi) ] )
			{
				if ( Circuit( *cvi, _cur_vertex ) )
				{
					flag = true;
				}
			}
		}

		if ( flag )
		{
			Unblock( v );
		}
		else
		{
			for ( boost::tie( cvi, cvi_end ) = boost::adjacent_vertices( v, _a_sub ); cvi != cvi_end; cvi++ )
			{
				boost::add_edge( boost::get( boost::vertex_index, _a_sub, *cvi), boost::get( boost::vertex_index, _a_sub, v ), *_b );
			}
		}

		if ( _vertex_stack.back() != boost::get( boost::vertex_index, _a_sub, v ) )
		{
			std::cout << "Popped vertex is not the one we're looking for!" << std::endl;
		}
		_vertex_stack.pop_back();

		return flag;
	}

	JohnsonsAlgorithm( CompGraph & cg ) : _a( cg ), _a_sub( _a.create_subgraph() ), _b( NULL )
	{
		_blocked.reserve( boost::num_vertices( _a ) );

		std::vector<idx_t> component( boost::num_vertices( _a ) ), discover_time( boost::num_vertices( _a ) );
		std::vector<boost::default_color_type> color( boost::num_vertices( _a ) );
		std::vector< CGVertex > root( boost::num_vertices( _a ) );
		idx_t num_comps = boost::strong_components( _a, &component[0], 
			boost::root_map( &root[0] ).color_map( &color[0] ).discover_time_map( &discover_time[0] ) );

		std::map< idx_t, std::set< idx_t > > comp_vertex_map;

		for ( idx_t idx = 0; idx < boost::num_vertices( _a ); idx++ )
		{
			(comp_vertex_map[ component[idx] ]).insert( idx );
		}

		// We won't worry about the strong components, we'll just iterate 
		//   over all of the vertices and let the adjacency structure take care of that
		for ( idx_t comp_idx = 0; comp_idx < num_comps; comp_idx++ )
		{
			while( comp_vertex_map[comp_idx].size() > 1 )
			{
				_a_sub = _a.create_subgraph( comp_vertex_map[comp_idx].begin(), comp_vertex_map[comp_idx].end() );

				_blocked.clear();
				_blocked.insert( _blocked.begin(), num_vertices( _a_sub ), false );

				_b = new CompGraph( num_vertices( _a_sub ) ); // effectively clear the b-graph for each iteration

				_vertex_stack.clear();
				Circuit( _a_sub.global_to_local(*comp_vertex_map[comp_idx].begin()), 
							_a_sub.global_to_local(*comp_vertex_map[comp_idx].begin()) );

				delete _b;

				comp_vertex_map[comp_idx].erase( comp_vertex_map[comp_idx].begin() );
			}
		}
	}

	void print() {

		std::ofstream out( "cycles.log", ios::app );
		for( std::vector< idxvector_t >::iterator cIter = _cycles.begin(); cIter != _cycles.end(); cIter++ )
		{
			for( idxvector_t::iterator vertIter = (*cIter).begin(); vertIter != (*cIter).end(); vertIter++ )
			{
				out << *vertIter << " ";
			}
			out << std::endl;
		}
	}

	cyclevector_t getCycles() { return _cycles; }
	std::size_t getNumCycles() { return _cycles.size(); }

protected:
	CompGraph & _a; // original graph (don't change it)
	CompGraph & _a_sub;
	CompGraph * _b;		  // keep track of things
	std::vector< bool > _blocked;  // also keep track of things
	idxvector_t		_vertex_stack;

	cyclevector_t _cycles;
};


#endif // JohnsonsAlgorithm_h__
