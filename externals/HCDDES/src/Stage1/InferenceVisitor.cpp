/*** Included Header Files ***/
#include "InferenceVisitor.h"
#include "ConfigKeeper.h"
#include <boost/graph/graphviz.hpp>


/***************************************************************************************/


class LabelWriter {
private:
	DataFlowGraph _graph;
public:
	LabelWriter( DataFlowGraph &graph )	: _graph(graph) {}
	void operator()( std::ostream &out, const EdgeDesc &edge) const {
		// Have to make sure there are no \n in the name
		std::string name = this->_graph[edge].name;
		std::string transType = this->_graph[edge].transmissionType;
		while ( name.find("\n") != std::string::npos ) name.erase( name.find("\n") );
		// Now build the output
		out << "[";
		if ( transType == "subsystem" )		out << "color=red";
		else if ( transType == "local" )	out << "color=black";
		else if ( transType == "io" )		out << "color=green";
		else if ( transType == "network" )	out << "color=blue";
		else out << "color=yellow";
		out << "]";
	}
	void operator()( std::ostream &out, const VertexDesc &vertex) const {
		// Have to make sure there are no \n in the name
		std::string name = this->_graph[vertex].name;
		std::string type = this->_graph[vertex].type;
		while ( name.find("\n") != std::string::npos ) name.erase( name.find("\n") );
		// Now build the output
		out << "[label=\"" << name << "\"";
		if ( type == "IODevice" ) out << ", color=lawngreen, shape=circle";
		else if ( type == "CCode" ) out << ", color=blue, shape=component";
		else if ( type == "PetriNet" ) out << ", color=green, shape=component";
		// Otherwise, we are into the dataflow blocks
		else {
			// Set the color to yellow
			out << ", color=red";
			// Set shape based on block type
			if ( type == "Gain" )			out << ", shape=invtriangle";
			if ( type == "Sum" )			out << ", shape=box";
			if ( type == "ZeroOrderHold" )	out << ", shape=box3d";
			if ( type == "UnitDelay" )		out << ", shape=note";
			if ( type == "EmbeddedMatlab" )	out << ", shape=component";
		}
		// Close the bracket and be done with it
		out << "]";
	}
};



InferenceVisitor::InferenceVisitor( const ESMoL::RootFolder &target ) : _numFailures(0), _graphList() {
	std::string targetName = target.name();
	std::cout << "Checking model type inference..." << std::endl;
	// Place the root graph object onto the list
	this->_graphList.push_back( DataFlowGraph() );
	// Set the graph name
	boost::get_property( this->_graphList.front(), boost::graph_name ) = targetName;
	// Visit the DesignFolders - to build the graph
	DesignFolderSet designFolderSet = target.DesignFolder_children();
	DesignFolderSet::iterator designFolderIter = designFolderSet.begin();
	for( ; designFolderIter!= designFolderSet.end(); designFolderIter++) {
		// Visit all HardwareUnits
		HardwareUnitSet hardwareSet = designFolderIter->HardwareUnit_children();
		HardwareUnitSet::iterator hardwareIter = hardwareSet.begin();
		for( ; hardwareIter != hardwareSet.end(); hardwareIter++ ) {
			// Visit the unit
			this->Visit_HardwareUnit( *hardwareIter );
		}
	}

	// Now color the graph with using type information
	// ...

	// Now generate a visualization of the graph (GraphViz format) - if debug is true
	if ( ConfigKeeper::Inst().GenerateGraph() ) {
		ofstream graphOutput;
		std::string filename = targetName + ".dot";
		graphOutput.open( filename.c_str() );
		write_graphviz( graphOutput, this->_graphList.front(), LabelWriter( this->_graphList.front() ), LabelWriter( this->_graphList.front() ) );
		graphOutput.close();
		// Execute GraphViz on the file
//		std::string execString = "dot -Tsvg " + filename + " -o " + targetName + "_dfg.svg";
//		system( execString.c_str() );
		// Finally, delete the intermediate .dot file
//		remove( filename.c_str() );
	}
}


InferenceVisitor::~InferenceVisitor() {
	// Nothing for now...
}


