#include "Uml.h"

#include "StateLabelInfo.hpp"
#include "CallGraph.hpp"

void CallGraph::init( const SFC::Program &program ) {
	StateLabelInfo::init( program );
//	StateLabelInfo::dump();

	std::string basename = program.filename();
	basename = basename.substr( 0, basename.find(".") );

	std::string initSuffix( "_init" );
	std::string mainSuffix( "_main" );

	SFC::Function mainFunction;
	SFC::Function initFunction;

	FunctionSet functionSet = program.Function_kind_children();

	for( FunctionSet::iterator fnsItr = functionSet.begin() ; fnsItr != functionSet.end() ; ++fnsItr ) {
		SFC::Function function = *fnsItr;
		std::string functionName( function.name() );

		CallGraph::registerFuncCoverage( functionName );

		if (  functionName.substr( 0, basename.size() ) == basename  ) {
			if ( functionName.substr( functionName.size() - initSuffix.size() ) == initSuffix ) {
				initFunction = function;
			} else if ( functionName.substr( functionName.size() - mainSuffix.size() ) == mainSuffix ) {
				_mainFunctionName = functionName;
				mainFunction = function;
			}
		}
	}

	if ( initFunction != Udm::null ) {
		FunctionProcessor initFunctionProcessor( initFunction, ArgValProcessor(), _graph );
		initFunctionProcessor.processSelf();
	}

	if ( mainFunction != Udm::null ) {
		FunctionProcessor mainFunctionProcessor( mainFunction, ArgValProcessor(), _graph );
		mainFunctionProcessor.processSelf();
	}

//	FunctionProcessor::makeArbitraryCallGraphEdge( "MissionPhaseStates_702_enter(-2,131085)", "GPS_737_exec" );
//	FunctionProcessor::makeArbitraryCallGraphEdge( "On_102_enter(-2,1)", "On_102_enter(0,0)" );

	listUncalledFunctions();
	listLackingFunctions();
	findDeadCode();
}

CallGraph::CallGraph( const std::string &filename ) {

	Udm::SmartDataNetwork smartDataNetwork( SFC::diagram );
	smartDataNetwork.OpenExisting( filename.c_str(), "SFC.xsd", Udm::CHANGES_PERSIST_ALWAYS );

	const SFC::Program program = SFC::Program::Cast( smartDataNetwork.GetRootObject() );

	init( program );
}

CallGraph::CallGraph( const SFC::Program &program ){
	init( program );
}

int CallGraph::exprsCount( const SFC::Exprs &exprs ) {
	int count = 0;
	if (  Udm::IsDerivedFrom( exprs.type(), SFC::BinaryExprs::meta )  ) {
		SFC::BinaryExprs binaryExprs = SFC::BinaryExprs::Cast( exprs );
		if (  static_cast< std::string >( binaryExprs.op() )  ==  ";"  ) {
			count += 1 + exprsCount( binaryExprs.leftexpr() );
		}
	} else {
		++count;
	}
	return count;
}

int CallGraph::subStatementCount( const SFC::CompoundStatement &compoundStatement ) {
	int count = 0;
	StatementSet statementSet = compoundStatement.stmnt_sorted( StatementOrder() );
	for( StatementSet::iterator stsItr = statementSet.begin() ; stsItr != statementSet.end() ; ++stsItr ) {
		const SFC::Statement &statement = *stsItr;
		if (  Udm::IsDerivedFrom( statement.type(), SFC::CompoundStatement::meta )  ) {
			count += subStatementCount(  SFC::CompoundStatement::Cast( statement )  );
		} else if (  Udm::IsDerivedFrom( statement.type(), SFC::UserCode::meta )  ) {
			SFC::UserCode userCode = SFC::UserCode::Cast( statement );
			SFC::Exprs exprs = userCode.codeexpr();
			if ( exprs != Udm::null ) {
				count += exprsCount( exprs );
			} else if (  !static_cast< std::string >( userCode.expr() ).empty()  ) {
				++count;
			}
		} else {
			++count;
		}
	}
	return count;
}

