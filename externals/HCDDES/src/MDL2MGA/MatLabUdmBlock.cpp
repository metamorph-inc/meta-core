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
 *  FILE NAME:	MatLabUdmBlock.cpp 
 *	SYNOPSIS:	Implementation of class "MatLabUdmBlock", translates SL/SF 'Simulink.Block'
 *				objects that are NOT PORTS (i.e. not 'Simulink.Inport', 'Simulink.Outport', etc).
 *				into equivalent GME SLSF Systems/Primitives.					
 *  AUTHOR:		Harmon Nine - hnine@isis.vanderbilt.edu
 *  CREATED:	07/20/06
 *  LAST UPDATE: 
 */

#include <limits>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <fstream>

#include "MatLabUdmBlock.hpp"
#include "MatLabUdmState.hpp"
#include "MatLabUdmTransition.hpp"
#include "MatLabUdmMiscSimulink.hpp"
#include "MatLabUdmMiscStateflow.hpp"
#include "BExprMapBuilder.hpp"
#include "ConfigKeeper.hpp"

#include "BlockChecker.h"

#if PARADIGM == CyberComposition_PARADIGM
#define Port_children SF_Port_children
#define Parameter_kind_children SF_Parameter_kind_children
#define Port_kind_children SF_Port_kind_children
#endif

Udm::Object MatLabUdmBlock::distinguish( Udm::Object &udmParent, const std::string &matLabName, int matLabObjectNo ) {

	std::string matLabObject = getMatLabObjectString( matLabName, matLabObjectNo );

	if ( udmParent.isInstance() ) {
		SLSF::Subsystem subsystem = SLSF::Subsystem::Cast( udmParent );
		BlockMap blockMap = getChildBlockMap( subsystem );
		std::string blockName = trim(  MatLabEngine::globalEngine().getMxStringValue( matLabObject + ".Name" )  );

		BlockMap::iterator bkmItr = blockMap.lower_bound( blockName );
		std::string name = bkmItr->first;
		if (  blockName == name.substr( 0, blockName.size() )  ) return bkmItr->second;

		throw std::runtime_error(
		 std::string( "Block \"" ) + blockName + "\" not found in instance of \"" +
		  static_cast< std::string >( subsystem.name() ) + "\" block."
		);
	}

	std::string blockType = MatLabEngine::globalEngine().getMxStringValue( matLabObject + ".BlockType" );
	transform( blockType.begin(), blockType.end(), blockType.begin(), ToLower() );

	if ( blockType == "subsystem" )	{

		_embeddedBlockName = matLabName + boost::lexical_cast< std::string >( matLabObjectNo ) + "e";

		std::string command = _embeddedBlockName + " = " + matLabObject +
		 ".find( '-isa', 'Stateflow.EMChart', '-depth', 1 )";

		MatLabEngine::globalEngine().executeCommand( command );

		int noCurrentObjects = MatLabEngine::globalEngine().getMxLinearSize( _embeddedBlockName );

		if ( noCurrentObjects == 1 ) {

			_blockType = PRIMITIVE;
			SLSF::Primitive primitive = Primitive::Create( udmParent );

			Parameter parameter = Parameter::Create( primitive );
			parameter.RefId() = getRefId();
			parameter.name() = "Script";
			std::string script = MatLabEngine::globalEngine().getData( _embeddedBlockName + "(1).Script" );
			script = UdmEngine::escape_chars( script );
			parameter.Value() = script;

			return primitive;

		} else {

			_embeddedBlockName.clear();

		}

		_blockType = SYSTEM;
		return Subsystem::Create( udmParent );
	}

	if ( blockType == "reference" )	{ _blockType = REFERENCE;	return Reference::Create( udmParent ); }

	_blockType = PRIMITIVE;
	return Primitive::Create( udmParent );
}

void MatLabUdmBlock::getPortTypes( const std::string &portType ) {

	PortVector portVector;

	if ( portType == "Inport" ) {
		InputPortSet inputPortSet = getUdmBlock().InputPort_kind_children_sorted( InputPortSorter() );
		for( InputPortSet::iterator ipsItr = inputPortSet.begin() ; ipsItr != inputPortSet.end() ; ++ipsItr ) portVector.push_back( *ipsItr );
	} else {
		OutputPortSet outputPortSet = getUdmBlock().OutputPort_kind_children_sorted( OutputPortSorter() );
		for( OutputPortSet::iterator opsItr = outputPortSet.begin() ; opsItr != outputPortSet.end() ; ++opsItr ) portVector.push_back( *opsItr );
	}

	std::string compiledDimensions = MatLabEngine::globalEngine().getData( getMatLabObject() + ".CompiledPortDimensions." + portType );
	std::string compiledDataTypes  = MatLabEngine::globalEngine().getData( getMatLabObject() + ".CompiledPortDataTypes." + portType );


	IntList dimensionList = MatLabUdm::getIntList( compiledDimensions );
	if ( dimensionList.empty() ) return;

	StringList typeList = MatLabUdm::getWordList( compiledDataTypes );

	
	//
	// FOR EACH PORT
	//
	for( PortVector::iterator prvItr = portVector.begin() ; prvItr != portVector.end() ; ++prvItr ) {

		// MAKE SURE PORT HAS A TypeBaseRef CHILD
		SLSF::Port port = *prvItr;
		SLSF::TypeBaseRef typeBaseRef = port.TypeBaseRef_child();
		if ( typeBaseRef == Udm::null ) {
			std::cerr << "ERROR:  " << portType << " PORT \"" << port.getPath( "/" ) << "\" HAS NO TypeBaseRef!" << std::endl;
			continue;
		}

		// THERE SHOULD BE A TYPE LEFT FOR THIS PORT
		if ( typeList.empty() ) {
			std::cerr << "ERROR:  Out of types when attempting to assign type to " << portType << " port \"" <<
			 port.getPath( "/" ) << "\"" << std::endl;
			return;
		}
		std::string type = typeList.front(); typeList.pop_front();
		if ( type == "fcn_call" ) type = "float";
		if ( type == "action" ) type = "int";

		// MAP type TO actualType
		std::string actualType = MatLabUdm::parameterValueSub( type );


		int noDims = 0;
		bool dimError = false;

		// GET NUMBER OF DIMENSIONS FOR THIS PORT
		if ( !dimensionList.empty() ) {
			noDims = dimensionList.front(); dimensionList.pop_front();
			while( noDims < 0 ) {
				if ( dimensionList.empty() ) { dimError = true; break; }
				dimensionList.pop_front();
				if ( dimensionList.empty() ) { dimError = true; break; }
				noDims = dimensionList.front(); dimensionList.pop_front();
			}
		} else {
			dimError = true;
		}

		// GET DIMENSIONS FOR THIS PORT
		int rows = 1;
		int columns = 1;
		if ( !dimError ) {

			// MAKE SURE NUMBER OF DIMENSIONS IS 2 OR LESS
			if ( noDims > 2 ) {
				std::cerr << "ERROR:  Out of dimension specs when attempting to assign type to " << portType <<
				 " port \"" << port.getPath( "/" ) << "\"" << std::endl;
				std::cerr << "WARNING: " << portType << " port \"" << getUdmBlock().getPath( "/" ) <<
				 ": NUMBER OF DIMENSIONS GREATER THAN 2 DETECTED (NOT SUPPORTED)." << std::endl;
				for( int ix = 0 ; !dimensionList.empty() && ix < noDims ; ++ix ) dimensionList.pop_front();
				continue;
			}

			if ( noDims <= 2 ) {
				if ( dimensionList.empty() ) dimError = true;
				else                         { rows = dimensionList.front(); dimensionList.pop_front(); }
			}
			if ( noDims == 2 ) {
				if ( dimensionList.empty() ) dimError = true;
				else                         { columns = dimensionList.front(); dimensionList.pop_front(); }
			}
		}

		if ( dimError ) {
			std::cerr << "ERROR:  Out of dimension specs when attempting to assign type to " << portType <<
			 " port \"" << port.getPath( "/" ) << "\"" << std::endl;
			return;
		}

		SLSF::TypeBase newTypeBase = UdmEngine::getTypeBase( type, rows, columns );
		if ( newTypeBase == Udm::null ) {
			newTypeBase = UdmEngine::createMatrix( type, rows, columns );
		}

		typeBaseRef.name() = newTypeBase.name();
		typeBaseRef.ref() = newTypeBase;
	}
}

void MatLabUdmBlock::getPortTypes( void ) {

	if ( isSpecialBlock() ) {
		getCollectorSet().insert( *this );
		getCollectorBlockSet().insert( getUdmBlock() );
		return;
	}

	getPortTypes( "Inport" );
	getPortTypes( "Outport" );
}

void MatLabUdmBlock::getSampleTime( void ) {

	Block udmBlock = getUdmBlock();

	double sampleTime = MatLabEngine::globalEngine().getMxDoubleValue( getMatLabObject(), "CompiledSampleTime" );

	udmBlock.SampleTime() =  sampleTime;
}

string MatLabUdmBlock::checkValue( void ) {

	Block udmBlock = getUdmBlock();
	std::string checkResult, errMsg;

	BlockChecker bc;
	if ( !bc.CheckBlock( udmBlock, checkResult ) )
	{
		ostringstream outStr;
		outStr << "Bad parameter value(s) in block " << udmBlock.getPath2() << ": " << std::endl;
		outStr << string("    ") << checkResult << std::endl;
		errMsg = outStr.str();
	}

	return errMsg;

}

void MatLabUdmBlock::setupBlock( void ) {

	// GENERIC DATA FOR BLOCKS
	Block udmBlock = getUdmBlock();

	std::string matLabBlockType = MatLabEngine::globalEngine().getMxStringValue( getMatLabObject() + ".BlockType" );
	if ( matLabBlockType == "SubSystem" && udmBlock.type() == SLSF::Primitive::meta ) matLabBlockType = "Embedded";

	udmBlock.BlockType() = matLabBlockType;
	udmBlock.Description() = MatLabEngine::globalEngine().getMxStringValue( getMatLabObject() + ".Description" ).c_str();
	udmBlock.Name() = udmBlock.name() = getUniqueName().c_str();
	udmBlock.Tag() = MatLabEngine::globalEngine().getMxStringValue( getMatLabObject() + ".Tag" ).c_str();
	udmBlock.RefId() = getRefId();


	// NOTE:  BECAUSE PRIORTY IS AN INTEGER, AND SO HAS NO INHERENT VALUE TO INDICATE "NO ASSIGNED PRIORITY"
	// PRIORITY VALUES (P) ARE TRANSLATED FROM MATLAB PRIORITY (M) AS FOLLOWS:
	//
	// IF ( M.NO_ASSIGNED_PRIORITY() ) P = 0
	// ELSE IF ( M > 0 ) P = M
	// ELSE IF ( M <= 0 ) P = M - 1
	//
	// THAT IS, P = 0 MEANS NO ASSIGNED PRIORITY IN ESMoL.  ALL OTHER VALUES INDICATE AN EXPLICIT PRIORITY
	std::string priorityString = MatLabEngine::globalEngine().getMxStringValue( getMatLabObject(), "Priority" );
	int priority;
	if ( priorityString.empty() ) {
		priority = 0;
	} else {
		priority = boost::lexical_cast< int >( priorityString );
		if ( priority <= 0 ) {
			--priority;
		}
	}
	udmBlock.Priority() = priority;
}

