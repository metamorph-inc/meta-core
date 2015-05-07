/*** Included Header Files ***/
#include "Visitor.h"
#include "utils.h"
#include "Uml.h"
#include "IndexKeeper.h"
#include "ConfigKeeper.h"

#include <sstream>


const long num_platforms = 6;
const char * platform_list[6] = { "PXA", "AVR", "ARM", "i686-pc-linux", "pxa-gumstix-linux", "atmega128-robostix-freertos" };

/***************************************************************************************/


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
//  * null reference checks for noderef, component, messageref, channelref, input/output port type refs
//  * component ports within a task having more than one mapping connection must use messages
//      to disambiguate the connections, also the case for one or more 
//      direct connections in the task model, but multiples in the arch model

// Node should have an OS sub-model

ConstraintVisitor::ConstraintVisitor( Semantics::RootFolder & target ) : 
AbstractVisitor( target ), _numFailures(0) {
	std::cout << "Checking model constraints..." << std::endl;

	_reserved_names.insert( string("Resolution") );
	_reserved_names.insert( string("Proc") );
	_reserved_names.insert( string("Bus") );
	_reserved_names.insert( string("Latency") );
	_reserved_names.insert( string("Comp") );
	_reserved_names.insert( string("CompGrp") );
	_reserved_names.insert( string("Msg") );
	_reserved_names.insert( string("MsgGrp") );
	_reserved_names.insert( string("Sync") );

	ConfigKeeper::Inst().EnableContext();
	//_contextTracker.showContext(true);
}


std::string ConstraintVisitor::print_reserved_names()
{
	ostringstream out;

	set< string >::iterator nameIter = _reserved_names.begin();
	out << "{ " << *nameIter;
	for ( nameIter++; nameIter != _reserved_names.end(); nameIter++ )
	{
		out << ", " << *nameIter;
	}
	out << " }";

	return out.str();
}


ConstraintVisitor::~ConstraintVisitor()
{
	ConfigKeeper::Inst().RestoreContextEnabling();
}

std::string ConstraintVisitor::MakeQualifiedName( const std::string & par, const std::string & child ) {

	return std::string(par + string("/") + child);
}

bool ConstraintVisitor::CheckDuplicateName( const std::string & name, ID_TYPE id, std::map< std::string, ID_TYPE > & names ) {

	bool bDuplicate = false;

	std::map< std::string, ID_TYPE >::iterator nameIter;
	nameIter = names.find( name );
	if ( nameIter != names.end() && (nameIter->second != id ) )
	{
		bDuplicate = true;
	}
	else
	{
		names.insert( make_pair(name, id) );
	}

	return bDuplicate;
}

void ConstraintVisitor::postError( std::string & msg )
{
	AbstractVisitor::postError(msg);
	_numFailures++;
}

void ConstraintVisitor::Visit_System( const ESMoL::System & sys ) {

	if ( sys == Udm::null ) {
		postError( string( "Null system object." ));
	}
	AbstractVisitor::Visit_System( sys );
}

void ConstraintVisitor::Visit_NodeRef( const ESMoL::NodeRef & nodeRef ) {

	if ( Udm::Object::Cast( nodeRef.ref() ) == Udm::null ) {
	
		postError( string( "Null node reference." ));
	}
}

void ConstraintVisitor::Visit_Node( const ESMoL::Node & node ) {
	
	string pftype = node.PlatformType();
	if ( pftype != string( "AVR" ) && pftype != string( "PXA" ) && pftype != string( "ARM" )
		&& pftype != string( "i686-pc-linux" ) && pftype != string( "atmega128-robostix-freertos") 
		&& pftype != string( "pxa-intel-linux" ) && pftype != string( "pxa-gumstix-linux") )
	{
		ostringstream errmsg;
		errmsg << "Platform must be one of the following: ";
		for ( int idx = 0; idx < num_platforms; idx++ )
		{
			errmsg << platform_list[idx] << " ";
		}
	    errmsg << ". You specified " << pftype << ".";
		postError( errmsg.str() );
	}

	set<ESMoL::OS> osobj = node.OS_kind_children();
	if ( osobj.empty() )
	{
		std::ostringstream warnmsg;
		warnmsg << "Node " << node.name() << " has no OS parameter object.  Using defaults.";
		postWarning( warnmsg.str() );
	}
	else
	{
		for (set<ESMoL::OS>::iterator it = osobj.begin(); it != osobj.end(); it++)
		{
			if ((*it).type() == ESMoL::OSEK::meta)
			{
				// if a node contains an OSEK OS, then it must contain an OSEKCOM object, too
				set<ESMoL::OSEKCOM> coms = node.OSEKCOM_kind_children();
				if (coms.empty())
				{
					std::string errmsg = "Node " + (std::string)node.name() + " contains an "
										 "OSEK OS object but doesn't contain an OSEKCOM object.";
					postError( errmsg );
				}
			}
		}
	}

	_curENode = node;
}

void ConstraintVisitor::Visit_BChan( const ESMoL::BChan & bc ) {

	string bchanname = MakeQualifiedName( string( _curENode.name() ), string( bc.name() ) );
	if ( CheckDuplicateName( bchanname, bc.uniqueId(), _bchan_names ) )
	{
		std::ostringstream errmsg;
		errmsg << "Duplicate bchan name (" << bchanname << ").";
		postError( errmsg.str() );	
	}
}

