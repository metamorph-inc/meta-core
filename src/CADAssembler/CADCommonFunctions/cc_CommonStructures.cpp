#include "cc_CommonStructures.h"
//#include <CommonUtilities.h>
//#include <CommonFunctions.h>
#include <boost/algorithm/string.hpp>
#include <list>
#include <set>

namespace isis
{

	//std::ostream& operator <<(std::ostream & out, const bool & value) {
	//    out << Bool_string(value);
	//	return out;
	//}

	std::string  Bool_string( bool in_Bool )
	{
		if ( in_Bool )
			return "TRUE";
		else
			return "FALSE";
	}


		BoundingBox::BoundingBox(): boundingBox_Point_1(0.0,0.0,0.0), boundingBox_Point_2(0.0,0.0,0.0),  boundingBox_Defined(false)
		{
			Dimensions_xyz[0] = 0.0;
			Dimensions_xyz[1] = 0.0;
			Dimensions_xyz[2] = 0.0;
		}


	MassProperties::MassProperties() 
	{
		setValuesToNotDefinedAndZeros();
	}

	void MassProperties::setValuesToNotDefinedAndZeros()
	{

		volume = 0.0;
		surfaceArea = 0.0;
		density = 0.0;
		mass = 0.0;
		massProperties_RetrievalInvoked = false;
		massProperties_Defined = false;
		volume_Defined = false;
		surfaceArea_Defined = false;
		density_Defined = false;
		mass_Defined = false;
		coordSysInertiaTensor_Defined = false;
		centerOfGravity_Defined = false;
		cGInertiaTensor_Defined = false;
		principalMomentsOfInertia_Defined = false;
		principalAxis_RotationMatrix_Defined = false;


		for ( int i = 0 ; i < 3; ++i )
			for ( int j = 0 ; j < 3; ++j)	coordSysInertiaTensor[i][j] = 0.0;

		for ( int i = 0 ; i < 3; ++i )		centerOfGravity[i] = 0.0;

		for ( int i = 0 ; i < 3; ++i )
			for ( int j = 0 ; j < 3; ++j)	cGInertiaTensor[i][j] = 0.0;

		for ( int i = 0 ; i < 3; ++i )		principalMomentsOfInertia[i] = 0.0;

		for ( int i = 0 ; i < 3; ++i )
			for ( int j = 0 ; j < 3; ++j)	principalAxis_RotationMatrix[i][j] = 0.0;

	}


		CADComponentData::CADComponentData():dataInitialSource( INITIAL_SOURCE_INPUT_XML_FILE),
							name(CAD_NAME_SIZE - 1), 
							cADModel_ptr_ptr(0), cADModel_hdl(0), 
							parametricParametersPresent(false),  
							//specialInstruction(CAD_SPECIAL_INSTRUCTION_NONE), 
							analysisTempMaterialDefined(false), 
							cyPhyComponent(true),
							geometryRepresentation(CAD_NAME_SIZE - 1),
							addedToAssemblyOrdinal(LONG_MAX),
							partOrdinal(-1),
							partAssemblyOrdinal(-1)
		{
			assembledFeature.id = 0;
			//assembledFeature.type = PRO_TYPE_INVALID;
			assembledFeature.type = CAD_FEATURE_GEOMETRY_TYPE_INVALID;
			assembledFeature.owner = 0;

			// Set initial position to identity matrix
			for (int i = 0; i < 4; ++i)
				for (int j = 0; j < 4; ++j) initialPosition[i][j] = 0.0;

			initialPosition[0][0] = 1.0;
			initialPosition[1][1] = 1.0;
			initialPosition[2][2] = 1.0;
			initialPosition[3][3] = 1.0;

		}


	std::ostream& operator<<(std::ostream& output, const CADParameter &in_CADParameter)
	{

		output << std::endl << std::endl << 
			"CADParameter:" << std::endl <<
			"   Name:  " <<		in_CADParameter.name << std::endl <<
			"   Type:  " <<		CADParameterType_string(in_CADParameter.type) << std::endl <<
			"   Value: " <<		in_CADParameter.value;
		return output;
	}

