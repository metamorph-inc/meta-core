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
 *  FILE NAME:	UdmEngine.hpp 
 *	SYNOPSIS:	Definition of class "UdmEngine", for setting up a Udm
 *				file to accept the translation of an SL/SF mdl file.
 *  AUTHOR:		Harmon Nine - hnine@isis.vanderbilt.edu
 *  CREATED:	12/12/07
 *  LAST UPDATE: 
 */
#ifndef _ISIS_SFCUDMENGINE
#define _ISIS_SFCUDMENGINE

#include <algorithm>
#include <map>
#include <vector>
#include <set>
#include <sstream>

#include <boost/lexical_cast.hpp>

#include "SFC.h"
#include "SFCHelper.hpp"

#include "UdmDom.h"
#include "Uml.h"


class SFCUdmEngine {
public:
	typedef std::set< SFC::Program > ProgramSet;
	typedef std::set< SFC::Class > ClassSet;

private:
	UdmDom::DomDataNetwork _domDataNetwork;
	SFC::Class _globalFunctionsClass;
	bool _isClosed;

public:
	SFCUdmEngine( void ) : _domDataNetwork( SFC::diagram ), _isClosed( true ) { }

	~SFCUdmEngine( void ) {
		closeWithUpdate();
	}

	UdmDom::DomDataNetwork &getDomDataNetwork( void ) { return _domDataNetwork; }

	Udm::Object getRootObject( void ) { return _domDataNetwork.GetRootObject(); }

	SFC::Project getRoot( void ) {
		return SFC::Project::Cast( getRootObject() );
	}

	SFC::Class getGlobalFunctionsClass( void );

	virtual void createNewDataNetwork( const std::string &sfcUdmFileName ) {	
		_domDataNetwork.CreateNew( sfcUdmFileName.c_str(), "SFC.xsd", SFC::Project::meta );
		_isClosed = false;
	}

	void openExisting( const std::string &sfcUdmFileName, const std::string &metaLocator ) {
		_domDataNetwork.OpenExisting( sfcUdmFileName, metaLocator );
		_isClosed = false;
	}

	void commitEditSequence( void ) {
		_domDataNetwork.CommitEditSequence();
	}

	void setIsClosed( bool isClosed ) { _isClosed = isClosed; }

	void closeWithUpdate( void ) {
		if ( _isClosed ) return;

		_domDataNetwork.CloseWithUpdate();
		_isClosed = true;
	}


public:
	static SFCUdmEngine &get_singleton( void ) {
		static SFCUdmEngine sfcUdmEngine;
		return sfcUdmEngine;
	}

	static SFC::Project get_root( void ) {
		return get_singleton().getRoot();
	}

	static void create_new_data_network( const std::string &udmFileName ) {
		get_singleton().createNewDataNetwork( udmFileName );
	}

	static void open_existing( const std::string &udmFileName, const std::string &metaLocator ) {
		get_singleton().openExisting( udmFileName, metaLocator );
	}

	static void commit_edit_sequence( void ) {
		get_singleton().commitEditSequence();
	}

	static void close_with_update( void ) {
		get_singleton().closeWithUpdate();
	}

};


class SFCManager {
public:
	static std::string unescapeChars( const std::string &str );
	static void numberStatement( SFC::Statement statement );
	static SFC::LocalVar createUniqueLocalVar( SFC::CompoundStatement block, const std::string &name = "ix" );
	static SFC::LocalVar createLocalVar( SFC::CompoundStatement context, const std::string &name, SFC::DT dt = SFC::DT() );

	template< typename SFCKIND >
	static SFCKIND getSFCObject( SFC::Statement statement ) {
		typedef SFCKIND SFCKind;
		while( statement != Udm::null && statement.type() != SFCKind::meta ) {
			statement = SFC::Statement::Cast( statement.GetParent() );
		}
		return statement == Udm::null ? SFCKind() : SFCKind::Cast( statement );
	}

	static SFC::Project getProject( SFC::Statement statement );
	static SFC::Program getProgram( SFC::Statement statement );
	static SFC::Class getClass( SFC::Statement statement );
	static SFC::Function getFunction( SFC::Statement statement );

	static SFC::Struct getClassStruct( SFC::Statement statement );
	static bool isContextVariable( SFC::ArgDeclBase argDeclBase );
	static SFC::Arg getFirstFunctionArg( SFC::Statement statement );
	static void printDT( SFC::DT dt, const std::string &indent = "", bool useIndent = true );
};

class SFCTypesManager {
private:

	static SFCTypesManager *&getSingletonPtr( void ) {
		static SFCTypesManager *singletonPtr = 0;
		return singletonPtr;
	}

	typedef std::map< std::string, SFC::DT > SFCTypeMap;
	typedef std::vector< SFC::DT > DTVector;
	typedef std::set< SFC::TypedEntity > TypedEntitySet;
	typedef std::set< SFC::ArgDeclRef > ArgDeclRefSet;

