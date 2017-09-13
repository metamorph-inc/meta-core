#include "cc_XMLtoCADStructures.h"
//#include "DiagnosticUtilities.h"
#include <iostream>
#include "cc_LoggerBoost.h"

#ifndef ISIS_VERSION_NUMBER_H
#define ISIS_VERSION_NUMBER_H
//#include <ISISVersionNumber.h>
#include "cc_CommonUtilities.h"
#endif

#include "UdmBase.h"
#include "AssemblyInterface.h"
#include "cc_StringToEnumConversions.h"
//#include <MaterialProperties.h>
#include "cc_CommonConstants.h"

#include <deque>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>

#include "cc_LoggerBoost.h"

#define ISIS_CYPHY_2_CAD_DLL_MIN_VERSION_FUNCTIONS "1.2.0.0"

namespace isis
{	

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void stream_IndentedAssemblyTree( vector<AssemblyInterface::CADComponent> &cadComponentVector, 
						  ostream &out_Stream, 
						  std::string   in_Indent = "")
{
	/////////////////////////////
	// Display indented version tree
	////////////////////////////

	for ( vector<AssemblyInterface::CADComponent>::const_iterator ci = cadComponentVector.begin(); ci != cadComponentVector.end(); ci++ )
	{
		try{
			out_Stream << std::endl << in_Indent <<  std::string(ci->Name())  << "  " << std::string(ci->ComponentID());

			//ProMdlType pro_model_type = isis::ProMdlType_enum( ci->Type());
			//if ( pro_model_type == PRO_MDL_ASSEMBLY) 

			e_CADMdlType cad_model_type = isis::CADMdlType_enum( ci->Type());
			if ( cad_model_type == CAD_MDL_ASSEMBLY) 
			{
				vector<AssemblyInterface::CADComponent> childCADComponentVector = ci->CADComponent_kind_children();
				stream_IndentedAssemblyTree( childCADComponentVector,
											 out_Stream, 
											 in_Indent + "     ");
			}
		} catch (isis::application_exception &ex)
		{
			ex.setComponentInfo(ci->Name());
			throw;
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void stream_AssemblyTree( vector<AssemblyInterface::CADComponent>		&in_CADComponentVector, 
						  std::ostream								&out_Stream, 
						  std::string								in_ParentName, 
						  std::string								in_Indent = "" )
{
	/////////////////////////////
	// Display contents of tree
	////////////////////////////

	for ( vector<AssemblyInterface::CADComponent>::const_iterator ci = in_CADComponentVector.begin(); ci != in_CADComponentVector.end(); ci++)
	{
		try
		{
			out_Stream << endl << endl << in_Indent << "PARENT Component Name:  " << in_ParentName;
			out_Stream << endl << in_Indent << "Component Name:         " << std::string(ci->Name());
			out_Stream << endl << in_Indent << "Component Instance  ID: " << std::string(ci->ComponentID());
			out_Stream << endl << in_Indent << "Component Type:         " << std::string(ci->Type());
			out_Stream << endl << in_Indent << "MaterialID:             " << std::string(ci->MaterialID());
			out_Stream << endl << in_Indent << "Representation:         " << std::string(ci->Representation());

			///////////////////////////////////////////////////////////////////////////////////////////////
			// ParametricParameters 
			///////////////////////////////////////////////////////////////////////////////////////////////
			AssemblyInterface::ParametricParameters parametric = ci->ParametricParameters_child();
			if (parametric != Udm::null)
			{
				vector<AssemblyInterface::CADParameter> parameterVector = parametric.CADParameter_kind_children();
				for ( vector<AssemblyInterface::CADParameter>::const_iterator ck = parameterVector.begin(); ck != parameterVector.end(); ck++ )
				{		
					out_Stream << endl << in_Indent << "   CADParameter";
					out_Stream << endl << in_Indent << "      Type                    " <<  std::string(ck->Type());
					out_Stream << endl << in_Indent << "      Name                    "	<<  std::string(ck->Name());
					out_Stream << endl << in_Indent << "      Value                   "	<<  std::string(ck->Value());

					AssemblyInterface::Units units = ck->Units_child();
					if ( units != Udm::null)
					{
						out_Stream << endl << in_Indent << "	  Units.Value             " << std::string(units.Value());     
					}

				}  //for ( AssemblyType::CADComponent_type::ParametricParameters_type::CADParameter_const_iterator k( i->ParametricParameters().get().CADParameter().begin());
			}

			//////////////////////////////////////////////////////////////////////////////////////////
			// Constraints  
			//////////////////////////////////////////////////////////////////////////////////////////
			vector<AssemblyInterface::Constraint> constraintVector = ci->Constraint_kind_children();
			for ( vector<AssemblyInterface::Constraint>::const_iterator cj = constraintVector.begin(); cj != constraintVector.end(); cj++ )
			{
			//if ( i->Constraint().present() )
			//{
			//	for ( AssemblyType::CADComponent_type::Constraint_type::Pair_const_iterator k(i->Constraint().get().Pair().begin());
			//		  k != i->Constraint().get().Pair().end();
			//		  ++k )

				vector<AssemblyInterface::Pair> pairVector = cj->Pair_kind_children();
				for ( vector<AssemblyInterface::Pair>::const_iterator ck = pairVector.begin(); ck != pairVector.end(); ck++ )
				{
					out_Stream << endl << in_Indent << "   FeatureGeometryType   " << std::string(ck->FeatureGeometryType());
					out_Stream << endl << in_Indent << "   FeatureInterfaceType  " << std::string(ck->FeatureInterfaceType());
					out_Stream << endl << in_Indent << "   FeatureAlignmentType  " << std::string(ck->FeatureAlignmentType());
					if ( std::string(ck->Guide()).size() == 0 )
						out_Stream << endl << in_Indent << "   Guide                 False";
					else
					    out_Stream << endl << in_Indent << "   Guide                 " << std::string(ck->Guide());
			
					vector<AssemblyInterface::ConstraintFeature> featureVector = ck->ConstraintFeature_kind_children();
					for ( vector<AssemblyInterface::ConstraintFeature>::const_iterator cl = featureVector.begin(); cl != featureVector.end(); cl++ )
					{
						out_Stream << endl << in_Indent << "      FeatureName             " << std::string(cl->FeatureName());
						out_Stream << endl << in_Indent << "      ComponentInstanceID     " << std::string(cl->ComponentID());
						out_Stream << endl << in_Indent << "      FeatureOrientationType  " << std::string(cl->FeatureOrientationType());
					
					}

					AssemblyInterface::ConstraintOffset offset = ck->ConstraintOffset_child();
					if ( offset != Udm::null)
					{
			
						//for ( AssemblyType::CADComponent_type::Constraint_type::Pair_type::ConstraintOffset_type::map::const_iterator m(k->ConstraintOffset()

						out_Stream << endl  << in_Indent << "      ConstraintOffset Value:               " << offset.Value();
						out_Stream << endl  << in_Indent << "      ConstraintOffset OffsetAlignmentType: " << std::string(offset.OffsetAlignmentType());
						AssemblyInterface::Units units = offset.Units_child();
						if ( units != Udm::null )
						{
							cout << endl << in_Indent << "      ConstraintOffset Units Type:          " << std::string(units.Value());
						}
					}
				} // END for ( AssemblyType::CADComponent_type::Constraint_type::Pair_const_iterator k(j->Pair().begin());
			}  // for ( AssemblyType::CADComponent_type::Constraint_type::Pair_const_iterator k(j->Pair().begin());

			//ProMdlType pro_model_type = isis::ProMdlType_enum( ci->Type());
			// if ( pro_model_type == PRO_MDL_ASSEMBLY) 
			e_CADMdlType pro_model_type = isis::CADMdlType_enum( ci->Type());
			if ( pro_model_type == CAD_MDL_ASSEMBLY) 
			{
				vector<AssemblyInterface::CADComponent>	subAssembly = ci->CADComponent_kind_children();
				stream_AssemblyTree( subAssembly,
									 out_Stream, 
									 ci->Name(),
									 in_Indent + "     ");
			}
		} catch (isis::application_exception &ex)
		{
			ex.setComponentInfo(ci->Name());
			throw;
		}

	} // end for ( AssemblyType::CADComponent_type::CADComponent_const_iterator i(in_Begin); i != in_End; ++i )
} // end Stream_AssemblyTree

///////////////////////////////////////////////////////////////////////////////////////////////////////
void SetCADParametricParameterAttributes( const AssemblyInterface::CADComponent	 &in_XML_CADComponent,
									 std::map<std::string, CADComponentData> &out_CADComponentData_map )
													throw (isis::application_exception)
{
	std::string  ID = in_XML_CADComponent.ComponentID();
	
	AssemblyInterface::ParametricParameters parametric = in_XML_CADComponent.ParametricParameters_child();
	if (parametric != Udm::null)
	{
		vector<AssemblyInterface::CADParameter> parameterVector = parametric.CADParameter_kind_children();
		for ( vector<AssemblyInterface::CADParameter>::const_iterator ci = parameterVector.begin(); ci != parameterVector.end(); ci++ )
		{
			CADParameter CADParameter_temp;
			CADParameter_temp.name  =  ci->Name();
			CADParameter_temp.type  =  CADParameterType_enum(  ci->Type() );
			CADParameter_temp.value =  ci->Value();       // this must remain a string for now, will convert when

			out_CADComponentData_map[ID].parametricParameters.push_back(CADParameter_temp);
			out_CADComponentData_map[ID].parametricParametersPresent = true;
		}
	}


}

///////////////////////////////////////////////////////////////////////////////////////////////////////
void SetCADComponentLevelAttributes( const AssemblyInterface::CADComponent	 &in_XML_CADComponent,
									 std::map<std::string, CADComponentData> &out_CADComponentData_map )
													throw (isis::application_exception)
{
	std::string  ID = in_XML_CADComponent.ComponentID();
	
	out_CADComponentData_map[ID].componentID	= in_XML_CADComponent.ComponentID();
	string compname = in_XML_CADComponent.Name();
	if (compname.size()>ISIS_CAD_MAX_NAME_SIZE)
	{
		std::stringstream errorString;
		errorString <<
		"The input xml file contains a component name that is longer than the maximum allowable length of "  
		<< ISIS_CAD_MAX_NAME_SIZE << " characters."
		<< std::endl << "   Component Name:          " << (string)in_XML_CADComponent.Name()
		<< std::endl << "   Component ID:            " << out_CADComponentData_map[ID].componentID
		<< std::endl << "   To locate the error, search for the Component ID in the input xml file.";
		throw isis::application_exception(errorString.str());
	}
	out_CADComponentData_map[ID].name			= compname;
	out_CADComponentData_map[ID].displayName	= in_XML_CADComponent.DisplayName();
	out_CADComponentData_map[ID].geometryRepresentation = in_XML_CADComponent.Representation();
	//out_CADComponentData_map[ID].cyPhyComponent = in_XML_CADComponent.

	out_CADComponentData_map[ID].materialID_FromCyPhy	= in_XML_CADComponent.MaterialID();
	out_CADComponentData_map[ID].modelType		= CADMdlType_enum(in_XML_CADComponent.Type()); // PRO_MDL_PART, PRO_MDL_ASSEMBLY
	//out_CADComponentData_map[ID].type_string	= in_XML_CADComponent.Type();
	//out_CADComponentData_map[ID].specialInstruction = isis::SpecialInstruction_enum(in_XML_CADComponent.SpecialInstruction());
	out_CADComponentData_map[ID].specialInstruction.FromCommaDelimitedString(in_XML_CADComponent.SpecialInstruction());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
void SetConstraintAttributes( const AssemblyInterface::CADComponent	 &in_XML_CADComponent,
									 std::map<std::string, CADComponentData> &out_CADComponentData_map )
														throw (isis::application_exception)
{
	std::string  ID = in_XML_CADComponent.ComponentID();

	vector<AssemblyInterface::Constraint> constraintVector = in_XML_CADComponent.Constraint_kind_children();
	for ( vector<AssemblyInterface::Constraint>::const_iterator ci = constraintVector.begin(); ci != constraintVector.end(); ci++ )
	{
		ConstraintData  Cst_Data;

		vector<AssemblyInterface::Joint> jointVector = ci->Joint_kind_children(); 
		if (jointVector.size()==1)
		{
			// Fill out type
			Cst_Data.inputJoint.Type = CADJointType_enum(jointVector[0].Type());
			Cst_Data.inputJoint.JointID = jointVector[0].ID();
			// Deal with limits
			vector<AssemblyInterface::Limit> limitVector = jointVector[0].Limit_kind_children();
			for (vector<AssemblyInterface::Limit>::const_iterator it = limitVector.begin(); it != limitVector.end(); ++it)
			{
				if (it->Role()=="RotationDefault")
				{
					Cst_Data.inputJoint.RotationLimits[InputJoint::DEFAULT].Provided = true;
					Cst_Data.inputJoint.RotationLimits[InputJoint::DEFAULT].Value = it->Value();
				}
				else if (it->Role()=="RotationMin")
				{
					Cst_Data.inputJoint.RotationLimits[InputJoint::MIN].Provided = true;
					Cst_Data.inputJoint.RotationLimits[InputJoint::MIN].Value = it->Value();
				}
				else if (it->Role()=="RotationMax")
				{
					Cst_Data.inputJoint.RotationLimits[InputJoint::MAX].Provided = true;
					Cst_Data.inputJoint.RotationLimits[InputJoint::MAX].Value = it->Value();
				}
				else if (it->Role()=="TranslationDefault")
				{
					Cst_Data.inputJoint.TranslationLimits[InputJoint::DEFAULT].Provided = true;
					Cst_Data.inputJoint.TranslationLimits[InputJoint::DEFAULT].Value = it->Value();
				}
				else if (it->Role()=="TranslationMin")
				{
					Cst_Data.inputJoint.TranslationLimits[InputJoint::MIN].Provided = true;
					Cst_Data.inputJoint.TranslationLimits[InputJoint::MIN].Value = it->Value();
				}
				else if (it->Role()=="TranslationMax")
				{
					Cst_Data.inputJoint.TranslationLimits[InputJoint::MAX].Provided = true;
					Cst_Data.inputJoint.TranslationLimits[InputJoint::MAX].Value = it->Value();
				} else {
					std::stringstream errorString;
					errorString << "When parsing the input xml file, encountered a Joint limit definition which has an invalid Role: " << (std::string)it->Role();
						throw isis::application_exception(errorString.str());
				}
			}
			// Limit references
			vector<AssemblyInterface::LimitReference> limitRefVector = jointVector[0].LimitReference_kind_children();
			if (limitRefVector.size()!=2 && limitRefVector.size()!=0)
			{
				std::stringstream errorString;
				errorString << "When parsing the input xml file, encountered a Joint limit definition which contains more than 2 (or 1) limit references. This is not supported at this point, there should be exactly 2 limit references specified, one on each component. (" << limitRefVector.size() << " references)";
					throw isis::application_exception(errorString.str());

			} else {
				for (vector<AssemblyInterface::LimitReference>::const_iterator it = limitRefVector.begin(); it != limitRefVector.end(); ++it)
				{
					InputJoint::LimitRef *rotationtarget = (it->ComponentID() == ID) ? Cst_Data.inputJoint.RotationLimitRefs_This : Cst_Data.inputJoint.RotationLimitRefs_Other;
					InputJoint::LimitRef *translationtarget = (it->ComponentID() == ID) ? Cst_Data.inputJoint.TranslationLimitRefs_This : Cst_Data.inputJoint.TranslationLimitRefs_Other;
					if (it->Role() == "RotationDefault")
					{
						rotationtarget[InputJoint::DEFAULT].ComponentID = it->ComponentID();
						rotationtarget[InputJoint::DEFAULT].FeatureName = it->FeatureName();
						rotationtarget[InputJoint::DEFAULT].FeatureType = CADFeatureGeometryType_enum(it->FeatureGeometryType());
						rotationtarget[InputJoint::DEFAULT].Provided = true;
					}
					else if (it->Role() == "RotationMin")
					{
						rotationtarget[InputJoint::MIN].ComponentID = it->ComponentID();
						rotationtarget[InputJoint::MIN].FeatureName = it->FeatureName();
						rotationtarget[InputJoint::MIN].FeatureType = CADFeatureGeometryType_enum(it->FeatureGeometryType());
						rotationtarget[InputJoint::MIN].Provided = true;
					}
					else if (it->Role() == "RotationMax")
					{
						rotationtarget[InputJoint::MAX].ComponentID = it->ComponentID();
						rotationtarget[InputJoint::MAX].FeatureName = it->FeatureName();
						rotationtarget[InputJoint::MAX].FeatureType = CADFeatureGeometryType_enum(it->FeatureGeometryType());
						rotationtarget[InputJoint::MAX].Provided = true;
					}
					else if (it->Role() == "TranslationDefault")
					{
						translationtarget[InputJoint::DEFAULT].ComponentID = it->ComponentID();
						translationtarget[InputJoint::DEFAULT].FeatureName = it->FeatureName();
						translationtarget[InputJoint::DEFAULT].FeatureType = CADFeatureGeometryType_enum(it->FeatureGeometryType());
						translationtarget[InputJoint::DEFAULT].Provided = true;
					}
					else
					{
						std::stringstream errorString;
						errorString << "When parsing the input xml file, encountered a Joint limit reference which has an invalid Role: " << (std::string)it->Role();
							throw isis::application_exception(errorString.str());
					}
				}
			}
		}
		else if (jointVector.size()>1)
		{
			std::stringstream errorString;
			errorString <<
				"When parsing the input xml file, encountered a Constraint definition which contains more than one joint definition.";
				throw isis::application_exception(errorString.str());
		}

		vector<AssemblyInterface::Pair> pairVector = ci->Pair_kind_children();
		for ( vector<AssemblyInterface::Pair>::const_iterator ck = pairVector.begin(); ck != pairVector.end(); ck++)
		{
			ConstraintPair Cst_Pair;

			 // ProAsmcompConstrType:		PRO_ASM_ALIGN, PRO_ASM_MATE, PRO_ASM_MATE_OFF, always PRO_ASM_ALIGN for now 
			Cst_Pair.featureAlignmentType = CADAssemblyConstraintType_enum( ck->FeatureAlignmentType());

			// ProType:						PRO_SURFACE, PRO_AXIS = 	
			Cst_Pair.featureGeometryType =	CADFeatureGeometryType_enum( ck->FeatureGeometryType() );
			//Cst_Pair.featureGeometryType =	FeatureGeometryType_enum( ck->FeatureGeometryType() );

			// e_FeatureInterfaceType:		CAD_DATUM, CAD_MODEL_INTERFACE, CAD_MODEL_USER_DATA
			Cst_Pair.featureInterfaceType = FeatureInterfaceType_enum( ck->FeatureInterfaceType() );

			// e_CADTreatConstraintAsAGuide	CAD_TREAT_CONSTRAINT_AS_A_GUIDE_TRUE, CAD_TREAT_CONSTRAINT_AS_A_GUIDE_FALSE
			Cst_Pair.treatConstraintAsAGuide = CADTreatConstraintAsAGuide_enum( ck->Guide() );


			vector<AssemblyInterface::ConstraintFeature> featureVector = ck->ConstraintFeature_kind_children();
			if (featureVector.size()!=2)
			{
					std::stringstream errorString;
					errorString <<
						"When parsing the input xml file, encountered a constraint which contains more than 2 features." ;
						throw isis::application_exception(errorString.str());
			}
			for (vector<AssemblyInterface::ConstraintFeature>::const_iterator cl = featureVector.begin();  cl != featureVector.end(); cl++)
			{
				ConstraintFeature  Cst_Feature;

				Cst_Feature.componentInstanceID =				cl->ComponentID();
				Cst_Feature.featureName =				cl->FeatureName();

				if ( Cst_Feature.componentInstanceID.size() == 0  )
				{
					std::stringstream errorString;
					errorString <<  
						"When parsing the input xml file ConstraintFeature section, encountered a ComponentID that is null (i.e. zero length string)."  
						<< std::endl << "Feature Name:          " << Cst_Feature.featureName
						<< std::endl << "Component ID:          " << Cst_Feature.componentInstanceID
						<< std::endl << "To locate the error, search for ComponentID=\"\" in the input xml file.";
						throw isis::application_exception(errorString.str());
				}

				if ( Cst_Feature.featureName.size() == 0  )
				{
					std::stringstream errorString;
					errorString <<
						"When parsing the input xml file, encountered a feature name that is null (i.e. zero length string)."  
						<< std::endl << "Feature Name:          " << Cst_Feature.featureName
						<< std::endl << "Component ID:          " << Cst_Feature.componentInstanceID
						<< std::endl << "To locate the error, search for the FeatureName=\"\" in the input xml file.";
						throw isis::application_exception(errorString.str());
				}

				if ( Cst_Feature.featureName.size() > ISIS_CAD_MAX_NAME_SIZE )
				{
					std::stringstream errorString;
					errorString <<
						"When parsing the input xml file, encountered a feature name that is longer than the maximum allowable length of "  
						<< ISIS_CAD_MAX_NAME_SIZE << " characters."
						<< std::endl << "Feature Name:          " << Cst_Feature.featureName
						<< std::endl << "Component ID:          " << Cst_Feature.componentInstanceID
						<< std::endl << "To locate the error, search for the Component ID in the input xml file.";
						throw isis::application_exception(errorString.str());
				}

				Cst_Feature.featureOrientationType = CADDatumside_enum(cl->FeatureOrientationType());

				 // The order is important later: the feature belonging to this component should be first in the list
				if (cl->ComponentID()==ID)
					Cst_Pair.constraintFeatures.insert(Cst_Pair.constraintFeatures.begin(), Cst_Feature);
				else
					Cst_Pair.constraintFeatures.push_back(Cst_Feature);
			}
			
			Cst_Data.constraintPairs.push_back( Cst_Pair );

		}

		out_CADComponentData_map[ID].constraintDef.constraints.push_back( Cst_Data );
	}

}


///////////////////////////////////////////////////////////////////////////////////////////////////////
void PopulateCADComponentStructure (	 
								const AssemblyInterface::CADComponent	&in_XML_CADComponent,
								const std::string 						&in_ParentComponentID,
								std::map<std::string, CADComponentData> &out_CADComponentData_map )
{

	std::string  ID = in_XML_CADComponent.ComponentID();

	if ( ID.size() == 0 )
	{
		std::stringstream errorString;
		errorString <<
			"When parsing the input xml file, encountered a Component ID that was null (i.e. empty string)."  
			<< std::endl << "Component ID: \"" << ID << "\","
			<< std::endl << "To locate the error, search for ComponentID=\"\" in the input xml file.";
			throw isis::application_exception(errorString.str());
	}

	if ( out_CADComponentData_map.find(ID) != out_CADComponentData_map.end() )
	{
		std::stringstream errorString;
		errorString <<
			"When parsing the input xml file, encountered a Component ID that was already used.  "  
			<< std::endl << "Component ID: \"" << ID << "\","
			<< std::endl << "To locate the error, search for the Component ID in the input xml file.  Find the"
			<< std::endl << "locations where the Component ID was used more than once.";
			throw isis::application_exception(errorString.str());
	}

	//std::clog << std::endl << "PopulateCADComponentStructure, Adding: " << ID;

	SetCADComponentLevelAttributes( in_XML_CADComponent, out_CADComponentData_map );

	SetConstraintAttributes( in_XML_CADComponent, out_CADComponentData_map );

	SetCADParametricParameterAttributes( in_XML_CADComponent, out_CADComponentData_map );

	if ( in_ParentComponentID.size() > 0 ) 
	{	// Has a parent
		out_CADComponentData_map[in_ParentComponentID].children.push_back(ID);	
		out_CADComponentData_map[ID].parentComponentID = in_ParentComponentID;
	}

	vector<AssemblyInterface::CADComponent> childCADComponentVector = in_XML_CADComponent.CADComponent_kind_children();
	for ( vector<AssemblyInterface::CADComponent>::const_iterator ci = childCADComponentVector.begin(); ci != childCADComponentVector.end(); ci++ )
	{
		try{
			PopulateCADComponentStructure(  *ci, ID , out_CADComponentData_map );
		} catch (isis::application_exception &ex)
		{
			ex.setComponentInfo(ci->Name());
			throw;
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
// in_Value must either be a number or the string "FIXED".
double ConstraintValue( const std::string &in_Value) throw (isis::application_exception)
{
	// check if the string is "FIXED"
	std::string valueString = isis::ConvertToUpperCase(in_Value);

	if ( strcmp("FIXED", valueString.c_str() )  == 0  ) return 0; 
			
	return isis::ConvertToDouble(valueString);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////

void PopulateGeometry ( const vector<AssemblyInterface::Geometry> in_GeometryVector, AnalysisGeometry &out_Struct_Geometry )
{
	for ( vector<AssemblyInterface::Geometry>::const_iterator i(in_GeometryVector.begin());
		  i != in_GeometryVector.end(); ++ i )
	{

		vector<AssemblyInterface::Features> featuresVector = i->Features_kind_children();

		for ( vector<AssemblyInterface::Features>::const_iterator ck = featuresVector.begin(); ck != featuresVector.end(); ck++ )
		{
		
			AnalysisGeometryFeature  analysisGeometryFeature;

			analysisGeometryFeature.featureID = ck->FeatureID();
			analysisGeometryFeature.geometryType = CADGeometryType_enum(ck->GeometryType());
			analysisGeometryFeature.featureInterfaceType = FeatureInterfaceType_enum(ck->FeatureInterfaceType());
			analysisGeometryFeature.analysisFeatureGeometryType = CADAnalysisFeatureGeometryType_enum(ck->FeatureGeometryType());
			analysisGeometryFeature.primaryGeometryQualifier =   CADPrimaryGeometryQualifier_enum(   ck->PrimaryGeometryQualifier());
			analysisGeometryFeature.secondaryGeometryQualifier = CADSecondaryGeometryQualifier_enum( ck->SecondaryGeometryQualifier());
			
			// zzz add set operation, schema needs to change to 0 or 1.

			vector<AssemblyInterface::Feature> featureChildren =  ck->Feature_kind_children();

			for ( vector<AssemblyInterface::Feature>::const_iterator fi(featureChildren.begin()); fi != featureChildren.end(); ++ fi)
			{
				CADFeature cadFeature;

				cadFeature.componentID = fi->ComponentID();
				cadFeature.datumName = fi->Name();
				analysisGeometryFeature.features.push_back( cadFeature );
			}

			out_Struct_Geometry.features.push_back(analysisGeometryFeature);
		}

		vector<AssemblyInterface::SetOperation> setOperations = i->SetOperation_kind_children();

		if ( setOperations.size() > 0 )
		{
			out_Struct_Geometry.setOperationDefined = true;
			for ( vector<AssemblyInterface::SetOperation>::const_iterator so(setOperations.begin());
				  so != setOperations.end(); ++ so )
			{
				AnalysisGeometrySetOperation  analysisGeometrySetOperation;
				analysisGeometrySetOperation.setOperation =  GeometrySetOperation_enum(so->Type());

				// FIX THIS FIX THIS
				// Need to make this work with the new UDM model ( Cad_xml.xme)
				//	SetOperation
				//	Type string
				//	FeatureID	String[2..*/
	

				// most recent approach 2/22/2013 analysisGeometrySetOperation.feature_1_ID =  so->Feature1_ID();
				// most recent approach 2/22/2013 analysisGeometrySetOperation.feature_2_ID =  so->Feature2_ID();

				//analysisGeometrySetOperation.feature_1_PrimaryGeometryQualifier = CADPrimaryGeometryQualifier_enum( so->Feature1_GeometryQualifier());
				//analysisGeometrySetOperation.feature_2_PrimaryGeometryQualifier = CADPrimaryGeometryQualifier_enum( so->Feature2_GeometryQualifier());

				//analysisGeometrySetOperation.feature_1_SecondaryGeometryQualifier = CADSecondaryGeometryQualifier_enum( so->f);
				//analysisGeometrySetOperation.feature_2_SecondaryGeometryQualifier = CADSecondaryGeometryQualifier_enum( so->Feature2_GeometryQualifier());
				out_Struct_Geometry.setOperations.push_back(analysisGeometrySetOperation);
			}
		}

	
	}
}

/*

void PopulateGeometry ( const AssemblyInterface::Geometry &in_XML_Geometry, AnalysisGeometry &out_Struct_Geometry )
{
	vector<AssemblyInterface::Features> featuresVector = in_XML_Geometry.Features_kind_children();

	for ( vector<AssemblyInterface::Features>::const_iterator ck = featuresVector.begin(); ck != featuresVector.end(); ck++ )
	{
			AnalysisGeometryFeature  analysisGeometryFeature;

			analysisGeometryFeature.featureID = ck->FeatureID();
			analysisGeometryFeature.geometryType = CADGeometryType_enum(ck->GeometryType());
			analysisGeometryFeature.featureInterfaceType = FeatureInterfaceType_enum(ck->FeatureInterfaceType());
			analysisGeometryFeature.featureGeometryType = CADAnalysisFeatureGeometryType_enum(ck->FeatureGeometryType());
				
			vector<AssemblyInterface::Feature> featureChildren =  ck->Feature_kind_children();

			for ( vector<AssemblyInterface::Feature>::const_iterator fi(featureChildren.begin()); fi != featureChildren.end(); ++ fi)
			{
				CADFeature cadFeature;

				cadFeature.componentID = fi->ComponentID();
				cadFeature.datumName = fi->Name();
				analysisGeometryFeature.features.push_back( cadFeature );
			}

			out_Struct_Geometry.features.push_back(analysisGeometryFeature);
	}

	// Uniary Operation
	//vector<AssemblyInterface::UnaryOperation> unaryOperations = in_XML_Geometry.UnaryOperation_kind_children();

	//if ( unaryOperations.size() > 0 )
	//{
	//	out_Struct_Geometry.unaryOperationCount = unaryOperations.size();
	//	out_Struct_Geometry.unaryOperationDefined = true;
	//	out_Struct_Geometry.unaryGeometryQualifier =   CADGeometryQualifier_enum( unaryOperations.begin()->GeometryQualifier());
	//}

	// Set Operation
	// zzzz set operation xml should change, one set operation per <Geometry>

	vector<AssemblyInterface::SetOperation> setOperations = in_XML_Geometry.SetOperation_kind_children();

	if ( setOperations.size() > 0 )
	{
		out_Struct_Geometry.setOperationDefined = true;
		for ( vector<AssemblyInterface::SetOperation>::const_iterator so(setOperations.begin()); so != setOperations.end(); ++ so )
		{
			AnalysisGeometrySetOperation  analysisGeometrySetOperation;
			analysisGeometrySetOperation.feature_1_ID =  so->Feature1_ID();
			analysisGeometrySetOperation.feature_2_ID =  so->Feature2_ID();
			analysisGeometrySetOperation.feature_1_PrimaryGeometryQualifier = CADPrimaryGeometryQualifier_enum( so->Feature1_GeometryQualifier());
			analysisGeometrySetOperation.feature_2_PrimaryGeometryQualifier = CADPrimaryGeometryQualifier_enum( so->Feature2_GeometryQualifier());

			//analysisGeometrySetOperation.feature_1_SecondaryGeometryQualifier = CADSecondaryGeometryQualifier_enum( so->f);
			//analysisGeometrySetOperation.feature_2_SecondaryGeometryQualifier = CADSecondaryGeometryQualifier_enum( so->Feature2_GeometryQualifier());
			out_Struct_Geometry.setOperations.push_back(analysisGeometrySetOperation);
		}
	}
	
}
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////		
void PopulateMetricsInformation(	const vector<AssemblyInterface::Metrics> &in_Metrics_FromXMLFile,
									list<CADComputation> &in_out_Metrics_ToInternalSturctures )
{
	

	for each ( const AssemblyInterface::Metrics j in in_Metrics_FromXMLFile)
	{
		vector<AssemblyInterface::Metric> metric = j.Metric_kind_children();
		for each ( const AssemblyInterface::Metric k in metric)
		{
			// Temporarily skip Modes and Dynamics, this is currently only being done
			// by model-based FEA
			std::string ComputationType_temp = isis::ConvertToUpperCase(k.MetricType());		
			if ( ComputationType_temp ==  "MODES"  ||  ComputationType_temp ==  "DYNAMICS" ) 
			{
				isis_LOG(lg, isis_FILE, isis_WARN) << "WARNING: Computation type " << std::string(k.MetricType()) << " ignored.  This type is currently not supported.";
				continue;
			}

			// zzzz should check if k.ComponentID() exists in the map.  
			// it might not exist in the map at this point,  if not check later
			CADComputation cADComputation( k.ComponentID());
			cADComputation.computationType =  ComputationType_enum(k.MetricType());
			cADComputation.computationDimension = ComputationDimension_enum(k.RequestedValueType());
			cADComputation.metricID = k.MetricID();
			cADComputation.datumName = k.Details();
			cADComputation.metricName = k.MetricName();
			in_out_Metrics_ToInternalSturctures.push_back(cADComputation);			
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////		
void Log_MetricsInformation_SingleMetricType( const vector<AssemblyInterface::Metrics> &in_Metrics_FromXMLFile )
{
	
	for each ( const AssemblyInterface::Metrics j in in_Metrics_FromXMLFile)
	{
		vector<AssemblyInterface::Metric> metric = j.Metric_kind_children();
		for each ( const AssemblyInterface::Metric k in metric)
		{
			isis_LOG(lg, isis_FILE, isis_INFO) <<  "   ComponentID:        " << std::string( k.ComponentID());
			isis_LOG(lg, isis_FILE, isis_INFO) <<  "   MetricID:           " << std::string(k.MetricID());
			isis_LOG(lg, isis_FILE, isis_INFO) <<  "   MetricType:         " << std::string(k.MetricType());
			isis_LOG(lg, isis_FILE, isis_INFO) <<  "   RequestedValueType: " << std::string(k.RequestedValueType());
			isis_LOG(lg, isis_FILE, isis_INFO) <<  "   Details:            " << std::string(k.Details());	
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////

void Log_MetricsInformation_AllMetricTypes( const vector<AssemblyInterface::Analyses> &in_AnalysesVector )
{
	
	// Log Metrics
	isis_LOG(lg, isis_FILE, isis_INFO) << "Begin Metrics Information:";

	for each ( AssemblyInterface::Analyses i in in_AnalysesVector )
	{
		// Static
		isis_LOG(lg, isis_FILE, isis_INFO) << "Metrics Static Information:";
		vector<AssemblyInterface::Static> staticVector = i.Static_kind_children();
		for each ( AssemblyInterface::Static j in staticVector ) 
		{
			vector<AssemblyInterface::Metrics> metricsVector = j.Metrics_kind_children();
			Log_MetricsInformation_SingleMetricType( metricsVector );
		}
		// Ballistic
		isis_LOG(lg, isis_FILE, isis_INFO) << "Metrics Ballistic Information:";
		vector<AssemblyInterface::Ballistic> BallisticVector = i.Ballistic_kind_children();
		for each ( AssemblyInterface::Ballistic j in BallisticVector ) 
		{
			vector<AssemblyInterface::Metrics> metricsVector = j.Metrics_kind_children();
			Log_MetricsInformation_SingleMetricType(	metricsVector );
		}
		// Blast
		isis_LOG(lg, isis_FILE, isis_INFO) << "Metrics Blast Information:";
		vector<AssemblyInterface::Blast> blastVector = i.Blast_kind_children();
		for each ( AssemblyInterface::Blast j in blastVector ) 
		{
			vector<AssemblyInterface::Metrics> metricsVector = j.Metrics_kind_children();
			Log_MetricsInformation_SingleMetricType(	metricsVector );
		}
		// CFD
		isis_LOG(lg, isis_FILE, isis_INFO) << "Metrics CFD Information:";
		vector<AssemblyInterface::CFD> cFDVector = i.CFD_kind_children();
		for each ( AssemblyInterface::CFD j in cFDVector ) 
		{
			vector<AssemblyInterface::Metrics> metricsVector = j.Metrics_kind_children();
			Log_MetricsInformation_SingleMetricType(	metricsVector );
		}
	}
	isis_LOG(lg, isis_FILE, isis_INFO) << "End Metrics Information";
}
//////////////////////////////////////////////////////////////////////////////////////////////////////						
void PopulateAnalyses (	 const AssemblyInterface::Analyses &in_Analyses_FromXML,
						 CADAnalyses				  &in_out_Analyses_CAD )
													throw (isis::application_exception)
{
	////////////////////////////////////
	// CFD Analysis
	////////////////////////////////////
	vector<AssemblyInterface::CFD> cFDVector = in_Analyses_FromXML.CFD_kind_children();
	for each ( const AssemblyInterface::CFD i in cFDVector )
	{
		AnalysisCFD temp_AnalysisCFD;
		/////////////////
		// Metrics
		/////////////////

		// Metrics
		vector<AssemblyInterface::Metrics> metrics = i.Metrics_kind_children();
		PopulateMetricsInformation(	metrics, temp_AnalysisCFD.metrics );

		// Set reference plane type
		temp_AnalysisCFD.referencePlane = PLANE_WATERLINE;  // Only support waterline for now.

		// Add HydrostaticsSolverSettings
		vector<AssemblyInterface::HydrostaticsSolverSettings> hydrostaticsSolverSettings = i.HydrostaticsSolverSettings_kind_children();

		if ( hydrostaticsSolverSettings.size() > 1 )
		{
			// throw exception here.  One and only one HydrostaticsSolverSettings allowed.
			std::stringstream errorString;
			errorString <<
			"Function PopulateAnalyses recieved from the input xml " <<  hydrostaticsSolverSettings.size() << 
			"  HydrostaticsSolverSettings XML tags/structures.  There must zero or one hydrostaticsSolverSettings XML tag/structure.";

			throw isis::application_exception(errorString.str());
		}

		// At this point there can be 0 or 1 hydrostaticsSolverSettings;
		if ( hydrostaticsSolverSettings.size() == 1 )
		{
			temp_AnalysisCFD.cADHydrostaticParameters_exist = true;
			for each ( AssemblyInterface::HydrostaticsSolverSettings j in hydrostaticsSolverSettings )
			{
				temp_AnalysisCFD.cADHydrostaticParameters.pitchAngleStart	  = j.PitchAngleStart();		// Degrees
				temp_AnalysisCFD.cADHydrostaticParameters.pitchAngleEnd		  = j.PitchAngleEnd();			// Degrees
				temp_AnalysisCFD.cADHydrostaticParameters.pitchAngleStepSize  = j.PitchAngleStepSize();		// Degrees

				temp_AnalysisCFD.cADHydrostaticParameters.rollAngleStart		  = j.RollAngleStart();			// Degrees
				temp_AnalysisCFD.cADHydrostaticParameters.rollAngleEnd		  = j.RollAngleEnd();			// Degrees
				temp_AnalysisCFD.cADHydrostaticParameters.rollAngleStepSize	  = j.RollAngleStepSize();		// Degrees

				temp_AnalysisCFD.cADHydrostaticParameters.fluidMaterial		  = j.FluidMaterial();			// material library entry 
				temp_AnalysisCFD.cADHydrostaticParameters.fluidTemperature    = j.FluidTemperature();		// Celsius only

			}
		}
		else
		{   // No hydrostaticsSolverSettings values
			temp_AnalysisCFD.cADHydrostaticParameters_exist = false;
		}

		in_out_Analyses_CAD.analysesCFD.push_back(temp_AnalysisCFD);
	}

	////////////////////////////////////
	// Ballistic Analysis
	////////////////////////////////////
	vector<AssemblyInterface::Ballistic> ballisticVector = in_Analyses_FromXML.Ballistic_kind_children();
	for each ( const AssemblyInterface::Ballistic i in ballisticVector )
	{
		AnalysisBallistic temp_AnalysisBallistic;
		// Shotlines
		vector<AssemblyInterface::Shotlines> shotlinesVector = i.Shotlines_kind_children();
		for each ( const AssemblyInterface::Shotlines j in shotlinesVector)
		{
			vector<AssemblyInterface::Shotline> shotlineVector = j.Shotline_kind_children();
			for each ( const AssemblyInterface::Shotline k in shotlineVector )
			{
				Shotline temp_Shotline;
				//temp_Shotline.name = "Custom";
				temp_Shotline.name = k.ShotlineName();
				temp_Shotline.ballisticThreatRef =	k.BallisticThreatRef();
				temp_Shotline.datumPoint_ComponentID = k.ComponentID();
				temp_Shotline.datumPoint =			k.DatumPoint();
				temp_Shotline.azimuth =				k.Azimuth();
				temp_Shotline.elevation =			k.Elevation();
				temp_AnalysisBallistic.shotlines.push_back(temp_Shotline);
			}
		}
		// Reference Planes
		//std::cout << std::endl << "*************** Reference Plane *************";
		vector<AssemblyInterface::ReferencePlane> referencePlaneVector = i.ReferencePlane_kind_children();
		for each ( const AssemblyInterface::ReferencePlane j in referencePlaneVector)
		{
			temp_AnalysisBallistic.referencePlanes.push_back( ReferencePlane_enum(j.Type()));
			//std::cout << std::endl << "***** ReferencePlane_enum(j.Type()" <<  ReferencePlane_enum(j.Type());
		}

		/////////////////
		// Metrics
		/////////////////
		vector<AssemblyInterface::Metrics> metrics = i.Metrics_kind_children();
		PopulateMetricsInformation(	metrics, temp_AnalysisBallistic.metrics );

		in_out_Analyses_CAD.analysesBallistic.push_back(temp_AnalysisBallistic);
	}

	////////////////////////////////////
	// Blast Analysis
	////////////////////////////////////
	vector<AssemblyInterface::Blast> BlastVector = in_Analyses_FromXML.Blast_kind_children();
	for each ( const AssemblyInterface::Blast i in BlastVector )
	{
		AnalysisBlast temp_AnalysisBlast;

		// Reference Planes
		//std::cout << std::endl << "*************** Reference Plane *************";
		vector<AssemblyInterface::ReferencePlane> referencePlaneVector = i.ReferencePlane_kind_children();
		for each ( const AssemblyInterface::ReferencePlane j in referencePlaneVector)
		{
			temp_AnalysisBlast.referencePlanes.push_back( ReferencePlane_enum(j.Type()));
			//std::cout << std::endl << "***** ReferencePlane_enum(j.Type()" <<  ReferencePlane_enum(j.Type());
		}

		/////////////////
		// Metrics
		/////////////////
		vector<AssemblyInterface::Metrics> metrics = i.Metrics_kind_children();
		PopulateMetricsInformation(	metrics, temp_AnalysisBlast.metrics );

		in_out_Analyses_CAD.analysesBlast.push_back(temp_AnalysisBlast);
	}

	//////////////////////////////////////////////
	// Interference Analysis, Deprecated Approach
	//////////////////////////////////////////////
	// 1/26/2015, Specifying an interference analysis via the following xml has been deprecated
	//    <Interference AnalysisID="id-0065-00000054" _id="id99349157"/>
	// Now an inference analyis is specified via
	//    <Metric ComponentID="a57a4f72-263f-400a-964c-bc0148340328|1" Details="" MetricID="id-0067-0000044d" MetricType="InterferenceCount" RequestedValueType="Boolean" _id="id15599"/>	
	
	//vector<AssemblyInterface::Interference> InterferenceVector = in_Analyses_FromXML.Interference_kind_children();
	//if ( InterferenceVector.size() > 0 ) in_out_Analyses_CAD.interference = true;
	
	////////////////////////////////////
	// FEA Analysis
	////////////////////////////////////
	vector<AssemblyInterface::FEA> feaVector = in_Analyses_FromXML.FEA_kind_children();
	for ( vector<AssemblyInterface::FEA>::const_iterator ci = feaVector.begin(); ci != feaVector.end(); ci++ )
	{
		AnalysisFEA  analysisFEA;

		analysisFEA.ID = ci->AnalysisID();
		analysisFEA.type = isis::AnalysisType_enum(ci->Type());
		AssemblyInterface::AnalysisConstraints analysisConstraints = ci->AnalysisConstraints_child();
		
		////////////////////////////////
		// Constraints
		////////////////////////////////
		// Constraint		Allowed Geometry Types	
		// ------------		----------------------
		// Displacement		Polygon, Circle, Concentric_Circles, Extrusion, Cylinder, Sphere
		// Pin				Cylinder
		// Ball				Sphere

		if ( analysisConstraints != Udm::null)
		{
			vector<AssemblyInterface::AnalysisConstraint> constraintVector = analysisConstraints.AnalysisConstraint_kind_children();
			for ( vector<AssemblyInterface::AnalysisConstraint>::const_iterator cj = constraintVector.begin(); cj != constraintVector.end(); cj++)
			{
				isis::AnalysisConstraint analysisConstraint;
				//////////////
				// Geometry
				//////////////
				vector<AssemblyInterface::Geometry> geometryVector = cj->Geometry_kind_children();
				//AssemblyInterface::Geometry geometry = cj->Geometry_child();

				//PopulateGeometry( geometry, analysisConstraint.geometry );
				PopulateGeometry( geometryVector, analysisConstraint.geometry );
				
				/*
				PopulateGeometry ( const AssemblyInterface::Geometry &in_XML_Geometry, AnalysisGeometry	&out_Struct_Geometry )


				vector<AssemblyInterface::Features> featuresVector = geometry.Features_kind_children();

				for ( vector<AssemblyInterface::Features>::const_iterator ck = featuresVector.begin(); ck != featuresVector.end(); ck++ )
				{
						AnalysisGeometryFeature  analysisGeometryFeature;

						analysisGeometryFeature.featureID = ck->FeatureID();
						analysisGeometryFeature.geometryType = CADGeometryType_enum(ck->GeometryType());
					    analysisGeometryFeature.componentID = ck->ComponentID();
						analysisGeometryFeature.featureInterfaceType = FeatureInterfaceType_enum(ck->FeatureInterfaceType());
						analysisGeometryFeature.featureGeometryType = CADAnalysisFeatureGeometryType_enum(ck->FeatureGeometryType());
						
						analysisConstraint.geometry.features.push_back(analysisGeometryFeature);
				}

				// Uniary Operation
				vector<AssemblyInterface::UnaryOperation> unaryOperations = geometry.UnaryOperation_kind_children();

				if ( unaryOperations.size() > 0 )
				{
					analysisConstraint.geometry.unaryOperationCount = unaryOperations.size();
					analysisConstraint.geometry.unaryOperationDefined = true;
					analysisConstraint.geometry.unaryGeometryQualifier =   CADGeometryQualifier_enum( unaryOperations.begin()->GeometryQualifier());
				}

				// Set Operation
				vector<AssemblyInterface::SetOperation> setOperations = geometry.SetOperation_kind_children();

				if ( setOperations.size() > 0 )
				{
					analysisConstraint.geometry.setOperationDefined = true;
					for ( vector<AssemblyInterface::SetOperation>::const_iterator so(setOperations.begin()); so != setOperations.end(); ++ so )
					{
						AnalysisGeometrySetOperation  analysisGeometrySetOperation;
						analysisGeometrySetOperation.feature_1_ID =  so->Feature1_ID();
						analysisGeometrySetOperation.feature_2_ID =  so->Feature2_ID();
						analysisGeometrySetOperation.feature_1_GeometryQualifier = CADGeometryQualifier_enum( so->Feature1_GeometryQualifier());
						analysisGeometrySetOperation.feature_2_GeometryQualifier = CADGeometryQualifier_enum( so->Feature2_GeometryQualifier());
						analysisConstraint.geometry.setOperations.push_back(analysisGeometrySetOperation);
					}
				}
				*/
				//////////////////////////////
				// Constraint - Displacement 
				//////////////////////////////
				AssemblyInterface::ConstraintType constraintType = cj->ConstraintType_child();
				if (constraintType != Udm::null)
				{
					Uml::Class type = constraintType.type();
					if ( type == AssemblyInterface::Displacement::meta)
					{
						AssemblyInterface::Displacement displacement = AssemblyInterface::Displacement::Cast(constraintType);
						try
						{
							analysisConstraint.analysisDisplacementDefined = true;
						
							AssemblyInterface::Translation translation = displacement.Translation_child();
							analysisConstraint.displacement.translation.units = CADUnitsDistance_enum(translation.Units());

							if (IsANumber(translation.x() ))
							{
								analysisConstraint.displacement.translation.x = ConvertToDouble(translation.x());
								analysisConstraint.displacement.translation.x_FixedFree = ANALYSIS_CONSTRAINT_SCALAR;
							}
							else
							{
								analysisConstraint.displacement.translation.x = 0.0;
								analysisConstraint.displacement.translation.x_FixedFree = AnalysisConstraintFixedFree_enum(translation.x());
							}

							if (IsANumber(translation.y() ))
							{
								analysisConstraint.displacement.translation.y = ConvertToDouble(translation.y());
								analysisConstraint.displacement.translation.y_FixedFree = ANALYSIS_CONSTRAINT_SCALAR;
							}
							else
							{
								analysisConstraint.displacement.translation.y = 0.0;
								analysisConstraint.displacement.translation.y_FixedFree = AnalysisConstraintFixedFree_enum(translation.y());
							}

							if (IsANumber(translation.z() ))
							{
								analysisConstraint.displacement.translation.z = ConvertToDouble(translation.z());
								analysisConstraint.displacement.translation.z_FixedFree = ANALYSIS_CONSTRAINT_SCALAR;
							}
							else
							{
								analysisConstraint.displacement.translation.z = 0.0;
								analysisConstraint.displacement.translation.z_FixedFree = AnalysisConstraintFixedFree_enum(translation.z());
							}


							AssemblyInterface::Rotation rotation = displacement.Rotation_child();
							analysisConstraint.displacement.rotation.units = CADUnitsAngle_enum(rotation.Units());

							if (IsANumber(rotation.x() ))
							{
								analysisConstraint.displacement.rotation.alpha = ConvertToDouble(rotation.x());
								analysisConstraint.displacement.rotation.alpha_FixedFree = ANALYSIS_CONSTRAINT_SCALAR;
							}
							else
							{
								analysisConstraint.displacement.rotation.alpha = 0.0;
								analysisConstraint.displacement.rotation.alpha_FixedFree = AnalysisConstraintFixedFree_enum(rotation.x());
							}

							if (IsANumber(rotation.y() ))
							{
								analysisConstraint.displacement.rotation.beta = ConvertToDouble(rotation.y());
								analysisConstraint.displacement.rotation.beta_FixedFree = ANALYSIS_CONSTRAINT_SCALAR;
							}
							else
							{
								analysisConstraint.displacement.rotation.beta = 0.0;
								analysisConstraint.displacement.rotation.beta_FixedFree = AnalysisConstraintFixedFree_enum(rotation.y());
							}

							if (IsANumber(rotation.z() ))
							{
								analysisConstraint.displacement.rotation.gamma = ConvertToDouble(rotation.z());
								analysisConstraint.displacement.rotation.gamma_FixedFree = ANALYSIS_CONSTRAINT_SCALAR;
							}
							else
							{
								analysisConstraint.displacement.rotation.gamma = 0.0;
								analysisConstraint.displacement.rotation.gamma_FixedFree = AnalysisConstraintFixedFree_enum(rotation.z());
							}

						}
						catch (isis::application_exception &ex )
						{
							std::string TempError =   "Function PopulateAnalyses failed to convert displacement constraint data to internal representation. Exception: " + 
													 std::string(ex.what()); 
							throw isis::application_exception(TempError.c_str());
						}
					}
					else if ( type == AssemblyInterface::Pin::meta )					// Constraint - Pin 
					{
						AssemblyInterface::Pin pin = AssemblyInterface::Pin::Cast(constraintType);
						try
						{
							analysisConstraint.analysisPinDefined = true;
							AssemblyInterface::AxialRotation axialRotation = pin.AxialRotation_child();
							analysisConstraint.pin.axialRotationConstraint =     AnalysisConstraintFixedFree_enum(axialRotation.Property());
							AssemblyInterface::AxialDisplacement axialDisplacement = pin.AxialDisplacement_child();
							analysisConstraint.pin.axialDisplacementConstraint = AnalysisConstraintFixedFree_enum(axialDisplacement.Property());
						}
						catch (isis::application_exception &ex )
						{
							std::string TempError = "Function PopulateAnalyses, failed to convert pin constraint data to internal representation. Exception: " + 
													 std::string(ex.what()); 
							throw isis::application_exception(TempError.c_str());
						}
					}
					else if ( type == AssemblyInterface::Ball::meta)				// Constraint - Ball 
					{
						// Ball constraint does not have any freedoms constrained.  We
						// simply need to know if this type constriant is a ball constraint.
						analysisConstraint.analysisBallDefined = true;
					}
				}

				analysisFEA.analysisConstraints.push_back(analysisConstraint);
			}
		}

		////////////////////////////////
		// Loads
		////////////////////////////////
		// Load				Allowed Geometry Types	
		// ------------		----------------------
		// Force			Polygon, Circle, Concentric_Circles, Extrusion
		// Pressure			Polygon, Circle, Concentric_Circles, Cylinder_Surface(To be added),Sphere_Surface (To be added) 
		// Acceleration     N/A
		AssemblyInterface::Loads loads = ci->Loads_child();
		if ( loads != Udm::null )
		{
			vector<AssemblyInterface::Load> loadVector = loads.Load_kind_children();
			for ( vector<AssemblyInterface::Load>::const_iterator cj = loadVector.begin(); cj != loadVector.end(); cj++ )
			{
				isis::AnalysisLoad analysisLoad;

				//////////////
				// Geometry
				//////////////
				//AssemblyInterface::Geometry geometry = cj->Geometry_child();

				//PopulateGeometry( geometry, analysisLoad.geometry );

				vector<AssemblyInterface::Geometry> geometryVector = cj->Geometry_kind_children();

				PopulateGeometry( geometryVector, analysisLoad.geometry );

				/*
				AssemblyInterface::Features features = cj->Features_child();
				if ( features != Udm::null )
				{
				
					analysisLoad.geometry.geometryType = CADGeometryType_enum(features.GeometryType());
					analysisLoad.geometry.componentID = features.ComponentID();
					analysisLoad.geometry.featureInterfaceType = FeatureInterfaceType_enum(features.FeatureInterfaceType());
					analysisLoad.geometry.featureGeometryType =  CADAnalysisFeatureGeometryType_enum(features.FeatureGeometryType());

					vector<AssemblyInterface::Feature> featureVector = features.Feature_kind_children();
					for ( vector<AssemblyInterface::Feature>::const_iterator ck = featureVector.begin(); ck != featureVector.end(); ck++)
					{
						analysisLoad.geometry.features.push_back( ck->Name());
					}
				}
				*/
				AssemblyInterface::LoadType loadType = cj->LoadType_child();
				Uml::Class type = loadType.type();
				// Force, Moment
				if ( type == AssemblyInterface::ForceMoment::meta )
				{
					AssemblyInterface::ForceMoment forceMoment = AssemblyInterface::ForceMoment::Cast(loadType);
					try
					{
						// Force
						analysisLoad.forceDefined = true;

						AssemblyInterface::Force force = forceMoment.Force_child();
						analysisLoad.force.force.units = CADUnitsForce_enum( force.Units());
						analysisLoad.force.force.x = force.x();
						analysisLoad.force.force.y = force.y();
						analysisLoad.force.force.z = force.z();

						// Moment
						AssemblyInterface::Moment moment = forceMoment.Moment_child();
						analysisLoad.force.moment.units = CADUnitsMoment_enum( moment.Units());
						analysisLoad.force.moment.x =   moment.x();
						analysisLoad.force.moment.y =	moment.y();
						analysisLoad.force.moment.z =	moment.z();
					}
					catch (isis::application_exception& ex)
					{
						std::string TempError = "Function PopulateAnalyses, failed to convert force/moment data to internal representation. Exception: " + 
							                     std::string(ex.what()); 
						throw isis::application_exception(TempError.c_str());
					}

				}
				else if (type == AssemblyInterface::Pressure::meta)		// Pressure
				{
					AssemblyInterface::Pressure pressure = AssemblyInterface::Pressure::Cast(loadType);
					try
					{
						analysisLoad.pressureDefined = true;
						analysisLoad.pressure.units = CADUnitsPressure_enum( pressure.Units());
						analysisLoad.pressure.value = pressure.Value();
					}
					catch (isis::application_exception& ex)
					{
						std::string TempError = "Function PopulateAnalyses, failed to convert pressure data to internal representation. Exception: " + 
							                     std::string(ex.what()); 
						throw isis::application_exception(TempError.c_str());
					}

				}
				else if (type == AssemblyInterface::Acceleration::meta)				// Acceleration
				{
					AssemblyInterface::Acceleration acceleration = AssemblyInterface::Acceleration::Cast(loadType);
					try
					{
						analysisLoad.accelerationDefined = true;
						analysisLoad.acceleration.units = CADUnitsAcceleration_enum( acceleration.Units());
						//analysisLoad.acceleration.magnitude = j->Acceleration().get().Value();
						analysisLoad.acceleration.direction.x = acceleration.x();
						analysisLoad.acceleration.direction.y = acceleration.y();
						analysisLoad.acceleration.direction.z = acceleration.z();
					}
					catch (isis::application_exception& ex)
					{
						std::string TempError = "Function PopulateAnalyses, failed to convert acceleration data to internal representation. Exception: " + 
							                     std::string(ex.what()); 
						throw isis::application_exception(TempError.c_str());
					}
				}

				analysisFEA.analysisLoads.push_back(analysisLoad);
			}  // End for ( AssemblyType::Analyses_type::FEA_type::Loads_type::Load_const_iterator 
	
		}

		////////////////////////////////
		// ThermalElements
		////////////////////////////////
		// Load						Allowed Geometry Types	
		// ------------				----------------------
		// SpecifiedTemperature		Polygon, Circle, Concentric_Circles, Cylinder_Surface(To be added),Sphere_Surface (To be added) 
		// InitialTemperature		None, or Polygon, Circle, Concentric_Circles, Cylinder_Surface(To be added),Sphere_Surface (To be added) 
		// HeatFlux					Polygon, Circle, Concentric_Circles, Cylinder_Surface(To be added),Sphere_Surface (To be added) 
		// HeatGeneration           Not Supported
		//
		// Constraint				Allowed Geometry Types	
		// ------------				----------------------
		// ConvectionHeat			Polygon, Circle, Concentric_Circles, Cylinder_Surface(To be added),Sphere_Surface (To be added) 

		AssemblyInterface::ThermalElements thermalElements = ci->ThermalElements_child();

		if ( thermalElements != Udm::null )
		{
			vector<AssemblyInterface::ThermalElement> thermalElementVector = thermalElements.ThermalElement_kind_children();

			for ( vector<AssemblyInterface::ThermalElement>::const_iterator therm_i = thermalElementVector.begin(); therm_i != thermalElementVector.end(); therm_i++ )
			{
				bool analysisLoad_defined		= false;
				bool analysisConstraint_defined = false;
				
				e_Thermal_LoadConstraint thermalLoadConstraint = Thermal_LoadConstraint_enum(therm_i->LoadType());

				isis::AnalysisConstraint	analysisConstraint;
				isis::AnalysisLoad			analysisLoad;

				vector<AssemblyInterface::Geometry> geometryVector = therm_i->Geometry_kind_children();
				vector<AssemblyInterface::Component> componentVector = therm_i->Component_kind_children();

				switch ( thermalLoadConstraint )
				{
					case THERMAL_CONVECTION:			// Constraint in Nastran terms
						analysisConstraint.convectionBoundary.convectionCoefficient = therm_i->Value();
						analysisConstraint.convectionBoundary.unit = therm_i->Unit();
						analysisConstraint.convectionBoundaryDefined = true;
						PopulateGeometry( geometryVector, analysisConstraint.geometry );	
						analysisFEA.analysisConstraints.push_back(analysisConstraint);
						break;
					case THERMAL_HEAT_FLUX:				// Load
						analysisLoad.heatFlux.value = therm_i->Value();
						analysisLoad.heatFlux.unit = therm_i->Unit();
						analysisLoad.heatFluxDefined = true;
						PopulateGeometry( geometryVector, analysisLoad.geometry );
						analysisFEA.analysisLoads.push_back(analysisLoad);
						break;
					case THERMAL_HEAT_GENERATION:		// Load
						analysisLoad.heatGeneration.value = therm_i->Value();
						analysisLoad.heatGeneration.unit = therm_i->Unit();
						analysisLoad.heatGenerationDefined = true;
						// There should be no geometry for heat generation.  Heat generation of an entire
						// part is supported, but not heat generation on features of a part.
						// Leave the call to PopulateGeometry and later in ValidateFEAAnalysisInputs throw
						// and exception if any geometry was actually populated.
						PopulateGeometry( geometryVector, analysisLoad.geometry );
						// Add the component instance IDs
						for each ( const AssemblyInterface::Component &i_comp in componentVector ) 
							analysisLoad.geometry.GeometryPerEntireComponent_ComponentInstanceIDs.push_back(i_comp.ComponentID());
						analysisFEA.analysisLoads.push_back(analysisLoad);
						break;
					case THERMAL_INITIAL_TEMPERATURE:	// Load
						// Thermal-initial-temperature is currently not supported.
						// Will read the values and throw an exception via  ValidateFEAAnalysisInputs
						// if thermal-initial-temperature is present in CADAssmebly.xml
						analysisLoad.gridPointInitialTemperature.value = therm_i->Value();
						analysisLoad.gridPointInitialTemperatureDefined = true;
						analysisLoad.gridPointInitialTemperature.unit = therm_i->Unit(); 
						PopulateGeometry( geometryVector, analysisLoad.geometry );
						analysisFEA.analysisLoads.push_back(analysisLoad);
						break;
					case THERMAL_SPECIFIED_TEMPERATURE:	// Load
						analysisLoad.gridPointTemperature.value = therm_i->Value();
						analysisLoad.gridPointTemperature.unit = therm_i->Unit(); 
						analysisLoad.gridPointTemperatureDefined = true;
						PopulateGeometry( geometryVector, analysisLoad.geometry );
						analysisFEA.analysisLoads.push_back(analysisLoad);
						break;
					case THERMAL_AMBIENT_TEMPERATURE:
						// Note - an AmbientTemperature in CADAssembly.xml has a "<Component ComponentID="f5..."; however,
						// the Component is just the test injection point component.  It really has nothing to do
						// with the ambient temperatrue.  Therefore, the <Component structure will not be read here.
						analysisLoad.ambientTemperature.value = therm_i->Value();
						analysisLoad.ambientTemperature.unit = therm_i->Unit();
						analysisLoad.ambientTemperatureDefined = true;
						analysisFEA.analysisLoads.push_back(analysisLoad);
						break;  
					default:
						std::string TempError = "Function PopulateAnalyses, unknown thermal analysis type: " + 
												Thermal_LoadConstraint_string(thermalLoadConstraint);
						throw isis::application_exception(TempError.c_str());
				}
			}
		}
		////////////////////////////////
		// Part Interfaces
		////////////////////////////////
		AssemblyInterface::PartInterfaces partInterfaces = ci->PartInterfaces_child();
		if ( partInterfaces != Udm::null )
		{
			vector<AssemblyInterface::PartInterface> piVector = partInterfaces.PartInterface_kind_children();
			for ( vector<AssemblyInterface::PartInterface>::const_iterator cj = piVector.begin(); cj != piVector.end(); cj++ )
			{
				AnalysisPartInterface analysisPartInterface;
				analysisPartInterface.type = AnalysisPartInterfaceType_enum(cj->Type());

				analysisPartInterface.componentID_1 = cj->ComponentID_1();
				analysisPartInterface.componentID_2 = cj->ComponentID_2();

				analysisFEA.analysisPartInterfaces.push_back(analysisPartInterface);
			}
		}

		////////////////////////////////
		// Solvers
		////////////////////////////////
		AssemblyInterface::Solvers solvers = ci->Solvers_child();
		if (solvers != Udm::null)
		{
			vector<AssemblyInterface::Solver> solverVector = solvers.Solver_kind_children();
			for ( vector<AssemblyInterface::Solver>::const_iterator cj = solverVector.begin(); cj != solverVector.end(); cj++ )
			{
				AnalysisSolver analysisSolver;
				analysisSolver.type				= CADAnalysisSolverType_enum( cj->Type());
				analysisSolver.analysisSolutionType = AnalysisSolutionType_enum( cj->Type()); // ANALYSIS_DECK_BASED, or ANALYSIS_MODEL_BASED
				analysisSolver.meshType			= CADAnalysisMeshType_enum( cj->MeshType());
				analysisSolver.shellElementType	= CADAnalysisShellElementType_enum( cj->ShellElementType() );
				analysisSolver.elementShapeType = CADAnalysisElementShapeType_enum( cj->ElementShapeType() );

				analysisFEA.analysisSolvers.push_back(analysisSolver);
			}
		}

		////////////////////////////////
		// Analysis Components   8/20/2013, This is redundant information.  Could remove and just use metrics.
		////////////////////////////////
		//AssemblyInterface::AnalysisComponents analysisComponents = ci->AnalysisComponents_child();
		//if ( analysisComponents != Udm::null )  // if not present, assume stress in all components should be returned
		//{
		//	vector<AssemblyInterface::Component> componentVector = analysisComponents.Component_kind_children();
		//	for (  vector<AssemblyInterface::Component>::const_iterator cj = componentVector.begin(); 
		//		   cj != componentVector.end(); cj++ )
		//	{
		//		CADAnalysisComponent temp_CADAnalysisComponent;
		//		temp_CADAnalysisComponent.componentID = cj->ComponentID();
		//		temp_CADAnalysisComponent.infiniteCycle = cj->InfiniteCycle();
		//			 
		//		analysisFEA.cADAnalysisComponents.push_back(temp_CADAnalysisComponent);
		//	}
		//
		//}

		////////////////////////////////
		// Metrics
		////////////////////////////////
		vector<AssemblyInterface::Metrics> metrics = ci->Metrics_kind_children();
		PopulateMetricsInformation(	metrics, analysisFEA.metrics );
	
		////////////////////////////////
		// Push back analysisFEA
		////////////////////////////////
		in_out_Analyses_CAD.analysesFEA.push_back(analysisFEA);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
void PopulateProEAssemblyStructures (	AssemblyInterface::Assemblies    &in_Assemblies_ptr,
										CADAssemblies					 &out_Assemblies,
										std::map<std::string, CADComponentData> &out_CADComponentData_map )
													throw (isis::application_exception)
{
	vector<AssemblyInterface::Assembly> assemblyVector = in_Assemblies_ptr.Assembly_kind_children();
	for (vector<AssemblyInterface::Assembly>::iterator i = assemblyVector.begin(); i != assemblyVector.end(); i++)
	{
		
		AssemblyInterface::Assembly assembly(*i);
		AssemblyInterface::CADComponent cadComponent = assembly.CADComponent_child();
		if (cadComponent != Udm::null)
		{
			TopLevelAssemblyData topLevelAssemblyData( cadComponent.ComponentID(), assembly.ConfigurationID() );

			////////////////////////////////
			// Metrics
			////////////////////////////////
			// Metrics, Top level assembly may have  <Static AnalysisID="...
			vector<AssemblyInterface::Analyses> analysesVector = i->Analyses_kind_children();
			for each ( AssemblyInterface::Analyses ii in analysesVector )
			{
				vector<AssemblyInterface::Static> staticVector = ii.Static_kind_children();
				for each ( AssemblyInterface::Static ij in staticVector ) 
				{
					vector<AssemblyInterface::Metrics> metricsVector = ij.Metrics_kind_children();
					PopulateMetricsInformation(	metricsVector, topLevelAssemblyData.assemblyMetrics );
				}
			}

			AssemblyInterface::Analyses analyses = assembly.Analyses_child();
			if ( analyses != Udm::null)
			{
				PopulateAnalyses( analyses, topLevelAssemblyData.analysesCAD );
			}

			//out_Assemblies.topLevelAssemblies.push_back( topLevelAssemblyData);			

			PopulateCADComponentStructure(  cadComponent, "", out_CADComponentData_map );

			/* MMMMMMMMMMMMMM Metrics need to replcate elsewhere
			AssemblyInterface::Computations computations = assembly.Computations_child();

			/////////////////////
			// Computations
			/////////////////////

			if ( computations != Udm::null)
			{
				////////////////////////
				// Bounding Box
				////////////////////////
				vector<AssemblyInterface::BoundingBox> boundingBoxs = computations.BoundingBox_kind_children();

				for ( vector<AssemblyInterface::BoundingBox>::const_iterator j(boundingBoxs.begin());
					  j != 	boundingBoxs.end(); ++j )
				{
					std::string componentID = j->ComponentID();

					if (  out_CADComponentData_map.find(componentID) == out_CADComponentData_map.end() )
					{
						std::string TempError = std::string("When parsing the input XML file, encountered a " +
							std::string("<Computations><BoundingBox> with a ComponentID (ID = ") + componentID +
							") that does not exist in the assembly.  " +
							"A BoundingBox component ID must refer to a component in the assembly.");
						throw isis::application_exception(TempError.c_str());
					}

					CADComputation  computation(componentID);

					computation.computationType = COMPUTATION_BOUNDING_BOX;
					computation.computationDimension = ComputationDimension_enum(j->RequestedValueType());
					computation.metricID = j->MetricID();

					topLevelAssemblyData.cADComputations.computations.push_back(computation);					

				} // END for ( vector<AssemblyInterface::BoundingBox>::const


				////////////////////////
				// Mass
				////////////////////////
				vector<AssemblyInterface::Mass> masses = computations.Mass_kind_children();

				for ( vector<AssemblyInterface::Mass>::const_iterator j(masses.begin());
					  j != 	masses.end(); ++j )
				{
					std::string componentID = j->ComponentID();

					if (  out_CADComponentData_map.find(componentID) == out_CADComponentData_map.end() )
					{
						std::string TempError = std::string("When parsing the input XML file, encountered a " +
							std::string("<Computations><Mass> with a ComponentID (ID = ") + componentID +
							") that does not exist in the assembly.  " +
							"A Mass component ID must refer to a component in the assembly.");
						throw isis::application_exception(TempError.c_str());
					}

					CADComputation  computation(componentID);

					computation.computationType = COMPUTATION_MASS;				
					//computation.computationDimension = ComputationDimension_enum(j->RequestedValueType());
					// Set this to none for now.  Will see what CyPhy does for the default.
					computation.computationDimension = COMPUTATION_DIMENSION_NONE;
					computation.metricID = j->MetricID();				
					topLevelAssemblyData.cADComputations.computations.push_back(computation);					

				} // END for ( vector<AssemblyInterface::BoundingBox>::const

				////////////////////////
				// Center of Gravity
				////////////////////////
				vector<AssemblyInterface::CenterOfGravity> CGs = computations.CenterOfGravity_kind_children();

				for ( vector<AssemblyInterface::CenterOfGravity>::const_iterator k(CGs.begin());
					  k != 	CGs.end(); ++k )
				{
					std::string componentID = k->ComponentID();

					if (  out_CADComponentData_map.find(componentID) == out_CADComponentData_map.end() )
					{
						std::string TempError = std::string("When parsing the input XML file, encountered a " +
							std::string("<Computations><CenterOfGravity> with a ComponentID (ID = ") + componentID +
							") that does not exist in the assembly.  " +
							"A CenterOfGravity component ID must refer to a component in the assembly.");
						throw isis::application_exception(TempError.c_str());
					}

					CADComputation  computation(componentID);

					computation.computationType = COMPUTATION_CG;
					computation.computationDimension = ComputationDimension_enum(k->RequestedValueType());
					computation.metricID = k->MetricID();
					topLevelAssemblyData.cADComputations.computations.push_back(computation);					

				} // END for ( vector<AssemblyInterface::CenterOfGravity>::const

				////////////////////////
				// Point Coordinates
				////////////////////////
				vector<AssemblyInterface::PointCoordinates> pointCoordinates = computations.PointCoordinates_kind_children();

				for ( vector<AssemblyInterface::PointCoordinates>::const_iterator l(pointCoordinates.begin());
					  l != 	pointCoordinates.end(); ++l )
				{
					std::string componentID = l->ComponentID();

					if (  out_CADComponentData_map.find(componentID) == out_CADComponentData_map.end() )
					{
						std::string TempError = std::string("When parsing the input XML file, encountered a " +
							std::string("<Computations><PointCoordinates> with a ComponentID (ID = ") + componentID +
							") that does not exist in the assembly.  " +
							"A PointCoordinates component ID must refer to a component in the assembly.");
						throw isis::application_exception(TempError.c_str());
					}

					CADComputation  computation(componentID);

					computation.computationType = COMPUTATION_POINT;
					computation.computationDimension = ComputationDimension_enum(l->RequestedValueType());
					computation.metricID = l->MetricID();
					computation.datumName = l->FeatureDatumName();

					topLevelAssemblyData.cADComputations.computations.push_back(computation);					

				} // END for ( vector<AssemblyInterface::PointCoordinates>::const

				//////////////////////////////
				// Coefficient Of Drag  (CFD)
				//////////////////////////////
				vector<AssemblyInterface::CoefficientOfDrag> coefficientsOfDrag = computations.CoefficientOfDrag_kind_children();

				//for ( vector<AssemblyInterface::CoefficientOfDrag>::const_iterator l(coefficientsOfDrag.begin());
				//	  l != 	coefficientsOfDrag.end(); ++l )
				for each ( const AssemblyInterface::CoefficientOfDrag l in coefficientsOfDrag )
				{
					std::string componentID = l.ComponentID();

					if (  out_CADComponentData_map.find(componentID) == out_CADComponentData_map.end() )
					{
						std::string TempError = std::string("When parsing the input XML file, encountered a " +
							std::string("<Computations><CoefficientOfDrag> with a ComponentID (ID = ") + componentID +
							") that does not exist in the assembly.  " +
							"A CoefficientOfDrag component ID must refer to a component in the assembly.");
						throw isis::application_exception(TempError.c_str());
					}

					CADComputation  computation(componentID);
					computation.computationType = COMPUTATION_COEFFICIENT_OF_DRAG;				
					computation.computationDimension = ComputationDimension_enum(l.RequestedValueType());
					computation.metricID = l.MetricID();

					topLevelAssemblyData.cADComputations.computations.push_back(computation);					

				} // END for ( vector<AssemblyInterface::CoefficientOfDrag>::const

			}	// end if ( computations != Udm::null)
			 END MMMMMMMMMMMMMM Metrics need to replcate elsewhere */
			out_Assemblies.topLevelAssemblies.push_back( topLevelAssemblyData);	
		}
	
	}
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////
void FromXMLFile_PopulateCADComponentAssemblyAndMap( 
					e_XMLDefinitionFormat								in_InputFormat,
																		// if XML_DEFINED_BY_FILE this would be the in_AssemblyDefinitionXML_PathAndFileName
																		// if XML_DEFINED_BY_STRING this would be a string containing the XML 
					const string 										&in_AssemblyDefinitionXML,  
					isis::CADAssemblies									&out_CADComponentAssemblies,
					std::map<std::string, isis::CADComponentData>		&out_CADComponentData_map,
					std::vector<CADCreateAssemblyError>					&out_ErrorList)
										throw (isis::application_exception )
{
		try
		{
			
			Udm::SmartDataNetwork ai(AssemblyInterface::diagram);
			
			if ( in_InputFormat == XML_DEFINED_BY_FILE )
				ai.OpenExisting(in_AssemblyDefinitionXML.c_str(),"AssemblyInterface", Udm::CHANGES_PERSIST_ALWAYS);
			else
				ai.OpenExistingFromString((string)in_AssemblyDefinitionXML,"AssemblyInterface", Udm::CHANGES_PERSIST_ALWAYS);

			AssemblyInterface::Assemblies Assemblies_ptr = AssemblyInterface::Assemblies::Cast(ai.GetRootObject());		

			///////////////////////////////////////////////////////////////
			// Populate Materials Structures and Log Materials XMl Content 
			///////////////////////////////////////////////////////////////
			/* 9/5/2013 No longer needed since the material library is being used.
			clog << std::endl << std::endl << "************** Begin XML Tree (Materials) *****************";
			vector<AssemblyInterface::Materials> materialsVector = Assemblies_ptr.Materials_kind_children();
			for ( vector<AssemblyInterface::Materials>::const_iterator i = materialsVector.begin(); i != materialsVector.end(); ++i )
			{
				vector<AssemblyInterface::Material> materialVector = i->Material_kind_children();

				for ( vector<AssemblyInterface::Material>::const_iterator j = materialVector.begin(); j != materialVector.end(); ++j )
				{
					clog << std::endl << "Material, MaterialName: " << std::string(j->MaterialName()) << ", MaterialID: " 
						              <<   std::string(j->MaterialID()) << ", MaterialType: " << std::string(j->MaterialType());

					Material tempMaterial(j->MaterialName(), isis::MaterialType_enum(j->MaterialType()) ); 

					vector<AssemblyInterface::MaterialProperty> materialPropertyVector = j->MaterialProperty_kind_children();

					for ( vector<AssemblyInterface::MaterialProperty>::const_iterator k = materialPropertyVector.begin(); k != materialPropertyVector.end(); ++k )
					{	
						clog << std::endl << "   MaterialProperty, PropertyName: " <<  std::string(k->PropertyName())
							              << ", Value: " <<  k->Value() <<", Units: " << std::string(k->Units());

						tempMaterial.materialMetrics_map.insert(std::pair<std::string, MaterialMetrics>(k->PropertyName(),MaterialMetrics(k->Value(), k->Units())));
						// this doesn't compile, tempMaterial.materialMetrics_map[k->PropertyName()] = MaterialMetrics(k->Value(), k->Units());
					}


					//AnalysisMaterialProperties  analysisMaterialProperties;

					//isis::PopulateAnalysisMaterialStruct( 
					//					out_CADComponentAssemblies.materials[j->MaterialID()], 
					//					out_CADComponentAssemblies.materials[j->MaterialID()].analysisMaterialProperties);	

					isis::PopulateAnalysisMaterialStruct( 
										tempMaterial, 
										tempMaterial.analysisMaterialProperties);	
				
					out_CADComponentAssemblies.materials[j->MaterialID()] = tempMaterial;  
					//out_CADComponentAssemblies.materials.insert(std::pair<std::string, Material>(j->MaterialID(),tempMaterial));
				}
			}
			clog << std::endl << std::endl << "************** End XML Tree (Materials) *****************";
			///////////////////////////////////////////////////////////////
			// Log Materials Internal Structures Content 
			///////////////////////////////////////////////////////////////
			clog << std::endl << std::endl << "************** Begin Material Internal Structures ***************";
			stream_MaterialsData(out_CADComponentAssemblies.materials, clog);
			clog << std::endl << "************** End Material Internal Structures *****************";
			*/
			/////////////////////////////////
			// Populate Assembly Structures
			/////////////////////////////////

			isis::PopulateProEAssemblyStructures ( Assemblies_ptr, out_CADComponentAssemblies, out_CADComponentData_map );

			/////////////////////////
			// Log Assembly Info
			/////////////////////////

			vector<AssemblyInterface::Assembly> assemblyVector = Assemblies_ptr.Assembly_kind_children();

			for ( vector<AssemblyInterface::Assembly>::const_iterator ci = assemblyVector.begin(); ci != assemblyVector.end(); ci++ )
			{
		
				vector<AssemblyInterface::CADComponent> cadComponentVector = ci->CADComponent_kind_children();
				if ( cadComponentVector.size() > 0)
				{
					isis_LOG(lg, isis_FILE, isis_INFO) << "";
					isis_LOG(lg, isis_FILE, isis_INFO) << "******* Begin XML Tree Assembly Table of Contents (Name ComponentID) **********";
					isis_LOG(lg, isis_FILE, isis_INFO)  << "Assembly Configuration ID: " << std::string(ci->ConfigurationID());
				
					std::stringstream str;
					stream_IndentedAssemblyTree( 
										cadComponentVector, 
										str);
					isis_LOG(lg, isis_FILE, isis_INFO) << str.str();
					isis_LOG(lg, isis_FILE, isis_INFO) << "******* End XML Tree Assembly Table of Contents  (Name ComponentID) ***********";

					isis_LOG(lg, isis_FILE, isis_INFO) << "";
					isis_LOG(lg, isis_FILE, isis_INFO) << "************** Begin Entire Assembly XML Tree (Detailed Info) *****************";
					isis_LOG(lg, isis_FILE, isis_INFO) << "Assembly Configuration ID: " << std::string(ci->ConfigurationID());
					str.clear();
					stream_AssemblyTree(  
									cadComponentVector, 
									str,
									"Root");
					isis_LOG(lg, isis_FILE, isis_INFO) << str.str();
					isis_LOG(lg, isis_FILE, isis_INFO) << "*************** End XML Entire Assembly Tree (Detailed Info) ******************";		
				}

				///////////////////////////////////
				// Log Computations (i.e. Metrics)
				///////////////////////////////////
				vector<AssemblyInterface::Analyses>  analysesVector = ci->Analyses_kind_children();
				Log_MetricsInformation_AllMetricTypes( analysesVector);  // All analyses types (i.e. Static, Blast, Ballistic, CFD)

			}
			isis_LOG(lg, isis_FILE, isis_INFO) << "**************************** End Computations  ********************************";
		

			/////////////////////////////////////////////
			// Populate ProcessingInstructions Structures
			/////////////////////////////////////////////

			isis_LOG(lg, isis_FILE, isis_INFO) << "";
			isis_LOG(lg, isis_FILE, isis_INFO) << "************** Begin XML Tree (Data Exchange) *****************";
			vector<AssemblyInterface::ProcessingInstructions> processingInstructionsVector = Assemblies_ptr.ProcessingInstructions_kind_children();

			for ( vector<AssemblyInterface::ProcessingInstructions>::const_iterator pr_insts(processingInstructionsVector.begin());
				  pr_insts != processingInstructionsVector.end();
				  ++pr_insts )
			{
				vector<AssemblyInterface::ProcessingInstruction> processingInstructionVector = pr_insts->ProcessingInstruction_kind_children();
				for ( vector<AssemblyInterface::ProcessingInstruction>::const_iterator pr_inst(processingInstructionVector.begin());
				  pr_inst != processingInstructionVector.end();
				  ++pr_inst )
				{
					ProcessingInstruction processingInstruction;

					processingInstruction.primary = ProcessingInstruction_Primary_enum(pr_inst->Primary());
					processingInstruction.secondary = ProcessingInstruction_Secondary_enum(pr_inst->Secondary());

					out_CADComponentAssemblies.processingInstructions.push_back(processingInstruction);
				}
			}
			//////////////////////////////////////
			// Populate Data Exchange Structures
			//////////////////////////////////////

			isis_LOG(lg, isis_FILE, isis_INFO) << "";
			isis_LOG(lg, isis_FILE, isis_INFO) << "************** Begin XML Tree (Data Exchange) *****************";
			vector<AssemblyInterface::DataExchange> dataExchangeVector = Assemblies_ptr.DataExchange_kind_children();

			for ( vector<AssemblyInterface::DataExchange>::const_iterator de(dataExchangeVector.begin());
				  de != dataExchangeVector.end();
				  ++de )
			{
				// STEPFormat
				vector<AssemblyInterface::STEPFormat> sTEPFormatVector = de->STEPFormat_kind_children();
				for ( vector<AssemblyInterface::STEPFormat>::const_iterator step(sTEPFormatVector.begin());
					  step != sTEPFormatVector.end();
					  ++step )
				{
					isis::DataExchangeSpecification  tempDataExchangeSpec(  
																isis::DataExchangeFormat_enum( "STEP" ),
																isis::DataExchangeVersion_enum( step->Name() ));

					isis_LOG(lg, isis_FILE, isis_INFO) << "Step  Format: " << std::string(step->Name());

					out_CADComponentAssemblies.DataExchangeSpecifications.push_back(tempDataExchangeSpec);
				}			
				/////////////////////////////////
				// NonSTEPFormat, e.g. Inventor, 
				////////////////////////////////
				vector<AssemblyInterface::NonSTEPFormat > nonSTEPFormatVector = de->NonSTEPFormat_kind_children();
				for ( vector<AssemblyInterface::NonSTEPFormat>::const_iterator nsf(nonSTEPFormatVector.begin());
					  nsf != nonSTEPFormatVector.end();
					  ++nsf )
				{

					isis::DataExchangeSpecification  tempDataExchangeSpec(  
																isis::DataExchangeFormat_enum(  nsf->FormatType() ), // STEP, STEREOLITHOGRAPHY,  INVENTOR, PARASOLID
																isis::DataExchangeVersion_enum(  nsf->FormatSubType() ));   // This could be Null/Binary/ASCII/STEP-Formats

					isis_LOG(lg, isis_FILE, isis_INFO) << "NonSTEPFormat  Format: " << std::string(nsf->FormatType());

					out_CADComponentAssemblies.DataExchangeSpecifications.push_back(tempDataExchangeSpec);
				}			
			

				/////////////////////////////////
				// SpecialDataFormatInstruction, e.g. LeafAssembliesMetrics, 
				////////////////////////////////
				vector<AssemblyInterface::SpecialDataFormatInstruction > specialDataFormatInstructionVector = de->SpecialDataFormatInstruction_kind_children();
				for ( vector<AssemblyInterface::SpecialDataFormatInstruction>::const_iterator nsf(specialDataFormatInstructionVector.begin());
					  nsf != specialDataFormatInstructionVector.end();
					  ++nsf )
				{
					out_CADComponentAssemblies.dataExchangeSpecialInstruction.push_back(DataExchangeSpecialInstruction_enum(nsf->Instruction()));			
					isis_LOG(lg, isis_FILE, isis_INFO) << "SpecialDataFormatInstruction  Format: " << std::string(nsf->Instruction());				
				}			
			}

			isis_LOG(lg, isis_FILE, isis_INFO) << "";
			isis_LOG(lg, isis_FILE, isis_INFO) << "************** End XML Tree (Data Exchange) *****************";
	
			//////////////////////////////////////////////
			// Populate Unassembled Components Structures
			/////////////////////////////////////////////
			
			isis_LOG(lg, isis_FILE, isis_INFO) << "";
			isis_LOG(lg, isis_FILE, isis_INFO) << "************** Begin XML Tree (Unassembled Components) *****************";
			vector<AssemblyInterface::UnassembledComponents> unAssembledCompVector = Assemblies_ptr.UnassembledComponents_kind_children();

			for ( vector<AssemblyInterface::UnassembledComponents>::const_iterator unac(unAssembledCompVector.begin());
				  unac != unAssembledCompVector.end();
				  ++unac )
			{			
				vector<AssemblyInterface::CADComponent> cadCompsVector = unac->CADComponent_kind_children();

				for ( vector<AssemblyInterface::CADComponent>::const_iterator cadComp(cadCompsVector.begin());
					  cadComp != cadCompsVector.end();
					  ++cadComp )
				{

					out_CADComponentAssemblies.unassembledComponents.push_back(
									UnassembledComponent(	cadComp->Name(),
															CADMdlType_enum(cadComp->Type()),
															cadComp->Representation(),
															cadComp->ComponentID()));

					isis_LOG(lg, isis_FILE, isis_INFO) << "UnAssembledComponent, Name: " << std::string(cadComp->Name()) << "  Type: " << std::string(cadComp->Type()) << "  Component Instance ID: " << std::string(cadComp->ComponentID());
				}			
			}
			isis_LOG(lg, isis_FILE, isis_INFO) << "";
			isis_LOG(lg, isis_FILE, isis_INFO) << "************** End XML Tree (Unassembled Components) *****************";
			
			//////////////////////////////////////////
			// Log Data Exchange Internal Structures
			//////////////////////////////////////////
			isis_LOG(lg, isis_FILE, isis_INFO) << "";
			isis_LOG(lg, isis_FILE, isis_INFO) << "************** Begin Data Exchange Internal Structures ***************";
			for ( std::list<DataExchangeSpecification>::const_iterator dei(out_CADComponentAssemblies.DataExchangeSpecifications.begin());
				  dei != out_CADComponentAssemblies.DataExchangeSpecifications.end();
				  ++dei )
			{
				isis_LOG(lg, isis_FILE, isis_INFO) << isis::DataExchangeFormat_string(dei->dataExchangeFormat) << "  " << 
					    isis::DataExchangeVersion_string(dei->dataExchangeVersion);
			}

			isis_LOG(lg, isis_FILE, isis_INFO) << "";
			isis_LOG(lg, isis_FILE, isis_INFO) << "************** End Data Exchange Internal Structures ***************";

			//////////////////////////////////////////
			// MetaLinkData
			//////////////////////////////////////////
			isis_LOG(lg, isis_FILE, isis_INFO) << "";
			isis_LOG(lg, isis_FILE, isis_INFO) << "************** Begin MetaLinkData ***************";
			vector<AssemblyInterface::Assembly> assemblyCompVector = Assemblies_ptr.Assembly_kind_children();
			for (std::vector<AssemblyInterface::Assembly>::const_iterator it = assemblyCompVector.begin(); it !=assemblyCompVector.end(); ++it)
			{
				try{
					AssemblyInterface::MetaLinkData metaLinkData = it->MetaLinkData_child();
					vector<AssemblyInterface::CadComponentMLData> cadComponentMLDataVector = metaLinkData.CadComponentMLData_kind_children();
					for (std::vector<AssemblyInterface::CadComponentMLData>::const_iterator it = cadComponentMLDataVector.begin(); it != cadComponentMLDataVector.end(); ++it)
					{
						std::string id = it->ID();
						if (id[0] != '{')
						{
							id = "{"+id+"}";
						}
						try{
							CADComponentData &data = out_CADComponentData_map.at(id);
							data.avmComponentId = it->AvmID();
							vector<AssemblyInterface::ConnectorMLData> connectorMLDataVector = it->ConnectorMLData_kind_children();
							for (vector<AssemblyInterface::ConnectorMLData>::const_iterator it = connectorMLDataVector.begin(); it != connectorMLDataVector.end(); ++it)
							{
								CADComponentConnector connector;
								connector.parentid = data.componentID;
								connector.id = it->ID();
								connector.displayName = it->DisplayName();
								vector<AssemblyInterface::DatumMLData> datumMLDataVector = it->DatumMLData_kind_children();
								for (vector<AssemblyInterface::DatumMLData>::const_iterator it = datumMLDataVector.begin(); it != datumMLDataVector.end(); ++it)
								{
									CADDatum datum;
									datum.id = it->ID();
									datum.displayName = it->DisplayName();
									connector.datums.push_back(datum);
								}
								data.connectors.push_back(connector);
								//isis_LOG(lg, isis_FILE, isis_WARN) << "Connector added. Component: " << (std::string)id << "Connector: " << connector.id;
							}
						//} catch (std::out_of_range &ex)
						} catch (std::out_of_range)
						{
							isis_LOG(lg, isis_FILE, isis_WARN) << "FromXMLFile_PopulateCADComponentAssemblyAndMap(): Component not found in map, id:" << (std::string)it->ID();
						}

					}
				} catch (...)
				{
					// No metalinkdata, continue
				}

			}

			isis_LOG(lg, isis_FILE, isis_INFO) << "************** End MetaLinkData ***************";

			if ( ai.isOpen()) ai.CloseNoUpdate();

		}
		catch (udm_exception& ex)
		{
			if ( in_InputFormat == XML_DEFINED_BY_FILE)
			{		
				std::string TempError = std::string(".  A parsing error typically occurs because of either a manually edited XML file that was malformed, or an XML file that was created with a CyPhy2CAD version that was not compatible with the CADCreoParametricCreateAssembly.exe version.") +
				"\nXML file: " + in_AssemblyDefinitionXML + 
				"\nUDM Exception: " + ex.what();
				throw isis::application_exception(TempError.c_str());
			}
			else
			{
				// XML_DEFINED_BY_STRING
				std::string TempError = std::string(" A parsing error occurred when parsing the XML received via MetaLink.  This XML defines the assembly represented by CyPhy.  See the log file for a listing of the XML.") +
				 "  UDM Exception: " + ex.what();
				throw isis::application_exception(TempError.c_str());
			}
		}
	}
} // END namespace isis