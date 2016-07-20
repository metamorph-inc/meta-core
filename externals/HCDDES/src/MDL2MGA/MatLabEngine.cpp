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
 *  FILE NAME:	MatLabEngine.cpp 
 *	SYNOPSIS:	Implementation of class "MatLabEngine", to communicate with
 *				and gather data from a MatLab server, currently on the local machine only.
 *  AUTHOR:		Harmon Nine - hnine@isis.vanderbilt.edu
 *  CREATED:	07/20/06
 *  LAST UPDATE: 
 */

#if defined(LINUX) && defined(DEVEL)
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#endif

#include "MatLabEngine.hpp"
#include "MatLabUdm.hpp"


#ifdef LINUX
#include <unistd.h>
#define SLEEP usleep( 500000 )
#else
#include <windows.h>
#define SLEEP Sleep( 500 )
#endif


MatLabEngine::MatLabEngine( std::string host ) : _ep( 0 ), _matLabLogFileStreamPtr( 0 ) {
	static std::string startError = "Could not start MatLab Engine.\n"
	 "The most common reasons for a MatLab exception are as follows:\n\n"
	 "   1) MatLab isn't installed on this (the local) machine.  MatLab *MUST*\n"
	 "      be installed on the machine on which MDL2MGA is run.\n"
	 "      IF YOU ARE RUNNING 64-bit WINDOWS XP, YOU MUST INSTALL THE\n"
	 "      32-BIT VERSION OF MATLAB FOR MDL2MGA TO RUN.\n\n"
	 "   2) If MatLab is installed on this machine, it may be that the\n"
	 "      MatLab service is not registered.  To register the MatLab service,\n"
	 "      in a command-prompt type:\n"
	 "        matlab /regserver\n"
	 "      Once you have done this, try running MDL2MGA again.\n\n"
	 "   3) If MatLab is installed on this machine and registered as a server,\n"
	 "      it may be that MatLab is taking a long time to initialize.  So long,\n"
	 "      in fact, that MDL2MGA is timing out before MatLab is able to respond\n"
	 "      to it.  To test this, re-execute MDL2MGA at least 3 more times.\n"
	 "      If MDL2MGA still will not run, please contact your support center.\n\n";

//		int retcode;
//		if (   !(  _ep = engOpenSingleUse( "\0", 0, &retcode )  )   ) {
//			throw Exception( "Cannot start MatLab engine (retcode = " + retcode + ')' );
//		}
//		if (   !(  _ep = engOpen( "\0" )  )   ) {

#ifdef LINUX
	host += "\0";
	if (   !(  _ep = engOpen( host.c_str() )  )   ) {
		throw MatLabUdm::Exception( startError );
	}
#else // Windows
	int retcode;
		if (   !(  _ep = engOpenSingleUse( "\0", 0, &retcode )  )   ) {
			throw std::exception( "Cannot start MatLab engine (retcode = " + retcode + ')' );
		}
#endif
	
	getBuffer()[ BUFSIZE ] = '\0';
	engOutputBuffer( _ep, getBuffer(), BUFSIZE );
}

void MatLabEngine::executeCommand( const std::string &command, const bool unchecked ) {

	if ( _matLabLogFileStreamPtr ) {
		std::cerr << command << std::endl;
		(*_matLabLogFileStreamPtr) << "try\n\t" << command << "\nend" << std::endl;
	}

	if (  engEvalString( _ep, command.c_str() )  !=  0  ) {
		_ep = 0;
		throw MatLabUdm::Exception( "MatLab connection lost." );
	}

	_matLabOutput = getBuffer();
	if (  _matLabOutput.find( "???" ) == 0  ) {
#ifdef _DEBUG
	// KMS: is there a better way to check for an error???
		std::string error = "Executing MatLab command '";
		error += command + "' resulted in error '" + _matLabOutput;
		if (!unchecked) {
#ifdef _WIN32
			OutputDebugString(error.c_str());
			DebugBreak();
#endif
			throw MatLabUdm::Exception(error);
		} else {
#ifdef _WIN32
			OutputDebugString(error.c_str());
#endif
		}
#endif
	}

}

#if defined(LINUX) && defined(DEVEL)
void MatLabEngine::executeInteractiveCommands( void ) {

	std::string command;
	while( true ) {

		char *commandString = readline( ">> " );
		if ( !commandString ) {
			continue;
		}
		if ( *commandString ) {
			add_history( commandString );
		}
		command = commandString;
		free( commandString );

		if (  command.substr( 0, 4 ) == "quit"  ) {
			break;
		}

		zeroBuffer();

		if (  engEvalString( _ep, command.c_str() )  !=  0  ) {
			_ep = 0;
			throw MatLabUdm::Exception( "MatLab connection lost." );
		}
		std::string output( getBuffer() );
		output = output.substr( output.find( '\n' ) + 1 );
		std::cout << output << std::endl;
	}
}
#endif

