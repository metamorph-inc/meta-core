#ifndef ASSEMBLE_UTILS_H
#define ASSEMBLE_UTILS_H
#include <cc_CommonStructures.h>
#include <cc_CommonFunctions.h>
#include <isis_application_exception.h>
#include <isis_include_ptc_headers.h>
#include <fstream>
#include <map>
#include <unordered_map>
#include "cc_CadFactoryAbstract.h"
#include <CommonFeatureUtils.h>

#include <boost/filesystem.hpp>

namespace isis
{

	ProBoolean Bool_to_ProBoolean ( bool in_Bool);
	bool       ProBoolean_to_Bool ( ProBoolean in_ProBoolean );

	void RetrieveComputationOfAGivenType( const std::list<CADComputation>	&in_AssemblyMetrics,
										  e_ComputationType					in_ComputationType,
										  std::vector<CADComputation>		&out_CADComputations );

	//const std::string manufacturingManifestJson_PathAndFileName = ".\\manufacturing.manifest.json";

	void ProAsmcomppathToList(std::list<int> &out_list, const ProAsmcomppath &in_path);

    ProError Retrieve_ProAsmcomppath(const ProSolid in_assembly_model, const std::list<int> in_base_model_path_list, ProAsmcomppath &out_base_comp_path);
    
	void     Retrieve_ProAsmcomppath_WithExceptions(	const ProSolid in_assembly_model, 
														const std::list<int> &in_base_model_path_list, 
														ProAsmcomppath &out_base_comp_path)  throw (isis::application_exception);
	
	void Populate_c_id_table( const std::list<int> &in_path_list, ProIdTable out_c_id_table, int &out_c_id_table_size );


	void RetrieveTranformationMatrix_Assembly_to_Child (  
							const ProSolid	   &in_assembly_model,
							const std::list<int>	   &in_ChildComponentPaths, 
							ProBoolean   in_bottom_up,
							double out_TransformationMatrix[4][4] )  throw (isis::application_exception);






	///////////////////////////////////////////////////////////////////////////////////////////////////////////////

	struct ModelInstanceData
	{
		MultiFormatString	modelName;
		//ProMdlType			modelType;
		e_CADMdlType		modelType;
		ProSolid			modelHandle;

		// the assebled feature must be a PRO_PART, PRO_ASSEMBLY
		ProAsmcomp			assembledFeature;

		// Information from the top assembly to the instance
		ProSolid			topAssemblyModelHandle;
		std::list<int>			componentPaths;
		
	};

	// Description:
	//		Rename an instance in a Creo assembly to in_ToModelName.
	// Pre-Conditions:
	//		in_ToModelName must be a verbatim copy of the Creo model descirbed in in_FromModelInstanceData
	// Post-Condtions:
	//		if in_ToModelName is not a Creo model in the search path of the Creo session 
	//			then isis::application_exception) would be thrown.
	void  Assembly_RenameSubPartOrSubAssembly (	ModelInstanceData &in_FromModelInstanceData,  // Not constant because ProFeatureElemtreeExtract does not declare input only functions as constant
														MultiFormatString &in_ToModelName,
														ProMdl     &out_RenamedModelHandle)
																throw (isis::application_exception);


	

	// This function computes the bounding box based on excluding all geometry except for the solid geometry.
	// This means that datums, coordinate system, and sketch curves/lines would be excluded.
	// If the bounding box information has already been computed for in_ComponentInstanceID then those
	// values would be returned; otherwise, the values are computed and persisted in in_CADComponentData_map 
	// and then returned.
	/**
	void 	RetrieveBoundingBox_ComputeFirstIfNotAlreadyComputed( 
								//cad::CadFactoryAbstract							&in_Factory,
								const std::string								&in_ComponentInstanceID,
								std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map,
								isis_CADCommon::Point_3D						&out_BoundingBox_Point_1,
								isis_CADCommon::Point_3D						&out_BoundingBox_Point_2,
								double							out_Dimensions_xyz[3] )
																		throw (isis::application_exception);	
***/


	// e.g. "C:\\Users\\Public\\Documents\\META Documents\\MaterialLibrary\\MATERIALS_CREO_MTL"
	std::string CreoMaterialMTLFilesDir_Path();


