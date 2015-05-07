/*** Included Header Files ***/
#include "motivate.h"
#include "templateAction.h"
#include "motivate_process.h"
// Windows Includes
#include <sys/stat.h>
#include <windows.h> 
#include <tchar.h>
#include <stdio.h> 
#include <strsafe.h>


/*** Locally Defined Value ***/
#define BUFSIZE 4096 


/*** Initialize Static Variables ***/
bool Conductor::_init = false;
PyObject* Conductor::_mainMod = NULL;
PyObject* Conductor::_mainNamespace = NULL;
Process*  Conductor::_currentProcess = NULL;


/************************************************************************************/


void ConsoleWrite( std::string outputStr ) {
	// Write the string to the GME console
	Console::Out::WriteLine( outputStr.c_str() );
}


void ErrOutput( std::string output ) {
	static std::string final;
	// Append the new string to the end of the existing one
	final.append( output );
	// See if there is a \n in there
	std::string::size_type loc = final.find( "\n", 0 );
	if( loc != std::string::npos ) {
		// Get everything up to and including the \n
		std::string consoleOut = final.substr( 0, loc+1 );
		// Check to see if second to last character is ^
		std::string::size_type loc2 = consoleOut.find( "^", 0 );
		if ( loc2 != std::string::npos ) {
			// Replace all spaces with .'s
			consoleOut.replace( 0, loc2, "--------------------------------------------------------------------------------------------------", 0, loc2 );
		}
		// Write out to the console
		Console::Out::WriteLine( consoleOut.c_str() );
		// Truncate final down to everything after the \n
		final = final.substr( loc+1 );
	}
}


void SetActionStatus( bool status ) {
	// Get and check the process
	Process *process = Conductor::CurrentProcess();
	if ( process ) {
		// Get and check the action
		Action *action = process->CurrentAction();
		if ( action ) {
			// Only need to set true or false
			action->SetStatus( status );
		}
	}
}


void SetActionResults( std::string results ) {
	// Get and check the process
	Process *process = Conductor::CurrentProcess();
	if ( process ) {
		// Get and check the action
		Action *action = process->CurrentAction();
		if ( action ) {
			// Only need to set the string value
			action->SetResults( results );
		}
	}
}