bool MatLabUdmBlock::evalTest( const std::string &fieldName ) {

	std::string maskVariables = MatLabEngine::globalEngine().getMxStringValue( getMatLabObject() + ".MaskVariables" );
	if (  maskVariables.find( fieldName + "=&" ) != std::string::npos  ) {
		return false;
	}

	std::string fieldDialogParameters = getMatLabObject() + ".DialogParameters." + fieldName;

	std::string attributesObject = fieldDialogParameters + ".Attributes";
	int noAttributes = MatLabEngine::globalEngine().getMxLinearSize( attributesObject );
	for( int ix = 0 ; ix < noAttributes ; ++ix ) {
		std::string attributeValue =
		 MatLabEngine::globalEngine().getMxStringValue(  attributesObject + "{ " + boost::lexical_cast< std::string >( ix + 1 ) + " }"  );
		if ( attributeValue == "dont-eval" ) {
			return false;
		}
	}


	std::string type = MatLabEngine::globalEngine().getMxStringValue( fieldDialogParameters + ".Type" );

	if ( type != "string" ) {
		return false;
	}

	std::string fieldValue = MatLabEngine::globalEngine().getMxStringValue( getMatLabObject() + "." + fieldName );
	if ( !getMFileScope().isValidMatLabInput( fieldValue ) ) {
		return false;
	}

	boost::trim( fieldValue );

	if (  boost::regex_match( fieldValue, RegexCommon::getIdentifierRegex() )  ) {
		fieldValue = getScopeSymbolTable().getSymbol( fieldValue );
		return MatLabEngine::globalEngine().varExists( fieldValue );
	}

	return true;
}

void MatLabUdmBlock::delayedSetupAux( void ) {

	Block udmBlock = getUdmBlock();
	std::string matLabBlockType = udmBlock.BlockType();
	std::string command;

	bool hasFunctionName = false;

	ParameterMap parameterMap = getParameterMap( udmBlock );

	// GET MASK PARAMETERS FOR SUBSYSTEMS AND PRIMITIVES
	bool maskFlag = MatLabEngine::globalEngine().getMxStringValue( getMatLabObject() + ".Mask" ) == "on";
	_hasMask = maskFlag;
	string maskVariables;
	string maskValueString;

	if ( maskFlag ) {

		getScopeSymbolTable().newScope();

		// GET MASK PARAMETERS
		SymbolSet symbolSet;

		std::string maskNamesObject = getMatLabPrefix() + "n";
		std::string command = maskNamesObject + " = " + getMatLabObject() + ".MaskNames";
		MatLabEngine::globalEngine().executeCommand( command );

		int maskNamesObjectLength = MatLabEngine::globalEngine().getMxLinearSize( maskNamesObject );
		for( int ix = 0 ; ix < maskNamesObjectLength ; ++ix ) {
			std::string maskName = MatLabEngine::globalEngine().getMxStringValue(  maskNamesObject + "{" + boost::lexical_cast< std::string >( ix + 1 ) + "}"  );
			symbolSet.insert( maskName );
		}

		AssignmentList subSystemAssignmentList;

		// CYCLE THROUGH DIALOG PARAMETERS
		std::set< std::string > fieldNames = MatLabEngine::globalEngine().getMxFieldNames( getMatLabObject() + ".DialogParameters" );
		for( std::set< std::string >::iterator fnvItr = fieldNames.begin() ; fnvItr != fieldNames.end() ; ++fnvItr ) {

			const std::string &fieldName( *fnvItr );
			if ( fieldName == "SampleTime" ) continue;

			Parameter parameter;
			ParameterMap::iterator pmpItr = parameterMap.find( fieldName );
			if ( pmpItr != parameterMap.end() ) {
				parameter = pmpItr->second;
			} else if ( udmBlock.isInstance() ) {
				continue;
			} else {
				parameter = Parameter::Create( udmBlock );
				parameter.RefId() = getRefId();
				parameter.name() = fieldName;
			}

			std::string fieldValue = MatLabEngine::globalEngine().getData( getMatLabObject() + '.' + fieldName );
			if (  boost::trim_copy( fieldValue ).empty() || !evalTest( fieldName )  ) {
				fieldValue = boost::regex_replace(  fieldValue, boost::regex( "'" ), "''"  );
				fieldValue = "'" + fieldValue + "'";
			}

			// COLLECT "ASSIGNMENTS" FOR MASKED BLOCKS, IF IT HAS MASK PARAMETERS
			// ASSIGNMENTS ARE EVALUATED LATER, IN PARALLEL, SINCE THERE MAY BE LOCAL OVERRIDES,
			// I.E. VARIABLES OF THE SAME NAME AT DIFFERENT (MORE LOCAL) SCOPES.
			if (  symbolSet.find( fieldName ) != symbolSet.end()  ) {

				Assignment assignment;
				assignment.setVarName( fieldName );
				assignment.setRHS( fieldValue );
				subSystemAssignmentList.push_back( assignment );

			}

			// PARAMETER VALUE *MUST* BE SET HERE -- "fieldValue" MAY HAVE BEEN MODIFIED IN ABOVE IF-STATEMENT
			parameter.Value() = fieldValue; // NOT SET TO VALUE EVALUATED BY MATLAB (NO REAL NEED)

			if (  fieldName == "FunctionName" && fieldValue.substr( 0, 3 ) != "sf_"  ) hasFunctionName = true;
 
		}

		// PERFORM MASK PARAMETER ASSIGNMENTS IN PARALLEL
		performParallelAssignment( subSystemAssignmentList );

		performMaskInitialization( *this );
	}

	// TURN OFF THE MASK FOR MASKED PRIMITIVIES SO THAT NATIVE DIALOG PARAMETERS CAN BE ACCESSED
	if ( maskFlag && matLabBlockType != "SubSystem" && matLabBlockType != "Embedded" && matLabBlockType != "S-Function" ) {
		std::string command = getMatLabObject() + ".LinkStatus = 'none'";
		MatLabEngine::globalEngine().executeCommand( command, true );

		maskVariables = MatLabEngine::globalEngine().getMxStringValue( getMatLabObject() + ".MaskVariables" );
		maskValueString = MatLabEngine::globalEngine().getMxStringValue( getMatLabObject() + ".MaskValueString" );

		command = getMatLabObject() + ".Mask = 'off'";
		// FIXME: fails on libraries
		MatLabEngine::globalEngine().executeCommand( command, true );
		maskFlag = false;
	}

	if ( !maskFlag ) {

		// CYCLE THROUGH DIALOG PARAMETERS
		std::set< std::string > fieldNames = MatLabEngine::globalEngine().getMxFieldNames( getMatLabObject() + ".DialogParameters" );
//		fieldNames.insert( "SampleTime" ); // PLACE "SampleTime" IN SET IF IT'S NOT THERE ALREADY

		for( std::set< std::string >::iterator fnvItr = fieldNames.begin() ; fnvItr != fieldNames.end() ; ++fnvItr ) {

			const std::string &fieldName( *fnvItr );

			Parameter parameter;
			if ( fieldName != "SampleTime" ) {
				ParameterMap::iterator pmpItr = parameterMap.find( fieldName );
				if ( pmpItr != parameterMap.end() ) {
					parameter = pmpItr->second;
				} else if ( udmBlock.isInstance() ) {
					continue;
				} else {
					parameter = Parameter::Create( udmBlock );
					parameter.RefId() = getRefId();
					parameter.name() = fieldName;
				}
			}

			std::string fieldValue = MatLabEngine::globalEngine().getData( getMatLabObject() + '.' + fieldName );

			if (  !boost::trim_copy( fieldValue ).empty() && evalTest( fieldName ) && matLabBlockType != "Goto" && matLabBlockType != "From"  ) {
				fieldValue = getScopeSymbolTable().performSimpleSubstitution( fieldValue );
				// BETTER TO USE A UNIQUE VARIABLE NAME FOR EXPRESSION EVALUATION, INSTEAD OF "ans"
				std::string varName = MatLabEngine::globalEngine().getUniqueVarName();
				MatLabEngine::globalEngine().executeCommand( varName + " = " + fieldValue );
				fieldValue = MatLabEngine::globalEngine().getData( varName );
				MatLabEngine::globalEngine().clearVar( varName );
			}

			// PARAMETER VALUE *MUST* BE SET HERE -- "fieldValue" MAY HAVE BEEN MODIFIED IN ABOVE IF-STATEMENT
			if ( fieldName == "SampleTime" ) {
				
				// We need to get the post-compilation sample time instead
				//double sampleTime = 0;
				//if ( fieldValue == "inf" ) {
				//	sampleTime = numeric_limits< double >::max();
				//} else if (  boost::regex_match( fieldValue, RegexCommon::getFloatingPointRegex() )  ) {
				//	sampleTime = boost::lexical_cast< double >( fieldValue );
				//} else {
				//	std::cerr << "WARNING:  Could not resolve \"SampleType\" (\"" << fieldValue << "\") for block \"" << MatLabUdmBlock::getBlockPath( udmBlock ) << "\"" << std::endl;
				//	sampleTime = -2;
				//}
				//udmBlock.SampleTime() = sampleTime;
			} else {
				parameter.Value() = fieldValue;
			}

			if (  fieldName == "FunctionName" && fieldValue.substr( 0, 3 ) != "sf_"  ) hasFunctionName = true;
		}
	}

	// TURN MASK BACK ON SO MODEL CAN BE COMPILED
	if ( _hasMask && matLabBlockType != "SubSystem" && matLabBlockType != "Embedded" && matLabBlockType != "S-Function" ) {

		std::string command = getMatLabObject() + ".Mask = 'on'";
		MatLabEngine::globalEngine().executeCommand( command, true );

		command = getMatLabObject() + ".MaskVariables = '" + maskVariables + "'";
		MatLabEngine::globalEngine().executeCommand( command, true );

		command = getMatLabObject() + ".MaskValueString = '" + maskValueString + "'";
		MatLabEngine::globalEngine().executeCommand( command, true );
	}

	if ( !udmBlock.isInstance() && matLabBlockType == "S-Function" && hasFunctionName ) {
		getMFileSFuncs().push_back( *this );
	}

	MatLabUdm::buildUdm(  *this, MatLabUdmPort::Factory( _embeddedBlockName )  );

	setupPorts();

	getMatLabUdmBlockVector().push_back( *this );

	if ( !udmBlock.isInstance() ) {

		ParameterMap parameterMap = getParameterMap( udmBlock );
		ParameterMap::iterator prmItr = parameterMap.find( "ParamDataTypeStr" );
		if ( prmItr == parameterMap.end() ) {
			SLSF::Parameter parameter = SLSF::Parameter::Create( udmBlock );
			parameter.RefId() = getRefId();
			parameter.name() = "ParamDataTypeStr";
			parameter.Value() = MatLabUdm::parameterValueSub( "double" );
		} else {
			SLSF::Parameter parameter = prmItr->second;
			std::string dataType = parameter.Value();
			if (  inherited( dataType )  ) parameter.Value() = MatLabUdm::parameterValueSub( "double" );
			else                           parameter.Value() = MatLabUdm::parameterValueSub( dataType );
		}

		if ( matLabBlockType == "Goto" ) { addToGotoBlockMap(); }
		if ( matLabBlockType == "From" ) { addToFromBlockMap(); }
		if ( matLabBlockType == "GotoTagVisibility" ) { addToGotoTagVisibilityBlockMap(); }
	}
}


