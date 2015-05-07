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
 *  FILE NAME:	MatLabUdm.cpp 
 *	SYNOPSIS:	Implementation of class "MatLabUdm", provides base functionality for translating
 *				MatLab SL/SF models into equivalent GME ESMoL models.					
 *  AUTHOR:		Harmon Nine - hnine@isis.vanderbilt.edu
 *  CREATED:	07/20/06
 *  LAST UPDATE: 
 */
#include "MatLabUdm.hpp"
#include "UdmEngine.hpp"
#include "mfile2SFC.hpp"
#include "mfileScope.hpp"
#include "ScopeSymbolTable.hpp"

#include "ConfigKeeper.hpp"

#include <fstream>
#include <boost/algorithm/string.hpp>
#include <ctype.h>

#ifdef LINUX
#include <unistd.h>
#define SLEEP usleep( 500000 )
#else
#include <windows.h>
#define SLEEP Sleep( 500 )
#endif


bool MatLabUdm::onlyTopLevel = false;
boost::filesystem::path MatLabUdm::TopLevelModelManager::registerModel( const std::string &modelFilename ) {
	std::string modelPath = MatLabEngine::globalEngine().getMxStringValue( "ans = which( '" + modelFilename + "' )" );
	boost::filesystem::path path( modelPath );

	return registerModel( path );
}

boost::filesystem::path MatLabUdm::TopLevelModelManager::registerModel( boost::filesystem::path path ) {

#if BOOST_VERSION / 100 % 1000 < 46
	path = boost::filesystem::complete( path );
	boost::filesystem::path path_mdl2mga = path.parent_path() / ( path.stem() + "Xmdl2mga" + path.extension() );

	if (  boost::filesystem::exists( path_mdl2mga )  ) {
		boost::filesystem::remove( path_mdl2mga );
	}
	boost::filesystem::copy_file( path, path_mdl2mga );

	std::string modelName_mdl2mga = path_mdl2mga.stem();

#else
	path = boost::filesystem::absolute( path );
	boost::filesystem::path path_mdl2mga = path.parent_path() / ( path.stem().string() + "Xmdl2mga" + path.extension().string() );

	if (  boost::filesystem::exists( path_mdl2mga )  ) {
		boost::filesystem::remove( path_mdl2mga );
	}
	boost::filesystem::copy_file( path, path_mdl2mga );

	std::string modelName_mdl2mga = path_mdl2mga.stem().string();

#endif
	_modelMap.insert(  std::make_pair( modelName_mdl2mga, path_mdl2mga )  );
	_modelList.push_front(  std::make_pair( modelName_mdl2mga, path_mdl2mga )  );

	return path_mdl2mga;
}

void MatLabUdm::TopLevelModelManager::compileModels( void ) {

	for( ModelList::reverse_iterator mdlItr = _modelList.rbegin() ; mdlItr != _modelList.rend() ; ++mdlItr ) {

		std::string modelName = mdlItr->first;

		MatLabEngine::globalEngine().executeCommand( "blockDiagram = r.find( '-isa', 'Simulink.BlockDiagram', '-depth', 1, 'name', '" + modelName + "' )" );
		std::string simulationStatus = MatLabEngine::globalEngine().getMxStringValue( "blockDiagram.SimulationStatus" );

		if ( simulationStatus == "paused" ) return;

		std::string functionName = modelName;

		std::cerr << "Compiling " << modelName << " ...";
		MatLabEngine::globalEngine().executeCommand( functionName + "( [], [], [], 'compile' )" );
		simulationStatus = MatLabEngine::globalEngine().getMxStringValue( "blockDiagram.SimulationStatus" );

		for ( int ix = 0 ; ix < 40 && simulationStatus != "paused" ; ++ix ) {
			SLEEP;
			simulationStatus = MatLabEngine::globalEngine().getMxStringValue( "blockDiagram.SimulationStatus" );
			++ix;
		}

		if ( simulationStatus == "paused" ) {

			std::cerr << " done" << std::endl;

		} else {

			std::cerr << " FAILED" << std::endl;

			std::string exceptionMessage =
			 "Could not compile Simulink model \"" + modelName + "\".\n"
			 "Please try to compile it manually by loading the\n"
			 "\"" + modelName + ".mdl\" file into MatLab and executing\n"
			 "the following command:\n\n"
			 "    " + functionName + "( [],[],[],'compile' )\n\n"
			 "This should give you diagnostic information concerning\n"
			 "how to fix the model so it will compile.\n\n"
			 "Also, once you get the model to compile, be sure to execute\n"
			 "the following command before trying to exit MatLab:\n\n"
			 "    " + functionName + "( [],[],[],'term' )\n\n"
			 "If you don't, MatLab won't let you exit.\n\n";
			throw MatLabUdm::Exception( exceptionMessage );
		}

	}

}

