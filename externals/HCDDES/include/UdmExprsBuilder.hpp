#ifndef ISIS_UDM_EXPRS_BUILDER
#define ISIS_UDM_EXPRS_BUILDER

#include <string>
#include "unify_ptr.hpp"
#include <map>
#include <set>
#include <list>
#include "SFC.h"
#include "Uml.h"

typedef std::list< std::string > StringList;
typedef std::list< double > DoubleList;

//
// SMART POINTER TO Exprs CLASS
//
class ExprsBase;
typedef boost::shared_ptr< ExprsBase > ExprsBaseSP;
typedef std::list<ExprsBaseSP>	ExprsBaseSPList; 
//
// Exprs class
// BASE CLASS FOR EXPRESSION PROCESSING
// 
// ALSO ACTS AS A PROXY FOR TYPES DERIVED FROM Exprs (THIS IS FOR SYNTAX SIMPLIFICATION)
//
class ExprsBase {

public:
	//
	// VIRTUAL CONSTRUCTOR
	//
	virtual ExprsBaseSP clone( void ) const = 0;
	virtual std::string getClassName( void ) const = 0;
	//
	// PROCESSING (CREATE UDM DATA TREE)
	//
	virtual void buildUdmAux( const Udm::Object &parent, const Uml::CompositionChildRole &childRole ) const = 0;

	void buildUdm( const Udm::Object &parent, const Uml::CompositionChildRole &childRole ) const;

	virtual bool isBound( void ) { return true; }
};

//
// PROXY CLASS FOR DERIVED CLASSES OF ExprsBase
// SOLE PURPOSE IS TO ALLOW ANY EXPRESSION TO BE DEFINED AS "Exprs" INSTEAD OF IN A
// PARTICULAR DERIVED CLASS OF ExprsBase
// FOR EXAMPLE, WITH THIS Exprs CLASS, THESE DEFINTIONS:
// BE exprs1 = BE( ... );
// UE exprs2 = UE( ... );
// CAN BE CHANGED TO
// Exprs exprs1 = BE( ... );
// Exprs exprs2 = UE( ... );
//
class Exprs : public ExprsBase {
private:
	ExprsBaseSP _exprsBaseSP;

public:
	//
	// THESE CONSTRUCTORS EXIST SOLELY FOR SYNTAX SIMPLIFICATION, I.E. ALL CONSTRUCTORS
	// OF DERIVED CLASSES TAKE "const Exprs &", E.G. "UE( std::string &op, const Exprs &subexpr)".
	// WITHOUT THESE CONSTRUCTORS, "UE" EXPRESSION CONSTRUCTION WOULD HAVE TO LOOK LIKE THIS:
	// UE(  "!", SE( "foo" )  )
	// WITH THESE CONSTRUCTORS, THE SAME CONSTRUCTION CAN LOOK LIKE THIS:
	// BE( "!", "foo" )
	//
	Exprs( void ) { }
	Exprs( int integer );
	Exprs( double real );
	Exprs( const std::string &str );
	Exprs( const char *str );

	Exprs( const ExprsBase &exprsBase ) : _exprsBaseSP( exprsBase.clone() ) { }

	virtual std::string getClassName( void ) const { return _exprsBaseSP->getClassName(); }
	virtual ExprsBaseSP clone( void ) const { return _exprsBaseSP->clone(); }

	operator ExprsBase & ( void ) const { return *_exprsBaseSP; }

	template< typename EXPRS >
	boost::shared_ptr< EXPRS > dynamic_exprs_cast( void ) {
		return boost::dynamic_pointer_cast< EXPRS, ExprsBase >( _exprsBaseSP );
	}

	virtual void buildUdmAux( const Udm::Object &parent, const Uml::CompositionChildRole &childRole ) const {
		if ( _exprsBaseSP != 0 ) _exprsBaseSP->buildUdm( parent, childRole );
	}

	virtual bool isBound( void ) { return _exprsBaseSP != 0; }

	void setExprs( const ExprsBase &exprsBase ) { _exprsBaseSP = exprsBase.clone(); }

	bool isEmpty( void ) const { return _exprsBaseSP == 0; }
};

class ExprsProxy : public ExprsBase {
private:
	typedef unify_ptr< Exprs > UnifyExprs;

