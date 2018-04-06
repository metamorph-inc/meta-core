#ifndef CC_COMMON_FUNCTIONS_H
#define CC_COMMON_FUNCTIONS_H
#include <string>
#include <list>
#include <map>
#include "cc_CommonStructures.h"

namespace isis
{

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void CreateXMLFile_RequestedMetrics( 
						const	std::string									&in_PathAndFileName,
						const	std::list<TopLevelAssemblyData>				&in_TopLevelAssemblies,
						std::map<std::string, isis::CADComponentData>		&in_CADComponentData_map )
																	throw (isis::application_exception);


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////


	class ComponentVistor
	{
		public:
			virtual void operator() ( const std::string  &in_ComponentID, 
									  std::map<std::string, isis::CADComponentData> &in_out_CADComponentData_map )=0;
	};


	class ComponentPredicate
	{
		public:
			virtual bool operator() ( const isis::CADComponentData &in_CADComponentData) = 0;
	};


	void VisitComponents(	const std::string								&in_ComponentID, 
							std::map<std::string, isis::CADComponentData>	&in_out_CADComponentData_map,
							ComponentVistor									&in_componentVistor) 
																		throw (isis::application_exception);


	class ComponentVistorMaterialTokens: public ComponentVistor
	{
		public:
			//std::map<std::string, AnalysisTempMaterial> materialKey_MaterialDefintion_map;
			std::map<std::string, double> componentID_PoissonsRatio_map;
			std::map<std::string, std::string> materialKey_ComponentID_map;

			virtual void operator() ( const std::string  &in_ComponentID, 
									  std::map<std::string, isis::CADComponentData> &in_out_CADComponentData_map );
	};

	class ComponentVistorCountAssemblyComponents: public ComponentVistor
	{
		public:
			int numberOfComponents;

			virtual void operator() ( const std::string  &in_ComponentID, 
									  std::map<std::string, isis::CADComponentData> &in_out_CADComponentData_map );

			ComponentVistorCountAssemblyComponents();

	};

	class ComponentVistorBuildListOfComponentIDs: public ComponentVistor
	{
		public:
			std::vector<std::string> listOfComponentIDs;
			virtual void operator() ( const std::string  &in_ComponentID, 
					std::map<std::string, isis::CADComponentData> &in_out_CADComponentData_map );

			ComponentVistorBuildListOfComponentIDs(bool excludeTopAssembly = false);
			ComponentVistorBuildListOfComponentIDs( ComponentPredicate &in_Predicate, bool in_ExcludeTopAssembly = false );
		private:
			bool flagExcludeEntry;
			ComponentPredicate &predicate;

	};

	class SelectAllComponents : public ComponentPredicate
	{
		public:
			virtual bool operator() ( const isis::CADComponentData &in_CADComponentData);
	};

	class SelectComponentDerivedFromLeafAssembly : public ComponentPredicate
	{
		public:
			virtual bool operator() ( const isis::CADComponentData &in_CADComponentData);
	};

	class SelectComponentInInputXML : public ComponentPredicate
	{
		public:
			virtual bool operator() ( const isis::CADComponentData &in_CADComponentData);
	};


	class SelectLeafAssemblies : public ComponentPredicate
	{
		public:
			virtual bool operator() ( const isis::CADComponentData &in_CADComponentData);
	};
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void ComputeUnitNames_Distance( e_CADUnitsDistance in_Unit, std::string &out_ShortName, std::string &out_LongName  )
																	throw (isis::application_exception);

	void ComputeUnitNames_Mass( e_CADUnitsMass in_Unit, std::string &out_ShortName, std::string &out_LongName  )
																	throw (isis::application_exception);

	void ComputeUnitNames_Force( e_CADUnitsForce in_Unit, std::string &out_ShortName, std::string &out_LongName  )
																	throw (isis::application_exception);

	void ComputeUnitNames_Time( e_CADUnitsTime in_Unit, std::string &out_ShortName, std::string &out_LongName  )
																	throw (isis::application_exception);
	void ComputeUnitNames_Temperature ( e_CADUnitsTemperature in_Unit, std::string &out_ShortName, std::string &out_LongName  )
																	throw (isis::application_exception);

	e_CADUnitsVolume convertDistanceUnitToVolumeUnit( e_CADUnitsDistance in_CADUnitsDistance )
													throw (isis::application_exception);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// This function throws isis::application_exception if the file in_PathAndFileName cannot be opened for write access
	// It is acceptable for in_PartInterferences to have no entries.  This just means no interferences were found.
	void WriteInterferenceReport(	const std::string								&in_AssemblyComponentInstanceID,  // This must be an assembly
									std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map,	
									const CADComputation								&in_CADComputation,
									const std::string								&in_PathAndFileName,
									bool												in_InterferenceAnalysisCompletedWithoutError,
									const std::vector<PartInterferences>				&in_PartInterferences)
																			throw (isis::application_exception);


