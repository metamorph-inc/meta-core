#include "cc_CommonFunctions.h"
#include "cc_CommonDefinitions.h"
#include "cc_CommonStructures.h"
#include <CADPostProcessingParameters.h>   //  This is a generated file, resides in cc_
#include <CADAnalysisMetaData.h>           //  This is a generated file, resides in cc_#include "UdmBase.h"
#include "AssemblyInterface.h"
#include "cc_MiscellaneousFunctions.h"
#include "cc_CADFactoryAbstract.h"
#include "cc_CommonUtilities.h"
#include "cc_AssemblyUtilities.h"
#include <ostream>
#include <fstream>
#include <iomanip>
namespace isis
{

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
			//if ( in_CADComponentData_map[i.cADComputation.componentID].modelType == PRO_MDL_PART )
			if ( in_CADComponentData_map[i.cADComputation.componentID].modelType == CAD_MDL_PART )
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

	void VisitComponents(	const std::string								&in_ComponentID, 
							std::map<std::string, isis::CADComponentData>	&in_out_CADComponentData_map,
							ComponentVistor									&in_componentVistor) 
																		throw (isis::application_exception) 
	{
		// This must always be first, so that the first entry (i.e. top-assembly can be include/excluded)
		// See ComponentVistorBuildListOfComponentIDs::operator()
		in_componentVistor(	in_ComponentID, in_out_CADComponentData_map);

		//if ( in_out_CADComponentData_map[in_ComponentID].modelType == PRO_MDL_ASSEMBLY )
		if ( in_out_CADComponentData_map[in_ComponentID].modelType == CAD_MDL_ASSEMBLY )
		{
			for ( std::list<string>::const_iterator i(in_out_CADComponentData_map[in_ComponentID].children.begin());
				i != in_out_CADComponentData_map[in_ComponentID].children.end();
				++i )
			{
				VisitComponents( *i,in_out_CADComponentData_map, in_componentVistor);
			}
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void ComponentVistorMaterialTokens::operator() ( const std::string  &in_ComponentID, 
									  std::map<std::string, isis::CADComponentData> &in_out_CADComponentData_map )
	{
		if ( in_out_CADComponentData_map[in_ComponentID].modelType == CAD_MDL_PART )
		{
			if ( in_out_CADComponentData_map[in_ComponentID].analysisTempMaterialDefined )
			{
				AnalysisTempMaterial analysisTempMaterialDefinition = in_out_CADComponentData_map[in_ComponentID].analysisTempMaterialDefinition;
				componentID_PoissonsRatio_map[in_ComponentID] = analysisTempMaterialDefinition.poissonsRatio;
				materialKey_ComponentID_map[analysisTempMaterialDefinition.tokenPoissonsRatio] = in_ComponentID;
			}
			else
			{
				std::string errorString;
				errorString = "exception : Function - " + std::string(__FUNCTION__) +  " Functor, called from ReplaceMaterialTokens_ReturnMaterialToComponentID), material properties not defined for component Instance ID: " + in_ComponentID;
				throw isis::application_exception("C03003", errorString.c_str());	
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ComponentVistorCountAssemblyComponents::ComponentVistorCountAssemblyComponents():numberOfComponents(0){};

	void ComponentVistorCountAssemblyComponents::operator() ( const std::string  &in_ComponentID, 
									  std::map<std::string, isis::CADComponentData> &in_out_CADComponentData_map )
	{
		if ( in_out_CADComponentData_map[in_ComponentID].children.size() == 0 ) ++numberOfComponents; // This is a part or a leaf assembly

	}

	SelectAllComponents SelectAllComponentsDefault; 


	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ComponentVistorBuildListOfComponentIDs::ComponentVistorBuildListOfComponentIDs(bool excludeTopAssembly) : flagExcludeEntry(excludeTopAssembly), predicate(SelectAllComponentsDefault) {};
	ComponentVistorBuildListOfComponentIDs::ComponentVistorBuildListOfComponentIDs(ComponentPredicate &in_Predicate, bool in_ExcludeTopAssembly) : predicate(in_Predicate), flagExcludeEntry(in_ExcludeTopAssembly) {};

	void ComponentVistorBuildListOfComponentIDs::operator() ( const std::string  &in_ComponentID, 
									  std::map<std::string, isis::CADComponentData> &in_out_CADComponentData_map )
	{
		if ( flagExcludeEntry )
			flagExcludeEntry = false;  // This will skip the first entry which would be the top-assembly component ID.
		else
			if ( predicate(in_out_CADComponentData_map[in_ComponentID] ) ) listOfComponentIDs.push_back(in_ComponentID);		
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	 bool SelectAllComponents::operator() ( const isis::CADComponentData &in_CADComponentData)
	{
		return true;
	};

	 bool SelectAllParts::operator() ( const isis::CADComponentData &in_CADComponentData)
	{
		if ( in_CADComponentData.modelType == CAD_MDL_PART )
			return true;
		else 
			return false;
	};


	 bool SelectComponentDerivedFromLeafAssembly::operator() ( const isis::CADComponentData &in_CADComponentData)
	{
		if ( in_CADComponentData.dataInitialSource == INITIAL_SOURCE_DERIVED_FROM_LEAF_ASSEMBLY_DESCENDANTS )
			return true;
		else 
			return false;
	};

	bool SelectComponentInInputXML::operator() ( const isis::CADComponentData &in_CADComponentData)
	{
		if ( in_CADComponentData.dataInitialSource == INITIAL_SOURCE_INPUT_XML_FILE )
			return true;
		else 
			return false;
	};

	bool SelectLeafAssemblies::operator() ( const isis::CADComponentData &in_CADComponentData)
	{
		//if ( in_CADComponentData.dataInitialSource == INITIAL_SOURCE_INPUT_XML_FILE && in_CADComponentData.modelType == PRO_MDL_ASSEMBLY )
		if ( in_CADComponentData.dataInitialSource == INITIAL_SOURCE_INPUT_XML_FILE && in_CADComponentData.modelType == CAD_MDL_ASSEMBLY )
			return true;
		else 
			return false;
	};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ComputeUnitNames_Distance( e_CADUnitsDistance in_Unit, std::string &out_ShortName, std::string &out_LongName  )
																	throw (isis::application_exception)
{

	bool valid_unit = false;

	if ( in_Unit == CAD_UNITS_INCH ) 
	{ 
		out_ShortName = "inch";	
		out_LongName	  = "inch";			
		valid_unit = true;
	} 
	else if ( in_Unit == CAD_UNITS_FOOT ) 
	{ 
		out_ShortName = "foot";	
		out_LongName  = "foot";			
		valid_unit = true;
	}
	else if ( in_Unit == CAD_UNITS_MM ) 
	{ 
		out_ShortName = "mm";		
		out_LongName  = "millimeter";		
		valid_unit = true;
	}
	else if ( in_Unit == CAD_UNITS_CM ) 
	{ 
		out_ShortName = "cm";		
		out_LongName  = "centimeter";		
		valid_unit = true;
	}
	else if ( in_Unit == CAD_UNITS_M )	
	{ 
		out_ShortName = "m";		
		out_LongName  = "meter";			
		valid_unit = true;
	}
	else if ( in_Unit == CAD_UNITS_KM )
	{ 
		out_ShortName = "km";		
		out_LongName  = "kilometer";		
		valid_unit = true;
	}

	
	if ( !valid_unit )
	{
		std::stringstream errorString;
		errorString << "Function - " << __FUNCTION__ << ", " << std::endl <<
					"received in_DistanceUnit: " << std::endl << CADUnitsDistance_string(in_Unit) << 
					", which is an unkown unit type.  Valid unit types CAD_UNITS_INCH, CAD_UNITS_FOOT, CAD_UNITS_MM, CAD_UNITS_CM, CAD_UNITS_M and CAD_UNITS_KM";

		throw isis::application_exception(errorString);		
	}

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ComputeUnitNames_Mass( e_CADUnitsMass in_Unit, std::string &out_ShortName, std::string &out_LongName  )
																	throw (isis::application_exception)
{

	bool valid_unit = false;


	if ( in_Unit == CAD_UNITS_LBM)
	{ 
		out_ShortName = "lbm";		
		out_LongName = "poundmass"; 
		valid_unit = true;
	}
	else if ( in_Unit == CAD_UNITS_GRAM )	
	{ 
		out_ShortName = "g";			
		out_LongName = "gram"; 
		valid_unit = true;
	}
	else if ( in_Unit == CAD_UNITS_KGRAM )	
	{ 
		out_ShortName = "kg";			
		out_LongName = "kilogram"; 
		valid_unit = true;
	}
	else if ( in_Unit == CAD_UNITS_TONNE )	
	{ 
		out_ShortName = "tonne";  
		out_LongName = "tonne"; 
		valid_unit = true;
	}


	if ( !valid_unit )
	{
		std::stringstream errorString;
		errorString << "Function - " << __FUNCTION__ << ", " << std::endl <<
					"received in_MassUnit: " << std::endl << CADUnitsMass_string(in_Unit) << 
					", which is an unkown unit type.  Valid unit types CAD_UNITS_LBM, CAD_UNITS_GRAM, CAD_UNITS_KGRAM, and CAD_UNITS_TONNE.";

		throw isis::application_exception(errorString);		
	}

}																	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ComputeUnitNames_Force( e_CADUnitsForce in_Unit, std::string &out_ShortName, std::string &out_LongName  )
																	throw (isis::application_exception)
{

	bool valid_unit = false;

	if ( in_Unit == CAD_UNITS_LBF ) 
	{ 
		out_ShortName = "lbf";	
		out_LongName = "poundforce"; 
		valid_unit = true;
	}
	else if ( in_Unit == CAD_UNITS_N )	 
	{ 
		out_ShortName = "N";		
		out_LongName = "newton"; 
		valid_unit = true;
	}

	if ( !valid_unit )
	{
		std::stringstream errorString;
		errorString << "Function - " << __FUNCTION__ << ", " << std::endl <<
					"received in_ForceUnit: " << std::endl << CADUnitsForce_string(in_Unit) << 
					", which is an unkown unit type.  Valid types CAD_UNITS_LBF and CAD_UNITS_N.";
		throw isis::application_exception(errorString);		
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ComputeUnitNames_Time( e_CADUnitsTime in_Unit, std::string &out_ShortName, std::string &out_LongName  ) 
																			throw (isis::application_exception)
{

	bool valid_unit = false;

	if ( in_Unit == CAD_UNITS_SECOND ) 
	{ 
		out_ShortName = "sec";	
		out_LongName = "second"; 
		valid_unit = true;
	}


	if ( in_Unit == CAD_UNITS_HOUR ) 
	{ 
		out_ShortName = "hr";	
		out_LongName = "hour"; 
		valid_unit = true;
	}

	if ( !valid_unit )
	{
		std::stringstream errorString;
		errorString << "Function - " << __FUNCTION__ << ", " << std::endl <<
					"received in_TimeUnit: " << std::endl << CADUnitsTime_string(in_Unit) << 
					", which is an unkown unit type.  Valid unit types are CAD_UNITS_SECOND and CAD_UNITS_HOUR.";

		throw isis::application_exception(errorString);		
	}

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ComputeUnitNames_Temperature ( e_CADUnitsTemperature in_Unit, std::string &out_ShortName, std::string &out_LongName  )
																			throw (isis::application_exception)
{
	bool valid_unit = false;

	if ( in_Unit == CAD_UNITS_CELSIUS )	
	{ 
		out_ShortName = "C";
		out_LongName = "centigrade"; 
		valid_unit = true;
	}
	else if ( in_Unit == CAD_UNITS_FAHRENHEIT )	
	{ 
		out_ShortName = "F";	
		out_LongName = "fahrenheit"; 
		valid_unit = true;
	}
	else if ( in_Unit == CAD_UNITS_KELVIN )	
	{ 
		out_ShortName = "K";
		out_LongName = "kelvin";
		valid_unit = true;
	}

	if ( !valid_unit )
	{
		std::stringstream errorString;
		errorString << "Function - " << __FUNCTION__ << ", " << std::endl <<
					"received in_Unit: " << std::endl << CADUnitsTemperature_string(in_Unit) << 
					", which is an unkown unit type.  Valid unit types are CAD_UNITS_CELSIUS, CAD_UNITS_FAHRENHEIT, and CAD_UNITS_KELVIN";

		throw isis::application_exception(errorString);		
	}

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ComputeUnitNames_Angle ( e_CADUnitsAngle in_Unit, std::string &out_ShortName, std::string &out_LongName  )
																			throw (isis::application_exception)
{
	bool valid_unit = false;

	if ( in_Unit == CAD_UNITS_RADIAN )	
	{ 
		out_ShortName = "rad";
		out_LongName = "radian"; 
		valid_unit = true;
	}
	else if ( in_Unit == CAD_UNITS_DEGREE )	
	{ 
		out_ShortName = "deg";	
		out_LongName = "degree"; 
		valid_unit = true;
	}

	if ( !valid_unit )
	{
		std::stringstream errorString;
		errorString << "Function - " << __FUNCTION__ << ", " << std::endl <<
					"received in_Unit: " << std::endl << CADUnitsAngle_string(in_Unit) << 
					", which is an unkown unit type.  Valid unit types are CAD_UNITS_RADIAN and CAD_UNITS_DEGREE";

		throw isis::application_exception(errorString);		
	}

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
e_CADUnitsVolume convertDistanceUnitToVolumeUnit( e_CADUnitsDistance in_CADUnitsDistance )
													throw (isis::application_exception)
{
	switch ( in_CADUnitsDistance )
	{
		case CAD_UNITS_MM:
			return CAD_UNITS_MM_CUBED;
			break;

		case CAD_UNITS_CM:
			return CAD_UNITS_CM_CUBED;
			break;

		case CAD_UNITS_M:
			return CAD_UNITS_M_CUBED;
			break;

		case CAD_UNITS_KM:
			return CAD_UNITS_KM_CUBED;
			break;

		case CAD_UNITS_INCH:
			return CAD_UNITS_INCH_CUBED;
			break;

		case CAD_UNITS_FOOT:
			return CAD_UNITS_FOOT_CUBED;
			break;

		case CAD_UNITS_DISTANCE_NA:
			return CAD_UNITS_VOLUME_NA;
			break;

		default:
			std::stringstream errorString;
			errorString << "Function - " << __FUNCTION__ << ", " << std::endl <<
						"received in_Unit: " << std::endl << CADUnitsDistance_string(in_CADUnitsDistance) << 
						", which is an unkown unit type.  Valid unit types are CAD_UNITS_MM, CAD_UNITS_CM, CAD_UNITS_M, CAD_UNITS_KM, CAD_UNITS_INCH, CAD_UNITS_FOOT, and CAD_UNITS_DISTANCE_NA";

			throw isis::application_exception(errorString);		

	}

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WriteInterferenceReport(	const std::string								&in_AssemblyComponentInstanceID,  // This must be an assembly
								std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map,	
								const CADComputation								&in_CADComputation,
								const std::string								&in_PathAndFileName,
								bool												in_InterferenceAnalysisCompletedWithoutError,
								const std::vector<PartInterferences>				&in_PartInterferences)
																			throw (isis::application_exception)
{

		isis::cad::CadFactoryAbstract_global *cadFactoryAbstract_global_ptr = isis::cad::CadFactoryAbstract_global::instance();
		isis::cad::CadFactoryAbstract::ptr	                cAD_Factory_ptr = cadFactoryAbstract_global_ptr->getCadFactoryAbstract_ptr();
		isis::cad::IModelHandling&        modelHandling = cAD_Factory_ptr->getModelHandling();

		std::ofstream interferenceReport_file(in_PathAndFileName, std::ofstream::out);

		if ( !interferenceReport_file.is_open() )
		{
			std::stringstream errorString;
			errorString <<	"Function - " << __FUNCTION__  << std::endl << "could not open file for writing, File: " << in_PathAndFileName;
			throw isis::application_exception(errorString);		
		}

		interferenceReport_file << isis_CADCommon::GetDayMonthTimeYear();

		interferenceReport_file << std::endl << "Assembly Name: " << 
			//(std::string)in_CADComponentData_map[in_AssemblyComponentInstanceID].name << "." << 
			//CADMdlType_string(in_CADComponentData_map[in_AssemblyComponentInstanceID].modelType);
			modelHandling.combineCADModelNameAndSuffix(in_CADComponentData_map[in_AssemblyComponentInstanceID].name, in_CADComponentData_map[in_AssemblyComponentInstanceID].modelType );


		// WARNING UpdateReportJson_CAD.py uses "MetricID:" and "InterferenceCount:".  Do NOT change these strings
		// without updating UpdateReportJson_CAD.py
		interferenceReport_file << std::endl << "MetricID: " << in_CADComputation.metricID; 
		interferenceReport_file << std::endl << "InterferenceCount: " << in_PartInterferences.size(); 
		interferenceReport_file << in_CADComputation;

		if (in_PartInterferences.size() == 0)
		{
			interferenceReport_file << "No interferences were detected.";
			if ( interferenceReport_file.is_open() ) interferenceReport_file.close();
			return;
		}

		interferenceReport_file << "Part 1 Name                     Part 2 Name                     Interference Volume   Units";
		interferenceReport_file << std::endl;
		interferenceReport_file << "------------------------------  ------------------------------  --------------------  --------------";
			
		for each (PartInterferences i in in_PartInterferences)
		{
			interferenceReport_file << std::endl << std::left << std::setw(32) << i.modelName_1 <<   
				                                    std::left <<  std::setw(32) << i.modelName_2 <<  
												    std::left <<  std::setw(22) << i.volume << 
												    CADUnitsVolume_string(i.volumeUnits);
			//interferenceReport_file << std::endl << std::left << std::setw(32) << ProWstringToString (name_1, info_1.name) <<   std::left <<  std::setw(32) << ProWstringToString (name_2, info_2.name) <<  volume;
		}
		
		if ( in_InterferenceAnalysisCompletedWithoutError )
			interferenceReport_file << std::endl << "Interference analysis completed successfully.";
		else
			interferenceReport_file << std::endl << "Error occurred when computing interferences. This file is incomplete.";

		if ( interferenceReport_file.is_open() ) interferenceReport_file.close();

}

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
						//cad::CadFactoryAbstract								&in_Factory,
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

	isis::cad::CadFactoryAbstract_global *cadFactoryAbstract_global_ptr = isis::cad::CadFactoryAbstract_global::instance();
	isis::cad::CadFactoryAbstract::ptr	cAD_Factory_ptr = cadFactoryAbstract_global_ptr->getCadFactoryAbstract_ptr();

	isis::cad::IModelOperations&         modelOperations = cAD_Factory_ptr->getModelOperations();

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

			CADModelUnits	cADModelUnits_temp; 
			
			//RetrieveUnits_withDescriptiveErrorMsg( 
			//				in_Factory,
			//				i.first,
			//				in_CADComponentData_map,
			//				cADModelUnits_temp );

			modelOperations.retrieveCADModelUnits(
							//in_Factory,
							i.first,
							in_CADComponentData_map,
							cADModelUnits_temp );

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

				//Pro3dPnt  r_outline_points[2];

				double temp_x;
				double temp_y;
				double temp_z;
				double temp_scalar;
				std::vector<isis_CADCommon::Point_3D> temp_points;			

				//ProMassProperty  mass_prop;
				MassProperties		massProperties_temp;
				//double   MatrixBuffer[4][4];
				std::string temp_units = cADModelUnits_temp.distanceUnit_LongName;

				isis_CADCommon::Point_3D	boundingBox_Point_1;
				isis_CADCommon::Point_3D	boundingBox_Point_2;
				double						boundingBoxDimensions_xyz[3];		

				isis::cad::IModelOperations&         modelOperations = cAD_Factory_ptr->getModelOperations();

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

						//isis_ProSolidMassPropertyGet_WithDescriptiveErrorMsg( i.first, in_CADComponentData_map, &mass_prop );
						// old isis::isis_ProSolidMassPropertyGet( in_CADComponentData_map[i.first].modelHandle, NULL, &mass_prop );
						//temp_x =  mass_prop.center_of_gravity[0];
						//temp_y  = mass_prop.center_of_gravity[1];
						//temp_z =  mass_prop.center_of_gravity[2];

						massProperties_temp.setValuesToNotDefinedAndZeros();
						modelOperations.retrieveMassProperties( i.first, in_CADComponentData_map, massProperties_temp);

						temp_x =  massProperties_temp.centerOfGravity[0];
						temp_y  = massProperties_temp.centerOfGravity[1];
						temp_z =  massProperties_temp.centerOfGravity[2];

						break;

					case COMPUTATION_MASS:
						//isis_ProSolidMassPropertyGet_WithDescriptiveErrorMsg( i.first, in_CADComponentData_map, &mass_prop );						
						//  old isis::isis_ProSolidMassPropertyGet( in_CADComponentData_map[i.first].modelHandle, NULL, &mass_prop );

						//isis::isis_ProSolidMassPropertyGet( in_CADComponentData_map[j.componentID].modelHandle, NULL, &mass_prop );
						//temp_scalar =  mass_prop.mass;
						//temp_units = massUnit_LongName;

						massProperties_temp.setValuesToNotDefinedAndZeros();
						modelOperations.retrieveMassProperties( i.first, in_CADComponentData_map, massProperties_temp);
						temp_scalar =  massProperties_temp.mass;
						temp_units = cADModelUnits_temp.massUnit_LongName;

						break;

					case COMPUTATION_POINT:
						CADPoint point;
						//RetrieveDatumPointCoordinates(	//in_Factory, 
						//								componentID_to_AssemblyComponentID_map[i.first],
						//								j.componentID,
						//								in_CADComponentData_map,
						//								j.datumName, 
						//								point); 

						modelOperations.retrievePointCoordinates(	componentID_to_AssemblyComponentID_map[i.first],
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

							//modelOperations.computeVehicleGroundPlane(	componentID_to_AssemblyComponentID_map[i.first],
							//											in_CADComponentData_map,
							//											temp_points );
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
						//cad::CadFactoryAbstract								&in_Factory,
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
														//in_Factory,
														computedValues_PathAndFileName,
														i.configurationID,
														i.assemblyComponentID,
														i.assemblyMetrics,
														in_CADComponentData_map );			
			}

		}
	}


	void OrganizeMetricsBasedOnComponentIDs( 
							const list<CADComputation>							&in_Metrics,
							std::map<std::string, std::list<CADComputation>>	&out_componentID_to_ListofComputations_map,
							std::set<std::string>								&out_ComponentIDs_set )
	{											 
			for each ( const CADComputation i in in_Metrics)
			{
				out_componentID_to_ListofComputations_map[i.componentID].push_back(i);
				out_ComponentIDs_set.insert(i.componentID);
			}
	}







} // END namespace isis