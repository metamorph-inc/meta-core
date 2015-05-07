#include "ConfigKeeper.h"

#include <iostream>

using namespace std;
namespace po = boost::program_options;

// Singleton stuff
ConfigKeeper * ConfigKeeper::_instance = 0;
ConfigKeeper * ConfigKeeper::inst()
{
	if (_instance == 0)
	{
		_instance = new ConfigKeeper;
	}
	return _instance;
}

ConfigKeeper::ConfigKeeper(void) : _desc("Allowed options")
{
	_desc.add_options()
		("help,h", "Usage information.")
		("verbose,v", po::value< bool >()->default_value(true), "Show detailed information.")
		("debug,d", po::value< vector<string> >(&_dbgList), "Entities (procs/buses) to debug.")
		("input-file,f", po::value<string>()->default_value("test.scs"), "Input filename.")
		("output-file,o", po::value<string>()->default_value("times.rslt"), "Output filename.")
	;

	_popt_desc.add("output-file", 1);
}

bool ConfigKeeper::processCmdLine( int argc, char **argv)
{
	po::store(po::command_line_parser(argc, argv).options(_desc).positional(_popt_desc).run(), _vm);
	po::notify(_vm);

	if ( _vm.count("help"))
	{
		cout << _desc << endl;
		return false;
	}

	if ( _vm.count("output-file") == 0)
	{
		cout << "No output file specified." << endl;
		return false;
	}

	_schedModel = string("Default");
	if ( _vm.count("debug") > 0 )
	{
		_schedModel = string("Debug");
		if (_dbgList.size() == 0)
		{
			_dbgList.push_back( string("all") ); // Handle everything if -d is alone
		}
	}

	return true;
}

