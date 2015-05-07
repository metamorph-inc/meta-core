/*** Warning Workaround ***/
#pragma warning( disable: 4251 )


/*** Included Header Files ***/
#include "TTVisitor.h"
#include "ConfigKeeper.h"
#include "Uml.h"
#include <boost/Filesystem.hpp>


/***************************************************************************************/

// Automatic registration of the TrueTime visitor
bool TrueTimeVisitor::_initialized = 
	VisitorFactory::RegisterInstantiator( std::string("TrueTime"), &TrueTimeVisitor::GetTrueTimeVisitor );


//static int sched_count( std::string sched ) {
//	// Double check the empty case
//	if ( sched == "" ) return 0;
//	int count = 1;
//	std::string::size_type pos;
//	// Count the number of commas
//	while ( ( pos = sched.find(",") ) != std::string::npos ) count++;
//	return count;
//}


int TrueTimeVisitor::SignalSize( const std::string &name ) {
	// Check for simple single cases
	if ( name == "unsigned short" || name == "float" || name == "double" || name == "single" ) return 1;
	if ( name[0] == 'C' ) return 1;
	// Check matrix case
	if ( name[0] == 'M' ) {
		// Find the first 'C'
		int i = name.find_first_of( 'C' ) + 1;
		// Get the number of rows
		int width = 0;
		while ( name[i + width] >= '0' && name[i + width] <= '9' ) width++;
		std::string numRowStr = name.substr( i, width );
		int numRow = atoi( numRowStr.c_str() );
		
		// Get the number of columns
		i = name.find( 'C', i ) + 1;
		width = 0;
		while ( name[i + width] >= '0' && name[i + width] <= '9' ) width++;
		std::string numColStr = name.substr( i, width );
		int numCol = atoi( numColStr.c_str() );
		return numRow * numCol;
	}
	// Check struct case
	else if ( name[0] == 'S' ) {
		int count = 0, start = 1, end = 1;
		// Loop through all signals in the struct
		do {
			// Find the start of the signal
			start = name.find( "signal", start ) + 7;
			// Get the signal end
			end = name.find( name[start], start + 1 ) + 1;
			// Get the signal itself
			std::string subsignal = name.substr( start, end - start );
			// Get the size of the signal
			count += SignalSize( subsignal );
		}
		// The second clause is a workaround for a bug where last signal is not terminated properly
		while ( ( end != (int)name.size() - 1 ) && ( end != 0 ) );
		// Return the count of signals in the struct
		return count;
	}
	// Check array case
	else if ( name[0] == 'A' ) {
		// Get the number of elements in array
		int width = 0;
		int start = 1;
		while ( name[start + width] >= '0' && name[start + width] <= '9' ) width++;
		std::string numRowStr = name.substr( start, width );
		int numRow = atoi( numRowStr.c_str() );
		// Get the signal type itself
		int subStringWidth = name.find_last_of( "A" ) - start - width;
		std::string subsignal = name.substr( start + width, subStringWidth );
		return numRow * SignalSize( subsignal );
	}
	// Else, there is a problem
	else {
		std::cout << "Error in SignalSize: " << name << std::endl;
		return 0;
	}
}


std::string TrueTimeVisitor::SignalName( const std::string &type, const int &callIndex ) {
	// See if this is the simple case
	if ( type == "unsigned short" || type == "float" || type == "double" || type == "single" ) return "";
	if ( type[0] == 'C' ) return "";
	// Handle matrix indexing
	else if ( type[0] == 'M' ) {
		// Check to see if is a 1x1
		if ( type == "MsingleC1C1M" || type == "MdoubleC1C1M" ) return "";
		// Otherwise use array notation
		return "[" + boost::lexical_cast< std::string >( callIndex ) + "]";
	}
	// Handle array indexing
	else if ( type[0] == 'A' ) {
		// Check to see if is a 1x1
		if ( type == "A1singleA" || type == "A1doubleA" ) return "";
		// Otherwise use array notation
		return "[" + boost::lexical_cast< std::string >( callIndex ) + "]";
	}
	// Simple approach to structs for the moment
	else if ( type[0] == 'S' ) {
		std::string name = type;
		// Build the string ( remember the +1 offset )
		return ".signal" + boost::lexical_cast< std::string >( callIndex + 1 );
	}
	// Error case
	std::cout << "Error in SignalName: " << type << std::endl;
	return "";
}


