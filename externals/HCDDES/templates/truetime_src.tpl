/***
 * Generated code: DO NOT EDIT MANUALLY
 * For use in generating Matlab/Simulink + TrueTime
 * models from ESMoL models.  Please consult ESMoL
 * documentation for additional information.
***/


/*** Gloablly Included Header Files ***/
#define S_FUNCTION_NAME {{FILENAME}}
#define mexPutArray
#include "{{FILENAME}}_defines.h"
{{#BUS_DEFINES}}#include "{{BUSNAME}}_init_defines.h"
{{/BUS_DEFINES}}
#include "ttkernel.cpp"
#include <map>
{{#TASK_SECTION}}{{#TASK_INCLUDE}}#include "{{TASK_HEADER}}"
{{/TASK_INCLUDE}}{{/TASK_SECTION}}


/*** Message Structures ***/
{{#MESSAGE_SECTION}}
// Message struct for message type {{MESSAGE_NAME}}
typedef struct {
	{{#SIGNAL_MEMBER}}{{SIGNAL_TYPENAME}} {{SIGNAL_NAME}};
	{{/SIGNAL_MEMBER}}
} {{MESSAGE_NAME}}_t;
{{/MESSAGE_SECTION}}


/*** Setup Kernel Data Structure ***/
// Kernel data structure
struct KernelData {
	{{#TASK_SECTION}}{{TASK_MSUBSYS}}_context* {{TASK_COMPNAME}}_ctx;
	{{/TASK_SECTION}}
	{{#MESSAGE_SECTION}}{{MESSAGE_NAME}}_t*		{{MESSAGE_NAME}};
	{{/MESSAGE_SECTION}}
	// Other kernel data needed by the scheduler
	unsigned int								hyperperiod;
	double										hyperperiodStart;
	std::map< double, std::string >				tasks;
	std::map< double, std::string >::iterator	currentTask;
};


{{#TASK_SECTION}}/************************************************************/

//Code to handle the execution of {{TASK_NAME}}
double {{TASK_FUNCTION}}( int seg, void *data ) {
	// Get the kernel data
	KernelData *kernelData = (KernelData*)ttGetUserData();
	// Go through the two possible phases
	switch ( seg ) {
		// Execute component code
		case 1:
			// Execute the task component
			{{TASK_MSUBSYS}}_main( 
				kernelData->{{TASK_COMPNAME}}_ctx{{#INPUT_SIGNALS}},
				kernelData->{{SIGNAL_MSGNAME}}->{{SIGNAL_NAME}}{{/INPUT_SIGNALS}}{{#OUTPUT_SIGNALS}},
				&kernelData->{{SIGNAL_MSGNAME}}->{{SIGNAL_NAME}}{{/OUTPUT_SIGNALS}}
				);
			// Return the WCET for the task
			return {{TASK_WCET_STR}};
		
		// We are done phase
		default:
			return FINISHED;
	}
}
{{/TASK_SECTION}}

{{#PERIPHERAL_SECTION}}/************************************************************/

//Code to handle the execution of {{PERIPHERAL_NAME}}
double {{PERIPHERAL_FUNCTION}}( int seg, void *data ) {
	// Get the kernel data
	KernelData *kernelData = (KernelData*)ttGetUserData();
	// Go through the two possible phases
	switch ( seg ) {
		// Execute component code
		case 1:
			{{#PERIPHERAL_INPUT}}
			// Get input for the peripheral
			{{#PERIPHERAL_MESSAGE}}kernelData->{{PERIPHERAL_MSGNAME}} = (float)ttAnalogIn( {{PERIPHERAL_CHANNEL}} );
			{{/PERIPHERAL_MESSAGE}}{{/PERIPHERAL_INPUT}}{{#PERIPHERAL_OUTPUT}}
			// Send output for the peripheral
			{{#PERIPHERAL_MESSAGE}}ttAnalogOut( {{PERIPHERAL_CHANNEL}}, kernelData->{{PERIPHERAL_MSGNAME}} );
			{{/PERIPHERAL_MESSAGE}}{{/PERIPHERAL_OUTPUT}}
			// Return the WCET for the peripheral
			return {{PERIPHERAL_WCET_STR}};
		
		// We are done phase
		default:
			return FINISHED;
	}
}
{{/PERIPHERAL_SECTION}}

{{#BUSMESSAGE_SECTION}}/************************************************************/

//Code to handle the communication of {{BUSMESSAGE_FULLNAME}}
double {{BUSMESSAGE_FUNCTION}}( int seg, void *data ) {
	// Get the kernel data
	KernelData *kernelData = (KernelData*)ttGetUserData();
	void* localBuffer;
	// Go through the two possible phases
	switch ( seg ) {
		// Execute component code
		case 1:{{#BUSMESSAGE_SEND}}
			// Send the message onto the network
			localBuffer = (void*)kernelData->{{BUSMESSAGE_FULLNAME}};
			{{#BUSMESSAGE_SENDBLOCK}}ttSendMsg( {{BUSMESSAGE_NETWORK}}, {{BUSMESSAGE_RECEIVER}}, localBuffer, sizeof( {{BUSMESSAGE_FULLNAME}}_t ) );{{/BUSMESSAGE_SENDBLOCK}}
			return {{BUSMESSAGE_WCET_STR}};
			{{/BUSMESSAGE_SEND}}
			{{#BUSMESSAGE_RECEIVE}}
			// Receive the message from the network
			localBuffer = ttGetMsg( {{BUSMESSAGE_NETWORK}} );
			// Check if null local buffer
			if ( localBuffer != NULL ) {
				// Copy data into the actual message buffer
				memcpy( kernelData->{{BUSMESSAGE_FULLNAME}}, localBuffer, sizeof( {{BUSMESSAGE_FULLNAME}}_t ) );
			} 
			return 0.000001;  // ${WCET} = {{BUSMESSAGE_WCET_STR}};
			{{/BUSMESSAGE_RECEIVE}}

		// We are done phase
		default:
			return FINISHED;
	}
}
{{/BUSMESSAGE_SECTION}}

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


// Primary entry point for the {{FILENAME}} Matlab function
void init() {
	// Initialize TrueTime kernel  
	ttInitKernel( {{KERNEL_SCHEDULE}} );
  
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
{{#MESSAGE_SECTION}}	kernelData->{{MESSAGE_NAME}} = new {{MESSAGE_NAME}}_t;
	memset( kernelData->{{MESSAGE_NAME}}, 0, sizeof({{MESSAGE_NAME}}_t) );
{{/MESSAGE_SECTION}}	

{{#TASK_SECTION}}
	/*****************************************************/

	// Create and store pointer in kernel data
	kernelData->{{TASK_COMPNAME}}_ctx = new {{TASK_MSUBSYS}}_context;
	// Initialize component and its context data
	{{TASK_MSUBSYS}}_init( kernelData->{{TASK_COMPNAME}}_ctx );
	// Add all instances of the tasks to the global task list
	double {{TASK_NAME}}_data[{{TASK_COUNT}}] = { {{TASK_SCHEDULE}} };
	add_tasks( "{{TASK_NAME}}", {{TASK_COUNT}}, {{TASK_NAME}}_data );
	// Create a sporadic controller task
	ttCreateTask( "{{TASK_NAME}}", {{TASK_WCET_STR}}, {{TASK_FUNCTION}} );
	ttSetPriority( 2, "{{TASK_NAME}}" );
{{/TASK_SECTION}}

{{#PERIPHERAL_SECTION}}
	/*****************************************************/

	// Add all instances of the peripheral to the global task list
	double {{PERIPHERAL_NAME}}_data[{{PERIPHERAL_COUNT}}] = { {{PERIPHERAL_SCHEDULE}} };
	add_tasks( "{{PERIPHERAL_NAME}}", {{PERIPHERAL_COUNT}}, {{PERIPHERAL_NAME}}_data );
	// Create a sporadic peripheral task
	ttCreateTask( "{{PERIPHERAL_NAME}}", {{PERIPHERAL_WCET_STR}}, {{PERIPHERAL_FUNCTION}} );
	ttSetPriority( 2, "{{PERIPHERAL_NAME}}" );
{{/PERIPHERAL_SECTION}}

{{#BUSMESSAGE_SECTION}}
	/*****************************************************/

	// Add all instances of the bus message to the global task list
	double {{BUSMESSAGE_NAME}}_data[{{BUSMESSAGE_COUNT}}] = { {{BUSMESSAGE_SCHEDULE}} };
	add_tasks( "{{BUSMESSAGE_NAME}}", {{BUSMESSAGE_COUNT}}, {{BUSMESSAGE_NAME}}_data );
	// Create a sporadic bus message task
	ttCreateTask( "{{BUSMESSAGE_NAME}}", {{BUSMESSAGE_WCET_STR}}, {{BUSMESSAGE_FUNCTION}} );
	ttSetPriority( 2, "{{BUSMESSAGE_NAME}}" );
{{/BUSMESSAGE_SECTION}}

	/*****************************************************/


	// Setup the master hyperperiod schedule and get ready to run
	kernelData->hyperperiod = 0;
	kernelData->currentTask = kernelData->tasks.begin();
	// Create the primary scheduler task
	ttCreatePeriodicTask( "FRODO_Scheduler", {{NODE_HYPERPERIOD_STR}}, scheduler_exec );
	ttSetPriority( 1, "FRODO_Scheduler" );
}


void cleanup() {
	// Free all dynamic memory allocated during run-time
	KernelData *kernelData = (KernelData*)ttGetUserData();
{{#TASK_SECTION}}	delete kernelData->{{TASK_COMPNAME}}_ctx;
{{/TASK_SECTION}}
{{#MESSAGE_SECTION}}	delete kernelData->{{MESSAGE_NAME}};
{{/MESSAGE_SECTION}}
	kernelData->tasks.clear();
	kernelData->currentTask = kernelData->tasks.begin();
	// Finally, delete the kernel data structure itself
	delete kernelData;
}


/************************************************************/

