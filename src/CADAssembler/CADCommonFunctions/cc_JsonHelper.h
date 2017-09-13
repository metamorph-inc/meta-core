#ifndef JSONHELPER_H
#define JSONHELPER_H

#include "isis_application_exception.h"
#include <string>
#include <list>
#include <map>
#include <set>
#include <boost\filesystem.hpp>

#pragma warning( disable : 4290) 

namespace isis_CADCommon
{
	/* Parses a json file specified by path. Populates the "STEPModel" tag based on "id" using key and value of the input stepFiles parameter.
	* @param	in_ManifestJson_PathAndFileName path/name to the json file (e.g. .\manufacturing.manifest.json) 
	* @param	in_ComponentInstanceId_to_StepFile_map map of component instance ID to step file name
	* @param	out_ComponentInstanceIds_AddedToManifest component instance IDs that were updated with the STEP file name in the manifest.
	*/
	void AddStepFileMappingToManufacturingManifest(
									const std::string &in_ManifestJson_PathAndFileName,
									const std::map<std::string, std::string> &in_ComponentInstanceId_to_StepFile_map,
									std::set<std::string> &out_ComponentInstanceIds_AddedToManifest ) 
															throw (isis::application_exception);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*  hydrostatics.json example
			{
				"NumCount": 1,
				"Density": ,
				   "DensityUnits": "kg/mm^3",
				"CG_X": ,
				"CG_Y": ,
				"CG_Z": ,
				   "CG_Units": "mm",
				"DisplacedVolume": ,
				   "DisplacedVolumeUnits": "mm^3",
				"HydrostaticVolume": ,
				   "HydrostaticVolumeUnits": "mm^3",
				"Data": [
					{
						"Description": "1", 
						"Roll": 0, 
						"Pitch": 0,
						"Yaw": 0,
						"RollPitchYawUnits": "degrees",					       
						"WaterLine": "",
						   "WaterLineUnits": "mm",
						"ReferenceArea": "",
						   "ReferenceAreaUnits": "mm^2",
						"RightingMomentArm": "",
						   "RightingMomentArmUnits": "mm",
						"CB_X": ,
						"CB_Y": ,
						"CB_Z": ,
						   "CB_Units": "mm",
						"XSections": [
							{
								"Offset": 0.0,
								"Area": 0.0,
							}
							{
								"Offset": 0.0,
								"Area": 0.0,
							}
							{
								"Offset": 0.0,
								"Area": 0.0,
							}
						]
					}
				], 
				"TestBench": "Hydrostatics Testbench"
			}
	*/

	//	See json file example above.
	//	UpdateHydrostaticsJsonFile assumes that there is only one entry under "Data".  If there are more than one entry, all entries will 
	//	be filled in with identical data.
	//
	//	in_RightingMomentArm, in_CB_x, in_CB_y, and in_CB_z are not currently written to the json file.  This functio will be updated later
	//	to write those entries.

	void UpdateHydrostaticsJsonFile( const boost::filesystem::path	in_HydrostaticsFile_PathAndFileName,
									double				in_FluidDensity,
									double				in_WaterLine_Height_zAxis,
									double				in_ReferenceArea,  // Wetted surface area
									double				in_DisplacedVolume,
									double				in_HydrostaticVolume,
									double				in_RightingMomentArm, 
									double				in_CG_x, // Center of Gravity
									double				in_CG_y,
									double				in_CG_z,
									double				in_CB_x,  // Center of Bouncy
									double				in_CB_y,
									double				in_CB_z,
									std::vector< std::pair<double,double> > in_XSect)
														throw (isis::application_exception);

}

#endif