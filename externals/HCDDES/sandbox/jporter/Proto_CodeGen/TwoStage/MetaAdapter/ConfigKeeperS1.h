#ifndef _CONFIG_KEEPER_S1_H
#define _CONFIG_KEEPER_S1_H

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

class ConfigKeeperS1 {

protected:
	boost::program_options::options_description _desc;
	boost::program_options::positional_options_description _popt_desc;

	boost::program_options::variables_map _vm;

public:
	static ConfigKeeperS1 & inst( void ) {
		static ConfigKeeperS1 configKeeperS1;
		return configKeeperS1;
	}

	virtual ~ConfigKeeperS1( void ) {}

	bool processCmdLine( int argc, char **argv );

	// Accessors!
	bool getContextEnabled( void ) { return _vm.count( "context" ) > 0; }
	std::string getInfileName() { return _vm[ "input-file" ].as< std::string >(); }
	std::string getOutfileName() { return _vm[ "output-file" ].as< std::string >(); }

private:
	ConfigKeeperS1( void );

};

#endif // _CONFIG_KEEPER_S1_H