void MatLabUdmBlock::setupPorts() {

	std::string command;
	SLSF::Block udmBlock = getUdmBlock();

	// GET PORTS
	InputPortSet inputPortSet   = udmBlock.InputPort_kind_children_sorted( InputPortSorter() );
	OutputPortSet outputPortSet = udmBlock.OutputPort_kind_children_sorted( OutputPortSorter() );
	EnablePortVector enablePortVector   = udmBlock.EnablePort_kind_children();
	TriggerPortVector triggerPortVector = udmBlock.TriggerPort_kind_children();

#if PARADIGM == CyberComposition_PARADIGM
	ActionPortVector actionPortVector   = udmBlock.EnablePort_kind_children();
#else
	ActionPortVector actionPortVector   = udmBlock.ActionPort_kind_children();
	StatePortVector statePortVector     = udmBlock.StatePort_kind_children();
#endif

	int blockHandle = MatLabEngine::globalEngine().getMxIntValue( getMatLabObject() + ".Handle" );

	std::string portConns = getMatLabPrefix() + "o";
	command = portConns + " = " + getMatLabObject() + ".PortConnectivity";
	MatLabEngine::globalEngine().executeCommand( command );

	int noPortConns = MatLabEngine::globalEngine().getMxLinearSize( portConns );

	InputPortSet::iterator inpItr      = inputPortSet.begin();
	OutputPortSet::iterator otpItr     = outputPortSet.begin();
	EnablePortVector::iterator enpItr  = enablePortVector.begin();
	TriggerPortVector::iterator tgpItr = triggerPortVector.begin();
	ActionPortVector::iterator acpItr  = actionPortVector.begin();

#if PARADIGM != CyberComposition_PARADIGM
	StatePortVector::iterator stpItr   = statePortVector.begin();
#endif

	int noInputPorts = 0, noOutputPorts = 0; 

	for( int portConnNo = 0 ; portConnNo < noPortConns ; ++portConnNo ) {

		std::string portConn = portConns + '(' + boost::lexical_cast< std::string >( portConnNo + 1 ) + ')';

		std::string portType = MatLabEngine::globalEngine().getMxStringValue( portConn + ".Type" );
		bool srcFlag = MatLabEngine::globalEngine().getMxLinearSize( portConn + ".SrcBlock" )  !=  0;

		Port port;
		std::string otherPortType;

		int portNo = atoi( portType.c_str() );
		if ( portNo > 0 ) {
			if ( srcFlag ) {
				if ( inputPortSet.empty() ) {
					InputPort inputPort = InputPort::Create( udmBlock );
					inputPort.RefId() = getRefId();
					
					SLSF::TypeBaseRef typeBaseRef = SLSF::TypeBaseRef::Create( inputPort );
					typeBaseRef.name() = "undefined";

					inputPort.Number() = portNo;
					inputPort.name() = std::string( "In_" ) + boost::lexical_cast< std::string >( portNo );
					port = inputPort;
				} else {
					port = *inpItr++;
				}
				otherPortType = "Src";
			} else {
				if ( outputPortSet.empty() )	{
					OutputPort outputPort = OutputPort::Create( udmBlock );
					outputPort.RefId() = getRefId();
					
					SLSF::TypeBaseRef typeBaseRef = SLSF::TypeBaseRef::Create( outputPort );
					typeBaseRef.name() = "undefined";

					outputPort.Number() = portNo;
					outputPort.name() = std::string( "Out_" ) + boost::lexical_cast< std::string >( portNo );
					port = outputPort;
				} else {
					port = *otpItr++;
				}
				otherPortType = "Dst";
			}
		} else if ( portType == "enable" ) {
			if ( enablePortVector.empty() ) {
				port = EnablePort::Create( udmBlock );
				port.RefId() = getRefId();
				
				SLSF::TypeBaseRef typeBaseRef = SLSF::TypeBaseRef::Create( port );
				typeBaseRef.name() = "undefined";

				port.name() = "Enable";
			} else {
				port = *enpItr++;
			}
			otherPortType = "Src";

		} else if ( portType == "trigger" ) {
			if ( triggerPortVector.empty() ) {
				port = TriggerPort::Create( udmBlock );
				port.RefId() = getRefId();
				
				SLSF::TypeBaseRef typeBaseRef = SLSF::TypeBaseRef::Create( port );
				typeBaseRef.name() = "undefined";

				port.name() = "Trigger";
			} else {
				port = *tgpItr++;
			}
			otherPortType = "Src";

		} else if ( portType == "ifaction" ) {
			if ( actionPortVector.empty() ) {
#if PARADIGM == CyberComposition_PARADIGM
				port = EnablePort::Create( udmBlock );
#else
				port = ActionPort::Create( udmBlock );
#endif
				port.RefId() = getRefId();

				SLSF::TypeBaseRef typeBaseRef = SLSF::TypeBaseRef::Create( port );
				typeBaseRef.name() = "undefined";

				port.name() = "Action";
			} else {
				port = *acpItr++;
			}
			otherPortType = "Src";
#if PARADIGM == CyberComposition_PARADIGM
		}
#else
		} else {  // "state"
			if ( statePortVector.empty() ) {
				port = StatePort::Create( udmBlock );
				port.RefId() = getRefId();
				
				SLSF::TypeBaseRef typeBaseRef = SLSF::TypeBaseRef::Create( port );
				typeBaseRef.name() = "undefined";

				port.name() = "State";
			} else {
				port = *stpItr++;
			}
			otherPortType = "Dst";
		}
#endif

		if ( srcFlag ) {
			getUdmSocketMap()[ UdmSocket( blockHandle, noInputPorts++, INPUTPORT ) ] = port;
		} else {
			getUdmSocketMap()[ UdmSocket( blockHandle, noOutputPorts++, OUTPUTPORT ) ] = port;
		}

		connectPort( port, srcFlag, portConn, otherPortType );
	}

}


MatLabUdm::SmartPointer MatLabUdmBlock::Factory::create( Udm::Object &udmParent, const std::string &matLabName, int matLabObjectNo ) const {

	std::string command;

	std::string matLabPrefix = getMatLabPrefixString( matLabName, matLabObjectNo );
	std::string matLabObject = getMatLabObjectString( matLabName, matLabObjectNo );

	std::string blockType = MatLabEngine::globalEngine().getMxStringValue( matLabObject + ".BlockType" );
	transform( blockType.begin(), blockType.end(), blockType.begin(), ToLower() );

	std::string referenceName = blockType == "modelreference" ? "ModelFile" : "ReferenceBlock";
	std::string referenceBlock = MatLabEngine::globalEngine().getMxStringValue( matLabObject, referenceName );

	MatLabEngine::globalEngine().setMxStringValue( matLabObject, "LinkStatus", "none" );

	if (
	 ConfigKeeper::getSingleton().getCount( "unique" ) > 0 ||
	 referenceBlock.empty() ||
	 ( blockType != "subsystem" && blockType != "modelreference" ) ||
	 udmParent.isInstance()
	) {
		return MatLabUdm::SmartPointer(  new MatLabUdmBlock( udmParent, matLabName, matLabObjectNo )  );
	}

	ReferenceMap::iterator mrmItr = getReferenceMap().find( referenceBlock );
	if ( mrmItr != getReferenceMap().end() ) {
		return MatLabUdm::SmartPointer(  new MatLabUdmBlock( udmParent, mrmItr->second, matLabName, matLabObjectNo )  );
	}

	int matLabRefObjectNo = getSubSystemIndex();

	std::string matLabRefObject = getMatLabObjectString( "s", matLabRefObjectNo );
	MatLabUdm::SmartPointer blockSP;
	SLSF::Subsystem subsystem;

	if ( blockType == "modelreference" ) {

		boost::filesystem::path modelFilePath_mdl2mga = MatLabUdm::getTopLevelModelManager().registerModel( referenceBlock );

#if BOOST_VERSION / 100 % 1000 < 46
		std::string command = matLabObject + ".ModelFile = '" + modelFilePath_mdl2mga.filename() + "'";
		MatLabEngine::globalEngine().executeCommand( command );

		command = matLabObject + ".ModelName = '" + modelFilePath_mdl2mga.stem() + "'";
		MatLabEngine::globalEngine().executeCommand( command );

		command = "load_system( '" + modelFilePath_mdl2mga.string() + "' )";
		MatLabEngine::globalEngine().executeCommand( command );

		command = matLabRefObject + " = r.find( '-isa', 'Simulink.BlockDiagram', '-and', 'name', '" + modelFilePath_mdl2mga.stem() + "' )";
		MatLabEngine::globalEngine().executeCommand( command );
#else
		std::string command = matLabObject + ".ModelFile = '" + modelFilePath_mdl2mga.filename().string() + "'";
		MatLabEngine::globalEngine().executeCommand( command );

		command = matLabObject + ".ModelName = '" + modelFilePath_mdl2mga.stem().string() + "'";
		MatLabEngine::globalEngine().executeCommand( command );

		command = "load_system( '" + modelFilePath_mdl2mga.string() + "' )";
		MatLabEngine::globalEngine().executeCommand( command );

		command = matLabRefObject + " = r.find( '-isa', 'Simulink.BlockDiagram', '-and', 'name', '" + modelFilePath_mdl2mga.stem().string() + "' )";
		MatLabEngine::globalEngine().executeCommand( command );
#endif
		blockSP = MatLabUdm::SmartPointer(  new MatLabUdmModel( UdmEngine::get_singleton().getDataflowParent(), "s", matLabRefObjectNo )  );
		subsystem = boost::static_pointer_cast< MatLabUdmModel >( blockSP )->getUdmModel();

	} else {

		command = matLabRefObject + " = " + matLabObject;
		MatLabEngine::globalEngine().executeCommand( command );

		blockSP = MatLabUdm::SmartPointer(  new MatLabUdmBlock( UdmEngine::get_singleton().getDataflowParent(), "s", matLabRefObjectNo )  );
		subsystem = boost::static_pointer_cast< MatLabUdmBlock >( blockSP )->getUdmSystem();

	}

	constructBlocks( blockSP );
	getReferenceMap().insert(   std::make_pair(  referenceBlock, boost::static_pointer_cast< MatLabUdmBlock >( blockSP )->getUdmSystem()  )   );

	return MatLabUdm::SmartPointer(  new MatLabUdmBlock( udmParent, subsystem, matLabName, matLabObjectNo ) );
}


