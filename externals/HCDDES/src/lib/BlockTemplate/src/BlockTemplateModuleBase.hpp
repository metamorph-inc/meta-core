#ifndef _BLOCKTEMPLATEMODULEBASE_HPP_
#define _BLOCKTEMPLATEMODULEBASE_HPP_

#include <iostream>
#include <iomanip>
#include <string>

#include <ctype.h>

#include "SLSF.h"

//#include <ESMoL.h>
#include <ESMoLHelper.hpp>
#include <SFC.h>
#include <SFCHelper.hpp>

#include <boost/lexical_cast.hpp>
#define BOOST_PYTHON_STATIC_LIB 1
#define BOOST_PYTHON_NO_LIB
#include <boost/python.hpp>
#include <boost/shared_ptr.hpp>

#include "SFCUdmEngine.hpp"
#include "mfile2SFC.hpp"


std::string convertToMatLabIdentifierName( const std::string &name );

class DTBase {
	
	friend class DT;

private:
	SFC::DT _dt;

protected:
	DTBaseWP _wp;

	void setWP( DTBaseWP wp ) { _wp = wp; }
	DTBaseWP getWP( void ) { return _wp; }

	void setSFCType( SFC::DT dt ) { _dt = dt; }

public:
	DTBase( void ) { }
	DTBase( const std::string &typeName ) : _dt(  SFCTypesManager::getSingleton().getBasicType( typeName )  ) { }
	DTBase( SFC::DT dt ) : _dt( dt ) { }

	std::string getName( void ) const { return isNull() ? "NULL" : static_cast< std::string >( _dt.name() ); }
	std::string __str__( void ) const { return getName(); }
	
	bool isNull( void ) const { return _dt == Udm::null; }
	bool isBasicType( void ) const { return !isNull() && _dt.type() == SFC::BasicType::meta; }
	bool isArray( void ) const { return !isNull() && _dt.type() == SFC::Array::meta; }
	bool isStruct( void ) const { return !isNull() && _dt.type() == SFC::Struct::meta; }
	std::string getTypeString( void ) const;

	int getDimension( void ) const { 
		return isArray() ? int(  SFC::Array::Cast( _dt ).noelem()  ) : 0;
	}

	DT getSubArray( void ) {
		return isArray() ? DT(  SFC::Array::Cast( _dt ).dt()  ) : DT( *this );
	}

	int getNoElements( void ) const;

	DT getElementType( void ) const {
		return DT(  SFCTypesManager::getBaseDT( _dt )  );
	}
	void setElementType( DT dt ) {
		_dt = SFCTypesManager::getSingleton().getNewTypeArray( _dt, dt.getSFCType() );
	}
	boost::python::list getMembers( void ) const;

	boost::python::list getDimensions( void ) const;
	boost::python::list getAllDimensions( void ) const;

	SFC::DT getSFCType( void ) const { return _dt; }

	bool operator==( const DTBase &rhs ) { return _dt == rhs._dt; }
	bool operator!=( const DTBase &rhs ) { return !operator==( rhs ); }
};


class BasicTypeBase : public DTBase {
public:
	BasicTypeBase( const std::string &type ) : DTBase(  SFCTypesManager::getSingleton().getBasicType( type )  ) { }
};

boost::python::list flatten( boost::python::object list );

class ArrayBase : public DTBase {
public:
	typedef std::list< int > IntList;

private:
	boost::python::list normalizeList( boost::python::list list );

public:
	ArrayBase( boost::python::tuple args, boost::python::dict keywordArgs );
};

class StructBase : public DTBase {
public:
	StructBase( boost::python::tuple args, boost::python::dict keywordArgs );
};

class ArgDeclBaseBase {

	friend class ArgDeclBase;

protected:
	ArgDeclBaseBaseWP _wp;

	SFC::ArgDeclBase _sfcArgDeclBase;
	bool _partOfContext;

	std::string _symbolTableName;

	void setWP( ArgDeclBaseBaseWP wp ) { _wp = wp; }
	ArgDeclBaseBaseWP getWP( void ) { return _wp; }

