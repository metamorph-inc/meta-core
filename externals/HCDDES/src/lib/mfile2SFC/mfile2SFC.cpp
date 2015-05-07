#include "mfile2SFC.hpp"

#include <boost/filesystem.hpp>

extern "C" {
#include "MatLabTokens.h"
#include "MatLab.h"
}

typedef std::list< pANTLR3_BASE_TREE > PAntlr3BaseTreeList;

#include "RuleReturn.hpp"
#include "ExprReturn.hpp"
#include "VarExprReturn.hpp"
#include "ColonExprReturn.hpp"
#include "ReferenceReturn.hpp"
#include "ArgReturn.hpp"
#include "VarMatrixExprReturn.hpp"
#include "LitMatrixExprReturn.hpp"

#include "SFCUdmEngine.hpp"
#include "SFCHelper.hpp"
#include "MFileManager.hpp"
#include "antlr3.h"

bool treeCompare( pANTLR3_BASE_TREE tree1, pANTLR3_BASE_TREE tree2 );
void inputTree( pANTLR3_BASE_TREE tree, SFC::CompoundStatement block, DTList &inputDTList );
void functionTree( pANTLR3_BASE_TREE tree, SFC::CompoundStatement block, DTList &inputDTList );
void finputTree( pANTLR3_BASE_TREE tree, SFC::Function sfcFunction, DTList &inputDTList );
SFC::ArgDeclBase foutputTree( pANTLR3_BASE_TREE tree, SFC::Function sfcFunction );
void nameFunctionAncestors( SFC::Function sfcFunction );
void statementTree( pANTLR3_BASE_TREE tree, SFC::CompoundStatement block );
void ifTree( pANTLR3_BASE_TREE tree, SFC::CompoundStatement block );
void switchTree( pANTLR3_BASE_TREE tree, SFC::CompoundStatement block );
void makeCase( pANTLR3_BASE_TREE tree, pANTLR3_BASE_TREE switch_expr, SFC::CompoundStatement block );
void whileTree( pANTLR3_BASE_TREE tree, SFC::CompoundStatement block );
void forTree( pANTLR3_BASE_TREE tree, SFC::CompoundStatement block );
ExprReturnSP exprTree( pANTLR3_BASE_TREE tree, SFC::CompoundStatement block );
ExprReturnSP colonTree( pANTLR3_BASE_TREE tree, SFC::CompoundStatement block );
ExprReturnSP referenceTree( pANTLR3_BASE_TREE tree, SFC::CompoundStatement block, bool lvalue );
ExprReturnSP referenceItemTree( pANTLR3_BASE_TREE tree, SFC::CompoundStatement block, bool lvalue, ExprReturnSP exprReturnSP = ExprReturnSP() );
ArgReturnSP argTree( const pANTLR3_BASE_TREE tree, SFC::CompoundStatement block, bool lvalue = false );
ExprReturnSP matrixTree( pANTLR3_BASE_TREE tree, SFC::CompoundStatement block );
VarExprReturnSPVector matrixReferenceTree( pANTLR3_BASE_TREE tree, SFC::CompoundStatement block );
std::string getTreeText( pANTLR3_BASE_TREE tree );
std::string getDeepTreeText( pANTLR3_BASE_TREE tree );
void createExternalFunctionCall( pANTLR3_BASE_TREE reference, pANTLR3_BASE_TREE expr, SFC::CompoundStatement block );
void assignType( pANTLR3_BASE_TREE reference, SFC::DT dt );
SFC::ArgDeclBase getArgDeclBase( pANTLR3_BASE_TREE tree );

void addMFileDirectory( const std::string &directoryName ) {
	MFileManager::get_singleton().addDirectory( directoryName );
}
void addMFileDirectories( const DirectoryVector &directoryVector ) {
	for( DirectoryVector::const_iterator drvItr = directoryVector.begin() ; drvItr != directoryVector.end() ; ++drvItr ) addMFileDirectory( *drvItr );
}
void deleteMFileDirectory( const std::string &directoryName ) {
	MFileManager::get_singleton().deleteDirectory( directoryName );
}

void mfile2SFC( SFC::CompoundStatement compoundStatement, DTList &inputDTList, const std::string &mfile ) {

	try {
		pANTLR3_INPUT_STREAM input = antlr3NewAsciiStringCopyStream(
		 ( pANTLR3_UINT8 )mfile.c_str(), (ANTLR3_UINT32) mfile.size(), 0
		);

		pMatLabTokens lexer = MatLabTokensNew( input );

		pANTLR3_COMMON_TOKEN_STREAM tokens = antlr3CommonTokenStreamSourceNew(  ANTLR3_SIZE_HINT, TOKENSOURCE( lexer )  );

		pMatLab parser = MatLabNew( tokens );
		MatLab_input_return r = parser->input( parser );

		pANTLR3_BASE_TREE tree = r.tree;

		std::cout << getTreeText( tree ) << std::endl;

		if (  tree->isNilNode( tree )  ) {
			int noChildren = static_cast< int >(  tree->getChildCount( tree )  );
			for( int ix = 0 ; ix < noChildren ; (void)++ix ) {

				pANTLR3_BASE_TREE child = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, ix )  );
				bool printStatement = true;

				try {

					inputTree( child, compoundStatement, inputDTList );
					printStatement = false;

				} catch ( udm_exception &u ) {

					std::cerr << "The following UDM exception occurred:" << std::endl;
					std::cerr << u.what() << std::endl << std::endl;

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
			inputTree( tree, compoundStatement, inputDTList );
		}

		parser->free( parser );
		tokens->free( tokens );
		lexer->free( lexer );
		input->close( input );

	} catch ( udm_exception &u ) {

		std::cerr << "The following UDM exception occurred:" << std::endl;
		std::cerr << u.what() << std::endl << std::endl;

	} catch ( std::exception &e ) {

		std::cerr << "The following std::exception occurred:" << std::endl;
		std::cerr << e.what() << std::endl << std::endl;

	} catch ( ... ) {

		std::cerr << "Exception of unknown type caught." << std::endl;

	}

}

void emfunc2SFC(
 SFC::CompoundStatement compoundStatement,
 const VList& inputs,
 const VList& outputs,
 const std::string &statement
) {

	try {

#ifdef _DEBUG
		std::cout << "INPUT:" << std::endl << statement << std::endl;
#endif

		pANTLR3_INPUT_STREAM input = antlr3NewAsciiStringCopyStream(
		 ( pANTLR3_UINT8 )statement.c_str(), (ANTLR3_UINT32) statement.size(), 0
		);

		pMatLabTokens lexer = MatLabTokensNew( input );

		pANTLR3_COMMON_TOKEN_STREAM tokens = antlr3CommonTokenStreamSourceNew(  ANTLR3_SIZE_HINT, TOKENSOURCE( lexer )  );

		pMatLab parser = MatLabNew( tokens );
		MatLab_input_return r = parser->input( parser );

		pANTLR3_BASE_TREE tree = r.tree;

#ifdef _DEBUG
		std::cout << "PARSE TREE:" << std::endl <<
		 static_cast< unsigned char * >(  tree->toStringTree( tree )->chars  ) << std::endl << std::endl;
#endif

		int noChildren = static_cast< int >(  tree->getChildCount( tree )  );

		for( int ix = 0 ; ix < noChildren ; (void)++ix ) {
			pANTLR3_BASE_TREE child = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, ix )  );

			switch(  child->getType( child )  ) {
			case FNAME:
				{
					break;
				}
			case FINPUT:
				{
					int noGrandChildren = static_cast< int >(  child->getChildCount( child )  );

					assert( noGrandChildren == inputs.size() );

					VList::const_iterator inpItr = inputs.begin();

					int ix = 0;
					while( ix < noGrandChildren ) {
						pANTLR3_BASE_TREE grandChild = static_cast< pANTLR3_BASE_TREE >(  child->getChild( child, ix )  );

						std::string argName = getTreeText( grandChild );

						getSFCSymbolTable().push( argName, *inpItr );
						(void)++ix; (void)++inpItr;
					}
					break;
				}
			case FOUTPUT:
				{
					int noGrandChildren = static_cast< int >(  child->getChildCount( child )  );

					assert( noGrandChildren == outputs.size() );

					VList::const_iterator otpItr = outputs.begin();

					int ix = 0;
					while( ix < noGrandChildren ) {
						pANTLR3_BASE_TREE grandChild = static_cast< pANTLR3_BASE_TREE >(  child->getChild( child, ix )  );

						std::string argName = getTreeText( grandChild );

						getSFCSymbolTable().push( argName, *otpItr );
						(void)++ix; (void)++otpItr;
					}
					break;
				}
			default:
				{
					try {

						statementTree( child, compoundStatement );

					} catch ( udm_exception &u ) {

						std::cerr << "The following UDM exception occurred:" << std::endl;
						std::cerr << u.what() << std::endl << std::endl;

					} catch ( std::exception &e ) {

						std::cerr << "The following std::exception occurred:" << std::endl;
						std::cerr << e.what() << std::endl << std::endl;

					} catch ( ... ) {

						std::cerr << "Exception of unknown type caught." << std::endl;

					}

					break;
				}
			}
		}

		getSFCSymbolTable().clear();

		parser->free( parser );
		tokens->free( tokens );
		lexer->free( lexer );
		input->close( input );	

	} catch ( udm_exception &u ) {

		std::cerr << "The following UDM exception occurred:" << std::endl;
		std::cerr << u.what() << std::endl << std::endl;

	} catch ( std::exception &e ) {

		std::cerr << "The following std::exception occurred:" << std::endl;
		std::cerr << e.what() << std::endl << std::endl;

	} catch ( ... ) {

		std::cerr << "Exception of unknown type caught." << std::endl;

	}
}


