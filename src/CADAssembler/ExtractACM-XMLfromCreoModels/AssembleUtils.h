#ifndef ASSEMBLE_UTILS_H
#define ASSEMBLE_UTILS_H
//#include <CommonStructures.h>
#include <isis_application_exception.h>
#include <fstream>
#include <map>
#include <AssembleUtils.h>

#include <boost/filesystem.hpp>

namespace isis
{
	std::string META_PATH();

	// This function hase a side effect, it changed the current working directory, and the input parameter.
	::boost::filesystem::path SetupWorkingDirectory( std::string & inout_workingDirectory );

	const std::string manufacturingManifestJson_PathAndFileName = ".\\manufacturing.manifest.json";



	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//const std::string	CouldNotFindManufacturingManifestError =
	//		"\nDid not update manufacturing manifest file (" +
	//		isis::manufacturingManifestJson_PathAndFileName + ") with " + 
	//		"\nthe mapping of component-instance-ID to STEP-file-name because the manifest file " +
	//		"\ncould not be found.  The manufacturing manifest file would only exist if the " +
	//		"\nCyPhyPrepareIFab interpreter had been invoked. Typically, CyPhyPrepareIFab " +
	//		"\nwould not have been invoked."; 

	//const std::string	NotUpdatingManufacturingManifest_SeparateSTEPFilesNotRequested =
	//		"\nDid not update manufacturing manifest file (" +
	//		isis::manufacturingManifestJson_PathAndFileName + ") for the" +
	//		"\nassembled parts because separate STEP part files were not requested." +
	//		"\nAP203_E2_SEPARATE_PART_FILES or AP214_SEPARATE_PART_FILES must be requested" +
	//		"\nin order for the manufacturing manifest to be updated for assembled parts.  The " +
	//		"\nmanifest will be updated for any unassembled parts.";


	std::string GetDayMonthTimeYear();

}

#endif