/*** Included Header Files ***/
#include "schedResults.h"
#include "Uml.h"
#include "ESMoL.h"
#include "Semantics.h"
#include <sstream>
#include <iostream>
#include <fstream>


/*** Locally Defined Types ***/
typedef enum {FIND_HYPERPERIOD, FIND_ELEMENTS} ParserState;
// Typedefs for ESMoL Paradigm
typedef std::set<ESMoL::DesignFolder> DesignFolderSet;
typedef std::set<ESMoL::Deployment> DeploymentSet;
typedef std::set<ESMoL::TTTask> TTTaskSet;
typedef std::set<ESMoL::TTMessage> TTMessageSet;
// Typedefs for ESMoL_Abstract Paradigm
typedef std::set<Semantics::Deployment> AbstractDeploymentSet;
typedef std::set<Semantics::TTBus> AbstractTTBusSet;
typedef std::set<Semantics::TTTask> AbstractTTTaskSet;
typedef std::set<Semantics::TTMsg> AbstractTTMessageSet;


/***************************************************************************************/


bool SchedKeeper::ParseResults(const std::string &resultFile)
{
	// Read in the results file
	std::ifstream resultIn;
	resultIn.open(resultFile.c_str());
	// Slurp the input file
	char buf[1000];
	std::vector<std::string> lines;
	//Add all of the file lines into the lines vector
	while(!resultIn.getline(buf, 1000).eof())
	{
		lines.push_back( std::string(buf) );
	}
	// Close the input file
	resultIn.close();

	// Parse the results file
	ParserState state = FIND_HYPERPERIOD;
	std::string currentElement;
	std::vector<std::string>::iterator lineIter;
	// Iterate through each line
	for (lineIter = lines.begin(); lineIter != lines.end(); lineIter++ )
	{
		// First we're looking for hyperperiod
		if (state == FIND_HYPERPERIOD)
		{
			// Check to see if we found Hyperperiod
			if (lineIter->find( "Hyperperiod" ) != std::string::npos)
			{
				std::string hp, units("");
				double val;
				std::istringstream inp( *lineIter );
				// Parse the line to find Hyperperiod value units
				if (inp >> hp >> val)
				{
					// If units are present, convert them for the _hyperperiodSec value
					this->_hyperperiodSec = val;
					if ( inp >> units )
					{
						if ( units.find( "ms" ) != string::npos ) this->_hyperperiodSec *= 1e-3;
						if ( units.find( "us" ) != string::npos ) this->_hyperperiodSec *= 1e-6;
						if ( units.find( "ns" ) != string::npos ) this->_hyperperiodSec *= 1e-9;
						if ( units.find( "ps" ) != string::npos ) this->_hyperperiodSec *= 1e-12;
					}

				}
				// Load the _hyperperiodStr value
				std::ostringstream outp;
				outp << val << units;
				this->_hyperperiodStr = outp.str();
				// Move to the next state
				state = FIND_ELEMENTS;
			}
			// Error case - not able to find "Hyperperiod" in the first line
			else
			{
				std::string err( "Hyperperiod parsing failure in line: " );
				err += *lineIter;
				throw SchedKeeperException(err);
			}
		}

		// Now we look for individual elements
		else if (state == FIND_ELEMENTS)
		{
			// Check to see if we found the hardware name
			if ( lineIter->find( ":" ) != std::string::npos )
			{
				// Find the semicolon
				currentElement = lineIter->substr(0, lineIter->find( ":" ));
				// Save before the semicolon for later
				this->_harewareElements.push_back(currentElement);
			}
			// Otherwise, we must have an element going on
			else if ( lineIter->find( "/" ) != std::string::npos ) // is this a schedule line?
			{
				std::string name;
				double start;
				std::istringstream inp(*lineIter);
				if (inp >> name >> start)
				{
					// Convert start to a string
					std::ostringstream startStr;
					startStr << start;
					// Trim the instance #
					name = name.substr(0, name.find_last_of("_"));
					// See if this element is already in the map
					std::map<std::string, std::string>::iterator mapIter = this->_elements.find(name);
					// If it is not in the map, add it in with a blank schedule string
					if (mapIter == this->_elements.end())
					{
						mapIter = this->_elements.insert(this->_elements.end(), std::make_pair(name, startStr.str()));
						// If debug, how what is being inserted
						if (this->_debug) std::cout << "Inserting " << name << ":" << startStr.str() << " into element map.\n";
					}
					// Otherwise, add the schedule value onto the schedule string
					else
					{
						// Append the new schedule item
						std::string schedule = (*mapIter).second;
						schedule.append(",");
						schedule.append(startStr.str());
						// Update the schedule string value of the element
						(*mapIter).second = schedule;
						// If debug, show update
						if (this->_debug) std::cout << "\tUpdating " << name << " with schedule: " << startStr.str() << "\n.";
					}
				}
				// Error Case - error in parsing the schedule line
				else
				{
					std::string err( "Failed to parse schedule line: " );
					err += *lineIter;
					throw SchedKeeperException( err );
				}
			}
		}
	}
	// We are done here
	return true;
}


