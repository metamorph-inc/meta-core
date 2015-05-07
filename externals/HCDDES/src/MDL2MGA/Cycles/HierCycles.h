#ifndef HierCycles_h__
#define HierCycles_h__

#include <map>
#include <vector>
#include <string>
#include <set>
#include <iostream>
#include <stack>
#include <fstream>

#include "boost/graph/transitive_closure.hpp"

#include "ESMoL.h"

#include "CycleGraphs.h"
#include "JohnsonsAlgorithm.h"

typedef std::set< ESMoL::Subsystem >		SubsystemSet;
typedef std::set< ESMoL::SubsystemRef >		SubsystemRefSet;
typedef std::set< ESMoL::Primitive >		PrimitiveSet;
typedef std::set< ESMoL::InputPort >		InputPortSet;
typedef std::set< ESMoL::OutputPort >		OutputPortSet;
typedef std::set< ESMoL::Input >			InputSet;
typedef std::set< ESMoL::Output >			OutputSet;
typedef std::set< ESMoL::Line >				LineSet;
typedef std::set< ESMoL::Connector >		ConnectorSet;
typedef std::set< ESMoL::BlockPathEdge >	PathEdgeSet;
typedef std::set< Udm::Object >				ObjectSet;

typedef std::map< Udm::Object, cyclevector_t > CycleMap; 
typedef std::vector< std::string > PathList;
typedef std::map< idx_t, CompGraph * > CompGraphMap;
//typedef std::vector< std::vector< std::string > > FullCycleList;

class ESMoLCycles
{
public:
	ESMoLCycles() :  _comp_count(0), _comp_processed_count(0), 
		_elements_processed_count(0), _abstract_cycles(0), _total_cycles(0) { }
	virtual ~ESMoLCycles() { }

	bool findHierCycles( ESMoL::Subsystem & subsys )
	{
		_paths.push_back( std::string( subsys.name() ) );
		std::ofstream out( "cycles.log", ios::app );
		// DEBUG out << "Entering hierarchical subsys " << getCurPath() << endl;
		bool success = false;
		_comp_count++;

		// First, process all subgraphs to get cycles and create path edges
		SubsystemSet subsysChildren = subsys.Subsystem_kind_children();
		for ( SubsystemSet::iterator ssIter = subsysChildren.begin(); ssIter != subsysChildren.end(); ssIter++ )
		{
			findHierCycles( *ssIter );
		}

		// Collect the modified status, propagating child modifications upward
		bool selfModified = subsys.SubsystemModified();
		bool childModified = false;
		for ( SubsystemSet::iterator ssIter = subsysChildren.begin(); ssIter != subsysChildren.end(); ssIter++ )
		{
			childModified = childModified || (*ssIter).SubsystemModified();
		}
		subsys.SubsystemModified() = selfModified || childModified; // if this is modified or any child, then this is modified

		if ( subsys.SubsystemModified() )
		{
			_comp_processed_count++; 

			// Second, build the local graph including path edges
			idxmap_t vertextable;
			CompGraph & cg = buildLocalGraph( subsys, vertextable );

			// Third, use Johnson's algorithm to find cycles here
			if ( boost::num_vertices( cg ) > 0 )
			{
				JohnsonsAlgorithm ja( cg );
			
				// at this level
				//   Take the cycle list from above (we want the index version)
				//   Invoke the depth algorithm on it
				//_cycles[ Udm::Object::Cast( subsys ) ] = ja.getCycles(); // store cycle map
				cyclevector_t cycles = ja.getCycles();
				std::string name = string( subsys.name() );
				//std::string fname = name + std::string("_cycles.log");
				std::string fname = std::string("cycles.log");
				std::ofstream out( fname.c_str(), ios::app );
				out << "Cycles for subsystem " << name << " (" << cycles.size() << "):" << std::endl;
				_abstract_cycles += cycles.size();
				idx_t count = 0;
				for ( cyclevector_t::iterator icycle = cycles.begin(); icycle != cycles.end(); icycle++ )
				{
					CompGraph cycg;
					//extractFullCycle( *icycle, cg, cycg );
					
					if ( extractFullCycle( *icycle, cg, cycg ) ) // did we find path edges in icycle?
					{
						/* DEBUG
						{
							// Graph debugging
							out << count << ": c " << (*icycle).size() << " v " 
								<< num_vertices( cycg ) << " e " << num_edges( cycg ) << " " << std::endl;

							std::ostringstream fname1;
							fname1 << subsys.name() << "_cycle" << ++count << ".dot";
							std::ofstream out1( fname1.str().c_str() );
							write_graphviz( out1, cycg, 
											CGLabelWriter( cycg ), 
											CGLabelWriter( cycg ) );
						} */
						JohnsonsAlgorithm jacyc( cycg );
						cyclevector_t cycle_cycles = jacyc.getCycles();
						out << "Real Cycles (" << cycle_cycles.size() << "):" << std::endl;
						_total_cycles += cycle_cycles.size();
						if ( cycle_cycles.size() )
							printCycles( Udm::Object::Cast( subsys ), cycle_cycles, cycg, out );
					}
					else
					{
						printCycle( Udm::Object::Cast( subsys ), *icycle, cg, out ); // no path edges in *icycle
						_total_cycles++;
					}
				}

				// Create path edges for the parent to use
				createPathEdges( subsys, vertextable );
			}
		}
		
		_paths.pop_back(); // remove this path from the stack

		return success;
	}

