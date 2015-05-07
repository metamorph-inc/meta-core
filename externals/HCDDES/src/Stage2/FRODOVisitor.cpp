/*** Warning Workaround ***/
#pragma warning( disable: 4251 )


/*** Included Header Files ***/
#include "VisitorFactory.h"
#include "FRODOVisitor.h"
#include "ConfigKeeper.h"
#include "Uml.h"
#include <boost/filesystem/operations.hpp>


/***************************************************************************************/


bool FRODOVisitor::_initialized =
VisitorFactory::RegisterInstantiator( std::string( "FRODO" ), &FRODOVisitor::GetFRODOVisitor  );

void FRODOVisitor::Visit_RootFolder( const Semantics::RootFolder & rootFolder ) {
	DeploymentVector deploymentVector = rootFolder.Deployment_children();
	for( DeploymentVector::iterator dpvIter = deploymentVector.begin() ; dpvIter != deploymentVector.end() ; (void)++dpvIter ) {
		Visit_Deployment( *dpvIter );
	}
}


void FRODOVisitor::Visit_Deployment( const Semantics::Deployment & deployment ) {
	NodeVector nodeVector = deployment.Node_kind_children();
	for( NodeVector::iterator ndsIter = nodeVector.begin() ; ndsIter != nodeVector.end() ; (void)++ndsIter ) {
		Visit_Node( *ndsIter );
	}

}


void FRODOVisitor::Visit_Node( const Semantics::Node & node ) {
	std::string tmpl_fname = ConfigKeeper::inst().GetTemplatePath() + "\\FRODO.tpl";
	ctemplate::Template *googleTemplate = ctemplate::Template::GetTemplate( tmpl_fname, ctemplate::DO_NOT_STRIP );
	ClearDictionary();
	_ttTaskScheduleMap.clear();
	_qualifiedTTMsgScheduleMap.clear();
	_platformType = node.platformtype();
	_msgNameKeeper.clear();

	if ( _platformType == "PXA" || _platformType == string("pxa-gumstix-linux") ) {
		AddSectionDictionary( "PXA_INCLUDE" );
		PopSectionDictionary();
	} else if ( _platformType == "AVR" || _platformType == string("atmega128-robostix-freertos") ) {
		AddSectionDictionary( "AVR_INCLUDE" );
		PopSectionDictionary();
	}

	GetTemplateDictionary().SetValue(
	 "NODE_hyperperiod", boost::lexical_cast< std::string >(  static_cast< int >( node.hyperperiodticks() )  )
	);

	// PROCESS TASKS
	TaskSet taskSet = node.executes();
	TaskVector taskVector( taskSet.begin(), taskSet.end() );
	for( TaskVector::iterator tksIter = taskVector.begin() ; tksIter != taskVector.end() ; ++tksIter ) {
		if ( tksIter->type() != Semantics::Task::meta ) continue;
		Visit_TTTask(  Semantics::Task::Cast( *tksIter )  );
	}

	BuildInitSection( taskVector ); // relies on the global message map, so we call this after
	                                   // the tasks are built

	AddSectionDictionary( "SCHEDULE_SECTION" );
	if ( _platformType == string("PXA") || _platformType == string("pxa-gumstix-linux") ) {
		AddSectionDictionary( "PXA_VERSION" );
	} else if ( _platformType == string("AVR") || _platformType == string("atmega128-robostix-freertos") ) {
		AddSectionDictionary( "AVR_VERSION" );
	}
	ProcessTTTaskScheduleMap();
	ProcessQualifiedTTMsgScheduleMap();
	PopSectionDictionary();
	PopSectionDictionary();
 	std::string output;
	googleTemplate->Expand( &output, &GetMainDictionary() );
	// Setup the output directory
	std::string outputDir = ConfigKeeper::inst().GetOutfileName();
	// Only work on directory if not .
	if ( outputDir.compare( "." ) != 0 ) {
		// Create a path
		boost::filesystem::path outPath( outputDir );
		// See if the directory exists
		if (! boost::filesystem::exists( outPath ) ) {
			// If not, create the directory
			boost::filesystem::create_directory( outPath );
		}
	}
	std::string filename = outputDir + "\\" + static_cast< std::string >( node.name() ) + ".c";
	ofstream outputFile( filename.c_str() );
	outputFile << output;
	outputFile.close();
}


