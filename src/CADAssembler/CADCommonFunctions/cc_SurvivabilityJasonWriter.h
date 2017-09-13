#ifndef SURVIVABILITY_JASON_WRITER_H
#define SURVIVABILITY_JASON_WRITER_H
#include "isis_application_exception.h"
#include "cc_GraphicsFunctions.h"
#include <vector>
#include <string>
namespace isis_CADCommon
{

	
	struct CADCommon_Shotline
	{
		std::string					name;
		std::string					threatRef; 
		isis_CADCommon::Point_3D	startPoint;
		isis_CADCommon::Point_3D	targetPoint;

		CADCommon_Shotline();
		
		CADCommon_Shotline( const std::string &in_Name,
				  const std::string &in_ThreatRef,
				  const isis_CADCommon::Point_3D &in_StartPoint,
				  const isis_CADCommon::Point_3D &in_TargetPoint );


	};
	
	enum e_CADCommon_ReferencePlane
    {
        CADCommon_GROUND,  
		CADCommon_WATERLINE, 
    };

	struct  CADCommon_ReferencePlane
	{
		e_CADCommon_ReferencePlane type;
		std::vector<isis_CADCommon::Point_3D> points;
	};

	// Normally there would be only one reference plane; however this function supports writing
	// multiple reference planes.
	// if in_ReferencePlanes and in_Shotlines are empty, then this program would not change the
	// json file except for deleting the referencePlanes and shotlines from the json file.
	// Deleting the referencePlanes and shotlines assures that the json file would not be used
	// with obsolete data.
	void Write_ReferencePlane_and_ShotLines_to_Json( const std::string &in_BallisticConfig_PathAndFileName,
										  std::vector<CADCommon_ReferencePlane> in_ReferencePlanes,
										  const std::vector<CADCommon_Shotline> in_Shotlines,
										  bool isBallistic = true /*07-29-2013: DY added to distinguish between ballistic and blast configs*/) 
															 throw (isis::application_exception);



}  // END namespace isis

#endif