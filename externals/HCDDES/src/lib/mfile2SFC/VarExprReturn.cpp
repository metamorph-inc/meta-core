#include "VarExprReturn.hpp"
#include "ReferenceReturn.hpp"
#include "LitMatrixExprReturn.hpp"
#include "VarMatrixExprReturn.hpp"

COMBINE_VISITOR_DEFINITION( VarExprReturn )
MULTIPLY_VISITOR_DEFINITION( VarExprReturn )
APPEND_VISITOR_PIVOT_DEFINITION( VarExprReturn )

/**/
/* VarExprReturn */
/**/
void VarExprReturn::collapseAux( SFC::CompoundStatement block, ExprsProxyVector::iterator vpvItr, SFC::DT dt) {
	if ( vpvItr == getExprsProxyVector().end() || dt.type() != SFC::Array::meta ) {
		buildUdm( block );
		return;
	}

	SFC::Array sfcArray = SFC::Array::Cast( dt );
	if ( sfcArray.noelem() == 1 ) {
		collapseAux( block, vpvItr, sfcArray.dt() );
	} else {
		CombineLoop loop( block, sfcArray.noelem() );
		vpvItr->setExprs(  VE( loop.getLocalVar() )  );
		collapseAux( loop.getIterativeBlock(), vpvItr + 1, sfcArray.dt() );
	}
}

VarExprReturnSP VarExprReturn::collapseWithVarInternal( SFC::ArgDeclBase argDeclBase ) {

	ReferenceReturnSP referenceReturnSP = ReferenceReturn::create( getBlock(), argDeclBase );

	referenceReturnSP->combine( "=", *this )->collapse();

	VarExprReturnSP varExprReturnSP = VarExprReturn::create( getBlock(), argDeclBase, getDT() );

	return varExprReturnSP;
}

VarExprReturnSP VarExprReturn::collapseWithVarInternal( void ) {
	return collapseWithVarInternal(  SFCManager::createUniqueLocalVar( getBlock(), "result" )  );
}

ExprReturnSP VarExprReturn::collapseLogical( void ) {

	if ( getDT().type() != SFC::Array::meta ) {
		return create( *this );
	}

	SFC::LocalVar localVar = SFCManager::createUniqueLocalVar( getBlock(), "test" );
	localVar.dt() = SFCTypesManager::getSingleton().getBasicType( "boolean" );
	Exprs exprs = BE(  VE( localVar ), "=", IE( 1 )  );
	exprs.buildUdm( getBlock(), SFC::CompoundStatement::meta_stmnt );

	exprs = BE(   VE( localVar ), "=", BE(  VE( localVar ), "&&", getExprs()  )   );
	VarExprReturnSP varExprReturnSP = create(  getBlock(), exprs, getExprsProxyVector(), getDT(), getLevel() + 1, getPreStatement()  );

	varExprReturnSP->collapse();

	Exprs localVarExprs = VE( localVar );
	return create(  getBlock(), localVarExprs, ExprsProxyVector(), SFCTypesManager::getSingleton().getBasicType( "int" )  );
}


VarExprReturn::CombineLoop::CombineLoop( SFC::CompoundStatement block, __int64 upperBound ) :
 _upperBound( upperBound ) {

	_localVar = SFCManager::createUniqueLocalVar( block );
	_localVar.dt() = SFCTypesManager::getSingleton().getBasicType( "int" );
	Exprs exprs = BE(  VE( _localVar ), "=", IE( 0 )  );
	exprs.buildUdm( block, SFC::CompoundStatement::meta_stmnt );

	_iterativeBlock = SFC::IterativeBlock::Create( block );
	SFCManager::numberStatement( _iterativeBlock );

	exprs = BE(  VE( _localVar ), "<", IE( (int) upperBound )  );
	exprs.buildUdm( _iterativeBlock, SFC::IterativeBlock::meta_cond );

	_incExprs = UE(  "++", VE( _localVar )  );
}