void MatLabUdm::TopLevelModelManager::stopCompileModels( void ) {

	for( ModelList::iterator mdlItr = _modelList.begin() ; mdlItr != _modelList.end() ; ++mdlItr ) {

		std::string modelName = mdlItr->first;

		MatLabEngine::globalEngine().executeCommand( "blockDiagram = r.find( '-isa', 'Simulink.BlockDiagram', '-depth', 1, 'name', '" + modelName + "' )" );
		std::string simulationStatus = MatLabEngine::globalEngine().getMxStringValue( "blockDiagram.SimulationStatus" );

		if ( simulationStatus == "stopped" ) return;

		std::string functionName = modelName;

		std::cerr << "Stopping compile on " << modelName << " ...";
		MatLabEngine::globalEngine().executeCommand( functionName + "( [], [], [], 'term' )" );

		int ix = 0;
		while( ix < 10 && simulationStatus != "stopped" ) {
			SLEEP;
			simulationStatus = MatLabEngine::globalEngine().getMxStringValue( "blockDiagram.SimulationStatus" );
			++ix;
		}

		if ( simulationStatus == "stopped" ) {

			std::cerr << " done" << std::endl;

		} else {

			std::cerr << " FAILED" << std::endl;

			std::string exceptionMessage =
			 "Could not terminate compilation of Simulink model \"" + modelName + "\".\n"
			 "You will need to do this manually in the Simulink command window\n"
			 "(that was spawned by this process) by entering the following command:\n\n"
			 "    " + functionName + "( [],[],[],'term' )\n\n"
			 "Of MatLab doesn't indicate any problems with this command, enter:\n\n"
			 "exit\n\n";
		}
	}

}


MatLabUdm::IntList MatLabUdm::getIntList( const std::string &input ) {

	IntList intList;

	static boost::regex numberRegex( "(?:-\\s*)?\\d+", boost::regex_constants::perl );
	std::string::const_iterator start = input.begin();
	std::string::const_iterator finish = input.end();
	boost::match_results<std::string::const_iterator> results;
	while (  regex_search( start, finish, results, numberRegex )  ) {
		intList.push_back(    boost::lexical_cast< int >(  std::string( results[0].first, results[0].second )  )   );
		start = results[0].second;
	}

	return intList;
}

MatLabUdm::StringList MatLabUdm::getWordList( const std::string &input ) {

	StringList wordList;

	static boost::regex numberRegex( "\\w[\\w\\d]*", boost::regex_constants::perl );
	std::string::const_iterator start = input.begin();
	std::string::const_iterator finish = input.end();
	boost::match_results<std::string::const_iterator> results;
	while (  regex_search( start, finish, results, numberRegex )  ) {
		wordList.push_back(  std::string( results[0].first, results[0].second )  );
		start = results[0].second;
	}

	return wordList;
}

