/*! \file StringToEnumConversions.h
    \brief  Functions to convert between strings and enums.

	The input/output XML files that are used by the CAD applications 
	do not persist enums for known values.  They instead persist strings, 
	which for efficiency reasons are converted to enums.  This file provides 
	the conversion routines.
*/
#ifndef STRING_TO_ENUM_CONVERSIONS_H
#define STRING_TO_ENUM_CONVERSIONS_H

#pragma warning( disable : 4290 )  // a future feature : exception specification, i.e. throw

#include <isis_application_exception.h>
#include <isis_include_ptc_headers.h>
#include <string>
#include <iostream>


using namespace std;

namespace isis
{
	std::string  Bool_string( bool in_Bool );



	// in_CADComponentType_string		ProType
	// -----------------------------	-------------
	// PART							PRO_MDL_PART
	// ASSEMBLY						PRO_MDL_ASSEMBLY
	ProMdlType ProMdlType_enum( 
		                         string in_ProMdlType_string)
								 throw (isis::application_exception);

	std::string ProMdlType_string(  ProMdlType in_ProMdlType_enum)
											throw (isis::application_exception);


	// in_FeatureGeometryType_string	ProType
	// -----------------------------	-------------
	// SURFACE							PRO_SURFACE
	// AXIS								PRO_AXIS
	ProType FeatureGeometryType_enum( 
		                         string in_StringFeatureGeometryType)
								 throw (isis::application_exception);

	std::string FeatureGeometryType_string(  ProType in_FeatureGeometryType_enum)
											throw (isis::application_exception);


	//	in_FeatureAlignmentType_string	ProAsmcompConstrType
	//	------------------------------	--------------------
	//	ALIGN							PRO_ASM_ALIGN
	//	MATE							PRO_ASM_MATE
	//	ALIGN_OFFSET					PRO_ALIGN_OFFSET
	//	MATE_OFFSET						PRO_ASM_MATE_OFF
	//  ORIENT							PRO_ASM_ORIENT
	ProAsmcompConstrType ProAsmcompConstrType_enum( 
		                         string in_FeatureAlignmentType_string)
								 throw (isis::application_exception);

	std::string ProAsmcompConstrType_string(  ProAsmcompConstrType in_ProAsmcompConstrType_enum)
											throw (isis::application_exception);


	//	in_FeatureOrientationType_string	ProDatumside
	//	--------------------------------	-------------
	//  NONE								PRO_DATUM_SIDE_NONE
	//  SIDE_A								PRO_DATUM_SIDE_YELLOW
	//  SIDE_B								PRO_DATUM_SIDE_RED
	ProDatumside ProDatumside_enum( 
		                         string in_FeatureOrientationType_string)
								 throw (isis::application_exception);

	std::string ProDatumside_string(  ProDatumside in_ProDatumside_enum)
											throw (isis::application_exception);

    enum e_FeatureInterfaceType
    {
        CAD_DATUM,
        CAD_MODEL_INTERFACE,
        CAD_MODEL_USER_DATA
    };


    e_FeatureInterfaceType FeatureInterfaceType_enum( 
		                         const string &in_FeatureInterfaceType_string)
								 throw (isis::application_exception);

	std::string FeatureInterfaceType_string( e_FeatureInterfaceType in_FeatureInterfaceType_enum)
											throw (isis::application_exception);


	enum e_CADParameterType
    {
        CAD_FLOAT,
        CAD_INTEGER,
        CAD_BOOLEAN,
		CAD_STRING
    };

	 e_CADParameterType CADParameterType_enum( 
		                         const string &in_CADParameterType_string)
								 throw (isis::application_exception);


	 std::string  CADParameterType_string(   e_CADParameterType in_CADParameterType_enum )
											throw (isis::application_exception);

	ProBoolean String_To_ProBoolean( 
		                         const string &in_Boolean_string)
								 throw (isis::application_exception);

	//////////////////////////////////////////////////////////////////////////////////////////
	enum e_CADSpecialInstruction
    {
        CAD_SPECIAL_INSTRUCTION_NONE,
        CAD_SPECIAL_INSTRUCTION_SIZE_TO_FIT,
		CAD_SPECIAL_INSTRUCTION_HAS_KINEMATIC_JOINT 
    };

	 e_CADSpecialInstruction SpecialInstruction_enum( 
		                         const string &in_SpecialInstruction_string)
								 throw (isis::application_exception);

