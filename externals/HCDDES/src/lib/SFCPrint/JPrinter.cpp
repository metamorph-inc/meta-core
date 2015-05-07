#include <boost/regex.hpp>

#include "JPrinter.hpp"
#include "OpPred.hpp"

JPrinter::JPrinter( SFC::Program program, std::string indentChars, bool sfcLegacy ) :
	_program( program ),
	_filebuf( 0 ),
	_outputFile( 0 ),
	_indentChars( indentChars ),

	_sfcLegacy( sfcLegacy ),
	_jpf( false ),
	_status( false )
{
	SFC::Project project = SFC::Project::Cast( program.GetParent() );

	_globalsName = convertToIdentifierName( project.name() ) + "_globals";

	if ( _sfcLegacy ) {
		std::string filename = program.filename();
		_className = program.filename();
		_mainFunctionName = filename + "_main";
		_initFunctionName = filename + "_init";
	}
}

void JPrinter::printProject( SFC::Project project, bool pathfinder, bool generateStatusFunction ) {
	std::string globalsName = convertToIdentifierName( project.name() ) + "_globals";

	bool sfcLegacy = false;

	ProgramSet programSet = project.Program_kind_children_sorted( StatementSorter() );
	for( ProgramSet::iterator pgsItr = programSet.begin() ; !sfcLegacy && pgsItr != programSet.end() ; ++pgsItr ) {
		sfcLegacy = static_cast< __int64 >( pgsItr->statementCount() ) < 0;
	}
	if ( !sfcLegacy ) {
		LocalVarSet localVarSet = project.LocalVar_kind_children_sorted( StatementSorter() );
		if ( !localVarSet.empty() ) {
			std::string className = globalsName;
			JPrinter globalsPrinter;
			globalsPrinter.openFile( className + ".java" );
			globalsPrinter.print( globalsPrinter.getIndent() + "public class " + className + " {" + getEndl() );
			globalsPrinter.increaseIndent();
			globalsPrinter.print( globalsPrinter.getIndent() + "public static " );
			globalsPrinter.iterate(  localVarSet, std::string( ";" ) + getEndl() + globalsPrinter.getIndent() + "public static "  );
			globalsPrinter.print( std::string( ";" ) + getEndl() );
			globalsPrinter.decreaseIndent();
			globalsPrinter.print( globalsPrinter.getIndent() + "} " + getEndl() );
			globalsPrinter.closeFile();
		}
	}

	for( ProgramSet::iterator pgsItr = programSet.begin() ; pgsItr != programSet.end() ; ++pgsItr ) {
		sfcLegacy = static_cast< __int64 >( pgsItr->statementCount() ) < 0;
		JPrinter jPrinter( *pgsItr, "  ", sfcLegacy );
		jPrinter.setJPF( pathfinder );
		jPrinter.setStatus( generateStatusFunction );
		jPrinter.print();
	}
}


void JPrinter::print( void ) {
	printClasses();
	printNonClassStructs();
}


void JPrinter::printClasses( void ) {

	if ( _sfcLegacy ) {

		LocalVarSet localVarSet = _program.LocalVar_kind_children_sorted( StatementSorter() );

		IOLocalVarFilter ioLocalVarFilter = for_each( localVarSet.begin(), localVarSet.end(), IOLocalVarFilter() );
		LocalVarSet inputLocalVarSet  = ioLocalVarFilter.getInputLocalVarSet();
		LocalVarSet outputLocalVarSet = ioLocalVarFilter.getOutputLocalVarSet();
		LocalVarSet staticLocalVarSet  = ioLocalVarFilter.getStaticLocalVarSet();
		LocalVarSet otherLocalVarSet  = ioLocalVarFilter.getOtherLocalVarSet();

		StateVarSet stateVarSet = _program.StateVar_kind_children();

		openFile( _className + ".java" );
		print( "public class " + _className + " {" + getEndl() );
		increaseIndent();

		if ( !staticLocalVarSet.empty() ) {
			print( getIndent() + "private static " );
			iterate(  staticLocalVarSet, std::string( ";" ) + getEndl() + getIndent() + "private static "  );
			print( std::string( ";" ) + getEndl() );
			print( getEndl() );
		}

		if ( !stateVarSet.empty() ) {
			// INSERT STATEVAR(S)
			StatementVector statementVector = for_each(
			 stateVarSet.begin(), stateVarSet.end(), StatementTranslator()
			).getStatementVector();
			iterate( statementVector );

			// INSERT OUTPUT VARIABLE(S)
			statementVector = for_each(
			 outputLocalVarSet.begin(), outputLocalVarSet.end(), StatementTranslator()
			).getStatementVector();
			iterate( statementVector );

			// INSERT OTHER VARIABLE(S)
			statementVector = for_each(
			 otherLocalVarSet.begin(), otherLocalVarSet.end(), StatementTranslator()
			).getStatementVector();
			iterate( statementVector );
		}

		if ( !stateVarSet.empty() || !localVarSet.empty() ) {

			StatementVector statementVector = for_each(
			 inputLocalVarSet.begin(), inputLocalVarSet.end(), StatementTranslator()
			).getStatementVector();
			iterate( statementVector );

			print( getEndl() );

			StateLabelSet stateLabelSet = _program.StateLabel_kind_children_sorted( StatementSorter() );
			statementVector =
			 for_each( stateLabelSet.begin(), stateLabelSet.end(), StatementTranslator() ).getStatementVector();
			iterate( statementVector );

			print( getEndl() );
		}

		FunctionSet functionSet = _program.Function_kind_children_sorted( StatementSorter() );
		iterate( functionSet );

		decreaseIndent();
		print(  std::string( "}" ) + getEndl()  );

	} else {

		ClassSet classSet = _program.Class_kind_children_sorted( StatementSorter() );
		iterate( classSet );

	}

	closeFile();
}

void JPrinter::printClassStaticVariables( SFC::Class sfcClass ) {

	LocalVarSet localVarSet = sfcClass.LocalVar_kind_children_sorted( StatementSorter() );
	if ( localVarSet.empty() ) return;
	print( getIndent() + "private static " );
	iterate(  localVarSet, std::string( ";" ) + getEndl() + getIndent() + "private static "  );
	print( std::string( ";" ) + getEndl() );

	print( getEndl() );
}

void JPrinter::printClassStruct( SFC::Class sfcClass ) {
	StructSet structSet = sfcClass.Struct_kind_children();
	if ( structSet.empty() ) return;

	SFC::Struct sfcStruct = *structSet.begin();

	DeclarationSet declarationSet = sfcStruct.Declaration_kind_children_sorted( StatementSorter() );
	StatementVector statementVector = for_each( declarationSet.begin(), declarationSet.end(), StatementTranslator() ).getStatementVector();
	iterate( statementVector );

	print( getEndl() );
}

std::string getProjectName( SFC::Statement statement ) {
	SFC::Project project = SFCManager::getProject( statement );
	return project.name();
}

std::string JPrinter::getClassName( SFC::Class sfcClass ) {

	static std::string projectName = getProjectName( sfcClass );

	typedef std::map< SFC::Class, std::string > ClassNameMap;
	static ClassNameMap classNameMap;

	static int classNo = 0;
	std::string className;
	ClassNameMap::iterator cnmItr = classNameMap.find( sfcClass );
	if ( cnmItr == classNameMap.end() ) {
		className = convertToIdentifierName( sfcClass.name() );

		std::string::size_type truncatePos = className.rfind( "_" );
		if (  truncatePos != std::string::npos && className.substr( truncatePos + 1 ) == "class" ) {
			std::string truncatedClassName = className.substr( 0, truncatePos );
			truncatePos = truncatedClassName.rfind( "_" );
			if (
			 truncatePos != std::string::npos && isNumber(  truncatedClassName.substr( truncatePos + 1 )  )
			) {
				className = className.substr( 0, truncatePos );
			}
		}

		if ( className != projectName ) {
			int suffix = classNo++;
			if (  !isStub( sfcClass )  ) className += boost::lexical_cast< std::string >( suffix );
		}

		classNameMap.insert(  std::make_pair( sfcClass, className )  );
	} else {
		className = cnmItr->second;
	}

	return className;
}


void JPrinter::printNonClassStructs() {
	Udm::Object object = _program.GetParent();
	if ( object == Udm::null ) return;

	SFC::Project project = SFC::Project::Cast( object );
	StructSet structSet = project.Struct_kind_children();
	iterate( structSet );
}


void JPrinter::printArg( SFC::Arg arg ) {
	SFC::DT dt = arg.dt();
	if ( dt == Udm::null ) {
		printWithoutDT( arg );
		return;
	}

	printWithDT( arg );
}

