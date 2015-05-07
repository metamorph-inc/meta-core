#ifndef ISIS_CPRINTER
#define ISIS_CPRINTER

#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <set>
#include <map>
#include <vector>

#include <boost/lexical_cast.hpp>

#include "SFC.h"
#include "Uml.h"
#include "CTypeMap.hpp"
#include "SFCUdmEngine.hpp"
#include "Helper.hpp"

class CPrinter {
private:
	SFC::Program  _program;

	std::string _headerFileName;
	std::ofstream _headerFile;

	std::string _sourceFileName;
	std::ofstream _sourceFile;

	std::ofstream *_outputFile;

	std::string _indentChars;
	std::string _indent;

	std::string _globalsName;

	bool _sfcLegacy;
	std::string _contextName;
	std::string _threadContextName;
	std::string _mainFunctionName;
	std::string _initFunctionName;


	class LineTerm {
	private:
		std::string endl;
	public:
		LineTerm( void ) {
			ostringstream oss;
			oss << std::endl;
			endl = oss.str();
		}
		std::string getEndl( void ) { return endl; }
	};
	static std::string getEndl( void ) {
		static LineTerm lineTerm;
		return lineTerm.getEndl();
	}


public:
	CPrinter( SFC::Program program, std::string indentChars = "  ", bool sfcLegacy = false ) :
	 _program( program ),
	 _headerFileName(   convertToIdentifierName(  static_cast< std::string >( program.filename() )  ) + ".h"   ),
	 _headerFile( _headerFileName.c_str() ),
	 _sourceFileName(   convertToIdentifierName(  static_cast< std::string >( program.filename() )  ) + ".c"   ),
	 _sourceFile( _sourceFileName.c_str() ),
	 _outputFile( &_sourceFile ),
	 _indentChars( indentChars ),

	 _sfcLegacy( sfcLegacy )
	{
		SFC::Project project = SFC::Project::Cast( program.GetParent() );
		_globalsName = convertToIdentifierName( project.name() ) + "_globals";

		if ( _sfcLegacy ) {
			std::string filename = program.filename();
			_contextName = filename + "_context";
			_threadContextName = filename + "_thread_context";
			_mainFunctionName = filename + "_main";
			_initFunctionName = filename + "_init";
		}
		_sourceFile.precision( 40 );
		_headerFile.precision( 40 );
	}

	CPrinter( std::string filenameBase, std::string indentChars = "  " ) :
	 _headerFileName(  convertToIdentifierName( filenameBase ) + ".h"  ),
	 _headerFile( _headerFileName.c_str() ),
	 _sourceFileName(  convertToIdentifierName( filenameBase ) + ".c"  ),
	 _outputFile( &_headerFile ),
	 _indentChars( indentChars ),

	 _sfcLegacy( false )
	{
		_sourceFile.precision( 40 );
		_headerFile.precision( 40 );
	}

private:
	std::string getHeaderFileName( void ) { return _headerFileName; }
	std::string getSourceFileName( void ) { return _sourceFileName; }

	std::string getGlobalsName( void ) { return _globalsName; }
	void outputToHeader( void ) {
		if ( !_headerFile.is_open() ) _headerFile.open( _headerFileName.c_str(), ios_base::app );
		_outputFile = &_headerFile;
	}
	void outputToSource( void ) {
		if ( !_sourceFile.is_open() ) _sourceFile.open( _sourceFileName.c_str(), ios_base::app );
		_outputFile = &_sourceFile;
	}

	void increaseIndent( void ) { _indent += _indentChars; }
	void decreaseIndent( void ) { _indent = _indent.substr( 0, _indent.length() - _indentChars.length() ); }

	std::string getIndent( void ) const { return _indent; }
	void printIndent( void ) { (*_outputFile) << getIndent(); }


	//
	// COLLECTIONS OF SFC CLASS OBJECTS
	//
	struct StatementSorter {
		bool operator()( const SFC::Statement &sl, const SFC::Statement &s2 ) {
			return static_cast< __int64 >( sl.statementIndex() ) < static_cast< __int64 >( s2.statementIndex() );
		}
	};

