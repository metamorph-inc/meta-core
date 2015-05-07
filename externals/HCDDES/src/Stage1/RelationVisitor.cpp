#include "Visitor.h"
#include "utils.h"
#include "Uml.h"
#include "ConfigKeeper.h"
#include "IndexKeeper.h"
#include "MessageKeeper.h"
#include "ESMoL.h"

#include "DepGraph.h"

#pragma warning(disable:4996)
#pragma warning(disable:4267)
#pragma warning(disable:4244)


/***************************************************************************************/

const Semantics::Task RelationVisitor::GetTaskFromCompRef( const ESMoL::ComponentRef & cref )
{
	set< ESMoL::ExecutionAssignment > exaSet = cref.srcExecutionAssignment();
	ESMoL::ExecutionAssignment exa = *exaSet.begin(); // should only be 1
	ESMoL::ExecutionInfo exinfo = exa.srcExecutionAssignment_end();

	pair< Semantics::Task, Semantics::ExecInfo > p = 
		IndexKeeper::Inst()->LookupPair< Semantics::Task, Semantics::ExecInfo >( exinfo.uniqueId() );
	return p.first;
}


RelationVisitor::RelationVisitor( Semantics::RootFolder & target ) : 
	AbstractVisitor( target )  {

	_contextTracker.showContext( ConfigKeeper::Inst().GetContextEnabled() );
	std::cout << "Translating model relations..." << std::endl;
}

void RelationVisitor::Visit_System( const ESMoL::System & sys ) {
	Semantics::Deployment sDeployment = IndexKeeper::Inst()->Lookup< Semantics::Deployment >( sys.uniqueId() );
	_currentDeployment = sDeployment;

	DependencyKeeper::Inst()->CloseGraph();
	AbstractVisitor::Visit_System( sys );
}

void RelationVisitor::Visit_Node( const ESMoL::Node & node ) {

	_curENode = node;
	_curSNode = IndexKeeper::Inst()->Lookup< Semantics::Node >( node.uniqueId() );
}