std::string JPrinter::getBaseType( SFC::ArgDeclBase argDeclBase ) {
	SFC::DT dt = argDeclBase.dt();
	if ( dt == Udm::null ) {
		if ( argDeclBase.type() == SFC::Arg::meta ) return SFC::Arg::Cast( argDeclBase ).type();
		if (  Udm::IsDerivedFrom( argDeclBase.type(), SFC::Var::meta )  ) {
			return JTypeMap::lookup(  SFC::Var::Cast( argDeclBase ).type()  );
		}
		if ( argDeclBase.type() == SFC::StateLabel::meta ) return "int";
		return "";
	}

	SFC::DT baseDT = SFCTypesManager::getBaseDT( dt );
	if ( baseDT.type() == SFC::Struct::meta ) return getStructName(  SFC::Struct::Cast( baseDT )  );

	return JTypeMap::lookup(  SFC::BasicType::Cast( baseDT ).name()  );
}

void JPrinter::printWithoutDT( SFC::ArgDeclBase argDeclBase ) {

	printQualifier( argDeclBase );

	std::string varName;
	std::string type;
	int size = 1;

	if ( argDeclBase.type() == SFC::Arg::meta ) {
		SFC::Arg arg = SFC::Arg::Cast( argDeclBase );
		varName = convertToIdentifierName( arg.name() );
		type = JTypeMap::lookup(  static_cast< std::string >( arg.type() )  );
		size = arg.size();
	} else {
		SFC::Var var = SFC::Var::Cast( argDeclBase );
		varName = convertToIdentifierName( var.name() );
		type = JTypeMap::lookup(  static_cast< std::string >( var.type() )  );
		size = var.size();
	}

	if ( size < 1 ) size = 1;

	print( type + " " + varName );

	if ( argDeclBase.type() == SFC::Arg::meta ) {
		SFC::Arg arg = SFC::Arg::Cast( argDeclBase );
		if ( size > 1 || arg.ptr() ) print( "[]" );
	} else {
		SFC::Declaration declaration = SFC::Declaration::Cast( argDeclBase );
		if ( size > 1 ) {
			print( "[] = new " + type + "[ " + boost::lexical_cast< std::string >( size ) + " ]" );
		}
	}
}

void JPrinter::printWithDT( SFC::ArgDeclBase argDeclBase ) {

	printQualifier( argDeclBase );

	SFC::DT dt = argDeclBase.dt();

	Uml::Class dtType = dt.type();
	std::string varName = convertToIdentifierName( argDeclBase.name() );

	SFC::DT baseType = SFCTypesManager::getBaseDT( dt );
	std::string baseTypeName;
	if ( baseType.type() == SFC::Struct::meta ) {

		SFC::Struct sfcStruct = SFC::Struct::Cast( dt );
		baseTypeName =
		 static_cast< __int64 >( sfcStruct.memberCount() ) >= 0 && isEmpty( sfcStruct ) ?
		  "void" : convertToIdentifierName(  getStructName( sfcStruct )  );
	} else {
		baseTypeName = baseType.name();
	}

	if ( dt.type() != SFC::Struct::meta ) baseTypeName = JTypeMap::lookup( baseTypeName );

	print( baseTypeName );
	SFC::DT dt2 = dt;
	while( dt2.type() == SFC::Array::meta ) {
		SFC::Array sfcArray = SFC::Array::Cast( dt2 );
		if ( sfcArray.noelem() > 1 ) print( "[]" );
		dt2 = sfcArray.dt();
	}
	if ( argDeclBase.type() == SFC::Arg::meta ) {
		SFC::Arg arg = SFC::Arg::Cast( argDeclBase );
		if ( dt.type() != SFC::Array::meta && arg.ptr() ) print( "[]" );
		print( " " + varName );
	} else {
		print( " " + varName );
		if ( dt.type() != SFC::Array::meta ) {
			if ( baseTypeName == "boolean" ) {
				print( " = false" );
			} else if ( dt.type() == SFC::BasicType::meta && baseTypeName != "String" ) {
				print( " = 0" );
			} else {
				print( " = new " + baseTypeName + "()" );
			}
		} else {
			print( " = new " + baseTypeName );

			dt2 = dt;
			while( dt2.type() == SFC::Array::meta ) {
				SFC::Array sfcArray = SFC::Array::Cast( dt2 );
				int noelem = sfcArray.noelem();
				if ( noelem > 1 ) print(  std::string( "[" ) + boost::lexical_cast< std::string >( noelem ) + "]"  );
				dt2 = sfcArray.dt();
			}
		}
	}

}


void JPrinter::printQualifier( SFC::ArgDeclBase argDeclBase ) {
	if ( argDeclBase.type() == SFC::Arg::meta ) return;

	SFC::Declaration declaration = SFC::Declaration::Cast( argDeclBase );
	Udm::Object parent = declaration.GetParent();
	if ( parent.type() == SFC::Struct::meta ) {
		SFC::Struct sfcStruct = SFC::Struct::Cast( parent );
		Udm::Object grandParent = sfcStruct.GetParent();
		print( grandParent.type() == SFC::Class::meta ? "private " : "public " );
		return;
	}

	if ( parent.type() == SFC::Program::meta ) {
		if ( argDeclBase.type() != SFC::LocalVar::meta ) {
			print( "public " );
			return;
		}

		SFC::LocalVar localVar = SFC::LocalVar::Cast( argDeclBase );
		SetVarSet setVarSet = localVar.arg();
		if ( !setVarSet.empty() ) {
			SFC::SetVar setVar = *setVarSet.begin();
			print( setVar.invert() ? "public " : "private " );
		}
	}
	return;
}

JPrinter::FunctionVector JPrinter::getFunctions( void ) {
	FunctionVector functionVector;

	{
		FunctionSet functionSet = _program.Function_kind_children_sorted( StatementSorter() );
		functionVector =
			for_each( functionSet.begin(), functionSet.end(), EmptyFunctionFilter() ).getFunctionVector();
	}

	{
		ClassSet classSet = _program.Class_kind_children_sorted( StatementSorter() );
		FunctionVector classFunctionVector =
			for_each( classSet.begin(), classSet.end(), ClassFunctions() ).getFunctionVector();
		classFunctionVector = for_each(
			classFunctionVector.begin(), classFunctionVector.end(), EmptyFunctionFilter()
		).getFunctionVector();

		functionVector.insert( functionVector.end(), classFunctionVector.begin(), classFunctionVector.end() );
	}

	return functionVector;
}

bool JPrinter::isStatusFunction( SFC::Function function ) {
	std::string name( function.name() );
	if ( name == "getStatus" ) return true;

	std::string suffix( "_status" );
	if (  name.size() < suffix.size() || name.substr( name.size() - suffix.size() ) != suffix  ) return false;

	SFC::DT dt = function.dt();
	if ( dt != Udm::null ) {
		SFC::DT baseDT = SFCTypesManager::getBaseDT( dt );
		return static_cast< std::string >( baseDT.name() ) == "string";
	}
	return static_cast< std::string >( function.returnType() ) == "string";
}

bool JPrinter::isMethod( SFC::Function function ) {

	if (  !isClassEntity( function )  ) return false;
	
	ArgSet argSet = function.Arg_children_sorted( ArgSorter() );
	if ( argSet.empty() ) return false;
	SFC::Arg firstArg = *argSet.begin();

	SFC::DT argDT = firstArg.dt();
	if ( argDT == Udm::null || argDT.type() != SFC::Struct::meta ) return false;
	SFC::Struct sfcStruct = SFC::Struct::Cast( argDT );

	Udm::Object parent = sfcStruct.GetParent();
	if ( parent != function.GetParent() ) return false;

	return true;
}

std::string JPrinter::getFunctionName( SFC::Function function ) {

	typedef std::map< SFC::Function, std::string > FunctionNameMap;
	static FunctionNameMap functionNameMap;

	static int functionNo = 0;
	std::string functionName;
	FunctionNameMap::iterator fnmItr = functionNameMap.find( function );
	if ( fnmItr == functionNameMap.end() ) {
		functionName = convertToIdentifierName( function.name() );

		bool method = isMethod( function );
		if (
		 method ||
		 _sfcLegacy && ( functionName == _mainFunctionName || functionName == _initFunctionName )
		) {
			std::string::size_type nameStart = functionName.rfind( "_" ) + 1;
			functionName = functionName.substr( nameStart );
			char convert[2] = { toupper(  functionName.at( 0 )  ), '\0' };
			functionName = std::string( convert ) + functionName.substr( 1 );
		}

		if ( function.statementCount() != -1 && !_sfcLegacy && method ) {
			functionName = functionName + boost::lexical_cast< std::string >( functionNo++ );
		}

		functionNameMap.insert(  std::make_pair( function, functionName )  );
	} else {
		functionName = fnmItr->second;
	}

	return functionName;
}

