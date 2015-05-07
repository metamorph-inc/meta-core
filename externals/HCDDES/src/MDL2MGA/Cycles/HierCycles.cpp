#include "Uml.h"
#include "UdmUtil.h"

#include "HierCycles.h"

using namespace std;
using namespace boost;

void ESMoLCycles::printLocalGraph( CompGraph & cg )
{
	std::ofstream out( "cycles.log", ios::app );
	CGVertexIter cgv, cgv_end;
	out << "Vertices: " << std::endl;
	for ( boost::tie( cgv, cgv_end ) = boost::vertices( cg ); cgv != cgv_end; cgv++ )
	{
		out << (*cgv) << ": " << cg[*cgv].path << std::endl;
	}
	out << "Edges: " << std::endl;
	CGEdgeIter cge, cge_end;
	for ( boost::tie( cge, cge_end) = boost::edges( cg ); cge != cge_end; cge++ )
	{
		out << boost::get( boost::edge_index, cg, *cge ) << ": " 
			<< boost::source( *cge, cg ) << ", " << boost::target( *cge, cg ) << " "
			<< ( ( cg[*cge].edgetype == connection ) ? "conn" 
			: ( ( cg[*cge].edgetype == pathedge ) ? "pathedge" : "unknown") ) << std::endl;
	}
	out << std::endl;
}

void ESMoLCycles::printCycle( const Udm::Object & obj, idxvector_t & cycle, CompGraph & cg, std::ostream & out )
{
		for ( idxvector_t::iterator ivIter = cycle.begin(); ivIter != cycle.end(); ivIter++ )
		{
			CGVertex cgv = vertex( *ivIter, cg );
			out << cg[cgv].path << " ";
		}
		out << std::endl;
}

