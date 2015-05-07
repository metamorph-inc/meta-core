#include "BlockTemplateModule.hpp"
#include "BlockTemplate.hpp"

#include <string>
#include <iostream>

#define BOOST_PYTHON_STATIC_LIB 1
#define BOOST_PYTHON_NO_LIB
#include <Python.h>
#include <boost/python.hpp>
#include <boost/python/raw_function.hpp>
#include <boost/lexical_cast.hpp>

#include "SFC.h"
//#include "ESMoL.h"


BOOST_PYTHON_MODULE( BlockTemplate ) {

	boost::python::class_< DT >( "DT", boost::python::init< DT >() )
	 .add_property( "name", &DT::getName )
	 .def( "__str__", &DT::__str__ )
	 .def( "isNull", &DT::isNull )
	 .def( "isBasicType", &DT::isBasicType )
	 .def( "isArray", &DT::isArray )
	 .def( "isStruct", &DT::isStruct )
	 .def( "getTypeString", &DT::getTypeString )
	 .def( "getDimension", &DT::getDimension )
	 .def( "getSubArray", &DT::getSubArray )
	 .def( "getNoElements", &DT::getNoElements )
	 .def( "getElementType", &DT::getElementType )
	 .def( "setElementType", &DT::setElementType )
	 .def( "getMembers", &DT::getMembers )
	 .def( "getDimensions", &DT::getDimensions )
	 .def( "getAllDimensions", &DT::getAllDimensions )
	 .def( boost::python::self_ns::self == DT() )
	 .def( boost::python::self_ns::self != DT() )
	;

	boost::python::class_<  BasicType, boost::python::bases< DT >  >( "BasicType", boost::python::init< std::string >() )
	;

	boost::python::class_<  Array, boost::python::bases< DT >  >
	 bpArray( "Array", boost::python::init< boost::python::tuple, boost::python::dict >() );
	bpArray
	 .def(  "__tmp_init__", boost::python::raw_function( &raw_constructor )  )
	;
	{
		boost::python::object realBPArrayConstructor = boost::python::getattr( bpArray, "__init__");
		boost::python::object rawBPArrayConstructor = boost::python::getattr( bpArray, "__tmp_init__");
		boost::python::setattr( bpArray, "__init__", rawBPArrayConstructor );
		boost::python::delattr( bpArray, "__tmp_init__" );
		boost::python::setattr( bpArray, "__real_init__", realBPArrayConstructor );
	}

	boost::python::class_<  Struct, boost::python::bases< DT >  >
	 bpStruct( "Struct", boost::python::init< boost::python::tuple, boost::python::dict >() );
	bpStruct
	 .def(  "__tmp_init__", boost::python::raw_function( &raw_constructor )  )
	;
	{
		boost::python::object realBPStructConstructor = boost::python::getattr( bpStruct, "__init__");
		boost::python::object rawBPStructConstructor = boost::python::getattr( bpStruct, "__tmp_init__");
		boost::python::setattr( bpStruct, "__init__", rawBPStructConstructor );
		boost::python::delattr( bpStruct, "__tmp_init__" );
		boost::python::setattr( bpStruct, "__real_init__", realBPStructConstructor );
	}

	boost::python::class_< ArgDeclBase >( "ArgDeclBase", boost::python::init< ArgDeclBase >() )
	 .add_property( "name", &ArgDeclBase::getName )
	 .add_property( "type", &ArgDeclBase::getType, &ArgDeclBase::setType )
	 .def( "isNull", &ArgDeclBase::isNull )
	 .def( "getSubArray", &ArgDeclBase::getSubArray )
	 .def( "getNoElements", &ArgDeclBase::getNoElements )
	 .def( "getElementType", &ArgDeclBase::getElementType )
	 .def( "setElementType", &ArgDeclBase::setElementType )
	 .def( "getMembers", &ArgDeclBase::getMembers )
	 .def( "getDimension", &ArgDeclBase::getDimension )
	 .def( "getDimensions", &ArgDeclBase::getDimensions )
	 .def( "getAllDimensions", &ArgDeclBase::getAllDimensions )
	 .def( "setInitialValue", &ArgDeclBase::setInitialValue )
	 .def( "isBasicType", &ArgDeclBase::isBasicType )
	 .def( "isArray", &ArgDeclBase::isArray )
	 .def( "isStruct", &ArgDeclBase::isStruct )
	 .def( "__str__", &ArgDeclBase::__str__ )
	 .def( "__add__", &ArgDeclBase::__add__ )
	 .def( "__radd__", &ArgDeclBase::__radd__ )
	;

	boost::python::class_<  IO, boost::python::bases< ArgDeclBase >  >
	 ( "IO", boost::python::init< IO >() )
	 .def( "setKeepStruct", &IO::setKeepStruct )
	 .def( "getKeepStruct", &IO::getKeepStruct )
	 .def( "convertToArray", &IO::convertToArray )
	 .def( "alias", &IO::alias )
	;

	boost::python::class_<  Input, boost::python::bases< IO >  >
	 ( "Input", boost::python::init< Input >() )
	;

	boost::python::class_<  Output, boost::python::bases< IO >  >
	 ( "Output", boost::python::init< Output >() )
	;

	boost::python::class_< Parameter >( "Parameter", boost::python::init< Parameter >() )
	 .add_property( "name", &Parameter::getName )
	 .add_property( "value", &Parameter::getValue )
	 .def( "placeInContext", &Parameter::placeInContextNoArg )
	 .def( "placeInContext", &Parameter::placeInContextOneArg )
	 .def( "__str__", &Parameter::__str__ )
	 .def( boost::python::self_ns::self == std::string() )
	 .def( boost::python::self_ns::self != std::string() )
	;

	boost::python::class_< Block >( "Block", boost::python::no_init )
	 .add_property( "name", &Block::getName )
	 .add_property( "parent", &Block::getParentBlock )
	 .def( "hasParent", &Block::hasParentBlock )
	 .def( "isNull", &Block::isNull )
	 .def( "createMatlabVariable", &Block::createMatlabVariable )
	 .staticmethod( "createMatlabVariable" )
	 .def( "__hash__", &Block::__hash__ )
	 .def( boost::python::self_ns::self == boost::python::self_ns::self )
	 .def( boost::python::self_ns::self != boost::python::self_ns::self )
	;

	boost::python::class_<  PrimitiveBlock, boost::python::bases< Block >  >
	 ( "PrimitiveBlock", boost::python::init< PrimitiveBlock >() )
	 .add_property( "inputs", &PrimitiveBlock::getInputs )
	 .add_property( "outputs", &PrimitiveBlock::getOutputs )
	 .add_property( "parameters", &PrimitiveBlock::getParameters )
	 .def( "setZeroIndexing", &PrimitiveBlock::setZeroIndexing )
	 .def( "setOneIndexing", &PrimitiveBlock::setOneIndexing )
	 .def( "setAcceptCSyntax", &PrimitiveBlock::setAcceptCSyntax )
	 .def( "setKeepStructs", &PrimitiveBlock::setKeepStructs )
	 .def( "getKeepStructs", &PrimitiveBlock::getKeepStructs )
	 .def( "createGlobalVariable", &PrimitiveBlock::createGlobalVariable )
	 .def( "createContextVariable", &PrimitiveBlock::createContextVariableDT )
	 .def( "createContextVariable", &PrimitiveBlock::createContextVariableStr )
	 .def( "createVariable", &PrimitiveBlock::createVariableDT )
	 .def( "createVariable", &PrimitiveBlock::createVariableStr )
	;

}

void BlockTemplateInit( void ) { initBlockTemplate(); }
