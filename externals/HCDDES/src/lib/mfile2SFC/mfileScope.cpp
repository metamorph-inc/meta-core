#include "mfileScope.hpp"

#include <boost/filesystem.hpp>
extern "C" {
#include "MatLabTokens.h"
#include "MatLab.h"
}

#include <iostream>
#include <boost/algorithm/string.hpp>

#include "antlr3.h"


bool MFileScope::isValidMatLabInput( const std::string &matLabExpression ) {

	bool isValid = true;

	try {
		pANTLR3_INPUT_STREAM input = antlr3NewAsciiStringCopyStream(  ( pANTLR3_UINT8 )matLabExpression.c_str(), (ANTLR3_UINT32) matLabExpression.size(), 0  );

		pMatLabTokens lexer = MatLabTokensNew( input );

		pANTLR3_COMMON_TOKEN_STREAM tokens = antlr3CommonTokenStreamSourceNew(  ANTLR3_SIZE_HINT, TOKENSOURCE( lexer )  );

		pMatLab parser = MatLabNew( tokens );

		MatLab_input_return s = parser->input( parser );

		pANTLR3_BASE_RECOGNIZER recognizer = parser->pParser->rec;
		int noSyntaxErrors = recognizer->getNumberOfSyntaxErrors( recognizer );

		if ( noSyntaxErrors != 0 ) {
			std::cerr << "Could not parse:" << std::endl;
			std::cerr << matLabExpression << std::endl << std::endl;
			isValid = false;
		}

	} catch ( ... ) {

		std::cerr << "Could not parse:" << std::endl;
		std::cerr << matLabExpression << std::endl << std::endl;
		isValid = false;
	}

	return isValid;
}

std::string MFileScope::scopeTrans( const std::string &mfile, ScopeSymbolTable &scopeSymbolTable ) {

std::cout << "ScopeTrans\n---------\ninput:\n" << mfile << std::endl;

	if (  boost::trim_copy( mfile ).empty()  ) {
		return std::string();
	}

	_scopeSymbolTablePtr = &scopeSymbolTable;

	std::string outputString;
	bool errorOccurred = true;

	try {
		pANTLR3_INPUT_STREAM input = antlr3NewAsciiStringCopyStream(
		 ( pANTLR3_UINT8 )mfile.c_str(), (ANTLR3_UINT32) mfile.size(), 0
		);

		pMatLabTokens lexer = MatLabTokensNew( input );

		pANTLR3_COMMON_TOKEN_STREAM tokens = antlr3CommonTokenStreamSourceNew(  ANTLR3_SIZE_HINT, TOKENSOURCE( lexer )  );

		pMatLab parser = MatLabNew( tokens );
		MatLab_input_return r = parser->input( parser );

		pANTLR3_BASE_TREE tree = r.tree;

		std::cout << "PARSE TREE:" << std::endl <<
		static_cast< unsigned char * >(  tree->toStringTree( tree )->chars  ) << std::endl << std::endl;

//		std::cout << getTreeText( tree ) << std::endl;

		if (  tree->isNilNode( tree )  ) {
			int noChildren = static_cast< int >(  tree->getChildCount( tree )  );
			for( int ix = 0 ; ix < noChildren ; (void)++ix ) {

				pANTLR3_BASE_TREE child = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, ix )  );
				bool printStatement = true;

				try {

					outputString += statementScopeTrans( child, "" );
					printStatement = false;

				} catch ( std::exception &e ) {

					std::cerr << "The following std::exception occurred:" << std::endl;
					std::cerr << e.what() << std::endl << std::endl;

				} catch ( ... ) {

					std::cerr << "Exception of unknown type caught." << std::endl;

				}

				if ( printStatement ) {
					std::cerr << "Exception caused by: \n" << getDeepTreeText( child ) << std::endl << std::endl;
				}
			}
		} else {
			outputString = statementScopeTrans( tree, "" );
		}

		parser->free( parser );
		tokens->free( tokens );
		lexer->free( lexer );
		input->close( input );

		errorOccurred = false;

	} catch ( std::exception &e ) {

		std::cerr << "The following std::exception occurred:" << std::endl;
		std::cerr << e.what() << std::endl << std::endl;

	} catch ( ... ) {

		std::cerr << "Exception of unknown type caught." << std::endl;

	}

	_scopeSymbolTablePtr = &getDefaultScopeSymbolTable();