CompGraph & ESMoLCycles::buildLocalGraph( ESMoL::Component & comp, std::map< idx_t, idx_t > & vertextable )
{
	// First, get the current path
	std::string curPath = getCurPath();

	CompGraph * cg;
	// get the graph object that we need to use - if it's not there then make one
	CompGraphMap::iterator cgIter = _compgraphs.find( comp.uniqueId() );
	if ( cgIter != _compgraphs.end() )
	{
		// if we found one, use it!
		cg = cgIter->second;
	}
	else
	{
		// if not, make one and save it
		cg = new CompGraph();
		_compgraphs[ comp.uniqueId() ] = cg;
	}

	// we need a local map from block ids to vertex ids
	std::set< idx_t > inputlist;  // keep track of subsystem input ports
	std::set< idx_t > outputlist; // keep track of subsystem output ports

	// add vertices from input ports
	InputSet inputs = comp.Input_kind_children();
	_elements_processed_count += inputs.size();
	for ( InputSet::iterator inpIter = inputs.begin(); inpIter != inputs.end(); inpIter++ )
	{
		CGVertex cgv = add_vertex(*cg);
		vertextable[ (*inpIter).uniqueId() ] = cgv; // put it in the vertex map

		// what do we need to keep in the vertex?
		//   path string, UDM object pointer?, what else?
		(*cg)[cgv].path = curPath + string( (*inpIter).name() );
		(*cg)[cgv].primtype = inport;

		(*cg)[cgv].udmobj = Udm::Object::Cast( (*inpIter) );
	}

	// add vertices from output ports
	OutputSet outputs = comp.Output_kind_children();
	_elements_processed_count += outputs.size();
	for ( OutputSet::iterator outpIter = outputs.begin(); outpIter != outputs.end(); outpIter++ )
	{
		CGVertex cgv = add_vertex(*cg);
		vertextable[ (*outpIter).uniqueId() ] = cgv; // put it in the vertex map

		// what do we need to keep in the vertex?
		//   path string, UDM object pointer?, what else?
		(*cg)[cgv].path = curPath + string( (*outpIter).name() );
		(*cg)[cgv].primtype = outport;

		(*cg)[cgv].udmobj = Udm::Object::Cast( (*outpIter) );
	}

	// add vertices from the ports of subsystem children
	SubsystemSet subsysChildren = comp.Subsystem_kind_children();
	_elements_processed_count += subsysChildren.size();
	for ( SubsystemSet::iterator subsysrefIter = subsysChildren.begin(); subsysrefIter != subsysChildren.end(); subsysrefIter++ )
	{
		InputPortSet inports = (*subsysrefIter).InputPort_kind_children();
		for ( InputPortSet::iterator inpIter = inports.begin(); inpIter != inports.end(); inpIter++ )
		{
			CGVertex cgv = add_vertex(*cg);
			vertextable[ (*inpIter).uniqueId() ] = cgv; // put it in the vertex map

			(*cg)[cgv].path = curPath + std::string("/") 
				+ std::string((*subsysrefIter).name()) + std::string("/") + std::string((*inpIter).name());
			(*cg)[cgv].primtype = subsys_inport;
			(*cg)[cgv].udmobj = Udm::Object::Cast( *inpIter );
		}

		OutputPortSet outports = (*subsysrefIter).OutputPort_kind_children();
		for ( OutputPortSet::iterator outpIter = outports.begin(); outpIter != outports.end(); outpIter++ )
		{
			CGVertex cgv = add_vertex(*cg);
			vertextable[ (*outpIter).uniqueId() ] = cgv; // put it in the vertex map

			(*cg)[cgv].path = curPath + std::string("/") 
				+ std::string((*subsysrefIter).name()) + std::string("/") + std::string((*outpIter).name());
			(*cg)[cgv].primtype = subsys_outport;
			(*cg)[cgv].udmobj = Udm::Object::Cast( *outpIter );
		}
	}

	// add vertices from the ports of subsystem ref children
	SubsystemRefSet subsysrefChildren = comp.SubsystemRef_kind_children();
	_elements_processed_count += subsysrefChildren.size();
	for ( SubsystemRefSet::iterator subsysrefIter = subsysrefChildren.begin(); subsysrefIter != subsysrefChildren.end(); subsysrefIter++ )
	{
		ESMoL::SubsystemRef_RefersTo_Base subsys_rrtb = (*subsysrefIter).ref();
		ESMoL::Subsystem subsys = ESMoL::Subsystem::Cast( subsys_rrtb );
		if ( subsys != Udm::null )
		{
			InputPortSet inports = subsys.InputPort_kind_children();
			for ( InputPortSet::iterator inpIter = inports.begin(); inpIter != inports.end(); inpIter++ )
			{
				CGVertex cgv = add_vertex(*cg);
				vertextable[ (*inpIter).uniqueId() ] = cgv; // put it in the vertex map

				(*cg)[cgv].path = curPath + std::string("/") 
					+ std::string((*subsysrefIter).name()) + std::string("/") + std::string((*inpIter).name());
				(*cg)[cgv].primtype = subsys_inport;
				(*cg)[cgv].udmobj = Udm::Object::Cast( *inpIter );
			}

			OutputPortSet outports = subsys.OutputPort_kind_children();
			for ( OutputPortSet::iterator outpIter = outports.begin(); outpIter != outports.end(); outpIter++ )
			{
				CGVertex cgv = add_vertex(*cg);
				vertextable[ (*outpIter).uniqueId() ] = cgv; // put it in the vertex map

				(*cg)[cgv].path = curPath + std::string("/") 
					+ std::string((*subsysrefIter).name()) + std::string("/") + std::string((*outpIter).name());
				(*cg)[cgv].primtype = subsys_outport;
				(*cg)[cgv].udmobj = Udm::Object::Cast( *outpIter );
			}
		}
	}

	// add edges corresponding to Connectors
	ConnectorSet connectors = comp.Connector_kind_children();
	for ( ConnectorSet::iterator connectorIter = connectors.begin(); connectorIter != connectors.end(); connectorIter++ )
	{

		ESMoL::Input inport = (*connectorIter).dstConnector_end();
		ESMoL::Output outport = (*connectorIter).srcConnector_end();

		Udm::Object inparent = inport.parent();
		Udm::Object outparent = outport.parent();

		// Cases: 
		// if outport is a MsgPort (messageref) and inport is an InputPort (subsysref|subsys)
		// if outport is an OutputPort (subsysref|subsys) and inport is a MsgPort (subsysref|subsys)
		// if outport is an OutputPort (subsysref|subsys) and inport is an InputPort (subsysref|subsys)

		// we'll have to use special IDs to identify the refport object combinations in the vertextable :-(

		// TODO: fix all this
		// Four cases - 1) primitive to primitive - connect the prim vertices directly with an edge
		//	if ( ( srcParent.type() == ESMoL::Primitive::meta ) 
		//		&& ( dstParent.type() == ESMoL::Primitive::meta ) )
		//	{
		//		CGVertex cgsrc = vertextable[ srcParent.uniqueId() ];
		//		CGVertex cgdst = vertextable[ dstParent.uniqueId() ];

		//		std::pair< CGEdge, bool > cge = add_edge( cgsrc, cgdst, *cg );

		//		(*cg)[cge.first].edgetype = connection;
		//		(*cg)[cge.first].udmobj   = Udm::Object::Cast( *lineIter );
		//	}

		//	// 2) primitive to subsystem - connect the prim vertex to the subsys port vertex
		//	//   NOTE: the subsystem parent could be the current subsystem -- the logic is still OK
		//	else if ( (srcParent.type() == ESMoL::Primitive::meta )
		//		&& ( dstParent.type() == ESMoL::Subsystem::meta ) )
		//	{
		//		CGVertex cgsrc = vertextable[ srcParent.uniqueId() ];
		//		CGVertex cgdst = vertextable[ dstPort.uniqueId() ];

		//		std::pair< CGEdge, bool > cge = add_edge( cgsrc, cgdst, *cg );

		//		(*cg)[cge.first].edgetype = connection;
		//		(*cg)[cge.first].udmobj   = Udm::Object::Cast( *lineIter );
		//	}

		//	// 3) subsystem to primitive - connect the subsys port vertex to the prim vertex
		//	//   NOTE: the subsystem parent could be the current subsystem
		//	else if ( (srcParent.type() == ESMoL::Subsystem::meta )
		//		&& ( dstParent.type() == ESMoL::Primitive::meta ) )
		//	{
		//		CGVertex cgsrc = vertextable[ srcPort.uniqueId() ];
		//		CGVertex cgdst = vertextable[ dstParent.uniqueId() ];

		//		std::pair< CGEdge, bool > cge = add_edge( cgsrc, cgdst, *cg );

		//		(*cg)[cge.first].edgetype = connection;
		//		(*cg)[cge.first].udmobj   = Udm::Object::Cast( *lineIter );
		//	}

		//	// 4) subsystem to subsystem - connect the subsys port vertex to the subsys port vertex
		//	//   NOTE: either subsystem parent could be the current subsystem
		//	else if ( (srcParent.type() == ESMoL::Subsystem::meta )
		//		&& ( dstParent.type() == ESMoL::Subsystem::meta ) )
		//	{
		//		CGVertex cgsrc = vertextable[ srcPort.uniqueId() ];
		//		CGVertex cgdst = vertextable[ dstPort.uniqueId() ];

		//		std::pair< CGEdge, bool > cge = add_edge( cgsrc, cgdst, *cg );

		//		(*cg)[cge.first].edgetype = connection;
		//		(*cg)[cge.first].udmobj   = Udm::Object::Cast( *lineIter );
		//	}
		//}

		//// add path edges from subsystem children
		//for ( SubsystemSet::iterator subsysrefIter = subsysChildren.begin(); subsysrefIter != subsysChildren.end(); subsysrefIter++ )
		//{
		//	PathEdgeSet pathedges = (*subsysrefIter).BlockPathEdge_kind_children();
		//	for ( PathEdgeSet::iterator edgeIter = pathedges.begin(); edgeIter != pathedges.end(); edgeIter++ )
		//	{
		//		// connect the endpoints
		//		ESMoL::Input inp = (*edgeIter).srcBlockPathEdge_end();
		//		ESMoL::Output outp = (*edgeIter).dstBlockPathEdge_end();

		//		ESMoL::InputPort inport = ESMoL::InputPort::Cast( inp );
		//		ESMoL::OutputPort outport = ESMoL::OutputPort::Cast( outp );

		//		CGVertex cgsrc = vertextable[ inport.uniqueId() ];
		//		CGVertex cgdst = vertextable[ outport.uniqueId() ];

		//		std::pair< CGEdge, bool > cge = add_edge( cgsrc, cgdst, *cg );

		//		(*cg)[cge.first].edgetype = pathedge;
		//		(*cg)[cge.first].udmobj   = Udm::Object::Cast( *edgeIter );
		//	}
	}

	return *cg;
}