std::string MatLabUdm::getParentMatLabObject( void ) {

	std::string matLabPrefix = getMatLabPrefix();

	std::string::size_type pos = matLabPrefix.size() - 1;
	while(  pos >= 0 && isdigit( matLabPrefix[ pos ] )  ) --pos;	
	if ( pos < 0 ) return getMatLabObject();

	std::string matLabArray = matLabPrefix.substr( 0, pos + 1 );
	std::string matLabParentPrefix = matLabArray.substr( 0, matLabArray.size() - 1 );

	pos = matLabParentPrefix.size() - 1;
	while(  pos >= 0 && isdigit( matLabParentPrefix[ pos ] )  ) --pos;	
	if ( pos < 0 ) return getMatLabObject();

	std::string matLabParentArray = matLabParentPrefix.substr( 0, pos + 1 );
	std::string number = matLabParentPrefix.substr( pos + 1 );
	int subscript = boost::lexical_cast< int >( number ) + 1;

	return matLabParentArray + "(" + boost::lexical_cast< std::string >( subscript ) + ")";
}

MatLabUdm::ValueSubTableInit::ValueSubTableInit( void ) {

	if (  ConfigKeeper::getSingleton().getCount( "substitute" ) == 0  ) return;

	std::string filename =  ConfigKeeper::getSingleton().getStringValue( "substitute" );

	if ( filename[1] != ':' ) {
		if (  filename.find_first_of( "/\\" ) == std::string::npos  ) {
			filename = std::string( "config\\" ) + filename;
		}

		std::string vcp_path = ConfigKeeper::getSingleton().getStringValue( "vcp_path" );
		filename = vcp_path + "\\" + filename;
	}

	boost::filesystem::path path( filename );
	if (  !boost::filesystem::exists( path )  ) {
		std::cerr << "WARNING:  Could not find config file \"" << path.filename() << "\"" << std::endl;
		return;
	}

	static boost::regex parseTypesRegex( "^\\s*(\\S+(?: \\S+)*)(?:\t|\\s{2,})(\\S+(?: \\S+)*)" );
	boost::match_results< std::string::const_iterator > results;

	std::ifstream input( path.string().c_str() );
	std::string line;
	while( std::getline( input, line )  ) {
		const std::string constLine( line );
		if (  !regex_search( constLine.begin(), constLine.end(), results, parseTypesRegex )  ) continue;

		std::string fromType( results[1].first, results[1].second );
		std::string toType( results[2].first, results[2].second );

		_valueSubTable[ fromType ] = toType;
	}
}

bool MatLabUdm::toBuildUdm(const Udm::Object &udmParent)
{
	if(!onlyTopLevel) return true;
	string ut = udmParent.type().name();
	if(ut!="Subsystem") return true;
	Udm::Object pp = udmParent.GetParent();
	ut = pp.type().name();
	if(ut!="Subsystem") return true;
	Udm::Object ppp = pp.GetParent();
	ut = ppp.type().name();
	if(ut!="Subsystem") return true;
	return false;
}

MatLabUdm::SPVector MatLabUdm::buildUdm( MatLabUdm &matLabUdmParent, const Factory &factory ) {

	matLabUdmParent.delayedSetup();

	SPVector collection;
	std::string command;

	Udm::Object udmParent = matLabUdmParent.getUdmObject();
	if(!toBuildUdm(udmParent)) return collection;

	const std::string matLabParent = matLabUdmParent.getMatLabObject();
	std::string matLabName = matLabUdmParent.getMatLabPrefix() + factory.getMatLabChar();

	command = matLabName + " = " + matLabParent + ".find( " + factory.getMatLabType() + ", '-depth', 1 )";

	MatLabEngine::globalEngine().executeCommand( command );
	command = matLabName + " = setdiff(" + matLabName + ',' + matLabParent + ')';
	MatLabEngine::globalEngine().executeCommand( command );

	int noCurrentObjects = MatLabEngine::globalEngine().getMxLinearSize( matLabName );

	for( int ix = 0 ; ix < noCurrentObjects ; ++ix ) {
		const std::string matLabObject = getMatLabObjectString( matLabName, ix );

		if (  factory.exclude( matLabObject )  ) continue;

		SmartPointer matLabUdmPtr = factory.create( udmParent, matLabName, ix );
		if ( matLabUdmPtr->collect() ) collection.push_back( matLabUdmPtr );
		else                           matLabUdmPtr->delayedSetup();
	}

	return collection;
}

