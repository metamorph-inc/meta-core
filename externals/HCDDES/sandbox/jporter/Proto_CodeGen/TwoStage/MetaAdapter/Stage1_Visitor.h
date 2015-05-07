#ifndef STAGE1_VISITOR_H
#define STAGE1_VISITOR_H

#include <map>
#include <deque>

#include "ESMoL.h"
#include "Semantics.h"

#include "utils.h"



// We can use the same traversal for both passes, so we pushed it up to a superclass.
class Stage1_AbstractVisitor : public ESMoL::Visitor
{
	public:

		Stage1_AbstractVisitor(Semantics::RootFolder & target);

		virtual ~Stage1_AbstractVisitor() { }

		virtual void Visit_RootFolder(const ESMoL::RootFolder &); //In a command-line interpreter this is where the "visit" starts.
		virtual void Visit_Deployment(const ESMoL::Deployment &); //In a GME-embedded interpreter, the deployment model could be 
														  //the focus object being interpreted.
		virtual void Visit_TimingSheet(const ESMoL::TimingSheet &);

		virtual void Visit_NodeRef(const ESMoL::NodeRef &) { }
		virtual void Visit_Task(const ESMoL::Task &) { }
		virtual void Visit_TTTask(const ESMoL::TTTask &) { }
		virtual void Visit_TTMessage(const ESMoL::TTMessage &) { }
		virtual void Visit_MessageRef(const ESMoL::MessageRef & ) { }
		virtual void Visit_Component(const ESMoL::Component &) { }
		virtual void Visit_CInputPort(const ESMoL::CInputPort&) { }
		virtual void Visit_COutputPort(const ESMoL::COutputPort&) { }
		virtual void Visit_OutputPort(const ESMoL::OutputPort&) { }
		virtual void Visit_InputPort(const ESMoL::InputPort&) { }
		virtual void Visit_Connector(const ESMoL::Connector &) { }
		virtual void Visit_Router(const ESMoL::Router&) { }
		virtual void Visit_IChan(const ESMoL::IChan &) { }
		virtual void Visit_OChan(const ESMoL::OChan &) { }
		virtual void Visit_BChan(const ESMoL::BChan &) { }
		virtual void Visit_TTBus(const ESMoL::TTBus &) { }
		virtual void Visit_ChannelRef(const ESMoL::ChannelRef &) { }

		virtual void Visit_TaskAssignment(const ESMoL::TaskAssignment &) { }
		virtual void Visit_OutCommMapping(const ESMoL::OutCommMapping &) { }
		virtual void Visit_InCommMapping(const ESMoL::InCommMapping &) { }
		virtual void Visit_MsgAssignment(const ESMoL::MsgAssignment &) { }

	private:

		bool getBus(ESMoL::BChan bchan, set<ESMoL::BChan>& nodesVisited, ESMoL::TTBus& bus);

	protected:

		ESMoL::Message getMsg( ESMoL::CommDst & cd );

		contextTracker _ct;
		std::string _ctxtName;

		Semantics::RootFolder _RootFolder;
		Semantics::Deployment _curDeployment;
};

class Stage1_ConstraintVisitor : public Stage1_AbstractVisitor
{
public:

	Stage1_ConstraintVisitor(Semantics::RootFolder & target);

	virtual ~Stage1_ConstraintVisitor() { }

	virtual void Visit_Deployment(const ESMoL::Deployment &); // Create the deployment object in the target model
	//virtual void Visit_TimingSheet(const ESMoL::TimingSheet &);

	virtual void Visit_NodeRef(const ESMoL::NodeRef &);
	//virtual void Visit_TTTask(const ESMoL::TTTask &);
	//virtual void Visit_TTMessage(const ESMoL::TTMessage &);
	virtual void Visit_MessageRef(const ESMoL::MessageRef & );

	virtual void Visit_Component(const ESMoL::Component &);
	//virtual void Visit_CInputPort(const ESMoL::CInputPort&);
	//virtual void Visit_COutputPort(const ESMoL::COutputPort&);
	//virtual void Visit_OutputPort(const ESMoL::OutputPort&);
	//virtual void Visit_InputPort(const ESMoL::InputPort&);

	//virtual void Visit_Router(const ESMoL::Router&);

	//virtual void Visit_IChan(const ESMoL::IChan &);
	//virtual void Visit_OChan(const ESMoL::OChan &);
	//virtual void Visit_BChan(const ESMoL::BChan &);
	virtual void Visit_ChannelRef(const ESMoL::ChannelRef &);

