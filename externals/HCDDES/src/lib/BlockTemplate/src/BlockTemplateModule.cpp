#define BOOST_PYTHON_STATIC_LIB 1
#define BOOST_PYTHON_NO_LIB
#include <Python.h>
#include <boost/lexical_cast.hpp>
#include <boost/python.hpp>

#include "BlockTemplateModule.hpp"
#include "BlockTemplateModuleBase.hpp"
#include "CodeGenerator.hpp"


boost::python::object raw_constructor( boost::python::tuple args, boost::python::dict keywordArgs ) {
	boost::python::tuple rest(  args.slice( 1,boost::python::_ )  );
	return args[0].attr( "__real_init__" )( rest, keywordArgs );
} 


DT::DT( void ) : _dtBaseSP( new DTBase() ) { _dtBaseSP->setWP( _dtBaseSP ); }
DT::DT( const std::string &typeName ) : _dtBaseSP(  new DTBase( typeName )  ) { _dtBaseSP->setWP( _dtBaseSP ); }
DT::DT( SFC::DT dt ) : _dtBaseSP(  new DTBase( dt )  ) { _dtBaseSP->setWP( _dtBaseSP ); }
DT::DT( DTBase &dtBase ) { _dtBaseSP = DTBaseSP( dtBase.getWP() ); }
DT::DT( DTBase *dtBasePtr ) : _dtBaseSP( dtBasePtr ) { _dtBaseSP->setWP( _dtBaseSP ); }

DTBase &DT::getDTBase( void ) { return *_dtBaseSP; }
const DTBase &DT::getDTBase ( void ) const { return *_dtBaseSP; }

std::string DT::getName( void ) const { return _dtBaseSP->getName(); }
std::string DT::__str__( void ) const { return _dtBaseSP->__str__(); }

bool DT::isNull( void ) const { return _dtBaseSP->isNull(); }
bool DT::isBasicType( void ) const { return _dtBaseSP->isBasicType(); }
bool DT::isArray( void ) const { return _dtBaseSP->isArray(); }
bool DT::isStruct( void ) const { return _dtBaseSP->isStruct(); }
std::string DT::getTypeString( void ) const { return _dtBaseSP->getTypeString(); }

int DT::getDimension( void ) const { return _dtBaseSP->getDimension(); }
DT DT::getSubArray( void ) { return _dtBaseSP->getSubArray(); }
int DT::getNoElements( void ) const { return _dtBaseSP->getNoElements(); }

DT DT::getElementType( void ) const { return _dtBaseSP->getElementType(); }
void DT::setElementType( DT dt ) { _dtBaseSP->setElementType( dt ); }
boost::python::list DT::getMembers( void ) const { return _dtBaseSP->getMembers(); }

boost::python::list DT::getDimensions( void ) const { return _dtBaseSP->getDimensions(); }
boost::python::list DT::getAllDimensions( void ) const { return _dtBaseSP->getAllDimensions(); }

SFC::DT DT::getSFCType( void ) const { return _dtBaseSP->getSFCType(); }

bool DT::operator==( const DT &rhs ) { return *_dtBaseSP == *rhs._dtBaseSP; }
bool DT::operator!=( const DT &rhs ) { return *_dtBaseSP != *rhs._dtBaseSP; }


BasicType::BasicType( const std::string &type ) : DT(  new BasicTypeBase( type )  ) { }

Array::Array( boost::python::tuple args, boost::python::dict keywordArgs ) :
 DT(  new ArrayBase( args, keywordArgs )  ) { }

Struct::Struct( boost::python::tuple positionalArgs, boost::python::dict keywordArgs ) :
 DT(  new StructBase( positionalArgs, keywordArgs )  ) { }

ArgDeclBaseBaseWP ArgDeclBase::getWP( void ) {
	return _argDeclBaseBaseSP;
}