void JPrinter::printFunctionType( SFC::DT dt ) {
	if ( dt == Udm::null ) return;

	if ( dt.type() == SFC::BasicType::meta ) {
		SFC::BasicType basicType = SFC::BasicType::Cast( dt );
		std::string typeName = JTypeMap::lookup( basicType.name() );
		print( typeName );
	} else if ( dt.type() == SFC::Struct::meta ) {
		SFC::Struct sfcStruct = SFC::Struct::Cast( dt );
		print(   convertToIdentifierName(  getStructName( sfcStruct )  )   );
	} else { // SFC::Array
		SFC::Array sfcArray = SFC::Array::Cast( dt );
		printFunctionType( sfcArray.dt() );
		int noElem = sfcArray.noelem();
		print(  std::string( "[]" )  );
	}
}

void JPrinter::printArgDeclRef( SFC::ArgDeclRef argDeclRef ) {

	SFC::ArgDeclBase argDeclBase = argDeclRef.argdecl();

	bool isAV = isArgVal( argDeclRef );

	if ( argDeclBase == Udm::null ) {
		print( argDeclRef.unres() );
		return;
	}

	if ( argDeclBase.type() == SFC::StateLabel::meta ) {
		print(  SFC::StateLabel::Cast( argDeclBase ).name()  );
		return;
	}

	if (  Udm::IsDerivedFrom( argDeclBase.type(), SFC::Var::meta )  ) {

		SFC::Var var = SFC::Var::Cast( argDeclBase );
		if ( var.GetParent().type() == SFC::Project::meta ) print( getGlobalsName() + "." );
		print(  convertToIdentifierName( var.name() )  );
		return;
	}
	
	if (  argDeclBase.type() == SFC::Arg::meta ) {

		SFC::Arg arg = SFC::Arg::Cast( argDeclBase );
		SFC::DT argDT = arg.dt();
		std::string argName = convertToIdentifierName( arg.name() );
		print( argName );
		if ( arg.ptr() && argDT.type() != SFC::Array::meta ) print( "[0]" );
		return;
	}
	
	assert( false );
}

bool JPrinter::isArgVal( SFC::ArgDeclRef argDeclRef ) {
	SFC::ArgVal argVal = SFC::ArgVal::Cast( argDeclRef.argexprpar() );
	if ( argVal == Udm::null ) return false;
	int argValIndex = argVal.argIndex();

	// KMS: this is a bug fixed in Udm 3.2.8
	// Replace the next two lines with this when 3.2.8 is required: SFC::FunctionCall functionCall = argVal.FunctionCall_parent();
	Udm::Object parent = argVal.GetParent();
	SFC::FunctionCall functionCall = SFC::FunctionCall::Cast( parent );
	SFC::Function sfcFunction = functionCall.callee();
	if ( sfcFunction == Udm::null ) return false;

	ArgSet argSet = sfcFunction.Arg_children_sorted( ArgSorter() );
	for( ArgSet::iterator arsItr = argSet.begin() ; arsItr != argSet.end() ; (void)++arsItr ) {
		int argIndex = arsItr->argIndex();
		if ( argIndex == argValIndex ) return arsItr->ptr();
	}
	return false;
}

void JPrinter::printArgVal( SFC::ArgVal argVal ) {
	ArgSet argSet = argVal.arg_sorted( ArgSorter() );
	if ( !argSet.empty() ) {
		SFC::Arg arg = *argSet.begin();
		SFC::DT dt = arg.dt();
		if ( dt != Udm::null && dt.type() == SFC::Struct::meta ) {
			SFC::Struct sfcStruct = SFC::Struct::Cast( dt );
			if (  static_cast< __int64 >( sfcStruct.memberCount() ) >= 0 && isEmpty( sfcStruct )  ) {
				print( "(void *)0" );
				return;
			}
		}
	}

	SFC::FunctionCall functionCall = argVal.FunctionCall_parent();
	FunctionCall_AVNTVNM_Map::iterator famItr = getFunctionCall_AVNTVNM_Map().find( functionCall );
	if ( famItr != getFunctionCall_AVNTVNM_Map().end() ) {
		ArgValNoTempVarNameMap &argValNoTempVarNameMap = famItr->second;
		ArgValNoTempVarNameMap::iterator atmItr = argValNoTempVarNameMap.find( argVal.argIndex() );
		if ( atmItr != argValNoTempVarNameMap.end() ) {
			print( atmItr->second );
			return;
		}
	}

	SFC::Exprs argexpr = argVal.argexpr();
	if ( argexpr != Udm::null ) {
		printExprs( argexpr );
	} else {
		std::string val = argVal.val();
		if ( !val.empty() ) print( val );
	}
}

void JPrinter::printBinaryExprs( SFC::BinaryExprs binaryExprs ) {

	std::string op( binaryExprs.op() );

	SFC::Exprs leftExprs = SFC::Exprs::Cast( binaryExprs.leftexpr() );
	SFC::Exprs rightExprs = SFC::Exprs::Cast( binaryExprs.rightexpr() );

	if ( op == "." && leftExprs.type() == SFC::ArgDeclRef::meta ) {

		SFC::ArgDeclBase argDeclBase = SFC::ArgDeclRef::Cast( leftExprs ).argdecl();
		if ( argDeclBase != Udm::null && argDeclBase.type() == SFC::Arg::meta ) {

			SFC::Arg arg = SFC::Arg::Cast( argDeclBase );
			if ( arg.argIndex() == 0 ) {

				SFC::Function function = SFC::Function::Cast( arg.GetParent() );
				if (  function != Udm::null && isMethod( function )  ) {
					printExprs( rightExprs );
					return;
				}
			}
		}
	}

	int this_precedence = OpPred::getBinaryPrecedence( op );
	int leftExprs_precedence = OpPred::getOperatorPrecedence( leftExprs, false );

	if ( op == "." ) {
		printExprs( leftExprs );
		print( "." );
		printExprs( rightExprs );
		return;
	}

	std::string leftType = getBaseType( leftExprs );

	if ( op == "?" ) {
		if ( leftType != "boolean" ) {
			print( "( " );
			printExprs( leftExprs );
			print( " != 0 ) ? " );
		} else {
			printExprs( leftExprs );
			print( " ? " );
		}
		printExprs( rightExprs );
		return;
	}

	std::string rightType = getBaseType( rightExprs );

	// THESE ARE THE ONLY BINARY OPERATORS THAT TAKE BOOLEAN OPERANDS, EXCEPT "="
	if ( op == "&&" || op == "||" ) {
		if ( leftType != "boolean" ) { print( "( " ); printExprs( leftExprs ); print( " != 0 )" ); }
		else                         printExprs( leftExprs );
		print(  std::string( " " ) + op + " "  );
		if ( rightType != "boolean" ) { print( "( " ); printExprs( rightExprs ); print( " != 0 )" ); }
		else                          printExprs( rightExprs );
		return;
	}

	std::string dominantType = JTypePrecedence::getDominantType( leftType, rightType );

	// HANDLE "="
	if (  op.substr( op.size() - 1 ) == "="  ) {
		printExprs( leftExprs );
		print(  std::string( " " ) + op + " "  );
		if ( rightType == "boolean" && leftType != "boolean" ) {
			print(  std::string( "(" ) + leftType + ")(  ( "  );
			printExprs( rightExprs );
			print(  " ) ? 1 : 0  )"  );
			return;
		}
		if ( rightType != "boolean" && leftType == "boolean" ) {
			if ( op != "=" ) print( "( " );
			printExprs( rightExprs );
			print( " != 0" );
			if ( op != "=" ) print( " )" );
			return;
		}

		if ( leftType != dominantType || rightType == "void" ) print( "(" + leftType + ")( " );
		printExprs( rightExprs );
		if ( leftType != dominantType || rightType == "void" ) print( " )" );
		return;
	}

	// HANDLE ARRAY INDEXING
	if ( op == "[" ) {
		printExprs( leftExprs );
		print( "[ (int)( " );
		printExprs( rightExprs );
		print( " ) ]" );
		return;
	}

	// NOT USED IN JPRINTER -- SEE op.substr( op.size() - 1 ) == "=" ABOVE
	if ( op == "==" || op == "!=" ) {
		if ( leftType == "boolean" && rightType != "boolean" ) {
			printExprs( leftExprs ); print( op ); print( "(  ( " ); printExprs( rightExprs ); print( " ) != 0  )" );
		} else if ( leftType != "boolean" && rightType == "boolean" ) {
			print( "(  ( " ); printExprs( leftExprs ); print ( " ) != 0  )" ); print( op ); printExprs( rightExprs );
		} else {
			printExprs( leftExprs ); print( op ); printExprs( rightExprs );
		}
		return;
	}
	// HANDLE ALL OTHER OPERATORS
	if ( leftType == "boolean" ) {
		print(  std::string( "(  ( " )  );
		printExprs( leftExprs );
		print(  std::string( " ) ? 1 : 0  )" )  );
	} else {
		bool parensNeeded = false;
		if ( leftType == dominantType ) {
			parensNeeded = this_precedence > leftExprs_precedence;
		} else {
			print( "(" + dominantType + ")" );
			parensNeeded = true;
		}

		if ( parensNeeded ) print( "( " );
		printExprs( leftExprs );
		if ( parensNeeded ) print( " )" );
	}

	bool spaces = !( op == "." );
	if ( spaces ) print( " " );
	print( op );
	if ( spaces ) print( " " );

	if ( rightType == "boolean" ) {
		print(  std::string( "(  ( " )  );
		printExprs( rightExprs );
		print(  std::string( " ) ? 1 : 0  )" )  );
	} else {
		int rightExprs_precedence = OpPred::getOperatorPrecedence( rightExprs, false );
		bool parensNeeded = this_precedence > rightExprs_precedence || this_precedence == rightExprs_precedence && op != "." && op != "?";
		if ( parensNeeded ) print( "( " );
		printExprs( rightExprs );
		if ( parensNeeded ) print( " )" );
	}
}


