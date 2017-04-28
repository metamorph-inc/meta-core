#include <AssembleUtils.h>
#include <CommonUtilities.h>
#include <CADCommonConstants.h>
#include <CommonFeatureUtils.h>
//#include "WindowsHDependentCommonFunctions.h"
#include <ToolKitPassThroughFunctions.h>
#include <CommonFeatureUtils.h>
#include <CommonFunctions.h>
#include <JsonHelper.h>
#include <time.h>
#include "LoggerBoost.h"
#include "CommonDefinitions.h"
#include <CommonFunctions.h>
#include <DatumRefResolver.h>
#include <GraphicsFunctions.h>
#include <boost/algorithm/string.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include <e3ga.h>
#include <WindowsFunctions.h>
#include <JointCreo.h>
#include <Windows.h>

namespace isis
{
	const int DISABLED_CONSTRAINT_MASK = 32;

	bool Get_CompleteTheHierarchyForLeafAssemblies( const CADAssemblies &in_CADAssemblies )
	{
		for each ( ProcessingInstruction i in in_CADAssemblies.processingInstructions )
			if ( i.primary == COMPLETE_THE_HIERARCHY_FOR_LEAF_ASSEMBLIES )
				return true;
		return false;
	}
	
	bool Get_UniquelyNameAllCADModelInstances( const CADAssemblies &in_CADAssemblies )
	{
		for each ( ProcessingInstruction i in in_CADAssemblies.processingInstructions )
			if ( i.primary == UNIQUELY_NAME_ALL_CAD_MODEL_INSTANCES)
				return true;
		return false;
	}

	bool Get_OutputJointInformation( const CADAssemblies &in_CADAssemblies )
	{
		for each ( ProcessingInstruction i in in_CADAssemblies.processingInstructions )
			if ( i.primary == OUTPUT_JOINT_INFORMATION )
				return true;
		return false;
	}

	bool Get_ValidateJointInformation( const CADAssemblies &in_CADAssemblies )
	{
		for each ( ProcessingInstruction i in in_CADAssemblies.processingInstructions )
			if ( i.secondary == VALIDATE_JOINT_INFORMATION )
				return true;
		return false;
	}


	// If at least one of the assemblies in in_CADAssemblies specifies cFDAnalysis == true, then return true.
	bool HasAssemblyBasedComputations( const CADAssemblies &in_CADAssemblies )
	{
		for ( std::list<isis::TopLevelAssemblyData>::const_iterator i( in_CADAssemblies.topLevelAssemblies.begin()); 
				i !=  in_CADAssemblies.topLevelAssemblies.end();
				++i)
		{
			if ( i->assemblyMetrics.size() > 0 ) return true;
		}
		return false;
	}

	// If at lease one of the assemblies in in_CADAssemblies specifies analysesCAD.interference == true, then return true.
	bool IsAInterferenceRun( const CADAssemblies &in_CADAssemblies )
	{
		for ( std::list<isis::TopLevelAssemblyData>::const_iterator i( in_CADAssemblies.topLevelAssemblies.begin()); 
				i !=  in_CADAssemblies.topLevelAssemblies.end();
				++i)
		{
			// Old approach using assemblyMetrics now, if ( i->analysesCAD.interference ) return true;
			for each (const CADComputation &j in i->assemblyMetrics) if ( j.computationType == COMPUTATION_INTERFERENCE_COUNT ) return true;
		}
		return false;
	}

	// Restrictions:
	//	There can be at most one InterferenceCount computation.  
	//	InterferenceCount must reference the top assembly.  Sub-assemblies are not supported at this time.
	void Validate_ComputationInterferenceCount_ThrowExceptionIfInvalid (  
											const CADAssemblies								&in_CADAssemblies,
											std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map)
															throw (isis::application_exception)
	{
		for each ( const TopLevelAssemblyData &i in in_CADAssemblies.topLevelAssemblies)
		{
			int count = 0;
			std::vector<CADComputation> interferenceCountReferencedComponents;
			for each (const CADComputation &j in i.assemblyMetrics) 
			{
				if ( j.computationType == COMPUTATION_INTERFERENCE_COUNT )
				{ 
					++count;
					interferenceCountReferencedComponents.push_back(j);
					if ( j.componentID != i.assemblyComponentID )
					{
						std::stringstream errorString;
						errorString <<	"Function - " << __FUNCTION__ << ", An " << std::endl <<
										"InterferenceCount computation must always be for the top assembly. It cannot" << std::endl <<
										"be for sub-assemblies.  Check the CyPhy model and verify that a CADComputationComponent that contains" << std::endl <<
										"an InterferenceCount is not connected to a test-injection point other than the top assembly." << std::endl <<
										"   Top Assembly Model Name:  " <<	 in_CADComponentData_map[i.assemblyComponentID].name << 
										 j;
										//"   InterferenceCount Referenced Model Name: " <<	 in_CADComponentData_map[j.componentID].name;
						throw isis::application_exception(errorString);		
					}
				}

			}
			if ( count > 1 )
			{
				std::stringstream errorString;
				errorString << "Function - " << __FUNCTION__ << ", " << std::endl <<
							"There must be no more than one InterferenceCount CADComputationComponent within a CyPhy testbench." << std::endl <<
							"Number of InterferenceCount Found: " << count << std::endl <<
							"InterferenceCount CADComputationComponents: " << std::endl;
							for each ( const CADComputation &k in interferenceCountReferencedComponents )
							{
								errorString << "Referenced Model Name:        " << in_CADComponentData_map[k.componentID].name << std::endl;;
								errorString << "Referenced Model ComponentID: " << k.componentID;
								errorString << k;
							}
				throw isis::application_exception(errorString);		
			}
		}
	}


