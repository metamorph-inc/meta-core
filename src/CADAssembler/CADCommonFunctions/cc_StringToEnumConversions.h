
// WARNING - DO NOT EDIT THIS FILE
// This file was auto generated by src\CADAssembler\CodeGenerationTools\enums\CreateCADEnums.bat.
// To edit, modify src\CADAssembler\CodeGenerationTools\enums\CreateCADEnums_InputFile.txt and run CreateCADEnums.bat.

#ifndef CC_STRING_TO_ENUM_CONVERSIONS_H
#define CC_STRING_TO_ENUM_CONVERSIONS_H

#include "isis_application_exception.h"
#include <string>

#pragma warning( disable : 4290 )  // a future feature : exception specification, i.e. throw

namespace isis
{

	////////////////////////////////////////////////////////////////////////////////////////

	enum e_FeatureInterfaceType
	{
		CAD_DATUM,
		CAD_MODEL_INTERFACE,
		CAD_MODEL_USER_DATA,
	};

	e_FeatureInterfaceType FeatureInterfaceType_enum( const std::string &in_String)
										throw (isis::application_exception);

	std::string FeatureInterfaceType_string( e_FeatureInterfaceType in_Enum )
										throw (isis::application_exception);

	////////////////////////////////////////////////////////////////////////////////////////

	enum e_CADParameterType
	{
		CAD_FLOAT,
		CAD_INTEGER,
		CAD_BOOLEAN,
		CAD_STRING,
	};

	e_CADParameterType CADParameterType_enum( const std::string &in_String)
										throw (isis::application_exception);

	std::string CADParameterType_string( e_CADParameterType in_Enum )
										throw (isis::application_exception);

	////////////////////////////////////////////////////////////////////////////////////////

	enum e_CADSpecialInstruction
	{
		CAD_SPECIAL_INSTRUCTION_NONE,
		CAD_SPECIAL_INSTRUCTION_SIZE_TO_FIT,
		CAD_SPECIAL_INSTRUCTION_HAS_KINEMATIC_JOINT,
	};

	e_CADSpecialInstruction SpecialInstruction_enum( const std::string &in_String)
										throw (isis::application_exception);

	std::string SpecialInstruction_string( e_CADSpecialInstruction in_Enum )
										throw (isis::application_exception);

	////////////////////////////////////////////////////////////////////////////////////////

	enum e_CADUnitsType
	{
		CAD_UNITS_DISTANCE,
		CAD_UNITS_AREA,
		CAD_UNITS_VOLUME,
		CAD_UNITS_DENSITY,
		CAD_UNITS_MASS,
		CAD_UNITS_FORCE,
		CAD_UNITS_MOMENT,
		CAD_UNITS_PRESSURE,
		CAD_UNITS_ACCELERATION,
		CAD_UNITS_HEAT_CAPACITY,
		CAD_UNITS_THERMAL_CONDUCTIVITY,
		CAD_UNITS_ANGLE,
		CAD_UNITS_TIME,
		CAD_UNITS_TEMPERATURE,
		CAD_UNITS_TYPE_NA,
	};

	e_CADUnitsType CADUnitsType_enum( const std::string &in_String)
										throw (isis::application_exception);

	std::string CADUnitsType_string( e_CADUnitsType in_Enum )
										throw (isis::application_exception);

	////////////////////////////////////////////////////////////////////////////////////////

	enum e_CADUnitsDistance
	{
		CAD_UNITS_MM,
		CAD_UNITS_CM,
		CAD_UNITS_M,
		CAD_UNITS_KM,
		CAD_UNITS_INCH,
		CAD_UNITS_FOOT,
		CAD_UNITS_DISTANCE_NA,
	};

	e_CADUnitsDistance CADUnitsDistance_enum( const std::string &in_String)
										throw (isis::application_exception);

	std::string CADUnitsDistance_string( e_CADUnitsDistance in_Enum )
										throw (isis::application_exception);

