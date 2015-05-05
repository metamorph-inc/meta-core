#ifndef CAD_COMMON_ENUMS_H
#define	CAD_COMMON_ENUMS_H

namespace isis_CADCommon
{
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

}

#endif