void ConstraintVisitor::Visit_TTBus( const ESMoL::TTBus & ttbus ) {

	if ( _reserved_names.find(string( ttbus.name() )) != _reserved_names.end() )
	{
		ostringstream errmsg;
		errmsg << "TTBus name: " << ttbus.name() << " is invalid. Do not use any of the following names:";
		errmsg << print_reserved_names();
		postError( errmsg.str() );
	}
	
	if ( CheckDuplicateName( string( ttbus.name() ), ttbus.uniqueId(), _bus_names ) )
	{
		postError( string( "Duplicate bus name." ));
	}
}

void ConstraintVisitor::Visit_CANBus( const ESMoL::CANBus & canbus ) {

	if ( _reserved_names.find(string( canbus.name() )) != _reserved_names.end() )
	{
		ostringstream errmsg;
		errmsg << "CANBus name: " << canbus.name() << " is invalid. Do not use any of the following names:";
		errmsg << print_reserved_names();
		postError( errmsg.str() );
	}

	if ( CheckDuplicateName( string( canbus.name() ), canbus.uniqueId(), _bus_names ) )
	{
		postError( string( "Duplicate bus name." ));
	}
}

void ConstraintVisitor::Visit_SerialLink( const ESMoL::SerialLink & seriallink ) {

	// Exactly two wires should be connected
	set< ESMoL::Wire > swires = seriallink.srcWire();
	set< ESMoL::Wire > dwires = seriallink.dstWire();

	if ( (swires.size() + dwires.size()) == 0 )
	{
		ostringstream errmsg;
		errmsg << "Serial link " << seriallink.name() << " has no hardware (Wire) connections.";
		postWarning( errmsg.str() );
	}
	else if ( (swires.size() + dwires.size()) != 2 )
	{
		ostringstream errmsg;
		errmsg << "Serial link " << seriallink.name() << " must have exactly two hardware (Wire) connections.";
		postError( errmsg.str() );
	}

	if ( CheckDuplicateName( string( seriallink.name() ), seriallink.uniqueId(), _bus_names ) )
	{
		postError( string( "Duplicate bus name: ") + string( seriallink.name() ) );
	}
}

void ConstraintVisitor::Visit_IChan( const ESMoL::IChan & ic ) {

	string ichanname = MakeQualifiedName( string( _curENode.name() ), string( ic.name() ) );
	if ( CheckDuplicateName( ichanname, ic.uniqueId(), _ichan_names ) )
	{
		std::ostringstream errmsg;
		errmsg << "Duplicate ichan name (" << ichanname << ").";
		postError( errmsg.str() );		
	}

	bool found_hw_dev = false;

	set< ESMoL::AcquisitionConnection> acqconns = ic.srcAcquisitionConnection();
	if ( !acqconns.empty() )
	{
		found_hw_dev = true;
	}

	set< ESMoL::Wire > in_wires = ic.srcWire();
	//cout << "IChan: found " << in_wires.size() << " src wires." << endl;
	for ( set< ESMoL::Wire >::iterator wireIter = in_wires.begin(); wireIter != in_wires.end(); wireIter++ )
	{
		ESMoL::Connectable conn = (*wireIter).srcWire_end();
		Udm::Object srcParent = conn.parent();
		if ( srcParent.type() == ESMoL::IODevice::meta )
		{
			found_hw_dev = true;
		}
	}

	set< ESMoL::Wire > out_wires = ic.dstWire();
	//cout << "IChan: found " << out_wires.size() << " dst wires." << endl;
	for ( set< ESMoL::Wire >::iterator wireIter = out_wires.begin(); wireIter != out_wires.end(); wireIter++ )
	{
		ESMoL::Connectable conn = (*wireIter).dstWire_end();
		Udm::Object dstParent = conn.parent();
		if ( dstParent.type() == ESMoL::IODevice::meta )
		{
			found_hw_dev = true;
		}
	}

	if ( !found_hw_dev )
	{
		std::ostringstream errmsg;
		errmsg << "IChan " << ic.name() << " is not connected to a hardware device.";
		postError( errmsg.str() );
	}
}

