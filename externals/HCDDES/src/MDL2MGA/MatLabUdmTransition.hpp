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
 *  FILE NAME:	MatLabUdmTransition.hpp 
 *	SYNOPSIS:	Definition of class "MatLabUdmTransition", translates SL/SF 'Stateflow.Transition'
 *				objects into equivalent GME ESMoL Transition connections.					
 *  AUTHOR:		Harmon Nine - hnine@isis.vanderbilt.edu
 *  CREATED:	07/20/06
 *  LAST UPDATE: 
 */
#ifndef _ISIS_MATLABUDMTRANSITION
#define _ISIS_MATLABUDMTRANSITION

#include "MatLabUdm.hpp"
#include "UdmEngine.hpp"

using namespace SLSF;

class MatLabUdmTransition : public MatLabUdm {

private:
	SLSF::State getSourceOrDestParent( const std::string &matLabName, int matLabObjectNo );
	void setupTransition( void );

	static std::string rightBracketSubstitution( std::string input );
	static std::string dollarSignSubstitution( std::string input );

public:
	MatLabUdmTransition( Udm::Object &udmParent, const std::string &matLabName, int matLabObjectNo ) :
	 MatLabUdm(  Transition::Create( udmParent ),  matLabName,  matLabObjectNo  )
//	 MatLabUdm(	  Transition::Create(  getSourceOrDestParent( matLabName, matLabObjectNo )  ),  matLabName,  matLabObjectNo   )
	{ setupTransition(); }

	Transition getUdmTransition( void ) const { return Transition::Cast( getUdmObject() ); }

	static const std::string &getMatLabChar( void ) {
		static const std::string matLabChar( "t" );
		return matLabChar;
	}
	static const std::string &getMatLabType( void ) {
		static const std::string matLabType( "'-isa', 'Stateflow.Transition'" );
		return matLabType;
	}

	class Factory : public MatLabUdm::Factory {
	public:
		virtual MatLabUdm::SmartPointer create( Udm::Object &udmParent, const std::string &matLabName,
		 int matLabObjectNo ) const {
			return MatLabUdm::SmartPointer(  new MatLabUdmTransition( udmParent, matLabName, matLabObjectNo )  );
		}

		virtual const std::string &getMatLabChar( void ) const { return MatLabUdmTransition::getMatLabChar(); }
		virtual const std::string &getMatLabType( void ) const { return MatLabUdmTransition::getMatLabType(); }
	};

	class TransitionParents {
	private:
		MatLabUdm::SPVector _transitionParents;

	public:
		void add( const MatLabUdm::SPVector &matLabUdmSPVector ) {
			_transitionParents.insert( _transitionParents.end(), matLabUdmSPVector.begin(), matLabUdmSPVector.end() );
		}

		void buildTransitions( void ) {
			for(
			 MatLabUdm::SPVector::iterator spvItr = _transitionParents.begin() ;
			 spvItr != _transitionParents.end() ;
			 ++spvItr
			) {
				(void)MatLabUdm::buildUdm( *spvItr, MatLabUdmTransition::Factory() );
			}
		}
	};

	static TransitionParents &getTransitionParents( void ) {
		static TransitionParents transitionParents;
		return transitionParents;
	}

private:
	typedef std::set< ConnectorRef > ConnectorRefSet;
	typedef std::map< int, ConnectorRef > ConnectorRefMap;

	static ConnectorRefMap getConnectorRefMap( State &state ) {
		ConnectorRefMap connectorRefMap;
		ConnectorRefSet connectorRefSet = state.ConnectorRef_kind_children();
		for( ConnectorRefSet::iterator crsItr = connectorRefSet.begin() ; crsItr != connectorRefSet.end() ; ++crsItr ) {
			connectorRefMap[ (int) TransConnector::Cast( (*crsItr).ref() ).uniqueId() ] = *crsItr;
		}
		return connectorRefMap;
	}

	TransConnector getTransitionEnd( int transitionEndId );
};

#endif
