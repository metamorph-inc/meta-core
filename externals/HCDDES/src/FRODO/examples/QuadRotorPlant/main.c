/*** Included Header Files ***/
#include "frodo.h"
#include "QuadRotorPlant.h"					// Model's header file
#include "QuadRotorPlant_private.h"			// Many internal functions
#include "rtwtypes.h"						// MathWorks types


/*****************************************************************************/


/*** Message Structures ***/
// Global Context
typedef struct {
	// Schedulable instances
	SchedSchedulable*			QuadRotorPlant_task;
	// Channel instances
	UDPChannel*					udpChannel;
	// Message Ports
	PortId_t					inputPort;
	PortId_t					outputPort;
} Context_t;


// Model variables
static RT_MODEL_QuadRotorPlant QuadRotorPlant_M_;
static RT_MODEL_QuadRotorPlant *const QuadRotorPlant_M = &QuadRotorPlant_M_;	/* Real-time model */
static BlockIO_QuadRotorPlant QuadRotorPlant_B;									/* Observable signals */
static D_Work_QuadRotorPlant QuadRotorPlant_DWork;								/* Observable states */
static ContinuousStates_QuadRotorPlant QuadRotorPlant_X;						/* Observable continuous states */
static ExternalInputs_QuadRotorPlant QuadRotorPlant_U;							/* External inputs */
static ExternalOutputs_QuadRotorPlant QuadRotorPlant_Y;							/* External outputs */


/*****************************************************************************/


void rt_OneStep( RT_MODEL_QuadRotorPlant *const QuadRotorPlant_M ) {
	// Static variables (for some reason)
	static boolean_T OverrunFlags[5] = { 0, 0, 0, 0, 0 };
	static boolean_T eventFlags[5] = { 0, 0, 0, 0, 0 };		/* Model has 5 rates */
	static int_T taskCounter[5] = { 0, 0, 0, 0, 0 };
	int_T i;
	// Check base rate for overrun
	if ( OverrunFlags[0] ) {
		rtmSetErrorStatus( QuadRotorPlant_M, "Overrun" );
		assert( false );
	}
	OverrunFlags[0] = TRUE;
	/* For a bare-board target (i.e., no operating system), the
	 * following code checks whether any subrate overruns,
	 * and also sets the rates that need to run this time step. */
	for ( i = 2; i < 5; i++ ) {
		if ( taskCounter[i] == 0 ) {
			if ( eventFlags[i] ) {
				OverrunFlags[0] = FALSE;
				OverrunFlags[i] = TRUE;
				/* Sampling too fast */
				rtmSetErrorStatus( QuadRotorPlant_M, "Overrun" );
				assert( false );
			}
			eventFlags[i] = TRUE;
		}
	}
	taskCounter[1]++;
	if ( taskCounter[1] == 1 ) {
		taskCounter[1]= 0;
	}
	taskCounter[2]++;
	if ( taskCounter[2] == 2 ) {
		taskCounter[2]= 0;
	}
	taskCounter[3]++;
	if ( taskCounter[3] == 10 ) {
		taskCounter[3]= 0;
	}
	taskCounter[4]++;
	if ( taskCounter[4] == 20 ) {
		taskCounter[4]= 0;
	}
	/* Set model inputs associated with base rate here */
	/* Step the model for base rate */
	QuadRotorPlant_step0( QuadRotorPlant_M, &QuadRotorPlant_B,
						 &QuadRotorPlant_DWork, &QuadRotorPlant_X,
						 &QuadRotorPlant_U, &QuadRotorPlant_Y );
	/* Get model outputs here */
	/* Indicate task for base rate complete */
	OverrunFlags[0] = FALSE;
	/* Step the model for any subrate */
	for ( i = 2; i < 5; i++ ) {
		/* If task "i" is running, don't run any lower priority task */
		if ( OverrunFlags[i] ) {
			assert( false );
		}
		if ( eventFlags[i] ) {
			OverrunFlags[i] = TRUE;
			/* Set model inputs associated with subrates here */
			/* Step the model for subrate "i" */
			switch ( i ) {
				case 2:
					QuadRotorPlant_step2( QuadRotorPlant_M, &QuadRotorPlant_B,
										 &QuadRotorPlant_DWork, &QuadRotorPlant_U,
										 &QuadRotorPlant_Y );
					/* Get model outputs here */
					break;
				case 3:
					QuadRotorPlant_step3( &QuadRotorPlant_B, &QuadRotorPlant_DWork );
					/* Get model outputs here */
					break;
				case 4:

					QuadRotorPlant_step4( &QuadRotorPlant_B, &QuadRotorPlant_DWork );
					/* Get model outputs here */
					break;
				default:
					break;
			}
			/* Indicate task complete for sample time "i" */
			OverrunFlags[i] = FALSE;
			eventFlags[i] = FALSE;
		}
	}
		// Check for rtm initiated halting conditions
	if ( ( rtmGetErrorStatus( QuadRotorPlant_M ) != NULL ) || rtmGetStopRequested( QuadRotorPlant_M ) ) {
		assert( false );
	}
}