void RelationVisitor::Visit_Full_Dependency_Graph( const ESMoL::Node & node ) {

	//cout << "Visiting dependency graph for node " << node.name() << endl;

	DependencyKeeper * dk = DependencyKeeper::Inst();
	MessageKeeper * mk = MessageKeeper::Inst();
	IndexKeeper * ik = IndexKeeper::Inst();

	std::string nodeName( node.name() );
	ESMoL::ComponentRef srcComp;
	ESMoL::ComponentRef dstComp;

	Semantics::Msg m;

	ID_TYPE src_mref_id, dst_mref_id;

	if ( dk->SelectSubgraph( nodeName ) )
	{
		while ( dk->GetNextDependency( srcComp, src_mref_id, dstComp, dst_mref_id ) )
		{
			//cout << "sc " << srcComp.name() << " dc " << dstComp.name() << endl;

			ID_TYPE sid = MakeCombinedID( srcComp.uniqueId(), src_mref_id );
			ID_TYPE did = MakeCombinedID( dstComp.uniqueId(), dst_mref_id );

			// This is to make sure we don't make extra relations
			if ( _keepCommRels.find(sid) == _keepCommRels.end() )
			{
				// Source end
				Semantics::Msg m;
				if( mk->SeenMessage( srcComp.uniqueId(), src_mref_id ) )
				{
					ID_TYPE mid = mk->GetMessage( srcComp.uniqueId(), src_mref_id );
					m = ik->Lookup< Semantics::Msg >( mid );
				}
				else 
				{
					m = Semantics::Msg::Create( _currentDeployment );
					ostringstream localMsgName;
					localMsgName << srcComp.name() << "_" << src_mref_id << "_";
					localMsgName << dstComp.name() << "_" << dst_mref_id;
					m.name() = localMsgName.str();
					m.sizebytes() = 0;
					m.messagetype() = string( "Dummy" );
					mk->AddMessage( srcComp.uniqueId(), src_mref_id, sid );
				}

				//cout << " msg " << m.name() << endl;

				Semantics::LocalDependency ld = Semantics::LocalDependency::Create( _currentDeployment );
				ld.depMsg() = m;
				m.msgDep() = ld;

				Semantics::Task st = GetTaskFromCompRef( srcComp );
				ld.localDepSendingTask() = st;
				st.localSendingTaskDeps() += ld;
				
				Semantics::Node n = ik->Lookup< Semantics::Node >( node.uniqueId() );
				n.nodeLocalDeps() += ld;
				ld.localDepNode() = n;

				//cout << "For sending task " << st.name() << endl;
				std::map< unsigned long long, Semantics::Signal > sendSigs;
				
				// We only want the signals from st that are carried by the message in this local
				// dependency
				set< Semantics::Signal > sigs = m.carries();
				for ( set< Semantics::Signal >::iterator sigIter = sigs.begin(); sigIter != sigs.end(); sigIter++ )
				{
					Semantics::Component c = (*sigIter).generatedBy();
					if ( c != Udm::null )
					{
						Semantics::Task t = c.invokedBy();
						if ( t.uniqueId() == st.uniqueId() )
						{
							//cout << "   added signal " << (*sigIter).name() << " w msgIndex " << (*sigIter).msgIndex() << endl;
							sendSigs[ (*sigIter).msgIndex() ] = *sigIter;
						}
					}
				}

				_sigMap[ ld.uniqueId() ] = sendSigs;
				
				//cout << "Created local dependency " << ld.uniqueId() <<  " src " << st.name();
				_keepCommRels[ sid ] = Semantics::CommRelations::Cast(ld);
			}

			if ( _keepCommRels.find(did) == _keepCommRels.end() )
			{
				// Destination end
				map< unsigned long long, Semantics::CommRelations >::iterator pic = _keepCommRels.find( sid );		
				Semantics::CommRelations crel = pic->second;

				Semantics::LocalDependency ld = Semantics::LocalDependency::Cast( crel );
				Semantics::Task rt = GetTaskFromCompRef( dstComp );
				ld.localDepReceivingTasks() += rt;
				rt.localReceivingTaskDeps() += ld;

				Semantics::Task st = ld.localDepSendingTask();
				Semantics::Msg m = ld.depMsg();

				//cout << "For receiving task " << rt.name() << " and msg " << m.name() << endl;

				// We only want the signals from rt that are carried by the message in this local
				// dependency
				set< Semantics::Signal > sigs = m.carries();
				for ( set< Semantics::Signal >::iterator sigIter = sigs.begin(); sigIter != sigs.end(); sigIter++ )
				{
					// We want to ignore signals from the sending side
					Semantics::Component scomp = (*sigIter).generatedBy();
					if ( scomp != Udm::null )
						continue;

					// ... and modify/delete signals from the receive side
					set< Semantics::Component > comps = (*sigIter).consumedBy();
					for ( set< Semantics::Component >::iterator c = comps.begin(); c != comps.end(); c++ )
					{
						
						Semantics::Task t = (*c).invokedBy();
						if ( t.uniqueId() == rt.uniqueId() )
						{
							std::map< unsigned long long, Semantics::Signal >::iterator sigLookup;
							sigLookup = _sigMap[ ld.uniqueId() ].find( (*sigIter).msgIndex() );
							if ( sigLookup != _sigMap[ ld.uniqueId() ].end() )
							{
								//cout << "   Removing signal " << (*sigIter).name() << " w msgIndex " << (*sigIter).msgIndex() << endl;
								//cout << "   Wiring in signal " << (sigLookup->second).name() << " w msgIndex " << (sigLookup->second).name() << endl;

								// Add the sender's copy of the signal to the receiving component
								/*(sigLookup->second).consumedBy() += (*c);
								(*c).consumes() += (sigLookup->second);

								// Remove the receiver's copy of the signal
								(*c).consumes() -= (*sigIter);
								(*sigIter).consumedBy() -= (*c);

								m.carries() -= (*sigIter);
								(*sigIter).carriedBy() -= m;

								Semantics::SignalType st = (*sigIter).isOfType();
								st.signalsOfType() -= (*sigIter);
								(*sigIter).isOfType() = Semantics::SignalType::Cast(Udm::null);
								

								(*sigIter).parent() = Semantics::Deployment::Cast(Udm::null);*/

								// Instead of all that trouble, simply set the name of the receive
								// signals to be the same as the sending signals
								std::string sendname = (sigLookup->second).name();
								(*sigIter).name() = sendname;
							}
						}
					}
				}

				//cout << "Created local dependency " << ld.uniqueId() <<  " dst " << rt.name();
				_keepCommRels[ did ] = Semantics::CommRelations::Cast(ld);
			}
		}
	}
}

void RelationVisitor::Visit_BChan( const ESMoL::BChan &eBChan ) {

	std::pair< Semantics::CommInterface, Semantics::BChan > sPair =
		IndexKeeper::Inst()->LookupPair< Semantics::CommInterface, Semantics::BChan >( eBChan.uniqueId() );
	Semantics::CommInterface commInterface = sPair.first;
	Semantics::BChan sBChan = sPair.second;

	// Associate interface with node
	_curSNode.integrates() += commInterface;
	commInterface.integratedBy() = _curSNode;

	// Associate channel with interface
	commInterface.commChannels() += sBChan;
	sBChan.chanInterface() = commInterface;

	// Store context for later in traversal
	_curSBChan = sBChan;
	_curSCommIface = commInterface;
}