std::cout << "output:\n" << outputString << "\n" << std::endl;
	return errorOccurred ? "ERROR" : outputString;
}

std::string MFileScope::statementScopeTrans( pANTLR3_BASE_TREE tree, const std::string &indent ) {

	std::string outputString;

	switch(  tree->getType( tree )  ) {
	case IF:
		{
			outputString += ifScopeTrans( tree, indent );
			break;
		}
	case SWITCH:
		{
			outputString += switchScopeTrans( tree, indent );
			break;
		}
	case WHILE:
		{
			outputString += whileScopeTrans( tree, indent );
			break;
		}
	case FOR:
		{
			outputString += forScopeTrans( tree, indent );
			break;
		}
	case TRY:
		{
			outputString += tryScopeTrans( tree, indent );
			break;
		}
	case BREAK:
		{
			outputString += indent + "break\n";
			break;
		}
	case CONTINUE:
		{
			outputString += indent + "continue\n";
			break;
		}
	case RETURN:
		{
			outputString += indent + "return\n";
			break;
		}
	case COMMENT:
		{
			std::string content = getTreeText( tree );
			outputString += content + "\n";
			break;
		}
	default:
		{
			outputString += indent + exprScopeTrans( tree ) + "\n";
			break;
		}
	}

	return outputString;
}

std::string MFileScope::ifScopeTrans( pANTLR3_BASE_TREE tree, const std::string &indent ) {

	std::string outputString;

	int noChildren = static_cast< int >(  tree->getChildCount( tree )  );
	bool first = true;

	for( int ix = 0 ; ix < noChildren ; (void)++ix ) {
		pANTLR3_BASE_TREE child = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, ix )  );

		outputString += makeIfScopeTrans( child, first, indent );
		first = false;
	}

	outputString += indent + "end\n\n";

	return outputString;
}

std::string MFileScope::makeIfScopeTrans( pANTLR3_BASE_TREE tree, bool first, const std::string &indent ) {

	std::string outputString;

	int noChildren = static_cast< int >(  tree->getChildCount( tree )  );
	int statementStart = 1;

	outputString += indent;

	switch (  tree->getType( tree )  ) {
	case IF:
		{
			pANTLR3_BASE_TREE child = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, 0 )  );
			if ( !first ) {
				outputString += "else";
			}
			outputString += "if " + exprScopeTrans( child ) + "\n";
			break;
		}
	case ELSE:
		{
			outputString += "else\n";
			statementStart = 0;
			break;
		}
	}

	for( int ix = statementStart ; ix < noChildren ; (void)++ix ) {
		pANTLR3_BASE_TREE child = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, ix )  );
		outputString += statementScopeTrans( child, indent + "    " );
	}

	return outputString;
}

std::string MFileScope::switchScopeTrans( pANTLR3_BASE_TREE tree, const std::string &indent ) {

	std::string outputString;

	outputString += indent + "switch ";

	int noChildren = static_cast< int >(  tree->getChildCount( tree )  );

	pANTLR3_BASE_TREE switch_expr = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, 0 )  );
	outputString += exprScopeTrans( switch_expr ) + "\n";

	for( int ix = 1 ; ix < noChildren ; ++ix ) {
		pANTLR3_BASE_TREE child = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, ix )  );

		outputString += caseScopeTrans( child, indent + "    " );
	}

	outputString += "end\n\n";

	return outputString;
}

