#ifndef ASSEMBLY_CREATION_VIA_INPUT_FILE_H
#define ASSEMBLY_CREATION_VIA_INPUT_FILE_H

#include <InputArgumentsParser.h>
#include <ISISVersionNumber.h>


namespace isis
{

const std::string ASSEMBLE_PTC_VERSION = ISIS_PRODUCT_VERSION_WITH_v_AND_DOTS;

void CreateAssemblyViaInputFile( const isis::ProgramInputArguments              &in_ProgramInputArguments,
								 const std::string								&in_CADToolDir,
								 const std::string								&in_XMLInputFile_PathAndFileName,
								 const std::string								&in_WorkingDirector,
								 const std::string								&in_CADPartsLibDir,
								 const std::string								&in_LogFile_PathAndFileName,
								 const std::string								&in_templateFile_PathAndFileName,
								 const std::string								&in_CreoStartCommand,
								 const std::string								&in_ProgramName_Version_TimeStamp,
								 unsigned int									&in_out_UniqueNameIndex,
								 bool											&out_Pro_E_Running,
								 isis::CADAssemblies							&out_CADComponentAssemblies,
								 std::map<std::string, isis::CADComponentData>	&out_CADComponentData_map) 
																					throw (isis::application_exception);

} // END namespace isis

#endif