void RelationVisitor::Visit_TTBus( const ESMoL::TTBus & eTTBus ) {	

	Semantics::TTBus sTTBus = IndexKeeper::Inst()->Lookup< Semantics::TTBus >( eTTBus.uniqueId() );

	// Link interface to bus
	_curSCommIface.commMedium() = sTTBus;
	sTTBus.interfaces() += _curSCommIface;
}

void RelationVisitor::Visit_CANBus( const ESMoL::CANBus & eCANBus ) {	

	Semantics::CANBus sCANBus = IndexKeeper::Inst()->Lookup< Semantics::CANBus >( eCANBus.uniqueId() );

	// Link interface to bus
	_curSCommIface.commMedium() = sCANBus;
	sCANBus.interfaces() += _curSCommIface;
}

void RelationVisitor::Visit_SerialLink( const ESMoL::SerialLink & eSL )
{
	Semantics::SerialLink sSLink = IndexKeeper::Inst()->Lookup< Semantics::SerialLink >( eSL.uniqueId() );

	// Link interface to bus
	_curSCommIface.commMedium() = sSLink;
	sSLink.interfaces() += _curSCommIface;
}

void RelationVisitor::Visit_IChan( const ESMoL::IChan & eIChan ) {

	Semantics::IChan sIChan = IndexKeeper::Inst()->Lookup< Semantics::IChan >( eIChan.uniqueId() );
	_curSIChan = sIChan;
}

void RelationVisitor::Visit_OChan( const ESMoL::OChan & eOChan ) {

	Semantics::OChan sOChan = IndexKeeper::Inst()->Lookup< Semantics::OChan >( eOChan.uniqueId() );
	_curSOChan = sOChan;
}

void RelationVisitor::Visit_IODevice(const ESMoL::IODevice & iod) {

	Semantics::Device dev = IndexKeeper::Inst()->Lookup< Semantics::Device >( iod.uniqueId() );

	if ( dev.type() == Semantics::InputDevice::meta )
	{
		Semantics::InputDevice inputDevice = Semantics::InputDevice::Cast( dev );

		// Associate device with node
		_curSNode.integrates() += inputDevice;
		inputDevice.integratedBy() = _curSNode;

		// Associate channel with device
		inputDevice.inputChannels() += _curSIChan;
		_curSIChan.ichanDevice() = inputDevice;

		_curSIDev = inputDevice;
		_curSODev = Semantics::OutputDevice::Cast(Udm::null);

		//cout << "Linked input device " << iod.name() << " to node " << _curSNode.name();
		//cout << " and channel " << _curSIChan.name() << endl;
	}
	else if ( dev.type() == Semantics::OutputDevice::meta )
	{
		Semantics::OutputDevice outputDevice = Semantics::OutputDevice::Cast( dev );

		// Associate device with node
		_curSNode.integrates() += outputDevice;
		outputDevice.integratedBy() = _curSNode;

		// Associate channel with device
		outputDevice.outputChannels() += _curSOChan;
		_curSOChan.ochanDevice() = outputDevice;

		_curSIDev = Semantics::InputDevice::Cast(Udm::null);
		_curSODev = outputDevice;

		//cout << "Linked output device " << iod.name() << " to node " << _curSNode.name();
		//cout << " and channel " << _curSOChan.name() << endl;
	}
}

void RelationVisitor::Visit_Plant(const ESMoL::Plant & plant) {

	Semantics::Plant p = IndexKeeper::Inst()->Lookup< Semantics::Plant >( plant.uniqueId() );
	if ( _curSIDev )
	{
		// Associate device with plant
		_curSIDev.inputEnvironment() = p;
		p.acquisitionDevices() += _curSIDev;
	}
	else if ( _curSODev )
	{
		// Associate device with plant
		_curSODev.outputEnvironment() = p;
		p.actuationDevices() += _curSODev;
	}

	_curSIDev = Semantics::InputDevice::Cast(Udm::null);
	_curSODev = Semantics::OutputDevice::Cast(Udm::null);
}

void RelationVisitor::Visit_ComponentRef( const ESMoL::ComponentRef & eCompRef ) {

	// @todo remove debug prints
	//cout << " CompRef " << eCompRef.uniqueId() << endl;
	_curSTask = GetTaskFromCompRef( eCompRef );

	// @todo This conditional is for debugging only -- do NOT leave it here.
	/*cout << " CompRef " << eCompRef.uniqueId() << endl;
	if ( ! IndexKeeper::Inst()->SeenIndex( eCompRef.uniqueId() ) )
	{
		cout << "We have not seen this one before!" << endl;
	}
	else
	{*/

	// Also look up the attached processor node
	set< ESMoL::ComponentAssignment > procs = eCompRef.dstComponentAssignment(); // should be one-to-one
	ESMoL::ComponentAssignment ca = *procs.begin();
	ESMoL::NodeRef nr = ca.dstComponentAssignment_end();
	ESMoL::Node node = nr.ref();

	_curSNode = IndexKeeper::Inst()->Lookup< Semantics::Node >( node.uniqueId() );

	Semantics::Component c = IndexKeeper::Inst()->Lookup< Semantics::Component >( eCompRef.uniqueId() );

	_curSTask.invokes() += c;
	c.invokedBy() = _curSTask;
	_curSComp = c;
	
	/*}*/

	_curECompRef = eCompRef;
}