	void registerInSymbolTable( void ) const;

	void setSymbolTableName( const std::string &name ) {
		_symbolTableName = name;
		registerInSymbolTable();
	}

public:
	ArgDeclBaseBase( SFC::ArgDeclBase sfcArgDeclBase, bool partOfContext = false );

	virtual ~ArgDeclBaseBase( void ) { }

	bool isNull( void ) { return _sfcArgDeclBase == Udm::null; }

	virtual std::string getName( void ) {
		if ( _sfcArgDeclBase == Udm::null ) {
			std::cerr << "Warning:  Attempt to get name of null variable" << std::endl;
		}

		registerInSymbolTable();

		std::string name( _symbolTableName );
		if ( _partOfContext ) name = std::string( "context." ) + name;
		return name;
	}

	std::string __str__( void ) { return getName(); }

	virtual SFC::DT getSFCType( void ) { return _sfcArgDeclBase.dt(); }
	virtual SFC::ArgDeclBase getSFCArgDeclBase( void ) { return _sfcArgDeclBase; }

	virtual DT getType( void ) {
		if ( _sfcArgDeclBase == Udm::null ) {
			std::cerr << "Warning:  Attempt to take type of null variable" << std::endl;
			return SFC::DT();
		}
		return DT( getSFCType() );
	}

	void setType( DT dt ) {
		if ( _sfcArgDeclBase == Udm::null ) {
			std::cerr << "Warning:  Attempt to set type of null variable" << std::endl;
			return;
		}
		SFC::DT sfcDTBase = dt.getSFCType();
		SFCTypesManager::getSingleton().makeTypeUnique( sfcDTBase );
		_sfcArgDeclBase.dt() = sfcDTBase;
	}

	void setArgDeclBase( ArgDeclBaseBase &argDeclBaseBase ) {
		_sfcArgDeclBase = argDeclBaseBase._sfcArgDeclBase;
		_symbolTableName = _sfcArgDeclBase.name();
	}

	virtual int getNoElements( void ) { return getType().getNoElements(); }

	virtual DT getElementType( void ) { return getType().getElementType(); }
	virtual void setElementType( DT dt ) {
		SFC::DT sfcDT = SFCTypesManager::getSingleton().getNewTypeArray( getSFCType(), dt.getSFCType() );
		setType( sfcDT );
	}

	virtual DT getSubArray( void ) { return getType().getSubArray(); }
	virtual boost::python::list getMembers( void ) { return getType().getMembers(); }

	virtual int getDimension( void ) { return getType().getDimension(); }
	virtual boost::python::list getDimensions( void ) { return getType().getDimensions(); }
	virtual boost::python::list getAllDimensions( void ) { return getType().getAllDimensions(); }

	void setInitialValue( const std::string &initialValue );

	virtual bool isBasicType( void ) { return getType().isBasicType(); }
	virtual bool isArray( void ) { return getType().isArray(); }
	virtual bool isStruct( void ) { return getType().isStruct(); }

	virtual std::string __add__( const std::string &rhs ) { return getName() + rhs; }
	virtual std::string __radd__( const std::string &rhs ) { return rhs + getName(); }
};


class PrimitiveBlockBaseProxy;
typedef boost::shared_ptr< PrimitiveBlockBaseProxy > PrimitiveBlockBaseProxySP;

class IOBase : public ArgDeclBaseBase {
public:
	typedef ArgDeclBaseBase Super;

private:
	PrimitiveBlockBaseProxySP _primitiveBlockBaseProxySP;

	bool _convertToArray;
	bool _alreadyAccessed;

	void checkFirstAccess( void );

protected:
	void convertToArrayBase( bool inputConversion );

public:
	IOBase( PrimitiveBlock &primitiveBlock, SFC::ArgDeclBase sfcArgDeclBase );
	IOBase( PrimitiveBlock &primitiveBlock, ArgDeclBase argDeclBase );

	virtual ~IOBase( void ) { }