bool MatLabUdmBlock::Factory::exclude( const std::string &matLabObject ) const {

	std::string blockType = MatLabEngine::globalEngine().getMxStringValue( matLabObject + ".BlockType" );
	transform( blockType.begin(), blockType.end(), blockType.begin(), ToLower() );

	if ( blockType != "subsystem" ) return false;

	std::string matLabName( "portTest" );
	std::string command = matLabName + " = " + matLabObject + ".find( " + MatLabUdmPort::getMatLabType() + ", '-depth', 1 )";
	MatLabEngine::globalEngine().executeCommand( command );

	int noPorts = MatLabEngine::globalEngine().getMxLinearSize( matLabName );

	command = std::string( "clear " ) + matLabName;
	MatLabEngine::globalEngine().executeCommand( command );

	if ( noPorts > 0 ) return false;

	matLabName = "blockTest";
	command = matLabName + " = " + matLabObject + ".find( " + MatLabUdmBlock::getMatLabType() + ")";
	MatLabEngine::globalEngine().executeCommand( command );
	command = matLabName + " = setdiff(" + matLabName + ',' + matLabObject + ')';
	MatLabEngine::globalEngine().executeCommand( command );
	
	int noBlocks = MatLabEngine::globalEngine().getMxLinearSize( matLabName );

	bool retval = true;
	for( int ix = 0 ; ix < noBlocks ; ++ix ) {
		std::string blockType = MatLabEngine::globalEngine().getMxStringValue( matLabName + "(" + boost::lexical_cast< std::string >( ix + 1 ) + ").BlockType" );
		transform( blockType.begin(), blockType.end(), blockType.begin(), ToLower() );

		if ( blockType != "subsystem" )	{
			retval = false;
			break;
		}
	}

	command = std::string( "clear " ) + matLabName;
	MatLabEngine::globalEngine().executeCommand( command );

	return retval;
}

void MatLabUdmBlock::constructBlocks( MatLabUdm::SmartPointer &matLabUdmParentSP ) {

	// GET BLOCKS
	MatLabUdm::SPVector blockVector = MatLabUdm::buildUdm( matLabUdmParentSP, MatLabUdmBlock::Factory() );
	for( MatLabUdm::SPVector::iterator spvItr = blockVector.begin() ; spvItr != blockVector.end() ; ++spvItr ) {
		constructBlocks( *spvItr );
		(*spvItr)->cleanup();
	}

	// GET CHARTS
	if (  matLabUdmParentSP->getUdmObject().isInstance() || !MatLabUdm::hasChildren( matLabUdmParentSP, MatLabUdmChart::Factory() )  ) {
		return;
	}

	MatLabUdm::SPVector recurseVector = MatLabUdm::buildUdm( matLabUdmParentSP, MatLabUdmChart::Factory() );
	MatLabUdmTransition::getTransitionParents().add( recurseVector );

	for( MatLabUdm::SPVector::iterator spvItr = recurseVector.begin() ; spvItr != recurseVector.end() ; ++spvItr ) {
		MatLabUdmState::constructStates( *spvItr );
	}

}

std::string MatLabUdmBlock::zeroSubstitution( std::string value ) {
	static boost::regex varAssignRegex(
	 "(?:[0-9]+\\.?[0-9]*|[0-9]*\\.?[0-9]+)(?:[Ee][+-][0-9]+)?", boost::regex_constants::perl
	);
	boost::match_results< std::string::const_iterator > results;

	const std::string &constValue = value;
	std::string newValue;

	while (  regex_search( value, results, varAssignRegex )  ) {

		newValue += results.prefix();

		std::string numberString( results[0].first, results[0].second );
		double number = boost::lexical_cast< double >( numberString );

		if ( number < getEPS() ) {
			// GET RID OF '-' SIGN, IF NECESSARY
			if ( !newValue.empty() && newValue.substr( newValue.size() - 1 ) == "-" ) {
				newValue = newValue.substr( 0, newValue.size() - 1 );
			}
			newValue += "0";
		} else {
			newValue += numberString;
		}

		value = results.suffix();
	}

	newValue += value;

	return newValue;
}

void MatLabUdmBlock::kludgeMFileSFuncs( void ) {

	MFileSFuncs &mFileSFuncs = getMFileSFuncs();

	for( MFileSFuncs::iterator msfItr = mFileSFuncs.begin() ; msfItr != mFileSFuncs.end() ; ++msfItr ) {

		std::string subSystemName;

		Block udmBlock = msfItr->getUdmBlock();
		ParameterMap parameterMap = getParameterMap( udmBlock );

		ParameterMap::iterator pmmItr = parameterMap.find( "FunctionName" );
		if ( pmmItr != parameterMap.end() ) subSystemName = pmmItr->second.Value();

		Subsystem parentSystem = Subsystem::Cast( udmBlock.parent() );

		// FIND THE SUBSYSTEM.  IF IT DOESN'T EXIST, ABORT.
		Block subSystem;
		{
			BlockVector blockVector = parentSystem.Block_kind_children();
			for(
			 BlockVector::iterator blvItr = blockVector.begin() ;
			 blvItr != blockVector.end() ;
			(void)++blvItr
			) {
				if (
				 static_cast< std::string >( blvItr->BlockType() ) == "SubSystem" &&
				 static_cast< std::string >( blvItr->Name() ) == subSystemName
				) {
					subSystem = *blvItr;
					break;
				}
			}
		}
		if ( subSystem == Udm::null ) continue;

		// FIND MUX INPUT TO THE udmBlock.  IF IT DOESN'T EXIST, ABORT.
		Block mux;
		{
			InputPortSet inputPortSet = udmBlock.InputPort_kind_children_sorted( InputPortSorter() );
			if ( inputPortSet.empty() ) continue;

			Port inputPort = Port::Cast( *inputPortSet.begin() );

			LineSet lineSet = inputPort.srcLine();
			Line line = *lineSet.begin();

			OutputPort muxOutputPort = OutputPort::Cast( line.srcLine_end() );

			mux = Block::Cast( muxOutputPort.GetParent() );
			if (  static_cast< std::string >( mux.BlockType() ) != "Mux"  ) continue;
		}

		// FIND DEMUX OUTPUT TO THE udmBlock.  IF IT DOESN'T EXIST, ABORT.
		Block demux;
		{
			OutputPortSet outputPortSet = udmBlock.OutputPort_kind_children_sorted( OutputPortSorter() );
			if ( outputPortSet.empty() ) continue;

			Port outputPort = Port::Cast( *outputPortSet.begin() );

			LineSet lineSet = outputPort.dstLine();
			Line line = *lineSet.begin();

			InputPort demuxInputPort = InputPort::Cast( line.dstLine_end() );

			demux = Block::Cast( demuxInputPort.GetParent() );
			if (  static_cast< std::string >( demux.BlockType() ) != "Demux"  ) continue;
		}

		InputPortSet muxInputPortSet = mux.InputPort_kind_children_sorted( InputPortSorter() );
		InputPortSet subSystemInputPortSet = subSystem.InputPort_kind_children_sorted( InputPortSorter() );

		InputPortSet::iterator misItr = muxInputPortSet.begin();
		InputPortSet::iterator sisItr = subSystemInputPortSet.begin();

		while( misItr != muxInputPortSet.end() ) {

			LineSet lineSet = misItr->srcLine();
			for( LineSet::iterator lneItr = lineSet.begin() ; lneItr != lineSet.end() ; ++lneItr ) {
				Line line = Line::Create( parentSystem );
				line.RefId() = getRefId();
				line.srcLine_end() = lneItr->srcLine_end();
				line.dstLine_end() = *sisItr;
			}

			++misItr; ++sisItr;
		}

		OutputPortSet demuxOutputPortSet = demux.OutputPort_kind_children_sorted( OutputPortSorter() );
		OutputPortSet subSystemOutputPortSet = subSystem.OutputPort_kind_children_sorted( OutputPortSorter() );

		OutputPortSet::iterator dosItr = demuxOutputPortSet.begin();
		OutputPortSet::iterator sosItr = subSystemOutputPortSet.begin();

		while( dosItr != demuxOutputPortSet.end() ) {

			LineSet lineSet = dosItr->dstLine();
			for( LineSet::iterator lneItr = lineSet.begin() ; lneItr != lineSet.end() ; ++lneItr ) {
				Line line = Line::Create( parentSystem );
				line.RefId() = getRefId();
				line.srcLine_end() = *sosItr;
				line.dstLine_end() = lneItr->dstLine_end();
			}

			++dosItr; ++sosItr;
		}

		mux.DeleteObject();
		demux.DeleteObject();
		udmBlock.DeleteObject();
	}
}


void MatLabUdmBlock::addToBlockMap( GotoFromBlockMap &gotoFromBlockMap ) {

	SLSF::Block gotoFromBlock = getUdmBlock();

	ParameterMap parameterMap = getParameterMap( gotoFromBlock );
	std::string gotoTag = parameterMap[ "GotoTag" ].Value();

	GotoFromBlockMap::iterator gfbItr = gotoFromBlockMap.find( gotoTag );
	if ( gfbItr == gotoFromBlockMap.end() ) {
		gotoFromBlockMap.insert(  std::make_pair( gotoTag, BlockList() )  );
		gfbItr = gotoFromBlockMap.find( gotoTag );
	}
	BlockList &blockList = gfbItr->second;
	blockList.push_back( gotoFromBlock );
}


void MatLabUdmBlock::processGotoFromBlocks( void ) {

	GotoFromBlockMap &fromBlockMap = getFromBlockMap();

	for( GotoFromBlockMap::iterator gfmItr = fromBlockMap.begin() ; gfmItr != fromBlockMap.end() ; (void)++gfmItr ) {
		std::string gotoTag = gfmItr->first;
		BlockList &fromBlockList = gfmItr->second;

		for(
		 BlockList::iterator fblItr = fromBlockList.begin() ;
		 fblItr != fromBlockList.end() ;
		 (void)++fblItr
		) {
			SLSF::Block fromBlock = *fblItr;
			if (  noLines( fromBlock )  ) continue;

			SLSF::Block gotoBlock;
			SLSF::Block lcaBlock;
			getGotoAndLCABlocks( gotoTag, fromBlock, gotoBlock, lcaBlock );
			if (  gotoBlock == Udm::null || noLines( gotoBlock )  ) continue;
			if (  lcaBlock == Udm::null ) {
				std::cerr << "WARNING:  LCA (Least Common Ancestor) for Goto/From blocks with tag \"" << gotoTag <<
					"\" does not exist!" << std::endl;
				continue;
			}

			SLSF::Port gotoSourcePort = getGotoSourcePort( gotoBlock, lcaBlock );
			PortSet fromPortSet = getFromSourcePort( fromBlock, lcaBlock );
			for_each(  fromPortSet.begin(), fromPortSet.end(), PortConnector( gotoSourcePort, lcaBlock )  );
		}
	}

	deleteBlocks( getFromBlockMap() );
	deleteBlocks( getGotoBlockMap() );
	deleteBlocks( getGotoTagVisibilityBlockMap() );
}

