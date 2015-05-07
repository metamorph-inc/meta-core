#ifndef __VISITOR_H__
#define __VISITOR_H__


/*** Included Header Files ***/
#include <map>
#include <set>
#include "ESMoL.h"
#include "Semantics.h"
#include "IndexMerge.h"
#include "utils.h"


/***************************************************************************************/

enum CONN_DIR {INPUT_CONN, OUTPUT_CONN, NO_CONN};

// We can use the same traversal for both passes, so we pushed it up to a superclass.
class AbstractVisitor : public virtual ESMoL::Visitor {
private:

	bool GetBus( ESMoL::BChan bchan, std::set< ESMoL::BChan > & nodesVisited, ESMoL::Bus & bus);

protected:
	ContextTracker							_contextTracker;
	std::string								_contextName;
	Semantics::RootFolder					_rootFolder;
	Semantics::Deployment					_currentDeployment;

	std::map< ESMoL::MessageRef, ESMoL::ComponentRef >	_msgRefsLeft;
	std::map< ESMoL::MsgPort, ESMoL::MessageRef > _msgPortsLeft;
	std::map< ESMoL::MessageRef, CONN_DIR > _msgDirection; 

	std::string idToString( const char * prefix, Udm::Object::uniqueId_type id );

	// Utility functions for posting error and warning messages using a consistent format.
	// The output already includes the model context path, so the message string only needs
	//  to contain error details.
	virtual void postError( std::string & msg );
	virtual void postWarning( std::string & msg );

	void GetNodes( ESMoL::HardwareUnit hwunit, std::set< ESMoL::Node > & collectedNodes );

public:
	AbstractVisitor( Semantics::RootFolder & target ) : _rootFolder(target) { }
	virtual ~AbstractVisitor()				{ }

	// In a command-line interpreter this is where the "visit" starts.
	virtual void Visit_RootFolder(const ESMoL::RootFolder &rootFolder );
	// In a GME-embedded interpreter, the deployment model could be the focus object being interpreted.
	
	virtual void Visit_TimingSheet(const ESMoL::TimingSheet & timingSheet );
	virtual void Visit_HardwareUnit(const ESMoL::HardwareUnit &)    { }
	virtual void Visit_System(const ESMoL::System &);
	virtual void Visit_NodeRef(const ESMoL::NodeRef &)				{ }
	virtual void Visit_Node(const ESMoL::Node &)                    { }
	virtual void Visit_OS(const ESMoL::OS &)                        { }
	virtual void Visit_Full_Dependency_Graph(const ESMoL::Node &)   { }
	virtual void Visit_BChan(const ESMoL::BChan &)					{ }
	virtual void Visit_TTBus(const ESMoL::TTBus &)					{ }
	virtual void Visit_CANBus(const ESMoL::CANBus &)				{ }
	virtual void Visit_SerialLink( const ESMoL::SerialLink &)		{ }
	virtual void Visit_IChan(const ESMoL::IChan &)					{ }
	virtual void Visit_OChan(const ESMoL::OChan &)					{ }
	virtual void Visit_IODevice(const ESMoL::IODevice &)            { }
	virtual void Visit_Plant(const ESMoL::Plant &)                  { }
	virtual void Visit_ComponentRef(const ESMoL::ComponentRef &)    { }
	virtual void Visit_Component(const ESMoL::Component &)			{ }
	virtual void Visit_MessageRef(const ESMoL::MessageRef &)        { }
	virtual void Visit_Message(const ESMoL::Message &)              { }
	virtual void Visit_MsgPort(const ESMoL::MsgPort &)              { }
	virtual void Visit_CCode(const ESMoL::CCode&)                   { }
	virtual void Visit_CInputPort(const ESMoL::CInputPort &)		{ }
	virtual void Visit_COutputPort(const ESMoL::COutputPort &)		{ }
	virtual void Visit_SubsystemRef(const ESMoL::SubsystemRef &)    { }
	virtual void Visit_OutputPort(const ESMoL::OutputPort &)		{ }
	virtual void Visit_InputPort(const ESMoL::InputPort &)			{ }
	virtual void Visit_Connector(const ESMoL::Connector &)          { }
	virtual void Visit_Dependency(const ESMoL::Dependency &)        { }
	virtual void Visit_CommMapping(const ESMoL::CommMapping &)      { }
	virtual void Visit_ComponentAssignment(const ESMoL::ComponentAssignment &)      { }
	virtual void Visit_ExecutionAssignment(const ESMoL::ExecutionAssignment &)		{ }
	virtual void Visit_TTExecInfo(const ESMoL::TTExecInfo &)						{ }
	virtual void Visit_AsyncPeriodicExecInfo(const ESMoL::AsyncPeriodicExecInfo &)	{ }
	virtual void Visit_SporadicExecInfo(const ESMoL::SporadicExecInfo &)			{ }
	virtual void Visit_TTExecContext(const ESMoL::TTExecContext &)					{ }
	virtual void Visit_Unused_MsgRefs() { }
	virtual void Visit_Unused_MsgPorts() { }
};


