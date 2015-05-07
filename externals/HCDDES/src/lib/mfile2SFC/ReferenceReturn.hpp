#ifndef _ISIS_REFERENCERETURN
#define _ISIS_REFERENCERETURN

#include "VarExprReturn.hpp"

extern "C" {
#include "MatLabTokens.h"
#include "MatLab.h"
}

typedef std::list< pANTLR3_BASE_TREE > PAntlr3BaseTreeList;

/* THIS CLASS CONTAINS:
  A) AN EXPRESSION THAT CHARACTERIZES THE REFERENCE IN SFC
  B) THE LocalVar THAT IS BEING ACCESSED BY THE REFERENCE.  THIS COULD BE A STANDALONE VARIABLE OR
     A MEMBER OF A STRUCT.  THIS LocalVar IS NEEDED ONLY IF IT DOESN'T YET HAVE A TYPE, I.E.
     IT IS PART OF AN LVALUE IN MatLab IN AN ASSIGNMENT STATEMENT THAT IS DEFINING THIS LocalVar VARIABLE.
  C) THE STRUCTURE TYPE OF WHICH THE NEXT DOT OPERATOR SHOULD REFERENCE A MEMBER,
     OR A BASIC TYPE (I.E. THIS IS *NOT* AN ARRAY TYPE).
  D) THE (PARTIAL) ARRAY TYPE THAT THE REFERENCE IS RETURNING
*/
class ReferenceReturn : public VarExprReturn {
public:
	typedef std::vector< int > DimensionVector;

	class StaticVariableAttribute : public Attribute {
	private:
		SFC::ArgDeclBase _argDeclBase;
	public:
		StaticVariableAttribute( void ) { }
		StaticVariableAttribute( const SFC::ArgDeclBase argDeclBase ) : _argDeclBase( argDeclBase ) { }
		virtual SP clone( void ) const {
			return SP(  new StaticVariableAttribute( *this )  );
		}
		void setArgDeclBase( const SFC::ArgDeclBase argDeclBase ) {
			_argDeclBase = argDeclBase;
		}
		SFC::ArgDeclBase getArgDeclBase( void ) const {
			return _argDeclBase;
		}
	};

	typedef boost::shared_ptr< StaticVariableAttribute > StaticVariableAttributeSP;
	typedef std::list< StaticVariableAttributeSP > StaticVariableAttributeSPList;

private:
	SFC::ArgDeclBase _argDeclBase;

	void setDT( SFC::DT dt ) {
		getArgDeclBase().dt() = dt;
		ExprReturn::setDT( dt );
	}
protected:
	ReferenceReturn( SFC::CompoundStatement block, SFC::ArgDeclBase argDeclBase ) :
	 VarExprReturn(   block, Exprs(  VE( argDeclBase )  ), ExprsProxyVector(), argDeclBase.dt()   ),
	 _argDeclBase( argDeclBase ) { }

	ReferenceReturn( SFC::CompoundStatement block, SFC::ArgDeclBase argDeclBase, Exprs exprs ) :
	 VarExprReturn( block, exprs ), _argDeclBase( argDeclBase ) { }

public:
	static ReferenceReturnSP create( SFC::CompoundStatement block, SFC::ArgDeclBase argDeclBase ) {
		return ReferenceReturnSP(  new ReferenceReturn( block, argDeclBase )  );
	}

	static ArgReturnSPList getArgReturnSPList( SFC::CompoundStatement block, const PAntlr3BaseTreeList &pAntlr3BaseTreeList );

	static int getNextIndex( DimensionVector &countVector, DimensionVector &dimensionVector );

	template< typename SEQUENCE >
	static SEQUENCE reshape( SEQUENCE &sourceSequence, DimensionVector &sourceDimensionVector, DimensionVector &dimensionVector );

	static ReferenceReturnSP create( SFC::CompoundStatement block, SFC::ArgDeclBase argDeclBase, Exprs exprs ) {
		return ReferenceReturnSP(  new ReferenceReturn( block, argDeclBase, exprs )  );
	}

	static ReferenceReturnSP create( ReferenceReturn &referenceReturn ) {
		return ReferenceReturnSP(  new ReferenceReturn( referenceReturn )  );
	}

	static ExprReturnSP create(
	 SFC::CompoundStatement block,
	 const std::string &id,
	 const PAntlr3BaseTreeList &pAntlrBaseTreeList,
	 bool lvalue,
	 ExprReturnSP exprReturnSP = ExprReturnSP()
	);

	static const std::string getName( void ) { return "ReferenceReturn"; }
	virtual const std::string getClassName( void ) { return getName(); }

	SFC::ArgDeclBase &getArgDeclBase( void ) { return _argDeclBase; }

	/* COMBINE */
	COMBINE_VISITOR_DECLARATION

	ExprReturnSP combine( const std::string &op, VarExprReturn &varExprReturn, bool func = false ) {
		acquireType( varExprReturn.getDT() );
		return VarExprReturn::combine( op, varExprReturn, func );
	}
	ExprReturnSP combine( const std::string &op, MatrixExprReturn &matrixExprReturn, bool func = false );


	static ExprReturnSP createFunctionCall( SFC::CompoundStatement block, const std::string &id, const ArgReturnSPList &argSPReturnList );
	static ExprReturnSP initArray( SFC::CompoundStatement block, const ArgReturnSPList &argReturnSPList, double val );

	void acquireType( SFC::DT dt ) {
		setDT( dt );
		addExprsProxies();
	}

};

#endif