void FRODOVisitor::Visit_QualifiedTTMsg( const QualifiedTTMsg & qualifiedTTMsg ) {
	Semantics::Msg m = qualifiedTTMsg.ttMsg;

	// Get config parameters from the acquisition and actuation messages
	std::string scheduleTimes = GetTTSched( m );
	if ( scheduleTimes.empty() && qualifiedTTMsg.channel != Udm::null && qualifiedTTMsg.msgTimed == SCHEDULED ) {
		std::string config;
		if ( qualifiedTTMsg.channel.type() == Semantics::IChan::meta ) {
			config = Semantics::IChan::Cast( qualifiedTTMsg.channel ).config();
		} else if ( qualifiedTTMsg.channel.type() == Semantics::OChan::meta ) {
			config = Semantics::OChan::Cast( qualifiedTTMsg.channel ).config();
		}
		ConfigParameterMap configParameterMap = GetConfigParameterMap( config );
		scheduleTimes = configParameterMap[ "schedule" ];
	}

	ScheduleTimeSet scheduleTimeSet = GetScheduleTimeSet( scheduleTimes );
	if ( scheduleTimeSet.empty() && qualifiedTTMsg.msgTimed == SCHEDULED ) {
		std::cerr << "WARNING:  TTMsg \"" << qualifiedTTMsg.ttMsg.name() << "\" HAS NO SCHEDULE!" << std::endl;
	}

	for( ScheduleTimeSet::iterator stsItr = scheduleTimeSet.begin() ; stsItr != scheduleTimeSet.end() ; ++stsItr ) {
		_qualifiedTTMsgScheduleMap.insert(  std::make_pair( *stsItr, qualifiedTTMsg )  );
	}
}


void FRODOVisitor::ProcessQualifiedTTMsgScheduleMap( void ) {
	int ttMsgIndex = 1;
	for(
	 QualifiedTTMsgScheduleMap::iterator qtmItr = _qualifiedTTMsgScheduleMap.begin() ;
	 qtmItr != _qualifiedTTMsgScheduleMap.end() ;
	 ++qtmItr
	) {
		Semantics::Msg ttMsg = qtmItr->second.ttMsg;
		if ( qtmItr->second.msgDirect == SEND 
			&& qtmItr->second.msgTimed == SCHEDULED 
			&& qtmItr->second.msgCtxt == BUS ) {

			ScheduleTTMsg( ttMsg, ttMsgIndex, (unsigned __int64)qtmItr->first, "MSG_DIR_SEND" );
			++ttMsgIndex;

		} else if ( qtmItr->second.msgDirect == RECEIVE 
			&& qtmItr->second.msgTimed == SCHEDULED 
			&& qtmItr->second.msgCtxt == BUS ) {
			ScheduleTTMsg( ttMsg, ttMsgIndex, (unsigned __int64)qtmItr->first, "MSG_DIR_RECV" );
			++ttMsgIndex;
		}
	}

	if ( _platformType == "AVR" || _platformType == string("atmega128-robostix-freertos") ) {
		int peripheralIndex = 1;
		for(
		 QualifiedTTMsgScheduleMap::iterator qtmItr = _qualifiedTTMsgScheduleMap.begin() ;
		 qtmItr != _qualifiedTTMsgScheduleMap.end() ;
		 ++qtmItr
		) {
			Semantics::Msg ttMsg = qtmItr->second.ttMsg;
			if ( qtmItr->second.msgCtxt == PERIPHERAL && qtmItr->second.msgTimed == SCHEDULED ) {
				SchedulePeripheral( qtmItr->second, peripheralIndex, (unsigned __int64)qtmItr->first );
				++peripheralIndex;
			}
		}
	}
}


