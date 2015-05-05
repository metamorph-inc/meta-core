///*
//	Copyright (c) 2013 PTC Inc. and/or Its Subsidiary Companies. All Rights Reserved.
//*/
//
//#include "creo_save_feature_cpp.h"
//#include <ProElemId.h>
//#include <pfcGlobal.h>
//#include <ProFeature.h>
//
//
//
//wfcCrvCollectionInstrAttributes_ptr CollectionHelpers::CreateCurveCollInstrAttrs (int Attr[], int size)
//{
//	wfcCrvCollectionInstrAttributes_ptr Attrs = new wfcCrvCollectionInstrAttributes();
//	for (int i=0;i<size;i++)
//	{
//		wfcCrvCollectionInstrAttribute_ptr CrvAttr = wfcCrvCollectionInstrAttribute::Create ();
//		CrvAttr->SetAttribute(wfcCurveCollectionInstrAttribute (Attr[i]));
//		Attrs->append (CrvAttr);
//	}
//
//	return (Attrs);
//}
//
//
//wfcCrvCollectionInstrAttributes_ptr CollectionHelpers::CreateCurveCollInstrAttrs_One (int Attr)
//{
//	wfcCrvCollectionInstrAttributes_ptr Attrs = new wfcCrvCollectionInstrAttributes();
//	wfcCrvCollectionInstrAttribute_ptr CrvAttr = wfcCrvCollectionInstrAttribute::Create ();
//	CrvAttr->SetAttribute(wfcCurveCollectionInstrAttribute (Attr));
//	Attrs->append (CrvAttr);
//
//	return (Attrs);
//}
//
//
//wfcCurveCollectionInstruction_ptr CollectionHelpers::CreateCurveInstruction (int Type, double value,
//						pfcSelections_ptr References, wfcCrvCollectionInstrAttributes_ptr Attrs)
//{
//	wfcCurveCollectionInstruction_ptr CrvInstr = wfcCurveCollectionInstruction::Create (wfcCurveCollectionInstrType (Type));
//	CrvInstr->SetValue (value);
//	CrvInstr->SetReferences (References);
//	CrvInstr->SetAttributes(Attrs);
//
//	return (CrvInstr);
//}
//
//pfcSelection_ptr CollectionHelpers::CreateSelection (pfcModel_ptr Model, int Type, int Id)
//{
//	pfcModelItem_ptr ModelItem = Model->GetItemById (pfcModelItemType(Type), Id);
//
//	pfcSelection_ptr Selection = pfcCreateModelItemSelection(ModelItem);
//
//	return (Selection);
//
//}
//
//void CollectionHelpers::CreateSelections (pfcModel_ptr Model, int Type, int Id, pfcSelections_ptr Sels)
//{
//	pfcModelItem_ptr ModelItem = Model->GetItemById (pfcModelItemType(Type), Id);
//
//	pfcSelection_ptr Selection = pfcCreateModelItemSelection(ModelItem);
//
//	Sels->append(Selection);
//}
//
//pfcSelections_ptr CollectionHelpers::CreateSelections_One (pfcModel_ptr Model, int Type, int Id)
//{
//	pfcSelections_ptr Sels = new pfcSelections();
//
//	pfcModelItem_ptr ModelItem = Model->GetItemById (pfcModelItemType(Type), Id);
//
//	pfcSelection_ptr Selection = pfcCreateModelItemSelection(ModelItem);
//
//	Sels->append(Selection);
//
//	return (Sels);
//}
//
//
//void CollectionHelpers::CreateCurveCollInstrs (wfcCurveCollectionInstruction_ptr CrvCollInstr, wfcCurveCollectionInstructions_ptr WCrvCollInstrs)
//{
//	WCrvCollInstrs->append(CrvCollInstr);
//}
//
//wfcSurfaceCollectionInstruction_ptr CollectionHelpers::CreateSurfCollInstr (wfcSurfaceCollectionInstrType Type, bool Include, wfcSurfaceCollectionReferences_ptr Refs)
//{
//	wfcSurfaceCollectionInstruction_ptr WSrfCollInstr = wfcSurfaceCollectionInstruction::Create(Type);
//	WSrfCollInstr->SetInclude(Include);
//	WSrfCollInstr->SetSrfCollectionReferences (Refs);
//
//	return (WSrfCollInstr);
//}
//
//wfcSurfaceCollectionReference_ptr CollectionHelpers::CreateSurfCollReference (wfcSurfaceCollectionRefType RefType, pfcModel_ptr Model, int Type, int Id)
//{
//	wfcSurfaceCollectionReference_ptr WSrfCollRef = wfcSurfaceCollectionReference::Create (RefType);
//	pfcSelection_ptr Sel = CreateSelection (Model, Type, Id);
//	WSrfCollRef->SetReference (Sel);
//	return (WSrfCollRef);
//}
//
//
//char* otkxEnums::wfcCurveCollectionInstrAttributeGet (int id)
//{
//	switch (id)
//	{
//		case 	wfcCURVCOLL_NO_ATTR	: return ((char*)"wfcCURVCOLL_NO_ATTR");
//		case 	wfcCURVCOLL_ALL	: return ((char*)"wfcCURVCOLL_ALL");
//		case 	wfcCURVCOLL_CONVEX	: return ((char*)"wfcCURVCOLL_CONVEX");
//		case 	wfcCURVCOLL_CONCAVE	: return ((char*)"wfcCURVCOLL_CONCAVE");
//		case 	wfcCURVCOLL_RESERVED_ATTR	: return ((char*)"wfcCURVCOLL_RESERVED_ATTR");
//	}
//
//	char *str = (char *) malloc(100);
//	sprintf (str, "%d", id);
//	
//	return (str);
//}
//
//
//char* otkxEnums::wfcCurveCollectionInstrTypeGet (int id)
//{
//	switch (id)
//	{
//		case 	wfcCURVCOLL_EMPTY_INSTR 	: return ((char*)"wfcCURVCOLL_EMPTY_INSTR");
//		case 	wfcCURVCOLL_ADD_ONE_INSTR	: return ((char*)"wfcCURVCOLL_ADD_ONE_INSTR");
//		case 	wfcCURVCOLL_TAN_INSTR	: return ((char*)"wfcCURVCOLL_TAN_INSTR");
//		case 	wfcCURVCOLL_CURVE_INSTR	: return ((char*)"wfcCURVCOLL_CURVE_INSTR");
//		case 	wfcCURVCOLL_SURF_INSTR	: return ((char*)"wfcCURVCOLL_SURF_INSTR");
//		case 	wfcCURVCOLL_BNDRY_INSTR	: return ((char*)"wfcCURVCOLL_BNDRY_INSTR");
//		case 	wfcCURVCOLL_LOG_OBJ_INSTR	: return ((char*)"wfcCURVCOLL_LOG_OBJ_INSTR");
//		case 	wfcCURVCOLL_PART_INSTR	: return ((char*)"wfcCURVCOLL_PART_INSTR");
//		case 	wfcCURVCOLL_FEATURE_INSTR	: return ((char*)"wfcCURVCOLL_FEATURE_INSTR");
//		case 	wfcCURVCOLL_FROM_TO_INSTR	: return ((char*)"wfcCURVCOLL_FROM_TO_INSTR");
//		case 	wfcCURVCOLL_EXCLUDE_ONE_INSTR	: return ((char*)"wfcCURVCOLL_EXCLUDE_ONE_INSTR");
//		case 	wfcCURVCOLL_TRIM_INSTR	: return ((char*)"wfcCURVCOLL_TRIM_INSTR");
//		case 	wfcCURVCOLL_EXTEND_INSTR	: return ((char*)"wfcCURVCOLL_EXTEND_INSTR");
//		case 	wfcCURVCOLL_START_PNT_INSTR	: return ((char*)"wfcCURVCOLL_START_PNT_INSTR");
//		case 	wfcCURVCOLL_ADD_TANGENT_INSTR	: return ((char*)"wfcCURVCOLL_ADD_TANGENT_INSTR");
//		case 	wfcCURVCOLL_ADD_POINT_INSTR	: return ((char*)"wfcCURVCOLL_ADD_POINT_INSTR");
//		case 	wfcCURVCOLL_OPEN_CLOSE_LOOP_INSTR	: return ((char*)"wfcCURVCOLL_OPEN_CLOSE_LOOP_INSTR");
//		case 	wfcCURVCOLL_QUERY_INSTR	: return ((char*)"wfcCURVCOLL_QUERY_INSTR");
//		case 	wfcCURVCOLL_RESERVED_INSTR	: return ((char*)"wfcCURVCOLL_RESERVED_INSTR");
//
//	}
//
//	char *str = (char *) malloc(100);
//	sprintf (str, "%d", id);
//	
//	return (str);
//}
//
//char* otkxEnums::wfcSurfaceCollectionRefTypeGet (int id)
//{
//	switch (id)
//	{
//		case	wfcSURFCOLL_REF_SINGLE	: return ((char*)"wfcSURFCOLL_REF_SINGLE");
//		case	wfcSURFCOLL_REF_SINGLE_EDGE	: return ((char*)"wfcSURFCOLL_REF_SINGLE_EDGE");
//		case	wfcSURFCOLL_REF_SEED 	: return ((char*)"wfcSURFCOLL_REF_SEED");
//		case	wfcSURFCOLL_REF_BND  	: return ((char*)"wfcSURFCOLL_REF_BND");
//		case	wfcSURFCOLL_REF_SEED_EDGE	: return ((char*)"wfcSURFCOLL_REF_SEED_EDGE");
//		case	wfcSURFCOLL_REF_NEIGHBOR 	: return ((char*)"wfcSURFCOLL_REF_NEIGHBOR");
//		case	wfcSURFCOLL_REF_NEIGHBOR_EDGE	: return ((char*)"wfcSURFCOLL_REF_NEIGHBOR_EDGE");
//		case	wfcSURFCOLL_REF_GENERIC	: return ((char*)"wfcSURFCOLL_REF_GENERIC");
//	}
//
//	char *str = (char *) malloc(100);
//	sprintf (str, "%d", id);
//	
//	return (str);
//}
//
//char* otkxEnums::wfcSurfaceCollectionInstrTypeGet (int id)
//{
//	switch (id)
//	{
//		case	wfcSURFCOLL_SINGLE_SURF	: return ((char*)"wfcSURFCOLL_SINGLE_SURF");
//		case	wfcSURFCOLL_SEED_N_BND	: return ((char*)"wfcSURFCOLL_SEED_N_BND");
//		case	wfcSURFCOLL_QUILT_SRFS	: return ((char*)"wfcSURFCOLL_QUILT_SRFS");
//		case	wfcSURFCOLL_ALL_SOLID_SRFS	: return ((char*)"wfcSURFCOLL_ALL_SOLID_SRFS");
//		case	wfcSURFCOLL_NEIGHBOR	: return ((char*)"wfcSURFCOLL_NEIGHBOR");
//		case	wfcSURFCOLL_NEIGHBOR_INC	: return ((char*)"wfcSURFCOLL_NEIGHBOR_INC");
//		case	wfcSURFCOLL_ALL_QUILT_SRFS	: return ((char*)"wfcSURFCOLL_ALL_QUILT_SRFS");
//		case	wfcSURFCOLL_ALL_MODEL_SRFS	: return ((char*)"wfcSURFCOLL_ALL_MODEL_SRFS");
//		case	wfcSURFCOLL_LOGOBJ_SRFS	: return ((char*)"wfcSURFCOLL_LOGOBJ_SRFS");
//		case	wfcSURFCOLL_DTM_PLN	: return ((char*)"wfcSURFCOLL_DTM_PLN");
//		case	wfcSURFCOLL_DISALLOW_QLT	: return ((char*)"wfcSURFCOLL_DISALLOW_QLT");
//		case	wfcSURFCOLL_DISALLOW_SLD	: return ((char*)"wfcSURFCOLL_DISALLOW_SLD");
//		case	wfcSURFCOLL_DONT_MIX	: return ((char*)"wfcSURFCOLL_DONT_MIX");
//		case	wfcSURFCOLL_SAME_SRF_LST	: return ((char*)"wfcSURFCOLL_SAME_SRF_LST");
//		case	wfcSURFCOLL_USE_BACKUP	: return ((char*)"wfcSURFCOLL_USE_BACKUP");
//		case	wfcSURFCOLL_DONT_BACKUP	: return ((char*)"wfcSURFCOLL_DONT_BACKUP");
//		case	wfcSURFCOLL_DISALLOW_LOBJ	: return ((char*)"wfcSURFCOLL_DISALLOW_LOBJ");
//		case	wfcSURFCOLL_ALLOW_DTM_PLN	: return ((char*)"wfcSURFCOLL_ALLOW_DTM_PLN");
//		case	wfcSURFCOLL_SEED_N_BND_INC_BND	: return ((char*)"wfcSURFCOLL_SEED_N_BND_INC_BND");
//	}
//
//	char *str = (char *) malloc(100);
//	sprintf (str, "%d", id);
//	
//	return (str);
//}
//
