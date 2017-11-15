#ifndef CC_ASSEMBLY_UTILITIES_H
#define CC_ASSEMBLY_UTILITIES_H

#include "isis_application_exception.h"
#include "cc_CADFactoryAbstract.h"


namespace isis
{

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Description:
	//		Create Creo model name with a unique suffix.  Examples:
	//			Non-Family Table - Within in_AllowedSize:
	//				in_UniqueNameIndex:							123
	//				in_ModelName_CouldIncludeFamilyTableEntry:	bracket					// Length: 7
	//				out_ModelName_Without_Suffix:               bracket					// Length: 7
	//				out_ModelName_With_Suffix:                  bracketZ123Z			// Length: 12
	//				out_CompleteName:                           bracketZ123Z			// Length: 12
	//
	//			Family Table  - Within in_AllowedSiZe
	//				in_UniqueNameIndex:							123
	//				in_ModelName_CouldIncludeFamilyTableEntry:	M3-50-10<NAS132-Bolt>		// Length: 21
	//				out_ModelName_Without_Suffix:               NAS132-Bolt					// Length: 11
	//				out_ModelName_With_Suffix:                  NAS132-BoltZ123Z			// Length: 16
	//				out_CompleteName:                           M3-50-10<NAS132-BoltZ123Z>	// Length: 26
	//
	//			Non-Family Table - Truncation Required:	
	//				in_UniqueNameIndex:                        99999
	//				in_ModelName_CouldIncludeFamilyTableEntry: 12345678901234567890123456789   // Length: 29
	//				out_ModelName_Without_Suffix:              12345678901234567890123456789   // Length: 29
	//				out_ModelName_With_Suffix:                 123456789012345678901234Z99999Z //  Length: 31
	//				out_CompleteName:                          123456789012345678901234Z99999Z //  Length: 31
	//
	//			Family Table - Truncation Required
	//				in_UniqueNameIndex:                        123
	//				in_ModelName_CouldIncludeFamilyTableEntry: Bolst_M3-50-10_Plated<NAS132>	// Length: 29
	//				out_ModelName_Without_Suffix:              NAS132							// Length: 6
	//				out_ModelName_With_Suffix:                 NASZ123Z							// Length: 8
	//				out_CompleteName:                          Bolst_M3-50-10_Plated<NASZ123Z>  // Length: 31
	//
	//	Pre-Conditions:
	//		in_ModelName_CouldIncludeFamilyTableEntry must be a legitimate Creo model name.
	//
	//		0 < in_UniqueNameIndex  < max unsigned int
	//
	//		if in_ModelName_CouldIncludeFamilyTableEntry describes a family table
	//			then
	//				in_ModelName_CouldIncludeFamilyTableEntry must contain '<', and '>', where '>' appears later in the string than '<'
	//			Examples:
	//				Engine						// Non family table example
	//				Chassis_8_Wheel<Chassis>	// Family table example, If '<' exists then '>' must exist. One and only
	//											// one '<' and one and only one '>'
	// 	Post-Conditions	
	//		throw application_exception if:
	//			a) in_ModelName_CouldIncludeFamilyTableEntry.size() > 31
	//			b) out_CompleteName.size() > 31. This would occur if the in_UniqueNameIndex forced the string to be too large.
	//			c) in_ModelName_CouldIncludeFamilyTableEntry has a malformed name e.g. one "<" but not a closing ">"
	//
	//			Example of out_CompleteName.size() > 31
	//				in_UniqueNameIndex:							12345
	//				in_ModelName_CouldIncludeFamilyTableEntry:	Bolst_M3-50-10_Plated_S<NAS132>		// Length: 31
	//				out_ModelName_Without_Suffix:               NAS132								// Length: 6
	//				out_ModelName_With_Suffix:                  Z12345Z								// Length: 7
	//				out_CompleteName:                           Bolst_M3-50-10_Plated_S<Z12345Z>	// Length: 32
	//
	//		if no exceptions, return out_... variables.  An example follows:	
	//			a) Non Family Table - out_ModelName_With_Suffix="EngineZ1Z",	out_CompleteName="EngineZ1Z"
	//			b) Family table		- out_ModelName_With_Suffix="ChassisZ1Z",	out_CompleteName="Chassis_8_Wheel<ChassisZ1Z>"


