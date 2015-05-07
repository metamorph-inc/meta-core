#include "VarMatrixExprReturn.hpp"
#include "ReferenceReturn.hpp"

APPEND_VISITOR_PIVOT_DEFINITION( VarMatrixExprReturn )
APPEND_VISITOR_IMPLEMENTATION_DEFINITION( VarMatrixExprReturn )

ExprReturnSP VarMatrixExprReturn::collapse( SFC::ArgDeclBase argDeclBase ) {
	convertToVarExprReturn( argDeclBase )->collapse();
	return VarExprReturn::create( getBlock() );
}

MatrixExprReturnSP VarMatrixExprReturn::appendColumns( VarExprReturn &varExprReturn ) {

	__int64 rhsRows, rhsColumns;
	varExprReturn.getRowsAndColumns( rhsRows, rhsColumns );
	if ( rhsRows == 0 ) return VarMatrixExprReturn::create( *this );

	VarMatrixExprReturnSP varMatrixExprReturnSP = varExprReturn.convertToVarMatrixExprReturn();

	__int64 thisRows, thisColumns;
	getRowsAndColumns( thisRows, thisColumns );
	if ( thisRows == 0 ) return varMatrixExprReturnSP;

	return appendColumns( varMatrixExprReturnSP );
}

MatrixExprReturnSP VarMatrixExprReturn::appendColumns( VarMatrixExprReturn &varMatrixExprReturn ) {

	__int64 thisRows, thisColumns;
	getRowsAndColumns( thisRows, thisColumns );
	if ( thisRows == 0 ) return VarMatrixExprReturn::create( varMatrixExprReturn );

	__int64 rhsRows, rhsColumns;
	varMatrixExprReturn.getRowsAndColumns( rhsRows, rhsColumns );
	if ( rhsRows == 0 ) return VarMatrixExprReturn::create( *this );

	assert( thisRows == rhsRows );

	Sequence sequence;
	Sequence::iterator sqnItr1 = getSequence().begin();
	Sequence::iterator sqnItr2 = varMatrixExprReturn.getSequence().begin();
	for( int ix = 0 ; ix < thisRows ; (void)++ix ) {
		for ( int jx = 0 ; jx < thisColumns ; (void)++jx ) {
			sequence.push_back( *sqnItr1 );
			(void)++sqnItr1;
		}
		for ( int jx = 0 ; jx < rhsColumns ; (void)++jx ) {
			sequence.push_back( *sqnItr2 );
			(void)++sqnItr2;
		}
	}

	SFC::DT dominantDT = SFCTypesManager::getDominantType( getDT(), varMatrixExprReturn.getDT() );

	return create( getBlock(), (int) thisRows, (int) thisColumns + rhsColumns, dominantDT, sequence );
}

MatrixExprReturnSP VarMatrixExprReturn::appendRows( VarExprReturn &varExprReturn ) {

	__int64 rhsRows, rhsColumns;
	varExprReturn.getRowsAndColumns( rhsRows, rhsColumns );
	if ( rhsColumns == 0 ) return VarMatrixExprReturn::create( *this );

	VarMatrixExprReturnSP varMatrixExprReturnSP = varExprReturn.convertToVarMatrixExprReturn();

	__int64 thisRows, thisColumns;
	getRowsAndColumns( thisRows, thisColumns );
	if ( thisColumns == 0 ) return varMatrixExprReturnSP;

	return appendRows( varMatrixExprReturnSP );
}

MatrixExprReturnSP VarMatrixExprReturn::appendRows( VarMatrixExprReturn &varMatrixExprReturn ) {

	__int64 thisRows, thisColumns;
	getRowsAndColumns( thisRows, thisColumns );
	if ( thisRows == 0 ) return VarMatrixExprReturn::create( varMatrixExprReturn );

	__int64 rhsRows, rhsColumns;
	varMatrixExprReturn.getRowsAndColumns( rhsRows, rhsColumns );
	if ( rhsRows == 0 ) return VarMatrixExprReturn::create( *this );

	assert( thisColumns == rhsColumns );

	Sequence sequence = getSequence();
	sequence.insert(
	 sequence.end(), varMatrixExprReturn.getSequence().begin(), varMatrixExprReturn.getSequence().end()
	);

	SFC::DT dominantDT = SFCTypesManager::getSingleton().getDominantType( getDT(), varMatrixExprReturn.getDT() );

	return create( getBlock(), (int) thisRows + rhsRows, (int) thisColumns, dominantDT, sequence );
}

