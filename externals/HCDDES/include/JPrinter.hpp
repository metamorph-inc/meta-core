#ifndef ISIS_JPRINTER
#define ISIS_JPRINTER

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <set>
#include <map>
#include <vector>

#include "SFC.h"
#include "Uml.h"
#include "JTypeMap.hpp"
#include "SFCUdmEngine.hpp"
#include "Helper.hpp"

class JPrinter {
private:
	SFC::Program  _program;

	std::filebuf *_filebuf;
	std::ostream *_outputFile;

	std::string _indentChars;
	std::string _indent;

	std::string _globalsName;

	bool _sfcLegacy;
	std::string _className;
	std::string _mainFunctionName;
	std::string _initFunctionName;

	bool _jpf;
	bool _status;

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

	static int getUniqueNo( void ) {
		static int uniqueNo = -1;
		return ++uniqueNo;
	}

public:
	JPrinter( SFC::Program program, std::string indentChars = "  ", bool sfcLegacy = false );

 	JPrinter( std::string indentChars = "  " ) :
	 _filebuf( 0 ),
	 _outputFile( 0 ),
	 _indentChars( indentChars ),

	 _sfcLegacy( false ),
	 _jpf( false ),
	 _status( false )
	{ }

	 void setJPF( bool jpf ) { _jpf = jpf; }
	 bool getJPF( void ) { return _jpf; }

	 void setStatus( bool status ) { _status = status; }
	 bool getStatus( void) { return _status; }

	 void setOutputStream( std::ostream &os ) {
		 _outputFile = &os;
		 _outputFile->precision( 40 );
	 }

private:
	void openFile( const std::string &fileName ) {
		if ( _outputFile ) closeFile();
		_filebuf = new std::filebuf();
		_filebuf->open( fileName.c_str(), ios::out );
		_outputFile = new std::ostream( _filebuf );
		_outputFile->precision( 40 );
	}

	void closeFile( void ) {
		delete _outputFile;
		_outputFile = 0;
		if ( _filebuf ) {
			_filebuf->close();
			delete _filebuf;
			_filebuf = 0;
		}
	}

	std::string getGlobalsName( void ) { return _globalsName; }

	void increaseIndent( void ) { _indent += _indentChars; }
	void decreaseIndent( void ) { _indent = _indent.substr( 0, _indent.length() - _indentChars.length() ); }

	std::string getIndent( void ) const { return _indent; }
	void printIndent( void ) { (*_outputFile) << getIndent(); }

	typedef std::vector< std::string > StringVector;
	typedef std::map< std::string, std::string > StringMap;

	//
	// COLLECTIONS OF SFC CLASS OBJECTS
	//
	struct StatementSorter {
		bool operator()( const SFC::Statement &sl, const SFC::Statement &s2 ) {
			return static_cast< __int64 >( sl.statementIndex() ) < static_cast< __int64 >( s2.statementIndex() );
		}
	};

	typedef std::set< Udm::Object > ObjectSet;

	typedef std::set< SFC::ArgDeclRef >                         ArgDeclRefSet;
	typedef std::set< SFC::Class, StatementSorter >             ClassSet;
	typedef std::set< SFC::CompoundStatement, StatementSorter > CompoundStatementSet;
	typedef std::vector< SFC::CompoundStatement >               CompoundStatementVector;
	typedef std::set< SFC::Condition >                          ConditionSet;
	typedef std::set< SFC::ConditionalBlock, StatementSorter >  ConditionalBlockSet;
	typedef std::set< SFC::Declaration, StatementSorter >       DeclarationSet;
	typedef std::set< SFC::Exprs >                              ExprsSet;
	typedef std::set< SFC::Function, StatementSorter >          FunctionSet;
	typedef std::vector< SFC::Function >                        FunctionVector;
	typedef std::set< SFC::FunctionCall >                       FunctionCallSet;
	typedef std::set< SFC::LocalVar, StatementSorter >          LocalVarSet;
	typedef std::set< SFC::Program, StatementSorter >           ProgramSet;
	typedef std::set< SFC::SetVar >                             SetVarSet;
	typedef std::set< SFC::StateLabel, StatementSorter >        StateLabelSet;
	typedef std::set< SFC::Statement, StatementSorter >         StatementSet;
	typedef std::vector< SFC::Statement >                       StatementVector;
	typedef std::set< SFC::StateVar >                           StateVarSet;
	typedef std::set< SFC::Struct >                             StructSet;
	typedef std::set< SFC::UserCode, StatementSorter >          UserCodeSet;