	//virtual void Visit_TTBus(const ESMoL::TTBus &);

	//virtual void Visit_MsgAssignment(const ESMoL::MsgAssignment &);

	unsigned long getNumFailures() { return _nfailures; }

private:
	unsigned long _nfailures;
};

class Stage1_ElementVisitor : public Stage1_AbstractVisitor
{
	public:

		Stage1_ElementVisitor(Semantics::RootFolder & target);

		virtual ~Stage1_ElementVisitor() { }

		//virtual void Visit_RootFolder(const ESMoL::RootFolder &);

		virtual void Visit_Deployment(const ESMoL::Deployment &); // Create the deployment object in the target model
		virtual void Visit_TimingSheet(const ESMoL::TimingSheet &) { }

		virtual void Visit_NodeRef(const ESMoL::NodeRef &);
		virtual void Visit_TTTask(const ESMoL::TTTask &);
		virtual void Visit_TTMessage(const ESMoL::TTMessage &);
		virtual void Visit_MessageRef(const ESMoL::MessageRef & );

		virtual void Visit_Component(const ESMoL::Component &);
		virtual void Visit_CInputPort(const ESMoL::CInputPort&);
		virtual void Visit_COutputPort(const ESMoL::COutputPort&);
		virtual void Visit_OutputPort(const ESMoL::OutputPort&);
		virtual void Visit_InputPort(const ESMoL::InputPort&);

		//virtual void Visit_Router(const ESMoL::Router&);

		virtual void Visit_IChan(const ESMoL::IChan &);
		virtual void Visit_OChan(const ESMoL::OChan &);
		virtual void Visit_BChan(const ESMoL::BChan &);

		virtual void Visit_TTBus(const ESMoL::TTBus &);

		virtual void Visit_OutCommMapping(const ESMoL::OutCommMapping &);
		virtual void Visit_InCommMapping(const ESMoL::InCommMapping &);
		virtual void Visit_MsgAssignment(const ESMoL::MsgAssignment &);

	private:

		void addToFileList( std::string & fileList, const std::string & elt );

		ESMoL::ChannelRef _curChanRef;
		double _hyperperiodsecs;
		unsigned long long _portnum;
	
		std::map< std::string, Semantics::SignalType > _typesVisited;
};

class Stage1_RelationVisitor : public Stage1_AbstractVisitor
{
	public:

		Stage1_RelationVisitor(Semantics::RootFolder & target);

		virtual ~Stage1_RelationVisitor() {  }

		//virtual void Visit_RootFolder(const ESMoL::RootFolder &);

		virtual void Visit_Deployment(const ESMoL::Deployment &); // Create the deployment object in the target model
		virtual void Visit_TimingSheet(const ESMoL::TimingSheet &) { }

		virtual void Visit_NodeRef(const ESMoL::NodeRef &);
		virtual void Visit_Task(const ESMoL::Task &);
		virtual void Visit_TTTask(const ESMoL::TTTask &);
		virtual void Visit_TTMessage(const ESMoL::TTMessage &);

		virtual void Visit_Component(const ESMoL::Component &);
		virtual void Visit_CInputPort(const ESMoL::CInputPort&);
		virtual void Visit_COutputPort(const ESMoL::COutputPort&);
		virtual void Visit_OutputPort(const ESMoL::OutputPort&);
		virtual void Visit_InputPort(const ESMoL::InputPort&);
		virtual void Visit_Connector(const ESMoL::Connector &);

		//virtual void Visit_Router(const ESMoL::Router&);

		virtual void Visit_IChan(const ESMoL::IChan &);
		virtual void Visit_OChan(const ESMoL::OChan &);
		virtual void Visit_BChan(const ESMoL::BChan &);

		virtual void Visit_TTBus(const ESMoL::TTBus &);

		virtual void Visit_TaskAssignment(const ESMoL::TaskAssignment &);
		virtual void Visit_OutCommMapping(const ESMoL::OutCommMapping &);
		virtual void Visit_InCommMapping(const ESMoL::InCommMapping &);
		virtual void Visit_MsgAssignment(const ESMoL::MsgAssignment &);

	private:

		Semantics::Node _curNode;
		Semantics::Task _curTask;
		ESMoL::Task _curETask;
		Semantics::Component _curComp;

		Semantics::BChan _curBChan;
		Semantics::CommInterface _curCommIface;
		Semantics::Plant _curPlant;
};

#endif//STAGE1_VISITOR_H