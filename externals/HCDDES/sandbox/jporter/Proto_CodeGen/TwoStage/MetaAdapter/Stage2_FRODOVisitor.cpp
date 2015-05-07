#include "Stage2_VisitorFactory.h"
#include "Stage2_FRODOVisitor.h"

#include "Uml.h"

bool Stage2_FRODOVisitor::_initialized =
 Stage2_VisitorFactory::registerInstantiator(  string( "FRODO" ), &Stage2_FRODOVisitor::getFRODOVisitor  );

Stage2_FRODOVisitor::Stage2_FRODOVisitor( Semantics::RootFolder & rf, timeStamp & ts ) : 
 Stage2_AbstractVisitor( rf, ts ) 
{  
}

Stage2_AbstractVisitor * getFRODOVisitor( Semantics::RootFolder & rf, timeStamp & ts ) { 
	return new Stage2_FRODOVisitor( rf, ts );
}

void Stage2_FRODOVisitor::Visit_RootFolder( const Semantics::RootFolder &rootFolder ) {

	DeploymentSet deploymentSet = rootFolder.Deployment_children();
	for( DeploymentSet::iterator dpsItr = deploymentSet.begin() ; dpsItr != deploymentSet.end() ; (void)++dpsItr ) {
		Visit_Deployment( *dpsItr );
	}

}

void Stage2_FRODOVisitor::Visit_Deployment( const Semantics::Deployment &deployment ) {

	NodeSet nodeSet = deployment.Node_kind_children();
	for( NodeSet::iterator ndsItr = nodeSet.begin() ; ndsItr != nodeSet.end() ; (void)++ndsItr ) {
		Visit_Node( *ndsItr );
	}

}


void Stage2_FRODOVisitor::Visit_Node( const Semantics::Node &node ) {

	google::Template *googleTemplate = google::Template::GetTemplate( "FRODO.tpl", google::DO_NOT_STRIP );

	clearDictionary();
	_ttTaskScheduleMap.clear();
	_qualifiedTTMsgScheduleMap.clear();

	_platformType = node.platformtype();

	if ( _platformType == "PXA" ) {
		addSectionDictionary( "PXA_INCLUDE" );
		popSectionDictionary();
	} else if ( _platformType == "ATMega128" ) {
		addSectionDictionary( "AVR_INCLUDE" );
		popSectionDictionary();
	}

	getTemplateDictionary().SetValue(
	 "NODE_hyperperiod", boost::lexical_cast< std::string >(  static_cast< int >( node.hyperperiodsecs() * 1000 )  )
	);

	// PROCESS TASKS
	TaskSet taskSet = node.executes();
	for( TaskSet::iterator tksItr = taskSet.begin() ; tksItr != taskSet.end() ; ++tksItr ) {
		if ( tksItr->type() != Semantics::TTTask::meta ) continue;
		Visit_TTTask(  Semantics::TTTask::Cast( *tksItr )  );
	}

	processTTTaskScheduleMap();
	processQualifiedTTMsgScheduleMap();

 	std::string output;
	googleTemplate->Expand( &output, &getMainDictionary() );

	std::string filename = static_cast< std::string >( node.name() ) + ".c";
	ofstream outputFile( filename.c_str() );
	outputFile << output;
	outputFile.close();
}

