#include "CPrinter.hpp"
#include "OpPred.hpp"

std::string CPrinter::getStructName( SFC::Struct sfcStruct ) {
	return static_cast< std::string >( sfcStruct.name() );
}

void CPrinter::printGlobals( SFC::Project project ) {
	LocalVarSet localVarSet = project.LocalVar_kind_children_sorted( StatementSorter() );
	if ( localVarSet.empty() ) return;

	std::string globalsName = convertToIdentifierName( project.name() ) + "_globals";

	std::string structName = globalsName + "_struct";
	std::string gateName = globalsName + "_GATE";

	CPrinter globalsPrinter( globalsName );

	globalsPrinter.outputToHeader();
	globalsPrinter.print(  std::string( "#ifndef " ) + gateName + getEndl()  );
	globalsPrinter.print(  std::string( "#define " ) + gateName + getEndl()  );

	globalsPrinter.print( globalsPrinter.getIndent() + "typedef struct {" + getEndl() );
	globalsPrinter.increaseIndent();

	globalsPrinter.print( globalsPrinter.getIndent() );
	globalsPrinter.iterate(  localVarSet, std::string( ";" ) + getEndl() + globalsPrinter.getIndent()  );
	globalsPrinter.print( std::string( ";" ) + getEndl() );

	globalsPrinter.decreaseIndent();
	globalsPrinter.print( globalsPrinter.getIndent() + "} " + structName + ";" + getEndl() + getEndl() );
	globalsPrinter.print( globalsPrinter.getIndent() + "extern " + structName + " " + globalsName + ";" + getEndl() );
	globalsPrinter.print(  std::string( "#endif" ) + getEndl()  );

	globalsPrinter.outputToSource();
	globalsPrinter.print(  std::string( "#include \"" ) + globalsPrinter.getHeaderFileName() + "\"" + getEndl()  );
	globalsPrinter.print( globalsPrinter.getIndent() + structName + " " + globalsName + ";" + getEndl() );

	return;
}

void CPrinter::print( SFC::Project project ) {

	bool sfcLegacy = false;

	ProgramSet programSet = project.Program_kind_children_sorted( StatementSorter() );
	for( ProgramSet::iterator pgsItr = programSet.begin() ; !sfcLegacy && pgsItr != programSet.end() ; ++pgsItr ) {
		sfcLegacy = static_cast< __int64 >( pgsItr->statementCount() ) < 0;
	}
	if ( !sfcLegacy ) printGlobals( project );

	makeIncludeMap( project );
	makeStructFiles( project );

	for( ProgramSet::iterator pgsItr = programSet.begin() ; pgsItr != programSet.end() ; ++pgsItr ) {
		sfcLegacy = static_cast< __int64 >( pgsItr->statementCount() ) < 0;
		CPrinter cPrinter( *pgsItr, "  ", sfcLegacy );
		cPrinter.print();
	}
}

SFC::Statement CPrinter::getLocalVarContainer( SFC::LocalVar localVar ) {
	SFC::Statement statement = SFC::Statement::Cast( localVar.GetParent() );
	while( statement != Udm::null ) {
		if (
		 statement.type() == SFC::Program::meta ||
		 statement.type() == SFC::Struct::meta && statement.GetParent().type() == SFC::Project::meta
		) return statement;
		statement = SFC::Statement::Cast( statement.GetParent() );
	}

	std::cerr << "ERROR: NULL LocalVar container!!" << std::endl;
	return statement;
}


void CPrinter::addStatementToIncludeMap( SFC::Struct sfcStruct, SFC::Statement statement ) {

	TypedEntitySet typedEntitySet = sfcStruct.te();
	for(
	 TypedEntitySet::const_iterator tesItr = typedEntitySet.begin() ; tesItr != typedEntitySet.end() ; (void)++tesItr
	) {
		SFC::TypedEntity typedEntity = *tesItr;
		if ( typedEntity.type() != SFC::LocalVar::meta ) continue;

		SFC::Statement teStatement = getLocalVarContainer(  SFC::LocalVar::Cast( typedEntity )  );
		if ( teStatement == statement ) continue;

		IncludeMap::iterator stmItr = getIncludeMap().find( teStatement );
		if ( stmItr == getIncludeMap().end() ) {
			getIncludeMap().insert(  std::make_pair( teStatement, UnorderedStatementSet() )  );
			stmItr = getIncludeMap().find( teStatement );
		}
		stmItr->second.insert( statement );
	}
}


void CPrinter::makeIncludeMap( SFC::Project project ) {

	// GET ALL GLOBAL STRUCTS
	StructSet structSet = project.Struct_kind_children();
	for( StructSet::const_iterator stsItr = structSet.begin() ; stsItr != structSet.end() ; (void)++stsItr ) {
		addStructToIncludeMap( *stsItr );
	}

	// GET ALL CLASS STRUCTS
	ProgramSet programSet = project.Program_kind_children_sorted( StatementSorter() );
	for( ProgramSet::const_iterator pgsItr = programSet.begin() ; pgsItr != programSet.end() ; (void)++pgsItr ) {
		ClassSet classSet = pgsItr->Class_kind_children_sorted( StatementSorter() );
		for( ClassSet::const_iterator clsItr = classSet.begin() ; clsItr != classSet.end() ; (void)++clsItr ) {
			StructSet structSet = clsItr->Struct_kind_children();
			if ( !structSet.empty() ) addStatementToIncludeMap( *structSet.begin(), *pgsItr );
		}
	}

}

