#include <CommonFunctions.h>
#include <ToolKitPassThroughFunctions.h>
#include <cc_CommonFunctions.h>
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


///////////////////////////////////////////////////////////////////////////////////////////////////
void RetrieveUnits( //cad::CadFactoryAbstract		&in_Factory,
					ProMdl						in_Model,
					CADModelUnits				&out_CADModelUnits )
										throw(isis::application_exception)
{
	isis::cad::CadFactoryAbstract_global *cadFactoryAbstract_global_ptr = isis::cad::CadFactoryAbstract_global::instance();
	isis::cad::CadFactoryAbstract::ptr	cAD_Factory_ptr = cadFactoryAbstract_global_ptr->getCadFactoryAbstract_ptr();

	std::string unitsString;

	ProUnitsystem unitSystem;
	//ProUnititem unit, forceUnit, timeUnit, lengthUnit;
	ProUnititem massUnit, forceUnit, timeUnit, lengthUnit, temperatureUnit, angleUnit;
	//ProLine massUnitsLabel;
	//ProUnitsystemType type;

	isis::isis_ProMdlPrincipalunitsystemGet (in_Model, &unitSystem);

	//  PRO_UNITTYPE_LENGTH          L
	//  PRO_UNITTYPE_MASS            M
	//  PRO_UNITTYPE_FORCE           F
	//  PRO_UNITTYPE_TIME            T
	//	PRO_UNITTYPE_TEMPERATURE     D 

	
	isis::isis_ProUnitsystemUnitGet (&unitSystem, PRO_UNITTYPE_LENGTH, &lengthUnit); 

	MultiFormatString distanceUnit_multiformat( lengthUnit.name);
	out_CADModelUnits.distanceUnit = CADUnitsDistance_enum( distanceUnit_multiformat );
	ComputeUnitNames_Distance(  out_CADModelUnits.distanceUnit , out_CADModelUnits.distanceUnit_ShortName, out_CADModelUnits.distanceUnit_LongName );


	bool derived = false;
	try 
	{
		isis::isis_ProUnitsystemUnitGet (&unitSystem, PRO_UNITTYPE_MASS, &massUnit); 
		//ConvertCreoUnitToGMEUnit_Mass(  massUnit.name, out_MassUnit_ShortName, out_MassUnit_LongName );

	}
	catch(...)
	{
		out_CADModelUnits.massUnit_LongName = "Derived";
		derived = true;
	}

	if ( !derived ) 
	{
		//modelOperations.convertCADUnitToGMEUnit_Mass(massUnit.name, out_CADModelUnits.massUnit_ShortName,out_CADModelUnits.massUnit_LongName );
		MultiFormatString massUnit_multiformat( massUnit.name);
		out_CADModelUnits.massUnit = CADUnitsMass_enum( massUnit_multiformat );
		ComputeUnitNames_Mass(  out_CADModelUnits.massUnit , out_CADModelUnits.massUnit_ShortName, out_CADModelUnits.massUnit_LongName );
	}

	derived = false;
	try 
	{
		isis::isis_ProUnitsystemUnitGet (&unitSystem, PRO_UNITTYPE_FORCE, &forceUnit); 
		//ConvertCreoUnitToGMEUnit_Force( forceUnit.name, out_ForceUnit_ShortName, out_ForceUnit_LongName );
		
	}
	catch(...)
	{
		out_CADModelUnits.forceUnit_ShortName = "Derived";
		out_CADModelUnits.forceUnit_LongName  = "Derived";
		derived = true;
	}
	if ( !derived ) 
	{
		//modelOperations.convertCADUnitToGMEUnit_Force( forceUnit.name, out_CADModelUnits.forceUnit_ShortName, out_CADModelUnits.forceUnit_LongName );
		MultiFormatString forceUnit_multiformat( forceUnit.name);
		out_CADModelUnits.forceUnit = CADUnitsForce_enum( forceUnit_multiformat );
		ComputeUnitNames_Force(  out_CADModelUnits.forceUnit , out_CADModelUnits.forceUnit_ShortName, out_CADModelUnits.forceUnit_LongName );
	}

	
	isis::isis_ProUnitsystemUnitGet (&unitSystem, PRO_UNITTYPE_TEMPERATURE , &temperatureUnit); 
	//ConvertCreoUnitToGMEUnit_Temperature( temperatureUint.name, out_TemperatureUnit_ShortName, out_TemperatureUnit_LongName );
	//modelOperations.convertCADUnitToGMEUnit_Temperature(temperatureUnit.name, out_CADModelUnits.temperatureUnit_ShortName, out_CADModelUnits.temperatureUnit_LongName );
	MultiFormatString temperatureUnit_multiformat( temperatureUnit.name);
	out_CADModelUnits.temperatureUnit = CADUnitsTemperature_enum( temperatureUnit_multiformat );
	ComputeUnitNames_Temperature(  out_CADModelUnits.temperatureUnit , out_CADModelUnits.temperatureUnit_ShortName, out_CADModelUnits.temperatureUnit_LongName );

	isis::isis_ProUnitsystemUnitGet (&unitSystem, PRO_UNITTYPE_TIME, &timeUnit);  
	//ConvertCreoUnitToGMEUnit_Time( timeUnit.name, out_TimeUnit_ShortName, out_TimeUnit_LongName );
	//modelOperations.convertCADUnitToGMEUnit_Time( timeUnit.name, out_CADModelUnits.timeUnit_ShortName, out_CADModelUnits.timeUnit_LongName );
	MultiFormatString timeUnit_multiformat( timeUnit.name);
	out_CADModelUnits.timeUnit = CADUnitsTime_enum( timeUnit_multiformat );
	ComputeUnitNames_Time(  out_CADModelUnits.timeUnit , out_CADModelUnits.timeUnit_ShortName, out_CADModelUnits.timeUnit_LongName );

	isis::isis_ProUnitsystemUnitGet(&unitSystem, PRO_UNITTYPE_ANGLE, &angleUnit);
	// FIXME Creo reports rad, but it seems deg is used everywhere
	// MultiFormatString angleUnit_multiformat(angleUnit.name);
	MultiFormatString angleUnit_multiformat("deg");
	out_CADModelUnits.angleUnit = CADUnitsAngle_enum(angleUnit_multiformat);
	ComputeUnitNames_Angle(out_CADModelUnits.angleUnit, out_CADModelUnits.angleUnit_ShortName, out_CADModelUnits.angleUnit_LongName);
}