	////////////////////////////////////////////////////////////////////////////////////////

	enum e_CADUnitsArea
	{
		CAD_UNITS_MM_SQUARED,
		CAD_UNITS_CM_SQUARED,
		CAD_UNITS_INCH_SQUARED,
		CAD_UNITS_AREA_NA,
	};

	e_CADUnitsArea CADUnitsArea_enum( const std::string &in_String)
										throw (isis::application_exception);

	std::string CADUnitsArea_string( e_CADUnitsArea in_Enum )
										throw (isis::application_exception);

	////////////////////////////////////////////////////////////////////////////////////////

	enum e_CADUnitsVolume
	{
		CAD_UNITS_MM_CUBED,
		CAD_UNITS_CM_CUBED,
		CAD_UNITS_M_CUBED,
		CAD_UNITS_KM_CUBED,
		CAD_UNITS_INCH_CUBED,
		CAD_UNITS_FOOT_CUBED,
		CAD_UNITS_VOLUME_NA,
	};

	e_CADUnitsVolume CADUnitsVolume_enum( const std::string &in_String)
										throw (isis::application_exception);

	std::string CADUnitsVolume_string( e_CADUnitsVolume in_Enum )
										throw (isis::application_exception);

	////////////////////////////////////////////////////////////////////////////////////////

	enum e_CADUnitsMass
	{
		CAD_UNITS_LBM,
		CAD_UNITS_GRAM,
		CAD_UNITS_KGRAM,
		CAD_UNITS_TONNE,
		CAD_UNITS_MASS_NA,
	};

	e_CADUnitsMass CADUnitsMass_enum( const std::string &in_String)
										throw (isis::application_exception);

	std::string CADUnitsMass_string( e_CADUnitsMass in_Enum )
										throw (isis::application_exception);

	////////////////////////////////////////////////////////////////////////////////////////

	enum e_CADUnitsForce
	{
		CAD_UNITS_LBF,
		CAD_UNITS_N,
		CAD_UNITS_FORCE_NA,
	};

	e_CADUnitsForce CADUnitsForce_enum( const std::string &in_String)
										throw (isis::application_exception);

	std::string CADUnitsForce_string( e_CADUnitsForce in_Enum )
										throw (isis::application_exception);

	////////////////////////////////////////////////////////////////////////////////////////

	enum e_CADUnitsMoment
	{
		CAD_UNITS_FT_LBF,
		CAD_UNITS_IN_LBF,
		CAD_UNITS_N_M,
		CAD_UNITS_N_MM,
	};

	e_CADUnitsMoment CADUnitsMoment_enum( const std::string &in_String)
										throw (isis::application_exception);

	std::string CADUnitsMoment_string( e_CADUnitsMoment in_Enum )
										throw (isis::application_exception);

	////////////////////////////////////////////////////////////////////////////////////////

	enum e_CADUnitsPressure
	{
		CAD_UNITS_MPA,
		CAD_UNITS_LBF_PER_INCH_SQ,
		CAD_UNITS_PA,
	};

	e_CADUnitsPressure CADUnitsPressure_enum( const std::string &in_String)
										throw (isis::application_exception);

	std::string CADUnitsPressure_string( e_CADUnitsPressure in_Enum )
										throw (isis::application_exception);

	////////////////////////////////////////////////////////////////////////////////////////

	enum e_CADUnitsAcceleration
	{
		CAD_UNITS_FT_PER_SEC_SQ,
		CAD_UNITS_MM_PER_SEC_SQ,
		CAD_UNITS_M_PER_SEC_SQ,
	};

	e_CADUnitsAcceleration CADUnitsAcceleration_enum( const std::string &in_String)
										throw (isis::application_exception);

	std::string CADUnitsAcceleration_string( e_CADUnitsAcceleration in_Enum )
										throw (isis::application_exception);

	////////////////////////////////////////////////////////////////////////////////////////