bool SchedKeeper::PopulateEsmol(const std::string &esmolModel)
{
	// Quality flag
	bool clean = true;
	// Start UDM using the ESMoL paradigm
	Udm::SmartDataNetwork esmolSDN(ESMoL::diagram);
	try
	{
		// Open the model file
		esmolSDN.OpenExisting(esmolModel);
	} catch (udm_exception &ex) {
		std::cout << "Exception occured in udm:" << std::endl;
		std::cout << ex.what() << std::endl;
		throw SchedKeeperException(std::string(ex.what()));
	}
	//Get the root folder of the project
	ESMoL::RootFolder erf = ESMoL::RootFolder::Cast(esmolSDN.GetRootObject());
	// Get the set of DesignFolder in the root folder
	DesignFolderSet designFolderSet = erf.DesignFolder_children();
	DesignFolderSet::iterator designFolderSetIter = designFolderSet.begin();
	// For each design folder ...
	for (; designFolderSetIter != designFolderSet.end(); designFolderSetIter++)
	{
		// Get the set of children Deployments
		DeploymentSet deploymentSet = (*designFolderSetIter).Deployment_children();
		DeploymentSet::iterator deploymentSetIter = deploymentSet.begin();
		// For each deployment ...
		for (; deploymentSetIter != deploymentSet.end(); deploymentSetIter++)
		{
			// Set the Hyperperiod
			(*deploymentSetIter).setStringAttr(ESMoL::Deployment::meta_Hyperperiod, this->_hyperperiodStr);
			// Get the set of children tasts
			TTTaskSet ttTaskSet = (*deploymentSetIter).TTTask_kind_children();
			TTTaskSet::iterator ttTaskSetIter = ttTaskSet.begin();
			// For each task ...
			for (; ttTaskSetIter != ttTaskSet.end(); ttTaskSetIter++)
			{
				// Get the set of task assignments
				std::set<ESMoL::TaskAssignment> taskAssignmentSet = (*ttTaskSetIter).dstTaskAssignment();
				// Make sure there is one assignment
				if (taskAssignmentSet.size() == 1)
				{
					// Get the name of the Node this task is assigned to
					ESMoL::NodeRef nodeRef = (*taskAssignmentSet.begin()).dstTaskAssignment_end();
					ESMoL::Node node = nodeRef.ref();
					std::string nodeName = node.name();
					 // Build the full name of the TTTask
					std::string taskName = nodeName + std::string("/");
					taskName += (*ttTaskSetIter).name();
					// See if that name is in the elements map
					std::map<std::string,std::string>::iterator elemIter = this->_elements.find(taskName);
					// If found, set the TTSched attribute
					if (elemIter != this->_elements.end())
					{
						(*ttTaskSetIter).setStringAttr(ESMoL::TTTask::meta_TTSchedule, (*elemIter).second);
						// If debug, show what is being updated
						if (this->_debug) std::cout << "ESMoL Task Update: " << taskName << ", with sched: " << (*elemIter).second << ".\n";
					}
					// Error if task is not found in schedule
					else
					{
						std::string err( "Task not found in elements map: ");
						err += taskName;
//						throw SchedKeeperException( err );
						std::cout << err << std::endl;
					}
				}

				// Get the set of children TTMessages
				TTMessageSet ttMessageSet = (*ttTaskSetIter).TTMessage_kind_children();
				TTMessageSet::iterator ttMessageSetIter = ttMessageSet.begin();
				// For each message ...
				for (; ttMessageSetIter != ttMessageSet.end(); ttMessageSetIter++)
				{
					std::string debugName = (*ttMessageSetIter).name();
					// Get the set of assignments for this message
					std::set<ESMoL::MsgAssignment> msgAssignmentSet = (*ttMessageSetIter).dstMsgAssignment();
					// Make sure the msg is assigned to only one channel
					if (msgAssignmentSet.size() == 1) {
						// Get the channel the message is assigned to
						ESMoL::ChannelRef channelRef = (*msgAssignmentSet.begin()).dstMsgAssignment_end();
						ESMoL::Channel channel = channelRef.ref();
						std::set<ESMoL::Wire> wireSet = channel.dstWire();
						// Make sure the channel is connected to only one wire
						if (wireSet.size() == 1) {
							// Get the wire
							ESMoL::Wire wire = *wireSet.begin();
							// Get the bus the wire is connected to
							ESMoL::Bus bus = ESMoL::Bus::Cast(wire.dstWire_end());
							std::string busName = bus.name();
							// Build the full name of the TTMessage
							std::string msgName = busName + std::string("/");
							msgName += (*ttMessageSetIter).name();
							// See if that name is in the elements map
							std::map<std::string,std::string>::iterator elemIter = this->_elements.find(msgName);
							// If found, set the TTSched attribute
							if (elemIter != this->_elements.end())
							{
								(*ttMessageSetIter).setStringAttr(ESMoL::TTMessage::meta_TTSchedule, (*elemIter).second);
								// If debug, show what is being updated
								if (this->_debug) std::cout << "ESMoL Message Update: " << msgName << ", with sched: " << (*elemIter).second << ".\n";
							}
							// Error if message is not found in element list
							else
							{
								std::string err( "Message not found in elements map: ");
								err += msgName;
//								throw SchedKeeperException( err );
								std::cout << err << std::endl;
							}
						}
					}
				}
			}
		}
	}
	// If all is good, update the file
	if (clean) esmolSDN.CloseWithUpdate();
	else esmolSDN.CloseNoUpdate();
	// Return the clean flag
	return clean;
}


