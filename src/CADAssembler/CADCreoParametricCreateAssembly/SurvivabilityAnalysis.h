#ifndef SURVIVABILITY_ANALYSIS_H
#define SURVIVABILITY_ANALYSIS_H

#include <cc_GraphicsFunctions.h>

namespace isis
{



	// If at lease one of the assemblies in in_CADAssemblies contains analysesBallistic, then return true.    
	bool IsABallisticAnalysisRun( const CADAssemblies &in_CADAssemblies );
	// If at lease one of the assemblies in in_CADAssemblies contains analysesBlast, then return true.    
	bool IsABlastAnlysisRun( const CADAssemblies &in_CADAssemblies );

	void PopulateBallisticFiles( 
					const TopLevelAssemblyData							&in_TopLevelAssemblyData,
					const std::string									&in_WORKING_DIR,
					std::map<std::string, isis::CADComponentData>		&in_CADComponentData_map )
																	throw (isis::application_exception);

	void PopulateBlastFiles( 
					const TopLevelAssemblyData							&in_TopLevelAssemblyData,
					const std::string									&in_WORKING_DIR,
					std::map<std::string, isis::CADComponentData>		&in_CADComponentData_map )
																	throw (isis::application_exception);

	// This function assumes:
	//	1. The assembly is of a vehicle
	//	2. The coordinate system of the vehicle is as follows:
	//		z axis pointing in the direction of backward motion of the vehicle
	//		y axis pointing upward
	//		x axis in accordance to the right-hand rule
	//	3.	Pt_0  x, y, z values
	//		Pt_1  x, y, z values
	//		Pt_2  x, y, z values
	//		Where
    //           Vector ( Pt_0 to Pt_1 )   X  Vector ( Pt_0 to Pt_2 )   would define the upward direction for a vehicle.
    //           X represents the cross product
	//	4.  For tracked vehicles, the tracks are parallel to the z-axis
	//  5.  For wheeled vehicles, the portion of the wheels touching the ground form a 
	//		plane. 
	void ComputeVehicleGroundPlane( const std::string								&in_AssemblyComponentID,
									std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map,
									std::vector<isis_CADCommon::Point_3D>			&out_GroundPlanePoints )
																			throw (isis::application_exception);




} // END namespace isis

#endif