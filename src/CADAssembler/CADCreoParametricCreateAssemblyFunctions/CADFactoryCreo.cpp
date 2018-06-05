#include "CadFactoryCreo.h"
#include <ProToolkit.h>
#include <vector>
#include <list>
#include <ProFeature.h>
#include <JointCreo.h>
#include "ApplyModelConstraints.h"
#include <ISISConstants.h>
#include <ToolKitPassThroughFunctions.h>
#include <cc_CommonUtilities.h>
#include "CADEnvironmentSettings.h"
#include "CommonFeatureUtils.h"
#include "CommonFunctions.h"
#include "AssembleUtils.h"
#include "ModelMaterials.h"
#include <cc_ApplyModelConstraints.h>


namespace isis {
namespace cad {
namespace creo {

const int MAX_STRING_PARAMETER_LENGTH = 79;

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
	(std::string in_component_insctance_id, 
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
				feature.componentInstanceID == in_component_insctance_id 
				|| ComponentIDChildOf( feature.componentInstanceID, in_component_insctance_id, in_CADComponentData_map );

			model_path_list[ix] = in_CADComponentData_map[feature.componentInstanceID].componentPaths;
			model_datum_name[ix] = feature.featureName;
			model_datum_side[ix] = ProDatumside_enum(feature.featureOrientationType);
			model_constraint_feature_component_ID[ix] = feature.componentInstanceID;
		}