ExprReturnSP VarExprReturn::combine( const std::string &op, VarExprReturn &varExprReturn, bool func ) {

	if ( op == "=" ) {
		if ( !isStructType() && varExprReturn.isStructType() ) {
			return combine( op, varExprReturn.convertToVarMatrixExprReturn() );
		}
	} else {
		if ( isStructType() ) {
			VarMatrixExprReturnSP varMatrixExprReturnSP = convertToVarMatrixExprReturn();
			return varMatrixExprReturnSP->combine( op, varExprReturn, func );
		}
		if ( varExprReturn.isStructType() ) {
			VarMatrixExprReturnSP varMatrixExprReturnSP = convertToVarMatrixExprReturn();
			return combine( op, varExprReturn.convertToVarMatrixExprReturn(), func );
		}
	}

	ExprsProxyVector exprsProxyVector = unifyExprsProxyVectors( getExprsProxyVector(), varExprReturn.getExprsProxyVector() );

	Exprs exprs;
	if ( func ) {
		FE fe( op );
		fe.addArgVal( getExprs() );
		fe.addArgVal( varExprReturn.getExprs() );
		exprs = fe;
	} else {
		exprs = BE( getExprs(), op, varExprReturn.getExprs() );
	}

	return create(
	 getBlock(),
	 exprs,
	 exprsProxyVector,
	 SFCTypesManager::getSingleton().combineDT( getDT(), varExprReturn.getDT() ),
	 getLevel() + varExprReturn.getLevel() + 1,
	 getPreStatement().combine( varExprReturn.getPreStatement() )
	);
}

ExprReturnSP VarExprReturn::combine(
 const std::string &op,
 MatrixExprReturn &matrixExprReturn,
 bool func
) {
	if ( op == "=" ) {
		matrixExprReturn.convertToVarExprReturn( *this );
		return VarExprReturn::create( getBlock() );
	}
	VarExprReturnSP varExprReturnSP = matrixExprReturn.convertToVarExprReturn();
	return combine( op, varExprReturnSP, func );
}

ExprReturnSP VarExprReturn::multiply( VarExprReturn &varExprReturn ) {

	if (  getDT().type() == SFC::Struct::meta ) {
		VarMatrixExprReturnSP varMatrixExprReturnSP = convertToVarMatrixExprReturn();
		return varMatrixExprReturnSP->multiply( varExprReturn );
	}
	if ( varExprReturn.getDT().type() == SFC::Struct::meta ) {
		VarMatrixExprReturnSP varMatrixExprReturnSP = varExprReturn.convertToVarMatrixExprReturn();
		return multiply( varMatrixExprReturnSP );
	}

	__int64 rows1, columns1;
	getRowsAndColumns( rows1, columns1 );

	__int64 rows2, columns2;
	varExprReturn.getRowsAndColumns( rows2, columns2 );

	if ( rows1 == 1 && columns1 == 1 || rows2 == 1 && columns2 == 1 ) return combine( "*", varExprReturn );

	if ( columns1 != rows2 ) {
		if (  ( rows1 == 1 || columns1 == 1 ) && rows1 == rows2  ) std::swap( rows1, columns1 );
		else if (  ( rows2 == 1 || columns2 == 1 ) && columns1 == columns2  ) std::swap( rows2, columns2 ); 
		else { /* THROW ERROR */ }
	}

	VarExprReturnSP lhsVarExprReturnSP;
	if ( getLevel() > 0 ) lhsVarExprReturnSP = collapseWithVarInternal();
	VarExprReturn &lhsVarExprReturn = getLevel() > 0 ? *lhsVarExprReturnSP : *this;

	VarExprReturnSP rhsVarExprReturnSP;
	if ( varExprReturn.getLevel() > 0 ) rhsVarExprReturnSP = varExprReturn.collapseWithVarInternal();
	VarExprReturn &rhsVarExprReturn = varExprReturn.getLevel() > 0 ? *rhsVarExprReturnSP : varExprReturn;


	ExprsProxyVector prodExprsProxyVector;
	if ( rows1 > 1 ) prodExprsProxyVector.push_back( lhsVarExprReturn.getExprsProxyVector().front() );
	if ( columns2 > 1 ) prodExprsProxyVector.push_back( rhsVarExprReturn.getExprsProxyVector().back() );

	SFC::DT dominantDT = SFCTypesManager::getDominantType( getDT(), varExprReturn.getDT() );
	SFC::DT dt = SFCTypesManager::getSingleton().getMatrix( (int) rows1, (int) columns2, dominantDT );

	/* MAKE NECESSARY LOOPS */
	Statement statement;
	Exprs exprs;
	SFC::LocalVar sumLV;
	int level = 0;
	if ( columns1 > 1 ) {
		SFC::LocalVar kxLV = SFCManager::createUniqueLocalVar( getBlock(), "kx" );
		kxLV.dt() = SFCTypesManager::getSingleton().getBasicType( "int" );
		lhsVarExprReturn.getExprsProxyVector().back().unify( rhsVarExprReturn.getExprsProxyVector().front() );
		lhsVarExprReturn.getExprsProxyVector().back().setExprs(  VE( kxLV )  );

		sumLV = SFCManager::createUniqueLocalVar( getBlock(), "sum" );
		sumLV.dt() = dominantDT;
		statement = SS(
		 SS(
		  US(   BE(  VE( kxLV ), "=", IE( 0 )  )   ),
		  US(   BE(  VE( sumLV ), "=", IE( 0 )  )   )
		 ),
		 IS(
		  BE(  VE( kxLV ), "<", IE( (int) columns1 )  ),
		  SS(
		   lhsVarExprReturn.getPreStatement().combine( rhsVarExprReturn.getPreStatement() ),
		   SS(
		    US(   BE(  VE( sumLV ), "+=", BE( lhsVarExprReturn.getExprs(), "*", rhsVarExprReturn.getExprs() )  )   ),
		    US(   UE(  "++", VE( kxLV )  )   )
		   )
		  )
		 )
		);
		exprs = VE( sumLV );
		level = 1;
	} else {
		statement = lhsVarExprReturn.getPreStatement().combine( rhsVarExprReturn.getPreStatement() );
		exprs = BE( lhsVarExprReturn.getExprs(), "*", rhsVarExprReturn.getExprs() );
		level = max( lhsVarExprReturn.getLevel(), rhsVarExprReturn.getLevel() ) + 1;
	}

	return create( getBlock(), exprs, prodExprsProxyVector, dt, level, statement );
}
ExprReturnSP VarExprReturn::multiply( MatrixExprReturn &matrixExprReturn ) {
	VarExprReturnSP varExprReturnSP = matrixExprReturn.convertToVarExprReturn();
	return multiply( varExprReturnSP );
}

