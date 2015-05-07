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
 *  FILE NAME:	MatLabUdmData.hpp 
 *	SYNOPSIS:	Definition of class "MatLabUdmData", translates SL/SF 'Stateflow.Data'
 *				objects into equivalent GME ESMoL Data atoms.					
 *  AUTHOR:		Harmon Nine - hnine@isis.vanderbilt.edu
 *  CREATED:	07/20/06
 *  LAST UPDATE: 
 */
#ifndef _ISIS_MATLABUDMDATA
#define _ISIS_MATLABUDMDATA

#include <ctype.h>
#include <string>
#include <boost/regex.hpp>
#include "SLSF.h"
#include "MatLabUdm.hpp"

using namespace SLSF;


class MatLabUdmData : public MatLabUdm {

private:
	void setupData( void );

	typedef std::vector< MatLabUdmData > MatLabUdmDataVector;

	static MatLabUdmDataVector &getMatLabUdmDataVector( void ) {
		static MatLabUdmDataVector matLabUdmDataVector;
		return matLabUdmDataVector;
	}

public:
	static void getAllDataTypes( void ) {
		for(
		 MatLabUdmDataVector::iterator mdvItr = getMatLabUdmDataVector().begin() ;
		 mdvItr != getMatLabUdmDataVector().end() ;
		 ++mdvItr
		) {
			mdvItr->getType();
		}
	}

	MatLabUdmData( Udm::Object &udmParent, const std::string &matLabName, int matLabObjectNo ) :
	  MatLabUdm(  Data::Create( udmParent ), matLabName, matLabObjectNo  ) { setupData(); }

	Data getUdmData( void ) { return Data::Cast( getUdmObject() ); }

	static const std::string &getMatLabChar( void ) {
		static const std::string matLabChar( "d" );
		return matLabChar;
	}
	static const std::string &getMatLabType( void ) {
		static const std::string matLabType( "'-isa', 'Stateflow.Data'" );
		return matLabType;
	}

	class Factory : public MatLabUdm::Factory {
	public:
		virtual MatLabUdm::SmartPointer create( Udm::Object &udmParent, const std::string &matLabName,
		 int matLabObjectNo ) const {
			return MatLabUdm::SmartPointer(  new MatLabUdmData( udmParent, matLabName, matLabObjectNo )  );
		}

		virtual const std::string &getMatLabChar( void ) const { return MatLabUdmData::getMatLabChar(); }
		virtual const std::string &getMatLabType( void ) const { return MatLabUdmData::getMatLabType(); }
	};

	void getType( void );
};


#endif
