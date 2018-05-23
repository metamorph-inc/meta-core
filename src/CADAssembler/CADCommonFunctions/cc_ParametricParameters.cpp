#include "cc_ParametricParameters.h"
#include "cc_CADFactoryAbstract.h"

#include <string>


namespace isis
{

void SetParametricParameterInCADModel ( const std::string									&in_ComponentInstanceID, 
										const std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map,
										const CADParameter									&in_CADParameter )
																						throw (isis::application_exception)
{

	std::map<std::string, isis::CADComponentData>::const_iterator itr;
	itr = in_CADComponentData_map.find(in_ComponentInstanceID);
		
	if ( itr == in_CADComponentData_map.end())
	{
		std::stringstream errorString;
		errorString << "Function - " << __FUNCTION__ << ", was passed an in_ComponentInstanceID that is not in in_CADComponentData_map. in_ComponentInstanceID:  " << in_ComponentInstanceID;
		throw isis::application_exception(errorString);	
	}


	isis::cad::CadFactoryAbstract_global *cadFactoryAbstract_global_ptr =	isis::cad::CadFactoryAbstract_global::instance();
	isis::cad::CadFactoryAbstract::ptr	                cAD_Factory_ptr =	cadFactoryAbstract_global_ptr->getCadFactoryAbstract_ptr();
	isis::cad::IModelOperations& modelOperations =							cAD_Factory_ptr->getModelOperations();


	///////////////////////////////////////////
	// Check if Parameter Exists in CAD Model
	///////////////////////////////////////////
	if ( !modelOperations.parameterDefinedInCADModel( in_CADParameter.name, in_ComponentInstanceID, in_CADComponentData_map ))
	{
		std::stringstream errorString;
		errorString << "Function - " << __FUNCTION__ << ", was passed an in_CADParameter that does not exists in the CAD Model, in_ComponentInstanceID:  " 
									<< in_ComponentInstanceID << ", CAD nodel name: " << itr->second.name;
		throw isis::application_exception(errorString);	
	}


	double scaleFactor = 1.0;
	double scaleOffset = 0.0;


	// Note if in_CADParameter.units.size() == 0 then the parameter in CADAssembly.xml has no units; and therefore, we will not change scaleFactor/scaleOffset
	if ( (in_CADParameter.type == CAD_FLOAT || in_CADParameter.type == CAD_INTEGER ) && in_CADParameter.units.size() != 0 )
	{
		///////////////////////////////////////////////////
		// Determine if parameter has units assigned to it
		///////////////////////////////////////////////////
		CADModelUnits parameterInCADModel_Units;

		modelOperations.retrieveParameterUnits( in_CADParameter.name, 
												in_ComponentInstanceID,
												in_CADComponentData_map,
												parameterInCADModel_Units );

		// We are only concerned with distance or angle units.  Not setting other units at this time. Parameters are usually varying length or angles in the CAD model
		if ( parameterInCADModel_Units.distanceUnit != CAD_UNITS_DISTANCE_NA  || parameterInCADModel_Units.angleUnit != CAD_UNITS_ANGLE_NA )
		{
			// This case:
			//		1) CADAssembly.xml has units set for the parameter
			//		2) Parameter in CAD model has units set

			// Note - if this section fails to do the conversion, an exception will be throw.  If that happens, then more unit types will need to be added to
			//		  the enums (e.g. CADUnitsDistance_enum and CADUnitsAngle_enum )

			if ( parameterInCADModel_Units.distanceUnit != CAD_UNITS_DISTANCE_NA  )
			{
				if ( CADUnitsDistance_enum( parameterInCADModel_Units.distanceUnit_ShortName) != CADUnitsDistance_enum(in_CADParameter.units))
				{
					// This case:
					//		1) CADAssembly.xml has units set for the parameter
					//		2) Parameter in CAD model has units set
					//		3) The parameter units in the CAD model are different from the units for the parameter in the CADAssembly.xml
					modelOperations.unitConversionFactorsComputation(	
						in_ComponentInstanceID,
						in_CADComponentData_map,
						CADUnitsDistance_string(CADUnitsDistance_enum(in_CADParameter.units)), // These are the units from CADAssembly.xml, must normalize
						parameterInCADModel_Units.distanceUnit_ShortName,
						scaleFactor,
						scaleOffset );	
				}
			}
			else
			{
				// This must be a CAD_UNITS_ANGLE
				if ( CADUnitsAngle_enum( parameterInCADModel_Units.angleUnit_ShortName) != CADUnitsAngle_enum(in_CADParameter.units))
				{
					// This case:
					//		1) CADAssembly.xml has units set for the parameter
					//		2) Parameter in CAD model has units set
					//		3) The parameter units in the CAD model are different from the units for the parameter in the CADAssembly.xml
					modelOperations.unitConversionFactorsComputation(	
						in_ComponentInstanceID,
						in_CADComponentData_map,
						CADUnitsAngle_string(CADUnitsAngle_enum(in_CADParameter.units)), // These are the units from CADAssembly.xml, must normalize
						parameterInCADModel_Units.angleUnit_ShortName,
						scaleFactor,
						scaleOffset );	
				}
			}
		}
		else
		{
			// This case:
			//		1) CADAssembly.xml has units set for the parameter
			//		2) Parameter in CAD model does NOT have units set

			// CAD models do not have angle units, so this section only applies to distance units.

			// Check the units for the CAD model.

			CADModelUnits cADModel_Units;

			modelOperations.retrieveCADModelUnits(	in_ComponentInstanceID,
													in_CADComponentData_map,
													cADModel_Units );

			if ( cADModel_Units.distanceUnit == CAD_UNITS_DISTANCE_NA )
			{
				// CAD model units should always be defined
				std::stringstream errorString;
				errorString << "Function - " << __FUNCTION__ << ", CAD model has no units set, in_ComponentInstanceID:  " 
											<< in_ComponentInstanceID << " CAD nodel name: " << itr->second.name;
				throw isis::application_exception(errorString);	
			}
			else
			{
				if ( CADUnitsDistance_enum( cADModel_Units.distanceUnit_ShortName) != CADUnitsDistance_enum(in_CADParameter.units))

				{
					// This case:
					//		1) CADAssembly.xml has units set for the parameter
					//		2) Parameter in CAD model does NOT have units set
					//		3) The CAD model units are different from the units for the parameter in the CADAssembly.xml
					modelOperations.unitConversionFactorsComputation(	
					in_ComponentInstanceID,
					in_CADComponentData_map,
					CADUnitsDistance_string(CADUnitsDistance_enum(in_CADParameter.units)), // These are the units from CADAssembly.xml, must normalize
					cADModel_Units.distanceUnit_ShortName,
					scaleFactor,
					scaleOffset );
				}

			}  // END Else

		} // End Else
	} // END if ( (in_CADParameter.type == CAD_FLOAT || in_CADParameter.type == CAD_INTEGER ) && in_CADParameter.units.size() != 0 )


	if ( (in_CADParameter.type == CAD_FLOAT || in_CADParameter.type == CAD_INTEGER  ) && scaleFactor != 1.0 ) 
	{
		std::string value_string;

		if ( in_CADParameter.type == CAD_FLOAT )
		{
			double value_double = 0;
			value_double = atof(in_CADParameter.value);
			value_double = value_double * scaleFactor + scaleOffset;
			value_string = std::to_string(value_double);
		}
		else
		{
			int value_int = 0;
			value_int = atoi(in_CADParameter.value);
			value_int = value_int * scaleFactor + scaleOffset;
			value_string = std::to_string(value_int);
		}

		modelOperations.setParameter(	in_CADParameter.type,
										in_CADParameter.name,
										MultiFormatString(value_string),
										in_ComponentInstanceID,
										in_CADComponentData_map);

		isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) <<  "   Parameter Successfully Set in CAD Model";
		isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) <<  "      Model Name            "  <<	itr->second.name;	
		isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) <<  "      ComponentID           "  <<	itr->second.componentID;	
		isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) <<  "      Type                  "  <<   CADParameterType_string(in_CADParameter.type);
		isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) <<  "      Name                  "	<<  in_CADParameter.name;
		isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) <<  "      Value                 "	<<  (std::string)in_CADParameter.value;
		isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) <<  "      Scale Factor          "	<<  scaleFactor;
		isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) <<  "      Scale Offset          "	<<  scaleOffset;
		isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) <<  "      Value after Scaling   "	<<  value_string;
	}
	else
	{		
		modelOperations.setParameter(	in_CADParameter.type,
										in_CADParameter.name,
										in_CADParameter.value,
										in_ComponentInstanceID,
										in_CADComponentData_map);

		isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) <<  "Parameter Successfully Set in CAD Model";
		isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) <<  "      Model Name            "  <<   itr->second.name;	
		isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) <<  "      ComponentID           "  <<   itr->second.componentID;	
		isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) <<  "      Type                  "  <<   CADParameterType_string(in_CADParameter.type);
		isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) <<  "      Name                  "	<<  in_CADParameter.name;
		isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) <<  "      Value                 "	<<  (std::string)in_CADParameter.value;

		if ( in_CADParameter.type == CAD_FLOAT || in_CADParameter.type == CAD_INTEGER  )
		{
			isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) <<  "      Scale Factor          "	<<  scaleFactor;
			isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) <<  "      Scale Offset          "	<<  scaleOffset;
			isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) <<  "      Value after Scaling   "	<<  (std::string)in_CADParameter.value;
		}
	}
						

} // END SetParametricParameterInCADModel

}  // END namespace isis
