#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

#include "RegexCommon.hpp"
#include "MFileManager.hpp"
#include "UdmStatementBuilder.hpp"
#include "mfile2SFC.hpp"
#include "SFCSymbolTable.hpp"
#include "SFCUdmEngine.hpp"
#include "SFCHelper.hpp"


MFileManager::MFile::ArgumentNameVector MFileManager::MFile::getIdentifiers( std::string input ) {

	ArgumentNameVector ArgumentNameVector;

	boost::match_results<std::string::const_iterator> results;
	while(  regex_search( input, results, RegexCommon::getIdentifierRegex() )  ) {
		ArgumentNameVector.push_back(  std::string( results[0].first, results[0].second )  );
		input = std::string( results.suffix().first, results.suffix().second );
	}

	return ArgumentNameVector;
}


bool MFileManager::MFile::loadMFileFunction( void ) {

	if ( _loaded ) return true;

	std::string mFileContents;
	char buffer[ 1024 ];
	ifstream mFileInput( _mFileName.c_str() );
	while(  mFileInput.read( buffer, 1024 )  ) mFileContents.append( buffer, 1024 );
	mFileContents.append( buffer, (unsigned int) mFileInput.gcount() );
	mFileInput.close();

	mFileContents = RegexCommon::eliminateContinuations( mFileContents );

	boost::match_results<std::string::const_iterator> results;
	if (  !regex_search( mFileContents, results, RegexCommon::getFunctionHeaderRegex() )  ) {
		std::cerr << "ERROR:  COULD NOT FIND FUNCTION HEADER IN \"" << _mFileName << "\" M-FILE" << std::endl;
		std::cerr << "Contents:" << std::endl;
		std::cerr << mFileContents << std::endl;
		return false;
	}

	std::string inputArguments( results[2].first, results[2].second );
	_inputArgumentNameVector = getIdentifiers( inputArguments );

	std::string outputArguments( results[1].first, results[1].second );
	if ( outputArguments[0] == '[' ) _outputArgumentNameVector = getIdentifiers(  outputArguments.substr( 1, outputArguments.size() - 2 )  );
	else                             _outputArgumentNameVector.push_back( outputArguments );

	_scriptBody = std::string( results.suffix().first, results.suffix().second );

	_loaded = true;

	return true;
}

bool MFileManager::MFile::checkInputDTVector( const DTVector &inputDTVector ) {

	if ( inputDTVector.size() != _inputArgumentNameVector.size() ) {
		std::cerr << "ERROR:  NUMBER OF INPUT TYPES (" << inputDTVector.size() << ") FOR MFILE FUNCTION \"" << _mFileFunctionName <<
		 "\" DOES NOT MATCH NUMBER OF INPUT ARGUMENTS (" << _inputArgumentNameVector.size() << ")" << std::endl;
		return false;
	}

	for( DTVector::const_iterator dtvItr = inputDTVector.begin() ; dtvItr != inputDTVector.end() ; ++dtvItr ) {
		if ( *dtvItr == Udm::null ) {
			std::cerr << "ERROR:  ALL INPUT TYPES OF MFILE-FUNCTION MUST BE SPECIFIED (I.E. NON-NULL)" << std::endl;
			return false;
		}
	}

	return true;
}

SFC::Function MFileManager::MFile::createFunction( const DTVector &inputDTVector, const DTVector &outputDTVector ) {

	getSFCSymbolTable().pushCleanScope();

	SFC::Function sfcFunction = SFC::Function::Create( SFCUdmEngine::get_singleton().getGlobalFunctionsClass() );
	SFCManager::numberStatement( sfcFunction );
	sfcFunction.name() = _mFileFunctionName + "_" + boost::lexical_cast< std::string >( _instanceNumber++ );
	sfcFunction.dt() = SFCTypesManager::getSingleton().getBasicType( "void" );

	int argNumber = 0;
	ArgumentNameVector::iterator anvItr = _inputArgumentNameVector.begin();
	for( DTVector::const_iterator dtvItr = inputDTVector.begin() ; dtvItr != inputDTVector.end() ; ++dtvItr, ++anvItr ) {
		SFC::Arg arg = SFC::Arg::Create( sfcFunction );
		arg.name() = *anvItr;
		arg.argIndex() = argNumber++;
		arg.dt() = *dtvItr;
		getSFCSymbolTable().push( *anvItr, arg );
	}

	anvItr = _outputArgumentNameVector.begin();
	for( DTVector::const_iterator dtvItr = outputDTVector.begin() ; dtvItr != outputDTVector.end() ; ++dtvItr, ++anvItr ) {
		SFC::Arg arg = SFC::Arg::Create( sfcFunction );
		arg.name() = *anvItr;
		arg.argIndex() = argNumber++;
		arg.dt() = *dtvItr;
		arg.ptr() = true;
		getSFCSymbolTable().push( *anvItr, arg );
	}

	mstat2SFC( sfcFunction, _scriptBody );

	getSFCSymbolTable().popCleanScope();

	return sfcFunction;
}

