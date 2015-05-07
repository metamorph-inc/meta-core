#include "schedResults.h"
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


namespace po = boost::program_options;


// Global Variables
std::string _resultFile;
std::string _esmolFile;
std::string _abstractFile;
bool		_debugOutput;


// Process the command line arguments
bool processCmdLine( int argc, char **argv )
{
	// Setup the options description
	po::options_description optionsDesc("Allowed Options");
	optionsDesc.add_options()
		( "help,h", "Usage information.")
		( "result-file,r", po::value<std::string>(&_resultFile)->default_value(std::string("result.rslt")), "Results file (*.rslt).")
		( "esmol-file,e", po::value<std::string>(&_esmolFile), "ESMoL model file (*.mga).")
		( "abstract-file,a", po::value<std::string>(&_abstractFile), "ESMoL_Abstract model file (*.xml).")
		( "debug-output,d", "Show the debug output.")
	;

	// Setup positional options
	po::positional_options_description popt_desc;
	// Prepare and parse the command line
	po::variables_map vm;
	try {
		po::store(po::command_line_parser( argc, argv ).options(optionsDesc).positional(popt_desc ).run(), vm );
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
	if (vm.count("result-file") == 0)
	{
		std::cout << "Result file must be specified." << std::endl;
		return false;
	}
	// Make sure there is either an esmol or and abstract model file
	if ((vm.count("esmol-file") == 0) &&
		(vm.count("abstract-file") == 0)) {
			std::cout << "Either an ESMoL and/or ESMoL_Abstract model file must be specified." << std::endl;
		return false;
	}
	// Set the option value for debug output
	_debugOutput = vm.count("debug-output") != 0;
	// All set - return true
	return true;
}



int main(int argc, char **argv)
{
	// Process the command line
	if (!processCmdLine(argc, argv)) {
		exit(0);
	}
	// Now process the results file into the model file
	SchedKeeper sk(_resultFile, _esmolFile, _abstractFile, _debugOutput);
//	sk.Dump();

	return 0;
}