void InferenceVisitor::Visit_HardwareUnit( const ESMoL::HardwareUnit &hardwareUnit ) {
	std::string unitName = hardwareUnit.name();
	DEBUGOUT( "\tHardware Unit: " << unitName << std::endl )
	// Visit all IODevices
	IODeviceSet deviceSet = hardwareUnit.IODevice_kind_children();
	IODeviceSet::iterator deviceIter = deviceSet.begin();
	for ( ; deviceIter != deviceSet.end(); deviceIter++ ) {
		// Only visit devices with iChan connections
		AcqConnSet acqConnSet = deviceIter->dstAcquisitionConnection();
		if ( !acqConnSet.empty() ) {
			// Visit the IO Device
			ESMoL::MsgPort nullMsgPort = ESMoL::MsgPort::Cast( Udm::null );
			this->Visit_IODevice( *deviceIter, nullMsgPort, -1 );
		}
	}
}


// Only IODevices with iChan connections should be visited here
void InferenceVisitor::Visit_IODevice( const ESMoL::IODevice &device, const ESMoL::MsgPort &msgPort, VertexDesc refVertex ) {
	// Is this a input device
	if ( msgPort == ESMoL::MsgPort::Cast( Udm::null ) ) {
		std::string deviceName = device.name();
		DEBUGOUT( "\tIODevice: " << deviceName << std::endl )
		// Add vertex into graph for IO device
		VertexDesc vertex = add_vertex( this->_graphList.front() );
		// Add to the subgraph as appropriate
		if ( this->_graphList.size() > 1 ) add_vertex( vertex, this->_graphList.back() );
		this->_graphList.front()[vertex].name = deviceName;
		this->_graphList.front()[vertex].type = "IODevice";
		// Add the vertex into the global map
		this->_vertexMap.insert( std::make_pair( deviceName, vertex ) );
		// Navigate towards the connected blocks
		AcqConnSet acqConnSet = device.dstAcquisitionConnection();
		AcqConnSet::iterator acqConnIter = acqConnSet.begin();
		// Go through all Acquistions Connections
		for ( ; acqConnIter != acqConnSet.end(); acqConnIter++ ) {
			ESMoL::IChan iChan = acqConnIter->dstAcquisitionConnection_end();
			// Get the attached commMappings
			CommMappingSet commSet = iChan.dstCommMapping();
			CommMappingSet::iterator commIter = commSet.begin();
			for ( ; commIter != commSet.end(); commIter++ ) { 
				// Some hoops to jump through since CommMapping does not have direction
				ESMoL::CommMapping_Members_Base srcCommBase = commIter->srcCommMapping_end();
				ESMoL::CommMapping_Members_Base dstCommBase = commIter->dstCommMapping_end();
				ESMoL::MessageRef messageRef;
				ESMoL::ComponentRef componentRef;
				// Make sure we are getting the receiving end
				if ( srcCommBase.type() == ESMoL::MessageRef::meta ) {
					messageRef = ESMoL::MessageRef::Cast( srcCommBase );
					componentRef = ESMoL::ComponentRef::Cast(commIter->srcCommMapping__rp_helper());
				}
				else {
					messageRef = ESMoL::MessageRef::Cast( dstCommBase );
					componentRef = ESMoL::ComponentRef::Cast(commIter->dstCommMapping__rp_helper());
				}
				ESMoL::Node node = ESMoL::Node::Cast( iChan.parent() );
				// Get the actual message type from the reference
				ESMoL::MessageRef_RefersTo_Base messageRefBase = messageRef.ref();
				ESMoL::Message message = ESMoL::Message::Cast( messageRefBase );
				// Now visit the compRef for each port in the message
				MsgPortSet msgPortSet = message.MsgPort_children();
				MsgPortSet::iterator msgPortIter = msgPortSet.begin();
				for ( ; msgPortIter != msgPortSet.end(); msgPortIter++ ) {
					// Push the edge type onto the stack
					this->_edgeTypeStack.push( "io" );
					// Visit the componentRef via the message port
					this->Visit_ComponentRef( componentRef, node, *msgPortIter, vertex );
					// Clear edge type
					this->_edgeTypeStack.pop();
				}
			}
		}
	}
	// Must be an output device
	else {
		std::string deviceName = device.name();
		VertexDesc vertex;
		// Have we been here before
		std::map< std::string, VertexDesc >::iterator vertIter = this->_vertexMap.find( deviceName );
		if ( vertIter == this->_vertexMap.end() ) {
			DEBUGOUT( "\tIODevice: " << deviceName << std::endl )
			// Create a vertex
			vertex = add_vertex( this->_graphList.front() );
			// Add to the subgraph as appropriate
			if ( this->_graphList.size() > 1 ) add_vertex( vertex, this->_graphList.back() );
			this->_graphList.front()[vertex].name = deviceName;
			this->_graphList.front()[vertex].type = "IODevice";
			// Insert vertex into vertex map
			this->_vertexMap.insert( std::make_pair( deviceName, vertex ) );
		}
		// We have been here before
		else {
			DEBUGOUT( "\tIODevice Revisit: " << deviceName << std::endl )
			vertex = vertIter->second;
		}
		// Build an edge between the two vertices
		std::pair< EdgeDesc, bool > edgeResult = add_edge( refVertex, vertex, this->_graphList.front() );
		this->_graphList.front()[edgeResult.first].name = std::string( msgPort.name() );
		this->_graphList.front()[edgeResult.first].transmissionType = this->_edgeTypeStack.top();
	}
}


