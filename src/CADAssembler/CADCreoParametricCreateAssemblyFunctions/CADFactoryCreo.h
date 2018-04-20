
#ifndef CAD_FACTORY_CREO_H
#define CAD_FACTORY_CREO_H

/**
This class provides factories for the
concrete Creo CAD system.
*/
#include "cc_CadFactoryAbstract.h"
#include  <string>
#include <vector>
#include <ProSolid.h>
#include <unordered_map>

namespace isis {
namespace cad {
namespace creo {

class AssemblerCreo : public IAssembler {
private:
	ProSolid  assembly_model;

public:
	// provide the name of the concrete assembler
	std::string name() {
		return "Creo Assembler";
	}

	// std::vector<Joint> get_joints();

	/**
	For a particular component (specified by id) extract
	the set of joints which join that component to other
	components.
	*/
	std::vector< Joint::pair_t >  extract_joint_pair_vector
		(const std::string in_component_instance_id,
		 std::vector<ConstraintPair> in_vector,
		 std::map<std::string, isis::CADComponentData> &	in_map);

	/**
	The void pointer is CAD system specific.
	It should be eliminated as the strategy is completed.
	*/
	void* get_assembly_component
		( const std::string in_working_directory,
		  const std::string &	in_id, 
		  std::map<std::string, isis::CADComponentData> &	in_map);

};


class  CADSessionCreo : public ICADSession {
	public:
		std::string name() { return "CADSessionCreo";}


	void setupCADEnvironment(	
			const CreateAssemblyInputArguments &in_CreateAssemblyInputArguments,
			std::string		&out_CADStartCommand,	
			std::string		&out_CADExtensionsDir,
			std::string		&out_TemplateFile_PathAndFileName ) const throw (isis::application_exception);

	void setupCADEnvironment(	
			const MetaLinkInputArguments &in_MetaLinkInputArguments,
			std::string		&out_CADStartCommand,	
			std::string		&out_CADExtensionsDir,
			std::string		&out_TemplateFile_PathAndFileName ) const throw (isis::application_exception);


	void setupCADEnvironment(	
			const ExtractACMInputArguments &in_ExtractACMInputArguments,
			std::string		&out_CADStartCommand,	
			std::string		&out_CADExtensionsDir,
			std::string		&out_TemplateFile_PathAndFileName ) const throw (isis::application_exception);

	virtual void startCADProgram( const std::string &in_StartCommand ) const throw (isis::application_exception);
	virtual void stopCADProgram() const throw (isis::application_exception);

	virtual void getCADProgramVersion(	bool	&out_IntVersionNumber_Set,  
										int		&out_IntVersionNumber, 
										bool	&out_FloatVersionNumber_Set,
										double  &out_FloatVersionNumber )  const throw (isis::application_exception);

	virtual void setCADWorkingDirectory ( const MultiFormatString &in_MultiFormatString ) throw (isis::application_exception);

};




class  ModelHandlingCreo : public IModelHandling {
	public:
		// provide the name of the concrete assembler
		std::string name() { return "ModelHandlingCreo";}


	virtual void cADModelRetrieve(	const isis::MultiFormatString		&in_ModelName,  // model name without the suffix.  e.g  1234567  not 1234567.prt
									e_CADMdlType 						in_ModelType,      // CAD_MDL_ASSEMBLY CAD_MDL_PART,
									void 								**out_RetrievedModelHandle_ptr ) const throw (isis::application_exception);

	virtual void cADModelRetrieve(	const std::string					&in_ComponentInstanceID,		// this is included for logging purposes; otherwise this function does not use in_ComponentInstanceID	
									const isis::MultiFormatString		&in_ModelName,				// model name without the suffix.  e.g  1234567  not 1234567.prt
									e_CADMdlType 						in_ModelType,				// CAD_MDL_ASSEMBLY or CAD_MDL_PART
									const MultiFormatString				&in_GeometryRepresentation, // This would be null if no simplified representation was specified
									void 								**out_RetrievedModelHandle_ptr ) const throw (isis::application_exception);

	virtual void cADModelSave(	   void 								*in_ModelHandle ) const throw (isis::application_exception);

	virtual void cADModelFileCopy (e_CADMdlType 						in_ModelType,
								   const isis::MultiFormatString		&in_FromModelName,
								   const isis::MultiFormatString        &in_ToModelName) const throw (isis::application_exception);

	virtual void cADModelSave( 
					const std::string									&in_ComponentInstanceID,
					const std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map )
																		throw (isis::application_exception);

	// e.g. in_OrigName				Chassis_8_Wheel<Chassis>
	//		out_ModelName			Chassis
	//		out_FamilyTableEntry	Chassis_8_Wheel
	//		out_FamilyTableModel	true
	//
	// e.g. in_OrigName				bracket
	//		out_ModelName			bracket
	//		out_FamilyTableEntry	""
	//		out_FamilyTableModel	false
	virtual void extractModelNameAndFamilyTableEntry(	const std::string	&in_OrigName, 
														std::string			&out_ModelName,
														std::string			&out_FamilyTableEntry,
														bool				&out_FamilyTableModel ) const throw (isis::application_exception);

