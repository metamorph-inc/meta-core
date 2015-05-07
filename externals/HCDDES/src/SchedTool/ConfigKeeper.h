#ifndef _CONFIG_KEEPER_H
#define _CONFIG_KEEPER_H

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

class ConfigKeeper
{
public:
	static ConfigKeeper * inst();
	virtual ~ConfigKeeper(void) {}

	bool processCmdLine( int argc, char **argv);

	// Accessors!
	bool getVerbosity() { return _vm["verbose"].as< bool >(); }
	const std::vector<std::string> getDebugOptions() { return _dbgList; }
	std::string getSchedModel() { return  _schedModel; }
	std::string getInFileName() { return _vm["input-file"].as<std::string>(); }
	std::string getOutfileName() { return _vm["output-file"].as<std::string>(); }
	double getOutputScaleFactor() { return scale_factor; }
	std::string getOutputUnits() { return _vm["output-units"].as<std::string>(); }
	std::string getBranching() { return _vm["branch-order"].as<std::string>(); }
	bool getGist() { return _vm["gist"].as< bool >(); }

private:
	ConfigKeeper(void);
	static ConfigKeeper * _instance;

protected:
	boost::program_options::options_description _desc;
	boost::program_options::positional_options_description _popt_desc;
	boost::program_options::variables_map _vm;

	std::vector< std::string > _dbgList;
	std::string _schedModel;
	double scale_factor;

};

#endif // _CONFIG_KEEPER_H