	bool findHierCycles( ESMoL::Component & comp );

	// Extract the full cycle list using the cycle map, and then add the cycles to the full_cycles list here
	//void getFullCycleList( FullCycleList & full_cycles ) 
	//{
		//throw std::exception("The method or operation is not implemented.");
	//}

	void createPathEdges( ESMoL::Subsystem & subsys, std::map< idx_t, idx_t > & vertextable ) 
	{
		//  Here is what needs to be done:
	
		//  Clear all old path edges
		//  Get the stored compgraph for this subsystem
		//  Perform a DFS from each inport - 
		//      outports[ inport ] = []
		//      if we visit any outports which have the current parent, 
		//          outports[ inport ] += outport
		//      ignore back edges?  - I think so

		// Convoluted UDM delete!
		PathEdgeSet emptypathedges;    // create empty set
		subsys.BlockPathEdge_children() = emptypathedges; // assign the empty set to the BPE child set

		CompGraph * cg = _compgraphs[ subsys.uniqueId() ];
		CompGraph tcg;
		// Transitive closure may be more efficient than multiple DFS passes (not much)
		boost::transitive_closure( *cg, tcg );

		CGVertexIter cgv_src, cgv_end;
		for ( boost::tie( cgv_src, cgv_end) = boost::vertices( tcg ); cgv_src != cgv_end; cgv_src++ )
		{
			// if this is a starting vertex
			if ( ( boost::in_degree( *cgv_src, tcg ) == 0 ) && ( (*cg)[*cgv_src].primtype == inport ) )  
			{
				ESMoL::InputPort inp = ESMoL::InputPort::Cast((*cg)[*cgv_src].udmobj);
				PathEdgeSet pathedges = inp.dstBlockPathEdge();
				OutputPortSet outputports;
				for ( PathEdgeSet::iterator peIter = pathedges.begin(); peIter != pathedges.end(); peIter++ )
				{
					if ( (*peIter).type() == ESMoL::OutputPort::meta )
					{
						outputports.insert( ESMoL::OutputPort::Cast( (*peIter).dstBlockPathEdge_end() ) );
					}
				}

				CGOutEdgeIter cge, cge_end;
				for ( boost::tie(cge, cge_end) = boost::out_edges( *cgv_src, tcg ); cge != cge_end; cge++ )
				{
					CGVertex cgv_dst = boost::target( *cge, tcg );

					// we have an ending vertex!
					if ( ( boost::out_degree( cgv_dst, tcg ) == 0 ) && ( (*cg)[cgv_dst].primtype == outport ) )
					{
						// Create the path edge only if necessary
						ESMoL::OutputPort outp = ESMoL::OutputPort::Cast((*cg)[cgv_dst].udmobj);

						if ( outputports.find( outp ) == outputports.end() )
						{
							// DEBUG ofstream out( "cycles.log", ios::app );
							//out << "Creating path edge between Input " << inp.name() << " (" << (*cg)[*cgv_src].path 
							//	<< ") and Output " << outp.name() << " (" << (*cg)[cgv_dst].path << ")" << std::endl;

							try {
								ESMoL::BlockPathEdge bpe = ESMoL::BlockPathEdge::Create( subsys );
								bpe.srcBlockPathEdge_end() = ESMoL::Input::Cast( inp );
								bpe.dstBlockPathEdge_end() = ESMoL::Output::Cast( outp );
							}
							catch ( udm_exception & ue )
							{
								ofstream out( "cycles.log", ios::app );
								out << "Attempt to create path edge in subsystem " << subsys.name() << "failed." << std::endl;
							}
						}
					}
				}
			}
		}

		// outline the "create a connectivity graph for a path edge" algorithm
		//   for each newly created path edge -
		//      create a subgraph for the compgraph
		//      get the start and end vertices
		//		recursively build the subgraph from the last vertex
		PathEdgeSet bpedges = subsys.BlockPathEdge_children();
		// DEBUG std::ofstream out( "cycles.log", ios::app );
		// out << "Subsys: " << subsys.name() << " pathedges " << bpedges.size() << std::endl;
		idx_t count = 0;
		for ( PathEdgeSet::iterator ibpe = bpedges.begin(); ibpe != bpedges.end(); ibpe++ )
		{
			idx_t bpidx = (*ibpe).uniqueId();
			CompGraph & subcg(cg->create_subgraph());

			ESMoL::Input inp = (*ibpe).srcBlockPathEdge_end();
			ESMoL::Output outp = (*ibpe).dstBlockPathEdge_end();

			CGVertex inpv = vertextable[ inp.uniqueId() ];
			CGVertex outpv = vertextable[ outp.uniqueId() ];

			CGUtils::CGResetColor( *cg );

			// Recursively walk backwards from the output and build the path
			// TODO Figure out what this is doing wrong and fix it (maybe)
			// DEBUG buildPathEdgeSubgraph( outpv, inpv, *cg, tcg, subcg );

			//_pathedge_subgraphs[bpidx] = &subcg;

			// DEBUG out << count++ << " : v " << num_vertices( subcg )
			//	<< " e " << num_edges( subcg ) << std::endl;
		
			/* DEBUG 
			{ // Dump graph files to see what's happening
				string pname = (*cg)[inpv].path;
				size_t pos = pname.find( '/' );
				while ( pos != string::npos )
				{
					pname.replace(pos,1,"_");
					pos = pname.find('/');
				}

				ostringstream fname;
				fname << pname << (*ibpe).uniqueId() << ".dot";
				ofstream pfile( fname.str().c_str() );
				write_graphviz( pfile, subcg, CGLabelWriter(subcg), CGLabelWriter(subcg) );
			} */
		}

		
	}

