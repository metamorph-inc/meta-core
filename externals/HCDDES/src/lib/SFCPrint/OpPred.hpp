#ifndef _ISIS_OPPRED
#define _ISIS_OPPRED

#include "SFC.h"
#include "Uml.h"
#include <map>
#include <string>

class OpPred {

	typedef std::map< std::string, int > PrecedenceHash;

	struct PrecedenceTable {
		PrecedenceHash _precedenceHash;
		PrecedenceTable( void );
	};

	static PrecedenceHash &getPrecedenceHash( void ) {
		static PrecedenceTable precedenceTable;
		return precedenceTable._precedenceHash;
	}

public:

	static int getBinaryPrecedence( std::string op ) {
		PrecedenceHash::iterator prhItr = getPrecedenceHash().find(  std::string( "binary" ) + op  );
		return prhItr == getPrecedenceHash().end() ? 99 : prhItr->second;
	}

	static int getUnaryPrecedence( std::string op ) {
		PrecedenceHash::iterator prhItr = getPrecedenceHash().find(  std::string( "unary" ) + op  );
		return prhItr == getPrecedenceHash().end() ? 99 : prhItr->second;
	}

	static int getOperatorPrecedence( SFC::Exprs exprs, bool derefArg = true );
};

#endif