	 std::string SpecialInstruction_string(  e_CADSpecialInstruction in_CADComponentType_enum)
											throw (isis::application_exception);

	//////////////////////////////////////////////////////////////////////////////////////////
	enum e_CADUnitsDistance
    {
        CAD_UNITS_MM,
		CAD_UNITS_CM,
		CAD_UNITS_INCH,
		CAD_UNITS_NA,
    };

	 e_CADUnitsDistance			CADUnitsDistance_enum( 
		                         const string &in_CADUnits_string)
								 throw (isis::application_exception);


	std::string CADUnitsDistance_string(  e_CADUnitsDistance in_Units_enum)
											throw (isis::application_exception);
	//////////////////////////////////////////////////////////////////////////////////////////	
	enum e_CADUnitsForce
    {
        CAD_UNITS_LBF,
		CAD_UNITS_N,
    };

	 e_CADUnitsForce			CADUnitsForce_enum( 
		                         const string &in_CADUnitsForce_string)
								 throw (isis::application_exception);


	std::string CADUnitsForce_string(  e_CADUnitsForce in_UnitsForce_enum)
											throw (isis::application_exception);
	//////////////////////////////////////////////////////////////////////////////////////////

	enum e_CADUnitsMoment
    {
        CAD_UNITS_FT_LBF,
		CAD_UNITS_IN_LBF,
		CAD_UNITS_N_M,
		CAD_UNITS_N_MM,
    };

	 e_CADUnitsMoment			CADUnitsMoment_enum( 
		                         const string &in_CADUnitsMoment_string)
								 throw (isis::application_exception);


	std::string CADUnitsMoment_string(  e_CADUnitsMoment in_UnitsMoment_enum)
											throw (isis::application_exception);
	//////////////////////////////////////////////////////////////////////////////////////////

	enum e_CADUnitsPressure
    {
		CAD_UNITS_MPA,
        CAD_UNITS_LBF_PER_INCH_SQ,
		CAD_UNITS_PA
    };

	 e_CADUnitsPressure			CADUnitsPressure_enum( 
		                         const string &in_CADUnitsPressure_string)
								 throw (isis::application_exception);


	std::string CADUnitsPressure_string(  e_CADUnitsPressure in_UnitsPressure_enum)
											throw (isis::application_exception);


	//////////////////////////////////////////////////////////////////////////////////////////
	enum e_CADUnitsAcceleration
    {
        CAD_UNITS_FT_PER_SEC_SQ,
		CAD_UNITS_MM_PER_SEC_SQ,
		CAD_UNITS_M_PER_SEC_SQ
    };

	 e_CADUnitsAcceleration			CADUnitsAcceleration_enum( 
		                         const string &in_CADUnitsAcceleration_string)
								 throw (isis::application_exception);


	std::string CADUnitsAcceleration_string(  e_CADUnitsAcceleration in_Units_enum)
											throw (isis::application_exception);

	//////////////////////////////////////////////////////////////////////////////////////////
	enum e_CADUnitsDensity
    {
		CAD_UNITS_KG_PER_MM_CUBED,
        CAD_UNITS_KG_PER_M_CUBED
    };

	 e_CADUnitsDensity			CADUnitsDensity_enum( 
		                         const string &in_CADUnitsDensity_string)
								 throw (isis::application_exception);


	std::string CADUnitsDensity_string(  e_CADUnitsDensity in_Units_enum)
											throw (isis::application_exception);

	//////////////////////////////////////////////////////////////////////////////////////////
	enum e_CADUnitsHeatCapacity
    {
		CAD_UNITS_J_PER_KG_K,
    };

	 e_CADUnitsHeatCapacity			CADUnitsHeatCapacity_enum( 
		                         const string &in_CADUnitsHeatCapacity_string)
								 throw (isis::application_exception);


	std::string CADUnitsHeatCapacity_string(  e_CADUnitsHeatCapacity in_Units_enum)
											throw (isis::application_exception);

	//////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////
	enum e_CADUnitsThermalConductivity
    {
		CAD_UNITS_W_PER_MM_K,
    };

	 e_CADUnitsThermalConductivity			CADUnitsThermalConductivity_enum( 
		                         const string &in_CADUnitsThermalConductivity_string)
								 throw (isis::application_exception);


