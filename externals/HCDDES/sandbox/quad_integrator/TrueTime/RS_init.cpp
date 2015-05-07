/***
 * Generated code: DO NOT EDIT MANUALLY
 * For use in generating Matlab/Simulink + TrueTime
 * models from ESMoL models.  Please consult ESMoL
 * documentation for additional information.
***/


/*** Gloablly Included Header Files ***/
#define S_FUNCTION_NAME RS_init
#define mexPutArray
#include "ttkernel.cpp"
#include <map>
#include "InnerLoop_sl.c"
#include "DataHandler_sl.c"



/*** Message Structures ***/

// Message struct for message type OuterLoop_ang_ref
typedef struct {
	A5singleA InnerLoop_ang_ref;
	
} OuterLoop_ang_ref_t;

// Message struct for message type InnerLoop_thrust_commands
typedef struct {
	A1A9singleAA InnerLoop_Torque;
	
} InnerLoop_thrust_commands_t;

// Message struct for message type DataHandling_pos_msg
typedef struct {
	single DataHandling_pos_out;
	
} DataHandling_pos_msg_t;

// Message struct for message type DataHandling_sensor_data_in
typedef struct {
	single DataHandling_pos_in;
	single DataHandling_ang_in;
	
} DataHandling_sensor_data_in_t;

// Message struct for message type DataHandling_ang_msg
typedef struct {
	single DataHandling_ang_out;
	
} DataHandling_ang_msg_t;



/*** Setup Kernel Data Structure ***/
// Kernel data strucutre
struct KernelData {
	InnerLoop_context* InnerLoop_ctx;
	DataHandler_context* DataHandling_ctx;
	
	OuterLoop_ang_ref_t*		OuterLoop_ang_ref;
	InnerLoop_thrust_commands_t*		InnerLoop_thrust_commands;
	DataHandling_pos_msg_t*		DataHandling_pos_msg;
	DataHandling_sensor_data_in_t*		DataHandling_sensor_data_in;
	DataHandling_ang_msg_t*		DataHandling_ang_msg;
	
	// Other kernel data needed by the scheduler
	unsigned int								hyperperiod;
	double										hyperperiodStart;
	std::map< double, std::string >				tasks;
	std::map< double, std::string >::iterator	currentTask;
};


/************************************************************/

//Code to handle the execution of InnerLoop_task
double InnerLoop_code( int seg, void *data ) {
	// Get the kernel data
	KernelData *kernelData = (KernelData*)ttGetUserData();
	// Go through the two possible phases
	switch ( seg ) {
		// Execute component code
		case 1:
			// Execute the task component
			InnerLoop_main( 
				kernelData->InnerLoop_ctx,
				kernelData->OuterLoop_ang_ref->InnerLoop_ang_ref,
				kernelData->DataHandling_ang_msg->DataHandling_ang_out,
				&kernelData->InnerLoop_thrust_commands->InnerLoop_Torque
				);
			// Return the WCET for the task
			return 0.001900;
		
		// We are done phase
		default:
			return FINISHED;
	}
}
/************************************************************/

//Code to handle the execution of DataHandling_task
double DataHandling_code( int seg, void *data ) {
	// Get the kernel data
	KernelData *kernelData = (KernelData*)ttGetUserData();
	// Go through the two possible phases
	switch ( seg ) {
		// Execute component code
		case 1:
			// Execute the task component
			DataHandler_main( 
				kernelData->DataHandling_ctx,
				kernelData->DataHandling_sensor_data_in->DataHandling_ang_in,
				kernelData->DataHandling_sensor_data_in->DataHandling_pos_in,
				&kernelData->DataHandling_ang_msg->DataHandling_ang_out,
				&kernelData->DataHandling_pos_msg->DataHandling_pos_out
				);
			// Return the WCET for the task
			return 0.001800;
		
		// We are done phase
		default:
			return FINISHED;
	}
}


/************************************************************/

//Code to handle the execution of ADCChan_task
double ADCChan_code( int seg, void *data ) {
	// Get the kernel data
	KernelData *kernelData = (KernelData*)ttGetUserData();
	// Go through the two possible phases
	switch ( seg ) {
		// Execute component code
		case 1:
			
			// Return the WCET for the peripheral
			return 0.010000;
		
		// We are done phase
		default:
			return FINISHED;
	}
}
/************************************************************/

//Code to handle the execution of SerialIChan_task
double SerialIChan_code( int seg, void *data ) {
	// Get the kernel data
	KernelData *kernelData = (KernelData*)ttGetUserData();
	// Go through the two possible phases
	switch ( seg ) {
		// Execute component code
		case 1:
			
			// Get input for the peripheral
			kernelData->DataHandling_sensor_data_in->DataHandling_pos_in = (float)ttAnalogIn( 2 );
			kernelData->DataHandling_sensor_data_in->DataHandling_ang_in = (float)ttAnalogIn( 1 );
			
			// Return the WCET for the peripheral
			return 0.000001;
		
		// We are done phase
		default:
			return FINISHED;
	}
}
/************************************************************/

