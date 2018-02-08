#include "stdafx.h"
#include "ModelMaterials.h"
#include <cc_CommonUtilities.h>
#include <isis_include_ptc_headers.h>
#include <isis_ptc_toolkit_functions.h>
#include <stdlib.h>

namespace isis
{

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// If a current material is not set in the Creo model, then the functions in this file will not work; and 
	// therefore, throw an exception if the current material is not set.
	void TestRetrievingCurrentMaterial (	const std::string								&in_AssemblyComponentID, 
											std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map) 
													throw (isis::application_exception)
	{
		try
		{
			// Get current material
			ProMaterial currentMaterial;
			isis::isis_ProMaterialCurrentGet( static_cast<ProSolid>(in_CADComponentData_map[in_AssemblyComponentID].cADModel_hdl), &currentMaterial );
		}
		catch (isis::application_exception &e )
		{
			std::string TempError = "Function TestRetrievingCurrentMaterial(...) failed to retrieve a current(i.e. default) material for model: " + 
				(const std::string&)in_CADComponentData_map[in_AssemblyComponentID].name +
				", Component Instance ID: " + in_AssemblyComponentID + ", Creo Exception: " + e.what() ;
			throw isis::application_exception(TempError.c_str());
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// If the current material does not have the properties needed by FEA, then some of functions in this file 
	// will not work; and therefore, throw an exception.
	void TestIfCurrentMaterialHasNeededFEAProperties (	const std::string								&in_AssemblyComponentID, 
														std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map)
														throw (isis::application_exception)
	{
		std::string materialPropertyType;
		try
		{
			// Get current material
			ProMaterial currentMaterial;
			isis::isis_ProMaterialCurrentGet( static_cast<ProSolid>(in_CADComponentData_map[in_AssemblyComponentID].cADModel_hdl), &currentMaterial );	
			ProModelitem  currentMaterialModelItem;
		
			isis_ProModelitemByNameInit ( in_CADComponentData_map[in_AssemblyComponentID].cADModel_hdl, PRO_RP_MATERIAL, currentMaterial.matl_name, &currentMaterialModelItem );

			ProParamvalue materialValue;
			ProUnititem   materialUnit;

			// Density
			materialPropertyType = "Density";
			isis::isis_ProMaterialPropertyGet( &currentMaterialModelItem,		  PRO_MATPROP_MASS_DENSITY, &materialValue, &materialUnit );

			// Poisson' Ratio
			materialPropertyType = "Poisson' Ratio";
			isis::isis_ProMaterialPropertyGet( &currentMaterialModelItem,		  PRO_MATPROP_POISSON_RATIO, &materialValue, &materialUnit );

			// Young's Modulus
			materialPropertyType = "Young's Modulus";
			isis::isis_ProMaterialPropertyGet( &currentMaterialModelItem,		  PRO_MATPROP_YOUNG_MODULUS, &materialValue, &materialUnit );
		
			// Coeef. of Thermal Expansion
			materialPropertyType = "Coefficient of Thermal Expansion";
			isis::isis_ProMaterialPropertyGet( &currentMaterialModelItem,		  PRO_MATPROP_THERMAL_EXPANSION_COEFFICIENT, &materialValue, &materialUnit );

			// Stress-Strain Response e.g. Linear, Hyperelastic, Elastroplastic
			// Couldn't find the enum entry
		
			// Specific Heat Capicity
			materialPropertyType = "Specific Heat Capicity";
			isis::isis_ProMaterialPropertyGet( &currentMaterialModelItem, PRO_MATPROP_SPECIFIC_HEAT, &materialValue, &materialUnit );

			// Thermal Conductivity
			materialPropertyType = "Thermal Conductivity";
			isis::isis_ProMaterialPropertyGet( &currentMaterialModelItem, PRO_MATPROP_THERMAL_CONDUCTIVITY, &materialValue, &materialUnit );

			// Symmetry e.g. isotropic
			// Could not find the enum entry


		}
		catch (isis::application_exception &e )
		{
			std::string TempError = "Function: TestIfCurrentMaterialHasNeededFEAProperties(...). For the material property assigned to the Creo model, could not retrieve " + materialPropertyType +
				" material property. Model name: " + (const std::string&)in_CADComponentData_map[in_AssemblyComponentID].name +
				", Component Instance ID: " + in_AssemblyComponentID + ", Creo Exception: " + e.what() ;
			throw isis::application_exception(TempError.c_str());
		}

	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Return true if in_MaterialName material exists in in_Part
	// in_MaterialName length must be < 32 chars
	bool  MaterialExists( ProSolid in_Part, const std::string &in_MaterialName ) 
													throw (isis::application_exception)
	{
		std::string MaterialName = isis::ConvertToUpperCase(in_MaterialName);

		ProName * p_name;
		isis::isis_ProArrayAlloc(0, sizeof(ProName), 1, (ProArray*) &p_name);
		isis::isis_ProPartMaterialsGet(in_Part, &p_name);

		int numLines;

		isis::isis_ProArraySizeGet((ProArray)p_name, &numLines);

		char stringBuffer[PRO_NAME_SIZE];  // PRO_NAME_SIZE = 32
		for (int i=0; i < numLines; ++i)
		{
			ProWstringToString(stringBuffer, p_name[i]);
			//std::cout << std::endl << stringBuffer;
			if ( strcmp(MaterialName.c_str(), _strupr(stringBuffer ))  == 0  ) 
			{
				isis::isis_ProArrayFree((ProArray*)&p_name);
				return true; 
			}
		}

		return false;
	}

	//	Description:
	//		Create in_MaterialName 
	//
	//	Pre-Conditions:
	//		in_Part must be a model retrieved into memory.
	//		in_MaterialName length < 32 chars
	//
	//	Post-Conditions
	//		if in_MaterialName already exist
	//			return; //no action required
	//		else
	//			create in_MaterialName material
	void CreateMaterial( ProSolid in_part, const std::string &in_MaterialName ) 
													throw (isis::application_exception)
	{
		// Check if material exist
		if ( MaterialExists(in_part, in_MaterialName) )
		{
			return;
		}

		// Create new material
		ProMaterial newMaterial;
		ProName  materialName_wchar;
		ProStringToWstring(materialName_wchar, (char *)in_MaterialName.c_str() );
		isis::isis_ProMaterialCreate ( in_part, materialName_wchar, NULL, &newMaterial  );

	}

	//	Description:
	//		Set in_MaterialName material properties to be the same as the current material (i.e. material that
	//		is set to current in the Creo Model)
	//
	//	Pre-Conditions:
	//		in_Part must be a model retrieved into memory.
	//		in_MaterialName length < 32 chars
	//
	//	Post-Conditions
	//		if in_MaterialName is already the current material
	//			return; // no action required.
	//		else
	//			if in_MaterialName material does not exist
	//				throw isis::application_exception
	//			else
	//				set in_MaterialName properties to be equal to the current material properties
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	void SetMaterialPropertiesToBeTheSameAsCurrentMaterial(ProSolid in_part, const std::string &in_MaterialName ) 
													throw (isis::application_exception)
	{

		// Check if material exist
		if ( !MaterialExists(in_part, in_MaterialName) )
		{
			std::string TempError = "Function SetMaterialPropertiesToBeTheSameAsCurrentMaterial(...) was passed a material name that does not exist in the model (i.e. argument in_part).  in_MaterialName: " +
				in_MaterialName;
			throw isis::application_exception(TempError.c_str());
		}


		// Get current material
		ProMaterial currentMaterial;
		isis::isis_ProMaterialCurrentGet( in_part, &currentMaterial );
		ProModelitem  currentMaterialModelItem;
		isis_ProModelitemByNameInit ( in_part, PRO_RP_MATERIAL, currentMaterial.matl_name, &currentMaterialModelItem );

		// Check if in_MaterialName is the current material, if so, no action required.
		char stringBuffer[PRO_NAME_SIZE];  // PRO_NAME_SIZE = 32
		ProWstringToString(stringBuffer, currentMaterial.matl_name);
		std::string tempString = isis::ConvertToUpperCase(in_MaterialName);
		if ( strcmp(tempString.c_str(), _strupr(stringBuffer ))  == 0  )
		{
			// The current material is in_MaterialName. No action needed.
			return;
		}

		// Destination material
		ProMaterial destinationMaterial;
		ProName  materialName_wchar;
		ProStringToWstring(materialName_wchar, (char *)in_MaterialName.c_str() );
		ProModelitem  destinationMaterialModelItem;
		isis_ProModelitemByNameInit ( in_part, PRO_RP_MATERIAL, materialName_wchar, &destinationMaterialModelItem );

		// Copy the material properties from the current to destination material property
		ProParamvalue materialValue;
		ProUnititem   materialUnit;

		// Density
		isis::isis_ProMaterialPropertyGet( &currentMaterialModelItem,		  PRO_MATPROP_MASS_DENSITY, &materialValue, &materialUnit );
		isis::isis_ProMaterialPropertySet( &destinationMaterialModelItem,	  PRO_MATPROP_MASS_DENSITY, &materialValue, &materialUnit );

		// Poisson' Ratio
		isis::isis_ProMaterialPropertyGet( &currentMaterialModelItem,		  PRO_MATPROP_POISSON_RATIO, &materialValue, &materialUnit );
		isis::isis_ProMaterialPropertySet( &destinationMaterialModelItem,	  PRO_MATPROP_POISSON_RATIO, &materialValue, &materialUnit );

		// Young's Modulus
		isis::isis_ProMaterialPropertyGet( &currentMaterialModelItem,		  PRO_MATPROP_YOUNG_MODULUS, &materialValue, &materialUnit );
		isis::isis_ProMaterialPropertySet( &destinationMaterialModelItem,	  PRO_MATPROP_YOUNG_MODULUS, &materialValue, &materialUnit );
		
		// Coeef. of Thermal Expansion
		isis::isis_ProMaterialPropertyGet( &currentMaterialModelItem,		  PRO_MATPROP_THERMAL_EXPANSION_COEFFICIENT, &materialValue, &materialUnit );
		isis::isis_ProMaterialPropertySet( &destinationMaterialModelItem,	  PRO_MATPROP_THERMAL_EXPANSION_COEFFICIENT, &materialValue, &materialUnit );

		// Stress-Strain Response e.g. Linear, Hyperelastic, Elastroplastic
		// Couldn't find the enum entry
		
		// Specific Heat Capicity
		isis::isis_ProMaterialPropertyGet( &currentMaterialModelItem, PRO_MATPROP_SPECIFIC_HEAT, &materialValue, &materialUnit );
		isis::isis_ProMaterialPropertySet( &destinationMaterialModelItem,	  PRO_MATPROP_SPECIFIC_HEAT, &materialValue, &materialUnit );

		// Thermal Conductivity
		isis::isis_ProMaterialPropertyGet( &currentMaterialModelItem, PRO_MATPROP_THERMAL_CONDUCTIVITY, &materialValue, &materialUnit );
		isis::isis_ProMaterialPropertySet( &destinationMaterialModelItem,	  PRO_MATPROP_THERMAL_CONDUCTIVITY, &materialValue, &materialUnit );

		// Symmetry e.g. isotropic
		// Could not find the enum entry

		try 
		{  // Don't fail if this doesnt't exist.
		// Initial Bend Y-Factor
			isis::isis_ProMaterialPropertyGet( &currentMaterialModelItem, PRO_MATPROP_INITIAL_BEND_Y_FACTOR, &materialValue, &materialUnit );
			isis::isis_ProMaterialPropertySet( &destinationMaterialModelItem,	  PRO_MATPROP_INITIAL_BEND_Y_FACTOR, &materialValue, &materialUnit );
		}
		catch (...)
		{
		}

		// Bend Table, skip this 
		//isis::isis_ProMaterialPropertyGet( &currentMaterialModelItem, PRO_MATPROP_BEND_TABLE, &materialValue, &materialUnit );
		//isis::isis_ProMaterialPropertySet( &destinationMaterialModelItem,	  PRO_MATPROP_BEND_TABLE, &materialValue, &materialUnit );

		// Cross Hatching
		// Could not find the enum entry

	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	void SetMaterial_to_CurrentMaterial( ProSolid in_part, const std::string &in_MaterialName ) 
													throw (isis::application_exception)
	{
		// Check if material exists
		if ( !MaterialExists(in_part, in_MaterialName) )
		{
			std::string TempError = "Function SetMaterial_to_CurrentMaterial(...) was passed a material name that does not exist in the model (i.e. argument in_part).  in_MaterialName: " +
				in_MaterialName;
			throw isis::application_exception(TempError.c_str());
		}

		ProMaterial material;
		ProName  materialName_wchar;
		ProStringToWstring(material.matl_name, (char *)in_MaterialName.c_str() );
		material.part = in_part;

		// Set material as the current material
		ProMaterialCurrentSet(&material);

	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	void AddTokenMaterialIdentifier(const std::string								&in_AssemblyComponentID, 
									std::map<std::string, isis::CADComponentData>	&in_out_CADComponentData_map, 
									const std::string								&in_MaterialName,
									double											in_MaterialTokenNumber ) 
	{
		ProSolid part = static_cast<ProSolid>(in_out_CADComponentData_map[in_AssemblyComponentID].cADModel_hdl);

		// Get current material
		ProMaterial material;
		isis::isis_ProMaterialCurrentGet( part, &material );
		ProModelitem  materialModelItem;
		isis_ProModelitemByNameInit ( part, PRO_RP_MATERIAL, material.matl_name, &materialModelItem );

		ProParamvalue materialValue;
		ProUnititem   materialUnit;

		// Get Poisson's ration
		isis::isis_ProMaterialPropertyGet( &materialModelItem, PRO_MATPROP_POISSON_RATIO, &materialValue, &materialUnit );

		// Set the map contents
		in_out_CADComponentData_map[in_AssemblyComponentID].analysisTempMaterialDefinition.poissonsRatio = materialValue.value.d_val;

		char buffer[64];
		sprintf(buffer, "%6.5lf", in_MaterialTokenNumber ); 

		in_out_CADComponentData_map[in_AssemblyComponentID].analysisTempMaterialDefinition.tokenPoissonsRatio = std::string(buffer);
		in_out_CADComponentData_map[in_AssemblyComponentID].analysisTempMaterialDefinition.name = in_MaterialName;

		in_out_CADComponentData_map[in_AssemblyComponentID].analysisTempMaterialDefined = true;


		//std::cout << std::endl << "in_MaterialTokenNumber: " << in_MaterialTokenNumber;
		// Set the token value in the model's material
		materialValue.value.d_val = in_MaterialTokenNumber;
		isis::isis_ProMaterialPropertySet( &materialModelItem, PRO_MATPROP_POISSON_RATIO, &materialValue, &materialUnit );
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void CreateUniquelyNamedMaterials_impl( 
					const std::string								&in_AssemblyComponentID, 
					std::map<std::string, isis::CADComponentData>	&in_out_CADComponentData_map, // in_out_CADComponentData_map modified by AddTokenMaterialIdentifier
					double											&in_out_MaterialTokenNumber )
																throw (isis::application_exception)
	{

		

		int materialIndex = 0;	

	    // Warining - in_out_MaterialTokenNumber should be of the form -0.11111, -0.11112, -0.11113...
		// Changin the precision of in_out_MaterialTokenNumber will cause problems because this double
		// is converted to a string in the function AddTokenMaterialIdentifier(..) with the following code:
		//   sprintf(buffer, "%6.5lf", in_MaterialTokenNumber );  
		//	The precission (i.e. %6.5lf) is critical to using the sting version as a key into a map.
		double decrement = -.00001;
		
		char buffer[64];
		for ( std::list<std::string>::const_iterator i (in_out_CADComponentData_map[in_AssemblyComponentID].children.begin());
		  i != in_out_CADComponentData_map[in_AssemblyComponentID].children.end();
		  ++i )
		{
			++materialIndex;
			std::string materialName = "ZZ_FEA_MAT_" + std::string(itoa(materialIndex,buffer,10)) + "_ZZ";

			isis_LOG(lg, isis_FILE, isis_INFO) << "";
			isis_LOG(lg, isis_FILE, isis_INFO)  << "CreateUniquelyNamedMaterials_impl: " << 
				isis_EOL << "   Material:     "  << materialName <<
				isis_EOL << "   Model Name:   " << in_out_CADComponentData_map[*i].name <<
				isis_EOL << "   Model Type:   " << isis::CADMdlType_string(in_out_CADComponentData_map[*i].modelType) << 
				isis_EOL << "   cADModel_hdl: " <<    (const void*)in_out_CADComponentData_map[*i].cADModel_hdl;

			if ( materialName.size() >= PRO_NAME_SIZE )
			{
				std::string TempError = "Function CreateUniquelyNamedMaterials_impl(...) created a material name that exceeds 31 characters.  in_MaterialName: " + materialName;
				throw isis::application_exception(TempError.c_str());
			}

			//std::cout << std::endl << "materialName: " << materialName;

			//if ( in_out_CADComponentData_map[*i].modelType ==  PRO_MDL_PART) 
			if ( in_out_CADComponentData_map[*i].modelType ==  CAD_MDL_PART) 

			{
				TestRetrievingCurrentMaterial (	*i,	in_out_CADComponentData_map ); 
				TestIfCurrentMaterialHasNeededFEAProperties(*i,	in_out_CADComponentData_map );
				CreateMaterial(static_cast<ProSolid>(in_out_CADComponentData_map[*i].cADModel_hdl), materialName );
				SetMaterialPropertiesToBeTheSameAsCurrentMaterial(static_cast<ProSolid>(in_out_CADComponentData_map[*i].cADModel_hdl), materialName );
				SetMaterial_to_CurrentMaterial( static_cast<ProSolid>(in_out_CADComponentData_map[*i].cADModel_hdl), materialName );
				AddTokenMaterialIdentifier( *i, in_out_CADComponentData_map, materialName, in_out_MaterialTokenNumber );
				
				in_out_MaterialTokenNumber += decrement;
		

				// add the following temporarily to save the material changes for debuggin purposes
				// WARNING - if a save occurs here, it will only work in an empty workig directory.
				//           this is because subsequent runs will pick-up the models with the wrong material
				//           names.
				// isis::isis_ProMdlSave(in_out_CADComponentData_map[*i].modelHandle );
			}
			//if ( in_out_CADComponentData_map[*i].modelType == PRO_MDL_ASSEMBLY  && in_out_CADComponentData_map[*i].children.size() > 0) // Note - an existing assmbly would have no children. 
			if ( in_out_CADComponentData_map[*i].modelType == CAD_MDL_ASSEMBLY  && in_out_CADComponentData_map[*i].children.size() > 0) // Note - an existing assmbly would have no children.
			{
				CreateUniquelyNamedMaterials_impl( *i,in_out_CADComponentData_map, in_out_MaterialTokenNumber);
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	void CreateUniquelyNamedMaterials( 
					const std::string								&in_AssemblyComponentID, 
					std::map<std::string, isis::CADComponentData>	&in_out_CADComponentData_map) // in_out_CADComponentData_map modified by AddTokenMaterialIdentifier
																	throw (isis::application_exception)

	{
		// Since Poisson's ratio can be in the range of -1 to .5, starting with -.11111 and decrementing by .00001 will
		// result in about 80,000 values before -.1 is reached.  Since our assemblies will be at most 5,000 parts, and the
		// analysis assemblies will be only a few parts (probably less than 30), this approach should meet our needs.
		// Also, negative Poisson's ratios are rare; and therefore, collisions are unlikely.
		double materialTokenNumber = - .11111;
		CreateUniquelyNamedMaterials_impl(in_AssemblyComponentID, in_out_CADComponentData_map, materialTokenNumber );
	}


///////////////////////////////////////////////////////////////////////////////////////
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

	///////////////////////////////////////////////////////////////////////////////////////
	void RetrieveCreoMaterialsNames( const std::vector<std::string>						&in_ListOfComponentInstanceIDs,
									 std::map<std::string, isis::CADComponentData>		&in_CADComponentData_map,
									 std::map<std::string, std::string>					&out_ComponentID_to_MaterialName_map ) 
																						throw (isis::application_exception)
	{
		bool errorOccrred = false;
		
		std::stringstream errorNotFoundMaterials;
		errorNotFoundMaterials << "Material(s) not set in Creo Model(s):";

		ProMaterial  material;
		char stringBuffer[PRO_NAME_SIZE];
		std::string materialName;

		for each ( std::string i in in_ListOfComponentInstanceIDs)
		{
			if ( in_CADComponentData_map[i].modelType == PRO_PART )
			{
				try
				{
					isis::isis_ProMaterialCurrentGet( static_cast<ProSolid>(in_CADComponentData_map[i].cADModel_hdl), &material );

					char stringBuffer[PRO_NAME_SIZE];  // PRO_NAME_SIZE = 32
					materialName = ProWstringToString( stringBuffer, material.matl_name );	

					// A component ID should not occurr more than once
					if ( out_ComponentID_to_MaterialName_map.find(i) != out_ComponentID_to_MaterialName_map.end() )
					{
						std::stringstream errorString;
						errorString <<
							"Function: RetrieveCreoMaterialsNames, A ComponentInstanceID (" <<   in_CADComponentData_map[i].componentID << ") appeared more than once in in_ListOfComponentInstanceIDs" << std::endl <<
						"in_ListOfComponentInstanceIDs: "  << std::endl;
						for each ( std::string j in in_ListOfComponentInstanceIDs ) 
										errorString << "ComponentInstanceID: " << i << "ComponentName: " << in_CADComponentData_map[i].name;

						throw isis::application_exception(errorString.str());	
					}
					out_ComponentID_to_MaterialName_map[i] = materialName;

				}
				catch ( isis::application_exception ex )
				{
					errorOccrred = true;
					errorNotFoundMaterials << std::endl <<
					"  Component Instance ID: " <<  in_CADComponentData_map[i].componentID << std::endl << 
					"  Component Name: " <<  in_CADComponentData_map[i].name <<  std::endl << 
					"  Component Type: " <<  CADMdlType_string(in_CADComponentData_map[i].modelType)  <<  std::endl << 
					"  Exception: " <<  ex.what() << std::endl;
				}
			}
		}

		if (errorOccrred) throw isis::application_exception(errorNotFoundMaterials.str().c_str());

	}

} // END namespace isis
