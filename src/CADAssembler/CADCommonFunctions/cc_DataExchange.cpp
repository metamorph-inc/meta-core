#include "cc_DataExchange.h"
#include "cc_CADFactoryAbstract.h"
#include "cc_CommonUtilities.h"
#include "cc_CommonFunctions.h"

namespace isis 
{

///////////////////////////////////////////////////////////////////////////////////////////////////
void	 GetDataExchangeOutputDirectoryName(	const	DataExchangeSpecification		&in_DataExchangeSpecification,
										MultiFormatString						&out_DataExchangeDirectoryName // This is not a path, just the final directory name (e.g. PARASOLID)
										)  
{

	switch ( in_DataExchangeSpecification.dataExchangeFormat )
	{
		case DATA_EXCHANGE_FORMAT_STEP:
			out_DataExchangeDirectoryName = isis::DataExchangeVersion_string(in_DataExchangeSpecification.dataExchangeVersion);
			break;
		case DATA_EXCHANGE_FORMAT_STEREOLITHOGRAPHY:
			out_DataExchangeDirectoryName = "STL_" + isis::DataExchangeVersion_string(in_DataExchangeSpecification.dataExchangeVersion);
			break;
		case DATA_EXCHANGE_FORMAT_INVENTOR:
			out_DataExchangeDirectoryName = "INVENTOR";
			break;
		case DATA_EXCHANGE_FORMAT_PARASOLID:
			out_DataExchangeDirectoryName = "PARASOLID";				
			break;
		case DATA_EXCHANGE_DXF:
			out_DataExchangeDirectoryName = "DXF_" + isis::DataExchangeVersion_string(in_DataExchangeSpecification.dataExchangeVersion);
			break;
		default:
			// Force this to be updated if new format is added.
			std::stringstream errorString;
			// WARNING - Do not call DataExchangeFormat_string in the following message. This could result in another exception being thrown.
			errorString << "Function - " << __FUNCTION__ << ", was passed: " << in_DataExchangeSpecification.dataExchangeFormat  <<
				", which is an erroneous value. Allowed values are: " <<
				"DATA_EXCHANGE_FORMAT_STEP   DATA_EXCHANGE_FORMAT_STEREOLITHOGRAPHY   DATA_EXCHANGE_FORMAT_INVENTOR   DATA_EXCHANGE_FORMAT_PARASOLID   DATA_EXCHANGE_DXF";
			throw isis::application_exception(errorString);
			break;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void	 GetDataExchangeOutputFileName(	const	DataExchangeSpecification		&in_DataExchangeSpecification,
									const	MultiFormatString				&in_CADModelName_NoSuffix,
									e_CADMdlType								in_ModelType,
									MultiFormatString						&out_DataExchangeFileName_WithSuffix )  
{

	std::string suffix;

	switch ( in_DataExchangeSpecification.dataExchangeFormat )
	{
		case DATA_EXCHANGE_FORMAT_STEP:
			suffix = "stp";
			break;
		case DATA_EXCHANGE_FORMAT_STEREOLITHOGRAPHY:
			suffix = "stl";	
			break;
		case DATA_EXCHANGE_FORMAT_INVENTOR:
			suffix = "iv";	
			break;
		case DATA_EXCHANGE_FORMAT_PARASOLID:
			suffix = "x_t";				
			break;
		case DATA_EXCHANGE_DXF:
			suffix = "dxf";
			break;
		default:
			// Force this to be updated if new format is added.
			std::stringstream errorString;
			// WARNING - Do not call DataExchangeFormat_string in the following message. This could result in another exception being thrown.
			errorString << "Function - " << __FUNCTION__ << ", was passed: " << in_DataExchangeSpecification.dataExchangeFormat  <<
				", which is an erroneous value. Allowed values are: " <<
				"DATA_EXCHANGE_FORMAT_STEP   DATA_EXCHANGE_FORMAT_STEREOLITHOGRAPHY   DATA_EXCHANGE_FORMAT_INVENTOR   DATA_EXCHANGE_FORMAT_PARASOLID   DATA_EXCHANGE_DXF";
			throw isis::application_exception(errorString);
			break;
	}

	switch (in_ModelType)
	{
		case CAD_MDL_PART:
			out_DataExchangeFileName_WithSuffix = (std::string)in_CADModelName_NoSuffix + "_prt." + suffix;
			break;
		case CAD_MDL_ASSEMBLY:
			out_DataExchangeFileName_WithSuffix = (std::string)in_CADModelName_NoSuffix + "_asm." + suffix;
			break;
		default:
			out_DataExchangeFileName_WithSuffix = (std::string)in_CADModelName_NoSuffix + "." + suffix;
	}

}

///////////////////////////////////////////////////////////////////////////////////////////////////
void	 CreateOutputDir_GetOutputDirFullPath_GetOutputFileName(
									const std::string					&in_WORKING_DIR,
									const DataExchangeSpecification		&in_DataExchangeSpecification,
									const MultiFormatString				&in_CADModelName_NoSuffix,
									e_CADMdlType							in_ModelType,
									MultiFormatString					&out_OutputDirFullPath,
									MultiFormatString					&out_DataExchangeFileName_WithSuffix )  
																				throw (isis::application_exception)
{

		//////////////////////////
		// Create the directory
		//////////////////////////
		MultiFormatString outDirName_multi;
		GetDataExchangeOutputDirectoryName(	in_DataExchangeSpecification, outDirName_multi);
		std::string outDirName_string = outDirName_multi;
		if ( !boost::filesystem::exists(outDirName_string)) boost::filesystem::create_directory(outDirName_string);

		/////////////////////////////////////////////
		// Set the Full Path to the Output Directory
		/////////////////////////////////////////////
		std::string outPathAndDir_string = in_WORKING_DIR + "\\" + outDirName_string;
		if ( outPathAndDir_string.size() > (CAD_PATH_SIZE - 1) )
		{
			std::stringstream errorString;
			errorString << "Function - " << __FUNCTION__ << "outPathAndDir_string exceeds length of " << (CAD_PATH_SIZE - 1) << 
						   " characters. outPathAndDir_string: " << outPathAndDir_string;
			throw isis::application_exception(errorString);

		}
		out_OutputDirFullPath = outPathAndDir_string;

		/////////////////////////
		// Get output File Name
		////////////////////////
		MultiFormatString	outputFileName_multi;
		GetDataExchangeOutputFileName(	in_DataExchangeSpecification, in_CADModelName_NoSuffix, in_ModelType, out_DataExchangeFileName_WithSuffix );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ExportDataExchangeFiles_driver( 
					const std::string									&in_ComponentInstanceID,   // This is just for logging
					const MultiFormatString								&in_ModelName, 
					e_CADMdlType											in_ModelType,
					const MultiFormatString								&in_GeometryRepresentation,
					const std::string									&in_WORKING_DIR,
					const std::list<DataExchangeSpecification>          &in_DataExchangeSpecifications,
					bool													in_LogProgress )
																	throw (isis::application_exception)
{

	isis::cad::CadFactoryAbstract_global *cadFactoryAbstract_global_ptr = isis::cad::CadFactoryAbstract_global::instance();
	isis::cad::CadFactoryAbstract::ptr	cAD_Factory_ptr = cadFactoryAbstract_global_ptr->getCadFactoryAbstract_ptr();

	isis::cad::IModelHandling&				modelHandling = cAD_Factory_ptr->getModelHandling();
	isis::cad::ICADSession&					cADsession = cAD_Factory_ptr->getCADSession();	
	isis::cad::IModelOperations&			modelOperations = cAD_Factory_ptr->getModelOperations();


	void     *p_Model;

	//////////////////////
	// Retrieve Model
	//////////////////////
	modelHandling.cADModelRetrieve(	in_ComponentInstanceID,
									in_ModelName,
									in_ModelType,
									in_GeometryRepresentation,
									&p_Model);

	// std::cout << std::endl << " ******  ExportDataExchangeFiles_STEP, in_ModelName: " << in_ModelName;

	for each ( DataExchangeSpecification i in in_DataExchangeSpecifications )
	{




		MultiFormatString   outputPathAndDir_multi;
		MultiFormatString	outputFileName_multi;
		CreateOutputDir_GetOutputDirFullPath_GetOutputFileName( in_WORKING_DIR, 
																i, 
																in_ModelName, 
																in_ModelType, 
																outputPathAndDir_multi, 
																outputFileName_multi);


		if ( !modelOperations.dataExchangeFormatSupported(i) )
		{
			// Note -	We can use function DataExchangeFormat_string and DataExchangeVersion_string because CreateOutputDir_GetOutputDirFullPath_GetOutputFileName
			//			would have thrown an exception if i.dataExchangeFormat or i.dataExchangeVersion were invalid.

			std::stringstream errorMsg_temp;
			errorMsg_temp << "Failed to create DataExchange file." <<  std::endl <<
				"   Data Exchange Format not supported for the particular CAD system." <<  std::endl <<
				"       DataExchangeFormat: " << DataExchangeFormat_string(i.dataExchangeFormat) << "  DataExchangeVersion: " << DataExchangeVersion_string(i.dataExchangeVersion) << std::endl <<
				"       Model: " << in_ModelName << std::endl <<
				"   modelOperations.dataExchangeFormatSupported() function returned that the format is not supported.";


			// Must have .str() on errorMsg_temp.str(); otherwise only the address of errorMsg_temp will be logged.
			isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) << errorMsg_temp.str();

			std::ofstream DataExchangeNotSupported_file;
			DataExchangeNotSupported_file.open ((std::string)outputPathAndDir_multi + "\\_FAILED_Data_Exchange.txt", std::ofstream::app);
			DataExchangeNotSupported_file << errorMsg_temp.str() << std::endl;
			DataExchangeNotSupported_file.close();
			continue;
		}


		switch ( i.dataExchangeFormat )
		{
			case DATA_EXCHANGE_FORMAT_STEP:

				modelOperations.exportDataExchangeFile_STEP(				p_Model,
																			in_ModelType,
																			i,
																			outputPathAndDir_multi,
																			outputFileName_multi );
				if ( in_LogProgress ) isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "   Created STEP file(s) for: "  << in_ModelName;
				
				break;

			case DATA_EXCHANGE_FORMAT_STEREOLITHOGRAPHY:
				modelOperations.exportDataExchangeFile_Stereolithography(	p_Model,
																			in_ModelType,
																			i,
																			outputPathAndDir_multi,			// Only the path to the directory
																			outputFileName_multi);	// This the complete file name (e.g. bracket_asm.stp)	

				if ( in_LogProgress ) isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "   Created Stereolithograph file(s) for "  << in_ModelName;
				break;

			case DATA_EXCHANGE_FORMAT_INVENTOR:
				modelOperations.exportDataExchangeFile_Inventor(				p_Model,
																			in_ModelType,
																			i,
																			outputPathAndDir_multi,			// Only the path to the directory
																			outputFileName_multi);	// This the complete file name (e.g. bracket_asm.stp)	

				if ( in_LogProgress ) isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "   Created Inventor file(s) for "  << in_ModelName;
				break;

			case DATA_EXCHANGE_FORMAT_PARASOLID:
				modelOperations.exportDataExchangeFile_Parasolid(			p_Model,
																			in_ModelType,
																			i,
																			outputPathAndDir_multi,			// Only the path to the directory
																			outputFileName_multi);	// This the complete file name (e.g. bracket_asm.stp)	
				if ( in_LogProgress ) isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "   Created Parasolid file(s) for "  << in_ModelName;
				break;

			case DATA_EXCHANGE_DXF:
				modelOperations.exportDataExchangeFile_DXF(					p_Model,
																			in_ModelType,
																			i,
																			outputPathAndDir_multi,			// Only the path to the directory
																			outputFileName_multi);	// This the complete file name (e.g. bracket_asm.stp)	
				if ( in_LogProgress ) isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "   Created Parasolid file(s) for "  << in_ModelName;
				break;

			default:
				// Force this to be updated if new format is added.
				std::stringstream errorString;
				// WARNING - Do not call DataExchangeFormat_string in the following message. This could result in another exception being thrown.
				errorString << "Function - " << __FUNCTION__ << ", was passed: " << i.dataExchangeFormat  <<
					", which is an erroneous value. Allowed values are: " <<
					"DATA_EXCHANGE_FORMAT_STEP   DATA_EXCHANGE_FORMAT_STEREOLITHOGRAPHY   DATA_EXCHANGE_FORMAT_INVENTOR   DATA_EXCHANGE_FORMAT_PARASOLID   DATA_EXCHANGE_DXF";
				throw isis::application_exception(errorString);
				break;
		}


	}  // END 	for each ( DataExchangeSpecification i in in_DataExchangeSpecifications )

	// Set Creo back to the original working directory.  This would only be necessary
	// if multiple assemblies were begin created.
	//std::cout << std::endl << "in_WORKING_DIR: " << in_WORKING_DIR;
	MultiFormatString originalWorkingDirPath_MultiString( in_WORKING_DIR, CAD_PATH_SIZE - 1 );

	//std::cout << std::endl << "originalWorkingDirPath_MultiString: " << originalWorkingDirPath_MultiString;
	//isis::setCreoWorkingDirectory( originalWorkingDirPath_MultiString );
	cADsession.setCADWorkingDirectory(originalWorkingDirPath_MultiString);		
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////




bool DataExchange_Format_Version_InList( 
		e_DataExchangeFormat  in_DataExchangeFormat,  // Only currently support DATA_EXCHANGE_FORMAT_STEP
		e_DataExchangeVersion in_DataExchangeVersion, // AP203_SINGLE_FILE, AP203_E2_SINGLE_FILE, AP203_E2_SEPARATE_PART_FILES...
		const  std::list<DataExchangeSpecification> &in_DataExchangeSpecifications )
{
	for each ( isis::DataExchangeSpecification dexSpec in in_DataExchangeSpecifications )
	{
		if (  in_DataExchangeFormat   ==  dexSpec.dataExchangeFormat &&
				in_DataExchangeVersion  ==  dexSpec.dataExchangeVersion ) return true;
	}
	return false;
}
	
/*
bool SeparateSTEPPartFilesSpecified(const  std::list<DataExchangeSpecification> &in_DataExchangeSpecifications)
{

	for each ( isis::DataExchangeSpecification dexSpec in in_DataExchangeSpecifications )
	{
		if ( dexSpec.dataExchangeVersion == isis::AP203_E2_SEPARATE_PART_FILES || 
				dexSpec.dataExchangeVersion == isis::AP214_SEPARATE_PART_FILES )
		{
			return true;
		}
	}
	return false;
}
	
*/


/////////////////////////////////////////////////////////////////////////////////////////////////////////

// Pre-Conditions
//		The Creo model identified by in_AssemblyComponentID must be open and active in the Creo session
// Post-Conditions
//		If no exceptions, exported files (e.g. STEP) specified in in_DataExchangeSpecifications
void ExportDataExchangeFiles( 
					const std::string									&in_ComponentInstanceID,
					const MultiFormatString								&in_ModelName, 
					e_CADMdlType											in_ModelType,
					const MultiFormatString								&in_GeometryRepresentation,
					const std::string									&in_WORKING_DIR,
					const std::list<DataExchangeSpecification>          &in_DataExchangeSpecifications,
					bool												in_LogProgress )
																	throw (isis::application_exception)
{


	ExportDataExchangeFiles_driver(	in_ComponentInstanceID,
									in_ModelName, 
									in_ModelType,
									in_GeometryRepresentation,
									in_WORKING_DIR,
									in_DataExchangeSpecifications,
									in_LogProgress );



	/*****  Wavefront export not supported as of Creo 3.0
	See 
	Creo Support Site - See Document CS249910
	https://support.ptc.com/appserver/cs/view/solution.jsp?n=CS249910&art_lang=en&posno=11&q=ProIntf3DFileWrite&ProductFamily=Creo%20%26%20Pro%2FENGINEER%7CCreo%20Simulate%20%26%20Pro%2FMechanica&source=search

	*********/


}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef void ( *t_ExportDataExchangeFunction) ( 
					const std::string									&in_ComponentInstanceID,				
					const MultiFormatString								&in_ModelName, 
					//ProMdlType											in_ModelType,
					e_CADMdlType											in_ModelType,
					const MultiFormatString								&in_GeometryRepresentation,
					const std::string									&in_WORKING_DIR,
					const std::list<DataExchangeSpecification>          &in_DataExchangeSpecifications,
					bool												in_LogProgress );
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ComponentVistorExportDataExchange: public ComponentVistor
{
	private:
		t_ExportDataExchangeFunction			exportDataExchangeFuncdtion;
		std::string								workingDir;
		std::list<DataExchangeSpecification>	dataExchangeSpecifications;
		bool									logProgress;
		std::set<std::string>					exportedCADModels;  // 123.prt, bracket.asm

		// Don't allow default constructor
		ComponentVistorExportDataExchange();

	public:		
		ComponentVistorExportDataExchange( t_ExportDataExchangeFunction in_Function_ptr,
										   const std::string							&in_WORKING_DIR,
										   const std::list<DataExchangeSpecification>   &in_DataExchangeSpecifications,
										   bool											in_LogProgress ): 
														exportDataExchangeFuncdtion(in_Function_ptr),
														workingDir(in_WORKING_DIR),
														dataExchangeSpecifications(in_DataExchangeSpecifications),
														logProgress(in_LogProgress) {};

		virtual void operator() ( const std::string  &in_ComponentID, 
								  std::map<std::string, isis::CADComponentData> &in_CADComponentData_map )
		{

			isis::cad::CadFactoryAbstract_global *cadFactoryAbstract_global_ptr = isis::cad::CadFactoryAbstract_global::instance();
			isis::cad::CadFactoryAbstract::ptr	 cAD_Factory_ptr = cadFactoryAbstract_global_ptr->getCadFactoryAbstract_ptr();
			isis::cad::IModelHandling&           modelHandling = cAD_Factory_ptr->getModelHandling();


			//std::string modelNameWithSuffix = ConvertToUpperCase (CombineCreoModelNameAndSuffix(
			//											in_CADComponentData_map[in_ComponentID].name,
			//											ProMdlType_enum(in_CADComponentData_map[in_ComponentID].modelType)) );

			std::string modelNameWithSuffix = 
				ConvertToUpperCase(modelHandling.combineCADModelNameAndSuffix(in_CADComponentData_map[in_ComponentID].name, in_CADComponentData_map[in_ComponentID].modelType) );


			if ( exportedCADModels.find(modelNameWithSuffix) ==  exportedCADModels.end())
			{
				// Have not created this STEP file yet
				(*exportDataExchangeFuncdtion)( 
						in_ComponentID,
						in_CADComponentData_map[in_ComponentID].name,
						//ProMdlType_enum(in_CADComponentData_map[in_ComponentID].modelType),
						in_CADComponentData_map[in_ComponentID].modelType,
						in_CADComponentData_map[in_ComponentID].geometryRepresentation,
						workingDir,
						dataExchangeSpecifications,
						logProgress );

				// Add to the list of created this STEP files 
				exportedCADModels.insert(modelNameWithSuffix);
			}

		}
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ExportDataExchangeFiles( 
				const std::string								&in_ComponentID,  // Could be an assembly or part
				const std::string								&in_WORKING_DIR,
				const std::list<DataExchangeSpecification>      &in_DataExchangeSpecifications,
				std::map<std::string, isis::CADComponentData>			&in_CADComponentData_map,
				bool												in_LogProgress )
																throw (isis::application_exception)
{

	ComponentVistorExportDataExchange  componentVistorExportDataExchange( &ExportDataExchangeFiles_driver,
																			in_WORKING_DIR,
																			in_DataExchangeSpecifications,
																			in_LogProgress);
	VisitComponents(		in_ComponentID, 
						in_CADComponentData_map,
						componentVistorExportDataExchange);



	/*****  Wavefront export not supported as of Creo 3.0
	See 
	Creo Support Site - See Document CS249910
	https://support.ptc.com/appserver/cs/view/solution.jsp?n=CS249910&art_lang=en&posno=11&q=ProIntf3DFileWrite&ProductFamily=Creo%20%26%20Pro%2FENGINEER%7CCreo%20Simulate%20%26%20Pro%2FMechanica&source=search

	*********/

}

} // END 