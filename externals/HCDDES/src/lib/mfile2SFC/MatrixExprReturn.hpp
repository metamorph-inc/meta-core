#ifndef _ISIS_MATRIXEXPRRETURN
#define _ISIS_MATRIXEXPRRETURN

#include "ExprReturn.hpp"
#include "VarExprReturn.hpp"

class MatrixExprReturn : public ExprReturn {

protected:
	MatrixExprReturn( SFC::CompoundStatement block, int rows, int columns ) :
	 ExprReturn(  block, SFCTypesManager::getSingleton().getMatrix( rows, columns, "double" )  ) { }

	MatrixExprReturn( SFC::CompoundStatement block, SFC::DT dt ) :
	 ExprReturn( block, dt ) { }

	MatrixExprReturn( SFC::CompoundStatement block ) : ExprReturn( block ) { }

public:

	/* APPLYUNARYOP */
	virtual ExprReturnSP applyUnaryOp( const std::string &op ) {
		VarExprReturnSP varExprReturnSP = convertToVarExprReturn();
		return varExprReturnSP->applyUnaryOp( op );
	}

	/* COMBINE */
	COMBINE_VISITOR_DECLARATION

	ExprReturnSP combine( const std::string &op, VarExprReturn &varExprReturn, bool func = false ) {
		VarExprReturnSP varExprReturnSP = convertToVarExprReturn();
		return varExprReturnSP->combine( op, varExprReturn, func );
	}
	ExprReturnSP combine(
	 const std::string &op, MatrixExprReturn &matrixExprReturn, bool func = false
	) {
		VarExprReturnSP varExprReturnSP1 = convertToVarExprReturn();
		VarExprReturnSP varExprReturnSP2 = matrixExprReturn.convertToVarExprReturn();
		return varExprReturnSP1->combine( op, varExprReturnSP2, func );
	}

	virtual ExprReturnSP collapseLogical( void ) {
		VarExprReturnSP varExprReturnSP = convertToVarExprReturn();
		return varExprReturnSP->collapseLogical();
	}

	/* MULTIPLY */
	MULTIPLY_VISITOR_DECLARATION

	ExprReturnSP multiply( VarExprReturn &varExprReturn ) {
		VarExprReturnSP varExprReturnSP = convertToVarExprReturn();
		return varExprReturnSP->multiply( varExprReturn );
	}
	ExprReturnSP multiply( MatrixExprReturn &matrixExprReturn ) {
		VarExprReturnSP varExprReturnSP1 = convertToVarExprReturn();
		VarExprReturnSP varExprReturnSP2 = matrixExprReturn.convertToVarExprReturn();
		return varExprReturnSP1->multiply( varExprReturnSP2 );
	}

	/* APPEND MATRIX PIVOT */
	virtual MatrixExprReturnSP appendRows( ExprReturnSP exprReturnSP ) = 0;
	virtual MatrixExprReturnSP appendColumns( ExprReturnSP exprReturnSP ) = 0;

	virtual VarExprReturnSP convertToVarExprReturn( SFC::ArgDeclBase argDeclBase = SFC::ArgDeclBase() ) = 0;
	virtual VarExprReturnSP convertToVarExprReturn( VarExprReturn &varExprReturn ) = 0;
};

template< typename SEQUENCE >
class MatrixVectorExprReturn : public MatrixExprReturn {
public:
	typedef SEQUENCE Sequence;
private:
	Sequence _sequence;

protected:
	MatrixVectorExprReturn( SFC::CompoundStatement block, int rows, int columns, const Sequence &sequence ) :
	 MatrixExprReturn( block, rows, columns ), _sequence( sequence ) { }

	MatrixVectorExprReturn( SFC::CompoundStatement block, SFC::DT dt, const Sequence &sequence = Sequence() ) :
	 MatrixExprReturn( block, dt ), _sequence( sequence ) { }

	MatrixVectorExprReturn( SFC::CompoundStatement block ) : MatrixExprReturn( block ) { }

	void setSequence( Sequence &sequence ) { _sequence = sequence; }

public:

	Sequence &getSequence( void ) { return _sequence; }

	/* TRANSPOSE */
	virtual ExprReturnSP transpose( void );
	virtual ExprReturnSP createMatrix( SFC::CompoundStatement block, int rows, int columns, SFC::DT dt, Sequence &sequence ) = 0;

	typename Sequence::iterator begin( void ) { return getSequence().begin(); }
	typename Sequence::iterator end( void ) { return getSequence().end(); }
	int size( void ) { return getSequence().size(); }

	typedef typename Sequence::iterator iterator;
};

template< typename SEQUENCE >
ExprReturnSP MatrixVectorExprReturn< SEQUENCE >::transpose( void ) {
	__int64 rows, columns;
	getRowsAndColumns( rows, columns );

	typedef std::vector< SEQUENCE > SequenceVector;

	SequenceVector sequenceVector( columns );

	SEQUENCE sequence;
	SEQUENCE thisSequence = getSequence();

	if ( rows > 1 && columns > 1 ) {

		typename SequenceVector::iterator sqvItr = sequenceVector.begin();
		for(
		 typename SEQUENCE::iterator sqnItr = thisSequence.begin() ;
		 sqnItr != thisSequence.end() ;
		 (void)++sqnItr
		) {
			sqvItr->push_back( *sqnItr );
			(void)++sqvItr; if ( sqvItr == sequenceVector.end() ) sqvItr = sequenceVector.begin();
		}

		for( sqvItr = sequenceVector.begin() ; sqvItr != sequenceVector.end() ; (void)++sqvItr ) {
			sequence.insert( sequence.end(), sqvItr->begin(), sqvItr->end() );
		}
	} else {
		sequence = getSequence();
	}

	return createMatrix( getBlock(), (int) columns, (int) rows, SFCTypesManager::getBaseDT( getDT() ), sequence );
}

#endif