void CallGraph::displayDeadCond( SFC::CompoundStatement compoundStatement ) {
	typedef std::list< SFC::CompoundStatement > CompoundStatementList;
	CompoundStatementList compoundStatementList;

	while(  !Udm::IsDerivedFrom( compoundStatement.type(), SFC::Function::meta )  ) {
		compoundStatementList.push_front( compoundStatement );
		compoundStatement = SFC::CompoundStatement::Cast( compoundStatement.parent() );
	}

	std::string spaces;
	std::cerr << "In Function \"" << SFC::Function::Cast( compoundStatement ).name() << "\"" << std::endl;
	for(
	 CompoundStatementList::iterator cslItr = compoundStatementList.begin() ;
	 cslItr != compoundStatementList.end() ;
	 ++cslItr
	) {
		spaces += "  ";

		if (   Udm::IsDerivedFrom(  ( *cslItr ).type(), SFC::ConditionalBlock::meta  )   ) {
			SFC::ConditionalBlock conditionalBlock = SFC::ConditionalBlock::Cast( *cslItr );
			std::cerr << spaces << "In the ConditionalBlock with " <<
			 identifyConditionalBlock( conditionalBlock ) << std::endl;
		} else if (  Udm::IsDerivedFrom(  ( *cslItr ).type(), SFC::ConditionalGroup::meta  )   ) {
			SFC::ConditionalGroup conditionalGroup = SFC::ConditionalGroup::Cast( *cslItr );
			std::cerr << spaces << "In the ConditionalGroup with " <<
			 identifyConditionalGroup( conditionalGroup ) << std::endl;
		}
	}
	std::cerr << std::endl;
}

void CallGraph::listUncalledFunctions( void ) {
	
	bool uncoveredFunction = false;

	for(
	 FuncCoverageMap::iterator fcmItr = getFuncCoverageMap().begin() ;
	 fcmItr != getFuncCoverageMap().end() ;
	 (void)++fcmItr
	) {
		if ( !fcmItr->second ) {
			if ( !uncoveredFunction ) {
				uncoveredFunction = true;
				std::cerr << "THE FOLLOWING FUNCTIONS ARE NEVER CALLED:" << std::endl;
			}
			std::cerr << fcmItr->first << std::endl;
		}
	}
	std::cerr << std::endl;
}

void CallGraph::listLackingFunctions( void ) {

	if ( getFuncCondCoverageSet().empty() ) return;

	std::cerr << "THE FOLLOWING FUNCTIONS ARE (POTENTIALLY) MISSING CONDITIONALS:" << std::endl;

	for(
	 FuncCondCoverageSet::iterator fcsItr = getFuncCondCoverageSet().begin() ;
	 fcsItr != getFuncCondCoverageSet().end() ;
	 (void)++fcsItr
	) {
		std::cerr << *fcsItr << std::endl;
	}
	std::cerr << std::endl;
}

void CallGraph::findDeadCode( void ) {
	bool deadCode = false;

	for(
	 CondBlockTakenMap::iterator ctmItr = CallGraph::getCondBlockTakenMap().begin() ;
	 ctmItr != CallGraph::getCondBlockTakenMap().end() ;
	 ++ctmItr
	) {
		int count = subStatementCount( ctmItr->first );
		if (  !ctmItr->second  &&  count != 0  ) {
			if ( !deadCode ) {
				deadCode = true;
				std::cerr << "DEAD CODE DETECTED:" << std::endl;
			}
			displayDeadCond( ctmItr->first );
		}
	}
}