ExprReturnSP VarExprReturn::transpose( void ) {
	if ( getExprsProxyVector().size() == 0 ) return create( *this );

	__int64 rows, columns;
	getRowsAndColumns( rows, columns );

	SFC::DT dt = SFCTypesManager::getSingleton().getMatrix(
	 (int) columns, (int) rows, SFCTypesManager::getSingleton().getBaseDT( getDT() )
	);

	ExprsProxyVector exprsProxyVector;
	switch( getExprsProxyVector().size() ) {
		case 1:
		{
			exprsProxyVector = getExprsProxyVector();
			break;
		}

		case 2:
		{
			exprsProxyVector.push_back( getExprsProxyVector()[1] );
			exprsProxyVector.push_back( getExprsProxyVector()[0] );
			break;
		}

	}

	return create( getBlock(), getExprs(), exprsProxyVector, dt, getLevel(), getPreStatement() );
}

VarMatrixExprReturnSP VarExprReturn::convertToVarMatrixExprReturn( void ) {
	if ( isStructType() ) return flatten();

	VarExprReturnSP varExprReturnSP = create( *this );
	__int64 rows, columns;
	getRowsAndColumns( rows, columns );

	VarMatrixExprReturn::Sequence sequence( rows*columns, varExprReturnSP );

	SFC::DT dt = SFCTypesManager::getBaseDT( getDT() );
	return VarMatrixExprReturn::create( getBlock(), (int) rows, (int) columns, dt, sequence );
}

SFC::DT VarExprReturn::dotCombineDT( SFC::DT rhsDT ) {
	SFC::DT dt = SFCTypesManager::getSingleton().copyArrayDT( getDT() );

	SFC::Array lastArray = SFCTypesManager::getArrayTail( dt );
	if ( lastArray == Udm::null ) return rhsDT;

	lastArray.dt() = rhsDT;
	SFCTypesManager::getSingleton().makeTypeUnique( dt );

	assert( dt != Udm::null );
	return dt;
}


VarExprReturnSP VarExprReturn::dotCombine( VarExprReturnSP varExprReturnSP ) {
	ExprsProxyVector exprsProxyVector = getExprsProxyVector();

	exprsProxyVector.insert(
	 exprsProxyVector.end(),
	 varExprReturnSP->getExprsProxyVector().begin(),
	 varExprReturnSP->getExprsProxyVector().end()
	);
	Exprs exprs = BE( getExprs(), ".", varExprReturnSP->getExprs() );
	SFC::DT dt = dotCombineDT( varExprReturnSP->getDT() );
	return create( getBlock(), exprs, exprsProxyVector, dt );
}

ReferenceReturnSP VarExprReturn::dotCombine( ReferenceReturnSP referenceReturnSP ) {
	Exprs exprs = BE( getExprs(), ".", referenceReturnSP->getExprs() );
	return ReferenceReturn::create( getBlock(), referenceReturnSP->getArgDeclBase(), exprs );
}

