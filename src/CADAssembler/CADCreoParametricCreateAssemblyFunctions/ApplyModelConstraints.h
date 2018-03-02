#ifndef APPLY_MODEL_CONSTRAINTS_H
#define APPLY_MODEL_CONSTRAINTS_H
#include <isis_application_exception.h>
#include <isis_ptc_toolkit_functions.h>
#include <isis_include_ptc_headers.h>
#include <string>
#include <iostream>
#include <CreoStringToEnumConversions.h>
#include <BuildAssembly.h>
#include <AssembleComponents.h>
#include <cc_CommonStructures.h>
#include "cc_CadFactoryAbstract.h"

using namespace std;

namespace isis
{


bool ApplySingleModelConstraints_Creo( 
				const std::string							&in_AssemblyComponentID,
				const std::string							&in_ComponentIDToBeConstrained,		
				std::map<string, isis::CADComponentData>		&in_CADComponentData_map )
													throw (isis::application_exception);




} // end namespace isis

#endif