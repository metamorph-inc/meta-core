#include <CommonFunctions.h>
#include <string>


namespace isis
{
	/**
	std::ostream& operator <<(std::ostream & out, const bool & value) {
	    out << Bool_string(value);
		return out;
	}

	std::string  Bool_string( bool in_Bool )
	{
		if ( in_Bool )
			return "TRUE";
		else
			return "FALSE";
	}
	**/



	// This function is necessary  because the working directory buffer (e.g. setCreoWorkingDirectory_buffer
	// in the setCreoWorkingDirectory function) must be persisted between calls to isis_ProDirectoryChange.
	// This is because after the initial call to isis_ProDirectoryChange, isis_ProDirectoryChange appears to 
	// access the address of the buffer used in the previous call.  Therefore, the same address must be used 
	// between calls, or at least, the previously used buffer address must still be valid.
	//
	// This function is not thread safe.
	void setCreoWorkingDirectory ( const MultiFormatString &in_MultiFormatString ) throw (isis::application_exception)
	{
		

		isis_LOG(lg, isis_FILE, isis_INFO)	<< "setCreoWorkingDirectory: " << in_MultiFormatString;
		static wchar_t *setCreoWorkingDirectory_buffer = NULL;

		if ( !setCreoWorkingDirectory_buffer) setCreoWorkingDirectory_buffer = new wchar_t[PRO_PATH_SIZE];
		
		wcscpy_s( setCreoWorkingDirectory_buffer, PRO_PATH_SIZE, (const wchar_t*)in_MultiFormatString);
		isis::isis_ProDirectoryChange( setCreoWorkingDirectory_buffer );
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	std::string CombineCreoModelNameAndSuffix ( const std::string &in_ModelName, ProMdlType in_ModelType )
																		throw (isis::application_exception)
	{
		if ( in_ModelName.size() == 0 )
		{
			std::stringstream errorString;
			errorString <<
			"exception: Function" + std::string(__FUNCTION__) + " was passed a null string for in_ModelName.";
			throw isis::application_exception(errorString.str());
		}

		std::string tempString;
		switch (in_ModelType)
		{
			case PRO_PART:
				tempString = in_ModelName + ".prt";
				break;
			case PRO_ASSEMBLY:
				tempString = in_ModelName + ".asm";
				break;
			default:
				std::stringstream errorString;
				errorString <<
				"exception: Function" + std::string(__FUNCTION__) + " was passed in_ModelType that was not PRO_PART or PRO_ASSEMBLY, in_ModelType: " << in_ModelType;
				throw isis::application_exception(errorString.str());
		
		}
		return tempString;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void	ComputeBoundingBox( e_ComputeBoundingBoxMethod		in_ComputeBoundingBoxMethod,
							ProSolid							in_p_solid,
							isis_CADCommon::Point_3D			&out_BoundingBox_Point_1,
							isis_CADCommon::Point_3D			&out_BoundingBox_Point_2,
							double								out_Dimensions_xyz[3] )
																		throw (isis::application_exception)
	{
		Pro3dPnt  r_outline_points[2];
	
		if ( in_ComputeBoundingBoxMethod == CAD_PRO_SOLID_OUTLINE_COMPUTE )
		{
			ProMatrix transf= {{1,0.0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
			ProSolidOutlExclTypes excludes[] = {	PRO_OUTL_EXC_DATUM_PLANE, 
													PRO_OUTL_EXC_DATUM_POINT, 
													PRO_OUTL_EXC_DATUM_CSYS, 
													PRO_OUTL_EXC_DATUM_AXES, 
													PRO_OUTL_EXC_FACETS, 
													PRO_OUTL_EXC_ALL_CRVS }; 

			isis_ProSolidOutlineCompute(	in_p_solid, 
                                            transf,
                                            excludes,
                                            6,
                                            r_outline_points);
		}
		else
		{
			isis::isis_ProSolidOutlineGet( in_p_solid, r_outline_points);
		}

		out_BoundingBox_Point_1.x = r_outline_points[0][0];
		out_BoundingBox_Point_1.y = r_outline_points[0][1];
		out_BoundingBox_Point_1.z = r_outline_points[0][2];

		out_BoundingBox_Point_2.x = r_outline_points[1][0];
		out_BoundingBox_Point_2.y = r_outline_points[1][1];
		out_BoundingBox_Point_2.z = r_outline_points[1][2];

		out_Dimensions_xyz[0] =  abs(r_outline_points[1][0] - r_outline_points[0][0]);  
		out_Dimensions_xyz[1] =  abs(r_outline_points[1][1] - r_outline_points[0][1]);  
		out_Dimensions_xyz[2] =  abs(r_outline_points[1][2] - r_outline_points[0][2]);

	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	std::string CAD_SimplifiedRep_InclusionStatus_string(  e_CAD_SimplifiedRep_InclusionStatus in_SimplifiedRep_InclusionStatus_enum)
											throw (isis::application_exception)
	{
		switch (in_SimplifiedRep_InclusionStatus_enum )
		{
			case CAD_SIMPLIFIED_REP_INCLUDE:
				return "CAD_SIMPLIFIED_REP_INCLUDE";
				break;
			case CAD_SIMPLIFIED_REP_EXCLUDE:
				return "CAD_SIMPLIFIED_REP_EXCLUDE";
				break;

			default:
				char temp_char_array[64];
				string temp_string = "Function CAD_SimplifiedRep_InclusionStatus_string was passed " + 
					std::string(itoa(in_SimplifiedRep_InclusionStatus_enum, temp_char_array, 10)) + 
					" which is an erroneous type.  Allowed enum values are CAD_SIMPLIFIED_REP_INCLUDE and CAD_SIMPLIFIED_REP_EXCLUDE.";
				throw isis::application_exception(temp_string.c_str());
	  }
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	std::map<int, CAD_SimplifiedRepData> FeatureID_to_SimplifiedRepData_map_temp;

	ProError SimplifiedRep_Action (
						ProSimprepitem  *item,
						ProError         err, 
						ProSolid         owner)
	{
		
		//std::cout << std::endl << "-----------------+ SimplifiedRep_Action,  owner:                     " << owner;
		//std::cout << std::endl << "-----------------+ SimplifiedRep_Action,  item->item_path.feat_id:   " << item->item_path.feat_id;
		//std::cout << std::endl << "-----------------+ SimplifiedRep_Action,  item->action.type:         " << item->action.type;
		//std::cout << std::endl << "-----------------+ SimplifiedRep_Action,  item->item_path.path_size: " << item->item_path.path_size;
		//std::cout << std::endl << "-----------------+ SimplifiedRep_Action,  item->item_path.comp_path[i]: ";
		//for ( int i = 0; i < item->item_path.path_size ; ++ i ) 
		//	     std::cout << std::endl << "-----------------+                  " << item->item_path.comp_path[i];
		//std::cout << std::endl;

		// This function is only intended to be called for an assembly where the simplified rep exists in the assembly.
		// No deeper hierarchies, other than one assembly, are supported.
		// Therefore, item_path.comp_path[0 ] should contain the feature ID of the .prt/.asm which is either include or exclude from the
		// representation.
		if ( item->item_path.path_size != 1 ) 
		{	
			// It would be safer to throw an exception here, but at this point I am not
			// sure about all the perturbations of simplified reps.  The goal is to 
			// determine if a .prt or .asm was include or exclude.  If the simplified
			// rep is doing other things, then allow this function to continue 
			// would be appropriate.
			return PRO_TK_NO_ERROR;
		}

		// Note: item->item_path.feat_id would be set to -1.  This means the tem->item_path defines the list of feature IDs to the .prt/.asm.
		//		 For our case the path only has one feature ID.

		int feature_ID = item->item_path.comp_path[0];  

		CAD_SimplifiedRepData cAD_SimplifiedRepData_temp;

		cAD_SimplifiedRepData_temp.featureID = feature_ID;

		if ( item->action.type == PRO_SIMPREP_EXCLUDE )
			cAD_SimplifiedRepData_temp.inclusionStatus = CAD_SIMPLIFIED_REP_EXCLUDE;
		else
			cAD_SimplifiedRepData_temp.inclusionStatus = CAD_SIMPLIFIED_REP_INCLUDE;

		FeatureID_to_SimplifiedRepData_map_temp[feature_ID] = cAD_SimplifiedRepData_temp;

		return PRO_TK_NO_ERROR;
	}
	

	/*
	class SimplifiedRep_Action
	{

	public:
		//ProError operator() (){ return PRO_TK_NO_ERROR; };

		ProError operator() (
							ProSimprepitem  *item,
							ProError         err, 
							ProSolid         owner)
		{
		
			std::cout << std::endl << "-----------------+ SimplifiedRep_Action,  owner:                     " << owner;
			std::cout << std::endl << "-----------------+ SimplifiedRep_Action,  item->item_path.feat_id:   " << item->item_path.feat_id;
			std::cout << std::endl << "-----------------+ SimplifiedRep_Action,  item->action.type:         " << item->action.type;
			std::cout << std::endl << "-----------------+ SimplifiedRep_Action,  item->item_path.path_size: " << item->item_path.path_size;
			std::cout << std::endl << "-----------------+ SimplifiedRep_Action,  item->item_path.comp_path[i]: ";
			for ( int i = 0; i < item->item_path.path_size ; ++ i ) 
					 std::cout << std::endl << "-----------------+                  " << item->item_path.comp_path[i];

			std::cout << std::endl;

			return PRO_TK_NO_ERROR;
		}

	};
	*/

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	
	void AssemblySimplifiedRep_RetrieveModelInclusionStatus ( 
								ProMdl		  &in_Owner,
								ProSimprep &in_SimplifiedRep,
								std::map<int, CAD_SimplifiedRepData> &out_FeatureID_to_SimplifiedRepData_map ) 
											throw (isis::application_exception)
	{
		// Erase the global map
		FeatureID_to_SimplifiedRepData_map_temp.erase(FeatureID_to_SimplifiedRepData_map_temp.begin(),FeatureID_to_SimplifiedRepData_map_temp.end()); 
		
		ProSimprepdata       *simpRepData;

		//ProError SimplifiedRep_Action();

		isis_ProSimprepdataGet( &in_SimplifiedRep, &simpRepData);

		isis_ProSimprepdataitemsVisit (simpRepData, NULL, (ProFunction)SimplifiedRep_Action, (ProAppData) in_Owner);

		
		/* This did not work, tried to use a functor
		SimplifiedRep_Action simplifiedRep_Action_obj;
		ProSimprepitem  *item_dummy = NULL;
		ProError         err_dummy;
		ProSolid         owner_dummy;

		isis_ProSimprepdataitemsVisit (	simpRepData, NULL, 
										(ProFunction)simplifiedRep_Action_obj(item_dummy,err_dummy,owner_dummy ), 
										(ProAppData) in_Owner);

		*/

		out_FeatureID_to_SimplifiedRepData_map = FeatureID_to_SimplifiedRepData_map_temp;

		FeatureID_to_SimplifiedRepData_map_temp.erase(FeatureID_to_SimplifiedRepData_map_temp.begin(),FeatureID_to_SimplifiedRepData_map_temp.end());

		isis_ProSimprepdataFree( &simpRepData);
	}

	ProAsmcomp getProAsmcomp(const CADAssembledFeature &in_CADAssembledFeature )
	{
		ProAsmcomp ProAsmcomp_temp;
		ProAsmcomp_temp.type =	FeatureGeometryType_enum(in_CADAssembledFeature.type);
		ProAsmcomp_temp.id   =	                         in_CADAssembledFeature.id;
		ProAsmcomp_temp.owner =	                         in_CADAssembledFeature.owner; 

		return ProAsmcomp_temp;

	}

	CADAssembledFeature getCADAssembledFeature( const ProAsmcomp &in_ProAsmcomp )
	{
		CADAssembledFeature CADAssembledFeature_temp;

		CADAssembledFeature_temp.type	= CADFeatureGeometryType_enum(in_ProAsmcomp.type);
		CADAssembledFeature_temp.id		= in_ProAsmcomp.id;
		CADAssembledFeature_temp.owner	= in_ProAsmcomp.owner;	

		return CADAssembledFeature_temp;
	}


} // END namespace isis