void ConstraintVisitor::Visit_OChan( const ESMoL::OChan & oc ) {

	string ochanname = MakeQualifiedName( string( _curENode.name() ), string( oc.name() ) );
	if ( CheckDuplicateName( ochanname, oc.uniqueId(), _ochan_names ) )
	{
		std::ostringstream errmsg;
		errmsg << "Duplicate ochan name (" << ochanname << ").";
		postError( errmsg.str() );		
	}

	bool found_hw_dev = false;

	set< ESMoL::ActuationConnection > actconns = oc.dstActuationConnection();
	if ( !actconns.empty() )
	{
		found_hw_dev = true;	
	}

	set< ESMoL::Wire > in_wires = oc.srcWire();
	for ( set< ESMoL::Wire >::iterator wireIter = in_wires.begin(); wireIter != in_wires.end(); wireIter++ )
	{
		ESMoL::Connectable conn = (*wireIter).srcWire_end();
		Udm::Object srcParent = conn.parent();
		if ( srcParent.type() == ESMoL::IODevice::meta )
		{
			found_hw_dev = true;
		}
	}

	set< ESMoL::Wire > out_wires = oc.dstWire();
	for ( set< ESMoL::Wire >::iterator wireIter = out_wires.begin(); wireIter != out_wires.end(); wireIter++ )
	{
		ESMoL::Connectable conn = (*wireIter).dstWire_end();
		Udm::Object dstParent = conn.parent();
		if ( dstParent.type() == ESMoL::IODevice::meta )
		{
			found_hw_dev = true;
		}
	}

	if ( !found_hw_dev )
	{
		std::ostringstream errmsg;
		errmsg << "OChan " << oc.name() << " is not connected to a hardware device.";
		postError( errmsg.str() );
	}
}


//void ConstraintVisitor::Visit_Task( const ESMoL::Task & task ) {
//	_curTask = task;
//}

void ConstraintVisitor::Visit_SubsystemRef( const ESMoL::SubsystemRef & subsysref ) {

	if ( Udm::Object::Cast( subsysref.ref() ) == Udm::null ) {

		std::ostringstream errmsg;
		errmsg << "Null subsystem reference.";
		postError( errmsg.str() );
	}

}

void ConstraintVisitor::Visit_Component( const ESMoL::Component & component ) {

	if ( _reserved_names.find(string( component.name() )) != _reserved_names.end() )
	{
		ostringstream errmsg;
		errmsg << "Component name: " << component.name() << " is invalid. Do not use any of the following names:";
		errmsg << print_reserved_names();
		postError( errmsg.str() );
	}

	if ( CheckDuplicateName( string( component.name() ), component.uniqueId(),  _comp_type_names ) )
	{
		postError( string( "Duplicate component type name." ));
	}
}

void ConstraintVisitor::Visit_Dependency( const ESMoL::Dependency & dep ) {

	ESMoL::MessageRef src_mref = dep.srcDependency_end();
	ESMoL::MessageRef dst_mref = dep.dstDependency_end();

	ESMoL::MessageRef_RefersTo_Base src_mrrtb = src_mref.ref();
	ESMoL::MessageRef_RefersTo_Base dst_mrrtb = dst_mref.ref();

	ESMoL::Message src_msg = ESMoL::Message::Cast( src_mrrtb );
	ESMoL::Message dst_msg = ESMoL::Message::Cast( dst_mrrtb );

	ESMoL::ComponentRef src_cref = dep.srcDependency__rp_helper();
	ESMoL::ComponentRef dst_cref = dep.dstDependency__rp_helper();

	//cout << "Dependency src " << src_mref.name() << "_" << src_cref.name() 
	//	<< " dst " << dst_mref.name() << "_" << dst_cref.name() << endl;

	set< ESMoL::ComponentAssignment > snrcas = src_cref.dstComponentAssignment();
	ESMoL::ComponentAssignment snrca = *(snrcas.begin());
	ESMoL::NodeRef snr = snrca.dstComponentAssignment_end();
	ESMoL::Node snode = snr.ref();

	set< ESMoL::ComponentAssignment > dnrcas = dst_cref.dstComponentAssignment();
	ESMoL::ComponentAssignment dnrca = *(dnrcas.begin());
	ESMoL::NodeRef dnr = dnrca.dstComponentAssignment_end();
	ESMoL::Node dnode = dnr.ref();

	// Constraint: Make sure both endpoints refer to the same message type
	if ( src_msg.uniqueId() != dst_msg.uniqueId() )
	{
		std::ostringstream errmsg;
		errmsg << "Dependency from " << src_cref.name() << "/" << src_mref.name() << " to ";
		errmsg << dst_cref.name() << "/" << dst_mref.name() << " connects incompatible message types: ";
		errmsg << "    " << src_msg.name() << " != " << dst_msg.name();
		postError( errmsg.str() );
	}

	// Constraint: Make sure we only visit an input port once
	ID_TYPE cid = MakeCombinedID( dst_cref.uniqueId(), dst_mref.uniqueId() );
	std::set< ID_TYPE >::iterator pFoundit =  _visited_input_ports.find( cid );
	if ( pFoundit == _visited_input_ports.end() ) // didn't find it
	{
		_visited_input_ports.insert( cid );
	}
	else // we've already seen this input port on this component ref
	{
		std::ostringstream errmsg;
		errmsg << "Visit_Dependency: Multiple connections to port " << dst_mref.name() << " on comp instance ";
		errmsg << dst_cref.name() << ". Only one connection is allowed to an input port.";
		postError( errmsg.str() );
	}

	if ( snode.uniqueId() != dnode.uniqueId() )
	{
		// If the two connected tasks are on different nodes, then they must have 
		//  CommMappings that connect to a bus channel - if not, give an error.
		bool srcFlag = false, dstFlag = false;

		ESMoL::System depParent = ESMoL::System::Cast( dep.parent() );
		set<ESMoL::CommMapping> cms = depParent.CommMapping_children();
		for ( set<ESMoL::CommMapping>::iterator cmIter = cms.begin(); cmIter != cms.end(); cmIter++ )
		{
			if ( !srcFlag )
			{
				ESMoL::CommMapping_Members_Base cmb = (*cmIter).srcCommMapping_end();
				ESMoL::CommMapping_srcCommMapping_RPContainer_Base cref = (*cmIter).srcCommMapping__rp_helper();
				if ( cmb.type() == ESMoL::MessageRef::meta 
					&& cmb.uniqueId() == src_mref.uniqueId()
					&& cref.uniqueId() == src_cref.uniqueId() )
				{
					srcFlag = true;
				}
			}

			if ( !dstFlag )
			{
				ESMoL::CommMapping_Members_Base cmb = (*cmIter).dstCommMapping_end();
				ESMoL::CommMapping_dstCommMapping_RPContainer_Base cref = (*cmIter).dstCommMapping__rp_helper();
				if ( cmb.type() == ESMoL::MessageRef::meta 
					&& cmb.uniqueId() == dst_mref.uniqueId() 
					&& cref.uniqueId() == dst_cref.uniqueId() )
				{
					dstFlag = true;
				}
			}
		}

		if ( !srcFlag )
		{
			std::ostringstream errmsg;
			errmsg << "Visit_Dependency: "  << endl; 
			errmsg << "  Source end of dependency " << src_mref.name(); 
			errmsg << " on comp instance " << src_cref.name() << " ";
			errmsg << " is not connected to a bus channel. " << endl << endl;
			errmsg << "If dependency endpoints are not on the same node, then they must also have bus channel connections.";
			postError( errmsg.str() );
		}

		if ( !dstFlag )
		{
			std::ostringstream errmsg;
			errmsg << "Visit_Dependency: "; 
			errmsg << "Destination end of dependency " << dst_mref.name();
			errmsg << " on comp instance " << dst_cref.name() << " ";
			errmsg << " is not connected to a bus channel. ";
			errmsg << "If dependency endpoints are not on the same node, then they must also have bus channel connections.";
			postError( errmsg.str() );
		}
	}

}

