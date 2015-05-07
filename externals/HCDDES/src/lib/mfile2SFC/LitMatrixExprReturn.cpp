#include "LitMatrixExprReturn.hpp"
#include "ReferenceReturn.hpp"
#include "VarMatrixExprReturn.hpp"

COMBINE_VISITOR_DEFINITION( LitMatrixExprReturn )
MULTIPLY_VISITOR_DEFINITION( LitMatrixExprReturn )
DIVIDE_VISITOR_DEFINITION( LitMatrixExprReturn )
APPEND_VISITOR_PIVOT_DEFINITION( LitMatrixExprReturn )
APPEND_VISITOR_IMPLEMENTATION_DEFINITION( LitMatrixExprReturn )

ExprReturnSP LitMatrixExprReturn::applyUnaryOp( const std::string &op ) {
	UnaryFunction unaryFunction = getUnaryFunction( op );
	if ( unaryFunction == 0 ) {
		std::cerr << "WARNING: operation \"" << op << "\" not supported on literals." << std::endl;
		VarExprReturnSP varExprReturnSP = convertToVarExprReturn();
		return varExprReturnSP->applyUnaryOp( op );
	}

	Sequence sequence;

	for( Sequence::iterator sqnItr = getSequence().begin() ; sqnItr != getSequence().end() ; (void)++sqnItr ) {
		sequence.push_back(  unaryFunction( *sqnItr )  );
	}

	return create( getBlock(), getDT(), sequence );
}

ExprReturnSP LitMatrixExprReturn::combine( const std::string &op, LitMatrixExprReturn &litMatrixExprReturn, bool func ) {

	BinaryFunction binaryFunction = getBinaryFunction( op );
	if ( binaryFunction == 0 ) {
		std::cerr << "WARNING: operation \"" << op << "\" not supported on literals." << std::endl;
		VarExprReturnSP varExprReturnSP1 = convertToVarExprReturn();
		VarExprReturnSP varExprReturnSP2 = litMatrixExprReturn.convertToVarExprReturn();
		return varExprReturnSP1->combine( op, varExprReturnSP2, func );
	}

	Sequence sequence;

	Sequence::iterator sqnItr1 = getSequence().begin();
	Sequence::iterator sqnItr2 = litMatrixExprReturn.getSequence().begin();
	while( sqnItr1 != getSequence().end() ) {
		sequence.push_back(  binaryFunction( *sqnItr1, *sqnItr2 )  );
		(void)++sqnItr1;
		(void)++sqnItr2;
	}

	return create( getBlock(), getDT(), sequence );
}

ExprReturnSP LitMatrixExprReturn::collapse( SFC::ArgDeclBase argDeclBase ) {
	convertToVarExprReturn( argDeclBase )->collapse();
	return VarExprReturn::create( getBlock() );
}

ExprReturnSP LitMatrixExprReturn::collapseLogical() {
	if ( getDT().type() != SFC::Array::meta ) return create( *this );

	Sequence::iterator sqnItr = getSequence().begin();

	SFC::LocalVar localVar = SFCManager::createUniqueLocalVar( getBlock(), "test" );
	bool value = *sqnItr != 0;

	while( ++sqnItr != getSequence().end() ) value = value && ( *sqnItr != 0 );

	return create( getBlock(), value );
}


