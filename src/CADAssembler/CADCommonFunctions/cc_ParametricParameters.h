#ifndef cc_PARAMETRIC_PARAMETERS_H
#define cc_PARAMETRIC_PARAMETERS_H

#include <string>
#include "isis_application_exception.h"
#include "cc_CommonStructures.h"


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
//
//		The supported types (i.e. in_ParameterType) are:
//			FLOAT	
//			INTEGER
//			BOOLEAN
//			STRING
//
//		Note –	in_ParameterName and in_ParameterType are case insensitive (e.g. 
//				in_ParameterName="HeigHT" in_ParameterType="FloAT")
//
//	Pre-Conditions:
//
//		in_model_name must be populated with the name of the model containing the parameters.
//
//		in_p_model must point to an active Pro/E model
//
//		in_ParameterType and the type in the Pro/E parameters table must align as follows:
//
//			in_ParameterType	Pro/E Model
//			-----------			------------
//			FLOAT				Real Number
//			BOOLEAN				Yes No
//			INTEGER				Integer
//			STRING				String
//
//	Post-Conditions:
//
//		If in_ParameterName does not already exist in the Pro/E model, then 
//		isis::application_exception would be thrown.
//
//		if in_ParameterType == STRING 
//			if (in_ParameterValue length) > 79
//				isis::application_exception  would be thrown
//
//		If the parameter types do not align as described in the pre-conditions, 
//		then in some cases isis::application_exception  would be thrown, and in 
//		other cases the type in Pro/E would be modified.  In general, in_ParameterType
//		should be set so this does not happen.
//
//		Otherwise, the parameter would be modified in the Pro/E Model.
void SetParametricParameterInCADModel ( const std::string									&in_ComponentInstanceID, 
										const std::map<std::string, isis::CADComponentData>	&in_CADComponentData_map,
										const CADParameter									&in_CADParameter )
																						throw (isis::application_exception);

} // END namespace isis


#endif