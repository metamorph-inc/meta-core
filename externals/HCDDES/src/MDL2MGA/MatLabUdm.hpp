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
 *  FILE NAME:	MatLabUdm.hpp 
 *	SYNOPSIS:	Definition of class "MatLabUdm", provides base functionality for translating
 *				MatLab SL/SF models into equivalent GME ESMoL models.
 *  AUTHOR:		Harmon Nine - hnine@isis.vanderbilt.edu
 *  CREATED:	07/20/06
 *  LAST UPDATE: 
 *
 *
 *	DOCUMENTATION:
 *	This is the base class for classes that translate MatLab objects into UDM objects.
 *
 *
 *	BASIC ARCHITECTURE FOR DERIVED CLASSES:
 *	=======================================
 *	Each derived class should have:
 *	1)	A (static) method which returns a std::string containing a single character that is unique
 *		to the derived class, and, in particular, unique to the Matlab class of SL/SF objects
 *		(e.g. 'Simulink.Block', 'Stateflow.State') that this derived class translates to UDM.
 *		The use for this character is described below (actually, it can be a sequence of characters).
 *
 *	2)	A (static) method with returns an std:string that contains an argument list for the SL/SF
 *		'find' function, so that this function returns only those SL/SF objects processed by this
 *		derived class.
 *
 *	3)	An internal "Factory" class, derived from the internal "Factory" class of this class (MatLabUdm).
 *		This internal "Factory" class should have:
 *		A)	A "create" method that returns a Boost shared_ptr (smart pointer) to an object of the
 *			derived class.  This is used as a "virtual constructor" for classes derived from this class
 *			(MatLabUdm), and for this derived class in particular.
 *		B)	A "getMatLabChar" method that returns the std::string from the static method of the containing
 *			class, described in 1) above.
 *		C)	A "getMatLabType" method that returns the std::string from the static method of the containing
 *			class, described in 2) above.
 *					
 *
 *	Naming of MatLab variables that contain vectors of SL/SF objects:
 *	=================================================================
 *	The SL/SF objects that make up a SL/SF model in MatLab are organized hierarchically.
 *
 *	The names of the variables used to represent vectors of SL/SF objects of a given type and at
 *	a given level of the hierarchy have the following properties:
 *
 *	a)	They mimic the hierarchy.
 *	b)	Have a unique name based on the MatLab SF/SL components that the variable represents
 *		at the given level of the hierarchy.
 *		(components can be of types , etc.).
 *
 *	Example:
 *	========
 *	r0m0b2c1:	The 2nd 'Stateflow.Chart' of the 3rd 'Simulink.Block' of the
 *				1st 'Simulink.BlockDiagram' of the 1st (and only) 'Simulink.Root'
 *				('m' is for "Model", which is essentially what a 'BlockDiagram" is).
 *
 *	This works as follows:
 *	*	The MatLabUdmRoot derived class represents the object at the top of the SL/SF hierarchy.
 *		Its method, as described in 1) above, returns "r".  Note:  AS ALL VARIABLES USED IN MATLAB
 *		ARE VECTORS, r is no exception.  However, it has only one element, i.e. r(1) (1-based).
 *	*	For any given MatLab variable "var", which could represent a vector of any type of
 *		SL/SF object anywhere in the hierarchy, each element (i.e. var(1), var(2), etc.) is
 *		processed to find its children in the SL/SF model.  The children searched for are of
 *		different SL/SF types (e.g. 'Simulink.Block', 'Simulink.Parameter'), and each kind of
 *		child is processed by a given DERIVED CLASS of this class.  The actual searching is
 *		performed by the MatLab API 'find' function, with the arguments returned by the static
 *		method of the DERIVED CLASS as described in 2) above.
 *	*	The MatLab variable name used to represent the children of a given element of "var"
 *		is named as the concatenation of the following:
 *		*	"var" - the name of the parent variable
 *		*	The 0-based index (this is C++ after all) of the given element of "var"
 *		*	The unique character of the DERIVED CLASS used to process these children, as provided by
 *						this class's static method described in 1) above.
 *
 *	Examples:
 *	=========
 *	DERIVED CLASS NAME			: MatLabUdmModel
 *	PROCESSES SL/SF TYPE OBJECT	: 'Simulink.BlockDiagram'
 *	UNIQUE CHARACTER			: "m"
 *	ARGUMENTS FOR 'find'		: '-isa', 'Simulink.BlockDiagram'
 *				
 *	Name and assignment of variable to represent 'Simulink.BlockDiagram' children
 *	of the 'Simulink.Root' variable:
 *
 *	r0m = r(1).find( '-isa', 'Simulink.BlockDiagram', '-depth', 1 );
 *						
 *
 *	DERIVED CLASS NAME			: MatLabUdmState
 *	PROCESSES SL/SF TYPE OBJECT	: 'Stateflow.State'
 *	UNIQUE CHARACTER			: "s"
 *	ARGUMENTS FOR 'find'		: '-isa', 'Stateflow.State'
 *				
 *	Name and assignment of variable to represent 'Stateflow.State' children
 *	of the 3rd element of the r0m0b4b variable:
 *
 *	r0m0b4b2s = r0m0b4b(3).find( '-isa', 'Stateflow.State', '-depth', 1 );						
 *					
 */