///////////////////////////////////////////////////////////////////////////////////////
					
void RetrieveUnits_withDescriptiveErrorMsg( 
					const std::string								&in_ComponentInstanceID,
					const isis::CADComponentData						&in_CADComponentData,
					CADModelUnits									&out_CADModelUnits )
											throw(isis::application_exception)
{
	
	try
	{
		RetrieveUnits(	in_CADComponentData.cADModel_hdl,
						out_CADModelUnits ); 

	}
	catch(const isis::application_exception &exc)
	{		
		std::stringstream errorString;
		errorString << "Error retrieving model units:" <<  std::endl <<
						"   ComponentInstanceID: " << in_ComponentInstanceID <<  std::endl <<
						"   ModelName:           " << in_CADComponentData.name <<  std::endl <<
						"   Error:               " << exc.what();
		throw isis::application_exception(errorString.str());
	}

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RetrieveDatumPointCoordinates( //cad::CadFactoryAbstract						&in_Factory,
									const std::string								&in_AssemblyComponentID,
									const std::string								&in_PartComponentID,
									const std::map<string, isis::CADComponentData>	&in_CADComponentData_map,
									const MultiFormatString							&in_DatumName,
									CADPoint											&out_CADPoint) 
																				throw (isis::application_exception)						
{

	//wchar_t  datum_name[PRO_NAME_SIZE ];
	//ProStringToWstring(datum_name, (char *)in_DatumName.c_str() );

	isis::cad::CadFactoryAbstract_global *cadFactoryAbstract_global_ptr = isis::cad::CadFactoryAbstract_global::instance();
	isis::cad::CadFactoryAbstract::ptr	cAD_Factory_ptr = cadFactoryAbstract_global_ptr->getCadFactoryAbstract_ptr();

	std::map<std::string, isis::CADComponentData>::const_iterator itr_part;
	itr_part = in_CADComponentData_map.find(in_PartComponentID);
	if ( itr_part == in_CADComponentData_map.end())
	{
		std::stringstream errorString;
		errorString << "Function - " << __FUNCTION__ << ", was passed an in_PartComponentID that is not in in_CADComponentData_map. in_PartComponentID:  " << in_PartComponentID;
		throw isis::application_exception(errorString);	
	}

	ProModelitem  datum_point;
	//isis::isis_ProModelitemByNameInit_WithDescriptiveErrorMsg (
	//	in_PartComponentID, in_CADComponentData_map[in_PartComponentID].name, ProMdlType_enum(in_CADComponentData_map[in_PartComponentID].modelType),
	//	in_CADComponentData_map[in_PartComponentID].cADModel_hdl, PRO_POINT, (wchar_t*)(const wchar_t*)in_DatumName, &datum_point);

	isis::isis_ProModelitemByNameInit_WithDescriptiveErrorMsg (
		in_PartComponentID, 
		itr_part->second.name, ProMdlType_enum( itr_part->second.modelType),
		itr_part->second.cADModel_hdl, PRO_POINT, (wchar_t*)(const wchar_t*)in_DatumName, &datum_point);


	ProPoint  point;
	isis::isis_ProPointInit (	(ProSolid) datum_point.owner,  // ProSolid   owner_handle,
								datum_point.id,
								&point);

	ProVector part_xyz_point;
	isis::isis_ProPointCoordGet (point, part_xyz_point);



	
	double transformationMatrix[4][4];
	//RetrieveTranformationMatrix_Assembly_to_Child (	in_AssemblyComponentID,
	//													in_CADComponentData_map[in_PartComponentID].componentPaths,
	//													in_CADComponentData_map,  
	//													PRO_B_TRUE,  // bottom up
	//													transformationMatrix);

	 isis::cad::IModelOperations&         modelOperations = cAD_Factory_ptr->getModelOperations();
	// modelOperations.retrieveTranformationMatrix_Assembly_to_Child( in_AssemblyComponentID,
	//																in_CADComponentData_map[in_PartComponentID].componentPaths,
	//																in_CADComponentData_map,  
	//																true,  // bottom up
	//																transformationMatrix);

	 modelOperations.retrieveTranformationMatrix_Assembly_to_Child( in_AssemblyComponentID,
																	in_PartComponentID,
																	in_CADComponentData_map,  
																	true,  // bottom up
																	transformationMatrix);

			
	 ProVector from_assembly_xyz_point;
	 isis::isis_ProPntTrfEval( part_xyz_point, transformationMatrix, from_assembly_xyz_point);

	 out_CADPoint.x = from_assembly_xyz_point[0];
	 out_CADPoint.y = from_assembly_xyz_point[1];
	 out_CADPoint.z = from_assembly_xyz_point[2];
	 
}





} // END namespace isis