void ConstraintVisitor::Visit_CommMapping(const ESMoL::CommMapping & ecomm )
{
	bool inputconn = false;
	ESMoL::CommMapping_Members_Base secmb = ecomm.srcCommMapping_end();
	ESMoL::CommMapping_Members_Base decmb = ecomm.dstCommMapping_end();

	ESMoL::MessageRef mr_end;
	ESMoL::Channel chan_end;
	ESMoL::ComponentRef cref;

	if ( secmb.type() == ESMoL::MessageRef::meta )
	{
		mr_end = ESMoL::MessageRef::Cast( secmb );
		chan_end = ESMoL::Channel::Cast( decmb );
		cref = ESMoL::ComponentRef::Cast(ecomm.srcCommMapping__rp_helper());
		inputconn = false;  // which direction is the data going?
	}
	else
	{
		mr_end = ESMoL::MessageRef::Cast( decmb );
		chan_end = ESMoL::Channel::Cast( secmb );
		cref = ESMoL::ComponentRef::Cast(ecomm.dstCommMapping__rp_helper());
		inputconn = true;
	}

	// Constraint: Make sure we only visit an input port once
	ID_TYPE cid = MakeCombinedID( cref.uniqueId(), mr_end.uniqueId() );
	std::set< ID_TYPE >::iterator pFoundit =  _visited_input_ports.find( cid );
	if ( chan_end.type() == ESMoL::IChan::meta )
	{
		if ( inputconn && ( pFoundit == _visited_input_ports.end() ) ) // didn't find it
		{
			_visited_input_ports.insert( cid );
		}
		else if ( inputconn ) // we've already seen this input port on this component ref
		{
			std::ostringstream errmsg;
			errmsg << "Visit_CommMapping: "; 
			errmsg << "Multiple connections to port " << mr_end.name() << " on comp instance ";
			errmsg << cref.name() << ".";
			errmsg << " Only one connection is allowed to an input port. Most likely you have both a ";
			errmsg << std::endl << "dependency connection (logical arch aspect)";
			errmsg << " and an IChan connection (deployment aspect). Please double-check.";
			postError( errmsg.str() );
		}
	}

	// Constraint -- make sure that any CommMapping connection to a BChan is also has a Dependency connection.
	bool foundone = false;
	if ( chan_end.type() == ESMoL::BChan::meta )
	{
		set< ESMoL::Dependency > srcDeps = mr_end.srcDependency();
		for ( set< ESMoL::Dependency >::iterator depIter = srcDeps.begin();
				( depIter != srcDeps.end() ) && ( foundone == false ) ; depIter++ )
		{
			ESMoL::ComponentRef depcref = (*depIter).dstDependency__rp_helper();
			if ( cref.uniqueId() == depcref.uniqueId() )
			{
				foundone = true; // note: this appears in the loop condition
			}
		}

		set< ESMoL::Dependency > dstDeps = mr_end.dstDependency();
		for ( set< ESMoL::Dependency >::iterator depIter = dstDeps.begin();
				( depIter != dstDeps.end() ) && ( foundone == false ) ; depIter++ )
		{
			ESMoL::ComponentRef depcref = (*depIter).srcDependency__rp_helper();
			if ( cref.uniqueId() == depcref.uniqueId() )
			{
				foundone = true; // note: this appears in the loop condition
			}
		}

		if ( foundone == false )
		{
			std::ostringstream errmsg;
			errmsg << "Message instance " << cref.name() << "/" << mr_end.name();
			errmsg << " has a CommMapping connection, but no Dependency connection.  Look at the LogicalArchitectureView aspect.";
			postError( errmsg.str() );
		}
	}

	
}