bool MatLabEngine::isProperty( const std::string &mxVarName, const std::string &fieldName ) {
	int isValidProperty = getMxIntValue( mxVarName + ".isValidProperty( '" + fieldName + "' )" );
	int isEditableProperty = getMxIntValue( mxVarName + ".isEditableProperty( '" + fieldName + "' )" );
	int isReadonlyProperty = getMxIntValue( mxVarName + ".isReadonlyProperty( '" + fieldName + "' )" );
	return isValidProperty || ( isEditableProperty && !isReadonlyProperty ) || ( !isEditableProperty && isReadonlyProperty );

}

MatLabEngine::ArraySize MatLabEngine::getMxMatrixSize( const std::string &mxVarName, const bool unchecked ) {
	executeCommand( "ans = 0" );
	std::string command = std::string( "size( " ) + mxVarName + " )";
//std::cerr << "getMxLinearSize: \"" << command << "\"" << std::endl;
	executeCommand( command, unchecked );
	mxArray *mxSizeArray;
	if (   (  mxSizeArray = engGetVariable( _ep, "ans" )  )  ==  0   ) {
		throw MatLabUdm::Exception( std::string( "Could not get size of array " ) + mxVarName );
	}

	ArraySize retval = getArraySize( mxSizeArray );
	mxDestroyArray( mxSizeArray );

	return retval;
}

int MatLabEngine::getMxLinearSize( const std::string &mxVarName, const bool unchecked ) {
	executeCommand( "ans = 0" );
	std::string command = std::string( "size( " ) + mxVarName + " )";
//std::cerr << "getMxLinearSize: \"" << command << "\"" << std::endl;
	executeCommand( command, unchecked );
	mxArray *mxSizeArray;
	if (   (  mxSizeArray = engGetVariable( _ep, "ans" )  )  ==  0   ) {
		throw MatLabUdm::Exception( std::string( "Could not get size of array " ) + mxVarName );
	}

	int retval = getVectorSize( mxSizeArray );
	mxDestroyArray( mxSizeArray );

	return retval;
}

std::string MatLabEngine::getMxStringValue( const std::string &mxVarName, const bool unchecked ) {
	executeCommand( "ans = ''" );
//std::cerr << "getMxStringValue: \"" << mxVarName << "\"" << std::endl;
	executeCommand( mxVarName, unchecked );
	mxArray *mxStringArray;
	std::string retval("");

	if (   (  mxStringArray = engGetVariable( _ep, "ans" )  )  !=  0   ) {
		int stringLength = getVectorSize(  mxGetDimensions( mxStringArray )  );
#ifdef _WIN32
		wchar_t *stringValue  = static_cast< wchar_t * >(  mxGetData( mxStringArray )  );
#else
		short *stringValue  = static_cast< short * >(  mxGetData( mxStringArray )  );
#endif
		retval.assign( stringValue, stringValue + stringLength );
		mxDestroyArray( mxStringArray );
	}

	return retval;
}

std::string MatLabEngine::getMxStringValue( const std::string &mxVarName, const std::string &fieldName ) {
	return isProperty( mxVarName, fieldName ) ? getMxStringValue( mxVarName + "." + fieldName ) : "";
}

std::string MatLabEngine::getMxStringValue( const std::string &mxVarName, const char *fieldName ) {
	return getMxStringValue(  mxVarName, std::string( fieldName )  );
}

void MatLabEngine::setMxStringValue( const std::string &mxStructName, const std::string &fieldName, const std::string &value ) {
	if (  isProperty( mxStructName, fieldName )  ) {
		MatLabEngine::globalEngine().executeCommand( mxStructName + "." + fieldName + " = '" + value + "'" );
	}
}


int MatLabEngine::getMxIntValue( const std::string &mxVarName, const bool unchecked ) {
	executeCommand( "ans = 0" );
	executeCommand(  std::string( "double( " ) + mxVarName + " )", unchecked  ); // COERCE "ans" TO DOUBLE TYPE

	mxArray *mxIntArray;
	if (   (  mxIntArray = engGetVariable( _ep, "ans" )  )  ==  0   ) {
		throw MatLabUdm::Exception( std::string( "Could not get value of " ) + mxVarName );
	}
	double *dblPtr = mxGetPr( mxIntArray );
	int retval  = dblPtr == 0 ? 0 : static_cast< int >( dblPtr[0] );
	mxDestroyArray( mxIntArray );

	return retval;
}

int MatLabEngine::getMxIntValue( const std::string &mxVarName, const std::string &fieldName ) {
	return isProperty( mxVarName, fieldName ) ? getMxIntValue( mxVarName + "." + fieldName ) : -1;
}

