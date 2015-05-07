#include "CyberCycles.h"

#include <set>
#include <iostream>

using namespace std;

class NLWData {

	public:
		idx_t _nvertices;
		idx_t _nvisited;
		string _mapLabel;
};


class NumLabelWriter {
private:
	CompGraph & _cg;
public:
	NumLabelWriter( CompGraph & g ) : _cg( g ) {  }
	NumLabelWriter( CompGraph & g, const std::map< idx_t, std::string > & id2path, NLWData * nlwd ) : _cg( g ), _nlwd( nlwd ) 
	{
		std::ostringstream oss;
		oss << "< " << endl;
		oss << "<table>" << endl;
		for ( std::map< idx_t, std::string >::const_iterator pathIter = id2path.begin(); pathIter != id2path.end(); pathIter++ )
		{
			oss << "<tr><td>" << pathIter->first << "</td><td>" << pathIter->second << "</td></tr>"  << endl;;
		}

		oss << "</table> >" << endl;

		_nlwd->_nvisited = 0;
		_nlwd->_nvertices = boost::num_vertices( g );
		_nlwd->_mapLabel = oss.str();

	}

	void operator()( std::ostream & out, const CGEdge & e ) const {

		out << "[label=\"" 
			<< "\"]";
	}

	void operator()( std::ostream & out, const CGVertex & v ) const {

		out << "[label=\"" << v << "\"]";

		if ( ++(_nlwd->_nvisited) >= _nlwd->_nvertices )
		{
			out << endl;
			out << (_nlwd->_nvisited + 1) << " [shape = plaintext, label =" << _nlwd->_mapLabel << "]";
		}
	}

private:

	NLWData * _nlwd;
};

// Create the graph
CyberCycles::CyberCycles( const CyberComposition::Simulink::Subsystem & sub ) : _log( "cycle_graph.log" )
{
	_cg = new CompGraph();
	_log << "Building graph for subsystem" << sub.name() << endl;
	BuildCycleGraph( sub );
	ofstream fout( "cycle_graph.dot" );
	NLWData * nlwdata = new NLWData;
	boost::write_graphviz( fout, *_cg, NumLabelWriter(*_cg, _vertexToPathStrMap, nlwdata), NumLabelWriter(*_cg) );
	delete nlwdata;
}

bool CyberCycles::FindCycles()
{
	if ( boost::num_vertices( *_cg ) > 0 )
	{
		JohnsonsAlgorithm ja( *_cg );
		cyclevector_t cycles = ja.getCycles();

		if ( cycles.size() > 0 )
		{
			_cycleList.clear();

			for ( cyclevector_t::iterator cycleIter = cycles.begin(); cycleIter != cycles.end(); cycleIter++ )
			{
				vector< string > cycleName;
				for ( idxvector_t::iterator vertexIter = (*cycleIter).begin(); vertexIter != (*cycleIter).end(); vertexIter++ )
				{
					cycleName.push_back( _vertexToPathStrMap[ *vertexIter ] );
				}
				_cycleList.push_back( cycleName );
			}

			return true;
		}
	}

	return false;
}

