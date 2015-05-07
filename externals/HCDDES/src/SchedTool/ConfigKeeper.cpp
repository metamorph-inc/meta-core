#include "ConfigKeeper.h"

#include <iostream>

using namespace std;
namespace po = boost::program_options;


// Singleton stuff
ConfigKeeper * ConfigKeeper::_instance = 0;
ConfigKeeper * ConfigKeeper::inst() {
	if (_instance == 0) {
		_instance = new ConfigKeeper;
	}
	return _instance;
}


ConfigKeeper::ConfigKeeper(void) : _desc("Allowed options") {
	_desc.add_options()
		("help,h", "Usage information.")
		("verbose,v", po::value< bool >()->default_value(true), "Show detailed information.")
		("debug,d", po::value< vector<string> >(&_dbgList), "Entities (procs/buses) to debug.")
		("input-file,f", po::value<string>()->default_value("test.scs"), "Input filename.")
		("output-file,o", po::value<string>()->default_value("times.rslt"), "Output filename.")
		("output-units,u", po::value<string>()->default_value("ms"), "Output units (us,ms,s).")
		("branch-order,b", po::value<string>()->default_value("med"), "Branching (min=start search on the left,med=start in the middle)." )
		("gist", po::value< bool >()->default_value(false), "Visualize solutions using gist.")
	;
	_popt_desc.add("output-file", 1);
}


bool ConfigKeeper::processCmdLine( int argc, char **argv) {
	// Try to catch any exceptions
	try {
		po::store(po::command_line_parser(argc, argv).options(_desc).positional(_popt_desc).run(), _vm);
		po::notify(_vm);
	// And now the catch...
	} catch (...) {
		std::cout << "Invalid command line format.\n";
		std::cout << this->_desc << std::endl;
		return false;
	}
	if ( _vm.count("help") || argc <= 1) {
		cout << _desc << endl;
		return false;
	}
	if ( _vm.count("output-file") == 0) {
		cout << "No output file specified." << endl;
		return false;
	}

	if ( getBranching() != string("min") && getBranching() != string("med") ) {
		cout << "Branching spec must be either min or med." << endl;
		return false;
	}

	scale_factor = 0.0; // deliberately bad
	string unitstr( _vm["output-units"].as<std::string>() );
	if ( unitstr == string( "s" ) ) {
		scale_factor = 1.0;
	}
	else if ( unitstr == string( "ms" ) ) {
		scale_factor = 1.0e-3;
	}
	else if ( unitstr == string( "us" ) ) {
		scale_factor = 1.0e-6;
	}
	else {
		cout << "Invalid output time units specified: " << unitstr << " -- please choose one of (us,ms,s)." << endl;
		return false;
	}

	_schedModel = string("Default");
	if ( _vm.count("debug") > 0 ) {
		_schedModel = string("Debug");
		if (_dbgList.size() == 0) {
			_dbgList.push_back( string("all") ); // Handle everything if -d is alone
		}
	}

	return true;
}