/*void TrueTimeVisitor::DetermineAnalogIO( const Semantics::Node &node, int &inputs, int &outputs ) {
	// Reset the number of inputs and outputs
	inputs = 0;
	outputs = 0;
	// Get all devices
	DeviceSet deviceSet = node.integrates();
	DeviceSet::iterator deviceIter = deviceSet.begin();
	for ( ; deviceIter != deviceSet.end(); deviceIter++ ) {
		// See if device is input
		if ( Semantics::InputDevice::meta == deviceIter->type() ) {
			// Cast to an InputDevice
			Semantics::InputDevice inputDevice = Semantics::InputDevice::Cast( *deviceIter );
			// Get the associated IChans
			IChanSet iChanSet = inputDevice.inputChannels();
			IChanSet::iterator iChanIter = iChanSet.begin();
			// Visit the IChans
			for ( ; iChanIter != iChanSet.end(); iChanIter++ ) {
				// Get the acquisition
				Semantics::Acquisition acquisition = iChanIter->channelAcquisition();
				// Make sure there is an acquisition
				if ( acquisition != Udm::null ) {
					// Get the message
					Semantics::Msg msg = acquisition.acquiredMsg();
					// Get the set of signals
					SortedSignal_ByMsgIndex_Set signalSet = msg.carries_sorted( SignalMsgIndexSorter() );
					// Go through each signal
					SortedSignal_ByMsgIndex_Set::iterator signalIter = signalSet.begin();
					for ( ; signalIter != signalSet.end(); signalIter++ ) {
						// Get the signal type
						Semantics::SignalType signalType = signalIter->isOfType();
						std::string signalTypeStr = signalType.getStringAttr( Semantics::SignalType::meta_name );
						// Add signal set size to inputs
						inputs += SignalSize( signalTypeStr );
					}
				}
			}	// IChan
		}	// InputDevice
		// See if device is output
		if ( Semantics::OutputDevice::meta == deviceIter->type() ) {
			// Cast to an InputDevice
			Semantics::OutputDevice outputDevice = Semantics::OutputDevice::Cast( *deviceIter );
			// Get the associated OChans
			OChanSet oChanSet = outputDevice.outputChannels();
			OChanSet::iterator oChanIter = oChanSet.begin();
			// Visit the OChans
			for ( ; oChanIter != oChanSet.end(); oChanIter++ ) {
				// Get the actuators
				ActuationSet actuationSet = oChanIter->channelActuations();
				ActuationSet::iterator actuationIter = actuationSet.begin();
				for ( ; actuationIter != actuationSet.end(); actuationIter++ ) { 
					// Get the message
					Semantics::Msg msg = actuationIter->actuatingMsg();
					// Get the set of signals
					SortedSignal_ByMsgIndex_Set signalSet = msg.carries_sorted( SignalMsgIndexSorter() );
					// Go through each signal
					SortedSignal_ByMsgIndex_Set::iterator signalIter = signalSet.begin();
					for ( ; signalIter != signalSet.end(); signalIter++ ) {
						// Get the signal type
						Semantics::SignalType signalType = signalIter->isOfType();
						std::string signalTypeStr = signalType.getStringAttr( Semantics::SignalType::meta_mtypestr );
						// Add signal set size to inputs
						outputs += SignalSize( signalTypeStr );
					}
				}	// Actuation
			}	// OChan
		}	// OutputDevice
	}	// Devices
}
*/


/***************************************************************************************/


TrueTimeVisitor::TrueTimeVisitor( Semantics::RootFolder &rootFolder, TimeStamp &timeStamp ) :
	AbstractVisitor(rootFolder, timeStamp) {
}

TrueTimeVisitor::TrueTimeVisitor() : AbstractVisitor() { }

void TrueTimeVisitor::Visit_RootFolder( const Semantics::RootFolder & rootFolder ) {

	/*** Pass 1 - Generate the _gen.m files ***/
	TrueTimeSimulinkVisitor sl_gen;
	sl_gen.Visit_RootFolder( rootFolder );

	/*** Pass 2 - Generate the _init.cpp wrapper files ***/
	TrueTimeSourceVisitor src_gen( sl_gen.getNetworkMap() );
	src_gen.Visit_RootFolder( rootFolder );
}


//void TrueTimeVisitor::Visit_Deployment( const Semantics::Deployment &deployment ) {
//	// Check on which pass this is
//	if ( !_pass ) {
//		// Get set of TTBUs children - create TT Networks
//		TTBusVector ttbusVector = deployment.TTBus_kind_children();
//		// Visit them all
//		TTBusVector::iterator ttbusIter = ttbusVector.begin();
//		for ( ; ttbusIter != ttbusVector.end(); ttbusIter++ ) {
//			// Visit the TTBus
//			ttbusIter->Accept( *this );
//		}
//
//		// Get set of CANBus children - create CAN networks
//		CANBusVector canbusVector = deployment.CANBus_kind_children();
//		CANBusVector::iterator canbusIter = canbusVector.begin();
//		for ( ; canbusIter != canbusVector.end(); canbusIter++ )
//		{
//			canbusIter->Accept( *this );
//		}
//
//		// Get set of Node children - create TT Kernels
//		NodeVector nodeVector = deployment.Node_kind_children();
//		// Visit them all
//		NodeVector::iterator nodeIter = nodeVector.begin();
//		for ( ; nodeIter != nodeVector.end(); nodeIter++ ) {
//			// Visit the node
//			nodeIter->Accept( *this );
//		}
//
//		// Get set of Plant children - create simulink blocks
//		PlantSet plantSet = deployment.Plant_children();
//		// Visit them all
//		PlantSet::iterator plantIter = plantSet.begin();
//		for ( ; plantIter != plantSet.end(); plantIter++ ) {
//			// Visit the plant
//			plantIter->Accept( *this );
//		}
//	}
//	// Must be second pass
//	else {
//		// Get set of Node children - create _init files
//		NodeVector nodeVector = deployment.Node_children();
//		// Visit them all
//		NodeVector::iterator nodeIter = nodeVector.begin();
//		for ( ; nodeIter != nodeVector.end(); nodeIter++ ) {
//			// Visit the node again to gen _init files
//			nodeIter->Accept( *this );
//		}
//	}
//}


