#ifndef _ISIS_VAREXPRRETURN
#define _ISIS_VAREXPRRETURN

#include "ExprReturn.hpp"


class VarExprReturn : public ExprReturn {
public:
	typedef std::vector< int > OffsetVector;

private:
	Statement _preStatement;

	Exprs _exprs;
	ExprsProxyVector _exprsProxyVector;

	class CombineLoop {
	private:
		SFC::LocalVar _localVar;
		__int64 _upperBound;
		SFC::IterativeBlock _iterativeBlock;

		Exprs _incExprs;

		CombineLoop( CombineLoop &other );

	public:
		CombineLoop( SFC::CompoundStatement block, __int64 upperBound );

		~CombineLoop( void ) { _incExprs.buildUdm( _iterativeBlock, SFC::CompoundStatement::meta_stmnt ); }

		SFC::LocalVar getLocalVar( void ) const { return _localVar; }
		SFC::IterativeBlock getIterativeBlock( void ) const { return _iterativeBlock; }
		__int64 getUpperBound( void ) const { return _upperBound; }
	};

	void collapseAux( SFC::CompoundStatement block, ExprsProxyVector::iterator vpvItr, SFC::DT dt );
	void collapseInternal( void ) {
		if ( getExprs().isEmpty() ) return;
		collapseAux( getBlock(), getExprsProxyVector().begin(), getDT() );
	}
	VarExprReturnSP collapseWithVarInternal( SFC::ArgDeclBase argDeclBase );
	VarExprReturnSP collapseWithVarInternal( void );

	bool checkFlattenVector( SFC::Struct sfcStruct ) {

		MemberSet memberSet = sfcStruct.LocalVar_kind_children_sorted( MemberSorter() );
		for( MemberSet::iterator mbsItr = memberSet.begin() ; mbsItr != memberSet.end() ; (void)++mbsItr ) {
			SFC::LocalVar localVar = *mbsItr;
			SFC::DT dt = localVar.dt();

			if ( dt.type() == SFC::Struct::meta ) {
				SFC::Struct memberStruct = SFC::Struct::Cast( dt );
				if (  !checkFlattenVector( memberStruct )  ) return false;
			} else {
				SFCTypesManager::DimensionVector dimensionVector = SFCTypesManager::getDimensions( dt );
				if ( dimensionVector.size() > 1 ) return false;
				SFC::DT baseDT = SFCTypesManager::getBaseDT( dt );
				if (
				 baseDT.type() == SFC::Struct::meta && !checkFlattenVector(  SFC::Struct::Cast( baseDT )  )
				) return false;
			}
		}

		return true;
	}

	VarMatrixExprReturnSP flatten( void ) {
		assert( getDT().type() == SFC::Struct::meta );

		SFC::Struct sfcStruct = SFC::Struct::Cast( getDT() );
		return flatten(  sfcStruct, getExprs(), checkFlattenVector( sfcStruct )  );
	}

	VarMatrixExprReturnSP flatten( SFC::Struct sfcStruct, Exprs exprs, bool vector = false );

protected:
	VarExprReturn( SFC::CompoundStatement block, int level = 0 ) : ExprReturn( block, level ) { }

	VarExprReturn( SFC::CompoundStatement block, Exprs &exprs, int level = 0 ) :
	 ExprReturn( block, level ), _exprs( exprs ) { }

	VarExprReturn( SFC::CompoundStatement block, const Exprs &exprs, const ExprsProxyVector &exprsProxyVector, SFC::DT dt, int level = 0, Statement preStatement = Statement() ) :
	 ExprReturn( block, dt, level ), _preStatement( preStatement ), _exprs( exprs ), _exprsProxyVector( exprsProxyVector ) { }

	VarExprReturn( SFC::CompoundStatement block, Exprs &exprs, SFC::DT dt, int level = 0, Statement preStatement = Statement() ) :
	 ExprReturn( block, dt, level ), _preStatement( preStatement ), _exprs( exprs ) {
		addExprsProxies();
	}

	VarExprReturn( SFC::CompoundStatement block, SFC::ArgDeclBase argDeclBase, SFC::DT dt ) :
	 ExprReturn( block, dt ), _exprs(  VE( argDeclBase )  ) {
		addExprsProxies();
	}

	VarExprReturn( SFC::CompoundStatement block, SFC::ArgDeclBase argDeclBase, const OffsetVector &offsetVector, SFC::DT dt ) :
	 ExprReturn( block, dt ), _exprs(  VE( argDeclBase )  ) {
		addExprsProxies( offsetVector );
	}

	void setPreStatement( Statement &statement ) { _preStatement = statement; }
	void setExprs( Exprs &exprs ) { _exprs = exprs; }
	void setExprsProxyVector( ExprsProxyVector &exprsProxyVector ) { _exprsProxyVector = exprsProxyVector; }

