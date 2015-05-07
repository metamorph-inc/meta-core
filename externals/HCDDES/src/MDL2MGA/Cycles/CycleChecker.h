#ifndef CycleChecker_h__
#define CycleChecker_h__

#include "CycleGraphs.h"

// Paper title: Integrated incremental cycle checking for ESMoL models -> submit to Models or SoSym
// Cycle checking is important for component-based systems, but it requires a scalable method for 
// large designs.  We use the hierarchical abstraction already present in the models to speed up 
// existing cycle checking algorithms, rendering them compositional.

// Create an appropriate boost::graph structure
//     Edges should have an integer value that indicates the number of delay tokens in that link
// Create an appropriate tree structure for the component hierarchy, and populate it via DFS
// Start at the leaves -- 
//     Process each leaf for cycles: build a data flow graph and identify delays.  Then do Johnson's cycle finder.
//     Process each leaf for abstraction, function: component -> (port graph with connections for feed-forward paths, map between port objects and port graph indices)

// Move up the tree --
//     Process each component for cycles: build a data flow graph with the abstracted leaf components, identifying delays.  Do Johnson's cycle finder.
//	   Process each component for abstraction, function: (component, port map for all leaves)->(port graph, updated port map)

// At the top -- where can we store this information?  The class interface needs a query function
// Add method for checking any software component type in the SysTypes specification - based on multiple Simulink components
// Add method for checking the full logical architecture and issuing warnings (e.g. cycle found in data flow -- broken by hardware dependency)
//   for this one, that's a first cut because we may want to do a more detailed SDF analysis (a la Tripakis)

#include <string>
#include <map>
#include <set>
#include <iostream>
#include <stack>
#include <vector>

#include "ESMoL.h"

typedef std::set< ESMoL::Subsystem >  SubsystemSet;
typedef std::set< ESMoL::Primitive >  PrimitiveSet;
typedef std::set< ESMoL::InputPort >  InputPortSet;
typedef std::set< ESMoL::OutputPort > OutputPortSet;
typedef std::set< ESMoL::Line >		  LineSet;
typedef std::set< ESMoL::BlockPathEdge >  PathEdgeSet;

class CycleFoundException {
public:
	std::string msg;
};


class CycleChecker
{
public:
	CycleChecker( ESMoL::Subsystem & subsys ) {

		try {

			// think about the ordering carefully -- we need to make sure the right abstraction edges
			// exist before we try to do the cycle checking.  If we do both recursively, we may have to
			// do all of the abstractions first, and then all of the cycle checks.  That's not possible,
			// we have a double-dependency --

			//    

			SubsystemSet subsystems = subsys.Subsystem_kind_children();
			if ( subsystems.size() == 0 )
			{
				ProcessLeafComponent( subsys );
			}
			else
			{
				ProcessBranchComponent( subsys );
			}
		}
		catch ( CycleFoundException & cfe ) {
			
			std::cerr << "Found cycle: " << cfe.msg << std::endl;
		}
	}
	virtual ~CycleChecker() { }