void Stage2_FRODOVisitor::Visit_QualifiedTTMsg( const QualifiedTTMsg &qualifiedTTMsg ) {

	std::string scheduleTimes = qualifiedTTMsg.ttMsg.schedule();
	if ( scheduleTimes.empty() ) {
		std::string config;
		if ( qualifiedTTMsg.channel.type() == Semantics::IChan::meta ) {
			config = Semantics::IChan::Cast( qualifiedTTMsg.channel ).config();
		} else if ( qualifiedTTMsg.channel.type() == Semantics::OChan::meta ) {
			config = Semantics::OChan::Cast( qualifiedTTMsg.channel ).config();
		}
		ConfigParameterMap configParameterMap = getConfigParameterMap( config );
		scheduleTimes = configParameterMap[ "schedule" ];
	}

	ScheduleTimeSet scheduleTimeSet = getScheduleTimeSet( scheduleTimes );

	if ( scheduleTimeSet.empty() ) {
		std::cerr << "WARNING:  TTMsg \"" << qualifiedTTMsg.ttMsg.name() << "\" HAS NO SCHEDULE!" << std::endl;
	}

	for( ScheduleTimeSet::iterator stsItr = scheduleTimeSet.begin() ; stsItr != scheduleTimeSet.end() ; ++stsItr ) {
		_qualifiedTTMsgScheduleMap.insert(  std::make_pair( *stsItr, qualifiedTTMsg )  );
	}
}

void Stage2_FRODOVisitor::processQualifiedTTMsgScheduleMap( void ) {

	int ttMsgIndex = 1;
	for(
	 QualifiedTTMsgScheduleMap::iterator qtmItr = _qualifiedTTMsgScheduleMap.begin() ;
	 qtmItr != _qualifiedTTMsgScheduleMap.end() ;
	 ++qtmItr
	) {
		Semantics::TTMsg ttMsg = qtmItr->second.ttMsg;

		if ( qtmItr->second.msgType == TRANSMIT ) {
			scheduleTTMsg( ttMsg, ttMsgIndex, qtmItr->first, "MSG_DIR_SEND" );
			++ttMsgIndex;
		} else if ( qtmItr->second.msgType == RECEIVE ) {
			scheduleTTMsg( ttMsg, ttMsgIndex, qtmItr->first, "MSG_DIR_RECV" );
			++ttMsgIndex;
		}

	}

	if ( _platformType == "ATMega128" ) {

		addSectionDictionary( "AVR_PER" );

		int peripheralIndex = 1;
		for(
		 QualifiedTTMsgScheduleMap::iterator qtmItr = _qualifiedTTMsgScheduleMap.begin() ;
		 qtmItr != _qualifiedTTMsgScheduleMap.end() ;
		 ++qtmItr
		) {
			Semantics::TTMsg ttMsg = qtmItr->second.ttMsg;

			if ( qtmItr->second.msgType == PERIPHERAL ) {
				schedulePeripheral( qtmItr->second, peripheralIndex, qtmItr->first );
				++peripheralIndex;
			}

		}

		popSectionDictionary();
	}

}

void Stage2_FRODOVisitor::scheduleTTMsg(
 const Semantics::TTMsg &ttMsg, int index, double scheduleTime, const std::string &sendReceiveStr
) {
	addSectionDictionary( "MESSAGE_NAME" );
	getTemplateDictionary().SetValue(  "MESSAGE_index", boost::lexical_cast< std::string >( index )  );
	getTemplateDictionary().SetValue(  "MESSAGE_sendreceive", sendReceiveStr );
	getTemplateDictionary().SetValue(  "MESSAGE_name", static_cast< std::string >( ttMsg.name() )  );
	getTemplateDictionary().SetValue(  "MESSAGE_startTime", boost::lexical_cast< std::string >( scheduleTime )  );
	popSectionDictionary();
}

void Stage2_FRODOVisitor::schedulePeripheral( const QualifiedTTMsg qualifiedTTMsg, int index, double scheduleTime ) {

	ConfigParameterMap configParameterMap = getConfigParameterMap( qualifiedTTMsg.channel.config() );
	addSectionDictionary( "PER_NAME" );
	getTemplateDictionary().SetValue(  "PER_index", boost::lexical_cast< std::string >( index )  );
	getTemplateDictionary().SetValue(  "PER_type", configParameterMap[ "name" ] );
	getTemplateDictionary().SetValue(  "PER_name", std::string( qualifiedTTMsg.ttMsg.name() )  );
	getTemplateDictionary().SetValue(  "PER_way", configParameterMap[ "way" ] );
	getTemplateDictionary().SetValue(  "PER_pin_number", configParameterMap[ "pin_number" ] );
	getTemplateDictionary().SetValue(  "PER_startTime", boost::lexical_cast< std::string >( scheduleTime )  );
	(void)++index;
	popSectionDictionary();
}

