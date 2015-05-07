#ifndef _ISIS_MFILEMANAGER_
#define _ISIS_MFILEMANAGER_


#include <vector>
#include <map>
#include <set>
#include <list>
#include <string>

#include <fstream>
#include <boost/filesystem.hpp>

#include "SFC.h"

/* STRUCTURES/METHODS FOR LOOKING UP/PARSING EXTERNAL MATLAB FUNCTIONS */
class MFileManager {
public:
	typedef std::vector< SFC::DT > DTVector;

private:
	typedef std::vector< std::string > MFileNameVector;
	typedef std::map< std::string, MFileNameVector > DirectoryMap;
	typedef std::list< std::string > DirectoryList;

	struct DTVectorComparator {
		bool operator()( const DTVector &dtv1, const DTVector &dtv2 ) const;
	};

	class MFile {
	public:
		typedef std::map< DTVector, SFC::Function, DTVectorComparator > OverloadMap;
		typedef std::vector< std::string > ArgumentNameVector;

		struct StatementSorter {
			bool operator()( const SFC::Statement &s1, const SFC::Statement &s2 ) {
				return static_cast< int >( s1.statementIndex() ) < static_cast< int >( s2.statementIndex() );
			}
		};
		typedef std::set< SFC::LocalVar, StatementSorter > LocalVarSet;

	private:
		std::string _mFileName;
		bool _loaded;
		std::string _mFileFunctionName;
		int _instanceNumber;
		ArgumentNameVector _inputArgumentNameVector;
		ArgumentNameVector _outputArgumentNameVector;
		std::string _scriptBody;
		OverloadMap _primaryOverloadMap;
		OverloadMap _overloadMap;

		static ArgumentNameVector getIdentifiers( std::string input );
		bool loadMFileFunction( void );
		bool checkInputDTVector( const DTVector &inputDTVector );
		SFC::Function createFunction( const DTVector &inputDTVector, const DTVector &outputDTVector );
		static DTVector getFunctionArgDTVector( SFC::Function sfcFunction );

	public:
		MFile( const std::string &mFileName, const std::string &mFileFunctionName ) :
		 _mFileName( mFileName ), _loaded( false ), _mFileFunctionName( mFileFunctionName ), _instanceNumber( 0 ) { }

		SFC::Function getFunction( const DTVector &inputDTVector );
		SFC::Function getFunction( const DTVector &inputDTVector, const DTVector &outputDTVector );
	};

	typedef std::map< std::string, MFile > MFileNameMap;

	DirectoryMap _directoryMap;
	DirectoryList _directoryList;
	MFileNameMap _mFileNameMap;
	bool _currentDirectoryDefault;

public:
	MFileManager( void ) : _currentDirectoryDefault( true ) {
		_directoryMap.insert(  std::make_pair( ".", MFileNameVector() )  );
		_directoryList.push_back( "." );
	}
	void addDirectory( const std::string &directoryName );
	void deleteDirectory( const std::string &directoryName );

	SFC::Function getMFileFunction( const std::string &mFileName, DTVector &inputDTVector );
	SFC::Function getMFileFunction( const std::string &mFileName, DTVector &inputDTVector, DTVector &outputDTVector );

	bool hasMFileFunction( const std::string &mFileName );

	static MFileManager &get_singleton( void ) {
		static MFileManager mFileManager;
		return mFileManager;
	}

};

#endif