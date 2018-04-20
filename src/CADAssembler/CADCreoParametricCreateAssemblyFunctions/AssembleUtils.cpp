#include <AssembleUtils.h>
#include <cc_CommonUtilities.h>
#include <cc_CommonConstants.h>
#include <cc_CommonFunctions.h>
#include <CommonFeatureUtils.h>
//#include "WindowsHDependentCommonFunctions.h"
#include <ToolKitPassThroughFunctions.h>

#include <CommonFunctions.h>
#include <cc_JsonHelper.h>
#include <time.h>
#include "cc_LoggerBoost.h"
#include <CommonFunctions.h>
#include <DatumRefResolver.h>
#include <cc_GraphicsFunctions.h>
#include <boost/algorithm/string.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include <e3ga.h>
#include <cc_WindowsFunctions.h>
#include <JointCreo.h>
#include <cc_AssemblyUtilities.h>
#include <Windows.h>

namespace isis
{
	const int DISABLED_CONSTRAINT_MASK = 32;



	void RetrieveComputationOfAGivenType( const std::list<CADComputation>	&in_AssemblyMetrics,
										  e_ComputationType					in_ComputationType,
										  std::vector<CADComputation>		&out_CADComputations )
	{
		for each (const CADComputation &i in in_AssemblyMetrics)
		{
			if ( i.computationType == in_ComputationType ) out_CADComputations.push_back(i);
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ProBoolean Bool_to_ProBoolean ( bool in_Bool)
	{
		if ( in_Bool )
			return PRO_B_TRUE;
		else
			return PRO_B_FALSE;
	}


	bool ProBoolean_to_Bool ( ProBoolean in_ProBoolean )
	{
		if ( in_ProBoolean == PRO_B_TRUE )
			return true;
		else
			return false;
	}





////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
   Purpose:     Initializes the <i>ProAsmcompath</i> data structure.

   Input Arguments:
      in_assembly_model       - The top-level assembly
      in_base_model_path_list - The idetifiers in the assembly path

   Output Arguments:
      out_base_comp_path      - The assembly component path to a component instance

   Return Values:
      PRO_TK_NO_ERROR   - The function successfully initialized the data structure.
      PRO_TK_BAD_INPUTS - One or more of the input arguments are invalid.
*/
ProError Retrieve_ProAsmcomppath(const ProSolid in_assembly_model, const std::list<int> in_base_model_path_list, ProAsmcomppath &out_base_comp_path) {
   ProIdTable c_id_table_base_model;
   int base_model_table_size;
   Populate_c_id_table( in_base_model_path_list, c_id_table_base_model, base_model_table_size );

   return ProAsmcomppathInit ( in_assembly_model,	
                               c_id_table_base_model,	
                               base_model_table_size,	
                               &out_base_comp_path);
}

void Retrieve_ProAsmcomppath_WithExceptions(	const ProSolid in_assembly_model, 
												const std::list<int> &in_base_model_path_list, 
												ProAsmcomppath &out_base_comp_path)  throw (isis::application_exception)
{
   ProIdTable c_id_table_base_model;
   int base_model_table_size;
   Populate_c_id_table( in_base_model_path_list, c_id_table_base_model, base_model_table_size );

   isis::isis_ProAsmcomppathInit ( in_assembly_model,	
                               c_id_table_base_model,	
                               base_model_table_size,	
                               &out_base_comp_path);
}

void ProAsmcomppathToList(std::list<int> &out_list, const ProAsmcomppath &in_path)
{
	for (int i = 0; i < in_path.table_num; i++)
	{
		out_list.push_back(in_path.comp_id_table[i]);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Populate_c_id_table( const list<int> &in_path_list, ProIdTable out_c_id_table, int &out_c_id_table_size )
{
	int index = 0;
	for ( list<int>::const_iterator i = in_path_list.begin();
	  i != in_path_list.end(); ++i )
	{
		out_c_id_table[index] = *i;
		++index;
	}
	out_c_id_table [index] = -1;
	out_c_id_table_size = in_path_list.size();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////


void RetrieveTranformationMatrix_Assembly_to_Child (  
							const ProSolid		&in_assembly_model,
							const list<int>	   &in_ChildComponentPaths, 
							ProBoolean   in_bottom_up,
							double out_TransformationMatrix[4][4] )  throw (isis::application_exception)
{
	// Must get the path from the assembly to the child
	ProIdTable	c_id_table;
	int			c_id_table_size;

	Populate_c_id_table( in_ChildComponentPaths, c_id_table, c_id_table_size );

	ProAsmcomppath	comp_path;
	isis::isis_ProAsmcomppathInit (	in_assembly_model,	//ProSolid   p_solid_handle
									c_id_table,			// ProIdTable 
									c_id_table_size,	// table_size 
									&comp_path);		// ProAsmcomppath *p_handle


	isis::isis_ProAsmcomppathTrfGet (	&comp_path, 				//	ProAsmcomppath *p_path,
										in_bottom_up,				// ProBoolean   bottom_up,
										out_TransformationMatrix ); //ProMatrix    transformation);

}


	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// STEP file name case:
	// AP203_E2_SEPARATE_PART_FILES
	//		Assembly:						As entered	e.g.  FourBar.asm  -->  FourBar_asm.stp
	//		Part Included in Assembly:		Lower Case  e.g.  BAR_02.prt   -->	bar_02_prt.stp
	//		Part standalone					As entered	e.g.  MaSs		   -->	MaSs_prt.stp 
	// AP203_E2_SINGLE_FILE
	//		Assembly:						As entered	e.g.  FourBar.asm  -->  FourBar_asm.stp
	//		Part standalone					As entered	e.g.  MaSs		   -->	MaSs_prt.stp 	
	// AP203_SINGLE_FILE
	//		Assembly:						As entered	e.g.  FourBar.asm  -->  FourBar_asm.stp
	//		Part standalone					As entered	e.g.  MaSs		   -->	MaSs_prt.stp 	
	// AP214_SEPARATE_PART_FILES
	//		Assembly:						As entered	e.g.  FourBar.asm  -->  FourBar_asm.stp
	//		Part Included in Assembly:		Lower Case  e.g.  BAR_02.prt   -->	bar_02_prt.stp
	//		Part standalone					As entered	e.g.  MaSs		   -->	MaSs_prt.stp 
	// AP214_SINGLE_FILE
	//		Assembly:						As entered	e.g.  FourBar.asm  -->  FourBar_asm.stp
	//		Part standalone					As entered	e.g.  MaSs		   -->	MaSs_prt.stp 	

	////////////////////////////////////////////////////////////////////////////////////////////////////////////


	void RetrieveNameOfAssembledFeature( ProFeature	*in_AssyCompFeature, isis::MultiFormatString &out_ModelName )
	{
		ProElement component_element_tree;
		isis::isis_ProFeatureElemtreeExtract(in_AssyCompFeature, NULL, PRO_FEAT_EXTRACT_NO_OPTS, &component_element_tree);

		ProElempath elemPath;
		ProElempathAlloc(&elemPath);
		ProElempathItem pathItem[1]; 
		pathItem[0].type = PRO_ELEM_PATH_ITEM_TYPE_ID;
		pathItem[0].path_item.elem_id = PRO_E_COMPONENT_MODEL;

		isis::isis_ProElempathDataSet(elemPath, pathItem, 1);

		ProElement component_model_elem;
		isis::isis_ProElemtreeElementGet(component_element_tree, elemPath, &component_model_elem);
		isis::isis_ProElempathFree(&elemPath);

		ProValue  val_component_model;
		isis::isis_ProElementValueGet(component_model_elem, &val_component_model );
		ProValueData value_data;
		isis::isis_ProValueDataGet(val_component_model, &value_data);

		ProMdl	modelHandle = value_data.v.p;

		ProName  modelName_wide;

		isis::isis_ProMdlNameGet(modelHandle, modelName_wide);

		out_ModelName = modelName_wide;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Description:
	//		This function renames a model in the assembly tree.  The analogous Creo UI functionality is “right-click on a model 
	//		in the model tree, select “By Copy”, and  follow prompts”.  Note, the UI function does not rename assemblies; however, 
	//		it is imperative that we are able to rename assemblies.  This function is intended to work on .prts and .ams.
	//
	//		Note - This function was supplied by PTC support.
	//
	//	Pre-Conditions:
	//		1.	in_ToModelName must not exists in the assembly. 
	//		2.	in_AssyCompFeature must point to a valid assembly feature where the assembly feature is for a .prt or .asm.
	//
	//	Post-Conditions:
	//		If the model rename fails, then typically, isis::application_exception would be thown; however, there have been cases 
	//		where the model was not successfully renamed and an exception was not thrown. Subsequent to this function invocation 
	//		there should be a function that validates that the rename worked properly. 
	ProError ReplaceCompFeatwithNewModel( ProFeature	*in_AssyCompFeature, 
										  ProName		in_ToModelName,
										  ProMdl		&out_FromModelHandle,
										  ProMdl		&out_ToModelHandle)
												throw (isis::application_exception)
	{
		ProMdl existing_model;
		ProErr err;
		err = ProAsmcompMdlGet(in_AssyCompFeature, &existing_model);

		ProMdl replacement_model;	
		isis::isis_ProMdlCopy(existing_model, in_ToModelName, &replacement_model);
		out_ToModelHandle= replacement_model;

		ProElement component_element_tree;
		isis::isis_ProFeatureElemtreeExtract(in_AssyCompFeature, NULL, PRO_FEAT_EXTRACT_NO_OPTS, &component_element_tree);

		ProElempath elemPath;
		ProElempathAlloc(&elemPath);
		ProElempathItem pathItem[1]; 
		pathItem[0].type = PRO_ELEM_PATH_ITEM_TYPE_ID;
		pathItem[0].path_item.elem_id = PRO_E_COMPONENT_MODEL;

		isis::isis_ProElempathDataSet(elemPath, pathItem, 1);

		ProElement component_model_elem;
		isis::isis_ProElemtreeElementGet(component_element_tree, elemPath, &component_model_elem);
		isis::isis_ProElempathFree(&elemPath);

		ProValue  val_component_model;
		isis::isis_ProElementValueGet(component_model_elem, &val_component_model );
		ProValueData value_data;
		isis::isis_ProValueDataGet(val_component_model, &value_data);

		out_FromModelHandle = value_data.v.p;

		value_data.v.p = replacement_model;
		isis::isis_ProValueDataSet( val_component_model, &value_data );
		isis::isis_ProElementValueSet( component_model_elem, val_component_model );

		ProModelitem mdlItem;
		isis::isis_ProMdlToModelitem(in_AssyCompFeature->owner, &mdlItem);
		ProSelection mdl_sel;
		ProSelectionAlloc(NULL, &mdlItem, &mdl_sel);
	
		ProErrorlist errors;	
		ProFeatureCreateOptions *opts;  
		isis::isis_ProArrayAlloc(1, sizeof (ProFeatureCreateOptions), 1, (ProArray*) &opts);    
		opts[0] = PRO_FEAT_CR_DEFINE_MISS_ELEMS;

		isis::isis_ProFeatureWithoptionsRedefine(NULL, in_AssyCompFeature, component_element_tree, opts, PRO_REGEN_FORCE_REGEN, &errors);

		isis::isis_ProElempathFree(&elemPath);
		isis::isis_ProSelectionAlloc(NULL, &mdlItem, &mdl_sel);

		return err;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void  Assembly_RenameSubPartOrSubAssembly(	ModelInstanceData	&in_FromModelInstanceData,  // Not constant because ProFeatureElemtreeExtract does not declare input only functions as constant
												MultiFormatString	&in_ToModelName,
												ProMdl				&out_RenamedModelHandle)
					throw (isis::application_exception)
	{
		
		isis_LOG(lg, isis_FILE, isis_INFO) << "\n ***** InAnAssembly_RenamePartOrAssemblyInstance: ";
		isis_LOG(lg, isis_FILE, isis_INFO) << "   topAssemblyModelHandle " << (const void*)in_FromModelInstanceData.topAssemblyModelHandle;
		isis_LOG(lg, isis_FILE, isis_INFO) << "   in_FromModelName:      " << in_FromModelInstanceData.modelName;
		isis_LOG(lg, isis_FILE, isis_INFO) << "   model type:            " << CADMdlType_string(in_FromModelInstanceData.modelType);
		isis_LOG(lg, isis_FILE, isis_INFO) << "   modelHandle:           " << (const void*)in_FromModelInstanceData.modelHandle;
		isis_LOG(lg, isis_FILE, isis_INFO) << "   in_ToModelName:        " << in_ToModelName;
		isis_LOG(lg, isis_FILE, isis_INFO) << "   From Model assembledFeature: ";
		isis_LOG(lg, isis_FILE, isis_INFO) << "        id:    "  << in_FromModelInstanceData.assembledFeature.id;
		isis_LOG(lg, isis_FILE, isis_INFO) << "        owner: "  << (const void*)in_FromModelInstanceData.assembledFeature.owner;
		isis_LOG(lg, isis_FILE, isis_INFO) << "        type: "   << FeatureGeometryType_string(in_FromModelInstanceData.assembledFeature.type);

		/////////////////////////////////////
		// Check Validity of input data
		/////////////////////////////////////
		if ( in_FromModelInstanceData.modelName.size() == 0 )
		{
			std::stringstream errorString;
			errorString 
				<< "Function - InAnAssembly_RenamePartOrAssemblyInstance, recieve an empty in_FromModelInstanceData.modelName string.  See log file for additional information."; 	  
			throw isis::application_exception("C03002", errorString);
		}

		if ( in_ToModelName.size() == 0 )
		{
			std::stringstream errorString;
			errorString 
				<< "Function - InAnAssembly_RenamePartOrAssemblyInstance, recieve an empty in_ToModelName string.  See log file for additional information."; 	  
			throw isis::application_exception("C03002", errorString);
		}

		if ( in_FromModelInstanceData.modelType != CAD_MDL_PART && 
			 in_FromModelInstanceData.modelType != CAD_MDL_ASSEMBLY )

		{
			std::stringstream errorString;
			errorString 
				<< "Function - InAnAssembly_RenamePartOrAssemblyInstance, recieve a model type that was not PRO_MDL_PART or PRO_MDL_ASSEMBLY.  See log file for additional information."; 	  
			throw isis::application_exception("C03002", errorString);
		}
		//// Test 
		
		isis_LOG(lg, isis_FILE, isis_INFO) << "in_FromModelInstanceData.assembledFeature.owner: " << (const void*)in_FromModelInstanceData.assembledFeature.owner;
		isis_LOG(lg, isis_FILE, isis_INFO) << "in_FromModelInstanceData.assembledFeature.id:    " << in_FromModelInstanceData.assembledFeature.id;
		isis_LOG(lg, isis_FILE, isis_INFO) << "in_FromModelInstanceData.assembledFeature.type:  " << in_FromModelInstanceData.assembledFeature.type;		
		
		//ProMdl outModifiedMdl;
		 ProName newName;
		 wcscpy_s(newName, 32, (wchar_t*)(const wchar_t*)in_ToModelName);

		 ProMdl fromModelHandle;

		 ReplaceCompFeatwithNewModel(	&in_FromModelInstanceData.assembledFeature, 
										newName,
										fromModelHandle,
										out_RenamedModelHandle );



		if ( fromModelHandle != in_FromModelInstanceData.modelHandle )
		{
			std::stringstream errorString;
			errorString << "Assembly_RenameSubPartOrSubAssembly - This function renames a model; however this program’s internal data structures for the address of the rename-from model ";
			errorString << std::endl <<	"do not agree with the Creo data structures for the rename-from model.  These addresses must be the same. ";
			errorString << std::endl <<	"This is due to a software error; and therefore, there is not a change you can make that would correct this. ";
			errorString << std::endl <<	"Please report this error to the support desk.";
			throw isis::application_exception("C03002", errorString);
		}


		// Verify that the modified assembled feature points to a model with the name in_ToModelName.  This provides an
		// indication if the rename was/was-not successful.  In some cases the function ReplaceCompFeatwithNewModel will 
		// not complete the rename and not return an error.  
		isis::MultiFormatString assembledFeatureModelName;
		RetrieveNameOfAssembledFeature( &in_FromModelInstanceData.assembledFeature, assembledFeatureModelName );	

		if ( !boost::iequals((const string)in_ToModelName, (const string) assembledFeatureModelName) )
		{
			std::stringstream errorString;
			errorString << "Assembly_RenameSubPartOrSubAssembly - Failed to rename model to: " << in_ToModelName;
			throw isis::application_exception("C03002", errorString);
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/****
	void	 RetrieveBoundingBox_ComputeFirstIfNotAlreadyComputed(
								const std::string								&in_ComponentInstanceID,
								std::map<std::string, isis::CADComponentData>	&in_out_CADComponentData_map,
								isis_CADCommon::Point_3D							&out_BoundingBox_Point_1,
								isis_CADCommon::Point_3D							&out_BoundingBox_Point_2,
								double											out_Dimensions_xyz[3] )
																		throw (isis::application_exception)
	{
		isis::cad::CadFactoryAbstract_global		*cadFactoryAbstract_global_ptr = isis::cad::CadFactoryAbstract_global::instance();
		isis::cad::CadFactoryAbstract::ptr		cAD_Factory_ptr = cadFactoryAbstract_global_ptr->getCadFactoryAbstract_ptr();
		isis::cad::IModelOperations&				modelOperations = cAD_Factory_ptr->getModelOperations();		

		if ( !in_out_CADComponentData_map[in_ComponentInstanceID].boundingBox.boundingBox_Defined )
		{
			modelOperations.retrieveBoundingBox(		in_ComponentInstanceID,
													in_out_CADComponentData_map,
													in_out_CADComponentData_map[in_ComponentInstanceID].boundingBox.boundingBox_Point_1,
													in_out_CADComponentData_map[in_ComponentInstanceID].boundingBox.boundingBox_Point_2,
													in_out_CADComponentData_map[in_ComponentInstanceID].boundingBox.Dimensions_xyz);											
		}

		out_BoundingBox_Point_1 = in_out_CADComponentData_map[in_ComponentInstanceID].boundingBox.boundingBox_Point_1;
		out_BoundingBox_Point_2 = in_out_CADComponentData_map[in_ComponentInstanceID].boundingBox.boundingBox_Point_2;

		out_Dimensions_xyz[0] = in_out_CADComponentData_map[in_ComponentInstanceID].boundingBox.Dimensions_xyz[0];
		out_Dimensions_xyz[1] = in_out_CADComponentData_map[in_ComponentInstanceID].boundingBox.Dimensions_xyz[1];
		out_Dimensions_xyz[2] = in_out_CADComponentData_map[in_ComponentInstanceID].boundingBox.Dimensions_xyz[2];
	}
	****/
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// e.g. "C:\\Users\\Public\\Documents\\META Documents\\MaterialLibrary\\MATERIALS_CREO_MTL"
	std::string CreoMaterialMTLFilesDir_Path()
	{
		std::wstring publicDocuments_wide;

		// Public Documents e.g. "C:\\Users\\Public\\Documents\\META Documents"
		isis_CADCommon::GetPublicDocuments(publicDocuments_wide);
		isis::MultiFormatString publicDocuments_multiformat(publicDocuments_wide.c_str() );

		// Materials MTD Dir
		// e.g. "C:\\Users\\Public\\Documents\\META Documents\\MaterialLibrary\\MATERIALS_CREO_MTL"

		return (std::string)publicDocuments_multiformat + std::string("\\META Documents\\MaterialLibrary\\MATERIALS_CREO_MTL");	
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ValidatePathAndModelItem_ThrowExceptionIfInvalid( ProAsmcomppath	&in_Path, ProModelitem  &in_ModelItem )
		throw (isis::application_exception)
{
	// in_base_comp_path should point to the same model as in_base_model_datum.owner

	ProMdl         p_ModelPathModel;

	isis_ProAsmcomppathMdlGet( &in_Path, &p_ModelPathModel);

	if ( p_ModelPathModel != in_ModelItem.owner )
	{
		std::stringstream errorString;
		errorString << "ValidatePathAndModelItem_ThrowExceptionIfInvalid, Component-path (i.e. ProAsmcomppath) model is not the same as ";
		errorString << std::endl <<	"the model-item (i.e. ProModelitem) model.  They must be the same. This is due to a software error; and therefore,  ";
		errorString << std::endl <<	"there is not a change you can make that would correct this. Please report this error to the support desk.";

		errorString <<  std::endl <<  std::endl << "Path: ";
				errorString <<  std::endl << "   Path Top Model: " << in_Path.owner;
		errorString <<  std::endl << "   Path (Feature IDs): ";
		errorString <<  std::endl << "      Number of Feature IDs in the path: " << in_Path.table_num;

		if ( in_Path.table_num > -1 &&  in_Path.table_num <= PRO_MAX_ASSEM_LEVEL )
		{

			ProAsmcomppath   proAsmcomppath_temp;
			proAsmcomppath_temp.owner						=	in_Path.owner;
			proAsmcomppath_temp.table_num					=	in_Path.table_num;
			for (int i = 0; i < in_Path.table_num; ++i )		proAsmcomppath_temp.comp_id_table[i] = in_Path.comp_id_table[i];

			for ( int i = 0; i < in_Path.table_num; ++i )
			{
				errorString << std::endl << "      Path Feature ID: " <<  in_Path.comp_id_table[i] << "   Model Name: ";
				// Get the model name to this point in the path

				proAsmcomppath_temp.table_num = i + 1;

				try
				{
					ProMdl		p_ModelPathModel_temp;
					isis_ProAsmcomppathMdlGet( &proAsmcomppath_temp, &p_ModelPathModel_temp);

					ProName modelPathName_temp;
					isis::isis_ProMdlNameGet ( p_ModelPathModel_temp, modelPathName_temp );
					isis::MultiFormatString modelPathName_multiFormat_temp(modelPathName_temp);
					errorString << (std::string) modelPathName_multiFormat_temp;
				}
				catch (...)
				{
					errorString <<  "Could not retrieve model name based on the path.";
				}
			}
		}
		else
		{
			errorString << std::endl << "   Number of feature IDs in the path is not in the range of 0 to " << PRO_MAX_ASSEM_LEVEL << ".";
		}

		errorString <<  std::endl <<  std::endl << "Model Item: ";
		errorString <<  std::endl << "   Model Item Owner:         " << in_ModelItem.owner;
		errorString <<  std::endl << "   Model Item Feature ID:    " << in_ModelItem.id;
		errorString <<  std::endl << "   Model Item Type (1-Assembly, 2-Part, 5-Surface, 21-Axis, 66-Point): " << in_ModelItem.type;
		errorString <<  std::endl;
	
		try
		{
			ProName modelPathName;
			isis::isis_ProMdlNameGet ( p_ModelPathModel, modelPathName );
			isis::MultiFormatString modelPathName_multiFormat(modelPathName);
			errorString <<  std::endl << "Path model name:        "  << (std::string) modelPathName_multiFormat;
			errorString <<  std::endl << "Path model address:     " <<  p_ModelPathModel;
		}
		catch (...)
		{
			errorString <<  std::endl << "Could not retrieve the model-item model name";
		}

		ProName modelItemName;
		try
		{
			ProName modelItemName;
			isis::isis_ProMdlNameGet ( in_ModelItem.owner, modelItemName );
			isis::MultiFormatString modelItemName_multiFormat(modelItemName);
			errorString <<  std::endl <<  std::endl << "Model-Item model name:     " << (std::string) modelItemName_multiFormat;
			
			ProName modelItemFeatureName;
			isis::isis_ProModelitemNameGet ( &in_ModelItem, modelItemFeatureName );
			isis::MultiFormatString modelItemFeatureName_multiFormat(modelItemFeatureName);
			errorString <<  std::endl <<  "Model-Item feature name:   " << (std::string) modelItemFeatureName_multiFormat;
			errorString <<  std::endl <<  "Model-Item owner address:  " <<  in_ModelItem.owner;
		}
		catch (...)
		{
			errorString <<  std::endl << "Could not retrieve the model-item model name";
		}
		throw isis::application_exception("C04002",errorString.str().c_str());		
	}


}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/***
	void PopulateMap_with_JunctionInformation_SingleJunction( 
					cad::CadFactoryAbstract							&in_Factory,
					const std::string								&in_ComponentID, 
					const std::vector<ConstraintPair>				&in_ConstraintPairs,
					isis::cad::Junction								&out_Junction,
					std::map<std::string, isis::CADComponentData>	&in_out_CADComponentData_map )
						throw (isis::application_exception)
	{
		

		isis_LOG(lg, isis_FILE, isis_INFO) << "*************  PopulateMap_with_JunctionInformation_SingleJunction (Constraints derived from CADAssembly.xml)";

		isis_LOG(lg, isis_FILE, isis_INFO) << (std::string)in_out_CADComponentData_map[in_ComponentID].name;

		cad::IAssembler& assembler = in_Factory.get_assembler();

		std::vector< cad::Joint::pair_t > joint_pair_vector = assembler.extract_joint_pair_vector(in_ComponentID, in_ConstraintPairs, in_out_CADComponentData_map);
		
		for each ( ConstraintPair i in in_ConstraintPairs )
		{
			isis_LOG(lg, isis_FILE, isis_INFO) << "FeatureGeometryType_string(i.featureGeometryType): " << FeatureGeometryType_string(i.featureGeometryType);
			for each ( ConstraintFeature j in i.constraintFeatures )
				isis_LOG(lg, isis_FILE, isis_INFO) << "   FeatureName: "  << (std::string) j.featureName;
		}

		cad::Joint::pair_t inferred_joint = cad::infer_joint_pair(joint_pair_vector);

		isis_LOG(lg, isis_FILE, isis_INFO) << "inferred_joint.first.type:    " << JointType_string(inferred_joint.first.type);
		isis_LOG(lg, isis_FILE, isis_INFO) << "inferred_joint.first.location:" << inferred_joint.first.location.c_str_e20();
		isis_LOG(lg, isis_FILE, isis_INFO) << "inferred_joint.first.orientation:" << inferred_joint.first.orientation.c_str_e20();
		isis_LOG(lg, isis_FILE, isis_INFO) << "inferred_joint.second.type:   " << JointType_string(inferred_joint.second.type);
		isis_LOG(lg, isis_FILE, isis_INFO) << "inferred_joint.second.location:" << inferred_joint.second.location.c_str_e20();
		isis_LOG(lg, isis_FILE, isis_INFO) << "inferred_joint.second.orientation:" << inferred_joint.second.orientation.c_str_e20();


		out_Junction.update(inferred_joint);

		// A type of cad::COMPOSITE means that infer_joint_pair could not determine the actual joint type.  This program
		// should be extended to evaluate the particular set of datums.
		if ( inferred_joint.first.type == cad::COMPOSITE || inferred_joint.second.type == cad::COMPOSITE )
		{
			std::stringstream errorString;
			errorString 
				<< "Function - infer_joint_pair, returned a type of COMPOSITE.  This means that this program " 
				<<	std::endl << "could not infer the joint type given the set of datums; therefore, this program should be "
				<<	std::endl << "extended to handle this case.";

			errorString << std::endl << "   Assembled model name: " << (std::string)in_out_CADComponentData_map[in_ComponentID].name;
			for each( ConstraintPair i in in_ConstraintPairs )
			{
				errorString << std::endl << "      FeatureGeometryType: " << CADFeatureGeometryType_string(i.featureGeometryType);
				for each ( ConstraintFeature j in i.constraintFeatures ) errorString << std::endl << "         ModelName: " << (string)in_out_CADComponentData_map[j.componentInstanceID].name <<  "  FeatureName: "  << (std::string) j.featureName;
			}
			errorString << std::endl << "   inferred_joint.first: " << inferred_joint.first;
			errorString << std::endl << "   inferred_joint.second: " << inferred_joint.second;

			for each (  cad::Joint::pair_t i in joint_pair_vector ) 
			{
				errorString << std::endl << "   First Joint Primitive:  " << i.first;
				errorString << std::endl << "   Second Joint Primitive: " << i.second;
			}

			for each ( cad::Joint i in inferred_joint.first.composite )
			{
				errorString << std::endl << "   Composite joint: " << i;
			}


			isis_LOG(lg, isis_FILE, isis_WARN) << errorString.str();

			// snyako@isis.vanderbilt.edu : take this error lightly for the time being - otherwise many older model won't work.

			//throw isis::application_exception(errorString);


		}

	}
	****/
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void PopulateMap_with_JunctionInformation_SingleJunction( 
					//cad::CadFactoryAbstract							&in_Factory,
					const CreoAssembledFeatureDefinition			&in_AssembledFeatureDefinition,
					int												in_SetIndex,
					isis::cad::Junction								&out_Junction,
					std::map<std::string, isis::CADComponentData>	&in_out_CADComponentData_map )
						throw (isis::application_exception)
	{
		
		//isis::cad::CadFactoryAbstract_global *cadFactoryAbstract_global_ptr = isis::cad::CadFactoryAbstract_global::instance();
		//isis::cad::CadFactoryAbstract::ptr	cAD_Factory_ptr = cadFactoryAbstract_global_ptr->getCadFactoryAbstract_ptr();

		isis_LOG(lg, isis_FILE, isis_INFO) << "*************  PopulateMap_with_JunctionInformation_SingleJunction (Constraints derived from feature tree)";

		isis_LOG(lg, isis_FILE, isis_INFO) << in_AssembledFeatureDefinition;

		isis_LOG(lg, isis_FILE, isis_INFO) << (std::string)in_out_CADComponentData_map[in_AssembledFeatureDefinition.componentInstanceID].name;

		//cad::IAssembler& assembler = cAD_Factory_ptr->get_assembler();

		std::vector< cad::Joint::pair_t > joint_pair_vector;

		int constraintOffsetCounter = 0;

		for each ( CreoConstraintDefinition i in in_AssembledFeatureDefinition.constraintDefinitions )
		{	
			if ( in_SetIndex == i.component_constr_set_id )  
			{		
				//std::cout << std::endl << "#######: disabledConstraint: " << (i.component_constr_attr & DISABLED_CONSTRAINT_MASK );

				if ( (i.component_constr_attr & DISABLED_CONSTRAINT_MASK ) == DISABLED_CONSTRAINT_MASK ) 
				{					
					isis_LOG(lg, isis_FILE, isis_INFO) << "Constraint disabled.  SetIndex: " <<  i.component_constr_set_id << ", Index within constraint set: " << constraintOffsetCounter << ", Component Instance ID: " <<  in_AssembledFeatureDefinition.componentInstanceID;	
					continue;  
				}
				cad::Joint::pair_t  jointPair;
				// first must always be component_comp_constr_ref
				jointPair.first = cad::creo::extract_marker(i.component_comp_constr_ref);
				jointPair.second = cad::creo::extract_marker(i.component_assem_constr_ref);
				joint_pair_vector.push_back(jointPair);
			}
			++constraintOffsetCounter;
		}
		//std::vector< cad::Joint::pair_t > joint_pair_vector = assembler.extract_joint_pair_vector(in_ComponentID, in_ConstraintPairs, in_out_CADComponentData_map);

		if ( joint_pair_vector.size() == 0 )
		{
			isis_LOG(lg, isis_FILE, isis_INFO) << "All constraints were disabled, SetIndex: " << in_SetIndex;
			isis_LOG(lg, isis_FILE, isis_INFO) << "in_AssembledFeatureDefinition: ";
			isis_LOG(lg, isis_FILE, isis_INFO) << in_AssembledFeatureDefinition;
			return; 
		}

		cad::Joint::pair_t inferred_joint = cad::infer_joint_pair(joint_pair_vector);

		//isis_LOG(lg, isis_FILE, isis_INFO) << "inferred_joint.first.type:    " << JointType_string(inferred_joint.first.type);
		//isis_LOG(lg, isis_FILE, isis_INFO) << "inferred_joint.first.location:" << inferred_joint.first.location.c_str_e20();
		//isis_LOG(lg, isis_FILE, isis_INFO) << "inferred_joint.first.orientation:" << inferred_joint.first.orientation.c_str_e20();
		//isis_LOG(lg, isis_FILE, isis_INFO) << "inferred_joint.second.type:   " << JointType_string(inferred_joint.second.type);
		//isis_LOG(lg, isis_FILE, isis_INFO) << "inferred_joint.second.location:" << inferred_joint.second.location.c_str_e20();
		//isis_LOG(lg, isis_FILE, isis_INFO) << "inferred_joint.second.orientation:" << inferred_joint.second.orientation.c_str_e20();

		out_Junction.update(inferred_joint);

		// A type of cad::COMPOSITE means that infer_joint_pair could not determine the actual joint type.  This program
		// should be extended to evaluate the particular set of datums.
		if ( inferred_joint.first.type == cad::COMPOSITE || inferred_joint.second.type == cad::COMPOSITE )
		{
			std::stringstream errorString;
			errorString 
				<< "Function - infer_joint_pair, returned a type of COMPOSITE.  This means that this program " 
				<<	std::endl << "could not infer the joint type given the set of datums; therefore, this program should be "
				<<	std::endl << "extended to handle this case.";

			errorString << in_AssembledFeatureDefinition;

			errorString << std::endl << "   inferred_joint.first: " << inferred_joint.first;
			errorString << std::endl << "   inferred_joint.second: " << inferred_joint.second;

			for each (  cad::Joint::pair_t i in joint_pair_vector ) 
			{
				errorString << std::endl << "   First Joint Primitive:  " << i.first;
				errorString << std::endl << "   Second Joint Primitive: " << i.second;
			}

			for each ( cad::Joint i in inferred_joint.first.composite )
			{
				errorString << std::endl << "   Composite joint: " << i;
			}

			isis_LOG(lg, isis_FILE, isis_WARN) << errorString.str();

			// snyako@isis.vanderbilt.edu : take this error lightly for the time being - otherwise many older model won't work.

			//throw isis::application_exception(errorString);

		}

	}

	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Description:
	//		if in_ProSelection refers to a .prt return the .prt ComponentInstanceID in out_ReferencedPartComponentInstanceID
	// Pre-Conditions:
	//		in_ProSelection must point to a valid ProSelection object.
	// Post-Conditions:
	//		if in_ProSelection points to a .prt
	//			return out_ReferencedPartComponentInstanceID populated with the ComponentInstanceID of the .prt 
	//		if in_ProSelection points to a .asm
	//			return an empty out_ReferencedPartComponentInstanceID
	//		if in_ProSelection does not contain a .prt or .asm that is in in_FeatureIDs_to_ComponentInstanceID_hashtable
	//			throw (isis::application_exception)
	void GetReferencedPartComponentInstanceID ( 
						ProSelection															in_ProSelection, 
						const std::list<int>													&in_OffSetFeatureIDPath_list,
						const std::unordered_map<IntList, std::string, ContainerHash<IntList>>	&in_FeatureIDs_to_ComponentInstanceID_hashtable,
						std::string																&out_ReferencedPartComponentInstanceID )
															throw (isis::application_exception)
	{
		
		

		ProAsmcomppath asmCompPath;
		isis_ProSelectionAsmcomppathGet(in_ProSelection, &asmCompPath );

		ProModelitem  selectionModelItem;
		isis::isis_ProSelectionModelitemGet( in_ProSelection, &selectionModelItem );

		ProMdlType ownerProMdlType;
		isis_ProMdlTypeGet(selectionModelItem.owner, &ownerProMdlType );

		
		if ( ownerProMdlType != PRO_MDL_PART )
		{
			// Don't log this condition.  Parts will be constrained to assemblies, but the real test is if every part
			// is constrained to at least one other part.  That will be checked in AssemblyCreationViaInputFile.cpp for
			// the case where OutputJointInformation == true.
			//try
			//{
			//	ProName ownerModelName_wide;
			//	isis::isis_ProMdlNameGet( selectionModelItem.owner, ownerModelName_wide);
			//	isis::MultiFormatString ownerModelName_wide_multi(ownerModelName_wide);
			//	isis_LOG(lg, isis_FILE, isis_WARN) << "\nWARNING, " << __FUNCTION__ << " was passed a ProSelection with an owner type of: " << isis::ProMdlType_string(ownerProMdlType);
			//	isis_LOG(lg, isis_FILE, isis_WARN) << "         Parent (constrained via the ProSelection ) Model Name: " <<  (std::string)ownerModelName_wide_multi;
			//	isis_LOG(lg, isis_FILE, isis_WARN) << "         Currently only types of PRO_MDL_PART are supported.  This may result in the metrics file having missing joint information.";
			//	isis_LOG(lg, isis_FILE, isis_WARN) << "         If every .prt is constrained to at least one other .prt, then the necessary joint information will be present in the metrics file.";
			//}
			//catch ( isis::application_exception &ex)
			//{
			//	isis_LOG(lg, isis_FILE, isis_ERROR) << "ERROR GetReferencedPartComponentInstanceID failed to retrieve the ModelItem for the owner of a ProSelection";
			//	throw;
			//}
			return;
		}
                     

		// WARNING - Limiting this to paths that have a least one feature ID is fundamentally wrong.  This says
		//			 we can't have a reference to the very top assembly.  Since, for now we, are excluding assemblies
		//			 (i.e. only targeting .prts) this should be ok.
		if ( asmCompPath.table_num > 0 )  // Found a feature ID
		{
			//std::cout << std::endl << "GetReferencedPartComponentInstanceID, Feature IDs";
			std::list<int> featureIDs_list; 
			featureIDs_list = in_OffSetFeatureIDPath_list;  // Start with feature IDs leading to the parent assembly.

			for ( int j = 0; j < asmCompPath.table_num; ++j ) 
			{
				//std::cout << std::endl << "   Feature ID: " << asmCompPath.comp_id_table[j];
				featureIDs_list.push_back(asmCompPath.comp_id_table[j]);
			}

			std::unordered_map<IntList, std::string, ContainerHash<IntList>>::const_iterator itr; 
			itr = in_FeatureIDs_to_ComponentInstanceID_hashtable.find(featureIDs_list);
		
			//std::cout << std::endl << "SSSSS Search for key: "; for each ( int jj in featureIDs_list) std::cout << "  " << jj;
			if ( itr != in_FeatureIDs_to_ComponentInstanceID_hashtable.end() )
			{
				out_ReferencedPartComponentInstanceID = itr->second;
			//	std::cout << std::endl << "PPPP hash table BEGIN";
			//	for each ( std::pair< IntList, std::string> i_pair in in_FeatureIDs_to_ComponentInstanceID_hashtable )
			//	{
			//		std::cout << std::endl << "   Key:   ";
			//		for each (int k in i_pair.first ) std::cout << "  " << k;
			//		std::cout << std::endl << "   Value: " << i_pair.second;
			//	}
			//	std::cout << std::endl << "PPPP hash table END";
				// snyako@isis.vanderbilt.edu: This logging is too excessive
				/*std::cout << std::endl << "PPPP hash table BEGIN";
				for each ( std::pair< IntList, std::string> i_pair in in_FeatureIDs_to_ComponentInstanceID_hashtable )
				{
					std::cout << std::endl << "   Key:   ";
					for each (int k in i_pair.first ) std::cout << "  " << k;
					std::cout << std::endl << "   Value: " << i_pair.second;
				}
				std::cout << std::endl << "PPPP hash table END";*/
			}	
			else
			{	
			//	std::cout << std::endl << "PPPP hash table BEGIN";
			//	for each ( std::pair< IntList, std::string> i_pair in in_FeatureIDs_to_ComponentInstanceID_hashtable )
			//	{
			//		std::cout << std::endl << "   Key:   ";
			//		for each (int k in i_pair.first ) std::cout << "  " << k;
			//		std::cout << std::endl << "   Value: " << i_pair.second;
			//	}
			//	std::cout << std::endl << "PPPP hash table END";

				std::stringstream errorString;
				errorString << "Function - GetReferencedPartComponentInstanceID, could not find a known ComponentInstanceID for in_ProSelection.";
				throw isis::application_exception(errorString);					
			}
		}

	}

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
																				throw (isis::application_exception)
	{
		

		for each ( CreoConstraintDefinition i in in_AssembledFeatureDefinition.constraintDefinitions )
		{	
			if ( in_SetIndex == i.component_constr_set_id )
			{
				DatumRefResolver datumRefResolver_assem_constr_ref(i.component_assem_constr_ref);
				DatumRefResolver datumRefResolver_comp_constr_ref(i.component_comp_constr_ref);

				vector<ProSelection> resolved_assem_constr_ref;
				vector<ProSelection> resolved_comp_constr_ref;

				try{
					datumRefResolver_assem_constr_ref.Resolve(resolved_assem_constr_ref);
					datumRefResolver_comp_constr_ref.Resolve(resolved_comp_constr_ref);
				} catch (isis::application_exception &ex)
				{
					isis_LOG(lg, isis_FILE, isis_ERROR) << "Datum resolution failed: " << ex.what();
				}

				for each ( ProSelection i in resolved_assem_constr_ref )
				{
					std::string componentInstanceID;				
					GetReferencedPartComponentInstanceID (	i, 
															in_OffSetFeatureIDPath_list,
															in_FeatureIDs_to_ComponentInstanceID_hashtable, 
															componentInstanceID );				
					if ( componentInstanceID.size() > 0 && componentInstanceID != in_AssembledFeatureDefinition.componentInstanceID  )
					{
										out_ReferencedComponentInstanceIDs.insert(componentInstanceID);

					isis_LOG(lg, isis_FILE, isis_INFO) << "RetrieveReferencedComponentInstanceIDs, resolved_assem_constr_ref, componentInstanceID: " << componentInstanceID;
					}
				}

				for each ( ProSelection i in resolved_comp_constr_ref )
				{
					std::string componentInstanceID;				
					GetReferencedPartComponentInstanceID (	i, 
															in_OffSetFeatureIDPath_list,
															in_FeatureIDs_to_ComponentInstanceID_hashtable, 
															componentInstanceID );				
					if ( componentInstanceID.size() > 0 && componentInstanceID != in_AssembledFeatureDefinition.componentInstanceID  )
					{
						out_ReferencedComponentInstanceIDs.insert(componentInstanceID);
						isis_LOG(lg, isis_FILE, isis_INFO) << "vvvvvvvvvv RetrieveReferencedComponentInstanceIDs, resolved_comp_constr_ref, componentInstanceID: " << componentInstanceID;
					}
				}
			}			
		}

	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	static void transform(const e3ga::vector &location, const e3ga::vector &orientation, double m[4][4], e3ga::vector &out_location, e3ga::vector& out_orientation)
	{
		//double x = m[0][0]*orientation.m_e1+m[0][1]*orientation.m_e2+m[0][2]*orientation.m_e3;
		//double y = m[1][0]*orientation.m_e1+m[1][1]*orientation.m_e2+m[1][2]*orientation.m_e3;
		//double z = m[2][0]*orientation.m_e1+m[2][1]*orientation.m_e2+m[2][2]*orientation.m_e3;
		//out_orientation.m_e1 = x;
		//out_orientation.m_e2 = y;
		//out_orientation.m_e3 = z;

		// Add logging so that functions ProPntTrfEval and ProVectorTrfEval can be replaced with non Creo functions
		isis_LOG(lg, isis_FILE, isis_INFO) << "******************** BEGIN transform(const e3ga::vector &location, const e3ga::vector &orientation,... ****************";


		double p[3];
		p[0] = location.m_e1;
		p[1] = location.m_e2;
		p[2] = location.m_e3;

		isis_LOG(lg, isis_FILE, isis_INFO) << "Transformation Matrix:";
		isis_LOG(lg, isis_FILE, isis_INFO) << "   " << m[0][0] << "  " << m[0][1] << "  " << m[0][2] << "  " << m[0][3]; 
		isis_LOG(lg, isis_FILE, isis_INFO) << "   " << m[1][0] << "  " << m[1][1] << "  " << m[1][2] << "  " << m[1][3]; 
		isis_LOG(lg, isis_FILE, isis_INFO) << "   " << m[2][0] << "  " << m[2][1] << "  " << m[2][2] << "  " << m[2][3]; 
		isis_LOG(lg, isis_FILE, isis_INFO) << "   " << m[3][0] << "  " << m[3][1] << "  " << m[3][2] << "  " << m[3][3]; 

		isis_LOG(lg, isis_FILE, isis_INFO) << "input point x, y, z:              " << location.m_e1 << "  " << location.m_e2 << "  " << location.m_e3;

		double buff[3];
		ProError err = ProPntTrfEval(p, m, buff);
		out_location.m_e1 = buff[0];
		out_location.m_e2 = buff[1];
		out_location.m_e3 = buff[2];

		isis_LOG(lg, isis_FILE, isis_INFO) << "transformed point via Creo Functions x, y, z:    " << out_location.m_e1 << "  " << out_location.m_e2 << "  " << out_location.m_e3;

		/////////////////////////////////////////////////////////////////////////////
		// BEGIN Test Math Functions Transformation of Point
		/////////////////////////////////////////////////////////////////////////////

		double	rotationMatrix[3][3];
		std::vector<double>	offset(3, 0.0);

		offset[0] = m[0][3]; 
		offset[1] = m[1][3]; 
		offset[2] = m[2][3]; 

		for (int i = 0; i < 3; ++i ) 
			for (int j = 0; j < 3; ++j )  rotationMatrix[i][j] = m[i][j];

		isis_CADCommon::TransformationMatrix transformationMatrix;

		transformationMatrix.setTransformationMatrix(rotationMatrix, offset);

		isis_CADCommon::Point_3D  point_start(	location.m_e1,
												location.m_e2,
												location.m_e3);
	
		isis_CADCommon::Point_3D  point_transformed = transformationMatrix.getTransformedCoordinates( point_start);
		isis_LOG(lg, isis_FILE, isis_INFO) << "transformed point via Math Functions x, y, z:   " << point_transformed.x << "  " << point_transformed.y  << "  " << point_transformed.z ;

		/////////////////////////////////////////////////////////////////////////////
		// END est Math Functions Transformation of Point
		/////////////////////////////////////////////////////////////////////////////

		p[0] = orientation.m_e1;
		p[1] = orientation.m_e2;
		p[2] = orientation.m_e3;

		isis_LOG(lg, isis_FILE, isis_INFO) << "orientation x, y, z:             " << orientation.m_e1 << "  " << orientation.m_e2 << "  " << orientation.m_e3;

		err = ProVectorTrfEval(p, m, buff);
		out_orientation.m_e1 = buff[0];
		out_orientation.m_e2 = buff[1];
		out_orientation.m_e3 = buff[2];

		isis_LOG(lg, isis_FILE, isis_INFO) << "transformed orientation via Creo Functions x, y, z: " << out_orientation.m_e1 << "  " << out_orientation.m_e2 << "  " << out_orientation.m_e3;

		/////////////////////////////////////////////////////////////////////////////
		// BEGIN Test Math Functions Transformation of Orientation
		/////////////////////////////////////////////////////////////////////////////
		// Only interested in rotation,  setting offset to zero
		std::vector<double>	offset_zeros(3, 0.0);
		transformationMatrix.setTransformationMatrix(rotationMatrix, offset_zeros);

		isis_CADCommon::Point_3D  orientation_start(	orientation.m_e1,
													orientation.m_e2,
													orientation.m_e3);

		isis_CADCommon::Point_3D  orientation_transformed = transformationMatrix.getTransformedCoordinates( orientation_start);

		isis_LOG(lg, isis_FILE, isis_INFO) << "transformed orientation via Math Functions x, y, z: " << orientation_transformed.x << "  " << orientation_transformed.y << "  " << orientation_transformed.z;

		/////////////////////////////////////////////////////////////////////////////
		// END Test Math Functions Transformation of Orientation
		/////////////////////////////////////////////////////////////////////////////

		isis_LOG(lg, isis_FILE, isis_INFO) << "******************** END transform(const e3ga::vector &location, const e3ga::vector &orientation,... *****************";

	}
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//    MOVE this to cc_ when the transfrom (previous function) function is moved


	void 	PopulateMap_with_JunctionDataInGlobalCoordinates( 
			//cad::CadFactoryAbstract							&in_Factory,
			const std::string								&in_AssemblyComponentID,
			const std::vector<std::string>					&in_ListOfComponentIDsInTheAssembly, // This includes the assembly component ID
			std::map<std::string, isis::CADComponentData>	&in_out_CADComponentData_map )
																			throw (isis::application_exception)
	{		
		
		isis::cad::CadFactoryAbstract_global *cadFactoryAbstract_global_ptr = isis::cad::CadFactoryAbstract_global::instance();
		isis::cad::CadFactoryAbstract::ptr	cAD_Factory_ptr = cadFactoryAbstract_global_ptr->getCadFactoryAbstract_ptr();

		for each ( const std::string &i in in_ListOfComponentIDsInTheAssembly )
		{
			for ( std::vector<ConstraintData>::iterator j = in_out_CADComponentData_map[i].constraintDef.constraints.begin();
				j != in_out_CADComponentData_map[i].constraintDef.constraints.end();
				++j )
			{
				if ( !j->computedJointData.junctiondDefined_withoutGuide )
				{
					std::stringstream errorString;
					errorString 
					<< "Function - PopulateMap_with_JunctionDataInGlobalCoordinates, component does not have a junction defined, " <<
					   std::endl << "and thus the global coordinates of the junction cannot be computed.  " <<  
					   std::endl <<	"   Model Name:            " <<	 in_out_CADComponentData_map[i].name << 
					   std::endl << "   Model Type:            " <<  isis::CADMdlType_string(in_out_CADComponentData_map[i].modelType) << 
					   std::endl << "   Component Instance ID: " <<  i;   	  
					throw isis::application_exception(errorString);
				}



				
				// Get transformation matrix from the global coordinate system to the assembled .prt/.asm
				double transformationMatrix[4][4];  // rotation 3 X 3, translation at bottom row of the 4 X 4
				//RetrieveTranformationMatrix_Assembly_to_Child (	in_AssemblyComponentID,
				//												in_out_CADComponentData_map[j->computedJointData.coordinateSystem_ComponentInstanceID].componentPaths,
				//												in_out_CADComponentData_map,  
				//												PRO_B_TRUE,  // bottom up
				//												transformationMatrix );

				isis::cad::IModelOperations&         modelOperations = cAD_Factory_ptr->getModelOperations();
				modelOperations.retrieveTranformationMatrix_Assembly_to_Child ( in_AssemblyComponentID,
																				j->computedJointData.coordinateSystem_ComponentInstanceID,  
																				in_out_CADComponentData_map,  
																				true,  // bottom up
																				transformationMatrix );

#if 0
				// There's something wrong with the quaternion transformation
				cad::MotorPair motorPair = cad::MotorPair::create(transformationMatrix);
				cad::Joint jointGlobal = motorPair.move(j->computedJointData.junction_withoutguide.joint_pair.first);
#else
				// Use matrix multiplication until the quaternion rotation is not fixed
				isis_LOG(lg, isis_FILE, isis_INFO) << "Computing global coordinates of " << i << ", In the coordinate system of: " << j->computedJointData.coordinateSystem_ComponentInstanceID;
				isis_LOG(lg, isis_FILE, isis_INFO) << "   location: " << j->computedJointData.junction_withoutguide.joint_pair.first.location.c_str_e20();
				isis_LOG(lg, isis_FILE, isis_INFO) << "   orient  : " << j->computedJointData.junction_withoutguide.joint_pair.first.orientation.c_str_e20();
				e3ga::vector loc = j->computedJointData.junction_withoutguide.joint_pair.first.location;
				e3ga::vector orient = j->computedJointData.junction_withoutguide.joint_pair.first.orientation;
				transform(j->computedJointData.junction_withoutguide.joint_pair.first.location, j->computedJointData.junction_withoutguide.joint_pair.first.orientation, transformationMatrix, loc, orient);
				orient = e3ga::unit(orient);
				cad::Joint jointGlobal(j->computedJointData.junction_withoutguide.joint_pair.first.type, loc, orient, j->computedJointData.junction_withoutguide.joint_pair.first.rotation, 0);
				isis_LOG(lg, isis_FILE, isis_INFO) << " transformed";
				isis_LOG(lg, isis_FILE, isis_INFO) << "   location: " << loc.c_str_e20();
				isis_LOG(lg, isis_FILE, isis_INFO) << "   orient  : " << orient.c_str_e20();

#endif

				j->computedJointData.globalJointPosition.location.x = jointGlobal.location.get_e1();
				j->computedJointData.globalJointPosition.location.y = jointGlobal.location.get_e2();
				j->computedJointData.globalJointPosition.location.z = jointGlobal.location.get_e3();

				j->computedJointData.globalJointPosition.orientation.x = jointGlobal.orientation.get_e1();
				j->computedJointData.globalJointPosition.orientation.y = jointGlobal.orientation.get_e2();;
				j->computedJointData.globalJointPosition.orientation.z = jointGlobal.orientation.get_e3();;

				j->computedJointData.globalJointPosition.rotation = jointGlobal.rotation;

				j->computedJointData.globalJointPositionDefined = true;			
			}
		}	
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*
	void ResolveAssemblyConstraints_AddMarkersToMap( 
			cad::CadFactoryAbstract													&in_Factory,
			const std::vector<std::string>											&in_AssemblyComponentIDs,
			std::unordered_map<IntList, std::string, ContainerHash<IntList>>		&in_FeatureIDs_to_ComponentInstanceID_hashtable,
			std::map<std::string, isis::CADComponentData>	&in_out_CADComponentData_map )
																		throw (isis::application_exception)
	{
		log4cpp::Category& logcat = log4cpp::Category::getInstance(LOGCAT_LOGFILEONLY);
		for each ( const std::string &i in in_AssemblyComponentIDs )
		{
			isis::CADComponentData &leafAssembly = in_out_CADComponentData_map[i];
			for (std::vector<isis::ConstraintData>::iterator it = leafAssembly.constraintDef.constraints.begin(); it != leafAssembly.constraintDef.constraints.end(); ++it)
			{
				if (it->constrainedFrom_Paths.size()>1)
				{
					throw isis::application_exception("The constraint in assembly " + i + " resolves into multiple parts.");
				}

				if (it->constrainedFrom_Paths.size()==1)
				{
					// Merge the 2 paths into one
					IntList pathToPart = leafAssembly.componentPaths;
					pathToPart.insert(pathToPart.end(), it->constrainedFrom_Paths.begin()->begin(), it->constrainedFrom_Paths.begin()->end());
					std::string partid = in_FeatureIDs_to_ComponentInstanceID_hashtable[pathToPart];
					if (partid.size()==0)
					{
						throw isis::application_exception("Can't find constrainedfrom part for assembly " + i + ".");
					}
					it->constrainedFrom_InstanceID = partid;
					if (in_out_CADComponentData_map[partid].modelType == PRO_MDL_ASSEMBLY)
					{
						isis_LOG(lg, isis_FILE, isis_WARN) << "Resolved constrainedFrom_Paths points to an assembly: " + i;
					}
				}

				if (it->constrainedTo_Paths.size()>1)
				{
					throw isis::application_exception("The constraint in assembly " + i + " resolves into multiple parts.");
				}

				if (it->constrainedTo_Paths.size()==1)
				{
					// Merge the 2 paths into one
					if (it->constraintPairs[0].constraintFeatures[0].componentInstanceID != i)
					{
						throw isis::application_exception("In a constraint pair, the first is not the component beign constrained - something is wrong with this structure, assembly id: " + i);
					}
					isis::CADComponentData &other = in_out_CADComponentData_map[it->constraintPairs[0].constraintFeatures[1].componentInstanceID];
					IntList pathToPart = other.componentPaths;
					pathToPart.insert(pathToPart.end(), it->constrainedTo_Paths.begin()->begin(), it->constrainedTo_Paths.begin()->end());
					string partid = in_FeatureIDs_to_ComponentInstanceID_hashtable[pathToPart];
					if (partid.size()==0)
					{
						throw isis::application_exception("Internal error: can't find constrainedto part for assembly " + i + ".");
					}
					it->constrainedTo_InstanceID = partid;
					if (in_out_CADComponentData_map[partid].modelType == PRO_MDL_ASSEMBLY)
					{
						isis_LOG(lg, isis_FILE, isis_WARN) << "Resolved constrainedTo_Paths points to an assembly. Assembly: " << i << ", constrainedTo: " << partid;
					}
				}
			}
		}
	}
	*/
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void FindPartsReferencedByFeature(	
						const std::string									&in_TopAssemblyComponentInstanceID, 
						const std::string									&in_ComponentInstanceID,
						const MultiFormatString								&in_FeatureName,
						//ProType											in_FeatureGeometryType,
						e_CADFeatureGeometryType								in_FeatureGeometryType,
						const std::unordered_map<IntList, std::string, ContainerHash<IntList>>		&in_FeatureIDs_to_ComponentInstanceID_hashtable,
						const std::map<string, isis::CADComponentData>		&in_CADComponentData_map,
						std::set<std::string>								&out_ComponentInstanceIDs_of_PartsReferencedByFeature_set)
																			throw (isis::application_exception)
	{

		std::map<std::string, isis::CADComponentData>::const_iterator itr_assembly;
		itr_assembly = in_CADComponentData_map.find(in_TopAssemblyComponentInstanceID);
		
		if ( itr_assembly == in_CADComponentData_map.end())
		{
			std::stringstream errorString;
			errorString << "Function - " << __FUNCTION__ << ", was passed an in_TopAssemblyComponentInstanceID that is not in in_CADComponentData_map. in_TopAssemblyComponentInstanceID:  " << in_TopAssemblyComponentInstanceID;
			throw isis::application_exception(errorString);	
		}			
		
		std::map<std::string, isis::CADComponentData>::const_iterator itr_component;
		itr_component = in_CADComponentData_map.find(in_ComponentInstanceID);
		
		if ( itr_component == in_CADComponentData_map.end())
		{
			std::stringstream errorString;
			errorString << "Function - " << __FUNCTION__ << ", was passed an in_ComponentInstanceID that is not in in_CADComponentData_map. in_ComponentInstanceID:  " << in_ComponentInstanceID;
			throw isis::application_exception(errorString);	
		}

		if ( itr_component->second.modelType == CAD_MDL_PART )
		{
			// If in_ComponentInstanceID is a part, then in_FeatureName is owned by in_ComponentInstanceID
			out_ComponentInstanceIDs_of_PartsReferencedByFeature_set.insert(in_ComponentInstanceID);
			return;
		}

		ProAsmcomppath	comp_path;
		isis::Retrieve_ProAsmcomppath_WithExceptions(	static_cast<ProSolid>(itr_assembly->second.cADModel_hdl), 
														itr_component->second.componentPaths, 
														comp_path );

		//////////////////////////////////////////////////////////
		// Find the address to the model identified by comp_path
		//////////////////////////////////////////////////////////
		ProMdl		 model;					
		isis::isis_ProAsmcomppathMdlGet( &comp_path, &model);


	    //	Find the datum ModelItem
		ProModelitem  model_datum;

		isis::isis_ProModelitemByNameInit_WithDescriptiveErrorMsg (	
												in_ComponentInstanceID, // Added arguments
												itr_component->second.name, 
												ProMdlType_enum(itr_component->second.modelType),   
																	//in_ContraintDef.p_base_model, //base_model, // Original arguments
												model, //base_model, // Original arguments
												//in_FeatureGeometryType, 
												FeatureGeometryType_enum(in_FeatureGeometryType),
												in_FeatureName, 
												&model_datum);  

		//	Allocate the selections
		ProSelection model_select;
	
		ValidatePathAndModelItem_ThrowExceptionIfInvalid(comp_path, model_datum);
		isis::isis_ProSelectionAlloc(&comp_path, &model_datum, &model_select);
		
		DatumRefResolver resolver_added(model_select);
		vector<ProSelection> resolved_list;
		resolver_added.Resolve(resolved_list);
		
		for each ( ProSelection i in resolved_list )
		{
			ProAsmcomppath comppath;
			isis_ProSelectionAsmcomppathGet(i, &comppath);
			list<int> pathaslist;
			isis::ProAsmcomppathToList(pathaslist, comppath);
			list<int> fullist = itr_component->second.componentPaths;
			fullist.insert(fullist.end(), pathaslist.begin(), pathaslist.end());

			std::unordered_map<IntList, std::string, ContainerHash<IntList>>::const_iterator itr; 
			itr = in_FeatureIDs_to_ComponentInstanceID_hashtable.find(fullist);
	
			if ( itr != in_FeatureIDs_to_ComponentInstanceID_hashtable.end() )
			{
				std::map<std::string, isis::CADComponentData>::const_iterator itr_component_feature_ref;
				itr_component_feature_ref = in_CADComponentData_map.find(itr->second);
				if ( itr_component_feature_ref == in_CADComponentData_map.end())
				{
					std::stringstream errorString;
					errorString << "Function - " << __FUNCTION__ << ", a part that was identifed as a referenced feature was not found in in_CADComponentData_map. reference feature ComponentInstanceID:  " << itr->second <<
						". This would indicate an error/bug in in_FeatureIDs_to_ComponentInstanceID_hashtable, which was passed to " << __FUNCTION__;
					throw isis::application_exception(errorString);	
				}

				// We are only interested in parts constrained to parts.  A part constrained
				// to an assembly is not of interest because an assembly is just an abstract
				// concept for organizing parts.  In a physical assembly, parts are constrained
				// to parts.
				//if (in_CADComponentData_map[itr->second].modelType == CAD_MDL_PART )
				//				out_ComponentInstanceIDs_of_PartsReferencedByFeature_set.insert(itr->second);
				if (itr_component_feature_ref->second.modelType == CAD_MDL_PART )
								out_ComponentInstanceIDs_of_PartsReferencedByFeature_set.insert(itr->second);

			}	
			else
			{	
				std::stringstream errorString;
				errorString << "Function - " << __FUNCTION__ << ", could not find a known ComponentInstanceID for in_ProSelection." << std::endl <<
								"One possible cause for this error is that CADAssembly.xml does not have the the following entry:  "  << std::endl <<
							    "<ProcessingInstruction Primary=\"COMPLETE_THE_HIERARCHY_FOR_LEAF_ASSEMBLIES\" Secondary=\"\" />:  "  << std::endl <<
								"   Model Name:            " <<	 itr_component->second.name << std::endl <<
								"   Model Type:            " <<	 isis::ProMdlType_string(itr_component->second.modelType)<<  std::endl <<
								"   Component Instance ID: " <<  in_ComponentInstanceID <<  std::endl <<
								"   FeatureName:           " <<  (std::string)in_FeatureName <<  std::endl <<
								"   Path Feature IDs:     ";
						for each ( int i_fid  in fullist ) errorString << " " << i_fid;
				throw isis::application_exception(errorString);					
			}
		}
	}	


	////////////////////////////////////////////////////////////////////////////////////////////////////////
/***
	void PopulateMap_with_Junctions_and_ConstrainedToInfo_per_CreoAsmFeatureTrees( 
			cad::CadFactoryAbstract													&in_Factory,
			const std::vector<std::string>											&in_AssemblyComponentIDs,
			const std::unordered_map<IntList, std::string, ContainerHash<IntList>>	&in_FeatureIDs_to_ComponentInstanceID_hashtable,
			std::map<std::string, isis::CADComponentData>	&in_out_CADComponentData_map )
																		throw (isis::application_exception)
	{
		

		std::vector<std::string> trimmedListOfComponentIDs;
		FurtherTrimList_Remove_TreatAsOneBodeParts( in_AssemblyComponentIDs,
													in_out_CADComponentData_map,
													trimmedListOfComponentIDs );

		for each ( const std::string &i in trimmedListOfComponentIDs )
		{

			isis_LOG(lg, isis_FILE, isis_INFO) << "************** PopulateMap_with_Junctions_and_ConstrainedToInfo_per_CreoAsmFeatureTrees, ComponentID: " << i;
		
			ProAsmcomppath asmcomppath;

			Retrieve_ProAsmcomppath_WithExceptions( static_cast<ProSolid>(in_out_CADComponentData_map[i].cADModel_hdl),
													in_out_CADComponentData_map[i].componentPaths,
													asmcomppath);

			//isis_ProMdlDisplay( 	in_out_CADComponentData_map[i].modelHandle );

			CreoAssembledFeatureDefinition  assembledFeatureDefinition;

			assembledFeatureDefinition.modelName = in_out_CADComponentData_map[i].name;
			assembledFeatureDefinition.componentInstanceID = i;

			ProElement						ElemTree;


			ProAsmcomp					assembledFeature_temp;			
			//assembledFeature_temp.type =	FeatureGeometryType_enum(in_out_CADComponentData_map[i].assembledFeature.type);
			//assembledFeature_temp.id   =	                         in_out_CADComponentData_map[i].assembledFeature.id;
			//assembledFeature_temp.owner =	                         in_out_CADComponentData_map[i].assembledFeature.owner; 
			assembledFeature_temp = getProAsmcomp( in_out_CADComponentData_map[i].assembledFeature);


			RetrieveCreoElementTreeConstraints(	//in_out_CADComponentData_map[i].assembledFeature,
												assembledFeature_temp,
												asmcomppath,
												assembledFeatureDefinition,
												ElemTree);


			//isis_LOG(lg, isis_FILE, isis_INFO) << assembledFeatureDefinition;
			//std::cout << std::endl << std::endl << assembledFeatureDefinition;

			if ( assembledFeatureDefinition.constraintDefinitions.size() > 0 )
			{		
				int setIndex = 0;
				int numSets =  assembledFeatureDefinition.constraintSetDefinitions.size();

				int setIndex_Start;
				int setIndex_End;    // this is 1 past the end, like STL containers
				if ( numSets == 0 )
				{
					setIndex_Start = -1;  // in the Creo Constraint, -1 means no sets
					setIndex_End = 0;	 
				}
				else
				{
					setIndex_Start = 0;
					setIndex_End = numSets;
				}

		

				for ( int setIndex = setIndex_Start; setIndex < setIndex_End; ++setIndex )
				{
					if ( (PmConnectionAttr)assembledFeatureDefinition.constraintSetDefinitions[setIndex].component_set_misc_attr == PRO_ASM_DISABLE_COMP_SET)
					{
						isis_LOG(lg, isis_FILE, isis_INFO) << "Constraint set disabled.  SetIndex: " << setIndex << "  Component Instance ID: " <<  i;				
						continue;
					}

					isis_LOG(lg, isis_FILE, isis_INFO) << "Adding junction information for the following constraint feature: ";
					ConstraintData constraintData_PerFeatureTree;

					// If a particular constraint was disabled, the following function would not include the junction information for 
					// the disabled constraint in the map.
					PopulateMap_with_JunctionInformation_SingleJunction( 
												in_Factory,
												assembledFeatureDefinition,
												setIndex,
												constraintData_PerFeatureTree.computedJointData.junction_withoutguide,
												in_out_CADComponentData_map );

					constraintData_PerFeatureTree.computedJointData.jointType_withoutguide =  GetCADJointType(constraintData_PerFeatureTree.computedJointData.junction_withoutguide.joint_pair.first.type);
					constraintData_PerFeatureTree.computedJointData.junctiondDefined_withoutGuide = true;
					constraintData_PerFeatureTree.computedJointData.coordinateSystem_ComponentInstanceID = assembledFeatureDefinition.componentInstanceID;

					try
					{
						// WARNING - Computing the offSetFeatureIDPath_list is temporary fix and will only work for the case
						//			 where a CyPhy leaf assembly has no sub-assemblies.  Later a better approach will
						//           be needed to support a leaf assembly having multiple levels (i.e. multiple sub-assemblies)
						//			 and a sub-assembly has .prts constrained to .prts outside the sub-assembly.
						std::string parentComponentInstanceID = in_out_CADComponentData_map[i].parentComponentID;
						std::list<int>  offSetFeatureIDPath_list = in_out_CADComponentData_map[parentComponentInstanceID].componentPaths;
						RetrieveReferencedComponentInstanceIDs (	offSetFeatureIDPath_list,
																	assembledFeatureDefinition,
																	setIndex,
																	in_FeatureIDs_to_ComponentInstanceID_hashtable,
																	constraintData_PerFeatureTree.constrainedTo_ComponentInstanceIDs_InferredFromLeafAssemblySubordinates);								
					}
					catch ( isis::application_exception &ex)
					{
						std::stringstream errorString;
						errorString <<
							"Function: " << __FUNCTION__ << ", failed to retrieve referenced ComponentInstanceIDs:"  << std::endl <<
							"   Model Name:            " <<	 in_out_CADComponentData_map[i].name << std::endl <<
							"   Model Type:            " <<	 isis::ProMdlType_string(in_out_CADComponentData_map[i].modelType)<<  std::endl <<
							"   Component Instance ID: " <<  i <<  std::endl <<
							"   Exception Message: " << ex.what();
						throw isis::application_exception(errorString);	
					}


					in_out_CADComponentData_map[i].constraintDef.constraints.push_back(constraintData_PerFeatureTree);		
				}
			}
			else  // ELSE if ( assembledFeatureDefinition.constraintDefinitions.size() > 0 )
			{
				//isis_LOG(lg, isis_FILE, isis_INFO) << "\nNOT adding junction information (no constraint references found) for the following constraint feature: ";
				//isis_LOG(lg, isis_FILE, isis_INFO) << assembledFeatureDefinition;

				std::stringstream errorString;
				errorString <<
					"Function: " << __FUNCTION__ << ", No constraint references found for:"  << std::endl <<
					"   Model Name:            " <<	 in_out_CADComponentData_map[i].name << std::endl <<
					"   Model Type:            " <<	 isis::CADMdlType_string(in_out_CADComponentData_map[i].modelType)<<  std::endl <<
					"   Component Instance ID: " << i << std::endl <<
					"   Assembled Feature Definition: " << std::endl <<
					assembledFeatureDefinition;

				throw isis::application_exception(errorString);	
			}

			// Need to set					

			// Only free this after using assembledFeatureDefinition. assembledFeatureDefinition is
			// invalid after ProFeatureElemtreeFree
			//ProFeatureElemtreeFree(&in_out_CADComponentData_map[i].assembledFeature, ElemTree);	
			ProFeatureElemtreeFree(&assembledFeature_temp, ElemTree);	
		}

	}
	**/


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

} // end namespace isis