	enum e_CADUnitsDensity
	{
		CAD_UNITS_KG_PER_MM_CUBED,
		CAD_UNITS_KG_PER_M_CUBED,
	};

	e_CADUnitsDensity CADUnitsDensity_enum( const std::string &in_String)
										throw (isis::application_exception);

	std::string CADUnitsDensity_string( e_CADUnitsDensity in_Enum )
										throw (isis::application_exception);

	////////////////////////////////////////////////////////////////////////////////////////

	enum e_CADUnitsTemperature
	{
		CAD_UNITS_CELSIUS,
		CAD_UNITS_FAHRENHEIT,
		CAD_UNITS_KELVIN,
		CAD_UNITS_TEMPERATURE_NA,
	};

	e_CADUnitsTemperature CADUnitsTemperature_enum( const std::string &in_String)
										throw (isis::application_exception);

	std::string CADUnitsTemperature_string( e_CADUnitsTemperature in_Enum )
										throw (isis::application_exception);

	////////////////////////////////////////////////////////////////////////////////////////

	enum e_CADUnitsHeatCapacity
	{
		CAD_UNITS_J_PER_KG_K,
	};

	e_CADUnitsHeatCapacity CADUnitsHeatCapacity_enum( const std::string &in_String)
										throw (isis::application_exception);

	std::string CADUnitsHeatCapacity_string( e_CADUnitsHeatCapacity in_Enum )
										throw (isis::application_exception);

	////////////////////////////////////////////////////////////////////////////////////////

	enum e_CADUnitsThermalConductivity
	{
		CAD_UNITS_W_PER_MM_K,
	};

	e_CADUnitsThermalConductivity CADUnitsThermalConductivity_enum( const std::string &in_String)
										throw (isis::application_exception);

	std::string CADUnitsThermalConductivity_string( e_CADUnitsThermalConductivity in_Enum )
										throw (isis::application_exception);

	////////////////////////////////////////////////////////////////////////////////////////

	enum e_CADUnitsAngle
	{
		CAD_UNITS_RADIAN,
		CAD_UNITS_DEGREE,
		CAD_UNITS_ANGLE_NA,
	};

	e_CADUnitsAngle CADUnitsAngle_enum( const std::string &in_String)
										throw (isis::application_exception);

	std::string CADUnitsAngle_string( e_CADUnitsAngle in_Enum )
										throw (isis::application_exception);

	////////////////////////////////////////////////////////////////////////////////////////

	enum e_CADUnitsTime
	{
		CAD_UNITS_SECOND,
		CAD_UNITS_HOUR,
		CAD_UNITS_TIME_NA,
	};

	e_CADUnitsTime CADUnitsTime_enum( const std::string &in_String)
										throw (isis::application_exception);

	std::string CADUnitsTime_string( e_CADUnitsTime in_Enum )
										throw (isis::application_exception);

	////////////////////////////////////////////////////////////////////////////////////////

	enum e_AnalysisType
	{
		ANALYSIS_STRUCTURAL,
		ANALYSIS_BUCKLING,
		ANALYSIS_MODAL,
		ANALYSIS_THERMAL,
		ANALYSIS_FATIGUE,
	};

	e_AnalysisType AnalysisType_enum( const std::string &in_String)
										throw (isis::application_exception);

	std::string AnalysisType_string( e_AnalysisType in_Enum )
										throw (isis::application_exception);

	////////////////////////////////////////////////////////////////////////////////////////

	enum e_AnalysisPartInterfaceType
	{
		ANALYSIS_PART_INTERFACE_FREE,
		ANALYSIS_PART_INTERFACE_CONTACTING,
		ANALYSIS_PART_INTERFACE_BONDED,
	};

	e_AnalysisPartInterfaceType AnalysisPartInterfaceType_enum( const std::string &in_String)
										throw (isis::application_exception);

	std::string AnalysisPartInterfaceType_string( e_AnalysisPartInterfaceType in_Enum )
										throw (isis::application_exception);

