
#ifndef CC_CAD_FACTORY_ABSTRACT_H
#define CC_CAD_FACTORY_ABSTRACT_H

#include <string>
#include "cc_Joint.h"
#include "cc_CommonStructures.h"
#include "isis_application_exception.h"
#include "cc_CreateAssemblyInputArgumentsParser.h"
#include "cc_MetaLinkInputArgumentsParser.h"
#include "cc_ExtractACMInputArgumentsParser.h"
//#include "cc_AssemblyUtilities.h"
#include <boost/smart_ptr.hpp>
#include <unordered_map>

/**
This abstract class provides factories for the
concrete CAD systems.
*/

namespace isis {
namespace cad {


//enum e_OpenMETAApplication
//{
//	OPENMETA_CREATE_ASSEMBLY,
//	OPENMETA_META_LINK,
//	OPENMETA_EXTRACT_ACM
//};

class IAssembler {
public:
	// provide the name of the concrete assembler
	virtual std::string name() = 0;

	// virtual std::vector<Joint> get_joints() = 0;
	
	/**
	Given a set of constraints infer the joint.
	*/
	virtual std::vector<Joint::pair_t>  extract_joint_pair_vector
		(const std::string in_component_id,
		 std::vector<ConstraintPair> in_component_pair_vector,
		 std::map<std::string, isis::CADComponentData> &	in_CADComponentData_map)
		 = 0;

	/**
	There is an CAD specific object for the assembly.
	This function sets that object.
	*/
	virtual void* get_assembly_component
		( const std::string in_working_directory,
		  const std::string &	in_id, 
		  std::map<std::string, isis::CADComponentData> &	in_map)
		= 0;


	// This function does any setup that is necessary before invoking the CAD application
	//virtual void SetupCADEnvirnoment ( const DataContainer &in_DataContainer) throw (isis::application_exception) = 0;

};


class IEnvironment {
public:
	// provide the name of the concrete assembler
	virtual std::string name() = 0;
	
	/****
	// Description:
	//		Get the full path to the CAD Extensions Directory.  This directory contains the executables, starter CAD models, schemas, and 
	//		templates that are necessary for the CreateAssembly and MetaLink programs to run.
	//		For Creo, this would typically be: C:\Program Files (x86)\META\bin\CAD\Creo
	// Pre-Conditions:
	//		None
	// Post-Conditions:
	//		If the registry entry META_PATH does not exist, then application_exception will be thrown;
	//		otherwise, the path to CADExtensionsDir will be returned.
	virtual std::string getCADExtensionsDir() throw (isis::application_exception) = 0;
	******/



	// Description:
	//		The setupCADEnvironment functions set up the environment that is necessary to run the CAD application.
	//		After running this command, the API call to run the CAD application should work.
	//		For Creo this command is isis::isis_ProEngineerStart(...).
	//
	//		This program also writes special files (e.g. configuration files) 
	//		that are needed for the CAD program to run.
	//
	// Pre-Conditions:
	//		None
	//
	// Post-Conditions:
	//		isis::application_exception will be thrown if
	//			1.	the supported CAD application is not found. Typically, this program only supports one version of 
	//				the CAD application (e.g. 3.0 but not 2.0 or 1.0).
	//			2.  the registry entry META_PATH does not exist. This should typically be in the registry
	//				at HKEY_LOCAL_MACHINE\SOFTWARE\META.  The META installer creates this registry entry.
	//		
	//		If no exceptions, then the CAD environment would be initialized and the out_* variables returned.
	//


	virtual void setupCADEnvironment(	

			const CreateAssemblyInputArguments &in_CreateAssemblyInputArguments,

			// This is the command or other information that would be passed to the API call that starts the CAD application
			// e.g. passed to isis::isis_ProEngineerStart(...)
			std::string		&out_CADStartCommand,	

			// This directory contains the executables, starter CAD models, schemas, and 
			// templates that are necessary for the CreateAssembly and MetaLink programs to run.
			// e.g. C:\Program Files (x86)\META\bin\CAD\Creo
			std::string		&out_CADExtensionsDir,

			// This is the CAD model name of the empty starter assembly (e.g. zzz_template_assy_mmks_creo.asm )
			// When this program creates a CAD assembly, it copies this assembly to the working directory,
			// renames it to the correct name, and adds parts/sub-assemblies.
			std::string		&out_TemplateFile_PathAndFileName ) const throw (isis::application_exception) = 0;

