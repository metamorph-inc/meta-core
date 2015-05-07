/*** Included Header Files ***/
#include "schedResults.h"


/***************************************************************************************/


std::string BuildESMoLName( ESMoL::Executable &executable, ESMoL::ComponentRef &execRef, std::string &busHyperperiod );


/***************************************************************************************/


bool SchedResults::PopulateEsmol( const std::string &esmolModel ) {
	std::cout << "***** Populating ESMoL Model: " << esmolModel << " *****\n";
	// Quality flag
	bool clean = true;
	// Start UDM using the ESMoL paradigm
	Udm::SmartDataNetwork esmolSDN( ESMoL::diagram );
	try {
		// Open the model file
		esmolSDN.OpenExisting( esmolModel );
	} catch ( udm_exception &ex ) {
		std::cout << "Exception occured trying to open " << esmolModel << " in udm:" << std::endl;
		std::cout << ex.what() << std::endl;
		throw SchedResultsException( std::string( ex.what() ) );
	}
	//Get the root folder of the project
	ESMoL::RootFolder erf = ESMoL::RootFolder::Cast(esmolSDN.GetRootObject());
	// Get the set of DesignFolder in the root folder
	DesignFolderSet designFolderSet = erf.DesignFolder_children();
	DesignFolderSet::iterator designFolderSetIter = designFolderSet.begin();
	// For each design folder ...
	for (; designFolderSetIter != designFolderSet.end(); designFolderSetIter++) {
		// Get the set of children Systems
		SystemSet systemSet = (*designFolderSetIter).System_children();
		SystemSet::iterator systemSetIter = systemSet.begin();
		// For each system ...
		for (; systemSetIter != systemSet.end(); systemSetIter++) {
			// Get the set of children TTExecInfos
			TTExecInfoVector ttExecInfoVector = systemSetIter->TTExecInfo_kind_children();
			TTExecInfoVector::iterator ttExecInfoVectorIter = ttExecInfoVector.begin();
			for (; ttExecInfoVectorIter != ttExecInfoVector.end(); ttExecInfoVectorIter++) {
				// Get the first (if any) execution assignment
				ExecutionAssignmentSet execAssignmentSet = ttExecInfoVectorIter->dstExecutionAssignment();
				ExecutionAssignmentSet::iterator execAssignmentIter = execAssignmentSet.begin();
				if (this->_debug) 
					std::cout << "\tWorking on " << ttExecInfoVectorIter->name() << " with " << execAssignmentSet.size() << " assignments.\n";
				// No connections to the TTExecInfo object
				if ( execAssignmentSet.size() == 0 ) {
					// If there is debugging...
					std::cout <<"\t--- Error: No ExecutionAssignment connection to TTExecInfo: " << ttExecInfoVectorIter->name() << std::endl;
				}

				// Check each assignment
				for ( ; execAssignmentIter != execAssignmentSet.end(); execAssignmentIter++ ) {
					// Get the base executable it is attached to
					ESMoL::Executable executable = execAssignmentIter->dstExecutionAssignment_end();
					ESMoL::ComponentRef execRef = ESMoL::ComponentRef::Cast(execAssignmentIter->dstExecutionAssignment__rp_helper());
					// Build the name for the element
					std::string elementName = BuildESMoLName( executable, execRef, this->_hyperperiodStr );
					// Make sure a name is returned
					if ( elementName != "" ) {
						// See if that name is in the elements map
						std::map<std::string,std::string>::iterator elemIter = this->_elements.find( elementName );
						// If found ...
						if (elemIter != this->_elements.end()) {
							// Set the schedule string for this execInfo
							ttExecInfoVectorIter->setStringAttr( ESMoL::TTExecInfo::meta_TTSchedule, elemIter->second );
							// If debug, show what is being updated
							if (this->_debug) std::cout << "\t\tESMoL Update: " << elementName << ", set Sched: " << (*elemIter).second << ".\n";
						}
						// Print some info if element is not found in schedule
//						else {
//							std::cout << "\t--- Warning: " << elementName << " - not found in map.\n";
//						}
					}
				}

			}
		} // System Iterator
	} // DesignFolder Iterator
	// If all is good, update the file
	if (clean) {
		esmolSDN.CloseWithUpdate();
	}
	else {
		std::cout << "--- There were errors.  Not saving results.\n";
		esmolSDN.CloseNoUpdate();
	}
	// Return the clean flag
	return clean;
}