#ifndef _ISIS_MATLABUDM
#define _ISIS_MATLABUDM

#include <ctype.h>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <list>
#include <algorithm>
#include <functional>
#include <exception>
#include <boost/version.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include "SLSF.h"
#include "MatLabEngine.hpp"
#include "ScopeSymbolTable.hpp"
#include "mfileScope.hpp"
#include "RegexCommon.hpp"
//#include "ESMoLHelper.hpp"
#include <Uml.h>
#include <stdio.h>

struct ToLower : public unary_function< int, int > {
        int operator()( int input ) { return tolower( input ); }
};

struct ToUpper : public unary_function< int, int > {
        int operator()( int input ) { return toupper( input ); }
};


using namespace SLSF;

class MatLabUdm {
public:
	typedef std::list< int > IntList;
	typedef std::list< std::string > StringList;

	static IntList getIntList( const std::string &input );
	static StringList getWordList( const std::string &input );

	class Exception : public std::exception {
	private:
		std::string _description;
	public:
		Exception( std::string description ) : _description( description ) { }
		virtual ~Exception() throw() {}
		virtual const char* what() const throw() { return _description.c_str(); }
	};

	class TopLevelModelManager {
	public:
		typedef std::map< std::string, boost::filesystem::path > ModelMap;
		typedef std::list<  std::pair< std::string, boost::filesystem::path >  > ModelList;

	private:
		ModelMap _modelMap;
		ModelList _modelList;

	public:
		boost::filesystem::path registerModel( const std::string &modelFilename );
		boost::filesystem::path registerModel( boost::filesystem::path path );

		void compileModels( void );
		void stopCompileModels( void );

		void saveModels( void ) {
			for( ModelList::iterator mdlItr = _modelList.begin() ; mdlItr != _modelList.end() ; ++mdlItr ) {
				MatLabEngine::globalEngine().executeCommand( "save_system( '" + mdlItr->first + "' )" );
			}
		}

		void deleteModels( void ) {
			for( ModelList::iterator mdlItr = _modelList.begin() ; mdlItr != _modelList.end() ; ++mdlItr ) {
				boost::filesystem::remove( mdlItr->second );
			}
		}
	};

	static TopLevelModelManager &getTopLevelModelManager( void ) {
		static TopLevelModelManager topLevelModelManager;
		return topLevelModelManager;
	}

private:
	Udm::Object _udmObject;
	std::string _matLabPrefix;
	std::string _matLabObject;
	int _matLabObjectNo;
	bool _delayedSetupCalled;

	typedef std::map< std::string, int > RefSuffixMap;
	static RefSuffixMap &getRefSuffixMap( void ) {
		static RefSuffixMap refSuffixMap;
		return refSuffixMap;
	};

	static std::string getUniqueID( std::string id ) {
		if (  getRefSuffixMap().find( id )  ==  getRefSuffixMap().end()  ) {
			getRefSuffixMap()[ id ] = 1;
		} else {
			id += std::string( "_i" ) + boost::lexical_cast< std::string >( getRefSuffixMap()[ id ]++ );
		}
		return id;
	}

	class ToLowerCase {
	private:
		std::string _lowerCase;

	public:
		void operator()( const std::string::value_type &value ) { _lowerCase.push_back(  tolower( value )  ); }
		std::string getLowerCase( void ) const { return _lowerCase; }
	};

public:
	static std::string convertToLowerCase( const std::string &str ) {
		return std::for_each( str.begin(), str.end(), ToLowerCase() ).getLowerCase();
	}
	static bool inherited( const std::string &type ) {
		std::string lowerCaseType = convertToLowerCase( type );
		return lowerCaseType.find( "inherit" ) != std::string::npos;
	}

protected:
	static std::string getRefId( void ) {
		static int id = 0;
		return std::string( "ID" ) + boost::lexical_cast< std::string >( id++ );
	}

