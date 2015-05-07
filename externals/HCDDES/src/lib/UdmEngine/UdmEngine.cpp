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
 *  FILE NAME:	UdmEngine.cpp 
 *	SYNOPSIS:	Implementation of class "UdmEngine", for setting up a Udm
 *				file to accept the translation of an SL/SF mdl file.
 *  AUTHOR:		Harmon Nine - hnine@isis.vanderbilt.edu
 *  CREATED:	07/20/06
 *  LAST UPDATE: 
 */
#include "UdmEngine.hpp"


void UdmEngine::createNew( const std::string &udmFileName ) {

	setBackend( udmFileName );

	boost::filesystem::path parentPath = boost::filesystem::path( udmFileName ).parent_path();
	if ( !parentPath.empty() ) boost::filesystem::create_directories( parentPath );

	std::string metainfo = SLSF_PARADIGM_NAME;
	if ( _backend == XML ) metainfo += ".xsd";

	_smartDataNetwork.CreateNew( udmFileName.c_str(), metainfo.c_str(), SLSF_ROOT::RootFolder::meta );
	_isClosed = false;
}

void UdmEngine::openExisting( const std::string &udmFileName ) {
	setBackend( udmFileName );
	_smartDataNetwork.OpenExisting( udmFileName, "" );

	if ( !_smartDataNetwork.isOpen() ) throw udm_exception( "Corrupt mga file" );
}

void UdmEngine::closeNoUpdate( void ) {
	if ( _isClosed ) return;

	_backend = UNKNOWN;

	_smartDataNetwork.CloseNoUpdate();
	_isClosed = true;
}

void UdmEngine::closeWithUpdate( void ) {
	if ( _isClosed ) return;

	_backend = UNKNOWN;

	_smartDataNetwork.CloseWithUpdate();
	_isClosed = true;
}

#if SLSF_MODEL_LEVEL == 3
SLSF::ModelsFolder UdmEngine::getModelsFolder( void ) {

	if ( _modelsFolder != Udm::null ) return _modelsFolder;

	SLSF::DesignFolder designFolder;
	DesignFolderVector designFolderVector = getRootFolder().DesignFolder_children();
	for( DesignFolderVector::iterator dfvItr = designFolderVector.begin() ; dfvItr != designFolderVector.end() ; ++dfvItr ) {
		ModelsFolderVector modelsFolderVector = dfvItr->ModelsFolder_children();
		if ( !modelsFolderVector.empty() ) {
			designFolder = *dfvItr;
			break;
		}
	}

	if ( designFolder == Udm::null ) {
		designFolder = SLSF::DesignFolder::Create( getRootFolder() );
		designFolder.name() = "Imported Models";
	}

	StringSet modelNameSet;
	ModelsFolderVector modelsFolderVector = designFolder.ModelsFolder_children();

	for(
	 ModelsFolderVector::iterator mfvItr = modelsFolderVector.begin() ;
	 mfvItr != modelsFolderVector.end() ;
	 ++mfvItr
	) modelNameSet.insert( mfvItr->name() );

	int ix = 0;
	std::string uniqueModelName;
	do {
		std::ostringstream sout;
		sout << "New" << getModelName() << setfill( '0' ) << setw( 8 ) << ix;
		uniqueModelName = sout.str();
		++ix;
	} while(  modelNameSet.find( uniqueModelName ) != modelNameSet.end()  );

	_modelsFolder = SLSF::ModelsFolder::Create( designFolder );
	_modelsFolder.name() = uniqueModelName;

	return _modelsFolder;
}
#endif

SLSF::Matrix UdmEngine::makeDefaultMatrix( void ) {
	SLSF::Matrix ESMoLMatrix = SLSF::Matrix::Create( getTypes() );

	ESMoLMatrix.Type() = "double";
	ESMoLMatrix.rows() = 1;
	ESMoLMatrix.columns() = 1;

	return ESMoLMatrix;
}

std::string UdmEngine::escape_chars( const std::string &str ) {

	std::string escapedChars = "\n\r";

	std::string returnString = str;
	for( unsigned int ix = 0 ; ix < escapedChars.size() ; (void)++ix ) {
		char escapedChar = escapedChars.at( ix );
		std::string escapeSequence;

		std::string newString;
		std::string::size_type pos = 0;
		std::string::size_type newpos;
		while(   (  newpos = returnString.find( escapedChar, pos )  ) != std::string::npos   ) {
			newString += returnString.substr( pos, newpos - pos );
			if ( escapeSequence.empty() ) {
				std::ostringstream newStream;
				newStream << "&#x" << std::hex << std::setw( 2 ) << std::setfill( '0' ) << int( escapedChar ) << ";";
				escapeSequence = newStream.str();
			}
			newString += escapeSequence;
			pos = newpos + 1;
		}
		returnString = newString + returnString.substr( pos );
	}

	return returnString;
}