	std::string CADUnitsThermalConductivity_string(  e_CADUnitsThermalConductivity in_Units_enum)
											throw (isis::application_exception);

	//////////////////////////////////////////////////////////////////////////////////////////


	enum e_CADUnitsAngle
    {
        CAD_UNITS_RADIAN,
		CAD_UNITS_DEGREE,
		CAD_UNITS_ANGLE_NA
    };

	 e_CADUnitsAngle			CADUnitsAngle_enum( 
		                         const string &in_CADUnitsAngle_string)
								 throw (isis::application_exception);


	std::string CADUnitsAngle_string(  e_CADUnitsAngle in_UnitsAngle_enum)
											throw (isis::application_exception);

	//////////////////////////////////////////////////////////////////////////////////////////
	enum e_AnalysisType
	{
		ANALYSIS_STRUCTURAL,
		ANALYSIS_BUCKLING,
		ANALYSIS_MODAL,
		ANALYSIS_THERMAL,
		ANALYSIS_FATIGUE
	};

	 e_AnalysisType			AnalysisType_enum( 
		                         const string &in_AnalysisType_string)
								 throw (isis::application_exception);


	std::string AnalysisType_string(  e_AnalysisType in_AnalysisType_enum)
											throw (isis::application_exception);
	//////////////////////////////////////////////////////////////////////////////////////////
	/*
	enum e_AnalysisLoadType
	{
		ANALYSIS_LOAD_FORCE,
		ANALYSIS_LOAD_PRESSURE,
		ANALYSIS_LOAD_BEARING,
		ANALYSIS_LOAD_ACCELERATION,
		ANALYSIS_LOAD_CENTRIFUGAL,
		ANALYSIS_LOAD_PRELOAD,
		ANALYSIS_LOAD_TEMPERATURE
	};

	 e_AnalysisLoadType			AnalysisLoadType_enum( 
		                         const string &in_CADUnits_string)
								 throw (isis::application_exception);


	std::string AnalysisLoadType_string(  e_AnalysisLoadType in_Units_enum)
											throw (isis::application_exception);
	*/
	//////////////////////////////////////////////////////////////////////////////////////////

	enum e_AnalysisPartInterfaceType
	{
		ANALYSIS_PART_INTERFACE_FREE,
		ANALYSIS_PART_INTERFACE_CONTACTING,
		ANALYSIS_PART_INTERFACE_BONDED
	};

	 e_AnalysisPartInterfaceType			AnalysisPartInterfaceType_enum( 
		                         const string &in_AnalysisPartInterfaceType_string)
								 throw (isis::application_exception);


	std::string AnalysisPartInterfaceType_string(  e_AnalysisPartInterfaceType in_AnalysisPartInterfaceType_enum)
											throw (isis::application_exception);
	//////////////////////////////////////////////////////////////////////////////////////////

	//enum e_AnalysisSolverType
	//{
	//	ANALYSIS_SOLVER_NASTRAN,
	//	ANALYSIS_SOLVER_ANSYS,
	//	ANALYSIS_SOLVER_SIMULIA
	//};

	 pro_fem_solver_type			AnalysisSolverType_enum( 
		                         const string &in_AnalysisSolverType_string)
								 throw (isis::application_exception);


	std::string AnalysisSolverType_string(  pro_fem_solver_type in_AnalysisSolverType_enum)
											throw (isis::application_exception);
	//////////////////////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////////////////////

	enum e_AnalysisSolutionType
	{
		ANALYSIS_DECK_BASED,
		ANALYSIS_MODEL_BASED
	};

	 e_AnalysisSolutionType			AnalysisSolutionType_enum( 
		                         const string &in_AnalysisSolutionType_string)
								 throw (isis::application_exception);


	std::string AnalysisSolutionType_string(  e_AnalysisSolutionType in_AnalysisSolutionType_enum)
											throw (isis::application_exception);
	//////////////////////////////////////////////////////////////////////////////////////////

	enum e_AnalysisConstraintFixedFree
	{
		ANALYSIS_CONSTRAINT_SCALAR,
		ANALYSIS_CONSTRAINT_FIXED,
		ANALYSIS_CONSTRAINT_FREE
	};

	 e_AnalysisConstraintFixedFree			AnalysisConstraintFixedFree_enum( 
		                         const string &in_AnalysisConstraintFixedFree_string)
								 throw (isis::application_exception);

