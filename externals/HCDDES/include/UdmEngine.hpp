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
 *  CREATED:	07/20/06
 *  LAST UPDATE: 
 */
#ifndef _ISIS_UDMENGINE
#define _ISIS_UDMENGINE

#include <sstream>
#include <iostream>
#include <iomanip>
#include <set>
#include <SLSF.h>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include "Uml.h"

class UdmEngine {

public:
	typedef std::set< std::string > StringSet;

#if PARADIGM != CyberComposition_PARADIGM
	typedef std::vector< SLSF::Component > ComponentVector;
	typedef std::vector< SLSF::Dataflow > DataflowVector;
	typedef std::vector< SLSF::DesignFolder > DesignFolderVector;
	typedef std::vector< SLSF::ModelsFolder > ModelsFolderVector;
	typedef std::vector< SLSF::Stateflow > StateflowVector;
	typedef std::vector< SLSF::SubsystemRef > SubsystemRefVector;
	typedef std::vector< SLSF::SystemTypes > SystemTypesVector;
#endif

	typedef std::vector< SLSF::Parameter > ParameterVector;
	typedef std::vector< SLSF::Port > PortVector;
	typedef std::vector< SLSF::State > StateVector;
	typedef std::set< SLSF::Subsystem > SubsystemSet;
	typedef std::vector< SLSF::Types > TypesVector;

private:
	Udm::SmartDataNetwork _smartDataNetwork;
	bool _isClosed;
	std::string _modelName;

#if SLSF_MODEL_LEVEL == 3
	SLSF::ModelsFolder _modelsFolder;
#endif

	Udm::Object _dataflowParent;

#if PARADIGM != CyberComposition_PARADIGM
	SLSF::Stateflow _stateflow;
#endif

	SLSF::State _state;
	SLSF::Types _types;

	enum Backend { XML, MGA, MEM, UNKNOWN };
	Backend _backend;


	void setBackend( const std::string &udmFileName ) {
		std::string::size_type dotPos = udmFileName.rfind( '.' );
		if ( dotPos == std::string::npos ) { _backend = UNKNOWN; return; }

		std::string suffix = udmFileName.substr( dotPos + 1 );

		if ( suffix == "xml" )      _backend = XML;
		else if ( suffix == "mem" ) _backend = MEM;
		else                        _backend = MGA;
	}

	void setIsClosed( bool isClosed ) { _isClosed = isClosed; }

public:
	UdmEngine( void ) : _smartDataNetwork( SLSF_ROOT::diagram ), _isClosed( true ), _modelName( "Model" ), _backend( UNKNOWN ) { }

	~UdmEngine( void ) { closeWithUpdate(); }

	void createNew( const std::string &udmFileName );
	void openExisting( const std::string &udmFileName );
	Backend getBackend( void ) { return _backend; }

	void commitEditSequence( void ) {
		_smartDataNetwork.CommitEditSequence();
	}

	void closeNoUpdate( void );
	void closeWithUpdate( void );

	Udm::Object getRootObject( void ) { return _smartDataNetwork.GetRootObject(); }

	SLSF_ROOT::RootFolder getRootFolder( void ) {
		return SLSF_ROOT::RootFolder::Cast( getRootObject() );
	}

#if SLSF_MODEL_LEVEL == 3
	SLSF::ModelsFolder getModelsFolder( void );
#endif

	void setDataflowParent( Udm::Object object ) {
		_dataflowParent = object;
	}
	Udm::Object getDataflowParent( void ) {
		return _dataflowParent;
	}

#if PARADIGM == CyberComposition_PARADIGM
	SLSF_ROOT::SimulinkWrapper UdmEngine::getRootComponent( void );
#else
	SLSF::Stateflow getStateflow( void );
	SLSF::Dataflow getDataflow( void );
#endif

	SLSF::State getTopLevelState( void );
	SLSF::Types getTypes( void );

	SLSF::Matrix makeDefaultMatrix( void );