void InferenceVisitor::Visit_ComponentRef( const ESMoL::ComponentRef &componentRef, const ESMoL::Node &node, const ESMoL::MsgPort &msgPort, VertexDesc refVertex ) {
	// What type of component is this
	ESMoL::ComponentRef_RefersTo_Base compRefBase = componentRef.ref();
	ESMoL::Component component = ESMoL::Component::Cast( compRefBase );
	ComponentBaseSet compBaseSet = component.ComponentBase_children();
	ComponentBaseSet::iterator compBaseIter = compBaseSet.begin();
	for ( ; compBaseIter != compBaseSet.end(); compBaseIter++ ) {
		InferenceReturn retVal;
		// Is it a CCode block
		if ( compBaseIter->type() == ESMoL::CCode::meta ) {
			// Cast to a CCode and visit
			ESMoL::CCode cCode = ESMoL::CCode::Cast( *compBaseIter );
			retVal = this->Visit_CCode( cCode, node, msgPort, refVertex );
		}
		// Is it a PetriNet block
		else if ( compBaseIter->type() == ESMoL::PetriNet::meta ) {
			// Cast to a PetriNet and visit
			ESMoL::PetriNet petriNet = ESMoL::PetriNet::Cast( *compBaseIter );
			retVal = this->Visit_PetriNet( petriNet, node, msgPort, refVertex );
		}
		// It is a Subsystem Ref
		else if ( compBaseIter->type() == ESMoL::SubsystemRef::meta ) {
			// Cast to a SubsystemRef and visit
			ESMoL::SubsystemRef subsysRef = ESMoL::SubsystemRef::Cast( *compBaseIter );
			ESMoL::Subsystem subsystem = subsysRef.ref();
			retVal = this->Visit_Subsystem( subsystem, node, msgPort, refVertex );
		}

		// If there are new connections to explore
		if ( !retVal.empty() ) {
			// Let's start with Dependencies
			ESMoL::System parentSys = componentRef.System_parent();
			// Get all dependencies
			DependencySet dependencySet = parentSys.Dependency_children();
			DependencySet::iterator dependencyIter = dependencySet.begin();
			for ( ; dependencyIter != dependencySet.end(); dependencyIter++ ) {
				// Get the sending side
				ESMoL::ComponentRef srcCompRef = dependencyIter->srcDependency__rp_helper();
				ESMoL::MessageRef messageRef = dependencyIter->srcDependency_end();
				// Is this message ref on this component
				if ( componentRef == srcCompRef ) {
					// Get the actual message type from the reference
					ESMoL::MessageRef_RefersTo_Base messageRefBase = messageRef.ref();
					ESMoL::Message message = ESMoL::Message::Cast( messageRefBase );
					// Get the ports in the message
					MsgPortSet msgPortSet = message.MsgPort_children();
					MsgPortSet::iterator msgPortIter = msgPortSet.begin();
					// Go through all message ports
					for ( ; msgPortIter != msgPortSet.end(); msgPortIter++ ) {
						// Build the name of the port
						std::string fullname = std::string( messageRef.name() ) + "." + std::string( msgPortIter->name() );
						// Find the name in the map
						InferenceReturn::iterator mapIter = retVal.find( fullname );
						// If the port is in the map of return values
						if ( mapIter != retVal.end() ) {
							std::cout << "\t - Dependency: " << fullname << std::endl;
							// Get the ComponentRef at the other end of the dependency
							ESMoL::ComponentRef dstCompRef = dependencyIter->dstDependency__rp_helper();
							// Get the node this componentRef is in
							ComponentAssignmentSet compAssignSet = dstCompRef.dstComponentAssignment();
							ESMoL::NodeRef dstNodeRef = compAssignSet.begin()->dstComponentAssignment_end();
							ESMoL::Node dstNode = dstNodeRef.ref();
							// Set the edge type in the stack based on the two nodes
							if ( dstNode == node ) this->_edgeTypeStack.push( "local" );
							else this->_edgeTypeStack.push( "network" );
							// Visit the componentRef
							this->Visit_ComponentRef( dstCompRef, dstNode, *msgPortIter, mapIter->second );
							// Pop the edge type
							this->_edgeTypeStack.pop();
						}
					}
				}
			}

			// Moving on to CommMappings
			CommMappingSet commMappingSet = parentSys.CommMapping_children();
			CommMappingSet::iterator commMappingIter = commMappingSet.begin();
			for ( ; commMappingIter != commMappingSet.end(); commMappingIter++ ) {
				// Get both sides of the commMapping
				//ESMoL::ComponentRef srcCompRef = ESMoL::ComponentRef::Cast(commMappingIter->srcCommMapping__rp_helper());
				//ESMoL::ComponentRef dstCompRef = ESMoL::ComponentRef::Cast(commMappingIter->dstCommMapping__rp_helper());
				ESMoL::CommMapping_srcCommMapping_RPContainer_Base cmscmrpcb = commMappingIter->srcCommMapping__rp_helper();
				ESMoL::CommMapping_dstCommMapping_RPContainer_Base cmdcmrpcb = commMappingIter->dstCommMapping__rp_helper();
				// Is this message ref on this component
				//if ( componentRef == srcCompRef || componentRef == dstCompRef ) {
				if ( componentRef.uniqueId() == cmscmrpcb.uniqueId() || componentRef.uniqueId() == cmdcmrpcb.uniqueId() ) {
					// Some hoops to jump through since CommMapping does not have direction
					ESMoL::CommMapping_Members_Base srcCommBase = commMappingIter->srcCommMapping_end();
					ESMoL::CommMapping_Members_Base dstCommBase = commMappingIter->dstCommMapping_end();
					ESMoL::MessageRef messageRef;
					ESMoL::CommMapping_Members_Base chanCommBase;
					// Make sure we are getting the receiving end
					if ( srcCommBase.type() == ESMoL::MessageRef::meta ) {
						messageRef = ESMoL::MessageRef::Cast( srcCommBase );
						chanCommBase = dstCommBase;
					}
					else {
						messageRef = ESMoL::MessageRef::Cast( dstCommBase );
						chanCommBase = srcCommBase;
					}
					// Get the actual message type from the reference
					ESMoL::MessageRef_RefersTo_Base messageRefBase = messageRef.ref();
					ESMoL::Message message = ESMoL::Message::Cast( messageRefBase );
					// Get the ports in the message
					MsgPortSet msgPortSet = message.MsgPort_children();
					MsgPortSet::iterator msgPortIter = msgPortSet.begin();
					for ( ; msgPortIter != msgPortSet.end(); msgPortIter++ ) {
						// Build the name of the port
						std::string fullname = std::string( messageRef.name() ) + "." + std::string( msgPortIter->name() );
						// Find the name in the map
						InferenceReturn::iterator mapIter = retVal.find( fullname );
						// If the port is in the map of return values
						if ( mapIter != retVal.end() ) {
							// Make sure it is an OChan - can't be a BChan or IChan
							if ( chanCommBase.type() == ESMoL::OChan::meta ) {
								std::cout << "\t - CommMapping: " << fullname << std::endl;
								ESMoL::OChan oChan = ESMoL::OChan::Cast( chanCommBase );
								// Get the destination IODevice
								ActConnSet actConnSet = oChan.dstActuationConnection();
								ActConnSet::iterator actConnIter = actConnSet.begin();
								ESMoL::IODevice device = actConnIter->dstActuationConnection_end();
								// Set the edge type
								this->_edgeTypeStack.push( "io" );
								// Visit the IODevice
								this->Visit_IODevice( device, *msgPortIter, mapIter->second );
								// Pop the edge type
								this->_edgeTypeStack.pop();
							}
						}
					}
				}
			}
		}
	}
}