void RelationVisitor::Visit_Component( const ESMoL::Component & component ) {

}

// Hold that thought
void RelationVisitor::Visit_MessageRef( const ESMoL::MessageRef & messageRef ) {

	MessageKeeper * mk = MessageKeeper::Inst();
	if ( mk->SeenMessage( _curECompRef.uniqueId(), messageRef.uniqueId() ) ) {
	
		ID_TYPE mid = mk->GetMessage( _curECompRef.uniqueId(), messageRef.uniqueId() );
		_curSMsg = IndexKeeper::Inst()->Lookup< Semantics::Msg >( mid );
	
		// Connect message with msg type
		ESMoL::MessageRef_RefersTo_Base mrrtb = messageRef.ref();
		ESMoL::Message msg = ESMoL::Message::Cast( mrrtb );

		IndexKeeper * ik = IndexKeeper::Inst();
		if ( ik->SeenIndex( msg.uniqueId() ) )
		{
			Semantics::MsgType mt = ik->Lookup< Semantics::MsgType >( msg.uniqueId() );
			_curSMsg.msgType() = mt;
			mt.msgTypeInstances() += _curSMsg;
		}

	}
	
}

void RelationVisitor::Visit_Message(const ESMoL::Message &) {

}

void RelationVisitor::Visit_MsgPort(const ESMoL::MsgPort &) { 

}

void RelationVisitor::Visit_CCode( const ESMoL::CCode & ccode ) {

}

void RelationVisitor::Visit_CInputPort( const ESMoL::CInputPort &cInputPort ) {

	std::pair< Semantics::Signal, Semantics::SignalType > sPair 
		= IndexKeeper::Inst()->LookupPair< Semantics::Signal, Semantics::SignalType >( MakeCombinedID(_curECompRef.uniqueId(), cInputPort.uniqueId() ));
	Semantics::Signal signal = sPair.first;
	Semantics::SignalType signalType = sPair.second;
	signal.isOfType() = signalType;
	signalType.signalsOfType() += signal;

	_curSComp.consumes() += signal;
	signal.consumedBy() += _curSComp;
}

void RelationVisitor::Visit_COutputPort( const ESMoL::COutputPort & cOutputPort ) {

	// First, find the signal for this port
	std::pair< Semantics::Signal, Semantics::SignalType > sPair 
		= IndexKeeper::Inst()->LookupPair< Semantics::Signal, Semantics::SignalType >( MakeCombinedID(_curECompRef.uniqueId(), cOutputPort.uniqueId() ));
	Semantics::Signal signal = sPair.first;
	Semantics::SignalType signalType = sPair.second;
	signal.isOfType() = signalType;
	signalType.signalsOfType() += signal;

	_curSComp.generates() += signal;
	signal.generatedBy() = _curSComp;

	// Next check to see whether it is unconnected, and create a dummy message if necessary
	Semantics::Msg dummsg;
	set< ESMoL::Connector > oconn = cOutputPort.dstConnector();
	if ( oconn.size() == 0 )
	{
		// Figure out whether the attached node already has a dummy message
		dummsg = _curSNode.nodeDummyMsg();

		// if not, create one and attach it
		if ( dummsg == Udm::null )
		{
			dummsg = Semantics::Msg::Create( _currentDeployment );
			ostringstream msgname;
			msgname << _curSNode.name() << ".DummyMsg";
			dummsg.name() = msgname.str();
			dummsg.messagetype() = "Dummy";

			_curSNode.nodeDummyMsg() = dummsg;
			dummsg.dummyMsgNode() = _curSNode;

			Semantics::MsgType dmt = Semantics::MsgType::Create( _currentDeployment );
			dummsg.msgType() = dmt;
			dmt.msgTypeInstances() += dummsg;

			ostringstream msgtypename;
			msgtypename << _curSNode.name() << "_DummyMsg_type";
			dmt.name() = msgtypename.str();
			dmt.id() = 0;
		}

		// Create a dummy dependency and attach it
		Semantics::DummyDependency dumdep = Semantics::DummyDependency::Create( _currentDeployment );
		dumdep.dummyDepNode() = _curSNode;
		_curSNode.nodeDummyDeps() += dumdep;

		dumdep.dummyMsg() = dummsg;
		dummsg.dummyMsgDeps() += dumdep;

		dumdep.sendingTask() = _curSTask;
		_curSTask.dummyDeps() += dumdep;

		// Attach the signal to the dummy message
		set< Semantics::Signal > existing_sigs = dummsg.carries();
		signal.msgIndex() = existing_sigs.size();
		dummsg.carries() += signal;
		signal.carriedBy() += dummsg;

		// Create a new field in the message type object
		Semantics::MsgType dummt = dummsg.msgType();
		Semantics::MsgField dfield = Semantics::MsgField::Create( _currentDeployment );
		dfield.name() = signal.name();
		dfield.msgindex() = existing_sigs.size();
		// FIXME: How can we get the number of bits for the dummy message data type?
		dummt.msgFields() += dfield;
		dfield.fieldMsg() = dummt;
		
		dfield.implementedBy() += signal;
		signal.implements() = dfield;
	}

}

