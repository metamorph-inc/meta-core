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
 *  FILE NAME:	MatLabUdmState.hpp 
 *	SYNOPSIS:	Definition of class "MatLabUdmState", translates SL/SF 'Stateflow.State'
 *				objects into equivalent GME ESMoL State models.					
 *  AUTHOR:		Harmon Nine - hnine@isis.vanderbilt.edu
 *  CREATED:	07/20/06
 *  LAST UPDATE: 
 */
#ifndef _ISIS_MATLABUDMSTATE
#define _ISIS_MATLABUDMSTATE

#include "MatLabUdm.hpp"

using namespace SLSF;

class MatLabUdmState : public MatLabUdm {

private:
	void setupState( void );

public:
	MatLabUdmState( Udm::Object &udmParent, const std::string &matLabName, int matLabObjectNo ) :
	  MatLabUdm(  State::Create( udmParent ), matLabName, matLabObjectNo  ) { setupState(); }

	State getUdmState( void ) const { return State::Cast( getUdmObject() ); }

	static const std::string &getMatLabChar( void ) {
		static const std::string matLabChar( "s" );
		return matLabChar;
	}
	static const std::string &getMatLabType( void ) {
		static const std::string matLabType( "'-isa', 'Stateflow.State', '-or', '-isa', 'Stateflow.Box'" );
		return matLabType;
	}
	virtual bool collect( void ) const { return true; }

	class Factory : public MatLabUdm::Factory {
	public:
		virtual MatLabUdm::SmartPointer create( Udm::Object &udmParent, const std::string &matLabName,
		 int matLabObjectNo ) const {
			return MatLabUdm::SmartPointer(  new MatLabUdmState( udmParent, matLabName, matLabObjectNo )  );
		}

		virtual const std::string &getMatLabChar( void ) const { return MatLabUdmState::getMatLabChar(); }
		virtual const std::string &getMatLabType( void ) const { return MatLabUdmState::getMatLabType(); }
	};

	static void constructStates( MatLabUdm::SmartPointer &matLabUdmParentSP );

private:
	typedef std::set< State > StateSet;

	struct AndOrState {
		bool operator()( const State &state) {
			static std::string andState( "AND_STATE" );
			static std::string orState( "OR_STATE" );

			std::string stateDecomposition( state.Decomposition() );
			return stateDecomposition != andState && stateDecomposition != orState;
		}
	};

};


#endif