void ConstraintVisitor::Visit_ComponentRef( const ESMoL::ComponentRef & compref ) {

	if ( Udm::Object::Cast( compref.ref() ) == Udm::null ) {

		postError( string( "Null component reference." ));
	}

	if ( _reserved_names.find(string( compref.name() )) != _reserved_names.end() )
	{
		ostringstream errmsg;
		errmsg << "Component reference name: " << compref.name() << " is invalid. Do not use any of the following names:";
		errmsg << print_reserved_names();
		postError( errmsg.str() );
	}

	if ( CheckDuplicateName( string( compref.name() ), compref.uniqueId(), _comp_inst_names ) )
	{
		postError( string( "Duplicate component ref name." ));
	}

	set< ESMoL::ComponentAssignment > cas = compref.dstComponentAssignment();
	if ( cas.size() > 1 )
	{
		std::ostringstream errmsg;
		errmsg << "Component instance " << compref.name() << " is assigned to multiple processor nodes ( ";
		set< ESMoL::ComponentAssignment >::iterator caIter = cas.begin();
		ESMoL::NodeRef nr = (*caIter).dstComponentAssignment_end();
		ESMoL::Node node = nr.ref();
		errmsg << node.name();
		for ( caIter++; caIter != cas.end(); caIter++ )
		{
			ESMoL::NodeRef nr = (*caIter).dstComponentAssignment_end();
			ESMoL::Node node = nr.ref();
			errmsg << ", " << node.name();
		}
		errmsg << " ).  Exactly one assignment is allowed for each component instance.";
		postError( errmsg.str() );
	}
	else if ( cas.size() == 0 )
	{
		std::ostringstream errmsg;
		errmsg << "Component instance " << compref.name() << " is not assigned to any processor.";
		postError( errmsg.str() );
	}

	set< ESMoL::ExecutionAssignment > exaSet = compref.srcExecutionAssignment();
	if ( exaSet.size() == 0 )
	{
		std::ostringstream errmsg;
		errmsg << "Component instance " << compref.name() << " has no execInfo object.";
		postError( errmsg.str() );
	}

	_curECompRef = compref;
}

void ConstraintVisitor::Visit_MessageRef( const ESMoL::MessageRef & messageRef ) {

	if ( Udm::Object::Cast( messageRef.ref() ) == Udm::null ) {

		postError( string( "Null message reference." ));
	}

	if ( _reserved_names.find(string( messageRef.name() )) != _reserved_names.end() )
	{
		ostringstream errmsg;
		errmsg << "MessageRef name: " << messageRef.name() << " is invalid. Do not use any of the following names:";
		errmsg << print_reserved_names();
		postError( errmsg.str() );
	}

	string msgname = MakeQualifiedName( string( _curECompRef.name() ), string( messageRef.name() ) );
	if ( CheckDuplicateName( msgname, messageRef.uniqueId(), _msg_inst_names ) )
	{
		std::ostringstream errmsg;
		errmsg << "Duplicate message ref name (" << msgname << ").";
		postError( errmsg.str() );
	}
}

void ConstraintVisitor::Visit_Message( const ESMoL::Message & msg ) {

	if ( _reserved_names.find(string( msg.name() )) != _reserved_names.end() )
	{
		ostringstream errmsg;
		errmsg << "Message name: " << msg.name() << " is invalid. Do not use any of the following names:";
		errmsg << print_reserved_names();
		postError( errmsg.str() );
	}	

	// These will get visited multiple times in the current traversal
	if ( CheckDuplicateName( string( msg.name() ), msg.uniqueId(), _msg_type_names ) )
	{
		postError( string( "Duplicate message type name." ));
	}
}

void ConstraintVisitor::Visit_MsgPort( const ESMoL::MsgPort & msgport ) {

	// These will get visited multiple times in the current traversal
	ESMoL::Message msg = msgport.Message_parent();
	string msgportname = MakeQualifiedName( string(msg.name()), string(msgport.name()) );
	if ( CheckDuplicateName( msgportname, msgport.uniqueId(), _msg_port_names ) )
	{
		std::ostringstream errmsg;
		errmsg << "Duplicate message port name(" << msgportname << ").";
		postError( errmsg.str() );
	}
}


/*void ConstraintVisitor::Visit_ChannelRef( const ESMoL::ChannelRef &channelRef ) {
	if ( Udm::Object::Cast( channelRef.ref() ) == Udm::null ) {
		std::cout << this->_contextTracker.GetContext() << ": Null channel reference." << std::endl;
		this->_numFailures++;
	}
}*/

