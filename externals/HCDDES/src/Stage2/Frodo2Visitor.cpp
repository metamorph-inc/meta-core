/*** Warning Workaround ***/
#pragma warning( disable: 4251 )


/*** Included Header Files ***/
#include "Frodo2Visitor.h"
#include "ConfigKeeper.h"
#include "Uml.h"
#include <boost/Filesystem.hpp>


/***************************************************************************************/

// Automatic registration of the Frodo2 visitor
bool Frodo2Visitor::_initialized = 
	VisitorFactory::RegisterInstantiator( std::string("Frodo2"), &Frodo2Visitor::GetFrodo2Visitor );


static int sched_count( std::string sched ) {
	// Double check the empty case
	if ( sched == "" ) return 0;
	int count = 1;
	std::string::size_type pos;
	// Count the number of commas
	while ( ( pos = sched.find(",") ) != std::string::npos ) count++;
	return count;
}


int Frodo2Visitor::SignalSize( const std::string &name ) {
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
			count += this->SignalSize( subsignal );
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
		return numRow * this->SignalSize( subsignal );
	}
	// Else, there is a problem
	else {
		std::cout << "Error in SignalSize: " << name << std::endl;
		return 0;
	}
}


std::string Frodo2Visitor::SignalName( const std::string &type, const int &callIndex ) {
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

/*
void Frodo2Visitor::DetermineAnalogIO( const Semantics::Node &node, int &inputs, int &outputs ) {
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
					SignalSet signalSet = msg.carries();
					// Go through each signal
					SignalSet::iterator signalIter = signalSet.begin();
					for ( ; signalIter != signalSet.end(); signalIter++ ) {
						// Get the signal type
						Semantics::SignalType signalType = signalIter->isOfType();
						std::string signalTypeStr = signalType.getStringAttr( Semantics::SignalType::meta_name );
						// Add signal set size to inputs
						inputs += this->SignalSize( signalTypeStr );
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
					SignalSet signalSet = msg.carries();
					// Go through each signal
					SignalSet::iterator signalIter = signalSet.begin();
					for ( ; signalIter != signalSet.end(); signalIter++ ) {
						// Get the signal type
						Semantics::SignalType signalType = signalIter->isOfType();
						std::string signalTypeStr = signalType.getStringAttr( Semantics::SignalType::meta_mtypestr );
						// Add signal set size to inputs
						outputs += this->SignalSize( signalTypeStr );
					}
				}	// Actuation
			}	// OChan
		}	// OutputDevice
	}	// Devices
}
*/


/***************************************************************************************/


Frodo2Visitor::Frodo2Visitor( Semantics::RootFolder &rootFolder, TimeStamp &timeStamp ) :
	AbstractVisitor(rootFolder, timeStamp) {
}


void Frodo2Visitor::Visit_RootFolder( const Semantics::RootFolder &rootFolder ) {

	/*** Pass 1 - Generate all of the _init and _code files ***/

	std::cout << "-- Starting Second Pass:\n";
	// Clear the dictionary
	this->ClearDictionary();
	// Get the set of deployment children
	DeploymentVector deploymentVector = rootFolder.Deployment_children();
	// Visit each deployment to start off
	DeploymentVector::iterator deploymentIter = deploymentVector.begin();
	for ( ; deploymentIter != deploymentVector.end(); deploymentIter++) {
		// Visit the deployment again, this time with pass == true
		deploymentIter->Accept( *this );
	}
}


void Frodo2Visitor::Visit_Deployment( const Semantics::Deployment &deployment ) {
	// Get set of TTBUs children - create TT Networks
	TTBusVector ttbusVector = deployment.TTBus_kind_children();
	// Visit them all
	TTBusVector::iterator ttbusIter = ttbusVector.begin();
	for ( ; ttbusIter != ttbusVector.end(); ttbusIter++ ) {
		// Visit the TTBus
		ttbusIter->Accept( *this );
	}

	CANBusVector cbusSet = deployment.CANBus_kind_children();
	CANBusVector::iterator cbusIter = cbusSet.begin();
	for ( ; cbusIter != cbusSet.end(); cbusIter++ ) {

		cbusIter->Accept( *this );
	}

	// Get set of Node children - need to create _main files
	NodeVector nodeVector = deployment.Node_children();
	// Visit them all
	NodeVector::iterator nodeIter = nodeVector.begin();
	for ( ; nodeIter != nodeVector.end(); nodeIter++ ) {
		// Visit the node to gen _main files
		nodeIter->Accept( *this );
	}
}


void Frodo2Visitor::Visit_Node( const Semantics::Node &node ) {
	// Setup the node name
	std::string kernelName = node.name();
	std::string kernelInitName = kernelName + "_init";
	DEBUGOUT( "\tNode: " << kernelName << std::endl );

	// Create the file name
	std::string filename = std::string( node.name() ) + "_main";
	// Set some dictionary items for this node
	this->GetMainDictionary().SetValue( "FILENAME", filename );
	this->GetMainDictionary().SetValue( "KERNEL_SCHEDULE", "prioFP" );
	this->GetMainDictionary().SetFormattedValue( "KERNEL_HYPERPERIOD", "%f", (double)node.hyperperiodsecs() );

	// Visit all tasks assigned to this node
	TaskSet taskSet = node.executes();
	TaskVector taskVector( taskSet.begin(), taskSet.end() );
	TaskVector::iterator taskIter = taskVector.begin();
	for ( ; taskIter != taskVector.end(); taskIter++ ){
		// Visit the task
		taskIter->Accept( *this );
	}

	// Clear the analog in/out counters
//	this->_analogIn = 1;
//	this->_analogOut = 1;
	// Visit all devices contained in this node
	DeviceSet deviceSet = node.integrates();
	DeviceVector deviceVector( deviceSet.begin(), deviceSet.end() );
	DeviceVector::iterator deviceIter = deviceVector.begin();
	for ( ; deviceIter != deviceVector.end(); deviceIter++ ) {
		// Visit the device
		deviceIter->Accept( *this );
	}

	// Visit all local dependencies on this node
	LocalDependencySet dependencySet = node.nodeLocalDeps();
	LocalDependencyVector dependencyVector( dependencySet.begin(), dependencySet.end() );
	LocalDependencyVector::iterator dependencyIter = dependencyVector.begin();
	for ( ; dependencyIter != dependencyVector.end(); dependencyIter++ ) {
		// Visit the dependency
		dependencyIter->Accept( *this );
	}

	// Get the template file name
	std::string templateName = ConfigKeeper::inst().GetTemplatePath() + "\\FRODOv2.tpl";
	// Initialize the template 
	ctemplate::Template* googleTemplate = ctemplate::Template::GetTemplate( templateName, ctemplate::DO_NOT_STRIP );
	std::string output;
	// Expand the output into a string
	googleTemplate->Expand( &output, &this->GetMainDictionary() );
		
	// Create the _init file for this TT kernel
	std::string directoryName = ConfigKeeper::inst().GetDirectoryName();
	filename = directoryName + "\\" + filename + ".cpp";
	ofstream initFile( filename.c_str() );
	// Write the generated code out to the file and close
	initFile << output;
	// Close up the file
	initFile.close();
	// Clear out the dictionary
	this->ClearDictionary();
}


void Frodo2Visitor::Visit_LocalDependency( const Semantics::LocalDependency &dependency ) {
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
	this->Visit_QualifiedMessageStructure( qualifiedMessage, task );
}


void Frodo2Visitor::Visit_Task( const Semantics::Task &task ) {
	// Get the task name
	std::string taskName = task.name();
	DEBUGOUT( "\tTask: " << taskName << std::endl );

	// Process messages for this task
	QualifiedMessageSet messageSet = this->GetQualifiedMessages( task );
	QualifiedMessageSet::iterator messageIter = messageSet.begin();
	for ( ; messageIter != messageSet.end() ; messageIter++ ) {
		// Visit the message to generate template section
		this->Visit_QualifiedMessageStructure( *messageIter, task );
	}

	// Get the exec info and schedule
	ExecInfoSet execInfoSet = task.info();
	ExecInfoSet::iterator execInfoIter = execInfoSet.begin();
	Semantics::Duration duration = execInfoIter->Duration_child();
	double wcet = duration.exectimesecs();
	// Add a section for the task execution
	this->AddSectionDictionary( "TASK_SECTION" );
	this->GetTemplateDictionary().SetFormattedValue( "TASK_WCET", "%f", wcet );
	// Setup the peripheral schedule
	Semantics::Schedule schedule = execInfoIter->Schedule_child();
	std::string sched = schedule.sched();
	this->GetTemplateDictionary().SetIntValue( "TASK_COUNT", sched_count( sched ) );
	this->GetTemplateDictionary().SetValue( "TASK_SCHEDULE", sched );

	// Get the set of components this task invokes
	ComponentSet componentSet = task.invokes();
	// If there are components...
	if ( !componentSet.empty() ) {
		// Start with the first component
		Semantics::Component component = *componentSet.begin();
		// Get the component name
		std::string compName = component.name();
		this->GetTemplateDictionary().SetValue( "TASK_NAME", compName + "_task" );
		this->GetTemplateDictionary().SetValue( "TASK_FUNCTION", compName + "_code" );
		// Create the list of component header files
		StringList headerList = Split( component.hfiles() );
		StringList::iterator headerIter = headerList.begin();
		// Iterate through the list
		for( ; headerIter != headerList.end() ; headerIter++ ) {
			// Include a header section and populate it
			this->AddSectionDictionary( "TASK_INCLUDE" );
			this->GetTemplateDictionary().SetValue( "TASK_HEADER", *headerIter );
			this->PopSectionDictionary();
		}
		// Set the component function name
		this->GetTemplateDictionary().SetValue( "TASK_COMPNAME", compName );
		std::string compMSubsys = component.msubsystem();
		// Account for 
		this->GetTemplateDictionary().SetValue( "TASK_MSUBSYS", compMSubsys );
		// Setup the input signals to the component
		SortedSignal_ByCallIndex_Set inputSignalSet = component.consumes_sorted( SignalCallIndexSorter() );
		SortedSignal_ByCallIndex_Set::iterator inputIter = inputSignalSet.begin() ;
		for( ; inputIter != inputSignalSet.end(); inputIter++ ) {
			this->AddSectionDictionary( "INPUT_SIGNALS" );
			this->Visit_Signal( *inputIter );
			this->PopSectionDictionary();
		}
		// Setup the output signals to the component
		SortedSignal_ByCallIndex_Set outputSignalSet = component.generates_sorted( SignalCallIndexSorter() );
		SortedSignal_ByCallIndex_Set::iterator outputIter = outputSignalSet.begin();
		for ( ; outputIter != outputSignalSet.end(); outputIter++ ) {
			this->AddSectionDictionary( "OUTPUT_SIGNALS" );
			this->Visit_Signal( *outputIter );
			this->PopSectionDictionary();
		}
	}
	// Done with section dictionary
	this->PopSectionDictionary();
}


void Frodo2Visitor::Visit_Device( const Semantics::Device &device ) {
	// See if device is input
	if ( Semantics::InputDevice::meta == device.type() ) {
		// Cast to an InputDevice
		Semantics::InputDevice input = Semantics::InputDevice::Cast( device );
		// Get the associated IChans
		IChanSet iChanSet = input.inputChannels();
		IChanVector iChanVector( iChanSet.begin(), iChanSet.end() );
		IChanVector::iterator iChanIter = iChanVector.begin();
		// Visit the IChans
		for ( ; iChanIter != iChanVector.end(); iChanIter++ ) {
			iChanIter->Accept( *this );
		}
	}
	// See if device is output
	else if ( Semantics::OutputDevice::meta == device.type() ) {
		// Cast to an InputDevice
		Semantics::OutputDevice output = Semantics::OutputDevice::Cast( device );
		// Get the associated OChans
		OChanSet oChanSet = output.outputChannels();
		OChanVector oChanVector( oChanSet.begin(), oChanSet.end() );
		OChanVector::iterator oChanIter = oChanVector.begin();
		// Visit the OChans
		for ( ; oChanIter != oChanVector.end(); oChanIter++ ) {
			oChanIter->Accept( *this );
		}
	}
	// See if device is CommInterface
	else if ( Semantics::CommInterface::meta == device.type() ) {
		// Nothing for now...
	}
}


void Frodo2Visitor::Visit_IChan( const Semantics::IChan &iChan ) {
	// Get the IChan name
	std::string iChanName = iChan.name();
	DEBUGOUT( "\tIChan: " << iChanName << std::endl );
	// Include a peripheral section and populate it
	this->AddSectionDictionary( "PERIPHERAL_SECTION" );
	this->GetTemplateDictionary().SetValue( "PERIPHERAL_NAME", iChanName + "_task" );
	this->GetTemplateDictionary().SetValue( "PERIPHERAL_FUNCTION", iChanName + "_code" );

	// Get all signals in message
	Semantics::Acquisition acquisition = iChan.channelAcquisition();
	// Make sure there is an acquisition
	if ( acquisition != Udm::null ) {
		// Setup the data input code
		this->AddSectionDictionary( "PERIPHERAL_INPUT" );
		Semantics::Msg msg = acquisition.acquiredMsg();
		SignalSet signalSet = msg.carries();
		SignalSet::iterator signalIter = signalSet.begin();
		// Loop through all signals
		for ( ; signalIter != signalSet.end(); signalIter++ ) {
			// Get the signal type info
			Semantics::SignalType type = signalIter->isOfType();
			std::string typeName = type.getStringAttr( Semantics::SignalType::meta_name );
			int numSignals = this->SignalSize( typeName );
			// Loop through each signal
			for ( int i=0; i < numSignals; i++ ) {
				// Get the style of this signal
				std::string signalName = this->SignalName( typeName, i );
				// Add a PERIPHERAL_MESSAGE section
				this->AddSectionDictionary( "PERIPHERAL_MESSAGE" );
				// Set the template values for the section
//				this->GetTemplateDictionary().SetIntValue( "PERIPHERAL_CHANNEL", this->_analogIn++ );
				std::string messageName = msg.name();
				messageName.replace( messageName.find(".") , 1, "_" );
				std::string fullName = messageName + "->" + std::string( signalIter->name() ) + signalName;
				this->GetTemplateDictionary().SetValue( "PERIPHERAL_MSGNAME", fullName );
				this->PopSectionDictionary();
			}
		}
		// Close the peripheral input section
		this->PopSectionDictionary();
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
		this->GetTemplateDictionary().SetFormattedValue( "PERIPHERAL_WCET", "%f", wcet );
		Semantics::Schedule schedule = execInfoIter->Schedule_child();
		std::string sched = schedule.sched();
		this->GetTemplateDictionary().SetIntValue( "PERIPHERAL_COUNT", sched_count( sched ) );
		this->GetTemplateDictionary().SetValue( "PERIPHERAL_SCHEDULE", sched );
	}
	// There are problems...
	else {
		DEBUGOUT( "*** Error: This IChan has no schedule - Check your model.\n" );
		this->GetTemplateDictionary().SetFormattedValue( "PERIPHERAL_WCET", "%f", 0.01 );
		this->GetTemplateDictionary().SetIntValue( "PERIPHERAL_COUNT", 1 );
		this->GetTemplateDictionary().SetValue( "PERIPHERAL_SCHEDULE", "-1.0" );
	}

	// Close the peripheral section
	this->PopSectionDictionary();
}


void Frodo2Visitor::Visit_OChan( const Semantics::OChan &oChan) {
	// Get the OChan name
	std::string oChanName = oChan.name();
	DEBUGOUT( "\tOChan: " << oChanName << std::endl );
	// Include a peripheral section and populate it
	this->AddSectionDictionary( "PERIPHERAL_SECTION" );
	this->GetTemplateDictionary().SetValue( "PERIPHERAL_NAME", oChanName + "_task" );
	this->GetTemplateDictionary().SetValue( "PERIPHERAL_FUNCTION", oChanName + "_code" );

	// Get all signals in message
	ActuationSet actuatorSet = oChan.channelActuations();
	// Make sure there are signals
	if ( actuatorSet.size() != 0 ) {
		// Setup the data input code
		this->AddSectionDictionary( "PERIPHERAL_OUTPUT" );
		Semantics::Msg msg = actuatorSet.begin()->actuatingMsg();
		SignalSet signalSet = msg.carries();
		SignalSet::iterator signalIter = signalSet.begin();
		// Loop through all signals
		for ( ; signalIter != signalSet.end(); signalIter++ ) {
			// Get the signal type info
			Semantics::SignalType type = signalIter->isOfType();
			std::string typeName = type.getStringAttr( Semantics::SignalType::meta_name );
			int numSignals = this->SignalSize( typeName );
			// Loop through each signal
			for ( int i=0; i < numSignals; i++ ) {
				// Get the style of this signal
				std::string signalName = this->SignalName( typeName, i );
				// Add a PERIPHERAL_MESSAGE section
				this->AddSectionDictionary( "PERIPHERAL_MESSAGE" );
				// Set the template values for the section
//				this->GetTemplateDictionary().SetIntValue( "PERIPHERAL_CHANNEL", this->_analogOut++ );
				std::string messageName = msg.name();
				messageName.replace( messageName.find(".") , 1, "_" );
				std::string fullName = messageName + "->" + std::string( signalIter->name() ) + signalName;
				this->GetTemplateDictionary().SetValue( "PERIPHERAL_MSGNAME", fullName );
				this->PopSectionDictionary();
			}
		}
		// Close the peripheral input section
		this->PopSectionDictionary();
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
		Semantics::Schedule schedule = execInfoIter->Schedule_child();
		std::string sched = schedule.sched();
		this->GetTemplateDictionary().SetFormattedValue( "PERIPHERAL_WCET", "%f", wcet );
		this->GetTemplateDictionary().SetIntValue( "PERIPHERAL_COUNT", sched_count( sched ) );
		this->GetTemplateDictionary().SetValue( "PERIPHERAL_SCHEDULE", sched );
	}
	// There are problems...
	else {
		DEBUGOUT( "*** Error: This OChan has no schedule - Check your model.\n" );
		this->GetTemplateDictionary().SetFormattedValue( "PERIPHERAL_WCET", "%f", 0.01 );
		this->GetTemplateDictionary().SetIntValue( "PERIPHERAL_COUNT", 1 );
		this->GetTemplateDictionary().SetValue( "PERIPHERAL_SCHEDULE", "-1.0" );
	}
	// We are done here
	this->PopSectionDictionary();
}


void Frodo2Visitor::Visit_TTBus( const Semantics::TTBus &bus ) {
	// Get the bus/network name
	std::string networkName = bus.name();
	double datarate = bus.dataratebps();
	int id = this->_networkMap.size() + 1;
	DEBUGOUT( "\tTTBus(" << id << "): " << networkName << std::endl );
	// Insert network into NetworkMap
	TTNetwork network = { id, new std::map< std::string, int >() };
	this->_networkMap.insert( std::make_pair( networkName, network ) );
	// Add a TTKernel for the node
	this->AddSectionDictionary( "TTNETWORK_SECTION" );
	this->GetTemplateDictionary().SetValue( "NETWORK_NAME", networkName );
	this->GetTemplateDictionary().SetValue( "NETWORK_TYPE", "TDMA" );
	this->GetTemplateDictionary().SetIntValue( "NETWORK_NUMBER", id );
	this->GetTemplateDictionary().SetFormattedValue( "NETWORK_DATARATE", "%4.0f", datarate );
	this->GetTemplateDictionary().SetValue( "NETWORK_FRAMESIZE", "512");
	this->GetTemplateDictionary().SetValue( "NETWORK_LOSSPROB", "0");
	this->GetTemplateDictionary().SetValue( "NETWORK_SEED", "0");
	// Determine the number of nodes attached to the bus
	int numNodes = 0;
	// First get all of the comm interfaces attached to the bus
	IfaceSet commInterfaceSet = bus.interfaces();
	IfaceSet::iterator commInterfaceIter = commInterfaceSet.begin();
	// Loop through the comm interfaces
	for ( ; commInterfaceIter != commInterfaceSet.end(); commInterfaceIter++ ) {
		// Get the bChan associated with each comm interface
		BChanSet bChanSet = commInterfaceIter->commChannels();
		// Add the number of bChans to the num nodes
		numNodes += bChanSet.size();
	}
	// Now build the schedule string
	std::string netSchedule = "[ ";
	for ( int i=0; i < numNodes; i++ ) {
		// Make sure to add 1 to i since TT network nodes are 1-indexed
		netSchedule = netSchedule + boost::lexical_cast<std::string>(i+1) + std::string(" ");
	}
	netSchedule += "]";
	this->GetTemplateDictionary().SetValue( "NETWORK_SCHEDULE", netSchedule );
	this->GetTemplateDictionary().SetIntValue( "NETWORK_NUMNODES", numNodes );
	// Done with section dictionary
	this->PopSectionDictionary();
}

void Frodo2Visitor::Visit_CANBus( const Semantics::CANBus & bus ) {
	// Get the bus/network name
	std::string networkName = bus.name();
	double datarate = bus.dataratebps();
	int id = this->_networkMap.size() + 1;
	DEBUGOUT( "\tCANBus(" << id << "): " << networkName << std::endl );
	// Insert network into NetworkMap
	TTNetwork network = { id, new std::map< std::string, int >() };
	this->_networkMap.insert( std::make_pair( networkName, network ) );
	// Add a TTKernel for the node
	// TODO: Change this to create a CAN network
	//this->AddSectionDictionary( "TTNETWORK_SECTION" );
	//this->GetTemplateDictionary().SetValue( "NETWORK_NAME", networkName );
	//this->GetTemplateDictionary().SetValue( "NETWORK_TYPE", "TDMA" );
	//this->GetTemplateDictionary().SetIntValue( "NETWORK_NUMBER", id );
	//this->GetTemplateDictionary().SetFormattedValue( "NETWORK_DATARATE", "%4.0f", datarate );
	//this->GetTemplateDictionary().SetValue( "NETWORK_FRAMESIZE", "512");
	//this->GetTemplateDictionary().SetValue( "NETWORK_LOSSPROB", "0");
	//this->GetTemplateDictionary().SetValue( "NETWORK_SEED", "0");
	// Determine the number of nodes attached to the bus
	int numNodes = 0;
	// First get all of the comm interfaces attached to the bus
	IfaceSet commInterfaceSet = bus.interfaces();
	IfaceSet::iterator commInterfaceIter = commInterfaceSet.begin();
	// Loop through the comm interfaces
	for ( ; commInterfaceIter != commInterfaceSet.end(); commInterfaceIter++ ) {
		// Get the bChan associated with each comm interface
		BChanSet bChanSet = commInterfaceIter->commChannels();
		// Add the number of bChans to the num nodes
		numNodes += bChanSet.size();
	}
	// Now build the schedule string
	std::string netSchedule = "[ ";
	for ( int i=0; i < numNodes; i++ ) {
		// Make sure to add 1 to i since TT network nodes are 1-indexed
		netSchedule = netSchedule + boost::lexical_cast<std::string>(i+1) + std::string(" ");
	}
	netSchedule += "]";

	// TODO: Uncomment this when the template is fixed for CAN bus
	//this->GetTemplateDictionary().SetValue( "NETWORK_SCHEDULE", netSchedule );
	//this->GetTemplateDictionary().SetIntValue( "NETWORK_NUMNODES", numNodes );
	// Done with section dictionary
	//this->PopSectionDictionary();
}


void Frodo2Visitor::Visit_Plant( const Semantics::Plant &plant ) {
	// Get the plant name
	std::string plantName = plant.name();
	DEBUGOUT( "\tPlant: " << plantName << std::endl );
	// Add a Subsystem for the plant
	// ...

	// Wire the plant to the nodes
	// ...
}


void Frodo2Visitor::Visit_Signal( const Semantics::Signal &signal, bool isParameter ) {
	MsgSet msgSet = signal.carriedBy();
	MsgVector msgVector( msgSet.begin(), msgSet.end() );
	TTMsgSet ttMsgSet;
	// Cast all of the signals to...
	for( MsgVector::iterator mgsIter = msgVector.begin() ; mgsIter != msgVector.end() ; (void)++mgsIter ) {
		ttMsgSet.insert( Semantics::Msg::Cast( *mgsIter ) );
	}
	Semantics::Msg ttMsg = *ttMsgSet.begin();
	// Hack to remove the '.' in the message name
	std::string messageName = ttMsg.name();
	messageName.replace( messageName.find("."), 1, "_" );
	this->GetTemplateDictionary().SetValue( "SIGNAL_MSGNAME", messageName );

	std::string signalName = signal.name();
	if ( isParameter ) {
		std::string::size_type pos = signalName.rfind( "__" );
		if ( pos != std::string::npos ) signalName = signalName.substr( pos + 2 );
	}
	Semantics::SignalType signalType = signal.isOfType();
	if ( signalType != Udm::null ) {
		this->GetTemplateDictionary().SetValue(  "SIGNAL_TYPE", std::string( signalType.ctypestr() )  );
	}
	this->GetTemplateDictionary().SetValue( "SIGNAL_NAME", signalName );
}


void Frodo2Visitor::Visit_QualifiedMessageStructure( const QualifiedMessage &qualifiedMessage, const Semantics::Task &task ) {
	// Get the actual message object
	Semantics::Msg message = qualifiedMessage.message;
	// Create a new section for it
	this->AddSectionDictionary( "MESSAGE_SECTION" );
	// Get the message name
	std::string messageName = message.name();
	// Figure out non-dotted name
	messageName.replace( messageName.find("."), 1, "_" );
	DEBUGOUT( "\tMessage: " << messageName << std::endl );

	std::string messageType = message.messagetype();
	this->GetTemplateDictionary().SetValue( "MESSAGE_NAME", messageName );
	// Get the associated signals
	SignalSet signalSet = message.carries();
	SignalSet::iterator signalIter = signalSet.begin();

	// Loop through all signals
	for( ; signalIter != signalSet.end() ; signalIter++ ) {
		std::string signalName = signalIter->name();
		ComponentSet receivingComponentSet = signalIter->consumedBy();
		Semantics::Component transmittingComponent = signalIter->generatedBy();
		// Check to make sure for something?!?!?!?!?
		if ( qualifiedMessage.type == TRANSMIT && receivingComponentSet.empty() ||
			 qualifiedMessage.type == RECEIVE && transmittingComponent == Udm::null ||
			 qualifiedMessage.type == PERIPHERAL ) {
			// Insert the signal into the set
			this->Visit_Signal_Member( *signalIter );
		}
	}
	// Finish up with this section
	this->PopSectionDictionary();

	// See if message is remote
	if ( messageType.compare( "Remote" ) == 0 ) {
		this->AddSectionDictionary( "BUSMESSAGE_SECTION" );
		this->GetTemplateDictionary().SetValue( "BUSMESSAGE_NAME", messageName + "_task" );
		this->GetTemplateDictionary().SetValue( "BUSMESSAGE_FULLNAME", messageName );
		this->GetTemplateDictionary().SetValue( "BUSMESSAGE_FUNCTION", messageName + "_code" );
		// Get the exec info and schedule
		ExecInfoSet execInfoSet = message.info();
		ExecInfoSet::iterator execInfoIter = execInfoSet.begin();
		Semantics::Duration duration = execInfoIter->Duration_child();
		double wcet = duration.exectimesecs();
		this->GetTemplateDictionary().SetFormattedValue( "BUSMESSAGE_WCET", "%f", wcet );
		// Setup the peripheral schedule
		Semantics::Schedule schedule = execInfoIter->Schedule_child();
		std::string sched = schedule.sched();
		this->GetTemplateDictionary().SetIntValue( "BUSMESSAGE_COUNT", sched_count( sched ) );
		this->GetTemplateDictionary().SetValue( "BUSMESSAGE_SCHEDULE", sched );

		// Get the network that transmits the message
		ReceivesSet receiveSet = message.msgListeners();
		// TODO: Need to figure out how to disambiguate receivers for this message
		Semantics::BChan bChan = receiveSet.begin()->receivingChannel();
		Semantics::CommInterface commInterface = bChan.chanInterface();
		Semantics::CommMedium network = commInterface.commMedium();
		// Find the network in the map
		std::map< std::string, TTNetwork >::iterator mapIter = this->_networkMap.find( network.name() );
		// Setup the message handling code
		Semantics::Transmits transmitter = message.msgTransmitter();
		Semantics::Task sendingTask = transmitter.sendingTask();
		// See if this task is being sent or received
		if ( sendingTask == task ) {
			// Add the appropriate section
			this->AddSectionDictionary( "BUSMESSAGE_SEND" );
			// Loop on all receivers
			ReceivesSet::iterator receivesIter = receiveSet.begin();
			for ( ; receivesIter != receiveSet.end(); receivesIter++ ) {
				this->AddSectionDictionary( "BUSMESSAGE_SENDBLOCK" );
				// Which node is receiving
				Semantics::Task task = receivesIter->receivingTask();
				Semantics::Node node = task.executedBy();
				// Find the node in the mapIter
				std::map< std::string, int >::iterator nodeIter = mapIter->second.nodes->find( node.name() );
				// Set the network
//				this->GetTemplateDictionary().SetIntValue( "BUSMESSAGE_NETWORK", mapIter->second.id );
				// Set the receiver
//				this->GetTemplateDictionary().SetIntValue( "BUSMESSAGE_RECEIVER", nodeIter->second );
				// Done with this send block
				this->PopSectionDictionary();
			}
			// Done with the BUSMESSAGE_SEND
			this->PopSectionDictionary();
		}
		// Must be receiving
		else {
			// Add the appropriate section
			this->AddSectionDictionary( "BUSMESSAGE_RECEIVE" );
			// Set the network ID
			this->GetTemplateDictionary().SetIntValue( "BUSMESSAGE_NETWORK", mapIter->second.id );
			// Done with this section
			this->PopSectionDictionary();
		}
		this->PopSectionDictionary();
	}
}


void Frodo2Visitor::Visit_Signal_Member( const Semantics::Signal &signal ) {
	std::string signalName = signal.name();
	DEBUGOUT( "\t\tSignal Member: " << signalName << std::endl );
	// Get the signal type
	Semantics::SignalType signalType = signal.isOfType();
	// Create a new section for the signal member
	this->AddSectionDictionary( "SIGNAL_MEMBER" );
	std::string signalTypeName = signalType.ctypestr();
	// Make sure the signalTypeName is valid
	if ( signalTypeName.empty() )
		signalTypeName = signalType.mtypestr();
	// Populate the local dictionary
	this->GetTemplateDictionary().SetValue( "SIGNAL_TYPENAME", signalTypeName );
	this->GetTemplateDictionary().SetValue( "SIGNAL_NAME", signalName );
	// We are done with this...
	this->PopSectionDictionary();
}


/***************************************************************************************/
