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
 *  FILE NAME:	MatLabUdmMiscStateflow.hpp 
 *	SYNOPSIS:	Definition of classes:
 *				"MatLabUdmFunction", translates SL/SF 'Stateflow.Function' objects into equivalent GME ESMoL State models.
 *				"MatLabUdmJunction", translates SL/SF 'Stateflow.Junction' objects into equivalent GME ESMoL
 *				Junction/History atoms.
 *				"MatLabUdmEvent", translates SL/SF 'Stateflow.Event' objects into equivalent GME ESMoL Event atoms.
 *				"MatLabUdmChart", translates SL/SF 'Stateflow.Chart' objects into equivalent GME ESMoL State models.
 *  AUTHOR:		Harmon Nine - hnine@isis.vanderbilt.edu
 *  CREATED:	07/20/06
 *  LAST UPDATE: 
 */
#ifndef _ISIS_MATLABUDMMISCSTATEFLOW
#define _ISIS_MATLABUDMMISCSTATEFLOW

#include "UdmEngine.hpp" // CAN'T INCLUDE IN BASE CLASS "MatLabUdm" -- CAUSES "#include" LOOP
#include "MatLabUdm.hpp"

using namespace SLSF;

class MatLabUdmFunction : public MatLabUdm {

private:

	void setupFunction( void );

public:
	MatLabUdmFunction( Udm::Object &udmParent, const std::string &matLabName, int matLabObjectNo ) :
	  MatLabUdm(  State::Create( udmParent ), matLabName, matLabObjectNo  ) { setupFunction(); }

	State getUdmFunction( void ) { return State::Cast( getUdmObject() ); }

	static const std::string &getMatLabChar( void ) {
		static const std::string matLabChar( "f" );
		return matLabChar;
	}
	static const std::string &getMatLabType( void ) {
		static const std::string matLabType( "'-isa', 'Stateflow.Function'" );
		return matLabType;
	}
	virtual bool collect( void ) const { return true; }

	class Factory : public MatLabUdm::Factory {
	public:
		virtual MatLabUdm::SmartPointer create( Udm::Object &udmParent, const std::string &matLabName,
		  int matLabObjectNo ) const {
			return MatLabUdm::SmartPointer(  new MatLabUdmFunction( udmParent, matLabName, matLabObjectNo )  );
		}

		virtual const std::string &getMatLabChar( void ) const { return MatLabUdmFunction::getMatLabChar(); }
		virtual const std::string &getMatLabType( void ) const { return MatLabUdmFunction::getMatLabType(); }
	};
};


class MatLabUdmEMFunction : public MatLabUdm {

private:

	void setupEMFunction( void );

public:
	MatLabUdmEMFunction( Udm::Object &udmParent, const std::string &matLabName, int matLabObjectNo ) :
	  MatLabUdm(  State::Create( udmParent ), matLabName, matLabObjectNo  ) { setupEMFunction(); }

	State getUdmEMFunction( void ) { return State::Cast( getUdmObject() ); }

	static const std::string &getMatLabChar( void ) {
		static const std::string matLabChar( "g" );
		return matLabChar;
	}
	static const std::string &getMatLabType( void ) {
		static const std::string matLabType( "'-isa', 'Stateflow.EMFunction'" );
		return matLabType;
	}
	virtual bool collect( void ) const { return true; }

	class Factory : public MatLabUdm::Factory {
	public:
		virtual MatLabUdm::SmartPointer create( Udm::Object &udmParent, const std::string &matLabName,
		  int matLabObjectNo ) const {
			return MatLabUdm::SmartPointer(  new MatLabUdmEMFunction( udmParent, matLabName, matLabObjectNo )  );
		}

		virtual const std::string &getMatLabChar( void ) const { return MatLabUdmEMFunction::getMatLabChar(); }
		virtual const std::string &getMatLabType( void ) const { return MatLabUdmEMFunction::getMatLabType(); }
	};
};


class MatLabUdmTruthTable : public MatLabUdm {

public:
	typedef std::vector< std::string > StringVector;
	typedef boost::shared_ptr< StringVector > StringVectorSP;
	typedef std::vector< StringVectorSP > StringMatrix;

	typedef std::map< std::string, StringVectorSP > StringVectorMap;

	class StringVectorManager {
	private:
		StringMatrix _stringMatrix;
		StringVectorMap _stringVectorMap;

	public:
		StringVectorManager( const StringMatrix stringMatrix, const StringVectorMap stringVectorMap ) :
		 _stringMatrix( stringMatrix ), _stringVectorMap( stringVectorMap ) { }

		StringMatrix getStringMatrix( void ) {
			return _stringMatrix;
		}
		void setStringMatrix( const StringMatrix &stringMatrix ) {
			_stringMatrix = stringMatrix;
		}

		StringVectorMap getStringVectorMap( void ) {
			return _stringVectorMap;
		}
		void setStringVectorMap( const StringVectorMap &stringVectorMap ) {
			_stringVectorMap = stringVectorMap;
		}

	};

private:
	StringVectorManager getStringVectorManager( std::string matrix );

	void setupTruthTable( void );

public:
	MatLabUdmTruthTable( Udm::Object &udmParent, const std::string &matLabName, int matLabObjectNo ) :
	  MatLabUdm(  State::Create( udmParent ), matLabName, matLabObjectNo  ) { setupTruthTable(); }

