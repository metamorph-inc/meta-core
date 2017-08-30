#ifndef TOOL_KIT_PASS_THROUGH_FUNCTIONS_H
#define TOOL_KIT_PASS_THROUGH_FUNCTIONS_H

#include <isis_ptc_toolkit_functions.h>
#include <isis_application_exception.h>
#include <isis_include_ptc_headers.h>
#include <cc_CommonStructures.h>
#include <string>
#include <map>

namespace isis
{
	// This function first trys to retrieve the model (part/assembly) based on the simplified  rep (in_Representation); and
	// if that fails, then the model is opened without a simplified representation.  Often, models will not have 
	// simplified reps, but if they exists, and specified via in_Representation, they will be used.  A use case would be 
	// for FEA, you would prefer the
	// simplified rep.  The input xml to this program specifies the representation (i.e. simplified rep).  In some
	// cases. it would specify Featured_Rep and in other cased DeFeatured_Rep.  Other reps would be added in the 
	// future.
	//
	//	Model Type	Reps Supported
	//	----------  --------------
	//	Part		MASTER REP
	//	Part		<user defined (e.g. Featured_Rep, Defeatured_Rep, My_rep...)>
	//	Assembly	DEFAULT ENVELOPE REP
	//  Assembly	<user defined (e.g. Featured_Rep, Defeatured_Rep, My_rep...)>
	//
	// Notes:
	//		1) in_Representation is case insensitive.  Creo supports storing case senstive simplified reps, but
	//		   the SDK is treating those in a case insensitive fashion.
	//		2) this insensitive will retrieve "Master_Rep" for parts but will NOT retrieve Master_Rep for assemblies.

    void isis_ProMdlRetrieve_WithDescriptiveErrorMsg( 
										// Added Arguments
										const std::string &in_ComponentID,
										const std::string &in_Model_Name,
										const isis::MultiFormatString &in_Representation, // Simplified representation
										// Original arguments
										const ProFamilyName name, 
										ProMdlType    type,
										ProMdl       *p_handle)
												throw (isis::application_exception); 

	void isis_ProModelitemByNameInit_WithDescriptiveErrorMsg( 
										 // Added Arguments
										const std::string &in_ComponentID,
										const std::string &in_Model_Name,
										ProMdlType     mdltype,
										 // Original arguments
										ProMdl         mdl, 
                                        ProType        type, 
                                        const ProName        name, 
                                        ProModelitem*  p_item ) 
												throw(isis::application_exception); 

	void isis_ProSolidMassPropertyGet_WithDescriptiveErrorMsg( 
						const std::string									&in_ComponentID,
						std::map<std::string, isis::CADComponentData>		&in_CADComponentData_map,
						ProMassProperty*									mass_prop )
										throw(isis::application_exception); 
} // end namespace isis

#endif