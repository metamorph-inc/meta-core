#include "ExtractSubgraph.h"

ExtractSubgraph::ExtractSubgraph(DepGraph & G, const DepVertex & firstvert, const DepVertex & lastvert) :
_depgr(G), _depsubgr( _depgr.create_subgraph() ) {

	//_depsubgr = _depgr.create_subgraph();
	_depcollector = new DepCollection();

	DepPath p;
	std::cout << "Finding path from " << firstvert << " to " << lastvert << std::endl;
	findPath( firstvert, lastvert, p);

	DepCollection::iterator pVertex;
	for ( pVertex = _depcollector->begin(); pVertex != _depcollector->end(); pVertex ++ ) {
		DepVertex new_vert = add_vertex( *pVertex, _depsubgr );
		_depSubMap.insert( std::make_pair( new_vert, *pVertex ) );
	}

	delete _depcollector;

	//print_graph( _depsubgr, get(vertex_index, _depgr) );
	DepVertexIter dvi, dvi_end;
	for ( tie( dvi, dvi_end) = vertices(_depsubgr); dvi != dvi_end; dvi++ )
	{
		std::cout << "vertex " << *dvi << std::endl;
	}

	DepEdgeIter dei, dei_end;
	for ( tie( dei, dei_end ) = edges(_depsubgr); dei != dei_end; dei++ )
	{
		std::cout << "s: source " << source(*dei, _depsubgr) << " target " << target(*dei, _depsubgr) << std::endl;
		std::cout << "g: source " << _depSubMap[source(*dei, _depgr)]
		<< " target " << _depSubMap[target(*dei, _depgr)] << std::endl;
	}
}

// Technique from http://forums.sun.com/thread.jspa?threadID=623461&start=15&tstart=0
void ExtractSubgraph::findPath( DepVertex current, DepVertex target, DepPath & path) {

	std::cout << "cur " << current << " target " << target << std::endl;
	if( current == target ) {
		path.push_back(current);
		// Add all of the vertices in the path to the subgraph
		DepPath::iterator pIter;
		std::cout << "Adding path: ";
		for ( pIter = path.begin(); pIter != path.end(); pIter++ )
		{
			std::cout << *pIter << " ";
			_depcollector->insert( *pIter );
			//add_vertex( *pIter, _depsubgr );
		}
		std::cout << std::endl;
		path.pop_back();
		return;
	}

	// We already have a DAG
	//if(path.contains(current)) {
	//    return;
	//}

	path.push_back(current);

	DepAdjIter dai, dai_end;
	for( tie(dai, dai_end) = adjacent_vertices(current, _depgr); dai != dai_end; dai++ ) {

		findPath(*dai,target,path);
	}
	path.pop_back();
}