InferenceReturn
InferenceVisitor::Visit_CCode( const ESMoL::CCode &cCode, const ESMoL::Node &node, const ESMoL::MsgPort &msgPort, VertexDesc refVertex ) {
	bool newFlag = true;
	std::string nodeName = node.name();
	std::string cCodeName = nodeName + "." + std::string( cCode.name() );
	// See if this CCode is already registered in map
	VertexDesc vertex;
	std::map< std::string, VertexDesc >::iterator vertIter = this->_vertexMap.find( cCodeName );
	if ( vertIter == this->_vertexMap.end() ) {
		DEBUGOUT( "\tCCode: " << cCodeName << std::endl )
		// Create a vertex
		vertex = add_vertex( this->_graphList.back() );
		// Add to the subgraph as appropriate
		if ( this->_graphList.size() > 1 ) add_vertex( vertex, this->_graphList.back() );
		this->_graphList.front()[vertex].name = cCodeName;
		this->_graphList.front()[vertex].type = "CCode";
		// Insert vertex into vertex map
		this->_vertexMap.insert( std::make_pair( cCodeName, vertex ) );
	}
	// Otherwise, grab the vertex
	else {
		DEBUGOUT( "\tCCode Revisit: " << cCodeName << std::endl )
		vertex = vertIter->second;
		// Set the new flag to false
		newFlag = false;
	}
	// Build an edge between the two vertices
	std::pair< EdgeDesc, bool > edgeResult = add_edge( refVertex, vertex, this->_graphList.front() );
	this->_graphList.front()[edgeResult.first].name = std::string( msgPort.name() );
	this->_graphList.front()[edgeResult.first].transmissionType = this->_edgeTypeStack.top();

	//If not new, return with no paths to follow
	if ( !newFlag ) return InferenceReturn();
	InferenceReturn retVal;
	// Determine the outbound message ports
	COutputPortSet outputPortSet = cCode.COutputPort_children();
	COutputPortSet::iterator outputPortIter = outputPortSet.begin();
	for ( ; outputPortIter != outputPortSet.end(); outputPortIter++ ) {
		// Visit each output port and see what it is connected to
		ConnectorSet connectorSet = outputPortIter->dstConnector();
		ConnectorSet::iterator connectorIter = connectorSet.begin();
		for ( ; connectorIter != connectorSet.end(); connectorIter++ ) {
			// Visit each connector and see what is on the other end
			ESMoL::Input input = connectorIter->dstConnector_end();
			// Try casting to a MsgPort ( it is the only type we care about for now )
			if ( input.type() == ESMoL::MsgPort::meta ) {
				// Cast to a MsgPort
				ESMoL::MsgPort msgPort = ESMoL::MsgPort::Cast( input );
				// What else?
				ESMoL::Message message = msgPort.Message_parent();
				ESMoL::MessageRef messageRef = ESMoL::MessageRef::Cast(connectorIter->dstConnector__rp_helper());
				std::string msgPortOutName = std::string( messageRef.name() ) + "." + std::string( msgPort.name() );
				std::cout << "\t\tOutput connection: " << msgPortOutName << std::endl;
				// Add to the return set
				retVal.insert( std::make_pair( msgPortOutName, vertex ) );
			}
		}
	}
	// Return the output ports
	return retVal;
}


