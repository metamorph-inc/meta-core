#include "stdafx.h"
#include <AssemblyCreationViaInputFile.h>

#include <BuildAssembly.h>
#include <DiagnosticUtilities.h>
#include <ISISConstants.h>
#include <cc_XMLtoCADStructures.h>
#include <cc_CommonUtilities.h>
#include <ProEStructuresUtils.h>
#include <Metrics.h>
#include "CADEnvironmentSettings.h"
#include <DataExchange.h>
//#include "WindowsHDependentCommonFunctions.h"
#include "cc_WindowsFunctions.h"
#include <cc_MultiFormatString.h>
#include <CommonFunctions.h>
#include <SurvivabilityAnalysis.h>
#include <CFDAnalysis.h>
#include <MaterialProperties.h>
#include <sstream>
#include <fstream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp> 
#include "AssembleUtils.h"

// #define DEBUG_BREAKER 1

namespace isis
{


void CreateAssemblyViaInputFile( cad::CadFactoryAbstract						&in_factory,
								 const isis::ProgramInputArguments              &in_ProgramInputArguments,
								 const std::string								&in_CADExtensionsDir,
								 const std::string								&in_XMLInputFile_PathAndFileName,
								 const std::string								&in_WorkingDirector,
								 const std::string								&in_CADPartsLibDir,
								 const std::string								&in_LogFile_PathAndFileName,
								 const std::string								&in_templateFile_PathAndFileName,
								 const std::string								&in_CreoStartCommand,
								 const std::string								&in_ProgramName_Version_TimeStamp,
								 unsigned int									&in_out_UniqueNameIndex,
								 bool											&out_Pro_E_Running,
								 isis::CADAssemblies							&out_CADComponentAssemblies,
								 std::map<std::string, isis::CADComponentData>	&out_CADComponentData_map) 
																					throw (isis::application_exception)
{
#ifdef DEBUG_BREAKER
	bool breaker = true;
	while( breaker ) {
		boost::this_thread::sleep(boost::posix_time::seconds(2));
	}
#endif

	//cad::CadFactoryAbstract::ptr cad_factory = isis::cad::creo::create();
	

	//bool Pro_E_Running = false;
	bool Template_Copied = false;
	int NonCyPhyID_counter = 0;  // This will be incremented before it is used.
	int ExitCode = 0;
	try
	{
		time_t time_start; /* calendar time */
		time_start=time(NULL); /* get current cal time */

		bool regenerationSucceeded_ForAllAssemblies = true;
		std::string cadPartsLibDir = in_CADPartsLibDir;

		// Verify that the XML file exists.
		if (!isis::FileExists( in_XMLInputFile_PathAndFileName.c_str()))
		{
			std::string TempError = "Could not find the assembly definition xml file, file name: " + in_XMLInputFile_PathAndFileName;
			throw isis::application_exception(TempError.c_str());
		} 

		//std::map<std::string, isis::CADComponentData> zCADComponentData_map;
		//isis::CADAssemblies zCADComponentAssemblies;

		vector<CADCreateAssemblyError> errorList;

		/**
		Initial load of the map, one entry for each part/assembly file.
		*/
		FromXMLFile_PopulateCADComponentAssemblyAndMap(
													XML_DEFINED_BY_FILE,
													in_XMLInputFile_PathAndFileName,
													out_CADComponentAssemblies,
													out_CADComponentData_map,
													errorList);


		if ( out_CADComponentAssemblies.topLevelAssemblies.size() == 0 ) 
		{
			isis_LOG(lg, isis_FILE, isis_INFO) << "";
			isis_LOG(lg, isis_FILE, isis_INFO) << "*************************** Begin Assembly Creation **************************";
			std::stringstream informationString;
			informationString << "No assemblies were created because the input xml file, " 
					  << std::endl << in_XMLInputFile_PathAndFileName 
					  << ", did not define any assemblies.";
			isis_LOG(lg, isis_FILE, isis_WARN) << informationString.str();

			if ( out_CADComponentAssemblies.unassembledComponents.size() == 0 )
			{
				std::stringstream errorString;
				errorString <<
						"The input xml file, "
						 << std::endl << in_XMLInputFile_PathAndFileName 
						 << ", did not define any assemblies nor did it define unassembled parts/sub-assemblies.  "
						 << std::endl << "The input xml file must specify assemblies and/or unassembled parts/sub-assemblies."; 
						throw isis::application_exception(errorString.str());
			}
			isis_LOG(lg, isis_FILE, isis_INFO) << "************************** End Assembly Creation *****************************";
		}

		///////////////////////////////////
		// Output Requested Metrics file
		///////////////////////////////////
		std::string requestedMetricFile_PathAndFileName =  in_WorkingDirector + "\\RequestedMetrics.xml";
		isis::IfFileExists_DeleteFile( requestedMetricFile_PathAndFileName);
		isis::CreateXMLFile_RequestedMetrics(	requestedMetricFile_PathAndFileName,
												out_CADComponentAssemblies.topLevelAssemblies,
												out_CADComponentData_map );


		//////////////////////////////////////////////////
		// Modify Parametric Models to Have a Unique Name
		//////////////////////////////////////////////////
		std::vector<CopyModelDefinition>			fromModel_ToModel;


		isis::ModifyToHaveAUniqueName_ForEach_PartAndOrAssembly(	in_out_UniqueNameIndex, 
																	e_PART_OR_ASSEMBLY_MODEL_TYPE,
																	e_SELECT_ONLY_PARAMETRIC_MODELS,
																	true, 
																	out_CADComponentData_map, 
																	fromModel_ToModel );

		isis_LOG(lg, isis_FILE, isis_INFO) << "";
		isis_LOG(lg, isis_FILE, isis_INFO) << "************** Begin Modified Part Names for Multiple Parametric Parts *****************";
		isis_LOG(lg, isis_FILE, isis_INFO) << "From-Model-Name    To-Model-Name    Model-Type";
		isis_LOG(lg, isis_FILE, isis_INFO) << fromModel_ToModel;
		isis_LOG(lg, isis_FILE, isis_INFO) << "";
		isis_LOG(lg, isis_FILE, isis_INFO) << "************** End Modified Part Names for Multiple Parametric Parts *****************";

		// If this is an analysis run, must modify so that each part has a unique name.  
		// e.g. if Part_A appears three times
		// in the assembly, then copies must be made such that there is a Part_A, Part_A_C001, and Part_A_C002.
		// This is needed to assure that the material properties in the generated mesh will have a unique name.
		// A subsequent part of this program will create dummy materials for each part and the dummy materials
		// will have the name of the form ZZ_FEA_MAT_1_ZZ, ZZ_FEA_MAT_2_ZZ... This will assure that all materials
		// have a unique name and thus have a separate MAT property in the Nastran deck.
		// Note - We are assuming if any assembly (out_CADComponentAssemblies) is an analysis run then all 
		// assemblies will be for analaysis purposes.  We will not mix building assembies for analsys purposes
		// and building assembies for general purposes.
		
		bool fEAAnalysisDeckBasedRun		= IsAFEAAnlysisDeckBasedRun(out_CADComponentAssemblies);
		bool fEAAnalysisAbaqusModelBasedRun	= IsFEAAnalysisAbaqusModelBasedRun(out_CADComponentAssemblies);
		bool fEAAnalysisRun = false;
		if ( fEAAnalysisDeckBasedRun  || fEAAnalysisAbaqusModelBasedRun ) fEAAnalysisRun = true;
		bool ballisticAnalysisRun				= IsABallisticAnalysisRun(out_CADComponentAssemblies);
		bool blastAnalysisRun					= IsABlastAnlysisRun(out_CADComponentAssemblies);
		bool cFDAnalysisRun						= IsACFDAnalysisRun(out_CADComponentAssemblies);
		bool interferenceRun					= IsAInterferenceRun(out_CADComponentAssemblies);
		bool hasAssemblyBasedComputations		= HasAssemblyBasedComputations( out_CADComponentAssemblies);
		bool fillOutMetricsFileForLeafAssemblies = FillOutMetricsFileForLeafAssemblies( out_CADComponentAssemblies.dataExchangeSpecialInstruction);
		
		bool CompleteTheHierarchyForLeafAssemblies =	false;
		bool UniquelyNameAllCADModelInstances =			false;
		bool OutputJointInformation	=					false;
		bool ValidateJointInformation =					false;

		if ( fEAAnalysisRun )
		{
			UniquelyNameAllCADModelInstances = true;
			CompleteTheHierarchyForLeafAssemblies = true;
		}

		if ( ballisticAnalysisRun  || blastAnalysisRun || fEAAnalysisRun ||  fillOutMetricsFileForLeafAssemblies )
			CompleteTheHierarchyForLeafAssemblies = true;

		if ( !CompleteTheHierarchyForLeafAssemblies ) 
				CompleteTheHierarchyForLeafAssemblies = Get_CompleteTheHierarchyForLeafAssemblies (out_CADComponentAssemblies);

		if ( !UniquelyNameAllCADModelInstances ) 
				UniquelyNameAllCADModelInstances = Get_UniquelyNameAllCADModelInstances (out_CADComponentAssemblies);

		if ( !OutputJointInformation ) 
				OutputJointInformation = Get_OutputJointInformation (out_CADComponentAssemblies);

		if ( OutputJointInformation )
				ValidateJointInformation = Get_ValidateJointInformation (out_CADComponentAssemblies);

		if ( fEAAnalysisRun || ballisticAnalysisRun  || blastAnalysisRun || cFDAnalysisRun )
		{
			/////////////////////////////////////////////////////
			// Verify One and only One Assembly per analysis run
			/////////////////////////////////////////////////////
			if ( out_CADComponentAssemblies.topLevelAssemblies.size() != 1 )
			{
				std::stringstream errorString;
				errorString << "One and only one assembly allowed per analysis run (FEA, Ballistic, Blast, or  CFD).  The XML file defines " <<
				out_CADComponentAssemblies.topLevelAssemblies.size() << std::string(" assemblies.");
					throw isis::application_exception(errorString.str());
			}

			if ( out_CADComponentAssemblies.unassembledComponents.size() != 0 )
			{
				std::stringstream errorString;
				errorString << "Unassembled components were found.  Cannot perform analysis (FEA, Ballistic, Blast, or  CFD) on partial assemblies.  Number of unassembled components: " <<
				out_CADComponentAssemblies.unassembledComponents.size();
					throw isis::application_exception(errorString.str());
			}
		}

		if ( interferenceRun ) 
			Validate_ComputationInterferenceCount_ThrowExceptionIfInvalid ( out_CADComponentAssemblies, out_CADComponentData_map);
										

		if ( UniquelyNameAllCADModelInstances  )
		{
			////////////////////////////////////////////////////////////
			// Uniquely name parts for parts that occur multiple times
			////////////////////////////////////////////////////////////
			// Do the rename here, this will get all models that are defined in the input xml.  A later
			// rename will get the models that were CyPhy leafs.  Renaming the models that were do to leafs is
			// more difficult.  The rename occurring via the following function call requires less resources 
			// because it will assembly the renamed model as opposed to the more complex Assembly_RenameSubPartOrSubAssembly 
			// function called later.
 	
			// Since we already have unique names for parametric parts/assemblies, this will make the non-parametric parts unique.
			isis::ModifyToHaveAUniqueName_ForEach_PartAndOrAssembly(	in_out_UniqueNameIndex, 
																		e_PART_OR_ASSEMBLY_MODEL_TYPE,
																		e_SELECT_ALL_MODELS,
																		false,  
																		out_CADComponentData_map, 
																		fromModel_ToModel );
			////////////////////////////
			// Log Part Copy/Renames
			///////////////////////////
			isis_LOG(lg, isis_FILE, isis_INFO) << "";
			isis_LOG(lg, isis_FILE, isis_INFO) << isis_EOL << "************** Begin Modified Part Names for Analysis Purposes *****************";
			isis_LOG(lg, isis_FILE, isis_INFO) << "From_Part_Name   To_Part_Name";
			isis_LOG(lg, isis_FILE, isis_INFO) << fromModel_ToModel;
			isis_LOG(lg, isis_FILE, isis_INFO) << "";
			isis_LOG(lg, isis_FILE, isis_INFO) << isis_EOL << "************** End Modified Part Names for Analysis Purposes *****************";
		}
		

		// Add the depends-on information to the CADComponentData
		isis::Add_dependsOn( out_CADComponentData_map );

		//stream_CADComponentData_map(out_CADComponentData_map, clog );
		//stream_CADComponentData_map(out_CADComponentData_map, cout );

		//for ( std::list<isis::TopLevelAssemblyData>::const_iterator i(out_CADComponentAssemblies.topLevelAssemblies.begin());
		//	  i != out_CADComponentAssemblies.topLevelAssemblies.end();
		//	  ++i )
		for each( isis::TopLevelAssemblyData i in out_CADComponentAssemblies.topLevelAssemblies)
		{
			isis_LOG(lg, isis_FILE, isis_INFO) << "";
			isis_LOG(lg, isis_FILE, isis_INFO) << "************** Begin Entire Tree For a Single Assembly  *****************";
			isis_LOG(lg, isis_FILE, isis_INFO) << "";
			isis_LOG(lg, isis_FILE, isis_INFO) << "************** Begin Assembly Level Computations (i.e. Metrics)  *****************";
			for each ( CADComputation ii in i.assemblyMetrics ) isis_LOG(lg, isis_FILE, isis_INFO) << ii;
			isis_LOG(lg, isis_FILE, isis_INFO) << "";
			isis_LOG(lg, isis_FILE, isis_INFO) << "************** End Assembly Level Computations (i.e. Metrics)  *******************";
			isis_LOG(lg, isis_FILE, isis_INFO) << "";
			isis_LOG(lg, isis_FILE, isis_INFO) << "************** Begin Analysis Data For a Single Assembly  *****************";
			std::stringstream str;
			stream_AnalysisInputData( i.analysesCAD, str);
			isis_LOG(lg, isis_FILE, isis_INFO) << str.str();
			isis_LOG(lg, isis_FILE, isis_INFO) << "";
			isis_LOG(lg, isis_FILE, isis_INFO) << "************** End Analysis Data For a Single Assembly  *****************";
			isis_LOG(lg, isis_FILE, isis_INFO) << "";
			isis_LOG(lg, isis_FILE, isis_INFO) << "************** Begin Entire Component Data Tree (CAD Internal Structures) *****************";
			str.clear();
			stream_AssemblyCADComponentData( i.assemblyComponentID, out_CADComponentData_map, str);
			isis_LOG(lg, isis_FILE, isis_INFO) << str.str();
			isis_LOG(lg, isis_FILE, isis_INFO) << "";
			isis_LOG(lg, isis_FILE, isis_INFO) << "************** Begin Entire Component Data Tree (CAD Internal Structures) *****************";
			isis_LOG(lg, isis_FILE, isis_INFO) << "";
			isis_LOG(lg, isis_FILE, isis_INFO) << "************** End Entire Tree For a Single Assembly  *****************";
		}

		///////////////////////////////////////////////////////////////////////////////////
		// Check for Non-Size-To-Fit components being constrained to Size-To-Fit components
		////////////////////////////////////////////////////////////////////////////////////
		std::set<std::string> TempIntersection = NonSizeToFitComponents_ReferencedBy_SizeToFitConstraints( out_CADComponentData_map );
		if ( TempIntersection.size() > 0 )
		{
			string err_str = "Erroneous XML File: A NON_SIZE_TO_FIT component cannot be constrained to a SIZE_TO_FIT component. " +
				std::string("The SIZE_TO_FIT component(s) (i.e. ComponentIDs) that were erroneously referenced are:");
			for ( std::set<std::string>::const_iterator i(TempIntersection.begin()); i != TempIntersection.end(); ++i )
			{
				err_str += " " + *i;
			}
			err_str += ".  Search for the ComponentIDs in the log file to identify the places where the SIZE_TO_FIT components are erroneously referenced.";
			throw isis::application_exception(err_str.c_str());  

		}

		std::string configPro_PathAndFileName = in_WorkingDirector + "\\config.pro";
		// * 1-10-2013 Cphy2CAD now creates the search_META.pro" std::string searchMetaPro_PathAndFileName = workingDir + "\\search_META.pro";
		isis::IfFileExists_DeleteFile( configPro_PathAndFileName);
		// * 1-10-2013 Cphy2CAD now creates the search_META.pro" isis::IfFileExists_DeleteFile( searchMetaPro_PathAndFileName);



		//isis::cad::DataContainer 

		////////////////////////////////////////////////
		// Write config.pro 
		///////////////////////////////////////////////

		ofstream config_Pro;
		config_Pro.open (configPro_PathAndFileName );
		config_Pro << "override_store_back yes\n";
		std::string searchMetaFileName = ".\\search_META.pro";
		if ( isis::FileExists( searchMetaFileName.c_str() )) {
			config_Pro << "search_path_file " <<  searchMetaFileName;
		}


		if ( cadPartsLibDir.size() > 0 )
		{
			// Check if cADPartsLibDir is in double quotes.  If not add double quotes.
			// This is necessary because Creo Config options with spaces must be enclosed in double quotes.
			if ( cadPartsLibDir.find("\"")  == string::npos )	cadPartsLibDir = "\"" + cadPartsLibDir + "\"";
			config_Pro << std::endl << "search_path " <<  cadPartsLibDir;
		}
		
		config_Pro << std::endl << "pro_material_dir " << CreoMaterialMTLFilesDir_Path();

		// add component creo plugin if in graphics mode
		// config_Pro << std::endl << "toolkit_registry_file  \"" << META_PATH() << "bin\\CAD\\Creo\\plugins\\protk.dat\"";

		config_Pro.close();

		/////////////////////////////
		/////// Start Pro/E /////////
		/////////////////////////////
		isis_LOG(lg, isis_CONSOLE, isis_INFO)  << "CADCreoParametricCreateAssembly "<< ASSEMBLE_PTC_VERSION;
		isis_LOG(lg, isis_CONSOLE, isis_INFO)  << "Starting Creo-Parametric, this takes about 10 seconds...";

		char tempBuffer[1024];
		strcpy(tempBuffer, in_CreoStartCommand.c_str() );
		isis::isis_ProEngineerStart(tempBuffer,"");

		int creoVersionNumber;
		isis_ProEngineerReleaseNumericversionGet(&creoVersionNumber);
		isis_LOG(lg, isis_FILE, isis_INFO) << "Creo Version Number: " <<  creoVersionNumber << ".  Creo Parametric 2.0 is version 31.";

		out_Pro_E_Running = true;
		cout <<  endl << "Creo-Parametric successfully started." << endl;

		//ProPath  WorkingDirPath_wchar;
		//ProStringToWstring(WorkingDirPath_wchar, (char *)workingDir.c_str() );

		isis::MultiFormatString workingDir_MultiFormat(in_WorkingDirector, PRO_PATH_SIZE - 1);
		//isis::isis_ProDirectoryChange( workingDir_MultiFormat );
		isis::setCreoWorkingDirectory( workingDir_MultiFormat );

		// Copy template model to the working directory
		isis::CopyFileIsis( in_templateFile_PathAndFileName,  in_WorkingDirector );
		Template_Copied = true;

		// *** If model renames occured
		// WARNING - isis::CopyModels must be called after 
		//			 1) Creo has started
		//			 2) isis_ProDirectoryChange changed to the working directory
		//           been created.
		//			 3) the search_META.pro has been set 
		//if ( ToPartName_FromPartName_map.size() > 0 ) isis::CopyModels(ToPartName_FromPartName_map);
		if ( fromModel_ToModel.size() > 0 ) isis::CopyModels(fromModel_ToModel);

		////////////////////////////////////////////////////////////////////////////
		// Setup for writing STEP file informatoin to \\manufacturing.manifest.json
		////////////////////////////////////////////////////////////////////////////
		bool manufacturingManifestJsonExists = isis::FileExists( isis::manufacturingManifestJson_PathAndFileName.c_str());
		bool separateStepPartFilesRequested = false;
		isis::e_DataExchangeVersion  stepFileVersion_ForManufacturing;

		if ( isis::DataExchange_Format_Version_InList(	isis::DATA_EXCHANGE_FORMAT_STEP,  
														isis::AP203_E2_SEPARATE_PART_FILES, 
														out_CADComponentAssemblies.DataExchangeSpecifications) )
		{
			stepFileVersion_ForManufacturing = isis::AP203_E2_SEPARATE_PART_FILES;
			separateStepPartFilesRequested = true;
		}
		else
		{
			if ( isis::DataExchange_Format_Version_InList(	isis::DATA_EXCHANGE_FORMAT_STEP,  
															isis::AP214_SEPARATE_PART_FILES, 
															out_CADComponentAssemblies.DataExchangeSpecifications) )
			{
				stepFileVersion_ForManufacturing = isis::AP214_SEPARATE_PART_FILES;
				separateStepPartFilesRequested = true;
			}
		}

		////////////////////////////////////////
		// Build the assemblies
		////////////////////////////////////////
		for ( std::list<isis::TopLevelAssemblyData>::const_iterator i(out_CADComponentAssemblies.topLevelAssemblies.begin());
			  i != out_CADComponentAssemblies.topLevelAssemblies.end();
			  ++i )
		{
			bool regenerationSucceeded;

			//////////////////////////
			// Build the Assembly
			//////////////////////////
			isis::BuildAssembly(	in_factory, 
									i->assemblyComponentID, 
									in_WorkingDirector, 
									true, 
									out_CADComponentData_map, 
									regenerationSucceeded, 
									errorList );		
			if ( !regenerationSucceeded ) regenerationSucceeded_ForAllAssemblies = false;

			//////////////////////////////////////
			// Display assembly created messages
			////////////////////////////////////
			time_t time_end;		/* calendar time */
			time_end=time(NULL);	/* get current cal time */

			time_t time_elapsed = time_end - time_start;

			isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "";
			isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "Assembly creation completed successfully.";

			// Get component count
			isis::ComponentVistorCountAssemblyComponents  componentVistorCountAssemblyComponents;
			isis::VisitComponents(i->assemblyComponentID, out_CADComponentData_map, componentVistorCountAssemblyComponents);

			// Log component count
			isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "   Number of assembled components: " << componentVistorCountAssemblyComponents.numberOfComponents;
			isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "   Elapsed wall-clock time:        " << time_elapsed << " seconds";

			time_start=time(NULL); // reset start time for subsequent assemblies if any


			///////////////////////////////////////////////
			// Complete The Hierarchy For Leaf Assemblies
			//////////////////////////////////////////////
			if (CompleteTheHierarchyForLeafAssemblies )
			{
				//////////////////////////////////////////////
				// Complete The Hierarchy For Leaf Assemblies
				//////////////////////////////////////////////
				isis::ComponentVistorBuildListOfComponentIDs  assemblyComponentIDs_ExcludingTopAssembly(true);
				isis::VisitComponents(i->assemblyComponentID, out_CADComponentData_map, assemblyComponentIDs_ExcludingTopAssembly );
				// When complete each component may not have a unique name (nor unique file).
				// They will *all* be present in the map.
				ForEachLeafAssemblyInTheInputXML_AddInformationAboutSubordinates(	
															assemblyComponentIDs_ExcludingTopAssembly.listOfComponentIDs, 
															NonCyPhyID_counter,
															out_CADComponentData_map );
			}		

			
			///////////////////////////////////////////////
			// Uniquely Name All CAD Model Instances
			//////////////////////////////////////////////
			if (CompleteTheHierarchyForLeafAssemblies && UniquelyNameAllCADModelInstances  )
			{
					// At this point, the leaf assembly information would have been completed (i.e. leaf assemblies 
					// filled out). Now, we must make sure each part is uniquely named.  Previously each model listed in the
					// input xml would have uniqeuly named.  Now we need to uniquely name the models that 
					// would have been added because the hierarchy was completed for leaf assemblies.
					std::vector<CopyModelDefinition>			fromModel_ToModel_FEA;
					isis::ModifyToHaveAUniqueName_ForEach_PartAndOrAssembly(	in_out_UniqueNameIndex, 
																	//e_PART_MODEL_TYPE,
																	e_PART_OR_ASSEMBLY_MODEL_TYPE,
																	e_SELECT_ALL_MODELS,
																	false,
																	out_CADComponentData_map, 
																	fromModel_ToModel_FEA );

					////////////////////////////
					// Log Part Copy/Renames
					///////////////////////////
					isis_LOG(lg, isis_FILE, isis_INFO) << "";
					isis_LOG(lg, isis_FILE, isis_INFO)  << "************** Begin Modified Part Names for Analysis Purposes *****************";
					isis_LOG(lg, isis_FILE, isis_INFO) << "From_Part_Name   To_Part_Name";
					isis_LOG(lg, isis_FILE, isis_INFO) << fromModel_ToModel_FEA;
					isis_LOG(lg, isis_FILE, isis_INFO) << "";
					isis_LOG(lg, isis_FILE, isis_INFO)  << "************** End Modified Part Names for Analysis Purposes *****************";

					if ( fromModel_ToModel_FEA.size() > 0 ) isis::CopyModels(fromModel_ToModel_FEA);

					if ( fromModel_ToModel_FEA.size() > 0 ) 
					{
						//isis::CopyModels(fromModel_ToModel);
						// Rename the parts/sub-assemblies in the Creo assembly that were copied in the previouisl step

						// Must do assemblies first
						for each ( CopyModelDefinition model_def in fromModel_ToModel_FEA )
						{
							if (model_def.modelType == PRO_MDL_ASSEMBLY )
							{
								ModelInstanceData modelInstanceData_temp;
								modelInstanceData_temp.modelName				= model_def.fromModelName;
								modelInstanceData_temp.modelType				= model_def.modelType;
								modelInstanceData_temp.modelHandle				= static_cast<ProSolid>(out_CADComponentData_map[model_def.componentInstanceID].cADModel_hdl);

								//modelInstanceData_temp.assembledFeature			= out_CADComponentData_map[model_def.componentInstanceID].assembledFeature;			
								//modelInstanceData_temp.assembledFeature.type =	FeatureGeometryType_enum(out_CADComponentData_map[model_def.componentInstanceID].assembledFeature.type);
								//modelInstanceData_temp.assembledFeature.id   =	                         out_CADComponentData_map[model_def.componentInstanceID].assembledFeature.id;
								//modelInstanceData_temp.assembledFeature.owner =	                         out_CADComponentData_map[model_def.componentInstanceID].assembledFeature.owner; 
								modelInstanceData_temp.assembledFeature = getProAsmcomp(out_CADComponentData_map[model_def.componentInstanceID].assembledFeature);
								
								modelInstanceData_temp.topAssemblyModelHandle	= static_cast<ProSolid>(out_CADComponentData_map[i->assemblyComponentID].cADModel_hdl);
								modelInstanceData_temp.componentPaths			= out_CADComponentData_map[model_def.componentInstanceID].componentPaths;
								isis::MultiFormatString  CopyToPartName_temp(model_def.toModelName);

								ProMdl     renamedModelHandle;
								Assembly_RenameSubPartOrSubAssembly ( modelInstanceData_temp, CopyToPartName_temp, renamedModelHandle );
								out_CADComponentData_map[model_def.componentInstanceID].cADModel_hdl = (ProSolid)renamedModelHandle;
								out_CADComponentData_map[model_def.componentInstanceID].cADModel_ptr_ptr = (ProMdl*)&renamedModelHandle;


								// Must fix the children assembledFeature to point to the new parent (new owner)
								for each ( std::string i_child in out_CADComponentData_map[model_def.componentInstanceID].children )
								{
									out_CADComponentData_map[i_child].assembledFeature.owner = renamedModelHandle;
								} 
							}
						}


						// For testing
						//isis::isis_ProMdlSave(out_CADComponentData_map[i->assemblyComponentID].modelHandle);

						// Next rename parts
						for each ( CopyModelDefinition model_def in fromModel_ToModel_FEA )
						{
							if (model_def.modelType == PRO_MDL_PART )
							{
								ModelInstanceData modelInstanceData_temp;
								modelInstanceData_temp.modelName				= model_def.fromModelName;
								modelInstanceData_temp.modelType				= model_def.modelType;
								modelInstanceData_temp.modelHandle				= static_cast<ProSolid>(out_CADComponentData_map[model_def.componentInstanceID].cADModel_hdl);
								//modelInstanceData_temp.assembledFeature			= out_CADComponentData_map[model_def.componentInstanceID].assembledFeature;		
								//modelInstanceData_temp.assembledFeature.type =	FeatureGeometryType_enum(out_CADComponentData_map[model_def.componentInstanceID].assembledFeature.type);
								//modelInstanceData_temp.assembledFeature.id   =	                         out_CADComponentData_map[model_def.componentInstanceID].assembledFeature.id;
								//modelInstanceData_temp.assembledFeature.owner =	                         out_CADComponentData_map[model_def.componentInstanceID].assembledFeature.owner; 
								modelInstanceData_temp.assembledFeature = getProAsmcomp(out_CADComponentData_map[model_def.componentInstanceID].assembledFeature);


								modelInstanceData_temp.topAssemblyModelHandle	= static_cast<ProSolid>(out_CADComponentData_map[i->assemblyComponentID].cADModel_hdl);
								modelInstanceData_temp.componentPaths			= out_CADComponentData_map[model_def.componentInstanceID].componentPaths;
								isis::MultiFormatString  CopyToPartName_temp(model_def.toModelName);

								ProMdl     renamedModelHandle;
								Assembly_RenameSubPartOrSubAssembly ( modelInstanceData_temp, CopyToPartName_temp, renamedModelHandle );
								out_CADComponentData_map[model_def.componentInstanceID].cADModel_hdl = (ProSolid)renamedModelHandle;
								out_CADComponentData_map[model_def.componentInstanceID].cADModel_ptr_ptr = (ProMdl*)&renamedModelHandle;

								// For testing
								//isis::isis_ProMdlSave(out_CADComponentData_map[i->assemblyComponentID].modelHandle);

							}						
						}
	
						// Must save this version in a scratch directory.  This is because the assemblies were changed to have
						// different part names.  If we save those assemblies in the working directory, then subsequent runs
						// in the same working directory would start with the change assemblies instead of the original assemblies.

						std::string scratchFEADir = "Analysis_Scratch";
						// FIXME replace with _mkdir(scratchFEADir.c_str());
						std::string createScratchFEADir = "if not exist " +  scratchFEADir +  " mkdir  " + scratchFEADir;
						isis::ExecuteSystemCommand( createScratchFEADir);
						
						isis::ExecuteSystemCommand( "del /q " + scratchFEADir + "\\*.*");

						isis::MultiFormatString scratchFEADir_MultiFormat(scratchFEADir, PRO_PATH_SIZE - 1);
						isis::setCreoWorkingDirectory( scratchFEADir_MultiFormat );

						isis::isis_ProMdlSave(out_CADComponentData_map[i->assemblyComponentID].cADModel_hdl);

						// Change back to the working dir
						isis::setCreoWorkingDirectory( workingDir_MultiFormat );
					}


			} // END if (CompleteTheHierarchyForLeafAssemblies && UniquelyNameAllCADModelInstances  )


			/////////////////////////////////////////////////////////////////////////////////////////////
			// Add Joint Information to the map for the entries that were added via completing heirarchy
			// This must done after filling out the hierarchy
			////////////////////////////////////////////////////////////////////////////////////////////
			if ( OutputJointInformation )
			{
				// Joint information for components in CADAssembly.xml
				// See example at http://stackoverflow.com/questions/10405030/c-unordered-map-fail-when-used-with-a-vector-as-key
				std::unordered_map<IntList, std::string, ContainerHash<IntList>> featureIDs_to_ComponentInstanceID_hashtable;
				isis::ComponentVistorBuildListOfComponentIDs  assemblyComponentIDs_IncludingTopAssembly;
				isis::VisitComponents(i->assemblyComponentID, out_CADComponentData_map, assemblyComponentIDs_IncludingTopAssembly );

				Populate_FeatureIDs_to_ComponentInstanceID_hashtable(
										assemblyComponentIDs_IncludingTopAssembly.listOfComponentIDs,
										out_CADComponentData_map,
										featureIDs_to_ComponentInstanceID_hashtable);

				if (  CompleteTheHierarchyForLeafAssemblies )  // The hierarchy would have been completed previously.
				{
					//ResolveAssemblyConstraints_AddMarkersToMap( 
					//										in_factory, 
					//										assemblyComponentIDs_IncludingTopAssembly.listOfComponentIDs,
					//										featureIDs_to_ComponentInstanceID_hashtable,
					//										out_CADComponentData_map );

					SelectComponentDerivedFromLeafAssembly selectComponentDerivedFromLeafAssembly;
					isis::ComponentVistorBuildListOfComponentIDs  assemblyComponentIDs_ExcludeTopAssembly_SelectDerivedComps(selectComponentDerivedFromLeafAssembly, true);
					isis::VisitComponents( i->assemblyComponentID, out_CADComponentData_map, assemblyComponentIDs_ExcludeTopAssembly_SelectDerivedComps );

					PopulateMap_with_Junctions_and_ConstrainedToInfo_per_CreoAsmFeatureTrees( 
															in_factory, 
															assemblyComponentIDs_ExcludeTopAssembly_SelectDerivedComps.listOfComponentIDs,
															featureIDs_to_ComponentInstanceID_hashtable,
															out_CADComponentData_map );
				}

				SelectComponentInInputXML selectComponentInInputXML;
				isis::ComponentVistorBuildListOfComponentIDs  assemblyComponentIDs_ExcludeTopAssembly_SelectInputXMLComponents(selectComponentInInputXML, true);
				isis::VisitComponents( i->assemblyComponentID, out_CADComponentData_map, assemblyComponentIDs_ExcludeTopAssembly_SelectInputXMLComponents );

				//	We must recalculate the junctions here.  The junctions were originally calculated in Apply_CADDatum_ModelConstraints;  
				//	however, that calculation was just to determine the joint type( revolute, prismatic…).  The actual position 
				//	(i.e. markers) could have been incorrect based on the Apply_CADDatum_ModelConstraints junction computations because 
				//	a kinematic assembly could have repositioned the parts based on the complete assembly.  An example is a prismatic joint
				//	defined in a leaf assembly where the markers for the joint would only be valid in the context of the complete assembly.	
				//	In other words, the prismatic joint would allow the datums defining the joints to move based on the position of 
				//	connecting parts in the assembly.
				PopulateMap_with_Junctions_per_InputXMLConstraints(in_factory, assemblyComponentIDs_ExcludeTopAssembly_SelectInputXMLComponents.listOfComponentIDs, out_CADComponentData_map, true );

				// This must be after the call to PopulateMap_with_Junctions_per_InputXMLConstraints
				PopulateMap_with_ConstrainedToInfo_per_InputXMLConstraints (	
										i->assemblyComponentID,
										assemblyComponentIDs_ExcludeTopAssembly_SelectInputXMLComponents.listOfComponentIDs,
										featureIDs_to_ComponentInstanceID_hashtable,
										out_CADComponentData_map );


				isis::ComponentVistorBuildListOfComponentIDs  assemblyComponentIDs_ExcludingTopAssembly(true);
				isis::VisitComponents(i->assemblyComponentID, out_CADComponentData_map, assemblyComponentIDs_ExcludingTopAssembly );

				//CheckValidityOfJointInformation
				if ( ValidateJointInformation ) 
				{
					std::vector<std::string> jointErrors;
					CheckValidityOfJointInformation(	assemblyComponentIDs_ExcludingTopAssembly.listOfComponentIDs, 
														out_CADComponentData_map,
														jointErrors);
					if ( jointErrors.size() > 0 )
					{   
						int errorCount = 0;

						std::stringstream errorString;			

						for each ( const std::string &err in jointErrors)
						{
							errorString << std::endl << err;
							if ( ++errorCount > 25 )
							{
								errorString << std::endl << "Stopping, exceeded 25 errors.";
								break;
							}
						}
						throw isis::application_exception(errorString.str());
					}
				}

				// This must be the last step.
			    isis::PopulateMap_with_JunctionDataInGlobalCoordinates( i->assemblyComponentID,
															 assemblyComponentIDs_ExcludingTopAssembly.listOfComponentIDs, 
															 out_CADComponentData_map );

				// Determine

			}  // END if ( OutputJointInformation )

			isis_LOG(lg, isis_FILE, isis_INFO) << "";
			isis_LOG(lg, isis_FILE, isis_INFO) << "*** BEGIN Entire Component Data Tree with Internal Addresses, Leaf Subordinates (if requested), and Joints (if requested) ****";
			std::stringstream str;
			stream_AssemblyCADComponentData( i->assemblyComponentID, out_CADComponentData_map, str );
			isis_LOG(lg, isis_FILE, isis_INFO) << str.str();
			isis_LOG(lg, isis_FILE, isis_INFO) << "";
			isis_LOG(lg, isis_FILE, isis_INFO) << "*** END Begin Entire Component Data Tree with Internal Addresses, Leaf Subordinates (if requested), and Joints (if requested) ****";

			/* Old Metrics output should not be here because it applies to all assemblies
			/////////////////////////////////////////////
			// Output Metrics File
			/////////////////////////////////////////////
			if ( regenerationSucceeded_ForAllAssemblies )
			{
				std::string MeticsOutputXML_PathAndFileName = in_XMLInputFile_PathAndFileName;	 		
					MeticsOutputXML_PathAndFileName.replace(  MeticsOutputXML_PathAndFileName.end() - 4, 
														MeticsOutputXML_PathAndFileName.end(), "_metrics.xml");  
					OutputCADMetricsToXML_Driver(	regenerationSucceeded_ForAllAssemblies,
													OutputJointInformation,
													out_CADComponentAssemblies,
													out_CADComponentData_map,  
													MeticsOutputXML_PathAndFileName,
													in_LogFile_PathAndFileName );				
			}
			else
			{
				isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) << "\nMetrics file not created because all assemblies did not regenerate.";
			}

			*/

			/////////////////////////////////////////////////
			//	Output STEP Files and Manufacturing Manifest
			////////////////////////////////////////////////	
			// For testing, un-comment out this code to Export all STEP files
			//if ( out_CADComponentAssemblies.DataExchangeSpecifications.size() == 0 )
			//{
			//	out_CADComponentAssemblies.DataExchangeSpecifications.push_back(
			//		isis::DataExchangeSpecification(isis::DATA_EXCHANGE_FORMAT_STEP, isis::AP203_SINGLE_FILE ));
			//	out_CADComponentAssemblies.DataExchangeSpecifications.push_back(
			//		isis::DataExchangeSpecification(isis::DATA_EXCHANGE_FORMAT_STEP, isis::AP203_E2_SINGLE_FILE ));
			//	out_CADComponentAssemblies.DataExchangeSpecifications.push_back(
			//		isis::DataExchangeSpecification(isis::DATA_EXCHANGE_FORMAT_STEP, isis::AP203_E2_SEPARATE_PART_FILES ));
			//	out_CADComponentAssemblies.DataExchangeSpecifications.push_back(
			//		isis::DataExchangeSpecification(isis::DATA_EXCHANGE_FORMAT_STEP, isis::AP214_SINGLE_FILE ));
			//	out_CADComponentAssemblies.DataExchangeSpecifications.push_back(
			//		isis::DataExchangeSpecification(isis::DATA_EXCHANGE_FORMAT_STEP, isis::AP214_SEPARATE_PART_FILES ));
			//}	

			bool manufacturingManifestUpdated = false; 

			// If FEA run will output data exchange files after the models are renamed for FEA purposes.
			if ( out_CADComponentAssemblies.DataExchangeSpecifications.size() > 0  )
			{
				if ( regenerationSucceeded )
				{
					isis_LOG(lg, isis_CONSOLE, isis_INFO) << "";
					isis_LOG(lg, isis_CONSOLE, isis_INFO) << isis_EOL << "Exporting STEP files, this could take several minutes...";
					isis_LOG(lg, isis_FILE, isis_INFO) << "";
					isis_LOG(lg, isis_FILE, isis_INFO) << isis_EOL << "Exporting STEP files";
					
					isis_LOG(lg, isis_FILE, isis_INFO) << "STEP file formats: ";

					for each( isis::DataExchangeSpecification de in out_CADComponentAssemblies.DataExchangeSpecifications)
					{
						isis_LOG(lg, isis_FILE, isis_INFO) << "   " << isis::DataExchangeVersion_string(de.dataExchangeVersion);
					}
					
					isis::ExportDataExchangeFiles(	 i->assemblyComponentID,
													in_WorkingDirector,
													out_CADComponentAssemblies.DataExchangeSpecifications,
													out_CADComponentData_map,
													true);
													
					/////////////////////////////////
					// manufacturing.manifest.json
					/////////////////////////////////
					if ( manufacturingManifestJsonExists && separateStepPartFilesRequested )
					{
						isis_LOG(lg, isis_FILE, isis_INFO) << "";
						isis_LOG(lg, isis_FILE, isis_INFO) << "Updating Manufacturing Manifest (manufacturing.manifest.json) for assembly: " << i->assemblyComponentID;
						isis::UpdateManufacturingManifestWithSTEPFileInfo(
														stepFileVersion_ForManufacturing, 
														i->assemblyComponentID, 
														false, // OnlyUpdateManifestForParts, Since we are now creating STEP files for sub-assemblies, we can update the manufacturing manifest for parts and assemblies.
														true, // change case of step file to lower case.  By default, when an assembly is saved, the detail part STEP files are all lower case.
														out_CADComponentData_map );
						manufacturingManifestUpdated = true;
					}
				}
				else
				{
					isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "";
					isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << "STEP Files - Did NOT export STEP file(s) because the attempts to regenerate the assembly failed.";	
				}
			}
			else
			{
				isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) << "";
				isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) << "STEP Files - Export of STEP files was not explicitly requested; however, STEP files may be exported for particular analyses (e.g. FEA).";		
			}

			/////////////////////////////////////////////
			// Log Status of Updating Manufacturing Json
			/////////////////////////////////////////////
			if ( !manufacturingManifestUpdated )
			{
				isis_LOG(lg, isis_FILE, isis_WARN) << "";
				isis_LOG(lg, isis_FILE, isis_WARN)  << "Manufacturing manifest (i.e. manufacturing.manifest.json) NOT updated for assembly: " << out_CADComponentData_map[i->assemblyComponentID].name;
				isis_LOG(lg, isis_FILE, isis_WARN) << "   For the manufacturing manifest to be updated, all three of the following must be True"; 
				if (separateStepPartFilesRequested )
					isis_LOG(lg, isis_FILE, isis_WARN) << "      Separate STEP Part Files Requested = True";
				else
					isis_LOG(lg, isis_FILE, isis_WARN) << "      Separate STEP Part Files Requested = False";

				if (regenerationSucceeded )
					isis_LOG(lg, isis_FILE, isis_WARN) << "      Assembly Regeneration Succeeded    = True";
				else
					isis_LOG(lg, isis_FILE, isis_WARN) << "      Assembly Regeneration Succeeded    = False";

				if (manufacturingManifestJsonExists )
					isis_LOG(lg, isis_FILE, isis_WARN) << "      Manufacturing Manifest Exists      = True";
				else
				{
					isis_LOG(lg, isis_FILE, isis_WARN) << "      Manufacturing Manifest Exists      = False";
					isis_LOG(lg, isis_FILE, isis_WARN) <<  isis::CouldNotFindManufacturingManifestError;
				}
			}

			/////////////////
			// CFD Run
			////////////////
			if ( cFDAnalysisRun) {
				CFD_Driver( isis::V1, in_CADExtensionsDir, 
					in_WorkingDirector, *i, out_CADComponentData_map );
			}

			///////////////////////
			//	FEA Run
			///////////////////////
			if ( regenerationSucceeded && ( fEAAnalysisRun  )) 
			{

				std::map<std::string, std::string>  componentID_to_MaterialName_map;		 
				isis::ComponentVistorBuildListOfComponentIDs  assemblyComponentIDs_IncludingTopAssembly;
				isis::VisitComponents(i->assemblyComponentID, out_CADComponentData_map, assemblyComponentIDs_IncludingTopAssembly );
				isis::RetrieveCreoMaterialsNames(	assemblyComponentIDs_IncludingTopAssembly.listOfComponentIDs,
													out_CADComponentData_map,
													componentID_to_MaterialName_map) ;

				//for each ( std::string i_temp in assembllyComponenteIDs.listOfComponentIDs ) 
				//{
				//	std::cout << std::endl << "Material Info, Component Instance ID: " << i_temp <<
				//		"  Component Material: " << out_CADComponentData_map[i_temp].materialID_FromCADPart <<
				//		"  Retrieved Material: " << componentID_to_MaterialName_map[i_temp];
				//}

				bool materialMisMatchFound = false;
				std::set<std::string> materialNames;
				for each ( std::pair<std::string, std::string> mat_pair in componentID_to_MaterialName_map ) 
				{
					if ( out_CADComponentData_map[mat_pair.first].modelType == PRO_MDL_PART )
					{
						out_CADComponentData_map[mat_pair.first].materialID_FromCADPart = mat_pair.second;
						materialNames.insert(mat_pair.second);

						if ( isis::ConvertToUpperCase(out_CADComponentData_map[mat_pair.first].materialID_FromCADPart) !=
							isis::ConvertToUpperCase(out_CADComponentData_map[mat_pair.first].materialID_FromCyPhy) )
						{
							materialMisMatchFound = true;
							std::stringstream errorString;
							errorString << std::endl << std::endl << "WARNING - Material names (CyPhy/Creo) differ: " << std::endl << 
								"  Component Instance ID: " <<  out_CADComponentData_map[mat_pair.first].componentID << std::endl << 
								"  Component Name:        " <<  out_CADComponentData_map[mat_pair.first].name << std::endl << 
								"  Component Type:        " <<  ProMdlType_string(out_CADComponentData_map[mat_pair.first].modelType) <<  std::endl << 
								"  CyPhy Material Name:   " <<  out_CADComponentData_map[mat_pair.first].materialID_FromCyPhy  <<  std::endl << 
								"  Creo Material Name:    " <<  out_CADComponentData_map[mat_pair.first].materialID_FromCADPart  <<  std::endl << 
								"  Using Material Name:   " <<  out_CADComponentData_map[mat_pair.first].materialID_FromCADPart << std::endl;

							isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << errorString.str();

						}
					}
				}
				if ( materialMisMatchFound ) 
					isis_LOG(lg, isis_FILE, isis_WARN) << "Material name(s) in CyPhy differ from material name(s) in the Creo Part(s). See " << 
					in_LogFile_PathAndFileName << " in the log directory for a listing of the differences.  Search on WARNING.";

				//for each ( std::string i_temp in assembllyComponenteIDs.listOfComponentIDs ) 
				//{
				//	std::cout << std::endl << "Material Info, Component Instance ID: " << i_temp <<
				//		"  Component Material: " << out_CADComponentData_map[i_temp].materialID_FromCADPart <<
				//		"  Retrieved Material: " << componentID_to_MaterialName_map[i_temp];
				//}			
	
				bool materialLibLookup_ErrorsOccured = false;

				try
				{
					isis::ReadMaterialsLibrary( MaterialsLibrary_PathAndFileName(), materialNames, out_CADComponentAssemblies.materials);

					// Log materials
					isis_LOG(lg, isis_FILE, isis_INFO) << "Materials: ";
					for each ( std::pair<std::string, isis::Material> i in out_CADComponentAssemblies.materials )
					{
						isis_LOG(lg, isis_FILE, isis_INFO) << i.second.analysisMaterialProperties;
					}
				}
				catch (isis::application_exception &ex )
				{
					materialLibLookup_ErrorsOccured = true;
					if ( fEAAnalysisAbaqusModelBasedRun )
					{
						isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "\n\nWARNING: Material lookup anomalies.  This may/may-not cause problems with Abaqus model-based FEA. ";
						isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "         If a lack of material information causes a problem with FEA, the FEA job will fail and the FEA log will explain the error condition.";
						isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "         " << ex.what();
					}
					else
					{
						// For all cases fEAAnalysisAbaqusModelBasedRun this error should be considered fatal
						throw;
					}
				}
				

				if ( fEAAnalysisDeckBasedRun )
				{
					// This writes to ComputedValues.xml, which is in the .\Analysis\Abaqus directory
					isis::Create_FEADecks_BatFiles( *i,   // TopLevelAssemblyData	
												out_CADComponentAssemblies.materials, 
												in_WorkingDirector,
												in_ProgramName_Version_TimeStamp,
												out_CADComponentData_map );
				}
				else
				{   // Abaqus Model Based, Patran_Nastran
					// For Abaqus Model Based, only need to create AnalysisMetaData.xml

					// Create Analysis Directories

					if ( !materialLibLookup_ErrorsOccured )
					{
						try
						{
						std::string createAnalysisDir = "if not exist Analysis mkdir Analysis";
						// FIXME replace with _mkdir(createAnalysisDir.c_str());
						isis::ExecuteSystemCommand( createAnalysisDir);
						std::string analysisMetaData_PathAndFileName = ".\\Analysis\\AnalysisMetaData.xml";
					
						// Empty nastranMaterialID_to_CompnentID_map is an allowable argument to CreateXMLFile_FEA_AnalysisMetaData
						std::map<std::string, std::string>        nastranMaterialID_to_CompnentID_map;
						CreateXMLFile_FEA_AnalysisMetaData( analysisMetaData_PathAndFileName,
															*i,
															out_CADComponentAssemblies.materials,
															nastranMaterialID_to_CompnentID_map,
															out_CADComponentData_map );
						}
						catch (isis::application_exception &ex )
						{

							isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "\n\nWARNING: Material lookup anomalies.  This may/may-not cause problems with Abaqus model-based FEA. ";
							isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "         If a lack of material information causes a problem with FEA, the FEA job will fail and the FEA log will explain the error condition.";
							isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "         " << ex.what();
						}

					}
					else
					{
						isis_LOG(lg, isis_FILE, isis_INFO) << "\nFile AnalysisMetaData.xml not created because there were errors reading all the material information from them material library";
					}
				}

			}  // END if ( regenerationSucceeded && ( fEAAnalysisRun  )) 
			

			if ( ballisticAnalysisRun )
			{
				// This does not write to ComputedValues.xml
				// The reference plane is written to a json file
				isis::PopulateBallisticFiles( *i, in_WorkingDirector, out_CADComponentData_map);
			}

			if ( blastAnalysisRun )
			{
				// This does not write to ComputedValues.xml
				// The reference plane is written to a json file
				isis::PopulateBlastFiles( *i, in_WorkingDirector, out_CADComponentData_map);
			}

			// This is not needed because the only file needed for CFD in the ComputedValues.xml
			// is the reference plane, which is created by CreateXMLFile_ComputedValues below
			//if ( cFDAnalysisRun )
			//{
			//	isis::PopulateCFDFiles( *i, in_WorkingDirector, out_CADComponentData_map);
			//}

			// WARNING - Do not save the assembly/models after this point.  Doing so will save the temporarily created material.


			///////////////////////////////////
			//	Output Interference Report
			//////////////////////////////////	
	
			std::string InterferenceReport_PathAndFileName = in_XMLInputFile_PathAndFileName;	 		
			InterferenceReport_PathAndFileName.replace(  InterferenceReport_PathAndFileName.end() - 4, 
														InterferenceReport_PathAndFileName.end(), "_interference.txt");  
			// Delete InterferenceReport_PathAndFileName if it exists.  This will assure that an old interference file would not be used. 
			::boost::filesystem::path deleteFile = InterferenceReport_PathAndFileName;
			if (::boost::filesystem::exists(deleteFile) ) ::boost::filesystem::remove(deleteFile);	

			if ( interferenceRun )
			{
				std::vector<CADComputation>		interferenceCount_CADComputations;

				RetrieveComputationOfAGivenType(	i->assemblyMetrics, 
													COMPUTATION_INTERFERENCE_COUNT,
													interferenceCount_CADComputations );

				// At this point there should be one and only one CADComputation of type COMPUTATION_INTERFERENCE_COUNT.
				// This was enforced via the function Validate_ComputationInterferenceCount_ThrowExceptionIfInvalid.
				// Check again to prevent a bug from being introduced.
				if ( interferenceCount_CADComputations.size() != 1 )
				{
					std::stringstream errorString;
					errorString << "Function - " << __FUNCTION__ << ", " << std::endl <<
								"For an interference analysis, there must be one and only one InterferenceCount CADComputationComponent." << std::endl <<
								"Number of InterferenceCount Found: " << interferenceCount_CADComputations.size() << std::endl <<
								"InterferenceCount CADComputationComponents: " << std::endl;
								for each ( const CADComputation &k in interferenceCount_CADComputations )
								{
									errorString << "Referenced Model Name:        " << out_CADComponentData_map[k.componentID].name << std::endl;;
									errorString << "Referenced Model ComponentID: " << k.componentID;
									errorString << k;
								}
							throw isis::application_exception(errorString);		
				}

				if ( regenerationSucceeded_ForAllAssemblies && 
					 out_CADComponentAssemblies.unassembledComponents.size() == 0 &&  
					 out_CADComponentAssemblies.topLevelAssemblies.size() == 1)
				{			
					isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "";
					isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "Creating Interference File";	

					isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "   Created:    " + InterferenceReport_PathAndFileName;
					isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "   Populating: " + InterferenceReport_PathAndFileName + ", Note: For large assemblies, this could take several minutes.";
					try 
					{
						CreateInterferenceReport( InterferenceReport_PathAndFileName, i->assemblyComponentID, interferenceCount_CADComputations[0], out_CADComponentData_map );
					}

					catch ( isis::application_exception& ex )
					{
						isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "\nError occurred when computing interferences: " << ex.what();
						isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "\nContinue processing...";
					}
					catch ( std::exception& ex )
					{
						isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "\nError occurred when computing interferences: " << ex.what();
						isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "\nContinue processing...";
					}
					catch ( ... )
					{
						isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "\nError occurred when computing interferences: ";
						isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "\nContinue processing...";
					}
				}
				else
				{
					if ( !regenerationSucceeded_ForAllAssemblies )
					{
						isis_LOG(lg, isis_FILE, isis_WARN) << "";
						isis_LOG(lg, isis_FILE, isis_WARN)  << "Interference Report NOT created because all assemblies could not be regenerated.";
					}
					else
					{
						if ( out_CADComponentAssemblies.unassembledComponents.size() != 0 )
						{
							// Unassembled Components
							isis_LOG(lg, isis_FILE, isis_WARN) << "";
							isis_LOG(lg, isis_FILE, isis_WARN)  << "Interference Report NOT created because there were unassembled components.";
						}
						else
						{
							// More than one assembly
							isis_LOG(lg, isis_FILE, isis_WARN) << "";
							isis_LOG(lg, isis_FILE, isis_WARN)  << "Interference Report NOT created because there was more than one assembly defined in the input xml.";

						}
					}
				}  // END if ( regenerationSucceeded_ForAllAssemblies && out_CADComponentAssemblies.unassembledComponents.size() == 0 &&  out_CADComponentAssemblies.topLevelAssemblies.size() == 1)

			}  // END if ( interferenceRun )

		} // for ( std::list<isis::TopLevelAssemblyData>::const_iterator i(out_CADComponentAssemblies.topLevelAssemblies.begin());

