/*! \file CADCreoParametricComputeMetrics.cpp
    \brief The main(...) function for CADCreoParametricComputeMetrics.exe

	This program gathers metrics about Creo models.  The gathered metrics are volume, surface area, 
	density, mass, CGx, CGy, and CGz.  The program also supports incrementing model parameters and 
	gathering the metrics after each increment.  The metrics are written to csv file(s).

	The input to the program is an XML file.  An example xml file can be found in <solution_dir>\Examples.  
	Also, the Examples directory contains an example .bat file and an example .csv file.  The schema is 
	located in the <solution_dir>\Schema. 

	Pre-Conditions:

		1.	"Creo Parametric 1.0" must be installed and functioning properly on your machine.  The license for 
			"Creo Parametric 1.0" must include the Toolkit license.

		2.	The environment variables must be set per 
			...\trunk\deploy\CAD_Installs\Proe ISIS Extensions\0ReadMe.txt

		3.	The .bat file used to invoke CADCreoParametricComputeMetrics.exe must be of the form 
			of <solution_dir>\Examples\ComputeMetricsExample.bat

		4.	The input xml must be compliant with 
			<solution_dir>\Schema\AssemblyInterface.xsd


	Post-Conditions:

	1.	If an exception occurs, the program will log the error message, display the error message to the console, 
		and exit.  Some reasons that exceptions could occur follow:

-              Could not locate a part/assembly.

-              Could not locate a parameter in the part/assembly.

	2.	If a warning occurs, the program will log the warning message, display the warning message to the console, 
		and continue processing.  An example of a warning would be if the part/assembly would not regenerate.  

	3.	If no exceptions occur, the output .csv file(s) would be populated and saved.

*/

#include <stdlib.h>
#include <isis_application_exception.h>
#include <ComputeMetricsXML.hxx>
#include <ISISConstants.h>
#include <CommonUtilities.h>
#include <DiagnosticUtilities.h>
#include <SetCADModelParamters.h>
#include <string>
#include <iostream>
#include <fstream>