InferenceReturn
InferenceVisitor::Visit_Subsystem( const ESMoL::Subsystem &subsystem, const ESMoL::Node &node, const ESMoL::MsgPort &msgPort, VertexDesc refVertex ) {
	bool newFlag = true;
	std::string nodeName = node.name();
	// Get the connectors from the msgPort
	ConnectorSet connectorSet = msgPort.dstConnector();
	ConnectorSet::iterator connectorIter = connectorSet.begin();
	// Find the input port that the msgPort is connected to
	InputPortSet inputPortSet = subsystem.InputPort_kind_children();
	BlockReturn retVal;
	for ( ; connectorIter != connectorSet.end(); connectorIter++ ) {
		ESMoL::Input dstInput = connectorIter->dstConnector_end();
		// Check all of the input
		InputPortSet::iterator inputPortIter = inputPortSet.begin();
		for ( ; inputPortIter != inputPortSet.end(); inputPortIter++ ) {
			// Are they the same...have we found the entry port into the subsystem?
			if ( dstInput == *inputPortIter) {
				// Push subgraph onto the list
				this->_graphList.push_back( this->_graphList.back().create_subgraph() );
				// Visit the subsystem as a generic block
				retVal = this->Visit_SubsystemBlock( subsystem, nodeName, *inputPortIter, refVertex, true );
				// Pop subgraph from the list
				this->_graphList.pop_back();
			}
		}
	}
	// If there are output ports
	InferenceReturn inferenceReturn;
	if ( retVal.size() > 0 ) {
		// Visit all return outputPorts
		BlockReturn::iterator blockIter = retVal.begin();
		for ( ; blockIter != retVal.end(); blockIter++ ) {
			// Get the outputPort
			ESMoL::OutputPort outputPort = blockIter->first;
			// Visit each output port and see what it is connected to
			ConnectorSet connectorSet = outputPort.dstConnector();
			ConnectorSet::iterator connectorIter = connectorSet.begin();
			for ( ; connectorIter != connectorSet.end(); connectorIter++ ) {
				// Visit each connector and see what is on the other end
				ESMoL::Input input = connectorIter->dstConnector_end();
				// Try casting to a MsgPort ( it should be the only type we care about long-term )
				if ( input.type() == ESMoL::MsgPort::meta ) {
					// Cast to a MsgPort
					ESMoL::MsgPort msgPort = ESMoL::MsgPort::Cast( input );
					// What else?
					ESMoL::Message message = msgPort.Message_parent();
					ESMoL::MessageRef messageRef = ESMoL::MessageRef::Cast(connectorIter->dstConnector__rp_helper());
					std::string msgPortOutName = std::string( messageRef.name() ) + "." + std::string( msgPort.name() );
					std::cout << "\t\tOutput connection: " << msgPortOutName << std::endl;
					// Add to the return set
					inferenceReturn.insert( std::make_pair( msgPortOutName, blockIter->second ) );
				}
			}
		}
	}
	// Return the output ports
	return inferenceReturn;
}