void RelationVisitor::Visit_SubsystemRef(const ESMoL::SubsystemRef &) {

}

void RelationVisitor::Visit_Subsystem( const ESMoL::Subsystem & subsys ) {
	
}

void RelationVisitor::Visit_InputPort( const ESMoL::InputPort & inputPort ) {

	std::pair< Semantics::Signal, Semantics::SignalType > sPair 
		= IndexKeeper::Inst()->LookupPair< Semantics::Signal, Semantics::SignalType >( MakeCombinedID(_curECompRef.uniqueId(), inputPort.uniqueId() ));
	Semantics::Signal signal = sPair.first;
	Semantics::SignalType signalType = sPair.second;
	signal.isOfType() = signalType;
	signalType.signalsOfType() += signal;

	_curSComp.consumes() += signal;
	signal.consumedBy() += _curSComp;
}

void RelationVisitor::Visit_OutputPort( const ESMoL::OutputPort & outputPort ) {

	// First look up the signal for this port
	std::pair< Semantics::Signal, Semantics::SignalType > sPair 
		= IndexKeeper::Inst()->LookupPair< Semantics::Signal, Semantics::SignalType >( MakeCombinedID(_curECompRef.uniqueId(), outputPort.uniqueId() ));
	Semantics::Signal signal = sPair.first;
	Semantics::SignalType signalType = sPair.second;
	signal.isOfType() = signalType;
	signalType.signalsOfType() += signal;

	_curSComp.generates() += signal;
	signal.generatedBy() = _curSComp;

	// Next check to see whether it is unconnected, and create a dummy message if necessary
	Semantics::Msg dummsg;
	set< ESMoL::Connector > oconn = outputPort.dstConnector();
	if ( oconn.size() == 0 )
	{
		// Figure out whether the attached node already has a dummy message
		dummsg = _curSNode.nodeDummyMsg();

		// if not, create one and attach it
		if ( dummsg == Udm::null )
		{
			dummsg = Semantics::Msg::Create( _currentDeployment );
			ostringstream msgname;
			msgname << _curSNode.name() << ".DummyMsg";
			dummsg.name() = msgname.str();
			dummsg.messagetype() = "Dummy";

			_curSNode.nodeDummyMsg() = dummsg;
			dummsg.dummyMsgNode() = _curSNode;
		}

		// Create a dummy dependency and attach it
		Semantics::DummyDependency dumdep = Semantics::DummyDependency::Create( _currentDeployment );
		dumdep.dummyDepNode() = _curSNode;
		_curSNode.nodeDummyDeps() += dumdep;

		dumdep.dummyMsg() = dummsg;
		dummsg.dummyMsgDeps() += dumdep;

		dumdep.sendingTask() = _curSTask;
		_curSTask.dummyDeps() += dumdep;

		// Attach the signal to the dummy message
		set< Semantics::Signal > existing_sigs = dummsg.carries();
		signal.msgIndex() = existing_sigs.size();
		dummsg.carries() += signal;
		signal.carriedBy() += dummsg;
	}


}

