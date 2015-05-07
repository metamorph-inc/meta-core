#ifndef _ISIS_EXPRRETURN
#define _ISIS_EXPRRETURN

/* THIS CLASS CONTAINS:
 	A) AN Exprs EXPRESSION TO GENERATE ITS SFC EQUIVALENT
 	B) THE TYPE OF THE EXPRESSION (INCLUDES DIMENSIONS)
*/

#include "RuleReturn.hpp"

class ExprReturn {
private:
	SFC::CompoundStatement _block;
	SFC::DT _dt;
	int _level;

	static bool &retrieveZeroIndex( void ) {
		static bool zeroIndex = false;
		return zeroIndex;
	}

public:
	class EndStack {
	public:
		typedef std::vector< int > Stack;

	private:
		Stack _endStack;

	public:

		int back( void ) {
			if ( _endStack.empty() ) {
				std::cerr << "WARNING: \"back\" METHOD CALLED FOR EndStack: RETURNING (-1)" << std::endl;
				return -1;
			}

			return _endStack.back();
		}

		void push_back( int value ) {
			_endStack.push_back( value );
		}

		void pop_back( void ) {
			_endStack.pop_back();
		}

		void replace_back( int value ) {
			_endStack.back() = value;
		}
	};

	static EndStack &getEndStack( void ) {
		static EndStack endStack;
		return endStack;
	}

	class Attribute {
	public:
		typedef boost::shared_ptr< Attribute > SP;
		virtual SP clone( void ) const = 0;
	};
	typedef Attribute::SP AttributeSP;
	typedef std::list< AttributeSP > AttributeSPList;

private:
	AttributeSPList _attributeSPList;

public:
	void addAttribute( const Attribute &attribute ) {
		_attributeSPList.push_back( attribute.clone() );
	}

	void combineAttributes( ExprReturn &exprReturn ) {
		AttributeSPList attributeSPList = exprReturn.getAttributes< Attribute >();
		_attributeSPList.insert( _attributeSPList.end(), attributeSPList.begin(), attributeSPList.end() );
	}

	void combineAttributes( ExprReturnSP exprReturnSP ) {
		combineAttributes( *exprReturnSP );
	}

	template< typename DERIVED_ATTRIBUTE >
	std::list<  boost::shared_ptr< DERIVED_ATTRIBUTE >  > getAttributes( void ) {

		typedef DERIVED_ATTRIBUTE DerivedAttribute;
		typedef boost::shared_ptr< DerivedAttribute > DerivedAttributeSP;
		typedef std::list< DerivedAttributeSP > DerivedAttributeSPList;

		DerivedAttributeSPList derivedAttributeSPList;

		for( AttributeSPList::iterator aslItr = _attributeSPList.begin() ; aslItr != _attributeSPList.end() ; ++aslItr ) {
			DerivedAttributeSP derivedAttributeSP = boost::dynamic_pointer_cast< DerivedAttribute, Attribute >( *aslItr );
			if ( derivedAttributeSP != 0 ) {
				derivedAttributeSPList.push_back( derivedAttributeSP );
			}
		}

		return derivedAttributeSPList;
	}

protected:
	ExprReturn( SFC::CompoundStatement block, int level = 0 ) : _block( block ), _level( level ) { }
	ExprReturn( SFC::CompoundStatement block, SFC::DT dt, int level = 0 ) :
	 _block( block ), _dt( dt ), _level( level ) { }

public:
	static bool getZeroIndex( void ) { return retrieveZeroIndex(); }
	static void setZeroIndex( bool zeroIndex ) { retrieveZeroIndex() = zeroIndex; }

	virtual const std::string getClassName( void ) = 0;

	virtual ~ExprReturn( void ) { }

	template< typename ER >
	static boost::shared_ptr< ER > dynamic_pointer_cast( ExprReturnSP exprReturnSP ) {
		return boost::dynamic_pointer_cast< ER, ExprReturn >( exprReturnSP );
	}

	template< typename DER, typename BER >
	static boost::shared_ptr< DER > static_pointer_cast( boost::shared_ptr< BER > &berSP ) {
		return boost::static_pointer_cast< DER, BER >( berSP );
	}

	SFC::CompoundStatement getBlock( void ) { return _block; }