	void createPathEdges( ESMoL::Component & comp, 	std::map< idx_t, idx_t > & vertextable )
	{
		
	}

	void clearModFlags( ESMoL::Subsystem & subsys )
	{
		subsys.SubsystemModified() = false;

		SubsystemSet subsysChildren = subsys.Subsystem_kind_children();
		for ( SubsystemSet::iterator ssIter = subsysChildren.begin(); ssIter != subsysChildren.end(); ssIter++ )
		{
			clearModFlags( *ssIter );
			(*ssIter).SubsystemModified() = false;
		}
	}

	void clearModFlags( ESMoL::Component & comp );

	void setModFlags( ESMoL::Subsystem & subsys )
	{
		subsys.SubsystemModified() = true;

		SubsystemSet subsysChildren = subsys.Subsystem_kind_children();
		for ( SubsystemSet::iterator ssIter = subsysChildren.begin(); ssIter != subsysChildren.end(); ssIter++ )
		{
			(*ssIter).SubsystemModified() = true;
		}
	}

	unsigned long numComponents() { return _comp_count; }
	unsigned long numComponentProcessed() { return _comp_processed_count; }
	unsigned long numElementsProcessed() { return _elements_processed_count; }
	unsigned long numAbstractCycles() { return _abstract_cycles; }
	unsigned long numTotalCycles() { return _total_cycles; }

protected:

