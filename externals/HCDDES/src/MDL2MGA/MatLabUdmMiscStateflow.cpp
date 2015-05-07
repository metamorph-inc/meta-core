/*
 *	Copyright (c) Vanderbilt University, 2006
 *	ALL RIGHTS RESERVED
 *
 *	Vanderbilt University disclaims all warranties with regard to this
 *	software, including all implied warranties of merchantability
 *	and fitness.  In no event shall Vanderbilt University be liable for
 *	any special, indirect or consequential damages or any damages
 *	whatsoever resulting from loss of use, data or profits, whether
 *	in an action of contract, negligence or other tortious action,
 *	arising out of or in connection with the use or performance of
 *	this software.
 *
 *  FILE NAME:	MatLabUdmMiscStateflow.cpp 
 *	SYNOPSIS:	Implementation of classes:
 *				"MatLabUdmFunction", translates SL/SF 'Stateflow.Function' objects into equivalent GME ESMoL State models.
 *				"MatLabUdmJunction", translates SL/SF 'Stateflow.Junction' objects into equivalent GME ESMoL
 *				Junction/History atoms.
 *				"MatLabUdmEvent", translates SL/SF 'Stateflow.Event' objects into equivalent GME ESMoL Event atoms.
 *				"MatLabUdmChart", translates SL/SF 'Stateflow.Chart' objects into equivalent GME ESMoL State models.
 *  AUTHOR:		Harmon Nine - hnine@isis.vanderbilt.edu
 *  CREATED:	07/20/06
 *  LAST UPDATE: 
 */
#include "MatLabUdmMiscStateflow.hpp"
#include <boost/regex.hpp>
#include "RegexCommon.hpp"

void MatLabUdmFunction::setupFunction( void ) {
	State udmFunction = getUdmFunction();

	udmFunction.Name() = udmFunction.name() = getUniqueName().c_str();
	udmFunction.Decomposition() = "FUNC_STATE";
	udmFunction.Order() = "-1";
	udmFunction.RefId() = getRefId();

	udmFunction.EnterAction() = RegexCommon::eliminateContinuations(  MatLabEngine::globalEngine().getMxStringValue( getMatLabObject() + ".LabelString" )  );
}


void MatLabUdmEMFunction::setupEMFunction( void ) {

	State udmEMFunction = getUdmEMFunction();

	udmEMFunction.Name() = udmEMFunction.name() = getUniqueName().c_str();
	udmEMFunction.RefId() = getRefId();
	udmEMFunction.Decomposition() = "FUNC_STATE";
	udmEMFunction.Order() = "-2";

	udmEMFunction.EnterAction() = RegexCommon::eliminateContinuations(  MatLabEngine::globalEngine().getMxStringValue( getMatLabObject() + ".LabelString" )  );

	std::string functionBody = RegexCommon::eliminateContinuations(  MatLabEngine::globalEngine().getMxStringValue( getMatLabObject() + ".Script" )  );
	functionBody = boost::regex_replace( functionBody, RegexCommon::getFunctionHeaderRegex(), "" );
	
	udmEMFunction.DuringAction() = functionBody;
}

MatLabUdmTruthTable::StringVectorManager MatLabUdmTruthTable::getStringVectorManager( std::string matrix ) {

	static boost::regex scPattern( "(?<!&#x[0-9A-Fa-f]{2});", boost::regex_constants::perl );
	static boost::regex scCodePattern( "&#x3[Bb];", boost::regex_constants::perl );
	static boost::regex cmPattern( ",", boost::regex_constants::perl );
	static boost::regex cmCodePattern( "&#x2[Cc];", boost::regex_constants::perl );
	static boost::regex labeledStringPattern( "\\s*(\\S+)\\s*:\\s*(.*?)\\s*\\Z", boost::regex_constants::perl | boost::regex_constants::mod_s);

	matrix = matrix.substr( 1, matrix.size() - 2 ); // GET RID OF ENCLOSING {}

	StringMatrix stringMatrix;

	bool scMatch = true;
	boost::match_results< std::string::const_iterator > results;
	while (   scMatch && (  boost::regex_search( matrix, results, scPattern ) || !(scMatch = false)  )   ) {

		std::string row;
		if ( scMatch ) {
			row = results.prefix();
			matrix = results.suffix();
		} else {
			row = matrix;
		}

		row = boost::regex_replace( row, scCodePattern, ";" );

		stringMatrix.push_back(  StringVectorSP( new StringVector )  );
		StringVector &stringVector = *stringMatrix.back();

		bool cmMatch = true;
		while(   cmMatch && (  boost::regex_search( row, results, cmPattern ) || !( cmMatch = false )  )   ) {
			std::string cell;
			if ( cmMatch ) {
				cell = results.prefix();
				row = results.suffix();
			} else {
				cell = row;
			}

			cell = boost::regex_replace( cell, cmCodePattern, "," );
			stringVector.push_back( cell );
		}
	}

	StringVectorMap stringVectorMap;
	for( StringMatrix::iterator stmItr = stringMatrix.begin() ; stmItr != stringMatrix.end() ; ++stmItr ) {
		StringVector &stringVector = **stmItr;

		std::string &labeledString = stringVector[1];
		labeledString = RegexCommon::eliminateContinuations( labeledString );

		if (  boost::regex_search( labeledString, results, labeledStringPattern )  ) {
			std::string key( results[1] );
			std::string condition( results[2] );

			stringVectorMap.insert(  std::make_pair( key, *stmItr )  );
			labeledString = condition;
		}

	}

	return StringVectorManager( stringMatrix, stringVectorMap );
}