	static std::string trim( const std::string &rhs ) {
		std::string retval = rhs;

		std::string::iterator stsItr = retval.begin();
		while( stsItr != retval.end() ) {
			if (  isspace( *stsItr )  ) stsItr = retval.erase( stsItr );
			else                        break;
		}
		stsItr = retval.end();
		while( stsItr != retval.begin() ) {
			(void)--stsItr;
			if (  isspace( *stsItr )  ) stsItr = retval.erase( stsItr );
			else                        break;
		}
		return retval;
	}

private:
	typedef std::map< std::string, std::string > ValueSubTable;

	class ValueSubTableInit {
	private:
		ValueSubTable _valueSubTable;

		void initValueSubTableDefault( void );

	public:
		ValueSubTableInit( void );

		ValueSubTable &getValueSubTable( void ) {
			return _valueSubTable;
		}
	};

	static ValueSubTable &getValueSubTable( void ) {
		static ValueSubTableInit valueSubTableInit;
		return valueSubTableInit.getValueSubTable();
	}

public:
	typedef std::set< Udm::Object > ObjectSet;

	static std::string parameterValueSub( const std::string &value ) {

		if ( getValueSubTable().empty() ) return value;

		ValueSubTable::iterator vstItr;

		if (   (  vstItr = getValueSubTable().find( value )  ) != getValueSubTable().end()   ) return vstItr->second;

		vstItr = getValueSubTable().lower_bound( value );
		if ( vstItr != getValueSubTable().end() ) {
			std::string key = vstItr->first;
			if (  key.substr( 0, value.size() ) == value  ) return vstItr->second;
		}
		for( vstItr = getValueSubTable().begin() ; vstItr != getValueSubTable().end() ; ++vstItr ) {
			if ( vstItr->first == "default" ) continue;
			if ( value.find( vstItr->first ) != std::string::npos ) return vstItr->second;
		}
		vstItr = getValueSubTable().find( "default" );
		return vstItr == getValueSubTable().end() ? value : vstItr->second;
	}

	MatLabUdm( Udm::Object udmObject, const std::string &matLabName, int matLabObjectNo ) :
	 _udmObject( udmObject ),
	 _matLabPrefix(  getMatLabPrefixString( matLabName, matLabObjectNo )  ),
	 _matLabObject(  getMatLabObjectString( matLabName, matLabObjectNo )  ),
	 _matLabObjectNo( matLabObjectNo ),
	 _delayedSetupCalled( false )
	 { }

	MatLabUdm( const std::string &matLabName, int matLabObjectNo ) :
	 _matLabPrefix(  getMatLabPrefixString( matLabName, matLabObjectNo )  ),
	 _matLabObject(  getMatLabObjectString( matLabName, matLabObjectNo )  ),
	 _matLabObjectNo( matLabObjectNo ),
	 _delayedSetupCalled( false )
	 { }

	std::string getName( ) {
		return trim(  MatLabEngine::globalEngine().getMxStringValue( getMatLabObject() + ".Name" )  );
	}

protected:
	void setUdmObject( Udm::Object udmObject ) { _udmObject = udmObject; }

	std::string forceGetUniqueName( ) {
		return getUniqueID( getName() );
	}

	std::string getUniqueName( ) {
		if (
		  MatLabEngine::globalEngine().getMxStringValue( getMatLabObject() + ".getParent()", "ReferenceBlock" ) == "" &&
		  MatLabEngine::globalEngine().getMxStringValue( getMatLabObject(), "ReferenceBlock" ) != ""
		) {
			return forceGetUniqueName();
		}
		return getName();
	}

	static std::string getMatLabPrefixString( const std::string &matLabName, int matLabObjectNo ) {
		return matLabName + boost::lexical_cast< std::string >( matLabObjectNo );
	}

	static std::string getMatLabObjectString( const std::string &matLabName, int matLabObjectNo ) {
		return matLabName + '(' + boost::lexical_cast< std::string >( matLabObjectNo + 1 ) + ')';
	}

	virtual void delayedSetupAux( void ) { }

public:
	void delayedSetup( void ) {
		if ( _delayedSetupCalled ) return;
		_delayedSetupCalled = true;
		delayedSetupAux();
	}
	virtual bool collect( void ) const { return false; }

	const Udm::Object &getUdmObject( void ) const { return _udmObject; }
	const std::string &getMatLabPrefix( void ) const { return _matLabPrefix; }
	const std::string &getMatLabObject( void ) const { return _matLabObject; }
	const int getMatLabObjectNo( void ) const { return _matLabObjectNo; }

	std::string getParentMatLabObject( void );

	virtual void cleanup( void ) { }

	bool hasMatLabField( const std::string &fieldName ) {
		return MatLabEngine::globalEngine().hasField( getMatLabObject(), fieldName );
	}

