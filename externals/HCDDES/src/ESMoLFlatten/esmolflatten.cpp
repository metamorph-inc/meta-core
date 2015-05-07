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
 *  FILE NAME:	esmolflatten.cpp 
 *	SYNOPSIS:	Main program for translating SL/SF mdl files into GME ESMoL files.					
 *  AUTHOR:		Harmon Nine - hnine@isis.vanderbilt.edu
 *  CREATED:	03/10/11
 *  LAST UPDATE: 
 */

#include <vector>
#include <set>
#include <boost/regex.hpp>
#include <boost/filesystem/operations.hpp>

#ifdef _WIN32
#include <windows.h>
#include <tchar.h>
#endif

#include "UdmEngine.hpp"
#include "ConfigKeeper.hpp"

typedef std::vector< ESMoL::ActionPort > ActionPortVector;
typedef std::vector< ESMoL::ConnectorRef > ConnectorRefVector;
typedef std::vector< ESMoL::Data > DataVector;
typedef std::vector< ESMoL::Dataflow > DataflowVector;
typedef std::vector< ESMoL::DesignFolder > DesignFolderVector;
typedef std::vector< ESMoL::Event > EventVector;
typedef std::vector< ESMoL::Junction > JunctionVector;
typedef std::set< ESMoL::Line > LineSet;
typedef std::vector< ESMoL::ModelsFolder > ModelsFolderVector;
typedef std::vector< ESMoL::Port > PortVector;
typedef std::vector< ESMoL::Primitive > PrimitiveVector;
typedef std::vector< ESMoL::State > StateVector;
typedef std::vector< ESMoL::Stateflow > StateflowVector;
typedef std::vector< ESMoL::Subsystem > SubsystemVector;
typedef std::vector< ESMoL::TransStart > TransStartVector;
typedef std::vector< ESMoL::Transition > TransitionVector;
typedef std::vector< ESMoL::TypeBase > TypeBaseVector;
typedef std::vector< ESMoL::TypeBaseRef > TypeBaseRefVector;


typedef std::map< ESMoL::Port, ESMoL::Port > PortMap;
static PortMap &getPortMap( void ) {
	static PortMap portMap;
	return portMap;
}

typedef std::list< ESMoL::Port > PortList;
static PortList &getPortList( void ) {
	static PortList portList;
	return portList;
}

typedef std::map< ESMoL::TypeBase, ESMoL::TypeBase > TypeBaseMap;
static TypeBaseMap &getTypeBaseMap( void ) {
	static TypeBaseMap typeBaseMap;
	return typeBaseMap;
}

typedef std::map< ESMoL::TransConnector, ESMoL::TransConnector > TransConnectorMap;
static TransConnectorMap &getTransConnectorMap( void ) {
	static TransConnectorMap transConnectorMap;
	return transConnectorMap;
}

typedef std::list< ESMoL::ConnectorRef > ConnectorRefList;
static ConnectorRefList &getConnectorRefList( void ) {
	static ConnectorRefList connectorRefList;
	return connectorRefList;
}

void copyTypes( UdmEngine &inputModel, UdmEngine &outputModel );
ESMoL::TypeBase copyTypesAux( ESMoL::TypeBase typeBase, ESMoL::Types outputTypes );
void copyDesignFolders( ESMoL::RootFolder inputRootFolder, ESMoL::RootFolder outputRootFolder );
void copyModelsFolders( ESMoL::DesignFolder inputDesignFolder, ESMoL::DesignFolder outputDesignFolder );
void copyStateflows( ESMoL::ModelsFolder inputModelsFolder, ESMoL::ModelsFolder outputModelsFolder );
void copyStates( ESMoL::Stateflow inputStateflow, ESMoL::Stateflow outputStateflow );
void copyState( ESMoL::State inputState, ESMoL::State outputState );
void copyData( ESMoL::Data inputData, ESMoL::Data outputData );
void copyEvent( ESMoL::Event inputEvent, ESMoL::Event outputEvent );
void copyDataflows( ESMoL::ModelsFolder inputModelsFolder, ESMoL::ModelsFolder outputModelsFolder );
void copySubsystems_flatten( ESMoL::Dataflow inputDataflow, ESMoL::Dataflow outputDataflow );
void flattenSubsystem( ESMoL::Subsystem inputSubsystem, ESMoL::Subsystem outputSubsystem );
bool isStateMachine( ESMoL::Subsystem subsystem );
bool isConditionActionSubsystem( ESMoL::Subsystem subsystem );
void copySubsystem( ESMoL::Subsystem inputSubSubsystem, ESMoL::Subsystem outputSubsystem, bool deep = true );
void copyPorts( ESMoL::Block inputBlock, ESMoL::Block outputBlock );


