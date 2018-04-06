#include "stdafx.h"
#include <AssemblyCreationViaInputFile.h>

#include <BuildAssembly.h>
#include <DiagnosticUtilities.h>
#include <cc_XMLtoCADStructures.h>
#include <cc_CommonUtilities.h>
#include <cc_Metrics.h>
#include <cc_DataExchange.h>
#include <cc_MultiFormatString.h>
#include <CommonFunctions.h>    // Work on this one
#include <SurvivabilityAnalysis.h>
#include <CFDAnalysis.h>
#include <MaterialProperties.h>
#include <sstream>
#include <fstream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp> 
#include "cc_CommonFunctions.h"
#include <cc_AssemblyUtilities.h>

// #define DEBUG_BREAKER 1

namespace isis
{


void CreateAssemblyViaInputFile( //cad::CadFactoryAbstract						&in_Factory,
								 const std::string								&in_CreateAssemblyProgramName,
								 const std::string								&in_CreateAssemblyProgramVersion,
								 const std::string								&in_CADApplicationName,  
								 const isis::CreateAssemblyInputArguments       &in_ProgramInputArguments,
								 const std::string								&in_CADExtensionsDir,
								 const std::string								&in_templateFile_PathAndFileName,
								 const std::string								&in_CreoStartCommand,
								 const std::string								&in_ProgramName_Version_TimeStamp,
								 unsigned int									in_MaxCADModelNameLength,   
								 unsigned int									in_MaxCADPathLength,
								 bool											&out_Pro_E_Running )
																					throw (isis::application_exception)
{
#ifdef DEBUG_BREAKER
	bool breaker = true;
	while( breaker ) {
		boost::this_thread::sleep(boost::posix_time::seconds(2));
	}
#endif

	isis::cad::CadFactoryAbstract_global *cadFactoryAbstract_global_ptr = isis::cad::CadFactoryAbstract_global::instance();
	isis::cad::CadFactoryAbstract::ptr	                cAD_Factory_ptr = cadFactoryAbstract_global_ptr->getCadFactoryAbstract_ptr();
	isis::cad::IModelOperations& modelOperations = cAD_Factory_ptr->getModelOperations();

	unsigned int uniqueNameIndex = 1;
	std::string xMLInputFile_PathAndFileName = in_ProgramInputArguments.inputXmlFileName;

	// Must get the complete path to the working directory.  This is necessary because when
	// isis_ProDirectoryChange is called to change to a STEP directory, workingDir must be fully
	// defined so that isis_ProDirectoryChange can be called to return to the original directory.
	std::string workingDir_string = in_ProgramInputArguments.workingDirectory;
	::boost::filesystem::path    workingDir_path;
	workingDir_path = isis::SetupWorkingDirectory( workingDir_string );
	std::string workingDirector = workingDir_path.generic_string();

	std::string logFile_PathAndFileName = in_ProgramInputArguments.logFileName;

	isis::CADAssemblies cADComponentAssemblies;

	std::map<std::string, isis::CADComponentData> cADComponentData_map;

	//bool Pro_E_Running = false;
	bool Template_Copied = false;
	int NonCyPhyID_counter = 0;  // This will be incremented before it is used.
	int ExitCode = 0;
	try
	{
		time_t time_start; /* calendar time */
		time_start=time(NULL); /* get current cal time */

		bool regenerationSucceeded_ForAllAssemblies = true;
		std::string cadPartsLibDir = in_ProgramInputArguments.auxiliaryCADDirectory;

		// Verify that the XML file exists.
		if (!isis::FileExists( xMLInputFile_PathAndFileName.c_str()))
		{
			std::string TempError = "Could not find the assembly definition xml file, file name: " + xMLInputFile_PathAndFileName;
			throw isis::application_exception(TempError.c_str());
		} 


		std::vector<CADCreateAssemblyError> errorList;

		//Initial load of the map, one entry for each part/assembly file.
	
		FromXMLFile_PopulateCADComponentAssemblyAndMap(
													XML_DEFINED_BY_FILE,
													xMLInputFile_PathAndFileName,
													cADComponentAssemblies,
													cADComponentData_map,
													errorList);

		if ( cADComponentAssemblies.topLevelAssemblies.size() == 0 ) 
		{
			isis_LOG(lg, isis_FILE, isis_INFO) << "";
			isis_LOG(lg, isis_FILE, isis_INFO) << "*************************** Begin Assembly Creation **************************";
			std::stringstream informationString;
			informationString << "No assemblies were created because the input xml file, " 
					  << std::endl << xMLInputFile_PathAndFileName 
					  << ", did not define any assemblies.";
			isis_LOG(lg, isis_FILE, isis_WARN) << informationString.str();

			if ( cADComponentAssemblies.unassembledComponents.size() == 0 )
			{
				std::stringstream errorString;
				errorString <<
						"The input xml file, "
						 << std::endl << xMLInputFile_PathAndFileName 
						 << ", did not define any assemblies nor did it define unassembled parts/sub-assemblies.  "
						 << std::endl << "The input xml file must specify assemblies and/or unassembled parts/sub-assemblies."; 
						throw isis::application_exception(errorString.str());
			}
			isis_LOG(lg, isis_FILE, isis_INFO) << "************************** End Assembly Creation *****************************";
		}

		///////////////////////////////////
		// Output Requested Metrics file
		///////////////////////////////////
		std::string requestedMetricFile_PathAndFileName =  workingDirector + "\\RequestedMetrics.xml";
		isis::IfFileExists_DeleteFile( requestedMetricFile_PathAndFileName);
		isis::CreateXMLFile_RequestedMetrics(	requestedMetricFile_PathAndFileName,
												cADComponentAssemblies.topLevelAssemblies,
												cADComponentData_map );


		//////////////////////////////////////////////////
		// Modify Parametric Models to Have a Unique Name
		//////////////////////////////////////////////////
		std::vector<CopyModelDefinition>			fromModel_ToModel;


		isis::BuildListOfCADModels_ThatShouldBeCopiedToNewNames(		//*cAD_Factory_ptr,
																	uniqueNameIndex, 
																	e_PART_OR_ASSEMBLY_MODEL_TYPE,
																	e_SELECT_ONLY_PARAMETRIC_MODELS,
																	true, 
																	in_MaxCADModelNameLength,
																	cADComponentData_map, 
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
		// Note - We are assuming if any assembly (cADComponentAssemblies) is an analysis run then all 
		// assemblies will be for analaysis purposes.  We will not mix building assembies for analsys purposes
		// and building assembies for general purposes.
		
		bool fEAAnalysisDeckBasedRun		= IsAFEAAnlysisDeckBasedRun(cADComponentAssemblies);
		bool fEAAnalysisAbaqusModelBasedRun	= IsFEAAnalysisAbaqusModelBasedRun(cADComponentAssemblies);
		bool fEAAnalysisRun = false;
		if ( fEAAnalysisDeckBasedRun  || fEAAnalysisAbaqusModelBasedRun ) fEAAnalysisRun = true;
		bool ballisticAnalysisRun				= IsABallisticAnalysisRun(cADComponentAssemblies);
		bool blastAnalysisRun					= IsABlastAnlysisRun(cADComponentAssemblies);
		bool cFDAnalysisRun						= IsACFDAnalysisRun(cADComponentAssemblies);
		bool interferenceRun					= IsAInterferenceRun(cADComponentAssemblies);
		bool hasAssemblyBasedComputations		= HasAssemblyBasedComputations( cADComponentAssemblies);
		bool fillOutMetricsFileForLeafAssemblies = FillOutMetricsFileForLeafAssemblies( cADComponentAssemblies.dataExchangeSpecialInstruction);
		
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
				CompleteTheHierarchyForLeafAssemblies = Get_CompleteTheHierarchyForLeafAssemblies (cADComponentAssemblies);

		if ( !UniquelyNameAllCADModelInstances ) 
				UniquelyNameAllCADModelInstances = Get_UniquelyNameAllCADModelInstances (cADComponentAssemblies);

		if ( !OutputJointInformation ) 
				OutputJointInformation = Get_OutputJointInformation (cADComponentAssemblies);

		if ( OutputJointInformation )
				ValidateJointInformation = Get_ValidateJointInformation (cADComponentAssemblies);

		if ( fEAAnalysisRun || ballisticAnalysisRun  || blastAnalysisRun || cFDAnalysisRun )
		{
			/////////////////////////////////////////////////////
			// Verify One and only One Assembly per analysis run
			/////////////////////////////////////////////////////
			if ( cADComponentAssemblies.topLevelAssemblies.size() != 1 )
			{
				std::stringstream errorString;
				errorString << "One and only one assembly allowed per analysis run (FEA, Ballistic, Blast, or  CFD).  The XML file defines " <<
				cADComponentAssemblies.topLevelAssemblies.size() << std::string(" assemblies.");
					throw isis::application_exception(errorString.str());
			}

			if ( cADComponentAssemblies.unassembledComponents.size() != 0 )
			{
				std::stringstream errorString;
				errorString << "Unassembled components were found.  Cannot perform analysis (FEA, Ballistic, Blast, or  CFD) on partial assemblies.  Number of unassembled components: " <<
				cADComponentAssemblies.unassembledComponents.size();
					throw isis::application_exception(errorString.str());
			}
		}

		if ( interferenceRun ) 
			Validate_ComputationInterferenceCount_ThrowExceptionIfInvalid ( cADComponentAssemblies, cADComponentData_map);
										

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
			isis::BuildListOfCADModels_ThatShouldBeCopiedToNewNames(		//*cAD_Factory_ptr,
																		uniqueNameIndex, 
																		e_PART_OR_ASSEMBLY_MODEL_TYPE,
																		e_SELECT_ALL_MODELS,
																		false,  
																		in_MaxCADModelNameLength,
																		cADComponentData_map, 
																		fromModel_ToModel );
			////////////////////////////
			// Log Part Copy/Renames
			///////////////////////////
			isis_LOG(lg, isis_FILE, isis_INFO) << "";
			isis_LOG(lg, isis_FILE, isis_INFO) << "****************** if ( UniquelyNameAllCADModelInstances ) **********************";
			isis_LOG(lg, isis_FILE, isis_INFO) << "************** Begin Modified Part Names for Analysis Purposes *****************";
			isis_LOG(lg, isis_FILE, isis_INFO) << "From_Part_Name   To_Part_Name";
			isis_LOG(lg, isis_FILE, isis_INFO) << fromModel_ToModel;
			isis_LOG(lg, isis_FILE, isis_INFO) << "";
			isis_LOG(lg, isis_FILE, isis_INFO) << "************** End Modified Part Names for Analysis Purposes *****************";
		}
		

		// Add the depends-on information to the CADComponentData
		isis::Add_DependsOn( cADComponentData_map );

		//stream_CADComponentData_map(cADComponentData_map, clog );
		//stream_CADComponentData_map(cADComponentData_map, cout );

		//for ( std::list<isis::TopLevelAssemblyData>::const_iterator i(cADComponentAssemblies.topLevelAssemblies.begin());
		//	  i != cADComponentAssemblies.topLevelAssemblies.end();
		//	  ++i )
		for each( isis::TopLevelAssemblyData i in cADComponentAssemblies.topLevelAssemblies)
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
			stream_AssemblyCADComponentData( i.assemblyComponentID, cADComponentData_map, str);
			isis_LOG(lg, isis_FILE, isis_INFO) << str.str();
			isis_LOG(lg, isis_FILE, isis_INFO) << "";
			isis_LOG(lg, isis_FILE, isis_INFO) << "************** Begin Entire Component Data Tree (CAD Internal Structures) *****************";
			isis_LOG(lg, isis_FILE, isis_INFO) << "";
			isis_LOG(lg, isis_FILE, isis_INFO) << "************** End Entire Tree For a Single Assembly  *****************";
		}

		///////////////////////////////////////////////////////////////////////////////////
		// Check for Non-Size-To-Fit components being constrained to Size-To-Fit components
		////////////////////////////////////////////////////////////////////////////////////
		std::set<std::string> TempIntersection = NonSizeToFitComponents_ReferencedBy_SizeToFitConstraints( cADComponentData_map );
		if ( TempIntersection.size() > 0 )
		{
			std::string err_str = "Erroneous XML File: A NON_SIZE_TO_FIT component cannot be constrained to a SIZE_TO_FIT component. " +
				std::string("The SIZE_TO_FIT component(s) (i.e. ComponentIDs) that were erroneously referenced are:");
			for ( std::set<std::string>::const_iterator i(TempIntersection.begin()); i != TempIntersection.end(); ++i )
			{
				err_str += " " + *i;
			}
			err_str += ".  Search for the ComponentIDs in the log file to identify the places where the SIZE_TO_FIT components are erroneously referenced.";
			throw isis::application_exception(err_str.c_str());  

		}

		
		/////////////////////////////
		/////// Start CAD Program ///
		/////////////////////////////
		isis::cad::ICADSession&            cADsession = cAD_Factory_ptr->getCADSession();

		isis_LOG(lg, isis_CONSOLE, isis_INFO)  << in_CreateAssemblyProgramName << " "<< in_CreateAssemblyProgramVersion;
		isis_LOG(lg, isis_CONSOLE, isis_INFO)  << "Starting " <<  in_CADApplicationName << ", this takes about 10 seconds...";

		//char tempBuffer[1024];
		//strcpy(tempBuffer, in_CreoStartCommand.c_str() );
		//isis::isis_ProEngineerStart(tempBuffer,"");
		cADsession.startCADProgram(in_CreoStartCommand);


		//int creoVersionNumber;
		//isis_ProEngineerReleaseNumericversionGet(&creoVersionNumber);

	    bool	intVersionNumber_Set = false;
		int		intVersionNumber;
		bool	floatVersionNumber_Set = false;
		double	floatVersionNumber;
				
		// The CAD session must be started before calling the following command.
		cADsession.getCADProgramVersion(intVersionNumber_Set, intVersionNumber, floatVersionNumber_Set, floatVersionNumber );

		if ( intVersionNumber_Set )	  isis_LOG(lg, isis_FILE, isis_INFO) << in_CADApplicationName << " Version Number: " <<  intVersionNumber;
		if ( floatVersionNumber_Set ) isis_LOG(lg, isis_FILE, isis_INFO) << in_CADApplicationName << " Version Number: " <<  floatVersionNumber;

		out_Pro_E_Running = true;
		std::cout <<  std::endl << in_CADApplicationName << " successfully started." << std::endl;

		//ProPath  WorkingDirPath_wchar;
		//ProStringToWstring(WorkingDirPath_wchar, (char *)workingDir.c_str() );

		
		//isis::MultiFormatString workingDir_MultiFormat(workingDirector, PRO_PATH_SIZE - 1);
		isis::MultiFormatString workingDir_MultiFormat(workingDirector, in_MaxCADPathLength);

		//isis::setCreoWorkingDirectory( workingDir_MultiFormat );
		cADsession.setCADWorkingDirectory(workingDir_MultiFormat);
		isis_LOG(lg, isis_FILE, isis_INFO)	<< "setCADWorkingDirectory: " << workingDir_MultiFormat;

		// Copy template model to the working directory
		isis::CopyFileIsis( in_templateFile_PathAndFileName,  workingDirector );
		Template_Copied = true;

		// *** If model renames occured
		// WARNING - isis::CopyModels must be called after 
		//			 1) Creo has started
		//			 2) isis_ProDirectoryChange changed to the working directory
		//           been created.
		//			 3) the search_META.pro has been set 
		//if ( ToPartName_FromPartName_map.size() > 0 ) isis::CopyModels(ToPartName_FromPartName_map);
		if ( fromModel_ToModel.size() > 0 ) isis::CopyModels(//*cAD_Factory_ptr, 
															 fromModel_ToModel);

		////////////////////////////////////////////////////////////////////////////
		// Setup for writing STEP file informatoin to \\manufacturing.manifest.json
		////////////////////////////////////////////////////////////////////////////
		bool manufacturingManifestJsonExists = isis::FileExists( isis::manufacturingManifestJson_PathAndFileName.c_str());
		bool separateStepPartFilesRequested = false;
		isis::e_DataExchangeVersion  stepFileVersion_ForManufacturing;

		if ( isis::DataExchange_Format_Version_InList(	isis::DATA_EXCHANGE_FORMAT_STEP,  
														isis::AP203_E2_SEPARATE_PART_FILES, 
														cADComponentAssemblies.DataExchangeSpecifications) )
		{
			stepFileVersion_ForManufacturing = isis::AP203_E2_SEPARATE_PART_FILES;
			separateStepPartFilesRequested = true;
		}
		else
		{
			if ( isis::DataExchange_Format_Version_InList(	isis::DATA_EXCHANGE_FORMAT_STEP,  
															isis::AP214_SEPARATE_PART_FILES, 
															cADComponentAssemblies.DataExchangeSpecifications) )
			{
				stepFileVersion_ForManufacturing = isis::AP214_SEPARATE_PART_FILES;
				separateStepPartFilesRequested = true;
			}
		}

		////////////////////////////////////////
		// Build the assemblies
		////////////////////////////////////////
		for ( std::list<isis::TopLevelAssemblyData>::const_iterator i(cADComponentAssemblies.topLevelAssemblies.begin());
			  i != cADComponentAssemblies.topLevelAssemblies.end();
			  ++i )
		{
			bool regenerationSucceeded;

			//////////////////////////
			// Build the Assembly
			//////////////////////////
			isis::BuildAssembly(		//*cAD_Factory_ptr, 
									i->assemblyComponentID, 
									workingDirector, 
									true, 
									cADComponentData_map, 
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
			isis::VisitComponents(i->assemblyComponentID, cADComponentData_map, componentVistorCountAssemblyComponents);

			// Log component count
			isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "   Number of assembled components: " << componentVistorCountAssemblyComponents.numberOfComponents;
			isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "   Elapsed wall-clock time:        " << time_elapsed << " seconds";

			time_start=time(NULL); // reset start time for subsequent assemblies if any




			///////////////////////////////////////////////
			// Complete The Hierarchy For Leaf Assemblies
			//////////////////////////////////////////////
			if (CompleteTheHierarchyForLeafAssemblies )
			{
				isis_LOG(lg, isis_FILE, isis_INFO) << "";
				isis_LOG(lg, isis_FILE, isis_INFO) << "************** Begin CompleteTheHierarchyForLeafAssemblies *****************";

				//////////////////////////////////////////////
				// Complete The Hierarchy For Leaf Assemblies
				//////////////////////////////////////////////
				isis::ComponentVistorBuildListOfComponentIDs  assemblyComponentIDs_ExcludingTopAssembly(true);
				isis::VisitComponents(i->assemblyComponentID, cADComponentData_map, assemblyComponentIDs_ExcludingTopAssembly );
				// When complete each component may not have a unique name (nor unique file).
				// They will *all* be present in the map.

				//ForEachLeafAssemblyInTheInputXML_AddInformationAboutSubordinates(	
				//											assemblyComponentIDs_ExcludingTopAssembly.listOfComponentIDs, 
				//											NonCyPhyID_counter,
				//											cADComponentData_map );




				isis_LOG(lg, isis_FILE, isis_INFO) << "";
				isis_LOG(lg, isis_FILE, isis_INFO) << "************** Begin modelOperations.forEachLeafAssemblyInTheInputXML_AddInformationAboutSubordinates *****************";
				modelOperations.forEachLeafAssemblyInTheInputXML_AddInformationAboutSubordinates( 
															assemblyComponentIDs_ExcludingTopAssembly.listOfComponentIDs, 
															NonCyPhyID_counter,
															cADComponentData_map );
				isis_LOG(lg, isis_FILE, isis_INFO) << "************** End modelOperations.forEachLeafAssemblyInTheInputXML_AddInformationAboutSubordinates *******************";

				isis_LOG(lg, isis_FILE, isis_INFO) << "************** End CompleteTheHierarchyForLeafAssemblies *****************";
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
					isis::BuildListOfCADModels_ThatShouldBeCopiedToNewNames(		//*cAD_Factory_ptr,
																				uniqueNameIndex, 
																				//e_PART_MODEL_TYPE,
																				e_PART_OR_ASSEMBLY_MODEL_TYPE,
																				e_SELECT_ALL_MODELS,
																				false,
																				in_MaxCADModelNameLength,
																				cADComponentData_map, 
																				fromModel_ToModel_FEA );

					////////////////////////////
					// Log Part Copy/Renames
					///////////////////////////
					isis_LOG(lg, isis_FILE, isis_INFO) << "";
					isis_LOG(lg, isis_FILE, isis_INFO) << "**** if (CompleteTheHierarchyForLeafAssemblies && UniquelyNameAllCADModelInstances) ***";
					isis_LOG(lg, isis_FILE, isis_INFO) << "************** Begin Modified Part Names for Analysis Purposes *****************";
					isis_LOG(lg, isis_FILE, isis_INFO) << "From_Part_Name   To_Part_Name";
					isis_LOG(lg, isis_FILE, isis_INFO) << fromModel_ToModel_FEA;
					isis_LOG(lg, isis_FILE, isis_INFO) << "";
					isis_LOG(lg, isis_FILE, isis_INFO)  << "************** End Modified Part Names for Analysis Purposes *****************";

					if ( fromModel_ToModel_FEA.size() > 0 ) isis::CopyModels(	//*cAD_Factory_ptr, 
																				fromModel_ToModel_FEA);

					if ( fromModel_ToModel_FEA.size() > 0 ) 
					{
						//isis::CopyModels(fromModel_ToModel);
						// Rename the parts/sub-assemblies in the Creo assembly that were copied in the previouisl step


						//ModelOperationsCreo::modify_CADInternalHierarchyRepresentation_CADComponentData__ForCopiedModel

						isis::cad::IModelOperations&  modelOpertations = cAD_Factory_ptr->getModelOperations();

						// Must do assemblies first
						for each ( CopyModelDefinition model_def in fromModel_ToModel_FEA )
						{
							if (model_def.modelType == PRO_MDL_ASSEMBLY )
							{
								modelOpertations.modify_CADInternalHierarchyRepresentation_CADComponentData__ForCopiedModel(i->assemblyComponentID,model_def, cADComponentData_map );
								/**
								ModelInstanceData modelInstanceData_temp;
								modelInstanceData_temp.modelName				= model_def.fromModelName;
								modelInstanceData_temp.modelType				= model_def.modelType;
								modelInstanceData_temp.modelHandle				= static_cast<ProSolid>(cADComponentData_map[model_def.componentInstanceID].cADModel_hdl);

								modelInstanceData_temp.assembledFeature = getProAsmcomp(cADComponentData_map[model_def.componentInstanceID].assembledFeature);
								
								modelInstanceData_temp.topAssemblyModelHandle	= static_cast<ProSolid>(cADComponentData_map[i->assemblyComponentID].cADModel_hdl);
								modelInstanceData_temp.componentPaths			= cADComponentData_map[model_def.componentInstanceID].componentPaths;
								isis::MultiFormatString  CopyToPartName_temp(model_def.toModelName);

								ProMdl     renamedModelHandle;
								Assembly_RenameSubPartOrSubAssembly ( modelInstanceData_temp, CopyToPartName_temp, renamedModelHandle );
								cADComponentData_map[model_def.componentInstanceID].cADModel_hdl = (ProSolid)renamedModelHandle;
								cADComponentData_map[model_def.componentInstanceID].cADModel_ptr_ptr = (ProMdl*)&renamedModelHandle;


								// Must fix the children assembledFeature to point to the new parent (new owner)
								for each ( std::string i_child in cADComponentData_map[model_def.componentInstanceID].children )
								{
									cADComponentData_map[i_child].assembledFeature.owner = renamedModelHandle;
								} 
								***/
							}
						}

						// For testing
						//isis::isis_ProMdlSave(cADComponentData_map[i->assemblyComponentID].modelHandle);

						// Next rename parts
						for each ( CopyModelDefinition model_def in fromModel_ToModel_FEA )
						{
							if (model_def.modelType == PRO_MDL_PART )
							{
								modelOpertations.modify_CADInternalHierarchyRepresentation_CADComponentData__ForCopiedModel(i->assemblyComponentID,model_def, cADComponentData_map );
								/***
								ModelInstanceData modelInstanceData_temp;
								modelInstanceData_temp.modelName				= model_def.fromModelName;
								modelInstanceData_temp.modelType				= model_def.modelType;
								modelInstanceData_temp.modelHandle				= static_cast<ProSolid>(cADComponentData_map[model_def.componentInstanceID].cADModel_hdl);

								modelInstanceData_temp.assembledFeature = getProAsmcomp(cADComponentData_map[model_def.componentInstanceID].assembledFeature);


								modelInstanceData_temp.topAssemblyModelHandle	= static_cast<ProSolid>(cADComponentData_map[i->assemblyComponentID].cADModel_hdl);
								modelInstanceData_temp.componentPaths			= cADComponentData_map[model_def.componentInstanceID].componentPaths;
								isis::MultiFormatString  CopyToPartName_temp(model_def.toModelName);

								ProMdl     renamedModelHandle;
								Assembly_RenameSubPartOrSubAssembly ( modelInstanceData_temp, CopyToPartName_temp, renamedModelHandle );
								cADComponentData_map[model_def.componentInstanceID].cADModel_hdl = (ProSolid)renamedModelHandle;
								cADComponentData_map[model_def.componentInstanceID].cADModel_ptr_ptr = (ProMdl*)&renamedModelHandle;

								// For testing
								//isis::isis_ProMdlSave(cADComponentData_map[i->assemblyComponentID].modelHandle);
								***/
							}						
						}
	
						// Must save this version in a scratch directory.  This is because the assemblies were changed to have
						// different part names.  If we save those assemblies in the working directory, then subsequent runs
						// in the same working directory would start with the changed assemblies instead of the original assemblies.

						std::string scratchFEADir = "Analysis_Scratch";
						// FIXME replace with _mkdir(scratchFEADir.c_str());
						std::string createScratchFEADir = "if not exist " +  scratchFEADir +  " mkdir  " + scratchFEADir;
						isis::ExecuteSystemCommand( createScratchFEADir);
						
						isis::ExecuteSystemCommand( "del /q " + scratchFEADir + "\\*.*");

						isis::MultiFormatString scratchFEADir_MultiFormat(scratchFEADir, in_MaxCADPathLength);
						//isis::setCreoWorkingDirectory( scratchFEADir_MultiFormat );
						cADsession.setCADWorkingDirectory(scratchFEADir_MultiFormat );
						isis_LOG(lg, isis_FILE, isis_INFO)	<< "setCADWorkingDirectory: " << scratchFEADir_MultiFormat;



						isis::isis_ProMdlSave(cADComponentData_map[i->assemblyComponentID].cADModel_hdl);
						isis::cad::IModelHandling&        modelHandling = cAD_Factory_ptr->getModelHandling();
						modelHandling.cADModelSave( i->assemblyComponentID, cADComponentData_map);

						// Change back to the working dir
						//isis::setCreoWorkingDirectory( workingDir_MultiFormat );
						cADsession.setCADWorkingDirectory(workingDir_MultiFormat );
						isis_LOG(lg, isis_FILE, isis_INFO)	<< "setCADWorkingDirectory: " << workingDir_MultiFormat;

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
				isis::VisitComponents(i->assemblyComponentID, cADComponentData_map, assemblyComponentIDs_IncludingTopAssembly );

				Populate_FeatureIDs_to_ComponentInstanceID_hashtable(
										assemblyComponentIDs_IncludingTopAssembly.listOfComponentIDs,
										cADComponentData_map,
										featureIDs_to_ComponentInstanceID_hashtable);

				if (  CompleteTheHierarchyForLeafAssemblies )  // The non-leaf hierarchy would have been completed previously.
				{
					//ResolveAssemblyConstraints_AddMarkersToMap( 
					//										*cAD_Factory_ptr, 
					//										assemblyComponentIDs_IncludingTopAssembly.listOfComponentIDs,
					//										featureIDs_to_ComponentInstanceID_hashtable,
					//										cADComponentData_map );

					SelectComponentDerivedFromLeafAssembly selectComponentDerivedFromLeafAssembly;
					isis::ComponentVistorBuildListOfComponentIDs  assemblyComponentIDs_ExcludeTopAssembly_SelectDerivedComps(selectComponentDerivedFromLeafAssembly, true);
					isis::VisitComponents( i->assemblyComponentID, cADComponentData_map, assemblyComponentIDs_ExcludeTopAssembly_SelectDerivedComps );


					//PopulateMap_with_Junctions_and_ConstrainedToInfo_per_CreoAsmFeatureTrees( 
					//										*cAD_Factory_ptr, 
					//										assemblyComponentIDs_ExcludeTopAssembly_SelectDerivedComps.listOfComponentIDs,
					//										featureIDs_to_ComponentInstanceID_hashtable,
					//										cADComponentData_map );
					
					isis::cad::IModelOperations&        modelOperationsCreo = cAD_Factory_ptr->getModelOperations();

					modelOperationsCreo.populateMap_with_Junctions_and_ConstrainedToInfo_per_CADAsmFeatureTrees(
															//*cAD_Factory_ptr, 
															assemblyComponentIDs_ExcludeTopAssembly_SelectDerivedComps.listOfComponentIDs,
															featureIDs_to_ComponentInstanceID_hashtable,
															cADComponentData_map );


				}

				SelectComponentInInputXML selectComponentInInputXML;
				isis::ComponentVistorBuildListOfComponentIDs  assemblyComponentIDs_ExcludeTopAssembly_SelectInputXMLComponents(selectComponentInInputXML, true);
				isis::VisitComponents( i->assemblyComponentID, cADComponentData_map, assemblyComponentIDs_ExcludeTopAssembly_SelectInputXMLComponents );


				//  ggggggggggggggggggggggggg good to here  no creo dependencies   zzzzzzzzzzzzzzzzzz

				//	We must recalculate the junctions here.  The junctions were originally calculated in Apply_CADDatum_ModelConstraints;  
				//	however, that calculation was just to determine the joint type( revolute, prismatic…).  The actual position 
				//	(i.e. markers) could have been incorrect based on the Apply_CADDatum_ModelConstraints junction computations because 
				//	a kinematic assembly could have repositioned the parts based on the complete assembly.  An example is a prismatic joint
				//	defined in a leaf assembly where the markers for the joint would only be valid in the context of the complete assembly.	
				//	In other words, the prismatic joint would allow the datums defining the joints to move based on the position of 
				//	connecting parts in the assembly.
				PopulateMap_with_Junctions_per_InputXMLConstraints(//*cAD_Factory_ptr, 
																	assemblyComponentIDs_ExcludeTopAssembly_SelectInputXMLComponents.listOfComponentIDs, cADComponentData_map, true );

				// This must be after the call to PopulateMap_with_Junctions_per_InputXMLConstraints
				PopulateMap_with_ConstrainedToInfo_per_InputXMLConstraints (	
										//*cAD_Factory_ptr,
										i->assemblyComponentID,
										assemblyComponentIDs_ExcludeTopAssembly_SelectInputXMLComponents.listOfComponentIDs,
										featureIDs_to_ComponentInstanceID_hashtable,
										cADComponentData_map );


				isis::ComponentVistorBuildListOfComponentIDs  assemblyComponentIDs_ExcludingTopAssembly(true);
				isis::VisitComponents(i->assemblyComponentID, cADComponentData_map, assemblyComponentIDs_ExcludingTopAssembly );

				//CheckValidityOfJointInformation
				if ( ValidateJointInformation ) 
				{
					std::vector<std::string> jointErrors;
					CheckValidityOfJointInformation(	assemblyComponentIDs_ExcludingTopAssembly.listOfComponentIDs, 
														cADComponentData_map,
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
			    isis::PopulateMap_with_JunctionDataInGlobalCoordinates( //*cAD_Factory_ptr,
																		i->assemblyComponentID,
																		assemblyComponentIDs_ExcludingTopAssembly.listOfComponentIDs, 
																		cADComponentData_map );

				// Determine

			}  // END if ( OutputJointInformation )

			isis_LOG(lg, isis_FILE, isis_INFO) << "";
			isis_LOG(lg, isis_FILE, isis_INFO) << "*** BEGIN Entire Component Data Tree with Internal Addresses, Leaf Subordinates (if requested), and Joints (if requested) ****";
			std::stringstream str;
			stream_AssemblyCADComponentData( i->assemblyComponentID, cADComponentData_map, str );
			isis_LOG(lg, isis_FILE, isis_INFO) << str.str();
			isis_LOG(lg, isis_FILE, isis_INFO) << "";
			isis_LOG(lg, isis_FILE, isis_INFO) << "*** END Begin Entire Component Data Tree with Internal Addresses, Leaf Subordinates (if requested), and Joints (if requested) ****";



			/////////////////////////////////////////////////
			//	Output STEP Files and Manufacturing Manifest
			////////////////////////////////////////////////	
			// For testing, un-comment out this code to Export all STEP files
			//if ( cADComponentAssemblies.DataExchangeSpecifications.size() == 0 )
			//{
			//	cADComponentAssemblies.DataExchangeSpecifications.push_back(
			//		isis::DataExchangeSpecification(isis::DATA_EXCHANGE_FORMAT_STEP, isis::AP203_SINGLE_FILE ));
			//	cADComponentAssemblies.DataExchangeSpecifications.push_back(
			//		isis::DataExchangeSpecification(isis::DATA_EXCHANGE_FORMAT_STEP, isis::AP203_E2_SINGLE_FILE ));
			//	cADComponentAssemblies.DataExchangeSpecifications.push_back(
			//		isis::DataExchangeSpecification(isis::DATA_EXCHANGE_FORMAT_STEP, isis::AP203_E2_SEPARATE_PART_FILES ));
			//	cADComponentAssemblies.DataExchangeSpecifications.push_back(
			//		isis::DataExchangeSpecification(isis::DATA_EXCHANGE_FORMAT_STEP, isis::AP214_SINGLE_FILE ));
			//	cADComponentAssemblies.DataExchangeSpecifications.push_back(
			//		isis::DataExchangeSpecification(isis::DATA_EXCHANGE_FORMAT_STEP, isis::AP214_SEPARATE_PART_FILES ));
			//}	

			bool manufacturingManifestUpdated = false; 

			// If FEA run will output data exchange files after the models are renamed for FEA purposes.
			if ( cADComponentAssemblies.DataExchangeSpecifications.size() > 0  )
			{
				if ( regenerationSucceeded )
				{
					isis_LOG(lg, isis_CONSOLE, isis_INFO) << "";
					isis_LOG(lg, isis_CONSOLE, isis_INFO) << isis_EOL << "Exporting STEP files, this could take several minutes...";
					isis_LOG(lg, isis_FILE, isis_INFO) << "";
					isis_LOG(lg, isis_FILE, isis_INFO) << isis_EOL << "Exporting STEP files";
					
					isis_LOG(lg, isis_FILE, isis_INFO) << "STEP file formats: ";

					for each( isis::DataExchangeSpecification de in cADComponentAssemblies.DataExchangeSpecifications)
					{
						isis_LOG(lg, isis_FILE, isis_INFO) << "   " << isis::DataExchangeVersion_string(de.dataExchangeVersion);
					}
					
					isis::ExportDataExchangeFiles(	 i->assemblyComponentID,
													workingDirector,
													cADComponentAssemblies.DataExchangeSpecifications,
													cADComponentData_map,
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
														cADComponentData_map );
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
				isis_LOG(lg, isis_FILE, isis_WARN)  << "Manufacturing manifest (i.e. manufacturing.manifest.json) NOT updated for assembly: " << cADComponentData_map[i->assemblyComponentID].name;
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
				CFD_Driver( //*cAD_Factory_ptr, 
					isis::V1, in_CADExtensionsDir, 
					workingDirector, *i, cADComponentData_map );
			}

			///////////////////////
			//	FEA Run
			///////////////////////
			if ( regenerationSucceeded && ( fEAAnalysisRun  )) 
			{

				std::map<std::string, std::string>  componentID_to_MaterialName_map;		 
				isis::ComponentVistorBuildListOfComponentIDs  assemblyComponentIDs_IncludingTopAssembly;
				isis::VisitComponents(i->assemblyComponentID, cADComponentData_map, assemblyComponentIDs_IncludingTopAssembly );
				isis::RetrieveCreoMaterialsNames(	assemblyComponentIDs_IncludingTopAssembly.listOfComponentIDs,
													cADComponentData_map,
													componentID_to_MaterialName_map) ;

				//for each ( std::string i_temp in assembllyComponenteIDs.listOfComponentIDs ) 
				//{
				//	std::cout << std::endl << "Material Info, Component Instance ID: " << i_temp <<
				//		"  Component Material: " << cADComponentData_map[i_temp].materialID_FromCADPart <<
				//		"  Retrieved Material: " << componentID_to_MaterialName_map[i_temp];
				//}

				bool materialMisMatchFound = false;
				std::set<std::string> materialNames;
				for each ( std::pair<std::string, std::string> mat_pair in componentID_to_MaterialName_map ) 
				{
					if ( cADComponentData_map[mat_pair.first].modelType == PRO_MDL_PART )
					{
						cADComponentData_map[mat_pair.first].materialID_FromCADPart = mat_pair.second;
						materialNames.insert(mat_pair.second);

						if ( isis::ConvertToUpperCase(cADComponentData_map[mat_pair.first].materialID_FromCADPart) !=
							isis::ConvertToUpperCase(cADComponentData_map[mat_pair.first].materialID_FromCyPhy) )
						{
							materialMisMatchFound = true;
							std::stringstream errorString;
							errorString << std::endl << std::endl << "WARNING - Material names (CyPhy/Creo) differ: " << std::endl << 
								"  Component Instance ID: " <<  cADComponentData_map[mat_pair.first].componentID << std::endl << 
								"  Component Name:        " <<  cADComponentData_map[mat_pair.first].name << std::endl << 
								"  Component Type:        " <<  ProMdlType_string(cADComponentData_map[mat_pair.first].modelType) <<  std::endl << 
								"  CyPhy Material Name:   " <<  cADComponentData_map[mat_pair.first].materialID_FromCyPhy  <<  std::endl << 
								"  Creo Material Name:    " <<  cADComponentData_map[mat_pair.first].materialID_FromCADPart  <<  std::endl << 
								"  Using Material Name:   " <<  cADComponentData_map[mat_pair.first].materialID_FromCADPart << std::endl;

							isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << errorString.str();

						}
					}
				}
				if ( materialMisMatchFound ) 
					isis_LOG(lg, isis_FILE, isis_WARN) << "Material name(s) in CyPhy differ from material name(s) in the Creo Part(s). See " << 
					logFile_PathAndFileName << " in the log directory for a listing of the differences.  Search on WARNING.";

				//for each ( std::string i_temp in assembllyComponenteIDs.listOfComponentIDs ) 
				//{
				//	std::cout << std::endl << "Material Info, Component Instance ID: " << i_temp <<
				//		"  Component Material: " << cADComponentData_map[i_temp].materialID_FromCADPart <<
				//		"  Retrieved Material: " << componentID_to_MaterialName_map[i_temp];
				//}			
	
				bool materialLibLookup_ErrorsOccured = false;

				try
				{
					isis::ReadMaterialsLibrary( MaterialsLibrary_PathAndFileName(), materialNames, cADComponentAssemblies.materials);

					// Log materials
					isis_LOG(lg, isis_FILE, isis_INFO) << "Materials: ";
					for each ( std::pair<std::string, isis::Material> i in cADComponentAssemblies.materials )
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
					isis::Create_FEADecks_BatFiles( //*cAD_Factory_ptr,
													*i,   // TopLevelAssemblyData	
													cADComponentAssemblies.materials, 
													workingDirector,
													in_ProgramName_Version_TimeStamp,
													cADComponentData_map );
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
															cADComponentAssemblies.materials,
															nastranMaterialID_to_CompnentID_map,
															cADComponentData_map );
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
				isis::PopulateBallisticFiles(// *cAD_Factory_ptr, 
												*i, workingDirector, cADComponentData_map);
			}

			if ( blastAnalysisRun )
			{
				// This does not write to ComputedValues.xml
				// The reference plane is written to a json file
				isis::PopulateBlastFiles( //*cAD_Factory_ptr, 
											*i, workingDirector, cADComponentData_map);
			}

			// This is not needed because the only file needed for CFD in the ComputedValues.xml
			// is the reference plane, which is created by CreateXMLFile_ComputedValues below
			//if ( cFDAnalysisRun )
			//{
			//	isis::PopulateCFDFiles( *i, workingDirector, cADComponentData_map);
			//}

			// WARNING - Do not save the assembly/models after this point.  Doing so will save the temporarily created material.


			///////////////////////////////////
			//	Output Interference Report
			//////////////////////////////////	
	
			std::string InterferenceReport_PathAndFileName = xMLInputFile_PathAndFileName;	 		
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
									errorString << "Referenced Model Name:        " << cADComponentData_map[k.componentID].name << std::endl;;
									errorString << "Referenced Model ComponentID: " << k.componentID;
									errorString << k;
								}
							throw isis::application_exception(errorString);		
				}

				if ( regenerationSucceeded_ForAllAssemblies && 
					 cADComponentAssemblies.unassembledComponents.size() == 0 &&  
					 cADComponentAssemblies.topLevelAssemblies.size() == 1)
				{			
					isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "";
					isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "Creating Interference File";	

					isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "   Created:    " + InterferenceReport_PathAndFileName;
					isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO) << "   Populating: " + InterferenceReport_PathAndFileName + ", Note: For large assemblies, this could take several minutes.";

					std::vector<PartInterferences>	partInterferences;
					bool interferenceAnalysisCompletedWithoutError = false;

					try 
					{
						//CreateInterferenceReport( InterferenceReport_PathAndFileName, i->assemblyComponentID, interferenceCount_CADComputations[0], cADComponentData_map );
						modelOperations.computePartInterferences(	i->assemblyComponentID, // This must be an assembly
																	cADComponentData_map, 
																	partInterferences);
						interferenceAnalysisCompletedWithoutError = true;
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

					WriteInterferenceReport(		i->assemblyComponentID,  
												cADComponentData_map,	
												interferenceCount_CADComputations[0],
												InterferenceReport_PathAndFileName,
												interferenceAnalysisCompletedWithoutError,
												partInterferences);
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
						if ( cADComponentAssemblies.unassembledComponents.size() != 0 )
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
				}  // END if ( regenerationSucceeded_ForAllAssemblies && cADComponentAssemblies.unassembledComponents.size() == 0 &&  cADComponentAssemblies.topLevelAssemblies.size() == 1)

			}  // END if ( interferenceRun )

		} // for ( std::list<isis::TopLevelAssemblyData>::const_iterator i(cADComponentAssemblies.topLevelAssemblies.begin());

		/////////////////////////////////////////////
		// Output Metrics File 
		/////////////////////////////////////////////
		// Metrics Files Should be outputted after all assemblies are built.
		// The functions that follow apply to all assemblies.
		std::string MeticsOutputXML_PathAndFileName = xMLInputFile_PathAndFileName;	 		
		MeticsOutputXML_PathAndFileName.replace(  MeticsOutputXML_PathAndFileName.end() - 4, 
													MeticsOutputXML_PathAndFileName.end(), "_metrics.xml"); 
		// Delete old metrics file if it exists.  This should be done so that an old metrics file would not be inadvertently used if a
		// new metrics file was not created.
		isis::IfFileExists_DeleteFile( MeticsOutputXML_PathAndFileName);
		if ( regenerationSucceeded_ForAllAssemblies && cADComponentAssemblies.unassembledComponents.size() == 0 )
		{
			for each ( const TopLevelAssemblyData &i_tl in cADComponentAssemblies.topLevelAssemblies)
			{
				isis::ComponentVistorBuildListOfComponentIDs  assemblyComponentIDs_IncludingTopAssembly;
				isis::VisitComponents(i_tl.assemblyComponentID, cADComponentData_map, assemblyComponentIDs_IncludingTopAssembly );

				// Add Bounding Box Values to Map
				AddBoundingBoxValuesToMap( //*cAD_Factory_ptr, 
											assemblyComponentIDs_IncludingTopAssembly.listOfComponentIDs, cADComponentData_map );
				// Add Mass Property Values to Map
				AddMassPropertyValuesToMap( *cAD_Factory_ptr, assemblyComponentIDs_IncludingTopAssembly.listOfComponentIDs, cADComponentData_map );
			}

			// The following function iterates through each top-assembly and recursively
			// finds the children.  This was necessary because the MetricID that is persisted in the metrics
			// file must must be unique across all assemblies.  At some point, this function should be changed
			// to take a list of ComponentInstanceIDs and operate on only that list.  This would supersede the 
			// approach of recursively finding the children and would be more compatible with the approach in
			// other areas of this program.
			OutputCADMetricsToXML_Driver(	//*cAD_Factory_ptr,
											ISIS_METRIC_FILE_VERSION,
											regenerationSucceeded_ForAllAssemblies,
											OutputJointInformation,
											cADComponentAssemblies,
											cADComponentData_map,  
											MeticsOutputXML_PathAndFileName,
											logFile_PathAndFileName );				
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
		    
			CreateXMLFile_ComputedValues(	//*cAD_Factory_ptr,
											workingDirector,
											computationTypes,
											cADComponentAssemblies,
											cADComponentData_map );
		}
		else
		{
			isis_LOG(lg, isis_FILE, isis_WARN) << "ComputedValues File - Did NOT create a ComputedValues file because the attempts to regenerate the assembly failed.";	
		}
		
		////////////////////////////////////////////
		// Output Step Files for Unassembled Models
		////////////////////////////////////////////
		bool manufacturingManifestUpdated = false;
		if ( cADComponentAssemblies.unassembledComponents.size() > 0 )
		{
			isis_LOG(lg, isis_CONSOLE, isis_INFO) << "";
			isis_LOG(lg, isis_CONSOLE, isis_INFO)  << "Exporting STEP files for unassembled models, this could take several minutes...";
			isis_LOG(lg, isis_FILE, isis_INFO) << "";
			isis_LOG(lg, isis_FILE, isis_INFO)  << "Exporting STEP files for unassembled models";

			bool logCouldNotFindManifestError = true;

			for ( std::list<isis::UnassembledComponent>::const_iterator unusedComp( cADComponentAssemblies.unassembledComponents.begin());
				  unusedComp != cADComponentAssemblies.unassembledComponents.end(); ++unusedComp )
			{

				//std::cout << std::endl << "Exporting STEP file for " + unusedComp->name + ", this could take several seconds...";
				isis::ExportDataExchangeFiles(	unusedComp->componentID,
												unusedComp->name,
												//ProMdlType_enum(unusedComp->modelType),
												unusedComp->modelType,
												unusedComp->geometryRepresentation,
												workingDirector,
												cADComponentAssemblies.DataExchangeSpecifications, true );
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
												cADComponentData_map );
					isis_LOG(lg, isis_FILE, isis_INFO) << "";
					isis_LOG(lg, isis_FILE, isis_INFO)  << "Updating Manufacturing Manifest (manufacturing.manifest.json) for unassembled model: " << unusedComp->name;
					manufacturingManifestUpdated = true; 
				}
			}
		}	

		/////////////////////////////////////////////
		// Log Status of Updating Manufacturing Json
		/////////////////////////////////////////////
		if ( !manufacturingManifestUpdated  && cADComponentAssemblies.unassembledComponents.size() > 0 )
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
						<< std::endl << "Log File: " <<  logFile_PathAndFileName;
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