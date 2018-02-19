#include <ApplyModelConstraints.h>
#include <CommonFunctions.h>
#include <BuildAssembly.h>
#include <AssembleComponents.h>
#include <DiagnosticUtilities.h>
#include <AssembleUtils.h>
#include <ToolKitPassThroughFunctions.h> 
#include "cc_LoggerBoost.h"
#include <boost/algorithm/string.hpp>
#define PRO_USE_VAR_ARGS
#include "ProMessage.h"
#include "cc_AssemblyOptions.h"
#include "DatumRefResolver.h"
#include "cc_GraphicsFunctions.h"
#include "JointCreo.h"

/*
Notes:
1.	Reference [1] (search on [1] to find the place in the code that is 
	applicable to this note).

	When assembling Pro/E models, there is the concept of the added_model and base_model.  
	The base_model is the model used to constrain the added_model.   It is important to 
	identify the added_model and base_model.  A clear indication of the added model is 
	the case where the component is constrained to geometry within itself and to geometry 
	in another component.  Because it is constrained to geometry within itself, it is the 
	added_model.  It gets more complicated when the model is not constrained using geometry 
	within itself.  This is the case when assembling an assembly where the assembly datums 
	are not used for constraining.  For that case, the  added_model (i.e. added assembly) 
	is the assembly for which one of its subordinates (ComponentIDChildOf) contain the 
	constraining geometry. 
	
	The following statement assures compliance with the above rules because the 
	ComponentIDChildOf() function would return true only if the constraint feature was 
	in a model subordinate to the current assembly.  If the constraint feature was in 
	the assembly model, then ComponentIDChildOf() function would return false, which would 
	force that constraint to be applied to the base_model.

	if ( !added_model_defined  && 
		 ( l->componentID == in_ComponentID || ComponentIDChildOf( l->componentID, in_ComponentID, in_CADComponentData_map ) )
		 ) 

	Another way of accomplishing the same result would be with a statement such as

	if ( !added_model_defined  &&  ( l->ComponentID() == i->ComponentID() || 
		 l->ComponentID() != Assembly_ComponentID))   // Note would have to pass Assembly_ComponentID
													  // into ApplyModelConstraints(…)

	This approach would be computationally faster but would not catch the error condition due to 
	the XML containing incorrect data.  The current approach results in an exception if the 
	added_model is not constrained by a model in the assembly hierarchy.  For now, stick with 
	the current approach unless performance becomes an issue.

End Notes: */
namespace isis
{
struct ContraintFeatureDefinition
{

	ProMdl					p_base_model;
	std::string				base_model_component_instance_ID;
	MultiFormatString		base_model_name;
	ProMdlType				base_model_type;

	ProMdl					p_added_model;
	std::string				added_model_component_instance_ID;
	MultiFormatString		added_model_name;
	ProMdlType				added_model_type;

	list<int>				base_model_path_list;    // list of Creo feature IDs leading to the part/assembly
	list<int>				added_model_path_list;   // list of Creo feature IDs leading to the part/assembly
	ProType					pro_datum_type;			// enum PRO_SURFACE, PRO_AXIS
	ProName					base_model_datum_name;	// ASM_RIGHT, A_1..
	ProDatumside			base_model_datum_side;	// enum PRO_DATUM_SIDE_YELLOW (SIDE_A), PRO_DATUM_SIDE_RED (SIDE_B), PRO_DATUM_SIDE_NONE
	ProName					added_model_datum_name;	// RIGHT, A23 ..
	ProDatumside			added_model_datum_side;	// enum PRO_DATUM_SIDE_YELLOW (SIDE_A), PRO_DATUM_SIDE_RED (SIDE_B), PRO_DATUM_SIDE_NONE
	ProAsmcompConstrType	constraint_type;		// enum PRO_ASM_ALIGN, PRO_ASM_ALIGN_OFF...
	double					offset_between_datums;	// This is currently not used; would only be used if in_constraint_type == PRO_ASM_ALIGN_OFF or PRO_ASM_MATE_OFF

	int						setID;
	e_CADTreatConstraintAsAGuide treatConstraintAsAGuide;

	ContraintFeatureDefinition(): treatConstraintAsAGuide(CAD_TREAT_CONSTRAINT_AS_A_GUIDE_FALSE) {};
};


struct ContraintFeatureDefinition_2
{
	ProMdl					p_base_model;
	std::string				base_model_component_instance_ID;
	MultiFormatString		base_model_name;
	ProMdlType				base_model_type;

	ProMdl					p_added_model;
	std::string				added_model_component_instance_ID;
	MultiFormatString		added_model_name;
	ProMdlType				added_model_type;

	list<int>				base_model_path_list;   // list of Creo feature IDs leading to the part/assembly
	list<int>				added_model_path_list;  // list of Creo feature IDs leading to the part/assembly
	ProType					pro_datum_type;			// enum PRO_SURFACE, PRO_AXIS
	ProName					base_model_datum_name;	// ASM_RIGHT, A_1..
	ProDatumside			base_model_datum_side;	// enum PRO_DATUM_SIDE_YELLOW (SIDE_A), PRO_DATUM_SIDE_RED (SIDE_B), PRO_DATUM_SIDE_NONE
	ProName					added_model_datum_name;	// RIGHT, A23 ..
	ProDatumside			added_model_datum_side;	// enum PRO_DATUM_SIDE_YELLOW (SIDE_A), PRO_DATUM_SIDE_RED (SIDE_B), PRO_DATUM_SIDE_NONE
	ProAsmcompConstrType	constraint_type;		// enum PRO_ASM_ALIGN, PRO_ASM_ALIGN_OFF...
	double					offset_between_datums;	// This is currently not used; would only be used if in_constraint_type == PRO_ASM_ALIGN_OFF or PRO_ASM_MATE_OFF
	bool					flip_orientation;		// This applies to axes and would be set if the initial constraint 
													// resulted in an insolvent constraint. For example, if the start and end points of two
													// axes (i.e. base and added axes) resulted in a vectors pointing int the opposited directions,
													// then flip_orientation would be set to true.
	e_CADTreatConstraintAsAGuide treatConstraintAsAGuide;