InferenceReturn
InferenceVisitor::Visit_PetriNet( const ESMoL::PetriNet &petriNet, const ESMoL::Node &node, const ESMoL::MsgPort &msgPort, VertexDesc refVertex ) {
	std::string nodeName = node.name();
	std::string petriNetName = nodeName + "." + std::string( petriNet.name() );
	DEBUGOUT( "\tPetriNet: " << petriNetName << std::endl )
	// Return dummy data for now
	InferenceReturn retVal;
	return retVal;
}


BlockReturn
InferenceVisitor::Visit_Block( const ESMoL::Block &block, const std::string &path, const ESMoL::InputPort &inputPort, VertexDesc refVertex ) {
	// Check to see if this is a subsystem block
	std::string blockType = block.BlockType();
	if ( blockType == "SubSystem" ) {
		// Push subgraph onto the list
		this->_graphList.push_back( this->_graphList.back().create_subgraph() );
		// Visit the subsystem block
		return this->Visit_SubsystemBlock( block, path, inputPort, refVertex, false );
		// Pop subgraph from the list
		this->_graphList.pop_back();
	}
/*	// Handle the Mux special case
	if ( blockType == "Mux" ) {
		// Visit the subsystem block
		return this->Visit_MuxBlock( block, path, inputPort, refVertex );
	}
	// Handle the Demux special case
	if ( blockType == "Demux" ) {
		// Visit the subsystem block
		return this->Visit_DemuxBlock( block, path, inputPort, refVertex );
	} */
	// Handle sink special cases
	if ( blockType == "Scope" || blockType == "Terminator" ) {
		// Nothing to do
		return BlockReturn();
	}

	// Otherwise handle it as an atomic block
	bool newFlag = true;
	std::string blockName = path + "." + std::string( block.Name() );
	// See if this block is already registered in map
	VertexDesc vertex;
	std::map< std::string, VertexDesc >::iterator vertIter = this->_vertexMap.find( blockName );
	if ( vertIter == this->_vertexMap.end() ) {
		DEBUGOUT( "\tBlock: " << blockName << "\t(T) = " << blockType << std::endl )
		// Create a vertex
		vertex = add_vertex( this->_graphList.front() );
		// Add to the subgraph as appropriate
		add_vertex( vertex, this->_graphList.back() );
		this->_graphList.front()[vertex].name = blockName;
		this->_graphList.front()[vertex].type = blockType;
		// Insert vertex into vertex map
		this->_vertexMap.insert( std::make_pair( blockName, vertex ) );
	}
	// Otherwise, grab the vertex
	else {
		DEBUGOUT( "\tBlock Revisit: " << blockName << std::endl )
		vertex = vertIter->second;
		// Set the new flag to false
		newFlag = false;
	}
	// Build an edge between the two vertices
	std::pair< EdgeDesc, bool > edgeResult = add_edge( refVertex, vertex, this->_graphList.front() );
	this->_graphList.front()[edgeResult.first].name = std::string( inputPort.name() );
	this->_graphList.front()[edgeResult.first].transmissionType = this->_edgeTypeStack.top();
	// If not new (revisit) return now
	if ( !newFlag ) return BlockReturn();

	// Visit all downstream blocks
	OutputPortSet outputPortSet = block.OutputPort_kind_children();
	OutputPortSet::iterator outputPortIter = outputPortSet.begin();
	BlockReturn outPorts;
	for ( ; outputPortIter != outputPortSet.end(); outputPortIter++ ) {
		// Get the lines we are attached to
		LineSet lineSet = outputPortIter->dstLine();
		LineSet::iterator lineIter = lineSet.begin();
		for ( ; lineIter != lineSet.end(); lineIter++ ) {
			// Get the input ports on the other end
			ESMoL::Port dstPort = lineIter->dstLine_end();
			// What type of port is this, and output port?
			if ( dstPort.type() == ESMoL::OutputPort::meta ) {
				// Cast to an outputPort
				ESMoL::OutputPort dstOutputPort = ESMoL::OutputPort::Cast( dstPort );
				outPorts.insert( std::make_pair( dstOutputPort, vertex ) );
			}
			// Let's try an input port
			else if ( dstPort.type() == ESMoL::InputPort::meta ) {
				// Cast to an inputPort
				ESMoL::InputPort dstInputPort = ESMoL::InputPort::Cast( dstPort );
				// What block is it connected to
				ESMoL::Block dstBlock = dstInputPort.Block_parent();
				// Set edge type
				this->_edgeTypeStack.push( "subsystem" );
				// Visit the block
				BlockReturn blockReturnSet = this->Visit_Block( dstBlock, path, dstInputPort, vertex );
				// Clear edge type
				this->_edgeTypeStack.pop();
				// Append the return set
				outPorts.insert( blockReturnSet.begin(), blockReturnSet.end() );
			}
			// Error condition
			else {
				std::cout << "*** Error - This block is connected to some funky stuff.\n";
			}
		}
	}
	// Otherwise, return the set of output ports
	return outPorts;
}