/***************************************************************************************/


std::string BuildESMoLName( ESMoL::Executable &executable, ESMoL::ComponentRef &compRef, std::string &busHyperperiod ) {
	std::string elementName;

	/*** See if this is a ComponentRef ***/
	if ( ESMoL::ComponentRef::meta == executable.type() ) {
		try {
			// Cast to a component ref
			ESMoL::ComponentRef componentRef = ESMoL::ComponentRef::Cast( executable );
			// Get the component
			ESMoL::ComponentRef_RefersTo_Base tmpBase = componentRef.ref();
			ESMoL::Component component = ESMoL::Component::Cast( tmpBase );
			// Get the set of component assignments
			ComponentAssignmentSet componentAssignmentSet = componentRef.dstComponentAssignment();
			// Make sure there is only one assignment
			if ( componentAssignmentSet.size() != 1) {
				// Do something ...
				std::cout << "\t--- Error: Component " << component.name() << " must have only one Assignment.\n";
				return "";
			}
			// Get the name of the Node this componentRef is assigned to
			ESMoL::NodeRef nodeRef = componentAssignmentSet.begin()->dstComponentAssignment_end();
			ESMoL::Node node = nodeRef.ref();
			std::string nodeName = node.name();
			 // Build the full name of the Component
			elementName = nodeName + std::string("/");
			elementName += component.name();
		} 
		// Catch any exceptions
		catch(...) {
			std::cout << "There was an exception while handling an ESMoL::ComponentRef element.\n";
			return "";
		}
	}

	/*** See if this is a MessageRef ***/
	else if ( ESMoL::MessageRef::meta == executable.type() ) {
		try {
			// Cast to a message ref
			ESMoL::MessageRef messageRef = ESMoL::MessageRef::Cast( executable );
			std::string messageName = messageRef.name();
			// Get the associated parent Component name
			std::string componentName = compRef.name();

			// Get the associated destination commMapping
			ESMoL::Channel goldenChannel;
			bool found = false;
			std::set< ESMoL::CommMapping > commMappingSet = messageRef.dstCommMapping();
			std::set< ESMoL::CommMapping >::iterator commMappingIter = commMappingSet.begin();
			// Iterate through dest commMappings
			for ( ; commMappingIter != commMappingSet.end(); commMappingIter++ ) {
				// Get the Ref-Port helper for the commMapping Destinations
				ESMoL::ComponentRef tmpCompRef = ESMoL::ComponentRef::Cast(commMappingIter->srcCommMapping__rp_helper());
				std::string tmpName = tmpCompRef.name();
				// Make sure we are looking at the same things
				if ( tmpName == componentName ) {
					goldenChannel = ESMoL::Channel::Cast( commMappingIter->dstCommMapping_end() );
					found = true;
				}
			}

			// Get the associated source commMapping
			commMappingSet = messageRef.srcCommMapping();
			commMappingIter = commMappingSet.begin();
			// Iterate through commMappings
			for ( ; commMappingIter != commMappingSet.end(); commMappingIter++ ) {
				// Get the compRef
				ESMoL::ComponentRef tmpCompRef = ESMoL::ComponentRef::Cast(commMappingIter->dstCommMapping__rp_helper());
				std::string tmpName = tmpCompRef.name();
				// Make sure we are looking at the same things
				if ( tmpName == componentName ) {
					goldenChannel = ESMoL::Channel::Cast( commMappingIter->srcCommMapping_end() );
					found = true;
				}
			}
			// If not found
			if ( !found ) {
				std::cout << "\t--- Error: " << componentName << "." << messageName << " -- No valid channel.\n";
				return "";
			}
			// Check if golden channel is OChan
			if ( goldenChannel.type() == ESMoL::OChan::meta ) {
				// Get the associated channel
				ESMoL::OChan channel = ESMoL::OChan::Cast( goldenChannel );
//				std::cout << "\t--- OChan Found: " << componentName << "." << messageName << ".\n";
				// Get the set of component assignments
				ComponentAssignmentSet componentAssignmentSet = compRef.dstComponentAssignment();
				// Make sure there is only one assignment
				if ( componentAssignmentSet.size() != 1) {
					// Do something ...
					std::cout << "\t--- Error: Component " << compRef.name() << " must have only one Assignment.\n";
					return "";
				}
				// Get the name of the Node this componentRef is assigned to
				ESMoL::NodeRef nodeRef = componentAssignmentSet.begin()->dstComponentAssignment_end();
				ESMoL::Node node = nodeRef.ref();
				std::string nodeName = node.name();
				// Get the attached IO devices
				std::set< ESMoL::ActuationConnection > actuationSet = channel.dstActuationConnection();
				// Make sure there is only one device
				if (actuationSet.size() != 1 ) {
					std::cout << "\t--- Error: Component " << compRef.name() << " must have only one Actuator.\n";
					return "";
				}
				// Get the name of the device this actuation connection is connected to
				ESMoL::IODevice device = actuationSet.begin()->dstActuationConnection_end();
				std::string deviceName = device.name();
				elementName = nodeName + std::string("/") + deviceName;
				return elementName;
			}
			// Check if golden channel is IChan
			else if ( goldenChannel.type() == ESMoL::IChan::meta ) {
				// Get the associated channel
				ESMoL::IChan channel = ESMoL::IChan::Cast( goldenChannel );
//				std::cout << "\t--- IChan Found: " << componentName << "." << messageName << ".\n";
				// Get the set of component assignments
				ComponentAssignmentSet componentAssignmentSet = compRef.dstComponentAssignment();
				// Make sure there is only one assignment
				if ( componentAssignmentSet.size() != 1) {
					// Do something ...
					std::cout << "\t--- Error: Component " << compRef.name() << " must have only one Assignment.\n";
				}
				// Get the name of the Node this componentRef is assigned to
				ESMoL::NodeRef nodeRef = componentAssignmentSet.begin()->dstComponentAssignment_end();
				ESMoL::Node node = nodeRef.ref();
				std::string nodeName = node.name();
				// Get the attached IO devices
				std::set< ESMoL::AcquisitionConnection > acquisitionSet = channel.srcAcquisitionConnection();
				// Make sure there is only one device
				if (acquisitionSet.size() != 1 ) {
					std::cout << "\t--- Error: Component " << compRef.name() << " must have only one Sensor.\n";
					return "";
				}
				// Get the name of the device this actuation connection is connected to
				ESMoL::IODevice device = acquisitionSet.begin()->srcAcquisitionConnection_end();
				std::string deviceName = device.name();
				elementName = nodeName + std::string("/") + deviceName;
				return elementName;
			}
			// Check if golden channel is BChan
			else if ( goldenChannel.type() == ESMoL::BChan::meta ) {
				// Get the associated BChan
				ESMoL::BChan bChan = ESMoL::BChan::Cast( goldenChannel );
				// Get the associated Wire
				std::set< ESMoL::Wire > wireSet = bChan.dstWire();
				ESMoL::Bus bus;
				// Make sure wireset > 0
				if ( wireSet.size() != 1 ) {
					// Flip around
					wireSet = bChan.srcWire();
					// Make sure there is only one
					if ( wireSet.size() != 1 ) {
						std::cout << "\t--- Error: " << componentName << "." << messageName << " -- No valid WireSet.\n";
						return "";
					// Otherwise get the bus
					} else {
						// Get the associated bus
						bus = ESMoL::Bus::Cast( wireSet.begin()->srcWire_end() );
					}
				}
				else {
					// Get the associated bus
					bus = ESMoL::Bus::Cast( wireSet.begin()->dstWire_end() );
				}
				// Build the full name of the Message
				std::string busName = bus.name();
				elementName = busName + std::string("/");
				elementName += componentName;
				elementName += ".";
				elementName += messageName;
				// Set the Hyperperiod for the bus

				if ( bus.type() == ESMoL::TTBus::meta )
				{
					ESMoL::TTBus ttbus = ESMoL::TTBus::Cast( bus );
					ttbus.setStringAttr( ESMoL::TTBus::meta_Hyperperiod, busHyperperiod );
				}
				else if ( bus.type() == ESMoL::CANBus::meta )
				{
					ESMoL::CANBus canbus = ESMoL::CANBus::Cast( bus );
					canbus.setStringAttr( ESMoL::CANBus::meta_Hyperperiod, busHyperperiod );
				}
				
			}
		}
		// Catch any exceptions
		catch (...) {
			std::cout << "There was an exception while handling an ESMoL::MessageRef element.\n";
			return "";
		}
	}

	/*** Error Case ***/
	else {
		std::cout << "There was an exception while handling an ESMoL::????? element.\n";
		return "";
	}
	// Return the element name
	return elementName;
}


/***************************************************************************************/