//void TrueTimeVisitor::Visit_Node( const Semantics::Node &node ) {
//
//	// Setup the node name
//	std::string kernelName = node.name();
//	std::string kernelInitName = kernelName + "_init";
//	DEBUGOUT( "\tNode: " << kernelName << std::endl );
//
//	// See if this is the first pass
//	if ( !_pass ) {
//		// Add a TTKernel for the node
//		AddSectionDictionary( "TTKERNEL_SECTION" );
//		GetTemplateDictionary().SetValue( "KERNEL_NAME", kernelName );
//		GetTemplateDictionary().SetValue( "KERNEL_INITFUNC", kernelInitName );
//		GetTemplateDictionary().SetValue( "KERNEL_ARGS", "[1]" );
//
//		// Set the number of input and output analog values
//		int inputs, outputs;
//		DetermineAnalogIO( node, inputs, outputs );
//		GetTemplateDictionary().SetFormattedValue( "KERNEL_IOPORTS", "[%d %d]", inputs, outputs );
//
//		// Determine network node number
//		std::string netNodeStr = "[";
//		DeviceSet deviceSet = node.integrates();
//		DeviceSet::iterator deviceIter = deviceSet.begin();
//		for ( ; deviceIter != deviceSet.end(); deviceIter++ ) {
//			// Make sure we are only looking at commInterfaces
//			if ( Semantics::CommInterface::meta == deviceIter->type() ) {
//				// Cast to a commInterface
//				Semantics::CommInterface commInterface = Semantics::CommInterface::Cast( *deviceIter );
//				// Get the associated CommMedium (i.e. TTNetwork)
//				Semantics::CommMedium medium = commInterface.commMedium();
//				// Get network name
//				std::string networkName = medium.name();
//				// Find the network in the map
//				std::map< std::string, TTNetwork >::iterator netIter = _networkMap.find( networkName );
//				TTNetwork network = netIter->second;
//				// Make sure the node is not already in this network (i.e. multi BChan to one TTBus)
//				if ( network.nodes->find( kernelName ) == network.nodes->end() ) { 
//					// Build the netNode String
//					int nodeNum = network.nodes->size() + 1;
//					// If nodeNum
//					netNodeStr += ";" + boost::lexical_cast<std::string>( network.id );
//					netNodeStr += " " + boost::lexical_cast<std::string>( nodeNum );
//					// Insert the node into the networkMap
//					network.nodes->insert( std::make_pair( kernelName, nodeNum ) );
//				}
//			}
//		}
//		// Close the network-node string
//		netNodeStr += "]";
//		// Remove leading ;
//		if ( netNodeStr.find_first_of( ";" ) != string::npos ) {
//			netNodeStr.erase( netNodeStr.find_first_of(";"), 1 );
//		}
//		GetTemplateDictionary().SetValue( "KERNEL_NETNODENUM", netNodeStr );
//		// Finally, set the schedule scope port on the kernel
//		if ( inputs == 0 || outputs == 0 )
//			GetTemplateDictionary().SetIntValue( "KERNEL_SCOPE_PORT", 1 );
//		else
//			GetTemplateDictionary().SetIntValue( "KERNEL_SCOPE_PORT", 2 );
//		// Done with section dictionary
//		PopSectionDictionary();
//	}
//	// Must be the second pass
//	else {
//		// Create the file name
//		std::string filename = std::string( node.name() ) + "_init";
//		// Set some dictionary items for this node
//		GetMainDictionary().SetValue( "FILENAME", filename );
//		GetMainDictionary().SetValue( "KERNEL_SCHEDULE", "prioFP" );
//		GetMainDictionary().SetFormattedValue( "KERNEL_HYPERPERIOD", "%f", (double)node.hyperperiodsecs() );
//
//		// Visit all tasks assigned to this node
//		TaskVector taskVector = node.executes();
//		TaskVector::iterator taskIter = taskVector.begin();
//		for ( ; taskIter != taskVector.end(); taskIter++ ){
//			// Visit the task
//			taskIter->Accept( *this );
//		}
//
//		// Clear the analog in/out counters
//		_analogIn = 1;
//		_analogOut = 1;
//		// Visit all devices contained in this node
//		DeviceSet deviceSet = node.integrates();
//		DeviceSet::iterator deviceIter = deviceSet.begin();
//		for ( ; deviceIter != deviceSet.end(); deviceIter++ ) {
//			// Visit the device
//			deviceIter->Accept( *this );
//		}
//
//		// Visit all local dependencies on this node
//		LocalDependencyVector dependencySet = node.nodeLocalDeps();
//		LocalDependencyVector::iterator dependencyIter = dependencySet.begin();
//		for ( ; dependencyIter != dependencySet.end(); dependencyIter++ ) {
//			// Visit the dependency
//			dependencyIter->Accept( *this );
//		}
//
//		// Visit the dummy dependency message on this node (if we have one)
//		Semantics::Msg dummyMsg = node.nodeDummyMsg();
//		if ( dummyMsg != Udm::null )
//			Visit_DummyMessage( dummyMsg );
//
//
//		// Get the template file name
//		std::string templateName = ConfigKeeper::inst().GetTemplatePath() + "\\truetime_src.tpl";
//		// Initialize the template 
//		ctemplate::Template* googleTemplate = ctemplate::Template::GetTemplate( templateName, ctemplate::DO_NOT_STRIP );
//		std::string output;
//		// Expand the output into a string
//		googleTemplate->Expand( &output, &GetMainDictionary() );
//		
//		// Create the _init file for this TT kernel
//		std::string directoryName = ConfigKeeper::inst().GetDirectoryName();
//		filename = directoryName + "\\" + filename + ".cpp";
//		ofstream initFile( filename.c_str() );
//		// Write the generated code out to the file and close
//		initFile << output;
//		// Close up the file
//		initFile.close();
//		// Clear out the dictionary
//		ClearDictionary();
//	}
//}