	template< class ARG >
	struct ArgSetSorter {
		bool operator()( const ARG &arg1, const ARG &arg2 ) {
			return static_cast< __int64 >( arg1.argIndex() ) < static_cast< __int64 >( arg2.argIndex() );
		}
	};
	typedef ArgSetSorter< SFC::Arg > ArgSorter;
	typedef ArgSetSorter< SFC::ArgVal > ArgValSorter;

	typedef std::set< SFC::Arg, ArgSorter >       ArgSet;
	typedef std::set< SFC::ArgVal, ArgValSorter > ArgValSet;

	typedef std::set< SFC::Arg > UnorderedArgSet;
	static UnorderedArgSet &getCoveredArgSet( void ) {
		static UnorderedArgSet unorderedArgSet;
		return unorderedArgSet;
	}

	//
	// END COLLECTIONS OF SFC CLASSES
	//


	//
	// FOR ITERATION OVER A SET OF CHILDREN (IN THE SFC PARADIGM)
	//
	template< typename COLLECTION >
	class StatementIterator {
	private:
		JPrinter &_jPrinter;

		bool _notFirstSeparator;
		std::string _separatorChars;

		void print( const std::string &str ) { _jPrinter.print( str ); }
		void print( int val ) { _jPrinter.print( val ); }
		void print( double val ) { _jPrinter.print( val ); }
		void print( SFC::Arg arg ) { _jPrinter.printArg( arg ); }
		void print( SFC::ArgVal argVal ) { _jPrinter.printArgVal( argVal ); }
		void print( SFC::Class sfcClass ) { _jPrinter.printClass( sfcClass ); }
		void print( SFC::Condition condition ) { _jPrinter.printCondition( condition ); }
		void print( SFC::ConditionalBlock conditionalBlock ) { _jPrinter.printConditionalBlock( conditionalBlock ); }
		void print( SFC::Function function ) { _jPrinter.printFunction( function ); }
		void print( SFC::LocalVar localVar ) { _jPrinter.printLocalVar( localVar ); }
		void print( SFC::Statement statement ) { _jPrinter.printStatement( statement ); }
		void print( SFC::Struct sfcStruct ) { _jPrinter.printStruct( sfcStruct ); }
		void print( SFC::UserCode userCode ) { _jPrinter.printUserCode( userCode ); }

	public:
		StatementIterator( JPrinter &jPrinter, std::string separatorChars = "" ) :
		 _jPrinter( jPrinter ), _notFirstSeparator( false ), _separatorChars( separatorChars ) { }

