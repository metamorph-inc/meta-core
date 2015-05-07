#include "Stage1_Visitor.h"
#include "utils.h"
#include "Uml.h"

#include "IndexKeeper.h"

#include <sstream>
#include "ESMoL.h"

#pragma warning(disable:4996)
#pragma warning(disable:4267)
#pragma warning(disable:4244)

using namespace std;

Stage1_RelationVisitor::Stage1_RelationVisitor(Semantics::RootFolder & target) : 
	Stage1_AbstractVisitor( target )
{
	cout << "Translating model relations..." << endl;
}


void Stage1_RelationVisitor::Visit_Deployment(const ESMoL::Deployment& deploy)
{
	Semantics::Deployment dpl = IndexKeeper::inst()->lookup<Semantics::Deployment>( deploy.uniqueId() );
	_curDeployment = dpl;

	// TODO: Fix this when we have real plants in the language 
	// This could also crash, but we don't have a choice at this point.
	// We have to trust that this was created.
	set< Semantics::Plant > plants = dpl.Plant_children();
	_curPlant = *(plants.begin());
	
	Stage1_AbstractVisitor::Visit_Deployment( deploy );
}

void Stage1_RelationVisitor::Visit_NodeRef(const ESMoL::NodeRef& nodeRef)
{
	ESMoL::Node nodeObj = nodeRef.ref();
	ESMoL::OS os = nodeObj.OS_child();

	_curNode = IndexKeeper::inst()->lookup<Semantics::Node>( nodeObj.uniqueId() );
}

void Stage1_RelationVisitor::Visit_Task(const ESMoL::Task & task)
{
	Semantics::Task t = IndexKeeper::inst()->lookup<Semantics::Task>( task.uniqueId() );

	_curTask = t;
	_curETask = task;
}

// Type-specific handler
void Stage1_RelationVisitor::Visit_TTTask(const ESMoL::TTTask & tttask)
{

}

void Stage1_RelationVisitor::Visit_TTMessage(const ESMoL::TTMessage& ttmsg)
{
	// Nothing to see here!  See the connection visitors
}

void Stage1_RelationVisitor::Visit_Component(const ESMoL::Component & ecomp)
{
	ESMoL::ComponentBase cb = ecomp.ref(); // dereference it first
	Semantics::Component c = IndexKeeper::inst()->lookup<Semantics::Component>( cb.uniqueId() );
	_curComp = c;

	// Create the association
	c.invokedBy() = _curTask;
	_curTask.invokes() += c;
}

void Stage1_RelationVisitor::Visit_CInputPort(const ESMoL::CInputPort & eip)
{
	std::pair< Semantics::Signal, Semantics::SignalType > pair1 
		= IndexKeeper::inst()->lookup_pair< Semantics::Signal, Semantics::SignalType >( eip.uniqueId() );

	Semantics::Signal s = pair1.first;
	Semantics::SignalType st = pair1.second;

	s.isOfType() = st;
	st.signalsOfType() += s;
}

void Stage1_RelationVisitor::Visit_COutputPort(const ESMoL::COutputPort & eop)
{
	std::pair< Semantics::Signal, Semantics::SignalType > pair1 
		= IndexKeeper::inst()->lookup_pair< Semantics::Signal, Semantics::SignalType >( eop.uniqueId() );

	Semantics::Signal s = pair1.first;
	Semantics::SignalType st = pair1.second;

	s.isOfType() = st;
	st.signalsOfType() += s;
}

void Stage1_RelationVisitor::Visit_InputPort(const ESMoL::InputPort & eip)
{
	std::pair< Semantics::Signal, Semantics::SignalType > pair1 
		= IndexKeeper::inst()->lookup_pair< Semantics::Signal, Semantics::SignalType >( eip.uniqueId() );

	Semantics::Signal s = pair1.first;
	Semantics::SignalType st = pair1.second;

	s.isOfType() = st;
	st.signalsOfType() += s;
}

void Stage1_RelationVisitor::Visit_OutputPort(const ESMoL::OutputPort & eop)
{
	std::pair< Semantics::Signal, Semantics::SignalType > pair1 
		= IndexKeeper::inst()->lookup_pair< Semantics::Signal, Semantics::SignalType >( eop.uniqueId() );

	Semantics::Signal s = pair1.first;
	Semantics::SignalType st = pair1.second;

	s.isOfType() = st;
	st.signalsOfType() += s;
}

void Stage1_RelationVisitor::Visit_Connector(const ESMoL::Connector&)
{
}

void Stage1_RelationVisitor::Visit_TTBus(const ESMoL::TTBus & ttbus)
{	
	Semantics::TTBus b = IndexKeeper::inst()->lookup< Semantics::TTBus >( ttbus.uniqueId() );
	
	// Link interface to bus
	_curCommIface.commMedium() = b;
	b.interfaces() += _curCommIface;
}

