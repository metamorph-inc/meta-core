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
#include "CreoModelMetaData.h"
#include "cc_AssemblyOptions.h"

using namespace std;

namespace isis
{

/////////////////////////////////////////////////////////////////////////////////////////
void UpdateAllSubassemblyComponentsToIncludeParentAssemblyPath( 
						const std::string in_ComponentID,
						std::map<string, isis::CADComponentData> &in_out_CADComponentData_map,
						int assembled_feat_id )
{
	//for ( AssemblyType::CADComponent_type::CADComponent_const_iterator i(in_Begin); i != in_End; ++i )
	//{
	for ( std::list<string>::const_iterator i(in_out_CADComponentData_map[in_ComponentID].children.begin());
		i != in_out_CADComponentData_map[in_ComponentID].children.end();
		++i )
	{
		in_out_CADComponentData_map[*i].componentPaths.push_front(assembled_feat_id);
		UpdateAllSubassemblyComponentsToIncludeParentAssemblyPath( *i,
																	in_out_CADComponentData_map,
																	assembled_feat_id );
	}
}
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
// If all the members in A are in B
//		return true;
// else
//		return false;
//
//  Special Cases:
//  -------------
//	If A is empty and B is not empty
//		return true;
//
//	If B is empty and A is not empty
//		return false;
//
//	If B is empty and A is empty
//		return true;
bool AinB(  const std::set<std::string> &in_A,  const std::set<std::string> &in_B )
{
	std::set<std::string>::const_iterator itr;

	std::set<std::string>  Intersection;

	std::insert_iterator<std::set<std::string> > ii(Intersection, Intersection.begin()); 

	std::set_intersection ( in_A.begin(), in_A.end(), in_B.begin(), in_B.end(), ii );

	//std::cout << std::endl << "Begin Intersection"; 
	//for ( std::set<std::string>::const_iterator itr = Intersection.begin(); itr !=  Intersection.end(); ++itr )
	//{
	//	std::cout << std::endl << *itr; 
	//}
	//std::cout << std::endl << "End Intersection"; 

	if ( in_A.size() == Intersection.size() )
		return true;
	else 
		return false;
}

/////////////////////////////////////////////////////////////////////////////////////
void	AddSubordinateComponentIDsToSet( const std::string &in_ComponentID, 
										 std::map<std::string, CADComponentData> &in_ComponentAssembledInfo,
										 std::set<std::string> &out_Existing )
{
	if ( in_ComponentAssembledInfo[in_ComponentID].children.size() > 0 )
	{
		for ( std::list<std::string>::const_iterator i = in_ComponentAssembledInfo[in_ComponentID].children.begin();
													i != in_ComponentAssembledInfo[in_ComponentID].children.end();
													++i )
		{
			//std::clog << std::endl << "   AddSubordinateComponentIDsToSet, ComponentID: " << *i; 
			out_Existing.insert(*i);
			AddSubordinateComponentIDsToSet( *i, in_ComponentAssembledInfo, out_Existing );

		}													
	}
}

/////////////////////////////////////////////////////////////////////////////////////
void  LogSortingSet(	const std::string	&in_Indent,
						const std::string	&in_Title,
						const std::string	&in_ParentName,
						const std::string	&in_ParentComponentID,
						std::map<std::string, CADComponentData> &in_ComponentAssembledInfo,
						const std::set<std::string> &in_Components )
{
	

	isis_LOG(lg, isis_FILE, isis_INFO) << in_Indent << in_Title;

	isis_LOG(lg, isis_FILE, isis_INFO) << in_Indent << "   Parent: " << in_ParentComponentID << "   " << in_ParentName;

		for ( std::set<std::string>::const_iterator i = in_Components.begin(); i != in_Components.end(); ++i )
		{
			isis_LOG(lg, isis_FILE, isis_INFO) << in_Indent << "      " <<
				in_ComponentAssembledInfo[*i].componentID  <<   "   " <<
				in_ComponentAssembledInfo[*i].name;
		}
}
////////////////////////////////////////////////////////////////////////////////////
void  LogSortingList(	const std::string	&in_Indent,
						const std::string	&in_Title,
						const std::string	&in_ParentName,
						const std::string	&in_ParentComponentID,
						std::map<std::string, CADComponentData> &in_ComponentAssembledInfo,
						const std::list<std::string>  &in_Components )
{
	

	isis_LOG(lg, isis_FILE, isis_INFO) << in_Indent << in_Title;

	isis_LOG(lg, isis_FILE, isis_INFO) << in_Indent << "   Parent: " << in_ParentComponentID << "   " << in_ParentName;

		for ( std::list<std::string>::const_iterator i = in_Components.begin(); i != in_Components.end(); ++i )
		{
			isis_LOG(lg, isis_FILE, isis_INFO) << in_Indent << "      " << in_ComponentAssembledInfo[*i].name  << 
				"  " << in_ComponentAssembledInfo[*i].componentID;
			isis_LOG(lg, isis_FILE, isis_INFO) << in_Indent << "                   Depends On: ";
			for ( std::set<std::string>::const_iterator j = in_ComponentAssembledInfo[*i].dependsOn.begin(); 
				   j != in_ComponentAssembledInfo[*i].dependsOn.end(); ++j )
			{
				isis_LOG(lg, isis_FILE, isis_INFO) << in_Indent << "                       "  <<
				in_ComponentAssembledInfo[*j].name  << "    " <<
				in_ComponentAssembledInfo[*j].componentID;

			}

		}
}
/////////////////////////////////////////////////////////////////////////////////////
void LogSortingInputs(	const std::string	&in_ParentName,
								const std::string  &in_ParentComponentID,
								const std::list<std::string> &in_Components, 
								std::map<std::string, CADComponentData> &in_ComponentAssembledInfo )
{
	

	LogSortingList(	"   ", 
					"BEGIN ##########   SortCADComponents, List of components to be sorted   ###########",
					in_ParentName,
					in_ParentComponentID,
					in_ComponentAssembledInfo,
					in_Components );
	isis_LOG(lg, isis_FILE, isis_INFO) << "END ############   SortCADComponents, List of components to be sorted  ###########";
}

/////////////////////////////////////////////////////////////////////////////////////
void LogStateWhenSortingFailed(	const std::string	&in_ParentName,
								const std::string  &in_ParentComponentID,
								const std::list<std::string> &in_Components, 
								const std::list<std::string> &out_Components, 
								const std::set<std::string>  &in_Existing,
								std::map<std::string, CADComponentData> &in_ComponentAssembledInfo )
{
	

	// This is a fatal error.  It means the list cannot be sorted.
	// Throw exception
	
	isis_LOG(lg, isis_FILE, isis_INFO) << "ERROR: Could not sort the component list";
	//clog << std::endl << std::endl << "BEGIN ##########   SORTING ERROR   ###########";
	//LogSortingList(	"   ", 
	//				"BEGIN SortCADComponents, To-be-added Components",
	//				in_ParentName,
	//				in_ParentComponentID,
	//				in_ComponentAssembledInfo,
	//				in_Components );
	//clog << std::endl << "END SortCADComponents, To-be-added Components";
	isis_LOG(lg, isis_FILE, isis_INFO) << "";
	LogSortingList(	"", 
					"BEGIN SortCADComponents, Components that were sorted when the sort failed",
					in_ParentName,
					in_ParentComponentID,
					in_ComponentAssembledInfo,
					out_Components );
	isis_LOG(lg, isis_FILE, isis_INFO) << "END  SortCADComponents, Components that were sorted when the sort failed";

	/////////////////
	// Log Not used
	/////////////////
	std::list<std::string> Components_temp = in_Components;
	for ( std::list<std::string>::const_iterator itr_2 =  out_Components.begin(); itr_2 != out_Components.end(); ++itr_2 )
	{
		Components_temp.remove( *itr_2 );
	}
	
	isis_LOG(lg, isis_FILE, isis_INFO) << "";
	isis_LOG(lg, isis_FILE, isis_INFO) << "BEGIN SortCADComponents, Not-sorted-yet list when the sort failed"; 
	for ( std::list<std::string>::const_iterator itr_2 =  Components_temp.begin(); itr_2 != Components_temp.end(); ++itr_2 )
	{
		isis_LOG(lg, isis_FILE, isis_INFO) <<  "     "  <<
			in_ComponentAssembledInfo[*itr_2].componentID  << "    " <<
			in_ComponentAssembledInfo[*itr_2].name;     
	}
	isis_LOG(lg, isis_FILE, isis_INFO) << "END SortCADComponents, Not-sorted-yet list when the sort failed"; 

	isis_LOG(lg, isis_FILE, isis_INFO) << "";
	LogSortingSet(	"", 
					"BEGIN Components Higher-up in Sort Tree (i.e. existing components from a sort perspective)",
					in_ParentName,
					in_ParentComponentID,
					in_ComponentAssembledInfo,
					in_Existing );
	isis_LOG(lg, isis_FILE, isis_INFO) << "END Higher-up in Sort Tree (i.e. existing components from a sort perspective)";

	isis_LOG(lg, isis_FILE, isis_INFO) << "END ###########   SORTING ERROR   ###########";

}
/////////////////////////////////////////////////////////////////////////////////////
void SearchForAComponentsThatDependOnMultipleAddedComponents(	
						const std::list<std::string>			&in_CandidateComponents,
						const std::set<std::string>				&in_ExistingComponents,
						std::map<std::string, CADComponentData> &in_ComponentAssembledInfo,
						bool									&out_ComponentFound,
						std::string								&out_FoundComponentID )
{
	out_ComponentFound = false;
	int maxDependsOnCountFound = 0;

	// First look for a component with a guide
	for ( std::list<std::string>::const_iterator itr = in_CandidateComponents.begin(); itr != in_CandidateComponents.end(); ++itr )
	{
		int dependsOnCount = in_ComponentAssembledInfo[*itr].dependsOn.size();

		if ( dependsOnCount > 1 &&  dependsOnCount > maxDependsOnCountFound  )
		{			
			std::set<std::string> Existing_temp = in_ExistingComponents;
			AddSubordinateComponentIDsToSet( *itr, in_ComponentAssembledInfo, Existing_temp );

			if ( AinB( in_ComponentAssembledInfo[*itr].dependsOn, Existing_temp ) )
			{
				maxDependsOnCountFound = dependsOnCount;
				// found suitbale candidate 
				out_FoundComponentID = *itr;
				out_ComponentFound = true;
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////
void SearchForAComponentsThatOnlyDependsOnAddedComponents(	
						const std::list<std::string>			&in_CandidateComponents,
						const std::set<std::string>				&in_ExistingComponents,
						bool									in_OnlySearchComponentsWithAGuideConstraint,
						std::map<std::string, CADComponentData> &in_ComponentAssembledInfo,
						bool									&out_ComponentFound,
						std::string								&out_FoundComponentID )
{

	out_ComponentFound = false;

	// First look for a component with a guide
	for ( std::list<std::string>::const_iterator itr = in_CandidateComponents.begin(); itr != in_CandidateComponents.end(); ++itr )
	{
		std::set<std::string> Existing_temp = in_ExistingComponents;

		AddSubordinateComponentIDsToSet( *itr, in_ComponentAssembledInfo, Existing_temp );

		bool guideFlag;

		if ( in_OnlySearchComponentsWithAGuideConstraint ) 
			guideFlag = in_ComponentAssembledInfo[*itr].constraintDef.hasAGuideConstraint();
		else
			guideFlag = true;


		if ( guideFlag &&  AinB( in_ComponentAssembledInfo[*itr].dependsOn, Existing_temp ) )
		{			
			// found suitbale candidate 
			out_FoundComponentID = *itr;
			out_ComponentFound = true;
			break;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////
void SortCADComponents ( const std::string	&in_ParentName,
						 const std::string  &in_ParentComponentID,
						 const std::list<std::string> &in_Components,
						 std::map<std::string, CADComponentData> &in_ComponentAssembledInfo,
						 std::list<std::string> &out_Components )						 
																throw (isis::application_exception)
{
	
	std::list<std::string> Components = in_Components;
	std::set<std::string> Existing;

	Existing.insert(in_ParentComponentID);

	int NumComponents = Components.size();

	//LogSortingInputs(in_ParentName, in_ParentComponentID, in_Components, in_ComponentAssembledInfo ); 

	for ( int i = 0; i < NumComponents; ++i )
	{
		bool FoundComponent = false;
		std::string FoundComponentID;
												 
		// First look for components that depend on multiple existing components
		SearchForAComponentsThatDependOnMultipleAddedComponents(	Components,
																	Existing,
																	in_ComponentAssembledInfo,
																	FoundComponent,
																	FoundComponentID );


		// Second look for a component with a guide
		if ( !FoundComponent )
		{
			SearchForAComponentsThatOnlyDependsOnAddedComponents(	Components,
																	Existing,
																	true,  // Only search for components that have a guide constraint
																	in_ComponentAssembledInfo,
																	FoundComponent,
																	FoundComponentID );
		}

		/*
		for ( std::list<std::string>::const_iterator itr = Components.begin(); itr != Components.end(); ++itr )
		{
			std::set<std::string> Existing_temp = Existing;

			AddSubordinateComponentIDsToSet( *itr, in_ComponentAssembledInfo, Existing_temp );

			if ( in_ComponentAssembledInfo[*itr].constraintDef.hasAGuideconstraint() && 
				 AinB( in_ComponentAssembledInfo[*itr].dependsOn, Existing_temp ) )
			{			
				// found suitbale candidate
				out_Components.push_back(*itr);
				Existing.insert(*itr);
			
				// If this is an assembly, must add all the assemblies subordinates to Existing set.
				// This is necessary because the children of the added assembly can be referenced for 
				// constraint purposes.
				//std::clog << std::endl << "BEGIN !!!!!!!!!!!!!! Adding AddSubordinateComponentIDsToSet, Component Instance ID: " << *itr; 
				AddSubordinateComponentIDsToSet( *itr, in_ComponentAssembledInfo, Existing );
				//std::clog << std::endl << "END !!!!!!!!!!!!!! Adding AddSubordinateComponentIDsToSet, Component Instance ID: " << *itr; 

				FoundComponentID = *itr;
				FoundComponent = true;
				break;
			}
		}
		*/
		// Third look for components with/without a guide
		if ( !FoundComponent )
		{
			SearchForAComponentsThatOnlyDependsOnAddedComponents(	Components,
																	Existing,
																	false,  // Only search for components that have a guide constraint
																	in_ComponentAssembledInfo,
																	FoundComponent,
																	FoundComponentID );
		}
		/*
		if ( !FoundComponent )  // Did not find a guide component, look for a non-guide component
		{  
			for ( std::list<std::string>::const_iterator itr = Components.begin(); itr != Components.end(); ++itr )
			{
				std::set<std::string> Existing_temp = Existing;

				AddSubordinateComponentIDsToSet( *itr, in_ComponentAssembledInfo, Existing_temp );

				if ( AinB( in_ComponentAssembledInfo[*itr].dependsOn, Existing_temp ) )
				{			
					// found suitbale candidate
					out_Components.push_back(*itr);
					Existing.insert(*itr);
			
					// If this is an assembly, must add all the assemblies subordinates to Existing set.
					// This is necessary because the children of the added assembly can be referenced for 
					// constraint purposes.
					//std::clog << std::endl << "BEGIN !!!!!!!!!!!!!! Adding AddSubordinateComponentIDsToSet, Component Instance ID: " << *itr; 
					AddSubordinateComponentIDsToSet( *itr, in_ComponentAssembledInfo, Existing );
					//std::clog << std::endl << "END !!!!!!!!!!!!!! Adding AddSubordinateComponentIDsToSet, Component Instance ID: " << *itr; 

					FoundComponentID = *itr;
					FoundComponent = true;
					break;
				}
			}
		}
		*/

		if (!FoundComponent)
		{
			LogStateWhenSortingFailed( in_ParentName, in_ParentComponentID, in_Components, out_Components, Existing, in_ComponentAssembledInfo );
			throw isis::application_exception("C04001", "exception : Sorting error.  See log file for additional information.");  
		}

		Existing.insert(FoundComponentID);
		out_Components.push_back(FoundComponentID);

		// If this is an assembly, must add all the assemblies subordinates to Existing set.
		// This is necessary because the children of the added assembly can be referenced for 
		// constraint purposes.
		//std::clog << std::endl << "BEGIN !!!!!!!!!!!!!! Adding AddSubordinateComponentIDsToSet, Component Instance ID: " << FoundComponentID; 
		AddSubordinateComponentIDsToSet( FoundComponentID, in_ComponentAssembledInfo, Existing );
		//std::clog << std::endl << "END !!!!!!!!!!!!!! Adding AddSubordinateComponentIDsToSet, Component Instance ID: " << FoundComponentID;
		Components.remove(FoundComponentID);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
enum e_InclusionExclusionInstructions
{
	INCLUDE_ALL_COMPONENTS,
	INCLUDE_ALL_COMPONENTS_EXCEPT_SIZE_TO_FIT,
	INCLUDE_ONLY_SIZE_TO_FIT
};


/////////////////////////////////////////////////////////////////////////////////////////

void BuildFiltereListOfComponents(	const std::string							&in_AssemblyComponentID, 
									std::map<string, isis::CADComponentData>	&in_CADComponentData_map, 
									e_InclusionExclusionInstructions			in_Instructions, 
									std::list<std::string>						&out_FilteredComponents )
{
	for ( std::list<string>::const_iterator i (in_CADComponentData_map[in_AssemblyComponentID].children.begin());
		  i != in_CADComponentData_map[in_AssemblyComponentID].children.end();
		  ++i )
	{
		if ( in_Instructions == INCLUDE_ALL_COMPONENTS_EXCEPT_SIZE_TO_FIT )
		{
			if ( in_CADComponentData_map[*i].specialInstruction == CAD_SPECIAL_INSTRUCTION_SIZE_TO_FIT ) continue;
		}
		else
		{
			if ( in_Instructions == INCLUDE_ONLY_SIZE_TO_FIT )
			{
				if ( in_CADComponentData_map[*i].specialInstruction != CAD_SPECIAL_INSTRUCTION_SIZE_TO_FIT ) continue;
			}
		}

		out_FilteredComponents.push_back(*i);
	}
}

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
					cad::CadFactoryAbstract						&in_Factory,
					ProMdl										in_p_asm,
					const std::string							&in_ParentName,
					const std::list<std::string>				&in_Components,
					std::map<string, isis::CADComponentData>	&in_CADComponentData_map,
					int											&in_out_addedToAssemblyOrdinal)
														throw (isis::application_exception)
{
	
	

	CreoModelMetaData constraintData("constraintdata.xml");

	for ( std::list<std::string>::const_iterator itr = in_Components.begin(); itr != in_Components.end(); ++itr )
	{

			// Note - the following "new" is not deleted.  This is a memory leak by design. The contents of this 
			// new are needed up unitl the program exits.  Will let the OS clean this up.
			// Also, note that assembled_feat_handle must go on the heap; otherwise, it would get deleted when the stack unwinds.
			ProAsmcomp assembled_feat_handle;
			ProMdl     *p_model = new ProMdl;

			std::string ModelNameWithSuffix = AmalgamateModelNameWithSuffix ( 
												in_CADComponentData_map[*itr].name, 
												in_CADComponentData_map[*itr].modelType);

			//wchar_t  PartName[ISIS_CHAR_BUFFER_LENGTH];
			//ProStringToWstring(PartName, (char *)(const char *)in_CADComponentData_map[*itr].name );

			isis::isis_ProMdlRetrieve_WithDescriptiveErrorMsg(
				*itr, in_CADComponentData_map[*itr].name, in_CADComponentData_map[*itr].geometryRepresentation,   // Added arguments
				in_CADComponentData_map[*itr].name,ProMdlType_enum(in_CADComponentData_map[*itr].modelType), p_model);	// Original Arguments
				//PartName,in_CADComponentData_map[*itr].modelType, p_model);	// Original Arguments

			ProMatrix identity_matrix = {{ 1.0, 0.0, 0.0, 0.0 }, 
							{0.0, 1.0, 0.0, 0.0}, 
							{0.0, 0.0, 1.0, 0.0}, 
							{0.0, 0.0, 0.0, 1.0}};

			if (constraintData.GetInitialPos(*itr) != 0)
			{
				memcpy (in_CADComponentData_map[*itr].initialPosition, constraintData.GetInitialPos(*itr), sizeof(double)*16);
			} else {
				memcpy (in_CADComponentData_map[*itr].initialPosition, identity_matrix, sizeof(double)*16);
			}

			// fixfixfix add try catch here to support the case where the first part added into the assembly
			// does not have the default datums. fixfixfix qqqqqq
			isis::AddComponentToAssembly( (ProAssembly)in_p_asm, (ProSolid)*p_model, &assembled_feat_handle, in_CADComponentData_map[*itr].initialPosition);

			// in_CADComponentData_map[itr->ComponentID()].ComponentID =	itr->ComponentID();
			// in_CADComponentData_map[itr->ComponentID()].Name =			itr->Name;

			in_CADComponentData_map[*itr].cADModel_ptr_ptr = p_model;
			in_CADComponentData_map[*itr].cADModel_hdl =	(ProSolid)*p_model;
			
			//in_CADComponentData_map[*itr].assembledFeature = assembled_feat_handle;
			//in_CADComponentData_map[*itr].assembledFeature.type = CADFeatureGeometryType_enum(assembled_feat_handle.type);
			//in_CADComponentData_map[*itr].assembledFeature.id = assembled_feat_handle.id;
			//in_CADComponentData_map[*itr].assembledFeature.owner = assembled_feat_handle.owner;
			in_CADComponentData_map[*itr].assembledFeature = getCADAssembledFeature(assembled_feat_handle);

			in_CADComponentData_map[*itr].componentPaths.push_back((assembled_feat_handle).id);
			in_CADComponentData_map[*itr].addedToAssemblyOrdinal =	in_out_addedToAssemblyOrdinal++;

			isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) <<  "   Assembly: " << in_ParentName << "   Added Model: "  << ModelNameWithSuffix; 
			isis_LOG(lg, isis_FILE, isis_INFO) << "       Added Model  componentInstanceID:   " <<  in_CADComponentData_map[*itr].componentID;
			isis_LOG(lg, isis_FILE, isis_INFO) << "       Added Model  parentComponentID:     " <<  in_CADComponentData_map[*itr].parentComponentID;
			isis_LOG(lg, isis_FILE, isis_INFO) << "       Added Model  cADModel_ptr_ptr:      " <<  (const void*)in_CADComponentData_map[*itr].cADModel_ptr_ptr; 
			isis_LOG(lg, isis_FILE, isis_INFO) << "       Added Model  cADModel_hdl:          "  << (const void*)in_CADComponentData_map[*itr].cADModel_hdl;
			isis_LOG(lg, isis_FILE, isis_INFO) << "       Added Model  assembledFeature.id:   " <<  in_CADComponentData_map[*itr].assembledFeature.id;
			isis_LOG(lg, isis_FILE, isis_INFO) << "       Added Model  assembledFeature.type: " <<  in_CADComponentData_map[*itr].assembledFeature.type;
			//isis_LOG(lg, isis_FILE, isis_INFO) << "       Added Model  specialInstruction:    " <<  isis::SpecialInstruction_string( in_CADComponentData_map[*itr].specialInstruction );
			isis_LOG(lg, isis_FILE, isis_INFO) << "       Added Model  specialInstruction:    " <<  in_CADComponentData_map[*itr].specialInstruction;
			isis_LOG(lg, isis_FILE, isis_INFO) << "       Added Model  representation:        " <<  in_CADComponentData_map[*itr].geometryRepresentation;
		
			//////////////////////////////////////////////
			// Set DDP parameters in the assembly feature
			/////////////////////////////////////////////

			if ( in_CADComponentData_map[*itr].componentID.size() > 79 )
			{
				/*std::stringstream errorString;
				errorString <<
				"exception : The ComponentID string is too long to store as a paramter in the Creo model.  Component Instance ID: " <<  in_CADComponentData_map[*itr].componentID;
				throw isis::application_exception("C04002",errorString.str().c_str());*/
				isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) << "";
				isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) << "WARNING: Couldn't set DDP_COMPONENT_INSTANCE_ID. STEP export might not work on this model.";
				
			} else {	

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

				MultiFormatString value_multiString(in_CADComponentData_map[*itr].componentID, PRO_LINE_SIZE - 2);

				value.type = PRO_PARAM_STRING;
				//ProStringToWstring(value.value.s_val, (char *) in_CADComponentData_map[*itr].componentID.c_str() );
				wcscpy_s( value.value.s_val, PRO_LINE_SIZE, value_multiString);
			
				err = ProParameterInit(&assembled_feat_handle, (wchar_t *)(const wchar_t *)name, &param);

				//clog << std::endl << "&&&&&& Add_Subassemblies_and_Parts, ProParameterInit&&&&&&&, assembled_feat_handle: " << assembled_feat_handle;

				if( err == PRO_TK_E_NOT_FOUND)
					 isis::isis_ProParameterCreate(&assembled_feat_handle, name, &value, &param);
				else
					 isis::isis_ProParameterValueSet(&param, &value);

			}

	} // END for ( std::list<std::string>::const_iterator itr = in_Components.begin(); itr != in_Components.end(); ++itr )
								 
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////

// Size-to-Fit components are constrained to Non-Size-to-Fit components (i.e. the constraint points outward), 
// but no other components are constrained to the Size-to-Fit components (no inward pointing constraints).   
// Also, a Size-to-Fit would NOT be constrained to another Size-to-Fit component.  Therefore, if a Size-to-Fit 
// component is constrained to Non-Size-to-Fit components that have already been added to the assembly structure, 
// then the Size-to-Fit components can be added to the current assembly.
void For_SizeToFit_ComponentsWithDependentsPresentAtThisLevel_AddAndConstrain(
									cad::CadFactoryAbstract				&	in_Factory,
									const std::string							&in_AssemblyComponentID,
									bool										in_AllowUnconstrainedModels,
									std::map<string, isis::CADComponentData>	&in_CADComponentData_map,
									const std::list<std::string>				&ComponentsAtCurrentLevel,
									std::list<std::string>						&in_out_SIZE_TO_FIT_Components,
									int											&in_out_addedToAssemblyOrdinal)
																			throw ( isis::application_exception )
{	
	
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

			
			Add_Subassemblies_and_Parts( in_Factory,
										in_CADComponentData_map[in_AssemblyComponentID].cADModel_hdl, 
										 in_CADComponentData_map[in_AssemblyComponentID].name,  
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
			isis::ApplyModelConstraints( in_Factory,
											reinterpret_cast<ProSolid*>(&in_CADComponentData_map[in_AssemblyComponentID].cADModel_hdl), //ProSolid	 in_assembly_model,
											in_AssemblyComponentID,
											Single_SIZE_TO_FIT_Component,
											in_AllowUnconstrainedModels,
											in_CADComponentData_map, 
											false, 
											false);
			//isis::ApplyModelConstraints( (ProSolid*)&in_p_asm, //ProSolid	 in_assembly_model,
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

		//	"Pro/TOOLKIT® User’s Guide" Section "Regenerating a Solid":
		//	PRO_REGEN_NO_RESOLVE_MODE—Specifies the
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
void AddSortOrderToMap(	const std::list<std::string>				&in_SortedComponents, 
						std::map<string, isis::CADComponentData>	&in_out_CADComponentData_map) 
{
	int partOrdinal_temp = 0;
	int partAssemblyOrdinal_temp = 0;

	for each ( std::string i in in_SortedComponents )
	{
		partAssemblyOrdinal_temp++;
		if ( in_out_CADComponentData_map[i].modelType == PRO_MDL_PART ) partOrdinal_temp++;

		in_out_CADComponentData_map[i].partAssemblyOrdinal = partAssemblyOrdinal_temp;
		in_out_CADComponentData_map[i].partOrdinal = partOrdinal_temp;
	}

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AssembleCADComponent( 	
							cad::CadFactoryAbstract						&in_Factory,
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

				AssembleCADComponent( in_Factory,
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
	
	void* handle = in_Factory.get_assembler().get_assembly_component
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
	Add_Subassemblies_and_Parts( in_Factory,
								p_asm, 
								 in_out_CADComponentData_map[in_AssemblyComponentID].name, 
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


	bool fail = isis::ApplyModelConstraints( in_Factory,
									(ProSolid*)&p_asm, //ProSolid	 in_assembly_model,
									in_AssemblyComponentID,
									SortedComponents,
									in_AllowUnconstrainedModels,
									in_out_CADComponentData_map,
									constraintdata,
									true);
		

	if (!fail)
	{
		if (AssemblyOptions::GetInstance().ApplyParamsAfter)
			ApplyParametricParameters(SortedComponents, in_out_CADComponentData_map, out_ErrorList);
		////////////////////////////////////////////////////////////////////////////
		// Determine if SIZE_TO_FIT Componenets can be added and set constrained
		///////////////////////////////////////////////////////////////////////////

		For_SizeToFit_ComponentsWithDependentsPresentAtThisLevel_AddAndConstrain(
										in_Factory,
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
void BuildAssembly( cad::CadFactoryAbstract				&in_Factory,
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
	AssembleCADComponent(	in_Factory,
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