ExprReturnSP LitMatrixExprReturn::multiply( LitMatrixExprReturn &litMatrixExprReturn ) {

	__int64 rows1, columns1;
	getRowsAndColumns( rows1, columns1 );

	__int64 rows2, columns2;
	litMatrixExprReturn.getRowsAndColumns( rows2, columns2 );

	if ( rows1 == 1 && columns1 == 1 ) {
		double multiplier = getSequence()[0];
		Sequence sequence = litMatrixExprReturn.getSequence();
		for( Sequence::iterator sqnItr = sequence.begin() ; sqnItr != sequence.end() ; (void)++sqnItr ) {
			*sqnItr *= multiplier;
		}
		return create( getBlock(), litMatrixExprReturn.getDT(), sequence );
	}

	if ( rows2 == 1 && columns2 == 1 ) {
		double multiplier = litMatrixExprReturn.getSequence()[0];
		Sequence sequence = getSequence();
		for( Sequence::iterator sqnItr = sequence.begin() ; sqnItr != sequence.end() ; (void)++sqnItr ) {
			*sqnItr *= multiplier;
		}
		return create( getBlock(), getDT(), sequence );
	}

	if ( columns1 != rows2 ) {
		if (  ( rows1 == 1 || columns1 == 1 ) && rows1 == rows2  ) std::swap( rows1, columns1 );
		else if (  ( rows2 == 1 || columns2 == 1 ) && columns1 == columns2  ) std::swap( rows2, columns2 ); 
		else { /* THROW ERROR */ }
	}

	Sequence sequence;
	Sequence &rhsSequence = litMatrixExprReturn.getSequence();
	for( long long ix = 0 ; ix < rows1 ; (void)++ix ) {
		for( long long jx = 0 ; jx < columns2 ; (void)++jx ) {
			double sum = 0;
			for( long long kx = 0 ; kx < columns1 ; (void)++kx ) {
				sum += getSequence()[ ix*rows1 + kx ] * rhsSequence[ kx*rows2 + jx ];
			}
			sequence.push_back( sum );
		}
	}

	SFC::DT dominantDT = SFCTypesManager::getDominantType( getDT(), litMatrixExprReturn.getDT() );
	return create( getBlock(), (int) rows1, (int) columns2, dominantDT, sequence );
}

ExprReturnSP LitMatrixExprReturn::divide( LitMatrixExprReturn &litMatrixExprReturn ) {

	__int64 rows1, columns1;
	getRowsAndColumns( rows1, columns1 );
	

	__int64 rows2, columns2;
	litMatrixExprReturn.getRowsAndColumns( rows2, columns2 );
	if ( rows2 != columns2 ) {
		std::cerr << "Division not supported for non-square matrix divisor." << std::endl;
		return create( *this );
	}

	Sequence sequence;
	for( int ix = 0 ; ix < rows2 ; (void)++ix ) {
		for( int jx = 0 ; jx < rows2 ; (void)++jx ) {
			sequence.push_back( ix == jx );
		}
	}

	Sequence rhsSequence = litMatrixExprReturn.getSequence();
	for( int ix = 0 ; ix < rows2 ; (void)++ix ) {
		double num = rhsSequence[ ix * rows2 + ix ];
		if ( num == 0 ) {
			std::cerr << "Zero along diagonal detected: please fix code to deal with this" << std::endl;
			return create( *this );
		}
		for( int jx = ix ; jx < rows2 ; (void)++jx ) {
			rhsSequence[ ix * rows2 + jx ] /= num;
			sequence[ ix * rows2 + jx ] /= num;
		}
		for( int kx = 0 ; kx < rows2 ; (void)++kx ) {
			if ( kx == ix ) continue;
			double mul = rhsSequence[ kx * rows2 + ix ];
			for( int jx = ix ; jx < rows2 ; (void)++jx ) {
				rhsSequence[ kx * rows2 + jx ] -= rhsSequence[ ix * rows2 + jx ] * mul;
			}
			for( int jx = ix ; jx < rows2 ; (void)++jx ) {
				sequence[ kx * rows2 + jx ] -= sequence[ ix * rows2 + jx ] * mul;
			}
		}
	}

	SFC::DT dominantDT = SFCTypesManager::getDominantType( getDT(), litMatrixExprReturn.getDT() );
	return multiply(  create( getBlock(), (int) rows2, (int) rows2, dominantDT, sequence )  );
}