void JPrinter::printCheckArg( SFC::CheckArg checkArg ) {
	SFC::StateLabel stateLabel = checkArg.slab_end();
	SFC::Arg arg = checkArg.arg_end();

	print(
	 static_cast< std::string >( stateLabel.name() ) + " != " +
	 convertToIdentifierName( arg.name() )
	);
}


void JPrinter::printCheckState( SFC::CheckState checkState ) {
	SFC::StateVar stateVar = checkState.svar();
	SFC::StateLabel indexStateLabel = checkState.index();
	SFC::StateLabel valueStateLabel = checkState.value();

	if ( checkState.andState() ) print( "(  " );

	print(
	 static_cast< std::string >( stateVar.name() ) + "[ " +
	 static_cast< std::string >( indexStateLabel.name() ) + " & 0xFFFF ]"
	);
	if ( checkState.andState() ) {
		print(  std::string( " & ( " ) + static_cast< std::string >( valueStateLabel.name() ) + " >> 16 )  )"  );
		print( checkState.invert() ? " == 0" : " != 0" );
	} else {
		print( checkState.invert() ? " != " : " == " );
		print( valueStateLabel.name() );
	}
}


void JPrinter::printComment( SFC::Comment comment ) {

	std::string content = comment.content();
	std::string::size_type pos = 0;
	while(   (  pos = content.find( "\n" )  ) != std::string::npos   ) {
		printIndent(); print( "//" ); print(  content.substr( 0, pos ) + getEndl() );
		content = content.substr( pos + 1 );
	}
	if ( !content.empty() ) {
		printIndent(); print( "//" ); print( content + getEndl() );
	}
}

void JPrinter::printClass( SFC::Class sfcClass ) {
	if (  isStubClass( sfcClass )  ) return;

	std::string className = convertToIdentifierName(  getClassName( sfcClass )  );
	openFile( className + ".java" );

	if ( getJPF() ) print( "import gov.nasa.jpf.annotation.*;" + getEndl() );
	print( "public class " + className + " {" + getEndl() );

	increaseIndent();
	printClassStaticVariables( sfcClass );
	printClassStruct( sfcClass );
	printClassFunctions( sfcClass );
	decreaseIndent();
	print( "}" + getEndl() );

	closeFile();
}


void JPrinter::printCompoundStatement( SFC::CompoundStatement compoundStatement ) {

	Uml::Class csType = compoundStatement.type();

	// CHECK FOR NULL CompoundStatement'S
	if ( csType == SFC::Function::meta ) {
		SFC::Function function = SFC::Function::Cast( compoundStatement );
		if (  static_cast< __int64 >( function.statementCount() ) == -1  ) return;
	} else {
		if (  isEmpty( compoundStatement )  ) return;
	}

	if ( csType == SFC::ConditionalGroup::meta ) {
		printConditionalGroup(  SFC::ConditionalGroup::Cast( compoundStatement )  );
	} else if ( csType == SFC::Function::meta ) {
		printIndent();
		printFunction(  SFC::Function::Cast( compoundStatement )  );
	} else if ( csType == SFC::IterativeBlock::meta ) {
		printIndent();
		printIterativeBlock(  SFC::IterativeBlock::Cast( compoundStatement )  );
	} else if ( csType == SFC::ConditionalBlock::meta ) {
		printIndent();
		printConditionalBlock(  SFC::ConditionalBlock::Cast( compoundStatement )  );
	} else {
		assert( false );
	}

	print( getEndl() );
}


void JPrinter::printCompoundStatementBody( SFC::CompoundStatement compoundStatement ) {

	print( getEndl() );
	printIndent(); print( "{" + getEndl() );
	increaseIndent();

	if ( compoundStatement.type() == SFC::Function::meta ) {
		SFC::Function function = SFC::Function::Cast( compoundStatement );
		std::string annotation = function.annotation();
		if (  getJPF() && annotation.find( "mtvErrorState" ) != std::string::npos  ) {
			printIndent(); print(  std::string( "assert false;" ) + getEndl()  );
		}
		if (  convertToIdentifierName( function.name() ) == "main"  ) {
			ArgSet argSet = function.Arg_kind_children_sorted( ArgSorter() );
			SFC::Arg argc = *argSet.begin();
			ArgDeclRefSet argDeclRefSet = argc.ref();
			if ( !argDeclRefSet.empty() ) {
				printIndent(); print(  std::string( "int argc = argv.length;" ) + getEndl()  );
			}
		}
	}

	{
		DeclarationSet declarationSet = compoundStatement.Declaration_kind_children_sorted( StatementSorter() );
		StatementVector statementVector =
		 for_each( declarationSet.begin(), declarationSet.end(), StatementTranslator() ).getStatementVector();
		if ( statementVector.size() > 0 ) {
			iterate( statementVector );
			print( getEndl() );
		}
	}
	
	StatementSet statementSet = compoundStatement.stmnt_sorted( StatementSorter() );	
	statementSet = for_each( statementSet.begin(), statementSet.end(), DeclarationFilter() ).getStatementSet();
	iterate( statementSet );

	decreaseIndent();
	printIndent(); print( "}" + getEndl() );
}


void JPrinter::printCondition( SFC::Condition condition ) {
	if ( condition.type() == SFC::CheckArg::meta ) {
		printCheckArg(  SFC::CheckArg::Cast( condition )  );
	} else if ( condition.type() == SFC::CheckState::meta ) {
		printCheckState(  SFC::CheckState::Cast( condition )  );
	} else if ( condition.type() == SFC::UserCode::meta ) {
		SFC::UserCode userCode = SFC::UserCode::Cast( condition );
		ExprsSet exprsSet = userCode.Exprs_kind_children();
		if ( !exprsSet.empty() ) {
			SFC::Exprs exprs = *exprsSet.begin();
			if (  getBaseType( exprs ) != "boolean"  ) print( " != 0" );
		}
		printUserCode(  SFC::UserCode::Cast( condition )  );
	}
	else assert( false );
}


bool JPrinter::printConditionalBlock( SFC::ConditionalBlock conditionalBlock ) {

	bool retval = printConditionalBlockHeader( conditionalBlock );
	printCompoundStatementBody( conditionalBlock );
	return retval;
}


