#include "DepGraph.h"

#include <boost/graph/transitive_closure.hpp>
#include <boost/graph/copy.hpp>

using namespace std;

/*** Static Member Initialization ***/
DependencyKeeper * DependencyKeeper::_inst = NULL;

// Private constructor for singleton pattern
DependencyKeeper * DependencyKeeper::Inst() {
	if ( DependencyKeeper::_inst == NULL ) {
		// We don't catch the exception, since we're dead if this fails.
		DependencyKeeper::_inst = new DependencyKeeper();
	}
	return DependencyKeeper::_inst;
}

// Methods
void DependencyKeeper::AddDependency( const ESMoL::ComponentRef & srcdep, 
									  const ESMoL::MessageRef & srcmsgref, 
									  const ESMoL::ComponentRef & dstdep, 
									  const ESMoL::MessageRef & dstmsgref )
{
	// Look up attached nodes
	set< ESMoL::ComponentAssignment > srcCAs = srcdep.dstComponentAssignment();
	set< ESMoL::ComponentAssignment > dstCAs = dstdep.dstComponentAssignment();

	ESMoL::ComponentAssignment srcCA = *srcCAs.begin();
	ESMoL::ComponentAssignment dstCA = *dstCAs.begin();

	ESMoL::NodeRef srcNR = srcCA.dstComponentAssignment_end();
	ESMoL::NodeRef dstNR = dstCA.dstComponentAssignment_end();

	ESMoL::Node srcNode = srcNR.ref();
	ESMoL::Node dstNode = dstNR.ref();

	VertexMapType::iterator srcIter = _crefmap.find( srcdep.name() );
	VertexMapType::iterator dstIter = _crefmap.find( dstdep.name() );

	NodeDepMap::iterator srcNodeIter = _node_dep_graphs.find( string( srcNode.name() ) );
	NodeDepMap::iterator dstNodeIter = _node_dep_graphs.find( string( dstNode.name() ) );

	string srcName = string( srcNode.name() );
	string dstName = string( dstNode.name() );

	if ( srcNodeIter == _node_dep_graphs.end() )
	{
		_node_dep_graphs[ srcName ] = &(_deps.create_subgraph());
	}

	if ( dstNodeIter == _node_dep_graphs.end() )
	{
		_node_dep_graphs[ dstName ] = &(_deps.create_subgraph());
	}

	DepVertex srcdv;
	DepVertex dstdv;

	if ( srcIter == _crefmap.end() )
	{
		srcdv = add_vertex( _deps );
		add_vertex( srcdv, *_node_dep_graphs[ srcName ] );
		_deps[srcdv].cref = srcdep;
		_crefmap[ srcdep.name() ] = srcdv;
	}
	else
	{
		srcdv = srcIter->second;
	}

	if ( dstIter == _crefmap.end() )
	{
		dstdv = add_vertex( _deps );
		add_vertex( dstdv, *_node_dep_graphs[ dstName ] );
		_deps[dstdv].cref = dstdep;
		_crefmap[ dstdep.name() ] = dstdv;
	}
	else
	{
		dstdv = dstIter->second;
	}

	DepEdge e;
	bool inserted;

	// adding an edge to the root graph induces edges in the subgraphs
	boost::tuples::tie( e, inserted ) = add_edge( srcdv, dstdv, _deps );

	if ( inserted )
	{
		_deps[ e ].src_mref_id = srcmsgref.uniqueId();
		_deps[ e ].dst_mref_id = dstmsgref.uniqueId();
	}
}

// Calculate the transitive closure (when we're finished adding things)
void DependencyKeeper::CloseGraph()
{
	boost::adjacency_list <> new_deps;
	transitive_closure( _deps, new_deps );
	
	auto new_deps_edges = edges(new_deps);
	auto cur_e = new_deps_edges.first;
	auto end_e = new_deps_edges.second;

	// Warning: this is a little bit of a hack -- we're explicitly relying on the fact that
	// vertices in the original graph and in the transitive closure graph are indexed in the same
	// way -- that might not be true.  If making a change that requires an explicit vertex map to
	// be passed to the transitive closure operation, this next section will break.
	// We're also relying on the fact that we have specified integer vertex and edge indexing to
	// make the performance of this next section reasonable.  Otherwise edge lookups could be slow.
	DepEdge tgt_edge;
	bool foundit;
	for ( ; cur_e != end_e; cur_e++ )
	{
		boost::tuples::tie( tgt_edge, foundit ) = edge( source( *cur_e, new_deps ), target( *cur_e, new_deps ), _deps );
		if ( !foundit )
		{
			add_edge( source( *cur_e, new_deps ), target( *cur_e, new_deps ), _deps );
		}
	}
}

