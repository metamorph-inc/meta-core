/*! \file CADCreoParametricDatumEditor.cpp
    \brief The main(...) function for CADCreoParametricDatumEditor.exe
   
	CADCreoParametricDatumEditor.exe reads an XML file that describes an assembly, and then builds the 
	assembly with "Creo Parametric 1.0" (i.e. PTC's ( http://www.ptc.com/ )  parametric solids modeler). 
 
 
	Pre-Conditions:

		1.	"Creo Parametric 1.0" must be installed and functioning properly on your machine.  The license for 
			"Creo Parametric 1.0" does not require Toolkit license.

		2.	No environment variables are needed; however, if another version (unsupported version) is used,
			then environment variables must be set per 0ReadMe.txt (delivered in the setup zip)

		3.	The requirements for the arguments passed to the exe follow: 

			Arg #   Required    Description

-           0       Yes         exe 

			1		Yes			-check/-edit
			
-           2       Yes         DATUM_INPUT_XML_FILE		e.g. RLC_Assembly_5_CAD.xml
		
-           3       Yes         LOG_FILE					e.g. RLC_Assembly_5_CAD.xml.log	

            4       Yes         Prompt before exiting		e.g. -exit_prompt  or -no_exit_prompt
			

		4.	The input xml must be compliant with CADDatumEditor.xsd


	Post-Conditions:

	1.	If an exception occurs, the program will log the error message, display the error message to the console, 
		and exit.  Some reasons that exceptions could occur follow:


	2.	If a warning occurs, the program will log the warning message, display the warning message to the console, 
		and continue processing.  

	3.	If no exceptions occur, the datums are created per the input xml file.

*/
#include <isis_application_exception.h>
#include <CommonUtilities.h>
#include <ISISConstants.h>
#include "CADEnvironmentSettings.h"
#include "DatumEditorRoutines.h"
#include <WindowsFunctions.h>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <map>
#include <time.h>

void DisplayUssageMessage()
{
	printf("\nUsage:   CADCreoParametricEditor.exe -<check/edit> <INPUT_XML_FILE>     <LOG_FILE>  -<exit_prompt/no_exit_prompt>");
	printf("\nExample: CADCreoParametricDatumEditor.exe -check DatumEditorInput.xml DatumEditorInput.xml.log  -exit_prompt");
	printf("\nCheck/Edit:");
	printf("\n   -check verifies that the input xml file is syntactically/semantically correct.  It does not");
	printf("\n          perform the actions in the input xml file.");
	printf("\n   -edit first performs the syntax/semantic checks and if no errors are found, then the Creo");
	printf("\n          models are edited per the input-xml-file instructions.  Errors can still occur at this");
	printf("\n          phase.  An example of an error that could still occur would be if a Creo model could not be found.");
	printf("\nExit Prompt:");
	printf("\n          -exit_prompt -    Prompt before exiting.");
	printf("\n          -no_exit_prompt - Do not prompt before exiting.");
	printf("\nError/Warning/Information Handling:");
	printf("\n   Anomalies are classified as Errors, Warnings, or Information.  All these conditions are logged.");
	printf("\n   Errors stop the execution of CADCreoParametricEditor.exe and must be corrected before proceeding.");
	printf("\n   Examples of these anomalies follow:");
	printf("\n      Errors:");
	printf("\n         <CADComponent> LibraryID not found in the <Libraries> section.");
	printf("\n         FlipDatumPlaneDirection specified for a axis, point, or coordinate system.");
	printf("\n         <CADComponent> Name not found.");
	printf("\n      Warnings:");
	printf("\n         <DeleteDatum> DatumName not found in the Creo model.");
	printf("\n      Information:");
	printf("\n         Datum deleted.");
	printf("\n         Datum created.");
	printf("\nDirectory Paths (e.g <Library DirectoryPath=\".\" ID=\"Lib1\"/>:");
	printf("\n         DirectoryPath can be the full path, the current path (i.e. .), or relative ");
	printf("\n         paths (e.g. .\\Engines)");  
	printf("\nKnown Issues:");
	printf("\n   Deleting of axes is currently not supported.  If the instructions to delete an axis or to");
	printf("\n   replace an axis are present, then an error will be returned and no further actions (e.g. editing");
	printf("\n   the Creo model) would be taken.  This is considered a -check type error.\n");
}

