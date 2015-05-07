#include "ConfigKeeper.hpp"
#include <iostream>


/***************************************************************************************/


ConfigKeeper::ConfigKeeper( void ) : _desc( "Allowed options" ), _hidden( "Hidden options" ), _commandLine( "All options" ) {
	_desc.add_options()
		( "help", "Produce help message" )
		( "subsystem,s", boost::program_options::value<  std::vector< std::string >  >(), "Specify subsystem(s) to flatten" )
	 ;

	_env.add_options()
		( "vcp_path,v", boost::program_options::value< std::string >(), "HCDDES Home directory path" );

	_hidden.add_options()
		( "inputFile,f", boost::program_options::value< std::string >(), "Specify MDL input file for translation to MGA/XML" )
		( "outputFile,o", boost::program_options::value< std::string >(), "Specify XML/MGA file for output" );
	// Add all decriptors together
	_commandLine.add( _desc ).add( _hidden );
	// Add the ordering of the two hidden inputs
	_posDesc.add( "inputFile", 1 ).add( "outputFile", 1 );
}

bool ConfigKeeper::processCommandLineArguments( int argc, char *argv[] ) {
	// Try to parse command line options
	try {
		boost::program_options::store(
		 boost::program_options::command_line_parser( argc, argv ).options( _commandLine ).positional( _posDesc ).run(),
		 _variablesMap
		);
		boost::program_options::store(
		 boost::program_options::parse_environment( _env, mapEnv ),
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