void ActionPOpen( std::string commandLine, unsigned long exitCheck ) {
	// Get and check the process
	Process *process = Conductor::CurrentProcess();
	if ( !process ) { return; }
	// Get and check the action
	Action *action = process->CurrentAction();
	if ( !action ) { return; }

	// Setup the pipes for capturing all stdin/stdout/stderr
	HANDLE pipeStdInRd = NULL;
	HANDLE pipeStdInWr = NULL;
	HANDLE pipeStdOutRd = NULL;
	HANDLE pipeStdOutWr = NULL;
	SECURITY_ATTRIBUTES saAttr; 
	// Set the bInheritHandle flag so pipe handles are inherited. 
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
	saAttr.bInheritHandle = TRUE; 
	saAttr.lpSecurityDescriptor = NULL; 
	// Create a pipe for the child process's STDOUT. 
	if ( !CreatePipe( &pipeStdOutRd, &pipeStdOutWr, &saAttr, 0 ) ) { return; }
	// Ensure the read handle to the pipe for STDOUT is not inherited.
	if ( !SetHandleInformation( pipeStdOutRd, HANDLE_FLAG_INHERIT, 0 ) ) { return; }
	// Create a pipe for the child process's STDIN. 
	if (! CreatePipe( &pipeStdInRd, &pipeStdInWr, &saAttr, 0 )) { return; }
	// Ensure the write handle to the pipe for STDIN is not inherited.
	if ( !SetHandleInformation( pipeStdInWr, HANDLE_FLAG_INHERIT, 0 ) ) { return; }

	// Create the child process.
	char* cmdLine = (char*)commandLine.c_str();
	PROCESS_INFORMATION procInfo;
	STARTUPINFO startInfo;
	BOOL success = FALSE; 
	// Set up members of the PROCESS_INFORMATION structure. 
	ZeroMemory( &procInfo, sizeof(PROCESS_INFORMATION) );
	// Set up members of the STARTUPINFO structure. 
	// This structure specifies the STDIN and STDOUT handles for redirection.
	ZeroMemory( &startInfo, sizeof(STARTUPINFO) );
	startInfo.cb = sizeof(STARTUPINFO);
	startInfo.hStdError = pipeStdOutWr;
	startInfo.hStdOutput = pipeStdOutWr;
	startInfo.hStdInput = pipeStdInRd;
	startInfo.dwFlags |= STARTF_USESTDHANDLES;
	// Create the child process.
	success = CreateProcess( NULL, 
		cmdLine,		// command line 
		NULL,			// process security attributes 
		NULL,			// primary thread security attributes 
		TRUE,			// handles are inherited 
		0,				// creation flags 
		NULL,			// use parent's environment 
		NULL,			// use parent's current directory 
		&startInfo,		// STARTUPINFO pointer 
		&procInfo);		// receives PROCESS_INFORMATION 
   
	// If an error occurs, exit the function. 
	if ( ! success ) {
		// Try to get a formatted error message
		LPSTR lpMsgBuf;
		FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR) &lpMsgBuf,
			0,
			NULL);
		// Display the string.
		action->SetResults( lpMsgBuf );
		action->SetStatus( false );
		// Free the buffer.
		LocalFree( lpMsgBuf );
		return;
	}
	// Close the write end of the pipe before reading
	if ( !CloseHandle( pipeStdOutWr ) ) { Console::Out::WriteLine( "Error trying to close write end of pipe from child app." ); } 
	// Read any stdout or stderr generated from the child process
	DWORD numRead; 
	CHAR charBuffer[BUFSIZE];
	std::string output;
	// Continue to try reading until nothing more is read
	for( ; ; ) {
		// Read from the pipe
		success = ReadFile( pipeStdOutRd, charBuffer, BUFSIZE, &numRead, NULL );
		// See if anything was read
		if( !success || numRead == 0 ) break;
		// If we are here, then copy the output into the string
		output.append( charBuffer, numRead );
	} 
	// Wait for the child process to terminate
	WaitForSingleObject( procInfo.hProcess, INFINITE );
	// Get the exit code from the application
	DWORD exitCode;
	GetExitCodeProcess( procInfo.hProcess, &exitCode );
	// Close the handles to the child process
	CloseHandle( procInfo.hProcess );
	CloseHandle( procInfo.hThread );
	// Clean up all of the pipes
	// ...
	// Set the results for the action
	action->SetResults( output );
	// Set the status for the action
	if (exitCheck != -999)
		action->SetStatus( exitCode == exitCheck );
	else
		action->SetStatus( true );
   return; 
} 


void ActionPOpenDefault( std::string commandLine ) {
	// Just call above
	ActionPOpen( commandLine, -999 );
}



BOOST_PYTHON_MODULE(Motivate)
{
    def( "Console", ConsoleWrite, "Write output to the GME console." );
	def( "HiddenErrOut", ErrOutput );
	def( "Status", SetActionStatus, "Set the status of the Action." );
	def( "Results", SetActionResults, "Set the results text of the Action." );
	def( "Popen", ActionPOpen, "Similar to subprocess.Popen with Actions.  Pass a commandline and an optional exit code value." );
	def( "Popen", ActionPOpenDefault, "Similar to subprocess.Popen with Actions.  Pass a commandline and an optional exit code value." );
}


/************************************************************************************/


std::map< std::string,BON::Model > Motivate::MapProcesses( const BON::Project &project ) {
	// Build list of processes
	BON::Folder rootFolder = project->getRootFolder();
	std::map< std::string,BON::Model > procMap;
	std::set< BON::Folder > folderSet = rootFolder->getChildFolders();
	std::set< BON::Folder >::iterator folderSetIter = folderSet.begin();
	// Loop through all of the Folders
	for(; folderSetIter != folderSet.end(); folderSetIter++) {
		BON::Folder folder = (*folderSetIter);
		std::set<BON::Model> modelSet = folder->getChildModels();
		// Go through all of the children models (Process)
		for (std::set<BON::Model>::iterator modelSetIter = modelSet.begin(); modelSetIter != modelSet.end(); modelSetIter++) {
			// Make sure model is a process
			std::string modelType = (*modelSetIter)->getModelMeta().name();
			if ( modelType.compare("Process") == 0 ) {
				// Get the name of the Process
				std::string processName = (*modelSetIter)->getName();
				// Add it to the process map
				procMap.insert( std::make_pair( processName, (*modelSetIter) ) );
			}
		}
	}
	//Now go through all root level processes
	std::set<BON::Model> modelSet = rootFolder->getChildModels();
	// Go through all of the children models (Process)
	for (std::set<BON::Model>::iterator modelSetIter = modelSet.begin(); modelSetIter != modelSet.end(); modelSetIter++) {
		// Make sure model is a process
		std::string modelType = (*modelSetIter)->getModelMeta().name();
		if ( modelType.compare("Process") == 0 ) {
			// Get the name of the Process
			std::string processName = (*modelSetIter)->getName();
			// Add it to the process map
			procMap.insert( std::make_pair( processName, (*modelSetIter) ) );
		}
	}
	// Return the process map
	return procMap;
}