//
// FunctionProcessor
//
void CallGraph::FunctionProcessor::init( const ArgValProcessor &argValProcessor ) {

	CallGraph::getFuncCoverageMap()[ _functionName ] = true;

	_entryMode = argValProcessor.getEntryModeArg();
	_tpp = argValProcessor.getTppArg();

	std::string dottyLabel = getExtendedName();

	NameVertexMap &nameVertexMap = getNameVertexMap();

	NameVertexMap::iterator lvmItr = nameVertexMap.find( dottyLabel );
	if ( lvmItr == nameVertexMap.end() ) {
		_hasBeenTraversed = false;

		CallGraph::Vertex newVertex = boost::add_vertex( _graph );
		_graph[ newVertex ].name = dottyLabel;

		if ( _function != Udm::null ) {
			StateLabelInfo::AndSSMap::const_iterator sasItr =
			 StateLabelInfo::getAndSSMap().find(  std::string( _function.scope() )  );
			if (  sasItr != StateLabelInfo::getAndSSMap().end()  &&  int( sasItr->second ) != 0  ) {
				_graph[ newVertex ].isAndSS = true;
			}
		}

		nameVertexMap.insert(  NameVertexMap::value_type( dottyLabel, newVertex )  );

	} else {

		_hasBeenTraversed = true;

	}

	return;
}

CallGraph::FunctionProcessor::~FunctionProcessor( void ) {

	if ( getFuncCondBlockTakenMap().empty() ) return;

	bool definiteCond = false;
	bool allIndefiniteCond = true;
	for(
	 FuncCondBlockTakenMap::iterator ctmItr = getFuncCondBlockTakenMap().begin() ;
	 ctmItr != getFuncCondBlockTakenMap().end() ;
	 (void)++ctmItr
	) {
		definiteCond = definiteCond || ( ctmItr->second ).first;
		allIndefiniteCond = allIndefiniteCond && ( ctmItr->second ).second;
	}
	if ( !definiteCond && !allIndefiniteCond ) getFuncCondCoverageSet().insert( getExtendedName() );
}

bool CallGraph::FunctionProcessor::processStatement( const SFC::Statement &statement ) {

//	std::cerr << "function: " << getExtendedName() <<
//	 ", statement: " << static_cast< int >( statement.statementIndex() ) << std::endl;

	// PROCESS ConditionalGroup
	if (  Udm::IsDerivedFrom( statement.type(), SFC::ConditionalGroup::meta )  ) {
		return processConditionalGroup(  SFC::ConditionalGroup::Cast( statement )  );
	}

	// PROCESS ConditionalBlock
	if (  Udm::IsDerivedFrom( statement.type(), SFC::ConditionalBlock::meta )  ) {
		SFC::ConditionalBlock conditionalBlock = SFC::ConditionalBlock::Cast( statement );
		ConditionSet conditionSet = conditionalBlock.Condition_kind_children();

		ConditionalBlockProcessor conditionalBlockProcessor =
		 for_each(  conditionSet.begin(), conditionSet.end(), ConditionalBlockProcessor( *this )  );

		CallGraph::registerCondBlock( conditionalBlock );
		funcRegisterCondBlock( conditionalBlock );

		getFuncCondBlockTakenMap()[ conditionalBlock ] =
		 std::make_pair( conditionalBlockProcessor.getDefinite(), conditionalBlockProcessor.getIndefinite() );

		if ( conditionalBlockProcessor.getResult() ) {
			CallGraph::getCondBlockTakenMap()[ conditionalBlock ] = true;
			return processCompoundStatement(  SFC::CompoundStatement::Cast( conditionalBlock )  );
		}

		return false;
	}

	// PROCESS CompoundStatement
	if (  Udm::IsDerivedFrom( statement.type(), SFC::CompoundStatement::meta )  ) {
		return processCompoundStatement(  SFC::CompoundStatement::Cast( statement )  );
	}

	//
	// SimpleStatement's
	//

	// PROCESS FunctionCall
	if (  Udm::IsDerivedFrom( statement.type(), SFC::FunctionCall::meta )  ) {
		return processFunctionCall(  SFC::FunctionCall::Cast( statement )  );
	}

	// PROCESS UserCode
	if (  Udm::IsDerivedFrom( statement.type(), SFC::UserCode::meta )  ) {
		const SFC::UserCode userCode = SFC::UserCode::Cast( statement );
		if (  static_cast< std::string >( userCode.expr() )  ==  ""  ) return false;
		( void )processExprs(  SFC::Exprs::Cast( userCode.codeexpr() )  );
		return false;
	}

	// PROCESS Return
	if (  Udm::IsDerivedFrom( statement.type(), SFC::Return::meta )  ) {
		SFC::Return returnStatement = SFC::Return::Cast( statement );
		if (  static_cast< std::string >( returnStatement.val() )  ==  ""  ) return true;
		( void )processExprs(  SFC::Exprs::Cast( returnStatement.retexpr() )  );
		return true;
	}

	return false;
}

