/*! \file ExtractACM-XMLfromCreoModels.cpp
    \brief The main(...) function for ExtractACM-XMLfromCreoModels.exe

	ExtractACM-XMLfromCreoModels.exe opens an assembly with "Creo Parametric 3.0" (i.e. PTC's ( http://www.ptc.com/ )  parametric solids modeler) 
	and calls ISIS added library functions to export an XML files representing that assembly for use with GME.

	Pre-Conditions:

		1.	"Creo Parametric 3.0" must be installed and functioning properly on your machine.  The license for
			"Creo Parametric" does not need the the Toolkit license.  The toolkit license is only
			needed during development.

		2.	For information on configuring the system, see registry entry META_PATH + bin\CAD\Creo\0Readme - CreateAssembly.txt

		3.	The requirements for the arguments passed to  are defined in cc_ExtractACMInputArgumentsParser.h


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
#include "cc_ExtractACMInputArgumentsParser.h"
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

#include <boost/thread/thread.hpp>

#include <boost/filesystem.hpp>
#include <boost/exception/all.hpp>

#include <iostream>

#include <boost/filesystem.hpp>

#include "CreoModelToCyphyXML.h"

#include "CommonFeatureUtils.h"
#include <cc_CommonUtilities.h>
#include "cc_CADFactoryAbstract.h"
#include "CADFactoryCreo.h"


static ProError ProTermAction( ProeTerminationStatus term_type )
{
	std::stringstream errorString;
	errorString << "Creo-Parametric has terminated with status : " << term_type;
						throw isis::application_exception("Creo-Parametric has terminated with status : " + term_type);
	return PRO_TK_NO_ERROR;
}

// determine if the creo file name is a part file or an assembly file
// kind of tricky since creo adds a ".number" extension for revision control
// returns true if the file is ASM, false if not (i.e. it is a PRT or unknown type)
static bool fileISasm(string sourcefile)
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

static string getpartname(string sourcefile)
{
	int location = sourcefile.rfind(".asm");
	if (location == string::npos)
    {
		location = sourcefile.rfind(".prt");
	}
	return sourcefile.substr(0, location);
}


int ExtractACM_XML_main( int argc, char *argv[] )
{
	const std::string ASSEMBLE_PTC_VERSION = ISIS_PRODUCT_VERSION_WITH_v_AND_DOTS;

	int ExitCode = 0;

	std::string			creoStartCommand; 
	std::string			CADExtensionsDir;

	std::string			templateFile_PathAndFileName;
	std::stringstream	exceptionErrorStringStream;
	bool				promptBeforeExiting;

	bool Pro_E_Running = false;

	isis::ExtractACMInputArguments  programInputArguments;
	programInputArguments.logFileName = "DefaultLogFile.log";
	::boost::filesystem::path    workingDir;

	std::string			exeName = "ExtractACM-XMLfromCreoModels.exe";

	std::stringstream inputLine;


	try
	{

		// Build string of input line
        for(int i = 0; i < argc; ++i)
        {
            inputLine << argv[i] << std::string(" ");
        }

		cout << "ExtractACM-XMLfromCreoModels "<< ASSEMBLE_PTC_VERSION;

// STEP 1: Parse Input Arguments
		programInputArguments.ParseInputArguments(argc, argv);

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
		
		// ***********************  Begin: Setup one one and only one time per exe ***********************************
		isis::cad::CadFactoryAbstract_global cadFactoryAbstract_global;
		isis::cad::CadFactoryAbstract_global *cadFactoryAbstract_global_ptr = isis::cad::CadFactoryAbstract_global::instance();
		// Call the following function one and only one time per executable.
		cadFactoryAbstract_global_ptr->setCadFactoryAbstract_ptr(isis::cad::creo::create());
		// ***********************  End: Setup sone one and only one time per exe *************************************

		isis::cad::CadFactoryAbstract::ptr	cAD_Factory_ptr = cadFactoryAbstract_global_ptr->getCadFactoryAbstract_ptr();
		//isis::cad::CadFactoryAbstract::ptr cAD_Factory = isis::cad::creo::create();
		isis::cad::ICADSession&           cADSession = cAD_Factory_ptr->getCADSession();


		cADSession.setupCADEnvironment(	programInputArguments,
											creoStartCommand,								// out
											CADExtensionsDir,								// out
											templateFile_PathAndFileName );					// out


// STEP 3: Start Creo in async mode
			char tempBuffer[1024];

			strcpy(tempBuffer, creoStartCommand.c_str() );

			cout << endl << "Starting Creo-Parametric, this takes about 10 seconds..." << endl ;

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


	catch( boost::program_options::required_option& ex)
	{
        exceptionErrorStringStream  << "application error: Missing required option when invoking " << exeName << ", " << boost::diagnostic_information(ex);
		exceptionErrorStringStream << std::endl << "Input Line: " <<  inputLine;
        ExitCode = -1;
	}
	catch( boost::program_options::error& ex)
	{
        exceptionErrorStringStream  << "application error: Error with the options passed to " << exeName << ", " << boost::diagnostic_information(ex);
		exceptionErrorStringStream << std::endl << "Input Line: " <<  inputLine;
        ExitCode = -2;
	}
	catch (boost::exception &ex)
	{
		exceptionErrorStringStream  << "application error: " << boost::diagnostic_information(ex);
        ExitCode = -3;
	}

    catch(isis::application_exception& ex)
    {
        exceptionErrorStringStream  << "application error: " << ex.what();
        ExitCode = -4;
    }
    catch(std::exception& ex)
    {
        exceptionErrorStringStream << "general exception: " << ex.what();
        ExitCode = -5;
    }
    catch(...)
    {
        exceptionErrorStringStream << "unspecified error, caught with (...):  Please report this error to the help desk.";
        ExitCode = -6;
    }

 
	if ( ExitCode != 0 )
	{
		LogMainNonZeroExitCode( exeName, ExitCode, inputLine.str(), false, programInputArguments.logFileName,  exceptionErrorStringStream.str() );

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