	////////////////////////////////////////////////////////////////////////////////////////

	enum e_AnalysisSolutionType
	{
		ANALYSIS_DECK_BASED,
		ANALYSIS_MODEL_BASED,
	};

	e_AnalysisSolutionType AnalysisSolutionType_enum( const std::string &in_String)
										throw (isis::application_exception);

	std::string AnalysisSolutionType_string( e_AnalysisSolutionType in_Enum )
										throw (isis::application_exception);

	////////////////////////////////////////////////////////////////////////////////////////

	enum e_AnalysisConstraintFixedFree
	{
		ANALYSIS_CONSTRAINT_SCALAR,
		ANALYSIS_CONSTRAINT_FIXED,
		ANALYSIS_CONSTRAINT_FREE,
	};

	e_AnalysisConstraintFixedFree AnalysisConstraintFixedFree_enum( const std::string &in_String)
										throw (isis::application_exception);

	std::string AnalysisConstraintFixedFree_string( e_AnalysisConstraintFixedFree in_Enum )
										throw (isis::application_exception);

	////////////////////////////////////////////////////////////////////////////////////////

	enum e_CADGeometryType
	{
		CAD_GEOMETRY_POLYGON,
		CAD_GEOMETRY_CIRCLE,
		CAD_GEOMETRY_CONCENTRIC_CIRCLES,
		CAD_GEOMETRY_CYLINDER,
		CAD_GEOMETRY_SPHERE,
		CAD_GEOMETRY_EXTRUSION,
		CAD_GEOMETRY_FACE,
		CAD_GEOMETRY_NONE,
	};

	e_CADGeometryType CADGeometryType_enum( const std::string &in_String)
										throw (isis::application_exception);

	std::string CADGeometryType_string( e_CADGeometryType in_Enum )
										throw (isis::application_exception);

	////////////////////////////////////////////////////////////////////////////////////////

	enum e_CADAnalysisFeatureGeometryType
	{
		CAD_ANALYSIS_FEATURE_GEOMETRY_POINT,
	};

	e_CADAnalysisFeatureGeometryType CADAnalysisFeatureGeometryType_enum( const std::string &in_String)
										throw (isis::application_exception);

	std::string CADAnalysisFeatureGeometryType_string( e_CADAnalysisFeatureGeometryType in_Enum )
										throw (isis::application_exception);

	////////////////////////////////////////////////////////////////////////////////////////

	enum e_CADPrimaryGeometryQualifier
	{
		CAD_INTERIOR_ONLY,
		CAD_BOUNDARY_ONLY,
		CAD_INTERIOR_AND_BOUNDARY,
	};

	e_CADPrimaryGeometryQualifier CADPrimaryGeometryQualifier_enum( const std::string &in_String)
										throw (isis::application_exception);

	std::string CADPrimaryGeometryQualifier_string( e_CADPrimaryGeometryQualifier in_Enum )
										throw (isis::application_exception);

	////////////////////////////////////////////////////////////////////////////////////////

	enum e_CADSecondaryGeometryQualifier
	{
		CAD_INCLUDE_END_CAPS,
		CAD_EXCLUDE_END_CAPS,
		CAD_SECONDARY_GEOMETRY_QUALIFIER_NONE,
	};

	e_CADSecondaryGeometryQualifier CADSecondaryGeometryQualifier_enum( const std::string &in_String)
										throw (isis::application_exception);

	std::string CADSecondaryGeometryQualifier_string( e_CADSecondaryGeometryQualifier in_Enum )
										throw (isis::application_exception);