void MatLabUdmTruthTable::setupTruthTable( void ) {
	State udmTruthTable = getUdmTruthTable();

	udmTruthTable.Name() = udmTruthTable.name() = getUniqueName().c_str();
	udmTruthTable.RefId() = getRefId();
	udmTruthTable.Decomposition() = "FUNC_STATE";
	udmTruthTable.Order() = "-2";

	udmTruthTable.EnterAction() = RegexCommon::eliminateContinuations(  MatLabEngine::globalEngine().getMxStringValue( getMatLabObject() + ".LabelString" )  );

	std::string conditionTable = MatLabEngine::globalEngine().getData( getMatLabObject() + ".ConditionTable" );
	StringVectorManager conditionSTM = getStringVectorManager( conditionTable );

	std::string actionTable = MatLabEngine::globalEngine().getData( getMatLabObject() + ".ActionTable" );
	StringVectorManager actionSTM = getStringVectorManager( actionTable );

	std::string functionBody;

	StringMatrix conditionStringMatrix = conditionSTM.getStringMatrix();
	int noConditions = (int) conditionStringMatrix[0]->size() - 2;
	int noConditionTerms = (int) conditionStringMatrix.size() - 1;

	static boost::regex crnlPattern( "\\r|\\n", boost::regex_constants::perl );

	for( int ix = 0 ; ix < noConditions ; ++ix ) {

		if ( ix != 0 ) functionBody += "else";
		functionBody += "if ";

		std::string condition;
		for( int jx = 0 ; jx < noConditionTerms ; ++jx ) {
			StringVector &stringVector = *conditionStringMatrix[ jx ];
			std::string tf = stringVector[ ix + 2 ];

			std::string rawCondition = "(" + boost::regex_replace( stringVector[1], crnlPattern, " " ) + ")";

			if (  tf.find( '-' ) != std::string::npos  ) continue;
			if (  tf.find( 'F' ) != std::string::npos  ) rawCondition = "!" + rawCondition;

			if ( !condition.empty() ) condition += " && ";
			condition += rawCondition;
		}
		if ( condition.empty() ) condition = "1";
		functionBody += condition + "\n";

		StringVector actionVector;
		std::string actions = ( *conditionStringMatrix.back() )[ ix + 2 ];

		static boost::regex actionPattern( "[0-9]+|[A-Za-z_][A-Za-z_0-9]*" );
		boost::match_results< std::string::const_iterator > results;
		while (  boost::regex_search( actions, results, actionPattern )  ) {
			actionVector.push_back( results[0] );
			actions = results.suffix();
		}

		static boost::regex newlinePattern( "\n" );
		for( StringVector::iterator stvItr = actionVector.begin() ; stvItr != actionVector.end() ; ++stvItr ) {
			std::string actionSpec = *stvItr;
			if (  isdigit( actionSpec[0] )  ) {
				int actionNo = boost::lexical_cast< int >( actionSpec ) - 1;
				std::string action = (*actionSTM.getStringMatrix()[ actionNo ])[ 1 ];
				action = boost::regex_replace( action, newlinePattern, "\n  " );
				functionBody += "  " + action + "\n";
			} else {
				std::string action = (*actionSTM.getStringVectorMap()[ actionSpec ])[ 1 ];
				action = boost::regex_replace( action, newlinePattern, "\n  " );
				functionBody += "  " + action + "\n";
			}
		}

	}

	functionBody += "end\n";

	udmTruthTable.DuringAction() = functionBody;
}


Udm::Object MatLabUdmJunction::distinguish(
 Udm::Object &udmParent, const std::string &matLabName, int matLabObjectNo
) {
	std::string matLabObject = getMatLabObjectString( matLabName, matLabObjectNo );

	std::string junctionType = MatLabEngine::globalEngine().getMxStringValue( matLabObject + ".Type" );
	transform( junctionType.begin(), junctionType.end(), junctionType.begin(), ToLower() );

	if ( junctionType == "history" ){
		_junctionType = HISTORY;
		return History::Create( udmParent );
	}
	_junctionType = JUNCTION;
	return Junction::Create( udmParent );
}