	virtual std::string buildAFamilyTableCompleteModelName ( const std::string &in_ModelName,
															 const std::string &in_FamilyTableEntry );


	// For Creo:
	//		in_ModelName = "123456789"
	//		in_ModelType = CAD_MDL_PART
	//		return "123456789.prt"
	// If in_ModelName is null string
	//	throw isis::application_exception
	virtual std::string combineCADModelNameAndSuffix ( const std::string &in_ModelName, e_CADMdlType in_ModelType )
															throw (isis::application_exception);

};




class  ModelOperationsCreo : public IModelOperations {
	public:
		std::string name() { return "ModelOperationsCreo";}

	virtual void forEachLeafAssemblyInTheInputXML_AddInformationAboutSubordinates( 
					const std::vector<std::string>					&in_ListOfComponentInstanceIDsInTheAssembly, // This includes the assembly component ID
					int												&in_out_NonCyPhyID_counter,
					std::map<std::string, isis::CADComponentData>	&in_out_CADComponentData_map )
																		throw (isis::application_exception);

	virtual void modify_CADInternalHierarchyRepresentation_CADComponentData__ForCopiedModel( 
					const std::string								&in_TopAssemblyComponentID,
					const isis::CopyModelDefinition					&in_CopyModelDefinition, 
					std::map<std::string, isis::CADComponentData>	&in_out_CADComponentData_map )
																		throw (isis::application_exception);

	virtual void populateMap_with_Junctions_and_ConstrainedToInfo_per_CADAsmFeatureTrees( 
			//cad::CadFactoryAbstract													&in_Factory,
			const std::vector<std::string>											&in_AssemblyComponentInstanceIDs,
			const std::unordered_map<IntList, std::string, ContainerHash<IntList>>	&in_FeatureIDs_to_ComponentInstanceID_hashtable,
			std::map<std::string, isis::CADComponentData>	&in_out_CADComponentData_map )
																		throw (isis::application_exception);


	virtual void retrieveTranformationMatrix_Assembly_to_Child (  
								const std::string										&in_AssemblyComponentInstanceID,
								const std::string										&in_ChildComponentID,
								const std::map<std::string, isis::CADComponentData>		&in_CADComponentData_map,  
								bool														in_bottom_up,
								double													out_TransformationMatrix[4][4] )  throw (isis::application_exception);

	virtual void retrieveTranformationMatrix_Assembly_to_Child (  
								const std::string										&in_AssemblyComponentInstanceID,
								const std::list<int>										&in_ChildComponentPaths,
								const std::map<std::string, isis::CADComponentData>		&in_CADComponentData_map,  
								bool														in_bottom_up,
								double													out_TransformationMatrix[4][4] )  throw (isis::application_exception);


	virtual void	 retrieveBoundingBox( 
								const std::string										&in_ComponentInstanceID,
								const std::map<std::string, isis::CADComponentData>		&in_CADComponentData_map,
								isis_CADCommon::Point_3D									&out_BoundingBox_Point_1,
								isis_CADCommon::Point_3D									&out_BoundingBox_Point_2,
								double													out_Dimensions_xyz[3] )
																		throw (isis::application_exception);


	virtual void ModelOperationsCreo::retrievePointCoordinates(	const std::string				&in_AssemblyComponentInstanceID,
											const std::string									&in_PartComponentID,
											const std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map,
											const MultiFormatString								&in_PointName,
											CADPoint												&out_CADPoint) 
																				throw (isis::application_exception);


	virtual void findPartsReferencedByFeature(	
						const std::string									&in_TopAssemblyComponentID, 
						const std::string									&in_ComponentInstanceID,
						const MultiFormatString								&in_FeatureName,
						e_CADFeatureGeometryType								in_FeatureGeometryType,
						const std::unordered_map<IntList, std::string, ContainerHash<IntList>>		&in_FeatureIDs_to_ComponentInstanceID_hashtable,
						const std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map,
						std::set<std::string>								&out_ComponentInstanceIDs_of_PartsReferencedByFeature_set)
																			throw (isis::application_exception);
	virtual void retrieveMassProperties( 
						const std::string									&in_ComponentInstanceID,
						const std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map,
						MassProperties										&out_MassProperties) 
																				throw (isis::application_exception);

	//virtual void  convertCADUnitToGMEUnit_Distance ( const MultiFormatString &in_DistanceUnit, 
	//												 std::string &out_ShortName, 
	//												 std::string &out_LongName  )
	//																			throw (isis::application_exception);

	//virtual void  convertCADUnitToGMEUnit_Mass ( const MultiFormatString &in_MassUnit, 
	//												 std::string &out_ShortName, 
	//												 std::string &out_LongName  )
	//																			throw (isis::application_exception);

	//virtual void convertCADUnitToGMEUnit_Force ( const MultiFormatString &in_ForceUnit, std::string &out_ShortName, std::string &out_LongName  )
	//																										throw (isis::application_exception);

