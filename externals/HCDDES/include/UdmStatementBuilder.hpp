#ifndef ISIS_UDM_STATEMENT_BUILDER
#define ISIS_UDM_STATEMENT_BUILDER

#include "UdmExprsBuilder.hpp"

//
// SMART POINTER TO Statement CLASS
//
class StatementBase;
typedef boost::shared_ptr< StatementBase > StatementBaseSP;

class StatementBase {
public:
	virtual StatementBaseSP clone( void ) const = 0;

	virtual void buildUdm( const SFC::CompoundStatement &parent, const Uml::CompositionChildRole &childRole ) const = 0;
};

class Statement : public StatementBase {
private:
	StatementBaseSP _statementBaseSP;

public:
	Statement( void ) : _statementBaseSP(  static_cast< StatementBase * >( 0 )  ) { }
	Statement( const StatementBase &statementBase ) : _statementBaseSP( statementBase.clone() ) { }
	Statement( const ExprsBase &exprsBase );

	virtual StatementBaseSP clone( void ) const {
		return _statementBaseSP == 0 ? _statementBaseSP : _statementBaseSP->clone();
	}

	virtual void buildUdm( const SFC::CompoundStatement &parent, const Uml::CompositionChildRole &childRole ) const {
		if ( _statementBaseSP != 0 ) _statementBaseSP->buildUdm( parent, childRole );
	}

	bool isEmpty( void ) { return _statementBaseSP == 0; }

	Statement combine( Statement &rhs );
};

class US : public StatementBase {
private:
	ExprsBaseSP _codeexpr;

public:
	US( const ExprsBase &exprsBase ) : _codeexpr( exprsBase.clone() ) { }

	virtual StatementBaseSP clone( void ) const { return StatementBaseSP(  new US( *this )  ); }

	virtual void buildUdm( const SFC::CompoundStatement &parent, const Uml::CompositionChildRole &childRole ) const;
};

class RS : public StatementBase {
private:
	ExprsBaseSP _retexpr;

public:
	RS( const ExprsBase &exprsBase ) : _retexpr( exprsBase.clone() ) { }
	RS( void ) { }

	virtual StatementBaseSP clone( void ) const { return StatementBaseSP(  new RS( *this )  ); }

	virtual void buildUdm( const SFC::CompoundStatement &parent, const Uml::CompositionChildRole &childRole ) const;
};

class LS : public StatementBase {
private:
	std::string _varName;
	int _size;
	SFC::DT _dt;

public:
	LS( const std::string &varName ) : _varName( varName ), _size( 1 ) { }
	LS( const std::string &varName, int size ) : _varName( varName ), _size( size ) { }
	LS( const std::string &varName, const SFC::DT& dt ) : _varName( varName ), _size( 0 ), _dt( dt ) { }

	virtual StatementBaseSP clone( void ) const { return StatementBaseSP(  new LS( *this )  ); }

	virtual void buildUdm( const SFC::CompoundStatement &parent, const Uml::CompositionChildRole &childRole ) const;
};

/** Sequential Statement */
class SS : public StatementBase {
private:
	StatementBaseSP _statementBase1;
	StatementBaseSP _statementBase2;

public:
	SS()
	 : _statementBase1( ), _statementBase2( ) { }

	SS( const StatementBase &statementBase1, const StatementBase &statementBase2 )
	 : _statementBase1( statementBase1.clone() ), _statementBase2( statementBase2.clone() ) { }

	SS( const Statement &statement1, const StatementBase &statementBase2 )
	 : _statementBase1( statement1.clone() ), _statementBase2( statementBase2.clone() ) { }

	SS( const StatementBase &statementBase1, const Statement &statement2 )
	 : _statementBase1( statementBase1.clone() ), _statementBase2( statement2.clone() ) { }

	SS( const Statement &statement1, const Statement &statement2 )
	 : _statementBase1( statement1.clone() ), _statementBase2( statement2.clone() ) { }

	virtual StatementBaseSP clone( void ) const { return StatementBaseSP(  new SS( *this )  ); }

	virtual void buildUdm( const SFC::CompoundStatement &parent, const Uml::CompositionChildRole &childRole ) const;
};

class PopVars {
public:
	void operator()( const SFC::LocalVar &localVar ) {
		SymbolTable::popLocalVar( localVar.name() );
	}
};

/** Compound Statement */
class CS : public StatementBase {
private:
	typedef std::set< SFC::LocalVar > LocalVarSet;
	StatementBaseSP _statementBase;

protected:
	CS( const StatementBase &statementBase ) : _statementBase( statementBase.clone() ) { }

	void buildUdmCS( const SFC::CompoundStatement &compoundStatement ) const;

public:
	static int incStatementCount( SFC::CompoundStatement compoundStatement ) {
		int retval = (int)compoundStatement.statementCount();
		compoundStatement.statementCount() = retval + 1;
		return retval;
	}
};

class IS : public CS {
private:
	ExprsBaseSP _condition;

public:
	IS( const Exprs &exprs, const StatementBase &statementBase )
	 : CS( statementBase ), _condition( exprs.clone() ) { }
	IS( const Exprs &exprs, const Statement &statement )
	 : CS( statement ), _condition( exprs.clone() ) { }

	virtual StatementBaseSP clone( void ) const { return StatementBaseSP(  new IS( *this )  ); }

	virtual void buildUdm( const SFC::CompoundStatement &parent, const Uml::CompositionChildRole &childRole ) const;
};

class CB : public CS {
private:
	ExprsBaseSP _condition;

public:
	CB( const Exprs &exprs, const StatementBase &statementBase )
	 : CS( statementBase ), _condition( exprs.clone() ) { }
	CB( const Exprs &exprs, const Statement &statement )
	 : CS( statement ), _condition( exprs.clone() ) { }

	virtual StatementBaseSP clone( void ) const { return StatementBaseSP(  new CB( *this )  ); }

	virtual void buildUdm( const SFC::CompoundStatement &parent, const Uml::CompositionChildRole &childRole ) const;
};

/** CONDITIONAL GROUP */
class CG : public CS {
public:
	CG( const StatementBase &statementBase ) : CS( statementBase ) { }
	CG( const Statement &statement ) : CS( statement ) { }

	virtual StatementBaseSP clone( void ) const { return StatementBaseSP(  new CG( *this )  ); }

	virtual void buildUdm( const SFC::CompoundStatement &parent, const Uml::CompositionChildRole &childRole ) const;
};

class DFStatement : public SS {
private:
	SS buildSS(
	 const StringList &inVarNameList,  const DoubleList &inCoefList,
	 const StringList &outVarNameList, const DoubleList &outCoefList
	);

public:
	DFStatement(
	 const StringList &inVarNameList,  const DoubleList &inCoefList,
	 const StringList &outVarNameList, const DoubleList &outCoefList
	) : SS(  buildSS( inVarNameList, inCoefList, outVarNameList, outCoefList )  ) { }

		
};


#endif
