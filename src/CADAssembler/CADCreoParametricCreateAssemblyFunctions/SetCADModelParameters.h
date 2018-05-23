#ifndef SET_CAD_MODEL_PARAMTERS_H
#define SET_CAD_MODEL_PARAMTERS_H

#include <isis_application_exception.h>
#include <isis_ptc_toolkit_functions.h>
#include <isis_include_ptc_headers.h>
//#include "AssemblyInterface.hxx"
#include <cc_CommonStructures.h>
#include <string>
#include <list>
#include <map>


namespace isis
{


///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Description: 
//
//		The following function modifies Pro/E parameters within a model or assembly.  These are 
//		the parameters that are displayed in the Pro/E parameters table (displayed via 
//		“Tools” “Parameters” menu option).  
//
//		Example XML follows:
//
//		<CADComponent ComponentID="100000001" Name="bracket_plate" Type="ASSEMBLY">
//			<CADComponent ComponentID="100000002" Name="bracket_01" Type="Part">
//				<ParametricParameters>
//					<CADParameter Name="Height" Type="Float" Value="3000"> 
//						<Units Value="MM"/>  
//					</CADParameter>
//					<CADParameter Name="Count" Type="Integer" Value="555"/>
//					<CADParameter Name="Show_Wheels" Type="Boolean" Value="False"/>
//				</ParametricParameters>
//				<Constraint>
//					<Pair Fe...
//
//		The supported types are:
//			FLOAT	
//			INTEGER
//			BOOLEAN
//
//		Note – 1)	Type and Name are case insensitive (e.g. Name="HeigHT" Type="FloAT")
//			   2)	Units are optional and are not used by this function.  If they are set, 
//					they would be ignored.  The units will always be in the units of the Pro/E model.
//
//	Pre-Conditions:
//
//		in_model_name must be populated with the name of the model containing the parameters.
//
//		in_p_model must point to an active Pro/E model
//
//		in_ParametricParameters must point to a xml tree defined with the contents of 
//		<ParametricParameters> … </ParametricParameters>
//
//		The type in the xml file and the type in the Pro/E parameters table must align as follows:
//
//			XML File		Pro/E Model
//			-----------		------------
//			FLOAT			Real Number
//			BOOLEAN			Yes No
//			INTEGER			Integer
//
//	Post-Condtions:
//
//		If the parameter does not already exist in the Pro/E model, then 
//		isis::application_exception would be thrown.
//
//		If the parameter types do not align as described in the pre-conditions, 
//		then in some cases isis::application_exception  would be thrown, and in 
//		other cases the type in Pro/E would be modified.  In general, the XML 
//		file should be constructed so this does not happen.
//
//		Otherwise, the parameter would be modified in the Pro/E Model.

void ApplyParametricParameters( const std::list<std::string>						&in_ComponentIDs, 
								const std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map,
								std::vector<isis::CADCreateAssemblyError>			&out_ErrorList)
																		throw (isis::application_exception);

} // End namespace isis

#endif // PARAMETRIC_PARAMETERS_H