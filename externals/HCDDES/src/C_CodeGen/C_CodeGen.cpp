/*** Warning Workaround ***/
#pragma warning( disable: 4251 )


/*** Included Header Files ***/
#include "C_CodeGen.h"
#include <boost/filesystem/operations.hpp>
#include <ctemplate\template.h>


/***************************************************************************************/


std::string CCodeGen::TemplatePath( void ) {
	std::string templatePath;
	// Setup the template path - if there is VCP_PATH
	if ( getenv( "VCP_PATH" ) ) {
		templatePath = std::string( getenv( "VCP_PATH" ) );
		templatePath = templatePath + std::string( "\\templates" );
	}
	// If no VCP_PATH environment variable
	else {
		templatePath = std::string( "..\\..\\templates" );
	}
	// Return the value
	return templatePath;
}


void DictionarySignals( ctemplate::TemplateDictionary *dictionary, ESMoL::CCode component ) {
	// Get all children inputs
	InputPortSet inputSet = component.CInputPort_children();
	InputPortSet::iterator inputIter = inputSet.begin();
	// Iterate over the inputs
	for( ; inputIter != inputSet.end(); inputIter++ ) {
		// Add a dictionary section
		ctemplate::TemplateDictionary* inDict = dictionary->AddSectionDictionary("INPUT_SIGNAL");
		// Get the type and add it to the dictionary
		inDict->SetValue( "SIGNAL_NAME", std::string( inputIter->name() ) );
		inDict->SetValue( "SIGNAL_TYPE", std::string( inputIter->getStringAttr( ESMoL::CInputPort::meta_CPortType ) ) );
	}

	OutputPortSet outputSet = component.COutputPort_children();
	OutputPortSet::iterator outputIter = outputSet.begin();
	for( ; outputIter != outputSet.end(); outputIter++ ) {
		// Add a dictionary section
		ctemplate::TemplateDictionary* outDict = dictionary->AddSectionDictionary("OUTPUT_SIGNAL");
		// Get the type and add it to the dictionary
		outDict->SetValue( "SIGNAL_NAME", std::string( outputIter->name() ) );
		outDict->SetValue( "SIGNAL_TYPE", std::string( outputIter->getStringAttr( ESMoL::COutputPort::meta_CPortType ) ) );
	}
}


/***************************************************************************************/


CCodeGen::CCodeGen(const std::string &outputDir, const std::string &esmolModel, const bool &debug) : _debug(debug) {
	// Override _debug if _DEBUG is defined
#ifdef _DEBUG
	this->_debug = true;
#endif

	std::cout << "***** Opening ESMoL Model: " << esmolModel << " *****\n";
	// Quality flag
	bool clean = true;
	// Start UDM using the ESMoL paradigm
	Udm::SmartDataNetwork esmolSDN( ESMoL::diagram );
	try {
		// Open the model file
		esmolSDN.OpenExisting( esmolModel, "", Udm::CHANGES_PERSIST_ALWAYS );
	} catch ( udm_exception &ex ) {
		std::cout << "Exception occured trying to open " << esmolModel << " in udm:" << std::endl;
		std::cout << ex.what() << std::endl;
		throw CCodeGenException( std::string( ex.what() ) );
	}

	// Only work on directory if not .
	if ( outputDir.compare( "." ) != 0 ) {
		// Create a path
		boost::filesystem::path outPath( outputDir );
		// See if the directory exists
		if (! boost::filesystem::exists( outPath ) ) {
			// If not, create the directory
			boost::filesystem::create_directory( outPath );
		}
	}

	//Get the root folder of the project
	ESMoL::RootFolder erf = ESMoL::RootFolder::Cast(esmolSDN.GetRootObject());
	// Get the set of DesignFolder in the root folder
	DesignFolderSet designFolderSet = erf.DesignFolder_children();
	DesignFolderSet::iterator designFolderSetIter = designFolderSet.begin();
	// For each design folder ...
	for ( ; designFolderSetIter != designFolderSet.end(); designFolderSetIter++) {
		// Get the set of children Systems
		SystemTypesSet typesSet = designFolderSetIter->SystemTypes_children();
		SystemTypesSet::iterator systemTypesIter = typesSet.begin();
		// For each system type ...
		for ( ; systemTypesIter != typesSet.end(); systemTypesIter++) {
			// Get the set of children Components
			ComponentSet componentSet = systemTypesIter->Component_children();
			ComponentSet::iterator componentIter = componentSet.begin();
			for ( ; componentIter != componentSet.end(); componentIter++ ) {
				// Get the set of children CCode objects
				ComponentBaseSet baseSet = componentIter->ComponentBase_children();
				ComponentBaseSet::iterator baseIter = baseSet.begin();
				for ( ; baseIter != baseSet.end(); baseIter++ ) {
					// Make sure it is of type CCode
					if ( ESMoL::CCode::meta == baseIter->type() ) {
						// Get the component name
						std::string componentName = componentIter->name();
						// Cast to a CCode object
						ESMoL::CCode cCode = ESMoL::CCode::Cast( *baseIter );

						// Create the header file for the component
						std::string headerName = outputDir + "\\" + componentName + std::string(".h");
						std::cout << "Header Filename: " << headerName << std::endl;
						// Initialize the header file ctemplate dictionary
						ctemplate::TemplateDictionary *headerDict = new ctemplate::TemplateDictionary( "MAIN" );
						headerDict->SetValue( "COMPONENT_NAME", componentName );
						// Setup all of the signals
						DictionarySignals( headerDict, cCode );
						// Initialize the template system
						std::string headerTemplate = this->TemplatePath() + "\\ccode_h.tpl";
						ctemplate::Template *googleTemplate = ctemplate::Template::GetTemplate( headerTemplate, ctemplate::DO_NOT_STRIP );
						std::string output;
						// Expand the output into a string
						googleTemplate->Expand( &output, headerDict );
						// Get the name of the output file
						ofstream headerFile( headerName.c_str() );
						// Write the generated code out to the file and close
						headerFile << output;
						headerFile.close();

						std::string sourceName = outputDir + "\\" + componentName + std::string(".c");
						std::cout << "Source Filename: " << sourceName << std::endl;
						// Initialize the source file ctemplate dictionary
						ctemplate::TemplateDictionary *sourceDict = new ctemplate::TemplateDictionary( "MAIN" );
						sourceDict->SetValue( "COMPONENT_NAME", componentName );
						sourceDict->SetValue( "CCODE_BODY", std::string( cCode.CodeInfo() ) );
						// Setup all of the signals
						DictionarySignals( sourceDict, cCode );
						// Initialize the template system
						std::string sourceTemplate = this->TemplatePath() + "\\ccode_src.tpl";
						googleTemplate = ctemplate::Template::GetTemplate( sourceTemplate, ctemplate::DO_NOT_STRIP );
						// Expand the output into a string
						output = "";
						googleTemplate->Expand( &output, sourceDict );
						// Get the name of the output file
						ofstream sourceFile( sourceName.c_str() );
						// Write the generated code out to the file and close
						sourceFile << output;
						sourceFile.close();

					}
				}
			} // Component Iterator
		} // SystemTypes Iterator
	} // DesignFolder Iterator
	// If all is good, close the file
	esmolSDN.CloseWithUpdate();
	// We are done
	return;
}


/***************************************************************************************/