	State getUdmTruthTable( void ) { return State::Cast( getUdmObject() ); }

	static const std::string &getMatLabChar( void ) {
		static const std::string matLabChar( "u" );
		return matLabChar;
	}
	static const std::string &getMatLabType( void ) {
		static const std::string matLabType( "'-isa', 'Stateflow.TruthTable'" );
		return matLabType;
	}
	virtual bool collect( void ) const { return true; }

	class Factory : public MatLabUdm::Factory {
	public:
		virtual MatLabUdm::SmartPointer create( Udm::Object &udmParent, const std::string &matLabName,
		  int matLabObjectNo ) const {
			return MatLabUdm::SmartPointer(  new MatLabUdmTruthTable( udmParent, matLabName, matLabObjectNo )  );
		}

		virtual const std::string &getMatLabChar( void ) const { return MatLabUdmTruthTable::getMatLabChar(); }
		virtual const std::string &getMatLabType( void ) const { return MatLabUdmTruthTable::getMatLabType(); }
	};
};


class MatLabUdmJunction : public MatLabUdm {

public:
	enum JunctionType { JUNCTION, HISTORY };

private:
	JunctionType _junctionType;

	Udm::Object distinguish( Udm::Object &udmParent, const std::string &matLabName, int matLabObjectNo );

	void setupJunction( void );

public:
	MatLabUdmJunction( Udm::Object &udmParent, const std::string &matLabName, int matLabObjectNo ) :
	 MatLabUdm(  distinguish( udmParent, matLabName, matLabObjectNo ), matLabName, matLabObjectNo  ) {
		setupJunction();
	}

	JunctionType getJunctionType( void ) { return _junctionType; }

	Junction getUdmJunction( void ) { return Junction::Cast( getUdmObject() ); }
	History getUdmHistory( void ) { return History::Cast( getUdmObject() ); }

	static const std::string &getMatLabChar( void ) {
		static const std::string matLabChar( "j" );
		return matLabChar;
	}
	static const std::string &getMatLabType( void ) {
		static const std::string matLabType( "'-isa', 'Stateflow.Junction'" );
		return matLabType;
	}

	class Factory : public MatLabUdm::Factory {
	public:
		virtual MatLabUdm::SmartPointer create( Udm::Object &udmParent, const std::string &matLabName,
		 int matLabObjectNo ) const {
			return MatLabUdm::SmartPointer(  new MatLabUdmJunction( udmParent, matLabName, matLabObjectNo )  );
		}

		virtual const std::string &getMatLabChar( void ) const { return MatLabUdmJunction::getMatLabChar(); }
		virtual const std::string &getMatLabType( void ) const { return MatLabUdmJunction::getMatLabType(); }
	};
};


class MatLabUdmEvent : public MatLabUdm {
private:

	void setupEvent( void );

public:
	MatLabUdmEvent( Udm::Object &udmParent, const std::string &matLabName, int matLabObjectNo ) :
	  MatLabUdm(  Event::Create( udmParent ), matLabName, matLabObjectNo  ) { setupEvent(); }

	Event getUdmEvent( void ) { return Event::Cast( getUdmObject() ); }

	static const std::string &getMatLabChar( void ) {
		static const std::string matLabChar( "e" );
		return matLabChar;
	}
	static const std::string &getMatLabType( void ) {
		static const std::string matLabType( "'-isa', 'Stateflow.Event'" );
		return matLabType;
	}

	class Factory : public MatLabUdm::Factory {
	public:
		virtual MatLabUdm::SmartPointer create( Udm::Object &udmParent, const std::string &matLabName,
		 int matLabObjectNo ) const {
			return MatLabUdm::SmartPointer(  new MatLabUdmEvent( udmParent, matLabName, matLabObjectNo )  );
		}

		virtual const std::string &getMatLabChar( void ) const { return MatLabUdmEvent::getMatLabChar(); }
		virtual const std::string &getMatLabType( void ) const { return MatLabUdmEvent::getMatLabType(); }
	};
};


class MatLabUdmChart : public MatLabUdm {
private:

	void setupChart( Udm::Object udmParent );
	Udm::Object distinguish( Udm::Object udmParent );

public:
	MatLabUdmChart( Udm::Object &udmParent, const std::string &matLabName, int matLabObjectNo ) :
	 MatLabUdm( matLabName, matLabObjectNo )
	{
		setUdmObject(  distinguish( udmParent )  );
		setupChart( udmParent );
	}

	State getUdmChart( void ) { return State::Cast( getUdmObject() ); }

	static const std::string &getMatLabChar( void ) {
		static const std::string matLabChar( "c" );
		return matLabChar;
	}
	static const std::string &getMatLabType( void ) {
		static const std::string matLabType( "'-isa', 'Stateflow.Chart'" );
		return matLabType;
	}
	virtual bool collect( void ) const { return true; }

	class Factory : public MatLabUdm::Factory {
	public:
		virtual MatLabUdm::SmartPointer create( Udm::Object &udmParent, const std::string &matLabName, int matLabObjectNo ) const {
			return MatLabUdm::SmartPointer(  new MatLabUdmChart( udmParent, matLabName, matLabObjectNo )  );
		}

		virtual const std::string &getMatLabChar( void ) const { return MatLabUdmChart::getMatLabChar(); }
		virtual const std::string &getMatLabType( void ) const { return MatLabUdmChart::getMatLabType(); }
	};
};

#endif