void ESMoLCycles::clearModFlags( ESMoL::Component & comp )
{
	SubsystemSet subsysChildren = comp.Subsystem_kind_children();
	SubsystemRefSet subsysrefChildren = comp.SubsystemRef_kind_children();

	for ( SubsystemRefSet::iterator srIter = subsysrefChildren.begin(); srIter != subsysrefChildren.end(); srIter++ )
	{
		ESMoL::SubsystemRef_RefersTo_Base subsys_rrtb = (*srIter).ref();
		ESMoL::Subsystem subsys = ESMoL::Subsystem::Cast( subsys_rrtb );
		if ( subsys != Udm::null )
		{
			subsysChildren.insert( subsys );
		}
	}

	for ( SubsystemSet::iterator ssIter = subsysChildren.begin(); ssIter != subsysChildren.end(); ssIter++ )
	{
		clearModFlags( *ssIter );
		(*ssIter).SubsystemModified() = false;
	}
}

bool ESMoLCycles::findHierCycles( ESMoL::Component & comp )
{
	_paths.push_back( std::string( comp.name() ) );
	std::ofstream out( "cycles.log", ios::app );
	out << "Entering component definition " << getCurPath() << endl;
	bool success = false;
	_comp_count++;

	// First, process all subgraphs to get cycles and create path edges
	SubsystemSet subsysChildren = comp.Subsystem_kind_children();
	SubsystemRefSet subsysrefChildren = comp.SubsystemRef_kind_children();

	for ( SubsystemRefSet::iterator srIter = subsysrefChildren.begin(); srIter != subsysrefChildren.end(); srIter++ )
	{
		ESMoL::SubsystemRef_RefersTo_Base subsys_rrtb = (*srIter).ref();
		ESMoL::Subsystem subsys = ESMoL::Subsystem::Cast( subsys_rrtb );
		if ( subsys != Udm::null )
		{
			subsysChildren.insert( subsys );  // this should get rid of duplicates
		}
	}

	for ( SubsystemSet::iterator ssIter = subsysChildren.begin(); ssIter != subsysChildren.end(); ssIter++ )
	{
		findHierCycles( *ssIter );
	}

	// Collect the modified status, propagating child modifications upward
	bool selfModified = comp.ComponentModified();
	bool childModified = false;
	for ( SubsystemSet::iterator ssIter = subsysChildren.begin(); ssIter != subsysChildren.end(); ssIter++ )
	{
		childModified = childModified || (*ssIter).SubsystemModified();
	}
	comp.ComponentModified() = selfModified || childModified; // if this is modified or any child, then this is modified

	if ( comp.ComponentModified() )
	{
		_comp_processed_count++; 

		// Second, build the local graph including path edges
		std::map< idx_t, idx_t > vertextable;
		CompGraph & cg = buildLocalGraph( comp, vertextable );

		// Third, use Johnson's algorithm to find cycles here
		if ( boost::num_vertices( cg ) > 0 )
		{
			JohnsonsAlgorithm ja( cg );

			_cycles[ Udm::Object::Cast( comp ) ] = ja.getCycles(); // store cycle map
			//printCycle( Udm::Object::Cast( comp ) );

			// Create path edges for the parent to use
			createPathEdges( comp, vertextable );
		}
	}

	_paths.pop_back(); // remove this path from the stack

	return success;
}

