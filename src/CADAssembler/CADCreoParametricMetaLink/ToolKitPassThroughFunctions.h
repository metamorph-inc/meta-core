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
void isis_ProMdlRetrieve_WithDescriptiveErrorMsg(
    // Added Arguments
    const std::string &in_ComponentID,
    const std::string &in_Model_Name,
    // Original arguments
    const ProFamilyName name,
    ProMdlType    type,
    ProMdl       *p_handle)
throw(isis::application_exception);

void isis_ProModelitemByNameInit_WithDescriptiveErrorMsg(
    // Added Arguments
    const std::string &in_ComponentID,
    const std::string &in_Model_Name,
    ProMdlType     mdltype,
    // Original arguments
    ProMdl         mdl,
    ProType        type,
    const ProName        name,
    ProModelitem*  p_item)
throw(isis::application_exception);

void isis_ProSolidMassPropertyGet_WithDescriptiveErrorMsg(
    const std::string									&in_ComponentID,
    std::map<std::string, isis::CADComponentData>		&in_CADComponentData_map,
    ProMassProperty*									mass_prop)
throw(isis::application_exception);
} // end namespace isis

#endif