bool CallGraph::FunctionProcessor::processConditionalGroup( const SFC::ConditionalGroup &conditionalGroup ) {

	SFC::CompoundStatement compoundStatement = SFC::CompoundStatement::Cast( conditionalGroup );
	StatementSet statementSet = compoundStatement.stmnt_sorted( StatementOrder() );

	for( StatementSetItr stsItr = statementSet.begin() ; stsItr != statementSet.end() ; ++stsItr ) {
		// ConditionalGroup ONLY CONTAINS ConditionalBlock's
		SFC::ConditionalBlock conditionalBlock = SFC::ConditionalBlock::Cast( *stsItr );
		CallGraph::registerCondBlock( conditionalBlock );
		funcRegisterCondBlock( conditionalBlock );
	}

	// PROCESS ConditionalBlocks INSIDE ConditionalGroup
	for( StatementSetItr stsItr = statementSet.begin() ; stsItr != statementSet.end() ; ++stsItr ) {

		// ConditionalGroup ONLY CONTAINS ConditionalBlock's
		SFC::ConditionalBlock conditionalBlock = SFC::ConditionalBlock::Cast( *stsItr );

		// GET THE Condition's FOR THE ConditionalBlock
		ConditionSet conditionSet = conditionalBlock.Condition_kind_children();

		// EVALUATE THE Condition's
		ConditionalBlockProcessor conditionalBlockProcessor =
		 for_each(  conditionSet.begin(), conditionSet.end(), ConditionalBlockProcessor( *this )  );

		getFuncCondBlockTakenMap()[ conditionalBlock ] =
		 std::make_pair( conditionalBlockProcessor.getDefinite(), conditionalBlockProcessor.getIndefinite() );

		// IF THE Condition (COULD HAVE) EVALUATED TO TRUE, GO THROUGH BODY
		if ( conditionalBlockProcessor.getResult() ) {
			CallGraph::getCondBlockTakenMap()[ conditionalBlock ] = true;
			bool retval = processCompoundStatement( SFC::CompoundStatement::Cast( conditionalBlock ) );
			if ( conditionalBlockProcessor.getTerminate() ) return retval;
		}

	}

	return false;
}
	
bool CallGraph::FunctionProcessor::processCompoundStatement( const SFC::CompoundStatement &compoundStatement ) {
	StatementSet statementSet = compoundStatement.stmnt_sorted( StatementOrder() );

	for( StatementSet::iterator sstItr = statementSet.begin() ; sstItr != statementSet.end() ; ++sstItr ) {
		if (  processStatement( *sstItr )  ) return true;
	}

	return false;
}

bool CallGraph::FunctionProcessor::processFunctionCall( const SFC::FunctionCall &functionCall ) {

	ArgValSet argValSet = functionCall.ArgVal_kind_children();
	ArgValProcessor argValProcessor = for_each(  argValSet.begin(), argValSet.end(), ArgValProcessor( *this )  );

	std::string libFuncName = functionCall.libFuncName();

	if ( libFuncName.empty() ) {

		/* MAKE LINK FROM StatementProcessor's FUNCTION TO THIS FUNCTION */
		SFC::Function function = SFC::Function::Cast( functionCall.callee() );

		/* IN EXPRESSIONS (EXPR's), 'tpp' AND 'entryMode' ARGS NOT REALLY NEEDED */
		FunctionProcessor functionProcessor( function, argValProcessor, _graph );
		makeCallGraphEdge( functionProcessor );

		if ( !functionProcessor.hasBeenTraversed() ) functionProcessor.processSelf();

		return false;
	}

	makeCallGraphEdge(  FunctionProcessor( libFuncName, ArgValProcessor(), _graph )  );

	return false;
}

