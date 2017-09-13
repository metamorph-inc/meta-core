#include <DiagnosticUtilities.h>
#include <cc_CommonStructures.h>
#include <CommonFunctions.h>
#include <CreoStringToEnumConversions.h>
#include <MaterialProperties.h>
#include <ISISConstants.h>
#include <cc_CommonConstants.h>
#include <iostream>
#include <set>
#include <iterator>
#include <algorithm>

namespace isis
{

void stream_PopulateOneConstraintInConstraintStructure(
	ProSolid	 *in_assembly_model,			// typedef struct sld_part* ProSolid;
	const list<int>	 &in_base_model_path_list,
	const list<int>	 &in_added_model_path_list,
	ProType		 in_pro_datum_type,				// enum PRO_SURFACE, PRO_AXIS
	const ProName		 in_base_model_datum_name,		// ASM_RIGHT, A_1..
	ProDatumside in_base_model_datum_side,		// enum PRO_DATUM_SIDE_YELLOW (SIDE_A), PRO_DATUM_SIDE_RED (SIDE_B), PRO_DATUM_SIDE_NONE
	const ProName		 in_added_model_datum_name,		// RIGHT, A23 ..
	ProDatumside in_added_model_datum_side,		// enum PRO_DATUM_SIDE_YELLOW (SIDE_A), PRO_DATUM_SIDE_RED (SIDE_B), PRO_DATUM_SIDE_NONE
	ProAsmcompConstrType	in_constraint_type, // enum PRO_ASM_ALIGN, PRO_ASM_ALIGN_OFF...
	double		 in_offset_between_datums,		// This is only used if in_constraint_type == PRO_ASM_ALIGN_OFF or PRO_ASM_MATE_OFF
	ostream &out_Stream)
{

	char temp_string[100];
	out_Stream << std::endl << "ProSolid     in_assembly_model: " << *in_assembly_model;	

	out_Stream << std::endl << "in_base_model_path_list, Feature IDs:";
	for each ( int i in in_base_model_path_list)  out_Stream << " " << i;

	out_Stream << std::endl << "in_added_model_path_list, Feature IDs:";
	for each ( int i in in_added_model_path_list)  out_Stream << " " << i;

	out_Stream << std::endl << "ProType       in_pro_datum_type:          " <<	in_pro_datum_type;
	out_Stream << std::endl << "ProName       in_base_model_datum_name:   " <<	ProWstringToString(temp_string, (wchar_t *) in_base_model_datum_name);	
	out_Stream << std::endl << "ProDatumside  in_base_model_datum_side:   " <<	in_base_model_datum_side;		
	out_Stream << std::endl << "ProName       in_added_model_datum_name:  " <<	ProWstringToString(temp_string, (wchar_t *) in_added_model_datum_name);	
	out_Stream << std::endl << "ProDatumside  in_added_model_datum_side:  " <<	in_added_model_datum_side;
	out_Stream << std::endl << "ProAsmcompConstrType  in_constraint_type: " <<	in_constraint_type;
	out_Stream << std::endl << "double        in_offset_between_datums:   " <<	in_offset_between_datums;
	
//	out_Stream << std::endl << "ProAsmcompconstraint*	&out_constraints: " <<	out_constraints;

}

void stream_PopulateOneConstraintInConstraintStructure_2(
	ProSolid	 *in_assembly_model,			// typedef struct sld_part* ProSolid;
	const list<int>	 &in_base_model_path_list,
	const list<int>	 &in_added_model_path_list,
	ProType		 in_pro_datum_type,				// enum PRO_SURFACE, PRO_AXIS
	const ProName		 in_base_model_datum_name,		// ASM_RIGHT, A_1..
	ProDatumside in_base_model_datum_side,		// enum PRO_DATUM_SIDE_YELLOW (SIDE_A), PRO_DATUM_SIDE_RED (SIDE_B), PRO_DATUM_SIDE_NONE
	const ProName		 in_added_model_datum_name,		// RIGHT, A23 ..
	ProDatumside in_added_model_datum_side,		// enum PRO_DATUM_SIDE_YELLOW (SIDE_A), PRO_DATUM_SIDE_RED (SIDE_B), PRO_DATUM_SIDE_NONE
	ProAsmcompConstrType	in_constraint_type, // enum PRO_ASM_ALIGN, PRO_ASM_ALIGN_OFF...
	double		 in_offset_between_datums,		// This is only used if in_constraint_type == PRO_ASM_ALIGN_OFF or PRO_ASM_MATE_OFF
	bool		flip_orientation,
	ostream &out_Stream)
{

	char temp_string[100];
	out_Stream << std::endl << "ProSolid     in_assembly_model: " << *in_assembly_model;	

	out_Stream << std::endl << "in_base_model_path_list, Feature IDs:";
	for each ( int i in in_base_model_path_list)  out_Stream << " " << i;

	out_Stream << std::endl << "in_added_model_path_list, Feature IDs:";
	for each ( int i in in_added_model_path_list)  out_Stream << " " << i;

	out_Stream << std::endl << "ProType       in_pro_datum_type:          " <<	in_pro_datum_type;
	out_Stream << std::endl << "ProName       in_base_model_datum_name:   " <<	ProWstringToString(temp_string, (wchar_t *) in_base_model_datum_name);	
	out_Stream << std::endl << "ProDatumside  in_base_model_datum_side:   " <<	in_base_model_datum_side;		
	out_Stream << std::endl << "ProName       in_added_model_datum_name:  " <<	ProWstringToString(temp_string, (wchar_t *) in_added_model_datum_name);	
	out_Stream << std::endl << "ProDatumside  in_added_model_datum_side:  " <<	in_added_model_datum_side;
	out_Stream << std::endl << "ProAsmcompConstrType  in_constraint_type: " <<	in_constraint_type;
	out_Stream << std::endl << "double        in_offset_between_datums:   " <<	in_offset_between_datums;
	out_Stream << std::endl << "bool		  flip_orientation:			  " <<	flip_orientation;
	
//	out_Stream << std::endl << "ProAsmcompconstraint*	&out_constraints: " <<	out_constraints;

}



void printf_assembled_info( wchar_t in_part_1[], 
						    wchar_t in_part_2[], 
							int in_num_datums, 
							ProName in_datums[3][2] )
{

	//char		temp_narrow_string_1[PRO_FAMILY_NAME_SIZE];
	//char		temp_narrow_string_2[PRO_FAMILY_NAME_SIZE];

	char		temp_narrow_string_1[ISIS_CHAR_BUFFER_LENGTH];
	char		temp_narrow_string_2[ISIS_CHAR_BUFFER_LENGTH];

	printf("\n\nAssembled %s --> %s",
		   ProWstringToString(temp_narrow_string_1,in_part_1),
		   ProWstringToString(temp_narrow_string_2,in_part_2));

	for ( int i = 0; i < in_num_datums; ++i )
	{
		printf("\n    %s --> %s", 
			ProWstringToString(temp_narrow_string_1,in_datums[i][1]),
			ProWstringToString(temp_narrow_string_2,in_datums[i][0]));
	}
}

///////////////////////////////////////////////////////////////////////////////////////////
void printf_assembled_orientation_info( int in_num_datums,  ProType in_datum_type[],
									    ProDatumside in_datum_side[3][2],
										ProAsmcompConstrType  in_constraint_type[],
										double in_offset[3])
{

		for ( int i = 0; i < in_num_datums; ++i )
		{
			if ( in_datum_type[i] == PRO_SURFACE ) printf("\n    %d datum_type: PRO_SURFACE", i);
			else
				if ( in_datum_type[i] == PRO_AXIS ) printf("\n    %d datum_type: PRO_AXIS", i);
				else
					printf("\n%d datum_type: UNKNOWN", i);				
		}

		for ( int i = 0; i < in_num_datums; ++i )
		{
			char part_0[ISIS_CHAR_BUFFER_LENGTH];
			char part_1[ISIS_CHAR_BUFFER_LENGTH];

			strcpy( part_0,  "PRO_DATUM_SIDE_NONE" );
			strcpy( part_1,  "PRO_DATUM_SIDE_NONE" );

			if ( in_datum_side[i][0] == PRO_DATUM_SIDE_RED )
				strcpy( part_0,  "PRO_DATUM_SIDE_RED (DATUM_B)" );
			else
				if ( in_datum_side[i][0] == PRO_DATUM_SIDE_YELLOW )
					strcpy( part_0,  "PRO_DATUM_SIDE_YELLOW (DATUM_A)" );

			if ( in_datum_side[i][1] == PRO_DATUM_SIDE_RED )
				strcpy( part_1,  "PRO_DATUM_SIDE_RED (DATUM_B)" );
			else
				if ( in_datum_side[i][1] == PRO_DATUM_SIDE_YELLOW )
					strcpy( part_1,  "PRO_DATUM_SIDE_YELLOW (DATUM_A)" );

			printf("\n    %d Datum_side_1 --> Datum_side_0, %s --> %s", i, part_0, part_1 );

		}

		for ( int i = 0; i < in_num_datums; ++i )
		{
			if ( in_constraint_type[i] == PRO_ASM_ALIGN ) printf("\n    %d PRO_ASM_ALIGN", i );
			else
				if ( in_constraint_type[i] == PRO_ASM_MATE ) printf("\n    %d PRO_ASM_MATE", i );
				else
					if ( in_constraint_type[i] == PRO_ASM_MATE_OFF ) printf("\n    %d PRO_ASM_MATE_OFF", i );
					else
						printf("\n   %d Unknown", i );
		}

		for ( int i = 0; i < in_num_datums; ++i )
		{
			printf("\n    %d offset: %f", i, in_offset[i]);
		}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
int ComputeNumberOfComponents(  std::map<std::string, CADComponentData> &in_CADComponentData_map )
{
	int count = 0;
	for ( std::map<string, isis::CADComponentData>::const_iterator i(in_CADComponentData_map.begin());
		  i != in_CADComponentData_map.end(); ++i )
	{
		if ( i->second.children.size() == 0 )  ++count; // This is a part or a leaf assembly
	}

	return count;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
void stream_CADComponentData_map( const std::map<std::string, isis::CADComponentData> &in_CADComponentData_map,
									    ostream &out_Stream )
{
	for ( std::map<string, isis::CADComponentData>::const_iterator i(in_CADComponentData_map.begin());
		  i != in_CADComponentData_map.end(); ++i )
	{
		out_Stream << std::endl <<  "Component Instance ID: " << i->first;
		out_Stream << i->second;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
void stream_SingleCADComponentData( const std::string					&in_ComponentID, 
					std::map<string, isis::CADComponentData>			&in_CADComponentData_map,
					 ostream &out_Stream )
{
		out_Stream << std::endl << std::endl << 
			"CADComponent:" << std::endl <<
			"   ComponentInstanceID:       " << in_CADComponentData_map[in_ComponentID].componentID.c_str() << std::endl <<
			"   Name:                      " << in_CADComponentData_map[in_ComponentID].name << std::endl <<
			"   Type:                      " << isis::ProMdlType_string(in_CADComponentData_map[in_ComponentID].modelType) <<  std::endl <<
			"   MaterialID (FromCyPhy):    " << in_CADComponentData_map[in_ComponentID].materialID_FromCyPhy <<  std::endl <<
			"   MaterialID (FromCreoPart): " << in_CADComponentData_map[in_ComponentID].materialID_FromCADPart <<  std::endl <<
			//"   SpecialInstruction:        " << isis::SpecialInstruction_string(in_CADComponentData_map[in_ComponentID].specialInstruction) <<  std::endl <<
			"   SpecialInstruction:        " << in_CADComponentData_map[in_ComponentID].specialInstruction <<  std::endl <<
			"   cADModel_hdl:              " << in_CADComponentData_map[in_ComponentID].cADModel_hdl <<  std::endl <<
			"   assembledFeature.id:       " << in_CADComponentData_map[in_ComponentID].assembledFeature.id <<  std::endl <<
			"   assembledFeature.type:     " << in_CADComponentData_map[in_ComponentID].assembledFeature.type <<  std::endl <<
			"   assembledFeature.owner:    " << in_CADComponentData_map[in_ComponentID].assembledFeature.owner <<  std::endl <<
			"   constraints:";

		
			out_Stream << std::endl <<
				"      applyAllConstraints:     "  << Bool_string(in_CADComponentData_map[in_ComponentID].constraintDef.applyAllConstraints) <<  std::endl <<
				"      applySingleConstaintID:  "  << in_CADComponentData_map[in_ComponentID].constraintDef.applySingleConstaintID;

			//out_Stream << std::endl << "in_CADComponentData_map[in_ComponentID].constraintDef.constraints.capacity() %%%  " << in_CADComponentData_map[in_ComponentID].constraintDef.constraints.capacity();
			
			
			for ( std::vector<ConstraintData>::const_iterator i(in_CADComponentData_map[in_ComponentID].constraintDef.constraints.begin());
				  i != in_CADComponentData_map[in_ComponentID].constraintDef.constraints.end();
				  ++i )
			{
				if ( i->computedJointData.globalJointPositionDefined )
				{
					out_Stream << std::endl << "      globalJointPosition, location:    "  << i->computedJointData.globalJointPosition.location;
					out_Stream << std::endl << "      globalJointPosition: orientation: "  << i->computedJointData.globalJointPosition.orientation;
					out_Stream << std::endl << "      globalJointPosition: rotation:    "  << i->computedJointData.globalJointPosition.rotation;
				}
				else
				{
					out_Stream << std::endl << "      globalJointPosition:     Not Defined"; 
				}
				for (	std::vector<ConstraintPair>::const_iterator j(i->constraintPairs.begin());
						j != i->constraintPairs.end();
						++j )
				{
				
					out_Stream << std::endl <<	"      constraintFeatures:";
					
					for (	std::vector<ConstraintFeature>::const_iterator k(j->constraintFeatures.begin());
							k != j->constraintFeatures.end();
							++k )
					{
						out_Stream << std::endl <<
						"         component:               " <<    k->componentInstanceID  <<  "  " << in_CADComponentData_map[k->componentInstanceID].name << std::endl <<
						"         featureName:             " <<  k->featureName  << std::endl <<
						"         featureOrientationType:  " <<  ProDatumside_string( k->featureOrientationType );

					}
				}

			}
			out_Stream << std::endl << "   parametricParameters:";

			if ( in_CADComponentData_map[in_ComponentID].parametricParametersPresent )
			{
				for ( std::list<CADParameter>::const_iterator i( in_CADComponentData_map[in_ComponentID].parametricParameters.begin());
					  i != in_CADComponentData_map[in_ComponentID].parametricParameters.end();
					  ++i )
				{
					out_Stream << std::endl << "      name:    " << i->name;
					out_Stream << std::endl << "      name:    " << CADParameterType_string(i->type);
					out_Stream << std::endl << "      value:   " << i->value;
				}
			}

			out_Stream << std::endl << "  componentPaths, Feature IDs:";
			for each ( int i in in_CADComponentData_map[in_ComponentID].componentPaths)  out_Stream << " " << i;

			out_Stream << std::endl << "   children:";
			for ( list<std::string>::const_iterator i = in_CADComponentData_map[in_ComponentID].children.begin();
			  i != in_CADComponentData_map[in_ComponentID].children.end(); ++i )
			{
				out_Stream << std::endl << "      Component:   " << *i  << "  " << in_CADComponentData_map[*i].name;
			}

			out_Stream << std::endl << "   depends on:";
			for ( set<std::string>::const_iterator i = in_CADComponentData_map[in_ComponentID].dependsOn.begin();
			  i != in_CADComponentData_map[in_ComponentID].dependsOn.end(); ++i )
			{
				out_Stream << std::endl << "      Component:   " << *i  << "  " << in_CADComponentData_map[*i].name;
			}

			if ( in_CADComponentData_map[in_ComponentID].analysisTempMaterialDefined )
			{
				out_Stream << std::endl << "   Material Definition:";
				out_Stream << std::endl << "      Name:                  " << in_CADComponentData_map[in_ComponentID].name;
				out_Stream << std::endl << "      Poisson's Ratio:       " << in_CADComponentData_map[in_ComponentID].analysisTempMaterialDefinition.poissonsRatio;
				out_Stream << std::endl << "      Token Poisson's Ratio: " << in_CADComponentData_map[in_ComponentID].analysisTempMaterialDefinition.tokenPoissonsRatio;
			}
			else
			{
				out_Stream << std::endl << "   Material Definition: Not retrieved from Creo Model.  Only retrieved for analysis runs.";
			}

			///////////////////////
			// Computations
			///////////////////////
			//out_Stream << std::endl << "   Computations:";
			//out_Stream << std::endl << "      Bounding Box Metric Defined: ";
			//if ( in_CADComponentData_map[in_ComponentID].cADComputations.boundingBoxMetricDefined ) 
			//{
			//	out_Stream << "True";
			//	out_Stream << std::endl << "      Bounding Box Height Metric ID: " << in_CADComponentData_map[in_ComponentID].cADComputations.heightMetricID;
			//	out_Stream << std::endl << "      Bounding Box Width  Metric ID: " << in_CADComponentData_map[in_ComponentID].cADComputations.widthMetricID;
			//	out_Stream << std::endl << "      Bounding Box Length Metric ID: " << in_CADComponentData_map[in_ComponentID].cADComputations.lengthMetricID;
			//}
			//else
			//{
			//	out_Stream << "False";
			//}			
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
void stream_AssemblyCADComponentData( const std::string					&in_AssemblyComponentID, 
					std::map<string, isis::CADComponentData>			&in_CADComponentData_map,
					 ostream											&out_Stream, 
					 bool												in_OutputChildren,
					 int												in_AssemblyLevel   )
{
	out_Stream << std::endl;

	if ( in_OutputChildren ) out_Stream << std::endl << "<<<<<< Assembly Hierarchy Level >>>>>: " << in_AssemblyLevel;

	stream_SingleCADComponentData(  in_AssemblyComponentID, 
									in_CADComponentData_map,
									out_Stream );

	for ( std::list<string>::const_iterator i (in_CADComponentData_map[in_AssemblyComponentID].children.begin());
	  i != in_CADComponentData_map[in_AssemblyComponentID].children.end();
	  ++i )
	{
		if ( in_OutputChildren ) stream_AssemblyCADComponentData( *i, in_CADComponentData_map, out_Stream, true, in_AssemblyLevel + 1 );

	} // END for ( std::list<string>::const_it...

}

/////////////////////////////////////////////////////////////////////////////////////////////////

void stream_MaterialsData(const std::map<std::string, Material>	&in_Materials, ostream &out_Stream )
																			throw (isis::application_exception)
{
	for ( std::map<std::string, Material>::const_iterator i(in_Materials.begin());
		  i != in_Materials.end(); ++i)
	{
		out_Stream << std::endl << "Material, MaterialName: " << i->second.materialName << ", MaterialID: " 
			<<   i->first << ", MaterialType: " << isis::MaterialType_string(i->second.materialType);

		for ( std::map<std::string, MaterialMetrics>::const_iterator j(i->second.materialMetrics_map.begin());
			j != i->second.materialMetrics_map.end(); ++j )
		{
			out_Stream << std::endl << "   MaterialProperty, PropertyName: " <<  j->first   
				<< ", Value: " <<  j->second.value <<", Units: " << j->second.units;

		}

		out_Stream << std::endl << std::endl << "Analysis Material Properties:";
		out_Stream << std::endl << "MaterialID: " << i->first;
		isis::stream_AnalysisMaterialProperties( i->second.analysisMaterialProperties, out_Stream);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
std::set<std::string> NonSizeToFitComponents_ReferencedBy_SizeToFitConstraints( 
								std::map<string, isis::CADComponentData>	&in_CADComponentData_map )
{
	std::set<std::string> SIZE_TO_FIT_ComponentIDs;	
	std::set<std::string> NON_SIZE_TO_FIT_ConstraintReferences;	

	for ( std::map<string, isis::CADComponentData>::const_iterator i(in_CADComponentData_map.begin());
		  i != in_CADComponentData_map.end(); ++i )
	{
		if ( i->second.specialInstruction == CAD_SPECIAL_INSTRUCTION_SIZE_TO_FIT )
		{
			SIZE_TO_FIT_ComponentIDs.insert( i->second.componentID );
		}
		else
		{
			for ( std::set<std::string>::const_iterator j(i->second.dependsOn.begin());
			j != i->second.dependsOn.end();
			++j )
			{
				NON_SIZE_TO_FIT_ConstraintReferences.insert( *j );
			}
		}
	}

	std::set<std::string>::const_iterator itr;
	std::set<std::string>  Intersection;
	std::insert_iterator<std::set<std::string> > ii(Intersection, Intersection.begin()); 

	std::set_intersection ( SIZE_TO_FIT_ComponentIDs.begin(), 
							SIZE_TO_FIT_ComponentIDs.end(), 
							NON_SIZE_TO_FIT_ConstraintReferences.begin(), 
							NON_SIZE_TO_FIT_ConstraintReferences.end(), ii );
	return Intersection;
}

//void stream_AnalysisGeometry( const AnalysisGeometry &in_Geometry, const std::string in_Indent, ostream &out_Stream  )
//																					throw (isis::application_exception)
//{
//	out_Stream << std::endl << in_Indent << "GeometryType:          " <<  CADGeometryType_string(in_Geometry.geometryType);
//	out_Stream << std::endl << in_Indent << "ComponentID:           " <<  in_Geometry.componentID;
//	out_Stream << std::endl << in_Indent << "FeatureInterfaceType:  " <<  FeatureInterfaceType_string(in_Geometry.featureInterfaceType);
//	out_Stream << std::endl << in_Indent << "FeatureGeometryType:   " <<  CADAnalysisFeatureGeometryType_string(in_Geometry.featureGeometryType);
//	out_Stream << std::endl << in_Indent << "Features: ";
//	
//	for( std::list<std::string>::const_iterator i( in_Geometry.features.begin()); i != in_Geometry.features.end(); ++i )
//	{
//			out_Stream << std::endl << in_Indent << "   Feature Name: " << *i;
//	}
//
//}

void stream_AnalysisGeometry( const AnalysisGeometry &in_Geometry, const std::string in_Indent, ostream &out_Stream  )
																					throw (isis::application_exception)
{
	
	//out_Stream << std::endl << in_Indent << "UnaryOperationDefined: " <<  	in_Geometry.unaryOperationDefined;
	out_Stream << std::endl << in_Indent << "SetOperationDefined: " <<  	in_Geometry.setOperationDefined;;

	for ( std::list<AnalysisGeometryFeature>::const_iterator i(in_Geometry.features.begin()); i != in_Geometry.features.end(); ++i)
	{
		out_Stream << std::endl << in_Indent << "FeatureID:             " << i->featureID;
		out_Stream << std::endl << in_Indent << "FeatureGeometryType:   " << CADGeometryType_string(i->geometryType);
		out_Stream << std::endl << in_Indent << "FeatureInterfaceType:  " << FeatureInterfaceType_string(i->featureInterfaceType);

		for( std::list<CADFeature>::const_iterator j(i->features.begin()); j != i->features.end(); ++j )
		{
			out_Stream << std::endl << in_Indent << "   Component Instance ID: " << j->componentID << "  Datum Name: " << j->datumName;
		}

	}

}
//////////////////////////////////////////////////////////////////////////////////////////////////////
void stream_AnalysisInputData( const CADAnalyses &in_CADAnalyses, ostream &out_Stream ) throw (isis::application_exception)
{

	out_Stream << std::endl << "Analyses:";
	for ( std::list<AnalysisFEA>::const_iterator i( in_CADAnalyses.analysesFEA.begin()); i != in_CADAnalyses.analysesFEA.end(); ++i )
	{
		out_Stream << std::endl << "   AnalysisID:   " << i->ID;
		out_Stream << std::endl << "   AnalysisType: " << AnalysisType_string(i->type);

		out_Stream << std::endl << std::endl << "************** Begin FEA Metrics  *****************";
		for each ( CADComputation ii in i->metrics ) out_Stream << ii;
		out_Stream << std::endl << std::endl << "************** End FEA Metrics  *******************";

		////////////////////////////////////////////
		// Solvers
		////////////////////////////////////////////
		out_Stream << std::endl << "   Solvers: ";
		out_Stream << std::endl << "      Solver: ";
		for ( std::list<AnalysisSolver>::const_iterator j( i->analysisSolvers.begin()); j !=  i->analysisSolvers.end(); ++j )
		{
			out_Stream << std::endl << "         Type:             " << ProAnalysisSolverType_string(j->type);
			out_Stream << std::endl << "         MeshType:         " << ProAnalysisMeshType_string(j->meshType);
			out_Stream << std::endl << "         ShellElementType: " << ProAnalysisShellElementType_string(j->shellElementType);
			out_Stream << std::endl << "         ElementShapeType: " << ProAnalysisElementShapeType_string(j->elementShapeType);
		}

		////////////////////////////////////////////
		// Constraints
		////////////////////////////////////////////
		out_Stream << std::endl << "   AnalysisConstraints: ";

		for ( std::list<AnalysisConstraint>::const_iterator j( i->analysisConstraints.begin()); j != i->analysisConstraints.end(); ++j )
		{
			out_Stream << std::endl << "      AnalysisConstraint: ";
			stream_AnalysisGeometry( j->geometry, "         ", out_Stream );

			// Displacement	
			if ( j->analysisDisplacementDefined )
			{
				out_Stream << std::endl << "            Displacement: Units: "  << CADUnitsDistance_string(j->displacement.translation.units) <<  ", alpha beta gamma " << 
						AnalysisConstraintFixedFree_string(j->displacement.translation.x_FixedFree) << "  " << j->displacement.translation.x << "  " <<
						AnalysisConstraintFixedFree_string(j->displacement.translation.y_FixedFree)  << "  " << j->displacement.translation.y << "  " <<
						AnalysisConstraintFixedFree_string(j->displacement.translation.z_FixedFree) << "  " << j->displacement.translation.z;

				out_Stream << std::endl << "            Rotation: Units: "  << CADUnitsAngle_string(j->displacement.rotation.units) <<  ", alpha beta gamma " << 
						AnalysisConstraintFixedFree_string(j->displacement.rotation.alpha_FixedFree) << "  " << j->displacement.rotation.alpha << "  " <<
						AnalysisConstraintFixedFree_string(j->displacement.rotation.beta_FixedFree)  << "  " << j->displacement.rotation.beta << "  " <<
						AnalysisConstraintFixedFree_string(j->displacement.rotation.gamma_FixedFree) << "  " << j->displacement.rotation.gamma;
			}
			// Pin
			if ( j->analysisPinDefined )
			{
				out_Stream << std::endl << "         Pin: (First two Points: Define the Axis, Third Point: Defines Radius";
				out_Stream << std::endl << "            AxialRotation: "     << AnalysisConstraintFixedFree_string(j->pin.axialRotationConstraint);   
				out_Stream << std::endl << "            AxialDisplacement: " << AnalysisConstraintFixedFree_string(j->pin.axialDisplacementConstraint);   
			}

			// Ball
			if ( j->analysisBallDefined )
			{
				out_Stream << std::endl << "         Ball (First Point: Center, Second Point: Defines Radius) ";
			}

			// ConvectionBoundary
			if ( j->convectionBoundaryDefined )
			{
				out_Stream << std::endl << "         ConvectionBoundary:";
				out_Stream << std::endl << "            ConvectionCoefficient:      " << j->convectionBoundary.convectionCoefficient;
				out_Stream << std::endl << "            ConvectionCoefficient unit: " << j->convectionBoundary.unit;
				//if ( j->convectionBoundary.ambientTemperatureDefined )
				//	out_Stream << std::endl << "            AmbientTemperature: " << j->convectionBoundary.ambientTemperature;
				//else
				//	out_Stream << std::endl << "            AmbientTemperature: " << "Not defined";
			}

		}	

		////////////////////////////////////////////
		// Loads
		////////////////////////////////////////////
		out_Stream << std::endl << "   Loads: ";

		for ( std::list<AnalysisLoad>::const_iterator j( i->analysisLoads.begin()); j != i->analysisLoads.end(); ++j )
		{
			out_Stream << std::endl << "      Load: ";
			//out_Stream << std::endl << "         Load Type: " <<  AnalysisLoadType_string( j->type );
			stream_AnalysisGeometry( j->geometry, "         ", out_Stream );
			
			// Force-Moment
			if ( j->forceDefined )
			{
				out_Stream << std::endl << "         Force: ";
				out_Stream << std::endl << "            Force: Units:  "  << CADUnitsForce_string(j->force.force.units) <<  ", x y z " << 
									j->force.force.x << "  " << j->force.force.y << "  " << j->force.force.z;
				out_Stream << std::endl << "            Moment: Units: "  << CADUnitsMoment_string(j->force.moment.units) <<  ", x y z " << 
							j->force.moment.x << "  " << j->force.moment.y << "  " << j->force.moment.z;
			}

			// Pressure
			if ( j->pressureDefined )
			{
				out_Stream << std::endl << "         Pressure: ";
				out_Stream << std::endl << "            Pressure: Units:  "  << CADUnitsPressure_string(j->pressure.units) <<  ", Value: " <<  j->pressure.value;

			}

			// Acceleration
			if ( j->accelerationDefined)
			{
				out_Stream << std::endl << "         Acceleration: ";
				out_Stream << std::endl << "            Acceleration: Units:  "  << CADUnitsAcceleration_string(j->acceleration.units) << 
					", Directions x y z " << j->acceleration.direction.x << "  " << j->acceleration.direction.y << "  " << j->acceleration.direction.z;
			}

			// GridPointInitialTemperature
			if ( j->gridPointInitialTemperatureDefined)
			{
				out_Stream << std::endl << "         GridPointInitialTemperature: ";
				out_Stream << std::endl << "            Value:  "  << j->gridPointInitialTemperature.value;
				out_Stream << std::endl << "            Unit:   "  << j->gridPointInitialTemperature.unit;
			}

			// GridPointInitialTemperature
			if ( j->gridPointTemperatureDefined)
			{
				out_Stream << std::endl << "         GridPointTemperature: ";
				out_Stream << std::endl << "            Value:  "  << j->gridPointTemperature.value;
				out_Stream << std::endl << "            Unit:  "  <<  j->gridPointTemperature.unit;
			}

			// HeatFlux
			if ( j->heatFluxDefined)
			{
				out_Stream << std::endl << "         HeatFlux: ";
				out_Stream << std::endl << "            Value:  "  << j->heatFlux.value;
				out_Stream << std::endl << "            Unit:   "  << j->heatFlux.unit;
			}

			// HeatGeneration
			if ( j->heatGenerationDefined)
			{
				out_Stream << std::endl << "         HeatGeneration: ";
				out_Stream << std::endl << "            Value:  "  << j->heatGeneration.value;
				out_Stream << std::endl << "            Unit:   "  << j->heatGeneration.unit;
			}

			// AmbientTemperature
			if (j->ambientTemperatureDefined)
			{
				out_Stream << std::endl << "         AmbientTemperature: ";
				out_Stream << std::endl << "            Value:  "  << j->ambientTemperature.value;
				out_Stream << std::endl << "            Unit:   "  << j->ambientTemperature.unit;
			}

		} // End for ( std::list<AnalysisLoad>::const_iterator

		////////////////////////////////////////////
		// Part Interfaces
		////////////////////////////////////////////
		out_Stream << std::endl << "   PartInterfaces: ";
		out_Stream << std::endl << "      PartInterface: ";

		for ( std::list<AnalysisPartInterface>::const_iterator j( i->analysisPartInterfaces.begin()); j != i->analysisPartInterfaces.end(); ++j )
		{
			out_Stream << std::endl << "         Type: " << AnalysisPartInterfaceType_string(j->type) << ", ComponentID_1: " << j->componentID_1 << ", ComponentID_2: " << j->componentID_2 ;
		}

		////////////////////////////////////////////
		// Analysis Component IDs
		////////////////////////////////////////////
		// 8/262013 Old approach, replaced with Metrics
		//out_Stream << std::endl << "   AnalysisComponents: ";
		//out_Stream << std::endl << "      AnalysisComponent: ";
		//for ( std::list<CADAnalysisComponent>::const_iterator j( i->cADAnalysisComponents.begin()); j != i->cADAnalysisComponents.end(); ++j )
		//{
		//	out_Stream << std::endl << "         Component Instance ID:   " << j->componentID;
		//	out_Stream << std::endl << "         Infinite Cycle: " << j->infiniteCycle;
		//	for ( std::list<CADAnalysisComponentMetrics>::const_iterator k(j->cadAnalysisComponentMetrics.begin());
		//		  k != j->cadAnalysisComponentMetrics.end();
		//		  ++k )
		//	{
		//		out_Stream << std::endl << "            Metrics  ID: " << k->type <<  " Type: " << k->type;
		//	}
		//
		//}

		////////////////////////////////////////////
		// Analysis Metrics
		////////////////////////////////////////////
		out_Stream << std::endl << "   AnalysisMetrics: ";
		for each ( CADComputation j in i->metrics ) out_Stream << j;


	} // End for ( std::list<AnalysisFEA>::const_iterator

	///////////////////////////////////////////
	//  Ballistic Analyses
	///////////////////////////////////////////
	for each ( const AnalysisBallistic i in in_CADAnalyses.analysesBallistic)
	{
		out_Stream << std::endl << "   AnalysisType:  Ballistic";
		out_Stream << std::endl << "   ReferencePlanes: ";

		out_Stream << std::endl << std::endl << "************** Begin Ballistic Metrics  *****************";
		for each ( CADComputation ii in i.metrics ) out_Stream << ii;
		out_Stream << std::endl << std::endl << "************** End Ballistic Metrics  *******************";

		for each ( const e_ReferencePlane j in i.referencePlanes )
		{
			out_Stream << std::endl << "      "  << ReferencePlane_string(j);
		}
		out_Stream << std::endl << "   Shotlines: ";
		for each ( const Shotline k in i.shotlines )
		{
			out_Stream << std::endl << "      name:                   " <<  k.name;
			out_Stream << std::endl << "      ballisticThreatRef:     " <<  k.ballisticThreatRef;
			out_Stream << std::endl << "      datumPoint_ComponentID: " <<  k.datumPoint_ComponentID;
			out_Stream << std::endl << "      datumPoint:             " <<  k.datumPoint;
			out_Stream << std::endl << "      azimuth:                " <<  k.azimuth;
			out_Stream << std::endl << "      elevation:              " <<  k.elevation;
		}
	}

	///////////////////////////////////////////
	//  Blast Analyses
	///////////////////////////////////////////
	for each ( const AnalysisBlast i in in_CADAnalyses.analysesBlast)
	{
		out_Stream << std::endl << "   AnalysisType:  Blast";
		out_Stream << std::endl << "   ReferencePlanes: ";
		out_Stream << std::endl << std::endl << "************** Begin Blast Metrics  *****************";
		for each ( CADComputation ii in i.metrics ) out_Stream << ii;
		out_Stream << std::endl << std::endl << "************** End Blast Metrics  *******************";

		for each ( const e_ReferencePlane j in i.referencePlanes )
		{
			out_Stream << std::endl << "      "  << ReferencePlane_string(j);
		}
	}

	///////////////////////////////////////////
	//  CFD Analyses
	///////////////////////////////////////////
	for each ( const AnalysisCFD i in in_CADAnalyses.analysesCFD)
	{
		out_Stream << std::endl << "   AnalysisType:  CFD";
		out_Stream << std::endl << "   ReferencePlanes: ";
		out_Stream << std::endl << "   HydrostaticsSolverSettings:";
		out_Stream << std::endl << "      pitchAngleStart:    " << i.cADHydrostaticParameters.pitchAngleStart;		
		out_Stream << std::endl << "      pitchAngleEnd:      " << i.cADHydrostaticParameters.pitchAngleEnd;			
		out_Stream << std::endl << "      pitchAngleStepSize: " << i.cADHydrostaticParameters.pitchAngleStepSize;	

		out_Stream << std::endl << "      rollAngleStart:     " << i.cADHydrostaticParameters.rollAngleStart;		
		out_Stream << std::endl << "      rollAngleEnd:       " << i.cADHydrostaticParameters.rollAngleEnd;			
		out_Stream << std::endl << "      rollAngleStepSize:  " << i.cADHydrostaticParameters.rollAngleStepSize;		

		out_Stream << std::endl << "      fluidMaterial:      " << i.cADHydrostaticParameters.fluidMaterial;		
		out_Stream << std::endl << "      fluidTemperature:   " << i.cADHydrostaticParameters.fluidTemperature;				

		out_Stream << std::endl << "************** Begin CFD Metrics  *****************";
		for each ( CADComputation ii in i.metrics ) out_Stream << ii;
		out_Stream << std::endl << "************** End CFD Metrics  *******************";

		//for each ( const e_ReferencePlane j in i.referencePlanes )
		//{
		//	out_Stream << std::endl << "      "  << ReferencePlane_string(j);
		//}
		out_Stream << std::endl << ReferencePlane_string( i.referencePlane );

	}

}

} // END namespace isis