		// make sure that it is a proper pair
		if ( model_in_family_tree[0] == model_in_family_tree[1])
		{
			err_str << "Erroneous constraint pair.  "
				<< " Assembled Component Instance ID: " << in_component_insctance_id 
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
				in_component_insctance_id, 
				in_CADComponentData_map[in_component_insctance_id].name,
				ProMdlType_enum(in_CADComponentData_map[in_component_insctance_id].modelType),
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
std::string CADSessionCreo::getCADExtensionsDir() throw (isis::application_exception)
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
void CADSessionCreo::startCADProgram( const std::string &in_StartCommand ) const throw (isis::application_exception)
{
	char tempBuffer[1024];
	strcpy(tempBuffer, in_StartCommand.c_str() );
	isis::isis_ProEngineerStart(tempBuffer,"");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void CADSessionCreo::stopCADProgram() const throw (isis::application_exception)
{
	isis::isis_ProEngineerEnd();
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void CADSessionCreo::getCADProgramVersion(	bool	&out_IntVersionNumber_Set,  
											int		&out_IntVersionNumber, 
											bool	&out_FloatVersionNumber_Set,
											double  &out_FloatVersionNumber )  const throw (isis::application_exception)
{
	out_IntVersionNumber_Set = false;
	out_FloatVersionNumber_Set = false;
	out_IntVersionNumber = 0;
	out_FloatVersionNumber = 0.0;

	int creoVersionNumber;
	isis_ProEngineerReleaseNumericversionGet(&creoVersionNumber);

	out_IntVersionNumber = creoVersionNumber;
	out_IntVersionNumber_Set = true;
}

//	"static wchar_t *setCreoWorkingDirectory_buffer" MUST be static.
//	This is necessary  because the working directory buffer (e.g. setCreoWorkingDirectory_buffer)
//	in the setCreoWorkingDirectory function must be persisted between calls to isis_ProDirectoryChange.
//	This is because after the initial call to isis_ProDirectoryChange, isis_ProDirectoryChange appears to 
//	access the address of the buffer used in the previous call.  Therefore, the same address must be used 
//	between calls, or at least, the previously used buffer address must still be valid.
//
// This function is NOT thread safe.
void CADSessionCreo::setCADWorkingDirectory ( const MultiFormatString &in_MultiFormatString ) throw (isis::application_exception)
{
	
	cADWorkingDirectory = in_MultiFormatString;

	static wchar_t *setCreoWorkingDirectory_buffer = NULL;

	if ( !setCreoWorkingDirectory_buffer) setCreoWorkingDirectory_buffer = new wchar_t[PRO_PATH_SIZE];
		
	wcscpy_s( setCreoWorkingDirectory_buffer, PRO_PATH_SIZE, (const wchar_t*)in_MultiFormatString);
	isis::isis_ProDirectoryChange( setCreoWorkingDirectory_buffer );
}
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
void CADSessionCreo::setupCADEnvironment(	
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


void CADSessionCreo::setupCADEnvironment(	
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


void CADSessionCreo::setupCADEnvironment(	
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
void ModelHandlingCreo::extractModelNameAndFamilyTableEntry(	const std::string	&in_OrigName, 
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
std::string ModelHandlingCreo::buildAFamilyTableCompleteModelName ( const std::string &in_ModelName,
																const std::string &in_FamilyTableEntry )
{
	return in_FamilyTableEntry + "<" + in_ModelName + ">";

}
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string ModelHandlingCreo::combineCADModelNameAndSuffix ( const std::string &in_ModelName, e_CADMdlType in_ModelType )
															throw (isis::application_exception)
{
	if ( in_ModelName.size() == 0 )
	{
		std::stringstream errorString;
		errorString <<
		"exception: Function " + std::string(__FUNCTION__) + " was passed a null string for in_ModelName.";
		throw isis::application_exception(errorString.str());
	}

	std::string tempString;
	switch (in_ModelType)
	{
		case CAD_MDL_PART:
			tempString = in_ModelName + ".prt";
			break;
		case CAD_MDL_ASSEMBLY:
			tempString = in_ModelName + ".asm";
			break;
		default:
			std::stringstream errorString;
			errorString <<
			"exception: Function " + std::string(__FUNCTION__) + " was passed in_ModelType that was not CAD_MDL_PART or CAD_MDL_ASSEMBLY, in_ModelType: " << in_ModelType;
			throw isis::application_exception(errorString.str());
		
	}
	return tempString;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
void ModelHandlingCreo::cADModelRetrieve(	const isis::MultiFormatString	&in_ModelName, 
											e_CADMdlType 					in_ModelType,      
											void 							**out_RetrievedModelHandle_ptr ) const throw (isis::application_exception)
{

	isis::isis_ProMdlRetrieve(in_ModelName, ProMdlType_enum(in_ModelType), out_RetrievedModelHandle_ptr);

}
///////////////////////////////////////////////////////////////////////////////////////////////////
void ModelHandlingCreo::cADModelRetrieve(	const std::string			&in_ComponentInstanceID,		
									const isis::MultiFormatString		&in_ModelName,				
									e_CADMdlType 						in_ModelType,				
									const MultiFormatString				&in_GeometryRepresentation, 
									void 								**out_RetrievedModelHandle_ptr ) const throw (isis::application_exception)
{

	isis_ProMdlRetrieve_WithDescriptiveErrorMsg( 
											// Added Arguments
											in_ComponentInstanceID,
											in_ModelName,
											in_GeometryRepresentation,
											// Original arguments
											in_ModelName, 
											ProMdlType_enum(in_ModelType),
											out_RetrievedModelHandle_ptr);

}

///////////////////////////////////////////////////////////////////////////////////////////////////

void  ModelHandlingCreo::cADModelSave( void	*in_ModelHandle ) const throw (isis::application_exception)
{
	isis::isis_ProMdlSave(in_ModelHandle);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void  ModelHandlingCreo::cADModelFileCopy (	e_CADMdlType 						in_ModelType,
											const isis::MultiFormatString		&in_FromModelName,
											const isis::MultiFormatString       &in_ToModelName) const throw (isis::application_exception)
{
	isis::isis_ProMdlfileCopy (ProMdlType_enum(in_ModelType), in_FromModelName, in_ToModelName);
}
///////////////////////////////////////////////////////////////////////////////////////////////////
 void ModelHandlingCreo::cADModelSave(	const std::string									&in_ComponentInstanceID,
										const std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map )
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

	//isis::isis_ProMdlSave(in_CADComponentData_map[in_ComponentInstanceID].cADModel_hdl);
	isis::isis_ProMdlSave(itr->second.cADModel_hdl);

 }
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
void ModelOperationsCreo::forEachLeafAssemblyInTheInputXML_AddInformationAboutSubordinates( 
				const std::vector<std::string>					&in_ListOfComponentInstanceIDsInTheAssembly, // This includes the assembly component ID
				int												&in_out_NonCyPhyID_counter,
				std::map<std::string, isis::CADComponentData>	&in_out_CADComponentData_map )
																	throw (isis::application_exception)
{

		
	for each ( const std::string &i in in_ListOfComponentInstanceIDsInTheAssembly )
	{			
		//if ( in_out_CADComponentData_map[i].modelType == PRO_MDL_ASSEMBLY && in_out_CADComponentData_map[i].children.size() == 0 )
		if ( in_out_CADComponentData_map[i].modelType == CAD_MDL_ASSEMBLY && in_out_CADComponentData_map[i].children.size() == 0 )
		{
			// Found an assembly that is a Leaf
			// Fill out the assemblyHierarchy
			CreoModelAssemblyAttributes assemblyHierarchy;

			//isis::RetrieveAssemblyHierarchyInformation(  static_cast<ProSolid>(in_out_CADComponentData_map[i].cADModel_hdl), false, assemblyHierarchy );

			isis::RetrieveAssemblyHierarchyInformation( i, false, in_out_CADComponentData_map, assemblyHierarchy );

			std::stringstream str;
			stream_AssemblyHierarchy (assemblyHierarchy, str);
			isis_LOG(lg, isis_FILE, isis_INFO) << str.str();

			bool checkExclusion_by_SimplifiedRep = false;

			std::map<int, CAD_SimplifiedRepData> featureID_to_SimplifiedRepData_map;
			//if ( in_out_CADComponentData_map[i].modelType == PRO_MDL_ASSEMBLY && 
			if ( in_out_CADComponentData_map[i].modelType == CAD_MDL_ASSEMBLY && 
					in_out_CADComponentData_map[i].geometryRepresentation.size() > 0 )  
			{
				///////////////////////////////////////////////////////////////////////////////////////
				// Build map of child feature IDs and with indication if they are included or excluded
				///////////////////////////////////////////////////////////////////////////////////////
				ProSimprep proSimprep_temp;
				ProError	proError_temp  = ProSimprepInit ((wchar_t*)(const wchar_t*) in_out_CADComponentData_map[i].geometryRepresentation,
														-1,
														static_cast<ProSolid>(in_out_CADComponentData_map[i].cADModel_hdl),
														&proSimprep_temp );

				if ( proError_temp == PRO_TK_NO_ERROR )  // Found simplified rep.
				{
					ProMdl ProMdl_temp = in_out_CADComponentData_map[i].cADModel_hdl;
					AssemblySimplifiedRep_RetrieveModelInclusionStatus ( 
								ProMdl_temp,
								proSimprep_temp,
								featureID_to_SimplifiedRepData_map ) ;

					if ( featureID_to_SimplifiedRepData_map.size() > 0 ) checkExclusion_by_SimplifiedRep = true;

					for each ( std::pair<int, CAD_SimplifiedRepData> i_simp in featureID_to_SimplifiedRepData_map)
					{
						isis_LOG(lg, isis_FILE, isis_INFO) <<  "\nSimplified Rep Included/Exclude Info, Feature ID: " << i_simp.first << "  " << CAD_SimplifiedRep_InclusionStatus_string(i_simp.second.inclusionStatus);
					}
				}

			}

			// Temporary Check
			for each ( CreoModelAssemblyAttributes j in assemblyHierarchy.children )
			{
				bool includeThisChild = true;
				if ( checkExclusion_by_SimplifiedRep )
				{				
					if ( featureID_to_SimplifiedRepData_map.find(j.proAsmcomp.id) != featureID_to_SimplifiedRepData_map.end())
					{
						if ( featureID_to_SimplifiedRepData_map[j.proAsmcomp.id].inclusionStatus == CAD_SIMPLIFIED_REP_EXCLUDE )
						{
							isis_LOG(lg, isis_FILE, isis_INFO)<<  "\nExcluding, Feature ID: " << j.proAsmcomp.id;
							includeThisChild = false;
						}
					}
				}

				if ( includeThisChild )
				{
					++in_out_NonCyPhyID_counter;
					std::stringstream nonCyPhyComponentID;
					nonCyPhyComponentID << "NON_CYPHY_ID_" << in_out_NonCyPhyID_counter;
					
					CADComponentData cADComponentData_temp;
					cADComponentData_temp.dataInitialSource = INITIAL_SOURCE_DERIVED_FROM_LEAF_ASSEMBLY_DESCENDANTS;
					cADComponentData_temp.name = j.modelname;
					cADComponentData_temp.modelType = j.modelType;
					cADComponentData_temp.cADModel_hdl = j.p_solid_handle;
					cADComponentData_temp.cyPhyComponent = false;
					cADComponentData_temp.componentID = nonCyPhyComponentID.str();
					cADComponentData_temp.parentComponentID = i;

					//cADComponentData_temp.assembledFeature = j.proAsmcomp;
					//cADComponentData_temp.assembledFeature.type = CADFeatureGeometryType_enum(j.proAsmcomp.type);
					//cADComponentData_temp.assembledFeature.id = j.proAsmcomp.id;
					//cADComponentData_temp.assembledFeature.owner = j.proAsmcomp.owner;		
					cADComponentData_temp.assembledFeature = getCADAssembledFeature( j.proAsmcomp );
						
					cADComponentData_temp.componentPaths = in_out_CADComponentData_map[i].componentPaths;
					cADComponentData_temp.componentPaths.push_back( j.proAsmcomp.id );					

					in_out_CADComponentData_map[nonCyPhyComponentID.str()] = cADComponentData_temp;
					in_out_CADComponentData_map[i].children.push_back(nonCyPhyComponentID.str());

					isis_LOG(lg, isis_FILE, isis_INFO) << "   Adding subordinate information, Modelname: " << cADComponentData_temp.name << ", modelType: " << CADMdlType_string(cADComponentData_temp.modelType);

					if ( j.modelType == CAD_MDL_ASSEMBLY )
					{
							std::vector<std::string> listOfComponentIDs_temp;
							listOfComponentIDs_temp.push_back(nonCyPhyComponentID.str());
							this->forEachLeafAssemblyInTheInputXML_AddInformationAboutSubordinates(
																			listOfComponentIDs_temp,
																			in_out_NonCyPhyID_counter,
																			in_out_CADComponentData_map );
					}
				} // END if ( includeThisChild )
			}
		}
	}
	
}

void ModelOperationsCreo::modify_CADInternalHierarchyRepresentation_CADComponentData__ForCopiedModel( 
					const std::string								&in_TopAssemblyComponentID,
					const isis::CopyModelDefinition						&in_CopyModelDefinition, 
					std::map<std::string, isis::CADComponentData>	&in_out_CADComponentData_map )
																		throw (isis::application_exception) 
{

	if (in_CopyModelDefinition.modelType == CAD_MDL_ASSEMBLY )
	{
		ModelInstanceData modelInstanceData_temp;
		modelInstanceData_temp.modelName				= in_CopyModelDefinition.fromModelName;
		modelInstanceData_temp.modelType				= in_CopyModelDefinition.modelType;
		modelInstanceData_temp.modelHandle				= static_cast<ProSolid>(in_out_CADComponentData_map[in_CopyModelDefinition.componentInstanceID].cADModel_hdl);

		modelInstanceData_temp.assembledFeature = getProAsmcomp(in_out_CADComponentData_map[in_CopyModelDefinition.componentInstanceID].assembledFeature);
								
		modelInstanceData_temp.topAssemblyModelHandle	= static_cast<ProSolid>(in_out_CADComponentData_map[in_TopAssemblyComponentID].cADModel_hdl);
		modelInstanceData_temp.componentPaths			= in_out_CADComponentData_map[in_CopyModelDefinition.componentInstanceID].componentPaths;
		isis::MultiFormatString  CopyToPartName_temp(in_CopyModelDefinition.toModelName);

		ProMdl     renamedModelHandle;
		Assembly_RenameSubPartOrSubAssembly ( modelInstanceData_temp, CopyToPartName_temp, renamedModelHandle );
		in_out_CADComponentData_map[in_CopyModelDefinition.componentInstanceID].cADModel_hdl = (ProSolid)renamedModelHandle;
		in_out_CADComponentData_map[in_CopyModelDefinition.componentInstanceID].cADModel_ptr_ptr = (ProMdl*)&renamedModelHandle;


		// Must fix the children assembledFeature to point to the new parent (new owner)
		for each ( std::string i_child in in_out_CADComponentData_map[in_CopyModelDefinition.componentInstanceID].children )
		{
			in_out_CADComponentData_map[i_child].assembledFeature.owner = renamedModelHandle;
		} 
	}
	

	// For testing
	//isis::isis_ProMdlSave(in_out_CADComponentData_map[i->assemblyComponentID].modelHandle);


	if (in_CopyModelDefinition.modelType == CAD_MDL_PART )
	{
		ModelInstanceData modelInstanceData_temp;
		modelInstanceData_temp.modelName				= in_CopyModelDefinition.fromModelName;
		modelInstanceData_temp.modelType				= in_CopyModelDefinition.modelType;
		modelInstanceData_temp.modelHandle				= static_cast<ProSolid>(in_out_CADComponentData_map[in_CopyModelDefinition.componentInstanceID].cADModel_hdl);

		modelInstanceData_temp.assembledFeature = getProAsmcomp(in_out_CADComponentData_map[in_CopyModelDefinition.componentInstanceID].assembledFeature);


		modelInstanceData_temp.topAssemblyModelHandle	= static_cast<ProSolid>(in_out_CADComponentData_map[in_TopAssemblyComponentID].cADModel_hdl);
		modelInstanceData_temp.componentPaths			= in_out_CADComponentData_map[in_CopyModelDefinition.componentInstanceID].componentPaths;
		isis::MultiFormatString  CopyToPartName_temp(in_CopyModelDefinition.toModelName);

		ProMdl     renamedModelHandle;
		Assembly_RenameSubPartOrSubAssembly ( modelInstanceData_temp, CopyToPartName_temp, renamedModelHandle );
		in_out_CADComponentData_map[in_CopyModelDefinition.componentInstanceID].cADModel_hdl = (ProSolid)renamedModelHandle;
		in_out_CADComponentData_map[in_CopyModelDefinition.componentInstanceID].cADModel_ptr_ptr = (ProMdl*)&renamedModelHandle;

		// For testing
		//isis::isis_ProMdlSave(in_out_CADComponentData_map[i->assemblyComponentID].modelHandle);

	}						

}


void ModelOperationsCreo::populateMap_with_Junctions_and_ConstrainedToInfo_per_CADAsmFeatureTrees( 
			//cad::CadFactoryAbstract													&in_Factory,
			const std::vector<std::string>											&in_AssemblyComponentInstanceIDs,
			const std::unordered_map<IntList, std::string, ContainerHash<IntList>>	&in_FeatureIDs_to_ComponentInstanceID_hashtable,
			std::map<std::string, isis::CADComponentData>	&in_out_CADComponentData_map )
																		throw (isis::application_exception)
{
		std::vector<std::string> trimmedListOfComponentIDs;
		FurtherTrimList_Remove_TreatAsOneBodeParts( in_AssemblyComponentInstanceIDs,
													in_out_CADComponentData_map,
													trimmedListOfComponentIDs );

		for each ( const std::string &i in trimmedListOfComponentIDs )
		{

			isis_LOG(lg, isis_FILE, isis_INFO) << "************** " + std::string(__FUNCTION__) + " , ComponentID: " << i;
		
			ProAsmcomppath asmcomppath;

			Retrieve_ProAsmcomppath_WithExceptions( static_cast<ProSolid>(in_out_CADComponentData_map[i].cADModel_hdl),
													in_out_CADComponentData_map[i].componentPaths,
													asmcomppath);

			//isis_ProMdlDisplay( 	in_out_CADComponentData_map[i].modelHandle );

			CreoAssembledFeatureDefinition  assembledFeatureDefinition;

			assembledFeatureDefinition.modelName = in_out_CADComponentData_map[i].name;
			assembledFeatureDefinition.componentInstanceID = i;

			ProElement						ElemTree;


			ProAsmcomp					assembledFeature_temp;			
			//assembledFeature_temp.type =	FeatureGeometryType_enum(in_out_CADComponentData_map[i].assembledFeature.type);
			//assembledFeature_temp.id   =	                         in_out_CADComponentData_map[i].assembledFeature.id;
			//assembledFeature_temp.owner =	                         in_out_CADComponentData_map[i].assembledFeature.owner; 
			assembledFeature_temp = getProAsmcomp( in_out_CADComponentData_map[i].assembledFeature);


			RetrieveCreoElementTreeConstraints(	//in_out_CADComponentData_map[i].assembledFeature,
												assembledFeature_temp,
												asmcomppath,
												assembledFeatureDefinition,
												ElemTree);


			//isis_LOG(lg, isis_FILE, isis_INFO) << assembledFeatureDefinition;
			//std::cout << std::endl << std::endl << assembledFeatureDefinition;

			if ( assembledFeatureDefinition.constraintDefinitions.size() > 0 )
			{		
				int setIndex = 0;
				int numSets =  assembledFeatureDefinition.constraintSetDefinitions.size();

				int setIndex_Start;
				int setIndex_End;    // this is 1 past the end, like STL containers
				if ( numSets == 0 )
				{
					setIndex_Start = -1;  // in the Creo Constraint, -1 means no sets
					setIndex_End = 0;	 
				}
				else
				{
					setIndex_Start = 0;
					setIndex_End = numSets;
				}

		

				for ( int setIndex = setIndex_Start; setIndex < setIndex_End; ++setIndex )
				{
					if ( (PmConnectionAttr)assembledFeatureDefinition.constraintSetDefinitions[setIndex].component_set_misc_attr == PRO_ASM_DISABLE_COMP_SET)
					{
						isis_LOG(lg, isis_FILE, isis_INFO) << "Constraint set disabled.  SetIndex: " << setIndex << "  Component Instance ID: " <<  i;				
						continue;
					}

					isis_LOG(lg, isis_FILE, isis_INFO) << "Adding junction information for the following constraint feature: ";
					ConstraintData constraintData_PerFeatureTree;

					// If a particular constraint was disabled, the following function would not include the junction information for 
					// the disabled constraint in the map.
					PopulateMap_with_JunctionInformation_SingleJunction( 
												//in_Factory,
												assembledFeatureDefinition,
												setIndex,
												constraintData_PerFeatureTree.computedJointData.junction_withoutguide,
												in_out_CADComponentData_map );

					constraintData_PerFeatureTree.computedJointData.jointType_withoutguide =  GetCADJointType(constraintData_PerFeatureTree.computedJointData.junction_withoutguide.joint_pair.first.type);
					constraintData_PerFeatureTree.computedJointData.junctiondDefined_withoutGuide = true;
					constraintData_PerFeatureTree.computedJointData.coordinateSystem_ComponentInstanceID = assembledFeatureDefinition.componentInstanceID;

					try
					{
						// WARNING - Computing the offSetFeatureIDPath_list is temporary fix and will only work for the case
						//			 where a CyPhy leaf assembly has no sub-assemblies.  Later a better approach will
						//           be needed to support a leaf assembly having multiple levels (i.e. multiple sub-assemblies)
						//			 and a sub-assembly has .prts constrained to .prts outside the sub-assembly.
						std::string parentComponentInstanceID = in_out_CADComponentData_map[i].parentComponentID;
						std::list<int>  offSetFeatureIDPath_list = in_out_CADComponentData_map[parentComponentInstanceID].componentPaths;
						RetrieveReferencedComponentInstanceIDs (	offSetFeatureIDPath_list,
																	assembledFeatureDefinition,
																	setIndex,
																	in_FeatureIDs_to_ComponentInstanceID_hashtable,
																	constraintData_PerFeatureTree.constrainedTo_ComponentInstanceIDs_InferredFromLeafAssemblySubordinates);								
					}
					catch ( isis::application_exception &ex)
					{
						std::stringstream errorString;
						errorString <<
							"Function: " << __FUNCTION__ << ", failed to retrieve referenced ComponentInstanceIDs:"  << std::endl <<
							"   Model Name:            " <<	 in_out_CADComponentData_map[i].name << std::endl <<
							"   Model Type:            " <<	 isis::ProMdlType_string(in_out_CADComponentData_map[i].modelType)<<  std::endl <<
							"   Component Instance ID: " <<  i <<  std::endl <<
							"   Exception Message: " << ex.what();
						throw isis::application_exception(errorString);	
					}


					in_out_CADComponentData_map[i].constraintDef.constraints.push_back(constraintData_PerFeatureTree);		
				}
			}
			else  // ELSE if ( assembledFeatureDefinition.constraintDefinitions.size() > 0 )
			{
				//isis_LOG(lg, isis_FILE, isis_INFO) << "\nNOT adding junction information (no constraint references found) for the following constraint feature: ";
				//isis_LOG(lg, isis_FILE, isis_INFO) << assembledFeatureDefinition;

				std::stringstream errorString;
				errorString <<
					"Function: " << __FUNCTION__ << ", No constraint references found for:"  << std::endl <<
					"   Model Name:            " <<	 in_out_CADComponentData_map[i].name << std::endl <<
					"   Model Type:            " <<	 isis::CADMdlType_string(in_out_CADComponentData_map[i].modelType)<<  std::endl <<
					"   Component Instance ID: " << i << std::endl <<
					"   Assembled Feature Definition: " << std::endl <<
					assembledFeatureDefinition;

				throw isis::application_exception(errorString);	
			}

			// Need to set					

			// Only free this after using assembledFeatureDefinition. assembledFeatureDefinition is
			// invalid after ProFeatureElemtreeFree
			//ProFeatureElemtreeFree(&in_out_CADComponentData_map[i].assembledFeature, ElemTree);	
			ProFeatureElemtreeFree(&assembledFeature_temp, ElemTree);	
		}

}


void ModelOperationsCreo::retrieveTranformationMatrix_Assembly_to_Child (  
							const std::string										&in_AssemblyComponentInstanceID,
							const std::string										&in_ChildComponentID,
							const std::map<std::string, isis::CADComponentData>		&in_CADComponentData_map,  
							bool  in_bottom_up,
							double out_TransformationMatrix[4][4] )  throw (isis::application_exception)
{

		
		std::map<std::string, isis::CADComponentData>::const_iterator itr_assembly;
		itr_assembly = in_CADComponentData_map.find(in_AssemblyComponentInstanceID);
		
		if ( itr_assembly == in_CADComponentData_map.end())
		{
			std::stringstream errorString;
			errorString << "Function - " << __FUNCTION__ << ", was passed an in_AssemblyComponentInstanceID that is not in in_CADComponentData_map. in_AssemblyComponentInstanceID:  " << in_AssemblyComponentInstanceID;
			throw isis::application_exception(errorString);	
		}

		std::map<std::string, isis::CADComponentData>::const_iterator itr_child;
		itr_child = in_CADComponentData_map.find(in_ChildComponentID);
		
		if ( itr_child == in_CADComponentData_map.end())
		{
			std::stringstream errorString;
			errorString << "Function - " << __FUNCTION__ << ", was passed an in_ChildComponentID that is not in in_CADComponentData_map. in_ChildComponentID:  " << in_ChildComponentID;
			throw isis::application_exception(errorString);	
		}

		//RetrieveTranformationMatrix_Assembly_to_Child (  
		//					static_cast<ProSolid>(in_CADComponentData_map[in_AssemblyComponentInstanceID].cADModel_hdl),
		//					in_CADComponentData_map[in_ChildComponentID].componentPaths, 
		//					Bool_to_ProBoolean(in_bottom_up),
		//					out_TransformationMatrix ); 

		RetrieveTranformationMatrix_Assembly_to_Child (  
							static_cast<ProSolid>(itr_assembly->second.cADModel_hdl),
							itr_child->second.componentPaths, 
							Bool_to_ProBoolean(in_bottom_up),
							out_TransformationMatrix ); 

}

void ModelOperationsCreo::retrieveTranformationMatrix_Assembly_to_Child (  
							const std::string										&in_AssemblyComponentInstanceID,
							const std::list<int>										&in_ChildComponentPaths,
							const std::map<std::string, isis::CADComponentData>		&in_CADComponentData_map,  
							bool  in_bottom_up,
							double out_TransformationMatrix[4][4] )  throw (isis::application_exception)
{

		std::map<std::string, isis::CADComponentData>::const_iterator itr;
		itr = in_CADComponentData_map.find(in_AssemblyComponentInstanceID);
		
		if ( itr == in_CADComponentData_map.end())
		{
			std::stringstream errorString;
			errorString << "Function - " << __FUNCTION__ << ", was passed an in_AssemblyComponentInstanceID that is not in in_CADComponentData_map. in_AssemblyComponentInstanceID:  " << in_AssemblyComponentInstanceID;
			throw isis::application_exception(errorString);	
		}

		//RetrieveTranformationMatrix_Assembly_to_Child (  
		//					static_cast<ProSolid>(in_CADComponentData_map[in_AssemblyComponentInstanceID].cADModel_hdl),
		//					in_ChildComponentPaths, 
		//					Bool_to_ProBoolean(in_bottom_up),
		//					out_TransformationMatrix ); 


		RetrieveTranformationMatrix_Assembly_to_Child (  
							static_cast<ProSolid>(itr->second.cADModel_hdl),
							in_ChildComponentPaths, 
							Bool_to_ProBoolean(in_bottom_up),
							out_TransformationMatrix ); 

}



void	 ModelOperationsCreo::retrieveBoundingBox( 
								const std::string										&in_ComponentInstanceID,
								const std::map<std::string, isis::CADComponentData>		&in_CADComponentData_map,
								isis_CADCommon::Point_3D									&out_BoundingBox_Point_1,
								isis_CADCommon::Point_3D									&out_BoundingBox_Point_2,
								double													out_Dimensions_xyz[3] )
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

	isis_CADCommon::Point_3D			boundingBox_Point_1_temp;
	isis_CADCommon::Point_3D			boundingBox_Point_2_temp;
	double							dimensions_xyz_temp[3];

	try
	{
		try
		{
			ComputeBoundingBox(		CAD_PRO_SOLID_OUTLINE_COMPUTE,
									static_cast<ProSolid>(itr->second.cADModel_hdl),
									boundingBox_Point_1_temp,
									boundingBox_Point_2_temp,
									dimensions_xyz_temp);
		}
		catch (...)
		{
			isis_LOG(lg, isis_FILE, isis_INFO) << "";
			isis_LOG(lg, isis_FILE, isis_INFO) << "WARNING - Function - " << __FUNCTION__ << ", Component Instance ID: " << in_ComponentInstanceID << ".  Model Name: " << 
			itr->second.name << ".  isis_ProSolidOutlineCompute failed, using isis_ProSolidOutlineGet.  isis_ProSolidOutlineGet is less accurate."; 
			isis_LOG(lg, isis_FILE, isis_INFO) << "";

			ComputeBoundingBox(		CAD_PRO_SOLID_OUTLINE_GET,
									static_cast<ProSolid>(itr->second.cADModel_hdl),
									boundingBox_Point_1_temp,
									boundingBox_Point_2_temp,
									dimensions_xyz_temp);
		} 

		//in_CADComponentData_map[in_ComponentInstanceID].boundingBox.boundingBox_Defined = true;

	
		out_BoundingBox_Point_1 = boundingBox_Point_1_temp;
		out_BoundingBox_Point_2 = boundingBox_Point_2_temp;

		out_Dimensions_xyz[0] = dimensions_xyz_temp[0];
		out_Dimensions_xyz[1] = dimensions_xyz_temp[1];
		out_Dimensions_xyz[2] = dimensions_xyz_temp[2];
	}
	catch ( isis::application_exception& ex )
	{
			
		// Get Mass properties and check for zero volume, this would indicate that there was no solid geometry.  Probably just
		// reference geometry used to define an interface.  For example, an interface between a hatch and hull.
		//ProMassProperty  mass_prop;
		//isis_ProSolidMassPropertyGet_WithDescriptiveErrorMsg(in_ComponentInstanceID, in_CADComponentData_map, &mass_prop );

		isis::cad::CadFactoryAbstract_global *cadFactoryAbstract_global_ptr = isis::cad::CadFactoryAbstract_global::instance();
		isis::cad::CadFactoryAbstract::ptr	cAD_Factory_ptr = cadFactoryAbstract_global_ptr->getCadFactoryAbstract_ptr();
		isis::cad::IModelOperations&         modelOperations = cAD_Factory_ptr->getModelOperations();
		MassProperties		massProperties_temp;

		modelOperations.retrieveMassProperties(in_ComponentInstanceID, in_CADComponentData_map, massProperties_temp);

		//std::cout << std::endl << "-------------> Volume: mass_prop.volume" << mass_prop.volume << std::endl;

		//if ( mass_prop.volume == 0.0 )
		if ( massProperties_temp.volume == 0.0 )
		{
			out_BoundingBox_Point_1.x = 0.0;
			out_BoundingBox_Point_1.y = 0.0;
			out_BoundingBox_Point_1.z = 0.0;
			out_BoundingBox_Point_2.x = 0.0;
			out_BoundingBox_Point_2.y = 0.0;
			out_BoundingBox_Point_2.z = 0.0;

			out_Dimensions_xyz[0] = 0.0;
			out_Dimensions_xyz[1] = 0.0;
			out_Dimensions_xyz[2] = 0.0;
			
			isis_LOG(lg, isis_FILE, isis_INFO) << "";
			isis_LOG(lg, isis_FILE, isis_INFO) << "   Component Instance ID: " << in_ComponentInstanceID << ".  Model Name: " << 
				itr->second.name << ".  Model has zero volume, setting bounding box to size of zero."; 
			isis_LOG(lg, isis_FILE, isis_INFO) << "";
		}
		else
		{
			std::stringstream errorString;
				errorString <<
						 "Function - " << __FUNCTION__ << ", Failed to retrieve bounding box information:"  << std::endl <<
						"   Model Name:            " <<	 itr->second.name << std::endl <<
						"   Model Type:            " << isis::ProMdlType_string(itr->second.modelType)<<  std::endl <<
						"   Component Instance ID: " <<  in_ComponentInstanceID <<  std::endl <<
						"   Exception Message: " << ex.what();
				throw isis::application_exception("C05002",errorString.str().c_str());
		}
	}

}

void ModelOperationsCreo::retrievePointCoordinates(	const std::string							&in_AssemblyComponentInstanceID,
											const std::string									&in_PartComponentID,
											const std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map,
											const MultiFormatString								&in_PointName,
											CADPoint												&out_CADPoint) 
																				throw (isis::application_exception)
{


		RetrieveDatumPointCoordinates(	in_AssemblyComponentInstanceID,
									in_PartComponentID,
									in_CADComponentData_map,
									in_PointName, 
									out_CADPoint); 


}


void ModelOperationsCreo::findPartsReferencedByFeature(	
						const std::string										&in_TopAssemblyComponentID, 
						const std::string										&in_ComponentInstanceID,
						const MultiFormatString									&in_FeatureName,
						e_CADFeatureGeometryType									in_FeatureGeometryType,
						const std::unordered_map<IntList, std::string, ContainerHash<IntList>>		&in_FeatureIDs_to_ComponentInstanceID_hashtable,
						const std::map<std::string, isis::CADComponentData>		&in_CADComponentData_map,
						std::set<std::string>									&out_ComponentInstanceIDs_of_PartsReferencedByFeature_set)
																			throw (isis::application_exception)
{
	FindPartsReferencedByFeature(	
						in_TopAssemblyComponentID, 
						in_ComponentInstanceID,
						in_FeatureName,
						in_FeatureGeometryType,
						in_FeatureIDs_to_ComponentInstanceID_hashtable,
						in_CADComponentData_map,
						out_ComponentInstanceIDs_of_PartsReferencedByFeature_set);

}

void ModelOperationsCreo::retrieveMassProperties( 
						const std::string									&in_ComponentInstanceID,
						const std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map,
						MassProperties										&out_MassProperties) 
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

	out_MassProperties.setValuesToNotDefinedAndZeros();

	// massProperties_RetrievalInvoked means that an attempt was made to read the mass properties from the CAD model.
	// The CAD model may not have mass properties set, and thus for that case massProperties_Defined would be set to false
	// and massProperties_RetrievalInvoked would be set to true because an attempt was made to retrieve the mass properties.
	out_MassProperties.massProperties_RetrievalInvoked = true;


	ProMassProperty  mass_prop;

	isis_ProSolidMassPropertyGet_WithDescriptiveErrorMsg(in_ComponentInstanceID, in_CADComponentData_map, &mass_prop );


	/////////////////////////////////////////
	// Check if Mass Properties are Defined
	////////////////////////////////////////

	// if mass_prop.density == 1.0, then mass properties were never set in Creo.  The never-set mode
	// means that the geometry and density of 1.0 would be used to compute the mass properties; however,
	// those computed values would be erroneous.
	// ERROR - ERROR Leave off the mass_prop.density != 1.0 for now.  This will allow erroneous mass props through, must
	// provide a better check later.
	//if ( mass_prop.volume != 0.0 && mass_prop.density != 0.0 && mass_prop.density != 1.0 && mass_prop.mass != 0.0 ) 
	if ( mass_prop.volume != 0.0 && mass_prop.density != 0.0 && mass_prop.mass != 0.0 ) 
	{
		out_MassProperties.massProperties_Defined = true;
	}
	else
	{
		// out_MassProperties.setValuesToNotDefinedAndZeros would have set out_MassProperties.massProperties_Defined = false;
		return;
	}


	////////////////////////////
	// volume
	////////////////////////////
	out_MassProperties.volume = mass_prop.volume;
	out_MassProperties.volume_Defined = true;

	////////////////////////////
	// density
	////////////////////////////
	out_MassProperties.density = mass_prop.density;
	out_MassProperties.density_Defined = true;

	////////////////////////////
	// mass
	////////////////////////////
	out_MassProperties.mass = mass_prop.mass;
	out_MassProperties.mass_Defined = true;


	////////////////////////////
	// surfaceArea
	////////////////////////////
	if (  mass_prop.surface_area != 0.0 )
	{
		out_MassProperties.surfaceArea	= mass_prop.surface_area;
		out_MassProperties.surfaceArea_Defined	= true;
	}


	////////////////////////////////////////////////////////////////////
	// Center-of-gravity in the coordinate systems of the part/assembly
	////////////////////////////////////////////////////////////////////
	for ( int i = 0 ; i < 3; ++i )	out_MassProperties.centerOfGravity[i] = mass_prop.center_of_gravity[i];
	out_MassProperties.centerOfGravity_Defined = true;

	/////////////////////////////////////////////////////////////////
	// Interia tensor in the coordinate systems of the part/assembly
	/////////////////////////////////////////////////////////////////
	if ( !isis_CADCommon::AllMatrixValuesEqualTarget_3X3(  mass_prop.coor_sys_inertia_tensor, 0.0 )  )
	{
		out_MassProperties.coordSysInertiaTensor_Defined = true;
		isis_CADCommon::SetFromToMatrix_3X3( mass_prop.coor_sys_inertia_tensor, out_MassProperties.coordSysInertiaTensor );

		if ( !isis_CADCommon::Positive_Definite_3_x_3( out_MassProperties.coordSysInertiaTensor ))
		{
			isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO)	<< "\n\nERROR: Non-positive-definite inertia tensor at the default coordinate system." 
												<< "\n       ComponentInstanceID: " << in_ComponentInstanceID
												<< "\n       Model Name:          " << itr->second.name 
												<< "\n       Model Type:          " << isis::ProMdlType_string(itr->second.modelType)
												<< "\n       Note: In the future, this will be treated as a fatal error.  Corrections to the mass properties in the CAD model are required.";
		}
	}


	/////////////////////////////////
	// Interia tensor at the C.G.
	/////////////////////////////////
	if ( !isis_CADCommon::AllMatrixValuesEqualTarget_3X3( mass_prop.cg_inertia_tensor, 0.0 )  )
	{
		out_MassProperties.cGInertiaTensor_Defined = true;
		isis_CADCommon::SetFromToMatrix_3X3( mass_prop.cg_inertia_tensor, out_MassProperties.cGInertiaTensor );

		if ( !isis_CADCommon::Positive_Definite_3_x_3( out_MassProperties.cGInertiaTensor ))
		{
			isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO)	<< "\n\nERROR: Non-positive-definite inertia tensor at the center of gravity." 
												<< "\n       ComponentInstanceID: " << in_ComponentInstanceID
												<< "\n       Model Name:          " << itr->second.name 
												<< "\n       Model Type:          " << isis::ProMdlType_string(itr->second.modelType)
												<< "\n       Note: In the future, this will be treated as a fatal error.  Corrections to the mass properties in the CAD model are required.";
		}

	}

	/////////////////////////////////
	// Principal Moments Of Inertia
	/////////////////////////////////
	for ( int i = 0 ; i < 3; ++i )	out_MassProperties.principalMomentsOfInertia[i] = mass_prop.principal_moments[i];
	out_MassProperties.principalMomentsOfInertia_Defined = true;


	/////////////////////////////////
	// Principal Axis Rotation Matrix
	/////////////////////////////////
	if ( !isis_CADCommon::AllMatrixValuesEqualTarget_3X3(  mass_prop.principal_axes, 0.0 )  )
	{
		out_MassProperties.principalAxis_RotationMatrix_Defined = true;
		isis_CADCommon::SetFromToMatrix_3X3( mass_prop.principal_axes, out_MassProperties.principalAxis_RotationMatrix );
	}


}


void ModelOperationsCreo::retrieveCADModelUnits( 
					const std::string									&in_ComponentInstanceID,
					const std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map,  
					CADModelUnits										&out_CADModelUnits )
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

	//RetrieveUnits_withDescriptiveErrorMsg(	in_ComponentInstanceID,
	//										in_CADComponentData_map[in_ComponentInstanceID],
	//										out_CADModelUnits );

	RetrieveUnits_withDescriptiveErrorMsg(	in_ComponentInstanceID,
											itr->second,
											out_CADModelUnits );
}

MultiFormatString ModelOperationsCreo::retrieveMaterialName( 	
										const std::string									&in_ComponentInstanceID,
										const std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map) 
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


	std::string material_temp;

	//RetrieveMaterial(	in_CADComponentData_map[in_ComponentInstanceID].name,
	//					static_cast<ProSolid>(in_CADComponentData_map[in_ComponentInstanceID].cADModel_hdl), material_temp );

	RetrieveMaterial(	itr->second.name,
						static_cast<ProSolid>(itr->second.cADModel_hdl), material_temp );

	return MultiFormatString(material_temp);
}

bool ModelOperationsCreo::parameterDefinedInCADModel ( const MultiFormatString									&in_ParameterName,
														 const std::string										&in_ComponentInstanceID,	
														 const std::map<std::string, isis::CADComponentData>		&in_CADComponentData_map ) 
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

	try
	{
		//ProName ParameterName;
		//ProStringToWstring(ParameterName, (char *)in_ParameterName.c_str() );

		//std::cout << std::endl << "ParameterName: " << ProWstringToString(temp_string, ParameterName);	
		//std::cout << std::endl << "in_p_model:    " << in_p_model;
		//std::cout << std::endl << "*in_p_model:    " << *in_p_model;

		ProModelitem  ParameterModelItem_struct;
	
		//isis::isis_ProMdlToModelitem ( *in_p_model, &ParameterModelItem_struct );
		//isis::isis_ProMdlToModelitem ( *(itr->second.cADModel_ptr_ptr), &ParameterModelItem_struct );
		isis::isis_ProMdlToModelitem ( itr->second.cADModel_hdl, &ParameterModelItem_struct );
	
		ProParameter  ProParameter_struct;

		//isis::isis_ProParameterInit ( &ParameterModelItem_struct, ParameterName, &ProParameter_struct);
		isis::isis_ProParameterInit ( &ParameterModelItem_struct, in_ParameterName, &ProParameter_struct);
	}
	catch (...)
	{
		return false;
	}
	
	return true;
}

// Warning this function only supports distance units (e.g. mm, inch...) and angle units (deg, radians)
void ModelOperationsCreo::retrieveParameterUnits (	const MultiFormatString								&in_ParameterName,
													const std::string									&in_ComponentInstanceID,	
													const std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map,
													CADModelUnits										&out_CADModelUnits ) 
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

	ProModelitem  ParameterModelItem_struct;
	//isis::isis_ProMdlToModelitem ( *(itr->second.cADModel_ptr_ptr), &ParameterModelItem_struct );
	isis::isis_ProMdlToModelitem ( itr->second.cADModel_hdl, &ParameterModelItem_struct );
	ProParameter  ProParameter_struct;

	isis::isis_ProParameterInit ( &ParameterModelItem_struct, in_ParameterName, &ProParameter_struct);

	out_CADModelUnits.distanceUnit =		CAD_UNITS_DISTANCE_NA; 
	out_CADModelUnits.massUnit =			CAD_UNITS_MASS_NA;
	out_CADModelUnits.forceUnit =		CAD_UNITS_FORCE_NA; 
	out_CADModelUnits.timeUnit =			CAD_UNITS_TIME_NA; 
	out_CADModelUnits.temperatureUnit = CAD_UNITS_TEMPERATURE_NA;	
	out_CADModelUnits.angleUnit       = CAD_UNITS_ANGLE_NA;

	ProParamvalue  ProParamvalue_struct;

	ProUnititem creo_parameter_units;
	isis_ProParameterValueWithUnitsGet(&ProParameter_struct, &ProParamvalue_struct, &creo_parameter_units);

	MultiFormatString unit_multiformat( creo_parameter_units.name);
	if ( wcslen(creo_parameter_units.name) == 0 ) return;  // No units specified for the parameter

	bool identifiedUnitsProperly = false;
	std::string ex_string_1;
	std::string ex_string_2;
	// At this point we don't know if the units are distance or angles.  Try both
	try
	{
		out_CADModelUnits.distanceUnit = CADUnitsDistance_enum( unit_multiformat );
		ComputeUnitNames_Distance(  out_CADModelUnits.distanceUnit , out_CADModelUnits.distanceUnit_ShortName, out_CADModelUnits.distanceUnit_LongName );
		identifiedUnitsProperly = true;
	}
	catch ( isis::application_exception& ex )
	{
		ex_string_1 = ex.what();
	}

	try
	{
		out_CADModelUnits.angleUnit = CADUnitsAngle_enum( unit_multiformat );
		ComputeUnitNames_Angle(  out_CADModelUnits.angleUnit, out_CADModelUnits.angleUnit_ShortName, out_CADModelUnits.angleUnit_LongName );
		identifiedUnitsProperly = true;
	}
	catch (isis::application_exception& ex)
	{
		ex_string_2 = ex.what();
	}

	if ( !identifiedUnitsProperly)
	{
		std::stringstream errorString;
		errorString << "Function - " << __FUNCTION__ << ", in_ComponentInstanceID: " << in_ComponentInstanceID << ", in_ParameterName:  " << in_ParameterName << 
			", Failed to retreive distance or angle units, " << std::endl << ex_string_1 << std::endl << ", "<< ex_string_2;

		throw isis::application_exception(errorString);	
	}

}

// Only supporting distance and angle units
void ModelOperationsCreo::unitConversionFactorsComputation (		const std::string										&in_ComponentInstanceID,	
																const std::map<std::string, isis::CADComponentData>		&in_CADComponentData_map,
																const std::string										&in_FromUnit,
																const std::string										&in_ToUnit,
																double													&out_ScaleFactor,
																double													&out_Offset ) 
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

