/*** Copyright Vanderbilt ***/


/*** Included Header Files ***/
#include "error_handler.h"
#include "scheduler.h"
#include "udp.h"
#include "serialio.h"
#include "logger.h"
#include "highres_timing.h"


/*****************************************************************************/


/*** Static Variables ***/
static bool						_ehInit = false;							// Flag for error handler initialization


/*** Externally Defined Functions and Variables ***/
extern int						_UDPGetError(void);
extern int						_SerialGetError(void);
extern bool						_schedExec;


/*****************************************************************************/


/* ErrScheduler Handling Functions */
uint8_t _ErrSchedulerSerialize( EventCategory category, EventID ID, char* buffer, va_list list );
void _ErrSchedulerPrint( char* string, EventCategory category, EventID ID, char* data );
void _ErrSchedulerProcess( EventCategory category, EventID ID, char* data );
/* ErrSerial Handling Functions */
uint8_t _ErrSerialSerialize( EventCategory category, EventID ID, char* buffer, va_list list );
void _ErrSerialPrint( char* string, EventCategory category, EventID ID, char* data );
void _ErrSerialProcess( EventCategory category, EventID ID, char* data );
/* ErrUDP Handling Functions */
uint8_t _ErrUDPSerialize( EventCategory category, EventID ID, char* buffer, va_list list );
void _ErrUDPPrint( char* string, EventCategory category, EventID ID, char* data );
void _ErrUDPProcess( EventCategory category, EventID ID, char* data );


/*****************************************************************************/


/* Initialize the error handler subsystem */
void ErrorHandlerInitialize( void ) {
	// Are we already initialized
	if ( _ehInit ) return;
	// Register all of the error handler events
	SysEventRegisterCategory( "Error Handler", Error, 3 );
	SysEventRegister( Error, ErrScheduler,	_ErrSchedulerSerialize, _ErrSchedulerPrint, NULL );
	SysEventRegister( Error, ErrSerial,		_ErrSerialSerialize, _ErrSerialPrint, NULL );
	SysEventRegister( Error, ErrUDP,		_ErrUDPSerialize, _ErrUDPPrint, NULL );
	
}


/* For now, just do nothing */
char* ErrGetErrorCondition( void ) {
	return "Problem with the warp drive\0";
}


/*****************************************************************************/


