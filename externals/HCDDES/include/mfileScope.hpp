#ifndef _MFILESCOPE_HPP_
#define _MFILESCOPE_HPP_

/*
 * mfileScope.hpp
 *
 *  Created on: Sep 23, 2011
 *      Author: c2wt
 */
#include <string>
#include <map>
#include "ScopeSymbolTable.hpp"

extern "C" {
	struct ANTLR3_BASE_TREE_struct;
	typedef ANTLR3_BASE_TREE_struct *pANTLR3_BASE_TREE;
}

class MFileScope {
public:
	enum Replace { NO, YES, LHS, POSTOP };

private:
	static ScopeSymbolTable &getDefaultScopeSymbolTable( void ) {
		static ScopeSymbolTable scopeSymbolTable;
		return scopeSymbolTable;
	}

	ScopeSymbolTable *_scopeSymbolTablePtr;

	ScopeSymbolTable &getScopeSymbolTable( void ) {
		return *_scopeSymbolTablePtr;
	}

public:
	MFileScope( void ) : _scopeSymbolTablePtr( &getDefaultScopeSymbolTable() ) { }

	bool isValidMatLabInput( const std::string &matLabExpression );

	std::string scopeTrans( const std::string &mfile, ScopeSymbolTable &scopeSymbolTable );

	std::string statementScopeTrans( pANTLR3_BASE_TREE tree, const std::string &indent );
	std::string ifScopeTrans( pANTLR3_BASE_TREE tree, const std::string &indent );
	std::string makeIfScopeTrans( pANTLR3_BASE_TREE tree, bool first, const std::string &indent );
	std::string switchScopeTrans( pANTLR3_BASE_TREE tree, const std::string &indent );
	std::string caseScopeTrans( pANTLR3_BASE_TREE tree, const std::string &indent );
	std::string whileScopeTrans( pANTLR3_BASE_TREE tree, const std::string &indent );
	std::string forScopeTrans( pANTLR3_BASE_TREE tree, const std::string &indent );
	std::string tryScopeTrans( pANTLR3_BASE_TREE tree, const std::string &indent );
	std::string exprScopeTrans( pANTLR3_BASE_TREE tree );
	std::string colonScopeTrans( pANTLR3_BASE_TREE tree );
	std::string referenceScopeTrans( pANTLR3_BASE_TREE tree, Replace replace );
	std::string printArgs( pANTLR3_BASE_TREE tree );
	std::string identifierScopeTrans( pANTLR3_BASE_TREE tree, Replace replace );
	std::string matrixScopeTrans( pANTLR3_BASE_TREE tree );

	std::string getTreeText( pANTLR3_BASE_TREE tree );
	std::string getDeepTreeText( pANTLR3_BASE_TREE tree );
};

#endif // _MFILESCOPE_HPP_