	out_ScaleFactor = 0.0;
	out_Offset = 0.0;

	if (  in_FromUnit.size() > (PRO_PATH_SIZE - 1 ))
	{
		std::stringstream errorString;
		errorString << "Function - " << __FUNCTION__ << ", was passed an in_FromUnit string that is longer than the allowed size.  Allowed size: " << (PRO_PATH_SIZE - 1 );
		throw isis::application_exception(errorString);	

	}

	if (  in_ToUnit.size() > (PRO_PATH_SIZE - 1 ))
	{
		std::stringstream errorString;
		errorString << "Function - " << __FUNCTION__ << ", was passed an in_ToUnit string that is longer than the allowed size.  Allowed size: " << (PRO_PATH_SIZE - 1 );
		throw isis::application_exception(errorString);	

	}

	MultiFormatString  fromUnit_MultiFromat;
	MultiFormatString  toUnit_MultiFromat;

	bool identifiedUnitsProperly = false;
	std::string ex_string_1;
	std::string ex_string_2;
	// At this point we don't know if the units are distance or angles.  Try both
	try
	{
		// We must assure that the strings are called the correct name (e.g. in, mm) that Creo uses.  This will be assured by calling  CreoUnitsDistance_string/CreoUnitsDistance_enum
		fromUnit_MultiFromat = CreoUnitsDistance_string(CreoUnitsDistance_enum(in_FromUnit));
		toUnit_MultiFromat  =  CreoUnitsDistance_string(CreoUnitsDistance_enum(in_ToUnit));
		identifiedUnitsProperly = true;
	}
	catch ( isis::application_exception& ex )
	{
		ex_string_1 = ex.what();
	}

