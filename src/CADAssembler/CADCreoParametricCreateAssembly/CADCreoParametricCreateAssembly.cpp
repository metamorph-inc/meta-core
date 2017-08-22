/*! \file CADCreoParametricCreateAssembly.cpp
    \brief The main(...) function for CADCreoParametricCreateAssembly.exe

	CADCreoParametricCreateAssembly.exe reads an XML file that describes an assembly, and then builds the
	assembly with "Creo Parametric 1.0 or 2.0" (i.e. PTC's ( http://www.ptc.com/ )  parametric solids modeler).

	Pre-Conditions:

		1.	"Creo Parametric 1.0 or 2.0" must be installed and functioning properly on your machine.  The license for
			"Creo Parametric" does not need the the Toolkit license.  The toolkit license is only
			needed during development.

		2.	The environment variables must be set per
			...META_Trunk_Working\deploy\CAD_Installs\Proe ISIS Extensions\0Readme - CreateAssembly.txt

		3.	The requirements for the arguments passed to the exe follow:

				-w  Working directory.  The directory where the generated files would be persisted.
				-i  Input XML file name.  This file defines the CAD assembly definition.
				-a  Auxiliary CAD directory.  Would contain CAD parts.
				-l  Log file name.
				-v  Log priority level (verbosity).
				-c  Log configuration file name.
				-p  Prompt before exiting.  Not prompting is the default.
				-g  Graphics mode.  No graphics is the default.
				-h  Help, Display keys along with the usage.
				-s  host:port combination for meta-link-bridge
				-d  Name of model that is synchronized between Creo and CyPhy
				-m  Mode which is being used {m | m in {"design", "component", "passive"}

				Required Keys: -w
				Key Order: No particular order required
				Key Grouping:  -p and -g may be grouped (e.g. -pg)
				Keys are case insensitive.

			Note - an example bat file is located at
			...META_Trunk_Working\deploy\CAD_Installs\Proe ISIS Extensions\docs\examples\CADCreoParametricCreateAssembly.bat

		4.	The input xml must be compliant with
			...META_SVN\trunk\deploy\CAD_Installs\Proe ISIS Extensions\schemas\AssemblyInterface.xsd


	Post-Conditions:

	For the non -s mode:
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


	For the -s argument, the errors and warnings are returned to the client (i.e. CyPhy)
    The -s argument requires the -g argument (graphics mode)
	The -s argument requires the -m argument to set the major-mode which is to
	"design": (default (implied by -s)) edit an assembly design
	"component": edit a component


	Special Notes:

	1.	For parametric parts, if a parameter is modified, the modification applies to all instances of the component.
		Therefore, for the case where a parametric model appears more than once in the assembly, this program creates
		uniquely named models for each instance.

	2.	This program uses UDM 64 bit version to parse the XML file.  Therefore, UDM (http://repo.isis.vanderbilt.edu/UDM)
		64 bit must be installed.


*/
#include "stdafx.h"
//#include <BuildAssembly.h>
#include <ISISConstants.h>
#include "GlobalModelData.h"

#include "CADEnvironmentSettings.h"
//#include "WindowsHDependentCommonFunctions.h"
#include "WindowsFunctions.h"
#include <AssemblyCreationViaInputFile.h>
#include "InputArgumentsParser.h"
#include <MiscellaneousFunctions.h>
#include <sstream>
#include "LoggerBoost.h"

#include <boost/asio.hpp>
//#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>

#include <boost/thread/thread.hpp>
#include <iostream>

#include <boost/filesystem.hpp>

void SetupLogging(const std::string &in_Logfilename, isis_LogSeverityLevel in_LogSeverityLevel)
{
	std::string logfilenamepath = "log\\"+ in_Logfilename;

	if (!isis_CADCommon::DirectoryExists("log"))
	{
		isis_CADCommon::isis_CreateDirectory("log");
	}

	isis::isis_DeleteFile(logfilenamepath);

	init_logging_boost(	false, false, in_LogSeverityLevel, isis_INFO, logfilenamepath);

}

