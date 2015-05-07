#include <iostream>
#include "UdmExprsBuilder.hpp"
#include "UdmStatementBuilder.hpp"
#include "Uml.h"

//
// CONSTRUCTORS FOR SYNTAX SIMPLIFICATION
//
void ExprsBase::buildUdm( const Udm::Object &parent, const Uml::CompositionChildRole &childRole ) const {
	if (  Udm::IsDerivedFrom( parent.type(), SFC::CompoundStatement::meta )  ) {
		US us = US( *this );
		us.buildUdm(  SFC::CompoundStatement::Cast( parent ), childRole  );
	} else {
		buildUdmAux( parent, childRole );
	}
}

Exprs::Exprs( int integer ) : _exprsBaseSP(  new IE( integer )  ) { }
Exprs::Exprs( double real ) : _exprsBaseSP(  new DE( real )  ) { }
Exprs::Exprs( const std::string &str ) : _exprsBaseSP(  new VE( str )  ) { }
Exprs::Exprs( const char *str ) : _exprsBaseSP(   new VE(  std::string( str )  )   ) { }


void IE::buildUdmAux( const Udm::Object &parent, const Uml::CompositionChildRole &childRole ) const {
	SFC::Int integer = SFC::Int::Create( parent, childRole );
	integer.val() = _integer;
}

void DE::buildUdmAux( const Udm::Object &parent, const Uml::CompositionChildRole &childRole ) const {
	SFC::Double real = SFC::Double::Create( parent, childRole );
	real.val() = _real;
}


// TODO: Clean up variable expressions (by reusing the other classes)
void VE::buildUdmAux( const Udm::Object &parent, const Uml::CompositionChildRole &childRole ) const {
	Udm::Object cparent = parent;
	Uml::CompositionChildRole crole = childRole;

	addIndex( cparent, crole, _index2, _j );
	addIndex( cparent, crole, _index1, _i );

	SFC::ArgDeclRef argDeclRef = SFC::ArgDeclRef::Create( cparent, crole );

	if ( _var == Udm::null ) std::cerr << "WARNING:  VE HAS UNDEFINED ArgDeclBase" << std::endl;
	argDeclRef.argdecl() = _var;
}

void VE::addIndex( Udm::Object &cparent, Uml::CompositionChildRole &crole, SFC::ArgDeclBase index, int num ) {

	if ( index != Udm::null || num > -1 ) {
		SFC::BinaryExprs binaryExprs = SFC::BinaryExprs::Create( cparent, crole );
		binaryExprs.op() = "[";

		if ( index != Udm::null ) {
			SFC::ArgDeclRef argDeclRef = SFC::ArgDeclRef::Create( binaryExprs, SFC::BinaryExprs::meta_rightexpr );
			argDeclRef.argdecl() = index;
		} else {
			SFC::Int integer = SFC::Int::Create( binaryExprs, SFC::BinaryExprs::meta_rightexpr );
			integer.val() = num;
		}

		cparent = binaryExprs;
		crole = SFC::BinaryExprs::meta_leftexpr;
	}
}


void FE::buildUdmAux( const Udm::Object &parent, const Uml::CompositionChildRole &childRole ) const {
	SFC::FunctionCall fCall = SFC::FunctionCall::Create(parent, childRole);

	if ( _fname.empty() ) {
		if ( _function == Udm::null ) return;

		fCall.callee() = _function;
		ArgSet argSet = _function.Arg_children_sorted( ArgSorter() );

		assert( argSet.size() == _argVals.size() );

		ArgSet::iterator arsItr = argSet.begin();
		ExprsBaseSPList::const_iterator it = _argVals.begin();
		while( arsItr != argSet.end() ) {

			SFC::ArgVal argVal = SFC::ArgVal::Create( fCall );

			argVal.arg() += *arsItr;
			argVal.argIndex() = arsItr->argIndex();

			(*it)->buildUdmAux( argVal, SFC::ArgVal::meta_argexpr );
			++arsItr;
			++it;
		}
		return;
	} 

	fCall.libFuncName() = _fname;
	int idx = 0;
	for (ExprsBaseSPList::const_iterator it = _argVals.begin(); it != _argVals.end(); ++it) {
		SFC::ArgVal argVal = SFC::ArgVal::Create(fCall);
		argVal.argIndex() = idx++;
		(*it)->buildUdmAux(argVal, SFC::ArgVal::meta_argexpr);
	}
}

void SE::buildUdmAux( const Udm::Object &parent, const Uml::CompositionChildRole &childRole ) const {
	SFC::Str str = SFC::Str::Create( parent, childRole );
	str.val() = _str;
}

void BE::buildUdmAux( const Udm::Object &parent, const Uml::CompositionChildRole &childRole ) const {
	if ( _op == "[" && !_rightexpr->isBound() ) {
		_leftexpr->buildUdmAux( parent, childRole );
		return;
	}

	SFC::BinaryExprs binaryExprs = SFC::BinaryExprs::Create( parent, childRole );

	_leftexpr->buildUdmAux( binaryExprs, SFC::BinaryExprs::meta_leftexpr );
	binaryExprs.op() = _op;
	_rightexpr->buildUdmAux( binaryExprs, SFC::BinaryExprs::meta_rightexpr );
}

void UE::buildUdmAux( const Udm::Object &parent, const Uml::CompositionChildRole &childRole ) const {
	SFC::UnaryExprs unaryExprs = SFC::UnaryExprs::Create( parent, childRole );

	unaryExprs.op() = _op;
	_subexpr->buildUdmAux( unaryExprs, SFC::UnaryExprs::meta_subexpr );
}

BE SumExprs::buildBE( const StringList &varNameList, const StringList &signList ) const {
	
	StringList::const_iterator vnlcit = varNameList.begin();
	StringList::const_iterator snlcit = signList.begin();
	BE be( 0, *snlcit, *vnlcit );
	while( ++vnlcit != varNameList.end() ) {
		(void)++snlcit;
		be = BE( be, *snlcit, *vnlcit );
	}
	return be;
}
