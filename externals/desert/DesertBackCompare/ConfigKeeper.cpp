#include "ConfigKeeper.hpp"
#include <iostream>


/***************************************************************************************/


ConfigKeeper::ConfigKeeper( void ) : _desc( "Allowed options" ), _hidden( "Hidden options" ), _commandLine( "All options" ) {
	_desc.add_options()
		( "help", "Produce help message" )
	 ;

	_hidden.add_options()
		( "compareFrom,f", boost::program_options::value< std::string >(), "Specify first input file for comparison to the second" )
		( "compareTo,t", boost::program_options::value< std::string >(), "Specify second input file for comparison to the first" );
	// Add all decriptors together
	_commandLine.add( _desc ).add( _hidden );
	// Add the ordering of the two hidden inputs
	_posDesc.add( "compareFrom", 1 ).add( "compareTo", 1 );
}

bool ConfigKeeper::processCommandLineArguments( int argc, TCHAR *argv[] ) {
	// Try to parse command line options
	try {
		boost::program_options::store(
		 boost::program_options::command_line_parser( argc, argv ).options( _commandLine ).positional( _posDesc ).run(),
		 _variablesMap
		);
	// Catch any exceptions
	} catch ( std::exception &e ) {
		std::cout << e.what() << std::endl;
		return false;
	} catch (...) {
		std::cout << "Invalid command line format.\n";
		std::cout << this->_commandLine << std::endl;
		return false;
	}
	// Update the variables map
	boost::program_options::notify( _variablesMap );
	return true;
}


/***************************************************************************************/