VarExprReturnSP VarMatrixExprReturn::convertToVarExprReturn( VarExprReturn &varExprReturn ) {

	SFC::DT dt = getDT();
	SFCTypesManager::DimensionVector dv1 = SFCTypesManager::getAllDimensions( dt );
	SFCTypesManager::DimensionVector dv2 = SFCTypesManager::getAllDimensions( varExprReturn.getDT() );

	VarExprReturnSP varExprReturnSP = VarExprReturn::create( varExprReturn );

	if (
	 dv1.size() == 1 && dv2.size() == 2 && dv1[ 0 ] > 1 && dv2[ 0 ] == 1 && dv1[ 0 ] == dv2[ 1 ] ||
	 dv2.size() == 1 && dv1.size() == 2 && dv2[ 0 ] > 1 && dv1[ 0 ] == 1 && dv2[ 0 ] == dv1[ 1 ]
	) {
		ExprReturnSP exprReturnSP = varExprReturnSP->transpose();
		varExprReturnSP = ExprReturn::static_pointer_cast< VarExprReturn >( exprReturnSP );
	}

//	assert( dt == varExprReturnSP->getDT() );

	/* SEE IF THE MATRIX IS MADE OF ONLY ONE VarExprReturn OBJECT.  IF SO, JUST RETURN THE OBJECT */

	typedef std::set< VarExprReturnSP > VarExprReturnSPSet;
	VarExprReturnSPSet varExprReturnSPSet;


	SFCTypesManager::DimensionVector dimensionVector = SFCTypesManager::getDimensions( dt );

	if ( dimensionVector.empty() ) {

		varExprReturnSP->combine( "=", getSequence().front() )->collapse();

	} else if ( dimensionVector.size() == 1 ) {

		int counter = 0;
		for( Sequence::iterator sqnItr = getSequence().begin() ; sqnItr != getSequence().end() ; ++sqnItr ) {

			VarExprReturnSP subVarExprReturnSP = *sqnItr;

			if (  varExprReturnSPSet.find( subVarExprReturnSP ) != varExprReturnSPSet.end()  ) continue;

			SFCTypesManager::DimensionVector subDimensionVector = SFCTypesManager::getDimensions( subVarExprReturnSP->getDT() );

			ExprsProxyVector exprsProxyVector;

			Exprs indexExprs;
			if ( subDimensionVector.empty() ) {
				indexExprs = IE( counter );
			} else {
				exprsProxyVector.push_back( ExprsProxy() );
				indexExprs = exprsProxyVector.front();
				if ( counter != 0 ) indexExprs = BE(  indexExprs, "+", IE( counter )  );
				varExprReturnSPSet.insert( subVarExprReturnSP );
			}

			varExprReturnSP->getExprsProxyVector().front().setExprs( indexExprs );

			VarExprReturnSP lhsVarExprReturnSP = VarExprReturn::create( getBlock(), varExprReturnSP->getExprs(), exprsProxyVector, dt );

			lhsVarExprReturnSP->combine( "=", subVarExprReturnSP )->collapse();

			++counter;
		}


	} else {

		SFCTypesManager::DimensionVector countVector( dimensionVector.size(), 0 );

		for( Sequence::iterator sqnItr = getSequence().begin() ; sqnItr != getSequence().end() ; ++sqnItr ) {

			VarExprReturnSP subVarExprReturnSP = *sqnItr;

			if (  varExprReturnSPSet.find( subVarExprReturnSP ) != varExprReturnSPSet.end()  ) continue;

			SFCTypesManager::DimensionVector subDimensionVector = SFCTypesManager::getAllDimensions( subVarExprReturnSP->getDT() );

			SFCTypesManager::DimensionVector::iterator dmvItr = subDimensionVector.begin();

			ExprsProxyVector exprsProxyVector;

			for( int ix = 0 ; ix < countVector.size() ; ++ix ) {

				int index = countVector[ ix ];
				Exprs indexExprs;

				int subDimension = *dmvItr;
				if ( subDimension == 1 ) {
					indexExprs = IE( index );
				} else {
					exprsProxyVector.push_back( ExprsProxy() );
					indexExprs = exprsProxyVector.back();
					if ( index != 0 ) indexExprs = BE(  indexExprs, "+", IE( index )  );
					varExprReturnSPSet.insert( subVarExprReturnSP );
				}

				varExprReturnSP->getExprsProxyVector()[ix].setExprs( indexExprs );
				++dmvItr;
			}

			VarExprReturnSP lhsVarExprReturnSP = VarExprReturn::create( getBlock(), varExprReturnSP->getExprs(), exprsProxyVector, subVarExprReturnSP->getDT() );

			lhsVarExprReturnSP->combine( "=", subVarExprReturnSP )->collapse();

			for( int ix = dimensionVector.size() ; --ix >= 0 ;) {
				if ( ++countVector[ix] < dimensionVector[ix] ) break;
				countVector[ix] = 0;
			}
		}
	}

	return varExprReturnSP;
}