void MatLabUdmBlock::getGotoAndLCABlocks( const std::string &gotoTag, SLSF::Block fromBlock, SLSF::Block &gotoBlock, SLSF::Block &lcaBlock ) {

	GotoFromBlockMap::iterator gbmItr = getGotoBlockMap().find( gotoTag );
	if ( gbmItr == getGotoBlockMap().end() ) return;

	BlockList &gotoBlockList = gbmItr->second;

	int maxLCALevel = 0;

	for(
	 BlockList::iterator gblItr = gotoBlockList.begin() ;
	 gblItr != gotoBlockList.end() ;
	 (void)++gblItr
	) {
		SLSF::Block localGotoBlock = *gblItr;

		ParameterMap parameterMap = getParameterMap( localGotoBlock );
		std::string tagVisibility = parameterMap[ "TagVisibility" ].Value();

		if ( fromBlock.GetParent() == localGotoBlock.GetParent() ) {
			gotoBlock = localGotoBlock;
			lcaBlock = SLSF::Block::Cast( localGotoBlock.GetParent() );
			return;
		}

		// HERE, WE KNOW THE From AND Goto BLOCKS AREN'T IN THE SAME SUBSYSTEM
		if ( tagVisibility == "local" ) continue;

		SLSF::Block localLCABlock = getLCABlock( fromBlock, localGotoBlock );
		if (  tagVisibility == "scoped" && !isGotoVisible( localLCABlock, gotoTag )  ) continue;

		int localLCALevel = getBlockLevel( localLCABlock );
		if ( localLCALevel > maxLCALevel ) {
			gotoBlock = localGotoBlock;
			lcaBlock = localLCABlock;
			maxLCALevel = localLCALevel;
		}
	}

}


// LCA = LEAST COMMON ANCESTOR
SLSF::Block MatLabUdmBlock::getLCABlock( SLSF::Block fromBlock, SLSF::Block gotoBlock ) {

	BlockList fromBlockList = getAncestors( fromBlock );
	BlockList gotoBlockList = getAncestors( gotoBlock );

	BlockList::iterator fblItr = fromBlockList.begin();
	BlockList::iterator gblItr = gotoBlockList.begin();

	while( fblItr != fromBlockList.end() && gblItr != gotoBlockList.end() && *fblItr == *gblItr ) {
		(void)++fblItr; (void)++gblItr;
	}

	return fblItr == fromBlockList.begin() ? SLSF::Block() : *--fblItr;
}


bool MatLabUdmBlock::isGotoVisible( SLSF::Block localLCABlock, const std::string &gotoTag ) {
	GotoFromBlockMap::iterator gfmItr = getGotoTagVisibilityBlockMap().find( gotoTag );
	if ( gfmItr == getGotoTagVisibilityBlockMap().end() ) return false;
	BlockList &gotoTagVisibilityBlockList = gfmItr->second;

	for(
	 BlockList::iterator bklItr = gotoTagVisibilityBlockList.begin() ;
	 bklItr != gotoTagVisibilityBlockList.end() ;
	 (void)++bklItr
	) {
		Udm::Object parentBlock = bklItr->GetParent();
		Udm::Object ancestorBlock = localLCABlock;
		while(  ancestorBlock != Udm::null && Udm::IsDerivedFrom( ancestorBlock.type(), SLSF::Block::meta )  ) {
			if ( parentBlock == ancestorBlock ) return true;
			ancestorBlock = ancestorBlock.GetParent();
		}
	}
	return false;
}


SLSF::Port MatLabUdmBlock::getGotoSourcePort( SLSF::Block gotoBlock, SLSF::Block lcaBlock ) {

	PortSet portSet = gotoBlock.Port_children();
	SLSF::Port port = *portSet.begin();

	LineSet lineSet = port.srcLine();
	portSet = for_each( lineSet.begin(), lineSet.end(), LineSrcPortTrans() ).getSrcPortSet();

	SLSF::Port gotoSourcePort = getNonFromBlockPort( portSet );

	PortSet outPortSet;

	SLSF::Block sourcePortLineBlock = SLSF::Block::Cast( gotoSourcePort.GetParent() );
	if (  Udm::IsDerivedFrom( gotoSourcePort.type(), SLSF::InPort::meta )  ) {
		if ( sourcePortLineBlock == lcaBlock ) return gotoSourcePort;
		outPortSet = getConnectedOutPorts( gotoSourcePort );
		if ( outPortSet.empty() ) {
			outPortSet.insert(  makeNewOutputPort( gotoSourcePort, sourcePortLineBlock )  );
		}
	} else {
		outPortSet.insert( gotoSourcePort );
	}

	sourcePortLineBlock = SLSF::Block::Cast( sourcePortLineBlock.GetParent() );

	while( sourcePortLineBlock != lcaBlock ) {
		PortSet lagOutPortSet = outPortSet;
		outPortSet = getConnectedOutPorts( outPortSet ); 
		if ( outPortSet.empty() ) {
			outPortSet.insert(  makeNewOutputPort( *lagOutPortSet.begin(), sourcePortLineBlock )  );
		}
		Udm::Object parent = sourcePortLineBlock.GetParent();
		if (  !Udm::IsDerivedFrom( parent.type(), SLSF::Block::meta )  ) {
			std::cerr << "WARNING: Non-block encountered in block heirarchy when trying to resolve Goto block" <<
			 std::endl;
			return SLSF::OutputPort();
		}
		sourcePortLineBlock = SLSF::Block::Cast( parent );
	}

	return *outPortSet.begin();
}


SLSF::Port MatLabUdmBlock::makeNewOutputPort( SLSF::Port port, SLSF::Block block ) {
	OutputPortSet outputPortSet = block.OutputPort_kind_children_sorted( OutputPortSorter() );
	SLSF::OutputPort newOutputPort = SLSF::OutputPort::Create( block );
	newOutputPort.RefId() = getRefId();

	SLSF::TypeBaseRef typeBaseRef = SLSF::TypeBaseRef::Create( newOutputPort );
	typeBaseRef.name() = "undefined";

	int newPortNo = (int) outputPortSet.size() + 1;
	newOutputPort.Number() = newPortNo;
	newOutputPort.name() = std::string( "FG_" ) + boost::lexical_cast< std::string >( newPortNo );

	SLSF::Line newLine = SLSF::Line::Create( block );
	newLine.RefId() = getRefId();
	newLine.srcLine_end() = port;
	newLine.dstLine_end() = newOutputPort;

	return newOutputPort;
}

// A "GOTO" BLOCK CAN POTENTIALLY BE FED BY A FROM BLOCK.  THE MEANS USED TO ELIMINATE GOTO AND FROM
// BLOCKS IN THIS PROGRAM ALWAYS MAKES IT SO THAT THE GOTO BLOCK IS:
// A) FED FROM A SINGLE PORT, WHICH IS NOT PART OF A FROM BLOCK
// B) FED BY TWO PORTS, ONE OF WHICH IS PART OF A FROM BLOCK
// IN CASE B), WE WANT THE PORT THAT IS NOT PART OF A FROM BLOCK
SLSF::Port MatLabUdmBlock::getNonFromBlockPort( PortSet &portSet ) {
	PortSet::iterator prsItr = portSet.begin();
	if ( portSet.size() == 1 ) return *prsItr;

	SLSF::Block block = SLSF::Block::Cast( prsItr->GetParent() );
	if (  static_cast< std::string >( block.BlockType() ) == "From"  ) ++prsItr;
	return *prsItr;
}


MatLabUdm::PortSet MatLabUdmBlock::getFromSourcePort( SLSF::Block fromBlock, SLSF::Block lcaBlock ) {

	SLSF::Block sourcePortLineBlock = SLSF::Block::Cast( fromBlock.GetParent() );

	PortSet portSet = fromBlock.Port_children();
	SLSF::Port fromSourcePort = *portSet.begin();

	LineSet lineSet = fromSourcePort.dstLine();
	portSet = for_each( lineSet.begin(), lineSet.end(), LineDstPortTrans() ).getDstPortSet();

	if ( sourcePortLineBlock == lcaBlock ) return portSet;

	InputPortSet inputPortSet = sourcePortLineBlock.InputPort_kind_children_sorted( InputPortSorter() );
	SLSF::InputPort inputPort = SLSF::InputPort::Create( sourcePortLineBlock );
	inputPort.RefId() = getRefId();

	SLSF::TypeBaseRef typeBaseRef = SLSF::TypeBaseRef::Create( inputPort );
	typeBaseRef.name() = "undefined";

	int newPortNo = (int) inputPortSet.size() + 1;
	inputPort.Number() = newPortNo;
	inputPort.name() = std::string( "FG_" ) + boost::lexical_cast< std::string >( newPortNo );

	for_each(  portSet.begin(), portSet.end(), PortConnector( inputPort, sourcePortLineBlock )  );

	fromSourcePort = inputPort;
	sourcePortLineBlock = SLSF::Block::Cast( sourcePortLineBlock.GetParent() );

	while( sourcePortLineBlock != lcaBlock ) {
		fromSourcePort = makeNewInputPort( fromSourcePort, sourcePortLineBlock );
		sourcePortLineBlock = SLSF::Block::Cast( sourcePortLineBlock.GetParent() );
	}

	portSet.clear();
	portSet.insert( fromSourcePort );
	return portSet;
}


SLSF::Port MatLabUdmBlock::makeNewInputPort( SLSF::Port port, SLSF::Block block ) {
	InputPortSet inputPortSet = block.InputPort_kind_children_sorted( InputPortSorter() );
	SLSF::InputPort newInputPort = SLSF::InputPort::Create( block );
	newInputPort.RefId() = getRefId();

	SLSF::TypeBaseRef typeBaseRef = SLSF::TypeBaseRef::Create( newInputPort );
	typeBaseRef.name() = "undefined";

	int newPortNo = (int) inputPortSet.size() + 1;
	newInputPort.Number() = newPortNo;
	newInputPort.name() = std::string( "FG_" ) + boost::lexical_cast< std::string >( newPortNo );

	SLSF::Line newLine = SLSF::Line::Create( block );
	newLine.RefId() = getRefId();
	newLine.srcLine_end() = newInputPort;
	newLine.dstLine_end() = port;

	return newInputPort;
}


