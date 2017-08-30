
#include <ProToolkit.h>
#include "CadFactoryCreo.h"
#include <vector>
#include <list>
#include <ProFeature.h>
#include <CadFactoryCreo.h>
#include <JointCreo.h>
#include "ApplyModelConstraints.h"
#include <ISISConstants.h>
#include <ToolKitPassThroughFunctions.h>
#include <cc_CommonUtilities.h>

namespace isis {
namespace cad {
namespace creo {

CadFactoryAbstract::ptr create() {
	return CadFactoryAbstract::ptr( new CadFactoryCreo() );
}

void* AssemblerCreo::get_assembly_component
		( const std::string in_working_directory,
		  const std::string &	in_id, 
		  std::map<std::string, isis::CADComponentData> &	in_map)
{
	// Delete assembly file if it exists. 
	// This function will rebuild the assembly.
	isis::IfFileExists_DeleteFile( in_working_directory + "\\" +
		(const std::string&) in_map[in_id].name + ".asm.*");

	MultiFormatString templateModelName
		(isis::TEMPLATE_MODEL_NAME_METRIC, 
		PRO_NAME_SIZE - 1);
	isis::isis_ProMdlfileCopy ( (ProMdlType)PRO_ASSEMBLY, 
								templateModelName,
								in_map[in_id].name);

	isis::isis_ProMdlRetrieve_WithDescriptiveErrorMsg(
		in_id, 
		in_map[in_id].name, 
		in_map[in_id].geometryRepresentation,
		in_map[in_id].name,
		(ProMdlType)PRO_ASSEMBLY, (ProMdl*)&assembly_model);  

		//in_map[in_id].modelType = PRO_MDL_ASSEMBLY;
		in_map[in_id].modelType = CAD_MDL_ASSEMBLY;
	return reinterpret_cast<void*>(&assembly_model);
}

std::vector<Joint> get_joints(std::vector<ProFeature> in_datums) {
	std::vector<Joint> joints(in_datums.size());
	for each (ProFeature feat in in_datums) {
		joints.push_back( convert_datum( feat ) );
	}
	return joints;
}

/**
For each constraint pair defined for the component,
extract the constraint feature.
Separate the pair into added and base components.
*/
std::vector< Joint::pair_t >  AssemblerCreo::extract_joint_pair_vector
	(std::string in_component_id, 
	 std::vector<ConstraintPair> in_component_pair_vector,
	 std::map<string, isis::CADComponentData> &	in_CADComponentData_map)
{
	std::vector< Joint::pair_t>  elementary_joints;

	for (std::vector<ConstraintPair>::const_iterator 
		k(in_component_pair_vector.begin());
		k != in_component_pair_vector.end(); 
		++k )
	{
		std::stringstream err_str;
		const ConstraintPair& pair = *k;
		std::list<int>  model_path_list[2];

		ProAsmcompConstrType constraint_type = ProAsmcompConstrType_enum(pair.featureAlignmentType);
		ProType	 pro_datum_type =  FeatureGeometryType_enum(pair.featureGeometryType);	
		//ProType	 pro_datum_type =  pair.featureGeometryType;	

		const wchar_t* model_datum_name[2];
		// enum PRO_DATUM_SIDE_YELLOW (SIDE_A), 
		// PRO_DATUM_SIDE_RED (SIDE_B), 
		// PRO_DATUM_SIDE_NONE
		ProDatumside model_datum_side[2]; 
		bool model_in_family_tree[2];
		string model_constraint_feature_component_ID[2];

		int ix = 0;
		for (	std::vector<ConstraintFeature>::const_iterator l(k->constraintFeatures.begin());
				l != k->constraintFeatures.end(); ++l, ++ix )
		{												
			const ConstraintFeature& feature = *l;
			// is myself or one of my decendants
			model_in_family_tree[ix] = 
				feature.componentInstanceID == in_component_id 
				|| ComponentIDChildOf( feature.componentInstanceID, in_component_id, in_CADComponentData_map );

			model_path_list[ix] = in_CADComponentData_map[feature.componentInstanceID].componentPaths;
			model_datum_name[ix] = feature.featureName;
			model_datum_side[ix] = ProDatumside_enum(feature.featureOrientationType);
			model_constraint_feature_component_ID[ix] = feature.componentInstanceID;
		}

		// make sure that it is a proper pair
		if ( model_in_family_tree[0] == model_in_family_tree[1])
		{
			err_str << "Erroneous constraint pair.  "
				<< " Assembled Component Instance ID: " << in_component_id 
				<< " Constraint Feature Component IDs: " 
					<< model_constraint_feature_component_ID[1] << ", " 
					<< model_constraint_feature_component_ID[0];
			throw isis::application_exception(err_str);  
		}

		Joint::pair_t joint_pair;
		for(int ix=0; ix < 2; ++ix) {
			ProIdTable  model_id_table; 
			int  model_id_table_size;
			Populate_c_id_table( model_path_list[ix], model_id_table, model_id_table_size ); 
			ProAsmcomppath  component_path;
			isis::isis_ProAsmcomppathInit (	assembly_model, model_id_table,
					model_id_table_size, &component_path);

			ProMdl  component_model;
			isis::isis_ProAsmcomppathMdlGet( &component_path, &component_model);

			ProModelitem datum_model; 
			isis::isis_ProModelitemByNameInit_WithDescriptiveErrorMsg (
				in_component_id, 
				in_CADComponentData_map[in_component_id].name,
				ProMdlType_enum(in_CADComponentData_map[in_component_id].modelType),
				component_model, pro_datum_type, model_datum_name[ix], 
				&datum_model);

			// it will probably be a ProGeomItem
			// we need the ProFeature object.
			ProFeature datum_feature;

			switch (datum_model.type) {
			case PRO_CSYS:
			case PRO_SURFACE:
			case PRO_AXIS:
			case PRO_POINT:
				ProGeomitemFeatureGet
					(static_cast<ProGeomitem*>(&datum_model), &datum_feature);
				break;
			case PRO_DATUM_PLANE:
				datum_feature = static_cast<ProFeature>(datum_model);
				break;
			default:
				err_str  
					<< "model item type [" << datum_model.type
					<< "] for datum feature [" << model_datum_name[ix]
					<< "] ";
				throw isis::application_exception(err_str);  
			}

			switch (datum_feature.type) {
			case PRO_DATUM_PLANE:
			case PRO_FEATURE:
				{
					Joint joint = convert_datum( datum_feature );
					if( model_in_family_tree[ix] ) {
						joint_pair.first = joint;
					} else {
						joint_pair.second = joint;
					}
				}
				break;
			default:
				err_str << "not a proper feature: " << datum_feature.type;
				throw isis::application_exception(err_str);  
			}
		}
		elementary_joints.push_back( joint_pair );
	}
	return elementary_joints;
}

} // creo
} // cad
} // isis
