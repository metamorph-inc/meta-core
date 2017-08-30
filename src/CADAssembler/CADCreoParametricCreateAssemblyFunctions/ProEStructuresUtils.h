#ifndef PRO_E_STRUCTURES_UTILS_H
#define PRO_E_STRUCTURES_UTILS_H

#include <isis_application_exception.h>
#include <cc_CommonStructures.h>
#include <map>

namespace isis
{

	void Add_dependsOn ( std::map<std::string, CADComponentData> &in_out_CADComponentData_map );
														

} // END namespace isis

#endif