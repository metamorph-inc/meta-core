/*** Included Header Files ***/
#include "ConfigKeeper.h"

#include <cstdlib>

/*** Namespace Inclusions ***/
namespace po = boost::program_options;


/***************************************************************************************/
using namespace std;

ConfigKeeper::ConfigKeeper() : _desc( "Allowed options" ) {

	this->_desc.add_options()
		( "help,h", "Usage information.")
		( "input-file,f", po::value< std::string >()->default_value( std::string( "model.xml" ) ), "Input file." )
		( "output-file,o", po::value< std::string >(), "Output filename base." )
		( "context,c", "Show translation context trace." )
		( "lang,l", po::value< std::string >()->default_value(std::string("c")), "Select language (c,c++)." )
		( "paradigm,p", po::value< std::string >()->default_value(std::string("BIP")), "Select paradigm (BIP, ESMoL)." )
	;
	this->_popt_desc.add( "output-file", 1 );
}


bool ConfigKeeper::ProcessCmdLine( int argc, char **argv ) {

	po::store(  po::command_line_parser( argc, argv ).options( this->_desc ).positional( this->_popt_desc ).run(), this->_vm  );
	po::notify( this->_vm );
	// Show help info if help was optioned
	if ( this->_vm.count( "help" ) ) {
		std::cout << _desc << std::endl;
		return false;
	}
	// Make sure an output file is specified
	if ( this->_vm.count( "output-file" ) == 0 ) {
		std::cout << "No output filename base specified." << std::endl;
		return false;
	}

	// Check valid values for lang
	string outlang = GetOutputLanguage();
	if ( ( outlang != string( "c" ) ) && ( outlang != string( "c++" ) ) )
	{
		std::cout << "Please specify c or c++ as the output language (-l <lang>).  The default is c." << std::endl;
		return false;
	}

	// Check valid values for paradigm
	string par = GetParadigm();
	if ( ( par != string( "BIP" ) ) && ( par != string( "ESMoL" ) ) )
	{
		std::cout << "Please specify ESMoL or BIP as the input modeling language (-p <paradigm>).  The default is BIP." << std::endl;
		return false;
	}

	if ( getenv( "VCP_PATH" ) )
	{
		_template_path = std::string( getenv( "VCP_PATH" ) );
		_template_path = _template_path + std::string( "\\templates" );
	}
	else 
	{
		_template_path = std::string( "..\\..\\templates" );
	}

	_path_mgr = new PathMgr( GetOutputLanguage(), GetOutfileName(), GetTemplatePath() );

	return true;
}


/***************************************************************************************/

