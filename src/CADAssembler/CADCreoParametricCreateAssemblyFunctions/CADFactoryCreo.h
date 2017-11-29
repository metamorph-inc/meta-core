
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
		(const std::string in_component_id,
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


class  EnvironmentCreo : public IEnvironment {
	public:
		// provide the name of the concrete assembler
		std::string name() { return "EnvironmentCreo";}

		//std::string getCADExtensionsDir() throw (isis::application_exception);


		// This function does any setup that is necessary before invoking the CAD application
		//virtual void setupCADEnvirnoment ( const DataContainer &in_DataContainer) throw (isis::application_exception) = 0;


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

};


class  ModelNamesCreo : public IModelNames {
	public:
		// provide the name of the concrete assembler
		std::string name() { return "ModelNamesCreo";}


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


class  ModelHandlingCreo : public IModelHandling {
	public:
		// provide the name of the concrete assembler
		std::string name() { return "ModelHandlingCreo";}


	virtual void cADModelRetrieve(	const isis::MultiFormatString		&in_ModelName,  // model name without the suffix.  e.g  1234567  not 1234567.prt
									e_CADMdlType 						in_ModelType,      // CAD_MDL_ASSEMBLY CAD_MDL_PART,
									void 								**out_RetrievedModelHandle_ptr ) const throw (isis::application_exception);

	virtual void cADModelSave(	   void 								*in_ModelHandle ) const throw (isis::application_exception);

	virtual void cADModelFileCopy (e_CADMdlType 						in_ModelType,
								   const isis::MultiFormatString		&in_FromModelName,
								   const isis::MultiFormatString        &in_ToModelName) const throw (isis::application_exception);

	virtual void cADModelSave( 
					const std::string								&in_ComponentID,
					std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map )
																		throw (isis::application_exception);
};

class  CADSessionCreo : public ICADSession {
	public:
		std::string name() { return "CADSessionCreo";}


	virtual void startCADProgram( const std::string &in_StartCommand ) const throw (isis::application_exception);
	virtual void stopCADProgram() const throw (isis::application_exception);

	virtual void getCADProgramVersion(	bool	&out_IntVersionNumber_Set,  
										int		&out_IntVersionNumber, 
										bool	&out_FloatVersionNumber_Set,
										double  &out_FloatVersionNumber )  const throw (isis::application_exception);

	virtual void setCADWorkingDirectory ( const MultiFormatString &in_MultiFormatString ) throw (isis::application_exception);

};


class  CADComponentDataStructureCreo : public ICADComponentDataStructure {
	public:
		std::string name() { return "CADComponentDataStructureCreo";}

	virtual void forEachLeafAssemblyInTheInputXML_AddInformationAboutSubordinates( 
					const std::vector<std::string>					&in_ListOfComponentIDsInTheAssembly, // This includes the assembly component ID
					int												&in_out_NonCyPhyID_counter,
					std::map<std::string, isis::CADComponentData>	&in_out_CADComponentData_map )
																		throw (isis::application_exception);

	virtual void modify_CADInternalHierarchyRepresentation_CADComponentData__ForCopiedModel( 
					const std::string								&in_TopAssemblyComponentID,
					const isis::CopyModelDefinition					&in_CopyModelDefinition, 
					std::map<std::string, isis::CADComponentData>	&in_out_CADComponentData_map )
																		throw (isis::application_exception);

};


class  AssemblyConstraintsCreo : public IAssemblyConstraints {
	public:
		std::string name() { return "AssemblyConstraintsCreo";}



	virtual void PopulateMap_with_Junctions_and_ConstrainedToInfo_per_CADAsmFeatureTrees( 
			cad::CadFactoryAbstract													&in_Factory,
			const std::vector<std::string>											&in_AssemblyComponentIDs,
			const std::unordered_map<IntList, std::string, ContainerHash<IntList>>	&in_FeatureIDs_to_ComponentInstanceID_hashtable,
			std::map<std::string, isis::CADComponentData>	&in_out_CADComponentData_map )
																		throw (isis::application_exception);

};



class CadFactoryCreo : public CadFactoryAbstract
{
private:
	AssemblerCreo assembler;
	EnvironmentCreo environment;
	ModelNamesCreo modelNames;
    ModelHandlingCreo modelHandling;
	CADSessionCreo  cADSession;
	CADComponentDataStructureCreo cADComponentDataStructureCreo;
	AssemblyConstraintsCreo   assemblyConstraintsCreo;

public:
	// provide the name of the concrete factory
	std::string name() {
		return "Creo CAD Factory";
	}
	IAssembler& get_assembler() {
		return assembler;
	};

	IEnvironment& getEnvironment() {
		return environment;
	};

	IModelNames& getModelNames() {
		return modelNames;
	};

	IModelHandling& getModelHandling() {
		return modelHandling;
	};

	ICADSession& getCADSession() {
		return cADSession;
	};

	ICADComponentDataStructure& getCADComponentDataStructure() {
		return cADComponentDataStructureCreo;
	};

	IAssemblyConstraints& getAssemblyConstraints() {
		return assemblyConstraintsCreo;
	};

};

CadFactoryAbstract::ptr create();


} // creo
} // cad
} // isis

#endif // CAD_FACTORY_CREO_H