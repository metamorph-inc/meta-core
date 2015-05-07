/*
 *	Copyright (c) Vanderbilt University, 2006
 *	ALL RIGHTS RESERVED
 *
 *	Vanderbilt University disclaims all warranties with regard to this
 *	software, including all implied warranties of merchantability
 *	and fitness.  In no event shall Vanderbilt University be liable for
 *	any special, indirect or consequential damages or any damages
 *	whatsoever resulting from loss of use, data or profits, whether
 *	in an action of contract, negligence or other tortious action,
 *	arising out of or in connection with the use or performance of
 *	this software.
 *
 *  FILE NAME:	MatLabEngine.hpp 
 *	SYNOPSIS:	Definition of class "MatLabEngine", to communicate with
 *				and gather data from a MatLab server, currently on the local machine only.					
 *  AUTHOR:		Harmon Nine - hnine@isis.vanderbilt.edu
 *  CREATED:	07/20/06
 *  LAST UPDATE: 
 */
#ifndef _ISIS_MATLABENGINE
#define _ISIS_MATLABENGINE

#include <engine.h>
#include <vector>
#include <set>
#include <cstring>
#include <string>
#include <boost/version.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <fstream>
#include <ctype.h>


#define BUFSIZE 65536

#ifdef _WIN32
typedef int int_compat;
#include "Windows.h"
#undef min
#undef max
#else
typedef mwSize int_compat;
#endif

class MatLabEngine {

private:
	Engine *_ep;
	std::string _matLabOutput;

	std::ofstream *_matLabLogFileStreamPtr;

	typedef boost::shared_ptr< MatLabEngine > MatLabEngineSP;

	static char *getBuffer( void ) {
		static char buffer[ BUFSIZE + 1 ];
		return buffer;
	}

	static void zeroBuffer( void ) {
		memset( getBuffer(), 0, BUFSIZE + 1 );
	}

public:

	MatLabEngine( std::string host = "" );
	~MatLabEngine( void ) {
		engClose( _ep );
		_ep = 0;
		if ( _matLabLogFileStreamPtr ) {
			_matLabLogFileStreamPtr->close();
			delete _matLabLogFileStreamPtr;
			_matLabLogFileStreamPtr = 0;
		}
	}

	void setLoggingEnabled( const std::string &matLabLogFilePath ) {
		boost::filesystem::path matLabLogFile( matLabLogFilePath );
#if ( BOOST_VERSION / 100 % 1000 ) < 46
		matLabLogFile = boost::filesystem::complete( matLabLogFile );
#else
		matLabLogFile = boost::filesystem::absolute( matLabLogFile );
#endif

		if (  !boost::filesystem::exists( matLabLogFile ) || boost::filesystem::is_regular( matLabLogFile )  ) {
			_matLabLogFileStreamPtr = new std::ofstream( matLabLogFile.string().c_str() );
		} else {
			std::cerr << "WARNING:  Could not open matlab log file \"" << matLabLogFile.string() <<
			 "\":  exists and is not a regular file." << std::endl;
		}
	}

	std::string getMatLabOutput( void ) {
		return _matLabOutput;
	}

	void loadFile( const std::string &matLabFilename ) {
		executeCommand(  std::string( "load_system('" )  +  matLabFilename  +  "')"  );
	}

	void executeCommand( const std::string &command, const bool unchecked=false );

#if defined(LINUX) && defined(DEVEL)
	void executeInteractiveCommands( void );
#endif

	typedef std::vector<int> ArraySize;

private:

	static ArraySize getArraySize( const mwSize *mxSizeArray, mwSize noElements = 2 ) {

		ArraySize retval;
		bool hasZero = false;
		for( unsigned int ix = 0 ; ix < noElements ; ++ix ) {
			retval.push_back( (int) mxSizeArray[ ix ] );
			if ( retval.back() == 0 ) hasZero = true;
		}

		if ( hasZero ) {
			retval = ArraySize( retval.size(), 0 );
		}

		return retval;
	}