//void TrueTimeVisitor::Visit_LocalDependency( const Semantics::LocalDependency &dependency ) {
//	// Get the associated message
//	Semantics::Msg message = dependency.depMsg();
//	//Create the qualified message structure
//	Semantics::Channel channel;
//	QualifiedMessage qualifiedMessage = { message, channel, TRANSMIT };
//	// See what direction we should categorize this message as ( default is TRANSMIT )
//	Semantics::Task task = dependency.localDepSendingTask();
//	if ( task == Udm::null ) {
//		// Change the message direction to RECEIVE
//		qualifiedMessage.type = RECEIVE;
//	}
//	// Visit the message
//	Visit_QualifiedMessageStructure( qualifiedMessage, task );
//}


//void TrueTimeVisitor::Visit_Task( const Semantics::Task &task ) {
//	// Get the task name
//	std::string taskName = task.name();
//	DEBUGOUT( "\tTask: " << taskName << std::endl );
//
//	// Process messages for this task
//	QualifiedMessageSet messageSet = GetQualifiedMessages( task );
//	QualifiedMessageSet::iterator messageIter = messageSet.begin();
//	for ( ; messageIter != messageSet.end() ; messageIter++ ) {
//		// Visit the message to generate template section
//		Visit_QualifiedMessageStructure( *messageIter, task );
//	}
//
//	// Get the exec info and schedule
//	ExecInfoSet execInfoSet = task.info();
//	ExecInfoSet::iterator execInfoIter = execInfoSet.begin();
//	Semantics::Duration duration = execInfoIter->Duration_child();
//	double wcet = duration.exectimesecs();
//	// Add a section for the task execution
//	AddSectionDictionary( "TASK_SECTION" );
//	GetTemplateDictionary().SetFormattedValue( "TASK_WCET", "%f", wcet );
//	// Setup the peripheral schedule
//	Semantics::Schedule schedule = execInfoIter->Schedule_child();
//	std::string sched = schedule.sched();
//	GetTemplateDictionary().SetIntValue( "TASK_COUNT", sched_count( sched ) );
//	GetTemplateDictionary().SetValue( "TASK_SCHEDULE", sched );
//	// Get the set of components this task invokes
//	ComponentSet componentSet = task.invokes();
//	// If there are components...
//	if ( !componentSet.empty() ) {
//		// Start with the first component
//		Semantics::Component component = *componentSet.begin();
//		// Get the component name
//		std::string compName = component.name();
//		GetTemplateDictionary().SetValue( "TASK_NAME", compName + "_task" );
//		GetTemplateDictionary().SetValue( "TASK_FUNCTION", compName + "_code" );
//		// Create the list of component header files
//		StringList headerList = Split( component.cfiles() );
//		StringList::iterator headerIter = headerList.begin();
//		// Iterate through the list
//		for( ; headerIter != headerList.end() ; headerIter++ ) {
//			// Include a header section and populate it
//			AddSectionDictionary( "TASK_INCLUDE" );
//			GetTemplateDictionary().SetValue( "TASK_HEADER", *headerIter );
//			PopSectionDictionary();
//		}
//		// Set the component function name
//		GetTemplateDictionary().SetValue( "TASK_COMPNAME", compName );
//		std::string compMSubsys = component.msubsystem();
//		// Account for 
//		GetTemplateDictionary().SetValue( "TASK_MSUBSYS", compMSubsys );
//		// Setup the input signals to the component
//		SortedSignal_ByCallIndex_Set inputSignalSet = component.consumes_sorted( SignalCallIndexSorter() );
//		SortedSignal_ByCallIndex_Set::iterator inputIter = inputSignalSet.begin() ;
//		for( ; inputIter != inputSignalSet.end(); inputIter++ ) {
//			AddSectionDictionary( "INPUT_SIGNALS" );
//			Visit_Signal( *inputIter );
//			PopSectionDictionary();
//		}
//		// Setup the output signals to the component
//		SortedSignal_ByCallIndex_Set outputSignalSet = component.generates_sorted( SignalCallIndexSorter() );
//		SortedSignal_ByCallIndex_Set::iterator outputIter = outputSignalSet.begin();
//		for ( ; outputIter != outputSignalSet.end(); outputIter++ ) {
//			AddSectionDictionary( "OUTPUT_SIGNALS" );
//			Visit_Signal( *outputIter );
//			PopSectionDictionary();
//		}
//	}
//	// Done with section dictionary
//	PopSectionDictionary();
//}


//void TrueTimeVisitor::Visit_Device( const Semantics::Device &device ) {
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