CallGraph::Ternary CallGraph::FunctionProcessor::processExprs( const SFC::Exprs &exprs ) {

	if (  Udm::IsDerivedFrom( exprs.type(), SFC::BinaryExprs::meta )  ) {
		return processBinaryExprs(  SFC::BinaryExprs::Cast( exprs )  );
	}
	
	if (  Udm::IsDerivedFrom( exprs.type(), SFC::UnaryExprs::meta )  ) {
		return processUnaryExprs(  SFC::UnaryExprs::Cast( exprs )  );
	}
	
	if (  Udm::IsDerivedFrom( exprs.type(), SFC::NullaryExprs::meta )  ) {
		return processNullaryExprs(  SFC::NullaryExprs::Cast( exprs )  );
	}

	return PASS;
}

CallGraph::Ternary CallGraph::FunctionProcessor::processBinaryExprs( const SFC::BinaryExprs &binaryExprs ) {

	SFC::Exprs leftexprs  = SFC::Exprs::Cast( binaryExprs.leftexpr() );
	SFC::Exprs rightexprs = SFC::Exprs::Cast( binaryExprs.rightexpr() );

	std::string op = binaryExprs.op();

	Ternary	retTernary;

	// "entryMode" EXPRESSION ALWAYS INVOLVES COMPARISON WITH AN INTEGER
	if (  isEntryMode( leftexprs )  ) {

		int rightExprsVal = getIntVal( rightexprs );

		int entryMode = getEntryMode();

		if ( op == ">" )       retTernary = entryMode >  rightExprsVal ? TRUE : FALSE;
		else if ( op == ">=" ) retTernary = entryMode >= rightExprsVal ? TRUE : FALSE;
		else if ( op == "<" )  retTernary = entryMode <  rightExprsVal ? TRUE : FALSE;
		else if ( op == "<=" ) retTernary = entryMode <= rightExprsVal ? TRUE : FALSE;
		else if ( op == "==" ) retTernary = entryMode == rightExprsVal ? TRUE : FALSE;
		else                   retTernary = entryMode != rightExprsVal ? TRUE : FALSE;

		return retTernary;
	}

	if (  isEntryMode( rightexprs )  ) {

		int leftExprsVal = getIntVal( leftexprs );

		int entryMode = getEntryMode();

		if ( op == ">" )       retTernary = leftExprsVal >  entryMode ? TRUE : FALSE;
		else if ( op == ">=" ) retTernary = leftExprsVal >= entryMode ? TRUE : FALSE;
		else if ( op == "<" )  retTernary = leftExprsVal <  entryMode ? TRUE : FALSE;
		else if ( op == "<=" ) retTernary = leftExprsVal <= entryMode ? TRUE : FALSE;
		else if ( op == "==" ) retTernary = leftExprsVal == entryMode ? TRUE : FALSE;
		else                   retTernary = leftExprsVal != entryMode ? TRUE : FALSE;

		return retTernary;
	}

	Ternary leftexprsTernary  = processExprs( leftexprs );
	Ternary rightexprsTernary = processExprs( rightexprs );

	if ( op == "&&" ) {
		if ( leftexprsTernary == FALSE || rightexprsTernary == FALSE ) retTernary = FALSE;
		else if ( leftexprsTernary == TRUE )                           retTernary = rightexprsTernary;
		else if ( rightexprsTernary == TRUE )                          retTernary = leftexprsTernary;
		else                                                           retTernary = PASS;
	} else if ( op == "||" ) {
		if ( leftexprsTernary == TRUE || rightexprsTernary == TRUE ) retTernary = TRUE;
		else if ( leftexprsTernary == FALSE )                        retTernary = rightexprsTernary;
		else if ( rightexprsTernary == FALSE )                       retTernary = leftexprsTernary;
		else                                                         retTernary = PASS;
	} else {
		retTernary = PASS;
	}

	return retTernary;
}