void Stage1_RelationVisitor::Visit_BChan(const ESMoL::BChan & bc)
{
	std::pair< Semantics::CommInterface, Semantics::BChan > pair1 =
	 IndexKeeper::inst()->lookup_pair< Semantics::CommInterface, Semantics::BChan >( bc.uniqueId() );

	Semantics::CommInterface ci = pair1.first;
	Semantics::BChan sbc = pair1.second;

	// Associate interface with node
	_curNode.integrates() += ci;
	ci.integratedBy() = _curNode;

	// Associate channel with interface
	ci.commChannels() += sbc;
	sbc.chanInterface() = ci;

	// Store context for later in traversal
	_curBChan = sbc;
	_curCommIface = ci;
}

void Stage1_RelationVisitor::Visit_IChan(const ESMoL::IChan & ic)
{
	// This can be connected either to:
	// a Component (directly) via an InCommMapping
	// a Message (or MessageRef or MsgPort) via a MsgAssignment
	std::pair< Semantics::InputDevice, Semantics::IChan > pair1 =
	 IndexKeeper::inst()->lookup_pair< Semantics::InputDevice, Semantics::IChan >( ic.uniqueId() );

	Semantics::InputDevice id = pair1.first;
	Semantics::IChan sic = pair1.second;

	// Associate device with node
	_curNode.integrates() += id;
	id.integratedBy() = _curNode;

	// Associate channel with device
	id.inputChannels() += sic;
	sic.ichanDevice() = id;

	// Associate device with plant
	id.inputEnvironment() = _curPlant;
	_curPlant.acquisitionDevices() += id;
}

void Stage1_RelationVisitor::Visit_OChan(const ESMoL::OChan& oc)
{
	pair< Semantics::OutputDevice, Semantics::OChan > pair1 =
	 IndexKeeper::inst()->lookup_pair< Semantics::OutputDevice, Semantics::OChan >( oc.uniqueId() );

	Semantics::OutputDevice od = pair1.first;
	Semantics::OChan soc = pair1.second;

	// Associate device with node
	_curNode.integrates() += od;
	od.integratedBy() = _curNode;

	// Associate channel with device
	od.outputChannels() += soc;
	soc.ochanDevice() = od;

	// Associate device with plant
	od.outputEnvironment() = _curPlant;
	_curPlant.actuationDevices() += od;
}

void Stage1_RelationVisitor::Visit_TaskAssignment(const ESMoL::TaskAssignment & ta)
{
	// Look up the ends, then find the semantics objects and link them
	ESMoL::NodeRef noderef = ta.dstTaskAssignment_end();
	ESMoL::Node node = noderef.ref();
	ESMoL::Task task = ta.srcTaskAssignment_end();

	// @todo: put in handling for different task types
	Semantics::Node n = IndexKeeper::inst()->lookup<Semantics::Node>( node.uniqueId() );
	Semantics::Task t = IndexKeeper::inst()->lookup<Semantics::Task>( task.uniqueId() );

	// Create the association
	n.executes() += t;
	t.executedBy() = n;
}