	SFC::CompoundStatement _typesBlock;
	SFCTypeMap _sfcTypeMap;

	class TypeUniqueFunctor {
	private:
		SFCTypesManager &_sfcTypesManager;
	public:
		TypeUniqueFunctor( SFCTypesManager &sfcTypesManager ) : _sfcTypesManager( sfcTypesManager ) { }
		void operator()( SFC::DT &dt ) { _sfcTypesManager.makeTypeUnique( dt ); }
	};

	class TransferTypeFunctor {
	private:
		SFC::DT _dt;
	public:
		TransferTypeFunctor( SFC::DT dt ) : _dt( dt ) { }
		void operator()( SFC::TypedEntity &typedEntity ) {
			typedEntity.dt() = SFC::DT();
			typedEntity.dt() = _dt;
		}
		void operator()( const SFC::TypedEntity &typedEntity ) {
			typedEntity.dt() = SFC::DT();
			typedEntity.dt() = _dt;
		}
	};

	class TransferMemberFunctor {
	private:
		SFC::LocalVar _localVar;
	public:
		TransferMemberFunctor( SFC::LocalVar localVar ) : _localVar( localVar ) { }
		void operator()( SFC::ArgDeclRef &argDeclRef ) {
			argDeclRef.argdecl() = _localVar;
		}
		void operator()( const SFC::ArgDeclRef &argDeclRef ) {
			argDeclRef.argdecl() = _localVar;
		}
	};

	class StructUniqueFunctor {
	private:
		SFCTypesManager &_sfcTypesManager;
		bool _notFirst;
		std::string _compoundName;
	public:
		StructUniqueFunctor( SFCTypesManager &sfcTypesManager ) :
		 _sfcTypesManager( sfcTypesManager ), _notFirst( false ) { }

		const std::string &getCompoundName( void ) { return _compoundName; }

		void operator()( SFC::LocalVar localVar ) {
			SFC::DT dt = localVar.dt();
			if ( _notFirst ) _compoundName += "C";
			_notFirst = true;
			_compoundName += static_cast< std::string >( localVar.name() ) + "C" + _sfcTypesManager.makeTypeUnique( dt );
		}
	};

	void makeUnique( const std::string &typeName, SFC::DT &dt );

	void transferType( SFC::DT &from, SFC::DT &to );

	class TypePrecedence {
	private:
		struct StringComparator {
			bool operator()(const std::string& a, const std::string& b) const { return a.compare( b ) < 0; }
		};

		typedef std::map< string, int, StringComparator > Map;

		Map _map;
		TypePrecedence( void ) {
			_map["float('double')"] = 1;
			_map["double"]          = 1;
			_map["float('single')"] = 2;
			_map["float"]           = 2;
			_map["single"]          = 2;
			_map["sfix(16)"]        = 3;
			_map["uint64"]          = 4;
			_map["uint(64)"]        = 4;
			_map["int64"]           = 5;
			_map["int(64)"]         = 5;
			_map["long"]            = 5;
			_map["uint32"]          = 6;
			_map["uint(32)"]        = 6;
			_map["int32"]           = 7;
			_map["int(32)"]         = 7;
			_map["int"]             = 7;
			_map["integer"]         = 7;
			_map["uint16"]          = 8;
			_map["int16"]           = 9;
			_map["int(16)"]         = 9;
			_map["short"]           = 9;
			_map["uint8"]           = 10;
			_map["int(8)"]          = 11;
			_map["int8"]            = 11;
			_map["byte"]            = 11;
			_map["char"]            = 11;
			_map["boolean"]         = 12;
		}

		static const Map &getTypePrecedence( void ) {
			static TypePrecedence typePrecedence;
			return typePrecedence._map;
		}

		static int lookup( const std::string &type ) {
			Map precedenceMap = getTypePrecedence();
			Map::const_iterator it = precedenceMap.find( type );
			if ( it == precedenceMap.end() ) {
				// TODO: throw an exception?
	//			std::cerr << "Unknown type " << type;
				return 99;
			}
			return it->second;
		}

	public:
		static std::string getDominantType( const std::string &type1, const std::string &type2 ) {
			int type1Precedence = lookup( type1 );
			int type2Precedence = lookup( type2 );

			return type1Precedence < type2Precedence ? type1 : type2;
		}
	};

public:

	SFCTypesManager( SFC::CompoundStatement compoundStatement ) :
	  _typesBlock( compoundStatement ), _sfcTypeMap() {
		DTVector dtVector = compoundStatement.DT_kind_children();
		std::for_each(  dtVector.begin(), dtVector.end(), TypeUniqueFunctor( *this )  );
	}

	struct StatementSorter {
		bool operator()( const SFC::Statement &sl, const SFC::Statement &s2 ) {
			return static_cast< __int64 >( sl.statementIndex() ) < static_cast< __int64 >( s2.statementIndex() );
		}
	};
	typedef std::set< SFC::LocalVar, StatementSorter > LocalVarSet;

