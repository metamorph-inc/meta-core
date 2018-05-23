#ifndef CC_XML_TO_CAD_STRUCTURES_H
#define CC_XML_TO_CAD_STRUCTURES_H

#include "cc_CommonStructures.h"
#include <map>
#include <vector>
#include <string>

namespace isis
{	
	enum e_XMLDefinitionFormat
	{
		XML_DEFINED_BY_FILE,
		XML_DEFINED_BY_STRING
	};

	void FromXMLFile_PopulateCADComponentAssemblyAndMap( 
					e_XMLDefinitionFormat								in_InputFormat,
																		// if XML_DEFINED_BY_FILE this would be the in_AssemblyDefinitionXML_PathAndFileName
																		// if XML_DEFINED_BY_STRING this would be a string containing the XML 
					const std::string 									&in_AssemblyDefinitionXML,  
					isis::CADAssemblies									&out_CADComponentAssemblies,
					std::map<std::string, isis::CADComponentData>		&out_CADComponentData_map )
					//std::vector<CADCreateAssemblyError>					&errorList )
										throw (isis::application_exception );


} // END namespace isis
#endif