	typedef boost::shared_ptr< MatLabUdm > SmartPointer;
	typedef std::vector< SmartPointer > SPVector;

	class Factory {
	public:
		virtual MatLabUdm::SmartPointer create(
		 Udm::Object &udmObject, const std::string &matLabName, int matLabObjectNo
		) const = 0;

		virtual const std::string &getMatLabChar( void ) const = 0;
		virtual const std::string &getMatLabType( void ) const = 0;

		virtual bool exclude( const std::string &matLabObject ) const { return false; }
	};

protected:
	static SPVector buildUdm( MatLabUdm &matLabUdmParent, const Factory &factory );
	static bool hasChildren( MatLabUdm &matLabUdmParent, const Factory &factory );

public:
	static SPVector buildUdm( SmartPointer &matLabUdmParentSP, const Factory &factory ) {
		return buildUdm( *matLabUdmParentSP, factory );
	}

	static bool hasChildren( SmartPointer &matLabUdmParentSP, const Factory &factory ) {
		return hasChildren( *matLabUdmParentSP, factory );
	}

	static std::string fixSpecialChars( const std::string &str );

private:

	typedef std::map< int, TransConnector > NodeMap;

	static NodeMap &getNodeMap( void ) {
		static NodeMap nodeMap;
		return nodeMap;
	}

public:
	void registerForTransitions( void ) {
		int transConnectorId = MatLabEngine::globalEngine().getMxIntValue( getMatLabObject() + ".Id" );
		getNodeMap()[ transConnectorId ] = TransConnector::Cast( getUdmObject() );
	}

	static TransConnector findTransConnector( int transConnectorId ) {
		return getNodeMap()[ transConnectorId ];
	}

	// PORT CONNECTION MANAGEMENT
	enum PortType {
		INPUTPORT,
		OUTPUTPORT,
		ENABLEPORT,
		TRIGGERPORT,
		ACTIONPORT,
		STATEPORT
	};

	struct UdmSocket {
		int _handle;
		int _portNo;
		PortType _portType;

		UdmSocket( int handle, int portNo, PortType portType )
		 : _handle( handle ), _portNo( portNo ), _portType( portType ) { }
	};

	struct UdmSocketComparator {
		bool operator()( const UdmSocket &us1, const UdmSocket &us2 ) const {
			if ( us1._handle < us2._handle ) return true;
			if ( us1._handle > us2._handle ) return false;
			if ( us1._portNo < us2._portNo ) return true;
			if ( us1._portNo > us2._portNo ) return false;
			if ( us1._portType < us2._portType ) return true;
			return false;
		}
	};

	typedef std::map< UdmSocket, Port, UdmSocketComparator > UdmSocketMap;

	static UdmSocketMap &getUdmSocketMap( void ) {
		static UdmSocketMap udmSocketMap;
		return udmSocketMap;
	}

	void connectPort(
	 SLSF::Port port,
	 bool srcFlag,
	 const std::string &portConn,
	 const std::string &otherPortType
	);

	typedef std::set< SLSF::Block >        BlockSet;
	typedef std::set< SLSF::ConnectorRef > ConnectorRefSet;
	typedef std::set< SLSF::Line >         LineSet;
	typedef std::set< SLSF::Port >         PortSet;

	
	struct InputPortSorter {
		bool operator()( const SLSF::InputPort &p1, const SLSF::InputPort &p2 ) const {
			return static_cast< int >( p1.Number() ) < static_cast< int >( p2.Number() );
		}
	};
	typedef std::set< SLSF::InputPort, InputPortSorter > InputPortSet;
	typedef std::vector< SLSF::InputPort > InputPortVector;

	struct OutputPortSorter {
		bool operator()( const SLSF::OutputPort &p1, const SLSF::OutputPort &p2 ) const {
			return static_cast< int >( p1.Number() ) < static_cast< int >( p2.Number() );
		}
	};
	typedef std::set< SLSF::OutputPort, OutputPortSorter > OutputPortSet;
	typedef std::vector< SLSF::OutputPort > OutputPortVector;

	typedef std::vector< SLSF::EnablePort >  EnablePortVector;
	typedef std::vector< SLSF::TriggerPort > TriggerPortVector;

#if PARADIGM == CyberComposition_PARADIGM
	typedef std::vector< SLSF::EnablePort >  ActionPortVector;  // ACTION PORTS ARE TREATED AS "ENABLEPORT"S IN CyPhyML
#else
	typedef std::vector< SLSF::ActionPort >  ActionPortVector;
	typedef std::vector< SLSF::StatePort >   StatePortVector;
#endif
	template< typename PORTTYPE >
	class LineSrcPortTransTemplate {
	private:
		PortSet _srcPortSet;