	void setKeepStruct( void ) {
		if ( _alreadyAccessed ) {
			std::cerr << "WARNING:  ATTEMPT TO PRESERVE STRUCT DATATYPE ON VARIABLE (\"" <<
			 getName() << "\") THAT HAS ALREADY BEEN ACCESSED." << std::endl;
		} else {
			_convertToArray = false;
		}
	}
	bool getKeepStruct( void ) { return !_convertToArray; }

	virtual void convertToArray( void ) {
		std::cout << "WARNING:  IOBase::convertToArray SHOULD NOT BE CALLED (AND IT WAS)." << std::endl;
	}

	virtual std::string getName( void ) { checkFirstAccess(); return Super::getName(); }

	void alias( const std::string &name );

	virtual SFC::DT getSFCType( void ) { checkFirstAccess(); return Super::getSFCType(); }
	virtual SFC::ArgDeclBase getSFCArgDeclBase( void ) { checkFirstAccess(); return Super::getSFCArgDeclBase(); }

	virtual DT getType( void ) { checkFirstAccess(); return Super::getType(); }

	virtual int getNoElements( void ) { checkFirstAccess(); return Super::getNoElements(); }

	virtual DT getElementType( void ) { checkFirstAccess(); return Super::getElementType(); }
	virtual void setElementType( DT dt ) { checkFirstAccess(); Super::setElementType( dt ); }

	virtual DT getSubArray( void ) { checkFirstAccess(); return Super::getSubArray(); }
	virtual boost::python::list getMembers( void ) { checkFirstAccess(); return Super::getMembers(); }

	virtual int getDimension( void ) { checkFirstAccess(); return Super::getDimension(); }
	virtual boost::python::list getDimensions( void ) { checkFirstAccess(); return Super::getDimensions(); }
	virtual boost::python::list getAllDimensions( void ) { checkFirstAccess(); return Super::getAllDimensions(); }

	virtual bool isBasicType( void ) { checkFirstAccess(); return Super::isBasicType(); }
	virtual bool isArray( void ) { checkFirstAccess(); return Super::isArray(); }
	virtual bool isStruct( void ) { checkFirstAccess(); return Super::isStruct(); }

	virtual std::string __add__( const std::string &rhs ) { checkFirstAccess(); return Super::__add__( rhs ); }
	virtual std::string __radd__( const std::string &rhs ) { checkFirstAccess(); return Super::__radd__( rhs ); }
};

class InputBase : public IOBase {
public:
	typedef IOBase Super;

	virtual void convertToArray( void ) { convertToArrayBase( true ); }

	InputBase( PrimitiveBlock &primitiveBlock, SFC::ArgDeclBase sfcArgDeclBase ) :
	 Super( primitiveBlock, sfcArgDeclBase ) { }

};

class OutputBase : public IOBase {
public:
	typedef IOBase Super;

	virtual void convertToArray( void ) { convertToArrayBase( false ); }

	OutputBase( PrimitiveBlock &primitiveBlock, SFC::ArgDeclBase sfcArgDeclBase ) :
	 Super( primitiveBlock, sfcArgDeclBase ) { }
};

class ParameterBase {
private:
	typedef boost::shared_ptr< ArgDeclBase > ArgDeclBaseSP;

	PrimitiveBlockBaseProxySP _primitiveBlockBaseProxySP;
	SLSF::Parameter _parameter;
	ArgDeclBaseSP _argDeclBaseSP;

public:
	ParameterBase( PrimitiveBlock &primitiveBlock, SLSF::Parameter parameter );

	std::string getName( void ) const { return _parameter.name(); }
	std::string getValue( void ) const { return _parameter.Value(); }
	std::string __str__( void ) const { return getValue(); }
	SLSF::Parameter getESMoLParameter( void ) const { return _parameter; }

	ArgDeclBase placeInContext( void );
	ArgDeclBase placeInContext( DT dt );

	bool operator==( std::string bpString ) const {
		return __str__() == bpString;
	}
	bool operator!=( std::string bpString ) const {
		return !operator==( bpString );
	}
};

class BlockBase {