	void RetrieveComputationOfAGivenType( const std::list<CADComputation>	&in_AssemblyMetrics,
										  e_ComputationType					in_ComputationType,
										  std::vector<CADComputation>		&out_CADComputations )
	{
		for each (const CADComputation &i in in_AssemblyMetrics)
		{
			if ( i.computationType == in_ComputationType ) out_CADComputations.push_back(i);
		}
	}

/**
 If a plain logfile is provided then it is expected that the logfile will be 
 placed into a directory, named "log", creating it if necessary.
 In every other case the normal file logic will be used.
 In the case of relative paths the current directory will be used.

 A viable alternative would be to consider relative paths from the working directory.
 If that were the case then the working directory needs to be passed as an argument.
*/
void SetupLogFile( const std::string in_LogFileName, std::ofstream &in_out_LogFile ) throw (isis::application_exception)
{
	::boost::filesystem::path logFilePath(in_LogFileName);
	if (logFilePath.empty()) {
		std::string TempError = "exception : empty log file name";
		throw isis::application_exception("C03001", TempError);
	}
	if (logFilePath.is_absolute()) {
		in_out_LogFile.open (logFilePath.c_str(), std::ios::out | std::ios::trunc );
		if (! in_out_LogFile.is_open()) {
			std::string TempError = "exception : Could not open absolute log file: " + logFilePath.generic_string();
			throw isis::application_exception("C03001", TempError);
		} 
		clog.rdbuf(in_out_LogFile.rdbuf());
		return;
	}
	// the file path is relative
	if (logFilePath.has_parent_path()) {
		in_out_LogFile.open (logFilePath.c_str(), std::ios::out | std::ios::trunc );
		if (! in_out_LogFile.is_open()) {
			std::string TempError = "exception : Could not open relative log file: " + logFilePath.generic_string();
			throw isis::application_exception("C03001", TempError);
		} 
		clog.rdbuf(in_out_LogFile.rdbuf());
		return;
	}
	::boost::filesystem::path localLogPath = 
		::boost::filesystem::current_path() / "log";

	if (! ::boost::filesystem::exists( localLogPath ) ) {
		if (! ::boost::filesystem::create_directory( localLogPath ) ) {
			std::string TempError = "exception : Could not create directory for log file: " + localLogPath.generic_string();
			throw isis::application_exception("C03001", TempError);
		}
	}
	::boost::filesystem::path localLogFilePath = localLogPath / logFilePath;

	in_out_LogFile.open (localLogFilePath.c_str(), std::ios::out | std::ios::trunc );
	if (! in_out_LogFile.is_open()) {
		std::string TempError = "exception : Could not open plain log file: " + localLogFilePath.generic_string();
		throw isis::application_exception("C03001", TempError);
	} 
	clog.rdbuf(in_out_LogFile.rdbuf());

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
::boost::filesystem::path SetupWorkingDirectory( std::string & inout_workingDirectory ) {
   ::boost::filesystem::current_path(inout_workingDirectory);
   ::boost::filesystem::path workingDir = ::boost::filesystem::current_path();
   inout_workingDirectory = workingDir.generic_string();
   return workingDir;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
   Purpose:     Initializes the <i>ProAsmcompath</i> data structure.

   Input Arguments:
      in_assembly_model       - The top-level assembly
      in_base_model_path_list - The idetifiers in the assembly path

   Output Arguments:
      out_base_comp_path      - The assembly component path to a component instance

   Return Values:
      PRO_TK_NO_ERROR   - The function successfully initialized the data structure.
      PRO_TK_BAD_INPUTS - One or more of the input arguments are invalid.
*/
ProError Retrieve_ProAsmcomppath(const ProSolid in_assembly_model, const std::list<int> in_base_model_path_list, ProAsmcomppath &out_base_comp_path) {
   ProIdTable c_id_table_base_model;
   int base_model_table_size;
   Populate_c_id_table( in_base_model_path_list, c_id_table_base_model, base_model_table_size );

   return ProAsmcomppathInit ( in_assembly_model,	
                               c_id_table_base_model,	
                               base_model_table_size,	
                               &out_base_comp_path);
}

void Retrieve_ProAsmcomppath_WithExceptions(	const ProSolid in_assembly_model, 
												const std::list<int> &in_base_model_path_list, 
												ProAsmcomppath &out_base_comp_path)  throw (isis::application_exception)
{
   ProIdTable c_id_table_base_model;
   int base_model_table_size;
   Populate_c_id_table( in_base_model_path_list, c_id_table_base_model, base_model_table_size );

   isis::isis_ProAsmcomppathInit ( in_assembly_model,	
                               c_id_table_base_model,	
                               base_model_table_size,	
                               &out_base_comp_path);
}

void ProAsmcomppathToList(std::list<int> &out_list, const ProAsmcomppath &in_path)
{
	for (int i = 0; i < in_path.table_num; i++)
	{
		out_list.push_back(in_path.comp_id_table[i]);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Populate_c_id_table( const list<int> &in_path_list, ProIdTable out_c_id_table, int &out_c_id_table_size )
{
	int index = 0;
	for ( list<int>::const_iterator i = in_path_list.begin();
	  i != in_path_list.end(); ++i )
	{
		out_c_id_table[index] = *i;
		++index;
	}
	out_c_id_table [index] = -1;
	out_c_id_table_size = in_path_list.size();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RetrieveTranformationMatrix_Assembly_to_Child (  
							const std::string  &in_AssemblyComponentID,
							const list<int>	   &in_ChildComponentPaths,
							std::map<std::string, isis::CADComponentData>		&in_CADComponentData_map,  
							ProBoolean   in_bottom_up,
							double out_TransformationMatrix[4][4] )  throw (isis::application_exception)
{
	/*
	// Must get the path from the assembly to the child
	ProIdTable	c_id_table;
	int			c_id_table_size;

	Populate_c_id_table( in_ChildComponentPaths, c_id_table, c_id_table_size );

	//std::cout << std::endl << std::endl << "in_AssemblyComponentID: " << in_AssemblyComponentID << " Name: " << in_CADComponentData_map[in_AssemblyComponentID].name ;
	//std::cout << std::endl << "in_CADComponentData_map[in_AssemblyComponentID].modelHandle: " << in_CADComponentData_map[in_AssemblyComponentID].modelHandle;
	//std::cout << std::endl << "in_ChildComponentID: " << in_ChildComponentID << " Name: " << in_CADComponentData_map[in_ChildComponentID].name;
	//std::cout << std::endl << "c_id_table[0]: " << c_id_table[0];

	ProAsmcomppath	comp_path;
	isis::isis_ProAsmcomppathInit (	in_CADComponentData_map[in_AssemblyComponentID].modelHandle,	//ProSolid   p_solid_handle
									c_id_table,			// ProIdTable 
									c_id_table_size,	// table_size 
									&comp_path);		// ProAsmcomppath *p_handle


	isis::isis_ProAsmcomppathTrfGet (	&comp_path, 				//	ProAsmcomppath *p_path,
										in_bottom_up,				// ProBoolean   bottom_up,
										out_TransformationMatrix ); //ProMatrix    transformation);

	*/
	RetrieveTranformationMatrix_Assembly_to_Child (  
							in_CADComponentData_map[in_AssemblyComponentID].modelHandle,
							in_ChildComponentPaths, 
							in_bottom_up,
							out_TransformationMatrix ); 


}


void RetrieveTranformationMatrix_Assembly_to_Child (  
							const ProSolid		&in_assembly_model,
							const list<int>	   &in_ChildComponentPaths, 
							ProBoolean   in_bottom_up,
							double out_TransformationMatrix[4][4] )  throw (isis::application_exception)
{
	// Must get the path from the assembly to the child
	ProIdTable	c_id_table;
	int			c_id_table_size;

	Populate_c_id_table( in_ChildComponentPaths, c_id_table, c_id_table_size );

	ProAsmcomppath	comp_path;
	isis::isis_ProAsmcomppathInit (	in_assembly_model,	//ProSolid   p_solid_handle
									c_id_table,			// ProIdTable 
									c_id_table_size,	// table_size 
									&comp_path);		// ProAsmcomppath *p_handle


	isis::isis_ProAsmcomppathTrfGet (	&comp_path, 				//	ProAsmcomppath *p_path,
										in_bottom_up,				// ProBoolean   bottom_up,
										out_TransformationMatrix ); //ProMatrix    transformation);

}


////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Note - in_UniqueNameIndex == 0, the returned string will be Z0Z
//		  in_UniqueNameIndex == -1, he returned string will be Z-1Z  // true even for unsigned int
//		  in_UniqueNameIndex == -1, he returned string will be Z1Z
std::string CreateStringBasedOnUniqueNameIndex( unsigned int in_UniqueNameIndex )
{
	char buffer[64];
	_itoa_s(in_UniqueNameIndex,buffer,64,10);

	std::string tempString = "_" + std::string(buffer) + "Z";
	return tempString;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
// Amalgamate in_FirstString + in_SecondString, where in_FirstString is truncated, if necessary,
// so that the amalgamated string is no longer that in_AllowedSize.
// Note: If in_SecondString.size() >= in_AllowedSize, then in_SecondString is returned.

std::string  MergeStrings_TryToKeepWithinAllowedSize (	const std::string &in_FirstString,
														const std::string &in_SecondString,
														unsigned int in_AllowedSize)
{
	// The following line, if true, would result in a string longer than in_AllowedSize
	if ( in_SecondString.size() >= in_AllowedSize ) return in_SecondString;

	if ( ( in_FirstString.size() + in_SecondString.size() ) <= in_AllowedSize ) return in_FirstString + in_SecondString;

	// Must truncate in_FirstString

	int allowedLengthFirstString = in_AllowedSize - in_SecondString.size();
	
	return in_FirstString.substr(0, allowedLengthFirstString) + in_SecondString;
}
	
/////////////////////////////////////////////////////////////////////////////////////////////////////
std::string BuildAFamilyTableCompleteModelName ( const std::string &in_ModelName,
												 const std::string &in_FamilyTableEntry )
{
	return in_FamilyTableEntry + "<" + in_ModelName + ">";

}

/////////////////////////////////////////////////////////////////////////////////////////////////////

void ExtractModelName_FamilyTable_Info (	const std::string	&in_OrigName, 
											std::string			&out_ModelName,
											std::string			&out_FamilyTableEntry,
											bool				&out_FamilyTableModel )
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CreateModelNameWithUniqueSuffix(  
			unsigned int		in_UniqueNameIndex, 
			const std::string	&in_ModelName_CouldIncludeFamilyTableEntry,
			std::string			&out_ModelName_Without_Suffix,		// e.g. Chassis
			std::string			&out_ModelName_With_Suffix,			// e.g. ChassisZ1Z
			std::string			&out_CompleteName,					// For family tables, would be the complete name
																	// e.g. Chassis_8_Wheel<ChassisZ1Z>
																	// otherwise, same as out_ModelName_With_Suffix
			unsigned int in_AllowedSize )   
													throw (isis::application_exception)
{

	if ( in_ModelName_CouldIncludeFamilyTableEntry.size() > in_AllowedSize )
	{
			std::stringstream errorString;
			errorString 
				<< "Function - " << __FUNCTION__ << ", string (in_ModelName_CouldIncludeFamilyTableEntry) length exceeds " <<  in_AllowedSize << " characters.  " <<
						std::string("in_ModelName_CouldIncludeFamilyTableEntry: ") + in_ModelName_CouldIncludeFamilyTableEntry + 
						"  out_ModelName_With_Suffix: " << out_ModelName_With_Suffix << 
						"  out_CompleteName: " + out_CompleteName;  	  
			throw isis::application_exception("C03002", errorString);
	}

	std::string tempUniqueString;
	tempUniqueString = CreateStringBasedOnUniqueNameIndex(in_UniqueNameIndex);

	std::string		familyTableEntry;
	bool			familyTableModel;

	ExtractModelName_FamilyTable_Info (	in_ModelName_CouldIncludeFamilyTableEntry, 
										out_ModelName_Without_Suffix,
										familyTableEntry,
										familyTableModel );
	if ( familyTableModel )
	{
		int tempAllowedSize = in_AllowedSize - familyTableEntry.size() - 2;  // -2 for the "<" aand ">"
		out_ModelName_With_Suffix = MergeStrings_TryToKeepWithinAllowedSize(out_ModelName_Without_Suffix, tempUniqueString, tempAllowedSize);
		out_CompleteName = BuildAFamilyTableCompleteModelName( out_ModelName_With_Suffix, familyTableEntry);
	}
	else
	{
		// The following line could result in a string longer than in_AllowedSize, the length will be checked at the end of this function.
		out_ModelName_With_Suffix = MergeStrings_TryToKeepWithinAllowedSize(out_ModelName_Without_Suffix, tempUniqueString, in_AllowedSize);
		out_CompleteName = out_ModelName_With_Suffix;
	}


	if ( out_CompleteName.size() > in_AllowedSize )
	{
			std::stringstream errorString;
			errorString 
				<< "Function - " + std::string(__FUNCTION__) +  ", string length exceeds " <<  in_AllowedSize << " characters.  " <<
						std::string("in_ModelName_CouldIncludeFamilyTableEntry: ") + in_ModelName_CouldIncludeFamilyTableEntry + 
						"  out_ModelName_With_Suffix: " << out_ModelName_With_Suffix << 
						"  out_CompleteName: " + out_CompleteName;  	  
			throw isis::application_exception("C03002", errorString);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// STEP file name case:
	// AP203_E2_SEPARATE_PART_FILES
	//		Assembly:						As entered	e.g.  FourBar.asm  -->  FourBar_asm.stp
	//		Part Included in Assembly:		Lower Case  e.g.  BAR_02.prt   -->	bar_02_prt.stp
	//		Part standalone					As entered	e.g.  MaSs		   -->	MaSs_prt.stp 
	// AP203_E2_SINGLE_FILE
	//		Assembly:						As entered	e.g.  FourBar.asm  -->  FourBar_asm.stp
	//		Part standalone					As entered	e.g.  MaSs		   -->	MaSs_prt.stp 	
	// AP203_SINGLE_FILE
	//		Assembly:						As entered	e.g.  FourBar.asm  -->  FourBar_asm.stp
	//		Part standalone					As entered	e.g.  MaSs		   -->	MaSs_prt.stp 	
	// AP214_SEPARATE_PART_FILES
	//		Assembly:						As entered	e.g.  FourBar.asm  -->  FourBar_asm.stp
	//		Part Included in Assembly:		Lower Case  e.g.  BAR_02.prt   -->	bar_02_prt.stp
	//		Part standalone					As entered	e.g.  MaSs		   -->	MaSs_prt.stp 
	// AP214_SINGLE_FILE
	//		Assembly:						As entered	e.g.  FourBar.asm  -->  FourBar_asm.stp
	//		Part standalone					As entered	e.g.  MaSs		   -->	MaSs_prt.stp 	

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void UpdateManufacturingManifestWithSTEPFileInfo( 
									e_DataExchangeVersion in_DataExchangeVersion, // AP203_SINGLE_FILE, AP203_E2_SINGLE_FILE, AP203_E2_SEPARATE_PART_FILES...
									const std::string	&in_ComponentID,
									bool				in_OnlyUpdateManifestForParts,
									bool				in_ChangeCaseOfPartStepFileToLowerCase,
									std::map<std::string, isis::CADComponentData> &in_CADComponentData_map )
														throw (isis::application_exception)
	{
		
		try
		{
			std::string stepFileDirectory = isis::DataExchangeVersion_string(in_DataExchangeVersion) + "/";

			if ( isis::FileExists( manufacturingManifestJson_PathAndFileName.c_str() ))
			{
				isis::ComponentVistorBuildListOfComponentIDs  assembllyComponenteIDs;
				isis::VisitComponents(in_ComponentID, in_CADComponentData_map, assembllyComponenteIDs );

				std::map<std::string, std::string> componentInstanceId_to_StepFile_map;

				for each ( std::string cID in assembllyComponenteIDs.listOfComponentIDs )
				{
					//  When Creo outputs STEP files with separate part-files option, 
					//  only the part files are created separately and are named with _prt.
					//  The assembly hierarchy is represented in the top-assembly STEP file.

					std::string tempFullPathToStepFile;
					if (in_ChangeCaseOfPartStepFileToLowerCase)
						tempFullPathToStepFile = stepFileDirectory + isis::ConvertToLowerCase((const std::string&)in_CADComponentData_map[cID].name);
					else
						tempFullPathToStepFile = stepFileDirectory + (const std::string&)in_CADComponentData_map[cID].name ;
					
					if ( in_CADComponentData_map[cID].modelType == PRO_PART )					
						tempFullPathToStepFile += "_prt.stp";
					else
						tempFullPathToStepFile += "_asm.stp";
					
					if (in_OnlyUpdateManifestForParts )
					{
						if ( in_CADComponentData_map[cID].modelType == PRO_PART )				
							componentInstanceId_to_StepFile_map[cID] = tempFullPathToStepFile;
					}
					else
					{
						componentInstanceId_to_StepFile_map[cID] = tempFullPathToStepFile;
					}
				}

				std::set<std::string> componentInstanceIds_AddedToManifest;
				isis_CADCommon::AddStepFileMappingToManufacturingManifest(
														manufacturingManifestJson_PathAndFileName,
														componentInstanceId_to_StepFile_map,
														componentInstanceIds_AddedToManifest );
				/////////////////////////////////////////
				// Log StEP File Names added to manifest
				/////////////////////////////////////////
				isis_LOG(lg, isis_FILE, isis_INFO) << "";
				isis_LOG(lg, isis_FILE, isis_INFO) << "Added STEP file names to " <<  
										  manufacturingManifestJson_PathAndFileName;
				bool flagAdded = false;
				for each ( std::string compID in componentInstanceIds_AddedToManifest )
				{
					isis_LOG(lg, isis_FILE, isis_INFO) << "   Component Instance ID: " << compID <<
						            "  STEP File Name: " << componentInstanceId_to_StepFile_map[compID];
					flagAdded = true;
				}
				if ( !flagAdded ) isis_LOG(lg, isis_FILE, isis_INFO) << "   None";

				/////////////////////////////////////////////
				// Log StEP File Names Not added to manifest
				/////////////////////////////////////////////
				isis_LOG(lg, isis_FILE, isis_INFO) << "";
				isis_LOG(lg, isis_FILE, isis_INFO) << "Did Not add STEP file names to " <<  
										  manufacturingManifestJson_PathAndFileName;

				flagAdded = false;
				for each ( std::string compID in assembllyComponenteIDs.listOfComponentIDs )
				{
					if ( componentInstanceIds_AddedToManifest.find(compID) == componentInstanceIds_AddedToManifest.end() &&
						 in_CADComponentData_map[compID].modelType == PRO_PART )
					{
						isis_LOG(lg, isis_FILE, isis_INFO) << "   Component Instance ID: " << compID <<
						            "  STEP File Name: " << componentInstanceId_to_StepFile_map[compID];
						flagAdded = true;
					}
				}
				if ( !flagAdded ) isis_LOG(lg, isis_FILE, isis_INFO) << "   None";
				
			}  
			else
			{
				isis_LOG(lg, isis_FILE, isis_INFO) << CouldNotFindManufacturingManifestError;
			} // END if ( isis::FileExists(
		}
		catch (isis::application_exception &ex )
		{
			isis_LOG(lg, isis_FILE, isis_ERROR) << "ERROR, Function: " << __FUNCTION__ << ", Error Message: " << ex.tostring(); 
		}
		catch (...)
		{
			isis_LOG(lg, isis_FILE, isis_ERROR) << "ERROR, Function: " << __FUNCTION__ << ", Error Message: Unkown Error"; 
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	// If a part name (not assembly name) appears more than once in in_out_CADComponentData_map
	//	then
	//		this function modifies in_out_CADComponentData_map to have unique name for the second
	//		and later occurrences of the particular part name.  The new and old part names are added
	//      to out_ToPartName_FromPartName. 
	/*
	void ModifyToHaveAUniqueNameForEachPart( 
							int &in_out_UniqueNameIndex,
							std::map<std::string, isis::CADComponentData> &in_out_CADComponentData_map, 
							std::map<std::string, std::string>			  &out_ToPartName_FromPartName )
																		throw (isis::application_exception)
	{
		std::set<std::string>  partNames;

		//int index = 1;
		//int maxPartNameLength_BeforeSuffix = PRO_NAME_SIZE - 8;  //  32 - 8 = 24

		char buffer[isis::ISIS_CHAR_BUFFER_LENGTH];

		for( std::map<std::string, isis::CADComponentData>::iterator i(in_out_CADComponentData_map.begin());
			 i != in_out_CADComponentData_map.end();
			 ++i )
		{
			//std::cout << std::endl << "Model Name +++++++++>           " << i->second.name;
			//std::cout << std::endl << " i->second.modelType +++++++++> " << i->second.modelType;

			// Only check for parts.  Not concerned about assemblies
			if ( i->second.modelType == PRO_PART  && 
				 partNames.find(isis::ConvertToUpperCase(i->second.name)) !=  partNames.end() )
			{
				std::string origNameWithoutFamilyEntry;
				std::string modelName;
				std::string completeName;
				CreateModelNameWithUniqueSuffix(  in_out_UniqueNameIndex, 
												i->second.name,
												origNameWithoutFamilyEntry,
												modelName, 
												completeName );
				//std::cout << std::endl << "origNameWithoutFamilyEntry +++++++++> "	<< origNameWithoutFamilyEntry;
				//std::cout << std::endl << "modelName +++++++++> "					<< modelName;
				//std::cout << std::endl << "completeName +++++++++> "				<< completeName;

				i->second.name = completeName;
				out_ToPartName_FromPartName[modelName] =  origNameWithoutFamilyEntry;
				++in_out_UniqueNameIndex;
			}
			partNames.insert(isis::ConvertToUpperCase(i->second.name));
		}
	}	// END ModifyToHaveAUniqueNameForEachPart
	*/
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool  ModelTypesMatch( e_ModelTypeIndicator			in_ModelTypeIndicator,
						   ProMdlType					in_ModelType )
	{
		if ( in_ModelTypeIndicator == e_PART_OR_ASSEMBLY_MODEL_TYPE ) return true;
		if ( in_ModelTypeIndicator == e_PART_MODEL_TYPE		&& in_ModelType == PRO_MDL_PART ) return true;
		if ( in_ModelTypeIndicator == e_ASSEMBLY_MODEL_TYPE && in_ModelType == PRO_MDL_ASSEMBLY ) return true;

		return false;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool SelectModelIndicated (		e_ModelSelectorIndicator	in_ModelSelectorIndicator,
									bool						in_ParametricParametersPresent )

	{
		if ( in_ModelSelectorIndicator == e_SELECT_ALL_MODELS ) 
			return true;
		else
			if ( in_ParametricParametersPresent && in_ModelSelectorIndicator == e_SELECT_ONLY_PARAMETRIC_MODELS )
				return true;
			else
				return false;
	
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::ostream& operator<<(std::ostream& output, const CopyModelDefinition &in_CopyModelDefinition)
	{
		output << "From Model: " << in_CopyModelDefinition.fromModelName << "  To Model: " << in_CopyModelDefinition.toModelName;

		output << "  Model Type: " << isis::ProMdlType_string(in_CopyModelDefinition.modelType) << "  ComponentInstanceID: "  << in_CopyModelDefinition.componentInstanceID;

		//if ( in_CopyModelDefinition.modelType == PRO_MDL_PART )
		//	output << "  Part";
		//else
		//	output << "  Assembly";

		return output;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::ostream& operator<<(std::ostream& output, const std::vector<CopyModelDefinition> &in_CopyModelDefinition_vector)
	{
		for each ( CopyModelDefinition i in in_CopyModelDefinition_vector )
		{
			output << std::endl << i;
		}

		return output;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void ModifyToHaveAUniqueName_ForEach_PartAndOrAssembly( 
							unsigned int									&in_out_UniqueNameIndex,
							e_ModelTypeIndicator							in_ModelTypeIndicator,
							e_ModelSelectorIndicator						in_ModelSelectorIndicator,
							bool											in_ForceAllParametricModelsToBeUnique,
							std::map<std::string, isis::CADComponentData>	&in_out_CADComponentData_map, 
							std::vector<CopyModelDefinition>				&out_FromModel_ToModel  )
																		throw (isis::application_exception)
	{
		
		std::set<std::string>  modelsAlreadyEncountered;

		//int index = 1;
		//int maxPartNameLength_BeforeSuffix = PRO_NAME_SIZE - 8;  //  32 - 8 = 24

		char buffer[isis::ISIS_CHAR_BUFFER_LENGTH];

		for( std::map<std::string, isis::CADComponentData>::iterator i(in_out_CADComponentData_map.begin());
			 i != in_out_CADComponentData_map.end();
			 ++i )
		{
			std::string			origNameWithoutFamilyEntry_temp;  
			std::string		    familyTableEntry;				  
			bool				familyTableModel;

			// e.g. i->second.name							123456789012345<abcd>
			//		origNameWithoutFamilyEntry_temp			abcd
			//		familyTableEntry						123456789012345
			//		familyTableModel						true
			ExtractModelName_FamilyTable_Info (	i->second.name, 
												origNameWithoutFamilyEntry_temp,
												familyTableEntry,
												familyTableModel );

			if ( i->second.name.size() == 0 )
			{
				std::stringstream errorString;
				errorString 
				<< "Function - " __FUNCTION__ << ", recieved an empty Creo Model Name for ComponentInstanceID: " << i->first << std::endl <<
				    ".  This is probably due to the ComponentInstanceID being referenced in the input XML file (CADAssembly.xml) but does not exists as a component in the input XML file." << std::endl <<
					"Search on " <<  i->first  << " in the input XML file to locate the error.";
				throw isis::application_exception("C03002", errorString);
			}

			std::string modelNameWithSuffix = 
				ConvertToUpperCase(CombineCreoModelNameAndSuffix(origNameWithoutFamilyEntry_temp, i->second.modelType) );

			//std::cout << std::endl << "############## ModifyToHaveAUniqueName_ForEach_PartAndOrAssembly, modelNameWithSuffix, ComponentInstanceID: " <<  modelNameWithSuffix << "  " << i->second.componentID;

			if ( in_ForceAllParametricModelsToBeUnique )
			{
				// We want to force all parametric parts/assemblies to have a suffix.  This is because we don't know if that 
				// parametric part or assembly was used in a CyPhy leaf assemblies.  
				if ( SelectModelIndicated (in_ModelSelectorIndicator, i->second.parametricParametersPresent) && i->second.parametricParametersPresent )
				{
					modelsAlreadyEncountered.insert(modelNameWithSuffix);
				}
			}

			// Not creating unique names for family table entries now because of the
			// error described in 
			// C:\Users\rowens\Documents\Meta\Error_Models\IFV Complete Assembly\Original_Files\0_Readme.txt
			// Once this error is resolved, will remove the !familyTableModel from the following if statement

			// Check for parametric parts
			if ( !familyTableModel &&  // Temporaily exclude family table models, see comment above for info on the family table bug.
				 SelectModelIndicated (in_ModelSelectorIndicator, i->second.parametricParametersPresent) && 
				 ModelTypesMatch(in_ModelTypeIndicator, i->second.modelType) &&
				 ( modelsAlreadyEncountered.find(modelNameWithSuffix) !=  modelsAlreadyEncountered.end()))  // Part/Assembly occurs a second time
			{
				std::string origNameWithoutFamilyEntry;
				std::string modelName;
				std::string completeName;
				CreateModelNameWithUniqueSuffix(	in_out_UniqueNameIndex, 
													i->second.name,
													origNameWithoutFamilyEntry,
													modelName, 
													completeName );
				i->second.name = completeName;

				CopyModelDefinition copyModelDefinition_temp;
				copyModelDefinition_temp.componentInstanceID = i->first;
				copyModelDefinition_temp.fromModelName = origNameWithoutFamilyEntry;
				copyModelDefinition_temp.toModelName = modelName;
				copyModelDefinition_temp.modelType = i->second.modelType;
				out_FromModel_ToModel.push_back(copyModelDefinition_temp);

				//std::cout << std::endl << copyModelDefinition_temp;

				++in_out_UniqueNameIndex;
			}
			isis_LOG(lg, isis_FILE, isis_INFO) << "ModelsAlreadyEncountered.insert: " << modelNameWithSuffix;
			modelsAlreadyEncountered.insert(modelNameWithSuffix);
		}
	}	// END ModifyToHaveAUniqueName_ForEach_PartAndOrAssembly	 

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void VisitComponents(	const std::string								&in_ComponentID, 
							std::map<std::string, isis::CADComponentData>	&in_out_CADComponentData_map,
							ComponentVistor									&in_componentVistor) 
																		throw (isis::application_exception) 
	{
		// This must always be first, so that the first entry (i.e. top-assembly can be include/excluded)
		// See ComponentVistorBuildListOfComponentIDs::operator()
		in_componentVistor(	in_ComponentID, in_out_CADComponentData_map);

		if ( in_out_CADComponentData_map[in_ComponentID].modelType == PRO_MDL_ASSEMBLY )
		{
			for ( std::list<string>::const_iterator i(in_out_CADComponentData_map[in_ComponentID].children.begin());
				i != in_out_CADComponentData_map[in_ComponentID].children.end();
				++i )
			{
				VisitComponents( *i,in_out_CADComponentData_map, in_componentVistor);
			}
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void ComponentVistorMaterialTokens::operator() ( const std::string  &in_ComponentID, 
									  std::map<std::string, isis::CADComponentData> &in_out_CADComponentData_map )
	{
		if ( in_out_CADComponentData_map[in_ComponentID].modelType == PRO_MDL_PART )
		{
			if ( in_out_CADComponentData_map[in_ComponentID].analysisTempMaterialDefined )
			{
				AnalysisTempMaterial analysisTempMaterialDefinition = in_out_CADComponentData_map[in_ComponentID].analysisTempMaterialDefinition;
				componentID_PoissonsRatio_map[in_ComponentID] = analysisTempMaterialDefinition.poissonsRatio;
				materialKey_ComponentID_map[analysisTempMaterialDefinition.tokenPoissonsRatio] = in_ComponentID;
			}
			else
			{
				std::string errorString;
				errorString = "exception : Function - " + std::string(__FUNCTION__) +  " Functor, called from ReplaceMaterialTokens_ReturnMaterialToComponentID), material properties not defined for component Instance ID: " + in_ComponentID;
				throw isis::application_exception("C03003", errorString.c_str());	
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ComponentVistorCountAssemblyComponents::ComponentVistorCountAssemblyComponents():numberOfComponents(0){};

	void ComponentVistorCountAssemblyComponents::operator() ( const std::string  &in_ComponentID, 
									  std::map<std::string, isis::CADComponentData> &in_out_CADComponentData_map )
	{
		if ( in_out_CADComponentData_map[in_ComponentID].children.size() == 0 ) ++numberOfComponents; // This is a part or a leaf assembly

	}

	SelectAllComponents SelectAllComponentsDefault; 


	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ComponentVistorBuildListOfComponentIDs::ComponentVistorBuildListOfComponentIDs(bool excludeTopAssembly) : flagExcludeEntry(excludeTopAssembly), predicate(SelectAllComponentsDefault) {};
	ComponentVistorBuildListOfComponentIDs::ComponentVistorBuildListOfComponentIDs(ComponentPredicate &in_Predicate, bool in_ExcludeTopAssembly) : predicate(in_Predicate), flagExcludeEntry(in_ExcludeTopAssembly) {};

	void ComponentVistorBuildListOfComponentIDs::operator() ( const std::string  &in_ComponentID, 
									  std::map<std::string, isis::CADComponentData> &in_out_CADComponentData_map )
	{
		if ( flagExcludeEntry )
			flagExcludeEntry = false;  // This will skip the first entry which would be the top-assembly component ID.
		else
			if ( predicate(in_out_CADComponentData_map[in_ComponentID] ) ) listOfComponentIDs.push_back(in_ComponentID);		
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	 bool SelectAllComponents::operator() ( const isis::CADComponentData &in_CADComponentData)
	{
		return true;
	};

	 bool SelectComponentDerivedFromLeafAssembly::operator() ( const isis::CADComponentData &in_CADComponentData)
	{
		if ( in_CADComponentData.dataInitialSource == INITIAL_SOURCE_DERIVED_FROM_LEAF_ASSEMBLY_DESCENDANTS )
			return true;
		else 
			return false;
	};

	bool SelectComponentInInputXML::operator() ( const isis::CADComponentData &in_CADComponentData)
	{
		if ( in_CADComponentData.dataInitialSource == INITIAL_SOURCE_INPUT_XML_FILE )
			return true;
		else 
			return false;
	};

	bool SelectLeafAssemblies::operator() ( const isis::CADComponentData &in_CADComponentData)
	{
		if ( in_CADComponentData.dataInitialSource == INITIAL_SOURCE_INPUT_XML_FILE && in_CADComponentData.modelType == PRO_MDL_ASSEMBLY )
			return true;
		else 
			return false;
	};
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*
	std::string GetDayMonthTimeYear()
	{
		time_t time_start;		// calendar time 
		time_start=time(NULL);	// get current cal time 

		std::string dayMonthTimeYear = asctime(localtime(&time_start));
		if (dayMonthTimeYear.size() > 0 )
		{
			// Remove the extra linefeed from the above string.
			dayMonthTimeYear.replace(dayMonthTimeYear.end() -1, dayMonthTimeYear.end(), "");

		}
		return dayMonthTimeYear;
	}
	*/
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void OrganizeMetricsBasedOnComponentIDs( 
							const list<CADComputation>							&in_Metrics,
							std::map<std::string, std::list<CADComputation>>	&out_componentID_to_ListofComputations_map,
							std::set<std::string>								&out_ComponentIDs_set )
	{											 
			for each ( const CADComputation i in in_Metrics)
			{
				out_componentID_to_ListofComputations_map[i.componentID].push_back(i);
				out_ComponentIDs_set.insert(i.componentID);
			}
	}

//	ComponentVistorBuildListOfBoundingBoxComputations::ComponentVistorBuildListOfBoundingBoxComputations(){};
//
//	void ComponentVistorBuildListOfBoundingBoxComputations::operator() ( const std::string  &in_ComponentID, 
//									  std::map<std::string, isis::CADComponentData> &in_out_CADComponentData_map )
//	{
//		if ( in_out_CADComponentData_map[in_ComponentID].cADComputations.boundingBoxMetricDefined ) 
//			boundingBoxComputationsComponentIDs.push_back( in_ComponentID );
//
//	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void ForEachLeafAssemblyInTheInputXML_AddInformationAboutSubordinates( 
					const std::vector<std::string>					&in_ListOfComponentIDsInTheAssembly, // This includes the assembly component ID
					int												&in_out_NonCyPhyID_counter,
					std::map<std::string, isis::CADComponentData>	&in_out_CADComponentData_map )
																		throw (isis::application_exception)
	{
		
		for each ( const std::string &i in in_ListOfComponentIDsInTheAssembly )
		{			
			if ( in_out_CADComponentData_map[i].modelType == PRO_MDL_ASSEMBLY && in_out_CADComponentData_map[i].children.size() == 0 )
			{
				// Found an assembly that is a Leaf
				// Fill out the assemblyHierarchy
				CreoModelAssemblyAttributes assemblyHierarchy;

				isis::RetrieveAssemblyHierarchyInformation(  in_out_CADComponentData_map[i].modelHandle, false, assemblyHierarchy );

				std::stringstream str;
				stream_AssemblyHierarchy (assemblyHierarchy, str);
				isis_LOG(lg, isis_FILE, isis_INFO) << str.str();

				bool checkExclusion_by_SimplifiedRep = false;

				std::map<int, CAD_SimplifiedRepData> featureID_to_SimplifiedRepData_map;
				if ( in_out_CADComponentData_map[i].modelType == PRO_MDL_ASSEMBLY && 
					 in_out_CADComponentData_map[i].geometryRepresentation.size() > 0 )  
				{
					///////////////////////////////////////////////////////////////////////////////////////
					// Build map of child feature IDs and with indication if they are included or excluded
					///////////////////////////////////////////////////////////////////////////////////////
					ProSimprep proSimprep_temp;
					ProError	proError_temp  = ProSimprepInit ((wchar_t*)(const wchar_t*) in_out_CADComponentData_map[i].geometryRepresentation,
														  -1,
														   in_out_CADComponentData_map[i].modelHandle,
														  &proSimprep_temp );

					if ( proError_temp == PRO_TK_NO_ERROR )  // Found simplified rep.
					{
						ProMdl ProMdl_temp = in_out_CADComponentData_map[i].modelHandle;
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
						cADComponentData_temp.modelHandle = j.p_solid_handle;
						cADComponentData_temp.cyPhyComponent = false;
						cADComponentData_temp.componentID = nonCyPhyComponentID.str();
						cADComponentData_temp.parentComponentID = i;
						cADComponentData_temp.assembledFeature = j.proAsmcomp;
						
						cADComponentData_temp.componentPaths = in_out_CADComponentData_map[i].componentPaths;
						cADComponentData_temp.componentPaths.push_back( j.proAsmcomp.id );					

						in_out_CADComponentData_map[nonCyPhyComponentID.str()] = cADComponentData_temp;
						in_out_CADComponentData_map[i].children.push_back(nonCyPhyComponentID.str());

						if ( j.modelType == PRO_MDL_ASSEMBLY )
						{
								std::vector<std::string> listOfComponentIDs_temp;
								listOfComponentIDs_temp.push_back(nonCyPhyComponentID.str());
								ForEachLeafAssemblyInTheInputXML_AddInformationAboutSubordinates(
																				listOfComponentIDs_temp,
																				in_out_NonCyPhyID_counter,
																				in_out_CADComponentData_map );
						}
					} // END if ( includeThisChild )
				}
			}
		}

	}  // END ForEachLeafAssemblyInTheInputXML_AddInformationAboutSubordinates
	
	void RetrieveNameOfAssembledFeature( ProFeature	*in_AssyCompFeature, isis::MultiFormatString &out_ModelName )
	{
		ProElement component_element_tree;
		isis::isis_ProFeatureElemtreeExtract(in_AssyCompFeature, NULL, PRO_FEAT_EXTRACT_NO_OPTS, &component_element_tree);

		ProElempath elemPath;
		ProElempathAlloc(&elemPath);
		ProElempathItem pathItem[1]; 
		pathItem[0].type = PRO_ELEM_PATH_ITEM_TYPE_ID;
		pathItem[0].path_item.elem_id = PRO_E_COMPONENT_MODEL;

		isis::isis_ProElempathDataSet(elemPath, pathItem, 1);

		ProElement component_model_elem;
		isis::isis_ProElemtreeElementGet(component_element_tree, elemPath, &component_model_elem);
		isis::isis_ProElempathFree(&elemPath);

		ProValue  val_component_model;
		isis::isis_ProElementValueGet(component_model_elem, &val_component_model );
		ProValueData value_data;
		isis::isis_ProValueDataGet(val_component_model, &value_data);

		ProMdl	modelHandle = value_data.v.p;

		ProName  modelName_wide;

		isis::isis_ProMdlNameGet(modelHandle, modelName_wide);

		out_ModelName = modelName_wide;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Description:
	//		This function renames a model in the assembly tree.  The analogous Creo UI functionality is “right-click on a model 
	//		in the model tree, select “By Copy”, and  follow prompts”.  Note, the UI function does not rename assemblies; however, 
	//		it is imperative that we are able to rename assemblies.  This function is intended to work on .prts and .ams.
	//
	//		Note - This function was supplied by PTC support.
	//
	//	Pre-Conditions:
	//		1.	in_ToModelName must not exists in the assembly. 
	//		2.	in_AssyCompFeature must point to a valid assembly feature where the assembly feature is for a .prt or .asm.
	//
	//	Post-Conditions:
	//		If the model rename fails, then typically, isis::application_exception would be thown; however, there have been cases 
	//		where the model was not successfully renamed and an exception was not thrown. Subsequent to this function invocation 
	//		there should be a function that validates that the rename worked properly. 
	ProError ReplaceCompFeatwithNewModel( ProFeature	*in_AssyCompFeature, 
										  ProName		in_ToModelName,
										  ProMdl		&out_FromModelHandle,
										  ProMdl		&out_ToModelHandle)
												throw (isis::application_exception)
	{
		ProMdl existing_model;
		ProErr err;
		err = ProAsmcompMdlGet(in_AssyCompFeature, &existing_model);

		ProMdl replacement_model;	
		isis::isis_ProMdlCopy(existing_model, in_ToModelName, &replacement_model);
		out_ToModelHandle= replacement_model;

		ProElement component_element_tree;
		isis::isis_ProFeatureElemtreeExtract(in_AssyCompFeature, NULL, PRO_FEAT_EXTRACT_NO_OPTS, &component_element_tree);

		ProElempath elemPath;
		ProElempathAlloc(&elemPath);
		ProElempathItem pathItem[1]; 
		pathItem[0].type = PRO_ELEM_PATH_ITEM_TYPE_ID;
		pathItem[0].path_item.elem_id = PRO_E_COMPONENT_MODEL;

		isis::isis_ProElempathDataSet(elemPath, pathItem, 1);

		ProElement component_model_elem;
		isis::isis_ProElemtreeElementGet(component_element_tree, elemPath, &component_model_elem);
		isis::isis_ProElempathFree(&elemPath);

		ProValue  val_component_model;
		isis::isis_ProElementValueGet(component_model_elem, &val_component_model );
		ProValueData value_data;
		isis::isis_ProValueDataGet(val_component_model, &value_data);

		out_FromModelHandle = value_data.v.p;

		value_data.v.p = replacement_model;
		isis::isis_ProValueDataSet( val_component_model, &value_data );
		isis::isis_ProElementValueSet( component_model_elem, val_component_model );

		ProModelitem mdlItem;
		isis::isis_ProMdlToModelitem(in_AssyCompFeature->owner, &mdlItem);
		ProSelection mdl_sel;
		ProSelectionAlloc(NULL, &mdlItem, &mdl_sel);
	
		ProErrorlist errors;	
		ProFeatureCreateOptions *opts;  
		isis::isis_ProArrayAlloc(1, sizeof (ProFeatureCreateOptions), 1, (ProArray*) &opts);    
		opts[0] = PRO_FEAT_CR_DEFINE_MISS_ELEMS;

		isis::isis_ProFeatureWithoptionsRedefine(NULL, in_AssyCompFeature, component_element_tree, opts, PRO_REGEN_FORCE_REGEN, &errors);

		isis::isis_ProElempathFree(&elemPath);
		isis::isis_ProSelectionAlloc(NULL, &mdlItem, &mdl_sel);

		return err;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void  Assembly_RenameSubPartOrSubAssembly(	ModelInstanceData	&in_FromModelInstanceData,  // Not constant because ProFeatureElemtreeExtract does not declare input only functions as constant
												MultiFormatString	&in_ToModelName,
												ProMdl				&out_RenamedModelHandle)
					throw (isis::application_exception)
	{
		
		isis_LOG(lg, isis_FILE, isis_INFO) << "\n ***** InAnAssembly_RenamePartOrAssemblyInstance: ";
		isis_LOG(lg, isis_FILE, isis_INFO) << "   topAssemblyModelHandle " << in_FromModelInstanceData.topAssemblyModelHandle;
		isis_LOG(lg, isis_FILE, isis_INFO) << "   in_FromModelName:      " << in_FromModelInstanceData.modelName;
		isis_LOG(lg, isis_FILE, isis_INFO) << "   model type:            " << ProMdlType_string(in_FromModelInstanceData.modelType);
		isis_LOG(lg, isis_FILE, isis_INFO) << "   modelHandle:           " << in_FromModelInstanceData.modelHandle;
		isis_LOG(lg, isis_FILE, isis_INFO) << "   in_ToModelName:        " << in_ToModelName;
		isis_LOG(lg, isis_FILE, isis_INFO) << "   From Model assembledFeature: ";
		isis_LOG(lg, isis_FILE, isis_INFO) << "        id:    "  << in_FromModelInstanceData.assembledFeature.id;
		isis_LOG(lg, isis_FILE, isis_INFO) << "        owner: "  << in_FromModelInstanceData.assembledFeature.owner;
		isis_LOG(lg, isis_FILE, isis_INFO) << "        type: "   << FeatureGeometryType_string(in_FromModelInstanceData.assembledFeature.type);

		/////////////////////////////////////
		// Check Validity of input data
		/////////////////////////////////////
		if ( in_FromModelInstanceData.modelName.size() == 0 )
		{
			std::stringstream errorString;
			errorString 
				<< "Function - InAnAssembly_RenamePartOrAssemblyInstance, recieve an empty in_FromModelInstanceData.modelName string.  See log file for additional information."; 	  
			throw isis::application_exception("C03002", errorString);
		}

		if ( in_ToModelName.size() == 0 )
		{
			std::stringstream errorString;
			errorString 
				<< "Function - InAnAssembly_RenamePartOrAssemblyInstance, recieve an empty in_ToModelName string.  See log file for additional information."; 	  
			throw isis::application_exception("C03002", errorString);
		}

		if ( in_FromModelInstanceData.modelType != PRO_MDL_PART && 
			 in_FromModelInstanceData.modelType != PRO_MDL_ASSEMBLY )

		{
			std::stringstream errorString;
			errorString 
				<< "Function - InAnAssembly_RenamePartOrAssemblyInstance, recieve a model type that was not PRO_MDL_PART or PRO_MDL_ASSEMBLY.  See log file for additional information."; 	  
			throw isis::application_exception("C03002", errorString);
		}
		//// Test 
		
		isis_LOG(lg, isis_FILE, isis_INFO) << "in_FromModelInstanceData.assembledFeature.owner: " << in_FromModelInstanceData.assembledFeature.owner;
		isis_LOG(lg, isis_FILE, isis_INFO) << "in_FromModelInstanceData.assembledFeature.id:    " << in_FromModelInstanceData.assembledFeature.id;
		isis_LOG(lg, isis_FILE, isis_INFO) << "in_FromModelInstanceData.assembledFeature.type:  " << in_FromModelInstanceData.assembledFeature.type;		
		
		//ProMdl outModifiedMdl;
		 ProName newName;
		 wcscpy_s(newName, 32, (wchar_t*)(const wchar_t*)in_ToModelName);

		 ProMdl fromModelHandle;

		 ReplaceCompFeatwithNewModel(	&in_FromModelInstanceData.assembledFeature, 
										newName,
										fromModelHandle,
										out_RenamedModelHandle );



		if ( fromModelHandle != in_FromModelInstanceData.modelHandle )
		{
			std::stringstream errorString;
			errorString << "Assembly_RenameSubPartOrSubAssembly - This function renames a model; however this program’s internal data structures for the address of the rename-from model ";
			errorString << std::endl <<	"do not agree with the Creo data structures for the rename-from model.  These addresses must be the same. ";
			errorString << std::endl <<	"This is due to a software error; and therefore, there is not a change you can make that would correct this. ";
			errorString << std::endl <<	"Please report this error to the support desk.";
			throw isis::application_exception("C03002", errorString);
		}


		// Verify that the modified assembled feature points to a model with the name in_ToModelName.  This provides an
		// indication if the rename was/was-not successful.  In some cases the function ReplaceCompFeatwithNewModel will 
		// not complete the rename and not return an error.  
		isis::MultiFormatString assembledFeatureModelName;
		RetrieveNameOfAssembledFeature( &in_FromModelInstanceData.assembledFeature, assembledFeatureModelName );	

		if ( !boost::iequals((const string)in_ToModelName, (const string) assembledFeatureModelName) )
		{
			std::stringstream errorString;
			errorString << "Assembly_RenameSubPartOrSubAssembly - Failed to rename model to: " << in_ToModelName;
			throw isis::application_exception("C03002", errorString);
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void	RetrieveBoundingBox_ComputeFirstIfNotAlreadyComputed( 
								const std::string								&in_ComponentInstanceID,
								std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map,
								isis_CADCommon::Point_3D						&out_BoundingBox_Point_1,
								isis_CADCommon::Point_3D						&out_BoundingBox_Point_2,
								double											out_Dimensions_xyz[3] )
																		throw (isis::application_exception)
	{

		

		try
		{
			if ( !in_CADComponentData_map[in_ComponentInstanceID].boundingBox.boundingBox_Defined )
			{

				try
				{
					ComputeBoundingBox(		CAD_PRO_SOLID_OUTLINE_COMPUTE,
											in_CADComponentData_map[in_ComponentInstanceID].modelHandle,
											in_CADComponentData_map[in_ComponentInstanceID].boundingBox.boundingBox_Point_1,
											in_CADComponentData_map[in_ComponentInstanceID].boundingBox.boundingBox_Point_2,
											in_CADComponentData_map[in_ComponentInstanceID].boundingBox.Dimensions_xyz);
				}
				catch (...)
				{
					isis_LOG(lg, isis_FILE, isis_INFO) << "";
					isis_LOG(lg, isis_FILE, isis_INFO) << "WARNING - Component Instance ID: " << in_ComponentInstanceID << ".  Model Name: " << 
					in_CADComponentData_map[in_ComponentInstanceID].name << ".  isis_ProSolidOutlineCompute failed, using isis_ProSolidOutlineGet.  isis_ProSolidOutlineGet is less accurate."; 
					isis_LOG(lg, isis_FILE, isis_INFO) << "";

					ComputeBoundingBox(		CAD_PRO_SOLID_OUTLINE_GET,
											in_CADComponentData_map[in_ComponentInstanceID].modelHandle,
											in_CADComponentData_map[in_ComponentInstanceID].boundingBox.boundingBox_Point_1,
											in_CADComponentData_map[in_ComponentInstanceID].boundingBox.boundingBox_Point_2,
											in_CADComponentData_map[in_ComponentInstanceID].boundingBox.Dimensions_xyz);

				} 


				in_CADComponentData_map[in_ComponentInstanceID].boundingBox.boundingBox_Defined = true;

			}
	
			out_BoundingBox_Point_1 = in_CADComponentData_map[in_ComponentInstanceID].boundingBox.boundingBox_Point_1;
			out_BoundingBox_Point_2 = in_CADComponentData_map[in_ComponentInstanceID].boundingBox.boundingBox_Point_2;

			out_Dimensions_xyz[0] = in_CADComponentData_map[in_ComponentInstanceID].boundingBox.Dimensions_xyz[0];
			out_Dimensions_xyz[1] = in_CADComponentData_map[in_ComponentInstanceID].boundingBox.Dimensions_xyz[1];
			out_Dimensions_xyz[2] = in_CADComponentData_map[in_ComponentInstanceID].boundingBox.Dimensions_xyz[2];
		}
		catch ( isis::application_exception& ex )
		{
			
			// Get Mass properties and check for zero volume, this would indicate that there was no solid geometry.  Probably just
			// reference geometry used to define an interface.  For example, an interface between a hatch and hull.
			ProMassProperty  mass_prop;

			isis_ProSolidMassPropertyGet_WithDescriptiveErrorMsg(in_ComponentInstanceID, in_CADComponentData_map, &mass_prop );

			//std::cout << std::endl << "-------------> Volume: mass_prop.volume" << mass_prop.volume << std::endl;

			if ( mass_prop.volume == 0.0 )
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
					in_CADComponentData_map[in_ComponentInstanceID].name << ".  Model has zero volume, setting bounding box to size of zero."; 
				isis_LOG(lg, isis_FILE, isis_INFO) << "";
			}
			else
			{
				std::stringstream errorString;
					errorString <<
							"Failed to retrieve bounding box information:"  << std::endl <<
							"   Model Name:            " <<	 in_CADComponentData_map[in_ComponentInstanceID].name << std::endl <<
							"   Model Type:            " << isis::ProMdlType_string(in_CADComponentData_map[in_ComponentInstanceID].modelType)<<  std::endl <<
							"   Component Instance ID: " <<  in_ComponentInstanceID <<  std::endl <<
							"   Exception Message: " << ex.what();
					throw isis::application_exception("C05002",errorString.str().c_str());
			}
		}

	}

	// e.g. "C:\\Users\\Public\\Documents\\META Documents\\MaterialLibrary\\MATERIALS_CREO_MTL"
	std::string CreoMaterialMTLFilesDir_Path()
	{
		std::wstring publicDocuments_wide;

		// Public Documents e.g. "C:\\Users\\Public\\Documents\\META Documents"
		isis_CADCommon::GetPublicDocuments(publicDocuments_wide);
		isis::MultiFormatString publicDocuments_multiformat(publicDocuments_wide.c_str() );

		// Materials MTD Dir
		// e.g. "C:\\Users\\Public\\Documents\\META Documents\\MaterialLibrary\\MATERIALS_CREO_MTL"

		return (std::string)publicDocuments_multiformat + std::string("\\META Documents\\MaterialLibrary\\MATERIALS_CREO_MTL");		;
	}

	std::string META_PATH()
	{
		std::string metapath;
		HKEY software_meta;
		if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "Software\\META", 0, KEY_READ, &software_meta) == ERROR_SUCCESS)
		{
			BYTE data[MAX_PATH];
			DWORD type, size = sizeof(data) / sizeof(data[0]);
			if (RegQueryValueExA(software_meta, "META_PATH", 0, &type, data, &size) == ERROR_SUCCESS)
			{
				metapath = std::string(data, data + strnlen((const char*)data, size));
			}
			RegCloseKey(software_meta);
		}
		return metapath;
	}

	// e.g "C:\\Users\\Public\\Documents\\META Documents\\MaterialLibrary\\material_library.json"
	std::string MaterialsLibrary_PathAndFileName()
	{
		auto dev_path = boost::filesystem::path(META_PATH()) / "models" / "MaterialLibrary" / "material_library.json";
		OutputDebugStringA(dev_path.string().c_str());
		OutputDebugStringA("\n");
		if (boost::filesystem::is_regular_file(dev_path))
		{
			return dev_path.string();
		}

		std::wstring publicDocuments_wide;

		// Public Documents e.g. "C:\\Users\\Public\\Documents\\META Documents"
		isis_CADCommon::GetPublicDocuments(publicDocuments_wide);
		isis::MultiFormatString publicDocuments_multiformat(publicDocuments_wide.c_str() );

		// Materials Library
		// e.g. "C:\\Users\\Public\\Documents\\META Documents\\MaterialLibrary\\material_library.json"

		return (std::string)publicDocuments_multiformat + std::string("\\META Documents\\MaterialLibrary\\material_library.json");	;
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ValidatePathAndModelItem_ThrowExceptionIfInvalid( ProAsmcomppath	&in_Path, ProModelitem  &in_ModelItem )
		throw (isis::application_exception)
{
	// in_base_comp_path should point to the same model as in_base_model_datum.owner

	ProMdl         p_ModelPathModel;

	isis_ProAsmcomppathMdlGet( &in_Path, &p_ModelPathModel);

	if ( p_ModelPathModel != in_ModelItem.owner )
	{
		std::stringstream errorString;
		errorString << "ValidatePathAndModelItem_ThrowExceptionIfInvalid, Component-path (i.e. ProAsmcomppath) model is not the same as ";
		errorString << std::endl <<	"the model-item (i.e. ProModelitem) model.  They must be the same. This is due to a software error; and therefore,  ";
		errorString << std::endl <<	"there is not a change you can make that would correct this. Please report this error to the support desk.";

		errorString <<  std::endl <<  std::endl << "Path: ";
				errorString <<  std::endl << "   Path Top Model: " << in_Path.owner;
		errorString <<  std::endl << "   Path (Feature IDs): ";
		errorString <<  std::endl << "      Number of Feature IDs in the path: " << in_Path.table_num;

		if ( in_Path.table_num > -1 &&  in_Path.table_num <= PRO_MAX_ASSEM_LEVEL )
		{

			ProAsmcomppath   proAsmcomppath_temp;
			proAsmcomppath_temp.owner						=	in_Path.owner;
			proAsmcomppath_temp.table_num					=	in_Path.table_num;
			for (int i = 0; i < in_Path.table_num; ++i )		proAsmcomppath_temp.comp_id_table[i] = in_Path.comp_id_table[i];

			for ( int i = 0; i < in_Path.table_num; ++i )
			{
				errorString << std::endl << "      Path Feature ID: " <<  in_Path.comp_id_table[i] << "   Model Name: ";
				// Get the model name to this point in the path

				proAsmcomppath_temp.table_num = i + 1;

				try
				{
					ProMdl		p_ModelPathModel_temp;
					isis_ProAsmcomppathMdlGet( &proAsmcomppath_temp, &p_ModelPathModel_temp);

					ProName modelPathName_temp;
					isis::isis_ProMdlNameGet ( p_ModelPathModel_temp, modelPathName_temp );
					isis::MultiFormatString modelPathName_multiFormat_temp(modelPathName_temp);
					errorString << (std::string) modelPathName_multiFormat_temp;
				}
				catch (...)
				{
					errorString <<  "Could not retrieve model name based on the path.";
				}
			}
		}
		else
		{
			errorString << std::endl << "   Number of feature IDs in the path is not in the range of 0 to " << PRO_MAX_ASSEM_LEVEL << ".";
		}

		errorString <<  std::endl <<  std::endl << "Model Item: ";
		errorString <<  std::endl << "   Model Item Owner:         " << in_ModelItem.owner;
		errorString <<  std::endl << "   Model Item Feature ID:    " << in_ModelItem.id;
		errorString <<  std::endl << "   Model Item Type (1-Assembly, 2-Part, 5-Surface, 21-Axis, 66-Point): " << in_ModelItem.type;
		errorString <<  std::endl;
	
		try
		{
			ProName modelPathName;
			isis::isis_ProMdlNameGet ( p_ModelPathModel, modelPathName );
			isis::MultiFormatString modelPathName_multiFormat(modelPathName);
			errorString <<  std::endl << "Path model name:        "  << (std::string) modelPathName_multiFormat;
			errorString <<  std::endl << "Path model address:     " <<  p_ModelPathModel;
		}
		catch (...)
		{
			errorString <<  std::endl << "Could not retrieve the model-item model name";
		}

		ProName modelItemName;
		try
		{
			ProName modelItemName;
			isis::isis_ProMdlNameGet ( in_ModelItem.owner, modelItemName );
			isis::MultiFormatString modelItemName_multiFormat(modelItemName);
			errorString <<  std::endl <<  std::endl << "Model-Item model name:     " << (std::string) modelItemName_multiFormat;
			
			ProName modelItemFeatureName;
			isis::isis_ProModelitemNameGet ( &in_ModelItem, modelItemFeatureName );
			isis::MultiFormatString modelItemFeatureName_multiFormat(modelItemFeatureName);
			errorString <<  std::endl <<  "Model-Item feature name:   " << (std::string) modelItemFeatureName_multiFormat;
			errorString <<  std::endl <<  "Model-Item owner address:  " <<  in_ModelItem.owner;
		}
		catch (...)
		{
			errorString <<  std::endl << "Could not retrieve the model-item model name";
		}
		throw isis::application_exception("C04002",errorString.str().c_str());		
	}


}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	e_CADJointType GetCADJointType( isis::cad::JointType in_JointType)
	{
		switch (in_JointType )
		{
			case  isis::cad::FIXED:
				return FIXED_JOINT;
				break;
			case  isis::cad::REVOLUTE:
				return REVOLUTE_JOINT;
				break;
			case  isis::cad::UNIVERSAL:
				return UNIVERSAL_JOINT;
				break;
			case  isis::cad::SPHERICAL:
				return SPHERICAL_JOINT;
				break;
			case  isis::cad::PRISMATIC:
				return PRISMATIC_JOINT;
				break;
			case  isis::cad::CYLINDRICAL:
				return CYLINDRICAL_JOINT;
				break;
			case  isis::cad::PLANAR:
				return PLANAR_JOINT;
				break;
			case  isis::cad::FREE:
				return FREE_JOINT;
				break;
			default:
				return UNKNOWN_JOINT_TYPE;
		}
	};

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	void PopulateMap_with_JunctionInformation_SingleJunction( 
					cad::CadFactoryAbstract							&in_factory,
					const std::string								&in_ComponentID, 
					const std::vector<ConstraintPair>				&in_ConstraintPairs,
					isis::cad::Junction								&out_Junction,
					std::map<std::string, isis::CADComponentData>	&in_out_CADComponentData_map )
						throw (isis::application_exception)
	{
		

		isis_LOG(lg, isis_FILE, isis_INFO) << "*************  PopulateMap_with_JunctionInformation_SingleJunction (Constraints derived from CADAssembly.xml)";

		isis_LOG(lg, isis_FILE, isis_INFO) << (std::string)in_out_CADComponentData_map[in_ComponentID].name;

		cad::IAssembler& assembler = in_factory.get_assembler();

		std::vector< cad::Joint::pair_t > joint_pair_vector = assembler.extract_joint_pair_vector(in_ComponentID, in_ConstraintPairs, in_out_CADComponentData_map);
		
		for each ( ConstraintPair i in in_ConstraintPairs )
		{
			isis_LOG(lg, isis_FILE, isis_INFO) << "FeatureGeometryType_string(i.featureGeometryType): " << FeatureGeometryType_string(i.featureGeometryType);
			for each ( ConstraintFeature j in i.constraintFeatures )
				isis_LOG(lg, isis_FILE, isis_INFO) << "   FeatureName: "  << (std::string) j.featureName;
		}

		cad::Joint::pair_t inferred_joint = cad::infer_joint_pair(joint_pair_vector);

		isis_LOG(lg, isis_FILE, isis_INFO) << "inferred_joint.first.type:    " << JointType_string(inferred_joint.first.type);
		isis_LOG(lg, isis_FILE, isis_INFO) << "inferred_joint.first.location:" << inferred_joint.first.location.c_str_e20();
		isis_LOG(lg, isis_FILE, isis_INFO) << "inferred_joint.first.orientation:" << inferred_joint.first.orientation.c_str_e20();
		isis_LOG(lg, isis_FILE, isis_INFO) << "inferred_joint.second.type:   " << JointType_string(inferred_joint.second.type);
		isis_LOG(lg, isis_FILE, isis_INFO) << "inferred_joint.second.location:" << inferred_joint.second.location.c_str_e20();
		isis_LOG(lg, isis_FILE, isis_INFO) << "inferred_joint.second.orientation:" << inferred_joint.second.orientation.c_str_e20();


		out_Junction.update(inferred_joint);

		// A type of cad::COMPOSITE means that infer_joint_pair could not determine the actual joint type.  This program
		// should be extended to evaluate the particular set of datums.
		if ( inferred_joint.first.type == cad::COMPOSITE || inferred_joint.second.type == cad::COMPOSITE )
		{
			std::stringstream errorString;
			errorString 
				<< "Function - infer_joint_pair, returned a type of COMPOSITE.  This means that this program " 
				<<	std::endl << "could not infer the joint type given the set of datums; therefore, this program should be "
				<<	std::endl << "extended to handle this case.";

			errorString << std::endl << "   Assembled model name: " << (std::string)in_out_CADComponentData_map[in_ComponentID].name;
			for each( ConstraintPair i in in_ConstraintPairs )
			{
				errorString << std::endl << "      FeatureGeometryType: " << FeatureGeometryType_string(i.featureGeometryType);
				for each ( ConstraintFeature j in i.constraintFeatures ) errorString << std::endl << "         ModelName: " << (string)in_out_CADComponentData_map[j.componentInstanceID].name <<  "  FeatureName: "  << (std::string) j.featureName;
			}
			errorString << std::endl << "   inferred_joint.first: " << inferred_joint.first;
			errorString << std::endl << "   inferred_joint.second: " << inferred_joint.second;

			for each (  cad::Joint::pair_t i in joint_pair_vector ) 
			{
				errorString << std::endl << "   First Joint Primitive:  " << i.first;
				errorString << std::endl << "   Second Joint Primitive: " << i.second;
			}

			for each ( cad::Joint i in inferred_joint.first.composite )
			{
				errorString << std::endl << "   Composite joint: " << i;
			}


			isis_LOG(lg, isis_FILE, isis_WARN) << errorString.str();

			// snyako@isis.vanderbilt.edu : take this error lightly for the time being - otherwise many older model won't work.

			//throw isis::application_exception(errorString);


		}

	}
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void PopulateMap_with_JunctionInformation_SingleJunction( 
					cad::CadFactoryAbstract							&in_factory,
					const CreoAssembledFeatureDefinition			&in_AssembledFeatureDefinition,
					int												in_SetIndex,
					isis::cad::Junction								&out_Junction,
					std::map<std::string, isis::CADComponentData>	&in_out_CADComponentData_map )
						throw (isis::application_exception)
	{
		

		isis_LOG(lg, isis_FILE, isis_INFO) << "*************  PopulateMap_with_JunctionInformation_SingleJunction (Constraints derived from feature tree)";

		isis_LOG(lg, isis_FILE, isis_INFO) << in_AssembledFeatureDefinition;

		isis_LOG(lg, isis_FILE, isis_INFO) << (std::string)in_out_CADComponentData_map[in_AssembledFeatureDefinition.componentInstanceID].name;

		cad::IAssembler& assembler = in_factory.get_assembler();

		std::vector< cad::Joint::pair_t > joint_pair_vector;

		int constraintOffsetCounter = 0;

		for each ( CreoConstraintDefinition i in in_AssembledFeatureDefinition.constraintDefinitions )
		{	
			if ( in_SetIndex == i.component_constr_set_id )  
			{		
				//std::cout << std::endl << "#######: disabledConstraint: " << (i.component_constr_attr & DISABLED_CONSTRAINT_MASK );

				if ( (i.component_constr_attr & DISABLED_CONSTRAINT_MASK ) == DISABLED_CONSTRAINT_MASK ) 
				{					
					isis_LOG(lg, isis_FILE, isis_INFO) << "Constraint disabled.  SetIndex: " <<  i.component_constr_set_id << ", Index within constraint set: " << constraintOffsetCounter << ", Component Instance ID: " <<  in_AssembledFeatureDefinition.componentInstanceID;	
					continue;  
				}
				cad::Joint::pair_t  jointPair;
				// first must always be component_comp_constr_ref
				jointPair.first = cad::creo::extract_marker(i.component_comp_constr_ref);
				jointPair.second = cad::creo::extract_marker(i.component_assem_constr_ref);
				joint_pair_vector.push_back(jointPair);
			}
			++constraintOffsetCounter;
		}
		//std::vector< cad::Joint::pair_t > joint_pair_vector = assembler.extract_joint_pair_vector(in_ComponentID, in_ConstraintPairs, in_out_CADComponentData_map);

		if ( joint_pair_vector.size() == 0 )
		{
			isis_LOG(lg, isis_FILE, isis_INFO) << "All constraints were disabled, SetIndex: " << in_SetIndex;
			isis_LOG(lg, isis_FILE, isis_INFO) << "in_AssembledFeatureDefinition: ";
			isis_LOG(lg, isis_FILE, isis_INFO) << in_AssembledFeatureDefinition;
			return; 
		}

		cad::Joint::pair_t inferred_joint = cad::infer_joint_pair(joint_pair_vector);

		//isis_LOG(lg, isis_FILE, isis_INFO) << "inferred_joint.first.type:    " << JointType_string(inferred_joint.first.type);
		//isis_LOG(lg, isis_FILE, isis_INFO) << "inferred_joint.first.location:" << inferred_joint.first.location.c_str_e20();
		//isis_LOG(lg, isis_FILE, isis_INFO) << "inferred_joint.first.orientation:" << inferred_joint.first.orientation.c_str_e20();
		//isis_LOG(lg, isis_FILE, isis_INFO) << "inferred_joint.second.type:   " << JointType_string(inferred_joint.second.type);
		//isis_LOG(lg, isis_FILE, isis_INFO) << "inferred_joint.second.location:" << inferred_joint.second.location.c_str_e20();
		//isis_LOG(lg, isis_FILE, isis_INFO) << "inferred_joint.second.orientation:" << inferred_joint.second.orientation.c_str_e20();

		out_Junction.update(inferred_joint);

		// A type of cad::COMPOSITE means that infer_joint_pair could not determine the actual joint type.  This program
		// should be extended to evaluate the particular set of datums.
		if ( inferred_joint.first.type == cad::COMPOSITE || inferred_joint.second.type == cad::COMPOSITE )
		{
			std::stringstream errorString;
			errorString 
				<< "Function - infer_joint_pair, returned a type of COMPOSITE.  This means that this program " 
				<<	std::endl << "could not infer the joint type given the set of datums; therefore, this program should be "
				<<	std::endl << "extended to handle this case.";

			errorString << in_AssembledFeatureDefinition;

			errorString << std::endl << "   inferred_joint.first: " << inferred_joint.first;
			errorString << std::endl << "   inferred_joint.second: " << inferred_joint.second;

			for each (  cad::Joint::pair_t i in joint_pair_vector ) 
			{
				errorString << std::endl << "   First Joint Primitive:  " << i.first;
				errorString << std::endl << "   Second Joint Primitive: " << i.second;
			}

			for each ( cad::Joint i in inferred_joint.first.composite )
			{
				errorString << std::endl << "   Composite joint: " << i;
			}

			isis_LOG(lg, isis_FILE, isis_WARN) << errorString.str();

			// snyako@isis.vanderbilt.edu : take this error lightly for the time being - otherwise many older model won't work.

			//throw isis::application_exception(errorString);

		}

	}
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Description:
	//		if in_ProSelection refers to a .prt return the .prt ComponentInstanceID in out_ReferencedPartComponentInstanceID
	// Pre-Conditions:
	//		in_ProSelection must point to a valid ProSelection object.
	// Post-Conditions:
	//		if in_ProSelection points to a .prt
	//			return out_ReferencedPartComponentInstanceID populated with the ComponentInstanceID of the .prt 
	//		if in_ProSelection points to a .asm
	//			return an empty out_ReferencedPartComponentInstanceID
	//		if in_ProSelection does not contain a .prt or .asm that is in in_FeatureIDs_to_ComponentInstanceID_hashtable
	//			throw (isis::application_exception)
	void GetReferencedPartComponentInstanceID ( 
						ProSelection															in_ProSelection, 
						const std::list<int>													&in_OffSetFeatureIDPath_list,
						const std::unordered_map<IntList, std::string, ContainerHash<IntList>>	&in_FeatureIDs_to_ComponentInstanceID_hashtable,
						std::string																&out_ReferencedPartComponentInstanceID )
															throw (isis::application_exception)
	{
		
		

		ProAsmcomppath asmCompPath;
		isis_ProSelectionAsmcomppathGet(in_ProSelection, &asmCompPath );

		ProModelitem  selectionModelItem;
		isis::isis_ProSelectionModelitemGet( in_ProSelection, &selectionModelItem );

		ProMdlType ownerProMdlType;
		isis_ProMdlTypeGet(selectionModelItem.owner, &ownerProMdlType );

		
		if ( ownerProMdlType != PRO_MDL_PART )
		{
			// Don't log this condition.  Parts will be constrained to assemblies, but the real test is if every part
			// is constrained to at least one other part.  That will be checked in AssemblyCreationViaInputFile.cpp for
			// the case where OutputJointInformation == true.
			//try
			//{
			//	ProName ownerModelName_wide;
			//	isis::isis_ProMdlNameGet( selectionModelItem.owner, ownerModelName_wide);
			//	isis::MultiFormatString ownerModelName_wide_multi(ownerModelName_wide);
			//	isis_LOG(lg, isis_FILE, isis_WARN) << "\nWARNING, " << __FUNCTION__ << " was passed a ProSelection with an owner type of: " << isis::ProMdlType_string(ownerProMdlType);
			//	isis_LOG(lg, isis_FILE, isis_WARN) << "         Parent (constrained via the ProSelection ) Model Name: " <<  (std::string)ownerModelName_wide_multi;
			//	isis_LOG(lg, isis_FILE, isis_WARN) << "         Currently only types of PRO_MDL_PART are supported.  This may result in the metrics file having missing joint information.";
			//	isis_LOG(lg, isis_FILE, isis_WARN) << "         If every .prt is constrained to at least one other .prt, then the necessary joint information will be present in the metrics file.";
			//}
			//catch ( isis::application_exception &ex)
			//{
			//	isis_LOG(lg, isis_FILE, isis_ERROR) << "ERROR GetReferencedPartComponentInstanceID failed to retrieve the ModelItem for the owner of a ProSelection";
			//	throw;
			//}
			return;
		}
                     

		// WARNING - Limiting this to paths that have a least one feature ID is fundamentally wrong.  This says
		//			 we can't have a reference to the very top assembly.  Since, for now we, are excluding assemblies
		//			 (i.e. only targeting .prts) this should be ok.
		if ( asmCompPath.table_num > 0 )  // Found a feature ID
		{
			//std::cout << std::endl << "GetReferencedPartComponentInstanceID, Feature IDs";
			std::list<int> featureIDs_list; 
			featureIDs_list = in_OffSetFeatureIDPath_list;  // Start with feature IDs leading to the parent assembly.

			for ( int j = 0; j < asmCompPath.table_num; ++j ) 
			{
				//std::cout << std::endl << "   Feature ID: " << asmCompPath.comp_id_table[j];
				featureIDs_list.push_back(asmCompPath.comp_id_table[j]);
			}

			std::unordered_map<IntList, std::string, ContainerHash<IntList>>::const_iterator itr; 
			itr = in_FeatureIDs_to_ComponentInstanceID_hashtable.find(featureIDs_list);
		
			//std::cout << std::endl << "SSSSS Search for key: "; for each ( int jj in featureIDs_list) std::cout << "  " << jj;
			if ( itr != in_FeatureIDs_to_ComponentInstanceID_hashtable.end() )
			{
				out_ReferencedPartComponentInstanceID = itr->second;
			//	std::cout << std::endl << "PPPP hash table BEGIN";
			//	for each ( std::pair< IntList, std::string> i_pair in in_FeatureIDs_to_ComponentInstanceID_hashtable )
			//	{
			//		std::cout << std::endl << "   Key:   ";
			//		for each (int k in i_pair.first ) std::cout << "  " << k;
			//		std::cout << std::endl << "   Value: " << i_pair.second;
			//	}
			//	std::cout << std::endl << "PPPP hash table END";
				// snyako@isis.vanderbilt.edu: This logging is too excessive
				/*std::cout << std::endl << "PPPP hash table BEGIN";
				for each ( std::pair< IntList, std::string> i_pair in in_FeatureIDs_to_ComponentInstanceID_hashtable )
				{
					std::cout << std::endl << "   Key:   ";
					for each (int k in i_pair.first ) std::cout << "  " << k;
					std::cout << std::endl << "   Value: " << i_pair.second;
				}
				std::cout << std::endl << "PPPP hash table END";*/
			}	
			else
			{	
			//	std::cout << std::endl << "PPPP hash table BEGIN";
			//	for each ( std::pair< IntList, std::string> i_pair in in_FeatureIDs_to_ComponentInstanceID_hashtable )
			//	{
			//		std::cout << std::endl << "   Key:   ";
			//		for each (int k in i_pair.first ) std::cout << "  " << k;
			//		std::cout << std::endl << "   Value: " << i_pair.second;
			//	}
			//	std::cout << std::endl << "PPPP hash table END";

				std::stringstream errorString;
				errorString << "Function - GetReferencedPartComponentInstanceID, could not find a known ComponentInstanceID for in_ProSelection.";
				throw isis::application_exception(errorString);					
			}
		}

	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// This function returns the componentInstanceIDs (out_ReferencedComponentInstanceIDs) that are referenced by the 
	// ProSelections in in_AssembledFeatureDefinition.constraintDefinitions.
	// The parent model (i.e. in_AssembledFeatureDefinition.componentInstanceID  ) is not considered a reference; and 
	// therefore, will not appear in out_ReferencedComponentInstanceIDs.
	void RetrieveReferencedComponentInstanceIDs (
						const std::list<int>												&in_OffSetFeatureIDPath_list,
						const CreoAssembledFeatureDefinition								&in_AssembledFeatureDefinition,
						int																	in_SetIndex,
						const std::unordered_map<IntList, std::string, ContainerHash<IntList>>	&in_FeatureIDs_to_ComponentInstanceID_hashtable,
						std::set<std::string>												&out_ReferencedComponentInstanceIDs)
																				throw (isis::application_exception)
	{
		

		for each ( CreoConstraintDefinition i in in_AssembledFeatureDefinition.constraintDefinitions )
		{	
			if ( in_SetIndex == i.component_constr_set_id )
			{
				DatumRefResolver datumRefResolver_assem_constr_ref(i.component_assem_constr_ref);
				DatumRefResolver datumRefResolver_comp_constr_ref(i.component_comp_constr_ref);

				vector<ProSelection> resolved_assem_constr_ref;
				vector<ProSelection> resolved_comp_constr_ref;

				try{
					datumRefResolver_assem_constr_ref.Resolve(resolved_assem_constr_ref);
					datumRefResolver_comp_constr_ref.Resolve(resolved_comp_constr_ref);
				} catch (isis::application_exception &ex)
				{
					isis_LOG(lg, isis_FILE, isis_ERROR) << "Datum resolution failed: " << ex.what();
				}

				for each ( ProSelection i in resolved_assem_constr_ref )
				{
					std::string componentInstanceID;				
					GetReferencedPartComponentInstanceID (	i, 
															in_OffSetFeatureIDPath_list,
															in_FeatureIDs_to_ComponentInstanceID_hashtable, 
															componentInstanceID );				
					if ( componentInstanceID.size() > 0 && componentInstanceID != in_AssembledFeatureDefinition.componentInstanceID  )
					{
										out_ReferencedComponentInstanceIDs.insert(componentInstanceID);

					isis_LOG(lg, isis_FILE, isis_INFO) << "RetrieveReferencedComponentInstanceIDs, resolved_assem_constr_ref, componentInstanceID: " << componentInstanceID;
					}
				}

				for each ( ProSelection i in resolved_comp_constr_ref )
				{
					std::string componentInstanceID;				
					GetReferencedPartComponentInstanceID (	i, 
															in_OffSetFeatureIDPath_list,
															in_FeatureIDs_to_ComponentInstanceID_hashtable, 
															componentInstanceID );				
					if ( componentInstanceID.size() > 0 && componentInstanceID != in_AssembledFeatureDefinition.componentInstanceID  )
					{
						out_ReferencedComponentInstanceIDs.insert(componentInstanceID);
						isis_LOG(lg, isis_FILE, isis_INFO) << "vvvvvvvvvv RetrieveReferencedComponentInstanceIDs, resolved_comp_constr_ref, componentInstanceID: " << componentInstanceID;
					}
				}
			}			
		}

	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	struct RequiredGeometriesData
	{
		const std::vector<ProType>	geometries;
		const int		geometryCount;
		RequiredGeometriesData( const std::vector<ProType> in_Geometries, int in_GeometryCount) : geometries(in_Geometries), geometryCount(in_GeometryCount){};
	};

	// Verify that the geometries (e.g. surface, axis, point...) defined in in_ConstraintPairs (excluding constraints with guides)  
	// exactly equal (no more no less) in_RequiredGeometries
	bool GeometryMatchesJointType(	const std::vector<ConstraintPair>			&in_ConstraintPairs,
									const std::vector<RequiredGeometriesData>	&in_RequiredGeometries)

	{
		int numberGeometries = in_RequiredGeometries.size();
		std::vector<int> actualCounts(numberGeometries, 0);

		int totalCountExpected = 0;
		for each ( const RequiredGeometriesData &i in in_RequiredGeometries ) totalCountExpected += i.geometryCount;

		int totalConstraintPairs_NonGuide_count = 0;

		for each (  const ConstraintPair &i in in_ConstraintPairs) 
		{
			if ( !i.treatConstraintAsAGuide )
			{
				++totalConstraintPairs_NonGuide_count;
				for ( int j = 0; j < numberGeometries; ++j)
				{			
					for each ( ProType k in in_RequiredGeometries[j].geometries)
					{
						//if ( i.featureGeometryType == k )
						if ( FeatureGeometryType_enum(i.featureGeometryType) == k )
							
						{
							++actualCounts[j];
							break;
						}
					}
				}
			}
		}  // END for

		if ( totalConstraintPairs_NonGuide_count != totalCountExpected) return false;

		for ( int i = 0; i < numberGeometries; ++i)
		{
			if ( actualCounts[i] != in_RequiredGeometries[i].geometryCount ) return false;
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Creo Geometry Types Required for Creo Kinematic Joints:
	//
	// Creo Joint Type		Other Name			Creo Required Geometry
	// ---------------		---------------		---------------------------------------
	//	Pin					Revolute			Axis, Point or Plane
	//	Cylinder			Cylindrical			Axis
	//	Slider				Prismatic			Axis, Plane	
	//	Planar									Plane (Note- Creo supports further restrictions (i.e. additional planes) but
	//											we will assume the classical definition (3 degrees of freedom) of a planar constraint.  
	//											Additional, planes/geometry will result in a user defined constraint.
	//	Ball				Spherical			Point (Creo supports other geometry types, but we will only support a point)
	//
	//  Pre-Conditions:
	//		in_ConstraintPairs could contain a guide, but the guide would be ignored. DO NOT call this function to determine if the constraints
	//		including a guide represent a particular type joint.
	//		
	//		The order of in_ConstraintPairs is does not influence the output functions.  Elsewhere in this code, the proper sorting is applied.
	//
	//	Post-Conditions
	//		If the geometry requirements in the above table are satisfied
	//			returns the specific joint type (e.g. REVOLUTE_JOINT, SPHERICAL_JOINT, CYLINDRICAL_JOINT...)
	//		else
	//			return UNKNOWN_JOINT_TYPE
	e_CADJointType AdjustJointTypeToCreoGeometryTypes( const std::vector<ConstraintPair> &in_ConstraintPairs,
													   cad::JointType in_JointType )
	{
			

		int counter_1 = 0;
		int counter_2 = 0;
		std::vector<RequiredGeometriesData> requiredGeometries;
		std::vector<ProType>	geometries;

		switch ( in_JointType )
		{
			case  isis::cad::FIXED:
				return FIXED_JOINT;
				break;
			case  isis::cad::REVOLUTE:
				// Axis and ( plane or point)
				geometries.push_back(PRO_AXIS);
				requiredGeometries.push_back(RequiredGeometriesData( geometries, 1));

				geometries.clear();
				geometries.push_back(PRO_SURFACE);
				geometries.push_back(PRO_POINT);
				requiredGeometries.push_back(RequiredGeometriesData( geometries, 1));

				if ( GeometryMatchesJointType(in_ConstraintPairs, requiredGeometries))
				{
					return REVOLUTE_JOINT;
				}
				else
				{
					isis_LOG(lg, isis_FILE, isis_INFO) << "Due to constraint geometry not consisting of a axis and (plane or point), converted REVOLUTE joint type to UNKNOWN_JOINT_TYPE";
					return UNKNOWN_JOINT_TYPE;
				}
				break;
			case  isis::cad::UNIVERSAL:
				return UNIVERSAL_JOINT;
				break;
			case  isis::cad::SPHERICAL:
				// Requires one and only one point
				geometries.push_back(PRO_POINT);
				requiredGeometries.push_back(RequiredGeometriesData( geometries, 1));

				if ( GeometryMatchesJointType(in_ConstraintPairs, requiredGeometries))
				{
					return SPHERICAL_JOINT;
				}
				else
				{
					isis_LOG(lg, isis_FILE, isis_INFO) << "Due to constraint geometry not consisting of a point, converted SPHERICAL joint type to UNKNOWN_JOINT_TYPE";
					return UNKNOWN_JOINT_TYPE;
				}
				break;
			case  isis::cad::PRISMATIC:
				// Requires an axis and plane
				geometries.push_back(PRO_AXIS);
				requiredGeometries.push_back(RequiredGeometriesData( geometries, 1));

				geometries.clear();
				geometries.push_back(PRO_SURFACE);
				requiredGeometries.push_back(RequiredGeometriesData( geometries, 1));

				if ( GeometryMatchesJointType(in_ConstraintPairs, requiredGeometries))
				{
					return PRISMATIC_JOINT;
				}
				else
				{
					isis_LOG(lg, isis_FILE, isis_INFO) << "Due to constraint geometry not consisting of an axis and plane, converted PRISMATIC joint type to UNKNOWN_JOINT_TYPE";
					return UNKNOWN_JOINT_TYPE;
				}
				break;
			case  isis::cad::CYLINDRICAL:
				// Requires an axis
				geometries.push_back(PRO_AXIS);
				requiredGeometries.push_back(RequiredGeometriesData( geometries, 1));

				if ( GeometryMatchesJointType(in_ConstraintPairs, requiredGeometries))
				{
					return CYLINDRICAL_JOINT;
				}
				else
				{
					isis_LOG(lg, isis_FILE, isis_INFO) << "Due to constraint geometry not consisting of an axis, converted CYLINDRICAL joint type to UNKNOWN_JOINT_TYPE";
					return UNKNOWN_JOINT_TYPE;
				}

				break;
			
			case  isis::cad::PLANAR:
				// Requires an plane
				geometries.push_back(PRO_SURFACE);
				requiredGeometries.push_back(RequiredGeometriesData( geometries, 1));

				if ( GeometryMatchesJointType(in_ConstraintPairs, requiredGeometries))
				{
					return PLANAR_JOINT;
				}
				else
				{
					isis_LOG(lg, isis_FILE, isis_INFO) << "Due to constraint geometry not consisting of a plane, converted PLANAR joint type to UNKNOWN_JOINT_TYPE";
					return UNKNOWN_JOINT_TYPE;
				}
				break;
			case  isis::cad::FREE:
				return FREE_JOINT;
				break;
			default:
				isis_LOG(lg, isis_FILE, isis_INFO) << "Due to unknown joint type, set joint type to UNKNOWN_JOINT_TYPE";
				return UNKNOWN_JOINT_TYPE;
		}

	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void PopulateMap_with_Junctions_per_InputXMLConstraints( 
					cad::CadFactoryAbstract							&in_factory,
					const std::vector<std::string>					&in_ListOfComponentIDsInTheAssembly, 
					std::map<std::string, isis::CADComponentData>	&in_out_CADComponentData_map,
					bool											in_Force)
																			throw (isis::application_exception)
																	
	{
			

		for each ( const std::string &i in in_ListOfComponentIDsInTheAssembly )
		{		
			int constraintPairs_counter = 1;
			for (std::vector<ConstraintData>::iterator j = in_out_CADComponentData_map[i].constraintDef.constraints.begin();  
				 j < in_out_CADComponentData_map[i].constraintDef.constraints.end	();
				 ++j )
			{
				isis_LOG(lg, isis_FILE, isis_INFO) << "Computed joint information, ComponentInstanceID: " << i << ", Constraint pairs set " << constraintPairs_counter << " of " << in_out_CADComponentData_map[i].constraintDef.constraints.size();
				if ( !j->computedJointData.junctiondDefined_withoutGuide || in_Force )  // without-a-guide would always be defined if either with/with-out were defined.
				{
					if ( j->hasAGuideConstraint() )
					{
						if ( !j->computedJointData.junctiondDefined_withoutGuide )
						{				
							// Just set the values for now
							j->computedJointData.jointType_withguide =  FIXED_JOINT;
							j->computedJointData.junctiondDefined_withGuide = true;						

							isis_LOG(lg, isis_FILE, isis_INFO) << "   With guide (by default set to FIXED_JOINT), Joint type: " << CADJointType_string(j->computedJointData.jointType_withguide);

							std::vector<ConstraintPair> constraintPairs_withoutGuide = j->getConstraintPairsWithoutGuide();
							PopulateMap_with_JunctionInformation_SingleJunction( in_factory, 
																				i,
																				constraintPairs_withoutGuide,
																				j->computedJointData.junction_withoutguide,
																				in_out_CADComponentData_map);
							// ttttt						
							//j->computedJointData.jointType_withoutguide =  GetCADJointType(j->computedJointData.junction_withoutguide.joint_pair.first.type);
							j->computedJointData.jointType_withoutguide =  AdjustJointTypeToCreoGeometryTypes(j->constraintPairs, j->computedJointData.junction_withoutguide.joint_pair.first.type);
							// ttttt
							j->computedJointData.coordinatesystem = i;
							j->computedJointData.junctiondDefined_withoutGuide = true;
							isis_LOG(lg, isis_FILE, isis_INFO) << "   Without guide, Joint type: " << CADJointType_string(j->computedJointData.jointType_withoutguide);
						}
					}
					else
					{
						PopulateMap_with_JunctionInformation_SingleJunction( in_factory, 
																			i,
																			j->constraintPairs,
																			j->computedJointData.junction_withoutguide,
																			in_out_CADComponentData_map );
						j->computedJointData.junctiondDefined_withGuide = false;
						j->computedJointData.coordinatesystem = i;
						j->computedJointData.junctiondDefined_withoutGuide = true;
						//j->computedJointData.jointType_withoutguide = GetCADJointType(j->computedJointData.junction_withoutguide.joint_pair.first.type);
						j->computedJointData.jointType_withoutguide =  AdjustJointTypeToCreoGeometryTypes(j->constraintPairs, j->computedJointData.junction_withoutguide.joint_pair.first.type);
						isis_LOG(lg, isis_FILE, isis_INFO) << "   Constraint pairs do not have a guide.";
						isis_LOG(lg, isis_FILE, isis_INFO) << "   Without guide, Joint type: " << CADJointType_string(j->computedJointData.jointType_withoutguide);
					}	
				}
				else
				{
					isis_LOG(lg, isis_FILE, isis_INFO) << "   Computed joint information already defined for ComponentInstanceID: " << i;
				}

				++constraintPairs_counter;
			}  // for (std::vector<ConstraintData>::iterator j = ...
		} // for each ( const std::string i in in_ListOfComponentIDsInTheAssembly )
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	static void transform(const e3ga::vector &location, const e3ga::vector &orientation, double m[4][4], e3ga::vector &out_location, e3ga::vector& out_orientation)
	{
		/*double x = m[0][0]*orientation.m_e1+m[0][1]*orientation.m_e2+m[0][2]*orientation.m_e3;
		double y = m[1][0]*orientation.m_e1+m[1][1]*orientation.m_e2+m[1][2]*orientation.m_e3;
		double z = m[2][0]*orientation.m_e1+m[2][1]*orientation.m_e2+m[2][2]*orientation.m_e3;
		out_orientation.m_e1 = x;
		out_orientation.m_e2 = y;
		out_orientation.m_e3 = z;*/
		double p[3];
		p[0] = location.m_e1;
		p[1] = location.m_e2;
		p[2] = location.m_e3;
		double buff[3];
		ProError err = ProPntTrfEval(p, m, buff);
		out_location.m_e1 = buff[0];
		out_location.m_e2 = buff[1];
		out_location.m_e3 = buff[2];
		p[0] = orientation.m_e1;
		p[1] = orientation.m_e2;
		p[2] = orientation.m_e3;
		err = ProVectorTrfEval(p, m, buff);
		out_orientation.m_e1 = buff[0];
		out_orientation.m_e2 = buff[1];
		out_orientation.m_e3 = buff[2];
	}

	void	PopulateMap_with_JunctionDataInGlobalCoordinates( 
			const std::string								&in_AssemblyComponentID,
			const std::vector<std::string>					&in_ListOfComponentIDsInTheAssembly, // This includes the assembly component ID
			std::map<std::string, isis::CADComponentData>	&in_out_CADComponentData_map )
																			throw (isis::application_exception)
	{		
		
		for each ( const std::string &i in in_ListOfComponentIDsInTheAssembly )
		{
			for ( std::vector<ConstraintData>::iterator j = in_out_CADComponentData_map[i].constraintDef.constraints.begin();
				j != in_out_CADComponentData_map[i].constraintDef.constraints.end();
				++j )
			{
				if ( !j->computedJointData.junctiondDefined_withoutGuide )
				{
					std::stringstream errorString;
					errorString 
					<< "Function - PopulateMap_with_JunctionDataInGlobalCoordinates, component does not have a junction defined, " <<
					   std::endl << "and thus the global coordinates of the junction cannot be computed.  " <<  
					   std::endl <<	"   Model Name:            " <<	 in_out_CADComponentData_map[i].name << 
					   std::endl << "   Model Type:            " <<  isis::ProMdlType_string(in_out_CADComponentData_map[i].modelType) << 
					   std::endl << "   Component Instance ID: " <<  i;   	  
					throw isis::application_exception(errorString);
				}
				
				// Get transformation matrix from the global coordinate system to the assembled .prt/.asm
				double transformationMatrix[4][4];  // rotation 3 X 3, translation at bottom row of the 4 X 4
				RetrieveTranformationMatrix_Assembly_to_Child (	in_AssemblyComponentID,
																in_out_CADComponentData_map[j->computedJointData.coordinatesystem].componentPaths,
																in_out_CADComponentData_map,  
																PRO_B_TRUE,  // bottom up
																transformationMatrix );

#if 0
				// There's something wrong with the quaternion transformation
				cad::MotorPair motorPair = cad::MotorPair::create(transformationMatrix);
				cad::Joint jointGlobal = motorPair.move(j->computedJointData.junction_withoutguide.joint_pair.first);
#else
				// Use matrix multiplication until the quaternion rotation is not fixed
				isis_LOG(lg, isis_FILE, isis_INFO) << "Computing global coordinates of " << i << ", In the coordinate system of: " << j->computedJointData.coordinatesystem;
				isis_LOG(lg, isis_FILE, isis_INFO) << "   location: " << j->computedJointData.junction_withoutguide.joint_pair.first.location.c_str_e20();
				isis_LOG(lg, isis_FILE, isis_INFO) << "   orient  : " << j->computedJointData.junction_withoutguide.joint_pair.first.orientation.c_str_e20();
				e3ga::vector loc = j->computedJointData.junction_withoutguide.joint_pair.first.location;
				e3ga::vector orient = j->computedJointData.junction_withoutguide.joint_pair.first.orientation;
				transform(j->computedJointData.junction_withoutguide.joint_pair.first.location, j->computedJointData.junction_withoutguide.joint_pair.first.orientation, transformationMatrix, loc, orient);
				orient = e3ga::unit(orient);
				cad::Joint jointGlobal(j->computedJointData.junction_withoutguide.joint_pair.first.type, loc, orient, j->computedJointData.junction_withoutguide.joint_pair.first.rotation, 0);
				isis_LOG(lg, isis_FILE, isis_INFO) << " transformed";
				isis_LOG(lg, isis_FILE, isis_INFO) << "   location: " << loc.c_str_e20();
				isis_LOG(lg, isis_FILE, isis_INFO) << "   orient  : " << orient.c_str_e20();

#endif

				j->computedJointData.globalJointPosition.location.x = jointGlobal.location.get_e1();
				j->computedJointData.globalJointPosition.location.y = jointGlobal.location.get_e2();
				j->computedJointData.globalJointPosition.location.z = jointGlobal.location.get_e3();

				j->computedJointData.globalJointPosition.orientation.x = jointGlobal.orientation.get_e1();
				j->computedJointData.globalJointPosition.orientation.y = jointGlobal.orientation.get_e2();;
				j->computedJointData.globalJointPosition.orientation.z = jointGlobal.orientation.get_e3();;

				j->computedJointData.globalJointPosition.rotation = jointGlobal.rotation;

				j->computedJointData.globalJointPositionDefined = true;			
			}
		}	
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*
	void ResolveAssemblyConstraints_AddMarkersToMap( 
			cad::CadFactoryAbstract													&in_factory,
			const std::vector<std::string>											&in_AssemblyComponentIDs,
			std::unordered_map<IntList, std::string, ContainerHash<IntList>>		&in_FeatureIDs_to_ComponentInstanceID_hashtable,
			std::map<std::string, isis::CADComponentData>	&in_out_CADComponentData_map )
																		throw (isis::application_exception)
	{
		log4cpp::Category& logcat = log4cpp::Category::getInstance(LOGCAT_LOGFILEONLY);
		for each ( const std::string &i in in_AssemblyComponentIDs )
		{
			isis::CADComponentData &leafAssembly = in_out_CADComponentData_map[i];
			for (std::vector<isis::ConstraintData>::iterator it = leafAssembly.constraintDef.constraints.begin(); it != leafAssembly.constraintDef.constraints.end(); ++it)
			{
				if (it->constrainedFrom_Paths.size()>1)
				{
					throw isis::application_exception("The constraint in assembly " + i + " resolves into multiple parts.");
				}

				if (it->constrainedFrom_Paths.size()==1)
				{
					// Merge the 2 paths into one
					IntList pathToPart = leafAssembly.componentPaths;
					pathToPart.insert(pathToPart.end(), it->constrainedFrom_Paths.begin()->begin(), it->constrainedFrom_Paths.begin()->end());
					std::string partid = in_FeatureIDs_to_ComponentInstanceID_hashtable[pathToPart];
					if (partid.size()==0)
					{
						throw isis::application_exception("Can't find constrainedfrom part for assembly " + i + ".");
					}
					it->constrainedFrom_InstanceID = partid;
					if (in_out_CADComponentData_map[partid].modelType == PRO_MDL_ASSEMBLY)
					{
						isis_LOG(lg, isis_FILE, isis_WARN) << "Resolved constrainedFrom_Paths points to an assembly: " + i;
					}
				}

				if (it->constrainedTo_Paths.size()>1)
				{
					throw isis::application_exception("The constraint in assembly " + i + " resolves into multiple parts.");
				}

				if (it->constrainedTo_Paths.size()==1)
				{
					// Merge the 2 paths into one
					if (it->constraintPairs[0].constraintFeatures[0].componentInstanceID != i)
					{
						throw isis::application_exception("In a constraint pair, the first is not the component beign constrained - something is wrong with this structure, assembly id: " + i);
					}
					isis::CADComponentData &other = in_out_CADComponentData_map[it->constraintPairs[0].constraintFeatures[1].componentInstanceID];
					IntList pathToPart = other.componentPaths;
					pathToPart.insert(pathToPart.end(), it->constrainedTo_Paths.begin()->begin(), it->constrainedTo_Paths.begin()->end());
					string partid = in_FeatureIDs_to_ComponentInstanceID_hashtable[pathToPart];
					if (partid.size()==0)
					{
						throw isis::application_exception("Internal error: can't find constrainedto part for assembly " + i + ".");
					}
					it->constrainedTo_InstanceID = partid;
					if (in_out_CADComponentData_map[partid].modelType == PRO_MDL_ASSEMBLY)
					{
						isis_LOG(lg, isis_FILE, isis_WARN) << "Resolved constrainedTo_Paths points to an assembly. Assembly: " << i << ", constrainedTo: " << partid;
					}
				}
			}
		}
	}
	*/
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Description: 
	//		This function resolves the references of in_FeatureName to the parts that are referenced by in_FeatureName.  
	//		For example, in_FeatureName could be a datum (i.e. plane, axis, point, or coordinate system) and that datum 
	//		could be defined by pointing to part geometry.  This function follows those refereces until it finds the parts 
	//		that own the referenced geometry.  A common use case is a datum plane in an assembly where the datum plane 
	//		points to a second datum plane in a sub-part, wherein the second datum plane points to geometry in the sub-part.  
	//		This function would return the ComponentInstanceIDs of the parts that own the referenced geometry.
	//
	//		It should be noted that a datum can reference more than one geometries and those geometries can be in separate
	//		parts.
	// 
	//	Pre-Conditions:
	//		This function should be called with out_ComponentInstanceIDs_of_PartsReferencedByFeature_set being empty.
	//
	//		in_TopAssemblyModel_ComponentInstanceID must be in in_CADComponentData_map and be the top assembly for the 
	//		entire assembly structure containing in_ComponentInstanceID
	//
	//		in_ComponentInstanceID must be in CADComponentData_map
	//
	//	Post-Conditions
	//		if in_FeatureName not found in in_ComponentInstanceID then throw throw (isis::application_exception)
	//
	//		If no exceptions 
	//			then 
	//				out_ComponentInstanceIDs_of_PartsReferencedByFeature_set would be populated with 0 to many ComponentInstanceIDs.
	//				It could be 0 because in_FeatureName might only have references to an assembly and this function only finds 
	//				references to parts.
	static void FindPartsReferencedByFeature(	
						const std::string							&in_TopAssemblyComponentInstanceID, 
						const std::string							&in_ComponentInstanceID,
						const MultiFormatString						&in_FeatureName,
						ProType										in_FeatureGeometryType,
						const std::unordered_map<IntList, std::string, ContainerHash<IntList>>		&in_FeatureIDs_to_ComponentInstanceID_hashtable,
						std::map<string, isis::CADComponentData>	&in_CADComponentData_map,
						std::set<std::string>						&out_ComponentInstanceIDs_of_PartsReferencedByFeature_set)
																			throw (isis::application_exception)
	{
		if ( in_CADComponentData_map[in_ComponentInstanceID].modelType == PRO_MDL_PART )
		{
			// If in_ComponentInstanceID is a part, then in_FeatureName is owned by in_ComponentInstanceID
			out_ComponentInstanceIDs_of_PartsReferencedByFeature_set.insert(in_ComponentInstanceID);
			return;
		}

		ProAsmcomppath	comp_path;
		isis::Retrieve_ProAsmcomppath_WithExceptions(	in_CADComponentData_map[in_TopAssemblyComponentInstanceID].modelHandle, 
														in_CADComponentData_map[in_ComponentInstanceID].componentPaths, 
														comp_path );

		//////////////////////////////////////////////////////////
		// Find the address to the model identified by comp_path
		//////////////////////////////////////////////////////////
		ProMdl		 model;					
		isis::isis_ProAsmcomppathMdlGet( &comp_path, &model);


	    //	Find the datum ModelItem
		ProModelitem  model_datum;

		isis::isis_ProModelitemByNameInit_WithDescriptiveErrorMsg (	
												in_ComponentInstanceID, // Added arguments
												in_CADComponentData_map[in_ComponentInstanceID].name, 
												in_CADComponentData_map[in_ComponentInstanceID].modelType,   
																	//in_ContraintDef.p_base_model, //base_model, // Original arguments
												model, //base_model, // Original arguments
												in_FeatureGeometryType, 
												in_FeatureName, 
												&model_datum);  

		//	Allocate the selections
		ProSelection model_select;
	
		ValidatePathAndModelItem_ThrowExceptionIfInvalid(comp_path, model_datum);
		isis::isis_ProSelectionAlloc(&comp_path, &model_datum, &model_select);
		
		DatumRefResolver resolver_added(model_select);
		vector<ProSelection> resolved_list;
		resolver_added.Resolve(resolved_list);
		
		for each ( ProSelection i in resolved_list )
		{
			ProAsmcomppath comppath;
			isis_ProSelectionAsmcomppathGet(i, &comppath);
			list<int> pathaslist;
			isis::ProAsmcomppathToList(pathaslist, comppath);
			list<int> fullist = in_CADComponentData_map[in_ComponentInstanceID].componentPaths;
			fullist.insert(fullist.end(), pathaslist.begin(), pathaslist.end());

			std::unordered_map<IntList, std::string, ContainerHash<IntList>>::const_iterator itr; 
			itr = in_FeatureIDs_to_ComponentInstanceID_hashtable.find(fullist);
	
			if ( itr != in_FeatureIDs_to_ComponentInstanceID_hashtable.end() )
			{
				// We are only interested in parts constrained to parts.  A part constrained
				// to an assembly is not of interest because an assembly is just an abstract
				// concept for organizing parts.  In a physical assembly, parts are constrained
				// to parts.
				if (in_CADComponentData_map[itr->second].modelType == PRO_MDL_PART )
								out_ComponentInstanceIDs_of_PartsReferencedByFeature_set.insert(itr->second);
			}	
			else
			{	
				std::stringstream errorString;
				errorString << "Function - " << __FUNCTION__ << ", could not find a known ComponentInstanceID for in_ProSelection." << std::endl <<
								"One possible cause for this error is that CADAssembly.xml does not have the the following entry:  "  << std::endl <<
							    "<ProcessingInstruction Primary=\"COMPLETE_THE_HIERARCHY_FOR_LEAF_ASSEMBLIES\" Secondary=\"\" />:  "  << std::endl <<
								"   Model Name:            " <<	 in_CADComponentData_map[in_ComponentInstanceID].name << std::endl <<
								"   Model Type:            " <<	 isis::ProMdlType_string(in_CADComponentData_map[in_ComponentInstanceID].modelType)<<  std::endl <<
								"   Component Instance ID: " <<  in_ComponentInstanceID <<  std::endl <<
								"   FeatureName:           " <<  (std::string)in_FeatureName <<  std::endl <<
								"   Path Feature IDs:     ";
						for each ( int i_fid  in fullist ) errorString << " " << i_fid;
				throw isis::application_exception(errorString);					
			}
		}
	}	

	struct ConstraintSourceAndDestination
	{
		std::string Source_ComponentID_AssemblyPart;
		std::string Destination_ComponentID_Part;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// WARNING in_AssemblyComponentIDs should only be parts or assemblies from CADAssembly.xml.  If it is
	// an assembly, it would be a leaf assembly
	void PopulateMap_with_ConstrainedToInfo_per_InputXMLConstraints ( 
			const std::string														&in_TopAssemblyComponentInstanceID,
			const std::vector<std::string>											&in_AssemblyComponentIDs,
			const std::unordered_map<IntList, std::string, ContainerHash<IntList>>	&in_FeatureIDs_to_ComponentInstanceID_hashtable,
			std::map<std::string, isis::CADComponentData>							&in_out_CADComponentData_map )
																		throw (isis::application_exception)
	{
		

		for each (  const std::string &i in in_AssemblyComponentIDs )
		{		
			if ( in_out_CADComponentData_map[i].modelType == PRO_MDL_PART || 
				(in_out_CADComponentData_map[i].specialInstruction != CAD_SPECIAL_INSTRUCTION_HAS_KINEMATIC_JOINT ) )
			// Leaf assembly without kinematic joints (i.e. treat as one body) or just a part
			{
				for ( std::vector<ConstraintData>::iterator j(in_out_CADComponentData_map[i].constraintDef.constraints.begin());
					  j < in_out_CADComponentData_map[i].constraintDef.constraints.end();
					  ++j)
				{
					for ( std::vector<ConstraintPair>::iterator k(j->constraintPairs.begin());
						  k < j->constraintPairs.end(); 
						  ++k )				
					{
						for ( std::vector<ConstraintFeature>::iterator l(k->constraintFeatures.begin());
							l < k->constraintFeatures.end();
							++l)
						{
							if ( l->componentInstanceID != i )  // A component cannot depend on itself
							{						
								if ( in_out_CADComponentData_map[l->componentInstanceID].modelType == PRO_MDL_PART ||
									 ( in_out_CADComponentData_map[l->componentInstanceID].specialInstruction != CAD_SPECIAL_INSTRUCTION_HAS_KINEMATIC_JOINT ) )
								{
									j->constrainedTo_ComponentInstanceIDs_DerivedFromConstraintPairs.insert(l->componentInstanceID);
								}
								else
								{			
									// Assembly with CAD_SPECIAL_INSTRUCTION_HAS_KINEMATIC_JOINT
									std::set<std::string>	componentInstanceIDs_of_PartsReferencedByFeature_set;
									FindPartsReferencedByFeature(	in_TopAssemblyComponentInstanceID,
																	l->componentInstanceID,
																	l->featureName,
																	FeatureGeometryType_enum(k->featureGeometryType),
																	//k->featureGeometryType,
																	in_FeatureIDs_to_ComponentInstanceID_hashtable,
																	in_out_CADComponentData_map,
																	componentInstanceIDs_of_PartsReferencedByFeature_set);
									for each ( const std::string &m in componentInstanceIDs_of_PartsReferencedByFeature_set ) 
									{
										j->constrainedTo_ComponentInstanceIDs_DerivedFromConstraintPairs.insert(m);
									}
								}
							}
						} // END for each ( const ConstraintFeature &l in k.constraintFeatures )
					}  // END for each ( const ConstraintPair &k in j.constraintPairs )
				} // END for each ( const ConstraintData &j in in_out_CADComponentData_map[i].constraintDef.constraints )
			}
			else
			{
				// At this point the following conditions must hold:
				// 1. CADAssembly.xml has the tag HAS_KINEMATIC_JOINT (enum CAD_SPECIAL_INSTRUCTION_HAS_KINEMATIC_JOINT)
				// 2. Must be an assembly.  Only assemblies have the tag CAD_SPECIAL_INSTRUCTION_HAS_KINEMATIC_JOINT
				// 3. The datums in the constraint sets for the assembly must point to parts within the assembly and
				//    must point to one and only one part per constraint set.
				// 4. The datums in the constraint set that point to external parts/assemblies (i.e. not in the assembly) must 
				//	  point to one and only one part/assembly per constraint set
				// 5. The joint type for each constraint set would have already been computed. 

				// Per constraint set, this section will:
				// 1. Follow each assembly datum reference per constraint set to its part (found part) or to its assembly for
				//    !CAD_SPECIAL_INSTRUCTION_HAS_KINEMATIC_JOINT
				// 2. Modify in_out_CADComponentData_map for the found part to have the previously computed
				//    joint information for the assembly.

				if ( in_out_CADComponentData_map[i].modelType != PRO_MDL_ASSEMBLY )
				{
					std::stringstream errorString;
					errorString <<
						"Function: " << __FUNCTION__ << ", A part model has the CADAssembly.xml tag SpecialInstruction=\"HAS_KINEMATIC_JOINT\" .  Only assemblies can have this tag."  << std::endl <<
						"   Model Name:            " <<	 in_out_CADComponentData_map[i].name << std::endl <<
						"   Model Type:            " <<	 isis::ProMdlType_string(in_out_CADComponentData_map[i].modelType)<<  std::endl <<
						"   Component Instance ID: " <<  i;
					throw isis::application_exception(errorString);	
				}


				for ( std::vector<ConstraintData>::iterator j(in_out_CADComponentData_map[i].constraintDef.constraints.begin());
					  j < in_out_CADComponentData_map[i].constraintDef.constraints.end();
					  ++j )
				{
					std::set<std::string>	componentInstanceIDs_of_Assembly_PartsReferencedByFeature_set;
					std::vector<isis::MultiFormatString>  featureNames_AssemblyReferences;

					std::set<std::string>	componentInstanceIDs_of_NonAssembly_ModelsReferencedByFeature_set;
					std::vector<isis::MultiFormatString>  featureNames_NonAssemblyReferences;

					std::vector<ConstraintSourceAndDestination> constraintSourceAndDestination;

					for ( std::vector<ConstraintPair>::iterator k(j->constraintPairs.begin());
						  k < j->constraintPairs.end(); 
						  ++k )				
					{
						
						for ( std::vector<ConstraintFeature>::iterator l(k->constraintFeatures.begin());
							l < k->constraintFeatures.end();
							++l)
						{
							if ( l->componentInstanceID == i )  
							{
								featureNames_AssemblyReferences.push_back(l->featureName);
								// This is the assembly references,  must follow the datums to the parts in the assembly
								FindPartsReferencedByFeature(	in_TopAssemblyComponentInstanceID,
																l->componentInstanceID,
																l->featureName,
																FeatureGeometryType_enum(k->featureGeometryType),
																//k->featureGeometryType,
																in_FeatureIDs_to_ComponentInstanceID_hashtable,
																in_out_CADComponentData_map,
																componentInstanceIDs_of_Assembly_PartsReferencedByFeature_set);
							}
							else
							{		
								featureNames_NonAssemblyReferences.push_back(l->featureName);
								componentInstanceIDs_of_NonAssembly_ModelsReferencedByFeature_set.insert(l->componentInstanceID);

								
								// Non-assembly references.  The parts in the assembly would be constrained to these parts.
								if ( in_out_CADComponentData_map[l->componentInstanceID].modelType == PRO_MDL_PART || 
									(in_out_CADComponentData_map[l->componentInstanceID].specialInstruction != CAD_SPECIAL_INSTRUCTION_HAS_KINEMATIC_JOINT )) 
								{ // If part or and treat-as-one body assembly
									j->constrainedTo_ComponentInstanceIDs_DerivedFromConstraintPairs.insert(l->componentInstanceID);
								}
								else
								{			
									FindPartsReferencedByFeature(	in_TopAssemblyComponentInstanceID,
																	l->componentInstanceID,
																	l->featureName,
																	FeatureGeometryType_enum(k->featureGeometryType),
																	//k->featureGeometryType,
																	in_FeatureIDs_to_ComponentInstanceID_hashtable,
																	in_out_CADComponentData_map,
																	componentInstanceIDs_of_NonAssembly_ModelsReferencedByFeature_set);

									for each ( const std::string &m in componentInstanceIDs_of_NonAssembly_ModelsReferencedByFeature_set )
									{
										j->constrainedTo_ComponentInstanceIDs_DerivedFromConstraintPairs.insert(m);
									}

								}	
							}
						} // END for each ( const ConstraintFeature &l in k.constraintFeatures )
					}  // END for each ( const ConstraintPair &k in j.constraintPairs )

					// The assembly references should be to only one part							
					if ( componentInstanceIDs_of_Assembly_PartsReferencedByFeature_set.size() != 1 )
					{
						std::stringstream errorString;
						errorString <<
							"Function: " << __FUNCTION__ << ", An assembly model with the CADAssembly.xml tag SpecialInstruction=\"HAS_KINEMATIC_JOINT\" has constraints within a constraint set that references more than one part. " <<  std::endl <<
							"          Assemblies with this tag must reference one and only one part within the assembly per constraint set."  << std::endl <<
							"   Model Name:            " <<	 in_out_CADComponentData_map[i].name << std::endl <<
							"   Model Type:            " <<	 isis::ProMdlType_string(in_out_CADComponentData_map[i].modelType) <<  std::endl <<
							"   Component Instance ID: " <<  i <<  std::endl <<
							"   Referenced Component Instance IDs: ";
						for each ( const std::string &i_ref in componentInstanceIDs_of_Assembly_PartsReferencedByFeature_set )
							  errorString << std::endl << "       Instance ID: " << i_ref << "  Model Name: " << in_out_CADComponentData_map[i_ref].name;						 
						 errorString << std::endl << "   Referenced Feature Names:";
						for each ( const isis::MultiFormatString &i_feat in featureNames_AssemblyReferences)
							 errorString << std::endl << "       Feature Name: " <<  i_feat;
						throw isis::application_exception(errorString);	
					}

					// The references external to the assembly should point to one assembly or one part						
					if ( componentInstanceIDs_of_NonAssembly_ModelsReferencedByFeature_set.size() != 1 )
					{
						std::stringstream errorString;
						errorString <<
							"Function: " << __FUNCTION__ << ", An assembly model with the CADAssembly.xml tag SpecialInstruction=\"HAS_KINEMATIC_JOINT:\" has constraints within a constraint set that references more than one part or more than one treat-as-one-body assembly. " <<  std::endl <<
							"          Assemblies with this tag must reference one and only one part or one and only one treat-as-one-body assembly that is external to the assembled model (tagged model) constraint set."  << std::endl <<
							"   Model Name:            " <<	 in_out_CADComponentData_map[i].name << std::endl <<
							"   Model Type:            " <<	 isis::ProMdlType_string(in_out_CADComponentData_map[i].modelType)<<  std::endl <<
							"   Component Instance ID: " <<  i <<  std::endl <<
							"   Referenced Component Instance IDs: ";
						for each ( const std::string &i_ref in componentInstanceIDs_of_NonAssembly_ModelsReferencedByFeature_set )
							  errorString << std::endl << "       Instance ID: " << i_ref << "  Model Name: " << in_out_CADComponentData_map[i_ref].name;					 
						 errorString << std::endl << "   Referenced Feature Names:";
						for each ( const isis::MultiFormatString &i_feat in featureNames_NonAssemblyReferences)
							 errorString << std::endl << "       Feature Name: " <<  i_feat;
						throw isis::application_exception(errorString);	
					}

					// For componentInstanceIDs_of_Assembly_PartsReferencedByFeature_set, we need to:
					// 1. Set the constrainedTo_ComponentInstanceIDs_InferredFromLeafAssemblySubordinates
					// 2. Transfer the joint information from the assembly to the componentInstanceIDs_of_Assembly_PartsReferencedByFeature_set part


					// At this point, we know that componentInstanceIDs_of_Assembly_PartsReferencedByFeature_set
					std::string assemblyChildConstrainingPart = *componentInstanceIDs_of_Assembly_PartsReferencedByFeature_set.begin();
					std::string assemblyExternalConstrainingPart = *componentInstanceIDs_of_NonAssembly_ModelsReferencedByFeature_set.begin();
					
					// Add a new constraint set to in_out_CADComponentData_map[in_out_CADComponentData_map]				

					ConstraintData constraintData;
					constraintData.constrainedTo_ComponentInstanceIDs_DerivedFromConstraintPairs.insert(assemblyExternalConstrainingPart);

					constraintData.computedJointData = j->computedJointData;
				
					in_out_CADComponentData_map[assemblyChildConstrainingPart].constraintDef.constraints.push_back(constraintData);

				}  // END ELSE has CAD_SPECIAL_INSTRUCTION_HAS_KINEMATIC_JOINT			

			} // END for each ( const ConstraintData &j in in_out_CADComponentData_map[i].constraintDef.constraints )
		} // END for each ( const std::string &i in in_AssemblyComponentIDs )
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool AncestorAssembly_TaggedWith_HasKinematicJoint( 
					const std::string								in_ComponentIntanceID,
					std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map )
	{
		std::string i = in_ComponentIntanceID;
		while ( in_CADComponentData_map[i].parentComponentID.size() != 0 )
		{
			// Note - a parent must always be an assembly
			if ( in_CADComponentData_map[i].specialInstruction == CAD_SPECIAL_INSTRUCTION_HAS_KINEMATIC_JOINT) return true;
			i = in_CADComponentData_map[i].parentComponentID;
		}

		return false;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	void FurtherTrimList_Remove_TreatAsOneBodeParts (
						const std::vector<std::string>					&in_ListOfComponentIDsInTheAssembly,
						std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map,
						std::vector<std::string>						&out_trimmedListOfComponentIDs )
	{
		for each ( const std::string &i in in_ListOfComponentIDsInTheAssembly )
		{
			if ( in_CADComponentData_map[i].dataInitialSource == INITIAL_SOURCE_INPUT_XML_FILE )
			{
				out_trimmedListOfComponentIDs.push_back(i);
			}
			else
			{
				// INITIAL_SOURCE_DERIVED_FROM_LEAF_ASSEMBLY_DESCENDANTS
				if (AncestorAssembly_TaggedWith_HasKinematicJoint(i, in_CADComponentData_map)) out_trimmedListOfComponentIDs.push_back(i);
			}
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////

	void PopulateMap_with_Junctions_and_ConstrainedToInfo_per_CreoAsmFeatureTrees( 
			cad::CadFactoryAbstract													&in_factory,
			const std::vector<std::string>											&in_AssemblyComponentIDs,
			const std::unordered_map<IntList, std::string, ContainerHash<IntList>>	&in_FeatureIDs_to_ComponentInstanceID_hashtable,
			std::map<std::string, isis::CADComponentData>	&in_out_CADComponentData_map )
																		throw (isis::application_exception)
	{
		

		std::vector<std::string> trimmedListOfComponentIDs;
		FurtherTrimList_Remove_TreatAsOneBodeParts( in_AssemblyComponentIDs,
													in_out_CADComponentData_map,
													trimmedListOfComponentIDs );

		for each ( const std::string &i in trimmedListOfComponentIDs )
		{

			isis_LOG(lg, isis_FILE, isis_INFO) << "************** PopulateMap_with_Junctions_and_ConstrainedToInfo_per_CreoAsmFeatureTrees, ComponentID: " << i;
		
			ProAsmcomppath asmcomppath;

			Retrieve_ProAsmcomppath_WithExceptions( in_out_CADComponentData_map[i].modelHandle,
													in_out_CADComponentData_map[i].componentPaths,
													asmcomppath);

			//isis_ProMdlDisplay( 	in_out_CADComponentData_map[i].modelHandle );

			CreoAssembledFeatureDefinition  assembledFeatureDefinition;

			assembledFeatureDefinition.modelName = in_out_CADComponentData_map[i].name;
			assembledFeatureDefinition.componentInstanceID = i;

			ProElement						ElemTree;

			RetrieveCreoElementTreeConstraints(	in_out_CADComponentData_map[i].assembledFeature,
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
												in_factory,
												assembledFeatureDefinition,
												setIndex,
												constraintData_PerFeatureTree.computedJointData.junction_withoutguide,
												in_out_CADComponentData_map );

					constraintData_PerFeatureTree.computedJointData.jointType_withoutguide =  GetCADJointType(constraintData_PerFeatureTree.computedJointData.junction_withoutguide.joint_pair.first.type);
					constraintData_PerFeatureTree.computedJointData.junctiondDefined_withoutGuide = true;
					constraintData_PerFeatureTree.computedJointData.coordinatesystem = assembledFeatureDefinition.componentInstanceID;

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
					"   Model Type:            " <<	 isis::ProMdlType_string(in_out_CADComponentData_map[i].modelType)<<  std::endl <<
					"   Component Instance ID: " << i << std::endl <<
					"   Assembled Feature Definition: " << std::endl <<
					assembledFeatureDefinition;

				throw isis::application_exception(errorString);	
			}

			// Need to set					

			// Only free this after using assembledFeatureDefinition. assembledFeatureDefinition is
			// invalid after ProFeatureElemtreeFree
			ProFeatureElemtreeFree(&in_out_CADComponentData_map[i].assembledFeature, ElemTree);									
		}

	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Populate_FeatureIDs_to_ComponentInstanceID_hashtable( 
						const std::vector<std::string>	&in_AssemblyComponentIDs,
						std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map,
						std::unordered_map<IntList, std::string, ContainerHash<IntList>> &out_FeatureIDs_to_ComponentInstanceID_hashtable )
	{
		for each ( const std::string &i in in_AssemblyComponentIDs )
			out_FeatureIDs_to_ComponentInstanceID_hashtable[in_CADComponentData_map[i].componentPaths] = i;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CheckValidityOfJointInformation( 
			const std::vector<std::string>					&in_ListOfComponentIDsInTheAssembly, 
			std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map,
			std::vector<std::string>						&out_Errors )
	{
		std::set<std::string> constrainedComponentIDs_perAssembly;

		std::vector<std::string> trimmedListOfComponentIDs;
		FurtherTrimList_Remove_TreatAsOneBodeParts( in_ListOfComponentIDsInTheAssembly,
													in_CADComponentData_map,
													trimmedListOfComponentIDs );

		for each ( const std::string &i in trimmedListOfComponentIDs )
		{
			if ( in_CADComponentData_map[i].modelType == PRO_MDL_PART )
			{
				for each ( const ConstraintData &j in in_CADComponentData_map[i].constraintDef.constraints )
				{
					//////////////////////////////////////
					// ConstraintData is at the set level
					//////////////////////////////////////
					std::set<std::string> constrainedComponentIDs_perSet;

					bool foundConstrainedTo = false;
					
					for each ( const std::string &k in j.constrainedTo_ComponentInstanceIDs_DerivedFromConstraintPairs )
					{
						if ( in_CADComponentData_map[k].modelType == PRO_MDL_PART ||
							( in_CADComponentData_map[k].dataInitialSource ==  INITIAL_SOURCE_INPUT_XML_FILE && 
							  in_CADComponentData_map[k].specialInstruction != CAD_SPECIAL_INSTRUCTION_HAS_KINEMATIC_JOINT))
						{
							constrainedComponentIDs_perAssembly.insert(k);
							constrainedComponentIDs_perSet.insert(k);
							foundConstrainedTo = true;
						}
					}


					for each ( const std::string &k in j.constrainedTo_ComponentInstanceIDs_InferredFromLeafAssemblySubordinates )
					{
						if ( in_CADComponentData_map[k].modelType == PRO_MDL_PART ||
							(in_CADComponentData_map[k].dataInitialSource ==  INITIAL_SOURCE_INPUT_XML_FILE && 
							  in_CADComponentData_map[k].specialInstruction != CAD_SPECIAL_INSTRUCTION_HAS_KINEMATIC_JOINT))
						{
							constrainedComponentIDs_perAssembly.insert(k);
							constrainedComponentIDs_perSet.insert(k);
							foundConstrainedTo = true;
						}
					}
					// if i was constrained to ComponentIDs then i is constrained
					if ( foundConstrainedTo ) constrainedComponentIDs_perAssembly.insert(i);

					// If a kinematic joint, make sure it is constrained to only one other part
					if ( j.computedJointData.jointType_withoutguide  != FIXED_JOINT &&
						 j.computedJointData.jointType_withoutguide  != UNKNOWN_JOINT_TYPE &&
						 j.computedJointData.jointType_withoutguide  != FREE_JOINT )
					{
						int componentIDs_count = 0;
						for each ( const std::string &k in constrainedComponentIDs_perSet ) if ( k != i ) ++componentIDs_count;
						
						if ( componentIDs_count != 1)
						{
							std::stringstream errorString;
							errorString <<
							"A part constrained via a kinematic joint was not constrained to one and only one other part."  << std::endl <<
							"   Model Name:            " <<	 in_CADComponentData_map[i].name << std::endl <<
							"   Model Type:            " <<  isis::ProMdlType_string(in_CADComponentData_map[i].modelType) << std::endl <<
							"   Joint Type:            " <<  CADJointType_string(j.computedJointData.jointType_withoutguide) << std::endl <<
							"   Component Instance ID: " <<  i << std::endl <<
							"   Constrained to Component Instance IDs: " << std::endl;
							for each ( const std::string &k in constrainedComponentIDs_perSet ) errorString <<  "                  " << k << std::endl;
							out_Errors.push_back(errorString.str());
						}
					}

				}  // END for each ( const ConstraintData &j in in_CADComponentData_map[i].constraintDef.constraints)

			} // END if ( in_CADComponentData_map[i].modelType == PRO_MDL_PART )

		} // END for each ( const std::string &i in trimmedListOfComponentIDs )

		///////////////////////////////////////////////////////////////////
		//  Check that each part is constrained to at least one other part
		///////////////////////////////////////////////////////////////////
		for each ( const std::string &i in trimmedListOfComponentIDs )
		{
			if ( in_CADComponentData_map[i].modelType == PRO_MDL_PART )
			{
				if ( constrainedComponentIDs_perAssembly.find(i) == constrainedComponentIDs_perAssembly.end() )
				{
					// Part not constrained to at least one other part
					std::stringstream errorString;
					errorString <<
							"Part not constrained to at least one other part. For valid joint information between parts, all parts must be connected to at least one other part."  << std::endl <<
							"   Model Name:            " <<	 in_CADComponentData_map[i].name << std::endl <<
							"   Model Type:            " << isis::ProMdlType_string(in_CADComponentData_map[i].modelType)<<  std::endl <<
							"   Component Instance ID: " <<  i;
					out_Errors.push_back(errorString.str());
				}
			}
		}
	} // END CheckValidityOfJointInformation

	
	//	Description:
	//		Returns if in_ComponentInstanceID is a CyPhy leaf assembly.  This is  determineed  by checking if 
	//		the immediate (i.e. not grandchildren) children of in_ComponentInstanceID have the setting of 
	//		INITIAL_SOURCE_DERIVED_FROM_LEAF_ASSEMBLY_DESCENDANTS
	//
	//	Pre-Conditions:
	//		ForEachLeafAssemblyInTheInputXML_AddInformationAboutSubordinates must have been invoked before 
	//		calling this function; and if not, CyPhyLeafAssembly will always return false.
	//
	//	Post-Conditions:
	//		if in_ComponentInstanceID is a part
	//			return false
	//		if the first immediate child has the setting of INITIAL_SOURCE_DERIVED_FROM_LEAF_ASSEMBLY_DESCENDANTS
	//			return true
	//		else
	//			return false
	bool CyPhyLeafAssembly (	const std::string								&in_ComponentInstanceID,
								std::map<std::string, isis::CADComponentData>	&in_out_CADComponentData_map )
	{

		if ( in_out_CADComponentData_map[in_ComponentInstanceID].modelType != PRO_MDL_ASSEMBLY ) return false;
		// The assembly could have no children, and thus is not actually a CyPhy leaf assembly 
		// At this point, it could have not children because:
		//	a) it is an empty assembly.
		//  b) the children have not been completed by the function
		//		ForEachLeafAssemblyInTheInputXML_AddInformationAboutSubordinates
		if ( in_out_CADComponentData_map[in_ComponentInstanceID].children.size() == 0 ) return false;

		for each ( const std::string &i in in_out_CADComponentData_map[in_ComponentInstanceID].children )
		{
			if ( in_out_CADComponentData_map[i].dataInitialSource == 
				 INITIAL_SOURCE_DERIVED_FROM_LEAF_ASSEMBLY_DESCENDANTS ) return true;			
		}

		return false;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void AddBoundingBoxValuesToMap( 
						const std::vector<std::string>					&in_AssemblyComponentIDs,
						std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map) 
																				throw (isis::application_exception)
	{
		for each ( const std::string &i in in_AssemblyComponentIDs )
		{
			if ( !in_CADComponentData_map[i].boundingBox.boundingBox_Defined )
			{
				isis_CADCommon::Point_3D	boundingBox_Point_1;
				isis_CADCommon::Point_3D	boundingBox_Point_2;
				double						boundingBoxDimensions_xyz[3];

				RetrieveBoundingBox_ComputeFirstIfNotAlreadyComputed(	i,
																		in_CADComponentData_map,
																		boundingBox_Point_1,
																		boundingBox_Point_2,
																		boundingBoxDimensions_xyz );

				in_CADComponentData_map[i].boundingBox.boundingBox_Point_1 = boundingBox_Point_1;
				in_CADComponentData_map[i].boundingBox.boundingBox_Point_2 = boundingBox_Point_2;
				for ( int j = 0; j < 3; ++j ) in_CADComponentData_map[i].boundingBox.Dimensions_xyz[j] = boundingBoxDimensions_xyz[j];
				in_CADComponentData_map[i].boundingBox.boundingBox_Defined = true;
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void AddMassPropertyValuesToMap( 
						const std::vector<std::string>					&in_AssemblyComponentIDs,
						std::map<std::string, isis::CADComponentData>	&in_out_CADComponentData_map) 
																				throw (isis::application_exception)
	{
		

		for each ( const std::string &i in in_AssemblyComponentIDs )
		{
			if ( !in_out_CADComponentData_map[i].massProperties.massProperties_RetrievalInvoked )
			{
				in_out_CADComponentData_map[i].massProperties.massProperties_RetrievalInvoked = true;

				ProMassProperty  mass_prop;

				isis_ProSolidMassPropertyGet_WithDescriptiveErrorMsg(i, in_out_CADComponentData_map, &mass_prop );
				double   MatrixBuffer[4][4];
				double   MatrixBuffer_temp[4][4];

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
					in_out_CADComponentData_map[i].massProperties.massProperties_Defined = true;
				else
					continue;
			
				in_out_CADComponentData_map[i].massProperties.volume_Defined		= true;			
				in_out_CADComponentData_map[i].massProperties.density_Defined		= true;
				in_out_CADComponentData_map[i].massProperties.mass_Defined			= true;

				in_out_CADComponentData_map[i].massProperties.volume		= mass_prop.volume;				
				in_out_CADComponentData_map[i].massProperties.density		= mass_prop.density;
				in_out_CADComponentData_map[i].massProperties.mass			= mass_prop.mass;
				
				// Surface Area
				if (  mass_prop.surface_area != 0.0 )
				{
					in_out_CADComponentData_map[i].massProperties.surfaceArea_Defined	= true;
					in_out_CADComponentData_map[i].massProperties.surfaceArea	= mass_prop.surface_area;
				}

				// coordSysInertiaTensor
				if ( !isis_CADCommon::AllMatrixValuesEqualTarget_3X3(  mass_prop.coor_sys_inertia_tensor, 0.0 )  )
				{
					in_out_CADComponentData_map[i].massProperties.coordSysInertiaTensor_Defined = true;
					isis_CADCommon::SetFromToMatrix_3X3( mass_prop.coor_sys_inertia_tensor, in_out_CADComponentData_map[i].massProperties.coordSysInertiaTensor );

					if ( !isis_CADCommon::Positive_Definite_3_x_3( in_out_CADComponentData_map[i].massProperties.coordSysInertiaTensor ))
					{
						isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO)	<< "\n\nERROR: Non-positive-definite inertia tensor at the default coordinate system." 
															<< "\n       ComponentInstanceID: " << i
															<< "\n       Model Name:          " << in_out_CADComponentData_map[i].name 
															<< "\n       Model Type:          " << isis::ProMdlType_string(in_out_CADComponentData_map[i].modelType)
															<< "\n       Note: In the future, this will be treated as a fatal error.  Corrections to the mass properties in the CAD model are required.";
					}
				}

				// C.G.
				in_out_CADComponentData_map[i].massProperties.centerOfGravity_Defined = true;
				in_out_CADComponentData_map[i].massProperties.centerOfGravity[0] = mass_prop.center_of_gravity[0];
				in_out_CADComponentData_map[i].massProperties.centerOfGravity[1] = mass_prop.center_of_gravity[1];
				in_out_CADComponentData_map[i].massProperties.centerOfGravity[2] = mass_prop.center_of_gravity[2];

				// cGInertiaTensor
				if ( !isis_CADCommon::AllMatrixValuesEqualTarget_3X3( mass_prop.cg_inertia_tensor, 0.0 )  )
				{
					in_out_CADComponentData_map[i].massProperties.cGInertiaTensor_Defined = true;
					isis_CADCommon::SetFromToMatrix_3X3( mass_prop.cg_inertia_tensor, in_out_CADComponentData_map[i].massProperties.cGInertiaTensor );

					if ( !isis_CADCommon::Positive_Definite_3_x_3( in_out_CADComponentData_map[i].massProperties.cGInertiaTensor ))
					{
						isis_LOG(lg, isis_CONSOLE_FILE, isis_INFO)	<< "\n\nERROR: Non-positive-definite inertia tensor at the center of gravity." 
															<< "\n       ComponentInstanceID: " << i
															<< "\n       Model Name:          " << in_out_CADComponentData_map[i].name 
															<< "\n       Model Type:          " << isis::ProMdlType_string(in_out_CADComponentData_map[i].modelType)
															<< "\n       Note: In the future, this will be treated as a fatal error.  Corrections to the mass properties in the CAD model are required.";
					}

				}

				// Principal Moment of Inertia
				in_out_CADComponentData_map[i].massProperties.principalAxis_RotationMatrix_Defined = true;
				in_out_CADComponentData_map[i].massProperties.principalMomentsOfInertia[0] = mass_prop.principal_moments[0];
				in_out_CADComponentData_map[i].massProperties.principalMomentsOfInertia[1] = mass_prop.principal_moments[1];
				in_out_CADComponentData_map[i].massProperties.principalMomentsOfInertia[2] = mass_prop.principal_moments[2];

				// Principal Axis
				if ( !isis_CADCommon::AllMatrixValuesEqualTarget_3X3(  mass_prop.principal_axes, 0.0 )  )
				{
					in_out_CADComponentData_map[i].massProperties.principalAxis_RotationMatrix_Defined = true;
					isis_CADCommon::SetFromToMatrix_3X3( mass_prop.principal_axes, in_out_CADComponentData_map[i].massProperties.principalAxis_RotationMatrix );
				}
			}
		}
	}
} // end namespace isis
