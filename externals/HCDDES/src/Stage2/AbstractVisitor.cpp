/*** Warning Workaround ***/
#pragma warning( disable: 4251 )


/*** Included Header Files ***/
#include "AbstractVisitor.h"


/***************************************************************************************/


AbstractVisitor::AbstractVisitor( Semantics::RootFolder &rootFolder, TimeStamp &timeStamp ) :
	_datetime( timeStamp ), _emptyDictionary( "MAIN" ), _mainDictionary( NULL ) {
	// Just need to clean the dictionary
	this->ClearDictionary();
}


AbstractVisitor::AbstractVisitor() :
	_datetime(), _emptyDictionary( "MAIN" ), _mainDictionary( NULL ) {
	// Just need to clean the dictionary
	this->ClearDictionary();
}

AbstractVisitor::ScheduleTimeSet AbstractVisitor::GetScheduleTimeSet( const std::string &scheduleTimes ) {
	ScheduleTimeSet scheduleTimeSet;
	if ( scheduleTimes.empty() ) 
		return scheduleTimeSet;
	StringList scheduleTimeStrings = Split( scheduleTimes );
	for( StringList::iterator stlIter = scheduleTimeStrings.begin() ; stlIter != scheduleTimeStrings.end() ; ++stlIter ) {
		ostringstream cleanup;
		for ( unsigned int idx = 0; idx < (*stlIter).size(); idx++ )
		{
			if ( (*stlIter)[idx] != ' ' )
				cleanup << (*stlIter)[idx];
		}
		scheduleTimeSet.insert(  boost::lexical_cast< double >( cleanup.str() )  );
	}
	return scheduleTimeSet;
}

bool AbstractVisitor::HasTTSched( const Semantics::Executable & ea ) {

	set< Semantics::ExecInfo > eis = ea.info();

	if ( eis.empty() )
		return false;

	Semantics::ExecInfo ei = *(eis.begin());
	Semantics::Schedule sch = ei.Schedule_child();

	return (sch != Udm::null ? (sch.type() == Semantics::TTSchedule::meta) : false);
}

std::string AbstractVisitor::GetTTSched( const Semantics::Executable &executable ) {

	std::string execName;
	// See if the executable is a Msg
	if ( executable.type() == Semantics::Msg::meta ) {
		// Cast to a Msg
		Semantics::Msg msg = Semantics::Msg::Cast( executable );
		execName = msg.name();
	}
	// See if executable is a Task
	else if ( executable.type() == Semantics::Task::meta ) {
		// Cast to a task
		Semantics::Task task = Semantics::Task::Cast( executable );
		execName = task.name();
	}
	// Get the execInfo set
	std::set< Semantics::ExecInfo > execInfoSet = executable.info();
	// See if there are any EI in the set
	if ( execInfoSet.empty() ) {
		// For now debug some info and return ""
		std::cout << "*** WARNING: No ExecInfo attached to executable " << execName << ".\n";
		return "";
	}
	// Good, there is exec info
	else {
		// Get the first (should be only) execInfo
		Semantics::ExecInfo execInfo = *execInfoSet.begin();
		// Get the TTSchedule child (if present)
		std::string scheduleTimes = "";
		Semantics::Schedule schedule = execInfo.Schedule_child();
		if ( schedule.type() == Semantics::TTSchedule::meta ) {
			Semantics::TTSchedule ttSchedule = Semantics::TTSchedule::Cast( schedule );
			scheduleTimes = ttSchedule.ticksched();
		}
		// Else, we are not properly handling the situation
		else {
			// For now send some debug info and return ""
			std::cout << "*** WARNING: No TTSchedule attached to execInfo for " << execName << ".\n";
		}
		// Return the schedule
		return scheduleTimes;
	}
}


bool AbstractVisitor::HasSampPeriod( const Semantics::Executable & ea ) {
	
	set< Semantics::ExecInfo > eis = ea.info();
	Semantics::ExecInfo ei = *(eis.begin());
	Semantics::Periodicity pd = ei.Periodicity_child();
	return ( pd.type() == Semantics::SamplePeriod::meta );
}

double AbstractVisitor::GetSampPeriod( const Semantics::Executable & ea ) {

	set< Semantics::ExecInfo > eis = ea.info();
	Semantics::ExecInfo ei = *(eis.begin());
	Semantics::Periodicity pd = ei.Periodicity_child();
	Semantics::SamplePeriod spd = Semantics::SamplePeriod::Cast( pd );
	return spd.periodsecs();
}

double AbstractVisitor::GetDesiredOffset( const Semantics::Executable & ea )
{
	set< Semantics::ExecInfo > eis = ea.info();
	Semantics::ExecInfo ei = *(eis.begin());
	Semantics::Periodicity pd = ei.Periodicity_child();
	Semantics::SamplePeriod spd = Semantics::SamplePeriod::Cast( pd );
	return spd.desiredoffsetsecs();
}

