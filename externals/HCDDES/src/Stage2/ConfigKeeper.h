#ifndef __CONFIGKEEPER_H__
#define __CONFIGKEEPER_H__


/*** Included Header Files ***/
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
#include "utils.h"


/***************************************************************************************/


class ConfigKeeper {
protected:
	boost::program_options::options_description				_desc;
	boost::program_options::positional_options_description	_popt_desc;
	std::vector<std::string>								_trnList;
	boost::program_options::variables_map					_vm;
	std::string												_template_path;
	bool													_debug;
	bool													_metamode;
private:
	// Hidden default constructor
	ConfigKeeper(void);
public:
	// Constructors and Destructors
	static ConfigKeeper &inst() {
		static ConfigKeeper configKeeper;
		return configKeeper;
	}
	virtual ~ConfigKeeper() {}

	// Accessors
	bool ProcessCommandLine( int argc, char **argv );
	const std::vector< std::string > GetTranslators()	{ return this->_trnList; }
	bool GetList()					{ return (this->_vm.count("list") > 0); }
	std::string GetInfileName()		{ return this->_vm["input-file"].as<std::string>(); }
	std::string GetOutfileName()	{ return this->_vm["output-file"].as<std::string>(); }
	std::string GetDirectoryName()	{ return this->_vm["directory-name"].as<std::string>(); }
	std::string GetResolution()		{ return this->_vm["res"].as<std::string>(); }
	std::string GetTemplatePath()   { return _template_path; }
	bool Debug(void)				{ return this->_debug; }
	bool MetaMode(void)				{ return _metamode; }
};


#define DEBUGOUT(str) { if (ConfigKeeper::inst().Debug()) std::cout << str; }


/***************************************************************************************/


#endif //__CONFIGKEEPER_H__