	////////////////////////////////////////////////////////////////////////////////////////

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
		MATERIAL_FLUID,
	};

	e_MaterialType MaterialType_enum( const std::string &in_String)
										throw (isis::application_exception);

	std::string MaterialType_string( e_MaterialType in_Enum )
										throw (isis::application_exception);

	////////////////////////////////////////////////////////////////////////////////////////

	enum e_DataExchangeFormat
	{
		DATA_EXCHANGE_FORMAT_STEP,
		DATA_EXCHANGE_FORMAT_STEREOLITHOGRAPHY,
		DATA_EXCHANGE_FORMAT_INVENTOR,
		DATA_EXCHANGE_FORMAT_PARASOLID,
		DATA_EXCHANGE_DXF,
	};

	e_DataExchangeFormat DataExchangeFormat_enum( const std::string &in_String)
										throw (isis::application_exception);

	std::string DataExchangeFormat_string( e_DataExchangeFormat in_Enum )
										throw (isis::application_exception);

	////////////////////////////////////////////////////////////////////////////////////////

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
		Y2013,
	};

	e_DataExchangeVersion DataExchangeVersion_enum( const std::string &in_String)
										throw (isis::application_exception);

	std::string DataExchangeVersion_string( e_DataExchangeVersion in_Enum )
										throw (isis::application_exception);

	////////////////////////////////////////////////////////////////////////////////////////

	enum e_DataExchangeSpecialInstruction
	{
		DATA_EXCHANGE_LEAF_ASSEMBLIES_METRICS,
	};

	e_DataExchangeSpecialInstruction DataExchangeSpecialInstruction_enum( const std::string &in_String)
										throw (isis::application_exception);

	std::string DataExchangeSpecialInstruction_string( e_DataExchangeSpecialInstruction in_Enum )
										throw (isis::application_exception);

	////////////////////////////////////////////////////////////////////////////////////////

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
		MAXIMUM_FAILURE,
	};

	e_ComputationType ComputationType_enum( const std::string &in_String)
										throw (isis::application_exception);

	std::string ComputationType_string( e_ComputationType in_Enum )
										throw (isis::application_exception);

	////////////////////////////////////////////////////////////////////////////////////////

	enum e_ComputationSubType
	{
		COMPUTATION_SUBTYPE_GROUND,
		COMPUTATION_SUBTYPE_NONE,
	};

	e_ComputationSubType ComputationSubType_enum( const std::string &in_String)
										throw (isis::application_exception);

	std::string ComputationSubType_string( e_ComputationSubType in_Enum )
										throw (isis::application_exception);

	////////////////////////////////////////////////////////////////////////////////////////

	enum e_ComputationDimension
	{
		COMPUTATION_X_COORDINATE,
		COMPUTATION_Y_COORDINATE,
		COMPUTATION_Z_COORDINATE,
		COMPUTATION_VECTOR,
		COMPUTATION_SCALAR,
		COMPUTATION_BOOLEAN,
		COMPUTATION_POINTS,
		COMPUTATION_DIMENSION_NONE,
	};

	e_ComputationDimension ComputationDimension_enum( const std::string &in_String)
										throw (isis::application_exception);

	std::string ComputationDimension_string( e_ComputationDimension in_Enum )
										throw (isis::application_exception);

	////////////////////////////////////////////////////////////////////////////////////////

	enum e_GeometrySetOperation
	{
		CAD_GEOMETRY_SET_PLUS,
		CAD_GEOMETRY_SET_MINUS,
		CAD_GEOMETRY_SET_INTERSECTION,
	};

	e_GeometrySetOperation GeometrySetOperation_enum( const std::string &in_String)
										throw (isis::application_exception);

	std::string GeometrySetOperation_string( e_GeometrySetOperation in_Enum )
										throw (isis::application_exception);

	////////////////////////////////////////////////////////////////////////////////////////

	enum e_CADFileType
	{
		CAD_CREO,
		CAD_STEP,
	};

	e_CADFileType CADFileType_enum( const std::string &in_String)
										throw (isis::application_exception);

	std::string CADFileType_string( e_CADFileType in_Enum )
										throw (isis::application_exception);

	////////////////////////////////////////////////////////////////////////////////////////

	enum e_CADMetricsError
	{
		CAD_MASS_PROPERTIES_NOT_DEFINED,
		CAD_INTERIA_AT_DEFAULT_CSYS_NOT_DEFINED,
		CAD_INTERIA_AT_CENTER_OF_GRAVITY_NOT_DEFINED,
		CAD_SURFACE_AREA_NOT_DEFINED,
	};

	e_CADMetricsError CADMetricsError_enum( const std::string &in_String)
										throw (isis::application_exception);

	std::string CADMetricsError_string( e_CADMetricsError in_Enum )
										throw (isis::application_exception);

	////////////////////////////////////////////////////////////////////////////////////////

	enum e_CADInertiaAt
	{
		CAD_DEFAULT_CSYS,
		CAD_CENTER_OF_GRAVITY,
	};

	e_CADInertiaAt CADInertiaAt_enum( const std::string &in_String)
										throw (isis::application_exception);

	std::string CADInertiaAt_string( e_CADInertiaAt in_Enum )
										throw (isis::application_exception);

	////////////////////////////////////////////////////////////////////////////////////////

	enum e_ReferencePlane
	{
		PLANE_NONE,
		PLANE_GROUND,
		PLANE_WATERLINE,
	};

	e_ReferencePlane ReferencePlane_enum( const std::string &in_String)
										throw (isis::application_exception);

	std::string ReferencePlane_string( e_ReferencePlane in_Enum )
										throw (isis::application_exception);

	////////////////////////////////////////////////////////////////////////////////////////

	enum e_ComponentDefinitionSource
	{
		COMPONENT_DEFINITION_SOURCE_CYPHY,
		COMPONENT_DEFINITION_SOURCE_CAD_MODEL,
	};

	e_ComponentDefinitionSource ComponentDefinitionSource_enum( const std::string &in_String)
										throw (isis::application_exception);

	std::string ComponentDefinitionSource_string( e_ComponentDefinitionSource in_Enum )
										throw (isis::application_exception);

	////////////////////////////////////////////////////////////////////////////////////////

	enum e_CADModelRepresentation
	{
		CAD_MODEL_FEATURED_REP,
		CAD_MODEL_DEFEATURED_REP,
	};

	e_CADModelRepresentation CADModelRepresentation_enum( const std::string &in_String)
										throw (isis::application_exception);

	std::string CADModelRepresentation_string( e_CADModelRepresentation in_Enum )
										throw (isis::application_exception);

	////////////////////////////////////////////////////////////////////////////////////////

	enum e_CADTreatConstraintAsAGuide
	{
		CAD_TREAT_CONSTRAINT_AS_A_GUIDE_FALSE,
		CAD_TREAT_CONSTRAINT_AS_A_GUIDE_TRUE,
	};

	e_CADTreatConstraintAsAGuide CADTreatConstraintAsAGuide_enum( const std::string &in_String)
										throw (isis::application_exception);

	std::string CADTreatConstraintAsAGuide_string( e_CADTreatConstraintAsAGuide in_Enum )
										throw (isis::application_exception);

	////////////////////////////////////////////////////////////////////////////////////////

	enum e_CADJointType
	{
		FIXED_JOINT,
		REVOLUTE_JOINT,
		UNIVERSAL_JOINT,
		SPHERICAL_JOINT,
		PRISMATIC_JOINT,
		CYLINDRICAL_JOINT,
		PLANAR_JOINT,
		FREE_JOINT,
		UNKNOWN_JOINT_TYPE,
	};

	e_CADJointType CADJointType_enum( const std::string &in_String)
										throw (isis::application_exception);

	std::string CADJointType_string( e_CADJointType in_Enum )
										throw (isis::application_exception);

	////////////////////////////////////////////////////////////////////////////////////////

	enum e_ProcessingInstruction_Primary
	{
		COMPLETE_THE_HIERARCHY_FOR_LEAF_ASSEMBLIES,
		UNIQUELY_NAME_ALL_CAD_MODEL_INSTANCES,
		OUTPUT_JOINT_INFORMATION,
	};

	e_ProcessingInstruction_Primary ProcessingInstruction_Primary_enum( const std::string &in_String)
										throw (isis::application_exception);

	std::string ProcessingInstruction_Primary_string( e_ProcessingInstruction_Primary in_Enum )
										throw (isis::application_exception);

	////////////////////////////////////////////////////////////////////////////////////////

	enum e_ProcessingInstruction_Secondary
	{
		PROCESSING_INSTRUCTION_SECONDARY_NONE,
		VALIDATE_JOINT_INFORMATION,
	};

	e_ProcessingInstruction_Secondary ProcessingInstruction_Secondary_enum( const std::string &in_String)
										throw (isis::application_exception);

	std::string ProcessingInstruction_Secondary_string( e_ProcessingInstruction_Secondary in_Enum )
										throw (isis::application_exception);

	////////////////////////////////////////////////////////////////////////////////////////

	enum e_Metrics_JointsComponentInstanceData_Primary
	{
		METRICS_JOINTS_HAS_KINEMATIC_JOINT,
		METRICS_JOINTS_LEAF_ASSEMBLY_NO_KINEMATIC_JOINTS,
		METRICS_JOINTS_ANCHOR_COMPONENT,
	};

	e_Metrics_JointsComponentInstanceData_Primary Metrics_JointsComponentInstanceData_Primary_enum( const std::string &in_String)
										throw (isis::application_exception);

	std::string Metrics_JointsComponentInstanceData_Primary_string( e_Metrics_JointsComponentInstanceData_Primary in_Enum )
										throw (isis::application_exception);

	////////////////////////////////////////////////////////////////////////////////////////

	enum e_Metrics_JointsComponentInstanceData_Secondary
	{
		METRICS_JOINTS_NONE,
		METRICS_JOINTS_TREAT_AS_ONE_BODY,
		METRICS_JOINTS_TREAT_AS_SEPERATE_BODIES,
	};

	e_Metrics_JointsComponentInstanceData_Secondary Metrics_JointsComponentInstanceData_Secondary_enum( const std::string &in_String)
										throw (isis::application_exception);

	std::string Metrics_JointsComponentInstanceData_Secondary_string( e_Metrics_JointsComponentInstanceData_Secondary in_Enum )
										throw (isis::application_exception);

	////////////////////////////////////////////////////////////////////////////////////////

	enum e_Thermal_LoadConstraint
	{
		THERMAL_CONVECTION_HEAT,
		THERMAL_CONVECTION,
		THERMAL_HEAT_FLUX,
		THERMAL_HEAT_GENERATION,
		THERMAL_INITIAL_TEMPERATURE,
		THERMAL_SPECIFIED_TEMPERATURE,
		THERMAL_AMBIENT_TEMPERATURE,
	};

	e_Thermal_LoadConstraint Thermal_LoadConstraint_enum( const std::string &in_String)
										throw (isis::application_exception);

	std::string Thermal_LoadConstraint_string( e_Thermal_LoadConstraint in_Enum )
										throw (isis::application_exception);

	////////////////////////////////////////////////////////////////////////////////////////

	enum e_CADAssemblyConstraintType
	{
		CAD_ASM_MATE,
		CAD_ASM_MATE_OFF,
		CAD_ASM_ALIGN,
		CAD_ASM_ALIGN_OFF,
		CAD_ASM_CSYS,
	};

	e_CADAssemblyConstraintType CADAssemblyConstraintType_enum( const std::string &in_String)
										throw (isis::application_exception);

	std::string CADAssemblyConstraintType_string( e_CADAssemblyConstraintType in_Enum )
										throw (isis::application_exception);

	////////////////////////////////////////////////////////////////////////////////////////

	enum e_CADFeatureGeometryType
	{
		CAD_SURFACE,
		CAD_DATUM_PLANE,
		CAD_AXIS,
		CAD_POINT,
		CAD_CSYS,
		CAD_PART,
		CAD_ASSEMBLY,
		CAD_FEATURE,
		CAD_EDGE,
		CAD_FEATURE_GEOMETRY_TYPE_INVALID,
	};

	e_CADFeatureGeometryType CADFeatureGeometryType_enum( const std::string &in_String)
										throw (isis::application_exception);

	std::string CADFeatureGeometryType_string( e_CADFeatureGeometryType in_Enum )
										throw (isis::application_exception);

	////////////////////////////////////////////////////////////////////////////////////////

	enum e_CADDatumside
	{
		CAD_DATUM_SIDE_NONE,
		CAD_DATUM_SIDE_YELLOW,
		CAD_DATUM_SIDE_RED,
	};

	e_CADDatumside CADDatumside_enum( const std::string &in_String)
										throw (isis::application_exception);

	std::string CADDatumside_string( e_CADDatumside in_Enum )
										throw (isis::application_exception);

	////////////////////////////////////////////////////////////////////////////////////////

	enum e_CADAnalysisSolverType
	{
		CAD_FEM_FEAS_NASTRAN,
		CAD_FEM_FEAS_ANSYS,
		CAD_FEM_FEAS_ABAQUS,
		CAD_FEM_FEAS_PATRAN,
	};

	e_CADAnalysisSolverType CADAnalysisSolverType_enum( const std::string &in_String)
										throw (isis::application_exception);

	std::string CADAnalysisSolverType_string( e_CADAnalysisSolverType in_Enum )
										throw (isis::application_exception);

	////////////////////////////////////////////////////////////////////////////////////////

	enum e_CADAnalysisMeshType
	{
		CAD_FEM_SOLID_MESH,
		CAD_FEM_SHELL_MESH,
		CAD_FEM_MIXED_MESH,
		CAD_FEM_QUILT_MESH,
		CAD_FEM_BOUNDARY_MESH,
		CAD_FEM_BAR_MESH,
	};

	e_CADAnalysisMeshType CADAnalysisMeshType_enum( const std::string &in_String)
										throw (isis::application_exception);

	std::string CADAnalysisMeshType_string( e_CADAnalysisMeshType in_Enum )
										throw (isis::application_exception);

	////////////////////////////////////////////////////////////////////////////////////////

	enum e_CADAnalysisShellElementType
	{
		CAD_FEM_SHELL_MESH_TRIANGLE,
		CAD_FEM_SHELL_MESH_QUADRANGLE,
	};

	e_CADAnalysisShellElementType CADAnalysisShellElementType_enum( const std::string &in_String)
										throw (isis::application_exception);

	std::string CADAnalysisShellElementType_string( e_CADAnalysisShellElementType in_Enum )
										throw (isis::application_exception);

	////////////////////////////////////////////////////////////////////////////////////////

	enum e_CADAnalysisElementShapeType
	{
		CAD_FEM_MIDPNT_LINEAR,
		CAD_FEM_MIDPNT_PARABOLIC,
		CAD_FEM_MIDPNT_PARABOLIC_FIXED,
	};

	e_CADAnalysisElementShapeType CADAnalysisElementShapeType_enum( const std::string &in_String)
										throw (isis::application_exception);

	std::string CADAnalysisElementShapeType_string( e_CADAnalysisElementShapeType in_Enum )
										throw (isis::application_exception);

	////////////////////////////////////////////////////////////////////////////////////////

	enum e_CADMdlType
	{
		CAD_MDL_UNUSED,
		CAD_MDL_ASSEMBLY,
		CAD_MDL_PART,
	};

	e_CADMdlType CADMdlType_enum( const std::string &in_String)
										throw (isis::application_exception);

	std::string CADMdlType_string( e_CADMdlType in_Enum )
										throw (isis::application_exception);

} // End Namespace
#endif