std::string MFileScope::caseScopeTrans( pANTLR3_BASE_TREE tree, const std::string &indent ) {

	std::string outputString;

	int noChildren = static_cast< int >(  tree->getChildCount( tree )  );
	int statementStart = 1;

	switch (  tree->getType( tree )  ) {
	case CASE:
		{
			outputString += indent + "case ";
			pANTLR3_BASE_TREE child = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, 0 )  );
			pANTLR3_BASE_TREE descendant = child;

			// THERE SHOULD ONLY BE ONE GRANDCHILD -- AN EXPR (EXPRESSION).
			// A CELL-EXPRESSION MEANS SEVERAL CONDITIONS.  OTHERWISE THERE IS ONLY ONE.
			pANTLR3_BASE_TREE grandChild = static_cast< pANTLR3_BASE_TREE >(  child->getChild( child, 0 )  );
			if (  grandChild->getType( grandChild ) == LBRACE  ) {
				descendant = grandChild;
			}

			int noExpressions = static_cast< int >(  descendant->getChildCount( descendant )  );
			if ( noExpressions > 1 ) {
				outputString += "{ ";
			}

			bool first = true;
			for( int ix = 0 ; ix < noExpressions ; ++ix ) {
				if ( first ) {
					first = false;
				} else {
					outputString += ", ";
				}
				pANTLR3_BASE_TREE expression = static_cast< pANTLR3_BASE_TREE >(  descendant->getChild( descendant, ix )  );
				outputString += exprScopeTrans( expression );
			}

			if ( noExpressions > 1 ) {
				outputString += " }";
			}

			outputString += "\n";
			break;
		}
	case OTHERWISE:
		{
			outputString += indent + "otherwise\n";

			statementStart = 0;
			break;
		}
	}

	for( int ix = statementStart ; ix < noChildren ; (void)++ix ) {
		pANTLR3_BASE_TREE child = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, ix )  );
		outputString += statementScopeTrans( child, indent + "    " );
	}

	return outputString;
}

std::string MFileScope::whileScopeTrans( pANTLR3_BASE_TREE tree, const std::string &indent ) {

	std::string outputString;

	outputString += indent + "while ";
	int noChildren = static_cast< int >(  tree->getChildCount( tree )  );

	pANTLR3_BASE_TREE firstChild = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, 0 )  );
	outputString += exprScopeTrans( firstChild ) + "\n";

	for( int ix = 1 ; ix < noChildren ; (void)++ix ) {
		pANTLR3_BASE_TREE child = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, ix )  );
		outputString += statementScopeTrans( child, indent + "    " );
	}

	outputString += "end\n\n";

	return outputString;
}

std::string MFileScope::forScopeTrans( pANTLR3_BASE_TREE tree, const std::string &indent ) {

	std::string outputString;

	outputString += indent + "for ";

	int noChildren = static_cast< int >(  tree->getChildCount( tree )  );

	pANTLR3_BASE_TREE firstChild = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, 0 )  );
	outputString += exprScopeTrans( firstChild ) + " = ";

	pANTLR3_BASE_TREE secondChild = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, 1 )  );
	outputString += exprScopeTrans( secondChild ) + "\n";

	for( int ix = 2 ; ix < noChildren ; (void)++ix ) {
		pANTLR3_BASE_TREE child = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, ix )  );
		outputString += statementScopeTrans( child, indent + "    " );
	}

	outputString += "end\n\n";

	return outputString;
}

std::string MFileScope::tryScopeTrans( pANTLR3_BASE_TREE tree, const std::string &indent ) {

	std::string outputString;

	outputString += indent + "try\n";

	pANTLR3_BASE_TREE firstChild = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, 0 )  );
	int noGrandChildren = static_cast< int >(  firstChild->getChildCount( firstChild )  );
	for( int ix = 0 ; ix < noGrandChildren ; ++ix ) {
		pANTLR3_BASE_TREE grandChild = static_cast< pANTLR3_BASE_TREE >(  firstChild->getChild( firstChild, ix )  );
		outputString += statementScopeTrans( grandChild, indent + "    " );
	}

	int noChildren = static_cast< int >(  tree->getChildCount( tree )  );
	if ( noChildren > 1 ) {
		pANTLR3_BASE_TREE secondChild = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, 1 )  );
		noGrandChildren = static_cast< int >(  secondChild->getChildCount( secondChild )  );
		if ( noGrandChildren != 0 ) {
			outputString += indent + "catch\n";
			for( int ix = 0 ; ix < noGrandChildren ; ++ix ) {
				pANTLR3_BASE_TREE grandChild = static_cast< pANTLR3_BASE_TREE >(  secondChild->getChild( secondChild, ix )  );
				outputString += statementScopeTrans( grandChild, indent + "    " );
			}
		}
	}

	outputString += indent + "end\n\n";
	
	return outputString;
}