void RelationVisitor::Visit_Connector( const ESMoL::Connector & conn ) {

	IndexKeeper * ik = IndexKeeper::Inst();
	MessageKeeper * mk = MessageKeeper::Inst();

	ESMoL::Output outp = conn.srcConnector_end();
	ESMoL::Input inp = conn.dstConnector_end();

	ESMoL::Message emsg;
	ESMoL::MessageRef emsgref;

	Semantics::Msg m;
	Semantics::Signal s;

	std::pair< Semantics::Signal, Semantics::SignalType > sPair;

	if ( outp.type() == ESMoL::MsgPort::meta || outp.type() == ESMoL::AutoCANParam::meta ) // input must be the port end
	{
		//cout << "Following reference port on the output side... (srcConnector)" << endl;
		emsgref = ESMoL::MessageRef::Cast(conn.srcConnector__rp_helper()); // get the refport parent
		sPair = ik->LookupPair< Semantics::Signal, Semantics::SignalType >( MakeCombinedID(_curECompRef.uniqueId(), inp.uniqueId() ) );
		s = sPair.first;
	}
	else if ( outp.type() == ESMoL::MessageRef::meta )
	{
		//cout << "Getting message ref on the output side... (srcConnector)" << endl;
		emsgref = ESMoL::MessageRef::Cast( outp );
		sPair = ik->LookupPair< Semantics::Signal, Semantics::SignalType >( MakeCombinedID(_curECompRef.uniqueId(), inp.uniqueId() ));
		s = sPair.first;
	}
	else if ( inp.type() == ESMoL::MsgPort::meta || inp.type() == ESMoL::AutoCANParam::meta )
	{
		//cout << "Following reference port on the input side... (dstConnector)" << endl;
		emsgref = ESMoL::MessageRef::Cast(conn.dstConnector__rp_helper()); // get the refport parent
		sPair = ik->LookupPair< Semantics::Signal, Semantics::SignalType >( MakeCombinedID(_curECompRef.uniqueId(), outp.uniqueId() ));
		s = sPair.first;
	}
	else if ( inp.type() == ESMoL::MessageRef::meta )
	{
		//cout << "Getting message ref on the input side... (dstConnector)" << endl;
		emsgref = ESMoL::MessageRef::Cast( inp );
		sPair = ik->LookupPair< Semantics::Signal, Semantics::SignalType >( MakeCombinedID(_curECompRef.uniqueId(), outp.uniqueId() ));
		s = sPair.first;
	}

	// Because of the N:1 relationship between ref pairs and messages, we have to use the message keeper
	ID_TYPE mid = mk->GetMessage( _curECompRef.uniqueId(), emsgref.uniqueId() );

	try {

		m = ik->Lookup< Semantics::Msg >( mid );
		m.carries() += s;
		s.carriedBy() += m;

		Semantics::MsgType mt = m.msgType();
		ID_TYPE midx = s.msgIndex();
		set< Semantics::MsgField > fields = mt.msgFields();
		for ( set< Semantics::MsgField >::iterator fieldIter = fields.begin(); fieldIter != fields.end(); fieldIter++ )
		{
			if ( (*fieldIter).msgindex() == midx )
			{
				s.implements() = (*fieldIter);
				(*fieldIter).implementedBy() += s;
				break;
			}
		}

	} catch ( LookupErr & ) {

		std::ostringstream errmsg;
		errmsg << "Msg lookup failed for component " << _curECompRef.name();
		errmsg << " and msg ref " << emsgref.name() << ".";
		postError( errmsg.str() );
	}

}

void RelationVisitor::Visit_ComponentAssignment( const ESMoL::ComponentAssignment & ca ) {

	ESMoL::NodeRef nr = ca.dstComponentAssignment_end();
	ESMoL::ComponentRef cr = ca.srcComponentAssignment_end();
	ESMoL::Node nd = nr.ref(); // already checked for null-ness

	// We should only associate this pair once
	if ( _uniqueMapping.find( make_pair( nd.uniqueId(), cr.uniqueId() ) ) == _uniqueMapping.end() )
	{
		IndexKeeper * ik = IndexKeeper::Inst();
		Semantics::Task t = GetTaskFromCompRef( cr );
		Semantics::Node n = ik->Lookup< Semantics::Node >( nd.uniqueId() );
		t.executedBy() = n;
		n.executes() += t;

		_uniqueMapping.insert( make_pair( nd.uniqueId(), cr.uniqueId() ) );
	}
}