void usage( const std::string &exeName );


int main( int argc, char *argv[] ) {
	int exit_status = 0;
	// Try to process the command line arguments
	if ( !ConfigKeeper::getSingleton().processCommandLineArguments( argc, argv ) ) {
		return 0;
	}
	// Get the input file name
	if (  ConfigKeeper::getSingleton().getCount( "help" )  ) {
		usage( argv[0] );
		return 0;
	}

	if (  !ConfigKeeper::getSingleton().getCount( "inputFile" )  ) {
		usage( argv[0] );
		return 1;
	}
	std::string inputFilename = ConfigKeeper::getSingleton().getStringValue( "inputFile" );
	// CHECK IF INPUT FILE EXISTS
	if (  !boost::filesystem::exists( inputFilename )  ) {
		std::cerr << "ERROR: Could not access file \"" << inputFilename << "\"." << std::endl << std::endl;
		return 2;
	}

	boost::regex absPathTest( "^(?:[A-Za-z]:|[\\/])" );
	boost::match_results< std::string::const_iterator > results;

	if (  !boost::regex_search( inputFilename, results, absPathTest )  ) {
#ifdef _WIN32
		TCHAR currentDirectory[ 1024 ];
		GetCurrentDirectory( 1024, currentDirectory );
		inputFilename = std::string( currentDirectory ) + '\\' + inputFilename;
#else
		char currentDirectory[ 1024 ];
		getcwd( currentDirectory, 1024 );
		inputFilename = std::string( currentDirectory ) + '/' + inputFilename;
#endif
	}

	std::string outputFilename;
	if (  ConfigKeeper::getSingleton().getCount( "outputFile" )  ) {

		outputFilename = ConfigKeeper::getSingleton().getStringValue( "outputFile" );

	} else {

		std::string inputFilenameBase;
		std::string::size_type charPos = inputFilename.rfind( '/' );
		if ( charPos != std::string::npos ) {
			charPos = inputFilenameBase.rfind( '\\' );
			inputFilenameBase = charPos == std::string::npos ? inputFilenameBase : inputFilenameBase.substr( charPos + 1 );
		} else {
			inputFilenameBase = inputFilename.substr( charPos + 1 );
		}
		charPos = inputFilenameBase.rfind( '.' );
		inputFilenameBase = charPos == std::string::npos ? inputFilenameBase : inputFilenameBase.substr( 0, charPos );
		outputFilename = inputFilenameBase + "_flatten.xml";

	}

	// INITIALIZE THE DATA NETWORK
	UdmEngine inputModel;
	UdmEngine outputModel;

	//try {

		inputModel.openExisting( inputFilename );
		outputModel.createNew( outputFilename );

		copyTypes( inputModel, outputModel );

		ESMoL::RootFolder inputRoot = inputModel.getRootFolder();
		ESMoL::RootFolder outputRoot = outputModel.getRootFolder();
		outputRoot.name() = inputRoot.name();

		copyDesignFolders( inputRoot, outputRoot );

		inputModel.closeNoUpdate();
		outputModel.closeWithUpdate();

	//} catch ( udm_exception &err ) {

	//	std::cerr << "The following UDM exception occurred:" << std::endl;
	//	std::cerr << err.what() << std::endl << std::endl;

	//	exit_status = 3;

	//} catch ( std::exception &err ) {

	//	std::cerr << "The following std::exception occurred:" << std::endl;
	//	std::cerr << err.what() << std::endl << std::endl;
	//	exit_status = 4;

	//} catch ( ... ) {

	//	std::cerr << "Unknown exception caught" << std::endl;
	//	exit_status = 5;
	//}

	return exit_status;
}