	void CreateModelNameWithUniqueSuffix(  
				cad::CadFactoryAbstract		&in_Factory,
				unsigned int				in_UniqueNameIndex, 
				const std::string			&in_ModelName_CouldIncludeFamilyTableEntry, // e.g. Chassis_8_Wheel<Chassis>
				std::string					&out_ModelName_Without_Suffix,				// e.g. Chassis
				std::string					&out_ModelName_With_Suffix,					// e.g. ChassisZ1Z
				std::string					&out_CompleteName,							// For family tables, would be the complete name
																						// e.g. Chassis_8_Wheel<ChassisZ1Z>
																						// otherwise, same as out_ModelName_With_Suffix
				unsigned int in_AllowedSize   )											// e.g. PRO_NAME_SIZE - 1 
														throw (isis::application_exception);


	/////////////////////////////////////////////////////////////////////////////////////////////////////

	struct CopyModelDefinition
	{
		std::string					componentInstanceID;
		isis::MultiFormatString		fromModelName;
		isis::MultiFormatString		toModelName;
		//ProMdlType					modelType;
		e_CADMdlType 				modelType;

	};

	enum e_ModelTypeIndicator
	{
		e_PART_MODEL_TYPE,
		e_ASSEMBLY_MODEL_TYPE,
		e_PART_OR_ASSEMBLY_MODEL_TYPE,
	};

	enum e_ModelSelectorIndicator
	{
		e_SELECT_ALL_MODELS,
		e_SELECT_ONLY_PARAMETRIC_MODELS
	};


	// If part/assem appears more than once in in_out_CADComponentData_map and is a parametric part/assembly
	//	then
	//		this function modifies in_out_CADComponentData_map to have unique name for the second
	//		and later occurrences of the particular part/assembly that is a parametric part/assembly.  
	//		The new and old part names are added to out_FromModel_ToModel. 
	void ModifyToHaveAUniqueName_ForEach_PartAndOrAssembly( 
							cad::CadFactoryAbstract							&in_Factory,
							unsigned int									&in_out_UniqueNameIndex,
							e_ModelTypeIndicator							in_ModelTypeIndicator,
							e_ModelSelectorIndicator						in_ModelSelectorIndicator,
							bool											in_ForceAllParametricModelsToBeUnique,
							unsigned int									in_MaxCADModelNameLength,   // This is max model name length/size (not including the . suffix (e.g. .prt)) 
							std::map<std::string, isis::CADComponentData>	&in_out_CADComponentData_map, 
							std::vector<CopyModelDefinition>				&out_FromModel_ToModel  );

	std::ostream& operator<<(std::ostream& output, const CopyModelDefinition &in_CopyModelDefinition); 
	std::ostream& operator<<(std::ostream& output, const std::vector<CopyModelDefinition> &in_CopyModelDefinition_vector); 

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool IsAFEAAnlysisDeckBasedRun( const CADAssemblies &in_CADAssemblies );
	bool IsFEAAnalysisAbaqusModelBasedRun( const CADAssemblies &in_CADAssemblies );

	// If at lease one of the assemblies in in_CADAssemblies contains analysesBallistic, then return true.    
	bool IsABallisticAnalysisRun( const CADAssemblies &in_CADAssemblies );
	// If at lease one of the assemblies in in_CADAssemblies contains analysesBlast, then return true.    
	bool IsABlastAnlysisRun( const CADAssemblies &in_CADAssemblies );

	// If at lease one of the assemblies in in_CADAssemblies contains CFD analysis, then return true.    
	bool IsACFDAnalysisRun( const CADAssemblies &in_CADAssemblies );

	bool IsAInterferenceRun( const CADAssemblies &in_CADAssemblies );

	bool HasAssemblyBasedComputations( const CADAssemblies &in_CADAssemblies );
	bool FillOutMetricsFileForLeafAssemblies( const std::list<e_DataExchangeSpecialInstruction> &in_DataExchangeSpecifications);

	bool Get_CompleteTheHierarchyForLeafAssemblies( const CADAssemblies &in_CADAssemblies );
	bool Get_UniquelyNameAllCADModelInstances( const CADAssemblies &in_CADAssemblies );
	bool Get_OutputJointInformation( const CADAssemblies &in_CADAssemblies );
	bool Get_ValidateJointInformation( const CADAssemblies &in_CADAssemblies );

	void Validate_ComputationInterferenceCount_ThrowExceptionIfInvalid (  
											const CADAssemblies								&in_CADAssemblies,
											std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map)
																throw (isis::application_exception);

	void CopyModels( cad::CadFactoryAbstract						&in_Factory,
					 const std::vector<CopyModelDefinition>			&in_FromModel_ToModel )
																	throw (isis::application_exception);


	void Add_DependsOn ( std::map<std::string, CADComponentData> &in_out_CADComponentData_map );

}

#endif