//void TrueTimeVisitor::Visit_IChan( const Semantics::IChan &iChan ) {
//	// Get the IChan name
//	std::string iChanName = iChan.name();
//	DEBUGOUT( "\tIChan: " << iChanName << std::endl );
//	// Include a peripheral section and populate it
//	AddSectionDictionary( "PERIPHERAL_SECTION" );
//	GetTemplateDictionary().SetValue( "PERIPHERAL_NAME", iChanName + "_task" );
//	GetTemplateDictionary().SetValue( "PERIPHERAL_FUNCTION", iChanName + "_code" );
//
//	// Get all signals in message
//	Semantics::Acquisition acquisition = iChan.channelAcquisition();
//	// Make sure there is an acquisition
//	if ( acquisition != Udm::null ) {
//		// Setup the data input code
//		AddSectionDictionary( "PERIPHERAL_INPUT" );
//		Semantics::Msg msg = acquisition.acquiredMsg();
//		SortedSignal_ByMsgIndex_Set signalSet = msg.carries_sorted( SignalMsgIndexSorter() );
//		SortedSignal_ByMsgIndex_Set::iterator signalIter = signalSet.begin();
//		// Loop through all signals
//		for ( ; signalIter != signalSet.end(); signalIter++ ) {
//			// Get the signal type info
//			Semantics::SignalType type = signalIter->isOfType();
//			std::string typeName = type.getStringAttr( Semantics::SignalType::meta_name );
//			int numSignals = SignalSize( typeName );
//			// Loop through each signal
//			for ( int i=0; i < numSignals; i++ ) {
//				// Get the style of this signal
//				std::string signalName = SignalName( typeName, i );
//				// Add a PERIPHERAL_MESSAGE section
//				AddSectionDictionary( "PERIPHERAL_MESSAGE" );
//				// Set the template values for the section
//				GetTemplateDictionary().SetIntValue( "PERIPHERAL_CHANNEL", _analogIn++ );
//				std::string messageName = msg.name();
//				messageName.replace( messageName.find(".") , 1, "_" );
//				std::string fullName = messageName + "->" + std::string( signalIter->name() ) + signalName;
//				GetTemplateDictionary().SetValue( "PERIPHERAL_MSGNAME", fullName );
//				PopSectionDictionary();
//			}
//		}
//		// Close the peripheral input section
//		PopSectionDictionary();
//	}
//	// There are problems...
//	else {
//		DEBUGOUT( "*** Error: This IChan has no signals - Check your model.\n" );
//	}
//
//	// Setup the peripheral schedule
//	ExecInfoSet execInfoSet = iChan.info();
//	// Make sure there is a schedule
//	if ( execInfoSet.size() == 1 ) {
//		ExecInfoSet::iterator execInfoIter = execInfoSet.begin();
//		Semantics::Duration duration = execInfoIter->Duration_child();
//		double wcet = duration.exectimesecs();
//		GetTemplateDictionary().SetFormattedValue( "PERIPHERAL_WCET", "%f", wcet );
//		Semantics::Schedule schedule = execInfoIter->Schedule_child();
//		std::string sched = schedule.sched();
//		GetTemplateDictionary().SetIntValue( "PERIPHERAL_COUNT", sched_count( sched ) );
//		GetTemplateDictionary().SetValue( "PERIPHERAL_SCHEDULE", sched );
//	}
//	// There are problems...
//	else {
//		DEBUGOUT( "*** Error: This IChan has no schedule - Check your model.\n" );
//		GetTemplateDictionary().SetFormattedValue( "PERIPHERAL_WCET", "%f", 0.01 );
//		GetTemplateDictionary().SetIntValue( "PERIPHERAL_COUNT", 1 );
//		GetTemplateDictionary().SetValue( "PERIPHERAL_SCHEDULE", "-1.0" );
//	}
//	// Close the peripheral section
//	PopSectionDictionary();
//}


