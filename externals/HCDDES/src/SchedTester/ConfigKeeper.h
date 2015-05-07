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
	unsigned long getNetworks() { return _nets; }
	unsigned long getMaxNetsPerNode() { return std::min(_nets_per_node, _nets); }
	unsigned long getMaxTasksPerNode() { return _tasks_per_node; }
	double getSysPeriodSecs() { return _sysPeriodSecs; }
	double getMsgProb() { return _msg_prob; }
	unsigned long getMinMsgBytes() { return minMsgBytes; }
	unsigned long getMaxMsgBytes() { return _maxMsgBytes; }
	double getMinTaskUtil() { return _minTaskUtil; }
	double getMaxTaskUtil() { return _maxTaskUtil; }
	double getSysResolutionSecs() { return _sysResolutionSecs; }
	std::string getOutfileName() { return _vm["output-file"].as<std::string>(); }

private:
	ConfigKeeper(void);
	static ConfigKeeper * _instance;

protected:
	boost::program_options::options_description _desc;
	boost::program_options::positional_options_description _popt_desc;

	unsigned long _nodes;
	unsigned long _nets;
	unsigned long _nets_per_node;
	unsigned long _tasks_per_node;
	unsigned long minMsgBytes;
	unsigned long _maxMsgBytes;
	double _sysPeriodSecs;
	double _minTaskUtil;
	double _maxTaskUtil;
	double _sysResolutionSecs;
	double _msg_prob;

	boost::program_options::variables_map _vm;
};

#endif // _CONFIG_KEEPER_H