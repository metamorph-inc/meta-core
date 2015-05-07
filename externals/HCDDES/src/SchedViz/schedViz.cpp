/*** Warning Workaround ***/
#pragma warning( disable: 4251 )


/*** Included Header Files ***/
#include "schedViz.h"
#include "traceViz.h"
#include <boost\lexical_cast.hpp>


// Just a quick debug macro to ease the pain
#define DBGOUT(msg) {if(this->_debug) std::cout << msg;}


/***************************************************************************************/

std::string GetTemplatePath(void) {
	std::string templatePath;
	// Is the VCP_PATH environment variable set?
	if ( getenv( "VCP_PATH" ) ) {
		// If so, just use it
		templatePath = std::string( getenv( "VCP_PATH" ) ) + std::string( "\\templates" );;
	}
	// Otherwise, just use a guess using a relative path
	else {
		templatePath = std::string( "..\\..\\templates" );
	}
	// Return the path
	return templatePath;
}


std::vector< double > ConvertToVector( const std::string schedule ) {
	std::vector< double > retVal;
	std::string::size_type start = 0;
	std::string::size_type end;
	// Loop until end-of-line
	do {
		// Find the first comma after start
		end = schedule.find( ",", start );
		// Get the substring from start to end
		std::string subStr = schedule.substr( start, end-start );
		// Convert the subStr to a double and insert into vector
		double value = boost::lexical_cast< double >( subStr );
		retVal.push_back( value );
		// Adjust start and end
		if ( end != string::npos ) end++;
		start = end;
	}
	while ( end != string::npos );
	// Return the vector
	return retVal;
}


MessageSet GetMessageSet( const Semantics::CommMedium & bus ) {
	MessageSet outputSet;
	// Get all commInterfaces for the bus
	CommInterfaceSet commSet = bus.interfaces();
	// Iterate through all commInterfaces
	CommInterfaceSet::iterator commIter = commSet.begin();
	for ( ; commIter != commSet.end(); commIter++ ) {
		// Get the BChannels for this comm interface
		BChanSet bChanSet = commIter->commChannels();
		// Iterate through all bchans
		BChanSet::iterator bChanIter = bChanSet.begin();
		for ( ; bChanIter != bChanSet.end(); bChanIter++ ) {
			// Get the transmits set from the bChan
			TransmitsSet transmitsSet = bChanIter->channelTransmitters();
			// Iterate through all the transmits
			TransmitsSet::iterator transmitsIter = transmitsSet.begin();
			for ( ; transmitsIter != transmitsSet.end(); transmitsIter++ ) {
				// Get the message
				Semantics::Msg message = transmitsIter->transmittedMsg();
				// Add the message to the output set
				outputSet.insert( message );
			}
		}
	}
	// Return the message set
	return outputSet;
}


/***************************************************************************************/


SchedViz::SchedViz( const std::string &abstractModel, const std::string &vizFilename, const bool &debug ) :
	_debug(debug), _hyperperiodSec(0.0), _traceViz(NULL) {
	// Start UDM using the ESMoL_Abstract (Semantics) paradigm
	Udm::SmartDataNetwork abstractSDN( Semantics::diagram );
	try {
		// Try to open the model file
		abstractSDN.OpenExisting( abstractModel );
	} catch (udm_exception &ex) {
		std::cout << "Exception occured trying to open " << abstractModel << " in udm:" << std::endl;
		std::cout << ex.what() << std::endl;
		throw SchedVizException( std::string( ex.what() ) );
	}

	// Get the template file name
	std::string templateName = GetTemplatePath() + "\\sched_viz.tpl";
	// Start up the TraceViz
	this->_traceViz = new TraceViz( abstractModel, vizFilename, templateName );

	//Get the root folder of the project
	Semantics::RootFolder rootFolder = Semantics::RootFolder::Cast( abstractSDN.GetRootObject() );
	// Visit the root folder
	this->Visit_RootFolder( rootFolder );

	// Render the trace to file
	this->_traceViz->Render();
	// Delete the traceViz
	delete this->_traceViz;
}