BlockReturn
InferenceVisitor::Visit_SubsystemBlock( const ESMoL::Block &block, const std::string &path, const ESMoL::InputPort &inputPort, VertexDesc refVertex, const bool &root ) {
	std::string subsystemName = block.Name();
	std::string fullpath = path + "." + subsystemName;
	// Set the name for the current subgraph
	boost::get_property( this->_graphList.back(), boost::graph_name ) = subsystemName;
	DEBUGOUT( "\tBlock: " << fullpath << "\t(T) = Subsystem\n" )
	BlockReturn portSet;
	// Get all lines the input port is connected to (internally)
	LineSet lineSet = inputPort.dstLine();
	LineSet::iterator lineIter = lineSet.begin();
	// Visit all of the internal blocks at the end of these lines
	for ( ; lineIter != lineSet.end(); lineIter++ ) {
		// Other end of the line
		ESMoL::Port dstPort = lineIter->dstLine_end();
		// Make sure this is an input port
		if ( dstPort.type() == ESMoL::InputPort::meta ) {
			// Cast the port
			ESMoL::InputPort dstInputPort = ESMoL::InputPort::Cast( dstPort );
			ESMoL::Block dstBlock = dstPort.Block_parent();
			// Visit the block
			BlockReturn blockReturnSet = this->Visit_Block( dstBlock, fullpath, dstInputPort, refVertex );
			// Append the return set to the current return set
			portSet.insert( blockReturnSet.begin(), blockReturnSet.end() );
		}
		// Some possible error handling
		else {
			std::cout << "*** Found a rogue port not of type InputPort: " << std::string( dstPort.name() ) << std::endl;
		}
	}

	// If this is a root subsystem
	if ( root ) {
		// Just return the current set of ports and vertices
		return portSet;
	}

	// Visit all downstream blocks
	OutputPortSet outputPortSet = block.OutputPort_kind_children();
	OutputPortSet::iterator outputPortIter = outputPortSet.begin();
	BlockReturn outPorts;
	for ( ; outputPortIter != outputPortSet.end(); outputPortIter++ ) {
		// Get the associated vertex for this port
		BlockReturn::iterator vertexIter = portSet.find( *outputPortIter );
		// Check no correlation condition
		if ( vertexIter != portSet.end() ) {
			// Get the associated vertex
			VertexDesc vertex = vertexIter->second;
			// Get the lines we are attached to
			LineSet lineSet = outputPortIter->dstLine();
			LineSet::iterator lineIter = lineSet.begin();
			for ( ; lineIter != lineSet.end(); lineIter++ ) {
				// Get the input ports on the other end
				ESMoL::Port dstPort = lineIter->dstLine_end();
				// What type of port is this, and output port?
				if ( dstPort.type() == ESMoL::OutputPort::meta ) {
					// Cast to an outputPort
					ESMoL::OutputPort dstOutputPort = ESMoL::OutputPort::Cast( dstPort );
					outPorts.insert( std::make_pair( dstOutputPort, vertex ) );
				}
				// Let's try an input port
				else if ( dstPort.type() == ESMoL::InputPort::meta ) {
					// Cast to an inputPort
					ESMoL::InputPort dstInputPort = ESMoL::InputPort::Cast( dstPort );
					// What block is it connected to
					ESMoL::Block dstBlock = dstInputPort.Block_parent();
					// Try setting edge type
					this->_edgeTypeStack.push( "subsystem" );
					// Visit the block
					BlockReturn blockReturnSet = this->Visit_Block( dstBlock, path, dstInputPort, vertex );
					// Clear the edge type
					this->_edgeTypeStack.pop();
					// Append the return set
					outPorts.insert( blockReturnSet.begin(), blockReturnSet.end() );
				}
				// Error condition
				else {
					std::cout << "*** Error - This block is connected to some funky stuff.\n";
				}
			}
		}
	}
	// Otherwise, return the set of output ports
	return outPorts;
}


