/*** Included Header Files ***/
#include "Visitor.h"
#include "utils.h"
#include "Uml.h"
#include "ConfigKeeper.h"
#include "IndexKeeper.h"


/*** Compiler Warning Disables ***/
#pragma warning(disable:4996)
#pragma warning(disable:4267)
#pragma warning(disable:4244)

using namespace std;

/***************************************************************************************/

string AbstractVisitor::idToString( const char * prefix, Udm::Object::uniqueId_type id )
{
	ostringstream out;
	out << prefix << id;
	return out.str();
}

void AbstractVisitor::postError( std::string & msg )
{
	std::string ctxt( _contextTracker.GetContext() );
	if ( ctxt.empty() )
	{
		ctxt = std::string("unknown context");
	}
	cout << "ERROR (in " << ctxt << "): " << msg << endl << endl;

}

void AbstractVisitor::postWarning( std::string & msg )
{
	std::string ctxt( _contextTracker.GetContext() );
	if ( ctxt.empty() )
	{
		ctxt = std::string("unknown context");
	}
	cout << "WARNING (in " << ctxt << "): " << msg << endl << endl;
}

void AbstractVisitor::Visit_RootFolder( const ESMoL::RootFolder & rootFolder) {
	Context rootFolderContext( _contextTracker, rootFolder.name() );
	set< ESMoL::DesignFolder > designFolderSet = rootFolder.DesignFolder_children();

	for( set<ESMoL::DesignFolder>::iterator dfIter = designFolderSet.begin(); dfIter!= designFolderSet.end(); dfIter++) {
		
		// DESIGN FOLDER
		ESMoL::DesignFolder designFolder = (*dfIter);
		Context designFolderContext( _contextTracker, designFolder.name() );
		
		// HARDWARE UNIT
		vector< ESMoL::HardwareUnit > hwUnits = designFolder.HardwareUnit_children();
		for ( vector< ESMoL::HardwareUnit >::iterator hwUIter = hwUnits.begin(); hwUIter != hwUnits.end(); hwUIter++ )
		{
			Context hwuContext( _contextTracker, (*hwUIter).name() );
			(*hwUIter).Accept( *this );
		}

		// SYSTEM
		vector< ESMoL::System > sysVector = designFolder.System_children();
		for( vector< ESMoL::System >::iterator sysIter = sysVector.begin(); sysIter != sysVector.end(); sysIter++ ) {

			Context archContext( _contextTracker, (*sysIter).name() );
			(*sysIter).Accept( *this );
		}
		
		// TIMING SHEET
		vector< ESMoL::TimingSheet > timingSheetVector = designFolder.TimingSheet_children();
		for( vector< ESMoL::TimingSheet >::iterator tsIter = timingSheetVector.begin(); tsIter != timingSheetVector.end(); tsIter++) {

			Context timingSheetContext( _contextTracker, (*tsIter).name() );
			(*tsIter).Accept( *this );
		}
	}
}

