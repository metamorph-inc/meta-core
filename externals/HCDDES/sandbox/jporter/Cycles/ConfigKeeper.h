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
	unsigned long getNodes() { return _nodes; }
	double getEdgeProb() { return _edge_prob; }
	std::string getOutfileName() { return _vm["output-file"].as<std::string>(); }

private:
	ConfigKeeper(void);
	static ConfigKeeper * _instance;

protected:
	boost::program_options::options_description _desc;
	boost::program_options::positional_options_description _popt_desc;

	unsigned long _nodes;
	double _edge_prob;

	boost::program_options::variables_map _vm;
};

#endif // _CONFIG_KEEPER_H