	//virtual void convertCADUnitToGMEUnit_Time ( const MultiFormatString &in_TimeUnit, std::string &out_ShortName, std::string &out_LongName  )
	//																										throw (isis::application_exception);
	//virtual void convertCADUnitToGMEUnit_Temperature ( const MultiFormatString &in_TemperatureUnit, std::string &out_ShortName, std::string &out_LongName  )
	//																										throw (isis::application_exception);

	virtual void retrieveCADModelUnits( 
					const std::string									&in_ComponentInstanceID,
					const std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map,  
					CADModelUnits										&out_CADModelUnits )
																	throw (isis::application_exception);

	virtual MultiFormatString retrieveMaterialName( 	const std::string									&in_ComponentInstanceID,
													const std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map) 
																											throw (isis::application_exception);


	virtual void ModelOperationsCreo::addModelsToAssembly( 
					const std::string									&in_AssemblyComponentInstanceID,
					const std::list<std::string>							&in_ModelComponentIDsToAdd,
					std::map<std::string, isis::CADComponentData>		&in_out_CADComponentData_map,
					int													&in_out_AddedToAssemblyOrdinal)
																											throw (isis::application_exception);


	virtual bool ModelOperationsCreo::applySingleModelConstraints( 
				const std::string								&in_AssemblyComponentInstanceID,
				const std::string								&in_ComponentIDToBeConstrained,		
				std::map<std::string, isis::CADComponentData>	&in_out_CADComponentData_map )
																			throw (isis::application_exception);
	

	virtual bool	 dataExchangeFormatSupported(const DataExchangeSpecification &in_DataExchangeSpecification);

	virtual void exportDataExchangeFile_STEP( void 								*in_ModelHandle_ptr,
											  e_CADMdlType						in_ModelType,
											  const DataExchangeSpecification	&in_DataExchangeSpecification,
											  const MultiFormatString			&in_OutputDirectory,		// Only the path to the directory
											  const MultiFormatString			&in_OutputFileName_noDotSuffixes )		// This is just the name, not the path
																			throw (isis::application_exception);

	virtual void exportDataExchangeFile_Stereolithography(	void 							*in_ModelHandle_ptr,
															e_CADMdlType						in_ModelType,
															const DataExchangeSpecification	&in_DataExchangeSpecification,
															const MultiFormatString			&in_OutputDirectoryPath,		// Only the path to the directory
															const MultiFormatString			&in_OutputFileName)		    // This the complete file name (e.g. bracket_asm.stp)
																							throw (isis::application_exception);
	// Notes - Same notes as exportDataExchangeFile_STEP
	virtual void exportDataExchangeFile_Inventor(			void 							*in_ModelHandle_ptr,
															e_CADMdlType						in_ModelType,
															const DataExchangeSpecification	&in_DataExchangeSpecification,
															const MultiFormatString			&in_OutputDirectoryPath,		// Only the path to the directory
															const MultiFormatString			&in_OutputFileName)		    // This the complete file name (e.g. bracket_asm.stp)
																							throw (isis::application_exception);


	virtual void exportDataExchangeFile_DXF(					void 							*in_ModelHandle_ptr,
															e_CADMdlType						in_ModelType,
															const DataExchangeSpecification	&in_DataExchangeSpecification,
															const MultiFormatString			&in_OutputDirectoryPath,		// Only the path to the directory
															const MultiFormatString			&in_OutputFileName)		    // This the complete file name (e.g. bracket_asm.stp)
																							throw (isis::application_exception);

	// Notes - Same notes as exportDataExchangeFile_STEP
	virtual void exportDataExchangeFile_Parasolid(			void 							*in_ModelHandle_ptr,
															e_CADMdlType						in_ModelType,
															const DataExchangeSpecification	&in_DataExchangeSpecification,
															const MultiFormatString			&in_OutputDirectoryPath,		// Only the path to the directory
															const MultiFormatString			&in_OutputFileName)		    // This the complete file name (e.g. bracket_asm.stp)
																							throw (isis::application_exception);

	virtual void computePartInterferences(  const std::string									&in_AssemblyComponentInstanceID,  // This must be an assembly
											const std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map,
											std::vector<PartInterferences>						&out_PartInterferences )
																							throw (isis::application_exception);

	//virtual void computeVehicleGroundPlane( const std::string								&in_AssemblyComponentID,
	//										std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map,
	//										std::vector<isis_CADCommon::Point_3D>			&out_GroundPlanePoints )
	//																		throw (isis::application_exception);

};


class CadFactoryCreo : public CadFactoryAbstract
{
private:
	AssemblerCreo assembler;
	CADSessionCreo  cADSession;
    ModelHandlingCreo modelHandling;

	ModelOperationsCreo ModelOperationsCreo;

public:
	// provide the name of the concrete factory
	std::string name() {
		return "Creo CAD Factory";
	}
	IAssembler& get_assembler() {
		return assembler;
	};

	ICADSession& getCADSession() {
		return cADSession;
	};


	IModelHandling& getModelHandling() {
		return modelHandling;
	};


	IModelOperations& getModelOperations() {
		return ModelOperationsCreo;
	};



};

CadFactoryAbstract::ptr create();


} // creo
} // cad
} // isis

#endif // CAD_FACTORY_CREO_H