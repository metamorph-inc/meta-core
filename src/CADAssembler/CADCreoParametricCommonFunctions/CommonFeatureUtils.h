#ifndef COMMON_FEATURE_UTILS
#define COMMON_FEATURE_UTILS

#include "CADStringToEnumConversions.h"
#include <isis_ptc_toolkit_functions.h>
#include <isis_application_exception.h>
#include <MultiFormatString.h>
#include <ostream>
#include <vector> 
#include <map>
#include <UtilTree.h>

namespace isis
{
	struct CreoModelAssemblyAttributes
	{
		MultiFormatString								modelname;
		//ProMdlType									modelType;
		e_CADMdlType									modelType;
		ProSolid										p_solid_handle;
		ProAsmcomp										proAsmcomp;
		std::vector<CreoModelAssemblyAttributes>		children;
		bool											includeTheEntireHierarchy;

		CreoModelAssemblyAttributes() : includeTheEntireHierarchy(true){};
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Description:
	//		Retrieve the list of the hierarchy  for the p_solid_handle assembly.  The hierarchy includes the p_solid_handle
	//		plus its children.  The hierarchy  does not include suppressed parts/sub-assemblies.  It only include active
	//		parts/sub-assemblies
	//		if (in_IncludeTheEntireHierarchy_NotJustImmediateDependents == true )
	//			then
	//				the children, grandchildren, great grandchildren... are included.
	//
	//	Pre-Conditions:
	//		p_solid_handle must point to a valide Creo feature
	//
	//	Post-Conditions:
	//
	//		If p_solid_handle == NULL then throw isis::application_exception 
	//		If p_solid_handle points to any feature type other than an assembly, then throw isis::application_exception 
	//
	//		If p_solid_handle points to an assembly, then the structure out_AssemblyHierarchy is returned where
	//		this structure contains the assembly information for the p_solid_handle assembly plus the lists
	//		of the children and if (in_IncludeTheEntireHierarchy_NotJustImmediateDependents == true ) the
	//		grandchildren, great grandchildren... are included.
	//
	//		WARNING - The first entry in out_AssemblyHierarchy is the entry for p_solid_handle.  For that entry,
	//				  the values for proAsmcomp are not populated.  Therefore, for the first entry, DO NOT use 
	//				  those values.  For the children of the first entry all values are populated.
	//
	//		Note - use the function stream_AssemblyHierarchy to stream the contents of out_AssemblyHierarchy.
	//
	//		Example of a returned hierarchy that was formatted via stream_AssemblyHierarchy.
	/*			
			Saved Assembly:   Brake  310b7502-54c8-4f0c-9cd2-6cbf8df15049|1
			Model Name: 2_PISTON_CALIPER
			Type:       PRO_MDL_ASSEMBLY
			Address:    0000000012680638
			ProAsmcomp: id:    0  owner: 0000000000000000  type:  -1
			   Model Name: 2_PISTON_CALIPER
			   Type:       PRO_MDL_PART
			   Address:    0000000009716C98
			   ProAsmcomp: id:    39  owner: 0000000012680638  type:  3
			   Model Name: BRAKE_PISTON
			   Type:       PRO_MDL_PART
			   Address:    0000000012ABED48
			   ProAsmcomp: id:    40  owner: 0000000012680638  type:  3
			   Model Name: BRAKE_PISTON
			   Type:       PRO_MDL_PART
			   Address:    0000000012ABED48
			   ProAsmcomp: id:    42  owner: 0000000012680638  type:  3
			   Model Name: BRAKE_PISTON
			   Type:       PRO_MDL_PART
			   Address:    0000000012ABED48
			   ProAsmcomp: id:    44  owner: 0000000012680638  type:  3
			   Model Name: BRAKE_PISTON
			   Type:       PRO_MDL_PART
			   Address:    0000000012ABED48
			   ProAsmcomp: id:    46  owner: 0000000012680638  type:  3
			   Model Name: 4_PISTON_PAD
			   Type:       PRO_MDL_PART
			   Address:    0000000012EB2668
			   ProAsmcomp: id:    47  owner: 0000000012680638  type:  3
			   Model Name: 4_PISTON_PAD
			   Type:       PRO_MDL_PART
			   Address:    0000000012EB2668
			   ProAsmcomp: id:    49  owner: 0000000012680638  type:  3
			Model Name: 2_PISTON_CALIPER_02
			Type:       PRO_MDL_ASSEMBLY
			Address:    0000000012ABE018
			ProAsmcomp: id:    0  owner: 0000000000000000  type:  -1
			   Model Name: 2_PISTON_CALIPER
			   Type:       PRO_MDL_PART
			   Address:    0000000009716C98
			   ProAsmcomp: id:    39  owner: 0000000012ABE018  type:  3
			   Model Name: BRAKE_PISTON
			   Type:       PRO_MDL_PART
			   Address:    0000000012ABED48
			   ProAsmcomp: id:    40  owner: 0000000012ABE018  type:  3
			   Model Name: BRAKE_PISTON
			   Type:       PRO_MDL_PART
			   Address:    0000000012ABED48
			   ProAsmcomp: id:    42  owner: 0000000012ABE018  type:  3
			   Model Name: BRAKE_PISTON
			   Type:       PRO_MDL_PART
			   Address:    0000000012ABED48
			   ProAsmcomp: id:    44  owner: 0000000012ABE018  type:  3
			   Model Name: BRAKE_PISTON
			   Type:       PRO_MDL_PART
			   Address:    0000000012ABED48
			   ProAsmcomp: id:    46  owner: 0000000012ABE018  type:  3
			   Model Name: 4_PISTON_PAD
			   Type:       PRO_MDL_PART
			   Address:    0000000012EB2668
			   ProAsmcomp: id:    47  owner: 0000000012ABE018  type:  3
			   Model Name: 4_PISTON_PAD
			   Type:       PRO_MDL_PART
			   Address:    0000000012EB2668
			   ProAsmcomp: id:    49  owner: 0000000012ABE018  type:  3
			   Model Name: 2_PISTON_CALIPER_03
			   Type:       PRO_MDL_ASSEMBLY
			   Address:    0000000012EDEEC8
			   ProAsmcomp: id:    72  owner: 0000000012ABE018  type:  3
				  Model Name: 2_PISTON_CALIPER
				  Type:       PRO_MDL_PART
				  Address:    0000000009716C98
				  ProAsmcomp: id:    39  owner: 0000000012EDEEC8  type:  3
				  Model Name: BRAKE_PISTON
				  Type:       PRO_MDL_PART
				  Address:    0000000012ABED48
				  ProAsmcomp: id:    40  owner: 0000000012EDEEC8  type:  3
				  Model Name: BRAKE_PISTON
				  Type:       PRO_MDL_PART
				  Address:    0000000012ABED48
				  ProAsmcomp: id:    42  owner: 0000000012EDEEC8  type:  3
				  Model Name: BRAKE_PISTON
				  Type:       PRO_MDL_PART
				  Address:    0000000012ABED48
				  ProAsmcomp: id:    44  owner: 0000000012EDEEC8  type:  3
				  Model Name: BRAKE_PISTON
				  Type:       PRO_MDL_PART
				  Address:    0000000012ABED48
				  ProAsmcomp: id:    46  owner: 0000000012EDEEC8  type:  3
				  Model Name: 4_PISTON_PAD
				  Type:       PRO_MDL_PART
				  Address:    0000000012EB2668
				  ProAsmcomp: id:    47  owner: 0000000012EDEEC8  type:  3
				  Model Name: 4_PISTON_PAD
				  Type:       PRO_MDL_PART
				  Address:    0000000012EB2668
				  ProAsmcomp: id:    49  owner: 0000000012EDEEC8  type:  3
	*/
	//		
	void RetrieveAssemblyHierarchyInformation (  
						const ProSolid					p_solid_handle,
						bool							in_IncludeTheEntireHierarchy_NotJustImmediateDependents,
						CreoModelAssemblyAttributes     &out_AssemblyHierarchy  )
									throw (isis::application_exception);