std::string MFileScope::exprScopeTrans( pANTLR3_BASE_TREE tree ) {

	std::string outputString;

	std::string text = getTreeText( tree );

	switch(  tree->getType( tree )  ) {
	case ASSIGN:
	case PLASGN:
	case MIASGN:
	case MUASGN:
	case DIASGN:
		{
			pANTLR3_BASE_TREE reference = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, 0 )  );
			pANTLR3_BASE_TREE expr = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, 1 )  );
			
			// IT IS IMPORTANT THAT expr (I.E. THE RHS) MUST BE EVALUATED (I.E. HAVE SUBSTITUTIONS PUT IN) BEFORE THE LHS
			// AS THE LHS MAY REQUIRED A NEWLY SCOPED VARIABLE
			std::string expression = exprScopeTrans( expr );

			switch(  reference->getType( reference )  ) {
			case MATRIX:
				{
					outputString += "[ ";

					int noChildren = static_cast< int >(  reference->getChildCount( reference )  );

					bool first = true;
					for( int ix = 0 ; ix < noChildren ; ++ix ) {

						if ( first ) {
							first = false;
						} else {
							outputString += ", ";
						}
						pANTLR3_BASE_TREE child = static_cast< pANTLR3_BASE_TREE >(  reference->getChild( reference, ix )  );

						outputString += referenceScopeTrans( child, LHS );
					}

					outputString += " ]";
					break;
				}
			default:
				{
					outputString += referenceScopeTrans( reference, LHS );
					break;
				}
			}

			std::string op = getTreeText( tree );
			outputString += " " + op + " " + expression;

			return outputString;
		}

	case PSTINC:
	case PSTDEC:
		{
			std::string op = getTreeText( tree );
			pANTLR3_BASE_TREE reference = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, 0 )  );

			outputString += referenceScopeTrans( reference, POSTOP );
			outputString += op;

			return outputString;
		}

	case AND:
	case OR:
	case LAND:
	case LOR:
	case GTHAN:
	case GTHANE:
	case EQUAL:
	case UNEQUAL:
	case CUNEQUAL:
	case LTHAN:
	case LTHANE:
		{
			std::string op = getTreeText( tree );

			pANTLR3_BASE_TREE expr1 = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, 0 )  );
			pANTLR3_BASE_TREE expr2 = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, 1 )  );

			outputString += exprScopeTrans( expr1 );
			outputString += " " + op + " ";
			outputString += exprScopeTrans( expr2 );

			return outputString;
		}

	case PLUS:
	case MINUS:
		{
			int noChildren = static_cast< int >(  tree->getChildCount( tree )  );
			std::string op = getTreeText( tree );

			if ( noChildren == 1 ) {

				outputString += op;

				pANTLR3_BASE_TREE expr = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, 0 )  );
				outputString += exprScopeTrans( expr );

				return outputString;

			} else {

				pANTLR3_BASE_TREE expr1 = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, 0 )  );
				pANTLR3_BASE_TREE expr2 = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, 1 )  );

				outputString += exprScopeTrans( expr1 );
				outputString += " " + op + " ";
				outputString += exprScopeTrans( expr2 );

				return outputString;
			}
		}

	case COLON:
		{
			int noChildren = static_cast< int >(  tree->getChildCount( tree )  );

			if ( noChildren == 0 ) {

				outputString += ":";

			} else {

				bool first = true;
				for( int ix = 0 ; ix < noChildren ; ++ix ) {
					if ( first ) {
						first = false;
					} else {
						outputString += ":";
					}

					pANTLR3_BASE_TREE expr = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, ix )  );
					outputString += exprScopeTrans( expr );
				}

			}

			return outputString;
		}

	case EMUL:
	case EDIV:
	case ELEFTDIV:
	case EPOWER:
	case POWER:
	case MUL:
	case DIV:
	case LEFTDIV:
		{
			std::string op = getTreeText( tree );

			pANTLR3_BASE_TREE expr1 = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, 0 )  );
			pANTLR3_BASE_TREE expr2 = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, 1 )  );

			outputString += exprScopeTrans( expr1 );
			outputString += " " + op + " ";
			outputString += exprScopeTrans( expr2 );

			return outputString;
		}

	case NOT:
	case CNOT:
		{
			std::string op = getTreeText( tree );

			outputString += op;

			pANTLR3_BASE_TREE expr = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, 0 )  );

			outputString += exprScopeTrans( expr );

			return outputString;
		}

	case CTRANSPOSE:
	case TRANSPOSE:
		{
			std::string op = getTreeText( tree );

			pANTLR3_BASE_TREE expr = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, 0 )  );
			outputString += exprScopeTrans( expr );

			outputString += op;

			return outputString;
		}

	case INTEGER:
		{
			std::string stringVal = getTreeText( tree );

			outputString += stringVal;

			return outputString;
		}

	case DOUBLE:
		{
			std::string stringVal = getTreeText( tree );

			outputString += stringVal;

			return outputString;
		}

	case ZERO:
		{
			outputString += "0";
			return outputString;
		}

	case TEXT:
		{
			outputString += getTreeText( tree );
			return outputString;
		}

	case IDENTIFIER:
	case DOT:
		{
			return referenceScopeTrans( tree, YES );
		}

	case MATRIX:
		{
			return matrixScopeTrans( tree );
		}

	case LPAREN:
		{
			outputString += "( ";

			pANTLR3_BASE_TREE child = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, 0 )  );
			outputString += exprScopeTrans( child );

			outputString += " )";
			return outputString;
		}

	case LBRACE:
		{
			outputString += "{ ";

			int noChildren = static_cast< int >(  tree->getChildCount( tree )  );
			bool first = true;
			for( int ix = 0 ; ix < noChildren ; ++ix ) {
				if ( first ) {
					first = false;
				} else {
					outputString += ", ";
				}
				pANTLR3_BASE_TREE child = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, ix )  );
				outputString += exprScopeTrans( child );
			}

			outputString += " }";
			return outputString;
		}

	case END:
		{
			outputString += "end";
			return outputString;
		}
	}

	std::cerr << "operator \"" << getTreeText( tree ) << "\" not currently supported." << std::endl;
	assert( false );

	return "";
}