	std::string AnalysisConstraintFixedFree_string(  e_AnalysisConstraintFixedFree in_AnalysisConstraintFixedFree_enum)
											throw (isis::application_exception);

	e_AnalysisConstraintFixedFree AnalysisConstraintFixedFree_enum( const string &in_AnalysisConstraintFixedFree_string )
																throw (isis::application_exception);

	//////////////////////////////////////////////////////////////////////////////////////////

	//enum e_AnalysisMeshType
	//{
	//	ANALYSIS_MESH_SOLID,
	//	ANALYSIS_MESH_SHELL,
	//	ANALYSIS_MESH_QUILT,
	//	ANALYSIS_MESH_BOUNDARY,
	//	ANALYSIS_MESH_BAR
	//};

	 pro_fem_mesh_type			AnalysisMeshType_enum( 
		                         const string &in_AnalysisMeshType_string)
								 throw (isis::application_exception);


	std::string AnalysisMeshType_string(  pro_fem_mesh_type in_AnalysisMeshType_enum)
											throw (isis::application_exception);
	//////////////////////////////////////////////////////////////////////////////////////////

//	enum e_AnalysisShellElementType
//	{
//		ANALYSIS_SHELL_ELEMENT_NOT_APPLICABLE,
//		ANALYSIS_SHELL_ELEMENT_TRIANGLE,
//		ANALYSIS_SHELL_ELEMENT_QUADRANGLE
//	};

	 pro_fem_shell_mesh_type			AnalysisShellElementType_enum( 
		                         const string &in_AnalysisShellElementType_string)
								 throw (isis::application_exception);


	std::string AnalysisShellElementType_string(  pro_fem_shell_mesh_type in_AnalysisShellElementType_enum)
											throw (isis::application_exception);
	//////////////////////////////////////////////////////////////////////////////////////////

	//enum e_AnalysisElementShapeType
	//{
	//	ANALYSIS_ELEMENT_SHAPE_MIDPOINT_LINEAR,
	//	ANALYSIS_ELEMENT_SHAPE_MIDPOINT_PARABOLIC,
	//	ANALYSIS_ELEMENT_SHAPE_MIDPOINT_PARABOLIC_FIXED
	//};

	 pro_fem_elem_shape_type			AnalysisElementShapeType_enum( 
		                         const string &in_AnalysisElementShapeType_string)
								 throw (isis::application_exception);


	std::string AnalysisElementShapeType_string(  pro_fem_elem_shape_type in_AnalysisElementShapeType_enum)
											throw (isis::application_exception);
	//////////////////////////////////////////////////////////////////////////////////////////


	enum e_CADGeometryType
	{
		CAD_GEOMETRY_POLYGON,
		CAD_GEOMETRY_CIRCLE,
		CAD_GEOMETRY_CONCENTRIC_CIRCLES,
		CAD_GEOMETRY_CYLINDER,
		//CAD_GEOMETRY_CYLINDER_SURFACE,
		CAD_GEOMETRY_SPHERE,
		//CAD_GEOMETRY_SPHERE_SURFACE,
		CAD_GEOMETRY_EXTRUSION,
		CAD_GEOMETRY_FACE,
		//CAD_GEOMETRY_EXTRUSION_SURFACE
		CAD_GEOMETRY_NONE
	};

	 e_CADGeometryType			CADGeometryType_enum( 
		                         const string &in_CADGeometryType_string)
								 throw (isis::application_exception);


	std::string CADGeometryType_string(  e_CADGeometryType in_CADGeometryType_enum)
											throw (isis::application_exception);
	//////////////////////////////////////////////////////////////////////////////////////////

	enum e_CADFeatureGeometryType
	{
		CAD_FEATURE_GEOMETRY_POINT,
		//CAD_FEATURE_GEOMETRY_FACE  // For Abaqus model-based only.
	};

	 e_CADFeatureGeometryType			CADFeatureGeometryType_enum( 
		                         const string &in_CADFeatureGeometryType_string)
								 throw (isis::application_exception);


	std::string CADFeatureGeometryType_string(  e_CADFeatureGeometryType in_CADFeatureGeometryType_enum)
											throw (isis::application_exception);
	//////////////////////////////////////////////////////////////////////////////////////////

	enum e_CADPrimaryGeometryQualifier
	{
		CAD_INTERIOR_ONLY,				
		CAD_BOUNDARY_ONLY,				
		CAD_INTERIOR_AND_BOUNDARY		
	};

