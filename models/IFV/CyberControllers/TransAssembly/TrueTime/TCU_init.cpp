/***
 * Generated code: DO NOT EDIT MANUALLY
 * For use in generating Matlab/Simulink + TrueTime
 * models from ESMoL models.  Please consult ESMoL
 * documentation for additional information.
***/


/*** Gloablly Included Header Files ***/
#define S_FUNCTION_NAME TCU_init
#define mexPutArray
#include "ttkernel.cpp"
#include <map>
#include "AutoTransController_sl.c"
#include "PassThrough_sl.c"



/*** Message Structures ***/

// Message struct for message type AutoTransComp_2_GearRatioMsg
typedef struct {
	float AutoTransComp_2_gear_ratio;
	
} AutoTransComp_2_GearRatioMsg_t;

// Message struct for message type DieselEngineVelSensor_DataHandler_RPMMsg_In
typedef struct {
	float DieselEngineVelSensor_DataHandler_In;
	
} DieselEngineVelSensor_DataHandler_RPMMsg_In_t;

// Message struct for message type DieselEngineVelSensor_DataHandler_RPMMsg_Out
typedef struct {
	float DieselEngineVelSensor_DataHandler_Out;
	
} DieselEngineVelSensor_DataHandler_RPMMsg_Out_t;

// Message struct for message type TCU_DummyMsg
typedef struct {
	float AutoTransComp_2_gear_num;
	
} TCU_DummyMsg_t;



/*** Setup Kernel Data Structure ***/
// Kernel data structure
struct KernelData {
	AutoTransController_context* AutoTransComp_2_ctx;
	PassThrough_context* DieselEngineVelSensor_DataHandler_ctx;
	
	AutoTransComp_2_GearRatioMsg_t*		AutoTransComp_2_GearRatioMsg;
	DieselEngineVelSensor_DataHandler_RPMMsg_In_t*		DieselEngineVelSensor_DataHandler_RPMMsg_In;
	DieselEngineVelSensor_DataHandler_RPMMsg_Out_t*		DieselEngineVelSensor_DataHandler_RPMMsg_Out;
	TCU_DummyMsg_t*		TCU_DummyMsg;
	
	// Other kernel data needed by the scheduler
	unsigned int								hyperperiod;
	double										hyperperiodStart;
	std::map< double, std::string >				tasks;
	std::map< double, std::string >::iterator	currentTask;
};


/************************************************************/

//Code to handle the execution of AutoTransComp_2_task
double AutoTransComp_2_code( int seg, void *data ) {
	// Get the kernel data
	KernelData *kernelData = (KernelData*)ttGetUserData();
	// Go through the two possible phases
	switch ( seg ) {
		// Execute component code
		case 1:
			// Execute the task component
			AutoTransController_main( 
				kernelData->AutoTransComp_2_ctx,
				kernelData->DieselEngineVelSensor_DataHandler_RPMMsg_Out->DieselEngineVelSensor_DataHandler_Out,
				&kernelData->AutoTransComp_2_GearRatioMsg->AutoTransComp_2_gear_ratio,
				&kernelData->TCU_DummyMsg->AutoTransComp_2_gear_num
				);
			// Return the WCET for the task
			return 0.001000;
		
		// We are done phase
		default:
			return FINISHED;
	}
}
/************************************************************/

//Code to handle the execution of DieselEngineVelSensor_DataHandler_task
double DieselEngineVelSensor_DataHandler_code( int seg, void *data ) {
	// Get the kernel data
	KernelData *kernelData = (KernelData*)ttGetUserData();
	// Go through the two possible phases
	switch ( seg ) {
		// Execute component code
		case 1:
			// Execute the task component
			PassThrough_main( 
				kernelData->DieselEngineVelSensor_DataHandler_ctx,
				kernelData->DieselEngineVelSensor_DataHandler_RPMMsg_In->DieselEngineVelSensor_DataHandler_In,
				&kernelData->DieselEngineVelSensor_DataHandler_RPMMsg_Out->DieselEngineVelSensor_DataHandler_Out
				);
			// Return the WCET for the task
			return 0.001000;
		
		// We are done phase
		default:
			return FINISHED;
	}
}


/************************************************************/

//Code to handle the execution of DieselEngineVelSensor_task
double DieselEngineVelSensor_code( int seg, void *data ) {
	// Get the kernel data
	KernelData *kernelData = (KernelData*)ttGetUserData();
	// Go through the two possible phases
	switch ( seg ) {
		// Execute component code
		case 1:
			
			// Get input for the peripheral
			kernelData->DieselEngineVelSensor_DataHandler_RPMMsg_In->DieselEngineVelSensor_DataHandler_In = (float)ttAnalogIn( 1 );
			
			// Return the WCET for the peripheral
			return 0.001000;
		
		// We are done phase
		default:
			return FINISHED;
	}
}
/************************************************************/