int MatLabEngine::getMxIntValue( const std::string &mxVarName, const char *fieldName ) {
	return getMxIntValue(  mxVarName, std::string( fieldName )  );
}


double MatLabEngine::getMxDoubleValue( const std::string &mxVarName, bool unchecked ) {
	executeCommand( "ans = 0.0" );
	executeCommand(  std::string( "double( " ) + mxVarName + " )", unchecked ); // COERCE "ans" TO DOUBLE TYPE

	mxArray *mxDoubleArray;
	if (   (  mxDoubleArray = engGetVariable( _ep, "ans" )  )  ==  0   ) {
		throw MatLabUdm::Exception( std::string( "Could not get value of " ) + mxVarName );
	}

	double *dblPtr = mxGetPr( mxDoubleArray );
	double retval = dblPtr == 0 ? 0 : dblPtr[0];
	mxDestroyArray( mxDoubleArray );

	return retval;
}

double MatLabEngine::getMxDoubleValue( const std::string &mxVarName, const std::string &fieldName ) {
	return isProperty( mxVarName, fieldName ) ? getMxDoubleValue( mxVarName + "." + fieldName ) : -1;
}

double MatLabEngine::getMxDoubleValue( const std::string &mxVarName, const char *fieldName ) {
	return getMxDoubleValue(  mxVarName, std::string( fieldName )  );
}


std::vector< double > MatLabEngine::getMxDoubleVector( const std::string &mxVarName ) {
	int size = getMxLinearSize( mxVarName );
//std::cerr << "getMxDoubleVector: \"" << mxVarName << "\"" << std::endl;
	executeCommand( mxVarName );
	mxArray *mxDoubleArray;
	if (   (  mxDoubleArray = engGetVariable( _ep, "ans" )  )  ==  0   ) {
		throw MatLabUdm::Exception( std::string( "Could not get value of " ) + mxVarName );
	}

	double *dblPtr = mxGetPr( mxDoubleArray );
	std::vector< double > retval( dblPtr, dblPtr + size );
	mxDestroyArray( mxDoubleArray );

	return retval;
}

std::set< std::string > MatLabEngine::getMxFieldNames( const std::string &mxVarName ) {
	std::string command = "ans = []";
	executeCommand( command );

	int size = getMxLinearSize( mxVarName );
	if ( size == 0 ) {
		return std::set< std::string >();
	}

	command = std::string( "fieldnames( " ) + mxVarName + " )";
//std::cerr << "getMxFieldNames: \"" << command << "\"" << std::endl;
	executeCommand( command, true );

	std::set< std::string > retval;

	mxArray *mxCellArray;
	if (   (  mxCellArray = engGetVariable( _ep, "ans" )  )  !=  0   ) {

		int mxCellArraySize = getVectorSize(  mxGetDimensions( mxCellArray )  );

		for( int ix = 0 ; ix < mxCellArraySize ; ++ix ) {
			mxArray *mxCell = mxGetCell( mxCellArray, ix );

			int stringLength = getVectorSize(  mxGetDimensions( mxCell )  );
#ifdef _WIN32
			wchar_t *stringValue  = static_cast< wchar_t * >(  mxGetData( mxCell )  );
#else
			short *stringValue  = static_cast< short * >(  mxGetData( mxCell )  );
#endif
			retval.insert(  std::string( stringValue, stringValue + stringLength )  );

		}

		mxDestroyArray( mxCellArray );
	}

	return retval;
}