	 e_CADPrimaryGeometryQualifier			CADPrimaryGeometryQualifier_enum( 
		                         const string &in_CADPrimaryGeometryQualifier_string)
								 throw (isis::application_exception);

	std::string CADPrimaryGeometryQualifier_string(  e_CADPrimaryGeometryQualifier in_CADPrimaryGeometryQualifier_enum)
											throw (isis::application_exception);

	//////////////////////////////////////////////////////////////////////////////////////////


	enum e_CADSecondaryGeometryQualifier
	{
		CAD_INCLUDE_END_CAPS,				
		CAD_EXCLUDE_END_CAPS,
		CAD_SECONDARY_GEOMETRY_QUALIFIER_NONE
	};

	 e_CADSecondaryGeometryQualifier			CADSecondaryGeometryQualifier_enum( 
		                         const string &in_CADSecondaryGeometryQualifier_string)
								 throw (isis::application_exception);


	std::string CADSecondaryGeometryQualifier_string(  e_CADSecondaryGeometryQualifier in_CADSecondaryGeometryQualifier_enum)
											throw (isis::application_exception);

	//////////////////////////////////////////////////////////////////////////////////////////

	enum e_MaterialType
    {
        MATERIAL_STEEL,
		MATERIAL_ALUMINUM,
        MATERIAL_PLASTIC,
		MATERIAL_CERAMIC,
		MATERIAL_POLYMER,
		MATERIAL_TITANIUM,
		MATERIAL_STAINLESS,
		MATERIAL_COMPOSITE_FIBER,
		MATERIAL_HARD_PLASTIC,
		MATERIAL_FUEL,
		MATERIAL_FLUID
    };	

	 e_MaterialType			MaterialType_enum( 
		                         const string &in_MaterialType_string)
								 throw (isis::application_exception);


	std::string MaterialType_string(  e_MaterialType in_MaterialType_enum)
											throw (isis::application_exception);
	//////////////////////////////////////////////////////////////////////////////////////////

	enum e_DataExchangeFormat
    {
        DATA_EXCHANGE_FORMAT_STEP,
		DATA_EXCHANGE_FORMAT_STEREOLITHOGRAPHY,  //STL
		DATA_EXCHANGE_FORMAT_INVENTOR,
		DATA_EXCHANGE_FORMAT_PARASOLID
    };	

	e_DataExchangeFormat	DataExchangeFormat_enum( 
		                         const string &in_DataExchangeFormat_string)
								 throw (isis::application_exception);

	std::string DataExchangeFormat_string(  e_DataExchangeFormat in_DataExchangeFormat_enum)
											throw (isis::application_exception);
	//////////////////////////////////////////////////////////////////////////////////////////
	enum e_DataExchangeVersion
    {
        DATA_EXCHANGE_VERSION_NOT_APPLICABLE,
		AP203_SINGLE_FILE,
		AP203_E2_SINGLE_FILE,
        AP203_E2_SEPARATE_PART_FILES,
		AP214_SINGLE_FILE,
		AP214_SEPARATE_PART_FILES,
		ASCII,
		BINARY,
    };	

	 e_DataExchangeVersion	DataExchangeVersion_enum( 
		                         const string &in_DataExchangeVersion_string)
									throw (isis::application_exception);


	std::string DataExchangeVersion_string(  e_DataExchangeVersion in_DataExchangeVersion_enum)
											throw (isis::application_exception);


	//////////////////////////////////////////////////////////////////////////////////////////
	enum e_DataExchangeSpecialInstruction
    {
        DATA_EXCHANGE_LEAF_ASSEMBLIES_METRICS,
    };	

	 e_DataExchangeSpecialInstruction	DataExchangeSpecialInstruction_enum( 
		                         const string &in_DataExchangeSpecialInstruction_string)
									throw (isis::application_exception);


	std::string DataExchangeSpecialInstruction_string(  e_DataExchangeSpecialInstruction in_DataExchangeSpecialInstruction_enum)
											throw (isis::application_exception);