bool MatLabUdm::hasChildren( MatLabUdm &matLabUdmParent, const Factory &factory ) {

	const std::string matLabParent = matLabUdmParent.getMatLabObject();
	std::string matLabName = matLabUdmParent.getMatLabPrefix() + factory.getMatLabChar();

	std::string command = matLabName + " = " + matLabParent + ".find( " + factory.getMatLabType() + ", '-depth', 1 )";

	MatLabEngine::globalEngine().executeCommand( command );
	command = matLabName + " = setdiff(" + matLabName + ',' + matLabParent + ')';
	MatLabEngine::globalEngine().executeCommand( command );

	int noCurrentObjects = MatLabEngine::globalEngine().getMxLinearSize( matLabName );

	return noCurrentObjects > 0;
}

std::string MatLabUdm::fixSpecialChars( const std::string &str ) {

	static boost::regex specialChars( "[\n]", boost::regex_constants::perl );
	boost::match_results< std::string::const_iterator > results;

	if (  !boost::regex_search( str.begin(), str.end(), results, specialChars )  ) return std::string("'") + str + "'";

	std::string retval("[ ");
	if ( str.begin() != results[0].first ) {
		retval += '\'';
		retval.append( str.begin(), results[0].first );
		retval += "', ";
	}
	retval += boost::lexical_cast< std::string >(  static_cast< int >( *results[0].first )  );

	std::string::const_iterator strItr = results[0].second;

	while (  boost::regex_search( strItr, str.end(), results, specialChars )  ) {

		if ( strItr != results[0].first ) {
			retval += ", '";
			retval.append( strItr, results[0].first );
			retval += '\'';
		}
		retval += ", " + static_cast< int >( *results[0].first );

		strItr = results[0].second;
	}
	if ( strItr != str.end() ) {
		retval += ", '";
		retval.append( strItr, str.end() );
		retval += '\'';
	}
	retval += " ]";

	return retval;
}

void MatLabUdm::connectPort( SLSF::Port port, bool srcFlag, const std::string &portConn, const std::string &otherPortType ) {

	int portConnSize = MatLabEngine::globalEngine().getMxLinearSize( portConn, true );
	if ( portConnSize == 0 ) return;

	int noOtherPorts = MatLabEngine::globalEngine().getMxLinearSize( portConn + '.' + otherPortType + "Port", true );

	for( int ix = 0 ; ix < noOtherPorts ; ++ix ) {
		int otherPortHandle = MatLabEngine::globalEngine().getMxIntValue(
		 portConn + '.' + otherPortType + "Block(" + boost::lexical_cast< std::string >( ix + 1 ) + ')'
		);
		int otherPortNo = MatLabEngine::globalEngine().getMxIntValue(
		 portConn + '.' + otherPortType + "Port(" + boost::lexical_cast< std::string >( ix + 1 ) + ')'
		);
		PortType otherPortGroup = srcFlag ? OUTPUTPORT : INPUTPORT;

		UdmSocketMap::iterator optItr = getUdmSocketMap().find(  UdmSocket( otherPortHandle, otherPortNo, otherPortGroup )  );

		if ( optItr == getUdmSocketMap().end() ) continue;

		SLSF::Port otherPort = optItr->second;
		if (  connectionExists( port, otherPort )  ) continue;

		SLSF::Port fromPort;
		SLSF::Port toPort;

		if ( srcFlag ) {
			fromPort = otherPort;
			toPort = port;
		} else {
			fromPort = port;
			toPort = otherPort;
		}

		Udm::Object lineParent = Udm::IsDerivedFrom( fromPort.type(), SLSF::InPort::meta ) ? fromPort.GetParent() : fromPort.GetParent().GetParent();
		Udm::Object lineTestParent = Udm::IsDerivedFrom( toPort.type(), SLSF::OutPort::meta ) ? toPort.GetParent() : toPort.GetParent().GetParent();
		if ( lineParent != lineTestParent ) continue;

		// IN CASE WHERE PARENT OF UDMOBJECT IS THE DATAFLOW FOLDER
		if (  !lineParent.isInstance() && Udm::IsDerivedFrom( lineParent.type(), SLSF::Block::meta )  ) {
			Line line = Line::Create( lineParent );
			line.RefId() = getRefId();
			line.srcLine_end() = fromPort;
			line.dstLine_end() = toPort;
		}
	}
}