	if ( !identifiedUnitsProperly)
	{
		try
		{
			// We must assure that the strings are called the correct name (e.g. in, mm) that Creo uses.  This will be assured by calling  CreoUnitsDistance_string/CreoUnitsDistance_enum
			fromUnit_MultiFromat = CreoUnitsAngle_string(CreoUnitsAngle_enum(in_FromUnit));
			toUnit_MultiFromat  =  CreoUnitsAngle_string(CreoUnitsAngle_enum(in_ToUnit));
			identifiedUnitsProperly = true;
		}
		catch (isis::application_exception& ex)
		{
			ex_string_2 = ex.what();
		}
	}

	if ( !identifiedUnitsProperly)
	{
		std::stringstream errorString;
		errorString << "Function - " << __FUNCTION__ << ", in_ComponentInstanceID: " << in_ComponentInstanceID << ", in_FromUnit:  " << in_FromUnit << ", in_ToUnit: " << in_ToUnit << std::endl <<
			", Unknown distance or angle units, " << std::endl << ex_string_1 << std::endl << ", "<< ex_string_2;

		throw isis::application_exception(errorString);	
	}

	ProUnititem fromProUnit;
	ProUnititem toProUnit;

	isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << __FUNCTION__ << ", BEGIN isis_ProUnitCreateByExpression, isis_ProUnitConversionCalculate";

	isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "*(itr->second.cADModel_ptr_ptr):                            " << *(itr->second.cADModel_ptr_ptr);
	isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "const_cast<wchar_t*>((const wchar_t*)fromUnit_MultiFromat): " << const_cast<wchar_t*>((const wchar_t*)fromUnit_MultiFromat);
	isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "const_cast<wchar_t*>((const wchar_t*)toUnit_MultiFromat):   " << const_cast<wchar_t*>((const wchar_t*)toUnit_MultiFromat);
	

	struct UnitCleanup {
		ProUnititem &_proUnit;
		explicit UnitCleanup(ProUnititem &proUnit) : _proUnit(proUnit) { }

		~UnitCleanup()
		{
			isis::isis_ProUnitDelete(&_proUnit);
		}
	};
	isis::isis_ProUnitCreateByExpression(	*(itr->second.cADModel_ptr_ptr), 
											L"customunitfrom", 
											const_cast<wchar_t*>((const wchar_t*)fromUnit_MultiFromat),
											&fromProUnit);
	UnitCleanup _fromUnitCleanup(fromProUnit);


	isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "fromProUnit initialized";

	isis::isis_ProUnitCreateByExpression(	*(itr->second.cADModel_ptr_ptr), 
											L"customunitto", 
											const_cast<wchar_t*>((const wchar_t*)toUnit_MultiFromat),
											&toProUnit);
	UnitCleanup _toUnitCleanup(toProUnit);

	isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "toProUnit initialized";

	ProUnitConversion conversion = {1.0, 0.0};

	isis::isis_ProUnitConversionCalculate(&fromProUnit, &toProUnit, &conversion);

	out_ScaleFactor = conversion.scale;
	out_Offset = conversion.offset;

	isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "out_ScaleFactor: " << out_ScaleFactor;
	isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "out_Offset:      " << out_Offset;

	isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << __FUNCTION__ << ", END isis_ProUnitCreateByExpression, isis_ProUnitConversionCalculate";
}

void ModelOperationsCreo::setParameter (		e_CADParameterType										in_ParameterType,
											const MultiFormatString									&in_ParameterName,
											const std::string										&in_ParameterValue,
											const std::string										&in_ComponentInstanceID,	
											const std::map<std::string, isis::CADComponentData>		&in_CADComponentData_map ) 
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

	
	isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) <<  "   Setting CAD Parameter";
	isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) <<  "      Model Name:       "  <<  itr->second.name;	
	isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) <<  "      Type:             "  <<  CADParameterType_string(in_ParameterType);
	isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) <<  "      Parameter Name:   "  <<  in_ParameterName;
	isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) <<  "      Parameter Value:  "  <<  in_ParameterValue;


	//typedef wchar_t	ProName[PRO_NAME_SIZE];
	if ( in_ParameterName.size() >= PRO_NAME_SIZE )
	{
		std::stringstream errorString;
		errorString << "Function - " << __FUNCTION__ << ", Exceeded maximum number of characters. Parameter Name: "  << std::string(in_ParameterName) + ", Maximum allowed characters: " << PRO_NAME_SIZE;
		throw isis::application_exception(errorString);	
	}

	try
	{
		//ProName ParameterName;
		//ProStringToWstring(ParameterName, (char *)in_ParameterName.c_str() );
		//std::cout << std::endl << "ParameterName: " << ProWstringToString(temp_string, ParameterName);	
		//std::cout << std::endl << "in_p_model:    " << in_p_model;
		//std::cout << std::endl << "*in_p_model:    " << *in_p_model;

		ProModelitem  ParameterModelItem_struct;
	
		//isis::isis_ProMdlToModelitem ( *in_p_model, &ParameterModelItem_struct );
		isis::isis_ProMdlToModelitem ( *itr->second.cADModel_ptr_ptr, &ParameterModelItem_struct );
	
		ProParameter  ProParameter_struct;

		//isis::isis_ProParameterInit ( &ParameterModelItem_struct, ParameterName, &ProParameter_struct);
		isis::isis_ProParameterInit ( &ParameterModelItem_struct, in_ParameterName, &ProParameter_struct);
		ProParamvalue  ProParamvalue_struct;

		//switch ( CADParameterType_enum(in_ParameterType) )
		switch ( in_ParameterType )
		{
			case CAD_FLOAT:
				ProParamvalue_struct.type = PRO_PARAM_DOUBLE;
				ProParamvalue_struct.value.d_val = atof(in_ParameterValue.c_str());

				break;

			case CAD_INTEGER:
				ProParamvalue_struct.type = PRO_PARAM_INTEGER;
				ProParamvalue_struct.value.i_val = atoi(in_ParameterValue.c_str());

				break;
			
			case CAD_BOOLEAN:
				ProParamvalue_struct.type = PRO_PARAM_BOOLEAN;
				ProParamvalue_struct.value.l_val = isis::ProBoolean_enum(in_ParameterValue);
				break;

			case CAD_STRING:

				if ( in_ParameterValue.size() > MAX_STRING_PARAMETER_LENGTH )
				{
					std::stringstream errorString;
					errorString <<
					"Erroneous CADParameter Value, Parameter: " <<  CADParameterType_string(in_ParameterType) <<
					"  Value: " << in_ParameterValue << ", Value must be " << MAX_STRING_PARAMETER_LENGTH << 
					" characters or less.";
					throw isis::application_exception(errorString.str());
				}
				ProParamvalue_struct.type = PRO_PARAM_STRING;
				ProStringToWstring(ProParamvalue_struct.value.s_val,(char*)in_ParameterValue.c_str());
				break;

			default:
				std::stringstream errorString;
				errorString << "Function - " << __FUNCTION__ << ", Erroneous CADParameter Type: " <<  CADParameterType_string(in_ParameterType) << ", Should be FLOAT, INTEGER, or BOOLEAN.";
				throw isis::application_exception(errorString);	
		}

		isis::isis_ProParameterValueSet( &ProParameter_struct, &ProParamvalue_struct );

		//std::cout << std::endl << "   Modified parameter: " <<  in_model_name << "::" <<  in_ParameterName << " --> " << in_ParameterValue;
		//std::clog << std::endl << "   Modified parameter: " <<  in_model_name << "::" <<  in_ParameterName << " --> " << in_ParameterValue;

		// Note - we are not using the units (i.e. k->CADValue().Units().present()).  If we were using units,
		//        then ProUnitConversionGet() would probably be needed to compute the converstion factor.  Also,
		//		  if the parameter units were set then they would be used for computing the scaling factor;
		//		  otherwise, the module units would be used.

	}
	catch ( isis::application_exception& ex )
	{
		ex.setComponentInfo(itr->second.name);
		ex.setParameterInfo(in_ParameterName);
		std::stringstream errorString;
		errorString << "Function - " << __FUNCTION__ << ", exception : Part/Assembly Name: " << itr->second.name <<
								", Parameter Name: " << in_ParameterName << ", " << ex.what();
		throw isis::application_exception(errorString);	

	}



}


void ModelOperationsCreo::addModelsToAssembly( 
					const std::string									&in_AssemblyComponentInstanceID,
					const std::list<std::string>							&in_ModelComponentIDsToAdd,
					std::map<std::string, isis::CADComponentData>		&in_out_CADComponentData_map,
					int													&in_out_AddedToAssemblyOrdinal)
														throw (isis::application_exception) 