	//////////////////////////////////////////////////////////////////////////////////////////
	enum e_ComputationType
    {
        COMPUTATION_BOUNDING_BOX,  
		COMPUTATION_CG, 
        COMPUTATION_POINT,
		COMPUTATION_MASS,
		COMPUTATION_INTERFERENCE_COUNT,
		COMPUTATION_COEFFICIENT_OF_DRAG,
		COMPUTATION_PLANE,
		COMPUTATION_STRESS_MISES,
		COMPUTATION_STRESS_SHEAR,
		COMPUTATION_STRESS_BEARING,
		COMPUTATION_FACTOR_OF_SAFETY,
		COMPUTATION_TOTAL_SHOTS,
		COMPUTATION_TOTAL_INTERSECTIONS,
		COMPUTATION_TOTAL_PERFORATIONS,
		COMPUTATION_TOTAL_KILLS,
		MAX_VERTICAL_JUMP,
		MAX_VERTICAL_VELOCITY,
		MAX_HORIZONAL_VELOCITY,
		MINIMUM_TEMPERATURE,
		MAXIMUM_TEMPERATURE,
		TSAI_WU_FAILURE,
		HILL_FAILURE,
		HOFFMAN_FAILURE,
		MAXIMUM_FAILURE
    };	

	 e_ComputationType	ComputationType_enum( 
		                            const string &in_ComputationType_string)
									throw (isis::application_exception);


	std::string ComputationType_string(  e_ComputationType in_ComputationType_enum)
											throw (isis::application_exception);


	//////////////////////////////////////////////////////////////////////////////////////////
	enum e_ComputationSubType
    {
        COMPUTATION_SUBTYPE_GROUND,
		COMPUTATION_SUBTYPE_NONE
    };	

	 e_ComputationSubType	ComputationSubType_enum( 
		                            const string &in_ComputationSubType_string)
									throw (isis::application_exception);


	std::string ComputationSubType_string(  e_ComputationSubType in_ComputationSubType_enum)
											throw (isis::application_exception);

	//////////////////////////////////////////////////////////////////////////////////////////
	enum e_ComputationDimension
    {
        COMPUTATION_X_COORDINATE,  
		COMPUTATION_Y_COORDINATE, 
        COMPUTATION_Z_COORDINATE,
		COMPUTATION_VECTOR,
		COMPUTATION_SCALAR,
		COMPUTATION_BOOLEAN,
		COMPUTATION_POINTS,
		COMPUTATION_DIMENSION_NONE
    };	

	 e_ComputationDimension	ComputationDimension_enum( 
		                            const string &in_ComputationDimension_string)
									throw (isis::application_exception);


	std::string ComputationDimension_string(  e_ComputationDimension in_ComputationDimension_enum)
											throw (isis::application_exception);

	//////////////////////////////////////////////////////////////////////////////////////////
	enum e_GeometrySetOperation
    {
        CAD_GEOMETRY_SET_PLUS,  
		CAD_GEOMETRY_SET_MINUS,
		CAD_GEOMETRY_SET_INTERSECTION,
    };	

	 e_GeometrySetOperation	GeometrySetOperation_enum( 
		                            const string &in_GeometrySetOperation_string)
									throw (isis::application_exception);


	std::string GeometrySetOperation_string(  e_GeometrySetOperation in_GeometrySetOperation_enum)
											throw (isis::application_exception);
	//////////////////////////////////////////////////////////////////////////////////////////
	//enum e_CoordinateAlignment
    //{
     //   CAD_COORDINATE_ALIGNMENT_X,  
    //    CAD_COORDINATE_ALIGNMENT_Y,  
    //    CAD_COORDINATE_ALIGNMENT_Z,
    //    CAD_COORDINATE_ALIGNMENT_XY,  
    //    CAD_COORDINATE_ALIGNMENT_YZ,  
    //    CAD_COORDINATE_ALIGNMENT_ZX 
    //};	

	 ProDtmplnConstrType	CoordinatePlaneAlignment_enum( 
		                            const string &in_CoordinatePlaneAlignment_string)
									throw (isis::application_exception);


	std::string CoordinatePlaneAlignment_string(  ProDtmplnConstrType in_CoordinatePlaneAlignment_enum)
											throw (isis::application_exception);

    ProType CoordinateAxisAlignment_enum( const string &in_CoordinateAxisAlignment_string )
															throw (isis::application_exception);

	std::string CoordinateAxisAlignment_string(  ProType in_CoordinateAxisAlignment_enum)
											throw (isis::application_exception);


	//////////////////////////////////////////////////////////////////////////////////////////

	enum e_CADFileType
    {
        CAD_CREO,  
		CAD_STEP 
    };

	e_CADFileType CadFileType_enum( const string &in_CadFileType_string );


