/***
 * Generated code: DO NOT EDIT MANUALLY
 * For use in generated FRODO run-time of
 * models from ESMoL models.  Please consult ESMoL
 * documentation for additional information.
***/


/*** Gloablly Included Header Files ***/
#include "frodo.h"
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
typedef struct {
	{{#TASK_SECTION}}{{TASK_MSUBSYS}}_context* {{TASK_NAME}}_ctx;
	SchedSchedulable*	{{TASK_COMPNAME}};
	{{/TASK_SECTION}}
	{{#MESSAGE_SECTION}}PortID_t		{{MESSAGE_NAME}}_port;
	{{/MESSAGE_SECTION}}
} Context_t;


{{#TASK_SECTION}}/************************************************************/

//Code to handle the execution of {{TASK_NAME}}
void* {{TASK_FUNCTION}}( void *cntx ) {
	// Cast to a context structure
	Context_t *context = (Context_t*)cntx;{{#INPUT_MESSAGE}}
	{{MESSAGE_NAME}}_t {{MESSAGE_NAME}}_local;
	{{/INPUT_MESSAGE}}{{#OUTPUT_MESSAGE}}
	{{MESSAGE_NAME}}_t {{MESSAGE_NAME}}_local;
	{{/OUTPUT_MESSAGE}}
	ReturnCode_t returnCode;
	// Loop forever
	while (true)
	{
		// Wait for signal to begin execution
		SchedSignalExecution( context->{{TASK_COMPNAME}} );
		// Read any incoming message ports
		{{#INPUT_MESSAGE}}ReadSamplingMessage( context->{{MESSAGE_NAME}}_port, &{{MESSAGE_NAME}}_local, NULL, NULL, &returnCode );
		{{/INPUT_MESSAGE}}
		// Execute the task component
		{{TASK_MSUBSYS}}_main( 
			context->{{TASK_COMPNAME}}_ctx{{#INPUT_SIGNALS}},
			{{SIGNAL_MSGNAME}}_local->{{SIGNAL_NAME}}{{/INPUT_SIGNALS}}{{#OUTPUT_SIGNALS}},
			&{{SIGNAL_MSGNAME}}_local->{{SIGNAL_NAME}}{{/OUTPUT_SIGNALS}}
			);

		// Write any outgoing message ports
		{{#OUTPUT_MESSAGE}}WriteSamplingMessage( context->{{MESSAGE_NAME}}_port, &{{MESSAGE_NAME}}_local, sizeof({{MESSAGE_NAME}}_t), &returnCode );
		{{/OUTPUT_MESSAGE}}
		// Signal execution has completed
		SchedSignalCompletion( context->{{TASK_COMPNAME}} );
	}
	// Must return something - never get here until shutdown
	return NULL;
}
{{/TASK_SECTION}}

/************************************************************/


// Primary entry function
void main( int argc, char* argv[] ) {
	ReturnCode_t returnCode;
	// Create the master context
	Context_t context;
	// Zero the context
	memset( &context, 0, sizeof(Context_t) );
 
	/*** Initialize the FRODO subsystems ***/

	SysEventInitialize( AllCategories, false, true, false );
	PeripheralInitialize();
	LogInitialize();
	ErrorHandlerInitialize();
	SchedInitialize( EarliestDeadlineFirst, ErrSchedulerHandler, {{KERNEL_HYPERPERIOD}}, 1, &context );
	UDPInitialize( ErrUDPHandler );

	/*** Create message ports ***/

{{#MESSAGE_SECTION}}	{
		{{MESSAGE_NAME}}_t data;
		memset( &data, 0, sizeof({{MESSAGE_NAME}}_t );
		CreateSamplingPort( "{{MESSAGE_NAME}}", sizeof({{MESSAGE_NAME}}_t), Bidirectional, 0, &(context.{{MESSAGE_NAME}}), &returnCode );
		WriteSamplingMessage( context.{{MESSAGE_NAME}}, &data, sizeof({{MESSAGE_NAME}}_t), &returnCode );
	}
{{/MESSAGE_SECTION}}	

	/*****************************************************/

{{#TASK_SECTION}}	{
		// Create task with instances
		pfloat_t {{TASK_NAME}}_data[{{TASK_COUNT}}] = { {{TASK_SCHEDULE}} };
		SchedCreatePeriodicTask( "{{TASK_NAME}}", {{TASK_FUNCTION}}, &context.{{TASK_NAME}}, {{TASK_WCET}}, NoRestriction, {{TASK_COUNT}}, {{TASK_NAME}}_times );
	}
{{/TASK_SECTION}}


	/*****************************************************/

{{#TTNETWORK_SECTION}}	{
		\\Found {{NETWORK_NAME}}
		{{#MESSAGE_SECTION}}
		pfloat {{MESSAGE_NAME}}_send_times[{{MESSAGE_SEND_COUNT}}] = {{MESSAGE_SEND_SCHEDULE}};
		pfloat {{MESSAGE_NAME}}_recv_times[{{MESSAGE_RECV_COUNT}}] = {{MESSAGE_RECV_SCHEDULE}};
		SyncExpectation {{NETWORK_NAME}}Expects[{{NETWORK_COUNT}}] = {
			{{#MESSAGE_SECTION}}{ ExpectSend, 1, sizeof(asdfads), {{MESSAGE_SEND_COUNT}}, {{MESSAGE_NAME}}_send_times, {{MESSAGE_WCET}}, context.{{MESSAGE_NAME}}, NULL },
			{ ExpectReceive, 1, sizeof(data_t), 1, receive_times, 2.0, context.msgPort, NULL }
			{{/MESSAGE_SECTION}}
		};
		context.{{NETWORK_NAME}} = UDPCreateChannel( argv[1], argv[2], 21212, {{NETWORK_COUNT}}, {{NETWORK_NAME}}Expects, 0, NULL, true, {{NETWORK_MASTER}}, {{NETWORK_PEER_COUNT}} );
		{{/MESSAGE_SECTION}}
	}
{{/TTNETWORK_SECTION}}

	/*****************************************************/

	/*** Execute the schedule ***/

	// Wait for the platform to wake up ( Platform Dependent )
	NanoSleep( SCHEDULER_INITIALIZATION_WAIT );
	// Execute the schedule
	SchedExecute( atoi(argv[3]) );

	/*** Shutdown the application ***/

	// If this returns we must shut everything down
	UDPShutdown( );
	SchedShutdown( );
	SysEventShutdown( NULL );
}


/************************************************************/

