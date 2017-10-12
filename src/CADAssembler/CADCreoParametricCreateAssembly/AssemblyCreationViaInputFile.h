#ifndef ASSEMBLY_CREATION_VIA_INPUT_FILE_H
#define ASSEMBLY_CREATION_VIA_INPUT_FILE_H

#include <cc_CreateAssemblyInputArgumentsParser.h>
#include <ISISVersionNumber.h>


namespace isis
{

const std::string ASSEMBLE_PTC_VERSION = ISIS_PRODUCT_VERSION_WITH_v_AND_DOTS;

void CreateAssemblyViaInputFile( cad::CadFactoryAbstract						&in_factory,
								 const isis::CreateAssemblyInputArguments       &in_ProgramInputArguments,
								 const std::string								&in_CADExtensionsDir,
								 const std::string								&in_templateFile_PathAndFileName,
								 const std::string								&in_CreoStartCommand,
								 const std::string								&in_ProgramName_Version_TimeStamp,
								 bool											&out_Pro_E_Running ) 
																					throw (isis::application_exception);

} // END namespace isis

#endif