	friend class Block;

protected:
	SLSF::Block _block;

	BlockBaseWP _wp;

	void setWP( BlockBaseWP wp ) { _wp = wp; }
	BlockBaseWP getWP( void ) { return _wp; }


public:
	BlockBase( SLSF::Block block ) : _block( block ) { }

	static int getUniqueNumber( void ) {
		static int uniqueNumber = 0;
		return uniqueNumber++;
	}

	Block getParentBlock( void );
	bool hasParentBlock( void ) {
		return !isNull() && _block.GetParent() != Udm::null &&
		 Udm::IsDerivedFrom( _block.GetParent().type(), SLSF::Block::meta );
	}
	bool isNull( void ) { return _block == Udm::null; }

	std::string getName( void ) { return isNull() ? "" : static_cast< std::string >( _block.name() ); }

	static std::string createMatlabVariable( const std::string &name ) {
		return name + "_" + boost::lexical_cast< std::string >( getUniqueNumber() );
	}

	int __hash__( void ) { return isNull() ? 0 : _block.uniqueId(); }
	bool operator==( const BlockBase &block ) { return !isNull() && _block.uniqueId() == block._block.uniqueId(); }
	bool operator!=( const BlockBase &block ) { return !operator==( block ); }
};

class PrimitiveBlockBaseProxy {
private:
	typedef std::vector< std::string > StatementVector;

	PrimitiveBlockBaseWP _primitiveBlockBaseWP;
	SFC::CompoundStatement _context;
	std::string _contextArgName;
	bool _zeroIndexingFlag;
	bool _acceptCSyntaxFlag;
	bool _keepStructsFlag;
	bool _oldKeepStructsFlag;

	StatementVector _preStatementVector;
	StatementVector _postStatementVector;

public:
	PrimitiveBlockBaseProxy( PrimitiveBlockBaseWP primitiveBlockBaseWP, SFC::CompoundStatement context );

	SFC::CompoundStatement getContext( void ) { return _context; }
	std::string getContextArgName( void ) { return _contextArgName; }

	void setZeroIndexing( void ) { _zeroIndexingFlag = true; }
	void setOneIndexing( void ) { _zeroIndexingFlag = false; }

	void setAcceptCSyntax( void ) { _acceptCSyntaxFlag = true; }
	bool getAcceptCSyntaxFlag( void ) { return _acceptCSyntaxFlag; }

	void setKeepStructs( void ) {
		_oldKeepStructsFlag = _keepStructsFlag;
		_keepStructsFlag = true;
	}
	bool getKeepStructs( void ) { return _keepStructsFlag; }

	bool getZeroIndexingFlag( void ) { return _zeroIndexingFlag; }

	PrimitiveBlockBaseSP getPrimitiveBlockBaseSP( void ) { return _primitiveBlockBaseWP.lock(); }

	ArgDeclBase createGlobalVariable( const std::string &name, const ParameterBase &value );
	ArgDeclBase createContextVariable( const std::string &name, DTBase dt );
	ArgDeclBase createContextVariable( const std::string &name, const std::string &typeName );
	ArgDeclBase createVariable( const std::string &name, DTBase dt );
	ArgDeclBase createVariable( const std::string &name, const std::string &typeName );

	void addStatement( const std::string &statement, bool isPreStatement ) {
		if ( isPreStatement ) _preStatementVector.push_back( statement );
		else                  _postStatementVector.push_back( statement );
		_keepStructsFlag = _oldKeepStructsFlag;
	}

	std::string getPreStatements( void ) {
		std::string retval;
		for( StatementVector::iterator stvItr = _preStatementVector.begin() ; stvItr != _preStatementVector.end() ; ++stvItr ) {
			retval += *stvItr + "\n";
		}
		return retval;
	}

	std::string getPostStatements( void ) {
		std::string retval;
		for( StatementVector::iterator stvItr = _postStatementVector.begin() ; stvItr != _postStatementVector.end() ; ++stvItr ) {
			retval += *stvItr + "\n";
		}
		return retval;
	}
};

