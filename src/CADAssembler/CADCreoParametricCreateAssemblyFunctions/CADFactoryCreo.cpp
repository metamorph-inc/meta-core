
#include <ProToolkit.h>
#include "CadFactoryCreo.h"
#include <vector>
#include <list>
#include <ProFeature.h>
#include <JointCreo.h>
#include "ApplyModelConstraints.h"
#include <ISISConstants.h>
#include <ToolKitPassThroughFunctions.h>
#include <cc_CommonUtilities.h>
#include "CADEnvironmentSettings.h"

namespace isis {
namespace cad {
namespace creo {

// Forward declare
void writeMetaLinkConfigProFile(const ::boost::filesystem::path &workingDir, const isis::MetaLinkInputArguments &programInputArguments);

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

/////////////////////////////////////////////////////////////////////////////////////////
/**************
std::string EnvironmentCreo::getCADExtensionsDir() throw (isis::application_exception)
{
	std::string META_PATH_str = META_PATH();

	if ( META_PATH_str.length() == 0 )
	{
		std::stringstream errorString;
		errorString << "Function - " << __FUNCTION__ << ", " << std::endl <<
					"Registry entry META_PATH not found.  This should typically be in the registry" << std::endl <<
					"at HKEY_LOCAL_MACHINE\\SOFTWARE\\META.  The META installer creates this registry entry." << std::endl;

		throw isis::application_exception(errorString);	
	}
	return META_PATH_str + "\\bin\\CAD\\Creo";
}
**************/


/////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////
void EnvironmentCreo::setupCADEnvironment(	
			const CreateAssemblyInputArguments &in_CreateAssemblyInputArguments,
			std::string		&out_CADStartCommand,	
			std::string		&out_CADExtensionsDir,
			std::string		&out_TemplateFile_PathAndFileName ) const  throw (isis::application_exception)
{

		SetCreoEnvirVariable_RetrieveSystemSettings(    in_CreateAssemblyInputArguments.graphicsModeOn,
														false,    // CADExceptInputFromThisProgramAndCreoUI
														out_CADStartCommand,
														out_CADExtensionsDir,	
														out_TemplateFile_PathAndFileName );


		/////////////////////
		// Write config.pro
		/////////////////////

		// Get the complete path to the working directory
		std::string workingDir_string = in_CreateAssemblyInputArguments.workingDirectory;
		::boost::filesystem::path    workingDir_path;
		workingDir_path = isis::SetupWorkingDirectory( workingDir_string );
													
		std::string configPro_PathAndFileName = workingDir_path.generic_string() + "\\config.pro";
		isis::IfFileExists_DeleteFile( configPro_PathAndFileName);

		ofstream config_Pro;
		config_Pro.open (configPro_PathAndFileName );
		config_Pro << "override_store_back yes\n";
		std::string searchMetaFileName = ".\\search_META.pro";
		if ( isis::FileExists( searchMetaFileName.c_str() )) {
			config_Pro << "search_path_file " <<  searchMetaFileName;
		}

		std::string auxiliaryCADDirectory = in_CreateAssemblyInputArguments.auxiliaryCADDirectory;

		if ( auxiliaryCADDirectory.size() > 0 )
		{
			// Check if in_AuxiliaryCADDirectory is in double quotes.  If not add double quotes.
			// This is necessary because Creo Config options with spaces must be enclosed in double quotes.
			if ( auxiliaryCADDirectory.find("\"")  == string::npos )	auxiliaryCADDirectory = "\"" + auxiliaryCADDirectory + "\"";
			config_Pro << std::endl << "search_path " <<  auxiliaryCADDirectory;
		}
		
		config_Pro << std::endl << "pro_material_dir " << CreoMaterialMTLFilesDir_Path();

		// add component creo plugin if in graphics mode
		// config_Pro << std::endl << "toolkit_registry_file  \"" << META_PATH() << "bin\\CAD\\Creo\\plugins\\protk.dat\"";

		config_Pro.close();
}


void EnvironmentCreo::setupCADEnvironment(	
		const MetaLinkInputArguments &in_MetaLinkInputArguments,
		std::string		&out_CADStartCommand,	
		std::string		&out_CADExtensionsDir,
		std::string		&out_TemplateFile_PathAndFileName ) const throw (isis::application_exception)
{

		SetCreoEnvirVariable_RetrieveSystemSettings(    in_MetaLinkInputArguments.graphicsModeOn,
														in_MetaLinkInputArguments.synchronizeWithCyPhy,    // CADExceptInputFromThisProgramAndCreoUI
														out_CADStartCommand,
														out_CADExtensionsDir,	
														out_TemplateFile_PathAndFileName );

		// Get the complete path to the working directory
		std::string workingDir_string = in_MetaLinkInputArguments.workingDirectory;
		::boost::filesystem::path    workingDir_path;
		workingDir_path = isis::SetupWorkingDirectory( workingDir_string );

		writeMetaLinkConfigProFile( workingDir_path, in_MetaLinkInputArguments );

}


void EnvironmentCreo::setupCADEnvironment(	
			const ExtractACMInputArguments &in_ExtractACMInputArguments,
			std::string		&out_CADStartCommand,	
			std::string		&out_CADExtensionsDir,
			std::string		&out_TemplateFile_PathAndFileName ) const throw (isis::application_exception)
{

	SetCreoEnvirVariable_RetrieveSystemSettings( in_ExtractACMInputArguments.graphicsModeOn,
												false,    // CADExceptInputFromThisProgramAndCreoUI
												out_CADStartCommand,
												out_CADExtensionsDir,	
												out_TemplateFile_PathAndFileName );
}

/////////////////////////////////////////////
//  Should move the following code elsewhere
/////////////////////////////////////////////

void writeMetaLinkConfigProFile(const ::boost::filesystem::path &workingDir, const isis::MetaLinkInputArguments &programInputArguments)
{
		
	    ofstream config_Pro;
        ::boost::filesystem::path configPro_PathAndFileName = workingDir / "config.pro";
        config_Pro.open(configPro_PathAndFileName.string());
        config_Pro << "override_store_back yes\n";
        config_Pro << "enable_sociallink NO\n";

        ::boost::filesystem::path searchMetaFileName = "./search_META.pro";
        if(::boost::filesystem::exists(searchMetaFileName))
        {
            config_Pro << "search_path_file " <<  searchMetaFileName.string();
        }

        ::boost::filesystem::path cadPartsLibDir = programInputArguments.auxiliaryCADDirectory;
        if(::boost::filesystem::exists(cadPartsLibDir))
        {
            config_Pro << std::endl << "search_path " << "\"" << cadPartsLibDir.string() << "\"" << std::endl;
        }

        config_Pro << std::endl << "pro_material_dir " << isis::CreoMaterialMTLFilesDir_Path();

        // protk.dat configuration information
        std::string metaPath = isis::META_PATH();
        if(metaPath == "")
        {
            std::string msg = "META_PATH registry value is not set";
            throw isis::application_exception(msg);
        }
        ::boost::filesystem::path metaPathPath(metaPath);
        if(! ::boost::filesystem::is_directory(metaPathPath))
        {
            std::stringstream msg;
            msg << "META_PATH registry value is set but no such directory exists: "
                << metaPathPath.generic_string();
            throw isis::application_exception(msg);
        }

		if (std::getenv("HUDAT_INSTALLDIR") != NULL)
		{
			::boost::filesystem::path protkPath = metaPathPath / "bin" / "CAD" / "Creo" / "plugins" / "protk_hudat.dat";
			if (::boost::filesystem::is_regular_file(protkPath))
			{
				isis_LOG(lg, isis_FILE, isis_INFO) << " HuDat present : using custom protk_hudat.dat (" << protkPath.generic_string() << ")" << isis_EOL;
				config_Pro << std::endl << "toolkit_registry_file  " << "" << protkPath.string() << "" << std::endl;
			}
		}

        string treecfgfile = programInputArguments.is_designMode()?"tree_design_edit.cfg":"tree_component_edit.cfg";

        // only add the following line to the config when in design mode
        ::boost::filesystem::path modelTreeConfigPath = metaPathPath / "bin" / "CAD" / "Creo" / "plugins" / treecfgfile;
        if(! ::boost::filesystem::is_regular_file(modelTreeConfigPath))
        {
            isis_LOG(lg, isis_FILE, isis_WARN) << "the model tree config file file has a problem (doesn't exist?): "
                                         << modelTreeConfigPath.string();
            config_Pro << std::endl << "# ";
        }
        else
        {
            config_Pro << std::endl;
        }
        // config_Pro << "mdl_tree_cfg_file $PROE_ISIS_EXTENSIONS\plugins\tree.cfg" << std::endl;
        config_Pro << "mdl_tree_cfg_file  " << "" << modelTreeConfigPath.string() << "" << std::endl;

        if(programInputArguments.configPro.length()>0)
        {
            ifstream is(programInputArguments.configPro);
            config_Pro << is.rdbuf();
            config_Pro << std::endl;
        }

        config_Pro.close();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void ModelNamesCreo::extractModelNameAndFamilyTableEntry(	const std::string	&in_OrigName, 
															std::string			&out_ModelName,
															std::string			&out_FamilyTableEntry,
															bool				&out_FamilyTableModel ) const throw (isis::application_exception)
{
	size_t lessThanPos = in_OrigName.find('<');  // Family table would have the form Chassis_8_Wheel<Chassis>

	if ( lessThanPos != std::string::npos  )
	{  
		/////////////////
		// Family Table 
		/////////////////

		size_t greaterThanPos =  in_OrigName.find('>');

		if ( greaterThanPos == std::string::npos  )
		{
			std::string errorString;
					errorString = "Function - " + std::string(__FUNCTION__) + ", found a \"<\" but a closing \">\" was not found.  " +
						std::string("in_OrigName: ") + 	in_OrigName;
					throw isis::application_exception("C03002",errorString.c_str());
		}

		out_ModelName= in_OrigName.substr(lessThanPos + 1, greaterThanPos - lessThanPos - 1 );
		out_FamilyTableEntry = in_OrigName.substr(0, lessThanPos );
		out_FamilyTableModel = true;
	}
	else
	{
		out_ModelName = in_OrigName;
		out_FamilyTableEntry = "";
		out_FamilyTableModel = false;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////
} // creo
} // cad
} // isis