SLSF::Port UdmEngine::copy( SLSF::Port port, SLSF::Block block ) {

	SLSF::Port otherPort;

	if ( port.type() == SLSF::InputPort::meta ) {

		SLSF::InputPort inputPort = SLSF::InputPort::Cast( port );
		SLSF::InputPort otherInputPort = SLSF::InputPort::Create( block );
		otherInputPort.name() = inputPort.name();
		otherInputPort.Number() = inputPort.Number();

		otherPort = otherInputPort;

	} else if ( port.type() == SLSF::OutputPort::meta ) {

		SLSF::OutputPort outputPort = SLSF::OutputPort::Cast( port );
		SLSF::OutputPort otherOutputPort = SLSF::OutputPort::Create( block );
		otherOutputPort.Number() = outputPort.Number();

		otherPort = otherOutputPort;

	} else if ( port.type() == SLSF::EnablePort::meta ) {

		SLSF::EnablePort enablePort = SLSF::EnablePort::Cast( port );
		SLSF::EnablePort otherEnablePort = SLSF::EnablePort::Create( block );

#if PARADIGM != CyberComposition_PARADIGM
		otherEnablePort.StatesWhenEnabling() = enablePort.StatesWhenEnabling();
#endif

		otherPort = otherEnablePort;

	} else if ( port.type() == SLSF::TriggerPort::meta ) {

		SLSF::TriggerPort triggerPort = SLSF::TriggerPort::Cast( port );
		SLSF::TriggerPort otherTriggerPort = SLSF::TriggerPort::Create( block );
		otherTriggerPort.TriggerType() = triggerPort.TriggerType();

		otherPort = otherTriggerPort;

#if PARADIGM != CyberComposition_PARADIGM
	} else if ( port.type() == SLSF::ActionPort::meta ) {

		SLSF::ActionPort actionPort = SLSF::ActionPort::Cast( port );
		SLSF::ActionPort otherActionPort = SLSF::ActionPort::Create( block );

		otherPort = otherActionPort;

	} else if ( port.type() == SLSF::StatePort::meta ) {

		SLSF::StatePort actionPort = SLSF::StatePort::Cast( port );
		SLSF::StatePort otherStatePort = SLSF::StatePort::Create( block );

		otherPort = otherStatePort;
#endif
	} else {

		std::cerr << "WARNING:  UNKNOWN PORT OF TYPE \"" << port.type().name() << "\" ENCOUNTERED.  CANNOT COPY." << std::endl;

	}

	if ( otherPort != Udm::null ) otherPort.name() = port.name();

	return otherPort;
}

SLSF::Block UdmEngine::copy( SLSF::Block block, SLSF::Subsystem subsystem ) {

	SLSF::Block otherBlock;

	if ( block.type() == SLSF::Primitive::meta ) {
		otherBlock = SLSF::Primitive::Create( subsystem );
	} else if ( block.type() == SLSF::Subsystem::meta ) {
		otherBlock = SLSF::Subsystem::Create( subsystem );
	} else {
		std::cerr << "WARNING:  UNKNOWN BLOCK OF TYPE \"" << block.type().name() << "\" ENCOUNTERED.  CANNOT COPY." << std::endl;
	}

	copyBlock( block, otherBlock );

#if PARADIGM == CyberComposition_PARADIGM
	ParameterVector parameterVector = block.SF_Parameter_kind_children();
#else
	ParameterVector parameterVector = block.Parameter_kind_children();
#endif
	for( ParameterVector::iterator pmvItr = parameterVector.begin() ; pmvItr != parameterVector.end() ; ++pmvItr ) {
		SLSF::Parameter &parameter = *pmvItr;
		SLSF::Parameter otherParameter = SLSF::Parameter::Create( otherBlock );
		otherParameter.name() = parameter.name();
		otherParameter.Value() = parameter.Value();
	}

	return otherBlock;
}