void mstat2SFC(
 SFC::CompoundStatement compoundStatement,
 const std::string &statement,
 bool zeroIndex,
 bool stateflowFlag,
 const FunctionNameMap &functionNameMap
) {

	try {

		if ( statement.empty() ) return;

		MatLabStateflowFlag = stateflowFlag;

		ExprReturn::setZeroIndex( zeroIndex );
		ExprReturn::getFunctionNameMap() = functionNameMap;

#ifdef _DEBUG
		if ( zeroIndex ) {
			std::cout << "Zero-indexing is ON" << std::endl;
		}
		std::cout << "INPUT:" << std::endl << statement << std::endl;
#endif

		pANTLR3_INPUT_STREAM input = antlr3NewAsciiStringCopyStream(
		 ( pANTLR3_UINT8 )statement.c_str(), (ANTLR3_UINT32) statement.size(), 0
		);

		pMatLabTokens lexer = MatLabTokensNew( input );

		pANTLR3_COMMON_TOKEN_STREAM tokens = antlr3CommonTokenStreamSourceNew(  ANTLR3_SIZE_HINT, TOKENSOURCE( lexer )  );

		pMatLab parser = MatLabNew( tokens );
		MatLab_input_return r = parser->input( parser );

		pANTLR3_BASE_TREE tree = r.tree;

		if ( tree == 0 ) return;
#ifdef _DEBUG
		pANTLR3_STRING treeString = tree->toStringTree( tree );
		std::string sTreeString = treeString == NULL ? "[parse unsuccessful]" : reinterpret_cast< char * >( treeString->chars );
		std::cout << "PARSE TREE:" << std::endl << sTreeString << std::endl << std::endl;
#endif

		if (  tree->isNilNode( tree )  ) {
			int noChildren = static_cast< int >(  tree->getChildCount( tree )  );
			for( int ix = 0 ; ix < noChildren ; (void)++ix ) {

				pANTLR3_BASE_TREE child = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, ix )  );
				bool printStatement = true;

				try {

					statementTree( child, compoundStatement );
					printStatement = false;

				} catch ( udm_exception &u ) {

					std::cerr << "The following UDM exception occurred:" << std::endl;
					std::cerr << u.what() << std::endl << std::endl;

				} catch ( std::exception &e ) {

					std::cerr << "The following std::exception occurred:" << std::endl;
					std::cerr << e.what() << std::endl << std::endl;

				} catch ( ... ) {

					std::cerr << "Exception of unknown type caught." << std::endl;

				}

				if ( printStatement ) {
					std::cerr << "Exception caused by:" << std::endl;
					pANTLR3_STRING treeString = tree->toStringTree( child );
					std::string sTreeString = treeString == NULL ? "[parse unsuccessful]" : reinterpret_cast< char * >( treeString->chars );
					std::cerr << "PARSE TREE:" << std::endl << sTreeString << std::endl << std::endl;
				}
			}
		} else {
			statementTree( tree, compoundStatement );
		}

		ExprReturn::setZeroIndex( false );
		ExprReturn::getFunctionNameMap().clear();

		parser->free( parser );
		tokens->free( tokens );
		lexer->free( lexer );
		input->close( input );	

	} catch ( udm_exception &u ) {

		std::cerr << "The following UDM exception occurred:" << std::endl;
		std::cerr << u.what() << std::endl << std::endl;

	} catch ( std::exception &e ) {

		std::cerr << "The following std::exception occurred:" << std::endl;
		std::cerr << e.what() << std::endl << std::endl;

	} catch ( ... ) {

		std::cerr << "Exception of unknown type caught." << std::endl;

	}
}

void mexprStat2SFC(
 SFC::CompoundStatement compoundStatement,
 const std::string &statement,
 const FunctionNameMap &functionNameMap
) {

	try {

		if ( statement.empty() ) return;

		MatLabStateflowFlag = true;

		ExprReturn::setZeroIndex( true );
		ExprReturn::getFunctionNameMap() = functionNameMap;

#ifdef _DEBUG
		std::cout << "Zero-indexing is ON" << std::endl;
		std::cout << "INPUT:" << std::endl << statement << std::endl;
#endif

		pANTLR3_INPUT_STREAM input = antlr3NewAsciiStringCopyStream(
		 ( pANTLR3_UINT8 )statement.c_str(), (ANTLR3_UINT32) statement.size(), 0
		);

		pMatLabTokens lexer = MatLabTokensNew( input );

		pANTLR3_COMMON_TOKEN_STREAM tokens = antlr3CommonTokenStreamSourceNew(  ANTLR3_SIZE_HINT, TOKENSOURCE( lexer )  );

		pMatLab parser = MatLabNew( tokens );
		MatLab_input_return r = parser->input( parser );

		pANTLR3_BASE_TREE tree = r.tree;

		if ( tree == 0 ) return;
#ifdef _DEBUG
		std::cout << "PARSE TREE:" << std::endl <<
		 static_cast< unsigned char * >(  tree->toStringTree( tree )->chars  ) << std::endl << std::endl;
#endif

		if (  tree->isNilNode( tree )  ) {
			int noChildren = static_cast< int >(  tree->getChildCount( tree )  );
			for( int ix = 0 ; ix < noChildren ; (void)++ix ) {
				pANTLR3_BASE_TREE child = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, ix )  );

				try {

					exprTree( child, compoundStatement )->collapse();

				} catch ( udm_exception &u ) {

					std::cerr << "The following UDM exception occurred:" << std::endl;
					std::cerr << u.what() << std::endl << std::endl;

				} catch ( std::exception &e ) {

					std::cerr << "The following std::exception occurred:" << std::endl;
					std::cerr << e.what() << std::endl << std::endl;

				} catch ( ... ) {

					std::cerr << "Exception of unknown type caught." << std::endl;

				}

			}
		} else {
			exprTree( tree, compoundStatement )->collapse();
		}

		ExprReturn::setZeroIndex( false );
		ExprReturn::getFunctionNameMap().clear();

		parser->free( parser );
		tokens->free( tokens );
		lexer->free( lexer );
		input->close( input );	

	} catch ( udm_exception &u ) {

		std::cerr << "The following UDM exception occurred:" << std::endl;
		std::cerr << u.what() << std::endl << std::endl;

	} catch ( std::exception &e ) {

		std::cerr << "The following std::exception occurred:" << std::endl;
		std::cerr << e.what() << std::endl << std::endl;

	} catch ( ... ) {

		std::cerr << "Exception of unknown type caught." << std::endl;

	}
}

