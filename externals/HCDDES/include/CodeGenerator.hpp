#ifndef _CODEGENERATOR_HPP_
#define _CODEGENERATOR_HPP_

#include <string>
#include <map>

#define BOOST_PYTHON_STATIC_LIB 1
#define BOOST_PYTHON_NO_LIB
#include <Python.h>
#include <boost/python.hpp>
#include <boost/shared_ptr.hpp>

#include "BlockTemplateModule.hpp"
#include "SLSF.h"

class CodeGenerator {
public:
	typedef std::map< std::string, boost::python::object > PrimitiveNameModuleMap;

private:
	typedef boost::shared_ptr< PrimitiveBlock > PrimitiveBlockSP;

	boost::python::object _blockTemplateModule;
	boost::python::object _blockTemplateNameSpace;

	bool _error;

	PrimitiveNameModuleMap _primitiveNameModuleMap;
	PrimitiveBlockSP _primitiveBlockSP;

	CodeGenerator( void );

	boost::python::object getPrimitiveModuleNamespace( const std::string &primitiveName );

public:
	typedef std::list< SFC::ArgDeclBase > ArgDeclBaseList;
	typedef std::map< std::string, SLSF::Parameter > ParameterMap;

	class BlockSetup {
	private:
		SLSF::Block _block;
		SFC::CompoundStatement _context;

		ArgDeclBaseList _inputList;
		ArgDeclBaseList _outputList;
		ParameterMap _parameterMap;

	public:
		BlockSetup( SLSF::Block block ) : _block( block ) { }

		void setContext( SFC::CompoundStatement context ) { _context = context; }
		void addInput( SFC::ArgDeclBase input ) { _inputList.push_back( input ); }
		void addOutput( SFC::ArgDeclBase output ) { _outputList.push_back( output ); }
		void addParameter( SLSF::Parameter parameter ) {
			_parameterMap.insert(   std::make_pair(  static_cast< std::string >( parameter.name() ), parameter  )   );
		}

		SLSF::Block getBlock( void ) { return _block; }
		std::string getPrimitiveType( void ) { return _block.BlockType(); }
		SFC::CompoundStatement getContext( void ) { return _context; }
		ArgDeclBaseList &getInputs( void ) { return _inputList; }
		ArgDeclBaseList &getOutputs( void ) { return _outputList; }
		ParameterMap &getParameters( void ) { return _parameterMap; }
	};

	typedef std::map< SLSF::Block, BlockSetup > BlockSetupMap;

	BlockSetupMap _blockSetupMap;

	BlockSetup &getBlockSetup( SLSF::Block block ) {
		BlockSetupMap::iterator bsmItr = _blockSetupMap.find( block );
		if ( bsmItr == _blockSetupMap.end() ) {
			_blockSetupMap.insert(   std::make_pair(  block, BlockSetup( block )  )   );
			bsmItr = _blockSetupMap.find( block );
		}
		return bsmItr->second;
	}

	void removeBlockSetup( SLSF::Block block ) { _blockSetupMap.erase( block ); }

	boost::python::object getPrimitiveNamespace( const std::string &primitiveName );

	std::string normalizePrimitiveName( const std::string &primitiveName ) {
		std::string outputPrimitiveName;
		for( std::string::const_iterator strCit = primitiveName.begin() ; strCit != primitiveName.end() ; ++strCit ) {
			int ch = *strCit;
			if (  isalnum( ch ) || ( ch == '_' )  ) outputPrimitiveName += ch;
		}
		return outputPrimitiveName;
	}

	std::string generateMatLabCode(
	 const std::string &primitiveName, boost::python::list inputs, boost::python::list outputs, boost::python::dict parameters
	);

	void generateSFC( SLSF::Block block );

	bool getError( void ) { return _error; }

	static CodeGenerator &getSingleton( void ) {
		static CodeGenerator codeGenerator;
		return codeGenerator;
	}
};

#endif