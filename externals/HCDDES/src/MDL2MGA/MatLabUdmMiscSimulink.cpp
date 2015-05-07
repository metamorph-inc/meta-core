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
 *  FILE NAME:	MatLabUdmMiscSimulink.cpp 
 *	SYNOPSIS:	Implementation of classes "MatLabUdmModel", translates SL/SF 'Simulink.BlockDiagram'
 *				objects into equivalent GME ESMoL Systems.					
 *  AUTHOR:		Harmon Nine - hnine@isis.vanderbilt.edu
 *  CREATED:	07/20/06
 *  LAST UPDATE: 
 */
#include "MatLabUdmMiscSimulink.hpp"
#include "MatLabUdmPort.hpp"
#include "RegexCommon.hpp"

#include <limits>
#include <boost/lexical_cast.hpp>

void MatLabUdmModel::setupModel( void ) {
	Subsystem udmModel = getUdmModel();

	udmModel.BlockType() = "Model";
	udmModel.Description() = MatLabEngine::globalEngine().getMxStringValue( getMatLabObject() + ".Description" ).c_str();
	std::string name = MatLabEngine::globalEngine().getMxStringValue( getMatLabObject() + ".Name" );
	if (  name.substr( name.size() - 8 ) == "Xmdl2mga"  ) {
		name = name.substr( 0, name.size() - 8 );
	}
	udmModel.Name() = udmModel.name() = name;
	udmModel.Tag() = MatLabEngine::globalEngine().getMxStringValue( getMatLabObject() + ".Tag" ).c_str();
	udmModel.RefId() = getRefId();

	// NOTE:  BECAUSE PRIORTY IS AN INTEGER, AND SO HAS NO INHERENT VALUE TO INDICATE "NO ASSIGNED PRIORITY"
	// PRIORITY VALUES (P) ARE TRANSLATED FROM MATLAB PRIORITY (M) AS FOLLOWS:
	//
	// IF ( M.NO_ASSIGNED_PRIORITY() ) P = 0
	// ELSE IF ( M > 0 ) P = M
	// ELSE IF ( M <= 0 ) P = M - 1
	//
	// THAT IS, P = 0 MEANS NO ASSIGNED PRIORITY IN ESMoL.  ALL OTHER VALUES INDICATE AN EXPLICIT PRIORITY
	// Priority is undefined for the root
	std::string priorityString = MatLabEngine::globalEngine().getMxStringValue( getMatLabObject(), "Priority" );
	int priority;
	if ( priorityString.empty() ) {
		priority = 0;
	} else {
		priority = boost::lexical_cast< int >( priorityString );
		if ( priority <= 0 ) {
			--priority;
		}
	}
	udmModel.Priority() = priority;

	// We need to use the post-compilation sample time
	//std::string sampleTimeString = MatLabEngine::globalEngine().getMxStringValue( getMatLabObject(), "SampleTime" );
	//double sampleTime = 0;
	//if ( sampleTimeString == "inf" ) {
	//	sampleTime = numeric_limits< double >::max();
	//} else if (  boost::regex_match( sampleTimeString, RegexCommon::getFloatingPointRegex() )  ) {
	//	sampleTime = boost::lexical_cast< double >( sampleTimeString );
	//} else {
	//	sampleTime = -2;
	//}
	//udmModel.SampleTime() = sampleTime;
	udmModel.SampleTime() = -2.0;

	// THIS IS TO ALLOW THE MDL MODEL TO BE COMPILED
	// FIXME this fails on libraries
	MatLabEngine::globalEngine().executeCommand( getMatLabObject() + ".SaveFormat = 'Structure'", true );

	MatLabUdm::buildUdm( *this, MatLabUdmPort::Factory() );
}