void mexpr2SFCCondition(
 SFC::CompoundStatement conditionTypeStatement,
 const std::string &expression,
 bool zeroIndex,
 bool stateflowFlag,
 const FunctionNameMap &functionNameMap
) {

	try {
		assert(
		 conditionTypeStatement.type() == SFC::ConditionalBlock::meta ||
		 conditionTypeStatement.type() == SFC::IterativeBlock::meta
		);

		ExprReturn::setZeroIndex( zeroIndex );
		ExprReturn::getFunctionNameMap() = functionNameMap;

		MatLabStateflowFlag = stateflowFlag;

#ifdef _DEBUG
		if ( zeroIndex ) {
			std::cout << "Zero-indexing is ON" << std::endl;
		}
		std::cout << "INPUT:" << std::endl << expression << std::endl;
#endif

		pANTLR3_INPUT_STREAM input = antlr3NewAsciiStringCopyStream(
		 ( pANTLR3_UINT8 )expression.c_str(), (ANTLR3_UINT32) expression.size(), 0
		);

		pMatLabTokens lexer = MatLabTokensNew( input );

		pANTLR3_COMMON_TOKEN_STREAM tokens = antlr3CommonTokenStreamSourceNew(  ANTLR3_SIZE_HINT, TOKENSOURCE( lexer )  );

		pMatLab parser = MatLabNew( tokens );
		MatLab_expr_entry_return e = parser->expr_entry( parser );

		pANTLR3_BASE_TREE tree = e.tree;

		if ( tree == 0 ) return;
#ifdef _DEBUG
		std::cout << "PARSE TREE:" << std::endl <<
		 static_cast< unsigned char * >(  tree->toStringTree( tree )->chars  ) << std::endl << std::endl;
#endif

		ExprReturnSP exprReturnSP = exprTree( tree, conditionTypeStatement )->collapseLogical();
		exprReturnSP->buildUdm( conditionTypeStatement, conditionTypeStatement.type() == SFC::ConditionalBlock::meta ? SFC::ConditionalBlock::meta_cond : SFC::IterativeBlock::meta_cond );

		ExprReturn::setZeroIndex( false );
		ExprReturn::getFunctionNameMap().clear();

		parser->free( parser );
		tokens->free( tokens );
		lexer->free( lexer );
		input->close( input );	

	} catch ( udm_exception &u ) {

		std::cerr << "The following UDM exception occurred:" << std::endl;
		std::cerr << u.what() << std::endl << std::endl;

	} catch ( std::exception &e ) {

		std::cerr << "The following std::exception occurred:" << std::endl;
		std::cerr << e.what() << std::endl << std::endl;

	} catch ( ... ) {

		std::cerr << "Exception of unknown type caught." << std::endl;

	}
}


SFC::DT getMatrixSFCType( SFC::CompoundStatement compoundStatement, const std::string &matrix ) {

	pANTLR3_INPUT_STREAM input = antlr3NewAsciiStringCopyStream(
	 ( pANTLR3_UINT8 )matrix.c_str(), (ANTLR3_UINT32) matrix.size(), 0
	);

	pMatLabTokens lexer = MatLabTokensNew( input );

	pANTLR3_COMMON_TOKEN_STREAM tokens = antlr3CommonTokenStreamSourceNew(  ANTLR3_SIZE_HINT, TOKENSOURCE( lexer )  );

	pMatLab parser = MatLabNew( tokens );
	MatLab_expr_entry_return e = parser->expr_entry( parser );

	pANTLR3_BASE_TREE tree = e.tree;

#ifdef _DEBUG
	std::cout << static_cast< unsigned char * >(  tree->toStringTree( tree )->chars  ) << std::endl;
#endif

	ExprReturnSP exprReturnSP = exprTree( tree, compoundStatement );
	return exprReturnSP->getDT();
}


void getDimensions( const std::string &matrix, int &rows, int &columns ) {

	EnhancedMatrix enhancedMatrix = getMatrix( matrix );
	rows = 1;
	columns = 1;
	EnhancedMatrix::iterator1 enmItr1 = enhancedMatrix.begin1();
	EnhancedMatrix::iterator2 enmItr2 = enmItr1.begin();
	rows = (int) *enmItr2;
	if ( ++enmItr2 != enmItr1.end() ) {
		columns = (int) *enmItr2;
	} else if ( ++enmItr1 != enhancedMatrix.end1() ) {
		enmItr2 = enmItr1.begin();
		columns = (int) *enmItr2;
	}
	return;
}

EnhancedMatrix getMatrix( const std::string &matrix ) {

	SFCUdmEngine::get_singleton().createNewDataNetwork( "_sftemp.mem" );
	SFC::Project project = SFCUdmEngine::get_singleton().getRoot();
	SFCTypesManager::initSingleton( project );
	SFC::Program program = SFC::Program::Create( project );

	EnhancedMatrix enhancedMatrix = getMatrix( program, matrix );

	SFCUdmEngine::get_singleton().closeWithUpdate();
	boost::filesystem::remove( "_sftemp.mem" );

	return enhancedMatrix;
}


EnhancedMatrix getMatrix( SFC::CompoundStatement compoundStatement, const std::string &origMatrix ) {

	string matrix( origMatrix );
	if (  matrix.find( "[" ) == std::string::npos  ) matrix = "[" + matrix + "]";

	pANTLR3_INPUT_STREAM input = antlr3NewAsciiStringCopyStream(
	 ( pANTLR3_UINT8 )matrix.c_str(), (ANTLR3_UINT32) matrix.size(), 0
	);

	pMatLabTokens lexer = MatLabTokensNew( input );

	pANTLR3_COMMON_TOKEN_STREAM tokens = antlr3CommonTokenStreamSourceNew(  ANTLR3_SIZE_HINT, TOKENSOURCE( lexer )  );

	pMatLab parser = MatLabNew( tokens );
	MatLab_expr_entry_return e = parser->expr_entry( parser );

	pANTLR3_BASE_TREE tree = e.tree;

#ifdef _DEBUG
	std::cout << static_cast< unsigned char * >(  tree->toStringTree( tree )->chars  ) << std::endl;
#endif

	ExprReturnSP exprReturnSP = exprTree( tree, compoundStatement );

	if ( exprReturnSP->getClassName() != "LitMatrixExprReturn" ) {
		std::cerr << "ERROR:  extractSFCMatrix: Non-literal matrix" << std::endl;
		return EnhancedMatrix();
	}

	LitMatrixExprReturnSP litMatrixExprReturnSP =
	 ExprReturn::static_pointer_cast< LitMatrixExprReturn >( exprReturnSP );

	SFC::DT dt = exprReturnSP->getDT();
	__int64 rows, columns;
	SFCTypesManager::getRowsAndColumns( dt, rows, columns );

	LitMatrixExprReturn::Sequence::iterator sqnItr = litMatrixExprReturnSP->begin();
	EnhancedMatrix enhancedMatrix( (int) rows, (int) columns );
	for(
	 EnhancedMatrix::iterator1 enmItr1 = enhancedMatrix.begin1() ;
	 enmItr1 != enhancedMatrix.end1() ;
	 (void)++enmItr1
	) {

		for(
		 EnhancedMatrix::iterator2 enmItr2 = enmItr1.begin() ;
		 enmItr2 != enmItr1.end() ;
		 (void)++enmItr2
		) {
			*enmItr2 = *sqnItr++;
		}

	}

	return enhancedMatrix;
}


void getMatrixDimensions( const std::string &matrix, int &rows, int &columns ) {

	SFCUdmEngine::get_singleton().createNewDataNetwork( "_sftemp.mem" );
	SFC::Project project = SFCUdmEngine::get_singleton().getRoot();
	SFCTypesManager::initSingleton( project );
	SFC::Program program = SFC::Program::Create( project );

	getMatrixDimensions( program, matrix, rows, columns );

	SFCUdmEngine::get_singleton().closeWithUpdate();
	boost::filesystem::remove( "_sftemp.mem" );

	return;
}

void getMatrixDimensions( SFC::CompoundStatement compoundStatement, const std::string &matrix, int &rows, int &columns ) {

	pANTLR3_INPUT_STREAM input = antlr3NewAsciiStringCopyStream(
	 ( pANTLR3_UINT8 )matrix.c_str(), (ANTLR3_UINT32) matrix.size(), 0
	);

	pMatLabTokens lexer = MatLabTokensNew( input );

	pANTLR3_COMMON_TOKEN_STREAM tokens = antlr3CommonTokenStreamSourceNew(  ANTLR3_SIZE_HINT, TOKENSOURCE( lexer )  );

	pMatLab parser = MatLabNew( tokens );
	MatLab_expr_entry_return e = parser->expr_entry( parser );

	pANTLR3_BASE_TREE tree = e.tree;

#ifdef _DEBUG
	std::cout << static_cast< unsigned char * >(  tree->toStringTree( tree )->chars  ) << std::endl;
#endif

	ExprReturnSP exprReturnSP = exprTree( tree, compoundStatement );

	SFC::DT dt = exprReturnSP->getDT();
	__int64 rows64, columns64;
	SFCTypesManager::getRowsAndColumns( dt, rows64, columns64 );
	rows = (int) rows64;
	columns = (int) columns64;

	return;
}


