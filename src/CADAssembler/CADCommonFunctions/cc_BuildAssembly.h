#ifndef CC_BUILD_ASSEMBLY_H
#define CC_BUILD_ASSEMBLY_H

#include "cc_CommonStructures.h"

#include <list>
#include <string>
#include <map>

namespace isis 
{

///////////////////////////////////////////////////////////////////////////////////////////////////
void UpdateAllSubassemblyComponentsToIncludeParentAssemblyPath( 
						const std::string in_ComponentID,
						std::map<std::string, isis::CADComponentData> &in_out_CADComponentData_map,
						int assembled_feat_id );


bool AinB(  const std::set<std::string> &in_A,  const std::set<std::string> &in_B );

void	 AddSubordinateComponentIDsToSet( const std::string &in_ComponentID, 
										 std::map<std::string, CADComponentData> &in_ComponentAssembledInfo,
										 std::set<std::string> &out_Existing );

void  LogSortingSet(	const std::string	&in_Indent,
						const std::string	&in_Title,
						const std::string	&in_ParentName,
						const std::string	&in_ParentComponentID,
						std::map<std::string, CADComponentData> &in_ComponentAssembledInfo,
						const std::set<std::string> &in_Components );


void  LogSortingList(	const std::string	&in_Indent,
						const std::string	&in_Title,
						const std::string	&in_ParentName,
						const std::string	&in_ParentComponentID,
						std::map<std::string, CADComponentData> &in_ComponentAssembledInfo,
						const std::list<std::string>  &in_Components );


void LogSortingInputs(	const std::string	&in_ParentName,
								const std::string  &in_ParentComponentID,
								const std::list<std::string> &in_Components, 
								std::map<std::string, CADComponentData> &in_ComponentAssembledInfo );


void LogStateWhenSortingFailed(	const std::string	&in_ParentName,
								const std::string  &in_ParentComponentID,
								const std::list<std::string> &in_Components, 
								const std::list<std::string> &out_Components, 
								const std::set<std::string>  &in_Existing,
								std::map<std::string, CADComponentData> &in_ComponentAssembledInfo );


void SearchForAComponentsThatDependOnMultipleAddedComponents(	
						const std::list<std::string>			&in_CandidateComponents,
						const std::set<std::string>				&in_ExistingComponents,
						std::map<std::string, CADComponentData> &in_ComponentAssembledInfo,
						bool									&out_ComponentFound,
						std::string								&out_FoundComponentID );


void SearchForAComponentsThatOnlyDependsOnAddedComponents(	
						const std::list<std::string>			&in_CandidateComponents,
						const std::set<std::string>				&in_ExistingComponents,
						bool									in_OnlySearchComponentsWithAGuideConstraint,
						std::map<std::string, CADComponentData> &in_ComponentAssembledInfo,
						bool									&out_ComponentFound,
						std::string								&out_FoundComponentID );


void SortCADComponents ( const std::string	&in_ParentName,
						 const std::string  &in_ParentComponentID,
						 const std::list<std::string> &in_Components,
						 std::map<std::string, CADComponentData> &in_ComponentAssembledInfo,
						 std::list<std::string> &out_Components )						 
																throw (isis::application_exception);

enum e_InclusionExclusionInstructions
{
	INCLUDE_ALL_COMPONENTS,
	INCLUDE_ALL_COMPONENTS_EXCEPT_SIZE_TO_FIT,
	INCLUDE_ONLY_SIZE_TO_FIT
};


void BuildFiltereListOfComponents(	const std::string								&in_AssemblyComponentID, 
									std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map, 
									e_InclusionExclusionInstructions					in_Instructions, 
									std::list<std::string>							&out_FilteredComponents );


void AddSortOrderToMap(	const std::list<std::string>						&in_SortedComponents, 
						std::map<std::string, isis::CADComponentData>	&in_out_CADComponentData_map);








///////////////////////////////////////////////////////////////////////////////////////////////////

} // End namespace isis 

#endif