	public:
		void operator()( SLSF::Line line ) {
			SLSF::Port port = SLSF::Port::Cast( line.srcLine_end() );
			if (  Udm::IsDerivedFrom( port.type(), PORTTYPE::meta )  ) {
				_srcPortSet.insert(  SLSF::Port::Cast( line.srcLine_end() )  );
			}
		}
		PortSet getSrcPortSet( void ) { return _srcPortSet; }
	};

	typedef LineSrcPortTransTemplate< SLSF::Port > LineSrcPortTrans;


	template< typename PORTTYPE >
	class LineDstPortTransTemplate {
	private:
		PortSet _dstPortSet;

	public:
		void operator()( SLSF::Line line ) {
			SLSF::Port port = SLSF::Port::Cast( line.dstLine_end() );
			if (  Udm::IsDerivedFrom( port.type(), PORTTYPE::meta )  ) {
				_dstPortSet.insert(  SLSF::Port::Cast( line.dstLine_end() )  );
			}
		}
		PortSet getDstPortSet( void ) { return _dstPortSet; }
	};

	typedef LineDstPortTransTemplate< SLSF::Port > LineDstPortTrans;
	typedef LineDstPortTransTemplate< SLSF::OutPort > LineDstOutPortTrans;

	static bool connectionExists( SLSF::Port port1, SLSF::Port port2 );

public:
	static bool onlyTopLevel;

private:
	static bool toBuildUdm(const Udm::Object &udmParent);

	typedef std::map< std::string, SLSF::TypeStruct > BusObjectMap;
	static BusObjectMap &getBusObjectMap( void ) {
		static BusObjectMap busObjectMap;
		return busObjectMap;
	}
	static SLSF::TypeStruct makeBusObjectTypeStructAux( const std::string &busObjectName );

public:
	static SLSF::TypeStruct makeBusObjectTypeStruct( const std::string &busObjectName );

	class Assignment {
	private:
		std::string _varName;
		std::string _varMember;
		std::string _rhs;

	public:
		void setVarName( const std::string &varName ) { _varName = varName; }
		void setVarMember( const std::string &varMember ) { _varMember = varMember; }
		void setRHS( const std::string &rhs ) { _rhs = rhs; }

		const std::string &getVarName( void ) const { return _varName; }
		const std::string &getVarMember( void ) const { return _varMember; }
		const std::string &getRHS( void ) const { return _rhs; }

		void clear( void ) {
			_varName.clear();
			_varMember.clear();
			_rhs.clear();
		}
	};

	typedef std::list< Assignment > AssignmentList;

	void performParallelAssignment( AssignmentList &assigmentList );

	class MatLabUniqueVarNameFunctor : public ScopeSymbolTable::UniqueVarNameFunctor {
	public:
		typedef ScopeSymbolTable::UniqueVarNameFunctor Super;
		typedef Super::SP SP;

		virtual std::string operator()( const std::string &varName ) {
			return MatLabEngine::globalEngine().getUniqueVarName( varName );
		}
		virtual SP clone( void ) const { return SP(  new MatLabUniqueVarNameFunctor( *this )  ); }
	};

	class MatLabDestroyUniqueVarFunctor : public ScopeSymbolTable::DestroyUniqueVarFunctor {
	public:
		typedef ScopeSymbolTable::DestroyUniqueVarFunctor Super;
		typedef Super::SP SP;

		virtual void operator()( const std::string &varName ) const {
			MatLabEngine::globalEngine().clearVar( varName );
		};
		virtual SP clone( void ) const { return SP(  new DestroyUniqueVarFunctor( *this )  ); }
	};

	void performMaskInitialization( const MatLabUdm &matLabUdm );

	class ScopeSymbolTableInit {
	private:
		ScopeSymbolTable _scopeSymbolTable;

	public:
		ScopeSymbolTableInit( void ) {
			_scopeSymbolTable.setUniqueVarNameFunctor( MatLabUniqueVarNameFunctor() );
			_scopeSymbolTable.setDestroyUniqueVarFunctor( MatLabDestroyUniqueVarFunctor() );
		}

		ScopeSymbolTable &getScopeSymbolTable( void ) { return _scopeSymbolTable; }
	};

	static ScopeSymbolTable &getScopeSymbolTable( void ) {
		static ScopeSymbolTableInit scopeSymbolTableInit;
		return scopeSymbolTableInit.getScopeSymbolTable();
	}

	static MFileScope &getMFileScope( void ) {
		static MFileScope mFileScope;
		return mFileScope;
	}
};

#endif
