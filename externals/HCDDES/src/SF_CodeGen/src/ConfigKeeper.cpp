/*** Included Header Files***/
#include "ConfigKeeper.hpp"
#include <iostream>


/***************************************************************************************/


ConfigKeeper::ConfigKeeper( void ) : _desc( "Allowed options" ), _hidden( "Hidden options" ), _all( "All options" ) {
	// Primary arguments
	_desc.add_options()
		( "help", "produce help message" )
		( "annotations,a", "generate java pathfinder annotations (for java code generation only)" )
		( "c,c", "output code in the \"C\" language (default)" )
		( "java,j", "output code in the \"Java\" language" )
		( "libdir,L", boost::program_options::value<  std::vector< std::string >  >(), "Specify directory in which to search for m-files" )
		( "output-directory,p", boost::program_options::value< std::string >(), "specify output directory for models" )
		( "status,s", "(Java only) generate \"getStatus\" function, which returns a string containing all active states" )
	;
	_hidden.add_options()
		( "inputFile,f", boost::program_options::value< std::string >(), "specify input file for transform" );
	// Add all descriptors together
	_all.add( _desc ).add( _hidden );
	// Set position for the hidden argument
	_posDesc.add( "inputFile", 1 );
}


bool ConfigKeeper::processCommandLineArguments( int argc, char *argv[] ) {
	// Try to parse command line options
	try {
		boost::program_options::store(
			boost::program_options::command_line_parser( argc, argv ).options( _all ).positional( _posDesc ).run(),
			_variablesMap );
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

