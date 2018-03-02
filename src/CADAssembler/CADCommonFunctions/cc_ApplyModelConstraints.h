#ifndef CC_APPLY_MODEL_CONSTRAINTS
#define CC_APPLY_MODEL_CONSTRAINTS

#include "cc_CommonStructures.h"
#include "isis_application_exception.h"
#include <string>
#include <list>


namespace isis
{

	/*! \file ApplyListedModelsConstraints.h 
    \brief This file declares a function (ApplyListedModelsConstraints) that constrains (i.e. positions) Creo assemblies/parts.

	Detailed Description:

		The function ApplyListedModelsConstraints constrains/positions the parts/assemblies (identified by component IDs) 
		listed in in_ComponentIDsToBeConstrained.  in_CADComponentData_map contains the constraint information 
		for each component.  Constraining a part/assembly consists of invoking CAD Toolkit functions that align 
		datums on the part/assembly with datums on another part/assembly.  This alignment process positions the 
		part/assembly in 3D space.  The part position could be fixed or a kinematic.

	Pre-Conditions:

-		None

	Post-Conditions:

-		If in_assembly_model does not point to an in-memory Creo model, isis::application_exception would be thrown.

-		If in_ComponentIDsToBeConstrained.size() == 0, then this function returns without taking any action.

-		If in_CADComponentData_map does not contain constraint information for a particular component defined in 
		in_ComponentIDsToBeConstrained, then no action would be taken for that component. 

-		If an error occurs when applying the constraints (usually caused by erroneous constraint data), then 
		isis::application_exception would be thrown. 

	*/
bool ApplyListedModelsConstraints( 
			const std::string								&in_AssemblyComponentID,
			const std::list<std::string>						&in_ComponentIDsToBeConstrained,  
			bool												in_AllowUnconstrainedModels,
			std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map,
															// Provide for the case where the first assembled part does not have
															// the datums front, top, and right defined. 
			bool												in_FirstComponentToBePositionedAsIntiiallyPlaced_IfDatumsCannotBeFound )
																		throw (isis::application_exception);

	
bool ComponentIDChildOf(	
	const std::string &in_ChildID,
	const std::string &in_ParentID,
	std::map<std::string, isis::CADComponentData> &in_CADComponentData_map );

}

#endif