ArgDeclBase::ArgDeclBase( SFC::ArgDeclBase sfcArgDeclBase, bool partOfContext ) :
 _argDeclBaseBaseSP(  new ArgDeclBaseBase( sfcArgDeclBase, partOfContext )  ) {
	_argDeclBaseBaseSP->setWP( _argDeclBaseBaseSP );
}

ArgDeclBase::ArgDeclBase( ArgDeclBaseBase &argDeclBaseBase ) : _argDeclBaseBaseSP( argDeclBaseBase.getWP() ) { }

ArgDeclBase::ArgDeclBase( ArgDeclBaseBase *argDeclBaseBasePtr ) : _argDeclBaseBaseSP( argDeclBaseBasePtr ) {
	_argDeclBaseBaseSP->setWP( _argDeclBaseBaseSP );
}

ArgDeclBaseBase &ArgDeclBase::getArgDeclBaseBase( void ) { return *_argDeclBaseBaseSP; }

bool ArgDeclBase::isNull( void ) { return _argDeclBaseBaseSP->isNull(); }

std::string ArgDeclBase::getName( void ) { return _argDeclBaseBaseSP->getName(); }
std::string ArgDeclBase::__str__( void ) { return _argDeclBaseBaseSP->__str__(); }

SFC::DT ArgDeclBase::getSFCType( void ) { return _argDeclBaseBaseSP->getSFCType(); }
SFC::ArgDeclBase ArgDeclBase::getSFCArgDeclBase( void ) { return _argDeclBaseBaseSP->getSFCArgDeclBase(); }

DT ArgDeclBase::getType( void ) { return DT( _argDeclBaseBaseSP->getType() ); }

void ArgDeclBase::setType( DT dt ) { _argDeclBaseBaseSP->setType( dt ); }

DT ArgDeclBase::getSubArray( void ) { return _argDeclBaseBaseSP->getSubArray(); }

int ArgDeclBase::getNoElements( void ) { return _argDeclBaseBaseSP->getNoElements(); }

DT ArgDeclBase::getElementType( void ) { return _argDeclBaseBaseSP->getElementType(); }
void ArgDeclBase::setElementType( DT dt ) { return _argDeclBaseBaseSP->setElementType( dt ); }
boost::python::list ArgDeclBase::getMembers( void ) { return _argDeclBaseBaseSP->getMembers(); }

int ArgDeclBase::getDimension( void ) { return _argDeclBaseBaseSP->getDimension(); }
boost::python::list ArgDeclBase::getDimensions( void ) { return _argDeclBaseBaseSP->getDimensions(); }
boost::python::list ArgDeclBase::getAllDimensions( void ) { return _argDeclBaseBaseSP->getAllDimensions(); }

void ArgDeclBase::setInitialValue( const std::string &initialValue ) {
	_argDeclBaseBaseSP->setInitialValue( initialValue );
}

bool ArgDeclBase::isBasicType( void ) { return _argDeclBaseBaseSP->isBasicType(); }
bool ArgDeclBase::isArray( void ) { return _argDeclBaseBaseSP->isArray(); }
bool ArgDeclBase::isStruct( void ) { return _argDeclBaseBaseSP->isStruct(); }

std::string ArgDeclBase::__add__( const std::string &rhs ) { return _argDeclBaseBaseSP->__add__( rhs ); }
std::string ArgDeclBase::__radd__( const std::string &rhs ) { return _argDeclBaseBaseSP->__radd__( rhs ); }

/******/
/* IO */
/******/
IO::IO( PrimitiveBlock &primitiveBlock, SFC::ArgDeclBase sfcArgDeclBase ) :
 ArgDeclBase(  new IOBase( primitiveBlock, sfcArgDeclBase )  ) { }

IO::IO( PrimitiveBlock &primitiveBlock, ArgDeclBase argDeclBase ) :
 ArgDeclBase(  new IOBase( primitiveBlock, argDeclBase.getSFCArgDeclBase() )  ) { }

IO::IO( IOBase &ioBase ) : ArgDeclBase( ioBase ) { } 

IO::IO( IOBase *ioBasePtr ) : ArgDeclBase( ioBasePtr ) { }

