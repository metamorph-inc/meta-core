#ifndef _BLOCKTEMPLATEMODULE_HPP_
#define _BLOCKTEMPLATEMODULE_HPP_

#include <string>

#define BOOST_PYTHON_STATIC_LIB 1
#define BOOST_PYTHON_NO_LIB
#include <Python.h>
#include <boost/python.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

//#include <ESMoL.h>
#include <SFC.h>
#include "SLSF.h"

boost::python::object raw_constructor( boost::python::tuple args, boost::python::dict keywordArgs );

class DTBase;
typedef boost::shared_ptr< DTBase > DTBaseSP;
typedef boost::weak_ptr< DTBase > DTBaseWP;

class DT {
protected:
	DTBaseSP _dtBaseSP;

public:
	DT( void );
	DT( const std::string &typeName );
	DT( SFC::DT dt );
	DT( DTBase &dtBase );
	DT( DTBase *dtBasePtr );

	DTBase &getDTBase( void );
	const DTBase &getDTBase ( void ) const;

	std::string getName( void ) const;
	std::string __str__( void ) const;

	bool isNull( void ) const;
	bool isBasicType( void ) const;
	bool isArray( void ) const;
	bool isStruct( void ) const;
	std::string getTypeString( void ) const;

	int getDimension( void ) const;
	DT getSubArray( void );
	int getNoElements( void ) const;

	DT getElementType( void ) const;
	void setElementType( DT dt );
	boost::python::list getMembers( void ) const;

	boost::python::list getDimensions( void ) const;
	boost::python::list getAllDimensions( void ) const;

	SFC::DT getSFCType( void ) const;

	bool operator==( const DT &rhs );
	bool operator!=( const DT &rhs );
};


class BasicTypeBase;

class BasicType : public DT {
public:
	BasicType( const std::string &type );
};


class ArrayTypeBase;

class Array : public DT {
public:
	Array( boost::python::tuple args, boost::python::dict keywordArgs );
};


class StructTypeBase;

class Struct : public DT {
public:
	Struct( boost::python::tuple args, boost::python::dict keywordArgs );
};


class ArgDeclBaseBase;
typedef boost::shared_ptr< ArgDeclBaseBase > ArgDeclBaseBaseSP;
typedef boost::weak_ptr< ArgDeclBaseBase > ArgDeclBaseBaseWP;

class ArgDeclBase {

	friend class ArgDeclBaseBase;

protected:
	ArgDeclBaseBaseSP _argDeclBaseBaseSP;

	ArgDeclBaseBaseWP getWP( void );

public:
	ArgDeclBase( SFC::ArgDeclBase argDeclBase, bool partOfContext = false );
	ArgDeclBase( ArgDeclBaseBase &argDeclBaseBase );
	ArgDeclBase( ArgDeclBaseBase *argDeclBaseBasePtr );

	ArgDeclBaseBase &getArgDeclBaseBase( void );

	bool isNull( void );

	std::string getName( void );
	std::string __str__( void );

	SFC::DT getSFCType( void );
	SFC::ArgDeclBase getSFCArgDeclBase( void );

	DT getType( void );
	void setType( DT dt );

	DT getSubArray( void );
	int getNoElements( void );

	DT getElementType( void );
	void setElementType( DT dt );
	boost::python::list getMembers( void );

	int getDimension( void );
	boost::python::list getDimensions( void );
	boost::python::list getAllDimensions( void );

	void setInitialValue( const std::string &initialValue );

	bool isBasicType( void );
	bool isArray( void );
	bool isStruct( void );

	std::string __add__( const std::string &rhs );
	std::string __radd__( const std::string &rhs );
};


class IOBase;
class PrimitiveBlock;

class IO : public ArgDeclBase {
public:
	IO( PrimitiveBlock &primitiveBlock, SFC::ArgDeclBase argDeclBase );
	IO( PrimitiveBlock &primitiveBlock, ArgDeclBase argDeclBase );
	IO( IOBase &ioBase );
	IO( IOBase *ioBasePtr );

	IOBase &getIOBase( void );
	const IOBase &getIOBase( void ) const;

	void setKeepStruct( void );
	bool getKeepStruct( void );

	void convertToArray( void );

	void alias( const std::string &name );
};


class Input : public IO {
public:
	Input( PrimitiveBlock &primitiveBlock, SFC::ArgDeclBase argDeclBase );
};


