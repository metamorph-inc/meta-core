#include "OpPred.hpp"


OpPred::PrecedenceTable::PrecedenceTable( void ) {
	_precedenceHash[ "binary;" ] = 0;

	_precedenceHash[ "binary=" ] = 1;	_precedenceHash[ "binary|=" ] = 1;	_precedenceHash[ "binary^=" ] = 1;
	_precedenceHash[ "binary&=" ] = 1;	_precedenceHash[ "binary<<=" ] = 1;	_precedenceHash[ "binary>>=" ] = 1;
	_precedenceHash[ "binary+=" ] = 1;	_precedenceHash[ "binary-=" ] = 1;	_precedenceHash[ "binary*=" ] = 1;
	_precedenceHash[ "binary/=" ] = 1;	_precedenceHash[ "binary%=" ] = 1;

	_precedenceHash[ "binary||" ] = 2;
	_precedenceHash[ "binary&&" ] = 3;
	_precedenceHash[ "binary|" ] = 4;
	_precedenceHash[ "binary^" ] = 5;
	_precedenceHash[ "binary&" ] = 6;

	_precedenceHash[ "binary==" ] = 7;	_precedenceHash[ "binary!=" ] = 7;
	_precedenceHash[ "binary<" ] = 8;	_precedenceHash[ "binary<=" ] = 8;
	_precedenceHash[ "binary>" ] = 8;	_precedenceHash[ "binary>=" ] = 8;
	_precedenceHash[ "binary<<" ] = 9;	_precedenceHash[ "binary>>" ] = 9;
	_precedenceHash[ "binary+" ] = 10;	_precedenceHash[ "binary-" ] = 10;
	_precedenceHash[ "binary*" ] = 11;	_precedenceHash[ "binary/" ] = 11;	_precedenceHash[ "binary%" ] = 11;

	_precedenceHash[ "binary." ] = 14;	_precedenceHash[ "binary[" ] = 14;

	_precedenceHash[ "unary+" ] = 13;	_precedenceHash[ "unary-" ] = 13;	_precedenceHash[ "unary--" ] = 13;
	_precedenceHash[ "unary++" ] = 13;	_precedenceHash[ "unary~" ] = 13;	_precedenceHash[ "unary!" ] = 13;
	_precedenceHash[ "unary&" ] = 13;   _precedenceHash[ "unary*" ] = 13;

	_precedenceHash[ "unarypost--" ] = 14;
	_precedenceHash[ "unarypost++" ] = 14;

}

int OpPred::getOperatorPrecedence( SFC::Exprs exprs, bool derefArg ) {

	int retval = 99;

	if (  Udm::IsDerivedFrom( exprs.type(), SFC::BinaryExprs::meta )  ) {

		retval = getBinaryPrecedence( SFC::BinaryExprs::Cast( exprs ).op() );

	} else if (  Udm::IsDerivedFrom( exprs.type(), SFC::UnaryExprs::meta )  ) {

		SFC::UnaryExprs unaryExprs = SFC::UnaryExprs::Cast( exprs );

		std::string op = unaryExprs.op();
		retval = op == "" ?
		 getOperatorPrecedence(  SFC::Exprs::Cast( unaryExprs.subexpr() )  ) :
		 getUnaryPrecedence( op );

	} else if (  derefArg && Udm::IsDerivedFrom( exprs.type(), SFC::ArgDeclRef::meta )  ) {

		SFC::ArgDeclRef argDeclRef = SFC::ArgDeclRef::Cast( exprs );
		SFC::ArgDeclBase argDeclBase = SFC::ArgDeclBase::Cast( argDeclRef.argdecl() );
		if (  argDeclBase != Udm::null && argDeclBase.type() == SFC::Arg::meta  ) {
			SFC::Arg arg = SFC::Arg::Cast( argDeclBase );
			if ( arg.ptr() ) retval = getUnaryPrecedence( "*" );
		}
	}

	return retval;
}