	typedef std::set< SFC::Class, StatementSorter >             ClassSet;
	typedef std::set< SFC::CompoundStatement, StatementSorter > CompoundStatementSet;
	typedef std::vector< SFC::CompoundStatement >               CompoundStatementVector;
	typedef std::set< SFC::Condition >                          ConditionSet;
	typedef std::set< SFC::ConditionalBlock, StatementSorter >  ConditionalBlockSet;
	typedef std::set< SFC::Declaration, StatementSorter >       DeclarationSet;
	typedef std::set< SFC::DT >                                 DTSet;
	typedef std::set< SFC::Function, StatementSorter >          FunctionSet;
	typedef std::vector< SFC::Function >                        FunctionVector;
	typedef std::set< SFC::FunctionCall >                       FunctionCallSet;
	typedef std::set< SFC::LocalVar, StatementSorter >          LocalVarSet;
	typedef std::set< Udm::Object >                             ObjectSet;
	typedef std::set< SFC::Program, StatementSorter >           ProgramSet;
	typedef std::set< SFC::SetVar >                             SetVarSet;
	typedef std::set< SFC::StateLabel, StatementSorter >        StateLabelSet;
	typedef std::set< SFC::Statement, StatementSorter >         StatementSet;
	typedef std::set< SFC::Statement >                          UnorderedStatementSet;
	typedef std::vector< SFC::Statement >                       StatementVector;
	typedef std::set< SFC::StateVar >                           StateVarSet;
	typedef std::set< SFC::Struct >                             StructSet;
	typedef std::set< SFC::TypedEntity >                        TypedEntitySet;
	typedef std::set< SFC::UserCode, StatementSorter >          UserCodeSet;

	template< class ARG >
	struct ArgListSorter {
		bool operator()( const ARG &arg1, const ARG &arg2 ) {
			return static_cast< __int64 >( arg1.argIndex() ) < static_cast< __int64 >( arg2.argIndex() );
		}
	};
	typedef ArgListSorter< SFC::Arg > ArgSorter;
	typedef ArgListSorter< SFC::ArgVal > ArgValSorter;

	typedef std::set< SFC::Arg, ArgSorter >       ArgSet;
	typedef std::set< SFC::ArgVal, ArgValSorter > ArgValSet;
	//
	// END COLLECTIONS OF SFC CLASSES
	//


	typedef std::map< SFC::Statement, UnorderedStatementSet > IncludeMap;

	static IncludeMap &getIncludeMap( void ) {
		static IncludeMap includeMap;
		return includeMap;
	}

	static void addStatementToIncludeMap( SFC::Struct sfcStruct, SFC::Statement statement );
	static void addStructToIncludeMap( SFC::Struct sfcStruct ) {
		addStatementToIncludeMap( sfcStruct, sfcStruct );
	}
	static SFC::Statement getLocalVarContainer( SFC::LocalVar localVar );
	void printIncludes( SFC::Statement statement );
	void printIncludes( void ) { printIncludes( getProgram() ); }
	

public:
	static void makeIncludeMap( SFC::Project project );
	static void makeStructFiles( SFC::Project project );

	//
	// FOR ITERATION OVER A SET OF CHILDREN (IN THE SFC PARADIGM)
	//
	template< typename COLLECTION >
	class StatementIterator {
	private:
		CPrinter &_cPrinter;

		bool _notFirstSeparator;
		std::string _separatorChars;

	public:
		StatementIterator( CPrinter &cPrinter, std::string separatorChars = "" ) :
		 _cPrinter( cPrinter ), _notFirstSeparator( false ), _separatorChars( separatorChars ) { }

		void operator()( const typename COLLECTION::value_type &statement ) {
			if ( _notFirstSeparator ) _cPrinter.print( _separatorChars );
			else                      _notFirstSeparator = true;
			_cPrinter.print( statement );
		}
	};

	template< typename SET >
	void iterate( SET &aSet, std::string separatorChars = "" ) {
		std::for_each(  aSet.begin(), aSet.end(), StatementIterator< SET >( *this, separatorChars )  );
	}
	//
	// END "FOR ITERATION ..."
	//

	class IOLocalVarFilter {
	private:
		LocalVarSet _inputLocalVarSet;
		LocalVarSet _outputLocalVarSet;
		LocalVarSet _otherLocalVarSet;
		LocalVarSet _staticLocalVarSet;