void VarExprReturn::addExprsProxies( const OffsetVector &offsetVector ) {
	SFC::DT dt = getDT();
	
	OffsetVector::const_iterator ofvCit = offsetVector.begin();

	while( dt.type() == SFC::Array::meta ) {
		SFC::Array sfcArray = SFC::Array::Cast( dt );
		dt = sfcArray.dt();

		if ( sfcArray.noelem() == 1 ) continue;
		getExprsProxyVector().push_back( ExprsProxy() );

		Exprs exprs = getExprsProxyVector().back();
		if ( ofvCit != offsetVector.end() ) {
			if ( *ofvCit != 0 ) exprs = BE(  exprs, "+", IE( *ofvCit )  );
			(void)++ofvCit;
		}

		exprs = BE( getExprs(), "[", exprs );
		setExprs( exprs );

	}
}

void VarExprReturn::assignTo( VarExprReturnSPVector &varExprReturnSPVector ) {

	SFC::DT dt = SFCTypesManager::getSingleton().getBaseDT( getDT() );

	int ix = 0;
	for(
	 VarExprReturnSPVector::iterator vsvItr = varExprReturnSPVector.begin() ;
	 vsvItr != varExprReturnSPVector.end() ;
	 (void)++vsvItr
	) {
		VarExprReturnSP varExprReturnSP = *vsvItr;

		if ( varExprReturnSP->getClassName() == ReferenceReturn::getName() ) {
			ReferenceReturnSP referenceReturnSP =
			 ExprReturn::static_pointer_cast< ReferenceReturn >( varExprReturnSP );
			referenceReturnSP->acquireType( dt );
		}

		getExprsProxyVector().front().setExprs(  IE( ix )  );

		Exprs exprs = BE( varExprReturnSP->getExprs(), "=", getExprs() );
		exprs.buildUdm( getBlock(), SFC::CompoundStatement::meta_stmnt );

		(void)++ix;
	}
}


VarMatrixExprReturnSP VarExprReturn::flatten( SFC::Struct sfcStruct, Exprs exprs, bool vector ) {

	VarMatrixExprReturnSP varMatrixExprReturnSP = VarMatrixExprReturn::create( getBlock() );
	MatrixExprReturnSP matrixExprReturnSP;

	MemberSet memberSet = sfcStruct.LocalVar_kind_children_sorted( MemberSorter() );
	for( MemberSet::iterator mbsItr = memberSet.begin() ; mbsItr != memberSet.end() ; (void)++mbsItr ) {
		SFC::LocalVar localVar = *mbsItr;
		SFC::DT dt = localVar.dt();

		__int64 rows, columns;
		varMatrixExprReturnSP->getRowsAndColumns( rows, columns );

		Exprs memberExprs = BE(  exprs, ".", VE( localVar )  );
		ExprReturnSP nextExprReturnSP;
		if ( dt.type() == SFC::Struct::meta ) {
			SFC::Struct memberStruct = SFC::Struct::Cast( dt );
			nextExprReturnSP = flatten( memberStruct, memberExprs, vector );
		} else {
			nextExprReturnSP = VarExprReturn::create( getBlock(), memberExprs, dt );
		}

		__int64 nextRows, nextColumns;
		nextExprReturnSP->getRowsAndColumns( nextRows, nextColumns );

		if ( rows == nextRows || rows == 0 ) {
			matrixExprReturnSP = varMatrixExprReturnSP->appendColumns( nextExprReturnSP );
		} else if ( columns == nextColumns ) {
			matrixExprReturnSP = varMatrixExprReturnSP->appendRows( nextExprReturnSP );
		} else if ( vector ) {

			SFC::DT nextDT = nextExprReturnSP->getDT();

			if ( columns == 1 && nextRows == 1 ) {
				nextExprReturnSP->setDT( SFCTypesManager::getSingleton().getMatrix(
				 (int) nextColumns, 1, SFCTypesManager::getBaseDT( nextDT )
				) );
				matrixExprReturnSP = varMatrixExprReturnSP->appendColumns( nextExprReturnSP );
			} else if ( rows == 1 && nextColumns == 1 ) {
				nextExprReturnSP->setDT( SFCTypesManager::getSingleton().getMatrix(
				 1, (int) nextRows, SFCTypesManager::getBaseDT( nextDT )
				) );
				matrixExprReturnSP = varMatrixExprReturnSP->appendRows( nextExprReturnSP );
			} else {
				assert( false );
			}

		} else {

			assert( false );

		}

		varMatrixExprReturnSP = ExprReturn::static_pointer_cast< VarMatrixExprReturn >( matrixExprReturnSP );
	}

	return varMatrixExprReturnSP;

}