void usage( const std::string &exeName ) {

	std::string::size_type lastForwardSlash = exeName.rfind("/");
//	std::cerr << "lastForwardSlash = " << lastForwardSlash << std::endl;
	std::string::size_type lastBackSlash = exeName.rfind("\\");
//	std::cerr << "lastBackSlash = " << lastBackSlash << std::endl;

	std::string::size_type lastSlash = lastForwardSlash == std::string::npos ? lastBackSlash : lastForwardSlash;
	std::string baseName = exeName.substr( lastSlash + 1 );

	std::cerr << "Usage:" << std::endl;
	std::cerr << ConfigKeeper::getSingleton().getDescription();
	std::cerr << baseName << " [options] <esmol-input-file> [ <esmol-output-mga-file> | <esmol-output-xml-file> ] [ -s <subsystem> ]..." << std::endl << std::endl;
	std::cerr << "Required arguments:" << std::endl;
	std::cerr << "  <esmol-input-file> is the name of an ESMoL model file (either .mga or .xml)" << std::endl;
	std::cerr << "   which is to have its Dataflow models flattened and output to an output file." << std::endl << std::endl;
	std::cerr << "  If the second argument (the output file) is not specified, it defaults to a " << std::endl;
	std::cerr << "   file that has the same name as the input file, but with \"flattened\" appended" << std::endl;
	std::cerr << "   to the main filename, and with a .xml extension." << std::endl;
	std::cerr << "Optional arguments:" << std::endl;
	std::cerr << "  <esmol-output-mga-file> is the name of an ESMoL .mga model file that will be" << std::endl;
	std::cerr << "   created or overwritten with a flattened version of dataflow portion of the " << std::endl;
	std::cerr << "   input file." << std::endl << std::endl;
	std::cerr << "  <esmol-output-xml-file> is the name of an XML-based (.xml) ESMoL model file " << std::endl;
	std::cerr << "   that will be created or overwritten, and will conform to the ESMoL paradigm as" << std::endl;
	std::cerr << "   specified in the ESMoL.xsd XML schema file.  This schema file is embedded" << std::endl;
	std::cerr << "   in this program and so does not have to be explicitly specified." << std::endl << std::endl;
	std::cerr << "Options:" << std::endl;
	std::cerr << "  -s <subsytem>" << std::endl;
	std::cerr << "   This option can be specified multiple times.  <subsystem> specifies a subsystem " << std::endl;
	std::cerr << "   to flatten and place in the output file.  IF THIS OPTION IS NOT SPECIFIED, the " << std::endl;
	std::cerr << "   topmost subsytems are flattened, i.e. those with no instances in other subsystems" << std::endl << std::endl;
}

/***************************************************************************************/

void copyTypes( UdmEngine &inputModel, UdmEngine &outputModel ) {
	ESMoL::Types inputTypes = inputModel.getTypes();
	ESMoL::Types outputTypes = outputModel.getTypes();

	TypeBaseVector typeBaseVector = inputTypes.TypeBase_kind_children();
	for( TypeBaseVector::iterator tbvItr = typeBaseVector.begin() ; tbvItr != typeBaseVector.end() ; ++tbvItr ) {
		ESMoL::TypeBase &typeBase = *tbvItr;
		ESMoL::TypeBase otherTypeBase = copyTypesAux( typeBase, outputTypes );
	}
}

ESMoL::TypeBase copyTypesAux( ESMoL::TypeBase inputTypeBase, ESMoL::Types outputTypes ) {
	
	TypeBaseMap::iterator tbmItr = getTypeBaseMap().find( inputTypeBase );
	if ( tbmItr != getTypeBaseMap().end() ) return tbmItr->second;

	if ( inputTypeBase.type() == ESMoL::Matrix::meta ) {
		ESMoL::Matrix inputEsmolMatrix = ESMoL::Matrix::Cast( inputTypeBase );
		ESMoL::Matrix outputEsmolMatrix = ESMoL::Matrix::Create( outputTypes );
		UdmEngine::copyMatrix( inputEsmolMatrix, outputEsmolMatrix );
		getTypeBaseMap().insert(  std::make_pair( inputEsmolMatrix, outputEsmolMatrix )  );
		return outputEsmolMatrix;
	}

	ESMoL::TypeStruct inputTypeStruct = ESMoL::TypeStruct::Cast( inputTypeBase );
	ESMoL::TypeStruct outputTypeStruct = ESMoL::TypeStruct::Create( outputTypes );
	outputTypeStruct.name() = inputTypeStruct.name();
	outputTypeStruct.MemberCount() = inputTypeStruct.MemberCount();
	getTypeBaseMap().insert(  std::make_pair( inputTypeStruct, outputTypeStruct )  );

	TypeBaseRefVector typeBaseRefVector = inputTypeStruct.TypeBaseRef_kind_children();
	for( TypeBaseRefVector::iterator trvItr = typeBaseRefVector.begin() ; trvItr != typeBaseRefVector.end() ; ++trvItr ) {
		ESMoL::TypeBaseRef inputTypeBaseRef = *trvItr;
		ESMoL::TypeBaseRef outputTypeBaseRef = ESMoL::TypeBaseRef::Create( outputTypeStruct );
		outputTypeBaseRef.name() = inputTypeBaseRef.name();
		outputTypeBaseRef.MemberIndex() = inputTypeBaseRef.MemberIndex();
		ESMoL::TypeBase inputTypeBase = inputTypeBaseRef.ref();
		outputTypeBaseRef.ref() = copyTypesAux( inputTypeBase, outputTypes );
	}

	return outputTypeStruct;
}


