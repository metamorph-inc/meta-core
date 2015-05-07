/*** Copyright Vanderbilt ***/


/*** Included Header Files ***/
#include "logger.h"
#include "events.h"
#include "udp.h"
#include "scheduler.h"
#include "peripheral.h"
#include "error_handler.h"
#include "highres_timing.h"


/*****************************************************************************/


// Default Logger value definitions
#ifndef LOGGER_TEXTLENGTH
#define LOGGER_TEXTLENGTH					128							//
#endif


/*** Static Variables ***/
static bool								_logInit = false;				// Flag for logger initialization


/*****************************************************************************/


/* LogMessage Handling Functions */
uint8_t _LogMessageSerialize( EventCategory category, EventID ID, char* buffer, va_list list );
void _LogMessagePrint( char* string, EventCategory category, EventID ID, char* data );
void _LogMessageProcess( EventCategory category, EventID ID, char* data );
/* Signal Event Handling Functions */
uint8_t _SignalsSerialize( EventCategory category, EventID ID, char* buffer, va_list list );
void _SignalsPrint( char* string, EventCategory category, EventID ID, char* data );
void _SignalsProcess( EventCategory category, EventID ID, char* data );


/*****************************************************************************/


/* Handling logging to the OMNeT++ eventFile format *
static void _LogToStats( uint32_t index ) {
	SchedSchedulable* schedulable = NULL;
	SchedInstance *schedInstance = NULL;
	pfloat_t hpStart, offset;
	// Switch on the event category
	switch ( _logEntries[index].category ) {
		// Handle scheduler events
		case Scheduler:
			switch ( _logEntries[index].type ) {
				case HyperperiodStart:
					// Capture the start of the hyperperiod
					hpStart = _logEntries[index].absTimeMS; break;
				case TaskBegin:
					// Record offset at the beginning of the task
					schedulable = (SchedSchedulable*)_logEntries[index].object;
					assert( schedulable != NULL );
					schedInstance = schedulable->execInst;
					offset = fabs( schedInstance->timevalMS - schedulable->execInit );
					// Accrue the offset
					_logStats.taskBeginSum += offset;
					_logStats.taskBeginCount++;
					// Do we have a min or max
					if ( offset > _logStats.taskBeginMax ) _logStats.taskBeginMax = offset;
					if ( offset < _logStats.taskBeginMin ) _logStats.taskBeginMin = offset;
					break;
				case TaskEnd:
					// Record offset at the end of the task
					schedulable = (SchedSchedulable*)_logEntries[index].object;
					assert( schedulable != NULL );
					schedInstance = schedulable->execInst;
					offset = fabs( schedInstance->timevalMS + schedulable->wcetMS - schedulable->execHalt );
					// Accrue the offset
					_logStats.taskEndSum += offset;
					_logStats.taskEndCount++;
					// Do we have a min or max
					if ( offset > _logStats.taskEndMax ) _logStats.taskEndMax = offset;
					if ( offset < _logStats.taskEndMin ) _logStats.taskEndMin = offset;
					break;
			}
		break;
		// Handle error events for now
		case Error:
			printf( "%4.3f: %s", _logEntries[index].absTimeMS, _logEntries[index].text );
			break;
	}
}


/* Handling logging out to the STDOUT console
static void _LogToConsole( uint32_t index ) {
	switch ( _logEntries[index].category ) {
		case Peripheral:
			switch ( _logEntries[index].type ) {
				case SyncSendBegin:
					syncExpect = (SyncExpectation*)_logEntries[index].object;
					assert( syncExpect != NULL );
					printf( "%4.3f:\t\tSync Send Begin for Msg %d.\n", _logEntries[index].absTimeMS, syncExpect->msgID );
					break;
				case SyncSendEnd:
					syncExpect = (SyncExpectation*)_logEntries[index].object;
					assert( syncExpect != NULL );
					printf( "%4.3f:\t\tSync Send End for Msg %d.\n", _logEntries[index].absTimeMS, syncExpect->msgID );
					break;
				case SyncReceiveBegin:
					syncExpect = (SyncExpectation*)_logEntries[index].object;
					assert( syncExpect != NULL );
					printf( "%4.3f:\t\tSync Receive Begin for Msg %d.\n", _logEntries[index].absTimeMS, syncExpect->msgID );
					break;
				case SyncReceiveEnd:
					syncExpect = (SyncExpectation*)_logEntries[index].object;
					assert( syncExpect != NULL );
					printf( "%4.3f:\t\tSync Receive End for Msg %d.\n", _logEntries[index].absTimeMS, syncExpect->msgID );
					break;
				case AsyncSendBegin:
					asyncExpect = (AsyncExpectation*)_logEntries[index].object;
					assert( asyncExpect != NULL );
					printf( "%4.3f:\t\tAsync Send Begin for Msg %d.\n", _logEntries[index].absTimeMS, asyncExpect->msgID );
					break;
				case AsyncSendEnd:
					asyncExpect = (AsyncExpectation*)_logEntries[index].object;
					assert( asyncExpect != NULL );
					printf( "%4.3f:\t\tAsync Send End for Msg %d.\n", _logEntries[index].absTimeMS, asyncExpect->msgID );
					break;
				case ReceiveBegin:
					udpChannel = (UDP_Channel*)_logEntries[index].object;
					assert( udpChannel != NULL );
					printf( "%4.3f:\t\tReceive Begin on Channel:%d.\n", _logEntries[index].absTimeMS, udpChannel->receivePort );
					break;
				case ReceiveEnd:
					udpChannel = (UDP_Channel*)_logEntries[index].object;
					assert( udpChannel != NULL );
					printf( "%4.3f:\t\tReceive End on Channel:%d.\n", _logEntries[index].absTimeMS, udpChannel->receivePort );
					break;
				case AsyncReceive:
					udpChannel = (UDP_Channel*)_logEntries[index].object;
					assert( udpChannel != NULL );
					printf( "%4.3f:\t\tAsync Receive on Channel:%d.\n", _logEntries[index].absTimeMS, udpChannel->receivePort );
					break;
			}
			break;
}


/*****************************************************************************/