	SFC::DT getDT( void ) { return _dt; }
	void setDT( SFC::DT dt ) { _dt = dt; }

	int getLevel( void ) { return _level; }
	bool isStructType( void ) { return getDT() != Udm::null && getDT().type() == SFC::Struct::meta; }
	bool hasType( void ) { return getDT() != Udm::null; }

	void getRowsAndColumns( __int64 &rows, __int64 &columns ) {
		SFCTypesManager::getRowsAndColumns( getDT(), rows, columns );
	}

	SFCTypesManager::DimensionVector getDimensions( void ) {
		return SFCTypesManager::getDimensions( getDT() );
	}

	SFCTypesManager::DimensionVector getAllDimensions( void ) {
		return SFCTypesManager::getAllDimensions( getDT() );
	}

	/* APPLYUNARYOP */
	virtual ExprReturnSP applyUnaryOp( const std::string &op ) = 0;

	/* COMBINE */
	virtual ExprReturnSP combineVisitor( const std::string &op, VarExprReturn &varExprReturn, bool func = false ) = 0;
	virtual ExprReturnSP combineVisitor( const std::string &op, ReferenceReturn &referenceReturn, bool func = false ) = 0;
	virtual ExprReturnSP combineVisitor( const std::string &op, LitMatrixExprReturn &litMatrixExprReturn, bool func = false ) = 0;
	virtual ExprReturnSP combineVisitor( const std::string &op, MatrixExprReturn &matrixExprReturn, bool func = false ) = 0;
	virtual ExprReturnSP combine( const std::string &op, ExprReturnSP exprReturnSP, bool func = false ) = 0;
	virtual ExprReturnSP combine( const std::string &op, ExprReturn &exprReturn, bool func = false ) = 0;

	ExprReturnSP combine( const std::string &op, double value, bool func = false );

	/* COLLAPSE */
	virtual void collapse( void ) = 0;
	virtual ExprReturnSP collapseLogical( void ) = 0;

	/* MULTIPLY */
	virtual ExprReturnSP multiplyVisitor( VarExprReturn &varExprReturn ) = 0;
	virtual ExprReturnSP multiplyVisitor( LitMatrixExprReturn &litMatrixExprReturn ) = 0;
	virtual ExprReturnSP multiplyVisitor( MatrixExprReturn &matrixExprReturn ) = 0;
	virtual ExprReturnSP multiply( ExprReturnSP exprReturnSP ) = 0;

	/* DIVIDE */
	DIVIDE_VISITOR_DECLARATION

	virtual ExprReturnSP divide( ExprReturn &exprReturn );

	/* APPEND MATRIX PIVOT */
	virtual MatrixExprReturnSP appendColumnsVisitor( LitMatrixExprReturn &litMatrixExprReturn ) = 0;
	virtual MatrixExprReturnSP appendColumnsVisitor( VarMatrixExprReturn &varMatrixExprReturn ) = 0;
	virtual MatrixExprReturnSP appendRowsVisitor( LitMatrixExprReturn &litMatrixExprReturn ) = 0;
	virtual MatrixExprReturnSP appendRowsVisitor( VarMatrixExprReturn &varMatrixExprReturn ) = 0;

	/* TRANSPOSE */
	virtual ExprReturnSP transpose( void ) = 0;

	/* BUILDUDM */
	virtual void buildUdm( SFC::CompoundStatement block, const Uml::CompositionChildRole &childRole = SFC::CompoundStatement::meta_stmnt ) = 0;
	void buildUdm( const Uml::CompositionChildRole &childRole = SFC::CompoundStatement::meta_stmnt ) {
		buildUdm( getBlock(), childRole );
	}

	/* CONVERSION TO VarExprReturnSP */
	virtual VarExprReturnSP convertToVarExprReturn( SFC::ArgDeclBase argDeclBase = SFC::ArgDeclBase() ) = 0;
	virtual VarMatrixExprReturnSP convertToVarMatrixExprReturn( void ) = 0;

	/* ASSIGNMENT TO MATRIX */
	virtual void assignTo( VarExprReturnSPVector &varExprReturnSPVector ) = 0;

	typedef std::map< std::string, SFC::Function > FunctionNameMap;

	static FunctionNameMap &getFunctionNameMap( void ) {
		static FunctionNameMap functionNameMap;
		return functionNameMap;
	}
};

#endif