	static UdmEngine &get_singleton( void ) {
		static UdmEngine udmEngine;
		return udmEngine;
	}

	void setModelName( std::string modelName ) { _modelName = modelName; }
	std::string getModelName( void ) { return _modelName; }

#if SLSF_MODEL_LEVEL == 3
	static SLSF::ModelsFolder get_ModelsFolder( void ) { return get_singleton().getModelsFolder(); }
#endif

public:
	
	static void set_Model_name( std::string modelName ) { get_singleton().setModelName( modelName ); }
	static std::string get_Model_name( void ) { return get_singleton().getModelName(); }

	static void create_new( const std::string &udmFileName ) {
		get_singleton().createNew( udmFileName );
	}

	static void open( const std::string &udmFileName ) {
		if (  boost::filesystem::exists( udmFileName )  ) open_existing( udmFileName );
		else                                              create_new( udmFileName );
	}

	static void open_existing( const std::string &udmFileName ) {
		get_singleton().openExisting( udmFileName );
	}

	static void commit_edit_sequence( void ) {
		get_singleton().commitEditSequence();
	}

	static void close( void ) {
		elimNonRefTypeBase();
		get_singleton().closeWithUpdate();
	}

	static SLSF_ROOT::RootFolder &get_root_folder( void ) {
		static SLSF_ROOT::RootFolder rootFolder( get_singleton().getRootFolder() );
		return rootFolder;
	}

	static Backend get_backend( void ) {
		return get_singleton().getBackend();
	}

	static std::string escape_chars( const std::string &str );

	static SLSF::Port copy( SLSF::Port port, SLSF::Block block );
	static SLSF::Block copy( SLSF::Block block, SLSF::Subsystem subsystem );

	static void copyData( SLSF::Data inputData, SLSF::Data outputData );
	static void copyEvent( SLSF::Event inputEvent, SLSF::Event outputEvent );
	static void copyMatrix( SLSF::Matrix inputMatrix, SLSF::Matrix outputMatrix );
	static void copyState( SLSF::State inputState, SLSF::State outputState );
	static void copyTransition( SLSF::Transition inputTransition, SLSF::Transition outputTransition );
	static void copyBlock( SLSF::Block inputBlock, SLSF::Block outputBlock );

#if PARADIGM != CyberComposition_PARADIGM
	static SLSF::Dataflow get_Dataflow( void ) {
		return get_singleton().getDataflow();
	}

	static SLSF::Stateflow get_Stateflow( void ) {
		return get_singleton().getStateflow();
	}
#endif
	
	static std::string getModelName( Udm::Object object );

	static SLSF::State get_TopLevel_State( void ) {
		return get_singleton().getTopLevelState();
	}


	//
	// TYPE MANAGEMENT
	//
	static SLSF::Types get_Types( void ) {
		return get_singleton().getTypes();
	}

private:
	class TypeBaseRefSorter {
	public:
		bool operator()( const SLSF::TypeBaseRef &tbr1, const SLSF::TypeBaseRef &tbr2 ) {
			return static_cast< int >( tbr1.MemberIndex() )  <  static_cast< int >( tbr2.MemberIndex() );
		}
	};
	typedef std::set< SLSF::TypeBaseRef, TypeBaseRefSorter > TypeBaseRefSet;
	typedef std::set< SLSF::TypeBaseRef > TypeBaseRefUnorderedSet;

public:
	static std::string getTypeBaseString( SLSF::Matrix matrix );

	static std::string getTypeBaseString( SLSF::TypeBase typeBase ) {
		return typeBase.type() == SLSF::Matrix::meta ?
		 getTypeBaseString(  SLSF::Matrix::Cast( typeBase )  ) :
		 getTypeBaseString(  SLSF::TypeStruct::Cast( typeBase )  );
	}

	static std::string getTypeBaseString( SLSF::TypeBaseRef typeBaseRef ) {
		return getTypeBaseString(  SLSF::TypeBase::Cast( typeBaseRef.ref() )  );
	}