	std::string CadFileType_string(  e_CADFileType in_CadFileType_enum)
											throw (isis::application_exception);

	//////////////////////////////////////////////////////////////////////////////////////////

	enum e_CADMetricsError
    {
        CAD_MASS_PROPERTIES_NOT_DEFINED,  
		CAD_INTERIA_AT_DEFAULT_CSYS_NOT_DEFINED,
		CAD_INTERIA_AT_CENTER_OF_GRAVITY_NOT_DEFINED,
		CAD_SURFACE_AREA_NOT_DEFINED
    };

	e_CADMetricsError CADMetricsError_enum( const string &in_CADMetricsError_string );


	std::string CADMetricsError_string(  e_CADMetricsError in_CADMetricsError_enum)
											throw (isis::application_exception);

	//////////////////////////////////////////////////////////////////////////////////////////

	enum e_CADInertiaAt
    {
        CAD_DEFAULT_CSYS,  
		CAD_CENTER_OF_GRAVITY 
    };

	e_CADInertiaAt CADInertiaAt_enum( const string &in_CADInertiaAt_string );


	std::string CADInertiaAt_string(  e_CADInertiaAt in_CADInertiaAt_enum)
											throw (isis::application_exception);


	//////////////////////////////////////////////////////////////////////////////////////////

	enum e_ReferencePlane
    {
		PLANE_NONE,
        PLANE_GROUND,
		PLANE_WATERLINE

		//WATERLINE, Don't support waterline yet
    };

	e_ReferencePlane ReferencePlane_enum( const string &in_ReferencePlane_string );

	std::string ReferencePlane_string(  e_ReferencePlane in_ReferencePlane_enum)
											throw (isis::application_exception);

	//////////////////////////////////////////////////////////////////////////////////////////

	enum e_ComponentDefinitionSource
    {
		COMPONENT_DEFINITION_SOURCE_CYPHY,
        COMPONENT_DEFINITION_SOURCE_CAD_MODEL
    };

	e_ComponentDefinitionSource ComponentDefinitionSource_enum( const string &in_ComponentDefinitionSource_string );

	std::string ComponentDefinitionSource_string(  e_ComponentDefinitionSource in_ComponentDefinitionSource_enum)
											throw (isis::application_exception);


	//////////////////////////////////////////////////////////////////////////////////////////

	enum e_CADModelRepresentation
    {
		CAD_MODEL_FEATURED_REP,
        CAD_MODEL_DEFEATURED_REP
    };

	e_CADModelRepresentation CADModelRepresentation_enum( const string &in_CADModelRepresentation_string );

	std::string CADModelRepresentation_string(  e_CADModelRepresentation in_CADModelRepresentation_enum)
											throw (isis::application_exception);



	enum e_CADTreatConstraintAsAGuide
    {
        CAD_TREAT_CONSTRAINT_AS_A_GUIDE_FALSE,
		CAD_TREAT_CONSTRAINT_AS_A_GUIDE_TRUE,
    };

	e_CADTreatConstraintAsAGuide CADTreatConstraintAsAGuide_enum( const string &in_CADTreatConstraintAsAGuide_string );

	std::ostream& operator <<(std::ostream & out, const e_CADTreatConstraintAsAGuide & value) ;

	std::string CADTreatConstraintAsAGuide_string(  e_CADTreatConstraintAsAGuide in_CADTreatConstraintAsAGuide_enum)
											throw (isis::application_exception);

	//////////////////////////////////////////////////////////////////////////////////////////
	enum e_CADJointType
	{
		// Following is from Joint.h
		// FIXED = ooo,
		// REVOLUTE = ooi,
		// UNIVERSAL = oii,
		// SPHERICAL = iii,
		// PRISMATIC = oox,
		// PLANAR = xxi,
		// CYLINDRICAL = oof,
		// FREE = fff,

		FIXED_JOINT,
		REVOLUTE_JOINT,
		UNIVERSAL_JOINT,
		SPHERICAL_JOINT,
		PRISMATIC_JOINT,
		CYLINDRICAL_JOINT,
		PLANAR_JOINT,
		FREE_JOINT,
		UNKNOWN_JOINT_TYPE
	};

	e_CADJointType CADJointType_enum(const std::string &in_CADJointType_string)
												throw (isis::application_exception);