void ConstraintVisitor::Visit_CInputPort( const ESMoL::CInputPort & cip ) {

	set< ESMoL::Connector > iconn = cip.srcConnector();
	if ( iconn.size() == 0 )
	{
		postError( string( "Input port has no connections, which is not valid for a synchronous dataflow block." ));
	}	

/*	set< ESMoL::InCommMapping > icms = cip.srcInCommMapping();
	int mcount = 0;  // count incommmapping connections to port
	for ( set< ESMoL::InCommMapping >::iterator picm = icms.begin(); picm != icms.end(); picm++ )
	{
		Udm::Object obj = (*picm).parent();
		if ( obj.uniqueId() == _curTask.uniqueId() )
		{
			ESMoL::CommDst cd = picm->srcInCommMapping_end();
			if ( cd.type() == ESMoL::ChannelRef::meta )
				mcount++;
		}
	}

	int ccount = 0; // count connector connections to port (within task)
	int acount = 0; // count connector connections to port (in arch model)
	set< ESMoL::Connector > conns = cip.srcConnector();
	for ( set< ESMoL::Connector >::iterator pconn = conns.begin(); pconn != conns.end(); pconn++ )
	{
		Udm::Object obj = (*pconn).parent();
		if ( obj.uniqueId() == _curTask.uniqueId() )
			ccount++;
		else if ( obj.type() == ESMoL::System::meta )
			acount++;
	}

	// Decision logic
	if ( (mcount + ccount) > 1 ) {
		std::cout << this->_contextTracker.GetContext() 
			<< ": Component ports with more than one channel or component connection "
			<< "in the same task must use message references for disambiguation." << endl;
		this->_numFailures++;
	}

	if ( (acount > 1) && ((mcount + ccount) > 0) ) {
		std::cout << this->_contextTracker.GetContext()
			<< ": Component ports with more than one connection in the architecture "
			<< "model must use message references to disambiguate connections in the task." << endl;
		this->_numFailures++;
	}
	*/
}

void ConstraintVisitor::Visit_COutputPort( const ESMoL::COutputPort & cop ) {

	set< ESMoL::Connector > oconn = cop.dstConnector();
	if ( oconn.size() == 0 )
	{
		std::ostringstream warnmsg;
		warnmsg << "Output port " << cop.name() << " has no connector - creating a dummy message.";
		postWarning( warnmsg.str() );

	}

/*	set< ESMoL::OutCommMapping > ocms = cop.dstOutCommMapping();
	int mcount = 0;
	for ( set< ESMoL::OutCommMapping >::iterator pocm = ocms.begin(); pocm != ocms.end(); pocm++ )
	{
		Udm::Object obj = (*pocm).parent();
		if ( obj.uniqueId() == _curTask.uniqueId() )
		{
			ESMoL::CommDst cd = pocm->dstOutCommMapping_end();
			if ( cd.type() == ESMoL::ChannelRef::meta )
				mcount++;
		}
	}

	int ccount = 0; // count connector connections to port (within task)
	int acount = 0; // count connector connections to port (in arch model)
	set< ESMoL::Connector > conns = cop.dstConnector();
	for ( set< ESMoL::Connector >::iterator pconn = conns.begin(); pconn != conns.end(); pconn++ )
	{
		Udm::Object obj = (*pconn).parent();
		if ( obj.uniqueId() == _curTask.uniqueId() )
			ccount++;
		else if ( obj.type() == ESMoL::System::meta )
			acount++;
	}

	// Decision logic
	if ( (mcount + ccount) > 1 ) {
		std::cout << this->_contextTracker.GetContext() 
			<< ": Component ports with more than one channel or component connection "
			<< "in the same task must use message references for disambiguation." << endl;
		this->_numFailures++;
	}

	if ( (acount > 1) && ((mcount + ccount) > 0) ) {
		std::cout << this->_contextTracker.GetContext()
			<< ": Component ports with more than one connection in the architecture "
			<< "model must use message references to disambiguate connections in the task." << endl;
		this->_numFailures++;
	}
*/
}

void ConstraintVisitor::Visit_InputPort( const ESMoL::InputPort & ip ) {

	set< ESMoL::Connector > iconn = ip.srcConnector();
	if ( iconn.size() == 0 )
	{
		postError( string( "Input port has no connections, which is not valid for a synchronous dataflow block." ));
	}	

/*
	ESMoL::TypeBaseRef tbr = ip.TypeBaseRef_child();
	if ( tbr == Udm::null || (Udm::Object::Cast( tbr.ref() ) == Udm::null ) )
	{
		cout << this->_contextTracker.GetContext() << ": Input port has no type -- did you run addtypes?" << endl;
		this->_numFailures++;
	}

	set< ESMoL::InCommMapping > icms = ip.srcInCommMapping();
	int mcount = 0;

	for ( set< ESMoL::InCommMapping >::iterator picm = icms.begin(); picm != icms.end(); picm++ )
	{
		Udm::Object obj = (*picm).parent();
		if ( obj.uniqueId() == _curTask.uniqueId() )
		{
			ESMoL::CommDst cd = picm->srcInCommMapping_end();
			if ( cd.type() == ESMoL::ChannelRef::meta )
				mcount++;
		}
	}

	int ccount = 0; // count connector connections to port (within task)
	int acount = 0; // count connector connections to port (in arch model)
	set< ESMoL::Connector > conns = ip.srcConnector();
	for ( set< ESMoL::Connector >::iterator pconn = conns.begin(); pconn != conns.end(); pconn++ )
	{
		Udm::Object obj = (*pconn).parent();
		if ( obj.uniqueId() == _curTask.uniqueId() )
			ccount++;
		else if ( obj.type() == ESMoL::System::meta )
			acount++;
	}

	// Decision logic
	if ( (mcount + ccount) > 1 ) {
		std::cout << this->_contextTracker.GetContext() 
			<< ": Component ports with more than one channel or component connection "
			<< "in the same task must use message references for disambiguation." << endl;
		this->_numFailures++;
	}

	if ( (acount > 1) && ((mcount + ccount) > 0) ) {
		std::cout << this->_contextTracker.GetContext()
			<< ": Component ports with more than one connection in the architecture "
			<< "model must use message references to disambiguate connections in the task." << endl;
		this->_numFailures++;
	}
	*/
}

