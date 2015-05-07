#ifndef __VISITOR_H__
#define __VISITOR_H__


/*** Included Header Files ***/

#include <map>
#include <set>

#include "CreateTemplate.h"

#include "BIP.h"
#include "ESMoL.h"
#include "utils.h"

/***************************************************************************************/

class BIPVisitor : public virtual BIP::Visitor 
{
public:
	BIPVisitor( BIP::RootFolder & brf, TimeStamp & ts );
	virtual ~BIPVisitor() { }

	virtual void Visit_Module( const BIP::Module & mod );
	virtual void Visit_PetriNet( const BIP::PetriNet & pn );

	virtual void Visit_Variable( const BIP::Variable & v );
	virtual void Visit_State( const BIP::State & st );

	virtual void Visit_Port( const BIP::Port & p );
	virtual void Visit_VariableReference( const BIP::VariableReference & vr );

	virtual void Visit_Transition( const BIP::Transition & t );
	virtual void Visit_TransitionToState( const BIP::TransitionToState & tts );
	virtual void Visit_StateToTransition( const BIP::StateToTransition & stt );

	virtual void Visit_Connector( const BIP::Connector & conn );

protected:

	void HandleExterns( const BIP::Module & mod );

	ContextTracker							_contextTracker;
	PNTemplateBuilder *						_tb;
	std::map< std::string, std::string > *	_varMap;
	std::map< std::string, bool > *			_stateMap; // initial state
	std::map< std::string, bool > *			_transMap; // direction of port
	std::map< std::string, std::vector< pair< std::string, std::string > > > * _transVars; // variables
	TimeStamp								_ts;
	
};

/***************************************************************************************


// We can use the same traversal for both passes, so we pushed it up to a superclass.
class AbstractVisitor : public virtual ESMoL::Visitor {
private:
	bool GetBus(ESMoL::BChan bchan, std::set< ESMoL::BChan > &nodesVisited, ESMoL::TTBus &bus);

protected:
	ContextTracker							_contextTracker;
	std::string								_contextName;
	Semantics::RootFolder					_rootFolder;
	Semantics::Deployment					_currentDeployment;

	std::string idToString( const char * prefix, Udm::Object::uniqueId_type id );

public:
	AbstractVisitor( Semantics::RootFolder &target ) : _rootFolder(target) { }
	virtual ~AbstractVisitor()				{ }

	// In a command-line interpreter this is where the "visit" starts.
	virtual void Visit_RootFolder(const ESMoL::RootFolder &rootFolder );
	// In a GME-embedded interpreter, the deployment model could be the focus object being interpreted.
	
	virtual void Visit_TimingSheet(const ESMoL::TimingSheet & timingSheet );

	virtual void Visit_System(const ESMoL::System &);

	virtual void Visit_NodeRef(const ESMoL::NodeRef &)				{ }
	virtual void Visit_Node(const ESMoL::Node &)                    { }
	virtual void Visit_OS(const ESMoL::OS &)                        { }

	virtual void Visit_BChan(const ESMoL::BChan &)					{ }
	virtual void Visit_TTBus(const ESMoL::TTBus &)					{ }
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

};
*/

