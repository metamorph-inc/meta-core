#include <CreoStringToEnumConversions.h>
#include <CommonUtilities.h>
#include <CADCommonConstants.h>
#include <sstream>

namespace isis
{
	std::ostream& operator <<(std::ostream & out, const bool & value) 
	{
	    out << Bool_string(value);
		return out;
	}

	std::string  Bool_string( bool in_Bool )
	{
		if ( in_Bool )
			return "TRUE";
		else
			return "FALSE";
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	  ProMdlType ProMdlType_enum(  std::string in_ProMdlType_string)
												throw (isis::application_exception)
	  {

		std::string ProMdlType_string = ConvertToUpperCase(in_ProMdlType_string);

		  if ( ProMdlType_string.compare("PRO_MDL_UNUSED") == 0 )
				return PRO_MDL_UNUSED;
		  else if ( ProMdlType_string.compare("PRO_MDL_ASSEMBLY") == 0 )
				return PRO_MDL_ASSEMBLY;
		  else if ( ProMdlType_string.compare("ASSEMBLY") == 0 )
				return PRO_MDL_ASSEMBLY;
		  if ( ProMdlType_string.compare("PRO_MDL_PART") == 0 )
				return PRO_MDL_PART;
		  if ( ProMdlType_string.compare("PART") == 0 )
				return PRO_MDL_PART;
		  else if ( ProMdlType_string.compare("PRO_MDL_DRAWING") == 0 )
				return PRO_MDL_DRAWING;
		  if ( ProMdlType_string.compare("PRO_MDL_3DSECTION") == 0 )
				return PRO_MDL_3DSECTION;
		  else if ( ProMdlType_string.compare("PRO_MDL_2DSECTION") == 0 )
				return PRO_MDL_2DSECTION;
		  if ( ProMdlType_string.compare("PRO_MDL_LAYOUT") == 0 )
				return PRO_MDL_LAYOUT;
		  else if ( ProMdlType_string.compare("PRO_MDL_DWGFORM") == 0 )
				return PRO_MDL_DWGFORM;
		  if ( ProMdlType_string.compare("PRO_MDL_MFG") == 0 )
				return PRO_MDL_MFG;
		  else if ( ProMdlType_string.compare("PRO_MDL_REPORT") == 0 )
				return PRO_MDL_REPORT;
		  if ( ProMdlType_string.compare("PRO_MDL_MARKUP") == 0 )
				return PRO_MDL_MARKUP;
		  else if ( ProMdlType_string.compare("PRO_MDL_DIAGRAM") == 0 )
				return PRO_MDL_DIAGRAM;

		 
		  string temp_string = "Function ProMdlType_enum was passed '" + in_ProMdlType_string + "' which is an erroneous type. Allowed types are: PART or ASSEMBLY.";
		  throw isis::application_exception(temp_string.c_str());
	  }

	std::string ProMdlType_string(  ProMdlType in_ProMdlType_enum)
											throw (isis::application_exception)
	{
		switch (in_ProMdlType_enum )
		{
			case PRO_MDL_UNUSED:
				return "PRO_MDL_UNUSED,";
				break;
			case PRO_MDL_ASSEMBLY:
				return "PRO_MDL_ASSEMBLY";
				break;
			case PRO_MDL_PART:
				return "PRO_MDL_PART";
				break;
			case PRO_MDL_DRAWING:
				return "PRO_MDL_DRAWING";
				break;
			case PRO_MDL_3DSECTION:
				return "PRO_MDL_3DSECTION";
				break;
			case PRO_MDL_2DSECTION:
				return "PRO_MDL_2DSECTION";
				break;
			case PRO_MDL_LAYOUT:
				return "PRO_MDL_LAYOUT";
				break;
			case PRO_MDL_DWGFORM:
				return "PRO_MDL_DWGFORM";
				break;
			case PRO_MDL_MFG:
				return "PRO_MDL_MFG";
				break;
			case PRO_MDL_REPORT:
				return "PRO_MDL_REPORT";
				break;
			case PRO_MDL_MARKUP:
				return "PRO_MDL_MARKUP";				
				break;
			case PRO_MDL_DIAGRAM:
				return "PRO_MDL_DIAGRAM";
				break;
			default:
				char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
				string temp_string = "Function ProModelType_string was passed " + 
					std::string(_itoa(in_ProMdlType_enum, temp_char_array, 10)) + 
					" which is an erroneous type.";
				throw isis::application_exception(temp_string.c_str());
	  }

  }

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

  ProType FeatureGeometryType_enum( string in_FeatureGeometryType_string)
										throw (isis::application_exception)
  {

	  std::string FeatureGeometryType_string = ConvertToUpperCase(in_FeatureGeometryType_string);

	  if ( FeatureGeometryType_string.compare("SURFACE") == 0 )
		  return PRO_SURFACE;
	  else if ( FeatureGeometryType_string.compare("PLANE") == 0 )  // Not valid for CADCreoParametricCreateAssembly.exe
		  return PRO_SURFACE;										// Only valid for CADCreoParametricDatumEditor.exe, 
																	// Datum Plane is treated as PRO_SURFACE by Creo
	  else if ( FeatureGeometryType_string.compare("AXIS") == 0 )
			return PRO_AXIS;
	  else if ( FeatureGeometryType_string.compare("POINT") == 0 )
			return PRO_POINT;
	  else if ( FeatureGeometryType_string.compare("CSYS") == 0 )
			return PRO_CSYS;
	  else if ( FeatureGeometryType_string.compare("PART") == 0 )
			return PRO_PART;
	  else if ( FeatureGeometryType_string.compare("ASSEMBLY") == 0 )
			return PRO_ASSEMBLY;
	  else if ( FeatureGeometryType_string.compare("FEATURE") == 0 )
			return PRO_FEATURE;
	  else if ( FeatureGeometryType_string.compare("EDGE") == 0 )
			return PRO_EDGE;


	  string temp_string = "Function FeatureGeometryType_enum was passed: '" + in_FeatureGeometryType_string + "' which is an erroneous type. Allowed types are: SURFACE, PLANE, AXIS, POINT or CSYS.";
	  throw isis::application_exception(temp_string.c_str());
  }

 	std::string FeatureGeometryType_string(  ProType in_FeatureGeometryType_enum)
											throw (isis::application_exception)
	{
		
		switch (in_FeatureGeometryType_enum )
		{
			case PRO_SURFACE:
				return "SURFACE";
				break;
			case PRO_DATUM_PLANE:
				return "PLANE";				// Not valid for CADCreoParametricCreateAssembly.exe
				break;						// Only valid for CADCreoParametricDatumEditor.exe
			case PRO_AXIS:
				return "AXIS";
				break;
			case PRO_POINT:
				return "POINT";
				break;
			case PRO_CSYS:
				return "CSYS";
				break;
			case PRO_PART:
				return "PART";
				break;
			case PRO_ASSEMBLY:
				return "ASSEMBLY";
				break;
			case PRO_FEATURE:
				return "FEATURE";
				break;
			case PRO_EDGE:
				return "EDGE";
				break;

			default:
				char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
				string temp_string = "Function FeatureGeometryType_string was passed " + 
					std::string(_itoa(in_FeatureGeometryType_enum,temp_char_array, 10)) + 
					" which is an erroneous type.  Allowed enum values are PRO_POINT, PRO_SURFACE, PRO_AXIS, PRO_DATUM_PLANE, PRO_PART, PRO_ASSEMBLY, PRO_FEATURE, and PRO_EDGE.";
				throw isis::application_exception(temp_string.c_str());
	  }
	}

 	ProType FeatureGeometryType_enum(  e_CADFeatureGeometryType in_Enum)
											throw (isis::application_exception)
	{
		
		switch (in_Enum )
		{
			case CAD_SURFACE:
				return PRO_SURFACE;
				break;
			case CAD_DATUM_PLANE:
				return  PRO_DATUM_PLANE;			// Not valid for CADCreoParametricCreateAssembly.exe
				break;								// Only valid for CADCreoParametricDatumEditor.exe
			case CAD_AXIS:
				return PRO_AXIS;
				break;
			case CAD_POINT:
				return PRO_POINT;
				break;
			case CAD_CSYS:
				return PRO_CSYS;
				break;
			case CAD_PART:
				return PRO_PART;
				break;
			case CAD_ASSEMBLY:
				return PRO_ASSEMBLY;
				break;
			case CAD_FEATURE:
				return PRO_FEATURE;
				break;
			case CAD_EDGE:
				return PRO_EDGE;
				break;

			default:
				std::stringstream errorString;
				errorString << "Function - " << __FUNCTION__ << ", was passed: " << in_Enum  << 
							   ", which is an erroneous type. Allowed types are: " <<
							   "CAD_SURFACE, CAD_DATUM_PLANE, CAD_AXIS, CAD_POINT, CAD_CSYS, CAD_PART, CAD_ASSEMBLY, CAD_FEATURE, and CAD_EDGE.";
				throw isis::application_exception(errorString);		
	  }

  }

  	std::string FeatureGeometryType_string( e_CADFeatureGeometryType in_Enum)
											throw (isis::application_exception)
	{
		try
		{
			ProType tempType;
		 	tempType =  FeatureGeometryType_enum( in_Enum );
		             
			return FeatureGeometryType_string(tempType);
		}
		catch ( isis::application_exception ex )
		{
			std::stringstream errorString;
			errorString << ex.tostring() << std::endl << "Function - " << __FUNCTION__ << 
					", failed to convert isis_CADCommon::e_CADFeatureGeometryType to FeatureGeometryType_string";
			throw isis::application_exception(errorString);		
		}

	}	

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
  ProAsmcompConstrType ProAsmcompConstrType_enum( string in_FeatureAlignmentType_string)
																throw (isis::application_exception)
  {
	  std::string FeatureAlignmentType_string = ConvertToUpperCase(in_FeatureAlignmentType_string);

		// Currently supported types:
		//		PRO_ASM_ALIGN
		//		PRO_ASM_MATE
		//		PRO_ASM_ALIGN_OFF
		//		PRO_ASM_MATE_OFF
		//		PRO_ASM_LINE_DIST
		//		PRO_ASM_AUTO
		//		PRO_ASM_CSYS

	  if ( FeatureAlignmentType_string.compare("MATE") == 0 )
		  return PRO_ASM_MATE;
	  else if ( FeatureAlignmentType_string.compare("MATE_OFF") == 0 )
			return PRO_ASM_MATE_OFF;
	  else if ( FeatureAlignmentType_string.compare("ALIGN") == 0 )
			return PRO_ASM_ALIGN;
	  else if ( FeatureAlignmentType_string.compare("ALIGN_OFF") == 0 )
			return PRO_ASM_ALIGN_OFF;
	  else if ( FeatureAlignmentType_string.compare("INSERT") == 0 )
			return PRO_ASM_INSERT;
	  else if ( FeatureAlignmentType_string.compare("ORIENT") == 0 )
			return PRO_ASM_ORIENT;
	  else if ( FeatureAlignmentType_string.compare("CSYS") == 0 )
			return PRO_ASM_CSYS;
	  if ( FeatureAlignmentType_string.compare("TANGENT") == 0 )
		  return PRO_ASM_TANGENT;
	  else if ( FeatureAlignmentType_string.compare("PNT_ON_SRF") == 0 )
			return PRO_ASM_PNT_ON_SRF;
	  else if ( FeatureAlignmentType_string.compare("EDGE_ON_SRF") == 0 )
			return PRO_ASM_EDGE_ON_SRF;
	  else if ( FeatureAlignmentType_string.compare("DEF_PLACEMENT") == 0 )
			return PRO_ASM_DEF_PLACEMENT;
	  else if ( FeatureAlignmentType_string.compare("SUBSTITUTE") == 0 )
			return PRO_ASM_SUBSTITUTE;
	  else if ( FeatureAlignmentType_string.compare("PNT_ON_LINE") == 0 )
			return PRO_ASM_PNT_ON_LINE;
	  else if ( FeatureAlignmentType_string.compare("FIX") == 0 )
			return PRO_ASM_FIX;
	  else if ( FeatureAlignmentType_string.compare("AUTO") == 0 )
			return PRO_ASM_AUTO;
	  else if ( FeatureAlignmentType_string.compare("ALIGN_ANG_OFF") == 0 )
			return PRO_ASM_ALIGN_ANG_OFF;
	  else if ( FeatureAlignmentType_string.compare("MATE_ANG_OFF") == 0 )
			return PRO_ASM_MATE_ANG_OFF;
	  else if ( FeatureAlignmentType_string.compare("CSYS_PNT") == 0 )
			return PRO_ASM_CSYS_PNT;
	  else if ( FeatureAlignmentType_string.compare("LINE_NORMAL") == 0 )
			return PRO_ASM_LINE_NORMAL;
	  else if ( FeatureAlignmentType_string.compare("LINE_COPLANAR") == 0 )
			return PRO_ASM_LINE_COPLANAR;
	  else if ( FeatureAlignmentType_string.compare("LINE_PARL") == 0 )
			return PRO_ASM_LINE_PARL;
	  else if ( FeatureAlignmentType_string.compare("LINE_DIST") == 0 )
			return PRO_ASM_LINE_DIST;
	  else if ( FeatureAlignmentType_string.compare("PNT_DIST") == 0 )
			return PRO_ASM_PNT_DIST;
	  else if ( FeatureAlignmentType_string.compare("INSERT_NORM") == 0 )
			return PRO_ASM_INSERT_NORM;
	  else if ( FeatureAlignmentType_string.compare("INSERT_PARL") == 0 )
			return PRO_ASM_INSERT_PARL;
	  else if ( FeatureAlignmentType_string.compare("PNT_ON_LINE_DIST") == 0 )
			return PRO_ASM_PNT_ON_LINE_DIST;
	  else if ( FeatureAlignmentType_string.compare("PNT_ON_SRF_DIST") == 0 )
			return PRO_ASM_PNT_ON_SRF_DIST;
	  else if ( FeatureAlignmentType_string.compare("EDGE_ON_SRF_DIST") == 0 )
			return PRO_ASM_EDGE_ON_SRF_DIST;
	  else if ( FeatureAlignmentType_string.compare("EDGE_ON_SRF_ANG") == 0 )
			return PRO_ASM_EDGE_ON_SRF_ANG;
	  else if ( FeatureAlignmentType_string.compare("EDGE_ON_SRF_NORMAL") == 0 )
			return PRO_ASM_EDGE_ON_SRF_NORMAL;
	  else if ( FeatureAlignmentType_string.compare("ALIGN_NODEP_ANGLE") == 0 )
			return PRO_ASM_ALIGN_NODEP_ANGLE;
	  else if ( FeatureAlignmentType_string.compare("MATE_NODEP_ANGLE") == 0 )
			return PRO_ASM_MATE_NODEP_ANGLE;
	  else if ( FeatureAlignmentType_string.compare("LINE_ANGLE") == 0 )
			return PRO_ASM_LINE_ANGLE;
	  else if ( FeatureAlignmentType_string.compare("EDGE_ON_SRF_PARL") == 0 )
			return PRO_ASM_EDGE_ON_SRF_PARL;
	  else if ( FeatureAlignmentType_string.compare("SRF_NORMAL") == 0 )
			return PRO_ASM_SRF_NORMAL;
		 
	  string temp_string = "Function ProAsmcompConstrType_enum was passed '" + in_FeatureAlignmentType_string + "' which is an erroneous type. Allowed types are ALIGN, MATE, ALIGN_OFFSET, MATE_OFFSET, LINE_DIST, AUTO and CSYS.";
	  throw isis::application_exception(temp_string.c_str());
  }

  	ProAsmcompConstrType ProAsmcompConstrType_enum( 
		                         e_CADAssemblyConstraintType in_CADAssemblyConstraintType_enum)
								 throw (isis::application_exception)
	{

		switch (in_CADAssemblyConstraintType_enum )
		{
			case CAD_ASM_MATE:
				return PRO_ASM_MATE;
				break;
			case CAD_ASM_MATE_OFF:
				return PRO_ASM_MATE_OFF;
				break;
			case CAD_ASM_ALIGN:
				return PRO_ASM_ALIGN;
				break;
			case CAD_ASM_ALIGN_OFF:
				return PRO_ASM_ALIGN_OFF;
				break;
			case CAD_ASM_CSYS:
				return PRO_ASM_CSYS;
				break;

			default:
				std::stringstream errorString;
				errorString << "Function - " << __FUNCTION__ << ", was passed: " << in_CADAssemblyConstraintType_enum  << 
							   ", which is an erroneous type. Allowed types are: " <<
							   "CAD_ASM_MATE, CAD_ASM_MATE_OFF, CAD_ASM_ALIGN, CAD_ASM_ALIGN_OFF, or CAD_ASM_CSYS.";
				throw isis::application_exception(errorString);					
		}
	}

 	std::string ProAsmcompConstrType_string(  ProAsmcompConstrType in_ProAsmcompConstrType_enum)
											throw (isis::application_exception)
	{
		switch (in_ProAsmcompConstrType_enum )
		{
			case PRO_ASM_MATE:
				return "MATE";
				break;
			case PRO_ASM_MATE_OFF:
				return "MATE_OFF";
				break;
			case PRO_ASM_ALIGN:
				return "ALIGN";
				break;
			case PRO_ASM_ALIGN_OFF:
				return "ALIGN_OFF";
				break;
			case PRO_ASM_INSERT:
				return "INSERT";
				break;
			case PRO_ASM_ORIENT:
				return "ORIENT";
				break;
			case PRO_ASM_CSYS:
				return "CSYS";
				break;
			case PRO_ASM_TANGENT:
				return "TANGENT";
				break;
			case PRO_ASM_PNT_ON_SRF:
				return "PNT_ON_SRF";
				break;
			case PRO_ASM_EDGE_ON_SRF:
				return "EDGE_ON_SRF";
				break;
			case PRO_ASM_DEF_PLACEMENT:
				return "DEF_PLACEMENT";
				break;
			case PRO_ASM_SUBSTITUTE:
				return "SUBSTITUTE";
				break;
			case PRO_ASM_PNT_ON_LINE:
				return "PNT_ON_LINE";
				break;
			case PRO_ASM_FIX:
				return "FIX";
				break;
			case PRO_ASM_AUTO:
				return "AUTO";
				break;
			case PRO_ASM_ALIGN_ANG_OFF:
				return "ALIGN_ANG_OFF";
				break;
			case PRO_ASM_MATE_ANG_OFF:
				return "MATE_ANG_OFF";
				break;
			case PRO_ASM_CSYS_PNT:
				return "CSYS_PNT";
				break;
			case PRO_ASM_LINE_NORMAL:
				return "LINE_NORMAL";
				break;
			case PRO_ASM_LINE_COPLANAR:
				return "LINE_COPLANAR";
				break;
			case PRO_ASM_LINE_PARL:
				return "LINE_PARL";
				break;
			case PRO_ASM_LINE_DIST:
				return "LINE_DIST";
				break;
			case PRO_ASM_PNT_DIST:
				return "PNT_DIST";
				break;
			case PRO_ASM_INSERT_NORM:
				return "INSERT_NORM";
				break;
			case PRO_ASM_INSERT_PARL:
				return "INSERT_PARL";
				break;
			case PRO_ASM_PNT_ON_LINE_DIST:
				return "PNT_ON_LINE_DIST";
				break;
			case PRO_ASM_PNT_ON_SRF_DIST:
				return "PNT_ON_SRF_DIST";
				break;
			case PRO_ASM_EDGE_ON_SRF_DIST:
				return "EDGE_ON_SRF_DIST";
				break;
			case PRO_ASM_EDGE_ON_SRF_ANG:
				return "EDGE_ON_SRF_ANG";
				break;
			case PRO_ASM_EDGE_ON_SRF_NORMAL:
				return "EDGE_ON_SRF_NORMAL";
				break;
			case PRO_ASM_ALIGN_NODEP_ANGLE:
				return "ALIGN_NODEP_ANGLE";
				break;
			case PRO_ASM_MATE_NODEP_ANGLE:
				return "MATE_NODEP_ANGLE";
				break;
			case PRO_ASM_LINE_ANGLE:
				return "LINE_ANGLE";
				break;
			case PRO_ASM_EDGE_ON_SRF_PARL:
				return "EDGE_ON_SRF_PARL";
				break;
			case PRO_ASM_SRF_NORMAL:
				return "SRF_NORMAL";
				break;

			default:
				char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
				string temp_string = "Function ProAsmcompConstrType_string was passed " + 
					std::string(_itoa(in_ProAsmcompConstrType_enum,temp_char_array, 10)) + 
					// Note - the allowed types are the types that should be in CADAssembly.xm.
					" which is an erroneous type.  Allowed enum values are PRO_ASM_ALIGN,PRO_ASM_MATE,PRO_ASM_ALIGN_OFF, PRO_ASM_MATE_OFF, PRO_ASM_ORIENT, and PRO_ASM_AUTO.";
				throw isis::application_exception(temp_string.c_str());
	  }

  }


  	std::string ProAsmcompConstrType_string( e_CADAssemblyConstraintType in_CADAssemblyConstraintType_enum)
											throw (isis::application_exception)
	{
		try
		{
			ProAsmcompConstrType tempType;
		 	tempType =  ProAsmcompConstrType_enum( in_CADAssemblyConstraintType_enum );
		             
			return ProAsmcompConstrType_string(tempType);
		}
		catch ( isis::application_exception ex )
		{
			std::stringstream errorString;
			errorString << ex.tostring() << std::endl << "Function - " << __FUNCTION__ << 
					", failed to convert isis_CADCommon::e_CADAssemblyConstraintType to ProAsmcompConstrType_string";
			throw isis::application_exception(errorString);		
		}

	}
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////
  ProDatumside ProDatumside_enum( string in_FeatureOrientationType_string)
												throw (isis::application_exception)
  {
	  std::string FeatureOrientationType_string = ConvertToUpperCase(in_FeatureOrientationType_string);


	  if ( FeatureOrientationType_string.compare("NONE") == 0 )
		  return PRO_DATUM_SIDE_NONE;
	  else
		  if ( FeatureOrientationType_string.compare("SIDE_A") == 0 )
			return PRO_DATUM_SIDE_YELLOW;
		  else
			if ( FeatureOrientationType_string.compare("SIDE_B") == 0 )
				return PRO_DATUM_SIDE_RED;
		 
	  string temp_string = "Function ProDatumside_enum was passed '" + in_FeatureOrientationType_string + "' which is an erroneous type. Allowed types are: NONE, SIDE_A, SIDE_B.";
	  throw isis::application_exception(temp_string.c_str());
  }


	std::string ProDatumside_string(  ProDatumside in_ProDatumside_enum)
											throw (isis::application_exception)
	{
		switch (in_ProDatumside_enum )
		{
			case PRO_DATUM_SIDE_NONE:
				return "PRO_DATUM_SIDE_NONE";
				break;
			case PRO_DATUM_SIDE_YELLOW:
				return "PRO_DATUM_SIDE_YELLOW";
				break;
			case PRO_DATUM_SIDE_RED:
				return "PRO_DATUM_SIDE_RED";
				break;
			default:
				char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
				string temp_string = "Function ProDatumside_string was passed " + 
					std::string(_itoa(in_ProDatumside_enum,temp_char_array, 10)) + 
					" which is an erroneous type.  Allowed enum values are PRO_DATUM_SIDE_NONE, PRO_DATUM_SIDE_YELLOW,  and PRO_DATUM_SIDE_RED.";
				throw isis::application_exception(temp_string.c_str());
	   }

  }



   ProBoolean String_To_ProBoolean( 
		                         const string &in_Boolean_string)
								 throw (isis::application_exception)
  {
	  std::string Boolean_string = ConvertToUpperCase(in_Boolean_string);

	  if ( Boolean_string.compare("TRUE") == 0 ||  Boolean_string.compare("1"))
		  return PRO_B_TRUE;
	  else
		  if ( Boolean_string.compare("FALSE") == 0 ||  Boolean_string.compare("0"))
			return PRO_B_FALSE;

		 
	  string temp_string = "Function String_To_ProBoolean was passed: '" + in_Boolean_string + "' which is an erroneous type. Allowed types are TRUE, FALSE, 1, or 0.";
	  throw isis::application_exception(temp_string.c_str());
  }

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	 pro_fem_solver_type AnalysisSolverType_enum( const string &in_AnalysisSolverType_string )
																throw (isis::application_exception)
	 {
		  std::string AnalysisSolverType_string = ConvertToUpperCase(in_AnalysisSolverType_string);

		  if ( AnalysisSolverType_string.compare("NASTRAN") == 0 )
			  return PRO_FEM_FEAS_NASTRAN;
		  else if ( AnalysisSolverType_string.compare("ANSYS") == 0 )
			return PRO_FEM_FEAS_ANSYS;
		  else if ( AnalysisSolverType_string.compare("ABAQUS") == 0 )
			return PRO_FEM_FEAS_ABAQUS;
		  else if ( AnalysisSolverType_string.compare("ABAQUS_DECK_BASED") == 0 )
			return PRO_FEM_FEAS_ABAQUS;
		  else if ( AnalysisSolverType_string.compare("ABAQUS_MODEL_BASED") == 0 )
			return PRO_FEM_FEAS_ABAQUS;
		  else if ( AnalysisSolverType_string.compare("PATRAN_NASTRAN") == 0 )
			return PRO_FEM_FEAS_PATRAN;
		 
		  string temp_string = "Function AnalysisSolverType_enum was passed " + in_AnalysisSolverType_string + " which is an erroneous type.";
		  throw isis::application_exception(temp_string.c_str());
	 }


	std::string AnalysisSolverType_string(  pro_fem_solver_type in_AnalysisSolverType_enum)
											throw (isis::application_exception)
	{
		switch (in_AnalysisSolverType_enum )
		{
			case PRO_FEM_FEAS_NASTRAN:
				return "NASTRAN";
				break;
			case PRO_FEM_FEAS_ANSYS:
				return "ANSYS";
				break;
			case PRO_FEM_FEAS_ABAQUS:
				return "ABAQUS";
				break;
			case PRO_FEM_FEAS_PATRAN:
				return "PATRAN_NASTRAN";
				break;

			default:
				char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
				string temp_string = "Function AnalysisSolverType_string was passed " + 
					std::string(_itoa(in_AnalysisSolverType_enum, temp_char_array, 10)) + 
					" which is an erroneous type.  Allowed enum values are PRO_FEM_FEAS_NASTRAN, PRO_FEM_FEAS_ANSYS,PRO_FEM_FEAS_ABAQUS, and PRO_FEM_FEAS_PATRAN.";
				throw isis::application_exception(temp_string.c_str());
	  }
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	 pro_fem_mesh_type AnalysisMeshType_enum( const string &in_AnalysisMeshType_string )
																throw (isis::application_exception)
	 {
		  std::string AnalysisMeshType_string = ConvertToUpperCase(in_AnalysisMeshType_string);

		  if      ( AnalysisMeshType_string.compare("SOLID") == 0 )	return PRO_FEM_SOLID_MESH;
		  else if ( AnalysisMeshType_string.compare("SHELL") == 0 )	return PRO_FEM_SHELL_MESH;
		  else if ( AnalysisMeshType_string.compare("MIXED") == 0 )	return PRO_FEM_MIXED_MESH;
		  else if ( AnalysisMeshType_string.compare("QUILT")== 0 )		return PRO_FEM_QUILT_MESH;
		  else if ( AnalysisMeshType_string.compare("BOUNDARY") == 0 ) return PRO_FEM_BOUNDARY_MESH;
		  else if ( AnalysisMeshType_string.compare("BAR") == 0 )		return PRO_FEM_BAR_MESH;
		  else if ( AnalysisMeshType_string.compare("SURFACE") == 0 )		return PRO_FEM_SHELL_MESH;
		 
		  string temp_string = "Function AnalysisMeshType_enum was passed " + in_AnalysisMeshType_string + " which is an erroneous type.";
		  throw isis::application_exception(temp_string.c_str());
	 }



	std::string AnalysisMeshType_string(  pro_fem_mesh_type in_AnalysisMeshType_enum)
											throw (isis::application_exception)
	{
		switch (in_AnalysisMeshType_enum )
		{
			case PRO_FEM_SOLID_MESH:
				return "SOLID";
				break;
			case PRO_FEM_SHELL_MESH:
				return "SURFACE";
				break;
			case PRO_FEM_MIXED_MESH:
				return "MIXED";
				break;
			case PRO_FEM_QUILT_MESH:
				return "QUILT";
				break;
			case PRO_FEM_BOUNDARY_MESH:
				return "BOUNDARY";
				break;
			case PRO_FEM_BAR_MESH:
				return "BAR";
				break;

			default:
				char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
				string temp_string = "Function AnalysisMeshType_string was passed " + 
					std::string(_itoa(in_AnalysisMeshType_enum, temp_char_array, 10)) + 
					" which is an erroneous type.  Allowed enum values are PRO_FEM_SOLID_MESH, PRO_FEM_SHELL_MESH,PRO_FEM_MIXED_MESH, PRO_FEM_QUILT_MESH, PRO_FEM_BOUNDARY_MESH, and PRO_FEM_BAR_MESH.";
				throw isis::application_exception(temp_string.c_str());
	  }
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	 pro_fem_shell_mesh_type AnalysisShellElementType_enum( const string &in_AnalysisShellElementType_string )
																throw (isis::application_exception)
	 {
		  std::string AnalysisShellElementType_string = ConvertToUpperCase(in_AnalysisShellElementType_string);
		  // N/A Means that it is ignored for the analysis type. Use Quad as default.
		  if ( AnalysisShellElementType_string.compare("N/A") == 0 )  return PRO_FEM_SHELL_MESH_QUADRANGLE; 
		  else if ( AnalysisShellElementType_string.compare("TRIANGLE") == 0 ) return PRO_FEM_SHELL_MESH_TRIANGLE;
		  else if ( AnalysisShellElementType_string.compare("TETRA4") == 0 ) return PRO_FEM_SHELL_MESH_TRIANGLE;
		  else if ( AnalysisShellElementType_string.compare("TETRA10") == 0 ) return PRO_FEM_SHELL_MESH_TRIANGLE;
		  else if ( AnalysisShellElementType_string.compare("QUADRANGLE") == 0 ) return PRO_FEM_SHELL_MESH_QUADRANGLE;
		  else if ( AnalysisShellElementType_string.compare("PLATE4") == 0 ) return PRO_FEM_SHELL_MESH_QUADRANGLE;
		  else if ( AnalysisShellElementType_string.compare("PLATE8") == 0 ) return PRO_FEM_SHELL_MESH_QUADRANGLE;
		 
		  string temp_string = "Function AnalysisShellElementType_enum was passed " + in_AnalysisShellElementType_string + " which is an erroneous type.";
		  throw isis::application_exception(temp_string.c_str());
	 }



	std::string AnalysisShellElementType_string(  pro_fem_shell_mesh_type in_AnalysisShellElementType_enum)
											throw (isis::application_exception)
	{
		switch (in_AnalysisShellElementType_enum )
		{
			case PRO_FEM_SHELL_MESH_TRIANGLE:
				return "TRIANGLE";
				break;
			case PRO_FEM_SHELL_MESH_QUADRANGLE:
				return "QUADRANGLE";
				break;

			default:
				char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
				string temp_string = "Function AnalysisShellElementType_string was passed " + 
					std::string(_itoa(in_AnalysisShellElementType_enum, temp_char_array, 10)) + 
					" which is an erroneous type.  Allowed enum values are PRO_FEM_SHELL_MESH_TRIANGLE and PRO_FEM_SHELL_MESH_QUADRANGLE.";
				throw isis::application_exception(temp_string.c_str());
	  }
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	 pro_fem_elem_shape_type AnalysisElementShapeType_enum( const string &in_AnalysisElementShapeType_string )
																throw (isis::application_exception)
	 {
		  std::string AnalysisElementShapeType_string = ConvertToUpperCase(in_AnalysisElementShapeType_string);

		  if ( AnalysisElementShapeType_string.compare("MIDPOINT_LINEAR") == 0 )
			  return PRO_FEM_MIDPNT_LINEAR;
		  else
			  if ( AnalysisElementShapeType_string.compare("MIDPOINT_PARABOLIC") == 0 )
				return PRO_FEM_MIDPNT_PARABOLIC;
		  		  else
					  if ( AnalysisElementShapeType_string.compare("MIDPOINT_PARABOLIC_FIXED") == 0 )
						return PRO_FEM_MIDPNT_PARABOLIC_FIXED;
		 
		  string temp_string = "Function AnalysisElementShapeType_enum was passed " + in_AnalysisElementShapeType_string + " which is an erroneous type.";
		  throw isis::application_exception(temp_string.c_str());
	 }



	std::string AnalysisElementShapeType_string(  pro_fem_elem_shape_type in_AnalysisElementShapeType_enum)
											throw (isis::application_exception)
	{
		switch (in_AnalysisElementShapeType_enum )
		{
			case PRO_FEM_MIDPNT_LINEAR:
				return "MIDPOINT_LINEAR";
				break;
			case PRO_FEM_MIDPNT_PARABOLIC:
				return "MIDPOINT_PARABOLIC";
				break;
			case PRO_FEM_MIDPNT_PARABOLIC_FIXED:
				return "MIDPOINT_PARABOLIC_FIXED";
				break;

			default:
				char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
				string temp_string = "Function AnalysisElementShapeType_string was passed " + 
					std::string(_itoa(in_AnalysisElementShapeType_enum, temp_char_array, 10)) + 
					" which is an erroneous type.  Allowed enum values are PRO_FEM_MIDPNT_LINEAR, PRO_FEM_MIDPNT_PARABOLIC, and PRO_FEM_MIDPNT_PARABOLIC_FIXED.";
				throw isis::application_exception(temp_string.c_str());
	  }
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////


	 ProDtmplnConstrType CoordinatePlaneAlignment_enum( const string &in_CoordinatePlaneAlignment_string )
																throw (isis::application_exception)
	 {

		std::string CoordinatePlaneAlignment_string = ConvertToUpperCase(in_CoordinatePlaneAlignment_string);

		if ( CoordinatePlaneAlignment_string.compare("XY") == 0 )			return PRO_DTMPLN_THRU_CSYS_XY;
		else if ( CoordinatePlaneAlignment_string.compare("YZ") == 0 )		return PRO_DTMPLN_THRU_CSYS_YZ;
		else if ( CoordinatePlaneAlignment_string.compare("ZX") == 0 )		return PRO_DTMPLN_THRU_CSYS_ZX;
		 
		string temp_string = "Function CoordinatePlaneAlignment_enum was passed " + in_CoordinatePlaneAlignment_string + " which is an erroneous type.";
		throw isis::application_exception(temp_string.c_str());
	 }


	std::string CoordinatePlaneAlignment_string(  ProDtmplnConstrType in_CoordinatePlaneAlignment_enum)
											throw (isis::application_exception)
	{
		switch (in_CoordinatePlaneAlignment_enum )
		{
			case PRO_DTMPLN_THRU_CSYS_XY:
				return "XY";
				break;
			case PRO_DTMPLN_THRU_CSYS_YZ:
				return "YZ";
				break;
			case PRO_DTMPLN_THRU_CSYS_ZX:
				return "ZX";
				break;
			default:
				char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
				string temp_string = "Function CoordinatePlaneAlignment_string was passed " + 
					std::string(_itoa(in_CoordinatePlaneAlignment_enum, temp_char_array, 10)) + 
					" which is an erroneous type.  Allowed enum values are PRO_DTMPLN_THRU_CSYS_XY, PRO_DTMPLN_THRU_CSYS_YZ, and PRO_DTMPLN_THRU_CSYS_ZX.";
				throw isis::application_exception(temp_string.c_str());
	  }
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	 ProType CoordinateAxisAlignment_enum( const string &in_CoordinateAxisAlignment_string )
																throw (isis::application_exception)
	 {

		std::string CoordinateAxisAlignment_string = ConvertToUpperCase(in_CoordinateAxisAlignment_string);

		if ( CoordinateAxisAlignment_string.compare("X") == 0 )			    return PRO_CSYS_AXIS_X;
		else if ( CoordinateAxisAlignment_string.compare("Y") == 0 )		return PRO_CSYS_AXIS_Y;
		else if ( CoordinateAxisAlignment_string.compare("Z") == 0 )		return PRO_CSYS_AXIS_Z;
		 
		string temp_string = "Function CoordinateAxisAlignment_enum was passed " + in_CoordinateAxisAlignment_string + " which is an erroneous type.";
		throw isis::application_exception(temp_string.c_str());
	 }


	std::string CoordinateAxisAlignment_string(  ProType in_CoordinateAxisAlignment_enum)
											throw (isis::application_exception)
	{
		switch (in_CoordinateAxisAlignment_enum )
		{
			case PRO_CSYS_AXIS_X:
				return "X";
				break;
			case PRO_CSYS_AXIS_Y:
				return "Y";
				break;
			case PRO_CSYS_AXIS_Z:
				return "Z";
				break;
			default:
				char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
				string temp_string = "Function CoordinateAxisAlignment_string was passed " + 
					std::string(_itoa(in_CoordinateAxisAlignment_enum, temp_char_array, 10)) + 
					" which is an erroneous type.  Allowed enum values are PRO_CSYS_AXIS_X, PRO_CSYS_AXIS_Y, and PRO_CSYS_AXIS_Z.";
				throw isis::application_exception(temp_string.c_str());
	  }
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////


  ProAsmcompSetType ProAsmcompSetType_enum(const std::string &in_ProAsmcompSetType_string)
												throw (isis::application_exception)
  {
	  std::string ProAsmcompSetType_string = ConvertToUpperCase(in_ProAsmcompSetType_string);

	  if      ( ProAsmcompSetType_string.compare("PRO_ASM_SET_TYPE_FIXED ") == 0 )			return PRO_ASM_SET_TYPE_FIXED ;
	  else if ( ProAsmcompSetType_string.compare("PRO_ASM_SET_TYPE_PIN,") == 0 )			return PRO_ASM_SET_TYPE_PIN;
	  else if ( ProAsmcompSetType_string.compare("PRO_ASM_SET_TYPE_SLIDER,") == 0 )			return PRO_ASM_SET_TYPE_SLIDER;
	  else if ( ProAsmcompSetType_string.compare("PRO_ASM_SET_TYPE_CYLINDRICAL,") == 0 ) 	return PRO_ASM_SET_TYPE_CYLINDRICAL;
	  else if ( ProAsmcompSetType_string.compare("PRO_ASM_SET_TYPE_PLANAR,") == 0 )			return PRO_ASM_SET_TYPE_PLANAR;
	  else if ( ProAsmcompSetType_string.compare("PRO_ASM_SET_TYPE_BALL,") == 0 )			return PRO_ASM_SET_TYPE_BALL;
	  else if ( ProAsmcompSetType_string.compare("PRO_ASM_SET_TYPE_WELD,") == 0 )			return PRO_ASM_SET_TYPE_WELD;
	  else if ( ProAsmcompSetType_string.compare("PRO_ASM_SET_TYPE_BEARING,") == 0 )		return PRO_ASM_SET_TYPE_BEARING;
	  else if ( ProAsmcompSetType_string.compare("PRO_ASM_SET_TYPE_GENERAL,") == 0 )		return PRO_ASM_SET_TYPE_BEARING;
	  else if ( ProAsmcompSetType_string.compare("PRO_ASM_SET_TYPE_6DOF,") == 0 )			return PRO_ASM_SET_TYPE_BEARING;
	  else if ( ProAsmcompSetType_string.compare("PRO_ASM_SET_TYPE_SLOT,") == 0 )			return PRO_ASM_SET_TYPE_BEARING;
	  else if ( ProAsmcompSetType_string.compare("PRO_ASM_SET_TYPE_GIMBAL,") == 0 )			return PRO_ASM_SET_TYPE_BEARING;
	  else if ( ProAsmcompSetType_string.compare("PRO_ASM_SET_TYPE_DMA_DEFINED,") == 0 )	return PRO_ASM_SET_TYPE_BEARING;
	  else if ( ProAsmcompSetType_string.compare("PRO_ASM_SET_USER_DEFINED_TYPE,") == 0 )	return PRO_ASM_SET_TYPE_BEARING;
	  
	  std::string temp_string = "Function ProAsmcompSetType_enum was passed '" + ProAsmcompSetType_string + "' which is an erroneous type. Allowed strings are PRO_ASM_SET_TYPE_FIXED, PRO_ASM_SET_TYPE_PIN, PRO_ASM_SET_TYPE_SLIDER, PRO_ASM_SET_TYPE_CYLINDRICAL, PRO_ASM_SET_TYPE_PLANAR, PRO_ASM_SET_TYPE_BALL, PRO_ASM_SET_TYPE_WELD, PRO_ASM_SET_TYPE_BEARING, PRO_ASM_SET_TYPE_GENERAL, PRO_ASM_SET_TYPE_6DOF, PRO_ASM_SET_TYPE_SLOT, PRO_ASM_SET_TYPE_GIMBAL, PRO_ASM_SET_TYPE_DMA_DEFINED, and PRO_ASM_SET_USER_DEFINED_TYPE.";
	  throw isis::application_exception(temp_string.c_str());
  }

	std::string ProAsmcompSetType_string(  ProAsmcompSetType in_ProAsmcompSetType_enum)
											throw (isis::application_exception)
	{
		switch (in_ProAsmcompSetType_enum )
		{
			case PRO_ASM_SET_TYPE_FIXED:
				return "PRO_ASM_SET_TYPE_FIXED";
				break;
			case PRO_ASM_SET_TYPE_PIN:
				return "PRO_ASM_SET_TYPE_PIN";
				break;
			case PRO_ASM_SET_TYPE_SLIDER:
				return "PRO_ASM_SET_TYPE_SLIDER";
				break;
			case PRO_ASM_SET_TYPE_CYLINDRICAL:
				return "PRO_ASM_SET_TYPE_CYLINDRICAL";
				break;
			case PRO_ASM_SET_TYPE_PLANAR:
				return "PRO_ASM_SET_TYPE_PLANAR";
				break;
			case PRO_ASM_SET_TYPE_BALL:
				return "PRO_ASM_SET_TYPE_BALL";
				break;
			case PRO_ASM_SET_TYPE_WELD:
				return "PRO_ASM_SET_TYPE_WELD";
				break;
			case PRO_ASM_SET_TYPE_BEARING:
				return "PRO_ASM_SET_TYPE_BEARING";
				break;
			case PRO_ASM_SET_TYPE_GENERAL:
				return "PRO_ASM_SET_TYPE_GENERAL";
				break;
			case PRO_ASM_SET_TYPE_6DOF:
				return "PRO_ASM_SET_TYPE_6DOF";
				break;
			case PRO_ASM_SET_TYPE_SLOT:
				return "PRO_ASM_SET_TYPE_SLOT";
				break;
			case PRO_ASM_SET_TYPE_GIMBAL:
				return "PRO_ASM_SET_TYPE_GIMBAL";
				break;
			case PRO_ASM_SET_TYPE_DMA_DEFINED:
				return "PRO_ASM_SET_TYPE_DMA_DEFINED";
				break;
			case PRO_ASM_SET_USER_DEFINED_TYPE:
				return "PRO_ASM_SET_USER_DEFINED_TYPE";
				break;

			default:
				char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
				string temp_string = "Function ProAsmcompSetType_string was passed " + 
					std::string(_itoa(in_ProAsmcompSetType_enum, temp_char_array, 10)) + 
					" which is an erroneous type. Allowed types are PRO_ASM_SET_TYPE_FIXED, PRO_ASM_SET_TYPE_PIN, PRO_ASM_SET_TYPE_SLIDER, PRO_ASM_SET_TYPE_CYLINDRICAL, PRO_ASM_SET_TYPE_PLANAR, PRO_ASM_SET_TYPE_BALL, PRO_ASM_SET_TYPE_WELD, PRO_ASM_SET_TYPE_BEARING, PRO_ASM_SET_TYPE_GENERAL, PRO_ASM_SET_TYPE_6DOF, PRO_ASM_SET_TYPE_SLOT, PRO_ASM_SET_TYPE_GIMBAL, PRO_ASM_SET_TYPE_DMA_DEFINED, and PRO_ASM_SET_USER_DEFINED_TYPE.";
				throw isis::application_exception(temp_string.c_str());
	  }
	}



  /**
  std::string FeatureInterfaceType_enum( 
		                         e_FeatureInterfaceType in_FeatureInterfaceType_enum)
								 throw (isis::application_exception)
  {

	  if ( in_FeatureInterfaceType_enum) == CAD_DATUM )
		  return "CAD_DATUM";
	  else
		  if ( in_FeatureInterfaceType_enum == CAD_MODEL_INTERFACE )
			return "CAD_MODEL_INTERFACE";
		  else
			if ( in_FeatureInterfaceType_enum == CAD_MODEL_USER_DATA )
				return "CAD_MODEL_USER_DATA";
		 
	  string temp_string = "Function FeatureInterfaceType_enum was passed " + in_FeatureInterfaceType_enum + " which is an erroneous type.";;
	  throw isis::application_exception(temp_string.c_str());
  }
	**/

} // end namespace isis