void copyDesignFolders( ESMoL::RootFolder inputRootFolder, ESMoL::RootFolder outputRootFolder ) {
	DesignFolderVector designFolderVector = inputRootFolder.DesignFolder_kind_children();
	for( DesignFolderVector::iterator dfvItr = designFolderVector.begin() ; dfvItr != designFolderVector.end() ; ++dfvItr ) {
		ESMoL::DesignFolder inputDesignFolder = *dfvItr;
		ESMoL::DesignFolder outputDesignFolder = ESMoL::DesignFolder::Create( outputRootFolder );
		outputDesignFolder.name() = inputDesignFolder.name();
		copyModelsFolders( inputDesignFolder, outputDesignFolder );
	}
}

void copyModelsFolders( ESMoL::DesignFolder inputDesignFolder, ESMoL::DesignFolder outputDesignFolder ) {
	ModelsFolderVector modelsFolderVector = inputDesignFolder.ModelsFolder_kind_children();
	for( ModelsFolderVector::iterator mfvItr = modelsFolderVector.begin() ; mfvItr != modelsFolderVector.end() ; ++mfvItr ) {
		ESMoL::ModelsFolder inputModelsFolder = *mfvItr;
		ESMoL::ModelsFolder outputModelsFolder = ESMoL::ModelsFolder::Create( outputDesignFolder );
		outputModelsFolder.name() = inputModelsFolder.name();
		copyStateflows( inputModelsFolder, outputModelsFolder );
		copyDataflows( inputModelsFolder, outputModelsFolder );
	}
}

void copyStateflows( ESMoL::ModelsFolder inputModelsFolder, ESMoL::ModelsFolder outputModelsFolder ) {
	StateflowVector stateflowVector = inputModelsFolder.Stateflow_kind_children();
	for( StateflowVector::iterator sfvItr = stateflowVector.begin() ; sfvItr != stateflowVector.end() ; ++sfvItr ) {
		ESMoL::Stateflow inputStateflow = *sfvItr;
		ESMoL::Stateflow outputStateflow = ESMoL::Stateflow::Create( outputModelsFolder );
		outputStateflow.name() = inputStateflow.name();
		copyStates( inputStateflow, outputStateflow );
	}
}

