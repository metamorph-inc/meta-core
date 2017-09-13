#include "cc_SurvivabilityJasonWriter.h"
#include <boost/filesystem.hpp>
#include "json_spirit_reader_template.h"
#include "json_spirit_writer_template.h"
#include <cassert>
#include <fstream>

using namespace std;
using namespace json_spirit;
namespace isis_CADCommon
{

	CADCommon_Shotline::CADCommon_Shotline(){};
	CADCommon_Shotline::CADCommon_Shotline( const std::string &in_Name,
						const std::string &in_ThreatRef,
						const isis_CADCommon::Point_3D &in_StartPoint,
						const isis_CADCommon::Point_3D &in_TargetPoint ):
								name(in_Name), 
								threatRef(in_ThreatRef), 
								startPoint(in_StartPoint), 
								targetPoint(in_TargetPoint) {};

	//////////////////////////////////////////////////////////////////////////////////////////////
	struct Point
	{
		double x;
		double y;
		double z;
		Point()
			:x(0),
				y(0),
				z(0)
		{ }

		Point(double tx, double ty, double tz) 
			: x(tx),
				y(ty),
				z(tz)
		{ }

		void ConvertToJson(json_spirit::Object &pt)
		{
			pt.push_back(json_spirit::Pair(std::string("x_meters"), x));
			pt.push_back(json_spirit::Pair(std::string("y_meters"), y));
			pt.push_back(json_spirit::Pair(std::string("z_meters"), z));
		}
	};

	struct Ballistic_ReferencePlane
	{
		std::vector<Point> points;

		void ConvertToJson(json_spirit::Array &pt)
		{
			for (unsigned int i = 0; i < points.size(); i++)
			{
				json_spirit::Object pointObj;
				points[i].ConvertToJson(pointObj);
				pt.push_back(pointObj);
			}
		}
	};

	// 07-29-2013: DY added blast config reference plane	
	struct Blast_ReferencePlane
	{
		std::vector<Point> points;
		std::string type;

		void ConvertToJson(json_spirit::Object &refpln)
		{
			refpln.push_back(json_spirit::Pair("type", type));
			json_spirit::Array ptArray;
			for (unsigned int i = 0; i < points.size(); i++)
			{
				json_spirit::Object pointObj;
				points[i].ConvertToJson(pointObj);
				ptArray.push_back(pointObj);
			}
			refpln.push_back(json_spirit::Pair("points", ptArray));
		}
	};
	// 07-29-2013 end

	struct Blast_ReferencePlanes
	{
		std::vector<Blast_ReferencePlane> planes;
		
		void ConvertToJson(json_spirit::Array &pt)
		{
			for (unsigned int i = 0; i < planes.size(); i++)
			{
				json_spirit::Object planeObj;
				planes[i].ConvertToJson(planeObj);
				pt.push_back(planeObj);
			}
		}
	};

	struct CADCommon_Shotline_local
	{
		std::string name;
		std::string threatRef;
		Point targetPoint;
		Point startPoint;

		CADCommon_Shotline_local () {}

		void ConvertToJson(json_spirit::Object &pt)
		{
			pt.push_back(json_spirit::Pair("name", name));
			pt.push_back(json_spirit::Pair("threatRef", threatRef));

			json_spirit::Object startpt, targetpt;
			targetPoint.ConvertToJson(targetpt);
			startPoint.ConvertToJson(startpt);
			pt.push_back(json_spirit::Pair("startPoint", startpt));
			pt.push_back(json_spirit::Pair("targetPoint", targetpt));
		}
	};

	struct ShotlineSuite
	{
		std::vector<CADCommon_Shotline_local> shotlines;

		void ConvertToJson(json_spirit::Array &pt)
		{
			for (unsigned int i = 0; i < shotlines.size(); i++)
			{
				json_spirit::Object shotlineObj;
				shotlines[i].ConvertToJson(shotlineObj);
				pt.push_back(shotlineObj);
			}
		}
	};

	//////////////////////////////////////////////////////////////////////////////////////////////
	// 07-29-2013: DY added this function because in BlastConfig, the tag is referencePlanes instead of referencePlane
	bool IsReferencePlanes(json_spirit::Pair pair)
	{
			if (pair.name_ == "referencePlanes")
					return true;
			else
					return false;
	}
	// 07-29-2013 end
	
	bool IsReferencePlane(json_spirit::Pair pair)
	{
			if (pair.name_ == "referencePlane")
					return true;
			else
					return false;
	}