bool JPrinter::printConditionalBlockHeader( SFC::ConditionalBlock conditionalBlock ) {

	ConditionSet conditionSet = conditionalBlock.cond();

	bool trueFlag = true;
	for( ConditionSet::iterator cdsItr = conditionSet.begin() ; cdsItr != conditionSet.end() ; ++cdsItr ) {
		SFC::Condition condition = *cdsItr;
		if ( condition.type() != SFC::UserCode::meta ) {
			trueFlag = false;
			break;
		}
		SFC::UserCode userCode = SFC::UserCode::Cast( condition );
		SFC::Exprs codeexpr = userCode.codeexpr();
		if ( codeexpr == Udm::null ) {
			trueFlag = false;
			break;
		}

		if ( codeexpr.type() == SFC::Int::meta ) {
			int val = SFC::Int::Cast( codeexpr ).val();
			if ( val != 0 ) {
				continue;
			}
		}

		if ( codeexpr.type() == SFC::Double::meta ) {
			float val = SFC::Double::Cast( codeexpr ).val();
			if ( val != 0 ) {
				continue;
			}
		}

		trueFlag = false;
		break;
	}

	if ( trueFlag ) {
		return false;
	}

	print( "if ( " );
	switch( conditionSet.size() ) {
		case 1:
			iterate( conditionSet );
			break;
		default:
			print( " ( " );
			iterate( conditionSet, " ) && ( " );
			print( " ) " );
	}	
	print( " )" );

	return true;
}

void JPrinter::printConditionalGroup( SFC::ConditionalGroup conditionalGroup ) {
	ObjectSet objectSet = conditionalGroup.GetChildObjects();
	StatementSet statementSet;
	for( ObjectSet::iterator obsItr = objectSet.begin() ; obsItr != objectSet.end() ; ++obsItr ) {
		statementSet.insert(  SFC::Statement::Cast( *obsItr )  );
	}

	bool notFirstCondition = false;
	for( StatementSet::iterator stsItr = statementSet.begin() ; stsItr != statementSet.end() ; ++stsItr ) {
		SFC::Statement statement = *stsItr;
		if ( statement.type() == SFC::Comment::meta ) {
			SFC::Comment comment = SFC::Comment::Cast( statement );
			printComment( comment );
		} else if ( statement.type() == SFC::ConditionalBlock::meta ) {
			SFC::ConditionalBlock conditionalBlock = SFC::ConditionalBlock::Cast( statement );
			if (  isEmpty( conditionalBlock )  ) continue;
			print( getIndent() );
			if ( notFirstCondition ) print( "else " );
			else                     notFirstCondition = true;
			if (  !printConditionalBlock( conditionalBlock )  ) {
				return;
			}
		} else {
			std::cerr << "WARNING: Unexpected Statement type \"" << statement.type().name() << "\" encountered in ConditionalGroup." << std::endl;
		}
	}
}

void JPrinter::printDeclarations( void ) {
	StatementVector statementVector;

	if ( _sfcLegacy ) {
		StateLabelSet stateLabelSet = _program.StateLabel_kind_children_sorted( StatementSorter() );
		statementVector =
		 for_each( stateLabelSet.begin(), stateLabelSet.end(), StatementTranslator() ).getStatementVector();
	} else {
		DeclarationSet declarationSet = _program.Declaration_kind_children_sorted( StatementSorter() );
		declarationSet =
		 for_each( declarationSet.begin(), declarationSet.end(), DTReverseFilter() ).getDeclarationSet();
		statementVector =
		 for_each( declarationSet.begin(), declarationSet.end(), StatementTranslator() ).getStatementVector();
	}

	iterate( statementVector );

	if ( statementVector.size() > 0 ) print( getEndl() + getEndl() );
}

	
void JPrinter::printDeclaration( SFC::Declaration declaration ) {
	if ( declaration.type() == SFC::StateLabel::meta )    printStateLabel(  SFC::StateLabel::Cast( declaration )  );
	else if ( declaration.type() == SFC::StateVar::meta ) printStateVar(  SFC::StateVar::Cast( declaration )  );
	else if ( declaration.type() == SFC::LocalVar::meta ) printLocalVar(  SFC::LocalVar::Cast( declaration )  );
	else assert( false );
}


void JPrinter::printExprs( SFC::Exprs exprs ) {
	if ( exprs == Udm::null ) return;

	SFC::ArgVal argVal = exprs.argexprpar();
	bool closingParen = false;
	if ( argVal != Udm::null ) {
		ArgSet argSet = argVal.arg_sorted( ArgSorter() );
		if ( !argSet.empty() ) {
			SFC::Arg arg = *argSet.begin();
			std::string argType = getBaseType( arg );
			std::string argValType = getBaseType( exprs );
			std::string dominantType = JTypePrecedence::getDominantType( argType, argValType );
			if ( argType != dominantType ) {
				print(  std::string( "(" ) + argType + ")( "  );
				closingParen = true;
			}
		}
	}
	if ( exprs.type() == SFC::BinaryExprs::meta )       printBinaryExprs(  SFC::BinaryExprs::Cast( exprs )  );
	else if ( exprs.type() == SFC::UnaryExprs::meta )   printUnaryExprs(  SFC::UnaryExprs::Cast( exprs )  );
	else if ( exprs.type() == SFC::Int::meta )          printInt(  SFC::Int::Cast( exprs )  );
	else if ( exprs.type() == SFC::Double::meta )       printDouble(  SFC::Double::Cast( exprs )  );
	else if ( exprs.type() == SFC::Str::meta )          printStr(  SFC::Str::Cast( exprs )  );
	else if ( exprs.type() == SFC::ArgDeclRef::meta )   printArgDeclRef(  SFC::ArgDeclRef::Cast( exprs )  );
	else if ( exprs.type() == SFC::FunctionCall::meta ) printFunctionCall(  SFC::FunctionCall::Cast( exprs )  );
	else assert( false );

	if ( closingParen ) print( " )" );
}

std::string JPrinter::getBaseType( SFC::Exprs exprs ) {
	if (  Udm::IsDerivedFrom( exprs.type(), SFC::BinaryExprs::meta )  ) {
		SFC::BinaryExprs binaryExprs = SFC::BinaryExprs::Cast( exprs );

		std::string op = binaryExprs.op();
		if (
		 op == ">"  || op == ">=" || op == "<" || op == "<=" || op == "==" || op == "!=" ||
		 op == "&&" || op == "||"
		) return "boolean";

		SFC::Exprs rightExprs = binaryExprs.rightexpr();
		std::string rightType = getBaseType( rightExprs );

		if ( op == "." || op == "?" ) return rightType;

		SFC::Exprs leftExprs = binaryExprs.leftexpr();
		std::string leftType = getBaseType( leftExprs );

		if ( op == "[" ) return leftType;

		return JTypePrecedence::getDominantType( rightType, leftType );
	}

	if (  Udm::IsDerivedFrom( exprs.type(), SFC::UnaryExprs::meta )  ) {
		SFC::UnaryExprs unaryExprs = SFC::UnaryExprs::Cast( exprs );

		std::string op = unaryExprs.op();
		if ( op == "!" ) return "boolean";

		SFC::Exprs subExprs = unaryExprs.subexpr();
		return getBaseType( subExprs );
	}

	if ( exprs.type() == SFC::ArgDeclRef::meta ) {
		SFC::ArgDeclBase argDeclBase = SFC::ArgDeclRef::Cast( exprs ).argdecl();
		if ( argDeclBase == Udm::null ) return "";
		return getBaseType( argDeclBase );
	}

	if ( exprs.type() == SFC::Double::meta ) {
		return "double";
	}

	if ( exprs.type() == SFC::Int::meta ) {
		return "int";
	}

	if ( exprs.type() == SFC::Str::meta ) {
		return "String";
	}

	if ( exprs.type() == SFC::FunctionCall::meta ) {
		SFC::FunctionCall functionCall = SFC::FunctionCall::Cast( exprs );
		SFC::Function sfcFunction = functionCall.callee();
		if ( sfcFunction != Udm::null ) {
			SFC::DT dt = sfcFunction.dt();
			return dt == Udm::null ? JTypeMap::lookup(  static_cast< std::string >( sfcFunction.returnType() )  ) : JTypeMap::lookup(  static_cast< std::string >( dt.name() )  );
		}
		ArgValSet argValSet = functionCall.ArgVal_children_sorted( ArgValSorter() );
		if ( argValSet.empty() ) return "void";
		SFC::Exprs exprs = argValSet.begin()->argexpr();
		return getBaseType( exprs );
	}

	assert( false );
	return "bad_type";
}


//
// FUNCTION PROCESSING
//
JPrinter::StringVector JPrinter::getExpressionVector( const std::string &annotation, const std::string &type ) {

	std::string jpfDirective = type;
	jpfDirective[ 0 ] = toupper( jpfDirective[ 0 ] );

	boost::regex xmlElementRegex( "<\\s*" + type + "\\s+expression\\s*=\\s*\"(.*?)\"\\s*/>", boost::regex_constants::perl );
	boost::match_results< std::string::const_iterator > results;

	std::string::const_iterator start = annotation.begin();
	std::string::const_iterator finish = annotation.end();

	StringVector stringVector;

	while (  regex_search( start, finish, results, xmlElementRegex )  ) {
		std::string expression( results[1].first, results[1].second );
		stringVector.push_back( expression );
		start = results[0].second;
	}

	return stringVector;
}

