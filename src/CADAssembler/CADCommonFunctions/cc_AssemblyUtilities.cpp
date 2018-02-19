#include "cc_AssemblyUtilities.h"
#include "cc_CommonConstants.h"
#include "cc_CommonUtilities.h"
#include "cc_CommonFunctions.h"
#include "cc_JsonHelper.h"

namespace isis
{

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

	void CreateModelNameWithUniqueSuffix(  
				//cad::CadFactoryAbstract	&in_Factory,
				unsigned int			in_UniqueNameIndex, 
				const std::string		&in_ModelName_CouldIncludeFamilyTableEntry,
				std::string				&out_ModelName_Without_Suffix,		// Creo Example:	Chassis
				std::string				&out_ModelName_With_Suffix,			//					ChassisZ1Z
				std::string				&out_CompleteName,					//					For family tables, would be the complete name
																			//					Chassis_8_Wheel<ChassisZ1Z>
																			//					otherwise, same as out_ModelName_With_Suffix
				unsigned int			in_AllowedSize )   
														throw (isis::application_exception)
	{

		isis::cad::CadFactoryAbstract_global *cadFactoryAbstract_global_ptr = isis::cad::CadFactoryAbstract_global::instance();
		isis::cad::CadFactoryAbstract::ptr	cAD_Factory_ptr = cadFactoryAbstract_global_ptr->getCadFactoryAbstract_ptr();

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

		//ExtractModelName_FamilyTable_Info (	in_ModelName_CouldIncludeFamilyTableEntry, 
		//									out_ModelName_Without_Suffix,
		//									familyTableEntry,
		//									familyTableModel );

		isis::cad::IModelHandling&           modelHandling = cAD_Factory_ptr->getModelHandling();
		modelHandling.extractModelNameAndFamilyTableEntry( in_ModelName_CouldIncludeFamilyTableEntry, 
														out_ModelName_Without_Suffix,
														familyTableEntry,
														familyTableModel );

		if ( familyTableModel )
		{
			int tempAllowedSize = in_AllowedSize - familyTableEntry.size() - 2;  // -2 for the "<" aand ">"
			out_ModelName_With_Suffix = MergeStrings_TryToKeepWithinAllowedSize(out_ModelName_Without_Suffix, tempUniqueString, tempAllowedSize);
			//out_CompleteName = BuildAFamilyTableCompleteModelName( out_ModelName_With_Suffix, familyTableEntry);
			out_CompleteName = modelHandling.buildAFamilyTableCompleteModelName( out_ModelName_With_Suffix, familyTableEntry);

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
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool  ModelTypesMatch( e_ModelTypeIndicator			in_ModelTypeIndicator,
						   e_CADMdlType					in_ModelType )
	{
		if ( in_ModelTypeIndicator == e_PART_OR_ASSEMBLY_MODEL_TYPE ) return true;
		if ( in_ModelTypeIndicator == e_PART_MODEL_TYPE		&& in_ModelType == CAD_MDL_PART ) return true;
		if ( in_ModelTypeIndicator == e_ASSEMBLY_MODEL_TYPE && in_ModelType == CAD_MDL_ASSEMBLY ) return true;

		return false;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	void BuildListOfCADModels_ThatShouldBeCopiedToNewNames( 
							//cad::CadFactoryAbstract							&in_Factory,
							unsigned int									&in_out_UniqueNameIndex,
							e_ModelTypeIndicator							in_ModelTypeIndicator,
							e_ModelSelectorIndicator						in_ModelSelectorIndicator,
							bool											in_ForceAllParametricModelsToBeUnique,
							unsigned int									in_MaxCADModelNameLength,
							std::map<std::string, isis::CADComponentData>	&in_out_CADComponentData_map, 
							std::vector<CopyModelDefinition>				&out_FromModel_ToModel  )
																		throw (isis::application_exception)
	{
		

		isis::cad::CadFactoryAbstract_global *cadFactoryAbstract_global_ptr = isis::cad::CadFactoryAbstract_global::instance();
		isis::cad::CadFactoryAbstract::ptr					cAD_Factory_ptr = cadFactoryAbstract_global_ptr->getCadFactoryAbstract_ptr();

		std::set<std::string>  modelsAlreadyEncountered;

		//int index = 1;
		//int maxPartNameLength_BeforeSuffix = PRO_NAME_SIZE - 8;  //  32 - 8 = 24

		//char buffer[isis::ISIS_CHAR_BUFFER_LENGTH];

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
			//ExtractModelName_FamilyTable_Info (	i->second.name, 
			//									origNameWithoutFamilyEntry_temp,
			//									familyTableEntry,
			//									familyTableModel );

			isis::cad::IModelHandling&           modelHandling = cAD_Factory_ptr->getModelHandling();
			modelHandling.extractModelNameAndFamilyTableEntry( i->second.name, 
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
				ConvertToUpperCase(modelHandling.combineCADModelNameAndSuffix(origNameWithoutFamilyEntry_temp, i->second.modelType) );
				//ConvertToUpperCase(CombineCreoModelNameAndSuffix(origNameWithoutFamilyEntry_temp, ProMdlType_enum(i->second.modelType)) );

			//std::cout << std::endl << "############## BuildListOfCADModels_ThatShouldBeCopiedToNewNames, modelNameWithSuffix, ComponentInstanceID: " <<  modelNameWithSuffix << "  " << i->second.componentID;

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
					//ModelTypesMatch(in_ModelTypeIndicator, ProMdlType_enum(i->second.modelType)) &&
					ModelTypesMatch(in_ModelTypeIndicator, i->second.modelType) &&
					( modelsAlreadyEncountered.find(modelNameWithSuffix) !=  modelsAlreadyEncountered.end()))  // Part/Assembly occurs a second time
			{
				std::string origNameWithoutFamilyEntry;
				std::string modelName;
				std::string completeName;
				CreateModelNameWithUniqueSuffix( 	//*cAD_Factory_ptr,
													in_out_UniqueNameIndex, 
													i->second.name,
													origNameWithoutFamilyEntry,
													modelName, 
													completeName,
													in_MaxCADModelNameLength);
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
	}	// END BuildListOfCADModels_ThatShouldBeCopiedToNewNames	 


	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// If at lease one of the assemblies in in_CADAssemblies contains analysisFEA and the analysisFEA is for ANALYSIS_DECK_BASED then return true.
	bool IsAFEAAnlysisDeckBasedRun( const CADAssemblies &in_CADAssemblies )
	{
		for ( std::list<isis::TopLevelAssemblyData>::const_iterator i( in_CADAssemblies.topLevelAssemblies.begin()); 
				i !=  in_CADAssemblies.topLevelAssemblies.end();
				++i)
		{
			if ( i->analysesCAD.analysesFEA.size() > 0   ) 
			{
				for each ( 	AnalysisFEA j in i->analysesCAD.analysesFEA )
				{
					for each ( AnalysisSolver k in j.analysisSolvers) 
					{
						if ( k.analysisSolutionType == ANALYSIS_DECK_BASED ) return true;
					}
				}
			}	//return true;
		}
		return false;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
// If at lease one of the assemblies in in_CADAssemblies contains analysisFEA and the analysisFEA is for ANALYSIS_DECK_BASED then return true.
	bool IsFEAAnalysisAbaqusModelBasedRun( const CADAssemblies &in_CADAssemblies )
	{
		for ( std::list<isis::TopLevelAssemblyData>::const_iterator i( in_CADAssemblies.topLevelAssemblies.begin()); 
				i !=  in_CADAssemblies.topLevelAssemblies.end();
				++i)
		{
			if ( i->analysesCAD.analysesFEA.size() > 0   ) 
			{
				for each ( 	AnalysisFEA j in i->analysesCAD.analysesFEA )
				{
					for each ( AnalysisSolver k in j.analysisSolvers) 
					{
						if ( k.analysisSolutionType == ANALYSIS_MODEL_BASED ) return true;
					}
				}
			}	//return true;
		}
		return false;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// If at lease one of the assemblies in in_CADAssemblies contains analysesBallistic, then return true.
	bool IsABallisticAnalysisRun( const CADAssemblies &in_CADAssemblies )
	{
		for ( std::list<isis::TopLevelAssemblyData>::const_iterator i( in_CADAssemblies.topLevelAssemblies.begin()); 
				i !=  in_CADAssemblies.topLevelAssemblies.end();
				++i)
		{
			if ( i->analysesCAD.analysesBallistic.size() > 0 ) return true;
		}
		return false;
	}
	bool IsABlastAnlysisRun( const CADAssemblies &in_CADAssemblies )
	{
		for ( std::list<isis::TopLevelAssemblyData>::const_iterator i( in_CADAssemblies.topLevelAssemblies.begin()); 
				i !=  in_CADAssemblies.topLevelAssemblies.end();
				++i)
		{
			if ( i->analysesCAD.analysesBlast.size() > 0 ) return true;
		}
		return false;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// If at lease one of the assemblies in in_CADAssemblies specifies cFDAnalysis == true, then return true.
	bool IsACFDAnalysisRun( const CADAssemblies &in_CADAssemblies )
	{
		for ( std::list<isis::TopLevelAssemblyData>::const_iterator i( in_CADAssemblies.topLevelAssemblies.begin()); 
				i !=  in_CADAssemblies.topLevelAssemblies.end();
				++i)
		{
			if ( i->analysesCAD.analysesCFD.size() > 0 ) return true;
		}
		return false;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool FillOutMetricsFileForLeafAssemblies( const std::list<e_DataExchangeSpecialInstruction> &in_DataExchangeSpecifications)
	{
		for each ( e_DataExchangeSpecialInstruction i in in_DataExchangeSpecifications ) if ( i == DATA_EXCHANGE_LEAF_ASSEMBLIES_METRICS) return true; 

		return false;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool Get_CompleteTheHierarchyForLeafAssemblies( const CADAssemblies &in_CADAssemblies )
	{
		for each ( ProcessingInstruction i in in_CADAssemblies.processingInstructions )
			if ( i.primary == COMPLETE_THE_HIERARCHY_FOR_LEAF_ASSEMBLIES )
				return true;
		return false;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool Get_UniquelyNameAllCADModelInstances( const CADAssemblies &in_CADAssemblies )
	{
		for each ( ProcessingInstruction i in in_CADAssemblies.processingInstructions )
			if ( i.primary == UNIQUELY_NAME_ALL_CAD_MODEL_INSTANCES)
				return true;
		return false;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool Get_OutputJointInformation( const CADAssemblies &in_CADAssemblies )
	{
		for each ( ProcessingInstruction i in in_CADAssemblies.processingInstructions )
			if ( i.primary == OUTPUT_JOINT_INFORMATION )
				return true;
		return false;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool Get_ValidateJointInformation( const CADAssemblies &in_CADAssemblies )
	{
		for each ( ProcessingInstruction i in in_CADAssemblies.processingInstructions )
			if ( i.secondary == VALIDATE_JOINT_INFORMATION )
				return true;
		return false;
	}


	////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void CopyModels(	//cad::CadFactoryAbstract						&cAD_Factory_ptr,
						const std::vector<CopyModelDefinition>		&in_FromModel_ToModel )
																	throw (isis::application_exception)
	{

		isis::cad::CadFactoryAbstract_global *cadFactoryAbstract_global_ptr = isis::cad::CadFactoryAbstract_global::instance();
		isis::cad::CadFactoryAbstract::ptr	cAD_Factory_ptr = cadFactoryAbstract_global_ptr->getCadFactoryAbstract_ptr();

		std::set<std::string> savedToWorkingDirSourceModels;

		for each ( CopyModelDefinition i  in in_FromModel_ToModel)

		{
			isis::cad::IModelHandling&        modelHandling = cAD_Factory_ptr->getModelHandling();

			std::string modelNameWithSuffix = 
				   ConvertToUpperCase (modelHandling.combineCADModelNameAndSuffix(i.fromModelName, i.modelType) );
				   //ConvertToUpperCase (CombineCreoModelNameAndSuffix(i.fromModelName, i.modelType) );

			// Assure that the source model is saved to the working directory only once.
			if ( savedToWorkingDirSourceModels.find( modelNameWithSuffix ) == savedToWorkingDirSourceModels.end() )
			{
				// Since the source model (i.e. fromModelName) could be anywhere in the search path, we
				// must open the source model and save it to force a copy to exist in the working directory.
				void  *p_model; 
				if ( i.modelType == CAD_MDL_PART )
					//   isis::isis_ProMdlRetrieve(i.fromModelName,CAD_MDL_PART, &p_model);
					modelHandling.cADModelRetrieve(i.fromModelName,CAD_MDL_PART, &p_model);
				else
					//   isis::isis_ProMdlRetrieve(i.fromModelName,CAD_MDL_ASSEMBLY, &p_model);
					modelHandling.cADModelRetrieve(i.fromModelName,CAD_MDL_ASSEMBLY, &p_model);
				
				//isis::isis_ProMdlSave(p_model);
				modelHandling.cADModelSave(p_model);
				savedToWorkingDirSourceModels.insert(modelNameWithSuffix);
			}

			if ( i.modelType == CAD_MDL_PART )
				//  isis::isis_ProMdlfileCopy ( CAD_MDL_PART, i.fromModelName, i.toModelName);
				modelHandling.cADModelFileCopy( CAD_MDL_PART, i.fromModelName, i.toModelName);
			else
				//  isis::isis_ProMdlfileCopy ( CAD_MDL_ASSEMBLY, i.fromModelName, i.toModelName);
				modelHandling.cADModelFileCopy( CAD_MDL_ASSEMBLY, i.fromModelName, i.toModelName);
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	void Add_DependsOn (  std::map<std::string, CADComponentData> &in_out_CADComponentData_map )
	{
		for ( std::map<std::string, CADComponentData>::iterator i(in_out_CADComponentData_map.begin());
			  i != in_out_CADComponentData_map.end();
			  ++i )
		{
			for ( std::vector<ConstraintData>::const_iterator j( i->second.constraintDef.constraints.begin());
				  j != i->second.constraintDef.constraints.end();
				  ++j )
			{
				for (	std::vector<ConstraintPair>::const_iterator k(j->constraintPairs.begin());
						k != j->constraintPairs.end();
						++k )
				{
					for (	std::vector<ConstraintFeature>::const_iterator l(k->constraintFeatures.begin());
							l != k->constraintFeatures.end();
							++l )
					{
						if (  i->first != l->componentInstanceID  ) i->second.dependsOn.insert( l->componentInstanceID );
					}
				}
			}
		}	
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	void Populate_FeatureIDs_to_ComponentInstanceID_hashtable( 
						const std::vector<std::string>	&in_AssemblyComponentIDs,
						std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map,
						std::unordered_map<IntList, std::string, ContainerHash<IntList>> &out_FeatureIDs_to_ComponentInstanceID_hashtable )
	{
		for each ( const std::string &i in in_AssemblyComponentIDs )
			out_FeatureIDs_to_ComponentInstanceID_hashtable[in_CADComponentData_map[i].componentPaths] = i;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////

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
			if ( in_CADComponentData_map[i].modelType == CAD_MDL_PART )
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
						if ( in_CADComponentData_map[k].modelType == CAD_MDL_PART ||
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
						if ( in_CADComponentData_map[k].modelType == CAD_MDL_PART ||
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
							"   Model Type:            " <<  isis::CADMdlType_string(in_CADComponentData_map[i].modelType) << std::endl <<
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
			if ( in_CADComponentData_map[i].modelType == CAD_MDL_PART )
			{
				if ( constrainedComponentIDs_perAssembly.find(i) == constrainedComponentIDs_perAssembly.end() )
				{
					// Part not constrained to at least one other part
					std::stringstream errorString;
					errorString <<
							"Part not constrained to at least one other part. For valid joint information between parts, all parts must be connected to at least one other part."  << std::endl <<
							"   Model Name:            " <<	 in_CADComponentData_map[i].name << std::endl <<
							"   Model Type:            " << isis::CADMdlType_string(in_CADComponentData_map[i].modelType)<<  std::endl <<
							"   Component Instance ID: " <<  i;
					out_Errors.push_back(errorString.str());
				}
			}
		}
	} // END CheckValidityOfJointInformation

	////////////////////////////////////////////////////////////////////////////////////////////////////////

	void PopulateMap_with_JunctionInformation_SingleJunction( 
					//cad::CadFactoryAbstract							&in_Factory,
					const std::string								&in_ComponentID, 
					const std::vector<ConstraintPair>				&in_ConstraintPairs,
					isis::cad::Junction								&out_Junction,
					std::map<std::string, isis::CADComponentData>	&in_out_CADComponentData_map )
						throw (isis::application_exception)
	{
		
		isis::cad::CadFactoryAbstract_global *cadFactoryAbstract_global_ptr = isis::cad::CadFactoryAbstract_global::instance();
		isis::cad::CadFactoryAbstract::ptr	cAD_Factory_ptr = cadFactoryAbstract_global_ptr->getCadFactoryAbstract_ptr();


		isis_LOG(lg, isis_FILE, isis_INFO) << "*************  PopulateMap_with_JunctionInformation_SingleJunction (Constraints derived from CADAssembly.xml)";

		isis_LOG(lg, isis_FILE, isis_INFO) << (std::string)in_out_CADComponentData_map[in_ComponentID].name;

		cad::IAssembler& assembler = cAD_Factory_ptr->get_assembler();

		std::vector< cad::Joint::pair_t > joint_pair_vector = assembler.extract_joint_pair_vector(in_ComponentID, in_ConstraintPairs, in_out_CADComponentData_map);
		
		for each ( ConstraintPair i in in_ConstraintPairs )
		{
			isis_LOG(lg, isis_FILE, isis_INFO) << "FeatureGeometryType_string(i.featureGeometryType): " << CADFeatureGeometryType_string(i.featureGeometryType);
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
				errorString << std::endl << "      FeatureGeometryType: " << CADFeatureGeometryType_string(i.featureGeometryType);
				for each ( ConstraintFeature j in i.constraintFeatures ) errorString << std::endl << "         ModelName: " << (std::string)in_out_CADComponentData_map[j.componentInstanceID].name <<  "  FeatureName: "  << (std::string) j.featureName;
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



	////////////////////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// gggggggggggggggg BEGIN  END Code Remove
	struct RequiredGeometriesData
	{
		// const std::vector<ProType>	geometries;
		const std::vector<e_CADFeatureGeometryType>		geometries;
		const int		geometryCount;
		//RequiredGeometriesData( const std::vector<ProType> in_Geometries, int in_GeometryCount) : geometries(in_Geometries), geometryCount(in_GeometryCount){};
		RequiredGeometriesData( const std::vector<e_CADFeatureGeometryType> in_Geometries, int in_GeometryCount) : geometries(in_Geometries), geometryCount(in_GeometryCount){};
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
					//for each ( ProType k in in_RequiredGeometries[j].geometries)
					for each ( e_CADFeatureGeometryType k in in_RequiredGeometries[j].geometries)
					{
						//if ( FeatureGeometryType_enum(i.featureGeometryType) == k )
						if ( i.featureGeometryType == k )	
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

	e_CADJointType AdjustJointTypeToCreoGeometryTypes( const std::vector<ConstraintPair> &in_ConstraintPairs,
													   cad::JointType in_JointType )
	{
			

		int counter_1 = 0;
		int counter_2 = 0;
		std::vector<RequiredGeometriesData> requiredGeometries;

		//std::vector<ProType>	geometries;
		std::vector<e_CADFeatureGeometryType>	geometries;

		switch ( in_JointType )
		{
			case  isis::cad::FIXED:
				return FIXED_JOINT;
				break;
			case  isis::cad::REVOLUTE:
				// Axis and ( plane or point)
				//geometries.push_back(PRO_AXIS);
				geometries.push_back(CAD_AXIS);
				requiredGeometries.push_back(RequiredGeometriesData( geometries, 1));

				geometries.clear();
				//geometries.push_back(PRO_SURFACE);
				//geometries.push_back(PRO_POINT);
				geometries.push_back(CAD_SURFACE);
				geometries.push_back(CAD_POINT);

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
				//geometries.push_back(PRO_POINT);
				geometries.push_back(CAD_POINT);
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
				//geometries.push_back(PRO_AXIS);
				geometries.push_back(CAD_AXIS);
				requiredGeometries.push_back(RequiredGeometriesData( geometries, 1));

				geometries.clear();
				//geometries.push_back(PRO_SURFACE);
				geometries.push_back(CAD_SURFACE);
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
				//geometries.push_back(PRO_AXIS);
				geometries.push_back(CAD_AXIS);
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
				//geometries.push_back(PRO_SURFACE);
				geometries.push_back(CAD_SURFACE);
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
					//cad::CadFactoryAbstract							&in_Factory,
					const std::vector<std::string>					&in_ListOfComponentIDsInTheAssembly, 
					std::map<std::string, isis::CADComponentData>	&in_out_CADComponentData_map,
					bool												in_Force)
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
							PopulateMap_with_JunctionInformation_SingleJunction( //in_Factory, 
																				i,
																				constraintPairs_withoutGuide,
																				j->computedJointData.junction_withoutguide,
																				in_out_CADComponentData_map);

							// ttttt						
							//j->computedJointData.jointType_withoutguide =  GetCADJointType(j->computedJointData.junction_withoutguide.joint_pair.first.type);

							j->computedJointData.jointType_withoutguide =  AdjustJointTypeToCreoGeometryTypes(j->constraintPairs, j->computedJointData.junction_withoutguide.joint_pair.first.type);
							// ttttt
							j->computedJointData.coordinateSystem_ComponentInstanceID = i;
							j->computedJointData.junctiondDefined_withoutGuide = true;
							isis_LOG(lg, isis_FILE, isis_INFO) << "   Without guide, Joint type: " << CADJointType_string(j->computedJointData.jointType_withoutguide);


						}
					}
					else
					{
						PopulateMap_with_JunctionInformation_SingleJunction(// in_Factory, 
																			i,
																			j->constraintPairs,
																			j->computedJointData.junction_withoutguide,
																			in_out_CADComponentData_map );
						j->computedJointData.junctiondDefined_withGuide = false;
						j->computedJointData.coordinateSystem_ComponentInstanceID = i;
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

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
					
					if ( in_CADComponentData_map[cID].modelType == CAD_PART )					
						tempFullPathToStepFile += "_prt.stp";
					else
						tempFullPathToStepFile += "_asm.stp";
					
					if (in_OnlyUpdateManifestForParts )
					{
						if ( in_CADComponentData_map[cID].modelType == CAD_PART )				
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
						 in_CADComponentData_map[compID].modelType == CAD_PART )
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

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	struct ConstraintSourceAndDestination
	{
		std::string Source_ComponentID_AssemblyPart;
		std::string Destination_ComponentID_Part;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// WARNING in_AssemblyComponentIDs should only be parts or assemblies from CADAssembly.xml.  If it is
	// an assembly, it would be a leaf assembly
	void PopulateMap_with_ConstrainedToInfo_per_InputXMLConstraints ( 
		    //cad::CadFactoryAbstract													&in_Factory,
			const std::string														&in_TopAssemblyComponentInstanceID,
			const std::vector<std::string>											&in_AssemblyComponentIDs,
			const std::unordered_map<IntList, std::string, ContainerHash<IntList>>	&in_FeatureIDs_to_ComponentInstanceID_hashtable,
			std::map<std::string, isis::CADComponentData>							&in_out_CADComponentData_map )
																		throw (isis::application_exception)
	{
		

		isis::cad::CadFactoryAbstract_global *cadFactoryAbstract_global_ptr = isis::cad::CadFactoryAbstract_global::instance();
		isis::cad::CadFactoryAbstract::ptr	cAD_Factory_ptr = cadFactoryAbstract_global_ptr->getCadFactoryAbstract_ptr();

		isis::cad::IModelOperations&         modelOperations = cAD_Factory_ptr->getModelOperations();

		for each (  const std::string &i in in_AssemblyComponentIDs )
		{		
			if ( in_out_CADComponentData_map[i].modelType == CAD_MDL_PART || 
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
								if ( in_out_CADComponentData_map[l->componentInstanceID].modelType == CAD_MDL_PART ||
									 ( in_out_CADComponentData_map[l->componentInstanceID].specialInstruction != CAD_SPECIAL_INSTRUCTION_HAS_KINEMATIC_JOINT ) )
								{
									j->constrainedTo_ComponentInstanceIDs_DerivedFromConstraintPairs.insert(l->componentInstanceID);
								}
								else
								{			
									std::set<std::string>	componentInstanceIDs_of_PartsReferencedByFeature_set;
									// Assembly with CAD_SPECIAL_INSTRUCTION_HAS_KINEMATIC_JOINT

									//FindPartsReferencedByFeature(	in_TopAssemblyComponentInstanceID,
									//								l->componentInstanceID,
									//								l->featureName,
									//								//FeatureGeometryType_enum(k->featureGeometryType),
									//								k->featureGeometryType,
									//								//k->featureGeometryType,
									//								in_FeatureIDs_to_ComponentInstanceID_hashtable,
									//								in_out_CADComponentData_map,
									//								componentInstanceIDs_of_PartsReferencedByFeature_set);



									modelOperations.findPartsReferencedByFeature( in_TopAssemblyComponentInstanceID,
																	l->componentInstanceID,
																	l->featureName,
																	//FeatureGeometryType_enum(k->featureGeometryType),
																	k->featureGeometryType,
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

				//if ( in_out_CADComponentData_map[i].modelType != PRO_MDL_ASSEMBLY )
				if ( in_out_CADComponentData_map[i].modelType != CAD_MDL_ASSEMBLY )
				{
					std::stringstream errorString;
					errorString <<
						"Function: " << __FUNCTION__ << ", A part model has the CADAssembly.xml tag SpecialInstruction=\"HAS_KINEMATIC_JOINT\" .  Only assemblies can have this tag."  << std::endl <<
						"   Model Name:            " <<	 in_out_CADComponentData_map[i].name << std::endl <<
						"   Model Type:            " <<	 isis::CADMdlType_string(in_out_CADComponentData_map[i].modelType)<<  std::endl <<
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
								//FindPartsReferencedByFeature(	in_TopAssemblyComponentInstanceID,
								//								l->componentInstanceID,
								//								l->featureName,
								//								//FeatureGeometryType_enum(k->featureGeometryType),
								//								k->featureGeometryType,
								//								//k->featureGeometryType,
								//								in_FeatureIDs_to_ComponentInstanceID_hashtable,
								//								in_out_CADComponentData_map,
								//								componentInstanceIDs_of_Assembly_PartsReferencedByFeature_set);

								modelOperations.findPartsReferencedByFeature(in_TopAssemblyComponentInstanceID,
																l->componentInstanceID,
																l->featureName,
																//FeatureGeometryType_enum(k->featureGeometryType),
																k->featureGeometryType,
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
								if ( in_out_CADComponentData_map[l->componentInstanceID].modelType == CAD_MDL_PART || 
									(in_out_CADComponentData_map[l->componentInstanceID].specialInstruction != CAD_SPECIAL_INSTRUCTION_HAS_KINEMATIC_JOINT )) 
								{ // If part or and treat-as-one body assembly
									j->constrainedTo_ComponentInstanceIDs_DerivedFromConstraintPairs.insert(l->componentInstanceID);
								}
								else
								{			
									//FindPartsReferencedByFeature(	in_TopAssemblyComponentInstanceID,
									//								l->componentInstanceID,
									//								l->featureName,
									//								//FeatureGeometryType_enum(k->featureGeometryType),
									//								k->featureGeometryType,
									//								//k->featureGeometryType,
									//								in_FeatureIDs_to_ComponentInstanceID_hashtable,
									//								in_out_CADComponentData_map,
									//								componentInstanceIDs_of_NonAssembly_ModelsReferencedByFeature_set);

									modelOperations.findPartsReferencedByFeature( in_TopAssemblyComponentInstanceID,
																	l->componentInstanceID,
																	l->featureName,
																	//FeatureGeometryType_enum(k->featureGeometryType),
																	k->featureGeometryType,
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
							"   Model Type:            " <<	 isis::CADMdlType_string(in_out_CADComponentData_map[i].modelType) <<  std::endl <<
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
							"   Model Type:            " <<	 isis::CADMdlType_string(in_out_CADComponentData_map[i].modelType)<<  std::endl <<
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


	bool CyPhyLeafAssembly (	const std::string								&in_ComponentInstanceID,
								std::map<std::string, isis::CADComponentData>	&in_out_CADComponentData_map )
	{

		//if ( in_out_CADComponentData_map[in_ComponentInstanceID].modelType != PRO_MDL_ASSEMBLY ) return false;
		if ( in_out_CADComponentData_map[in_ComponentInstanceID].modelType != CAD_MDL_ASSEMBLY ) return false;
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

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	void AddBoundingBoxValuesToMap( //cad::CadFactoryAbstract				&in_Factory,
						const std::vector<std::string>					&in_AssemblyComponentIDs,
						std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map) 
																				throw (isis::application_exception)
	{

		isis::cad::CadFactoryAbstract_global *cadFactoryAbstract_global_ptr = isis::cad::CadFactoryAbstract_global::instance();
		isis::cad::CadFactoryAbstract::ptr	cAD_Factory_ptr = cadFactoryAbstract_global_ptr->getCadFactoryAbstract_ptr();

		for each ( const std::string &i in in_AssemblyComponentIDs )
		{
			if ( !in_CADComponentData_map[i].boundingBox.boundingBox_Defined )
			{
				isis_CADCommon::Point_3D	boundingBox_Point_1;
				isis_CADCommon::Point_3D	boundingBox_Point_2;
				double						boundingBoxDimensions_xyz[3];

				//RetrieveBoundingBox_ComputeFirstIfNotAlreadyComputed(	i,
				//														in_CADComponentData_map,
				//														boundingBox_Point_1,
				//														boundingBox_Point_2,
				//														boundingBoxDimensions_xyz );

				isis::cad::IModelOperations&         modelOperations = cAD_Factory_ptr->getModelOperations();
				modelOperations.retrieveBoundingBox_ComputeFirstIfNotAlreadyComputed(//in_Factory,
																		i,
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


	////////////////////////////////////////////////////////////////////////////////////////////////////////
	void AddMassPropertyValuesToMap( cad::CadFactoryAbstract				&in_Factory,
						const std::vector<std::string>					&in_AssemblyComponentIDs,
						std::map<std::string, isis::CADComponentData>	&in_out_CADComponentData_map) 
																				throw (isis::application_exception)
	{
		isis::cad::IModelOperations&         modelOperations = in_Factory.getModelOperations();

		for each ( const std::string &i in in_AssemblyComponentIDs )
		{
			if ( !in_out_CADComponentData_map[i].massProperties.massProperties_RetrievalInvoked )
			{

				//MassProperties massProperties_temp;  
				//modelOperations.retrieveMassProperties(i, in_out_CADComponentData_map, massProperties_temp );
				//in_out_CADComponentData_map[i].massProperties = massProperties_temp;

				modelOperations.retrieveMassProperties(i, in_out_CADComponentData_map, in_out_CADComponentData_map[i].massProperties );

	
				/***
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

				**/
			}
		}
	}


}