void FRODOVisitor::ScheduleTTMsg(
 const Semantics::Msg & ttMsg, int index, unsigned long long scheduleTime, const std::string & sendReceiveStr
) {
	AddSectionDictionary( "MESSAGE_NAME" );
	GetTemplateDictionary().SetValue(  "MESSAGE_index", boost::lexical_cast< std::string >( index )  );
	GetTemplateDictionary().SetValue(  "MESSAGE_sendreceive", sendReceiveStr );
	string new_msg_name = ReplaceString( string(ttMsg.name()), string( "." ), string( "_" ) );
	GetTemplateDictionary().SetValue(  "MESSAGE_name", static_cast< std::string >( new_msg_name )  );
	GetTemplateDictionary().SetValue(  "MESSAGE_startTime", boost::lexical_cast< std::string >( scheduleTime )  );
	PopSectionDictionary();
}


void FRODOVisitor::SchedulePeripheral( const QualifiedTTMsg qualifiedTTMsg, int index, unsigned long long scheduleTime ) {
	ConfigParameterMap configParameterMap = GetConfigParameterMap( qualifiedTTMsg.channel.config() );
	AddSectionDictionary( "PER_NAME" );
	GetTemplateDictionary().SetValue(  "PER_index", boost::lexical_cast< std::string >( index )  );
	GetTemplateDictionary().SetValue(  "PER_type", configParameterMap[ "name" ] );
	string new_msg_name = ReplaceString( string(qualifiedTTMsg.ttMsg.name()), string( "." ), string( "_" ) );
	GetTemplateDictionary().SetValue(  "PER_name", new_msg_name  );
	GetTemplateDictionary().SetValue(  "PER_way", configParameterMap[ "way" ] );
	GetTemplateDictionary().SetValue(  "PER_pin_number", configParameterMap[ "pin_number" ] );
	GetTemplateDictionary().SetValue(  "PER_startTime", boost::lexical_cast< std::string >( scheduleTime )  );
	(void)++index;
	PopSectionDictionary();
}


void FRODOVisitor::Visit_QualifiedTTMsg_Structure( const QualifiedTTMsg & qualifiedTTMsg ) {
	Semantics::Msg ttMsg = qualifiedTTMsg.ttMsg;
	AddSectionDictionary( "MESSAGE_STRUCTURE" );

	string new_msg_name = ReplaceString( string(ttMsg.name()), string( "." ), string( "_" ) );
	GetTemplateDictionary().SetValue( "MESSAGE_name", new_msg_name );
	SignalSet signalSet = ttMsg.carries();
	SortedSignal_ByMsgIndex_Set sortedSignalSet;
	for( SignalSet::iterator sssIter = signalSet.begin() ; sssIter != signalSet.end() ; ++sssIter ) {

		ComponentSet receivingComponentSet = sssIter->consumedBy();
		Semantics::Component transmittingComponent = sssIter->generatedBy();
		if (
			qualifiedTTMsg.msgDirect == SEND && !receivingComponentSet.empty() ||
			qualifiedTTMsg.msgDirect == RECEIVE && transmittingComponent != Udm::null
		) continue;
		sortedSignalSet.insert( *sssIter );
	}

	for(
		SortedSignal_ByMsgIndex_Set::iterator sssIter = sortedSignalSet.begin() ;
		sssIter != sortedSignalSet.end() ;
		++sssIter
	) {
		Visit_Signal_Member( *sssIter );
	}
	PopSectionDictionary();
}