std::map< std::string,BON::Model > Motivate::MapActions( const BON::Model &process ) {
	std::map< std::string,BON::Model > actionMap;
	std::set< BON::Model > actionSet = process->getChildModels();
	// Loop through all children
	std::set< BON::Model >::iterator actionIter = actionSet.begin();
	for(; actionIter != actionSet.end(); actionIter++) {
		// Make sure model is a CommandLineAction or a TemplateAction
		std::string modelType = (*actionIter)->getModelMeta().name();
		if ( ( modelType.compare( "CommandLineAction" ) == 0 ) || 
			 ( modelType.compare( "TemplateAction" ) == 0 ) ||
			 ( modelType.compare( "CompoundAction" ) == 0 ) )
		{
			// Add action to map
			actionMap.insert( std::make_pair( (*actionIter)->getName(), (*actionIter) ) );
		}
	}
	return actionMap;
}


std::vector< std::string > Motivate::OrderProcesses( const std::map< std::string,BON::Model > &procMap ) {
	std::map< long,std::string > orderMap;
	// Copy the BON::Models into a map
	for (std::map< std::string,BON::Model >::const_iterator mapIter = procMap.begin(); mapIter != procMap.end(); mapIter++) {
		BON::Model process = (*mapIter).second;
		// Get the order value for the process
		long order = process->getAttribute("Ordering")->getValue();
		// Allow a -1 to make the process invisible to the interpreter
		if (order != -1)
			orderMap.insert( std::make_pair( order, (*mapIter).first ) );
	}
	
	// Copy the sorted values into a string vector for output
	std::vector< std::string > processVector;
	for (std::map< long,std::string >::iterator mapIter = orderMap.begin(); mapIter != orderMap.end(); mapIter++) {
		processVector.push_back( (*mapIter).second );
	}
	// Return the good list
	return processVector;
}


int _ActionDepth( const BON::Model &action ) {
	// Get list of connections
	std::multiset< BON::ConnectionEnd > connectionSet = action->getConnEnds( "ActionSequence", "src", false );
	// Should only be only zero or one
	if (connectionSet.size() == 0) return 0;
	else {
		BON::Model parentAction( *(connectionSet.begin()) );
		// Recursively call and add one for each depth of child
		return _ActionDepth( parentAction ) + 1;
	}
}


std::vector< std::string > Motivate::OrderActions( const std::map< std::string,BON::Model > &actionMap ) {
	std::multimap< int,std::string > actionOrder;
	std::map< std::string,BON::Model >::const_iterator actionMapIter = actionMap.begin();
	// Insert all actions into a map using their depth as the key (this will order them)
	for (; actionMapIter != actionMap.end(); actionMapIter++) {
		actionOrder.insert( std::make_pair( _ActionDepth( (*actionMapIter).second ),(*actionMapIter).first ) );
	}
	// Iterate over the map to get an ordered vector
	std::vector< std::string > actionVector;
	std::multimap< int,std::string >::iterator orderIter = actionOrder.begin();
	for(; orderIter != actionOrder.end(); orderIter++) {
		actionVector.push_back( (*orderIter).second );
	}
	// Return the ordered list of actions
	return actionVector;
}


std::string Motivate::RootDirectory( const BON::Model &action ) {
	// Get the parent process
	BON::Model process = action->getParent();
	return process->getAttribute( "RootDirectory" )->getValue();
}


void Motivate::RecurseActions( const BON::Model &action, std::vector< BON::Model > &actionVec ) {
	// Add self to set
	actionVec.push_back( action );
	// Get list of connections
	std::multiset< BON::ConnectionEnd > connectionSet = action->getConnEnds( "ActionSequence", "dst", false );
	std::multiset< BON::ConnectionEnd >::iterator connSetIter = connectionSet.begin();
	// Visit each child action
	for (; connSetIter != connectionSet.end(); connSetIter++) {
		// Cast the connection end into a model
		BON::Model model( (*connSetIter) );
		// Recursively call on this action
		Motivate::RecurseActions( model, actionVec );
	}
}