		void operator()( const typename COLLECTION::value_type &statement ) {
			if ( _notFirstSeparator && !_separatorChars.empty() ) print( _separatorChars );
			else                                                  _notFirstSeparator = true;

			// HANDLE NON-BOOLEAN CONDITIONS
			bool isCondition = false;

			Udm::Object object = Udm::Object::Cast( statement );
			Udm::Object parent = object.GetParent();
			if ( parent.type() == SFC::ConditionalBlock::meta && object.type() == SFC::UserCode::meta ) {
				SFC::ConditionalBlock conditionalBlock = SFC::ConditionalBlock::Cast( parent );
				Udm::Object::CompositionInfo compositionInfo = { std::string( "cbPar" ), std::string( "cond" ) };
				ObjectSet objectSet = parent.GetChildObjects( compositionInfo, SFC::UserCode::meta );
				isCondition = objectSet.find( statement ) != objectSet.end();
			} else if ( parent.type() == SFC::IterativeBlock::meta && object.type() == SFC::UserCode::meta ) {
				SFC::IterativeBlock iterativeBlock = SFC::IterativeBlock::Cast( parent );
				Udm::Object::CompositionInfo compositionInfo = { std::string( "ibPar" ), std::string( "cond" ) };
				ObjectSet objectSet = parent.GetChildObjects( compositionInfo, SFC::UserCode::meta );
				isCondition = objectSet.find( statement ) != objectSet.end();
			}

			if ( isCondition ) {
				SFC::UserCode userCode = SFC::UserCode::Cast( statement );
				SFC::Exprs exprs = userCode.codeexpr();
				if (  JPrinter::getBaseType( exprs ) != "boolean"  ) {
					print( "(  ( " );
					print( userCode );
					print( " ) != 0  )" );
				} else {
					print( userCode );
				}
			} else {
				print( statement );
			}
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


public:
	void print( void );

private:
	void printClasses();
	void printNonClassStructs();

	//
	// BUILT-IN DATA TYPE PROCESSING
	//
public:
	void print( const std::string &str ) { if ( _outputFile != 0 ) (*_outputFile) << str; }
	void print( int val ) { if ( _outputFile != 0 ) (*_outputFile) << val; }
	void print( double val ) { if ( _outputFile != 0 ) (*_outputFile) << std::showpoint << val; }
	//
	// END BUILT-IN DATA TYPE PROCESSING
	//


	virtual void printArg( SFC::Arg arg );

	static std::string getBaseType( SFC::ArgDeclBase argDeclBase );
	void printWithoutDT( SFC::ArgDeclBase argDeclBase );
	void printWithDT( SFC::ArgDeclBase argDeclBase );
	void printQualifier( SFC::ArgDeclBase argDeclBase );

	virtual void printArgDeclRef( SFC::ArgDeclRef argDeclRef );
	bool isArgVal( SFC::ArgDeclRef argDeclRef );
	virtual void printArgVal( SFC::ArgVal argVal );
	virtual void printBinaryExprs( SFC::BinaryExprs binaryExprs );
	virtual void printCheckArg( SFC::CheckArg checkArg );
	virtual void printCheckState( SFC::CheckState checkState );
	virtual void printComment( SFC::Comment comment );

	virtual void printClass( SFC::Class sfcClass );
	static SFC::Class getClass( Udm::Object object ) {
		while( object != Udm::null && object.type() != SFC::Class::meta ) {
			object = object.GetParent();
		}
		return SFC::Class::Cast( object );
	}
	static std::string getClassName( SFC::Class sfcClass );
	static bool isNumber( const std::string &str ) {
		for( std::string::const_iterator cstItr = str.begin() ; cstItr != str.end() ; (void)++cstItr ) {
			if ( *cstItr < '0' || *cstItr > '9' ) return false;
		}
		return true;
	}
	void printClassFunctions( SFC::Class sfcClass ) {
		FunctionSet functionSet = sfcClass.Function_kind_children_sorted( StatementSorter() );
		iterate( functionSet );
	}
	bool isStubClass( SFC::Class sfcClass ) {
		StructSet structSet = sfcClass.Struct_kind_children();
		if ( structSet.empty() ) return false;
		SFC::Struct classStruct = *structSet.begin();
		return static_cast< __int64 >( classStruct.memberCount() ) == -1;
	}
	void printClassStaticVariables( SFC::Class sfcClass );
	void printClassStruct( SFC::Class sfcClass );
	static bool isStub( SFC::Class sfcClass ) {
		FunctionSet functionSet = sfcClass.Function_kind_children_sorted( StatementSorter() );
		SFC::Function function = *functionSet.begin();
		return function.statementCount() == -1;
	}

	template< typename ENTITY >
	static bool isClassEntity( ENTITY entity ) {
		if ( entity == Udm::null ) return false;

		Udm::Object parent = entity.GetParent();
		return parent.type() == SFC::Class::meta;
	}

	virtual void printCompoundStatement( SFC::CompoundStatement compoundStatement );
	void printStatements( SFC::CompoundStatement compoundStatement ) {
		StatementSet statementSet = compoundStatement.Statement_kind_children_sorted( StatementSorter() );
		iterate( statementSet, ";" );
	}
	virtual void printCompoundStatementBody( SFC::CompoundStatement compoundStatement );

public:
	virtual void printCondition( SFC::Condition condition );
	virtual bool printConditionalBlock( SFC::ConditionalBlock conditionalBlock );

private:
	virtual bool printConditionalBlockHeader( SFC::ConditionalBlock conditionalBlock );

	virtual void printConditionalGroup( SFC::ConditionalGroup conditionalGroup );

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
	virtual void printDeclaration( SFC::Declaration declaration );

	void printDouble( SFC::Double sfcDouble ) {
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
	virtual void printExprs( SFC::Exprs exprs );
	static std::string getBaseType( SFC::Exprs exprs );

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
		FunctionVector _functionVector;
	public:
		void operator()( SFC::Function function ) {
			if (  !isEmpty( function )  ) _functionVector.push_back( function );
		}

		FunctionVector &getFunctionVector( void ) { return _functionVector; }
	};

	FunctionVector getFunctions( void );

	virtual void printFunction( SFC::Function function ) {
		if (  isEmpty( function ) || !getStatus() && isStatusFunction( function )  ) return;
		printFunctionHeader( function );
		printCompoundStatementBody( function );
	}

	static bool isStatusFunction( SFC::Function function );
	static bool isMethod( SFC::Function function );

	std::string getFunctionName( SFC::Function function );
	void printFunctionType( SFC::DT dt );

private:
	template< typename TYPE >
	static TYPE getContainingEntity( SFC::Statement statement ) {
		while( statement != Udm::null ) {
			if ( statement.type() == TYPE::meta ) break;
			statement = SFC::Statement::Cast( statement.csPar() );
		}
		return TYPE::Cast( statement );
	}

	static SFC::Program getContainingProgram( SFC::Statement statement ) {
		return getContainingEntity< SFC::Program >( statement );
	}

	static SFC::Class getContainingClass( SFC::Statement statement ) {
		return getContainingEntity< SFC::Class >( statement );
	}

	static SFC::Function getContainingFunction( SFC::Statement statement ) {
		return getContainingEntity< SFC::Function >( statement );
	}

	class PrimaryFunctionFilter {
	private:
		JPrinter &_jPrinter;
		FunctionVector _functionVector;
		bool _reverse;

	public:
		PrimaryFunctionFilter( JPrinter &jPrinter, bool reverse = false ) : 
		 _jPrinter( jPrinter ), _reverse( reverse ) { }

		void operator()( SFC::Function function ) {
			FunctionCallSet functionCallSet = function.caller();

			SFC::Program program = _jPrinter.getProgram();
			for(
			 FunctionCallSet::iterator fcsItr = functionCallSet.begin() ;
			 fcsItr != functionCallSet.end() ;
			 (void)++fcsItr
			) {
				if ( getContainingProgram( *fcsItr ) == program  ) {
					if ( _reverse ) _functionVector.push_back( function );
					return;
				}
			}
			if ( !_reverse ) _functionVector.push_back( function );
		}

		FunctionVector &getFunctionVector( void ) { return _functionVector; }
	};

	class JPFAnnotationTypes {
	private:
		StringVector _annotationTypeVector;

	public:
		JPFAnnotationTypes( void ) {
			_annotationTypeVector.push_back( "invariant" );
			_annotationTypeVector.push_back( "ensures" );
			_annotationTypeVector.push_back( "requires" );
		}

		const StringVector &getAnnotationTypeVector( void ) { return _annotationTypeVector; }
	};

	static const StringVector &getAnnotationTypeVector( void ) {
		static JPFAnnotationTypes jpfAnnotationTypes;
		return jpfAnnotationTypes.getAnnotationTypeVector();
	}

	static StringVector getExpressionVector( const std::string &annotation, const std::string &type );
	static std::string normalizeWhitespace( const std::string &input );
	static StringMap getTranslationMap( SFC::Function function );
	static std::string makeSubstitutions( std::string expression, const StringMap &translationMap );

	void printAnnotations( SFC::Function function );

	virtual void printFunctionHeader( SFC::Function function );


public:
	//
	// END FUNCTION PROCESSING
	//

	typedef std::map< int, std::string > ArgValNoTempVarNameMap;
	typedef std::map< SFC::FunctionCall, ArgValNoTempVarNameMap > FunctionCall_AVNTVNM_Map;

	static FunctionCall_AVNTVNM_Map &getFunctionCall_AVNTVNM_Map( void ) {
		static FunctionCall_AVNTVNM_Map functionCall_AVNTVNM_Map;
		return functionCall_AVNTVNM_Map;
	}

	void generateTempArgVals( Udm::Object object );
	void assignFromTempArgVals( Udm::Object object );
	
	virtual void printFunctionCall( SFC::FunctionCall functionCall );

	void printInt( SFC::Int sfcInt ) {
		print(  static_cast< int >( sfcInt.val() )  );
	}
	virtual void printIterativeBlock( SFC::IterativeBlock iterativeBlock );

private:
	virtual void printIterativeBlockHeader( SFC::IterativeBlock iterativeBlock );


	//
	// LocalVar PROCESSING
	//
	void printLocalVars( void ) {
		LocalVarSet localVarSet = _program.LocalVar_kind_children_sorted( StatementSorter() );
		iterate( localVarSet );
	}

public:
	virtual void printLocalVar( SFC::LocalVar localVar );
	//
	// END LocalVar PROCESSING
	//

	SFC::Program getProgram( void ) { return _program; }

	static void printProject( SFC::Project project, bool pathfinder = false, bool generateStatusFunction = false );

	virtual void printReturn( SFC::Return sfcReturn );
	virtual void printSetState( SFC::SetState setState );


	//
	// SetVar PROCESSING
	//
private:
	void printLoopStarts( SFC::DT dt );
	void printLoopEnds( SFC::DT dt );
	void printIndexes( SFC::DT dt );

public:
	virtual void printSetVar( SFC::SetVar setVar );
	//
	// END SetVar PROCESSING
	//


	virtual void printStateLabel( SFC::StateLabel stateLabel );
	virtual void printStatement( SFC::Statement statement );

	static bool isEmpty( SFC::Statement statement );


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
	virtual void printStateVar( SFC::StateVar stateVar ) {
		int size = static_cast< int >( stateVar.size() );
		if ( size < 1 ) size = 1;
		print(  std::string( "public int " ) + static_cast< std::string >( stateVar.name() ) + "[] = new int [ " + boost::lexical_cast< std::string >( size ) + " ]"  );
	}

	void printStr( SFC::Str str ) {
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
			assert( structSet.size() == 1 );
			SFC::Struct sfcStruct = *structSet.begin();
			_structSet.insert( sfcStruct );
		}

		StructSet &getStructSet( void ) { return _structSet; }
	};

