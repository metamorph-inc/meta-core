#ifndef ASSEMBLY_CREATION_VIA_INPUT_FILE_H
#define ASSEMBLY_CREATION_VIA_INPUT_FILE_H

#include <ISISVersionNumber.h>


namespace isis
{


void CreateAssemblyViaInputFile( cad::CadFactoryAbstract						&in_Factory,
								 const std::string								&in_CreateAssemblyProgramName,    // e.g. CADCreoParametricCreateAssembly
								 const std::string								&in_CreateAssemblyProgramVersion, // e.g. v1.5.23.0
								 const std::string								&in_CADApplicationName,			  // e.g. CreoParametric
								 const isis::CreateAssemblyInputArguments       &in_ProgramInputArguments,
								 const std::string								&in_CADExtensionsDir,
								 const std::string								&in_templateFile_PathAndFileName,
								 const std::string								&in_CreoStartCommand,
								 const std::string								&in_ProgramName_Version_TimeStamp,
								 unsigned int									in_MaxCADModelNameLength,   // This is max model name length/size (not including the . suffix (e.g. .prt)) ) 
								 unsigned int									in_MaxCADPathLength,  // This is the max length of the directory and file name path.
								 bool											&out_Pro_E_Running )

																					throw (isis::application_exception);

} // END namespace isis

#endif