/* Initialize the logging subsystem */
void LogInitialize( void ) {
	// See if already initialized
	if ( _logInit ) return;
	// Register all of the logging and signal events
	SysEventRegisterCategory( "Message", Message, 1 );
	SysEventRegisterCategory( "Signals", Signals, 1 );
	SysEventRegister( Message, 0, _LogMessageSerialize, _LogMessagePrint, NULL );
	SysEventRegister( Signals, 0, _SignalsSerialize, _SignalsPrint, NULL );
	// Mark as initialized
	_logInit = true;
}


/* Write a message to the events subsystem */
void LogMessage( char* message ) {
	// Just pass this through to the SysEvents system
	SysEvent( Message, 0, message );
}


/*****************************************************************************/


/* Serialize an LogMessage event */
uint8_t _LogMessageSerialize( EventCategory category, EventID ID, char* buffer, va_list list ) {
	// Just looking for the absolute clock time
	pfloat_t timeMS = AbsoluteTimeMS();
	// Get the message from the va_list
	char* message = va_arg( list, char* );
	// See how big the message is (up to TEXTLENGTH)
	uint16_t messageSize = min( strlen( message ) + 1, LOGGER_TEXTLENGTH );
	// Copy the absolute time into the buffer
	memcpy( buffer, &timeMS, sizeof(pfloat_t) );
	buffer += sizeof(pfloat_t);
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
	return sizeof(pfloat_t) * 2 + messageSize + sizeof(uint16_t);
}


/* Print an LogMessage event */
void _LogMessagePrint( char* string, EventCategory category, EventID ID, char* data ) {
	pfloat_t absoluteTimeMS, intervalTimeMS;
	uint16_t messageSize;
	char message[LOGGER_TEXTLENGTH];
	// Retrieve the values from the buffer
	memcpy( &absoluteTimeMS, data, sizeof(pfloat_t) );
	data += sizeof(pfloat_t);
	memcpy( &messageSize, data, sizeof(uint16_t) );
	data += sizeof(uint16_t);
	// Copy the message out
	strncpy( message, data, messageSize );
	data += messageSize;
	// Retrieve the interval time
	memcpy( &intervalTimeMS, data, sizeof(pfloat_t) );
	// Print to the string
	sprintf( string, "%4.3f: (@%4.3f) %s", absoluteTimeMS, intervalTimeMS, message );
}


/* Process an LogMessage event */
void _LogMessageProcess( EventCategory category, EventID ID, char* data ) {
}


/*****************************************************************************/


/* Serialize an Signals event */
uint8_t _SignalsSerialize( EventCategory category, EventID ID, char* buffer, va_list list ) {
	// Just looking for the absolute clock time
	pfloat_t timeMS = AbsoluteTimeMS();
	// Get the signal ID from the va_list
	int signalID = va_arg( list, int );
	// Copy the absolute time into the buffer
	memcpy( buffer, &timeMS, sizeof(pfloat_t) );
	buffer += sizeof(pfloat_t);
	// Copy the signal ID into the buffer
	memcpy( buffer, &signalID, sizeof(int) );
	buffer += sizeof(int);
	// Get the interval time
	timeMS = GetTimeMS();
	memcpy( buffer, &timeMS, sizeof(pfloat_t) );
	// Return the number of bytes written into the buffer
	return sizeof(pfloat_t) * 2 + sizeof(int);
}


/* Print an Signals event */
void _SignalsPrint( char* string, EventCategory category, EventID ID, char* data ) {
	pfloat_t absoluteTimeMS, intervalTimeMS;
	int signalID;
	// Retrieve the values from the buffer
	memcpy( &absoluteTimeMS, data, sizeof(pfloat_t) );
	data += sizeof(pfloat_t);
	memcpy( &signalID, data, sizeof(int) );
	data += sizeof(int);
	memcpy( &intervalTimeMS, data, sizeof(pfloat_t) );
	// Print to the string
	sprintf( string, "%4.3f:\t(@%4.3f) Signal SIGINT Received.\n", absoluteTimeMS, intervalTimeMS );
}


/* Process an Signals event */
void _SignalsProcess( EventCategory category, EventID ID, char* data ) {
}


/*****************************************************************************/