	public:
		void operator()( SFC::LocalVar localVar ) {
			SetVarSet setVarSet = localVar.arg();
			if ( !setVarSet.empty() ) {
				SFC::SetVar setVar = *setVarSet.begin();
				if ( setVar.invert() ) _outputLocalVarSet.insert( localVar );
				else                   _inputLocalVarSet.insert( localVar );
			} else if ( localVar.Static() ) {
				_staticLocalVarSet.insert( localVar );
			} else {
				_otherLocalVarSet.insert( localVar );
			}
		}

		LocalVarSet &getInputLocalVarSet( void ) { return _inputLocalVarSet; }
		LocalVarSet &getOutputLocalVarSet( void ) { return _outputLocalVarSet; }
		LocalVarSet &getStaticLocalVarSet( void ) { return _staticLocalVarSet; }
		LocalVarSet &getOtherLocalVarSet( void ) { return _otherLocalVarSet; }
	};

	class StatementTranslator {
	private:
		StatementVector _statementVector;
	public:
		void operator()( SFC::Statement statement ) {
			_statementVector.push_back( statement );
		}
		StatementVector &getStatementVector( void ) { return _statementVector; }
	};

	class CompoundStatementTranslator {
	private:
		CompoundStatementVector _compoundStatementVector;
	public:
		void operator()( SFC::CompoundStatement compoundStatement ) {
			_compoundStatementVector.push_back( compoundStatement );
		}
		CompoundStatementVector &getCompoundStatementVector( void ) { return _compoundStatementVector; }
	};


	//
	// StructNameMap
	//
	typedef std::map< SFC::Struct, std::string > StructNameMap;

	static StructNameMap &getStructNameMap( void ) {
		static StructNameMap structNameMap;
		return structNameMap;
	}

	static bool structProcessed( SFC::Struct sfcStruct ) {
		return getStructNameMap().find( sfcStruct ) != getStructNameMap().end();
	}

	static std::string getStructName( SFC::Struct sfcStruct );
	//
	// END StructNameMap
	//

public:
	void print( void );

private:
	void printHeaderFileInclude( void ) {
		print( "#include \"" + getHeaderFileName() + "\"" + getEndl() + getEndl() + getEndl() );
	}

	void printProgramUsercode();

public:
	void print( const std::string &str ) { (*_outputFile) << str << flush; }
	void print( int val ) { (*_outputFile) << val; }
	void print( double val ) { (*_outputFile) << std::showpoint << val; }
	//
	// END BUILT-IN DATA TYPE PROCESSING
	//


	void print( SFC::Arg arg );
	void print( SFC::ArgDeclRef argDeclRef );
	void print( SFC::ArgVal argVal );
	void print( SFC::BinaryExprs binaryExprs );
	void print( SFC::CheckArg checkArg );
	void print( SFC::CheckState checkState );
	void print( SFC::Comment comment );

	void print( SFC::CompoundStatement compoundStatement );
	void printCompoundStatement( SFC::CompoundStatement compoundStatement );

public:
	void print( SFC::Condition condition );
	void print( SFC::ConditionalBlock conditionalBlock );

private:
	void printConditionalBlockHeader( SFC::ConditionalBlock conditionalBlock );
	void printConditionalGroup( SFC::ConditionalGroup conditionalGroup );

	void printDeclarations( void );

	class DeclarationFilter {
	private:
		StatementSet _statementSet;
	public:
		void operator()( SFC::Statement statement ) {
			if (  !Udm::IsDerivedFrom( statement.type(), SFC::Declaration::meta )  ) {
				_statementSet.insert( statement );
			}
		}
		StatementSet getStatementSet( void ) { return _statementSet; }
	};

public:
	void print( SFC::Declaration declaration );

	void print( SFC::Double sfcDouble ) {
		print( sfcDouble.val() );
	}

private:
	class DTReverseFilter {
	private:
		DeclarationSet _declarationSet;
	public:
		void operator()( SFC::Declaration declaration ) {
			if (  !Udm::IsDerivedFrom( declaration.type(), SFC::DT::meta )  ) {
				_declarationSet.insert( declaration );
			}
		}
		DeclarationSet getDeclarationSet( void ) { return _declarationSet; }
	};

public:
	void printWithoutDT( SFC::ArgDeclBase argDeclBase );
	void printWithDT( SFC::ArgDeclBase argDeclBase );
	void printVar( SFC::DT dt, const std::string &varName );

	void print( SFC::Exprs exprs );