	void ValidatePathAndModelItem_ThrowExceptionIfInvalid( ProAsmcomppath	&in_Path, ProModelitem  &in_ModelItem )
		throw (isis::application_exception);

	//void PopulateMap_with_Junctions_per_InputXMLConstraints( 
	//				//cad::CadFactoryAbstract							&in_Factory,
	//				const std::vector<std::string>					&in_ListOfComponentIDsInTheAssembly, // This does not include the top-assembly component ID
	//				std::map<std::string, isis::CADComponentData>	&in_out_CADComponentData_map,
	//				bool											in_Force = false)
	//																		throw (isis::application_exception);

	void 	PopulateMap_with_JunctionDataInGlobalCoordinates( 
			//cad::CadFactoryAbstract							&in_Factory,
			const std::string								&in_AssemblyComponentID,
			const std::vector<std::string>					&in_ListOfComponentIDsInTheAssembly, // This does not include the top-assembly component ID
			std::map<std::string, isis::CADComponentData>	&in_out_CADComponentData_map )
																			throw (isis::application_exception);




	//void PopulateMap_with_Junctions_and_ConstrainedToInfo_per_CreoAsmFeatureTrees( 
	//		cad::CadFactoryAbstract													&in_Factory,
	//		const std::vector<std::string>											&in_AssemblyComponentIDs,
	//		const std::unordered_map<IntList, std::string, ContainerHash<IntList>>	&in_FeatureIDs_to_ComponentInstanceID_hashtable,
	//		std::map<std::string, isis::CADComponentData>							&in_out_CADComponentData_map )
	//																	throw (isis::application_exception);


	void ResolveAssemblyConstraints_AddMarkersToMap( 
			//cad::CadFactoryAbstract													&in_Factory,
			const std::vector<std::string>											&in_AssemblyComponentIDs,
			std::unordered_map<IntList, std::string, ContainerHash<IntList>>		&in_FeatureIDs_to_ComponentInstanceID_hashtable,
			std::map<std::string, isis::CADComponentData>	&in_out_CADComponentData_map )
																		throw (isis::application_exception);
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// For valid joint information:
	//		1.  All parts that are not within a treat-as-one-body assembly (i.e. CADAssembly.xml 
	//			special instruction != HAS_KINEMATIC_JOINT ) must be connected to at least one other part.
	//		2.  For a kinematic joint between two parts or a part and assembly, only the two parts or part and assembly can be 
	//			engaged in the constraints.
	//			In other words, if part A is constrained to part B via a kinematic joint (i.e. revolute, prismatic...),
	//			then the constraint features must be between part A and part B.  They can't be between a part other than
	//			A and B.  Another valid example is part A is constrained to datums in assembly B wherein assembly B is a
	//			treat-as-one-body assembly. Only A and B can be engaged in the kinematic joint.
	//			For the assembly example, the assembly B could have datums that reference more than one part that are
	//			subordinate to B and this would still be acceptable since A is being constrained to B; howerver, the
	//          more-than-one-part condition may not be allowed via the Creo UI, but appears to be acceptable via the SDK.
	//			The Creo UI enforces this for kinematic joints.  We must ensure that CADAssembly.xml does not
	//			violate this rule.
	//			Note -	A kinematic joint between two parts is at the set level.  This means that set 0 could be a 
	//					a kinematic joint between one end of bar A to another bar B, and set 1 could be a kinematic
	//					joint from the other end of bar A to a another bar C.  
	//
	//	This function populates out_Errors with error descriptions.  
	//	If (out_Errors.size() == 0 )
	//		then
	//			no errors were found.
	//
	//	This function only checks parts.  If in_ListOfComponentIDsInTheAssembly contains assemblies, then the assemblies are skipped.	
	void CheckValidityOfJointInformation( 
			const std::vector<std::string>					&in_ListOfComponentIDsInTheAssembly, 
			std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map,
			std::vector<std::string>						&out_Errors );



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// This function returns the componentInstanceIDs (out_ReferencedComponentInstanceIDs) that are referenced by the 
	// ProSelections in in_AssembledFeatureDefinition.constraintDefinitions.
	// The parent model (i.e. in_AssembledFeatureDefinition.componentInstanceID  ) is not considered a reference; and 
	// therefore, will not appear in out_ReferencedComponentInstanceIDs.
	void RetrieveReferencedComponentInstanceIDs (
						const std::list<int>												&in_OffSetFeatureIDPath_list,
						const CreoAssembledFeatureDefinition								&in_AssembledFeatureDefinition,
						int																	in_SetIndex,
						const std::unordered_map<IntList, std::string, ContainerHash<IntList>>	&in_FeatureIDs_to_ComponentInstanceID_hashtable,
						std::set<std::string>												&out_ReferencedComponentInstanceIDs)
																				throw (isis::application_exception);

