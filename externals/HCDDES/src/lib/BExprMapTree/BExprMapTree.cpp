#include "BExprMapTree.hpp"

extern "C" {
#include "BExpr/BExprParserLexer.h"
#include "BExpr/BExprParserParser.h"
}

#include <antlr3.h>
#include <iostream>

BENodeSPVector buildBExprMapTree( pANTLR3_BASE_TREE tree );
BENodeSP buildBExprMapTreeAux( pANTLR3_BASE_TREE tree );

BENodeSPVector getBExprMapTree( std::string bExpr ) {

	pANTLR3_INPUT_STREAM input = antlr3NewAsciiStringCopyStream(
	 ( pANTLR3_UINT8 )bExpr.c_str(), (ANTLR3_UINT32) bExpr.size(), 0
	);

	pBExprParserLexer lexer = BExprParserLexerNew( input );

	pANTLR3_COMMON_TOKEN_STREAM tokens = antlr3CommonTokenStreamSourceNew(  ANTLR3_SIZE_HINT, TOKENSOURCE( lexer )  );

	pBExprParserParser parser = BExprParserParserNew( tokens );
	BExprParserParser_expr_return r = parser->expr( parser );

	pANTLR3_BASE_TREE tree = r.tree;

	std::cout << static_cast< unsigned char * >(  tree->toStringTree( tree )->chars  ) << std::endl;

	return buildBExprMapTree( tree );
}

BENodeSPVector buildBExprMapTree( pANTLR3_BASE_TREE tree ) {
	BENodeSPVector beNodeSPVector;

	int noChildren = static_cast< int >(  tree->getChildCount( tree )  );
	for( int ix = 0 ; ix < noChildren ; (void)++ix ) {
		pANTLR3_BASE_TREE child = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, ix )  );
		beNodeSPVector.push_back(  buildBExprMapTreeAux( child )  );
	}

	return beNodeSPVector;
}

BENodeSP buildBExprMapTreeAux( pANTLR3_BASE_TREE tree ) {

	std::string beNodeText = ( char * )tree->getText( tree )->chars;

	BENodeSP beNodeSP(  new BENode( beNodeText, BENodeSPVector() )  );

	BENodeSPVector &beNodeSPVector = beNodeSP->second;

	int noChildren = static_cast< int >(  tree->getChildCount( tree )  );
	for( int ix = 0 ; ix < noChildren ; (void)++ix ) {
		pANTLR3_BASE_TREE child = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, ix )  );
		beNodeSPVector.push_back(  buildBExprMapTreeAux( child )  );
	}

	return beNodeSP;
}


DotVector buildDotVector( pANTLR3_BASE_TREE tree );

BExprOutputVector getBExprOutputVector( std::string bExpr ) {

	pANTLR3_INPUT_STREAM input = antlr3NewAsciiStringCopyStream(
	 ( pANTLR3_UINT8 )bExpr.c_str(), (ANTLR3_UINT32) bExpr.size(), 0
	);

	pBExprParserLexer lexer = BExprParserLexerNew( input );

	pANTLR3_COMMON_TOKEN_STREAM tokens = antlr3CommonTokenStreamSourceNew(  ANTLR3_SIZE_HINT, TOKENSOURCE( lexer )  );

	pBExprParserParser parser = BExprParserParserNew( tokens );
	BExprParserParser_nexpr_return r = parser->nexpr( parser );

	pANTLR3_BASE_TREE tree = r.tree;

	std::cout << static_cast< unsigned char * >(  tree->toStringTree( tree )->chars  ) << std::endl;

	BExprOutputVector bExprOutputVector;

	if ( tree->getType( tree ) == NME ) {

		bExprOutputVector.push_back(  buildDotVector( tree )  );

	} else {

		int noChildren = static_cast< int >(  tree->getChildCount( tree )  );
		for( int ix = 0 ; ix < noChildren ; (void)++ix ) {
			pANTLR3_BASE_TREE child = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, ix )  );
			bExprOutputVector.push_back(  buildDotVector( child )  );
		}

	}

	return bExprOutputVector;
}

DotVector buildDotVector( pANTLR3_BASE_TREE tree ) {

	std::string dotText = ( char * )tree->getText( tree )->chars;

	DotVector dotVector;
	dotVector.push_back( dotText );
	
	int noChildren = static_cast< int >(  tree->getChildCount( tree )  );
	for( int ix = 0 ; ix < noChildren ; (void)++ix ) {
		pANTLR3_BASE_TREE child = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, ix )  );
		dotText = ( char * )child->getText( child )->chars;
		dotVector.push_back( dotText );
	}

	return dotVector;
}
