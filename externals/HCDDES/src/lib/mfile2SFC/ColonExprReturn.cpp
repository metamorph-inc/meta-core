#include "ColonExprReturn.hpp"
#include "LitMatrixExprReturn.hpp"

ExprReturnSP ColonExprReturn::create( SFC::CompoundStatement block, ExprReturnSPVector &exprReturnSPVector ) {

	LitMatrixExprReturnSPVector litMatrixExprReturnSPVector;

	bool allLiteral = true;
	for(
	 ExprReturnSPVector::iterator esvItr = exprReturnSPVector.begin() ;
	 esvItr != exprReturnSPVector.end() ;
	 (void)++esvItr
	) {
		LitMatrixExprReturnSP litMatrixExprReturnSP;
		if ( (*esvItr)->getClassName() == LitMatrixExprReturn::getName() ) {
			litMatrixExprReturnSP = ExprReturn::static_pointer_cast< LitMatrixExprReturn >( *esvItr );
		}
		if ( litMatrixExprReturnSP == 0 ) allLiteral = false;
		litMatrixExprReturnSPVector.push_back( litMatrixExprReturnSP );
	}

	if ( allLiteral ) {
		double startValue = litMatrixExprReturnSPVector.front()->getItem( 0 );
		double endValue = litMatrixExprReturnSPVector.back()->getItem( 0 );
		double step;

		if ( litMatrixExprReturnSPVector.size() == 2 ) step = startValue <= endValue ? 1 : -1;
		else                                           step = litMatrixExprReturnSPVector[ 1 ]->getItem( 0 );
		
		int dim = (int) (( endValue - startValue )/step + 1.0);
		assert( dim > 0 );

		if ( dim == 1 ) return litMatrixExprReturnSPVector.front();

		ExprsProxyVector exprsProxyVector( 1 );
		ExprsProxy &exprsProxy = exprsProxyVector[ 0 ];
		Exprs exprs;

		if ( step == 1 )       exprs = exprsProxy;
		else if ( step == -1 ) exprs = UE( "-", exprsProxy );
		else                   exprs = BE(  DE( step ), "*", exprsProxy  );

		if ( startValue != 0 ) {
			std::string op = startValue > 0 ? "+" : "-";
			exprs = BE(  exprs, op, fabs( startValue )  );
		}
		
		SFC::DT dt = SFCTypesManager::getSingleton().getMatrix(
		 dim, 1, SFCTypesManager::getSingleton().getBasicType( "double" )
		);

		Exprs dimExprs = IE( dim );
		return ColonExprReturnSP(  new ColonExprReturn( block, dimExprs, exprs, exprsProxyVector, dt )  );
	}

	Exprs startExprs = exprReturnSPVector.front()->convertToVarExprReturn()->getExprs();
	Exprs endExprs = exprReturnSPVector.back()->convertToVarExprReturn()->getExprs();
	Exprs stepExprs;

	if ( exprReturnSPVector.size() == 2 ) {
		SFC::LocalVar localVar = SFCManager::createUniqueLocalVar( block, "step" );
		localVar.dt() = SFCTypesManager::getSingleton().getBasicType( "double" );
		stepExprs = VE( localVar );
		CG cg(
		 SS(
		  CB(
		   BE( startExprs, "<", endExprs ),
		   BE(  stepExprs, "=", IE( 1 )  )
		  ),
		  CB(
		   IE( 1 ),
		   BE(  stepExprs, "=", IE( -1 )  )
		  )
		 )
		);
		cg.buildUdm( block, SFC::CompoundStatement::meta_stmnt );
	} else {
		stepExprs = exprReturnSPVector[1]->convertToVarExprReturn()->getExprs();
	}

	SFC::LocalVar localVar = SFCManager::createUniqueLocalVar( block, "dim" );
	localVar.dt() = SFCTypesManager::getSingleton().getBasicType( "int" );
	Exprs dimExprs = VE( localVar );
	BE be(
	 dimExprs,
	 "=",
	 BE(   BE(  BE( endExprs, "-", startExprs ), "/", stepExprs  ), "+", IE( 1 )   )
	);
	be.buildUdm( block, SFC::CompoundStatement::meta_stmnt );

	ExprsProxyVector exprsProxyVector( 1 );
	ExprsProxy &exprsProxy = exprsProxyVector[ 0 ];

	Exprs exprs = BE(  BE( stepExprs, "*", exprsProxy ), "+", startExprs  );

	return ColonExprReturnSP(  new ColonExprReturn( block, dimExprs, exprs, exprsProxyVector, SFC::DT() )  );
}