class PrimitiveBlockBase : public BlockBase {

public:
	typedef BlockBase Super;

private:
	SFC::CompoundStatement _context;
	PrimitiveBlockBaseProxySP _primitiveBlockBaseProxySP;

	ArgDeclBaseList _rawInputs;
	ArgDeclBaseList _rawOutputs;
	ParameterMap _rawParameters;

	boost::python::list _inputs;
	boost::python::list _outputs;
	boost::python::dict _parameters;

	bool _hasParameterBaseType;
	SFC::BasicType _parameterBaseType;

	class ToLowerCase {
	private:
		std::string _lowerCase;

	public:
		void operator()( const std::string::value_type &value ) { _lowerCase.push_back(  tolower( value )  ); }
		std::string getLowerCase( void ) const { return _lowerCase; }
	};

	static std::string convertToLowerCase( const std::string &str ) {
		return std::for_each( str.begin(), str.end(), ToLowerCase() ).getLowerCase();
	}
	static bool notInherited( const std::string &type ) {
		std::string lowerCaseType = convertToLowerCase( type );
		return lowerCaseType.find( "inherit" ) == std::string::npos;
	}

public:
	PrimitiveBlockBase(
	 SLSF::Block block, SFC::CompoundStatement context, ArgDeclBaseList &inputs, ArgDeclBaseList &outputs, ParameterMap &parameters
	);

	PrimitiveBlockBaseProxySP getProxySP( void ) { return _primitiveBlockBaseProxySP; }

	void initialize( void );

	SFC::CompoundStatement getContext( void ) { return _primitiveBlockBaseProxySP->getContext(); }
	std::string getContextArgName( void ) { return _primitiveBlockBaseProxySP->getContextArgName(); }

	void setZeroIndexing( void ) { _primitiveBlockBaseProxySP->setZeroIndexing(); }
	void setOneIndexing( void ) { _primitiveBlockBaseProxySP->setOneIndexing(); }

	void setAcceptCSyntax( void ) { _primitiveBlockBaseProxySP->setAcceptCSyntax(); }
	bool getAcceptCSyntaxFlag( void ) { return _primitiveBlockBaseProxySP->getAcceptCSyntaxFlag(); }

	void setKeepStructs( void ) { _primitiveBlockBaseProxySP->setKeepStructs(); }
	bool getKeepStructs( void ) { return _primitiveBlockBaseProxySP->getKeepStructs(); }

	bool getZeroIndexingFlag( void ) { return _primitiveBlockBaseProxySP->getZeroIndexingFlag(); }

	SFC::BasicType getParameterBaseType( void );

	boost::python::list getInputs( void ) { return _inputs; }
	boost::python::list getOutputs( void ) { return _outputs; }
	boost::python::dict getParameters( void ) { return _parameters; }

	ArgDeclBase createGlobalVariable( const std::string &name, const ParameterBase &value ) {
		return _primitiveBlockBaseProxySP->createGlobalVariable( name, value );
	}
	ArgDeclBase createContextVariable( const std::string &name, DTBase dt ) {
		return _primitiveBlockBaseProxySP->createContextVariable( name, dt );
	}
	ArgDeclBase createContextVariable( const std::string &name, const std::string &typeName ) {
		return _primitiveBlockBaseProxySP->createContextVariable( name, typeName );
	}
	ArgDeclBase createVariable( const std::string &name, DTBase dt ) {
		return _primitiveBlockBaseProxySP->createVariable( name, dt );
	}
	ArgDeclBase createVariable( const std::string &name, const std::string &typeName ) {
		return _primitiveBlockBaseProxySP->createVariable( name, typeName );
	}

	void addStatement( const std::string &statement, bool isPreStatement ) {
		_primitiveBlockBaseProxySP->addStatement( statement, isPreStatement );
	}

	std::string getPreStatements( void ) { return _primitiveBlockBaseProxySP->getPreStatements(); }
	std::string getPostStatements( void ) { return _primitiveBlockBaseProxySP->getPostStatements(); }
};

#endif