/*************************************************************************
****/


void* QuadRotorPlant_exec( void *cntx ) {
	// Cast to a context structure
	Context_t* context = (Context_t*)cntx;
	ReturnCode_t returnCode;

	while ( true ) {
		// Wait to execute
		SchedSignalExecution( context->QuadRotorPlant_task );
		// Read the message port
		ReadSamplingMessage( context->inputPort, &(QuadRotorPlant_U), NULL, NULL, &returnCode );

		/*****************************************************************************/

		// Step the solver
		rt_OneStep( QuadRotorPlant_M );

		/*****************************************************************************/

		// Write the new value back to the port (pass data from RTM struct into port struct)
		WriteSamplingMessage( context->outputPort, &(QuadRotorPlant_Y), sizeof(ExternalOutputs_QuadRotorPlant), &returnCode );
		// Request that the port's data be sent out via the UDP channel
		UDPAsyncSend( context->udpChannel, 1, 5.0 );
		// Signal that we are done with this execution
		SchedSignalCompletion( context->QuadRotorPlant_task );
	}
	// Must return something - never will get here
	return NULL;
}


/*****************************************************************************/


/** Primary entry function ***/
int_T main(int_T argc,  char *argv[]) {
	ReturnCode_t returnCode;
	// Create the master context
	Context_t context;
	// Zero the context
	memset( &context, 0, sizeof(Context_t) );
	// Initialize the FRODO subsystems
	SysEventInitialize( AllCategories, true, true, false );
	PeripheralInitialize();
	LogInitialize();
	ErrorHandlerInitialize();
	SchedInitialize( EarliestDeadlineFirst, ErrSchedulerHandler, 20.0, 1, &context );
	UDPInitialize( ErrUDPHandler );

	// Initialize model
	QuadRotorPlant_initialize(QuadRotorPlant_M, &QuadRotorPlant_B,
							  &QuadRotorPlant_DWork, &QuadRotorPlant_X, &QuadRotorPlant_U,
							  &QuadRotorPlant_Y);

	/*** Create message ports ***/
	{
		// Create input and output ports using RTW defined structures
		CreateSamplingPort( "ExternalInputs", sizeof(ExternalInputs_QuadRotorPlant), Bidirectional, 0, &(context.inputPort), &returnCode );
		CreateSamplingPort( "ExternalOutputs", sizeof(ExternalOutputs_QuadRotorPlant), Bidirectional, 0, &(context.outputPort), &returnCode );
	}

	/*** Create Tasks ***/
	{
		// Create task with instances
		pfloat_t QuadRotorPlant_times[1] = { 5.0 };
		SchedCreatePeriodicTask( "QuadRotorPlant", QuadRotorPlant_exec, &context.QuadRotorPlant_task, 10.00, NoRestriction, 1, QuadRotorPlant_times );
	}

	/*** Create Peripherals ***/
	{
		AsyncExpectation asyncExpects[2] = {
			{ ExpectSend, 1, sizeof(ExternalOutputs_QuadRotorPlant), 2.0, 0.0, 20.0, context.outputPort, NULL },
			{ ExpectReceive, 2, sizeof(ExternalInputs_QuadRotorPlant), 2.0, 0.0, 20.0, context.inputPort, NULL }
		};
		// Create the channel.  Make sure channel is not set as sync channel (no need to coordinate startup and shutdown)
		context.udpChannel = UDPCreateChannel( argv[1], argv[2], 21212, 0, NULL, 2, asyncExpects, false, true, 1 );
	}

	/*** Execute the schedule ***/

	// Wait for the platform to wake up ( Platform Dependent )
	NanoSleep( SCHEDULER_INITIALIZATION_WAIT );
	// Perform first step
	rt_OneStep( QuadRotorPlant_M );
	// Execute the schedule
	SchedExecute( 5 );

	/*** Shutdown the application ***/

	// Terminate model
	QuadRotorPlant_terminate();
	// If this returns we must shut everything down
	UDPShutdown( );
	SchedShutdown( );
	// Shutdown the logger
	SysEventShutdown( NULL );
	return 0;
}

