/*! \file CreoStringToEnumConversions.h
    \brief  Functions to convert between strings and enums.

	The input/output XML files that are used by the CAD applications 
	do not persist enums for known values.  They instead persist strings, 
	which for efficiency reasons are converted to enums.  This file provides 
	the conversion routines.
*/
#ifndef CREO_STRING_TO_ENUM_CONVERSIONS_H
#define CREO_STRING_TO_ENUM_CONVERSIONS_H

#pragma warning( disable : 4290 )  // a future feature : exception specification, i.e. throw

#include <isis_application_exception.h>
#include <isis_include_ptc_headers.h>
#include <string>
#include <iostream>
#include <CADStringToEnumConversions.h>

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

	ProType FeatureGeometryType_enum(  e_CADFeatureGeometryType in_Enum)
											throw (isis::application_exception);

  	std::string FeatureGeometryType_string( e_CADFeatureGeometryType in_Enum)
											throw (isis::application_exception);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
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

	ProAsmcompConstrType ProAsmcompConstrType_enum( 
		                         e_CADAssemblyConstraintType in_CADAssemblyConstraintType_enum)
								 throw (isis::application_exception);

	std::string ProAsmcompConstrType_string(  ProAsmcompConstrType in_ProAsmcompConstrType_enum)
											throw (isis::application_exception);

	std::string ProAsmcompConstrType_string( e_CADAssemblyConstraintType in_CADAssemblyConstraintType_enum)
											throw (isis::application_exception);

	//////////////////////////////////////////////////////////////////////////////////////////////////////

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

		ProBoolean String_To_ProBoolean( 
		                         const std::string &in_Boolean_string)
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
		                            const std::string &in_CoordinatePlaneAlignment_string)
									throw (isis::application_exception);


	std::string CoordinatePlaneAlignment_string(  ProDtmplnConstrType in_CoordinatePlaneAlignment_enum)
											throw (isis::application_exception);

    ProType CoordinateAxisAlignment_enum( const std::string &in_CoordinateAxisAlignment_string )
															throw (isis::application_exception);

	std::string CoordinateAxisAlignment_string(  ProType in_CoordinateAxisAlignment_enum)
											throw (isis::application_exception);


	ProAsmcompSetType ProAsmcompSetType_enum(const std::string &in_ProAsmcompSetType_string)
												throw (isis::application_exception);

	std::string ProAsmcompSetType_string(  ProAsmcompSetType in_ProAsmcompSetType_enum)
											throw (isis::application_exception);

}

#endif