void UdmEngine::copyData( SLSF::Data inputData, SLSF::Data outputData ) {
	outputData.name() = inputData.name();
	outputData.Name() = inputData.Name();
	outputData.Description() = inputData.Description();
	outputData.Scope() = inputData.Scope();
	outputData.DataType() = inputData.DataType();
	outputData.Port() = inputData.Port();
	outputData.Units() = inputData.Units();
	outputData.InitialValue() = inputData.InitialValue();
	outputData.Min() = inputData.Min();
	outputData.Max() = inputData.Max();
	outputData.ArraySize() = inputData.ArraySize();
	outputData.ArrayFirstIndex() = inputData.ArrayFirstIndex();
}

void UdmEngine::copyEvent( SLSF::Event inputEvent, SLSF::Event outputEvent ) {
	outputEvent.name() = inputEvent.name();
	outputEvent.Name() = inputEvent.Name();
	outputEvent.Description() = inputEvent.Description();
	outputEvent.Scope() = inputEvent.Scope();
	outputEvent.Trigger() = inputEvent.Trigger();
	outputEvent.Port() = inputEvent.Port();
}

void UdmEngine::copyMatrix( SLSF::Matrix inputMatrix, SLSF::Matrix outputMatrix ) {
	outputMatrix.name() = inputMatrix.name();
	outputMatrix.Type() = inputMatrix.Type();
	outputMatrix.rows() = inputMatrix.rows();
	outputMatrix.columns() = inputMatrix.columns();
}

void UdmEngine::copyState( SLSF::State inputState, SLSF::State outputState ) {
	outputState.Description() = inputState.Description();
	outputState.name() = inputState.name();
	outputState.Name() = inputState.Name();
	outputState.EnterAction() = inputState.EnterAction();
	outputState.DuringAction() = inputState.DuringAction();
	outputState.Methods() = inputState.Methods();
	outputState.ExitAction() = inputState.ExitAction();
	outputState.Decomposition() = inputState.Decomposition();
	outputState.Order() = inputState.Order();
}

void UdmEngine::copyTransition( SLSF::Transition inputTransition, SLSF::Transition outputTransition ) {
	outputTransition.Trigger() = inputTransition.Trigger();
	outputTransition.Guard() = inputTransition.Guard();
	outputTransition.ConditionAction() = inputTransition.ConditionAction();
	outputTransition.Action() = inputTransition.Action();
	outputTransition.Order() = inputTransition.Order();
}

void UdmEngine::copyBlock( SLSF::Block inputBlock, SLSF::Block outputBlock ) {
	outputBlock.Description() = inputBlock.Description();
	outputBlock.BlockType() = inputBlock.BlockType();
	outputBlock.Name() = inputBlock.Name();
	outputBlock.name() = inputBlock.name();
	outputBlock.Priority() = inputBlock.Priority();
	outputBlock.Tag() = inputBlock.Tag();
	outputBlock.SampleTime() = inputBlock.SampleTime();
	outputBlock.UserData() = inputBlock.UserData();
}

//
// Type Management
//
std::string UdmEngine::getTypeBaseString( SLSF::Matrix matrix ) {
	std::string type = matrix.Type();
	{
		std::string::size_type charPos = 0;
		while(   (  charPos = type.find( "(", charPos )  ) != std::string::npos   ) {
			type.erase( charPos, 1 );
		}
		charPos = 0;
		while(   (  charPos = type.find( ")", charPos )  ) != std::string::npos   ) {
			type.erase( charPos, 1 );
		}
		charPos = 0;
		while(   (  charPos = type.find( "'", charPos )  ) != std::string::npos   ) {
			type.replace( charPos, 1, "_" );
		}
	}

	__int64 rows = matrix.rows();
	__int64 columns = matrix.columns();

	if ( columns != 1 ) {
		return std::string( "A" ) + boost::lexical_cast< std::string >( rows ) + "A" +
		 boost::lexical_cast< std::string >( columns ) + type + "AA";
	}
	if ( rows != 1 ) {
		return std::string( "A" ) + boost::lexical_cast< std::string >( rows ) + type + "A";
	}
	return type;
}