std::string MFileScope::referenceScopeTrans( pANTLR3_BASE_TREE tree, Replace replace ) {

	std::string outputString;

	switch(  tree->getType( tree )  ) {

	case IDENTIFIER:
		{
			outputString = identifierScopeTrans( tree, replace );
			return outputString;
		}
	case DOT:
		{
			pANTLR3_BASE_TREE reference1 = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, 0 )  );
			pANTLR3_BASE_TREE reference2 = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, 1 )  );

			outputString += referenceScopeTrans( reference1, replace );
			outputString += ".";
			outputString += referenceScopeTrans( reference2, NO );

			return outputString;
		}
	}

	assert( false );
	return outputString;
}

std::string MFileScope::printArgs( pANTLR3_BASE_TREE tree ) {

	std::string outputString;
	int noChildren = static_cast< int >(  tree->getChildCount( tree )  );

	std::string leftContainerChar = getTreeText( tree );
	std::string rightContainerChar = leftContainerChar == "(" ? ")" : "}";

	outputString += leftContainerChar + " ";

	bool first = true;
	for( int ix = 0 ; ix < noChildren ; ++ix ) {

		if ( first ) {
			first = false;
		} else {
			outputString += ", ";
		}

		pANTLR3_BASE_TREE child = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, ix )  );
		outputString += exprScopeTrans( child );

	}
	outputString += " " + rightContainerChar;

	return outputString;
}

