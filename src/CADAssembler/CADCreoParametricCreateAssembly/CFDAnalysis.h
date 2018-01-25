#ifndef CFD_ANALYSIS_H
#define CFD_ANALYSIS_H

namespace isis
{


	enum CFD_Fidelity {
		V0 = 0,
		V1 = 1,
	};
		


	// Description: 
	//		Creates in_PathAndFileName (typically ComputedValues.xml ). An example contents follows: 
	//
	//			<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
	//			<Components ConfigurationID="|1" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="CADPostProcessingParameters.xsd">
	//				<Component ComponentID="|1" FEAElementID="" _id="id17">
	//					<Metrics _id="id18">
	//						<Metric DataFormat="SCALAR" MetricID="id-0067-00000001" Type="CoefficientOfDrag" Units="" _id="id19"/>
	//					</Metrics>
	//				</Component>
	//			</Components>
	//
	// Pre-Conditions: 
	//		in_CADAssemblies must point to a valid sis::CADAssemblies type
	//		in_out_CADComponentData_map must be a valid map
	// Post-Conditions:
	//		If in_CADAssemblies contains not CFD computation request, throw isis::application_exception
	//		if in_PathAndFileName could not be opened, throw isis::application_exception
	//		If no exceptions, in_PathAndFileName would be created.
	void CreateXMLFile_ComputedValues_CFD( 
					const std::string									&in_PathAndFileName,
					const isis::CADAssemblies							&in_CADAssemblies,
					std::map<std::string, isis::CADComponentData>		&in_CADComponentData_map )
																	throw (isis::application_exception);

	void CFD_Driver(//cad::CadFactoryAbstract								&in_Factory,
					const CFD_Fidelity									in_fidelity,
					const std::string									&in_ExtensionDirectory,
					const std::string									&in_WorkingDirectory,
					const isis::TopLevelAssemblyData						&in_TopLevelAssemblyData,
					std::map<std::string, isis::CADComponentData>		&in_CADComponentData_map )
																				throw (isis::application_exception);

} // end namespace isis


#endif
