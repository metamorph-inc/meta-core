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
		("nodes,n", po::value<unsigned long>(&_nodes)->default_value(3), "Number of nodes in the graph.")
		("edge-prob,e", po::value<double>(&_edge_prob)->default_value(0.75), "Edge creation probability [0.0,1.0].")
		("output-file,o", po::value<string>(), "Output filename.")
	;

	_popt_desc.add("output-file", 1);
}

bool ConfigKeeper::processCmdLine( int argc, char **argv)
{
	po::store(po::command_line_parser(argc, argv).options(_desc).positional(_popt_desc).run(), _vm);
	po::notify(_vm);

	if ( _vm.count("help") )
	{
		cout << _desc << endl;
		return false;
	}

	return true;
}