/* Default handler for scheduling errors */
void ErrSchedulerHandler( ErrorCode errorCode, void* object, pfloat_t atTimeMS ) {
	char logBuffer[128];
	pfloat_t timeTemp;
	// Cast to a schedulable
	SchedSchedulable *schedulable = (SchedSchedulable*)object;
	SchedInstance *instance = NULL;
	// Check some values
	assert( atTimeMS >= 0.0 );
	// Switch on the error code
	switch ( errorCode ) {
		case FS_ERR_SLEEPDEADLINEMISS:
			// What time is it now
			timeTemp = GetTimeMS();
			// Print some info
			sprintf( logBuffer, "Deadline: Req: %4.2f, Act: %4.2f, Ex: %4.3f.\n", atTimeMS, timeTemp, timeTemp - atTimeMS );
			SysEvent( Error, ErrScheduler, FS_ERR_SLEEPDEADLINEMISS, logBuffer );
			break;
		case FS_ERR_TASKWCETOVERRUN:
			// Check some values
			instance = (SchedInstance*)object;
			assert( instance != NULL );
			// Print some info
			sprintf( logBuffer, "Overrun: %s WCET:%4.1f Sched:%4.2f Start:%4.2f, Stop:%4.2f Overrun:%4.3f.\n",
					 instance->schedulable->name, instance->schedulable->wcetMS,
					 instance->timevalMS, instance->schedulable->execInit, instance->schedulable->execHalt, atTimeMS );
			SysEvent( Error, ErrScheduler, FS_ERR_TASKWCETOVERRUN, logBuffer );
			break;
		case FS_ERR_SPORADICDEADLINEMISS:
			// Check some values
			assert( schedulable != NULL );
			// Print some info
			sprintf( logBuffer, "Sporadic Task %s Pruned (WCET:%4.1f) @%4.3f.\n", schedulable->name, schedulable->wcetMS, atTimeMS );
			SysEvent( Error, ErrScheduler, FS_ERR_SPORADICDEADLINEMISS, logBuffer );
			break;
		case FS_ERR_SPORADICTOOOFTEN:
			// Check some values
			assert( schedulable != NULL );
			// Print some info
			sprintf( logBuffer, "*** Sporadic %s Too Often -- Freq: %d @ %4.3f.\n", schedulable->name, schedulable->frequencyHZ, atTimeMS );
			SysEvent( Error, ErrScheduler, FS_ERR_SPORADICTOOOFTEN, logBuffer );
			break;
		case FS_ERR_PERIODICCREATEFAILURE:
			// Print some info
			printf( "*** Not able to create periodic task %s -- EXITING!!!\n", schedulable->name );
			// Not recoverable
			exit( -FS_ERR_PERIODICCREATEFAILURE );
			break;
		case FS_ERR_SPORADICCREATEFAILURE:
			// Print some info
			printf("*** Not able to create sporadic task %s -- EXITING!!!\n", schedulable->name );
			// Not recoverable
			exit( -FS_ERR_SPORADICCREATEFAILURE );
			break;
		case FS_ERR_INITFAILURE:
			// Print some info
			printf("*** Error initializing the scheduler -- EXITING!!!\n" );
			// Not recoverable
			exit( -FS_ERR_INITFAILURE );
			break;
		case FS_ERR_SCHEDFIFOUNAVAILABLE:
			// Print some info
			sprintf( logBuffer, "Error setting scheduler to SCHED_FIFO with high priority (make sure you are root): %s\n", strerror( errno ) );
			SysEvent( Error, ErrScheduler, FS_ERR_SCHEDFIFOUNAVAILABLE, logBuffer );
			// Can we really continue (nope)?
			exit( -FS_ERR_SCHEDFIFOUNAVAILABLE );
			break;
		case FS_ERR_SCHEDPRIORITYNOTSET:
			// Print some info
			sprintf( logBuffer, "Error setting scheduler to correct priority.\n" );
			SysEvent( Error, ErrScheduler, FS_ERR_SCHEDPRIORITYNOTSET, logBuffer );
			break;
		case FS_ERR_SETAFFINITYFAIL:
			// Print some info
			sprintf( logBuffer, "Error setting CPU affinity.\n" );
			SysEvent( Error, ErrScheduler, FS_ERR_SETAFFINITYFAIL, logBuffer );
			break;
		default:
			sprintf( logBuffer, "Invalid error code for SchedulerError: %d @ %4.3f.\n", errorCode, atTimeMS );
			SysEvent( Error, ErrScheduler, 0, logBuffer );
			break;
	}
}


/* Serialize a Scheduler Error event */
uint8_t _ErrSchedulerSerialize( EventCategory category, EventID ID, char* buffer, va_list list ) {
	// Just looking for the absolute clock time
	pfloat_t timeMS = AbsoluteTimeMS() ;
	// Copy the value into the buffer
	memcpy( buffer, &timeMS, sizeof(pfloat_t) );
	// Get the in-hyperperiod time
	timeMS = GetTimeMS();
	// Write the in-hyperperiod time
	memcpy( buffer + sizeof(pfloat_t), &timeMS, sizeof(pfloat_t) );
	// Return the number of bytes
	return sizeof(pfloat_t) * 2;
}


/* Print a Scheduler Error event */
void _ErrSchedulerPrint( char* string, EventCategory category, EventID ID, char* data ) {
	pfloat_t absoluteTimeMS, relativeTimeMS;
	// Retrieve the values from the buffer
	memcpy( &absoluteTimeMS, data, sizeof(pfloat_t) );
	memcpy( &relativeTimeMS, data + sizeof(pfloat_t), sizeof(pfloat_t) );
	// Print to the string
	sprintf( string, "%4.3f:\tScheduler Error.\n", absoluteTimeMS, relativeTimeMS );
}


/* Process a Scheduler Error event */
void _ErrSchedulerProcess( EventCategory category, EventID ID, char* data ) {
}


/*****************************************************************************/