CallGraph::Ternary CallGraph::FunctionProcessor::processUnaryExprs( const SFC::UnaryExprs &unaryExprs ) {
	std::string op = unaryExprs.op();

	Ternary subexprTernary = processExprs(  SFC::Exprs::Cast( unaryExprs.subexpr() )  );

	Ternary retTernary;
	if ( op == "!" ) {
		if ( subexprTernary == FALSE )    retTernary = TRUE;
		else if (subexprTernary == PASS ) retTernary = PASS;
		else                              retTernary = FALSE;
	} else {
		retTernary = PASS;
	}

	return retTernary;
}

CallGraph::Ternary CallGraph::FunctionProcessor::processNullaryExprs( const SFC::NullaryExprs &nullaryExprs ) {
	if (  Udm::IsDerivedFrom( nullaryExprs.type(), SFC::FunctionCall::meta )  ) {
		processFunctionCall(  SFC::FunctionCall::Cast( nullaryExprs )  );
		return PASS;
	}
	if (  Udm::IsDerivedFrom( nullaryExprs.type(), SFC::Int::meta )  ) {
		int value = SFC::Int::Cast( nullaryExprs ).val();
		Ternary retTernary = value ? TRUE : FALSE;
		return retTernary;
	}
	if (  Udm::IsDerivedFrom( nullaryExprs.type(), SFC::Double::meta )  ) {
		double value = SFC::Double::Cast( nullaryExprs ).val();
		Ternary retTernary = value ? TRUE : FALSE;
		return retTernary;
	}

	return PASS;
}

void CallGraph::FunctionProcessor::makeCallGraphEdge( const FunctionProcessor &functionProcessor ) const {

	NameVertexMap &nameVertexMap = getNameVertexMap();

	NameVertexMap::iterator nvmItr1 = nameVertexMap.find( getExtendedName() );
	NameVertexMap::iterator nvmItr2 = nameVertexMap.find( functionProcessor.getExtendedName() );

	boost::add_edge( nvmItr1->second, nvmItr2->second, _graph );

	return;
}

int CallGraph::FunctionProcessor::getIntVal( const SFC::Exprs &exprs ) const {

	if (  Udm::IsDerivedFrom( exprs.type(), SFC::ArgDeclRef::meta )  ) {
		SFC::ArgDeclRef argDeclRef = SFC::ArgDeclRef::Cast( exprs );
		SFC::ArgDeclBase argDeclBase = SFC::ArgDeclBase::Cast( argDeclRef.argdecl() );

		if ( argDeclBase != Udm::null ) {

			if (  Udm::IsDerivedFrom( argDeclBase.type(), SFC::Declaration::meta )  ) {
				SFC::Declaration declaration = SFC::Declaration::Cast( argDeclBase );

				if (  Udm::IsDerivedFrom( declaration.type(), SFC::StateLabel::meta )  ) {
					SFC::StateLabel stateLabel = SFC::StateLabel::Cast( declaration );

					StateLabelInfo::StateNumberMap::const_iterator stnItr =
					 StateLabelInfo::getStateNumberMap().find( stateLabel.name() );
					if ( stnItr != StateLabelInfo::getStateNumberMap().end() ) return stnItr->second;
				}

			} else {

				SFC::Arg arg = SFC::Arg::Cast( argDeclBase );

				std::string argName = arg.name();

				if ( argName == "tpp" ) return getTpp();
				if ( argName == "entryMode" ) return getEntryMode();
			}
		}

		return 0;
	}

	// ASSUME INTEGER FROM THIS POINT
	SFC::Exprs auxExprs = exprs;
	bool negFlag = false;

	if (  Udm::IsDerivedFrom( exprs.type(), SFC::UnaryExprs::meta )  ) {
		SFC::UnaryExprs unaryExprs = SFC::UnaryExprs::Cast( exprs );
		negFlag = true;
		auxExprs = SFC::Exprs::Cast( unaryExprs.subexpr() );
	}

	int retval = SFC::Int::Cast( auxExprs ).val();
	return negFlag ? -retval : retval;
}