bool treeCompare( pANTLR3_BASE_TREE tree1, pANTLR3_BASE_TREE tree2 ) {

	int type1 = tree1->getType( tree1 );
	int type2 = tree2->getType( tree2 );
	if ( type1 != type2 ) return false;

	std::string text1 = getTreeText( tree1 );
	std::string text2 = getTreeText( tree2 );
	if ( text1 != text2 ) return false;

	int noChildren1 = static_cast< int >(  tree1->getChildCount( tree1 )  );
	int noChildren2 = static_cast< int >(  tree2->getChildCount( tree2 )  );
	if ( noChildren1 != noChildren2 ) return false;

	for( int ix = 0 ; ix < noChildren1 ; (void)++ix ) {
		pANTLR3_BASE_TREE child1 = static_cast< pANTLR3_BASE_TREE >(  tree1->getChild( tree1, ix )  );
		pANTLR3_BASE_TREE child2 = static_cast< pANTLR3_BASE_TREE >(  tree2->getChild( tree2, ix )  );

		if (  !treeCompare( child1, child2 )  ) return false;
	}

	return true;
}

void inputTree( pANTLR3_BASE_TREE tree, SFC::CompoundStatement block, DTList &inputDTList ) {

	switch(  tree->getType( tree )  ) {
	case FUNCTION:
		{
			functionTree( tree, block, inputDTList );
			break;
		}
	default:
		{
			statementTree( tree, block );
			break;
		}
	}
}

void functionTree( pANTLR3_BASE_TREE tree, SFC::CompoundStatement block, DTList &inputDTList ) {

	SFC::Function sfcFunction = SFC::Function::Create( block );

	int noChildren = static_cast< int >(  tree->getChildCount( tree )  );
	SFC::ArgDeclBase argDeclBase;

	for( int ix = 0 ; ix < noChildren ; (void)++ix ) {
		pANTLR3_BASE_TREE child = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, ix )  );

		switch(  child->getType( child )  ) {
		case FNAME:
			{
				pANTLR3_BASE_TREE fname_child = static_cast< pANTLR3_BASE_TREE >(  child->getChild( child, 0 )  );
				std::string functionName = getTreeText( fname_child );
				sfcFunction.name() = functionName;
				nameFunctionAncestors( sfcFunction );
				break;
			}
		case FINPUT:
			{
				finputTree( child, sfcFunction, inputDTList );
				break;
			}
		case FOUTPUT:
			{
				argDeclBase = foutputTree( child, sfcFunction );
				break;
			}
		default:
			{
				statementTree( child, sfcFunction );
				break;
			}
		}
	}

	if ( argDeclBase != Udm::null ) {
		SFC::Return sfcReturn = SFC::Return::Create( sfcFunction );
		SFCManager::numberStatement( sfcReturn );
		SFC::ArgDeclRef argDeclRef = SFC::ArgDeclRef::Create( sfcReturn, SFC::Return::meta_retexpr );
		argDeclRef.argdecl() = argDeclBase;
		sfcFunction.dt() = argDeclBase.dt();
	}
}


void finputTree( pANTLR3_BASE_TREE tree, SFC::Function sfcFunction, DTList &inputDTList ) {
	int noChildren = static_cast< int >(  tree->getChildCount( tree )  );

	assert( noChildren == inputDTList.size() );

	DTList::iterator dtlItr = inputDTList.begin();

	for( int ix = 0 ; ix < noChildren ; (void)++ix ) {
		pANTLR3_BASE_TREE child = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, ix )  );

		SFC::Arg arg = SFC::Arg::Create( sfcFunction );
		std::string argName = getTreeText( child );
		arg.name() = argName;
		arg.ptr() = false;

		__int64 argCount = sfcFunction.argCount();
		arg.argIndex() = argCount++;
		sfcFunction.argCount() = argCount;

		arg.dt() = *dtlItr;
		(void)++dtlItr;

		getSFCSymbolTable().push( argName, arg );
	}
}


SFC::ArgDeclBase foutputTree( pANTLR3_BASE_TREE tree, SFC::Function sfcFunction ) {
	int noChildren = static_cast< int >(  tree->getChildCount( tree )  );

	if ( noChildren == 1 ) {
		pANTLR3_BASE_TREE child = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, 0 )  );

		SFC::LocalVar localVar = SFC::LocalVar::Create( sfcFunction );
		SFCManager::numberStatement( localVar );
		std::string localVarName = getTreeText( child );
		localVar.name() = localVarName;

		getSFCSymbolTable().push( localVarName, localVar );
		return localVar;
	}

	for( int ix = 0 ; ix < noChildren ; (void)++ix ) {
		pANTLR3_BASE_TREE child = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, ix )  );

		SFC::Arg arg = SFC::Arg::Create( sfcFunction );
		std::string argName = getTreeText( child );
		arg.name() = argName;
		arg.ptr() = true;

		__int64 argCount = sfcFunction.argCount();
		arg.argIndex() = argCount++;
		sfcFunction.argCount() = argCount;

		getSFCSymbolTable().push( argName, arg );
	}
	return SFC::ArgDeclBase();
}


void nameFunctionAncestors(	SFC::Function sfcFunction ) {

	Udm::Object parent = sfcFunction.parent();
	std::string functionName = sfcFunction.name();
	while( parent != Udm::null && parent.type() != SFC::Project::meta ) {
		if ( parent.type() == SFC::Program::meta ) {
			SFC::Program program = SFC::Program::Cast( parent );
			program.filename() = functionName;
		} else if ( parent.type() == SFC::Class::meta ) {
			SFC::Class sfcClass = SFC::Class::Cast( parent );
			sfcClass.name() = functionName + "_class";
		}
		parent = parent.GetParent();
	}
}


void statementTree( pANTLR3_BASE_TREE tree, SFC::CompoundStatement block ) {

	switch(  tree->getType( tree )  ) {
	case PERSISTENT:
		{
			pANTLR3_BASE_TREE child = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, 0 )  );
			std::string name = getTreeText( child );

			std::string varName = name;
			SFC::CompoundStatement staticVarBlock = block;
			SFC::Function sfcFunction = SFCManager::getFunction( block );
			if ( sfcFunction != Udm::null ) {
				varName = static_cast< std::string >( sfcFunction.name() ) + "_" + varName;
				staticVarBlock = sfcFunction;
			}

			SFC::Class sfcClass = SFCManager::getClass( staticVarBlock );
			if ( sfcClass != Udm::null ) {
				varName = static_cast< std::string >( sfcClass.name() ) + "_" + varName;
				staticVarBlock = sfcClass;
			} else {
				SFC::Program program = SFCManager::getProgram( staticVarBlock );
				staticVarBlock = program;
			}

			SFC::LocalVar localVar = SFCManager::createLocalVar( staticVarBlock, varName );
			localVar.Static() = true;
			SFCManager::numberStatement( localVar );
			getSFCSymbolTable().push( name, localVar );
			getSFCSymbolTable().push( varName, localVar );

			std::string init_name = name + "_init";
			std::string init_varName = varName + "_init";
			SFC::LocalVar localVarInit = SFCManager::createLocalVar( staticVarBlock, init_varName );
			localVarInit.Static() = true;
			localVarInit.dt() = SFCTypesManager::getSingleton().getBasicType( "int" );
			SFCManager::numberStatement( localVarInit );
			getSFCSymbolTable().push( init_name, localVarInit );
			getSFCSymbolTable().push( init_varName, localVarInit );
			break;
		}
	case IF:
		{
			ifTree( tree, block );
			break;
		}
	case SWITCH:
		{
			switchTree( tree, block );
			break;
		}
	case WHILE:
		{
			whileTree( tree, block );
			break;
		}
	case FOR:
		{
			forTree( tree, block );
			break;
		}
	case BREAK:
		{
			SFC::UserCode userCode = SFC::UserCode::Create( block, SFC::CompoundStatement::meta_stmnt );
			SFCManager::numberStatement( userCode );
			userCode.expr() = "break";
			break;
		}
	case CONTINUE:
		{
			SFC::UserCode userCode = SFC::UserCode::Create( block, SFC::CompoundStatement::meta_stmnt );
			SFCManager::numberStatement( userCode );
			userCode.expr() = "continue";
			break;
		}
	case RETURN:
		{
			SFC::Return sfcReturn = SFC::Return::Create( block, SFC::CompoundStatement::meta_stmnt );
			SFCManager::numberStatement( sfcReturn );
			break;
		}
	case COMMENT:
		{
			SFC::Comment comment = SFC::Comment::Create( block, SFC::CompoundStatement::meta_stmnt );
			SFCManager::numberStatement( comment );
			std::string content = getTreeText( tree );
			if ( !content.empty() ) {
				if ( content[0] == '%' ) {
					content = content.substr( 1, content.size() - 1 );
					if ( content[ content.size() - 1 ] == '\n' ) content = content.substr( 0, content.size() - 1 );
				} else if (  content.substr( 0, 2 ) == "//"  ) {
					content = content.substr( 2, content.size() - 2 );
					if ( content[ content.size() - 1 ] == '\n' ) content = content.substr( 0, content.size() - 1 );
				} else if (  content.substr( 0, 2 ) == "/*"  ) {
					content = content.substr( 2, content.size() - 4 );
				}
			}
			comment.content() = content;
			break;
		}
	default:
		{
			exprTree( tree, block );
			break;
		}
	}
}