void AbstractVisitor::Visit_System(const ESMoL::System & sys) {

	// Any instantiation/setup was done already by the specialized subclass

	// NODEREFS
	set< ESMoL::NodeRef > nodeRefSet = sys.NodeRef_children();
	for( set< ESMoL::NodeRef >::iterator nodeRefIter = nodeRefSet.begin(); nodeRefIter != nodeRefSet.end(); nodeRefIter++) {
		ESMoL::NodeRef nodeRef = (*nodeRefIter);

		Context nodeRefContext( _contextTracker, nodeRef.name() );
		nodeRef.Accept(*this);

		{
			ESMoL::Node node = nodeRef.ref(); // dangling NodeRefs were checked previously
			Context nodeContext( _contextTracker, node.name() );
			node.Accept(*this);

			{
				set<ESMoL::OS> oses = node.OS_kind_children(); // this should also have been checked for existence
				if (oses.size() > 0) {
					ESMoL::OS os = *(oses.begin());
					Context osContext( _contextTracker, os.name() );
					os.Accept(*this);
				}
			}
		

			// BUS CHANNELS
			set< ESMoL::BChan> bChanSet = node.BChan_kind_children();
			for(set< ESMoL::BChan>::iterator bChanIter = bChanSet.begin(); bChanIter != bChanSet.end(); bChanIter++) {
				ESMoL::BChan bChan = (*bChanIter);
				ESMoL::Bus bus;

				Context bChanContext( _contextTracker, bChan.name() );
				bChan.Accept( *this) ;

				set< ESMoL::BChan > nodesVisited;
				nodesVisited.clear();
				nodesVisited.insert( bChan );
				if ( GetBus( bChan, nodesVisited, bus )) {
					Context busContext( _contextTracker, bus.name() );

					if ( bus.type() == ESMoL::TTBus::meta )
					{
						ESMoL::TTBus ttbus = ESMoL::TTBus::Cast( bus );
						ttbus.Accept( *this );
					}

					if ( bus.type() == ESMoL::CANBus::meta )
					{
						ESMoL::CANBus canbus = ESMoL::CANBus::Cast( bus );
						canbus.Accept( *this );
					}

					if ( bus.type() == ESMoL::SerialLink::meta )
					{
						ESMoL::SerialLink seriallink = ESMoL::SerialLink::Cast( bus );
						seriallink.Accept( *this );
					}
				}
			}

			// INPUT CHANNELS (ACQUISITION DEVICES)
			vector< ESMoL::IChan > icVector = node.IChan_kind_children();
			for( vector< ESMoL::IChan >::iterator icIter = icVector.begin(); icIter != icVector.end(); icIter++) {
				// Create a task and message
				Context icContext( _contextTracker, (*icIter).name() );
				(*icIter).Accept(*this);

				vector< ESMoL::IODevice > iodevices;

				// Collect all of the IODevices connected directly to the IChan object
				set< ESMoL::AcquisitionConnection > acqSet = (*icIter).srcAcquisitionConnection();
				for ( set< ESMoL::AcquisitionConnection >::iterator acqIter = acqSet.begin(); acqIter != acqSet.end(); acqIter++ )
				{
					ESMoL::IODevice iod = (*acqIter).srcAcquisitionConnection_end();
					iodevices.push_back( iod );
				}

				// Collect all of the IODevices connected to the IChan by a Wire (dst)
				set< ESMoL::Wire > wireSet = (*icIter).dstWire();
				for ( set< ESMoL::Wire >::iterator wireIter = wireSet.begin(); wireIter != wireSet.end(); wireIter++ )
				{
					ESMoL::Connectable conn = (*wireIter).dstWire_end();
					if ( conn.type() == ESMoL::OChan::meta )
					{
						Udm::Object obj = conn.parent();
						if ( obj.type() == ESMoL::IODevice::meta )
						{
							ESMoL::IODevice iod = ESMoL::IODevice::Cast( obj );
							iodevices.push_back( iod );
						}
					}
				}

				// Collect all of the IODevices connected to the IChan by a Wire (src)
				wireSet = (*icIter).srcWire();
				for ( set< ESMoL::Wire >::iterator wireIter = wireSet.begin(); wireIter != wireSet.end(); wireIter++ )
				{
					ESMoL::Connectable conn = (*wireIter).srcWire_end();
					if ( conn.type() == ESMoL::OChan::meta )
					{
						Udm::Object obj = conn.parent();
						if ( obj.type() == ESMoL::IODevice::meta )
						{
							ESMoL::IODevice iod = ESMoL::IODevice::Cast( obj );
							iodevices.push_back( iod );
						}
					}
				}

				// Process all of the collected IODevice objects
				for ( vector< ESMoL::IODevice >::iterator ioIter = iodevices.begin(); ioIter != iodevices.end(); ioIter++ )
				{
					
					Context iodContext( _contextTracker, (*ioIter).name() );
					(*ioIter).Accept(*this);

					set< ESMoL::PhysConnection > pcSet = (*ioIter).srcPhysConnection();
					set< ESMoL::PhysConnection > pcSet2 = (*ioIter).dstPhysConnection();
					pcSet.insert( pcSet2.begin(), pcSet2.end() );

					for ( set< ESMoL::PhysConnection >::iterator pcIter = pcSet.begin(); pcIter != pcSet.end(); pcIter++ )
					{
						if ( (*pcIter).type() == ESMoL::Plant::meta )
						{
							ESMoL::Plant pl = ESMoL::Plant::Cast( *pcIter );
							Context plContext( _contextTracker, pl.name() );
							pl.Accept( *this );
						}
					}
				}
			} 

			// OUTPUT CHANNELS (ACTUATION DEVICES)
			vector< ESMoL::OChan > ocVector = node.OChan_kind_children();
			for( vector< ESMoL::OChan >::iterator ocIter = ocVector.begin(); ocIter != ocVector.end(); ocIter++) {
				// Create a task and message
				Context ocContext( _contextTracker, (*ocIter).name() );
				(*ocIter).Accept(*this);

				vector< ESMoL::IODevice > iodevices;
				
				// Collect iodevices directly connected to this OChan
				set< ESMoL::ActuationConnection > actSet = (*ocIter).dstActuationConnection();
				for ( set< ESMoL::ActuationConnection >::iterator actIter = actSet.begin(); actIter != actSet.end(); actIter++ )
				{
					ESMoL::IODevice iod = (*actIter).dstActuationConnection_end();
					iodevices.push_back( iod );
				}

				// Collect all of the IODevices connected to the OChan by a Wire (dst)
				set< ESMoL::Wire > wireSet = (*ocIter).dstWire();
				for ( set< ESMoL::Wire >::iterator wireIter = wireSet.begin(); wireIter != wireSet.end(); wireIter++ )
				{
					ESMoL::Connectable conn = (*wireIter).dstWire_end();
					if ( conn.type() == ESMoL::IChan::meta )
					{
						Udm::Object obj = conn.parent();
						if ( obj.type() == ESMoL::IODevice::meta )
						{
							ESMoL::IODevice iod = ESMoL::IODevice::Cast( obj );
							iodevices.push_back( iod );
						}
					}
				}

				// Collect all of the IODevices connected to the OChan by a Wire (src)
				wireSet = (*ocIter).srcWire();
				for ( set< ESMoL::Wire >::iterator wireIter = wireSet.begin(); wireIter != wireSet.end(); wireIter++ )
				{
					ESMoL::Connectable conn = (*wireIter).srcWire_end();
					if ( conn.type() == ESMoL::IChan::meta )
					{
						Udm::Object obj = conn.parent();
						if ( obj.type() == ESMoL::IODevice::meta )
						{
							ESMoL::IODevice iod = ESMoL::IODevice::Cast( obj );
							iodevices.push_back( iod );
						}
					}
				}

				// Process all of the collected IODevices
				for ( vector< ESMoL::IODevice >::iterator ioIter = iodevices.begin(); ioIter != iodevices.end(); ioIter++ )
				{
					Context iodContext( _contextTracker, (*ioIter).name() );
					(*ioIter).Accept(*this);

					set< ESMoL::PhysConnection > pcSet = (*ioIter).srcPhysConnection();
					set< ESMoL::PhysConnection > pcSet2 = (*ioIter).dstPhysConnection();
					pcSet.insert( pcSet2.begin(), pcSet2.end() );

					for ( set< ESMoL::PhysConnection >::iterator pcIter = pcSet.begin(); pcIter != pcSet.end(); pcIter++ )
					{
						if ( (*pcIter).type() == ESMoL::Plant::meta )
						{
							ESMoL::Plant pl = ESMoL::Plant::Cast( *pcIter );
							Context plContext( _contextTracker, pl.name() );
							pl.Accept( *this );
						}
					}
				}
			}
		}
	}

	// COMPONENT REFS
	vector< ESMoL::ComponentRef > compVector = sys.ComponentRef_children();
	for ( vector< ESMoL::ComponentRef >::iterator crIter = compVector.begin(); crIter != compVector.end(); crIter++ ) {

		Context crContext( _contextTracker, (*crIter).name() );
		(*crIter).Accept( *this ); 

		// COMPONENT DEFINITION
		{
			ESMoL::ComponentRef_RefersTo_Base comprrb = (*crIter).ref(); // we should have already checked this
			ESMoL::Component comp = ESMoL::Component::Cast( comprrb );
			_contextName = string( (*crIter).name() ) + "_" + string( comp.name() ) + "_";
			Context compContext( _contextTracker, comp.name());
			comp.Accept(*this);

			set< ESMoL::MessageRef > mrSet = comp.MessageRef_children();
			for ( set< ESMoL::MessageRef >::iterator mrIter = mrSet.begin(); mrIter != mrSet.end(); mrIter++ )
			{
				// MESSAGE REF
				ESMoL::MessageRef mrf = *mrIter;
				Context mrContext( _contextTracker, mrf.name() );
				_msgRefsLeft.insert( make_pair( mrf, *crIter) );
				_msgDirection.insert( make_pair( mrf, NO_CONN ));  // Default setting
				//cout << "********** Msg Ref **************"  << endl;
				mrf.Accept(*this);

				// MESSAGE TYPE
				{
					ESMoL::MessageRef_RefersTo_Base mrtb = mrf.ref();  // Should have already checked for null refs
					ESMoL::Message msg = ESMoL::Message::Cast( mrtb ); 
					Context msgContext( _contextTracker, msg.name() );
					//cout << "********** Message **************"  << endl;
					msg.Accept(*this);

					// MESSAGE PORTS
					{
						vector< ESMoL::MsgPort > mpVector = msg.MsgPort_children();
						for ( vector< ESMoL::MsgPort >::iterator mpIter = mpVector.begin(); mpIter != mpVector.end(); mpIter++ )
						{
							//cout << "********** MsgPort **************"  << endl;
							Context msgPortContext( _contextTracker, (*mpIter).name() );
							_msgPortsLeft.insert( make_pair( *mpIter, mrf ) );
							(*mpIter).Accept(*this);
						}
					}
				}
			}

			set< ESMoL::ComponentBase > cbSet = comp.ComponentBase_children();
			for ( set< ESMoL::ComponentBase >::iterator cbIter = cbSet.begin(); cbIter != cbSet.end(); cbIter++ )
			{
				ESMoL::ComponentBase cb = (*cbIter);

				if (cb.type() == ESMoL::SubsystemRef::meta) {
					cb = ESMoL::SubsystemRef::Cast(cb).ref();
				} // dereference once, if necessary

				if (cb.type() == ESMoL::CCode::meta) {
					ESMoL::CCode ccode = ESMoL::CCode::Cast(cb);

					if ( ccode ) {  // @todo: push this to the constraint checker, and get rid of the conditional

						Context ccContext( _contextTracker, ccode.name() );
						ccode.Accept( *this );

						vector< ESMoL::CInputPort > vectorInPorts = ccode.CInputPort_children();
						for ( vector< ESMoL::CInputPort >::iterator iip = vectorInPorts.begin(); iip != vectorInPorts.end(); iip++) {
							(*iip).Accept(*this);
						}

						vector< ESMoL::COutputPort > vectorOutPorts = ccode.COutputPort_children();
						for ( vector< ESMoL::COutputPort >::iterator iop = vectorOutPorts.begin(); iop != vectorOutPorts.end(); iop++) {
							(*iop).Accept(*this);
						}
					}
				}
				else if (cb.type() == ESMoL::Subsystem::meta)  {
					ESMoL::Subsystem subSys = ESMoL::Subsystem::Cast(cb);

					if ( subSys ) {

						Context cbContext( _contextTracker, subSys.name() );
						subSys.Accept( *this );

						vector< ESMoL::InputPort > vectorInPorts = subSys.InputPort_kind_children();
						for ( vector< ESMoL::InputPort >::iterator iip = vectorInPorts.begin(); iip != vectorInPorts.end(); iip++)
						{
							(*iip).Accept(*this);
						}

						vector< ESMoL::OutputPort > vectorOutPorts = subSys.OutputPort_kind_children();
						for ( vector< ESMoL::OutputPort >::iterator iop = vectorOutPorts.begin(); iop != vectorOutPorts.end(); iop++)
						{
							(*iop).Accept(*this);
						}
					}
				}
			
			}

			vector< ESMoL::Connector > connVector = comp.Connector_children();
			for ( vector< ESMoL::Connector >::iterator connIter = connVector.begin(); connIter != connVector.end(); connIter++ )
			{
				Context crContext( _contextTracker, idToString( "CN", (*connIter).uniqueId() ) );

				
				ESMoL::Input outp = (*connIter).dstConnector_end();
				if ( outp.type() == ESMoL::MsgPort::meta || outp.type() == ESMoL::AutoCANParam::meta )
				{
					ESMoL::MsgPort mp = ESMoL::MsgPort::Cast( outp );
					_msgPortsLeft.erase( mp );

					ESMoL::MessageRef mref = ESMoL::MessageRef::Cast((*connIter).dstConnector__rp_helper());
					_msgDirection[mref] = OUTPUT_CONN;
				}

				ESMoL::Output inp = (*connIter).srcConnector_end();
				if ( inp.type() == ESMoL::MsgPort::meta || inp.type() == ESMoL::AutoCANParam::meta )
				{
					ESMoL::MsgPort mp = ESMoL::MsgPort::Cast( inp );
					_msgPortsLeft.erase( mp );

					ESMoL::MessageRef mref = ESMoL::MessageRef::Cast((*connIter).srcConnector__rp_helper());
					_msgDirection[mref] = INPUT_CONN;
				}
				(*connIter).Accept( *this );
			}
		}
	}

	// Do local dependencies
	for( set< ESMoL::NodeRef >::iterator nodeRefIter = nodeRefSet.begin(); nodeRefIter != nodeRefSet.end(); nodeRefIter++) 
	{
		ESMoL::NodeRef nodeRef = (*nodeRefIter);
		{
			ESMoL::Node node = nodeRef.ref(); // dangling NodeRefs were checked previously
			Context nodeContext( _contextTracker, node.name() );
				
			Visit_Full_Dependency_Graph( node );
		}
	}

	// DEPENDENCY LINKS
	vector< ESMoL::Dependency > depVector = sys.Dependency_children();
	for ( vector< ESMoL::Dependency >::iterator depIter = depVector.begin(); depIter != depVector.end(); depIter++ )
	{		
		Context depContext( _contextTracker, idToString( "DP", (*depIter).uniqueId() ) ); // @todo: convert the id into a string
		ESMoL::MessageRef smr = (*depIter).srcDependency_end();
		ESMoL::MessageRef dmr = (*depIter).dstDependency_end();

		_msgRefsLeft.erase(smr);
		_msgRefsLeft.erase(dmr);

		_msgDirection[smr] = OUTPUT_CONN;
		_msgDirection[dmr] = INPUT_CONN;

		(*depIter).Accept( *this );
	}

	// COMPONENT ASSIGNMENT LINKS
	vector< ESMoL::ComponentAssignment > caVector = sys.ComponentAssignment_children();
	for ( vector< ESMoL::ComponentAssignment >::iterator caIter = caVector.begin(); caIter != caVector.end(); caIter++ ) {

		Context caContext( _contextTracker, idToString( "CA", (*caIter).uniqueId() ) );
		(*caIter).Accept( *this );
	}

	// COMMMAPPING LINKS
	vector< ESMoL::CommMapping > cmVector = sys.CommMapping_children();
	for ( vector< ESMoL::CommMapping >::iterator cmIter = cmVector.begin(); cmIter != cmVector.end(); cmIter++ ) {

		Context cmContext( _contextTracker, idToString( "CM", (*cmIter).uniqueId() ) );

		// Can't be determined by direction, so we have to check the channel type
		ESMoL::CommMapping_Members_Base secmb = (*cmIter).srcCommMapping_end();
		ESMoL::CommMapping_Members_Base decmb = (*cmIter).dstCommMapping_end();

		ESMoL::MessageRef mr_end;
		if ( secmb.type() == ESMoL::IChan::meta && decmb.type() == ESMoL::MessageRef::meta )
		{
			// this is an input message
			mr_end = ESMoL::MessageRef::Cast( decmb );
			_msgDirection[mr_end] = INPUT_CONN;
		}
		else if ( decmb.type() == ESMoL::IChan::meta && secmb.type() == ESMoL::MessageRef::meta )
		{
			// this is an input message
			mr_end = ESMoL::MessageRef::Cast( secmb );
			_msgDirection[mr_end] = INPUT_CONN;
		}
		else if ( secmb.type() == ESMoL::OChan::meta && decmb.type() == ESMoL::MessageRef::meta )
		{
			// this is an input message
			mr_end = ESMoL::MessageRef::Cast( decmb );
			_msgDirection[mr_end] = OUTPUT_CONN;
		}
		else if ( decmb.type() == ESMoL::OChan::meta && secmb.type() == ESMoL::MessageRef::meta )
		{
			// this is an input message
			mr_end = ESMoL::MessageRef::Cast( secmb );
			_msgDirection[mr_end] = OUTPUT_CONN;
		}

		// if it's connected to a BChan, then it must have a Dependency edge as well to specify its direction.

		_msgRefsLeft.erase(mr_end);
		(*cmIter).Accept( *this );
	}

	// EXECUTION INFO
	set< ESMoL::ExecutionInfo > eiSet = sys.ExecutionInfo_children();
	for ( set< ESMoL::ExecutionInfo >::iterator eiIter = eiSet.begin(); eiIter != eiSet.end(); eiIter++ ) {

		// Resolve the subclass type and visit appropriately
		if ( (*eiIter).type() == ESMoL::TTExecInfo::meta )
		{
			ESMoL::TTExecInfo ttei = ESMoL::TTExecInfo::Cast( (*eiIter) );
			//Context eiContext( _contextTracker, idToString( "TTEI", ttei.uniqueId()) );
			Context eiContext( _contextTracker, string( ttei.name() ) );
			ttei.Accept( *this );
		}
		else if ( (*eiIter).type() == ESMoL::AsyncPeriodicExecInfo::meta )
		{
			ESMoL::AsyncPeriodicExecInfo apei = ESMoL::AsyncPeriodicExecInfo::Cast( (*eiIter) );
			Context eiContext( _contextTracker, idToString( "APEI", apei.uniqueId()) );
			apei.Accept( *this );
		}
		else if ( (*eiIter).type() == ESMoL::SporadicExecInfo::meta )
		{
			ESMoL::SporadicExecInfo spei = ESMoL::SporadicExecInfo::Cast( (*eiIter) );
			Context eiContext( _contextTracker, idToString( "SPEI", spei.uniqueId()) );
			spei.Accept( *this );
		}
	}


	// EXECUTION ASSIGNMENT LINKS
	vector< ESMoL::ExecutionAssignment > eaVector = sys.ExecutionAssignment_children();
	for ( vector< ESMoL::ExecutionAssignment >::iterator eaIter = eaVector.begin(); eaIter != eaVector.end(); eaIter++ ) {

		Context eaContext( _contextTracker, idToString( "EA", (*eaIter).uniqueId() ) );
		(*eaIter).Accept( *this );
	}

	// LEFT OVER MESSAGE REFS
	this->Visit_Unused_MsgRefs();

	// LEFT OVER MESSAGE PORTS
	this->Visit_Unused_MsgPorts();

	Udm::ObjectImpl * imp = sys.__impl();
	Udm::DataNetwork * dn = imp->__getdn();
	dn->CommitEditSequence();

}


