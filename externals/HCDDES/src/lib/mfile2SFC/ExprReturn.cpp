#include "ExprReturn.hpp"
#include "LitMatrixExprReturn.hpp"
#include "mfile2SFC.hpp"

DIVIDE_VISITOR_DEFINITION( ExprReturn )

/**/
/* ExprReturn */
/**/
ExprReturnSP ExprReturn::combine(
 const std::string &op,
 double value,
 bool func
) {
	ExprReturnSP exprReturnSP = LitMatrixExprReturn::create( getBlock(), value );
	return combine( op, exprReturnSP, func );
}


ExprReturnSP ExprReturn::divide( ExprReturn &exprReturn ) {

	SFC::DT origDT = exprReturn.getDT();

	SFCTypesManager::DimensionVector dimensionVector = SFCTypesManager::getDimensions( origDT );
	if ( dimensionVector.empty() ) return combine( "/", exprReturn );

	int dimension = dimensionVector.back();

	SFC::LocalVar varCopy = SFCManager::createUniqueLocalVar( getBlock(), "copy" );
	varCopy.dt() = origDT;
	std::string copyName = varCopy.name();
	getSFCSymbolTable().push( copyName, varCopy );

	ExprReturnSP exprReturnSP3 = VarExprReturn::create( getBlock(), varCopy, origDT );
	exprReturnSP3->combine( "=", exprReturn )->collapse();

	SFC::LocalVar varIdentity = SFCManager::createUniqueLocalVar( getBlock(), "identity" );
	varIdentity.dt() = origDT;
	std::string identityName = varIdentity.name();
	getSFCSymbolTable().push( identityName, varIdentity );

	ExprReturnSP exprReturnSP4 = VarExprReturn::create( getBlock(), varIdentity, origDT );

	SFC::LocalVar index1 = SFCManager::createUniqueLocalVar( getBlock() );
	std::string index1Name = index1.name();
	getSFCSymbolTable().push( index1Name, index1 );
	index1.dt() = SFCTypesManager::getSingleton().getBasicType( "int" );

	SFC::LocalVar index2 = SFCManager::createUniqueLocalVar( getBlock() );
	std::string index2Name = index2.name();
	getSFCSymbolTable().push( index2Name, index2 );
	index2.dt() = SFCTypesManager::getSingleton().getBasicType( "int" );

	SFC::LocalVar varTemp = SFCManager::createUniqueLocalVar( getBlock(), "temp" );
	std::string tempName = varTemp.name();
	getSFCSymbolTable().push( tempName, varTemp );
	varTemp.dt() = SFCTypesManager::getSingleton().getBasicType( "double" );

	std::string program =
	 identityName + " = 0\n" +
	 index1Name + " = 0\n" +
	 "while " + index1Name + " < " + boost::lexical_cast< std::string >( dimension ) + "\n" +
	 "  " + identityName + "( " + index1Name + ", " + index1Name + " ) = 1\n" +
	 "  " + index1Name + " = " + index1Name + " + 1\n"
	 "end\n\n" +

	 index1Name + " = 0\n" +
	 "while " + index1Name + " < " + boost::lexical_cast< std::string >( dimension ) + "\n" +
	 "  " + tempName + " = " + copyName + "( " + index1Name + ", " + index1Name + " )\n" +
	 "  " + identityName + "( " + index1Name + ", : ) = " + identityName + "(" + index1Name + ", : ) ./ " +
	  tempName + "\n" +
	 "  " + copyName + "( " + index1Name + ", : ) = " + copyName + "(" + index1Name + ", : ) ./ " +
	  tempName + "\n" +
	 "  " + index2Name + " = 0\n" +
	 "  while " + index2Name + " < " + boost::lexical_cast< std::string >( dimension ) + "\n" +
	 "    if " + index1Name + " ~= " + index2Name + "\n" +
	 "      " + tempName + " = " + copyName + "( " + index2Name + ", " + index1Name + " )\n" +
	 "      " + identityName + "( " + index2Name + ", : ) = " + identityName + "(" + index2Name + ", : ) - " +
	  identityName + "( " + index1Name + ", : ) * " + tempName + "\n" +
	 "      " + copyName + "( " + index2Name + ", : ) = " + copyName + "(" + index2Name + ", : ) - " +
	  copyName + "( " + index1Name + ", : ) * " + tempName + "\n" +
	 "    end\n" +
	 "    " + index2Name + " = " + index2Name + " + 1\n"
	 "  end\n" +
	 "  " + index1Name + " = " + index1Name + " + 1\n"
	 "end\n";

	mstat2SFC( getBlock(), program, true );

	return multiply( exprReturnSP4 );
}