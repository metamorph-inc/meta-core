/*** Included Header Files ***/
#include "ConfigKeeper.hpp"
#include <iostream>


/***************************************************************************************/


ConfigKeeper::ConfigKeeper( void ) : _desc( "Allowed options" ), _hidden( "Hidden options" ), _all( "All options" ) {
	// Add the command line switches
	_desc.add_options()
		( "help,h", "Usage information." )
		( "output-directory,p", boost::program_options::value< std::string >(), "Output directory for models." )
		( "c,c", "Output code in the \"C\" language (default)" )
		( "java,j", "Output code in the \"Java\" language" )
		( "libdir,L", boost::program_options::value<  std::vector< std::string >  >(), "Specify directory in which to search for m-files" )
		( "annotations,a", "generate java pathfinder annotations (for java code generation only)" )
	;
	// Add a hidden switch for input file
	_hidden.add_options()
		( "slsf-file,f", boost::program_options::value< std::string >(), "SLSF model file (*.mga or *.xml)." );
	// Add all the switches together
	_all.add( _desc ).add( _hidden );
	// Make sure that input file is first
	_posDesc.add( "slsf-file", 1 );
}


bool ConfigKeeper::processCommandLineArguments( int argc, char *argv[] ) {
	// Try to parse command line options
	try {
		boost::program_options::store(
			boost::program_options::command_line_parser( argc, argv ).options( _all ).positional( _posDesc ).run(), _variablesMap );
	}
	// Catch any exceptions
	catch (boost::program_options::ambiguous_option& e) {
		std::cout << "Ambiguous option " + e.get_option_name() << std::endl;
		std::cout << this->_all << std::endl;
		return false;
	}
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