std::string MFileScope::identifierScopeTrans( pANTLR3_BASE_TREE tree, Replace replace ) {
	std::string outputString;

	std::string symbol = getTreeText( tree );

	if (  !getScopeSymbolTable().isInCurrentScope( symbol )  ) {

		switch( replace ) {
			case POSTOP: {
				std::string expression = " = " + getScopeSymbolTable().getSymbol( symbol );
				outputString += getScopeSymbolTable().addSymbol( symbol ) + expression + "\n";
				break;
			}
			case LHS: {
				getScopeSymbolTable().addSymbol( symbol );
				break;
			}
		}

	}

	// "symbol" REASSIGNED HERE
	if ( replace != NO ) {
		symbol = getScopeSymbolTable().getSymbol( symbol );
	}

	outputString += symbol;

	int noChildren = static_cast< int >(  tree->getChildCount( tree )  );

	for( int ix = 0 ; ix < noChildren - 1 ; ++ix ) {
		pANTLR3_BASE_TREE child = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, ix )  );
		outputString += printArgs( child );
	}

	if ( noChildren > 0 ) {
		pANTLR3_BASE_TREE child = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, noChildren - 1 )  );
		outputString += printArgs( child );
	}

	return outputString;
}

std::string MFileScope::matrixScopeTrans( pANTLR3_BASE_TREE tree ) {

	std::string outputString;

	outputString += "[ ";

	int noChildren = static_cast< int >(  tree->getChildCount( tree )  );

	bool firstRow = true;
	for( int ix = 0 ; ix < noChildren ; ++ix ) {

		if ( firstRow ) {
			firstRow = false;
		} else {
			outputString += " ; ";
		}
		pANTLR3_BASE_TREE child = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, ix )  );

		bool firstColumn = true;
		int noGrandChildren = static_cast< int >(  child->getChildCount( child )  );
		for( int jx = 0 ; jx < noGrandChildren ; ++jx ) {

			if ( firstColumn ) {
				firstColumn = false;
			} else {
				outputString += ", ";
			}
			pANTLR3_BASE_TREE grandChild = static_cast< pANTLR3_BASE_TREE >(  child->getChild( child, jx )  );

			outputString += exprScopeTrans( grandChild );
		}
	}

	outputString += " ]";

	return outputString;
}

std::string MFileScope::getTreeText( pANTLR3_BASE_TREE tree ) {
	ANTLR3_STRING_struct *antlr3StringStruct = tree->getText( tree );
	return antlr3StringStruct == 0 ? "" : (char *)antlr3StringStruct->chars;
}

std::string MFileScope::getDeepTreeText( pANTLR3_BASE_TREE tree ) {
	
	std::string parentText = getTreeText( tree );

	int noChildren = static_cast< int >(  tree->getChildCount( tree )  );

	switch( noChildren ) {
		case 0: {
			return parentText;
		}
		case 1: {
			pANTLR3_BASE_TREE child = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, 0 )  );
			return parentText + getDeepTreeText( child );
		}
		case 2: {
			pANTLR3_BASE_TREE leftChild = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, 0 )  );
			pANTLR3_BASE_TREE rightChild = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, 1 )  );
			return getDeepTreeText( leftChild ) + parentText + getDeepTreeText( rightChild );
		}
		default: {
			std::string returnText = parentText + "(";
			bool first = true;
			for( int ix = 0 ; ix < noChildren ; ++ix ) {
				if ( first ) first = false;
				else returnText += ",";

				pANTLR3_BASE_TREE child = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, ix )  );
				returnText += getDeepTreeText( child );
			}
			return returnText;
		}
	}

	return "";
}