// Recursively build cycle graph
void CyberCycles::BuildCycleGraph( const CyberComposition::Simulink::Subsystem & sub )
{
	_curPath.push_back( string( sub.name() ) );
	_log << "Entered subsystem " << _curPath.back() << endl;
	set< CyberComposition::Simulink::Subsystem > subs = sub.Subsystem_kind_children();
	for ( set< CyberComposition::Simulink::Subsystem >::iterator subIter = subs.begin(); subIter != subs.end(); subIter++ )
	{
		BuildCycleGraph( *subIter );
	}

	// Process I/O ports at the current level
	set< CyberComposition::Simulink::SF_Port > ioports = sub.SF_Port_children();
	for ( set< CyberComposition::Simulink::SF_Port >::iterator ioportIter = ioports.begin(); ioportIter != ioports.end(); ioportIter++ )
	{
		// add a new vertex for each, and store it in the map
		CGVertex cgv = add_vertex( *_cg );
		_idToVertexMap[ (*ioportIter).uniqueId() ] = cgv;
		_vertexToPathStrMap[ cgv ] = MakePathName( string( (*ioportIter ).name() ) );

		// TODO: Get this into a file
		_log << "Added vertex " << _idToVertexMap[ (*ioportIter).uniqueId() ] << " for I/O port at path " << _vertexToPathStrMap[ cgv ] << " with id " << (*ioportIter).uniqueId() << endl;
	}

	// Process primitives
	set< CyberComposition::Simulink::Primitive > prims = sub.Primitive_kind_children();
	for ( set< CyberComposition::Simulink::Primitive >::iterator primIter = prims.begin(); primIter != prims.end(); primIter++ )
	{
		CGVertex cgv = add_vertex( *_cg );
		_idToVertexMap[ (*primIter).uniqueId() ] = cgv;
		_vertexToPathStrMap[ cgv ] = MakePathName( string( (*primIter ).name() ) );

		_log << "Added vertex " << _idToVertexMap[ (*primIter).uniqueId() ] << " for Primitive at " << _vertexToPathStrMap[ cgv ] << " with id " << (*primIter).uniqueId() << endl;
	}

	// Process lines
	set< CyberComposition::Simulink::Line > lines = sub.Line_kind_children();
	for ( set< CyberComposition::Simulink::Line >::iterator lineIter = lines.begin(); lineIter != lines.end(); lineIter++ )
	{
		// Each endpoint is either an I/O port or is within a primitive
		// Create edges for each line
		// Look up vertex numbers for the I/O port endpoints
		// Look up vertex number for a primitive parent
		// If the primitive parent is some kind of delay, then skip edge creation (leave those vertices disconnected)
		bool delayFlag = false;

		CyberComposition::Simulink::SF_Port src_end, dst_end;
		src_end = (*lineIter).srcLine_end();
		dst_end = (*lineIter).dstLine_end();

		CGVertex src_vertex, dst_vertex;

		// Check src end
		Udm::Object par = src_end.parent();
		if ( par.type() == CyberComposition::Simulink::Primitive::meta )
		{
			CyberComposition::Simulink::Primitive prim_par = CyberComposition::Simulink::Primitive::Cast( par );
			string prim_par_blocktype = prim_par.BlockType();
			if ( prim_par_blocktype == string( "UnitDelay" ) )
			{
				delayFlag = true;
			}
			else
			{
				src_vertex = _idToVertexMap[ prim_par.uniqueId() ];
			}
		}
		else
		{
			src_vertex = _idToVertexMap[ src_end.uniqueId() ];
		}

		// Check dst end
		par = dst_end.parent();
		if ( par.type() == CyberComposition::Simulink::Primitive::meta )
		{
			CyberComposition::Simulink::Primitive prim_par = CyberComposition::Simulink::Primitive::Cast( par );
			string prim_par_blocktype = prim_par.BlockType();
			if ( prim_par_blocktype == string( "UnitDelay" ) )
			{
				delayFlag = true;
			}
			else
			{
				dst_vertex = _idToVertexMap[ prim_par.uniqueId() ];
			}
		}
		else
		{
			dst_vertex = _idToVertexMap[ dst_end.uniqueId() ];
		}

		if ( !delayFlag )
		{
			boost::add_edge( src_vertex, dst_vertex, *_cg );
			_log << "Added edge (" << _idToVertexMap[ src_end.uniqueId() ] << ", " << _idToVertexMap[ dst_end.uniqueId() ] << ")" << endl;
		}
	}

	_curPath.pop_back();
	_log << "Leaving subsystem " << ((_curPath.size() > 0) ? _curPath.back() : string("")) << endl;
	_log.flush();
}

string CyberCycles::MakePathName( std::string & curObjName )
{
	string newPath;

	for ( vector< string >::iterator pathIter = _curPath.begin(); pathIter != _curPath.end(); pathIter++ )
	{
		newPath += *pathIter + string("/");
	}

	newPath += curObjName;

	return newPath;
}