void ifTree( pANTLR3_BASE_TREE tree, SFC::CompoundStatement block ) {

	SFC::CompoundStatement conditionalGroupParent = block;
	SFC::CompoundStatement conditionalBlockParent = block;
	bool inCG = false;

	int noChildren = static_cast< int >(  tree->getChildCount( tree )  );
	for( int ix = 0 ; ix < noChildren ; ++ix ) {

		pANTLR3_BASE_TREE child = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, ix )  );

		int statementStart = 1;

		switch (  child->getType( child )  ) {
		case IF:
			{
				// THIS IS (POTENTIALLY) THE FINAL "ELSE" PART OF THE CONDITIONAL GROUP'S IF-ELSE-IF
				// (PROVIDED WE'RE IN A CONDITIONAL GROUP)
				if ( inCG ) {
					conditionalGroupParent = SFC::ConditionalBlock::Create( conditionalBlockParent );
					SFCManager::numberStatement( conditionalGroupParent );
				}

				__int64 statementCount = conditionalGroupParent.statementCount();
				pANTLR3_BASE_TREE grandChild = static_cast< pANTLR3_BASE_TREE >(  child->getChild( child, 0 )  );

				ExprReturnSP exprReturnSP = exprTree( grandChild, conditionalGroupParent )->collapseLogical();
				__int64 newStatementCount = conditionalGroupParent.statementCount();

				// SEE OF CONDITION OF CONDITIONAL GENERATED ANY STATEMENTS
				if ( newStatementCount != statementCount || !inCG ) {

					if ( ix < noChildren - 1 ) {
						conditionalBlockParent = SFC::ConditionalGroup::Create( conditionalGroupParent );
						SFCManager::numberStatement( conditionalBlockParent );
						inCG = true;
					};

					conditionalGroupParent = SFC::ConditionalBlock::Create( conditionalBlockParent );
					SFCManager::numberStatement( conditionalGroupParent );
				}

				exprReturnSP->buildUdm( conditionalGroupParent, SFC::ConditionalBlock::meta_cond );
				ReferenceReturn::StaticVariableAttributeSPList staticVariableAttributeSPList =
				 exprReturnSP->getAttributes< ReferenceReturn::StaticVariableAttribute >();

				for(
				 ReferenceReturn::StaticVariableAttributeSPList::iterator rslItr = staticVariableAttributeSPList.begin() ;
				 rslItr != staticVariableAttributeSPList.end() ;
				 ++rslItr
				) {
					Exprs exprs(   BE(  VE( (*rslItr)->getArgDeclBase() ), "=", IE( 1 )  )   );
					exprs.buildUdm( conditionalGroupParent, SFC::CompoundStatement::meta_stmnt );
				}
				break;
			}
		case ELSE:
			{
				conditionalGroupParent = SFC::ConditionalBlock::Create( conditionalBlockParent );
				SFCManager::numberStatement( conditionalGroupParent );
				statementStart = 0;
				break;
			}
		}

		int noGrandChildren = static_cast< int >(  child->getChildCount( child )  );
		for( int jx = statementStart ; jx < noGrandChildren ; ++jx ) {
			pANTLR3_BASE_TREE grandChild = static_cast< pANTLR3_BASE_TREE >(  child->getChild( child, jx )  );
			statementTree( grandChild, conditionalGroupParent );
		}
	}
}

void switchTree( pANTLR3_BASE_TREE tree, SFC::CompoundStatement block ) {

	SFC::CompoundStatement newBlock = block;

	int noChildren = static_cast< int >(  tree->getChildCount( tree )  ); 
	if ( noChildren > 3 ) {
		newBlock = SFC::ConditionalGroup::Create( block );
		SFCManager::numberStatement( newBlock );
	}

	pANTLR3_BASE_TREE switch_expr = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, 0 )  );

	for( int ix = 1 ; ix < noChildren ; (void)++ix ) {
		pANTLR3_BASE_TREE child = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, ix )  );

		makeCase( child, switch_expr, newBlock );
	}
}

void makeCase( pANTLR3_BASE_TREE tree, pANTLR3_BASE_TREE switch_expr, SFC::CompoundStatement block ) {
	SFC::ConditionalBlock conditionalBlock = SFC::ConditionalBlock::Create( block );
	SFCManager::numberStatement( conditionalBlock );

	int noChildren = static_cast< int >(  tree->getChildCount( tree )  );
	int statementStart = 1;

	switch (  tree->getType( tree )  ) {
	case CASE:
		{
			// THIS GETS THE 'EXPR' CHILD
			pANTLR3_BASE_TREE child = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, 0 )  );
			pANTLR3_BASE_TREE descendant = child;

			// THERE SHOULD ONLY BE ONE GRANDCHILD -- AN EXPR (EXPRESSION).
			// A CELL-EXPRESSION MEANS SEVERAL CONDITIONS.  OTHERWISE THERE IS ONLY ONE.
			pANTLR3_BASE_TREE grandChild = static_cast< pANTLR3_BASE_TREE >(  child->getChild( child, 0 )  );
			if (  grandChild->getType( grandChild ) == LBRACE  ) {
				descendant = grandChild;
			}

			int noExpressions = static_cast< int >(  descendant->getChildCount( descendant )  );

			pANTLR3_BASE_TREE expression = static_cast< pANTLR3_BASE_TREE >(  descendant->getChild( descendant, 0 )  );
			ExprReturnSP switchExprReturnSP = exprTree( switch_expr, block );
			ExprReturnSP caseExprReturnSP = exprTree( expression, block );
			ExprReturnSP exprReturnSP = switchExprReturnSP->combine( "==", caseExprReturnSP );

			for( int ix = 1 ; ix < noExpressions ; ++ix ) {
				expression = static_cast< pANTLR3_BASE_TREE >(  descendant->getChild( descendant, ix )  );
				switchExprReturnSP = exprTree( switch_expr, block );
				caseExprReturnSP = exprTree( expression, block );
				ExprReturnSP newExprReturnSP = switchExprReturnSP->combine( "==", caseExprReturnSP );
				exprReturnSP = exprReturnSP->combine( "||", newExprReturnSP );
			}
			exprReturnSP->collapseLogical();

			exprReturnSP->buildUdm( conditionalBlock, SFC::ConditionalBlock::meta_cond );
			break;
		}
	case OTHERWISE:
		{
			statementStart = 0;
			break;
		}
	}

	for( int ix = statementStart ; ix < noChildren ; (void)++ix ) {
		pANTLR3_BASE_TREE child = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, ix )  );
		statementTree( child, conditionalBlock );
	}
}

void whileTree( pANTLR3_BASE_TREE tree, SFC::CompoundStatement block ) {
	SFC::IterativeBlock iterativeBlock = SFC::IterativeBlock::Create( block );
	SFCManager::numberStatement( iterativeBlock );

	int noChildren = static_cast< int >(  tree->getChildCount( tree )  );

	pANTLR3_BASE_TREE firstChild = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, 0 )  );
	ExprReturnSP exprReturnSP = exprTree( firstChild, block )->collapseLogical();
	exprReturnSP->buildUdm( iterativeBlock, SFC::IterativeBlock::meta_cond );

	for( int ix = 1 ; ix < noChildren ; (void)++ix ) {
		pANTLR3_BASE_TREE child = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, ix )  );
		statementTree( child, iterativeBlock );
	}
}