	SFC::Array makeArray( int noelem ) {
		SFC::Array sfcArray = SFC::Array::Create( _typesBlock );
		sfcArray.noelem() = noelem;
		return sfcArray;
	}

	SFC::BasicType getBasicType( const std::string &typeName );
	SFC::DT getMatrix( int rows, int columns, SFC::DT dt );
	SFC::DT getMatrix( int rows, int columns, const std::string &typeName ) {
		return getMatrix(  rows, columns, getBasicType( typeName )  );
	}

	static SFC::DT getBaseDT( SFC::DT dt ) {
		while( dt != Udm::null && dt.type() == SFC::Array::meta ) dt = SFC::Array::Cast( dt ).dt();
		return dt;
	}

	static std::string getBaseDTName( SFC::DT dt ) {
		if ( dt == Udm::null ) return "NULL";

		SFC::DT baseDT = getBaseDT( dt );
		if ( baseDT.type() == SFC::BasicType::meta ) {
			SFC::BasicType basicType = SFC::BasicType::Cast( baseDT );
			return basicType.name();
		}
		SFC::Struct sfcStruct = SFC::Struct::Cast( baseDT );
		return sfcStruct.name();
	}

	static SFC::DT getDominantType( SFC::DT dt1, SFC::DT dt2 ) {
		SFC::DT baseDT1 = getSingleton().getBaseDT( dt1 );
		SFC::DT baseDT2 = getSingleton().getBaseDT( dt2 );
		assert( baseDT1.type() == SFC::BasicType::meta && baseDT2.type() == SFC::BasicType::meta );

		SFC::BasicType basicType1 = SFC::BasicType::Cast( baseDT1 );
		SFC::BasicType basicType2 = SFC::BasicType::Cast( baseDT2 );

		std::string dominantTypeName = TypePrecedence::getDominantType( basicType1.name(), basicType2.name() );

		return static_cast< std::string >( basicType1.name() ) == dominantTypeName ? basicType1 : basicType2;
	}

	SFC::DT getParameterType( const std::string &baseParameterType, SFC::DT inputDT, const std::string &defaultType = "single" ) {
		if (  baseParameterType.find( "Inherit" ) != std::string::npos  ) {
			if (  baseParameterType.find( "input" ) != std::string::npos  ) return getBaseDT( inputDT );
			return getBasicType( defaultType );
		}
		if (  baseParameterType.find( "fix" ) != std::string::npos  ) return getBasicType( defaultType );
		return getBasicType( baseParameterType );
	}

	SFC::DT combineDT( SFC::DT dt1, SFC::DT dt2 );
	SFC::DT getNewTypeArray( SFC::DT arrayDT, SFC::DT baseDT );
	SFC::DT getNewTypeArray( SFC::DT arrayDT, const std::string &baseDTName );

	typedef std::vector< int > DimensionVector;
	static DimensionVector getDimensions( SFC::DT dt );
	static DimensionVector getAllDimensions( SFC::DT dt );

	SFC::DT copyArrayDT( SFC::DT dt );
	static SFC::Array getArrayTail( SFC::DT dt );
	static void getRowsAndColumns( SFC::DT dt, __int64 &rows, __int64 &columns );

	SFC::Struct getStruct( void ) {
		static int num = 0;
		SFC::Struct sfcStruct = SFC::Struct::Create( _typesBlock );
		SFCManager::numberStatement( sfcStruct );
		sfcStruct.name() = std::string( "struct" ) + boost::lexical_cast< std::string >( ++num );
		return sfcStruct;
	}

	static SFC::LocalVar createMember( SFC::Struct sfcStruct, const std::string &memberName, SFC::DT dt );

	std::string makeTypeUnique( SFC::DT &dt );

	static void initSingleton( SFC::CompoundStatement compoundStatement ) {
		if ( getSingletonPtr() != 0 ) delete getSingletonPtr();
		getSingletonPtr() = new SFCTypesManager( compoundStatement );
	}

	static SFCTypesManager &getSingleton( void ) {
		assert( getSingletonPtr() != 0 );
		return *getSingletonPtr();
	}
};


class ArrayDTBuilder {
private:
	SFC::DT _dt;
	SFC::DT _dtBase;
	SFC::Array _sfcArray;
	bool _isFinished;

public:
	ArrayDTBuilder( SFC::DT dtBase ) :
	 _dtBase(  SFCTypesManager::getSingleton().getBaseDT( dtBase )  ), _isFinished( false ) { }

	void addArray( int noelem ) {
		assert( !_isFinished );

		SFC::Array sfcArray = SFCTypesManager::getSingleton().makeArray( noelem );
		if ( _sfcArray == Udm::null ) {
			_dt = _sfcArray = sfcArray;
		} else {
			_sfcArray.dt() = sfcArray;
			_sfcArray = sfcArray;
		}
	}

	SFC::DT getDT( void );
};

#endif
