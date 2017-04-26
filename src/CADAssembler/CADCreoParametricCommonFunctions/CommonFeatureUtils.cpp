#include <CommonFeatureUtils.h>
#include <CreoStringToEnumConversions.h>
#include "CommonDefinitions.h"
#include <iomanip>
#include "LoggerBoost.h"

//#include "ProFeatType.h"
namespace isis
{
	void stream_AssemblyHierarchy( const CreoModelAssemblyAttributes &in_AssemblyAttributes,
									   std::ostream &out_Stream, const std::string &in_Space )
	{		
		out_Stream << std::endl << in_Space << "Model Name: " << in_AssemblyAttributes.modelname; 
		out_Stream << std::endl << in_Space << "Type:       " << ProMdlType_string(in_AssemblyAttributes.modelType);
		out_Stream << std::endl << in_Space << "Address:    " << in_AssemblyAttributes.p_solid_handle;
		out_Stream << std::endl << in_Space << "ProAsmcomp: ";
		out_Stream << "id:    " << in_AssemblyAttributes.proAsmcomp.id;
		out_Stream << "  owner: " << in_AssemblyAttributes.proAsmcomp.owner;
		out_Stream << "  type:  " << in_AssemblyAttributes.proAsmcomp.type;

		//out_Stream << std::endl << in_Space << "   id:    " << in_AssemblyAttributes.proAsmcomp.id;
		//out_Stream << std::endl << in_Space << "   owner: " << in_AssemblyAttributes.proAsmcomp.owner;
		//out_Stream << std::endl << in_Space << "   type:  " << in_AssemblyAttributes.proAsmcomp.type;

		std::string space = in_Space + "   ";

		for each ( CreoModelAssemblyAttributes i in in_AssemblyAttributes.children ) 
			stream_AssemblyHierarchy(i, out_Stream, space );
	}


typedef struct user_appdata
{
	CreoModelAssemblyAttributes &assemblyHierarchy;

	user_appdata(CreoModelAssemblyAttributes &in_assemblyHierarchy): assemblyHierarchy(in_assemblyHierarchy){};

} UserAppdata;

/*====================================================================*\
    FUNCTION :		UserAsmCompFilter()
    PURPOSE  :		A filter used by ProSolidFeatVisit() to visit
					features which are assembly components
	Example Code:	C:\Program Files\PTC\Creo 2.0\Common Files\M070\protoolkit\protk_appls\pt_userguide\ptu_asm\UgAsmCompVisit.c
\*====================================================================*/
ProError UserAsmCompFilter(
    ProFeature *feature,
    ProAppData app_data)
{
	//std::cout << std::endl << "-------- UserAsmCompFilter";

    ProFeattype ftype;

/*--------------------------------------------------------------------*\
    Get the feature type
\*--------------------------------------------------------------------*/
    isis::isis_ProFeatureTypeGet(feature, &ftype);

/*--------------------------------------------------------------------*\
    If the feature is an assembly component,
        return NO ERROR,
    else
        return CONTINUE
\*--------------------------------------------------------------------*/
    if(ftype == PRO_FEAT_COMPONENT)
	{

		ProFeatStatus  featureStatus;
		isis::isis_ProFeatureStatusGet( feature, &featureStatus );

		if ( featureStatus == PRO_FEAT_ACTIVE )
		{
			unsigned int statusFlags;
			ProFeatureStatusflagsGet  (feature,
				          &statusFlags);

			//std::cout << std::endl << "----------------------------------------------------> status flags: " << statusFlags;

			return(PRO_TK_NO_ERROR);
		}
		else
			return(PRO_TK_CONTINUE);
	}
	else
	{
		return(PRO_TK_CONTINUE);
	}
}


/*---------------------------------------------------------------------*\
    Populate appdata
	Example Code:	C:\Program Files\PTC\Creo 2.0\Common Files\M070\protoolkit\protk_appls\pt_userguide\ptu_asm\UgAsmCompVisit.c
\*---------------------------------------------------------------------*/
ProError user_action( ProFeature *feature, ProError status, ProAppData appdata)
{

    ProMdl mdl;
    char name[PRO_NAME_SIZE];
    char type[PRO_TYPE_SIZE];
    // wchar_t wname[PRO_NAME_SIZE];
    UserAppdata *appd;
    ProMdldata mdldata;

    appd = (UserAppdata *)appdata;

	try
	{
		isis::isis_ProAsmcompMdlGet(feature , &mdl);
	}
	catch(...)
	{
		
		// Call again to get the error code
		ProError  proError_temp = ProAsmcompMdlGet(feature , &mdl);
		// PRO_TK_E_NOT_FOUND  - Assembly component model is not a solid or is not in memory.
		//						 It would not be in memory because it was not pulled in as a simplified rep
		if ( proError_temp == PRO_TK_E_NOT_FOUND )
		{
			//std::cout << std::endl << std::endl << "@@@@@@@@@@@@@@@@@@@@@ isis_ProAsmcompMdlGet @@@@@@@@@@@@@@@@@@@@@@@@@@@@";
			isis_LOG(lg, isis_FILE, isis_ERROR) << "INFORMATION: isis_ProAsmcompMdlGet(feature , &mdl), Could not retrieve sub-part/assembly, probably because the simplified rep did not contain the part/assembly";
			isis_LOG(lg, isis_FILE, isis_ERROR) << "isis_ProAsmcompMdlGet(feature , &mdl), feature: " << feature;
			isis_LOG(lg, isis_FILE, isis_ERROR) << "                                       id       " << feature->id;
			isis_LOG(lg, isis_FILE, isis_ERROR) << "                                       owner    " << feature->owner;
			isis_LOG(lg, isis_FILE, isis_ERROR) <<  "                                      type     " << feature->type;	
			return(PRO_TK_CONTINUE);
		}
		else
		{
			isis_LOG(lg, isis_FILE, isis_ERROR) << "isis_ProAsmcompMdlGet(feature , &mdl), feature: " << feature;
			isis_LOG(lg, isis_FILE, isis_ERROR) << "                                        id      " << feature->id;
			isis_LOG(lg, isis_FILE, isis_ERROR) << "                                        owner   " << feature->owner;
			isis_LOG(lg, isis_FILE, isis_ERROR) <<  "                                       type    " << feature->type;	
			throw;
		}
	}

    isis::isis_ProMdlDataGet(mdl, &mdldata);
    ProWstringToString(name,mdldata.name);
    ProWstringToString(type,mdldata.type);

	CreoModelAssemblyAttributes modelAttributes;
	if ( strncmp(type,"ASM",3) == 0 )
		modelAttributes.modelType = PRO_MDL_ASSEMBLY;
	else
		modelAttributes.modelType = PRO_MDL_PART;

	modelAttributes.modelname = mdldata.name;
	modelAttributes.p_solid_handle = (ProSolid)mdl;
	modelAttributes.proAsmcomp = *feature;

	if ( appd->assemblyHierarchy.includeTheEntireHierarchy && strncmp(type,"ASM",3) == 0)
    {
		UserAppdata appdata_temp(modelAttributes);
		appdata_temp.assemblyHierarchy.includeTheEntireHierarchy = true;
		isis::isis_ProSolidFeatVisit((ProSolid)mdl, user_action, UserAsmCompFilter, &appdata_temp);
    }

	appd->assemblyHierarchy.children.push_back(modelAttributes);

    if (feature != NULL) return(PRO_TK_NO_ERROR);
    return(PRO_TK_CONTINUE);
}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void RetrieveAssemblyHierarchyInformation (  
									const ProSolid					  p_solid_handle,
									bool							  in_IncludeTheEntireHierarchy_NotJustImmediateDependents,
									CreoModelAssemblyAttributes        &out_AssemblyHierarchy  )
									throw (isis::application_exception)
	{	
		if ( p_solid_handle == NULL )
		{
			std::stringstream errorString;
			errorString << "BuildAssemblyHierarchy was passed a NULL pointer for p_solid_handle.";
					throw isis::application_exception(errorString);
		}

		UserAppdata appdata(out_AssemblyHierarchy);

		ProMdldata mdldata;
		isis::isis_ProMdlDataGet(p_solid_handle, &mdldata);

		char name[PRO_NAME_SIZE];
		char type[PRO_TYPE_SIZE];

		ProWstringToString(name,mdldata.name);
		ProWstringToString(type,mdldata.type);

		if ( in_IncludeTheEntireHierarchy_NotJustImmediateDependents) 
			appdata.assemblyHierarchy.includeTheEntireHierarchy = true;
		else
			appdata.assemblyHierarchy.includeTheEntireHierarchy = false;

		if ( strncmp(type,"ASM",3) == 0 )
		{
			appdata.assemblyHierarchy.modelType = PRO_MDL_ASSEMBLY;
			appdata.assemblyHierarchy.proAsmcomp.type = PRO_TYPE_UNUSED;
			appdata.assemblyHierarchy.proAsmcomp.id = 0;
			appdata.assemblyHierarchy.proAsmcomp.owner = 0;
		}
		else
		{
			// This would be a PRO_MDL_PART
			std::stringstream errorString;
			errorString << "BuildAssemblyHierarchy was passed a p_solid_handle that pointed to a model " << type  << " type.  The only allowed model type is ASM.";
					throw isis::application_exception(errorString);
		}

		appdata.assemblyHierarchy.modelname = mdldata.name;
		appdata.assemblyHierarchy.p_solid_handle = p_solid_handle;

		isis::isis_ProSolidFeatVisit(p_solid_handle, user_action, UserAsmCompFilter, &appdata);
	}


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::ostream& operator<<( std::ostream & in_stream,  ProModelitem &in_ModelItem  )
	{
		///////////////////////////
		// Model Item Type
		///////////////////////////

		try // FeatureGeometryType_string does not support all the ProType(s) 
		{
			in_stream << std::left << std::setw(32) << "   feature type: " <<	FeatureGeometryType_string(in_ModelItem.type) << std::endl;
		}
		catch (...)
		{
			in_stream << std::left << std::setw(32) << "   feature type: " <<	in_ModelItem.type << std::endl;
		}

		////////////////////////////
		// Model Item Owner Address
		////////////////////////////
		in_stream << std::left << std::setw(32) << "   feature owner address: " <<	in_ModelItem.owner << std::endl;

		////////////////////////////
		// Model Item Owner Name
		////////////////////////////
		/*  This always throws an exception, probably because a datum's owner is probably not a legitimate model item.
		ProModelitem ownerModelItem;
		ProName ownerName_wide;
		isis::MultiFormatString  ownerName_multi;

		try 
		{
			isis_ProMdlToModelitem (in_ModelItem.owner, &ownerModelItem);
			isis_ProModelitemNameGet ( &ownerModelItem,  ownerName_wide);
			ownerName_multi = ownerName_wide;
		}

		catch (isis::application_exception &ex )
		{
			in_stream << "   feature owner name: could not retrieve feature owner name, Exception: "  << ex.what() << std::endl;
		}
		*/
		////////////////////////////
		// Model Item Feature ID
		////////////////////////////

		in_stream << std::left << std::setw(32) << "   feature ID: " <<	in_ModelItem.id << std::endl;

		////////////////////////////
		// Model Item Feature Name
		////////////////////////////
		//  This always throws an exception, probably because all model items don't have a name.
		//ProName      modelItemName_wide;
		//isis::MultiFormatString modelItemName_multi;
		//try
		//{
		//	isis_ProModelitemNameGet ( &in_ModelItem, modelItemName_wide );
		//	modelItemName_multi = modelItemName_wide;
		//	in_stream << std::left << std::setw(32) << "   feature name: " << modelItemName_multi << std::endl;		
		//}
		//catch (isis::application_exception &ex )
		//{
		//	in_stream << std::left << std::setw(32) << "   feature name: could not retrieve feature  name, Exception: "  << ex.what() << std::endl;
		//}
		
		return in_stream;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::ostream& operator<<( std::ostream & in_stream, const CreoConstraintDefinition &in_ConstrDef )
	{
		in_stream << "***** Constraint-Definition ******" << std::endl;
		in_stream << "   component_constr_type: "		<< ProAsmcompConstrType_string(in_ConstrDef.component_constr_type) << std::endl;

		ProModelitem modelItem;

		/////////////////////////////
		//component_comp_constr_ref
		////////////////////////////
		isis::isis_ProSelectionModelitemGet(in_ConstrDef.component_comp_constr_ref, &modelItem );
		in_stream << "   ***** component_comp_constr_ref *****"  <<   std::endl;
		in_stream << std::left << std::setw(32) << "   address: " <<	in_ConstrDef.component_comp_constr_ref << std::endl;
		in_stream << modelItem;


		/////////////////////////////
		//component_assem_constr_ref
		////////////////////////////

		isis::isis_ProSelectionModelitemGet(in_ConstrDef.component_assem_constr_ref, &modelItem );
		in_stream << "   ***** component_assem_constr_ref *****"  <<  std::endl; 
		in_stream << std::left << std::setw(32) << "   address: " <<	in_ConstrDef.component_assem_constr_ref << std::endl;

		in_stream << modelItem;

		////////////////////////////
		// Offset, attr, orient
		////////////////////////////
		in_stream << "   ***** common attributes *****"  <<  std::endl;
		in_stream << std::left << std::setw(32)  << "   component_constr_ref_offset: "  <<   in_ConstrDef.component_constr_ref_offset << std::endl;

		in_stream << std::left << std::setw(32) <<  "   component_constr_attr: "  <<   in_ConstrDef.component_constr_attr << std::endl;

		in_stream << std::left << std::setw(32) <<  "   component_comp_orient: "  <<   in_ConstrDef.component_comp_orient << std::endl;
		in_stream << std::left << std::setw(32) <<  "   component_assm_orient: "  <<   in_ConstrDef.component_assm_orient << std::endl;
		in_stream << std::left << std::setw(32) <<  "   component_constr_set_id: "  <<   in_ConstrDef.component_constr_set_id << std::endl;

		return in_stream;
	}

	std::ostream& operator<<(std::ostream & in_stream, const CreoConstraintSetDefinition &in_CreoConstraintSetDefinition)
	{
		in_stream << "***** Set-Definition ******:" << std::endl;
		in_stream << "   component_set_id:        "	<< in_CreoConstraintSetDefinition.component_set_id << std::endl;
		in_stream << "   component_set_type:      "	<< ProAsmcompSetType_string(in_CreoConstraintSetDefinition.component_set_type) << std::endl;
		in_stream << "   component_set_misc_attr: "	<< in_CreoConstraintSetDefinition.component_set_misc_attr << std::endl;
		in_stream << "   component_set_flip:      "	<< in_CreoConstraintSetDefinition.component_set_flip << std::endl;
		return in_stream;
	}

	std::ostream& operator<<(std::ostream & in_stream, const CreoAssembledFeatureDefinition &in_CreoAssembledFeatureDefinition)
	{
		in_stream << "****************************************************************************" << std::endl;
		in_stream << "Creo-Assembled-Feature-Definition read from the assembled-model feature tree" << std::endl;
		in_stream << "Model Name:            " <<  (std::string)in_CreoAssembledFeatureDefinition.modelName << std::endl;
		in_stream << "Component Instance ID: " <<  (std::string)in_CreoAssembledFeatureDefinition.componentInstanceID << std::endl;
		in_stream << "****************************************************************************" << std::endl;

		if ( in_CreoAssembledFeatureDefinition.constraintSetDefinitions.size() == 0 ) 
			in_stream << "No sets found." << std::endl;

		for each ( CreoConstraintSetDefinition i in in_CreoAssembledFeatureDefinition.constraintSetDefinitions) in_stream << i;

		if ( in_CreoAssembledFeatureDefinition.constraintDefinitions.size() == 0 ) 
			in_stream << "No constraints found with valid references." << std::endl;

		for each ( CreoConstraintDefinition i in in_CreoAssembledFeatureDefinition.constraintDefinitions) in_stream << i;
		return in_stream;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int GetIntegerFromFeatureTree(ProElement in_ElementTree, ProElempathItem in_ElemPath[], int in_PathSize)
																				throw (isis::application_exception)
	{
		int value;
		ProElempath path;
		ProElement element;
		isis::isis_ProElempathAlloc(&path);
		isis::isis_ProElempathDataSet(path, in_ElemPath,in_PathSize);
		isis::isis_ProElemtreeElementGet(in_ElementTree, path, &element);
		isis::isis_ProElementIntegerGet(element, 0, &value);
		isis::isis_ProElempathFree(&path);
		return value;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	double GetDoubleFromFeatureTree(ProElement in_ElementTree, ProElempathItem in_ElemPath[], int in_PathSize)
																				throw (isis::application_exception)
	{	
		double value;
		ProElempath path;
		ProElement element;
		isis::isis_ProElempathAlloc(&path);
		isis::isis_ProElempathDataSet(path, in_ElemPath,in_PathSize);
		isis::isis_ProElemtreeElementGet(in_ElementTree, path, &element);
		isis::isis_ProElementDoubleGet(element, 0, &value);
		isis::isis_ProElempathFree(&path);
		return value;	
	}

	ProSelection GetSelectionFromFeatureTree(ProElement in_ElementTree, ProElempathItem in_ElemPath[], int in_PathSize)
																				throw (isis::application_exception)
	{	
		ProElempath path;
		ProElement element;
		isis::isis_ProElempathAlloc(&path);
		isis::isis_ProElempathDataSet(path, in_ElemPath,in_PathSize);
		isis::isis_ProElemtreeElementGet(in_ElementTree, path, &element);

		ProValue value;
		isis::isis_ProElementValueGet(element, &value);
		
		ProValueData value_data;
		isis::isis_ProValueDataGet(value, &value_data);


		isis::isis_ProElempathFree(&path);
		return value_data.v.r;	
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void RetrieveCreoElementTreeConstraints(	ProFeature &in_comp, 
												ProAsmcomppath &in_comppath, 
												CreoAssembledFeatureDefinition  &out_AssembledFeatureDefinition,
												ProElement &out_ElemTree)
															throw (isis::application_exception)
	{

		//ProElement elemTree;
		isis::isis_ProFeatureElemtreeExtract (&in_comp, &in_comppath, PRO_FEAT_EXTRACT_NO_OPTS, &out_ElemTree );

		//Write tree for debuging
		/*string featureTreeFileName_narrow = (string)out_AssembledFeatureDefinition.modelName + "_retrieve.xml";
	    MultiFormatString featureTreeFileName_multi = (string)featureTreeFileName_narrow;
	    ProElemtreeWrite(out_ElemTree, PRO_ELEMTREE_XML, (wchar_t*)(const wchar_t*)featureTreeFileName_multi);*/

		//////////////////////////////
		// Get constraint set array
		//////////////////////////////
		int count;

		ProElement* constraintSetsArray;

		ProElempathItem constraint_sets_path_items[] = {
			{PRO_ELEM_PATH_ITEM_TYPE_ID, PRO_E_COMPONENT_SETS},
		};

		ProElempath contraint_sets_elem_path;
		isis::isis_ProElempathAlloc(&contraint_sets_elem_path);
		isis::isis_ProElempathDataSet(contraint_sets_elem_path, constraint_sets_path_items, sizeof(constraint_sets_path_items)/sizeof(ProElempathItem));

		isis::isis_ProElementArrayCount(out_ElemTree, contraint_sets_elem_path, &count);

		isis::isis_ProArrayAlloc(count,sizeof(ProElement),1,(ProArray*) &constraintSetsArray);

		isis::isis_ProElementArrayGet(out_ElemTree, contraint_sets_elem_path, &constraintSetsArray);

		// Iterate through constraint sets
		for (int i = 0; i < count; ++i)
		{
			CreoConstraintSetDefinition constraintSetDefinition;
			////////////////////////////////
			// Get component_set_id
			////////////////////////////////
			ProElempathItem elemPath[] = {
				{PRO_ELEM_PATH_ITEM_TYPE_ID, PRO_E_COMPONENT_SETS},
				{PRO_ELEM_PATH_ITEM_TYPE_INDEX, i},
				{PRO_ELEM_PATH_ITEM_TYPE_ID, PRO_E_COMPONENT_SET_ID},
			};

			constraintSetDefinition.component_set_id = GetIntegerFromFeatureTree(out_ElemTree, elemPath, 3);
			
			/////////////////////////////////////////
			// Get component_set_type 
			////////////////////////////////////////
			elemPath[2].path_item.elem_id = PRO_E_COMPONENT_SET_TYPE;	
			constraintSetDefinition.component_set_type = (ProAsmcompSetType)GetIntegerFromFeatureTree(out_ElemTree, elemPath, 3);

			/////////////////////////////////////////
			// Get component_set_misc_attr 
			////////////////////////////////////////
			elemPath[2].path_item.elem_id = PRO_E_COMPONENT_SET_MISC_ATTR;	
			constraintSetDefinition.component_set_misc_attr = GetIntegerFromFeatureTree(out_ElemTree, elemPath, 3);

			/////////////////////////////////////////
			// Get component_set_flip 
			////////////////////////////////////////
			elemPath[2].path_item.elem_id = PRO_E_COMPONENT_SET_FLIP;	
			constraintSetDefinition.component_set_flip = GetIntegerFromFeatureTree(out_ElemTree, elemPath, 3);
			


			out_AssembledFeatureDefinition.constraintSetDefinitions.push_back(constraintSetDefinition);
		}
		//////////////////////////////
		// Get constraint array
		//////////////////////////////
		ProElement* constraintArray;

		ProElempathItem constraints_path_items[] = {
			{PRO_ELEM_PATH_ITEM_TYPE_ID, PRO_E_COMPONENT_CONSTRAINTS},
		};

		ProElempath contraints_elem_path;
		isis::isis_ProElempathAlloc(&contraints_elem_path);
		isis::isis_ProElempathDataSet(contraints_elem_path, constraints_path_items, sizeof(constraints_path_items)/sizeof(ProElempathItem));

		isis::isis_ProElementArrayCount(out_ElemTree, contraints_elem_path, &count);

		isis::isis_ProArrayAlloc(count,sizeof(ProElement),1,(ProArray*) &constraintArray);

		isis::isis_ProElementArrayGet(out_ElemTree, contraints_elem_path, &constraintArray);

		// Iterate through constraints
		for (int i = 0; i < count; ++i)
		{
			CreoConstraintDefinition constraintDefinition;

			////////////////////////////////
			// Get component_constr_type ID
			////////////////////////////////
			ProElempathItem elemPath[] = {
				{PRO_ELEM_PATH_ITEM_TYPE_ID, PRO_E_COMPONENT_CONSTRAINTS},
				{PRO_ELEM_PATH_ITEM_TYPE_INDEX, i},
				{PRO_ELEM_PATH_ITEM_TYPE_ID, PRO_E_COMPONENT_CONSTR_TYPE},
			};

			constraintDefinition.component_constr_type = (ProAsmcompConstrType)GetIntegerFromFeatureTree(out_ElemTree, elemPath, 3);

			///////////////////////////
			// Get comp_constr_ref 
			///////////////////////////////

			elemPath[2].path_item.elem_id = PRO_E_COMPONENT_COMP_CONSTR_REF;

			constraintDefinition.component_comp_constr_ref = GetSelectionFromFeatureTree(out_ElemTree, elemPath, 3);

			ProModelitem modelItem;
			// Check if the Modelitem can be retrieved.  If so this is not a NULL or PRO_REF_MISSING reference
			try
			{
				isis::isis_ProSelectionModelitemGet(constraintDefinition.component_comp_constr_ref, &modelItem );
			}
			catch (...)
			{
				//std::cout << std::endl << "Model item not found";
				continue;
			}

			///////////////////////////////
			// Get assem_constr_ref 
			///////////////////////////////

			elemPath[2].path_item.elem_id = PRO_E_COMPONENT_ASSEM_CONSTR_REF;

			constraintDefinition.component_assem_constr_ref = GetSelectionFromFeatureTree(out_ElemTree, elemPath, 3);

			// Check if the Modelitem can be retrieved.  If so this is not a NULL or PRO_REF_MISSING reference
			try
			{
				isis::isis_ProSelectionModelitemGet(constraintDefinition.component_assem_constr_ref, &modelItem );
			}
			catch (...)
			{
				continue;
			}

			/////////////////////////////////////////
			// Get component_constr_ref_offset 
			////////////////////////////////////////		
			elemPath[2].path_item.elem_id = PRO_E_COMPONENT_CONSTR_REF_OFFSET;	
			constraintDefinition.component_constr_ref_offset = GetDoubleFromFeatureTree(out_ElemTree, elemPath, 3);

			/////////////////////////////////////////
			// Get component_constr_attr 
			////////////////////////////////////////
			elemPath[2].path_item.elem_id = PRO_E_COMPONENT_CONSTR_ATTR;	
			constraintDefinition.component_constr_attr = GetIntegerFromFeatureTree(out_ElemTree, elemPath, 3);

			/////////////////////////////////////////
			// Get component_comp_orient 
			////////////////////////////////////////
			elemPath[2].path_item.elem_id = PRO_E_COMPONENT_COMP_ORIENT;		
			constraintDefinition.component_comp_orient = GetIntegerFromFeatureTree(out_ElemTree, elemPath, 3);

			/////////////////////////////////////////
			// Get component_assm_orient 
			////////////////////////////////////////
			elemPath[2].path_item.elem_id = PRO_E_COMPONENT_ASSM_ORIENT;	
			constraintDefinition.component_assm_orient = GetIntegerFromFeatureTree(out_ElemTree, elemPath, 3);

			///////////////////////////////
			// Get Set ID
			///////////////////////////////
			elemPath[2].path_item.elem_id = PRO_E_COMPONENT_CONSTR_SET_ID;
			int setID = GetIntegerFromFeatureTree(out_ElemTree, elemPath, 3);
			constraintDefinition.component_constr_set_id = setID;
			out_AssembledFeatureDefinition.constraintDefinitions.push_back(constraintDefinition);

		}


		//std::cout << std::endl << std::endl << out_AssembledFeatureDefinition;

		// Don't free this here, doing so would invalidate the ProSelections in out_AssembledFeatureDefinition.
		// The caller of this routine should do the free.
		//ProFeatureElemtreeFree(&in_comp, elemTree);
	}
	

} // END namespace isis