/*! \file ExtractACM-XMLfromCreoModels.cpp
    \brief The main(...) function for ExtractACM-XMLfromCreoModels.exe

	ExtractACM-XMLfromCreoModels.exe opens an assembly with "Creo Parametric 2.0" (i.e. PTC's ( http://www.ptc.com/ )  parametric solids modeler) 
	and calls ISIS added library functions to export an XML files representing that assembly for use with GME.

	Pre-Conditions:

		1.	"Creo Parametric 1.0 or 2.0" must be installed and functioning properly on your machine.  The license for
			"Creo Parametric" does not need the the Toolkit license.  The toolkit license is only
			needed during development.

		2.	The environment variables must be set per
			...META_Trunk_Working\deploy\CAD_Installs\Proe ISIS Extensions\0Readme - CreateAssembly.txt

		3.	The requirements for the arguments passed to the exe follow:

				Required
				-c  input CAD file name.  This file defines the CAD assembly definition for Creo.
				-x  Output XML file name.  This file defines the CAD assembly definition for GME.

				Optional
				-p  Prompt before exiting.  Not prompting is the default.
				-g  Graphics mode.  No graphics is the default.
				-h  Help, Display keys along with the usage.

				Required Keys: -c, -x
				Optional Keys: -p, -g
				Key Order: No particular order required
				Key Grouping:  -p and -g may be grouped (e.g. -pg)
				Keys are case insensitive.

	Post-Conditions:

		1.	If an exception occurs, the program will display the error message to the console,
			and exit.

			Some reasons that exceptions could occur follow:

-              Could not locate a part/assembly.

-              Missing required parameters.

-              Could not start Creo properly.

	2.	This program uses UDM 64 bit version to parse the XML file.  Therefore, UDM (http://repo.isis.vanderbilt.edu/UDM)
		64 bit must be installed.


*/
#include <isis_application_exception.h>
#include <CommonUtilities.h>
#include <ISISConstants.h>

#include <AssembleUtils.h>

#include "CADEnvironmentSettings.h"
#include "InputArgumentsParser.h"
#include "cc_WindowsFunctions.h"
#include <ISISVersionNumber.h>

#include <string>
#include <sstream>
#include <iostream>
#include <time.h>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <iostream>

#include <boost/filesystem.hpp>

#include "CreoModelToCyphyXML.h"

#include "CommonFeatureUtils.h"
#include <cc_CommonUtilities.h>


ProError ProTermAction( ProeTerminationStatus term_type )
{
	std::stringstream errorString;
	errorString << "Creo-Parametric has terminated with status : " << term_type;
						throw isis::application_exception("Creo-Parametric has terminated with status : " + term_type);
	return PRO_TK_NO_ERROR;
}

// determine if the creo file name is a part file or an assembly file
// kind of tricky since creo adds a ".number" extension for revision control
// returns true if the file is ASM, false if not (i.e. it is a PRT or unknown type)
bool fileISasm(string sourcefile)
{
	int locateASM = sourcefile.rfind(".asm");
    int locatePRT = sourcefile.rfind(".prt");

    if (locateASM > locatePRT)
    {
		return true;
	}
    else
    {
		return false;
	}
}

string getpartname(string sourcefile)
{
	int location = sourcefile.rfind(".asm");
	if (location == string::npos)
    {
		location = sourcefile.rfind(".prt");
	}
	return sourcefile.substr(0, location);
}