int main( int argc, char *argv[] )
{
    
	if ( argc != 6 && argc != 7 ) 
    {
	  printf("Usage : \n%EXE%     %PROE_START_CMD%    \"%PROE_ISIS_EXTENSIONS%\"     %WORKING_DIR%      %COMPUTE_METRICS_XML_FILE%     %LOG_FILE%   [Optional]%EXIT_PROMPT%\n");
	  printf("\n See \"C:\\Program Files\\Proe ISIS Extensions\\docs\\examples\\compute_metrics_ptc.bat\" for an example of how to invoke this executable."); 
	  printf("\nType Enter to exit.");
	  getc(stdin);
      exit(1);
	}

	int ExitCode = 0;

	//////////////////
	// Open Log file
	//////////////////
    std::ofstream LogFile;
	LogFile.open (argv[5], std::ios::out | std::ios::trunc );
	if (LogFile.is_open())
		std::clog.rdbuf(LogFile.rdbuf());
	else
		std::cout << std::endl << "Could not open log file: " << argv[5];


	std::clog << "compute_metrics_ptc " << isis::COMPUTE_METRICS_PTC_VERSION << "      ";
	///////////////////
	// Time Stamp
	///////////////////
	
	time_t ltime; /* calendar time */
    ltime=time(NULL); /* get current cal time */
	std::clog << asctime( localtime(&ltime));


	// cout << endl << "arg 0, EXE:                      "  << argv[0];  // EXE					e.g. C:\Program Files\Proe ISIS Extensions\bin\compute_metrics_ptc.exe
	// cout << endl << "arg 1, PROE_START_CMD:			 "  << argv[1];  // PROE_START_CMD		e.g  c:\Progra~1\proeWi~1.0\/bin/proe.exe -g:no_graphics -i:rpc_input
	// cout << endl << "arg 2, PROE_ISIS_EXTENSIONS:	 "  << argv[2];	// PROE_ISIS_EXTENSIONS	e.g  C:\Program Files\Proe ISIS Extensions
	// cout << endl << "arg 3, WORKING_DIR:              "  << argv[3];	// WORKING_DIR			e.g  .
	// cout << endl << "arg 4, COMPUTE_METRICS_XML_FILE: "  << argv[4];  // COMPUTE_METRICS_XML_FILE	e.g. bracket_plate.xml
	// cout << endl << "arg 5, LOG_FILE:				 "  << argv[5];  // LOG_FILE			e.g. bracket_plate.xml.log
	
	std::clog << std::endl << "arg 0, EXE:                      "  << argv[0];
	std::clog << std::endl << "arg 1, PROE_START_CMD:           "  << argv[1];
	std::clog << std::endl << "arg 2, PROE_ISIS_EXTENSIONS:     "  << argv[2];
	std::clog << std::endl << "arg 3, WORKING_DIR:              "  << argv[3];
	std::clog << std::endl << "arg 4, COMPUTE_METRICS_XML_FILE: "  << argv[4];
	std::clog << std::endl << "arg 5, LOG_FILE:                 "  << argv[5];

	bool PromptBeforeExiting = true;
	if ( argc == 7 ) 
	{
		std::clog << std::endl << "arg 6, PROMPT (before exiting):  "  << argv[6];
		if ( strcmp("NO",  _strupr(argv[6]))  == 0  ) PromptBeforeExiting = false;
	}
	else
	{
		std::clog << std::endl << "arg 6 not passed, assume prompt before exiting";
	}
	
	const std::string PROE_ISIS_EXTENSIONS_DIR = argv[2];
	const std::string WORKING_DIR = argv[3];
	const std::string ComputeMetricsDefinitionXML_PathAndFileName = argv[4];



	bool Pro_E_Running = false;
  	try
	{
		if (!isis::FileExists( argv[4] ))
		{
			std::string TempError = "Could not find the compute metrics definition xml file, file name: " + ComputeMetricsDefinitionXML_PathAndFileName;
			throw isis::application_exception(TempError.c_str());
		} 

		std::auto_ptr<CADComponentsType> CADComponents_ptr( CADComponents(ComputeMetricsDefinitionXML_PathAndFileName.c_str(), 
												  xml_schema::flags::dont_validate));
			
		std::clog << std::endl << "************ Begin Entire Tree ***************";
		isis::stream_CADComponentsTree(CADComponents_ptr, std::clog);
		std::clog << std::endl << "************ End Entire Tree *****************";

		isis::stream_CADComponentsTree(CADComponents_ptr, std::cout);

		/////// Start Creo /////////
		std::cout << std::endl << std::endl << "CADProEComputeMetrics "<< isis::COMPUTE_METRICS_PTC_VERSION;
		std::cout << std::endl << std::endl << "Starting Creo, this takes about 10 seconds...";
		isis::isis_ProEngineerStart(argv[1],"");
		Pro_E_Running = true;
		std::cout <<  std::endl << "Creo successfully started." << std::endl;

		isis::SetCADModelParamters(CADComponents_ptr);


		std::cout << std::endl << std::endl << "Compute metrics completed successfully." << std::endl << std::endl;

	}

	catch (const xml_schema::exception& e)
	{
		std::cout << std::endl << "Schema exception: " << e.what() << std::endl << std::endl;
		if (LogFile.is_open()) std::clog << std::endl << "Schema exception: " << e.what();
	}
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
		ExitCode = 1;
	}
	catch ( ... )
	{
		std::cout << std::endl << "Caught exception (...)"  << std::endl << std::endl;
		ExitCode = 1;
	}


	/////// Stop Creo /////////
	if (Pro_E_Running) ProEngineerEnd();
	if (LogFile.is_open())LogFile.close();

	if ( PromptBeforeExiting )
	{
		printf("\nType Enter to exit.");
		getc(stdin);
	}

	exit(ExitCode);

} // end main