#ifndef _ISIS_LITMATRIXEXPRRETURN
#define _ISIS_LITMATRIXEXPRRETURN

#include "MatrixExprReturn.hpp"

class LitMatrixExprReturn : public MatrixVectorExprReturn<  std::vector< double >  > {
public:
	typedef std::vector< double > Sequence;
	typedef MatrixVectorExprReturn< Sequence > Super;

private:
	bool _isText;

	void init( void ) {
		_isText = false;
	}

	LitMatrixExprReturn( SFC::CompoundStatement block, int rows, int columns, SFC::DT dt, Sequence &sequence ) :
	 Super(  block, SFCTypesManager::getSingleton().getMatrix( rows, columns, dt ), sequence  )
	{ init(); }

	LitMatrixExprReturn( SFC::CompoundStatement block, SFC::DT dt, Sequence &sequence ) :
	 Super( block, dt, sequence ) { init(); }

	LitMatrixExprReturn( SFC::CompoundStatement block, double value ) :
	 Super(  block, SFCTypesManager::getSingleton().getBasicType( "double" ), Sequence( 1, value )  ) { init(); }

	LitMatrixExprReturn( SFC::CompoundStatement block, int value ) :
	 Super(  block, SFCTypesManager::getSingleton().getBasicType( "int" ), Sequence( 1, value )  ) { init(); }

	LitMatrixExprReturn( SFC::CompoundStatement block, bool value ) :
	 Super(  block, SFCTypesManager::getSingleton().getBasicType( "boolean" ), Sequence( 1, value )  ) { init(); }

	LitMatrixExprReturn( SFC::CompoundStatement block ) : Super( block ) { init(); }

	typedef double (*BinaryFunction)( double, double );

	class BinaryFunctionMapClass {
	private:
		static double add( double x, double y ) { return x+y; }
		static double sub( double x, double y ) { return x-y; }
		static double mul( double x, double y ) { return x*y; }
		static double div( double x, double y ) { return x/y; }
		static double ldv( double x, double y ) { return y/x; }
		static double pow( double x, double y ) { return ::pow( x, y ); }

		typedef std::map< std::string, BinaryFunction > BinaryFunctionMap;

		BinaryFunctionMap _binaryFunctionMap;

	public:
		BinaryFunctionMapClass( void ) {
			_binaryFunctionMap.insert(  std::make_pair( std::string("+"),  add )  );
			_binaryFunctionMap.insert(  std::make_pair( std::string("-"),  sub )  );
			_binaryFunctionMap.insert(  std::make_pair( std::string("*"),  mul )  );
			_binaryFunctionMap.insert(  std::make_pair( std::string("/"),  div )  );
			_binaryFunctionMap.insert(  std::make_pair( std::string("\\"), ldv )  );
			_binaryFunctionMap.insert(  std::make_pair( std::string("^"),  pow )  );
		}

		BinaryFunction getBinaryFunction( const std::string &op ) {
			BinaryFunctionMap::iterator bfmItr = _binaryFunctionMap.find( op );
			if ( bfmItr == _binaryFunctionMap.end() ) return 0;
			return bfmItr->second;
		}
	};

	static BinaryFunction getBinaryFunction( const std::string &op ) {
		static BinaryFunctionMapClass binaryFunctionMapObject;
		return binaryFunctionMapObject.getBinaryFunction( op );
	}

	typedef double (*UnaryFunction)( double );

	class UnaryFunctionMapClass {
	private:
		static double pos( double x ) { return x; }
		static double neg( double x ) { return -x; }

		typedef std::map< std::string, UnaryFunction > UnaryFunctionMap;

		UnaryFunctionMap _unaryFunctionMap;

	public:
		UnaryFunctionMapClass( void ) {
			_unaryFunctionMap.insert(  std::make_pair( std::string("+"), pos )  );
			_unaryFunctionMap.insert(  std::make_pair( std::string("-"), neg )  );
		}

		UnaryFunction getUnaryFunction( const std::string &op ) {
			UnaryFunctionMap::iterator ufmItr = _unaryFunctionMap.find( op );
			if ( ufmItr == _unaryFunctionMap.end() ) return 0;
			return ufmItr->second;
		}
	};

	static UnaryFunction getUnaryFunction( const std::string &op ) {
		static UnaryFunctionMapClass unaryFunctionMapObject;
		return unaryFunctionMapObject.getUnaryFunction( op );
	}

public:
	static LitMatrixExprReturnSP create( SFC::CompoundStatement block, int rows, int columns, SFC::DT dt, Sequence &sequence ) {
		return LitMatrixExprReturnSP(  new LitMatrixExprReturn( block, rows, columns, dt, sequence )  );
	}
	static LitMatrixExprReturnSP create( SFC::CompoundStatement block, SFC::DT dt, Sequence &sequence ) {
		return LitMatrixExprReturnSP(  new LitMatrixExprReturn( block, dt, sequence )  );
	}
	static LitMatrixExprReturnSP create( SFC::CompoundStatement block, int value ) {
		return LitMatrixExprReturnSP(  new LitMatrixExprReturn( block, value )  );
	}
	static LitMatrixExprReturnSP create( SFC::CompoundStatement block, double value ) {
		return LitMatrixExprReturnSP(  new LitMatrixExprReturn( block, value )  );
	}
	static LitMatrixExprReturnSP create( SFC::CompoundStatement block ) {
		return LitMatrixExprReturnSP(  new LitMatrixExprReturn( block )  );
	}
	static LitMatrixExprReturnSP create( LitMatrixExprReturn &litMatrixExprReturn ) {
		return LitMatrixExprReturnSP(  new LitMatrixExprReturn( litMatrixExprReturn )  );
	}

