/*! \file CADCreoParametricCreateAssembly.cpp
    \brief The main(...) function for CADCreoParametricCreateAssembly.exe

	CADCreoParametricCreateAssembly.exe reads an XML file that describes an assembly, and then builds the
	assembly with "Creo Parametric 3.0" (i.e. PTC's ( http://www.ptc.com/ )  parametric solids modeler).

	Pre-Conditions:

		1.	"Creo Parametric 3.0" must be installed and functioning properly on your machine.  The license for
			"Creo Parametric" does not need the the Toolkit license.  The toolkit license is only
			needed during development.

		2.	For information on configuring the system, see registry entry META_PATH + bin\CAD\Creo\0Readme - CreateAssembly.txt

		3.	The requirements for the arguments passed to  are defined in cc_CreateAssemblyInputArgumentsParser.h

		4.	The input xml must be compliant with
			...META_SVN\trunk\deploy\CAD_Installs\Proe ISIS Extensions\schemas\AssemblyInterface.xsd


	Post-Conditions:

	1.	If an exception occurs, the program will log the error message, display the error message to the console,
		and exit.

		Some reasons that exceptions could occur follow:
        - Could not locate a part/assembly.
        - Could not find a datum in a part/assembly.
        - The input XML file constrained a Non-Size-to-Fit part/assembly to a Size-to-Fit part/assembly.

	2.	If a warning occurs, the program will log the warning message, display the warning message to the console,
		and continue processing.  An example of a warning would be if the assembly would not regenerate.  The assembly
		would have been built and saved, but when opened via Creo, the anomalous models would appear in red in the
		history tree.

	3.	If no exceptions occur, the assemblies and analysis files (e.g. FEA decks) defined in the XML are built
		and saved in the working directory.



	Special Notes:

	1.	For parametric parts, if a parameter is modified, the modification applies to all instances of the component.
		Therefore, for the case where a parametric model appears more than once in the assembly, this program creates
		uniquely named models for each instance.

	2.	This program uses UDM 64 bit version to parse the XML file.  Therefore, UDM (http://repo.isis.vanderbilt.edu/UDM)
		64 bit must be installed.

*/
#include "stdafx.h"
#include <ISISConstants.h>
#include "GlobalModelData.h"
#include "cc_WindowsFunctions.h"
#include <AssemblyCreationViaInputFile.h>
#include "cc_CreateAssemblyInputArgumentsParser.h"
#include <cc_CommonUtilities.h>
#include <cc_MiscellaneousFunctions.h>
#include <sstream>
#include <fstream>
#include "cc_LoggerBoost.h"
#include "CADFactoryCreo.h"
#include <boost/asio.hpp>
#include <boost/smart_ptr.hpp>

#include <boost/thread/thread.hpp>
#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/exception/all.hpp>


