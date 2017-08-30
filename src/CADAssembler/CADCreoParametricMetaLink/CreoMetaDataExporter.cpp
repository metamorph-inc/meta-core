#include "stdafx.h"
#include "CreoMetaDataExporter.h"
#include "CreoMetaData.h"
#include "ProSolid.h"
#include "cc_CommonStructures.h"
#include "CreoPlugins\CreoPluginFunctions.h"
#include <ostream>
#include <iomanip>

static ProError FeatVisitId(ProFeature* p_feature, ProError status, ProAppData app_data)
{
    FeatVisitID* fid = (FeatVisitID*)app_data;
    ErrorDialogStr(std::to_string((long long)p_feature->id) + " " + std::to_string((long long)fid->id));
    if(p_feature->id==fid->id)
    {
        fid->result = p_feature;
    }
    return PRO_TK_NO_ERROR;
}

static ProFeature* GetFeatureFromID(ProSolid mdl, int id)
{
    FeatVisitID fid;
    fid.id = id;
    fid.result = 0;
    ProSolidFeatVisit(mdl, FeatVisitId, NULL, &fid);
    return fid.result;
}

static std::string GetFeatureName(ProFeature* feature)
{
    ProElempath path;
    ProValue value;
    ProValueData  value_data;
    ProElempathItem constr_type[3];

    constr_type[0].type = PRO_ELEM_PATH_ITEM_TYPE_ID;
    constr_type[0].path_item.elem_id = PRO_E_STD_FEATURE_NAME;

    ProElempathAlloc(&path);
    ProElempathDataSet(path, constr_type, 1);
    ProError error = ProFeatureElemValueGet(feature, path, &value);
    wstring wname;
    ostringstream ostream;
    ProValueDataGet(value, &value_data);
    wname = wstring(value_data.v.w);
    return string(wname.begin(),wname.end());
}

CreoMetaDataExporter::CreoMetaDataExporter(ProMdl assembly)
{
    this->assembly = assembly;
}

ProReference GetReference(ProElempathItem *path, ProElement elemTree)
{
    ProElement constr1;
    ProReference ref1;
    ProElempath elem_path;
    ProElempathAlloc(&elem_path);
    ProError err = ProElempathDataSet(elem_path, path, 3);
    if(err != PRO_TK_NO_ERROR)
    {
        ErrorDialogStr("Error in ProElempathDataSet:" + std::to_string((long long)err));
    }
    err = ProElemtreeElementGet(elemTree, elem_path, &constr1);
    if(err != PRO_TK_NO_ERROR)
    {
        ErrorDialogStr("Error in ProElemtreeElementGet:" + std::to_string((long long)err));
    }
    err = ProElementReferenceGet(constr1, 0, &ref1);
    if(err != PRO_TK_NO_ERROR)
    {
        ErrorDialogStr("Error in ProElementReferenceGet:" + std::to_string((long long)err));
    }
    ProElempathFree(&elem_path);
    return ref1;
}

void GetModelItemFromRef(ProReference ref, ProModelitem *item)
{
    ProMdl owner;
    int id;
    ProType type;
    ProReferenceOwnerGet(ref, &owner);
    ProReferenceIdGet(ref, &id);
    ProReferenceTypeGet(ref, &type);
    ProModelitemInit(owner, id, type, item);
}