	CompGraph & buildLocalGraph( ESMoL::Subsystem & subsys, std::map< idx_t, idx_t > & vertextable ) 
	{
		// First, get the current path
		std::string curPath = getCurPath();

		CompGraph * cg;
		// get the graph object that we need to use - if it's not there then make one
		CompGraphMap::iterator cgIter = _compgraphs.find( subsys.uniqueId() );
		if ( cgIter != _compgraphs.end() )
		{
			// if we found one, use it!
			cg = cgIter->second;
		}
		else
		{
			// if not, make one and save it
			cg = new CompGraph();
			_compgraphs[ subsys.uniqueId() ] = cg;
		}

		// we need a local map from block ids to vertex ids
		std::set< idx_t > inputlist;  // keep track of subsystem input ports
		std::set< idx_t > outputlist; // keep track of subsystem output ports

		// add vertices from input ports
		InputPortSet inputs = subsys.InputPort_kind_children();
		_elements_processed_count += inputs.size();
		for ( InputPortSet::iterator inpIter = inputs.begin(); inpIter != inputs.end(); inpIter++ )
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
		OutputPortSet outputs = subsys.OutputPort_kind_children();
		_elements_processed_count += outputs.size();
		for ( OutputPortSet::iterator outpIter = outputs.begin(); outpIter != outputs.end(); outpIter++ )
		{
			CGVertex cgv = add_vertex(*cg);
			vertextable[ (*outpIter).uniqueId() ] = cgv; // put it in the vertex map

			// what do we need to keep in the vertex?
			//   path string, UDM object pointer?, what else?
			(*cg)[cgv].path = curPath + string( (*outpIter).name() );
			(*cg)[cgv].primtype = outport;

			(*cg)[cgv].udmobj = Udm::Object::Cast( (*outpIter) );
		}

		// add vertices from primitives
		PrimitiveSet prims = subsys.Primitive_kind_children();
		_elements_processed_count += prims.size();
		for ( PrimitiveSet::iterator primIter = prims.begin(); primIter != prims.end(); primIter++ )
		{
			CGVertex cgv = add_vertex(*cg);
			vertextable[ (*primIter).uniqueId() ] = cgv; // put it in the vertex map

			// what do we need to keep in the vertex?
			//   path string, UDM object pointer?, what else?
			(*cg)[cgv].path = curPath + string( (*primIter).name() );

			if ( string((*primIter).BlockType()) == string( "UnitDelay" ) )
			{
				(*cg)[cgv].primtype = delay;
			}
			else
			{
				(*cg)[cgv].primtype = prim;
			}
			
			(*cg)[cgv].udmobj = Udm::Object::Cast( (*primIter) );
		}

		// add vertices from the ports of subsystem children
		SubsystemSet subsysChildren = subsys.Subsystem_kind_children();
		_elements_processed_count += subsysChildren.size();
		for ( SubsystemSet::iterator subsysIter = subsysChildren.begin(); subsysIter != subsysChildren.end(); subsysIter++ )
		{
			InputPortSet inports = (*subsysIter).InputPort_kind_children();
			for ( InputPortSet::iterator inpIter = inports.begin(); inpIter != inports.end(); inpIter++ )
			{
				CGVertex cgv = add_vertex(*cg);
				vertextable[ (*inpIter).uniqueId() ] = cgv; // put it in the vertex map

				(*cg)[cgv].path = curPath + std::string("/") 
					+ std::string((*subsysIter).name()) + std::string("/") + std::string((*inpIter).name());
				(*cg)[cgv].primtype = subsys_inport;
				(*cg)[cgv].udmobj = Udm::Object::Cast( *inpIter );
			}

			OutputPortSet outports = (*subsysIter).OutputPort_kind_children();
			for ( OutputPortSet::iterator outpIter = outports.begin(); outpIter != outports.end(); outpIter++ )
			{
				CGVertex cgv = add_vertex(*cg);
				vertextable[ (*outpIter).uniqueId() ] = cgv; // put it in the vertex map

				(*cg)[cgv].path = curPath + std::string("/") 
					+ std::string((*subsysIter).name()) + std::string("/") + std::string((*outpIter).name());
				(*cg)[cgv].primtype = subsys_outport;
				(*cg)[cgv].udmobj = Udm::Object::Cast( *outpIter );
			}
		}

		// add edges corresponding to Lines
		LineSet lines = subsys.Line_kind_children();
		for ( LineSet::iterator lineIter = lines.begin(); lineIter != lines.end(); lineIter++ )
		{
			ESMoL::Port srcPort = (*lineIter).srcLine_end();
			ESMoL::Port dstPort = (*lineIter).dstLine_end();

			Udm::Object srcParent = srcPort.parent();
			Udm::Object dstParent = dstPort.parent();

			// Four cases - 1) primitive to primitive - connect the prim vertices directly with an edge
			if ( ( srcParent.type() == ESMoL::Primitive::meta ) 
				&& ( dstParent.type() == ESMoL::Primitive::meta ) )
			{
				CGVertex cgsrc = vertextable[ srcParent.uniqueId() ];
				CGVertex cgdst = vertextable[ dstParent.uniqueId() ];

				std::pair< CGEdge, bool > cge = add_edge( cgsrc, cgdst, *cg );

				(*cg)[cge.first].edgetype = connection;
				(*cg)[cge.first].udmobj   = Udm::Object::Cast( *lineIter );
			}

			// 2) primitive to subsystem - connect the prim vertex to the subsys port vertex
			//   NOTE: the subsystem parent could be the current subsystem -- the logic is still OK
			else if ( (srcParent.type() == ESMoL::Primitive::meta )
					&& ( dstParent.type() == ESMoL::Subsystem::meta ) )
			{
				CGVertex cgsrc = vertextable[ srcParent.uniqueId() ];
				CGVertex cgdst = vertextable[ dstPort.uniqueId() ];

				std::pair< CGEdge, bool > cge = add_edge( cgsrc, cgdst, *cg );

				(*cg)[cge.first].edgetype = connection;
				(*cg)[cge.first].udmobj   = Udm::Object::Cast( *lineIter );
			}

			// 3) subsystem to primitive - connect the subsys port vertex to the prim vertex
			//   NOTE: the subsystem parent could be the current subsystem
			else if ( (srcParent.type() == ESMoL::Subsystem::meta )
				&& ( dstParent.type() == ESMoL::Primitive::meta ) )
			{
				CGVertex cgsrc = vertextable[ srcPort.uniqueId() ];
				CGVertex cgdst = vertextable[ dstParent.uniqueId() ];

				std::pair< CGEdge, bool > cge = add_edge( cgsrc, cgdst, *cg );

				(*cg)[cge.first].edgetype = connection;
				(*cg)[cge.first].udmobj   = Udm::Object::Cast( *lineIter );
			}

			// 4) subsystem to subsystem - connect the subsys port vertex to the subsys port vertex
			//   NOTE: either subsystem parent could be the current subsystem
			else if ( (srcParent.type() == ESMoL::Subsystem::meta )
				&& ( dstParent.type() == ESMoL::Subsystem::meta ) )
			{
				CGVertex cgsrc = vertextable[ srcPort.uniqueId() ];
				CGVertex cgdst = vertextable[ dstPort.uniqueId() ];

				std::pair< CGEdge, bool > cge = add_edge( cgsrc, cgdst, *cg );

				(*cg)[cge.first].edgetype = connection;
				(*cg)[cge.first].udmobj   = Udm::Object::Cast( *lineIter );
			}
		}

		// add path edges from subsystem children
		for ( SubsystemSet::iterator subsysIter = subsysChildren.begin(); subsysIter != subsysChildren.end(); subsysIter++ )
		{
			PathEdgeSet pathedges = (*subsysIter).BlockPathEdge_kind_children();
			for ( PathEdgeSet::iterator edgeIter = pathedges.begin(); edgeIter != pathedges.end(); edgeIter++ )
			{
				// connect the endpoints
				ESMoL::Input inp = (*edgeIter).srcBlockPathEdge_end();
				ESMoL::Output outp = (*edgeIter).dstBlockPathEdge_end();

				ESMoL::InputPort inport = ESMoL::InputPort::Cast( inp );
				ESMoL::OutputPort outport = ESMoL::OutputPort::Cast( outp );
				
				CGVertex cgsrc = vertextable[ inport.uniqueId() ];
				CGVertex cgdst = vertextable[ outport.uniqueId() ];

				std::pair< CGEdge, bool > cge = add_edge( cgsrc, cgdst, *cg );

				(*cg)[cge.first].edgetype = pathedge;
				(*cg)[cge.first].udmobj   = Udm::Object::Cast( *edgeIter );
				//std::ofstream out( "cycles.log", ios::app );
				//out << "Associating object " << (*edgeIter).uniqueId() << " with pathedge." << std::endl;
			}
		}

		return *cg;
	}