int main( int argc, char *argv[] )
{
	::boost::filesystem::path original_directory = ::boost::filesystem::current_path();

	int ExitCode = 0;

	std::string			creoStartCommand;
	std::string			CADExtensionsDir;

	std::string			templateFile_PathAndFileName;
	std::stringstream	exceptionErrorStringStream;

	std::string			exeName = "CADCreoParametricCreateAssembly.exe";

	bool				promptBeforeExiting;

	bool Pro_E_Running = false;
	bool Template_Copied = false;

	bool Logging_Set_Up = false;

	bool logFileOpen = false;

	isis::CreateAssemblyInputArguments  programInputArguments;

	::boost::filesystem::path    workingDir;

	std::stringstream inputLine;

	try
	{

		// Build string of input line
        for(int i = 0; i < argc; ++i)
        {
            inputLine << argv[i] << std::string(" ");
        }

		bool regenerationSucceeded_ForAllAssemblies = true;

		// Parse Input Arguments

		programInputArguments.ParseInputArguments(argc, argv);
		programInputArguments.ThrowExecption_If_InvalidInputArguments(argc, argv);

		SetupLogging("log", programInputArguments.logFileName, false, false, programInputArguments.logVerbosity, isis_INFO );

		// In case of exception we need to know if  logging is available
		Logging_Set_Up = true;	


		// Must get the complete path to the working directory.  This is necessary because when
		// isis_ProDirectoryChange is called to change to a STEP directory, workingDir must be fully
		// defined so that isis_ProDirectoryChange can be called to return to the original directory.
		workingDir = isis::SetupWorkingDirectory( programInputArguments.workingDirectory );

		//cADPartsLibDir = programInputArguments.auxiliaryCADDirectory;
		//xmlInputFile_PathAndFileName = programInputArguments.inputXmlFileName;
		promptBeforeExiting = programInputArguments.promptBeforeExiting;

		// Log CADCreoParametricCreateAssembly version information
		std::string programName_Version_TimeStamp;
		programName_Version_TimeStamp = "CADCreoParametricCreateAssembly " + std::string(ISIS_PRODUCT_VERSION_WITH_v_AND_DOTS) + "      ";

		///////////////////
		// Add Time Stamp
		///////////////////

		programName_Version_TimeStamp += isis_CADCommon::GetDayMonthTimeYear();
		
		isis_LOG(lg, isis_FILE, isis_INFO) << programName_Version_TimeStamp;

		isis_LOG(lg, isis_FILE, isis_INFO) << "";
		isis_LOG(lg, isis_FILE, isis_INFO) << "Notes: " << isis_EOL
		  << "   1. The \"Component Instance ID\"s in this file equate to ComponentInstanceIDs in CyPhy."  << isis_EOL
		  << "   2. To map \"Component Instance ID\"s in this file to AVM-IDs, see .\\log\\CyPhy2CAD.log.";
		
		//return 0;

		time_t time_start; /* calendar time */
		time_start=time(NULL); /* get current cal time */

		// Log input line and parameters
		isis_LOG(lg, isis_FILE, isis_INFO) << "";
		isis_LOG(lg, isis_FILE, isis_INFO) 
			<< "************** Begin Input Line *****************" << isis_EOL 
		  << inputLine.str() << isis_EOL
	      << "************** End Input Line *****************" ;
		  


		if ( workingDir.generic_string().size() >= PRO_PATH_SIZE )  // PRO_PATH_SIZE   260
		{
			std::stringstream errorString;
			errorString << "WORKING_DIR string too long.  Maximum allowed number of characters: "  << PRO_PATH_SIZE - 1 << " WORKING_DIR string: " << workingDir;
					throw isis::application_exception(errorString);
		}
		isis_LOG(lg, isis_FILE, isis_INFO) << "";
		isis_LOG(lg, isis_FILE, isis_INFO) 
		  <<  isis_EOL << "************** Begin Directory Settings *****************"
		  <<  isis_EOL << "workingDir:                    "	<< workingDir
		  <<  isis_EOL << "inputXmlFileName:              "	<< programInputArguments.inputXmlFileName
		  <<  isis_EOL << "logFileName:                   "	<< programInputArguments.logFileName
		  <<  isis_EOL << "*************** End Directory Settings *****************";
		 

		isis::cad::CadFactoryAbstract::ptr cAD_Factory = isis::cad::creo::create();
		isis::cad::ICADSession&           cADSession = cAD_Factory->getCADSession();

		cADSession.setupCADEnvironment(programInputArguments,							// in 
										creoStartCommand,								// out
										CADExtensionsDir,								// out
										templateFile_PathAndFileName );					// out
  

		isis::CreateAssemblyViaInputFile(	*cAD_Factory,
											"CADCreoParametricCreateAssembly",
											ISIS_PRODUCT_VERSION_WITH_v_AND_DOTS,
											"Creo-Parametric",
											programInputArguments,
											CADExtensionsDir,
											templateFile_PathAndFileName,
											creoStartCommand,
											programName_Version_TimeStamp,
											PRO_NAME_SIZE - 1,
											PRO_PATH_SIZE - 1,
											Pro_E_Running );
	 

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

    if(ExitCode != 0)
    {
		LogMainNonZeroExitCode( exeName, ExitCode, inputLine.str(), Logging_Set_Up, programInputArguments.logFileName,  exceptionErrorStringStream.str() );
    }

	// Delete the copied template assembly file if it exists.
	// Note - Added "if ( Template_Copied )" because the function call was returning a message to the
	// console if the file did not exist.
	if ( Template_Copied )
	{
		::boost::filesystem::path deleteFile = workingDir / (isis::TEMPLATE_MODEL_NAME_METRIC + isis::TEMPLATE_MODEL_NAME_METRIC_SUFFIX);
		if (::boost::filesystem::exists(deleteFile) ) 
		{
		   ::boost::filesystem::remove(deleteFile);
		}
		// isis::IfFileExists_DeleteFile( deleteFile.generic_string() );
	}

	/////// Stop Pro/E /////////
//	try 
//	{
//		if (Pro_E_Running) isis::isis_ProEngineerEnd();
//	}
//	catch (...)
//	{
//		// Do nothing if this fails.  Creo will still exit.
//	}

	try 
	{
		if (Pro_E_Running)
		{
			isis::cad::CadFactoryAbstract::ptr cAD_Factory = isis::cad::creo::create();
			isis::cad::ICADSession&            cADsession = cAD_Factory->getCADSession();
			cADsession.stopCADProgram();
		}
	}
	catch (...)
	{
		// Do nothing if this fails.  Creo will still exit.
	}


		//isis::cad::CadFactoryAbstract::ptr cAD_Factory = isis::cad::creo::create();
		//isis::cad::ICADSession&           cADSession = cAD_Factory->getCADSession();


	if ( promptBeforeExiting )
	{
		printf("\nType Enter to exit.");
		getc(stdin);
	}

    ::boost::filesystem::current_path(original_directory);

	exit(ExitCode);
}