std::string JPrinter::normalizeWhitespace( const std::string &input ) {

	std::string output;
	for( std::string::const_iterator strCit = input.begin() ; strCit != input.end() ; ++strCit ) {
		char character = *strCit;
		switch( character ) {
			case '\n':
			case '\r': {
				output += ' ';
				break;
			}
			default: {
				output += character;
			}
		}
	}

	return output;
}

JPrinter::StringMap JPrinter::getTranslationMap( SFC::Function function ) {

	StringMap translationMap;

	translationMap.insert(  std::make_pair( "&lt;", "<" )  );
	translationMap.insert(  std::make_pair( "&gt;", ">" )  );

	ArgSet argSet = function.Arg_children_sorted( ArgSorter() );
	ArgSet::iterator arsItr = argSet.begin();
	while( ++arsItr != argSet.end() ) {
		SFC::Arg arg = *arsItr;
		std::string argName = arg.name();
		SFC::DT dt = arg.dt();
		std::string origName = argName.substr(  0, argName.rfind( "_" )  );
		origName = normalizeWhitespace( origName );
		argName = convertToIdentifierName( argName );
		if ( dt.type() != SFC::Array::meta && arg.ptr() ) argName += "[ 0 ]";
		translationMap.insert(  std::make_pair( origName, argName )  );
	}

	return translationMap;
}

std::string JPrinter::makeSubstitutions( std::string expression, const StringMap &translationMap ) {

	for( StringMap::const_iterator stmCit = translationMap.begin() ; stmCit != translationMap.end() ; ++stmCit ) {
		std::string newExpression;
		std::string origName = stmCit->first;
		std::string argName = stmCit->second;

		std::string::size_type lagPos = 0;
		std::string::size_type pos = 0;

		while(   (  pos = expression.find( origName, lagPos )  ) != std::string::npos   ) {
			newExpression += expression.substr( lagPos, pos - lagPos ) + argName;
			lagPos = pos + origName.length();
		}
		newExpression += expression.substr( lagPos );

		expression = newExpression;
	}

	return expression;
}

void JPrinter::printAnnotations( SFC::Function function ) {

	if ( function.GetParent().type() != SFC::Class::meta ) return;

	SFC::Class sfcClass = SFC::Class::Cast( function.GetParent() );

	std::string annotation = sfcClass.annotation();

	StringMap translationMap = getTranslationMap( function );

	StringVector annotationTypeVector = getAnnotationTypeVector();
	for( StringVector::iterator atvItr = annotationTypeVector.begin() ; atvItr != annotationTypeVector.end() ; ++atvItr ) {

		std::string annotationType = *atvItr;
		StringVector expressionVector = getExpressionVector( annotation, annotationType );

		if ( !expressionVector.empty() ) {

			std::string jpfAnnotation = annotationType;
			jpfAnnotation[ 0 ] = toupper( jpfAnnotation[ 0 ] );

			print( getIndent() + "@" + jpfAnnotation + "(" );
			if ( expressionVector.size() > 1 ) print( "{" );
			bool printComma = false;
			for( StringVector::iterator exvItr = expressionVector.begin() ; exvItr != expressionVector.end() ; ++exvItr ) {
				if ( printComma ) print( "," );
				else              printComma = true;
				print(  "\"" + makeSubstitutions( *exvItr, translationMap ) + "\""  );
			}
			if ( expressionVector.size() > 1 ) print( "}" );
			print( ")" + getEndl() );

		}
	}
}

void JPrinter::printFunctionHeader( SFC::Function function ) {

	std::string functionName = convertToIdentifierName( function.name() );
	if (
	 isClassEntity( function ) ||
	 _sfcLegacy && ( functionName == _mainFunctionName || functionName == _initFunctionName )
	) functionName = convertToIdentifierName(  getFunctionName( function )  );

	if (  getJPF() && functionName.substr( 0, 4 ) == "Main"  ) printAnnotations( function );

	printIndent();
	print( "public " );

	bool method = isMethod( function );

	if ( !method && !_sfcLegacy ) print( "static " );

	bool inMain = convertToIdentifierName( function.name() ) == "main";
	SFC::DT dt = function.dt();

	if ( inMain ) {
		print( "void " );
	} else if ( dt == Udm::null ) {
		print(   JTypeMap::lookup(  static_cast< std::string >( function.returnType() )  )   );
	} else {
		printFunctionType( dt );
	}

	print( " " + functionName + "( " );

	if ( inMain ) {
		print( "String argv[]" );
	} else {
		ArgSet argSet = function.Arg_children_sorted( ArgSorter() );
		if ( method && !_sfcLegacy ) argSet.erase( argSet.begin() );
		iterate( argSet, ", " );
	}

	print( " )" );
}



//
// END FUNCTION PROCESSING
//


void JPrinter::generateTempArgVals( Udm::Object object ) {

	ObjectSet objectSet = object.GetChildObjects();
	for( ObjectSet::iterator obsItr = objectSet.begin() ; obsItr != objectSet.end() ; ++obsItr ) {
		generateTempArgVals( *obsItr );
	}

	if ( object.type() == SFC::FunctionCall::meta ) {

		SFC::FunctionCall functionCall = SFC::FunctionCall::Cast( object );

		SFC::Function sfcFunction = functionCall.callee();
		if ( sfcFunction == Udm::null ) return;

		ArgSet argSet = sfcFunction.Arg_children_sorted( ArgSorter() );
		ArgValSet argValSet = functionCall.ArgVal_children_sorted( ArgValSorter() );

		if (  isMethod( sfcFunction )  ) {
			argSet.erase( argSet.begin() );
			argValSet.erase( argValSet.begin() );
		}

		if ( argSet.empty() ) return;

		ArgValNoTempVarNameMap argVarNoTempVarNameMap;

		ArgSet::iterator arsItr = argSet.begin();
		ArgValSet::iterator avsItr = argValSet.begin();
		for( ; arsItr != argSet.end() ; ++arsItr, ++avsItr ) {

			SFC::Arg arg = *arsItr;
			SFC::DT argDT = arg.dt();

			SFC::ArgVal argVal = *avsItr;

			if ( arg.ptr() && argDT.type() != SFC::Array::meta ) {

				std::string typeName = JTypeMap::lookup( argDT.name() );

				std::string tempArgValName = "tempArgVal_" + boost::lexical_cast< std::string >( getUniqueNo() );

				print( typeName + "[] " + tempArgValName + " = new " + typeName + "[1]" +";" + getEndl() );
				print( getIndent() + tempArgValName + "[0] = " );
				printArgVal( argVal );
				print( ";" + getEndl() + getIndent() );

				argVarNoTempVarNameMap.insert(  std::make_pair( arg.argIndex(), tempArgValName )  );
			}

		}

		if ( !argVarNoTempVarNameMap.empty() ) getFunctionCall_AVNTVNM_Map()[ functionCall ] = argVarNoTempVarNameMap;
	}
}


void JPrinter::assignFromTempArgVals( Udm::Object object ) {

	if ( object.type() == SFC::FunctionCall::meta ) {

		SFC::FunctionCall functionCall = SFC::FunctionCall::Cast( object );

		FunctionCall_AVNTVNM_Map::iterator famItr = getFunctionCall_AVNTVNM_Map().find( functionCall );
		if ( famItr != getFunctionCall_AVNTVNM_Map().end() ) {

			ArgValNoTempVarNameMap &argValNoTempVarNameMap = famItr->second;

			ArgValSet argValSet = functionCall.ArgVal_children_sorted( ArgValSorter() );

			for( ArgValSet::iterator avsItr = argValSet.begin() ; avsItr != argValSet.end() ; ++avsItr ) {
				SFC::ArgVal argVal = *avsItr;

				ArgValNoTempVarNameMap::iterator atmItr = argValNoTempVarNameMap.find( argVal.argIndex() );
				if ( atmItr == argValNoTempVarNameMap.end() ) continue;

				std::string tempArgValName = atmItr->second;
				print( ";" + getEndl() + getIndent() );

				argValNoTempVarNameMap.erase( atmItr );

				printArgVal( argVal );
				print( " = " + tempArgValName + "[0]" );

				if ( argValNoTempVarNameMap.empty() ) {
					getFunctionCall_AVNTVNM_Map().erase( famItr );
					break;
				}
			}
		}
	}

	ObjectSet objectSet = object.GetChildObjects();
	for( ObjectSet::iterator obsItr = objectSet.begin() ; obsItr != objectSet.end() ; ++obsItr ) {
		assignFromTempArgVals( *obsItr );
	}
}

