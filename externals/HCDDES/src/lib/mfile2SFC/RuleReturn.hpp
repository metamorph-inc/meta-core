#ifndef _ISIS_RULERETURN
#define _ISIS_RULERETURN

#define _USE_MATH_DEFINES
#include <math.h>
#include <limits.h>

#include <string>
#include <vector>
#include <set>
#include <list>
#include <boost/shared_ptr.hpp>

#include "SFCSymbolTable.hpp"

#include "SFC.h"
#include "UdmStatementBuilder.hpp"
#include "SFCUdmEngine.hpp"

#define COMBINE_VISITOR_DECLARATION \
	virtual ExprReturnSP combineVisitor(\
	 const std::string &op, VarExprReturn &varExprReturn, bool func = false\
	);\
	virtual ExprReturnSP combineVisitor(\
	 const std::string &op, ReferenceReturn &referenceReturn, bool func = false\
	);\
	virtual ExprReturnSP combineVisitor(\
	 const std::string &op, LitMatrixExprReturn &litMatrixExprReturn, bool func = false\
	);\
	virtual ExprReturnSP combineVisitor(\
	 const std::string &op, MatrixExprReturn &matrixExprReturn, bool func = false\
	);\
	virtual ExprReturnSP combine(\
	 const std::string &op, ExprReturnSP exprReturnSP, bool func = false\
	);\
	virtual ExprReturnSP combine(\
	 const std::string &op, ExprReturn &exprReturn, bool func = false\
	);\
	virtual ExprReturnSP combine(\
	 const std::string &op, VarExprReturnSP varExprReturnSP, bool func = false\
	);\
	virtual ExprReturnSP combine(\
	 const std::string &op, LitMatrixExprReturnSP litMatrixExprReturnSP, bool func = false\
	);\
	virtual ExprReturnSP combine(\
	 const std::string &op, VarMatrixExprReturnSP varMatrixExprReturnSP, bool func = false\
	);

#define COMBINE_VISITOR_DEFINITION( CLASS ) \
	ExprReturnSP CLASS::combineVisitor(\
	 const std::string &op, VarExprReturn &varExprReturn, bool func\
	) {\
		return varExprReturn.combine( op, *this, func );\
	}\
	ExprReturnSP CLASS::combineVisitor(\
	 const std::string &op, ReferenceReturn &referenceReturn, bool func\
	) {\
		return referenceReturn.combine( op, *this, func );\
	}\
	ExprReturnSP CLASS::combineVisitor(\
	 const std::string &op, LitMatrixExprReturn &litMatrixExprReturn, bool func\
	) {\
		return litMatrixExprReturn.combine( op, *this, func );\
	}\
	ExprReturnSP CLASS::combineVisitor(\
	 const std::string &op, MatrixExprReturn &matrixExprReturn, bool func\
	) {\
		return matrixExprReturn.combine( op, *this, func );\
	}\
	ExprReturnSP CLASS::combine(\
	 const std::string &op, ExprReturnSP exprReturnSP, bool func\
	) {\
		return exprReturnSP->combineVisitor( op, *this, func );\
	}\
	ExprReturnSP CLASS::combine(\
	 const std::string &op, ExprReturn &exprReturn, bool func\
	) {\
		return exprReturn.combineVisitor( op, *this, func );\
	}\
	ExprReturnSP CLASS::combine(\
	 const std::string &op, VarExprReturnSP varExprReturnSP, bool func\
	) {\
		return combine( op, *varExprReturnSP, func );\
	}\
	ExprReturnSP CLASS::combine(\
	 const std::string &op, LitMatrixExprReturnSP litMatrixExprReturnSP, bool func\
	) {\
		return combine( op, *litMatrixExprReturnSP, func );\
	}\
	ExprReturnSP CLASS::combine(\
	 const std::string &op, VarMatrixExprReturnSP varMatrixExprReturnSP, bool func\
	) {\
		return combine( op, *varMatrixExprReturnSP, func );\
	}

#define MULTIPLY_VISITOR_DECLARATION \
	virtual ExprReturnSP multiplyVisitor( VarExprReturn &varExprReturn );\
	virtual ExprReturnSP multiplyVisitor( LitMatrixExprReturn &litMatrixExprReturn );\
	virtual ExprReturnSP multiplyVisitor( MatrixExprReturn &matrixExprReturn );\
	virtual ExprReturnSP multiply( ExprReturnSP exprReturnSP );\
	ExprReturnSP multiply( VarExprReturnSP varExprReturnSP );\
	ExprReturnSP multiply( LitMatrixExprReturnSP litMatrixExprReturnSP );\
	ExprReturnSP multiply( VarMatrixExprReturnSP varMatrixExprReturnSP );