	static ArraySize getArraySize( mxArray *mxSizeArray ) {

		double *data = mxGetPr( mxSizeArray );

		ArraySize metaArraySize = getArraySize(  mxGetDimensions( mxSizeArray ), mxGetNumberOfDimensions( mxSizeArray )  );
		int noElements = 1;
		for( ArraySize::iterator arsItr = metaArraySize.begin() ; arsItr != metaArraySize.end() ; ++arsItr ) {
			noElements *= *arsItr;
		}

		ArraySize retval;
		bool hasZero = false;
		for( int ix = 0 ; ix < noElements ; ++ix ) {
			retval.push_back( (int) data[ ix ] );
			if ( retval.back() == 0 ) hasZero = true;
		}

		if ( hasZero ) {
			retval = ArraySize( retval.size(), 0 );
		}

		return retval;
	}

	static int getVectorSize( const mwSize *mxSizeArray, mwSize noElements = 2 ) {

		ArraySize arraySize = getArraySize( mxSizeArray, noElements );

		if ( arraySize.empty() ) return 0;

		ArraySize::iterator arsItr = arraySize.begin();
		int max = *arsItr;
		while( ++arsItr != arraySize.end() ) {
			if ( *arsItr > max ) max = *arsItr;
		}

		return max;
	}

	static int getVectorSize( mxArray *mxSizeArray ) {
		ArraySize arraySize = getArraySize( mxSizeArray );
		return std::max( arraySize[0], arraySize[1] );
	}

	static MatLabEngineSP &globalEngineInternal() {
		static MatLabEngineSP matLabEngineSP;
		return matLabEngineSP;
	}

	static int getUniqueNo( void ) {
		static int number = 0;
		return ++number;
	}

public:
	bool isProperty( const std::string &mxVarName, const std::string &fieldName );

	ArraySize getMxMatrixSize( const std::string &mxVarName, const bool unchecked=false );
	int getMxLinearSize( const std::string &mxVarName, const bool unchecked=false );

	std::string getMxStringValue( const std::string &mxVarName, const bool unchecked=false );
	std::string getMxStringValue( const std::string &mxStructName, const std::string &fieldName );
	std::string getMxStringValue( const std::string &mxStructName, const char *fieldName );

	void setMxStringValue( const std::string &mxStructName, const std::string &fieldName, const std::string &value );

	int getMxIntValue( const std::string &mxVarName, const bool unchecked=false );
	int getMxIntValue( const std::string &mxVarName, const std::string &fieldName );
	int getMxIntValue( const std::string &mxVarName, const char *fieldName );

	int getMxCharValue( const std::string &mxVarName );

	double getMxDoubleValue( const std::string &mxVarName, bool unchecked=false );
	double getMxDoubleValue( const std::string &mxVarName, const std::string &fieldName );
	double getMxDoubleValue( const std::string &mxVarName, const char *fieldName );

	std::vector< double > getMxDoubleVector( const std::string &mxVarName );
	std::set< std::string > getMxFieldNames( const std::string &mxVarName );
	std::string getData( const std::string &fullFieldName, bool encodeString = false );
	bool hasField( const std::string &matLabObject, const std::string &fieldName );

	std::string getUniqueVarName( const std::string &varName );
	std::string getUniqueVarName( void ) { return getUniqueVarName( "var" ); }
	bool varExists( const std::string &varName );
	void clearVar( const std::string &varName );

	static void initGlobalEngine( std::string host = "" ) {
		if ( globalEngineInternal() != 0 ) globalEngineInternal().reset();
		globalEngineInternal() = MatLabEngineSP(  new MatLabEngine( host )  );
	}

	static MatLabEngine &globalEngine() {
		if ( globalEngineInternal() == 0 ) initGlobalEngine();
		return *globalEngineInternal();
	}

	static boost::regex &matLabComment( void ) {
		static boost::regex matLabRemark( "/\\*.*?\\*/", boost::regex_constants::perl );
		return matLabRemark;
	}

};

#endif