//Code to handle the execution of GearRatioTransducer_task
double GearRatioTransducer_code( int seg, void *data ) {
	// Get the kernel data
	KernelData *kernelData = (KernelData*)ttGetUserData();
	// Go through the two possible phases
	switch ( seg ) {
		// Execute component code
		case 1:
			
			// Send output for the peripheral
			ttAnalogOut( 1, kernelData->AutoTransComp_2_GearRatioMsg->AutoTransComp_2_gear_ratio );
			
			// Return the WCET for the peripheral
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
		if ( times[i] > 0.0 ) {
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
	return 0.000001;
}


/************************************************************/


// Primary entry point for the TCU_init Matlab function
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
	kernelData->AutoTransComp_2_GearRatioMsg = new AutoTransComp_2_GearRatioMsg_t;
	memset( kernelData->AutoTransComp_2_GearRatioMsg, 0, sizeof(AutoTransComp_2_GearRatioMsg_t) );
	kernelData->DieselEngineVelSensor_DataHandler_RPMMsg_In = new DieselEngineVelSensor_DataHandler_RPMMsg_In_t;
	memset( kernelData->DieselEngineVelSensor_DataHandler_RPMMsg_In, 0, sizeof(DieselEngineVelSensor_DataHandler_RPMMsg_In_t) );
	kernelData->DieselEngineVelSensor_DataHandler_RPMMsg_Out = new DieselEngineVelSensor_DataHandler_RPMMsg_Out_t;
	memset( kernelData->DieselEngineVelSensor_DataHandler_RPMMsg_Out, 0, sizeof(DieselEngineVelSensor_DataHandler_RPMMsg_Out_t) );
	kernelData->TCU_DummyMsg = new TCU_DummyMsg_t;
	memset( kernelData->TCU_DummyMsg, 0, sizeof(TCU_DummyMsg_t) );
	


	/*****************************************************/

	// Create and store pointer in kernel data
	kernelData->AutoTransComp_2_ctx = new AutoTransController_context;
	// Initialize component and its context data
	AutoTransController_init( kernelData->AutoTransComp_2_ctx );
	// Add all instances of the tasks to the global task list
	double AutoTransComp_2_task_data[1] = { 0.05 };
	add_tasks( "AutoTransComp_2_task", 1, AutoTransComp_2_task_data );
	// Create a sporadic controller task
	ttCreateTask( "AutoTransComp_2_task", 0.001000, AutoTransComp_2_code );
	ttSetPriority( 2, "AutoTransComp_2_task" );

	/*****************************************************/

	// Create and store pointer in kernel data
	kernelData->DieselEngineVelSensor_DataHandler_ctx = new PassThrough_context;
	// Initialize component and its context data
	PassThrough_init( kernelData->DieselEngineVelSensor_DataHandler_ctx );
	// Add all instances of the tasks to the global task list
	double DieselEngineVelSensor_DataHandler_task_data[1] = { 0.025 };
	add_tasks( "DieselEngineVelSensor_DataHandler_task", 1, DieselEngineVelSensor_DataHandler_task_data );
	// Create a sporadic controller task
	ttCreateTask( "DieselEngineVelSensor_DataHandler_task", 0.001000, DieselEngineVelSensor_DataHandler_code );
	ttSetPriority( 2, "DieselEngineVelSensor_DataHandler_task" );



	/*****************************************************/

	// Add all instances of the peripheral to the global task list
	double DieselEngineVelSensor_task_data[1] = { 0.012 };
	add_tasks( "DieselEngineVelSensor_task", 1, DieselEngineVelSensor_task_data );
	// Create a sporadic peripheral task
	ttCreateTask( "DieselEngineVelSensor_task", 0.001000, DieselEngineVelSensor_code );
	ttSetPriority( 2, "DieselEngineVelSensor_task" );

	/*****************************************************/

	// Add all instances of the peripheral to the global task list
	double GearRatioTransducer_task_data[1] = { 0.075 };
	add_tasks( "GearRatioTransducer_task", 1, GearRatioTransducer_task_data );
	// Create a sporadic peripheral task
	ttCreateTask( "GearRatioTransducer_task", 0.001000, GearRatioTransducer_code );
	ttSetPriority( 2, "GearRatioTransducer_task" );




	/*****************************************************/


	// Setup the master hyperperiod schedule and get ready to run
	kernelData->hyperperiod = 0;
	kernelData->currentTask = kernelData->tasks.begin();
	// Create the primary scheduler task
	ttCreatePeriodicTask( "FRODO_Scheduler", 0.100000, scheduler_exec );
	ttSetPriority( 1, "FRODO_Scheduler" );
}


void cleanup() {
	// Free all dynamic memory allocated during run-time
	KernelData *kernelData = (KernelData*)ttGetUserData();
	delete kernelData->AutoTransComp_2_ctx;
	delete kernelData->DieselEngineVelSensor_DataHandler_ctx;

	delete kernelData->AutoTransComp_2_GearRatioMsg;
	delete kernelData->DieselEngineVelSensor_DataHandler_RPMMsg_In;
	delete kernelData->DieselEngineVelSensor_DataHandler_RPMMsg_Out;
	delete kernelData->TCU_DummyMsg;

	kernelData->tasks.clear();
	kernelData->currentTask = kernelData->tasks.begin();
	// Finally, delete the kernel data structure itself
	delete kernelData;
}


/************************************************************/

