#include "DatumRefResolver.h"
#include "ProToolkitErrors.h"
#include "ProElement.h"
#include "ProFeature.h"
#include "ProFeatType.h"
#include "isis_application_exception.h"
#include "isis_ptc_toolkit_functions.h"
#include "UIFunctions.h"
#include "ProMdl.h"
#include "ProModelitem.h"

namespace isis
{

DatumRefResolver::DatumRefResolver(ProSelection datum)
{
	sel = datum;
}


static void GetModelItemFromRef(ProReference ref, ProModelitem *item)
{
    ProMdl owner;
    int id;
    ProType type;
    ProReferenceOwnerGet(ref, &owner);
    ProReferenceIdGet(ref, &id);
    ProReferenceTypeGet(ref, &type);
    ProModelitemInit(owner, id, type, item);
}

void DatumRefResolver::HandleDatumPlane(ProSelection datum, vector<ProSelection> & result)
{
	ProAsmcomppath comppath;
	isis_ProSelectionAsmcomppathGet(datum, &comppath);

	ProModelitem mitem;
	isis_ProSelectionModelitemGet(datum, &mitem);

	ProFeature* feat;
	ProFeature f;

	if (mitem.type == PRO_FEATURE)
	{
		feat = &mitem;
	} else if (mitem.type == PRO_SURFACE || mitem.type == PRO_AXIS || mitem.type == PRO_EDGE || mitem.type == PRO_CSYS)
	{
		ProError err = isis_ProGeomitemFeatureGet ((ProGeomitem*)&mitem, &f);
		if (err != PRO_TK_NO_ERROR)
		{
			throw isis::application_exception("ProGeomitemFeatureGet error");
		}
		feat = &f;
	} else {
		throw isis::application_exception("DatumRefResolver::Resolve(): Unsupported datum type, only AXIS, PLANE and CSYS are supported. Datum type is:" + std::to_string((long long)mitem.type));
	}

	ProElement elemTree;
	isis_ProFeatureElemtreeExtract (feat, &comppath, PRO_FEAT_EXTRACT_NO_OPTS, &elemTree );

	ProElempathItem constraints_path_items[] = {
		{PRO_ELEM_PATH_ITEM_TYPE_ID, PRO_E_DTMPLN_CONSTRAINTS},
	};

	/*wostringstream ostr;
	ostr << feat->id;
	ostr << L"elemtree.xml";
	ProPath filepath;
	wcscpy(filepath, ostr.str().c_str());
	isis_ProElemtreeWrite(elemTree, PRO_ELEMTREE_XML, filepath);*/

	ProElempath contraints_elem_path;
	isis_ProElempathAlloc(&contraints_elem_path);
	isis_ProElempathDataSet(contraints_elem_path, constraints_path_items, sizeof(constraints_path_items)/sizeof(ProElempathItem));

	int count;
	isis_ProElementArrayCount(elemTree, contraints_elem_path, &count);
	//isis::CreoMessageDialog(std::to_string((long long)count));

	// Get constraint array
	ProElement* constraintarray;
	isis_ProArrayAlloc(count,sizeof(ProElement),1,(ProArray*) &constraintarray);
	isis_ProElementArrayGet(elemTree, contraints_elem_path, &constraintarray);

	// Iterate through constraints
	for (int i = 0; i < count; i++)
	{
			ProElempathItem set_path_items[] = {
				{PRO_ELEM_PATH_ITEM_TYPE_ID, PRO_E_DTMPLN_CONSTRAINTS},
				{PRO_ELEM_PATH_ITEM_TYPE_INDEX, i},
				{PRO_ELEM_PATH_ITEM_TYPE_ID, PRO_E_DTMPLN_CONSTR_REF},
			};

			ProElement constraint;
			ProReference ref;
			int setid;
			ProElempath elem_path;
			ProElempathAlloc(&elem_path);
			isis_ProElempathDataSet(elem_path, set_path_items, sizeof(set_path_items)/sizeof(ProElempathItem));
			isis_ProElemtreeElementGet(elemTree, elem_path, &constraint);
			ProError err = ProElementReferenceGet(constraint, 0, &ref);
			if (err == PRO_TK_EMPTY)
			{
				AddIfMissing(datum, result);
				continue;
			} else if (err != PRO_TK_NO_ERROR)
			{
				throw isis::application_exception("ProElemtreeElementGet error :" + std::to_string((long long)err));
			}

			ProMdl refowner;
			err = ProReferenceOwnerGet(ref, &refowner);
			if (err != PRO_TK_NO_ERROR)
			{
				throw isis::application_exception("ProReferenceOwnerGet error");
			}
			/*ProName name;
			err = ProMdlNameGet(refowner, name);
			if (err != PRO_TK_NO_ERROR)
			{
				throw isis::application_exception("ProMdlNameGet error");
			}*/
			ProMdlType mdltype;
			err = ProMdlTypeGet(refowner, &mdltype);
			if (err != PRO_TK_NO_ERROR)
			{
				throw isis::application_exception("ProMdlTypeGet error");
			}
			ProSelection selection;
			ProReferenceToSelection(ref, &selection);
			if (mdltype==PRO_MDL_PART)
			{
				//isis::CreoMessageDialog(name);
				AddIfMissing(selection, result);
			} else {
				Resolve(selection, result);
				//throw isis::application_exception("Datum is not derived from a part");
			}
			//result.push_back(refowner);
			//ProSelection
	}
}

void DatumRefResolver::AddIfMissing(ProSelection selection, vector<ProSelection> &result)
{
	ProAsmcomppath selpath;
	isis_ProSelectionAsmcomppathGet(selection, &selpath);

	ProModelitem selitem;
	isis_ProSelectionModelitemGet(selection, &selitem);

	for (vector<ProSelection>::const_iterator it = result.begin(); it != result.end(); ++it)
	{
		ProAsmcomppath path;
		isis_ProSelectionAsmcomppathGet(*it, &path);

		ProModelitem item;
		isis_ProSelectionModelitemGet(*it, &item);

		if (!Equals(path, selpath) || !Equals(selitem, item))
		{
			continue;
		} else {
			return;
		}
	}

	result.push_back(selection);
}

bool DatumRefResolver::Equals(ProAsmcomppath &path1, ProAsmcomppath &path2)
{
	if (path1.owner != path2.owner || path1.table_num != path2.table_num) return false;

	return memcmp(path1.comp_id_table, path2.comp_id_table, path1.table_num)==0;
}

bool DatumRefResolver::Equals(ProModelitem &item1, ProModelitem &item2)
{
	return item1.id==item2.id && item1.owner == item2.owner && item1.type == item2.type;
}

void DatumRefResolver::HandleDatumAxis(ProSelection datum, vector<ProSelection> &result)
{
	ProAsmcomppath comppath;
	isis_ProSelectionAsmcomppathGet(datum, &comppath);

	ProModelitem mitem;
	isis_ProSelectionModelitemGet(datum, &mitem);

	ProFeature* feat;
	ProFeature f;

	if (mitem.type == PRO_FEATURE)
	{
		feat = &mitem;
	} else if (mitem.type == PRO_SURFACE || mitem.type == PRO_AXIS || mitem.type == PRO_EDGE || mitem.type == PRO_CSYS)
	{
		isis_ProGeomitemFeatureGet ((ProGeomitem*)&mitem, &f);
		feat = &f;
	} else {
		throw isis::application_exception("DatumRefResolver::Resolve(): Unsupported datum type, only AXIS, PLANE and CSYS are supported. Datum type is:" + std::to_string((long long)mitem.type));
	}

	ProElement elemTree;
	isis_ProFeatureElemtreeExtract (feat, &comppath, PRO_FEAT_EXTRACT_NO_OPTS, &elemTree );

	ProElempathItem constraints_path_items[] = {
		{PRO_ELEM_PATH_ITEM_TYPE_ID, PRO_E_DTMAXIS_CONSTRAINTS},
	};

	/*wostringstream ostr;
	ostr << feat->id;
	ostr << L"elemtree.xml";
	ProPath filepath;
	wcscpy(filepath, ostr.str().c_str());
	isis_ProElemtreeWrite(elemTree, PRO_ELEMTREE_XML, filepath);*/

	ProElempath contraints_elem_path;
	isis_ProElempathAlloc(&contraints_elem_path);
	isis_ProElempathDataSet(contraints_elem_path, constraints_path_items, sizeof(constraints_path_items)/sizeof(ProElempathItem));

	int count;
	isis_ProElementArrayCount(elemTree, contraints_elem_path, &count);
	//isis::CreoMessageDialog(std::to_string((long long)count));


	// Iterate through constraints
	for (int i = 0; i < count; i++)
	{
			ProElempathItem set_path_items[] = {
				{PRO_ELEM_PATH_ITEM_TYPE_ID, PRO_E_DTMAXIS_CONSTRAINTS},
				{PRO_ELEM_PATH_ITEM_TYPE_INDEX, i},
				{PRO_ELEM_PATH_ITEM_TYPE_ID, PRO_E_DTMAXIS_CONSTR_REF},
			};

			ProElement constraint;
			ProReference ref;
			int setid;
			ProElempath elem_path;
			isis_ProElempathAlloc(&elem_path);
			isis_ProElempathDataSet(elem_path, set_path_items, sizeof(set_path_items)/sizeof(ProElempathItem));
			isis_ProElemtreeElementGet(elemTree, elem_path, &constraint);
			ProError err = ProElementReferenceGet(constraint, 0, &ref);
			if (err == PRO_TK_EMPTY)
			{
				AddIfMissing(datum, result);
				continue;
			} else if (err != PRO_TK_NO_ERROR)
			{
				throw isis::application_exception("ProElemtreeElementGet error :" + std::to_string((long long)err));
			}

			ProMdl refowner;
			err = ProReferenceOwnerGet(ref, &refowner);
			if (err != PRO_TK_NO_ERROR)
			{
				throw isis::application_exception("ProReferenceOwnerGet error");
			}
			/*ProName name;
			err = ProMdlNameGet(refowner, name);
			if (err != PRO_TK_NO_ERROR)
			{
				throw isis::application_exception("ProMdlNameGet error");
			}*/
			ProMdlType mdltype;
			err = ProMdlTypeGet(refowner, &mdltype);
			if (err != PRO_TK_NO_ERROR)
			{
				throw isis::application_exception("ProMdlTypeGet error");
			}
			ProSelection selection;
			ProReferenceToSelection(ref, &selection);
			if (mdltype==PRO_MDL_PART)
			{
				AddIfMissing(selection, result);
			} else {
				Resolve(selection, result);
//				throw isis::application_exception("Datum is not derived from a part");
			}
			//result.push_back(refowner);
			//ProSelection
	}
}

void DatumRefResolver::HandleDatumCSys(ProSelection datum, vector<ProSelection> &result)
{
	ProAsmcomppath comppath;
	isis_ProSelectionAsmcomppathGet(datum, &comppath);

	ProModelitem mitem;
	isis_ProSelectionModelitemGet(datum, &mitem);

	ProFeature* feat;
	ProFeature f;

	if (mitem.type == PRO_FEATURE)
	{
		feat = &mitem;
	} else if (mitem.type == PRO_SURFACE || mitem.type == PRO_AXIS || mitem.type == PRO_EDGE || mitem.type == PRO_CSYS)
	{
		isis_ProGeomitemFeatureGet ((ProGeomitem*)&mitem, &f);
		feat = &f;
	} else {
		throw isis::application_exception("DatumRefResolver::Resolve(): Unsupported datum type, only AXIS, PLANE and CSYS are supported. Datum type is:" + std::to_string((long long)mitem.type));
	}

	ProElement elemTree;
	isis_ProFeatureElemtreeExtract (feat, &comppath, PRO_FEAT_EXTRACT_NO_OPTS, &elemTree );

	/*wostringstream ostr;
	ostr << feat->id;
	ostr << L"elemtree.xml";
	ProPath filepath;
	wcscpy(filepath, ostr.str().c_str());
	isis_ProElemtreeWrite(elemTree, PRO_ELEMTREE_XML, filepath);*/

	{
		ProElempathItem constraints_path_items[] = {
			{PRO_ELEM_PATH_ITEM_TYPE_ID, PRO_E_CSYS_ORIGIN_CONSTRS},
		};

		ProElempath contraints_elem_path;
		isis_ProElempathAlloc(&contraints_elem_path);
		isis_ProElempathDataSet(contraints_elem_path, constraints_path_items, sizeof(constraints_path_items)/sizeof(ProElempathItem));

		int count;
		isis_ProElementArrayCount(elemTree, contraints_elem_path, &count);

		// Iterate through constraints
		for (int i = 0; i < count; i++)
		{
				ProElempathItem set_path_items[] = {
					{PRO_ELEM_PATH_ITEM_TYPE_ID, PRO_E_CSYS_ORIGIN_CONSTRS},
					{PRO_ELEM_PATH_ITEM_TYPE_INDEX, i},
					{PRO_ELEM_PATH_ITEM_TYPE_ID, PRO_E_CSYS_ORIGIN_CONSTR_REF},
				};

				ProElement constraint;
				ProReference ref;
				int setid;
				ProElempath elem_path;
				isis_ProElempathAlloc(&elem_path);
				isis_ProElempathDataSet(elem_path, set_path_items, sizeof(set_path_items)/sizeof(ProElempathItem));
				isis_ProElemtreeElementGet(elemTree, elem_path, &constraint);
				ProError err = ProElementReferenceGet(constraint, 0, &ref);
				if (err == PRO_TK_EMPTY)
				{
					AddIfMissing(datum, result);
					continue;
				} else if (err != PRO_TK_NO_ERROR)
				{
					throw isis::application_exception("ProElemtreeElementGet error :" + std::to_string((long long)err));
				}

				ProMdl refowner;
				err = ProReferenceOwnerGet(ref, &refowner);
				if (err != PRO_TK_NO_ERROR)
				{
					throw isis::application_exception("ProReferenceOwnerGet error");
				}

				ProMdlType mdltype;
				err = ProMdlTypeGet(refowner, &mdltype);
				if (err != PRO_TK_NO_ERROR)
				{
					throw isis::application_exception("ProMdlTypeGet error");
				}
				ProSelection selection;
				ProReferenceToSelection(ref, &selection);
				if (mdltype==PRO_MDL_PART)
				{
					AddIfMissing(selection, result);
				} else {
					Resolve(selection, result);
				}
		}
	}
	{
		ProElempathItem constraints_path_items[] = {
			{PRO_ELEM_PATH_ITEM_TYPE_ID, PRO_E_CSYS_DIM_CONSTRS},
		};

		ProElempath contraints_elem_path;
		isis_ProElempathAlloc(&contraints_elem_path);
		isis_ProElempathDataSet(contraints_elem_path, constraints_path_items, sizeof(constraints_path_items)/sizeof(ProElempathItem));

		int count;
		isis_ProElementArrayCount(elemTree, contraints_elem_path, &count);

		// Iterate through constraints
		for (int i = 0; i < count; i++)
		{
				ProElempathItem set_path_items[] = {
					{PRO_ELEM_PATH_ITEM_TYPE_ID, PRO_E_CSYS_DIM_CONSTRS},
					{PRO_ELEM_PATH_ITEM_TYPE_INDEX, i},
					{PRO_ELEM_PATH_ITEM_TYPE_ID, PRO_E_CSYS_DIM_CONSTR_REF},
				};

				ProElement constraint;
				ProReference ref;
				int setid;
				ProElempath elem_path;
				isis_ProElempathAlloc(&elem_path);
				isis_ProElempathDataSet(elem_path, set_path_items, sizeof(set_path_items)/sizeof(ProElempathItem));
				isis_ProElemtreeElementGet(elemTree, elem_path, &constraint);
				ProError err = ProElementReferenceGet(constraint, 0, &ref);
				if (err == PRO_TK_EMPTY)
				{
					AddIfMissing(datum, result);
					continue;
				} else if (err != PRO_TK_NO_ERROR)
				{
					throw isis::application_exception("ProElemtreeElementGet error :" + std::to_string((long long)err));
				}

				ProMdl refowner;
				err = ProReferenceOwnerGet(ref, &refowner);
				if (err != PRO_TK_NO_ERROR)
				{
					throw isis::application_exception("ProReferenceOwnerGet error");
				}

				ProMdlType mdltype;
				err = ProMdlTypeGet(refowner, &mdltype);
				if (err != PRO_TK_NO_ERROR)
				{
					throw isis::application_exception("ProMdlTypeGet error");
				}
				ProSelection selection;
				ProReferenceToSelection(ref, &selection);
				if (mdltype==PRO_MDL_PART)
				{
					AddIfMissing(selection, result);
				} else {
					Resolve(selection, result);
				}
		}
	}
}

void DatumRefResolver::HandleDatumPoint(ProSelection feat, vector<ProSelection> &result)
{
	throw isis::application_exception("Datum point system is not supported.");
}

void DatumRefResolver::Resolve(ProSelection datum, vector<ProSelection> &result)
{
	ProModelitem p_mdl_item;
	ProAsmcomppath p_cmp_path;
	isis_ProSelectionModelitemGet(datum, &p_mdl_item);

	// Check if the owner is already a part
	ProMdlType mdltype;
	isis_ProMdlTypeGet(p_mdl_item.owner, &mdltype);
	if (mdltype == PRO_PART)
	{
		AddIfMissing(datum, result);
	} else { // If the owner is not a part, investigate further
		ProFeature* feat;
		ProFeature f;
		if (p_mdl_item.type == PRO_FEATURE)
		{
			feat = &p_mdl_item;
		} else if (p_mdl_item.type == PRO_SURFACE || p_mdl_item.type == PRO_AXIS || p_mdl_item.type == PRO_EDGE || p_mdl_item.type == PRO_CSYS)
		{
			isis_ProGeomitemFeatureGet ((ProGeomitem*)&p_mdl_item, &f);
			feat = &f;
		} else {
			throw isis::application_exception("DatumRefResolver::Resolve(): Unsupported datum type, only AXIS, PLANE and CSYS are supported. Datum type is:" + std::to_string((long long)p_mdl_item.type));
		}

		ProElement* constraintarray;
		ProFeattype type;
		isis_ProFeatureTypeGet(feat, &type);

		//isis::CreoMessageDialog("Feat type: " + std::to_string((long long)type));

		if (type == PRO_FEAT_DATUM) // Plane
			HandleDatumPlane(datum, result);
		else if (type == PRO_FEAT_DATUM_AXIS)
			HandleDatumAxis(datum, result);
		else if (type == PRO_FEAT_CSYS)
			HandleDatumCSys(datum, result);
		else {
			throw isis::application_exception("DatumRefResolver::Resolve(): Unsupported datum type, only AXIS, PLANE and CSYS are supported. Datum type is:" + std::to_string((long long)type));
		}
	}
}

void DatumRefResolver::Resolve(vector<ProSelection> &result)
{
	Resolve(sel, result);

	return;
}

}