int main( int argc, char *argv[] )
{
    
	if ( argc != 5 )   
    {
		DisplayUssageMessage();
		//printf("\nType Enter to exit.");
		//getc(stdin);
		exit(1);
	}

	// Edit Mode check/edit
	std::string switchString = isis::ConvertToUpperCase(argv[1]);
	if ( switchString != "-CHECK" && switchString != "-EDIT" )
	{
		std::cout << std::endl << "Erroneous switch type: " << switchString << std::endl;
		DisplayUssageMessage();
		exit(1);
	}
	bool editMode = false;
	if ( switchString == "-EDIT" ) editMode = true;

	// Prompt before exiting  -exit_prompt/-no_exit_prompt 
	std::string promptString = isis::ConvertToUpperCase(argv[4]);
	if ( promptString != "-EXIT_PROMPT" && promptString != "-NO_EXIT_PROMPT" )
	{
		std::cout << std::endl << "Erroneous prompt argument: " << promptString << std::endl;
		DisplayUssageMessage();
		exit(1);
	}
	bool promptBeforeExiting = false;
	if ( promptString == "-EXIT_PROMPT" ) promptBeforeExiting = true;

	int ExitCode = 0;

	std::string		creoStartCommand;
	std::string		CADToolDir;
	std::string		workingDir;
	std::string		cADPartsLibDir;
	std::string		xmlInputFile_PathAndFileName;
	std::string		logFile_PathAndFileName;
	std::string		templateFile_PathAndFileName;

	bool Pro_E_Running = false;

	std::ofstream LogFile;

	std::cout << std::endl << "CADCreoParametricEditor "<< isis::ASSEMBLE_PTC_VERSION;
	
	//bool warningsOccurred = false;
	//bool errorsOccurred = false;

	int logFileArg = 3;

	isis::ErrorStatus	errorStatus;

	try
	{
		//////////////////
		// Open Log file
		//////////////////
		// logfile will always be the third argument
		LogFile.open (argv[logFileArg], std::ios::out | std::ios::trunc );
		if (LogFile.is_open())
			std::clog.rdbuf(LogFile.rdbuf());
		else
		{
			std::string TempError = "Could not open log file: " + std::string(argv[logFileArg]);
			throw isis::application_exception(TempError.c_str());
		} 

		std::string programName_Version_TimeStamp;
		programName_Version_TimeStamp = "CADCreoParametricDatumEditor " + isis::ASSEMBLE_PTC_VERSION + "      ";

		///////////////////
		// Add Time Stamp
		///////////////////
		time_t time_start;		// calendar time 
		time_start=time(NULL);	// get current cal time 

		programName_Version_TimeStamp += asctime(localtime(&time_start));
		// Remove the extra linefeed from the above string.
		programName_Version_TimeStamp.replace(programName_Version_TimeStamp.end() -1, programName_Version_TimeStamp.end(), "");
		std::clog << programName_Version_TimeStamp;

		std::string startingDirectory = isis_CADCommon::isis_GetCurrentDirectoryA();

		isis::SetCreoEnvirVariable_RetrieveSystemSettings(	argc, argv,
															creoStartCommand,
															xmlInputFile_PathAndFileName,
															logFile_PathAndFileName);

		// Verify that the XML file exists.
		if (!isis::FileExists( xmlInputFile_PathAndFileName.c_str()))
		{
			std::string TempError = "Could not find the datum definition xml file, file name: " + xmlInputFile_PathAndFileName;
			throw isis::application_exception(TempError.c_str());
		} 

		Udm::SmartDataNetwork parsedXMLDiagram(CADDatumEditor::diagram);
		parsedXMLDiagram.OpenExisting(xmlInputFile_PathAndFileName.c_str(),"CADDatumEditor", Udm::CHANGES_PERSIST_ALWAYS);

		isis::Log_DatumEditorDiagram_functor logDatumEditorDiagram_functor;
		isis::TraverseXMLDocument(parsedXMLDiagram, logDatumEditorDiagram_functor);

		isis::CheckDataValidity_DatumEditorDiagram_functor checkDataValidity_DatumEditorDiagram_functor;
		isis::TraverseXMLDocument(parsedXMLDiagram, checkDataValidity_DatumEditorDiagram_functor);

		isis::PopulateLibraryMap_DatumEditorDiagram_functor libraryMap;
		//executeExecuteEditOperations.operationType = isis::INTERNAL_OPERATION_POPULATE_LIB_MAP;
		isis::TraverseXMLDocument(parsedXMLDiagram, libraryMap);

		std::clog << std::endl << std::endl << "Syntax/Semantic checks completed with no errors.";
		std::cout << std::endl << std::endl << "Syntax/Semantic checks completed with no errors.";

		if ( !editMode )
		{
			std::clog << std::endl << std::endl << "Edit mode not enabled.";
			std::cout << std::endl << "Edit mode not enabled.";
		}
		else
		{
			std::clog << std::endl << std::endl << "Edit mode enabled.";
		}

		if ( editMode )
		{
	
			/////////////////////////////
			/////// Start Pro/E /////////
			/////////////////////////////

			std::cout << std::endl << std::endl << "Starting Creo-Parametric, this takes about 10 seconds...";

			char tempBuffer[1024];
			strcpy(tempBuffer, creoStartCommand.c_str() );
			isis::isis_ProEngineerStart(tempBuffer,"");

			Pro_E_Running = true;
			std::cout <<  std::endl << "Creo-Parametric successfully started." << std::endl;													


			isis::DeleteAndCreateCSysAndDatums( parsedXMLDiagram, 
										 libraryMap.libID_to_DirectoryPath_map,
										 startingDirectory,
										 errorStatus);
			parsedXMLDiagram.CloseNoUpdate();

			std::cout << std::endl << std::endl << isis::ErrorStatusMessage(errorStatus) << "  See the log file for details.  Log File: " << argv[logFileArg] << std::endl;
			std::clog << std::endl << std::endl << isis::ErrorStatusMessage(errorStatus);

			ExitCode = 0;

		} // END if ( editMode )

	} // END Try


	catch ( isis::application_exception& ex )
	{
		std::cout << std::endl << "isis:application_exception: " << ex.what() << std::endl << std::endl;
		if (LogFile.is_open()) std::clog << std::endl << "isis:application_exception: " << ex.what();
		ExitCode = 1;
	}
	catch ( std::exception& ex )
	{
		std::cout << std::endl << "std::exception: " << ex.what() << std::endl << std::endl;
		if (LogFile.is_open()) std::clog << std::endl << "std::exception: " << ex.what();
		ExitCode = -1;
	}
	catch ( ... )
	{
		std::cout << std::endl << "Caught exception (...)"  << std::endl << std::endl;
		ExitCode = -1;
	}

	
	/////// Stop Pro/E /////////
	//std::cout << std::endl << std::endl << "Stopping Creo-Parametric.";

	if (Pro_E_Running) ProEngineerEnd();
	if (LogFile.is_open())LogFile.close();



	if ( promptBeforeExiting )
	{
		printf("\nType Enter to exit.");
		getc(stdin);
	}

	exit(ExitCode);
}