// Rationale -- the guess is that the end vertex usually will have less edges than the start vertex
//  so -- we traverse backwards only for efficiency
void ESMoLCycles::buildPathEdgeSubgraph( const CGVertex & curv, const CGVertex & inpv, 
										 CompGraph & cg, CompGraph & tcg, CompGraph & subcg )
{
	//      traverse back from outpv (in original graph), along in_edges
	//      if source of in_edge has an in_edge source in trans closure pointing to startv then
	//         add edge to the subgraph

	//      Probably not: the gain should be very small
	//         maybe - cache the vertex -> inpv relationship in a map (startv -> set of vertices): speed up the check from n to logn
	
	// DEBUG ofstream out( "cycles.log", ios::app );
	//out << "Back from " << (*cg)[curv].path << " towards input " << (*cg)[inpv].path << std::endl;
	//out << "Vertex " << (*cg)[curv].path << " has " << in_degree(curv, *cg) << " in edges." << std::endl;

	CGInEdgeIter cgie, cgie_end;
	for ( tie( cgie, cgie_end ) = in_edges( curv, cg ); cgie != cgie_end; cgie++ )
	{

		CGVertex v = source( *cgie, cg );
		if ( get( vertex_color, cg, v ) != white_color )
			continue;  // avoid back edges

		put( vertex_color, cg, v, gray_color );

		if ( v == inpv )
		{
			CGVertex localv, localcurv;
			bool found = false;

			tie( localv, found ) = subcg.find_vertex( v );
			if ( !found )
			{
				localv = add_vertex(v, subcg);
			}

			found = false;
			tie( localcurv, found ) = subcg.find_vertex( curv );
			if ( !found )
			{
				localcurv = add_vertex( curv, subcg );
			}
			
			//pair< CGEdge, bool > e;
			//e = add_edge( localv, localcurv, subcg );

			//if ( e.second )
			//{
			//	subcg[e.first].edgetype = (*cg)[*cgie].edgetype;
			//	subcg[e.first].udmobj = (*cg)[*cgie].udmobj;
			//	out << "  Created edge from " << (subcg)[localv].path << " to " << (subcg)[localcurv].path << std::endl;
			//}

			//continue;
		}
		else
		{
			// check back to see whether one of the in_edges touches inpv in the transitive closure
			//out << "  Vertex " << tcg[v].path << " reaches back to " << in_degree( v, tcg ) << " in the trans closure." << endl;
			CGInEdgeIter cgie2, cgie2_end;
			for ( tie( cgie2, cgie2_end ) = in_edges( v, tcg ); cgie2 != cgie2_end; cgie2++ )
			{
				CGVertex iv = source( *cgie2, tcg );
				if ( get( vertex_index, tcg, iv ) == get( vertex_index, cg, inpv ) )
				{
					CGVertex localv, localcurv;
					bool found = false;

					tie( localv, found ) = subcg.find_vertex( v );
					if ( !found )
					{
						localv = add_vertex(v, subcg);
					}

					found = false;
					tie( localcurv, found ) = subcg.find_vertex( curv );
					if ( !found )
					{
						localcurv = add_vertex( curv, subcg );
					}

					//pair< CGEdge, bool > e;
					//e = add_edge( localv, localcurv, subcg );
					//out << "  Created edge from " << (subcg)[localv].path << " to " << (subcg)[localcurv].path << std::endl;

					//if ( e.second ) // only continue building from here if we have not inserted this edge already
					//{
					//	subcg[e.first].edgetype = (*cg)[*cgie].edgetype;
					//	subcg[e.first].udmobj = (*cg)[*cgie].udmobj;
					//	out << "  Copied edge props" << endl;
					buildPathEdgeSubgraph( v, inpv, cg, tcg, subcg );
						//break;
					//}
				}
			}
		}
	}
}

// outline the depth algorithm:
//  for each cycle -
//    copy the vertices from the cycle into the new compgraph (i.e. create new vertices and copy the properties)
//    if the edge between vertex a and its successor is NOT a pathedge
//        create an edge
//    else
//        get the connectivity graph for that edge
bool ESMoLCycles::extractFullCycle( idxvector_t & cycle, CompGraph & cg, CompGraph & newcg )
{
	bool foundPathEdge = false;
	idx_t cycle_edges_found = 0;
	idxmap_t old2new, new2old, cyclookup;
	
	idx_t idx;
	for ( idx = 0; idx < cycle.size()-1; idx++ )
	{
		cyclookup[cycle[idx]] = idx;
	}

	CGEdgeIter ei, ei_end;
	for ( tie(ei, ei_end) = edges( cg ); (ei != ei_end) && (cycle_edges_found < cycle.size()); ei++ )
	{
		CGVertex oldstartv = source(*ei, cg);  // old graph index
		idxmap_t::iterator src = cyclookup.find( oldstartv ); // (old idx, cyc vector idx)
		if ( src == cyclookup.end() )
			continue;

		CGVertex oldendv = target(*ei, cg);  // old idx
		idxmap_t::iterator tgt = cyclookup.find( oldendv ); // (old idx, cyc vector idx)
		if ( tgt == cyclookup.end() )
			continue;

		cycle_edges_found++;

		// add these cycle endpoints, properties, and edge to the new graph
		CGVertex startv, endv;
		idxmap_t::iterator findsrc = old2new.find( oldstartv ); // (old idx, new idx)
		if ( findsrc == old2new.end() )
		{
			startv = add_vertex(newcg);  // new idx
			newcg[startv].path = cg[oldstartv].path;
			newcg[startv].primtype = cg[oldstartv].primtype;
			newcg[startv].udmobj = cg[oldstartv].udmobj;
			old2new[oldstartv] = startv;
			new2old[startv] = oldstartv;
		}
		else
		{
			startv = findsrc->second;
		}

		idxmap_t::iterator findtgt = old2new.find( oldendv ); // (old idx, new idx)
		if ( findtgt == old2new.end() )
		{
			endv = add_vertex(newcg); // new idx
			newcg[endv].path = cg[oldendv].path;
			newcg[endv].primtype = cg[oldendv].primtype;
			newcg[endv].udmobj = cg[oldendv].udmobj;
			old2new[oldendv] = endv;
			new2old[endv] = oldendv;
		}
		else
		{
			endv = findtgt->second;
		}

		if ( cg[*ei].edgetype == pathedge )
		{
			foundPathEdge = true;
			getConnectedPaths( cg[*ei].udmobj, startv, endv, cg, newcg );
		}
		else
		{
			pair< CGEdge, bool > e;
			e = add_edge( startv, endv, newcg );
			if ( e.second )
			{
				newcg[e.first].edgetype = cg[*ei].edgetype;
				newcg[e.first].udmobj = cg[*ei].udmobj;
			}
		}
	}

	return foundPathEdge;
}

