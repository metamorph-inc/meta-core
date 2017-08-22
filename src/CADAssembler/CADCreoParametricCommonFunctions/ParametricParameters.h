/*! \file ParametricParameters.h
    \brief  Functions to retrieve and set Creo model parameters.

	These functions support programmatically retrieving/changing parameters in a Creo Model.  
	The parameters affect model geometry.  For example, changing a parameter that defines the length 
	of a part feature will alter the size of the part.  See the documentation in the .h file prior
	to each function declaration for more information.
*/
#ifndef PARAMETRIC_PARAMETERS_H
#define PARAMETRIC_PARAMETERS_H

#include <isis_application_exception.h>
#include <isis_ptc_toolkit_functions.h>
#include <isis_include_ptc_headers.h>
#include <CreoStringToEnumConversions.h>
#include <string>



namespace isis
{

	extern const int MAX_STRING_PARAMETER_LENGTH;

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

void SetParametricParameter(  
				const std::string	&in_model_name,
				ProMdl				*in_p_model, 
				const std::string   in_ParameterName,
				//const std::string	in_ParameterType,
				e_CADParameterType	in_ParameterType,
				const std::string   in_ParameterValue)
									throw (isis::application_exception);


///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Description: 
//
//		The following function retrieves a Pro/E parameter from a part or assembly.  These are 
//		the parameters that are displayed in the Pro/E parameters table (i.e. Pro/E UI 
//		“Tools” “Parameters” menu option; select "Filter by" drop down option "Current and all sublevels",
//		to view all the parameters). 
//
//		The supported Pro/E Parameter-Table types are:
//			Integer
//			Real Number
//			String 
//			Yes No 
//
//		Note –	in_ParameterOwner and in_ParameterName are case insensitive. 
//
//	Pre-Conditions:
//
//		in_model_name must be populated with the name of the model containing the parameters.
//
//		in_p_model must point to an active Pro/E model
//
//		if the parameter owner is the in_p_model model (i.e. Part/Assembly), then in_ParameterOwner 
//		must be set to "MODEL"; otherwise, in_ParameterOwner must be set to the parameter owner 
//		listed in Pro/E UI “Tools” “Parameters”.
//
//	Post-Conditions:
//
//		If the combination of in_ParameterOwner and in_ParameterName does not indentify an existing
//		entry in the Pro/E parameter table, then isis::application_exception would be thrown.
//
//		If an existing entry (defined by in_ParameterOwner-in_ParameterName) is found, but 
//		the existing entry is not one of the types described in the Description section,
//		then isis::application_exception would be thrown.
//
//		Otherwise, a string is returned that contains the valued of the Pro/E parameter.

std::string GetParametricParameter(  
				const std::string	&in_model_name,
				ProMdl				*in_p_model, 
				const std::string   &in_ParameterOwner,
				const std::string   &in_ParameterName )
									throw (isis::application_exception);


///////////////////////////////////////////////////
// Description:
//   FORCE_KEY (default) : if the key is not bound it will be bound and the value, if provided, is also bound.
//   FORCE_VALUE : if the key is bound you may set the value (whether the value is bound or not).
//   FORCE_NOT: if the key is bound and the value is not bound you may bind the value.
enum e_force { 
	FORCE_KEY = 0,
	FORCE_VALUE = 1,
	FORCE_NEITHER = 2
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Description: 
//
//		The following function modifies Pro/E parameters within a model or assembly.  
//      This function is very similar to SetParametricParameter() with one exception;
//      it takes a "force" argument to indicate what is to be done in the different
//      key:value binding conditions.
//
ProError SetParametricParameter( 
	            e_force             force,
				const std::string	&in_model_name,
				ProMdl				*in_p_model, 
				const std::string   in_ParameterName,
				//const std::string	in_ParameterType,
				e_CADParameterType	in_ParameterType,
				const std::string   in_ParameterValue)
									throw (isis::application_exception);




} // End namespace isis

#endif // PARAMETRIC_PARAMETERS_H