	void addExprsProxies( const OffsetVector &offsetVector = OffsetVector() );

public:
	static VarExprReturnSP create( SFC::CompoundStatement block, int level = 0 ) {
		return VarExprReturnSP(  new VarExprReturn( block, level )  );
	}

	static VarExprReturnSP create( SFC::CompoundStatement block, Exprs &exprs, int level = 0 ) {
		return VarExprReturnSP(  new VarExprReturn( block, exprs, level )  );
	}

	static VarExprReturnSP create(
	 SFC::CompoundStatement block, 
	 const Exprs &exprs,
	 const ExprsProxyVector &exprsProxyVector,
	 SFC::DT dt,
	 int level = 0,
	 Statement preStatement = Statement()
	) {
		return VarExprReturnSP(  new VarExprReturn( block, exprs, exprsProxyVector, dt, level, preStatement )  );
	}

	static VarExprReturnSP create(
	 SFC::CompoundStatement block, 
	 Exprs &exprs,
	 SFC::DT dt,
	 int level = 0,
	 Statement preStatement = Statement()
	) {
		return VarExprReturnSP(  new VarExprReturn( block, exprs, dt, level, preStatement )  );
	}

	static VarExprReturnSP create( SFC::CompoundStatement block, SFC::ArgDeclBase argDeclBase, SFC::DT dt ) {
		return VarExprReturnSP(  new VarExprReturn( block, argDeclBase, dt )  );
	}

	static VarExprReturnSP create(
	 SFC::CompoundStatement block, SFC::ArgDeclBase argDeclBase, OffsetVector &offsetVector, SFC::DT dt
	) {
		return VarExprReturnSP(  new VarExprReturn( block, argDeclBase, offsetVector, dt )  );
	}

	static VarExprReturnSP create(
	 SFC::CompoundStatement block, SFC::ArgDeclBase argDeclBase, int offset, SFC::DT dt
	) {
		return VarExprReturnSP(  new VarExprReturn( block, argDeclBase, OffsetVector( 1, offset ), dt )  );
	}

	static VarExprReturnSP create( VarExprReturn &varExprReturn ) {
		return VarExprReturnSP(  new VarExprReturn( varExprReturn )  );
	}

	static VarExprReturnSP create( VarExprReturnSP varExprReturnSP ) {
		return create( *varExprReturnSP );
	}

	static const std::string getName( void ) { return "VarExprReturn"; }
	virtual const std::string getClassName( void ) { return getName(); }

	/* APPLYUNARYOP */
	virtual ExprReturnSP applyUnaryOp( const std::string &op ) {
		Exprs exprs = UE( op, getExprs() );
		return create( getBlock(), exprs, getExprsProxyVector(), getDT(), getLevel() + 1, getPreStatement() );
	}

	/* COMBINE */
	COMBINE_VISITOR_DECLARATION

	ExprReturnSP combine( const std::string &op, VarExprReturn &varExprReturn, bool func = false );
	ExprReturnSP combine(
	 const std::string &op, MatrixExprReturn &matrixExprReturn, bool func = false
	);

	/* COLLAPSE */
	virtual void collapse( void ) {
		collapseInternal();
	};
	virtual ExprReturnSP collapseLogical( void );

	/* MULTIPLY */
	MULTIPLY_VISITOR_DECLARATION

	ExprReturnSP multiply( VarExprReturn &varExprReturn );
	ExprReturnSP multiply( MatrixExprReturn &matrixExprReturn );

	/* APPEND MATRIX PIVOT */
	APPEND_VISITOR_PIVOT_DECLARATION

	/* TRANSPOSE */
	virtual ExprReturnSP transpose( void );

	Statement &getPreStatement( void ) { return _preStatement; }
	virtual Exprs &getExprs( void ) { return _exprs; }
	ExprsProxyVector &getExprsProxyVector( void ) { return _exprsProxyVector; }

	/* BUILDUDM */
	virtual void buildUdm( SFC::CompoundStatement block, const Uml::CompositionChildRole &childRole = SFC::CompoundStatement::meta_stmnt ) {
		_preStatement.buildUdm( block, SFC::CompoundStatement::meta_stmnt );
		_exprs.buildUdm( block, childRole );
	}

	virtual VarExprReturnSP convertToVarExprReturn( SFC::ArgDeclBase argDeclBase = SFC::ArgDeclBase() ) {
		if ( argDeclBase == Udm::null ) return create( *this );
		return collapseWithVarInternal( argDeclBase );
	}
	virtual VarMatrixExprReturnSP convertToVarMatrixExprReturn( void );

	virtual void assignTo( VarExprReturnSPVector &varExprReturnSPVector );


	SFC::DT dotCombineDT( SFC::DT rhsDT );
	VarExprReturnSP dotCombine( VarExprReturnSP varExprReturnSP );
	ReferenceReturnSP dotCombine( ReferenceReturnSP referenceReturnSP );
};

#endif