void ESMoLCycles::getConnectedPaths( Udm::Object & obj, const CGVertex & startv, 
									 const CGVertex & endv, CompGraph & cg, CompGraph & newcg )
{
	// outline
	//   Look up the connectivity graph for this object
	//      How do we find the old index of the start and end objects?  That's important
	//		Check for the primtype property for inport, outport
	//   Process the edges of the connectivity graph in much the same way as we did above,
	//      matching the start and end vertices to those passed in here.
	//   Recursively call this on path edges in the connectivity graph

	// Try to get the path edge subgraph for this path edge object
	// If not, we'll have to make one
	
	// Resolve scope problems by declaring these out here
	CompGraph bp;
	CompGraph & subcg( bp.create_subgraph() );

	CompGraph * conng; 
	CompGraphMap::iterator psi = _pathedge_subgraphs.find( obj.uniqueId() );
	if ( psi != _pathedge_subgraphs.end() )
	{
		conng = psi->second;
	}
	else
	{
		// Recreate everything?
		Udm::Object par_obj = obj.GetParent();
		if ( (par_obj != Udm::null) && (par_obj.type() == ESMoL::Subsystem::meta) )
		{
			ESMoL::Subsystem subsys = ESMoL::Subsystem::Cast( par_obj );
			ESMoL::BlockPathEdge bpe = ESMoL::BlockPathEdge::Cast( obj );

			ESMoL::Input inp = bpe.srcBlockPathEdge_end();
			ESMoL::Output outp = bpe.dstBlockPathEdge_end();

			// Have these been stored?  Which graph do they correspond to?
			// Probably not --
			//    The subsystems no longer have cached graph information
			//    
			//CGVertex inpv = vertextable[ inp.uniqueId() ];
			//CGVertex outpv = vertextable[ outp.uniqueId() ];

			// BEST APPROACH SO FAR:
			// Create a common holder class for Line and PathEdge
			// Collect all of the edges (of both types) in a set
			// Walk backwards from the output vertex
			// Idea: Use a set for each vertex to collect the vertices visited so far 
			//    moving back from the output
			// Create a graph as we walk back, with a subgraph for input-reachable vertices.
			// Collect vertices into the sets a we move back.
			// Keep track of the edges visited as we move back, avoiding back edges.
			// When we terminate (by running out of edges for traversal),
			//     add all of the vertices from the set into the subgraph, and then report the subgraph.

			/* ObjectSet comp_edges;
			PathEdgeSet pathedges = subsys.BlockPathEdge_children();
			for ( PathEdgeSet::iterator pei = pathedges.begin(); pei != pathedges.end(); pei++ )
			{
				comp_edges.insert( Udm::Object::Cast( *pei ) );
			}

			LineSet lines = subsys.Line_kind_children();
			for ( LineSet::iterator li = lines.begin(); li != lines.end(); li++ )
			{
				comp_edges.insert( Udm::Object::Cast( *li ) );
			} */

			CGVertex bpendv = add_vertex( bp );
			bp[ bpendv ].path = newcg[ endv ].path;
			//bp[ bpendv ].primtype = newcg[ endv ].primtype;
			bp[ bpendv ].primtype = outport;
			bp[ bpendv ].udmobj = newcg[ endv ].udmobj;

			CGVertex bpstartv = add_vertex( bp );
			bp[ bpstartv ].path = newcg[ startv ].path;
			//bp[ bpstartv ].primtype = newcg[ startv ].primtype;
			bp[ bpstartv ].primtype = inport;
			bp[ bpstartv ].udmobj = newcg[ startv ].udmobj;

			idxsetmap_t comp_vertices;
			idxmap_t	vertextable;
			comp_vertices[ bpendv ].insert( bpendv );
			vertextable[ inp.uniqueId() ] = bpstartv;
			vertextable[ outp.uniqueId() ] = bpendv;

			string pathname;
			string prepathname = newcg[startv].path;
			getPathName( prepathname, pathname );

			string epathname;
			string preepathname = newcg[endv].path;
			getPathName( preepathname, epathname );

			rebuildConnGraph( bpstartv, bpendv, bp, inp, outp, subsys,
							  pathname, comp_vertices, vertextable );
	
			// Create a subgraph of bp,
			//    Look up comp_vertices[bpstartv]
			//    Add all of those vertices to the subgraph
			//    Return the subgraph as the result (subcg)

			// End of new stuff

			idx_t bpidx = obj.uniqueId();
			idxset_t conn_to_start = comp_vertices[ bpstartv ];

			for ( idxset_t::iterator idi = conn_to_start.begin(); idi != conn_to_start.end(); idi++ )
			{
				add_vertex( *idi, subcg );
			}

			conng = &subcg; 
			
			//_pathedge_subgraphs[bpidx] = conng; // Keep a reference to that storage
			

			// DEBUG 
			// ofstream out( "cycles.log", ios::app );
			// out << "Rebuilt graph for " << subsys.name() << " : v " << num_vertices( subcg )
			//	<< " e " << num_edges( subcg ) << std::endl;

			/* DEBUG
			{ // Dump graph files to see what's happening
				string pname = newcg[startv].path;
				size_t pos = pname.find( '/' );
				while ( pos != string::npos )
				{
					pname.replace(pos, 1, "_");
					pos = pname.find( '/' );
				}

				ostringstream fname;
				fname << pname << bpe.uniqueId() << ".dot";
				ofstream pfile( fname.str().c_str() );
				write_graphviz( pfile, (*conng), CGLabelWriter((*conng)), CGLabelWriter((*conng)) );
			} */
		}
	}
	
	/* { // Dump graph files to see what's happening
		string pname = newcg[startv].path;
		size_t pos = pname.find( '/' );
		while ( pos != string::npos )
		{
			pname.replace(pos,1,"_");
			pos = pname.find('/');
		}
		
		ostringstream fname;
		fname << pname << obj.uniqueId() << ".dot";
		ofstream pfile( fname.str().c_str() );
		write_graphviz( pfile, *conng, CGLabelWriter(*conng), CGLabelWriter(*conng) );
	} */
	
	idxmap_t old2new, new2old;

	CGEdgeIter ei, ei_end;
	for ( tie( ei, ei_end ) = edges( *conng ); ei != ei_end; ei++ )
	{
		CGVertex oldsrcv = source( *ei, *conng );  // old graph index
		CGVertex oldtgtv = target( *ei, *conng );  // old idx

		// add these cycle endpoints, properties, and edge to the new graph
		CGVertex newsrcv, newtgtv;
		idxmap_t::iterator findsrc = old2new.find( oldsrcv ); // (old idx, new idx)
		if ( findsrc == old2new.end() )
		{
			if ( (*conng)[oldsrcv].primtype == inport )
			{
				newsrcv = startv;
				old2new[oldsrcv] = startv;
				new2old[startv] = oldsrcv;
			}
			else
			{
				newsrcv = add_vertex(newcg);  // new idx
				newcg[newsrcv].path = (*conng)[oldsrcv].path;
				newcg[newsrcv].primtype = (*conng)[oldsrcv].primtype;
				newcg[newsrcv].udmobj = (*conng)[oldsrcv].udmobj;
				old2new[oldsrcv] = newsrcv;
				new2old[newsrcv] = oldsrcv;
			}
		}
		else
		{
			newsrcv = findsrc->second;
		}

		idxmap_t::iterator findtgt = old2new.find( oldtgtv ); // (old idx, new idx)
		if ( findtgt == old2new.end() )
		{
			if ( (*conng)[oldtgtv].primtype == outport )
			{
				newtgtv = endv;
				old2new[oldtgtv] = endv;
				new2old[endv] = oldtgtv;
			}
			else
			{
				newtgtv = add_vertex(newcg);  // new idx
				newcg[newtgtv].path = (*conng)[oldtgtv].path;
				newcg[newtgtv].primtype = (*conng)[oldtgtv].primtype;
				newcg[newtgtv].udmobj = (*conng)[oldtgtv].udmobj;
				old2new[oldtgtv] = newtgtv;
				new2old[newtgtv] = oldtgtv;
			}
		}
		else
		{
			newtgtv = findtgt->second;
		}

		if ( (*conng)[*ei].edgetype == pathedge )
		{
			getConnectedPaths( (*conng)[*ei].udmobj, newsrcv, newtgtv, cg, newcg );
		}
		else
		{
			pair< CGEdge, bool > e;
			e = add_edge( newsrcv, newtgtv, newcg );
			if ( e.second )
			{
				newcg[e.first].edgetype = (*conng)[*ei].edgetype;
				newcg[e.first].udmobj = (*conng)[*ei].udmobj;
			}
		}
	}
}