	bool CADParameter::operator==(const CADParameter &in_Parameter ) const
	{
		//return ConvertToUpperCase(name) ==  ConvertToUpperCase(in_Parameter.name);
		return boost::iequals((std::string)name, (std::string)in_Parameter.name);  
	}

	std::ostream& operator<<(std::ostream& output, const CADComponentData& in_AssembledInfo)
	{
		output << std::endl << std::endl << 
			"CADComponent:" << std::endl <<
			"   ComponentInstanceID:       " << in_AssembledInfo.componentID.c_str() << std::endl <<
			"   Name:                      " << in_AssembledInfo.name << std::endl <<
			"   Type:                      " << isis::CADMdlType_string(in_AssembledInfo.modelType) <<  std::endl <<
			"   MaterialID (FromCyPhy):    " << in_AssembledInfo.materialID_FromCyPhy <<  std::endl <<
			"   MaterialID (FromCreoPart): " << in_AssembledInfo.materialID_FromCADPart <<  std::endl <<
			//"   SpecialInstruction:        " << isis::SpecialInstruction_string(in_AssembledInfo.specialInstruction) <<  std::endl <<
			"   SpecialInstruction:        " << in_AssembledInfo.specialInstruction <<  std::endl <<
			"   Representation:            " << in_AssembledInfo.geometryRepresentation <<  std::endl <<
			"   cADModel_hdl:              " << in_AssembledInfo.cADModel_hdl <<  std::endl <<
			"   cADModel_ptr_ptr:          " << in_AssembledInfo.cADModel_ptr_ptr << std::endl <<
			"   assembledFeature.id:       " << in_AssembledInfo.assembledFeature.id <<  std::endl <<
			"   assembledFeature.type:     " << in_AssembledInfo.assembledFeature.type <<  std::endl <<
			"   assembledFeature.owner:    " << in_AssembledInfo.assembledFeature.owner <<  std::endl <<

			"   constraints:";

			output << std::endl <<
				"      applyAllConstraints:     "  << Bool_string(in_AssembledInfo.constraintDef.applyAllConstraints) <<  std::endl <<
				"      applySingleConstaintID:  "  << in_AssembledInfo.constraintDef.applySingleConstaintID;

			//output << std::endl << "in_AssembledInfo.constraintDef.constraints.capacity() %%%  " << in_AssembledInfo.constraintDef.constraints.capacity();
			
			
			for ( std::vector<ConstraintData>::const_iterator i(in_AssembledInfo.constraintDef.constraints.begin());
				  i != in_AssembledInfo.constraintDef.constraints.end();
				  ++i )
			{
				if ( i->computedJointData.globalJointPositionDefined )
				{
					output << std::endl << "      globalJointPosition, location:    "  << i->computedJointData.globalJointPosition.location;
					output << std::endl << "      globalJointPosition: orientation: "  << i->computedJointData.globalJointPosition.orientation;
					output << std::endl << "      globalJointPosition: rotation:    "  << i->computedJointData.globalJointPosition.rotation;
				}
				else
				{
					output << std::endl << "      globalJointPosition:     Not Defined"; 
				}
				for (	std::vector<ConstraintPair>::const_iterator j(i->constraintPairs.begin());
						j != i->constraintPairs.end();
						++j )
				{					
					output << std::endl <<	"      treatConstraintAsAGuide: " << CADTreatConstraintAsAGuide_string(j->treatConstraintAsAGuide);
					output << std::endl <<	"      constraintFeatures:";
					for (	std::vector<ConstraintFeature>::const_iterator k(j->constraintFeatures.begin());
							k != j->constraintFeatures.end();
							++k )
					{
						output << std::endl <<
						"         componentInstanceID:     " <<  k->componentInstanceID  << std::endl <<
						"         featureName:             " <<  k->featureName  << std::endl <<
						"         featureOrientationType:  " <<  CADDatumside_string( k->featureOrientationType );

					}
				}

			}
			output << std::endl << "   parametricParameters:";

			if ( in_AssembledInfo.parametricParametersPresent )
			{
				for ( std::list<CADParameter>::const_iterator i( in_AssembledInfo.parametricParameters.begin());
					  i != in_AssembledInfo.parametricParameters.end();
					  ++i )
				{
					output << std::endl << "      name:    " << i->name;
					output << std::endl << "      name:    " << CADParameterType_string(i->type);
					output << std::endl << "      value:   " << i->value;
				}
			}

			output << std::endl <<
			"   componentPaths list:";
			for ( std::list<int>::const_iterator i = in_AssembledInfo.componentPaths.begin();
			  i != in_AssembledInfo.componentPaths.end(); ++i )
			{
				output << std::endl << "      Feature ID: " << *i;
			}

			output << std::endl << "   children set:";
			for ( std::list<std::string>::const_iterator i = in_AssembledInfo.children.begin();
			  i != in_AssembledInfo.children.end(); ++i )
			{
				output << std::endl << "      ComponentID: " << *i;
			}

			output << std::endl << "   dependsOn set:";
			for ( std::set<std::string>::const_iterator i = in_AssembledInfo.dependsOn.begin();
			  i != in_AssembledInfo.dependsOn.end(); ++i )
			{
				output << std::endl << "      ComponentID: " << *i;
			}

			if ( in_AssembledInfo.analysisTempMaterialDefined )
			{
				output << std::endl << "   Material Definition:";
				output << std::endl << "      Name:                  " << in_AssembledInfo.analysisTempMaterialDefinition.name;
				output << std::endl << "      Poisson's Ratio:       " << in_AssembledInfo.analysisTempMaterialDefinition.poissonsRatio;
				output << std::endl << "      Token Poisson's Ratio: " << in_AssembledInfo.analysisTempMaterialDefinition.tokenPoissonsRatio;
			}
			else
			{
				output << std::endl << "   Material Definition: Not retrieved from Creo Model.  Only retrieved for analysis runs.";
			}

		return output;
	};

////////////////////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& output, const CADComputation &in_CADComputation )
{
	output << std::endl << std::endl << 
			 "CADComputation (i.e. Metrics):" << std::endl <<
			"   componentID:                                "   << in_CADComputation.componentID << std::endl <<
			"   metricID:                                   "   << in_CADComputation.metricID << std::endl <<
			"   computationType (MetricType):               "   << isis::ComputationType_string(in_CADComputation.computationType) << std::endl <<
			"   computationSubType:                         "   << isis::ComputationSubType_string(in_CADComputation.computationSubType) << std::endl <<
			"   computationDimension (RequestedValueType):  "   << isis::ComputationDimension_string(in_CADComputation.computationDimension) << std::endl <<			
			"   datumName (details)                         "   << in_CADComputation.datumName << std::endl;  // typically datum name
	//zzz

	return output;
}
////////////////////////////////////////////////////////////////////////////////////////

std::ostream& operator<<(std::ostream& output, const CADHydrostaticParameters &in_CADHydrostaticParameters)
{
	output << std::endl << 
		"HydrostaticParameters: " <<  std::endl <<
		"     pitchAngleStart:    " << in_CADHydrostaticParameters.pitchAngleStart << std::endl << 
		"	  pitchAngleEnd:      " << in_CADHydrostaticParameters.pitchAngleEnd << std::endl << 
		"     pitchAngleStepSize: " << in_CADHydrostaticParameters.pitchAngleStepSize << std::endl << 
		"     rollAngleStart:     " << in_CADHydrostaticParameters.rollAngleStart << std::endl << 
		"     rollAngleEnd:       " << in_CADHydrostaticParameters.rollAngleEnd << std::endl <<
		"     rollAngleStepSize:  " << in_CADHydrostaticParameters.rollAngleStepSize << std::endl << 
		"     fluidMaterial:      " << in_CADHydrostaticParameters.fluidMaterial << std::endl << 
		"     fluidTemperature:   " << in_CADHydrostaticParameters.fluidTemperature;

	return output;
}
////////////////////////////////////////////////////////////////////////////////////////

std::ostream& operator<<(std::ostream& output, const AnalysisMaterialProperties &in_AnalysisMaterialProperties )
{
	output << std::endl << std::endl << 
		"AnalysisMaterialProperties, Material Name: "  << in_AnalysisMaterialProperties.materialName <<  std::endl <<

			"   MaterialType:                    "  << MaterialType_string(in_AnalysisMaterialProperties.materialType) << std::endl <<
			 std::endl <<
			"   modulusOfElasticityDefined:      "  << in_AnalysisMaterialProperties.modulusOfElasticityDefined << std::endl <<	
			"   modulusOfElasticity:             "  << in_AnalysisMaterialProperties.modulusOfElasticity << std::endl <<	
			"   modulusOfElasticityUnit:         "  << CADUnitsPressure_string(in_AnalysisMaterialProperties.modulusOfElasticityUnit) << std::endl <<	

			 std::endl <<	
			"   poissonsRatioDefined:            "  << in_AnalysisMaterialProperties.poissonsRatioDefined << std::endl <<	
			"   poissonsRatio:                   "  << in_AnalysisMaterialProperties.poissonsRatio << std::endl <<	
			 std::endl <<	
			"   tensileYieldStrengthDefined:     " << in_AnalysisMaterialProperties.tensileYieldStrengthDefined << std::endl <<
			"   tensileYieldStrength:            "  << in_AnalysisMaterialProperties.tensileYieldStrength << std::endl <<	
			"   tensileYieldStrengthUnit:        "  << CADUnitsPressure_string(in_AnalysisMaterialProperties.tensileYieldStrengthUnit) << std::endl <<	
			 std::endl <<	
			"   tensileUltimateStrengthDefined:  " << in_AnalysisMaterialProperties.tensileUltimateStrengthDefined << std::endl <<
			"   tensileUltimateStrength:         "  << in_AnalysisMaterialProperties.tensileUltimateStrength << std::endl <<	
			"   tensileUltimateStrengthUnit:     "  << CADUnitsPressure_string(in_AnalysisMaterialProperties.tensileUltimateStrengthUnit) << std::endl <<	
			 std::endl <<	
			"   shearStrengthDefined:            "  << in_AnalysisMaterialProperties.shearStrengthDefined << std::endl <<
			"   shearStrength:                   "  << in_AnalysisMaterialProperties.shearStrength << std::endl <<			
			"   shearStrengthUnit:               "  << CADUnitsPressure_string(in_AnalysisMaterialProperties.shearStrengthUnit) << std::endl <<	
			 std::endl <<	
			"   bearingYieldStrengthDefined:     "  << in_AnalysisMaterialProperties.bearingYieldStrengthDefined << std::endl <<
			"   bearingYieldStrength:            "  << in_AnalysisMaterialProperties.bearingYieldStrength << std::endl <<
			"   bearingYieldStrengthUnit:        "  << CADUnitsPressure_string(in_AnalysisMaterialProperties.bearingYieldStrengthUnit) << std::endl <<	
			 std::endl <<	
			"   bearingUltimateStrengthDefined:  "  << in_AnalysisMaterialProperties.bearingUltimateStrengthDefined << std::endl <<	
			"   bearingUltimateStrength:         "  << in_AnalysisMaterialProperties.bearingUltimateStrength << std::endl <<	
			"   bearingUltimateStrengthUnit:     "  << CADUnitsPressure_string(in_AnalysisMaterialProperties.bearingUltimateStrengthUnit) << std::endl <<	
			 std::endl <<	
			"   fatigueStrengthDefined:          "  << in_AnalysisMaterialProperties.fatigueStrengthDefined << std::endl <<	
			"   fatigueStrength:                 "  << in_AnalysisMaterialProperties.fatigueStrength << std::endl <<	
			"   fatigueNumberOfCycles:           "  << in_AnalysisMaterialProperties.fatigueNumberOfCycles << std::endl <<	
			 std::endl <<	
			"   denstiyDefined:                  " << in_AnalysisMaterialProperties.denstiyDefined << std::endl <<
			"   density:                         " << in_AnalysisMaterialProperties.density << std::endl <<
			"   densityUnit:                     "  << CADUnitsDensity_string(in_AnalysisMaterialProperties.densityUnit);

	return output;
}
////////////////////////////////////////////////////////////////////////////////////////
//std::string AmalgamateModelNameWithSuffix ( const std::string &in_Name, const std::string &in_Type )
std::string AmalgamateModelNameWithSuffix ( const std::string &in_Name, e_CADMdlType in_pro_model_type )
{
	std::string ModelNameWithSuffix;

	//wchar_t  PartName[PRO_FAMILY_NAME_SIZE];
	//wchar_t  PartName[ISIS_CHAR_BUFFER_LENGTH];

	//ProMdlType pro_model_type = isis::ProMdlType_enum( in_Type);

	if ( in_pro_model_type == CAD_MDL_ASSEMBLY)
		ModelNameWithSuffix = in_Name + ".asm";
	else
		ModelNameWithSuffix = in_Name + ".prt";

	return ModelNameWithSuffix;
}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool ConstraintData::hasAGuideConstraint() const
	{
		for each ( ConstraintPair i  in constraintPairs ) if ( i.treatConstraintAsAGuide == CAD_TREAT_CONSTRAINT_AS_A_GUIDE_TRUE ) return true;	
		return false;
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::vector<ConstraintPair> ConstraintData::getConstraintPairsWithoutGuide() const
	{
		std::vector<ConstraintPair>  constraintPairs_temp;

		for each ( ConstraintPair i  in constraintPairs )
		{
			if ( i.treatConstraintAsAGuide == CAD_TREAT_CONSTRAINT_AS_A_GUIDE_FALSE ) constraintPairs_temp.push_back(i);
		}

		return constraintPairs_temp;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool ConstraintDefinition::hasAGuideConstraint() const
	{
		for each ( ConstraintData i  in constraints )if ( i.hasAGuideConstraint()) return true;
		return false;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool CADSpecialInstruction::operator == (e_CADSpecialInstruction in_specialInstruction) const
	{
		if ( specialInstructions.find(in_specialInstruction) != specialInstructions.end() )
			return true;
		else
			return false;
	}

	bool CADSpecialInstruction::operator != (e_CADSpecialInstruction in_specialInstruction) const
	{
		if ( specialInstructions.find(in_specialInstruction) == specialInstructions.end() )
			return true;
		else
			return false;
	}

	std::string CADSpecialInstruction::ToCommaDelimitedString() const
	{
		std::stringstream outString;
		bool firstString = true;
		for each ( const e_CADSpecialInstruction &i in specialInstructions )
		{
			if ( firstString ) 
				firstString = false;
			else
				outString << ",";

			outString << isis::SpecialInstruction_string(i);
		}
		return outString.str();
	}


	void CADSpecialInstruction::FromCommaDelimitedString(const std::string &in_String) 
	{
		std::vector< std::string > split_vector;
		boost::split(split_vector, in_String, boost::is_any_of(","));

		for each (const std::string &i in split_vector )
		{
			std::string temp_string = i;
			boost::trim(temp_string);
			specialInstructions.insert(isis::SpecialInstruction_enum(temp_string));
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::ostream& operator<<(std::ostream& output,const CADSpecialInstruction &in_CADSpecialInstruction )
	{
		//for each ( const e_CADSpecialInstruction &i in in_CADSpecialInstruction.specialInstructions )
		//	output <<  isis::SpecialInstruction_string(i) << " ";
		output << in_CADSpecialInstruction.ToCommaDelimitedString();
				
		return output;
	}


	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	std::ostream& operator<<(std::ostream& output, const CopyModelDefinition &in_CopyModelDefinition)
	{
		output << "From Model: " << in_CopyModelDefinition.fromModelName << "  To Model: " << in_CopyModelDefinition.toModelName;

		output << "  Model Type: " << isis::CADMdlType_string(in_CopyModelDefinition.modelType) << "  ComponentInstanceID: "  << in_CopyModelDefinition.componentInstanceID;

		//if ( in_CopyModelDefinition.modelType == CAD_MDL_PART )
		//	output << "  Part";
		//else
		//	output << "  Assembly";

		return output;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::ostream& operator<<(std::ostream& output, const std::vector<CopyModelDefinition> &in_CopyModelDefinition_vector)
	{
		for each ( CopyModelDefinition i in in_CopyModelDefinition_vector )
		{
			output << std::endl << i;
		}

		return output;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CADModelUnits::CADModelUnits() :	distanceUnit(CAD_UNITS_DISTANCE_NA), 
									massUnit(		CAD_UNITS_MASS_NA), 
									forceUnit(		CAD_UNITS_FORCE_NA), 
									timeUnit(		CAD_UNITS_TIME_NA) , 
									temperatureUnit(CAD_UNITS_TEMPERATURE_NA) {};
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////


}