void RelationVisitor::Visit_Dependency( const ESMoL::Dependency & dep ) { 

	MessageKeeper * mk = MessageKeeper::Inst();
	IndexKeeper * ik = IndexKeeper::Inst();

	ESMoL::MessageRef smr = dep.srcDependency_end();
	ESMoL::MessageRef dmr = dep.dstDependency_end();

	ESMoL::ComponentRef scr = dep.srcDependency__rp_helper();
	ESMoL::ComponentRef dcr = dep.dstDependency__rp_helper();

	ID_TYPE sid = MakeCombinedID( scr.uniqueId(), smr.uniqueId() );
	ID_TYPE did = MakeCombinedID( dcr.uniqueId(), dmr.uniqueId() );

	// Two cases: both tasks in the same context, or not
	// if so, create a local dependency
	set< ESMoL::ComponentAssignment > snrcas = scr.dstComponentAssignment();
	ESMoL::ComponentAssignment snrca = *(snrcas.begin());
	ESMoL::NodeRef snr = snrca.dstComponentAssignment_end();
	ESMoL::Node snode = snr.ref();

	set< ESMoL::ComponentAssignment > dnrcas = dcr.dstComponentAssignment();
	ESMoL::ComponentAssignment dnrca = *(dnrcas.begin());
	ESMoL::NodeRef dnr = dnrca.dstComponentAssignment_end();
	ESMoL::Node dnode = dnr.ref();

	// @todo Remove debug prints
	//cout << "Dep src comp " << scr.name() << " par nr ";
	//cout << snr.uniqueId() << "/nd " << snode.uniqueId() << " dst comp " << dcr.name();
	//cout << " par nr " << dnr.uniqueId() << "/nd " << dnode.uniqueId() << endl;

	// If we're sending remotely
	if ( snode.uniqueId() != dnode.uniqueId() )
	{
		if ( _keepCommRels.find(sid) == _keepCommRels.end() )
		{
			// We need to make a new transmission object
			ID_TYPE mid = mk->GetMessage( scr.uniqueId(), smr.uniqueId() );
			Semantics::Msg m = ik->Lookup< Semantics::Msg >( mid );

			Semantics::Transmits tr = Semantics::Transmits::Create( _currentDeployment );
			tr.transmittedMsg() = m;
			m.msgTransmitter() = tr;

			_keepCommRels[ sid ] = Semantics::CommRelations::Cast(tr);
		}

		if ( _keepCommRels.find( did ) == _keepCommRels.end() )
		{
			// We need to make a new receiver object
			ID_TYPE mid = mk->GetMessage( dcr.uniqueId(), dmr.uniqueId() );
			Semantics::Msg m = ik->Lookup< Semantics::Msg >( mid );

			Semantics::Receives rc = Semantics::Receives::Create( _currentDeployment );
			rc.receivedMsg() = m;
			m.msgListeners() += rc;

			_keepCommRels[ did ] = Semantics::CommRelations::Cast(rc);
		}
	}
}


void RelationVisitor::Visit_CommMapping( const ESMoL::CommMapping & ecomm ) {

	IndexKeeper * ik = IndexKeeper::Inst();
	MessageKeeper * mk = MessageKeeper::Inst();

	ESMoL::CommMapping_Members_Base secmb = ecomm.srcCommMapping_end();
	ESMoL::CommMapping_Members_Base decmb = ecomm.dstCommMapping_end();

	ESMoL::MessageRef mr_end;
	ESMoL::Channel chan_end;
	ESMoL::ComponentRef cref;
	ESMoL::Node node;

	if ( secmb.type() == ESMoL::MessageRef::meta )
	{
		mr_end = ESMoL::MessageRef::Cast( secmb );
		chan_end = ESMoL::Channel::Cast( decmb );
		cref = ESMoL::ComponentRef::Cast(ecomm.srcCommMapping__rp_helper());
	}
	else
	{
		mr_end = ESMoL::MessageRef::Cast( decmb );
		chan_end = ESMoL::Channel::Cast( secmb );
		cref = ESMoL::ComponentRef::Cast(ecomm.dstCommMapping__rp_helper());
	}

	node = ESMoL::Node::Cast( chan_end.parent() );

	Semantics::Task t = GetTaskFromCompRef( cref );

	// @todo remove debug prints
	//cout << "Node " << node.name() << " (" << node.uniqueId() << ") ";
	//cout << "Chan " << chan_end.name() << " (" << chan_end.uniqueId() << ") ";
	//cout << "CRef " << cref.name() << " (" << cref.uniqueId() << ") ";
	//cout << "MRef " << mr_end.name() << " (" << mr_end.uniqueId() << ") " << endl;

	// We should only associate this node/task pair once
	if ( _uniqueMapping.find( make_pair( node.uniqueId(), cref.uniqueId() ) ) == _uniqueMapping.end() )
	{
		Semantics::Node n = ik->Lookup< Semantics::Node >( node.uniqueId() );
		t.executedBy() = n;
		n.executes() += t;

		_uniqueMapping.insert( make_pair( node.uniqueId(), cref.uniqueId() ) );
	}
	
	// Find the right message
	ID_TYPE mid = mk->GetMessage( cref.uniqueId(), mr_end.uniqueId() );
	if ( mid ) // if this port had no dependencies, then it sends no messages
	{
		Semantics::Msg m = ik->Lookup< Semantics::Msg >( mid );
		
		// Now do the proper relations
		if ( chan_end.type() == ESMoL::BChan::meta )
		{
			ID_TYPE crid = MakeCombinedID( cref.uniqueId(), mr_end.uniqueId() );
			map< unsigned long long, Semantics::CommRelations >::iterator pic = _keepCommRels.find( crid );		
			Semantics::CommRelations cr = pic->second;

			
			pair< Semantics::CommInterface, Semantics::BChan > p = ik->LookupPair< Semantics::CommInterface, Semantics::BChan >( chan_end.uniqueId() );
			Semantics::CommInterface ci = p.first;
			Semantics::BChan bc = p.second;

			if ( cr.type() == Semantics::Receives::meta )
			{
				// component is the receiver
				Semantics::Receives rc = Semantics::Receives::Cast( cr );

				rc.receivingTask() = t;
				t.incomingData() += rc;

				rc.receivingChannel() = bc;
				bc.channelListeners() += rc;
			}
			else
			{
				// component is the sender
				Semantics::Transmits tr = Semantics::Transmits::Cast( cr );

				tr.sendingTask() = t;
				t.outboundData() += tr;

				tr.sendingChan() = bc;
				bc.channelTransmitters() += tr;
			}
		}
		else if ( chan_end.type() == ESMoL::IChan::meta )
		{
			Semantics::IChan ic = ik->Lookup< Semantics::IChan >( chan_end.uniqueId() );
			// Create an acquisition relation
			Semantics::Acquisition acq = ic.channelAcquisition();
			
			if ( acq == Udm::null ) 
			{
				acq = Semantics::Acquisition::Create( _currentDeployment );
			
				acq.acquiringChannel() = ic;
				ic.channelAcquisition() = acq;

				acq.acquiredMsg() = m;
				m.msgAcquisitions() += acq;
			}

			acq.acquiringTasks() += t;
			t.taskAcquisitions() += acq;
		}
		else if ( chan_end.type() == ESMoL::OChan::meta )
		{
			Semantics::OChan oc = ik->Lookup< Semantics::OChan >( chan_end.uniqueId() );
			// Create an actuation relation
			Semantics::Actuation act = Semantics::Actuation::Create( _currentDeployment );

			act.actuatingTask() = t;
			t.taskActuations() += act;

			act.actuatingMsg() = m;
			m.msgActuation() = act;

			act.actuatingChannel() = oc;
			oc.channelActuations() += act;
		}
	}
}