int main( int argc, char *argv[] )
{
	::boost::filesystem::path original_directory = ::boost::filesystem::current_path();

	int ExitCode = 0;

	std::string			creoStartCommand;
	std::string			CADToolDir;

	std::string			templateFile_PathAndFileName;
	std::stringstream	exceptionErrorStringStream;
	bool				promptBeforeExiting;

	bool Pro_E_Running = false;
	bool Template_Copied = false;

	bool Logging_Set_Up = false;

	bool logFileOpen = false;

	isis::ProgramInputArguments  programInputArguments;
	::boost::filesystem::path    workingDir;

	try
	{
		bool regenerationSucceeded_ForAllAssemblies = true;

		// Parse Input Arguments
		isis::ParseInputArguments(argc, argv, programInputArguments);
		isis::ThrowExecption_If_InvalidInputArguments(argc, argv, programInputArguments);

		SetupLogging(programInputArguments.logFileName, programInputArguments.logVerbosity);

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
		programName_Version_TimeStamp = "CADCreoParametricCreateAssembly " + isis::ASSEMBLE_PTC_VERSION + "      ";

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
		std::ostringstream inputLine;
		for ( int i = 0; i < argc; ++i) inputLine << argv[i] << std::string(" ");
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
		 
		bool graphicsModeOn = false;
		bool creoExceptInputFromThisProgramAndCreoUI = false;

		if (programInputArguments.graphicsModeOn) graphicsModeOn = true;
		if (programInputArguments.synchronizeWithCyPhy) creoExceptInputFromThisProgramAndCreoUI = true;

		isis::SetCreoEnvirVariable_RetrieveSystemSettings(	graphicsModeOn,
															creoExceptInputFromThisProgramAndCreoUI,
															creoStartCommand,
															CADToolDir,
															templateFile_PathAndFileName );

		std::map<std::string, isis::CADComponentData> CADComponentData_map;
		isis::CADAssemblies CADComponentAssemblies;


		   unsigned int UniqueNameIndex = 1;

		   isis::CreateAssemblyViaInputFile(	programInputArguments,
												CADToolDir,
												programInputArguments.inputXmlFileName,
												workingDir.generic_string(),
												programInputArguments.auxiliaryCADDirectory,
												programInputArguments.logFileName,
												templateFile_PathAndFileName,
												creoStartCommand,
												programName_Version_TimeStamp,
												UniqueNameIndex,
												Pro_E_Running,
												CADComponentAssemblies,
												CADComponentData_map);

		 


	} // END Try
    catch ( isis::application_exception& ex )
	{
		exceptionErrorStringStream  << std::endl << "application error: " << ex.what();
		ExitCode = -1;
	}
	catch ( std::exception& ex )
	{
		exceptionErrorStringStream << std::endl  << "general exception: " << ex.what();
		ExitCode = -2;
	}
	catch ( ... )
	{
		exceptionErrorStringStream << "unspecified throwable (...):  Please report the error to the help desk.";
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
			failedTxtFileStream <<  isis_CADCommon::GetDayMonthTimeYear() << ", CADCreoParametricCreateAssembly.exe error code: " << ExitCode << ". Error was: " << exceptionErrorStringStream.str();
			failedTxtFileStream << std::endl;
			failedTxtFileStream.close();
		}

		if (Logging_Set_Up)
		{
			
			isis_LOG(lg, isis_CONSOLE_FILE, isis_ERROR) << exceptionErrorStringStream.str();
		}
		else
		{
			std::cerr <<  std::endl << std::endl << exceptionErrorStringStream.str() << std::endl << std::endl;
		}	
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
	if (Pro_E_Running) ProEngineerEnd();

	if ( promptBeforeExiting )
	{
		printf("\nType Enter to exit.");
		getc(stdin);
	}

    ::boost::filesystem::current_path(original_directory);

	exit(ExitCode);
}