	/****

	void PopulateMap_with_JunctionInformation_SingleJunction( 
					cad::CadFactoryAbstract							&in_Factory,
					const std::string								&in_ComponentID, 
					const std::vector<ConstraintPair>				&in_ConstraintPairs,
					isis::cad::Junction								&out_Junction,
					std::map<std::string, isis::CADComponentData>	&in_out_CADComponentData_map )
						throw (isis::application_exception);

	***/



// xxx missing	inferred_joint.first.type:    REVOLUTE
//inferred_joint.first.location:2.76261820746033480000e+003*e1
//inferred_joint.first.orientation:1.00000000000000000000e+000*e2
//inferred_joint.second.type:   REVOLUTE
//inferred_joint.second.location:1.72938114533873950000e+003*e1 + 3.37884685221013340000e+002*e2 - 2.80000000000000000000e+002*e3
//inferred_joint.second.orientation:1.00000000000000000000e+000*e3

	void PopulateMap_with_JunctionInformation_SingleJunction( 
					//cad::CadFactoryAbstract							&in_Factory,
					const CreoAssembledFeatureDefinition			&in_AssembledFeatureDefinition,
					int												in_SetIndex,
					isis::cad::Junction								&out_Junction,
					std::map<std::string, isis::CADComponentData>	&in_out_CADComponentData_map )
							throw (isis::application_exception);


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Description: 
	//		This function resolves the references of in_FeatureName to the parts that are referenced by in_FeatureName.  
	//		For example, in_FeatureName could be a datum (i.e. plane, axis, point, or coordinate system) and that datum 
	//		could be defined by pointing to part geometry.  This function follows those refereces until it finds the parts 
	//		that own the referenced geometry.  A common use case is a datum plane in an assembly where the datum plane 
	//		points to a second datum plane in a sub-part, wherein the second datum plane points to geometry in the sub-part.  
	//		This function would return the ComponentInstanceIDs of the parts that own the referenced geometry.
	//
	//		It should be noted that a datum can reference more than one geometries and those geometries can be in separate
	//		parts.
	// 
	//	Pre-Conditions:
	//		This function should be called with out_ComponentInstanceIDs_of_PartsReferencedByFeature_set being empty.
	//
	//		in_TopAssemblyModel_ComponentInstanceID must be in in_CADComponentData_map and be the top assembly for the 
	//		entire assembly structure containing in_ComponentInstanceID
	//
	//		in_ComponentInstanceID must be in CADComponentData_map
	//
	//	Post-Conditions
	//		if in_FeatureName not found in in_ComponentInstanceID then throw throw (isis::application_exception)
	//
	//		If no exceptions 
	//			then 
	//				out_ComponentInstanceIDs_of_PartsReferencedByFeature_set would be populated with 0 to many ComponentInstanceIDs.
	//				It could be 0 because in_FeatureName might only have references to an assembly and this function only finds 
	//				references to parts.
	void FindPartsReferencedByFeature(	
						const std::string									&in_TopAssemblyComponentInstanceID, 
						const std::string									&in_ComponentInstanceID,
						const MultiFormatString								&in_FeatureName,
						//ProType											in_FeatureGeometryType,
						e_CADFeatureGeometryType								in_FeatureGeometryType,
						const std::unordered_map<IntList, std::string, ContainerHash<IntList>>		&in_FeatureIDs_to_ComponentInstanceID_hashtable,
						const std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map,
						std::set<std::string>								&out_ComponentInstanceIDs_of_PartsReferencedByFeature_set)
																			throw (isis::application_exception);

}

#endif