int main( int argc, char *argv[] )
{
	const std::string ASSEMBLE_PTC_VERSION = ISIS_PRODUCT_VERSION_WITH_v_AND_DOTS;

	int ExitCode = 0;

	std::string			creoStartCommand; 
	std::string			CADExtensionsDir;

	std::string			templateFile_PathAndFileName;
	std::stringstream	exceptionErrorStringStream;
	bool				promptBeforeExiting;

	bool Pro_E_Running = false;

	isis::ProgramInputArguments  programInputArguments;
	programInputArguments.logFileName = "DefaultLogFile.log";
	::boost::filesystem::path    workingDir;

	try
	{
		cout << "ExtractACM-XMLfromCreoModels "<< ASSEMBLE_PTC_VERSION;

// STEP 1: Parse Input Arguments
		isis::ParseInputArguments(argc, argv, programInputArguments);

		promptBeforeExiting = programInputArguments.promptBeforeExiting;

// STEP 2: Configure the Creo start command string
		// Must get the complete path to the working directory.  This is necessary because when
		// isis_ProDirectoryChange is called to change to a STEP directory, workingDir must be fully
		// defined so that isis_ProDirectoryChange can be called to return to the original directory.
		workingDir = isis::SetupWorkingDirectory( programInputArguments.workingDirectory );

		if ( workingDir.generic_string().size() >= PRO_PATH_SIZE )  // PRO_PATH_SIZE   260
		{
			std::stringstream errorString;
			errorString << "WORKING_DIR string too long.  Maximum allowed number of characters: "  << PRO_PATH_SIZE - 1 << " WORKING_DIR string: " << workingDir;
					throw isis::application_exception(errorString);
		}
		::boost::filesystem::current_path(workingDir);

		// configure start string
		bool graphicsModeOn = programInputArguments.graphicsModeOn?true:false;
		bool creoAcceptInputFromThisProgramAndCreoUI = programInputArguments.synchronizeWithCyPhy?true:false;

		isis::SetCreoEnvirVariable_RetrieveSystemSettings(	graphicsModeOn,
															creoAcceptInputFromThisProgramAndCreoUI,
															creoStartCommand,
															CADExtensionsDir,
															templateFile_PathAndFileName );

// STEP 3: Start Creo in async mode
			char tempBuffer[1024];

			strcpy(tempBuffer, creoStartCommand.c_str() );

			cout << endl << "Starting Creo-Parametric takes about 10 seconds, be patient..." << endl ;

			ProError err = isis::isis_ProEngineerStart(tempBuffer,"");

			Pro_E_Running = true;

			// callback in case Creo ends with an error
			ProTermFuncSet(ProTermAction);

// STEP 4: Open the file
			ProMdl mdl;
			isis::MultiFormatString partstr(getpartname(programInputArguments.inputCADFileName));
			isis_ProMdlRetrieve(partstr, fileISasm(programInputArguments.inputCADFileName) ? PRO_MDL_ASSEMBLY : PRO_MDL_PART, &mdl);

// STEP 5: Request creo give us an output XML for use in CyPhy
			std::string xmlbuffer = isis::CreoModelToCyphyXML(mdl);

			if(xmlbuffer=="")
			{
				std::stringstream errorString;
				errorString << "CreotoCyPhy call resulted in an empty string. Unable to convert file";
				throw isis::application_exception(errorString);
			}

			// the above call returns a text buffer.  Dump that buffer to a file
			ofstream xmlfile;
			std::string xmlfilename;

			if(programInputArguments.outputXmlFileName == "")
			{
				xmlfilename = programInputArguments.inputCADFileName+".xml";
//				cout <<  endl << "No output file specified, creating new XML file: " << xmlfilename << std::endl ;
			}
			else
			{
				xmlfilename = programInputArguments.outputXmlFileName;
			}
			cout << "Writing to XML file: " << xmlfilename << std::endl << std::endl;
			xmlfile.open (xmlfilename);
			xmlfile << xmlbuffer;
			xmlfile.close();

//// JGG W.I.P. - Create list of part files fould in the assembly
//			// determine if we have an assembly file
//			// ASSUMING that by this point any invalid file type would have barfed by now
//			//  so the file is either .prt or .asm at this point
//			if( fileISasm(programInputArguments.inputCADFileName) )
//			{
//				isis::CreoModelAssemblyAttributes local_out_AssemblyHierarchy;
//				const ProSolid	local_p_solid_handle = (ProSolid)mdl;
//				isis::RetrieveAssemblyHierarchyInformation ( local_p_solid_handle,
//														true,
//														local_out_AssemblyHierarchy  );
//							// Temporary Check
//				cout << "Parsing assembly file..." << std::endl;
//				for each ( isis::CreoModelAssemblyAttributes j in local_out_AssemblyHierarchy.children )
//				{
//					cout << "Found assemply part:  " << j.modelname << std::endl;
//				}
//			}
	} // END Try


    catch ( isis::application_exception& ex )
	{
		exceptionErrorStringStream  << "ERROR: " << ex.what();
		ExitCode = -1;
	}
	catch ( std::exception& ex )
	{
		exceptionErrorStringStream << "ERROR: " << ex.what();
		ExitCode = -2;
	}
	catch ( ... )
	{
		exceptionErrorStringStream << "ERROR: std::exception: Caught exception (...).  Please report the error to the help desk.";
		ExitCode = -3;
	}

	if ( ExitCode != 0 )
	{
		// Write to _FAILED.txt
		std::string failedTxtFileName = "_FAILED.txt";
		bool addLineFeed = false;
		if ( isis::FileExists( failedTxtFileName.c_str() )) addLineFeed = true;

		ofstream failedTxtFileStream;
		failedTxtFileStream.open (failedTxtFileName, ios::app );
		if ( failedTxtFileStream.is_open() )
		{
			if ( addLineFeed ) failedTxtFileStream << std::endl;
			failedTxtFileStream <<  isis::GetDayMonthTimeYear() << ", ExtractACM-XMLfromCreoModels.exe error code: " << ExitCode ;
			failedTxtFileStream << exceptionErrorStringStream << std::endl;
			switch(ExitCode)
			{
			case -1:
				failedTxtFileStream << "This error means a problem running Creo Parametric. Make sure the license server is reachable. Make sure the input file is valid." << std::endl;
				break;
			case -2:
				failedTxtFileStream << "This error code means a problem with this executable.  Double check the input parameters." << std::endl;
				break;
			case -3:
			default:
				failedTxtFileStream << "This error code means an 'other' problem occured.  Check environment and environment variables." << std::endl;
				break;
			}

			failedTxtFileStream.close();
		}

		std::cerr << std::endl << std::endl << exceptionErrorStringStream.str() << std::endl << std::endl;
	}

		/////// Stop Pro/E /////////
		if (Pro_E_Running) isis::isis_ProEngineerEnd();

		if ( promptBeforeExiting )
		{
			printf("\nType Enter to exit.");
			getc(stdin);
		}
	exit(ExitCode);
}