#ifndef MODEL_MATERIALS_H
#define MODEL_MATERIALS_H
#include <string>
#include <map>
#include <cc_CommonStructures.h>
#include <isis_include_ptc_headers.h>

namespace isis
{

	void CreateUniquelyNamedMaterials( 
					const std::string									&in_AssemblyComponentID, 
					std::map<std::string, isis::CADComponentData>			&in_CADComponentData_map )
																	throw (isis::application_exception);

	void RetrieveMaterial(	const std::string &in_ModelName, 
						ProSolid part, 
						std::string &out_MaterialName ) throw(isis::application_exception);

	void RetrieveCreoMaterialsNames( const std::vector<std::string>					&listOfComponentIDs,
									 std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map,
									 std::map<std::string, std::string>				&out_ComponentID_to_MaterialName_map ) 
																						throw (isis::application_exception);

} // END namespace isis


#endif