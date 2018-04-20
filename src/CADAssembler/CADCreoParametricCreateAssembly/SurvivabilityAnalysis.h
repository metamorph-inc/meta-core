#ifndef SURVIVABILITY_ANALYSIS_H
#define SURVIVABILITY_ANALYSIS_H

#include <cc_GraphicsFunctions.h>

namespace isis
{




	void PopulateBallisticFiles( 
					//cad::CadFactoryAbstract								&in_Factory,
					const TopLevelAssemblyData							&in_TopLevelAssemblyData,
					const std::string									&in_WORKING_DIR,
					std::map<std::string, isis::CADComponentData>		&in_CADComponentData_map )
																	throw (isis::application_exception);

	void PopulateBlastFiles( 
					//cad::CadFactoryAbstract								&in_Factory,
					const TopLevelAssemblyData							&in_TopLevelAssemblyData,
					const std::string									&in_WORKING_DIR,
					std::map<std::string, isis::CADComponentData>		&in_CADComponentData_map )
																	throw (isis::application_exception);



} // END namespace isis

#endif