/* UDP Error Handler */
void ErrUDPHandler( ErrorCode errorCode, void* object, pfloat_t atTimeMS ) {
	char logBuffer[128];
	// Cast to a UDP_Channel
	UDPChannel *channel = (UDPChannel*)object;
	// Make sure we have a channel
	assert( channel != NULL );
	// Switch on the type of error
	switch ( errorCode ) {
		case UDP_ERR_BINDSOCKETFAIL:
			// Log the error
			sprintf( logBuffer, "Bind Socket Failure on channel %s:%d - %s\n", channel->localAddress, channel->receivePort,
				   strerror( _UDPGetError() ) );
			SysEvent( Error, ErrUDP, UDP_ERR_BINDSOCKETFAIL, logBuffer );
			break;
		case UDP_ERR_CREATERECVSOCKETFAIL:
			// Log the error
			sprintf( logBuffer, "Create Socket Failure on channel %s:%d - %s\n", channel->localAddress, channel->receivePort, strerror( _UDPGetError() ) );
			SysEvent( Error, ErrUDP, UDP_ERR_CREATERECVSOCKETFAIL, logBuffer );
			break;
		case UDP_ERR_CLOSESOCKETFAIL:
			// Log the error
			sprintf( logBuffer, "Close Socket Failure - %s\n", strerror( _UDPGetError() ) );
			SysEvent( Error, ErrUDP, UDP_ERR_CLOSESOCKETFAIL, logBuffer );
			break;
		case UDP_ERR_SELECTFAIL:
			// Log the error
			sprintf( logBuffer, "Select Failure on channel %s:%d - %s\n", channel->localAddress, channel->receivePort, strerror( _UDPGetError() ) );
			SysEvent( Error, ErrUDP, UDP_ERR_SELECTFAIL, logBuffer );
			break;
		case UDP_ERR_RECVFROMFAIL:
			// Log the error
			sprintf( logBuffer, "RecvFrom Failure on channel %s:%d - %s\n", channel->localAddress, channel->receivePort, strerror( _UDPGetError() ) );
			SysEvent( Error, ErrUDP, UDP_ERR_RECVFROMFAIL, logBuffer );
			break;
		case UDP_ERR_SENDTOFAIL:
			// Log the error
			sprintf( logBuffer, "SendTo Failure on channel %s:%d - %s\n", channel->localAddress, channel->receivePort, strerror( _UDPGetError() ) );
			SysEvent( Error, ErrUDP, UDP_ERR_SENDTOFAIL, logBuffer );
			break;
		case UDP_ERR_PORTREADFAIL:
			// Log the error
			sprintf( logBuffer, "Failure to read from port on channel %s:%d - Likely port has not been writen to.\n", channel->localAddress, channel->receivePort );
			SysEvent( Error, ErrUDP, UDP_ERR_PORTREADFAIL, logBuffer );
			break;
		case FP_ERR_SYNCFAIL:
			// Log the error
			sprintf( logBuffer, "Initialize syncronization error in %s: %d\n", channel->localAddress, _UDPGetError() );
			SysEvent( Error, ErrUDP, FP_ERR_SYNCFAIL, logBuffer );
			// Set the run flag to false
			_schedExec = false;
			break;
		case FP_ERR_CHANNELCREATEFAIL:
			// Log the error
			sprintf( logBuffer, "Channel (%s) was not able to be created. Error: %d\n", channel->localAddress, _UDPGetError() );
			SysEvent( Error, ErrUDP, FP_ERR_CHANNELCREATEFAIL, logBuffer );
			// Set the run flag to false
			_schedExec = false;
			break;
		case FP_ERR_CHANNELDESTROYFAIL:
			// Log the error
			sprintf( logBuffer, "Channel (%s) was not able to be destroyed. Error: %d\n", channel->localAddress, _UDPGetError() );
			SysEvent( Error, ErrUDP, FP_ERR_CHANNELDESTROYFAIL, logBuffer );
			break;
		case FP_ERR_UNEXPECTEDMSGIDRECV:
			// Log the error
			sprintf( logBuffer, "Unexpected message received on %s.\n", channel->localAddress );
			SysEvent( Error, ErrUDP, FP_ERR_UNEXPECTEDMSGIDRECV, logBuffer );
			break;
		case FP_ERR_UNEXPECTEDMSGIDSEND:
			// Log the error
			sprintf( logBuffer, "Unexpected message send on %s.\n", channel->localAddress );
			SysEvent( Error, ErrUDP, FP_ERR_UNEXPECTEDMSGIDSEND, logBuffer );
			break;
		case FP_ERR_WRONGMSGSIZE:
			// Log the error
			sprintf( logBuffer, "Unexpected message size on %s.\n", channel->localAddress );
			SysEvent( Error, ErrUDP, FP_ERR_WRONGMSGSIZE, logBuffer );
			break;
		case FP_ERR_FAILURETOSEND:
			// Log the error
			sprintf( logBuffer, "Message not able to be sent on %s.\n", channel->localAddress );
			SysEvent( Error, ErrUDP, FP_ERR_FAILURETOSEND, logBuffer );
			break;
		case FP_ERR_FAILURETORECEIVE:
			// Log the error
			sprintf( logBuffer, "\tMessage not received on %s by Deadline:%4.3f, Now:%4.3f.\n", channel->localAddress, atTimeMS, GetTimeMS() );
			SysEvent( Error, ErrUDP, FP_ERR_FAILURETORECEIVE, logBuffer );
			break;
		default:
			sprintf( logBuffer, "Invalid error code for UDPError: %d @ %4.3f.\n", errorCode, atTimeMS );
			SysEvent( Error, ErrUDP, 0, logBuffer );
			break;
	}
}