//void TrueTimeVisitor::Visit_OChan( const Semantics::OChan &oChan) {
//	// Get the OChan name
//	std::string oChanName = oChan.name();
//	DEBUGOUT( "\tOChan: " << oChanName << std::endl );
//	// Include a peripheral section and populate it
//	AddSectionDictionary( "PERIPHERAL_SECTION" );
//	GetTemplateDictionary().SetValue( "PERIPHERAL_NAME", oChanName + "_task" );
//	GetTemplateDictionary().SetValue( "PERIPHERAL_FUNCTION", oChanName + "_code" );
//
//	// Get all signals in message
//	ActuationSet actuatorSet = oChan.channelActuations();
//	// Make sure there are signals
//	if ( actuatorSet.size() != 0 ) {
//		// Setup the data input code
//		AddSectionDictionary( "PERIPHERAL_OUTPUT" );
//		Semantics::Msg msg = actuatorSet.begin()->actuatingMsg();
//		SortedSignal_ByMsgIndex_Set signalSet = msg.carries_sorted( SignalMsgIndexSorter() );
//		SortedSignal_ByMsgIndex_Set::iterator signalIter = signalSet.begin();
//		// Loop through all signals
//		for ( ; signalIter != signalSet.end(); signalIter++ ) {
//			// Get the signal type info
//			Semantics::SignalType type = signalIter->isOfType();
//			std::string typeName = type.getStringAttr( Semantics::SignalType::meta_name );
//			int numSignals = SignalSize( typeName );
//			// Loop through each signal
//			for ( int i=0; i < numSignals; i++ ) {
//				// Get the style of this signal
//				std::string signalName = SignalName( typeName, i );
//				// Add a PERIPHERAL_MESSAGE section
//				AddSectionDictionary( "PERIPHERAL_MESSAGE" );
//				// Set the template values for the section
//				GetTemplateDictionary().SetIntValue( "PERIPHERAL_CHANNEL", _analogOut++ );
//				std::string messageName = msg.name();
//				messageName.replace( messageName.find(".") , 1, "_" );
//				std::string fullName = messageName + "->" + std::string( signalIter->name() ) + signalName;
//				GetTemplateDictionary().SetValue( "PERIPHERAL_MSGNAME", fullName );
//				PopSectionDictionary();
//			}
//		}
//		// Close the peripheral input section
//		PopSectionDictionary();
//	}
//	// There are problems...
//	else {
//		DEBUGOUT( "*** Error: This OChan has no signals - Check your model.\n" );
//	}
//
//	// Get the exec info and schedule
//	ExecInfoSet execInfoSet = oChan.info();
//	// Make sure there is a schedule
//	if ( execInfoSet.size() == 1 ) {
//		ExecInfoSet::iterator execInfoIter = execInfoSet.begin();
//		Semantics::Duration duration = execInfoIter->Duration_child();
//		double wcet = duration.exectimesecs();
//		Semantics::Schedule schedule = execInfoIter->Schedule_child();
//		std::string sched = schedule.sched();
//		GetTemplateDictionary().SetFormattedValue( "PERIPHERAL_WCET", "%f", wcet );
//		GetTemplateDictionary().SetIntValue( "PERIPHERAL_COUNT", sched_count( sched ) );
//		GetTemplateDictionary().SetValue( "PERIPHERAL_SCHEDULE", sched );
//	}
//	// There are problems...
//	else {
//		DEBUGOUT( "*** Error: This OChan has no schedule - Check your model.\n" );
//		GetTemplateDictionary().SetFormattedValue( "PERIPHERAL_WCET", "%f", 0.01 );
//		GetTemplateDictionary().SetIntValue( "PERIPHERAL_COUNT", 1 );
//		GetTemplateDictionary().SetValue( "PERIPHERAL_SCHEDULE", "-1.0" );
//	}
//	// We are done here
//	PopSectionDictionary();
//}


//void TrueTimeVisitor::Visit_TTBus( const Semantics::TTBus &bus ) {
//	// Get the bus/network name
//	std::string networkName = bus.name();
//	double datarate = bus.dataratebps();
//	int id = _networkMap.size() + 1;
//	DEBUGOUT( "\tTTBus(" << id << "): " << networkName << std::endl );
//	// Insert network into NetworkMap
//	TTNetwork network = { id, new std::map< std::string, int >() };
//	_networkMap.insert( std::make_pair( networkName, network ) );
//	// Add a TTKernel for the node
//	AddSectionDictionary( "NETWORK_SECTION" );
//	GetTemplateDictionary().SetValue( "NETWORK_NAME", networkName );
//	GetTemplateDictionary().SetValue( "NETWORK_TYPE", "TDMA" );
//	GetTemplateDictionary().SetIntValue( "NETWORK_NUMBER", id );
//	GetTemplateDictionary().SetFormattedValue( "NETWORK_DATARATE", "%4.0f", datarate );
//	GetTemplateDictionary().SetValue( "NETWORK_FRAMESIZE", "512");
//	GetTemplateDictionary().SetValue( "NETWORK_LOSSPROB", "0");
//	GetTemplateDictionary().SetValue( "NETWORK_SEED", "0");
//	// Determine the number of nodes attached to the bus
//	int numNodes = 0;
//	// First get all of the comm interfaces attached to the bus
//	IfaceSet commInterfaceSet = bus.interfaces();
//	IfaceSet::iterator commInterfaceIter = commInterfaceSet.begin();
//	// Loop through the comm interfaces
//	for ( ; commInterfaceIter != commInterfaceSet.end(); commInterfaceIter++ ) {
//		// Get the bChan associated with each comm interface
//		BChanSet bChanSet = commInterfaceIter->commChannels();
//		// Add the number of bChans to the num nodes
//		numNodes += bChanSet.size();
//	}
//	// Now build the schedule string
//	std::string netSchedule = "[ ";
//	for ( int i=0; i < numNodes; i++ ) {
//		// Make sure to add 1 to i since TT network nodes are 1-indexed
//		netSchedule = netSchedule + boost::lexical_cast<std::string>(i+1) + std::string(" ");
//	}
//	netSchedule += "]";
//	AddSectionDictionary( "TT_SECTION" );
//	GetTemplateDictionary().SetValue( "NETWORK_SCHEDULE", netSchedule );
//	PopSectionDictionary();
//	GetTemplateDictionary().SetIntValue( "NETWORK_NUMNODES", numNodes );
//	// Done with section dictionary
//	PopSectionDictionary();
//}