/***************************************************************************************/


class ConstraintVisitor : public virtual AbstractVisitor {
private:
	unsigned long							_numFailures;
	//ESMoL::Task								_curTask;
	ESMoL::Component						_curComponent;
	ESMoL::ComponentRef                     _curECompRef;
	ESMoL::Node								_curENode;

	std::map< std::string, ID_TYPE >  _msg_type_names;
	std::map< std::string, ID_TYPE >  _msg_port_names;
	std::map< std::string, ID_TYPE >  _comp_type_names;
	std::map< std::string, ID_TYPE >  _comp_inst_names;
	std::map< std::string, ID_TYPE >  _msg_inst_names;
	std::map< std::string, ID_TYPE >  _node_names;
	std::map< std::string, ID_TYPE >  _ichan_names;
	std::map< std::string, ID_TYPE >  _ochan_names;
	std::map< std::string, ID_TYPE >  _bchan_names;
	std::map< std::string, ID_TYPE >  _bus_names;
	std::map< std::string, ID_TYPE >  _exec_info_names;

	std::set< ID_TYPE > _visited_input_ports;

	std::set< std::string > _reserved_names;
	std::string print_reserved_names();

	std::string MakeQualifiedName( const std::string & par, const std::string & child );
	bool CheckDuplicateName( const std::string & name, ID_TYPE id, std::map< std::string, ID_TYPE > & names );

	virtual void postError( std::string & msg );

public:
	ConstraintVisitor( Semantics::RootFolder &target );
	virtual ~ConstraintVisitor();

	virtual void Visit_System( const ESMoL::System & sys );
	//virtual void Visit_Deployment(const ESMoL::Deployment &deployment );
	//virtual void Visit_TimingSheet(const ESMoL::TimingSheet& );
	virtual void Visit_NodeRef( const ESMoL::NodeRef & nodeRef );
	virtual void Visit_Node( const ESMoL::Node & node );
	virtual void Visit_BChan(const ESMoL::BChan & );
	virtual void Visit_TTBus(const ESMoL::TTBus & );
	virtual void Visit_CANBus(const ESMoL::CANBus & );
	virtual void Visit_SerialLink( const ESMoL::SerialLink &);
	virtual void Visit_IChan(const ESMoL::IChan & );
	virtual void Visit_OChan(const ESMoL::OChan & );
	virtual void Visit_SubsystemRef( const ESMoL::SubsystemRef & subsysref );
	virtual void Visit_MessageRef( const ESMoL::MessageRef & messageRef );
	virtual void Visit_Message(const ESMoL::Message & );
	virtual void Visit_MsgPort(const ESMoL::MsgPort & );
	virtual void Visit_Component( const ESMoL::Component & component );
	virtual void Visit_Dependency( const ESMoL::Dependency & dep );
	virtual void Visit_CommMapping(const ESMoL::CommMapping & cmap );
	virtual void Visit_ComponentRef( const ESMoL::ComponentRef & compref );
	virtual void Visit_CInputPort( const ESMoL::CInputPort & );
	virtual void Visit_COutputPort( const ESMoL::COutputPort & );
	virtual void Visit_OutputPort( const ESMoL::OutputPort & );
	virtual void Visit_InputPort( const ESMoL::InputPort & );
	virtual void Visit_TTExecInfo(const ESMoL::TTExecInfo &);
	virtual void Visit_AsyncPeriodicExecInfo(const ESMoL::AsyncPeriodicExecInfo &);
	virtual void Visit_SporadicExecInfo(const ESMoL::SporadicExecInfo &);
	virtual void Visit_Unused_MsgRefs();
	virtual void Visit_Unused_MsgPorts();
	unsigned long getNumFailures()			{ return _numFailures; }
};


