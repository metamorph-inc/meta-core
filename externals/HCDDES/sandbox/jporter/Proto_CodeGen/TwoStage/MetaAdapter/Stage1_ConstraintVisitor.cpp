#include "Stage1_Visitor.h"
#include "utils.h"
#include "Uml.h"

#include "IndexKeeper.h"

#include <sstream>

// This is where we keep the constraints
// Make sure each constraint returns meaningful error messages.

// List of constraints that need implementing:

// All required data fields must have a value.
// All data fields must have a valid value.

// No null references!

// No MsgAssignment connection from a ChannelRef to another ChannelRef
//  Later, we may want to allow a simple message transfer, and here is where
//   we would check to make sure the data types are compatible.

// Message identity for devices:
// If multiple tasks use the same IChan/OChan objects, here are the message 
// definition rules:
//  If one task in the set contains an explicit message object, all the rest
//  must have a reference to the same message.
//  If one task in the set has a direct connection to a component, then all of
//  them must have a direct connection to a component.
// Obviously, all Components connected to the same device must have compatible
//  data types.

// An IChan reference should only be connected via an InCommMapping, and
// an OChan reference should only be connected via an OutCommMapping.

// A MsgAssignment shouldn't be connected to a MsgPort on the comms channel side.

// Each bus channel object should connect to at most one bus (a direct connect may be OK,
//  depending on the protocol).

// List of constraints that are done:


Stage1_ConstraintVisitor::Stage1_ConstraintVisitor(Semantics::RootFolder & target) : 
Stage1_AbstractVisitor( target ), _nfailures(0)
{
	cout << "Enforcing model constraints..." << endl;
}

void Stage1_ConstraintVisitor::Visit_Deployment(const ESMoL::Deployment& deploy)
{
	if (deploy==Udm::null)
	{
		cout << _ct.getContext() << ": Null deployment object." << endl;
		_nfailures++;
	}

	Stage1_AbstractVisitor::Visit_Deployment( deploy );
}

void Stage1_ConstraintVisitor::Visit_NodeRef(const ESMoL::NodeRef & noderef )
{
	if ( Udm::Object::Cast(noderef.ref()) == Udm::null )
	{
		cout << _ct.getContext() << ": null node reference." << endl;
		_nfailures++;
	}
}

void Stage1_ConstraintVisitor::Visit_Component(const ESMoL::Component & comp)
{
	if ( Udm::Object::Cast(comp.ref()) == Udm::null )
	{
		cout << _ct.getContext() << ": null component reference." << endl;
		_nfailures++;
	}
}

void Stage1_ConstraintVisitor::Visit_MessageRef(const ESMoL::MessageRef & msgref )
{
	if ( Udm::Object::Cast(msgref.ref()) == Udm::null )
	{
		cout << _ct.getContext() << ": null message reference." << endl;
		_nfailures++;
	}
}

void Stage1_ConstraintVisitor::Visit_ChannelRef(const ESMoL::ChannelRef & chanref)
{
	if ( Udm::Object::Cast(chanref.ref()) == Udm::null )
	{
		cout << _ct.getContext() << ": null channel reference." << endl;
		_nfailures++;
	}
}
