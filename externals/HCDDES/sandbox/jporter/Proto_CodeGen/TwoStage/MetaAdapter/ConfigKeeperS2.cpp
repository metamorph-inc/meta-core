#include "ConfigKeeperS2.h"

#include <iostream>

using namespace std;
namespace po = boost::program_options;

// Singleton stuff
ConfigKeeperS2::ConfigKeeperS2(void) : _desc("Allowed options")
{
	_desc.add_options()
		("help,h", "Usage information.")
		("res,r", po::value<string>()->default_value(string("5us")), "Scheduler resolution." )
		("input-file,f", po::value<string>()->default_value(string("model.xml")), "Input file." )
		("output-file,o", po::value<string>(), "Output filename base.")
		("translate,t", po::value< vector<string> >(&_trnList), "Translator(s) to use.")
		("list,l", "List available translators (and exit).")
	;

	_popt_desc.add("output-file", 1);
}

bool ConfigKeeperS2::processCmdLine( int argc, char **argv)
{
	po::store(po::command_line_parser(argc, argv).options(_desc).positional(_popt_desc).run(), _vm);
	po::notify(_vm);

	if ( _vm.count("help"))
	{
		cout << _desc << endl;
		return false;
	}

	if ( _vm.count("list"))
	{
		return false;
	}

	if ( _vm.count("output-file") == 0)
	{
		cout << "No output file specified." << endl;
		return false;
	}

	if ( _trnList.size() == 0 )
	{
		cout << "No translators selected." << endl;
		return false;
	}

	return true;
}

