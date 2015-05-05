#include "XMLToProEStructures.h"
#include "DiagnosticUtilities.h"
#include <iostream>

#ifndef ISIS_VERSION_NUMBER_H
#define ISIS_VERSION_NUMBER_H
#include <ISISVersionNumber.h>
#include <CommonUtilities.h>
#endif

#include "UdmBase.h"
#include "AssemblyInterface.h"
#include <StringToEnumConversions.h>
#include <MaterialProperties.h>
#include <CADCommonConstants.h>

#include <deque>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>

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
			std::clog << std::endl << in_Indent <<  std::string(ci->Name())  << "  " << std::string(ci->ComponentID());

			ProMdlType pro_model_type = isis::ProMdlType_enum( ci->Type());
			if ( pro_model_type == PRO_MDL_ASSEMBLY) 
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
					out_Stream << endl << in_Indent << "   FeatureGeometryType  " << std::string(ck->FeatureGeometryType());
					out_Stream << endl << in_Indent << "   FeatureInterfaceType " << std::string(ck->FeatureInterfaceType());
					out_Stream << endl << in_Indent << "   FeatureAlignmentType " << std::string(ck->FeatureAlignmentType());
			
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

			ProMdlType pro_model_type = isis::ProMdlType_enum( ci->Type());
			if ( pro_model_type == PRO_MDL_ASSEMBLY) 
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
	out_CADComponentData_map[ID].name			= in_XML_CADComponent.Name();
	out_CADComponentData_map[ID].displayName	= in_XML_CADComponent.DisplayName();

	if ( out_CADComponentData_map[ID].name.size() > (ISIS_CREO_MAX_NAME_SIZE ))
	{
		std::stringstream errorString;
		errorString <<
		"The input xml file contains a component name that is longer than the maximum allowable length of "  
		<< ISIS_CREO_MAX_NAME_SIZE << " characters."
		<< std::endl << "   Component Name:          " << out_CADComponentData_map[ID].name
		<< std::endl << "   Component ID:            " << out_CADComponentData_map[ID].componentID
		<< std::endl << "   To locate the error, search for the Component ID in the input xml file.";
		throw isis::application_exception(errorString.str().c_str());
	}

	out_CADComponentData_map[ID].materialID		= in_XML_CADComponent.MaterialID();
	out_CADComponentData_map[ID].modelType		= isis::ProMdlType_enum(in_XML_CADComponent.Type()); // PRO_MDL_PART, PRO_MDL_ASSEMBLY
	//out_CADComponentData_map[ID].type_string	= in_XML_CADComponent.Type();
	out_CADComponentData_map[ID].specialInstruction = isis::SpecialInstruction_enum(in_XML_CADComponent.SpecialInstruction());

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

		vector<AssemblyInterface::Pair> pairVector = ci->Pair_kind_children();
		for ( vector<AssemblyInterface::Pair>::const_iterator ck = pairVector.begin(); ck != pairVector.end(); ck++)
		{
			ConstraintPair Cst_Pair;

			 // ProAsmcompConstrType:		PRO_ASM_ALIGN, PRO_ASM_MATE, PRO_ASM_MATE_OFF, always PRO_ASM_ALIGN for now 
			Cst_Pair.featureAlignmentType = ProAsmcompConstrType_enum( ck->FeatureAlignmentType() );

			// ProType:						PRO_SURFACE, PRO_AXIS = 	
			Cst_Pair.featureGeometryType =	FeatureGeometryType_enum( ck->FeatureGeometryType() );

			// e_FeatureInterfaceType:		CAD_DATUM, CAD_MODEL_INTERFACE, CAD_MODEL_USER_DATA
			Cst_Pair.featureInterfaceType = FeatureInterfaceType_enum( ck->FeatureInterfaceType() );

			vector<AssemblyInterface::ConstraintFeature> featureVector = ck->ConstraintFeature_kind_children();
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
						throw isis::application_exception(errorString.str().c_str());
				}

				if ( Cst_Feature.featureName.size() == 0  )
				{
					std::stringstream errorString;
					errorString <<
						"When parsing the input xml file, encountered a feature name that is null (i.e. zero length string)."  
						<< std::endl << "Feature Name:          " << Cst_Feature.featureName
						<< std::endl << "Component ID:          " << Cst_Feature.componentInstanceID
						<< std::endl << "To locate the error, search for the FeatureName=\"\" in the input xml file.";
						throw isis::application_exception(errorString.str().c_str());
				}

				if ( Cst_Feature.featureName.size() > ISIS_CREO_MAX_NAME_SIZE )
				{
					std::stringstream errorString;
					errorString <<
						"When parsing the input xml file, encountered a feature name that is longer than the maximum allowable length of "  
						<< ISIS_CREO_MAX_NAME_SIZE << " characters."
						<< std::endl << "Feature Name:          " << Cst_Feature.featureName
						<< std::endl << "Component ID:          " << Cst_Feature.componentInstanceID
						<< std::endl << "To locate the error, search for the Component ID in the input xml file.";
						throw isis::application_exception(errorString.str().c_str());
				}

				Cst_Feature.featureOrientationType =	ProDatumside_enum(cl->FeatureOrientationType());

				Cst_Pair.constraintFeatures.push_back( Cst_Feature ); 

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
			throw isis::application_exception(errorString.str().c_str());
	}

	if ( out_CADComponentData_map.find(ID) != out_CADComponentData_map.end() )
	{
		std::stringstream errorString;
		errorString <<
			"When parsing the input xml file, encountered a Component ID that was already used.  "  
			<< std::endl << "Component ID: \"" << ID << "\","
			<< std::endl << "To locate the error, search for the Component ID in the input xml file.  Find the"
			<< std::endl << "locations where the Component ID was used more than once.";
			throw isis::application_exception(errorString.str().c_str());
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
			analysisGeometryFeature.featureGeometryType = CADFeatureGeometryType_enum(ck->FeatureGeometryType());
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
			analysisGeometryFeature.featureGeometryType = CADFeatureGeometryType_enum(ck->FeatureGeometryType());
				
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
			// zzzz should check if k.ComponentID() exists in the map.  
			// it might not exist in the map at this point,  if not check later
			CADComputation cADComputation( k.ComponentID());
			cADComputation.computationType =  ComputationType_enum(k.MetricType());
			cADComputation.computationDimension = ComputationDimension_enum(k.RequestedValueType());
			cADComputation.metricID = k.MetricID();
			cADComputation.datumName = k.Details(); 
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
			clog << std::endl <<  "   ComponentID:        " << std::string( k.ComponentID());
			clog << std::endl <<  "   MetricID:           " << std::string(k.MetricID());
			clog << std::endl <<  "   MetricType:         " << std::string(k.MetricType());
			clog << std::endl <<  "   RequestedValueType: " << std::string(k.RequestedValueType());
			clog << std::endl <<  "   Details:            " << std::string(k.Details());	
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////

void Log_MetricsInformation_AllMetricTypes( const vector<AssemblyInterface::Analyses> &in_AnalysesVector )
{
	// Log Metrics
	clog << std::endl << "Begin Metrics Information:";

	for each ( AssemblyInterface::Analyses i in in_AnalysesVector )
	{
		// Static
		clog << std::endl << "Metrics Static Information:";
		vector<AssemblyInterface::Static> staticVector = i.Static_kind_children();
		for each ( AssemblyInterface::Static j in staticVector ) 
		{
			vector<AssemblyInterface::Metrics> metricsVector = j.Metrics_kind_children();
			Log_MetricsInformation_SingleMetricType( metricsVector );
		}
		// Ballistic
		clog << std::endl << "Metrics Ballistic Information:";
		vector<AssemblyInterface::Ballistic> BallisticVector = i.Ballistic_kind_children();
		for each ( AssemblyInterface::Ballistic j in BallisticVector ) 
		{
			vector<AssemblyInterface::Metrics> metricsVector = j.Metrics_kind_children();
			Log_MetricsInformation_SingleMetricType(	metricsVector );
		}
		// Blast
		clog << std::endl << "Metrics Blast Information:";
		vector<AssemblyInterface::Blast> blastVector = i.Blast_kind_children();
		for each ( AssemblyInterface::Blast j in blastVector ) 
		{
			vector<AssemblyInterface::Metrics> metricsVector = j.Metrics_kind_children();
			Log_MetricsInformation_SingleMetricType(	metricsVector );
		}
		// CFD
		clog << std::endl << "Metrics CFD Information:";
		vector<AssemblyInterface::CFD> cFDVector = i.CFD_kind_children();
		for each ( AssemblyInterface::CFD j in cFDVector ) 
		{
			vector<AssemblyInterface::Metrics> metricsVector = j.Metrics_kind_children();
			Log_MetricsInformation_SingleMetricType(	metricsVector );
		}
	}
	clog << std::endl << "End Metrics Information";
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
		AnalysisCFD temp_AnalysisBallistic;
		/////////////////
		// Metrics
		/////////////////
		vector<AssemblyInterface::Metrics> metrics = i.Metrics_kind_children();
		PopulateMetricsInformation(	metrics, temp_AnalysisBallistic.metrics );
		in_out_Analyses_CAD.analysesCFD.push_back(temp_AnalysisBallistic);
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
						analysisGeometryFeature.featureGeometryType = CADFeatureGeometryType_enum(ck->FeatureGeometryType());
						
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
							analysisConstraint.displacement.translation.x = ConstraintValue(translation.x());
							analysisConstraint.displacement.translation.y = ConstraintValue(translation.y());
							analysisConstraint.displacement.translation.z = ConstraintValue(translation.z());

							AssemblyInterface::Rotation rotation = displacement.Rotation_child();
							analysisConstraint.displacement.rotation.units = CADUnitsAngle_enum(rotation.Units());
							analysisConstraint.displacement.rotation.x = AnalysisConstraintFixedFree_enum(rotation.x());
							analysisConstraint.displacement.rotation.y = AnalysisConstraintFixedFree_enum(rotation.y());
							analysisConstraint.displacement.rotation.z = AnalysisConstraintFixedFree_enum(rotation.z());
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
					analysisLoad.geometry.featureGeometryType =  CADFeatureGeometryType_enum(features.FeatureGeometryType());

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
				analysisSolver.type				= AnalysisSolverType_enum( cj->Type());
				analysisSolver.meshType			= AnalysisMeshType_enum( cj->MeshType());
				analysisSolver.shellElementType	= AnalysisShellElementType_enum( cj->ShellElementType() );
				analysisSolver.elementShapeType = AnalysisElementShapeType_enum( cj->ElementShapeType() );

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
			
					clog << std::endl  << std::endl << "******* Begin XML Tree Assembly Table of Contents (Name ComponentID) **********";
					clog << std::endl  << "Assembly Configuration ID: " << std::string(ci->ConfigurationID());
				
					stream_IndentedAssemblyTree( 
										cadComponentVector, 
										clog);
					clog << std::endl << "******* End XML Tree Assembly Table of Contents  (Name ComponentID) ***********";

					clog << std::endl << std::endl << "************** Begin Entire Assembly XML Tree (Detailed Info) *****************";
					clog << std::endl  << "Assembly Configuration ID: " << std::string(ci->ConfigurationID());
					stream_AssemblyTree(  
									cadComponentVector, 
									clog,
									"Root");
					clog << std::endl << "*************** End XML Entire Assembly Tree (Detailed Info) ******************";		
				}

				///////////////////////////////////
				// Log Computations (i.e. Metrics)
				///////////////////////////////////
				vector<AssemblyInterface::Analyses>  analysesVector = ci->Analyses_kind_children();
				Log_MetricsInformation_AllMetricTypes( analysesVector);  // All analyses types (i.e. Static, Blast, Ballistic, CFD)

			}
			clog << std::endl << "**************************** End Computations  ********************************";
		

			//////////////////////////////////////
			// Populate Data Exchange Structures
			//////////////////////////////////////

			clog << std::endl << std::endl << "************** Begin XML Tree (Data Exchange) *****************";
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

					clog << std::endl << "Step  Format: " << std::string(step->Name());

					out_CADComponentAssemblies.DataExchangeSpecifications.push_back(tempDataExchangeSpec);
				}			
				//////////////////////
				// Stereolithography
				/////////////////////
				vector<AssemblyInterface::Stereolithography> stereolithographyVector = de->Stereolithography_kind_children();
				for ( vector<AssemblyInterface::Stereolithography>::const_iterator stl(stereolithographyVector.begin());
					  stl != stereolithographyVector.end();
					  ++stl )
				{
					isis::DataExchangeSpecification  tempDataExchangeSpec(  
																isis::DataExchangeFormat_enum( "Stereolithography" ),
																isis::DataExchangeVersion_enum( stl->FormatType() ));

					clog << std::endl << "Stereolithography  Format: " << std::string(stl->FormatType());

					out_CADComponentAssemblies.DataExchangeSpecifications.push_back(tempDataExchangeSpec);
				}			
			}
			clog << std::endl << std::endl << "************** End XML Tree (Data Exchange) *****************";
	
			//////////////////////////////////////////////
			// Populate Unassembled Components Structures
			/////////////////////////////////////////////
			
			clog << std::endl << std::endl << "************** Begin XML Tree (Unassembled Components) *****************";
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
									UnassembledComponent(cadComp->Name(),ProMdlType_enum(cadComp->Type()),cadComp->ComponentID()));

					clog << std::endl << "UnAssembledComponent, Name: " << std::string(cadComp->Name()) << "  Type: " << std::string(cadComp->Type()) << "  Component Instance ID: " << std::string(cadComp->ComponentID());
				}			
			}
			clog << std::endl << std::endl << "************** End XML Tree (Unassembled Components) *****************";
			
			//////////////////////////////////////////
			// Log Data Exchange Internal Structures
			//////////////////////////////////////////
			clog << std::endl << std::endl << "************** Begin Data Exchange Internal Structures ***************";
			for ( std::list<DataExchangeSpecification>::const_iterator dei(out_CADComponentAssemblies.DataExchangeSpecifications.begin());
				  dei != out_CADComponentAssemblies.DataExchangeSpecifications.end();
				  ++dei )
			{
				clog << std::endl << isis::DataExchangeFormat_string(dei->dataExchangeFormat) << "  " << 
					    isis::DataExchangeVersion_string(dei->dataExchangeVersion);
			}

			clog << std::endl << std::endl << "************** End Data Exchange Internal Structures ***************";

			if ( ai.isOpen()) ai.CloseNoUpdate();

		}
		catch (udm_exception& ex)
		{
			if ( in_InputFormat == XML_DEFINED_BY_FILE)
			{		
				std::string TempError = std::string(".  A parsing error typically occurs because of either a manually edited XML file that was malformed, or an XML file that was created with the wrong version of CyPhy2CAD.dll.  The CyPhy2CAD.dll version must be ") +
				ISIS_CYPHY_2_CAD_DLL_MIN_VERSION + 
				" or later.  To determine the version of CyPhy2CAD.dll, right click on the dll, select Properties, and select the Details tab." + 
				"  XML file: " + in_AssemblyDefinitionXML + "  UDM Exception: " + ex.what();
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