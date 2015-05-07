#ifndef _ISIS_VARMATRIXEXPRRETURN
#define _ISIS_VARMATRIXEXPRRETURN

#include "MatrixExprReturn.hpp"
#include "LitMatrixExprReturn.hpp"

class VarMatrixExprReturn : public MatrixVectorExprReturn< VarExprReturnSPVector > {
public:
	typedef VarExprReturnSPVector Sequence;
	typedef MatrixVectorExprReturn< Sequence > Super;

private:
	VarMatrixExprReturn( SFC::CompoundStatement block ) : Super( block ) { }
	VarMatrixExprReturn( SFC::CompoundStatement block, int rows, int columns, SFC::DT dt, Sequence &sequence ) :
	 Super( block, SFCTypesManager::getSingleton().getMatrix( rows, columns, dt ), sequence ) { }

	VarMatrixExprReturn( SFC::CompoundStatement block, SFC::DT dt, Sequence &sequence ) :
	 Super( block, dt, sequence ) { }

public:
	static VarMatrixExprReturnSP create( SFC::CompoundStatement block ) {
		return VarMatrixExprReturnSP(  new VarMatrixExprReturn( block )  );
	}
	static VarMatrixExprReturnSP create(
	 SFC::CompoundStatement block, int rows, int columns, SFC::DT dt, Sequence &sequence
	) {
		return VarMatrixExprReturnSP(  new VarMatrixExprReturn( block, rows, columns, dt, sequence )  );
	}
	static VarMatrixExprReturnSP create( SFC::CompoundStatement block, SFC::DT dt, Sequence &sequence ) {
		return VarMatrixExprReturnSP(  new VarMatrixExprReturn( block, dt, sequence )  );
	}
	static VarMatrixExprReturnSP create( VarMatrixExprReturn &varMatrixExprReturn ) {
		return VarMatrixExprReturnSP(  new VarMatrixExprReturn( varMatrixExprReturn )  );
	}

	virtual ExprReturnSP createMatrix( SFC::CompoundStatement block, int rows, int columns, SFC::DT dt, Sequence &sequence ) {
		return create( block, rows, columns, dt, sequence );
	}

	static const std::string getName( void ) { return "VarMatrixExprReturn"; }
	virtual const std::string getClassName( void ) { return getName(); }

	/* COMBINE -- DONE IN MatrixExprReturn BASE CLASS */

	/* COLLAPSE */
	virtual ExprReturnSP collapse( SFC::ArgDeclBase argDeclBase );
	virtual void collapse( void ) {	};

	/* MULTIPLY -- DONE IN MatrixExprReturn BASE CLASS */

	/* APPEND MATRIX PIVOT */
	APPEND_VISITOR_PIVOT_DECLARATION
	APPEND_VISITOR_IMPLEMENTATION_DECLARATION

	MatrixExprReturnSP appendColumns( VarExprReturn &varExprReturn );
	MatrixExprReturnSP appendColumns( LitMatrixExprReturn &litMatrixExprReturn ) {
		VarMatrixExprReturnSP varMatrixExprReturnSP = litMatrixExprReturn.convertToVarMatrixExprReturn();
		return appendColumns( varMatrixExprReturnSP );
	}
	MatrixExprReturnSP appendColumns( VarMatrixExprReturn &varMatrixExprReturn );

	MatrixExprReturnSP appendRows( VarExprReturn &varExprReturn );
	MatrixExprReturnSP appendRows( LitMatrixExprReturn &litMatrixExprReturn ) {
		VarMatrixExprReturnSP varMatrixExprReturnSP = litMatrixExprReturn.convertToVarMatrixExprReturn();
		return appendRows( varMatrixExprReturnSP );
	}
	MatrixExprReturnSP appendRows( VarMatrixExprReturn &varMatrixExprReturn );

	/* BUILDUDM */
	virtual void buildUdm(
	 SFC::CompoundStatement block, const Uml::CompositionChildRole &childRole = SFC::CompoundStatement::meta_stmnt
	) {
		if ( getDT().type() != SFC::Array::meta ) {
			getSequence().front()->buildUdm( block, childRole );
			return;
		}

		ExprReturnSP exprReturnSP = collapseLogical();
		exprReturnSP->buildUdm( block );
	}

	virtual VarExprReturnSP convertToVarExprReturn( SFC::ArgDeclBase argDeclBase = SFC::ArgDeclBase() );
	virtual VarExprReturnSP convertToVarExprReturn( VarExprReturn &varExprReturn );


	virtual VarMatrixExprReturnSP convertToVarMatrixExprReturn( void ) { return create( *this ); }

	virtual void assignTo( VarExprReturnSPVector &varExprReturnSPVector );
};

#endif