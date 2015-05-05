#include <ApplyModelConstraints.h>
#include <BuildAssembly.h>
#include <AssembleComponents.h>
#include <DiagnosticUtilities.h>
#include <AssembleUtils.h>
#include <ToolKitPassThroughFunctions.h> 
#include <log4cpp/Category.hh>
#include <log4cpp/OstreamAppender.hh>
#include "CommonDefinitions.h"
#include <boost/algorithm/string.hpp>
#define PRO_USE_VAR_ARGS
#include "ProMessage.h"
#include "AssemblyOptions.h"
#include "DatumRefResolver.h"

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
	double					offset_between_datums;	// This is only used if in_constraint_type == PRO_ASM_ALIGN_OFF or PRO_ASM_MATE_OFF
	int						setID;
	e_CADTreatConstraintAsAGuide treatConstraintAsAGuide;

	ContraintFeatureDefinition(): treatConstraintAsAGuide(CAD_TREAT_CONSTRAINT_AS_A_GUIDE_FALSE){};
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

	//log4cpp::Category& logcat_fileonly = log4cpp::Category::getInstance(LOGCAT_LOGFILEONLY);
	
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
// This function has memory leaks.  Should fix.
void Add_PRO_E_COMPONENT_CONSTRAINT( ProElement &out_constrs_elem, 
									const ContraintFeatureDefinition &in_ContraintDef, 
									ProSolid &in_TopAssemblyModel,
									e_CADJointType in_JointType, int axiscounter, int surfacecounter, bool guide, int setidx)
														throw (isis::application_exception)
{

		log4cpp::Category& logcat_fileonly = log4cpp::Category::getInstance(LOGCAT_LOGFILEONLY);

		std::stringstream str;
		logcat_fileonly.infoStream() << "Add_PRO_E_COMPONENT_CONSTRAINT >>>>>>>>>>>>>>>";
		//logcat_fileonly.infoStream() << "\n**** Axis Constraint Index: " <<  counter << "  ******";
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
			
			logcat_fileonly.infoStream() << str.str();
			logcat_fileonly.infoStream();
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
		isis::MultiFormatString  temp_string(in_ContraintDef.base_model_datum_name);

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

		temp_string = in_ContraintDef.added_model_datum_name;
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

// This function is certainly very inefficient
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

		log4cpp::Category& logcat_fileonly = log4cpp::Category::getInstance(LOGCAT_LOGFILEONLY);


	//logcat_fileonly.errorStream() << constraintdefs[0].ToString();
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

		isis_ProModelitemByNameInit(constraintdefs[0].p_base_model, base_limitrefs[InputJoint::DEFAULT].FeatureType, featurename_base, &base_model_item);
		isis_ProModelitemByNameInit(constraintdefs[0].p_added_model, added_limitrefs[InputJoint::DEFAULT].FeatureType, featurename_added, &added_model_item);


		ProError err = ProReferenceSet(asm_ref, &base_comp_path, &base_model_item);
		if (err != PRO_TK_NO_ERROR) throw isis::application_exception("crap");
		err = ProReferenceSet(comp_ref, &added_comp_path, &added_model_item);
		if (err != PRO_TK_NO_ERROR) throw isis::application_exception("crap");

		//logcat_fileonly.warnStream() << "Setting rotation limits on: " << constraintdefs[0].base_model_component_instance_ID << ", " << constraintdefs[0].added_model_component_instance_ID;

		Set_Limits_On_Set(set_elem, comp_ref, asm_ref, joint, joint.RotationLimits);

		// I don't know what can be freed
		//ProReferenceFree(&comp_ref);
		//ProReferenceFree(&asm_ref);
		//ProElempathFree(&setPath);

		//ProElemtreeWrite(sets_elem_tree, PRO_ELEMTREE_XML, L"crap.xml");
}

