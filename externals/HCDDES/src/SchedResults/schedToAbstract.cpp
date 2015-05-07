/*** Included Header Files ***/
#include "schedResults.h"
#include "Uml.h"
#include "ESMoL\generated\ESMoL.h"

#include <sstream>

/***************************************************************************************/


bool SchedResults::PopulateAbstract( const std::string &abstractModel ) {
	std::cout << "***** Populating ESMoL_Abstract Model: " << abstractModel << " *****\n";
	// Quality flag
	bool clean = true;
	// Start UDM using the ESMoL_Abstract (Semantics) paradigm
	Udm::SmartDataNetwork abstractSDN( Semantics::diagram );
	try {
		// Try to open the model file
		abstractSDN.OpenExisting( abstractModel );
	} catch (udm_exception &ex) {
		std::cout << "Exception occured trying to open " << abstractModel << " in udm:" << std::endl;
		std::cout << ex.what() << std::endl;
		throw SchedResultsException( std::string( ex.what() ) );
	}
	//Get the root folder of the project
	Semantics::RootFolder srf = Semantics::RootFolder::Cast( abstractSDN.GetRootObject() );
	// Get the set of children Deployments
	AbstractDeploymentSet deploymentSet = srf.Deployment_children();
	AbstractDeploymentSet::iterator deploymentSetIter = deploymentSet.begin();
	// For each deployment ...
	for ( ; deploymentSetIter != deploymentSet.end(); deploymentSetIter++ ) {
		// We had better have at least one node child
		AbstractNodeSet nodeSet = deploymentSetIter->Node_children();
		AbstractNodeSet::iterator nodeIter = nodeSet.begin();
		double tickRes;
		unsigned long long hyperperiodTicks;
		for ( ; nodeIter != nodeSet.end(); nodeIter++ ) {
			// Get the tick resolution
			tickRes = nodeIter->tickresolutionsecs();
			// If the tick is very small...
			if ( tickRes < 1.0e-12 ) {
				// Substitute 1ms as the resolution
				std::cout << "Tick resolution of node " << nodeIter->name() << " is very close to zero!!! Using 1 ms instead.\n";
				tickRes = 1.0e-3;
			}
			nodeIter->hyperperiodsecs() = this->_hyperperiodSec;
			nodeIter->hyperperiodticks() = (unsigned long long)( ( this->_hyperperiodSec / tickRes ) + 0.5 );
			hyperperiodTicks = nodeIter->hyperperiodticks();
		}

		// Get the set of children TTBuss
		AbstractBusVector busVector = deploymentSetIter->CommMedium_children();
		AbstractBusVector::iterator busVectorIter = busVector.begin();
		// For each bus ...
		for ( ; busVectorIter != busVector.end(); busVectorIter++ ) {
			// Set the bus hyperperiod value using _hyperperiodSec value
			busVectorIter->setRealAttr( Semantics::CommMedium::meta_hyperperiodsecs, this->_hyperperiodSec );
			busVectorIter->hyperperiodticks() = hyperperiodTicks;
		}

		// Get the set of children ExecInfo
		std::set< Semantics::ExecInfo > execInfoSet = deploymentSetIter->ExecInfo_children();
		std::set< Semantics::ExecInfo >::iterator execInfoIter = execInfoSet.begin();
		// For each ExecInfo ..
		for ( ; execInfoIter != execInfoSet.end(); execInfoIter++ ) {
			std::vector< std::string > elementNames;
			// Determine name of associated Executable
			std::set< Semantics::Executable > executableSet = execInfoIter->executor();
			std::set< Semantics::Executable >::iterator execSetIter = executableSet.begin();
			// Work through all executables
			for ( ; execSetIter != executableSet.end(); execSetIter++ ) {
				std::string elementName;
				// Message Case
				if ( Semantics::Msg::meta == execSetIter->type() ) {
					// Cast to a msg
					Semantics::Msg msg = Semantics::Msg::Cast( *execSetIter );
					// Get the message transmitter
					Semantics::Transmits transmitter = msg.msgTransmitter();
					// Make sure the transmitter is valid
					if ( transmitter != Udm::null ) {
						// Get the associated node
						Semantics::BChan bChan = transmitter.sendingChan();
						Semantics::CommInterface commInterface = bChan.chanInterface();
						Semantics::CommMedium commMedium = commInterface.commMedium();
						elementName = std::string( commMedium.name() ) + "/" + std::string( msg.name() );
						// Add to the vector
						elementNames.push_back( elementName );
					}
				}
				// Task Case
				else if ( Semantics::Task::meta == execSetIter->type() ) {
					// Cast to a task
					Semantics::Task task = Semantics::Task::Cast( *execSetIter );
					// Get the task node
					Semantics::Node node = task.executedBy();
					// Determine the final elementName
					elementName = std::string( node.name() ) + "/" + std::string( task.name() );
					// Add to the vector
					elementNames.push_back( elementName );
				}
				// InputDevice Case
				else if ( Semantics::InputDevice::meta == execSetIter->type() ) {
					// Cast to a input device
					Semantics::InputDevice device = Semantics::InputDevice::Cast( *execSetIter );
					// Get the node
					Semantics::Node node = device.integratedBy();
					// Determine the final elementName
					elementName = std::string( node.name() ) + "/" + std::string( device.name() );
					// Add to the vector
					elementNames.push_back( elementName );
				}
				// OutputDevice Case
				else if ( Semantics::OutputDevice::meta == execSetIter->type() ) {
					// Cast to a output device
					Semantics::OutputDevice device = Semantics::OutputDevice::Cast( *execSetIter );
					// Get the node
					Semantics::Node node = device.integratedBy();
					// Determine the final elementName
					elementName = std::string( node.name() ) + "/" + std::string( device.name() );
					// Add to the vector
					elementNames.push_back( elementName );
				}
				// IChan Case
				else if ( Semantics::IChan::meta == execSetIter->type() ) {
					// Cast to a IChan channel
					Semantics::IChan channel = Semantics::IChan::Cast( *execSetIter );
					std::string chanName = channel.name();
					// Get the device and check if there is one
					Semantics::InputDevice device = channel.ichanDevice();
					if ( device != Udm::null ) {
						// Get the node
						Semantics::Node node = device.integratedBy();
						// Determine the final elementName
						elementName = std::string( node.name() ) + "/" + std::string( device.name() );
						// Add to the vector
						elementNames.push_back( elementName );
					}
					// Debug some output
					else {
						if (this->_debug) std::cout << "\t--- Warning: IChan " << channel.name() << " not connected to anything.\n";
					}
				}
				// OChan Case
				else if ( Semantics::OChan::meta == execSetIter->type() ) {
					// Cast to a OChan channel
					Semantics::OChan channel = Semantics::OChan::Cast( *execSetIter );
					// Get the device and check if there is one
					Semantics::OutputDevice device = channel.ochanDevice();
					if ( device != Udm::null ) {
						// Get the node
						Semantics::Node node = device.integratedBy();
						// Determine the final elementName
						elementName = std::string( node.name() ) + "/" + std::string( device.name() );
						// Add to the vector
						elementNames.push_back( elementName );
					}
					// Debug some output
					else {
						if (this->_debug) std::cout << "\t--- Warning: OChan " << channel.name() << " not connected to anything.\n";
					}
				}
				// BChan Case
				//if ( Semantics::BChan::meta == execSetIter->type() ) {
				//	// Cast to a BChan channel
				//	Semantics::BChan channel = Semantics::BChan::Cast( *execSetIter );
				//	// Get the name
				//	std::cout << "\t--- BChan: " << channel.name() << std::endl;
				//	elementName = "";
				//}
				// Error case
				else {
					// Debug some output
					std::cout << "\t--- Unable to determine type of ExecInfo!!!\n";
				}
			}
			// Loop through any names
			std::vector< std::string >::iterator nameIter = elementNames.begin();
			for ( ; nameIter != elementNames.end(); nameIter++ ) {
				// Get the child TTSchedule
				std::set< Semantics::TTSchedule > ttSchedSet = execInfoIter->TTSchedule_kind_children();
				Semantics::TTSchedule ttSched = *( ttSchedSet.begin() );
				// See if that name is in the elements map
				std::map< std::string,std::string >::iterator elemIter = this->_elements.find( *nameIter );
				std::map< std::string, std::vector< double > >::iterator valIter = this->_values.find( *nameIter );
				// If found ...
				if (elemIter != this->_elements.end()) {
					// Set the schedule string for this execInfo
					ttSched.setStringAttr( Semantics::TTSchedule::meta_sched, elemIter->second );
					// If debug, show what is being updated
					if (this->_debug) std::cout << "\tSemantics Update: " << *nameIter << ", Set Sched: " << elemIter->second << ".\n";
					// Need to update the tickSched value
					bool first = true;
					std::ostringstream tickStr;
					vector< double >::iterator tickIter = valIter->second.begin();
					// Loop and append the tick values to the string
					for ( ; tickIter != valIter->second.end(); tickIter++ ) {
						if ( !first ) {
							tickStr << ", ";
						} else {
							first = false;
						}
						// Calculate the tickVal
						unsigned long long tickVal = (unsigned long long) (( *tickIter / tickRes ) + 0.5 );
						tickStr << tickVal;
					}
					// Update the tickSched value
					ttSched.ticksched() = tickStr.str();
				}
				// Error if element is not found in schedule
				else {
					std::cout << "\t--- Warning: " << *nameIter << " - not found in map.\n";
				}
			}
		}
	}


	// If all is good, update the file
	if ( clean ) {
		abstractSDN.CloseWithUpdate();
	}
	else {
		std::cout << "--- There were errors.  Not saving results.\n";
		abstractSDN.CloseNoUpdate();
	}
	// Return the clean flag
	return clean;
}


/***************************************************************************************/