void FRODOVisitor::Visit_TTTask( const Semantics::Task & ttTask ) {
	// PROCESS MESSAGES
	GetQualifiedTTMsgs( ttTask );
	for(
		QualifiedTTMsgSet::iterator qtsIter = _qualifiedTTMsgSet.begin() ;
		qtsIter != _qualifiedTTMsgSet.end() ;
		++qtsIter
	) {
		if ( qtsIter->msgCtxt != DUMMY )
		{
			Visit_QualifiedTTMsg( *qtsIter );
		}

		if ( ( _msgNameKeeper.find( string( qtsIter->ttMsg.name() ) ) == _msgNameKeeper.end() )
			&&  qtsIter->generateStruct ) // Skip to avoid a duplicate
		{
			Visit_QualifiedTTMsg_Structure( *qtsIter );
		}

		_msgNameKeeper.insert( string( qtsIter->ttMsg.name() ) );
	}

	ScheduleTimeSet scheduleTimeSet = GetScheduleTimeSet( GetTTSched( ttTask ) );
	if ( scheduleTimeSet.empty() ) {
		std::cerr << "WARNING:  TTTask \"" << ttTask.name() << "\" HAS NO SCHEDULE!" << std::endl;
	}
	for( ScheduleTimeSet::iterator stsIter = scheduleTimeSet.begin() ; stsIter != scheduleTimeSet.end() ; (void)++stsIter ) {
		_ttTaskScheduleMap.insert( std::make_pair( *stsIter, ttTask ) );
	}
	Visit_TTTask_Subsystem( ttTask );
}

void FRODOVisitor::BuildInitSection( TaskVector & tasks ) {

	AddSectionDictionary("INITIALIZATION"); 
	AddSectionDictionary("CONTEXT_INIT");
 
	for ( TaskVector::iterator taskIter = tasks.begin(); taskIter != tasks.end(); taskIter++ )
	{
		AddSectionDictionary( "TASK_CONTEXT" );
		ComponentSet componentSet = (*taskIter).invokes();
		if ( !componentSet.empty() ) {
			Semantics::Component ttTaskComponent = *componentSet.begin();
			GetTemplateDictionary().SetValue( "TASK_functionName", std::string( ttTaskComponent.msubsystem() ) );
		}
		PopSectionDictionary();
	}
	PopSectionDictionary(); // CONTEXT_INIT

	for(
	 QualifiedTTMsgSet::iterator qmsItr = _qualifiedTTMsgSet.begin();
		qmsItr != _qualifiedTTMsgSet.end(); qmsItr++
	) {
		if ( qmsItr->generateStruct ) {
			Semantics::Msg ttMsg = qmsItr->ttMsg;		
			AddSectionDictionary( "MESSAGE_INIT" );
			string new_msg_name = ReplaceString( string(ttMsg.name()), string( "." ), string( "_" ) );
			GetTemplateDictionary().SetValue( "MESSAGE_name", new_msg_name );
			PopSectionDictionary();
		}
	}
	PopSectionDictionary();
}