	virtual void setupCADEnvironment(	

			const MetaLinkInputArguments &in_MetaLinkInputArguments,

			// This is the command or other information that would be passed to the API call that starts the CAD application
			// e.g. passed to isis::isis_ProEngineerStart(...)
			std::string		&out_CADStartCommand,	

			// This directory contains the executables, starter CAD models, schemas, and 
			// templates that are necessary for the CreateAssembly and MetaLink programs to run.
			// e.g. C:\Program Files (x86)\META\bin\CAD\Creo
			std::string		&out_CADExtensionsDir,

			// This is the CAD model name of the empty starter assembly (e.g. zzz_template_assy_mmks_creo.asm )
			// When this program creates a CAD assembly, it copies this assembly to the working directory,
			// renames it to the correct name, and adds parts/sub-assemblies.
			std::string		&out_TemplateFile_PathAndFileName ) const throw (isis::application_exception) = 0;


	virtual void setupCADEnvironment(	

			const ExtractACMInputArguments &in_ExtractACMInputArguments,

			// This is the command or other information that would be passed to the API call that starts the CAD application
			// e.g. passed to isis::isis_ProEngineerStart(...)
			std::string		&out_CADStartCommand,	

			// This directory contains the executables, starter CAD models, schemas, and 
			// templates that are necessary for the CreateAssembly and MetaLink programs to run.
			// e.g. C:\Program Files (x86)\META\bin\CAD\Creo
			std::string		&out_CADExtensionsDir,

			// This is the CAD model name of the empty starter assembly (e.g. zzz_template_assy_mmks_creo.asm )
			// When this program creates a CAD assembly, it copies this assembly to the working directory,
			// renames it to the correct name, and adds parts/sub-assemblies.
			std::string		&out_TemplateFile_PathAndFileName ) const throw (isis::application_exception) = 0;

};

class IModelNames {
public:
	// provide the name of the concrete assembler
	virtual std::string name() = 0;

	virtual void extractModelNameAndFamilyTableEntry(	
		// This could be either:
		//		a) CAD model name (e.g. wheel_bearing or 123456)
		//		b) CAD model name with a family table entry 
		//		   (e.g. for Creo Chassis_8_Wheel<Chassis>, where Chassis is the model name and Chassis_8_Wheel is the family table entry)
		const std::string	&in_OrigName, 
		// Model name (e.g. wheel_bearing, 123456, Chassis)
		std::string			&out_ModelName,
		// FamilyTableEntry (e.g. "", Chassis_8_Wheel), Note - "" indicated no family table entry.
		std::string			&out_FamilyTableEntry,
		// FamilyTableModel True/False, False is no familty table entry
		bool				&out_FamilyTableModel ) const throw (isis::application_exception) = 0;

		// e.g. in Creo 
		// in_ModelName = Chassis
		// in_FamilyTableEntry = Chassis_8_Wheel
		// return Creo Chassis_8_Wheel<Chassis>
		// Note - Actual entries are CAD system dependent
		virtual std::string buildAFamilyTableCompleteModelName ( const std::string &in_ModelName,
																 const std::string &in_FamilyTableEntry ) = 0;

		virtual std::string combineCADModelNameAndSuffix ( const std::string &in_ModelName, e_CADMdlType in_ModelType )
															throw (isis::application_exception) = 0 ;


};


class IModelHandling {
public:
	// provide the name of the concrete assembler
	virtual std::string name() = 0;

	// out_RetrievedModelHandle handle is an in memory handle to the model
	virtual void cADModelRetrieve(	const isis::MultiFormatString		&in_ModelName,  // model name without the suffix.  e.g  1234567  not 1234567.prt
									e_CADMdlType 						in_ModelType,      // CAD_MDL_ASSEMBLY CAD_MDL_PART,
									void 								**out_RetrievedModelHandle_ptr ) const throw (isis::application_exception) = 0;

	// in_ModelHandle handle is an in memory handle to the model
	virtual void cADModelSave(	   void 								*in_ModelHandle ) const throw (isis::application_exception) = 0;

	virtual void cADModelFileCopy (e_CADMdlType 						in_ModelType,
								   const isis::MultiFormatString		&in_FromModelName,
								   const isis::MultiFormatString        &in_ToModelName) const throw (isis::application_exception) = 0;

	virtual void cADModelSave( 
					const std::string								&in_ComponentID,
					std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map )
																		throw (isis::application_exception) = 0;	
};



class ICADSession {
public:
	// provide the name of the concrete assembler
	virtual std::string name() = 0;