void Stage1_RelationVisitor::Visit_MsgAssignment(const ESMoL::MsgAssignment & ma)
{
	// One case:  ESMoL Messages and Channels -> Semantics Messages and Media
	ESMoL::ChannelRef chanref = ma.dstMsgAssignment_end();
	ESMoL::Channel chan = chanref.ref();
	
	ESMoL::CommDst comdst = ma.srcMsgAssignment_end();
	ESMoL::Message msg = getMsg( comdst );

	if ( chan == Udm::null )
	{
		cout << _ct.getContext() << ": Null channel reference." << endl;
		return;
	}

	if ( msg == Udm::null )
	{
		cout << _ct.getContext() << ": Null message reference." << endl;
		cout << "CommDst: " << comdst.name() << endl;
		cout << "ChanRef: " << chanref.name() << " to chan: " << chan.name() << endl;
		return;
	}

	cout << chanref.type().name() << ":";
	cout << chanref.name() << endl;
	cout << chan.type().name() << ":";
	cout << chan.name() << endl;
	cout << comdst.type().name() << ":";
	cout << comdst.name() << endl;
	cout << msg.type().name() << ":";
	cout << msg.name() << endl;

	if ( chan.type() == ESMoL::BChan::meta )
	{
		ESMoL::BChan bc = ESMoL::BChan::Cast( chan );

		pair< Semantics::BChan, Semantics::Msg > pair1 =
		 IndexKeeper::inst()->lookup_pair< Semantics::BChan, Semantics::Msg >( ma.uniqueId() );

		Semantics::BChan b = pair1.first;
		Semantics::Msg m = pair1.second;

		//set< ESMoL::InCommMapping > icms = msg.dstInCommMapping();
		//set< ESMoL::OutCommMapping > ocms = msg.srcOutCommMapping();

		set< ESMoL::InCommMapping > icms = comdst.dstInCommMapping();
		set< ESMoL::OutCommMapping > ocms = comdst.srcOutCommMapping();

		set< ESMoL::MsgPort > msgports = msg.MsgPort_children();
		for ( set< ESMoL::MsgPort >::iterator itMP = msgports.begin(); itMP != msgports.end(); itMP++ )
		{
			set< ESMoL::InCommMapping > icmps = (*itMP).dstInCommMapping();
			icms.insert( icmps.begin(), icmps.end() );
			set< ESMoL::OutCommMapping > ocmps = (*itMP).srcOutCommMapping();
			ocms.insert( ocmps.begin(), ocmps.end() );
		}

		set< ESMoL::InCommMapping > icmsnew;
		set< ESMoL::OutCommMapping > ocmsnew;

		for ( set< ESMoL::InCommMapping >::iterator itICM = icms.begin(); itICM != icms.end(); itICM++ )
		{
			// Remove everything that has a different parent than right here
			ESMoL::TTTask tsk = ESMoL::TTTask::Cast((*itICM).GetParent());
			
			if ( tsk.uniqueId() == _curETask.uniqueId() )
				icmsnew.insert( *itICM );
		}

		for ( set< ESMoL::OutCommMapping >::iterator itOCM = ocms.begin(); itOCM != ocms.end(); itOCM++ )
		{
			// Remove everything that has a different parent than right here
			ESMoL::TTTask tsk = ESMoL::TTTask::Cast((*itOCM).GetParent());

			if ( tsk.uniqueId() == _curETask.uniqueId() )
				ocmsnew.insert( *itOCM );
		}

		// These two conditions should be exclusive in a single task - 
		//  it might still work if they're not, but no guarantees
		if ( icmsnew.size() > 0 ) // The message is inbound
		{
			Semantics::Receives rcv = Semantics::Receives::Create( _curDeployment );
			rcv.receivingTask() = _curTask;
			_curTask.incomingData() += rcv;

			rcv.receivedMsg() = m;
			m.msgListeners() += rcv;

			rcv.receivingChannel() = b;
			b.channelListeners() += rcv;

			cout << "Created receiver " << endl;
		}
		
		if ( ocmsnew.size() > 0 ) // The message is outbound
		{
			Semantics::Transmits trn = Semantics::Transmits::Create( _curDeployment );
			trn.sendingTask() = _curTask;
			_curTask.outboundData() += trn;

			trn.transmittedMsg() = m;
			m.msgTransmitter() = trn;

			trn.sendingChan() = b;
			b.channelTransmitters() += trn;
			cout << "Created transmitter " << endl;
		}
	}

	else if ( chan.type() == ESMoL::IChan::meta )
	{
		// There are two task relationships to worry about here:  
		// 1. The device task (represented by the IChan), which sends the message
		// 2. The receiving task (IChan parent)

		// @todo: There may be some redundant visiting in these connection handlers
		//  Look at them very carefully.  Are the relations implemented as sets?

		pair< Semantics::IChan, Semantics::Msg > pair1 =
		 IndexKeeper::inst()->lookup_pair< Semantics::IChan, Semantics::Msg >( ma.uniqueId() );
		
		Semantics::IChan ic = pair1.first;
		Semantics::Msg m = pair1.second;

		// Implement the relation
		Semantics::Acquisition acq = Semantics::Acquisition::Create( _curDeployment );
		acq.acquiredMsg() = m;
		m.msgAcquisitions() += acq;
		acq.acquiringChannel() = ic;
		ic.channelAcquisition() = acq;
		acq.acquiringTasks() += _curTask;
		_curTask.taskAcquisitions() += acq;

		cout << "Created acquisition " << endl;
	}

	else if ( chan.type() == ESMoL::OChan::meta )
	{
		Semantics::Actuation act = Semantics::Actuation::Create( _curDeployment );

		pair< Semantics::OChan, Semantics::Msg > pair1 =
		 IndexKeeper::inst()->lookup_pair< Semantics::OChan, Semantics::Msg >( ma.uniqueId() );

		Semantics::OChan oc = pair1.first;
		Semantics::Msg m = pair1.second;

		act.actuatingMsg() = m;
		m.msgActuation() = act;
		act.actuatingChannel() = oc;
		oc.channelActuations() += act;
		act.actuatingTask() = _curTask;
		_curTask.taskActuations() += act;

		cout << "Created actuation " << endl;
	}

	else
	{
		cout << "Created nothing " << endl;
	}
}

