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

#include "PathMgr.h"

#include "utils.h"


/***************************************************************************************/


class ConfigKeeper {
private:
	// Deny access to default constructor
	ConfigKeeper( void );
protected:
	boost::program_options::options_description				_desc;
	boost::program_options::positional_options_description	_popt_desc;
	boost::program_options::variables_map					_vm;
	bool                                                    _ctxtEnableOverride;
	std::string												_template_path;
	PathMgr	*												_path_mgr;

public:
	// Constructors and Destructors
	static ConfigKeeper &Inst( void ) {
		static ConfigKeeper configKeeper;
		return configKeeper;
	}
	virtual ~ConfigKeeper( void )		{ }

	// Accessors!
	void EnableContext() { _ctxtEnableOverride = true; }
	void RestoreContextEnabling() { _ctxtEnableOverride = false; }
	bool GetContextEnabled( void )	{ return _ctxtEnableOverride || (this->_vm.count( "context" ) > 0); }
	std::string GetInfileName()		{ return _vm[ "input-file" ].as< std::string >(); }
	std::string GetOutfileName()	{ return _vm[ "output-file" ].as< std::string >(); }
	std::string GetOutputLanguage() { return _vm[ "lang" ].as< std::string >(); }
	std::string GetParadigm()		{ return _vm[ "paradigm" ].as< std::string >(); }
	std::string GetTemplatePath()   { return _template_path; }

	PathMgr & GetPathMgr() { return *_path_mgr; }

	bool ProcessCmdLine( int argc, char **argv );
};


/***************************************************************************************/


/* 
 * To use the context tracker with config keeper, do the following:
 *   1. Create a contextTracker object as a member of your traversal class:
 *   protected:
 *   	ContextTracker							_contextTracker;
 *   
 *   2. Before each call to Accept, put in a local Context object to
 *      record where you are:
 *      VarSet vSet = pnIter->Variable_children();
 *		for ( VarSet::iterator vIter = vSet.begin(); vIter != vSet.end(); vIter++ )
 *		{
 *			Context varCtxt( _contextTracker, (*vIter).name() );
 *			(*vIter).Accept(*this);
 *		}
 * 
 *   3. At the top of the traversal, turn on the context tracking:
 *      _contextTracker.showContext( ConfigKeeper::Inst().GetContextEnabled() );
 * 
 *   4. If the context flag is not set on the command line, no tracking is
 *      done at all.  To override this for a particular class, use this call:
 *      	_contextTracker.showContext();
 *      Make the call before traversal starts, and then use GetContext() to 
 *      find out where you are in the traversal.  This is great for traversals
 *      that do error checking, as you can also print the location of the error.
 */
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

