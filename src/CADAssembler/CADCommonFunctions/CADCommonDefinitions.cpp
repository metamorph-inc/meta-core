#include "CADCommonDefinitions.h"
#include <boost/algorithm/string/case_conv.hpp>



namespace isis_CADCommon
{

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	e_CADAssemblyConstraintType CADAssemblyConstraintType_enum( const std::string &in_CADAssemblyConstraintType_string)
																								throw (isis::application_exception)
	{
		std::string target_string = in_CADAssemblyConstraintType_string;
		boost::algorithm::to_upper(target_string);

		if ( target_string.compare("CAD_ASM_MATE") == 0 )
			return CAD_ASM_MATE;
		else if ( target_string.compare("CAD_ASM_MATE_OFFSET") == 0 )  
			return CAD_ASM_MATE_OFFSET;																	
		else if ( target_string.compare("CAD_ASM_ALIGN") == 0 )
			return CAD_ASM_ALIGN;
		else if ( target_string.compare("CAD_ASM_ALIGN_OFFSET") == 0 )
			return CAD_ASM_ALIGN_OFFSET;

		std::stringstream errorString;
		errorString << "Function - " << __FUNCTION__ << ", was passed: " + in_CADAssemblyConstraintType_string  << 
			           ", which is an erroneous type. Allowed types are: " << 
					   "CAD_ASM_MATE, CAD_ASM_MATE_OFFSET, CAD_ASM_ALIGN, or CAD_ASM_ALIGN_OFFSET.";

		throw isis::application_exception(errorString);		

	}


	std::string CADAssemblyConstraintType_string(  e_CADAssemblyConstraintType in_CADAssemblyConstraintType_enum)
																								throw (isis::application_exception)
	{
		switch (in_CADAssemblyConstraintType_enum )
		{
			case CAD_ASM_MATE:
				return "CAD_ASM_MATE";
				break;
			case CAD_ASM_MATE_OFFSET:
				return "CAD_ASM_MATE_OFFSET";
				break;
			case CAD_ASM_ALIGN:
				return "CAD_ASM_ALIGN";
				break;
			case CAD_ASM_ALIGN_OFFSET:
				return "CAD_ASM_ALIGN_OFFSET";
				break;

			default:
				std::stringstream errorString;
				errorString << "Function - " << __FUNCTION__ << ", was passed: " << in_CADAssemblyConstraintType_enum  << 
							   ", which is an erroneous type. Allowed types are: " <<
							   "CAD_ASM_MATE, CAD_ASM_MATE_OFFSET, CAD_ASM_ALIGN, or CAD_ASM_ALIGN_OFFSET.";
				throw isis::application_exception(errorString);		
		}
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}