#include "stdafx.h"
#include <DataExchange.h>
#include <ToolKitPassThroughFunctions.h>
#include <cc_CommonConstants.h>
#include <CommonFunctions.h>
#include "ProIntf3Dexport.h"
#include <cc_CommonUtilities.h>

namespace isis
{

	typedef struct raster_settings {
		ProRasterDepth depth;
		ProRasterType type;
		ProDotsPerInch inch;
		double  size[2];
	} RasterSettings;

	bool FillOutMetricsFileForLeafAssemblies( const std::list<e_DataExchangeSpecialInstruction> &in_DataExchangeSpecifications)
	{
		for each ( e_DataExchangeSpecialInstruction i in in_DataExchangeSpecifications ) if ( i == DATA_EXCHANGE_LEAF_ASSEMBLIES_METRICS) return true; 

		return false;
	}


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
void ExportRasterImage(	const std::string									&in_AssemblyComponentID, 
						std::map<string, isis::CADComponentData>			&in_CADComponentData_map )
																	throw (isis::application_exception)
{
	

	ProMdl     p_asm;
	wchar_t  AssemblyName[ISIS_CHAR_BUFFER_LENGTH];
	ProStringToWstring(AssemblyName, (char *)(const char*)in_CADComponentData_map[in_AssemblyComponentID].name );
	isis::isis_ProMdlRetrieve((const wchar_t *)AssemblyName,(ProMdlType)PRO_ASSEMBLY,&p_asm);


	RasterSettings set;
    set.depth = PRORASTERDEPTH_8;
    set.type = PRORASTERTYPE_TIFF;
    set.inch = PRORASTERDPI_100;
    set.size[0] = 4;
    set.size[1] = 5;

    int            window_id;
	isis::isis_ProWindowCurrentGet(&window_id);

	isis_LOG(lg, isis_FILE, isis_INFO) << "@@@@@@@@@@@@@@@ Writing tiff file";
	wchar_t  OutFileName_wide[ISIS_CHAR_BUFFER_LENGTH];

	string OutfileName = (const std::string&)in_CADComponentData_map[in_AssemblyComponentID].name + ".tif";
	
	ProStringToWstring(OutFileName_wide, (char *)OutfileName.c_str() );

	isis::isis_ProRasterFileWrite(window_id, set.depth, set.size[0], set.size[1],
        set.inch, set.type, OutFileName_wide);
}

// Pre-Conditions
//		The Creo model identified by in_AssemblyComponentID must be open and active in the Creo session
// Post-Conditions
//		If no exceptions, exported files (e.g. STEP) specified in in_DataExchangeSpecifications
void ExportDataExchangeFiles_STEP( 
					const std::string									&in_ComponentInstanceID,
					const MultiFormatString								&in_ModelName, 
					ProMdlType											in_ModelType,
					const MultiFormatString								&in_GeometryRepresentation,
					const std::string									&in_WORKING_DIR,
					const std::list<DataExchangeSpecification>          &in_DataExchangeSpecifications,
					bool												in_LogProgress )
																	throw (isis::application_exception)
{
	ProMdl     p_Model;
	

	//////////////////////
	// Retrieve Model
	//////////////////////
	//MultiFormatString ModelName(in_ModelName, PRO_NAME_SIZE - 1);

	//std::cout << std::endl << ModelName;

	//isis::isis_ProMdlRetrieve(in_ModelName,in_ModelType,&p_Model);

	isis_ProMdlRetrieve_WithDescriptiveErrorMsg( 
											// Added Arguments
											in_ComponentInstanceID,
											in_ModelName,
											in_GeometryRepresentation,
											// Original arguments
											in_ModelName, 
											in_ModelType,
											&p_Model);


	// std::cout << std::endl << " ******  ExportDataExchangeFiles_STEP, in_ModelName: " << in_ModelName;

	//////////////////////////////////
	// Set Macroscopic Config Setting
	/////////////////////////////////
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

	for each ( DataExchangeSpecification i in in_DataExchangeSpecifications )
	{
		if ( i.dataExchangeFormat == DATA_EXCHANGE_FORMAT_STEP )
		{
			switch(i.dataExchangeVersion)
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
					char temp_char_array[ISIS_CHAR_BUFFER_LENGTH];
					string temp_string = "Function ExportDataExchangeFiles was passed " + 
						std::string(itoa(i.dataExchangeVersion, temp_char_array, 10)) + 
						" which is an erroneous type.  Allowed enum values are AP203_SINGLE_FILE, AP203_E2_SINGLE_FILE, AP203_E2_SEPARATE_PART_FILES, AP214_SINGLE_FILE and AP214_SEPARATE_PART_FILES.";
					throw isis::application_exception(temp_string.c_str());
			}


			//////////////////////////
			// Create the directory
			//////////////////////////
			std::string outDirName = isis::DataExchangeVersion_string(i.dataExchangeVersion);
			std::string createOutDir = "if not exist " +  outDirName +  " mkdir  " + outDirName;
			isis::ExecuteSystemCommand( createOutDir);



			// Set the Creo directory to the created directory
			std::string outPathAndDir = in_WORKING_DIR + "\\" + outDirName;

			MultiFormatString WorkingDirPath_MultiString( outPathAndDir, PRO_PATH_SIZE - 1 );

			//std::cout << std::endl << "Before isis::isis_ProDirectoryChange( WorkingDirPath_MultiString );";
			setCreoWorkingDirectory(WorkingDirPath_MultiString);  
			//std::cout << std::endl << "After isis::isis_ProDirectoryChange( WorkingDirPath_MultiString );";


			// Write Data Exchange file (e.g. STEP File)
			int Arg_2 = 1;

			//std::string StepFileName_string;
			MultiFormatString	StepFileName_string;

			switch (in_ModelType)
			{
				case PRO_PART:
					StepFileName_string = (std::string)in_ModelName + "_prt.stp";
					break;
				case PRO_ASSEMBLY:
					StepFileName_string = (std::string)in_ModelName + "_asm.stp";
					break;
				default:
					StepFileName_string = (std::string)in_ModelName + ".stp";
			}

			//MultiFormatString StepFileName_multiString(StepFileName_string, PRO_FILE_NAME_SIZE - 1 );

		

				//isis_ProOutputFileWrite(	p_Model,    // This function was deprecated as of Creo 3.0
				isis_ProOutputFileMdlnameWrite(	p_Model,
										StepFileName_string,
										PRO_STEP_FILE,
										NULL,
										&Arg_2,
										NULL,
										NULL );



			if ( in_LogProgress )
			{
				isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "   Created " << outDirName << " STEP file(s) for "  << in_ModelName;
			}
		} // END if ( i.dataExchangeFormat == DATA_EXCHANGE_FORMAT_STEP )
	}  // END 	for each ( DataExchangeSpecification i in in_DataExchangeSpecifications )

