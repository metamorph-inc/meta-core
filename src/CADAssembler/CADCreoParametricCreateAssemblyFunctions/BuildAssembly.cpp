#include <BuildAssembly.h>
#include <isis_ptc_toolkit_functions.h>
#include <isis_application_exception.h>
#include <isis_include_ptc_headers.h>
#include <AssembleComponents.h>
#include <CreoStringToEnumConversions.h>
#include <ApplyModelConstraints.h>
#include <cc_CommonUtilities.h>
#include <DiagnosticUtilities.h>
#include <cc_CommonConstants.h>
#include <ISISConstants.h>
// #include <FiniteElementAnalysis.h>
#include <SetCADModelParameters.h>
#include <CommonFunctions.h>
#include <cc_CommonStructures.h>
#include <ToolKitPassThroughFunctions.h>
#include <stdio.h>
#include <stdlib.h>
#include <set>
#include <iterator>
#include <algorithm>
#include <sstream>
#include "cc_LoggerBoost.h"
#include "ProAssembly.h"
//#include "CreoModelMetaData.h"
#include "cc_AssemblyOptions.h"
#include <cc_BuildAssembly.h>
#include <cc_ApplyModelConstraints.h>

using namespace std;

namespace isis
{

/////////////////////////////////////////////////////////////////////////////////////////


// This is setting simplified reps in Creo
// Typically would set to Featured_Rep or Defeatured_rep
// If the particular rep does not exist in the creo model then the active rep would not be changed
/*

This function was started but not completed.  It will not work properly
void SetCreoModelRepresentation( isis::CADComponentData &in_CADComponentData )
															throw (isis::application_exception)
{

	// No action needed if the simplified rep name is Null
	if ( in_CADComponentData.geometryRepresentation.size() == 0 ) return;

	ProError status;
	ProSimprep simp_rep;

	// Retrieve the simplified rep construct (i.e. ProSimprep)
	
	status = ProSimprepInit(	(wchar_t*)(const wchar_t*)in_CADComponentData.geometryRepresentation, 
									-1, 
									(ProSolid)in_CADComponentData.modelHandle,
									&simp_rep);
		
	if ( status != PRO_TK_E_FOUND && status != PRO_TK_E_NOT_FOUND )
	{
			// Error occured, erroneous inputs 
			std::stringstream errorString;
			errorString 
				<<	"Function - SetCreoModelRepresentation, ProSimprepInit received erroneous inputs: " << std::endl <<
					"   ComponentInstanceID: " <<  in_CADComponentData.componentID << std::endl <<
					"   Model Name:          " <<  in_CADComponentData.name << std::endl <<
					"   Model Handle:         " << in_CADComponentData.modelHandle << std::endl <<
					"   Representation:      " <<  in_CADComponentData.geometryRepresentation << std::endl;  
			throw isis::application_exception(errorString);
	}

	if ( status != PRO_TK_E_NOT_FOUND ) return;  // Do nothing if simplified rep not found.

	// Assembly
	if ( in_CADComponentData.modelType == PRO_MDL_ASSEMBLY )
		isis_ProSimprepActivate((ProSolid)in_CADComponentData.modelHandle, &simp_rep);
	else

}
*/
/////////////////////////////////////////////////////////////////////////////////////////
void Add_Subassemblies_and_Parts( 
					ProMdl										in_p_asm,
					const std::string							&in_ParentName,
					const std::list<std::string>					&in_Components,
					std::map<string, isis::CADComponentData>		&in_out_CADComponentData_map,
					int											&in_out_addedToAssemblyOrdinal)
														throw (isis::application_exception)
{
	
	isis::cad::CadFactoryAbstract_global *cadFactoryAbstract_global_ptr = isis::cad::CadFactoryAbstract_global::instance();
	isis::cad::CadFactoryAbstract::ptr	cAD_Factory_ptr = cadFactoryAbstract_global_ptr->getCadFactoryAbstract_ptr();								
	isis::cad::IModelHandling&        modelHandling = cAD_Factory_ptr->getModelHandling();	


	for ( std::list<std::string>::const_iterator itr = in_Components.begin(); itr != in_Components.end(); ++itr )
	{


			// Note - the following "new" is not deleted.  This is a memory leak by design. The contents of this 
			// new are needed up unitl the program exits.  Will let the OS clean this up.
			// Also, note that assembled_feat_handle must go on the heap; otherwise, it would get deleted when the stack unwinds.
			ProAsmcomp assembled_feat_handle;
			ProMdl     *p_model = new ProMdl;

			std::string ModelNameWithSuffix = AmalgamateModelNameWithSuffix ( 
												in_out_CADComponentData_map[*itr].name, 
												in_out_CADComponentData_map[*itr].modelType);


			modelHandling.cADModelRetrieve(	*itr, 
											in_out_CADComponentData_map[*itr].name, 
											in_out_CADComponentData_map[*itr].modelType,
											in_out_CADComponentData_map[*itr].geometryRepresentation,
											p_model);

			// NOTE - The constructor for CADComponentData::CADComponentData() sets in_out_CADComponentData_map[*itr].initialPosition to 
			//			the identiy matrix.  Should not make any change here because another function my be added that would
			//			change the position prior to calling this funciton


			isis::AddComponentToAssembly( (ProAssembly)in_p_asm, (ProSolid)*p_model, &assembled_feat_handle, in_out_CADComponentData_map[*itr].initialPosition);

			// in_out_CADComponentData_map[itr->ComponentID()].ComponentID =	itr->ComponentID();
			// in_out_CADComponentData_map[itr->ComponentID()].Name =			itr->Name;

			in_out_CADComponentData_map[*itr].cADModel_ptr_ptr = p_model;
			in_out_CADComponentData_map[*itr].cADModel_hdl =	(ProSolid)*p_model;
			
			//in_out_CADComponentData_map[*itr].assembledFeature = assembled_feat_handle;
			//in_out_CADComponentData_map[*itr].assembledFeature.type = CADFeatureGeometryType_enum(assembled_feat_handle.type);
			//in_out_CADComponentData_map[*itr].assembledFeature.id = assembled_feat_handle.id;
			//in_out_CADComponentData_map[*itr].assembledFeature.owner = assembled_feat_handle.owner;
			in_out_CADComponentData_map[*itr].assembledFeature = getCADAssembledFeature(assembled_feat_handle);

			in_out_CADComponentData_map[*itr].componentPaths.push_back((assembled_feat_handle).id);
			in_out_CADComponentData_map[*itr].addedToAssemblyOrdinal =	in_out_addedToAssemblyOrdinal++;

			isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) <<  "   Assembly: " << in_ParentName << "   Added Model: "  << ModelNameWithSuffix; 
			isis_LOG(lg, isis_FILE, isis_INFO) << "       Added Model  componentInstanceID:   " <<  in_out_CADComponentData_map[*itr].componentID;
			isis_LOG(lg, isis_FILE, isis_INFO) << "       Added Model  parentComponentID:     " <<  in_out_CADComponentData_map[*itr].parentComponentID;
			isis_LOG(lg, isis_FILE, isis_INFO) << "       Added Model  cADModel_ptr_ptr:      " <<  (const void*)in_out_CADComponentData_map[*itr].cADModel_ptr_ptr; 
			isis_LOG(lg, isis_FILE, isis_INFO) << "       Added Model  *cADModel_ptr_ptr:     " <<  (const void*)*(in_out_CADComponentData_map[*itr].cADModel_ptr_ptr);
			isis_LOG(lg, isis_FILE, isis_INFO) << "       Added Model  cADModel_hdl:          " <<  (const void*)in_out_CADComponentData_map[*itr].cADModel_hdl;
			isis_LOG(lg, isis_FILE, isis_INFO) << "       Added Model  assembledFeature.id:   " <<  in_out_CADComponentData_map[*itr].assembledFeature.id;
			isis_LOG(lg, isis_FILE, isis_INFO) << "       Added Model  assembledFeature.type: " <<  in_out_CADComponentData_map[*itr].assembledFeature.type;
			//isis_LOG(lg, isis_FILE, isis_INFO) << "       Added Model  specialInstruction:    " <<  isis::SpecialInstruction_string( in_out_CADComponentData_map[*itr].specialInstruction );
			isis_LOG(lg, isis_FILE, isis_INFO) << "       Added Model  specialInstruction:    " <<  in_out_CADComponentData_map[*itr].specialInstruction;
			isis_LOG(lg, isis_FILE, isis_INFO) << "       Added Model  representation:        " <<  in_out_CADComponentData_map[*itr].geometryRepresentation;
		
			//////////////////////////////////////////////
			// Set DDP parameters in the assembly feature
			/////////////////////////////////////////////

			if ( in_out_CADComponentData_map[*itr].componentID.size() > 79 )
			{
				/*std::stringstream errorString;
				errorString <<
				"exception : The ComponentID string is too long to store as a paramter in the Creo model.  Component Instance ID: " <<  in_out_CADComponentData_map[*itr].componentID;
				throw isis::application_exception("C04002",errorString.str().c_str());*/
				isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) << "";
				isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) << "WARNING: Couldn't set DDP_COMPONENT_INSTANCE_ID. STEP export might not work on this model.";
				
			} 
			else 
			{	

				ProError      err;
				//ProName       name;
				ProParameter  param;
				ProParamvalue value;

				// DDP_COMPONENT_INSTANCE_ID, for now only output this one.  Later, after schema change, add the following 
				// DDP_COMPONENT_ID
				// DDP_COMPONENT_VERSION
				// DDP_COMPONENT_REVISION
				// DDP_DESIGN_ID
				//ProStringToWstring(name, "DDP_COMPONENT_INSTANCE_ID" );
				MultiFormatString name("DDP_COMPONENT_INSTANCE_ID", PRO_NAME_SIZE - 1);

				MultiFormatString value_multiString(in_out_CADComponentData_map[*itr].componentID, PRO_LINE_SIZE - 2);

				value.type = PRO_PARAM_STRING;
				//ProStringToWstring(value.value.s_val, (char *) in_out_CADComponentData_map[*itr].componentID.c_str() );
				wcscpy_s( value.value.s_val, PRO_LINE_SIZE, value_multiString);
			
				err = ProParameterInit(&assembled_feat_handle, (wchar_t *)(const wchar_t *)name, &param);

				//clog << std::endl << "&&&&&& Add_Subassemblies_and_Parts, ProParameterInit&&&&&&&, assembled_feat_handle: " << assembled_feat_handle;

				if( err == PRO_TK_E_NOT_FOUND)
					 isis::isis_ProParameterCreate(&assembled_feat_handle, name, &value, &param);
				else
					 isis::isis_ProParameterValueWithUnitsSet(&param, &value, nullptr);

			}

	} // END for ( std::list<std::string>::const_iterator itr = in_Components.begin(); itr != in_Components.end(); ++itr )
								 
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////