	bool IsShotLines(json_spirit::Pair pair)
	{
			if (pair.name_ == "shotLines")
					return true;
			else
					return false;
	}
	void DeleteExisting_ReferencPlanes_ShotLines(json_spirit::Object &in_out_ContentObj )
	{
		
		bool stillSeaching = true;

		// Delete Shotlines
		while ( stillSeaching )
		{
			json_spirit::Object::iterator i = std::find_if(in_out_ContentObj.begin(), in_out_ContentObj.end(), IsShotLines);
			if (i == in_out_ContentObj.end())
				stillSeaching = false;
			else			
				in_out_ContentObj.erase(i); 
		}

		stillSeaching = true;
		// Delete Reference Planes
		while ( stillSeaching )
		{
            json_spirit::Object::iterator j = std::find_if(in_out_ContentObj.begin(), in_out_ContentObj.end(), IsReferencePlane);
            if (j == in_out_ContentObj.end())
				stillSeaching = false;
			else                   
				in_out_ContentObj.erase(j);  
		}

		// 07-29-2013: DY added so referencePlanes in BlastConfig can be deleted
		stillSeaching = true;
		// Delete Reference Planes for BlastConfig
		while ( stillSeaching )
		{
            json_spirit::Object::iterator j = std::find_if(in_out_ContentObj.begin(), in_out_ContentObj.end(), IsReferencePlanes);
            if (j == in_out_ContentObj.end())
				stillSeaching = false;
			else                   
				in_out_ContentObj.erase(j);  
		}
		// 07-29-2013 end
	}
	//////////////////////////////////////////////////////////////////////////////////////////////
	void Write_ReferencePlane_and_ShotLines_to_Json( 
										const std::string &in_BallisticConfig_PathAndFileName,
										std::vector<CADCommon_ReferencePlane> in_ReferencePlanes,
										const std::vector<CADCommon_Shotline> in_Shotlines,
										bool isBallistic) 
															 throw (isis::application_exception)
	{
		// http://www.codeproject.com/Articles/20027/JSON-Spirit-A-C-JSON-Parser-Generator-Implemented
		
		try
		{
			boost::filesystem::path p(in_BallisticConfig_PathAndFileName);
			if (boost::filesystem::exists(p))
			{
				std::ifstream jsonfile(p.make_preferred().string());		

				if (jsonfile.is_open())
				{
					json_spirit::Value content;
					json_spirit::read_stream_or_throw(jsonfile, content);
					json_spirit::Object contentObj = content.get_obj();

					DeleteExisting_ReferencPlanes_ShotLines(contentObj);

					// 07-29-2013: DY added b/c referencePlane in blast and ballistic configs differ
					if (isBallistic)
					{
						for each( CADCommon_ReferencePlane i in in_ReferencePlanes)
						{
							Ballistic_ReferencePlane refPlane;
							for each( const isis_CADCommon::Point_3D j in i.points )
							{							
								refPlane.points.push_back( Point(j.x, j.y, j.z));									
							}
							json_spirit::Array refJsonArray;
							refPlane.ConvertToJson(refJsonArray);
							contentObj.push_back(json_spirit::Pair("referencePlane", refJsonArray));	
						}

						if ( in_Shotlines.size() > 0 )
						{
							ShotlineSuite shotlinesuite;

							for each( const CADCommon_Shotline i in in_Shotlines )
							{
								CADCommon_Shotline_local shotln;

								shotln.name = i.name;
								shotln.threatRef = i.threatRef;
				 
								shotln.startPoint.x = i.startPoint.x;
								shotln.startPoint.y = i.startPoint.y;
								shotln.startPoint.z = i.startPoint.z;

								shotln.targetPoint.x = i.targetPoint.x;
								shotln.targetPoint.y = i.targetPoint.y;
								shotln.targetPoint.z = i.targetPoint.z;

								shotlinesuite.shotlines.push_back( shotln);					
							}
							json_spirit::Array shotlineJsonArray;
							shotlinesuite.ConvertToJson(shotlineJsonArray);				

							contentObj.push_back(json_spirit::Pair("shotLines", shotlineJsonArray));
						}
					}
					else
					{
						Blast_ReferencePlanes refPlanesList;

						for each( CADCommon_ReferencePlane i in in_ReferencePlanes)
						{
							Blast_ReferencePlane refPlane;
							refPlane.type = (i.type == CADCommon_GROUND) ? "Ground" : "Water";
							for each( const isis_CADCommon::Point_3D j in i.points )
							{							
								refPlane.points.push_back( Point(j.x, j.y, j.z));									
							}
							refPlanesList.planes.push_back(refPlane);
						}
						json_spirit::Array refJsonArray;
						refPlanesList.ConvertToJson(refJsonArray);
						contentObj.push_back(json_spirit::Pair("referencePlanes", refJsonArray));	
					}
					// 07-29-2013 end

					content = contentObj;
					std::ofstream outjsonfile(in_BallisticConfig_PathAndFileName);
					json_spirit::write_stream(content, outjsonfile, pretty_print);
					outjsonfile.close();
				} // END if (jsonfile.is_open())
			} // END if (boost::filesystem::exists(p))
			else
			{
				std::stringstream errorString;
				errorString << "Could not find survivability analysis file: " << in_BallisticConfig_PathAndFileName;  
				throw isis::application_exception(errorString.str());
			}
		}
		catch ( json_spirit::Error_position &ex )
		{
			std::stringstream errorString;
			errorString << "Error parsing file: " << in_BallisticConfig_PathAndFileName << "  Line: "<< ex.line_  << " Column: " << ex.column_ 
				<< " Error: " << ex.reason_;
			throw isis::application_exception(errorString.str());

		}
		catch(const std::exception &ex)
		{
			std::stringstream errorString;
			errorString << "Error parsing file: " << in_BallisticConfig_PathAndFileName << "  Error: " << ex.what() << std::endl;
			throw isis::application_exception(errorString.str());
		}
	}

}  // END namespace isis