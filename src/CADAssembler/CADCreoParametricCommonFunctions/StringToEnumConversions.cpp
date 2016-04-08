#include <StringToEnumConversions.h>
#include <CommonUtilities.h>
#include <CADCommonConstants.h>
#include <sstream>

namespace isis
{
	std::ostream& operator <<(std::ostream & out, const bool & value) {
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
	  ProMdlType ProMdlType_enum(  string in_ProMdlType_string)
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
					std::string(itoa(in_ProMdlType_enum, temp_char_array, 10)) + 
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
																	// Datum Plane is treated as PRO_SURFACE by 
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
				return "PLANE";	// Not valid for CADCreoParametricCreateAssembly.exe
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
					std::string(itoa(in_FeatureGeometryType_enum,temp_char_array, 10)) + 
					" which is an erroneous type.  Allowed enum values are PRO_POINT, PRO_SURFACE, PRO_AXIS, PRO_DATUM_PLANE, PRO_PART, PRO_ASSEMBLY, and PRO_FEATURE.";
				throw isis::application_exception(temp_string.c_str());
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
					std::string(itoa(in_ProAsmcompConstrType_enum,temp_char_array, 10)) + 
					// Note - the allowed types are the types that should be in CADAssembly.xm.
					" which is an erroneous type.  Allowed enum values are PRO_ASM_ALIGN,PRO_ASM_MATE,PRO_ASM_ALIGN_OFF, PRO_ASM_MATE_OFF, PRO_ASM_ORIENT, and PRO_ASM_AUTO.";
				throw isis::application_exception(temp_string.c_str());
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
					std::string(itoa(in_ProDatumside_enum,temp_char_array, 10)) + 
					" which is an erroneous type.  Allowed enum values are PRO_DATUM_SIDE_NONE, PRO_DATUM_SIDE_YELLOW,  and PRO_DATUM_SIDE_RED.";
				throw isis::application_exception(temp_string.c_str());
	   }

  }

 	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
  e_FeatureInterfaceType FeatureInterfaceType_enum(  const string &in_FeatureInterfaceType_string)
																		throw (isis::application_exception)
  {
	  std::string FeatureInterfaceType_string = ConvertToUpperCase(in_FeatureInterfaceType_string);


	  if ( FeatureInterfaceType_string.compare("CAD_DATUM") == 0 )
		  return CAD_DATUM;
	  else
		  if ( FeatureInterfaceType_string.compare("CAD_MODEL_INTERFACE") == 0 )
			return CAD_MODEL_INTERFACE;
		  else
			if ( FeatureInterfaceType_string.compare("CAD_MODEL_USER_DATA") == 0 )
				return CAD_MODEL_USER_DATA;
		 
	  string temp_string = "Function FeatureInterfaceType_enum was passed: '" + in_FeatureInterfaceType_string + "' which is an erroneous type. Allowed types are CAD_DATUM, CAD_MODEL_INTERFACE, CAD_MODEL_USER_DATA.";
	  throw isis::application_exception(temp_string.c_str());
  }

 
	std::string FeatureInterfaceType_string( e_FeatureInterfaceType in_FeatureInterfaceType_enum)
											throw (isis::application_exception)
	{
		switch (in_FeatureInterfaceType_enum )
		{
			case CAD_DATUM:
				return "CAD_DATUM";
				break;
			case CAD_MODEL_INTERFACE:
				return "CAD_MODEL_INTERFACE";
				break;
			case CAD_MODEL_USER_DATA:
				return "CAD_MODEL_USER_DATA";
				break;
			default:
				char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
				string temp_string = "Function ProModelType_string was passed " + 
					std::string(itoa(in_FeatureInterfaceType_enum, temp_char_array, 10)) + 
					" which is an erroneous type.  Allowed enum values are CAD_DATUM, CAD_MODEL_INTERFACE, and CAD_MODEL_USER_DATA.";
				throw isis::application_exception(temp_string.c_str());
	  }

  }

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
   e_CADParameterType CADParameterType_enum( const string &in_CADParameterType_string)
																throw (isis::application_exception)
  {

	  std::string CADParameterType_string = ConvertToUpperCase(in_CADParameterType_string);

	  if ( CADParameterType_string.compare("FLOAT") == 0 )
		  return CAD_FLOAT;
	  else
		  if ( CADParameterType_string.compare("INTEGER") == 0 )
			return CAD_INTEGER;
		  else
			if ( CADParameterType_string.compare("BOOLEAN") == 0 )
				return CAD_BOOLEAN;
			else
	  			if ( CADParameterType_string.compare("STRING") == 0 )
					return CAD_STRING;
		 
	  string temp_string = "Function CADParameterType_enum was passed '" + in_CADParameterType_string + "' which is an erroneous type. Allowed types are FLOAT, INTEGER, BOOLEAN and STRING.";;
	  throw isis::application_exception(temp_string.c_str());
  }


	std::string  CADParameterType_string(   e_CADParameterType in_CADParameterType_enum )
											throw (isis::application_exception)
	{
		switch (in_CADParameterType_enum )
		{
			case CAD_FLOAT:
				return "FLOAT";
				break;
			case CAD_INTEGER:
				return "INTEGER";
				break;
			case CAD_BOOLEAN:
				return "BOOLEAN";
				break;

			case CAD_STRING:
				return "STRING";
				break;

			default:
				char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
				string temp_string = "Function ProModelType_string was passed " + 
					std::string(itoa(in_CADParameterType_enum, temp_char_array, 10)) + 
					" which is an erroneous type.  Allowed enum values are PRO_MDL_PART and PRO_MDL_ASSEMBLY.";
				throw isis::application_exception(temp_string.c_str());
	  }

  }

   ProBoolean String_To_ProBoolean( 
		                         const string &in_Boolean_string)
								 throw (isis::application_exception)
  {
	  std::string Boolean_string = ConvertToUpperCase(in_Boolean_string);

	  if ( Boolean_string.compare("TRUE") == 0 )
		  return PRO_B_TRUE;
	  else
		  if ( Boolean_string.compare("FALSE") == 0 )
			return PRO_B_FALSE;

		 
	  string temp_string = "Function String_To_ProBoolean was passed: '" + in_Boolean_string + "' which is an erroneous type. Allowed types are TRUE and FALSE.";
	  throw isis::application_exception(temp_string.c_str());
  }

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	 e_CADSpecialInstruction SpecialInstruction_enum( const string &in_SpecialInstruction_string )
																throw (isis::application_exception)
	 {
		  std::string SpecialInstruction_string = ConvertToUpperCase(in_SpecialInstruction_string);

		  if ( SpecialInstruction_string.compare("") == 0 )
			  return CAD_SPECIAL_INSTRUCTION_NONE;
		  else
			  if ( SpecialInstruction_string.compare("SIZE_TO_FIT") == 0 )
				return CAD_SPECIAL_INSTRUCTION_SIZE_TO_FIT;
		  else
			  if ( SpecialInstruction_string.compare("HAS_KINEMATIC_JOINT") == 0 )
				return CAD_SPECIAL_INSTRUCTION_HAS_KINEMATIC_JOINT;

		  string temp_string = "Function SpecialInstruction_enum was passed '" + in_SpecialInstruction_string + "' which is an erroneous type. Allowed types are <empty>, SIZE_TO_FIT, and HAS_KINEMATIC_JOINT.";;
		  throw isis::application_exception(temp_string.c_str());
	 }


	std::string SpecialInstruction_string(  e_CADSpecialInstruction in_CADComponentType_enum)
											throw (isis::application_exception)
	{
		switch (in_CADComponentType_enum )
		{
			case CAD_SPECIAL_INSTRUCTION_NONE:
				return "";
				break;
			case CAD_SPECIAL_INSTRUCTION_SIZE_TO_FIT:
				return "SIZE_TO_FIT";
				break;
			case CAD_SPECIAL_INSTRUCTION_HAS_KINEMATIC_JOINT:
				return "HAS_KINEMATIC_JOINT";
				break;
			default:
				char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
				string temp_string = "Function SpecialInstruction_string was passed " + 
					std::string(itoa(in_CADComponentType_enum,temp_char_array, 10)) + 
					" which is an erroneous type.  Allowed enum values are CAD_SPECIAL_INSTRUCTION_NONE, CAD_SPECIAL_INSTRUCTION_SIZE_TO_FIT, and CAD_SPECIAL_INSTRUCTION_HAS_KINEMATIC_JOINT.";
				throw isis::application_exception(temp_string.c_str());
	  }

  }


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	 e_CADUnitsDistance CADUnitsDistance_enum( const string &in_Units_string )
																throw (isis::application_exception)
	 {
		  std::string Units_string = ConvertToUpperCase(in_Units_string);

		  if ( Units_string.compare("MILLIMETER") == 0 )	return CAD_UNITS_MM;
		  else if ( Units_string.compare("MM") == 0 )		return CAD_UNITS_MM;
		  else if ( Units_string.compare("CM") == 0 )		return CAD_UNITS_CM;
		  else if ( Units_string.compare("INCH") == 0 )		return CAD_UNITS_INCH;
		  else if ( Units_string.compare("N/A") == 0 )		return CAD_UNITS_NA;
			 
		  string temp_string = "Function CADUnitsDistance_enum was passed '" + in_Units_string + "' which is an erroneous type. Allowed types are MILLIMETER, CM, INCH or N/A.";
		  throw isis::application_exception(temp_string.c_str());
	 }



	std::string CADUnitsDistance_string(  e_CADUnitsDistance in_Units_enum)
											throw (isis::application_exception)
	{
		switch (in_Units_enum )
		{
			case CAD_UNITS_MM:
				return "MM";
				break;
			case CAD_UNITS_CM:
				return "CM";
				break;
			case CAD_UNITS_INCH:
				return "INCH";
				break;
			case CAD_UNITS_NA:
				return "N/A";
				break;
			default:
				char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
				string temp_string = "Function CADUnitsDistance_string was passed " + 
					std::string(itoa(in_Units_enum, temp_char_array, 10)) + 
					" which is an erroneous type.  Allowed enum values are CAD_UNITS_MM, CAD_UNITS_CM, CAD_UNITS_INCH, and CAD_UNITS_NA.";
				throw isis::application_exception(temp_string.c_str());
	  }

  }
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	e_CADUnitsForce CADUnitsForce_enum( const string &in_UnitsForce_string )
																throw (isis::application_exception)
	 {
		  std::string UnitsForce_string = ConvertToUpperCase(in_UnitsForce_string);

		  if ( UnitsForce_string.compare("LBF") == 0 )
			  return CAD_UNITS_LBF;
		  else
			  if ( UnitsForce_string.compare("NEWTON") == 0 || UnitsForce_string.compare("N") == 0)
				return CAD_UNITS_N;
			 
		  string temp_string = "Function CADUnitsForce_enum was passed " + in_UnitsForce_string + " which is an erroneous type.";
		  throw isis::application_exception(temp_string.c_str());
	 }



	std::string CADUnitsForce_string(  e_CADUnitsForce in_UnitsForce_enum)
											throw (isis::application_exception)
	{
		switch (in_UnitsForce_enum )
		{
			case CAD_UNITS_LBF:
				return "LBF";
				break;
			case CAD_UNITS_N:
				return "N";
				break;
			default:
				char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
				string temp_string = "Function CADUnitsForce_string was passed " + 
					std::string(itoa(in_UnitsForce_enum, temp_char_array, 10)) + 
					" which is an erroneous type.  Allowed enum values are CAD_UNITS_LBF and CAD_UNITS_N.";
				throw isis::application_exception(temp_string.c_str());
	  }
  }


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////


	e_CADUnitsMoment CADUnitsMoment_enum( const string &in_UnitsMoment_string )
																throw (isis::application_exception)
	 {
		std::string UnitsMoment_string = ConvertToUpperCase(in_UnitsMoment_string);

		//std::cout << std::endl << "CADUnitsMoment_enum " << in_UnitsMoment_string << "  " << Units_string;

		if (      UnitsMoment_string.compare("FT-LBF") == 0 )		return CAD_UNITS_FT_LBF;
		else if ( UnitsMoment_string.compare("IN-LBF") == 0 )		return CAD_UNITS_IN_LBF;
		else if ( UnitsMoment_string.compare("NEWTON-M") == 0 || UnitsMoment_string.compare("N-M") == 0 )			return CAD_UNITS_N_M;
		else if ( UnitsMoment_string.compare("NEWTON-MM") == 0 || UnitsMoment_string.compare("N-MM") == 0 )			return CAD_UNITS_N_MM;
			 
		string temp_string = "Function CADUnitsMoment_enum was passed " + in_UnitsMoment_string + " which is an erroneous type.";
		throw isis::application_exception(temp_string.c_str());
	 }


	std::string CADUnitsMoment_string(  e_CADUnitsMoment in_UnitsMoment_enum)
											throw (isis::application_exception)
	{
		switch (in_UnitsMoment_enum )
		{
			case CAD_UNITS_FT_LBF:
				return "FT-LBF";
				break;
			case CAD_UNITS_IN_LBF:
				return "IN-LBF";
				break;
			case CAD_UNITS_N_M:
				return "N-M";
				break;
			case CAD_UNITS_N_MM:
				return "N-MM";
				break;
			default:
				char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
				string temp_string = "Function CADUnitsMoment_string was passed " + 
					std::string(itoa(in_UnitsMoment_enum, temp_char_array, 10)) + 
					" which is an erroneous type.  Allowed enum values are CAD_UNITS_FT_LBF, CAD_UNITS_IN_LBF, CAD_UNITS_N_M and CAD_UNITS_N_MM.";
				throw isis::application_exception(temp_string.c_str());
	  }
  }

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	 e_CADUnitsPressure CADUnitsPressure_enum( const string &in_UnitsPressure_string )
																throw (isis::application_exception)
	 {
		  std::string UnitsPressure_string = ConvertToUpperCase(in_UnitsPressure_string);

		  if ( UnitsPressure_string.compare("PSI") == 0 )
			  return CAD_UNITS_LBF_PER_INCH_SQ;
		  else
		  if ( UnitsPressure_string.compare("PA") == 0 )
				return CAD_UNITS_PA;
		  if ( UnitsPressure_string.compare("PASCAL") == 0 )
				return CAD_UNITS_PA;
		  if ( UnitsPressure_string.compare("MPA") == 0 )
				return CAD_UNITS_MPA;
			 
		  string temp_string = "Function UnitsPressure_enum was passed: '" + in_UnitsPressure_string + "' which is an erroneous type. Allowed types are: PSI, PA, PASCAL or MPA.";
		  throw isis::application_exception(temp_string.c_str());
	 }


	std::string CADUnitsPressure_string(  e_CADUnitsPressure in_UnitsPressure_enum)
											throw (isis::application_exception)
	{
		switch (in_UnitsPressure_enum )
		{
			case CAD_UNITS_LBF_PER_INCH_SQ:
				return "psi";
				break;
			case CAD_UNITS_PA:
				return "Pa";
				break;
			case CAD_UNITS_MPA:
				return "MPa";
				break;
			default:
				char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
				string temp_string = "Function UnitsPressure_string was passed " + 
					std::string(itoa(in_UnitsPressure_enum, temp_char_array, 10)) + 
					" which is an erroneous type.  Allowed enum values are CAD_UNITS_LBF_PER_INCH_SQ, CAD_UNITS_PA, and CAD_UNITS_MPA.";
				throw isis::application_exception(temp_string.c_str());
	  }
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	 e_CADUnitsAcceleration CADUnitsAcceleration_enum( const string &in_UnitsAcceleration_string )
																throw (isis::application_exception)
	 {
		  std::string UnitsAcceleration_string = ConvertToUpperCase(in_UnitsAcceleration_string);

		  if ( UnitsAcceleration_string.compare("FT/S2") == 0 || UnitsAcceleration_string.compare("FT/S^2") == 0 )
			  return CAD_UNITS_FT_PER_SEC_SQ;
		  else
			  if ( UnitsAcceleration_string.compare("MM/S2") == 0 || UnitsAcceleration_string.compare("MM/S^2") == 0 )
				return CAD_UNITS_MM_PER_SEC_SQ;
			  else
				 if ( UnitsAcceleration_string.compare("M/S2") == 0 || UnitsAcceleration_string.compare("M/S^2") == 0 )
						return CAD_UNITS_M_PER_SEC_SQ;
		 
		  string temp_string = "Function UnitsAcceleration_enum was passed " + in_UnitsAcceleration_string + " which is an erroneous type.";
		  throw isis::application_exception(temp_string.c_str());
	 }



	std::string CADUnitsAcceleration_string(  e_CADUnitsAcceleration in_UnitsAcceleration_enum)
											throw (isis::application_exception)
	{
		switch (in_UnitsAcceleration_enum )
		{
			case CAD_UNITS_FT_PER_SEC_SQ:
				return "FT/S^2";
				break;
			case CAD_UNITS_MM_PER_SEC_SQ:
				return "MM/S^2";
				break;
			case CAD_UNITS_M_PER_SEC_SQ:
				return "M/S^2";
				break;
			default:
				char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
				string temp_string = "Function UnitsAcceleration_string was passed " + 
					std::string(itoa(in_UnitsAcceleration_enum, temp_char_array, 10)) + 
					" which is an erroneous type.  Allowed enum values are CAD_UNITS_FT_PER_SEC_SQ, CAD_UNITS_MM_PER_SEC_SQ, and CAD_UNITS_M_PER_SEC_SQ.";
				throw isis::application_exception(temp_string.c_str());
	  }
	}


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	 e_CADUnitsDensity CADUnitsDensity_enum( const string &in_UnitsDensity_string )
																throw (isis::application_exception)
	 {
		  std::string UnitsDensity_string = ConvertToUpperCase(in_UnitsDensity_string);

		  if ( UnitsDensity_string.compare("KG/MM3") == 0 )
			  return CAD_UNITS_KG_PER_MM_CUBED;
		  else	if ( UnitsDensity_string.compare("KG/M3") == 0 )
			  return CAD_UNITS_KG_PER_M_CUBED;
	 
		  string temp_string = "Function CADUnitsDensity_enum was passed " + in_UnitsDensity_string + " which is an erroneous type.";
		  throw isis::application_exception(temp_string.c_str());
	 }


	std::string CADUnitsDensity_string(  e_CADUnitsDensity in_UnitsDensity_enum)
											throw (isis::application_exception)
	{
		switch (in_UnitsDensity_enum )
		{
			case CAD_UNITS_KG_PER_MM_CUBED:
				return "kg/mm3";
				break;
			case CAD_UNITS_KG_PER_M_CUBED:
				return "kg/m3";
				break;

			default:
				char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
				string temp_string = "Function UnitsDensity_string was passed " + 
					std::string(itoa(in_UnitsDensity_enum, temp_char_array, 10)) + 
					" which is an erroneous type.  Allowed enum values are CAD_UNITS_KG_PER_MM_CUBED and CAD_UNITS_KG_PER_M_CUBED.";
				throw isis::application_exception(temp_string.c_str());
	  }
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	 e_CADUnitsHeatCapacity CADUnitsHeatCapacity_enum( const string &in_UnitsHeatCapacity_string )
																throw (isis::application_exception)
	 {
		  std::string UnitsHeatCapacity_string = ConvertToUpperCase(in_UnitsHeatCapacity_string);

		  if ( UnitsHeatCapacity_string.compare("J/KGK") == 0 )
			  return CAD_UNITS_J_PER_KG_K;
	 
		  string temp_string = "Function CADUnitsHeatCapacity_enum was passed " + in_UnitsHeatCapacity_string + " which is an erroneous type.";
		  throw isis::application_exception(temp_string.c_str());
	 }


	std::string CADUnitsHeatCapacity_string(  e_CADUnitsHeatCapacity in_UnitsHeatCapacity_enum)
											throw (isis::application_exception)
	{
		switch (in_UnitsHeatCapacity_enum )
		{
			case CAD_UNITS_J_PER_KG_K:
				return "J/KgK";
				break;

			default:
				char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
				string temp_string = "Function UnitsHeatCapacity_string was passed " + 
					std::string(itoa(in_UnitsHeatCapacity_enum, temp_char_array, 10)) + 
					" which is an erroneous type.  Allowed enum values are CAD_UNITS_J_PER_KG_K.";
				throw isis::application_exception(temp_string.c_str());
	  }
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	 e_CADUnitsThermalConductivity CADUnitsThermalConductivity_enum( const string &in_UnitsThermalConductivity_string )
																throw (isis::application_exception)
	 {
		  std::string UnitsThermalConductivity_string = ConvertToUpperCase(in_UnitsThermalConductivity_string);

		  if ( UnitsThermalConductivity_string.compare("W/MMK") == 0 )
			  return CAD_UNITS_W_PER_MM_K;

		  string temp_string = "Function CADUnitsThermalConductivity_enum was passed " + in_UnitsThermalConductivity_string + " which is an erroneous type.";
		  throw isis::application_exception(temp_string.c_str());
	 }


	std::string CADUnitsThermalConductivity_string(  e_CADUnitsThermalConductivity in_UnitsThermalConductivity_enum)
											throw (isis::application_exception)
	{
		switch (in_UnitsThermalConductivity_enum )
		{
			case CAD_UNITS_W_PER_MM_K:
				return "W/mmK";
				break;


			default:
				char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
				string temp_string = "Function UnitsThermalConductivity_string was passed " + 
					std::string(itoa(in_UnitsThermalConductivity_enum, temp_char_array, 10)) + 
					" which is an erroneous type.  Allowed enum values are CAD_UNITS_W_PER_MM_K.";
				throw isis::application_exception(temp_string.c_str());
	  }
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	 e_CADUnitsAngle CADUnitsAngle_enum( const string &in_UnitsAngle_string )
																throw (isis::application_exception)
	 {
		  std::string UnitsAngle_string = ConvertToUpperCase(in_UnitsAngle_string);

		  if ( UnitsAngle_string.compare("RADIAN") == 0 )		return CAD_UNITS_RADIAN;
		  else if ( UnitsAngle_string.compare("DEGREE") == 0 )	return CAD_UNITS_DEGREE;
		  else if ( UnitsAngle_string.compare("DEGREES") == 0 )	return CAD_UNITS_DEGREE;
		  else if ( UnitsAngle_string.compare("DEG") == 0 )		return CAD_UNITS_DEGREE;
		  else if ( UnitsAngle_string.compare("N/A") == 0 )		return CAD_UNITS_ANGLE_NA;
		 

		  string temp_string = "Function CADUnitsAngle_enum was passed " + in_UnitsAngle_string + " which is an erroneous type.";
		  throw isis::application_exception(temp_string.c_str());
	 }


	std::string CADUnitsAngle_string(  e_CADUnitsAngle in_UnitsAngle_enum)
											throw (isis::application_exception)
	{
		switch (in_UnitsAngle_enum )
		{
			case CAD_UNITS_RADIAN:
				return "RADIAN";
				break;
			case CAD_UNITS_DEGREE:
				return "DEGREE";
				break;
			case CAD_UNITS_ANGLE_NA:
				return "N/A";
				break;
			default:
				char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
				string temp_string = "Function CADUnitsAngle_string was passed " + 
					std::string(itoa(in_UnitsAngle_enum, temp_char_array, 10)) + 
					" which is an erroneous type.  Allowed enum values are CAD_UNITS_RADIANS, and CAD_UNITS_DEGREES.";
				throw isis::application_exception(temp_string.c_str());
	  }
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	 e_AnalysisType AnalysisType_enum( const string &in_AnalysisType_string )
																throw (isis::application_exception)
	 {
		  std::string AnalysisType_string = ConvertToUpperCase(in_AnalysisType_string);

		  if ( AnalysisType_string.compare("STRUCTURAL") == 0 )
			  return ANALYSIS_STRUCTURAL;
		  else
			  if ( AnalysisType_string.compare("BUCKLING") == 0 )
				return ANALYSIS_BUCKLING;
		  		  else
					  if ( AnalysisType_string.compare("MODAL") == 0 )
						return ANALYSIS_MODAL;
		  				  else
							  if ( AnalysisType_string.compare("THERMAL") == 0 )
								return ANALYSIS_THERMAL;
		  							  else
									  if ( AnalysisType_string.compare("FATIGUE") == 0 )
										return ANALYSIS_FATIGUE;

		 
		  string temp_string = "Function AnalysisType_enum was passed " + in_AnalysisType_string + " which is an erroneous type.";
		  throw isis::application_exception(temp_string.c_str());
	 }


	std::string AnalysisType_string(  e_AnalysisType in_AnalysisType_enum)
											throw (isis::application_exception)
	{
		switch (in_AnalysisType_enum )
		{
			case ANALYSIS_STRUCTURAL:
				return "STRUCTURAL";
				break;
			case ANALYSIS_BUCKLING:
				return "BUCKLING";
				break;
			case ANALYSIS_MODAL:
				return "MODAL";
				break;
			case ANALYSIS_THERMAL:
				return "THERMAL";
				break;
			case ANALYSIS_FATIGUE:
				return "FATIGUE";
				break;
			default:
				char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
				string temp_string = "Function AnalysisType_string was passed " + 
					std::string(itoa(in_AnalysisType_enum, temp_char_array, 10)) + 
					" which is an erroneous type.  Allowed enum values are ANALYSIS_STRUCTURAL, ANALYSIS_BUCKLING, ANALYSIS_MODAL, ANALYSIS_THERMAL, and ANALYSIS_FATIGUE.";
				throw isis::application_exception(temp_string.c_str());
	  }
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*

	 e_AnalysisLoadType AnalysisLoadType_enum( const string &in_Units_string )
																throw (isis::application_exception)
	 {
		  std::string Units_string = ConvertToUpperCase(in_Units_string);

		  if ( Units_string.compare("FORCE") == 0 )
			  return ANALYSIS_LOAD_FORCE;
		  else
			  if ( Units_string.compare("PRESSURE") == 0 )
				return ANALYSIS_LOAD_PRESSURE;
		  		  else
					  if ( Units_string.compare("BEARING") == 0 )
						return ANALYSIS_LOAD_BEARING;
		  				  else
							  if ( Units_string.compare("ACCELERATION") == 0 )
								return ANALYSIS_LOAD_ACCELERATION;
		  							  else
									  if ( Units_string.compare("CENTRIFUGAL") == 0 )
											return ANALYSIS_LOAD_CENTRIFUGAL;
		  		  						  else
											  if ( Units_string.compare("PRELOAD") == 0 )
												return ANALYSIS_LOAD_PRELOAD;
		  		  							  else
												  if ( Units_string.compare("TEMPERATURE") == 0 )
													return ANALYSIS_LOAD_TEMPERATURE;
		 
		  string temp_string = "Function AnalysisLoadType_enum was passed " + in_Units_string + " which is an erroneous type.";
		  throw isis::application_exception(temp_string.c_str());
	 }


	std::string AnalysisLoadType_string(  e_AnalysisLoadType in_Units_enum)
											throw (isis::application_exception)
	{
		switch (in_Units_enum )
		{
			case ANALYSIS_LOAD_FORCE:
				return "FORCE";
				break;
			case ANALYSIS_LOAD_PRESSURE:
				return "PRESSURE";
				break;
			case ANALYSIS_LOAD_BEARING:
				return "BEARING";
				break;
			case ANALYSIS_LOAD_ACCELERATION:
				return "ACCELERATION";
				break;
			case ANALYSIS_LOAD_CENTRIFUGAL:
				return "CENTRIFUGAL";
				break;
			case ANALYSIS_LOAD_PRELOAD:
				return "PRELOAD";
				break;
			case ANALYSIS_LOAD_TEMPERATURE:
				return "TEMPERATURE";
				break;
			default:
				char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
				string temp_string = "Function AnalysisLoadType_string was passed " + 
					std::string(itoa(in_Units_enum, temp_char_array, 10)) + 
					" which is an erroneous type.  Allowed enum values are ANALYSIS_LOAD_FORCE, ANALYSIS_LOAD_PRESSURE, ANALYSIS_LOAD_BEARING, ANALYSIS_LOAD_ACCELERATION, ANALYSIS_LOAD_CENTRIFUGAL, ANALYSIS_LOAD_PRELOAD, and ANALYSIS_LOAD_TEMPERATURE.";
				throw isis::application_exception(temp_string.c_str());
	  }
	}
	*/

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	 e_AnalysisPartInterfaceType AnalysisPartInterfaceType_enum( const string &in_AnalysisPartInterfaceType_string )
																throw (isis::application_exception)
	 {
		  std::string AnalysisPartInterfaceType_string = ConvertToUpperCase(in_AnalysisPartInterfaceType_string);

		  if ( AnalysisPartInterfaceType_string.compare("FREE") == 0 )
			  return ANALYSIS_PART_INTERFACE_FREE;
		  else
			  if ( AnalysisPartInterfaceType_string.compare("CONTACTING") == 0 )
				return ANALYSIS_PART_INTERFACE_CONTACTING;
		  		  else
					  if ( AnalysisPartInterfaceType_string.compare("BONDED") == 0 )
						return ANALYSIS_PART_INTERFACE_BONDED;
		 
		  string temp_string = "Function AnalysisPartInterfaceType_enum was passed " + in_AnalysisPartInterfaceType_string + " which is an erroneous type.";
		  throw isis::application_exception(temp_string.c_str());
	 }


	 std::string AnalysisPartInterfaceType_string(  e_AnalysisPartInterfaceType in_AnalysisPartInterfaceType_enum)
											throw (isis::application_exception)
	{
		switch (in_AnalysisPartInterfaceType_enum )
		{
			case ANALYSIS_PART_INTERFACE_FREE:
				return "FREE";
				break;
			case ANALYSIS_PART_INTERFACE_CONTACTING:
				return "CONTACTING";
				break;
			case ANALYSIS_PART_INTERFACE_BONDED:
				return "BONDED";
				break;

			default:
				char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
				string temp_string = "Function AnalysisPartInterfaceType_string was passed " + 
					std::string(itoa(in_AnalysisPartInterfaceType_enum, temp_char_array, 10)) + 
					" which is an erroneous type.  Allowed enum values are ANALYSIS_PART_INTERFACE_FREE, ANALYSIS_PART_INTERFACE_CONTACTING, and ANALYSIS_PART_INTERFACE_BONDED.";
				throw isis::application_exception(temp_string.c_str());
	  }
	}
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

			default:
				char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
				string temp_string = "Function AnalysisSolverType_string was passed " + 
					std::string(itoa(in_AnalysisSolverType_enum, temp_char_array, 10)) + 
					" which is an erroneous type.  Allowed enum values are PRO_FEM_FEAS_NASTRAN, PRO_FEM_FEAS_ANSYS, and PRO_FEM_FEAS_ABAQUS.";
				throw isis::application_exception(temp_string.c_str());
	  }
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	 e_AnalysisSolutionType AnalysisSolutionType_enum( const string &in_AnalysisSolutionType_string )
																throw (isis::application_exception)
	 {
		  std::string AnalysisSolutionType_string = ConvertToUpperCase(in_AnalysisSolutionType_string);

		  if ( AnalysisSolutionType_string.compare("ABAQUS_MODEL_BASED") == 0 )
			  return ANALYSIS_MODEL_BASED;
		  else
			  return ANALYSIS_DECK_BASED;
	 }


	std::string AnalysisSolutionType_string(  e_AnalysisSolutionType in_AnalysisSolutionType_enum)
											throw (isis::application_exception)
	{
		switch (in_AnalysisSolutionType_enum )
		{
			case ANALYSIS_DECK_BASED:
				return "ANALYSIS_DECK_BASED";
				break;
			case ANALYSIS_MODEL_BASED:
				return "ANALYSIS_MODEL_BASED";
				break;

			default:
				char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
				string temp_string = "Function AnalysisSolutionType_string was passed " + 
					std::string(itoa(in_AnalysisSolutionType_enum, temp_char_array, 10)) + 
					" which is an erroneous type.  Allowed enum values are ANALYSIS_DECK_BASED and ANALYSIS_MODEL_BASED.";
				throw isis::application_exception(temp_string.c_str());
	  }
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	 e_AnalysisConstraintFixedFree AnalysisConstraintFixedFree_enum( const string &in_AnalysisConstraintFixedFree_string )
																throw (isis::application_exception)
	 {
		  std::string AnalysisConstraintFixedFree_string = ConvertToUpperCase(in_AnalysisConstraintFixedFree_string);
		  
		if ( AnalysisConstraintFixedFree_string.compare("SCALAR") == 0 )		return ANALYSIS_CONSTRAINT_SCALAR;
		else if ( AnalysisConstraintFixedFree_string.compare("FIXED") == 0 )	return ANALYSIS_CONSTRAINT_FIXED;
		else if ( AnalysisConstraintFixedFree_string.compare("FREE") == 0 )		return ANALYSIS_CONSTRAINT_FREE;
		 
		string temp_string = "Function AnalysisConstraintFixedFree_enum was passed " + in_AnalysisConstraintFixedFree_string + " which is an erroneous type.";
		throw isis::application_exception(temp_string.c_str());
	 }


	std::string AnalysisConstraintFixedFree_string(  e_AnalysisConstraintFixedFree in_AnalysisConstraintFixedFree_enum)
											throw (isis::application_exception)
	{
		switch (in_AnalysisConstraintFixedFree_enum )
		{
			case ANALYSIS_CONSTRAINT_SCALAR:
				return "SCALAR";
				break;
			case ANALYSIS_CONSTRAINT_FIXED:
				return "FIXED";
				break;
			case ANALYSIS_CONSTRAINT_FREE:
				return "FREE";
				break;

			default:
				char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
				string temp_string = "Function AnalysisConstraintFixedFree_string was passed " + 
					std::string(itoa(in_AnalysisConstraintFixedFree_enum, temp_char_array, 10)) + 
					" which is an erroneous type.  Allowed enum values are ANALYSIS_CONSTRAINT_SCALAR, ANALYSIS_CONSTRAINT_FIXED and ANALYSIS_CONSTRAINT_FREE.";
				throw isis::application_exception(temp_string.c_str());
	  }
	}

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
				return "SHELL";
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
					std::string(itoa(in_AnalysisMeshType_enum, temp_char_array, 10)) + 
					" which is an erroneous type.  Allowed enum values are PRO_FEM_SOLID_MESH, PRO_FEM_SHELL_MESH,PRO_FEM_MIXED_MESH, PRO_FEM_QUILT_MESH, PRO_FEM_BOUNDARY_MESH, and PRO_FEM_BAR_MESH.";
				throw isis::application_exception(temp_string.c_str());
	  }
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	 pro_fem_shell_mesh_type AnalysisShellElementType_enum( const string &in_AnalysisShellElementType_string )
																throw (isis::application_exception)
	 {
		  std::string AnalysisShellElementType_string = ConvertToUpperCase(in_AnalysisShellElementType_string);

		  if ( AnalysisShellElementType_string.compare("N/A") == 0 )  // N/A Means that it is ignored for the analysis type. Use Quad as default.
			  return PRO_FEM_SHELL_MESH_QUADRANGLE;
		  else
			  if ( AnalysisShellElementType_string.compare("TRIANGLE") == 0 )
				return PRO_FEM_SHELL_MESH_TRIANGLE;
		  		  else
					  if ( AnalysisShellElementType_string.compare("QUADRANGLE") == 0 )
						return PRO_FEM_SHELL_MESH_QUADRANGLE;
		 
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
					std::string(itoa(in_AnalysisShellElementType_enum, temp_char_array, 10)) + 
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
					std::string(itoa(in_AnalysisElementShapeType_enum, temp_char_array, 10)) + 
					" which is an erroneous type.  Allowed enum values are PRO_FEM_MIDPNT_LINEAR, PRO_FEM_MIDPNT_PARABOLIC, and PRO_FEM_MIDPNT_PARABOLIC_FIXED.";
				throw isis::application_exception(temp_string.c_str());
	  }
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	 e_CADGeometryType CADGeometryType_enum( const string &in_CADGeometryType_string )
																throw (isis::application_exception)
	 {
		  std::string CADGeometryType_string = ConvertToUpperCase(in_CADGeometryType_string);

		  if ( CADGeometryType_string.compare("POLYGON") == 0 ) return CAD_GEOMETRY_POLYGON;
		  else if ( CADGeometryType_string.compare("CIRCLE") == 0 ) return CAD_GEOMETRY_CIRCLE;
		  else if ( CADGeometryType_string.compare("CONCENTRIC_CIRCLES") == 0 ) return CAD_GEOMETRY_CONCENTRIC_CIRCLES;	 
		  else if ( CADGeometryType_string.compare("CYLINDER") == 0 ) return CAD_GEOMETRY_CYLINDER;  // zzzz fix this later, should not be surface
		  //else if ( CADGeometryType_string.compare("CYLINDER_SURFACE") == 0 ) return CAD_GEOMETRY_CYLINDER_SURFACE;
		  else if ( CADGeometryType_string.compare("SPHERE") == 0 ) return CAD_GEOMETRY_SPHERE;     // zzzz fix this later, should not be surface
		  //else if ( CADGeometryType_string.compare("SPHERE_SURFACE") == 0 ) return CAD_GEOMETRY_SPHERE_SURFACE;
		  else if ( CADGeometryType_string.compare("EXTRUSION") == 0 ) return CAD_GEOMETRY_EXTRUSION;   
		  else if ( CADGeometryType_string.compare("FACE") == 0 ) return CAD_GEOMETRY_FACE;   
		  // else if ( CADGeometryType_string.compare("EXTRUSION_SURFACE") == 0 ) return CAD_GEOMETRY_EXTRUSION_SURFACE;
		  else if ( CADGeometryType_string.compare("NONE") == 0 ) return CAD_GEOMETRY_NONE;


		  string temp_string = "Function CADGeometryType_enum was passed " + in_CADGeometryType_string + " which is an erroneous type.";
		  throw isis::application_exception(temp_string.c_str());
	 }


	std::string CADGeometryType_string(  e_CADGeometryType in_CADGeometryType_enum)
											throw (isis::application_exception)
	{
		switch (in_CADGeometryType_enum )
		{
			case CAD_GEOMETRY_POLYGON:
				return "POLYGON";
				break;
			case CAD_GEOMETRY_CIRCLE:
				return "CIRCLE";
				break;
			case CAD_GEOMETRY_CONCENTRIC_CIRCLES:
				return "CONCENTRIC_CIRCLES";
				break;
			case CAD_GEOMETRY_CYLINDER:
				return "CYLINDER";
				break;
			//case CAD_GEOMETRY_CYLINDER_SURFACE:
			//	return "CYLINDER_SURFACE";
			//	break;
			case CAD_GEOMETRY_SPHERE:
				return "SPHERE";
				break;
			//case CAD_GEOMETRY_SPHERE_SURFACE:
			//	return "SPHERE_SURFACE";
			//	break;
			case CAD_GEOMETRY_EXTRUSION:
				return "EXTRUSION";
				break;
			case CAD_GEOMETRY_FACE:
				return "FACE";
				break;

			//case CAD_GEOMETRY_EXTRUSION_SURFACE:
			//	return "EXTRUSION_SURFACE";
			//	break;
			case CAD_GEOMETRY_NONE:
				return "NONE";
				break;

			default:
				char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
				string temp_string = "Function CADGeometryType_string was passed " + 
					std::string(itoa(in_CADGeometryType_enum, temp_char_array, 10)) + 
					" which is an erroneous type.  Allowed enum values are CAD_GEOMETRY_POLYGON, CAD_GEOMETRY_CYLINDER, CAD_GEOMETRY_CONCENTRIC_CIRCLES, CAD_GEOMETRY_CYLINDER, CAD_GEOMETRY_CYLINDER_SURFACE, CAD_GEOMETRY_SPHERE, CAD_GEOMETRY_SPHERE_SURFACE, CAD_GEOMETRY_EXTRUSION, and CAD_GEOMETRY_EXTRUSION_SURFACE.";
				throw isis::application_exception(temp_string.c_str());
	  }
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	 e_CADFeatureGeometryType CADFeatureGeometryType_enum( const string &in_CADFeatureGeometryType_string )
																throw (isis::application_exception)
	 {
		  std::string cADFeatureGeometryType_string = ConvertToUpperCase(in_CADFeatureGeometryType_string);

		  if ( cADFeatureGeometryType_string.compare("POINT") == 0 )
			  return CAD_FEATURE_GEOMETRY_POINT;
		  //else if ( cADFeatureGeometryType_string.compare("FACE") == 0 )
		//	  return CAD_FEATURE_GEOMETRY_FACE;
		 
		  string temp_string = "Function CADFeatureGeometryType_enum was passed " + in_CADFeatureGeometryType_string + " which is an erroneous type.";
		  throw isis::application_exception(temp_string.c_str());
	 }


	std::string CADFeatureGeometryType_string(  e_CADFeatureGeometryType in_CADFeatureGeometryType_enum)
											throw (isis::application_exception)
	{
		switch (in_CADFeatureGeometryType_enum )
		{
			case CAD_FEATURE_GEOMETRY_POINT:
				return "POINT";
				break;
			//case CAD_FEATURE_GEOMETRY_FACE:
			//	return "FACE";
			//	break;

			default:
				char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
				string temp_string = "Function CADFeatureGeometryType_string was passed " + 
					std::string(itoa(in_CADFeatureGeometryType_enum, temp_char_array, 10)) + 
					" which is an erroneous type.  Allowed enum values are CAD_FEATURE_GEOMETRY_POINT.";
				throw isis::application_exception(temp_string.c_str());
	  }
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	 e_CADPrimaryGeometryQualifier CADPrimaryGeometryQualifier_enum( const string &in_CADPrimaryGeometryQualifier_string )
																throw (isis::application_exception)
	 {
		  std::string CADPrimaryGeometryQualifier_string = ConvertToUpperCase(in_CADPrimaryGeometryQualifier_string);

		  if (      CADPrimaryGeometryQualifier_string.compare("INTERIORONLY") == 0 )			return CAD_INTERIOR_ONLY;
		  else if ( CADPrimaryGeometryQualifier_string.compare("BOUNDARYONLY") == 0 )			return CAD_BOUNDARY_ONLY;
		  else if ( CADPrimaryGeometryQualifier_string.compare("INTERIORANDBOUNDARY") == 0 ) return CAD_INTERIOR_AND_BOUNDARY;
		 
		  string temp_string = "Function CADPrimaryGeometryQualifier_enum was passed " + in_CADPrimaryGeometryQualifier_string + " which is an erroneous type.";
		  throw isis::application_exception(temp_string.c_str());
	 }


	std::string CADPrimaryGeometryQualifier_string(  e_CADPrimaryGeometryQualifier in_CADPrimaryGeometryQualifier_enum)
											throw (isis::application_exception)
	{
		switch (in_CADPrimaryGeometryQualifier_enum )
		{
			case CAD_INTERIOR_ONLY:
				return "InteriorOnly";
				break;
			case CAD_BOUNDARY_ONLY:
				return "BoundaryOnly";
				break;
			case CAD_INTERIOR_AND_BOUNDARY:
				return "InteriorAndBoundary";
				break;
			default:
				char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
				string temp_string = "Function CADPrimaryGeometryQualifier_string was passed " + 
					std::string(itoa(in_CADPrimaryGeometryQualifier_enum, temp_char_array, 10)) + 
					" which is an erroneous type.  Allowed enum values are CAD_INTERIOR_ONLY, CAD_BOUNDARY_ONLY, and CAD_INTERIOR_AND_BOUNDARY.";
				throw isis::application_exception(temp_string.c_str());
	  }
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	e_CADSecondaryGeometryQualifier CADSecondaryGeometryQualifier_enum( const string &in_CADSecondaryGeometryQualifier_string )
																throw (isis::application_exception)
	 {
		  std::string CADSecondaryGeometryQualifier_string = ConvertToUpperCase(in_CADSecondaryGeometryQualifier_string);

		  if (      CADSecondaryGeometryQualifier_string.compare("INCLUDEENDCAPS") == 0 )			return CAD_INCLUDE_END_CAPS;
		  else if ( CADSecondaryGeometryQualifier_string.compare("EXCLUDEENDCAPS") == 0 )			return CAD_EXCLUDE_END_CAPS;
		  else if ( CADSecondaryGeometryQualifier_string.compare("") == 0 )			return CAD_SECONDARY_GEOMETRY_QUALIFIER_NONE;

		  string temp_string = "Function CADSecondaryGeometryQualifier_enum was passed " + in_CADSecondaryGeometryQualifier_string + " which is an erroneous type.";
		  throw isis::application_exception(temp_string.c_str());
	 }


	std::string CADSecondaryGeometryQualifier_string(  e_CADSecondaryGeometryQualifier in_CADSecondaryGeometryQualifier_enum)
											throw (isis::application_exception)
	{
		switch (in_CADSecondaryGeometryQualifier_enum )
		{
			case CAD_INCLUDE_END_CAPS:
				return "IncludeEndCaps";
				break;
			case CAD_EXCLUDE_END_CAPS:
				return "ExcludEendCaps";
				break;
			case CAD_SECONDARY_GEOMETRY_QUALIFIER_NONE:
				return "";
				break;

			default:
				char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
				string temp_string = "Function CADSecondaryGeometryQualifier_string was passed " + 
					std::string(itoa(in_CADSecondaryGeometryQualifier_enum, temp_char_array, 10)) + 
					" which is an erroneous type.  Allowed enum values are CAD_INCLUDE_END_CAPS, CAD_EXCLUDE_END_CAPS, and CAD_SECONDARY_GEOMETRY_QUALIFIER_NONE";
				throw isis::application_exception(temp_string.c_str());
	  }
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	 e_MaterialType MaterialType_enum( const string &in_MaterialType_string )
																throw (isis::application_exception)
	 {
		  std::string Units_string = ConvertToUpperCase(in_MaterialType_string);

		  if ( Units_string.compare("STEEL") == 0 )					return MATERIAL_STEEL;
		  else if ( Units_string.compare("ALUMINUM") == 0 )			return MATERIAL_ALUMINUM;
		  else if ( Units_string.compare("PLASTIC") == 0 )			return MATERIAL_PLASTIC;
		  else if ( Units_string.compare("CERAMIC") == 0 )			return MATERIAL_CERAMIC;
		  else if ( Units_string.compare("POLYMER") == 0 )			return MATERIAL_POLYMER;
		  else if ( Units_string.compare("TITANIUM") == 0 )			return MATERIAL_TITANIUM;
		  else if ( Units_string.compare("STAINLESS") == 0 )	    return MATERIAL_STAINLESS;
		  else if ( Units_string.compare("COMPOSITE_FIBER") == 0 )	return MATERIAL_COMPOSITE_FIBER;
		  else if ( Units_string.compare("COMPOSITE FIBER") == 0 )	return MATERIAL_COMPOSITE_FIBER;  // Material library had this with and without the "_".
		  else if ( Units_string.compare("HARD_PLASTIC") == 0 )	    return MATERIAL_HARD_PLASTIC;
		  else if ( Units_string.compare("HARD PLASTIC") == 0 )	    return MATERIAL_HARD_PLASTIC;  // Support the space, just in case.
		  else if ( Units_string.compare("FUEL") == 0 )				return MATERIAL_FUEL;
		  else if ( Units_string.compare("FLUID") == 0 )			return MATERIAL_FLUID;

		 
		  string temp_string = "Function MaterialType_enum was passed " + in_MaterialType_string + " which is an erroneous type.";
		  throw isis::application_exception(temp_string.c_str());
	 }



	std::string MaterialType_string(  e_MaterialType in_MaterialType_enum)
											throw (isis::application_exception)
	{
		switch (in_MaterialType_enum )
		{
			case MATERIAL_STEEL:
				return "STEEL";
				break;
			case MATERIAL_ALUMINUM:
				return "ALUMINUM";
				break;
			case MATERIAL_PLASTIC:
				return "PLASTIC";
				break;
			case MATERIAL_CERAMIC:
				return "CERAMIC";
				break;
			case MATERIAL_POLYMER:
				return "POLYMER";
				break;
			case MATERIAL_TITANIUM:
				return "POLYMER";
				break;
			case MATERIAL_STAINLESS:
				return "STAINLESS";
				break;
			case MATERIAL_COMPOSITE_FIBER:
				return "COMPOSITE_FIBER";
				break;
			case MATERIAL_HARD_PLASTIC:
				return "HARD_PLASTIC";
				break;
			case MATERIAL_FUEL:
				return "FUEL";
				break;
			case MATERIAL_FLUID:
				return "FLUID";
				break;

			default:
				char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
				string temp_string = "Function MaterialType_string was passed " + 
					std::string(itoa(in_MaterialType_enum, temp_char_array, 10)) + 
					" which is an erroneous type.  Allowed enum values are MATERIAL_STEEL, MATERIAL_ALUMINUM, MATERIAL_PLASTIC, MATERIAL_CERAMIC, MATERIAL_POLYMER, MATERIAL_TITANIUM, MATERIAL_STAINLESS, MATERIAL_COMPOSITE_FIBER, MATERIAL_HARD_PLASTIC, MATERIAL_FUEL, and MATERIAL_FLUID.";
				throw isis::application_exception(temp_string.c_str());
	  }
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	 e_DataExchangeFormat DataExchangeFormat_enum( const string &in_DataExchangeFormat_string )
																throw (isis::application_exception)
	 {
		  std::string DataExchangeFormat = ConvertToUpperCase(in_DataExchangeFormat_string);

		  if ( DataExchangeFormat.compare("STEP") == 0 )					return DATA_EXCHANGE_FORMAT_STEP;
		  else if ( DataExchangeFormat.compare("STEREOLITHOGRAPHY") == 0 )	return DATA_EXCHANGE_FORMAT_STEREOLITHOGRAPHY;
		  else if ( DataExchangeFormat.compare("INVENTOR") == 0 )	        return DATA_EXCHANGE_FORMAT_INVENTOR;	
		  else if ( DataExchangeFormat.compare("PARASOLID") == 0 )	        return DATA_EXCHANGE_FORMAT_PARASOLID;
		  

		  string temp_string = "Function DataExchangeFormat_enum was passed " + in_DataExchangeFormat_string + " which is an erroneous type.";
		  throw isis::application_exception(temp_string.c_str());
	 }


	std::string DataExchangeFormat_string(  e_DataExchangeFormat in_DataExchangeFormat_enum)
											throw (isis::application_exception)
	{
		switch (in_DataExchangeFormat_enum )
		{
			case DATA_EXCHANGE_FORMAT_STEP:
				return "STEP";
				break;
			case DATA_EXCHANGE_FORMAT_STEREOLITHOGRAPHY:
				return "STEREOLITHOGRAPHY";
				break;
			case DATA_EXCHANGE_FORMAT_INVENTOR:
				return "INVENTOR";
				break;
			case DATA_EXCHANGE_FORMAT_PARASOLID:
				return "PARASOLID";
				break;
			default:
				char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
				string temp_string = "Function DataExchangeFormat_string was passed " + 
					std::string(itoa(in_DataExchangeFormat_enum, temp_char_array, 10)) + 
					" which is an erroneous type.  Allowed enum value DATA_EXCHANGE_FORMAT_STEP, DATA_EXCHANGE_FORMAT_STEREOLITHOGRAPHY, and DATA_EXCHANGE_FORMAT_INVENTOR.";
				throw isis::application_exception(temp_string.c_str());
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	 e_DataExchangeVersion DataExchangeVersion_enum( const string &in_DataExchangeVersion_string )
																throw (isis::application_exception)
	 {
		  std::string DataExchangeVersion_string = ConvertToUpperCase(in_DataExchangeVersion_string);

		  if ( DataExchangeVersion_string.compare("N/A") == 0 ) return DATA_EXCHANGE_VERSION_NOT_APPLICABLE;
		  else if ( DataExchangeVersion_string.compare("") == 0 ) return DATA_EXCHANGE_VERSION_NOT_APPLICABLE;
		  else if ( DataExchangeVersion_string.compare("AP203_SINGLE_FILE") == 0 )				return AP203_SINGLE_FILE;
		  else if ( DataExchangeVersion_string.compare("AP203_E2_SINGLE_FILE") == 0 )			return AP203_E2_SINGLE_FILE;
		  else if ( DataExchangeVersion_string.compare("AP203_E2_SEPERATE_PART_FILES") == 0 )	return AP203_E2_SEPARATE_PART_FILES;
		  else if ( DataExchangeVersion_string.compare("AP203_E2_SEPARATE_PART_FILES") == 0 )	return AP203_E2_SEPARATE_PART_FILES;
		  else if ( DataExchangeVersion_string.compare("AP214_SINGLE_FILE") == 0 )				return AP214_SINGLE_FILE;
		  else if ( DataExchangeVersion_string.compare("AP214_SEPARATE_PART_FILES") == 0 )		return AP214_SEPARATE_PART_FILES;
		  else if ( DataExchangeVersion_string.compare("AP214_SEPARATE_PART_FILES") == 0 )		return AP214_SEPARATE_PART_FILES;
		  else if ( DataExchangeVersion_string.compare("ASCII") == 0 )							return ASCII;
		  else if ( DataExchangeVersion_string.compare("BINARY") == 0 )							return BINARY;
		 
		  string temp_string = "Function DataExchangeVersion_enum was passed " + in_DataExchangeVersion_string + " which is an erroneous type.";
		  throw isis::application_exception(temp_string.c_str());
	 }


	std::string DataExchangeVersion_string(  e_DataExchangeVersion in_DataExchangeVersion_enum)
											throw (isis::application_exception)
	{
		switch (in_DataExchangeVersion_enum )
		{
			case DATA_EXCHANGE_VERSION_NOT_APPLICABLE:
				return "N/A";
				break;
			case AP203_SINGLE_FILE:
				return "AP203_SINGLE_FILE";
				break;
			case AP203_E2_SINGLE_FILE:
				return "AP203_E2_SINGLE_FILE";
				break;
			case AP203_E2_SEPARATE_PART_FILES:
				return "AP203_E2_SEPARATE_PART_FILES";
				break;
			case AP214_SINGLE_FILE:
				return "AP214_SINGLE_FILE";
				break;
			case AP214_SEPARATE_PART_FILES:
				return "AP214_SEPARATE_PART_FILES";
				break;
			case ASCII:
				return "ASCII";
				break;
			case BINARY:
				return "BINARY";
				break;
			default:
				char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
				string temp_string = "Function DataExchangeVersion_string was passed " + 
					std::string(itoa(in_DataExchangeVersion_enum, temp_char_array, 10)) + 
					" which is an erroneous type.  Allowed enum values are DATA_EXCHANGE_VERSION_NOT_APPLICABLE, AP203_SINGLE_FILE, AP203_E2_SINGLE_FILE, AP203_E2_SEPARATE_PART_FILES, AP214_SINGLE_FILE, AP214_SEPARATE_PART_FILES, STEREOLITHOGRAPHY_ASCII and STEREOLITHOGRAPHY_BINARY.";
				throw isis::application_exception(temp_string.c_str());
	  }
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	 e_DataExchangeSpecialInstruction DataExchangeSpecialInstruction_enum( const string &in_DataExchangeSpecialInstruction_string )
																throw (isis::application_exception)
	 {
		  std::string DataExchangeSpecialInstruction_string = ConvertToUpperCase(in_DataExchangeSpecialInstruction_string);

		  if ( DataExchangeSpecialInstruction_string.compare("LEAFASSEMBLIESMETRICS") == 0 ) return DATA_EXCHANGE_LEAF_ASSEMBLIES_METRICS;
		 
		  string temp_string = "Function DataExchangeSpecialInstruction_enum was passed " + in_DataExchangeSpecialInstruction_string + " which is an erroneous type.";
		  throw isis::application_exception(temp_string.c_str());
	 }



	std::string DataExchangeSpecialInstruction_string(  e_DataExchangeSpecialInstruction in_DataExchangeSpecialInstruction_enum)
											throw (isis::application_exception)
	{
		switch (in_DataExchangeSpecialInstruction_enum )
		{
			case DATA_EXCHANGE_LEAF_ASSEMBLIES_METRICS:
				return "LeafAssembliesMetrics";
				break;

			default:
				char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
				string temp_string = "Function DataExchangeSpecialInstruction_string was passed " + 
					std::string(itoa(in_DataExchangeSpecialInstruction_enum, temp_char_array, 10)) + 
					" which is an erroneous type.  Allowed enum value is DATA_EXCHANGE_LEAF_ASSEMBLIES_METRICS.";
				throw isis::application_exception(temp_string.c_str());
	  }
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	 e_ComputationType ComputationType_enum( const string &in_ComputationType_string )
																throw (isis::application_exception)
	 {
		  
		  std::string ComputationType_string = ConvertToUpperCase(in_ComputationType_string);

		  if ( ComputationType_string.compare("BOUNDINGBOX") == 0 )  return COMPUTATION_BOUNDING_BOX;
		  else if ( ComputationType_string.compare("CG") == 0 )		  return COMPUTATION_CG;
		  else if ( ComputationType_string.compare("CENTEROFGRAVITY") == 0 )		  return COMPUTATION_CG;
		  else if ( ComputationType_string.compare("POINT") == 0 )	  return COMPUTATION_POINT;
		  else if ( ComputationType_string.compare("POINTCOORDINATES") == 0 )	  return COMPUTATION_POINT;
		  else if ( ComputationType_string.compare("MASS") == 0 )	  return COMPUTATION_MASS;
		  else if ( ComputationType_string.compare("INTERFERENCECOUNT") == 0 )	  return COMPUTATION_INTERFERENCE_COUNT;
		  else if ( ComputationType_string.compare("COEFFICIENTOFDRAG") == 0 )	  return COMPUTATION_COEFFICIENT_OF_DRAG;
		  else if ( ComputationType_string.compare("PLANE") == 0 )	  return COMPUTATION_PLANE;
		  else	if ( ComputationType_string.compare("MISES") == 0 )  return COMPUTATION_STRESS_MISES;
		  else	if ( ComputationType_string.compare("SHEAR") == 0 )  return COMPUTATION_STRESS_SHEAR;
		  else	if ( ComputationType_string.compare("BEARING") == 0 )  return COMPUTATION_STRESS_BEARING;
		  else	if ( ComputationType_string.compare("VONMISESSTRESS") == 0 )  return COMPUTATION_STRESS_MISES;
		  else	if ( ComputationType_string.compare("SHEARSTRESS") == 0 )  return COMPUTATION_STRESS_SHEAR;
		  else	if ( ComputationType_string.compare("BEARINGSTRESS") == 0 )  return COMPUTATION_STRESS_BEARING;
		  else	if ( ComputationType_string.compare("FACTOROFSAFETY") == 0 )  return COMPUTATION_FACTOR_OF_SAFETY;	
		  else	if ( ComputationType_string.compare("TOTALSHOTS") == 0 )  return COMPUTATION_TOTAL_SHOTS;	
		  else	if ( ComputationType_string.compare("TOTALINTERSECTIONS") == 0 )  return COMPUTATION_TOTAL_INTERSECTIONS;	
		  else	if ( ComputationType_string.compare("TOTALPERFORATIONS") == 0 )  return COMPUTATION_TOTAL_PERFORATIONS;	
		  else	if ( ComputationType_string.compare("TOTALKILLS") == 0 )  return COMPUTATION_TOTAL_KILLS;
		  else	if ( ComputationType_string.compare("MAXVERTICALJUMP") == 0 )  return MAX_VERTICAL_JUMP;
		  else	if ( ComputationType_string.compare("MAXVERTICALVELOCITY") == 0 )  return MAX_VERTICAL_VELOCITY;
		  else	if ( ComputationType_string.compare("MAXHORIZONALVELOCITY") == 0 )  return MAX_HORIZONAL_VELOCITY;
		  else	if ( ComputationType_string.compare("MINIMUMTEMPERATURE") == 0 )  return MINIMUM_TEMPERATURE;
		  else	if ( ComputationType_string.compare("MAXIMUMTEMPERATURE") == 0 )  return MAXIMUM_TEMPERATURE;

		  string temp_string = "Function ComputationType_enum was passed " + in_ComputationType_string + " which is an erroneous type.";
		  throw isis::application_exception(temp_string.c_str());
	 }


	std::string ComputationType_string(  e_ComputationType in_ComputationType_enum)
											throw (isis::application_exception)
	{
		switch (in_ComputationType_enum )
		{
			case COMPUTATION_BOUNDING_BOX:
				return "BoundingBox";
				break;
			case COMPUTATION_CG:
				return "CG";
				break;
			case COMPUTATION_POINT:
				return "Point";
				break;
			case COMPUTATION_MASS:
				return "Mass";
				break;
			case COMPUTATION_INTERFERENCE_COUNT:
				return "InterferencCount";
				break;
			case COMPUTATION_COEFFICIENT_OF_DRAG:
				return "CoefficientOfDrag";
				break;
			case COMPUTATION_PLANE:
				return "Plane";
				break;
			case COMPUTATION_STRESS_MISES:
				return "VonMisesStress";
				break;
			case COMPUTATION_STRESS_SHEAR:
				return "ShearStress";
				break;
			case COMPUTATION_STRESS_BEARING:
				return "BearingStress";
				break;
			case COMPUTATION_FACTOR_OF_SAFETY:
				return "FactorOfSafety";
				break;
			case COMPUTATION_TOTAL_SHOTS:
				return "TotalShots";
				break;
			case COMPUTATION_TOTAL_INTERSECTIONS:
				return "TotalIntersections";
				break;
			case COMPUTATION_TOTAL_PERFORATIONS:
				return "TotalPerforations";
				break;	
			case COMPUTATION_TOTAL_KILLS:
				return "TotalKills";
				break;	
			case MAX_VERTICAL_JUMP:
				return "MaxVerticalJump";
				break;	
			case MAX_VERTICAL_VELOCITY:
				return "MaxVerticalVelocity";
				break;	
			case MAX_HORIZONAL_VELOCITY:
				return "MaxHorizonalVelocity";
				break;	

			case MINIMUM_TEMPERATURE:
				return "MinimumTemperature";
				break;	

			case MAXIMUM_TEMPERATURE:
				return "MaximumTemperature";
				break;	

			default:
				char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
				std::stringstream errorString;
				errorString << "Function ComputationType_string was passed " <<  std::string(itoa(in_ComputationType_enum, temp_char_array, 10)) << " which is an erroneous type. " << 
					   std::endl << "Allowed enum values are: COMPUTATION_BOUNDING_BOX, COMPUTATION_CG, Z_COORDINATE, COMPUTATION_MASS, COMPUTATION_POINT," <<
					   std::endl << "COMPUTATION_PLANE, COMPUTATION_COEFFICIENT_OF_DRAG, COMPUTATION_STRESS_MISES, COMPUTATION_STRESS_SHEAR, "  <<
					   std::endl << "COMPUTATION_STRESS_BEARING, COMPUTATION_FACTOR_OF_SAFETY, COMPUTATION_TOTAL_INTERSECTIONS, COMPUTATION_TOTAL_KILLS, " <<
					   std::endl <<  "MAX_VERTICAL_JUMP, MAX_VERTICAL_VELOCITY, MAX_HORIZONAL_VELOCITY, MINIMUM_TEMPERATURE, and MAXIMUM_TEMPERATURE.";
				throw isis::application_exception(errorString.str());

	  }
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	 e_ComputationSubType ComputationSubType_enum( const string &in_ComputationSubType_string )
																throw (isis::application_exception)
	 {
		  std::string ComputationSubType_string = ConvertToUpperCase(in_ComputationSubType_string);

		  if ( ComputationSubType_string.compare("NONE") == 0 )  return COMPUTATION_SUBTYPE_NONE;
		  else if ( ComputationSubType_string.compare("GROUND") == 0 )  return COMPUTATION_SUBTYPE_GROUND;
		  //else if ( ComputationSubType_string.compare("WATERLINE") == 0 )  return COMPUTATION_SUBTYPE_WATERLINE;

			
		  string temp_string = "Function ComputationSubType_enum was passed " + in_ComputationSubType_string + " which is an erroneous type.";
		  throw isis::application_exception(temp_string.c_str());
	 }


	std::string ComputationSubType_string(  e_ComputationSubType in_ComputationSubType_enum)
											throw (isis::application_exception)
	{
		switch (in_ComputationSubType_enum )
		{
			case COMPUTATION_SUBTYPE_GROUND:
				return "GROUND";
				break;
			//case COMPUTATION_SUBTYPE_WATERLINE:
			//	return "WATERLINE";
			//	break;
			case COMPUTATION_SUBTYPE_NONE:
				return "NONE";
				break;
			default:
				char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
				string temp_string = "Function ComputationSubType_string was passed " + 
					std::string(itoa(in_ComputationSubType_enum, temp_char_array, 10)) + 
					" which is an erroneous type.  Allowed enum values are COMPUTATION_SUBTYPE_GROUND,  or COMPUTATION_SUBTYPE_NONE.";
				throw isis::application_exception(temp_string.c_str());
	  }
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	 e_ComputationDimension ComputationDimension_enum( const string &in_ComputationDimension_string )
																throw (isis::application_exception)
	 {
		  std::string ComputationDimension_string = ConvertToUpperCase(in_ComputationDimension_string);

		  if ( ComputationDimension_string.compare("X_COORDINATE") == 0 )		  return COMPUTATION_X_COORDINATE;
		  else if ( ComputationDimension_string.compare("Y_COORDINATE") == 0 )	  return COMPUTATION_Y_COORDINATE;
		  else if ( ComputationDimension_string.compare("Z_COORDINATE") == 0 )	  return COMPUTATION_Z_COORDINATE;
		  else if ( ComputationDimension_string.compare("VECTOR") == 0 )	      return COMPUTATION_VECTOR;
		  else if ( ComputationDimension_string.compare("SCALAR") == 0 )	      return COMPUTATION_SCALAR;
		  else if ( ComputationDimension_string.compare("BOOLEAN") == 0 )	      return COMPUTATION_BOOLEAN;
		  else if ( ComputationDimension_string.compare("POINTS") == 0 )	      return COMPUTATION_POINTS;
		  //Assume an empty string means Scalar
		  else if ( ComputationDimension_string.compare("") == 0 )	      return COMPUTATION_DIMENSION_NONE;
		 
		  string temp_string = "Function ComputationDimension_enum was passed " + in_ComputationDimension_string + " which is an erroneous type.";
		  throw isis::application_exception(temp_string.c_str());
	 }


	std::string ComputationDimension_string(  e_ComputationDimension in_ComputationDimension_enum)
											throw (isis::application_exception)
	{
		switch (in_ComputationDimension_enum )
		{
			case COMPUTATION_X_COORDINATE:
				return "X_COORDINATE";
				break;
			case COMPUTATION_Y_COORDINATE:
				return "Y_COORDINATE";
				break;
			case COMPUTATION_Z_COORDINATE:
				return "Z_COORDINATE";
				break;
			case COMPUTATION_VECTOR:
				return "VECTOR";
				break;
			case COMPUTATION_SCALAR:
				return "SCALAR";
				break;
			case COMPUTATION_BOOLEAN:
				return "BOOLEAN";
				break;
			case COMPUTATION_POINTS:
				return "POINTS";
				break;

			case COMPUTATION_DIMENSION_NONE:
				return "";
				break;
			default:
				char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
				string temp_string = "Function ComputationDimension_string was passed " + 
					std::string(itoa(in_ComputationDimension_enum, temp_char_array, 10)) + 
					" which is an erroneous type.  Allowed enum values are COMPUTATION_X_COORDINATE, COMPUTATION_Y_COORDINATE, Z_COORDINATE, COMPUTATION_VECTOR, COMPUTATION_POINTS, COMPUTATION_SCALAR.";
				throw isis::application_exception(temp_string.c_str());
	  }
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	 e_GeometrySetOperation GeometrySetOperation_enum( const string &in_GeometrySetOperation_string )
																throw (isis::application_exception)
	 {
		  std::string GeometrySetOperation_string = ConvertToUpperCase(in_GeometrySetOperation_string);

		  if ( GeometrySetOperation_string.compare("+") == 0 )				  return CAD_GEOMETRY_SET_PLUS;
		  else if ( GeometrySetOperation_string.compare("-") == 0 )		  return CAD_GEOMETRY_SET_MINUS;
		  else if ( GeometrySetOperation_string.compare("INTERSECTION") == 0 )		  return CAD_GEOMETRY_SET_INTERSECTION;
		 
		  string temp_string = "Function GeometrySetOperation_enum was passed " + in_GeometrySetOperation_string + " which is an erroneous type.";
		  throw isis::application_exception(temp_string.c_str());
	 }



	std::string GeometrySetOperation_string(  e_GeometrySetOperation in_GeometrySetOperation_enum)
											throw (isis::application_exception)
	{
		switch (in_GeometrySetOperation_enum )
		{
			case CAD_GEOMETRY_SET_PLUS:
				return "+";
				break;
			case CAD_GEOMETRY_SET_MINUS:
				return "-";
				break;
			case CAD_GEOMETRY_SET_INTERSECTION:
				return "INTERSECTION";
				break;

			default:
				char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
				string temp_string = "Function GeometrySetOperation_string was passed " + 
					std::string(itoa(in_GeometrySetOperation_enum, temp_char_array, 10)) + 
					" which is an erroneous type.  Allowed enum values are CAD_GEOMETRY_SET_PLUS, CAD_GEOMETRY_SET_MINUS, and CAD_GEOMETRY_SET_INTERSECTION.";
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
					std::string(itoa(in_CoordinatePlaneAlignment_enum, temp_char_array, 10)) + 
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
					std::string(itoa(in_CoordinateAxisAlignment_enum, temp_char_array, 10)) + 
					" which is an erroneous type.  Allowed enum values are PRO_CSYS_AXIS_X, PRO_CSYS_AXIS_Y, and PRO_CSYS_AXIS_Z.";
				throw isis::application_exception(temp_string.c_str());
	  }
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	 e_CADFileType CadFileType_enum( const string &in_CadFileType_string )
																throw (isis::application_exception)
	 {
		std::string CadFileType_string = ConvertToUpperCase(in_CadFileType_string);

		if ( CadFileType_string.compare("CREO") == 0 )			return CAD_CREO;
		else if ( CadFileType_string.compare("STEP") == 0 )		return CAD_STEP;
		 
		string temp_string = "Function CadFileType_enum was passed " + in_CadFileType_string + " which is an erroneous type.";
		throw isis::application_exception(temp_string.c_str());
	 }


	std::string CadFileType_string(  e_CADFileType in_CadFileType_enum)
											throw (isis::application_exception)
	{
		switch (in_CadFileType_enum )
		{
			case CAD_CREO:
				return "CREO";
				break;
			case CAD_STEP:
				return "STEP";
				break;
			default:
				char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
				string temp_string = "Function CadFileType_string was passed " + 
					std::string(itoa(in_CadFileType_enum, temp_char_array, 10)) + 
					" which is an erroneous type.  Allowed enum values are CAD_CREO and CAD_STEP.";
				throw isis::application_exception(temp_string.c_str());
	  }
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	 e_CADMetricsError CADMetricsError_enum( const string &in_CADMetricsError_string )
																throw (isis::application_exception)
	 {
		std::string CADMetricsError_string = ConvertToUpperCase(in_CADMetricsError_string);

		if ( CADMetricsError_string.compare("MASS_PROPERTIES_NOT_DEFINED") == 0 )			return CAD_MASS_PROPERTIES_NOT_DEFINED;
		else if ( CADMetricsError_string.compare("INTERIA_AT_DEFAULT_CSYS_NOT_DEFINED") == 0 )	return CAD_INTERIA_AT_DEFAULT_CSYS_NOT_DEFINED;
		else if ( CADMetricsError_string.compare("INTERIA_AT_CENTER_OF_GRAVITY_NOT_DEFINED") == 0 )	return CAD_INTERIA_AT_CENTER_OF_GRAVITY_NOT_DEFINED;
		else if ( CADMetricsError_string.compare("SURFACE_AREA_NOT_DEFINED") == 0 )		return CAD_SURFACE_AREA_NOT_DEFINED;
		 
		string temp_string = "Function CADMetricsError_enum was passed " + in_CADMetricsError_string + " which is an erroneous type.";
		throw isis::application_exception(temp_string.c_str());
	 }


	std::string CADMetricsError_string(  e_CADMetricsError in_CADMetricsError_enum)
											throw (isis::application_exception)
	{
		switch (in_CADMetricsError_enum )
		{
			case CAD_MASS_PROPERTIES_NOT_DEFINED:
				return "MASS_PROPERTIES_NOT_DEFINED";
				break;
			case CAD_INTERIA_AT_DEFAULT_CSYS_NOT_DEFINED:
				return "INTERIA_AT_DEFAULT_CSYS_NOT_DEFINED";
				break;
			case CAD_INTERIA_AT_CENTER_OF_GRAVITY_NOT_DEFINED:
				return "INTERIA_AT_CENTER_OF_GRAVITY_NOT_DEFINED";
				break;
			case CAD_SURFACE_AREA_NOT_DEFINED:
				return "SURFACE_AREA_NOT_DEFINED";
				break;
			default:
				char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
				string temp_string = "Function CADMetricsError_string was passed " + 
					std::string(itoa(in_CADMetricsError_enum, temp_char_array, 10)) + 
					" which is an erroneous type.  Allowed enum values are CAD_MASS_PROP_NOT_DEFINED, CAD_INTERIA_AT_DEFAULT_CSYS_NOT_DEFINED, CAD_INTERIA_AT_CENTER_OF_GRAVITY_NOT_DEFINED, and CAD_SURFACE_AREA_NOT_DEFINED.";
				throw isis::application_exception(temp_string.c_str());
	  }
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	 e_CADInertiaAt CADInertiaAt_enum( const string &in_CADInertiaAt_string )
																throw (isis::application_exception)
	 {
		std::string CADInertiaAt_string = ConvertToUpperCase(in_CADInertiaAt_string);

		if ( CADInertiaAt_string.compare("DEFAULT_CSYS") == 0 )			return CAD_DEFAULT_CSYS;
		else if ( CADInertiaAt_string.compare("CENTER_OF_GRAVITY") == 0 )		return CAD_CENTER_OF_GRAVITY ;
		 
		string temp_string = "Function CADInertiaAt_enum was passed " + in_CADInertiaAt_string + " which is an erroneous type.";
		throw isis::application_exception(temp_string.c_str());
	 }


	std::string CADInertiaAt_string(  e_CADInertiaAt in_CADInertiaAt_enum)
											throw (isis::application_exception)
	{
		switch (in_CADInertiaAt_enum )
		{
			case CAD_DEFAULT_CSYS:
				return "DEFAULT_CSYS";
				break;
			case CAD_CENTER_OF_GRAVITY:
				return "CENTER_OF_GRAVITY";
				break;
			default:
				char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
				string temp_string = "Function CADInertiaAt_string was passed " + 
					std::string(itoa(in_CADInertiaAt_enum, temp_char_array, 10)) + 
					" which is an erroneous type.  Allowed enum values are CAD_DEFAULT_CSYS and CAD_CENTER_OF_GRAVITY.";
				throw isis::application_exception(temp_string.c_str());
	  }
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	 e_ReferencePlane ReferencePlane_enum( const string &in_ReferencePlane_string )
																throw (isis::application_exception)
	 {
		std::string ReferencePlane_string = ConvertToUpperCase(in_ReferencePlane_string);
		if ( ReferencePlane_string.compare("NONE") == 0 )			return PLANE_NONE;
		else if ( ReferencePlane_string.compare("GROUND") == 0 )	return PLANE_GROUND;
		else if ( ReferencePlane_string.compare("WATERLINE") == 0 )	return PLANE_WATERLINE ;
		 
		string temp_string = "Function ReferencePlane_enum was passed " + in_ReferencePlane_string + " which is an erroneous type.";
		throw isis::application_exception(temp_string.c_str());
	 }


	std::string ReferencePlane_string(  e_ReferencePlane in_ReferencePlane_enum)
											throw (isis::application_exception)
	{
		switch (in_ReferencePlane_enum )
		{
			case PLANE_NONE:
				return "NONE";
				break;
			case PLANE_GROUND:
				return "GROUND";
				break;
			case PLANE_WATERLINE:
				return "WATERLINE";
				break;

			default:
				char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
				string temp_string = "Function ReferencePlane_string was passed " + 
					std::string(itoa(in_ReferencePlane_enum, temp_char_array, 10)) + 
					" which is an erroneous type.  Allowed enum values are NONE, PLANE_GROUND, and PLANE_WATERLINE.";
				throw isis::application_exception(temp_string.c_str());
	  }
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	 e_ComponentDefinitionSource ComponentDefinitionSource_enum( const string &in_ComponentDefinitionSource_string )
																throw (isis::application_exception)
	 {
		std::string ComponentDefinitionSource_string = ConvertToUpperCase(in_ComponentDefinitionSource_string);
		if ( ComponentDefinitionSource_string.compare("CYPHY") == 0 )			return COMPONENT_DEFINITION_SOURCE_CYPHY;
		else if ( ComponentDefinitionSource_string.compare("CAD_MODEL") == 0 )	return COMPONENT_DEFINITION_SOURCE_CAD_MODEL;
		//else if ( ComponentDefinitionSource_string.compare("WATERLINE") == 0 )	return WATERLINE ;
		 
		string temp_string = "Function ComponentDefinitionSource_enum was passed " + in_ComponentDefinitionSource_string + " which is an erroneous type.";
		throw isis::application_exception(temp_string.c_str());
	 }


	std::string ComponentDefinitionSource_string(  e_ComponentDefinitionSource in_ComponentDefinitionSource_enum)
											throw (isis::application_exception)
	{
		switch (in_ComponentDefinitionSource_enum )
		{
			case COMPONENT_DEFINITION_SOURCE_CYPHY:
				return "CYPHY";
				break;
			case COMPONENT_DEFINITION_SOURCE_CAD_MODEL:
				return "CAD_MODEL";
				break;

			default:
				char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
				string temp_string = "Function ComponentDefinitionSource_string was passed " + 
					std::string(_itoa(in_ComponentDefinitionSource_enum, temp_char_array, 10)) + 
					" which is an erroneous type.  Allowed enum values are COMPONENT_DEFINITION_SOURCE_CYPHY and COMPONENT_DEFINITION_SOURCE_CAD_MODEL.";
				throw isis::application_exception(temp_string.c_str());
	  }
	}


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	 e_CADModelRepresentation CADModelRepresentation_enum( const string &in_CADModelRepresentation_string )
																throw (isis::application_exception)
	 {
		std::string CADModelRepresentation_string = ConvertToUpperCase(in_CADModelRepresentation_string);
		if ( CADModelRepresentation_string.compare("") == 0 )			return CAD_MODEL_FEATURED_REP;
		else if ( CADModelRepresentation_string.compare("FEATURED_REP") == 0 )	return CAD_MODEL_FEATURED_REP;
		else if ( CADModelRepresentation_string.compare("DEFEATURED_REP") == 0 )	return CAD_MODEL_DEFEATURED_REP;
		 
		string temp_string = "Function CADModelRepresentation_enum was passed " + in_CADModelRepresentation_string + " which is an erroneous type.";
		throw isis::application_exception(temp_string.c_str());
	 }


	std::string CADModelRepresentation_string(  e_CADModelRepresentation in_CADModelRepresentation_enum)
											throw (isis::application_exception)
	{
		switch (in_CADModelRepresentation_enum )
		{
			case CAD_MODEL_FEATURED_REP:
				return "Featured_Rep";
				break;
			case CAD_MODEL_DEFEATURED_REP:
				return "Defeatured_Rep";
				break;

			default:
				char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
				string temp_string = "Function CADModelRepresentation_string was passed " + 
					std::string(itoa(in_CADModelRepresentation_enum, temp_char_array, 10)) + 
					" which is an erroneous type.  Allowed enum values are CAD_MODEL_FEATURED_REP and CAD_MODEL_DEFEATURED_REP.";
				throw isis::application_exception(temp_string.c_str());
	  }
	}


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	 e_CADTreatConstraintAsAGuide CADTreatConstraintAsAGuide_enum( const string &in_CADTreatConstraintAsAGuide_string )
																throw (isis::application_exception)
	 {
		std::string CADTreatConstraintAsAGuide_string = ConvertToUpperCase(in_CADTreatConstraintAsAGuide_string);
		if ( CADTreatConstraintAsAGuide_string.compare("") == 0 )			return CAD_TREAT_CONSTRAINT_AS_A_GUIDE_FALSE;
		else if ( CADTreatConstraintAsAGuide_string.compare("TRUE") == 0 )	return CAD_TREAT_CONSTRAINT_AS_A_GUIDE_TRUE;
		else if ( CADTreatConstraintAsAGuide_string.compare("FALSE") == 0 )	return CAD_TREAT_CONSTRAINT_AS_A_GUIDE_FALSE;
		 
		string temp_string = "Function CADTreatConstraintAsAGuide_enum was passed " + in_CADTreatConstraintAsAGuide_string + " which is an erroneous type.";
		throw isis::application_exception(temp_string.c_str());
	 }


	std::string CADTreatConstraintAsAGuide_string(  e_CADTreatConstraintAsAGuide in_CADTreatConstraintAsAGuide_enum)
											throw (isis::application_exception)
	{
		switch (in_CADTreatConstraintAsAGuide_enum )
		{
			case CAD_TREAT_CONSTRAINT_AS_A_GUIDE_TRUE:
				return "TRUE";
				break;
			case CAD_TREAT_CONSTRAINT_AS_A_GUIDE_FALSE:
				return "FALSE";
				break;

			default:
				char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
				string temp_string = "Function CADTreatConstraintAsAGuide_string was passed " + 
					std::string(itoa(in_CADTreatConstraintAsAGuide_enum, temp_char_array, 10)) + 
					" which is an erroneous type.  Allowed enum values are CAD_TREAT_CONSTRAINT_AS_A_GUIDE_TRUE and CAD_TREAT_CONSTRAINT_AS_A_GUIDE_FALSE.";
				throw isis::application_exception(temp_string.c_str());
	  }
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

		
  e_CADJointType CADJointType_enum(const std::string &in_CADJointType_string)
												throw (isis::application_exception)
  {
	  std::string CADJointType_string = ConvertToUpperCase(in_CADJointType_string);

	  if      ( CADJointType_string.compare("FIXED") == 0 )			return FIXED_JOINT;
	  else if ( CADJointType_string.compare("REVOLUTE") == 0 )		return REVOLUTE_JOINT;
	  else if ( CADJointType_string.compare("UNIVERSAL") == 0 )		return UNIVERSAL_JOINT;
	  else if ( CADJointType_string.compare("SPHERICAL") == 0 )		return SPHERICAL_JOINT;
	  else if ( CADJointType_string.compare("PRISMATIC") == 0 )		return PRISMATIC_JOINT;
	  else if ( CADJointType_string.compare("CYLINDRICAL") == 0 )	return CYLINDRICAL_JOINT;
	  else if ( CADJointType_string.compare("PLANAR") == 0 )		return PLANAR_JOINT;	  
	  else if ( CADJointType_string.compare("FREE") == 0 )			return FREE_JOINT;
	  else if ( CADJointType_string.compare("UNKNOWN") == 0 )		return UNKNOWN_JOINT_TYPE;
	  
	  std::string temp_string = "Function CADJointType_enum was passed '" + CADJointType_string + "' which is an erroneous type. Allowed types are: FIXED, REVOLUTE, UNIVERSAL, SPHERICAL, PRISMATIC, CYLINDRICAL, FREE, and UNKNOWN.";
	  throw isis::application_exception(temp_string.c_str());
  }

	std::string CADJointType_string(  e_CADJointType in_CADJointType_enum)
											throw (isis::application_exception)
	{
		switch (in_CADJointType_enum )
		{
			case FIXED_JOINT:
				return "FIXED";
				break;
			case REVOLUTE_JOINT:
				return "REVOLUTE";
				break;
			case UNIVERSAL_JOINT:
				return "UNIVERSAL";
				break;
			case SPHERICAL_JOINT:
				return "SPHERICAL";
				break;
			case PRISMATIC_JOINT:
				return "PRISMATIC";
				break;
			case CYLINDRICAL_JOINT:
				return "CYLINDRICAL";
				break;
			case PLANAR_JOINT:
				return "PLANAR";
				break;
			case FREE_JOINT:
				return "FREE";
				break;
			case UNKNOWN_JOINT_TYPE:
				return "UNKNOWN";
				break;
			default:
				char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
				string temp_string = "Function CADJointType_string was passed " + 
					std::string(itoa(in_CADJointType_enum, temp_char_array, 10)) + 
					" which is an erroneous type.  Allowed enum values are FIXED_JOINT, REVOLUTE_JOINT, UNIVERSAL_JOINT, SPHERICAL_JOINT, PRISMATIC_JOINT, CYLINDRICAL_JOINT, FREE_JOINT, and UNKNOWN_JOINT_TYPE.";
				throw isis::application_exception(temp_string.c_str());
	  }
	}

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
					std::string(itoa(in_ProAsmcompSetType_enum, temp_char_array, 10)) + 
					" which is an erroneous type. Allowed types are PRO_ASM_SET_TYPE_FIXED, PRO_ASM_SET_TYPE_PIN, PRO_ASM_SET_TYPE_SLIDER, PRO_ASM_SET_TYPE_CYLINDRICAL, PRO_ASM_SET_TYPE_PLANAR, PRO_ASM_SET_TYPE_BALL, PRO_ASM_SET_TYPE_WELD, PRO_ASM_SET_TYPE_BEARING, PRO_ASM_SET_TYPE_GENERAL, PRO_ASM_SET_TYPE_6DOF, PRO_ASM_SET_TYPE_SLOT, PRO_ASM_SET_TYPE_GIMBAL, PRO_ASM_SET_TYPE_DMA_DEFINED, and PRO_ASM_SET_USER_DEFINED_TYPE.";
				throw isis::application_exception(temp_string.c_str());
	  }
	}


 e_ProcessingInstruction_Primary ProcessingInstruction_Primary_enum(const std::string &in_ProcessingInstruction_Primary_string)
												throw (isis::application_exception)
  {

	  std::string ProcessingInstruction_Primary_string = ConvertToUpperCase(in_ProcessingInstruction_Primary_string);

	  if      ( ProcessingInstruction_Primary_string.compare("COMPLETE_THE_HIERARCHY_FOR_LEAF_ASSEMBLIES") == 0 )			
												return COMPLETE_THE_HIERARCHY_FOR_LEAF_ASSEMBLIES;
	  else if ( ProcessingInstruction_Primary_string.compare("UNIQUELY_NAME_ALL_CAD_MODEL_INSTANCES") == 0 )		
												return UNIQUELY_NAME_ALL_CAD_MODEL_INSTANCES;
	  else if ( ProcessingInstruction_Primary_string.compare("OUTPUT_JOINT_INFORMATION") == 0 )		
												return OUTPUT_JOINT_INFORMATION;

	  
	  std::string temp_string = "Function ProcessingInstruction_Primary_enum was passed '" + ProcessingInstruction_Primary_string + "' which is an erroneous type. Allowed types are: COMPLETE_THE_HIERARCHY_FOR_LEAF_ASSEMBLIES, UNIQUELY_NAME_ALL_CAD_MODEL_INSTANCES, and OUTPUT_JOINT_INFORMATION.";
	  throw isis::application_exception(temp_string.c_str());
  }

	std::string ProcessingInstruction_Primary_string(  e_ProcessingInstruction_Primary in_ProcessingInstruction_Primary_enum)
											throw (isis::application_exception)
	{
		switch (in_ProcessingInstruction_Primary_enum )
		{
			case COMPLETE_THE_HIERARCHY_FOR_LEAF_ASSEMBLIES:
				return "COMPLETE_THE_HIERARCHY_FOR_LEAF_ASSEMBLIES";
				break;
			case UNIQUELY_NAME_ALL_CAD_MODEL_INSTANCES:
				return "UNIQUELY_NAME_ALL_CAD_MODEL_INSTANCES";
				break;
			case OUTPUT_JOINT_INFORMATION:
				return "OUTPUT_JOINT_INFORMATION";
				break;
			default:
				char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
				string temp_string = "Function ProcessingInstruction_Primary_string was passed " + 
					std::string(itoa(in_ProcessingInstruction_Primary_enum, temp_char_array, 10)) + 
					" which is an erroneous type.  Allowed enum values are COMPLETE_THE_HIERARCHY_FOR_LEAF_ASSEMBLIES, UNIQUELY_NAME_ALL_CAD_MODEL_INSTANCES, and OUTPUT_JOINT_INFORMATION.";
				throw isis::application_exception(temp_string.c_str());
	  }
	}

 e_ProcessingInstruction_Secondary ProcessingInstruction_Secondary_enum(const std::string &in_ProcessingInstruction_Secondary_string)
												throw (isis::application_exception)
  {
	  std::string ProcessingInstruction_Secondary_string = ConvertToUpperCase(in_ProcessingInstruction_Secondary_string);

	  if      ( ProcessingInstruction_Secondary_string.compare("PROCESSING_INSTRUCTION_SECONDARY_NONE") == 0 )			
												return PROCESSING_INSTRUCTION_SECONDARY_NONE;
	  else if ( ProcessingInstruction_Secondary_string.compare("NONE") == 0 )		
												return PROCESSING_INSTRUCTION_SECONDARY_NONE;
	  else if ( ProcessingInstruction_Secondary_string.compare("") == 0 )		
												return PROCESSING_INSTRUCTION_SECONDARY_NONE;
	  else if ( ProcessingInstruction_Secondary_string.compare("VALIDATE_JOINT_INFORMATION") == 0 )		
												return VALIDATE_JOINT_INFORMATION;
	  
	  std::string temp_string = "Function ProcessingInstruction_Secondary_enum was passed '" + ProcessingInstruction_Secondary_string + "' which is an erroneous type. Allowed types are: PROCESSING_INSTRUCTION_SECONDARY_NONE, NONE, empty tring, and VALIDATE_JOINT_INFORMATION.";
	  throw isis::application_exception(temp_string.c_str());
  }

	std::string ProcessingInstruction_Secondary_string(  e_ProcessingInstruction_Secondary in_ProcessingInstruction_Secondary_enum)
											throw (isis::application_exception)
	{
		switch (in_ProcessingInstruction_Secondary_enum )
		{
			case PROCESSING_INSTRUCTION_SECONDARY_NONE:
				return "NONE";
				break;
			case VALIDATE_JOINT_INFORMATION:
				return "VALIDATE_JOINT_INFORMATION";
				break;
			default:
				char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
				string temp_string = "Function ProcessingInstruction_Secondary_string was passed " + 
					std::string(itoa(in_ProcessingInstruction_Secondary_enum, temp_char_array, 10)) + 
					" which is an erroneous type.  Allowed enum values are PROCESSING_INSTRUCTION_SECONDARY_NONE and VALIDATE_JOINT_INFORMATION.";
				throw isis::application_exception(temp_string.c_str());
	  }
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

 e_Metrics_JointsComponentInstanceData_Primary Metrics_JointsComponentInstanceData_Primary_enum(const std::string &in_Metrics_JointsComponentInstanceData_Primary_string)
												throw (isis::application_exception)
  {

	  std::string Metrics_JointsComponentInstanceData_Primary_string = ConvertToUpperCase(in_Metrics_JointsComponentInstanceData_Primary_string);

	  if      ( Metrics_JointsComponentInstanceData_Primary_string.compare("HAS_KINEMATIC_JOINT") == 0 )			
												return METRICS_JOINTS_HAS_KINEMATIC_JOINT;
	  else if ( Metrics_JointsComponentInstanceData_Primary_string.compare("LEAF_ASSEMBLY_NO_KINEMATIC_JOINTS") == 0 )		
												return METRICS_JOINTS_LEAF_ASSEMBLY_NO_KINEMATIC_JOINTS;
	  else if ( Metrics_JointsComponentInstanceData_Primary_string.compare("ANCHOR_COMPONENT") == 0 )		
												return METRICS_JOINTS_ANCHOR_COMPONENT;

	  
	  std::string temp_string = "Function Metrics_JointsComponentInstanceData_Primary_enum was passed '" + Metrics_JointsComponentInstanceData_Primary_string + "' which is an erroneous type. Allowed types are: HAS_KINEMATIC_JOINT, LEAF_ASSEMBLY_NO_KINEMATIC_JOINTS, and  ANCHOR_COMPONENT.";
	  throw isis::application_exception(temp_string.c_str());
  }

	std::string Metrics_JointsComponentInstanceData_Primary_string(  e_Metrics_JointsComponentInstanceData_Primary in_Metrics_JointsComponentInstanceData_Primary_enum)
											throw (isis::application_exception)
	{
		switch (in_Metrics_JointsComponentInstanceData_Primary_enum )
		{
			case METRICS_JOINTS_HAS_KINEMATIC_JOINT:
				return "HAS_KINEMATIC_JOINT";
				break;

			case METRICS_JOINTS_LEAF_ASSEMBLY_NO_KINEMATIC_JOINTS:
				return "LEAF_ASSEMBLY_NO_KINEMATIC_JOINTS";
				break;

			case METRICS_JOINTS_ANCHOR_COMPONENT:
				return "ANCHOR_COMPONENT";
				break;
			default:
				char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
				string temp_string = "Function Metrics_JointsComponentInstanceData_Primary_string was passed " + 
					std::string(itoa(in_Metrics_JointsComponentInstanceData_Primary_enum, temp_char_array, 10)) + 
					" which is an erroneous type.  Allowed enum values are METRICS_JOINTS_HAS_KINEMATIC_JOINT, METRICS_JOINTS_LEAF_ASSEMBLY_NO_KINEMATIC_JOINTS, and METRICS_JOINTS_ANCHOR_COMPONENT.";
				throw isis::application_exception(temp_string.c_str());
	  }
	}

 e_Metrics_JointsComponentInstanceData_Secondary Metrics_JointsComponentInstanceData_Secondary_enum(const std::string &in_Metrics_JointsComponentInstanceData_Secondary_string)
												throw (isis::application_exception)
  {
	  std::string Metrics_JointsComponentInstanceData_Secondary_string = ConvertToUpperCase(in_Metrics_JointsComponentInstanceData_Secondary_string);

	  if      ( Metrics_JointsComponentInstanceData_Secondary_string.compare("METRICS_JOINTS_NONE") == 0 )			
												return METRICS_JOINTS_NONE;
	  else if ( Metrics_JointsComponentInstanceData_Secondary_string.compare("NONE") == 0 )		
												return METRICS_JOINTS_NONE;
	  else if ( Metrics_JointsComponentInstanceData_Secondary_string.compare("") == 0 )		
												return METRICS_JOINTS_NONE;
	  else if ( Metrics_JointsComponentInstanceData_Secondary_string.compare("TREAT_AS_ONE_BODY") == 0 )		
												return METRICS_JOINTS_TREAT_AS_ONE_BODY;

	  
	  std::string temp_string = "Function Metrics_JointsComponentInstanceData_Secondary_enum was passed '" + Metrics_JointsComponentInstanceData_Secondary_string + "' which is an erroneous type. Allowed types are: METRICS_JOINTS_NONE, TREAT_AS_ONE_BODY, NONE, and empty string.";
	  throw isis::application_exception(temp_string.c_str());
  }

	std::string Metrics_JointsComponentInstanceData_Secondary_string(  e_Metrics_JointsComponentInstanceData_Secondary in_Metrics_JointsComponentInstanceData_Secondary_enum)
											throw (isis::application_exception)
	{
		switch (in_Metrics_JointsComponentInstanceData_Secondary_enum )
		{
			case METRICS_JOINTS_NONE:
				return "NONE";
				break;
			case METRICS_JOINTS_TREAT_AS_ONE_BODY:
				return "TREAT_AS_ONE_BODY";
				break;
			default:
				char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
				string temp_string = "Function Metrics_JointsComponentInstanceData_Secondary_string was passed " + 
					std::string(itoa(in_Metrics_JointsComponentInstanceData_Secondary_enum, temp_char_array, 10)) + 
					" which is an erroneous type.  Allowed enum values are METRICS_JOINTS_NONE, and METRICS_JOINTS_TREAT_AS_ONE_BODY.";
				throw isis::application_exception(temp_string.c_str());
	  }
	}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

 e_Thermal_LoadConstraint Thermal_LoadConstraint_enum(const std::string &in_Thermal_LoadConstraint_string)
												throw (isis::application_exception)
  {
	  std::string Thermal_LoadConstraint_string = ConvertToUpperCase(in_Thermal_LoadConstraint_string);

	  if      ( Thermal_LoadConstraint_string.compare("CONVECTIONHEAT") == 0 )	// CONVECTIONHEAT is deprecated, use CONVECTION		
												return THERMAL_CONVECTION;
	  else if ( Thermal_LoadConstraint_string.compare("CONVECTION") == 0 )  
												return THERMAL_CONVECTION;
	  else if ( Thermal_LoadConstraint_string.compare("HEATFLUX") == 0 )		
												return THERMAL_HEAT_FLUX;
	  else if ( Thermal_LoadConstraint_string.compare("HEATGENERATION") == 0 )		
												return THERMAL_HEAT_GENERATION;
	  else if ( Thermal_LoadConstraint_string.compare("INITIALTEMPERATURE") == 0 )		
												return THERMAL_INITIAL_TEMPERATURE;
	  else if ( Thermal_LoadConstraint_string.compare("SPECIFIEDTEMPERATURE") == 0 )		
												return THERMAL_SPECIFIED_TEMPERATURE;
	  else if ( Thermal_LoadConstraint_string.compare("AMBIENTTEMPERATURE") == 0 )		
												return THERMAL_AMBIENT_TEMPERATURE;

	  std::string temp_string = "Function Thermal_LoadConstraint_enum was passed '" + Thermal_LoadConstraint_string + "' which is an erroneous type. Allowed types are: ConvectionHeat, HeatFlux, HeatGeneration, InitialTemperature, and SpecifiedTemperature.";
	  throw isis::application_exception(temp_string.c_str());
  }

	std::string Thermal_LoadConstraint_string(  e_Thermal_LoadConstraint in_Thermal_LoadConstraint_enum)
											throw (isis::application_exception)
	{
		switch (in_Thermal_LoadConstraint_enum )
		{
			case THERMAL_CONVECTION_HEAT:  // THERMAL_CONVECTION_HEAT is deprecated, use THERMAL_CONVECTION
				return "Convection";
				break;
			case THERMAL_CONVECTION:
				return "Convection";
				break;
			case THERMAL_HEAT_FLUX:
				return "HeatFlux";
				break;
			case THERMAL_HEAT_GENERATION:
				return "HeatGeneration";
				break;
			case THERMAL_INITIAL_TEMPERATURE:
				return "InitialTemperature";
				break;
			case THERMAL_SPECIFIED_TEMPERATURE:
				return "SpecifiedTemperature";
				break;
			case THERMAL_AMBIENT_TEMPERATURE:
				return "AmbientTemperature";
				break;
			default:
				char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
				string temp_string = "Function Thermal_LoadConstraint_string was passed " + 
					std::string(itoa(in_Thermal_LoadConstraint_enum, temp_char_array, 10)) + 
					" which is an erroneous type.  Allowed enum values are THERMAL_CONVECTION, THERMAL_HEAT_FLUX, THERMAL_HEAT_GENERATION, THERMAL_INITIAL_TEMPERATURE, HERMAL_SPECIFIED_TEMPERATURE, and THERMAL_AMBIENT_TEMPERATURE.";
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