/***************************************************************************************/


void SchedViz::Visit_RootFolder( const Semantics::RootFolder &rootFolder ) {
	// Get the set of children Deployments
	DeploymentSet deploymentSet = rootFolder.Deployment_children();
	DeploymentSet::iterator deploymentIter = deploymentSet.begin();
	// For each deployment ...
	for ( ; deploymentIter != deploymentSet.end(); deploymentIter++ ) {
		// Visit it
		this->Visit_Deployment( *deploymentIter );
	}
}


void SchedViz::Visit_Deployment( const Semantics::Deployment &deployment ) {
	// Determine the hyperperiod
	// TODO: quick fix -- this needs to be changed if we have multiple hyperperiods
	BusSet busSet = deployment.CommMedium_children();
	BusSet::iterator busIter = busSet.begin();
	this->_hyperperiodSec = busIter->hyperperiodsecs();

	// Get the set of children Nodes
	NodeSet nodeSet = deployment.Node_children();
	NodeSet::iterator nodeIter = nodeSet.begin();
	// For each node ...
	for ( ; nodeIter != nodeSet.end(); nodeIter++ ) {
		// Visit the node
		this->Visit_Node( *nodeIter );
	}
	// For each bus ...
	for ( ; busIter != busSet.end(); busIter++ ) {
		// Visit each bus
		this->Visit_Bus( *busIter );
	}
}


void SchedViz::Visit_Node( const Semantics::Node &node ) {
	std::string nodeName = node.name();
	DBGOUT("Node: " << nodeName << std::endl)

	// Add a superblock to the viz
	TVSuperblock* superblock = this->_traceViz->AddSuperblock( nodeName, this->_hyperperiodSec );

	// Now iterate through all of the task children
	TaskSet taskSet = node.executes();
	TaskSet::iterator taskIter = taskSet.begin();
	for ( ; taskIter != taskSet.end(); taskIter++ ) {
		// Visit the task
		this->Visit_Task( *taskIter, superblock );
	}

	// Now interate through all of the device children
	DeviceSet deviceSet = node.integrates();
	DeviceSet::iterator deviceIter = deviceSet.begin();
	for ( ; deviceIter != deviceSet.end(); deviceIter++ ) {
		// Visit the device
		this->Visit_Device( *deviceIter, superblock );
	}
}


void SchedViz::Visit_Task( const Semantics::Task &task, TVSuperblock *superblock ) {
	std::string taskName = task.name();
	DBGOUT("\tTask: " << taskName << " -> ")

	// Need to create a Task trace
	TVTrace* trace = superblock->AddTrace( taskName );
	// Create a new schedulable in the traceViz
	TVSchedulable* sched = new TVSchedulable( Task, taskName, false );

	// Loop through all of the scheduling info
	ExecInfoSet execSet = task.info();
	// Make sure there is one ( TTSchedule )
	if (execSet.size() == 1 ) {
		Semantics::ExecInfo execInfo = *execSet.begin();
		// Get the children of various types
		Semantics::Schedule schedChild = execInfo.Schedule_child();
		Semantics::Duration durationChild = execInfo.Duration_child();
		double duration = durationChild.exectimesecs();
		// See if child is a TTSchedule
		if ( Semantics::TTSchedule::meta == schedChild.type() ) {
			// Cast to a TTSchedule
			Semantics::TTSchedule ttSched = Semantics::TTSchedule::Cast( schedChild );
			// Grab the schedule info
			std::string schedule = ttSched.sched();
			// Convert the string to a nice vector
			std::vector< double > schedTimes = ConvertToVector( schedule );
			// Now create visual blocks for each invocation
			std::vector< double >::iterator timesIter = schedTimes.begin();
			for ( ; timesIter != schedTimes.end(); timesIter++ ) {
				// Add the task as a trace to the traceViz
				DBGOUT(*timesIter << " ")
				// Add an instance to the traceViz
				sched->AddInstance( trace, *timesIter, duration );
			}
		}
		// ExecInfo type not supported
		else {
			std::cout << "*** Task: ExecInfo type not currently supported.\n";
		}
	}
	// Debug some info
	else {
		std::cout << "*** Task: Not able to find appropriate ExecInfo.\n";
	}
	// Clean up the output
	DBGOUT(std::endl)
}