#define MULTIPLY_VISITOR_DEFINITION( CLASS ) \
	ExprReturnSP CLASS::multiplyVisitor( VarExprReturn &varExprReturn ) {\
		return varExprReturn.multiply( *this );\
	}\
	ExprReturnSP CLASS::multiplyVisitor( LitMatrixExprReturn &litMatrixExprReturn ) {\
		return litMatrixExprReturn.multiply( *this );\
	}\
	ExprReturnSP CLASS::multiplyVisitor( MatrixExprReturn &matrixExprReturn ) {\
		return matrixExprReturn.multiply( *this );\
	}\
	ExprReturnSP CLASS::multiply( ExprReturnSP exprReturnSP ) {\
		return exprReturnSP->multiplyVisitor( *this );\
	}\
	ExprReturnSP CLASS::multiply( VarExprReturnSP varExprReturnSP ) {\
		return multiply( *varExprReturnSP );\
	}\
	ExprReturnSP CLASS::multiply( LitMatrixExprReturnSP litMatrixExprReturnSP ) {\
		return multiply( *litMatrixExprReturnSP );\
	}\
	ExprReturnSP CLASS::multiply( VarMatrixExprReturnSP varMatrixExprReturnSP ) {\
		return multiply( *varMatrixExprReturnSP );\
	}

#define DIVIDE_VISITOR_DECLARATION \
	virtual ExprReturnSP divideVisitor( LitMatrixExprReturn &litMatrixExprReturn );\
	virtual ExprReturnSP divideVisitor( ExprReturn &exprReturn );\
	virtual ExprReturnSP divide( ExprReturnSP exprReturnSP );\
	ExprReturnSP divide( LitMatrixExprReturnSP litMatrixExprReturnSP );

#define DIVIDE_VISITOR_DEFINITION( CLASS ) \
	ExprReturnSP CLASS::divideVisitor( LitMatrixExprReturn &litMatrixExprReturn ) {\
		return litMatrixExprReturn.divide( *this );\
	}\
	ExprReturnSP CLASS::divideVisitor( ExprReturn &exprReturn ) {\
		return exprReturn.divide( *this );\
	}\
	ExprReturnSP CLASS::divide( ExprReturnSP exprReturnSP ) {\
		return exprReturnSP->divideVisitor( *this );\
	}\
	ExprReturnSP CLASS::divide( LitMatrixExprReturnSP litMatrixExprReturnSP ) {\
		return divide( *litMatrixExprReturnSP );\
	}

#define APPEND_VISITOR_PIVOT_DECLARATION \
	virtual MatrixExprReturnSP appendColumnsVisitor( LitMatrixExprReturn &litMatrixExprReturn );\
	virtual MatrixExprReturnSP appendColumnsVisitor( VarMatrixExprReturn &varMatrixExprReturn );\
	virtual MatrixExprReturnSP appendRowsVisitor( LitMatrixExprReturn &litMatrixExprReturn );\
	virtual MatrixExprReturnSP appendRowsVisitor( VarMatrixExprReturn &varMatrixExprReturn );

#define APPEND_VISITOR_PIVOT_DEFINITION( CLASS ) \
	MatrixExprReturnSP CLASS::appendColumnsVisitor( LitMatrixExprReturn &litMatrixExprReturn ) {\
		return litMatrixExprReturn.appendColumns( *this );\
	}\
	MatrixExprReturnSP CLASS::appendColumnsVisitor( VarMatrixExprReturn &varMatrixExprReturn ) {\
		return varMatrixExprReturn.appendColumns( *this );\
	}\
	MatrixExprReturnSP CLASS::appendRowsVisitor( LitMatrixExprReturn &litMatrixExprReturn ) {\
		return litMatrixExprReturn.appendRows( *this );\
	}\
	MatrixExprReturnSP CLASS::appendRowsVisitor( VarMatrixExprReturn &varMatrixExprReturn ) {\
		return varMatrixExprReturn.appendRows( *this );\
	}