void JPrinter::printFunctionCall( SFC::FunctionCall functionCall ) {

	SFC::Function callee = functionCall.callee();
	ArgValSet argValSet = functionCall.ArgVal_children_sorted( ArgValSorter() );

	if (  isClassEntity( callee )  ) {

		std::string calleeName = convertToIdentifierName(  getFunctionName( callee )  );

		if (  isMethod( callee )  ) {
			SFC::ArgVal contextArgVal = *argValSet.begin();
			argValSet.erase( argValSet.begin() );

			SFC::ArgDeclRef argDeclRef;
			SFC::Exprs exprs = contextArgVal.argexpr();

			if ( exprs.type() == SFC::BinaryExprs::meta ) {
				SFC::BinaryExprs binaryExprs = SFC::BinaryExprs::Cast( contextArgVal.argexpr() );
				if (  static_cast< std::string >( binaryExprs.op() ) == "."  ) {
					argDeclRef = SFC::ArgDeclRef::Cast( binaryExprs.rightexpr() );
				}
			} else {
				argDeclRef = SFC::ArgDeclRef::Cast( exprs );
			}

			SFC::LocalVar localVar = SFC::LocalVar::Cast( argDeclRef.argdecl() );
			print(  convertToIdentifierName( localVar.name() ) + "." + calleeName  );
		} else {
			print(   getClassName(  getClass( callee )  ) + "." + calleeName   );
		}

	} else {
		std::string calleeName;
		if ( callee == Udm::null ) {
			calleeName = functionCall.libFuncName();
			if (
			 calleeName == "sin"   || calleeName == "cos"    || calleeName == "tan"   ||
			 calleeName == "asin"  || calleeName == "acos"   || calleeName == "atan"  || calleeName == "atan2" ||
			 calleeName == "sinh"  || calleeName == "cosh"   || calleeName == "tanh"  ||
			 calleeName == "abs"   || calleeName == "fabs"   || calleeName == "hypot" ||
			 calleeName == "floor" || calleeName == "ceil"   ||
			 calleeName == "exp"   || calleeName == "pow"    || calleeName == "sqrt"  || calleeName == "cbrt"  ||
			 calleeName == "log"   || calleeName == "log10"  || calleeName == "log1p" ||
			 calleeName == "sign"  || calleeName == "signum" || calleeName == "round" || calleeName == "min"   || calleeName == "max"
			) {
				if ( calleeName == "fabs" )      calleeName = "abs";
				else if ( calleeName == "sign" ) calleeName = "signum";

				calleeName = "Math." + calleeName;

			} else if ( calleeName == "isnan" || calleeName == "isinf" ) {

				if ( calleeName == "isnan" ) calleeName = "isNaN";
				else if ( calleeName == "isinf" ) calleeName = "isInfinite";

				calleeName = "Double." + calleeName;

			} else if ( calleeName == "fmod" ) {

				getMyMath().makeFunction( "fmod" );
				calleeName = "MyMath." + calleeName;

			} else if ( calleeName == "printf" || calleeName == "mprintf" ) {

				calleeName = "System.out.printf";

			}
		} else {
			calleeName = convertToIdentifierName( callee.name() );
		}

		SFC::Function containingFunction = getContainingFunction( functionCall );

		print( getIndent() + calleeName );
	}

	print( "( " );
	iterate( argValSet, ", " );
	print( " )" );

}


void JPrinter::printIterativeBlock( SFC::IterativeBlock iterativeBlock ) {
	printIterativeBlockHeader( iterativeBlock );
	printCompoundStatementBody( iterativeBlock );
}


void JPrinter::printIterativeBlockHeader( SFC::IterativeBlock iterativeBlock ) {
	print( "while( " );
	ConditionSet conditionSet = iterativeBlock.cond();
	switch( conditionSet.size() ) {
		case 0:
				print( "true" );
				break;
		case 1:
				iterate( conditionSet );
				break;
		default:
			print( " ( " );
			iterate( conditionSet, " ) && ( " );
			print( " ) " );
	}	
	print( " )" );
}


//
// LocalVar PROCESSING
//
void JPrinter::printLocalVar( SFC::LocalVar localVar ) {
	SFC::DT dt = localVar.dt();
	if ( dt == Udm::null ) printWithoutDT( localVar );
	else                   printWithDT( localVar );
}
//
// END LocalVar PROCESSING
//

void JPrinter::printReturn( SFC::Return sfcReturn ) {
	print( "return" );
	SFC::Exprs retexpr = sfcReturn.retexpr();
	if ( retexpr != Udm::null ) {
		print( " " );
		printExprs( retexpr );
	} else {
		std::string val = sfcReturn.val();
		if ( !val.empty() ) print( std::string( " " ) + val );
	}
}

void JPrinter::printSetState( SFC::SetState setState ) {
	SFC::StateVar stateVar = setState.svar();
	SFC::StateLabel indexStateLabel = setState.index();
	SFC::StateLabel valueStateLabel = setState.value();

	print(
	 static_cast< std::string >( stateVar.name() ) + "[ " +
	 static_cast< std::string >( indexStateLabel.name() ) + " & 0xFFFF ]"
	);

	if ( setState.andState() ) {
		print( setState.invert() ? "&= ~" : "|= " );
		print(  std::string( "( " ) + static_cast< std::string >( valueStateLabel.name() ) + " >> 16 )"  );
	} else {
		print( " = " );
		if ( setState.invert() ) print( "-" );
		print( valueStateLabel.name() );
	}
}


//
// SetVar PROCESSING
//
void JPrinter::printLoopStarts( SFC::DT dt ) {

	while( dt.type() == SFC::Array::meta ) {

		SFC::Array sfcArray = SFC::Array::Cast( dt );
		int noelem = sfcArray.noelem();

		dt = sfcArray.dt();
		if ( noelem <= 1 ) continue;

		printIndent(); print( "{" + getEndl() );
		increaseIndent();

		std::string indexVarName = std::string( "ix" ) + boost::lexical_cast< std::string >( dt.uniqueId() );
		printIndent(); print(  std::string( "int " ) + indexVarName + ";" + getEndl()  );
		printIndent();
		print(
		 std::string( "for( " ) + indexVarName + " = 0 ; " + indexVarName + " < " +
		  boost::lexical_cast< std::string >( noelem ) + " ; ++" + indexVarName + " )" + getEndl()
		);
	}
}

void JPrinter::printLoopEnds( SFC::DT dt ) {
	while( dt.type() == SFC::Array::meta ) {
		SFC::Array sfcArray = SFC::Array::Cast( dt );
		int noelem = sfcArray.noelem();

		dt = sfcArray.dt();
		if ( noelem <= 1 ) continue;

		decreaseIndent();
		printIndent(); print(  std::string( "}" ) + getEndl()  );
	}
}

void JPrinter::printIndexes( SFC::DT dt ) {

	while( dt.type() == SFC::Array::meta ) {
		SFC::Array sfcArray = SFC::Array::Cast( dt );
		int noelem = sfcArray.noelem();

		dt = sfcArray.dt();
		if ( noelem <= 1 ) continue;

		std::string indexVarName = std::string( "ix" ) + boost::lexical_cast< std::string >( dt.uniqueId() );
		print(  std::string( "[" ) + boost::lexical_cast< std::string >( indexVarName ) + "]"  );
	}
}

void JPrinter::printSetVar( SFC::SetVar setVar ) {
	SFC::Arg arg = setVar.arg_end();
	std::string argIdentifierName = convertToIdentifierName( arg.name() );

	SFC::LocalVar localVar = setVar.lvar_end();
	std::string localVarIdentifierName = convertToIdentifierName( localVar.name() );

	SFC::DT dt = arg.dt();
	if ( dt == Udm::null ) {
		printIndent();
		if ( setVar.invert() ) {
			print( argIdentifierName ); print( "[ 0 ] = " );
			print( localVarIdentifierName );
		} else {
			print( localVarIdentifierName );
			print( " = " );
			print( argIdentifierName );
		}
		print( ";" + getEndl() );
		return;
	}

	if ( dt.type() != SFC::Struct::meta ) {

		printLoopStarts( dt );

		printIndent(); print( "{" + getEndl() );
		increaseIndent();

		printIndent();
		if ( setVar.invert() ) {
			print( argIdentifierName );
			if ( dt.type() == SFC::Array::meta ) printIndexes( dt );
			else                                 print( "[ 0 ]" );
			print( " = " );
			print( localVarIdentifierName ); printIndexes( dt );
		} else {
			print( localVarIdentifierName ); printIndexes( dt );
			print( " = " );
			print( argIdentifierName );
			if ( dt.type() == SFC::Array::meta ) printIndexes( dt );
		}
		print( ";" + getEndl() );
		decreaseIndent();
		printIndent();
		print( "}" + getEndl() );

		printLoopEnds( dt );

	} else {

		if ( setVar.invert() ) {
			print( argIdentifierName + "[ 0 ] = " + localVarIdentifierName );
		} else {
			print( localVarIdentifierName + " = " + argIdentifierName );
		}
		print( ";" + getEndl() );

	}
}
//
// END SetVar PROCESSING
//