//Code to handle the execution of SerialOChan_task
double SerialOChan_code( int seg, void *data ) {
	// Get the kernel data
	KernelData *kernelData = (KernelData*)ttGetUserData();
	// Go through the two possible phases
	switch ( seg ) {
		// Execute component code
		case 1:
			
			// Send output for the peripheral
			ttAnalogOut( 1, kernelData->InnerLoop_thrust_commands->InnerLoop_Torque[0] );
			ttAnalogOut( 2, kernelData->InnerLoop_thrust_commands->InnerLoop_Torque[1] );
			ttAnalogOut( 3, kernelData->InnerLoop_thrust_commands->InnerLoop_Torque[2] );
			ttAnalogOut( 4, kernelData->InnerLoop_thrust_commands->InnerLoop_Torque[3] );
			ttAnalogOut( 5, kernelData->InnerLoop_thrust_commands->InnerLoop_Torque[4] );
			ttAnalogOut( 6, kernelData->InnerLoop_thrust_commands->InnerLoop_Torque[5] );
			ttAnalogOut( 7, kernelData->InnerLoop_thrust_commands->InnerLoop_Torque[6] );
			ttAnalogOut( 8, kernelData->InnerLoop_thrust_commands->InnerLoop_Torque[7] );
			ttAnalogOut( 9, kernelData->InnerLoop_thrust_commands->InnerLoop_Torque[8] );
			
			// Return the WCET for the peripheral
			return 0.001000;
		
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
			
			// Receive the message from the network
			localBuffer = ttGetMsg( 1 );
			// Check if null local buffer
			if ( localBuffer != NULL ) {
				// Copy data into the actual message buffer
				memcpy( kernelData->OuterLoop_ang_ref, localBuffer, sizeof( OuterLoop_ang_ref_t ) );
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

//Code to handle the communication of DataHandling_pos_msg
double DataHandling_pos_msg_code( int seg, void *data ) {
	// Get the kernel data
	KernelData *kernelData = (KernelData*)ttGetUserData();
	void* localBuffer;
	// Go through the two possible phases
	switch ( seg ) {
		// Execute component code
		case 1:
			// Send the message onto the network
			localBuffer = (void*)kernelData->DataHandling_pos_msg;
			ttSendMsg( 1, 2, localBuffer, sizeof( DataHandling_pos_msg_t ) );
			return 0.001000;
			
			

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


// Primary entry point for the RS_init Matlab function
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
	kernelData->InnerLoop_thrust_commands = new InnerLoop_thrust_commands_t;
	memset( kernelData->InnerLoop_thrust_commands, 0, sizeof(InnerLoop_thrust_commands_t) );
	kernelData->DataHandling_pos_msg = new DataHandling_pos_msg_t;
	memset( kernelData->DataHandling_pos_msg, 0, sizeof(DataHandling_pos_msg_t) );
	kernelData->DataHandling_sensor_data_in = new DataHandling_sensor_data_in_t;
	memset( kernelData->DataHandling_sensor_data_in, 0, sizeof(DataHandling_sensor_data_in_t) );
	kernelData->DataHandling_ang_msg = new DataHandling_ang_msg_t;
	memset( kernelData->DataHandling_ang_msg, 0, sizeof(DataHandling_ang_msg_t) );
	


	/*****************************************************/

	// Create and store pointer in kernel data
	kernelData->InnerLoop_ctx = new InnerLoop_context;
	// Initialize component and its context data
	InnerLoop_init( kernelData->InnerLoop_ctx );
	// Add all instances of the tasks to the global task list
	double InnerLoop_task_data[1] = { 0.009 };
	add_tasks( "InnerLoop_task", 1, InnerLoop_task_data );
	// Create a sporadic controller task
	ttCreateTask( "InnerLoop_task", 0.001900, InnerLoop_code );
	ttSetPriority( 2, "InnerLoop_task" );

	/*****************************************************/

	// Create and store pointer in kernel data
	kernelData->DataHandling_ctx = new DataHandler_context;
	// Initialize component and its context data
	DataHandler_init( kernelData->DataHandling_ctx );
	// Add all instances of the tasks to the global task list
	double DataHandling_task_data[1] = { 0.004 };
	add_tasks( "DataHandling_task", 1, DataHandling_task_data );
	// Create a sporadic controller task
	ttCreateTask( "DataHandling_task", 0.001800, DataHandling_code );
	ttSetPriority( 2, "DataHandling_task" );



	/*****************************************************/

	// Add all instances of the peripheral to the global task list
	double ADCChan_task_data[1] = { -1.0 };
	add_tasks( "ADCChan_task", 1, ADCChan_task_data );
	// Create a sporadic peripheral task
	ttCreateTask( "ADCChan_task", 0.010000, ADCChan_code );
	ttSetPriority( 2, "ADCChan_task" );

	/*****************************************************/

	// Add all instances of the peripheral to the global task list
	double SerialIChan_task_data[1] = { 0.001 };
	add_tasks( "SerialIChan_task", 1, SerialIChan_task_data );
	// Create a sporadic peripheral task
	ttCreateTask( "SerialIChan_task", 0.000001, SerialIChan_code );
	ttSetPriority( 2, "SerialIChan_task" );

	/*****************************************************/

	// Add all instances of the peripheral to the global task list
	double SerialOChan_task_data[1] = { 0.014 };
	add_tasks( "SerialOChan_task", 1, SerialOChan_task_data );
	// Create a sporadic peripheral task
	ttCreateTask( "SerialOChan_task", 0.001000, SerialOChan_code );
	ttSetPriority( 2, "SerialOChan_task" );



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
	delete kernelData->InnerLoop_ctx;
	delete kernelData->DataHandling_ctx;

	delete kernelData->OuterLoop_ang_ref;
	delete kernelData->InnerLoop_thrust_commands;
	delete kernelData->DataHandling_pos_msg;
	delete kernelData->DataHandling_sensor_data_in;
	delete kernelData->DataHandling_ang_msg;

	kernelData->tasks.clear();
	kernelData->currentTask = kernelData->tasks.begin();
	// Finally, delete the kernel data structure itself
	delete kernelData;
}


/************************************************************/