class Output : public IO {
public:
	Output( PrimitiveBlock &primitiveBlock, SFC::ArgDeclBase argDeclBase );
};


class ParameterBase;
typedef boost::shared_ptr< ParameterBase > ParameterBaseSP;

class Parameter {
protected:
	ParameterBaseSP _parameterBaseSP;

public:
	Parameter( PrimitiveBlock &primitiveBlock, SLSF::Parameter parameter );

	ParameterBase &getParameterBase( void );
	const ParameterBase &getParameterBase( void ) const;

	std::string getName( void ) const;
	std::string getValue( void ) const;
	std::string __str__( void ) const;

	ArgDeclBase placeInContext( void );
	ArgDeclBase placeInContextNoArg( void );
	ArgDeclBase placeInContext( DT dt );
	ArgDeclBase placeInContextOneArg( DT dt );

	bool operator==( std::string bpString ) const;
	bool operator!=( std::string bpString ) const;
};

class BlockBase;
typedef boost::shared_ptr< BlockBase > BlockBaseSP;
typedef boost::weak_ptr< BlockBase > BlockBaseWP;

class Block {
protected:
	BlockBaseSP _blockBaseSP;

	BlockBaseWP getWP( void );

public:
	Block( SLSF::Block block );
	Block( BlockBase &blockBase );
	Block( BlockBase *blockBasePtr );

	static int getUniqueNumber( void );

	Block getParentBlock( void );
	bool hasParentBlock( void );
	bool isNull( void );

	std::string getName( void );

	static std::string createMatlabVariable( const std::string &name );

	int __hash__( void );
	bool operator==( const Block &block );
	bool operator!=( const Block &block );
};


class PrimitiveBlockBase;
typedef boost::shared_ptr< PrimitiveBlockBase > PrimitiveBlockBaseSP;
typedef boost::weak_ptr< PrimitiveBlockBase > PrimitiveBlockBaseWP;

class PrimitiveBlockBaseProxy;
typedef boost::shared_ptr< PrimitiveBlockBaseProxy > PrimitiveBlockBaseProxySP;

typedef std::list< SFC::ArgDeclBase > ArgDeclBaseList;
typedef std::map< std::string, SLSF::Parameter > ParameterMap;

class PrimitiveBlock : public Block {

public:
	PrimitiveBlock(
	 SLSF::Block block, SFC::CompoundStatement context, ArgDeclBaseList &inputs, ArgDeclBaseList &outputs, ParameterMap &parameters
	);
	PrimitiveBlock( PrimitiveBlockBase &primitiveBlockBase );

	PrimitiveBlockBase &getPrimitiveBlockBase( void );
	const PrimitiveBlockBase &getPrimitiveBlockBase( void ) const;

	PrimitiveBlockBaseProxySP getProxySP( void );

	SFC::CompoundStatement getContext( void );
	std::string getContextArgName( void );

	void setZeroIndexing( void );
	void setOneIndexing( void );
	bool getZeroIndexingFlag( void );

	void setAcceptCSyntax( void );
	bool getAcceptCSyntaxFlag( void );

	void setKeepStructs( void );
	bool getKeepStructs( void );

	boost::python::list getInputs( void );
	boost::python::list getOutputs( void );
	boost::python::dict getParameters( void );

	ArgDeclBase createGlobalVariable( const std::string &name, const Parameter &value );

	ArgDeclBase createContextVariable( const std::string &name, DT dt );
	ArgDeclBase createContextVariable( const std::string &name, const std::string &typeName );
	ArgDeclBase createContextVariableDT( const std::string &name, DT dt ) {
		return createContextVariable( name, dt );
	}
	ArgDeclBase createContextVariableStr( const std::string &name, const std::string &typeName ) {
		return createContextVariable( name, typeName );
	}

	ArgDeclBase createVariable( const std::string &name, DT dt );
	ArgDeclBase createVariable( const std::string &name, const std::string &typeName );
	ArgDeclBase createVariableDT( const std::string &name, DT dt ) {
		return createVariable( name, dt );
	}
	ArgDeclBase createVariableStr( const std::string &name, const std::string &typeName ) {
		return createVariable( name, typeName );
	}

	void addStatement( const std::string &statement, bool isPreStatement );
	std::string getPreStatements( void );
	std::string getPostStatements( void );
};

#endif
