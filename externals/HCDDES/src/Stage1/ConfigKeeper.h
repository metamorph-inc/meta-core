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
private:
	// Deny access to default constructor
	ConfigKeeper( void );
protected:
	bool													_debug, _generateGraph;
	boost::program_options::options_description				_desc;
	boost::program_options::positional_options_description	_popt_desc;
	boost::program_options::variables_map					_vm;
	bool                                                    _ctxtEnableOverride;
public:
	// Constructors and Destructors
	static ConfigKeeper &Inst( void ) {
		static ConfigKeeper configKeeper;
		return configKeeper;
	}
	virtual ~ConfigKeeper( void )		{ }

	// Accessors!
	bool Debug(void)					{ return this->_debug; }
	bool GenerateGraph(void)			{ return this->_generateGraph; }
	void EnableContext() { _ctxtEnableOverride = true; }
	void RestoreContextEnabling() { _ctxtEnableOverride = false; }
	bool GetContextEnabled( void )		{ return _ctxtEnableOverride || (this->_vm.count( "context" ) > 0); }
	std::string GetInfileName()			{ return this->_vm[ "input-file" ].as< std::string >(); }
	std::string GetOutfileName()		{ return this->_vm[ "output-file" ].as< std::string >(); }
	bool ProcessCmdLine( int argc, char **argv );
};


// Helpful macro for controlling debug output
#define DEBUGOUT(str) { if (ConfigKeeper::Inst().Debug()) std::cout << str; }


/***************************************************************************************/


class Context {
private: 
	ContextTracker						*_contextTracker;
public:
	Context( ContextTracker &contextTracker, std::string contextName ) : _contextTracker( &contextTracker ) {
		if ( ConfigKeeper::Inst().GetContextEnabled() ) _contextTracker->Enter( contextName );
	}
	virtual ~Context() {
		if ( ConfigKeeper::Inst().GetContextEnabled() ) _contextTracker->Leave();
	}
};


/***************************************************************************************/


#endif // __CONFIGKEEPER_H__