void MatLabUdmJunction::setupJunction( void ) {

	Junction junction = getUdmJunction();
	junction.RefId() = getRefId();

	registerForTransitions();
}


void MatLabUdmEvent::setupEvent( void ) {
	Event event = getUdmEvent();

	event.Description() = MatLabEngine::globalEngine().getMxStringValue( getMatLabObject() + ".Description" ).c_str();
	event.Name() = event.name() = getUniqueName().c_str();
	event.RefId() = getRefId();

	std::string eventScope = MatLabEngine::globalEngine().getMxStringValue( getMatLabObject() + ".Scope" );
	std::transform( eventScope.begin(), eventScope.end(), eventScope.begin(), ToUpper() );
	event.Scope() = ( eventScope + "_EVENT" ).c_str();

	int port = MatLabEngine::globalEngine().getMxIntValue( getMatLabObject() + ".Port" );
	event.Port() = port > 0 ? port : -1;

	static std::string funcTestString( "FUNCTION" );

	std::string eventTrigger = MatLabEngine::globalEngine().getMxStringValue( getMatLabObject() + ".Trigger" );
	std::transform( eventTrigger.begin(), eventTrigger.end(), eventTrigger.begin(), ToUpper() );
	event.Trigger() = eventTrigger.compare( 0, funcTestString.size(), funcTestString ) == 0 ? "FUNCTION_CALL_EVENT" : ( eventTrigger + "_EDGE_EVENT" ).c_str();

	SLSF::TypeBaseRef typeBaseRef = SLSF::TypeBaseRef::Create( event );

	SLSF::Matrix ESMoLMatrix = SLSF::Matrix::Create( UdmEngine::getUdmTypesRoot() );
	ESMoLMatrix.Type() = "boolean";
	ESMoLMatrix.rows() = 1;
	ESMoLMatrix.columns() = 1;

	UdmEngine::integrateTypeBase( ESMoLMatrix );
	typeBaseRef.ref() = ESMoLMatrix;
	typeBaseRef.name() = ESMoLMatrix.name();
}


Udm::Object MatLabUdmChart::distinguish( Udm::Object udmParent ) {

	SLSF::State state;

	static boost::regex stateflowRegex( "stateflow", boost::regex_constants::perl | boost::regex_constants::icase );
	boost::match_results<std::string::const_iterator> results;

	SLSF::Subsystem subsystemParent = SLSF::Subsystem::Cast( udmParent );

	BlockSet blockSet = subsystemParent.Block_kind_children();

	Udm::Object chartParent = udmParent;
	for( BlockSet::iterator blsItr = blockSet.begin() ; blsItr != blockSet.end() ; ++blsItr ) {
		Block block = *blsItr;
		std::string tag( block.Tag() );
		if (  regex_search( tag, results, stateflowRegex )  ) {
			chartParent = block;
			break;
		}
	}

#if PARADIGM == CyberComposition_PARADIGM
	state = SLSF::State::Create( chartParent );
#else
	state = SLSF::State::Create( UdmEngine::get_singleton().getTopLevelState() );
	SLSF::ConnectorRef connectorRef = SLSF::ConnectorRef::Create( chartParent );
	connectorRef.ref() = state;
#endif

	return state;
}

void MatLabUdmChart::setupChart( Udm::Object udmParent ) {

	SLSF::Block parentBlock = SLSF::Block::Cast( udmParent );
	SLSF::Parameter parameter = SLSF::Parameter::Create( parentBlock );
	parameter.RefId() = getRefId();
	parameter.name() = "ExecuteAtInitialization";
	int executeAtInitialization = MatLabEngine::globalEngine().getMxIntValue( getMatLabObject() + ".ExecuteAtInitialization" );
	parameter.Value() = executeAtInitialization ? "on" : "off";

	State chart = getUdmChart();

	chart.Name() = chart.name() = forceGetUniqueName().c_str();
	chart.RefId() = getRefId();

#if PARADIGM != CyberComposition_PARADIGM
	ConnectorRefSet connectorRefSet = chart.referedbyConnectorRef();
	for( ConnectorRefSet::iterator crsItr = connectorRefSet.begin() ; crsItr != connectorRefSet.end() ; ++crsItr ) {
		crsItr->name() = chart.name();
	}
#endif

	chart.Description() = MatLabEngine::globalEngine().getMxStringValue( getMatLabObject() + ".Description" );
	std::string decomposition = MatLabEngine::globalEngine().getMxStringValue( getMatLabObject() + ".Decomposition" );
	if (  decomposition == "OR" || decomposition.find( "_OR" )  !=  std::string::npos  ) {
		chart.Decomposition() = "OR_STATE";
	} if (  decomposition == "AND" || decomposition.find( "_AND" )  !=  std::string::npos  ) {
		chart.Decomposition() = "AND_STATE";
	}

	chart.Order() = "0";
}