void copyStates( ESMoL::Stateflow inputStateflow, ESMoL::Stateflow outputStateflow ) {

	getConnectorRefList().clear();
	getTransConnectorMap().clear();

	StateVector stateVector = inputStateflow.State_kind_children();
	for( StateVector::iterator stvItr = stateVector.begin() ; stvItr != stateVector.end() ; ++stvItr ) {
		ESMoL::State inputState = *stvItr;
		ESMoL::State outputState = ESMoL::State::Create( outputStateflow );
		copyState( inputState, outputState );
	}
	for( ConnectorRefList::iterator crlItr = getConnectorRefList().begin() ; crlItr != getConnectorRefList().end() ; ++crlItr ) {
		ESMoL::ConnectorRef inputConnectorRef = *crlItr;
		ESMoL::TransConnector inputTransConnector = inputConnectorRef.ref();
		if ( inputTransConnector == Udm::null ) {
			std::cerr << "Warning:  ConnectorRef is a null reference!" << std::endl;
			continue;
		}

		TransConnectorMap::iterator tcmItr = getTransConnectorMap().find( inputConnectorRef );
		if ( tcmItr == getTransConnectorMap().end() ) {
			std::cerr << "Warning:  ConnectorRef has no copy in TransConnectorMap" << std::endl;
			continue;
		}
		ESMoL::TransConnector outputTransConnector = tcmItr->second;
		if ( outputTransConnector.type() != ESMoL::ConnectorRef::meta ) {
			std::cerr << "Warning:  TransConnector should be a ConnectorRef" << std::endl;
			continue;
		}
		ESMoL::ConnectorRef outputConnectorRef = ESMoL::ConnectorRef::Cast( outputTransConnector );

		tcmItr = getTransConnectorMap().find( inputTransConnector );
		if ( tcmItr == getTransConnectorMap().end() ) {
			std::cerr << "Warning:  TransConnector has no copy in TransConnectorMap" << std::endl;
			continue;
		}
		outputTransConnector = tcmItr->second;

		outputConnectorRef.ref() = outputTransConnector;
	}
}

void copyState( ESMoL::State inputState, ESMoL::State outputState ) {

	UdmEngine::copyState( inputState, outputState );

	TransStartVector transStartVector = inputState.TransStart_kind_children();
	for( TransStartVector::iterator tsvItr = transStartVector.begin() ; tsvItr != transStartVector.end() ; ++tsvItr ) {
		ESMoL::TransStart inputTransStart = *tsvItr;
		ESMoL::TransStart outputTransStart = ESMoL::TransStart::Create( outputState );
		getTransConnectorMap().insert(  std::make_pair( inputTransStart, outputTransStart )  );
	}

	DataVector dataVector = inputState.Data_kind_children();
	for( DataVector::iterator dtvItr = dataVector.begin() ; dtvItr != dataVector.end() ; ++dtvItr ) {
		copyData(  *dtvItr, ESMoL::Data::Create( outputState )  );
	}

	EventVector eventVector = inputState.Event_kind_children();
	for( EventVector::iterator envItr = eventVector.begin() ; envItr != eventVector.end() ; ++envItr ) {
		copyEvent(  *envItr, ESMoL::Event::Create( outputState )  );
	}

	JunctionVector junctionVector = inputState.Junction_kind_children();
	for( JunctionVector::iterator jnvItr = junctionVector.begin() ; jnvItr != junctionVector.end() ; ++jnvItr ) {
		ESMoL::Junction inputJunction = *jnvItr;
		ESMoL::Junction outputJunction = ESMoL::Junction::Create( outputState );
		getTransConnectorMap().insert(  std::make_pair( inputJunction, outputJunction )  );
	}

	StateVector stateVector = inputState.State_kind_children();
	for( StateVector::iterator stvItr = stateVector.begin() ; stvItr != stateVector.end() ; ++stvItr ) {
		ESMoL::State inputSubState = *stvItr;
		ESMoL::State outputSubState = ESMoL::State::Create( outputState );
		getTransConnectorMap().insert(  std::make_pair( inputSubState, outputSubState )  );
		copyState( inputSubState, outputSubState );
	}

	ConnectorRefVector connectorRefVector = inputState.ConnectorRef_kind_children();
	for( ConnectorRefVector::iterator jnvItr = connectorRefVector.begin() ; jnvItr != connectorRefVector.end() ; ++jnvItr ) {
		ESMoL::ConnectorRef inputConnectorRef = *jnvItr;
		ESMoL::ConnectorRef outputConnectorRef = ESMoL::ConnectorRef::Create( outputState );
		getTransConnectorMap().insert(  std::make_pair( inputConnectorRef, outputConnectorRef )  );
		getConnectorRefList().push_back( inputConnectorRef );
	}

	TransitionVector transitionVector = inputState.Transition_kind_children();
	for( TransitionVector::iterator trvItr = transitionVector.begin() ; trvItr != transitionVector.end() ; ++trvItr ) {
		ESMoL::Transition inputTransition = *trvItr;

		ESMoL::TransConnector inputSrcTransConnector = inputTransition.srcTransition_end();
		TransConnectorMap::iterator tcmItr = getTransConnectorMap().find( inputSrcTransConnector );
		if ( tcmItr == getTransConnectorMap().end() ) {
			std::cerr << "WARNING:  transconnector does not map to copy" << std::endl;
			continue;
		}
		ESMoL::TransConnector outputSrcTransConnector = tcmItr->second;

		ESMoL::TransConnector inputDstTransConnector = inputTransition.dstTransition_end();
		tcmItr = getTransConnectorMap().find( inputDstTransConnector );
		if ( tcmItr == getTransConnectorMap().end() ) {
			std::cerr << "WARNING:  transconnector does not map to copy" << std::endl;
			continue;
		}
		ESMoL::TransConnector outputDstTransConnector = tcmItr->second;

		ESMoL::Transition outputTransition = ESMoL::Transition::Create( outputState );
		UdmEngine::copyTransition( inputTransition, outputTransition );

		outputTransition.srcTransition_end() = outputSrcTransConnector;
		outputTransition.dstTransition_end() = outputDstTransConnector;
	}
}