bool MatLabUdm::connectionExists( SLSF::Port port1, SLSF::Port port2 ) {
	LineSet lineSet = port1.srcLine();
	PortSet dstPortSet = for_each( lineSet.begin(), lineSet.end(), LineDstPortTrans() ).getDstPortSet();
	if ( dstPortSet.find( port2 ) != dstPortSet.end() ) return true;

	lineSet = port2.srcLine();
	dstPortSet = for_each( lineSet.begin(), lineSet.end(), LineDstPortTrans() ).getDstPortSet();
	return dstPortSet.find( port1 ) != dstPortSet.end();
}

SLSF::TypeStruct MatLabUdm::makeBusObjectTypeStructAux( const std::string &busObjectName ) {

	 static boost::regex busPrefixPattern( "^[^:]*:\\s*" );

	SLSF::TypeStruct typeStruct = SLSF::TypeStruct::Create( UdmEngine::getUdmTypesRoot() );
	typeStruct.name() = busObjectName;

	std::string boElements = busObjectName + "E";
	MatLabEngine::globalEngine().executeCommand( boElements + " = " + busObjectName + ".Element" );
	if (  !MatLabEngine::globalEngine().varExists( boElements )  ) {
		MatLabEngine::globalEngine().executeCommand( boElements + " = " + busObjectName + ".Elements" );
	}
	int noElements = MatLabEngine::globalEngine().getMxLinearSize( boElements );
	typeStruct.MemberCount() = noElements;
	for( int ix = 0 ; ix < noElements ; (void)++ix ) {
		SLSF::TypeBaseRef typeBaseRef = SLSF::TypeBaseRef::Create( typeStruct );
		typeBaseRef.MemberIndex() = ix;
		typeBaseRef.name() = MatLabEngine::globalEngine().getMxStringValue(  boElements + "(" + boost::lexical_cast< std::string >( ix + 1 ) + ").Name"  );

		std::string elementDataType = MatLabEngine::globalEngine().getMxStringValue(  boElements + "(" + boost::lexical_cast< std::string >( ix + 1 ) + ").DataType"  );
		elementDataType = boost::regex_replace( elementDataType, busPrefixPattern, "" );

		int exist = MatLabEngine::globalEngine().getMxIntValue( "exist( '" + elementDataType + "' )", true );
		int isBusObject = ( exist != 5 ) && MatLabEngine::globalEngine().getMxIntValue( "isa( " + elementDataType + ", 'Simulink.Bus' )", true );

		if ( isBusObject != 0 ) {
			typeBaseRef.ref() = makeBusObjectTypeStructAux( elementDataType );
		} else {

			SLSF::Matrix ESMoLMatrix = SLSF::Matrix::Create( UdmEngine::getUdmTypesRoot() );
			ESMoLMatrix.Type() = MatLabUdm::parameterValueSub( elementDataType );

			std::string dimensions = MatLabEngine::globalEngine().getData(  boElements + "(" + boost::lexical_cast< std::string >( ix + 1 ) + ").Dimensions"  );
			int rows = 1;
			int columns = 1;
			getDimensions( dimensions, rows, columns );
			ESMoLMatrix.rows() = rows;
			ESMoLMatrix.columns() = columns;

			typeBaseRef.ref() = ESMoLMatrix;
		}
	}

	return typeStruct;
}