// This function is certainly very inefficient
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

		log4cpp::Category& logcat_fileonly = log4cpp::Category::getInstance(LOGCAT_LOGFILEONLY);


	//logcat_fileonly.errorStream() << constraintdefs[0].ToString();
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

		isis_ProModelitemByNameInit(constraintdefs[0].p_base_model, base_limitrefs[InputJoint::DEFAULT].FeatureType, featurename_base, &base_model_item);
		isis_ProModelitemByNameInit(constraintdefs[0].p_added_model, added_limitrefs[InputJoint::DEFAULT].FeatureType, featurename_added, &added_model_item);


		ProError err = ProReferenceSet(asm_ref, &base_comp_path, &base_model_item);
		if (err != PRO_TK_NO_ERROR) throw isis::application_exception("crap");
		err = ProReferenceSet(comp_ref, &added_comp_path, &added_model_item);
		if (err != PRO_TK_NO_ERROR) throw isis::application_exception("crap");

		//logcat_fileonly.warnStream() << "Setting translation limits on: " << constraintdefs[0].base_model_component_instance_ID << ", " << constraintdefs[0].added_model_component_instance_ID;

		Set_Limits_On_Set(set_elem, comp_ref, asm_ref, joint, joint.TranslationLimits);

		// I don't know what can be freed
		//ProReferenceFree(&comp_ref);
		//ProReferenceFree(&asm_ref);
		//ProElempathFree(&setPath);

		//ProElemtreeWrite(sets_elem_tree, PRO_ELEMTREE_XML, L"crap.xml");
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
	log4cpp::Category& logcat_fileonly = log4cpp::Category::getInstance(LOGCAT_LOGFILEONLY);

	logcat_fileonly.infoStream() << "";
	logcat_fileonly.infoStream() << "********** Begin SetConstraints, Constraint Definitions  ***************";
	
	int constraint_counter = 0;

	/////////////////////////////////////
	// Log Constraint Information
	/////////////////////////////////////
	for each ( const PerSetConstraintDefinition  &i_PerSetConstaint in in_PerSetConstraintDefinitions )
	{
		logcat_fileonly.infoStream();
		logcat_fileonly.infoStream() << "\n**** Constraint Index: " <<  constraint_counter << "  ******";
		logcat_fileonly.infoStream() << "setID:                   " <<  i_PerSetConstaint.setID;
		logcat_fileonly.infoStream() << "hasAGuideConstraint:     " <<  i_PerSetConstaint.hasAGuideConstraint;
		logcat_fileonly.infoStream() << "jointType_withoutguide:  " <<  CADJointType_string(i_PerSetConstaint.jointType_withoutguide);
		logcat_fileonly.infoStream() << "jointType_withguide:     " <<  CADJointType_string(i_PerSetConstaint.jointType_withguide);

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
			
				logcat_fileonly.infoStream() << str.str();
				logcat_fileonly.infoStream() << "1st Round: " << in_1stRound;
				logcat_fileonly.infoStream();
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

	isis::isis_ProFeatureElemtreeExtract (	&in_CADComponentData_map[in_ComponentID].assembledFeature,
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
				&in_CADComponentData_map[in_ComponentID].assembledFeature,
				elem_tree,
				opts,
				//PRO_REGEN_NO_FLAGS,
				PRO_REGEN_FORCE_REGEN,
				&p_errors);

	// Verify if regeneration succeeded
	ProSolidRegenerationStatus s;
	ProSolidRegenerationstatusGet((ProSolid)(in_TopAssembly), &s);

	if (s == PRO_SOLID_FAILED_REGENERATION)
		out_Warnings = "WARNING, Function SetConstraints - Feature redefine failed. Open the Creo assembly along with the log file to locate the problem.  ComponentInstanceID: " + in_ComponentID + "  Model Name: " + (std::string)in_CADComponentData_map[in_ComponentID].name;

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

	log4cpp::Category& logcat_fileonly = log4cpp::Category::getInstance(LOGCAT_LOGFILEONLY);

	logcat_fileonly.infoStream() << "";
	logcat_fileonly.infoStream() << "********** Begin PopulateOneConstraintInConstraintStructure  ***************";
	std::stringstream str;
	int constraint_counter = 0;
	for each ( ContraintFeatureDefinition i_constraint in in_ContraintFeatureDefinitions )
	{
		logcat_fileonly.infoStream() << "**** Constraint Index: " <<  constraint_counter << "  ******";
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
	logcat_fileonly.infoStream() << str.str();
	logcat_fileonly.infoStream() << "********** End PopulateOneConstraintInConstraintStructure  ***************" << log4cpp::eol;

	
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
	log4cpp::Category& logcat_fileonly = log4cpp::Category::getInstance(LOGCAT_LOGFILEONLY);

	logcat_fileonly.infoStream() << "";
	logcat_fileonly.infoStream() << "********** Begin PopulateOneConstraintInConstraintStructure  ***************";
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
	logcat_fileonly.infoStream() << str.str();
	logcat_fileonly.infoStream() << "********** End PopulateOneConstraintInConstraintStructure  ***************" << log4cpp::eol;
	
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
	log4cpp::Category& logcat_fileonly = log4cpp::Category::getInstance(LOGCAT_LOGFILEONLY);

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

	logcat_fileonly.infoStream() << "\nPRO_E_COMPONENT_MISC_ATTR ProValueDataGet, type: " << value_data.type;
	logcat_fileonly.infoStream() << "\nPRO_E_COMPONENT_MISC_ATTR ProValueDataGet, v: "    <<  value_data.v.i;

	//std::cout << std::endl << "\n   Type:       " << FeatureGeometryType_string(in_ProAsmcomp->type);

	if ( value_data.v.i == in_AllowAssumptions )
	{
		logcat_fileonly.infoStream() << "\nAllowAssumptions already set to: " << ConstraintAllowAssumptionse_string(in_AllowAssumptions) << ".  For:";
		logcat_fileonly.infoStream() << "\n   Feature ID: " << in_ProAsmcomp->id;
		logcat_fileonly.infoStream() << "\n   Owner:      " << in_ProAsmcomp->owner;
		logcat_fileonly.infoStream() << "\n   Type:       " << FeatureGeometryType_string(in_ProAsmcomp->type);
		
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
				cad::CadFactoryAbstract						&in_factory,
				ProSolid									&in_assembly_model,
				const std::string							&in_ComponentID,		
				ConstraintDefinition					&in_ConstraintDefinition,
				std::map<string, isis::CADComponentData>	&in_CADComponentData_map,
				bool										in_IgnoreGuides )

										throw (isis::application_exception)
{


			// First need to determine the joint type (e.g. Fixed, Revolute, Prismatic)
			std::vector<std::string>			listOfComponentIDsInTheAssembly;
			listOfComponentIDsInTheAssembly.push_back(in_ComponentID);
			PopulateMap_with_Junctions_per_InputXMLConstraints(	in_factory,
													listOfComponentIDsInTheAssembly, 
													in_CADComponentData_map );
			

			ProAsmcomp		added_model_assembled_feature;
			log4cpp::Category& logcat_fileonly = log4cpp::Category::getInstance(LOGCAT_LOGFILEONLY);
			log4cpp::Category& logcat_consoleandfile = log4cpp::Category::getInstance(LOGCAT_CONSOLEANDLOGFILE);
	
			const isis::CADComponentData &ComponentAssembledInfo_temp = in_CADComponentData_map[in_ComponentID];
			added_model_assembled_feature = ComponentAssembledInfo_temp.assembledFeature;


			ProAsmcompconstraint* constraints;
			isis::isis_ProArrayAlloc (0, sizeof (ProAsmcompconstraint), 1, (ProArray*)&constraints);

			//for ( AssemblyType::CADComponent_type::Constraint_const_iterator j(in_CADComponent_itr->Constraint().begin());
			//      j != in_CADComponent_itr->Constraint().end();
			//	  ++j )
			
			logcat_fileonly.infoStream() << "";

			
			//int numberOfConstraints = 0;
			//for each (ConstraintData j_temp in in_ConstraintDefinition.constraints ) 
			//	numberOfConstraints += j_temp.constraintPairs.size();

			std::list<int> removeConstraintIndexes; 

			cad::IAssembler& assembler = in_factory.get_assembler();

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
					ProAsmcompConstrType constraint_type = k->featureAlignmentType;

					ProType	 pro_datum_type =  k->featureGeometryType;	

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
					logcat_fileonly.infoStream() << "";

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
							added_model_datum_side = l->featureOrientationType;
							added_model_defined = true;
							added_model_constraint_feature_component_ID = l->componentInstanceID;
							//logcat_consoleandfile.infoStream() << in_CADComponentData_map[l->componentInstanceID].name << "::" << l->featureName;
							constraintFeature_From_To += (std::string)in_CADComponentData_map[l->componentInstanceID].name + "::" + (std::string)l->featureName;
							if (!base_model_defined)
							{ 
								//logcat_consoleandfile.infoStream() << " --> ";
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
							base_model_datum_side = l->featureOrientationType;
							base_model_defined = true;
							base_model_constraint_feature_component_ID = l->componentInstanceID;
							//logcat_consoleandfile.infoStream() << in_CADComponentData_map[l->componentInstanceID].name << "::" << l->featureName;
							constraintFeature_From_To += (std::string)in_CADComponentData_map[l->componentInstanceID].name + "::" + (std::string)l->featureName;
							if (!added_model_defined) 
							{ 
								//logcat_consoleandfile.infoStream() << " -zz-> ";
								constraintFeature_From_To += " --> ";
							}
						}		
					
					}
					logcat_consoleandfile.infoStream() << constraintFeature_From_To;

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
						logcat_fileonly.infoStream() << "      ConstraintOffset Value:               " <<  k->constraintOffset.value;
						logcat_fileonly.infoStream() << "      ConstraintOffset OffsetAlignmentType: " <<  ProAsmcompConstrType_string( k->constraintOffset.offsetAlignmentType );
						if ( k->constraintOffset.unitsPresent )
						{
							logcat_fileonly.infoStream() << "      ConstraintOffset Units Type:          " << CADUnitsDistance_string( k->constraintOffset.units );
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

						cFDef.p_base_model =						(ProMdl)in_CADComponentData_map[base_model_component_instance_ID].modelHandle;
						cFDef.base_model_name =						in_CADComponentData_map[base_model_component_instance_ID].name;
						cFDef.base_model_type =						in_CADComponentData_map[base_model_component_instance_ID].modelType;
						cFDef.base_model_component_instance_ID =	base_model_component_instance_ID;

						cFDef.added_model_name =					in_CADComponentData_map[added_model_component_instance_ID].name;
						cFDef.added_model_type =					in_CADComponentData_map[added_model_component_instance_ID].modelType;
						cFDef.added_model_component_instance_ID =	added_model_component_instance_ID;

						cFDef.p_added_model =						(ProMdl)in_CADComponentData_map[added_model_component_instance_ID].modelHandle;
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
			logcat_fileonly.errorStream() << msg;*/

			// 1st round of setup: install guides and the 1st kinematic joint as non-kinematic ones to establish the initial position
			if (!in_IgnoreGuides)
			{
				std::string warnings;
				SetConstraints(in_assembly_model, in_ComponentID, in_CADComponentData_map, perSetConstraintDefinitions, true, warnings );
				if ( warnings.size() > 0 )
				{
					if (isis::AssemblyOptions::GetInstance().FailLevel == isis::AssemblyOptions::FEATURE_REDEF)
					{
						logcat_consoleandfile.errorStream() << warnings;
						return true;
					} else {
						logcat_consoleandfile.warnStream() << warnings;
					}
				}

				// Regenerate placement of the assembled model
				int i = 5;
				ProSolidRegenerationStatus s = PRO_SOLID_FAILED_REGENERATION;
				while (i > 0 && s == PRO_SOLID_FAILED_REGENERATION)
				{
					isis::isis_ProAsmcompRegenerate( &in_CADComponentData_map[in_ComponentID].assembledFeature, PRO_B_TRUE);

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
						logcat_fileonly.errorStream() << errMsg;
						return true;
					} else {
						logcat_fileonly.warnStream() << errMsg;
					}
				}



				// Remove all of the constraints
				isis::isis_ProAsmcompConstrRemove (	&in_CADComponentData_map[in_ComponentID].assembledFeature, -1 );

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
					logcat_consoleandfile.errorStream() << warnings;
					return true;
				} else {
					logcat_consoleandfile.warnStream() << warnings;
				}
			}


			//ProMessageDisplay(L"msg_user.txt", "USER %0s", "regen3");
			//logcat_fileonly.errorStream() << "regen3";
			// Regenerate placement of the assembled model

			int i = 5;
			ProSolidRegenerationStatus s = PRO_SOLID_FAILED_REGENERATION;
			while (i > 0 && s == PRO_SOLID_FAILED_REGENERATION)
			{
				if (isis::AssemblyOptions::GetInstance().FullRegen)
				{
					ProSolidRegenerate(in_assembly_model, PRO_REGEN_NO_RESOLVE_MODE);
				} else {
					isis::isis_ProAsmcompRegenerate( &in_CADComponentData_map[in_ComponentID].assembledFeature, PRO_B_TRUE);
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
					logcat_fileonly.errorStream() << errMsg;
					return true;
				} else {
					logcat_fileonly.warnStream() << errMsg;
				}
			}

			return false;

			// try the following see if it solves the kinematic assem probles
			//isis::isis_ProSolidRegenerate (in_assembly_model, PRO_REGEN_NO_RESOLVE_MODE);

			// Set the assembly component constraints
			/*
			logcat_fileonly.infoStream() << "";
			logcat_fileonly.infoStream() << "********* Begin Set Constraint Set - ProAsmcompConstraintsSet ";
			logcat_fileonly.infoStream() << "added_model_assembled_feature";
			logcat_fileonly.infoStream() <<	 "   added_model_assembled_feature.id:     " << added_model_assembled_feature.id <<  log4cpp::eol <<
	     	 "   added_model_assembled_feature.type:   " << added_model_assembled_feature.type <<  log4cpp::eol <<
	     	 "   added_model_assembled_feature.owner:  " << added_model_assembled_feature.owner;
			logcat_fileonly.infoStream() << "Constraints address: " <<  constraints;
			logcat_fileonly.infoStream() << "********* End Set Constraint Set - ProAsmcompConstraintsSet " << log4cpp::eol;

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
					logcat_consoleandfile.infoStream() << "ERROR: Failed to regenerate after adding guide constraint(s) (i.e. initial position constraint(s)) to" 
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
				logcat_fileonly.infoStream() << "\nRemoved guide constraint, ComponentInstanceID: " << ComponentAssembledInfo_temp.componentID << ".  Constraint index: " << *i_del;
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
			cad::CadFactoryAbstract				&in_factory,
			ProSolid									*in_assembly_model,
			const std::list<std::string>				&in_ComponentIDsToBeConstrained, 
			bool										in_AllowUnconstrainedModels,
			std::map<string, isis::CADComponentData>	&in_CADComponentData_map,
														// Provide for the case where the first assembled part does not have
														// the datums front, top, and right defined. 
			bool										in_IgnoreGuides,
			bool										in_FirstComponentToBePositionedAsIntiiallyPlaced_IfDatumsCannotBeFound )  
														
										throw (isis::application_exception)
{
	bool fail = false;
	log4cpp::Category& logcat_fileonly = log4cpp::Category::getInstance(LOGCAT_LOGFILEONLY);
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
				logcat_fileonly.infoStream() << "      "  << 	in_CADComponentData_map[*i].name << "::Unconstrained";
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

				bool stop = Apply_CADDatum_ModelConstraints(	in_factory, 
													*in_assembly_model,
													*i,
													in_CADComponentData_map[*i].constraintDef, 
													in_CADComponentData_map,
													in_IgnoreGuides);		
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
					logcat_fileonly.infoStream() << "      "  << 	in_CADComponentData_map[*i].name << "::Coordinate System --> Assembly Coordinate System";
				}
				else
				{
					throw;
				}
			}  // END try - Catch
			logcat_fileonly.infoStream() << "      "  << 	in_CADComponentData_map[*i].name << "::Coordinate System --> Assembly Coordinate System";
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