	CompGraph & buildLocalGraph( ESMoL::Component & comp, std::map< idx_t, idx_t > & vertextable );

	std::string getCurPath() 
	{
		std::string collectPath;
		for ( PathList::iterator pathIter = _paths.begin(); pathIter != _paths.end(); pathIter++ )
		{
			collectPath += *pathIter + string("/");
		}
		return collectPath;
	}

	void printCycles( const Udm::Object & obj, cyclevector_t & cycles, CompGraph & cg, std::ostream & out )
	{
		for ( cyclevector_t::iterator icyc = cycles.begin(); icyc != cycles.end(); icyc++ )
		{
			printCycle( obj, *icyc, cg, out );
		}
	}
	void printCycle( const Udm::Object & obj, idxvector_t & cyc, CompGraph & cg, std::ostream & out );
	void printLocalGraph( CompGraph & cg );
	void buildPathEdgeSubgraph( const CGVertex & outpv, const CGVertex & inpv, CompGraph & cg, CompGraph & tcg, CompGraph & subcg );
	bool extractFullCycle( idxvector_t & cycle, CompGraph & cg, CompGraph & newcg );
	void getConnectedPaths( Udm::Object & obj, const CGVertex & startv, const CGVertex & endv, CompGraph & cg, CompGraph & newcg );
	void rebuildConnGraph( const CGVertex & bpstartv, const CGVertex & bpcurv, CompGraph & bp, 
							ESMoL::Input & inp, Udm::Object & curp, ESMoL::Subsystem & subsys, 
							std::string pathname, idxsetmap_t & comp_vertices, idxmap_t & vertextable );
	void getPathName( const std::string & vpath, std::string & pathname );

	CycleMap _cycles;
	PathList _paths;
	CompGraphMap _compgraphs;
	CompGraphMap _pathedge_subgraphs;

	unsigned long _comp_count;
	unsigned long _comp_processed_count;
	unsigned long _elements_processed_count;
	unsigned long _abstract_cycles;
	unsigned long _total_cycles;
};


#endif // HierCycles_h__