	void setIsText( bool isText ) { _isText = isText; }
	bool getIsText( void ) const { return _isText; }

	std::string getText( void ) {

		std::string retval;

		Sequence &sequence = getSequence();
		for( Sequence::iterator sqnItr = sequence.begin() ; sqnItr != sequence.end() ; ++sqnItr ) {
			retval += int( *sqnItr );
		}
		return retval;
	}

	virtual ExprReturnSP createMatrix( SFC::CompoundStatement block, int rows, int columns, SFC::DT dt, Sequence &sequence ) {
		return create( block, rows, columns, dt, sequence );
	}

	static const std::string getName( void ) { return "LitMatrixExprReturn"; }
	virtual const std::string getClassName( void ) { return getName(); }

	/* APPLYUNARYOP */
	virtual ExprReturnSP applyUnaryOp( const std::string &op );

	/* COMBINE */
	COMBINE_VISITOR_DECLARATION

	ExprReturnSP combine(
	 const std::string &op, VarExprReturn &varExprReturn, bool func = false
	) {
		return MatrixExprReturn::combine( op, varExprReturn, func );
	}
	ExprReturnSP combine(
	 const std::string &op, MatrixExprReturn &matrixExprReturn, bool func = false
	) {
		return MatrixExprReturn::combine( op, matrixExprReturn, func );
	}
	ExprReturnSP combine(
	 const std::string &op, LitMatrixExprReturn &litMatrixExprReturn, bool func = false
	);

	/* COLLAPSE */
	virtual ExprReturnSP collapse( SFC::ArgDeclBase argDeclBase );
	virtual void collapse( void ) { };
	virtual ExprReturnSP collapseLogical( void );

	/* MULTIPLY */
	MULTIPLY_VISITOR_DECLARATION

	ExprReturnSP multiply( VarExprReturn &varExprReturn ) {
		return MatrixExprReturn::multiply( varExprReturn );
	}
	ExprReturnSP multiply( MatrixExprReturn &matrixExprReturn ) {
		return MatrixExprReturn::multiply( matrixExprReturn );
	}
	ExprReturnSP multiply( LitMatrixExprReturn &litMatrixExprReturn );

	/* MULTIPLY */
	DIVIDE_VISITOR_DECLARATION

	ExprReturnSP divide( ExprReturn &exprReturn ) {
		return ExprReturn::divide( exprReturn );
	}
	ExprReturnSP divide( LitMatrixExprReturn &litMatrixExprReturn );

	/* APPEND MATRIX PIVOT */
	APPEND_VISITOR_PIVOT_DECLARATION
	APPEND_VISITOR_IMPLEMENTATION_DECLARATION

	MatrixExprReturnSP appendColumns( VarExprReturn &varExprReturn );
	MatrixExprReturnSP appendColumns( LitMatrixExprReturn &litMatrixExprReturn );
	MatrixExprReturnSP appendColumns( VarMatrixExprReturn &varMatrixExprReturn );

	MatrixExprReturnSP appendRows( VarExprReturn &varExprReturn );
	MatrixExprReturnSP appendRows( LitMatrixExprReturn &litMatrixExprReturn );
	MatrixExprReturnSP appendRows( VarMatrixExprReturn &varMatrixExprReturn );

	Sequence::value_type getItem( int x ) { return getSequence()[ x ]; }

	/* BUILDUDM */
	virtual void buildUdm(
	 SFC::CompoundStatement block, const Uml::CompositionChildRole &childRole = SFC::CompoundStatement::meta_stmnt
	) {
		if ( getDT().type() != SFC::Array::meta ) {
			Exprs exprs = DE( getSequence().front() );
			exprs.buildUdm( block, childRole );
			return;
		}

		ExprReturnSP exprReturnSP = collapseLogical();
		exprReturnSP->buildUdm( block );
	}

	virtual VarExprReturnSP convertToVarExprReturn( SFC::ArgDeclBase argDeclBase = SFC::ArgDeclBase() );
	virtual VarExprReturnSP convertToVarExprReturn( VarExprReturn &varExprReturn );

	virtual VarMatrixExprReturnSP convertToVarMatrixExprReturn( void );

	virtual void assignTo( VarExprReturnSPVector &varExprReturnSPVector );
};

#endif