void CPrinter::printIncludes( SFC::Statement statement ) {

	IncludeMap::iterator stmItr = getIncludeMap().find( statement );
	if ( stmItr != getIncludeMap().end() ) {
		UnorderedStatementSet statementSet = stmItr->second;
		for(
		 UnorderedStatementSet::const_iterator stsItr = statementSet.begin() ;
		 stsItr != statementSet.end() ;
		 (void)++stsItr
		) {
			SFC::Statement statement = *stsItr;
			std::string name = convertToIdentifierName( 
			 statement.type() == SFC::Struct::meta ? SFC::Struct::Cast( statement ).name() : SFC::Program::Cast( statement ).filename()
			);
			print(  std::string( "#include \"" ) + name + ".h\"" + getEndl()  );
		}
	}

	print( getEndl() );
}

void CPrinter::makeStructFiles( SFC::Project project ) {

	StructSet structSet = project.Struct_kind_children();
	for( StructSet::const_iterator stsItr = structSet.begin() ; stsItr != structSet.end() ; (void)++stsItr ) {
		SFC::Struct sfcStruct = *stsItr;

		CPrinter cPrinter(  convertToIdentifierName( sfcStruct.name() )  );

		cPrinter.printIncludes( sfcStruct );
		cPrinter.print( sfcStruct );
	}
}

void CPrinter::print( void ) {
	outputToHeader();
	printIfDefGuard();
	printStandardMacros();
	printIncludes();
	printTypedefs();
	printClassStructs();
	printPrimaryFunctionHeaders();
	if ( !_sfcLegacy ) printSimplifiedPrimaryFunctionHeaders();
	printIfDefGuardEnd();

	outputToSource();
	printHeaderFileInclude();
	printClassStaticVariables();
	if ( !_sfcLegacy ) printSimplifiedPrimaryFunctions();
	printProgramUsercode();
	printFunctionHeaders();
	printDeclarations();
	printFunctions();
}


void CPrinter::print( SFC::Arg arg ) {
	SFC::DT dt = arg.dt();
	if ( dt == Udm::null ) {
		printWithoutDT( arg );
		return;
	}

	printWithDT( arg );
}


void CPrinter::print( SFC::ArgDeclRef argDeclRef ) {
	SFC::ArgDeclBase argDeclBase = argDeclRef.argdecl();
	if ( argDeclBase == Udm::null ) {
		print( argDeclRef.unres() );
	} else if ( argDeclBase.type() == SFC::StateLabel::meta ) {
		print(  SFC::StateLabel::Cast( argDeclBase ).name()  );
	} else if (  Udm::IsDerivedFrom( argDeclBase.type(), SFC::Var::meta )  ) {
		if ( _sfcLegacy ) {
			if ( argDeclBase.type() == SFC::LocalVar::meta ) {
				SFC::LocalVar localVar = SFC::LocalVar::Cast( argDeclBase );
				if ( localVar.GetParent() == _program && !localVar.Static() ) {
					print( "thread_context->" );
					SetVarSet setVarSet = localVar.arg();
					if ( setVarSet.empty() ) {
						print( "context->" );
					} else {
						SFC::SetVar setVar = *setVarSet.begin();
						if ( setVar.invert() ) print( "context->" );
					}
				}
			} else if ( argDeclBase.type() == SFC::StateVar::meta ) {
				print( "thread_context->context->" );
			}
		}

		if ( argDeclBase.GetParent().type() == SFC::Project::meta ) {
			print( getGlobalsName() + "." );
		}

		print(   convertToIdentifierName(  SFC::Var::Cast( argDeclBase ).name()  )   );
	} else if (  argDeclBase.type() == SFC::Arg::meta ) {
		SFC::Arg arg = SFC::Arg::Cast( argDeclBase );
		if ( arg.ptr() ) print( "*" );
		print(  convertToIdentifierName( arg.name() )  );
	} else assert( false );
}


void CPrinter::print( SFC::ArgVal argVal ) {
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
		if ( arg.ptr() ) print( "&" );
	}
	SFC::Exprs argexpr = argVal.argexpr();
	if ( argexpr != Udm::null ) {
		print( argexpr );
	} else {
		std::string val = argVal.val();
		if ( !val.empty() ) print( val );
	}
}


void CPrinter::printProgramUsercode() {
	UserCodeSet userCodeSet = _program.UserCode_kind_children_sorted( StatementSorter() );
	if ( !userCodeSet.empty() ) {
		iterate( userCodeSet, getEndl() );
		print( getEndl() );
	}
}

void CPrinter::print( SFC::BinaryExprs binaryExprs ) {
	std::string op( binaryExprs.op() );

	int this_precedence = OpPred::getBinaryPrecedence( op );

	SFC::Exprs leftExprs = SFC::Exprs::Cast( binaryExprs.leftexpr() );
	int leftExprs_precedence = OpPred::getOperatorPrecedence( leftExprs );

	bool parensNeeded = this_precedence > leftExprs_precedence;
	if ( parensNeeded ) print( "( " );
	print( leftExprs );
	if ( parensNeeded ) print( " )" );

	SFC::Exprs rightExprs = SFC::Exprs::Cast( binaryExprs.rightexpr() );

	if ( op == "[" ) {
		print( "[ (int)( " );
		print( rightExprs );
		print( " ) ]" );
		return;
	}

	bool spaces = !( op == "." );
	if ( spaces ) print( " " );
	print( op );
	if ( spaces ) print( " " );

	int rightExprs_precedence = OpPred::getOperatorPrecedence( rightExprs );

	parensNeeded = this_precedence > rightExprs_precedence || this_precedence == rightExprs_precedence && op != "." && op != "?";
	if ( parensNeeded ) print( "( " );
	print( rightExprs );
	if ( parensNeeded ) print( " )" );
}


void CPrinter::print( SFC::CheckArg checkArg ) {
	SFC::StateLabel stateLabel = checkArg.slab_end();
	SFC::Arg arg = checkArg.arg_end();

	print(  static_cast< std::string >( stateLabel.name() ) + " != " + convertToIdentifierName( arg.name() )  );
}