void copyData( ESMoL::Data inputData, ESMoL::Data outputData ) {
	UdmEngine::copyData( inputData, outputData );

	ESMoL::TypeBaseRef inputTypeBaseRef = inputData.TypeBaseRef_child();
	if ( inputTypeBaseRef == Udm::null ) {
		std::cerr << "No TypeBaseRef in Data" << std::endl;
		return;
	}
	ESMoL::TypeBaseRef outputTypeBaseRef = ESMoL::TypeBaseRef::Create( outputData );

	ESMoL::TypeBase inputTypeBase = inputTypeBaseRef.ref();
	if ( inputTypeBase == Udm::null ) {
		std::cerr << "No TypeBase in Data" << std::endl;
		return;
	}

	TypeBaseMap::iterator tbmItr = getTypeBaseMap().find( inputTypeBase );
	if ( tbmItr == getTypeBaseMap().end() ) {
		std::cerr << "input TypeBase does not map to output TypeBase" << std::endl;
		return;
	}

	ESMoL::TypeBase outputTypeBase = tbmItr->second;
	outputTypeBaseRef.name() = inputTypeBaseRef.name();
	outputTypeBaseRef.ref() = outputTypeBase;
}

void copyEvent( ESMoL::Event inputEvent, ESMoL::Event outputEvent ) {
	UdmEngine::copyEvent( inputEvent, outputEvent );

	ESMoL::TypeBaseRef inputTypeBaseRef = inputEvent.TypeBaseRef_child();
	if ( inputTypeBaseRef == Udm::null ) {
		std::cerr << "No TypeBaseRef in Event" << std::endl;
		return;
	}
	ESMoL::TypeBaseRef outputTypeBaseRef = ESMoL::TypeBaseRef::Create( outputEvent );

	ESMoL::TypeBase inputTypeBase = inputTypeBaseRef.ref();
	if ( inputTypeBase == Udm::null ) {
		std::cerr << "No TypeBase in Event" << std::endl;
		return;
	}

	TypeBaseMap::iterator tbmItr = getTypeBaseMap().find( inputTypeBase );
	if ( tbmItr == getTypeBaseMap().end() ) {
		std::cerr << "input TypeBase does not map to output TypeBase" << std::endl;
		return;
	}

	ESMoL::TypeBase outputTypeBase = tbmItr->second;
	outputTypeBaseRef.name() = inputTypeBaseRef.name();
	outputTypeBaseRef.ref() = outputTypeBase;
}

