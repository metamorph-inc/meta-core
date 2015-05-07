#ifndef _ISIS_COLONEXPRRETURN
#define _ISIS_COLONEXPRRETURN

#include "VarExprReturn.hpp"

class ColonExprReturn : public VarExprReturn {
public:
	typedef std::vector< ExprReturnSP > ExprReturnSPVector;
private:
	typedef std::vector < LitMatrixExprReturnSP > LitMatrixExprReturnSPVector;

	Exprs _dimExprs;

	ColonExprReturn(
	 SFC::CompoundStatement block,
	 Exprs dimExprs,
	 Exprs exprs,
	 ExprsProxyVector &exprsProxyVector,
	 SFC::DT dt
	) :
	 VarExprReturn( block, exprs, exprsProxyVector, dt ), _dimExprs( dimExprs ) { }

public:

	Exprs &getDimExprs( void ) { return _dimExprs; }

	static ExprReturnSP create( SFC::CompoundStatement block, ExprReturnSPVector &exprReturnSPVector );

	static const std::string getName( void ) { return "ColonExprReturn"; }
	virtual const std::string getClassName( void ) { return getName(); }
};

#endif