std::string UdmEngine::getTypeBaseString( SLSF::TypeStruct typeStruct ) {

	std::string retval = "S";

	bool addComma = false;
	TypeBaseRefSet typeBaseRefSet =
		typeStruct.TypeBaseRef_children_sorted( TypeBaseRefSorter() );

	for(
	 TypeBaseRefSet::iterator tbsItr = typeBaseRefSet.begin() ;
	 tbsItr != typeBaseRefSet.end() ;
	 (void)++tbsItr
	) {
		if ( addComma ) retval += "C";
		addComma = true;

		retval += tbsItr->name();

		SLSF::TypeBase typeBase = tbsItr->ref();
		if ( typeBase.type() == SLSF::Matrix::meta ) {
			retval += getTypeBaseString(  SLSF::Matrix::Cast( typeBase )  );
		} else if ( typeBase.type() == SLSF::TypeStruct::meta ) {
			retval += getTypeBaseString(  SLSF::TypeStruct::Cast( typeBase )  );
		}
	}

	retval += "S";

	return retval;
}


std::string UdmEngine::getTypeBaseString( const std::string &type, __int64 rows, __int64 columns ) {

	if ( columns != 1 ) {
		return std::string( "A" ) + boost::lexical_cast< std::string >( rows ) + "A" +
		 boost::lexical_cast< std::string >( columns ) + type + "AA";
	}
	if ( rows != 1 ) {
		return std::string( "A" ) + boost::lexical_cast< std::string >( rows ) + type + "A";
	}

	return type;

}

#if PARADIGM == CyberComposition_PARADIGM
SLSF_ROOT::SimulinkWrapper UdmEngine::getRootComponent( void ) {
	return SLSF_ROOT::SimulinkWrapper::Cast( _dataflowParent );
}
#else
SLSF::Dataflow UdmEngine::getDataflow( void ) {

	if ( _dataflowParent != Udm::null ) return SLSF::Dataflow::Cast( _dataflowParent );

	SLSF::Dataflow dataflow;

#if !defined(SLSF_MODEL_LEVEL) || SLSF_MODEL_LEVEL == 1
	DataflowVector dataflowVector = getRootFolder().Dataflow_children();
	if ( dataflowVector.empty() ) {
		dataflow = SLSF::Dataflow::Create( UdmEngine::getRootFolder() );
		dataflow.name() = "Dataflow";
	} else {
		dataflow = *dataflowVector.begin();
	}
#elif SLSF_MODEL_LEVEL == 3
	dataflow = SLSF::Dataflow::Create( getModelsFolder() );
	dataflow.name() = "Dataflow";
#endif

	setDataflowParent( dataflow );

	return dataflow;
}

SLSF::Stateflow UdmEngine::getStateflow( void ) {

	if ( _stateflow != Udm::null ) return _stateflow;

#if !defined(SLSF_MODEL_LEVEL) || SLSF_MODEL_LEVEL == 1
	StateflowVector stateflowVector = getRootFolder().Stateflow_children();
	if ( stateflowVector.empty() ) {
		_stateflow = SLSF::Stateflow::Create( UdmEngine::getRootFolder() );
		_stateflow.name() = "Stateflow";
	} else {
		_stateflow = *stateflowVector.begin();
	}
#elif SLSF_MODEL_LEVEL == 3
	_stateflow = SLSF::Stateflow::Create( getModelsFolder() );
	_stateflow.name() = "Stateflow";
#endif

	return _stateflow;
}

SLSF::State UdmEngine::getTopLevelState( void ) {
	if ( _state != Udm::null ) return _state;

	StateVector stateVector = getStateflow().State_children();
	if ( stateVector.empty() ) {
		_state = SLSF::State::Create( getStateflow() );
		_state.name() = "TopLevel";
		_state.Order() = "0";
	} else {
		_state = *stateVector.begin();
	}
	return _state;
}
#endif

SLSF::Types UdmEngine::getTypes( void ) {

	if ( _types != Udm::null ) return _types;

#if PARADIGM == CyberComposition_PARADIGM
	TypesVector typesVector = getRootFolder().Simulink_Types_children();
#else
	TypesVector typesVector = getRootFolder().Types_children();
#endif
	if ( typesVector.empty() ) {
		_types = SLSF::Types::Create( getRootFolder() );
		_types.name() = "Types";
	} else {
		_types = *typesVector.begin();
	}
	return _types;
}


#if SLSF_MODEL_LEVEL == 3
std::string UdmEngine::getModelName( Udm::Object object ) {
	
	while( object != Udm::null && object.type() != SLSF::ModelsFolder::meta ) {
		object = object.GetParent();
	}

	if ( object != Udm::null ) {
		SLSF::ModelsFolder modelsFolder = SLSF::ModelsFolder::Cast( object );
		return modelsFolder.name();
	}

	return "";
}
#endif