void FRODOVisitor::GetQualifiedTTMsgs( const Semantics::Task &ttTask )
{
	TransmitsSet transmitsSet = ttTask.outboundData();
	for( TransmitsSet::iterator tmsIter = transmitsSet.begin() ; tmsIter != transmitsSet.end() ; ++tmsIter ) {
		Semantics::Msg msg = tmsIter->transmittedMsg();
		if ( msg.type() != Semantics::Msg::meta ) continue;
		QualifiedTTMsg qualifiedTTMsg = { Semantics::Msg::Cast( msg ), tmsIter->sendingChan(), SEND, BUS, SCHEDULED, true };
		_qualifiedTTMsgSet.insert( qualifiedTTMsg );
	}

	LocalDepSet localDepSet = ttTask.localSendingTaskDeps();
	for ( LocalDepSet::iterator ldsIter = localDepSet.begin(); ldsIter != localDepSet.end(); ++ldsIter ) {
		Semantics::Msg msg = ldsIter->depMsg();
		if ( msg.type() != Semantics::Msg::meta ) continue;
		QualifiedTTMsg qualifiedTTMsg = { Semantics::Msg::Cast( msg ), Semantics::Channel::Cast( Udm::null ), SEND, LOCAL, UNSCHEDULED, true };
		_qualifiedTTMsgSet.insert( qualifiedTTMsg );
	}

	ReceivesSet receivesSet = ttTask.incomingData();
	for( ReceivesSet::iterator rvsIter = receivesSet.begin() ; rvsIter != receivesSet.end() ; ++rvsIter ) {
		Semantics::Msg msg = rvsIter->receivedMsg();
		if ( msg.type() != Semantics::Msg::meta ) continue;
		QualifiedTTMsg qualifiedTTMsg = { Semantics::Msg::Cast( msg ), rvsIter->receivingChannel(), RECEIVE, BUS, SCHEDULED, true };
		_qualifiedTTMsgSet.insert( qualifiedTTMsg );
	}

	localDepSet = ttTask.localReceivingTaskDeps();
	for ( LocalDepSet::iterator ldsIter = localDepSet.begin(); ldsIter != localDepSet.end(); ++ldsIter ) {
		Semantics::Msg msg = ldsIter->depMsg();
		if ( msg.type() != Semantics::Msg::meta ) continue;
		QualifiedTTMsg qualifiedTTMsg = { Semantics::Msg::Cast( msg ), Semantics::Channel::Cast( Udm::null ), RECEIVE, LOCAL, UNSCHEDULED, false };
		Semantics::Task t = (*ldsIter).localDepSendingTask();
		if ( t == Semantics::Task::Cast(Udm::null) ) qualifiedTTMsg.generateStruct = true;
		_qualifiedTTMsgSet.insert( qualifiedTTMsg );
	}

	AcquisitionSet acquisitionSet = ttTask.taskAcquisitions();
	for( AcquisitionSet::iterator acsItr = acquisitionSet.begin() ; acsItr != acquisitionSet.end() ; ++acsItr ) {
		Semantics::Msg msg = acsItr->acquiredMsg();
		if ( msg.type() != Semantics::Msg::meta ) continue;
		QualifiedTTMsg qualifiedTTMsg = { Semantics::Msg::Cast( msg ), acsItr->acquiringChannel(), RECEIVE, PERIPHERAL, SCHEDULED, true };
		_qualifiedTTMsgSet.insert( qualifiedTTMsg );
	}

	ActuationSet actuationSet = ttTask.taskActuations();
	for( ActuationSet::iterator acsItr = actuationSet.begin() ; acsItr != actuationSet.end() ; ++acsItr ) {
		Semantics::Msg msg = acsItr->actuatingMsg();
		if ( msg.type() != Semantics::Msg::meta ) continue;
		QualifiedTTMsg qualifiedTTMsg = { Semantics::Msg::Cast( msg ), acsItr->actuatingChannel(), SEND, PERIPHERAL, SCHEDULED, true };
		_qualifiedTTMsgSet.insert( qualifiedTTMsg );
	}

	DummyDependencySet dummySet = ttTask.dummyDeps();
	if ( dummySet.size() > 0 )
	{
		// in any node all dummy dependencies point to the same message
		Semantics::DummyDependency dumdep = *(dummySet.begin());  
		Semantics::Msg msg = dumdep.dummyMsg();
		QualifiedTTMsg qualifiedTTMsg = { Semantics::Msg::Cast( msg ), Semantics::Channel::Cast( Udm::null ), SEND, DUMMY, UNSCHEDULED, true };
		_qualifiedTTMsgSet.insert( qualifiedTTMsg );
	}
}


void FRODOVisitor::ProcessTTTaskScheduleMap( void ) {
	for(
		TTTaskScheduleMap::iterator tsmItr = _ttTaskScheduleMap.begin() ;
		tsmItr != _ttTaskScheduleMap.end() ;
		(void)++tsmItr
	) {
		AddSectionDictionary( "TASK" );
		GetTemplateDictionary().SetValue( "TASK_name", std::string( tsmItr->second.name() ) );
		GetTemplateDictionary().SetValue( "TASK_startTime", boost::lexical_cast< std::string >( tsmItr->first ) );
		PopSectionDictionary();
	}
}


void FRODOVisitor::Visit_Signal_Member( const Semantics::Signal & signal ) {
	Semantics::SignalType sigType = signal.isOfType();
	AddSectionDictionary( "SIGNAL_MEMBER" );
	std::string signalTypeName = sigType.ctypestr();
	if ( signalTypeName.empty() )
		signalTypeName = sigType.mtypestr();
	GetTemplateDictionary().SetValue( "SIGNAL_typeName", std::string( signalTypeName ) );
	GetTemplateDictionary().SetValue( "SIGNAL_name", std::string( signal.name() ) );
	PopSectionDictionary();
}