std::string MatLabEngine::getData( const std::string &fullFieldName, bool encodeString ) {
	ArraySize arraySize = getMxMatrixSize( fullFieldName, true );

	std::string retval;

	if (   getMxIntValue(  std::string( "iscell( " ) + fullFieldName + " )", true  )   ) {
		retval += "{";

		for( int ix = 1 ; ix <= arraySize[0] ; (void)++ix ) {
			if ( ix != 1 ) retval += ";";
			for( int jx = 1 ; jx <= arraySize[1] ; (void)++jx ) {
				if ( jx != 1 ) retval += ",";
				retval += getData(
				 fullFieldName + "{" + boost::lexical_cast< std::string >( ix ) + "," +
				  boost::lexical_cast< std::string >( jx ) + "}",
				 true
				);
			}
		}

		retval += "}";
		return retval;
	}
	
	bool isscalar = getMxIntValue(  std::string( "isscalar( " ) + fullFieldName + " )", true  ) == 0 ? false : true;
	bool ischar = getMxIntValue(  std::string( "ischar( " ) + fullFieldName + " )", true  ) == 0 ? false : true;
	bool isstruct = getMxIntValue(  std::string( "isstruct( " ) + fullFieldName + " )", true  ) == 0 ? false : true;

	bool isMatrix = !isscalar && !ischar && !isstruct;

	if ( arraySize.size() > 2 ) retval += "reshape( ";
	if ( isMatrix ) retval += "[";

	if ( ischar ) {

		std::string data = getMxStringValue( fullFieldName );
		if ( encodeString ) {
			static boost::regex scPattern( ";" );
			static boost::regex cmPattern( "," );

			data = boost::regex_replace( data, scPattern, "&#x3B;" );
			data = boost::regex_replace( data, cmPattern, "&#x2C;" );
		}
		retval += data;

	} else if ( isstruct ) {
		retval += "{";
		// FIXME: this doesn't work right yet. try testing on cesna_a_37
		executeCommand(std::string("mdl2mga_fieldnames_ = fieldnames(") + fullFieldName + ")");
		for (int i = 1; i <= getMxIntValue("length(mdl2mga_fieldnames_)"); i++) {
			executeCommand(std::string("mdl2mga_fieldnames_field_ = mdl2mga_fieldnames_(") + boost::lexical_cast< std::string >(i) + ")");
			retval += getData("mdl2mga_fieldnames_field_") + ":";
			retval += getData(std::string("getfield(") + fullFieldName + ", mdl2mga_fieldnames_field_)");
		}
		retval += "}";
	} else if ( arraySize.size() <= 2 ) {
		for( int ix = 1 ; ix <= arraySize[0] ; ++ix ) {
			if ( ix != 1 ) retval += ";";
			for( int jx = 1 ; jx <= arraySize[1] ; ++jx ) {
				if ( jx != 1 ) retval += " ";

				std::string value = boost::lexical_cast< std::string >( getMxDoubleValue(
					fullFieldName + "(" + boost::lexical_cast< std::string >( ix ) + "," + boost::lexical_cast< std::string >( jx ) + ")"
				) );
				if ( value == "1.#INF" )       value = "inf";
				else if ( value == "-1.#INF" ) value = "-inf";

				retval += value;
			}
		}
	} else {
		int noElements = 1;
		for( ArraySize::iterator arsItr = arraySize.begin() ; arsItr != arraySize.end() ; ++arsItr ) {
			noElements *= *arsItr;
		}
		ArraySize counter( arraySize.size(), 1 );
		for( int ix = 0 ; ix < noElements ; ++ix ) {

			if ( ix != 0 ) retval += " ";

			std::string indexingString = "";
			bool firstIndex = true;
			for( ArraySize::iterator arsItr = counter.begin() ; arsItr != counter.end() ; ++arsItr ) {
				if ( firstIndex ) firstIndex = false;
				else              indexingString += ",";
				indexingString += boost::lexical_cast< std::string >( *arsItr );
			}

			std::string value = boost::lexical_cast< std::string >(  getMxDoubleValue( fullFieldName + "(" + indexingString + ")" )  );
			if ( value == "1.#INF" )       value = "inf";
			else if ( value == "-1.#INF" ) value = "-inf";

			retval += value;

			for( unsigned int jx = 0 ; jx < (unsigned int) arraySize.size() ; ++jx ) {
				if ( counter[jx] == arraySize[jx] ) counter[jx] = 1;
				else                                { ++counter[jx]; break; }
			}

		}

	}

	if ( isMatrix ) retval += "]";
	if ( arraySize.size() > 2 ) {
		retval += ",[";
		bool firstSize = false;
		for( ArraySize::iterator arsItr = arraySize.begin() ; arsItr != arraySize.end() ; ++arsItr ) {
			if ( firstSize ) firstSize = false;
			else             retval += " ";
			retval += boost::lexical_cast< std::string >( *arsItr );
		}
		retval += "])";
	}


	return retval;
}

bool MatLabEngine::hasField( const std::string &matLabObject, const std::string &fieldName ) {
	return getMxIntValue(
	 std::string( "isfield( " ) + matLabObject + ".struct, '" + fieldName + "' )"
	) == 1;
}

bool MatLabEngine::varExists( const std::string &varName ) {
	executeCommand( "ans = 0" );
	executeCommand(  std::string( "exist " ) + varName  );

	mxArray *mxIntArray;
	if (   (  mxIntArray = engGetVariable( _ep, "ans" )  )  ==  0   ) {
		throw MatLabUdm::Exception( std::string( "Could not see if variable \"" + varName + "\" exists" ) );
	}
	double *dblPtr = mxGetPr( mxIntArray );
	int retval  = dblPtr == 0 ? 0 : static_cast< int >( dblPtr[0] );
	mxDestroyArray( mxIntArray );

	return retval != 0;
}

std::string MatLabEngine::getUniqueVarName( const std::string &varName ) {
	std::string newVarName;
	do {
		newVarName = varName + boost::lexical_cast< std::string >( getUniqueNo() );
	} while(  varExists( newVarName )  );
	return newVarName;
}

void MatLabEngine::clearVar( const std::string &varName ) {
	executeCommand(  std::string( "clear " ) + varName  );
}
