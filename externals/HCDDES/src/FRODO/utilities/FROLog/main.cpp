/*** Included Header Files ***/
#include "FROLog.h"
#include "error_handler.h"
#include "scheduler.h"
#include "arch/highres_timing.h"
#include "logger.h"


/*****************************************************************************/


FS_Schedulable					*_nextTask = NULL;
void (*_SchedErrorHandler)(FE_ErrorCode,void*,pfloat_t) = NULL;
bool							_schedExec;
FROLogger						*_logger = NULL;

/*****************************************************************************/



void _ErrorHandler( FE_ErrorCode errorCode , void* object ,pfloat_t atTimeMS ) {
	std::cout << "Somehow we are in ErrorHandler!!!\n";
}


void LogEvent( FL_EventCategory		category, uint32_t eventType, void* object ) {
}



/* Handle signals gracefully */
void HandleSignals( int signal ) {
	// Switch on type of signal
	switch ( signal ) {
		case SIGINT:
			// All we should have to do is set _schedExec to false
			_logger->HaltExecution();
			break;
	}
}


/* */
int main(int argc, const char *argv[]) {
	// Set the error handler
	_SchedErrorHandler = _ErrorHandler;
	// Get the config file
	std::string configFile;
	if ( argc == 2 ) configFile = argv[1];
	// Make sure there are two command-line arguments
	if ( argc != 2 ) {
		printf("Usage: FROLog <config.xml>\n" );
		configFile = "config.xml";
	}
	// Create the logger and get to running
	_logger = new FROLogger( configFile );
	// Install the signal handler
	signal( SIGINT, HandleSignals );
	// Run the logger
	_logger->Execute();
	// When this returns, everything should be shutdown
}


/*****************************************************************************/

