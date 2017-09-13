#include "stdafx.h"
#include <Computations.h>
#include <CADPostProcessingParameters.h>
#include <CADAnalysisMetaData.h>
#include <cc_CommonUtilities.h>
#include <Metrics.h>
#include <CFDAnalysis.h>
#include <SurvivabilityAnalysis.h>
#include <ToolKitPassThroughFunctions.h>
#include <cc_MiscellaneousFunctions.h>
#include <sstream>
#include <iomanip>

namespace isis
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////

//void BuildList_ComponentsRequiringBoundingBoxes( 
//			std::map<string, isis::CADComponentData>	&in_CADComponentData_map,
//			std::list<std::string>						&out_componentsRequiringBoundingBoxes )
//															throw (isis::application_exception)
//{
//	for( std::map<std::string, isis::CADComponentData>::iterator i(in_CADComponentData_map.begin());
//	i != in_CADComponentData_map.end();
//	++i )
//	{
//		if ( i->second.cADComputations.boundingBoxMetricDefined ) 
//			out_componentsRequiringBoundingBoxes.push_back(i->first);
//
//	}
//}

/*
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// In general this function does not honor the ConfigurationID as an input field.  This is because
// the current schema (CADPostProcessingParameters.xsd) only supports one ConfigurationID.
// The schema should modified such that there  is a hierarchy  of components.
void CreateXMLFile_ComputedValues_ComputedByThisProgram( 
						const std::string									&in_PathAndFileName,
						const isis::CADAssemblies							&in_CADAssemblies,
						std::map<std::string, isis::CADComponentData>		&in_CADComponentData_map )
																	throw (isis::application_exception)
{
	// Example XML file
	//
	// <?xml version="1.0" encoding="UTF-8" standalone="no" ?>
	// <Components ConfigurationID="" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="CADPostProcessingParameters.xsd">
	//
	//	<Component ComponentID="100000445" FEAElementID="" _id="id9960">
    //		<Metrics _id="id9961">
    //			<Metric MetricID="6a632aaf-da49-49c1-81e8-168ff0cd7bb1" Type="BoundingBoxLength" Units="mm" Value="7116.749755347874" _id="id9962"/>
    //		</Metrics>
	//	</Component>
	//
	//	<Component ComponentID="100000452" FEAElementID="" _id="id9963">
    //		<Metrics _id="id9964">
    //			<Metric MetricID="6a632aaf-da49-49c1-81e8-168f999" Type="BoundingBoxWidth" Units="mm" Value="649.2874999999999" _id="id9965"/>
    //		</Metrics>
	//	</Component>
	//	</Components>


	Udm::SmartDataNetwork dn_FEAPostProcessingParameters( CADPostProcessingParameters::diagram );
	dn_FEAPostProcessingParameters.CreateNew( in_PathAndFileName, "CADPostProcessingParameters", CADPostProcessingParameters::Components::meta);
	
	try
	{
		CADPostProcessingParameters::Components componentsRoot = 
			      CADPostProcessingParameters::Components::Cast(dn_FEAPostProcessingParameters.GetRootObject());
	
		componentsRoot.ConfigurationID() = "";

		for ( std::list<TopLevelAssemblyData>::const_iterator i(in_CADAssemblies.topLevelAssemblies.begin());
				i != in_CADAssemblies.topLevelAssemblies.end();
				++i )
		{
			for( std::list<CADComputation>::const_iterator j(i->cADComputations.computations.begin());
				 j != i->cADComputations.computations.end(); ++j )
			{
				/////////////////////////////////////////////////////////////////////
				// Add ComponentID and FEAElementID
				// e.g. <Component ComponentID="100000678" FEAElementID="PSOLID_1">
				/////////////////////////////////////////////////////////////////////
				CADPostProcessingParameters::Component  componentRoot = CADPostProcessingParameters::Component::Create( componentsRoot );
				componentRoot.ComponentID() = j->componentID;
				componentRoot.FEAElementID() = "";

				/////////////////////////////////////////////////////////////////////
				// Add Metrics
				//
				/////////////////////////////////////////////////////////////////////

				CADPostProcessingParameters::Metrics metricsRoot = CADPostProcessingParameters::Metrics::Create( componentRoot );

				Pro3dPnt  r_outline_points[2];

				double temp_x;
				double temp_y;
				double temp_z;

				switch ( j->computationType)
				{
				case COMPUTATION_BOUNDING_BOX:
					isis::isis_ProSolidOutlineGet( in_CADComponentData_map[j->componentID].modelHandle, r_outline_points);
					temp_x  =  abs(r_outline_points[1][0] - r_outline_points[0][0]); // Typically Width  x direction	
					temp_y =  abs(r_outline_points[1][1] - r_outline_points[0][1]);  // Typically Height y direction
					temp_z =  abs(r_outline_points[1][2] - r_outline_points[0][2]);  // Typically Length Z direction
					break;			
				case COMPUTATION_CG:
					ProMassProperty  mass_prop;
					isis::isis_ProSolidMassPropertyGet( in_CADComponentData_map[j->componentID].modelHandle, NULL, &mass_prop );
					double   MatrixBuffer[4][4];
					temp_x =  mass_prop.center_of_gravity[0];
					temp_y  = mass_prop.center_of_gravity[1];
					temp_z =  mass_prop.center_of_gravity[2];
					break;
				case COMPUTATION_POINT:
					CADPoint point;
					RetrieveDatumPointCoordinates( i->assemblyComponentID,
												   j->componentID,
												   in_CADComponentData_map,
												   j->datumName,
												   point); 
					temp_x =  point.x;
					temp_y  = point.y;
					temp_z =  point.z;
					break;
				default:
					std::stringstream errorString;
					errorString <<
					"Error: (Function - CreateXMLFile_ComputationsParameters), received ComputationType that is not currently supported." << std::endl <<
					"ComputationType: " << ComputationType_string( j->computationType);
					throw isis::application_exception(errorString.str());
				}

			
				CADPostProcessingParameters::Metric metricRoot = CADPostProcessingParameters::Metric::Create( metricsRoot );
				metricRoot.MetricID() = j->metricID;
				metricRoot.Type() = ComputationDimension_string(j->computationDimension);
				metricRoot.Units() = "mm";

				if ( j->computationDimension == COMPUTATION_VECTOR )
				{				
					std::vector<double> values;
					values.push_back(temp_x);
					values.push_back(temp_y);
					values.push_back(temp_z);
					metricRoot.ArrayValue() = values;
				}
				else
				{
					double temp_cord;
					switch ( j->computationDimension )
					{
						case COMPUTATION_X_COORDINATE:
							temp_cord = temp_x;
							break;
						case COMPUTATION_Y_COORDINATE:
							temp_cord = temp_y;
							break;
						case COMPUTATION_Z_COORDINATE:
							temp_cord = temp_z;
							break;
					}

					std::vector<double> values;
					values.push_back(temp_cord);
					metricRoot.ArrayValue() = values;
				}
	
			
			}  // END for j loop

		} // END for ( std::list<TopLevelAssemblyData>::const_...
		///////////////////////////
		// Write XML File
		//////////////////////////
		dn_FEAPostProcessingParameters.CloseWithUpdate();
	}
	catch ( ... )
	{
		// If any error occurs, do not save the metrics xml file.  It makes no sense to save a metrics file
		// with erroneous data.
		dn_FEAPostProcessingParameters.CloseNoUpdate();
		throw;
	}

}
*/

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//	If in_PathAndFileName exist, then this function appends to the existing file.
//
//	If there are no computations requested, this function returns without taking any action (does not
//	create the xml file).
//  This function will ignore computationTypes other than:
//      COMPUTATION_BOUNDING_BOX,  
//		COMPUTATION_CG, 
//      COMPUTATION_POINT,
//		COMPUTATION_MASS,
//		COMPUTATION_PLANE
//
void CreateXMLFile_ComputedValues_ComputedByThisProgram( 
						const std::string									&in_PathAndFileName,
						const std::string									&in_ConfigruationID,
						const std::string									&in_AssemblyComponentID,
						const list<CADComputation>							&in_Metrics,
						std::map<std::string, isis::CADComponentData>		&in_CADComponentData_map )
																	throw (isis::application_exception)
{
	// Example XML file
	//
	// <?xml version="1.0" encoding="UTF-8" standalone="no" ?>
	// <Components ConfigurationID="" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="CADPostProcessingParameters.xsd">
	//
	//	<Component ComponentID="100000445" FEAElementID="" _id="id9960">
    //		<Metrics _id="id9961">
    //			<Metric MetricID="6a632aaf-da49-49c1-81e8-168ff0cd7bb1" Type="BoundingBoxLength" Units="mm" Value="7116.749755347874" _id="id9962"/>
    //		</Metrics>
	//	</Component>
	//
	//	<Component ComponentID="100000452" FEAElementID="" _id="id9963">
    //		<Metrics _id="id9964">
    //			<Metric MetricID="6a632aaf-da49-49c1-81e8-168f999" Type="BoundingBoxWidth" Units="mm" Value="649.2874999999999" _id="id9965"/>
    //		</Metrics>
	//	</Component>
	//	</Components>

	//
	//

	

	std::map<std::string, std::list<CADComputation>> componentID_to_ListofComputations_map;
	std::map<std::string, std::string> componentID_to_AssemblyComponentID_map;

	// Need to group all of the requested computations by component_ID.
	for each ( CADComputation j in in_Metrics )
	{
		if ( j.computationType == COMPUTATION_BOUNDING_BOX ||
				j.computationType == COMPUTATION_CG ||
				j.computationType == COMPUTATION_POINT ||
				j.computationType == COMPUTATION_MASS ||
				j.computationType == COMPUTATION_PLANE )
		{
			componentID_to_AssemblyComponentID_map[j.componentID] = in_AssemblyComponentID;
			componentID_to_ListofComputations_map[j.componentID].push_back(j);
		}
		}


	// Check for no computations
	if ( componentID_to_ListofComputations_map.size() == 0 ) 
	{
		isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "";
		isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) << "No computations requested.  ComputedValues file NOT created.";	
		return;
	}
	else
	{
		isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "";
		isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "Creating ComputedValues File";	
	}

	Udm::SmartDataNetwork dn_FEAPostProcessingParameters( CADPostProcessingParameters::diagram );
	if (isis::FileExists( in_PathAndFileName.c_str()) )
		dn_FEAPostProcessingParameters.OpenExisting( in_PathAndFileName, "CADPostProcessingParameters");
	else
		dn_FEAPostProcessingParameters.CreateNew( in_PathAndFileName, "CADPostProcessingParameters", CADPostProcessingParameters::Components::meta);
	
	try
	{
		CADPostProcessingParameters::Components componentsRoot = 
			      CADPostProcessingParameters::Components::Cast(dn_FEAPostProcessingParameters.GetRootObject());
	
		componentsRoot.ConfigurationID() = in_ConfigruationID;

		for each( std::pair<std::string, std::list<CADComputation>> i in componentID_to_ListofComputations_map)
		{			

			////////////////////////
			// Units
			////////////////////////

			std::string		distanceUnit_LongName;
			std::string		distanceUnit_ShortName;
		
			std::string		massUnit_LongName;
			std::string		massUnit_ShortName;

			std::string		forceUnit_LongName;
			std::string		forceUnit_ShortName;

			std::string		temperatureUnit_LongName;
			std::string		temperatureUnit_ShortName;
		
			std::string		timeUnit_LongName;
			std::string		timeUnit_ShortName;
			
			RetrieveUnits_withDescriptiveErrorMsg( 
							in_CADComponentData_map[i.first].componentID,
							in_CADComponentData_map[i.first].name,
							in_CADComponentData_map[i.first].cADModel_hdl, 
							distanceUnit_ShortName,		distanceUnit_LongName, 
							massUnit_ShortName,			massUnit_LongName, 
							forceUnit_ShortName,			forceUnit_LongName, 
							timeUnit_ShortName,			timeUnit_LongName, 
							temperatureUnit_ShortName,	temperatureUnit_LongName );

			/////////////////////////////////////////////////////////////////////
			// Add ComponentID and FEAElementID
			// e.g. <Component ComponentID="100000678" FEAElementID="PSOLID_1">
			/////////////////////////////////////////////////////////////////////
			CADPostProcessingParameters::Component  componentRoot = CADPostProcessingParameters::Component::Create( componentsRoot );
			componentRoot.ComponentInstanceID() = i.first;
			componentRoot.FEAElementID() = "";

			CADPostProcessingParameters::Metrics metricsRoot = CADPostProcessingParameters::Metrics::Create( componentRoot );
			

			for each ( CADComputation j in i.second )
			{
				bool complexMetric = false;
				if ( j.computationType == COMPUTATION_PLANE ) complexMetric = true;

				/////////////////////////////////////////////////////////////////////
				// Add Metrics
				//
				/////////////////////////////////////////////////////////////////////

				Pro3dPnt  r_outline_points[2];

				double temp_x;
				double temp_y;
				double temp_z;
				double temp_scalar;
				std::vector<isis_CADCommon::Point_3D> temp_points;			

				ProMassProperty  mass_prop;
				double   MatrixBuffer[4][4];
				std::string temp_units = distanceUnit_LongName;

				isis_CADCommon::Point_3D	boundingBox_Point_1;
				isis_CADCommon::Point_3D	boundingBox_Point_2;
				double						boundingBoxDimensions_xyz[3];		

				switch ( j.computationType)
				{
					case COMPUTATION_BOUNDING_BOX:
						
						//isis::isis_ProSolidOutlineGet( in_CADComponentData_map[i.first].modelHandle, r_outline_points);

						//temp_x  =  abs(r_outline_points[1][0] - r_outline_points[0][0]); // Typically Width  x direction	
						//temp_y =  abs(r_outline_points[1][1] - r_outline_points[0][1]);  // Typically Height y direction
						//temp_z =  abs(r_outline_points[1][2] - r_outline_points[0][2]);  // Typically Length Z direction

	

						RetrieveBoundingBox_ComputeFirstIfNotAlreadyComputed(	in_CADComponentData_map[i.first].componentID,
																in_CADComponentData_map,
																boundingBox_Point_1,
																boundingBox_Point_2,
																boundingBoxDimensions_xyz );

						temp_x  = boundingBoxDimensions_xyz[0]; // Typically Width  x direction	
						temp_y =  boundingBoxDimensions_xyz[1];  // Typically Height y direction
						temp_z =  boundingBoxDimensions_xyz[2];  // Typically Length Z direction

						break;			
					case COMPUTATION_CG:
						isis_ProSolidMassPropertyGet_WithDescriptiveErrorMsg( i.first, in_CADComponentData_map, &mass_prop );
						//isis::isis_ProSolidMassPropertyGet( in_CADComponentData_map[i.first].modelHandle, NULL, &mass_prop );
						temp_x =  mass_prop.center_of_gravity[0];
						temp_y  = mass_prop.center_of_gravity[1];
						temp_z =  mass_prop.center_of_gravity[2];
						break;

					case COMPUTATION_MASS:
						isis_ProSolidMassPropertyGet_WithDescriptiveErrorMsg( i.first, in_CADComponentData_map, &mass_prop );						
						//isis::isis_ProSolidMassPropertyGet( in_CADComponentData_map[i.first].modelHandle, NULL, &mass_prop );

						//isis::isis_ProSolidMassPropertyGet( in_CADComponentData_map[j.componentID].modelHandle, NULL, &mass_prop );
						temp_scalar =  mass_prop.mass;
						temp_units = massUnit_LongName;
						break;

					case COMPUTATION_POINT:
						CADPoint point;
						RetrieveDatumPointCoordinates( componentID_to_AssemblyComponentID_map[i.first],
													   j.componentID,
													   in_CADComponentData_map,
													   j.datumName, 
													   point); 
						temp_x =  point.x;
						temp_y  = point.y;
						temp_z =  point.z;
						break;
					case COMPUTATION_PLANE:
						// For now, only supporting ground plane
						if ( j.computationSubType == COMPUTATION_SUBTYPE_GROUND )
						{
							ComputeVehicleGroundPlane(	componentID_to_AssemblyComponentID_map[i.first],
													in_CADComponentData_map,
													temp_points );
						}
						else
						{
							std::stringstream errorString;
							errorString <<
							"Function - CreateXMLFile_ComputationsParameters, received ComputationSubType that is not currently supported." << std::endl <<
							"ComputationSubType: " << ComputationSubType_string( j.computationSubType);
							throw isis::application_exception(errorString.str());
						}
						break;
					default:
						std::stringstream errorString;
						errorString <<
						"Function - CreateXMLFile_ComputationsParameters, received ComputationType that is not currently supported." << std::endl <<
						"ComputationType: " << ComputationType_string( j.computationType);
						throw isis::application_exception(errorString.str());
				} // End switch ( j.computationType)
			

				if ( complexMetric )
				{
					CADPostProcessingParameters::ComplexMetric complexMetricRoot;
					complexMetricRoot = CADPostProcessingParameters::ComplexMetric::Create( metricsRoot );
					complexMetricRoot.MetricID() = j.metricID;
					complexMetricRoot.Type() = ComputationType_string(j.computationType);
					complexMetricRoot.SubType() = ComputationSubType_string(j.computationSubType);
					complexMetricRoot.SubType() = ComputationSubType_string(j.computationSubType);
					complexMetricRoot.DataFormat() = ComputationDimension_string(j.computationDimension);

					CADPostProcessingParameters::Metric metricRoot;
					switch ( j.computationSubType )
					{
						case COMPUTATION_SUBTYPE_GROUND:
							for each ( isis_CADCommon::Point_3D k in temp_points )
							{
								metricRoot = CADPostProcessingParameters::Metric::Create( complexMetricRoot );
								metricRoot.Units() = temp_units;
								metricRoot.Type() = "POINT";
								metricRoot.DataFormat() = ComputationDimension_string(COMPUTATION_VECTOR);	
								std::vector<double> values;
								values.push_back(k.x);
								values.push_back(k.y);
								values.push_back(k.z);
								metricRoot.ArrayValue() = values;
							}
							break;
						default:
							std::stringstream errorString;
							errorString <<
							"Function - CreateXMLFile_ComputationsParameters, received computationSubType that is not currently supported." << std::endl <<
							"ComputationSubType: " << ComputationSubType_string( j.computationSubType);
							throw isis::application_exception(errorString.str());
					}
				}
				else // Simple metric
				{
					CADPostProcessingParameters::Metric metricRoot;
					metricRoot = CADPostProcessingParameters::Metric::Create( metricsRoot );
					metricRoot.MetricID() = j.metricID;
					metricRoot.Type() = ComputationDimension_string(j.computationDimension);
					metricRoot.Units() = temp_units;
					metricRoot.MetricName() = j.metricName;

					std::vector<double> values;
					double temp_value;
				
					switch ( j.computationDimension )
					{
						case COMPUTATION_VECTOR:			
							values.push_back(temp_x);
							values.push_back(temp_y);
							values.push_back(temp_z);
							metricRoot.ArrayValue() = values;
							break;
				
						case COMPUTATION_X_COORDINATE:
						case COMPUTATION_Y_COORDINATE:
						case COMPUTATION_Z_COORDINATE:
						case COMPUTATION_DIMENSION_NONE:
						case COMPUTATION_SCALAR:
							switch ( j.computationDimension )
							{
									case COMPUTATION_X_COORDINATE:
										temp_value = temp_x;
										break;
									case COMPUTATION_Y_COORDINATE:
										temp_value = temp_y;
										break;
									case COMPUTATION_Z_COORDINATE:
										temp_value = temp_z;
										break;
									case COMPUTATION_DIMENSION_NONE:
									case COMPUTATION_SCALAR:
										temp_value = temp_scalar;
										break;
									default:
										std::stringstream errorString;
										errorString <<
										"Function - CreateXMLFile_ComputationsParameters, received computationDimension that is not currently supported." << std::endl <<
										"ComputationType: " << ComputationDimension_string( j.computationDimension);
										throw isis::application_exception(errorString.str());
							}
							values.push_back(temp_value);
							metricRoot.ArrayValue() = values;
							break;
		
						default:
							std::stringstream errorString;
							errorString <<
							"Function - CreateXMLFile_ComputationsParameters, received computationDimension that is not currently supported." << std::endl <<
							"ComputationType: " << ComputationDimension_string( j.computationDimension);
							throw isis::application_exception(errorString.str());
					}  // END switch ( j.computationDimension )
				} // END  if ( complexMetric ) else
			
			}  // END for j loop

		} // END for ( std::list<TopLevelAssemblyData>::const_...
		///////////////////////////
		// Write XML File
		//////////////////////////
		if ( dn_FEAPostProcessingParameters.isOpen()) dn_FEAPostProcessingParameters.CloseWithUpdate();
		isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "   Created:  " << in_PathAndFileName;	

	}
	catch ( ... )
	{
		// If any error occurs, do not save the metrics xml file.  It makes no sense to save a metrics file
		// with erroneous data.
		if ( dn_FEAPostProcessingParameters.isOpen()) dn_FEAPostProcessingParameters.CloseNoUpdate();
		throw;
	}

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RetrieveAllComputations(	const	std::list<TopLevelAssemblyData>				&topLevelAssemblies,
								std::vector<CADComputation_WithParentData>			&out_CADComputations_WithParentData )
{

	for each ( TopLevelAssemblyData i in topLevelAssemblies )
	{
		for each ( CADComputation j in i.assemblyMetrics ) 
		{
			CADComputation_WithParentData  cADComputation_WithParentData(j.componentID);
			cADComputation_WithParentData.cADComputation = j;
			cADComputation_WithParentData.configurationID = i.configurationID;
			cADComputation_WithParentData.topAssemblyComponentInstanceID = i.assemblyComponentID;
			out_CADComputations_WithParentData.push_back(cADComputation_WithParentData);
		}

		// Ballistics
		for each ( AnalysisBallistic j in i.analysesCAD.analysesBallistic )
		{
			for each (  CADComputation k in j.metrics )
			{
				CADComputation_WithParentData  cADComputation_WithParentData(k.componentID);
				cADComputation_WithParentData.cADComputation = k;
				cADComputation_WithParentData.configurationID = i.configurationID;
				cADComputation_WithParentData.topAssemblyComponentInstanceID = i.assemblyComponentID;
				out_CADComputations_WithParentData.push_back(cADComputation_WithParentData);
			}
		}

		// Blast
		for each ( AnalysisBlast j in i.analysesCAD.analysesBlast)
		{
			for each (  CADComputation k in j.metrics )
			{
				CADComputation_WithParentData  cADComputation_WithParentData(k.componentID);;
				cADComputation_WithParentData.cADComputation = k;
				cADComputation_WithParentData.configurationID = i.configurationID;
				cADComputation_WithParentData.topAssemblyComponentInstanceID = i.assemblyComponentID;
				out_CADComputations_WithParentData.push_back(cADComputation_WithParentData);
			}
		}

		// CFD AnalysisCFD
		for each ( AnalysisCFD j in i.analysesCAD.analysesCFD)
		{
			for each (  CADComputation k in j.metrics )
			{
				CADComputation_WithParentData  cADComputation_WithParentData(k.componentID);;
				cADComputation_WithParentData.cADComputation = k;
				cADComputation_WithParentData.configurationID = i.configurationID;
				cADComputation_WithParentData.topAssemblyComponentInstanceID = i.assemblyComponentID;
				out_CADComputations_WithParentData.push_back(cADComputation_WithParentData);
			}
		}

		// FEA AnalysisFEA
		for each ( AnalysisFEA j in i.analysesCAD.analysesFEA)
		{
			for each (  CADComputation k in j.metrics )
			{
				CADComputation_WithParentData  cADComputation_WithParentData(k.componentID);;
				cADComputation_WithParentData.cADComputation = k;
				cADComputation_WithParentData.configurationID = i.configurationID;
				cADComputation_WithParentData.topAssemblyComponentInstanceID = i.assemblyComponentID;
				out_CADComputations_WithParentData.push_back(cADComputation_WithParentData);
			}
		}

	}

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CreateXMLFile_RequestedMetrics( 
						const	std::string									&in_PathAndFileName,
						const	std::list<TopLevelAssemblyData>				&in_TopLevelAssemblies,
						std::map<std::string, isis::CADComponentData>		&in_CADComponentData_map )
																	throw (isis::application_exception)
{

	Udm::SmartDataNetwork dn_Metrics( CADAnalysisMetaData::diagram );
	

	try
	{
		std::vector<CADComputation_WithParentData>		cADComputations_WithParentData;

		RetrieveAllComputations( in_TopLevelAssemblies, cADComputations_WithParentData );

		isis_LOG(lg, isis_FILE, isis_INFO) << "";
		isis_LOG(lg, isis_FILE, isis_INFO) << "**** Begin Complete Set of Computations ****";
		for each ( CADComputation_WithParentData i in cADComputations_WithParentData )
		{
			isis_LOG(lg, isis_FILE, isis_INFO) << "**** Computation ****";
			isis_LOG(lg, isis_FILE, isis_INFO) << "configurationID: " << i.configurationID;
			isis_LOG(lg, isis_FILE, isis_INFO) << "topAssemblyComponentInstanceID: " << i.topAssemblyComponentInstanceID;
			isis_LOG(lg, isis_FILE, isis_INFO) << i.cADComputation;
		}
		isis_LOG(lg, isis_FILE, isis_INFO) << "***** End Complete Set of Computations *****";


		dn_Metrics.CreateNew( in_PathAndFileName, "CADAnalysisMetaData", CADAnalysisMetaData::Metrics::meta);
		

		CADAnalysisMetaData::Metrics metricsRoot = CADAnalysisMetaData::Metrics::Cast(dn_Metrics.GetRootObject());
		

		for each ( CADComputation_WithParentData i in cADComputations_WithParentData )
		{	
			CADAnalysisMetaData::Metric metricRoot = CADAnalysisMetaData::Metric::Create(metricsRoot);
			metricRoot.ConfigurationID() = i.configurationID;
			metricRoot.TopAssemblyComponentInstanceID() = i.topAssemblyComponentInstanceID;
			metricRoot.MetricType() = ComputationType_string(i.cADComputation.computationType);
			metricRoot.ComponentInstanceID() = i.cADComputation.componentID;
			metricRoot.ComponentName() = in_CADComponentData_map[i.cADComputation.componentID].name;
			// Model Type
			if ( in_CADComponentData_map[i.cADComputation.componentID].modelType == PRO_MDL_PART )
				metricRoot.ComponentType() = "PART";
			else
				metricRoot.ComponentType() = "ASSEMBLY";

			metricRoot.MetricID() = i.cADComputation.metricID;

			metricRoot.RequestedValueType() = ComputationDimension_string(i.cADComputation.computationDimension);

		}
		
		///////////////////////////
		// Write XML File
		//////////////////////////
		if ( dn_Metrics.isOpen()) dn_Metrics.CloseWithUpdate();
		
	}
	catch ( ... )
	{
		// If any error occurs, do not save the metrics xml file.  It makes no sense to save a metrics file
		// with erroneous data.
		if ( dn_Metrics.isOpen()) dn_Metrics.CloseNoUpdate();
		throw;
	}
}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//    If in_PathAndFileName exist, then this function appends to the existing file.
	void CreateXMLFile_ComputedValues_Metrics( 
					const std::string									&in_MetricsType,
					const std::string									&in_PathAndFileName,
					const std::string									&in_ConfigruationID,
					const list<CADComputation>							&in_Metrics,
					std::map<std::string, isis::CADComponentData>		&in_CADComponentData_map )
																	throw (isis::application_exception)
	{
		//<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
		//<Components ConfigurationID="66b2cc10-88e7-4d50-9756-57293be035af|1" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="CADPostProcessingParameters.xsd">

		//  <Component ComponentInstanceID="66b2cc10-88e7-4d50-9756-57293be035af|1" FEAElementID="" _id="id12c">
		//    <Metrics _id="id12d">
		//      <Metric DataFormat="SCALAR" MetricID="id-0067-00000a5a" Type="MaxVerticalJump" Units="" _id="id12e"/>
		//      <Metric DataFormat="SCALAR" MetricID="id-0067-00000a5b" Type="MaxVerticalVelocity" Units="" _id="id12f"/>
		//      <Metric DataFormat="SCALAR" MetricID="id-0067-00000a59" Type="MaxHorizonalVelocity" Units="" _id="id130"/>
		//    </Metrics>
		//  </Component>
		//
		//  <Component ComponentInstanceID="66b2cc10-88e7-4d50-9756-57293be035af|1" FEAElementID="" _id="id132">
		//    <Metrics _id="id133">
		//      <ComplexMetric DataFormat="POINTS" MetricID="NONE" SubType="GROUND" Type="PLANE" _id="id134">
		//        <Metric ArrayValue="0;-25;0" DataFormat="VECTOR" MetricID="" Type="POINT" Units="millimeter" _id="id135"/>
		//        <Metric ArrayValue="0;-25;1" DataFormat="VECTOR" MetricID="" Type="POINT" Units="millimeter" _id="id136"/>
		//        <Metric ArrayValue="1;-25;0" DataFormat="VECTOR" MetricID="" Type="POINT" Units="millimeter" _id="id137"/>
		//      </ComplexMetric>
		//    </Metrics>
		//  </Component>
		//
		//</Components>
		

		Udm::SmartDataNetwork dn_CFDParameters ( CADPostProcessingParameters::diagram );
		if (isis::FileExists( in_PathAndFileName.c_str()) )
			dn_CFDParameters.OpenExisting( in_PathAndFileName, "CADPostProcessingParameters");
		else
			dn_CFDParameters.CreateNew( in_PathAndFileName, "CADPostProcessingParameters", CADPostProcessingParameters::Components::meta);\

		isis_LOG(lg, isis_FILE, isis_INFO) << "";
		isis_LOG(lg, isis_FILE, isis_INFO) << "Begin **** CreateXMLFile_ComputedValues_Metrics *****";
		for each ( CADComputation i in in_Metrics ) isis_LOG(lg, isis_FILE, isis_INFO) << i;
		isis_LOG(lg, isis_FILE, isis_INFO) << "End **** CreateXMLFile_ComputedValues_Metrics *****";
			

		bool computedValues_Found = false;
		try
		{
			CADPostProcessingParameters::Components componentsRoot = 
						CADPostProcessingParameters::Components::Cast(dn_CFDParameters.GetRootObject());

			componentsRoot.ConfigurationID() = in_ConfigruationID;

												 

			std::map<std::string, std::list<CADComputation>> componentID_to_ListofComputations_map;
			std::set<std::string> componentIDs_set;

			OrganizeMetricsBasedOnComponentIDs(		in_Metrics,
													componentID_to_ListofComputations_map,
													componentIDs_set );

			for each ( const std::string i in componentIDs_set)
			{
				computedValues_Found = true;
				CADPostProcessingParameters::Component  componentRoot = CADPostProcessingParameters::Component::Create( componentsRoot );
				componentRoot.ComponentInstanceID() = i;
				componentRoot.FEAElementID() = "";
				CADPostProcessingParameters::Metrics metricsRoot = CADPostProcessingParameters::Metrics::Create( componentRoot );

				for each ( const CADComputation j in componentID_to_ListofComputations_map[i] )
				{							
					CADPostProcessingParameters::Metric metricRoot = CADPostProcessingParameters::Metric::Create( metricsRoot );
					metricRoot.MetricID()	=	j.metricID;
					metricRoot.Type()		=   ComputationType_string(j.computationType);
					metricRoot.DataFormat() =   ComputationDimension_string(j.computationDimension);
				}
			}

		
			if ( !computedValues_Found )
			{
				if ( dn_CFDParameters.isOpen()) dn_CFDParameters.CloseNoUpdate();
				std::stringstream errorString;
				errorString <<
						"Function CreateXMLFile_ComputedValues_Metrics, was invoked but in_CADAssemblies does not contain any computations.  MetricsType " << in_MetricsType;
						throw isis::application_exception(errorString.str());
			}

			///////////////////////////
			// Write XML File
			//////////////////////////
			
			if ( dn_CFDParameters.isOpen()) dn_CFDParameters.CloseWithUpdate();
		}
		catch ( udm_exception &ex )
		{
			// If any error occurs, do not save the metrics xml file.  It makes no sense to save a computed values file
			// with erroneous data.
			if ( dn_CFDParameters.isOpen()) dn_CFDParameters.CloseNoUpdate();
			std::stringstream errorString;
			errorString <<  "Function CreateXMLFile_ComputedValues_Metrics threw a udm_exception.  " << ex.what();
			throw isis::application_exception(errorString.str());
		}
		catch ( isis::application_exception& ex )
		{
			if ( dn_CFDParameters.isOpen()) dn_CFDParameters.CloseNoUpdate();
			std::stringstream errorString;
			errorString <<  "Function CreateXMLFile_ComputedValues_Metrics threw an application_exception.  " << ex.what();
			throw isis::application_exception(errorString.str());
		}
		catch ( std::exception& ex )
		{
			if ( dn_CFDParameters.isOpen()) dn_CFDParameters.CloseNoUpdate();
			std::stringstream errorString;
			errorString <<  "Function CreateXMLFile_ComputedValues_Metrics threw an exception.  " << ex.what();
			throw isis::application_exception(errorString.str());
		}
		catch ( ... )
		{
			if ( dn_CFDParameters.isOpen()) dn_CFDParameters.CloseNoUpdate();
			std::stringstream errorString;
			errorString <<  "Function CreateXMLFile_ComputedValues_Metrics, threw an unknown excpetion";
			throw isis::application_exception(errorString.str());

		}
		
	}	
																			
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void CreateXMLFile_ComputedValues( 
						const std::string									&in_WorkingDirector,
						const ComputationTypes								&in_ComputationTypes,
						isis::CADAssemblies									&in_CADAssemblies,
						std::map<std::string, isis::CADComponentData>		&in_CADComponentData_map )
																	throw (isis::application_exception)
	{

		std::string computedValues_PathAndFileName = in_WorkingDirector +  "\\ComputedValues.xml";
		isis::IfFileExists_DeleteFile( computedValues_PathAndFileName);
		
		if ( in_ComputationTypes.cFD ) 
		{
			std::string cFD_dir = "CFD";
			std::string createCFDDir = "if not exist " +  cFD_dir +  " mkdir  " + cFD_dir;
			isis::ExecuteSystemCommand( createCFDDir);
			std::string computedValues_PathAndFileName = in_WorkingDirector + "\\" + cFD_dir + "\\ComputedValues.xml";
			isis::IfFileExists_DeleteFile( computedValues_PathAndFileName);

			for each ( TopLevelAssemblyData i in in_CADAssemblies.topLevelAssemblies )
			{
				for each ( AnalysisCFD j in i.analysesCAD.analysesCFD )
				{
					// Must check j.metrics.size(), this is because
					// in_ComputationTypes.ballistics == true because the
					// the fileds j.referencePlanes contains values but
					// j.metrics is empty 				
					if ( j.metrics.size() > 0 )
					{
						CreateXMLFile_ComputedValues_Metrics(	"CFD",
																computedValues_PathAndFileName,
																i.configurationID,
																j.metrics,
																in_CADComponentData_map );
					}
				}
			}
		}
	
		if (  in_ComputationTypes.ballistics ) 
		{
			for each ( TopLevelAssemblyData i in in_CADAssemblies.topLevelAssemblies )
			{
				for each ( AnalysisBallistic j in i.analysesCAD.analysesBallistic )
				{
					// Must check j.metrics.size(), this is because
					// in_ComputationTypes.ballistics == true because the
					// the fileds j.referencePlanes contains values but
					// j.metrics is empty 				
					if ( j.metrics.size() > 0 )
					{
						CreateXMLFile_ComputedValues_Metrics(	"Ballistic",
															computedValues_PathAndFileName,
															i.configurationID,
															j.metrics,
															in_CADComponentData_map );
					}
				}
			}
		}

		if (  in_ComputationTypes.blast  ) 
		{

			for each ( TopLevelAssemblyData i in in_CADAssemblies.topLevelAssemblies )
			{
				for each ( AnalysisBlast j in i.analysesCAD.analysesBlast )
				{
					// Must check j.metrics.size(), this is because
					// in_ComputationTypes.ballistics == true because the
					// the fileds j.referencePlanes contains values but
					// j.metrics is empty 				
					if ( j.metrics.size() > 0 )
					{
						CreateXMLFile_ComputedValues_Metrics(	"Blast",
																computedValues_PathAndFileName,
																i.configurationID,
																j.metrics,
																in_CADComponentData_map );
					}
				}
			}
		}

		// Always do the following (for now) to force the reference plane to be created for field-of-view.
		// Long-term field-of-view should request a ground plane.  CyPhy should be modified to allow a
		// reference plane to be requested for general test benches or the other option is to create
		// a field-of-view test bench that has reference planes. 
		//if ( in_ComputationTypes.assemblyBasedComputations )
		if ( true )
		{

			// Temporarily check for a plane being in the list of request.
			// If not in the list, add it.  This will force a ground plane to always be created.
			// Later this code will be removed and replaced with an actual request from CyPhy2CAD
			if ( in_CADAssemblies.topLevelAssemblies.size() == 1 )
			{
				// Only add a ground plane if there is one and only one assembly.
				bool planeRequested = false;

				std::list<TopLevelAssemblyData>::iterator itr = in_CADAssemblies.topLevelAssemblies.begin();
				for each ( CADComputation i in itr->assemblyMetrics )
				{
					if ( i.computationType == COMPUTATION_PLANE ) planeRequested = true;
				}
				
				if ( !planeRequested )
				{
					CADComputation cADComputation(itr->assemblyComponentID);
					cADComputation.computationType =	COMPUTATION_PLANE;
					cADComputation.computationSubType = COMPUTATION_SUBTYPE_GROUND;
					cADComputation.computationDimension = COMPUTATION_POINTS;
					cADComputation.metricID = "";
					itr->assemblyMetrics.push_back(cADComputation);
				}
			}
	
	
			for each ( TopLevelAssemblyData i in in_CADAssemblies.topLevelAssemblies )
			{
				// If no computations by this program, then the following function will do nothing.
				// Always call the following function. 
				CreateXMLFile_ComputedValues_ComputedByThisProgram( 
														computedValues_PathAndFileName,
														i.configurationID,
														i.assemblyComponentID,
														i.assemblyMetrics,
														in_CADComponentData_map );			
			}

		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void CreateInterferenceReport( 
						const	std::string									&in_PathAndFileName,
						const	std::string									&in_ComponentInstanceID,
						const   CADComputation								&in_CADComputation,
						std::map<std::string, isis::CADComponentData>		&in_CADComponentData_map )
																	throw (isis::application_exception)
	{
		if ( in_ComponentInstanceID != in_CADComputation.componentID )
		{
			std::stringstream errorString;
			errorString <<	"Function - " << __FUNCTION__  << std::endl <<
							"in_ComponentInstanceID != in_CADComputation.componentID, these values must be equal. in_ComponentInstanceID refers" << std::endl <<
							"to the top assembly and in_CADComputation.componentID must also refer to the top assembly." << std::endl <<
							"   Top Assembly Model Name:   " <<	 in_CADComponentData_map[in_ComponentInstanceID].name    << std::endl <<
							"   CADComputation Model Name: " <<  in_CADComponentData_map[in_CADComputation.componentID].name    << std::endl <<
							in_CADComputation;
		}


		ofstream interferenceReport_file;
		try 
		{

			interferenceReport_file.open (in_PathAndFileName );

			// This code is based on UgGeomInterferCheck.c (located in the PTC installed examples)
			ProInterferenceInfo   *interf_info_arr;
		 
			ProAssembly assembly = static_cast<ProAssembly>( in_CADComponentData_map[in_ComponentInstanceID].cADModel_hdl);

			isis_ProFitGlobalinterferenceCompute(	assembly,
													//PRO_FIT_SUB_ASSEMBLY,  Not sure
													PRO_FIT_SUB_ASSEMBLY_DETAILED,
													PRO_B_FALSE,   // The options to include facets.
													PRO_B_FALSE,   // The options to include quilts
													PRO_B_FALSE,   // fast_calculation
													&interf_info_arr);

			int   n_intf_parts;
			isis_ProArraySizeGet(interf_info_arr, &n_intf_parts);

			interferenceReport_file << isis_CADCommon::GetDayMonthTimeYear();

			interferenceReport_file << std::endl << "Assembly Name: " << 
				(std::string)in_CADComponentData_map[in_ComponentInstanceID].name << "." << 
				CADMdlType_string(in_CADComponentData_map[in_ComponentInstanceID].modelType);
			// WARNING UpdateReportJson_CAD.py uses "MetricID:" and "InterferenceCount:".  Do NOT change these strings
			// without updating UpdateReportJson_CAD.py
			interferenceReport_file << std::endl << "MetricID: " << in_CADComputation.metricID; 
			interferenceReport_file << std::endl << "InterferenceCount: " << n_intf_parts; 
			interferenceReport_file << in_CADComputation;

			if (n_intf_parts == 0)
			{
				interferenceReport_file << "No interferences were detected.";
				if ( interferenceReport_file.is_open() ) interferenceReport_file.close();
				return;
			}

			interferenceReport_file << "Part 1 Name                     Part 2 Name                     Interference Volume (mm^3)";
			interferenceReport_file << std::endl;
			interferenceReport_file << "------------------------------  ------------------------------- ------------------------";
			
			for ( int i = 0; i < n_intf_parts; ++i )
			{
				double          volume;
				isis_ProFitInterferencevolumeCompute(interf_info_arr[i].interf_data, &volume);

				//ProAsmcomppath  cmp_path_1, cmp_path_2;
				//isis_ProSelectionAsmcomppathGet(interf_info_arr[i].part_1, &cmp_path_1);
				//isis_ProSelectionAsmcomppathGet(interf_info_arr[i].part_2, &cmp_path_2);

				ProModelitem modelitem_1;
				ProModelitem modelitem_2;
				isis_ProSelectionModelitemGet(interf_info_arr[i].part_1, &modelitem_1);
				isis_ProSelectionModelitemGet(interf_info_arr[i].part_2, &modelitem_2);

				ProMdldata           info_1;
				ProMdldata           info_2;

				isis_ProMdlDataGet (modelitem_1.owner, &info_1);
				isis_ProMdlDataGet (modelitem_2.owner, &info_2);

				char name_1[80];
				char name_2[80];
			
				interferenceReport_file << std::endl << std::left << std::setw(32) << ProWstringToString (name_1, info_1.name) <<   std::left <<  std::setw(32) << ProWstringToString (name_2, info_2.name) <<  volume;

			}

			// Free interf_info_arr
			isis_ProInterferenceInfoProarrayFree(interf_info_arr);
		
			interferenceReport_file << std::endl << "Interference analysis completed successfully.";
			if ( interferenceReport_file.is_open() ) interferenceReport_file.close();
		}

		catch ( isis::application_exception& ex )
		{
			if ( interferenceReport_file.is_open() ) 
			{
				interferenceReport_file << std::endl << "Error occurred when computing interferences. This file is incomplete.";
				interferenceReport_file.close();
			}
			throw;
		}
		catch ( std::exception& ex )
		{
			{
				interferenceReport_file << std::endl << "Error occurred when computing interferences. This file is incomplete.";
				interferenceReport_file.close();
			}
			throw;
		}
		catch ( ... )
		{
			if ( interferenceReport_file.is_open() ) 
			{
				interferenceReport_file << std::endl << "Error occurred when computing interferences. This file is incomplete.";
				interferenceReport_file.close();
			}
			std::stringstream	exceptionErrorStringStream;
			exceptionErrorStringStream << " std::exception: Caught exception (...) in function CreateInterferenceReport.  Please report the error to the help desk.";
			throw isis::application_exception(exceptionErrorStringStream.str().c_str());
		}

	}




} // end namespace isis