bool AbstractVisitor::HasMinPeriod( const Semantics::Executable & ea ) {
	
	set< Semantics::ExecInfo > eis = ea.info();
	Semantics::ExecInfo ei = *(eis.begin());
	Semantics::Periodicity pd = ei.Periodicity_child();
	return ( pd.type() == Semantics::MinimumPeriod::meta );
}

double AbstractVisitor::GetMinPeriod( const Semantics::Executable & ea ) {

	set< Semantics::ExecInfo > eis = ea.info();
	Semantics::ExecInfo ei = *(eis.begin());
	Semantics::Periodicity pd = ei.Periodicity_child();
	Semantics::MinimumPeriod mpd = Semantics::MinimumPeriod::Cast( pd );
	return mpd.periodsecs();
}

bool AbstractVisitor::HasMaxDuration( const Semantics::Executable & ea ) {
	
	set< Semantics::ExecInfo > eis = ea.info();
	Semantics::ExecInfo ei = *(eis.begin());
	Semantics::Duration dur = ei.Duration_child();
	return ( dur.type() == Semantics::MaxDuration::meta );
}

double AbstractVisitor::GetMaxDuration( const Semantics::Executable & ea ) {

	set< Semantics::ExecInfo > eis = ea.info();
	Semantics::ExecInfo ei = *(eis.begin());
	Semantics::MaxDuration mxd = Semantics::MaxDuration::Cast( ei.Duration_child() );
	return mxd.exectimesecs();
}

AbstractVisitor::StringList AbstractVisitor::Split( const std::string &str, const std::string &separator ) {
	StringList stringList;
	std::string::size_type position = 0, oldPosition = 0;
	while( ( position = str.find_first_of( separator, oldPosition ) ) != std::string::npos ) {
		if ( oldPosition != position ) {
			stringList.push_back( str.substr( oldPosition, position - oldPosition ) );
		}
		oldPosition = position + 1;
	}
	if ( oldPosition < str.length() ) 
		stringList.push_back( str.substr( oldPosition ) );
	return stringList;
}

AbstractVisitor::QualifiedMessageSet AbstractVisitor::GetQualifiedMessages( const Semantics::Task &task ) {
	QualifiedMessageSet qualifiedSet;
	// Process all outbound message
	TransmitsSet transmitsSet = task.outboundData();
	for( TransmitsSet::iterator tmsIter = transmitsSet.begin() ; tmsIter != transmitsSet.end() ; ++tmsIter ) {
		Semantics::Msg msg = tmsIter->transmittedMsg();
		// Make sure we are dealing with a message
		if ( msg.type() != Semantics::Msg::meta ) continue;
		QualifiedMessage qualifiedMsg = { Semantics::Msg::Cast( msg ), tmsIter->sendingChan(), TRANSMIT };
		qualifiedSet.insert( qualifiedMsg );
	}
	// Process all incoming message
	ReceivesSet receivesSet = task.incomingData();
	for( ReceivesSet::iterator rvsIter = receivesSet.begin() ; rvsIter != receivesSet.end() ; ++rvsIter ) {
		Semantics::Msg msg = rvsIter->receivedMsg();
		// Make sure we are dealing with a message
		if ( msg.type() != Semantics::Msg::meta ) continue;
		QualifiedMessage qualifiedTTMsg = { Semantics::Msg::Cast( msg ), rvsIter->receivingChannel(), RECEIVE };
		qualifiedSet.insert( qualifiedTTMsg );
	}
	// Process all acquisitions
	AcquisitionSet acquisitionSet = task.taskAcquisitions();
	for( AcquisitionSet::iterator acsItr = acquisitionSet.begin() ; acsItr != acquisitionSet.end() ; ++acsItr ) {
		Semantics::Msg msg = acsItr->acquiredMsg();
		// Make sure we are dealing with a message
		if ( msg.type() != Semantics::Msg::meta ) continue;
		QualifiedMessage qualifiedMsg = { Semantics::Msg::Cast( msg ), acsItr->acquiringChannel(), PERIPHERAL };
		qualifiedSet.insert( qualifiedMsg );
	}
	// Process all actuations
	ActuationSet actuationSet = task.taskActuations();
	for( ActuationSet::iterator acsItr = actuationSet.begin() ; acsItr != actuationSet.end() ; ++acsItr ) {
		Semantics::Msg msg = acsItr->actuatingMsg();
		// Make sure we are dealing with a message
		if ( msg.type() != Semantics::Msg::meta ) continue;
		QualifiedMessage qualifiedMsg = { Semantics::Msg::Cast( msg ), acsItr->actuatingChannel(), PERIPHERAL };
		qualifiedSet.insert( qualifiedMsg );
	}
	// Return the set of qualified messages
	return qualifiedSet;
}

/***************************************************************************************/