SLSF::TypeStruct MatLabUdm::makeBusObjectTypeStruct( const std::string &busObjectName ) {

	BusObjectMap::iterator bomItr = getBusObjectMap().find( busObjectName );
	if ( bomItr != getBusObjectMap().end() ) return bomItr->second;

	SLSF::TypeStruct typeStruct = makeBusObjectTypeStructAux( busObjectName );
	UdmEngine::integrateTypeBase( typeStruct );

	getBusObjectMap().insert(  std::make_pair( busObjectName, typeStruct )  );
	return typeStruct;
}

void MatLabUdm::performParallelAssignment( AssignmentList &assignmentList ) {

	// PERFORM UNIQUE-NAME SUBSTITUTION ON RHS FIRST
	for( AssignmentList::iterator aslItr = assignmentList.begin() ; aslItr != assignmentList.end() ; ++aslItr ) {
		std::string rhs = aslItr->getRHS();
		if ( rhs[0] != '\'' ) {
			aslItr->setRHS(  getScopeSymbolTable().performSimpleSubstitution( rhs )  );
		}
	}

	// GIVE UNIQUE NAMES TO LHS (LVALUE VARIABLES -- FOR SCOPING)
	for( AssignmentList::iterator aslItr = assignmentList.begin() ; aslItr != assignmentList.end() ; ++aslItr ) {
		aslItr->setVarName(  getScopeSymbolTable().addSymbol( aslItr->getVarName() )  );
	}

	// EVALUATE ASSIGNMENT
	for( AssignmentList::iterator aslItr = assignmentList.begin() ; aslItr != assignmentList.end() ; ++aslItr ) {
		std::string command = aslItr->getVarName() + aslItr->getVarMember() + " = " + aslItr->getRHS();
		MatLabEngine::globalEngine().executeCommand( command, true );
	}

}

void MatLabUdm::performMaskInitialization( const MatLabUdm &matLabUdm ) {

	std::string maskInitialization = MatLabEngine::globalEngine().getMxStringValue( matLabUdm.getMatLabObject() + ".MaskInitialization" );

	if ( maskInitialization.empty() ) return;

//std::cerr << "maskInitialization:" << std::endl;
//std::cerr << "----------------------" << std::endl;
//std::cerr << maskInitialization << std::endl;
//std::cerr << "----------------------" << std::endl << std::endl;

	maskInitialization = RegexCommon::eliminateComments( maskInitialization );
	maskInitialization = RegexCommon::eliminateContinuations( maskInitialization );
	maskInitialization = RegexCommon::eliminateBlankLines( maskInitialization );

//std::cerr << "maskInitialization:" << std::endl;
//std::cerr << "----------------------" << std::endl;
//std::cerr << maskInitialization << std::endl;
//std::cerr << "----------------------" << std::endl << std::endl;

	std::string fullPath = MatLabEngine::globalEngine().getMxStringValue( matLabUdm.getMatLabObject() + ".Path" ) + "/" + MatLabEngine::globalEngine().getMxStringValue( matLabUdm.getMatLabObject() + ".Name" );
	getScopeSymbolTable().addSpecialSymbol( "gcb", "'" + fullPath + "'" );

	std::string handle = boost::lexical_cast< std::string >(  MatLabEngine::globalEngine().getMxDoubleValue( matLabUdm.getMatLabObject() + ".Handle" )  );
	getScopeSymbolTable().addSpecialSymbol( "gcbh", handle );

	maskInitialization = getMFileScope().scopeTrans( maskInitialization, getScopeSymbolTable() );

//std::cerr << "maskInitialization:" << std::endl;
//std::cerr << "----------------------" << std::endl;
//std::cerr << maskInitialization << std::endl;
//std::cerr << "----------------------" << std::endl << std::endl;

	MatLabEngine::globalEngine().executeCommand( maskInitialization, true );
}