VarExprReturnSP VarMatrixExprReturn::convertToVarExprReturn( SFC::ArgDeclBase argDeclBase ) {

	if ( argDeclBase == Udm::null ) {
		Sequence::iterator sqnItr = getSequence().begin();
		VarExprReturnSP varExprReturnSP = *sqnItr;
		(void)++sqnItr;
		while( sqnItr != getSequence().end() ) {
			if ( *sqnItr != varExprReturnSP ) break;
			(void)++sqnItr;
		}
		if ( sqnItr == getSequence().end() ) return varExprReturnSP;

		argDeclBase = SFCManager::createUniqueLocalVar( getBlock(), "result" );
		argDeclBase.dt() = getDT();
	}

	VarExprReturnSP varExprReturnSP = VarExprReturn::create( getBlock(), argDeclBase, getDT() );
	return convertToVarExprReturn( *varExprReturnSP );
}


void VarMatrixExprReturn::assignTo( VarExprReturnSPVector &varExprReturnSPVector ) {
	SFC::DT dt = SFCTypesManager::getSingleton().getBaseDT( getDT() );

	VarExprReturnSPVector::iterator vsvItr = varExprReturnSPVector.begin();

	int ix = 0;
	VarExprReturnSP testVarExprReturnSP;

	for( Sequence::iterator sqnItr = getSequence().begin() ; sqnItr != getSequence().end() ; (void)++sqnItr ) {
		VarExprReturnSP varExprReturnSP1 = *vsvItr;

		if ( varExprReturnSP1->getClassName() == ReferenceReturn::getName() ) {
			ReferenceReturnSP referenceReturnSP =
			 ExprReturn::static_pointer_cast< ReferenceReturn >( varExprReturnSP1 );
			referenceReturnSP->acquireType( dt );
		}

		VarExprReturnSP varExprReturnSP2 = *sqnItr;

		__int64 rows, columns;
		varExprReturnSP2->getRowsAndColumns( rows, columns );
		int maxIndex = (int) max( rows, columns );

		if ( maxIndex == 1 ) {
			Exprs exprs = BE( varExprReturnSP1->getExprs(), "=", varExprReturnSP2->getExprs() );
			exprs.buildUdm( getBlock(), SFC::CompoundStatement::meta_stmnt );
		} else {

			if ( testVarExprReturnSP != varExprReturnSP2 ) {
				testVarExprReturnSP = varExprReturnSP2;
				ix = 0;
			}

			varExprReturnSP2->getExprsProxyVector().front().setExprs(  IE( ix )  );

			Exprs exprs = BE( varExprReturnSP1->getExprs(), "=", varExprReturnSP2->getExprs() );
			exprs.buildUdm( getBlock(), SFC::CompoundStatement::meta_stmnt );

			(void)++ix;
		}

		(void)++vsvItr;
	}
}