/* Serialize a UDP Error event */
uint8_t _ErrUDPSerialize( EventCategory category, EventID ID, char* buffer, va_list list ) {
	// Just looking for the absolute clock time
	pfloat_t timeMS = AbsoluteTimeMS();
	// Get the error code from the va_list
	int errorID = va_arg( list, int );
	// Get the message from the va_list
	char* message = va_arg( list, char* );
	// See how big the message is (up to TEXTLENGTH)
	uint16_t messageSize = min( strlen( message ) + 1, LOGGER_TEXTLENGTH );
	// Copy the absolute time into the buffer
	memcpy( buffer, &timeMS, sizeof(pfloat_t) );
	buffer += sizeof(pfloat_t);
	// Copy the error code into the buffer
	memcpy( buffer, &errorID, sizeof(int) );
	buffer += sizeof(int);
	// Copy the size of the message into the buffer
	memcpy( buffer, &messageSize, sizeof(uint16_t) );
	buffer += sizeof(uint16_t);
	// Copy in the message itself
	strncpy( buffer, message, messageSize );
	buffer += messageSize;
	// Get the interval time
	timeMS = GetTimeMS();
	memcpy( buffer, &timeMS, sizeof(pfloat_t) );
	// Return the number of bytes written into the buffer
	return sizeof(pfloat_t) * 2 + sizeof(int) + messageSize + sizeof(uint16_t);
}


/* Print a UDP Error event */
void _ErrUDPPrint( char* string, EventCategory category, EventID ID, char* data ) {
	pfloat_t absoluteTimeMS, intervalTimeMS;
	uint16_t messageSize;
	int errorID;
	char message[LOGGER_TEXTLENGTH];
	// Retrieve the values from the buffer
	memcpy( &absoluteTimeMS, data, sizeof(pfloat_t) );
	data += sizeof(pfloat_t);
	memcpy( &errorID, data, sizeof(int) );
	data += sizeof(int);
	memcpy( &messageSize, data, sizeof(uint16_t) );
	data += sizeof(uint16_t);
	// Copy the message out
	strncpy( message, data, messageSize );
	data += messageSize;
	// Retrieve the interval time
	memcpy( &intervalTimeMS, data, sizeof(pfloat_t) );
	// Print to the string
	sprintf( string, "%4.3f:(@%4.3f)\t UDP Error: %s", absoluteTimeMS, intervalTimeMS, message );
}


/* Process a UDP Error event */
void _ErrUDPProcess( EventCategory category, EventID ID, char* data ) {
}


/*****************************************************************************/