void ConstraintVisitor::Visit_OutputPort( const ESMoL::OutputPort & op ) {

	set< ESMoL::Connector > oconn = op.dstConnector();
	if ( oconn.size() == 0 )
	{
		std::ostringstream warnmsg;
		warnmsg << "Output port " << op.name() <<  " has no connector - creating a dummy message.";
		postWarning( warnmsg.str() );
	}
/*
	ESMoL::TypeBaseRef tbr = op.TypeBaseRef_child();
	if ( tbr == Udm::null || (Udm::Object::Cast( tbr.ref() ) == Udm::null ) )
	{
		cout << this->_contextTracker.GetContext() << ": Output port has no type -- did you run addtypes?" << endl;
		this->_numFailures++;
	}

	set< ESMoL::OutCommMapping > ocms = op.dstOutCommMapping();
	int mcount = 0;
	for ( set< ESMoL::OutCommMapping >::iterator pocm = ocms.begin(); pocm != ocms.end(); pocm++ )
	{
		Udm::Object obj = (*pocm).parent();
		if ( obj.uniqueId() == _curTask.uniqueId() )
		{
			ESMoL::CommDst cd = pocm->dstOutCommMapping_end();
			if ( cd.type() == ESMoL::ChannelRef::meta )
				mcount++;
		}
	}

	int ccount = 0; // count connector connections to port (within task)
	int acount = 0; // count connector connections to port (in arch model)
	set< ESMoL::Connector > conns = op.dstConnector();
	for ( set< ESMoL::Connector >::iterator pconn = conns.begin(); pconn != conns.end(); pconn++ )
	{
		Udm::Object obj = (*pconn).parent();
		if ( obj.uniqueId() == _curTask.uniqueId() )
			ccount++;
		else if ( obj.type() == ESMoL::System::meta )
			acount++;
	}

	// Decision logic
	if ( (mcount + ccount) > 1 ) {
		std::cout << this->_contextTracker.GetContext() 
			<< ": Component ports with more than one channel or component connection "
			<< "in the same task must use message references for disambiguation." << endl;
		this->_numFailures++;
	}

	if ( (acount > 1) && ((mcount + ccount) > 0) ) {
		std::cout << this->_contextTracker.GetContext()
			<< ": Component ports with more than one connection in the architecture "
			<< "model must use message references to disambiguate connections in the task." << endl;
		this->_numFailures++;
	}
	*/
}

void ConstraintVisitor::Visit_TTExecInfo( const ESMoL::TTExecInfo & ttei ) {

	if ( CheckDuplicateName( string( ttei.name() ), ttei.uniqueId(), _exec_info_names ) )
	{
		string msg = string( "Duplicate ExecInfo name:" ) + string( ttei.name() );
		postError( msg );
	}
}

void ConstraintVisitor::Visit_AsyncPeriodicExecInfo( const ESMoL::AsyncPeriodicExecInfo & apei ) {

	if ( CheckDuplicateName( string( apei.name() ), apei.uniqueId(), _exec_info_names ) )
	{
		string msg = string( "Duplicate ExecInfo name:" ) + string( apei.name() );
		postError( msg );
	}
}

void ConstraintVisitor::Visit_SporadicExecInfo( const ESMoL::SporadicExecInfo & spei ) {

	if ( CheckDuplicateName( string( spei.name() ), spei.uniqueId(), _exec_info_names ) )
	{
		string msg = string( "Duplicate ExecInfo name:" ) + string( spei.name() );
		postError( msg );
	}
}

