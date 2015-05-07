#include "Stage2_AbstractVisitor.h"

Stage2_AbstractVisitor::Stage2_AbstractVisitor( Semantics::RootFolder & rf, timeStamp & ts ) :
 _datetime(ts), _emptyDictionary( "MAIN" ), _mainDictionary( NULL ) {
	clearDictionary();
}

Stage2_AbstractVisitor::ScheduleTimeSet Stage2_AbstractVisitor::getScheduleTimeSet( const std::string &scheduleTimes ) {

	ScheduleTimeSet scheduleTimeSet;

	if ( scheduleTimes.empty() ) return scheduleTimeSet;

	StringList scheduleTimeStrings = split( scheduleTimes );

	for( StringList::iterator stlItr = scheduleTimeStrings.begin() ; stlItr != scheduleTimeStrings.end() ; ++stlItr ) {
		scheduleTimeSet.insert(  boost::lexical_cast< double >( *stlItr )  );
	}

	return scheduleTimeSet;
}

Stage2_AbstractVisitor::StringList Stage2_AbstractVisitor::split( const std::string &str, const std::string &separator ) {

	StringList stringList;

	std::string::size_type position = 0, oldPosition = 0;
	while(   (  position = str.find_first_of( separator, oldPosition )  )  !=  std::string::npos   ) {
		if ( oldPosition != position ) {
			stringList.push_back(  str.substr( oldPosition, position - oldPosition )  );
		}
		oldPosition = position + 1;
	}

	if ( oldPosition < str.length() ) stringList.push_back(  str.substr( oldPosition )  );

	return stringList;
}

