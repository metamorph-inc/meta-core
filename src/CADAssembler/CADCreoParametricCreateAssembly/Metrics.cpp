#include "stdafx.h"
#include <Metrics.h>
#include <fstream>
#include <CADMetrics.h>
#include <ToolKitPassThroughFunctions.h>
#include <iomanip>
#include <cc_GraphicsFunctions.h>
#include "cc_Joint.h"
#include <cc_CommonUtilities.h>
#ifndef ISIS_VERSION_NUMBER_H
#define ISIS_VERSION_NUMBER_H
#include <ISISVersionNumber.h>
#include <cc_CommonFunctions.h>
#endif

namespace isis
{

///////////////////////////////////////////////////////////////////////////////////////

void  ConvertCreoUnitToGMEUnit_Distance ( ProName in_DistanceUnit, std::string &out_ShortName, std::string &out_LongName  )
{
	char stringBuffer[PRO_NAME_SIZE];  // PRO_NAME_SIZE = 32

	std::string unit = ProWstringToString( stringBuffer, in_DistanceUnit );

	out_ShortName	= unit;
	out_LongName	= unit;

	if ( unit == "in" ) { out_ShortName = "inch";	out_LongName = "inch"; }
	if ( unit == "ft" ) { out_ShortName = "foot";	out_LongName = "foot"; }
	if ( unit == "mm" ) { out_ShortName = "mm";		out_LongName = "millimeter"; }
	if ( unit == "cm" ) { out_ShortName = "cm";		out_LongName = "centimeter"; }
	if ( unit == "m" )	{ out_ShortName = "m";		out_LongName = "meter"; }
	if ( unit == "km" )	{ out_ShortName = "km";		out_LongName = "kilometer"; }

}

///////////////////////////////////////////////////////////////////////////////////////
void  ConvertCreoUnitToGMEUnit_Mass ( ProName in_MassUnit,  std::string &out_ShortName, std::string &out_LongName  )
{
	char stringBuffer[PRO_NAME_SIZE];  // PRO_NAME_SIZE = 32

	std::string unit = ProWstringToString( stringBuffer, in_MassUnit );

	out_ShortName	= unit;
	out_LongName	= unit;

	if ( unit == "lbm" ){ out_ShortName = "lbm";		out_LongName = "poundmass"; }
	if ( unit == "g" )	{ out_ShortName = "g";			out_LongName = "gram"; }
	if ( unit == "kg" )	{ out_ShortName = "kg";			out_LongName = "kilogram"; }
	if ( unit == "tonne" )	{ out_ShortName = "tonne";  out_LongName = "tonne"; }

}
///////////////////////////////////////////////////////////////////////////////////////
void  ConvertCreoUnitToGMEUnit_Force ( ProName in_ForceUnit, std::string &out_ShortName, std::string &out_LongName  )
{
	char stringBuffer[PRO_NAME_SIZE];    // PRO_NAME_SIZE = 32

	std::string unit = ProWstringToString( stringBuffer, in_ForceUnit );

	out_ShortName	= unit;
	out_LongName	= unit;

	if ( unit == "lbf" ) { out_ShortName = "lbf";	out_LongName = "poundforce"; }
	if ( unit == "N" )	 { out_ShortName = "N";		out_LongName = "newton"; }

}
///////////////////////////////////////////////////////////////////////////////////////
void  ConvertCreoUnitToGMEUnit_Temperature ( ProName in_Temperature, std::string &out_ShortName, std::string &out_LongName  )
{
	char stringBuffer[PRO_NAME_SIZE];  // PRO_NAME_SIZE = 32

	std::string unit = ProWstringToString( stringBuffer, in_Temperature );

	out_ShortName	= unit;
	out_LongName	= unit;

	if ( unit == "C" )	{ out_ShortName = "C";	out_LongName = "centigrade"; }
	if ( unit == "F" )	{ out_ShortName = "F";	out_LongName = "fahrenheit"; }
	if ( unit == "K" )	{ out_ShortName = "K";	out_LongName = "kelvin"; }

}
///////////////////////////////////////////////////////////////////////////////////////
void ConvertCreoUnitToGMEUnit_Time ( ProName in_TimeUnit, std::string &out_ShortName, std::string &out_LongName  )
{
	char stringBuffer[PRO_NAME_SIZE];  // PRO_NAME_SIZE = 32

	std::string unit = ProWstringToString( stringBuffer, in_TimeUnit );

	out_ShortName	= unit;
	out_LongName	= unit;

	if ( unit == "sec" ) { out_ShortName = "sec";	out_LongName = "second"; }

}



///////////////////////////////////////////////////////////////////////////////////////////////////
void RetrieveUnits( ProMdl			in_Model,
					std::string		&out_DistanceUnit_ShortName,
					std::string		&out_DistanceUnit_LongName,
					
					std::string		&out_MassUnit_ShortName,
					std::string		&out_MassUnit_LongName,

					std::string		&out_ForceUnit_ShortName,
					std::string		&out_ForceUnit_LongName,

					std::string		&out_TimeUnit_ShortName,
					std::string		&out_TimeUnit_LongName,

					std::string		&out_TemperatureUnit_ShortName, 
					std::string		&out_TemperatureUnit_LongName )
											throw(isis::application_exception)
{
	std::string unitsString;

	ProUnitsystem unitSystem;
	//ProUnititem unit, forceUnit, timeUnit, lengthUnit;
	ProUnititem massUnit, forceUnit, timeUnit, lengthUnit, temperatureUint;
	ProLine massUnitsLabel;
	ProUnitsystemType type;

	isis::isis_ProMdlPrincipalunitsystemGet (in_Model, &unitSystem);

	//  PRO_UNITTYPE_LENGTH          L
	//  PRO_UNITTYPE_MASS            M
	//  PRO_UNITTYPE_FORCE           F
	//  PRO_UNITTYPE_TIME            T
	//	PRO_UNITTYPE_TEMPERATURE     D 

	
	isis::isis_ProUnitsystemUnitGet (&unitSystem, PRO_UNITTYPE_LENGTH, &lengthUnit); 
	ConvertCreoUnitToGMEUnit_Distance( lengthUnit.name,out_DistanceUnit_ShortName, out_DistanceUnit_LongName  );

	try 
	{
		isis::isis_ProUnitsystemUnitGet (&unitSystem, PRO_UNITTYPE_MASS, &massUnit); 
		ConvertCreoUnitToGMEUnit_Mass(  massUnit.name, out_MassUnit_ShortName, out_MassUnit_LongName );
	}
	catch(...)
	{
		out_MassUnit_LongName = "Derived";
	}
	
	try 
	{
	isis::isis_ProUnitsystemUnitGet (&unitSystem, PRO_UNITTYPE_FORCE, &forceUnit); 
	ConvertCreoUnitToGMEUnit_Force( forceUnit.name, out_ForceUnit_ShortName, out_ForceUnit_LongName );
	}
	catch(...)
	{
		out_ForceUnit_ShortName = "Derived";
		out_ForceUnit_LongName = "Derived";
	}
	
	isis::isis_ProUnitsystemUnitGet (&unitSystem, PRO_UNITTYPE_TEMPERATURE , &timeUnit); 
	ConvertCreoUnitToGMEUnit_Temperature( timeUnit.name, out_TemperatureUnit_ShortName, out_TemperatureUnit_LongName );
	
	isis::isis_ProUnitsystemUnitGet (&unitSystem, PRO_UNITTYPE_TIME, &temperatureUint);  
	ConvertCreoUnitToGMEUnit_Time( temperatureUint.name, out_TimeUnit_ShortName, out_TimeUnit_LongName );

}

///////////////////////////////////////////////////////////////////////////////////////
	void RetrieveUnits_withDescriptiveErrorMsg( 
					const std::string				&in_ComponentInstanceID,
					const isis::MultiFormatString	&in_ModelName,				
					ProMdl							in_Model,
					std::string						&out_DistanceUnit_ShortName,
					std::string						&out_DistanceUnit_LongName,
					
					std::string						&out_MassUnit_ShortName,
					std::string						&out_MassUnit_LongName,

					std::string						&out_ForceUnit_ShortName,
					std::string						&out_ForceUnit_LongName,

					std::string						&out_TimeUnit_ShortName,
					std::string						&out_TimeUnit_LongName,

					std::string						&out_TemperatureUnit_ShortName, 
					std::string						&out_TemperatureUnit_LongName )
											throw(isis::application_exception)
	{
		try
		{
			RetrieveUnits(	in_Model,
							out_DistanceUnit_ShortName,
							out_DistanceUnit_LongName,
					
							out_MassUnit_ShortName,
							out_MassUnit_LongName,

							out_ForceUnit_ShortName,
							out_ForceUnit_LongName,

							out_TimeUnit_ShortName,
							out_TimeUnit_LongName,

							out_TemperatureUnit_ShortName, 
							out_TemperatureUnit_LongName ); 

		}
		catch(const isis::application_exception &exc)
		{		
			std::stringstream errorString;
			errorString << "Error retrieving model units:" <<  std::endl <<
							"   ComponentInstanceID: " << in_ComponentInstanceID <<  std::endl <<
							"   ModelName:           " << in_ModelName <<  std::endl <<
							"   Error:               " << exc.what();
			throw isis::application_exception(errorString.str());
		}

	}

///////////////////////////////////////////////////////////////////////////////////////
/*
void RetrieveMaterial(	const std::string &in_ModelName, 
						ProSolid part, 
						std::string &out_MaterialName ) throw(isis::application_exception)
{

	ProMaterial  material;

	try
	{
		isis::isis_ProMaterialCurrentGet( part, &material );

		char stringBuffer[PRO_NAME_SIZE];  // PRO_NAME_SIZE = 32
		out_MaterialName = ProWstringToString( stringBuffer, material.matl_name );	
	}
	catch (...)
	{
		// out_MaterialName = "NOT_DEFINED";
		std::string TempError = 
					"Material not defined for part: " + in_ModelName + 
					".  To compute mass properties, all parts must have a material assignment. "  +
					" Please open the model with Creo and select File Prepare \"Model Properties\"" +
					" to assign a material to the model.";
					throw isis::application_exception(TempError.c_str());		
	}
}
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
void PopulateMatrix(	int					in_NumRows, 
						int					in_NumColumns, 
						const double		in_MatrixBuffer[4][4],  
						CADMetrics::Rows	&out_RowsRoot)
{
	for (int i = 0; i < in_NumRows; ++i )
	{
		CADMetrics::Row		   rowRoot = CADMetrics::Row::Create(out_RowsRoot);

		for (int j = 0; j < in_NumColumns; ++j )
		{
			CADMetrics::Column	columnRoot = CADMetrics::Column::Create(rowRoot);
			columnRoot.Value() = in_MatrixBuffer[i][j];
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void PopulateScalar( const std::string		in_Name,
					 double					in_Value,
					 const std::string		in_Unit,
					 CADMetrics::Scalars	&out_ScalarsRoot )
{
	CADMetrics::Scalar   scalarRoot = CADMetrics::Scalar::Create( out_ScalarsRoot );
	scalarRoot.Name()  = in_Name;
	scalarRoot.Value() = in_Value;
	scalarRoot.Unit()  = in_Unit;
}

struct MetricsDefined
{
	bool massProp_Defined;
	bool interiaAtDefaultCSys_Defined;
	bool interiaAtCOG_Defined;
	bool surfaceArea_Defined;

	MetricsDefined(): massProp_Defined(false), 
						interiaAtDefaultCSys_Defined(false),
						interiaAtCOG_Defined(false),
						surfaceArea_Defined(false) {}
	bool anomalyOccurred() 
	{ 
		return (!massProp_Defined || !interiaAtDefaultCSys_Defined || !interiaAtCOG_Defined || !surfaceArea_Defined);
	}
	
							 
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////
void Populate_Single_MetricComponent( 
			const std::string													&in_ComponentID,
			std::map<std::string, isis::CADComponentData>						&in_CADComponentData_map,  
			std::set<int>														&in_out_DataCreated_for_MetricIDs,
			std::map<int, MetricsDefined>										&out_MetricID_to_Anomalies_map,
			CADMetrics::MetricComponents										&out_metricComponentsRoot )
																						throw (isis::application_exception)
{
	
	
	if ( !in_CADComponentData_map[in_ComponentID].massProperties.massProperties_RetrievalInvoked )
	{
			std::stringstream errorString;
			errorString << "ERROR, Function - " + std::string(__FUNCTION__) << ", could not create metrics file.  Mass properties were not retrieved prior to writing metrics file."; 
			throw isis::application_exception(errorString.str());
	}

	if ( !in_CADComponentData_map[in_ComponentID].boundingBox.boundingBox_Defined )
	{
			std::stringstream errorString;
			errorString << "ERROR, Function - " + std::string(__FUNCTION__) << ", could not create metrics file.  Bounding box was not retrieved prior to writing metrics file."; 
			throw isis::application_exception(errorString.str());
	}

	// Check if data for the MetricID has NOT been created.
	if ( in_out_DataCreated_for_MetricIDs.find( in_CADComponentData_map[in_ComponentID].metricID ) == in_out_DataCreated_for_MetricIDs.end() )
	{
		////////////////////////////
		// Create Metric Component
		////////////////////////////
		CADMetrics::MetricComponent  metricComponentRoot = 	CADMetrics::MetricComponent::Create( out_metricComponentsRoot );

		////////////////////////
		// Add Attributes
		////////////////////////
		metricComponentRoot.CoordinateSystem() = std::string("DEFAULT");
		metricComponentRoot.MetricID() = in_CADComponentData_map[in_ComponentID].metricID;
		metricComponentRoot.Name() = std::string(in_CADComponentData_map[in_ComponentID].name);

		//metricComponentRoot.SpecialInstruction() = isis::SpecialInstruction_string(in_CADComponentData_map[in_ComponentID].specialInstruction);
		metricComponentRoot.SpecialInstruction() = in_CADComponentData_map[in_ComponentID].specialInstruction.ToCommaDelimitedString();

		if ( in_CADComponentData_map[in_ComponentID].cyPhyComponent )
			metricComponentRoot.ComponentDefinitionSource() =  ComponentDefinitionSource_string(COMPONENT_DEFINITION_SOURCE_CYPHY);
		else
			metricComponentRoot.ComponentDefinitionSource() =  ComponentDefinitionSource_string(COMPONENT_DEFINITION_SOURCE_CAD_MODEL);

		// Model Type
		//if ( in_CADComponentData_map[in_ComponentID].modelType == PRO_MDL_PART )
		if ( in_CADComponentData_map[in_ComponentID].modelType == CAD_MDL_PART )
			metricComponentRoot.Type() = "PART";
		else
			metricComponentRoot.Type() = "ASSEMBLY";

		///////////////////////////////
		// Mass Properties
		///////////////////////////////

		double   MatrixBuffer[4][4];
		double   MatrixBuffer_temp[4][4];

		/////////////////////////////////////////
		// Check if Mass Properties are Defined
		////////////////////////////////////////
		MetricsDefined MetricsDefined;

		MetricsDefined.massProp_Defined				= in_CADComponentData_map[in_ComponentID].massProperties.massProperties_Defined;
		MetricsDefined.interiaAtDefaultCSys_Defined = in_CADComponentData_map[in_ComponentID].massProperties.coordSysInertiaTensor_Defined;
		MetricsDefined.interiaAtCOG_Defined			= in_CADComponentData_map[in_ComponentID].massProperties.cGInertiaTensor_Defined;
		MetricsDefined.surfaceArea_Defined			= in_CADComponentData_map[in_ComponentID].massProperties.surfaceArea_Defined;

		if ( MetricsDefined.anomalyOccurred() )
			out_MetricID_to_Anomalies_map[in_CADComponentData_map[in_ComponentID].metricID] = MetricsDefined;

		////////////////////////
		// CG
		////////////////////////
		
		CADMetrics::CG  cGRoot = CADMetrics::CG::Create( metricComponentRoot );

		cGRoot.X() = in_CADComponentData_map[in_ComponentID].massProperties.centerOfGravity[0];
		cGRoot.Y() = in_CADComponentData_map[in_ComponentID].massProperties.centerOfGravity[1];
		cGRoot.Z() = in_CADComponentData_map[in_ComponentID].massProperties.centerOfGravity[2];

		////////////////////////
		// Bounding Box
		////////////////////////

		CADMetrics::BoundingBox boundingBoxRoot = CADMetrics::BoundingBox::Create(metricComponentRoot);

		boundingBoxRoot.X() =  in_CADComponentData_map[in_ComponentID].boundingBox.Dimensions_xyz[0];
		boundingBoxRoot.Y() =  in_CADComponentData_map[in_ComponentID].boundingBox.Dimensions_xyz[1];
		boundingBoxRoot.Z() =  in_CADComponentData_map[in_ComponentID].boundingBox.Dimensions_xyz[2];

		CADMetrics::OutlinePoints rootOutlinePoints = CADMetrics::OutlinePoints::Create(boundingBoxRoot);

		CADMetrics::Point  rootPoint_1 = CADMetrics::Point::Create(rootOutlinePoints);
		CADMetrics::Point  rootPoint_2 = CADMetrics::Point::Create(rootOutlinePoints);
		rootPoint_1.X() = in_CADComponentData_map[in_ComponentID].boundingBox.boundingBox_Point_1.x;
		rootPoint_1.Y() = in_CADComponentData_map[in_ComponentID].boundingBox.boundingBox_Point_1.y;
		rootPoint_1.Z() = in_CADComponentData_map[in_ComponentID].boundingBox.boundingBox_Point_1.z;

		rootPoint_2.X() = in_CADComponentData_map[in_ComponentID].boundingBox.boundingBox_Point_2.x;
		rootPoint_2.Y() = in_CADComponentData_map[in_ComponentID].boundingBox.boundingBox_Point_2.y;
		rootPoint_2.Z() = in_CADComponentData_map[in_ComponentID].boundingBox.boundingBox_Point_2.z;

		///////////////////////////////////////////////
		// Interia Tensor at Default Coordinate System
		//////////////////////////////////////////////
		if ( MetricsDefined.massProp_Defined && MetricsDefined.interiaAtDefaultCSys_Defined )
		{
			for ( int i = 0; i < 3; ++i )
				for ( int j = 0; j < 3; ++j )
					MatrixBuffer[i][j] =in_CADComponentData_map[in_ComponentID].massProperties.coordSysInertiaTensor[i][j];
		
			CADMetrics::InertiaTensor  inertiaDefaultCSysTensorRoot = CADMetrics::InertiaTensor::Create( metricComponentRoot );
			inertiaDefaultCSysTensorRoot.At() = isis::CADInertiaAt_string(CAD_DEFAULT_CSYS);
		
			CADMetrics::Rows		   rowsInertiaDefaultCSysTensorRoot = CADMetrics::Rows::Create(inertiaDefaultCSysTensorRoot);

			PopulateMatrix(	3, 3, MatrixBuffer, rowsInertiaDefaultCSysTensorRoot  );
		}
		///////////////////////////////////////////////
		// Interia Tensor at Center of Gravity
		//////////////////////////////////////////////
		if (  MetricsDefined.massProp_Defined && MetricsDefined.interiaAtCOG_Defined )
		{
			for ( int i = 0; i < 3; ++i )
				for ( int j = 0; j < 3; ++j )
					MatrixBuffer[i][j] = in_CADComponentData_map[in_ComponentID].massProperties.cGInertiaTensor[i][j];
		
			CADMetrics::InertiaTensor  inertiaCOGTensorRoot = CADMetrics::InertiaTensor::Create( metricComponentRoot );
			inertiaCOGTensorRoot.At() = isis::CADInertiaAt_string(CAD_CENTER_OF_GRAVITY);
			CADMetrics::Rows		   rowsInertiaCOGTensorRoot = CADMetrics::Rows::Create(inertiaCOGTensorRoot);

			PopulateMatrix(	3, 3, MatrixBuffer, rowsInertiaCOGTensorRoot  );
		}
		//////////////////////////////////
		// Principal Moments of Intertia
		//////////////////////////////////

		if (  MetricsDefined.massProp_Defined )
		{

			// Rotation Matrix
			CADMetrics::PrincipleMomentsOfInertia   principleMomentsOfInertiaRoot = CADMetrics::PrincipleMomentsOfInertia::Create( metricComponentRoot );
			CADMetrics::RotationMatrix			    rotationMatrixRoot = CADMetrics::RotationMatrix::Create( principleMomentsOfInertiaRoot );
			CADMetrics::Rows						rowsRotationMatrixRoot = CADMetrics::Rows::Create(rotationMatrixRoot);

			for ( int i = 0; i < 3; ++i )
				for ( int j = 0; j < 3; ++j )
					MatrixBuffer[i][j] =  in_CADComponentData_map[in_ComponentID].massProperties.principalAxis_RotationMatrix[i][j];
			PopulateMatrix(	3, 3, MatrixBuffer, rowsRotationMatrixRoot  );
	
			// Principal Moments
			CADMetrics::Rows	rowsPrincipleMomentsOfInertiaValues = CADMetrics::Rows::Create(principleMomentsOfInertiaRoot);

			for ( int i = 0; i < 3; ++i ) MatrixBuffer[i][0] =  in_CADComponentData_map[in_ComponentID].massProperties.principalMomentsOfInertia[i];

			PopulateMatrix(	3, 1,MatrixBuffer, rowsPrincipleMomentsOfInertiaValues  );
		}	

		////////////////////////
		// Units
		////////////////////////

		std::string		distanceUnit_LongName;
		std::string		distanceUnit_ShortName;
		
		std::string		massUnit_LongName;
		std::string		massUnit_ShortName;

		std::string		forceUnit_LongName;
		std::string		forceUnit_ShortName;

		std::string		temperatureUnit_LongName;
		std::string		temperatureUnit_ShortName;
		
		std::string		timeUnit_LongName;
		std::string		timeUnit_ShortName;
		
		//std::cout << std::endl << "=====> Model Units, Model: " << in_CADComponentData_map[in_ComponentID].name;
		
		RetrieveUnits_withDescriptiveErrorMsg( 
						in_CADComponentData_map[in_ComponentID].componentID,
						in_CADComponentData_map[in_ComponentID].name,
						in_CADComponentData_map[in_ComponentID].cADModel_hdl, 
						distanceUnit_ShortName,		distanceUnit_LongName, 
						massUnit_ShortName,			massUnit_LongName, 
						forceUnit_ShortName,			forceUnit_LongName, 
						timeUnit_ShortName,			timeUnit_LongName, 
						temperatureUnit_ShortName,	temperatureUnit_LongName );
		
		std::string forceUnit_temp;
		if ( forceUnit_LongName == "Derived" )
			forceUnit_temp = massUnit_ShortName + " " + distanceUnit_ShortName + "/" + timeUnit_ShortName + "2";
		else
			forceUnit_temp = forceUnit_ShortName;


		CADMetrics::Units  unitsRoot = CADMetrics::Units::Create( metricComponentRoot );

		unitsRoot.Distance()	= distanceUnit_LongName;
		unitsRoot.Force()		= forceUnit_temp;
		unitsRoot.Mass()		= massUnit_LongName;
		unitsRoot.Temperature() = temperatureUnit_LongName;
		unitsRoot.Time()		= timeUnit_LongName;

		////////////////////////
		// Scalars
		////////////////////////
		if ( MetricsDefined.massProp_Defined || MetricsDefined.surfaceArea_Defined )
		{
			CADMetrics::Scalars  scalarsRoot = CADMetrics::Scalars::Create( metricComponentRoot );
			//   ScalarType (const Value_type&,
			//        const Name_type&,
			//        const Unit_type&);

			// Surface Area
			if ( MetricsDefined.surfaceArea_Defined )
				PopulateScalar( "SurfaceArea", in_CADComponentData_map[in_ComponentID].massProperties.surfaceArea, distanceUnit_ShortName + "2", scalarsRoot );

			if (MetricsDefined.massProp_Defined)
			{
				// Volume
				PopulateScalar( "Volume", in_CADComponentData_map[in_ComponentID].massProperties.volume, distanceUnit_ShortName + "3", scalarsRoot );

				// Density
				// Note - Only parts have a density.  Assemblies do not have densities.
				//if ( in_CADComponentData_map[in_ComponentID].modelType == PRO_MDL_PART )
				if ( in_CADComponentData_map[in_ComponentID].modelType == CAD_MDL_PART )
				{
					PopulateScalar( "Density", in_CADComponentData_map[in_ComponentID].massProperties.density, massUnit_ShortName + "/" + distanceUnit_ShortName + "3", scalarsRoot );
				}

				// Mass
				PopulateScalar( "Mass", in_CADComponentData_map[in_ComponentID].massProperties.mass, massUnit_ShortName, scalarsRoot );
			}
		}

		//////////////////////////////////
		// Material
		//////////////////////////////////
		
		//if ( in_CADComponentData_map[in_ComponentID].modelType == PRO_MDL_PART )
		if ( in_CADComponentData_map[in_ComponentID].modelType == CAD_MDL_PART )
		{
			std::string material_temp = "Undefined";
			try
			{ // zzzz Material need to check for 
				RetrieveMaterial(	in_CADComponentData_map[in_ComponentID].name,
									static_cast<ProSolid>(in_CADComponentData_map[in_ComponentID].cADModel_hdl), material_temp );
			}
			catch (...)
			{
			}

			CADMetrics::Material  materialRoot = CADMetrics::Material::Create( metricComponentRoot );
			materialRoot.Type() = material_temp;
		}

		////////////////////////
		// Children
		////////////////////////
		if ( in_CADComponentData_map[in_ComponentID].children.size() > 0 )
		{

			CADMetrics::Children childrenRoot = CADMetrics::Children::Create( metricComponentRoot );

			for ( std::list<std::string>::const_iterator i = in_CADComponentData_map[in_ComponentID].children.begin();
				  i != in_CADComponentData_map[in_ComponentID].children.end();
				  ++i )
			{

				// The last item in componentPaths is the path from the parent assembly to the child.  The other
				// entries in componentPaths are paths from higher up assemblies.  
				std::list<int>  assemblyToChild_compentPath;
				assemblyToChild_compentPath.push_back( in_CADComponentData_map[*i].componentPaths.back());

				RetrieveTranformationMatrix_Assembly_to_Child ( in_ComponentID,  
																assemblyToChild_compentPath,
																in_CADComponentData_map,  
																//PRO_B_FALSE,  // bottom up = False
																PRO_B_TRUE,  // bottom up = True, Changed this 8/8/2013, V1.4.6
																MatrixBuffer );
				// At this point MatrixBuffer would contain
				// Rotation		3 X 3,  rows 0 thru 2
				// Translation	1 X 3,  row 3

				// Rotation Matrix
				CADMetrics::ChildMetric		 childMetricRoot = CADMetrics::ChildMetric::Create( childrenRoot );
				childMetricRoot.MetricID() = in_CADComponentData_map[*i].metricID;
				childMetricRoot.ComponentInstanceID() = in_CADComponentData_map[*i].componentID;
				CADMetrics::RotationMatrix	 childRotationMatrixRoot = CADMetrics::RotationMatrix::Create( childMetricRoot );
				CADMetrics::Rows			 rowsChildRotationMatrixRoot = CADMetrics::Rows::Create( childRotationMatrixRoot );

				// Transpose the rotation matrix. Ricardo and SWRI need the transposed the roatation matrix.
				// Interactively, if you retrieve the transformation from the assembly to the detail part, the
				// transposed matrix is returned.  This change was done for v1.4.17.0, 10/02/2013
				for ( int i = 0; i < 3; ++i )
					for ( int j = 0; j < 3; ++j )
						MatrixBuffer_temp[j][i] =  MatrixBuffer[i][j];

				PopulateMatrix(	3, 3, MatrixBuffer_temp, rowsChildRotationMatrixRoot  );

				CADMetrics::Translation	 childTranslationRoot = CADMetrics::Translation::Create( childMetricRoot );
				childTranslationRoot.X() = MatrixBuffer[3][0];
				childTranslationRoot.Y() = MatrixBuffer[3][1];
				childTranslationRoot.Z() = MatrixBuffer[3][2];

			}

			

		}  // END if ( in_CADComponentData_map[in_ComponentID].children.size() > 0 )
	    
		in_out_DataCreated_for_MetricIDs.insert(in_CADComponentData_map[in_ComponentID].metricID);

	}  // END if ( in_out_DataCreated_for_MetricIDs.find( in_CADComponentData_map[in_ComponentID].metricID ) == in_out_DataCreated_for_MetricIDs.end() )
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
void Populate_MetricComponents( 
			const std::string																&in_ComponentID,
			std::map<std::string, isis::CADComponentData>									&in_CADComponentData_map,  
			std::set<int>																	&in_out_DataCreated_for_MetricIDs,
			std::map<int, MetricsDefined>													&out_MetricID_to_Anomalies_map,
			CADMetrics::MetricComponents													&out_metricComponentsRoot )
																						throw (isis::application_exception)
{

	Populate_Single_MetricComponent(	in_ComponentID, 
										in_CADComponentData_map,  
										in_out_DataCreated_for_MetricIDs,
										out_MetricID_to_Anomalies_map,
										out_metricComponentsRoot );
	

	if ( in_CADComponentData_map[in_ComponentID].modelType == CAD_MDL_ASSEMBLY )
	{
		for ( std::list<string>::const_iterator i(in_CADComponentData_map[in_ComponentID].children.begin());
			i != in_CADComponentData_map[in_ComponentID].children.end();
			++i )
		{

			Populate_MetricComponents( 	*i,
										in_CADComponentData_map,  
										in_out_DataCreated_for_MetricIDs,
										out_MetricID_to_Anomalies_map,
										out_metricComponentsRoot );
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
void PopulateJoints_for_SingleComponent( 
			const std::string													&in_ComponentID,
			std::map<std::string, isis::CADComponentData>						&in_CADComponentData_map,  
			std::set<int>														&in_out_DataCreated_for_MetricIDs,
			std::map<int, MetricsDefined>										&out_MetricID_to_Anomalies_map,
			CADMetrics::Joints													&out_jointsRoot )
																						throw (isis::application_exception)
{
	


	if ( in_CADComponentData_map[in_ComponentID].constraintDef.constraints.size() == 0  ) return;  // No constraints and therefore no joints


	////////////////////////////
	// Create Joint Component
	////////////////////////////

	for each ( const ConstraintData &i in  in_CADComponentData_map[in_ComponentID].constraintDef.constraints )
	{
		// For now, skip constraints that do not have a junction defined.  When the junction information
		// is completed for CyPhy leaf-assemblies, then constraints should have a junction defined.
		// At that time, an exception should be thrown instead of the continue in the following statement.
		if ( !i.computedJointData.junctiondDefined_withoutGuide ) continue;


		///////////////////////////////////
		// Get Constrained-to Information
		///////////////////////////////////
		std::set<std::string>  constrainedToComponentInstanceIDs;

		// Derived from Constraint Pairs
		for each ( std::string j in i.constrainedTo_ComponentInstanceIDs_DerivedFromConstraintPairs )
		{
			//if ( in_CADComponentData_map[j].modelType == PRO_MDL_PART ||
			if ( in_CADComponentData_map[j].modelType == CAD_MDL_PART ||
				 (in_CADComponentData_map[j].specialInstruction != CAD_SPECIAL_INSTRUCTION_HAS_KINEMATIC_JOINT)) 
									constrainedToComponentInstanceIDs.insert(j);
		}

		// Inferred from Leaf Assemblies
		for each ( std::string j in i.constrainedTo_ComponentInstanceIDs_InferredFromLeafAssemblySubordinates )
		{
			//if ( in_CADComponentData_map[j].modelType == PRO_MDL_PART ||
			if ( in_CADComponentData_map[j].modelType == CAD_MDL_PART ||
			   (in_CADComponentData_map[j].specialInstruction != CAD_SPECIAL_INSTRUCTION_HAS_KINEMATIC_JOINT)) 
									constrainedToComponentInstanceIDs.insert(j);
		}

		// If there are no constrained-to parts, continue.  This would occur for the first part added to the assembly,
		// which would be constrained to its assembly.  Constraints between parts are of interest, but constraints
		// to assemblies are not of interest.
		if ( constrainedToComponentInstanceIDs.size() == 0 )
		{
			isis_LOG(lg, isis_FILE, isis_WARN) << "Component was not constrained to a part. This is usually due to the first component added to an assembly would only be constrained to the assembly and thus not constrained to a part. " + in_ComponentID;
			continue;  
		}

		//////////////////////////////
		// Joint
		/////////////////////////////

		CADMetrics::Joint  jointRoot = CADMetrics::Joint::Create(out_jointsRoot);

		//jointRoot.AssembledComponentInstanceID() = i.constrainedFrom_InstanceID.size()!=0?i.constrainedFrom_InstanceID:in_ComponentID;
		jointRoot.AssembledComponentInstanceID() = in_ComponentID;
		jointRoot.Type() = CADJointType_string( i.computedJointData.jointType_withoutguide );

		jointRoot.ID() = i.inputJoint.JointID;

		//if (in_CADComponentData_map[jointRoot.AssembledComponentInstanceID()].modelType == PRO_MDL_ASSEMBLY && 
		if (in_CADComponentData_map[jointRoot.AssembledComponentInstanceID()].modelType == CAD_MDL_ASSEMBLY && 
			in_CADComponentData_map[jointRoot.AssembledComponentInstanceID()].specialInstruction == CAD_SPECIAL_INSTRUCTION_HAS_KINEMATIC_JOINT )
		{
			isis_LOG(lg, isis_FILE, isis_WARN) << "Only parts and assemblies without CAD_SPECIAL_INSTRUCTION_HAS_KINEMATIC_JOINT appear in joints: " << (string)jointRoot.AssembledComponentInstanceID();
			continue;
		}

		//////////////////////////////
		// ConstrainedToComponents
		/////////////////////////////

		CADMetrics::ConstrainedToComponents  constrainedToComponentsRoot = CADMetrics::ConstrainedToComponents::Create(jointRoot);

		//for each ( ConstraintPair j in  i.constraintPairs )
		//	for each (ConstraintFeature k in j.constraintFeatures)
		//		if ( k.componentInstanceID != in_ComponentID ) constrainedToComponentInstanceIDs.insert(k.componentInstanceID);

		// This does not belong here
		// if (constrainedToComponentInstanceIDs.size()>1)
		//{
		//	throw isis::application_exception("There are more than 2 parts mixed up within one connection. This is a modelling mistake. Component ID: " + in_ComponentID);
		//}


		//if (i.constrainedTo_InstanceID.size()!=0 && constrainedToComponentInstanceIDs.size()==1)
		//{
		//	constrainedToComponentInstanceIDs.clear();
		//	constrainedToComponentInstanceIDs.insert(i.constrainedTo_InstanceID);
		//}

		// Check for dependencies due to a leaf assembly in CyPhy having subordinates
		//for each ( const std::string &constrainedto in i.constrainedTo_ComponentInstanceIDs_InferredFromLeafAssemblySubordinates)
		//{
		//	constrainedToComponentInstanceIDs.insert(constrainedto);
		//}


		for each ( const std::string &j in constrainedToComponentInstanceIDs)
		{
			CADMetrics::ConstrainedToComponent  constrainedToComponent = CADMetrics::ConstrainedToComponent::Create(constrainedToComponentsRoot);		
			constrainedToComponent.ComponentInstanceID() =  j;
		}

		//////////////////////////////
		// GlobalCoordinateSystem
		/////////////////////////////
		CADMetrics::GlobalCoordinateSystem  globalCoordinateSystemRoot = CADMetrics::GlobalCoordinateSystem::Create(jointRoot);

		CADMetrics::Location		location	 = CADMetrics::Location::Create(globalCoordinateSystemRoot);
		CADMetrics::Orientation		orientation	 = CADMetrics::Orientation::Create(globalCoordinateSystemRoot);
		CADMetrics::Rotation		rotation	 = CADMetrics::Rotation::Create(globalCoordinateSystemRoot);

		std::vector<double> values;
		// location
		values.push_back(i.computedJointData.globalJointPosition.location.x);
		values.push_back(i.computedJointData.globalJointPosition.location.y);
		values.push_back(i.computedJointData.globalJointPosition.location.z);
		location.ArrayValue() = values;

		location.Units() = "millimeter";

		// orientation
		values.clear();
		values.push_back(i.computedJointData.globalJointPosition.orientation.x);
		values.push_back(i.computedJointData.globalJointPosition.orientation.y);
		values.push_back(i.computedJointData.globalJointPosition.orientation.z);
		orientation.ArrayValue() = values;
		orientation.Units() = "millimeter";

		
		// Rotation
		rotation.Value() = i.computedJointData.globalJointPosition.rotation;
		rotation.Units() = "radians";
		
	}  // END for each ( ConstraintData i in  in_CADComponentData_map[in_ComponentID].constraintDef.constraints )

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void Populate_Joints( 
			const std::string																&in_ComponentID,
			std::map<std::string, isis::CADComponentData>									&in_CADComponentData_map,  
			std::set<int>																	&in_out_DataCreated_for_MetricIDs,
			std::map<int, MetricsDefined>													&out_MetricID_to_Anomalies_map,
			CADMetrics::Joints													&out_jointsRoot )
																						throw (isis::application_exception)
{

	// Only add joint information for parts or assemblies that should be treated as one body.
	//if ( in_CADComponentData_map[in_ComponentID].modelType == PRO_MDL_PART ||
	if ( in_CADComponentData_map[in_ComponentID].modelType == CAD_MDL_PART ||
	   ( in_CADComponentData_map[in_ComponentID].specialInstruction != CAD_SPECIAL_INSTRUCTION_HAS_KINEMATIC_JOINT ) )	
			PopulateJoints_for_SingleComponent(	in_ComponentID, 
												in_CADComponentData_map,  
												in_out_DataCreated_for_MetricIDs,
												out_MetricID_to_Anomalies_map,
												out_jointsRoot );
	
//	if ( in_CADComponentData_map[in_ComponentID].modelType == PRO_MDL_ASSEMBLY )
	if ( in_CADComponentData_map[in_ComponentID].modelType == CAD_MDL_ASSEMBLY )
	{
		for ( std::list<string>::const_iterator i(in_CADComponentData_map[in_ComponentID].children.begin());
			i != in_CADComponentData_map[in_ComponentID].children.end();
			++i )
		{

			Populate_Joints( 	*i,
										in_CADComponentData_map,  
										in_out_DataCreated_for_MetricIDs,
										out_MetricID_to_Anomalies_map,
										out_jointsRoot );
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Populate_Joints_MetaData( 
			const std::vector<std::string>					&in_ListOfComponentIDsInTheAssembly, 
			std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map,
			CADMetrics::Joints								&out_jointsRoot )
																throw (isis::application_exception)
{
	bool foundProcessingInstruction = false;
	for each ( const std::string &i in in_ListOfComponentIDsInTheAssembly)
	{
		//if (	in_CADComponentData_map[i].modelType ==  PRO_MDL_ASSEMBLY  &&
		if (	in_CADComponentData_map[i].modelType ==  CAD_MDL_ASSEMBLY  &&
				CyPhyLeafAssembly ( i, in_CADComponentData_map ) &&
				in_CADComponentData_map[i].specialInstruction != CAD_SPECIAL_INSTRUCTION_HAS_KINEMATIC_JOINT  )
		{
			foundProcessingInstruction = true;
			break;
		}
	}

	if ( foundProcessingInstruction )
	{	
		CADMetrics::JointsMetaData  jointsMetaDataRoot = CADMetrics::JointsMetaData::Create(out_jointsRoot);

		for each ( const std::string &i in in_ListOfComponentIDsInTheAssembly)
		{
			//if (	in_CADComponentData_map[i].modelType ==  PRO_MDL_ASSEMBLY  &&
			if (	in_CADComponentData_map[i].modelType ==  CAD_MDL_ASSEMBLY  &&
					CyPhyLeafAssembly ( i, in_CADComponentData_map ) &&
					in_CADComponentData_map[i].specialInstruction != CAD_SPECIAL_INSTRUCTION_HAS_KINEMATIC_JOINT  )
			{
				CADMetrics::ComponentInstanceData  componentInstanceDataRoot = CADMetrics::ComponentInstanceData::Create(jointsMetaDataRoot);
				componentInstanceDataRoot.ComponentInstanceID() = i;
				componentInstanceDataRoot.Primary()   = Metrics_JointsComponentInstanceData_Primary_string(METRICS_JOINTS_LEAF_ASSEMBLY_NO_KINEMATIC_JOINTS);
				componentInstanceDataRoot.Secondary() = Metrics_JointsComponentInstanceData_Secondary_string(METRICS_JOINTS_TREAT_AS_ONE_BODY);
			}
		}
	}
}																					
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void Populate_Assemblies( 
			const std::string													&in_ComponentID,
			std::map<std::string, isis::CADComponentData>						&in_CADComponentData_map,  
			CADMetrics::CADComponent											&out_CADComponent )


{
	//if ( in_CADComponentData_map[in_ComponentID].modelType == PRO_MDL_ASSEMBLY )
	if ( in_CADComponentData_map[in_ComponentID].modelType == CAD_MDL_ASSEMBLY )
	{
		if ( in_CADComponentData_map[in_ComponentID].children.size() > 0 )
		{
			// We want the order to be in the order of the CAD Assembly and not in the order of CADAssembly.xml
			bool sortRequired = true;

			for each ( const std::string &i in in_CADComponentData_map[in_ComponentID].children )
			{
				if ( in_CADComponentData_map[i].dataInitialSource == INITIAL_SOURCE_DERIVED_FROM_LEAF_ASSEMBLY_DESCENDANTS ) 
				{
					sortRequired = false;
					break;
				}
			}

			std::vector<std::string> children;

			if ( sortRequired )
			{
				// Make sure sort keys are all unique.  This is a verification that a modification was not made elsewhere in this program
				// that caused the sort keys to be malformed.
				std::set<int> sortKeys;
				for each ( const std::string &i in in_CADComponentData_map[in_ComponentID].children )
				{
					if ( sortKeys.find(in_CADComponentData_map[i].addedToAssemblyOrdinal ) != sortKeys.end() )
					{
						std::stringstream errorString;
						errorString << std::endl << "Redundant sort key for a child of: "
						"  Component Instance ID: " <<  in_ComponentID << std::endl << 
						"  Model Name:            " <<  in_CADComponentData_map[in_ComponentID].name <<  std::endl << 
						"  Model Type:            " <<  CADMdlType_string(in_CADComponentData_map[in_ComponentID].modelType)  <<  std::endl << 
						"  Children:              ";
						for each ( const std::string &j in in_CADComponentData_map[in_ComponentID].children ) 
						{
							errorString << std::endl <<
							"     Component Instance ID: " <<  in_ComponentID << std::endl << 
							"     Model Name:   " <<  in_CADComponentData_map[j].name <<  std::endl << 
							"     Model Type:   " <<  CADMdlType_string(in_CADComponentData_map[j].modelType) <<  std::endl << 
							"	  Sort Ordinal: " <<  in_CADComponentData_map[j].addedToAssemblyOrdinal;
						}
						throw isis::application_exception(errorString.str());
					}
					else
					{
						sortKeys.insert(in_CADComponentData_map[i].addedToAssemblyOrdinal);
					}
				}

				std::map<int, std::string> sortID_to_ComponentInstanceID_map;
				for each ( const std::string &i in in_CADComponentData_map[in_ComponentID].children )
					sortID_to_ComponentInstanceID_map[in_CADComponentData_map[i].addedToAssemblyOrdinal] = i;

				for each ( const std::pair<int, std::string> &i in sortID_to_ComponentInstanceID_map )  children.push_back(i.second);
			}
			else
			{   
				// Sort not required
				for each ( const std::string &i in in_CADComponentData_map[in_ComponentID].children ) children.push_back(i);
			}


			//for ( std::list<string>::const_iterator i(in_CADComponentData_map[in_ComponentID].children.begin());
		 	//	i != in_CADComponentData_map[in_ComponentID].children.end();
			//	++i )

			for each ( const std::string &i in children )
			{
				CADMetrics::CADComponent	cADComponentRoot = CADMetrics::CADComponent::Create(out_CADComponent);

				cADComponentRoot.ComponentInstanceID() = i;
				cADComponentRoot.MetricID() = in_CADComponentData_map[i].metricID;

				//if ( in_CADComponentData_map[i].modelType == PRO_MDL_ASSEMBLY )
				if ( in_CADComponentData_map[i].modelType == CAD_MDL_ASSEMBLY )
				{
					Populate_Assemblies(	i,
											in_CADComponentData_map,  
											out_CADComponent );
				}
			}
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void Assign_MetricIDs_To_Components( 
			const std::string									&in_ComponentID,
			std::map<std::string, isis::CADComponentData>		&in_out_CADComponentData_map,  
			int													&in_out_NextMetricID,
			std::map< std::string, int>							&in_out_ModelName_MetricID_map,
			std::map<int, std::string>							&in_out_MetricID_ModelName_map )
																						throw (isis::application_exception)
{
	std::string ModelNameWithSuffix = AmalgamateModelNameWithSuffix( in_out_CADComponentData_map[in_ComponentID].name, 
																	 in_out_CADComponentData_map[in_ComponentID].modelType);

	if ( ( in_out_CADComponentData_map[in_ComponentID].parametricParametersPresent )  || 
		 ( in_out_CADComponentData_map[in_ComponentID].specialInstruction == CAD_SPECIAL_INSTRUCTION_SIZE_TO_FIT ) ||
		 ( in_out_ModelName_MetricID_map.find( ModelNameWithSuffix ) == in_out_ModelName_MetricID_map.end() ) )
	{
		in_out_CADComponentData_map[in_ComponentID].metricID = in_out_NextMetricID;

		// Don't add parametric and SIZE_TO_FIT models to in_out_ModelName_MetricID.  This will force
		// parametric and SIZE_TO_FIT models to always have a unique MetricID.
		if ( ( !in_out_CADComponentData_map[in_ComponentID].parametricParametersPresent )  && 
		 ( in_out_CADComponentData_map[in_ComponentID].specialInstruction != CAD_SPECIAL_INSTRUCTION_SIZE_TO_FIT ))
		{
			in_out_ModelName_MetricID_map[ModelNameWithSuffix] = in_out_NextMetricID;
		}
		in_out_MetricID_ModelName_map[in_out_NextMetricID] = ModelNameWithSuffix;
		++in_out_NextMetricID;
	}
	else
	{
		in_out_CADComponentData_map[in_ComponentID].metricID = in_out_ModelName_MetricID_map[ ModelNameWithSuffix ];
	}
	
	//if ( in_out_CADComponentData_map[in_ComponentID].modelType == PRO_MDL_ASSEMBLY )
	if ( in_out_CADComponentData_map[in_ComponentID].modelType == CAD_MDL_ASSEMBLY )
	{
		for ( std::list<string>::const_iterator i(in_out_CADComponentData_map[in_ComponentID].children.begin());
			i != in_out_CADComponentData_map[in_ComponentID].children.end();
			++i )
		{
			Assign_MetricIDs_To_Components( *i,
											in_out_CADComponentData_map,  
											in_out_NextMetricID,
											in_out_ModelName_MetricID_map,
											in_out_MetricID_ModelName_map);
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MetricsNotDefinedErrors( const MetricsDefined &in_MetricsDefined,
							  std::list<std::string> &out_ErrorStrings )
{
	if ( !in_MetricsDefined.massProp_Defined ) 
	{
		out_ErrorStrings.push_back( CADMetricsError_string(CAD_MASS_PROPERTIES_NOT_DEFINED));
	}
	else
	{
		if ( !in_MetricsDefined.interiaAtDefaultCSys_Defined ) 
			out_ErrorStrings.push_back( CADMetricsError_string(CAD_INTERIA_AT_DEFAULT_CSYS_NOT_DEFINED));
		if ( !in_MetricsDefined.interiaAtCOG_Defined ) 
			out_ErrorStrings.push_back( CADMetricsError_string(CAD_INTERIA_AT_CENTER_OF_GRAVITY_NOT_DEFINED));

	}

	if ( !in_MetricsDefined.surfaceArea_Defined )
		out_ErrorStrings.push_back( CADMetricsError_string(CAD_SURFACE_AREA_NOT_DEFINED));


	if (out_ErrorStrings.size() == 0 )
		out_ErrorStrings.push_back("NONE");

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Populate_Anomalies(	std::map<int, MetricsDefined>	&in_MetricID_to_Anomalies_map, 
							CADMetrics::Anomalies			&out_AanomaliesRoot)
{
	for each ( std::pair<int, MetricsDefined>  i in in_MetricID_to_Anomalies_map )
	{
		char metricID_buffer[64];
		itoa(i.first, metricID_buffer,10);

		std::list<std::string> errorStrings;

		MetricsNotDefinedErrors(i.second, errorStrings);

		for each (std::string j in errorStrings )
		{
			CADMetrics::Anomaly	anomalyRoot = CADMetrics::Anomaly::Create(out_AanomaliesRoot);
			anomalyRoot.MetricID() = metricID_buffer;
			anomalyRoot.Error() = j;
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Log_Anomalies(	const std::map<int, MetricsDefined>	&in_MetricID_to_Anomalies_map, 
					const std::string					&in_MeticsOutputXML_PathAndFileName,
						  std::map<int, std::string>	&in_MetricID_ModelName_map)
{
	
	if (in_MetricID_to_Anomalies_map.size() > 0 )
	{
		isis_LOG(lg, isis_FILE, isis_WARN) << "";
	}

	for each ( std::pair<int, MetricsDefined>  i in in_MetricID_to_Anomalies_map )
	{
		std::list<std::string> errorStrings;

		MetricsNotDefinedErrors(i.second, errorStrings);

		for each (std::string j in errorStrings ) 
			isis_LOG(lg, isis_FILE, isis_WARN) << METRICS_FILE_ERROR_string << ", Metric ID: " << i.first << "  Model Name: " <<  setiosflags(ios::left) << std::setw(35) <<in_MetricID_ModelName_map[i.first]  << " Error: " << j;
	}
	if (in_MetricID_to_Anomalies_map.size() > 0 ) 
		isis_LOG(lg, isis_FILE, isis_WARN) << "For a mapping of Metric ID to Component ID, see " << in_MeticsOutputXML_PathAndFileName;

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void OutputCADMetricsToXML_Driver( 
							bool											in_regenerationSucceeded_ForAllAssemblies,
							bool											in_OutputJoints,
							const isis::CADAssemblies						&in_CADAssemblies,
							std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map,  
							const std::string								&in_MeticsOutputXML_PathAndFileName,
							const std::string								&in_LogFile_PathAndFileName )
							throw (isis::application_exception, std::exception)
{
		
		

		isis::IfFileExists_DeleteFile( in_MeticsOutputXML_PathAndFileName);

		if ( in_regenerationSucceeded_ForAllAssemblies )
		{
			isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "";
			isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "Creating Metrics File";	
			
			bool metricsErrorOccurred;
			isis::OutputCADMetricsToXML(in_CADAssemblies, in_CADComponentData_map,  in_MeticsOutputXML_PathAndFileName, in_OutputJoints, metricsErrorOccurred);
			isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "   Created: " + in_MeticsOutputXML_PathAndFileName;

			if ( metricsErrorOccurred )
			{
				isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "   Error(s) occurred when creating the metrics file.  To view the errors, search on" << isis_EOL << 
										  "   " << isis::METRICS_FILE_ERROR_string <<" in "<< in_LogFile_PathAndFileName << ".  The errors are listed at the end of this file." << isis_EOL <<
										  "   Also, the errors are listed in the \"Anomalies\" section of " << in_MeticsOutputXML_PathAndFileName << ".";
			}
		}
		else
		{		
			isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "Metrics File - Did NOT create a metrics file because the attempts to regenerate the assembly failed.";	
		}

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void OutputCADMetricsToXML( const isis::CADAssemblies						&in_CADAssemblies,
							std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map,  
							const std::string								&in_MeticsOutputXML_PathAndFileName, 
							bool											in_OutputJoints,
							bool											&out_ErrorOccurred )
							throw (isis::application_exception, std::exception)
{
	////////////////////////
	// Setup UDM out file
	////////////////////////
	Udm::SmartDataNetwork dn_CADMetricRoot(CADMetrics::diagram);
	dn_CADMetricRoot.CreateNew(in_MeticsOutputXML_PathAndFileName, "CADMetrics", CADMetrics::CADMetricRoot::meta);

	try
	{
		int nextMetricID = 1;
		std::map< std::string, int > modelName_MetricID_map;
		std::map<int, std::string>	 metricID_ModelName_map; 
		std::set<int>				 dataCreated_for_MetricIDs;
		//std::set<int>				 metricIDs_ChildrenDefined;
	
		//////////////////////////////////////////////////////////////////////////////////
		// Assign MetricIDs to CADComponents (i.e. contained in in_CADComponentData_map )
		///////////////////////////////////////////////////////////////////////////////////
		for ( std::list<TopLevelAssemblyData>::const_iterator i(in_CADAssemblies.topLevelAssemblies.begin());
				i != in_CADAssemblies.topLevelAssemblies.end();
				++i )
		{	
			Assign_MetricIDs_To_Components( i->assemblyComponentID, 
											in_CADComponentData_map,  
											nextMetricID, 
											modelName_MetricID_map,
											metricID_ModelName_map);
		}

		//for ( std::map< std::string, int >::const_iterator temp_i(ModelName_MetricID.begin());
		//	  temp_i != ModelName_MetricID.end(); ++temp_i )
		//{
		//	std::cout << std::endl << "ModelName_MetricID, Model Name: " << temp_i->first << " MetricID: " << temp_i->second;
		//}

		//////////////////////////////
		// Cast to root of UDM object
		//////////////////////////////
		CADMetrics::CADMetricRoot CADMetricRoot = CADMetrics::CADMetricRoot::Cast(dn_CADMetricRoot.GetRootObject());
		CADMetricRoot.VersionInfo() = ISIS_METRIC_FILE_VERSION;

		//  Anomalies
		CADMetrics::Anomalies  anomaliesRoot = CADMetrics::Anomalies::Create( CADMetricRoot );

		//  Assemblies
		CADMetrics::Assemblies  assembliesRoot = CADMetrics::Assemblies::Create( CADMetricRoot );

		// MetricComponents
		CADMetrics::MetricComponents metricComponentsRoot = CADMetrics::MetricComponents::Create( CADMetricRoot );

		CADMetrics::Joints jointsRoot = CADMetrics::Joints::Create( CADMetricRoot );

		std::map<int, MetricsDefined> metricID_to_Anomalies_map;

		////////////////////////
		// Populate XML Tree
		////////////////////////
		for ( std::list<TopLevelAssemblyData>::const_iterator i(in_CADAssemblies.topLevelAssemblies.begin());
				i != in_CADAssemblies.topLevelAssemblies.end();
				++i )
		{	
			//CADMetrics::MetricComponent  metricComponentRoot = 	CADMetrics::MetricComponent::Create( metricComponentsRoot );
			Populate_MetricComponents(	i->assemblyComponentID, 
										in_CADComponentData_map, 								
										dataCreated_for_MetricIDs,
										metricID_to_Anomalies_map,
										metricComponentsRoot );	

			CADMetrics::Assembly    assemblyRoot = 	CADMetrics::Assembly::Create( assembliesRoot );
			assemblyRoot.ConfigurationID() = i->configurationID;
			CADMetrics::CADComponent    cADComponentRoot = 	CADMetrics::CADComponent::Create( assemblyRoot );
			cADComponentRoot.ComponentInstanceID() = i->assemblyComponentID;
			cADComponentRoot.MetricID() = in_CADComponentData_map[i->assemblyComponentID].metricID;

			Populate_Assemblies(	i->assemblyComponentID,
									in_CADComponentData_map,  
									cADComponentRoot );

			if ( in_OutputJoints)	
			{
				Populate_Joints( i->assemblyComponentID, 
								 in_CADComponentData_map, 								
								 dataCreated_for_MetricIDs,
								 metricID_to_Anomalies_map,
								 jointsRoot );	

				isis::ComponentVistorBuildListOfComponentIDs  assemblyComponentIDs_ExcludingTopAssembly(true);
				isis::VisitComponents(i->assemblyComponentID, in_CADComponentData_map, assemblyComponentIDs_ExcludingTopAssembly );

				Populate_Joints_MetaData(	assemblyComponentIDs_ExcludingTopAssembly.listOfComponentIDs,
											in_CADComponentData_map,
											jointsRoot );
			}

	
		}


		Populate_Anomalies( metricID_to_Anomalies_map, anomaliesRoot );
		Log_Anomalies( metricID_to_Anomalies_map, in_MeticsOutputXML_PathAndFileName, metricID_ModelName_map );

		if ( metricID_to_Anomalies_map.size() == 0 )
			out_ErrorOccurred = false;
		else
			out_ErrorOccurred = true;

		///////////////////////////
		// Write XML File
		//////////////////////////
		if ( dn_CADMetricRoot.isOpen()) dn_CADMetricRoot.CloseWithUpdate();
	}
	catch ( ... )
	{
		// If any error occurs, do not save the metrics xml file.  It makes no sense to save a metrics file
		// with erroneous data.
		if ( dn_CADMetricRoot.isOpen()) dn_CADMetricRoot.CloseNoUpdate();
		throw;
	}

}
	

} // end namespace isis