IOBase &IO::getIOBase( void ) {
	return *boost::static_pointer_cast< IOBase >( _argDeclBaseBaseSP );
}

const IOBase &IO::getIOBase( void ) const {
	return *boost::static_pointer_cast< IOBase >( _argDeclBaseBaseSP );
}

void IO::setKeepStruct( void ) { getIOBase().setKeepStruct(); }
bool IO::getKeepStruct( void ) { return getIOBase().getKeepStruct(); }

void IO::convertToArray( void ) { getIOBase().convertToArray(); }

void IO::alias( const std::string &name ) { boost::static_pointer_cast< IOBase >( _argDeclBaseBaseSP )->alias( name ); }


Input::Input( PrimitiveBlock &primitiveBlock, SFC::ArgDeclBase argDeclBase ) :
 IO(  new InputBase( primitiveBlock, argDeclBase )  ) { }


Output::Output( PrimitiveBlock &primitiveBlock, SFC::ArgDeclBase argDeclBase ) :
 IO(  new OutputBase( primitiveBlock, argDeclBase )  ) { }


Parameter::Parameter( PrimitiveBlock &primitiveBlock, SLSF::Parameter parameter ) :
 _parameterBaseSP(  new ParameterBase( primitiveBlock, parameter )  ) { }

ParameterBase &Parameter::getParameterBase( void ) { return *_parameterBaseSP; }
const ParameterBase &Parameter::getParameterBase( void ) const { return *_parameterBaseSP; }

std::string Parameter::getName( void ) const { return _parameterBaseSP->getName(); }
std::string Parameter::getValue( void ) const { return _parameterBaseSP->getValue(); }
std::string Parameter::__str__( void ) const { return _parameterBaseSP->getValue(); }

ArgDeclBase Parameter::placeInContext( void ) {
	return _parameterBaseSP->placeInContext();
}
ArgDeclBase Parameter::placeInContextNoArg( void ) {
	return placeInContext();
}
ArgDeclBase Parameter::placeInContext( DT dt ) {
	return _parameterBaseSP->placeInContext( dt );
}
ArgDeclBase Parameter::placeInContextOneArg( DT dt ) {
	return placeInContext( dt );
}

bool Parameter::operator==( std::string bpString ) const { return *_parameterBaseSP == bpString; }
bool Parameter::operator!=( std::string bpString ) const { return *_parameterBaseSP == bpString; }


BlockBaseWP Block::getWP( void ) { return _blockBaseSP; }

Block::Block( SLSF::Block block ) : _blockBaseSP(  new BlockBase( block )  ) {
	_blockBaseSP->setWP( _blockBaseSP );
}
//Block::Block( BlockBase &blockBase ) : _blockBaseSP( blockBase.getWP() ) { }
Block::Block( BlockBase &blockBase ) {
	_blockBaseSP = BlockBaseSP( blockBase.getWP() );
}
Block::Block( BlockBase *blockBasePtr ) : _blockBaseSP( blockBasePtr ) {
	_blockBaseSP->setWP( _blockBaseSP );
}

int Block::getUniqueNumber( void ) { return BlockBase::getUniqueNumber(); }

Block Block::getParentBlock( void ) { return _blockBaseSP->getParentBlock(); }
bool Block::hasParentBlock( void ) { return _blockBaseSP->hasParentBlock(); }
bool Block::isNull( void ) { return _blockBaseSP->isNull(); }

std::string Block::getName( void ) { return _blockBaseSP->getName(); }

std::string Block::createMatlabVariable( const std::string &name ) {
	return BlockBase::createMatlabVariable( name );
}

int Block::__hash__( void ) { return _blockBaseSP->__hash__(); }
bool Block::operator==( const Block &rhs ) { return *_blockBaseSP == *rhs._blockBaseSP; }
bool Block::operator!=( const Block &rhs ) { return *_blockBaseSP != *rhs._blockBaseSP; }