BlockReturn InferenceVisitor::Visit_MuxBlock( const ESMoL::Block &block, const std::string &path, const ESMoL::InputPort &inputPort, VertexDesc refVertex ) {
	// Get the output port set
	OutputPortSet outputPortSet = block.OutputPort_kind_children();
	OutputPortSet::iterator outputPortIter = outputPortSet.begin();
	BlockReturn outPorts;
	// Visit all outputPort connections
	for ( ; outputPortIter != outputPortSet.end(); outputPortIter++ ) {
		// Get the lines we are attached to
		LineSet lineSet = outputPortIter->dstLine();
		LineSet::iterator lineIter = lineSet.begin();
		for ( ; lineIter != lineSet.end(); lineIter++ ) {
			// Get the input ports on the other end
			ESMoL::Port dstPort = lineIter->dstLine_end();
			// What type of port is this, and output port?
			if ( dstPort.type() == ESMoL::OutputPort::meta ) {
				// Cast to an outputPort
				ESMoL::OutputPort dstOutputPort = ESMoL::OutputPort::Cast( dstPort );
				outPorts.insert( std::make_pair( dstOutputPort, refVertex ) );
			}
			// Let's try an input port
			else if ( dstPort.type() == ESMoL::InputPort::meta ) {
				// Cast to an inputPort
				ESMoL::InputPort dstInputPort = ESMoL::InputPort::Cast( dstPort );
				// What block is it connected to
				ESMoL::Block dstBlock = dstInputPort.Block_parent();
				// Visit the block
				BlockReturn blockReturnSet = this->Visit_Block( dstBlock, path, dstInputPort, refVertex );
				// Append the return set
				outPorts.insert( blockReturnSet.begin(), blockReturnSet.end() );
			}
			// Error condition
			else {
				std::cout << "*** MuxError - This block is connected to some funky stuff.\n";
			}
		}
	}
	// Otherwise, return the set of output ports
	return outPorts;
}


BlockReturn InferenceVisitor::Visit_DemuxBlock( const ESMoL::Block &block, const std::string &path, const ESMoL::InputPort &inputPort, VertexDesc refVertex ) {
	std::cout << "*** Error: We do not yet handle demux blocks correctly.\n";
	std::string blockName = path + "." + std::string( block.Name() );
	DEBUGOUT( "\tBlock: " << blockName << "\t(T) = Demux\n" )
	return BlockReturn();
}


/***************************************************************************************/