void forTree( pANTLR3_BASE_TREE tree, SFC::CompoundStatement block ) {

	int noChildren = static_cast< int >(  tree->getChildCount( tree )  );

	pANTLR3_BASE_TREE firstChild = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, 0 )  );
	assert(  firstChild->getType( firstChild ) == IDENTIFIER  );

	std::string loopVarName = getTreeText( firstChild );

	pANTLR3_BASE_TREE secondChild = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, 1 )  );
	assert(  secondChild->getType( secondChild ) == COLON  );

	ExprReturnSP exprReturnSP = colonTree( secondChild, block );

	SFC::LocalVar indexVar = SFCManager::createUniqueLocalVar( block );
	indexVar.dt() = SFCTypesManager::getSingleton().getBasicType( "int" );
	Exprs exprs = BE(  VE( indexVar ), "=", IE( 0 )  );
	exprs.buildUdm( block, SFC::CompoundStatement::meta_stmnt );

	LitMatrixExprReturnSP litMatrixExprReturnSP;
	if ( exprReturnSP->getClassName() == LitMatrixExprReturn::getName() ) {
		litMatrixExprReturnSP = ExprReturn::static_pointer_cast< LitMatrixExprReturn >( exprReturnSP );
	}
	ColonExprReturnSP colonExprReturnSP;
	if ( exprReturnSP->getClassName() == ColonExprReturn::getName() ) {
		colonExprReturnSP = ExprReturn::static_pointer_cast< ColonExprReturn >( exprReturnSP );
	}

	SFC::IterativeBlock iterativeBlock = SFC::IterativeBlock::Create( block );
	SFCManager::numberStatement( iterativeBlock );

	if ( litMatrixExprReturnSP != 0 ) exprs = IE( 1 );
	else                              exprs = colonExprReturnSP->getDimExprs();
	exprs = BE(  VE( indexVar ), "<", exprs  );
	exprs.buildUdm( iterativeBlock, SFC::IterativeBlock::meta_cond );

	SFC::LocalVar loopVar = SFC::LocalVar::Create( iterativeBlock );
	loopVar.dt() = SFCTypesManager::getSingleton().getBasicType( "double" );
	loopVar.name() = loopVarName;
	SFCManager::numberStatement( loopVar );

	getSFCSymbolTable().push( loopVarName, loopVar );

	colonExprReturnSP->getExprsProxyVector().front().setExprs(  VE( indexVar )  );

	if ( litMatrixExprReturnSP != 0 ) exprs = DE(  litMatrixExprReturnSP->getItem( 0 )  );
	else                              exprs = colonExprReturnSP->getExprs();
	exprs = BE(  VE( loopVar ), "=", exprs  );
	exprs.buildUdm( iterativeBlock, SFC::CompoundStatement::meta_stmnt );

	for( int ix = 2 ; ix < noChildren ; (void)++ix ) {
		pANTLR3_BASE_TREE child = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, ix )  );
		statementTree( child, iterativeBlock );
	}

	exprs = UE(  "++", VE( indexVar )  );
	exprs.buildUdm( iterativeBlock, SFC::CompoundStatement::meta_stmnt );
}

ExprReturnSP exprTree( pANTLR3_BASE_TREE tree, SFC::CompoundStatement block ) {
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
			
			if (  expr->getType( expr ) == IDENTIFIER  ) {

				std::string name = getTreeText( expr );
				if ( name != "eps" ) {
					SFC::ArgDeclBase argDeclBase = getSFCSymbolTable().lookup( name );

					// "IDENTIFIER" IS ASSUMED TO BE A FUNCTION NAME IF IT IS NOT IN THE SYMBOL TABLE
					if (
					 argDeclBase == Udm::null &&
					 (
					  ExprReturn::getFunctionNameMap().find( name ) != ExprReturn::getFunctionNameMap().end() ||
					  MFileManager::get_singleton().hasMFileFunction( name )
					 )
					) {
						createExternalFunctionCall( reference, expr, block );
						return VarExprReturn::create( block );
					}
				}
			}

			std::string op = getTreeText( tree );

			switch(  reference->getType( reference )  ) {
			case MATRIX:
				{
					VarExprReturnSPVector varExprReturnSPVector = matrixReferenceTree( reference, block );
					ExprReturnSP exprReturnSP2 = exprTree( expr, block );

					__int64 rows, columns;
					exprReturnSP2->getRowsAndColumns( rows, columns );
					assert( rows == 1 || columns == 1 );
					int maxIndex = (int) max( rows, columns );
					assert( maxIndex == varExprReturnSPVector.size() );

					exprReturnSP2->assignTo( varExprReturnSPVector );

					break;
				}
			default:
				{
					ExprReturnSP exprReturnSP1 = referenceTree( reference, block, true );
					ExprReturnSP exprReturnSP2;

					// CHECK TO SEE OF "op=" OR PRE-INC(DEC)REMENT CAN BE USED.
					if ( op == "=" && exprReturnSP1->getClassName() != ReferenceReturn::getName() ) {

						// SEE IF RHS AST NODE HAS TWO CHILDREN (I.E. IS BINARY OPERATOR)
						int noChildren = static_cast< int >(  expr->getChildCount( expr )  );
						if ( noChildren == 2 ) {

							// SEE IF RHS AST NODE IS AN OPERATOR THAT HAS A "op=" ANALOG IN "C" (OR "Java")
							std::string op2 = getTreeText( expr );
							if ( op2 == "+" || op2 == "-" || op2 == ".*" || op2 == "./" ) {

								// SEE IF LHS AND FIRST CHILD OF RHS ARE IDENTICAL
								pANTLR3_BASE_TREE child1 =
								 static_cast< pANTLR3_BASE_TREE >(  expr->getChild( expr, 0 )  );
								if (  treeCompare( reference, child1 )  ) {

									// SEE IF SECOND CHILD OF RHS IS A LITERAL NUMBER
									pANTLR3_BASE_TREE child2 =
									 static_cast< pANTLR3_BASE_TREE >(  expr->getChild( expr, 1 )  );
									int child2type = child2->getType( child2 );
									if ( child2type == INTEGER || child2type == DOUBLE ) {

										// SEE IF SECOND CHILD OF RHS HAS VALUE "1"
										std::string child2text = getTreeText( child2 );
										double child2value = atof( child2text.c_str() );
										if ( child2value == 1 ) {

											// USE PRE-INC(DEC)REMENT OPERATORS IF OPERATOR IS "+" OR "-"
											if ( op2 == "+" ) {
												exprReturnSP1->applyUnaryOp( "++" )->collapse();
												return VarExprReturn::create( block );
											} else if ( op2 == "-" ) {
												exprReturnSP1->applyUnaryOp( "--" )->collapse();
												return VarExprReturn::create( block );
											}
										}
									}
								
									// USE "op=" ANALOG OF OPERATOR
									op = op2;
									if (  op.at( 0 ) == '.' ) op = op.substr( 1 );
									op += "=";
									exprReturnSP2 = exprTree( child2, block );
	
								}
							}
						}

					}

					// IF CANNOT USE "op=" OPERATOR, DO "=" OPERATION
					if ( exprReturnSP2 == 0 ) exprReturnSP2 = exprTree( expr, block );

					exprReturnSP1->combine( op, exprReturnSP2 )->collapse();
					break;
				}
			}

			return VarExprReturn::create( block );
		}

	case PSTINC:
	case PSTDEC:
		{
			std::string op = getTreeText( tree );
			pANTLR3_BASE_TREE reference = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, 0 )  );
			ExprReturnSP exprReturnSP = referenceTree( reference, block, true );
			exprReturnSP->applyUnaryOp( op )->collapse();
			return VarExprReturn::create( block );
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
			if      ( op == "~=" ) op = "!=";
			else if ( op == "&" )  op = "&&";
			else if ( op == "|" )  op = "||";

			pANTLR3_BASE_TREE expr1 = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, 0 )  );
			pANTLR3_BASE_TREE expr2 = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, 1 )  );

			ExprReturnSP exprReturnSP1 = exprTree( expr1, block );
			ExprReturnSP exprReturnSP2 = exprTree( expr2, block );

			return exprReturnSP1->combine( op, exprReturnSP2 );
		}

	case PLUS:
	case MINUS:
		{
			int noChildren = static_cast< int >(  tree->getChildCount( tree )  );
			std::string op = getTreeText( tree );

			if ( noChildren == 1 ) {
				pANTLR3_BASE_TREE expr = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, 0 )  );

				ExprReturnSP exprReturnSP = exprTree( expr, block );

				return exprReturnSP->applyUnaryOp( op );

			} else {
				pANTLR3_BASE_TREE expr1 = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, 0 )  );
				pANTLR3_BASE_TREE expr2 = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, 1 )  );

				ExprReturnSP exprReturnSP1 = exprTree( expr1, block );
				ExprReturnSP exprReturnSP2 = exprTree( expr2, block );

				return exprReturnSP1->combine( op, exprReturnSP2 );
			}
		}

	case COLON:
		{
			return colonTree( tree, block );
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
			bool isFunction = false;

			std::string op = getTreeText( tree );
			bool isElementWise = op.substr( 0, 1 ) == "." || MatLabStateflowFlag != 0;
			op = op.substr( op.size() - 1 ); // GET "*","/" OR "\" FROM ".*", "./", AND ".\", RESPECTIVELY.

			pANTLR3_BASE_TREE expr1 = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, 0 )  );
			pANTLR3_BASE_TREE expr2 = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, 1 )  );

			if ( op == "^" ) {
				op = "pow";
				isFunction = true;
			} else if ( op == "\\" ) {
				op = "/";
				std::swap( expr1, expr2 );
			}

			ExprReturnSP exprReturnSP1 = exprTree( expr1, block );
			ExprReturnSP exprReturnSP2 = exprTree( expr2, block );

			if ( op == "*" && !isElementWise ) return exprReturnSP1->multiply( exprReturnSP2 );
			if ( op == "/" && !isElementWise ) return exprReturnSP1->divide( exprReturnSP2 );

			return exprReturnSP1->combine( op, exprReturnSP2, isFunction );
		}

	case NOT:
	case CNOT:
		{
			std::string op = getTreeText( tree );
			if ( op == "~" ) op = "!";

			pANTLR3_BASE_TREE expr = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, 0 )  );

			ExprReturnSP exprReturnSP = exprTree( expr, block );

			return exprReturnSP->applyUnaryOp( op );
		}

	case CTRANSPOSE:
	case TRANSPOSE:
		{
			pANTLR3_BASE_TREE expr = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, 0 )  );

			ExprReturnSP exprReturnSP = exprTree( expr, block );

			return exprReturnSP->transpose();
		}

	case INTEGER:
		{
			std::string stringVal = getTreeText( tree );
			return LitMatrixExprReturn::create(  block, atoi( stringVal.c_str() )  );
		}

	case DOUBLE:
		{
			std::string stringVal = getTreeText( tree );
			return LitMatrixExprReturn::create(  block, atof( stringVal.c_str() )  );
		}

	case ZERO:
		{
			return LitMatrixExprReturn::create( block, 0 );
		}

	case TEXT:
		{
			std::string stringVal = getTreeText( tree );
			int substart = stringVal.at( 0 ) == '.' ? 2 : 1;
			stringVal = stringVal.substr( substart, stringVal.size() - 1 - substart );
			LitMatrixExprReturn::Sequence sequence;
			for( std::string::iterator stvItr = stringVal.begin() ; stvItr != stringVal.end() ; (void)++stvItr ) {
				sequence.push_back( *stvItr );
			}
			SFC::BasicType doubleType = SFCTypesManager::getSingleton().getBasicType( "double" );
			LitMatrixExprReturnSP litMatrixExprReturnSP = LitMatrixExprReturn::create( block, 1, (int) stringVal.size(), doubleType, sequence );
			litMatrixExprReturnSP->setIsText( true );
			return litMatrixExprReturnSP;
		}
	case IDENTIFIER:
	case DOT:
		{
			// SKIP "eml" STATEMENTS
			pANTLR3_BASE_TREE descendant = tree;
			while(  descendant->getType( descendant ) != IDENTIFIER  ) {
				descendant = static_cast< pANTLR3_BASE_TREE >(  descendant->getChild( descendant, 0 )  );
			}
			std::string descendantName = getTreeText( descendant );
			if ( descendantName == "eml" ) return ExprReturnSP();

			return referenceTree( tree, block, false );
		}

	case MATRIX:
		{
			return matrixTree( tree, block );
		}

	case END:
		{
			return LitMatrixExprReturn::create( block, ExprReturn::getEndStack().back() );
		}

	// LPAREN IS INCLUDED FOR CODE-REWRITING.  WITHOUT IT, THE
	// RE-WRITTEN CODE WOULD NOT KNOW WHERE TO PLACE PARENTHESES.
	case LPAREN:
		{
			pANTLR3_BASE_TREE expr = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, 0 )  );

			ExprReturnSP exprReturnSP = exprTree( expr, block );
			return exprReturnSP;
		}
	}

	std::cerr << "operator \"" << getTreeText( tree ) << "\" not currently supported." << std::endl;
	assert( false );

	return VarExprReturn::create( block );
}