	void BuildCompGraph( ESMoL::Subsystem & subsys, CompGraph & cg, portmap_t & vertex_map )
	{
		// Build dataflow graph for this component
		// Run Johnson on it to get simple cycles
		InputPortSet inports = subsys.InputPort_kind_children();
		OutputPortSet outports = subsys.OutputPort_kind_children();
		PrimitiveSet prims = subsys.Primitive_kind_children();
		LineSet		lines = subsys.Line_kind_children();

		port_edges_t record_edges;

		for ( InputPortSet::iterator inpIter = inports.begin(); inpIter != inports.end(); inpIter++ )
		{
			CGVertex cgv = add_vertex( cg );
			vertex_map[ (*inpIter).uniqueId() ] = get( boost::vertex_index, cg, cgv );
		}

		for ( OutputPortSet::iterator outpIter = outports.begin(); outpIter != outports.end(); outpIter++ )
		{
			CGVertex cgv = add_vertex( cg );
			vertex_map[ (*outpIter).uniqueId() ] = get( boost::vertex_index, cg, cgv );
		}

		for ( PrimitiveSet::iterator primIter = prims.begin(); primIter != prims.end(); primIter++ )
		{
			CGVertex cgv = add_vertex( cg );
			vertex_map[ (*primIter).uniqueId() ] = get( boost::vertex_index, cg, cgv );
		}

		for ( LineSet::iterator lineIter = lines.begin(); lineIter != lines.end(); lineIter++ )
		{
			// navigate -- if we hit an input/output port whose parent is subsys, connect it directly
			//   if not, use the primitive parent of the port for the connection
			CGVertex vertex_src, vertex_dst;
			uid_t id_src, id_dst;
			ESMoL::Port line_src = (*lineIter).srcLine_end();
			ESMoL::Block parent_block_src = ESMoL::Block::Cast( line_src.parent() );
			if ( parent_block_src.uniqueId() == subsys.uniqueId() ) // it's a port at this level
			{
				id_src = line_src.uniqueId();
				vertex_src = vertex_map[ line_src.uniqueId() ];
			}
			else // it's a port of a primitive child
			{
				id_src = parent_block_src.uniqueId();
				vertex_src = vertex_map[ parent_block_src.uniqueId() ];
			}
			ESMoL::Port line_dst = (*lineIter).dstLine_end();
			ESMoL::Block parent_block_dst = ESMoL::Block::Cast( line_dst.parent() );
			if ( parent_block_dst.uniqueId() == subsys.uniqueId() ) // it's a port at this level
			{
				id_dst = line_dst.uniqueId();
				vertex_dst = vertex_map[ line_dst.uniqueId() ];
			}
			else // it's a port of a primitive child
			{
				id_dst = parent_block_dst.uniqueId();
				vertex_dst = vertex_map[ parent_block_dst.uniqueId() ];
			}

			std::pair< CGEdge, bool > cge = add_edge( vertex_src, vertex_dst, cg );
			record_edges[id_src].insert( id_dst );
		}

		comp_graphs[subsys.uniqueId()] = record_edges;
	}


	bool LeafComponentCheckCycles( ESMoL::Subsystem & subsys ) 
	{
		bool bCycleFree = true;
		
		CompGraph cg;
		portmap_t vertex_map;

		BuildCompGraph( subsys, cg, vertex_map );


		// Now run Johnson on the resulting graph, and throw cycle messages if we find some
		// TODO: get some Johnson!  First look online and see if anyone has done it for boost::graph
		//FindCycles( cg );

		return bCycleFree;
	}

	bool BranchComponentCheckCycles( ESMoL::Subsystem & subsys ) 
	{
		bool bCycleFree = true;

		CompGraph cg;
		portmap_t vertex_map;

		BuildCompGraph( subsys, cg, vertex_map );

		// Also add edges from the abstractions between the ports of child subsystems
		SubsystemSet child_subsystems;
		for ( SubsystemSet::iterator childIter = child_subsystems.begin(); childIter != child_subsystems.end(); childIter++ )
		{
			port_edges_t gedges = comp_graphs[ (*childIter).uniqueId() ];
			InputPortSet inputs = (*childIter).InputPort_kind_children();
			OutputPortSet outputs = (*childIter).OutputPort_kind_children();

			// this is an n^2 operation, but the sets are small
			for ( InputPortSet::iterator inpIter = inputs.begin(); inpIter != inputs.end(); inpIter++ )
			{
				for ( OutputPortSet::iterator outpIter = outputs.begin(); outpIter != outputs.end(); outpIter++ )
				{
					if ( gedges[ (*inpIter).uniqueId() ].find( (*outpIter).uniqueId() ) != gedges[ (*inpIter).uniqueId() ].end() )
					{
						//CGVertex inp_vertex = 
						//add_edge( );
					}
				}
			}
		}

		//FindCycles( cg );

		return bCycleFree;
	}

	void ProcessLeafComponent( ESMoL::Subsystem & subsys ) 
	{
		// it's OK to check cycles here, because we have no deeper abstractions
		LeafComponentCheckCycles( subsys );
		//LeafComponentAbstraction( subsys );
	}

	void ProcessBranchComponent( ESMoL::Subsystem & subsys ) 
	{
		// First process all child components, then do the branch component
		SubsystemSet child_subsystems = subsys.Subsystem_kind_children();
		for ( SubsystemSet::iterator subIter = child_subsystems.begin(); subIter != child_subsystems.end(); subIter++ )
		{
			SubsystemSet subsystems = (*subIter).Subsystem_kind_children();
			if ( subsystems.size() == 0 )
			{
				ProcessLeafComponent( *subIter );
			}
			else
			{
				ProcessBranchComponent( *subIter );
			}
		}
	}

protected:
	portmap_t input_ports;
	portmap_t output_ports;
	PortGraph port_connections;
	compmap_t comp_graphs;
};




#endif // CycleChecker_h__