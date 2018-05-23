
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
		(const std::string in_component_instance_id,
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


class ICADSession {
protected:
	MultiFormatString cADWorkingDirectory;
public:
	// provide the name of the concrete assembler
	virtual std::string name() = 0;

	virtual void startCADProgram( const std::string &in_StartCommand ) const throw (isis::application_exception) = 0;
	virtual void stopCADProgram( )  const throw (isis::application_exception) = 0;

	// The CAD session must have been started before calling this function.  This function retrieves the version
	// number of the current CAD session.
	virtual void getCADProgramVersion(	bool		&out_IntVersionNumber_Set,  
										int		&out_IntVersionNumber, 
										bool		&out_FloatVersionNumber_Set,
										double  &out_FloatVersionNumber )  const throw (isis::application_exception) = 0;

	// The working directory is passed in as a command line argument to the CreateAssemble... exe.
	// cc (CAD Common) functions automatically call setCADWorkingDirectory.  setCADWorkingDirectory must persist the command line 
	// argument in ICADSession::cADWorkingDirectory.  Failure to persist this value will cause an exception when getCADWorkingDirectory
	// is invoked.
	//
	// For Creo setCADWorkingDirectory does:
	//	1)	saves in_MultiFormatString to ICADSession::cADWorkingDirectory.  This is so other functions can retrieve it via
	//		getCADWorkingDirectory
	//	2)  call the Creo toolkit API to set the working directory.  This is because Creo has the concepty of a working 
	//		directroy which must be set.  Models (parts, assemblies, STEP files...) are save to this working directory. 
	//
	// Other CAD systems may not have the concept of setting a working directory.  Other systems may simply prompt
	// or allow the user to specify where to  save the models.  If that is the case, then setCADWorkingDirectory should
	// only set ICADSession::cADWorkingDirectory.  Use getCADWorkingDirectory to determine where to save the model.
	virtual void setCADWorkingDirectory ( const MultiFormatString &in_MultiFormatString ) throw (isis::application_exception) = 0;

	// There is NO NEED to OVERRIDE this function.  There is a default implementation that returns ICADSession::cADWorkingDirectory.
	// ICADSession::cADWorkingDirectory must be set by setCADWorkingDirectory.  If this function is called prior to calling
	// setCADWorkingDirectory then application_exception will be thown.
	virtual MultiFormatString getCADWorkingDirectory () throw (isis::application_exception);


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


class IModelHandling {
public:
	// provide the name of the concrete assembler
	virtual std::string name() = 0;

	//	out_RetrievedModelHandle handle is an in memory handle to the model
	//	This function is useful for the case where a simplified represenations is not needed. 
	//	For example, reading a model (from the component library) and saving it to the current 
	//  working directory.  This example is equivalent to copying a model.
	//	This function is also used for unassembled components, which do not appear in in_CADComponentData_map
	virtual void cADModelRetrieve(	const isis::MultiFormatString		&in_ModelName,		// model name without the suffix.  e.g  1234567  not 1234567.prt
									e_CADMdlType 						in_ModelType,		// CAD_MDL_ASSEMBLY CAD_MDL_PART,
									void 								**out_RetrievedModelHandle_ptr ) const throw (isis::application_exception) = 0;

	// Use this function when retrieving a model that might have a simplified representation specified.
	// We cannot just specidfy in_ComponentInstanceID and in_CADComponentData_map because unassembled 
	// components would not be included in in_CADComponentData_map.  Unassembled components are specified
	// in CADAssembly.xml and are not placed in in_CADComponentData_map because most of the information
	// in in_CADComponentData_map is not relevant to unassembled components.
	virtual void cADModelRetrieve(	const std::string					&in_ComponentInstanceID,		// this is included for logging purposes; otherwise this function does not use in_ComponentInstanceID	
									const isis::MultiFormatString		&in_ModelName,				// model name without the suffix.  e.g  1234567  not 1234567.prt
									e_CADMdlType 						in_ModelType,				// CAD_MDL_ASSEMBLY or CAD_MDL_PART
									const MultiFormatString				&in_GeometryRepresentation, // This would be null if no simplified representation was specified
									void 								**out_RetrievedModelHandle_ptr ) const throw (isis::application_exception) = 0;


	// in_ModelHandle handle is an in memory handle to the model
	virtual void cADModelSave(	   void 									*in_ModelHandle ) const throw (isis::application_exception) = 0;

	virtual void cADModelFileCopy (e_CADMdlType 							in_ModelType,
								   const isis::MultiFormatString			&in_FromModelName,
								   const isis::MultiFormatString        &in_ToModelName) const throw (isis::application_exception) = 0;

	virtual void cADModelSave( 
					const std::string									&in_ComponentInstanceID,
					const std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map )
																		throw (isis::application_exception) = 0;	

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


// Forward declare, because used in CadFactoryAbstract 
class CadFactoryAbstract;


class IModelOperations {
public:
	// provide the name of the concrete assembler
	virtual std::string name() = 0;

	//	The CADAssembly.xml contains an assembly tree (i.e. hierarchy), where leafs in the 
	//	tree can be parts or sub-assemblies.  For the case where where the leaf is an assembly, 
	//	this function completes the hierarchy in in_out_CADComponentData_map such that the tree 
	//	is complete.  The in_out_NonCyPhyID_counter is a counter that is used to create component
	//	IDs for the subordinate parts/sub-assemblies. 
	virtual void forEachLeafAssemblyInTheInputXML_AddInformationAboutSubordinates( 
					const std::vector<std::string>					&in_ListOfComponentInstanceIDsInTheAssembly, // This includes the assembly component ID
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

	// This function should probably be further divided so that the following function is general (i.e. cad common (cc_)).
	// Calls to the CADFactroy would be made from the general function.
	// ?? Documenation of this function is needed.
	virtual void populateMap_with_Junctions_and_ConstrainedToInfo_per_CADAsmFeatureTrees( 
			//cad::CadFactoryAbstract													&in_Factory,
			const std::vector<std::string>											&in_AssemblyComponentInstanceIDs,
			const std::unordered_map<IntList, std::string, ContainerHash<IntList>>	&in_FeatureIDs_to_ComponentInstanceID_hashtable,
			std::map<std::string, isis::CADComponentData>	&in_out_CADComponentData_map )
																		throw (isis::application_exception) = 0;


	//	If in_AssemblyComponentInstanceID and/or in_ChildComponentID does not exist in in_CADComponentData_map 
	//		then
	//			isis::application_exception will be thrown
	virtual void retrieveTranformationMatrix_Assembly_to_Child (  
								const std::string										&in_AssemblyComponentInstanceID,
								const std::string										&in_ChildComponentID,
								const std::map<std::string, isis::CADComponentData>		&in_CADComponentData_map,  
								bool														in_bottom_up,
								double													out_TransformationMatrix[4][4] )  throw (isis::application_exception) = 0;

	//	If in_AssemblyComponentInstanceID does not exist in in_CADComponentData_map 
	//		then
	//			isis::application_exception will be thrown
	virtual void retrieveTranformationMatrix_Assembly_to_Child (  
								const std::string										&in_AssemblyComponentInstanceID,
								const std::list<int>										&in_ChildComponentPaths,
								const std::map<std::string, isis::CADComponentData>		&in_CADComponentData_map,  
								bool														in_bottom_up,
								double													out_TransformationMatrix[4][4] )  throw (isis::application_exception) = 0;


	// This function computes the bounding box based on excluding all geometry except for the solid geometry.
	// This means that datums, coordinate system, and sketch curves/lines would be excluded.
	virtual void	 retrieveBoundingBox( 
								const std::string										&in_ComponentInstanceID,
								const std::map<std::string, isis::CADComponentData>		&in_CADComponentData_map,
								isis_CADCommon::Point_3D									&out_BoundingBox_Point_1,
								isis_CADCommon::Point_3D									&out_BoundingBox_Point_2,
								double													out_Dimensions_xyz[3] )
																		throw (isis::application_exception) = 0;


	// The point coordinates are relative to the coordinate system in the in_AssemblyComponentInstanceID assembly
	virtual void retrievePointCoordinates(	const std::string									&in_AssemblyComponentInstanceID,
											const std::string									&in_PartComponentID,
											const std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map,
											const MultiFormatString								&in_PointName,
											CADPoint												&out_CADPoint) 
																				throw (isis::application_exception)	= 0;


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

	virtual void findPartsReferencedByFeature(	
						const std::string									&in_TopAssemblyComponentID, 
						const std::string									&in_ComponentInstanceID,
						const MultiFormatString								&in_FeatureName,
						e_CADFeatureGeometryType								in_FeatureGeometryType,
						const std::unordered_map<IntList, std::string, ContainerHash<IntList>>		&in_FeatureIDs_to_ComponentInstanceID_hashtable,
						const std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map,
						std::set<std::string>								&out_ComponentInstanceIDs_of_PartsReferencedByFeature_set)
																			throw (isis::application_exception) = 0;

	// See the ModelOperationsCreo::retrieveMassProperties funtion for how out_MassProperties should be set
	virtual void retrieveMassProperties( 
						const std::string									&in_ComponentInstanceID,
						const std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map,
						MassProperties										&out_MassProperties) 
																				throw (isis::application_exception) = 0;

	// Description:
	//		Every CAD model (assemblies and parts) should have a unit system.
	//		This function retrieves the units from the CAD model.
	// Pre-Conditions:
	//		none
	// Post-Conditions:
	//		isis::application_exception will be thrown if
	//				in_ComponentInstanceID does not represent a CAD model in memory where the in-memory address is defined in in_CADComponentData_map.
	//				The CreateAssembly framework reads the models into memory before calling this function.
	//		if no exception
	//			out_CADModelUnits is populated:
	virtual void retrieveCADModelUnits( 
					const std::string									&in_ComponentInstanceID,
					const std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map,  
					CADModelUnits										&out_CADModelUnits )
																	throw (isis::application_exception) = 0;

	// Only parts have designated materials.  Assemblies do not have designated materials; and thus, an exception will be
	// thrown if this functions is called if in_ComponentInstanceID is a assembly.
	// A part can have no material defined. For that case this function will throw an exception.
	virtual MultiFormatString retrieveMaterialName( 	const std::string									&in_ComponentInstanceID,
													const std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map) 
																											throw (isis::application_exception) = 0;

	// Description:
	//		This function determines if in_ParameterName is in the CAD Model designated by in_ComponentInstanceID.
	//		Note - A parameter can be in a part or assembly.
	// Pre-Conditions:
	//		none
	// Post-Conditions:
	//		isis::application_exception will be thrown if
	//			in_ComponentInstanceID does not represent a CAD model in memory where the in-memory address is defined in in_CADComponentData_map.
	//			The CreateAssembly framework reads the models into memory before calling this function.
	//		if no exception
	//			return true/false
	virtual bool parameterDefinedInCADModel ( const MultiFormatString									&in_ParameterName,
												const std::string										&in_ComponentInstanceID,	
												const std::map<std::string, isis::CADComponentData>		&in_CADComponentData_map ) 
																											throw (isis::application_exception) = 0;
	// Description:
	//		A parameter in a CAD model may/may-not have units assigned to the parameter.
	//		This function determines if units are assigned.  For now, only distance and angle units are determined. 
	//		Only determining distance and angle units is probably OK because almost always distance or angle units are modified
	//		via CyPhy.  Mass, force, time, and temperature units are not normally modified by CyPhy.
	//		Note - A parameter can be in a part or assembly.
	// Pre-Conditions:
	//		none
	// Post-Conditions:
	//		isis::application_exception will be thrown if
	//			1)	in_ComponentInstanceID does not represent a CAD model in memory where the in-memory address is defined in in_CADComponentData_map.
	//				The CreateAssembly framework reads the models into memory before calling this function.
	//			2)	in_ParameterName	 does not exist in in_ComponentInstanceID CAD model.  Typically, you would call parameterDefinedInCADModel
	//				before calling this function.
	//		if no exception
	//			out_CADModelUnits is set as follows:
	//				out_CADModelUnit.distanceUnit  if no units defined for parameter CAD_UNITS_DISTANCE_NA  otherwise the unit for the parameter
	//				out_CADModelUnit.massUnit =			CAD_UNITS_MASS_NA			// Always set to this for now 
	//				out_CADModelUnit.forceUnit =			CAD_UNITS_FORCE_NA  			// Always set to this for now 
	//				out_CADModelUnit.timeUnit =			CAD_UNITS_TIME_NA 			// Always set to this for now  
	//				out_CADModelUnit.temperatureUnit =  CAD_UNITS_TEMPERATURE_NA		// Always set to this for now 
	//				out_CADModelUnit.angleUnit  if no angle units defined for parameter CAD_UNITS_ANGL_NA  otherwise the unit for the parameter
	virtual void retrieveParameterUnits (	const MultiFormatString									&in_ParameterName,
											const std::string										&in_ComponentInstanceID,	
											const std::map<std::string, isis::CADComponentData>		&in_CADComponentData_map,
											CADModelUnits											&out_CADModelUnits ) 
																											throw (isis::application_exception) = 0;

	// Description:
	//		CAD systems usually provide functions for converting between units. This function invokes the particular
	//		CAD systems unit conversion capabilities.
	//		Since this function takes string as input for the in_FromUnit/in_ToUnit, it is important to assure that the strings
	//		are valid and compatible with the CAD systems unit conversion inputs.
	//		In general, you would use the following functions to retrieve the strings:
	//			std::string CADUnitsDistance_string( e_CADUnitsDistance in_Enum )
	//			std::string CADUnitsArea_string( e_CADUnitsArea in_Enum )
	//			std::string CADUnitsVolume_string( e_CADUnitsVolume in_Enum )
	//			std::string CADUnitsMass_string( e_CADUnitsMass in_Enum )
	//			std::string CADUnitsForce_string( e_CADUnitsForce in_Enum )
	//			std::string CADUnitsMoment_string( e_CADUnitsMoment in_Enum )
	//			std::string CADUnitsPressure_string( e_CADUnitsPressure in_Enum )
	//			std::string CADUnitsAcceleration_string( e_CADUnitsAcceleration in_Enum )
	//			std::string CADUnitsTemperature_string( e_CADUnitsTemperature in_Enum )
	//			std::string CADUnitsHeatCapacity_string( e_CADUnitsHeatCapacity in_Enum )
	//			std::string CADUnitsThermalConductivity_string( e_CADUnitsThermalConductivity in_Enum )
	//			std::string CADUnitsAngle_string( e_CADUnitsAngle in_Enum )
	//
	//
	//		The .h and .cpp files for the enums are auto generated. See meta-core\src\CADAssembler\CodeGenerationTools\enums
	//		for information on adding new enums.
	//
	//		The current use in this program is to convert distance units, so it is not known
	//		if the other unit systems will be converted properly.  If they are not converted 
	//		properly, then an exception will be thrown.
	//
	//		If you would like to convert other units other than the ones in the above functions,
	//		then CAD systems usually supply robust conversions functions that can take many 
	//		different types of names for units (e.g. s, sec, second, seconds) and do the conversion.
	//		Again if the conversion fails, then an exception would be thrown.
	//
	//		All of Creo unit conversions functions require a reference model.  That is why 
	//	    in_ComponentInstanceID and in_CADComponentData_map needed.  This capability 
	//		needs to be replaced with a generic unit conversion capability.  Will use Creos approach
	//		for now.  Hopefully, other CAD systems will not need a reference model.
	// Pre-Conditions:
	//		none
	// Post-Conditions:
	//		isis::application_exception will be thrown if
	//			the unit conversion fails.  This would typically be causes by
	//				a) The CAD system conversion routine does not know the unit names
	//				b) the in_FromUnit and in_ToUnit are incompatible
	//		if no exception
	//			out_ScaleFactor and out_Offset will be set
	//			out_Offset is typically zero except for temperature conversions	
	virtual void unitConversionFactorsComputation (	const std::string										&in_ComponentInstanceID,	
													const std::map<std::string, isis::CADComponentData>		&in_CADComponentData_map,
													const std::string										&in_FromUnit,
													const std::string										&in_ToUnit,
													double													&out_ScaleFactor,
													double													&out_Offset ) 
																											throw (isis::application_exception) = 0;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////// Begin: How Parameter Units are Handled ///////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Notes:
	//		1) This program only converts distance/length (e.g. mm, inch, m...) units for parameters.  This is the primary use case for parameters.
	//		2) If CADAssembly.xml contains a non distance/length dimension for a parameter, then this program would throw an exception.
	//
	//	if ( CADAssembly.xml does not have a unit set for the parameter )	// e.g. <CADParameter Name="Wall_Attachment_Plate_Length" Type="Float" Value="200" _id="id142">
	//		then																//	  <Units Value="" _id="id143" />
	//			Do no unit conversions.  
	//			Assign the parameter value in CADAssembly.xml to the parameter in the CAD Model
	//
	//		else                                                           // e.g. <CADParameter Name="Wall_Attachment_Plate_Length" Type="Float" Value="200" _id="id142">
	//																	   //	  <Units Value="mm" _id="id143" />
	//			if ( units are assigned to the parameter in the CAD Model)
	//				then
	//					Convert between the units in CADAssembly.xml to the units for the parameter in the CAD model
	//					Assign the converted parameter to the parameter in the CAD model			
	//				else
	//					if ( units are assigned to the CAD Model)
	//						then
	//							Convert between the units in CADAssembly.xml to the units for the CAD model
	//							Assign the converted parameter to the parameter in the CAD Model			
	//						else
	//							// No information to do the conversion
	//							// In Creo this would not happen. Parts/assemblies always have units by default
	//							// If another CAD system will allow you to create a part/assembly without units, then this would be a problem
	//							Assign the parameter value in CADAssembly.xml to the parameter in the CAD Model
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////// End: How Parameter Units are Handled /////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// Description:
	//		This function sets a parameter value in a CAD model
	//		Notes:
	//			1)	A parameter can be in a part or assembly.
	//			2)	For CAD_FLOAT or CAD_INTEGER in_ParameterType, note that this function does not do unit conversions.  
	//				Before calling this function unit conversions should be done such that in_ParameterValue is correct.
	//				See "How Parameter Units are Handled" in this .h file for more information
	//			3) The supported types (i.e. in_ParameterType) are:
	//				FLOAT	
	//				INTEGER
	//				BOOLEAN
	//				STRING
	//			4) in_ParameterName and in_ParameterType are case insensitive (e.g. 
	//			   in_ParameterName="HeigHT" in_ParameterType="FloAT")
	//			5) Creo limits in_ParameterValue to 79 characters, longer values will result in an exception
	//
	// Pre-Conditions:
	//		none
	// Post-Conditions:
	//		isis::application_exception will be thrown if
	//			1)	in_ComponentInstanceID does not represent a CAD model in memory where the in-memory address is defined in in_CADComponentData_map.
	//				The CreateAssembly framework reads the models into memory before calling this function.
	//			2)	in_ParameterName	 does not exist in in_ComponentInstanceID CAD model.  Typically, you would call parameterDefinedInCADModel
	//				before calling this function.
	//		if no exception
	//			in_ParameterName in in_ComponentInstanceID would be set to in_ParameterValue
	virtual void setParameter (		e_CADParameterType										in_ParameterType,
									const MultiFormatString									&in_ParameterName,
									const std::string										&in_ParameterValue,
									const std::string										&in_ComponentInstanceID,	
									const std::map<std::string, isis::CADComponentData>		&in_CADComponentData_map ) 
																											throw (isis::application_exception) = 0;


	// This function only adds in_ModelComponentIDsToAdd parts/sub-assemblies to the in_AssemblyComponentInstanceID assembly.  It does
	// not constrain the added parts/sub-assemblies.
	// This function modifies in_out_CADComponentData_map.  For Creo, The following fields are modified:
	//		in_out_CADComponentData_map[*itr].cADModel_ptr_ptr = p_model;
	//		in_out_CADComponentData_map[*itr].cADModel_hdl =	(ProSolid)*p_model;
	//		in_out_CADComponentData_map[*itr].assembledFeature = getCADAssembledFeature(assembled_feat_handle);
	//		in_out_CADComponentData_map[*itr].componentPaths.push_back((assembled_feat_handle).id);
	//		in_out_CADComponentData_map[*itr].addedToAssemblyOrdinal =	in_out_addedToAssemblyOrdinal++;
	//	For other CAD systems, only the above type fields should be modified.
	virtual void addModelsToAssembly( 
					const std::string									&in_AssemblyComponentInstanceID,
					const std::list<std::string>							&in_ModelComponentIDsToAdd,
					std::map<std::string, isis::CADComponentData>		&in_out_CADComponentData_map,
					int													&in_out_AddedToAssemblyOrdinal)
																			throw (isis::application_exception) = 0;

	// Description:
	// -----------
	//	This function constrains a single CAD model.  Constraining a CAD model entails aligning/mating features 
	//	in the assembled CAD model to features in models that are already in the assembly or features in the top assembly.  
	//	The features would be planes, axes, points, etc.
	//
	//	Implementing this function in Creo was very difficult.  This is because in Creo you must:
	//		a)	tell the systems what type of joint is being created.  For example, an axis and and an orthogonal plane 
	//			would normally define a revolute joint, but you must tell Creo that it is a revolute joint.  The system does
	//			not figure this out on its own. To know the type joint, there must be code that determines that the plane is 
	//			orthogonal to the axis.
	//		b)	order the constraints in a manner compatible to joint type.  For example, for a revolute joint, the axis 
	//			must be constrained first; otherwise, Creo will not treat the joint as a revolute joint.
	//		c)	properly orient axes.  In Creo, an axis has an orientation, which is not directly exposed via the UI; however, 
	//			from the toolkit (i.e. API) perspective the orientation must be correct to result in solvent kinematic joint.
	//		d)	treat the added model (e.g. part/sub-assembly) and the constrained to model(s) differently in the API.  The Creo 
	//			API makes a distinction of which model is the added model and thus this function must determine that.  This is 
	//			not as straightforward as one might think.  For example, the added model could be an assembly and the features used 
	//			to constrain the added assembly could be models (parts/sub-assemblies) subordinate to the added assembly. 
	//
	//	Ideally, a list of constraints could be given to the system and the system would order them properly and determine the 
	//	correct type of joint (e.g. fixed, revolute, cylindrical, planar, slider...).  Hopefully, other CAD systems will do 
	//	this and the complexity of the Creo implementation for this function will not be needed for other CAD systems.  If the
	//  complexity is needed for other CAD systems, then the Creo implementation of this function should be refactored so that
	//	most of the functionality goes through the abstract factory.
	//
	//	In general this function performs the following steps:
	//	if guide is present (i.e. in CADAssembly.xml):
	//		a)	apply the constraints including the guide.  This provides an orientation of the part.  For example, if the joint 
	//			is a revolute joint, then the guide would provide the clocking position.
	//		b)	determine the transformation to the part in space.
	//		c)	remove the constraints
	//		d)	apply the transformation
	//		e)	apply the constraints without the guide
	//
	//	if guide is not present:
	//		a) apply the constraints
	//		
	//	if InputJoint information (e.g. limits of rotation) is present (i.e. in CADAssembly.xml):
	//		a) apply the joint information in addition to the previously applied constraints
	//
	//	Regenerate - Normally, would not need to regenerate the entire assembly, just the constrained model.
	//
	// Pre-Conditions
	// --------------
	//	1) in_AssemblyComponentInstanceID and in_ComponentIDToBeConstrained must be in memory.  
	//
	// Post-Conditions
	// ---------------
	//  if no exceptions:
	//		1) in_ComponentIDToBeConstrained would be properly constrained
	//		2) if constraint features would result in a kinematic constraint, then the joint will be kinematic joint
	//		3) if limits (e.g. rotation limits) exist, then the joint will adhere to those limits.
	//
	//	an exception would occur if:
	//		1)	if the constraint features are not compatible or possible.  For example, if one model has three 
	//			orthogonal planes that are being constrained to another model but the planes are not orthogonal
	//			in the other model, then the features are not compatible.
	//		2)	if the model will not regenerate after applying the constraints.  This would indicate that the
	//			the constraints are malformed.

	// Note -	This function modifies in_out_CADComponentData_map with computed junction information.
	//			Should consider refactoring so that modifications to in_out_CADComponentData_map could
	//			be done by a cad common (cc) function that would retrieve the necessary information via
	//			abstract functions and the cc function would modify the map.
	virtual bool applySingleModelConstraints( 
				const std::string								&in_AssemblyComponentInstanceID,
				const std::string								&in_ComponentIDToBeConstrained,		
				std::map<std::string, isis::CADComponentData>	&in_out_CADComponentData_map )
																			throw (isis::application_exception) = 0;


	//	If a particular CAD system does not support a particular output format (e.g. STEP AP203_E2) then:
	//		1) this program (i.e. the overall program) does not return an error.  
	//		2) the particular output format will not be created.
	//		3) a warning message will be logged.
	//		4) in the directory that would normally contain the exported data-exchange files, _FAILED_Data_Exchange.txt will be 
	//			created and will explain that the particular CAD system does not support that export type.
	//	In general, it is the responsibility of the CyPhy Test Bench developer to assure the format that is 
	//	requested is supported by the particular CAD system.
	virtual bool	 dataExchangeFormatSupported(const DataExchangeSpecification &in_DataExchangeSpecificationt) = 0;  

	//	Notes:
	//		1)	We export files (e.g. unassembled components) that are not in in_CADComponentData_map.  Therefore, we
	//			must use the model handle. 
	//		2)  in_ModelHandle_ptr must point to a model in memory.  cc_DataExchange.c/.h retrieves a model before 
	//			calling this function.
	//		3)  The directory defined by in_OutputDirectoryPath must exist prior to calling this function. cc_DataExchange.c/.h 
	//			assures that this directory exists.
	//		4)	This function may change the working directory to in_OutputDirectory. Typically, this would be to 
	//			a sub-directory (.e.g. STEP) under the current working directory.  It is the caller's responsibility to change the
	//			the working directory back to the desired location after calling this function. cc_DataExchange.c/.h performs
	//			this change back to the original location automatically. 
	//		5)	If a format (i.e. format specified in in_DataExchangeSpecification) is not supported by the particular CAD
	//			system, then the lack of support should be logged and a filed file created, but an exception should not be thrown.  
	//			This is necessary because all formats will not be supported by all CAD systems.  The Test Bench developer will need to be 
	//			cognizant of what formats are supported by a particular CAD system.
	//			The failed file should be created in in_OutputDirectoryPath and should be called _FAILED_Data_Exchange.txt.  This file 
	//			should contain a message indicating the the particular format is not supported.
	virtual void exportDataExchangeFile_STEP(				void 							*in_ModelHandle_ptr,
															e_CADMdlType						in_ModelType,
															const DataExchangeSpecification	&in_DataExchangeSpecification,
															const MultiFormatString			&in_OutputDirectoryPath,		// Only the path to the directory
															const MultiFormatString			&in_OutputFileName)		    // This the complete file name (e.g. bracket_asm.stp)
																							throw (isis::application_exception) = 0;

	// Notes - Same notes as exportDataExchangeFile_STEP
	// Need to add control over the size of the mesh (e.g. in Creo terms: Chord height, Angle control, Step size)
	virtual void exportDataExchangeFile_Stereolithography(	void 							*in_ModelHandle_ptr,
															e_CADMdlType						in_ModelType,
															const DataExchangeSpecification	&in_DataExchangeSpecification,
															const MultiFormatString			&in_OutputDirectoryPath,		// Only the path to the directory
															const MultiFormatString			&in_OutputFileName)		    // This the complete file name (e.g. bracket_asm.stp)
																							throw (isis::application_exception) = 0;

	// Notes - Same notes as exportDataExchangeFile_STEP
	virtual void exportDataExchangeFile_Inventor(			void 							*in_ModelHandle_ptr,
															e_CADMdlType						in_ModelType,
															const DataExchangeSpecification	&in_DataExchangeSpecification,
															const MultiFormatString			&in_OutputDirectoryPath,		// Only the path to the directory
															const MultiFormatString			&in_OutputFileName)		    // This the complete file name (e.g. bracket_asm.stp)
																							throw (isis::application_exception) = 0;


	// Notes - Same notes as exportDataExchangeFile_STEP
	virtual void exportDataExchangeFile_DXF(				void 							*in_ModelHandle_ptr,
															e_CADMdlType					in_ModelType,
															const DataExchangeSpecification	&in_DataExchangeSpecification,
															const MultiFormatString			&in_OutputDirectoryPath,		// Only the path to the directory
															const MultiFormatString			&in_OutputFileName)		    // This the complete file name (e.g. bracket_asm.stp)
																							throw (isis::application_exception) = 0;

	// Notes - Same notes as exportDataExchangeFile_STEP
	virtual void exportDataExchangeFile_Parasolid(			void 							*in_ModelHandle_ptr,
															e_CADMdlType					in_ModelType,
															const DataExchangeSpecification	&in_DataExchangeSpecification,
															const MultiFormatString			&in_OutputDirectoryPath,		// Only the path to the directory
															const MultiFormatString			&in_OutputFileName)		    // This the complete file name (e.g. bracket_asm.stp)
																							throw (isis::application_exception) = 0;



	//	Pre-Conditions: 
	//		None
	//	Post-Conditions
	//		This function empties (i.e. clears) out_PartInterferences before doing any other operations.
	//
	//		If in_AssemblyComponentInstanceID is not an assembly, then isis::application_exception will be thrown
	//
	//      In the case of Creo, the interference analysis will fail some of the time because the SDK interference function fails.  
	//		For those failure cases:
	//			a)	isis::application_exception would be thrown. 
	//			b)	The WriteInterferenceReport routine will called (not by computePartInterferences, but other code in this solution) to write the results to a file.  
	//
	//		If no interferences are found, then out_PartInterferences.size() will be zero
	//		
	//		The order of PartInterferences in out_PartInterferences is the order that the particular CAD system SDK returns the interferneces.
	virtual void computePartInterferences(  const std::string									&in_AssemblyComponentInstanceID,  // This must be an assembly
											const std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map,
											std::vector<PartInterferences>						&out_PartInterferences )
																							throw (isis::application_exception) = 0;
																										

	// This function assumes:
	//	1. The assembly is of a vehicle
	//	2. The coordinate system of the vehicle is as follows:
	//		z axis pointing in the direction of backward motion of the vehicle
	//		y axis pointing upward
	//		x axis in accordance to the right-hand rule
	//	3.	Pt_0  x, y, z values
	//		Pt_1  x, y, z values
	//		Pt_2  x, y, z values
	//		Where
    //           Vector ( Pt_0 to Pt_1 )   X  Vector ( Pt_0 to Pt_2 )   would define the upward direction for a vehicle.
    //           X represents the cross product
	//	4.  For tracked vehicles, the tracks are parallel to the z-axis
	//  5.  For wheeled vehicles, the portion of the wheels touching the ground form a 
	//		plane. 
//	virtual void computeVehicleGroundPlane( const std::string								&in_AssemblyComponentID,
//											std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map,
//											std::vector<isis_CADCommon::Point_3D>			&out_GroundPlanePoints )
//																			throw (isis::application_exception) = 0;

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
	virtual ICADSession&				     getCADSession() = 0;
	virtual IModelHandling&				 getModelHandling() = 0;
    virtual IModelOperations&            getModelOperations() = 0;
};



class CadFactoryAbstract_global
{
	private:

		static CadFactoryAbstract_global *s_instance;

		CadFactoryAbstract::ptr cADFactory_ptr;

		bool factorySet;

	public:

		CadFactoryAbstract_global(): factorySet(false) {}

		static CadFactoryAbstract_global *instance();

		// Should call this function one and only one time.  Calling a second time will result in an exception.
		void setCadFactoryAbstract_ptr(isis::cad::CadFactoryAbstract::ptr in_cADFactory_ptr ) throw (isis::application_exception);

		// setCadFactoryAbstract_ptr must be called before calling this function; otherwise and exception would be thrown.
		CadFactoryAbstract::ptr getCadFactoryAbstract_ptr() throw (isis::application_exception);
};



} // cad
} // isis

#endif // CAD_FACTORY_ABSTRACT_H