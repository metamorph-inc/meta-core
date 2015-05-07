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
 *  FILE NAME:	MatLabUdmTransition.cpp 
 *	SYNOPSIS:	Implementation of class "MatLabUdmTransition", translates SL/SF 'Stateflow.Transition'
 *				objects into equivalent GME ESMoL Transition connections.					
 *  AUTHOR:		Harmon Nine - hnine@isis.vanderbilt.edu
 *  CREATED:	07/20/06
 *  LAST UPDATE: 
 */
#include "MatLabUdmTransition.hpp"

SLSF::State MatLabUdmTransition::getSourceOrDestParent( const std::string &matLabName, int matLabObjectNo ) {

	std::string matLabObject = getMatLabObjectString( matLabName, matLabObjectNo );

	std::string sourceTest = matLabObject + ".Source";

	int transitionEndId = -1;
	if (  MatLabEngine::globalEngine().getMxLinearSize( sourceTest ) != 0 ) {
		transitionEndId = MatLabEngine::globalEngine().getMxIntValue( sourceTest + ".Id" );
	} else {
		transitionEndId = MatLabEngine::globalEngine().getMxIntValue( matLabObject + ".Destination.Id" );
	}

	return SLSF::State::Cast(  MatLabUdm::findTransConnector( transitionEndId ).parent()  );
}

void MatLabUdmTransition::setupTransition( void ) {

	Transition udmTransition = getUdmTransition();

	udmTransition.Order() = boost::lexical_cast< std::string >(
	 MatLabEngine::globalEngine().getMxIntValue( getMatLabObject() + ".ExecutionOrder" )
	);
	udmTransition.RefId() = getRefId();

	// SET UP TRANSITION SOURCE AND DESTINATION
	std::string sourceTest = getMatLabObject() + ".Source";

	if (  MatLabEngine::globalEngine().getMxLinearSize( sourceTest ) != 0 ) {
		int sourceId = MatLabEngine::globalEngine().getMxIntValue( sourceTest + ".Id" );
		udmTransition.srcTransition_end() = getTransitionEnd( sourceId );
	} else {
		udmTransition.srcTransition_end() = TransStart::Create( udmTransition.parent() );
	}

	{
		int destId = MatLabEngine::globalEngine().getMxIntValue( getMatLabObject() + ".Destination.Id" );
		udmTransition.dstTransition_end() = getTransitionEnd( destId );
	}

	// PARSE LabelString TO GET TRIGGER, GUARD, CONDITIONACTION, ACTION.
	std::string labelString = RegexCommon::eliminateContinuations(  MatLabEngine::globalEngine().getMxStringValue( getMatLabObject() + ".LabelString" )  );

	labelString = rightBracketSubstitution( labelString );

	static boost::regex labelRegex(
	 "\\A((?:/\\*.*?\\*/|%.*?\\n|//.*?\\n|[^[{/])*)?"
	 "(?:\\[((?:/\\*.*?\\*/|%.*?\\n|//.*?\\n|[^\\]])*)\\])?"
	 "((?:/\\*.*?\\*/|%.*?\\n|//.*?\\n|\\s*)*)?"
	 "(?:\\{((?:/\\*.*?\\*/|%.*?\\n|//.*?\\n|[^}])*)\\})?"
	 "((?:/\\*.*?\\*/|%.*?\\n|//.*?\\n|\\s*)*)?"
	 "(?:/(.*))?"
	);

	boost::match_results<std::string::const_iterator> results;
	regex_search( labelString, results, labelRegex );

	std::string trigger( results[1].first, results[1].second );
	trigger = dollarSignSubstitution(  trim( trigger )  );
	if ( trigger == "?" ) trigger = "";

	std::string guard( results[2].first, results[2].second );
	guard = dollarSignSubstitution(  trim( guard )  );

	std::string conditionActionComments = trim(  std::string( results[3].first, results[3].second )  );

	std::string conditionAction( results[4].first, results[4].second );
	conditionAction = dollarSignSubstitution( conditionAction );

	conditionAction = conditionActionComments + conditionAction;

	conditionActionComments = trim(  std::string( results[5].first, results[5].second )  );
	conditionAction = conditionAction + conditionActionComments;

	std::string action( results[6].first, results[6].second );
	action = dollarSignSubstitution(  trim( action )  );
	if ( !action.empty() && action[ 0 ] == '{' && action[ action.size() - 1 ] == '}' ) {
		action = trim(  action.substr( 1, action.size() - 2 )  );
	}

	udmTransition.Trigger()         = udmTransition.name() = trigger;
	udmTransition.Guard()           = guard;
	udmTransition.ConditionAction() = conditionAction;
	udmTransition.Action()          = action;
}

std::string MatLabUdmTransition::rightBracketSubstitution( std::string input ) {
	static boost::regex rightBracketRegex( "/\\*.*?\\*/|%.*?\\n|//.*?\\n|\\[|\\]" );

	int noUnbalancedLeftBrackets = 0;
	std::string output;

	boost::match_results<std::string::const_iterator> results;
	while(  regex_search( input, results, rightBracketRegex )  ) {
		output += std::string( results.prefix().first, results.prefix().second  );
		char firstMatchCharacter = *results[0].first;
		switch( firstMatchCharacter ) {
			case '[': {
				++noUnbalancedLeftBrackets;
				output += "[";
				break;
			}
			case ']': {
				--noUnbalancedLeftBrackets;
				output += noUnbalancedLeftBrackets ? "$" : "]";
				break;
			}
			default: {
				output += std::string( results[0].first, results[0].second );
				break;
			}
		}
		input = std::string( results.suffix().first, results.suffix().second );
	}
	output += input;

	return output;
}

std::string MatLabUdmTransition::dollarSignSubstitution( std::string input ) {
	static boost::regex dollarSignRegex( "/\\*.*?\\*/|%.*?\\n|//.*?\\n|\\$" );

	std::string output;

	boost::match_results<std::string::const_iterator> results;
	while(  regex_search( input, results, dollarSignRegex )  ) {
		output += std::string( results.prefix().first, results.prefix().second );
		char firstMatchCharacter = *results[0].first;
		switch( firstMatchCharacter ) {
			case '$': {
				output += "]";
				break;
			}
			default: {
				output += std::string( results[0].first, results[0].second );
				break;
			}
		}
		input = std::string( results.suffix().first, results.suffix().second );
	}
	output += input;

	return output;
}

TransConnector MatLabUdmTransition::getTransitionEnd( int transitionEndId ) {
	Transition udmTransition = getUdmTransition();
	State udmTransitionParent = State::Cast( udmTransition.GetParent() );

	const TransConnector &transConnector = MatLabUdm::findTransConnector( transitionEndId );

	bool sameParent = udmTransitionParent.uniqueId() == transConnector.GetParent().uniqueId();

	if ( sameParent ) return transConnector;

	ConnectorRef connectorRef;
	ConnectorRefMap connectorRefMap = getConnectorRefMap( udmTransitionParent );

	ConnectorRefMap::iterator crhItr = connectorRefMap.find( transConnector.uniqueId() );
	if ( crhItr == connectorRefMap.end() ) {
		connectorRef = ConnectorRef::Create( udmTransitionParent );
		connectorRef.ref() = transConnector;
		connectorRef.name() = transConnector.name();	
	} else {
		connectorRef = (*crhItr).second;
	}

	return TransConnector::Cast( connectorRef );
}