	void printStandardMacros( void );
	void printIfDefGuard( void );
	void printIfDefGuardEnd( void ) {
		print( "#endif" + getEndl() );
	}

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
	virtual void printStruct( SFC::Struct sfcStruct );

	static bool isClassStruct( SFC::Struct sfcStruct ) {
		return isClassEntity( sfcStruct );
	}
	static std::string getStructName( SFC::Struct sfcStruct );
	//
	// END Struct PROCESSING
	//


	virtual void printUnaryExprs( SFC::UnaryExprs unaryExprs );
	virtual void printUserCode( SFC::UserCode userCode );

private:

	class MyMath {
	public:
		typedef std::set< std::string > StringSet;
		
	private:
		std::ofstream _myMath;
		StringSet _definedFunctionSet;

	public:
		MyMath( void ) : _myMath( "MyMath.java" ) {
			_myMath << "class MyMath {" << std::endl;
		}
		~MyMath( void ) {
			_myMath << std::endl << "}" << std::endl;
			_myMath.close();
		}

		void makeFunction( const std::string &functionName ) {
			if (  _definedFunctionSet.find( functionName ) != _definedFunctionSet.end() ) {
				return;
			}
			_definedFunctionSet.insert( functionName );
			if ( functionName == "fmod" ) {
				_myMath <<
				 "\tstatic double fmod( double x, double y ) {" << std::endl <<
				 "\t\tif ( Double.isInfinite( x ) || y == 0 ) {" << std::endl <<
				 "\t\t\treturn Double.NaN;" << std::endl <<
				 "\t\t}" << std::endl <<
				 "\t\treturn x - y*(int)(x/y);" << std::endl <<
				 "\t}" << std::endl << std::endl;
			} else {
				std::cerr << "WARNING: MyMath:  no implementation exists for function \"" <<
				 functionName << "\"" << std::endl;
				_definedFunctionSet.erase( functionName );
			}
		}
		
	};

public:
	static MyMath &getMyMath( void ) {
		static MyMath myMath;
		return myMath;
	}
};

#endif
