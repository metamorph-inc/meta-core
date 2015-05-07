#include "ScopeSymbolTable.hpp"
#include "RegexCommon.hpp"
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>

// SYMBOLS (E.G. MASK PARAMETERS) ARE GIVEN UNIQUE NAMES.
// WHEN YOU HAVE TWO VARIABLES OF THE SAME NAME AT DIFFERENT SCOPES, WHEN
// THE INNER SCOPE DISAPPEARS, YOU NEED TO RESET THE VARIABLE TO THE VALUE IT HAD
// AT THE OUTER SCOPE.
// THE WAY THIS IS ACCOMPLISHED HERE IS, IN MATLAB, THE VARIABLE IS MAPPED TO A UNIQUE
// NAME AT EACH SCOPE (I.E. BY APPENDING A NUMBER TO IT).  WHENEVER THE VARIABLE IS 
// ENCOUNTERED IN AN EXPRESSION, IT IS MAPPED TO THE UNIQUE NAME THAT IT HAS AT THE CURRENT
// SCOPE, AND THEN EVALUATED (IN MATLAB).
// WHEN THE CURRENT SCOPE DISAPPEARS, THE UNIQUE NAME FOR THIS INNER SCOPE IS ERASED, AND THE
// UNIQUE NAME FOR THE OUTER SCOPE TAKES OVER.

std::string ScopeSymbolTable::addSymbol( const std::string &varName ) {

	if ( _scopeStack.empty() ) {
		throwException( "ERROR:  SymbolTable:  addSymbol METHOD:  NO SYMBOL-SET ON SCOPE STACK" );
	}
	SymbolSet &symbolSet = _scopeStack.back();
	if ( symbolSet.find( varName ) != symbolSet.end() ) {
		throwException( "ERROR:  SymbolTable:  addSymbol METHOD:  SYMBOL ALREADY ON SCOPE STACK" );
	}
	symbolSet.insert( varName );

	SymbolStack &symbolStack = _symbolMap[ varName ];

	std::string uniqueVarName = (*_uniqueVarNameFunctorSP)( varName );
	symbolStack.push_back( uniqueVarName );

	return uniqueVarName;
}

void ScopeSymbolTable::deleteScope( void ) {

	if ( _scopeStack.empty() ) {
		throwException( "ERROR:  SymbolTable:  deleteScope METHOD:  NO SYMBOL-SET ON SCOPE STACK" );
	}

	SymbolSet &symbolSet = _scopeStack.back();

	for( SymbolSet::const_iterator sbsCit = symbolSet.begin() ; sbsCit != symbolSet.end() ; ++sbsCit ) {

		std::string symbol = *sbsCit;
		SymbolMap::iterator sbmItr = _symbolMap.find( symbol );
		if ( sbmItr == _symbolMap.end() ) {
			throwException( "ERROR:  SymbolTable:  deleteScope METHOD:  NO SymbolStack FOR SYMBOL \"" + symbol + "\"" );
		}

		SymbolStack &symbolStack = sbmItr->second;
		(*_destroyUniqueVarFunctorSP)( symbolStack.back() );
		symbolStack.pop_back();
	}

	_scopeStack.pop_back();
}

std::string ScopeSymbolTable::performSimpleSubstitution( std::string inputString ) {

	boost::regex identifierRegex(
	 "^(?:" + RegexCommon::getSkipStringCommentRegexString() + "|/(?=[^/*])|[^%'/A-Za-z_])*+" + "(" + RegexCommon::getDottedIdentifierRegexString() + ")",
	 boost::regex_constants::perl
	);

	boost::match_results< std::string::const_iterator > results;

	std::string outputString;

	const std::string &constInputString = inputString;

	while(  boost::regex_search( constInputString.begin(), constInputString.end(), results, identifierRegex )  ) {

		std::string dottedIdentifier( results[1].first, results[1].second );

		StringVector stringVector;
		boost::split(  stringVector, dottedIdentifier, LimitedSplitPredicate( ".", 1 )  );

		std::string newIdentifier = getSymbol( stringVector[0] );

		outputString += std::string( constInputString.begin(), results[1].first ) + newIdentifier;
		if ( stringVector.size() == 2 ) outputString += "." + stringVector[1];

		inputString = results.suffix();
	}

	outputString += inputString;

	return outputString;
}