MatLabUdm::PortSet MatLabUdmBlock::getConnectedOutPorts( PortSet &portSet ) {
	PortSet outPortSet;
	for( PortSet::iterator ptsItr = portSet.begin() ; ptsItr != portSet.end() ; (void)++ptsItr ) {
		LineSet lineSet = ptsItr->dstLine();
		PortSet localPortSet = for_each( lineSet.begin(), lineSet.end(), LineDstOutPortTrans() ).getDstPortSet();
		outPortSet.insert( localPortSet.begin(), localPortSet.end() );
	}
	return outPortSet;
}


bool MatLabUdmBlock::noLines( SLSF::Block block ) {
	PortSet portSet = block.Port_children();
	if ( portSet.empty() ) return true;

	SLSF::Port port = *portSet.begin();
	LineSet lineSet =
		static_cast< std::string >( block.BlockType() ) == "Goto" ? port.srcLine() : port.dstLine();

	return lineSet.empty();
}


void MatLabUdmBlock::deleteBlocks( GotoFromBlockMap &gotoFromBlockMap ) {

	while( !gotoFromBlockMap.empty() ) {

		BlockList &blockList = gotoFromBlockMap.begin()->second;

		while( !blockList.empty() ) {
			SLSF::Block block = *blockList.begin();

			PortSet portSet = block.Port_children();
			for( PortSet::iterator ptsItr = portSet.begin() ; ptsItr != portSet.end() ; (void)++ptsItr ) {
				SLSF::Port port = *ptsItr;
				LineSet lineSet = port.dstLine();
				while( !lineSet.empty() ) {
					SLSF::Line line = *lineSet.begin();
					line.DeleteObject();
					lineSet.erase( lineSet.begin() );
				}
				lineSet = port.srcLine();
				while( !lineSet.empty() ) {
					SLSF::Line line = *lineSet.begin();
					line.DeleteObject();
					lineSet.erase( lineSet.begin() );
				}
			}
			block.DeleteObject();
			blockList.erase( blockList.begin() );
		}

		gotoFromBlockMap.erase( gotoFromBlockMap.begin() );
	}
}


void MatLabUdmBlock::propagateType( SLSF::TypeBase typeBase, SLSF::Port port ) {

	if ( typeBase == Udm::null ) {
		typeBase = UdmEngine::getDefaultMatrix();
	}

	SLSF::TypeBaseRef typeBaseRef = port.TypeBaseRef_child();

	// SAFE CAST
	SLSF::Port portArchetype = SLSF::Port::Cast(  getTopLevelArchetype( port )  );
	SLSF::TypeBaseRef archetypeTypeBaseRef = portArchetype.TypeBaseRef_child();

	if (  SLSF::TypeBase::Cast( archetypeTypeBaseRef.ref() ) == Udm::null  ) {
		archetypeTypeBaseRef.ref() = typeBase;
		archetypeTypeBaseRef.name() = typeBase.name();
	} else if (  SLSF::TypeBase::Cast( archetypeTypeBaseRef.ref() ) != typeBase  ) {
		typeBaseRef.ref() = typeBase;
		typeBaseRef.name() = typeBase.name();
	}

	// SAFE CAST
	LineSet lineSet = port.dstLine();
	for( LineSet::iterator lnsItr = lineSet.begin() ; lnsItr != lineSet.end() ; ++lnsItr ) {
		propagateType( typeBase, lnsItr->dstLine_end() );
	}

}

void MatLabUdmBlock::propagateType( SLSF::Port port ) {
	SLSF::TypeBaseRef typeBaseRef = port.TypeBaseRef_child();
	propagateType( typeBaseRef.ref(), port );
}

void MatLabUdmBlock::propagateTypes( void ) {

	SubsystemSet subsystemSet = getTopLevelSubsystems();
	for( SubsystemSet::iterator sssItr = subsystemSet.begin() ; sssItr != subsystemSet.end() ; ++sssItr ) {
		topLevelPropagateTypes( *sssItr );
	}
}

bool MatLabUdmBlock::hasObjectInstances( SLSF::Subsystem subsystem ) {

	Udm::Object parent = subsystem.parent();
	if ( parent.type() == SLSF::Subsystem::meta ) {
		return true; // NOT TOPLEVEL SUBSYSTEM -- IGNORE
	}

	SubsystemSet subsystemSet = subsystem.Instances();
	for( SubsystemSet::iterator sssItr = subsystemSet.begin() ; sssItr != subsystemSet.end() ; ++sssItr ) {
		Udm::Object ancestor = sssItr->GetParent();
		while( ancestor.type() == SLSF::Subsystem::meta ) {
			ancestor = ancestor.GetParent();
		}
		if ( ancestor == parent ) return true;
	}

	return false;
}

//
// THE TYPE OF THE INPUT PORTS OF TOPLEVEL SUBSYSTEMS MUST BE PROPAGATED.  ALL OTHER
// (NON-TOPLEVEL) BLOCKS HAVE ONLY THE TYPE OF THEIR OUTPUT PORTS PROPAGATED.
//
void MatLabUdmBlock::topLevelPropagateTypes( SLSF::Subsystem subsystem ) {
	if (  hasObjectInstances( subsystem )  ) return;

	InPortSet inPortSet = subsystem.InPort_kind_children();
	for( InPortSet::iterator ipsItr = inPortSet.begin() ; ipsItr != inPortSet.end() ; ++ipsItr ) {
		SLSF::TypeBaseRef typeBaseRef = ipsItr->TypeBaseRef_child();
		SLSF::TypeBase typeBase = typeBaseRef.ref();
		if ( typeBase == Udm::null ) {
			std::cerr << "ERROR:  UPPER LEVEL INPUT PORT WITHOUT TYPE DETECTED!" << std::endl;
			continue;
		}
		propagateType( typeBase, *ipsItr );
	}

	propagateTypes( subsystem );
}

void MatLabUdmBlock::propagateTypes( SLSF::Subsystem subsystem ) {
	PrimitiveSet primitiveSet = subsystem.Primitive_kind_children();
	for( PrimitiveSet::iterator prsItr = primitiveSet.begin() ; prsItr != primitiveSet.end() ; ++prsItr ) {
		propagateTypes( *prsItr );
	}

	SubsystemSet subsystemSet = subsystem.Subsystem_kind_children();
	for( SubsystemSet::iterator sssItr = subsystemSet.begin() ; sssItr != subsystemSet.end() ; ++sssItr ) {
		propagateTypes( *sssItr );
	}
}

void MatLabUdmBlock::propagateTypes( SLSF::Primitive primitive ) {
	std::string blockType = primitive.BlockType();

	//
	// TYPES ARE PROPAGATED FROM OUTPUT PORTS DOWNSTREAM TO INPUT PORTS
	//
	OutputPortSet outputPortSet = primitive.OutputPort_kind_children_sorted( OutputPortSorter() );
	for( OutputPortSet::iterator opsItr = outputPortSet.begin() ; opsItr != outputPortSet.end() ; ++opsItr ) {

		SLSF::OutputPort outputPort = *opsItr;
		SLSF::TypeBaseRef typeBaseRef = outputPort.TypeBaseRef_child();

		if (  typeBaseRef == Udm::null || SLSF::TypeBase::Cast( typeBaseRef.ref() ) == Udm::null  ) {
			SLSF::Primitive archetype = SLSF::Primitive::Cast(  getTopLevelArchetype( primitive )  );
			if ( getCollectorBlockSet().find( archetype ) == getCollectorBlockSet().end() ) {
				std::cerr << "ERROR:  OUTPUT PORT \"" << static_cast< std::string >( outputPort.name() ) << "\" ON BLOCK \"" <<
				 getBlockPath( primitive ) << "\" HAS NO TYPE: CANNOT PROPAGATE" << std::endl;
			}
		} else {
			SLSF::TypeBase typeBase = typeBaseRef.ref();
			propagateType( typeBase, *opsItr );
		}
	}

}

void MatLabUdmBlock::propagateArchetypeOutPorts( SLSF::OutPort outPort ) {
	LineSet lineSet = outPort.dstLine();
	for( LineSet::iterator lnsItr = lineSet.begin() ; lnsItr != lineSet.end() ; ++lnsItr ) {
		propagateType( lnsItr->dstLine_end() );
	}

	OutPortSet outPortSet = outPort.Instances();
	for( OutPortSet::iterator opsItr = outPortSet.begin() ; opsItr != outPortSet.end() ; ++opsItr ) {
		propagateArchetypeOutPorts( *opsItr );
	}
}

void MatLabUdmBlock::propagateArchetypeOutPorts( void ) {

	SubsystemSet subsystemSet = getTopLevelSubsystems();
	for( SubsystemSet::iterator sssItr = subsystemSet.begin() ; sssItr != subsystemSet.end() ; ++sssItr ) {

		OutPortSet outPortSet = sssItr->OutPort_kind_children();
		for( OutPortSet::iterator opsItr = outPortSet.begin() ; opsItr != outPortSet.end() ; ++opsItr ) {
			propagateArchetypeOutPorts( *opsItr );
		}
	}
}

void MatLabUdmBlock::processCollectors( void ) {

	unsigned int oldNoCollectors = 0;

	// GET TYPES OF S-FUNCTIONS FIRST, SINCE THEY CAN INCUR BACK-PROPAGATION OF TYPES
	while( !getCollectorSet().empty() && oldNoCollectors != getCollectorSet().size() ) {

		oldNoCollectors = (int) getCollectorSet().size();

		MatLabUdmBlockSet::iterator blsItr = getCollectorSet().begin();
		while( blsItr != getCollectorSet().end() ) {
			MatLabUdmBlockSet::iterator nxtBlsItr = blsItr;
			++nxtBlsItr;
			const MatLabUdmBlock &matLabUdmBlock = *blsItr;
			if (  static_cast< std::string >( matLabUdmBlock.getUdmBlock().BlockType() ) == "S-Function" && callSpecialBlockFunction( matLabUdmBlock )  ) {
				getCollectorSet().erase( blsItr );
			}
			blsItr = nxtBlsItr;
		}
	}

	oldNoCollectors = 0;
	while( !getCollectorSet().empty() && oldNoCollectors != getCollectorSet().size() ) {

		oldNoCollectors = (int) getCollectorSet().size();

		MatLabUdmBlockSet::iterator blsItr = getCollectorSet().begin();
		while( blsItr != getCollectorSet().end() ) {
			MatLabUdmBlockSet::iterator nxtBlsItr = blsItr;
			++nxtBlsItr;
			if (  callSpecialBlockFunction( *blsItr )  ) {
				getCollectorSet().erase( blsItr );
			}
			blsItr = nxtBlsItr;
		}
	}


}

bool MatLabUdmBlock::getInputTypes( SLSF::Block block, TypeBaseVector &typeBaseVector ) {

	InputPortSet inPortSet = block.InputPort_kind_children_sorted( InputPortSorter() );

	// INPUT PORTS SHOULD NOW ALL HAVE THEIR TYPE AS A TYPEBASEREF CHILD
	for( InputPortSet::iterator ipsItr = inPortSet.begin() ; ipsItr != inPortSet.end() ; ++ipsItr ) {
		SLSF::TypeBaseRef typeBaseRef = ipsItr->TypeBaseRef_child();
		if (  typeBaseRef == Udm::null || SLSF::TypeBase( typeBaseRef.ref() ) == Udm::null  ) return false;
		typeBaseVector.push_back( typeBaseRef.ref() );
	}

	return true;
}

