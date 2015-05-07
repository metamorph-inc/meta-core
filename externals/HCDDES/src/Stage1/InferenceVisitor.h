#ifndef __INFERENCE_VISITOR_H__
#define __INFERENCE_VISITOR_H__


/*** Included Header Files ***/
#include <map>
#include <set>
#include <stack>
#include <list>
#include "ESMoL.h"
#include "utils.h"
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/subgraph.hpp>
#include <boost/graph/graphviz.hpp>


/***************************************************************************************/


// Define structure for Vertex information
struct VertexStruct{
	std::string			name;
	std::string			type;
	std::string			node;
};

// Define structure for Edge information
struct EdgeStruct{
	std::string			name;
	std::string			transmissionType;
};

typedef boost::property< boost::edge_index_t, int, EdgeStruct > EdgeProps;
typedef boost::property< boost::graph_name_t, std::string > GraphProps;

// Define graph types
typedef boost::subgraph< boost::adjacency_list< boost::vecS, boost::vecS, boost::directedS, VertexStruct, EdgeProps, GraphProps > > DataFlowGraph;
typedef boost::graph_traits< DataFlowGraph >::vertex_descriptor VertexDesc;
typedef boost::graph_traits< DataFlowGraph >::vertex_iterator VertexIter;
typedef boost::graph_traits< DataFlowGraph >::edge_descriptor EdgeDesc;
typedef boost::graph_traits< DataFlowGraph >::edge_iterator EdgeIter;


typedef std::map< std::string, VertexDesc > InferenceReturn;
typedef std::map< ESMoL::OutputPort, VertexDesc > BlockReturn;


/***************************************************************************************/


class InferenceVisitor {
private:
	unsigned long							_numFailures;
	std::list< DataFlowGraph >				_graphList;
	std::map< std::string, VertexDesc >		_vertexMap;
	std::map< std::string, EdgeDesc >		_edgeMap;
	std::stack< std::string >				_edgeTypeStack;
	std::string								_currentNode;
public:
	InferenceVisitor( const ESMoL::RootFolder &target );
	virtual ~InferenceVisitor();

	void Visit_HardwareUnit( const ESMoL::HardwareUnit &hardwareUnit );
	void Visit_IODevice( const ESMoL::IODevice &device, const ESMoL::MsgPort &msgPort, VertexDesc refVertex );
	void Visit_ComponentRef( const ESMoL::ComponentRef &componentRef, const ESMoL::Node &node, const ESMoL::MsgPort &msgPort, VertexDesc vertex );
	InferenceReturn Visit_CCode( const ESMoL::CCode &cCode, const ESMoL::Node &node, const ESMoL::MsgPort &msgPort, VertexDesc refVertex );
	InferenceReturn Visit_Subsystem( const ESMoL::Subsystem &subsystem, const ESMoL::Node &node, const ESMoL::MsgPort &msgPort, VertexDesc refVertex );
	BlockReturn Visit_Block( const ESMoL::Block &block, const std::string &path, const ESMoL::InputPort &inputPort, VertexDesc refVertex );
	BlockReturn Visit_SubsystemBlock( const ESMoL::Block &block, const std::string &path, const ESMoL::InputPort &inputPort, VertexDesc refVertex, const bool &root );
	BlockReturn Visit_MuxBlock( const ESMoL::Block &block, const std::string &path, const ESMoL::InputPort &inputPort, VertexDesc refVertex );
	BlockReturn Visit_DemuxBlock( const ESMoL::Block &block, const std::string &path, const ESMoL::InputPort &inputPort, VertexDesc refVertex );
	InferenceReturn Visit_PetriNet( const ESMoL::PetriNet &petriNet, const ESMoL::Node &node, const ESMoL::MsgPort &msgPort, VertexDesc refVertex );
};



// Bunch of Typedefs
typedef std::set< ESMoL::DesignFolder >		DesignFolderSet;
typedef std::set< ESMoL::HardwareUnit >		HardwareUnitSet;
typedef std::set< ESMoL::IODevice >			IODeviceSet;
typedef std::set< ESMoL::AcquisitionConnection > AcqConnSet;
typedef std::set< ESMoL::ActuationConnection > ActConnSet;
typedef std::set< ESMoL::CommMapping >		CommMappingSet;
typedef std::set< ESMoL::MessageRef >		MessageRefSet;
typedef std::set< ESMoL::Message >			MessageSet;
typedef std::set< ESMoL::MsgPort >			MsgPortSet;
typedef std::set< ESMoL::IChan >			IChanSet;
typedef std::set< ESMoL::OChan >			OChanSet;
typedef std::set< ESMoL::ComponentRef >		ComponentRefSet;
typedef std::set< ESMoL::ComponentBase >	ComponentBaseSet;
typedef std::set< ESMoL::ComponentAssignment > ComponentAssignmentSet;
typedef std::set< ESMoL::Subsystem >		SubsystemSet;
typedef std::set< ESMoL::PetriNet >			PetriNetSet;
typedef std::set< ESMoL::CCode >			CCodeSet;
typedef std::set< ESMoL::COutputPort >		COutputPortSet;
typedef std::set< ESMoL::OutputPort >		OutputPortSet;
typedef std::set< ESMoL::InputPort >		InputPortSet;
typedef std::set<ESMoL::Connector>			ConnectorSet;
typedef std::set< ESMoL::Dependency >		DependencySet;
typedef std::set< ESMoL::Line >				LineSet;


/***************************************************************************************/


#endif // __INFERENCE_VISITOR_H__

