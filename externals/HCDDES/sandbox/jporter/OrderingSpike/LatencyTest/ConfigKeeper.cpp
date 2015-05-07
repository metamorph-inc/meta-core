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
		("nodes", po::value<unsigned long>(&_nodes)->default_value(3), "Number of processing nodes in the system.")
		("networks", po::value<unsigned long>(&_nets)->default_value(1), "Number of networks in the system.")
		("nets-per-node", po::value<unsigned long>(&_nets_per_node)->default_value(2), "Max networks connected to a node.")
		("max-tasks", po::value<unsigned long>(&_tasks_per_node)->default_value(2), "Max tasks running on a node.")
		("period", po::value<double>(&_sysPeriodSecs)->default_value(0.02), "Fundamental period of tasks(secs).")
		("msg-prob", po::value<double>(&_msg_prob)->default_value(0.75), "Probability of creating a message between two tasks.")
		("min-msg", po::value<unsigned long>(&minMsgBytes)->default_value(1), "Min message length (bytes).")
		("max-msg", po::value<unsigned long>(&_maxMsgBytes)->default_value(1024), "Max message length (bytes).")
		("min-util", po::value<double>(&_minTaskUtil)->default_value(0.01), "Min task utilization (0.0,1.0).")
		("max-util", po::value<double>(&_maxTaskUtil)->default_value(0.5), "Max task utilization (0.0,1.0).")
		("sys-res", po::value<double>(&_sysResolutionSecs)->default_value(0.000001), "System resolution (secs).")
		("output-file", po::value<string>(), "Output filename.")
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

	return true;
}