void SchedViz::Visit_Device( const Semantics::Device &device, TVSuperblock *superblock ) {
	// See if device is input
	if ( Semantics::InputDevice::meta == device.type() ) {
		// Cast to an InputDevice
		Semantics::InputDevice input = Semantics::InputDevice::Cast( device );
		// Get the associated IChans
		IChanSet iChanSet = input.inputChannels();
		IChanSet::iterator iChanIter = iChanSet.begin();
		// Visit the IChans
		for ( ; iChanIter != iChanSet.end(); iChanIter++ ) {
			this->Visit_IChan( *iChanIter, superblock );
		}
	}
	// See if device is output
	else if ( Semantics::OutputDevice::meta == device.type() ) {
		// Cast to an InputDevice
		Semantics::OutputDevice output = Semantics::OutputDevice::Cast( device );
		// Get the associated OChans
		OChanSet oChanSet = output.outputChannels();
		OChanSet::iterator oChanIter = oChanSet.begin();
		// Visit the OChans
		for ( ; oChanIter != oChanSet.end(); oChanIter++ ) {
			this->Visit_OChan( *oChanIter, superblock );
		}
	}
	// See if device is CommInterface
	else if ( Semantics::CommInterface::meta == device.type() ) {
		// Nothing for now...
	}
}


void SchedViz::Visit_IChan( const Semantics::IChan &iChan, TVSuperblock *superblock ) {
	std::string iChanName = iChan.name();
	DBGOUT("\tIChan: " << iChanName << " -> ")
	// Need to create a IChan trace
	TVTrace* trace = superblock->AddTrace( iChanName );
	// Create a new schedulable in the traceViz
	TVSchedulable* sched = new TVSchedulable( Task, iChanName, false );

	// Setup the peripheral schedule
	ExecInfoSet execInfoSet = iChan.info();
	ExecInfoSet::iterator execInfoIter = execInfoSet.begin();
	// Make sure there is one ( TTSchedule )
	if (execInfoSet.size() == 1 ) {
		Semantics::Schedule schedChild = execInfoIter->Schedule_child();
		Semantics::Duration durationChild = execInfoIter->Duration_child();
		double duration = durationChild.exectimesecs();
		// See if child is a TTSchedule
		if ( Semantics::TTSchedule::meta == schedChild.type() ) {
			// Cast to a TTSchedule
			Semantics::TTSchedule ttSched = Semantics::TTSchedule::Cast( schedChild );
			// Grab the schedule info
			std::string schedule = ttSched.sched();
			// Convert the string to a nice vector
			std::vector< double > schedTimes = ConvertToVector( schedule );
			// Now create visual blocks for each invocation
			std::vector< double >::iterator timesIter = schedTimes.begin();
			for ( ; timesIter != schedTimes.end(); timesIter++ ) {
				// Add the task as a trace to the traceViz
				DBGOUT(*timesIter << " ")
				// Add an instance to the traceViz
				sched->AddInstance( trace, *timesIter, duration );
			}
		}
		// ExecInfo type not supported
		else {
			std::cout << "*** Task: ExecInfo type not currently supported.\n";
		}
	}
	// Clean up the output
	DBGOUT(std::endl)
}