/***************************************************************************************/


class ElementVisitor : public virtual AbstractVisitor {
private:
	
	Semantics::Component					_curSComp;
	Semantics::Node							_curSNode;
	ESMoL::ComponentRef						_curECompRef;
	ESMoL::MessageRef						_curEMsgRef;
	bool									_inputDevice;
	double									_hyperperiodSecs;
	unsigned long long						_portNum;
	unsigned long long						_msgNum;
	unsigned long long						_devNum;
	std::map< std::string, Semantics::SignalType > _typesVisited;
	void AddToFileList( std::string &fileList, const std::string &element );
	void HandleOSEKExecInfo( ESMoL::ExecutionInfo & execinfo, Semantics::ExecInfo & sei );

public:
	ElementVisitor( Semantics::RootFolder &target );
	virtual ~ElementVisitor()				{ }

	virtual void Visit_TimingSheet(const ESMoL::TimingSheet &timingSheet ) { }
	virtual void Visit_HardwareUnit( const ESMoL::HardwareUnit & hwunit );
	virtual void Visit_System(const ESMoL::System & );
	virtual void Visit_NodeRef(const ESMoL::NodeRef & nodeRef );
	virtual void Visit_Node(const ESMoL::Node & );
	virtual void Visit_OS(const ESMoL::OS & );
	virtual void Visit_BChan(const ESMoL::BChan & );
	virtual void Visit_TTBus(const ESMoL::TTBus & );
	virtual void Visit_CANBus( const ESMoL::CANBus & );
	virtual void Visit_SerialLink( const ESMoL::SerialLink &);
	virtual void Visit_IChan(const ESMoL::IChan & );
	virtual void Visit_OChan(const ESMoL::OChan & );
	virtual void Visit_IODevice(const ESMoL::IODevice & );
	virtual void Visit_Plant(const ESMoL::Plant & );
	virtual void Visit_ComponentRef(const ESMoL::ComponentRef & );
	virtual void Visit_Component(const ESMoL::Component & component );
	virtual void Visit_MessageRef(const ESMoL::MessageRef & messageRef );
	virtual void Visit_Message(const ESMoL::Message & );
	virtual void Visit_MsgPort(const ESMoL::MsgPort & );
	virtual void Visit_CCode(const ESMoL::CCode & );
	virtual void Visit_CInputPort(const ESMoL::CInputPort & cInputPort );
	virtual void Visit_COutputPort(const ESMoL::COutputPort & cOutputPort );
	virtual void Visit_SubsystemRef(const ESMoL::SubsystemRef & );
	virtual void Visit_Subsystem(const ESMoL::Subsystem & );
	virtual void Visit_OutputPort(const ESMoL::OutputPort & outputPort );
	virtual void Visit_InputPort(const ESMoL::InputPort & inputPort );
	virtual void Visit_Connector(const ESMoL::Connector & );
	virtual void Visit_Dependency(const ESMoL::Dependency & );
	virtual void Visit_CommMapping(const ESMoL::CommMapping & );
	virtual void Visit_ComponentAssignment(const ESMoL::ComponentAssignment & )     { }
	virtual void Visit_ExecutionAssignment(const ESMoL::ExecutionAssignment & );
	virtual void Visit_TTExecInfo(const ESMoL::TTExecInfo & );
	virtual void Visit_AsyncPeriodicExecInfo(const ESMoL::AsyncPeriodicExecInfo & );
	virtual void Visit_SporadicExecInfo(const ESMoL::SporadicExecInfo & );
	virtual void Visit_TTExecContext(const ESMoL::TTExecContext & )                 { }
	virtual void Visit_Unused_MsgRefs();
};


