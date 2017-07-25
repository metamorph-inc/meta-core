
#include "FeatureFactory.h"

#include <CommonDefinitions.h>
#include "LoggerBoost.h"

#include "Selector.h"

#include <ProElemId.h>
#include <ProFeatType.h>
#include <ProFeatForm.h>
#include <ProExtrude.h>
#include <ProValue.h>
#include <ProSurface.h>
#include <ProQuilt.h>
#include <ProDtmPln.h>
#include <ProDtmPnt.h>
#include <ProPoint.h>
#include <ProDtmAxis.h>
#include <ProSolidify.h>
#include <ProSolid.h>
#include <ProUtil.h>
#include <UtilTree.h>
#include <ProFeature.h>
#include <UtilPtc.h>
#include <ProModelitem.h>
#include <ProDtmCsys.h>
#include <sstream>

#define PRO_VALUE_TYPE_NIL static_cast<ProValueDataType>(-1)

namespace isis {
namespace creo { 
namespace feature { 
namespace create {


ProError make_feature( ProFeature& out_feature, ProSelection in_selection, ElemTreeData* in_datum_array, int in_datum_array_size ) {
	ProError status;
	
	
	
	ProElement datum_element_tree;
	switch( status = ProUtilElemtreeCreate( in_datum_array, in_datum_array_size, 
		 NULL, &datum_element_tree) ) {
	case PRO_TK_NO_ERROR: break;
	default:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "could not create  datum element tree";
		return PRO_TK_GENERAL_ERROR;
	}
#if DEBUG
	switch( status = ProUtilElemtreePrint(datum_element_tree, PRO_TEST_INFO_WINDOW, NULL) ) {
	case PRO_TK_NO_ERROR: break;
	default:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "could not print datum element tree " << status;
		return PRO_TK_GENERAL_ERROR;
	}
#endif
	ProFeatureCreateOptions* datum_opts;
	{
		const int num_datum_opts = 1;
		ProArray* datum_opts_array = (ProArray*)(&datum_opts);
		switch( ProArrayAlloc(0, sizeof(ProFeatureCreateOptions), num_datum_opts, datum_opts_array) ) {
			case PRO_TK_NO_ERROR: break;
			default: return PRO_TK_GENERAL_ERROR;
		}
		ProFeatureCreateOptions opts[2];
		switch( ProArrayObjectAdd(datum_opts_array, PRO_VALUE_UNUSED, 1, static_cast<void*>(&(opts[0]))) ) {
			case PRO_TK_NO_ERROR: break;
			default: return PRO_TK_GENERAL_ERROR;
		}
	}
	int datum_flags = PRO_REGEN_FORCE_REGEN;

	ProErrorlist datum_error_list;
	std::ostringstream out_str;
	switch( status = ProFeatureWithoptionsCreate(in_selection, 
		datum_element_tree, datum_opts, datum_flags, 
		&out_feature, &datum_error_list) ) 
	{
	case PRO_TK_NO_ERROR: break;
	case PRO_TK_BAD_INPUTS:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "datum failed: One or more of the input arguments are invalid.";
		return PRO_TK_GENERAL_ERROR;
	case PRO_TK_GENERAL_ERROR:
		// ProUtilElemtreePrint(out_str, tree);
		ProUtilFeatErrsSerialize(out_str, &datum_error_list);
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "the feature was not created: " << out_str.str();
		return PRO_TK_GENERAL_ERROR;
	default:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "failed to make datum feature: " << status;
		return PRO_TK_GENERAL_ERROR;
	}

	switch( ProArrayFree( (ProArray*)(&datum_opts) ) ) {
	case PRO_TK_NO_ERROR: break;
	default:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "failed to free options";
		return PRO_TK_GENERAL_ERROR;
	}
	return PRO_TK_NO_ERROR;
}

/**
FEATURE_TREE   (COMPOUND) 
 |---FEATURE_TYPE  VALUE (INT)       = 979 [PRO_FEAT_CSYS]
 |---STD_FEATURE_NAME  VALUE (WSTRING)   = ACS0
 |---CSYS_ORIGIN_CONSTRS   (ARRAY) 
 |---CSYS_OFFSET_TYPE  VALUE (INT)       = 0 [PRO_CSYS_OFFSET_CARTESIAN]
 |---CSYS_ONSURF_TYPE  VALUE (INT)       = 0
 |---CSYS_DIM_CONSTRS   (ARRAY) 
 |---CSYS_ORIENTMOVES   (ARRAY) 
 |---CSYS_NORMAL_TO_SCREEN  VALUE (INT)       = 0
 |---CSYS_ORIENT_BY_METHOD  VALUE (INT)       = 0 [PRO_CSYS_ORIENT_BY_SEL_REFS]
 |---CSYS_ORIENTSELAXIS1_REF  VALUE (SELECTION) = 
 |---CSYS_ORIENTSELAXIS1_REF_OPT  VALUE (INT)       = 0
 |---CSYS_ORIENTSELAXIS1_OPT  VALUE (INT)       = 0
 |---CSYS_ORIENTSELAXIS1_FLIP  VALUE (INT)       = 0
 |---CSYS_ORIENTSELAXIS2_REF  VALUE (SELECTION) = 
 |---CSYS_ORIENTSELAXIS2_REF_OPT  VALUE (INT)       = 0
 |---CSYS_ORIENTSELAXIS2_OPT  VALUE (INT)       = 0
 |---CSYS_ORIENTSELAXIS2_FLIP  VALUE (INT)       = 0
 |---CSYS_ORIENTSELAXIS2_ROT_OPT  VALUE (INT)       = 0
 |---CSYS_ORIENTSELAXIS2_ROT  VALUE (DOUBLE)    = 0.000000
 |---CSYS_TYPE_MECH  VALUE (INT)       = 0
 |---CSYS_FOLLOW_SRF_OPT  VALUE (INT)       = 0
*/
ProError Csys_default_fail( ProFeature& out_feature, ProMdl in_model, ProSelection in_selection, 
	std::string in_name ) 
{
	
	

	ProName datum_name;
	ProError status;

	ProValueData datum_name_data;
	datum_name_data.type = PRO_VALUE_TYPE_WSTRING;
	datum_name_data.v.w = ProStringToWstring( datum_name, const_cast<char*>(in_name.c_str()));

	ProValueData zero_value_data;
	zero_value_data.type = PRO_VALUE_TYPE_DOUBLE;
	zero_value_data.v.d = 0.0;

	ProValueData nil_selection_data;
	nil_selection_data.type = PRO_VALUE_TYPE_SELECTION;
	nil_selection_data.v.w = NULL;

	ElemTreeData datum_array[] = {
		{0, PRO_E_FEATURE_TREE, {PRO_VALUE_TYPE_NIL}},
		{1, PRO_E_FEATURE_TYPE, {PRO_VALUE_TYPE_INT, {PRO_FEAT_CSYS}}},
		{1, PRO_E_STD_FEATURE_NAME, datum_name_data},
		{1, PRO_E_CSYS_METHOD, {PRO_VALUE_TYPE_INT, {PRO_CSYS_DEFAULT}}},
	};
	 
	switch( status = make_feature( out_feature, in_selection, datum_array, sizeof(datum_array)/sizeof(ElemTreeData ) )) {
	case PRO_TK_NO_ERROR: break;
	default:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "could not create default csys feature " << status;
	}
	return PRO_TK_NO_ERROR;
}

ProError Csys_default( ProFeature& out_feature, ProMdl in_model, ProSelection in_selection, 
	std::string in_name ) 
{ 
	
	

	ProError err;
	ProElement rootElement;
	err = ProElementAlloc(PRO_E_FEATURE_TREE, &rootElement);
	/*---------------------------------------------------------------*\
	Populating element PRO_E_STD_FEATURE_NAME
	\*---------------------------------------------------------------*/
	ProElement featNameElement;
	err = ProElementAlloc(PRO_E_STD_FEATURE_NAME, &featNameElement);
	ProName datum_name;
	ProStringToWstring( datum_name, const_cast<char*>(in_name.c_str()));
	err = ProElementWstringSet(featNameElement, datum_name);
	err = ProElemtreeElementAdd(rootElement, NULL, featNameElement);
	/*---------------------------------------------------------------*\
	Populating element PRO_E_FEATURE_TYPE
	\*---------------------------------------------------------------*/
	ProElement feattypeElement;
	err = ProElementAlloc(PRO_E_FEATURE_TYPE, &feattypeElement);
	err = ProElementIntegerSet(feattypeElement, PRO_FEAT_CSYS);
	err = ProElemtreeElementAdd(rootElement, NULL, feattypeElement);
	/*---------------------------------------------------------------*\
	Populating element PRO_E_FEATURE_TYPE
	\*---------------------------------------------------------------*/
	ProElement csysmethodElement;
	err = ProElementAlloc(PRO_E_CSYS_METHOD, &csysmethodElement);
	err = ProElementIntegerSet(csysmethodElement, PRO_CSYS_DEFAULT);
	err = ProElemtreeElementAdd(rootElement, NULL, csysmethodElement);
 
	ProModelitem modelitem;
	err = ProMdlToModelitem(in_model, &modelitem);
	ProSelection mdl_sel;
	if ( err == PRO_TK_NO_ERROR )
	err = ProSelectionAlloc(NULL, &modelitem, &mdl_sel);
 
	ProFeatureCreateOptions *opts = NULL;;
	err = ProArrayAlloc(1, sizeof(ProFeatureCreateOptions), 1, (ProArray *)&opts);
 
	opts[0] = PRO_FEAT_CR_DEFINE_MISS_ELEMS;
	ProErrorlist errors;
	if ( err == PRO_TK_NO_ERROR )
	err = ProFeatureCreate(mdl_sel, rootElement, opts, 1, &out_feature, &errors);
	if( rootElement != NULL )
	ProElementFree( &rootElement);
 
	return (PRO_TK_NO_ERROR);
}

/*
FEATURE_TREE   (COMPOUND) 
 |---FEATURE_TYPE  VALUE (INT)       = 931  [PRO_FEAT_DATUM_POINT]
 |---DPOINT_TYPE  VALUE (INT)       = 6  [PRO_DPOINT_TYPE_OFFSET_CSYS]
 |---STD_FEATURE_NAME  VALUE (WSTRING)   = PNT0
 |---DPOINT_OFST_CSYS_TYPE  VALUE (INT)       = 0 [PRO_DTMPNT_OFFCSYS_CARTESIAN]
 |---DPOINT_OFST_CSYS_REF  VALUE (SELECTION)   id = 239
 |---DPOINT_OFST_CSYS_WITH_DIMS  VALUE (INT)       = 1 [PRO_B_TRUE]
 |---DPOINT_OFST_CSYS_PNTS_ARRAY   (ARRAY) 
      |---DPOINT_OFST_CSYS_PNT   (COMPOUND) 
           |---DPOINT_OFST_CSYS_PNT_NAME  VALUE (WSTRING)   = PNT0
           |---DPOINT_OFST_CSYS_DIR1_VAL  VALUE (DOUBLE)    = 10.000000
           |---DPOINT_OFST_CSYS_DIR2_VAL  VALUE (DOUBLE)    = 20.000000
           |---DPOINT_OFST_CSYS_DIR3_VAL  VALUE (DOUBLE)    = 30.000000
*/
ProError Point( ProFeature& out_feature, ProMdl in_model, ProSelection in_selection, 
	std::string in_name,  ProFeature in_csys, Pro3dPnt in_coords ) 
{
	
	

	ProError status;
	Selector default_csys_selector( in_model, in_csys, PRO_CSYS );

	ProName datum_name;

	ProValueData datum_name_data;
	datum_name_data.type = PRO_VALUE_TYPE_WSTRING;
	datum_name_data.v.w = ProStringToWstring( datum_name, const_cast<char*>(in_name.c_str()));

	ProName pt_name;
	ProValueData pt_name_data;
	pt_name_data.type = PRO_VALUE_TYPE_WSTRING;
	pt_name_data.v.w = ProStringToWstring( pt_name, const_cast<char*>(in_name.c_str()) );

	isis_LOG(lg, isis_FILE, isis_INFO) << "coordinates: " 
		<< " x = " << in_coords[0] << " y = " << in_coords[1] << " z = " << in_coords[2];
	ProValueData coord_x_data;
	coord_x_data.type = PRO_VALUE_TYPE_DOUBLE;
	coord_x_data.v.d = in_coords[0];

	ProValueData coord_y_data;
	coord_y_data.type = PRO_VALUE_TYPE_DOUBLE;
	coord_y_data.v.d = in_coords[1];

	ProValueData coord_z_data;
	coord_z_data.type = PRO_VALUE_TYPE_DOUBLE;
	coord_z_data.v.d = in_coords[2];

	ElemTreeData datum_array[] = {
		{0, PRO_E_FEATURE_TREE, {PRO_VALUE_TYPE_NIL}},
		{1, PRO_E_FEATURE_TYPE, {PRO_VALUE_TYPE_INT, {PRO_FEAT_DATUM_POINT}}},
		{1, PRO_E_DPOINT_TYPE, {PRO_VALUE_TYPE_INT, {PRO_DPOINT_TYPE_OFFSET_CSYS}}},
		{1, PRO_E_STD_FEATURE_NAME, datum_name_data},
		{1, PRO_E_DPOINT_OFST_CSYS_TYPE, {PRO_VALUE_TYPE_INT, {PRO_DTMPNT_OFFCSYS_CARTESIAN}}},
		{1, PRO_E_DPOINT_OFST_CSYS_REF, default_csys_selector.getValueData()},
		{1, PRO_E_DPOINT_OFST_CSYS_WITH_DIMS, {PRO_VALUE_TYPE_INT, {PRO_B_TRUE}}},
		{1, PRO_E_DPOINT_OFST_CSYS_PNTS_ARRAY, {PRO_VALUE_TYPE_NIL}},
  			{2, PRO_E_DPOINT_OFST_CSYS_PNT, {PRO_VALUE_TYPE_NIL}},
				{3, PRO_E_DPOINT_OFST_CSYS_PNT_NAME, pt_name_data},
				{3, PRO_E_DPOINT_OFST_CSYS_DIR1_VAL, coord_x_data},
				{3, PRO_E_DPOINT_OFST_CSYS_DIR2_VAL, coord_y_data},
				{3, PRO_E_DPOINT_OFST_CSYS_DIR3_VAL, coord_z_data},
	};
	 
	switch( status = make_feature( out_feature, in_selection, datum_array, sizeof(datum_array)/sizeof(ElemTreeData ) )) {
	case PRO_TK_NO_ERROR: break;
	default:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "could not create base datum plane feature " << status;
	}
	return PRO_TK_NO_ERROR;
}

/*
type = PRO_DTMPLN_DEF_X

	Element tree (example)
	============
	FEATURE_TREE   (COMPOUND) 
	|---FEATURE_TYPE  VALUE (INT)       = 923 [PRO_FEAT_DATUM]
	|---STD_FEATURE_NAME  VALUE (WSTRING)   = Y_DTM_PLN
	|---DTMPLN_CONSTRAINTS   (ARRAY) 
	|    |---DTMPLN_CONSTRAINT   (COMPOUND) 
	|         |---DTMPLN_CONSTR_TYPE  VALUE (INT)       = 7 [PRO_DTMPLN_DEF_Y]
	|         |---DTMPLN_CONSTR_REF  VALUE (SELECTION) = 
	|         |---DTMPLN_CONSTR_REF_OFFSET  VALUE (DOUBLE)    = 0.000000
	|         |---DTMPLN_CONSTR_REF_ANGLE  VALUE (DOUBLE)    = 0.000000
	|         |---DTMPLN_SEC_IND  VALUE (INT)       = 0
	|         |---DTMPLN_OFF_CSYS  VALUE (INT)       = 0
	|         |---DTMPLN_OFF_CSYS_OFFSET  VALUE (DOUBLE)    = 0.000000
	|---DTMPLN_FLIP_DIR  VALUE (INT)       = 0
	|---DTMPLN_FIT   (COMPOUND) 
		|---DTMPLN_FIT_TYPE  VALUE (INT)       = -1
		|---DTMPLN_FIT_REF  VALUE (SELECTION) = 
		|---DTMPLN_FIT_DTM_RAD  VALUE (DOUBLE)    = 0.000000
		|---DTMPLN_FIT_OUTLINE  POINTER           = **
*/
ProError Plane( ProFeature& out_feature, ProMdl in_model, ProSelection in_selection, 
	std::string in_name, ProDtmplnConstrType in_type ) 
{
	
	

	ProName name;
	ProError status;

	ProValueData datum_name_data;
	datum_name_data.type = PRO_VALUE_TYPE_WSTRING;
	datum_name_data.v.w = ProStringToWstring( name, const_cast<char*>(in_name.c_str()));

	ElemTreeData datum_array[] = {
		{0, PRO_E_FEATURE_TREE, {PRO_VALUE_TYPE_NIL}},
		{1, PRO_E_FEATURE_TYPE, {PRO_VALUE_TYPE_INT, {PRO_FEAT_DATUM}}},
		{1, PRO_E_STD_FEATURE_NAME, datum_name_data},
		{1, PRO_E_DTMPLN_CONSTRAINTS, {PRO_VALUE_TYPE_NIL}},
		{2, PRO_E_DTMPLN_CONSTRAINT, {PRO_VALUE_TYPE_NIL}},
		{3, PRO_E_DTMPLN_CONSTR_TYPE, {PRO_VALUE_TYPE_INT, {in_type}}}
	};
	 
	switch( status = make_feature( out_feature, in_selection, datum_array, sizeof(datum_array)/sizeof(ElemTreeData ) )) {
	case PRO_TK_NO_ERROR: break;
	default:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "could not create base datum plane feature " << status;
	}
	return PRO_TK_NO_ERROR;
}

/**
FEATURE_TREE   (COMPOUND) 
	|---FEATURE_TYPE  VALUE (INT)       = 926 [PRO_FEAT_DATUM_AXIS]
	|---STD_FEATURE_NAME  VALUE (WSTRING)   = TRIM_AXIS [in_name]
	|---DTMAXIS_CONSTRAINTS   (ARRAY) 
	|    |---DTMAXIS_CONSTRAINT   (COMPOUND) 
	|    |    |---DTMAXIS_CONSTR_TYPE  VALUE (INT)       = 1  [PRO_DTMAXIS_CONSTR_TYPE_THRU]
	|    |    |---DTMAXIS_CONSTR_REF  VALUE (SELECTION)   id = 6 [in_datum_plane_a]
	|    |---DTMAXIS_CONSTRAINT   (COMPOUND) 
	|         |---DTMAXIS_CONSTR_TYPE  VALUE (INT)       = 1  [PRO_DTMAXIS_CONSTR_TYPE_THRU]
	|         |---DTMAXIS_CONSTR_REF  VALUE (SELECTION)   id = 4 [in_datum_plane_b]
	|---DTMAXIS_DIM_CONSTRAINTS   (ARRAY) 
	|---Elem Id: 3201   (COMPOUND) 
		|---Elem Id: 3202  VALUE (INT)       = 0
		|---Elem Id: 3203  VALUE (SELECTION) = 
		|---Elem Id: 3204  VALUE (DOUBLE)    = 0.000000
*/
ProError Axis( ProFeature& out_feature, ProMdl in_model, ProSelection in_selection, 
	std::string in_name, ProFeature in_datum_plane_a, ProFeature in_datum_plane_b ) 
{
	
	

	ProName name;
	ProError status;

	ProValueData datum_name_data; 
	datum_name_data.type = PRO_VALUE_TYPE_WSTRING;
	datum_name_data.v.w = ProStringToWstring( name, const_cast<char*>(in_name.c_str()));

	/* get the sufaces built upon the datum planes */
	ProValueData datum_pln_a_value;
	ProValueData datum_pln_b_value;
	try {
		Selector datum_pln_a( in_model, in_datum_plane_a, PRO_SURFACE );
		Selector datum_pln_b( in_model, in_datum_plane_b, PRO_SURFACE );

		datum_pln_a_value = datum_pln_a.getValueData();
		datum_pln_b_value = datum_pln_b.getValueData();

		ElemTreeData datum_array[] = {
			{0, PRO_E_FEATURE_TREE, {PRO_VALUE_TYPE_NIL}},
			{1, PRO_E_FEATURE_TYPE, {PRO_VALUE_TYPE_INT, {PRO_FEAT_DATUM_AXIS}}},
			{1, PRO_E_STD_FEATURE_NAME, datum_name_data},
			{1, PRO_E_DTMAXIS_CONSTRAINTS, {PRO_VALUE_TYPE_NIL}},
			{2, PRO_E_DTMAXIS_CONSTRAINT, {PRO_VALUE_TYPE_NIL}},
			{3, PRO_E_DTMAXIS_CONSTR_TYPE, {PRO_VALUE_TYPE_INT, {PRO_DTMAXIS_CONSTR_TYPE_THRU}}},
			{3, PRO_E_DTMAXIS_CONSTR_REF, datum_pln_a_value },
			{2, PRO_E_DTMAXIS_CONSTRAINT, {PRO_VALUE_TYPE_NIL}},
			{3, PRO_E_DTMAXIS_CONSTR_TYPE, {PRO_VALUE_TYPE_INT, {PRO_DTMAXIS_CONSTR_TYPE_THRU}}},
			{3, PRO_E_DTMAXIS_CONSTR_REF, datum_pln_b_value }
		};

		switch( status = make_feature( out_feature, in_selection, datum_array, sizeof(datum_array)/sizeof(ElemTreeData ) )) {
		case PRO_TK_NO_ERROR: break;
		default:
			isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "could not create 2-plane axis feature " << status;
		}
	} catch (std::runtime_error ex) {
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "could not select the datum plane surface " << ex.what();
	    return PRO_TK_GENERAL_ERROR;
	}
	return PRO_TK_NO_ERROR;
}
				
/**
FEATURE_TREE   (COMPOUND) 
	|---FEATURE_TYPE  VALUE (INT)       = 923  [PRO_FEAT_DATUM]
	|---STD_FEATURE_NAME  VALUE (WSTRING)   = TRIM_PLN [in_name]
	|---DTMPLN_CONSTRAINTS   (ARRAY) 
	|    |---DTMPLN_CONSTRAINT   (COMPOUND) 
	|    |    |---DTMPLN_CONSTR_TYPE  VALUE (INT)       = 0 [PRO_DTMPLN_THRU]
	|    |    |---DTMPLN_CONSTR_REF  VALUE (SELECTION)   id = 183 [in_datum_plane]
	|    |    |---DTMPLN_CONSTR_REF_OFFSET  VALUE (DOUBLE)    = 0.000000
	|    |    |---DTMPLN_CONSTR_REF_ANGLE  VALUE (DOUBLE)    = 0.000000
	|    |    |---DTMPLN_SEC_IND  VALUE (INT)       = 0
	|    |    |---DTMPLN_OFF_CSYS  VALUE (INT)       = 0
	|    |    |---DTMPLN_OFF_CSYS_OFFSET  VALUE (DOUBLE)    = 0.000000
	|    |---DTMPLN_CONSTRAINT   (COMPOUND) 
	|         |---DTMPLN_CONSTR_TYPE  VALUE (INT)       = 4 [PRO_DTMPLN_ANG]
	|         |---DTMPLN_CONSTR_REF  VALUE (SELECTION)   id = 177 [in_datum_axis]
	|         |---DTMPLN_CONSTR_REF_OFFSET  VALUE (DOUBLE)    = 0.000000
	|         |---DTMPLN_CONSTR_REF_ANGLE  VALUE (DOUBLE)    = 9.000000 [in_angle]
	|         |---DTMPLN_SEC_IND  VALUE (INT)       = 0
	|         |---DTMPLN_OFF_CSYS  VALUE (INT)       = 0
	|         |---DTMPLN_OFF_CSYS_OFFSET  VALUE (DOUBLE)    = 0.000000
	|---DTMPLN_FLIP_DIR  VALUE (INT)       = 0
	|---DTMPLN_FIT   (COMPOUND) 
		|---DTMPLN_FIT_TYPE  VALUE (INT)       = -1
		|---DTMPLN_FIT_REF  VALUE (SELECTION) = 
		|---DTMPLN_FIT_DTM_RAD  VALUE (DOUBLE)    = 0.000000
		|---DTMPLN_FIT_OUTLINE  POINTER           = **
*/
ProError Plane_Pivot( ProFeature& out_feature, ProMdl in_model, ProSelection in_selection, 
	std::string in_name, ProFeature in_datum_axis, ProFeature in_datum_plane, 
	double in_angle ) 
{
	
	

	ProName name;
	ProError status;

	ProValueData datum_name_data; 
	datum_name_data.type = PRO_VALUE_TYPE_WSTRING;
	datum_name_data.v.w = ProStringToWstring( name, const_cast<char*>(in_name.c_str()));

	/* get the geometries built upon the datums */
	try {
		Selector datum_pln( in_model, in_datum_plane, PRO_SURFACE );
		ProValueData datum_pln_value = datum_pln.getValueData();

		Selector datum_axis( in_model, in_datum_axis, PRO_AXIS );
		ProValueData datum_axis_value = datum_axis.getValueData();

		ProValueData datum_angle_data; 
		datum_angle_data.type = PRO_VALUE_TYPE_DOUBLE;
		datum_angle_data.v.d = in_angle;

		ElemTreeData datum_array[] = {
			{0, PRO_E_FEATURE_TREE, {PRO_VALUE_TYPE_NIL}},
			{1, PRO_E_FEATURE_TYPE, {PRO_VALUE_TYPE_INT, {PRO_FEAT_DATUM}}},
			{1, PRO_E_STD_FEATURE_NAME, datum_name_data},
			{1, PRO_E_DTMPLN_CONSTRAINTS, {PRO_VALUE_TYPE_NIL}},

				{2, PRO_E_DTMPLN_CONSTRAINT, {PRO_VALUE_TYPE_NIL}},
					{3, PRO_E_DTMPLN_CONSTR_TYPE, {PRO_VALUE_TYPE_INT, {PRO_DTMPLN_ANG}}},
					{3, PRO_E_DTMPLN_CONSTR_REF, datum_pln_value },
					{3, PRO_E_DTMPLN_CONSTR_REF_ANGLE, datum_angle_data },

				{2, PRO_E_DTMPLN_CONSTRAINT, {PRO_VALUE_TYPE_NIL}},
					{3, PRO_E_DTMPLN_CONSTR_TYPE, {PRO_VALUE_TYPE_INT, {PRO_DTMPLN_THRU}}},
					{3, PRO_E_DTMPLN_CONSTR_REF, datum_axis_value },

		};

		switch( status = make_feature( out_feature, in_selection, datum_array, sizeof(datum_array)/sizeof(ElemTreeData ) )) {
		case PRO_TK_NO_ERROR: break;
		default:
			isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "could not create pivot-plane feature " << status;
		}
	} catch (std::runtime_error ex) {
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "could not select a datum " << ex.what();
	    return PRO_TK_GENERAL_ERROR;
	}
	return PRO_TK_NO_ERROR;
}

ProError Plane_Offset( ProFeature& out_feature, ProMdl in_model, ProSelection in_selection, 
	std::string in_name, ProDtmplnFlipDir in_flip_dir, ProFeature in_datum_plane, double in_depth ) 
{
	
	

	ProName name;
	ProError status;

	ProValueData datum_name_data; 
	datum_name_data.type = PRO_VALUE_TYPE_WSTRING;
	datum_name_data.v.w = ProStringToWstring( name, const_cast<char*>(in_name.c_str()));

	/* get the geometries built upon the datums */
	try {
	    Selector datum_pln( in_model, in_datum_plane, PRO_SURFACE );
		ProValueData datum_pln_value = datum_pln.getValueData();

		ProValueData datum_depth_data; 
		datum_depth_data.type = PRO_VALUE_TYPE_DOUBLE;
		datum_depth_data.v.d = in_depth;

		ElemTreeData datum_array[] = {
			{0, PRO_E_FEATURE_TREE, {PRO_VALUE_TYPE_NIL}},
			{1, PRO_E_FEATURE_TYPE, {PRO_VALUE_TYPE_INT, {PRO_FEAT_DATUM}}},
			{1, PRO_E_STD_FEATURE_NAME, datum_name_data},
			{1, PRO_E_DTMPLN_CONSTRAINTS, {PRO_VALUE_TYPE_NIL}},

			{2, PRO_E_DTMPLN_CONSTRAINT, {PRO_VALUE_TYPE_NIL}},
			{3, PRO_E_DTMPLN_CONSTR_TYPE, {PRO_VALUE_TYPE_INT, {PRO_DTMPLN_OFFS}}},
			{3, PRO_E_DTMPLN_CONSTR_REF, datum_pln_value },
			{3, PRO_E_DTMPLN_CONSTR_REF_OFFSET, datum_depth_data },

			{1, PRO_E_DTMPLN_FLIP_DIR, {PRO_VALUE_TYPE_INT, {in_flip_dir} }},
		};

		switch( status = make_feature( out_feature, in_selection, datum_array, sizeof(datum_array)/sizeof(ElemTreeData ) )) {
		case PRO_TK_NO_ERROR: break;
		default:
			isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "could not create pivot-plane feature " << status;
		}
	} catch (std::runtime_error ex) {
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "could not select the surface " << ex.what();
	    return PRO_TK_GENERAL_ERROR;
	}
	return PRO_TK_NO_ERROR;
}

/**
This proceeds by getting the element tree of the input feature.
The relevant element is retrieved from the tree.
The element is updated with the new angle value.
The tree is updated with that element.
The feature is redefined with the revised element tree.
*/
ProError Plane_Angle_adjust( ProSelection in_selection, ProFeature in_datum_plane, double in_angle ) 
{
	
	

	ProError status;
	 
	ProFeatureElemtreeExtractOptions opts = PRO_FEAT_EXTRACT_NO_OPTS;
    ProElement angle_elem_tree;
	switch( status = ProFeatureElemtreeExtract( &in_datum_plane, NULL, opts, &angle_elem_tree ) ) {
	case PRO_TK_NO_ERROR: break;
	default:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "could not extract tree " << status;
		return PRO_TK_GENERAL_ERROR;
	}
	ProElempathItem angle_path_items[] = {
		{PRO_ELEM_PATH_ITEM_TYPE_ID, PRO_E_DTMPLN_CONSTRAINTS},
		{PRO_ELEM_PATH_ITEM_TYPE_INDEX, 0 },
		{PRO_ELEM_PATH_ITEM_TYPE_ID, PRO_E_DTMPLN_CONSTR_REF_ANGLE},
	};
    ProElempath angle_elem_path;
	ProElempathAlloc(&angle_elem_path);
	ProElempathDataSet( angle_elem_path, angle_path_items, sizeof(angle_path_items)/sizeof(ProElempathItem) );
	ProElement angle_elem;
	switch( status = ProElemtreeElementGet( angle_elem_tree, angle_elem_path, &angle_elem ) ) {
	case PRO_TK_NO_ERROR: break;
	default:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "could not extract angle tree " << status;
		return PRO_TK_GENERAL_ERROR;
	}
	double current_depth;
	switch( status = ProElementDoubleGet( angle_elem, NULL, &current_depth ) ) {
	case PRO_TK_NO_ERROR: break;
	default:	
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "could not get angle " << status;
		return PRO_TK_GENERAL_ERROR;
	}
#ifdef TRACE_ITERATION
	isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "current angle " << current_depth;
#endif
	switch( status = ProElementDoubleSet( angle_elem, in_angle) ) {
	case PRO_TK_NO_ERROR: break;
	default:	
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "could not set angle " << status;
		return PRO_TK_GENERAL_ERROR;
	}

	{ // regenerate the datum feature
		ProFeatureCreateOptions* datum_opts;
		{
			const int num_datum_opts = 1;
			ProArray* datum_opts_array = (ProArray*)(&datum_opts);
			switch( ProArrayAlloc(0, sizeof(ProFeatureCreateOptions), num_datum_opts, datum_opts_array) ) {
				case PRO_TK_NO_ERROR: break;
				default: return PRO_TK_GENERAL_ERROR;
			}
			ProFeatureCreateOptions opts[2];
			switch( ProArrayObjectAdd(datum_opts_array, PRO_VALUE_UNUSED, 1, static_cast<void*>(&(opts[0]))) ) {
				case PRO_TK_NO_ERROR: break;
				default: return PRO_TK_GENERAL_ERROR;
			}
		}
		
		int flags = PRO_REGEN_NO_FLAGS;
		ProErrorlist datum_error_list;

		switch( status = ProFeatureWithoptionsRedefine(NULL, &in_datum_plane, angle_elem_tree, datum_opts, 
		 	flags, &datum_error_list) ) 
		{
		case PRO_TK_NO_ERROR: break;
		case PRO_TK_BAD_INPUTS:
			isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "one or more of the input arguments are invalid.";
			ProUtilFeatErrsWrite("datum feature: ", PRO_TK_GENERAL_ERROR, angle_elem_tree, &datum_error_list);
			break;
		}
	}
	return PRO_TK_NO_ERROR;
}


/**
This proceeds by getting the element tree of the input feature.
The relevant element is retrieved from the tree.
The element is updated with the new offset value.
The tree is updated with that element.
The feature is redefined with the revised element tree.
*/
ProError Plane_Offset_adjust( ProSelection in_selection, ProFeature in_datum_plane, double in_depth ) 
{
	
	

	ProError status;
	 
	ProFeatureElemtreeExtractOptions opts = PRO_FEAT_EXTRACT_NO_OPTS;
    ProElement offset_elem_tree;
	switch( status = ProFeatureElemtreeExtract( &in_datum_plane, NULL, opts, &offset_elem_tree ) ) {
	case PRO_TK_NO_ERROR: break;
	default:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "could not extract tree " << status;
		return PRO_TK_GENERAL_ERROR;
	}
	ProElempathItem offset_path_items[] = {
		{PRO_ELEM_PATH_ITEM_TYPE_ID, PRO_E_DTMPLN_CONSTRAINTS},
		{PRO_ELEM_PATH_ITEM_TYPE_INDEX, 0 },
		{PRO_ELEM_PATH_ITEM_TYPE_ID, PRO_E_DTMPLN_CONSTR_REF_OFFSET},
	};
    ProElempath offset_elem_path;
	ProElempathAlloc(&offset_elem_path);
	ProElempathDataSet( offset_elem_path, offset_path_items, sizeof(offset_path_items)/sizeof(ProElempathItem) );
	ProElement offset_elem;
	switch( status = ProElemtreeElementGet( offset_elem_tree, offset_elem_path, &offset_elem ) ) {
	case PRO_TK_NO_ERROR: break;
	default:
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "could not extract tree " << status;
		return PRO_TK_GENERAL_ERROR;
	}
	double current_depth;
	switch( status = ProElementDoubleGet( offset_elem, NULL, &current_depth ) ) {
	case PRO_TK_NO_ERROR: break;
	default:	
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "could not get offset " << status;
		return PRO_TK_GENERAL_ERROR;
	}
#ifdef TRACE_ITERATION
	isis_LOG(lg, isis_FILE, isis_INFO) << "current depth " << current_depth;
#endif
	switch( status = ProElementDoubleSet( offset_elem, in_depth ) ) {
	case PRO_TK_NO_ERROR: break;
	default:	
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "could not set offset " << status;
		return PRO_TK_GENERAL_ERROR;
	}

	{ // regenerate the datum feature
		ProFeatureCreateOptions* datum_opts;
		{
			const int num_datum_opts = 1;
			ProArray* datum_opts_array = (ProArray*)(&datum_opts);
			switch( ProArrayAlloc(0, sizeof(ProFeatureCreateOptions), num_datum_opts, datum_opts_array) ) {
				case PRO_TK_NO_ERROR: break;
				default: return PRO_TK_GENERAL_ERROR;
			}
			ProFeatureCreateOptions opts[2];
			switch( ProArrayObjectAdd(datum_opts_array, PRO_VALUE_UNUSED, 1, static_cast<void*>(&(opts[0]))) ) {
				case PRO_TK_NO_ERROR: break;
				default: return PRO_TK_GENERAL_ERROR;
			}
		}
		
		int flags = PRO_REGEN_NO_FLAGS;
		ProErrorlist datum_error_list;

		switch( status = ProFeatureWithoptionsRedefine(NULL, &in_datum_plane, offset_elem_tree, datum_opts, 
		 	flags, &datum_error_list) ) 
		{
		case PRO_TK_NO_ERROR: break;
		case PRO_TK_BAD_INPUTS:
			isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "one or more of the input arguments are invalid.";
			ProUtilFeatErrsWrite("datum feature: ", PRO_TK_GENERAL_ERROR, offset_elem_tree, &datum_error_list);
			break;
		}
	}
	return PRO_TK_NO_ERROR;
}

/**
FEATURE_TREE   (COMPOUND) 
	|---FEATURE_TYPE  VALUE (INT)           = 916
	|---FEATURE_FORM  VALUE (INT)           = 512
	|---PATCH_QUILT  VALUE (SELECTION)   id = 233
	|---PATCH_TYPE  VALUE (INT)             = 916
	|---PATCH_MATERIAL_SIDE  VALUE (INT)    = -1 
	|---Elem Id: 3330  VALUE (INT)          = 0
	|---Elem Id: 3329  VALUE (INT)          = 0
	|---STD_FEATURE_NAME  VALUE (WSTRING)   = SOLIDIFY_2 [in_name]
*/
ProError Solidify_Truncate( ProFeature& out_feature, ProMdl in_model, ProSelection in_selection, 
	std::string in_name, ProFeature in_datum_plane ) 
{
	
	

	ProName name;
	ProError status;

	ProValueData datum_name_data; 
	datum_name_data.type = PRO_VALUE_TYPE_WSTRING;
	datum_name_data.v.w = ProStringToWstring( name, const_cast<char*>(in_name.c_str()));

	/* get the geometries built upon the datums */
	try {
		Selector datum_pln( in_model, in_datum_plane, PRO_SURFACE );
		ProValueData quilt_value = datum_pln.getValueData();

		ElemTreeData datum_array[] = {
			{0, PRO_E_FEATURE_TREE, {PRO_VALUE_TYPE_NIL}},
			{1, PRO_E_FEATURE_TYPE, {PRO_VALUE_TYPE_INT, {PRO_FEAT_CUT}}},
			{1, PRO_E_STD_FEATURE_NAME, datum_name_data},
			{1, PRO_E_FEATURE_FORM, {PRO_VALUE_TYPE_INT, {PRO_USE_SURFS}}},

			{1, PRO_E_PATCH_QUILT, quilt_value },
			{1, PRO_E_PATCH_TYPE, {PRO_VALUE_TYPE_INT, {PRO_EXT_MATERIAL_REMOVE}}},
			{1, PRO_E_PATCH_MATERIAL_SIDE, {PRO_VALUE_TYPE_INT, {PRO_SOLIDIFY_SIDE_ONE}}},
		};

		switch( status = make_feature( out_feature, in_selection, datum_array, sizeof(datum_array)/sizeof(ElemTreeData ) )) {
		case PRO_TK_NO_ERROR: break;
		case PRO_TK_GENERAL_ERROR:
			{
				ProModelitem quilt_item;
				ProSelectionModelitemGet( quilt_value.v.r, &quilt_item );
				isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "could not create truncated-solid: "
					<< " datum-id = " << in_datum_plane.id
					<< " geom-id = " << quilt_item.id;
//				isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "model "
				switch( status = ProMdlSave(in_model) ) {
				case PRO_TK_NO_ERROR: break;
				default:
					isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "could not save model [" << status << "] ";
				}
			}
			break;
		default:
			isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "could not create truncated-solid [" << status << "] "
				<< (const void*)quilt_value.v.r << " fid: " << in_datum_plane.id;
		}
	} catch (std::runtime_error ex) {
		isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "could not select the datum plane surface";
	    return PRO_TK_GENERAL_ERROR;
	}
	return PRO_TK_NO_ERROR;
}

/**
Get the cut surface produced by the solidify-truncate.
*/
ProError Plane_SurfaceGet( ProSurface& out_surface, ProFeature* in_dpf, ProSurface* out_surf ) {
	
	

	ProError status = PRO_TK_NO_ERROR;

	// switch( status = ProFeatureGet( &in_datum_plane_a, &surface_a ) ) {
	// case PRO_TK_NO_ERROR: break;
	// default:
	// 	isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "could not get surface for datum plane" << status;
	// 	return PRO_TK_GENERAL_ERROR;
	// }
	// ProElementReferenceSet();
	return status;
}

} // create
} // feature
} // creo
} // isis