void AbstractVisitor::Visit_TimingSheet( const ESMoL::TimingSheet &timingSheet ) {
	// iterate and get all of the constraints and the connected tasks
}


// Function invoked recursively to identify the bus to which a BChan node/port is associated.
//bool AbstractVisitor::GetBus( ESMoL::BChan bchan, set<ESMoL::BChan> &nodesVisited, ESMoL::TTBus &bus ) {
bool AbstractVisitor::GetBus( ESMoL::BChan bchan, set<ESMoL::BChan> & nodesVisited, ESMoL::Bus & bus ) {
	// Source connections
	set<ESMoL::Wire> srcWire = bchan.srcWire();
	for(set<ESMoL::Wire>::iterator srcWireIter = srcWire.begin(); srcWireIter !=srcWire.end(); srcWireIter++) {
		ESMoL::Connectable con = (*srcWireIter).srcWire_end();
		if ( con.type() == ESMoL::TTBus::meta || con.type() == ESMoL::CANBus::meta || con.type() == ESMoL::SerialLink::meta ) {
			bus = ESMoL::Bus::Cast( con );
			return true;
		}
		if ( con.type() == ESMoL::BChan::meta ) {
			ESMoL::BChan nbchan = ESMoL::BChan::Cast(con);
			if ( nodesVisited.find( nbchan ) != nodesVisited.end() ) continue;
			nodesVisited.insert( nbchan );
			this->GetBus( nbchan, nodesVisited, bus );
		}
	}

	// Destination connections
	set< ESMoL::Wire > dstWire = bchan.dstWire();
	for(set< ESMoL::Wire >::iterator dstWireIter = dstWire.begin(); dstWireIter != dstWire.end(); dstWireIter++) {
		ESMoL::Connectable con = (*dstWireIter).dstWire_end();
		if ( con.type() == ESMoL::TTBus::meta || con.type() == ESMoL::CANBus::meta || con.type() == ESMoL::SerialLink::meta  ) {
			bus = ESMoL::Bus::Cast( con );
			return true;
		}
		if ( con.type() == ESMoL::BChan::meta ) {
			ESMoL::BChan nbchan = ESMoL::BChan::Cast( con );
			if ( nodesVisited.find( nbchan ) != nodesVisited.end() ) continue;
			nodesVisited.insert( nbchan );
			this->GetBus( nbchan, nodesVisited, bus );
		}
	}
	return false;
}

void AbstractVisitor::GetNodes( ESMoL::HardwareUnit hwunit, std::set< ESMoL::Node > & collectedNodes )
{
	set< ESMoL::Node > local_nodes = hwunit.Node_kind_children();
	collectedNodes.insert( local_nodes.begin(), local_nodes.end() );

	set< ESMoL::HardwareUnit > child_hwunits = hwunit.HardwareUnit_kind_children();
	for ( set< ESMoL::HardwareUnit >::iterator hwUIter = child_hwunits.begin(); hwUIter != child_hwunits.end(); hwUIter++ )
	{
		GetNodes( *hwUIter, collectedNodes );
	}
}

/***************************************************************************************/