void JPrinter::printStateLabel( SFC::StateLabel stateLabel ) {
	ostringstream toHex;
	toHex << std::hex << static_cast< int >( stateLabel.value() );
	std::string hexValue = toHex.str();

	print(
	 std::string( "final static int " ) + static_cast< std::string >( stateLabel.name() ) + " = 0x" + hexValue
	);
}


void JPrinter::printStatement( SFC::Statement statement ) {

	// CHECK FOR EMPTY SFC::Statement'S
	if (  isEmpty( statement )  ) return;

	if (  Udm::IsDerivedFrom( statement.type(), SFC::CompoundStatement::meta )  ) {
		printCompoundStatement(  SFC::CompoundStatement::Cast( statement )  );
		return;
	}

	if ( statement.type() == SFC::SetVar::meta ) {
		printSetVar(  SFC::SetVar::Cast( statement )  );
		return;
	}

	if ( statement.type() == SFC::Comment::meta ) {
		printComment(  SFC::Comment::Cast( statement )  );
		return;
	}

	printIndent();

	if (  Udm::IsDerivedFrom( statement.type(), SFC::Declaration::meta )  ) {
		printDeclaration(  SFC::Declaration::Cast( statement )  );
	} else {
		generateTempArgVals( statement );
		if ( statement.type() == SFC::Return::meta )            printReturn(  SFC::Return::Cast( statement )  );
		else if ( statement.type() == SFC::UserCode::meta )     printUserCode(  SFC::UserCode::Cast( statement )  );
		else if ( statement.type() == SFC::FunctionCall::meta ) printFunctionCall(  SFC::FunctionCall::Cast( statement )  );
		else if ( statement.type() == SFC::SetState::meta )     printSetState(  SFC::SetState::Cast( statement )  );
		else assert( false );
		assignFromTempArgVals( statement );
	}

	print( ";" + getEndl() );
}

bool JPrinter::isEmpty( SFC::Statement statement ) {

	if (  Udm::IsDerivedFrom( statement.type(), SFC::CompoundStatement::meta )  ) {
		SFC::CompoundStatement compoundStatement = SFC::CompoundStatement::Cast( statement );
		if (  static_cast< __int64 >( compoundStatement.statementCount() ) < 0  ) return false;
		StatementSet statementSet = compoundStatement.stmnt_sorted( StatementSorter() );
		for( StatementSet::iterator stsItr = statementSet.begin() ; stsItr != statementSet.end() ; (void)++stsItr ) {
			if ( !isEmpty( *stsItr ) ) return false;
		}
		return true;
	}

	if ( statement.type() == SFC::Struct::meta ) {
		SFC::Struct sfcStruct = SFC::Struct::Cast( statement );
		Udm::Object parent = sfcStruct.GetParent();
		if ( parent.type() == SFC::Class::meta ) {
			SFC::Class sfcClass = SFC::Class::Cast( parent );
			FunctionVector functionVector = sfcClass.Function_kind_children();
			if ( !functionVector.empty() ) return false;
		}
		DeclarationSet declarationSet = sfcStruct.Declaration_kind_children_sorted( StatementSorter() );
		for(
		 DeclarationSet::iterator dcsItr = declarationSet.begin() ;
		 dcsItr != declarationSet.end() ;
		 (void)++dcsItr
		) if (  !isEmpty( *dcsItr )  ) return false;
		return true;
	}

	if ( statement.type() == SFC::FunctionCall::meta ) {
		SFC::FunctionCall functionCall = SFC::FunctionCall::Cast( statement );
		SFC::Function callee = functionCall.callee();
		return callee != Udm::null && isEmpty( callee );
	}

	if ( statement.type() == SFC::UserCode::meta ) {
		SFC::UserCode userCode = SFC::UserCode::Cast( statement );
		SFC::Exprs exprs = SFC::Exprs::Cast( userCode.codeexpr() );
		return exprs == Udm::null && static_cast< std::string >( userCode.expr() ).empty();
	}

	if (  Udm::IsDerivedFrom( statement.type(), SFC::Declaration::meta )  ) {
		SFC::Declaration declaration = SFC::Declaration::Cast( statement );
		SFC::DT dt = declaration.dt();
		if ( dt != Udm::null && dt.type() == SFC::Struct::meta ) {
			SFC::Struct sfcStruct = SFC::Struct::Cast( dt );
			if (  static_cast< __int64 >( sfcStruct.memberCount() ) < 0 ) return false;
			return isEmpty( sfcStruct );
		}
		return false;
	}

	return false;
}


//
// Struct PROCESSING
//
void JPrinter::printStruct( SFC::Struct sfcStruct ) {

	if (  isEmpty( sfcStruct )  ) return;

	DeclarationSet declarationSet = sfcStruct.Declaration_kind_children_sorted( StatementSorter() );
	declarationSet =
	 for_each( declarationSet.begin(), declarationSet.end(), EmptyDeclarationFilter() ).getStatementSet();
	StatementVector statementVector =
	 for_each( declarationSet.begin(), declarationSet.end(), StatementTranslator() ).getStatementVector();


	std::string structName = convertToIdentifierName(  getStructName( sfcStruct )  );

	openFile( structName + ".java" );

	print( "public class " + structName + " {" + getEndl() );

	increaseIndent();
	iterate( statementVector );
	decreaseIndent();

	print( "}" + getEndl() + getEndl() );

	closeFile();
}


std::string JPrinter::getStructName( SFC::Struct sfcStruct ) {

	typedef std::map< SFC::Struct, std::string > StructNameMap;
	static StructNameMap structNameMap;

	if (  isClassStruct( sfcStruct )  ) {
		SFC::Class sfcClass = SFC::Class::Cast( sfcStruct.GetParent() );
		return getClassName( sfcClass );
	}

	static int structNo = 0;

	std::string structName;
	StructNameMap::iterator snmItr = structNameMap.find( sfcStruct );

	if ( snmItr == structNameMap.end() ) {
		__int64 memberCount = sfcStruct.memberCount();
		structName = convertToIdentifierName( sfcStruct.name() );
		if (
		 memberCount > 0 && (
		  structName.substr(0, 1) == "M" && structName.substr( structName.size() - 1, 1 ) == "M" ||
		  structName.substr(0, 1) == "S" && structName.substr( structName.size() - 1, 1 ) == "S"
		)) {
			structName = std::string( "struct" ) + boost::lexical_cast< std::string >( structNo++ );
		}
		structNameMap.insert(  std::make_pair( sfcStruct, structName )  );
	} else {
		structName = snmItr->second;
	}

	return structName;
}
//
// END Struct PROCESSING
//


void JPrinter::printUnaryExprs( SFC::UnaryExprs unaryExprs ) {
	std::string op = unaryExprs.op();
	if ( op.empty() ) {
		printExprs( unaryExprs.subexpr() );
		return;
	}

	int this_precedence = OpPred::getUnaryPrecedence( op );

	SFC::Exprs subExprs = SFC::Exprs::Cast( unaryExprs.subexpr() );
	int subExpr_precedence = OpPred::getOperatorPrecedence( subExprs, false );

	bool parensNeeded = this_precedence > subExpr_precedence;

	bool postOp = op.substr( 0, 4 ) == "post";

	if ( !postOp ) print( op );
	if( parensNeeded ) print( "( " );

	std::string subExprsType = getBaseType( subExprs );
	if ( op == "!" && subExprsType != "boolean" ) {
		print( "( " );
		printExprs( subExprs );
		print( " != 0 )" );
	} else {
		printExprs( subExprs );
	}

	if( parensNeeded ) print( " )" );
	if ( postOp ) print(  op.substr( 4 )  );
}

void JPrinter::printUserCode( SFC::UserCode userCode ) {
	SFC::Exprs codeexpr = userCode.codeexpr();

	if ( codeexpr != Udm::null ) {
		printExprs( codeexpr );
	} else {
		std::string expr = userCode.expr();
		if ( !expr.empty() ) print( expr );
	}
}
