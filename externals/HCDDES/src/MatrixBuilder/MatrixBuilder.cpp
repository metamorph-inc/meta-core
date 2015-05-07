#include <iostream>
#include <sstream>
#include <cmath>
#include <limits>

#ifdef _WIN32
#include <float.h>
#define ISNAN(x) _isnan(x)
#define M_PI 3.14159265358979323846
#define M_E 2.7182818284590452354
#else
#define ISNAN(x) isnan(x)
#endif

#include "MatrixBuilder.hpp"

#include "MExpr/MExprLexer.hpp"
#include "MExpr/MExprParser.hpp"
#include "MExpr/MExprParserTokenTypes.hpp"


double getValue( antlr::RefAST &refAST );
EnhancedMatrix buildMatrixAux( antlr::RefAST &refAST );


EnhancedMatrix buildMatrix( const std::string &input ) {

	std::istringstream mExprStream( input.c_str() );

	MExprLexer mExprLexer( mExprStream );
	MExprParser mExprParser( mExprLexer );

#ifdef ANTLR_AST_INIT
	antlr::ASTFactory astFactory;
	mExprParser.initializeASTFactory( astFactory );
	mExprParser.setASTFactory( &astFactory );
#endif

	try {
		mExprParser.expr();
	} catch (antlr::ANTLRException &antlrException) {
		std::cerr << "Exception in ANTLR:" << std::endl;
		std::cerr << antlrException.getMessage() << std::endl;
		exit(1);
	}

	antlr::RefAST refAST = mExprParser.getAST();
	return buildMatrixAux( refAST );
}


EnhancedMatrix buildMatrixAux( antlr::RefAST &refAST ) {

	switch( refAST->getType() ) {

		case MExprParserTokenTypes::SEM :
		{
			//std::cerr << "SEM" << std::endl;
			antlr::RefAST child1 = refAST->getFirstChild();
			EnhancedMatrix enhancedMatrix1 = buildMatrixAux( child1 );
			antlr::RefAST child2 = child1->getNextSibling();
			EnhancedMatrix enhancedMatrix2 = buildMatrixAux( child2 );
			return enhancedMatrix1.colJoin( enhancedMatrix2 );
		}

		case MExprParserTokenTypes::WS  :
		case MExprParserTokenTypes::CMA :
		{
			//std::cerr << "WS|CMA" << std::endl;
			antlr::RefAST child = refAST->getFirstChild();
			if ( child == antlr::nullAST ) return EnhancedMatrix();

			EnhancedMatrix enhancedMatrix = buildMatrixAux( child );
			while(  ( child = child->getNextSibling() )  !=  antlr::nullAST  ) {
				enhancedMatrix = enhancedMatrix.rowJoin(  buildMatrixAux( child )  );
			}
			return enhancedMatrix;
		}

		case MExprParserTokenTypes::LBRACK :
		{
			//std::cerr << "LBRACK" << std::endl;
			antlr::RefAST child = refAST->getFirstChild();
			if ( child == antlr::nullAST ) return EnhancedMatrix();

			EnhancedMatrix enhancedMatrix = buildMatrixAux( child );
			while(  ( child = child->getNextSibling() )  !=  antlr::nullAST  ) {
				enhancedMatrix = enhancedMatrix.rowJoin(  buildMatrixAux( child )  );
			}
			return enhancedMatrix;
		}

		case MExprParserTokenTypes::ADD :
		case MExprParserTokenTypes::SUB :
		case MExprParserTokenTypes::MUL :
		case MExprParserTokenTypes::DIV :
		case MExprParserTokenTypes::MOD :
		case MExprParserTokenTypes::VAR :
		case MExprParserTokenTypes::NUM :
		{
			//std::cerr << "VALUE" << std::endl;
			double value = getValue( refAST );
			return EnhancedMatrix( value );
		}
	}
	return EnhancedMatrix();
}


double getValue( antlr::RefAST &refAST ) {

	switch( refAST->getType() ) {

		case MExprParserTokenTypes::ADD :
		case MExprParserTokenTypes::SUB :
		{
			std::string op = refAST->getText();

			antlr::RefAST child1 = refAST->getFirstChild();
			double value1 = getValue( child1 );
			if (  ISNAN( value1 )  ) return std::numeric_limits< double >::quiet_NaN();

			antlr::RefAST child2 = child1->getNextSibling();
			if ( child2 == antlr::nullAST ) {
				return op == "+" ? value1 : -value1;
			}
			double value2 = getValue( child2 );
			if (  ISNAN( value2 )  ) return std::numeric_limits< double >::quiet_NaN();

			return op == "+" ? value1 + value2 : value1 - value2;
		}

		case MExprParserTokenTypes::MUL :
		case MExprParserTokenTypes::DIV :
		case MExprParserTokenTypes::MOD :
		{
			std::string op = refAST->getText();
			antlr::RefAST child1 = refAST->getFirstChild();
			double value1 = getValue( child1 );
			if (  ISNAN( value1 )  ) return std::numeric_limits< double >::quiet_NaN();

			antlr::RefAST child2 = child1->getNextSibling();
			double value2 = getValue( child2 );
			if (  ISNAN( value2 )  ) return std::numeric_limits< double >::quiet_NaN();

			if ( op == "*" ) return value1 * value2;
			if ( op == "/" ) return value1 / value2;

			// MOD (%)
			if ( value2 == 0 ) return std::numeric_limits< double >::quiet_NaN();
			return static_cast< int >( value1 ) % static_cast< int >( value2 );
		}

		case MExprParserTokenTypes::VAR :
		{
			std::string op = refAST->getText();
			if ( op == "pi" ) return M_PI;
			if ( op == "e" ) return M_E;
			return std::numeric_limits< double >::quiet_NaN();
		}

		case MExprParserTokenTypes::NUM :
		{
			std::string num = refAST->getText();
			return atof( num.c_str() );
		}

	}

	return std::numeric_limits< double >::quiet_NaN();
}
