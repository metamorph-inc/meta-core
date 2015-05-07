/*** Included Header Files ***/
#include "schedResults.h"


/***************************************************************************************/


bool SchedResults::ParseResults( const std::string &resultFile ) {
	std::cout << "***** Parsing Scheduler Results: " << resultFile << " *****\n";
	// Read in the results file
	std::ifstream resultIn;
	resultIn.open(resultFile.c_str());
	if (!resultIn.good())
		throw SchedResultsException(std::string("Couldn't open file '") + resultFile + "'");
	// Slurp the input file
	char buf[1000];
	std::vector<std::string> lines;
	//Add all of the file lines into the lines vector
	while(!resultIn.getline(buf, 1000).eof()) {
		if (!resultIn.good())
			throw SchedResultsException(std::string("Error reading '") + resultFile + "'");
		lines.push_back( std::string(buf) );
	}
	// Close the input file
	resultIn.close();

	// Parse the results file
	ParserState state = FIND_HYPERPERIOD;
	std::string currentElement;
	std::vector<std::string>::iterator lineIter;
	// Iterate through each line
	for (lineIter = lines.begin(); lineIter != lines.end(); lineIter++ ) {
		// First we're looking for hyperperiod
		if (state == FIND_HYPERPERIOD) {
			// Check to see if we found Hyperperiod
			if (lineIter->find( "Hyperperiod" ) != std::string::npos) {
				std::string hp, units("");
				double val;
				std::istringstream inp( *lineIter );
				// Parse the line to find Hyperperiod value units
				if ( inp >> hp >> val ) {
					// If units are present, convert them for the _hyperperiodSec value
					this->_hyperperiodSec = val;
					if ( inp >> units ) {
						// Determine the results units factor to adjust all times to seconds
						if ( units.find( "ms" ) != string::npos ) this->_unitsFactor = 1e-3;
						else if ( units.find( "us" ) != string::npos ) this->_unitsFactor = 1e-6;
						else if ( units.find( "ns" ) != string::npos ) this->_unitsFactor = 1e-9;
						else if ( units.find( "ps" ) != string::npos ) this->_unitsFactor = 1e-12;
						// Error check
						else {
							std::cout << "--- !!! Unknown units describing hyperperiod: " << units << ". Assuming Seconds\n";
							this->_unitsFactor = 1.0;
						}
						// Adjust the hyperperiod to seconds
						this->_hyperperiodSec *= this->_unitsFactor;
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
			else {
				std::string err( "Hyperperiod parsing failure in line: " );
				err += *lineIter;
				throw SchedResultsException(err);
			}
		}

		// Now we look for individual elements
		else if (state == FIND_ELEMENTS) {
			// Check to see if we found the hardware name
			if ( lineIter->find( ":" ) != std::string::npos ) {
				// Find the semicolon
				currentElement = lineIter->substr( 0, lineIter->find( ":" ) );
				// Save before the semicolon for later
				this->_harewareElements.push_back(currentElement);
			}
			// Otherwise, we must have an element going on
			else if ( lineIter->find( "/" ) != std::string::npos ) {
				std::string name;
				double start;
				std::istringstream inp(*lineIter);
				if ( inp >> name >> start ) {
					// Convert start to a string
					std::ostringstream startStr;
					// Adjust all times to seconds using _unitsFactor
					start *= this->_unitsFactor;
					// Convert to a string
					startStr << start;
					// Trim the instance #
					name = name.substr(0, name.find_last_of("_"));
					// See if this element is already in the map
					std::map<std::string, std::string>::iterator mapIter = this->_elements.find(name);
					std::map<std::string, std::vector< double > >::iterator valIter = this->_values.find(name);
					// If it is not in the map, add it in with a blank schedule string
					if (mapIter == this->_elements.end()) {
						mapIter = this->_elements.insert(this->_elements.end(), std::make_pair(name, startStr.str()));
						valIter = this->_values.insert(this->_values.end(), std::make_pair(name, std::vector<double>(1,start) ) );

						// If debug, how what is being inserted
						if (this->_debug) std::cout << "\tInserting " << name << ": " << startStr.str() << ".\n";
					}
					// Otherwise, add the schedule value onto the schedule string
					else {
						// Append the new schedule item
						std::string schedule = (*mapIter).second;
						schedule.append(",");
						schedule.append(startStr.str());
						// Update the schedule string value of the element
						(*mapIter).second = schedule;
						// If debug, show update
						if (this->_debug) std::cout << "\tUpdating " << name << ": " << startStr.str() << ".\n";
						valIter->second.push_back( start );
					}
				}
				// Error Case - error in parsing the schedule line
				else {
					std::string err( "Failed to parse schedule line: " );
					err += *lineIter;
					throw SchedResultsException( err );
				}
			}
		}
	}
	// We are done here
	return true;
}


/***************************************************************************************/


SchedResults::SchedResults(const std::string &resultFile, const std::string &esmolModel, 
						   const std::string &abstractModel, const bool &debug)
						 :	_debug(debug), _hyperperiodStr(), _hyperperiodSec(), _elements(), _harewareElements() {
	// Override _debug if _DEBUG is defined
#ifdef _DEBUG
	this->_debug = true;
#endif
	// Parse the results file
	this->ParseResults( resultFile );
	// If an ESMoL model is present, inject results
	if ( esmolModel.compare("") != 0 ) {
		this->PopulateEsmol( esmolModel );
	}

	// If an ESMoL_Abstract model is present, inject results
	if ( abstractModel.compare( "") != 0 ) {
		this->PopulateAbstract( abstractModel );
	}
	return;
}


/***************************************************************************************/

