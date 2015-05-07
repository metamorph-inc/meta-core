#ifndef _CONFIG_KEEPER_S2_H
#define _CONFIG_KEEPER_S2_H

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

class ConfigKeeperS2 {
public:
	static ConfigKeeperS2 & inst() {
		static ConfigKeeperS2 configKeeperS2;
		return configKeeperS2;
	}

	virtual ~ConfigKeeperS2(void) {}

	bool processCmdLine( int argc, char **argv);

	// Accessors!
	const std::vector<std::string> getTranslators() { return _trnList; }
	bool getList() { return (_vm.count("list") > 0); }
	std::string getInfileName() { return _vm["input-file"].as<std::string>(); }
	std::string getOutfileName() { return _vm["output-file"].as<std::string>(); }
	std::string getResolution() { return _vm["res"].as<std::string>(); }

private:
	ConfigKeeperS2(void);

protected:
	boost::program_options::options_description _desc;
	boost::program_options::positional_options_description _popt_desc;

	std::vector<std::string> _trnList;

	boost::program_options::variables_map _vm;
};

#endif // _CONFIG_KEEPER_S2_H