/***************************************************************************************


class ConstraintVisitor : public virtual AbstractVisitor {
private:
	unsigned long							_numFailures;
	//ESMoL::Task								_curTask;
	ESMoL::Component						_curComponent;

public:
	ConstraintVisitor( Semantics::RootFolder &target );
	virtual ~ConstraintVisitor();

	virtual void Visit_System(const ESMoL::System &);

	//virtual void Visit_Deployment(const ESMoL::Deployment &deployment );
	//virtual void Visit_TimingSheet(const ESMoL::TimingSheet& );
	virtual void Visit_NodeRef(const ESMoL::NodeRef &nodeRef );
	//virtual void Visit_Task(const ESMoL::Task &task);
	//virtual void Visit_TTTask(const ESMoL::TTTask& );
	//virtual void Visit_TTMessage(const ESMoL::TTMessage& );
	virtual void Visit_MessageRef(const ESMoL::MessageRef &messageRef );
	virtual void Visit_Component(const ESMoL::Component &component );
	virtual void Visit_CInputPort(const ESMoL::CInputPort& );
	virtual void Visit_COutputPort(const ESMoL::COutputPort& );
	virtual void Visit_OutputPort(const ESMoL::OutputPort& );
	virtual void Visit_InputPort(const ESMoL::InputPort& );
	//virtual void Visit_Router(const ESMoL::Router& );
	//virtual void Visit_IChan(const ESMoL::IChan& );
	//virtual void Visit_OChan(const ESMoL::OChan& );
	//virtual void Visit_BChan(const ESMoL::BChan& );
	//virtual void Visit_ChannelRef(const ESMoL::ChannelRef &channelRef );
	//virtual void Visit_TTBus(const ESMoL::TTBus& );
	//virtual void Visit_MsgAssignment(const ESMoL::MsgAssignment& );
	unsigned long getNumFailures()			{ return _numFailures; }

};

*/
/***************************************************************************************


class ElementVisitor : public virtual AbstractVisitor {
private:
	
	Semantics::Component					_curSComp;

	ESMoL::ComponentRef						_curECompRef;
	ESMoL::MessageRef						_curEMsgRef;

	bool									_inputDevice;
	double									_hyperperiodSecs;
	unsigned long long						_portNum;
	unsigned long long						_msgNum;

	std::map< std::string, Semantics::SignalType > _typesVisited;

	void AddToFileList( std::string &fileList, const std::string &element );
public:
	ElementVisitor( Semantics::RootFolder &target );
	virtual ~ElementVisitor()				{ }

	virtual void Visit_TimingSheet(const ESMoL::TimingSheet &timingSheet ) { }

	virtual void Visit_System(const ESMoL::System &);

	virtual void Visit_NodeRef(const ESMoL::NodeRef &nodeRef );
	virtual void Visit_Node(const ESMoL::Node &);
	virtual void Visit_OS(const ESMoL::OS &);

	virtual void Visit_BChan(const ESMoL::BChan &);
	virtual void Visit_TTBus(const ESMoL::TTBus &);
	virtual void Visit_IChan(const ESMoL::IChan &);
	virtual void Visit_OChan(const ESMoL::OChan &);
	virtual void Visit_IODevice(const ESMoL::IODevice &);
	virtual void Visit_Plant(const ESMoL::Plant &);

	virtual void Visit_ComponentRef(const ESMoL::ComponentRef &);
	virtual void Visit_Component(const ESMoL::Component &component );
	virtual void Visit_MessageRef(const ESMoL::MessageRef &messageRef );
	virtual void Visit_Message(const ESMoL::Message &);
	virtual void Visit_MsgPort(const ESMoL::MsgPort &);
	virtual void Visit_CCode(const ESMoL::CCode&);
	virtual void Visit_CInputPort(const ESMoL::CInputPort &cInputPort );
	virtual void Visit_COutputPort(const ESMoL::COutputPort &cOutputPort );
	virtual void Visit_SubsystemRef(const ESMoL::SubsystemRef &);
	virtual void Visit_Subsystem(const ESMoL::Subsystem &);
	virtual void Visit_OutputPort(const ESMoL::OutputPort &outputPort );
	virtual void Visit_InputPort(const ESMoL::InputPort &inputPort );
	virtual void Visit_Connector(const ESMoL::Connector &);
	
	virtual void Visit_Dependency(const ESMoL::Dependency &);

	virtual void Visit_CommMapping(const ESMoL::CommMapping &);

	virtual void Visit_ComponentAssignment(const ESMoL::ComponentAssignment &)     { }

	virtual void Visit_ExecutionAssignment(const ESMoL::ExecutionAssignment &);
	virtual void Visit_TTExecInfo(const ESMoL::TTExecInfo &);
	virtual void Visit_AsyncPeriodicExecInfo(const ESMoL::AsyncPeriodicExecInfo &);
	virtual void Visit_SporadicExecInfo(const ESMoL::SporadicExecInfo &);
	virtual void Visit_TTExecContext(const ESMoL::TTExecContext &)                 { }

};

*/
/***************************************************************************************


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

	const Semantics::Task GetTaskFromCompRef( const ESMoL::ComponentRef & cref );

public:
	RelationVisitor( Semantics::RootFolder &target );
	virtual ~RelationVisitor()				{ }

	virtual void Visit_TimingSheet(const ESMoL::TimingSheet &timingSheet ) { }

	virtual void Visit_System(const ESMoL::System &);

	virtual void Visit_NodeRef(const ESMoL::NodeRef &nodeRef ) { }
	virtual void Visit_Node(const ESMoL::Node &);
	virtual void Visit_OS(const ESMoL::OS &) { }

	virtual void Visit_BChan(const ESMoL::BChan &);
	virtual void Visit_TTBus(const ESMoL::TTBus &);
	virtual void Visit_IChan(const ESMoL::IChan &);
	virtual void Visit_OChan(const ESMoL::OChan &);
	virtual void Visit_IODevice(const ESMoL::IODevice &);
	virtual void Visit_Plant(const ESMoL::Plant &);

	virtual void Visit_ComponentRef(const ESMoL::ComponentRef &);
	virtual void Visit_Component(const ESMoL::Component & component );
	virtual void Visit_MessageRef(const ESMoL::MessageRef & messageRef );
	virtual void Visit_Message(const ESMoL::Message &);
	virtual void Visit_MsgPort(const ESMoL::MsgPort &);
	virtual void Visit_CCode(const ESMoL::CCode&);
	virtual void Visit_CInputPort(const ESMoL::CInputPort & cInputPort );
	virtual void Visit_COutputPort(const ESMoL::COutputPort & cOutputPort );
	virtual void Visit_SubsystemRef(const ESMoL::SubsystemRef &);
	virtual void Visit_Subsystem(const ESMoL::Subsystem &);
	virtual void Visit_OutputPort(const ESMoL::OutputPort & outputPort );
	virtual void Visit_InputPort(const ESMoL::InputPort & inputPort );
	virtual void Visit_Connector(const ESMoL::Connector &);
	
	virtual void Visit_ComponentAssignment( const ESMoL::ComponentAssignment & );
	virtual void Visit_Dependency(const ESMoL::Dependency &);
	virtual void Visit_CommMapping(const ESMoL::CommMapping &);
	virtual void Visit_ExecutionAssignment(const ESMoL::ExecutionAssignment & );

	virtual void Visit_TTExecInfo(const ESMoL::TTExecInfo & );
	virtual void Visit_AsyncPeriodicExecInfo(const ESMoL::AsyncPeriodicExecInfo & ) { }
	virtual void Visit_SporadicExecInfo(const ESMoL::SporadicExecInfo & )           { }
	virtual void Visit_TTExecContext(const ESMoL::TTExecContext & )                 { }

};


***************************************************************************************/


#endif//__VISITOR_H__