// Size-to-Fit components are constrained to Non-Size-to-Fit components (i.e. the constraint points outward), 
// but no other components are constrained to the Size-to-Fit components (no inward pointing constraints).   
// Also, a Size-to-Fit would NOT be constrained to another Size-to-Fit component.  Therefore, if a Size-to-Fit 
// component is constrained to Non-Size-to-Fit components that have already been added to the assembly structure, 
// then the Size-to-Fit components can be added to the current assembly.

void For_SizeToFit_ComponentsWithDependentsPresentAtThisLevel_AddAndConstraint(
									//cad::CadFactoryAbstract				&	in_Factory,
									const std::string							&in_AssemblyComponentID,
									bool										in_AllowUnconstrainedModels,
									std::map<string, isis::CADComponentData>	&in_CADComponentData_map,
									const std::list<std::string>				&ComponentsAtCurrentLevel,
									std::list<std::string>						&in_out_SIZE_TO_FIT_Components,
									int											&in_out_addedToAssemblyOrdinal)
																			throw ( isis::application_exception )
{	
	
	isis::cad::CadFactoryAbstract_global *cadFactoryAbstract_global_ptr = isis::cad::CadFactoryAbstract_global::instance();
	isis::cad::CadFactoryAbstract::ptr	cAD_Factory_ptr = cadFactoryAbstract_global_ptr->getCadFactoryAbstract_ptr();
	isis::cad::IModelOperations&         modelOperations = cAD_Factory_ptr->getModelOperations();	

	////////////////////////////////////////////////////
	// First get all existing components at the level
	//////////////////////////////////////////////////
	std::set<std::string> ExistingComponentsSet;
	
	// Add non Size-to-Fit components to existing component set
	for ( std::list<std::string>::const_iterator t = ComponentsAtCurrentLevel.begin(); t != ComponentsAtCurrentLevel.end(); ++t )
	{
		ExistingComponentsSet.insert( *t );
		AddSubordinateComponentIDsToSet( *t,in_CADComponentData_map, ExistingComponentsSet );

	}

	std::list<std::string> Added_in_out_SIZE_TO_FIT_Components;

	for ( std::list<std::string>::const_iterator t = in_out_SIZE_TO_FIT_Components.begin(); t != in_out_SIZE_TO_FIT_Components.end(); ++t )
	{
		if ( AinB( in_CADComponentData_map[*t].dependsOn, ExistingComponentsSet ) )
		{
			std::list<std::string> Single_SIZE_TO_FIT_Component;
			
			// The component path would have been incorrectly maintined up to this point.  
			// Since the component was just added to the assembly. must reset the path.
			in_CADComponentData_map[*t].componentPaths.erase(
										in_CADComponentData_map[*t].componentPaths.begin(),
										in_CADComponentData_map[*t].componentPaths.end());

			Single_SIZE_TO_FIT_Component.push_back(*t);

			
			//Add_Subassemblies_and_Parts( //in_Factory,
			//							in_CADComponentData_map[in_AssemblyComponentID].cADModel_hdl, 
			//							 in_CADComponentData_map[in_AssemblyComponentID].name,  
			//							 Single_SIZE_TO_FIT_Component, 
			//							 in_CADComponentData_map,
			//							 in_out_addedToAssemblyOrdinal);


			modelOperations.addModelsToAssembly( in_AssemblyComponentID, 
												Single_SIZE_TO_FIT_Component, 
												in_CADComponentData_map,
												in_out_addedToAssemblyOrdinal);

			
			//	At this point, the xml assembly tree and in_CADComponentData_map probably do not match the model 
			//  Creo assembly tree.  This is because a SIZE_TO_FIT probably was probably moved higher in the 
			//	assembly tree.  This must be corrected because the tree is used to obtain metrics and if the tree 
			//	is incorrect then the routine that computes the translation between an assembly and its children will fail.

			std::string oldParentID = in_CADComponentData_map[*t].parentComponentID;

			if ( in_AssemblyComponentID != oldParentID )
			{
				// Remove Child from old assembly
				in_CADComponentData_map[oldParentID].children.remove(*t);
				// Add Child to new/correct assembly
				in_CADComponentData_map[in_AssemblyComponentID].children.push_back(*t);
				// Reset parent Id
				in_CADComponentData_map[*t].parentComponentID = in_AssemblyComponentID;
			}

			// No need to update the path to its sub-assemblies/parts.  A SIZE_TO_FIT component will always be a leaf assembly.

			// Can constrain this SIZE_TO_FIT component
			isis::ApplyListedModelsConstraints( //in_Factory,
											//reinterpret_cast<ProSolid*>(&in_CADComponentData_map[in_AssemblyComponentID].cADModel_hdl), //ProSolid	 in_assembly_model,
											in_AssemblyComponentID,
											Single_SIZE_TO_FIT_Component,
											in_AllowUnconstrainedModels,
											in_CADComponentData_map, 
											//false, 
											false);
			//isis::ApplyListedModelsConstraints( (ProSolid*)&in_p_asm, //ProSolid	 in_assembly_model,
			//								Single_SIZE_TO_FIT_Component,
			//								in_CADComponentData_map);

			Added_in_out_SIZE_TO_FIT_Components.push_back(*t);
			
		}
	} // END for ( std::list<std::string>::const_iterator t = in_out_in_out_SIZE_TO_FIT_Components.begin(); t != in_out_in_out_SIZE_TO_FIT_Components.end(); ++t )

	// Remove the added SIZE_TO_FIT components from Added_in_out_SIZE_TO_FIT_Components
	for ( std::list<std::string>::const_iterator t = Added_in_out_SIZE_TO_FIT_Components.begin(); t != Added_in_out_SIZE_TO_FIT_Components.end(); ++t )
	{
		in_out_SIZE_TO_FIT_Components.remove(*t);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void	RegenerateModel( ProSolid in_p_asm,
						 const std::string in_ParentName,
						 const std::string in_ParentComponentID,
						 bool  &out_RegenerationSucceeded,
						 bool  in_PresentDetailedErrorMessage)
											throw (isis::application_exception)
{
	bool ProSolidRegenerate_ThrewException = false;
	
	
	
	try 
	{
		isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "";
		isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "   Regenerating:     " << in_ParentName << "  " << in_ParentComponentID;

		//	"Pro/TOOLKIT User's Guide" Section "Regenerating a Solid":
		//	PRO_REGEN_NO_RESOLVE_MODE-Specifies the
		//	No-Resolve mode introduced in Pro/ENGINEER Wildfire 5.0.
		//	This is the default mode in Pro/ENGINEER Wildfire 5.0. In this
		//	mode, if a model and feature regeneration fails, failed features
		//	and children of failed features are created and regeneration of
		//	other features continues.
		//

		isis::isis_ProSolidRegenerate (in_p_asm, PRO_REGEN_NO_RESOLVE_MODE);

	
		//if (status != 
	}
	catch (...)
	{
		ProSolidRegenerate_ThrewException = true;
		out_RegenerationSucceeded = false;
	}



	ProSolidRegenerationStatus RegenStatus;
	//PRO_SOLID_FAILED_REGENERATION

	if ( !ProSolidRegenerate_ThrewException )
	{
		isis::isis_ProSolidRegenerationstatusGet(in_p_asm, &RegenStatus);
	}

	if ( ProSolidRegenerate_ThrewException || RegenStatus != PRO_SOLID_REGENERATED )
	{
		out_RegenerationSucceeded = false;
		// Treat this as a warning.  Continue building the rest of the assemblies.  The assembly with
		// the regeneration error should open; however, the components that failed to regenerate will show up
		// as red items in the assembly tree.  They will not be positioned properly.

		string err_str;

		if ( in_PresentDetailedErrorMessage )
		{
			err_str = "WARNING - Assembly failed to regenerate. Error caused by Model Name: " + 
				in_ParentName + " ComponentID: " + in_ParentComponentID + "." +
				"\nNote - Assembly/constraint process: " +
				"\n         a) add all the parts/sub-assemblies to the assembly" +
				"\n         b) apply constraints to a part/sub-assembly" +
				"\n         c) repeat step b until all constraints have been applied" +
				"\n         d) regnerate the assembly"  +
				"\nBecause of this process the regeneration could have failed because any one" +
				"\nof the added parts/sub-assemblies would not regenerate, or because the" +
				"\nconstraints used to position a part/sub-assembly were erroneous." +
				"\nThis warning message could appear multiple times.  This is because" +
				"\nonce a failure occurs, all subsequent regenerations will fail." +
				"\nOpen the parent assembly using Creo, and the anomalous" +
				"\nparts/sub-assemblies will appear in red in the assembly tree.\n";
		}
		else
		{
			err_str = "   Regeneration FAILED.";
		}

		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << err_str; 
	}

	if ( out_RegenerationSucceeded )
	{
		isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "   Regeneration succeeded.";
	}

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AssembleCADComponent( 	
							//cad::CadFactoryAbstract					&cAD_Factory_ptr,
							const std::string							&in_AssemblyComponentID,
							const std::string							&in_WORKING_DIR,
							bool										in_SaveAssembly,
							bool										in_AllowUnconstrainedModels,
							//isis::ComponentData_struct					&in_ParentComponentData,
							std::map<string, isis::CADComponentData>	&in_out_CADComponentData_map,
							std::list<std::string>						&in_out_SIZE_TO_FIT_Components,
							int											&in_out_addedToAssemblyOrdinal,
							bool										&out_RegenerationSucceeded,
							std::vector<isis::CADCreateAssemblyError>   &out_ErrorList)
																		throw (isis::application_exception)
{
		
	
	isis::cad::CadFactoryAbstract_global *cadFactoryAbstract_global_ptr = isis::cad::CadFactoryAbstract_global::instance();
	isis::cad::CadFactoryAbstract::ptr					cAD_Factory_ptr = cadFactoryAbstract_global_ptr->getCadFactoryAbstract_ptr();	
	isis::cad::IModelOperations&         modelOperations = cAD_Factory_ptr->getModelOperations();	


	// *************************************
	// Build Sub-assemblies
	// **************************************
//	for ( AssemblyType::CADComponent_type::CADComponent_const_iterator i(in_Begin); i != in_End; ++i )
	for ( std::list<string>::const_iterator i (in_out_CADComponentData_map[in_AssemblyComponentID].children.begin());
		  i != in_out_CADComponentData_map[in_AssemblyComponentID].children.end();
		  ++i )
	{
			//ProMdlType pro_model_type = isis::ProMdlType_enum( i->Type());
			//if ( pro_model_type == PRO_MDL_ASSEMBLY  && i->CADComponent().size() > 0) // Note - an existing assmbly would have no children. 
			//if ( in_out_CADComponentData_map[*i].modelType == PRO_MDL_ASSEMBLY  && in_out_CADComponentData_map[*i].children.size() > 0) // Note - an existing assembly would have no children. 
			if ( in_out_CADComponentData_map[*i].modelType == CAD_MDL_ASSEMBLY  && in_out_CADComponentData_map[*i].children.size() > 0) // Note - an existing assembly would have no children. 
			{
				//isis::C8omponentData_struct ParentComponentData_temp( i->ComponentID(), i->Name() );

				AssembleCADComponent( //cAD_Factory_ptr,
									  *i,
									  in_WORKING_DIR,
									  //ParentComponentData_temp,
									  in_SaveAssembly,
									  in_AllowUnconstrainedModels,
									  in_out_CADComponentData_map,
									  in_out_SIZE_TO_FIT_Components, 
									  in_out_addedToAssemblyOrdinal,
									  out_RegenerationSucceeded,
									  out_ErrorList );			
			}
	}

	//wchar_t TempString_wide[ISIS_CHAR_BUFFER_LENGTH];
	//string temp_string;	

	// *********************
	// 1) Delete current assembly if it exists
	// 2) Copy template assembly to new assembly name
	// 3) Retrieve copied assembly.
	// *********************
	
	//wchar_t  AssemblyName[PRO_FAMILY_NAME_SIZE];
	//wchar_t  AssemblyName[ISIS_CHAR_BUFFER_LENGTH];
	
	void* handle = cAD_Factory_ptr->get_assembler().get_assembly_component
		(in_WORKING_DIR, in_AssemblyComponentID, in_out_CADComponentData_map);

	ProMdl p_asm = *(reinterpret_cast<ProMdl*>(handle));

	//if (p_asm)

	isis_LOG(lg, isis_FILE, isis_INFO) << "";
	isis_LOG(lg, isis_FILE, isis_INFO) << "######################### Begin Assembly " << in_out_CADComponentData_map[in_AssemblyComponentID].name << " ########################";
	isis_LOG(lg, isis_FILE, isis_INFO) << "";
	isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "Created Assembly: " << in_out_CADComponentData_map[in_AssemblyComponentID].name;

	in_out_CADComponentData_map[in_AssemblyComponentID].cADModel_hdl = (ProSolid)p_asm;
	in_out_CADComponentData_map[in_AssemblyComponentID].addedToAssemblyOrdinal = in_out_addedToAssemblyOrdinal++;

	// *************************************
	// Add all detail parts / sub-assemblies 
	// **************************************

	// Sort the order of assembly.  This is necessary because a constraint cannot reference
	// a model lower down in the tree.

	std::list<std::string> UnsortedComponents;
	std::list<std::string> SIZE_TO_FIT_Components;

	
	BuildFiltereListOfComponents(	in_AssemblyComponentID, 
									in_out_CADComponentData_map, 
									INCLUDE_ALL_COMPONENTS_EXCEPT_SIZE_TO_FIT, 
									UnsortedComponents);

	BuildFiltereListOfComponents(	in_AssemblyComponentID, 
									in_out_CADComponentData_map, 
									INCLUDE_ONLY_SIZE_TO_FIT, 
									SIZE_TO_FIT_Components);

	for ( std::list<std::string>::const_iterator t = SIZE_TO_FIT_Components.begin(); 
		  t != SIZE_TO_FIT_Components.end(); ++t )
	{
		in_out_SIZE_TO_FIT_Components.push_back( *t );
	}

	std::list<std::string> SortedComponents;

	SortCADComponents(	in_out_CADComponentData_map[in_AssemblyComponentID].name, 
						in_out_CADComponentData_map[in_AssemblyComponentID].componentID,
						UnsortedComponents,
						in_out_CADComponentData_map,
						SortedComponents );

	isis_LOG(lg, isis_FILE, isis_INFO) << "";
	isis_LOG(lg, isis_FILE, isis_INFO)  << "BEGIN ----------   Unsorted List   ------------";
	LogSortingList(	"   ", 
					"UnsortedComponents, Components",
					in_out_CADComponentData_map[in_AssemblyComponentID].name,
					in_out_CADComponentData_map[in_AssemblyComponentID].componentID,
					in_out_CADComponentData_map,
					UnsortedComponents );
	isis_LOG(lg, isis_FILE, isis_INFO) << "END ------------   Unsorted List   ------------";

	isis_LOG(lg, isis_FILE, isis_INFO) << "";
	isis_LOG(lg, isis_FILE, isis_INFO)  << "BEGIN ----------   Sorted List   --------------";
	LogSortingList(	"", 
					"SortedComponents, Components",
					in_out_CADComponentData_map[in_AssemblyComponentID].name,
					in_out_CADComponentData_map[in_AssemblyComponentID].componentID,
					in_out_CADComponentData_map,
					SortedComponents );
	isis_LOG(lg, isis_FILE, isis_INFO) << "END ------------   Sorted List   --------------";

	AddSortOrderToMap(SortedComponents, in_out_CADComponentData_map);

	bool constraintdata = false;

	if (boost::filesystem::exists("constraintdata.xml"))
	{
		constraintdata = true;
	}

	////////////////////////////////////////
	// Add sub-assemblies and detail parts
	////////////////////////////////////////
	//Add_Subassemblies_and_Parts( //*cAD_Factory_ptr,
	//							 p_asm, 
	//							 in_out_CADComponentData_map[in_AssemblyComponentID].name, 
	//							 SortedComponents, 
	//							 in_out_CADComponentData_map,
	//							 in_out_addedToAssemblyOrdinal);

	modelOperations.addModelsToAssembly(		in_AssemblyComponentID, 
											SortedComponents, 
											in_out_CADComponentData_map,
											in_out_addedToAssemblyOrdinal);


	//stream_CADComponentData_map( in_out_CADComponentData_map, clog );

	/////////////////////////////////////////////////////////////////////
	// For each added assembly update the path to its sub-assemblies/parts 
	// to include feature id of the added assembly. 
	//////////////////////////////////////////////////////////////////////
	// Note - the following also updates the SIZE_TO_FIT components.  That
	// path information will be corrected when the SIZE_TO_FIT components 
	// are added.  We cannot skip the SIZE_TO_FIT components because once
	// they are added, the path information must be maintained.
	//for ( AssemblyType::CADComponent_type::CADComponent_const_iterator t(in_Begin); t != in_End; ++t )
	//{
	for ( std::list<string>::const_iterator t (in_out_CADComponentData_map[in_AssemblyComponentID].children.begin());
	  t != in_out_CADComponentData_map[in_AssemblyComponentID].children.end();
	  ++t )
	{
		//ProMdlType pro_model_type = isis::ProMdlType_enum( in_out_CADComponentData_map[t->ComponentID()].type_string);
		//if (  in_out_CADComponentData_map[*t].modelType == PRO_MDL_ASSEMBLY  ) 
		if (  in_out_CADComponentData_map[*t].modelType == CAD_MDL_ASSEMBLY  ) 
		{
			UpdateAllSubassemblyComponentsToIncludeParentAssemblyPath( 
											*t,
											in_out_CADComponentData_map,
											in_out_CADComponentData_map[*t].assembledFeature.id );
		}
	}

	///////////////////////////////////
	// Set Parametric Values
	///////////////////////////////////
	if (!AssemblyOptions::GetInstance().ApplyParamsAfter)
		ApplyParametricParameters(SortedComponents, in_out_CADComponentData_map, out_ErrorList);

	///////////////////////////////////
	// Log tree contents at this level
	///////////////////////////////////
	//clog << std::endl << std::endl << "************ Begin Sub-Tree (Name ComponentID) ***************";
	//stream_IndentedAssemblyTree( in_Begin, in_End, clog );
	//clog << std::endl << "************ End Sub-Tree  (Name ComponentID) *****************";

	//clog << std::endl << "************ Begin Sub-Tree (Detailed Info) ***************";
	//stream_AssemblyTree( in_Begin, in_End, clog );
	//clog << std::endl << "************ End Sub-Tree (Detailed Info) ***************";

	//////////////////////////////////////////////////
	// Log in_out_CADComponentData_map contents
	//////////////////////////////////////////////////
	isis_LOG(lg, isis_FILE, isis_INFO) <<  "\n************** Begin map CADComponent in BuildAssembly Function **************"; 
	std::stringstream str;
	stream_CADComponentData_map( in_out_CADComponentData_map, str );
	isis_LOG(lg, isis_FILE, isis_INFO) << str.str();
	isis_LOG(lg, isis_FILE, isis_INFO) << "\n************** End map CADComponent in BuildAssembly Function ***************";


	bool fail = isis::ApplyListedModelsConstraints( //*cAD_Factory_ptr,
									//(ProSolid*)&p_asm, //ProSolid	 in_assembly_model,
									in_AssemblyComponentID,
									SortedComponents,
									in_AllowUnconstrainedModels,
									in_out_CADComponentData_map,
									//constraintdata,
									true);
		


	if (!fail)
	{
		if (AssemblyOptions::GetInstance().ApplyParamsAfter)
			ApplyParametricParameters(SortedComponents, in_out_CADComponentData_map, out_ErrorList);
		////////////////////////////////////////////////////////////////////////////
		// Determine if SIZE_TO_FIT Componenets can be added and set constrained
		///////////////////////////////////////////////////////////////////////////

		For_SizeToFit_ComponentsWithDependentsPresentAtThisLevel_AddAndConstraint(
										//*cAD_Factory_ptr,
										in_AssemblyComponentID,
										in_AllowUnconstrainedModels,
										in_out_CADComponentData_map,
										SortedComponents,
										in_out_SIZE_TO_FIT_Components,
										in_out_addedToAssemblyOrdinal);

		/////////////////////
		// Regenerate Model
		/////////////////////
		// When using Creo interactively, the first regenerate will fail and the second, in some cases, will
		// succeed.  Therefore, if the first regenerate fails, try regnerate a second time. 
	
		RegenerateModel(	(ProSolid)p_asm,
							in_out_CADComponentData_map[in_AssemblyComponentID].name,
							in_out_CADComponentData_map[in_AssemblyComponentID].componentID,
							out_RegenerationSucceeded);

		if ( !out_RegenerationSucceeded )  // try to regenerate a second time
		{
			out_RegenerationSucceeded = true;
			RegenerateModel(	(ProSolid)p_asm,
								in_out_CADComponentData_map[in_AssemblyComponentID].name,
								in_out_CADComponentData_map[in_AssemblyComponentID].componentID,
								out_RegenerationSucceeded);
		}

		if ( !out_RegenerationSucceeded )  // try to regenerate a third time
		{
			out_RegenerationSucceeded = true;
			RegenerateModel(	(ProSolid)p_asm,
								in_out_CADComponentData_map[in_AssemblyComponentID].name,
								in_out_CADComponentData_map[in_AssemblyComponentID].componentID,
								out_RegenerationSucceeded);
		}

		if ( !out_RegenerationSucceeded )  // try to regenerate a fourth time
		{
			out_RegenerationSucceeded = true;
			RegenerateModel(	(ProSolid)p_asm,
								in_out_CADComponentData_map[in_AssemblyComponentID].name,
								in_out_CADComponentData_map[in_AssemblyComponentID].componentID,
								out_RegenerationSucceeded);
		}

		if ( !out_RegenerationSucceeded )  // try to regenerate a fifth time
		{
			out_RegenerationSucceeded = true;
			RegenerateModel(	(ProSolid)p_asm,
								in_out_CADComponentData_map[in_AssemblyComponentID].name,
								in_out_CADComponentData_map[in_AssemblyComponentID].componentID,
								out_RegenerationSucceeded);
		}

		if ( !out_RegenerationSucceeded )  // try to regenerate a sixth time
		{
			out_RegenerationSucceeded = true;
			RegenerateModel(	(ProSolid)p_asm,
								in_out_CADComponentData_map[in_AssemblyComponentID].name,
								in_out_CADComponentData_map[in_AssemblyComponentID].componentID,
								out_RegenerationSucceeded, true);
		}

		if ( !out_RegenerationSucceeded )
		{
			// Try regeneration each sub-component separately to identify the non-regeneration part/sub-assembly

			isis_LOG(lg, isis_FILE, isis_INFO) << "";
			isis_LOG(lg, isis_FILE, isis_INFO)  << "Regenerating parts/sub-assemblies. This will help indicate which part/sub-assembly would not generate."; 
			bool local_RegenerationSucceeded;
			for ( std::list<string>::const_iterator i_regen (in_out_CADComponentData_map[in_AssemblyComponentID].children.begin());
			  i_regen != in_out_CADComponentData_map[in_AssemblyComponentID].children.end();
			  ++i_regen )
			{
				local_RegenerationSucceeded = true;
				RegenerateModel( static_cast<ProSolid>(in_out_CADComponentData_map[*i_regen].cADModel_hdl),
								 in_out_CADComponentData_map[*i_regen].name,
								 in_out_CADComponentData_map[*i_regen].componentID,
								 local_RegenerationSucceeded, false);

			}
		}
	}
	///////////////////////////////////
	// Output Step Files
	///////////////////////////////////
	//std::string createAnalysisDir = "if not exist STEP mkdir STEP";

	/*
	int Arg_2 = 1;

	std::cout << std::endl << "Creating STEP file";

	ProName		optionName;
	ProPath		optionValue;

	wcscpy( optionName, L"step_export_format");
	wcscpy( optionValue, L"ap203_e2");
	//wcscpy( optionValue, L"ap214_is");
	isis_ProConfigoptSet(optionName, optionValue  );
	
	wcscpy( optionName, L"intf3d_out_datums_by_default");
	wcscpy( optionValue, L"yes");
	isis_ProConfigoptSet(optionName, optionValue  );

	wcscpy( optionName, L"step_export_ap214_asm_def_mode");
	wcscpy( optionValue, L"separate_parts_only");
	isis_ProConfigoptSet(optionName, optionValue  );

	// Also should set step_out_suppl_geom, set_out_material, 

	isis_ProOutputFileWrite(p_asm,
							AssemblyName,
							PRO_STEP_FILE,
							NULL,
							&Arg_2,
							NULL,
							NULL);
    */
	/////////////////////////
	// Save the assembly
	/////////////////////////

	if ( in_SaveAssembly ) isis::isis_ProMdlSave(p_asm);

	isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "";
	isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO)  << "Saved Assembly:   " <<  (const std::string&)in_out_CADComponentData_map[in_AssemblyComponentID].name + "  " + (const std::string&)in_out_CADComponentData_map[in_AssemblyComponentID].componentID;

	isis_LOG(lg, isis_FILE, isis_INFO) << "";
	isis_LOG(lg, isis_FILE, isis_INFO)  << "########################## End Assembly " << in_out_CADComponentData_map[in_AssemblyComponentID].name << " #########################";

	// Delete the version 1 of the assembly file.  It was just a temporary file that was copied from the 
	// template file.
	isis::isis_DeleteFile(in_WORKING_DIR + "\\" + (const std::string&)in_out_CADComponentData_map[in_AssemblyComponentID].name + ".asm.1" );

}

//////////////////////////////////////////////////////////////////////////////////////////////////////
void BuildAssembly( //cad::CadFactoryAbstract				&in_Factory,
					const std::string					&in_AssemblyComponentID, 
					const std::string					&in_WORKING_DIR,
					bool								in_SaveAssembly,
					std::map<string, isis::CADComponentData>			&in_out_CADComponentData_map,
					bool								&out_RegenerationSucceeded,
					std::vector<isis::CADCreateAssemblyError>			&out_ErrorList,
					bool								in_AllowUnconstrainedModels )
																	throw (isis::application_exception)
{
	

	int addedToAssemblyOrdinal =  0;

	//clog << endl;
	//clog << endl << "Component Name: "  << in_out_CADComponentData_map[in_AssemblyComponentID].name;
	//clog << endl<<	"Component Instance ID:   "  << in_out_CADComponentData_map[in_AssemblyComponentID].componentID;
	//clog << endl << "Component Type: "  << ProMdlType_string( in_out_CADComponentData_map[in_AssemblyComponentID].modelType );
	//clog << endl;

	isis_LOG(lg, isis_FILE, isis_INFO) << "";
	isis_LOG(lg, isis_FILE, isis_INFO)  << "*************************** Begin Assembly Creation **************************";

	std::list<std::string>  SIZE_TO_FIT_Components; 

	out_RegenerationSucceeded = true;
	AssembleCADComponent(	//in_Factory,
							in_AssemblyComponentID,
							in_WORKING_DIR,
							in_SaveAssembly,
							in_AllowUnconstrainedModels,
							in_out_CADComponentData_map,
							SIZE_TO_FIT_Components,
							addedToAssemblyOrdinal,
							out_RegenerationSucceeded,
							out_ErrorList );



	// At this point all SIZE_TO_FIT Components should have been assembled.  If not, throw an exception
	if ( SIZE_TO_FIT_Components.size() != 0 )
	{
		string err_str = "exception : All assemblies were built; however, some Size-to-Fit components were not added. " +
						std::string(" This condition would occur if the Size-to-Fit components were constrained to ComponentIDs") + 
						 " that were not present in the XML file. The unassembled Size-to-Fit components are: ";

		std::string comman_or_blank;
		for ( std::list<string>::const_iterator i ( SIZE_TO_FIT_Components.begin());
			  i !=  SIZE_TO_FIT_Components.end();
			  ++i )
		{
			err_str += comman_or_blank + in_out_CADComponentData_map[*i].componentID + " " + in_out_CADComponentData_map[*i].componentID;
			comman_or_blank = ", ";
		}
		err_str += ".";										
		throw isis::application_exception("C04003",err_str.c_str());  
	}

	isis_LOG(lg, isis_FILE, isis_INFO) << "";
	isis_LOG(lg, isis_FILE, isis_INFO)  << "************************** End Assembly Creation *************************";


}

//////////////////////////////////////////////////////////////////////////////////////////////////////

	

} // end namespace isis