ExprReturnSP colonTree( pANTLR3_BASE_TREE tree, SFC::CompoundStatement block ) {

	int noChildren = static_cast< int >(  tree->getChildCount( tree )  );

	ColonExprReturn::ExprReturnSPVector exprReturnSPVector;
	for( int ix = 0 ; ix < noChildren ; (void)++ix ) {
		pANTLR3_BASE_TREE child = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, ix )  );
		exprReturnSPVector.push_back(  exprTree( child, block )  );
	}

	for( int ix = 0 ; ix < exprReturnSPVector.size() ; (void)++ix ) {
		SFC::DT childDT = exprReturnSPVector[ ix ]->getDT();
		assert( childDT != Udm::null && childDT.type() != SFC::Array::meta );
	}

	return ColonExprReturn::create( block, exprReturnSPVector );
}

ExprReturnSP referenceTree( pANTLR3_BASE_TREE tree, SFC::CompoundStatement block, bool lvalue ) {
	switch(  tree->getType( tree )  ) {
	case IDENTIFIER:
		{
			return referenceItemTree( tree, block, lvalue );
		}
	case DOT:
		{
			pANTLR3_BASE_TREE reference1 = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, 0 )  );
			pANTLR3_BASE_TREE reference2 = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, 1 )  );

			ExprReturnSP exprReturnSP1 = referenceTree( reference1, block, lvalue );
			ExprReturnSP exprReturnSP2 = referenceItemTree( reference2, block, lvalue, exprReturnSP1 );

			VarExprReturnSP varExprReturnSP1 = ExprReturn::static_pointer_cast< VarExprReturn >( exprReturnSP1 );
			if ( exprReturnSP2->getClassName() == VarExprReturn::getName() ) {
				VarExprReturnSP varExprReturnSP2 = ExprReturn::static_pointer_cast< VarExprReturn >( exprReturnSP2 );
				return varExprReturnSP1->dotCombine( varExprReturnSP2 );
			}

			assert( exprReturnSP2->getClassName() == ReferenceReturn::getName() );
			ReferenceReturnSP referenceReturnSP =
			 ExprReturn::static_pointer_cast< ReferenceReturn >( exprReturnSP2 );

			return varExprReturnSP1->dotCombine( referenceReturnSP );
		}
	}

	assert( false );
	return ExprReturnSP();
}

ExprReturnSP referenceItemTree( pANTLR3_BASE_TREE tree, SFC::CompoundStatement block, bool lvalue, ExprReturnSP exprReturnSP ) {
	std::string name = getTreeText( tree );

	int noChildren = static_cast< int >(  tree->getChildCount( tree )  );
	pANTLR3_BASE_TREE child = 0;
	int noGrandChildren = 0;
	if ( noChildren > 0 ) {
		child = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, 0 )  );
		noGrandChildren = static_cast< int >(  child->getChildCount( child )  );
	}

	/* TEMPORARY KLUDGE FOR "IN" FUNCTION */
	if ( name == "in" && noGrandChildren == 1 ) {
		pANTLR3_BASE_TREE grandChild = static_cast< pANTLR3_BASE_TREE >(  child->getChild( child, 0 )  );
		FE fe = FE( "in" );
		fe.addArgVal(   SE(  getDeepTreeText( grandChild )  )   );
		Exprs exprs = fe;
		return VarExprReturn::create(  block, exprs, ExprsProxyVector(), SFCTypesManager::getSingleton().getBasicType( "boolean" )  );
	}

	PAntlr3BaseTreeList pAntlr3BaseTreeList;
	for( int ix = 0 ; ix < noGrandChildren ; ++ix ) {
		pANTLR3_BASE_TREE grandChild = static_cast< pANTLR3_BASE_TREE >(  child->getChild( child, ix )  );
		pAntlr3BaseTreeList.push_back( grandChild );
	}

	return ReferenceReturn::create( block, name, pAntlr3BaseTreeList, lvalue, exprReturnSP );
}

