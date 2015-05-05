protocol version 0.0.1
RESULT
blastbox_1_sw0001_feat_1.cxx
wfcElements_ptr CreateFeature_1(pfcModel_ptr Model)
{
  wfcElements_ptr newElems = new wfcElements();
  wfcElement_ptr newElem;
  CreateElem localElem;
  otkxEnums localEnum;
  CollectionHelpers localCollnHelper;

  /*      Element id:    386 : PRO_E_FEATURE_TREE    */
  localElem.CreateCompoundElem (PRO_E_FEATURE_TREE, 0, newElems );

  /*      Element id:    387 : PRO_E_FEATURE_TYPE    */
  localElem.CreateIntegerElem (PRO_E_FEATURE_TYPE, 923, 1, newElems);

  /*      Element id:    1964 : PRO_E_STD_FEATURE_NAME    */
  localElem.CreateStringElem (PRO_E_STD_FEATURE_NAME, "RIGHT", 1, newElems );

  /*      Element id:    410 : PRO_E_DTMPLN_CONSTRAINTS    */
  localElem.CreateCompoundElem (PRO_E_DTMPLN_CONSTRAINTS, 1, newElems );

  /*      Element id:    411 : PRO_E_DTMPLN_CONSTRAINT    */
  localElem.CreateCompoundElem (PRO_E_DTMPLN_CONSTRAINT, 2, newElems );

  /*      Element id:    412 : PRO_E_DTMPLN_CONSTR_TYPE    */
  localElem.CreateIntegerElem (PRO_E_DTMPLN_CONSTR_TYPE, 7, 3, newElems);

  /*      Element id:    413 : PRO_E_DTMPLN_CONSTR_REF    */
  localElem.CreateCompoundElem (PRO_E_DTMPLN_CONSTR_REF, 3, newElems );

  /*      Element id:    414 : PRO_E_DTMPLN_CONSTR_REF_OFFSET    */
  localElem.CreateDoubleElem (PRO_E_DTMPLN_CONSTR_REF_OFFSET, 0, 3, newElems) ;


  /*      Element id:    415 : PRO_E_DTMPLN_CONSTR_REF_ANGLE    */
  localElem.CreateDoubleElem (PRO_E_DTMPLN_CONSTR_REF_ANGLE, 0, 3, newElems) ;

  /*      Element id:    416 : PRO_E_DTMPLN_SEC_IND    */
  localElem.CreateIntegerElem (PRO_E_DTMPLN_SEC_IND, 0, 3, newElems);

  /*      Element id:    417 : PRO_E_DTMPLN_OFF_CSYS    */
  localElem.CreateIntegerElem (PRO_E_DTMPLN_OFF_CSYS, 0, 3, newElems);

  /*      Element id:    418 : PRO_E_DTMPLN_OFF_CSYS_OFFSET    */
  localElem.CreateDoubleElem (PRO_E_DTMPLN_OFF_CSYS_OFFSET, 0, 3, newElems) ;

  /*      Element id:    3123 : PRO_E_DTMPLN_FLIP_DIR    */
  localElem.CreateIntegerElem (PRO_E_DTMPLN_FLIP_DIR, 0, 1, newElems);

  /*      Element id:    419 : PRO_E_DTMPLN_FIT    */
  localElem.CreateCompoundElem (PRO_E_DTMPLN_FIT, 1, newElems );

  /*      Element id:    420 : PRO_E_DTMPLN_FIT_TYPE    */
  localElem.CreateIntegerElem (PRO_E_DTMPLN_FIT_TYPE, -1, 2, newElems);

  /*      Element id:    421 : PRO_E_DTMPLN_FIT_REF    */
  localElem.CreateCompoundElem (PRO_E_DTMPLN_FIT_REF, 2, newElems );

  /*      Element id:    422 : PRO_E_DTMPLN_FIT_DTM_RAD    */
  localElem.CreateDoubleElem (PRO_E_DTMPLN_FIT_DTM_RAD, 0, 2, newElems) ;


  return (newElems);

}
blastbox_1_sw0001_feat_3.cxx
wfcElements_ptr CreateFeature_3(pfcModel_ptr Model)
{
  wfcElements_ptr newElems = new wfcElements();
  wfcElement_ptr newElem;
  CreateElem localElem;
  otkxEnums localEnum;
  CollectionHelpers localCollnHelper;

  /*      Element id:    386 : PRO_E_FEATURE_TREE    */
  localElem.CreateCompoundElem (PRO_E_FEATURE_TREE, 0, newElems );

  /*      Element id:    387 : PRO_E_FEATURE_TYPE    */
  localElem.CreateIntegerElem (PRO_E_FEATURE_TYPE, 923, 1, newElems);

  /*      Element id:    1964 : PRO_E_STD_FEATURE_NAME    */
  localElem.CreateStringElem (PRO_E_STD_FEATURE_NAME, "TOP", 1, newElems );

  /*      Element id:    410 : PRO_E_DTMPLN_CONSTRAINTS    */
  localElem.CreateCompoundElem (PRO_E_DTMPLN_CONSTRAINTS, 1, newElems );

  /*      Element id:    411 : PRO_E_DTMPLN_CONSTRAINT    */
  localElem.CreateCompoundElem (PRO_E_DTMPLN_CONSTRAINT, 2, newElems );

  /*      Element id:    412 : PRO_E_DTMPLN_CONSTR_TYPE    */
  localElem.CreateIntegerElem (PRO_E_DTMPLN_CONSTR_TYPE, 8, 3, newElems);

  /*      Element id:    413 : PRO_E_DTMPLN_CONSTR_REF    */
  localElem.CreateCompoundElem (PRO_E_DTMPLN_CONSTR_REF, 3, newElems );

  /*      Element id:    414 : PRO_E_DTMPLN_CONSTR_REF_OFFSET    */
  localElem.CreateDoubleElem (PRO_E_DTMPLN_CONSTR_REF_OFFSET, 0, 3, newElems) ;


  /*      Element id:    415 : PRO_E_DTMPLN_CONSTR_REF_ANGLE    */
  localElem.CreateDoubleElem (PRO_E_DTMPLN_CONSTR_REF_ANGLE, 0, 3, newElems) ;

  /*      Element id:    416 : PRO_E_DTMPLN_SEC_IND    */
  localElem.CreateIntegerElem (PRO_E_DTMPLN_SEC_IND, 0, 3, newElems);

  /*      Element id:    417 : PRO_E_DTMPLN_OFF_CSYS    */
  localElem.CreateIntegerElem (PRO_E_DTMPLN_OFF_CSYS, 0, 3, newElems);

  /*      Element id:    418 : PRO_E_DTMPLN_OFF_CSYS_OFFSET    */
  localElem.CreateDoubleElem (PRO_E_DTMPLN_OFF_CSYS_OFFSET, 0, 3, newElems) ;

  /*      Element id:    3123 : PRO_E_DTMPLN_FLIP_DIR    */
  localElem.CreateIntegerElem (PRO_E_DTMPLN_FLIP_DIR, 0, 1, newElems);

  /*      Element id:    419 : PRO_E_DTMPLN_FIT    */
  localElem.CreateCompoundElem (PRO_E_DTMPLN_FIT, 1, newElems );

  /*      Element id:    420 : PRO_E_DTMPLN_FIT_TYPE    */
  localElem.CreateIntegerElem (PRO_E_DTMPLN_FIT_TYPE, -1, 2, newElems);

  /*      Element id:    421 : PRO_E_DTMPLN_FIT_REF    */
  localElem.CreateCompoundElem (PRO_E_DTMPLN_FIT_REF, 2, newElems );

  /*      Element id:    422 : PRO_E_DTMPLN_FIT_DTM_RAD    */
  localElem.CreateDoubleElem (PRO_E_DTMPLN_FIT_DTM_RAD, 0, 2, newElems) ;


  return (newElems);

}
blastbox_1_sw0001_feat_5.cxx
wfcElements_ptr CreateFeature_5(pfcModel_ptr Model)
{
  wfcElements_ptr newElems = new wfcElements();
  wfcElement_ptr newElem;
  CreateElem localElem;
  otkxEnums localEnum;
  CollectionHelpers localCollnHelper;

  /*      Element id:    386 : PRO_E_FEATURE_TREE    */
  localElem.CreateCompoundElem (PRO_E_FEATURE_TREE, 0, newElems );

  /*      Element id:    387 : PRO_E_FEATURE_TYPE    */
  localElem.CreateIntegerElem (PRO_E_FEATURE_TYPE, 923, 1, newElems);

  /*      Element id:    1964 : PRO_E_STD_FEATURE_NAME    */
  localElem.CreateStringElem (PRO_E_STD_FEATURE_NAME, "FRONT", 1, newElems );

  /*      Element id:    410 : PRO_E_DTMPLN_CONSTRAINTS    */
  localElem.CreateCompoundElem (PRO_E_DTMPLN_CONSTRAINTS, 1, newElems );

  /*      Element id:    411 : PRO_E_DTMPLN_CONSTRAINT    */
  localElem.CreateCompoundElem (PRO_E_DTMPLN_CONSTRAINT, 2, newElems );

  /*      Element id:    412 : PRO_E_DTMPLN_CONSTR_TYPE    */
  localElem.CreateIntegerElem (PRO_E_DTMPLN_CONSTR_TYPE, 9, 3, newElems);

  /*      Element id:    413 : PRO_E_DTMPLN_CONSTR_REF    */
  localElem.CreateCompoundElem (PRO_E_DTMPLN_CONSTR_REF, 3, newElems );

  /*      Element id:    414 : PRO_E_DTMPLN_CONSTR_REF_OFFSET    */
  localElem.CreateDoubleElem (PRO_E_DTMPLN_CONSTR_REF_OFFSET, 0, 3, newElems) ;


  /*      Element id:    415 : PRO_E_DTMPLN_CONSTR_REF_ANGLE    */
  localElem.CreateDoubleElem (PRO_E_DTMPLN_CONSTR_REF_ANGLE, 0, 3, newElems) ;

  /*      Element id:    416 : PRO_E_DTMPLN_SEC_IND    */
  localElem.CreateIntegerElem (PRO_E_DTMPLN_SEC_IND, 0, 3, newElems);

  /*      Element id:    417 : PRO_E_DTMPLN_OFF_CSYS    */
  localElem.CreateIntegerElem (PRO_E_DTMPLN_OFF_CSYS, 0, 3, newElems);

  /*      Element id:    418 : PRO_E_DTMPLN_OFF_CSYS_OFFSET    */
  localElem.CreateDoubleElem (PRO_E_DTMPLN_OFF_CSYS_OFFSET, 0, 3, newElems) ;

  /*      Element id:    3123 : PRO_E_DTMPLN_FLIP_DIR    */
  localElem.CreateIntegerElem (PRO_E_DTMPLN_FLIP_DIR, 0, 1, newElems);

  /*      Element id:    419 : PRO_E_DTMPLN_FIT    */
  localElem.CreateCompoundElem (PRO_E_DTMPLN_FIT, 1, newElems );

  /*      Element id:    420 : PRO_E_DTMPLN_FIT_TYPE    */
  localElem.CreateIntegerElem (PRO_E_DTMPLN_FIT_TYPE, -1, 2, newElems);

  /*      Element id:    421 : PRO_E_DTMPLN_FIT_REF    */
  localElem.CreateCompoundElem (PRO_E_DTMPLN_FIT_REF, 2, newElems );

  /*      Element id:    422 : PRO_E_DTMPLN_FIT_DTM_RAD    */
  localElem.CreateDoubleElem (PRO_E_DTMPLN_FIT_DTM_RAD, 0, 2, newElems) ;


  return (newElems);

}
blastbox_1_sw0001_feat_7.cxx
wfcElements_ptr CreateFeature_7(pfcModel_ptr Model)
{
  wfcElements_ptr newElems = new wfcElements();
  wfcElement_ptr newElem;
  CreateElem localElem;
  otkxEnums localEnum;
  CollectionHelpers localCollnHelper;

  /*      Element id:    386 : PRO_E_FEATURE_TREE    */
  localElem.CreateCompoundElem (PRO_E_FEATURE_TREE, 0, newElems );

  /*      Element id:    387 : PRO_E_FEATURE_TYPE    */
  localElem.CreateIntegerElem (PRO_E_FEATURE_TYPE, 979, 1, newElems);

  /*      Element id:    1964 : PRO_E_STD_FEATURE_NAME    */
  localElem.CreateStringElem (PRO_E_STD_FEATURE_NAME, "PRT_CSYS_DEF", 1, newElem
s );

  /*      Element id:    2134 : PRO_E_CSYS_ORIGIN_CONSTRS    */
  localElem.CreateCompoundElem (PRO_E_CSYS_ORIGIN_CONSTRS, 1, newElems );

  /*      Element id:    2147 : PRO_E_CSYS_OFFSET_TYPE    */
  localElem.CreateIntegerElem (PRO_E_CSYS_OFFSET_TYPE, 0, 1, newElems);

  /*      Element id:    3953 : PRO_E_CSYS_ONSURF_TYPE    */
  localElem.CreateIntegerElem (PRO_E_CSYS_ONSURF_TYPE, 0, 1, newElems);

  /*      Element id:    3954 : PRO_E_CSYS_DIM_CONSTRS    */
  localElem.CreateCompoundElem (PRO_E_CSYS_DIM_CONSTRS, 1, newElems );

  /*      Element id:    2137 : PRO_E_CSYS_ORIENTMOVES    */
  localElem.CreateCompoundElem (PRO_E_CSYS_ORIENTMOVES, 1, newElems );

  /*      Element id:    3054 : PRO_E_CSYS_NORMAL_TO_SCREEN    */
  localElem.CreateIntegerElem (PRO_E_CSYS_NORMAL_TO_SCREEN, 0, 1, newElems);

  /*      Element id:    3050 : PRO_E_CSYS_ORIENT_BY_METHOD    */
  localElem.CreateIntegerElem (PRO_E_CSYS_ORIENT_BY_METHOD, 0, 1, newElems);

  /*      Element id:    2142 : PRO_E_CSYS_ORIENTSELAXIS1_REF    */
  localElem.CreateCompoundElem (PRO_E_CSYS_ORIENTSELAXIS1_REF, 1, newElems );

  /*      Element id:    3167 : PRO_E_CSYS_ORIENTSELAXIS1_REF_OPT    */
  localElem.CreateIntegerElem (PRO_E_CSYS_ORIENTSELAXIS1_REF_OPT, 0, 1, newElems
);

  /*      Element id:    2143 : PRO_E_CSYS_ORIENTSELAXIS1_OPT    */
  localElem.CreateIntegerElem (PRO_E_CSYS_ORIENTSELAXIS1_OPT, 0, 1, newElems);

  /*      Element id:    3052 : PRO_E_CSYS_ORIENTSELAXIS1_FLIP    */
  localElem.CreateIntegerElem (PRO_E_CSYS_ORIENTSELAXIS1_FLIP, 0, 1, newElems);


  /*      Element id:    2144 : PRO_E_CSYS_ORIENTSELAXIS2_REF    */
  localElem.CreateCompoundElem (PRO_E_CSYS_ORIENTSELAXIS2_REF, 1, newElems );

  /*      Element id:    3168 : PRO_E_CSYS_ORIENTSELAXIS2_REF_OPT    */
  localElem.CreateIntegerElem (PRO_E_CSYS_ORIENTSELAXIS2_REF_OPT, 0, 1, newElems
);

  /*      Element id:    2145 : PRO_E_CSYS_ORIENTSELAXIS2_OPT    */
  localElem.CreateIntegerElem (PRO_E_CSYS_ORIENTSELAXIS2_OPT, 0, 1, newElems);

  /*      Element id:    3053 : PRO_E_CSYS_ORIENTSELAXIS2_FLIP    */
  localElem.CreateIntegerElem (PRO_E_CSYS_ORIENTSELAXIS2_FLIP, 0, 1, newElems);


  /*      Element id:    4123 : PRO_E_CSYS_ORIENTSELAXIS2_ROT_OPT    */
  localElem.CreateIntegerElem (PRO_E_CSYS_ORIENTSELAXIS2_ROT_OPT, 0, 1, newElems
);

  /*      Element id:    4124 : PRO_E_CSYS_ORIENTSELAXIS2_ROT    */
  localElem.CreateDoubleElem (PRO_E_CSYS_ORIENTSELAXIS2_ROT, 0, 1, newElems) ;

  /*      Element id:    3051 : PRO_E_CSYS_TYPE_MECH    */
  localElem.CreateIntegerElem (PRO_E_CSYS_TYPE_MECH, 0, 1, newElems);

  /*      Element id:    3932 : PRO_E_CSYS_FOLLOW_SRF_OPT    */
  localElem.CreateIntegerElem (PRO_E_CSYS_FOLLOW_SRF_OPT, 0, 1, newElems);


  return (newElems);

}
blastbox_1_sw0001_feat_40.cxx
wfcElements_ptr CreateFeature_40(pfcModel_ptr Model)
{
  wfcElements_ptr newElems = new wfcElements();
  wfcElement_ptr newElem;
  CreateElem localElem;
  otkxEnums localEnum;
  CollectionHelpers localCollnHelper;

  /*      Element id:    386 : PRO_E_FEATURE_TREE    */
  localElem.CreateCompoundElem (PRO_E_FEATURE_TREE, 0, newElems );

  /*      Element id:    387 : PRO_E_FEATURE_TYPE    */
  localElem.CreateIntegerElem (PRO_E_FEATURE_TYPE, 1084, 1, newElems);

  /*      Element id:    3489 : PRO_E_CG_FEAT_SUB_TYPE    */
  localElem.CreateIntegerElem (PRO_E_CG_FEAT_SUB_TYPE, 0, 1, newElems);

  /*      Element id:    1964 : PRO_E_STD_FEATURE_NAME    */
  localElem.CreateStringElem (PRO_E_STD_FEATURE_NAME, "EXTERN_COPY_GEOM_ID_40",
1, newElems );

  /*      Element id:    3418 : PRO_E_CG_REFS_TYPE    */
  localElem.CreateIntegerElem (PRO_E_CG_REFS_TYPE, 0, 1, newElems);

  /*      Element id:    3405 : PRO_E_CG_LOCATION    */
  localElem.CreateCompoundElem (PRO_E_CG_LOCATION, 1, newElems );

  /*      Element id:    3389 : PRO_E_DSF_EXT_LOCAL_TYPE    */
  localElem.CreateIntegerElem (PRO_E_DSF_EXT_LOCAL_TYPE, 1, 2, newElems);

  /*      Element id:    3390 : PRO_E_DSF_SEL_REF_MDL    */
  localElem.CreateCompoundElem (PRO_E_DSF_SEL_REF_MDL, 2, newElems );

  /*      Element id:    3409 : PRO_E_CG_PLACEMENT    */
  localElem.CreateCompoundElem (PRO_E_CG_PLACEMENT, 2, newElems );

  /*      Element id:    3415 : PRO_E_CG_PLACE_TYPE    */
  localElem.CreateIntegerElem (PRO_E_CG_PLACE_TYPE, 1, 3, newElems);

  /*      Element id:    3408 : PRO_E_CG_CSYS_PLACE    */
  localElem.CreateCompoundElem (PRO_E_CG_CSYS_PLACE, 3, newElems );

  /*      Element id:    3406 : PRO_E_CG_PLC_LOCAL_CSYS    */
  localElem.CreateCompoundElem (PRO_E_CG_PLC_LOCAL_CSYS, 4, newElems );

  /*      Element id:    3407 : PRO_E_CG_PLC_EXT_CSYS    */
  localElem.CreateCompoundElem (PRO_E_CG_PLC_EXT_CSYS, 4, newElems );

  /*      Element id:    3413 : PRO_E_CG_PG_OR_REFS    */
  localElem.CreateCompoundElem (PRO_E_CG_PG_OR_REFS, 1, newElems );

  /*      Element id:    3410 : PRO_E_CG_PUBD_GEOM    */
  localElem.CreateCompoundElem (PRO_E_CG_PUBD_GEOM, 2, newElems );

  /*      Element id:    3412 : PRO_E_CG_REFS_COLL    */
  localElem.CreateCompoundElem (PRO_E_CG_REFS_COLL, 2, newElems );

  /*      Element id:    2213 : PRO_E_STD_SURF_COLLECTION_APPL    */
  /* *** SURFACE COLLECTION *** */
  localElem.CreateSurfaceCollectionElem(PRO_E_STD_SURF_COLLECTION_APPL, 3,  newE
lems);

  /*      Element id:    2124 : PRO_E_STD_CURVE_COLLECTION_APPL    */
  localElem.CreateCurveCollectionElem(PRO_E_STD_CURVE_COLLECTION_APPL, 3, 17, ne
wElems);

  /*      Element id:    3411 : PRO_E_CG_OBJS_COLL    */
  localElem.CreateCompoundElem (PRO_E_CG_OBJS_COLL, 3, newElems );

  /*      Element id:    3861 : PRO_E_SW_COLLECTION_TYPE    */
  localElem.CreateIntegerElem (PRO_E_SW_COLLECTION_TYPE, 0, 1, newElems);

  /*      Element id:    3448 : PRO_E_SW_OPTIONS    */
  localElem.CreateCompoundElem (PRO_E_SW_OPTIONS, 1, newElems );

  /*      Element id:    3449 : PRO_E_SW_QUALITY    */
  localElem.CreateIntegerElem (PRO_E_SW_QUALITY, 0, 2, newElems);

  /*      Element id:    3450 : PRO_E_SW_FILL_HOLES    */
  localElem.CreateIntegerElem (PRO_E_SW_FILL_HOLES, 0, 2, newElems);

  /*      Element id:    3451 : PRO_E_SW_COLLECT_QUILTS    */
  localElem.CreateIntegerElem (PRO_E_SW_COLLECT_QUILTS, 0, 2, newElems);

  /*      Element id:    3453 : PRO_E_SW_SKIP_SURF_SIZE    */
  localElem.CreateIntegerElem (PRO_E_SW_SKIP_SURF_SIZE, 0, 2, newElems);

  /*      Element id:    3454 : PRO_E_SW_COLLECT_ORDER    */
  localElem.CreateIntegerElem (PRO_E_SW_COLLECT_ORDER, 0, 2, newElems);

  /*      Element id:    3958 : PRO_E_SW_RES_GEOM_OPT    */
  localElem.CreateIntegerElem (PRO_E_SW_RES_GEOM_OPT, 0, 2, newElems);

  /*      Element id:    4268 : PRO_E_SW_FAILED_SLD_OPT    */
  localElem.CreateIntegerElem (PRO_E_SW_FAILED_SLD_OPT, 0, 2, newElems);

  /*      Element id:    3780 : PRO_E_SW_FILL_CNTRS_ARR    */
  localElem.CreateCompoundElem (PRO_E_SW_FILL_CNTRS_ARR, 2, newElems );

  /*      Element id:    4167 : PRO_E_SW_FILL_CNTRS_DISP_CRV    */
  localElem.CreateIntegerElem (PRO_E_SW_FILL_CNTRS_DISP_CRV, 0, 2, newElems);

  /*      Element id:    3455 : PRO_E_SW_COMP_SUBSET    */
  localElem.CreateCompoundElem (PRO_E_SW_COMP_SUBSET, 1, newElems );

  /*      Element id:    3459 : PRO_E_SW_REFS_COLL    */
  localElem.CreateCompoundElem (PRO_E_SW_REFS_COLL, 1, newElems );

  /*      Element id:    2213 : PRO_E_STD_SURF_COLLECTION_APPL    */
  /* *** SURFACE COLLECTION *** */
  localElem.CreateSurfaceCollectionElem(PRO_E_STD_SURF_COLLECTION_APPL, 2,  newE
lems);

  /*      Element id:    3728 : PRO_E_SW_EXCLUDE_SURF_COLL_APPL    */
  localElem.CreateCompoundElem (PRO_E_SW_EXCLUDE_SURF_COLL_APPL, 2, newElems );


  /*      Element id:    2124 : PRO_E_STD_CURVE_COLLECTION_APPL    */
  localElem.CreateCurveCollectionElem(PRO_E_STD_CURVE_COLLECTION_APPL, 2, 34, ne
wElems);

  /*      Element id:    3411 : PRO_E_CG_OBJS_COLL    */
  localElem.CreateCompoundElem (PRO_E_CG_OBJS_COLL, 2, newElems );

  /*      Element id:    3402 : PRO_E_DSF_PROPAGATE_ANNOTS    */
  localElem.CreateCompoundElem (PRO_E_DSF_PROPAGATE_ANNOTS, 1, newElems );

  /*      Element id:    3393 : PRO_E_DSF_ANNOT_CPY_ALL    */
  localElem.CreateIntegerElem (PRO_E_DSF_ANNOT_CPY_ALL, 0, 2, newElems);

  /*      Element id:    3395 : PRO_E_DSF_ANNOT_DEPEND_ALL    */
  localElem.CreateIntegerElem (PRO_E_DSF_ANNOT_DEPEND_ALL, 1, 2, newElems);

  /*      Element id:    3396 : PRO_E_DSF_ANNOT_AUTO_CPY_DTM    */
  localElem.CreateIntegerElem (PRO_E_DSF_ANNOT_AUTO_CPY_DTM, 0, 2, newElems);

  /*      Element id:    3401 : PRO_E_DSF_ANNOT_SELECTIONS    */
  localElem.CreateCompoundElem (PRO_E_DSF_ANNOT_SELECTIONS, 2, newElems );

  /*      Element id:    3414 : PRO_E_CG_SRFS_COPY    */
  localElem.CreateCompoundElem (PRO_E_CG_SRFS_COPY, 1, newElems );

  /*      Element id:    2214 : PRO_E_SRF_COPY_TYPE    */
  localElem.CreateIntegerElem (PRO_E_SRF_COPY_TYPE, 1, 2, newElems);

  /*      Element id:    106 : PRO_E_SRF_COPY_EXCL    */
  localElem.CreateCompoundElem (PRO_E_SRF_COPY_EXCL, 2, newElems );

  /*      Element id:    107 : PRO_E_SRF_COPY_FILL    */
  localElem.CreateCompoundElem (PRO_E_SRF_COPY_FILL, 2, newElems );

  /*      Element id:    2124 : PRO_E_STD_CURVE_COLLECTION_APPL    */
  localElem.CreateCurveCollectionElem(PRO_E_STD_CURVE_COLLECTION_APPL, 2, 45, ne
wElems);

  /*      Element id:    3404 : PRO_E_DSF_DTMS_FIT    */
  localElem.CreateCompoundElem (PRO_E_DSF_DTMS_FIT, 1, newElems );

  /*      Element id:    3392 : PRO_E_DSF_DEPENDENCY    */
  localElem.CreateIntegerElem (PRO_E_DSF_DEPENDENCY, 1, 1, newElems);

  /*      Element id:    4244 : PRO_E_CG_SOLIDIFY    */
  localElem.CreateIntegerElem (PRO_E_CG_SOLIDIFY, 0, 1, newElems);


  return (newElems);

}
blastbox_1_sw0001_feat_90.cxx
wfcElements_ptr CreateFeature_90(pfcModel_ptr Model)
{
  wfcElements_ptr newElems = new wfcElements();
  wfcElement_ptr newElem;
  CreateElem localElem;
  otkxEnums localEnum;
  CollectionHelpers localCollnHelper;

  /*      Element id:    386 : PRO_E_FEATURE_TREE    */
  localElem.CreateCompoundElem (PRO_E_FEATURE_TREE, 0, newElems );

  /*      Element id:    387 : PRO_E_FEATURE_TYPE    */
  localElem.CreateIntegerElem (PRO_E_FEATURE_TYPE, 917, 1, newElems);

  /*      Element id:    388 : PRO_E_FEATURE_FORM    */
  localElem.CreateIntegerElem (PRO_E_FEATURE_FORM, 512, 1, newElems);

  /*      Element id:    3023 : PRO_E_PATCH_QUILT    */
  localElem.CreateSelectionElem (PRO_E_PATCH_QUILT, 1, Model, 6, 89,  newElems );

  /*      Element id:    3024 : PRO_E_PATCH_TYPE    */
  localElem.CreateIntegerElem (PRO_E_PATCH_TYPE, 917, 1, newElems);

  /*      Element id:    3025 : PRO_E_PATCH_MATERIAL_SIDE    */
  localElem.CreateIntegerElem (PRO_E_PATCH_MATERIAL_SIDE, 1, 1, newElems);

  /*      Element id:    3330 : PRO_E_IS_SMT_CUT    */
  localElem.CreateIntegerElem (PRO_E_IS_SMT_CUT, 0, 1, newElems);

  /*      Element id:    3329 : PRO_E_SMT_CUT_NORMAL_DIR    */
  localElem.CreateIntegerElem (PRO_E_SMT_CUT_NORMAL_DIR, 0, 1, newElems);

  /*      Element id:    1964 : PRO_E_STD_FEATURE_NAME    */
  localElem.CreateStringElem (PRO_E_STD_FEATURE_NAME, "SOLIDIFY_1", 1, newElems);


  return (newElems);

}
blastbox_1_sw0001_feat_93.cxx
wfcElements_ptr CreateFeature_93(pfcModel_ptr Model)
{
  wfcElements_ptr newElems = new wfcElements();
  wfcElement_ptr newElem;
  CreateElem localElem;
  otkxEnums localEnum;
  CollectionHelpers localCollnHelper;

  /*      Element id:    386 : PRO_E_FEATURE_TREE    */
  localElem.CreateCompoundElem (PRO_E_FEATURE_TREE, 0, newElems );

  /*      Element id:    387 : PRO_E_FEATURE_TYPE    */
  localElem.CreateIntegerElem (PRO_E_FEATURE_TYPE, 923, 1, newElems);

  /*      Element id:    1964 : PRO_E_STD_FEATURE_NAME    */
  localElem.CreateStringElem (PRO_E_STD_FEATURE_NAME, "DTM1", 1, newElems );

  /*      Element id:    410 : PRO_E_DTMPLN_CONSTRAINTS    */
  localElem.CreateCompoundElem (PRO_E_DTMPLN_CONSTRAINTS, 1, newElems );

  /*      Element id:    411 : PRO_E_DTMPLN_CONSTRAINT    */
  localElem.CreateCompoundElem (PRO_E_DTMPLN_CONSTRAINT, 2, newElems );

  /*      Element id:    412 : PRO_E_DTMPLN_CONSTR_TYPE    */
  localElem.CreateIntegerElem (PRO_E_DTMPLN_CONSTR_TYPE, 3, 3, newElems);

  /*      Element id:    413 : PRO_E_DTMPLN_CONSTR_REF    */
  localElem.CreateSelectionElem (PRO_E_DTMPLN_CONSTR_REF, 3, Model, 1, 43,  newElems );

  /*      Element id:    414 : PRO_E_DTMPLN_CONSTR_REF_OFFSET    */
  localElem.CreateDoubleElem (PRO_E_DTMPLN_CONSTR_REF_OFFSET, -5, 3, newElems) ;


  /*      Element id:    415 : PRO_E_DTMPLN_CONSTR_REF_ANGLE    */
  localElem.CreateDoubleElem (PRO_E_DTMPLN_CONSTR_REF_ANGLE, 0, 3, newElems) ;

  /*      Element id:    416 : PRO_E_DTMPLN_SEC_IND    */
  localElem.CreateIntegerElem (PRO_E_DTMPLN_SEC_IND, 0, 3, newElems);

  /*      Element id:    417 : PRO_E_DTMPLN_OFF_CSYS    */
  localElem.CreateIntegerElem (PRO_E_DTMPLN_OFF_CSYS, 0, 3, newElems);

  /*      Element id:    418 : PRO_E_DTMPLN_OFF_CSYS_OFFSET    */
  localElem.CreateDoubleElem (PRO_E_DTMPLN_OFF_CSYS_OFFSET, 0, 3, newElems) ;

  /*      Element id:    3123 : PRO_E_DTMPLN_FLIP_DIR    */
  localElem.CreateIntegerElem (PRO_E_DTMPLN_FLIP_DIR, 0, 1, newElems);

  /*      Element id:    419 : PRO_E_DTMPLN_FIT    */
  localElem.CreateCompoundElem (PRO_E_DTMPLN_FIT, 1, newElems );

  /*      Element id:    420 : PRO_E_DTMPLN_FIT_TYPE    */
  localElem.CreateIntegerElem (PRO_E_DTMPLN_FIT_TYPE, -1, 2, newElems);

  /*      Element id:    421 : PRO_E_DTMPLN_FIT_REF    */
  localElem.CreateCompoundElem (PRO_E_DTMPLN_FIT_REF, 2, newElems );

  /*      Element id:    422 : PRO_E_DTMPLN_FIT_DTM_RAD    */
  localElem.CreateDoubleElem (PRO_E_DTMPLN_FIT_DTM_RAD, 0, 2, newElems) ;


  return (newElems);

}
blastbox_1_sw0001_feat_95.cxx
wfcElements_ptr CreateFeature_95(pfcModel_ptr Model)
{
  wfcElements_ptr newElems = new wfcElements();
  wfcElement_ptr newElem;
  CreateElem localElem;
  otkxEnums localEnum;
  CollectionHelpers localCollnHelper;

  /*      Element id:    386 : PRO_E_FEATURE_TREE    */
  localElem.CreateCompoundElem (PRO_E_FEATURE_TREE, 0, newElems );

  /*      Element id:    387 : PRO_E_FEATURE_TYPE    */
  localElem.CreateIntegerElem (PRO_E_FEATURE_TYPE, 916, 1, newElems);

  /*      Element id:    388 : PRO_E_FEATURE_FORM    */
  localElem.CreateIntegerElem (PRO_E_FEATURE_FORM, 512, 1, newElems);

  /*      Element id:    3023 : PRO_E_PATCH_QUILT    */
  localElem.CreateSelectionElem (PRO_E_PATCH_QUILT, 1, Model, 1, 94,  newElems );

  /*      Element id:    3024 : PRO_E_PATCH_TYPE    */
  localElem.CreateIntegerElem (PRO_E_PATCH_TYPE, 916, 1, newElems);

  /*      Element id:    3025 : PRO_E_PATCH_MATERIAL_SIDE    */
  localElem.CreateIntegerElem (PRO_E_PATCH_MATERIAL_SIDE, -1, 1, newElems);

  /*      Element id:    3330 : PRO_E_IS_SMT_CUT    */
  localElem.CreateIntegerElem (PRO_E_IS_SMT_CUT, 0, 1, newElems);

  /*      Element id:    3329 : PRO_E_SMT_CUT_NORMAL_DIR    */
  localElem.CreateIntegerElem (PRO_E_SMT_CUT_NORMAL_DIR, 0, 1, newElems);

  /*      Element id:    1964 : PRO_E_STD_FEATURE_NAME    */
  localElem.CreateStringElem (PRO_E_STD_FEATURE_NAME, "SOLIDIFY_2", 1, newElems);


  return (newElems);

}