	mutable UnifyExprs _unifyExprs;

public:
	static std::string getName( void ) { return "ExprsProxy"; }

	ExprsProxy( void ) { }

	virtual std::string getClassName( void ) const { return getName(); }
	virtual ExprsBaseSP clone( void ) const { return ExprsBaseSP(  new ExprsProxy( *this )  ); }
	virtual void buildUdmAux( const Udm::Object &parent, const Uml::CompositionChildRole &childRole ) const {
		_unifyExprs.getObject().buildUdmAux( parent, childRole );
	}
	virtual bool isBound( void ) { return _unifyExprs.getObject().isBound(); }

	void setExprs( const ExprsBase &exprsBase ) { _unifyExprs.getObject().setExprs( exprsBase ); }
	void unify( ExprsProxy &rhs ) { _unifyExprs.unify( rhs._unifyExprs ); }
};


//
// INTEGER EXPRESSION
//
class IE : public ExprsBase {
private:
	int _integer;

public:
	static std::string getName( void ) { return "IE"; }

	IE( int integer ) : _integer( integer ) { }
	
	virtual std::string getClassName( void ) const { return getName(); }
	virtual ExprsBaseSP clone( void ) const { return ExprsBaseSP(  new IE( *this )  ); }
	virtual void buildUdmAux( const Udm::Object &parent, const Uml::CompositionChildRole &childRole ) const;
};


//
// DOUBLE EXPRESSION
//
class DE : public ExprsBase {
private:
	double _real;

public:
	static std::string getName( void ) { return "DE"; }

	DE( double real ) : _real( real ) { }

	virtual std::string getClassName( void ) const { return getName(); }
	virtual ExprsBaseSP clone( void ) const { return ExprsBaseSP(  new DE( *this )  ); }
	virtual void buildUdmAux( const Udm::Object &parent, const Uml::CompositionChildRole &childRole ) const;
};


//
// STRING EXPRESSION
//
class SE : public ExprsBase {
private:
	std::string _str;

public:
	static std::string getName( void ) { return "SE"; }

	SE( const std::string &str ) : _str( str ) { }

	virtual std::string getClassName( void ) const { return getName(); }
	virtual ExprsBaseSP clone( void ) const { return ExprsBaseSP(  new SE( *this )  ); }
	virtual void buildUdmAux( const Udm::Object &parent, const Uml::CompositionChildRole &childRole ) const;
};


class SymbolTable {
private:
	typedef std::list< SFC::LocalVar > LocalVarList;
	typedef std::map< std::string, LocalVarList > NameStackMap;

	static NameStackMap &getNameStackMap( void ) {
		static NameStackMap nameStackMap;
		return nameStackMap;
	}

public:
	static void pushLocalVar( SFC::LocalVar localVar ) {
		getNameStackMap()[ localVar.name() ].push_back( localVar );
	}

	static SFC::LocalVar getLocalVar( const std::string &name ) {
		return getNameStackMap()[ name ].back();
	}

	static void popLocalVar( const std::string &name ) {
		getNameStackMap()[ name ].pop_back();
	}
};

//
// VARIABLE EXPRESSION
//
class VE : public ExprsBase {
private:
	SFC::ArgDeclBase _var;
	int _i, _j;
	SFC::ArgDeclBase _index1;
	SFC::ArgDeclBase _index2;

public:
	static std::string getName( void ) { return "VE"; }

	VE( void ) { }

	VE( const std::string &name, int i = -1, int j = -1 ) :
	 _var(  SymbolTable::getLocalVar( name )  ), _i(i), _j(j) { }

	VE( SFC::ArgDeclBase var, int i = -1, int j = -1 ) :
	 _var( var ), _i( i ), _j( j ) { }

	VE( SFC::ArgDeclBase var, SFC::ArgDeclBase index1, int j = -1 ) :
	 _var( var ), _i( -1 ), _j( j ), _index1( index1 ) { }

	VE( SFC::ArgDeclBase var, int i, SFC::ArgDeclBase index2 ) :
	 _var( var ), _i( i ), _j( -1 ), _index2( index2 ) { }

	VE( SFC::ArgDeclBase var, SFC::ArgDeclBase index1, SFC::ArgDeclBase index2 ) :
	 _var( var ), _i( -1 ), _j( -1 ), _index1( index1 ), _index2( index2 ) { }