//void TrueTimeVisitor::Visit_CANBus( const Semantics::CANBus & bus ) {
//	// Get the bus/network name
//	std::string networkName = bus.name();
//	double datarate = bus.dataratebps();
//	int id = _networkMap.size() + 1;
//	DEBUGOUT( "\tCANBus(" << id << "): " << networkName << std::endl );
//	// Insert network into NetworkMap
//	TTNetwork network = { id, new std::map< std::string, int >() };
//	_networkMap.insert( std::make_pair( networkName, network ) );
//	// Add a TTKernel for the node
//	AddSectionDictionary( "NETWORK_SECTION" );
//	GetTemplateDictionary().SetValue( "NETWORK_NAME", networkName );
//	GetTemplateDictionary().SetValue( "NETWORK_TYPE", "CSMA/AMP (CAN)" );
//	GetTemplateDictionary().SetIntValue( "NETWORK_NUMBER", id );
//	GetTemplateDictionary().SetFormattedValue( "NETWORK_DATARATE", "%4.0f", datarate );
//	GetTemplateDictionary().SetValue( "NETWORK_FRAMESIZE", "512");
//	GetTemplateDictionary().SetValue( "NETWORK_LOSSPROB", "0");
//	GetTemplateDictionary().SetValue( "NETWORK_SEED", "0");
//	// Determine the number of nodes attached to the bus
//	int numNodes = 0;
//	// First get all of the comm interfaces attached to the bus
//	IfaceSet commInterfaceSet = bus.interfaces();
//	IfaceSet::iterator commInterfaceIter = commInterfaceSet.begin();
//	// Loop through the comm interfaces
//	for ( ; commInterfaceIter != commInterfaceSet.end(); commInterfaceIter++ ) {
//		// Get the bChan associated with each comm interface
//		BChanSet bChanSet = commInterfaceIter->commChannels();
//		// Add the number of bChans to the num nodes
//		numNodes += bChanSet.size();
//	}
//	GetTemplateDictionary().SetIntValue( "NETWORK_NUMNODES", numNodes );
//	// Done with section dictionary
//	PopSectionDictionary();
//}

//void TrueTimeVisitor::Visit_Plant( const Semantics::Plant &plant ) {
//	// Get the plant name
//	std::string plantName = plant.name();
//	DEBUGOUT( "\tPlant: " << plantName << std::endl );
//	// Add a Subsystem for the plant
//	// ...
//
//	// Wire the plant to the nodes
//	// ...
//}


//void TrueTimeVisitor::Visit_Signal( const Semantics::Signal &signal, bool isParameter ) {
//	MsgVector msgVector = signal.carriedBy();
//	TTMsgSet ttMsgSet;
//	// Cast all of the signals to...
//	for( MsgVector::iterator mgsIter = msgVector.begin() ; mgsIter != msgVector.end() ; (void)++mgsIter ) {
//		ttMsgSet.insert( Semantics::Msg::Cast( *mgsIter ) );
//	}
//
//	if ( ttMsgSet.size() == 0 )
//	{
//		DEBUGOUT( "*** Error: This signal is not connected to any message - Check your model.\n" );
//		return;
//	}
//	Semantics::Msg ttMsg = *ttMsgSet.begin();
//	// Hack to remove the '.' in the message name
//	std::string messageName = ttMsg.name();
//	messageName.replace( messageName.find("."), 1, "_" );
//	GetTemplateDictionary().SetValue( "SIGNAL_MSGNAME", messageName );
//
//	std::string signalName = signal.name();
//	if ( isParameter ) {
//		std::string::size_type pos = signalName.rfind( "__" );
//		if ( pos != std::string::npos ) signalName = signalName.substr( pos + 2 );
//	}
//	Semantics::SignalType signalType = signal.isOfType();
//	if ( signalType != Udm::null ) {
//		GetTemplateDictionary().SetValue(  "SIGNAL_TYPE", std::string( signalType.ctypestr() )  );
//	}
//	GetTemplateDictionary().SetValue( "SIGNAL_NAME", signalName );
//}