void Stage1_RelationVisitor::Visit_InCommMapping(const ESMoL::InCommMapping & icm)
{
	ESMoL::Input inp = icm.dstInCommMapping_end();
	ESMoL::CommDst comdst = icm.srcInCommMapping_end();

	ESMoL::ComponentBase comp = ESMoL::ComponentBase::Cast(inp.parent());
	ESMoL::Task tsk = ESMoL::Task::Cast(icm.parent());

	Semantics::Component c = IndexKeeper::inst()->lookup<Semantics::Component>( comp.uniqueId() );
	pair< Semantics::Signal, Semantics::SignalType > p
		= IndexKeeper::inst()->lookup_pair<Semantics::Signal, Semantics::SignalType>( inp.uniqueId() );

	Semantics::Signal s = p.first;

	if ( (comdst.type() == ESMoL::Message::meta || comdst.type() == ESMoL::TTMessage::meta)
		|| comdst.type() == ESMoL::MsgPort::meta
		|| comdst.type() == ESMoL::MessageRef::meta )
	{
		ESMoL::Message msg = getMsg( comdst );
		Semantics::Task t = IndexKeeper::inst()->lookup<Semantics::Task>( tsk.uniqueId() );
		Semantics::Msg m = IndexKeeper::inst()->lookup<Semantics::Msg>( msg.uniqueId() );

		// Signal associations
		c.consumes() += s;
		s.consumedBy() += c;

		m.carries() += s;
		s.carriedBy() += m;
	}
	else if ( comdst.type() == ESMoL::ChannelRef::meta )
	{
		ESMoL::ChannelRef chanref = ESMoL::ChannelRef::Cast(comdst);
		ESMoL::Channel chan = chanref.ref();
		if ( chan == Udm::null )
		{	
			cout << "Null channel reference at " << _ct.getContext() << "." << endl;
		}
		else if ( chan.type() == ESMoL::BChan::meta )
		{
			// @todo Handle the bus channel case
			// we'll need to create a message -- what about the other end?
			// This won't actually work:
			// T1->C1->S1->Ch1
			// T1->C2->S2->Ch1
			// Ch2->S3->C3->T2  (which one goes with it?)
			
		}
		else if ( chan.type() == ESMoL::IChan::meta )
		{
			// See the MsgAssignment handler in IChan for details
			ESMoL::IChan ic = ESMoL::IChan::Cast(chan);

			// TODO:
			// If it's a direct-connect, then
			//    create a message -- give it an identifiable name
			//    give it a type based on the signal type it's connected to
			//    hook it up to the right things (just like MsgAssignment and ICommMapping)
		}
	}
	
}

// @todo: Make this one look like the other one!
void Stage1_RelationVisitor::Visit_OutCommMapping(const ESMoL::OutCommMapping & ocm)
{
	ESMoL::Output outp = ocm.srcOutCommMapping_end();
	ESMoL::CommDst comdst = ocm.dstOutCommMapping_end();

	ESMoL::ComponentBase comp = ESMoL::ComponentBase::Cast(outp.parent());
	ESMoL::Task tsk = ESMoL::Task::Cast(ocm.parent());

	Semantics::Component c = IndexKeeper::inst()->lookup<Semantics::Component>( comp.uniqueId() );
	pair< Semantics::Signal, Semantics::SignalType > p
		= IndexKeeper::inst()->lookup_pair<Semantics::Signal, Semantics::SignalType>( outp.uniqueId() );

	Semantics::Signal s = p.first;

	if ( (comdst.type() == ESMoL::Message::meta || comdst.type() == ESMoL::TTMessage::meta)
		|| comdst.type() == ESMoL::MsgPort::meta
		|| comdst.type() == ESMoL::MessageRef::meta )
	{
		ESMoL::Message msg = getMsg( comdst );
		Semantics::Task t = IndexKeeper::inst()->lookup<Semantics::Task>( tsk.uniqueId() );
		Semantics::Msg m = IndexKeeper::inst()->lookup<Semantics::Msg>( msg.uniqueId() );

		// Signal associations
		c.generates() += s;
		s.generatedBy() = c;

		m.carries() += s;
		s.carriedBy() += m;
	}
	else if ( comdst.type() == ESMoL::ChannelRef::meta )
	{
		ESMoL::ChannelRef chanref = ESMoL::ChannelRef::Cast(comdst);
		ESMoL::Channel chan = chanref.ref();
		if ( chan == Udm::null )
		{	
			cout << "Null channel reference at " << _ct.getContext() << "." << endl;
		}
		else if ( chan.type() == ESMoL::BChan::meta )
		{
			// @todo Handle the bus channel case

		}
		else if ( chan.type() == ESMoL::OChan::meta )
		{
			

		}
	}
}