void RelationVisitor::Visit_ExecutionAssignment( const ESMoL::ExecutionAssignment & exa ) {

	IndexKeeper * ik = IndexKeeper::Inst();
	ESMoL::ExecutionInfo ei = exa.srcExecutionAssignment_end();
	ESMoL::Executable ex = exa.dstExecutionAssignment_end();

	//cout << "Visiting ExecAssignment";

	Semantics::ExecInfo sei = ik->Lookup< Semantics::ExecInfo >( ei.uniqueId() );
	if ( ex.type() == ESMoL::ComponentRef::meta )
	{
		pair< Semantics::Task, Semantics::ExecInfo > p = ik->LookupPair< Semantics::Task, Semantics::ExecInfo >( ei.uniqueId() );
		Semantics::Task t = p.first;

		sei.executor() += t;
		t.info() += sei;

		//cout << " it was a component: " << ex.name() << std::endl;
	}
	else if ( ex.type() == ESMoL::MessageRef::meta )
	{
		//cout << " it was a message ref: " << ex.name() << std::endl;
		ESMoL::ComponentRef cref = ESMoL::ComponentRef::Cast(exa.dstExecutionAssignment__rp_helper());
		ID_TYPE mid = MessageKeeper::Inst()->GetMessage(cref.uniqueId(), ex.uniqueId());
		Semantics::Msg m = ik->Lookup< Semantics::Msg >( mid );

		sei.executor() += m;
		m.info() += sei;

		
		// Also assign this execution model to attached devices (if any)
		set< Semantics::Acquisition > acqs = m.msgAcquisitions();
		for ( set< Semantics::Acquisition >::iterator acqIter = acqs.begin(); acqIter != acqs.end(); acqIter++ )
		{
			// for now, the channel and device all get the same execution model
			Semantics::IChan ic = (*acqIter).acquiringChannel();
			Semantics::InputDevice id = ic.ichanDevice();

			//cout << "Acquisition relation object -- IChan " << ic.name() << " InputDevice " << id.name();
			//cout << " ExecInfo " << sei.name() << endl;

			sei.executor() += ic;
			ASSERT(id);
			sei.executor() += id;
			ic.info() += sei;
			id.info() += sei;
		}

		Semantics::Actuation act = m.msgActuation();
		if ( act != Udm::null )
		{
			Semantics::OChan oc = act.actuatingChannel();
			Semantics::OutputDevice od = oc.ochanDevice();
			sei.executor() += oc;
			sei.executor() += od;
			oc.info() += sei;
			od.info() += sei;
		}
		
	}
	else if ( ex.type() == ESMoL::Channel::meta )
	{
		// Then we're not sure what to do with this yet...
		//cout << " it was a channel: " << ex.name() << std::endl;
	}
	else
	{
		//cout << " it was something unspecified :-(" << endl;
	}
}

void RelationVisitor::Visit_TTExecInfo(const ESMoL::TTExecInfo &) {

}


/***************************************************************************************/

