#include "SFManager.hpp"
#include "mfile2SFC.hpp"
#include <boost/regex.hpp>
#include "RegexCommon.hpp"
#include "Uml.h"

std::string SFManager::trim( const std::string &rhs ) {
	std::string retval = rhs;

	std::string::iterator stsItr = retval.begin();
	while( stsItr != retval.end() ) {
		if (  isspace( *stsItr )  ) stsItr = retval.erase( stsItr );
		else                        break;
	}
	stsItr = retval.end();
	while( stsItr != retval.begin() ) {
		(void)--stsItr;
		if (  isspace( *stsItr )  ) stsItr = retval.erase( stsItr );
		else                        break;
	}
	return retval;
}

std::string SFManager::filterComments( std::string &input ) {

	int noUnbalancedLeftBrackets = 0;
	std::string comments;
	std::string condition;

	boost::match_results<std::string::const_iterator> results;
	while(  regex_search( input, results, RegexCommon::getCommentRegex() )  ) {
		condition += results.prefix();

		std::string comment( results[0].first, results[0].second );
		if ( comment[0] == '%' ) comment = comment.substr( 1, comment.size() - 1 );
		else if ( comment[1] == '/' ) comment = comment.substr( 2, comment.size() - 2 );
		else comment = comment.substr( 2, comment.size() - 4 );

		comments += comment + "\n";
		input = results.suffix();
	}
	condition += input;
	condition = SFManager::trim( condition );

	input = condition;

	comments = SFManager::trim( comments );
	return comments;
}

std::string SFManager::AddTypedEntityScope( SFC::TypedEntity typedEntity ) {

	std::string scope( typedEntity.scope() );
	if ( scope == "" ) scope = getContainingFunctionScope( typedEntity );

	std::string teName;
	if ( !scope.empty() ) teName = scope + '/';
	teName += static_cast< std::string >( typedEntity.name() );
	getVarResolve().addScopePath(  VarResolve::getScopePath( teName )  );

	std::string resVarName = getVarResolve().resolveScope(
	 VarResolve::getScopePath( teName ),
	 VarResolve::getScopePath( scope )
	);
	typedEntity.name() = resVarName;

	if (  Udm::IsDerivedFrom( typedEntity.type(), SFC::ArgDeclBase::meta )  ) {
		SFC::ArgDeclBase argDeclBase = SFC::ArgDeclBase::Cast( typedEntity );
		getSFCSymbolTable().push( argDeclBase.name(), argDeclBase );
	} else if ( typedEntity.type() == SFC::Function::meta ) {
		SFC::Function sfcFunction = SFC::Function::Cast( typedEntity );
		getFunctionHash().insert(  std::make_pair( sfcFunction.name(), sfcFunction )  );
	}

	return resVarName;
}


std::string SFManager::getContainingFunctionScope( Udm::Object object ) {

	while(  !Udm::IsDerivedFrom( object.type(), SFC::Program::meta )  ) {
		if (  Udm::IsDerivedFrom( object.type(), SFC::Function::meta )  ) {
			return SFC::Function::Cast( object ).scope();
		}

		object = object.GetParent();
	}

	return "";
}


std::string SFManager::ResolveReferences( SFC::CompoundStatement compoundStatement, const std::string &expression ) {

	boost::match_results< std::string::const_iterator > results;

	std::string newExpression = expression;
	std::string::const_iterator start = newExpression.begin();
	std::string::const_iterator searchStart = start;
	std::string::const_iterator finish = newExpression.end();

	std::string functionScope = getContainingFunctionScope( compoundStatement );

	while(  regex_search( searchStart, finish, results, RegexCommon::getDottedIdentifierRegex() )  ) {
		std::string match( results[0].first, results[0].second );

		std::string resVarName = getVarResolve().resolveScope(
		 VarResolve::getScopePath( match, "." ),
		 VarResolve::getScopePath( functionScope )
		);
		if ( resVarName.empty() ) resVarName = match;

		std::string::difference_type searchStartInt = ( results[0].first - start ) + resVarName.size();
		newExpression = std::string( start, results[0].first ) + resVarName + std::string( results[1].second, finish );

		start = newExpression.begin();
		searchStart = start + searchStartInt;
		finish = newExpression.end();
	}

	return newExpression;
}


void SFManager::mstat2SFC( SFC::CompoundStatement compoundStatement, const std::string &statement ) {

	std::string newStatement = ResolveReferences( compoundStatement, statement );
	::mstat2SFC( compoundStatement, newStatement, true, true, getFunctionHash() );
}

void SFManager::mexpr2SFCCondition( SFC::CompoundStatement compoundStatement, const std::string &expression ) {

	std::string newExpression = ResolveReferences( compoundStatement, expression );
	::mexpr2SFCCondition( compoundStatement, newExpression, true, true );
}


void SFManager::initVarAux(
 SFC::CompoundStatement compoundStatement, SFC::LocalVar localVar, const std::string &initStr
) {
	SFC::DT dt = localVar.dt();
	if ( dt.type() == SFC::Struct::meta ) {
		SFC::Struct sfcStruct = SFC::Struct::Cast( dt ) ;
		LocalVarSet localVarSet = sfcStruct.LocalVar_kind_children();
		for( LocalVarSet::iterator lvsItr = localVarSet.begin() ; lvsItr != localVarSet.end() ; (void)++lvsItr ) {
			initVarAux(  compoundStatement, *lvsItr, initStr + "." + static_cast< std::string >( lvsItr->name() )  );
		}
		return;
	}

	::mstat2SFC( compoundStatement, initStr + " = 0", true, true );
}
void SFManager::initVar( SFC::CompoundStatement compoundStatement, SFC::Var var ) {
	Udm::Object object = Udm::Object::Cast( var );
	if ( object.type() != SFC::LocalVar::meta ) return;
	SFC::LocalVar localVar = SFC::LocalVar::Cast( var );

	SFC::DT dt = localVar.dt();
	std::string name = localVar.name();
	if ( dt.type() == SFC::Struct::meta ) {
		initVarAux( compoundStatement, localVar, name );
		return;
	}

	std::string initial = localVar.initial();
	if ( initial.empty() ) initial = "0";
	::mstat2SFC( compoundStatement, name + " = " + initial, true, true );
}