	static std::string getTypeBaseString( SLSF::TypeStruct typeStruct );

	static std::string getTypeBaseString( const std::string &type, __int64 rows, __int64 columns );

private:
	static SLSF::Matrix make_default_matrix( void ) {
		return get_singleton().makeDefaultMatrix();
	}

	typedef std::map< std::string, SLSF::TypeBase > StringTypeBaseMap;
	typedef std::vector< SLSF::TypeBase > TypeBaseVector;
	typedef std::set< SLSF::TypeBase > TypeBaseSet;

	class StringTypeBaseMapInit {
	private:
		StringSet _takenStructNames;
		StringTypeBaseMap _stringTypeBaseMap;
		SLSF::Matrix _defaultMatrix;

		StringTypeBaseMap &getStringTypeBaseMap( void ) { return _stringTypeBaseMap; }

		std::string getNewStructName( void ) {
			static int structNo = -1;
			std::string structName = std::string( "struct" ) + boost::lexical_cast< std::string >( ++structNo );
			while( _takenStructNames.find( structName ) != _takenStructNames.end() ) {
				structName = std::string( "struct" ) + boost::lexical_cast< std::string >( ++structNo );
			}
			return structName;
		}

	public:
		StringTypeBaseMapInit( void );

		SLSF::TypeBase getTypeBase( const std::string &typeName ) {
			StringTypeBaseMap::iterator stmItr = getStringTypeBaseMap().find( typeName );
			return stmItr == getStringTypeBaseMap().end() ? SLSF::TypeBase() : stmItr->second;
		}

		void integrateTypeBase( SLSF::TypeBase &typeBase );

		SLSF::Matrix getDefaultMatrix( void ) { return _defaultMatrix; }

		void cleanup( void );
	};

	static StringTypeBaseMapInit &getStringTypeBaseMapInit( void ) {
		static StringTypeBaseMapInit stringTypeBaseMapInit;
		return stringTypeBaseMapInit;
	}

public:

	static SLSF::Matrix getDefaultMatrix( void ) {
		return getStringTypeBaseMapInit().getDefaultMatrix();
	}

	static void integrateTypeBase( SLSF::TypeBase &typeBase ) {
		getStringTypeBaseMapInit().integrateTypeBase( typeBase );
	}

	static SLSF::Matrix createMatrix( const std::string &type, __int64 rows, __int64 columns ) {

		SLSF::Matrix slsfMatrix = SLSF::Matrix::Create( UdmEngine::getUdmTypesRoot() );
		slsfMatrix.Type() = type;
		slsfMatrix.rows() = rows;
		slsfMatrix.columns() = columns;

		integrateTypeBase( slsfMatrix );

		return slsfMatrix;
	}

	static SLSF::TypeBase getTypeBase( const std::string &typeName ) {
		return getStringTypeBaseMapInit().getTypeBase( typeName );
	}

	static SLSF::TypeBase getTypeBase( const std::string &type, __int64 rows, __int64 columns ) {
		return getStringTypeBaseMapInit().getTypeBase(  getTypeBaseString( type, rows, columns )  );
	}

	static void elimNonRefTypeBase( void ) {
		getStringTypeBaseMapInit().cleanup();
	}

private:
	static bool isStructName( const std::string &name ) {
		if ( name.length() < 7 ) return false;
		if ( name.substr( 0, 6 ) != "struct" ) return false;
		std::string digits = name.substr( 6 );
		for( std::string::iterator strItr = digits.begin() ; strItr != digits.end() ; ++strItr ) {
			if (  !isdigit( *strItr )  ) return false;
		}
		return true;
	}

public:
	static SLSF::Types getUdmTypesRoot( void ) {
		getStringTypeBaseMapInit();  // MAKE SURE StringTypeBaseMap IS INITIALIZED;
		return UdmEngine::get_Types();
	}
};

#endif