/***************************************************************************************/


class RelationVisitor : public virtual AbstractVisitor {

private:
	Semantics::Node							_curSNode;
	ESMoL::Node								_curENode;
	Semantics::Task							_curSTask;
	ESMoL::ComponentRef						_curECompRef;
	Semantics::Msg							_curSMsg;
	Semantics::Component					_curSComp;
	Semantics::BChan						_curSBChan;
	Semantics::CommInterface				_curSCommIface;
	Semantics::IChan						_curSIChan;
	Semantics::OChan						_curSOChan;
	Semantics::InputDevice					_curSIDev;
	Semantics::OutputDevice					_curSODev;
	std::set< std::pair< unsigned long long, unsigned long long > > _uniqueMapping;
	std::map< unsigned long long, Semantics::CommRelations > _keepCommRels;
	// LocalDependency id -> ( msgIndex -> Signal )
	std::map< unsigned long long, std::map< unsigned long long , Semantics::Signal > > _sigMap;
	const Semantics::Task GetTaskFromCompRef( const ESMoL::ComponentRef & cref );

public:
	RelationVisitor( Semantics::RootFolder &target );
	virtual ~RelationVisitor()				{ }

	virtual void Visit_TimingSheet(const ESMoL::TimingSheet &timingSheet ) { }
	virtual void Visit_System(const ESMoL::System & );
	virtual void Visit_NodeRef(const ESMoL::NodeRef & nodeRef ) { }
	virtual void Visit_Node(const ESMoL::Node & );
	virtual void Visit_OS(const ESMoL::OS & ) { }
	virtual void Visit_Full_Dependency_Graph(const ESMoL::Node & );
	virtual void Visit_BChan(const ESMoL::BChan & );
	virtual void Visit_TTBus(const ESMoL::TTBus & );
	virtual void Visit_CANBus(const ESMoL::CANBus & );
	virtual void Visit_SerialLink( const ESMoL::SerialLink &);
	virtual void Visit_IChan(const ESMoL::IChan & );
	virtual void Visit_OChan(const ESMoL::OChan & );
	virtual void Visit_IODevice(const ESMoL::IODevice & );
	virtual void Visit_Plant(const ESMoL::Plant & );
	virtual void Visit_ComponentRef(const ESMoL::ComponentRef & );
	virtual void Visit_Component(const ESMoL::Component & component );
	virtual void Visit_MessageRef(const ESMoL::MessageRef & messageRef );
	virtual void Visit_Message(const ESMoL::Message & );
	virtual void Visit_MsgPort(const ESMoL::MsgPort & );
	virtual void Visit_CCode(const ESMoL::CCode & );
	virtual void Visit_CInputPort(const ESMoL::CInputPort & cInputPort );
	virtual void Visit_COutputPort(const ESMoL::COutputPort & cOutputPort );
	virtual void Visit_SubsystemRef(const ESMoL::SubsystemRef & );
	virtual void Visit_Subsystem(const ESMoL::Subsystem & );
	virtual void Visit_OutputPort(const ESMoL::OutputPort & outputPort );
	virtual void Visit_InputPort(const ESMoL::InputPort & inputPort );
	virtual void Visit_Connector(const ESMoL::Connector & );
	virtual void Visit_ComponentAssignment( const ESMoL::ComponentAssignment & );
	virtual void Visit_Dependency(const ESMoL::Dependency & );
	virtual void Visit_CommMapping(const ESMoL::CommMapping & );
	virtual void Visit_ExecutionAssignment(const ESMoL::ExecutionAssignment & );
	virtual void Visit_TTExecInfo(const ESMoL::TTExecInfo & );
	virtual void Visit_AsyncPeriodicExecInfo(const ESMoL::AsyncPeriodicExecInfo & ) { }
	virtual void Visit_SporadicExecInfo(const ESMoL::SporadicExecInfo & )           { }
	virtual void Visit_TTExecContext(const ESMoL::TTExecContext & )                 { }
};


/***************************************************************************************/


#endif//__VISITOR_H__

