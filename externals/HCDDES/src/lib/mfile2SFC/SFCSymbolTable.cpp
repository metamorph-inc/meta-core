#include "SFCSymbolTable.hpp"

void SFCSymbolTable::push( const std::string &varName, SFC::ArgDeclBase argDeclBase ) {
	ArgDeclBaseMap::iterator abmItr = _argDeclBaseMap.find( varName );
	if ( abmItr == _argDeclBaseMap.end() ) {
		abmItr = _argDeclBaseMap.insert(  std::make_pair( varName, ADBVVector() )  ).first;
	}
	ADBVVector &adbvVector = abmItr->second;
	if ( adbvVector.empty() ) {
		adbvVector.push_back( ArgDeclBaseVector() );
	}

	ArgDeclBaseVector &argDeclBaseVector = adbvVector.back();

	if ( argDeclBaseVector.empty() || argDeclBaseVector.back() != argDeclBase ) {
		argDeclBaseVector.push_back( argDeclBase );

		ScopeSet &scopeSet = _scopeSetVector.back();
		ScopeSet::iterator scsItr = scopeSet.find( varName );
		if ( scsItr != scopeSet.end() ) {
			std::cerr << "WARNING:  Symbol Table (push):  variable of name \"" << varName <<
			 "\" already exists in current scope." << std::endl;
		} else {
			scopeSet.insert( varName );
		}
	}
}

void SFCSymbolTable::pop( const std::string &varName ) {
	ArgDeclBaseMap::iterator abmItr = _argDeclBaseMap.find( varName );
	if ( abmItr == _argDeclBaseMap.end() ) {
		std::cerr << "WARNING:  Symbol Table (pop):  no variable of name \"" << varName <<
		 "\" currently exists (in any scope)." << std::endl;
		return;
	}

	ADBVVector &adbvVector = abmItr->second;
	if ( adbvVector.empty() ) {
		std::cerr << "WARNING:  Symbol Table (pop):  variable name \"" << varName << "\" does not reference anything." << std::endl;
		return;
	}

	ArgDeclBaseVector &argDeclBaseVector = adbvVector.back();

	if ( argDeclBaseVector.empty() ) {
		std::cerr << "WARNING:  Symbol Table (pop):  variable name \"" << varName << "\" references nothing." << std::endl;
		return;
	}

	argDeclBaseVector.pop_back();
	if ( argDeclBaseVector.empty() && adbvVector.size() == 1 ) {
		_argDeclBaseMap.erase( abmItr );
	}
}

SFC::ArgDeclBase SFCSymbolTable::lookup( const std::string &varName ) {
	ArgDeclBaseMap::iterator abmItr = _argDeclBaseMap.find( varName );
	if ( abmItr == _argDeclBaseMap.end() ) return SFC::ArgDeclBase();

	ADBVVector &adbvVector = abmItr->second;
	if ( adbvVector.empty() ) return SFC::ArgDeclBase();

	ArgDeclBaseVector &argDeclBaseVector = adbvVector.back();
	if ( argDeclBaseVector.empty() ) return SFC::ArgDeclBase();

	return argDeclBaseVector.back();
}

void SFCSymbolTable::popScope( void ) {
	{
		ScopeSet &scopeSet = _scopeSetVector.back();
		for( ScopeSet::iterator scsItr = scopeSet.begin() ; scsItr != scopeSet.end() ; ++scsItr ) pop( *scsItr );
	}
	_scopeSetVector.pop_back();
	if ( _scopeSetVector.empty() ) pushScope();
}

void SFCSymbolTable::pushCleanScope( void ) {
	for( ArgDeclBaseMap::iterator admItr = _argDeclBaseMap.begin() ; admItr != _argDeclBaseMap.end() ; ++admItr ) {
		ADBVVector &adbvVector = admItr->second;
		adbvVector.push_back( ArgDeclBaseVector() );
	}
	pushScope();
}

void SFCSymbolTable::popCleanScope( void ) {

	popScope();

	ArgDeclBaseMap::iterator admItr = _argDeclBaseMap.begin();
	while( admItr != _argDeclBaseMap.end() ) {
		ADBVVector &adbvVector = admItr->second;
		ArgDeclBaseMap::iterator curADMItr = admItr;
		++admItr;

		if ( adbvVector.empty() ) {
			_argDeclBaseMap.erase( curADMItr );
			continue;
		}

		adbvVector.pop_back();
		if ( adbvVector.empty() ) {
			_argDeclBaseMap.erase( curADMItr );
			continue;
		}
	}
}

SFCSymbolTable &getSFCSymbolTable( void ) {
	static SFCSymbolTable sfcSymbolTable;
	return sfcSymbolTable;
}