bool CallGraph::FunctionProcessor::isEntryMode( const SFC::Exprs &exprs ) {
	if (  !Udm::IsDerivedFrom( exprs.type(), SFC::ArgDeclRef::meta )  ) return false;

	SFC::ArgDeclRef argDeclRef = SFC::ArgDeclRef::Cast( exprs );
	SFC::ArgDeclBase argDeclBase = SFC::ArgDeclBase::Cast( argDeclRef.argdecl() );
	if (  argDeclBase == Udm::null || !Udm::IsDerivedFrom( argDeclBase.type(), SFC::Arg::meta )  ) return false;

	SFC::Arg arg = SFC::Arg::Cast( argDeclBase );
	if (  static_cast< std::string >( arg.name() ) != "entryMode"  ) return false;

	return true;
}

//
// ConditionalBlockProcessor
//
void CallGraph::ConditionalBlockProcessor::operator()( const SFC::Condition &condition ) {

	if (  Udm::IsDerivedFrom( condition.type(), SFC::UserCode::meta )  ) {
		SFC::UserCode userCode = SFC::UserCode::Cast( condition );
		if (  static_cast< std::string >( userCode.expr() ) == ""  ) return;

		Ternary exprsTernary = _functionProcessor.processExprs(  SFC::Exprs::Cast( userCode.codeexpr() )  );

		_result     = _result     && ( exprsTernary == PASS || exprsTernary == TRUE );
		_terminate  = _terminate  && exprsTernary == TRUE;
		_definite   = _definite   || exprsTernary == TRUE;
		_indefinite = _indefinite && exprsTernary == PASS;

		return;
	}

	if (  Udm::IsDerivedFrom( condition.type(), SFC::CheckArg::meta )  ) {
		SFC::CheckArg checkArg = SFC::CheckArg::Cast( condition );
		SFC::Arg arg = SFC::Arg::Cast( checkArg.arg_end() );
		if ( static_cast< std::string >( arg.name() ) == "tpp" ) {
			SFC::StateLabel stateLabel = SFC::StateLabel::Cast( checkArg.slab_end() );
			StateLabelInfo::StateNumberMap::const_iterator snhItr =
			 StateLabelInfo::getStateNumberMap().find(  static_cast< std::string >( stateLabel.name() )  );
			bool tppCheck = snhItr->second != getTpp();
			_result     = _result   && tppCheck;
			_definite   = _definite || tppCheck;
			_indefinite = false;
		}

		return;
	}

	if (  Udm::IsDerivedFrom( condition.type(), SFC::CheckState::meta )  ) {
		_terminate = false;
		return;
	}

	return;
}

//
// ArgValProcessor
//
void CallGraph::ArgValProcessor::operator()( const SFC::ArgVal &argVal ) {
	SFC::Exprs exprs = SFC::Exprs::Cast( argVal.argexpr() );

	ArgSet argSet = argVal.arg();
	if ( !argSet.empty() ) {
		SFC::Arg arg = *argSet.begin();
		std::string argName = arg.name();
		_argExprsMap[ argName ] = exprs;

		if ( argName == "entryMode" || argName == "tpp" ) {
			_argValMap[ argName ] = _functionProcessor->getIntVal( exprs );
		}
	}

	_functionProcessor->processExprs( exprs );
}

void CallGraph::DFSAnalysis::forward_or_cross_edge( Edge edge, const Graph &graph ) {
	if ( _maxDepth < 0 ) return;

	Vertex sourceVertex = boost::source( edge, graph );
	Vertex targetVertex = boost::target( edge, graph );

	DepthData &sourceDepthData = _depthMap.find( sourceVertex )->second;
	DepthData &targetDepthData = _depthMap.find( targetVertex )->second;

	int currentTargetDepth = targetDepthData._depth;
	int newTargetDepth = sourceDepthData._depth + 1;

	if ( newTargetDepth > currentTargetDepth ) {
		int newMaxDepth = newTargetDepth + targetDepthData._maxDepth - currentTargetDepth;
		if ( sourceDepthData._maxDepth < newMaxDepth ) {
			sourceDepthData._maxDepth = newMaxDepth;
			if ( _diagnostics ) _successorMap[ sourceVertex ] = targetVertex;
		}
	}
}

