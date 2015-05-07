#ifndef __CONFIG_KEEPER_HPP__
#define __CONFIG_KEEPER_HPP__


/*** Included Header Files ***/
#include <boost/program_options.hpp>
#include <boost/lexical_cast.hpp>
#include <vector>
#include <set>
#include <ctype.h>
#include <tchar.h>


/***************************************************************************************/


class ConfigKeeper {
private:
	boost::program_options::options_description _desc;
	boost::program_options::options_description _hidden;
	boost::program_options::options_description _env;
	boost::program_options::options_description _commandLine;
	boost::program_options::positional_options_description _posDesc;

	boost::program_options::variables_map _variablesMap;
	ConfigKeeper( void );

	typedef std::set< std::string> EnviromentVarNameSet;

	class EnvironmentVarNameSetInit {
	private:
		EnviromentVarNameSet _environmentVarNameSet;

	public:
		EnvironmentVarNameSetInit( void ) {
			_environmentVarNameSet.insert( "DESERT_PATH" );
		}
		
		EnviromentVarNameSet &getEnvironmentVarNameSet( void ) {
			return _environmentVarNameSet;
		}
	};

	static EnviromentVarNameSet &getEnvironmentVarNameSet( void ) {
		static EnvironmentVarNameSetInit environmentVarNameSetInit;
		return environmentVarNameSetInit.getEnvironmentVarNameSet();
	}

	static std::string mapEnv( std::string str ) {
		if (  getEnvironmentVarNameSet().find( str ) == getEnvironmentVarNameSet().end() ) {
			return "";
		}

		std::transform( str.begin(), str.end(), str.begin(), tolower );
		return str;
	}

public:
	static ConfigKeeper &getSingleton( void ) {
		static ConfigKeeper configKeeper;
		return configKeeper;
	}

	bool processCommandLineArguments( int argc, TCHAR *argv[] );

	std::string getStringValue( const std::string &name ) { return _variablesMap[ name ].as< std::string >(); }
	std::vector< std::string > getStringVector( const std::string &name ) {
		return _variablesMap[ name ].as<  std::vector< std::string >  >();
	}
	int getCount( const std::string &name ) { return _variablesMap.count( name ); }
	boost::program_options::options_description getDescription( void ) { return _desc; }
};


/***************************************************************************************/


#endif //__CONFIG_KEEPER_HPP__