	struct ComputationTypes
	{
		bool  fEA;
		bool  cFD;
		bool  ballistics;
		bool  blast;
		bool  assemblyBasedComputations; // COMPUTATION_BOUNDING_BOX, COMPUTATION_CG, COMPUTATION_POINT, COMPUTATION_MASS

		ComputationTypes() : fEA(false), cFD(false), ballistics(false), blast(false), assemblyBasedComputations(false) {};
		ComputationTypes( bool in_FEA, bool in_CFD, bool in_Ballistics, bool in_Blast, bool in_AssemblyBasedComputations ): fEA(in_FEA), cFD(in_CFD), ballistics(in_Ballistics), blast(in_Blast), assemblyBasedComputations(in_AssemblyBasedComputations){};
	};

	void BuildList_ComponentsRequiringBoundingBoxes( 
					std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map,
					std::list<std::string>						&out_componentsRequiringBoundingBoxes )
																	throw (isis::application_exception);


	// Description: 
	//		Creates ComputedValues.xml file.  This function only creates ComputedValues.xml for:
	//			1) CFD computations
	//			2) Values computed based on the assembly (e.g. bounding box, C.G., Mass, point coordinates).
	//		This function does NOT created ComputeValues.xml for:
	//			1) FEA, a parameters file is created via CreateXMLFile_FEAPostProcessingParameters
	//			2) Ballistics, a Json file is updated via PopulateBallisticFiles
    //			
	//	Example for CFD:
	//			<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
	//			<Components ConfigurationID="|1" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="CADPostProcessingParameters.xsd">
	//				<Component ComponentID="|1" FEAElementID="" _id="id17">
	//					<Metrics _id="id18">
	//						<Metric DataFormat="SCALAR" MetricID="id-0067-00000001" Type="CoefficientOfDrag" Units="" _id="id19"/>
	//					</Metrics>
	//				</Component>
	//			</Components>
	//
	// Pre-Conditions: 
	//		in_CADAssemblies must point to a valid sis::CADAssemblies type
	//		in_out_CADComponentData_map must be a valid map
	//
	// Post-Conditions:
	//		if in_PathAndFileName could not be opened, throw isis::application_exception
	//      if n_ComputationTypes.cFD == true and there are no CFD computations in in_CADAssemblies, throw isis::application_exception
	//		if no exceptions
	//			if n_ComputationTypes.cFD == true, write the ComputedValues.xml file for CFD
	//			in_ComputationTypes.fEA == true, do nothing, this is ignored.
	//			in_ComputationTypes.ballistics == true, do nothing, this is ignored.
	//			in_ComputationTypes.assemblyBasedComputations == true
	//					then output (if they exists in in_CADAssemblies) computations such as bounding box, C.G. . ... if none exists, do nothing 		
	void CreateXMLFile_ComputedValues( 
						//cad::CadFactoryAbstract								&in_Factory,
						const std::string									&in_in_WorkingDirector,
						const ComputationTypes								&in_ComputationTypes,
						isis::CADAssemblies									&in_CADAssemblies,
						std::map<std::string, isis::CADComponentData>		&in_CADComponentData_map )
																	throw (isis::application_exception);


	// This is an preliminary pass at producing an interference report.  In the future, this function should be
	// broken out into a separate function that gathers the interference information and a separate program
	// that writes the information.  The interference report should include the ComponentInstanceID for each
	// interfering  part.
	//
	//	Pro-Conditions: 
	//		None
	//	Post-Conditions
	//		If in_PathAndFileName exists, it will be overwritten.
	//
	//      In many cases the interference analysis will fail because the SDK interference function fails.  
	//		For those failure cases, isis::application_exception would be thrown.  in_PathAndFileName would
	//		contain the interference results up to the failure point. 
	//
	//		If a interference computation failure occurs, then the end of the in_PathAndFileName would contain
	//		a message indicating that a failure occurred.
	//
	//		If a interference computation failure does not occur, then the end of the in_PathAndFileName would contain
	//		a message that the interference analysis completed successfully.
	//void CreateInterferenceReport( 
	//					const	std::string									&in_PathAndFileName,
	//					const	std::string									&in_ComponentInstanceID,
	//					const   CADComputation								&in_CADComputation,
	//					std::map<std::string, isis::CADComponentData>		&in_CADComponentData_map )
	//																throw (isis::application_exception);
   //

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void OrganizeMetricsBasedOnComponentIDs( 
							const std::list<CADComputation>							&in_Metrics,
							std::map<std::string, std::list<CADComputation>>	&out_componentID_to_ListofComputations_map,
							std::set<std::string>								&out_ComponentIDs_set );

}

#endif