{

	Add_Subassemblies_and_Parts( in_out_CADComponentData_map[in_AssemblyComponentInstanceID].cADModel_hdl, 
								 in_out_CADComponentData_map[in_AssemblyComponentInstanceID].name,  
								 in_ModelComponentIDsToAdd, 
								 in_out_CADComponentData_map,
								 in_out_AddedToAssemblyOrdinal);


}
///////////////////////////////////////////////////////////////////////////////////////////////////
bool ModelOperationsCreo::applySingleModelConstraints( 
				const std::string								&in_AssemblyComponentInstanceID,
				const std::string								&in_ComponentIDToBeConstrained,		
				std::map<std::string, isis::CADComponentData>	&in_out_CADComponentData_map )
																			throw (isis::application_exception)
{
	
	bool stop = ApplySingleModelConstraints_Creo(	in_AssemblyComponentInstanceID,
													in_ComponentIDToBeConstrained,
													in_out_CADComponentData_map );	

	return stop;

}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool	 ModelOperationsCreo::dataExchangeFormatSupported(const DataExchangeSpecification &in_DataExchangeSpecification)
{
	bool formatSupported = false;

	switch ( in_DataExchangeSpecification.dataExchangeFormat )
	{
		case DATA_EXCHANGE_FORMAT_STEP:
			switch(in_DataExchangeSpecification.dataExchangeVersion)
			{
				case AP203_SINGLE_FILE:
				case AP203_E2_SINGLE_FILE:
				case AP203_E2_SEPARATE_PART_FILES:
				case AP214_SINGLE_FILE:
				case AP214_SEPARATE_PART_FILES:
					formatSupported = true;
					break;	
				default:
					break;
			}
			break;
		case DATA_EXCHANGE_FORMAT_STEREOLITHOGRAPHY:
			switch(in_DataExchangeSpecification.dataExchangeVersion)
			{
				case ASCII:
				case BINARY:
					formatSupported = true;
					break;	
				default:
					break;
			}
			break;
		case DATA_EXCHANGE_FORMAT_INVENTOR:
			formatSupported = true;
			break;
		case DATA_EXCHANGE_FORMAT_PARASOLID:
			formatSupported = true;
			break;
		case DATA_EXCHANGE_DXF:
			switch(in_DataExchangeSpecification.dataExchangeVersion)
			{
				case Y2013:
					formatSupported = true;
  					break;	
				default:
					break;
			}
			break;
		default:
			break;
	}
	return 	formatSupported;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void ModelOperationsCreo::exportDataExchangeFile_STEP(	void 								*in_ModelHandle_ptr,
														e_CADMdlType							in_ModelType,
														const DataExchangeSpecification		&in_DataExchangeSpecification,
														const MultiFormatString				&in_OutputDirectoryPath,		
														const MultiFormatString				&in_OutputFileName )         
																			throw (isis::application_exception)
{

	isis::cad::CadFactoryAbstract_global *cadFactoryAbstract_global_ptr = isis::cad::CadFactoryAbstract_global::instance();
	isis::cad::CadFactoryAbstract::ptr	cAD_Factory_ptr = cadFactoryAbstract_global_ptr->getCadFactoryAbstract_ptr();

	isis::cad::ICADSession&            cADsession = cAD_Factory_ptr->getCADSession();	


	cADsession.setCADWorkingDirectory(in_OutputDirectoryPath);

	ProName		optionName;
	ProPath		optionValue;

	wcscpy( optionName, L"intf3d_out_datums_by_default");
	wcscpy( optionValue, L"yes");
	isis_ProConfigoptSet(optionName, optionValue  );

	wcscpy( optionName, L"intf3d_out_parameters");
	wcscpy( optionValue, L"all");
	isis_ProConfigoptSet(optionName, optionValue  );

	wcscpy( optionName, L"intf_out_blanked_entities");
	wcscpy( optionValue, L"no");
	isis_ProConfigoptSet(optionName, optionValue  );

	switch(in_DataExchangeSpecification.dataExchangeVersion)
	{
		case AP203_SINGLE_FILE:
			wcscpy( optionName, L"step_export_format");
			wcscpy( optionValue, L"ap203_is");			
			isis_ProConfigoptSet(optionName, optionValue  );
			break;
		case AP203_E2_SINGLE_FILE:
			wcscpy( optionName, L"step_export_format");
			wcscpy( optionValue, L"ap203_e2");			
			isis_ProConfigoptSet(optionName, optionValue  );

			// Note - there is not a step_export_ap203_e2_asm_def_mode config function.
			//		  Creo honors the 214 setting for 203_e2
			wcscpy( optionName,  L"step_export_ap214_asm_def_mode");
			wcscpy( optionValue, L"single_file");
			isis_ProConfigoptSet(optionName, optionValue  );

			break;
		case AP203_E2_SEPARATE_PART_FILES:
			wcscpy( optionName, L"step_export_format");
			wcscpy( optionValue, L"ap203_e2");			
			isis_ProConfigoptSet(optionName, optionValue  );

			// Note - there is not a step_export_ap203_e2_asm_def_mode config function.
			//		  Creo honors the 214 setting for 203_e2
			wcscpy( optionName,  L"step_export_ap214_asm_def_mode");
			wcscpy( optionValue, L"separate_parts_only");
			isis_ProConfigoptSet(optionName, optionValue  );

			break;
		case AP214_SINGLE_FILE:
			wcscpy( optionName, L"step_export_format");
			wcscpy( optionValue, L"ap214_is");
			isis_ProConfigoptSet(optionName, optionValue  );

			wcscpy( optionName,  L"step_export_ap214_asm_def_mode");
			wcscpy( optionValue, L"single_file");
			isis_ProConfigoptSet(optionName, optionValue  );

			break;
		case AP214_SEPARATE_PART_FILES:
			wcscpy( optionName, L"step_export_format");
			wcscpy( optionValue, L"ap214_is");
			isis_ProConfigoptSet(optionName, optionValue  );

			wcscpy( optionName,  L"step_export_ap214_asm_def_mode");
			wcscpy( optionValue, L"separate_parts_only");
			isis_ProConfigoptSet(optionName, optionValue  );

			break;

		default:

			std::stringstream errorString;
			errorString << "Function - " << __FUNCTION__ << ", Was passed an erroneous dataExchangeVersion enum.  " << std::endl <<
						 "\nAllowed enum values are AP203_SINGLE_FILE, AP203_E2_SINGLE_FILE, AP203_E2_SEPARATE_PART_FILES, AP214_SINGLE_FILE and AP214_SEPARATE_PART_FILES.";
			throw isis::application_exception(errorString);	
	}


	// Write Data Exchange file (e.g. STEP File)
	int Arg_2 = 1;


	//MultiFormatString StepFileName_multiString(StepFileName_string, PRO_FILE_NAME_SIZE - 1 );

		

		//isis_ProOutputFileWrite(	p_Model,    // This function was deprecated as of Creo 3.0
		isis_ProOutputFileMdlnameWrite(	in_ModelHandle_ptr,
								in_OutputFileName,
								PRO_STEP_FILE,
								NULL,
								&Arg_2,
								NULL,
								NULL );


}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ModelOperationsCreo::exportDataExchangeFile_Stereolithography(	void 					*in_ModelHandle_ptr,
															e_CADMdlType						in_ModelType,
															const DataExchangeSpecification	&in_DataExchangeSpecification,
															const MultiFormatString			&in_OutputDirectoryPath,		// Only the path to the directory
															const MultiFormatString			&in_OutputFileName)		    // This the complete file name (e.g. bracket_asm.stp)
																							throw (isis::application_exception)
{

	isis::cad::CadFactoryAbstract_global		*cadFactoryAbstract_global_ptr = isis::cad::CadFactoryAbstract_global::instance();
	isis::cad::CadFactoryAbstract::ptr		cAD_Factory_ptr = cadFactoryAbstract_global_ptr->getCadFactoryAbstract_ptr();
	isis::cad::ICADSession&					cADsession = cAD_Factory_ptr->getCADSession();	

	cADsession.setCADWorkingDirectory(in_OutputDirectoryPath);


	// Write Data Exchange file (e.g. Stereolithography File)
	int Arg_2 = 1;


	ProImportExportFile ExportFileType;

	
	ProName		optionName;
	ProPath		optionValue;

	wcscpy( optionName, L"intf3d_out_prop_chord_heights");
	wcscpy( optionValue, L"yes");
	isis_ProConfigoptSet(optionName, optionValue  );

	if ( in_DataExchangeSpecification.dataExchangeVersion == ASCII )
			ExportFileType = PRO_SLA_ASCII_FILE;
		else
			ExportFileType = PRO_SLA_BINARY_FILE;
	

	//isis_ProOutputFileWrite(	p_Model,   // This function was deprecated as of Creo 3.0
	isis_ProOutputFileMdlnameWrite(	in_ModelHandle_ptr,
							in_OutputFileName,
							ExportFileType,
							NULL,
							&Arg_2,
							NULL,
							NULL );


}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ModelOperationsCreo::exportDataExchangeFile_Inventor(	void 							*in_ModelHandle_ptr,
															e_CADMdlType						in_ModelType,
															const DataExchangeSpecification	&in_DataExchangeSpecification,
															const MultiFormatString			&in_OutputDirectoryPath,		// Only the path to the directory
															const MultiFormatString			&in_OutputFileName)		    // This the complete file name (e.g. bracket_asm.stp)
																							throw (isis::application_exception)
{

	isis::cad::CadFactoryAbstract_global		*cadFactoryAbstract_global_ptr = isis::cad::CadFactoryAbstract_global::instance();
	isis::cad::CadFactoryAbstract::ptr		cAD_Factory_ptr = cadFactoryAbstract_global_ptr->getCadFactoryAbstract_ptr();
	isis::cad::ICADSession&					cADsession = cAD_Factory_ptr->getCADSession();	

	cADsession.setCADWorkingDirectory(in_OutputDirectoryPath);

	// Write Data Exchange file (e.g. Stereolithography File)
	int Arg_2 = 1;


	ProImportExportFile ExportFileType;

	ExportFileType = PRO_INVENTOR_FILE;
	
	//isis_ProOutputFileWrite(	p_Model,   // This function was deprecated as of Creo 3.0
	isis_ProOutputFileMdlnameWrite(	in_ModelHandle_ptr,
							in_OutputFileName,
							ExportFileType,
							NULL,
							&Arg_2,
							NULL,
							NULL );

}



///////////////////////////////////////////////////////////////////////////////////////////////////

void ModelOperationsCreo::exportDataExchangeFile_DXF(		void 							*in_ModelHandle_ptr,
															e_CADMdlType						in_ModelType,
															const DataExchangeSpecification	&in_DataExchangeSpecification,
															const MultiFormatString			&in_OutputDirectoryPath,		// Only the path to the directory
															const MultiFormatString			&in_OutputFileName)		    // This the complete file name (e.g. bracket_asm.stp)
																							throw (isis::application_exception)
{

	isis::cad::CadFactoryAbstract_global		*cadFactoryAbstract_global_ptr = isis::cad::CadFactoryAbstract_global::instance();
	isis::cad::CadFactoryAbstract::ptr		cAD_Factory_ptr = cadFactoryAbstract_global_ptr->getCadFactoryAbstract_ptr();
	isis::cad::ICADSession&					cADsession = cAD_Factory_ptr->getCADSession();	


	cADsession.setCADWorkingDirectory(in_OutputDirectoryPath);


	ProIntf3DExportType ExportFileType = PRO_INTF_EXPORT_DXF;

	ProName		optionName;
	ProPath		optionValue;
	wcscpy( optionName, L"intf3d_out_prop_chord_heights");
	wcscpy( optionValue, L"yes");

	wcscpy( optionName, L"dxf_export_format");
	wcscpy( optionValue, L"2013");

	ProBoolean is_supported;

	pro_output_assembly_configuration assembly_configuration  = PRO_OUTPUT_ASSEMBLY_FLAT_FILE;
	isis_ProOutputAssemblyConfigurationIsSupported( ExportFileType,
           											assembly_configuration,
													&is_supported);
													
	if ( is_supported == PRO_B_TRUE )
	{
		//ProOutputBrepRepresentation representation;
		//isis_ProOutputBrepRepresentationAlloc( &representation);
														

		//isis_ProOutputBrepRepresentationFlagsSet(	representation,
		//											PRO_B_FALSE,   //as_wireframe
		//											PRO_B_FALSE,    // as_surfaces
		//											PRO_B_FALSE,   // as_solid
		//											PRO_B_TRUE);   // as_quilts

		//ProBoolean brepRepresentationFlags_supported;
		//isis_ProOutputBrepRepresentationIsSupported( ExportFileType, representation, &brepRepresentationFlags_supported);

		//if ( brepRepresentationFlags_supported == PRO_B_FALSE )
		//{
		//	stringstream errorMsg_temp;
		//	errorMsg_temp << "Functions " << __FUNCTION__ << ", ProOutputBrepRepresentationIsSupported flags not supported: " <<
		//		" as_wireframe == false, surfaces == false, solid = false, quilts = true";
		//	
		//	throw isis::application_exception(errorMsg_temp);
		// }

		//ProOutputInclusion inclusion;
		//isis_ProOutputInclusionAlloc( &inclusion);

		//isis_ProOutputInclusionFlagsSet( inclusion,
		//								  PRO_B_FALSE, //		 ProBoolean include_datums,
		//								  PRO_B_FALSE, //		 ProBoolean include_blanked,
		//								  PRO_B_TRUE); //		 ProBoolean include_facetted)

		// Suface models are not being exported via DXF. The resulting DXF file is empty.
		// Setting inclusion and/or representation does not help.

		isis::isis_ProIntf3DFileWrite( (ProSolid)in_ModelHandle_ptr, 
										ExportFileType, 
										(wchar_t*)(const wchar_t*)in_OutputFileName, 
										assembly_configuration,
										NULL, NULL, NULL, NULL);
										//NULL, NULL, inclusion, NULL);
										//NULL, representation, NULL, NULL);

		//isis_ProOutputBrepRepresentationFree( representation);
			//isis_ProOutputInclusionFree (inclusion);
	}
	else
	{
		stringstream errorMsg_temp;
		errorMsg_temp << "Failed to create DXF file." <<
			"\n   The Creo function ProOutputAssemblyConfigurationIsSupported indicated that the DXF format is not supported for the assembly." <<
			"\n   To reproduce the problem, with Creo, open the assembly and try to save it to the DXF format.";
		ofstream DXF_Failed;
		DXF_Failed.open ((string)in_OutputDirectoryPath + "\\_FAILED_Data_Exchange.txt", std::ofstream::app);
		DXF_Failed << errorMsg_temp.str();
		DXF_Failed.close();


		isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) << errorMsg_temp;

		// Don't throw an exception.  The failed message will be in the output directory.
		//throw isis::application_exception(errorMsg_temp);
	}


}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ModelOperationsCreo::exportDataExchangeFile_Parasolid(	void 							*in_ModelHandle_ptr,
															e_CADMdlType						in_ModelType,
															const DataExchangeSpecification	&in_DataExchangeSpecification,
															const MultiFormatString			&in_OutputDirectoryPath,		// Only the path to the directory
															const MultiFormatString			&in_OutputFileName)		    // This the complete file name (e.g. bracket_asm.stp)
																							throw (isis::application_exception)
{

	isis::cad::CadFactoryAbstract_global *cadFactoryAbstract_global_ptr = isis::cad::CadFactoryAbstract_global::instance();
	isis::cad::CadFactoryAbstract::ptr	cAD_Factory_ptr = cadFactoryAbstract_global_ptr->getCadFactoryAbstract_ptr();
									
	isis::cad::IModelHandling&        modelHandling = cAD_Factory_ptr->getModelHandling();
	isis::cad::ICADSession&           cADsession = cAD_Factory_ptr->getCADSession();


	cADsession.setCADWorkingDirectory(in_OutputDirectoryPath);


	pro_intf3d_export_type ExportFileType = PRO_INTF_EXPORT_PARASOLID;

	isis::isis_ProIntf3DFileWrite( (ProSolid)in_ModelHandle_ptr, 
									ExportFileType, 
									(wchar_t*)(const wchar_t*)in_OutputFileName, 
									PRO_OUTPUT_ASSEMBLY_FLAT_FILE,
									NULL, NULL, NULL, NULL);


}
///////////////////////////////////////////////////////////////////////////////////////////////////
void ModelOperationsCreo::computePartInterferences( const std::string									&in_AssemblyComponentInstanceID,  // This must be an assembly
													const std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map,
													std::vector<PartInterferences>						&out_PartInterferences )
																							throw (isis::application_exception)
{

	std::map<std::string, isis::CADComponentData>::const_iterator itr;
	itr = in_CADComponentData_map.find(in_AssemblyComponentInstanceID);
		
	if ( itr == in_CADComponentData_map.end())
	{
		std::stringstream errorString;
		errorString << "Function - " << __FUNCTION__ << ", was passed an in_ComponentInstanceID that is not in in_CADComponentData_map. in_AssemblyComponentInstanceID:  " << in_AssemblyComponentInstanceID;
		throw isis::application_exception(errorString);	
	}


	isis::cad::CadFactoryAbstract_global *cadFactoryAbstract_global_ptr = isis::cad::CadFactoryAbstract_global::instance();
	isis::cad::CadFactoryAbstract::ptr	cAD_Factory_ptr = cadFactoryAbstract_global_ptr->getCadFactoryAbstract_ptr();
	isis::cad::IModelOperations&         modelOperations = cAD_Factory_ptr->getModelOperations();

	out_PartInterferences.clear();

	if ( itr->second.modelType != CAD_MDL_ASSEMBLY )
	{
		std::stringstream errorString;
		errorString <<	"Function - " << __FUNCTION__  << std::endl <<
						"in_AssemblyComponentInstanceID.modelType != CAD_MDL_ASSEMBLY, interferences can only be computed on assemblies." << std::endl <<
						"   Model Name: " <<  itr->second.name		<< std::endl <<
						"   Model Type: " <<  itr->second.modelType;
		
		throw isis::application_exception(errorString.str());
	}


	// This code is based on UgGeomInterferCheck.c (located in the PTC installed examples)
	ProInterferenceInfo   *interf_info_arr = NULL;

	try 
	{
		 
		ProAssembly assembly = static_cast<ProAssembly>( itr->second.cADModel_hdl);

		isis_ProFitGlobalinterferenceCompute(	assembly,
												//PRO_FIT_SUB_ASSEMBLY,  Not sure
												PRO_FIT_SUB_ASSEMBLY_DETAILED,
												PRO_B_FALSE,   // The options to include facets.
												PRO_B_FALSE,   // The options to include quilts
												PRO_B_FALSE,   // fast_calculation
												&interf_info_arr);

		int   n_intf_parts;
		isis_ProArraySizeGet(interf_info_arr, &n_intf_parts);
			
		for ( int i = 0; i < n_intf_parts; ++i )
		{
			double          volume;
			isis_ProFitInterferencevolumeCompute(interf_info_arr[i].interf_data, &volume);

			//ProAsmcomppath  cmp_path_1, cmp_path_2;
			//isis_ProSelectionAsmcomppathGet(interf_info_arr[i].part_1, &cmp_path_1);
			//isis_ProSelectionAsmcomppathGet(interf_info_arr[i].part_2, &cmp_path_2);

			ProModelitem modelitem_1;
			ProModelitem modelitem_2;
			isis_ProSelectionModelitemGet(interf_info_arr[i].part_1, &modelitem_1);
			isis_ProSelectionModelitemGet(interf_info_arr[i].part_2, &modelitem_2);

			ProMdldata           info_1;
			ProMdldata           info_2;

			isis_ProMdlDataGet (modelitem_1.owner, &info_1);
			isis_ProMdlDataGet (modelitem_2.owner, &info_2);

			char name_1[80];
			char name_2[80];
			
			PartInterferences partInterferences_temp;

			partInterferences_temp.modelName_1 = info_1.name;
			partInterferences_temp.modelName_2 = info_2.name;
			partInterferences_temp.volume = volume;

			CADModelUnits	cADModelUnits_temp; 

			modelOperations.retrieveCADModelUnits(
							in_AssemblyComponentInstanceID,
							in_CADComponentData_map,
							cADModelUnits_temp );

			partInterferences_temp.volumeUnits = convertDistanceUnitToVolumeUnit(cADModelUnits_temp.distanceUnit);

			//interferenceReport_file << std::endl << std::left << std::setw(32) << ProWstringToString (name_1, info_1.name) <<   std::left <<  std::setw(32) << ProWstringToString (name_2, info_2.name) <<  volume;
			out_PartInterferences.push_back(partInterferences_temp);
		}

		// Free interf_info_arr
		isis_ProInterferenceInfoProarrayFree(interf_info_arr);
		
	}

	catch ( ... )
	{
		isis_ProInterferenceInfoProarrayFree(interf_info_arr);
		throw;
	}

}
///////////////////////////////////////////////////////////////////////////////////////////////////
/*
void ModelOperationsCreo::computeVehicleGroundPlane( const std::string								&in_AssemblyComponentID,
													 std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map,
													std::vector<isis_CADCommon::Point_3D>			&out_GroundPlanePoints )
																			throw (isis::application_exception)
{

		/ *
		Pro3dPnt  r_outline_points[2];
		isis::isis_ProSolidOutlineGet( in_CADComponentData_map[in_AssemblyComponentID].modelHandle, r_outline_points);
		
			
		// Need the smallest Y coordinate
		double minimum_y;

		if ( r_outline_points[0][1] < r_outline_points[1][1] )
			minimum_y = r_outline_points[0][1];
		else
			minimum_y = r_outline_points[1][1];

		isis_CADCommon::Point_3D  point;
		point.x = 0;
		point.y = minimum_y;  
		point.z = 0;
		out_GroundPlanePoints.push_back( point);
		
		point.z = 1;
		out_GroundPlanePoints.push_back( point);
		point.x = 1;
		point.z = 0;
		out_GroundPlanePoints.push_back( point);

		* /
		//Pro3dPnt  r_outline_points[2];
		//isis::isis_ProSolidOutlineGet( in_CADComponentData_map[in_AssemblyComponentID].modelHandle, r_outline_points);
		

		//isis::cad::CadFactoryAbstract_global *cadFactoryAbstract_global_ptr = isis::cad::CadFactoryAbstract_global::instance();
		//isis::cad::CadFactoryAbstract::ptr	cAD_Factory_ptr = cadFactoryAbstract_global_ptr->getCadFactoryAbstract_ptr();

		isis_CADCommon::Point_3D	boundingBox_Point_1;
		isis_CADCommon::Point_3D	boundingBox_Point_2;
		double						boundingBoxDimensions_xyz[3];

		RetrieveBoundingBox_ComputeFirstIfNotAlreadyComputed(	in_AssemblyComponentID,
																in_CADComponentData_map,
																boundingBox_Point_1,
																boundingBox_Point_2,
																boundingBoxDimensions_xyz );


		
		// Need the smallest Y coordinate
		double minimum_y;

		if ( boundingBox_Point_1.y < boundingBox_Point_2.y )
			minimum_y =  boundingBox_Point_1.y;
		else
			minimum_y =  boundingBox_Point_2.y;

		isis_CADCommon::Point_3D  point;
		point.x = 0;
		point.y = minimum_y;  
		point.z = 0;
		out_GroundPlanePoints.push_back( point);
		
		point.z = 1;
		out_GroundPlanePoints.push_back( point);
		point.x = 1;
		point.z = 0;
		out_GroundPlanePoints.push_back( point);

}
*/

} // creo
} // cad
} // isis