/* Serial Error Handler */
void ErrSerialHandler( ErrorCode errorCode, void* object, pfloat_t atTimeMS ) {
	char logBuffer[128];
	// Cast to a Serial_Channel
	Serial_Channel *channel = (Serial_Channel*)object;
	// Make sure we have a channel
	assert( channel != NULL );
	// Switch on the type of error
	switch ( errorCode ) {
		case SERIAL_ERR_OPENPORTFAIL:
                        sprintf( logBuffer, "\tFailed to open serial port %s at time:%4.3f.\n", channel->serialDevice, GetTimeMS() );
                        SysEvent( Error, ErrSerial, SERIAL_ERR_OPENPORTFAIL, logBuffer );
                        break;
		case SERIAL_ERR_READPORTFAIL:
                        sprintf( logBuffer, "\tFailed to read serial port %s at time:%4.3f.\n", channel->serialDevice, GetTimeMS() );
                        SysEvent( Error, ErrSerial, SERIAL_ERR_READPORTFAIL, logBuffer );
                        break;
		case SERIAL_ERR_SENDTOFAIL:
                        sprintf( logBuffer, "\tFailed to send on serial port %s at time:%4.3f.\n", channel->serialDevice, GetTimeMS() );
                        SysEvent( Error, ErrSerial, SERIAL_ERR_SENDTOFAIL, logBuffer );
                        break;
		case SERIAL_ERR_BADBAUDRATE:
                        sprintf( logBuffer, "\tError configuring serial port %s at time:%4.3f. \nInvalid baud rate %d. Try one of (38400,57600,115200).\n", channel->serialDevice, GetTimeMS(), channel->baud );
                        SysEvent( Error, ErrSerial, SERIAL_ERR_BADBAUDRATE, logBuffer );
                        break;
		case SERIAL_ERR_BADDATABITS:
                        sprintf( logBuffer, "\tError configuring serial port %s at time:%4.3f.\nInvalid number of data bits %d.  Try one of (5,6,7,8).\n", channel->serialDevice, GetTimeMS(), channel->dataBits );
                        SysEvent( Error, ErrSerial, SERIAL_ERR_BADDATABITS, logBuffer );
                        break;
		case SERIAL_ERR_CLOSEFAIL:
                        sprintf( logBuffer, "\tFailed to close serial port %s at time:%4.3f.\n", channel->serialDevice, GetTimeMS() );
                        SysEvent( Error, ErrSerial, SERIAL_ERR_CLOSEFAIL, logBuffer );
                        break;
		case SERIAL_ERR_SELECTFAIL:
                        sprintf( logBuffer, "\tSelect failed for serial port %s at time:%4.3f.\n", channel->serialDevice, GetTimeMS() );
                        SysEvent( Error, ErrSerial, SERIAL_ERR_SELECTFAIL, logBuffer );
                        break;
                default:
			sprintf( logBuffer, "Invalid error code for SerialError: %d @ %4.3f.\n", errorCode, atTimeMS );
			SysEvent( Error, ErrSerial, 0, logBuffer );
  
        }
}


/* Serialize a Serial Error event */
uint8_t _ErrSerialSerialize( EventCategory category, EventID ID, char* buffer, va_list list ) {
	// Just looking for the absolute clock time
	pfloat_t timeMS = AbsoluteTimeMS() ;
	// Copy the value into the buffer
	memcpy( buffer, &timeMS, sizeof(pfloat_t) );
	// Get the in-hyperperiod time
	timeMS = GetTimeMS();
	// Write the in-hyperperiod time
	memcpy( buffer + sizeof(pfloat_t), &timeMS, sizeof(pfloat_t) );
	// Return the number of bytes
	return sizeof(pfloat_t) * 2;
}


/* Print a Serial Error event */
void _ErrSerialPrint( char* string, EventCategory category, EventID ID, char* data ) {
	pfloat_t absoluteTimeMS, relativeTimeMS;
	// Retrieve the values from the buffer
	memcpy( &absoluteTimeMS, data, sizeof(pfloat_t) );
	memcpy( &relativeTimeMS, data + sizeof(pfloat_t), sizeof(pfloat_t) );
	// Print to the string
	sprintf( string, "%4.3f:\tSerial Error.\n", absoluteTimeMS, relativeTimeMS );
}


/* Process a Serial Error event */
void _ErrSerialProcess( EventCategory category, EventID ID, char* data ) {
}


/*****************************************************************************/

