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
 *  FILE NAME:	MatLabUdmPort.cpp 
 *	SYNOPSIS:	Implementation of class "MatLabUdmPort", translates SL/SF port objects
 *				(e.g. 'Simulink.Inport', 'Simulink.TriggerPort) into equivalent GME ESMoL
 *				InputPort, TriggerPort atoms.  ALSO PROVIDES FUNCTIONALITY TO CONNECT
 *				THESE GME ATOMS USING LINE CONNECTIONS.
 *  AUTHOR:		Harmon Nine - hnine@isis.vanderbilt.edu
 *  CREATED:	07/20/06
 *  LAST UPDATE: 
 */
#include "MatLabUdmPort.hpp"
#include "MatLabUdmBlock.hpp"
#include "mfile2SFC.hpp"

Udm::Object MatLabUdmPort::getPortInstance( std::string matLabObject, SLSF::Block block ) {

	Udm::Object retObject;

	switch( _portType ) {
		case INPUTPORT: {
			InputPortVector inputPortVector = block.InputPort_kind_children_sorted( InputPortSorter() );
			int portNo = boost::lexical_cast< int >(  MatLabEngine::globalEngine().getMxStringValue( matLabObject + ".Port" ).c_str()  );
			retObject = inputPortVector[ portNo - 1 ];
			break;
		}
		case OUTPUTPORT: {
			OutputPortVector outputPortVector = block.OutputPort_kind_children_sorted( OutputPortSorter() );
			int portNo = boost::lexical_cast< int >(  MatLabEngine::globalEngine().getMxStringValue( matLabObject + ".Port" ).c_str()  );
			retObject = outputPortVector[ portNo - 1 ];
			break;
		}
		case ENABLEPORT: {
			EnablePortVector enablePortVector = block.EnablePort_kind_children();
			retObject = enablePortVector.back();
			break;
		}
		case TRIGGERPORT: {
			TriggerPortVector triggerPortVector = block.TriggerPort_kind_children();
			retObject = triggerPortVector.back();
			break;
		}
#if PARADIGM == CyberComposition_PARADIGM
		default: {
			ActionPortVector actionPortVector = block.EnablePort_kind_children();
			retObject = actionPortVector.back();
			break;
		}
#else
		case ACTIONPORT: {
			ActionPortVector actionPortVector = block.ActionPort_kind_children();
			retObject = actionPortVector.back();
			break;
		}
		default: {
			StatePortVector statePortVector = block.StatePort_kind_children();
			retObject = statePortVector.back();
			break;
		}
#endif

	}

	return retObject;
}

Udm::Object MatLabUdmPort::distinguish( Udm::Object &udmParent, const std::string &matLabName, int matLabObjectNo ) {

	std::string matLabObject = getMatLabObjectString( matLabName, matLabObjectNo );

	std::string portType = MatLabEngine::globalEngine().getMxStringValue( matLabObject + ".BlockType" );
	transform( portType.begin(), portType.end(), portType.begin(), ToLower() );

	if ( portType == "inport" )           _portType = INPUTPORT;
	else if ( portType == "outport" )     _portType = OUTPUTPORT;
	else if ( portType == "enableport" )  _portType = ENABLEPORT;
	else if ( portType == "triggerport" ) _portType = TRIGGERPORT;
#if PARADIGM == CyberComposition_PARADIGM
	else                                  _portType = ACTIONPORT;
#else
	else if ( portType == "actionport" )  _portType = ACTIONPORT;
	else                                  _portType = STATEPORT;
#endif

	if ( udmParent.isInstance() ) {
		SLSF::Block block = SLSF::Block::Cast( udmParent );
		return getPortInstance( matLabObject, block );
	}

	SLSF::Port port;
	switch( _portType ) {
		case INPUTPORT:   port = InputPort::Create( udmParent );   break;
		case OUTPUTPORT:  port = OutputPort::Create( udmParent );  break;
		case ENABLEPORT:  port = EnablePort::Create( udmParent );  break;
		case TRIGGERPORT: port = TriggerPort::Create( udmParent ); break;
#if PARADIGM == CyberComposition_PARADIGM
		default:          port = EnablePort::Create( udmParent );  break; // THIS IS ACTUALLY FOR AN "ACTIONPORT"
#else
		case ACTIONPORT:  port = ActionPort::Create( udmParent );  break;
		default:          port = StatePort::Create( udmParent );   break;
#endif
	}

	SLSF::TypeBaseRef typeBaseRef = SLSF::TypeBaseRef::Create( port );
	typeBaseRef.name() = "undefined";

	return port;
}

void MatLabUdmPort::setupPort( void ) {
	std::string command;
	PortType portType = getPortType();

	_handle = MatLabEngine::globalEngine().getMxIntValue( getMatLabObject() + ".Handle" );

	Port port = getUdmPort();

	port.name() = getUniqueName().c_str();
	port.RefId() = getRefId();

	std::string portConn = getMatLabObject() + ".PortConnectivity";

	if ( portType == INPUTPORT ) {
		getUdmInputPort().Number() =
		 boost::lexical_cast< int >(  MatLabEngine::globalEngine().getMxStringValue( getMatLabObject() + ".Port" ).c_str()  );
		getUdmSocketMap()[ UdmSocket( _handle, 0, OUTPUTPORT ) ] = port;
		connectPort( port, false, portConn, "Dst" );
		getMatLabUdmPortVector().push_back( *this );
		return;
	}
	
	if ( portType == OUTPUTPORT ) {
		getUdmOutputPort().Number() =
		 boost::lexical_cast< int >(  MatLabEngine::globalEngine().getMxStringValue( getMatLabObject() + ".Port" ).c_str()  );
		getUdmSocketMap()[ UdmSocket( _handle, 0, INPUTPORT ) ] = port;
		connectPort( port, true, portConn, "Src" );
		getMatLabUdmPortVector().push_back( *this );
		return;
	}

	if ( portType == ENABLEPORT ) {
#if PARADIGM != CyberComposition_PARADIGM
		getUdmEnablePort().StatesWhenEnabling() =
		 MatLabEngine::globalEngine().getMxStringValue( getMatLabObject() + ".StatesWhenEnabling" ).c_str();
#endif
		getUdmSocketMap()[ UdmSocket( _handle, 0, OUTPUTPORT ) ] = port;
		connectPort( port, false, portConn, "Dst" );
		return;
	}

	if ( portType == TRIGGERPORT ) {
		getUdmTriggerPort().TriggerType() =
		 MatLabEngine::globalEngine().getMxStringValue( getMatLabObject() + ".TriggerType" ).c_str();
		getUdmSocketMap()[ UdmSocket( _handle, 0, OUTPUTPORT ) ] = port;
		connectPort( port, false, portConn, "Dst" );
		return;
	}

	/*  ACTUALLY, AN ACTIONPORT DOESN'T EXIST AS A SEPARATE PORT OBJECT.  THIS IS HERE JUST FOR COMPLETENESS  */
	if ( portType == ACTIONPORT ) {
		getUdmSocketMap()[ UdmSocket( _handle, 0, INPUTPORT ) ] = port;
		connectPort( port, false, portConn, "Dst" );
		return;
	}

	/*  ACTUALLY, A STATEPORT DOESN'T EXIST AS A SEPARATE PORT OBJECT.  THIS IS HERE JUST FOR COMPLETENESS  */
	if ( portType == STATEPORT ) {
		getUdmSocketMap()[ UdmSocket( _handle, 0, INPUTPORT ) ] = port;
		connectPort( port, true, portConn, "Src" );
		return;
	}

	return;
}


void MatLabUdmPort::getType( void ) {

	std::string MatLabObject = getMatLabObject();
	SLSF::Port port = getUdmPort();

	if ( _embeddedBlockName.empty() ) {
		std::string useBusObject = 
		 MatLabEngine::globalEngine().getMxStringValue( getMatLabObject() + ".UseBusObject" );

		if ( useBusObject == "off" ) {

			SLSF::TypeBaseRef typeBaseRef = port.TypeBaseRef_child();
			SLSF::TypeBase typeBase = typeBaseRef.ref();
			if ( typeBase != Udm::null ) return;

			// NOTE:  OUTPORTS IN MATLAB HAVE COMPONENT INPUT PORTS, AND VICE VERSA!!
			std::string portType = Udm::IsDerivedFrom( port.type(), SLSF::InPort::meta ) ? "Outport" : "Inport";

			std::string compiledDimensions =
			 MatLabEngine::globalEngine().getData( getMatLabObject() + ".CompiledPortDimensions." + portType );
			std::string compiledDataTypes =
			 MatLabEngine::globalEngine().getData( getMatLabObject() + ".CompiledPortDataTypes." + portType );

			IntList dimensionList = MatLabUdm::getIntList( compiledDimensions );
			if ( dimensionList.empty() ) return;

			StringList typeList = MatLabUdm::getWordList( compiledDataTypes );

			SLSF::Matrix slsfMatrix;

			int noDims = dimensionList.front(); dimensionList.pop_front();
			while( noDims < 0 ) {
				dimensionList.pop_front();
				noDims = dimensionList.front(); dimensionList.pop_front();
			}
			if ( noDims == 1 ) {
				slsfMatrix = SLSF::Matrix::Create( UdmEngine::getUdmTypesRoot() );
				std::string type = typeList.front(); typeList.pop_front();
				if ( type == "fcn_call" ) type = "double";
				slsfMatrix.Type() = MatLabUdm::parameterValueSub( type );
				slsfMatrix.rows() = dimensionList.front(); dimensionList.pop_front();
				slsfMatrix.columns() = 1;
			} else if ( noDims == 2 ) {
				slsfMatrix = SLSF::Matrix::Create( UdmEngine::getUdmTypesRoot() );
				std::string type = typeList.front(); typeList.pop_front();
				if ( type == "fcn_call" ) type = "double";
				slsfMatrix.Type() = MatLabUdm::parameterValueSub( type );
				slsfMatrix.rows() = dimensionList.front(); dimensionList.pop_front();
				slsfMatrix.columns() = dimensionList.front(); dimensionList.pop_front();
			} else {
				std::cerr << "WARNING: NUMBER OF DIMENSIONS GREATER THAN 2 DETECTED (NOT SUPPORTED)." << std::endl;
				return;;
			}

			UdmEngine::integrateTypeBase( slsfMatrix );

			typeBaseRef.ref() = slsfMatrix;
			typeBaseRef.name() = slsfMatrix.name();

		} else {

			std::string busObject =
			 MatLabEngine::globalEngine().getMxStringValue( getMatLabObject() + ".BusObject" );

			SLSF::TypeBaseRef typeBaseRef = port.TypeBaseRef_child();

			typeBaseRef.name() = busObject;
			typeBaseRef.ref() = makeBusObjectTypeStruct( busObject );

		}

	} else {

		__int64 portNumber;
		std::string ioDataSpec;
		char portTypeSpecifier;

		if ( getPortType() == INPUTPORT ) {
			portNumber = getUdmInputPort().Number();
			ioDataSpec = "Inputs";
			portTypeSpecifier = 'i';
		} else {
			portNumber = getUdmOutputPort().Number();
			ioDataSpec = "Outputs";
			portTypeSpecifier = 'o';
		}

		std::string embeddedDataName = _embeddedBlockName + "0" + portTypeSpecifier +
		 boost::lexical_cast< std::string >( portNumber );
		std::string command = embeddedDataName + " = " + getEmbeddedBlockName() + "(1)." + ioDataSpec +
		 "(" + boost::lexical_cast< std::string >( portNumber ) + ").Props(1)";

		MatLabEngine::globalEngine().executeCommand( command );

		std::string method = MatLabEngine::globalEngine().getMxStringValue( embeddedDataName + ".Type(1).Method" );
		if ( method == "Inherited" ) return;

		if (  method.substr( 0, 3 ) != "Bus"  ) {

			SLSF::Matrix ESMoLMatrix = SLSF::Matrix::Create( UdmEngine::getUdmTypesRoot() );

			std::string dataType = MatLabEngine::globalEngine().getMxStringValue( embeddedDataName + ".Type(1).Primitive" );
			ESMoLMatrix.Type() = MatLabUdm::parameterValueSub( dataType );

			std::string dimensions = MatLabEngine::globalEngine().getMxStringValue( embeddedDataName + ".Array(1).Size" );

			int rows = 1;
			int columns = 1;
			if ( !dimensions.empty() ) getDimensions( dimensions, rows, columns );

			ESMoLMatrix.rows() = rows;
			ESMoLMatrix.columns() = columns;
			UdmEngine::integrateTypeBase( ESMoLMatrix );

			SLSF::TypeBaseRef typeBaseRef = port.TypeBaseRef_child();

			typeBaseRef.MemberIndex() = 0;
			typeBaseRef.ref() = ESMoLMatrix;
			typeBaseRef.name() = ESMoLMatrix.name();

		} else {

			std::string busObject = MatLabEngine::globalEngine().getMxStringValue( embeddedDataName + ".Type(1).BusObject" );
			SLSF::TypeBaseRef typeBaseRef = port.TypeBaseRef_child();

			typeBaseRef.name() = busObject;
			typeBaseRef.ref() = makeBusObjectTypeStruct( busObject );

		}

	}

	return;
}