ArgReturnSP argTree( const pANTLR3_BASE_TREE tree, SFC::CompoundStatement block, bool lvalue ) {
	int nodeType = tree->getType( tree );
	switch( nodeType ) {
	case COLON:
		{
			return ArgReturn::create();
		}
	case DOT:
	case IDENTIFIER:
		{
			return ArgReturn::create(  referenceTree( tree, block, lvalue )  );
		}
	default:
		{
			return ArgReturn::create(  exprTree( tree, block )  );
		}
	}
}


ExprReturnSP matrixTree( pANTLR3_BASE_TREE tree, SFC::CompoundStatement block ) {

	MatrixExprReturnSP matrixExprReturnSP1 = LitMatrixExprReturn::create( block );

	int noChildren = static_cast< int >(  tree->getChildCount( tree )  );
	for( int ix = 0 ; ix < noChildren ; (void)++ix ) {

		pANTLR3_BASE_TREE child = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, ix )  );
		int noGrandChildren = static_cast< int >(  child->getChildCount( child )  );

		MatrixExprReturnSP matrixExprReturnSP2 = LitMatrixExprReturn::create( block );

		for( int jx = 0 ; jx < noGrandChildren ; (void)++jx ) {
			pANTLR3_BASE_TREE grandChild = static_cast< pANTLR3_BASE_TREE >(  child->getChild( child, jx )  );
			ExprReturnSP exprReturnSP = exprTree( grandChild, block );

			matrixExprReturnSP2 = matrixExprReturnSP2->appendColumns( exprReturnSP );
		}

		ExprReturnSP exprReturnSP2 = matrixExprReturnSP2;

		matrixExprReturnSP1 =
		 matrixExprReturnSP1->appendRows( exprReturnSP2 );
	}

	return matrixExprReturnSP1;
}


VarExprReturnSPVector matrixReferenceTree( pANTLR3_BASE_TREE tree, SFC::CompoundStatement block ) {

	VarExprReturnSPVector varExprReturnSPVector;
	int noChildren = static_cast< int >(  tree->getChildCount( tree )  );
	for( int ix = 0 ; ix < noChildren ; (void)++ix ) {
		pANTLR3_BASE_TREE child = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, ix )  );
		ExprReturnSP exprReturnSP = referenceTree( child, block, true );
		assert( exprReturnSP->getClassName() != LitMatrixExprReturn::getName() );
		varExprReturnSPVector.push_back(  ExprReturn::static_pointer_cast< VarExprReturn >( exprReturnSP )  );
	}

	return varExprReturnSPVector;

}

std::string getTreeText( pANTLR3_BASE_TREE tree ) {
	ANTLR3_STRING_struct *antlr3StringStruct = tree->getText( tree );
	return antlr3StringStruct == 0 ? "" : (char *)antlr3StringStruct->chars;
}

std::string getDeepTreeText( pANTLR3_BASE_TREE tree ) {
	
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
			for( int ix = 0 ; ix < noChildren ; (void)++ix ) {
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

void createExternalFunctionCall( pANTLR3_BASE_TREE reference, pANTLR3_BASE_TREE expr, SFC::CompoundStatement block ) {

	std::string name = getTreeText( expr );
	int noChildren = static_cast< int >(  expr->getChildCount( expr )  );

	ArgReturnSPList argReturnSPList;
	MFileManager::DTVector inputDTVector;

	if ( noChildren > 0 ) { // SHOULD BE 1
		pANTLR3_BASE_TREE child = static_cast< pANTLR3_BASE_TREE >(  expr->getChild( expr, 0 )  );

		int noGrandChildren = static_cast< int >(  child->getChildCount( child )  );
		for( int ix = 0 ; ix < noGrandChildren ; ++ix ) {
			pANTLR3_BASE_TREE grandChild = static_cast< pANTLR3_BASE_TREE >(  child->getChild( child, ix )  );
			ArgReturnSP argReturnSP = argTree( grandChild, block );

			SFC::DT inputDT = argReturnSP->getExprReturnSP()->getDT();
			if ( inputDT == Udm::null ) {
				std::cerr << "ERROR PROCESSING FUNCTION CALL TO \"" << name << "\":  INPUT ARGUMENT " << ix << " (0-BASED) HAS NO TYPE." << std::endl;
				return;
			}

			inputDTVector.push_back( inputDT );
			argReturnSPList.push_back( argReturnSP );
		}
	}

	PAntlr3BaseTreeList outputArgTreeList;
	MFileManager::DTVector outputDTVector;
	if (  reference->getType( reference ) == MATRIX  ) {

		noChildren = static_cast< int >(  reference->getChildCount( reference )  );

		for( int ix = 0 ; ix < noChildren ; ++ix ) {
			pANTLR3_BASE_TREE child = static_cast< pANTLR3_BASE_TREE >(  reference->getChild( reference, ix )  );
			ArgReturnSP argReturnSP = argTree( child, block, true );

			SFC::DT outputDT = argReturnSP->getExprReturnSP()->getDT();
			if ( outputDT == Udm::null ) {
				outputArgTreeList.push_back( child );
			}

			outputDTVector.push_back( outputDT );

			argReturnSPList.push_back( argReturnSP );
		}
		
	} else { // referenceType SHOULD BE "IDENTIFIER"

		ArgReturnSP argReturnSP = argTree( reference, block, true );

		SFC::DT outputDT = argReturnSP->getExprReturnSP()->getDT();
		if ( outputDT == Udm::null ) {
			outputArgTreeList.push_back( reference );
		}

		outputDTVector.push_back( outputDT );
		argReturnSPList.push_back( argReturnSP );
	}

	SFC::Function sfcFunction;
	ExprReturn::FunctionNameMap::iterator fnmItr = ExprReturn::getFunctionNameMap().find( name );
	if ( fnmItr != ExprReturn::getFunctionNameMap().end() ) {
		sfcFunction = fnmItr->second;
	} else {
		sfcFunction = MFileManager::get_singleton().getMFileFunction( name, inputDTVector, outputDTVector );
		if ( sfcFunction == Udm::null ) return;
	}
	FE fe( sfcFunction );


	PAntlr3BaseTreeList::iterator pblItr = outputArgTreeList.begin();
	SFC::ArgSet argSet = sfcFunction.Arg_children_sorted( SFC::ArgSorter() );
	SFC::ArgSet::iterator agsItr = argSet.begin();
	for( ArgReturnSPList::iterator aslItr = argReturnSPList.begin() ; aslItr != argReturnSPList.end() ; ++aslItr,++agsItr ) {

		SFC::Arg arg = *agsItr;
		ExprReturnSP exprReturnSP = (*aslItr)->getExprReturnSP();
		if ( arg.ptr() && exprReturnSP->getDT() == Udm::null ) {
			exprReturnSP->setDT( arg.dt() );
			assignType( *pblItr++, arg.dt() );
		}

		VarExprReturnSP varExprReturnSP = exprReturnSP->convertToVarExprReturn();
		fe.addArgVal( varExprReturnSP->getExprs() );
	}

	fe.buildUdm( block, SFC::CompoundStatement::meta_stmnt );
	return;
}

void assignType( pANTLR3_BASE_TREE reference, SFC::DT dt ) {

	SFC::ArgDeclBase argDeclBase = getArgDeclBase( reference );
	SFC::DT assignDT = argDeclBase.dt();
	if ( assignDT == Udm::null ) {
		argDeclBase.dt() = dt;
		return;
	}

	assert( assignDT == dt );
}

SFC::ArgDeclBase getArgDeclBase( pANTLR3_BASE_TREE tree ) {

	switch(  tree->getType( tree )  ) {
		case DOT: {
			pANTLR3_BASE_TREE leftTree = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, 0 )  );
			pANTLR3_BASE_TREE rightTree = static_cast< pANTLR3_BASE_TREE >(  tree->getChild( tree, 1 )  );

			SFC::ArgDeclBase argDeclBase = getArgDeclBase( leftTree );
			assert( argDeclBase != Udm::null );
			
			SFC::DT dt = argDeclBase.dt();
			assert( dt != Udm::null && dt.type() == SFC::Struct::meta );

			std::string memberName = getTreeText( rightTree );
			SFC::LocalVar localVar = findMember(  SFC::Struct::Cast( dt ), memberName  );
			assert( localVar != Udm::null );

			return localVar;
		}
		case IDENTIFIER: {
			std::string varName = getTreeText( tree );
			SFC::ArgDeclBase argDeclBase = getSFCSymbolTable().lookup( varName );
			assert( argDeclBase != Udm::null );
			return argDeclBase;
		}
	}

	return SFC::ArgDeclBase();
}

