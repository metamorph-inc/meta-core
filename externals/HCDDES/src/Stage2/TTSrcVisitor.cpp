/*** Warning Workaround ***/
#pragma warning( disable: 4251 )


/*** Included Header Files ***/
#include "TTVisitor.h"
#include "ConfigKeeper.h"
#include "Uml.h"
#include <boost/Filesystem.hpp>

#include <iostream>
#include <sstream>
#include <string>

using namespace std;

/***************************************************************************************/

static int sched_count( std::string sched ) {
	// Double check the empty case
	if ( sched == "" ) return 0;
	int count = 1;
	std::string::size_type pos;
	// Count the number of commas
	while ( ( pos = sched.find(",") ) != std::string::npos ) count++;
	return count;
}

// Taken from http://www.oopweb.com/CPP/Documents/CPPHOWTO/Volume/C++Programming-HOWTO-7.html
static void Tokenize(const string& str,
                      vector<string>& tokens,
                      const string& delimiters = " ")
{
    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (string::npos != pos || string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}

static vector< double > sched_values( std::string sched ) {
	vector< double > results;

	if ( sched == "" ) return results;

	vector< string > tokens;
	Tokenize( sched, tokens, string( "," ) );

	for ( vector<string>::iterator strIter = tokens.begin(); strIter != tokens.end(); strIter++ )
	{
		istringstream instr( *strIter );
		double val;

		instr >> skipws >> val;
		results.push_back(val);
	}

	return results;
}

Semantics::CommMedium TrueTimeSourceVisitor::GetMessageBus( Semantics::Msg & m ) {

	Semantics::Transmits t = m.msgTransmitter();
	if ( t != Udm::null )
	{
		Semantics::BChan b = t.sendingChan();
		if ( b != Udm::null )
		{
			Semantics::CommInterface ci = b.chanInterface();

			if ( ci != Udm::null )
			{
				Semantics::CommMedium bus = ci.commMedium();

				return bus;
			}
		}
	}

	return Semantics::CommMedium::Cast( Udm::null );
}

/***************************************************************************************/

TrueTimeSourceVisitor::TrueTimeSourceVisitor( const std::map< std::string, TTNetwork >  & nwMap )  : TrueTimeVisitor(), _networkMap( nwMap )  {

}


void TrueTimeSourceVisitor::Visit_RootFolder( const Semantics::RootFolder &rootFolder ) {

	/*** Pass 2 - Generate all of the _init and _code files ***/


	std::cout << "-- Starting Second Pass:\n";
	// Clear the dictionary
	ClearDictionary();
	// Get the set of deployment children
	DeploymentVector deploymentVector = rootFolder.Deployment_children();
	// Visit each deployment to start off
	DeploymentVector::iterator deploymentIter = deploymentVector.begin();
	for ( ; deploymentIter != deploymentVector.end(); deploymentIter++) {
		// Visit the deployment again, this time with pass == true
		deploymentIter->Accept( *this );
	}
}


void TrueTimeSourceVisitor::Visit_Deployment( const Semantics::Deployment & deployment ) {
	
	// Must be second pass

		// Iterate over all of the bus objects and create a header file list for each of them.
		CommMediumSet cmSet = deployment.CommMedium_children();
		for ( CommMediumSet::iterator cmIter = cmSet.begin(); cmIter != cmSet.end(); cmIter++ )
		{
			string busName = (*cmIter).name();
			ostringstream out;
			out << "#define " << busName << "_HYPERPERIOD " << (*cmIter).hyperperiodsecs();
			_BusHeaderLines[ busName ].insert( out.str() );
		}
		_BusHeaderLines[ string( "bus" ) ].insert( string( "#define bus_HYPERPERIOD 0.0" ) );

		// Get set of Node children - create _init files
		NodeVector nodeVector = deployment.Node_children();
		// Visit them all
		NodeVector::iterator nodeIter = nodeVector.begin();
		for ( ; nodeIter != nodeVector.end(); nodeIter++ ) {
			// Visit the node again to gen _init files
			nodeIter->Accept( *this );
		}

		// Once all is said and done, generate the headers for the buses
		std::string directoryName = ConfigKeeper::inst().GetDirectoryName();
		for ( std::map< std::string, std::set< std::string > >::iterator mapIter = _BusHeaderLines.begin();
				mapIter != _BusHeaderLines.end(); mapIter++ )
		{
			if ( !( ( mapIter->first == string( "bus" ) ) && ( mapIter->second.size() < 2 ) ) )
			{
				string filename = directoryName + "\\" + mapIter->first + "_init_defines.h";
				ofstream headerFile( filename.c_str() );

				for ( std::set< std::string >::iterator lineIter = (mapIter->second).begin();
					lineIter != (mapIter->second).end(); lineIter++ )
				{
					headerFile << *lineIter << endl;
				}

				headerFile.close();
			}
		}
		_BusHeaderLines.clear();
}


void TrueTimeSourceVisitor::Visit_Node( const Semantics::Node & node ) {

	// Setup the node name
	std::string kernelName = node.name();
	std::string kernelInitName = kernelName + "_init";
	DEBUGOUT( "\tNode: " << kernelName << std::endl );

	// Must be the second pass
		_SchedHeaderLines.push_back( string( "// Define Schedule Offsets and Durations" ) );
		_SchedHeaderLines.push_back( string( "" ) );
	
		// Create the file name
		std::string filename = std::string( node.name() ) + "_init";

		// Create the hyperperiod symbol string for the node
		std::string hyp_str = kernelName + "_HYPERPERIOD";

		// Set some dictionary items for this node
		GetMainDictionary().SetValue( "FILENAME", filename );
		GetMainDictionary().SetValue( "KERNEL_SCHEDULE", "prioFP" );
		GetMainDictionary().SetFormattedValue( "KERNEL_HYPERPERIOD", "%f", (double) node.hyperperiodsecs() );
		GetMainDictionary().SetValue( "NODE_HYPERPERIOD_STR", hyp_str );

		DeviceSet devices = node.integrates();

		for ( DeviceSet::iterator devIter = devices.begin(); devIter != devices.end(); devIter++ )
		{
			if ( (*devIter).type() == Semantics::CommInterface::meta )
			{
				Semantics::CommInterface ci = Semantics::CommInterface::Cast( *devIter );
				Semantics::CommMedium cm = ci.commMedium();

				if ( cm != Udm::null ) {
					string busname = cm.name();
					AddSectionDictionary( "BUS_DEFINES" );
					GetTemplateDictionary().SetValue( "BUSNAME", busname );
					PopSectionDictionary();
				}
			}
		}
			

		if ( _SchedHeaderLines.size() < 4 )
		{
			ostringstream out;
			out << "#define " << hyp_str << " " << (double)node.hyperperiodsecs();
			_SchedHeaderLines.push_back( out.str() );
		}

		_already_included.clear();  // clear include list for each node

		// Visit all tasks assigned to this node
		TaskSet taskSet = node.executes();
		TaskVector taskVector( taskSet.begin(), taskSet.end() );
		TaskVector::iterator taskIter = taskVector.begin();
		for ( ; taskIter != taskVector.end(); taskIter++ ){
			// Visit the task
			taskIter->Accept( *this );
		}

		// Clear the analog in/out counters
		_analogIn = 1;
		_analogOut = 1;

		// Set up a sorted list of IChans and OChans
		SortedIChan_ByChanIndex_Set ichans;
		SortedOChan_ByChanIndex_Set ochans;

		// Visit all devices contained in this node
		DeviceSet deviceSet = node.integrates();
		DeviceSet::iterator deviceIter = deviceSet.begin();
		for ( ; deviceIter != deviceSet.end(); deviceIter++ ) {

			// Collect all of the IChans and OChans, and keep them in the proper sorted order
			// See if device is input
			if ( Semantics::InputDevice::meta == (*deviceIter).type() ) {

				// Cast to an InputDevice
				Semantics::InputDevice input = Semantics::InputDevice::Cast( *deviceIter );

				// Get the associated IChans
				SortedIChan_ByChanIndex_Set iChanSet = input.inputChannels_sorted( IChanIndexSorter() );
				ichans.insert( iChanSet.begin(), iChanSet.end() );
			}

			// See if device is output
			else if ( Semantics::OutputDevice::meta == (*deviceIter).type() ) {

				// Cast to an InputDevice
				Semantics::OutputDevice output = Semantics::OutputDevice::Cast( *deviceIter );

				// Get the associated OChans
				SortedOChan_ByChanIndex_Set oChanSet = output.outputChannels_sorted( OChanIndexSorter() );
				ochans.insert( oChanSet.begin(), oChanSet.end() );
			}
		}

		// Now process the IChans and OChans in order
		IChanVector iChanVector( ichans.begin(), ichans.end() );
		for ( IChanVector::iterator ichanIter = iChanVector.begin(); ichanIter != iChanVector.end(); ichanIter++ )
		{
			ichanIter->Accept( *this );
		}

		OChanVector oChanVector( ochans.begin(), ochans.end() );
		for ( OChanVector::iterator ochanIter = oChanVector.begin(); ochanIter != oChanVector.end(); ochanIter++ )
		{
			ochanIter->Accept( *this );
		}

		// Visit all local dependencies on this node
		LocalDependencySet dependencySet = node.nodeLocalDeps();
		LocalDependencyVector dependencyVector( dependencySet.begin(), dependencySet.end() );
		LocalDependencyVector::iterator dependencyIter = dependencyVector.begin();
		for ( ; dependencyIter != dependencyVector.end(); dependencyIter++ ) {

			// Visit the dependency
			dependencyIter->Accept( *this );
		}

		// Visit the dummy dependency message on this node (if we have one)
		Semantics::Msg dummyMsg = node.nodeDummyMsg();
		if ( dummyMsg != Udm::null )
			Visit_DummyMessage( dummyMsg );


		// Get the template file name
		std::string templateName = ConfigKeeper::inst().GetTemplatePath() + "\\truetime_src.tpl";
		// Initialize the template 
		ctemplate::Template* googleTemplate = ctemplate::Template::GetTemplate( templateName, ctemplate::DO_NOT_STRIP );
		std::string output;
		// Expand the output into a string
		googleTemplate->Expand( &output, &GetMainDictionary() );
		
		// Create the _init file for this TT kernel
		std::string directoryName = ConfigKeeper::inst().GetDirectoryName();
		string srcfilename = directoryName + "\\" + filename + ".cpp";
		ofstream initFile( srcfilename.c_str() );
		// Write the generated code out to the file and close
		initFile << output;
		// Close up the file
		initFile.close();
		// Clear out the dictionary
		ClearDictionary();

		// Write out the header file for the defines
		string hdrfilename = directoryName + "\\" + filename + "_defines.h";
		cout << "Writing header: " << hdrfilename << endl;
		ofstream headerFile( hdrfilename.c_str() );

		for ( std::vector< std::string >::iterator lineIter = _SchedHeaderLines.begin(); 
				lineIter != _SchedHeaderLines.end(); lineIter++ )
		{
			headerFile << *lineIter << endl;
		}
		headerFile.close();
		_SchedHeaderLines.clear(); // flush the list
}


void TrueTimeSourceVisitor::Visit_LocalDependency( const Semantics::LocalDependency &dependency ) {

	// Get the associated message
	Semantics::Msg message = dependency.depMsg();
	//Create the qualified message structure
	Semantics::Channel channel;
	QualifiedMessage qualifiedMessage = { message, channel, TRANSMIT };
	// See what direction we should categorize this message as ( default is TRANSMIT )
	Semantics::Task task = dependency.localDepSendingTask();
	if ( task == Udm::null ) {
		// Change the message direction to RECEIVE
		qualifiedMessage.type = RECEIVE;
	}
	// Visit the message
	Visit_QualifiedMessageStructure( qualifiedMessage, task );
}


void TrueTimeSourceVisitor::Visit_Task( const Semantics::Task &task ) {

	// Get the task name
	std::string taskName = task.name();
	DEBUGOUT( "\tTask: " << taskName << std::endl );

	// Process messages for this task
	QualifiedMessageSet messageSet = GetQualifiedMessages( task );
	QualifiedMessageSet::iterator messageIter = messageSet.begin();
	for ( ; messageIter != messageSet.end() ; messageIter++ ) {
		// Visit the message to generate template section
		Visit_QualifiedMessageStructure( *messageIter, task );
	}

	// Get the exec info and schedule
	ExecInfoSet execInfoSet = task.info();
	ExecInfoSet::iterator execInfoIter = execInfoSet.begin();
	Semantics::Duration duration = execInfoIter->Duration_child();
	double wcet = duration.exectimesecs();
	
	// Add a section for the task execution
	AddSectionDictionary( "TASK_SECTION" );
	string wcet_symbol = taskName + "_WCET";
	GetTemplateDictionary().SetValue( "TASK_WCET_STR", wcet_symbol );
	GetTemplateDictionary().SetFormattedValue( "TASK_WCET", "%f", wcet );
	
	ostringstream out1;
	out1 << "#define " << wcet_symbol << " " << wcet;
	_SchedHeaderLines.push_back( out1.str() );

	// Setup the peripheral schedule
	Semantics::Schedule schedule = execInfoIter->Schedule_child();
	std::string sched = schedule.sched();
	int count = sched_count( sched );
	vector< double > start_times = sched_values( sched );
	ostringstream symbolic_array;
	for ( int idx = 0; idx < count; idx++ )
	{
		ostringstream out2;
		out2 << "#define " << taskName << "_" << idx << " " << start_times[idx];
		_SchedHeaderLines.push_back( out2.str() );
		if ( idx > 0 )
		{
			symbolic_array << ", ";
		}
		symbolic_array << taskName << "_" << idx;
	}

	GetTemplateDictionary().SetIntValue( "TASK_COUNT", count );
	//GetTemplateDictionary().SetValue( "TASK_SCHEDULE", sched );
	GetTemplateDictionary().SetValue( "TASK_SCHEDULE", symbolic_array.str() );

	// Get the set of components this task invokes
	ComponentSet componentSet = task.invokes();
	
	// If there are components...
	if ( !componentSet.empty() ) {
	
		// Start with the first component
		Semantics::Component component = *componentSet.begin();
		
		// Get the component name
		std::string compName = component.name();
		GetTemplateDictionary().SetValue( "TASK_NAME", compName + "_task" );
		GetTemplateDictionary().SetValue( "TASK_FUNCTION", compName + "_code" );
		
		// Create the list of component header files
		StringList headerList = Split( component.cfiles() );
		StringList::iterator headerIter = headerList.begin();
		// Iterate through the list
		for( ; headerIter != headerList.end() ; headerIter++ ) {

			if ( _already_included.find( *headerIter ) == _already_included.end() )
			{
				// Include a header section and populate it
				AddSectionDictionary( "TASK_INCLUDE" );
				GetTemplateDictionary().SetValue( "TASK_HEADER", *headerIter );
				PopSectionDictionary();
				_already_included.insert( *headerIter );
			}
		}
		
		// Set the component function name
		GetTemplateDictionary().SetValue( "TASK_COMPNAME", compName );
		std::string compMSubsys = component.msubsystem();
		
		// Account for 
		GetTemplateDictionary().SetValue( "TASK_MSUBSYS", compMSubsys );
		
		// Setup the input signals to the component
		SortedSignal_ByCallIndex_Set inputSignalSet = component.consumes_sorted( SignalCallIndexSorter() );
		SortedSignal_ByCallIndex_Set::iterator inputIter = inputSignalSet.begin() ;
		for( ; inputIter != inputSignalSet.end(); inputIter++ ) {
			AddSectionDictionary( "INPUT_SIGNALS" );
			Visit_Signal( *inputIter );
			PopSectionDictionary();
		}
		
		// Setup the output signals to the component
		SortedSignal_ByCallIndex_Set outputSignalSet = component.generates_sorted( SignalCallIndexSorter() );
		SortedSignal_ByCallIndex_Set::iterator outputIter = outputSignalSet.begin();
		for ( ; outputIter != outputSignalSet.end(); outputIter++ ) {
			AddSectionDictionary( "OUTPUT_SIGNALS" );
			Visit_Signal( *outputIter );
			PopSectionDictionary();
		}
	}

	// Done with section dictionary
	PopSectionDictionary();
}

// Deprecated 8/29/2011 JEP - IChans and OChans are collected per-node for sorting
//void TrueTimeSourceVisitor::Visit_Device( const Semantics::Device &device ) {
//	
//	// See if device is input
//	if ( Semantics::InputDevice::meta == device.type() ) {
//		// Cast to an InputDevice
//		Semantics::InputDevice input = Semantics::InputDevice::Cast( device );
//		// Get the associated IChans
//		IChanSet iChanSet = input.inputChannels();
//		IChanSet::iterator iChanIter = iChanSet.begin();
//		// Visit the IChans
//		for ( ; iChanIter != iChanSet.end(); iChanIter++ ) {
//			iChanIter->Accept( *this );
//		}
//	}
//	
//	// See if device is output
//	else if ( Semantics::OutputDevice::meta == device.type() ) {
//		// Cast to an InputDevice
//		Semantics::OutputDevice output = Semantics::OutputDevice::Cast( device );
//		// Get the associated OChans
//		OChanSet oChanSet = output.outputChannels();
//		OChanSet::iterator oChanIter = oChanSet.begin();
//		// Visit the OChans
//		for ( ; oChanIter != oChanSet.end(); oChanIter++ ) {
//			oChanIter->Accept( *this );
//		}
//	}
//	// See if device is CommInterface
//	else if ( Semantics::CommInterface::meta == device.type() ) {
//		// Nothing for now...
//	}
//}


void TrueTimeSourceVisitor::Visit_IChan( const Semantics::IChan &iChan ) {
	
	// Get the IChan name
	std::string iChanName = iChan.name();
	DEBUGOUT( "\tIChan: " << iChanName << std::endl );
	// Include a peripheral section and populate it
	string peripheralTaskName = iChanName + "_task";
	AddSectionDictionary( "PERIPHERAL_SECTION" );
	GetTemplateDictionary().SetValue( "PERIPHERAL_NAME", peripheralTaskName );
	GetTemplateDictionary().SetValue( "PERIPHERAL_FUNCTION", iChanName + "_code" );

	// Get all signals in message
	Semantics::Acquisition acquisition = iChan.channelAcquisition();
	// Make sure there is an acquisition
	if ( acquisition != Udm::null ) {
		// Setup the data input code
		AddSectionDictionary( "PERIPHERAL_INPUT" );
		Semantics::Msg msg = acquisition.acquiredMsg();
		SortedSignal_ByMsgIndex_Set signalSet = msg.carries_sorted( SignalMsgIndexSorter() );
		SortedSignal_ByMsgIndex_Set::iterator signalIter = signalSet.begin();
		// Loop through all signals
		for ( ; signalIter != signalSet.end(); signalIter++ ) {
			// Get the signal type info
			Semantics::SignalType type = signalIter->isOfType();
			std::string typeName = type.getStringAttr( Semantics::SignalType::meta_name );
			int numSignals = SignalSize( typeName );
			// Loop through each signal
			for ( int i=0; i < numSignals; i++ ) {
				// Get the style of this signal
				std::string signalName = SignalName( typeName, i );
				// Add a PERIPHERAL_MESSAGE section
				AddSectionDictionary( "PERIPHERAL_MESSAGE" );
				// Set the template values for the section
				GetTemplateDictionary().SetIntValue( "PERIPHERAL_CHANNEL", _analogIn++ );
				std::string messageName = msg.name();
				string::size_type pos = messageName.find(".");
				if ( pos != string::npos ) messageName.replace( pos , 1, "_" );
				std::string fullName = messageName + "->" + std::string( signalIter->name() ) + signalName;
				GetTemplateDictionary().SetValue( "PERIPHERAL_MSGNAME", fullName );
				PopSectionDictionary();
			}
		}
		// Close the peripheral input section
		PopSectionDictionary();
	}
	// There are problems...
	else {
		DEBUGOUT( "*** Error: This IChan has no signals - Check your model.\n" );
	}

	// Setup the peripheral schedule
	ExecInfoSet execInfoSet = iChan.info();
	// Make sure there is a schedule
	if ( execInfoSet.size() == 1 ) {
		ExecInfoSet::iterator execInfoIter = execInfoSet.begin();
		Semantics::Duration duration = execInfoIter->Duration_child();

		double wcet = duration.exectimesecs();
		string wcet_symbol = peripheralTaskName + "_WCET";
		ostringstream out1;
		out1 << "#define " << wcet_symbol << " " << wcet;
		_SchedHeaderLines.push_back( out1.str() );
		
		GetTemplateDictionary().SetValue( "PERIPHERAL_WCET_STR", wcet_symbol );
		GetTemplateDictionary().SetFormattedValue( "PERIPHERAL_WCET", "%f", wcet );

		Semantics::Schedule schedule = execInfoIter->Schedule_child();
		std::string sched = schedule.sched();
		int count = sched_count( sched );
		vector< double > start_times = sched_values( sched );
		ostringstream symbolic_array;
		for ( int idx = 0; idx < count; idx++ )
		{
			ostringstream out2;
			out2 << "#define " << peripheralTaskName << "_" << idx << " " << start_times[idx];
			_SchedHeaderLines.push_back( out2.str() );
			if ( idx > 0 )
			{
				symbolic_array << ", ";
			}
			symbolic_array << peripheralTaskName << "_" << idx;
		}
		GetTemplateDictionary().SetIntValue( "PERIPHERAL_COUNT", count );
		GetTemplateDictionary().SetValue( "PERIPHERAL_SCHEDULE", symbolic_array.str() );
	}
	// There are problems...
	else {
		DEBUGOUT( "*** Error: This IChan has no schedule - Check your model.\n" );
		GetTemplateDictionary().SetFormattedValue( "PERIPHERAL_WCET", "%f", 0.01 );
		GetTemplateDictionary().SetIntValue( "PERIPHERAL_COUNT", 1 );
		GetTemplateDictionary().SetValue( "PERIPHERAL_SCHEDULE", "-1.0" );
	}
	// Close the peripheral section
	PopSectionDictionary();
}


void TrueTimeSourceVisitor::Visit_OChan( const Semantics::OChan &oChan) {

	// Get the OChan name
	std::string oChanName = oChan.name();
	DEBUGOUT( "\tOChan: " << oChanName << std::endl );

	// Include a peripheral section and populate it
	string peripheralTaskName = oChanName + "_task";	
	AddSectionDictionary( "PERIPHERAL_SECTION" );
	GetTemplateDictionary().SetValue( "PERIPHERAL_NAME", peripheralTaskName );
	GetTemplateDictionary().SetValue( "PERIPHERAL_FUNCTION", oChanName + "_code" );

	// Get all signals in message
	ActuationSet actuatorSet = oChan.channelActuations();

	// Make sure there are signals
	if ( actuatorSet.size() != 0 ) {

		// Setup the data input code
		AddSectionDictionary( "PERIPHERAL_OUTPUT" );
		Semantics::Msg msg = actuatorSet.begin()->actuatingMsg();
		SortedSignal_ByMsgIndex_Set signalSet = msg.carries_sorted( SignalMsgIndexSorter() );
		SortedSignal_ByMsgIndex_Set::iterator signalIter = signalSet.begin();

		// Loop through all signals
		for ( ; signalIter != signalSet.end(); signalIter++ ) {

			// Get the signal type info
			Semantics::SignalType type = signalIter->isOfType();
			std::string typeName = type.getStringAttr( Semantics::SignalType::meta_name );
			int numSignals = SignalSize( typeName );

			// Loop through each signal
			for ( int i=0; i < numSignals; i++ ) {

				// Get the style of this signal
				std::string signalName = SignalName( typeName, i );
				// Add a PERIPHERAL_MESSAGE section
				AddSectionDictionary( "PERIPHERAL_MESSAGE" );
				// Set the template values for the section
				GetTemplateDictionary().SetIntValue( "PERIPHERAL_CHANNEL", _analogOut++ );
				std::string messageName = msg.name();
				string::size_type pos = messageName.find(".");
				if ( pos != string::npos ) messageName.replace( pos , 1, "_" );
				std::string fullName = messageName + "->" + std::string( signalIter->name() ) + signalName;
				GetTemplateDictionary().SetValue( "PERIPHERAL_MSGNAME", fullName );
				PopSectionDictionary();
			}
		}
		// Close the peripheral input section
		PopSectionDictionary();
	}
	// There are problems...
	else {
		DEBUGOUT( "*** Error: This OChan has no signals - Check your model.\n" );
	}

	// Get the exec info and schedule
	ExecInfoSet execInfoSet = oChan.info();
	// Make sure there is a schedule
	if ( execInfoSet.size() == 1 ) {
		ExecInfoSet::iterator execInfoIter = execInfoSet.begin();
		Semantics::Duration duration = execInfoIter->Duration_child();

		double wcet = duration.exectimesecs();
		string wcet_symbol = peripheralTaskName + "_WCET";
		ostringstream out1;
		out1 << "#define " << wcet_symbol << " " << wcet;
		_SchedHeaderLines.push_back( out1.str() );
		
		GetTemplateDictionary().SetValue( "PERIPHERAL_WCET_STR", wcet_symbol );
		GetTemplateDictionary().SetFormattedValue( "PERIPHERAL_WCET", "%f", wcet );
		
		Semantics::Schedule schedule = execInfoIter->Schedule_child();
		std::string sched = schedule.sched();
		int count = sched_count( sched );
		vector< double > start_times = sched_values( sched );
		ostringstream symbolic_array;
		for ( int idx = 0; idx < count; idx++ )
		{
			ostringstream out2;
			out2 << "#define " << peripheralTaskName << "_" << idx << " " << start_times[idx];
			_SchedHeaderLines.push_back( out2.str() );
			if ( idx > 0 )
			{
				symbolic_array << ", ";
			}
			symbolic_array << peripheralTaskName << "_" << idx;
		}
		GetTemplateDictionary().SetIntValue( "PERIPHERAL_COUNT", count );
		GetTemplateDictionary().SetValue( "PERIPHERAL_SCHEDULE", symbolic_array.str() );
	}
	// There are problems...
	else {
		DEBUGOUT( "*** Error: This OChan has no schedule - Check your model.\n" );
		GetTemplateDictionary().SetFormattedValue( "PERIPHERAL_WCET", "%f", 0.01 );
		GetTemplateDictionary().SetIntValue( "PERIPHERAL_COUNT", 1 );
		GetTemplateDictionary().SetValue( "PERIPHERAL_SCHEDULE", "-1.0" );
	}
	// We are done here
	PopSectionDictionary();
}

void TrueTimeSourceVisitor::Visit_Signal( const Semantics::Signal &signal, bool isParameter ) {
	MsgSet msgSet = signal.carriedBy();
	MsgVector msgVector( msgSet.begin(), msgSet.end() );
	TTMsgSet ttMsgSet;
	// Cast all of the signals to...
	for( MsgVector::iterator mgsIter = msgVector.begin() ; mgsIter != msgVector.end() ; (void)++mgsIter ) {
		ttMsgSet.insert( Semantics::Msg::Cast( *mgsIter ) );
	}

	if ( ttMsgSet.size() == 0 )
	{
		DEBUGOUT( "*** Error: This signal is not connected to any message - Check your model.\n" );
		return;
	}
	Semantics::Msg ttMsg = *ttMsgSet.begin();
	// Hack to remove the '.' in the message name
	std::string messageName = ttMsg.name();
	string::size_type pos = messageName.find(".");
	if ( pos != string::npos ) messageName.replace( pos, 1, "_" );
	GetTemplateDictionary().SetValue( "SIGNAL_MSGNAME", messageName );

	std::string signalName = signal.name();
	if ( isParameter ) {
		std::string::size_type pos = signalName.rfind( "__" );
		if ( pos != std::string::npos ) signalName = signalName.substr( pos + 2 );
	}
	Semantics::SignalType signalType = signal.isOfType();
	if ( signalType != Udm::null ) {
		GetTemplateDictionary().SetValue(  "SIGNAL_TYPE", std::string( signalType.ctypestr() )  );
	}
	GetTemplateDictionary().SetValue( "SIGNAL_NAME", signalName );
}


void TrueTimeSourceVisitor::Visit_QualifiedMessageStructure( const QualifiedMessage & qualifiedMessage, 
													   const Semantics::Task & task ) {
	// Get the actual message object
	Semantics::Msg message = qualifiedMessage.message;
	// Create a new section for it
	AddSectionDictionary( "MESSAGE_SECTION" );
	// Get the message name
	std::string messageName = message.name();
	// Figure out non-dotted name
	string::size_type pos = messageName.find(".");
	if ( pos != string::npos )
		messageName.replace( pos, 1, "_" );
	DEBUGOUT( "\tMessage: " << messageName << std::endl );

	std::string messageType = message.messagetype();
	GetTemplateDictionary().SetValue( "MESSAGE_NAME", messageName );

	// Get the associated signals
	SignalSet signalSet = message.carries();
	SignalSet::iterator signalIter = signalSet.begin();

	SortedSignal_ByMsgIndex_Set filteredSignalSet;

	// Loop through all signals
	for( ; signalIter != signalSet.end() ; signalIter++ ) {
		std::string signalName = signalIter->name();
		ComponentSet receivingComponentSet = signalIter->consumedBy();
		Semantics::Component transmittingComponent = signalIter->generatedBy();
		// Check to make sure for something?!?!?!?!?
		if ( qualifiedMessage.type == TRANSMIT && receivingComponentSet.empty() ||
			 qualifiedMessage.type == RECEIVE && transmittingComponent == Udm::null ||
			 qualifiedMessage.type == PERIPHERAL ) {
				// Insert the signal into the sorted set
				filteredSignalSet.insert( *signalIter );
		}
	}

	// Use the constructed sorted signal set to fill in the template
	SortedSignal_ByMsgIndex_Set::iterator filteredSignalIter = filteredSignalSet.begin();
	for ( ; filteredSignalIter != filteredSignalSet.end(); filteredSignalIter++ ) {
		Visit_Signal_Member( *filteredSignalIter );
	}

	// Finish up with this section
	PopSectionDictionary();

	// See if message is remote
	if ( messageType.compare( "Remote" ) == 0 ) {

		Semantics::CommMedium cm = GetMessageBus( message );
		string busName = "bus";
		if ( cm != Udm::null )
		{
			busName = cm.name();
		}

		AddSectionDictionary( "BUSMESSAGE_SECTION" );
		string msgTaskName = messageName + "_task";
		GetTemplateDictionary().SetValue( "BUSMESSAGE_NAME", msgTaskName );
		GetTemplateDictionary().SetValue( "BUSMESSAGE_FULLNAME", messageName );
		GetTemplateDictionary().SetValue( "BUSMESSAGE_FUNCTION", messageName + "_code" );
		// Get the exec info and schedule
		ExecInfoSet execInfoSet = message.info();
		ExecInfoSet::iterator execInfoIter = execInfoSet.begin();
		Semantics::Duration duration = execInfoIter->Duration_child();
		double wcet = duration.exectimesecs();
		string wcet_symbol = msgTaskName + "_WCET";
		ostringstream out1;
		out1 << "#define " << wcet_symbol << " " << wcet;
		_BusHeaderLines[busName].insert( out1.str() );

		GetTemplateDictionary().SetValue( "BUSMESSAGE_WCET_STR", wcet_symbol );
		GetTemplateDictionary().SetFormattedValue( "BUSMESSAGE_WCET", "%f", wcet );
		// Setup the peripheral schedule
		Semantics::Schedule schedule = execInfoIter->Schedule_child();
		std::string sched = schedule.sched();
		int count = sched_count( sched );
		vector< double > start_times = sched_values( sched );
		ostringstream symbolic_array;
		for ( int idx = 0; idx < count; idx++ )
		{
			ostringstream out2;
			out2 << "#define " << msgTaskName << "_" << idx << " " << start_times[idx];
			_BusHeaderLines[busName].insert( out2.str() );
			if ( idx > 0 )
			{
				symbolic_array << ", ";
			}
			symbolic_array << msgTaskName << "_" << idx;
		}


		GetTemplateDictionary().SetIntValue( "BUSMESSAGE_COUNT", count );
		GetTemplateDictionary().SetValue( "BUSMESSAGE_SCHEDULE", symbolic_array.str() );

		// Get the network that transmits the message
		ReceivesSet receiveSet = message.msgListeners();
		// TODO: Need to figure out how to disambiguate receivers for this message
		Semantics::BChan bChan = receiveSet.begin()->receivingChannel();
		Semantics::CommInterface commInterface = bChan.chanInterface();
		Semantics::CommMedium network = commInterface.commMedium();
		// Find the network in the map
		std::map< std::string, TTNetwork >::const_iterator mapIter = _networkMap.find( network.name() );
		// Setup the message handling code
		Semantics::Transmits transmitter = message.msgTransmitter();
		Semantics::Task sendingTask = transmitter.sendingTask();
		// See if this task is being sent or received
		if ( sendingTask == task ) {
			// Add the appropriate section
			AddSectionDictionary( "BUSMESSAGE_SEND" );
			// Loop on all receivers
			ReceivesSet::iterator receivesIter = receiveSet.begin();
			for ( ; receivesIter != receiveSet.end(); receivesIter++ ) {
				AddSectionDictionary( "BUSMESSAGE_SENDBLOCK" );
				// Which node is receiving
				Semantics::Task task = receivesIter->receivingTask();
				Semantics::Node node = task.executedBy();
				// Find the node in the mapIter
				std::map< std::string, int >::iterator nodeIter = mapIter->second.nodes->find( node.name() );
				// Set the network
				GetTemplateDictionary().SetIntValue( "BUSMESSAGE_NETWORK", mapIter->second.id );
				// Set the receiver
				GetTemplateDictionary().SetIntValue( "BUSMESSAGE_RECEIVER", nodeIter->second );
				// Done with this send block
				PopSectionDictionary();
			}
			// Done with the BUSMESSAGE_SEND
			PopSectionDictionary();
		}
		// Must be receiving
		else {
			// Add the appropriate section
			AddSectionDictionary( "BUSMESSAGE_RECEIVE" );
			// Set the network ID
			GetTemplateDictionary().SetIntValue( "BUSMESSAGE_NETWORK", mapIter->second.id );
			// Done with this section
			PopSectionDictionary();
		}
		PopSectionDictionary();
	}
}


void TrueTimeSourceVisitor::Visit_Signal_Member( const Semantics::Signal &signal ) {

	std::string signalName = signal.name();
	DEBUGOUT( "\t\tSignal Member: " << signalName << std::endl );
	// Get the signal type
	Semantics::SignalType signalType = signal.isOfType();
	// Create a new section for the signal member
	AddSectionDictionary( "SIGNAL_MEMBER" );
	std::string signalTypeName = signalType.ctypestr();
	// Make sure the signalTypeName is valid
	if ( signalTypeName.empty() )
		signalTypeName = signalType.mtypestr();
	// Populate the local dictionary
	GetTemplateDictionary().SetValue( "SIGNAL_TYPENAME", signalTypeName );
	GetTemplateDictionary().SetValue( "SIGNAL_NAME", signalName );
	// We are done with this...
	PopSectionDictionary();
}

void TrueTimeSourceVisitor::Visit_DummyMessage( Semantics::Msg & dummyMsg )
{
	Semantics::Channel channel;
	QualifiedMessage qualifiedMessage = { dummyMsg, channel, TRANSMIT };
	DummyDependencySet deps = dummyMsg.dummyMsgDeps();
	Semantics::DummyDependency dep1 = *deps.begin();
	Semantics::Task task = dep1.sendingTask(); // this doesn't make sense, since the message
	                                           //  may be related to multiple tasks, but the
	                                           // Visiting method doesn't use the task parameter
	                                           // unless it's a remote message.

	// Visit the message
	Visit_QualifiedMessageStructure( qualifiedMessage, task );
}

/***************************************************************************************/