void FRODOVisitor::Visit_TTTask_Subsystem( const Semantics::Task & ttTask ) {
	ComponentSet componentSet = ttTask.invokes();
	if ( !componentSet.empty() ) {
		Semantics::Component ttTaskComponent = *componentSet.begin();
		StringList headerFileNames = Split( ttTaskComponent.hfiles() );
		for( StringList::iterator stlIter = headerFileNames.begin() ; stlIter != headerFileNames.end() ; ++stlIter ) {
			AddSectionDictionary( "INCLUDE" );
			GetTemplateDictionary().SetValue( "NAME", *stlIter );
			PopSectionDictionary();
		}
		AddSectionDictionary( "TASK_SUBSYSTEM" );
		if ( _platformType == "AVR" || _platformType == string("atmega128-robostix-freertos")) {
			AddSectionDictionary( "AVR_START" );
			PopSectionDictionary();
			AddSectionDictionary( "AVR_END" );
			PopSectionDictionary();
		}


		GetTemplateDictionary().SetValue( "TASK_functionName", std::string( ttTaskComponent.msubsystem() ) );
		GetTemplateDictionary().SetValue( "TASK_name", std::string( ttTask.name() ) );

		SortedSignal_ByCallIndex_Set inputSignalSet = ttTaskComponent.consumes_sorted( SignalCallIndexSorter() );
		for(
			SortedSignal_ByCallIndex_Set::iterator issIter = inputSignalSet.begin() ;
			issIter != inputSignalSet.end() ;
			++issIter
		) {
			AddSectionDictionary( "INPUT_SIGNALS" );
			Visit_Signal( *issIter );
			PopSectionDictionary();
		}

		SortedSignal_ByCallIndex_Set outputSignalSet = ttTaskComponent.generates_sorted( SignalCallIndexSorter() );
		for(
			SortedSignal_ByCallIndex_Set::iterator ossIter = outputSignalSet.begin() ;
			ossIter != outputSignalSet.end() ;
			++ossIter
		) {
			AddSectionDictionary( "OUTPUT_SIGNALS" );
			Visit_Signal( *ossIter );
			PopSectionDictionary();
		}
		PopSectionDictionary();
	}
}


void FRODOVisitor::Visit_Signal( const Semantics::Signal & signal, bool isParameter ) {
	MsgSet msgSet = signal.carriedBy();
	MsgVector msgVector( msgSet.begin(), msgSet.end() );
	TTMsgSet ttMsgSet;
	for( MsgVector::iterator mgsIter = msgVector.begin() ; mgsIter != msgVector.end() ; (void)++mgsIter ) {
		ttMsgSet.insert( Semantics::Msg::Cast( *mgsIter ) );
	}
	Semantics::Msg ttMsg = *ttMsgSet.begin();

	string new_msg_name = ReplaceString( string(ttMsg.name()), string( "." ), string( "_" ) );
	GetTemplateDictionary().SetValue( "SIGNAL_message_name", std::string( new_msg_name ) );

	std::string signalName = signal.name();
	if ( isParameter ) {
		std::string::size_type pos = signalName.rfind( "__" );
		if ( pos != std::string::npos ) signalName = signalName.substr( pos + 2 );
	}
	Semantics::SignalType signalType = signal.isOfType();
	if ( signalType != Udm::null ) {
		GetTemplateDictionary().SetValue(  "SIGNAL_type", std::string( signalType.ctypestr() )  );
	}
	GetTemplateDictionary().SetValue( "SIGNAL_name", signalName );
}


FRODOVisitor::ConfigParameterMap FRODOVisitor::GetConfigParameterMap( const std::string & config ) {
	ConfigParameterMap configParameterMap;
	StringList parameterAssignments = Split( config, "\n" );

	for( StringList::iterator stlIter = parameterAssignments.begin() ; stlIter != parameterAssignments.end() ; ++stlIter ) {
		StringList assignment = Split( *stlIter, "=" );
		configParameterMap.insert(  std::make_pair( assignment.front(), assignment.back() )  );
	}
	return configParameterMap;
}


/***************************************************************************************/