void MatLabUdmBlock::propagateTypesToArchetypes( SLSF::Block blockInstance ) {
	PortSet portSet = blockInstance.Port_children();
	for( PortSet::iterator prsItr = portSet.begin() ; prsItr != portSet.end() ; ++prsItr ) {
		SLSF::TypeBaseRef typeBaseRefInstance = prsItr->TypeBaseRef_child();
		SLSF::TypeBaseRef typeBaseRef = typeBaseRefInstance.Archetype();
		typeBaseRef.ref() = typeBaseRefInstance.ref();
		typeBaseRef.name() = typeBaseRefInstance.name();
	}
	if ( blockInstance.type() == SLSF::Subsystem::meta ) {
		BlockSet blockSet = SLSF::Subsystem::Cast( blockInstance ).Block_children();
		for( BlockSet::iterator bksItr = blockSet.begin() ; bksItr != blockSet.end() ; ++bksItr ) {
			propagateTypesToArchetypes( *bksItr );
		}
	}
}

void MatLabUdmBlock::propagateTypesToArchetypes( void ) {

	SubsystemSet subsystemSet = getTopLevelSubsystems();
	for( SubsystemSet::iterator sssItr = subsystemSet.begin() ; sssItr != subsystemSet.end() ; ++sssItr ) {
		SLSF::Subsystem subsystem = *sssItr;
		if ( subsystem.isInstance() || !subsystem.HasObjectInstances() ) continue;
		SubsystemSet subsystemInstanceSet = subsystem.Instances();
		subsystemInstanceSet.erase( subsystem );
		propagateTypesToArchetypes( *subsystemInstanceSet.begin() );
	}
}

void MatLabUdmBlock::checkTypes( SLSF::Block block ) {

	SLSF::Block blockInstance;
	PortSet portSet = block.Port_children();

	bool first = true;
	for( PortSet::iterator prsItr = portSet.begin() ; prsItr != portSet.end() ; ++prsItr ) {
		SLSF::Port port = *prsItr;
		SLSF::TypeBaseRef typeBaseRef = port.TypeBaseRef_child();
		if (  typeBaseRef == Udm::null || SLSF::TypeBase::Cast( typeBaseRef.ref() ) == Udm::null  ) {
			if ( first ) {
				first = false;
				std::cerr << block.getPath( "/" ) << ":" << std::endl;
			}
			std::cerr << "\t" << port.type().name() << "\" PORT \"" << port.name() << "\" HAS NO TYPE." << std::endl;
		}
	}
	if ( block.type() == SLSF::Subsystem::meta ) {
		SLSF::Subsystem subsystem = SLSF::Subsystem::Cast( block );
		BlockSet blockSet = subsystem.Block_kind_children();
		for( BlockSet::iterator bksItr = blockSet.begin() ; bksItr != blockSet.end() ; ++bksItr ) {
			checkTypes( *bksItr );
		}
	}
}

void MatLabUdmBlock::checkTypes( void ) {

	SubsystemSet subsystemSet = getTopLevelSubsystems();
	for( SubsystemSet::iterator sssItr = subsystemSet.begin() ; sssItr != subsystemSet.end() ; ++sssItr ) {
		checkTypes( *sssItr );
	}
}

MatLabUdmBlock::ParameterMap MatLabUdmBlock::getParameterMap( SLSF::Block &block ) {
	ParameterMap parameterMap;

	ParameterVector parameterVector = block.Parameter_kind_children();
	for(
	 ParameterVector::iterator prvItr = parameterVector.begin() ;
	 prvItr != parameterVector.end() ;
	 (void)++prvItr
	) {
		parameterMap[ static_cast< std::string >( prvItr->name() ) ] = *prvItr;
	}

	return parameterMap;
}

SLSF::TypeStruct MatLabUdmBlock::typeBundle( const TypeBaseVector &typeBaseVector ) {

	SLSF::TypeStruct typeStruct = SLSF::TypeStruct::Create( UdmEngine::getUdmTypesRoot() );
	typeStruct.MemberCount() = typeBaseVector.size();

	int memberIndex = 0;
	for(
	 TypeBaseVector::const_iterator tbvCit = typeBaseVector.begin() ;
	 tbvCit != typeBaseVector.end() ;
	 (void)++tbvCit
	) {
		SLSF::TypeBaseRef typeBaseRef = SLSF::TypeBaseRef::Create( typeStruct );
		typeBaseRef.MemberIndex() = memberIndex;
		typeBaseRef.ref() = *tbvCit;
		typeBaseRef.name() = std::string( "signal" ) + boost::lexical_cast< std::string >( memberIndex + 1 );

		(void)++memberIndex;
	}

	UdmEngine::integrateTypeBase( typeStruct );

	return typeStruct;
}

bool MatLabUdmBlock::makeMuxType( MatLabUdmBlock &matLabUdmBlock ) {

	SLSF::Block block = matLabUdmBlock.getUdmBlock();

	TypeBaseVector inputTypeBaseVector;
	if (  !getInputTypes( block, inputTypeBaseVector )  ) return false;

	std::string useBusObject = MatLabEngine::globalEngine().getMxStringValue( matLabUdmBlock.getMatLabObject() + ".UseBusObject" );

	SLSF::TypeStruct typeStruct;
	if ( useBusObject == "off" ) {
		typeStruct = typeBundle( inputTypeBaseVector );
	} else {
		std::string busObject = MatLabEngine::globalEngine().getMxStringValue( matLabUdmBlock.getMatLabObject() + ".BusObject" );
		typeStruct = makeBusObjectTypeStruct( busObject );
	}

	OutputPortSet outputPortSet = block.OutputPort_kind_children_sorted( OutputPortSorter() );
	propagateType( typeStruct, *outputPortSet.begin() );

	return true;
}

MatLabUdmBlock::TypeBaseVector MatLabUdmBlock::getMatrixVector( SLSF::TypeBase typeBase ) {

	if ( typeBase == Udm::null ) return TypeBaseVector();

	TypeBaseVector typeBaseVector;
	if ( typeBase.type() == SLSF::Matrix::meta ) {

		// SAFE CAST
		typeBaseVector.push_back(  SLSF::Matrix::Cast( typeBase )  );

	} else {

		// SAFE CAST -- CAN ONLY BE TypeStruct
		SLSF::TypeStruct typeStruct = SLSF::TypeStruct::Cast( typeBase );
		TypeBaseRefSet typeBaseRefSet = typeStruct.TypeBaseRef_children_sorted( TypeBaseRefSorter() );

		for(
		 TypeBaseRefSet::iterator trsItr = typeBaseRefSet.begin() ;
		 trsItr != typeBaseRefSet.end() ;
		 (void)++trsItr
		) {
			TypeBaseVector childTypeBaseVector = getMatrixVector( trsItr->ref() );
			typeBaseVector.insert( typeBaseVector.end(), childTypeBaseVector.begin(), childTypeBaseVector.end() );
		}
	}

	return typeBaseVector;
}

bool MatLabUdmBlock::makeDemuxType( MatLabUdmBlock &matLabUdmBlock ) {

	SLSF::Block block = matLabUdmBlock.getUdmBlock();

	TypeBaseVector inputTypeBaseVector;
	if (  !getInputTypes( block, inputTypeBaseVector )  ) return false;

	TypeBaseVector typeBaseVector = getMatrixVector( *inputTypeBaseVector.begin() );

	OutputPortSet outputPortSet = block.OutputPort_kind_children_sorted( OutputPortSorter() );

	int noInputs = (int) typeBaseVector.size();
	int noOutputs = (int) outputPortSet.size();

	ParameterMap parameterMap = getParameterMap( block );

	std::string outputSpec = parameterMap[ "Outputs" ].Value();

	IntList outputSizeList = getIntList( outputSpec );
	if ( outputSizeList.empty() ) {
		std::cerr << "ERROR:  Demux \"" << block.name() << "\":  BAD OUTPUT SPEC: " << outputSpec << std::endl;
		return false;
	}

	// MANUFACTURE outputSizeList IF outputSizeList CURRENTLY CONTAINS A SCALAR.
	if (  outputSizeList.size() == 1 && outputSpec.find( "[" ) == std::string::npos  ) {
		// SHOULD BE THAT *outputSizeList.begin() == noOutputs
		outputSizeList.clear();
		for( int ix = 0 ; ix < noOutputs ; ++ix ) outputSizeList.push_back( -1 );
	}

	// REPLACE -1'S
	IntListItrList negOneItrList;
	int noInputsAccountedFor = 0;
	for( IntList::iterator oslItr = outputSizeList.begin() ; oslItr != outputSizeList.end() ; ++oslItr ) {
		if ( *oslItr > 0 ) noInputsAccountedFor += *oslItr;
		else               negOneItrList.push_back( oslItr );
	}
	int inputsLeft = noInputs - noInputsAccountedFor;

	int inputsPerOutput = inputsLeft / (int) negOneItrList.size();
	int noLargerOutputs = inputsLeft % negOneItrList.size();
	IntListItrList::iterator illItr = negOneItrList.begin();
	int ix = 0;
	while( ix < noLargerOutputs ) {
		**illItr++ = inputsPerOutput + 1;
		++ix;
	}
	while( ix < noOutputs ) {
		**illItr++ = inputsPerOutput;
		++ix;
	}

	// MANUFACTURE OUTPUT TYPES
	OutputPortSet::iterator opsItr = outputPortSet.begin();
	IntList::iterator otwItr = outputSizeList.begin();
	TypeBaseVector::iterator tbvItr = typeBaseVector.begin();
	SLSF::TypeBase typeBase;

	while( otwItr != outputSizeList.end() ) {

		if ( *otwItr == 1 ) {

			typeBase = *tbvItr;
			(void)++tbvItr;

		} else {

			typeBase = typeBundle(  TypeBaseVector( tbvItr, tbvItr + *otwItr )  );
			tbvItr += *otwItr;

		}

		propagateType( typeBase, *opsItr );

		(void)++opsItr; (void)++otwItr;
	}

	return true;
}