bool DependencyKeeper::CheckDependency( const ESMoL::ComponentRef & srcdep, 
									    const ESMoL::MessageRef & srcmsgref, 
									    const ESMoL::ComponentRef & dstdep, 
										const ESMoL::MessageRef & dstmsgref )
{
	VertexMapType::iterator srcIter = _crefmap.find( srcdep.name() );
	VertexMapType::iterator dstIter = _crefmap.find( dstdep.name() );

	if ( srcIter == _crefmap.end() || dstIter == _crefmap.end() )
	{
		return false;
	}

	DepVertex srcdv = srcIter->second;
	DepVertex dstdv = dstIter->second;

	DepEdge e;
	bool found;
	boost::tuples::tie(e, found) = edge( srcdv, dstdv, _deps );

	return found;
}

// set the internal iterator to the start of the current algorithm
bool DependencyKeeper::SelectSubgraph( const std::string & nodename )
{
	NodeDepMap::iterator nodeDepIter = _node_dep_graphs.find( nodename );
	if ( nodeDepIter == _node_dep_graphs.end() )
		return false;  // nodename not found

	_cur_subgraph = nodeDepIter->second;
	boost::tuples::tie( _cur_edge, _end_edge) = edges( *_cur_subgraph );

	//cout << "Selecting subgraph " << nodename << " w/ " << num_edges( *_cur_subgraph ) << " edges ";
	//cout << "and " << num_vertices( *_cur_subgraph ) << " vertices." << endl;
	return true;
}

bool DependencyKeeper::GetNextDependency( ESMoL::ComponentRef & srcdep, 
										  ID_TYPE & src_mref_id, 
									      ESMoL::ComponentRef & dstdep, 
										  ID_TYPE & dst_mref_id )
{
	bool success = false; 
	if ( _cur_edge != _end_edge )
	{
		DepVertex src_vertex = source( *_cur_edge, *_cur_subgraph );
		DepVertex dst_vertex = target( *_cur_edge, *_cur_subgraph );

		srcdep = (*_cur_subgraph)[src_vertex].cref;
		dstdep = (*_cur_subgraph)[dst_vertex].cref;

		src_mref_id = (*_cur_subgraph)[ *_cur_edge ].src_mref_id;
		dst_mref_id = (*_cur_subgraph)[ *_cur_edge ].dst_mref_id;

		_cur_edge++;
		success = true;
	}

	return success;
}

void DependencyKeeper::Dump()
{
	EdgeIterator cur_edge, end_edge;

	cout << "Root graph" << endl;
	boost::tuples::tie( cur_edge, end_edge ) = edges( _deps );
	for ( ; cur_edge != end_edge; cur_edge++ )
	{
		DepVertex src_vertex = source( *cur_edge, _deps );
		DepVertex dst_vertex = target( *cur_edge, _deps );

		ESMoL::ComponentRef src_cref = _deps[src_vertex].cref;
		ESMoL::ComponentRef dst_cref = _deps[dst_vertex].cref;

		string srcName = string( src_cref.name() );
		string dstName = string( dst_cref.name() );

		cout << srcName << " -> " << dstName << endl;
	}

	NodeDepMap::iterator nodeDepIter;
	for ( nodeDepIter = _node_dep_graphs.begin(); nodeDepIter != _node_dep_graphs.end(); nodeDepIter++ )
	{
		cout << "Node " << nodeDepIter->first << ":" << endl;

		SelectSubgraph( string( nodeDepIter->first ) );
		ESMoL::ComponentRef src_cref, dst_cref;
		ID_TYPE src_mref_id, dst_mref_id;
		while ( GetNextDependency( src_cref, src_mref_id, dst_cref, dst_mref_id ) )
		{
			cout << src_cref.name() << " -> " << dst_cref.name() << endl;
		}
	}
}