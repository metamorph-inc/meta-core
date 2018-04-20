#include "stdafx.h"
#include <SurvivabilityAnalysis.h>
#include "cc_SurvivabilityJasonWriter.h"
#include <cc_AssemblyUtilities.h>
#include <CommonFunctions.h>
#define _USE_MATH_DEFINES
#include <math.h>

namespace isis
{
////////////////////////////////////////////////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// For azimuth, north is in the direction of forward movement of the vehicle.  Since the vehicle coordinate 
	// systems has the z axis pointing from the front of the vehicle to the back of the vehicle and the Y axis 
	// pointing upward; the azimuth angle would be from the negative z axis clockwise around the y axis, where 
	// clockwise would be looking from a positive y coordinate onto the x-y plane. 
	void ComputeShotLine(	//cad::CadFactoryAbstract							&in_Factory,
							const Shotline									&in_Shotline, 
							const std::string								&in_AssemblyComponentID,
							std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map,
							isis_CADCommon::CADCommon_Shotline				&out_CADCommon_Shotline )
	{

		isis::cad::CadFactoryAbstract_global *cadFactoryAbstract_global_ptr = isis::cad::CadFactoryAbstract_global::instance();
		isis::cad::CadFactoryAbstract::ptr	cAD_Factory_ptr = cadFactoryAbstract_global_ptr->getCadFactoryAbstract_ptr();

		isis::cad::IModelOperations&         modelOperations = cAD_Factory_ptr->getModelOperations();	

		double azimuth_radians;
		double elevation_radians;

		azimuth_radians = ((in_Shotline.azimuth - 90.0) / 360.0) * M_PI;
		elevation_radians = (in_Shotline.elevation / 360.0 ) * M_PI;

		CADPoint targetPoint;

	    //RetrieveDatumPointCoordinates(	//in_Factory,
		//								in_AssemblyComponentID,
		//								in_Shotline.datumPoint_ComponentID,
		//								in_CADComponentData_map,
		//								in_Shotline.datumPoint,
		//								targetPoint);

	    modelOperations.retrievePointCoordinates(
										in_AssemblyComponentID,
										in_Shotline.datumPoint_ComponentID,
										in_CADComponentData_map,
										in_Shotline.datumPoint,
										targetPoint);



		out_CADCommon_Shotline.targetPoint.x = targetPoint.x / 1000.0;  // Convert to meters
		out_CADCommon_Shotline.targetPoint.y = targetPoint.y / 1000.0;
		out_CADCommon_Shotline.targetPoint.z = targetPoint.z / 1000.0;

		out_CADCommon_Shotline.startPoint.x = cos(azimuth_radians)   * 30.0;  // extend out 30 meters
		out_CADCommon_Shotline.startPoint.z = sin(azimuth_radians)   * 30.0;
		out_CADCommon_Shotline.startPoint.y = cos(elevation_radians) * 30.0;

		out_CADCommon_Shotline.name = in_Shotline.name;
		out_CADCommon_Shotline.threatRef = in_Shotline.ballisticThreatRef;

	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void PopulateBallisticFiles( 
					//cad::CadFactoryAbstract								&in_Factory,
					const TopLevelAssemblyData							&in_TopLevelAssemblyData,
					const std::string									&in_WORKING_DIR,
					std::map<std::string, isis::CADComponentData>		&in_CADComponentData_map )
																	throw (isis::application_exception)
	{


		//isis::cad::CadFactoryAbstract_global *cadFactoryAbstract_global_ptr = isis::cad::CadFactoryAbstract_global::instance();
		//isis::cad::CadFactoryAbstract::ptr	                cAD_Factory_ptr = cadFactoryAbstract_global_ptr->getCadFactoryAbstract_ptr();
		//isis::cad::IModelOperations& modelOperations = cAD_Factory_ptr->getModelOperations();

		// Must find the bounding box and the shotlines
		std::vector<isis_CADCommon::Point_3D>  groundPlanePoints;

		ComputeVehicleGroundPlane(	in_TopLevelAssemblyData.assemblyComponentID,
									in_CADComponentData_map,
									groundPlanePoints);

		//modelOperations.computeVehicleGroundPlane ( in_TopLevelAssemblyData.assemblyComponentID,
		//											in_CADComponentData_map,
		//											groundPlanePoints);


		// Must use interators here.  A for each loop will not actually change the values
		for ( std::vector<isis_CADCommon::Point_3D>::iterator i = groundPlanePoints.begin(); 
								i != groundPlanePoints.end(); ++i )
		{
			i->x = i->x / 1000.0;
			i->y = i->y / 1000.0;
			i->z = i->z / 1000.0;
		}

		std::vector<isis_CADCommon::CADCommon_ReferencePlane> referencePlanes;
		std::vector<isis_CADCommon::CADCommon_Shotline> shotlines;

		for each ( const AnalysisBallistic i in in_TopLevelAssemblyData.analysesCAD.analysesBallistic )
		{
			for each ( const e_ReferencePlane j in i.referencePlanes )
			{
				//if ( j == e_ReferencePlane::GROUND ) 
				if ( j ==  isis::PLANE_GROUND ) 
				{
					isis_CADCommon::CADCommon_ReferencePlane  gp;
					gp.type = isis_CADCommon::CADCommon_GROUND;
					for each ( const isis_CADCommon::Point_3D pt in groundPlanePoints)
					{
						gp.points.push_back( pt );
					}
					referencePlanes.push_back(gp);
				}
				// WATERLINE not yet supported if ( j = e_ReferencePlane::WATERLINE ) referencePlanes.push_back( isis_CADCommon::CADCommon_WATERLINE)
			}

			for each ( const Shotline j in i.shotlines)
			{
				isis_CADCommon::CADCommon_Shotline CADCommon_Shotline;
				ComputeShotLine(	   // in_Factory,
									j, 
									in_TopLevelAssemblyData.assemblyComponentID,
									in_CADComponentData_map,
									CADCommon_Shotline );
				shotlines.push_back(CADCommon_Shotline);
			}
		}

		//std::cout << std::endl << "Ground Plane Points:" << std::endl;
		//for each ( isis_CADCommon::Point_3D i in groundPlanePoints ) std::cout << i << "  ";

		std::string ballisticConfig_PathAndFileName = in_WORKING_DIR + ".\\BallisticConfig.json";

		isis_CADCommon::Write_ReferencePlane_and_ShotLines_to_Json( 
													ballisticConfig_PathAndFileName,
													referencePlanes,
													shotlines ); 
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////
	void PopulateBlastFiles( 
					//cad::CadFactoryAbstract								&in_Factory,
					const TopLevelAssemblyData							&in_TopLevelAssemblyData,
					const std::string									&in_WORKING_DIR,
					std::map<std::string, isis::CADComponentData>		&in_CADComponentData_map )
																	throw (isis::application_exception)
	{


		//isis::cad::CadFactoryAbstract_global *cadFactoryAbstract_global_ptr = isis::cad::CadFactoryAbstract_global::instance();
		//isis::cad::CadFactoryAbstract::ptr	                cAD_Factory_ptr = cadFactoryAbstract_global_ptr->getCadFactoryAbstract_ptr();
		//isis::cad::IModelOperations& modelOperations = cAD_Factory_ptr->getModelOperations();

		// Must find the bounding box and the shotlines
		std::vector<isis_CADCommon::Point_3D>  groundPlanePoints;

		ComputeVehicleGroundPlane(	in_TopLevelAssemblyData.assemblyComponentID,
									in_CADComponentData_map,
									groundPlanePoints);

		//modelOperations.computeVehicleGroundPlane(	in_TopLevelAssemblyData.assemblyComponentID,
		//											in_CADComponentData_map,
		//											groundPlanePoints);

	
		// Must use interators here.  A for each loop will not actually change the values
		for ( std::vector<isis_CADCommon::Point_3D>::iterator i = groundPlanePoints.begin(); 
								i != groundPlanePoints.end(); ++i )
		{
			i->x = i->x / 1000.0;
			i->y = i->y / 1000.0;
			i->z = i->z / 1000.0;
		}

		std::vector<isis_CADCommon::CADCommon_ReferencePlane> referencePlanes;
		std::vector<isis_CADCommon::CADCommon_Shotline> shotlines;

		for each ( const AnalysisBlast i in in_TopLevelAssemblyData.analysesCAD.analysesBlast )
		{
			for each ( const e_ReferencePlane j in i.referencePlanes )
			{
				//if ( j == e_ReferencePlane::GROUND ) 
				if ( j ==  isis::PLANE_GROUND ) 
				{
					isis_CADCommon::CADCommon_ReferencePlane  gp;
					gp.type = isis_CADCommon::CADCommon_GROUND;
					for each ( const isis_CADCommon::Point_3D pt in groundPlanePoints)
					{
						gp.points.push_back( pt );
					}
					referencePlanes.push_back(gp);
				}
				// WATERLINE not yet supported if ( j = e_ReferencePlane::WATERLINE ) referencePlanes.push_back( isis_CADCommon::CADCommon_WATERLINE)
			}

			// Shotlines not currently used for Blast
			//for each ( const Shotline j in i.shotlines)
			//{
			//	isis_CADCommon::CADCommon_Shotline CADCommon_Shotline;
			//	ComputeShotLine(	j, 
			//						in_TopLevelAssemblyData.assemblyComponentID,
			//						in_CADComponentData_map,
			//						CADCommon_Shotline );
			//	shotlines.push_back(CADCommon_Shotline);
			//}
		}

		//std::cout << std::endl << "Ground Plane Points:" << std::endl;
		//for each ( isis_CADCommon::Point_3D i in groundPlanePoints ) std::cout << i << "  ";

		std::string blastConfig_PathAndFileName = in_WORKING_DIR + ".\\BlastConfig.json";

		isis_CADCommon::Write_ReferencePlane_and_ShotLines_to_Json( 
													blastConfig_PathAndFileName,
													referencePlanes,
													shotlines,
													false		/*07-29-2013: DY added for blast config generation*/); 
	}

} // END namespace isis