void CallGraph::DFSAnalysis::tree_edge( Edge edge, const Graph &graph ) {

	// DON'T RECORD PREDECESSORS IF CYCLES DON'T NEED TO BE REPORTED
	if ( _diagnostics ) {
		_predecessorMap[ boost::target( edge, graph ) ] = boost::source( edge, graph );
	}

	if ( _maxDepth < 0 ) return;

	Vertex sourceVertex = boost::source( edge, graph );
	Vertex targetVertex = boost::target( edge, graph );

	// NO POINT CALCULATING MAXIMUM DEPTH IF CYCLES

	DepthMapItr srcDpmItr = _depthMap.find( sourceVertex );

	if ( srcDpmItr == _depthMap.end() ) {

		_depthMap.insert(   std::make_pair(  sourceVertex, DepthData( -1, -1 )  )   );
		_depthMap.insert(   std::make_pair(  targetVertex, DepthData( -1, -1 )  )   );
		return;
	}

	if ( srcDpmItr->second._depth == -1 ) {
		_depthMap.insert(   std::make_pair(  targetVertex, DepthData( -1, -1 )  )   );
		return;
	}

	int targetDepth = srcDpmItr->second._depth + 1;
	_depthMap.insert(   std::make_pair(  targetVertex, DepthData( targetDepth )  )   );

	return;
}

void CallGraph::DFSAnalysis::back_edge( Edge edge, const Graph &graph ) {

	_maxDepth = -1;

	if ( !_diagnostics ) return;

	std::cerr << std::endl << "Cycle Detected:" << std::endl;

	typedef std::list< Vertex > VertexList;
	typedef VertexList::iterator VertexListItr;

	VertexList vertexCycle;

	Vertex cycleStart(  boost::target( edge, graph )  );
	vertexCycle.push_front( cycleStart );

	Vertex curCycleVertex(  boost::source( edge, graph )  );
	while( curCycleVertex != cycleStart ) {
		vertexCycle.push_front( curCycleVertex );
		curCycleVertex = _predecessorMap.find( curCycleVertex )->second;
	}
	vertexCycle.push_front( cycleStart );

	for( VertexListItr vxlItr = vertexCycle.begin() ; vxlItr != vertexCycle.end() ; ++vxlItr ) {
		std::cerr << graph[ *vxlItr ].name << std::endl;
	}
	std::cerr << std::endl;
}

void CallGraph::DFSAnalysis::finish_vertex( Vertex vertex, const Graph &graph ) {
	if ( _maxDepth < 0 ) return;

	OutEdgeItrPair outEdgeItrPair = boost::out_edges( vertex, graph );

	DepthData &sourceDepthData = _depthMap.find( vertex )->second;

	if ( outEdgeItrPair.first == outEdgeItrPair.second ) {
		sourceDepthData._maxDepth = sourceDepthData._depth;
		return;
	}

	for( OutEdgeItr oedItr = outEdgeItrPair.first ; oedItr != outEdgeItrPair.second ; ++oedItr ) {
		Vertex targetVertex = boost::target( *oedItr, graph );

		int targetMaxDepth = _depthMap.find( targetVertex )->second._maxDepth;
		if ( sourceDepthData._maxDepth < targetMaxDepth ) {
			sourceDepthData._maxDepth = targetMaxDepth;
			if ( _diagnostics ) _successorMap[ vertex ] = targetVertex;
		}
	}

	if ( vertex != _startVertex ) return;

	_maxDepth = _depthMap.find( _startVertex )->second._maxDepth;

	if ( !_diagnostics ) return;

	std::cerr << "Maximum Call Sequence:" << std::endl;
	std::cerr << graph[ _startVertex ].name << std::endl;

	VertexSequenceMap::iterator vsmItr = _successorMap.find( _startVertex );
	while( vsmItr != _successorMap.end() ) {
		Vertex currentVertex = vsmItr->second;
		std::cerr << graph[ currentVertex ].name << std::endl;
		vsmItr = _successorMap.find( currentVertex );
	}
	std::cerr << std::endl;
	
}
