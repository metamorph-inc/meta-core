#include "MaterialProperties.h"
#include <CommonUtilities.h>
#include <string>
#include <stdlib.h>
#include <locale>
#include <iostream>
#include <ostream>
#include <sstream>

#include <boost\filesystem.hpp>
#include <boost\property_tree\ptree.hpp>
#include <boost\property_tree\json_parser.hpp>
#include <boost\foreach.hpp>
#include <boost\algorithm\string.hpp>
#include <sstream>

#include <fstream>
#include <iterator>
#include <stdlib.h>
#include "LoggerBoost.h"
#include "CommonDefinitions.h"


namespace isis
{

	const double PASCAL_TO_MEGA_PASCAL_CONVERSION_FACTOR = 1.0 / 1000000.0;
	const double CONVERSION_FACTOR_ONE = 1.0;
	const double M_CUBED_TO_MM_CUBED = 1000.0 * 1000.0 * 1000.0;
	const double M_CUBED_TO_MM_CUBED_RECIPROCAL = 1.0 / M_CUBED_TO_MM_CUBED;
	const double M_TO_MM_RECIPROCAL = 1.0  / 1000.0;
/*
	std::string ConvertToUpperCase(const std::string &in_String)
	{
		std::string temp_string(in_String);
		std::locale loc;
		for (std::string::iterator p = temp_string.begin(); temp_string.end() != p; ++p)    *p = toupper(*p, loc);
		return temp_string;
	}

*/


void stream_AnalysisMaterialProperties( const AnalysisMaterialProperties &in_CADAnalyses, std::ostream &out_Stream )																	
{
	out_Stream << std::endl << "Material Name: " << in_CADAnalyses.materialName;
	out_Stream << std::endl << "Material Type: " << isis::MaterialType_string(in_CADAnalyses.materialType);
	
	out_Stream << std::endl << "   ModulusOfElasticity";
	out_Stream << std::endl << "      Defined: " << in_CADAnalyses.modulusOfElasticityDefined;
	out_Stream << std::endl << "      Value: "   << in_CADAnalyses.modulusOfElasticity;

	out_Stream << std::endl << "   PoissonsRatio";
	out_Stream << std::endl << "      Defined: " << in_CADAnalyses.poissonsRatioDefined;
	out_Stream << std::endl << "      Value: "   << in_CADAnalyses.poissonsRatio;

	out_Stream << std::endl << "   TensileYieldStrength";
	out_Stream << std::endl << "      Defined: " << in_CADAnalyses.tensileYieldStrengthDefined;
	out_Stream << std::endl << "      Value: "   << in_CADAnalyses.tensileYieldStrength;

	out_Stream << std::endl << "   TensileUltimateStrength";
	out_Stream << std::endl << "      Defined: " << in_CADAnalyses.tensileUltimateStrengthDefined;
	out_Stream << std::endl << "      Value: "   << in_CADAnalyses.tensileUltimateStrength;

	out_Stream << std::endl << "   ShearStrength";
	out_Stream << std::endl << "      Defined: " << in_CADAnalyses.shearStrengthDefined;
	out_Stream << std::endl << "      Value: "   << in_CADAnalyses.shearStrength;

	out_Stream << std::endl << "   BearingYieldStrength";
	out_Stream << std::endl << "      Defined: " << in_CADAnalyses.bearingYieldStrengthDefined;
	out_Stream << std::endl << "      Value: "   << in_CADAnalyses.bearingYieldStrength;

	out_Stream << std::endl << "   BearingUltimateStrength";
	out_Stream << std::endl << "      Defined: " << in_CADAnalyses.bearingUltimateStrengthDefined;
	out_Stream << std::endl << "      Value: "   << in_CADAnalyses.bearingUltimateStrength;

	out_Stream << std::endl << "   FatigueStrength";
	out_Stream << std::endl << "      Defined: " << in_CADAnalyses.fatigueStrengthDefined;
	out_Stream << std::endl << "      Value: "   << in_CADAnalyses.fatigueStrength;

	out_Stream << std::endl << "   fatigueNumberOfCycles";
	out_Stream << std::endl << "      Defined: " << in_CADAnalyses.fatigueNumberOfCyclesDefined;
	out_Stream << std::endl << "      Value: "   << in_CADAnalyses.fatigueNumberOfCycles;

};

void stream_AnalysisMaterialProperties_Allowables( const AnalysisMaterialProperties_Allowables	&in_AnalysisMaterialProperties, std::ostream &out_Stream )																	
{
	out_Stream << std::endl << "Material Name: " << in_AnalysisMaterialProperties.materialName;
	out_Stream << std::endl << "Material Type: " << isis::MaterialType_string(in_AnalysisMaterialProperties.materialType);

	out_Stream << std::endl << "   TensileStrength";
	out_Stream << std::endl << "      Value: "   << in_AnalysisMaterialProperties.tensileStrength;

	out_Stream << std::endl << "   ShearStrength";
	out_Stream << std::endl << "      Value: "   << in_AnalysisMaterialProperties.shearStrength;

	out_Stream << std::endl << "   BearingStrength";
	out_Stream << std::endl << "      Value: "   << in_AnalysisMaterialProperties.bearingStrength;

	out_Stream << std::endl << "   NumberOfCycles";
	out_Stream << std::endl << "      Value: "   << in_AnalysisMaterialProperties.numberOfCycles;

};

///////////////////////////////////////////////////////////////////////////////////////////////
bool	StressValueComputed(	bool	in_Value_1_Defined,
								double	in_Value_1,	
								bool	in_Value_2_Defined,
								double	in_Value_2,
								double	in_ReferenceValue,
								double  &out_ComputedValue )
{

		if ( in_Value_1_Defined && in_Value_2_Defined  )
		{
			out_ComputedValue =  (in_ReferenceValue / in_Value_1 ) * in_Value_2;
			//std::cout << std::endl << "BEGIN StressValueComputed";
			//std::cout << std::endl << "  in_Value_1_Defined: "	<< in_Value_1_Defined;
			//std::cout << std::endl << "  in_Value_1: "			<< in_Value_1;
			//std::cout << std::endl << "  in_Value_2_Defined: "	<< in_Value_1_Defined;
			//std::cout << std::endl << "  in_Value_2: "			<< in_Value_2;
			//std::cout << std::endl << "  in_ReferenceValue:  "	<< in_ReferenceValue;
			//std::cout << std::endl << "  out_ComputedValue:  "	<< out_ComputedValue;
			//std::cout << std::endl << "END StressValueComputed";
			return true;
		}
		else
		{
			return false;
		} 
}
/////////////////////////////////////////////////////////////////////////////////////////////////

// General Rules:
//	1)	If in_NumberOfCycles == 0 assume infinite number of cycles
//	2)	Consider 500,000,000 (5M) cycles to be infinite cycles 
//	3)  Consider less than 1,000 cycles to be equivalent to 1 cycle for determining the allowable stress levels. 
//	3)	Aluminum must have in_AnalysisMaterialProperties.fatigueStrength and fatigueNumberOfCycles set.
//	4)	For Steel, if in_AnalysisMaterialProperties.fatigueStrength is not set, assume that it is 
//		50% of in_AnalysisMaterialProperties.tensileUltimateStrength
//	5)	TBD For cycle values between 1,000 and 500,000,000, we need a graph of X-Number Cycles, and Y-Stress.  These
//		are usually log base 10 graphs. 
//		
//	Material  		Fatigue		Number 		Tensile							Bearing							Shear
//					Given		Cycles		Strength						Strength						Strength
//	-----------		--------	-------		--------------					-----------						---------------
//	Steel/Aluminum	Yes			N < 1000	tensileYieldStrength			bearingYieldStrength			shearStrength
//	Stee/Aluminum	Yes			1000 < N 	fatigueStrength					SF * bearingUltimateStrength	SF * shearStrength
//	Steel			No			1000 < N 	.5 * tensileUltimateStrength	SF  * bearingYieldStrength		SF  * shearStrength
//	Aluminum		No			(Not Allowed, throw exception)
//
// SF - Scale Factor = fatigueStrength / tensileUltimateStrength; 

void ComputeAllowableStressLevels(	int										in_NumberOfCycles,
									const AnalysisMaterialProperties		&in_AnalysisMaterialProperties,
								    AnalysisMaterialProperties_Allowables	&out_AnalysisMaterialProperties )
																				throw (isis::application_exception)
{
	///////////////////////////////////////////////
	// zero out values
	///////////////////////////////////////////////
	out_AnalysisMaterialProperties.materialName = "";
	out_AnalysisMaterialProperties.tensileStrength	= 0;
	out_AnalysisMaterialProperties.bearingStrength	= 0;
	out_AnalysisMaterialProperties.shearStrength	= 0;
	out_AnalysisMaterialProperties.numberOfCycles	= 0;

	///////////////////////////////////////////////
	// Only Aluminum and Steel Currently Supported
	///////////////////////////////////////////////
	if ( ( in_AnalysisMaterialProperties.materialType != MATERIAL_ALUMINUM ) &&
		 ( in_AnalysisMaterialProperties.materialType != MATERIAL_STEEL ) )
																				
	{
		// zzz fix this to use the common functions
		std::string tempMaterialType = "Unknown";
		if (in_AnalysisMaterialProperties.materialType == MATERIAL_CERAMIC )tempMaterialType = "Ceramic";
		if (in_AnalysisMaterialProperties.materialType == MATERIAL_PLASTIC )tempMaterialType = "Plastic";

		std::string TempError = "Function ComputeAllowableStressLevels(...) for material " + in_AnalysisMaterialProperties.materialName + 
		" was passed: " + tempMaterialType + 
		".  Currently only Steel and Aluminum are supported. Plastic and Ceramic will be supported in future releases.";   
		throw isis::application_exception(TempError.c_str());
	}

	///////////////////////////////////////////////////////
	// Fatigue strength must be given defined for Aluminum
	///////////////////////////////////////////////////////
	if ( ( in_AnalysisMaterialProperties.materialType == MATERIAL_ALUMINUM ) &&
		 ( !in_AnalysisMaterialProperties.fatigueStrengthDefined ) )
	{
		std::string TempError = "Function ComputeAllowableStressLevels(...) for material " + in_AnalysisMaterialProperties.materialName + 
		" does not have a fatigue strength defined.  Aluminum must have fatigue strength defined.";   
		throw isis::application_exception(TempError.c_str());		
	}

	////////////////////////////////////////
	// Tensile yield strength must be given
	////////////////////////////////////////
	if ( !in_AnalysisMaterialProperties.tensileYieldStrengthDefined )
	{
		std::string TempError = "Function ComputeAllowableStressLevels(...) for material " + in_AnalysisMaterialProperties.materialName + 
		" does not have a tensile yield strength defined.  All material must have tensile yield strength defined.";   
		throw isis::application_exception(TempError.c_str());		
	}

	////////////////////////////////////////
	// Tensile Ultimate strength must be given
	////////////////////////////////////////
	if ( !in_AnalysisMaterialProperties.tensileUltimateStrengthDefined )
	{
		std::string TempError = "Function ComputeAllowableStressLevels(...) for material " + in_AnalysisMaterialProperties.materialName + 
		" does not have a tensile ultimate strength defined.  All material must have tensile ultimate strength defined.";   
		throw isis::application_exception(TempError.c_str());		
	}


	//////////////////////////////////////////////
	// Shear Strength must be given for Aluminum
	/////////////////////////////////////////////
	if ( !in_AnalysisMaterialProperties.shearStrengthDefined &&  in_AnalysisMaterialProperties.materialType == MATERIAL_ALUMINUM  )
	{
		std::string TempError = "Function ComputeAllowableStressLevels(...) for material " + in_AnalysisMaterialProperties.materialName + 
		" does not have a shear strength defined.  Aluminum must have shear strength defined.";   
		throw isis::application_exception(TempError.c_str());		
	}

	////////////////////////////////////////
	// Set Material Name and Type
	////////////////////////////////////////
	out_AnalysisMaterialProperties.materialName = in_AnalysisMaterialProperties.materialName;
	out_AnalysisMaterialProperties.materialType = in_AnalysisMaterialProperties.materialType;

	////////////////////////////////////////
	// Set Number of cycles
	////////////////////////////////////////
	out_AnalysisMaterialProperties.numberOfCycles = in_NumberOfCycles;

	////////////////////////////////////////
	// Compute tempShearStrength
	////////////////////////////////////////
	double tempShearStrength;
	if ( in_AnalysisMaterialProperties.shearStrengthDefined )
	{
		tempShearStrength = in_AnalysisMaterialProperties.shearStrength;
	}
	else 
	{
		// Case for steel when shear strength is not defined, use rule of thumb
		tempShearStrength = .58 * in_AnalysisMaterialProperties.tensileYieldStrength;
	}

	////////////////////////////////////////
	// No Fatigue Case
	////////////////////////////////////////
	if ( ( in_NumberOfCycles != 0 ) && ( in_NumberOfCycles < 1000 ) )
	{
		out_AnalysisMaterialProperties.tensileStrength = in_AnalysisMaterialProperties.tensileYieldStrength;
		out_AnalysisMaterialProperties.shearStrength   = tempShearStrength;
		
		if (in_AnalysisMaterialProperties.bearingYieldStrengthDefined )
			out_AnalysisMaterialProperties.bearingStrength = in_AnalysisMaterialProperties.bearingYieldStrength;
		else
			out_AnalysisMaterialProperties.bearingStrength = in_AnalysisMaterialProperties.tensileYieldStrength;
		
	}
	else
	{
		/////////////////////////////////////////
		// Fatigue Case - Steel and Aluminum
		////////////////////////////////////////

		// Note fatigue strength would be defined for aluminum as checked above, maybe not for steel.
		double tempFatigueStrength;

		if ( in_AnalysisMaterialProperties.fatigueStrengthDefined )
			tempFatigueStrength = in_AnalysisMaterialProperties.fatigueStrength;
		else
			tempFatigueStrength = .5 * in_AnalysisMaterialProperties.tensileUltimateStrength;

		// Tensile
		out_AnalysisMaterialProperties.tensileStrength = tempFatigueStrength;		

		// Bearing
		bool stressComputed = false;
		stressComputed = StressValueComputed(	
							in_AnalysisMaterialProperties.tensileUltimateStrengthDefined,
							in_AnalysisMaterialProperties.tensileUltimateStrength,	
							in_AnalysisMaterialProperties.bearingUltimateStrengthDefined,
							in_AnalysisMaterialProperties.bearingUltimateStrength,
							tempFatigueStrength,
							out_AnalysisMaterialProperties.bearingStrength );

		
		if ( !stressComputed )
		   stressComputed = StressValueComputed(	
							in_AnalysisMaterialProperties.tensileYieldStrengthDefined,
							in_AnalysisMaterialProperties.tensileYieldStrength,	
							in_AnalysisMaterialProperties.bearingYieldStrengthDefined,
							in_AnalysisMaterialProperties.bearingYieldStrength,
							tempFatigueStrength,
							out_AnalysisMaterialProperties.bearingStrength );

		if ( !stressComputed )
			out_AnalysisMaterialProperties.bearingStrength = tempFatigueStrength;

		// Shear
		stressComputed = StressValueComputed(	
							in_AnalysisMaterialProperties.tensileUltimateStrengthDefined,
							in_AnalysisMaterialProperties.tensileUltimateStrength,	
							true,
							tempShearStrength, 
							tempFatigueStrength,
							out_AnalysisMaterialProperties.shearStrength );

		if ( !stressComputed )
			stressComputed = StressValueComputed(	
							in_AnalysisMaterialProperties.tensileYieldStrengthDefined,
							in_AnalysisMaterialProperties.tensileYieldStrength,	
							true,
							tempShearStrength, 
							tempFatigueStrength,
							out_AnalysisMaterialProperties.shearStrength );
		if ( !stressComputed )
			out_AnalysisMaterialProperties.shearStrength =  .5 * in_AnalysisMaterialProperties.shearStrength;

	}

	///////////////////////////////////////////////
	//	At this point, the strength properties 
	//	should be set.  It would be a programming
	//	error if this was not the case. Throw
	//	and exception if the strength properties 
	//	are not set.
	///////////////////////////////////////////////

	if ( out_AnalysisMaterialProperties.tensileStrength	== 0 ||
		 out_AnalysisMaterialProperties.bearingStrength	== 0 ||
	     out_AnalysisMaterialProperties.shearStrength	== 0 )
	{
		std::ostringstream tensileStrength_str;
		std::ostringstream bearingStrength_str;
		std::ostringstream shearStrength_str;

		tensileStrength_str << out_AnalysisMaterialProperties.tensileStrength;
		bearingStrength_str << out_AnalysisMaterialProperties.bearingStrength;
		shearStrength_str   << out_AnalysisMaterialProperties.shearStrength;

		std::string TempError = "Function ComputeAllowableStressLevels(...) for material " + in_AnalysisMaterialProperties.materialName + 
		" failed to compute the all of the strength material properties.  Computed properties are: Tensile Strength: " +
		tensileStrength_str.str() + ", Bearing Strength: " + bearingStrength_str.str() + ", Shear Strength: " + shearStrength_str.str() + ".";   
		throw isis::application_exception(TempError.c_str());		
	}
}

enum e_UnitType
{
	UNIT_MPA,
	UNIT_GPA,
	UNIT_NA
};


e_UnitType RetrieveUnitType(	const std::string &in_MaterialName,
								const std::string &in_Units )
																	throw (isis::application_exception)
{															
	std::string units = isis::ConvertToUpperCase(in_Units);	

	if ( strcmp(units.c_str(), "MPA") == 0  )		
		return UNIT_MPA;
	else 
		if ( strcmp(units.c_str(), "GPA")  == 0  ) 
			return UNIT_GPA;
		else 
			if ( strcmp(units.c_str(), "N/A")  == 0  ) 
				return UNIT_NA;

	// No known type found.
	std::string TempError = "Function RetrieveUnitType(...) for material " + in_MaterialName + 
		" was passed: " + in_Units + 
		".  Acceptable types (case insensitive) are: MPA, GPA, and N/A.";   
	throw isis::application_exception(TempError.c_str());
}
////////////////////////////////////////////////////////////////////////////////////////////////////
// Description:
//		This function converts string representations of material properties
//		(i.e. in_MaterialMetrics_map[in_MaterialPropertyName] ) to doubles and stores 
//		the results in out_Property.
//		
//		Note -	All units stored in out_Property will be in MPa except for unit-less values
//				such as PoissonsRatio.
//		
// Pre-Conditions:
//		if in_MaterialPropertyName[in_MaterialPropertyName] exists
//			in_MaterialPropertyName[in_MaterialPropertyName].units must be MPa, GPa, or N/A
//
// Post-Conditions:
//		if in_MaterialPropertyName[in_MaterialPropertyName] not exists
//			return (no action taken)
//		else
//			if in_MaterialPropertyName[in_MaterialPropertyName].units != MPa, GPa, or N/A
//				throw isis::application_exception
//			else
//				out_Property =  in_MaterialPropertyName[in_MaterialPropertyName].value
//				out_PropertyDefnied = true
//				if in_MaterialPropertyName[in_MaterialPropertyName].units == GPa
//					scale out_Property to be in MPA
//				
void SetSingleAnalysisMaterialProperty(  
					const std::string						&in_MaterialPropertyName,
					const std::map<std::string, MaterialMetrics> &in_MaterialMetrics_map,
					double									&out_Property,
					bool									&out_PropertyDefnied) 
																throw (isis::application_exception)
{
	std::map<std::string, MaterialMetrics>::const_iterator itr = in_MaterialMetrics_map.find(in_MaterialPropertyName);

	if (itr != in_MaterialMetrics_map.end() )
	{
		// Note Poissoin's ration could be zero.  Since we will analyze metals, this should not be a problem.
		if ( itr->second.value == 0 )  
		{
			out_PropertyDefnied = false;
		}
		else
		{
			out_Property = itr->second.value;
			out_PropertyDefnied = true;
			if( RetrieveUnitType(in_MaterialPropertyName,itr->second.units ) == UNIT_GPA ) out_Property *= 1000;
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////
// This funcdtion will convert integer values.  The only integer value is FatigueNumberOfCycles
// which has no units.
void SetSingleAnalysisMaterialProperty(  
					const std::string						&in_MaterialPropertyName,
					const std::map<std::string, MaterialMetrics> &in_MaterialMetrics_map,
					long									&out_Property,
					bool									&out_PropertyDefnied)
{
	std::map<std::string, MaterialMetrics>::const_iterator itr = in_MaterialMetrics_map.find(in_MaterialPropertyName);

	if (itr != in_MaterialMetrics_map.end() )
	{
		//out_Property = strtol(itr->second.value.c_str(), NULL,10 );
		out_Property = itr->second.value;

		out_PropertyDefnied = true;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void PopulateAnalysisMaterialStruct( const Material				 &in_Material, 
									 AnalysisMaterialProperties  &out_AnalysisMaterialProperties_struct )
																		throw (isis::application_exception)
{
	out_AnalysisMaterialProperties_struct.materialType = in_Material.materialType;
	out_AnalysisMaterialProperties_struct.materialName = in_Material.materialName;

	//std::map<std::string, MaterialMetrics> i;

	SetSingleAnalysisMaterialProperty(	"ModulusOfElasticity", 
										in_Material.materialMetrics_map, 
										out_AnalysisMaterialProperties_struct.modulusOfElasticity,
										out_AnalysisMaterialProperties_struct.modulusOfElasticityDefined);

	SetSingleAnalysisMaterialProperty(	"PoissonsRatio", 
										in_Material.materialMetrics_map, 
										out_AnalysisMaterialProperties_struct.poissonsRatio,
										out_AnalysisMaterialProperties_struct.poissonsRatioDefined);

	SetSingleAnalysisMaterialProperty(	"TensileYieldStrength", 
										in_Material.materialMetrics_map, 
										out_AnalysisMaterialProperties_struct.tensileYieldStrength,
										out_AnalysisMaterialProperties_struct.tensileYieldStrengthDefined);

	SetSingleAnalysisMaterialProperty(	"TensileUltimateStrength", 
										in_Material.materialMetrics_map, 
										out_AnalysisMaterialProperties_struct.tensileUltimateStrength,
										out_AnalysisMaterialProperties_struct.tensileUltimateStrengthDefined);

	SetSingleAnalysisMaterialProperty(	"ShearStrength", 
										in_Material.materialMetrics_map, 
										out_AnalysisMaterialProperties_struct.shearStrength,
										out_AnalysisMaterialProperties_struct.shearStrengthDefined);

	SetSingleAnalysisMaterialProperty(	"BearingYieldStrength", 
										in_Material.materialMetrics_map, 
										out_AnalysisMaterialProperties_struct.bearingYieldStrength,
										out_AnalysisMaterialProperties_struct.bearingYieldStrengthDefined);

	SetSingleAnalysisMaterialProperty(	"BearingUltimateStrength", 
										in_Material.materialMetrics_map, 
										out_AnalysisMaterialProperties_struct.bearingUltimateStrength,
										out_AnalysisMaterialProperties_struct.bearingUltimateStrengthDefined);

	SetSingleAnalysisMaterialProperty(	"FatigueStrength", 
										in_Material.materialMetrics_map, 
										out_AnalysisMaterialProperties_struct.fatigueStrength,
										out_AnalysisMaterialProperties_struct.fatigueStrengthDefined);

	SetSingleAnalysisMaterialProperty(	"FatigueNumberOfCycles", 
										in_Material.materialMetrics_map, 
										out_AnalysisMaterialProperties_struct.fatigueNumberOfCycles,
										out_AnalysisMaterialProperties_struct.fatigueNumberOfCyclesDefined);

} 

	////////////////////////////////////////////////////////////////////////////////////////////////////

	// in_UnitsConversionFactor should equal to 1.0 if no conversion.
	bool MaterialPropertyFound_RetrieveValue( 
						const boost::property_tree::ptree::value_type	&in_Material_pt,
						const std::string	    &in_MaterialName,
						const std::string		&in_MaterialLibPropertyName, // e.g. strength_tensile_ultimate
						const					std::string in_MaterialLibRequiredUnit,  // e.g. Pa
						double					in_UnitsConversionFactor,  // e.g. PASCAL_TO_MEGA_PASCAL_CONVERSION_FACTOR
						const std::string		&in_ConvertToUnit,
						double					&out_Value)
	{

		//std::clog << std::endl << "   " << in_Material_pt.first.data();

		bool materialFound = false;

		// Case insensitive check.
		if ( isis::ConvertToUpperCase(std::string(in_Material_pt.first.data())) == isis::ConvertToUpperCase(in_MaterialLibPropertyName) )
		{
			std::string unit;
			if ( in_MaterialLibRequiredUnit.size() == 0 )
			{
				unit = "None";
			}
			else
			{
				try
				{
					unit = in_Material_pt.second.get<std::string>("unit");
				}
				catch (...)  // Could not get the unit, but units are required.  Assume the property is not defined
				{
					

					isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "Material units not defined for: " << isis_EOL <<
											  "   Material Name:     " << in_MaterialName << isis_EOL <<
											  "   Material Property: " << in_MaterialLibPropertyName << isis_EOL <<
											  "   Note:              Without units, this material property cannot be used.";
					return false;
				}
			}

			materialFound = true;
			if ( in_MaterialLibRequiredUnit.size() > 0 )
			{
				// Must verify units.  
				if ( isis::ConvertToUpperCase(in_MaterialLibRequiredUnit) != isis::ConvertToUpperCase(unit) )
				{
					std::stringstream errorString;
								errorString <<
										"Material:          " <<  in_MaterialName << std::endl <<
										"Material Property: " <<  in_MaterialLibPropertyName << std::endl <<
										"Material Unit:	    " <<  unit  << endl <<
										"Required Unit:     " << in_MaterialLibRequiredUnit << std::endl <<
										"The units in the material library are incompatible with the required units.";
					throw isis::application_exception(errorString.str());
				}
			}

			std::string value_string =  in_Material_pt.second.get<std::string>("value");

			if ( isis::ConvertToUpperCase(value_string) == "NONE") return false;

			out_Value =  atof(value_string.c_str()) * in_UnitsConversionFactor;
			// Alternate conversion approach, should consider.
			// boost::optional<float> v = pt.get_optional<float>("a.path.to.float.value");
			// http://www.boost.org/doc/libs/1_43_0/doc/html/boost_propertytree/accessing.html

			if ( out_Value == 0 )
			{
				std::stringstream errorString;
				errorString <<
					"Material:          " <<  in_MaterialName << std::endl <<
					"Material Property: " <<  in_MaterialLibPropertyName << std::endl <<
					"Material Unit:	    " <<  unit  << endl <<
					"Value:             " << value_string << std::endl <<
					"A zero value is not allowed.";
				throw isis::application_exception(errorString.str());
			}
			

			isis_LOG(lg, isis_FILE, isis_INFO) <<
					"Material:                " <<  in_MaterialName << isis_EOL <<
					"Material Property:       " <<  in_MaterialLibPropertyName << isis_EOL <<
					"Material Unit:	          " <<  unit  << isis_EOL <<
					"Material Library Value:  " <<  value_string << isis_EOL <<
					"Unit Conversion Factor:  " <<  in_UnitsConversionFactor << isis_EOL <<
					"Convert to Unit:         " << in_ConvertToUnit << isis_EOL <<
					"Converted Value		  " << out_Value;
		}

		return materialFound;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////

	// Throw an exception if:
	// a) All the meterials in in_MaterialNames are not found in n_MaterialLibrary_PathAndFileName
	// b) Material properties are not in Pa or kg/m3. 
	//
	//  Note - this function converts :
	//				Pa to MPa
	//				kg/m3 to kg/mm3 
	//				W/(m*K) to W/(mm*K)
	//				and stores the converted values
	//         in out_Materials
	//

	void ReadMaterialsLibrary(  	const std::string &in_MaterialLibrary_PathAndFileName,
									const std::set<std::string> &in_MaterialNames,
									std::map<std::string, Material> &out_Materials ) 
													 throw (isis::application_exception)
	{
		using boost::property_tree::ptree;

		std::string currentMaterialName;
		std::string currentMaterialProperty;

		try
		{
			// Create a set of material names.  When a material is found remove it from the set.
			// If the set is not empty at the end of the program, throw and exception.
			std::set<std::string>  notFound_MaterialNames_set;
			for each ( std::string i in in_MaterialNames) notFound_MaterialNames_set.insert(i);

			boost::filesystem::path p(in_MaterialLibrary_PathAndFileName);
			if (boost::filesystem::exists(p))
			{
				if (boost::filesystem::is_regular_file(p))
				{
					ptree pt;
					boost::property_tree::read_json(p.make_preferred().string(), pt);
					BOOST_FOREACH(ptree::value_type& materialLib_pt, pt.get_child("Material library")) 
					{
						//if (materialLib_pt.size() != 1 )
						//{
						//	std::stringstream errorString;
						//		errorString <<
						//				"Function:         ReadMaterialsLibrary" << endl <<
						//				"Material Library: "  << in_MaterialLibrary_PathAndFileName << endl <<
						//				"Number of \"Material library\" tags: " << pt.size() << endl <<
						//				"The material library must contain one and only one \"Material library\" tag.";
						//		throw isis::application_exception(errorString.str());
						//}

						//std::clog << std::endl << materialLib_pt.first.data();
						for each ( const std::string materialName in in_MaterialNames )
						{
							currentMaterialName = materialName;
							if ( isis::ConvertToUpperCase(std::string(materialLib_pt.first.data())) == isis::ConvertToUpperCase(materialName) )
							{
								notFound_MaterialNames_set.erase(materialName);
								out_Materials[materialName].materialName = materialName;
								out_Materials[materialName].analysisMaterialProperties.materialName = materialName;

								//std::cout << std::endl << materialLib_pt.first.data();
								BOOST_FOREACH(const ptree::value_type& material_pt, materialLib_pt.second)
								{		
									double materialPropertyValue;

									if ( isis::ConvertToUpperCase(material_pt.first) ==  "MATERIAL_CATEGORY" )
									{
										std::string materialType =  material_pt.second.get<std::string>("value");
										out_Materials[materialName].materialType = MaterialType_enum(materialType);
										out_Materials[materialName].analysisMaterialProperties.materialType = MaterialType_enum(materialType);
									}

									currentMaterialProperty = "mechanical__modulus_elastic";

									if ( MaterialPropertyFound_RetrieveValue( material_pt,
																	materialName,
																	currentMaterialProperty,
																	"Pa",
																	PASCAL_TO_MEGA_PASCAL_CONVERSION_FACTOR,
																	"MPa",
																	materialPropertyValue ) )
									{
										out_Materials[materialName].analysisMaterialProperties.modulusOfElasticityDefined = true;
										out_Materials[materialName].analysisMaterialProperties.modulusOfElasticity = materialPropertyValue;
										out_Materials[materialName].analysisMaterialProperties.modulusOfElasticityUnit = CADUnitsPressure_enum("MPa");
									}

									currentMaterialProperty = "mechanical__ratio_poissons";

									if ( MaterialPropertyFound_RetrieveValue( material_pt,
																	materialName,
																	currentMaterialProperty,
																	"",
																	1.0,
																	"",
																	materialPropertyValue ) )
									{
										out_Materials[materialName].analysisMaterialProperties.poissonsRatioDefined = true;
										out_Materials[materialName].analysisMaterialProperties.poissonsRatio = materialPropertyValue;
									}

									currentMaterialProperty = "mechanical__strength_tensile_yield";

									if ( MaterialPropertyFound_RetrieveValue( material_pt,
																	materialName,
																	currentMaterialProperty,
																	"Pa",
																	PASCAL_TO_MEGA_PASCAL_CONVERSION_FACTOR,
																	"MPa",
																	materialPropertyValue ) )
									{
										out_Materials[materialName].analysisMaterialProperties.tensileYieldStrengthDefined = true;
										out_Materials[materialName].analysisMaterialProperties.tensileYieldStrength = materialPropertyValue;
										out_Materials[materialName].analysisMaterialProperties.tensileYieldStrengthUnit = CADUnitsPressure_enum("MPa");
									}

									currentMaterialProperty = "mechanical__strength_tensile_ultimate";

									if ( MaterialPropertyFound_RetrieveValue( material_pt,
																	materialName,
																	currentMaterialProperty,
																	"Pa",
																	PASCAL_TO_MEGA_PASCAL_CONVERSION_FACTOR,
																	"MPa",
																	materialPropertyValue ) )
									{
										out_Materials[materialName].analysisMaterialProperties.tensileUltimateStrengthDefined = true;
										out_Materials[materialName].analysisMaterialProperties.tensileUltimateStrength = materialPropertyValue;
										out_Materials[materialName].analysisMaterialProperties.tensileUltimateStrengthUnit = CADUnitsPressure_enum("MPa");
									}

									currentMaterialProperty = "mechanical__strength_shear";

									if ( MaterialPropertyFound_RetrieveValue( material_pt,
																	materialName,
																	currentMaterialProperty,
																	"Pa",
																	PASCAL_TO_MEGA_PASCAL_CONVERSION_FACTOR,
																	"MPa",
																	materialPropertyValue ) )
									{
										out_Materials[materialName].analysisMaterialProperties.shearStrengthDefined = true;
										out_Materials[materialName].analysisMaterialProperties.shearStrength = materialPropertyValue;
										out_Materials[materialName].analysisMaterialProperties.shearStrengthUnit = CADUnitsPressure_enum("MPa");
									}

									currentMaterialProperty = "mechanical__strength_fatigue";

									if ( MaterialPropertyFound_RetrieveValue( material_pt,
																	materialName,
																	currentMaterialProperty,
																	"Pa",
																	PASCAL_TO_MEGA_PASCAL_CONVERSION_FACTOR,
																	"MPa",
																	materialPropertyValue ) )
									{
										out_Materials[materialName].analysisMaterialProperties.fatigueStrengthDefined = true;
										out_Materials[materialName].analysisMaterialProperties.fatigueStrength = materialPropertyValue;
										out_Materials[materialName].analysisMaterialProperties.fatigueStrengthUnit = CADUnitsPressure_enum("MPa");
									}
	
									currentMaterialProperty = "mechanical__strength_bearing_yield";

									if ( MaterialPropertyFound_RetrieveValue( material_pt,
																	materialName,
																	currentMaterialProperty,
																	"Pa",
																	PASCAL_TO_MEGA_PASCAL_CONVERSION_FACTOR,
																	"MPa",
																	materialPropertyValue ) )
									{
										out_Materials[materialName].analysisMaterialProperties.bearingYieldStrengthDefined = true;
										out_Materials[materialName].analysisMaterialProperties.bearingYieldStrength = materialPropertyValue;
										out_Materials[materialName].analysisMaterialProperties.bearingYieldStrengthUnit = CADUnitsPressure_enum("MPa");
									}

									currentMaterialProperty = "mechanical__strength_bearing_ultimate";

									if ( MaterialPropertyFound_RetrieveValue( material_pt,
																	materialName,
																	currentMaterialProperty,
																	"Pa",
																	PASCAL_TO_MEGA_PASCAL_CONVERSION_FACTOR,
																	"MPa",
																	materialPropertyValue ) )
									{
										out_Materials[materialName].analysisMaterialProperties.bearingUltimateStrengthDefined = true;
										out_Materials[materialName].analysisMaterialProperties.bearingUltimateStrength = materialPropertyValue;
										out_Materials[materialName].analysisMaterialProperties.bearingUltimateStrengthUnit = CADUnitsPressure_enum("MPa");
									}

									currentMaterialProperty = "density";

									if ( MaterialPropertyFound_RetrieveValue( material_pt,
																	materialName,
																	currentMaterialProperty,
																	//"kg/m3",
																	"kg/m^3",
																	M_CUBED_TO_MM_CUBED_RECIPROCAL,
																	"kg/mm3",
																	materialPropertyValue ) )
									{
										out_Materials[materialName].analysisMaterialProperties.denstiyDefined = true;
										out_Materials[materialName].analysisMaterialProperties.density= materialPropertyValue;
										out_Materials[materialName].analysisMaterialProperties.densityUnit = CADUnitsDensity_enum("kg/mm3");
									}

									currentMaterialProperty = "thermal__conductivity";

									if ( MaterialPropertyFound_RetrieveValue( material_pt,
																	materialName,
																	currentMaterialProperty,						
																	"W/(m*K)",
																	M_TO_MM_RECIPROCAL,
																	"w/mmk",
																	materialPropertyValue ) )
									{
										out_Materials[materialName].analysisMaterialProperties.thermalConductivityDefined = true;
										out_Materials[materialName].analysisMaterialProperties.thermalConductivity = materialPropertyValue;
										out_Materials[materialName].analysisMaterialProperties.thermalConductivityUnit = CADUnitsThermalConductivity_enum("w/mmk");
									}

									currentMaterialProperty = "thermal__capacity_specific_heat";

									if ( MaterialPropertyFound_RetrieveValue( material_pt,
																	materialName,
																	currentMaterialProperty,						
																	"J/(kg-K)",
																	1.0,
																	"j/kgk",
																	materialPropertyValue ) )
									{
										out_Materials[materialName].analysisMaterialProperties.heatCapacityDefined = true;
										out_Materials[materialName].analysisMaterialProperties.heatCapacity = materialPropertyValue;
										out_Materials[materialName].analysisMaterialProperties.heatCapacityUnit = CADUnitsHeatCapacity_enum("j/kgk");
									}

								}  // END BOOST_FOREACH(const ptree::value_type& material_pt, materialLib_pt.second)					
							}  // END if ( isis::ConvertToUpperCase(std::string(materialLib_pt.first.data())) == isis::ConvertToUpperCase(materialName) )
						}  // END for each ( const std::string i in in_MaterialNames )
					} // END BOOST_FOREACH(ptree::value_type& materialLib_pt, pt.get_child("Material library")) 
				} // END if (boost::filesystem::is_regular_file(p))
			} // END if (boost::filesystem::exists(p))
			else
			{
				std::stringstream errorString;
				errorString << "Material library input file not found: " << in_MaterialLibrary_PathAndFileName;
				throw isis::application_exception(errorString.str());
			}

			if ( notFound_MaterialNames_set.size() != 0 )
			{
				std::stringstream errorString;
				errorString << "Material(s) not found in the material library: " << in_MaterialLibrary_PathAndFileName << std::endl <<
								"Material(s):" << std::endl;
				for each (std::string i in notFound_MaterialNames_set) errorString << i << std::endl;
				throw isis::application_exception(errorString.str());

			}

		}
		catch(const boost::property_tree::ptree_error &exc)
		{		
			std::stringstream errorString;
			errorString << "Input file processing error: " << in_MaterialLibrary_PathAndFileName <<  std::endl <<
							"   Material Name:     " << currentMaterialName <<  std::endl <<
							"   Material Property: " << currentMaterialProperty <<  std::endl <<
							"   Error: " << exc.what();
			throw isis::application_exception(errorString.str());
		}

		catch(const isis::application_exception &exc)
		{		
			std::stringstream errorString;
			errorString << "Input file processing error: " << in_MaterialLibrary_PathAndFileName <<  std::endl <<
							"   Material Name:     " << currentMaterialName <<  std::endl <<
							"   Material Property: " << currentMaterialProperty <<  std::endl <<
							"   Error: " << exc.what();
			throw isis::application_exception(errorString.str());
		}

	} // END ReadMaterialsLibrary


} // namespace isis