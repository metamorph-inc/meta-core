#include "cc_CommonFunctions.h"
#include "cc_CommonDefinitions.h"
#include "CADAnalysisMetaData.h"
#include "UdmBase.h"
#include "AssemblyInterface.h"

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

}