		/////////////////////////////////////////////
		// Output Metrics File 
		/////////////////////////////////////////////
		// Metrics Files Should be outputted after all assemblies are built.
		// The functions that follow apply to all assemblies.
		std::string MeticsOutputXML_PathAndFileName = in_XMLInputFile_PathAndFileName;	 		
		MeticsOutputXML_PathAndFileName.replace(  MeticsOutputXML_PathAndFileName.end() - 4, 
													MeticsOutputXML_PathAndFileName.end(), "_metrics.xml"); 
		// Delete old metrics file if it exists.  This should be done so that an old metrics file would not be inadvertently used if a
		// new metrics file was not created.
		isis::IfFileExists_DeleteFile( MeticsOutputXML_PathAndFileName);
		if ( regenerationSucceeded_ForAllAssemblies && out_CADComponentAssemblies.unassembledComponents.size() == 0 )
		{
			for each ( const TopLevelAssemblyData &i_tl in out_CADComponentAssemblies.topLevelAssemblies)
			{
				isis::ComponentVistorBuildListOfComponentIDs  assemblyComponentIDs_IncludingTopAssembly;
				isis::VisitComponents(i_tl.assemblyComponentID, out_CADComponentData_map, assemblyComponentIDs_IncludingTopAssembly );

				// Add Bounding Box Values to Map
				AddBoundingBoxValuesToMap( assemblyComponentIDs_IncludingTopAssembly.listOfComponentIDs, out_CADComponentData_map );
				// Add Mass Property Values to Map
				AddMassPropertyValuesToMap( assemblyComponentIDs_IncludingTopAssembly.listOfComponentIDs, out_CADComponentData_map );
			}

			// The following function iterates through each top-assembly and recursively
			// finds the children.  This was necessary because the MetricID that is persisted in the metrics
			// file must must be unique across all assemblies.  At some point, this function should be changed
			// to take a list of ComponentInstanceIDs and operate on only that list.  This would supersede the 
			// approach of recursively finding the children and would be more compatible with the approach in
			// other areas of this program.
			OutputCADMetricsToXML_Driver(	regenerationSucceeded_ForAllAssemblies,
											OutputJointInformation,
											out_CADComponentAssemblies,
											out_CADComponentData_map,  
											MeticsOutputXML_PathAndFileName,
											in_LogFile_PathAndFileName );				
		}
		else
		{
			if (  !regenerationSucceeded_ForAllAssemblies )
				isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) << "\nMetrics file not created because all assemblies did not regenerate.";
			else
				isis_LOG(lg, isis_CONSOLE_FILE, isis_WARN) << "\nMetrics file not created because there were unassembled components.";
		}
														
		///////////////////////////////////
		//	Output Requested Computations
		//////////////////////////////////

		if ( regenerationSucceeded_ForAllAssemblies )
		{
			ComputationTypes	computationTypes(fEAAnalysisDeckBasedRun, cFDAnalysisRun,  ballisticAnalysisRun, blastAnalysisRun, hasAssemblyBasedComputations);	
		    
			CreateXMLFile_ComputedValues(	in_WorkingDirector,
											computationTypes,
											out_CADComponentAssemblies,
											out_CADComponentData_map );
		}
		else
		{
			isis_LOG(lg, isis_FILE, isis_WARN) << "ComputedValues File - Did NOT create a ComputedValues file because the attempts to regenerate the assembly failed.";	
		}
		
		////////////////////////////////////////////
		// Output Step Files for Unassembled Models
		////////////////////////////////////////////
		bool manufacturingManifestUpdated = false;
		if ( out_CADComponentAssemblies.unassembledComponents.size() > 0 )
		{
			isis_LOG(lg, isis_CONSOLE, isis_INFO) << "";
			isis_LOG(lg, isis_CONSOLE, isis_INFO)  << "Exporting STEP files for unassembled models, this could take several minutes...";
			isis_LOG(lg, isis_FILE, isis_INFO) << "";
			isis_LOG(lg, isis_FILE, isis_INFO)  << "Exporting STEP files for unassembled models";

			bool logCouldNotFindManifestError = true;

			for ( std::list<isis::UnassembledComponent>::const_iterator unusedComp( out_CADComponentAssemblies.unassembledComponents.begin());
				  unusedComp != out_CADComponentAssemblies.unassembledComponents.end(); ++unusedComp )
			{

				//std::cout << std::endl << "Exporting STEP file for " + unusedComp->name + ", this could take several seconds...";
				isis::ExportDataExchangeFiles(	unusedComp->componentID,
												unusedComp->name,
												ProMdlType_enum(unusedComp->modelType),
												unusedComp->geometryRepresentation,
												in_WorkingDirector,
												out_CADComponentAssemblies.DataExchangeSpecifications, true );
				////////////////////////////////////////////////////////
				// Write STEP file names to manufacturing.manifest.json
				////////////////////////////////////////////////////////
				if ( manufacturingManifestJsonExists && separateStepPartFilesRequested )
				{
					isis::UpdateManufacturingManifestWithSTEPFileInfo(
												stepFileVersion_ForManufacturing, 
												unusedComp->componentID,
												false, // Need to update the manifest for parts and assmeblies
												false, // The case should be the same as entered in CyPhy and the input xml
												out_CADComponentData_map );
					isis_LOG(lg, isis_FILE, isis_INFO) << "";
					isis_LOG(lg, isis_FILE, isis_INFO)  << "Updating Manufacturing Manifest (manufacturing.manifest.json) for unassembled model: " << unusedComp->name;
					manufacturingManifestUpdated = true; 
				}
			}
		}	

		/////////////////////////////////////////////
		// Log Status of Updating Manufacturing Json
		/////////////////////////////////////////////
		if ( !manufacturingManifestUpdated  && out_CADComponentAssemblies.unassembledComponents.size() > 0 )
		{
			isis_LOG(lg, isis_FILE, isis_WARN) << "";
			isis_LOG(lg, isis_FILE, isis_WARN)  << "Manufacturing manifest (i.e. manufacturing.manifest.json) NOT updated for unassembled parts/sub-assemblies";
			isis_LOG(lg, isis_FILE, isis_WARN) << "   For the manufacturing manifest to be updated, both of the following must be True"; 
			if (separateStepPartFilesRequested )
				isis_LOG(lg, isis_FILE, isis_WARN) << "      Separate STEP Part Files Requested = True";
			else
				isis_LOG(lg, isis_FILE, isis_WARN) << "      Separate STEP Part Files Requested = False";

			if (manufacturingManifestJsonExists )
				isis_LOG(lg, isis_FILE, isis_WARN) << "      Manufacturing Manifest Exists      = True";
			else
			{
				isis_LOG(lg, isis_FILE, isis_WARN) << "      Manufacturing Manifest Exists      = False";
				isis_LOG(lg, isis_FILE, isis_WARN) <<  isis::CouldNotFindManufacturingManifestError;
			}
		}			

		if ( !regenerationSucceeded_ForAllAssemblies )
		{
			std::stringstream errorString;
				errorString <<
						"At least one assembly would not regenerate.  See the log file to locate the assembly/part that would not regenerate."
						<< std::endl << "Log File: " <<  in_LogFile_PathAndFileName;
				throw isis::application_exception(errorString.str());
		}


	} // END Try


	catch ( isis::application_exception& ex )
	{
		throw;
	}
	catch ( std::exception& ex )
	{
		throw;
	}
	catch ( ... )
	{
		std::stringstream	exceptionErrorStringStream;
		exceptionErrorStringStream << " std::exception: Caught exception (...).  Please report the error to the help desk.";
		throw isis::application_exception(exceptionErrorStringStream.str().c_str());
	}

}

} // END namespace isis