void copyDataflows( ESMoL::ModelsFolder inputModelsFolder, ESMoL::ModelsFolder outputModelsFolder ) {

	getPortList().clear();
	getPortMap().clear();

	DataflowVector dataflowVector = inputModelsFolder.Dataflow_kind_children();
	for( DataflowVector::iterator dfvItr = dataflowVector.begin() ; dfvItr != dataflowVector.end() ; ++dfvItr ) {
		ESMoL::Dataflow inputDataflow = *dfvItr;
		ESMoL::Dataflow outputDataflow = ESMoL::Dataflow::Create( outputModelsFolder );
		outputDataflow.name() = inputDataflow.name();
		copySubsystems_flatten( inputDataflow, outputDataflow );
	}

	for( PortList::iterator ptlItr = getPortList().begin() ; ptlItr != getPortList().end() ; ++ptlItr ) {

		ESMoL::Port inputDstPort = *ptlItr;

		LineSet lineSet = inputDstPort.srcLine();
		if ( lineSet.empty() ) continue;
		ESMoL::Line line = *lineSet.begin();
		ESMoL::Port inputSrcPort = line.srcLine_end();
		lineSet = inputSrcPort.srcLine();
		while( getPortMap().find( inputSrcPort ) == getPortMap().end() && !lineSet.empty() ) {
			line = *lineSet.begin();
			inputSrcPort = line.srcLine_end();
			lineSet = inputSrcPort.srcLine();
		}

		PortMap::iterator ptmItr = getPortMap().find( inputDstPort );
		if ( ptmItr == getPortMap().end() ) {
			std::cerr << "Warning: port not in PortMap" << std::endl;
			continue;
		}
		ESMoL::Port outputDstPort = ptmItr->second;

		ptmItr = getPortMap().find( inputSrcPort );
		if ( ptmItr == getPortMap().end() ) {
			std::cerr << "Warning: port not in PortMap" << std::endl;
			continue;
		}
		ESMoL::Port outputSrcPort = ptmItr->second;

		Udm::Object lineParent = outputSrcPort.GetParent();
		if (  Udm::IsDerivedFrom( outputSrcPort.type(), ESMoL::OutPort::meta )  ) lineParent = lineParent.GetParent();

		ESMoL::Line outputLine = ESMoL::Line::Create( lineParent );
		outputLine.srcLine_end() = outputSrcPort;
		outputLine.dstLine_end() = outputDstPort;
	}
}

void copySubsystems_flatten( ESMoL::Dataflow inputDataflow, ESMoL::Dataflow outputDataflow ) {
	SubsystemVector subsystemVector = inputDataflow.Subsystem_kind_children();
	for( SubsystemVector::iterator ssvItr = subsystemVector.begin() ; ssvItr != subsystemVector.end() ; ++ssvItr ) {
		ESMoL::Subsystem inputSubsystem = *ssvItr;
		if ( inputSubsystem.HasObjectInstances() ) continue;
		ESMoL::Subsystem outputSubsystem = ESMoL::Subsystem::Create( outputDataflow );
		UdmEngine::copyBlock( inputSubsystem, outputSubsystem );
		copyPorts( inputSubsystem, outputSubsystem );
		flattenSubsystem( inputSubsystem, outputSubsystem );
	}
}

void flattenSubsystem( ESMoL::Subsystem inputSubsystem, ESMoL::Subsystem outputSubsystem ) {
	
	PrimitiveVector primitiveVector = inputSubsystem.Primitive_kind_children();
	for( PrimitiveVector::iterator pmvItr = primitiveVector.begin() ; pmvItr != primitiveVector.end() ; ++pmvItr ) {

		ESMoL::Primitive primitive = *pmvItr;
		ESMoL::Block block = UdmEngine::copy( primitive, outputSubsystem );
		copyPorts( primitive, block );
	}

	SubsystemVector subSubsystemVector = inputSubsystem.Subsystem_kind_children();
	for( SubsystemVector::iterator ssvItr = subSubsystemVector.begin() ; ssvItr != subSubsystemVector.end() ; ++ssvItr ) {

		ESMoL::Subsystem inputSubSubsystem = *ssvItr;
		if (  isStateMachine( inputSubSubsystem )  )                  copySubsystem( inputSubSubsystem, outputSubsystem );
		else if (  isConditionActionSubsystem( inputSubSubsystem )  ) copySubsystem( inputSubSubsystem, outputSubsystem, false );
		else                                                          flattenSubsystem( inputSubSubsystem, outputSubsystem );
	}
}

bool isStateMachine( ESMoL::Subsystem subsystem ) {
	ESMoL::Primitive sFunction;

	PrimitiveVector primitiveVector = subsystem.Primitive_kind_children();
	for( PrimitiveVector::iterator pmvItr = primitiveVector.begin() ; pmvItr != primitiveVector.end() ; ++pmvItr ) {

		ESMoL::Primitive primitive = *pmvItr;
		if ( primitive.BlockType() == "S-Function" ) {
			sFunction = primitive;
			break;
		}
	}

	if ( sFunction == Udm::null ) return false;

	ConnectorRefVector connectorRefVector = sFunction.ConnectorRef_kind_children();
	return !connectorRefVector.empty();
}

