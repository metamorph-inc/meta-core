#ifndef CAD_COMMON_DEFINITIONS_H
#define	CAD_COMMON_DEFINITIONS_H
#include "isis_application_exception.h"

#pragma warning( disable : 4290 )  // Turn off the warning "C++ exception specification ignored except to indicate a function is not __declspec(nothrow)"


namespace isis_CADCommon
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	enum e_CylinderGeometryInclusionSpecifier
	{
		GEOMETRY_INCLUDE_END_CAP,   
		GEOMETRY_EXCLUDE_END_CAP, 
	};

	enum e_GeneralGeometryInclusionSpecifier
	{
		GEOMETRY_INTERIOR_AND_BOUNDARY,   
		GEOMETRY_BOUNDARY_ONLY, 
		GEOMETRY_INTERIOR_ONLY,
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

#endif