void CPrinter::print( SFC::CheckState checkState ) {
	SFC::StateVar stateVar = checkState.svar();
	SFC::StateLabel indexStateLabel = checkState.index();
	SFC::StateLabel valueStateLabel = checkState.value();

	if ( _sfcLegacy ) print( "thread_context->context->" );

	print(
	 static_cast< std::string >( stateVar.name() ) + "[ " +
	 static_cast< std::string >( indexStateLabel.name() ) + " & 0xFFFF ]"
	);
	if ( checkState.andState() ) {
		print(  std::string( " & ( " ) + static_cast< std::string >( valueStateLabel.name() ) + " >> 16 )"  );
		print( checkState.invert() ? " == 0" : " != 0" );
	} else {
		print( checkState.invert() ? " != " : " == " );
		print( valueStateLabel.name() );
	}
}

void CPrinter::print( SFC::Comment comment ) {

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

bool CPrinter::isEmpty( SFC::Statement statement ) {

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

void CPrinter::print( SFC::CompoundStatement compoundStatement ) {

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
		print(  SFC::IterativeBlock::Cast( compoundStatement )  );
	} else if ( csType == SFC::ConditionalBlock::meta ) {
		printIndent();
		print(  SFC::ConditionalBlock::Cast( compoundStatement )  );
	} else {
		assert( false );
	}

	print( getEndl() );
}


void CPrinter::printCompoundStatement( SFC::CompoundStatement compoundStatement ) {

	print( getEndl() );
	printIndent(); print( "{" + getEndl() );
	increaseIndent();

	{
		DeclarationSet declarationSet = compoundStatement.Declaration_kind_children_sorted( StatementSorter() );
		StatementVector statementVector =
		 for_each( declarationSet.begin(), declarationSet.end(), StatementTranslator() ).getStatementVector();
		if ( statementVector.size() > 0 ) {
			iterate( statementVector );
			print( getEndl() );
		}
	}
	
	if ( _sfcLegacy ) {
		if ( compoundStatement.type() == SFC::Function::meta ) {
			SFC::Function function = SFC::Function::Cast( compoundStatement );
			std::string functionName = convertToIdentifierName( function.name() );
			if ( functionName == _mainFunctionName || functionName == _initFunctionName ) {
				printIndent(); print( _threadContextName + " thread_context_base;" + getEndl() );
				printIndent(); print( _threadContextName + " *thread_context = &thread_context_base;" + getEndl() );
				printIndent(); print( "thread_context->context = context;" + getEndl() + getEndl() );
			}
		}
	}
	StatementSet statementSet = compoundStatement.stmnt_sorted( StatementSorter() );	
	statementSet = for_each( statementSet.begin(), statementSet.end(), DeclarationFilter() ).getStatementSet();
	iterate( statementSet );

	decreaseIndent();
	printIndent(); print( "}" + getEndl() );
}


void CPrinter::print( SFC::Condition condition ) {
	if ( condition.type() == SFC::CheckArg::meta )        print(  SFC::CheckArg::Cast( condition )  );
	else if ( condition.type() == SFC::CheckState::meta ) print(  SFC::CheckState::Cast( condition )  );
	else if ( condition.type() == SFC::UserCode::meta )   print(  SFC::UserCode::Cast( condition )  );
	else assert( false );
}


void CPrinter::print( SFC::ConditionalBlock conditionalBlock ) {

	printConditionalBlockHeader( conditionalBlock );
	printCompoundStatement( conditionalBlock );
}


void CPrinter::printConditionalBlockHeader( SFC::ConditionalBlock conditionalBlock ) {
	print( "if ( " );
	ConditionSet conditionSet = conditionalBlock.cond();
	switch( conditionSet.size() ) {
		case 0:
				print( "1" );
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

void CPrinter::printConditionalGroup( SFC::ConditionalGroup conditionalGroup ) {
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
			print( comment );
		} else if ( statement.type() == SFC::ConditionalBlock::meta ) {
			SFC::ConditionalBlock conditionalBlock = SFC::ConditionalBlock::Cast( statement );
			if (  isEmpty( conditionalBlock )  ) continue;
			print( getIndent() );
			if ( notFirstCondition ) print( "else " );
			else                     notFirstCondition = true;
			print( conditionalBlock );
		} else {
			std::cerr << "WARNING: Unexpected Statement type \"" << statement.type().name() << "\" encountered in ConditionalGroup." << std::endl;
		}
	}
}

void CPrinter::printDeclarations( void ) {
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

	
void CPrinter::print( SFC::Declaration declaration ) {
	if ( declaration.type() == SFC::StateLabel::meta )    print(  SFC::StateLabel::Cast( declaration )  );
	else if ( declaration.type() == SFC::StateVar::meta ) print(  SFC::StateVar::Cast( declaration )  );
	else if ( declaration.type() == SFC::LocalVar::meta ) print(  SFC::LocalVar::Cast( declaration )  );
	else assert( false );
}


void CPrinter::printWithoutDT( SFC::ArgDeclBase argDeclBase ) {
	if ( argDeclBase.type() == SFC::Arg::meta ) {
		SFC::Arg arg = SFC::Arg::Cast( argDeclBase );
		std::string type = CTypeMap::lookup( static_cast< std::string >( arg.type() ) );
		print( type ); print( " " );
		__int64 size = arg.size();
		if ( arg.ptr() ) {
			if ( size > 1 ) print( "(" ); 
			print( "*" );
			print(  convertToIdentifierName( arg.name() )  );
			if ( size > 1 ) print( ")" ); 
		} else {
			print(  convertToIdentifierName( arg.name() )  );
		}
		if ( size > 1 ) print(  std::string( "[" ) + boost::lexical_cast< std::string >( size ) + "]"  );
	} else {
		SFC::Var var = SFC::Var::Cast( argDeclBase );
		std::string type = CTypeMap::lookup( static_cast< std::string >( var.type() ) );
		print( type ); print( " " ); print(  convertToIdentifierName( var.name() )  );
		__int64 size = var.size();
		if ( size > 1 ) print(  std::string( "[" ) + boost::lexical_cast< std::string >( size ) + "]"  );
	}
}


void CPrinter::printWithDT( SFC::ArgDeclBase argDeclBase ) {

	SFC::DT dt = argDeclBase.dt();

	Uml::Class dtType = dt.type();
	std::string varName;
	if ( argDeclBase.type() == SFC::Arg::meta ) {
		SFC::Arg arg = SFC::Arg::Cast( argDeclBase );
		if ( arg.ptr() ) {
			bool hasIndexes = false;
			if ( dtType == SFC::Array::meta ) hasIndexes = true;
			if ( hasIndexes ) varName += "(";
			varName += "*";
			varName += convertToIdentifierName( arg.name() );
			if ( hasIndexes ) varName += ")";
		} else {
			varName += convertToIdentifierName( arg.name() );
		}
	} else {
		SFC::Var var = SFC::Var::Cast( argDeclBase );
		varName += convertToIdentifierName( var.name() );
	}

	printVar( dt, varName );
}

void CPrinter::printVar( SFC::DT dt, const std::string &varName ) {

	SFC::DT baseType = SFCTypesManager::getBaseDT( dt );
	std::string baseTypeName;
	if ( baseType.type() == SFC::Struct::meta ) {
		SFC::Struct sfcStruct = SFC::Struct::Cast( dt );
		baseTypeName =
		 static_cast< __int64 >( sfcStruct.memberCount() ) >= 0 && isEmpty( sfcStruct ) ?
		 "void" : convertToIdentifierName(  getStructName( sfcStruct )  );
	} else { // BasicType
		baseTypeName = CTypeMap::lookup( baseType.name() );
	}
	print( baseTypeName + " " + varName );
	while( dt.type() == SFC::Array::meta ) {
		SFC::Array sfcArray = SFC::Array::Cast( dt );
		if ( sfcArray.noelem() > 1 ) {
			print(  std::string( "[" ) + boost::lexical_cast< std::string >( sfcArray.noelem() ) + "]"  );
		}
		dt = sfcArray.dt();
	}
}

void CPrinter::print( SFC::Exprs exprs ) {
	if ( exprs == Udm::null ) return;

	if ( exprs.type() == SFC::BinaryExprs::meta )       print(  SFC::BinaryExprs::Cast( exprs )  );
	else if ( exprs.type() == SFC::UnaryExprs::meta )   print(  SFC::UnaryExprs::Cast( exprs )  );
	else if ( exprs.type() == SFC::Int::meta )          print(  SFC::Int::Cast( exprs )  );
	else if ( exprs.type() == SFC::Double::meta )       print(  SFC::Double::Cast( exprs )  );
	else if ( exprs.type() == SFC::Str::meta )          print(  SFC::Str::Cast( exprs )  );
	else if ( exprs.type() == SFC::ArgDeclRef::meta )   print(  SFC::ArgDeclRef::Cast( exprs )  );
	else if ( exprs.type() == SFC::FunctionCall::meta ) print(  SFC::FunctionCall::Cast( exprs )  );
	else assert( false );
}


//
// FUNCTION PROCESSING
//
CPrinter::FunctionVector CPrinter::getFunctions( void ) {
	FunctionVector functionVector;

	{
		FunctionSet functionSet = _program.Function_kind_children_sorted( StatementSorter() );
		functionVector =
		 for_each( functionSet.begin(), functionSet.end(), EmptyFunctionFilter( *this ) ).getFunctionVector();
	}

	{
		ClassSet classSet = _program.Class_kind_children_sorted( StatementSorter() );
		FunctionVector classFunctionVector =
		 for_each( classSet.begin(), classSet.end(), ClassFunctions() ).getFunctionVector();
		classFunctionVector = for_each(
		 classFunctionVector.begin(), classFunctionVector.end(), EmptyFunctionFilter( *this )
		).getFunctionVector();

		functionVector.insert( functionVector.end(), classFunctionVector.begin(), classFunctionVector.end() );
	}

	return functionVector;
}

void CPrinter::printFunction( SFC::Function function ) {
	if (  isEmpty( function ) && !isPrimaryFunction( function ) || isStatusFunction( function )  ) return;
	printFunctionHeader( function );
	printCompoundStatement( function );
}

bool CPrinter::isStatusFunction( SFC::Function function ) {
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

void CPrinter::printFunctionHeader( SFC::Function function ) {
	SFC::DT dt = function.dt();
	if ( dt == Udm::null ) {
		std::string returnType = CTypeMap::lookup(   static_cast< std::string >( function.returnType() )   );
		print( returnType );
	} else {
		printFunctionType( dt );
	}

	std::string functionName = convertToIdentifierName( function.name() );
	print( " " + functionName + "( " );
	if ( _sfcLegacy ) {
		if ( functionName == _mainFunctionName || functionName == _initFunctionName ) {
			print( _contextName + " *context" );
		} else {
			print( _threadContextName + " *thread_context" );
		}
	}
	ArgSet argSet = function.Arg_children_sorted( ArgSorter() );
	if ( _sfcLegacy ) {
		if ( !argSet.empty() ) print( ", " );
	}
	iterate( argSet, ", " );
	print( " )" );
}

std::string CPrinter::getSimplifiedName( const std::string &complexName ) {
	std::string::size_type lastUnderscorePos = complexName.rfind( '_' );
	if ( lastUnderscorePos == std::string::npos ) return complexName;
	std::string::size_type nextToLastUnderscorePos = complexName.rfind( '_', lastUnderscorePos - 1 );
	if ( nextToLastUnderscorePos == std::string::npos ) return complexName;

	return complexName.substr( 0, nextToLastUnderscorePos ) + complexName.substr( lastUnderscorePos );
}

bool CPrinter::isPrimaryFunction( SFC::Function function ) {
	FunctionCallSet functionCallSet = function.caller();

	// ANY FUNCTIONS WITH NO CALLS TO THEM ARE PRIMARY.  THIS GENERALLY INCLUDES CONSTRUCTOR ("INIT")
	// FUNCTIONS WHICH ARE ONLY CALLED WHEN THEIR CONTAINING PROGRAM IS THE POINT-OF-ENTRY FOR EXECUTION.
	if ( functionCallSet.empty() ) return true;

	// ELIMINATE ANY FUNCTIONS THAT HAVE CALLS TO THEM WITHIN THEIR CONTAINING PROGRAM.
	SFC::Program program = getProgram();
	for(
	 FunctionCallSet::iterator fcsItr = functionCallSet.begin() ;
	 fcsItr != functionCallSet.end() ;
	 (void)++fcsItr
	) {
		if ( getContainingProgram( *fcsItr ) == program  ) return false;
	}

	// OF THE REMAINING FUNCTIONS, THOSE THAT ARE CONSIDERED "PRIMARY" ARE THOSE THAT ARE *NOT* CONSTRUCTORS
	std::string functionName = convertToIdentifierName( function.name() );
	return functionName.substr( functionName.size() - 4 ) != "init";
}

void CPrinter::printPrimaryFunctionHeaders( void ) {
	FunctionVector functionVector = getFunctions();

	FunctionVector primaryFunctionVector = for_each(
	 functionVector.begin(), functionVector.end(), PrimaryFunctionFilter( *this )
	).getFunctionVector();

	iterate( primaryFunctionVector );

	if ( !primaryFunctionVector.empty() ) print( getEndl() + getEndl() );
}

void CPrinter::printSimplifiedPrimaryFunctionHeaders( void ) {
	FunctionVector functionVector = getFunctions();

	FunctionVector primaryFunctionVector = for_each(
	 functionVector.begin(), functionVector.end(), PrimaryFunctionFilter( *this )
	).getFunctionVector();

	print( getEndl() );
	
	print(  std::string( "/* SIMPLIFIED PROGRAM FUNCTIONS */" ) + getEndl()  );
	for(
	 FunctionVector::iterator fnvItr = primaryFunctionVector.begin() ;
	 fnvItr != primaryFunctionVector.end() ;
	 ++fnvItr
	) {
		std::string complexName = convertToIdentifierName( fnvItr->name() );
		std::string simplifiedName = getSimplifiedName( complexName );
		if ( simplifiedName == complexName ) continue;
		fnvItr->name() = simplifiedName;
		printIndent(); printFunctionHeader( *fnvItr ); print( ";" + getEndl() );
		fnvItr->name() = complexName;
	}
}

void CPrinter::printFunctionHeaders( void ) {
	FunctionVector functionVector = getFunctions();

	FunctionVector nonPrimaryFunctionVector = for_each(
	 functionVector.begin(), functionVector.end(), PrimaryFunctionFilter( *this, true )
	).getFunctionVector();

	iterate( nonPrimaryFunctionVector );

	if ( !nonPrimaryFunctionVector.empty() ) print( getEndl() + getEndl() );
}

void CPrinter::printSimplifiedPrimaryFunctions( void ) {
	FunctionVector functionVector = getFunctions();

	FunctionVector primaryFunctionVector = for_each(
	 functionVector.begin(), functionVector.end(), PrimaryFunctionFilter( *this )
	).getFunctionVector();

	print( getEndl() );
	print(  std::string( "/* SIMPLIFIED PROGRAM FUNCTIONS */" ) + getEndl()  );
	for(
	 FunctionVector::iterator fnvItr = primaryFunctionVector.begin(); fnvItr != primaryFunctionVector.end() ; ++fnvItr
	) {
		std::string complexName = convertToIdentifierName( fnvItr->name() );
		std::string simplifiedName = getSimplifiedName( complexName );
		if ( complexName == simplifiedName ) continue;
		fnvItr->name() = simplifiedName;
		printIndent(); printFunctionHeader( *fnvItr ); print( " {" + getEndl() );
		fnvItr->name() = complexName;

		increaseIndent(); printIndent(); print( complexName + "( " );
		ArgSet argSet = fnvItr->Arg_children_sorted( ArgSorter() );
		bool printComma = false;
		for( ArgSet::iterator agsItr = argSet.begin() ; agsItr != argSet.end() ; ++agsItr ) {
			if ( printComma ) print( ", " );
			else              printComma = true;
			print(  convertToIdentifierName( agsItr->name() )  );
		}
		print( " );" + getEndl() );
		decreaseIndent(); printIndent(); print( "}" + getEndl() + getEndl() );
	}
}

void CPrinter::printFunctionType( SFC::DT dt ) {
	if ( dt == Udm::null ) return;

	if ( dt.type() == SFC::BasicType::meta ) {
		SFC::BasicType basicType = SFC::BasicType::Cast( dt );
		std::string typeName = CTypeMap::lookup( basicType.name() );
		print( typeName );
	} else if ( dt.type() == SFC::Struct::meta ) {
		SFC::Struct sfcStruct = SFC::Struct::Cast( dt );
		print(  getStructName( sfcStruct )  );
	} else { // SFC::Array
		SFC::DT arrayDT = dt;
		while( arrayDT.type() == SFC::Array::meta ) {
			SFC::Array sfcArray = SFC::Array::Cast( arrayDT );
			arrayDT = sfcArray.dt();
		}
		printFunctionType( arrayDT );
		arrayDT = dt;
		while( arrayDT.type() == SFC::Array::meta ) {
			SFC::Array sfcArray = SFC::Array::Cast( arrayDT );
			int noElem = sfcArray.noelem();
			print(  std::string( "[" ) + boost::lexical_cast< std::string >( noElem ) + "]"  );
			arrayDT = sfcArray.dt();
		}		
	}
}

//
// END FUNCTION PROCESSING
//


void CPrinter::print( SFC::FunctionCall functionCall ) {
	SFC::Function callee = functionCall.callee();
	print(  callee == Udm::null ? functionCall.libFuncName() : convertToIdentifierName( callee.name() )  );
	print( "( " );
	ArgValSet argValSet = functionCall.ArgVal_children_sorted( ArgValSorter() );
	if ( _sfcLegacy && callee != Udm::null ) {
		print( "thread_context" );
		if ( !argValSet.empty() ) print( ", " );
	}
	iterate( argValSet, ", " );
	print( " )" );
}


void CPrinter::print( SFC::IterativeBlock iterativeBlock ) {
	printIterativeBlockHeader( iterativeBlock );
	printCompoundStatement( iterativeBlock );
}


void CPrinter::printIterativeBlockHeader( SFC::IterativeBlock iterativeBlock ) {
	print( "while( " );
	ConditionSet conditionSet = iterativeBlock.cond();
	switch( conditionSet.size() ) {
		case 0:
				print( "1" );
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
void CPrinter::print( SFC::LocalVar localVar ) {
	SFC::DT dt = localVar.dt();
	if ( dt == Udm::null ) printWithoutDT( localVar );
	else                   printWithDT( localVar );
}
//
// END LocalVar PROCESSING
//


void CPrinter::print( SFC::SetState setState ) {
	SFC::StateVar stateVar = setState.svar();
	SFC::StateLabel indexStateLabel = setState.index();
	SFC::StateLabel valueStateLabel = setState.value();

	if ( _sfcLegacy ) print( "thread_context->context->" );

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
void CPrinter::printLoopStarts( SFC::DT dt ) {

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
		  boost::lexical_cast< std::string >( noelem ) + " ; (void)++" + indexVarName + " )" + getEndl()
		);
	}
}

void CPrinter::printLoopEnds( SFC::DT dt ) {
	while( dt.type() == SFC::Array::meta ) {
		SFC::Array sfcArray = SFC::Array::Cast( dt );
		int noelem = sfcArray.noelem();

		dt = sfcArray.dt();
		if ( noelem <= 1 ) continue;

		decreaseIndent();
		printIndent(); print(  std::string( "}" ) + getEndl()  );
	}
}

void CPrinter::printIndexes( SFC::DT dt ) {

	while( dt.type() == SFC::Array::meta ) {
		SFC::Array sfcArray = SFC::Array::Cast( dt );
		int noelem = sfcArray.noelem();

		dt = sfcArray.dt();
		if ( noelem <= 1 ) continue;

		std::string indexVarName = std::string( "ix" ) + boost::lexical_cast< std::string >( dt.uniqueId() );
		print(  std::string( "[" ) + indexVarName + "]"  );
	}
}

void CPrinter::print( SFC::SetVar setVar ) {
	SFC::Arg arg = setVar.arg_end();
	std::string argIdentifierName = convertToIdentifierName( arg.name() );

	SFC::LocalVar localVar = setVar.lvar_end();
	std::string localVarIdentifierName = convertToIdentifierName( localVar.name() );

	SFC::DT dt = arg.dt();
	if ( dt == Udm::null ) {
		printIndent();
		if ( setVar.invert() ) {
			if ( arg.ptr() ) print( "*" );
			print( argIdentifierName ); print( " = " );
			if ( _sfcLegacy ) {
				if ( localVar.GetParent() == _program ) {
					print( "thread_context->context->" );
				}
			}
			print( localVarIdentifierName );
		} else {
			if ( _sfcLegacy ) {
				if ( localVar.GetParent() == _program ) {
					print( "thread_context->" );
				}
			}
			print( localVarIdentifierName ); print( " = " );
			if ( arg.ptr() ) print( "*" );
			print( argIdentifierName );
		}
		print( ";" + getEndl() );
		return;
	}

	printLoopStarts( dt );

	printIndent(); print( "{" + getEndl() );
	increaseIndent();

	printIndent();
	if ( setVar.invert() ) {
		if ( arg.ptr() ) print( "(*" );
		print(  argIdentifierName  );
		if ( arg.ptr() ) print( ")" );
		printIndexes( dt );
		print( " = " );
		if ( _sfcLegacy ) {
			if ( localVar.GetParent() == _program ) {
				print( "thread_context->context->" );
			}
		}
		print( localVarIdentifierName ); printIndexes( dt );
	} else {
		if ( _sfcLegacy ) {
			if ( localVar.GetParent() == _program ) {
				print( "thread_context->" );
			}
		}
		print( localVarIdentifierName ); printIndexes( dt );
		print( " = " );
		if ( arg.ptr() ) print( "(*" );
		print( argIdentifierName );
		if ( arg.ptr() ) print( ")" );
		printIndexes( dt );
	}
	print( ";" + getEndl() );
	decreaseIndent();
	printIndent();
	print( "}" + getEndl() );

	printLoopEnds( dt );
}
//
// END SetVar PROCESSING
//


void CPrinter::print( SFC::StateLabel stateLabel ) {
	ostringstream toHex;
	toHex << std::hex << static_cast< int >( stateLabel.value() );
	std::string hexValue = toHex.str();

	print(
	 std::string( "static const int " ) + static_cast< std::string >( stateLabel.name() ) + " = 0x" + hexValue
	);
}


void CPrinter::print( SFC::Statement statement ) {

	// CHECK FOR EMPTY SFC::Statement'S
	if (  isEmpty( statement )  ) return;

	if (  Udm::IsDerivedFrom( statement.type(), SFC::CompoundStatement::meta )  ) {
		print(  SFC::CompoundStatement::Cast( statement )  );
		return;
	}

	if ( statement.type() == SFC::SetVar::meta ) {
		print(  SFC::SetVar::Cast( statement )  );
		return;
	}

	if ( statement.type() == SFC::Comment::meta ) {
		print(  SFC::Comment::Cast( statement )  );
		return;
	}

	printIndent();

	if (  Udm::IsDerivedFrom( statement.type(), SFC::Declaration::meta )  ) {
		print(  SFC::Declaration::Cast( statement )  );
	}
	else if ( statement.type() == SFC::Return::meta )       print(  SFC::Return::Cast( statement )  );
	else if ( statement.type() == SFC::UserCode::meta )     print(  SFC::UserCode::Cast( statement )  );
	else if ( statement.type() == SFC::FunctionCall::meta ) print(  SFC::FunctionCall::Cast( statement )  );
	else if ( statement.type() == SFC::SetState::meta )     print(  SFC::SetState::Cast( statement )  );
	else assert( false );

	print( ";" + getEndl() );
}


//
// Standard DEFINEs
// 
void CPrinter::printStandardMacros( void ) {
	print( "#ifndef NO_MATH_H" + getEndl() );
	print( "#include <math.h>" + getEndl() );
	print( "#endif" + getEndl() );

	SFC::Project project = SFC::Project::Cast( getProgram().GetParent() );
	LocalVarSet localVarSet = project.LocalVar_kind_children_sorted( StatementSorter() );
	if ( !localVarSet.empty() ) print( "#include \"" + getGlobalsName() + ".h\"" + getEndl() );
	print( "#ifndef min" + getEndl() );
	print( "#define min(x,y) (((x)>(y)) ? (y) : (x))" + getEndl() );
	print( "#endif" + getEndl() + getEndl() );
	print( "#ifndef sign" + getEndl() );
	print( "#define sign(x) ( ((x) > 0.0) ? (1.0)  : (((x) < 0.0 ) ? (-1.0) : (0.0)) )" + getEndl() );
	print( "#endif" + getEndl() + getEndl() );
	print( "#ifndef max" + getEndl() );
	print( "#define max(x,y) (((x)<(y)) ? (y) : (x))" + getEndl() );
	print( "#endif" + getEndl() + getEndl() );
}

//
// ifndef guard
//

void CPrinter::printIfDefGuard( void ) {
	std::string guardVar = "_" + getHeaderFileName().substr( 0, getHeaderFileName().length() - 2) + "_H_";
	print( "#ifndef " + guardVar + getEndl() );
	print( "#define " + guardVar + getEndl() + getEndl() );
}

//
// TYPEDEFS
//
void CPrinter::printTypedefs( void ) {
	SFC::Project project = SFC::Project::Cast( _program.GetParent() );

	DTSet dtSet = project.DT_kind_children();
	for( DTSet::iterator dtsItr = dtSet.begin() ; dtsItr != dtSet.end() ; ++dtsItr ) {
		SFC::DT dt = *dtsItr;
		SFC::DT baseDT = SFCTypesManager::getBaseDT( dt );
		if ( baseDT.type() == SFC::Struct::meta ) continue;
		std::string name = dt.name();
		if (  baseDT.type() == SFC::BasicType::meta && isBuiltInTypeName( name )  ) continue;
		name = convertToIdentifierName(  getBasicCTypeName( name )  );
		std::string cppGuard = name + "_GUARD";
		print(  std::string( "#ifndef " ) + cppGuard + getEndl()  );
		print(  std::string( "#define " ) + cppGuard + getEndl()  );
		print( "typedef " );
		printVar( dt, name );
		print(  std::string( ";" ) + getEndl()  );
		print(  std::string( "#endif" ) + getEndl()  );
	}
	print( getEndl() );
}

std::string CPrinter::getBasicCTypeName( std::string basicTypeName ) {
	std::string::size_type charPos = 0;
	while(   (  charPos = basicTypeName.find( "(", charPos )  ) != std::string::npos   ) {
		basicTypeName.erase( charPos, 1 );
	}
	charPos = 0;
	while(   (  charPos = basicTypeName.find( ")", charPos )  ) != std::string::npos   ) {
		basicTypeName.erase( charPos, 1 );
	}
	charPos = 0;
	while(   (  charPos = basicTypeName.find( "'", charPos )  ) != std::string::npos   ) {
		basicTypeName.replace( charPos, 1, "_" );
	}
	return basicTypeName;
}
//
// Struct PROCESSING
//
void CPrinter::printClassStaticVariables( void ) {

	if ( _sfcLegacy ) {
		LocalVarSet localVarSet = _program.LocalVar_kind_children_sorted( StatementSorter() );

		IOLocalVarFilter ioLocalVarFilter = for_each( localVarSet.begin(), localVarSet.end(), IOLocalVarFilter() );
		LocalVarSet staticLocalVarSet  = ioLocalVarFilter.getStaticLocalVarSet();

		if ( staticLocalVarSet.empty() ) return;

		print( getIndent() + "static " );
		iterate(  staticLocalVarSet, std::string( ";" ) + getEndl() + getIndent() + "static "  );
		print( std::string( ";" ) + getEndl() );

		print( getEndl() );
		return;
	}

	ClassSet classSet = _program.Class_kind_children_sorted( StatementSorter() );

	for( ClassSet::iterator clsItr = classSet.begin() ; clsItr != classSet.end() ; ++clsItr ) {

		SFC::Class sfcClass = *clsItr;
		LocalVarSet localVarSet = sfcClass.LocalVar_kind_children_sorted( StatementSorter() );
		if ( localVarSet.empty() ) continue;

		print( getIndent() + "static " );
		iterate(  localVarSet, std::string( ";" ) + getEndl() + getIndent() + "static "  );
		print( std::string( ";" ) + getEndl() );

		print( getEndl() );
	}
}

void CPrinter::printClassStructs( void ) {
	LocalVarSet localVarSet = _program.LocalVar_kind_children_sorted( StatementSorter() );

	IOLocalVarFilter ioLocalVarFilter = for_each( localVarSet.begin(), localVarSet.end(), IOLocalVarFilter() );
	LocalVarSet inputLocalVarSet  = ioLocalVarFilter.getInputLocalVarSet();
	LocalVarSet outputLocalVarSet = ioLocalVarFilter.getOutputLocalVarSet();
	LocalVarSet otherLocalVarSet = ioLocalVarFilter.getOtherLocalVarSet();

	if ( _sfcLegacy ) {

		StateVarSet stateVarSet = _program.StateVar_kind_children();
		if ( !stateVarSet.empty() ) {
			print( "typedef struct {" + getEndl() );
			increaseIndent();

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

			decreaseIndent();
			print( std::string( "} " ) + _contextName + ";" + getEndl() + getEndl() );
		}

		if ( !stateVarSet.empty() || !localVarSet.empty() ) {
			print( "typedef struct {" + getEndl() );
			if ( !stateVarSet.empty() ) print(  std::string( "  " ) + _contextName + " *context;" + getEndl()  );
			increaseIndent();

			StatementVector statementVector = for_each(
			 inputLocalVarSet.begin(), inputLocalVarSet.end(), StatementTranslator()
			).getStatementVector();
			iterate( statementVector );

			decreaseIndent();
			print(  std::string( "} " ) + _threadContextName + ";" + getEndl() + getEndl()  );
		}
	}
	ClassSet classSet = _program.Class_kind_children_sorted( StatementSorter() );
	StructSet structSet = for_each( classSet.begin(), classSet.end(), ClassStruct() ).getStructSet();
	iterate( structSet );

	if ( !structSet.empty() ) {
		print(  std::string( "/* SIMPLIFIED PROGRAM CONTEXT */" ) + getEndl()  );
		SFC::Struct sfcStruct = *structSet.begin();
		std::string simplifiedName = getSimplifiedName(  convertToIdentifierName( sfcStruct.name() )  );
		printIndent();
		if (  isEmpty( sfcStruct )  ) {
			print(  std::string( "typedef void *" ) + simplifiedName + ";" + getEndl()  );
		} else {
			print(
			 std::string( "typedef " ) + convertToIdentifierName( sfcStruct.name() ) + " " +
			  simplifiedName + ";" + getEndl()
			);
		}
		print( getEndl() );
	}
}

void CPrinter::printStructs( void ) {
	SFC::Project project = SFC::Project::Cast( _program.GetParent() );
	if ( project == Udm::null ) return;

	bool printNewlines = false;

	StructSet structSet = project.Struct_kind_children();
	iterate( structSet );	
	if ( structSet.size() > 0 ) printNewlines = true;

	structSet = _program.Struct_kind_children();
	iterate( structSet );	
	if ( structSet.size() > 0 ) printNewlines = true;

	if ( printNewlines ) print( getEndl() + getEndl() );
}


void CPrinter::print( SFC::Struct sfcStruct ) {
	if (  structProcessed( sfcStruct )  ) return;

	__int64 memberCount = sfcStruct.memberCount();
	if ( memberCount < 0 ) {
		std::string structName = getStructName( sfcStruct );
		std::string includeFileName = structName.substr(  0, structName.find( "_context" )  ) + ".h";
		print( "#include \"" + includeFileName + "\"" + getEndl() + getEndl() );
		return;
	}

	if (  isEmpty( sfcStruct )  ) return;

	DeclarationSet declarationSet = sfcStruct.Declaration_kind_children_sorted( StatementSorter() );
	declarationSet =
	 for_each( declarationSet.begin(), declarationSet.end(), EmptyDeclarationFilter() ).getStatementSet();
	StructSet structSet =
	 std::for_each( declarationSet.begin(), declarationSet.end(), StructFilter() ).getStructSet();
	iterate( structSet );

	StatementVector statementVector =
	 for_each( declarationSet.begin(), declarationSet.end(), StatementTranslator() ).getStatementVector();

	std::string structName = convertToIdentifierName(  getStructName( sfcStruct )  );
	printIndent();
	print(  std::string( "#ifndef " ) + structName + "_GUARD" + getEndl()  );
	print(  std::string( "#define " ) + structName + "_GUARD" + getEndl()  );

	print(  std::string( "typedef struct {" ) + getEndl()  );

	increaseIndent();
	iterate( statementVector );
	decreaseIndent();

	printIndent();
	print(  std::string( "} " ) + structName + ";" + getEndl()  );
	print(  std::string( "#endif" ) + getEndl() + getEndl()  );
}
//
// END Struct PROCESSING
//


void CPrinter::print( SFC::UnaryExprs unaryExprs ) {
	std::string op = unaryExprs.op();
	if ( op.empty() ) {
		print( unaryExprs.subexpr() );
		return;
	}

	int this_precedence = OpPred::getUnaryPrecedence( op );

	SFC::Exprs subExprs = SFC::Exprs::Cast( unaryExprs.subexpr() );
	int subExpr_precedence = OpPred::getOperatorPrecedence( subExprs );

	bool parensNeeded = this_precedence > subExpr_precedence;

	bool postOp = op.substr( 0, 4 ) == "post";

	if ( !postOp ) print( op );
	if( parensNeeded ) print( "( " );
	print( unaryExprs.subexpr() );
	if( parensNeeded ) print( " )" );
	if ( postOp ) print(  op.substr( 4 )  );
}