void SchedViz::Visit_OChan( const Semantics::OChan &oChan, TVSuperblock *superblock ) {
	std::string oChanName = oChan.name();
	DBGOUT("\tOChan: " << oChanName << " -> ")
	// Need to create a OChan trace
	TVTrace* trace = superblock->AddTrace( oChanName );
	// Create a new schedulable in the traceViz
	TVSchedulable* sched = new TVSchedulable( Task, oChanName, false );

	// Setup the peripheral schedule
	ExecInfoSet execInfoSet = oChan.info();
	ExecInfoSet::iterator execInfoIter = execInfoSet.begin();
	// Make sure there is one ( TTSchedule )
	if (execInfoSet.size() == 1 ) {
		Semantics::Schedule schedChild = execInfoIter->Schedule_child();
		Semantics::Duration durationChild = execInfoIter->Duration_child();
		double duration = durationChild.exectimesecs();
		// See if child is a TTSchedule
		if ( Semantics::TTSchedule::meta == schedChild.type() ) {
			// Cast to a TTSchedule
			Semantics::TTSchedule ttSched = Semantics::TTSchedule::Cast( schedChild );
			// Grab the schedule info
			std::string schedule = ttSched.sched();
			// Convert the string to a nice vector
			std::vector< double > schedTimes = ConvertToVector( schedule );
			// Now create visual blocks for each invocation
			std::vector< double >::iterator timesIter = schedTimes.begin();
			for ( ; timesIter != schedTimes.end(); timesIter++ ) {
				// Add the task as a trace to the traceViz
				DBGOUT(*timesIter << " ")
				// Add an instance to the traceViz
				sched->AddInstance( trace, *timesIter, duration );
			}
		}
		// ExecInfo type not supported
		else {
			std::cout << "*** Task: ExecInfo type not currently supported.\n";
		}
	}
	// Clean up the output
	DBGOUT(std::endl)
}


void SchedViz::Visit_Bus( const Semantics::CommMedium &bus ) {
	std::string busName = bus.name();
	DBGOUT("TTBus: " << busName << std::endl)

	// Add a superblock to the viz
	TVSuperblock* superblock = this->_traceViz->AddSuperblock( busName, this->_hyperperiodSec );
	// Need to create a TTBus trace
	TVTrace* trace = superblock->AddTrace( "" );

	// Build all of the messages for this bus
	MessageSet messageSet = GetMessageSet( bus );
	MessageSet::iterator messageIter = messageSet.begin();
	for ( ; messageIter != messageSet.end(); messageIter++ ) {
		// Visit the message
		this->Visit_Msg( *messageIter, trace );
	}
}


void SchedViz::Visit_Msg( const Semantics::Msg &message, TVTrace* trace ) {
	std::string messageName = message.name();
	DBGOUT("\tMessage: " << messageName << " -> ")

	// Create a new schedulable in the traceViz
	TVSchedulable* sched = new TVSchedulable( Message, messageName, true );
	// Determine the sending task
	Semantics::Transmits transmits = message.msgTransmitter();
	Semantics::Task sendingTask = transmits.sendingTask();
	// Determine the receiving tasks
	ReceivesSet receivesSet = message.msgListeners();

	// Loop through all of the scheduling info
	ExecInfoSet execSet = message.info();
	// Make sure there is one ( TTSchedule )
	if (execSet.size() == 1 ) {
		Semantics::ExecInfo execInfo = *execSet.begin();
		// Get the children of various types
		Semantics::Schedule schedChild = execInfo.Schedule_child();
		Semantics::Duration durationChild = execInfo.Duration_child();
		double duration = durationChild.exectimesecs();
		// See if child is a TTSchedule
		if ( Semantics::TTSchedule::meta == schedChild.type() ) {
			// Cast to a TTSchedule
			Semantics::TTSchedule ttSched = Semantics::TTSchedule::Cast( schedChild );
			// Grab the schedule info
			std::string schedule = ttSched.sched();
			// Convert the string to a nice vector
			std::vector< double > schedTimes = ConvertToVector( schedule );
			// Now create visual blocks for each sending
			std::vector< double >::iterator timesIter = schedTimes.begin();
			for ( ; timesIter != schedTimes.end(); timesIter++ ) {
				DBGOUT(*timesIter << " ")
				// Add an instance to the traceViz
				sched->AddInstance( trace, *timesIter, duration );
			}
		}
		// ExecInfo type not supported
		else {
			std::cout << "*** Message: ExecInfo type not currently supported.\n";
		}
	}
	// Debug some info
	else {
		std::cout << "*** Message: Not able to find appropriate ExecInfo.\n";
	}
	// Clean up the command output
	DBGOUT(std::endl)
}


/***************************************************************************************/