UdmEngine::StringTypeBaseMapInit::StringTypeBaseMapInit( void ) {

	SLSF::Types types = UdmEngine::get_Types();

#if PARADIGM == CyberComposition_PARADIGM
	TypeBaseVector typeBaseVector = types.SF_TypeBase_kind_children();
#else
	TypeBaseVector typeBaseVector = types.TypeBase_kind_children();
#endif

	for( TypeBaseVector::iterator tbvItr = typeBaseVector.begin() ; tbvItr != typeBaseVector.end() ; (void)++tbvItr ) {
		std::string typeName = tbvItr->name();
		if (  isStructName( typeName )  ) _takenStructNames.insert( typeName );
	}
	for( TypeBaseVector::iterator tbvItr = typeBaseVector.begin() ; tbvItr != typeBaseVector.end() ; (void)++tbvItr ) {
		integrateTypeBase( *tbvItr );
	}

	_defaultMatrix = SLSF::Matrix::Create( UdmEngine::get_Types() );
	_defaultMatrix.Type() = "double";
	_defaultMatrix.rows() = 1;
	_defaultMatrix.columns() = 1;

	integrateTypeBase( _defaultMatrix );
}

void UdmEngine::StringTypeBaseMapInit::cleanup( void ) {
	getStringTypeBaseMap().clear();

	SLSF::Types types = UdmEngine::get_Types();

#if PARADIGM == CyberComposition_PARADIGM
	TypeBaseSet typeBaseSet = types.SF_TypeBase_children();
#else
	TypeBaseSet typeBaseSet = types.TypeBase_children();
#endif
	TypeBaseSet::iterator tbsItr = typeBaseSet.begin();
	while( tbsItr != typeBaseSet.end() ) {
		TypeBaseSet::iterator nxtTbsItr = tbsItr;
		(void)++nxtTbsItr;
		TypeBaseRefUnorderedSet typeBaseRefUnorderedSet = tbsItr->referedbyTypeStructRef();
		if ( typeBaseRefUnorderedSet.empty() ) {
			SLSF::TypeBase typeBase = *tbsItr;
			typeBaseSet.erase( tbsItr );
			typeBase.DeleteObject();
		}
		tbsItr = nxtTbsItr;
	}
}

void UdmEngine::StringTypeBaseMapInit::integrateTypeBase( SLSF::TypeBase &typeBase ) {

	if ( typeBase == Udm::null ) return;

	if ( typeBase.type() == SLSF::TypeStruct::meta ) {
		SLSF::TypeStruct typeStruct = SLSF::TypeStruct::Cast( typeBase );
		TypeBaseRefSet typeBaseRefSet = typeStruct.TypeBaseRef_kind_children_sorted( TypeBaseRefSorter() );

		for(
		 TypeBaseRefSet::iterator trsItr = typeBaseRefSet.begin() ;
		 trsItr != typeBaseRefSet.end() ;
		 (void)++trsItr
		) {
			SLSF::TypeBase currentTypeBase = trsItr->ref();
			integrateTypeBase( currentTypeBase );
		}
	}

	std::string typeBaseString = getTypeBaseString( typeBase );
	StringTypeBaseMap::iterator stmItr = getStringTypeBaseMap().find( typeBaseString );

	if ( stmItr != getStringTypeBaseMap().end() ) {
		SLSF::TypeBase otherTypeBase = stmItr->second;
		if ( typeBase != otherTypeBase ) {

			SLSF::TypeBase nullTypeBase;
			TypeBaseRefSet typeBaseRefSet = typeBase.referedbyTypeStructRef_sorted( TypeBaseRefSorter() );
			for(
			 TypeBaseRefSet::iterator trsItr = typeBaseRefSet.begin() ;
			 trsItr != typeBaseRefSet.end() ;
			 (void)++trsItr
			) {
				SLSF::TypeBaseRef typeBaseRef = *trsItr;
				typeBaseRef.ref() = nullTypeBase;
				typeBaseRef.ref() = otherTypeBase;
			}

//			typeBase.DeleteObject();
			typeBase = otherTypeBase;
		}
		return;

	}

	getStringTypeBaseMap()[ typeBaseString ] = typeBase;
	
	if ( typeBase.type() == SLSF::TypeStruct::meta ) {
		std::string typeBaseName = typeBase.name();
		if ( typeBaseName.empty() ) typeBaseName = getNewStructName();
		typeBase.name() = typeBaseName;
		getStringTypeBaseMap()[ typeBaseName ] = typeBase;
	} else {
		typeBase.name() = typeBaseString;
	}

	return;
}
