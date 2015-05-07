/*** Included Header Files ***/
#include "ConfigKeeper.h"


/*** Namespace Inclusions ***/
namespace po = boost::program_options;


/***************************************************************************************/


ConfigKeeper::ConfigKeeper() : _desc( "Allowed options" ), _debug( false ) {
	this->_desc.add_options()
		( "help,h", "Usage information.")
		( "input-file,f", po::value< std::string >()->default_value( std::string( "model.xml" ) ), "Input file." )
		( "output-file,o", po::value< std::string >(), "Output filename base." )
		( "context,c", "Show translation context trace." )
		( "debug,d", "Show some debugging information." )
		( "graph", "Generate the DataFlow graph of the model." )
	;
	this->_popt_desc.add( "output-file", 1 );
}


bool ConfigKeeper::ProcessCmdLine( int argc, char **argv ) {
	// Make sure to catch any exceptions
	try {
		po::store(  po::command_line_parser( argc, argv ).options( this->_desc ).positional( this->_popt_desc ).run(), this->_vm  );
		po::notify( this->_vm );
	}
	// And catch...
	catch (...) {
		std::cout << "Invalid command line format.\n";
		std::cout << this->_desc << std::endl;
		return false;
	}
	// Show help info if help was optioned
	if ( this->_vm.count( "help" ) ) {
		std::cout << _desc << std::endl;
		return false;
	}
	// Make sure an output file is specified
	if ( this->_vm.count( "output-file" ) == 0 ) {
		std::cout << "No output file specified." << std::endl;
		return false;
	}
	// See if we want debugging info
	if ( this->_vm.count( "debug" ) != 0 ) {
		this->_debug = true;
	}
	// See if we want to generate a graph
	if ( this->_vm.count( "graph" ) != 0 ) {
		this->_generateGraph = true;
	}
#ifdef _DEBUG
	this->_debug = true;
#endif
	return true;
}


/***************************************************************************************/