MFileManager::DTVector MFileManager::MFile::getFunctionArgDTVector( SFC::Function sfcFunction ) {

	DTVector dtVector;

	SFC::ArgSet argSet = sfcFunction.Arg_children_sorted( SFC::ArgSorter() );
	for( SFC::ArgSet::iterator agsItr = argSet.begin() ; agsItr != argSet.end() ; ++agsItr ) {
		SFC::Arg arg = *agsItr;
		dtVector.push_back( arg.dt() );
	}

	return dtVector;
}

SFC::Function MFileManager::MFile::getFunction( const DTVector &inputDTVector ) {

	OverloadMap::const_iterator olmItr = _primaryOverloadMap.find( inputDTVector );
	if ( olmItr != _primaryOverloadMap.end() ) {
		return olmItr->second;
	}

	if ( !loadMFileFunction() ) return SFC::Function();

	DTVector outputDTVector( _outputArgumentNameVector.size(), SFC::DT() );

	SFC::Function sfcFunction = getFunction( inputDTVector, outputDTVector );
	_primaryOverloadMap.insert(  std::make_pair( inputDTVector, sfcFunction )  );

	return sfcFunction;


	// THE CODE BELOW WILL MAKE A "WRAPPER" FUNCTION AROUND THE M-FILE FUNCTION WITH OUTPUT ARGUMENTS.
	// THIS WRAPPER FUNCTION WILL RETURN A VALUE (A STRUCT FOR MULTIPLE VALUES) AS ITS RETURN VALUE RATHER THAN
	// OUTPUT ARGUMENTS.
	// CURRENTLY THIS IS NOT USED, SO THE CODE IS COMMENTED OUT.

	//if ( sfcFunction == Udm::null ) return sfcFunction;

	//SFC::Function primarySFCFunction = SFC::Function::Create( SFCUdmEngine::get_singleton().getGlobalFunctionsClass() );
	//SFCManager::numberStatement( primarySFCFunction );
	//primarySFCFunction.name() = _mFileFunctionName + "_" + boost::lexical_cast< std::string >( _instanceNumber++ );

	//ArgSet argSet = sfcFunction.Arg_children_sorted( ArgSorter() );
	//ArgSet::iterator agsItr = argSet.begin();
	//ArgVector argVector;
	//for( int ix = 0 ; ix < inputDTVector.size() ; ++ix ) {
	//	SFC::Arg arg = *agsItr;
	//	SFC::Arg primaryArg = SFC::Arg::Create( primarySFCFunction );
	//	primaryArg.name() = arg.name();
	//	primaryArg.dt() = arg.dt();
	//	primaryArg.argIndex() = arg.argIndex();
	//	argVector.push_back( primaryArg );
	//	++agsItr;
	//}


	//DTVector newOutputDTVector;
	//for( ; agsItr != argSet.end() ; ++agsItr ) {
	//	newOutputDTVector.push_back( agsItr->dt() );
	//}

	//SFC::DT dt;
	//if ( newOutputDTVector.size() == 1 ) {
	//	dt = newOutputDTVector[0];
	//} else {
	//	SFC::Struct sfcStruct = SFCTypesManager::getSingleton().getStruct();
	//	int ix = 0;
	//	for( DTVector::iterator dtvItr = newOutputDTVector.begin() ; dtvItr != newOutputDTVector.end() ; ++dtvItr, ++ix ) {
	//		SFCTypesManager::createMember( sfcStruct, "signal" + boost::lexical_cast< std::string >( ix ), *dtvItr );
	//	}
	//	SFCTypesManager::getSingleton().makeTypeUnique( sfcStruct );
	//	dt = sfcStruct;
	//}

	//primarySFCFunction.dt() = dt;
	//SFC::LocalVar localVar = SFCManager::createLocalVar( primarySFCFunction, "aux", dt );
	//VE ve( localVar );

	//FE fe( primarySFCFunction );
	//for( ArgVector::iterator agvItr = argVector.begin() ; agvItr != argVector.end() ; ++agvItr ) {
	//	fe.addArgVal(  VE( *agvItr )  );
	//}

	//if ( newOutputDTVector.size() == 1 ) {
	//	fe.addArgVal( ve );
	//} else {
	//	SFC::Struct sfcStruct = SFC::Struct::Cast( dt );
	//	LocalVarSet localVarSet = sfcStruct.LocalVar_kind_children_sorted( StatementSorter() );
	//	for( LocalVarSet::iterator lvsItr = localVarSet.begin() ; lvsItr != localVarSet.end() ; ++lvsItr ) {
	//		fe.addArgVal(   BE(  ve, ".", VE( *lvsItr )  )   );
	//	}
	//}

	//fe.buildUdm( primarySFCFunction, SFC::CompoundStatement::meta_stmnt );

	//RS rs( ve );
	//rs.buildUdm( primarySFCFunction, SFC::CompoundStatement::meta_stmnt );

	//_primaryOverloadMap.insert(  std::make_pair( inputDTVector, primarySFCFunction )  );

	//return primarySFCFunction;
}