MatrixExprReturnSP LitMatrixExprReturn::appendColumns( VarExprReturn &varExprReturn ) {
	VarMatrixExprReturnSP varMatrixExprReturnSP = convertToVarMatrixExprReturn();
	return varMatrixExprReturnSP->appendColumns( varExprReturn );
}

MatrixExprReturnSP LitMatrixExprReturn::appendColumns( LitMatrixExprReturn &litMatrixExprReturn ) {
	__int64 thisRows, thisColumns;
	getRowsAndColumns( thisRows, thisColumns );
	if ( thisRows == 0 ) return LitMatrixExprReturn::create( litMatrixExprReturn );

	__int64 rhsRows, rhsColumns;
	litMatrixExprReturn.getRowsAndColumns( rhsRows, rhsColumns );
	if ( rhsRows == 0 ) return LitMatrixExprReturn::create( *this );

	assert( thisRows == rhsRows );

	Sequence sequence;
	Sequence::iterator sqnItr1 = getSequence().begin();
	Sequence::iterator sqnItr2 = litMatrixExprReturn.getSequence().begin();
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

	SFC::DT dominantDT = SFCTypesManager::getDominantType( getDT(), litMatrixExprReturn.getDT() );
	return create( getBlock(), (int) thisRows, (int) thisColumns + rhsColumns, dominantDT, sequence );
}

MatrixExprReturnSP LitMatrixExprReturn::appendColumns( VarMatrixExprReturn &varMatrixExprReturn ) {
	VarMatrixExprReturnSP varMatrixExprReturnSP = convertToVarMatrixExprReturn();
	return varMatrixExprReturnSP->appendColumns( varMatrixExprReturn );
}

MatrixExprReturnSP LitMatrixExprReturn::appendRows( VarExprReturn &varExprReturn ) {
	VarMatrixExprReturnSP varMatrixExprReturnSP = convertToVarMatrixExprReturn();
	return varMatrixExprReturnSP->appendRows( varExprReturn );
}

MatrixExprReturnSP LitMatrixExprReturn::appendRows( LitMatrixExprReturn &litMatrixExprReturn ) {
	__int64 thisRows, thisColumns;
	getRowsAndColumns( thisRows, thisColumns );
	if ( thisRows == 0 ) return LitMatrixExprReturn::create( litMatrixExprReturn );

	__int64 rhsRows, rhsColumns;
	litMatrixExprReturn.getRowsAndColumns( rhsRows, rhsColumns );
	if ( rhsRows == 0 ) return LitMatrixExprReturn::create( *this );

	assert( thisColumns == rhsColumns );

	Sequence sequence = getSequence();
	sequence.insert(
	 sequence.end(), litMatrixExprReturn.getSequence().begin(), litMatrixExprReturn.getSequence().end()
	);

	SFC::DT dominantDT = SFCTypesManager::getDominantType( getDT(), litMatrixExprReturn.getDT() );
	return create( getBlock(), (int) thisRows + rhsRows, (int) thisColumns, dominantDT, sequence );
}

MatrixExprReturnSP LitMatrixExprReturn::appendRows( VarMatrixExprReturn &varMatrixExprReturn ) {
	VarMatrixExprReturnSP varMatrixExprReturnSP = convertToVarMatrixExprReturn();
	return varMatrixExprReturnSP->appendRows( varMatrixExprReturn );
}