	//
	// FUNCTION PROCESSING
	//
	class ClassFunctions {
	private:
		FunctionVector _functionVector;
	public:
		void operator()( SFC::Class sfcClass ) {
			FunctionSet functionSet = sfcClass.Function_kind_children_sorted( StatementSorter() );
			_functionVector.insert( _functionVector.end(), functionSet.begin(), functionSet.end() );
		}

		FunctionVector &getFunctionVector( void ) { return _functionVector; }
	};

	class EmptyFunctionFilter {
	private:
		CPrinter &_cPrinter;
		FunctionVector _functionVector;
	public:
		EmptyFunctionFilter( CPrinter &cPrinter ) : _cPrinter( cPrinter ) { }
		void operator()( SFC::Function function ) {
			if (  !isEmpty( function ) || _cPrinter.isPrimaryFunction( function  )  ) {
				_functionVector.push_back( function );
			}
		}

		FunctionVector &getFunctionVector( void ) { return _functionVector; }
	};

	FunctionVector getFunctions( void );

	void printFunction( SFC::Function function );
	
	bool isStatusFunction( SFC::Function function );

private:
	void printFunctionHeader( SFC::Function function );

	static SFC::Program getContainingProgram( SFC::Statement statement ) {
		while( statement != Udm::null ) {
			if ( statement.type() == SFC::Program::meta ) break;
			statement = SFC::Statement::Cast( statement.csPar() );
		}
		return SFC::Program::Cast( statement );
	}

	bool isPrimaryFunction( SFC::Function function );

	class PrimaryFunctionFilter {
	private:
		CPrinter &_cPrinter;
		FunctionVector _functionVector;
		bool _reverse;

	public:
		PrimaryFunctionFilter( CPrinter &cPrinter, bool reverse = false ) : 
		 _cPrinter( cPrinter ), _reverse( reverse ) { }

		void operator()( SFC::Function function ) {
			bool ipf = _cPrinter.isPrimaryFunction( function );
			if ( ipf && !_reverse || !ipf && _reverse ) _functionVector.push_back( function );
		}

		FunctionVector &getFunctionVector( void ) { return _functionVector; }
	};

	std::string getSimplifiedName( const std::string &complexName );

	void printPrimaryFunctionHeaders( void );
	void printSimplifiedPrimaryFunctionHeaders( void );
	void printFunctionHeaders( void );

	void printSimplifiedPrimaryFunctions( void );
	void printFunctions( void ) {
		FunctionVector functionVector = getFunctions();

		CompoundStatementVector compoundStatementVector =
		 for_each( functionVector.begin(), functionVector.end(), CompoundStatementTranslator() ).
		  getCompoundStatementVector();

		iterate( compoundStatementVector );
	}

public:
	void print( SFC::Function function ) {

		if (  isEmpty( function ) && !isPrimaryFunction( function ) || isStatusFunction( function )  ) return;

		if (  static_cast< __int64 >( function.statementCount() ) < 0  ) return;
		printIndent();
		printFunctionHeader( function );
		print(  std::string( ";" ) + getEndl()  );
	}
	void printFunctionType( SFC::DT dt );
	//
	// END FUNCTION PROCESSING
	//


	void print( SFC::FunctionCall functionCall );


	void print( SFC::Int sfcInt ) {
		print(  static_cast< int >( sfcInt.val() )  );
	}
	void print( SFC::IterativeBlock iterativeBlock );

private:
	void printIterativeBlockHeader( SFC::IterativeBlock iterativeBlock );


	//
	// LocalVar PROCESSING
	//
	void printLocalVars( void ) {
		LocalVarSet localVarSet = _program.LocalVar_kind_children_sorted( StatementSorter() );
		iterate( localVarSet );
	}

public:
	void print( SFC::LocalVar localVar );
	//
	// END LocalVar PROCESSING
	//

	SFC::Program getProgram( void ) { return _program; }

	static void printGlobals( SFC::Project project );
	static void print( SFC::Project project );

	void print( SFC::Return sfcReturn ) {
		print( "return" );
		SFC::Exprs retexpr = sfcReturn.retexpr();
		if ( retexpr != Udm::null ) {
			print( " " );
			print( retexpr );
		} else {
			std::string val = sfcReturn.val();
			if ( !val.empty() ) print( std::string( " " ) + val );
		}
	}