//void TrueTimeVisitor::Visit_QualifiedMessageStructure( const QualifiedMessage & qualifiedMessage, 
//													   const Semantics::Task & task ) {
//	// Get the actual message object
//	Semantics::Msg message = qualifiedMessage.message;
//	// Create a new section for it
//	AddSectionDictionary( "MESSAGE_SECTION" );
//	// Get the message name
//	std::string messageName = message.name();
//	// Figure out non-dotted name
//	messageName.replace( messageName.find("."), 1, "_" );
//	DEBUGOUT( "\tMessage: " << messageName << std::endl );
//
//	std::string messageType = message.messagetype();
//	GetTemplateDictionary().SetValue( "MESSAGE_NAME", messageName );
//
//	// Get the associated signals
//	SignalSet signalSet = message.carries();
//	SignalSet::iterator signalIter = signalSet.begin();
//
//	SortedSignal_ByMsgIndex_Set filteredSignalSet;
//
//	// Loop through all signals
//	for( ; signalIter != signalSet.end() ; signalIter++ ) {
//		std::string signalName = signalIter->name();
//		ComponentSet receivingComponentSet = signalIter->consumedBy();
//		Semantics::Component transmittingComponent = signalIter->generatedBy();
//		// Check to make sure for something?!?!?!?!?
//		if ( qualifiedMessage.type == TRANSMIT && receivingComponentSet.empty() ||
//			 qualifiedMessage.type == RECEIVE && transmittingComponent == Udm::null ||
//			 qualifiedMessage.type == PERIPHERAL ) {
//				// Insert the signal into the sorted set
//				filteredSignalSet.insert( *signalIter );
//		}
//	}
//
//	// Use the constructed sorted signal set to fill in the template
//	SortedSignal_ByMsgIndex_Set::iterator filteredSignalIter = filteredSignalSet.begin();
//	for ( ; filteredSignalIter != filteredSignalSet.end(); filteredSignalIter++ ) {
//		Visit_Signal_Member( *filteredSignalIter );
//	}
//
//	// Finish up with this section
//	PopSectionDictionary();
//
//	// See if message is remote
//	if ( messageType.compare( "Remote" ) == 0 ) {
//		AddSectionDictionary( "BUSMESSAGE_SECTION" );
//		GetTemplateDictionary().SetValue( "BUSMESSAGE_NAME", messageName + "_task" );
//		GetTemplateDictionary().SetValue( "BUSMESSAGE_FULLNAME", messageName );
//		GetTemplateDictionary().SetValue( "BUSMESSAGE_FUNCTION", messageName + "_code" );
//		// Get the exec info and schedule
//		ExecInfoSet execInfoSet = message.info();
//		ExecInfoSet::iterator execInfoIter = execInfoSet.begin();
//		Semantics::Duration duration = execInfoIter->Duration_child();
//		double wcet = duration.exectimesecs();
//		GetTemplateDictionary().SetFormattedValue( "BUSMESSAGE_WCET", "%f", wcet );
//		// Setup the peripheral schedule
//		Semantics::Schedule schedule = execInfoIter->Schedule_child();
//		std::string sched = schedule.sched();
//		GetTemplateDictionary().SetIntValue( "BUSMESSAGE_COUNT", sched_count( sched ) );
//		GetTemplateDictionary().SetValue( "BUSMESSAGE_SCHEDULE", sched );
//
//		// Get the network that transmits the message
//		ReceivesSet receiveSet = message.msgListeners();
//		// TODO: Need to figure out how to disambiguate receivers for this message
//		Semantics::BChan bChan = receiveSet.begin()->receivingChannel();
//		Semantics::CommInterface commInterface = bChan.chanInterface();
//		Semantics::CommMedium network = commInterface.commMedium();
//		// Find the network in the map
//		std::map< std::string, TTNetwork >::iterator mapIter = _networkMap.find( network.name() );
//		// Setup the message handling code
//		Semantics::Transmits transmitter = message.msgTransmitter();
//		Semantics::Task sendingTask = transmitter.sendingTask();
//		// See if this task is being sent or received
//		if ( sendingTask == task ) {
//			// Add the appropriate section
//			AddSectionDictionary( "BUSMESSAGE_SEND" );
//			// Loop on all receivers
//			ReceivesSet::iterator receivesIter = receiveSet.begin();
//			for ( ; receivesIter != receiveSet.end(); receivesIter++ ) {
//				AddSectionDictionary( "BUSMESSAGE_SENDBLOCK" );
//				// Which node is receiving
//				Semantics::Task task = receivesIter->receivingTask();
//				Semantics::Node node = task.executedBy();
//				// Find the node in the mapIter
//				std::map< std::string, int >::iterator nodeIter = mapIter->second.nodes->find( node.name() );
//				// Set the network
//				GetTemplateDictionary().SetIntValue( "BUSMESSAGE_NETWORK", mapIter->second.id );
//				// Set the receiver
//				GetTemplateDictionary().SetIntValue( "BUSMESSAGE_RECEIVER", nodeIter->second );
//				// Done with this send block
//				PopSectionDictionary();
//			}
//			// Done with the BUSMESSAGE_SEND
//			PopSectionDictionary();
//		}
//		// Must be receiving
//		else {
//			// Add the appropriate section
//			AddSectionDictionary( "BUSMESSAGE_RECEIVE" );
//			// Set the network ID
//			GetTemplateDictionary().SetIntValue( "BUSMESSAGE_NETWORK", mapIter->second.id );
//			// Done with this section
//			PopSectionDictionary();
//		}
//		PopSectionDictionary();
//	}
//}


//void TrueTimeVisitor::Visit_Signal_Member( const Semantics::Signal &signal ) {
//
//	std::string signalName = signal.name();
//	DEBUGOUT( "\t\tSignal Member: " << signalName << std::endl );
//	// Get the signal type
//	Semantics::SignalType signalType = signal.isOfType();
//	// Create a new section for the signal member
//	AddSectionDictionary( "SIGNAL_MEMBER" );
//	std::string signalTypeName = signalType.ctypestr();
//	// Make sure the signalTypeName is valid
//	if ( signalTypeName.empty() )
//		signalTypeName = signalType.mtypestr();
//	// Populate the local dictionary
//	GetTemplateDictionary().SetValue( "SIGNAL_TYPENAME", signalTypeName );
//	GetTemplateDictionary().SetValue( "SIGNAL_NAME", signalName );
//	// We are done with this...
//	PopSectionDictionary();
//}

//void TrueTimeVisitor::Visit_DummyMessage( Semantics::Msg & dummyMsg )
//{
//	Semantics::Channel channel;
//	QualifiedMessage qualifiedMessage = { dummyMsg, channel, TRANSMIT };
//	DummyDependencySet deps = dummyMsg.dummyMsgDeps();
//	Semantics::DummyDependency dep1 = *deps.begin();
//	Semantics::Task task = dep1.sendingTask(); // this doesn't make sense, since the message
//	                                           //  may be related to multiple tasks, but the
//	                                           // Visiting method doesn't use the task parameter
//	                                           // unless it's a remote message.
//
//	// Visit the message
//	Visit_QualifiedMessageStructure( qualifiedMessage, task );
//}

/***************************************************************************************/