SFC::Function MFileManager::MFile::getFunction( const DTVector &inputDTVector, const DTVector &outputDTVector ) {

	if (  !loadMFileFunction() || !checkInputDTVector( inputDTVector )  ) return SFC::Function();

	if ( outputDTVector.size() != _outputArgumentNameVector.size() ) {
		std::cerr << "ERROR:  NUMBER OF OUTPUT TYPES (" << outputDTVector.size() << ") FOR MFILE FUNCTION \"" << _mFileFunctionName <<
		 "\" DOES NOT MATCH NUMBER OF OUTPUT ARGUMENTS (" << _outputArgumentNameVector.size() << ")" << std::endl;
		return SFC::Function();
	}

	DTVector dtVector( inputDTVector );
	dtVector.insert( dtVector.end(), outputDTVector.begin(), outputDTVector.end() );
	OverloadMap::const_iterator olmItr = _overloadMap.find( dtVector );
	if ( olmItr != _overloadMap.end() ) {
		return olmItr->second;
	}

	SFC::Function sfcFunction = createFunction( inputDTVector, outputDTVector );

	if ( sfcFunction == Udm::null ) return sfcFunction;

	DTVector completeDTVector = getFunctionArgDTVector( sfcFunction );

	olmItr = _overloadMap.find( completeDTVector );
	if ( olmItr != _overloadMap.end() ) {
		sfcFunction.DeleteObject();
		sfcFunction = olmItr->second;
	} else {
		_overloadMap.insert(  std::make_pair( completeDTVector, sfcFunction )  );
	}

	_overloadMap.insert(  std::make_pair( dtVector, sfcFunction )  );

	return sfcFunction;

}

bool MFileManager::DTVectorComparator::operator()( const DTVector &dtv1, const DTVector &dtv2 ) const {
	DTVector::const_iterator dtvItr1 = dtv1.begin();
	DTVector::const_iterator dtvItr2 = dtv2.begin();

	while( dtvItr1 != dtv1.end() && dtvItr2 != dtv2.end() ) {
		const SFC::DT &dt1 = *dtvItr1;
		const SFC::DT &dt2 = *dtvItr2;

		if ( dt1 == Udm::null ) {
			if ( dt2 != Udm::null ) return true;
		} else {
			if ( dt2 == Udm::null ) return false;
			if ( dt1.uniqueId() < dt2.uniqueId() ) return true;
			if ( dt1.uniqueId() > dt2.uniqueId() ) return false;
		}
		++dtvItr1; ++dtvItr2;
	}
	if ( dtvItr2 != dtv2.end() ) return true;

	return false;
}

void MFileManager::addDirectory( const std::string &directoryName ) {

	if ( directoryName == "." ) _currentDirectoryDefault = false;

	DirectoryMap::iterator drmItr = _directoryMap.find( directoryName );
	if ( drmItr != _directoryMap.end() ) {
		std::cerr << "WARNING: Directory \"" << directoryName << "\" already registered -- moving to last directory checked." << std::endl;
		for( DirectoryList::iterator drlItr = _directoryList.begin() ; drlItr != _directoryList.end() ; ++drlItr ) {
			if ( *drlItr == directoryName ) {
				_directoryList.splice( _directoryList.end(), _directoryList, drlItr );
				break;
			}
		}
		return;
	}

	if ( _currentDirectoryDefault && _directoryList.back() == "." ) _directoryList.pop_back();
	_directoryList.push_back( directoryName );
	if ( _currentDirectoryDefault ) _directoryList.push_back( "." );
	_directoryMap.insert(  std::make_pair( directoryName, MFileNameVector() )  );
}