void ConstraintVisitor::Visit_Unused_MsgRefs() {

	//cout << "Unused msg refs: " << _msgRefsLeft.size() << endl;

	std::map< ESMoL::MessageRef, ESMoL::ComponentRef >::iterator mrefIter;
	for ( mrefIter = _msgRefsLeft.begin(); mrefIter != _msgRefsLeft.end(); mrefIter++ )
	{
		//cout << (mrefIter->first).name() << "/" << (mrefIter->second).name() << endl;
		ESMoL::MessageRef_RefersTo_Base mrrtb = (mrefIter->first).ref();
		ESMoL::Message msg = ESMoL::Message::Cast( mrrtb );
		std::set< ESMoL::MsgPort > fields = msg.MsgPort_children();
		
		CONN_DIR conndir = NO_CONN;
		for ( std::set< ESMoL::MsgPort >::iterator mpIter = fields.begin(); 
				mpIter != fields.end(); mpIter++ )
		{
			set< ESMoL::Connector > srcconns = mpIter->srcConnector();
			for ( set< ESMoL::Connector >::iterator connIter = srcconns.begin(); 
					connIter != srcconns.end(); connIter++ )
			{
				// These are input connections - check to see if one of them matches 
				ESMoL::MessageRef mref = ESMoL::MessageRef::Cast((*connIter).dstConnector__rp_helper());
				//ESMoL::Component comp = (*connIter).Component_parent();
				//cout << "Checking  " << mref.name() << "/" << comp.name() << endl;
				if ( mref.uniqueId() == (mrefIter->first).uniqueId() )
				{
					conndir = OUTPUT_CONN;
				}
			}

			set< ESMoL::Connector > dstconns = mpIter->dstConnector();
			for ( set< ESMoL::Connector >::iterator connIter = dstconns.begin();
					connIter != dstconns.end(); connIter++ )
			{
				// These are output connections - check to see if one of them matches
				ESMoL::MessageRef mref = ESMoL::MessageRef::Cast((*connIter).srcConnector__rp_helper());
				//ESMoL::Component comp = (*connIter).Component_parent();

				//cout << "Checking  " << mref.name() << "/" << comp.name() << endl;
				if ( mref.uniqueId() == (mrefIter->first).uniqueId() )
				{
					conndir = INPUT_CONN;
				}
			}
		}

		CONN_DIR mrefdir =  _msgDirection[mrefIter->first];
		if ( (conndir != NO_CONN && mrefdir != NO_CONN) && conndir != mrefdir )
		{
			std::ostringstream errmsg;
			errmsg << "For message instance " << (mrefIter->second).name() << "/";
			errmsg << (mrefIter->first).name() << " is connected inconsistently (check inside the component and ";
			errmsg << "outside the component).";
			postError( errmsg.str() );
		}

		else if ( conndir == INPUT_CONN )
		{
			std::ostringstream errmsg;
			errmsg << "Input message instance " << (mrefIter->second).name() << "/";
			errmsg << (mrefIter->first).name() << " is not connected.";
			postError( errmsg.str() );
		}
		else if ( conndir == OUTPUT_CONN )
		{
			std::ostringstream warnmsg;
			warnmsg << "Output message instance " << (mrefIter->second).name() << "/";
			warnmsg << (mrefIter->first).name() << " is not connected.";
			postWarning( warnmsg.str() );
		}
		else // NO_CONN
		{
			ostringstream errmsg;
			errmsg << "Message instance " << (mrefIter->second).name() << "/";
			errmsg << (mrefIter->first).name();
			errmsg << " is not connected, either within or outside of the component." << endl;
			postError( errmsg.str() );
		}
	

		set< ESMoL::ExecutionAssignment > eas = (mrefIter->first).srcExecutionAssignment();
		for( set< ESMoL::ExecutionAssignment >::iterator eaIter = eas.begin();
				eaIter != eas.end(); eaIter++ )
		{
			ESMoL::ComponentRef cref = ESMoL::ComponentRef::Cast((*eaIter).dstExecutionAssignment__rp_helper());
			if ( cref.uniqueId() == (mrefIter->second).uniqueId() )
			{
				ESMoL::ExecutionInfo ei = ( *eaIter ).srcExecutionAssignment_end();
				std::ostringstream errmsg;
				errmsg << "Message instance " << (mrefIter->second).name() << "/";
				errmsg << (mrefIter->first).name();
				errmsg << " is unconnected, but has an execInfo timing specification ";
				errmsg << "( " << ei.name()  << " ).";
				postError( errmsg.str() );
			}
		}
	}
}


void ConstraintVisitor::Visit_Unused_MsgPorts()
{
	std::map< ESMoL::MsgPort, ESMoL::MessageRef >::iterator mportIter;
	for ( mportIter = _msgPortsLeft.begin(); mportIter != _msgPortsLeft.end(); mportIter++ )
	{
		CONN_DIR mrefdir = _msgDirection[mportIter->second];
		if ( mrefdir == INPUT_CONN )
		{
			std::ostringstream warnmsg;
			warnmsg << "Port " << (mportIter->first).name() << " of input message instance ";
			warnmsg << (mportIter->second).name() << " is unconnected.";
			postWarning( warnmsg.str() );
		}
		else if ( mrefdir == OUTPUT_CONN )
		{
			ostringstream errmsg;
			errmsg << "Port " << (mportIter->first).name() << " of output message instance ";
			errmsg << (mportIter->second).name() << " is unconnected.";
			postError( errmsg.str() );
		}
		else // Unconnected message
		{
			std::ostringstream warnmsg;
			warnmsg << "Port " << (mportIter->first).name() << " of unconnected message instance ";
			warnmsg << (mportIter->second).name() << " is unconnected.";
			postWarning( warnmsg.str() );
		}
	}
}


/***************************************************************************************/