	ContraintFeatureDefinition_2(): treatConstraintAsAGuide(CAD_TREAT_CONSTRAINT_AS_A_GUIDE_FALSE), flip_orientation(false){};
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PerSetConstraintDefinition
{
	int setID;

	e_CADJointType  jointType_withoutguide;

	bool hasAGuideConstraint;
	e_CADJointType  jointType_withguide;

	std::vector<ContraintFeatureDefinition> contraintFeatureDefinitions;

	PerSetConstraintDefinition() : setID(1), 
								jointType_withoutguide(UNKNOWN_JOINT_TYPE), 
								hasAGuideConstraint(false),
								jointType_withguide(UNKNOWN_JOINT_TYPE) {};
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SortOrderBasedOnJointType( e_CADJointType			in_JointType,
								std::vector<ProType>	&out_SortOrder_vector )
{
	out_SortOrder_vector.clear();

	switch ( in_JointType )
	{
		case FIXED_JOINT:  // Rigid in Creo
			out_SortOrder_vector.push_back(PRO_CSYS);
			out_SortOrder_vector.push_back(PRO_SURFACE);
			out_SortOrder_vector.push_back(PRO_POINT);
			out_SortOrder_vector.push_back(PRO_AXIS);
		break;

		case REVOLUTE_JOINT:  // Pin constraint in Creo
			out_SortOrder_vector.push_back(PRO_AXIS);  // There would always be at least one axis
			out_SortOrder_vector.push_back(PRO_SURFACE);
			out_SortOrder_vector.push_back(PRO_POINT);
		break;

		case UNIVERSAL_JOINT:
			// Creo Does not have a universal joint
			// Normally this would require three parts, see http://en.wikipedia.org/wiki/Universal_joint
			out_SortOrder_vector.push_back(PRO_AXIS);
			out_SortOrder_vector.push_back(PRO_POINT);
			out_SortOrder_vector.push_back(PRO_SURFACE);
			out_SortOrder_vector.push_back(PRO_CSYS);
		break;

		case SPHERICAL_JOINT:  // Ball in Creo
			out_SortOrder_vector.push_back(PRO_POINT);
		break;

		case PRISMATIC_JOINT: // Slider in Creo
			out_SortOrder_vector.push_back(PRO_AXIS);  // Could be a plane and axis
			out_SortOrder_vector.push_back(PRO_SURFACE); // Could be two orthoganal planes

		break;
		case CYLINDRICAL_JOINT:  // Cylinder in Creo
			out_SortOrder_vector.push_back(PRO_AXIS);
		break;

		case PLANAR_JOINT:  // Planar in Creo
			out_SortOrder_vector.push_back(PRO_SURFACE);
		break;

		case UNKNOWN_JOINT_TYPE:
			out_SortOrder_vector.push_back(PRO_CSYS);
			out_SortOrder_vector.push_back(PRO_SURFACE);
			out_SortOrder_vector.push_back(PRO_AXIS);
			out_SortOrder_vector.push_back(PRO_POINT);
		break;
		
		default:
			out_SortOrder_vector.push_back(PRO_CSYS);
			out_SortOrder_vector.push_back(PRO_SURFACE);
			out_SortOrder_vector.push_back(PRO_AXIS);
			out_SortOrder_vector.push_back(PRO_POINT);
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class PerSetConstraintDefinition_2
{
	public:
			PerSetConstraintDefinition_2();

			void			set_JointType_withoutguide( e_CADJointType in_JointType_withoutguide );
			e_CADJointType	get_JointType_withoutguide() const;
			
			void			set_HasAGuideConstraint( bool in_HasAGuideConstraint );
			bool			get_HasAGuideConstraint() const;

			void			set_JointType_withguide( e_CADJointType in_JointType_withguide );
			e_CADJointType	get_JointType_withguide() const;

			void				set_JointMotionLimits( const InputJoint &in_JointMotionLimits);
			const InputJoint&	get_JointMotionLimits() const;

			void			add_ContraintFeatureDefinition (const ContraintFeatureDefinition_2 &in_ContraintFeatureDefinition );

			const ContraintFeatureDefinition_2&	get_ContraintFeatureDefinition_const ( int in_ConstraintID ) const
																							throw (isis::application_exception);

			ContraintFeatureDefinition_2&	get_ContraintFeatureDefinition ( int in_ConstraintID ) 
																			throw (isis::application_exception);

			std::vector<ContraintFeatureDefinition_2>::const_iterator begin_ContraintFeatureDefinitions() const;
			std::vector<ContraintFeatureDefinition_2>::const_iterator end_ContraintFeatureDefinitions()   const;

			int				get_NumberOfConstraintFeatures() const;

			// Note - The following function will add to out_DatumTypesUsedByConstraintFeatures.  It is the caller's
			//        responsibility to clear out_DatumTypesUsedByConstraintFeatures before calling if only the 
			//		  DatumTypes from this one instance are needed.
			void		    get_DatumTypesUsedByConstraintFeatures ( std::set<ProType> &out_DatumTypesUsedByConstraintFeatures) const;
			
	private:
		e_CADJointType								jointType_withoutguide;

		bool										hasAGuideConstraint;
		e_CADJointType								jointType_withguide;

		std::vector<ContraintFeatureDefinition_2>	contraintFeatureDefinitions;

		InputJoint									jointMotionLimits;

		std::vector<int>							sortedConstraintIDs;
 };

PerSetConstraintDefinition_2::PerSetConstraintDefinition_2() :  
									jointType_withoutguide(UNKNOWN_JOINT_TYPE), 
									hasAGuideConstraint(false),
									jointType_withguide(UNKNOWN_JOINT_TYPE) {};


void PerSetConstraintDefinition_2::set_JointType_withoutguide( e_CADJointType in_JointType_withoutguide ) 
{ 
	jointType_withoutguide = in_JointType_withoutguide;
}

e_CADJointType PerSetConstraintDefinition_2::get_JointType_withoutguide() const 
{ 
	return jointType_withoutguide; 
}

void PerSetConstraintDefinition_2::set_HasAGuideConstraint( bool in_HasAGuideConstraint )
{ 
	hasAGuideConstraint = in_HasAGuideConstraint;
}

bool PerSetConstraintDefinition_2::get_HasAGuideConstraint() const 
{
	return hasAGuideConstraint;
}

void PerSetConstraintDefinition_2::set_JointType_withguide( e_CADJointType in_JointType_withguide ) 
{ 
	jointType_withguide= in_JointType_withguide;
}

e_CADJointType PerSetConstraintDefinition_2::get_JointType_withguide() const 
{ 
	return jointType_withguide; 
}

void PerSetConstraintDefinition_2::set_JointMotionLimits( const InputJoint & in_JointMotionLimits) 
{ 
	jointMotionLimits = in_JointMotionLimits;
}

const InputJoint& PerSetConstraintDefinition_2::get_JointMotionLimits() const 
{ 
	return jointMotionLimits; 
}

void PerSetConstraintDefinition_2::add_ContraintFeatureDefinition (const ContraintFeatureDefinition_2 &in_ContraintFeatureDefinition )
{ 
	contraintFeatureDefinitions.push_back( in_ContraintFeatureDefinition ); 
}

const ContraintFeatureDefinition_2&	PerSetConstraintDefinition_2::get_ContraintFeatureDefinition_const ( int in_ConstraintID ) const
																					throw (isis::application_exception)
{
	if ( in_ConstraintID < 0  || in_ConstraintID >= contraintFeatureDefinitions.size() )
	{
		std::stringstream errorString;
		errorString << "Function - " << __FUNCTION__ << ", was passed an out-of-range in_ConstraintID" << std::endl <<
			"  in_ConstraintID: " << in_ConstraintID << std::endl <<
			"  Allowed Range:   " << "0 to " <<  contraintFeatureDefinitions.size() -1;
		throw isis::application_exception("C04002",errorString.str());			
	}
	return contraintFeatureDefinitions[in_ConstraintID];
}

ContraintFeatureDefinition_2&	PerSetConstraintDefinition_2::get_ContraintFeatureDefinition ( int in_ConstraintID ) 
																					throw (isis::application_exception)
{
	if ( in_ConstraintID < 0  || in_ConstraintID >= contraintFeatureDefinitions.size() )
	{
		std::stringstream errorString;
		errorString << "Function - " << __FUNCTION__ << ", was passed an out-of-range in_ConstraintID" << std::endl <<
			"  in_ConstraintID: " << in_ConstraintID << std::endl <<
			"  Allowed Range:   " << "0 to " <<  contraintFeatureDefinitions.size() -1;
		throw isis::application_exception("C04002",errorString.str());			
	}
	return contraintFeatureDefinitions[in_ConstraintID];
}


std::vector<ContraintFeatureDefinition_2>::const_iterator PerSetConstraintDefinition_2::begin_ContraintFeatureDefinitions() const 
{ 
	return  contraintFeatureDefinitions.begin(); 
}

std::vector<ContraintFeatureDefinition_2>::const_iterator PerSetConstraintDefinition_2::end_ContraintFeatureDefinitions()   const 
{ 
	return  contraintFeatureDefinitions.end(); 
}

int	PerSetConstraintDefinition_2::get_NumberOfConstraintFeatures() const
{
	return contraintFeatureDefinitions.size();
}

void PerSetConstraintDefinition_2::get_DatumTypesUsedByConstraintFeatures ( std::set<ProType> &out_DatumTypesUsedByConstraintFeatures) const
{
	for each ( const ContraintFeatureDefinition_2 &i in contraintFeatureDefinitions )
		out_DatumTypesUsedByConstraintFeatures.insert(i.pro_datum_type);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ContainsAGuide(const std::vector<ContraintFeatureDefinition> &in_ContraintFeatureDefinitions  )
{
	for (std::vector<ContraintFeatureDefinition>::const_iterator i = in_ContraintFeatureDefinitions.begin(); i != in_ContraintFeatureDefinitions.end(); ++i)
	{
		if ( i->treatConstraintAsAGuide == CAD_TREAT_CONSTRAINT_AS_A_GUIDE_TRUE ) return true;
	}

	return false;
}

bool ContainsAGuide(const std::vector<PerSetConstraintDefinition> &in_PerSetConstraintDefinitions  )
{
	for each ( PerSetConstraintDefinition i in in_PerSetConstraintDefinitions )
		for each ( ContraintFeatureDefinition j in i.contraintFeatureDefinitions)
			if ( j.treatConstraintAsAGuide == CAD_TREAT_CONSTRAINT_AS_A_GUIDE_TRUE ) return true;
	
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Temporary support for revolute joints.  Will assume a datum plane and axis result in a revolute joint
/*
e_CADJointType GetJointType( const std::vector<ContraintFeatureDefinition> &in_ContraintFeatureDefinitions, int in_SetID, bool in_IncludeGuides )
{
	std::vector<ContraintFeatureDefinition> ContraintFeatureDefinitions_PerSet;
	for each (ContraintFeatureDefinition i in in_ContraintFeatureDefinitions)
	{
		//std::cout << std::endl << "\n*** GetJointType, i.treatConstraintAsAGuide: << 

		if ( i.setID == in_SetID  )
		{
			if ( in_IncludeGuides )
			{
				ContraintFeatureDefinitions_PerSet.push_back(i);
			}
			else
			{
				if ( i.treatConstraintAsAGuide == CAD_TREAT_CONSTRAINT_AS_A_GUIDE_FALSE ) ContraintFeatureDefinitions_PerSet.push_back(i);
			}
		}
	}

	bool DatumPlane_found = false;
	bool DatumAxis_found = false;

	if ( ContraintFeatureDefinitions_PerSet.size() == 2 )
	{
		for each (ContraintFeatureDefinition i in ContraintFeatureDefinitions_PerSet )
		{
			if (i.pro_datum_type == PRO_SURFACE ) DatumPlane_found = true;
			if (i.pro_datum_type == PRO_AXIS ) DatumAxis_found = true;
		}
	}
	
	if ( DatumPlane_found && DatumAxis_found ) 
		return REVOLUTE_JOINT;
	else
	   return UNKNOWN_JOINT_TYPE;

}
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Temporary support for revolute joints.  Will assume a datum plane and axis result in a revolute joint
// This function returns UNKNOWN_JOINT_TYPE if the definition contains guides
/*
e_CADJointType GetJointType( std::vector<ContraintFeatureDefinition> &ContraintFeatureDefinitions_PerSet, const std::vector<ContraintFeatureDefinition> &in_ContraintFeatureDefinitions, int in_SetID, bool in_IncludeGuides )
{
	for each (ContraintFeatureDefinition i in in_ContraintFeatureDefinitions)
	{
		//std::cout << std::endl << "\n*** GetJointType, i.treatConstraintAsAGuide: << 

		if ( i.setID == in_SetID  )
		{
			if ( in_IncludeGuides )
			{
				ContraintFeatureDefinitions_PerSet.push_back(i);
			}
			else
			{
				if ( i.treatConstraintAsAGuide == CAD_TREAT_CONSTRAINT_AS_A_GUIDE_FALSE ) ContraintFeatureDefinitions_PerSet.push_back(i);
			}
		}
	}

	bool DatumPlane_found = false;
	bool DatumAxis_found = false;

	if ( ContraintFeatureDefinitions_PerSet.size() == 2 )
	{
		for each (ContraintFeatureDefinition i in ContraintFeatureDefinitions_PerSet )
		{
			if (i.pro_datum_type == PRO_SURFACE ) DatumPlane_found = true;
			if (i.pro_datum_type == PRO_AXIS ) DatumAxis_found = true;
		}
	}
	
	if ( DatumPlane_found && DatumAxis_found ) 
		return REVOLUTE_JOINT;
	else
	   return UNKNOWN_JOINT_TYPE;

}
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Set_Limits_On_Set(ProElement out_set, ProReference componentside_ref, ProReference assemblyside_ref, const InputJoint &joint, const InputJoint::Limit *limit)
{
	ProElemId elemid;
	ProValueData value_data;	

	// Ass SET_FLIP
	/*ProElement set_flip_elem;	
	isis::isis_ProElementAlloc (PRO_E_COMPONENT_SET_FLIP, &set_flip_elem);
	ProElementIntegerSet(set_flip_elem,0);
	isis::isis_ProElemtreeElementAdd (out_set, NULL, set_flip_elem ); */

	// Add JAS_SETS
	ProElement jas_sets_elem;	
	isis::isis_ProElementAlloc (PRO_E_COMPONENT_JAS_SETS, &jas_sets_elem);  
	isis::isis_ProElemtreeElementAdd (out_set, NULL, jas_sets_elem ); 

		// Add JAS_SET
		ProElement jas_set_elem;	
		isis::isis_ProElementAlloc (PRO_E_COMPONENT_JAS_SET, &jas_set_elem);  
		isis::isis_ProElemtreeElementAdd (jas_sets_elem, NULL, jas_set_elem );
			
			// snyako@isis.vanderbilt.edu: Don't know what this is. These are empirical values observed in Creo
			int zerotype;
			switch (joint.Type)
			{
			case REVOLUTE_JOINT:
				zerotype = 3;
				break;
			case PRISMATIC_JOINT:
				zerotype = 0;
				break;
			default:
				// ERROR
				break;
			}

			// Add JAS_ZERO_TYPE
			elemid = PRO_E_COMPONENT_JAS_ZERO_TYPE;
			value_data.type = PRO_VALUE_TYPE_INT;
			value_data.v.i = zerotype;
			ProElement zero_type_elem;
			isis::isis_AddElementtoElemTree(elemid, &jas_set_elem, &value_data, &zero_type_elem);

			// Add JAS_REFS
			ProElement jas_refs_elem;	
			isis::isis_ProElementAlloc (PRO_E_COMPONENT_JAS_REFS, &jas_refs_elem);  
			isis::isis_ProElemtreeElementAdd (jas_set_elem, NULL, jas_refs_elem ); 

				// Add JAS_ORANGE_REF (component side ref)
				ProElement jas_orange_ref_elem;	
				isis::isis_ProElementAlloc (PRO_E_COMPONENT_JAS_ORANGE_REF, &jas_orange_ref_elem);  
				ProElementReferenceSet(jas_orange_ref_elem, componentside_ref);
				isis::isis_ProElemtreeElementAdd (jas_refs_elem, NULL, jas_orange_ref_elem ); 

				// Add JAS_GREEN_REF (assembly side ref)
				ProElement jas_green_ref_elem;	
				isis::isis_ProElementAlloc (PRO_E_COMPONENT_JAS_GREEN_REF, &jas_green_ref_elem);  
				ProElementReferenceSet(jas_green_ref_elem, assemblyside_ref);
				isis::isis_ProElemtreeElementAdd (jas_refs_elem, NULL, jas_green_ref_elem ); 

				// Add JAS_0_OFFSET
				ProElement jas_zero_offset_elem;	
				isis::isis_ProElementAlloc (PRO_E_COMPONENT_JAS_0_OFFSET_VAL, &jas_zero_offset_elem);
				ProElementDoubleSet(jas_zero_offset_elem, limit[InputJoint::DEFAULT].Value);
				isis::isis_ProElemtreeElementAdd (jas_refs_elem, NULL, jas_zero_offset_elem );

			// Add JAS_REGEN_VALUE_GROUP
			ProElement jas_regen_value_group_elem;
			isis::isis_ProElementAlloc (PRO_E_COMPONENT_JAS_REGEN_VALUE_GROUP, &jas_regen_value_group_elem);  
			isis::isis_ProElemtreeElementAdd (jas_set_elem, NULL, jas_regen_value_group_elem );

				// Add JAS_REGEN_VALUE_FLAG
				ProElement jas_regen_value_flag_elem;
				isis::isis_ProElementAlloc (PRO_E_COMPONENT_JAS_REGEN_VALUE_FLAG, &jas_regen_value_flag_elem);  
				ProElementIntegerSet(jas_regen_value_flag_elem, 0);
				isis::isis_ProElemtreeElementAdd (jas_regen_value_group_elem, NULL, jas_regen_value_flag_elem ); 

				// Add JAS_REGEN_VALUE
				ProElement jas_regen_value_elem;
				isis::isis_ProElementAlloc (PRO_E_COMPONENT_JAS_REGEN_VALUE, &jas_regen_value_elem);  
				ProElementDoubleSet(jas_regen_value_elem, 0);
				isis::isis_ProElemtreeElementAdd (jas_regen_value_group_elem, NULL, jas_regen_value_elem ); 

				// Add JAS_MIN_LIMIT
			ProElement jas_min_limit_elem;
			isis::isis_ProElementAlloc (PRO_E_COMPONENT_JAS_MIN_LIMIT, &jas_min_limit_elem);  
			isis::isis_ProElemtreeElementAdd (jas_set_elem, NULL, jas_min_limit_elem ); 

				// Add JAS_MIN_LIMIT_FLAG
				ProElement jas_min_limit_flag_elem;
				isis::isis_ProElementAlloc (PRO_E_COMPONENT_JAS_MIN_LIMIT_FLAG, &jas_min_limit_flag_elem);  
				ProElementIntegerSet(jas_min_limit_flag_elem, limit[InputJoint::MIN].Provided?1:0);
				isis::isis_ProElemtreeElementAdd (jas_min_limit_elem, NULL, jas_min_limit_flag_elem ); 

				if (limit[InputJoint::MIN].Provided)
				{
					// Add JAS_MIN_LIMIT_VAL
					ProElement jas_min_limit_val_elem;
					isis::isis_ProElementAlloc (PRO_E_COMPONENT_JAS_MIN_LIMIT_VAL, &jas_min_limit_val_elem);  
					ProElementDoubleSet(jas_min_limit_val_elem, limit[InputJoint::MIN].Value);
					isis::isis_ProElemtreeElementAdd (jas_min_limit_elem, NULL, jas_min_limit_val_elem );
				}
				
			// Add JAS_MAX_LIMIT
			ProElement jas_max_limit_elem;
			isis::isis_ProElementAlloc (PRO_E_COMPONENT_JAS_MAX_LIMIT, &jas_max_limit_elem);  
			isis::isis_ProElemtreeElementAdd (jas_set_elem, NULL, jas_max_limit_elem ); 

				// Add JAS_MAX_LIMIT_FLAG
				ProElement jas_max_limit_flag_elem;
				isis::isis_ProElementAlloc (PRO_E_COMPONENT_JAS_MAX_LIMIT_FLAG, &jas_max_limit_flag_elem);  
				ProElementIntegerSet(jas_max_limit_flag_elem, limit[InputJoint::MAX].Provided?1:0);
				isis::isis_ProElemtreeElementAdd (jas_max_limit_elem, NULL, jas_max_limit_flag_elem ); 

				if (limit[InputJoint::MAX].Provided)
				{
					// Add JAS_MAX_LIMIT_VAL
					ProElement jas_MAX_limit_val_elem;
					isis::isis_ProElementAlloc (PRO_E_COMPONENT_JAS_MAX_LIMIT_VAL, &jas_MAX_limit_val_elem);  
					ProElementDoubleSet(jas_MAX_limit_val_elem, limit[InputJoint::MAX].Value);
					isis::isis_ProElemtreeElementAdd (jas_max_limit_elem, NULL, jas_MAX_limit_val_elem ); 
				}
				
			// Add PRO_E_COMPONENT_JAS_RESTITUTION
			ProElement jas_restitution_elem;
			isis::isis_ProElementAlloc (PRO_E_COMPONENT_JAS_RESTITUTION, &jas_restitution_elem);  
			isis::isis_ProElemtreeElementAdd (jas_set_elem, NULL, jas_restitution_elem );

				// Add PRO_E_COMPONENT_JAS_RESTITUTION_FLAG
				ProElement jas_restitution_flag_elem;
				isis::isis_ProElementAlloc (PRO_E_COMPONENT_JAS_RESTITUTION_FLAG, &jas_restitution_flag_elem);  
				ProElementIntegerSet(jas_restitution_flag_elem, 0);
				isis::isis_ProElemtreeElementAdd (jas_restitution_elem, NULL, jas_restitution_flag_elem ); 

				// Add PRO_E_COMPONENT_JAS_RESTITUTION_COEF
				ProElement jas_restitution_coef_elem;
				isis::isis_ProElementAlloc (PRO_E_COMPONENT_JAS_RESTITUTION_COEF, &jas_restitution_coef_elem);  
				ProElementDoubleSet(jas_restitution_coef_elem, 0);
				isis::isis_ProElemtreeElementAdd (jas_restitution_elem, NULL, jas_restitution_coef_elem ); 

			// Add PRO_E_COMPONENT_JAS_FRICTION
			ProElement jas_friction_elem;
			isis::isis_ProElementAlloc (PRO_E_COMPONENT_JAS_FRICTION, &jas_friction_elem);  
			isis::isis_ProElemtreeElementAdd (jas_set_elem, NULL, jas_friction_elem );

				// Add PRO_E_COMPONENT_JAS_FRICTION_FLAG
				ProElement jas_friction_flag_elem;
				isis::isis_ProElementAlloc (PRO_E_COMPONENT_JAS_FRICTION_FLAG, &jas_friction_flag_elem);  
				ProElementIntegerSet(jas_restitution_flag_elem, 0);
				isis::isis_ProElemtreeElementAdd (jas_friction_elem, NULL, jas_friction_flag_elem ); 

				// Add PRO_E_COMPONENT_JAS_KINEM_FRIC_COEF
				ProElement jas_kinem_fric_coef_elem;
				isis::isis_ProElementAlloc (PRO_E_COMPONENT_JAS_KINEM_FRIC_COEF, &jas_kinem_fric_coef_elem);  
				ProElementDoubleSet(jas_kinem_fric_coef_elem, 0);
				isis::isis_ProElemtreeElementAdd (jas_friction_elem, NULL, jas_kinem_fric_coef_elem ); 

				// Add PRO_E_COMPONENT_JAS_STATIC_FRIC_COEF
				ProElement jas_static_fric_coef_elem;
				isis::isis_ProElementAlloc (PRO_E_COMPONENT_JAS_STATIC_FRIC_COEF, &jas_static_fric_coef_elem);  
				ProElementDoubleSet(jas_static_fric_coef_elem, 0);
				isis::isis_ProElemtreeElementAdd (jas_friction_elem, NULL, jas_static_fric_coef_elem ); 

				// Add PRO_E_COMPONENT_JAS_RADIUS_VALUE
				ProElement jas_radius_elem;
				isis::isis_ProElementAlloc (PRO_E_COMPONENT_JAS_RADIUS_VALUE, &jas_radius_elem);  
				ProElementDoubleSet(jas_radius_elem, 0.01);
				isis::isis_ProElemtreeElementAdd (jas_friction_elem, NULL, jas_radius_elem ); 
				
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Add_PRO_E_COMPONENT_SET(	ProElement out_sets_elem, 
								const MultiFormatString &in_SetName, 
								int in_SetID,
								ProAsmcompSetType in_SetType )
											throw (isis::application_exception)
{
	ProElemId elemid;
	ProValueData value_data;	

	//
	
	////////////////////////////////
	// Add PRO_E_COMPONENT_SET
	////////////////////////////////
	ProElement comp_constr_set_elem;	
	isis::isis_ProElementAlloc (PRO_E_COMPONENT_SET, &comp_constr_set_elem);  
	isis::isis_ProElemtreeElementAdd (out_sets_elem, NULL, comp_constr_set_elem ); 
		
	////////////////////////////////
	// Add a PRO_E_COMPONENT_SET_ID
	////////////////////////////////
	elemid = PRO_E_COMPONENT_SET_ID;
	value_data.type = PRO_VALUE_TYPE_INT;
	value_data.v.i = in_SetID ;
	ProElement comp_setId_elem;
	isis::isis_AddElementtoElemTree(elemid, &comp_constr_set_elem,&value_data, &comp_setId_elem);

	////////////////////////////////
	// Add a PRO_E_COMPONENT_SET_ID
	////////////////////////////////
	elemid = PRO_E_COMPONENT_SET_FLIP;
	value_data.type = PRO_VALUE_TYPE_INT;
	value_data.v.i = 0 ;
	ProElement comp_setFlip_elem;
	isis::isis_AddElementtoElemTree(elemid, &comp_constr_set_elem,&value_data, &comp_setFlip_elem);

	//////////////////////////////////
	// Add a PRO_E_COMPONENT_SET_TYPE
	//////////////////////////////////
	elemid = PRO_E_COMPONENT_SET_TYPE;
	value_data.type = PRO_VALUE_TYPE_INT;
	value_data.v.i = in_SetType ;
	ProElement comp_setType_elem;
	isis::isis_AddElementtoElemTree(elemid, &comp_constr_set_elem,&value_data, &comp_setType_elem);
	
	////////////////////////////////////////
	// Add a PRO_E_COMPONENT_SET_NAME
	////////////////////////////////////////
	ProElement comp_setTID_elem;
	isis::isis_ProElementAlloc (PRO_E_COMPONENT_SET_NAME, &comp_setTID_elem);  
	ProElementWstringSet(comp_setTID_elem, (wchar_t*)(const wchar_t*)in_SetName);
	isis::isis_ProElemtreeElementAdd(comp_constr_set_elem, NULL, comp_setTID_elem);

	///////////////////////////////////////
	// Add a PRO_E_COMPONENT_SET_MISC_ATTR
	///////////////////////////////////////
	elemid = PRO_E_COMPONENT_SET_MISC_ATTR;
	value_data.type = PRO_VALUE_TYPE_INT;
	value_data.v.i = PRO_ASM_ENABLE_COMP_SET ;
	ProElement comp_setMiscAttr_elem;
	isis::isis_AddElementtoElemTree(elemid, &comp_constr_set_elem,&value_data, &comp_setMiscAttr_elem);
	
	////////////////////////////////
	// Free allocated structures
	////////////////////////////////
	// don't free here must commit the tree before freeing.
	//isis::isis_ProElementFree(&comp_constr_set_elem);

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RetrieveProSelectionForGeometry(  
			const ProSolid				&in_TopAssemblyModel,		// Used as the starting point of model_path_list
			const list<int>				&in_model_path_list,		// Used to locate the model in the assembly containing the geometry
			const std::string			&in_component_instance_ID,	// Used for logging error messages
			const MultiFormatString		&in_model_name,				// Used for logging error messages
			ProMdlType					in_model_type,				// Used for logging error messages
			ProType					    in_pro_geometry_type,		// Identifies the geometry type, e.g. Datum
			const ProName				in_pro_geometry_name,		// Geometry Name, e.g. Datum Name
			ProSelection				&out_pro_selection)			// Returned pro_selection
{
	ProAsmcomppath	comp_path;
	isis::Retrieve_ProAsmcomppath_WithExceptions(in_TopAssemblyModel, in_model_path_list, comp_path);

	ProMdl		 model_containing_datum;		// typedef void* ProMdl
	isis::isis_ProAsmcomppathMdlGet( &comp_path, &model_containing_datum);

	ProModelitem  model_datum;

	isis::isis_ProModelitemByNameInit_WithDescriptiveErrorMsg (	in_component_instance_ID, // Added arguments for logging
																in_model_name, 
																in_model_type,   
																//in_ContraintDef.p_base_model, //base_model, // Original arguments
																model_containing_datum, //base_model, // Original arguments
																in_pro_geometry_type, 
																in_pro_geometry_name, 
																&model_datum);  

	
	ValidatePathAndModelItem_ThrowExceptionIfInvalid(comp_path, model_datum);
	isis::isis_ProSelectionAlloc(&comp_path, &model_datum, &out_pro_selection);

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This function has memory leaks.  Should fix.
void Add_PRO_E_COMPONENT_CONSTRAINT( ProElement &out_constrs_elem, 
									const ContraintFeatureDefinition &in_ContraintDef, 
									ProSolid &in_TopAssemblyModel,
									e_CADJointType in_JointType, int axiscounter, int surfacecounter, bool guide, int setidx)
														throw (isis::application_exception)
{

		

		std::stringstream str;
		isis_LOG(lg, isis_FILE, isis_INFO) << "Add_PRO_E_COMPONENT_CONSTRAINT >>>>>>>>>>>>>>>";
		//isis_LOG(lg, isis_FILE, isis_INFO) << "\n**** Axis Constraint Index: " <<  counter << "  ******";
		stream_PopulateOneConstraintInConstraintStructure(
						&in_TopAssemblyModel,	// typedef struct sld_part* ProSolid;
						in_ContraintDef.base_model_path_list,		
						in_ContraintDef.added_model_path_list,
						in_ContraintDef.pro_datum_type,				// enum PRO_SURFACE, PRO_AXIS
						in_ContraintDef.base_model_datum_name,		// ASM_RIGHT, A_1..
						in_ContraintDef.base_model_datum_side,		// enum PRO_DATUM_SIDE_YELLOW (SIDE_A), PRO_DATUM_SIDE_RED (SIDE_B), PRO_DATUM_SIDE_NONE
						in_ContraintDef.added_model_datum_name,		// RIGHT, A23 ..
						in_ContraintDef.added_model_datum_side,		// enum PRO_DATUM_SIDE_YELLOW (SIDE_A), PRO_DATUM_SIDE_RED (SIDE_B), PRO_DATUM_SIDE_NONE
						in_ContraintDef.constraint_type,				// enum PRO_ASM_ALIGN, PRO_ASM_ALIGN_OFF...
						in_ContraintDef.offset_between_datums,		// This is only used if in_constraint_type == PRO_ASM_ALIGN_OFF or PRO_ASM_MATE_OFF
						str);
			
			isis_LOG(lg, isis_FILE, isis_INFO) << str.str();
			isis_LOG(lg, isis_FILE, isis_INFO);
		///////////////////////////////////
		// Add PRO_E_COMPONENT_CONSTRAINT
		//////////////////////////////////
		ProElement comp_constr_elem;

		isis::isis_ProElementAlloc (PRO_E_COMPONENT_CONSTRAINT, &comp_constr_elem);  
		isis::isis_ProElemtreeElementAdd (out_constrs_elem, NULL, comp_constr_elem);

		///////////////////////////////////
		// Add PRO_E_COMPONENT_CONSTR_TYPE
		///////////////////////////////////
		ProElemId elemid;
		ProValueData value_data;
		elemid = PRO_E_COMPONENT_CONSTR_TYPE;
		value_data.type = PRO_VALUE_TYPE_INT;

		ProAsmcompConstrType constrtype = in_ContraintDef.constraint_type;

		// snyako@isis.vanderbilt.edu:
		// Kinematic joints doesn't seem to respect the side setting, therefore need to achieve the same result
		// with the MATE/ALIGN
		if (!guide && in_ContraintDef.base_model_datum_side != in_ContraintDef.added_model_datum_side)
		{
			constrtype = constrtype==PRO_ASM_MATE?PRO_ASM_ALIGN:PRO_ASM_MATE;
		}

		value_data.v.i = constrtype;
		ProElement constr1_type_elem;
		isis::isis_AddElementtoElemTree(elemid, &comp_constr_elem,&value_data, &constr1_type_elem);

		/////////////////////////////////////////
		// Add PRO_E_COMPONENT_COMP_CONSTR_REF
		/////////////////////////////////////////

		// *******************************************
		// Initialize the path to the base component
		// ********************************************
		ProAsmcomppath	base_comp_path;
		isis::Retrieve_ProAsmcomppath_WithExceptions(in_TopAssemblyModel, in_ContraintDef.base_model_path_list, base_comp_path);

		// ***********************************************
		// Initialize the path to the assembled component
		// ***********************************************
		ProAsmcomppath	added_comp_path;
		isis::Retrieve_ProAsmcomppath_WithExceptions(in_TopAssemblyModel, in_ContraintDef.added_model_path_list, added_comp_path);
		//for (int m = 0; m < added_model_table_size; ++m) std::cout << std::endl << std::endl << c_id_table_added_model[m];

		// **************************************************
		// Find the addresse to the base and added compoents
		// **************************************************
		ProMdl		 base_model;					// typedef void* ProMdl
		ProMdl		 added_model;
		isis::isis_ProAsmcomppathMdlGet( &base_comp_path, &base_model);
		isis::isis_ProAsmcomppathMdlGet( &added_comp_path, &added_model);

		//	Find the assembly datum 
		ProModelitem  base_model_datum;
		//isis::MultiFormatString  temp_string(in_ContraintDef.base_model_datum_name);
		//std::cout << std::endl << "+++ Base model datum name: " << temp_string << std::endl;

		isis::isis_ProModelitemByNameInit_WithDescriptiveErrorMsg (	in_ContraintDef.base_model_component_instance_ID, // Added arguments
																	in_ContraintDef.base_model_name, 
																	in_ContraintDef.base_model_type,   
																	//in_ContraintDef.p_base_model, //base_model, // Original arguments
																	base_model, //base_model, // Original arguments
																	in_ContraintDef.pro_datum_type, 
																	in_ContraintDef.base_model_datum_name, 
																	&base_model_datum);  

		//	Find the component datum
		ProModelitem  add_model_datum;

		//temp_string = in_ContraintDef.added_model_datum_name;
		//std::cout << std::endl << "+++ Added model datum name: " << temp_string << std::endl;

		isis::isis_ProModelitemByNameInit_WithDescriptiveErrorMsg (	in_ContraintDef.added_model_component_instance_ID, // Added arguments
																	in_ContraintDef.added_model_name, 
																	in_ContraintDef.added_model_type,   
																	//in_ContraintDef.p_added_model, //added_model, // Original arguments
																	added_model,
																	in_ContraintDef.pro_datum_type, 
																	in_ContraintDef.added_model_datum_name, 
																	&add_model_datum);  

		//	For the assembly reference, initialize a component path.
		//	This is necessary even if the reference geometry is in the assembly.

		//	Allocate the references
		ProSelection base_model_select;
	
		ValidatePathAndModelItem_ThrowExceptionIfInvalid(base_comp_path, base_model_datum);
		isis::isis_ProSelectionAlloc(&base_comp_path, &base_model_datum, &base_model_select);

		ProSelection added_model_select;

		ValidatePathAndModelItem_ThrowExceptionIfInvalid(added_comp_path, add_model_datum);
		isis::isis_ProSelectionAlloc (&added_comp_path, &add_model_datum, &added_model_select);

		//////////////////////////////////////
		// Add PRO_E_COMPONENT_COMP_CONSTR_REF
		//////////////////////////////////////
		elemid = PRO_E_COMPONENT_COMP_CONSTR_REF ;
		value_data.type = PRO_VALUE_TYPE_SELECTION;
		value_data.v.r =  added_model_select;
		ProElement comp_const1_Ref_elem;
		isis::isis_AddElementtoElemTree(elemid, &comp_constr_elem,&value_data, &comp_const1_Ref_elem);

// add PRO_XML_ASMCOMP_PATH

		//////////////////////////////////////
		// Add PRO_E_COMPONENT_ASSEM_CONSTR_REF
		//////////////////////////////////////
		elemid = PRO_E_COMPONENT_ASSEM_CONSTR_REF;
		value_data.type = PRO_VALUE_TYPE_SELECTION;
		value_data.v.r =  base_model_select;
		ProElement asm_const2_Ref_elem;
		isis::isis_AddElementtoElemTree(elemid, &comp_constr_elem,&value_data, &asm_const2_Ref_elem);


		// In case of joints, the ORIENT information is interpreted differently, it takes the ProConnectionFlipState on the AXIS to indicate flippedness
		// of the joint
		int flip = PRO_ASM_NOT_FLIPPED;
		if (in_ContraintDef.pro_datum_type==PRO_SURFACE)
		{
			flip = PRO_ASM_FLIP_UNDEFINED;
		}

		////////////////////////////////////////
		// Add a PRO_E_COMPONENT_ASSM_ORIENT 
		////////////////////////////////////////
		elemid = PRO_E_COMPONENT_ASSM_ORIENT ;
		value_data.type = PRO_VALUE_TYPE_INT;
		value_data.v.i = ((in_JointType ==  REVOLUTE_JOINT || in_JointType == PRISMATIC_JOINT)&&!guide)?flip:in_ContraintDef.base_model_datum_side; 
		ProElement base_model_datum_side_elem;
		isis::isis_AddElementtoElemTree(elemid, &comp_constr_elem, &value_data, &base_model_datum_side_elem);

		////////////////////////////////////////
		// Add a PRO_E_COMPONENT_COMP_ORIENT 
		////////////////////////////////////////
		elemid = PRO_E_COMPONENT_COMP_ORIENT ;
		value_data.type = PRO_VALUE_TYPE_INT;
		value_data.v.i = ((in_JointType ==  REVOLUTE_JOINT || in_JointType == PRISMATIC_JOINT)&&!guide)?flip:in_ContraintDef.added_model_datum_side; 
		ProElement added_model_datum_side_elem;
		isis::isis_AddElementtoElemTree(elemid, &comp_constr_elem, &value_data, &added_model_datum_side_elem);

		////////////////////////////////////////
		// Add a PRO_E_COMPONENT_CONSTR_SET_ID 
		////////////////////////////////////////
		elemid = PRO_E_COMPONENT_CONSTR_SET_ID ;
		value_data.type = PRO_VALUE_TYPE_INT;
		value_data.v.i = setidx; // ?????The set Ids are the index into the array of sets.  Therefore, if setID==1, then this 
													//  would be the first entry in the set array and would be at offset 0;

		ProElement comp_constr_set_id;
		isis::isis_AddElementtoElemTree(elemid, &comp_constr_elem,&value_data, &comp_constr_set_id);
	
		// snyako@isis.vanderbilt.edu: At some point this "magic formula" was needed to proper assembling
		// It seems like it's not required anymore
		if (true)
		{
			////////////////////////////////////////
			// Add a PRO_E_COMPONENT_CONSTR_ATTR 
			////////////////////////////////////////
			// The following WAS necessary to get the excavator to work.
			elemid = PRO_E_COMPONENT_CONSTR_ATTR;
			value_data.type = PRO_VALUE_TYPE_INT;
			if (in_ContraintDef.pro_datum_type == PRO_AXIS)
			{
				value_data.v.i = ((axiscounter==0||setidx==0)&&!guide)?2:3;
				
			} else if (in_ContraintDef.pro_datum_type == PRO_SURFACE)
			{
				value_data.v.i = (surfacecounter==0||guide)?0:1;
			}
			ProElement comp_constr_attr;
			isis::isis_AddElementtoElemTree(elemid, &comp_constr_elem,&value_data, &comp_constr_attr);
		}
		
}

ProAsmcompConstrType Flip_MateToAlign_or_AlignToMate ( ProAsmcompConstrType in_ConstraintType)
{
	switch ( in_ConstraintType)
	{
		case PRO_ASM_MATE:
			return PRO_ASM_ALIGN;
			break;
		case PRO_ASM_ALIGN:
			return PRO_ASM_MATE;
			break;
		default:
			return in_ConstraintType;
	}
}


void Add_PRO_E_COMPONENT_CONSTRAINT_2(	ProElement &out_constrs_elem, 
										const ContraintFeatureDefinition_2 &in_ContraintDef, 
										ProSolid &in_TopAssemblyModel,
										int setidx )
														throw (isis::application_exception)
{

		

		std::stringstream str;
		isis_LOG(lg, isis_FILE, isis_INFO) << "Add_PRO_E_COMPONENT_CONSTRAINT >>>>>>>>>>>>>>>";
		//isis_LOG(lg, isis_FILE, isis_INFO) << "\n**** Axis Constraint Index: " <<  counter << "  ******";
		stream_PopulateOneConstraintInConstraintStructure_2(
						&in_TopAssemblyModel,	// typedef struct sld_part* ProSolid;
						in_ContraintDef.base_model_path_list,		
						in_ContraintDef.added_model_path_list,
						in_ContraintDef.pro_datum_type,				// enum PRO_SURFACE, PRO_AXIS
						in_ContraintDef.base_model_datum_name,		// ASM_RIGHT, A_1..
						in_ContraintDef.base_model_datum_side,		// enum PRO_DATUM_SIDE_YELLOW (SIDE_A), PRO_DATUM_SIDE_RED (SIDE_B), PRO_DATUM_SIDE_NONE
						in_ContraintDef.added_model_datum_name,		// RIGHT, A23 ..
						in_ContraintDef.added_model_datum_side,		// enum PRO_DATUM_SIDE_YELLOW (SIDE_A), PRO_DATUM_SIDE_RED (SIDE_B), PRO_DATUM_SIDE_NONE
						in_ContraintDef.constraint_type,				// enum PRO_ASM_ALIGN, PRO_ASM_ALIGN_OFF...
						in_ContraintDef.offset_between_datums,		// This is only used if in_constraint_type == PRO_ASM_ALIGN_OFF or PRO_ASM_MATE_OFF
						in_ContraintDef.flip_orientation,
						str);
			
			isis_LOG(lg, isis_FILE, isis_INFO) << str.str();


			isis_LOG(lg, isis_FILE, isis_INFO);
		///////////////////////////////////
		// Add PRO_E_COMPONENT_CONSTRAINT
		//////////////////////////////////
		ProElement comp_constr_elem;

		isis::isis_ProElementAlloc (PRO_E_COMPONENT_CONSTRAINT, &comp_constr_elem);  
		isis::isis_ProElemtreeElementAdd (out_constrs_elem, NULL, comp_constr_elem);

		///////////////////////////////////
		// Add PRO_E_COMPONENT_CONSTR_TYPE
		///////////////////////////////////
		// From proasmcomp.h
		// PRO_E_COMPONENT_CONSTR_TYPE           Type           PRO_VALUE_TYPE_INT       See ProAsmcompConstrType
		//
		//   Constraint Type     Valid Reference Types
		//   ---------------------------------------------------------------------
		//   PRO_ASM_MATE          PRO_SURFACE (Plane, Cone, Torus, Cylinder)
		//   PRO_ASM_MATE_OFF      PRO_SURFACE (Plane)
		//   PRO_ASM_ALIGN         PRO_POINT, PRO_AXIS, PRO_SURFACE (Plane)
		//   PRO_ASM_ALIGN_OFF     PRO_POINT, PRO_AXIS, PRO_SURFACE (Plane)
		//   PRO_ASM_INSERT        PRO_SURFACE (Cylinder, Cone, Torus)
		//   PRO_ASM_ORIENT        PRO_SURFACE (Plane), PRO_CSYS
		//	 PRO_ASM_CSYS          PRO_CSYS

		ProElemId elemid;
		ProValueData value_data;
		elemid = PRO_E_COMPONENT_CONSTR_TYPE;
		value_data.type = PRO_VALUE_TYPE_INT;
		ProAsmcompConstrType constrtype = in_ContraintDef.constraint_type;
		ProConnectionFlipState flipState = PRO_ASM_FLIP_UNDEFINED;

		switch ( in_ContraintDef.pro_datum_type  )
		{
			case PRO_SURFACE:
				// Mate/Align	Side	Side	Should Be
				//	Mate		A		A		Mate A A
				//	Mate		A		B		Algin A A
				//	Mate		B		B		Mate A A
				//	Align		A		A		Align A A
				//	Align		A		B		Mate A A
				//	Align		B		B		Align A A
				// PRO_E_COMPONENT_COMP_ORIENT/ASSM_ORIENT should both be PRO_ASM_NOT_FLIPPED.
				if ( in_ContraintDef.constraint_type == PRO_ASM_MATE )
				{
					if ( in_ContraintDef.base_model_datum_side == in_ContraintDef.added_model_datum_side ) 
						constrtype = PRO_ASM_MATE;
					else
						constrtype = PRO_ASM_ALIGN;
				}
				else
				{
					if ( in_ContraintDef.base_model_datum_side == in_ContraintDef.added_model_datum_side ) 
						constrtype = PRO_ASM_ALIGN;
					else
						constrtype = PRO_ASM_MATE;

				}

				flipState = PRO_ASM_NOT_FLIPPED;

				break;
			case PRO_POINT:
				constrtype = PRO_ASM_ALIGN;   // 6/19/2015 Correct, PRO_E_COMPONENT_COMP_ORIENT/ASSM_ORIENT should both be PRO_ASM_FLIPPED 
											  // PRO_ASM_FLIPPED does not make sense, but this was derived by dumping feature trees.
											  // PRO_ASM_FLIPPED is probably ignored
				flipState = PRO_ASM_FLIPPED;
				break;

			case PRO_AXIS:
				constrtype = PRO_ASM_ALIGN;  // 6/19/2015 Correct, PRO_E_COMPONENT_COMP_ORIENT/ASSM_ORIENT should both be either
											 // PRO_ASM_NOT_FLIPPED or PRO_ASM_FLIPPED.  If PRO_ASM_FLIPPED this means the axes should 
											 // be poiting in the opposite direction
				if (in_ContraintDef.flip_orientation )   // Would only be set for axes
				{
					flipState = PRO_ASM_FLIPPED;
					isis_LOG(lg, isis_FILE, isis_INFO) << "PRO_E_COMPONENT_CONSTR_ATTR:              PRO_ASM_FLIPPED";
				}
				else
				{
					flipState = PRO_ASM_NOT_FLIPPED;
					isis_LOG(lg, isis_FILE, isis_INFO) << "PRO_E_COMPONENT_CONSTR_ATTR:              PRO_ASM_NOT_FLIPPED";
				}
				break;

			case PRO_CSYS:
				constrtype = PRO_ASM_CSYS; // 6/19/2015 Correct, PRO_E_COMPONENT_COMP_ORIENT/ASSM_ORIENT should both be PRO_ASM_FLIPPED 
											  // PRO_ASM_FLIPPED does not make sense, but this was derived by dumping feature trees.
											  // PRO_ASM_FLIPPED is probably ignored
				flipState = PRO_ASM_FLIPPED;
				break;
			default:
				constrtype = PRO_ASM_ALIGN;
				flipState = PRO_ASM_NOT_FLIPPED;
		}

		value_data.v.i = constrtype;
		ProElement constr1_type_elem;
		isis::isis_AddElementtoElemTree(elemid, &comp_constr_elem,&value_data, &constr1_type_elem);

		/////////////////////////////////////////
		// Add PRO_E_COMPONENT_COMP_CONSTR_REF
		/////////////////////////////////////////
	
		ProSelection base_model_select;
		ProSelection added_model_select;

		RetrieveProSelectionForGeometry(in_TopAssemblyModel,					// Used as the starting point of model_path_list
										in_ContraintDef.base_model_path_list,	// Used to locate the model in the assembly containing the geometry
										in_ContraintDef.base_model_component_instance_ID,	// Used for logging error messages
										in_ContraintDef.base_model_name,		// Used for logging error messages
										in_ContraintDef.base_model_type,		// Used for logging error messages
										in_ContraintDef.pro_datum_type,			// Identifies the geometry type, e.g. Datum
										in_ContraintDef.base_model_datum_name,	// Geometry Name, e.g. Datum Name
										base_model_select);						// Returned pro_selection

		RetrieveProSelectionForGeometry(in_TopAssemblyModel,					// Used as the starting point of model_path_list
										in_ContraintDef.added_model_path_list,	// Used to locate the model in the assembly containing the geometry
										in_ContraintDef.added_model_component_instance_ID,	// Used for logging error messages
										in_ContraintDef.added_model_name,		// Used for logging error messages
										in_ContraintDef.added_model_type,		// Used for logging error messages
										in_ContraintDef.pro_datum_type,			// Identifies the geometry type, e.g. Datum
										in_ContraintDef.added_model_datum_name,	// Geometry Name, e.g. Datum Name
										added_model_select);					// Returned pro_selection

		//////////////////////////////////////
		// Add PRO_E_COMPONENT_COMP_CONSTR_REF
		//////////////////////////////////////
		elemid = PRO_E_COMPONENT_COMP_CONSTR_REF ;
		value_data.type = PRO_VALUE_TYPE_SELECTION;
		value_data.v.r =  added_model_select;
		ProElement comp_const1_Ref_elem;
		isis::isis_AddElementtoElemTree(elemid, &comp_constr_elem,&value_data, &comp_const1_Ref_elem);

		//////////////////////////////////////
		// Add PRO_E_COMPONENT_ASSEM_CONSTR_REF
		//////////////////////////////////////
		elemid = PRO_E_COMPONENT_ASSEM_CONSTR_REF;
		value_data.type = PRO_VALUE_TYPE_SELECTION;
		value_data.v.r =  base_model_select;
		ProElement asm_const2_Ref_elem;
		isis::isis_AddElementtoElemTree(elemid, &comp_constr_elem,&value_data, &asm_const2_Ref_elem);
	
		elemid = PRO_E_COMPONENT_ASSM_ORIENT ;
		value_data.type = PRO_VALUE_TYPE_INT;
		value_data.v.i = flipState; 
		ProElement base_model_datum_side_elem;
		isis::isis_AddElementtoElemTree(elemid, &comp_constr_elem, &value_data, &base_model_datum_side_elem);

		////////////////////////////////////////
		// Add a PRO_E_COMPONENT_COMP_ORIENT 
		////////////////////////////////////////

		elemid = PRO_E_COMPONENT_COMP_ORIENT ;
		value_data.type = PRO_VALUE_TYPE_INT;
		value_data.v.i = flipState; 
		ProElement added_model_datum_side_elem;
		isis::isis_AddElementtoElemTree(elemid, &comp_constr_elem, &value_data, &added_model_datum_side_elem);

		////////////////////////////////////////
		// Add a PRO_E_COMPONENT_CONSTR_SET_ID 
		////////////////////////////////////////
		elemid = PRO_E_COMPONENT_CONSTR_SET_ID ;
		value_data.type = PRO_VALUE_TYPE_INT;
		value_data.v.i = setidx; // ?????The set Ids are the index into the array of sets.  Therefore, if setID==1, then this 
													//  would be the first entry in the set array and would be at offset 0;

		ProElement comp_constr_set_id;
		isis::isis_AddElementtoElemTree(elemid, &comp_constr_elem,&value_data, &comp_constr_set_id);

		elemid = PRO_E_COMPONENT_CONSTR_ATTR;
		value_data.type = PRO_VALUE_TYPE_INT;
		value_data.v.i = 0;
		ProElement comp_constr_attr;
		isis::isis_AddElementtoElemTree(elemid, &comp_constr_elem,&value_data, &comp_constr_attr);		
}

void SetupRotationLimits(ProSolid parentAssembly, const std::vector<ContraintFeatureDefinition> &constraintdefs, ProElement sets_elem_tree, int setid, const InputJoint& joint)
{

		const InputJoint::LimitRef *base_limitrefs;
		const InputJoint::LimitRef *added_limitrefs;

		if (joint.RotationLimitRefs_This[InputJoint::DEFAULT].ComponentID == constraintdefs[0].base_model_component_instance_ID &&
			joint.RotationLimitRefs_Other[InputJoint::DEFAULT].ComponentID == constraintdefs[0].added_model_component_instance_ID)
		{
			base_limitrefs = joint.RotationLimitRefs_This;
			added_limitrefs = joint.RotationLimitRefs_Other;
		}
		else if (joint.RotationLimitRefs_Other[InputJoint::DEFAULT].ComponentID == constraintdefs[0].base_model_component_instance_ID &&
			     joint.RotationLimitRefs_This[InputJoint::DEFAULT].ComponentID == constraintdefs[0].added_model_component_instance_ID)
		{
			base_limitrefs = joint.RotationLimitRefs_Other;
			added_limitrefs = joint.RotationLimitRefs_This;
		}
		else
		{
			// This joint is not between these components
			return;
		}

		


	//isis_LOG(lg, isis_FILE, isis_ERROR) << constraintdefs[0].ToString();
		ProElempathItem set_path[] = 
			{{PRO_ELEM_PATH_ITEM_TYPE_ID, PRO_E_COMPONENT_SETS},
			{PRO_ELEM_PATH_ITEM_TYPE_INDEX, setid-1}};

		// Get PRO_E_COMPONENT_SETS 
		ProElement set_elem;
		ProElempath   setPath;
		isis::isis_ProElempathAlloc (&setPath);			
		isis::isis_ProElempathDataSet (setPath, set_path, 2);

		isis::isis_ProElemtreeElementGet (sets_elem_tree, setPath, &set_elem);

		ProReference comp_ref;
		ProReference asm_ref;

		ProReferenceAlloc(&comp_ref);
		ProReferenceAlloc(&asm_ref);

		ProAsmcomppath	base_comp_path;
		isis::Retrieve_ProAsmcomppath_WithExceptions(parentAssembly, constraintdefs[0].base_model_path_list, base_comp_path);

		ProAsmcomppath	added_comp_path;
		isis::Retrieve_ProAsmcomppath_WithExceptions(parentAssembly, constraintdefs[0].added_model_path_list, added_comp_path);

		ProModelitem base_model_item;
		ProModelitem added_model_item;


		MultiFormatString featurename_base(base_limitrefs[InputJoint::DEFAULT].FeatureName);
		MultiFormatString featurename_added(added_limitrefs[InputJoint::DEFAULT].FeatureName);

		isis_ProModelitemByNameInit(constraintdefs[0].p_base_model, FeatureGeometryType_enum(base_limitrefs[InputJoint::DEFAULT].FeatureType), featurename_base, &base_model_item);
		isis_ProModelitemByNameInit(constraintdefs[0].p_added_model, FeatureGeometryType_enum(added_limitrefs[InputJoint::DEFAULT].FeatureType), featurename_added, &added_model_item);


		ProError err = ProReferenceSet(asm_ref, &base_comp_path, &base_model_item);
		if (err != PRO_TK_NO_ERROR) throw isis::application_exception("Error: ProReferenceSet, in Function: SetupRotationLimits");
		err = ProReferenceSet(comp_ref, &added_comp_path, &added_model_item);
		if (err != PRO_TK_NO_ERROR) throw isis::application_exception("Error: ProReferenceSet, in Function: SetupRotationLimits_2");

		//isis_LOG(lg, isis_FILE, isis_WARN) << "Setting rotation limits on: " << constraintdefs[0].base_model_component_instance_ID << ", " << constraintdefs[0].added_model_component_instance_ID;

		Set_Limits_On_Set(set_elem, comp_ref, asm_ref, joint, joint.RotationLimits);

		// I don't know what can be freed
		//ProReferenceFree(&comp_ref);
		//ProReferenceFree(&asm_ref);
		//ProElempathFree(&setPath);

		//ProElemtreeWrite(sets_elem_tree, PRO_ELEMTREE_XML, L"SetupRotationLimits_2.xml");
}

void SetupRotationLimits_2(	ProSolid parentAssembly, 
							const ContraintFeatureDefinition_2 &constraintdef, 
							ProElement sets_elem_tree, 
							int in_SetID_index, 
							const InputJoint& joint)
{

		const InputJoint::LimitRef *base_limitrefs;
		const InputJoint::LimitRef *added_limitrefs;

		if (joint.RotationLimitRefs_This[InputJoint::DEFAULT].ComponentID == constraintdef.base_model_component_instance_ID &&
			joint.RotationLimitRefs_Other[InputJoint::DEFAULT].ComponentID == constraintdef.added_model_component_instance_ID)
		{
			base_limitrefs = joint.RotationLimitRefs_This;
			added_limitrefs = joint.RotationLimitRefs_Other;
		}
		else if (joint.RotationLimitRefs_Other[InputJoint::DEFAULT].ComponentID == constraintdef.base_model_component_instance_ID &&
			     joint.RotationLimitRefs_This[InputJoint::DEFAULT].ComponentID == constraintdef.added_model_component_instance_ID)
		{
			base_limitrefs = joint.RotationLimitRefs_Other;
			added_limitrefs = joint.RotationLimitRefs_This;
		}
		else
		{
			// This joint is not between these components
			return;
		}

		


	//isis_LOG(lg, isis_FILE, isis_ERROR) << constraintdefs[0].ToString();
		ProElempathItem set_path[] = 
			{{PRO_ELEM_PATH_ITEM_TYPE_ID, PRO_E_COMPONENT_SETS},
			{PRO_ELEM_PATH_ITEM_TYPE_INDEX, in_SetID_index}};

		// Get PRO_E_COMPONENT_SETS 
		ProElement set_elem;
		ProElempath   setPath;
		isis::isis_ProElempathAlloc (&setPath);			
		isis::isis_ProElempathDataSet (setPath, set_path, 2);

		isis::isis_ProElemtreeElementGet (sets_elem_tree, setPath, &set_elem);

		ProReference comp_ref;
		ProReference asm_ref;

		ProReferenceAlloc(&comp_ref);
		ProReferenceAlloc(&asm_ref);

		ProAsmcomppath	base_comp_path;
		isis::Retrieve_ProAsmcomppath_WithExceptions(parentAssembly, constraintdef.base_model_path_list, base_comp_path);

		ProAsmcomppath	added_comp_path;
		isis::Retrieve_ProAsmcomppath_WithExceptions(parentAssembly, constraintdef.added_model_path_list, added_comp_path);

		ProModelitem base_model_item;
		ProModelitem added_model_item;


		MultiFormatString featurename_base(base_limitrefs[InputJoint::DEFAULT].FeatureName);
		MultiFormatString featurename_added(added_limitrefs[InputJoint::DEFAULT].FeatureName);

		isis_ProModelitemByNameInit(constraintdef.p_base_model, FeatureGeometryType_enum(base_limitrefs[InputJoint::DEFAULT].FeatureType), featurename_base, &base_model_item);
		isis_ProModelitemByNameInit(constraintdef.p_added_model, FeatureGeometryType_enum(added_limitrefs[InputJoint::DEFAULT].FeatureType), featurename_added, &added_model_item);


		ProError err = ProReferenceSet(asm_ref, &base_comp_path, &base_model_item);
		if (err != PRO_TK_NO_ERROR) throw isis::application_exception("Error: ProReferenceSet, in Function: SetupRotationLimits_2");
		err = ProReferenceSet(comp_ref, &added_comp_path, &added_model_item);
		if (err != PRO_TK_NO_ERROR) throw isis::application_exception("Error: ProReferenceSet, in Function: SetupRotationLimits_2");

		//isis_LOG(lg, isis_FILE, isis_WARN) << "Setting rotation limits on: " << constraintdefs[0].base_model_component_instance_ID << ", " << constraintdefs[0].added_model_component_instance_ID;

		Set_Limits_On_Set(set_elem, comp_ref, asm_ref, joint, joint.RotationLimits);

		// I don't know what can be freed
		//ProReferenceFree(&comp_ref);
		//ProReferenceFree(&asm_ref);
		//ProElempathFree(&setPath);

		//ProElemtreeWrite(sets_elem_tree, PRO_ELEMTREE_XML, L"SetupRotationLimits_2.xml");
}


void SetupTranslationLimits(ProSolid parentAssembly, const std::vector<ContraintFeatureDefinition> &constraintdefs, ProElement sets_elem_tree, int setid, const InputJoint& joint)
{
		const InputJoint::LimitRef *base_limitrefs;
		const InputJoint::LimitRef *added_limitrefs;

		if (joint.TranslationLimitRefs_This[InputJoint::DEFAULT].ComponentID == constraintdefs[0].base_model_component_instance_ID &&
			joint.TranslationLimitRefs_Other[InputJoint::DEFAULT].ComponentID == constraintdefs[0].added_model_component_instance_ID)
		{
			base_limitrefs = joint.TranslationLimitRefs_This;
			added_limitrefs = joint.TranslationLimitRefs_Other;
		}
		else if (joint.TranslationLimitRefs_Other[InputJoint::DEFAULT].ComponentID == constraintdefs[0].base_model_component_instance_ID &&
			     joint.TranslationLimitRefs_This[InputJoint::DEFAULT].ComponentID == constraintdefs[0].added_model_component_instance_ID)
		{
			base_limitrefs = joint.TranslationLimitRefs_Other;
			added_limitrefs = joint.TranslationLimitRefs_This;
		}
		else
		{
			// This joint is not between these components
			return;
		}

		


	//isis_LOG(lg, isis_FILE, isis_ERROR) << constraintdefs[0].ToString();
		ProElempathItem set_path[] = 
			{{PRO_ELEM_PATH_ITEM_TYPE_ID, PRO_E_COMPONENT_SETS},
			{PRO_ELEM_PATH_ITEM_TYPE_INDEX, setid-1}};

		// Get PRO_E_COMPONENT_SETS 
		ProElement set_elem;
		ProElempath   setPath;
		isis::isis_ProElempathAlloc (&setPath);			
		isis::isis_ProElempathDataSet (setPath, set_path, 2);
		isis::isis_ProElemtreeElementGet (sets_elem_tree, setPath, &set_elem);

		ProReference comp_ref;
		ProReference asm_ref;

		ProReferenceAlloc(&comp_ref);
		ProReferenceAlloc(&asm_ref);

		ProAsmcomppath	base_comp_path;
		isis::Retrieve_ProAsmcomppath_WithExceptions(parentAssembly, constraintdefs[0].base_model_path_list, base_comp_path);

		ProAsmcomppath	added_comp_path;
		isis::Retrieve_ProAsmcomppath_WithExceptions(parentAssembly, constraintdefs[0].added_model_path_list, added_comp_path);

		ProModelitem base_model_item;
		ProModelitem added_model_item;

		MultiFormatString featurename_base(base_limitrefs[InputJoint::DEFAULT].FeatureName);
		MultiFormatString featurename_added(added_limitrefs[InputJoint::DEFAULT].FeatureName);

		isis_ProModelitemByNameInit(constraintdefs[0].p_base_model, FeatureGeometryType_enum(base_limitrefs[InputJoint::DEFAULT].FeatureType), featurename_base, &base_model_item);
		isis_ProModelitemByNameInit(constraintdefs[0].p_added_model, FeatureGeometryType_enum(added_limitrefs[InputJoint::DEFAULT].FeatureType), featurename_added, &added_model_item);


		ProError err = ProReferenceSet(asm_ref, &base_comp_path, &base_model_item);
		if (err != PRO_TK_NO_ERROR) throw isis::application_exception("Error: ProReferenceSet, in Function: SetupTranslationLimits");
		err = ProReferenceSet(comp_ref, &added_comp_path, &added_model_item);
		if (err != PRO_TK_NO_ERROR) throw isis::application_exception("Error: ProReferenceSet, in Function: SetupTranslationLimits");

		//isis_LOG(lg, isis_FILE, isis_WARN) << "Setting translation limits on: " << constraintdefs[0].base_model_component_instance_ID << ", " << constraintdefs[0].added_model_component_instance_ID;

		Set_Limits_On_Set(set_elem, comp_ref, asm_ref, joint, joint.TranslationLimits);

		// I don't know what can be freed
		//ProReferenceFree(&comp_ref);
		//ProReferenceFree(&asm_ref);
		//ProElempathFree(&setPath);

		//ProElemtreeWrite(sets_elem_tree, PRO_ELEMTREE_XML, L"SetupTranslationLimits.xml");
}

void SetupTranslationLimits_2(	ProSolid parentAssembly, 
								const ContraintFeatureDefinition_2 &constraintdef, 
								ProElement sets_elem_tree, 
								int in_SetID_index,     // if the setID == 1, then in_SetID_index = 0
								const InputJoint& joint )
{
		const InputJoint::LimitRef *base_limitrefs;
		const InputJoint::LimitRef *added_limitrefs;

		if (joint.TranslationLimitRefs_This[InputJoint::DEFAULT].ComponentID == constraintdef.base_model_component_instance_ID &&
			joint.TranslationLimitRefs_Other[InputJoint::DEFAULT].ComponentID == constraintdef.added_model_component_instance_ID)
		{
			base_limitrefs = joint.TranslationLimitRefs_This;
			added_limitrefs = joint.TranslationLimitRefs_Other;
		}
		else if (joint.TranslationLimitRefs_Other[InputJoint::DEFAULT].ComponentID == constraintdef.base_model_component_instance_ID &&
			     joint.TranslationLimitRefs_This[InputJoint::DEFAULT].ComponentID == constraintdef.added_model_component_instance_ID)
		{
			base_limitrefs = joint.TranslationLimitRefs_Other;
			added_limitrefs = joint.TranslationLimitRefs_This;
		}
		else
		{
			// This joint is not between these components
			return;
		}

		


	//isis_LOG(lg, isis_FILE, isis_ERROR) << constraintdef.ToString();
		ProElempathItem set_path[] = 
			{{PRO_ELEM_PATH_ITEM_TYPE_ID, PRO_E_COMPONENT_SETS},
			{PRO_ELEM_PATH_ITEM_TYPE_INDEX, in_SetID_index}};

		// Get PRO_E_COMPONENT_SETS 
		ProElement set_elem;
		ProElempath   setPath;
		isis::isis_ProElempathAlloc (&setPath);			
		isis::isis_ProElempathDataSet (setPath, set_path, 2);
		isis::isis_ProElemtreeElementGet (sets_elem_tree, setPath, &set_elem);

		ProReference comp_ref;
		ProReference asm_ref;

		ProReferenceAlloc(&comp_ref);
		ProReferenceAlloc(&asm_ref);

		ProAsmcomppath	base_comp_path;
		isis::Retrieve_ProAsmcomppath_WithExceptions(parentAssembly, constraintdef.base_model_path_list, base_comp_path);

		ProAsmcomppath	added_comp_path;
		isis::Retrieve_ProAsmcomppath_WithExceptions(parentAssembly, constraintdef.added_model_path_list, added_comp_path);

		ProModelitem base_model_item;
		ProModelitem added_model_item;

		MultiFormatString featurename_base(base_limitrefs[InputJoint::DEFAULT].FeatureName);
		MultiFormatString featurename_added(added_limitrefs[InputJoint::DEFAULT].FeatureName);

		isis_ProModelitemByNameInit(constraintdef.p_base_model, FeatureGeometryType_enum(base_limitrefs[InputJoint::DEFAULT].FeatureType), featurename_base, &base_model_item);
		isis_ProModelitemByNameInit(constraintdef.p_added_model, FeatureGeometryType_enum(added_limitrefs[InputJoint::DEFAULT].FeatureType), featurename_added, &added_model_item);


		ProError err = ProReferenceSet(asm_ref, &base_comp_path, &base_model_item);
		if (err != PRO_TK_NO_ERROR) throw isis::application_exception("Error: ProReferenceSet, in Function: SetupTranslationLimits_2");
		err = ProReferenceSet(comp_ref, &added_comp_path, &added_model_item);
		if (err != PRO_TK_NO_ERROR) throw isis::application_exception("Error: ProReferenceSet, in Function: SetupTranslationLimits_2");

		//isis_LOG(lg, isis_FILE, isis_WARN) << "Setting translation limits on: " << constraintdef.base_model_component_instance_ID << ", " << constraintdef.added_model_component_instance_ID;

		Set_Limits_On_Set(set_elem, comp_ref, asm_ref, joint, joint.TranslationLimits);

		// I don't know what can be freed
		//ProReferenceFree(&comp_ref);
		//ProReferenceFree(&asm_ref);
		//ProElempathFree(&setPath);

		//ProElemtreeWrite(sets_elem_tree, PRO_ELEMTREE_XML, L"SetupTranslationLimits.xml");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Description:
//		Add constraints to the feature tree for in_ComponentID.
// Pre-Conditions:
//		The feature tree for in_CADComponentData_map[in_ComponentID] must NOT contain any constraints.  It is assumed that
//		we are starting with a feature tree that only contains 
//			<PRO_E_FEATURE_TYPE
//			<PRO_E_COMPONENT_MODEL
//			<PRO_E_COMPONENT_INIT_POS
//			The other fields should not be set
//		
//		in_TopAssembly must point to the top-most assembly.  This is not necessarily the parent of in_ComponentID.
//		in_ComponentID must be the model that this function constrains.
//
// Post-Conditions
//		Complete feature tree with 
//			Constraint sets set with the name Set1, Set2, ... where the integer is the PerSetConstraintDefinition::setID
//			Constraints set per in_ContraintFeatureDefinitions.  The constraint will belong to the set ID as
//			specified in in_PerSetConstraintDefinitions.
//
//		If an error occurs, isis::application_exception would be thrown.
void SetConstraints (	ProSolid									&in_TopAssembly, 
						const std::string							&in_ComponentID, 
						std::map<string, isis::CADComponentData>	&in_CADComponentData_map , 
						//int in_MaxSetID,  // MaxSetIDs are numbered 1, 2, 3... so if in_MaxSetID = 2, there are two set IDs (1 and 2)
						const std::vector<PerSetConstraintDefinition> &in_PerSetConstraintDefinitions,
						bool in_1stRound,
						std::string  &out_Warnings )
								throw (isis::application_exception)
{

	ProAsmcompconstraint *proAsmcompconstraint_dummy_not_used = NULL;
	

	isis_LOG(lg, isis_FILE, isis_INFO) << "";
	isis_LOG(lg, isis_FILE, isis_INFO) << "********** Begin SetConstraints, Constraint Definitions  ***************";
	
	int constraint_counter = 0;

	/////////////////////////////////////
	// Log Constraint Information
	/////////////////////////////////////
	for each ( const PerSetConstraintDefinition  &i_PerSetConstaint in in_PerSetConstraintDefinitions )
	{
		isis_LOG(lg, isis_FILE, isis_INFO);
		isis_LOG(lg, isis_FILE, isis_INFO) << "\n**** Constraint Index: " <<  constraint_counter << "  ******";
		isis_LOG(lg, isis_FILE, isis_INFO) << "setID:                   " <<  i_PerSetConstaint.setID;
		isis_LOG(lg, isis_FILE, isis_INFO) << "hasAGuideConstraint:     " <<  i_PerSetConstaint.hasAGuideConstraint;
		isis_LOG(lg, isis_FILE, isis_INFO) << "jointType_withoutguide:  " <<  CADJointType_string(i_PerSetConstaint.jointType_withoutguide);
		isis_LOG(lg, isis_FILE, isis_INFO) << "jointType_withguide:     " <<  CADJointType_string(i_PerSetConstaint.jointType_withguide);

		for each ( const ContraintFeatureDefinition &j_constraint in i_PerSetConstaint.contraintFeatureDefinitions )
		{
			std::stringstream str;
			stream_PopulateOneConstraintInConstraintStructure(
							&in_TopAssembly,	// typedef struct sld_part* ProSolid;
							j_constraint.base_model_path_list,		
							j_constraint.added_model_path_list,
							j_constraint.pro_datum_type,				// enum PRO_SURFACE, PRO_AXIS
							j_constraint.base_model_datum_name,		// ASM_RIGHT, A_1..
							j_constraint.base_model_datum_side,		// enum PRO_DATUM_SIDE_YELLOW (SIDE_A), PRO_DATUM_SIDE_RED (SIDE_B), PRO_DATUM_SIDE_NONE
							j_constraint.added_model_datum_name,		// RIGHT, A23 ..
							j_constraint.added_model_datum_side,		// enum PRO_DATUM_SIDE_YELLOW (SIDE_A), PRO_DATUM_SIDE_RED (SIDE_B), PRO_DATUM_SIDE_NONE
							j_constraint.constraint_type,				// enum PRO_ASM_ALIGN, PRO_ASM_ALIGN_OFF...
							j_constraint.offset_between_datums,		// This is only used if in_constraint_type == PRO_ASM_ALIGN_OFF or PRO_ASM_MATE_OFF
							str);
			
				isis_LOG(lg, isis_FILE, isis_INFO) << str.str();
				isis_LOG(lg, isis_FILE, isis_INFO) << "1st Round: " << in_1stRound;
				isis_LOG(lg, isis_FILE, isis_INFO);
				++constraint_counter;
		}
	}

	ProElemId elemid;
	ProElement elem_tree;
	ProValueData value_data;	

	/////////////////////////////////////////
	// Setup the path to the parent assembly
	/////////////////////////////////////////
	ProAsmcomppath	assembly_comp_path;
	Retrieve_ProAsmcomppath_WithExceptions(in_TopAssembly, in_CADComponentData_map[in_ComponentID].componentPaths, assembly_comp_path);

	////////////////////////////////
	// Extract PRO_E_FEATURE_TREE
	////////////////////////////////
	isis::isis_ProElementAlloc(PRO_E_FEATURE_TREE, &elem_tree);

	ProAsmcomp					assembledFeature_temp;
	//assembledFeature_temp.type =	FeatureGeometryType_enum(in_CADComponentData_map[in_ComponentID].assembledFeature.type);
	//assembledFeature_temp.id   =	                         in_CADComponentData_map[in_ComponentID].assembledFeature.id;
	//assembledFeature_temp.owner =	                         in_CADComponentData_map[in_ComponentID].assembledFeature.owner; 	
	assembledFeature_temp = getProAsmcomp(in_CADComponentData_map[in_ComponentID].assembledFeature);
	
	isis::isis_ProFeatureElemtreeExtract (	&assembledFeature_temp,
											&assembly_comp_path,
											PRO_FEAT_EXTRACT_NO_OPTS,
											&elem_tree );
	
	//std::string featureTreeFileName_narrow = in_CADComponentData_map[in_ComponentID].name;
	//featureTreeFileName_narrow += "_1.xml";
	// For family table names, replace <>  e.g. Chassis_8_Wheel<Chassis> would become Chassis_8_Wheel_Chassis_
	//boost::replace_all(featureTreeFileName_narrow, "<", "_");
	//boost::replace_all(featureTreeFileName_narrow, ">", "_");
	//isis::MultiFormatString  featureTreeFileName_multi( featureTreeFileName_narrow);
	//isis::isis_ProElemtreeWrite(elem_tree, PRO_ELEMTREE_XML, (wchar_t*)(const wchar_t*)featureTreeFileName_multi);	

	////////////////////////////////
	// Add Sets 
	////////////////////////////////

	// Get PRO_E_COMPONENT_SETS 
	ProElement sets_elem;
	ProElempathItem sets_path_Item[] = {{PRO_ELEM_PATH_ITEM_TYPE_ID, PRO_E_COMPONENT_SETS}};
	ProElempath   setsPath;
	isis::isis_ProElempathAlloc (&setsPath);			
	isis::isis_ProElempathDataSet (setsPath, sets_path_Item, 1);
	isis::isis_ProElemtreeElementGet (elem_tree, setsPath, &sets_elem);

	if (in_1stRound)
	{
		std::string SetName_narrorw = "Set1";

		MultiFormatString SetName_multi(SetName_narrorw);


		Add_PRO_E_COMPONENT_SET( sets_elem, SetName_multi, 1, PRO_ASM_SET_USER_DEFINED_TYPE );
	} else {
		for each ( const PerSetConstraintDefinition  &i in in_PerSetConstraintDefinitions )
		{
			std::string SetName_narrorw = "Set" + std::to_string((long long)i.setID);

			MultiFormatString SetName_multi(SetName_narrorw);

			ProAsmcompSetType setType;

			if ( i.jointType_withoutguide == REVOLUTE_JOINT )
				setType = PRO_ASM_SET_TYPE_PIN;
			else if (i.jointType_withoutguide == FIXED_JOINT )
				setType = PRO_ASM_SET_TYPE_FIXED;
			else if (i.jointType_withoutguide == PRISMATIC_JOINT )
				setType = PRO_ASM_SET_TYPE_SLIDER;
			else
				setType = PRO_ASM_SET_USER_DEFINED_TYPE;

			Add_PRO_E_COMPONENT_SET( sets_elem, SetName_multi, i.setID, setType );

			InputJoint& inputJoint = in_CADComponentData_map[in_ComponentID].constraintDef.constraints[i.setID-1].inputJoint;

			// Setup rotation/translation limits
			if (i.jointType_withoutguide == REVOLUTE_JOINT && inputJoint.Type == REVOLUTE_JOINT)
			{
				if (inputJoint.RotationLimits[InputJoint::DEFAULT].Provided && inputJoint.RotationLimitRefs_This[InputJoint::DEFAULT].Provided && inputJoint.RotationLimitRefs_Other[InputJoint::DEFAULT].Provided)
					SetupRotationLimits(in_TopAssembly, i.contraintFeatureDefinitions, elem_tree, i.setID, inputJoint);
			}
		
			if (i.jointType_withoutguide == PRISMATIC_JOINT && inputJoint.Type == PRISMATIC_JOINT)
			{
				if (inputJoint.TranslationLimits[InputJoint::DEFAULT].Provided && inputJoint.TranslationLimitRefs_This[InputJoint::DEFAULT].Provided && inputJoint.TranslationLimitRefs_Other[InputJoint::DEFAULT].Provided)
					SetupTranslationLimits(in_TopAssembly, i.contraintFeatureDefinitions, elem_tree, i.setID, inputJoint);
			}
		}
	}

	///////////////////////////////////////
	// Get PRO_E_COMPONENT_CONSTRAINTS
	///////////////////////////////////////
	ProElement constrs_elem;
	ProElempathItem constrs_path_Item[] = {{PRO_ELEM_PATH_ITEM_TYPE_ID, PRO_E_COMPONENT_CONSTRAINTS}};
	ProElempath   constrsPath;
	isis::isis_ProElempathAlloc (&constrsPath);			
	isis::isis_ProElempathDataSet (constrsPath, constrs_path_Item, 1);
	isis::isis_ProElemtreeElementGet (elem_tree, constrsPath, &constrs_elem);

	int usedSetID = 0;
	int axiscounter=0; // Count how many constraints has been set up so far (needed for "ignore" magic formula)
	int surfacecounter=0; // Count how many constraints has been set up so far (needed for "ignore" magic formula)
	//for ( int i = 1; i <= in_MaxSetID; ++i )
	int loop_i = 0;
	for each ( const PerSetConstraintDefinition  &i in in_PerSetConstraintDefinitions )
	{
		++loop_i;

		e_CADJointType jointType;
		jointType = i.jointType_withoutguide;

		// R.O. I don't think a FIXED_JOINT should be included in the following if
		// WAS: if ( jointType == REVOLUTE_JOINT || jointType == FIXED_JOINT | jointType == PRISMATIC_JOINT )
		if ( !in_1stRound && ( jointType == REVOLUTE_JOINT  || jointType == PRISMATIC_JOINT ))
		{
			//Apply the Axes Constraints first
			for each ( const ContraintFeatureDefinition &j in i.contraintFeatureDefinitions )
			{
				if ( j.treatConstraintAsAGuide == CAD_TREAT_CONSTRAINT_AS_A_GUIDE_TRUE ) continue;
				
				if (j.pro_datum_type == PRO_AXIS)
				{
					Add_PRO_E_COMPONENT_CONSTRAINT( constrs_elem, j, in_TopAssembly, jointType, axiscounter++, surfacecounter, in_1stRound, usedSetID);
				}
			}

			// Apply the datum planes second
			for each ( const ContraintFeatureDefinition &j in i.contraintFeatureDefinitions )
			{
				if ( j.treatConstraintAsAGuide == CAD_TREAT_CONSTRAINT_AS_A_GUIDE_TRUE ) continue;

				if (j.pro_datum_type == PRO_SURFACE)
				{
					Add_PRO_E_COMPONENT_CONSTRAINT( constrs_elem, j, in_TopAssembly, jointType, axiscounter, surfacecounter++, in_1stRound, usedSetID);
				}
			}
			usedSetID++;
		}
		else
		{
			// Apply the contraints in the order as read from CADAssemlby.xml
			if (in_1stRound && loop_i>1) continue;
			for each ( const ContraintFeatureDefinition &j in i.contraintFeatureDefinitions )
			{
				if ( !in_1stRound && j.treatConstraintAsAGuide == CAD_TREAT_CONSTRAINT_AS_A_GUIDE_TRUE ) continue;

				if (j.pro_datum_type == PRO_SURFACE)
				{
					Add_PRO_E_COMPONENT_CONSTRAINT( constrs_elem, j, in_TopAssembly, jointType, axiscounter, surfacecounter++, in_1stRound, usedSetID);
				} else if (j.pro_datum_type == PRO_AXIS)
				{
					Add_PRO_E_COMPONENT_CONSTRAINT( constrs_elem, j, in_TopAssembly, jointType, axiscounter++, surfacecounter, in_1stRound, usedSetID);
				} else {
					Add_PRO_E_COMPONENT_CONSTRAINT( constrs_elem, j, in_TopAssembly, jointType, axiscounter, surfacecounter, in_1stRound, usedSetID);
				}

			}
			usedSetID++;

		}
	}  

	//string featureTreeFileName_narrow = (string)in_CADComponentData_map[in_ComponentID].name+"_1.xml";
	//MultiFormatString featureTreeFileName_multi = (string)featureTreeFileName_narrow;
	//ProElemtreeWrite(elem_tree, PRO_ELEMTREE_XML, (wchar_t*)(const wchar_t*)featureTreeFileName_multi);

	///////////////////////////////////////////////////////
	// Commit the element tree
	///////////////////////////////////////////////////////

	ProFeatureCreateOptions *opts;  
	isis::isis_ProArrayAlloc(2, sizeof (ProFeatureCreateOptions), 1, (ProArray*) &opts);
    opts[0] = PRO_FEAT_CR_DEFINE_MISS_ELEMS;   
	opts[1] = PRO_FEAT_CR_FIX_MODEL_ON_FAIL;

	ProErrorlist p_errors;


	isis::isis_ProFeatureWithoptionsRedefine(
				// &assembly_comp_path,
				NULL,  
				//&in_CADComponentData_map[in_ComponentID].assembledFeature,
				&assembledFeature_temp,
				elem_tree,
				opts,
				//PRO_REGEN_NO_FLAGS,
				PRO_REGEN_FORCE_REGEN,
				&p_errors);

	// zzzz Did the above function change assembledFeature_temp,  the documentation says no, but not so sure
	// If it did change it, we will need to do something like the following:
	//in_CADComponentData_map[in_ComponentID].assembledFeature.type = CADFeatureGeometryType_enum(assembledFeature_temp.type);
	//in_CADComponentData_map[in_ComponentID].assembledFeature.id = assembledFeature_temp.id;
	//in_CADComponentData_map[in_ComponentID].assembledFeature.owner = assembledFeature_temp.owner; 


	// Verify if regeneration succeeded
	ProSolidRegenerationStatus s;
	ProSolidRegenerationstatusGet((ProSolid)(in_TopAssembly), &s);

	if (s == PRO_SOLID_FAILED_REGENERATION)
		out_Warnings =	"WARNING, Function SetConstraints - Redefined feature failed to regenerate.  " + 
						std::string("\nIn some cases multiple regenerations are necessary to successfully regenerate.  In other cases, ") +
						"\nthe regeneration fails because constraints are malformed.  Open the Creo assembly and try to regenerate" +
						"\nthe component interactively. If the component will not regenerate interactively, then the problem is " +
						"\nprobably caused by malformed constraints." +
						"\n   ComponentInstanceID: " + in_ComponentID + "  Model Name: " + (std::string)in_CADComponentData_map[in_ComponentID].name;

	isis::isis_ProArrayFree( (ProArray*) &opts);

	/*isis::isis_ProFeatureElemtreeExtract (	&in_CADComponentData_map[in_ComponentID].assembledFeature,
											&assembly_comp_path,
											PRO_FEAT_EXTRACT_NO_OPTS,
											&elem_tree );*/

	//featureTreeFileName_narrow = (string)in_CADComponentData_map[in_ComponentID].name+"_2.xml";
	//featureTreeFileName_multi = (string)featureTreeFileName_narrow;
	//isis::isis_ProElemtreeWrite(elem_tree, PRO_ELEMTREE_XML, (wchar_t*)(const wchar_t*)featureTreeFileName_multi);

	//ProElement elem_tree_2;

	//isis::isis_ProElementAlloc(PRO_E_FEATURE_TREE, &elem_tree_2);

	//isis::isis_ProFeatureElemtreeExtract (	&in_CADComponentData_map[in_ComponentID].assembledFeature,
	//										&assembly_comp_path,
	//										PRO_FEAT_EXTRACT_NO_OPTS,
	//										&elem_tree_2 );

	//featureTreeFileName_narrow = (string)in_CADComponentData_map[in_ComponentID].name;
	//featureTreeFileName_narrow += "_5.xml";
	// For family table names, replace <>  e.g. Chassis_8_Wheel<Chassis> Chassis_8_Wheel_Chassis_
	//boost::replace_all(featureTreeFileName_narrow, "<", "_");
	//boost::replace_all(featureTreeFileName_narrow, ">", "_");
	//featureTreeFileName_multi = (string)featureTreeFileName_narrow;
	//isis::isis_ProElemtreeWrite(elem_tree_2, PRO_ELEMTREE_XML, (wchar_t*)(const wchar_t*)featureTreeFileName_multi);

	/*
	ProSelection	mdl_sel;
	ProModelitem	modelitem;
	
	isis::isis_ProMdlToModelitem(ParentAssemblyModel, &modelitem);
	std::cout << std::endl << "+++++++++++++++++++++++++++++++++++ Point 16";
	isis::isis_ProSelectionAlloc(NULL, &modelitem, &mdl_sel);
	std::cout << std::endl << "+++++++++++++++++++++++++++++++++++ Point 17";
	opts[0] = PRO_FEAT_CR_DEFINE_MISS_ELEMS;
	ProFeature Feature;
	isis::isis_ProFeatureWithoptionsCreate (mdl_sel,
                                 elem_tree_2,
                                 opts,
                                 PRO_REGEN_NO_FLAGS, 
								 &Feature,
                                 &p_errors );
	std::cout << std::endl << "+++++++++++++++++++++++++++++++++++ Point 18";
	*/
	// Free allocated objects
	//isis::isis_ProElementFree(&elem_tree);
	////isis::isis_ProElempathFree (&setsPath);
	//isis::isis_ProElempathFree (&constrsPath);
	
	//for each ( ProElement* i in allocatedElements) isis::isis_ProElementFree(i);

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct ConstraintOrder
{
	// ConstraintOrder expresses a sorted orthogonal view of std::vector<PerSetConstraintDefinition_2> perSetConstraintDefinitions, 
	// wherein the orthogonal view sorts PerSetConstraintDefinition_2s and within a PerSetConstraintDefinition_2s sorts the 
	// constraint features.

	// ConstraintOrder is an odering of the following two items:
	//	1) std::vector<PerSetConstraintDefinition_2> perSetConstraintDefinitions
	//	2) PerSetConstraintDefinition_2.contraintFeatureDefinitions;  i.e. std::vector<ContraintFeatureDefinition_2>	contraintFeatureDefinitions;
	//
	//	The first item is ordered by ConstraintOrder.constraintDefinitionOrder
	//	The second item is ordered by constraintFeatureOrder_map
	//
	//  Example mapping constraintDefinitionOrder[0] points to perSetConstraintDefinitions[2]
	//	constraintDefinitionOrder			perSetConstraintDefinitions
	//			2	----------------					0
	//			0					|					1
	//			1					|-------------->	2
	std::vector<int> constraintDefinitionOrder;  // Order of  const std::vector<PerSetConstraintDefinition_2> perSetConstraintDefinitions

	// Key (i.e. first):	int in perSetConstraintDefinitions  e.g. 2
	// Second:				Vector of IDs, wherein IDs are to PerSetConstraintDefinition_2.contraintFeatureDefinitions.  
	//						The IDs are offsets (starting with 0 ) to a vector of contraintFeatureDefinitions
	std::map<int, std::vector<int>> constraintFeatureOrder_map;  

	const std::vector<int> getFeatureIDs( int constraintDefinitionID) const
	{
		std::map<int, std::vector<int>>::const_iterator &i = constraintFeatureOrder_map.find(constraintDefinitionID);

		if ( i == constraintFeatureOrder_map.end() )
		{
			std::stringstream errorString;
			errorString << "Function - " << __FUNCTION__ << ",constraintFeatureOrder_map does not contain constraintDefinitionID: " << constraintDefinitionID;
			throw isis::application_exception("C04002",errorString.str());		
		}
		else
		{
			return i->second;
		}
	}
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& output, const ConstraintOrder &in_ConstraintOrder)
{
	output << 
		"ConstraintOrder: " << std::endl <<
		"   constraintDefinitionOrder:";
		for each ( int i in in_ConstraintOrder.constraintDefinitionOrder ) output << " " << i;
		output << std::endl <<
		"   constraintFeatureOrder_map:";
		for each ( const std::pair<int, std::vector<int>> i in in_ConstraintOrder.constraintFeatureOrder_map )
		{  
			output << std::endl <<
				"      constraintDefinitionOrder index: " << i.first;
			for each ( int j in i.second )
			{
				output << std::endl <<
				"         featureOrder index: " << j;
			}
		}
	return output;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ValidateConstraintOrder_ThrowExceptionIfInvalid(	const std::string									&in_CallingFunction,
														const ConstraintOrder								&in_ConstraintOrder,
														const std::vector<PerSetConstraintDefinition_2>		&in_PerSetConstraintDefinitions )
																					throw (isis::application_exception)
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
	// in_ConstraintOrder has indexes into in_PerSetConstraintDefinitions.  
	//		in_ConstraintOrder.constraintDefinitionOrder  to-index-into  in_PerSetConstraintDefinitions
	//		in_ConstraintOrder.constraintFeatureOrder	  to-index-into   the features in PerSetConstraintDefinition_2	
	// Therefore, we must make sure that the indexes are not out of range; otherwise a memory violation would occur.
	//////////////////////////////////////////////////////////////////
	// Verify in_ConstraintOrder.constraintDefinitionOrder is in Range
	//////////////////////////////////////////////////////////////////
	for each ( int i in in_ConstraintOrder.constraintDefinitionOrder )
	{
		if (  i < 0 ||  i >=  in_PerSetConstraintDefinitions.size())
		{
			std::stringstream errorString;
			errorString << "Function - " << __FUNCTION__ << "(Invoked by: " << in_CallingFunction << "), was passed an out-of-range index into in_PerSetConstraintDefinitions" << std::endl <<
			"  Index:           " << i << std::endl <<
			"  Allowed Range:   " << "0 to " <<  in_PerSetConstraintDefinitions.size() - 1;
			throw isis::application_exception("C04002",errorString.str());		
		}
	}

	//////////////////////////////////////////////////////////////////
	// Verify in_ConstraintOrder.constraintFeatureOrder is in Range
	//////////////////////////////////////////////////////////////////
	for each ( int i in in_ConstraintOrder.constraintDefinitionOrder )
	{
		const PerSetConstraintDefinition_2 &pSCD = in_PerSetConstraintDefinitions[i];

		for each (  int j in in_ConstraintOrder.getFeatureIDs(i) )
		{	
			if (  j < 0 ||  j >=  pSCD.get_NumberOfConstraintFeatures())
			{
				std::stringstream errorString;
				errorString << "Function - " << __FUNCTION__ << "(Invoked by: " << in_CallingFunction << "), was passed an out-of-range index into in_PerSetConstraintDefinitions[0] Constraint Features" << std::endl <<
				"  Index:           " << j << std::endl <<
				"  Allowed Range:   " << "0 to " <<  pSCD.get_NumberOfConstraintFeatures() - 1;
				throw isis::application_exception("C04002",errorString.str());	
			}
		}
	}

	//////////////////////////////////////////////////////////////
	// Verify that each constraint has geometry features defined
	//////////////////////////////////////////////////////////////
	for each ( int i in in_ConstraintOrder.constraintDefinitionOrder )
	{
		const PerSetConstraintDefinition_2 &pSCD = in_PerSetConstraintDefinitions[i];
		for each ( const int &j in in_ConstraintOrder.getFeatureIDs(i) )
		{	
			if ( pSCD.get_NumberOfConstraintFeatures() == 0 )
			{
				std::stringstream errorString;
				errorString << "Function - " << __FUNCTION__ << "(Invoked by: " << in_CallingFunction << "), was passed a constraint with no features defined.";
				throw isis::application_exception("C04002",errorString.str());	
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Description:
//		Add constraints to the feature tree for in_ComponentID.  in_ConstraintOrder lists the constraints and constraint features
//		that will be used to constrain the model. This list may be a subset of the constraints and constraint features listed
//		in in_PerSetConstraintDefinitions.  
// Pre-Conditions:
//		The feature tree for in_CADComponentData_map[in_ComponentID] must NOT contain any constraints.  It is assumed that
//		we are starting with a feature tree that only contains 
//			<PRO_E_FEATURE_TYPE
//			<PRO_E_COMPONENT_MODEL
//			<PRO_E_COMPONENT_INIT_POS
//			The other fields should not be set
//		
//		in_TopAssembly must point to the top-most assembly.  This is not necessarily the parent of in_ComponentID.
//		in_ComponentID must be the model that this function constrains.
//
//		if ( !in_TreatAsAUserDefinedJoint and in_PerSetConstraintDefinitions[i] defines  REVOLUTE_JOINT/PRISMATIC_JOINT joint)
//			in_ConstraintOrder must define the features in the order required for the REVOLUTE_JOINT/PRISMATIC_JOINT
//			e.g. REVOLUTE_JOINT axis first, PRISMATIC_JOINT axis first
//
// Post-Conditions
//		Complete feature tree with 
//			Constraint sets set with the name Set1, Set2, ... where the integer is started at 1 and incremented.
//			Internal to the Creo constraint-feature sets are numbered starting with 0.  This means than the set named Set1 would
//			have an internal ID of 0. The internal ID is actuall an offset into an array, so the integers must be 
//			consective.
//			Constraints set per the list of constraints and constraint features listed in in_ConstraintOrder.  
//
//		If an error occurs, isis::application_exception would be thrown.
void SetConstraints_2 (	
		ProSolid											&in_TopAssembly, 
		const std::string									&in_ComponentID, 
		std::map<string, isis::CADComponentData>			&in_CADComponentData_map, 
		const ConstraintOrder								&in_ConstraintOrder,
		const std::vector<PerSetConstraintDefinition_2>		&in_PerSetConstraintDefinitions,
		bool												in_TreatAsAUserDefinedJoint,  // if true, will not make the constraint set a kinematic
																						  // constraint set even if the constraints actually
																						  // define a kinematic joint. 
		std::string											&out_Warnings )
																		throw (isis::application_exception)
{
	ProAsmcompconstraint *proAsmcompconstraint_dummy_not_used = NULL;
	

	////////////////////////////////////////////////////
	// Verfify in_PerSetConstraintDefinitions not empty
	////////////////////////////////////////////////////
	if ( in_PerSetConstraintDefinitions.size() == 0 )
	{
		std::stringstream errorString;
		errorString << "Function - " << __FUNCTION__ << ", was passed an empty in_PerSetConstraintDefinitions.";
		throw isis::application_exception("C04002",errorString.str());		
	}

	ValidateConstraintOrder_ThrowExceptionIfInvalid(__FUNCTION__, in_ConstraintOrder, in_PerSetConstraintDefinitions);

	isis_LOG(lg, isis_FILE, isis_INFO) << __FUNCTION__ << ", ConstraintOrder";
	isis_LOG(lg, isis_FILE, isis_INFO) << in_ConstraintOrder;

	isis_LOG(lg, isis_FILE, isis_INFO) << "";
	isis_LOG(lg, isis_FILE, isis_INFO) << "********** Begin SetConstraints, Constraint Definitions  ***************";
	
	int constraint_counter = 0;

	/////////////////////////////////////
	// Log Constraint Information
	/////////////////////////////////////
	isis_LOG(lg, isis_FILE, isis_INFO);
	isis_LOG(lg, isis_FILE, isis_INFO) << "constraintDefinitionOrder: ";
	for each ( int i in in_ConstraintOrder.constraintDefinitionOrder) isis_LOG(lg, isis_FILE, isis_INFO) << "   " << i;
	isis_LOG(lg, isis_FILE, isis_INFO) << "constraintFeatureOrder: ";

	for each ( const std::pair<int, std::vector<int>>  &i in in_ConstraintOrder.constraintFeatureOrder_map )
			for each ( int j in i.second ) isis_LOG(lg, isis_FILE, isis_INFO)  << "   constraintDefinitionID" << i.first <<  ", constraintFeatureID: " << j;


	int setID_index = 0;  // This is not the setID. It is an index into an array of setIDs.  The first index is always 0;

	for each ( int i in in_ConstraintOrder.constraintDefinitionOrder )
	{
		const PerSetConstraintDefinition_2 &pSCD = in_PerSetConstraintDefinitions[i];
		isis_LOG(lg, isis_FILE, isis_INFO) << "constraintDefinition, order ID: " << i;

		isis_LOG(lg, isis_FILE, isis_INFO);
		isis_LOG(lg, isis_FILE, isis_INFO) << "\n**** Constraint Index: " <<  constraint_counter << "  ******";
		//isis_LOG(lg, isis_FILE, isis_INFO) << "setID:                   " <<  pSCD.get_SetID();
		isis_LOG(lg, isis_FILE, isis_INFO) << "setID:                   " <<  setID_index;
		isis_LOG(lg, isis_FILE, isis_INFO) << "hasAGuideConstraint:     " <<  pSCD.get_HasAGuideConstraint();
		isis_LOG(lg, isis_FILE, isis_INFO) << "jointType_withoutguide:  " <<  CADJointType_string(pSCD.get_JointType_withoutguide());
		isis_LOG(lg, isis_FILE, isis_INFO) << "jointType_withguide:     " <<  CADJointType_string(pSCD.get_JointType_withguide());

		for each ( const int &j in in_ConstraintOrder.getFeatureIDs(i) )
		{

			const ContraintFeatureDefinition_2 &cFD = pSCD.get_ContraintFeatureDefinition_const(j);
	
			std::stringstream str;
			stream_PopulateOneConstraintInConstraintStructure(
							&in_TopAssembly,	// typedef struct sld_part* ProSolid;
							cFD.base_model_path_list,		
							cFD.added_model_path_list,
							cFD.pro_datum_type,			// enum PRO_SURFACE, PRO_AXIS
							cFD.base_model_datum_name,		// ASM_RIGHT, A_1..
							cFD.base_model_datum_side,		// enum PRO_DATUM_SIDE_YELLOW (SIDE_A), PRO_DATUM_SIDE_RED (SIDE_B), PRO_DATUM_SIDE_NONE
							cFD.added_model_datum_name,	// RIGHT, A23 ..
							cFD.added_model_datum_side,	// enum PRO_DATUM_SIDE_YELLOW (SIDE_A), PRO_DATUM_SIDE_RED (SIDE_B), PRO_DATUM_SIDE_NONE
							cFD.constraint_type,			// enum PRO_ASM_ALIGN, PRO_ASM_ALIGN_OFF...
							cFD.offset_between_datums,		// This is only used if in_constraint_type == PRO_ASM_ALIGN_OFF or PRO_ASM_MATE_OFF
							str);
			
				isis_LOG(lg, isis_FILE, isis_INFO) << str.str();
				isis_LOG(lg, isis_FILE, isis_INFO) << "TreatAsAUserDefinedJoint (in_SET_USER_DEFINED_TYPE): " << in_TreatAsAUserDefinedJoint;
				isis_LOG(lg, isis_FILE, isis_INFO);
				++constraint_counter;
		}
		++setID_index;
	}



	ProElemId elemid;
	ProElement elem_tree;
	ProValueData value_data;	

	/////////////////////////////////////////
	// Setup the path to the parent assembly
	/////////////////////////////////////////
	ProAsmcomppath	assembly_comp_path;
	Retrieve_ProAsmcomppath_WithExceptions(in_TopAssembly, in_CADComponentData_map[in_ComponentID].componentPaths, assembly_comp_path);

	////////////////////////////////
	// Extract PRO_E_FEATURE_TREE
	////////////////////////////////
	isis::isis_ProElementAlloc(PRO_E_FEATURE_TREE, &elem_tree);

	ProAsmcomp					assembledFeature_temp;			
	//assembledFeature_temp.type =	FeatureGeometryType_enum(in_CADComponentData_map[in_ComponentID].assembledFeature.type);
	//assembledFeature_temp.id   =	                         in_CADComponentData_map[in_ComponentID].assembledFeature.id;
	//assembledFeature_temp.owner =	                         in_CADComponentData_map[in_ComponentID].assembledFeature.owner; 	
	assembledFeature_temp = getProAsmcomp(in_CADComponentData_map[in_ComponentID].assembledFeature);


	isis::isis_ProFeatureElemtreeExtract (	//&in_CADComponentData_map[in_ComponentID].assembledFeature,
											&assembledFeature_temp,
											&assembly_comp_path,
											PRO_FEAT_EXTRACT_NO_OPTS,
											&elem_tree );
	
	//std::string featureTreeFileName_narrow = in_CADComponentData_map[in_ComponentID].name;
	//featureTreeFileName_narrow += "_1.xml";
	// For family table names, replace <>  e.g. Chassis_8_Wheel<Chassis> would become Chassis_8_Wheel_Chassis_
	//boost::replace_all(featureTreeFileName_narrow, "<", "_");
	//boost::replace_all(featureTreeFileName_narrow, ">", "_");
	//isis::MultiFormatString  featureTreeFileName_multi( featureTreeFileName_narrow);
	//isis::isis_ProElemtreeWrite(elem_tree, PRO_ELEMTREE_XML, (wchar_t*)(const wchar_t*)featureTreeFileName_multi);	

	////////////////////////////////
	// Add Sets 
	////////////////////////////////

	// Get PRO_E_COMPONENT_SETS 
	ProElement sets_elem;
	ProElempathItem sets_path_Item[] = {{PRO_ELEM_PATH_ITEM_TYPE_ID, PRO_E_COMPONENT_SETS}};
	ProElempath   setsPath;
	isis::isis_ProElempathAlloc (&setsPath);			
	isis::isis_ProElempathDataSet (setsPath, sets_path_Item, 1);

	isis::isis_ProElemtreeElementGet (elem_tree, setsPath, &sets_elem);

	setID_index = 0;

	for each ( int i in in_ConstraintOrder.constraintDefinitionOrder )
	{
		const PerSetConstraintDefinition_2 &pSCD = in_PerSetConstraintDefinitions[i];
		//std::string SetName_narrorw = "Set" + std::to_string((long long) pSCD.get_SetID());
		std::string SetName_narrorw = "Set" + std::to_string((long long) setID_index + 1);
		MultiFormatString SetName_multi(SetName_narrorw);

		ProAsmcompSetType setType;
		if ( in_TreatAsAUserDefinedJoint )
		{
			setType = PRO_ASM_SET_USER_DEFINED_TYPE;
			Add_PRO_E_COMPONENT_SET( sets_elem, SetName_multi, setID_index, setType );
		}
		else
		{			
			switch ( pSCD.get_JointType_withoutguide() )
			{
				case FIXED_JOINT:
					setType = PRO_ASM_SET_TYPE_FIXED;
					break;
				case REVOLUTE_JOINT:
					setType = PRO_ASM_SET_TYPE_PIN;
					break;
				case UNIVERSAL_JOINT:
					setType = PRO_ASM_SET_USER_DEFINED_TYPE; // ?? Creo has support for universal joint, investigate how to accomplish this
					break;
				case SPHERICAL_JOINT:
					setType = PRO_ASM_SET_TYPE_BALL;
					break;
				case PRISMATIC_JOINT:
					setType = PRO_ASM_SET_TYPE_SLIDER;  
					break;
				case CYLINDRICAL_JOINT:
					setType = PRO_ASM_SET_TYPE_CYLINDRICAL;
					break;
				case PLANAR_JOINT:
					setType = PRO_ASM_SET_TYPE_PLANAR;
					break;
				case FREE_JOINT:
					setType = PRO_ASM_SET_USER_DEFINED_TYPE;  // ?? Creo has a PRO_ASM_SET_TYPE_6DOF, should this be used
					break;
				case UNKNOWN_JOINT_TYPE:
					setType = PRO_ASM_SET_USER_DEFINED_TYPE;
					break;
				default:
					setType = PRO_ASM_SET_USER_DEFINED_TYPE;
			}

			Add_PRO_E_COMPONENT_SET( sets_elem, SetName_multi, setID_index, setType );

			//InputJoint& inputJoint = in_CADComponentData_map[in_ComponentID].constraintDef.constraints[setID_index].inputJoint;		
			const InputJoint& inputJoint = pSCD.get_JointMotionLimits();	


			// Setup rotation/translation limits
			if (pSCD.get_JointType_withoutguide() == REVOLUTE_JOINT && inputJoint.Type == REVOLUTE_JOINT)
			{
				const ContraintFeatureDefinition_2 &firstConstraintFeatureDef =  pSCD.get_ContraintFeatureDefinition_const(in_ConstraintOrder.getFeatureIDs(i)[0]);
				if (inputJoint.RotationLimits[InputJoint::DEFAULT].Provided && inputJoint.RotationLimitRefs_This[InputJoint::DEFAULT].Provided && inputJoint.RotationLimitRefs_Other[InputJoint::DEFAULT].Provided)
					SetupRotationLimits_2(in_TopAssembly, firstConstraintFeatureDef, elem_tree, setID_index, inputJoint);
					//SetupRotationLimits_2(in_TopAssembly, pSCD, elem_tree, setID_index, inputJoint);
			}
		
			// ?? why prismatic 
			if (pSCD.get_JointType_withoutguide() == PRISMATIC_JOINT && inputJoint.Type == PRISMATIC_JOINT)
			{
				const ContraintFeatureDefinition_2 &firstConstraintFeatureDef =  pSCD.get_ContraintFeatureDefinition_const(in_ConstraintOrder.getFeatureIDs(i)[0]);
				if (inputJoint.TranslationLimits[InputJoint::DEFAULT].Provided && inputJoint.TranslationLimitRefs_This[InputJoint::DEFAULT].Provided && inputJoint.TranslationLimitRefs_Other[InputJoint::DEFAULT].Provided)
					SetupTranslationLimits_2(in_TopAssembly, firstConstraintFeatureDef, elem_tree, setID_index, inputJoint);
			}
			

		} // END else  if ( in_TreatAsAUserDefinedJoint )
		++setID_index;
	}

	// ttt March 29

	///////////////////////////////////////
	// Get PRO_E_COMPONENT_CONSTRAINTS
	///////////////////////////////////////
	ProElement constrs_elem;
	ProElempathItem constrs_path_Item[] = {{PRO_ELEM_PATH_ITEM_TYPE_ID, PRO_E_COMPONENT_CONSTRAINTS}};
	ProElempath   constrsPath;
	isis::isis_ProElempathAlloc (&constrsPath);			
	isis::isis_ProElempathDataSet (constrsPath, constrs_path_Item, 1);
	isis::isis_ProElemtreeElementGet (elem_tree, constrsPath, &constrs_elem);

	setID_index = 0;  

	for each ( int i in in_ConstraintOrder.constraintDefinitionOrder )
	{
		const PerSetConstraintDefinition_2 &pSCD = in_PerSetConstraintDefinitions[i];

		for each ( int j in in_ConstraintOrder.getFeatureIDs(i) )
		{
			isis_LOG(lg, isis_FILE, isis_INFO) << "";
			isis_LOG(lg, isis_FILE, isis_INFO) << "Add_PRO_E_COMPONENT_CONSTRAINT, Model Name: " << in_CADComponentData_map[in_ComponentID].name <<
				" ComponentID: " << in_ComponentID;
			Add_PRO_E_COMPONENT_CONSTRAINT_2( constrs_elem, pSCD.get_ContraintFeatureDefinition_const(j), in_TopAssembly, setID_index);
		}
		++setID_index;
	}

	/*
	// DEBUG DEBUG
	string featureTreeFileName_narrow = (string)in_CADComponentData_map[in_ComponentID].name+"_1.xml";
	MultiFormatString featureTreeFileName_multi = (string)featureTreeFileName_narrow;
	ProElemtreeWrite(elem_tree, PRO_ELEMTREE_XML, (wchar_t*)(const wchar_t*)featureTreeFileName_multi);
	// DEBUG DEBUG
	*/
	///////////////////////////////////////////////////////
	// Commit the element tree
	///////////////////////////////////////////////////////

	ProFeatureCreateOptions *opts;  
	isis::isis_ProArrayAlloc(2, sizeof (ProFeatureCreateOptions), 1, (ProArray*) &opts);
	opts[0] = PRO_FEAT_CR_DEFINE_MISS_ELEMS;   
	opts[1] = PRO_FEAT_CR_FIX_MODEL_ON_FAIL;

	//isis::isis_ProArrayAlloc(1, sizeof (ProFeatureCreateOptions), 1, (ProArray*) &opts);
	//opts[0] = PRO_FEAT_CR_DEFINE_MISS_ELEMS;   

	ProErrorlist p_errors;

	int flags = 0;

	isis::isis_ProFeatureWithoptionsRedefine(
				// &assembly_comp_path,
				NULL,  
				//&in_CADComponentData_map[in_ComponentID].assembledFeature,
				&assembledFeature_temp,
				elem_tree,
				opts,
				//PRO_REGEN_NO_FLAGS,
				PRO_REGEN_FORCE_REGEN,
				//PRO_REGEN_UPDATE_INSTS,
				&p_errors);


	// zzzz Did the above function change assembledFeature_temp,  the documentation says no, but not so sure
	// If it did change it, we will need to do something like the following:
	//in_CADComponentData_map[in_ComponentID].assembledFeature.type = CADFeatureGeometryType_enum(assembledFeature_temp.type);
	//in_CADComponentData_map[in_ComponentID].assembledFeature.id = assembledFeature_temp.id;
	//in_CADComponentData_map[in_ComponentID].assembledFeature.owner = assembledFeature_temp.owner; 


	// Verify if regeneration succeeded
	ProSolidRegenerationStatus s;
	ProSolidRegenerationstatusGet((ProSolid)(in_TopAssembly), &s);

	if (s == PRO_SOLID_FAILED_REGENERATION)
		out_Warnings =	"WARNING, Function SetConstraints - Redefined feature failed to regenerate.  " + 
						std::string("\n   In some cases multiple regenerations are necessary to successfully regenerate.  In other cases, ") +
						"\n   the regeneration fails because constraints are malformed.  Open the Creo assembly and try to regenerate" +
						"\n   the component interactively. If the component will not regenerate interactively, then the problem is " +
						"\n   probably caused by malformed constraints." +
						"\n      ComponentInstanceID: " + in_ComponentID + "  Model Name: " + (std::string)in_CADComponentData_map[in_ComponentID].name;

	isis::isis_ProArrayFree( (ProArray*) &opts);


	/*
	// DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG 
	isis::isis_ProFeatureElemtreeExtract (	&in_CADComponentData_map[in_ComponentID].assembledFeature,
											&assembly_comp_path,
											PRO_FEAT_EXTRACT_NO_OPTS,
											&elem_tree );

	featureTreeFileName_narrow = (string)in_CADComponentData_map[in_ComponentID].name+"_2.xml";
	featureTreeFileName_multi = (string)featureTreeFileName_narrow;
	isis::isis_ProElemtreeWrite(elem_tree, PRO_ELEMTREE_XML, (wchar_t*)(const wchar_t*)featureTreeFileName_multi);
	// DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG 	
	*/
	//ProElement elem_tree_2;

	//isis::isis_ProElementAlloc(PRO_E_FEATURE_TREE, &elem_tree_2);

	//isis::isis_ProFeatureElemtreeExtract (	&in_CADComponentData_map[in_ComponentID].assembledFeature,
	//										&assembly_comp_path,
	//										PRO_FEAT_EXTRACT_NO_OPTS,
	//										&elem_tree_2 );

	//featureTreeFileName_narrow = (string)in_CADComponentData_map[in_ComponentID].name;
	//featureTreeFileName_narrow += "_5.xml";
	// For family table names, replace <>  e.g. Chassis_8_Wheel<Chassis> Chassis_8_Wheel_Chassis_
	//boost::replace_all(featureTreeFileName_narrow, "<", "_");
	//boost::replace_all(featureTreeFileName_narrow, ">", "_");
	//featureTreeFileName_multi = (string)featureTreeFileName_narrow;
	//isis::isis_ProElemtreeWrite(elem_tree_2, PRO_ELEMTREE_XML, (wchar_t*)(const wchar_t*)featureTreeFileName_multi);

	/*
	ProSelection	mdl_sel;
	ProModelitem	modelitem;
	
	isis::isis_ProMdlToModelitem(ParentAssemblyModel, &modelitem);
	std::cout << std::endl << "+++++++++++++++++++++++++++++++++++ Point 16";
	isis::isis_ProSelectionAlloc(NULL, &modelitem, &mdl_sel);
	std::cout << std::endl << "+++++++++++++++++++++++++++++++++++ Point 17";
	opts[0] = PRO_FEAT_CR_DEFINE_MISS_ELEMS;
	ProFeature Feature;
	isis::isis_ProFeatureWithoptionsCreate (mdl_sel,
                                 elem_tree_2,
                                 opts,
                                 PRO_REGEN_NO_FLAGS, 
								 &Feature,
                                 &p_errors );
	std::cout << std::endl << "+++++++++++++++++++++++++++++++++++ Point 18";
	*/
	// Free allocated objects
	//isis::isis_ProElementFree(&elem_tree);
	////isis::isis_ProElempathFree (&setsPath);
	//isis::isis_ProElempathFree (&constrsPath);
	
	//for each ( ProElement* i in allocatedElements) isis::isis_ProElementFree(i);

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
void Apply_CADDatum_ModelConstraints_Via_FeatureTree(	const std::string &in_ComponentID,
														const std::string &in_Model_Name,
														ProMdlType     mdltype,	
														ProSolid	  *in_assembly_model,
														//ProAsmcomppath *in_assembly_path,
														ProSolid	   *in_assembled_Model,
														//ProAsmcomp		*in_assemble_Model_ProAsmcomp,
														std::vector<ContraintFeatureDefinition> in_ContraintFeatureDefinitions,
														ProFeature		*out_ProFeature )
																		throw (isis::application_exception)
{

	ProAsmcompconstraint *proAsmcompconstraint_dummy_not_used = NULL;

	

	isis_LOG(lg, isis_FILE, isis_INFO) << "";
	isis_LOG(lg, isis_FILE, isis_INFO) << "********** Begin PopulateOneConstraintInConstraintStructure  ***************";
	std::stringstream str;
	int constraint_counter = 0;
	for each ( ContraintFeatureDefinition i_constraint in in_ContraintFeatureDefinitions )
	{
		isis_LOG(lg, isis_FILE, isis_INFO) << "**** Constraint Index: " <<  constraint_counter << "  ******";
		stream_PopulateOneConstraintInConstraintStructure(
						in_assembly_model,				// typedef struct sld_part* ProSolid;
						i_constraint.base_model_path_list,		
						i_constraint.added_model_path_list,
						i_constraint.pro_datum_type,				// enum PRO_SURFACE, PRO_AXIS
						i_constraint.base_model_datum_name,		// ASM_RIGHT, A_1..
						i_constraint.base_model_datum_side,		// enum PRO_DATUM_SIDE_YELLOW (SIDE_A), PRO_DATUM_SIDE_RED (SIDE_B), PRO_DATUM_SIDE_NONE
						i_constraint.added_model_datum_name,		// RIGHT, A23 ..
						i_constraint.added_model_datum_side,		// enum PRO_DATUM_SIDE_YELLOW (SIDE_A), PRO_DATUM_SIDE_RED (SIDE_B), PRO_DATUM_SIDE_NONE
						i_constraint.constraint_type,				// enum PRO_ASM_ALIGN, PRO_ASM_ALIGN_OFF...
						i_constraint.offset_between_datums,		// This is only used if in_constraint_type == PRO_ASM_ALIGN_OFF or PRO_ASM_MATE_OFF
						proAsmcompconstraint_dummy_not_used,
						str);
	}
	isis_LOG(lg, isis_FILE, isis_INFO) << str.str();
	isis_LOG(lg, isis_FILE, isis_INFO) << "********** End PopulateOneConstraintInConstraintStructure  ***************" << isis_EOL;

	
	ProElemId elemid;
	ProElement root_tree;
	ProValueData value_data;	

	////////////////////////////////
	// Create PRO_E_FEATURE_TREE
	////////////////////////////////

	isis::isis_ProElementAlloc(PRO_E_FEATURE_TREE, &root_tree);

	
	ProAsmcomppath assem_path;
	ProIdTable c_id_table;
	c_id_table [0] = -1;
	isis::isis_ProAsmcomppathInit(*in_assembly_model, c_id_table, 0, &assem_path);	

	
	//isis::isis_ProFeatureElemtreeExtract (	in_assembled_Model,
	//										assem_path,
	//										PRO_FEAT_EXTRACT_NO_OPTS,
	//										&root_tree );
  
	isis::isis_ProElemtreeWrite(root_tree, PRO_ELEMTREE_XML,L"z_Element_tree_4.xml");
	std::cout << std::endl << "+++++++++++++++++++++++++++++++++++ Point 1";

	////////////////////////////////
	// Create PRO_E_FEATURE_TYPE
	////////////////////////////////
	elemid = PRO_E_FEATURE_TYPE ;	
	value_data.type = PRO_VALUE_TYPE_INT;
	value_data.v.i = PRO_FEAT_COMPONENT ;
	ProElement feat_type_elem;
	isis::isis_AddElementtoElemTree(elemid, &root_tree,&value_data, &feat_type_elem);

	std::cout << std::endl << "+++++++++++++++++++++++++++++++++++ Point 2";

	// zzzz good to here


	//////////////////////////////////////////////////
	// Set PRO_E_COMPONENT_MODEL, assembled model
	/////////////////////////////////////////////////
	elemid = PRO_E_COMPONENT_MODEL ;	
	value_data.type = PRO_VALUE_TYPE_POINTER;
	value_data.v.p = *in_assembled_Model;
	ProElement comp_model_elem;	
	isis::isis_AddElementtoElemTree(elemid, &root_tree,&value_data, &comp_model_elem);

		std::cout << std::endl << "+++++++++++++++++++++++++++++++++++ Point 3";

		isis::isis_ProElemtreeWrite(root_tree, PRO_ELEMTREE_XML,L"z_Element_tree_5.xml");

	////////////////////////////////
	// Add PRO_E_COMPONENT_SETS
	////////////////////////////////
	ProElement comp_constr_sets_elem;	
	isis::isis_ProElementAlloc (PRO_E_COMPONENT_SETS, &comp_constr_sets_elem);  
	isis::isis_ProElemtreeElementAdd (root_tree, NULL, comp_constr_sets_elem ); 

	std::cout << std::endl << "+++++++++++++++++++++++++++++++++++ Point 4";


	////////////////////////////////
	// Add PRO_E_COMPONENT_SET
	////////////////////////////////
	ProElement comp_constr_set_elem;	
	isis::isis_ProElementAlloc (PRO_E_COMPONENT_SET, &comp_constr_set_elem);  
	isis::isis_ProElemtreeElementAdd (comp_constr_sets_elem, NULL, comp_constr_set_elem ); 

	std::cout << std::endl << "+++++++++++++++++++++++++++++++++++ Point 5";

	////////////////////////////////
	// Add a PRO_E_COMPONENT_SET_ID
	////////////////////////////////
	elemid = PRO_E_COMPONENT_SET_ID;
	value_data.type = PRO_VALUE_TYPE_INT;
	value_data.v.i = 1 ;
	ProElement comp_setId_elem;
	isis::isis_AddElementtoElemTree(elemid, &comp_constr_set_elem,&value_data, &comp_setId_elem);

	std::cout << std::endl << "+++++++++++++++++++++++++++++++++++ Point 6";

	//////////////////////////////////
	// Add a PRO_E_COMPONENT_SET_TYPE
	//////////////////////////////////
	elemid = PRO_E_COMPONENT_SET_TYPE;
	value_data.type = PRO_VALUE_TYPE_INT;
	value_data.v.i = PRO_ASM_SET_USER_DEFINED_TYPE ;
	ProElement comp_setType_elem;
	isis::isis_AddElementtoElemTree(elemid, &comp_constr_set_elem,&value_data, &comp_setType_elem);

	std::cout << std::endl << "+++++++++++++++++++++++++++++++++++ Point 7";

	////////////////////////////////////////
	// Add a PRO_E_COMPONENT_CONSTR_SET_ID
	////////////////////////////////////////
	elemid = PRO_E_COMPONENT_CONSTR_SET_ID;
	value_data.type = PRO_VALUE_TYPE_WSTRING;
	value_data.v.w = (wchar_t*) malloc (sizeof(ProName)); 
	//value_data.v.w = new wchar_t (sizeof(ProName));   // WARNING, Don't use new, it causes a memory exception
	wcscpy( value_data.v.w, L"Set2");
	ProElement comp_setTID_elem;
	isis::isis_AddElementtoElemTree(elemid, &comp_constr_set_elem,&value_data, &comp_setTID_elem);

	std::cout << std::endl << "+++++++++++++++++++++++++++++++++++ Point 7 A";

	///////////////////////////////////////
	// Add a PRO_E_COMPONENT_SET_MISC_ATTR
	///////////////////////////////////////
	elemid = PRO_E_COMPONENT_SET_MISC_ATTR;
	value_data.type = PRO_VALUE_TYPE_INT;
	value_data.v.i = PRO_ASM_ENABLE_COMP_SET ;
	ProElement comp_setMiscAttr_elem;
	isis::isis_AddElementtoElemTree(elemid, &comp_constr_set_elem,&value_data, &comp_setMiscAttr_elem);

		std::cout << std::endl << "+++++++++++++++++++++++++++++++++++ Point 8";

	///////////////////////////////////////
	// Add PRO_E_COMPONENT_CONSTRAINTS
	///////////////////////////////////////
	ProElement comp_constrs_elem;	
	isis::isis_ProElementAlloc (PRO_E_COMPONENT_CONSTRAINTS, &comp_constrs_elem);  
	isis::isis_ProElemtreeElementAdd (root_tree, NULL, comp_constrs_elem ); 


	std::cout << std::endl << "aaaaaaa in_ContraintFeatureDefinitions.size()" << in_ContraintFeatureDefinitions.size();
	for each ( ContraintFeatureDefinition i in in_ContraintFeatureDefinitions)
	{
		std::cout << std::endl << "aaaaaaa added_model_component_instance_ID: " << i.added_model_component_instance_ID;

		///////////////////////////////////
		// Add PRO_E_COMPONENT_CONSTRAINT
		//////////////////////////////////
		ProElement comp_constr_elem;	
		isis::isis_ProElementAlloc (PRO_E_COMPONENT_CONSTRAINT, &comp_constr_elem);  
		isis::isis_ProElemtreeElementAdd (comp_constrs_elem, NULL, comp_constr_elem);

		///////////////////////////////////
		// Add PRO_E_COMPONENT_CONSTR_TYPE
		///////////////////////////////////
		elemid = PRO_E_COMPONENT_CONSTR_TYPE ;	
		value_data.type = PRO_VALUE_TYPE_INT;
		value_data.v.i = i.constraint_type ;
		ProElement constr1_type_elem;
		isis::isis_AddElementtoElemTree(elemid, &comp_constr_elem,&value_data, &constr1_type_elem);

		/////////////////////////////////////////
		// Add PRO_E_COMPONENT_COMP_CONSTR_REF
		/////////////////////////////////////////

		// *******************************************
		// Initialize the path to the base component
		// ********************************************
		ProIdTable c_id_table_base_model;

		int base_model_table_size;
		Populate_c_id_table( i.base_model_path_list, c_id_table_base_model, base_model_table_size );

		std::cout << std::endl << "+++++++++++++++++++++++++++++++++++ Point 8 a";

		//ProAsmcomppath  comp_path;
		ProAsmcomppath	base_comp_path;
		isis::isis_ProAsmcomppathInit (	*in_assembly_model,			//ProSolid   p_solid_handle
										c_id_table_base_model,		// ProIdTable 
										base_model_table_size,		// table_size 
										&base_comp_path);			// ProAsmcomppath *p_handle

		// ***********************************************
		// Initialize the path to the assembled component
		// ***********************************************

		std::cout << std::endl << "+++++++++++++++++++++++++++++++++++ Point 8 b";

		ProIdTable c_id_table_added_model;
		int added_model_table_size;
		Populate_c_id_table( i.added_model_path_list, c_id_table_added_model, added_model_table_size );

		//ProAsmcomppath  comp_path;
		ProAsmcomppath	added_comp_path;
		isis::isis_ProAsmcomppathInit (	*in_assembly_model,	//ProSolid   p_solid_handle
									c_id_table_added_model,			// ProIdTable 
									added_model_table_size,			// table_size 
									&added_comp_path);	// ProAsmcomppath *p_handle

		for (int m = 0; m < added_model_table_size; ++m) std::cout << std::endl << std::endl << c_id_table_added_model[m];

		std::cout << std::endl << "+++++++++++++++++++++++++++++++++++ Point 8 c";

		// **************************************************
		// Find the addresse to the base and added compoents
		// **************************************************
		ProMdl		 base_model;					// typedef void* ProMdl
		ProMdl		 added_model;
		isis::isis_ProAsmcomppathMdlGet( &base_comp_path, &base_model);
		isis::isis_ProAsmcomppathMdlGet( &added_comp_path, &added_model);

		std::cout << std::endl << "+++++++++++++++++++++++++++++++++++ Point 8 d";


		//	Find the assembly datum 
		ProModelitem  base_model_datum;
				isis::MultiFormatString  temp_string(i.base_model_datum_name);
		std::cout << std::endl << "+++ Base model datum name: " << temp_string << std::endl;
		isis::isis_ProModelitemByNameInit_WithDescriptiveErrorMsg (	i.base_model_component_instance_ID, // Added arguments
																	i.base_model_name, 
																	i.base_model_type,   
																	i.p_base_model, //base_model, // Original arguments
																	i.pro_datum_type, 
																	i.base_model_datum_name, 
																	&base_model_datum);  
		std::cout << std::endl << "+++++++++++++++++++++++++++++++++++ Point 8 e" << std::endl;

		//	Find the component datum
		ProModelitem  add_model_datum;

		temp_string = i.added_model_datum_name;
		std::cout << std::endl << "+++ Added model datum name: " << temp_string << std::endl;

		isis::isis_ProModelitemByNameInit_WithDescriptiveErrorMsg (	i.added_model_component_instance_ID, // Added arguments
																	i.added_model_name, 
																	i.added_model_type,   
																	i.p_added_model, //added_model, // Original arguments
																	i.pro_datum_type, 
																	i.added_model_datum_name, 
																	&add_model_datum);  

		std::cout << std::endl << "+++++++++++++++++++++++++++++++++++ Point 8 f";

		//	For the assembly reference, initialize a component path.
		//	This is necessary even if the reference geometry is in the assembly.

		//	Allocate the references
		ProSelection base_model_select;
		//isis::isis_ProSelectionAlloc(&base_comp_path, &base_model_datum, &base_model_select);
		isis::isis_ProSelectionAlloc(NULL, &base_model_datum, &base_model_select);

		ProSelection added_model_select;
		//isis::isis_ProSelectionAlloc (&added_comp_path, &add_model_datum, &added_model_select);
		isis::isis_ProSelectionAlloc (NULL, &add_model_datum, &added_model_select);

		isis::isis_ProElemtreeWrite(root_tree, PRO_ELEMTREE_XML,L"z_Element_tree_8f.xml");

		//////////////////////////////////////
		// Add PRO_E_COMPONENT_COMP_CONSTR_REF
		//////////////////////////////////////
		elemid = PRO_E_COMPONENT_COMP_CONSTR_REF ;
		value_data.type = PRO_VALUE_TYPE_SELECTION;
		value_data.v.r =  added_model_select;
		ProElement comp_const1_Ref_elem;
		isis::isis_AddElementtoElemTree(elemid, &comp_constr_elem,&value_data, &comp_const1_Ref_elem);
		std::cout << std::endl << "+++++++++++++++++++++++++++++++++++ Point 8 g";

		//////////////////////////////////////
		// Add PRO_E_COMPONENT_ASSEM_CONSTR_REF
		//////////////////////////////////////
		elemid = PRO_E_COMPONENT_ASSEM_CONSTR_REF;
		value_data.type = PRO_VALUE_TYPE_SELECTION;
		value_data.v.r =  base_model_select;
		ProElement asm_const2_Ref_elem;
		isis::isis_AddElementtoElemTree(elemid, &comp_constr_elem,&value_data, &asm_const2_Ref_elem);
		std::cout << std::endl << "+++++++++++++++++++++++++++++++++++ Point 8 h";

		isis::isis_ProElemtreeWrite(root_tree, PRO_ELEMTREE_XML,L"z_Element_tree_8h.xml");

		////////////////////////////////////////
		// Add a PRO_E_COMPONENT_CONSTR_SET_ID 
		////////////////////////////////////////
		elemid = PRO_E_COMPONENT_CONSTR_SET_ID ;
		value_data.type = PRO_VALUE_TYPE_INT;
		value_data.v.i = 1 ;
		ProElement comp_constr_set_id;
		isis::isis_AddElementtoElemTree(elemid, &comp_constr_elem,&value_data, &comp_constr_set_id);

		std::cout << std::endl << "+++++++++++++++++++++++++++++++++++ Point 8 i";

		isis::isis_ProElemtreeWrite(root_tree, PRO_ELEMTREE_XML,L"z_Element_tree_8i.xml");

	}

		std::cout << std::endl << "+++++++++++++++++++++++++++++++++++ Point 9";
		std::cout.flush();

	ProSelection	mdl_sel;
	ProModelitem	modelitem;
	std::cout << std::endl << "in_assembly_model: " << in_assembly_model;
	
	isis::isis_ProMdlToModelitem(*in_assembly_model, &modelitem);
	isis::isis_ProSelectionAlloc(NULL, &modelitem, &mdl_sel);

std::cout << std::endl << "+++++++++++++++++++++++++++++++++++ Point 10";


	//ProFeature feature;
	ProErrorlist errors;	
	ProFeatureCreateOptions *opts;  
	isis::isis_ProArrayAlloc(1, sizeof (ProFeatureCreateOptions), 1, (ProArray*) &opts);    
	opts[0] = PRO_FEAT_CR_DEFINE_MISS_ELEMS;

		std::cout << std::endl << "+++++++++++++++++++++++++++++++++++ Point 11";


	isis::isis_ProElemtreeWrite(root_tree, PRO_ELEMTREE_XML,L"z_Element_tree_33.xml");

	std::cout << std::endl << "+++++++++++++++++++++++++++++++++++ Point 12";

	//ProUtilShowTreeInInfo(root_tree);

	std::cout << std::endl << "+++++++++++++++++++++++++++++++++++ Point 12, mdl_sel: " << mdl_sel;

	isis::isis_ProFeatureWithoptionsCreate(mdl_sel, root_tree, opts, PRO_REGEN_NO_FLAGS, out_ProFeature, &errors);

	std::cout << std::endl << "+++++++++++++++++++++++++++++++++++ Point 13";

	///////////////////////////////////////////////////////
	// Commit the element tree
	///////////////////////////////////////////////////////

//	ProFeatureCreateOptions *opts;  
 //      isis::isis_ProArrayAlloc(1, sizeof (ProFeatureCreateOptions), 1, (ProArray*) &opts);    
  //     opts[0] = PRO_FEAT_CR_DEFINE_MISS_ELEMS;    

//	ProErrorlist p_errors;

//	std::cout << std::endl << "+++++++++++++++++++++++++++++++++++ Point 12 a";

//	isis::isis_ProFeatureWithoptionsRedefine( 
//				in_assembly_path,
//				//NULL,  
//				in_assemble_Model_ProAsmcomp,
//				root_tree,
//				opts,
//				PRO_REGEN_NO_FLAGS,
//				//PRO_REGEN_UPDATE_ASSEMBLY_ONLY,
//				&p_errors); 

//	std::cout << std::endl << "+++++++++++++++++++++++++++++++++++ Point 13 b";
//	isis::isis_ProArrayFree( (ProArray*) &opts);

}

*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PopulateOneConstraintInConstraintStructure( 
	const std::string &in_ComponentID,
	const std::string &in_Model_Name,
	ProMdlType     mdltype,
	ProSolid	 *in_assembly_model,				// typedef struct sld_part* ProSolid;
	const list<int>    &in_base_model_path_list,    // list of component ids leading to the part/assembly
	const list<int>    &in_added_model_path_list,   // list of component ids leading to the part/assembly
	ProType		 in_pro_datum_type,					// enum PRO_SURFACE, PRO_AXIS
	const ProName		 in_base_model_datum_name,			// ASM_RIGHT, A_1..
	ProDatumside in_base_model_datum_side,			// enum PRO_DATUM_SIDE_YELLOW (SIDE_A), PRO_DATUM_SIDE_RED (SIDE_B), PRO_DATUM_SIDE_NONE
	const ProName		 in_added_model_datum_name,			// RIGHT, A23 ..
	ProDatumside in_added_model_datum_side,			// enum PRO_DATUM_SIDE_YELLOW (SIDE_A), PRO_DATUM_SIDE_RED (SIDE_B), PRO_DATUM_SIDE_NONE
	ProAsmcompConstrType	in_constraint_type,		// enum PRO_ASM_ALIGN, PRO_ASM_ALIGN_OFF...
	double		 in_offset_between_datums,			// This is only used if in_constraint_type == PRO_ASM_ALIGN_OFF or PRO_ASM_MATE_OFF
	ProAsmcompconstraint* &out_constraints,
	int					  &out_indexOfAddedConstraint)
												throw (isis::application_exception)
{
	

	isis_LOG(lg, isis_FILE, isis_INFO) << "";
	isis_LOG(lg, isis_FILE, isis_INFO) << "********** Begin PopulateOneConstraintInConstraintStructure  ***************";
	std::stringstream str;
	stream_PopulateOneConstraintInConstraintStructure(
					in_assembly_model,				// typedef struct sld_part* ProSolid;
					in_base_model_path_list,		
					in_added_model_path_list,
					in_pro_datum_type,				// enum PRO_SURFACE, PRO_AXIS
					in_base_model_datum_name,		// ASM_RIGHT, A_1..
					in_base_model_datum_side,		// enum PRO_DATUM_SIDE_YELLOW (SIDE_A), PRO_DATUM_SIDE_RED (SIDE_B), PRO_DATUM_SIDE_NONE
					in_added_model_datum_name,		// RIGHT, A23 ..
					in_added_model_datum_side,		// enum PRO_DATUM_SIDE_YELLOW (SIDE_A), PRO_DATUM_SIDE_RED (SIDE_B), PRO_DATUM_SIDE_NONE
					in_constraint_type,				// enum PRO_ASM_ALIGN, PRO_ASM_ALIGN_OFF...
					in_offset_between_datums,		// This is only used if in_constraint_type == PRO_ASM_ALIGN_OFF or PRO_ASM_MATE_OFF
					str);
	isis_LOG(lg, isis_FILE, isis_INFO) << str.str();
	isis_LOG(lg, isis_FILE, isis_INFO) << "********** End PopulateOneConstraintInConstraintStructure  ***************" << isis_EOL;
	
	// *******************************************
	// Initialize the path to the base component
	// ********************************************
	ProAsmcomppath	base_comp_path;
	Retrieve_ProAsmcomppath_WithExceptions(*in_assembly_model, in_base_model_path_list, base_comp_path);

	// ***********************************************
	// Initialize the path to the assembled component
	// ***********************************************

	ProAsmcomppath	added_comp_path;
	Retrieve_ProAsmcomppath_WithExceptions(*in_assembly_model, in_added_model_path_list, added_comp_path); 

	// **************************************************
	// Find the address to the base and added components
	// **************************************************
	ProMdl		 base_model;					// typedef void* ProMdl
	ProMdl		 added_model;
	isis::isis_ProAsmcomppathMdlGet( &base_comp_path, &base_model);
	isis::isis_ProAsmcomppathMdlGet( &added_comp_path, &added_model);


	//	Find the assembly datum 
	ProModelitem  base_model_datum;
	isis::isis_ProModelitemByNameInit_WithDescriptiveErrorMsg (
		in_ComponentID, in_Model_Name, mdltype,   // Added arguments
		base_model, in_pro_datum_type, in_base_model_datum_name, &base_model_datum);  // Original arguments

	//	Find the component datum
	ProModelitem  add_model_datum;
	isis::isis_ProModelitemByNameInit_WithDescriptiveErrorMsg (
		in_ComponentID, in_Model_Name, mdltype,   // Added arguments
		added_model,in_pro_datum_type, in_added_model_datum_name, &add_model_datum); // Original arguments

	//	For the assembly reference, initialize a component path.
	//	This is necessary even if the reference geometry is in the assembly.

	//	Allocate the references
	ProSelection base_model_select;
	isis::isis_ProSelectionAlloc(&base_comp_path, &base_model_datum, &base_model_select);

	ProSelection added_model_select;
	isis::isis_ProSelectionAlloc (&added_comp_path, &add_model_datum, &added_model_select);
	//isis::isis_ProSelectionAlloc (NULL, &add_model_datum, &added_model_select);

	//	Allocate and fill the constraint.
	ProAsmcompconstraint constraint;
	isis::isis_ProAsmcompconstraintAlloc (&constraint);

	isis::isis_ProAsmcompconstraintTypeSet (
					constraint, 
					in_constraint_type); // PRO_ASM_ALIGN, PRO_ASM_ALIGN_OFF..

	isis::isis_ProAsmcompconstraintAsmreferenceSet (constraint, base_model_select, in_base_model_datum_side);

	isis::isis_ProAsmcompconstraintCompreferenceSet (constraint, added_model_select, in_added_model_datum_side);
	
	if (  in_constraint_type == PRO_ASM_ALIGN_OFF || 
		  in_constraint_type == PRO_ASM_MATE_OFF )
		  //in_constraint_type == PRO_ASM_LINE_DIST || PRO_ASM_LINE_PARL)
	{
		isis::isis_ProAsmcompconstraintOffsetSet( constraint, in_offset_between_datums );
	}

	// Needed to force kinematic joints
	if (in_constraint_type == PRO_ASM_ALIGN && in_pro_datum_type == PRO_AXIS)  isis::isis_ProAsmcompconstraintAttributesSet( constraint, PRO_ASM_CONSTR_ATTR_FORCE |  PRO_ASM_CONSTR_ATTR_IGNORE);

	isis::isis_ProArrayObjectAdd (
					(ProArray*)&out_constraints,
					-1, // int index, if -1 append the constraint to the end 
					 1, // int number of object to append
					 &constraint);

	int constraintArraySize;
	//isis_ProArraySizeGet( (ProArray*)&out_constraints, &constraintArraySize);
	isis_ProArraySizeGet( out_constraints, &constraintArraySize);
	out_indexOfAddedConstraint = constraintArraySize - 1;

	// The following line is commented out because it will cause an exception later in the code.  Hopefully, isis::isis_ProArrayFree will clear the added constraints. 
	//isis::isis_ProAsmcompconstraintFree(constraint);  
	//isis::isis_ProSelectionFree (&added_model_select);
	//isis::isis_ProSelectionFree (&base_model_select);


} // end PopulateOneConstraintInConstraintStructure


///////////////////////////////////////////////////////////////////////////////////////////////////////
bool ComponentIDChildOf(	const string &in_ChildID,
							const string &in_ParentID,
							std::map<string, isis::CADComponentData> &in_CADComponentData_map )
{
	bool found = false;

	for (	std::list<std::string>::const_iterator i( in_CADComponentData_map[in_ParentID].children.begin() ); 
			i != in_CADComponentData_map[in_ParentID].children.end(); ++i )
	{
		//clog << std::endl << "   ****** in_ComponentID, i->ComponentID(): " << in_ComponentID << "  " << i->ComponentID();
		if ( *i == in_ChildID )
		{
			//clog << std::endl << "   return true;";
			return true;

		}
		else
		{
			found = ComponentIDChildOf( in_ChildID,   *i , in_CADComponentData_map);
		}

		if (found)
		{
			//clog << std::endl << "   return true;";
			return true;
		}

	}
	//clog << std::endl << "   return found:  "  << found;
	return found;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Description: 
//		This function reads the following type of xml and returns the FeatureInterfaceType (i.e. CAD_DATUM, 
//		CAD_MODEL_INTERFACE, or CAD_MODEL_USER_DATA ).  For a <Constraint>, the FeatureInterfaceTypes
//		must be of the same type.  In other words, they must all be CAD_DATUM or they must all be 
//		CAD_MODEL_INTERFACE, or they must all be CAD_MODEL_USER_DATA.  If this rule is violated, an
//		exception would be thrown.
//				
//      <CADComponent ComponentID="100000701" Name="board" Type="PART">
//        <Constraint>
//          <Pair FeatureAlignmentType="ALIGN" FeatureGeometryType="SURFACE" FeatureInterfaceType="CAD_DATUM">
//            <ConstraintFeature ComponentID="100000701" FeatureName="FRONT" FeatureOrientationType="SIDE_A"/>
//            <ConstraintFeature ComponentID="100000036" FeatureName="ASM_FRONT" FeatureOrientationType="SIDE_A"/>
//          </Pair>
//          <Pair FeatureAlignmentType="ALIGN" FeatureGeometryType="SURFACE" FeatureInterfaceType="CAD_DATUM">
//            <ConstraintFeature ComponentID="100000701" FeatureName="TOP" FeatureOrientationType="SIDE_A"/>
//            <ConstraintFeature ComponentID="100000036" FeatureName="ASM_TOP" FeatureOrientationType="SIDE_A"/>
//          </Pair>
//          <Pair FeatureAlignmentType="ALIGN" FeatureGeometryType="SURFACE" FeatureInterfaceType="CAD_DATUM">
//            <ConstraintFeature ComponentID="100000701" FeatureName="RIGHT" FeatureOrientationType="SIDE_A"/>
//            <ConstraintFeature ComponentID="100000036" FeatureName="ASM_RIGHT" FeatureOrientationType="SIDE_A"/>
//          </Pair>
//        </Constraint>
//      </CADComponent>
//
//	Pre-Conditions:
//		in_CADComponent_itr must point to valid CADComponent construct (i.e. created from valid xml).
//
//	Post-Condtions:
//		if all FeatureInterfaceTypes are of one type (i.e. CAD_DATUM, CAD_MODEL_INTERFACE, or CAD_MODEL_USER_DATA) and
//		there is at least one <Pair ..
//		then
//			return FeatureInterfaceType
//		else
//			throw isis::application_exception
//
/*
isis::e_FeatureInterfaceType GetFeatureInterfaceType_enum( 
		AssemblyType::CADComponent_type::CADComponent_const_iterator in_CADComponent_itr )
			throw (isis::application_exception)
{

	isis::e_FeatureInterfaceType  FeatureInterfaceType;
	bool FeatureInterfaceTypeSet = false;
	std::string FeatureInterfaceType_string;

	for ( AssemblyType::CADComponent_type::Constraint_const_iterator j(in_CADComponent_itr->Constraint().begin());
		      j != in_CADComponent_itr->Constraint().end();
				  ++j )
	{
		for ( AssemblyType::CADComponent_type::Constraint_type::Pair_const_iterator k(j->Pair().begin());
					  k != j->Pair().end();
					  ++k )

	//if ( in_CADComponent_itr->Constraint().present() )
	//{
	//	for ( AssemblyType::CADComponent_type::Constraint_type::Pair_const_iterator k(in_CADComponent_itr->Constraint().get().Pair().begin());
	//				  k != in_CADComponent_itr->Constraint().get().Pair().end();
	//				  ++k )
		{
			if (FeatureInterfaceTypeSet )
			{
				if ( FeatureInterfaceType != FeatureInterfaceType_enum( k->FeatureInterfaceType()) )
				{
					string err_str = "Erroneous constraint FeatureInterfaceType (e.g. CAD_DATUM, CAD_MODEL_INTERFACE, CAD_MODEL_USER_DATA).  Within a <Constraint>, all FeatureInterfaceTypes must be of the same type.  Encountered types: " +
					FeatureInterfaceType_string + " and " + k->FeatureInterfaceType() + " in ComponentID: " +   in_CADComponent_itr->ComponentID();
					throw isis::application_exception(err_str.c_str());  
				}
			}
			else
			{
				FeatureInterfaceType = FeatureInterfaceType_enum( k->FeatureInterfaceType());
				FeatureInterfaceType_string = k->FeatureInterfaceType();
				FeatureInterfaceTypeSet = true;
			}

		}

		if ( !FeatureInterfaceTypeSet )
		{
			string err_str = "ComponentID: " +   in_CADComponent_itr->ComponentID() + "does not have constraints defined of the type CAD_DATUM, CAD_MODEL_INTERFACE, or CAD_MODEL_USER_DATA"; 
			throw isis::application_exception(err_str.c_str());  
		}
	}  // end if
	
	return FeatureInterfaceType;
}
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum e_ConstraintAllowAssumptions
{
	CAD_CONSTRAINT_ALLOW_ASSUMPTIONS_TRUE = 0,
	CAD_CONSTRAINT_ALLOW_ASSUMPTIONS_FALSE = 64
};


std::string ConstraintAllowAssumptionse_string(  e_ConstraintAllowAssumptions in_ConstraintAllowAssumptions_enum)
											throw (isis::application_exception)
{
	switch (in_ConstraintAllowAssumptions_enum )
	{
		case CAD_CONSTRAINT_ALLOW_ASSUMPTIONS_TRUE:
			return "CAD_CONSTRAINT_ALLOW_ASSUMPTIONS_TRUE";
			break;
		case CAD_CONSTRAINT_ALLOW_ASSUMPTIONS_FALSE:
			return "CAD_CONSTRAINT_ALLOW_ASSUMPTIONS_FALSE";
			break;
		default:
			char temp_char_array[32];
			string temp_string = "Function ProModelType_string was passed " + 
				std::string(itoa(in_ConstraintAllowAssumptions_enum, temp_char_array, 10)) + 
				" which is an erroneous type.  Allowed enum values are CAD_CONSTRAINT_ALLOW_ASSUMPTIONS_TRUE and CAD_CONSTRAINT_ALLOW_ASSUMPTIONS_FALSE.";
			throw isis::application_exception(temp_string.c_str());
	}
}

/*
void CreateConstriantSet_and_SetAllowAssumptions ( ProAsmcomppath *assem_path,  ProAsmcomp *in_ProAsmcomp, e_ConstraintAllowAssumptions in_AllowAssumptions )
{
	ProElement created_elemtree;	
	//isis::isis_ProFeatureElemtreeExtract (	in_ProAsmcomp,
	//										assem_path,
	//										PRO_FEAT_EXTRACT_NO_OPTS,
	//										&elemTree );

	//err = ProFeatureElemtreeExtract (&p_feature,NULL,PRO_FEAT_EXTRACT_NO_OPTS,&created_elemtree);	
	ProError err;
	isis::isis_ProFeatureElemtreeExtract (in_ProAsmcomp,assem_path,PRO_FEAT_EXTRACT_NO_OPTS,&created_elemtree);	


	ProElempathItem path_items [] =  {{ PRO_ELEM_PATH_ITEM_TYPE_ID, PRO_E_COMPONENT_MISC_ATTR}};
	ProElempath   path;
	ProElement   allow_assumptions;
	isis::isis_ProElempathAlloc (&path);			
	isis::isis_ProElempathDataSet (path, path_items, 1);
	isis::isis_ProElemtreeElementGet (created_elemtree, path, &allow_assumptions);
	isis::isis_ProElementIntegerSet(allow_assumptions, 64);
	isis::isis_ProElempathFree (&path);

	//PRO_E_COMPONENT_SETS
	ProElempathItem sets_path_Item[] = {{PRO_ELEM_PATH_ITEM_TYPE_ID, PRO_E_COMPONENT_SETS}};
	ProElement   sets;
	ProElempath   setsPath;
	isis::isis_ProElempathAlloc (&setsPath);			
	isis::isis_ProElempathDataSet (setsPath, sets_path_Item, 1);
	isis::isis_ProElemtreeElementGet (created_elemtree, setsPath, &sets);

	//Add a PRO_E_COMPONENT_SET to PRO_E_COMPONENT_SETS
	ProElement   compSet;
	isis::isis_ProElementAlloc(PRO_E_COMPONENT_SET, &compSet);
	isis::isis_ProElemtreeElementAdd(sets, NULL, compSet);

	//Add PRO_E_COMPONENT_SET_ID
	ProElement   comSetId;
	isis::isis_ProElementAlloc(PRO_E_COMPONENT_SET_ID, &comSetId);
	isis::isis_ProElementIntegerSet(comSetId, 1);
	isis::isis_ProElemtreeElementAdd(compSet, NULL, comSetId);

	// Add PRO_E_COMPONENT_SET_NAME
	ProElement   comSetName;
	isis::isis_ProElementAlloc(PRO_E_COMPONENT_SET_NAME, &comSetName);
	isis::isis_ProElementWstringSet(comSetName, L"SET123");
	isis::isis_ProElemtreeElementAdd(compSet, NULL, comSetName);

	//Add PRO_E_COMPONENT_SET_TYPE
	ProElement   comSetType;
	isis::isis_ProElementAlloc(PRO_E_COMPONENT_SET_TYPE, &comSetType);
	isis::isis_ProElementIntegerSet(comSetType, PRO_ASM_SET_USER_DEFINED_TYPE);
	isis::isis_ProElemtreeElementAdd(compSet, NULL, comSetType);

	//Add PRO_E_COMPONENT_SET_MISC_ATTR
	ProElement   comSetMiscAttr;
	isis::isis_ProElementAlloc(PRO_E_COMPONENT_SET_MISC_ATTR, &comSetMiscAttr);
	isis::isis_ProElementIntegerSet(comSetMiscAttr, PRO_ASM_ENABLE_COMP_SET);
	isis::isis_ProElemtreeElementAdd(compSet, NULL, comSetMiscAttr);	

	//ProElement   compConstrs;
	ProElempathItem compConstrsPathItem [] =   {{ PRO_ELEM_PATH_ITEM_TYPE_ID, PRO_E_COMPONENT_CONSTRAINTS}};
	ProElempath   compConstrspath;	
	isis::isis_ProElempathAlloc (&compConstrspath);			
	err = ProElempathDataSet (compConstrspath, compConstrsPathItem, 1);	

	if (err == PRO_TK_NO_ERROR)
	{
		int n_constraints = 0;
		ProBoolean isArray = PRO_B_FALSE;
		err = ProElementIsArray(created_elemtree, compConstrspath, &isArray);		
		if (err == PRO_TK_NO_ERROR && isArray == PRO_B_TRUE)
		{
			err = ProElementArrayCount(created_elemtree, compConstrspath, &n_constraints);
		}

		//update PRO_E_COMPONENT_CONSTR_SET_ID
		for (int iCnt = 0; iCnt < n_constraints ; iCnt++)
		{			
			ProElempathItem set_id_path[]= {{PRO_ELEM_PATH_ITEM_TYPE_ID, PRO_E_COMPONENT_CONSTRAINTS},
			{PRO_ELEM_PATH_ITEM_TYPE_INDEX, iCnt},
			{PRO_ELEM_PATH_ITEM_TYPE_ID, PRO_E_COMPONENT_CONSTR_SET_ID}};

			ProElement   compConstr;
			ProElempath   compConstrpath;
			err = ProElempathAlloc (&compConstrpath);			
			err = ProElempathDataSet (compConstrpath, set_id_path, sizeof(set_id_path)/sizeof(set_id_path[0]));
			err = ProElemtreeElementGet (created_elemtree, compConstrpath, &compConstr);
			err = ProElementIntegerSet(compConstr, 0);
			err = ProElempathFree (&compConstrpath);
		}
	}

	ProFeatureCreateOptions *opts;
	isis::isis_ProArrayAlloc(1, sizeof (ProFeatureCreateOptions), 1, (ProArray*) &opts);
	opts[0] = PRO_FEAT_CR_DEFINE_MISS_ELEMS;

	ProErrorlist errList;
	//err = ProFeatureRedefine(NULL,&p_feature,created_elemtree,opts,1,&errList);	
	//err = ProFeatureWithoptionsRedefine(NULL, &p_feature, created_elemtree, opts, PRO_REGEN_NO_FLAGS, &errList);
	isis::isis_ProFeatureWithoptionsRedefine(assem_path, in_ProAsmcomp, created_elemtree, opts, PRO_REGEN_NO_FLAGS, &errList);

	
	//isis::isis_ProFeatureWithoptionsRedefine( 
	//			assem_path,
	//			//NULL,  
	//			in_ProAsmcomp,
	//			elemTree,
	//			opts,
	//			PRO_REGEN_NO_FLAGS,
	//			//PRO_REGEN_UPDATE_ASSEMBLY_ONLY,
	//			&p_errors); 
	
}
*/

// From the CREO HELP CENTER:
// Constraint Orientation Assumptions
 
// When Allow Assumptions is selected (the default) during component assembly, constraint orientation assumptions are 
// automatically made. For example, only two coincident constraints are required to fully constrain a bolt to a hole 
// in a plate. After an Coincident constraint has been defined between the axes of the hole and the bolt and a Coincident 
// constraint has been defined between the bottom face of the bolt and the top face of the plate, the system assumes 
// a third constraint. This constraint controls rotation about the axes, thereby fully constraining the component. 

// When the Allow Assumptions check box is cleared, the third constraint must be defined before the component is 
// considered fully constrained. You can leave the bolt packaged, or you can create another constraint that explicitly 
// constrains the bolt’s rotational degree of freedom. 

// When Allow Assumptions is disabled, you can use the Move tab options to move a component out of a previously 
// assumed position, where it will remain. The component automatically snaps back to the assumed position if 
// you select the Allow Assumptions check box again. 


void SetConstraint_AllowAssumptions ( ProAsmcomppath *assem_path,  ProAsmcomp *in_ProAsmcomp, e_ConstraintAllowAssumptions in_AllowAssumptions )
{
	

	//int AllowAssumptions_flag = 0;  // This is the CAD_CONSTRAINT_ALLOW_ASSUMPTIONS_TRUE case
	//if ( in_AllowAssumptions == CAD_CONSTRAINT_ALLOW_ASSUMPTIONS_FALSE ) AllowAssumptions_flag = 64;

	ProElement elemTree;

	isis::isis_ProFeatureElemtreeExtract (	in_ProAsmcomp,
											assem_path,
											PRO_FEAT_EXTRACT_NO_OPTS,
											&elemTree );

	ProElempath elemPath;

	isis::isis_ProElempathAlloc(&elemPath);

	ProElempathItem pathItem[3];
	pathItem[0].type = PRO_ELEM_PATH_ITEM_TYPE_ID;
	//pathItem[0].path_item.elem_id = PRO_E_COMPONENT_MODEL  ;
	pathItem[0].path_item.elem_id = PRO_E_COMPONENT_MISC_ATTR;
			
	isis::isis_ProElempathDataSet(elemPath, pathItem, 1);
	ProElement elem_component_misc_attr;
	isis::isis_ProElemtreeElementGet( elemTree, elemPath, &elem_component_misc_attr);

	
					
	ProValue  val_component_misc_attr;
	isis::isis_ProElementValueGet( elem_component_misc_attr, &val_component_misc_attr );
	ProValueData value_data;
	isis::isis_ProValueDataGet(val_component_misc_attr, &value_data);

	isis_LOG(lg, isis_FILE, isis_INFO) << "\nPRO_E_COMPONENT_MISC_ATTR ProValueDataGet, type: " << value_data.type;
	isis_LOG(lg, isis_FILE, isis_INFO) << "\nPRO_E_COMPONENT_MISC_ATTR ProValueDataGet, v: "    <<  value_data.v.i;

	//std::cout << std::endl << "\n   Type:       " << FeatureGeometryType_string(in_ProAsmcomp->type);

	if ( value_data.v.i == in_AllowAssumptions )
	{
		isis_LOG(lg, isis_FILE, isis_INFO) << "\nAllowAssumptions already set to: " << ConstraintAllowAssumptionse_string(in_AllowAssumptions) << ".  For:";
		isis_LOG(lg, isis_FILE, isis_INFO) << "\n   Feature ID: " << in_ProAsmcomp->id;
		isis_LOG(lg, isis_FILE, isis_INFO) << "\n   Owner:      " << (const void*)in_ProAsmcomp->owner;
		isis_LOG(lg, isis_FILE, isis_INFO) << "\n   Type:       " << FeatureGeometryType_string(in_ProAsmcomp->type);
		
	}
	else
	{
		///////////////////////////////////////////////////////
		// Set the element tree to have in_AllowAssumptions
		///////////////////////////////////////////////////////

		value_data.v.i = in_AllowAssumptions;
		isis::isis_ProValueDataSet( val_component_misc_attr, &value_data );
	
		isis::isis_ProElementValueSet( elem_component_misc_attr, val_component_misc_attr );
	}

/*	
	///////////////////////////////////////////////////////
	// Add the Constraint Set
	///////////////////////////////////////////////////////
	
	pathItem[0].path_item.elem_id = PRO_E_COMPONENT_SETS;
	isis::isis_ProElempathDataSet(elemPath, pathItem, 1);

	ProElement elem_component_sets;

	isis::isis_ProElemtreeWrite(elemTree, PRO_ELEMTREE_XML, L"Junk_1.xml");
	
	isis::isis_ProElemtreeElementGet( elemTree, elemPath, &elem_component_sets );

	//isis::isis_ProElemtreeWrite(elemTree, PRO_ELEMTREE_XML, L"Junk_2.xml");

	///////////////////////////////////////////////////////
	// Add the Constraint Set
	///////////////////////////////////////////////////////
	//isis::isis_ProElemtreeWrite(elemTree, PRO_ELEMTREE_XML, L"Junk_1.xml");
	//ProElement comp_constr_set_elem;	
	//isis::isis_ProElementAlloc (PRO_E_COMPONENT_SET, &comp_constr_set_elem);  
	//isis::isis_ProElemtreeElementAdd (elemTree, NULL, comp_constr_set_elem ); 
	//isis::isis_ProElemtreeWrite(elemTree, PRO_ELEMTREE_XML, L"Junk_2.xml");

	isis::isis_ProElemtreeWrite(elemTree, PRO_ELEMTREE_XML, L"Junk_2.xml");

	////////////////////////////////
	// Add PRO_E_COMPONENT_SETS
	////////////////////////////////
	ProElement comp_constr_sets_elem;	
	isis::isis_ProElementAlloc (PRO_E_COMPONENT_SETS, &comp_constr_sets_elem);  
	isis::isis_ProElemtreeElementAdd (elemTree, NULL, comp_constr_sets_elem ); 

	isis::isis_ProElemtreeWrite(elemTree, PRO_ELEMTREE_XML, L"Junk_3.xml");

	////////////////////////////////
	// Add PRO_E_COMPONENT_SET
	////////////////////////////////
	ProElement comp_constr_set_elem;	
	isis::isis_ProElementAlloc (PRO_E_COMPONENT_SET, &comp_constr_set_elem);  
	isis::isis_ProElemtreeElementAdd (comp_constr_sets_elem, NULL, comp_constr_set_elem ); 

	
	////////////////////////////////
	// Add PRO_E_COMPONENT_SET
	////////////////////////////////	
	isis::isis_ProElementAlloc (PRO_E_COMPONENT_SET, &comp_constr_set_elem);  
	isis::isis_ProElemtreeElementAdd (comp_constr_set_elem, NULL, comp_constr_set_elem ); 

	////////////////////////////////
	// Add a PRO_E_COMPONENT_SET_ID
	////////////////////////////////
	ProElemId elemid;
	elemid = PRO_E_COMPONENT_SET_ID;
	value_data.type = PRO_VALUE_TYPE_INT;
	value_data.v.i = 1 ;
	ProElement comp_setId_elem;
	isis::isis_AddElementtoElemTree(elemid, &comp_constr_set_elem,&value_data, &comp_setId_elem);

	//////////////////////////////////
	// Add a PRO_E_COMPONENT_SET_TYPE
	//////////////////////////////////
	elemid = PRO_E_COMPONENT_SET_TYPE;
	value_data.type = PRO_VALUE_TYPE_INT;
	value_data.v.i = PRO_ASM_SET_USER_DEFINED_TYPE ;
	ProElement comp_setType_elem;
	isis::isis_AddElementtoElemTree(elemid, &comp_constr_set_elem,&value_data, &comp_setType_elem);

	////////////////////////////////////////
	// Add a PRO_E_COMPONENT_CONSTR_SET_ID
	////////////////////////////////////////
	elemid = PRO_E_COMPONENT_CONSTR_SET_ID;
	value_data.type = PRO_VALUE_TYPE_WSTRING;
	value_data.v.w = (wchar_t*) malloc (sizeof(ProName)); 
	// value_data.v.w = new wchar_t (sizeof(ProName));  // WARNING, Don't use new, it causes a memory exception
	wcscpy( value_data.v.w, L"Set2");
	ProElement comp_setTID_elem;
	isis::isis_AddElementtoElemTree(elemid, &comp_constr_set_elem,&value_data, &comp_setTID_elem);
	isis::isis_ProElemtreeWrite(elemTree, PRO_ELEMTREE_XML, L"Junk_4.xml");
	*/
	///////////////////////////////////////////////////////
	// Commit the element tree
	///////////////////////////////////////////////////////

	ProFeatureCreateOptions *opts;  
       isis::isis_ProArrayAlloc(1, sizeof (ProFeatureCreateOptions), 1, (ProArray*) &opts);    
       opts[0] = PRO_FEAT_CR_DEFINE_MISS_ELEMS;    

	ProErrorlist p_errors;

	isis::isis_ProFeatureWithoptionsRedefine( 
				//assem_path,
				NULL,  
				in_ProAsmcomp,
				elemTree,
				opts,
				PRO_REGEN_NO_FLAGS,
				//PRO_REGEN_UPDATE_ASSEMBLY_ONLY,
				&p_errors); 

	isis::isis_ProElempathFree(&elemPath);
	isis::isis_ProArrayFree( (ProArray*) &opts);


}

std::string toString(const wchar_t* name) {
	char rename[PRO_NAME_SIZE+1];
	ProWstringToString(rename, const_cast<wchar_t*>(name));
	return rename;
}


/*
static void CollectResolveInfo(ProSolid in_TopAssemblyModel, const std::map<string, isis::CADComponentData> &in_CADComponentData_map, const PerSetConstraintDefinition &constrdef, ConstraintData& constrdata)
{
	for each (const ContraintFeatureDefinition &def in constrdef.contraintFeatureDefinitions)
	{
		// Skip guides - guides won't appear in final connections or joint defs
		if (def.treatConstraintAsAGuide==CAD_TREAT_CONSTRAINT_AS_A_GUIDE_TRUE)
			continue;

		ProAsmcomppath	base_comp_path;
		isis::Retrieve_ProAsmcomppath_WithExceptions(in_TopAssemblyModel, def.base_model_path_list, base_comp_path);

		ProAsmcomppath	added_comp_path;
		isis::Retrieve_ProAsmcomppath_WithExceptions(in_TopAssemblyModel, def.added_model_path_list, added_comp_path);

		// **************************************************
		// Find the addresse to the base and added compoents
		// **************************************************
		ProMdl		 base_model;					// typedef void* ProMdl
		ProMdl		 added_model;
		isis::isis_ProAsmcomppathMdlGet( &base_comp_path, &base_model);
		isis::isis_ProAsmcomppathMdlGet( &added_comp_path, &added_model);

		//	Find the assembly datum 
		ProModelitem  base_model_datum;
		isis::MultiFormatString  temp_string(def.base_model_datum_name);

		//std::cout << std::endl << "+++ Base model datum name: " << temp_string << std::endl;
		isis::isis_ProModelitemByNameInit_WithDescriptiveErrorMsg (	def.base_model_component_instance_ID, // Added arguments
																	def.base_model_name, 
																	def.base_model_type,   
																	//in_ContraintDef.p_base_model, //base_model, // Original arguments
																	base_model, //base_model, // Original arguments
																	def.pro_datum_type, 
																	def.base_model_datum_name, 
																	&base_model_datum);  

		//	Find the component datum
		ProModelitem  add_model_datum;

		temp_string = def.added_model_datum_name;
		//std::cout << std::endl << "+++ Added model datum name: " << temp_string << std::endl;

		isis::isis_ProModelitemByNameInit_WithDescriptiveErrorMsg (	def.added_model_component_instance_ID, // Added arguments
																	def.added_model_name, 
																	def.added_model_type,   
																	//in_ContraintDef.p_added_model, //added_model, // Original arguments
																	added_model,
																	def.pro_datum_type, 
																	def.added_model_datum_name, 
																	&add_model_datum);  

		//	Allocate the selections
		ProSelection base_model_select;
	
		ValidatePathAndModelItem_ThrowExceptionIfInvalid(base_comp_path, base_model_datum);
		isis::isis_ProSelectionAlloc(&base_comp_path, &base_model_datum, &base_model_select);

		ProSelection added_model_select;

		ValidatePathAndModelItem_ThrowExceptionIfInvalid(added_comp_path, add_model_datum);
		isis::isis_ProSelectionAlloc (&added_comp_path, &add_model_datum, &added_model_select);

		// There's nothing to resolve in case of a PART
		if (def.base_model_type == PRO_MDL_ASSEMBLY)
		{
			DatumRefResolver resolver_base(base_model_select);
			vector<ProSelection> list_base;
			resolver_base.Resolve(list_base);
			// This connects to an assembly. The part which this assembly connects to has been resolved
			for each (ProSelection sel in list_base)
			{
				ProAsmcomppath comppath;
				isis_ProSelectionAsmcomppathGet(sel, &comppath);
				list<int> pathaslist;
				isis::ProAsmcomppathToList(pathaslist, comppath);
				constrdata.constrainedTo_Paths.insert(pathaslist);
			}
		}

		if (def.added_model_type == PRO_MDL_ASSEMBLY)
		{
			DatumRefResolver resolver_added(added_model_select);
			vector<ProSelection> list_added;
			resolver_added.Resolve(list_added);
			// This is an assembly. The part which this assembly connects from has been resolved			
			for each (ProSelection sel in list_added)
			{
				ProAsmcomppath comppath;
				isis_ProSelectionAsmcomppathGet(sel, &comppath);
				list<int> pathaslist;
				isis::ProAsmcomppathToList(pathaslist, comppath);
				constrdata.constrainedFrom_Paths.insert(pathaslist);
			}
		}
	}

}
*/


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Description:
//		This function populates out_PerSetConstraintDefinitions with the constraint information organized per 
//		constraint set.  A constraint set is a separate grouping of constraints within a Creo constraint definition.  
//		A major role of this function is to determine if the particular constraint feature is associated with the 
//		base_model or added_model. The base_model is a model that is already in the assembly.  The added_model would be 
//		constrained to the base_model.  Sometimes the base_model would be the parent assembly, which is the case 
//		for the first model added to the assembly. For other cases, the base_model would be a model already in the 
//		assembly.
//		See Note 1 (i.e. Reference [1]) at the top of this file for more details about the added/base models.
// Pre-Conditions
//		in_ComponentID must be a valid ComponentID in in_CADComponentData_map
// Post-Conditons
//		if exception
//			then
//				isis::application_exception will be thrown.
//			else
//				if in_ConstraintDefinition.constraints.size() == 0
//					then
//						out_PerSetConstraintDefinitions will be empty
//					else
//						out_PerSetConstraintDefinitions will be populated.
void Populate_PerSetConstraintDefinitions( 
				const std::string								&in_ComponentID,		
				const ConstraintDefinition						&in_ConstraintDefinition,
				std::map<string, isis::CADComponentData>		&in_CADComponentData_map,
				std::vector<PerSetConstraintDefinition_2>		&out_PerSetConstraintDefinitions) throw (isis::application_exception)
{
		
		
		isis_LOG(lg, isis_FILE, isis_INFO) << "";

		out_PerSetConstraintDefinitions.clear();

		//ProAsmcomp		added_model_assembled_feature;
		//const isis::CADComponentData &ComponentAssembledInfo_temp = in_CADComponentData_map[in_ComponentID];
		//added_model_assembled_feature = ComponentAssembledInfo_temp.assembledFeature;
	
		for (	std::vector<ConstraintData>::const_iterator j(in_ConstraintDefinition.constraints.begin());
				j != in_ConstraintDefinition.constraints.end();
				++j )
		{		
			PerSetConstraintDefinition_2 perSetConDef;

			perSetConDef.set_JointType_withoutguide(j->computedJointData.jointType_withoutguide);
			perSetConDef.set_HasAGuideConstraint(j->hasAGuideConstraint());
			perSetConDef.set_JointType_withguide( j->computedJointData.jointType_withguide);
			perSetConDef.set_JointMotionLimits(j->inputJoint);

			//ProAsmcomp	 base_model_assembled_feature;	
			list<int>  base_model_path_list;
			list<int>  added_model_path_list;
				
	   		for (	std::vector<ConstraintPair>::const_iterator k(j->constraintPairs.begin());
					k != j->constraintPairs.end();
					++k )
			{
				ProAsmcompConstrType constraint_type = ProAsmcompConstrType_enum(k->featureAlignmentType);

				ProType	 pro_datum_type =  FeatureGeometryType_enum(k->featureGeometryType);	
				//ProType	 pro_datum_type =  k->featureGeometryType;	

				//ProName		 base_model_datum_name;	 // ASM_RIGHT, A_1..
				MultiFormatString base_model_datum_name(PRO_NAME_SIZE - 1);  // ASM_RIGHT, A_1..
				std::string		  base_model_component_instance_ID;
				ProDatumside base_model_datum_side;	 // enum PRO_DATUM_SIDE_YELLOW (SIDE_A), PRO_DATUM_SIDE_RED (SIDE_B), PRO_DATUM_SIDE_NONE
						

				//ProName		 added_model_datum_name; // RIGHT, A23 ..
				MultiFormatString		 added_model_datum_name(PRO_NAME_SIZE - 1);  // ASM_RIGHT, A_1..
				std::string				added_model_component_instance_ID;
				ProDatumside added_model_datum_side; // enum PRO_DATUM_SIDE_YELLOW (SIDE_A), PRO_DATUM_SIDE_RED (SIDE_B), PRO_DATUM_SIDE_NONE	

				bool added_model_defined = false;
				bool base_model_defined = false;
				string added_model_constraint_feature_component_ID;
				string base_model_constraint_feature_component_ID;
				isis_LOG(lg, isis_FILE, isis_INFO) << "";

				std::string constraintFeature_From_To = "      ";
				for (	std::vector<ConstraintFeature>::const_iterator l(k->constraintFeatures.begin());
						l != k->constraintFeatures.end();
						++l )
				{												
					// See Note 1 (i.e. Reference [1]) at the top of this file for an explanation of the following if statement.
						
					if ( !added_model_defined  && 
							( l->componentInstanceID == in_ComponentID || ComponentIDChildOf( l->componentInstanceID, in_ComponentID, in_CADComponentData_map ) )
							) 
					{   // treat as added model constraint							
						added_model_path_list = in_CADComponentData_map[l->componentInstanceID].componentPaths;
						// Note:l->featureName is enforced by XMLToProEStructures.cpp(SetConstraintAttributes)
						//	    to be no more than 31 characters.		
						added_model_component_instance_ID = l->componentInstanceID;
						added_model_datum_name = l->featureName;
						added_model_datum_side = ProDatumside_enum(l->featureOrientationType);
						added_model_defined = true;
						added_model_constraint_feature_component_ID = l->componentInstanceID;
						constraintFeature_From_To += (std::string)in_CADComponentData_map[l->componentInstanceID].name + "::" + (std::string)l->featureName;
						if (!base_model_defined)
						{ 
							constraintFeature_From_To += " --> ";
						}
					}
					else
					{	// constraint features on a component in the assembly.
						base_model_path_list = in_CADComponentData_map[l->componentInstanceID].componentPaths;
						// Note:l->featureName is enforced by XMLToProEStructures.cpp(SetConstraintAttributes)
						//	    to be no more than 31 characters.		
						base_model_component_instance_ID = l->componentInstanceID;
						base_model_datum_name = l->featureName;
						base_model_datum_side = ProDatumside_enum(l->featureOrientationType);
						base_model_defined = true;
						base_model_constraint_feature_component_ID = l->componentInstanceID;
						constraintFeature_From_To += (std::string)in_CADComponentData_map[l->componentInstanceID].name + "::" + (std::string)l->featureName;
						if (!added_model_defined) 
						{ 
							constraintFeature_From_To += " --> ";
						}
					}		
					
				}
				isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << constraintFeature_From_To;

				if ( !base_model_defined || !added_model_defined)
				{

					string err_str = "Erroneous constraint pair.  Assembled Component Instance ID: " + in_ComponentID + 
										" Constraint Feature Component IDs: " + added_model_constraint_feature_component_ID +
										", " + base_model_constraint_feature_component_ID;
					throw isis::application_exception(err_str.c_str());  
				}
				
				double offset_between_datums = 0;
				if ( k->constraintOffsetPresent )
				{	
					offset_between_datums = k->constraintOffset.value;
					isis_LOG(lg, isis_FILE, isis_INFO) << "      ConstraintOffset Value:               " <<  k->constraintOffset.value;
					isis_LOG(lg, isis_FILE, isis_INFO) << "      ConstraintOffset OffsetAlignmentType: " <<  ProAsmcompConstrType_string( k->constraintOffset.offsetAlignmentType );
					if ( k->constraintOffset.unitsPresent )
					{
						isis_LOG(lg, isis_FILE, isis_INFO) << "      ConstraintOffset Units Type:          " << CADUnitsDistance_string( k->constraintOffset.units );
					}
				}				
				
				ContraintFeatureDefinition_2 cFDef;

				cFDef.p_base_model =						(ProMdl)in_CADComponentData_map[base_model_component_instance_ID].cADModel_hdl;
				cFDef.base_model_name =						in_CADComponentData_map[base_model_component_instance_ID].name;
				cFDef.base_model_type =						ProMdlType_enum(in_CADComponentData_map[base_model_component_instance_ID].modelType);
				cFDef.base_model_component_instance_ID =	base_model_component_instance_ID;

				cFDef.added_model_name =					in_CADComponentData_map[added_model_component_instance_ID].name;
				cFDef.added_model_type =					ProMdlType_enum(in_CADComponentData_map[added_model_component_instance_ID].modelType);
				cFDef.added_model_component_instance_ID =	added_model_component_instance_ID;

				cFDef.p_added_model =						(ProMdl)in_CADComponentData_map[added_model_component_instance_ID].cADModel_hdl;
				cFDef.base_model_path_list		= base_model_path_list;			// list of Creo feature IDs leading to the part/assembly
				cFDef.added_model_path_list		= added_model_path_list;		// list of Creo feature IDs  leading to the part/assembly
				cFDef.pro_datum_type			= pro_datum_type;				// enum PRO_SURFACE, PRO_AXIS
				wcscpy_s(cFDef.base_model_datum_name,base_model_datum_name);		// ASM_RIGHT, A_1..
				cFDef.base_model_datum_side		= base_model_datum_side;		// enum PRO_DATUM_SIDE_YELLOW (SIDE_A), PRO_DATUM_SIDE_RED (SIDE_B), PRO_DATUM_SIDE_NONE
				wcscpy_s(cFDef.added_model_datum_name, added_model_datum_name);	// RIGHT, A23 ..
				cFDef.added_model_datum_side	= added_model_datum_side;		// enum PRO_DATUM_SIDE_YELLOW (SIDE_A), PRO_DATUM_SIDE_RED (SIDE_B), PRO_DATUM_SIDE_NONE
				cFDef.constraint_type			= constraint_type;				// enum PRO_ASM_ALIGN, PRO_ASM_ALIGN_OFF...
				cFDef.offset_between_datums		= offset_between_datums;	 

				cFDef.treatConstraintAsAGuide =  k->treatConstraintAsAGuide;

				perSetConDef.add_ContraintFeatureDefinition(cFDef);					

			}  // END for (std::vector<ConstraintPair>::const_iterator k(j->constraintPairs.begin());
			out_PerSetConstraintDefinitions.push_back(perSetConDef);

		} // END for ( std::vector<ConstraintData>::iterator j(in_ConstraintDefinition.constraints.begin());
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Description:
//  This function sorts the constraint features in the order defined by in_SortOrderList.  For example, if in_SortOrderList 
//  contained PRO_CSYS and PRO_POINT, then coordinate systems and datum planes indexes would appear first in 
//  out_ConstraintFeatureOrder_indexes.  The indexes are interger offsets, starting with 0, into the feature definitions 
//  in in_perSetConstraintDefinitions.
//
// Pre-Conditions
//	None
// Post-Conditions
//	if in_SortOrderList.size() == 0 
//		then
//			begin/end_SortedConstraintIDs would return the IDs in the order of
//			begin/end_ContraintFeatureDefinitions 
//		else
//			begin/end_SortedConstraintIDs would return the IDs in the order
//			specified by in_SortOrderList
//		
//	Notes:
//		1) in_SortOrderList does not need to contain all of the ProTypes(PRO_POINT, PRO_SURFACE, 
//		PRO_CSYS, and PRO_AXIS) used to constrain models.  For example, if SortOrderList only contained 
//		PRO_AXIS, then any PRO_AXIS types would appear first in the 
//		begin/end_SortedConstraintIDs followed by the other types in their original order.
//		2) Calling sort a second time will overwirte the previously sorted data
//		3) if in_IncludeGuides == False then constraint features designated as a guide would be excluded.

void SortConstraintFeatures( const PerSetConstraintDefinition_2		&in_perSetConstraintDefinitions,
							 const std::vector<ProType>				&in_SortOrderList,
							 bool									in_IncludeGuides,
							 std::vector<int>						&out_ConstraintFeatureOrder_indexes )
{
	out_ConstraintFeatureOrder_indexes.clear();
    std::set<int> usedIndexes;

	for each ( const ProType &i in in_SortOrderList )
	{
		int constraintFeatureIndex = 0;
		for (	std::vector<ContraintFeatureDefinition_2>::const_iterator j =  in_perSetConstraintDefinitions.begin_ContraintFeatureDefinitions();
				j != in_perSetConstraintDefinitions.end_ContraintFeatureDefinitions();
				++j )
		{
			if (j->pro_datum_type == i) 
			{
				if ( in_IncludeGuides || j->treatConstraintAsAGuide == CAD_TREAT_CONSTRAINT_AS_A_GUIDE_FALSE )
				{
					if ( usedIndexes.find(constraintFeatureIndex) == usedIndexes.end() )
					{
						usedIndexes.insert(constraintFeatureIndex);
						out_ConstraintFeatureOrder_indexes.push_back(constraintFeatureIndex);
					}
				}
			}
			++constraintFeatureIndex;
		}	
	} 

	// in_SortOrderList may not have contained all the types (e.g. surfact, point, csys...)
	// Must add to contraintFeatureDefinitions any skipped entries
	int constraintFeatureIndex = 0;
	for (	std::vector<ContraintFeatureDefinition_2>::const_iterator j =  in_perSetConstraintDefinitions.begin_ContraintFeatureDefinitions();
			j != in_perSetConstraintDefinitions.end_ContraintFeatureDefinitions();
			++j )
	{
		if ( usedIndexes.find(constraintFeatureIndex) == usedIndexes.end() )
		{
			if ( in_IncludeGuides || j->treatConstraintAsAGuide == CAD_TREAT_CONSTRAINT_AS_A_GUIDE_FALSE )
			{
				out_ConstraintFeatureOrder_indexes.push_back(constraintFeatureIndex);
			}
		}
		++constraintFeatureIndex;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	See "struct ConstraintOrder" for an explanation of the contents of out_ConstraintOrder
//	See SortConstraintFeatures(...) for an explanation of how features are sorted
//	out_ConstraintOrder.constraintDefinitionOrder is the order of PerSetConstraintDefinition_2
//		PerSetConstraintDefinition_2s are sorted as follows:
//			if in_ConstraintSetsWithGuidesFirst 
//				then
//					guide constraint features come first, if multiple guide constraints the guide constraints with the most feature would appear first.
//					next would be the no-guide constraints with the constraints with the most features first.
//				else
//					the constraints (without regard to guide status) with the constraints with the most features first.
//
//	Notes:
//		1.	This sort routine maintains the order of the CADAssembly.xml file for the case where multiple constraints have the
//			same number of features.
//		2.	in_ConstraintSetsWithGuidesFirst is independent of in_IncludeGuideFeactures.  For example, if a constraint had
//			guides and in_ConstraintSetsWithGuidesFirst == true, then that constraint would be first in the sort order even
//			though the feature(s) labeled as guide would not be included if in_IncludeGuideFeactures == false.
//		3.	This function empties the contents of out_ConstraintOrder before adding new content.
void ComputeConstraintSortOrder (	
	const std::vector<std::vector<ProType>>			&in_SortOrderLists,   // There is a seperate sort order list for each constraint set
	bool											in_ConstraintSetsWithGuidesFirst,
	bool											in_IncludeGuideFeactures,
	const std::vector<PerSetConstraintDefinition_2>	&in_perSetConstraintDefinitions, 
	ConstraintOrder									&out_ConstraintOrder )
{
	if ( in_SortOrderLists.size() != in_perSetConstraintDefinitions.size() )
	{
		std::stringstream errorString;
		errorString << "Function - " << __FUNCTION__ << ", was passed asortOrderLists of a different size than in_perSetConstraintDefinitions";
			throw isis::application_exception(errorString.str()); 
	}

	out_ConstraintOrder.constraintDefinitionOrder.clear();
	out_ConstraintOrder.constraintFeatureOrder_map.clear();

	// Sort based on constraints with the largest numbers of features first.
	// If the number of features are equal, the sort order should be the order of
	// CADAssembly.xml.  This is extremely important because in the future a 
	// particular order may be dictated by CyPhy.
	
	std::multimap<int,int> withGuide_NumberFeatures_perSetConstraintDefinitionIndex_map;
	std::multimap<int,int> withOutGuide_NumberFeatures_perSetConstraintDefinitionIndex_map;
	std::multimap<int,int> all_NumberFeatures_perSetConstraintDefinitionIndex_map;
	
	// Use offset to make the map key negative.  When iterating forward though the map
	// the negative numbers will assure that the original order in the CADAssembly.xml would
	// be maintained for the constraints with a equal number of features.  In other words,
	// if there are two constraints each with two features, the constraint that appears first in
	// CADAssembly.xml would appear first.
	const int offset = -1000;
	
	for ( int i = 0; i < in_perSetConstraintDefinitions.size(); ++i )
	{
		int numberFeatures = in_perSetConstraintDefinitions[i].get_NumberOfConstraintFeatures();
		all_NumberFeatures_perSetConstraintDefinitionIndex_map.insert(std::pair<int,int>(numberFeatures + offset, i));
		if ( in_perSetConstraintDefinitions[i].get_HasAGuideConstraint() )
			withGuide_NumberFeatures_perSetConstraintDefinitionIndex_map.insert(std::pair<int,int>(numberFeatures + offset, i));
		else
			withOutGuide_NumberFeatures_perSetConstraintDefinitionIndex_map.insert(std::pair<int,int>(numberFeatures + offset, i));
	}

	std::multimap<int,int>::const_iterator fit;
	if ( in_ConstraintSetsWithGuidesFirst )
	{	
		for (fit =  withGuide_NumberFeatures_perSetConstraintDefinitionIndex_map.begin(); 
			 fit != withGuide_NumberFeatures_perSetConstraintDefinitionIndex_map.end(); ++fit)
				out_ConstraintOrder.constraintDefinitionOrder.push_back(fit->second);

		for (fit =  withOutGuide_NumberFeatures_perSetConstraintDefinitionIndex_map.begin(); 
			 fit != withOutGuide_NumberFeatures_perSetConstraintDefinitionIndex_map.end(); ++fit)
				out_ConstraintOrder.constraintDefinitionOrder.push_back(fit->second);
	}
	else
	{
		for (fit =  all_NumberFeatures_perSetConstraintDefinitionIndex_map.begin(); 
			 fit != all_NumberFeatures_perSetConstraintDefinitionIndex_map.end(); ++fit)
				out_ConstraintOrder.constraintDefinitionOrder.push_back(fit->second);
	}	

	for each ( int i in out_ConstraintOrder.constraintDefinitionOrder )
	{
		const PerSetConstraintDefinition_2 &pSCD = in_perSetConstraintDefinitions[i];
		std::vector<int> constraintFeatureOrder_indexes;

		SortConstraintFeatures( pSCD, in_SortOrderLists[i], in_IncludeGuideFeactures, constraintFeatureOrder_indexes );

		out_ConstraintOrder.constraintFeatureOrder_map[i] = constraintFeatureOrder_indexes;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
// This function was in-work, not tested,  KEEP if in the future an incremental approach was needed
bool incrementColumnFirst(	int in_Max_i,  int in_Max_j,
							int &in_out_i, int &in_out_j )
{
	// Increment j first
	if ( in_out_j < in_Max_j )
	{
		++in_out_j;
		return true;
	}
	else
	{
		if ( in_out_i < in_Max_i)
		{
			++in_out_i;
			in_out_j = 0;
			return true;
		}
	}

	return false;
}

// This function was in-work, not tested,  KEEP if in the future an incremental approach was needed
void BuildIncrementalConstraintOrder( 	
	const std::vector<PerSetConstraintDefinition_2>	in_perSetConstraintDefinitions,
	const ConstraintOrder							&in_ConstraintOrder_Complete,
	ConstraintOrder									&in_out_ConstraintOrder_temp,
	bool											&out_Done_NoMore_Constraints_Features )
{
	int max_i = in_perSetConstraintDefinitions.size() - 1;
	int cur_i = in_out_ConstraintOrder_temp.constraintDefinitionOrder.size() - 1;
	int max_j = in_ConstraintOrder_Complete.getFeatureIDs(cur_i).size() - 1;
	int cur_j = in_out_ConstraintOrder_temp.getFeatureIDs(cur_i).size() - 1;

	bool temp_found_axis_dummy = true;

	int cur_i_temp = cur_i;
	int cur_j_temp = cur_j;

	if ( incrementColumnFirst( max_i, max_j, cur_i_temp, cur_j_temp ))
	{
		if ( temp_found_axis_dummy )
		{
			if ( cur_i_temp != cur_i )
			{
				// i incremeted
				in_out_ConstraintOrder_temp.constraintDefinitionOrder.push_back(in_ConstraintOrder_Complete.constraintDefinitionOrder[cur_i_temp]);
				std::vector<int> temp_vec;
				std::vector<int> complete_vec = in_ConstraintOrder_Complete.getFeatureIDs(cur_i_temp);
				if ( complete_vec.size() < 1 )
				{
					std::stringstream errorString;
					errorString << "Function - " << __FUNCTION__ << ", in_ConstraintOrder_Complete.getFeatureIDs returned 0 feature IDs." << std::endl <<
						           "A constraint must have at least one feature.";
					throw isis::application_exception("C04002",errorString.str());	
				}
				temp_vec.push_back(complete_vec[0]);
				cur_i = cur_i_temp;
				cur_j = 0;
				max_j = in_ConstraintOrder_Complete.getFeatureIDs(cur_i).size() - 1;

			}
			else
			{
				// j incremented
			}

		}
	}
	else
	{
		out_Done_NoMore_Constraints_Features = true;
	}


	in_out_ConstraintOrder_temp.constraintDefinitionOrder.clear();
	in_out_ConstraintOrder_temp.constraintFeatureOrder_map.clear();
	
	int constraintDefinition_index = 0;
	int constraintFeature_index = 0;

	int current_constraintDefinition_index = in_out_ConstraintOrder_temp.constraintDefinitionOrder.size() - 1;
	int currernt_constraintFeature_index =   in_out_ConstraintOrder_temp.constraintFeatureOrder.size() - 1;



	for each ( int i in in_ConstraintOrder_Complete.constraintDefinitionOrder )
	{
		const PerSetConstraintDefinition_2 &pSCD = in_perSetConstraintDefinitions[i];
		if (  in_out_ConstraintOrder_temp.constraintDefinitionOrder.size() ) 

		for each ( int j in in_ConstraintOrder_Complete.constraintFeatureOrder )
		{
			const ContraintFeatureDefinition_2 &cFD = pSCD.get_ContraintFeatureDefinition_const(j);

		}
	}

}
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CheckAxesAlignments_FlipOrientationIndicatorAsNecessary( 
										//const ProSolid									&in_assembly_model,
										const std::string								&in_AssemblyComponentID,
										//const std::string								&in_ComponentID,
										ProMdlType										in_model_type,
										const ConstraintOrder							&in_ConstraintOrder,
										std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map,
										std::vector<PerSetConstraintDefinition_2>		&in_out_PerSetConstraintDefinitions )
{
	
	isis::cad::CadFactoryAbstract_global *cadFactoryAbstract_global_ptr = isis::cad::CadFactoryAbstract_global::instance();
	isis::cad::CadFactoryAbstract::ptr	cAD_Factory_ptr = cadFactoryAbstract_global_ptr->getCadFactoryAbstract_ptr();
	isis::cad::IModelOperations&         modelOperations = cAD_Factory_ptr->getModelOperations();

	ProSolid* assembly_model_handle = reinterpret_cast<ProSolid*>(&in_CADComponentData_map[in_AssemblyComponentID].cADModel_hdl);	

	for each ( int i in in_ConstraintOrder.constraintDefinitionOrder )
	{
		 PerSetConstraintDefinition_2 &pSCD = in_out_PerSetConstraintDefinitions[i];

		for each ( const int &j in in_ConstraintOrder.getFeatureIDs(i) )
		{
			const ContraintFeatureDefinition_2 &cFD = pSCD.get_ContraintFeatureDefinition_const(j);
			if ( cFD.pro_datum_type == PRO_AXIS )
			{
				ProSelection axis_added_model_select;
				RetrieveProSelectionForGeometry(*assembly_model_handle,			// Used as the starting point of model_path_list
												cFD.added_model_path_list,	// Used to locate the model in the assembly containing the geometry
												cFD.added_model_component_instance_ID,				// Used for logging error messages
												cFD.added_model_name,				// Used for logging error messages
												cFD.added_model_type,				// Used for logging error messages
												cFD.pro_datum_type,			// Identifies the geometry type, e.g. Datum
												cFD.added_model_datum_name,	// Geometry Name, e.g. Datum Name
												axis_added_model_select);	// Returned pro_selection

				ProSelection axis_base_model_select;
				RetrieveProSelectionForGeometry(*assembly_model_handle,			// Used as the starting point of model_path_list
												cFD.base_model_path_list,	// Used to locate the model in the assembly containing the geometry
												cFD.base_model_component_instance_ID,	// Used for logging error messages
												cFD.base_model_name,				// Used for logging error messages
												cFD.base_model_type,				// Used for logging error messages
												cFD.pro_datum_type,			// Identifies the geometry type, e.g. Datum
												cFD.base_model_datum_name,	// Geometry Name, e.g. Datum Name
												axis_base_model_select);	// Returned pro_selection


				isis::cad::Joint  joint_added_model;
				isis::cad::Joint  joint_base_model;
					
			
				joint_added_model =	isis::cad::creo::extract_marker( axis_added_model_select );
				joint_base_model =	isis::cad::creo::extract_marker( axis_base_model_select );

				// The following coordinates are int the added model coordinate system
				double added_axis_origin_x = joint_added_model.location.get_e1();
				double added_axis_origin_y = joint_added_model.location.get_e2();
				double added_axis_origin_z = joint_added_model.location.get_e3();

				double added_axis_offset_x = joint_added_model.orientation.get_e1();
				double added_axis_offset_y = joint_added_model.orientation.get_e2();
				double added_axis_offset_z = joint_added_model.orientation.get_e3();

				// The following coordinates are int the added model coordinate system
				double base_axis_origin_x = joint_base_model.location.get_e1();
				double base_axis_origin_y = joint_base_model.location.get_e2();
				double base_axis_origin_z = joint_base_model.location.get_e3();

				double base_axis_offset_x = joint_base_model.orientation.get_e1();
				double base_axis_offset_y = joint_base_model.orientation.get_e2();
				double base_axis_offset_z = joint_base_model.orientation.get_e3();


				double   matrixBuffer_added[4][4];
				double   matrixBuffer_base[4][4];

				//RetrieveTranformationMatrix_Assembly_to_Child ( *assembly_model_handle,  
				//												cFD.added_model_path_list,
				//												PRO_B_FALSE,  // bottom up = False
				//												//PRO_B_TRUE,  // bottom up = True, Changed this 8/8/2013, V1.4.6
				//												matrixBuffer_added );


				modelOperations.retrieveTranformationMatrix_Assembly_to_Child( in_AssemblyComponentID,  
																cFD.added_model_path_list,
																in_CADComponentData_map, 
																PRO_B_FALSE,  // bottom up = False
																//PRO_B_TRUE,  // bottom up = True, Changed this 8/8/2013, V1.4.6
																matrixBuffer_added );


				//RetrieveTranformationMatrix_Assembly_to_Child ( *assembly_model_handle,  
				//												cFD.base_model_path_list,
				//												PRO_B_FALSE,  // bottom up = False
				//												//PRO_B_TRUE,  // bottom up = True, Changed this 8/8/2013, V1.4.6
				//												matrixBuffer_base );

				modelOperations.retrieveTranformationMatrix_Assembly_to_Child(in_AssemblyComponentID,  
																cFD.base_model_path_list,
																in_CADComponentData_map, 
																PRO_B_FALSE,  // bottom up = False
																//PRO_B_TRUE,  // bottom up = True, Changed this 8/8/2013, V1.4.6
																matrixBuffer_base );


				isis_CADCommon::TransformationMatrix  			transformationMatrix_added;
				isis_CADCommon::TransformationMatrix  			transformationMatrix_base;

				std::vector<double> offset_added;
				offset_added.push_back( matrixBuffer_added[3][0] );
				offset_added.push_back( matrixBuffer_added[3][1] );
				offset_added.push_back( matrixBuffer_added[3][2] );

				double rotation_added[3][3];

				for ( int ii = 0; ii < 3; ++ii )
					for ( int jj = 0; jj < 3; ++jj ) rotation_added[ii][jj] = matrixBuffer_added[ii][jj];
					
				transformationMatrix_added.setTransformationMatrix(	rotation_added, offset_added );

				std::vector<double> offset_base;
				offset_base.push_back( matrixBuffer_base[3][0] );
				offset_base.push_back( matrixBuffer_base[3][1] );
				offset_base.push_back( matrixBuffer_base[3][2] );

				double rotation_base[3][3];

				for ( int ii = 0; ii < 3; ++ii )
					for ( int jj = 0; jj < 3; ++jj ) rotation_base[ii][jj] = matrixBuffer_base[ii][jj];
					
				transformationMatrix_base.setTransformationMatrix(rotation_base, offset_base );


				std::vector<isis_CADCommon::Point_3D> line_added;

				line_added.push_back(isis_CADCommon::Point_3D(
					transformationMatrix_added.getTransformedCoordinates(
										isis_CADCommon::Point_3D(	added_axis_origin_x,
																	added_axis_origin_y,
																	added_axis_origin_z))));
				
				
				line_added.push_back(isis_CADCommon::Point_3D(	
						transformationMatrix_added.getTransformedCoordinates(
										isis_CADCommon::Point_3D(	added_axis_origin_x + added_axis_offset_x,
																	added_axis_origin_y + added_axis_offset_y,
																	added_axis_origin_z + added_axis_offset_z))));

				std::vector<isis_CADCommon::Point_3D> line_base;
			
				line_base.push_back(isis_CADCommon::Point_3D(
							transformationMatrix_base.getTransformedCoordinates(
								isis_CADCommon::Point_3D(	base_axis_origin_x,
															base_axis_origin_y,
															base_axis_origin_z))));


				line_base.push_back(isis_CADCommon::Point_3D(	
						transformationMatrix_base.getTransformedCoordinates(
										isis_CADCommon::Point_3D(	base_axis_origin_x + base_axis_offset_x,
																	base_axis_origin_y + base_axis_offset_y,
																	base_axis_origin_z + base_axis_offset_z))));
	

				// Detemine if the points defining  added axis points are on the line formed by the base axis

				if ( !isis_CADCommon::LinesCollinear(line_base, line_added ))
				{
					// two axes are not colinear.  There must be something ill formed about the other constraints.  The user must correct 
					// this either by changing the CyPhy model or by changine the CAD models 
					isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) << "Function - " << __FUNCTION__ << ", WARNING, Found constrained axes that were not collinear.";
					isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) << "    base_model_component_instance_ID:  " << cFD.base_model_component_instance_ID;
					isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) << "    base_model_name:                   " << cFD.base_model_name;
					isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) << "    base_model_datum_name:             " << isis::MultiFormatString(cFD.base_model_datum_name);
					isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) << "    Line start point and end point:";
					for each ( const isis_CADCommon::Point_3D &pt in line_base )  isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) << "        " << pt;
					isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) << "    added_model_component_instance_ID: " << cFD.added_model_component_instance_ID;
					isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) << "    added_model_name:                  " << cFD.added_model_name;
					isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) << "    added_model_datum_name:            " <<  isis::MultiFormatString(cFD.added_model_datum_name);
					isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) << "    Line start point and end point:";
					for each ( const isis_CADCommon::Point_3D &pt in line_added ) isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) << "        " << pt;
					isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) << "    If two axes are constrained together, they should be collinear.  Non-collinear axes are ";
					isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) << "    usually due to other constraints making it impossible to align the axes.  Open the CAD ";
					isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) << "    assembly and inspect the constraints for the model listed above to identify the problem. ";
					isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) << "    Also, open the CyPhy model or  CADAssembly.xml and check if this component has a guide.";
					isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) << "    Ill placed guides can cause axes to not align.  This condition should be corrected before "; 
					isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) << "    proceeding.";
					continue;  
				}

				// Axes form collinear lines
				// Now check if unit vectors point in the same direction

				std::vector<double> vector_base_model;
				vector_base_model.push_back(line_base[1].x  - line_base[0].x );
				vector_base_model.push_back(line_base[1].y  - line_base[0].y );
				vector_base_model.push_back(line_base[1].z  - line_base[0].z );

				std::vector<double> vector_added_model;
				vector_added_model.push_back(line_added[1].x  - line_added[0].x );
				vector_added_model.push_back(line_added[1].y  - line_added[0].y );
				vector_added_model.push_back(line_added[1].z  - line_added[0].z );

				if ( !isis_CADCommon::VectorsPointInTheSameDirection_3D( vector_base_model, vector_added_model))
				{
					// The unit vectors are pointing in the opposite directions.
					// Must set flip_orientation
					ContraintFeatureDefinition_2 &cFD_mutable = pSCD.get_ContraintFeatureDefinition(j);
					cFD_mutable.flip_orientation = true;
					isis_LOG(lg, isis_FILE, isis_INFO) << "**** Axis Flipped  ****"; 
					isis_LOG(lg, isis_FILE, isis_INFO) << "    base_model_component_instance_ID:  " << cFD.base_model_component_instance_ID;
					isis_LOG(lg, isis_FILE, isis_INFO) << "    base_model_name:                   " << cFD.base_model_name;
					isis_LOG(lg, isis_FILE, isis_INFO) << "    base_model_datum_name:             " << isis::MultiFormatString(cFD.base_model_datum_name);
					isis_LOG(lg, isis_FILE, isis_INFO) << "    Line start point and end point:";
					for each ( const isis_CADCommon::Point_3D &pt in line_base )  isis_LOG(lg, isis_FILE, isis_INFO) << "        " << pt;
					isis_LOG(lg, isis_FILE, isis_INFO) << "    added_model_component_instance_ID: " << cFD.added_model_component_instance_ID;
					isis_LOG(lg, isis_FILE, isis_INFO) << "    added_model_name:                  " << cFD.added_model_name;
					isis_LOG(lg, isis_FILE, isis_INFO) << "    Line start point and end point:";
					isis_LOG(lg, isis_FILE, isis_INFO) << "    added_model_datum_name:            " <<  isis::MultiFormatString(cFD.added_model_datum_name);
					for each ( const isis_CADCommon::Point_3D &pt in line_added ) isis_LOG(lg, isis_FILE, isis_INFO) << "        " << pt;
				}			
			}
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Apply_CADDatum_ModelConstraints_2( 
				//cad::CadFactoryAbstract						&in_Factory,
				//ProSolid									&in_assembly_model,   // This ProSolid is modified by this function.
				//																  // The modification is to add constraints to the assembly
				//																  // to position a part or sub-assembly.
				const std::string							&in_AssemblyComponentID,
				const std::string							&in_ComponentIDToBeConstrained,		
				const ConstraintDefinition					&in_ConstraintDefinition,
				std::map<string, isis::CADComponentData>	&in_CADComponentData_map )
													throw (isis::application_exception)
{

	ProSolid* assembly_model_handle = reinterpret_cast<ProSolid*>(&in_CADComponentData_map[in_AssemblyComponentID].cADModel_hdl);

	bool stop = false;

	// First need to determine the joint type (e.g. Fixed, Revolute, Prismatic)
	std::vector<std::string>			listOfComponentIDsInTheAssembly;
	listOfComponentIDsInTheAssembly.push_back(in_ComponentIDToBeConstrained);

	PopulateMap_with_Junctions_per_InputXMLConstraints(	//in_Factory,
													listOfComponentIDsInTheAssembly, 
													in_CADComponentData_map );
		
	std::vector<PerSetConstraintDefinition_2>     perSetConstraintDefinitions;

	Populate_PerSetConstraintDefinitions(	in_ComponentIDToBeConstrained,		
											in_ConstraintDefinition,
											in_CADComponentData_map,
											perSetConstraintDefinitions );


	////////////////////////////////////////////////////////////////////////////////////////////////
	// Application of the Constraints to Correct Axis Orientation
	// This applies to:
	//		get_JointType_withoutguide() ==  REVOLUTE_JOINT || CYLINDRICAL_JOINT || PRISMATIC_JOINT
	///////////////////////////////////////////////////////////////////////////////////////////////

	// Specify sort an order that would reveal if the axes are/are-not oriented properly.
	// Axes have a start and end point and when aligning axes the start and end points must 
	// be oriented in the same direction.  At least this is true for the joint types
	// REVOLUTE_JOINT || CYLINDRICAL_JOINT || PRISMATIC_JOINT
	// For other joint types (e.g. fixed) Creo seems to be tollerant of a datum planes that
	// defines the orientation and orthogonal axes that may/may-not be aligned.  

	std::vector<std::vector<ProType>>	sortOrderLists;
	std::vector<ProType> sortOrderList;
	sortOrderList.push_back(PRO_CSYS);
	sortOrderList.push_back(PRO_SURFACE);	// If a plane should be flipped, it is the user's responsibilty.
											// The user can change the CyPhy model or the CAD model to flip a plane.
	sortOrderList.push_back(PRO_POINT);
	sortOrderList.push_back(PRO_AXIS);		// Axis last so that we can determine if it should be flipped

	// Need a constraint order for each constraint set.  In this case, they are identical for all constraint sets.
	for (int i = 0; i < perSetConstraintDefinitions.size(); ++ i ) sortOrderLists.push_back(sortOrderList);

	// A complete constraint definition could be composed of multiple constraint sets.
	// For example, a link in a four-bar mechanism could be constrained on one end
	// with a constraint set and on the other end with a different constraint set.
	// We need to apply the constraint sets with guides.  This is necessary for 
	// PRISMATIC_JOINT joints because the guide would flip the part to the correct
	// orientation along the axis.  For REVOLUTE_JOINT and CYLINDRICAL_JOINT joints,
	// the guide would be for clocking purposes and would not be instrumental in 
	// determining the axis orientation and thus would have no adverse impact on 
	// determining the axis orientation.

	ConstraintOrder constraintOrder;
	ComputeConstraintSortOrder (	sortOrderLists,
									true, //	in_ConstraintSetsWithGuidesFirst, always set this to true even if in_IncludeGuideFeactures == false
										  //	This is because the constraints including guides are applied first, then removed, and then 
										  //	the constraints without guides are applied.  The order must be the same for both cases for
										  //	the flip state to be correct.
									true, //	in_IncludeGuideFeactures,
									perSetConstraintDefinitions,
									constraintOrder );

	ConstraintOrder constraintOrder_temp = constraintOrder;


	ProAsmcomp					assembledFeature_temp;			
	//assembledFeature_temp.type =	FeatureGeometryType_enum(in_CADComponentData_map[in_ComponentIDToBeConstrained].assembledFeature.type);
	//assembledFeature_temp.id   =	                         in_CADComponentData_map[in_ComponentIDToBeConstrained].assembledFeature.id;
	//assembledFeature_temp.owner =	                         in_CADComponentData_map[in_ComponentIDToBeConstrained].assembledFeature.owner; 
	assembledFeature_temp = getProAsmcomp(in_CADComponentData_map[in_ComponentIDToBeConstrained].assembledFeature);


	for each ( int i in constraintOrder.constraintDefinitionOrder )
	{
		// We need to apply all PerSetConstraintDefinitions individually.  This is necessary because for the case
		// where a component was constrained on two ends, the axes on both ends were not line up based on the initial
		// regeneration. To determine if an axis should be flipped, the axes must be collinear.

		// Note - For REVOLUTE_JOINT, CYLINDRICAL_JOINT, and PRISMATIC_JOINT joints, the first constraint (not using the guide)
		//        will always be an axis.  It is critical that the axis is oriented properly.  For other types of joints,
		//		  the axis orientation is not critical because the axis would be constrained last and the previous 
		//		  constraints would have already dictated the orientation.

		constraintOrder_temp.constraintDefinitionOrder.clear();
		constraintOrder_temp.constraintDefinitionOrder.push_back(i);

		const PerSetConstraintDefinition_2 &pSCD = perSetConstraintDefinitions[i];

		if ( pSCD.get_JointType_withoutguide() ==  REVOLUTE_JOINT ||
			 pSCD.get_JointType_withoutguide() ==  CYLINDRICAL_JOINT ||
			 pSCD.get_JointType_withoutguide() ==  PRISMATIC_JOINT )
			
		{
			std::string warnings;
			SetConstraints_2 (	
					*assembly_model_handle, 
					in_ComponentIDToBeConstrained, 
					in_CADComponentData_map, 
					constraintOrder_temp,
					perSetConstraintDefinitions,
					true,	// Treat as a user-defined joint  
						warnings );



			// isis::isis_ProAsmcompRegenerate( &in_CADComponentData_map[in_ComponentIDToBeConstrained].assembledFeature, PRO_B_TRUE);
			isis::isis_ProAsmcompRegenerate( &assembledFeature_temp, PRO_B_TRUE);
			
			// The following isis_ProSolidRegenerate is required.  The axes would not be collinear without this regeneration.
			try
			{
				isis::isis_ProSolidRegenerate (*assembly_model_handle, PRO_REGEN_UPDATE_ASSEMBLY_ONLY);
			}
			catch (...)
			{
				isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) << "WARNING - Assembly failed to regenerate in PRO_REGEN_UPDATE_ASSEMBLY_ONLY mode.";
				isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) << "   Error caused by Assembly Name: " << in_CADComponentData_map[in_AssemblyComponentID].name << "  ComponentID: " + in_AssemblyComponentID + ".";
			}


			CheckAxesAlignments_FlipOrientationIndicatorAsNecessary( 
											//*assembly_model_handle,
											in_AssemblyComponentID,
											//in_ComponentIDToBeConstrained,	
											ProMdlType_enum(in_CADComponentData_map[in_ComponentIDToBeConstrained].modelType),
											constraintOrder_temp,
											in_CADComponentData_map,
											perSetConstraintDefinitions );
	
			// Delete the constraints

			isis::isis_ProAsmcompConstrRemove (	&assembledFeature_temp, -1 );
			//isis::isis_ProAsmcompConstrRemove (	&in_CADComponentData_map[in_ComponentIDToBeConstrained].assembledFeature, -1 );
		}
	}

	ProMatrix    position;
	//////////////////////////////////////////////////////////////
	// Application of the Constraints to Correct Axis Orientation
	//////////////////////////////////////////////////////////////

	bool hasAGuideConstraint = false;
	for each (  const PerSetConstraintDefinition_2 &i in perSetConstraintDefinitions )
		if ( i.get_HasAGuideConstraint()) hasAGuideConstraint=true;

	if ( hasAGuideConstraint )
	{
		std::vector<std::vector<ProType>>	sortOrderLists;
		std::vector<ProType> sortOrderList;
		sortOrderList.push_back(PRO_SURFACE);	// A plane would be a guide.  Apply planes first.
		sortOrderList.push_back(PRO_CSYS);
		sortOrderList.push_back(PRO_POINT);
		sortOrderList.push_back(PRO_AXIS);		

		// Need a constraint order for each constraint set.  In this case, they are identical for all constraint sets.
		for (int i = 0; i < perSetConstraintDefinitions.size(); ++ i ) sortOrderLists.push_back(sortOrderList);

		ConstraintOrder constraintOrder;
		ComputeConstraintSortOrder (sortOrderLists,
									true,	//	in_ConstraintSetsWithGuidesFirst, always set this to true even if in_IncludeGuideFeactures == false
											//	This is because the constraints including guides are applied first, then removed, and then 
											//	the constraints without guides are applied.  The order must be the same for both cases for
											//	the flip state to be correct.
									true,	//	in_IncludeGuideFeactures,
									perSetConstraintDefinitions,
									constraintOrder );
		std::string warnings;
		SetConstraints_2 (	
				*assembly_model_handle, 
				in_ComponentIDToBeConstrained, 
				in_CADComponentData_map, 
				constraintOrder,
				perSetConstraintDefinitions,
				true,	// Treat as a user-defined joint  
				warnings );

		if ( warnings.size() > 0 )
		{
			if (isis::AssemblyOptions::GetInstance().FailLevel == isis::AssemblyOptions::FEATURE_REDEF)
			{
				isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << warnings;
				return true;
			} 
			else 
			{
						isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) << warnings;
			}
		}


		ProAsmcomp					assembledFeature_temp;			
		//assembledFeature_temp.type =	FeatureGeometryType_enum(in_CADComponentData_map[in_ComponentIDToBeConstrained].assembledFeature.type);
		//assembledFeature_temp.id   =	                         in_CADComponentData_map[in_ComponentIDToBeConstrained].assembledFeature.id;
		//assembledFeature_temp.owner =	                         in_CADComponentData_map[in_ComponentIDToBeConstrained].assembledFeature.owner; 
		assembledFeature_temp = getProAsmcomp(in_CADComponentData_map[in_ComponentIDToBeConstrained].assembledFeature);

		//isis::isis_ProAsmcompPositionGet (&in_CADComponentData_map[in_ComponentIDToBeConstrained].assembledFeature, position);
		isis::isis_ProAsmcompPositionGet (&assembledFeature_temp, position);
		// Delete the constraints
		//isis::isis_ProAsmcompConstrRemove (	&in_CADComponentData_map[in_ComponentIDToBeConstrained].assembledFeature, -1 );
		isis::isis_ProAsmcompConstrRemove (	&assembledFeature_temp, -1 );

	} // END if ( hasAGuideConstraint )

	/////////////////////////////////////////
	// Final Application of the Constraints
	/////////////////////////////////////////

	sortOrderLists.clear();

	for each (  const PerSetConstraintDefinition_2 &i in perSetConstraintDefinitions )
	{
		std::vector<ProType> sortOrderList;
		SortOrderBasedOnJointType( i.get_JointType_withoutguide(), sortOrderList );
		sortOrderLists.push_back(sortOrderList);
	}

	ComputeConstraintSortOrder (sortOrderLists,
								true,	//	in_ConstraintSetsWithGuidesFirst, always set this to true even if in_IncludeGuideFeactures == false
										//	This is because the constraints including guides are applied first, then removed, and then 
										//	the constraints without guides are applied.  The order must be the same for both cases for
										//	the flip state to be correct.
								false,	//	in_IncludeGuideFeactures,
								perSetConstraintDefinitions,
								constraintOrder );
	std::string warnings;
	SetConstraints_2 (	
			*assembly_model_handle, 
			in_ComponentIDToBeConstrained, 
			in_CADComponentData_map, 
			constraintOrder,
			perSetConstraintDefinitions,
			false,	// Treat as a user-defined joint  
			warnings );

	
	if ( warnings.size() > 0 )
	{
		if (isis::AssemblyOptions::GetInstance().FailLevel == isis::AssemblyOptions::FEATURE_REDEF)
		{
			isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << warnings;
			return true;
		} 
		else 
		{
					isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) << warnings;
		}
	}

	/////////////////////////////
	// Apply the Initial Position
	/////////////////////////////
	if (hasAGuideConstraint )
	{
		ProAsmcomppath	comp_path;
		ProIdTable	c_id_table;
		int			c_id_table_size;
					
		Populate_c_id_table(in_CADComponentData_map[in_ComponentIDToBeConstrained].componentPaths, c_id_table, c_id_table_size );
				
		isis::isis_ProAsmcomppathInit (static_cast<ProSolid>(in_CADComponentData_map[in_ComponentIDToBeConstrained].cADModel_hdl),	//ProSolid   p_solid_handle
							c_id_table,			// ProIdTable 
							c_id_table_size,	// table_size 
							&comp_path);		// ProAsmcomppath *p_handle

		ProAsmcomp					assembledFeature_temp;			
		//assembledFeature_temp.type =	FeatureGeometryType_enum(in_CADComponentData_map[in_ComponentIDToBeConstrained].assembledFeature.type);
		//assembledFeature_temp.id   =	                         in_CADComponentData_map[in_ComponentIDToBeConstrained].assembledFeature.id;
		//assembledFeature_temp.owner =	                         in_CADComponentData_map[in_ComponentIDToBeConstrained].assembledFeature.owner; 
		assembledFeature_temp = getProAsmcomp(in_CADComponentData_map[in_ComponentIDToBeConstrained].assembledFeature);

		//isis::isis_ProAsmcompPositionSet (&comp_path, &in_CADComponentData_map[in_ComponentIDToBeConstrained].assembledFeature,  position);
		isis::isis_ProAsmcompPositionSet (&comp_path, &assembledFeature_temp,  position);

	}

	return stop;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
Description:
-----------
This function constrains in_assembly_model by applying the constraints defined in in_ConstraintDefinition. The definitions 
of the terms and the algorithm follow: 
	
Definitions:
-----------
Round – This function using the term round to indicate the sequence for applying constraints.  For example, for a fixed 
		constraint with no guides, there would only be one round for applying constraints.  For a constraint with a guide, 
		there would be two rounds per the following sequence:
			Round 1: apply the constraints with the guide, regenerate, remove the constraints
			Round 2: Apply the constraints except for the guide constraint
	 
Guide – an extra constraint that typically fixes all six degrees of freedom.  For example, a revolute joint would typically be 
		constrained to an axis and a plane.  A guide would be an extra constraint that positions the initial, 
		position (i.e. angular position).  

Implied Guide – There are cases where the constraints do not have a guide, but where a guide is implied.  For example, with a 
				revolute joint defined as a revolute joint in Creo, the datum plane side A/B is not honored.  Therefore, the constraints 
				have to be applied first as a non-kinematic joint (user defined joint instead of a revolute joint) and then in the 
				second round as a revolute joint.  

Algorithm:
----------
if constraints do not have a guide
	if fixed joint
		Round 1:  Apply constraints 
		Round 2:  Not needed
	if kinematic joint ( for now only addressing revolute joints)
		Round 1:  Apply constraints as a user-defined set (i.e. not kinematic joint), regenerate, remove constraints.  This
				  is necessary because the Side_A/Side_B is not honored by a purely kinematic constraint. It is honored
				  by a user-defined constraint.  Thus, first apply the constraints as user defined, get the initial position, 
				  then reapply as a kinematic joint.
		Round 2:  Apply constraints as kinematic joint.
	
else // has a guide
	Round 1: Apply the constraints with the guide as a user-defined constraint set, persist transformation matrix, remove constraints
	Round 2: Apply the constraints without the guide and as the appropriate kinematic joint.

Notes:  
	1)	Model with Two Joints - There are cases where a model is constrained via two different joints. For example, 
		a link in a four-bar mechanism could be constrained on one end to one bar and on the other end to another bar. 
		Additionally the bar could have a guide on one end.  If the bar has a guide, the constraints on the guide end 
		of the bar are applied in round 1 and in round 2 the constraints for both ends are applied.

	2)	For non-kinematic joints (i.e. user defined constraint  the flag PRO_E_COMPONENT_CONSTR_ATTR is set as follows
		a)	The 1st axis in the constraints (this is all constraints for the model), PRO_E_COMPONENT_CONSTR_ATTR = 2 // force
		b)	For all other axis constraints = 3 (force + ignore)

3)	When applying constraints one of the datums will be associated with the"added" model and the other is "base".
	The constraint is recorded on one component and refers to a datum in a non-child component. The "added" datum must be 
	contained in the recording component. The "base" datum must not be contained in the recording component.

Pre-Conditions:
--------------
in_ConstraintDefinition.constraints must contain either zero guides or one guide.  More than one guide is erroneous.

Post-Conditions:
---------------
If no exceptions, then in_assembly_model would be constrained in the assembly.  
There are cases where no exceptions occur but the assembly will not properly regenerate.

Exceptions could occur if the datums used in the constraints do not exist in the particular Creo model, or if the 
constraints are irreconcilable.
*/

bool Apply_CADDatum_ModelConstraints( 
				//cad::CadFactoryAbstract						&in_Factory,
				ProSolid									&in_assembly_model,
				const std::string							&in_ComponentID,		
				ConstraintDefinition						&in_ConstraintDefinition,
				std::map<string, isis::CADComponentData>	&in_CADComponentData_map,
				bool										in_IgnoreGuides ) throw (isis::application_exception)
{
			// First need to determine the joint type (e.g. Fixed, Revolute, Prismatic)
			std::vector<std::string>			listOfComponentIDsInTheAssembly;
			listOfComponentIDsInTheAssembly.push_back(in_ComponentID);
			PopulateMap_with_Junctions_per_InputXMLConstraints(//	in_Factory,
													listOfComponentIDsInTheAssembly, 
													in_CADComponentData_map );
			

			// Needed for the commented out code below
			//ProAsmcomp		added_model_assembled_feature;	
			//const isis::CADComponentData &ComponentAssembledInfo_temp = in_CADComponentData_map[in_ComponentID];
			////added_model_assembled_feature = ComponentAssembledInfo_temp.assembledFeature;
			//added_model_assembled_feature.type =	FeatureGeometryType_enum(ComponentAssembledInfo_temp.assembledFeature.type);
			//added_model_assembled_feature.id   =	                         ComponentAssembledInfo_temp.assembledFeature.id;
			//added_model_assembled_feature.owner =	                         ComponentAssembledInfo_temp.assembledFeature.owner; 	


			ProAsmcompconstraint* constraints;
			isis::isis_ProArrayAlloc (0, sizeof (ProAsmcompconstraint), 1, (ProArray*)&constraints);

			//for ( AssemblyType::CADComponent_type::Constraint_const_iterator j(in_CADComponent_itr->Constraint().begin());
			//      j != in_CADComponent_itr->Constraint().end();
			//	  ++j )
			
			isis_LOG(lg, isis_FILE, isis_INFO) << "";

			
			//int numberOfConstraints = 0;
			//for each (ConstraintData j_temp in in_ConstraintDefinition.constraints ) 
			//	numberOfConstraints += j_temp.constraintPairs.size();

			std::list<int> removeConstraintIndexes; 

			//cad::IAssembler& assembler = in_Factory.get_assembler();

			std::vector<PerSetConstraintDefinition> perSetConstraintDefinitions;

			int indexOfAddedConstraint = 0;
			int setID = 0;

			for ( std::vector<ConstraintData>::iterator j(in_ConstraintDefinition.constraints.begin());
				  j != in_ConstraintDefinition.constraints.end();
				  ++j )
			{		
				++setID;
				PerSetConstraintDefinition perSetConDef;

				perSetConDef.setID = setID;
				perSetConDef.jointType_withoutguide =	j->computedJointData.jointType_withoutguide;
				perSetConDef.hasAGuideConstraint    =	j->hasAGuideConstraint();
				perSetConDef.jointType_withguide    =	j->computedJointData.jointType_withguide;

				//ProAsmcomp	 base_model_assembled_feature;	
				list<int>  base_model_path_list;
				list<int>  added_model_path_list;
				
	   			for (	std::vector<ConstraintPair>::const_iterator k(j->constraintPairs.begin());
						k != j->constraintPairs.end();
						++k )
				{
					ProAsmcompConstrType constraint_type = ProAsmcompConstrType_enum(k->featureAlignmentType);

					ProType	 pro_datum_type =  FeatureGeometryType_enum(k->featureGeometryType);	
					//ProType	 pro_datum_type =  k->featureGeometryType;	

					//if ( constraint_type == PRO_ASM_ALIGN &&  pro_datum_type== PRO_AXIS && numberOfConstraints > 2 ) constraint_type = PRO_ASM_LINE_DIST;
					//if ( constraint_type == PRO_ASM_ALIGN &&  pro_datum_type== PRO_AXIS && numberOfConstraints > 2  ) constraint_type = PRO_ASM_LINE_PARL;
					//if ( constraint_type == PRO_ASM_ALIGN &&  pro_datum_type== PRO_AXIS && numberOfConstraints > 2  ) constraint_type = PRO_ASM_AUTO;
					

					//ProName		 base_model_datum_name;	 // ASM_RIGHT, A_1..
					MultiFormatString base_model_datum_name(PRO_NAME_SIZE - 1);  // ASM_RIGHT, A_1..
					std::string		  base_model_component_instance_ID;
					ProDatumside base_model_datum_side;	 // enum PRO_DATUM_SIDE_YELLOW (SIDE_A), PRO_DATUM_SIDE_RED (SIDE_B), PRO_DATUM_SIDE_NONE
						

					//ProName		 added_model_datum_name; // RIGHT, A23 ..
					MultiFormatString		 added_model_datum_name(PRO_NAME_SIZE - 1);  // ASM_RIGHT, A_1..
					std::string				added_model_component_instance_ID;
					ProDatumside added_model_datum_side; // enum PRO_DATUM_SIDE_YELLOW (SIDE_A), PRO_DATUM_SIDE_RED (SIDE_B), PRO_DATUM_SIDE_NONE	

					bool added_model_defined = false;
					bool base_model_defined = false;
					string added_model_constraint_feature_component_ID;
					string base_model_constraint_feature_component_ID;
					isis_LOG(lg, isis_FILE, isis_INFO) << "";

					//for ( AssemblyType::CADComponent_type::Constraint_type::Pair_type::ConstraintFeature_const_iterator l(k->ConstraintFeature().begin());
					//	  l != k->ConstraintFeature().end();
					//	  ++l )


					std::string constraintFeature_From_To = "      ";
					for (	std::vector<ConstraintFeature>::const_iterator l(k->constraintFeatures.begin());
							l != k->constraintFeatures.end();
							++l )
					{												
						// See Note 1 (i.e. Reference [1]) at the top of this file for an explanation of the following if statement.
						
						if ( !added_model_defined  && 
							 ( l->componentInstanceID == in_ComponentID || ComponentIDChildOf( l->componentInstanceID, in_ComponentID, in_CADComponentData_map ) )
							 ) 
						{   // treat as added model constraint							
							added_model_path_list = in_CADComponentData_map[l->componentInstanceID].componentPaths;
							// Note:l->featureName is enforced by XMLToProEStructures.cpp(SetConstraintAttributes)
							//	    to be no more than 31 characters.		
							//ProStringToWstring(added_model_datum_name, (char *)(const char *)l->featureName );
							added_model_component_instance_ID = l->componentInstanceID;
							added_model_datum_name = l->featureName;
							added_model_datum_side = ProDatumside_enum(l->featureOrientationType);
							added_model_defined = true;
							added_model_constraint_feature_component_ID = l->componentInstanceID;
							//isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << in_CADComponentData_map[l->componentInstanceID].name << "::" << l->featureName;
							constraintFeature_From_To += (std::string)in_CADComponentData_map[l->componentInstanceID].name + "::" + (std::string)l->featureName;
							if (!base_model_defined)
							{ 
								//isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << " --> ";
								constraintFeature_From_To += " --> ";
							}
						}
						else
						{	// constraint features on a component in the assembly.
							base_model_path_list = in_CADComponentData_map[l->componentInstanceID].componentPaths;
							// Note:l->featureName is enforced by XMLToProEStructures.cpp(SetConstraintAttributes)
							//	    to be no more than 31 characters.		
							// ProStringToWstring(base_model_datum_name, (char *) (const char*) l->featureName );
							base_model_component_instance_ID = l->componentInstanceID;
							base_model_datum_name = l->featureName;
							base_model_datum_side = ProDatumside_enum(l->featureOrientationType);
							base_model_defined = true;
							base_model_constraint_feature_component_ID = l->componentInstanceID;
							//isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << in_CADComponentData_map[l->componentInstanceID].name << "::" << l->featureName;
							constraintFeature_From_To += (std::string)in_CADComponentData_map[l->componentInstanceID].name + "::" + (std::string)l->featureName;
							if (!added_model_defined) 
							{ 
								//isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << " -zz-> ";
								constraintFeature_From_To += " --> ";
							}
						}		
					
					}
					isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << constraintFeature_From_To;

					if ( !base_model_defined || !added_model_defined)
					{

						string err_str = "Erroneous constraint pair.  Assembled Component Instance ID: " + in_ComponentID + 
										 " Constraint Feature Component IDs: " + added_model_constraint_feature_component_ID +
										 ", " + base_model_constraint_feature_component_ID;
						throw isis::application_exception(err_str.c_str());  
					}
				
					double offset_between_datums = 0;
					if ( k->constraintOffsetPresent )
					{	
						offset_between_datums = k->constraintOffset.value;
						isis_LOG(lg, isis_FILE, isis_INFO) << "      ConstraintOffset Value:               " <<  k->constraintOffset.value;
						isis_LOG(lg, isis_FILE, isis_INFO) << "      ConstraintOffset OffsetAlignmentType: " <<  ProAsmcompConstrType_string( k->constraintOffset.offsetAlignmentType );
						if ( k->constraintOffset.unitsPresent )
						{
							isis_LOG(lg, isis_FILE, isis_INFO) << "      ConstraintOffset Units Type:          " << CADUnitsDistance_string( k->constraintOffset.units );
						}
					}				

			
			//		PopulateOneConstraintInConstraintStructure( 
			//			in_ComponentID,
			//			in_CADComponentData_map[in_ComponentID].name,
			//			in_CADComponentData_map[in_ComponentID].modelType,
			//			in_assembly_model,			// ProSolid, typedef struct sld_part* ProSolid;
			//			base_model_path_list,       // Assembly path to the base part
			//			added_model_path_list,      // Assembly path to the added part
			//			pro_datum_type,				// ProType, enum PRO_SURFACE, PRO_AXIS
			//			base_model_datum_name,		// ProName, ASM_RIGHT, A_1..
			//			base_model_datum_side,		// ProDatumside, enum PRO_DATUM_SIDE_YELLOW (SIDE_A), PRO_DATUM_SIDE_RED (SIDE_B), PRO_DATUM_SIDE_NONE
			//			added_model_datum_name,		// ProName, RIGHT, A23 ..
			//			added_model_datum_side,		// ProDatumside, enum PRO_DATUM_SIDE_YELLOW (SIDE_A), PRO_DATUM_SIDE_RED (SIDE_B), PRO_DATUM_SIDE_NONE
			//			constraint_type, // ProAsmcompConstrType, enum PRO_ASM_ALIGN, PRO_ASM_ALIGN_OFF...
			//			offset_between_datums,		// double, This is only used if in_constraint_type == PRO_ASM_ALIGN_OFF or PRO_ASM_MATE_OFF
			//			constraints,				// ProAsmcompconstraint*	out_constraints
			//			indexOfAddedConstraint); 
			
						ContraintFeatureDefinition cFDef;

						//MultiFormatString		base_model_name;
						//ProType				base_model_type;
						//MultiFormatString		added_model_name;
						//ProType					added_model_type;

						cFDef.p_base_model =						(ProMdl)in_CADComponentData_map[base_model_component_instance_ID].cADModel_hdl;
						cFDef.base_model_name =						in_CADComponentData_map[base_model_component_instance_ID].name;
						cFDef.base_model_type =						ProMdlType_enum(in_CADComponentData_map[base_model_component_instance_ID].modelType);
						cFDef.base_model_component_instance_ID =	base_model_component_instance_ID;

						cFDef.added_model_name =					in_CADComponentData_map[added_model_component_instance_ID].name;
						cFDef.added_model_type =					ProMdlType_enum(in_CADComponentData_map[added_model_component_instance_ID].modelType);
						cFDef.added_model_component_instance_ID =	added_model_component_instance_ID;

						cFDef.p_added_model =						(ProMdl)in_CADComponentData_map[added_model_component_instance_ID].cADModel_hdl;
						cFDef.base_model_path_list		= base_model_path_list;			// list of Creo feature IDs leading to the part/assembly
						cFDef.added_model_path_list		= added_model_path_list;		// list of Creo feature IDs  leading to the part/assembly
						cFDef.pro_datum_type			= pro_datum_type;				// enum PRO_SURFACE, PRO_AXIS
						wcscpy_s(cFDef.base_model_datum_name,base_model_datum_name);		// ASM_RIGHT, A_1..
						cFDef.base_model_datum_side		= base_model_datum_side;		// enum PRO_DATUM_SIDE_YELLOW (SIDE_A), PRO_DATUM_SIDE_RED (SIDE_B), PRO_DATUM_SIDE_NONE
						wcscpy_s(cFDef.added_model_datum_name, added_model_datum_name);	// RIGHT, A23 ..
						cFDef.added_model_datum_side	= added_model_datum_side;		// enum PRO_DATUM_SIDE_YELLOW (SIDE_A), PRO_DATUM_SIDE_RED (SIDE_B), PRO_DATUM_SIDE_NONE
						cFDef.constraint_type			= constraint_type;				// enum PRO_ASM_ALIGN, PRO_ASM_ALIGN_OFF...
						cFDef.offset_between_datums = offset_between_datums;	 
						cFDef.setID = setID;

						cFDef.treatConstraintAsAGuide =  k->treatConstraintAsAGuide;

						perSetConDef.contraintFeatureDefinitions.push_back(cFDef);

					// We want to push indexOfAddedConstraint to the front of the list so that when the items are deleted, the highest index is deleted first. 
					if ( k->treatConstraintAsAGuide == CAD_TREAT_CONSTRAINT_AS_A_GUIDE_TRUE ) removeConstraintIndexes.push_front(indexOfAddedConstraint);				

					++indexOfAddedConstraint;				

				}  // END for (std::vector<ConstraintPair>::const_iterator k(j->constraintPairs.begin());
				perSetConstraintDefinitions.push_back(perSetConDef);


			} // END for ( std::vector<ConstraintData>::iterator j(in_ConstraintDefinition.constraints.begin());
	
		/*	string msg = "Adding comp: " + in_ComponentID;
			ProMessageDisplay(L"msg_user.txt", "USER %0s", msg.c_str());
			isis_LOG(lg, isis_FILE, isis_ERROR) << msg;*/


			ProAsmcomp					assembledFeature_temp;			
			//assembledFeature_temp.type =	FeatureGeometryType_enum(in_CADComponentData_map[in_ComponentID].assembledFeature.type);
			//assembledFeature_temp.id   =	                         in_CADComponentData_map[in_ComponentID].assembledFeature.id;
			//assembledFeature_temp.owner =	                         in_CADComponentData_map[in_ComponentID].assembledFeature.owner; 		
			//assembledFeature_temp = getProAsmcomp(in_CADComponentData_map[in_ComponentID].assembledFeature);

			// 1st round of setup: install guides and the 1st kinematic joint as non-kinematic ones to establish the initial position
			if (!in_IgnoreGuides)
			{
				std::string warnings;
				SetConstraints(in_assembly_model, in_ComponentID, in_CADComponentData_map, perSetConstraintDefinitions, true, warnings );
				if ( warnings.size() > 0 )
				{
					if (isis::AssemblyOptions::GetInstance().FailLevel == isis::AssemblyOptions::FEATURE_REDEF)
					{
						isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << warnings;
						return true;
					} else {
						isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) << warnings;
					}
				}

				// Regenerate placement of the assembled model
				int i = 5;
				ProSolidRegenerationStatus s = PRO_SOLID_FAILED_REGENERATION;




				while (i > 0 && s == PRO_SOLID_FAILED_REGENERATION)
				{
					//isis::isis_ProAsmcompRegenerate( &in_CADComponentData_map[in_ComponentID].assembledFeature, PRO_B_TRUE);
					isis::isis_ProAsmcompRegenerate( &assembledFeature_temp, PRO_B_TRUE);

					// Verify if regeneration succeeded
					ProSolidRegenerationstatusGet((ProSolid)(in_assembly_model), &s);
					i--;
				}

				// Get the position matrix
				//ProMatrix    position;
				//isis::isis_ProAsmcompPositionGet (&in_CADComponentData_map[in_ComponentID].assembledFeature, position);

				if (isis::AssemblyOptions::GetInstance().StopAssemblyAt == isis::AssemblyOptions::AFTER_GUIDE && 
					isis::AssemblyOptions::GetInstance().StopAssemblyComponentID == in_ComponentID)
				{
					return true;
				}

				if (s == PRO_SOLID_FAILED_REGENERATION)
				{
					string errMsg = "Assembly failed during regen after setting up constraints. Component: " + in_ComponentID;
					if (isis::AssemblyOptions::GetInstance().FailLevel == isis::AssemblyOptions::REGEN ||
						isis::AssemblyOptions::GetInstance().FailLevel == isis::AssemblyOptions::FEATURE_REDEF)
					{
						isis_LOG(lg, isis_FILE, isis_ERROR) << errMsg;
						return true;
					} else {
						isis_LOG(lg, isis_FILE, isis_WARN) << errMsg;
					}
				}



				// Remove all of the constraints
				//isis::isis_ProAsmcompConstrRemove (	&in_CADComponentData_map[in_ComponentID].assembledFeature, -1 );
				isis::isis_ProAsmcompConstrRemove (	&assembledFeature_temp, -1 );

				// Set the transformation matrix
				/*
				ProAsmcomppath	comp_path;
				ProIdTable	c_id_table;
				int			c_id_table_size;
					
				Populate_c_id_table(in_CADComponentData_map[in_ComponentID].componentPaths, c_id_table, c_id_table_size );
				
				isis::isis_ProAsmcomppathInit (in_CADComponentData_map[in_ComponentID].modelHandle,	//ProSolid   p_solid_handle
									c_id_table,			// ProIdTable 
									c_id_table_size,	// table_size 
									&comp_path);		// ProAsmcomppath *p_handle
				isis::isis_ProAsmcompPositionSet (&comp_path, &in_CADComponentData_map[in_ComponentID].assembledFeature,  position);
				*/

			}