	virtual std::string getClassName( void ) const { return getName(); }
	virtual ExprsBaseSP clone( void ) const { return ExprsBaseSP(  new VE( *this )  ); }
	virtual void buildUdmAux( const Udm::Object &parent, const Uml::CompositionChildRole &childRole ) const;

	SFC::ArgDeclBase getArgDeclBase( void ) { return _var; }

	static void addIndex( Udm::Object &cparent, Uml::CompositionChildRole &crole, SFC::ArgDeclBase index, int num );
};


//
// FUNCTION CALL EXPRESSION
//
class FE : public ExprsBase {
private:
	SFC::Function _function;
	std::string _fname;
	ExprsBaseSPList _argVals;

public:
	static std::string getName( void ) { return "FE"; }

	class ArgSorter {
	public:
		bool operator()( SFC::Arg arg1, SFC::Arg arg2 ) {
			return static_cast< int >( arg1.argIndex() ) < static_cast< int >( arg2.argIndex() );
		}
	};
	typedef std::set< SFC::Arg, ArgSorter > ArgSet;

	FE( void ) { }
	FE( const std::string &fname ) : _fname( fname ) { }
	FE( SFC::Function sfcFunction ) : _function( sfcFunction ) { }

	virtual void addArgVal(const ExprsBase &argVal) { _argVals.push_back(argVal.clone()); }
	virtual void addArgVal(const Exprs &argVal) { _argVals.push_back(argVal.clone()); }

	virtual std::string getClassName( void ) const { return getName(); }
	virtual ExprsBaseSP clone( void ) const { return ExprsBaseSP(  new FE( *this )  ); }
	virtual void buildUdmAux( const Udm::Object &parent, const Uml::CompositionChildRole &childRole ) const;
};

//
// BINARY EXPRESSION
//
class BE : public ExprsBase {
private:
	ExprsBaseSP _leftexpr;
	std::string _op;
	ExprsBaseSP _rightexpr;

public:
	static std::string getName( void ) { return "BE"; }

	BE( const ExprsBase &leftexpr, const std::string &op, const ExprsBase &rightexpr ) :
	 _leftexpr( leftexpr.clone() ), _op( op ), _rightexpr( rightexpr.clone() ) { }

	BE( const Exprs &leftexpr, const std::string &op, const ExprsBase &rightexpr ) :
	 _leftexpr( leftexpr.clone() ), _op( op ), _rightexpr( rightexpr.clone() ) { }

	BE( const ExprsBase &leftexpr, const std::string &op, const Exprs &rightexpr ) :
	 _leftexpr( leftexpr.clone() ), _op( op ), _rightexpr( rightexpr.clone() ) { }

	BE( const Exprs &leftexpr, const std::string &op, const Exprs &rightexpr ) :
	 _leftexpr( leftexpr.clone() ), _op( op ), _rightexpr( rightexpr.clone() ) { }

	virtual std::string getClassName( void ) const { return getName(); }
	virtual ExprsBaseSP clone( void ) const { return ExprsBaseSP(  new BE( *this )  ); }

	virtual void buildUdmAux( const Udm::Object &parent, const Uml::CompositionChildRole &childRole ) const;
};


//
// UNARY EXPRESSION
//
class UE : public ExprsBase {
private:
	std::string _op;
	ExprsBaseSP _subexpr;

public:
	static std::string getName( void ) { return "UE"; }

	UE( const std::string &op, const ExprsBase &subexpr ) :
	 _op( op ), _subexpr( subexpr.clone() ) { }

	UE( const std::string &op, const Exprs &subexpr ) :
	 _op( op ), _subexpr( subexpr.clone() ) { }

	virtual std::string getClassName( void ) const { return getName(); }
	virtual ExprsBaseSP clone( void ) const { return ExprsBaseSP(  new UE( *this )  ); }

	virtual void buildUdmAux( const Udm::Object &parent, const Uml::CompositionChildRole &childRole ) const;
};

 
class SumExprs : public BE {
private:
	BE buildBE( const StringList &varNameList, const StringList &signList ) const;

public:
	static std::string getName( void ) { return "SumExprs"; }

	SumExprs( const StringList &varNameList, const StringList &signList )
	 : BE(  buildBE( varNameList, signList )  ) { }

	virtual std::string getClassName( void ) const { return getName(); }

};

#endif
