#ifndef _SFCSYMBOLTABLE_HPP
#define _SFCSYMBOLTABLE_HPP

#include <iostream>
#include <map>
#include <vector>
#include <set>

#include "SFC.h"
#include "SFCUdmEngine.hpp"

class SFCSymbolTable {
public:
	typedef std::vector< SFC::ArgDeclBase > ArgDeclBaseVector;
	typedef std::vector< ArgDeclBaseVector > ADBVVector;
	typedef std::map< std::string, ADBVVector > ArgDeclBaseMap;
	typedef std::set< std::string > ScopeSet;
	typedef std::vector< ScopeSet > ScopeSetVector;

private:
	ArgDeclBaseMap _argDeclBaseMap;
	ScopeSetVector _scopeSetVector;

public:
	SFCSymbolTable( void ) { pushScope(); }

	void push( const std::string &varName, SFC::ArgDeclBase argDeclBase );
	void pop( const std::string &varName );

	SFC::ArgDeclBase lookup( const std::string &varName );

	void pushScope( void ) {
		_scopeSetVector.push_back( ScopeSet() );
	}

	void popScope( void );

	void pushCleanScope( void );
	void popCleanScope( void );

	void clear( void ) {
		_argDeclBaseMap.clear();
		_scopeSetVector.clear();
		pushScope();
	}

	void printSymbols( void ) {

		std::cerr << "Current symbols in SFC symbol table are as follows:" << std::endl;
		for( ArgDeclBaseMap::iterator abmItr = _argDeclBaseMap.begin() ; abmItr != _argDeclBaseMap.end() ; ++abmItr ) {
			std::string name = abmItr->first;
			std::string typeName;
			SFC::ArgDeclBase argDeclBase = lookup( name );
			if ( argDeclBase == Udm::null ) {
				typeName = "NULL ArgDeclBase!";
			} else {
				SFC::DT dt = argDeclBase.dt();
				typeName = dt == Udm::null ? "NULL" : static_cast< std::string >( dt.name() );
			}
			std::cerr << "\t" << abmItr->first << " (" << typeName << " )" << std::endl;
		}
	}


};

SFCSymbolTable &getSFCSymbolTable( void );

#endif