	// Set Creo back to the original working directory.  This would only be necessary
	// if multiple assemblies were begin created.
	//std::cout << std::endl << "in_WORKING_DIR: " << in_WORKING_DIR;
	MultiFormatString originalWorkingDirPath_MultiString( in_WORKING_DIR, PRO_PATH_SIZE - 1 );

	//std::cout << std::endl << "originalWorkingDirPath_MultiString: " << originalWorkingDirPath_MultiString;
	isis::setCreoWorkingDirectory( originalWorkingDirPath_MultiString );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ExportDataExchangeFiles_Stereolithography_or_Inventor( 
					const std::string									&in_ComponentInstanceID,
					const MultiFormatString								&in_ModelName, 
					ProMdlType											in_ModelType,
					const isis::MultiFormatString						&in_GeometryRepresentation,
					const std::string									&in_WORKING_DIR,
					const std::list<DataExchangeSpecification>          &in_DataExchangeSpecifications,
					bool												in_LogProgress )
																	throw (isis::application_exception)
{
	ProMdl     p_Model;
	

	//////////////////////
	// Retrieve Model
	//////////////////////
	//MultiFormatString ModelName(in_ModelName, PRO_NAME_SIZE - 1);

	//std::cout << std::endl << ModelName;

	//isis::isis_ProMdlRetrieve(in_ModelName,in_ModelType,&p_Model);

	isis_ProMdlRetrieve_WithDescriptiveErrorMsg( 
											// Added Arguments
											in_ComponentInstanceID,
											in_ModelName,
											in_GeometryRepresentation,
											// Original arguments
											in_ModelName, 
											in_ModelType,
											&p_Model);
	

	////////////////////////////
	// Set to simplified Rep
	///////////////////////////
	/*  Don't use this approach use isis_ProMdlRetrieve_WithDescriptiveErrorMsg

	if ( in_GeometryRepresentation.size() > 0 )
	{
		ProSimprep proSimprep_temp;
		ProError	proError_temp  = ProSimprepInit ((wchar_t*)(const wchar_t*) in_GeometryRepresentation,
														  -1,
														  (ProSolid) p_Model,
														  &proSimprep_temp );

		if ( proError_temp == PRO_TK_NO_ERROR )
		{
			// Found simplified rep.

		}

	}
	*/

	//////////////////////////////////
	// Set Macroscopic Config Setting
	/////////////////////////////////
	//ProName		optionName;
	//ProPath		optionValue;

	//wcscpy( optionName, L"intf3d_out_datums_by_default");
	//wcscpy( optionValue, L"yes");
	//isis_ProConfigoptSet(optionName, optionValue  );

	//wcscpy( optionName, L"intf3d_out_parameters");
	//wcscpy( optionValue, L"all");
	//isis_ProConfigoptSet(optionName, optionValue  );

	for each ( DataExchangeSpecification i in in_DataExchangeSpecifications )
	{
		if ( i.dataExchangeFormat == DATA_EXCHANGE_FORMAT_STEREOLITHOGRAPHY || i.dataExchangeFormat == DATA_EXCHANGE_FORMAT_INVENTOR )
		{

			//////////////////////////
			// Create the directory
			//////////////////////////
			std::string outDirName;
			if (i.dataExchangeFormat == DATA_EXCHANGE_FORMAT_STEREOLITHOGRAPHY)
				outDirName = "STL_" + isis::DataExchangeVersion_string(i.dataExchangeVersion);
			else
				outDirName = "INVENTOR";

			std::string createOutDir = "if not exist " +  outDirName +  " mkdir  " + outDirName;
			isis::ExecuteSystemCommand( createOutDir);

			// Set the Creo directory to the created directory
			std::string outPathAndDir = in_WORKING_DIR + "\\" + outDirName;
			MultiFormatString WorkingDirPath_MultiString( outPathAndDir, PRO_PATH_SIZE - 1 );

			setCreoWorkingDirectory(WorkingDirPath_MultiString);  


			// Write Data Exchange file (e.g. Stereolithography File)
			int Arg_2 = 1;

			std::string suffix;
			if (i.dataExchangeFormat == DATA_EXCHANGE_FORMAT_STEREOLITHOGRAPHY)
				suffix = ".stl";
			else  // Must be Inventor
				suffix = ".iv";

			//std::string StepFileName_string;
			MultiFormatString OutputFileName_string;
			switch (in_ModelType)
			{
				case PRO_PART:
					OutputFileName_string = (std::string)in_ModelName + "_prt" + suffix;
					break;
				case PRO_ASSEMBLY:
					OutputFileName_string = (std::string)in_ModelName + "_asm" + suffix;
					break;
				default:
					OutputFileName_string = (std::string)in_ModelName + suffix;
			}

			//MultiFormatString StepFileName_multiString(StepFileName_string, PRO_FILE_NAME_SIZE - 1 );

			ProImportExportFile ExportFileType;

			if (i.dataExchangeFormat == DATA_EXCHANGE_FORMAT_STEREOLITHOGRAPHY)
			{
				ProName		optionName;
				ProPath		optionValue;

				wcscpy( optionName, L"intf3d_out_prop_chord_heights");
				wcscpy( optionValue, L"yes");
				isis_ProConfigoptSet(optionName, optionValue  );

				if ( i.dataExchangeVersion == ASCII )
						ExportFileType = PRO_SLA_ASCII_FILE;
					else
						ExportFileType = PRO_SLA_BINARY_FILE;
			}
			else  // Must be Inventor
			{
				ExportFileType = PRO_INVENTOR_FILE;
			}

				//isis_ProOutputFileWrite(	p_Model,   // This function was deprecated as of Creo 3.0
				isis_ProOutputFileMdlnameWrite(	p_Model,
										OutputFileName_string,
										ExportFileType,
										NULL,
										&Arg_2,
										NULL,
										NULL );

			if ( in_LogProgress )
			{
				isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "   Created " << outDirName << " file(s) for "  << in_ModelName;
			}
		} // END if ( i.dataExchangeFormat == DATA_EXCHANGE_FORMAT_STEP )
	}  // END 	for each ( DataExchangeSpecification i in in_DataExchangeSpecifications )

	// Set Creo back to the original working directory.  This would only be necessary
	// if multiple assemblies were begin created.
	//std::cout << std::endl << "in_WORKING_DIR: " << in_WORKING_DIR;
	MultiFormatString originalWorkingDirPath_MultiString( in_WORKING_DIR, PRO_PATH_SIZE - 1 );

	//std::cout << std::endl << "originalWorkingDirPath_MultiString: " << originalWorkingDirPath_MultiString;
	isis::setCreoWorkingDirectory( originalWorkingDirPath_MultiString );
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ExportDataExchangeFiles_DXF( 
					const std::string									&in_ComponentInstanceID,
					const MultiFormatString								&in_ModelName, 
					ProMdlType											in_ModelType,
					const isis::MultiFormatString						&in_GeometryRepresentation,
					const std::string									&in_WORKING_DIR,
					const std::list<DataExchangeSpecification>          &in_DataExchangeSpecifications,
					bool												in_LogProgress )
																	throw (isis::application_exception)
{
	ProMdl     p_Model;


	isis_ProMdlRetrieve_WithDescriptiveErrorMsg( 
											// Added Arguments
											in_ComponentInstanceID,
											in_ModelName,
											in_GeometryRepresentation,
											// Original arguments
											in_ModelName, 
											in_ModelType,
											&p_Model);
	


	for each ( DataExchangeSpecification i in in_DataExchangeSpecifications )
	{
		if ( i.dataExchangeFormat == DATA_EXCHANGE_DXF )
		{

			//////////////////////////
			// Create the directory
			//////////////////////////
			std::string outDirName;

			// Only supporting 2013, if later version become available, must do a switch statement
			outDirName = "DXF_2013";

			switch ( i.dataExchangeVersion )
			{
				case Y2013:
					outDirName = "DXF_2013";
					break;
				default:					
					std::stringstream errorString;
					errorString << "Function - " << __FUNCTION__ << ", DXF dataExchangeVersion integer ID: " + i.dataExchangeVersion   << 
			           ", which is an erroneous type. Allowed types are: " << 
					   "2013 (ID " << Y2013 << ").";
					throw isis::application_exception(errorString);		
			}


			std::string createOutDir = "if not exist " +  outDirName +  " mkdir  " + outDirName;
			isis::ExecuteSystemCommand( createOutDir);

			// Set the Creo directory to the created directory
			std::string outPathAndDir = in_WORKING_DIR + "\\" + outDirName;
			MultiFormatString WorkingDirPath_MultiString( outPathAndDir, PRO_PATH_SIZE - 1 );

			setCreoWorkingDirectory(WorkingDirPath_MultiString);  


			// Write Data Exchange file (e.g. Stereolithography File)  ??? is this needed
			//int Arg_2 = 1;

			std::string suffix;
			suffix = ".dxf";

			//std::string StepFileName_string;
			MultiFormatString OutputFileName_string;

			OutputFileName_string = (std::string)in_ModelName + suffix;

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

				isis::isis_ProIntf3DFileWrite( (ProSolid)p_Model, 
												ExportFileType, 
												(wchar_t*)(const wchar_t*)OutputFileName_string, 
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
				errorMsg_temp << "Failed to create DXF file for: " << (std::string)in_ModelName << 
					"\n   ProOutputAssemblyConfigurationIsSupported indicated that the DXF format is not supported for the assembly." <<
					"\n   To reproduce the problem, with Creo open the " << (std::string)in_ModelName << " assembly and try to save it to the DXF format.";
				ofstream DXF_Failed;
				DXF_Failed.open (outPathAndDir + "\\_FAILED.txt");
				DXF_Failed << errorMsg_temp.str();
				DXF_Failed.close();

				isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) << errorMsg_temp;

				throw isis::application_exception(errorMsg_temp);
			}

			if ( in_LogProgress )
			{
				isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "   Created " << outDirName << " file(s) for "  << in_ModelName;
			}
		} // END if ( i.dataExchangeFormat == DATA_EXCHANGE_FORMAT_STEP )
	}  // END 	for each ( DataExchangeSpecification i in in_DataExchangeSpecifications )

	// Set Creo back to the original working directory.  This would only be necessary
	// if multiple assemblies were begin created.
	//std::cout << std::endl << "in_WORKING_DIR: " << in_WORKING_DIR;
	MultiFormatString originalWorkingDirPath_MultiString( in_WORKING_DIR, PRO_PATH_SIZE - 1 );

	//std::cout << std::endl << "originalWorkingDirPath_MultiString: " << originalWorkingDirPath_MultiString;
	isis::setCreoWorkingDirectory( originalWorkingDirPath_MultiString );
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ExportDataExchangeFiles_Parasolid( 
					const std::string									&in_ComponentInstanceID,
					const MultiFormatString								&in_ModelName, 
					ProMdlType											in_ModelType,
					const isis::MultiFormatString						&in_GeometryRepresentation,
					const std::string									&in_WORKING_DIR,
					const std::list<DataExchangeSpecification>          &in_DataExchangeSpecifications,
					bool												in_LogProgress )
																	throw (isis::application_exception)
{
	ProMdl     p_Model;
	

	//////////////////////
	// Retrieve Model
	//////////////////////

	isis_ProMdlRetrieve_WithDescriptiveErrorMsg( 
											// Added Arguments
											in_ComponentInstanceID,
											in_ModelName,
											in_GeometryRepresentation,
											// Original arguments
											in_ModelName, 
											in_ModelType,
											&p_Model);
	


		//////////////////////////
		// Create the directory
		//////////////////////////
		std::string outDirName = "PARASOLID";
		std::string createOutDir = "if not exist " +  outDirName +  " mkdir  " + outDirName;
		isis::ExecuteSystemCommand( createOutDir);

		// Set the Creo directory to the created directory
		std::string outPathAndDir = in_WORKING_DIR + "\\" + outDirName;
		MultiFormatString WorkingDirPath_MultiString( outPathAndDir, PRO_PATH_SIZE - 1 );

		setCreoWorkingDirectory(WorkingDirPath_MultiString);  

		std::string suffix = ".x_t";

		//std::string StepFileName_string;
		MultiFormatString ParaSolidName_multi;
		switch (in_ModelType)
		{
			case PRO_PART:
				ParaSolidName_multi = (std::string)in_ModelName + "_prt" + suffix;
				break;
			case PRO_ASSEMBLY:
				ParaSolidName_multi = (std::string)in_ModelName + "_asm" + suffix;
				break;
			default:
				ParaSolidName_multi = (std::string)in_ModelName + suffix;
		}

		//MultiFormatString StepFileName_multiString(StepFileName_string, PRO_FILE_NAME_SIZE - 1 );

		pro_intf3d_export_type ExportFileType = PRO_INTF_EXPORT_PARASOLID;

		isis::isis_ProIntf3DFileWrite( (ProSolid)p_Model, 
										ExportFileType, 
										(wchar_t*)(const wchar_t*)ParaSolidName_multi, 
										PRO_OUTPUT_ASSEMBLY_FLAT_FILE,
										NULL, NULL, NULL, NULL);


		if ( in_LogProgress )
		{
			isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "   Created " << outDirName << " file(s) for "  << in_ModelName;
		}


	// Set Creo back to the original working directory.  This would only be necessary
	// if multiple assemblies were begin created.
	//std::cout << std::endl << "in_WORKING_DIR: " << in_WORKING_DIR;
	MultiFormatString originalWorkingDirPath_MultiString( in_WORKING_DIR, PRO_PATH_SIZE - 1 );

	//std::cout << std::endl << "originalWorkingDirPath_MultiString: " << originalWorkingDirPath_MultiString;
	isis::setCreoWorkingDirectory( originalWorkingDirPath_MultiString );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

// Pre-Conditions
//		The Creo model identified by in_AssemblyComponentID must be open and active in the Creo session
// Post-Conditions
//		If no exceptions, exported files (e.g. STEP) specified in in_DataExchangeSpecifications
void ExportDataExchangeFiles( 
					const std::string									&in_ComponentInstanceID,
					const MultiFormatString								&in_ModelName, 
					ProMdlType											in_ModelType,
					const MultiFormatString								&in_GeometryRepresentation,
					const std::string									&in_WORKING_DIR,
					const std::list<DataExchangeSpecification>          &in_DataExchangeSpecifications,
					bool												in_LogProgress )
																	throw (isis::application_exception)
{

	bool STEP_Files_Requested = false;
	bool Stereolithography_Files_Requested = false;
	bool Inventor_Files_Requested = false;
	bool Parasolid_Files_Requested = false;
	bool DXF_Files_Requested = false;
	bool Wavefront_Files_Requested = false;

	for each ( DataExchangeSpecification i in in_DataExchangeSpecifications )
	{
		if ( i.dataExchangeFormat == DATA_EXCHANGE_FORMAT_STEP ) STEP_Files_Requested = true;
		if ( i.dataExchangeFormat == DATA_EXCHANGE_FORMAT_STEREOLITHOGRAPHY ) Stereolithography_Files_Requested = true;
		if ( i.dataExchangeFormat == DATA_EXCHANGE_FORMAT_INVENTOR ) Inventor_Files_Requested = true;
		if ( i.dataExchangeFormat == DATA_EXCHANGE_FORMAT_PARASOLID ) Parasolid_Files_Requested = true;
		if ( i.dataExchangeFormat == DATA_EXCHANGE_DXF ) DXF_Files_Requested = true;
	}

	if ( STEP_Files_Requested )
	{
		ExportDataExchangeFiles_STEP(	in_ComponentInstanceID,
										in_ModelName, 
										in_ModelType,
										in_GeometryRepresentation,
										in_WORKING_DIR,
										in_DataExchangeSpecifications,
										in_LogProgress );
	}

	if ( Stereolithography_Files_Requested || Inventor_Files_Requested )
	{
		ExportDataExchangeFiles_Stereolithography_or_Inventor(	
										in_ComponentInstanceID,
										in_ModelName, 
										in_ModelType,
										in_GeometryRepresentation,
										in_WORKING_DIR,
										in_DataExchangeSpecifications,
										in_LogProgress );
	}

	if ( Parasolid_Files_Requested )
	{
		ExportDataExchangeFiles_Parasolid( in_ComponentInstanceID,
					in_ModelName, 
					in_ModelType,
					in_GeometryRepresentation,
					in_WORKING_DIR,
					in_DataExchangeSpecifications,
					true );

	}

	if ( DXF_Files_Requested )
	{
		ExportDataExchangeFiles_DXF( in_ComponentInstanceID,
					in_ModelName, 
					in_ModelType,
					in_GeometryRepresentation,
					in_WORKING_DIR,
					in_DataExchangeSpecifications,
					true );

	}


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
					ProMdlType											in_ModelType,
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
			std::string modelNameWithSuffix = ConvertToUpperCase (CombineCreoModelNameAndSuffix(
														in_CADComponentData_map[in_ComponentID].name,
														ProMdlType_enum(in_CADComponentData_map[in_ComponentID].modelType)) );


			if ( exportedCADModels.find(modelNameWithSuffix) ==  exportedCADModels.end())
			{
				// Have not created this STEP file yet
				(*exportDataExchangeFuncdtion)( 
						in_ComponentID,
						in_CADComponentData_map[in_ComponentID].name,
						ProMdlType_enum(in_CADComponentData_map[in_ComponentID].modelType),
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
				const std::string									&in_ComponentID,  // Could be an assembly or part
				const std::string									&in_WORKING_DIR,
				const std::list<DataExchangeSpecification>          &in_DataExchangeSpecifications,
				std::map<string, isis::CADComponentData>			&in_CADComponentData_map,
				bool												in_LogProgress )
																throw (isis::application_exception)
{


	bool STEP_Files_Requested = false;
	bool Stereolithography_Files_Requested = false;
	bool Inventor_Files_Requested = false;
	bool Parasolid_Files_Requested = false;
	bool DXF_Files_Requested = false;
	bool Wavefront_Files_Requested = false;

	for each ( DataExchangeSpecification i in in_DataExchangeSpecifications )
	{
		if ( i.dataExchangeFormat == DATA_EXCHANGE_FORMAT_STEP ) STEP_Files_Requested = true;
		if ( i.dataExchangeFormat == DATA_EXCHANGE_FORMAT_STEREOLITHOGRAPHY ) Stereolithography_Files_Requested = true;
		if ( i.dataExchangeFormat == DATA_EXCHANGE_FORMAT_INVENTOR ) Inventor_Files_Requested = true;
		if ( i.dataExchangeFormat == DATA_EXCHANGE_FORMAT_PARASOLID ) Parasolid_Files_Requested = true;
		if ( i.dataExchangeFormat == DATA_EXCHANGE_DXF ) DXF_Files_Requested = true;
	}

	if ( STEP_Files_Requested )
	{
		//ExportDataExchangeFiles_STEP(	in_CADComponentData_map[in_ComponentID].name, 
		//								in_CADComponentData_map[in_ComponentID].modelType,
		//								in_WORKING_DIR,
		//								in_DataExchangeSpecifications,
		//								in_LogProgress );

		// For STEP file, we want a file for the top assembly and each leaf
		ComponentVistorExportDataExchange  componentVistorExportDataExchange( &ExportDataExchangeFiles_STEP,
																			  in_WORKING_DIR,
																			  in_DataExchangeSpecifications,
																			  in_LogProgress);
		 VisitComponents(	in_ComponentID, 
							in_CADComponentData_map,
							componentVistorExportDataExchange);

	}


	if ( Stereolithography_Files_Requested || Inventor_Files_Requested )
	{
		// For stereolithography, we want a file for the top assembly and each leaf
		ComponentVistorExportDataExchange  componentVistorExportDataExchange( &ExportDataExchangeFiles_Stereolithography_or_Inventor,
																			  in_WORKING_DIR,
																			  in_DataExchangeSpecifications,
																			  in_LogProgress);
		 VisitComponents(	in_ComponentID, 
							in_CADComponentData_map,
							componentVistorExportDataExchange);
	}

	if ( Parasolid_Files_Requested )
	{
		// For now, only export the top assembly.  Do not export individual files.  Currently, there is no requirement to 
		// export individual files.
		ExportDataExchangeFiles_Parasolid( in_ComponentID,
					in_CADComponentData_map[in_ComponentID].name, 
					ProMdlType_enum(in_CADComponentData_map[in_ComponentID].modelType),
					in_CADComponentData_map[in_ComponentID].geometryRepresentation,
					in_WORKING_DIR,
					in_DataExchangeSpecifications,
					true );
	}

	if ( DXF_Files_Requested )
	{
		// For now, only export the top assembly.  Do not export individual files.  Currently, there is no requirement to 
		// export individual files.
		ExportDataExchangeFiles_DXF( in_ComponentID,
					in_CADComponentData_map[in_ComponentID].name, 
					ProMdlType_enum(in_CADComponentData_map[in_ComponentID].modelType),
					in_CADComponentData_map[in_ComponentID].geometryRepresentation,
					in_WORKING_DIR,
					in_DataExchangeSpecifications,
					true );
	}


	/*****  Wavefront export not supported as of Creo 3.0
	See 
	Creo Support Site - See Document CS249910
	https://support.ptc.com/appserver/cs/view/solution.jsp?n=CS249910&art_lang=en&posno=11&q=ProIntf3DFileWrite&ProductFamily=Creo%20%26%20Pro%2FENGINEER%7CCreo%20Simulate%20%26%20Pro%2FMechanica&source=search

	*********/

}

}  // End namespace isis