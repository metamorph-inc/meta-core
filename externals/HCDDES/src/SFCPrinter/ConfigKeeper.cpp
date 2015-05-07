/*** Included Header Files ***/
#include "ConfigKeeper.hpp"
#include <iostream>


/***************************************************************************************/


ConfigKeeper::ConfigKeeper( void ) : _desc( "Allowed options" ), _hidden( "Hidden options" ), _all( "All options" ) {
	// Add the command line switches
	_desc.add_options()
		( "help,h", "Usage information." )
		( "annotations,a", "generate java pathfinder annotations (for java code generation only)" )
		( "c,c", "Output code in the \"C\" language (default)" )
		( "java,j", "Output code in the \"Java\" language" )
		( "status,s", "(Java only) generate \"getStatus\" function, which returns a string containing all active states" )
	;
	// Add a hidden switch for input file
	_hidden.add_options()
		( "sfc-file,f", boost::program_options::value< std::string >(), "ESMoL model file (*.mga)." );
	// Add all the switches together
	_all.add( _desc ).add( _hidden );
	// Make sure that input file is first
	_posDesc.add( "sfc-file", 1 );
}


bool ConfigKeeper::processCommandLineArguments( int argc, char *argv[] ) {
	// Try to parse command line options
	try {
		boost::program_options::store(
			boost::program_options::command_line_parser( argc, argv ).options( _all ).positional( _posDesc ).run(), _variablesMap );
	}
	// Catch any exceptions
	catch (...) {
		std::cout << "Invalid command line format.\n";
		std::cout << this->_all << std::endl;
		return false;
	}
	// Update the variables map
	boost::program_options::notify( _variablesMap );
	return true;
}


/***************************************************************************************/