bool isConditionActionSubsystem( ESMoL::Subsystem subsystem ) {
	ActionPortVector actionPortVector = subsystem.ActionPort_kind_children();
	if ( actionPortVector.size() != 1 ) return false;

	ESMoL::Port port = actionPortVector.front();

	LineSet lineSet = port.srcLine();
	if ( lineSet.size() != 1 ) return false;

	ESMoL::Line line = *lineSet.begin();
	ESMoL::Port srcPort = line.srcLine_end();

	ESMoL::Block block = ESMoL::Block::Cast( srcPort.GetParent() );
	
	return block.BlockType() == "If" || block.BlockType() == "SwitchCase";
}

void copySubsystem( ESMoL::Subsystem inputSubSubsystem, ESMoL::Subsystem outputSubsystem, bool deep ) {

	ESMoL::Subsystem outputSubSubsystem = ESMoL::Subsystem::Create( outputSubsystem );
	UdmEngine::copyBlock( inputSubSubsystem, outputSubSubsystem );
	copyPorts( inputSubSubsystem, outputSubSubsystem );

	PrimitiveVector primitiveVector = inputSubSubsystem.Primitive_kind_children();
	for( PrimitiveVector::iterator pmvItr = primitiveVector.begin() ; pmvItr != primitiveVector.end() ; ++pmvItr ) {

		ESMoL::Primitive primitive = *pmvItr;
		ESMoL::Block block = UdmEngine::copy( primitive, outputSubSubsystem );
		if ( primitive.BlockType() == "S-Function" ) {
			ConnectorRefVector connectorRefVector = primitive.ConnectorRef_kind_children();
			if ( !connectorRefVector.empty() ) {
				ESMoL::ConnectorRef inputConnectorRef = connectorRefVector.back();
				ESMoL::ConnectorRef outputConnectorRef = ESMoL::ConnectorRef::Create( block );
				outputConnectorRef.name() = inputConnectorRef.name();
				ESMoL::TransConnector inputTransConnector = inputConnectorRef.ref();
				TransConnectorMap::iterator tcmItr = getTransConnectorMap().find( inputTransConnector );
				if ( tcmItr != getTransConnectorMap().end() ) outputConnectorRef.ref() = tcmItr->second;
			}
		}
		copyPorts( primitive, block );
	}

	SubsystemVector sub3systemVector = inputSubSubsystem.Subsystem_kind_children();
	for( SubsystemVector::iterator ssvItr = sub3systemVector.begin() ; ssvItr != sub3systemVector.end() ; ++ssvItr ) {

		ESMoL::Subsystem inputSub3system = *ssvItr;
		if ( deep ) copySubsystem( inputSub3system, outputSubSubsystem );
		else        flattenSubsystem( inputSub3system, outputSubSubsystem );
	}
}


void copyPorts( ESMoL::Block inputBlock, ESMoL::Block outputBlock ) {

	PortVector portVector = inputBlock.Port_kind_children();
	for( PortVector::iterator ptvItr = portVector.begin() ; ptvItr != portVector.end() ; ++ptvItr ) {
		ESMoL::Port inputPort = *ptvItr;
		ESMoL::Port outputPort = UdmEngine::copy( inputPort, outputBlock  );
		if ( outputPort == Udm::null ) {
			std::cerr << "Warning: could not copy output port" << std::endl;
			return;
		}

		getPortList().push_back( inputPort );
		getPortMap().insert(  std::make_pair( inputPort, outputPort )  );

		ESMoL::TypeBaseRef inputTypeBaseRef = inputPort.TypeBaseRef_child();
		if ( inputTypeBaseRef != Udm::null ) {
			ESMoL::TypeBaseRef outputTypeBaseRef = ESMoL::TypeBaseRef::Create( outputPort );
			ESMoL::TypeBase inputTypeBase = inputTypeBaseRef.ref();
			if ( inputTypeBase != Udm::null ) {
				TypeBaseMap::iterator tbmItr = getTypeBaseMap().find( inputTypeBase );
				if ( tbmItr != getTypeBaseMap().end() ) {
					outputTypeBaseRef.name() = inputTypeBaseRef.name();
					outputTypeBaseRef.ref() = tbmItr->second;
				}
			}
		}

	}	
}