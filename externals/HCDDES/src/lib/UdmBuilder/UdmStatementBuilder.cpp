#include "UdmStatementBuilder.hpp"

Statement::Statement( const ExprsBase &exprsBase ) : _statementBaseSP(  new US( exprsBase )  ) { }

Statement Statement::combine( Statement &rhs ) {
	if ( !isEmpty() ) {
		if ( !rhs.isEmpty() ) {
			return Statement(  SS( *this, rhs )  );
		} else {
			return *this;
		}
	}
	return rhs;
}

void US::buildUdm( const SFC::CompoundStatement &parent, const Uml::CompositionChildRole &childRole ) const {
	SFC::UserCode userCode = SFC::UserCode::Create( parent, childRole );
	if ( childRole == SFC::CompoundStatement::meta_stmnt ) userCode.statementIndex() = CS::incStatementCount( parent );
	_codeexpr->buildUdm( userCode, SFC::UserCode::meta_codeexpr );
}

void RS::buildUdm( const SFC::CompoundStatement &parent, const Uml::CompositionChildRole &childRole ) const {
	SFC::Return sfcReturn = SFC::Return::Create( parent, childRole );
	sfcReturn.statementIndex() = CS::incStatementCount( parent );
	if ( _retexpr != 0 ) _retexpr->buildUdm( sfcReturn, SFC::Return::meta_retexpr );
}

void LS::buildUdm( const SFC::CompoundStatement &parent, const Uml::CompositionChildRole &childRole ) const {
	SFC::LocalVar localVar = SFC::LocalVar::Create( parent, childRole );
	localVar.statementIndex() = CS::incStatementCount( parent );
	localVar.name() = _varName;
	localVar.size() = _size;
	localVar.dt() = _dt;
	SymbolTable::pushLocalVar( localVar );
}

void SS::buildUdm( const SFC::CompoundStatement &parent, const Uml::CompositionChildRole &childRole ) const {
	if ( _statementBase1 )
		_statementBase1->buildUdm( parent, SFC::CompoundStatement::meta_stmnt );
	if ( _statementBase2 )
		_statementBase2->buildUdm( parent, SFC::CompoundStatement::meta_stmnt );		
}

void CS::buildUdmCS( const SFC::CompoundStatement &compoundStatement ) const {
	compoundStatement.statementCount() = 0;

	_statementBase->buildUdm( compoundStatement, SFC::CompoundStatement::meta_stmnt );

	LocalVarSet localVarSet = compoundStatement.LocalVar_kind_children();
	std::for_each( localVarSet.begin(), localVarSet.end(), PopVars() );
}

void IS::buildUdm( const SFC::CompoundStatement &parent, const Uml::CompositionChildRole &childRole ) const {

	SFC::IterativeBlock iterativeBlock = SFC::IterativeBlock::Create( parent, childRole );
	iterativeBlock.statementIndex() = CS::incStatementCount( parent );

	SFC::UserCode userCode = SFC::UserCode::Create( iterativeBlock, SFC::IterativeBlock::meta_cond );
	_condition->buildUdm( userCode, SFC::UserCode::meta_codeexpr );

	buildUdmCS( iterativeBlock );
}

void CB::buildUdm( const SFC::CompoundStatement &parent, const Uml::CompositionChildRole &childRole ) const {

	SFC::ConditionalBlock conditionalBlock = SFC::ConditionalBlock::Create( parent, childRole );
	conditionalBlock.statementIndex() = CS::incStatementCount( parent );

	SFC::UserCode userCode = SFC::UserCode::Create( conditionalBlock, SFC::ConditionalBlock::meta_cond );
	_condition->buildUdm( userCode, SFC::UserCode::meta_codeexpr );

	buildUdmCS( conditionalBlock );
}

void CG::buildUdm( const SFC::CompoundStatement &parent, const Uml::CompositionChildRole &childRole ) const {

	SFC::ConditionalGroup conditionalGroup = SFC::ConditionalGroup::Create( parent, childRole );
	conditionalGroup.statementIndex() = CS::incStatementCount( parent );

	buildUdmCS( conditionalGroup );
}


SS DFStatement::buildSS(
 const StringList &inVarNameList,  const DoubleList &inCoefList,
 const StringList &outVarNameList, const DoubleList &outCoefList
) {
	//
	// FIRST, BUILD THE FILTER EQUATION
	//
	StringList::const_iterator inVarNameItr  = inVarNameList.begin();
	DoubleList::const_iterator inCoefItr     = inCoefList.begin();
	StringList::const_iterator outVarNameItr = outVarNameList.begin();
	DoubleList::const_iterator outCoefItr    = outCoefList.begin();

	BE be(  0, "+", BE( *inCoefItr, "*", *inVarNameItr )  );
	while( ++inVarNameItr != inVarNameList.end() ) {
		(void)++inCoefItr;
		be = BE(  be, "+", BE( *inCoefItr, "*", *inVarNameItr )  );
	}
	while( ++outVarNameItr != outVarNameList.end() ) {
		(void)++outCoefItr;
		be = BE(  be, "-", BE( *outCoefItr, "*", *outVarNameItr )  );
	}

	be = BE( be, "/", outCoefList.front() );
	be = BE( outVarNameList.front(), "=", be );

	//
	// SHIFT THE VARIABLES
	//
	SS ss(  IE( 1 ), be  );

	(void)--inVarNameItr;
	while( inVarNameItr != inVarNameList.begin() ) {
		StringList::const_iterator nxtInVarNameItr = inVarNameItr;
		(void)--nxtInVarNameItr;

		ss = SS(  ss, BE( *inVarNameItr, "=", *nxtInVarNameItr )  );
		inVarNameItr = nxtInVarNameItr;
	}

	(void)--outVarNameItr;
	while( outVarNameItr != outVarNameList.begin() ) {
		StringList::const_iterator nxtOutVarNameItr = outVarNameItr;
		(void)--nxtOutVarNameItr;

		ss = SS(  ss, BE( *outVarNameItr, "=", *nxtOutVarNameItr )  );
		outVarNameItr = nxtOutVarNameItr;
	}

	return ss;
}