	void print( SFC::SetState setState );


	//
	// SetVar PROCESSING
	//
private:
	void printLoopStarts( SFC::DT dt );
	void printLoopEnds( SFC::DT dt );
	void printIndexes( SFC::DT dt );

public:
	void print( SFC::SetVar setVar );
	//
	// END SetVar PROCESSING
	//


	void print( SFC::StateLabel stateLabel );

	static bool isEmpty( SFC::Statement statement );
	void print( SFC::Statement statement );


private:
	template< typename SFC_TYPE >
	class EmptyFilter {
	private:
		typedef std::set< SFC_TYPE, StatementSorter > SFC_SET;
		SFC_SET _statementSet;

	public:
		void operator()( SFC_TYPE statement ) {
			if (  !isEmpty( statement )  ) _statementSet.insert( statement );
		}
		SFC_SET getStatementSet( void ) { return _statementSet; }
	};

	typedef EmptyFilter< SFC::ConditionalBlock > EmptyConditionalBlockFilter;
	typedef EmptyFilter< SFC::Declaration > EmptyDeclarationFilter;

public:
	void print( SFC::StateVar stateVar ) {
		__int64 size = stateVar.size();
		if ( size < 1 ) size = 1;
		print(  std::string( "int " ) + convertToIdentifierName( stateVar.name() ) + "[ " + boost::lexical_cast< std::string >( size ) + " ]"  );
	}

	void print( SFC::Str str ) {
		print(  std::string( "\"" ) + static_cast< std::string >( str.val() ) + "\""  );
	}


	//
	// Struct PROCESSING
	//
private:
	class ClassStruct {
	private:
		StructSet _structSet;
	public:
		void operator()( SFC::Class sfcClass ) {
			StructSet structSet = sfcClass.Struct_kind_children();
			assert( structSet.size() <= 1 );
			if ( !structSet.empty() ) {
				SFC::Struct sfcStruct = *structSet.begin();
				_structSet.insert( sfcStruct );
			}
		}

		StructSet &getStructSet( void ) { return _structSet; }
	};

	void printStandardMacros( void );
	void printIfDefGuard( void );
	void printIfDefGuardEnd( void ) {
		print( "#endif" + getEndl() );
	}
	void printTypedefs( void );

	typedef std::set< std::string > StringSet;

	class BuiltInTypeNamesInit {
	private:
		StringSet _builtInTypeNames;
	public:
		BuiltInTypeNamesInit( void ) {
			_builtInTypeNames.insert( "char" );
			_builtInTypeNames.insert( "short" );
			_builtInTypeNames.insert( "int" );
			_builtInTypeNames.insert( "long" );
			_builtInTypeNames.insert( "float" );
			_builtInTypeNames.insert( "double" );
			_builtInTypeNames.insert( "void" );
		}
		StringSet &getBuiltInTypeNames( void ) { return _builtInTypeNames; }
	};

	static StringSet &getBuiltInTypeNames( void ) {
		static BuiltInTypeNamesInit builtInTypeNamesInit;
		return builtInTypeNamesInit.getBuiltInTypeNames();
	}

	static bool isBuiltInTypeName( const std::string &typeName ) {
		return getBuiltInTypeNames().find( typeName ) != getBuiltInTypeNames().end();
	}

	std::string getBasicCTypeName( std::string );
	void printClassStaticVariables( void );
	void printClassStructs( void );

	class StructFilter {
	private:
		StructSet _structSet;
	public:
		void operator()( SFC::Declaration declaration ) {
			SFC::DT dt = declaration.dt();
			if ( dt == Udm::null || dt.type() != SFC::Struct::meta ) return;
			SFC::Struct sfcStruct = SFC::Struct::Cast( dt );
			_structSet.insert( sfcStruct );
		}

		StructSet &getStructSet( void ) { return _structSet; }
	};

	void printStructs( void );

public:
	void print( SFC::Struct sfcStruct );
	//
	// END Struct PROCESSING
	//


	void print( SFC::UnaryExprs unaryExprs );


	void print( SFC::UserCode userCode ) {
		SFC::Exprs codeexpr = userCode.codeexpr();
		if ( codeexpr != Udm::null ) {
			print( codeexpr );
		} else {
			std::string expr = userCode.expr();
			if ( !expr.empty() ) print( expr );
		}
	}
};

#endif
