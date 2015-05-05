#include <CoordinateSystemRoutines.h>

#include <isis_ptc_toolkit_functions.h>
#include <GraphicsFunctions.h>
#include <CommonUtilities.h>
#include "WindowsFunctions.h"
#include <CADCommonConstants.h>
#include <fstream>
#include <sstream>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace isis_CADCommon;
namespace isis
{
	////////////////////////////////////////////////////////////////////////////////////////
	template <class DATA_VALUE_TYPE> void FeatureTree_SetSingleValue ( 
															int in_ElementType, 
															ProValueDataType in_DataType,
															DATA_VALUE_TYPE in_Value,
															ProValue &in_out_Value_StorageField,
															ProElement &out_AddToElement )
	{
		ProValueData valueData;
		//ProValue value;
		ProElement proElement;
		//std::cout << std::endl << "out_AddToElement: " << out_AddToElement;
		isis::isis_ProElementAlloc ( in_ElementType, &proElement );
		valueData.type = in_DataType;

		switch ( in_DataType )
		{
			case PRO_VALUE_TYPE_INT:
				  valueData.v.i  = in_Value;  // DATA_VALUE_TYPE
				  //std::cout << std::endl << "valueData.v.i  = in_Value: " << in_Value;
				  break;
			case PRO_VALUE_TYPE_DOUBLE:
				  valueData.v.d  = in_Value;  // DATA_VALUE_TYPE
				  //std::cout << std::endl << "valueData.v.d  = in_Value: " << in_Value;
				  break;
			default:
				std::stringstream messageString;
					messageString <<
				"Error: (Function - FeatureTree_SetSingleValue), received a in_DataType that is not supported.  Supported types are PRO_VALUE_TYPE_INT(1) and PRO_VALUE_TYPE_DOUBLE(2).  " <<
				"  in_DataType: "	 << in_DataType;
			throw isis::application_exception(messageString.str().c_str());
		}

		isis::isis_ProValueAlloc ( &in_out_Value_StorageField );
		isis::isis_ProValueDataSet ( in_out_Value_StorageField, &valueData );
		isis::isis_ProElementValueSet ( proElement, in_out_Value_StorageField );
		isis::isis_ProElemtreeElementAdd ( out_AddToElement, NULL, proElement );
	}