/************************************************************************************/


std::map< std::string,std::string > Conductor::BuildParamMap( void ) {
	std::map< std::string,std::string > paramMap;
	// Nothing for now
	return paramMap;
}


bool Conductor::Initialize( void ) {
	// Make sure not already initialized
	if (!Conductor::_init) {
		Conductor::_init = true;

		// Initialize the python environment
		Py_Initialize();
		// Start things up, and import any packages
		try {
			// Get the __main__ module and its dictionary
			Conductor::_mainMod = PyImport_AddModule( "__main__" );
			Py_INCREF( Conductor::_mainMod );
			Conductor::_mainNamespace = PyModule_GetDict( Conductor::_mainMod );
			Py_INCREF( Conductor::_mainNamespace );

			// Import the os and sys modules
			PyObject* osModule = PyImport_ImportModuleEx( "os", Conductor::_mainNamespace, Conductor::_mainNamespace, NULL );
			PyModule_AddObject( Conductor::_mainMod, "os", osModule );
			PyObject* sysModule = PyImport_ImportModuleEx( "sys", Conductor::_mainNamespace, Conductor::_mainNamespace, NULL );
			PyModule_AddObject( Conductor::_mainMod, "sys", sysModule );
			PyObject* subprocessModule = PyImport_ImportModuleEx( "subprocess", Conductor::_mainNamespace, Conductor::_mainNamespace, NULL );
			PyModule_AddObject( Conductor::_mainMod, "subprocess", subprocessModule );

			// Import the Motivate module
			initMotivate();
			PyObject* motivateModule = PyImport_ImportModuleEx( "Motivate", Conductor::_mainNamespace, Conductor::_mainNamespace, NULL );
			PyModule_AddObject( Conductor::_mainMod, "Motivate", motivateModule );

			// Set up StdOut and StdErr to print to the console
			 PyRun_SimpleString("class StdoutCatcher:\n"
									"\tdef write(self, str):\n"
										"\t\tMotivate.Console(str)\n"
								"class StderrCatcher:\n"
									"\tdef write(self, str):\n"
										"\t\tMotivate.HiddenErrOut(str)\n"
								"sys.stdout = StdoutCatcher()\n"
								"sys.stderr = StderrCatcher()\n" );
			 // Now move on to better stuff...

		} catch (error_already_set) {
			// PyErr_Print();
			Console::Out::WriteLine( "*** Motivate::Initialize -- Exception in Python Importing ***" );
		}
	}
	// Otherwise, just do nothing...
	return Conductor::_init;
}


bool Conductor::Shutdown(void) {
	// Check if initialized
	if (Conductor::_init) {
		Conductor::_init = false;
		// Get rid of references to __main__ module and its dictionary
		Py_DECREF(Conductor::_mainNamespace);
		Py_DECREF(Conductor::_mainMod);
		// Cleanup the python environment
		Py_Finalize();
	}
	// All is good
	return Conductor::_init;
}


/************************************************************************************/


bool Conductor::Prepare( const BON::Model &process, const std::vector< BON::Model > &actions ) {
	// Initialize the environment (pass the root directory )
	Conductor::Initialize( );
	// Create a Motivate::Process to handle everything
	Process* newProcess = new Process( process, actions );
	Conductor::_currentProcess = newProcess;
	// Return the status
	return ( Conductor::_currentProcess != NULL );
}



Action* Conductor::ExecuteStep(void) {
	// Make sure we are initialized
	Conductor::Initialize();
	// Make sure we have a process
	if ( !Conductor::_currentProcess ) return NULL;
	// Build the paramMap
	std::map< std::string,std::string > paramMap = Conductor::BuildParamMap( );
	// Execute a step
	return Conductor::_currentProcess->ExecuteStep( paramMap );
}


void Conductor::Reset(void) {
	// Make sure we are initialized
	Conductor::Initialize();
	// If there is a process...
	if ( Conductor::_currentProcess ) {
		// Delete the process
		delete Conductor::_currentProcess;
		// Mark the current as NULL
		Conductor::_currentProcess = NULL;
	}
}


/************************************************************************************/