void Stage2_FRODOVisitor::Visit_QualifiedTTMsg_Structure( const QualifiedTTMsg &qualifiedTTMsg ) {

	Semantics::TTMsg ttMsg = qualifiedTTMsg.ttMsg;

	addSectionDictionary( "MESSAGE_STRUCTURE" );
	getTemplateDictionary().SetValue(  "MESSAGE_name", std::string( ttMsg.name() )  );

	SignalSet signalSet = ttMsg.carries();
	SortedSignalFnumSet sortedSignalFnumSet;
	for( SignalSet::iterator sssItr = signalSet.begin() ; sssItr != signalSet.end() ; ++sssItr ) {
		ComponentSet receivingComponentSet = sssItr->consumedBy();
		Semantics::Component transmittingComponent = sssItr->generatedBy();
		if (
		 qualifiedTTMsg.msgType == TRANSMIT && !receivingComponentSet.empty() ||
		 qualifiedTTMsg.msgType == RECEIVE && transmittingComponent != Udm::null
		) continue;
		sortedSignalFnumSet.insert( *sssItr );
	}

	for(
	 SortedSignalFnumSet::iterator sssItr = sortedSignalFnumSet.begin() ;
	 sssItr != sortedSignalFnumSet.end() ;
	 ++sssItr
	) {
		Visit_Signal_Member( *sssItr );
	}

	popSectionDictionary();
}

void Stage2_FRODOVisitor::Visit_TTTask( const Semantics::TTTask &ttTask ) {

	// PROCESS MESSAGES
	QualifiedTTMsgSet qualifiedTTMsgSet = getQualifiedTTMsgs( ttTask );

	for(
	 QualifiedTTMsgSet::iterator qtsItr = qualifiedTTMsgSet.begin() ;
	 qtsItr != qualifiedTTMsgSet.end() ;
	 ++qtsItr
	) {
		Visit_QualifiedTTMsg( *qtsItr );
		Visit_QualifiedTTMsg_Structure( *qtsItr );
	}

	ScheduleTimeSet scheduleTimeSet = getScheduleTimeSet(  std::string( ttTask.schedule() )  );

	if ( scheduleTimeSet.empty() ) {
		std::cerr << "WARNING:  TTTask \"" << ttTask.name() << "\" HAS NO SCHEDULE!" << std::endl;
	}

	for( ScheduleTimeSet::iterator stsItr = scheduleTimeSet.begin() ; stsItr != scheduleTimeSet.end() ; (void)++stsItr ) {
		_ttTaskScheduleMap.insert(  std::make_pair( *stsItr, ttTask )  );
	}

	Visit_TTTask_Subsystem( ttTask );
}

