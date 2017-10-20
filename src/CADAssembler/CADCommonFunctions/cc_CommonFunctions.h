#ifndef CC_COMMON_FUNCTIONS_H
#define CC_COMMON_FUNCTIONS_H
#include <string>
#include <list>
#include <map>
#include "cc_CommonStructures.h"

namespace isis
{

	void CreateXMLFile_RequestedMetrics( 
						const	std::string									&in_PathAndFileName,
						const	std::list<TopLevelAssemblyData>				&in_TopLevelAssemblies,
						std::map<std::string, isis::CADComponentData>		&in_CADComponentData_map )
																	throw (isis::application_exception);


}

#endif