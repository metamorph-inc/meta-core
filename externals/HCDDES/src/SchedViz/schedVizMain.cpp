/*** Included Header Files ***/
#include "schedViz.h"
#include <iostream>
#include <boost/program_options/cmdline.hpp>
#include <boost/program_options/environment_iterator.hpp>
#include <boost/program_options/eof_iterator.hpp>
#include <boost/program_options/errors.hpp>
#include <boost/program_options/option.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/positional_options.hpp>
#include <boost/program_options/value_semantic.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/version.hpp>


/*** Namespace Declaration ***/
namespace po = boost::program_options;


/*** Globally Define Variables ***/
std::string _abstractFile;
std::string _vizFile;
bool		_debugOutput;


/***************************************************************************************/


// Process the command line arguments
bool processCmdLine( int argc, char **argv ) {
	// Setup the options description
	po::options_description optionsDesc("Allowed Options");
	optionsDesc.add_options()
		( "help,h", "Usage information.")
		( "abstract-file,f", po::value<std::string>(&_abstractFile), "ESMoL_Abstract model file (*.xml).")
		( "visualization-file,o", po::value<std::string>(&_vizFile), "Schedule visualization filename (*.svg).")
		( "debug-output,d", "Show the debug output.")
	;

	// Setup positional options
	po::positional_options_description popt_desc;
	// Prepare and parse the command line
	po::variables_map vm;
	try {
		po::store( po::command_line_parser( argc, argv ).options(optionsDesc).positional(popt_desc ).run(), vm );
	}
	catch (...) {
		std::cout << "Invalid command line format.\n";
		std::cout << optionsDesc << std::endl;
		return false;
	}
	po::notify(vm);

	// See if help was asked for
	if (vm.count( "help" )) {
		std::cout << optionsDesc << std::endl;
		return false;
	}

	// Make sure there was a result file
	if (vm.count("visualization-file") == 0)
	{
		std::cout << "Output SVG file must be specified." << std::endl;
		return false;
	}
	// Make sure there is either an esmol or and abstract model file
	if ((vm.count("visualization-file") == 0) ||
		(vm.count("abstract-file") == 0)) {
			std::cout << "Either an outfile name and ESMoL_Abstract model file must be specified." << std::endl;
		return false;
	}
	// Set the option value for debug output
	_debugOutput = vm.count("debug-output") != 0;
#ifdef _DEBUG
	_debugOutput = true;
#endif
	// All set - return true
	return true;
}



int main( int argc, char **argv ) {
	// Process the command line
	if (!processCmdLine(argc, argv)) {
		exit(0);
	}
	// Now process the results file into the model file
	try {
		SchedViz sv( _abstractFile, _vizFile, _debugOutput);
	}
	// Catching here means a file was not able to be opened
	catch ( SchedVizException &ex ) {
		// Just exit the app for now
		exit(-1);
	}
	// Finish It Up
	return 0;
}


/***************************************************************************************/