Stage2_FRODOVisitor::QualifiedTTMsgSet Stage2_FRODOVisitor::getQualifiedTTMsgs( const Semantics::TTTask &ttTask ) {

	QualifiedTTMsgSet qualifiedTTMsgSet;

	TransmitsSet transmitsSet = ttTask.outboundData();
	for( TransmitsSet::iterator tmsItr = transmitsSet.begin() ; tmsItr != transmitsSet.end() ; ++tmsItr ) {
		Semantics::Msg msg = tmsItr->transmittedMsg();
		if ( msg.type() != Semantics::TTMsg::meta ) continue;
		QualifiedTTMsg qualifiedTTMsg = { Semantics::TTMsg::Cast( msg ), tmsItr->sendingChan(), TRANSMIT };
		qualifiedTTMsgSet.insert( qualifiedTTMsg );
	}

	ReceivesSet receivesSet = ttTask.incomingData();
	for( ReceivesSet::iterator rvsItr = receivesSet.begin() ; rvsItr != receivesSet.end() ; ++rvsItr ) {
		Semantics::Msg msg = rvsItr->receivedMsg();
		if ( msg.type() != Semantics::TTMsg::meta ) continue;
		QualifiedTTMsg qualifiedTTMsg = { Semantics::TTMsg::Cast( msg ), rvsItr->receivingChannel(), RECEIVE };
		qualifiedTTMsgSet.insert( qualifiedTTMsg );
	}

	AcquisitionSet acquisitionSet = ttTask.taskAcquisitions();
	for( AcquisitionSet::iterator acsItr = acquisitionSet.begin() ; acsItr != acquisitionSet.end() ; ++acsItr ) {
		Semantics::Msg msg = acsItr->acquiredMsg();
		if ( msg.type() != Semantics::TTMsg::meta ) continue;
		QualifiedTTMsg qualifiedTTMsg = { Semantics::TTMsg::Cast( msg ), acsItr->acquiringChannel(), PERIPHERAL };
		qualifiedTTMsgSet.insert( qualifiedTTMsg );
	}

	ActuationSet actuationSet = ttTask.taskActuations();
	for( ActuationSet::iterator acsItr = actuationSet.begin() ; acsItr != actuationSet.end() ; ++acsItr ) {
		Semantics::Msg msg = acsItr->actuatingMsg();
		if ( msg.type() != Semantics::TTMsg::meta ) continue;
		QualifiedTTMsg qualifiedTTMsg = { Semantics::TTMsg::Cast( msg ), acsItr->actuatingChannel(), PERIPHERAL };
		qualifiedTTMsgSet.insert( qualifiedTTMsg );
	}

	return qualifiedTTMsgSet;
}

void Stage2_FRODOVisitor::processTTTaskScheduleMap( void ) {
	for(
	 TTTaskScheduleMap::iterator tsmItr = _ttTaskScheduleMap.begin() ;
	 tsmItr != _ttTaskScheduleMap.end() ;
	 (void)++tsmItr
	) {
		addSectionDictionary( "TASK" );
		getTemplateDictionary().SetValue(  "TASK_name", std::string( tsmItr->second.name() )  );
		getTemplateDictionary().SetValue(  "TASK_startTime", boost::lexical_cast< std::string >( tsmItr->first )  );
		popSectionDictionary();
	}
}

void Stage2_FRODOVisitor::Visit_Signal_Member( const Semantics::Signal &signal ) {

	Semantics::SignalType sigType = signal.isOfType();

	addSectionDictionary( "SIGNAL_MEMBER" );
	std::string signalTypeName = sigType.ctypestr();
	if ( signalTypeName.empty() ) signalTypeName = sigType.mtypestr();
	getTemplateDictionary().SetValue(  "SIGNAL_typeName", std::string( signalTypeName )  );
	getTemplateDictionary().SetValue(  "SIGNAL_name", std::string( signal.name() )  );
	popSectionDictionary();
}