void CreoMetaDataExporter::ProcessComponent(ProFeature* comp, ProAsmcomppath *comp_path, vector<ConstraintData>& cdatav)
{
    ProElement elemTree;
    ProFeatureElemtreeExtract(comp, comp_path, PRO_FEAT_EXTRACT_NO_OPTS, &elemTree);

    // Get constraint array
    ProElement* constraintarray;

    ProElempathItem angle_path_items[] =
    {
        {PRO_ELEM_PATH_ITEM_TYPE_ID, PRO_E_COMPONENT_CONSTRAINTS},
    };

    ProElempath contraints_elem_path;
    ProElempathAlloc(&contraints_elem_path);
    ProElempathDataSet(contraints_elem_path, angle_path_items, sizeof(angle_path_items)/sizeof(ProElempathItem));

    int count;
    ProElementArrayCount(elemTree, contraints_elem_path, &count);

    ProArrayAlloc(count,sizeof(ProElement),1,(ProArray*) &constraintarray);

    ProElementArrayGet(elemTree, contraints_elem_path, &constraintarray);

    for(int i = 0; i < count; i++)
    {
        ProElempathItem ref1_path_items[] =
        {
            {PRO_ELEM_PATH_ITEM_TYPE_ID, PRO_E_COMPONENT_CONSTRAINTS},
            {PRO_ELEM_PATH_ITEM_TYPE_INDEX, i},
            {PRO_ELEM_PATH_ITEM_TYPE_ID, PRO_E_COMPONENT_COMP_CONSTR_REF},
        };

        ProReference ref1 = GetReference(ref1_path_items, elemTree);

        ProElempathItem ref2_path_items[] =
        {
            {PRO_ELEM_PATH_ITEM_TYPE_ID, PRO_E_COMPONENT_CONSTRAINTS},
            {PRO_ELEM_PATH_ITEM_TYPE_INDEX, i},
            {PRO_ELEM_PATH_ITEM_TYPE_ID, PRO_E_COMPONENT_ASSEM_CONSTR_REF},
        };

        ProReference ref2 = GetReference(ref2_path_items, elemTree);


        ConstraintData cdata;

        ProModelitem item;
        GetModelItemFromRef(ref1, &item);
        ProName name;
        ProModelitemNameGet(&item, name);
        cdata.dname1 = getStrFromWstr(name);

        GetModelItemFromRef(ref2, &item);
        ProModelitemNameGet(&item, name);
        cdata.dname2 = getStrFromWstr(name);

        ProAsmcomppath path;
        ProReferenceAsmcomppathGet(ref1, &path);
        path.table_num--;
        const isis::CADComponentData *cdata1 = isis::GlobalModelData::Instance.GetComponentDataFromModel(path);
        if(cdata1 != 0)
        {
            cdata.compid1 = cdata1->componentID;
        }

        ProReferenceAsmcomppathGet(ref2, &path);
        path.table_num--;
        const isis::CADComponentData *cdata2 = isis::GlobalModelData::Instance.GetComponentDataFromModel(path);
        if(cdata2 != 0)
        {
            cdata.compid2 = cdata2->componentID;
        }

        ProElempathItem attr_path_items[] =
        {
            {PRO_ELEM_PATH_ITEM_TYPE_ID, PRO_E_COMPONENT_CONSTRAINTS},
            {PRO_ELEM_PATH_ITEM_TYPE_INDEX, i},
            {PRO_ELEM_PATH_ITEM_TYPE_ID, PRO_E_COMPONENT_CONSTR_ATTR},
        };

        ProElement attr;
        ProElempath elem_path;
        ProElempathAlloc(&elem_path);
        ProError err = ProElempathDataSet(elem_path, attr_path_items, 3);
        if(err != PRO_TK_NO_ERROR)
        {
            ErrorDialogStr("Error in ProElempathDataSet:" + std::to_string((long long)err));
        }
        err = ProElemtreeElementGet(elemTree, elem_path, &attr);
        if(err != PRO_TK_NO_ERROR)
        {
            ErrorDialogStr("Error in ProElemtreeElementGet:" + std::to_string((long long)err));
        }
        err = ProElementIntegerGet(attr, 0, &cdata.attr);
        if(err != PRO_TK_NO_ERROR)
        {
            ErrorDialogStr("Error in ProElementReferenceGet:" + std::to_string((long long)err));
        }
        ProElempathFree(&elem_path);

        /*		ProMdl owner;
        	int id;
        	ProReferenceOriginalidGet(ref1, &id);
        	//ProReferenceOriginalownerGet(ref1, &owner);
        	ErrorDialogStr(std::to_string((long long)id));
        	ProFeature *feature = GetFeatureFromID(ProMdlToSolid(cdata1->modelHandle), id);*/
        /*if (feature != 0)
        	cdata.dname1 = GetFeatureName(feature);

        ProReferenceIdGet(ref2, &id);
        ProReferenceOwnerGet(ref2, &owner);
        feature = GetFeatureFromID(ProMdlToSolid(owner), id);
        if (feature != 0)
        	cdata.dname2 = GetFeatureName(feature);*/

        cdatav.push_back(cdata);
    }
}

void CreoMetaDataExporter::Export(std::string filename)
{
    Udm::SmartDataNetwork dn_CFDParameters(CreoMetaData::diagram);

    dn_CFDParameters.CreateNew(filename, "CreoMetaData", CreoMetaData::MetaData::meta);
    CreoMetaData::MetaData root = CreoMetaData::MetaData::Cast(dn_CFDParameters.GetRootObject());

    for(map<std::string, isis::CADComponentData>::const_iterator it = isis::GlobalModelData::Instance.CadComponentData.begin(); it != isis::GlobalModelData::Instance.CadComponentData.end(); ++it)
    {
        ProMatrix m;

        ProIdTable c_id_table_added_model;
        int added_model_table_size;
        isis::Populate_c_id_table(it->second.componentPaths, c_id_table_added_model, added_model_table_size);

        ProAsmcomppath	comp_path;
        isis::isis_ProAsmcomppathInit((ProSolid)assembly,	//ProSolid   p_solid_handle
                                      c_id_table_added_model,			// ProIdTable
                                      added_model_table_size,			// table_size
                                      &comp_path);					// ProAsmcomppath *p_handle

        ProAsmcomppathTrfGet(&comp_path, PRO_B_TRUE, m);

        //ErrorDialogStr(it->second.avmComponentId);

        vector<ConstraintData> cdatav;

        ProcessComponent((ProFeature*)&(it->second.assembledFeature), &comp_path, cdatav);

        for each(ConstraintData d in cdatav)
        {
            CreoMetaData::ConstraintData constraint = CreoMetaData::ConstraintData::Create(root);
            constraint.DatumName1() = d.dname1;
            constraint.DatumName2() = d.dname2;
            constraint.InstanceGUID1() = d.compid1;
            constraint.InstanceGUID2() = d.compid2;
            constraint.Flags() = d.attr;
        }

        CreoMetaData::InitialPosition initpos = CreoMetaData::InitialPosition::Create(root);

        initpos.GUID() = it->first;

        ostringstream str;
        for(int i = 0; i < 4; i++)
        {
            for(int j = 0; j < 4; j++)
            {
                str << setprecision(17) << m[i][j];
                if(j != 3)
                {
                    str << " ";
                }
            }
            str << " ";
        }

        initpos.matrix() = str.str();


    }

    if(dn_CFDParameters.isOpen())
    {
        dn_CFDParameters.CloseWithUpdate();
    }

}