	std::string CADJointType_string(  e_CADJointType in_CADJointType_enum)
											throw (isis::application_exception);

	ProAsmcompSetType ProAsmcompSetType_enum(const std::string &in_ProAsmcompSetType_string)
												throw (isis::application_exception);

	std::string ProAsmcompSetType_string(  ProAsmcompSetType in_ProAsmcompSetType_enum)
											throw (isis::application_exception);

	enum e_ProcessingInstruction_Primary
	{
		COMPLETE_THE_HIERARCHY_FOR_LEAF_ASSEMBLIES,
		UNIQUELY_NAME_ALL_CAD_MODEL_INSTANCES,
		OUTPUT_JOINT_INFORMATION
	};
	e_ProcessingInstruction_Primary ProcessingInstruction_Primary_enum(const std::string &in_ProcessingInstruction_Primary_string)
												throw (isis::application_exception);
	 e_ProcessingInstruction_Primary ProcessingInstruction_Primary_enum(const std::string &in_ProcessingInstruction_Primary_string)
												throw (isis::application_exception);

	enum e_ProcessingInstruction_Secondary
	{
		PROCESSING_INSTRUCTION_SECONDARY_NONE,
		VALIDATE_JOINT_INFORMATION
	};
	e_ProcessingInstruction_Secondary ProcessingInstruction_Secondary_enum(const std::string &in_ProcessingInstruction_Secondary_string)
												throw (isis::application_exception);
	e_ProcessingInstruction_Secondary ProcessingInstruction_Secondary_enum(const std::string &in_ProcessingInstruction_Secondary_string)
												throw (isis::application_exception);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	enum e_Metrics_JointsComponentInstanceData_Primary
	{
		METRICS_JOINTS_HAS_KINEMATIC_JOINT,
		METRICS_JOINTS_LEAF_ASSEMBLY_NO_KINEMATIC_JOINTS,
		METRICS_JOINTS_ANCHOR_COMPONENT
	};
	e_Metrics_JointsComponentInstanceData_Primary 
		Metrics_JointsComponentInstanceData_Primary_enum(const std::string &in_Metrics_JointsComponentInstanceData_Primary_string)
												throw (isis::application_exception);
	std::string									
		Metrics_JointsComponentInstanceData_Primary_string(  e_Metrics_JointsComponentInstanceData_Primary in_Metrics_JointsComponentInstanceData_Primary_enum)
											throw (isis::application_exception);

	enum e_Metrics_JointsComponentInstanceData_Secondary
	{
		METRICS_JOINTS_NONE,
		METRICS_JOINTS_TREAT_AS_ONE_BODY,
		METRICS_JOINTS_TREAT_AS_SEPERATE_BODIES,

	};
	e_Metrics_JointsComponentInstanceData_Secondary 
		Metrics_JointsComponentInstanceData_Secondary_enum(const std::string &in_Metrics_JointsComponentInstanceData_Secondary_string)
												throw (isis::application_exception);

	std::string 
		Metrics_JointsComponentInstanceData_Secondary_string(  e_Metrics_JointsComponentInstanceData_Secondary in_Metrics_JointsComponentInstanceData_Secondary_enum)
											throw (isis::application_exception);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	enum e_Thermal_LoadConstraint
	{
		THERMAL_CONVECTION_HEAT,		// CONSTRAINT, Convection Coefficient with Ambient Temp 
		THERMAL_CONVECTION,				// Same as THERMAL_CONVECTION_HEAT, THERMAL_CONVECTION_HEAT is being deprecated
		THERMAL_HEAT_FLUX,				// LOAD
		THERMAL_HEAT_GENERATION,		// LOAD Not supported by deck-based
		THERMAL_INITIAL_TEMPERATURE,    // LOAD
		THERMAL_SPECIFIED_TEMPERATURE,
		THERMAL_AMBIENT_TEMPERATURE// LOAD Specified temperature at grid points

		/*
		ConvectionHeat
		HeatFlux
		HeatGeneration
		InitialTemperature
		SpecifiedTemperature
		*/
	};
	e_Thermal_LoadConstraint 
		Thermal_LoadConstraint_enum(const std::string &in_Thermal_LoadConstraint_string)
												throw (isis::application_exception);

	std::string 
		Thermal_LoadConstraint_string(  e_Thermal_LoadConstraint in_Thermal_LoadConstraint_enum)
											throw (isis::application_exception);
}  // end namespace isis

#endif