void Stage2_FRODOVisitor::Visit_TTTask_Subsystem( const Semantics::TTTask &ttTask ) {

	ComponentSet componentSet = ttTask.invokes();
	if ( !componentSet.empty() ) {

		Semantics::Component ttTaskComponent = *componentSet.begin();
		
		StringList headerFileNames = split( ttTaskComponent.hfiles() );
		for( StringList::iterator stlItr = headerFileNames.begin() ; stlItr != headerFileNames.end() ; ++stlItr ) {
			addSectionDictionary( "INCLUDE" );
			getTemplateDictionary().SetValue( "NAME", *stlItr );
			popSectionDictionary();
		}

		addSectionDictionary( "TASK_SUBSYSTEM" );

		if ( _platformType == "ATMega128" ) {
			addSectionDictionary( "AVR_START" );
			popSectionDictionary();
			addSectionDictionary( "AVR_END" );
			popSectionDictionary();
		}

		getTemplateDictionary().SetValue(  "TASK_componentName", std::string( ttTaskComponent.name() )  );
		getTemplateDictionary().SetValue(  "TASK_name", std::string( ttTask.name() )  );

		SortedSignalNumSet inputSignalSet = ttTaskComponent.consumes_sorted( SignalNumSorter() );
		for(
		 SortedSignalNumSet::iterator issItr = inputSignalSet.begin() ;
		 issItr != inputSignalSet.end() ;
		 ++issItr
		) {
			addSectionDictionary( "INPUT_SIGNALS" );
			Visit_Signal( *issItr );
			popSectionDictionary();
		}

		SortedSignalNumSet outputSignalSet = ttTaskComponent.generates_sorted( SignalNumSorter() );
		for(
		 SortedSignalNumSet::iterator ossItr = outputSignalSet.begin() ;
		 ossItr != outputSignalSet.end() ;
		 ++ossItr
		) {
			addSectionDictionary( "OUTPUT_SIGNALS" );
			Visit_Signal( *ossItr );
			popSectionDictionary();
		}

		popSectionDictionary();

		std::string ccode = ttTaskComponent.ccode();
		if ( !ccode.empty() ) {
			addSectionDictionary( "CCODE" );
			getTemplateDictionary().SetValue(  "TASK_componentName", std::string( ttTaskComponent.name() )  );
			getTemplateDictionary().SetValue(  "CCODE_BODY", std::string( ttTaskComponent.ccode() )  );

			for(
			 SortedSignalNumSet::iterator issItr = inputSignalSet.begin() ;
			 issItr != inputSignalSet.end() ;
			 (void)++issItr
			) {
				addSectionDictionary( "INPUT_SIGNALS" );
				Visit_Signal( *issItr, true );
				popSectionDictionary();
			}

	 		for(
			 SortedSignalNumSet::iterator ossItr = outputSignalSet.begin() ;
			 ossItr != outputSignalSet.end() ;
			 (void)++ossItr
			) {
				addSectionDictionary( "OUTPUT_SIGNALS" );
				Visit_Signal( *ossItr, true );
				popSectionDictionary();
			}

			popSectionDictionary();
		}

	}

}

void Stage2_FRODOVisitor::Visit_Signal( const Semantics::Signal &signal, bool isParameter ) {
	MsgSet msgSet = signal.carriedBy();
	TTMsgSet ttMsgSet;
	for( MsgSet::iterator mgsItr = msgSet.begin() ; mgsItr != msgSet.end() ; (void)++mgsItr ) {
		ttMsgSet.insert(  Semantics::TTMsg::Cast( *mgsItr )  );
	}
	Semantics::TTMsg ttMsg = *ttMsgSet.begin();

	getTemplateDictionary().SetValue(  "SIGNAL_message_name", std::string( ttMsg.name() )  );

	std::string signalName = signal.name();
	if ( isParameter ) {
		std::string::size_type pos = signalName.rfind( "__" );
		if ( pos != std::string::npos ) signalName = signalName.substr( pos + 2 );
	}
	Semantics::SignalType signalType = signal.isOfType();
	if ( signalType != Udm::null ) {
		getTemplateDictionary().SetValue(  "SIGNAL_type", std::string( signalType.ctypestr() )  );
	}

	getTemplateDictionary().SetValue( "SIGNAL_name", signalName );
}

Stage2_FRODOVisitor::ConfigParameterMap Stage2_FRODOVisitor::getConfigParameterMap( const std::string &config ) {

	ConfigParameterMap configParameterMap;

	StringList parameterAssignments = split( config, "\n " );

	for( StringList::iterator stlItr = parameterAssignments.begin() ; stlItr != parameterAssignments.end() ; ++stlItr ) {
		StringList assignment = split( *stlItr, "=" );
		configParameterMap.insert(  std::make_pair( assignment.front(), assignment.back() )  );
	}

	return configParameterMap;
}

