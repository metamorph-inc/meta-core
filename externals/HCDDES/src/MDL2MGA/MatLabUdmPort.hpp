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
 *  FILE NAME:	MatLabUdmPort.hpp 
 *	SYNOPSIS:	Definition of class "MatLabUdmPort", translates SL/SF port objects
 *				(e.g. 'Simulink.Inport', 'Simulink.TriggerPort) into equivalent GME ESMoL
 *				InputPort, TriggerPort atoms.  ALSO PROVIDES FUNCTIONALITY TO CONNECT
 *				THESE GME ATOMS USING LINE CONNECTIONS.
 *  AUTHOR:		Harmon Nine - hnine@isis.vanderbilt.edu
 *  CREATED:	07/20/06
 *  LAST UPDATE: 
 */
#ifndef _ISIS_MATLABUDMPORT
#define _ISIS_MATLABUDMPORT

#include "MatLabUdm.hpp"

using namespace SLSF;


class MatLabUdmPort : public MatLabUdm {

private:
	PortType _portType;
	int _handle;
	std::string _embeddedBlockName;

	Udm::Object getPortInstance( std::string matLabObject, SLSF::Block block );
	Udm::Object distinguish( Udm::Object &udmParent, const std::string &matLabName, int matLabObjectNo );

	void setupPort( void );

	typedef std::vector< MatLabUdmPort > MatLabUdmPortVector;

	static MatLabUdmPortVector &getMatLabUdmPortVector( void ) {
		static MatLabUdmPortVector matLabUdmPortVector;
		return matLabUdmPortVector;
	}

public:
	static void getAllPortTypes( void ) {
		for(
		 MatLabUdmPortVector::iterator mpvItr = getMatLabUdmPortVector().begin() ;
		 mpvItr != getMatLabUdmPortVector().end() ;
		 ++mpvItr
		) {
			mpvItr->getType();
		}
	}

private:
	void getType( void );
	std::string getEmbeddedBlockName( void ) { return _embeddedBlockName; }

public:
	MatLabUdmPort( Udm::Object &udmParent, const std::string &matLabName, int matLabObjectNo, std::string embeddedBlockName = "" ) :
	 MatLabUdm( matLabName, matLabObjectNo  ), _embeddedBlockName( embeddedBlockName )
	{
		setUdmObject(  distinguish( udmParent, matLabName, matLabObjectNo )  );
		setupPort();
	}

	int getHandle( void ) const { return _handle; }

	PortType getPortType( void ) const { return _portType; }

	Port getUdmPort( void ) { return Port::Cast( getUdmObject() ); }

	InputPort getUdmInputPort( void ) { return InputPort::Cast( getUdmObject() ); }
	OutputPort getUdmOutputPort( void ) { return OutputPort::Cast( getUdmObject() ); }
	EnablePort getUdmEnablePort( void ) { return EnablePort::Cast( getUdmObject() ); }
	TriggerPort getUdmTriggerPort( void ) { return TriggerPort::Cast( getUdmObject() ); }

#if PARADIGM != CyberComposition_PARADIGM
	StatePort getUdmStatePort( void ) { return StatePort::Cast( getUdmObject() ); }
#endif


	static const std::string &getMatLabChar( void ) {
		static const std::string matLabChar( "p" );
		return matLabChar;
	}

	typedef std::vector< std::string > PortTypes;

private:
	class PortTypesInit {
	private:
		PortTypes _portTypes;

	public:
		PortTypesInit( void ) {
			_portTypes.push_back( "Simulink.Inport" );
			_portTypes.push_back( "Simulink.Outport" );
			_portTypes.push_back( "Simulink.TriggerPort" );
			_portTypes.push_back( "Simulink.EnablePort" );
			_portTypes.push_back( "Simulink.ActionPort" );
// APPARENTLY, THESE IS NO CLASS FOR "STATE" PORTS, EVEN THOUGH "STATE" PORTS EXIST
//			_portTypes.push_back( "Simulink.StatePort" );
		}
		const PortTypes &getPortTypes( void ) const { return _portTypes; }
	};

public:
	static const PortTypes &getPortTypes( void ) {
		static const PortTypesInit portTypesInit;
		return portTypesInit.getPortTypes();
	}


private:
	class MatLabTypeInit {
	private:
		std::string _matLabType;

	public:
		MatLabTypeInit( void ) {
			const PortTypes &portTypes = getPortTypes();
			PortTypes::const_iterator pttItr = portTypes.begin();
			_matLabType = std::string("'-isa', '") + *pttItr++ + '\'';
			while( pttItr != portTypes.end() ) {
				_matLabType += std::string(", '-or', '-isa', '") + *pttItr++ + '\'';
			}
		}
		const std::string &getMatLabType( void ) const { return _matLabType; }
	};

public:
	static const std::string &getMatLabType( void ) {
		static const MatLabTypeInit matLabTypeInit;
		return matLabTypeInit.getMatLabType();
	}

	virtual bool collect( void ) const { return true; }

	class Factory : public MatLabUdm::Factory {
	private:
		std::string _embeddedBlockName;

	public:
		Factory( std::string embeddedBlockName = "" ) : _embeddedBlockName( embeddedBlockName ) { }

		virtual MatLabUdm::SmartPointer create( Udm::Object &udmParent, const std::string &matLabName, int matLabObjectNo ) const {
			return MatLabUdm::SmartPointer(  new MatLabUdmPort( udmParent, matLabName, matLabObjectNo, _embeddedBlockName )  );
		}

		virtual const std::string &getMatLabChar( void ) const { return MatLabUdmPort::getMatLabChar(); }
		virtual const std::string &getMatLabType( void ) const { return MatLabUdmPort::getMatLabType(); }
	};

};

#endif