	void stream_AssemblyHierarchy( const CreoModelAssemblyAttributes &in_AssemblyAttributes,
									    std::ostream &out_Stream, const std::string &in_Space = "" );





	struct CreoConstraintDefinition
	{
		// See ProAsmcomp.h for the entire feature tree for an assembled model
		ProAsmcompConstrType	component_constr_type;			//  e.g. PRO_ASM_MATE/PRO_ASM_ALIGN		Tree: PRO_E_COMPONENT_CONSTR_TYPE   Type: PRO_VALUE_TYPE_INT
		ProSelection			component_comp_constr_ref;		//										Tree: PRO_E_COMPONENT_COMP_CONSTR_REF            
		ProSelection			component_assem_constr_ref;
		double					component_constr_ref_offset;	// Mate/align offset  
		int						component_constr_attr;          // JAS Limits valid values:
		int						component_comp_orient;			//  For connection constraint sets it means ProConnectionFlipState
		int						component_assm_orient;			//  For connection constraint sets it means ProConnectionFlipState
		int						component_constr_set_id;		//	Set ID, this is really the offset into the array 
																// ( i.e. std::vector<CreoConstraintSetDefinition>	constraintSetDefinitions)
																//  of sets.
																
		CreoConstraintDefinition() : component_comp_constr_ref(NULL), 
									 component_assem_constr_ref(NULL),
									 component_constr_ref_offset(0.0),
									 component_constr_attr(0),
									 component_comp_orient(0) {}
			
	};