void ESMoLCycles::rebuildConnGraph( const CGVertex & bpstartv, const CGVertex & bpcurv, CompGraph & bp, 
								    ESMoL::Input & inp, Udm::Object & curp, ESMoL::Subsystem & subsys, 
								    std::string pathname, idxsetmap_t & comp_vertices, idxmap_t & vertextable )
{

	// if curp is an Output port, 
	//    if curp has subsys as its parent (i.e. it's the exit port)
	//       get the Lines which connect to it
	//    if curp has a different subsys as its parent
	//       get the PathEdges which connect to it
	// if curp is an Input port
	//    get Lines which connect to outp

	idxset_t idxs = comp_vertices[ bpcurv ];

	// DEBUG ofstream out( "cycles.log", ios::app );
	string curnm = UdmUtil::ExtractName( curp );

	
	if ( curp.type() == ESMoL::OutputPort::meta )
	{
		ESMoL::Output outp_p = ESMoL::Output::Cast( curp );
		if ( curp.GetParent().uniqueId() == subsys.uniqueId() ) // this is the exit vertex
		{
			// DEBUG out << pathname << "/" << curnm << endl;
			ESMoL::OutputPort outp_pp = ESMoL::OutputPort::Cast( curp );
			LineSet lines = outp_pp.srcLine();

			for ( LineSet::iterator li = lines.begin(); li != lines.end(); li++ )
			{	
				ESMoL::Port port = (*li).srcLine_end();
				if ( port.uniqueId() == inp.uniqueId() )  // we made it!
				{
					// DEBUG out << "Reached InputPort " << pathname << "/" << port.name() << endl;
					// the interesting vertex is the input vertex
					comp_vertices[ bpstartv ].insert( idxs.begin(), idxs.end() );
					comp_vertices[ bpstartv ].insert( bpstartv );
					pair< CGEdge, bool > e = add_edge( bpstartv, bpcurv, bp );
					if ( e.second )
					{
						bp[e.first].edgetype = connection;
						bp[e.first].udmobj = Udm::Object::Cast( *li );
					}
				}
				else if ( port.type() == ESMoL::OutputPort::meta  // port on a primitive
					&& port.GetParent().type() == ESMoL::Primitive::meta )
				{
					string nm = UdmUtil::ExtractName( port.GetParent() );
					// DEBUG out << "Prim " << pathname << "/" << nm << endl;
					CGVertex srcv;
					idxmap_t::iterator foundit = vertextable.find( port.GetParent().uniqueId() );
					if ( foundit != vertextable.end() )
					{
						srcv = foundit->second;
					}
					else
					{	
						srcv = add_vertex( bp );
						bp[srcv].path = pathname + string("/") + string( nm );
						bp[srcv].primtype = prim;
						bp[srcv].udmobj = port.GetParent();
						vertextable[ port.GetParent().uniqueId() ] = srcv;
					}

					pair< CGEdge, bool > e = add_edge( srcv, bpcurv, bp );
					if ( e.second )
					{
						bp[e.first].edgetype = connection;
						bp[e.first].udmobj = Udm::Object::Cast( *li );
					}

					comp_vertices[ srcv ].insert( idxs.begin(), idxs.end() );
					comp_vertices[ srcv ].insert( srcv );

					if ( foundit == vertextable.end() )
					{
						ESMoL::Primitive par = ESMoL::Primitive::Cast( port.GetParent() );
						InputPortSet iports = par.InputPort_kind_children();
						for ( InputPortSet::iterator ipi = iports.begin(); ipi != iports.end(); ipi++ )
						{
							rebuildConnGraph( bpstartv, srcv, bp, 
												inp, Udm::Object::Cast(*ipi), subsys, 
												pathname, comp_vertices, vertextable );
						}
					}

					// DEBUG out << "Finished Prim " << pathname << "/" << nm << endl;
				}
				else if ( port.type() == ESMoL::OutputPort::meta  // port on a subsystem
					&& port.GetParent().type() == ESMoL::Subsystem::meta )
				{
					string pnm = UdmUtil::ExtractName( port.GetParent() );
					// DEBUG out << "Subsys OutputPort " << pathname << "/" << pnm << "/" << port.name() << endl;
					CGVertex srcv;
					idxmap_t::iterator foundit = vertextable.find( port.uniqueId() );
					if ( foundit != vertextable.end() )
					{
						srcv = foundit->second;
					}
					else
					{
						string nm = port.name();
						
						srcv = add_vertex( bp );
						bp[srcv].path = pathname + string("/") + pnm 
										+ string("/") + string( nm );
						bp[srcv].primtype = subsys_outport;
						bp[srcv].udmobj = Udm::Object::Cast(port);
						vertextable[ port.uniqueId() ] = srcv;
					}

					pair< CGEdge, bool > e = add_edge( srcv, bpcurv, bp );
					if ( e.second )
					{
						bp[e.first].edgetype = connection;
						bp[e.first].udmobj = Udm::Object::Cast( *li );
					}

					comp_vertices[ srcv ].insert( idxs.begin(), idxs.end() );
					comp_vertices[ srcv ].insert( srcv );

					if ( foundit == vertextable.end() )
					{
						rebuildConnGraph( bpstartv, srcv, bp, 
											inp, Udm::Object::Cast( port ), subsys, 
											pathname, comp_vertices, vertextable );
					}

					// DEBUG out << "Finished Subsys OutputPort " << pathname << "/" << pnm << "/" << port.name() << endl;
				}
			}
		} // end of case for subsys output port (exit vertex)
		else if ( curp.GetParent().uniqueId() != subsys.uniqueId() ) // not the exit vertex, so it must be on a subsystem
		{
			string pnm = UdmUtil::ExtractName( curp.GetParent() );
			// DEBUG out << pathname << "/" << pnm << "/" << curnm << endl;
			ESMoL::Output outp = ESMoL::Output::Cast( curp );
			PathEdgeSet pedges = outp.srcBlockPathEdge();
			for ( PathEdgeSet::iterator pei = pedges.begin(); pei != pedges.end(); pei++ )
			{
				ESMoL::Input pinp = (*pei).srcBlockPathEdge_end();
				CGVertex srcv;

				// DEBUG out << "Subsys InputPort " << pathname << "/" << pnm << "/" << pinp.name() << endl;
				idxmap_t::iterator foundit = vertextable.find( pinp.uniqueId() );
				if ( foundit != vertextable.end() )
				{
					srcv = vertex( foundit->second, bp );
				}
				else
				{
					string nm = pinp.name();
					
					srcv = add_vertex( bp );
					bp[srcv].path = pathname + string("/") + pnm 
						+ string("/") + string( nm );
					bp[srcv].primtype = subsys_inport;
					bp[srcv].udmobj = Udm::Object::Cast(pinp);
					vertextable[ pinp.uniqueId() ] = srcv;
				}

				pair< CGEdge, bool > e = add_edge( srcv, bpcurv, bp );
				if ( e.second )
				{
					bp[e.first].edgetype = pathedge;
					bp[e.first].udmobj = Udm::Object::Cast(*pei);
				}

				comp_vertices[ srcv ].insert( idxs.begin(), idxs.end() );
				comp_vertices[ srcv ].insert( srcv );

				if ( foundit == vertextable.end() )
				{
					rebuildConnGraph( bpstartv, srcv, bp,
										inp, Udm::Object::Cast( pinp ), subsys,
										pathname, comp_vertices, vertextable );
				}

				// DEBUG out << "Finished Subsys InputPort " << pathname << "/" << pnm << "/" << pinp.name() << endl;
			}  // iter over path edges
		} // not exit vertex, output on subsys
	}  // curp = output type vertex 
	else if ( curp.type() == ESMoL::InputPort::meta )
	{
		ESMoL::Input inp_p = ESMoL::Input::Cast( curp );
		ESMoL::InputPort inp_pp = ESMoL::InputPort::Cast( curp );
		LineSet lines = inp_pp.srcLine();

		string pnm = UdmUtil::ExtractName( curp.GetParent() );
		// DEBUG out << pathname << "/" << pnm << "/" << curnm << endl;

		for ( LineSet::iterator li = lines.begin(); li != lines.end(); li++ )
		{
			ESMoL::Port port = (*li).srcLine_end();
			if ( port.uniqueId() == inp.uniqueId() )  // we made it!
			{
				// DEBUG out << "Reached InputPort " << pathname << "/" << port.name() << endl;
				// the interesting vertex is the input vertex
				comp_vertices[ bpstartv ].insert( idxs.begin(), idxs.end() );
				comp_vertices[ bpstartv ].insert( bpstartv );
				pair< CGEdge, bool > e = add_edge( bpstartv, bpcurv, bp );
				if ( e.second )
				{
					bp[e.first].edgetype = connection;
					bp[e.first].udmobj = Udm::Object::Cast( *li );
				}


			}
			else if ( port.type() == ESMoL::OutputPort::meta  // port on a primitive
						&& port.GetParent().type() == ESMoL::Primitive::meta )
			{
				string nm = UdmUtil::ExtractName( port.GetParent() );
				// DEBUG out << "Prim " << pathname << "/" << nm << endl;
				CGVertex srcv;
				idxmap_t::iterator foundit = vertextable.find( port.GetParent().uniqueId() );
				if ( foundit != vertextable.end() )
				{
					srcv = vertex( foundit->second, bp );
				}
				else
				{
					
					srcv = add_vertex( bp );
					bp[srcv].path = pathname + string( nm );
					bp[srcv].primtype = prim;
					bp[srcv].udmobj = port.GetParent();
					vertextable[ port.GetParent().uniqueId() ] = srcv;
				}

				pair< CGEdge, bool > e = add_edge( srcv, bpcurv, bp );
				if ( e.second )
				{
					bp[e.first].edgetype = connection;
					bp[e.first].udmobj = Udm::Object::Cast( *li );
				}

				comp_vertices[ srcv ].insert( idxs.begin(), idxs.end() );
				comp_vertices[ srcv ].insert( srcv );

				if ( foundit == vertextable.end() )
				{
					ESMoL::Primitive par = ESMoL::Primitive::Cast( port.GetParent() );
					InputPortSet iports = par.InputPort_kind_children();
					for ( InputPortSet::iterator ipi = iports.begin(); ipi != iports.end(); ipi++ )
					{
						rebuildConnGraph( bpstartv, srcv, bp, 
											inp, Udm::Object::Cast(*ipi), subsys, 
											pathname, comp_vertices, vertextable );
					}
				}

				// DEBUG out << "Finished Prim " << pathname << "/" << nm << endl;
			}
			else if ( port.type() == ESMoL::OutputPort::meta  // port on a subsystem
				&& port.GetParent().type() == ESMoL::Subsystem::meta )
			{
				string pnm = UdmUtil::ExtractName( port.GetParent() );
				// DEBUG out << "Subsys OutputPort " << pathname << "/" << pnm << "/" << port.name() << endl;
				CGVertex srcv;
				idxmap_t::iterator foundit = vertextable.find( port.uniqueId() );
				if ( foundit != vertextable.end() )
				{
					srcv = foundit->second;
				}
				else
				{
					string nm = port.name();
					
					srcv = add_vertex( bp );
					bp[srcv].path = pathname + string("/") + pnm 
						+ string("/") + string( nm );
					bp[srcv].primtype = subsys_outport;
					bp[srcv].udmobj = Udm::Object::Cast(port);
					vertextable[ port.uniqueId() ] = srcv;
				}

				pair< CGEdge, bool > e = add_edge( srcv, bpcurv, bp );
				if ( e.second )
				{
					bp[e.first].edgetype = connection;
					bp[e.first].udmobj = Udm::Object::Cast( *li );
				}

				comp_vertices[ srcv ].insert( idxs.begin(), idxs.end() );
				comp_vertices[ srcv ].insert( srcv );

				if ( foundit == vertextable.end() )
				{
					rebuildConnGraph( bpstartv, srcv, bp, 
						inp, Udm::Object::Cast( port ), subsys, 
						pathname, comp_vertices, vertextable );
				}

				// DEBUG out << "Finished Subsys OutputPort " << pathname << "/" << pnm << "/" << port.name() << endl;
			}
		}
	}
}

void ESMoLCycles::getPathName( const std::string & vpath, std::string & pathname )
{
	string vertex_path( vpath );

	// Find the last bit of name in the path
	size_t pos = vertex_path.rfind( '/' );
	if ( pos != string::npos )
	{
		pathname = vertex_path.substr( 0, pos );
	}
}

/*
// Second, build the local graph including path edges
std::map< idx_t, idx_t > vertextable;
CompGraph & cg = buildLocalGraph( subsys, vertextable );

CGUtils::CGResetColor( cg );

// Get the transitive closure
CompGraph tcg;
// Transitive closure may be more efficient than multiple DFS passes (not much)
boost::transitive_closure( cg, tcg );

// Recursively walk backwards from the output and build the path
buildPathEdgeSubgraph( outpv, inpv, cg, tcg, subcg );
*/