			// 2nd round: set up the joints as they are defined

			std::string warnings;
			SetConstraints(in_assembly_model, in_ComponentID, in_CADComponentData_map, perSetConstraintDefinitions, false, warnings );
			if ( warnings.size() > 0 )
			{
				if (isis::AssemblyOptions::GetInstance().FailLevel == isis::AssemblyOptions::FEATURE_REDEF)
				{
					isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << warnings;
					return true;
				} else {
					isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) << warnings;
				}
			}


			//ProMessageDisplay(L"msg_user.txt", "USER %0s", "regen3");
			//isis_LOG(lg, isis_FILE, isis_ERROR) << "regen3";
			// Regenerate placement of the assembled model

			int i = 5;
			ProSolidRegenerationStatus s = PRO_SOLID_FAILED_REGENERATION;
			while (i > 0 && s == PRO_SOLID_FAILED_REGENERATION)
			{
				if (isis::AssemblyOptions::GetInstance().FullRegen)
				{
					ProSolidRegenerate(in_assembly_model, PRO_REGEN_NO_RESOLVE_MODE);
				} else {
					//isis::isis_ProAsmcompRegenerate( &in_CADComponentData_map[in_ComponentID].assembledFeature, PRO_B_TRUE);
					isis::isis_ProAsmcompRegenerate( &assembledFeature_temp, PRO_B_TRUE);
				}

				// Verify if regeneration succeeded
				ProSolidRegenerationstatusGet((ProSolid)(in_assembly_model), &s);
				i--;
			}

			if (isis::AssemblyOptions::GetInstance().StopAssemblyAt == isis::AssemblyOptions::AFTER_CONSTRAINTS && 
				isis::AssemblyOptions::GetInstance().StopAssemblyComponentID == in_ComponentID)
			{
				return true;
			}

			if (s == PRO_SOLID_FAILED_REGENERATION)
			{
				// Do a proper error reporting here. Now it'll just interrupt the assembly generation
				string errMsg = "Assembly failed during regen after setting up constraints. Component: " + in_ComponentID;
				if (isis::AssemblyOptions::GetInstance().FailLevel == isis::AssemblyOptions::REGEN ||
					isis::AssemblyOptions::GetInstance().FailLevel == isis::AssemblyOptions::FEATURE_REDEF)
				{
					isis_LOG(lg, isis_FILE, isis_ERROR) << errMsg;
					return true;
				} else {
					isis_LOG(lg, isis_FILE, isis_WARN) << errMsg;
				}
			}

			return false;

			// try the following see if it solves the kinematic assem probles
			//isis::isis_ProSolidRegenerate (in_assembly_model, PRO_REGEN_NO_RESOLVE_MODE);

			// Set the assembly component constraints
			/*
			isis_LOG(lg, isis_FILE, isis_INFO) << "";
			isis_LOG(lg, isis_FILE, isis_INFO) << "********* Begin Set Constraint Set - ProAsmcompConstraintsSet ";
			isis_LOG(lg, isis_FILE, isis_INFO) << "added_model_assembled_feature";
			isis_LOG(lg, isis_FILE, isis_INFO) <<	 "   added_model_assembled_feature.id:     " << added_model_assembled_feature.id <<  isis_EOL <<
	     	 "   added_model_assembled_feature.type:   " << added_model_assembled_feature.type <<  isis_EOL <<
	     	 "   added_model_assembled_feature.owner:  " << added_model_assembled_feature.owner;
			isis_LOG(lg, isis_FILE, isis_INFO) << "Constraints address: " <<  constraints;
			isis_LOG(lg, isis_FILE, isis_INFO) << "********* End Set Constraint Set - ProAsmcompConstraintsSet " << isis_EOL;

			ProAsmcomppath assem_path;
			ProIdTable c_id_table;
			c_id_table [0] = -1;
			isis::isis_ProAsmcomppathInit(*in_assembly_model, c_id_table, 0, &assem_path);	

			isis::isis_ProAsmcompConstraintsSet(&assem_path, &added_model_assembled_feature, constraints);
			isis::isis_ProArrayFree ((ProArray*)&constraints);
			*/
			///////////////////////////////////////
			//   Begin Temporary Code		
			///////////////////////////////////////
			/*
			ProElement elemTree;

			isis::isis_ProFeatureElemtreeExtract (	&added_model_assembled_feature,
													&assem_path,
													PRO_FEAT_EXTRACT_NO_OPTS,
													&elemTree );

			std::string featureTreeFileName_narrow = in_CADComponentData_map[in_ComponentID].name;
			//featureTreeFileName_narrow += "_before_set_create.xml";
			featureTreeFileName_narrow += "_old_approach.xml";
			isis::MultiFormatString  featureTreeFileName_multi( featureTreeFileName_narrow);

			isis::isis_ProElemtreeWrite(elemTree, PRO_ELEMTREE_XML, (wchar_t*)(const wchar_t*)featureTreeFileName_multi);		
			*/
			///////////////////////////////////////
			//   End Temporary Code		
			///////////////////////////////////////

			//SetConstraint_AllowAssumptions (&assem_path, &added_model_assembled_feature, CAD_CONSTRAINT_ALLOW_ASSUMPTIONS_FALSE );

			///////////////////////////////////////
			//   Begin Temporary Code		
			///////////////////////////////////////
			//ProElement elemTree;
			/*
			isis::isis_ProFeatureElemtreeExtract (	&added_model_assembled_feature,
													&assem_path,
													PRO_FEAT_EXTRACT_NO_OPTS,
													&elemTree );

			std::string featureTreeFileName_narrow_2 = in_CADComponentData_map[in_ComponentID].name;
			featureTreeFileName_narrow_2 += "_after_set_create.xml";
			isis::MultiFormatString  featureTreeFileName_multi_2( featureTreeFileName_narrow_2);

			isis::isis_ProElemtreeWrite(elemTree, PRO_ELEMTREE_XML, (wchar_t*)(const wchar_t*)featureTreeFileName_multi_2);		
			*/
			///////////////////////////////////////
			//   End Temporary Code		
			///////////////////////////////////////
			


			//////////////////////////////////////////////////////
			// Delete the constraints that were just guides.
			//////////////////////////////////////////////////////
			/*
			ProMatrix    position;
			
			if ( removeConstraintIndexes.size() > 0 )
			{
				
				try 
				{
					isis::isis_ProAsmcompRegenerate( &ComponentAssembledInfo_temp.assembledFeature, PRO_B_TRUE);
				}
				catch (...)
				{
					isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "ERROR: Failed to regenerate after adding guide constraint(s) (i.e. initial position constraint(s)) to" 
													   << "\n   ComponentInstanceID: " << in_ComponentID 
													   << "\n   Model Name:          " << in_CADComponentData_map[in_ComponentID].name;
					throw;
				}
				
				isis::isis_ProAsmcompPositionGet (&ComponentAssembledInfo_temp.assembledFeature, position);

				//for (int ii = 0 ; ii < 3; ++ii )
				//{
				//	std::cout << std::endl;
				//	for (int jj = 0 ; jj < 3; ++jj ) std::cout << "  " << position[ii][jj];
				//}

			}
			*/
			/*
			for ( std::list<int>::const_iterator i_del(removeConstraintIndexes.begin());  i_del != removeConstraintIndexes.end(); ++ i_del)
			{
				//isis_ProAsmcompConstrRemove (	ProAsmcomp *p_feat_handle, int  index ) throw(isis::application_exception);		
				isis::isis_ProAsmcompConstrRemove (	&ComponentAssembledInfo_temp.assembledFeature, *i_del );
				isis_LOG(lg, isis_FILE, isis_INFO) << "\nRemoved guide constraint, ComponentInstanceID: " << ComponentAssembledInfo_temp.componentID << ".  Constraint index: " << *i_del;
			}
			

			if ( removeConstraintIndexes.size() > 0 )
			{
				ProAsmcomppath	comp_path;

				ProIdTable	c_id_table;
				int			c_id_table_size;
					
				Populate_c_id_table( ComponentAssembledInfo_temp.componentPaths, c_id_table, c_id_table_size );
				
				isis::isis_ProAsmcomppathInit (ComponentAssembledInfo_temp.modelHandle,	//ProSolid   p_solid_handle
									c_id_table,			// ProIdTable 
									c_id_table_size,	// table_size 
									&comp_path);		// ProAsmcomppath *p_handle

				isis::isis_ProAsmcompPositionSet (&comp_path, &ComponentAssembledInfo_temp.assembledFeature,  position);			
			}
			*/
			//for (int ii = 0 ; ii < 3; ++ii )
			// {
			//	std::cout << std::endl;
			//	for (int jj = 0 ; jj < 3; ++jj ) std::cout << "  " << position[ii][jj];
			//}


			 //Uncomment this line for testing purposes.  Regeneration after constraining each component
			 //unacceptably slows down this application.  
			 //std::cout << std::endl << "Regenerating....";
			 //isis::isis_ProSolidRegenerate (*in_assembly_model, PRO_REGEN_NO_RESOLVE_MODE);
				
}
///////////////////////////////////////////////////////////////////////////////////////////////////
/* This function is not used,  KEEP for example purposes
void Get_CADModelInterface_ModelAddresses( 
				ProSolid	 *in_assembly_model,
				AssemblyType::CADComponent_type::CADComponent_const_iterator in_CADComponent_itr,
				std::map<string, isis::CADComponentData> &in_CADComponentData_map )
										throw (isis::application_exception)
{

	// WARNING - This function has NOT been tested.

	
	// For CAD_MODEL_INTERFACE FeatureInterfaceType, there should only be only one <Constraint> <Pair>
	int ConstraintPairCount = 0;

	vector<list<int>>  model_path(2);

	for ( AssemblyType::CADComponent_type::Constraint_const_iterator j(in_CADComponent_itr->Constraint().begin());
		      j != in_CADComponent_itr->Constraint().end();
				  ++j )
	{
		++ConstraintPairCount;
		if ( ConstraintPairCount >  1 )
		{
			string err_str = "Erroneous constraint pairs (i.e. <CADComponent...> <Constraint> <Pair...>. For FeatureInterfaceType == CAD_MODEL_INTERFACE, there must be one and only one pair (i.e. <Pair...>, Component Instance ID: " + in_CADComponent_itr->ComponentID(); 
			throw isis::application_exception(err_str.c_str());  	
		}
				

	//if ( in_CADComponent_itr->Constraint().present() )
	//{
	//	for ( AssemblyType::CADComponent_type::Constraint_type::Pair_const_iterator k(in_CADComponent_itr->Constraint().get().Pair().begin());
	//				  k != in_CADComponent_itr->Constraint().get().Pair().end();
	//				  ++k )
		for ( AssemblyType::CADComponent_type::Constraint_type::Pair_const_iterator k(j->Pair().begin());
				  k != j->Pair().end();
				  ++k )
		{
			
			++ConstraintPairCount;
			if ( ConstraintPairCount >  1 )
			{
				string err_str = "Erroneous constraint pairs (i.e. <CADComponent...> <Constraint> <Pair...>. For FeatureInterfaceType == CAD_MODEL_INTERFACE, there must be one and only one pair (i.e. <Pair...>, Component Instance  ID: " + in_CADComponent_itr->ComponentID(); 
				throw isis::application_exception(err_str.c_str());  	
			}

			int ConstraintFeatureIndex = 0;
			for ( AssemblyType::CADComponent_type::Constraint_type::Pair_type::ConstraintFeature_const_iterator l(k->ConstraintFeature().begin());
					  l != k->ConstraintFeature().end();
					  ++l )
			{
				model_path[ConstraintFeatureIndex] =  in_CADComponentData_map[l->ComponentID()].componentPaths;
				++ConstraintFeatureIndex;
				if ( ConstraintFeatureIndex >  1 )
				{
					string err_str = "Erroneous ConstraintFeature(i.e. <CADComponent...> <Constraint> <Pair...> <ConstraintFeature...>. For FeatureInterfaceType == CAD_MODEL_INTERFACE, there must be two and only two ConstraintFeatures (i.e. <ConstraintFeature...>, Component Instance ID: " + in_CADComponent_itr->ComponentID(); 
					throw isis::application_exception(err_str.c_str());  	
				}

			}

		}
		
	 }  // end if

	std::cout <<  std::endl <<  "******** Path list for CAD_MODEL_INTERFACE*******************";
	std::cout <<  std::endl << "ComponentID: " << in_CADComponent_itr->ComponentID();
	std::cout <<  std::endl <<  " ConstraintFeature 0 ";

	for ( list<int>::const_iterator i = model_path[1].begin();
			  i != model_path[1].end(); ++i )
	{
		std::cout << std::endl << "  Feature ID: " << *i;
	}

	std::cout <<  std::endl <<  " ConstraintFeature 1 ";
	for ( list<int>::const_iterator i = model_path[1].begin();
			  i != model_path[1].end(); ++i )
	{
		std::cout << std::endl << "  Feature ID: " << *i;
	}

} // Get_CADModelInterface_ModelAddresses
*/

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// PRO_E_FEATURE_TREE
//     |
//     |--PRO_E_FEATURE_TYPE
//     |
//     |--PRO_E_COMP_PLACE_INTERFACE
//     |    |--PRO_E_COMP_PLACE_INTERFACE_TYPE
//     |    |--PRO_E_COMP_PLACE_INTERFACE_COMP
//     |    |--PRO_E_COMP_PLACE_INTERFACE_ASSEMS
//     |         |--PRO_E_COMP_PLACE_INTERFACE_ASSEM
//     |             |--PRO_E_COMP_PLACE_INTERFACE_ASSEM_REF

