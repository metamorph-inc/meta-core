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
 *  FILE NAME:	MatLabUdmMiscSimulink.hpp 
 *	SYNOPSIS:	Definition of classes:
 *				 "MatLabUdmRoot", translates SL/SF 'Simulink.Root' object into equivalent GME ESMoL Subsystem model.
 *				 "MatLabUdmModel", translates SL/SF 'Simulink.BlockDiagram' objects into equivalent GME ESMoL Systems.					
 *  AUTHOR:		Harmon Nine - hnine@isis.vanderbilt.edu
 *  CREATED:	07/20/06
 *  LAST UPDATE: 
 */
#ifndef _ISIS_MATLABUDMMISCSIMULINK
#define _ISIS_MATLABUDMMISCSIMULINK

#include "MatLabUdm.hpp"

using namespace SLSF;

class MatLabUdmRoot : public MatLabUdm {

public:
	MatLabUdmRoot( Udm::Object &udmObject ) :
	 MatLabUdm( udmObject, "r", 0 ) { }

	class Factory {
	public:
		virtual MatLabUdm::SmartPointer create( Udm::Object udmObject ) const {
			return MatLabUdm::SmartPointer(  new MatLabUdmRoot( udmObject )  );
		}
	};
};


class MatLabUdmModel : public MatLabUdm {

private:
	void setupModel( void );

public:
	MatLabUdmModel( Udm::Object udmParent, const std::string &matLabName, int matLabObjectNo ) :
	  MatLabUdm(  Subsystem::Create( udmParent ), matLabName, matLabObjectNo  ) { setupModel(); }


	Subsystem getUdmModel( void ) { return Subsystem::Cast( getUdmObject() ); }

	static const std::string &getMatLabChar( void ) {
		static const std::string matLabChar( "m" );
		return matLabChar;
	}
	static const std::string &getMatLabType( void ) {
		static const std::string matLabType( "'-isa', 'Simulink.BlockDiagram'" );
		return matLabType;
	}
	virtual bool collect( void ) const { return true; }


	class Factory : public MatLabUdm::Factory {
	public:
		virtual MatLabUdm::SmartPointer create(
		 Udm::Object &udmParent, const std::string &matLabName, int matLabObjectNo
		) const {
			return MatLabUdm::SmartPointer(  new MatLabUdmModel( udmParent, matLabName, matLabObjectNo )  );
		}

		virtual const std::string &getMatLabChar( void ) const { return MatLabUdmModel::getMatLabChar(); }
		virtual const std::string &getMatLabType( void ) const { return MatLabUdmModel::getMatLabType(); }

		virtual bool exclude( const std::string &matLabObject ) const {
			return MatLabEngine::globalEngine().getMxIntValue( matLabObject + ".isLibrary" ) == 1;
		}
	};
};


#endif
