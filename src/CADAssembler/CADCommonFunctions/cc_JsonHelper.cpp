#include "cc_JsonHelper.h"

#include "cc_MiscellaneousFunctions.h"

#include "cc_LoggerBoost.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <sstream>

#include <fstream>
#include <iterator>
#include <string>
#include <set>
#include <iostream>

namespace isis_CADCommon
{

	void AddStepFileMappingToManufacturingManifest(
									const std::string &in_ManifestJson_PathAndFileName,
									const std::map<std::string, std::string> &in_ComponentInstanceId_to_StepFile_map,
									std::set<std::string> &out_ComponentInstanceIds_AddedToManifest ) 
															throw (isis::application_exception)
	{
		using boost::property_tree::ptree;
		//std::list<std::string> messages;
		try
		{
			boost::filesystem::path p(in_ManifestJson_PathAndFileName);
			if (boost::filesystem::exists(p))
			{
				if (boost::filesystem::is_regular_file(p))
				{
					std::string id = "";

					ptree pt;
					boost::property_tree::read_json(p.make_preferred().string(), pt);
					BOOST_FOREACH(ptree::value_type& child, pt.get_child("ComponentManufactureList")) 
					{
						//std::cout << child.second.get<string>("id") << "\n";
						id = child.second.get<std::string>("InstanceGUID");			

						if (id != "")
						{
							std::map<std::string, std::string>::const_iterator ci = in_ComponentInstanceId_to_StepFile_map.find(id);
							if (ci != in_ComponentInstanceId_to_StepFile_map.end())
							{
								child.second.put("STEPModel", ci->second);
								out_ComponentInstanceIds_AddedToManifest.insert(ci->first);
							}
						}
					}
					boost::property_tree::write_json(in_ManifestJson_PathAndFileName, pt);
					
				}
			}
			else
			{
				std::stringstream errorString;
				errorString << "Input file not found: " << in_ManifestJson_PathAndFileName;
				throw isis::application_exception(errorString.str());
			}
		}
		catch(const boost::property_tree::ptree_error &exc)
		{		
			std::stringstream errorString;
			errorString << "Input file processing error: " << in_ManifestJson_PathAndFileName 
						<< "  Error: " << exc.what();
			throw isis::application_exception(errorString.str());
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//id = pt.get<std::string>("Density");
	//if (id != "") 

	void UpdateHydrostaticsJsonFile( const ::boost::filesystem::path	in_HydrostaticsFile_PathAndFileName,
									 double		in_FluidDensity,
									 double		in_WaterLine_Height_zAxis,
									 double		in_ReferenceArea,  // Wetted surface area
									 double		in_DisplacedVolume,
									 double		in_HydrostaticVolume,
									 double		in_RightingMomentArm, 
									 double		in_CG_x, // Center of Gravity
									 double		in_CG_y,
									 double		in_CG_z,
									 double		in_CB_x,  // Center of Bouncy
									 double		in_CB_y,
									 double		in_CB_z,
									 std::vector< std::pair<double,double> > in_XSect ) // cross sections
												throw (isis::application_exception)
	{
		//log4cpp::Category& log_cf = log4cpp::Category::getInstance(LOGCAT_CONSOLEANDLOGFILE);

		std::stringstream errorString;
		using boost::property_tree::ptree;
		// std::set<std::string> notFoundTags; fix this later
		try
		{
			boost::filesystem::path p(in_HydrostaticsFile_PathAndFileName);
			if (! boost::filesystem::exists(p)) {
				errorString << "Input file not found: " << in_HydrostaticsFile_PathAndFileName;
				throw isis::application_exception(errorString.str());
			}
			if (! boost::filesystem::is_regular_file(p)) {
				errorString << "Input file not regular found: " << in_HydrostaticsFile_PathAndFileName;
				throw isis::application_exception(errorString.str());
			}
			std::string canonical_path = p.make_preferred().string();
			isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "writing hydrostatics.json : " << canonical_path;
			std::string id = "";
			
			ptree pt;
			boost::property_tree::read_json(canonical_path, pt);

			pt.put<std::string>("UpdatedDateTime", isis_CADCommon::GetDayMonthTimeYear());

			pt.put<double>("Density", in_FluidDensity);
			pt.put<double>("CG_X", in_CG_x);
			pt.put<double>("CG_Y", in_CG_y);
			pt.put<double>("CG_Z", in_CG_z);
			pt.put<double>("DisplacedVolume", in_DisplacedVolume);
			pt.put<double>("HydrostaticVolume", in_HydrostaticVolume);

			BOOST_FOREACH(ptree::value_type& child, pt.get_child("Data")) 
			{
				isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "writing hydrostatics.json data";
				ptree& ct = child.second;

				ct.put<double>("WaterLine", in_WaterLine_Height_zAxis);
				ct.put<double>("ReferenceArea", in_ReferenceArea);
				ct.put("RightingMomentArm", in_RightingMomentArm);	

				ct.put("CB_X", in_CB_x);
				ct.put("CB_Y", in_CB_y);
				ct.put("CB_Z", in_CB_z);

				ptree children;
				for each( std::pair<double,double> value in in_XSect ) {
					ptree section;
					section.put("Offset", value.first);
					section.put("Area", value.second);
					children.push_back( std::make_pair("", section) );
				}
				ct.put_child("XSections", children);
			}

			::boost::property_tree::write_json(canonical_path, pt);
		}
		catch(const boost::property_tree::ptree_error &exc)
		{		
			errorString << "Input file processing error: " << in_HydrostaticsFile_PathAndFileName 
						<< "  Error: " << exc.what();
			throw isis::application_exception(errorString.str());
		}	

	}

} // END isis_CADCommon