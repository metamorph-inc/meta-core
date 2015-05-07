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
 *  FILE NAME:	MatLabUdmState.cpp 
 *	SYNOPSIS:	Implementation of class "MatLabUdmState", translates SL/SF 'Stateflow.State'
 *				objects into equivalent GME ESMoL State models.					
 *  AUTHOR:		Harmon Nine - hnine@isis.vanderbilt.edu
 *  CREATED:	07/20/06
 *  LAST UPDATE: 
 */
#include "MatLabUdmState.hpp"
#include "MatLabUdmTransition.hpp"
#include "MatLabUdmData.hpp"
#include "MatLabUdmMiscStateflow.hpp"

void MatLabUdmState::setupState( void ) {

	State udmState = getUdmState();
	udmState.name() = getUniqueName().c_str();
	udmState.RefId() = getRefId();
	udmState.Description() = MatLabEngine::globalEngine().getMxStringValue( getMatLabObject() + ".Description" );

	// GET LABEL STRING, PERFORM SUBSTITUTIONS FOR READABILITY
	std::string labelString = RegexCommon::eliminateContinuations(  MatLabEngine::globalEngine().getMxStringValue( getMatLabObject() + ".LabelString" )  );
	labelString = boost::regex_replace( labelString, MatLabEngine::matLabComment(), "" );


	std::string stateType = MatLabEngine::globalEngine().getMxStringValue(  std::string( "class( " + getMatLabObject() + " )" )  );

	if ( stateType == "Stateflow.Box" ) {
		udmState.Decomposition() = "GROUP_STATE";
		return;
	}

	// REGISTER THIS STATE SO TRANSITIONS CAN LOCATE IT AND USE IT AS AN ENDPOINT
	registerForTransitions();

	std::string stateOrder = boost::lexical_cast< std::string >(
	 MatLabEngine::globalEngine().getMxIntValue( getMatLabObject() + ".ExecutionOrder" )
	);
	udmState.Order() = stateOrder.empty() ? "0" : stateOrder;

	static boost::regex nameRegex(  std::string( "\\A\\s*(\\w*)\\s*/?\\s*" ), boost::regex_constants::perl  );
	static boost::regex actionRegex(  std::string( "((\\w+)\\s*:\\s*(.*?\\S?)\\s*)(?:\\w+\\s*:|\\z)" ), boost::regex_constants::perl  );
	static boost::regex defaultActionRegex(  std::string( "\\A\\s*(.*\\S?)\\s*\\Z" ), boost::regex_constants::perl  );
	boost::match_results< std::string::const_iterator > results;

	if (  regex_search( labelString, results, nameRegex )  ) {
		udmState.Name() = std::string( results[1].first, results[1].second ).c_str();
		labelString = std::string(  results[0].second, static_cast< std::string::const_iterator >( labelString.end() )  );
	}

	while (  regex_search( labelString, results, actionRegex )  ) {
		std::string actionName( results[2].first, results[2].second );
		std::transform( actionName.begin(), actionName.end(), actionName.begin(), ToLower() );
		std::string actionValue( results[3].first, results[3].second );
		if      ( actionName.substr(0, 2) == "en" ) udmState.EnterAction()  = actionValue.c_str();
		else if ( actionName.substr(0, 1) == "d" )  udmState.DuringAction() = actionValue.c_str();
		else if ( actionName.substr(0, 2) == "ex" ) udmState.ExitAction()   = actionValue.c_str();

		labelString.replace(
		 results[1].first - labelString.begin(),
		 results[1].second - results[1].first,
		 ""
		);
	}

	regex_search( labelString, results, defaultActionRegex );
	std::string defaultEnterAction( results[1].first, results[1].second );
	if (  defaultEnterAction != ""  &&  std::string( udmState.EnterAction() ) == ""  ) {
		udmState.EnterAction() = defaultEnterAction;
	}

	udmState.Decomposition() = MatLabEngine::globalEngine().getMxStringValue( getMatLabObject() + ".Type" ) + "_STATE";
}


void MatLabUdmState::constructStates( MatLabUdm::SmartPointer &matLabUdmParentSP ) {

	MatLabUdm::SPVector recurseVector;

	// GET STATES
	recurseVector = MatLabUdm::buildUdm( matLabUdmParentSP, MatLabUdmState::Factory() );
	for( MatLabUdm::SPVector::iterator spvItr = recurseVector.begin() ; spvItr != recurseVector.end() ; ++spvItr ) {
		constructStates( *spvItr );
	}
	MatLabUdmTransition::getTransitionParents().add( recurseVector );

	// GET FUNCTIONS
	recurseVector = MatLabUdm::buildUdm( matLabUdmParentSP, MatLabUdmFunction::Factory() );
	for( MatLabUdm::SPVector::iterator spvItr = recurseVector.begin() ; spvItr != recurseVector.end() ; ++spvItr ) {
		constructStates( *spvItr );
	}
	MatLabUdmTransition::getTransitionParents().add( recurseVector );

	// GET EMFUNCTIONS
	recurseVector = MatLabUdm::buildUdm( matLabUdmParentSP, MatLabUdmEMFunction::Factory() );
	for( MatLabUdm::SPVector::iterator spvItr = recurseVector.begin() ; spvItr != recurseVector.end() ; ++spvItr ) {
		constructStates( *spvItr );
	}

	// GET TRUTHTABLES
	recurseVector = MatLabUdm::buildUdm( matLabUdmParentSP, MatLabUdmTruthTable::Factory() );
	for( MatLabUdm::SPVector::iterator spvItr = recurseVector.begin() ; spvItr != recurseVector.end() ; ++spvItr ) {
		constructStates( *spvItr );
	}

	// GET DATA
	(void)MatLabUdm::buildUdm( matLabUdmParentSP, MatLabUdmData::Factory() );

	// GET EVENTS
	(void)MatLabUdm::buildUdm( matLabUdmParentSP, MatLabUdmEvent::Factory() );

	// GET JUNCTIONS
	(void)MatLabUdm::buildUdm( matLabUdmParentSP, MatLabUdmJunction::Factory() );

}