/*** KEEP this function as an example

void Apply_CADModelInterface_ModelConstraints( 
				ProSolid	 *in_assembly_model,
				AssemblyType::CADComponent_type::CADComponent_const_iterator in_CADComponent_itr,
				std::map<string, isis::CADComponentData> &in_CADComponentData_map )
										throw (isis::application_exception)
{
	//////////////////////////////////////////////////
	// Find the addresses of the CAD Model interfaces
	//////////////////////////////////////////////////

	Get_CADModelInterface_ModelAddresses(in_assembly_model, in_CADComponent_itr, in_CADComponentData_map);

	return;

	//////////////////////////////////////////////
	// Create the root of the element tree.
	//////////////////////////////////////////////
	ProElement elem_tree;
	isis::isis_ProElementAlloc (PRO_E_FEATURE_TREE, &elem_tree);

	///////////////////////////////////////////////////////////
	//  Allocate and set the feature type element. 
	///////////////////////////////////////////////////////////
	ProElement elem_ftype;
	isis::isis_ProElementAlloc (PRO_E_FEATURE_TYPE, &elem_ftype);
	isis::isis_ProElementIntegerSet(elem_ftype, PRO_FEAT_COMPONENT);
	isis::isis_ProElemtreeElementAdd(elem_tree, NULL, elem_ftype);

	////////////////////////////////////////////////////////////////////
	// Add a PRO_E_COMP_PLACE_INTERFACE element to the root of the tree.
	////////////////////////////////////////////////////////////////////
	ProElement elem_comp_place_interface;
	isis::isis_ProElementAlloc(PRO_E_COMP_PLACE_INTERFACE, &elem_comp_place_interface);
	isis::isis_ProElemtreeElementAdd (elem_tree, NULL, elem_comp_place_interface);

	////////////////////////////////////////////////////////////////////
	// Allocate and set the PRO_E_COMP_PLACE_INTERFACE_TYPE element.
	////////////////////////////////////////////////////////////////////
	ProElement elem_comp_place_interface_type;
	isis::isis_ProElementAlloc (PRO_E_COMP_PLACE_INTERFACE_TYPE, &elem_comp_place_interface_type);
	isis::isis_ProElementIntegerSet (elem_comp_place_interface_type, PRO_ASM_INTFC_TO_INTFC);
	ProElemtreeElementAdd (elem_comp_place_interface, NULL, elem_comp_place_interface_type);

	////////////////////////////////////////////////////////////////////
	// Allocate the PRO_E_COMP_PLACE_INTERFACE_COMP element.  This is the
	// interface on the assembled model.
	// User Guide, Section "Placement via Interface": PRO_E_COMP_PLACE_INTERFACE_COMP
	// Specifies the component model interface. This should contain the component
	// interface feature.
	////////////////////////////////////////////////////////////////////
	ProElement elem_comp_place_interface_comp;
	isis::isis_ProElementAlloc (PRO_VALUE_TYPE_SELECTION, &elem_comp_place_interface_comp);
	//isis::isis_ProElementReferencesSet (elem_comp_place_interface_comp, ??? reference geometry);
	ProElemtreeElementAdd (elem_comp_place_interface, NULL,  elem_comp_place_interface_comp);

}

***********/
////////////////////////////////////////////////////////////////////////////////////////////////
/*  KEEP for the example switch switch( FeatureInterfaceType_enum )
void ApplyModelConstraints( 
			ProSolid	 *in_assembly_model,
			AssemblyType::CADComponent_type::CADComponent_const_iterator in_Begin,
			AssemblyType::CADComponent_type::CADComponent_const_iterator in_End,
			std::map<string, isis::CADComponentData> &in_CADComponentData_map,
			const std::set<std::string> &in_ConstrainComponentsSet )
										throw (isis::application_exception)
{
		for ( AssemblyType::CADComponent_type::CADComponent_const_iterator i(in_Begin); i != in_End; ++i )
		{
			if ( in_ConstrainComponentsSet.find( i->ComponentID() ) != in_ConstrainComponentsSet.end() )
			{

				isis::e_FeatureInterfaceType FeatureInterfaceType_enum;

				FeatureInterfaceType_enum = GetFeatureInterfaceType_enum(i);

				switch( FeatureInterfaceType_enum )
				{
					case CAD_DATUM:
						Apply_CADDatum_ModelConstraints_old( in_assembly_model, i, in_CADComponentData_map);
						break;
					//case CAD_MODEL_INTERFACE:
					//  this is being worked
					//	Apply_CADModelInterface_ModelConstraints(in_assembly_model, i, in_CADComponentData_map);
					//	break;
					default:
						string err_str = "Erroneous constraint FeatureInterfaceType. FeatureInterfaceType must be of type CAD_DATUM or CAD_MODEL_INTERFACE. Component Instance ID: " + i->ComponentID(); 
						throw isis::application_exception(err_str.c_str());  	
				}
			}

		}  // End for ( AssemblyType::CADComponent_type::CADComponent_const_iterator i(++in_Begin); i != in_End; ++i )

} // End ApplyModelConstraints
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ApplyModelConstraints( 
			//cad::CadFactoryAbstract						&in_Factory,
			//ProSolid									*in_assembly_model,
			const std::string							&in_AssemblyComponentID,
			const std::list<std::string>				&in_ComponentIDsToBeConstrained, 
			bool										in_AllowUnconstrainedModels,
			std::map<string, isis::CADComponentData>	&in_CADComponentData_map,
														// Provide for the case where the first assembled part does not have
														// the datums front, top, and right defined. 
			//bool										in_IgnoreGuides,
			bool										in_FirstComponentToBePositionedAsIntiiallyPlaced_IfDatumsCannotBeFound )  
														
										throw (isis::application_exception)
{
	bool fail = false;
	
	try
	{
		int count = 0;
		for ( std::list<std::string>::const_iterator i(in_ComponentIDsToBeConstrained.begin()); 
			  i != in_ComponentIDsToBeConstrained.end(); ++i, ++count )
		{

			//std::cout << std::endl << "*** in_AllowUnconstrainedModels: " << in_AllowUnconstrainedModels;
			//std::cout << std::endl << "*** Number of Constraints:       " << in_CADComponentData_map[*i].constraintDef.constraints.size();

			if ( in_AllowUnconstrainedModels && in_CADComponentData_map[*i].constraintDef.constraints.size() == 0 )
			{
				isis_LOG(lg, isis_FILE, isis_INFO) << "      "  << 	in_CADComponentData_map[*i].name << "::Unconstrained";
				continue;
			}
			if ( in_CADComponentData_map[*i].constraintDef.constraints.size() == 0 )
			{
				std::stringstream errorString;
				errorString <<
					"Unconstrained Component, Component Instance ID: " <<  in_CADComponentData_map[*i].componentID << "  Component Name: " << in_CADComponentData_map[*i].name;
				throw isis::application_exception("C04002",errorString.str().c_str());			
			}
			
			try
			{

				//bool stop = Apply_CADDatum_ModelConstraints(	in_Factory, 
				//									*in_assembly_model,
				//									*i,
				//									in_CADComponentData_map[*i].constraintDef, 
				//									in_CADComponentData_map,
				//									in_IgnoreGuides);	
				bool stop = Apply_CADDatum_ModelConstraints_2(//	in_Factory, 
													//*in_assembly_model,
													in_AssemblyComponentID,
													*i,
													in_CADComponentData_map[*i].constraintDef, 
													in_CADComponentData_map );		
				if (stop)
				{
					fail = true;
					break;
				}
			}
			catch (...)
			{
				if ( count == 0 && in_FirstComponentToBePositionedAsIntiiallyPlaced_IfDatumsCannotBeFound)
				{
					isis_LOG(lg, isis_FILE, isis_INFO) << "      "  << 	in_CADComponentData_map[*i].name << "::Coordinate System --> Assembly Coordinate System";
				}
				else
				{
					throw;
				}
			}  // END try - Catch
			isis_LOG(lg, isis_FILE, isis_INFO) << "      "  << 	in_CADComponentData_map[*i].name << "::Coordinate System --> Assembly Coordinate System";
		}
	}
	catch (...)
	{
		//isis::isis_ProMdlSave( &in_assembly_model);
		throw;
	}
	return fail;
} // End ApplyModelConstraints_2

// This function sets the selections in the ContraintFeatureDefinition structure
// These are used many times and it's inefficient to 
void SetSelections(ContraintFeatureDefinition &fdef)
{

}


} // End namespace isis