void MFileManager::deleteDirectory( const std::string &directoryName ) {

	if ( directoryName == "." ) _currentDirectoryDefault = false;

	DirectoryMap::iterator drmItr = _directoryMap.find( directoryName );
	if ( drmItr == _directoryMap.end() ) {
		std::cerr << "WARNING: Attempt to delete unregistered directory \"" << directoryName << "\".  Ignored." << std::endl;
		return;
	}

	MFileNameVector _mFileNameVector = drmItr->second;
	for( MFileNameVector::iterator mnvItr = _mFileNameVector.begin() ; mnvItr != _mFileNameVector.end() ; ++mnvItr ) {
		std::string mFileName = *mnvItr;
		MFileNameMap::iterator mnmItr = _mFileNameMap.find( mFileName );
		if ( mnmItr == _mFileNameMap.end() ) {
			std::cerr << "WARNING:  FOR \"" << directoryName << "\" directory, \"" << mFileName << "\" has no entry." << std::endl;
			continue;
		}
		_mFileNameMap.erase( mnmItr );
	}

	_directoryMap.erase( drmItr );

	for( DirectoryList::iterator drlItr = _directoryList.begin() ; drlItr != _directoryList.end() ; ++drlItr ) {
		for( DirectoryList::iterator drlItr = _directoryList.begin() ; drlItr != _directoryList.end() ; ++drlItr ) {
			if ( *drlItr == directoryName ) {
				_directoryList.erase( drlItr );
				break;
			}
		}
	}
}

bool MFileManager::hasMFileFunction( const std::string &mFileName ) {
	if ( _mFileNameMap.find( mFileName ) != _mFileNameMap.end() ) return true;

	boost::filesystem::path mFilePath;
	
	for( DirectoryList::iterator drlItr = _directoryList.begin() ; drlItr != _directoryList.end() ; ++drlItr ) {
		std::string directoryName = *drlItr;
		boost::filesystem::path directory( directoryName );

		mFilePath = directory / ( mFileName + ".m" );
		if (  boost::filesystem::exists( mFilePath )  ) {
			_mFileNameMap.insert(   std::make_pair(  mFileName, MFile( mFilePath.string(), mFileName )  )   );
			_directoryMap.find( directoryName )->second.push_back( mFileName );
			return true;
		}
	}

	return false;
}

SFC::Function MFileManager::getMFileFunction( const std::string &mFileName, DTVector &inputDTVector ) {

	// MFILE NOT FOUND
	if (  !hasMFileFunction( mFileName )  ) {
		std::cerr << "ERROR: COULD NOT FIND \"" << mFileName << "\" M-FILE" << std::endl;
		return SFC::Function();
	}

	MFileNameMap::iterator mnmItr = _mFileNameMap.find( mFileName );
	if ( mnmItr == _mFileNameMap.end() ) {
		std::cerr << "ERROR: MFILE \"" << mFileName << "\" SHOULD BE REGISTERED BUT ISN'T" << std::endl;
		return SFC::Function();
	}
	MFile &mFile = mnmItr->second;
	return mFile.getFunction( inputDTVector );
}

SFC::Function MFileManager::getMFileFunction( const std::string &mFileName, DTVector &inputDTVector, DTVector &outputDTVector ) {

	// MFILE NOT FOUND
	if (  !hasMFileFunction( mFileName )  ) {
		std::cerr << "ERROR: COULD NOT FIND \"" << mFileName << "\" M-FILE" << std::endl;
		return SFC::Function();
	}

	MFileNameMap::iterator mnmItr = _mFileNameMap.find( mFileName );
	if ( mnmItr == _mFileNameMap.end() ) {
		std::cerr << "ERROR: MFILE \"" << mFileName << "\" SHOULD BE REGISTERED BUT ISN'T" << std::endl;
		return SFC::Function();
	}
	MFile &mFile = mnmItr->second;
	return mFile.getFunction( inputDTVector, outputDTVector );
}
