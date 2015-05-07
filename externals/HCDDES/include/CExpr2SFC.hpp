#ifndef _ISIS_CEXPR2SFC
#define _ISIS_CEXPR2SFC

#include "SFC.h"

namespace CExpr2SFC {

	void ParseCExpr( SFC::Project &project );
//	void ParseCExpr( SFC::Program &program );

	int getBinaryPrecedence( std::string op );
	int getUnaryPrecedence( std::string op );
	int getOperatorPrecedence( SFC::Exprs &exprs, bool derefArg = true );

}
#endif