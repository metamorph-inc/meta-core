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
 *  FILE NAME:	MatLabUdmData.cpp 
 *	SYNOPSIS:	Implementation of class "MatLabUdmData", translates SL/SF 'Stateflow.Data'
 *				objects into equivalent GME SLSF Data atoms.					
 *  AUTHOR:		Harmon Nine - hnine@isis.vanderbilt.edu
 *  CREATED:	07/20/06
 *  LAST UPDATE: 
 */
#include "MatLabUdmData.hpp"
#include "UdmEngine.hpp"

void MatLabUdmData::setupData( void ) {
	Data udmData = getUdmData();

	udmData.Description() = MatLabEngine::globalEngine().getMxStringValue( getMatLabObject() + ".Description" ).c_str();
	std::string name = getUniqueName();
	udmData.Name() = udmData.name() = name;
	udmData.RefId() = getRefId();

	std::string dataScope = MatLabEngine::globalEngine().getMxStringValue( getMatLabObject() + ".Scope" );
	std::transform( dataScope.begin(), dataScope.end(), dataScope.begin(), ToUpper() );
	if ( dataScope == "PARAMETER" ) dataScope = "WORKSPACE";
	else if ( dataScope == "DATA STORE MEMORY" ) dataScope = "STORE";
	udmData.Scope() = ( dataScope + "_DATA" ).c_str();

	int port = MatLabEngine::globalEngine().getMxIntValue( getMatLabObject() + ".Port" );
	udmData.Port() = port > 0 ? port : -( getMatLabObjectNo() + 1 );

	udmData.Units()        = MatLabEngine::globalEngine().getMxStringValue( getMatLabObject() + ".Props.Type.Units" );
	udmData.InitialValue() = dataScope == "WORKSPACE" ?
	 MatLabEngine::globalEngine().getData( name ) :
	 MatLabEngine::globalEngine().getMxStringValue( getMatLabObject() + ".Props.InitialValue" );
	udmData.Min()          = MatLabEngine::globalEngine().getMxStringValue( getMatLabObject() + ".Props.Range.Minimum" );
	udmData.Max()          = MatLabEngine::globalEngine().getMxStringValue( getMatLabObject() + ".Props.Range.Maximum" );

	getMatLabUdmDataVector().push_back( *this );
}

void MatLabUdmData::getType() {

	Data udmData = getUdmData();

	SLSF::TypeBaseRef typeBaseRef = SLSF::TypeBaseRef::Create( udmData );

	/* ENCODING FOR DATA THAT TAKES BUS OBJECT INTO ACCOUNT */
	/* WITH BUS OBJECT, "ArraySize" IS ALWAYS BLANK, AND DataType IS NAME OF BUS OBJECT */
	/* WITHOUT BUS OBJECT, "ArraySize" IS ALWAY NON-BLANK, AND DataType IS THE DATA TYPE */
	std::string dataType = MatLabEngine::globalEngine().getMxStringValue( getMatLabObject() + ".CompiledType" );
	std::string dataTypeClass = MatLabEngine::globalEngine().getMxStringValue(  std::string( "class( " ) + dataType + "(1) )"  );

	if ( dataTypeClass == "Simulink.Bus" ) {

		typeBaseRef.name() = dataType;
		typeBaseRef.ref() = makeBusObjectTypeStruct( dataType );

	} else {

		SLSF::Matrix ESMoLMatrix = SLSF::Matrix::Create( UdmEngine::getUdmTypesRoot() );
		ESMoLMatrix.Type() = MatLabUdm::parameterValueSub( dataType );

		std::string arraySize = MatLabEngine::globalEngine().getMxStringValue( getMatLabObject() + ".CompiledSize" );
		IntList dimensionList = MatLabUdm::getIntList( arraySize );
		IntList::iterator inlItr = dimensionList.begin();

		int rows = 1;
		int columns = 1;
		if ( dimensionList.size() >= 1 ) rows = *inlItr++;
		if ( dimensionList.size() >= 2 ) columns = *inlItr++;
		if ( dimensionList.size() >= 3 ) {
			std::cerr << "WARNING: NUMBER OF DIMENSIONS GREATER THAN 2 DETECTED IN CHART DATA (NOT SUPPORTED)." << std::endl;
		}

		ESMoLMatrix.rows() = rows;
		ESMoLMatrix.columns() = columns;

		UdmEngine::integrateTypeBase( ESMoLMatrix );
		typeBaseRef.ref() = ESMoLMatrix;
		typeBaseRef.name() = ESMoLMatrix.name();

		udmData.ArrayFirstIndex() = MatLabEngine::globalEngine().getMxStringValue( getMatLabObject() + ".Props.Array.FirstIndex" );

	}

}