VarExprReturnSP LitMatrixExprReturn::convertToVarExprReturn( VarExprReturn &varExprReturn ) {
	SFC::DT dt = getDT();
//	assert( dt == varExprReturn.getDT() );

	SFCTypesManager::DimensionVector dimensionVector = SFCTypesManager::getDimensions( dt );

	if ( dimensionVector.empty() ) {
		Exprs exprs = DE( getSequence().front() );
		VarExprReturnSP varExprReturnSP = VarExprReturn::create(  getBlock(), exprs, ExprsProxyVector(), SFCTypesManager::getSingleton().getBasicType( "double" )  );
		varExprReturn.combine( "=", varExprReturnSP )->collapse();
		return VarExprReturn::create( varExprReturn );
	}

	SFCTypesManager::DimensionVector countVector( dimensionVector.size(), 0 );

	for( Sequence::iterator sqnItr = getSequence().begin() ; sqnItr != getSequence().end() ; ++sqnItr ) {

		for( unsigned int ix = 0 ; ix < (unsigned int) dimensionVector.size() ; ++ix ) {
			varExprReturn.getExprsProxyVector()[ ix ].setExprs(  IE( countVector[ ix ] )  );
		}

		Exprs exprs = BE(  varExprReturn.getExprs(), "=", DE( *sqnItr )  );
		exprs.buildUdm( getBlock(), SFC::CompoundStatement::meta_stmnt );

		for( int ix = dimensionVector.size() ; --ix >= 0 ;) {
			if ( ++countVector[ix] < dimensionVector[ix] ) break;
			countVector[ix] = 0;
		}

	}
	
	return VarExprReturn::create( varExprReturn );
}

VarExprReturnSP LitMatrixExprReturn::convertToVarExprReturn( SFC::ArgDeclBase argDeclBase ) {

	// IF TYPE ISN'T ARRAY, THEN IT MUST BE BASIC
	if ( getDT().type() != SFC::Array::meta ) {
		double value = getSequence().front();
		// SEE IF value IS CAN BE ACCURATELY REPRESENTED AS AN INTEGER
		Exprs exprs;
		if ( int(value) <= INT_MAX && int( value ) == value ) exprs = IE( (int) value );
		else                                             exprs = DE( value );
		return VarExprReturn::create( getBlock(), exprs, ExprsProxyVector(), getDT() );
	}

	if ( argDeclBase == Udm::null ) {
		argDeclBase = SFCManager::createUniqueLocalVar( getBlock(), "result" );
		argDeclBase.dt() = getDT();
	}

	VarExprReturnSP varExprReturnSP = VarExprReturn::create( getBlock(), argDeclBase, getDT() );
	return convertToVarExprReturn( *varExprReturnSP );
}


VarMatrixExprReturnSP LitMatrixExprReturn::convertToVarMatrixExprReturn( void ) {
	VarMatrixExprReturn::Sequence sequence;

	if ( !getSequence().empty() ) {
		SFC::DT dt = SFCTypesManager::getSingleton().getBaseDT( getDT() );
		for( Sequence::iterator sqnItr = getSequence().begin() ; sqnItr != getSequence().end() ; (void)++sqnItr ) {
			Exprs exprs = DE( *sqnItr );
			sequence.push_back(  VarExprReturn::create( getBlock(), exprs, ExprsProxyVector(), dt )  );
		}
	}

	return VarMatrixExprReturn::create( getBlock(), getDT(), sequence );
}


void LitMatrixExprReturn::assignTo( VarExprReturnSPVector &varExprReturnSPVector ) {

	SFC::DT dt = SFCTypesManager::getSingleton().getBaseDT( getDT() );

	VarExprReturnSPVector::iterator vsvItr = varExprReturnSPVector.begin();

	for( Sequence::iterator sqnItr = getSequence().begin() ; sqnItr != getSequence().end() ; (void)++sqnItr ) {
		VarExprReturnSP varExprReturnSP = *vsvItr;
	
		if ( varExprReturnSP->getClassName() == ReferenceReturn::getName() ) {
			ReferenceReturnSP referenceReturnSP =
			 ExprReturn::static_pointer_cast< ReferenceReturn >( varExprReturnSP );
			referenceReturnSP->acquireType( dt );
		}

		Exprs exprs = BE(  varExprReturnSP->getExprs(), "=", DE( *sqnItr )  );
		exprs.buildUdm( getBlock(), SFC::CompoundStatement::meta_stmnt );

		(void)++vsvItr;
	}
}

		