bool SchedKeeper::PopulateAbstract(const std::string &abstractModel)
{
	// Quality flag
	bool clean = true;
	// Start UDM using the ESMoL_Abstract (Semantics) paradigm
	Udm::SmartDataNetwork abstractSDN(Semantics::diagram);
	try
	{
		// Try to open the model file
		abstractSDN.OpenExisting(abstractModel);
	} catch (udm_exception &ex) {
		std::cout << "Exception occured in udm:" << std::endl;
		std::cout << ex.what() << std::endl;
		throw SchedKeeperException(std::string(ex.what()));
	}
	//Get the root folder of the project
	Semantics::RootFolder srf = Semantics::RootFolder::Cast(abstractSDN.GetRootObject());
	// Get the set of children Deployments
	AbstractDeploymentSet deploymentSet = srf.Deployment_children();
	AbstractDeploymentSet::iterator deploymentSetIter = deploymentSet.begin();
	// For each deployment ...
	for (; deploymentSetIter != deploymentSet.end(); deploymentSetIter++)
	{
		// Get the set of children TTBuss
		AbstractTTBusSet ttBusSet = (*deploymentSetIter).TTBus_kind_children();
		AbstractTTBusSet::iterator ttBusSetIter = ttBusSet.begin();
		// For each bus ...
		for (; ttBusSetIter != ttBusSet.end(); ttBusSetIter++)
		{
			// Set the bus hyperperiod value using _hyperperiodSec value
			(*ttBusSetIter).setRealAttr(Semantics::TTBus::meta_hyperperiodsecs, this->_hyperperiodSec);
		}

		// Get the set of children TTTasks
		AbstractTTTaskSet ttTaskSet = (*deploymentSetIter).TTTask_kind_children();
		AbstractTTTaskSet::iterator ttTaskSetIter = ttTaskSet.begin();
		// For each task ...
		for (; ttTaskSetIter != ttTaskSet.end(); ttTaskSetIter++)
		{
			// Make sure there is a node, then get the name
			std::string taskName;
			try
			{
				// Get the name of the Node this task is assigned to
				Semantics::Node node = (*ttTaskSetIter).executedBy();
				// Add the name and separator
				taskName = node.name();
				taskName += std::string("/");
			} catch (...) { }

			// Build the full name of the TTTask
			taskName += (*ttTaskSetIter).name();
			// See if that name is in the elements map
			std::map<std::string,std::string>::iterator elemIter = this->_elements.find(taskName);
			// If found, set the schedule attribute
			if (elemIter != this->_elements.end())
			{
				(*ttTaskSetIter).setStringAttr(Semantics::TTTask::meta_schedule, (*elemIter).second);
				// If debug, show what is being updated
				if (this->_debug) std::cout << "Abstract Task Update: " << taskName << ", with sched: " << (*elemIter).second << ".\n";
			}
			// Error if task is not found in schedule
			else
			{
				std::string err( "Task not found in elements map: ");
				err += taskName;
//				throw SchedKeeperException( err );
				std::cout << err << std::endl;
			}
		}

		// Get the set of children TTMsgs
		AbstractTTMessageSet ttMessageSet = (*deploymentSetIter).TTMsg_kind_children();
		AbstractTTMessageSet::iterator ttMessageSetIter = ttMessageSet.begin();
		// For each message ...
		for (; ttMessageSetIter != ttMessageSet.end(); ttMessageSetIter++)
		{
			try
			{
				// Get the Transmits, BChan, CommInterface, and TTBus(CommMedium) that the message is sent on
				Semantics::Transmits transmits = (*ttMessageSetIter).msgTransmitter();\
				Semantics::BChan bChan = transmits.sendingChan();
				Semantics::CommInterface commInterface = bChan.chanInterface();
				Semantics::CommMedium bus = commInterface.commMedium();
				// Add the bus name and separator to the message name
				std::string msgName = bus.name();
				msgName += std::string("/");

				// Build the full name of the TTMessage
				msgName += (*ttMessageSetIter).name();
				// See if that name is in the elements map
				std::map<std::string,std::string>::iterator elemIter = this->_elements.find(msgName);
				// If found, set the schedule attribute
				if (elemIter != this->_elements.end())
				{
					(*ttMessageSetIter).setStringAttr(Semantics::TTMsg::meta_schedule, (*elemIter).second);
					// If debug, show what is being updated
					if (this->_debug) std::cout << "Abstract Message Update: " << msgName << ", with sched: " << (*elemIter).second << ".\n";
				}
				// Error if message is not found in element list
				else
				{
					std::string err( "Message not found in elements map: ");
					err += msgName;
//					throw SchedKeeperException( err );
					std::cout << err << std::endl;
				}
			} 
			// This catches cases where a message is not fully wired (via Transmits, BChan, CommInterface,etc.) to a bus
			catch (...) { }
		}
	}


	// If all is good, update the file
	if (clean) abstractSDN.CloseWithUpdate();
	else abstractSDN.CloseNoUpdate();
	// Return the clean flag
	return clean;
}


/***************************************************************************************/


SchedKeeper::SchedKeeper(const std::string &resultFile, const std::string &esmolModel, 
						 const std::string &abstractModel, const bool &debug)
						 :	_debug(debug), _hyperperiodStr(), _hyperperiodSec(), _elements(), _harewareElements()
{
	// Override _debug if _DEBUG is defined
#ifdef _DEBUG
	this->_debug = true;
#endif

	try
	{
		// Parse the results file
		this->ParseResults(resultFile);
		// If an ESMoL model is present, inject results
		if (esmolModel.compare("") != 0)
		{
			this->PopulateEsmol(esmolModel);
		}

		// If an ESMoL_Abstract model is present, inject results
		if (abstractModel.compare("") != 0)
		{
			this->PopulateAbstract(abstractModel);
		}
	} 
	// Catch any exceptions and see what is going on
	catch(SchedKeeperException &ex) {
		std::cout << "Exception: " << ex._reason << std::endl;
	}
}


/***************************************************************************************/

