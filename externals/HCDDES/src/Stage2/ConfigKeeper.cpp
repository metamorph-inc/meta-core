/*** Included Header Files ***/
#include "ConfigKeeper.h"
#include <cstdlib>


/*** Namespace Definitions ***/
namespace po = boost::program_options;


/***************************************************************************************/


ConfigKeeper::ConfigKeeper(void) : _desc("Allowed options"), _debug(false) {
	_desc.add_options()
		("help,h", "Usage information.")
		("res,r", po::value< std::string >(), "Scheduler resolution." )
		("input-file,f", po::value< std::string >()->default_value( std::string("model.xml") ), "Input file." )
		("output-file,o", po::value< std::string >(), "Output filename base.")
		("directory-name,p", po::value< std::string >()->default_value( "." ), "Generated files directory")
		("translate,t", po::value< std::vector< std::string > >(&_trnList), "Translator(s) to use.")
		("meta-mode,m", "Do not use unless you know you need it!")
		("list,l", "List available translators (and exit).")
		("debug,d", "Print debugging information.")
	;
	this->_popt_desc.add("output-file", 1);
}


bool ConfigKeeper::ProcessCommandLine( int argc, char **argv ) {
	try {
		po::store( po::command_line_parser( argc, argv ).options( this->_desc ).positional( this->_popt_desc ).run(), this->_vm );
		po::notify( this->_vm );
	}
	catch (...) {
		std::cout << "Invalid command line format.\n";
		std::cout << this->_desc << std::endl;
		return false;
	}
	// If Help is specified, show description and exit
 	if ( this->_vm.count("help")) {
		std::cout << this->_desc << std::endl;
		return false;
	}
	// If list is specified, show the list of translators and exit
	if ( this->_vm.count("list")) {
		// ...
		return false;
	}
	// Make sure a translator is specified
	if ( this->_trnList.size() == 0 ) {
		std::cout << "No translators selected." << std::endl;
		return false;
	}
	// Make sure an output file is specified if -t Sched
	if ( this->_vm.count("output-file") == 0) {
		std::cout << "No output file specified." << std::endl;
		return false;
	}

	// For sched translator, recommend a resolution (important!)
	std::vector< std::string >::iterator pTrn;
	bool sched = false;
	for ( pTrn = this->_trnList.begin(); pTrn != this->_trnList.end(); pTrn++ )
	{
		if ( *pTrn == std::string( "Sched" ) )
		{
			if ( this->_vm.count("res") == 0 )
			{
				std::cout << "When using the Sched translator, please specify a resolution." << std::endl;
				std::cout << " Choose something appropriate to your platform (or use 5us if unsure)." << std::endl;
				return false;
			}
		}
	}
	// See if debug output was set
	if ( this->_vm.count("debug") != 0) {
		this->_debug = true;
	}
#ifdef _DEBUG
	this->_debug = true;
#endif

	// Issue a warning
	if ( this->_vm.count("meta-mode") != 0 ) {

		std::cout << "Warning: Using meta-mode, which assumes one Simulink signal and message per IChan/OChan." << std::endl;
		std::cout << " If you don't know what that means, then do not use meta-mode." << std::endl;
		_metamode = true;
	}
	else
	{
		_metamode = false;
	}
	//if ( getenv( "HCDDES_CONFIG_PATH" ) ) {
	//	_template_path = std::string( getenv( "HCDDES_CONFIG_PATH" ) ) + std::string( "\\templates" );
	//}
	//else
	if ( getenv( "VCP_PATH" ) ) {
		_template_path = std::string( getenv( "VCP_PATH" ) );
		_template_path = _template_path + std::string( "\\templates" );
	}
	else {
		_template_path = std::string( "..\\..\\templates" );
	}

	// All is good...
	return true;
}