	std::ostream& operator<<(std::ostream & in_stream, const CreoConstraintDefinition &in_CreoConstraintDefinition);

	struct CreoConstraintSetDefinition
	{
		int							component_set_id;
		ProAsmcompSetType			component_set_type;
		isis::MultiFormatString		component_set_name;
		int							component_set_misc_attr;
		int							component_set_flip;

		CreoConstraintSetDefinition() : component_set_id(0), 
										component_set_type(PRO_ASM_SET_USER_DEFINED_TYPE ),
										component_set_misc_attr(0),
										component_set_flip(0) {}
	};

	std::ostream& operator<<(std::ostream & in_stream, const CreoConstraintSetDefinition &in_CreoConstraintSetDefinition);

	struct CreoAssembledFeatureDefinition
	{
		isis::MultiFormatString						modelName;
		std::string									componentInstanceID;
		std::vector<CreoConstraintSetDefinition>	constraintSetDefinitions;
		std::vector<CreoConstraintDefinition>		constraintDefinitions;

	};

	std::ostream& operator<<(std::ostream & in_stream, const CreoAssembledFeatureDefinition &in_CreoAssembledFeatureDefinition);



	// Description:
	//	This function retrieves the feature tree for the assembled model specified per in_comp.  It then reads 
	//	the constraint set information and constraints information and populates out_AssembledFeatureDefinition.
	//
	//	In some cases, the constraints do not point to a valid reference.  This is normal, for example a .prt that 
	//	was fixed in space would not be constrained to valid PreSelections. The test for a valid ProSelection is to 
	//	call ProSelectionModelitemGet.  If this returns a valid ModelItem, the ProSelection is valid.
	//  If the ProSelection was invalid, then out_AssembledFeatureDefinition.constraintDefinitions.size() == 0.  
	//	Also, some assembly feature trees do not have constraints sets defined.  For that case, 
	//	out_AssembledFeatureDefinition.constraintSetDefinitions.size() == 0.
	//
	// Pre-Conditions
	//	in_comp must point to a valid feature tree
	//	in_comppath must be a valid path to comp
	//
	//	Post-Conditions:
	//		if anomalies occur, isis::application_exception would be thrown
	//		otherwise, a completed out_AssembledFeatureDefinition would be returned
	//
	//		The caller is responsible for freeing out_ElemTree.  Once the element tree is freed,
	//		to contents of out_AssembledFeatureDefinition are no longer valid.  ProSelection references
	//		will fail.

	void RetrieveCreoElementTreeConstraints(	
					ProFeature						&in_comp, 
					ProAsmcomppath					&in_comppath, 
					CreoAssembledFeatureDefinition  &out_AssembledFeatureDefinition,
					ProElement						&out_ElemTree)
																	throw (isis::application_exception);

	int GetIntegerFromFeatureTree(ProElement in_ElementTree, ProElempathItem in_ElemPath[], int in_PathSize)
																	throw (isis::application_exception);

	double GetDoubleFromFeatureTree(ProElement in_ElementTree, ProElempathItem in_ElemPath[], int in_PathSize)
																	throw (isis::application_exception);

} // END namespace isis

#endif