#define APPEND_VISITOR_IMPLEMENTATION_DECLARATION \
	virtual MatrixExprReturnSP appendColumns( ExprReturnSP exprReturnSP );\
	MatrixExprReturnSP appendColumns( VarExprReturnSP varMatrixExprReturnSP );\
	MatrixExprReturnSP appendColumns( LitMatrixExprReturnSP litMatrixExprReturnSP );\
	MatrixExprReturnSP appendColumns( VarMatrixExprReturnSP varMatrixExprReturnSP );\
	virtual MatrixExprReturnSP appendRows( ExprReturnSP exprReturnSP );\
	MatrixExprReturnSP appendRows( VarExprReturnSP varMatrixExprReturnSP );\
	MatrixExprReturnSP appendRows( LitMatrixExprReturnSP litMatrixExprReturnSP );\
	MatrixExprReturnSP appendRows( VarMatrixExprReturnSP varMatrixExprReturnSP );

#define APPEND_VISITOR_IMPLEMENTATION_DEFINITION( CLASS ) \
	MatrixExprReturnSP CLASS::appendColumns( ExprReturnSP exprReturnSP ) {\
		return exprReturnSP->appendColumnsVisitor( *this );\
	}\
	MatrixExprReturnSP CLASS::appendColumns( VarExprReturnSP varExprReturnSP ) {\
		return appendColumns( *varExprReturnSP );\
	}\
	MatrixExprReturnSP CLASS::appendColumns( LitMatrixExprReturnSP litMatrixExprReturnSP ) {\
		return appendColumns( *litMatrixExprReturnSP );\
	}\
	MatrixExprReturnSP CLASS::appendColumns( VarMatrixExprReturnSP varMatrixExprReturnSP ) {\
		return appendColumns( *varMatrixExprReturnSP );\
	}\
	MatrixExprReturnSP CLASS::appendRows( ExprReturnSP exprReturnSP ) {\
		return exprReturnSP->appendRowsVisitor( *this );\
	}\
	MatrixExprReturnSP CLASS::appendRows( VarExprReturnSP varExprReturnSP ) {\
		return appendRows( *varExprReturnSP );\
	}\
	MatrixExprReturnSP CLASS::appendRows( LitMatrixExprReturnSP litMatrixExprReturnSP ) {\
		return appendRows( *litMatrixExprReturnSP );\
	}\
	MatrixExprReturnSP CLASS::appendRows( VarMatrixExprReturnSP varMatrixExprReturnSP ) {\
		return appendRows( *varMatrixExprReturnSP );\
	}

SFC::LocalVar findMember( SFC::Struct sfcStruct, const std::string &name );

struct MemberSorter {
	bool operator()( const SFC::LocalVar &localVar1, const SFC::LocalVar &localVar2 ) {
		return static_cast< __int64 >( localVar1.statementIndex() ) <
		 static_cast< __int64 >( localVar2.statementIndex() );
	}
};

typedef std::set< SFC::LocalVar, MemberSorter > MemberSet;

typedef std::vector< ExprsProxy > ExprsProxyVector;
ExprsProxyVector &unifyExprsProxyVectors( ExprsProxyVector &exprsProxyVector1, ExprsProxyVector &exprsProxyVector2 );

class ExprReturn;
class VarExprReturn;
class ReferenceReturn;
class ArgReturn;
class MatrixExprReturn;
class LitMatrixExprReturn;
class VarMatrixExprReturn;
class ColonExprReturn;

typedef boost::shared_ptr< ExprReturn >          ExprReturnSP;
typedef boost::shared_ptr< VarExprReturn >       VarExprReturnSP;
typedef boost::shared_ptr< ReferenceReturn >     ReferenceReturnSP;
typedef boost::shared_ptr< ArgReturn >           ArgReturnSP;
typedef boost::shared_ptr< MatrixExprReturn >    MatrixExprReturnSP;
typedef boost::shared_ptr< LitMatrixExprReturn > LitMatrixExprReturnSP;
typedef boost::shared_ptr< VarMatrixExprReturn > VarMatrixExprReturnSP;
typedef boost::shared_ptr< ColonExprReturn >     ColonExprReturnSP;

typedef std::list< ArgReturnSP > ArgReturnSPList;
typedef std::list< LitMatrixExprReturnSP > LitMatrixExprReturnSPList;
typedef std::vector< VarExprReturnSP > VarExprReturnSPVector;

#endif