bool MatLabUdmBlock::makeBusSelectorType( MatLabUdmBlock &matLabUdmBlock ) {

	SLSF::Block block = matLabUdmBlock.getUdmBlock();

	TypeBaseVector inputTypeBaseVector;
	if (  !getInputTypes( block, inputTypeBaseVector )  ) return false;

	SLSF::TypeBase inputTypeBase = *inputTypeBaseVector.begin();

	OutputPortSet outputPortSet = block.OutputPort_kind_children_sorted( OutputPortSorter() );

	ParameterMap parameterMap = getParameterMap( block );

	std::string inputSignals = parameterMap[ "InputSignals" ].Value();

	TypeBaseVector outputTypeBaseVector = BExprMapBuilder::getOutputVector(
	 inputSignals, inputTypeBase, parameterMap[ "OutputSignals" ].Value()
	);

	std::string value = parameterMap[ "OutputAsBus" ].Value();
	if ( value == "on" ) {

		SLSF::OutputPort outputPort = *outputPortSet.begin();
		SLSF::TypeStruct typeStruct = typeBundle( outputTypeBaseVector );
		propagateType( typeStruct, outputPort );

	} else {

		TypeBaseVector::iterator tbvItr = outputTypeBaseVector.begin();
		OutputPortSet::iterator opsItr = outputPortSet.begin();

		while( opsItr != outputPortSet.end() ) {
			propagateType( *tbvItr, *opsItr );
			(void)++tbvItr; (void)++opsItr;
		}
	}

	return true;
}

MatLabUdmBlock::NameDEMap MatLabUdmBlock::getNameDEMap( SLSF::State state ) {

	NameDEMap nameDEMap;

	DataSet dataSet = state.Data_kind_children_sorted( DataSorter() );
	for( DataSet::iterator dtsItr = dataSet.begin(); dtsItr != dataSet.end() ; (void)++dtsItr ) {
		SLSF::Data data = *dtsItr;
		std::string scope = data.Scope();
		if ( scope == "INPUT_DATA" || scope == "OUTPUT_DATA" ) {
			std::string name = data.Name();
			nameDEMap.insert(  std::make_pair( name, data )  );
		}
	}

	EventSet eventSet = state.Event_kind_children_sorted( EventSorter() );
	for( EventSet::iterator evsItr = eventSet.begin(); evsItr != eventSet.end() ; (void)++evsItr ) {
		SLSF::Event SLSFEvent = *evsItr;
		std::string scope = SLSFEvent.Scope();
		if ( scope == "INPUT_EVENT" || scope == "OUTPUT_EVENT" ) {
			std::string name = SLSFEvent.Name();
			nameDEMap.insert(  std::make_pair( name, SLSFEvent )  );
		}
	}

	return nameDEMap;
}

bool MatLabUdmBlock::makeSFunctionType( MatLabUdmBlock &matLabUdmBlock ) {

	SLSF::Block block = matLabUdmBlock.getUdmBlock();

	TypeBaseVector inputTypeBaseVector;

	if (  !getInputTypes( block, inputTypeBaseVector )  ) return false;

	ParameterMap parameterMap = getParameterMap( block );

	std::string value = parameterMap[ "FunctionName" ].Value();
	if ( value == "sfblk_manswitch" ) {

		OutputPortSet outputPortSet = block.OutputPort_kind_children_sorted( OutputPortSorter() );
		SLSF::OutputPort outputPort = *outputPortSet.begin();

		SLSF::TypeBase typeBase = *inputTypeBaseVector.begin();

		propagateType( typeBase, outputPort );
		return true;
	}

#if PARADIGM == CyberComposition_PARADIGM
	StateSet stateSet = block.State_children();
	if ( stateSet.empty() ) {
		std::cerr << "ERROR:  S-Function \"" << block.Name() <<
		 "\":  HAS NO STATE MACHINE, CAN'T GET OUTPUT TYPES" << std::endl;
		return false;
	}

	SLSF::State state = *stateSet.begin();
#else
	ConnectorRefSet connectorRefSet = block.ConnectorRef_children();
	if ( connectorRefSet.empty() ) {
		std::cerr << "ERROR:  S-Function \"" << block.Name() <<
		 "\":  HAS NO REFERENCE TO STATE MACHINE, CAN'T GET OUTPUT TYPES" << std::endl;
		return false;
	}

	// SAFE CAST -- ConnectorRef ONLY REFERS TO State AT THIS LEVEL
	SLSF::State state = SLSF::State::Cast(  ( *connectorRefSet.begin() ).ref()  );
#endif

	NameDEMap nameDEMap = getNameDEMap( state );

	// SAFE CAST
	SLSF::Block parentBlock = SLSF::Block::Cast( block.GetParent() );

	PortSet parentPortSet = parentBlock.Port_kind_children();

	// PLACE TYPES ON ALL PORTS OF S-FUNCTION.
	//
	// IN ORDER TO PLACE TYPES ON THE *OUTPUT* PORTS THIS CHART-BASED S-FUNCTION, SOMETHING SPECIAL MUST
	// BE DONE.  THIS IS BECAUSE THE NUMBER OF OUTPUTS OF THIS S-FUNCTION IS ONE MORE THAN THE NUMBER OF
	// OUTPUT DATA/EVENTS IN THE CHART, WITH THE EXTRA OUTPUT OF THE S-FUNCTION GOING TO A DEMUX.
	//
	// THE OUTPUT PORTS OF THE S-FUNCTION THAT ACTUALLY PERTAIN TO OUTPUT DATA/EVENTS OF THE CHART, I.E. THOSE
	// THAT DO NOT GO TO A DEMUX, ARE DIRECTLY CONNECTED TO THE TO THE OUTPUT PORTS OF THE S-FUNCTION'S
	// CONTAINING SUBSYSTEM (I.E. ITS PARENT).
	//
	// INSTEAD OF SEARCHING FOR AND DISCARDING THE S-FUNCTION OUTPUT PORT THAT GOES TO THE DEMUX, WE SIMPLY
	// TAKE THE OUTPUT PORTS OF THE PARENT SUBSYSTEM AND TRACE THEM BACK TO THE S-FUNCTION OUTPUT PORTS.
	// ONCE WE HAVE THESE OUTPUT PORTS OF THE S-FUNCTION, WE ASSIGN TYPES TO THEM.

	bool retval = true;
	for( PortSet::iterator ptsItr = parentPortSet.begin() ; ptsItr != parentPortSet.end() ; (void)++ptsItr ) {

		SLSF::Port parentPort = *ptsItr;

		SLSF::Port port;
		SLSF::TypeBase deTypeBase;

		if ( parentPort.type() != SLSF::TriggerPort::meta ) {

			port = parentPort;
			LineSet lineSet = port.srcLine();
			while( !lineSet.empty() ) {
				SLSF::Line line = *lineSet.begin();
				port = line.srcLine_end();
//				SLSF::Block block = SLSF::Block::Cast( port.GetParent() );
//				std::cerr << "Tracing block \"" << block.name() << "\"" << std::endl;
				lineSet = port.srcLine();
			}

			std::string parentPortName = parentPort.name();
			NameDEMap::iterator ndmItr = nameDEMap.find( parentPortName );
			if ( ndmItr == nameDEMap.end() ) {
				std::string parentName = parentBlock.Name();
				std::cerr << "ERROR: port \"" << parentPortName << "\" of StateChart block \"" << parentName <<
				 "\" has no type and no corresponding Data or Event object." << std::endl;
				retval = false;
				continue;
			}

			SLSF::StateDE stateDE = ndmItr->second;
			SLSF::TypeBaseRef deTypeBaseRef = stateDE.TypeBaseRef_child();
			deTypeBase = deTypeBaseRef.ref();
//			std::cerr << "\tType \"" << deTypeBase.name() << "\"" << std::endl;

		} else {

			LineSet lineSet = parentPort.dstLine();
			SLSF::Line line = *lineSet.begin();
			port = line.dstLine_end();

			SLSF::TypeBaseRef deTypeBaseRef = parentPort.TypeBaseRef_child();
			deTypeBase = deTypeBaseRef.ref();

		}

		propagateType( deTypeBase, port );
	}

	// DEAL WITH THAT PESKY DEMUX -- ASSIGN OUTPUT PORT THAT CONNECTS TO IT THE DEFAULT TYPE.
	OutputPortSet outputPortSet = block.OutputPort_kind_children_sorted( OutputPortSorter() );
	for(
	 OutputPortSet::iterator opsItr = outputPortSet.begin() ;
	 opsItr != outputPortSet.end() ;
	 (void)++opsItr
	) {
		SLSF::Port port = *opsItr;
		SLSF::TypeBaseRef typeBaseRef = port.TypeBaseRef_child();
		SLSF::TypeBase typeBase = typeBaseRef.ref();
		if ( typeBase == Udm::null ) {
			propagateType( UdmEngine::getDefaultMatrix(), port );
		}
	}
	
	return retval;
}

bool MatLabUdmBlock::makeMergeType( MatLabUdmBlock &matLabUdmBlock ) {

	SLSF::Block block = matLabUdmBlock.getUdmBlock();

	TypeBaseVector inputTypeBaseVector;

	bool hasAllInputTypes = getInputTypes( block, inputTypeBaseVector );
	if (  !hasAllInputTypes  ) {
		return false;
	}

	OutputPortSet outputPortSet = block.OutputPort_kind_children_sorted( OutputPortSorter() );
	SLSF::OutputPort outputPort = *outputPortSet.begin();

	SLSF::TypeBase typeBase = *inputTypeBaseVector.begin();

	propagateType( typeBase, outputPort );
	return true;
}

bool MatLabUdmBlock::makeSubSystemType( MatLabUdmBlock &matLabUdmBlock ) {

	SLSF::Block block = matLabUdmBlock.getUdmBlock();

	InputPortSet inputPortSet = block.InputPort_kind_children_sorted( InputPortSorter() );
	for( InputPortSet::iterator ipsItr = inputPortSet.begin() ; ipsItr != inputPortSet.end() ; ++ipsItr ) {
		LineSet lineSet = ipsItr->srcLine();
		if ( lineSet.empty() ) propagateType( *ipsItr );
	}

	OutputPortSet outputPortSet = block.OutputPort_kind_children_sorted( OutputPortSorter() );
	for( OutputPortSet::iterator opsItr = outputPortSet.begin() ; opsItr != outputPortSet.end() ; ++opsItr ) {
		LineSet lineSet = opsItr->srcLine();
		if ( lineSet.empty() ) propagateType( *opsItr );
	}

	return true;
}

bool MatLabUdmBlock::isSpecialBlock( void ) {
	SLSF::Block block = getUdmBlock();
	std::string blockType = block.BlockType();

	if (  getSpecialBlockFunctionMap().find( blockType ) == getSpecialBlockFunctionMap().end()  ) {
		return false;
	}

	ParameterMap parameterMap = getParameterMap( block );
	if ( blockType == "Demux" ) {
		std::string value = parameterMap[ "BusSelectionMode" ].Value();
		if ( value == "off" ) return false;
	} else if ( blockType == "S-Function" ) {
		std::string testMatLabName = getMatLabPrefix() + "t";
		std::string command =
		 testMatLabName + " = " + getParentMatLabObject() + ".find( '-isa', 'Stateflow.Chart', '-depth', 1 )";
		MatLabEngine::globalEngine().executeCommand( command );

		int noCurrentObjects = MatLabEngine::globalEngine().getMxLinearSize( testMatLabName );
		if ( noCurrentObjects == 0 ) {
			return false;
		}

//			if (  parameterMap.find( "FunctionName" ) == parameterMap.end()  ) return false;
	}
	return true;
}