	////////////////////////////////////////////////////////////////////////////////////////
	void PopulateDummyCoordinateSystemFeatuerTree( 
							ProMdl	in_p_model,
							//ProMdlType in_pro_model_type,
							const ProCsys	   &in_ReferenceCoordinateSystem,
							const std::string &in_CoordinateSystemName,
							ProElement   &out_pro_e_feature_tree )
													throw(isis::application_exception)												
	{

		//---------------------------------------------------------------
		// Populating  element PRO_E_FEATURE_TREE
		// PRO_FEAT_CSYS
		// PRO_E_STD_FEATURE_NAME
		//---------------------------------------------------------------

		isis::isis_ProElementAlloc ( PRO_E_FEATURE_TREE, &out_pro_e_feature_tree );
		//std::cout << std::endl << "out_pro_e_feature_tree: " << out_pro_e_feature_tree;
		
		ProElement pro_e_feature_type;
		isis::isis_ProElementAlloc ( PRO_E_FEATURE_TYPE, &pro_e_feature_type );	
		isis::isis_ProElementIntegerSet(pro_e_feature_type, PRO_FEAT_CSYS );
		isis::isis_ProElemtreeElementAdd(out_pro_e_feature_tree, NULL, pro_e_feature_type);

		ProElement pro_e_std_feature_name;
		isis::isis_ProElementAlloc ( PRO_E_STD_FEATURE_NAME, &pro_e_std_feature_name );
		wchar_t   coordinateSystemName_wide[ISIS_CHAR_BUFFER_LENGTH];
		ProStringToWstring(coordinateSystemName_wide, (char *) in_CoordinateSystemName.c_str() );
		isis::isis_ProElementWstringSet(pro_e_std_feature_name, coordinateSystemName_wide);
		isis::isis_ProElemtreeElementAdd(out_pro_e_feature_tree, NULL, pro_e_std_feature_name);

		ProValueData value_data;
		ProValue value;
		//---------------------------------------------------------------
		// Populating  element pro_e_csys_origin_constrs
		// PRO_E_CSYS_ORIGIN_CONSTRS
		// PRO_E_CSYS_ORIGIN_CONSTR
		//---------------------------------------------------------------
		///*  Don't set a reference coordinate system so that the default coordinate system would be 
		 //   used by default
		ProElement pro_e_csys_origin_constrs;
		isis::isis_ProElementAlloc ( PRO_E_CSYS_ORIGIN_CONSTRS, &pro_e_csys_origin_constrs );
		isis::isis_ProElemtreeElementAdd ( out_pro_e_feature_tree, NULL, pro_e_csys_origin_constrs);

		ProElement pro_e_csys_origin_constr;
		isis::isis_ProElementAlloc ( PRO_E_CSYS_ORIGIN_CONSTR, &pro_e_csys_origin_constr );	
		isis::isis_ProElemtreeElementAdd ( pro_e_csys_origin_constrs, NULL, pro_e_csys_origin_constr  );

		//======================================================
		// Get the default coordinate system
		//======================================================
		
		ProModelitem  modelCoordinateSystemItem;
		/*
		wchar_t   defaultCoordinateSystemName[ISIS_CHAR_BUFFER_LENGTH];
		if ( in_pro_model_type == PRO_MDL_ASSEMBLY)
			ProStringToWstring(defaultCoordinateSystemName, "ASM_CSYS_DEF" );
		else
			ProStringToWstring(defaultCoordinateSystemName, "PRT_CSYS_DEF" );

		isis::isis_ProModelitemByNameInit (in_p_model, PRO_CSYS, defaultCoordinateSystemName, &modelCoordinateSystemItem);
		*/

		int cSys_ID;
		isis::isis_ProCsysIdGet (in_ReferenceCoordinateSystem, &cSys_ID);
		isis::isis_ProModelitemInit(in_p_model, cSys_ID, PRO_CSYS, &modelCoordinateSystemItem);

		ProSelection modelCoordinateSystemSelection;
		isis::isis_ProSelectionAlloc(NULL, &modelCoordinateSystemItem, &modelCoordinateSystemSelection );

		//=============================================================== 
		// PRO_E_CSYS_ORIGIN_CONSTR_REF
		//=============================================================== 

		value_data.type = PRO_VALUE_TYPE_SELECTION;
		value_data.v.r = modelCoordinateSystemSelection;
		isis::isis_ProValueAlloc ( &value );	
		isis::isis_ProValueDataSet ( value, &value_data );	

		ProElement pro_e_csys_origin_constr_ref;
		isis::isis_ProElementAlloc ( PRO_E_CSYS_ORIGIN_CONSTR_REF, &pro_e_csys_origin_constr_ref );
		isis::isis_ProElementValueSet ( pro_e_csys_origin_constr_ref, value );

		isis::isis_ProElemtreeElementAdd ( pro_e_csys_origin_constr, NULL, pro_e_csys_origin_constr_ref );	
	

		//=============================================================== 
		// PRO_E_CSYS_OFFSET_TYPE
		// PRO_E_CSYS_ORIENTMOVES
		// PRO_E_CSYS_ORIENTMOVE
		// PRO_E_CSYS_ORIENTMOVE_MOVE_TYPE
		//=============================================================== 
		ProElement pro_e_csys_offset_type;
		isis::isis_ProElementAlloc ( PRO_E_CSYS_OFFSET_TYPE, &pro_e_csys_offset_type );
		isis::isis_ProElementIntegerSet(pro_e_csys_offset_type, PRO_CSYS_OFFSET_CARTESIAN );
		isis::isis_ProElemtreeElementAdd ( out_pro_e_feature_tree, NULL, pro_e_csys_offset_type );

		ProElement pro_e_csys_orientmoves;
		isis::isis_ProElementAlloc ( PRO_E_CSYS_ORIENTMOVES, &pro_e_csys_orientmoves );
		isis::isis_ProElemtreeElementAdd ( out_pro_e_feature_tree, NULL, pro_e_csys_orientmoves  );

		ProElement pro_e_csys_orientmove;

		//---------------------------------------------------------------
		// X, Y, Z rotations
		//---------------------------------------------------------------
		double xRotation = 0, yRotation = 0, zRotation = 0;  // Degrees

		// Rotation x
		
		isis::isis_ProElementAlloc ( PRO_E_CSYS_ORIENTMOVE, &pro_e_csys_orientmove );
		isis::isis_ProElemtreeElementAdd ( pro_e_csys_orientmoves, NULL, pro_e_csys_orientmove  );
		
		ProValue value_xRot_Type;
		ProValue value_xRot_Value;

		FeatureTree_SetSingleValue<ProCsysOrientMoveMoveOpt> (	PRO_E_CSYS_ORIENTMOVE_MOVE_TYPE, 
																PRO_VALUE_TYPE_INT, 
																PRO_CSYS_ORIENTMOVE_MOVE_OPT_ROT_X,
																value_xRot_Type,
																pro_e_csys_orientmove);


		FeatureTree_SetSingleValue<double> (PRO_E_CSYS_ORIENTMOVE_MOVE_VAL, 
												  PRO_VALUE_TYPE_DOUBLE, 
												  -xRotation,
												  value_xRot_Value,
												  pro_e_csys_orientmove);
		
		// Rotation y
		isis::isis_ProElementAlloc ( PRO_E_CSYS_ORIENTMOVE, &pro_e_csys_orientmove );
		isis::isis_ProElemtreeElementAdd ( pro_e_csys_orientmoves, NULL, pro_e_csys_orientmove  );
		
		ProValue value_yRot_Type;
		ProValue value_yRot_Value;

		FeatureTree_SetSingleValue<ProCsysOrientMoveMoveOpt> (	PRO_E_CSYS_ORIENTMOVE_MOVE_TYPE, 
																PRO_VALUE_TYPE_INT, 
																PRO_CSYS_ORIENTMOVE_MOVE_OPT_ROT_Y,
																value_yRot_Type,
																pro_e_csys_orientmove);


		FeatureTree_SetSingleValue<double> (PRO_E_CSYS_ORIENTMOVE_MOVE_VAL, 
												  PRO_VALUE_TYPE_DOUBLE, 
												  -yRotation,
												  value_yRot_Value,
												  pro_e_csys_orientmove);

		// Rotation z
		isis::isis_ProElementAlloc ( PRO_E_CSYS_ORIENTMOVE, &pro_e_csys_orientmove );
		isis::isis_ProElemtreeElementAdd ( pro_e_csys_orientmoves, NULL, pro_e_csys_orientmove  );
		
		ProValue value_zRot_Type;
		ProValue value_zRot_Value;

		FeatureTree_SetSingleValue<ProCsysOrientMoveMoveOpt> (	PRO_E_CSYS_ORIENTMOVE_MOVE_TYPE, 
																PRO_VALUE_TYPE_INT, 
																PRO_CSYS_ORIENTMOVE_MOVE_OPT_ROT_Z,
																value_zRot_Type,
																pro_e_csys_orientmove);


		FeatureTree_SetSingleValue<double> (PRO_E_CSYS_ORIENTMOVE_MOVE_VAL, 
												  PRO_VALUE_TYPE_DOUBLE, 
												  -zRotation,
												  value_zRot_Value,
												  pro_e_csys_orientmove);

		//---------------------------------------------------------------
		// X, Y, Z offsets
		//---------------------------------------------------------------
		double xOffset = 0, yOffset = 0, zOffset = 0;
		// x offset
		
		isis::isis_ProElementAlloc ( PRO_E_CSYS_ORIENTMOVE, &pro_e_csys_orientmove );
		isis::isis_ProElemtreeElementAdd ( pro_e_csys_orientmoves, NULL, pro_e_csys_orientmove  );

		ProValue value_xTrans_Type;
		ProValue value_xTrans_Value;

		FeatureTree_SetSingleValue<ProCsysOrientMoveMoveOpt> (	PRO_E_CSYS_ORIENTMOVE_MOVE_TYPE, 
																PRO_VALUE_TYPE_INT, 
																PRO_CSYS_ORIENTMOVE_MOVE_OPT_TRAN_X,
																value_xTrans_Type,
																pro_e_csys_orientmove);


		FeatureTree_SetSingleValue<double> (	PRO_E_CSYS_ORIENTMOVE_MOVE_VAL, 
												PRO_VALUE_TYPE_DOUBLE, 
												xOffset,
												value_xTrans_Value,
												pro_e_csys_orientmove);
	

		// y offset
		isis::isis_ProElementAlloc ( PRO_E_CSYS_ORIENTMOVE, &pro_e_csys_orientmove );
		isis::isis_ProElemtreeElementAdd ( pro_e_csys_orientmoves, NULL, pro_e_csys_orientmove  );

		ProValue value_yTrans_Type;
		ProValue value_yTrans_Value;

		FeatureTree_SetSingleValue<ProCsysOrientMoveMoveOpt> (	PRO_E_CSYS_ORIENTMOVE_MOVE_TYPE, 
																PRO_VALUE_TYPE_INT, 
																PRO_CSYS_ORIENTMOVE_MOVE_OPT_TRAN_Y,
																value_yTrans_Type,
																pro_e_csys_orientmove);

		FeatureTree_SetSingleValue<double> (PRO_E_CSYS_ORIENTMOVE_MOVE_VAL, 
												  PRO_VALUE_TYPE_DOUBLE, 
												  yOffset,
												  value_yTrans_Value,
												  pro_e_csys_orientmove);

		// z offset
		isis::isis_ProElementAlloc ( PRO_E_CSYS_ORIENTMOVE, &pro_e_csys_orientmove );
		isis::isis_ProElemtreeElementAdd ( pro_e_csys_orientmoves, NULL, pro_e_csys_orientmove  );

		ProValue value_zTrans_Type;
		ProValue value_zTrans_Value;

		FeatureTree_SetSingleValue<ProCsysOrientMoveMoveOpt> (PRO_E_CSYS_ORIENTMOVE_MOVE_TYPE, 
															  PRO_VALUE_TYPE_INT, 
															  PRO_CSYS_ORIENTMOVE_MOVE_OPT_TRAN_Z,
															  value_zTrans_Type,
															  pro_e_csys_orientmove);

		FeatureTree_SetSingleValue<double> (PRO_E_CSYS_ORIENTMOVE_MOVE_VAL, 
												  PRO_VALUE_TYPE_DOUBLE, 
												  zOffset,
												  value_zTrans_Value,
												  pro_e_csys_orientmove);

		//---------------------------------------------------------------
		// Populating  element PRO_E_CSYS_ORIENT_BY_METHOD
		//---------------------------------------------------------------
		ProElement pro_e_csys_orient_by_method;
		isis::isis_ProElementAlloc ( PRO_E_CSYS_ORIENT_BY_METHOD, 
			&pro_e_csys_orient_by_method );

		value_data.type = PRO_VALUE_TYPE_INT;
		value_data.v.i = PRO_CSYS_ORIENT_BY_SEL_CSYS_AXES; 
		isis::isis_ProValueAlloc ( &value );

		isis::isis_ProValueDataSet ( value, &value_data );
		isis::isis_ProElementValueSet ( pro_e_csys_orient_by_method, value );
		isis::isis_ProElemtreeElementAdd ( out_pro_e_feature_tree, NULL, pro_e_csys_orient_by_method );
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	/*  Approach using transfromation matrix
	void PopulateCoordinateSystemFeatuerTree( 
							ProMdl	in_p_model,
							ProMdlType in_pro_model_type,
							const std::string &in_CoordinateSystemName,
							const TransformationMatrix  &transformationMatrix,
							ProElement   &out_pro_e_feature_tree )
													throw(isis::application_exception)												
	{

		//---------------------------------------------------------------
		// Populating  element PRO_E_FEATURE_TREE
		// PRO_FEAT_CSYS
		// PRO_E_STD_FEATURE_NAME
		//---------------------------------------------------------------

		double xRotation, yRotation, zRotation;
		double xOffset, yOffset, zOffset;
		transformationMatrix.getRotationAngles(xRotation, yRotation, zRotation);
		transformationMatrix.getOffset(xOffset, yOffset, zOffset);

		//std::cout << std::endl << "xOffset yOffset, zOffset, " << xOffset << "  " << yOffset << "  " << zOffset;
		//std::cout << std::endl << "xRotation yRotation, zRotation, " << xRotation << "  " << yRotation << "  " << zRotation;

		isis::isis_ProElementAlloc ( PRO_E_FEATURE_TREE, &out_pro_e_feature_tree );
		//std::cout << std::endl << "out_pro_e_feature_tree: " << out_pro_e_feature_tree;
		
		ProElement pro_e_feature_type;
		isis::isis_ProElementAlloc ( PRO_E_FEATURE_TYPE, &pro_e_feature_type );	
		isis::isis_ProElementIntegerSet(pro_e_feature_type, PRO_FEAT_CSYS );
		isis::isis_ProElemtreeElementAdd(out_pro_e_feature_tree, NULL, pro_e_feature_type);

		ProElement pro_e_std_feature_name;
		isis::isis_ProElementAlloc ( PRO_E_STD_FEATURE_NAME, &pro_e_std_feature_name );
		wchar_t   coordinateSystemName_wide[ISIS_CHAR_BUFFER_LENGTH];
		ProStringToWstring(coordinateSystemName_wide, (char *) in_CoordinateSystemName.c_str() );
		isis::isis_ProElementWstringSet(pro_e_std_feature_name, coordinateSystemName_wide);
		isis::isis_ProElemtreeElementAdd(out_pro_e_feature_tree, NULL, pro_e_std_feature_name);

		//---------------------------------------------------------------
		// Populating  element pro_e_csys_origin_constrs
		// PRO_E_CSYS_ORIGIN_CONSTRS
		// PRO_E_CSYS_ORIGIN_CONSTR
		//---------------------------------------------------------------

		ProElement pro_e_csys_origin_constrs;
		isis::isis_ProElementAlloc ( PRO_E_CSYS_ORIGIN_CONSTRS, &pro_e_csys_origin_constrs );
		isis::isis_ProElemtreeElementAdd ( out_pro_e_feature_tree, NULL, pro_e_csys_origin_constrs);

		ProElement pro_e_csys_origin_constr;
		isis::isis_ProElementAlloc ( PRO_E_CSYS_ORIGIN_CONSTR, &pro_e_csys_origin_constr );	
		isis::isis_ProElemtreeElementAdd ( pro_e_csys_origin_constrs, NULL, pro_e_csys_origin_constr  );

		//======================================================
		// Get the default coordinate system
		//======================================================
		ProModelitem  modelCoordinateSystemItem;
		
		wchar_t   defaultCoordinateSystemName[ISIS_CHAR_BUFFER_LENGTH];
		if ( in_pro_model_type == PRO_MDL_ASSEMBLY)
			ProStringToWstring(defaultCoordinateSystemName, "ASM_CSYS_DEF" );
		else
			ProStringToWstring(defaultCoordinateSystemName, "PRT_CSYS_DEF" );

		isis::isis_ProModelitemByNameInit (in_p_model, PRO_CSYS, defaultCoordinateSystemName, &modelCoordinateSystemItem);

		ProSelection modelCoordinateSystemSelection;
		isis::isis_ProSelectionAlloc(NULL, &modelCoordinateSystemItem, &modelCoordinateSystemSelection );

		//=============================================================== 
		// PRO_E_CSYS_ORIGIN_CONSTR_REF
		//=============================================================== 
		ProValueData value_data;
		ProValue value;
		value_data.type = PRO_VALUE_TYPE_SELECTION;
		value_data.v.r = modelCoordinateSystemSelection;
		isis::isis_ProValueAlloc ( &value );	
		isis::isis_ProValueDataSet ( value, &value_data );	

		ProElement pro_e_csys_origin_constr_ref;
		isis::isis_ProElementAlloc ( PRO_E_CSYS_ORIGIN_CONSTR_REF, &pro_e_csys_origin_constr_ref );
		isis::isis_ProElementValueSet ( pro_e_csys_origin_constr_ref, value );

		isis::isis_ProElemtreeElementAdd ( pro_e_csys_origin_constr, NULL, pro_e_csys_origin_constr_ref );	

		//=============================================================== 
		// PRO_E_CSYS_OFFSET_TYPE
		// PRO_E_CSYS_ORIENTMOVES
		// PRO_E_CSYS_ORIENTMOVE
		// PRO_E_CSYS_ORIENTMOVE_MOVE_TYPE
		//=============================================================== 
		ProElement pro_e_csys_offset_type;
		isis::isis_ProElementAlloc ( PRO_E_CSYS_OFFSET_TYPE, &pro_e_csys_offset_type );
		isis::isis_ProElementIntegerSet(pro_e_csys_offset_type, PRO_CSYS_OFFSET_CARTESIAN );
		isis::isis_ProElemtreeElementAdd ( out_pro_e_feature_tree, NULL, pro_e_csys_offset_type );

		ProElement pro_e_csys_orientmoves;
		isis::isis_ProElementAlloc ( PRO_E_CSYS_ORIENTMOVES, &pro_e_csys_orientmoves );
		isis::isis_ProElemtreeElementAdd ( out_pro_e_feature_tree, NULL, pro_e_csys_orientmoves  );

		ProElement pro_e_csys_orientmove;

		//---------------------------------------------------------------
		// X, Y, Z rotations
		//---------------------------------------------------------------
		
		// Rotation x
		
		isis::isis_ProElementAlloc ( PRO_E_CSYS_ORIENTMOVE, &pro_e_csys_orientmove );
		isis::isis_ProElemtreeElementAdd ( pro_e_csys_orientmoves, NULL, pro_e_csys_orientmove  );
		
		ProValue value_xRot_Type;
		ProValue value_xRot_Value;

		FeatureTree_SetSingleValue<ProCsysOrientMoveMoveOpt> (	PRO_E_CSYS_ORIENTMOVE_MOVE_TYPE, 
																PRO_VALUE_TYPE_INT, 
																PRO_CSYS_ORIENTMOVE_MOVE_OPT_ROT_X,
																value_xRot_Type,
																pro_e_csys_orientmove);


		FeatureTree_SetSingleValue<double> (PRO_E_CSYS_ORIENTMOVE_MOVE_VAL, 
												  PRO_VALUE_TYPE_DOUBLE, 
												  -xRotation / M_PI * 180.0,
												  value_xRot_Value,
												  pro_e_csys_orientmove);
		
		
		// Rotation y
		isis::isis_ProElementAlloc ( PRO_E_CSYS_ORIENTMOVE, &pro_e_csys_orientmove );
		isis::isis_ProElemtreeElementAdd ( pro_e_csys_orientmoves, NULL, pro_e_csys_orientmove  );
		
		ProValue value_yRot_Type;
		ProValue value_yRot_Value;

		FeatureTree_SetSingleValue<ProCsysOrientMoveMoveOpt> (	PRO_E_CSYS_ORIENTMOVE_MOVE_TYPE, 
																PRO_VALUE_TYPE_INT, 
																PRO_CSYS_ORIENTMOVE_MOVE_OPT_ROT_Y,
																value_yRot_Type,
																pro_e_csys_orientmove);


		FeatureTree_SetSingleValue<double> (PRO_E_CSYS_ORIENTMOVE_MOVE_VAL, 
												  PRO_VALUE_TYPE_DOUBLE, 
												  //0.0,
												  -yRotation / M_PI * 180.0,
												  value_yRot_Value,
												  pro_e_csys_orientmove);

		// Rotation z
		isis::isis_ProElementAlloc ( PRO_E_CSYS_ORIENTMOVE, &pro_e_csys_orientmove );
		isis::isis_ProElemtreeElementAdd ( pro_e_csys_orientmoves, NULL, pro_e_csys_orientmove  );
		
		ProValue value_zRot_Type;
		ProValue value_zRot_Value;

		FeatureTree_SetSingleValue<ProCsysOrientMoveMoveOpt> (	PRO_E_CSYS_ORIENTMOVE_MOVE_TYPE, 
																PRO_VALUE_TYPE_INT, 
																PRO_CSYS_ORIENTMOVE_MOVE_OPT_ROT_Z,
																value_zRot_Type,
																pro_e_csys_orientmove);


		FeatureTree_SetSingleValue<double> (PRO_E_CSYS_ORIENTMOVE_MOVE_VAL, 
												  PRO_VALUE_TYPE_DOUBLE, 
												  //0.0,
												  -zRotation / M_PI * 180.0,
												  value_zRot_Value,
												  pro_e_csys_orientmove);



		//---------------------------------------------------------------
		// X, Y, Z offsets
		//---------------------------------------------------------------

		// x offset
		
		isis::isis_ProElementAlloc ( PRO_E_CSYS_ORIENTMOVE, &pro_e_csys_orientmove );
		isis::isis_ProElemtreeElementAdd ( pro_e_csys_orientmoves, NULL, pro_e_csys_orientmove  );

		ProValue value_xTrans_Type;
		ProValue value_xTrans_Value;

		FeatureTree_SetSingleValue<ProCsysOrientMoveMoveOpt> (	PRO_E_CSYS_ORIENTMOVE_MOVE_TYPE, 
																PRO_VALUE_TYPE_INT, 
																PRO_CSYS_ORIENTMOVE_MOVE_OPT_TRAN_X,
																value_xTrans_Type,
																pro_e_csys_orientmove);


		FeatureTree_SetSingleValue<double> (	PRO_E_CSYS_ORIENTMOVE_MOVE_VAL, 
												PRO_VALUE_TYPE_DOUBLE, 
												xOffset,
												value_xTrans_Value,
												pro_e_csys_orientmove);
	

		// y offset
		isis::isis_ProElementAlloc ( PRO_E_CSYS_ORIENTMOVE, &pro_e_csys_orientmove );
		isis::isis_ProElemtreeElementAdd ( pro_e_csys_orientmoves, NULL, pro_e_csys_orientmove  );

		ProValue value_yTrans_Type;
		ProValue value_yTrans_Value;

		FeatureTree_SetSingleValue<ProCsysOrientMoveMoveOpt> (	PRO_E_CSYS_ORIENTMOVE_MOVE_TYPE, 
																PRO_VALUE_TYPE_INT, 
																PRO_CSYS_ORIENTMOVE_MOVE_OPT_TRAN_Y,
																value_yTrans_Type,
																pro_e_csys_orientmove);

		FeatureTree_SetSingleValue<double> (PRO_E_CSYS_ORIENTMOVE_MOVE_VAL, 
												  PRO_VALUE_TYPE_DOUBLE, 
												  yOffset,
												  value_yTrans_Value,
												  pro_e_csys_orientmove);

		// z offset
		isis::isis_ProElementAlloc ( PRO_E_CSYS_ORIENTMOVE, &pro_e_csys_orientmove );
		isis::isis_ProElemtreeElementAdd ( pro_e_csys_orientmoves, NULL, pro_e_csys_orientmove  );

		ProValue value_zTrans_Type;
		ProValue value_zTrans_Value;

		FeatureTree_SetSingleValue<ProCsysOrientMoveMoveOpt> (PRO_E_CSYS_ORIENTMOVE_MOVE_TYPE, 
															  PRO_VALUE_TYPE_INT, 
															  PRO_CSYS_ORIENTMOVE_MOVE_OPT_TRAN_Z,
															  value_zTrans_Type,
															  pro_e_csys_orientmove);

		FeatureTree_SetSingleValue<double> (PRO_E_CSYS_ORIENTMOVE_MOVE_VAL, 
												  PRO_VALUE_TYPE_DOUBLE, 
												  zOffset,
												  value_zTrans_Value,
												  pro_e_csys_orientmove);

											  

		//---------------------------------------------------------------
		// Populating  element PRO_E_CSYS_ORIENT_BY_METHOD
		//---------------------------------------------------------------
		ProElement pro_e_csys_orient_by_method;
		isis::isis_ProElementAlloc ( PRO_E_CSYS_ORIENT_BY_METHOD, 
			&pro_e_csys_orient_by_method );

		value_data.type = PRO_VALUE_TYPE_INT;
		value_data.v.i = PRO_CSYS_ORIENT_BY_SEL_CSYS_AXES; 
		isis::isis_ProValueAlloc ( &value );

		isis::isis_ProValueDataSet ( value, &value_data );
		isis::isis_ProElementValueSet ( pro_e_csys_orient_by_method, value );
		isis::isis_ProElemtreeElementAdd ( out_pro_e_feature_tree, NULL, pro_e_csys_orient_by_method );

		
		//  Must move this after the create function
		//isis::isis_ProValueFree ( value_xRot_Type );
		//isis::isis_ProValueFree ( value_xRot_Value );
		//isis::isis_ProValueFree ( value_yRot_Type );
		//isis::isis_ProValueFree ( value_yRot_Value );
		//isis::isis_ProValueFree ( value_zRot_Type );
		//isis::isis_ProValueFree ( value_zRot_Value );

		//isis::isis_ProValueFree ( value_xTrans_Type );
		//isis::isis_ProValueFree ( value_xTrans_Value );
		//isis::isis_ProValueFree ( value_yTrans_Type );
		//isis::isis_ProValueFree ( value_yTrans_Value );
		//isis::isis_ProValueFree ( value_zTrans_Type );
		//isis::isis_ProValueFree ( value_zTrans_Value );

	}
	*/
	////////////////////////////////////////////////////////////////////////////////////////
	
	void CreateCoordinateSystem( ProMdl	in_p_model, 
								 const ProCsys	   &in_ReferenceCoordinateSystem,
								 const std::string &in_CoordinateSystemName,
								 const std::vector<double> &in_Orgin,
								 const std::vector<double> &in_XVector,
								 const std::vector<double> &in_YVector ) throw(isis::application_exception)
	{

		if ( in_Orgin.size() != 3 || in_XVector.size() != 3  || in_YVector.size() != 3 )
		{
			std::stringstream messageString;
			messageString <<
				"Error: (Function - CreateCoordinateSystem), received a vector that does not have three and only three elements." <<
				"  in_Orgin: "	 << in_Orgin << 
				"  in_XVector: " << in_XVector << 
				"  in_YVector: " << in_YVector;
			throw isis::application_exception(messageString.str().c_str());
		}

		//std::cout << std::endl << "in_XVector: " << in_XVector;
		//std::cout << std::endl << "in_YVector: " << in_YVector;

		// Comput Z Vector
		//const std::vector<double>  zVector = CrossProduct_3D(in_XVector,in_YVector);

		TransformationMatrix  transformationMatrix;

		transformationMatrix.setTransformationMatrix(in_XVector, in_YVector, in_Orgin);

		double rotationMatrix[3][3];
		std::vector<double> offset;
		
		// Comput Z Vector
		const std::vector<double>  zVector = CrossProduct_3D(in_XVector,in_YVector);

		std::string workingDir = isis_CADCommon::isis_GetCurrentDirectoryA();		

		std::ofstream coordinateTransdFormFile_stream;
		//std::string   coordinateTransdFormFile_name = "zztrantmpzz.trf";
		std::string   coordinateTransdFormFile_name = workingDir + "\\zztrantmpzz.trf";

		coordinateTransdFormFile_stream.open(coordinateTransdFormFile_name );
		coordinateTransdFormFile_stream <<              in_XVector[0] << "  " << in_YVector[0] << "  " << zVector[0] << "  " << in_Orgin[0];
		coordinateTransdFormFile_stream << std::endl << in_XVector[1] << "  " << in_YVector[1] << "  " << zVector[1] << "  " << in_Orgin[1];
		coordinateTransdFormFile_stream << std::endl << in_XVector[2] << "  " << in_YVector[2] << "  " << zVector[2] << "  " << in_Orgin[2];
		
		coordinateTransdFormFile_stream.close();

		ProElement	elem_tree; 

		PopulateDummyCoordinateSystemFeatuerTree( in_p_model, in_ReferenceCoordinateSystem, in_CoordinateSystemName, elem_tree );

		wchar_t   coordinateTransFormFile_name_wide[ISIS_CHAR_BUFFER_LENGTH];

		ProStringToWstring(coordinateTransFormFile_name_wide, (char *)coordinateTransdFormFile_name.c_str() );
		isis::isis_ProDtmcsysTransformfileRead ( coordinateTransFormFile_name_wide, elem_tree );

		isis::IfFileExists_DeleteFile( coordinateTransdFormFile_name);

		//=============================================================== 
		// Intialize Pro Selection
		//=============================================================== 
		ProAsmcomppath model_path;
		ProIdTable c_id_table;
		c_id_table [0] = -1;
		isis::isis_ProAsmcomppathInit((ProSolid)in_p_model, c_id_table, 0, &model_path);

		ProModelitem	model_modelitem;
		isis_ProMdlToModelitem(model_path.owner, &model_modelitem);	
		
		ProSelection	mdl_sel;
		isis_ProSelectionAlloc(NULL, &model_modelitem, &mdl_sel);
		//=============================================================== 
		// Create the Datum Axis
		//=============================================================== 

		ProFeature	feature;
		ProErrorlist	errors;
		ProFeatureCreateOptions opts[1];
		opts[0] = PRO_FEAT_CR_DEFINE_MISS_ELEMS;

		isis_ProFeatureCreate(mdl_sel, elem_tree, opts, 1, &feature, &errors);

		//std::cout << std::endl << "Number of errors: " << errors.error_number;

		isis_ProElementFree(&elem_tree); 
	}
	
	////////////////////////////////////////////////////////////////////////////////////////
	/*
	// The following was based on Euler angles, which are not the angles used by
	// the Creo feature tree.  The Creo feature tree uses angles relative to the reference
	// coordinate system.
	void CreateCoordinateSystem( ProMdl	in_p_model, 
								 ProMdlType in_pro_model_type,
								 const std::string &in_CoordinateSystemName,
								 const std::vector<double> &in_Orgin,
								 const std::vector<double> &in_XVector,
								 const std::vector<double> &in_YVector ) throw(isis::application_exception)
	{

		if ( in_Orgin.size() != 3 || in_XVector.size() != 3  || in_YVector.size() != 3 )
		{
			std::stringstream messageString;
			messageString <<
				"Error: (Function - CreateCoordinateSystem), received a vector that does not have three and only three elements." <<
				"  in_Orgin: "	 << in_Orgin << 
				"  in_XVector: " << in_XVector << 
				"  in_YVector: " << in_YVector;
			throw isis::application_exception(messageString.str().c_str());
		}

		TransformationMatrix  transformationMatrix;

		transformationMatrix.setTransformationMatrix(in_XVector, in_YVector, in_Orgin);
		
		ProElement	elem_tree;

		PopulateCoordinateSystemFeatuerTree( in_p_model,in_pro_model_type,in_CoordinateSystemName, transformationMatrix, elem_tree );

		//=============================================================== 
		// Intialize Pro Selection
		//=============================================================== 
		ProAsmcomppath model_path;
		ProIdTable c_id_table;
		c_id_table [0] = -1;
		isis::isis_ProAsmcomppathInit((ProSolid)in_p_model, c_id_table, 0, &model_path);

		ProModelitem	model_modelitem;
		isis_ProMdlToModelitem(model_path.owner, &model_modelitem);	
		
		ProSelection	mdl_sel;
		isis_ProSelectionAlloc(NULL, &model_modelitem, &mdl_sel);
		//=============================================================== 
		// Create the Datum Axis
		//=============================================================== 

		ProFeature	feature;
		ProErrorlist	errors;
		ProFeatureCreateOptions opts[1];
		opts[0] = PRO_FEAT_CR_DEFINE_MISS_ELEMS;

		isis_ProFeatureCreate(mdl_sel, elem_tree, opts, 1, &feature, &errors);

		//std::cout << std::endl << "Number of errors: " << errors.error_number;

		// The line "isis_ProElementFree(&elem_tree)" throws and exception with ProError = -11.
		// Not sure why this is happening.  Will allow the leak for now.
		//isis_ProElementFree(&elem_tree); 
	}
	*/


	// The following code was taken from
	// C:\Program Files\PTC\Creo 1.0\Common Files\M030\protoolkit\protk_appls\pt_userguide\ptu_utils\UtilCsys.c
	// and modified to find the default coordinate system.  The code originally would find a coordinate
	// system based on the name of the coordinate system.

	//--------------------------------------------------------------------
	// ProAppData used while visiting Csys
	//--------------------------------------------------------------------
	typedef struct {
		ProMdl		model;
		ProCsys		p_csys;
		ProName		csys_name;
		}UserCsysAppData ; 

	//--------------------------------------------------------------------
	// Filter function used while visiting Csys
	//--------------------------------------------------------------------*/
	ProError CsysFindFilterAction (
		ProCsys		this_csys,
		ProAppData	app_data)
	{
		//ProError 		status;
		//ProName		this_csys_name;
		ProModelitem	mdlitem;
		int id;
		
		isis::isis_ProCsysIdGet (this_csys, &id);
		isis::isis_ProModelitemInit(((UserCsysAppData *) app_data)->model, id, PRO_CSYS, &mdlitem);

		//char temp_string[PRO_NAME_SIZE];
		//ProWstringToString(temp_string, this_csys_name);

		//std::cout << std::endl << "Coordinate System Name: " << temp_string;

		ProFeature feature;
		isis::isis_ProGeomitemFeatureGet(&mdlitem, &feature);

		ProElempathItem items[1];
		items[0].type = items[0].type = PRO_ELEM_PATH_ITEM_TYPE_ID;
		items[0].path_item.elem_id =	PRO_E_CSYS_ORIGIN_CONSTRS;
		
		ProElempath epath;
		isis::isis_ProElempathAlloc(&epath);
		isis::isis_ProElempathDataSet(epath, items, 1);

		ProElement elem_tree;
		isis::isis_ProFeatureElemtreeExtract (&feature, NULL, PRO_FEAT_EXTRACT_NO_OPTS, &elem_tree);

		int originConstrs_count;
		
		isis::isis_ProElementArrayCount( elem_tree, epath, &originConstrs_count);

		//std::cout << std::endl << "originConstrs_count: " << originConstrs_count;

		items[0].type = items[0].type = PRO_ELEM_PATH_ITEM_TYPE_ID;
		items[0].path_item.elem_id =	PRO_E_CSYS_ORIENTMOVES;

		int orientMoves_count;
		isis::isis_ProElementArrayCount( elem_tree, epath, &orientMoves_count);

		//std::cout << std::endl << "orientMoves_count: " << orientMoves_count;

		if ( originConstrs_count == 0 && orientMoves_count == 0 )
			return PRO_TK_NO_ERROR;
		else 
			return PRO_TK_CONTINUE;
	}  

	//--------------------------------------------------------------------
	// Visit Action used while visiting Csys
	//--------------------------------------------------------------------
	ProError CsysFindVisitAction (
		ProCsys		this_csys,
		ProError	filter_return,
			ProAppData      app_data)
	{
	  int 		status;
 
	  if (filter_return != PRO_TK_NO_ERROR)
		return (filter_return);
  
	   ( (UserCsysAppData *) app_data) -> p_csys = this_csys; 
	  return (PRO_TK_USER_ABORT);
	}

	//====================================================================
	//	FUNCTION :  ProUtilCsysFind()
	//	PURPOSE  :  Find a csys given a model and a csys name
	//====================================================================
	void FindDefaultCoordinateSystem(
		ProMdl				in_p_model,
		const std::string   &in_ModelName,
		ProCsys		&out_p_csys)	throw(isis::application_exception)
	{
	  ProError 			status;
	  UserCsysAppData	app_data;
 
	  app_data.model = in_p_model;

	  status = ProSolidCsysVisit( 	(ProSolid) in_p_model, 
					(ProCsysVisitAction) CsysFindVisitAction,
					(ProCsysFilterAction) CsysFindFilterAction,
					(ProAppData) &app_data);


	  if ( status != PRO_TK_USER_ABORT ) 
	  {
		std::stringstream errorString;
		errorString <<
			"Error: (Function - FindDefaultCoordinateSystem), could not locate the default coordinate system in model: " << in_ModelName;
		throw isis::application_exception(errorString.str().c_str());

	  }

	  out_p_csys = app_data.p_csys;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////

	//--------------------------------------------------------------------
	// ProAppData used while visiting Axis
	//--------------------------------------------------------------------
	typedef struct {
		ProMdl		model;
		ProAxis		p_axis;
		ProName		axis_name;
		}UserAxisAppData ; 

	//--------------------------------------------------------------------
	// Filter function used while visiting Axis
	//--------------------------------------------------------------------*/
	ProError AxisFindFilterAction (
		ProAxis		this_axis,
		ProAppData	app_data)
	{
		ProName		this_axis_name;
		ProModelitem	mdlitem;
		int id;
		
		isis::isis_ProAxisIdGet (this_axis, &id);

		std::cout << std::endl << "isis::isis_ProAxisIdGet , id: " << id;
		isis::isis_ProModelitemInit(((UserAxisAppData *) app_data)->model, id, PRO_AXIS, &mdlitem);
		isis::isis_ProModelitemNameGet ( &mdlitem, this_axis_name);

	  if ( ProUtilWstrcmp( ((UserAxisAppData *) app_data)->axis_name, this_axis_name) == 0 )
		 return(PRO_TK_NO_ERROR);
	  else
		return(PRO_TK_CONTINUE);
	}  

	//--------------------------------------------------------------------
	// Visit Action used while visiting Axis
	//--------------------------------------------------------------------
	ProError AxisFindVisitAction (
		ProAxis		this_axis,
		ProError	filter_return,
			ProAppData      app_data)
	{
	  int 		status;
 
	  if (filter_return != PRO_TK_NO_ERROR)
		return (filter_return);
  
	  ( (UserAxisAppData *) app_data) -> p_axis = this_axis; 
	  return (PRO_TK_USER_ABORT);
	}

	//====================================================================
	//	FUNCTION :  ProUtilAxisFind()
	//	PURPOSE  :  Find a axis given a model and a axis name
	//====================================================================
	bool FoundProAxisBasedOnAxisName(
		ProMdl				in_p_model,
		const std::string	&in_AxisName,	
		ProAxis				&out_p_axis)	throw(isis::application_exception)
	{
	  ProError 			status;
	  UserAxisAppData	app_data;

	  ProStringToWstring(app_data.axis_name, (char*)ConvertToUpperCase(in_AxisName).c_str());
	  app_data.model = in_p_model;

	  status = ProSolidAxisVisit( 	(ProSolid) in_p_model, 
					(ProAxisVisitAction) AxisFindVisitAction,
					(ProAxisFilterAction) AxisFindFilterAction,
					(ProAppData) &app_data);


	  if ( status != PRO_TK_USER_ABORT ) 
	  {
		return false;
	  }
	  else
	  {
		out_p_axis = app_data.p_axis;
		return true;
	  }

	}

} // END namespace isis