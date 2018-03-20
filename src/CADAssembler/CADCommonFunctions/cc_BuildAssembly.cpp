#include "cc_BuildAssembly.h"
#include "cc_CADFactoryAbstract.h"


namespace isis 
{

///////////////////////////////////////////////////////////////////////////////////////////////////
void UpdateAllSubassemblyComponentsToIncludeParentAssemblyPath( 
						const std::string in_ComponentID,
						std::map<std::string, isis::CADComponentData> &in_out_CADComponentData_map,
						int assembled_feat_id )
{
	//for ( AssemblyType::CADComponent_type::CADComponent_const_iterator i(in_Begin); i != in_End; ++i )
	//{
	for ( std::list<std::string>::const_iterator i(in_out_CADComponentData_map[in_ComponentID].children.begin());
		i != in_out_CADComponentData_map[in_ComponentID].children.end();
		++i )
	{
		in_out_CADComponentData_map[*i].componentPaths.push_front(assembled_feat_id);
		UpdateAllSubassemblyComponentsToIncludeParentAssemblyPath( *i,
																	in_out_CADComponentData_map,
																	assembled_feat_id );
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////////////////////////

void	 AddSubordinateComponentIDsToSet( const std::string &in_ComponentID, 
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
///////////////////////////////////////////////////////////////////////////////////////////////////

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
///////////////////////////////////////////////////////////////////////////////////////////////////

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
///////////////////////////////////////////////////////////////////////////////////////////////////

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
///////////////////////////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////////////////////////
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


///////////////////////////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////////////////////////
void BuildFiltereListOfComponents(	const std::string								&in_AssemblyComponentID, 
									std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map, 
									e_InclusionExclusionInstructions					in_Instructions, 
									std::list<std::string>							&out_FilteredComponents )
{
	for ( std::list<std::string>::const_iterator i (in_CADComponentData_map[in_AssemblyComponentID].children.begin());
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
///////////////////////////////////////////////////////////////////////////////////////////////////
void AddSortOrderToMap(	const std::list<std::string>						&in_SortedComponents, 
						std::map<std::string, isis::CADComponentData>	&in_out_CADComponentData_map) 
{
	int partOrdinal_temp = 0;
	int partAssemblyOrdinal_temp = 0;

	for each ( std::string i in in_SortedComponents )
	{
		partAssemblyOrdinal_temp++;
		if ( in_out_CADComponentData_map[i].modelType == CAD_MDL_PART ) partOrdinal_temp++;

		in_out_CADComponentData_map[i].partAssemblyOrdinal = partAssemblyOrdinal_temp;
		in_out_CADComponentData_map[i].partOrdinal = partOrdinal_temp;
	}

}
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////


} // End namespace isis 