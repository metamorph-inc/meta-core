/***
 * Generated code: DO NOT EDIT MANUALLY
 * For use in generating Matlab/Simulink + TrueTime
 * models from ESMoL models.  Please consult ESMoL
 * documentation for additional information.
***/


/*** Gloablly Included Header Files ***/
#define S_FUNCTION_NAME GS_init
#define mexPutArray
#include "ttkernel.cpp"
#include <map>
#include "OuterLoop_sl.c"
#include "RefHandler_sl.c"



/*** Message Structures ***/

// Message struct for message type OuterLoop_ang_ref
typedef struct {
	A1A5singleAA OuterLoop_ang_ref;
	
} OuterLoop_ang_ref_t;

// Message struct for message type DataHandling_pos_msg
typedef struct {
	single OuterLoop_pos;
	
} DataHandling_pos_msg_t;

// Message struct for message type RefHandling_pos_ref_out
typedef struct {
	single RefHandling_pos_ref_out;
	
} RefHandling_pos_ref_out_t;

// Message struct for message type RefHandling_pos_ref_in
typedef struct {
	single RefHandling_pos_ref_in;
	
} RefHandling_pos_ref_in_t;



/*** Setup Kernel Data Structure ***/
// Kernel data strucutre
struct KernelData {
	OuterLoop_context* OuterLoop_ctx;
	RefHandler_context* RefHandling_ctx;
	
	OuterLoop_ang_ref_t*		OuterLoop_ang_ref;
	DataHandling_pos_msg_t*		DataHandling_pos_msg;
	RefHandling_pos_ref_out_t*		RefHandling_pos_ref_out;
	RefHandling_pos_ref_in_t*		RefHandling_pos_ref_in;
	
	// Other kernel data needed by the scheduler
	unsigned int								hyperperiod;
	double										hyperperiodStart;
	std::map< double, std::string >				tasks;
	std::map< double, std::string >::iterator	currentTask;
};


/************************************************************/

//Code to handle the execution of OuterLoop_task
double OuterLoop_code( int seg, void *data ) {
	// Get the kernel data
	KernelData *kernelData = (KernelData*)ttGetUserData();
	// Go through the two possible phases
	switch ( seg ) {
		// Execute component code
		case 1:
			// Execute the task component
			OuterLoop_main( 
				kernelData->OuterLoop_ctx,
				kernelData->RefHandling_pos_ref_out->RefHandling_pos_ref_out,
				kernelData->DataHandling_pos_msg->OuterLoop_pos,
				&kernelData->OuterLoop_ang_ref->OuterLoop_ang_ref
				);
			// Return the WCET for the task
			return 0.000245;
		
		// We are done phase
		default:
			return FINISHED;
	}
}
/************************************************************/

//Code to handle the execution of RefHandling_task
double RefHandling_code( int seg, void *data ) {
	// Get the kernel data
	KernelData *kernelData = (KernelData*)ttGetUserData();
	// Go through the two possible phases
	switch ( seg ) {
		// Execute component code
		case 1:
            // Get input for the reference signal
			kernelData->RefHandling_pos_ref_in->RefHandling_pos_ref_in = (float)ttAnalogIn( 1 );
			// Execute the task component
			RefHandler_main( 
				kernelData->RefHandling_ctx,
				kernelData->RefHandling_pos_ref_in->RefHandling_pos_ref_in,
				&kernelData->RefHandling_pos_ref_out->RefHandling_pos_ref_out
				);
			// Return the WCET for the task
			return 0.000001;
		
		// We are done phase
		default:
			return FINISHED;
	}
}




/************************************************************/

//Code to handle the communication of OuterLoop_ang_ref
double OuterLoop_ang_ref_code( int seg, void *data ) {
	// Get the kernel data
	KernelData *kernelData = (KernelData*)ttGetUserData();
	void* localBuffer;
	// Go through the two possible phases
	switch ( seg ) {
		// Execute component code
		case 1:
			// Send the message onto the network
			localBuffer = (void*)kernelData->OuterLoop_ang_ref;
			ttSendMsg( 1, 1, localBuffer, sizeof( OuterLoop_ang_ref_t ) );
			return 0.001000;
			
			

		// We are done phase
		default:
			return FINISHED;
	}
}
/************************************************************/

//Code to handle the communication of DataHandling_pos_msg
double DataHandling_pos_msg_code( int seg, void *data ) {
	// Get the kernel data
	KernelData *kernelData = (KernelData*)ttGetUserData();
	void* localBuffer;
	// Go through the two possible phases
	switch ( seg ) {
		// Execute component code
		case 1:
			
			// Receive the message from the network
			localBuffer = ttGetMsg( 1 );
			// Check if null local buffer
			if ( localBuffer != NULL ) {
				// Copy data into the actual message buffer
				memcpy( kernelData->DataHandling_pos_msg, localBuffer, sizeof( DataHandling_pos_msg_t ) );
			} else {
				// ttAbortSimulation(); 
				ttSetNextSegment(1);
				return 0.000001;
			}
			return 0.000001;  // ${WCET} = 0.001000;
			

		// We are done phase
		default:
			return FINISHED;
	}
}


/************************************************************/


void add_tasks( std::string taskName, int count, double* times ) {
	KernelData *kernelData = (KernelData*)ttGetUserData();
	// Loop through the times
	for (int i = 0; i < count; i++ ) {
		// A negative time indicates not to add it
		if ( times[1] > 0.0 ) {
			// Add each time to the master task list with task name
			kernelData->tasks.insert( std::make_pair( times[i], taskName ) );
		}
	}
}