PrimitiveBlock::PrimitiveBlock(
 SLSF::Block block, SFC::CompoundStatement context, ArgDeclBaseList &inputs, ArgDeclBaseList &outputs, ParameterMap &parameters
) :
 Block(  new PrimitiveBlockBase( block, context, inputs, outputs, parameters )  ) {
	getPrimitiveBlockBase().initialize();
}

PrimitiveBlock::PrimitiveBlock( PrimitiveBlockBase &primitiveBlockBase ) :
 Block( primitiveBlockBase ) { }

PrimitiveBlockBase &PrimitiveBlock::getPrimitiveBlockBase( void ) {
	return *boost::static_pointer_cast< PrimitiveBlockBase >( _blockBaseSP );
}

const PrimitiveBlockBase &PrimitiveBlock::getPrimitiveBlockBase( void ) const {
	return *boost::static_pointer_cast< PrimitiveBlockBase >( _blockBaseSP );
}

PrimitiveBlockBaseProxySP PrimitiveBlock::getProxySP( void ) {
	return getPrimitiveBlockBase().getProxySP();
}

SFC::CompoundStatement PrimitiveBlock::getContext( void ) {
	return getPrimitiveBlockBase().getContext();
}
std::string PrimitiveBlock::getContextArgName( void ) {
	return getPrimitiveBlockBase().getContextArgName();
}


void PrimitiveBlock::setZeroIndexing( void ) {
	getPrimitiveBlockBase().setZeroIndexing();
}
void PrimitiveBlock::setOneIndexing( void ) {
	getPrimitiveBlockBase().setOneIndexing();
}

bool PrimitiveBlock::getZeroIndexingFlag( void ) {
	return getPrimitiveBlockBase().getZeroIndexingFlag();
}

void PrimitiveBlock::setAcceptCSyntax( void ) {
	getPrimitiveBlockBase().setAcceptCSyntax();
}
bool PrimitiveBlock::getAcceptCSyntaxFlag( void ) {
	return getPrimitiveBlockBase().getAcceptCSyntaxFlag();
}

void PrimitiveBlock::setKeepStructs( void ) {
	getPrimitiveBlockBase().setKeepStructs();
}
bool PrimitiveBlock::getKeepStructs( void ) {
	return getPrimitiveBlockBase().getKeepStructs();
}

boost::python::list PrimitiveBlock::getInputs( void ) {
	return getPrimitiveBlockBase().getInputs();
}
boost::python::list PrimitiveBlock::getOutputs( void ) {
	return getPrimitiveBlockBase().getOutputs();
}
boost::python::dict PrimitiveBlock::getParameters( void ) {
	return getPrimitiveBlockBase().getParameters();
}

ArgDeclBase PrimitiveBlock::createGlobalVariable( const std::string &name, const Parameter &initialValue ) {
	return getPrimitiveBlockBase().createGlobalVariable( name, initialValue.getParameterBase() );
}

ArgDeclBase PrimitiveBlock::createContextVariable( const std::string &name, DT dt ) {
	return getPrimitiveBlockBase().createContextVariable( name, dt.getDTBase() );
}

ArgDeclBase PrimitiveBlock::createContextVariable( const std::string &name, const std::string &typeName ) {
	return getPrimitiveBlockBase().createContextVariable( name, typeName );
}

ArgDeclBase PrimitiveBlock::createVariable( const std::string &name, DT dt ) {
	return getPrimitiveBlockBase().createVariable( name, dt.getDTBase() );
}

ArgDeclBase PrimitiveBlock::createVariable( const std::string &name, const std::string &typeName ) {
	return getPrimitiveBlockBase().createVariable( name, typeName );
}

void PrimitiveBlock::addStatement( const std::string &statement, bool isPreStatement ) {
	getPrimitiveBlockBase().addStatement( statement, isPreStatement );
}

std::string PrimitiveBlock::getPreStatements( void ) {
	return getPrimitiveBlockBase().getPreStatements();
}

std::string PrimitiveBlock::getPostStatements( void ) {
	return getPrimitiveBlockBase().getPostStatements();
}