	virtual void startCADProgram( const std::string &in_StartCommand ) const throw (isis::application_exception) = 0;
	virtual void stopCADProgram( )  const throw (isis::application_exception) = 0;

	// The CAD session must have been started before calling this function.  This function retreives the version
	// number of the current CAD session.
	virtual void getCADProgramVersion(	bool	&out_IntVersionNumber_Set,  
										int		&out_IntVersionNumber, 
										bool	&out_FloatVersionNumber_Set,
										double  &out_FloatVersionNumber )  const throw (isis::application_exception) = 0;

	// For Creo, the following function is not thread safe.
	virtual void setCADWorkingDirectory ( const MultiFormatString &in_MultiFormatString ) throw (isis::application_exception) = 0;
};


class ICADComponentDataStructure {
public:
	// provide the name of the concrete assembler
	virtual std::string name() = 0;


	//	The CADAssembly.xml contains an assembly tree (i.e. hierarchy), where leafs in the 
	//	tree can be parts or sub-assemblies.  For the case where where the leaf is an assembly, 
	//	this function completes the hierarchy in in_out_CADComponentData_map such that the tree 
	//	is complete.  The in_out_NonCyPhyID_counter is a counter that is used to create component
	//	IDs for the subordinate parts/sub-assemblies. 
	virtual void forEachLeafAssemblyInTheInputXML_AddInformationAboutSubordinates( 
					const std::vector<std::string>					&in_ListOfComponentIDsInTheAssembly, // This includes the assembly component ID
					int												&in_out_NonCyPhyID_counter,
					std::map<std::string, isis::CADComponentData>	&in_out_CADComponentData_map )
																		throw (isis::application_exception) = 0;

	//	A CAD model hierarchy is a tree structure wherein an assembly points to sub-assemblies and/or parts.
	//	This function will replace one assembly or one part as defined 
	//		in_CopyModelDefinition.fromModelName
	//  	in_CopyModelDefinition.toModelName
	//	The replacement process requires two steps:
	//		1) Update the CAD assembly hiearchy (i.e. update the CAD application (e.g. Creo) data structures)
	//		2) Update the in_out_CADComponentData_map
	//	Before this function is invoked
	//		1) The in_CopyModelDefinition.toModelName must exist in the CAD search path.
	//		2) The assembly containing in_CopyModelDefinition.fromModelName must be in memory
	//		3) In the case of Creo, in_CopyModelDefinition.toModelName must be in memory


	virtual void modify_CADInternalHierarchyRepresentation_CADComponentData__ForCopiedModel( 
					const std::string								&in_TopAssemblyComponentID,
					const isis::CopyModelDefinition					&in_CopyModelDefinition, 
					std::map<std::string, isis::CADComponentData>	&in_out_CADComponentData_map )
																		throw (isis::application_exception) = 0;																		
			
};

// Forward declare, because used in CadFactoryAbstract 
class CadFactoryAbstract;

class IAssemblyConstraints {
public:
	// provide the name of the concrete assembler
	virtual std::string name() = 0;

	// This function should probably be further divided so that the following function is general (i.e. cad common (cc_)).
	// Calls to the CADFactroy would be made from the general function.
	// ?? Documenation of this function is needed.
	virtual void PopulateMap_with_Junctions_and_ConstrainedToInfo_per_CADAsmFeatureTrees( 
			cad::CadFactoryAbstract													&in_Factory,
			const std::vector<std::string>											&in_AssemblyComponentIDs,
			const std::unordered_map<IntList, std::string, ContainerHash<IntList>>	&in_FeatureIDs_to_ComponentInstanceID_hashtable,
			std::map<std::string, isis::CADComponentData>	&in_out_CADComponentData_map )
																		throw (isis::application_exception) = 0;
};


class CadFactoryAbstract {
public:
	typedef boost::shared_ptr<CadFactoryAbstract> ptr;

	// provide the name of the concrete factory
	virtual std::string name() = 0;

	/**
	Get the CAD specific concrete functor for
	manipulating the assembly.
	*/
	virtual IAssembler&					 get_assembler() = 0;
	virtual IEnvironment&				 getEnvironment() = 0;
	virtual IModelNames&				 getModelNames() = 0;
	virtual IModelHandling&				 getModelHandling() = 0;
	virtual ICADSession&				 getCADSession() = 0;
    virtual ICADComponentDataStructure&  getCADComponentDataStructure() = 0;
	virtual IAssemblyConstraints&		 getAssemblyConstraints() = 0;
};



} // cad
} // isis

#endif // CAD_FACTORY_ABSTRACT_H