double scheduler_exec( int seg, void *data ) {
	// Get the kernel data structure
	KernelData *kernelData = (KernelData*)ttGetUserData();

	// See if we are in the start of a hyperperiod ...
	if ( seg == 1 ) {
		// Determine start of current hyperperiod
		kernelData->hyperperiodStart = ttCurrentTime();
	}
	// Otherwise we should schedule a task
	else {
		// We are woken up, now schedule the task
		ttCreateJob( kernelData->currentTask->second.c_str() );
		// Move on to the next task
		kernelData->currentTask++;
		// Double check for end of hyperperiod
		if ( kernelData->currentTask == kernelData->tasks.end() ) {
			// Reset the task list pointer
			kernelData->currentTask = kernelData->tasks.begin();
			// Increment the hyperperiod count
			kernelData->hyperperiod++;
			// And we are out of here
			return FINISHED;
		}
	}
	// Determine time of the next task to be executed
	double taskTime = kernelData->currentTask->first + kernelData->hyperperiodStart;
	// Sleep until that time
	ttSleepUntil( taskTime );
	// Each scheduler segment consumes 1us
	return 0.001;
}


/************************************************************/


// Primary entry point for the GS_init Matlab function
void init() {
	// Initialize TrueTime kernel  
	ttInitKernel( prioFP );
  
	// Allocate kernel data and store pointer in UserData
	KernelData* kernelData = new KernelData;
	ttSetUserData( kernelData );
  
	// Read the input argument from the block dialogue
	mxArray *initarg = ttGetInitArg();
	if ( !mxIsDoubleScalar( initarg ) ) {
		TT_MEX_ERROR( "The init argument must be a number!\n" );
		return;
	}

	/*****************************************************/
	
	// Allocate all local message structures into kernel data
	kernelData->OuterLoop_ang_ref = new OuterLoop_ang_ref_t;
	memset( kernelData->OuterLoop_ang_ref, 0, sizeof(OuterLoop_ang_ref_t) );
	kernelData->DataHandling_pos_msg = new DataHandling_pos_msg_t;
	memset( kernelData->DataHandling_pos_msg, 0, sizeof(DataHandling_pos_msg_t) );
	kernelData->RefHandling_pos_ref_out = new RefHandling_pos_ref_out_t;
	memset( kernelData->RefHandling_pos_ref_out, 0, sizeof(RefHandling_pos_ref_out_t) );
	kernelData->RefHandling_pos_ref_in = new RefHandling_pos_ref_in_t;
	memset( kernelData->RefHandling_pos_ref_in, 0, sizeof(RefHandling_pos_ref_in_t) );
	


	/*****************************************************/

	// Create and store pointer in kernel data
	kernelData->OuterLoop_ctx = new OuterLoop_context;
	// Initialize component and its context data
	OuterLoop_init( kernelData->OuterLoop_ctx );
	// Add all instances of the tasks to the global task list
	double OuterLoop_task_data[1] = { 0.013 };
	add_tasks( "OuterLoop_task", 1, OuterLoop_task_data );
	// Create a sporadic controller task
	ttCreateTask( "OuterLoop_task", 0.000245, OuterLoop_code );
	ttSetPriority( 2, "OuterLoop_task" );

	/*****************************************************/

	// Create and store pointer in kernel data
	kernelData->RefHandling_ctx = new RefHandler_context;
	// Initialize component and its context data
	RefHandler_init( kernelData->RefHandling_ctx );
	// Add all instances of the tasks to the global task list
	double RefHandling_task_data[1] = { 0.008 };
	add_tasks( "RefHandling_task", 1, RefHandling_task_data );
	// Create a sporadic controller task
	ttCreateTask( "RefHandling_task", 0.000001, RefHandling_code );
	ttSetPriority( 2, "RefHandling_task" );





	/*****************************************************/

	// Add all instances of the bus message to the global task list
	double OuterLoop_ang_ref_task_data[1] = { 0.007 };
	add_tasks( "OuterLoop_ang_ref_task", 1, OuterLoop_ang_ref_task_data );
	// Create a sporadic bus message task
	ttCreateTask( "OuterLoop_ang_ref_task", 0.001000, OuterLoop_ang_ref_code );
	ttSetPriority( 2, "OuterLoop_ang_ref_task" );

	/*****************************************************/

	// Add all instances of the bus message to the global task list
	double DataHandling_pos_msg_task_data[1] = { 0.011 };
	add_tasks( "DataHandling_pos_msg_task", 1, DataHandling_pos_msg_task_data );
	// Create a sporadic bus message task
	ttCreateTask( "DataHandling_pos_msg_task", 0.001000, DataHandling_pos_msg_code );
	ttSetPriority( 2, "DataHandling_pos_msg_task" );


	/*****************************************************/


	// Setup the master hyperperiod schedule and get ready to run
	kernelData->hyperperiod = 0;
	kernelData->currentTask = kernelData->tasks.begin();
	// Create the primary scheduler task
	ttCreatePeriodicTask( "FRODO_Scheduler", 0.020000, scheduler_exec );
	ttSetPriority( 1, "FRODO_Scheduler" );
}


void cleanup() {
	// Free all dynamic memory allocated during run-time
	KernelData *kernelData = (KernelData*)ttGetUserData();
	delete kernelData->OuterLoop_ctx;
	delete kernelData->RefHandling_ctx;

	delete kernelData->OuterLoop_ang_ref;
	delete kernelData->DataHandling_pos_msg;
	delete kernelData->RefHandling_pos_ref_out;
	delete kernelData->RefHandling_pos_ref_in;

	kernelData->tasks.clear();
	kernelData->currentTask = kernelData->tasks.begin();
	// Finally, delete the kernel data structure itself
	delete kernelData;
}


/************************************************************/

