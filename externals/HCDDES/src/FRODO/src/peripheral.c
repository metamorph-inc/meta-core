/*** Included Header Files ***/
#include "peripheral.h"
#include "events.h"
#include "highres_timing.h"


/*****************************************************************************/


/*** Static Variables ***/
static bool						_periInit = false;							// Flag for peripherals initialization


/*****************************************************************************/


/* AsyncReceive Handling Functions */
uint8_t _AsyncReceiveSerialize( EventCategory category, EventID ID, char* buffer, va_list list );
void _AsyncReceivePrint( char* string, EventCategory category, EventID ID, char* data );
void _AsyncReceiveProcess( EventCategory category, EventID ID, char* data );
/* SyncSendBegin Handling Functions */
uint8_t _SyncSendBeginSerialize( EventCategory category, EventID ID, char* buffer, va_list list );
void _SyncSendBeginPrint( char* string, EventCategory category, EventID ID, char* data );
void _SyncSendBeginProcess( EventCategory category, EventID ID, char* data );
/* SyncSendEnd Handling Functions */
uint8_t _SyncSendEndSerialize( EventCategory category, EventID ID, char* buffer, va_list list );
void _SyncSendEndPrint( char* string, EventCategory category, EventID ID, char* data );
void _SyncSendEndProcess( EventCategory category, EventID ID, char* data );
/* SyncReceiveBegin Handling Functions */
uint8_t _SyncReceiveBeginSerialize( EventCategory category, EventID ID, char* buffer, va_list list );
void _SyncReceiveBeginPrint( char* string, EventCategory category, EventID ID, char* data );
void _SyncReceiveBeginProcess( EventCategory category, EventID ID, char* data );
/* SyncReceiveEnd Handling Functions */
uint8_t _SyncReceiveEndSerialize( EventCategory category, EventID ID, char* buffer, va_list list );
void _SyncReceiveEndPrint( char* string, EventCategory category, EventID ID, char* data );
void _SyncReceiveEndProcess( EventCategory category, EventID ID, char* data );
/* AsyncSendBegin Handling Functions */
uint8_t _AsyncSendBeginSerialize( EventCategory category, EventID ID, char* buffer, va_list list );
void _AsyncSendBeginPrint( char* string, EventCategory category, EventID ID, char* data );
void _AsyncSendBeginProcess( EventCategory category, EventID ID, char* data );
/* AsyncSendEnd Handling Functions */
uint8_t _AsyncSendEndSerialize( EventCategory category, EventID ID, char* buffer, va_list list );
void _AsyncSendEndPrint( char* string, EventCategory category, EventID ID, char* data );
void _AsyncSendEndProcess( EventCategory category, EventID ID, char* data );
/* ReceiveBegin Handling Functions */
uint8_t _ReceiveBeginSerialize( EventCategory category, EventID ID, char* buffer, va_list list );
void _ReceiveBeginPrint( char* string, EventCategory category, EventID ID, char* data );
void _ReceiveBeginProcess( EventCategory category, EventID ID, char* data );
/* ReceiveEnd Handling Functions */
uint8_t _ReceiveEndSerialize( EventCategory category, EventID ID, char* buffer, va_list list );
void _ReceiveEndPrint( char* string, EventCategory category, EventID ID, char* data );
void _ReceiveEndProcess( EventCategory category, EventID ID, char* data );
/* SyncReadySend Handling Functions */
uint8_t _SyncReadySendSerialize( EventCategory category, EventID ID, char* buffer, va_list list );
void _SyncReadySendPrint( char* string, EventCategory category, EventID ID, char* data );
void _SyncReadySendProcess( EventCategory category, EventID ID, char* data );
/* SyncReadyReceive Handling Functions */
uint8_t _SyncReadyReceiveSerialize( EventCategory category, EventID ID, char* buffer, va_list list );
void _SyncReadyReceivePrint( char* string, EventCategory category, EventID ID, char* data );
void _SyncReadyReceiveProcess( EventCategory category, EventID ID, char* data );
/* SyncAckSend Handling Functions */
uint8_t _SyncAckSendSerialize( EventCategory category, EventID ID, char* buffer, va_list list );
void _SyncAckSendPrint( char* string, EventCategory category, EventID ID, char* data );
void _SyncAckSendProcess( EventCategory category, EventID ID, char* data );
/* SyncAckReceive Handling Functions */
uint8_t _SyncAckReceiveSerialize( EventCategory category, EventID ID, char* buffer, va_list list );
void _SyncAckReceivePrint( char* string, EventCategory category, EventID ID, char* data );
void _SyncAckReceiveProcess( EventCategory category, EventID ID, char* data );
/* SyncStartSend Handling Functions */
uint8_t _SyncStartSendSerialize( EventCategory category, EventID ID, char* buffer, va_list list );
void _SyncStartSendPrint( char* string, EventCategory category, EventID ID, char* data );
void _SyncStartSendProcess( EventCategory category, EventID ID, char* data );
/* SyncStartReceive Handling Functions */
uint8_t _SyncStartReceiveSerialize( EventCategory category, EventID ID, char* buffer, va_list list );
void _SyncStartReceivePrint( char* string, EventCategory category, EventID ID, char* data );
void _SyncStartReceiveProcess( EventCategory category, EventID ID, char* data );
/* SyncHaltSend Handling Functions */
uint8_t _SyncHaltSendSerialize( EventCategory category, EventID ID, char* buffer, va_list list );
void _SyncHaltSendPrint( char* string, EventCategory category, EventID ID, char* data );
void _SyncHaltSendProcess( EventCategory category, EventID ID, char* data );
/* SyncHaltReceive Handling Functions */
uint8_t _SyncHaltReceiveSerialize( EventCategory category, EventID ID, char* buffer, va_list list );
void _SyncHaltReceivePrint( char* string, EventCategory category, EventID ID, char* data );
void _SyncHaltReceiveProcess( EventCategory category, EventID ID, char* data );
/* SyncSuggestOffset Handling Functions */
uint8_t _SyncSuggestOffsetSerialize( EventCategory category, EventID ID, char* buffer, va_list list );
void _SyncSuggestOffsetPrint( char* string, EventCategory category, EventID ID, char* data );
void _SyncSuggestOffsetProcess( EventCategory category, EventID ID, char* data );


/*****************************************************************************/


/* Swap the data values within two ExpectInstance - used in sorting algorithm below */
static void _SwapExpectInstances( ExpectInstance* instA, ExpectInstance* instB ) {
	// Store the A values
	SyncExpectation*	expect = instA->expect;
	pfloat_t time = instA->time;
	// Set A values
	instA->expect = instB->expect;
	instA->time = instB->time;
	// Set B values
	instB->expect = expect;
	instB->time = time;
}


/* Initialize the peripheral subsystem */
void PeripheralInitialize( void ) {
	// See about multiple initializations
	if ( _periInit ) return;
	// Register all of the peripheral events
	SysEventRegisterCategory( "Peripheral", Peripheral, 11 );
	SysEventRegister( Peripheral, AsyncReceive, _AsyncReceiveSerialize, _AsyncReceivePrint, NULL );
	SysEventRegister( Peripheral, SyncSendBegin, _SyncSendBeginSerialize, _SyncSendBeginPrint, NULL );
	SysEventRegister( Peripheral, SyncSendEnd, _SyncSendEndSerialize, _SyncSendEndPrint, NULL );
	SysEventRegister( Peripheral, SyncReceiveBegin, _SyncReceiveBeginSerialize, _SyncReceiveBeginPrint, NULL );
	SysEventRegister( Peripheral, SyncSendEnd, _SyncSendEndSerialize, _SyncSendEndPrint, NULL );
	SysEventRegister( Peripheral, SyncReceiveBegin, _SyncReceiveBeginSerialize, _SyncReceiveBeginPrint, NULL );
	SysEventRegister( Peripheral, SyncReceiveEnd, _SyncReceiveEndSerialize, _SyncReceiveEndPrint, NULL );
	SysEventRegister( Peripheral, AsyncSendBegin, _AsyncSendBeginSerialize, _AsyncSendBeginPrint, NULL );
	SysEventRegister( Peripheral, AsyncSendEnd, _AsyncSendEndSerialize, _AsyncSendEndPrint, NULL );
//	SysEventRegister( Peripheral, ReceiveBegin, _ReceiveBeginSerialize, _ReceiveBeginPrint, NULL );
//	SysEventRegister( Peripheral, ReceiveEnd, _ReceiveEndSerialize, _ReceiveEndPrint, NULL );
	SysEventRegisterCategory( "Syncronization", Syncronization, 9 );
	SysEventRegister( Syncronization, SyncReadySend, _SyncReadySendSerialize, _SyncReadySendPrint, NULL );
	SysEventRegister( Syncronization, SyncReadyReceive, _SyncReadyReceiveSerialize, _SyncReadyReceivePrint, NULL );
	SysEventRegister( Syncronization, SyncAckSend, _SyncAckSendSerialize, _SyncAckSendPrint, NULL );
	SysEventRegister( Syncronization, SyncAckReceive, _SyncAckReceiveSerialize, _SyncAckReceivePrint, NULL );
	SysEventRegister( Syncronization, SyncStartSend, _SyncStartSendSerialize, _SyncStartSendPrint, NULL );
	SysEventRegister( Syncronization, SyncStartReceive, _SyncStartReceiveSerialize, _SyncStartReceivePrint, NULL );
	SysEventRegister( Syncronization, SyncHaltSend, _SyncHaltSendSerialize, _SyncHaltSendPrint, NULL );
	SysEventRegister( Syncronization, SyncHaltReceive, _SyncHaltReceiveSerialize, _SyncHaltReceivePrint, NULL );
	SysEventRegister( Syncronization, SyncSuggestOffset, _SyncSuggestOffsetSerialize, _SyncSuggestOffsetPrint, NULL );
	// Mark as initialized
	_periInit = true;
}


/* Sort an array of UDP_ExpectInstances according to their start times */
void SortExpectations( ExpectInstance* expectInstances, uint16_t numExpects ) {
	uint16_t i = 1, j = 2;
	// Case 1: Zero or one expect instances
	if ( numExpects <= 1 ) return;
	// Quick and dirty gnome_sort (see http://en.wikipedia.org/wiki/Gnome_sort)
	while ( i < numExpects ) {
		if ( expectInstances[i-1].time <= expectInstances[i].time ) {
			i = j;
			j++;
		}
		else {
			_SwapExpectInstances( expectInstances + i - 1, expectInstances + i );
			i--;
			if ( i == 0 ) {
				i = j;
				j++;
			}
		}
	}
}


/*****************************************************************************/


/* Serialize an AsyncReceive event */
uint8_t _AsyncReceiveSerialize( EventCategory category, EventID ID, char* buffer, va_list list ) {
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


/* Print an AsyncReceive event */
void _AsyncReceivePrint( char* string, EventCategory category, EventID ID, char* data ) {
	pfloat_t absoluteTimeMS, relativeTimeMS;
	// Retrieve the values from the buffer
	memcpy( &absoluteTimeMS, data, sizeof(pfloat_t) );
	memcpy( &relativeTimeMS, data + sizeof(pfloat_t), sizeof(pfloat_t) );
	// Print to the string
	sprintf( string, "%4.3f:\tAsyncReceive (@%4.3f).\n", absoluteTimeMS, relativeTimeMS );
}


/* Process an AsyncReceive event */
void _AsyncReceiveProcess( EventCategory category, EventID ID, char* data ) {
}


/*****************************************************************************/


/* Serialize an SyncSendBegin event */
uint8_t _SyncSendBeginSerialize( EventCategory category, EventID ID, char* buffer, va_list list ) {
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


/* Print an SyncSendBegin event */
void _SyncSendBeginPrint( char* string, EventCategory category, EventID ID, char* data ) {
	pfloat_t absoluteTimeMS, relativeTimeMS;
	// Retrieve the values from the buffer
	memcpy( &absoluteTimeMS, data, sizeof(pfloat_t) );
	memcpy( &relativeTimeMS, data + sizeof(pfloat_t), sizeof(pfloat_t) );
	// Print to the string
	sprintf( string, "%4.3f:\t\tSyncSendBegin (@%4.3f).\n", absoluteTimeMS, relativeTimeMS );
}


/* Process an SyncSendBegin event */
void _SyncSendBeginProcess( EventCategory category, EventID ID, char* data ) {
}


/*****************************************************************************/


/* Serialize an SyncSendEnd event */
uint8_t _SyncSendEndSerialize( EventCategory category, EventID ID, char* buffer, va_list list ) {
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


/* Print an AsyncReceive event */
void _SyncSendEndPrint( char* string, EventCategory category, EventID ID, char* data ) {
	pfloat_t absoluteTimeMS, relativeTimeMS;
	// Retrieve the values from the buffer
	memcpy( &absoluteTimeMS, data, sizeof(pfloat_t) );
	memcpy( &relativeTimeMS, data + sizeof(pfloat_t), sizeof(pfloat_t) );
	// Print to the string
	sprintf( string, "%4.3f:\t\tSyncSendEnd (@%4.3f).\n", absoluteTimeMS, relativeTimeMS );
}


/* Process an AsyncReceive event */
void _SyncSendEndProcess( EventCategory category, EventID ID, char* data ) {
}


/*****************************************************************************/


/* Serialize an SyncReceiveBegin event */
uint8_t _SyncReceiveBeginSerialize( EventCategory category, EventID ID, char* buffer, va_list list ) {
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


/* Print an SyncReceiveBegin event */
void _SyncReceiveBeginPrint( char* string, EventCategory category, EventID ID, char* data ) {
	pfloat_t absoluteTimeMS, relativeTimeMS;
	// Retrieve the values from the buffer
	memcpy( &absoluteTimeMS, data, sizeof(pfloat_t) );
	memcpy( &relativeTimeMS, data + sizeof(pfloat_t), sizeof(pfloat_t) );
	// Print to the string
	sprintf( string, "%4.3f:\tSyncReceiveBegin (@%4.3f).\n", absoluteTimeMS, relativeTimeMS );
}


/* Process an SyncReceiveBegin event */
void _SyncReceiveBeginProcess( EventCategory category, EventID ID, char* data ) {
}


/*****************************************************************************/


/* Serialize an SyncReceiveEnd event */
uint8_t _SyncReceiveEndSerialize( EventCategory category, EventID ID, char* buffer, va_list list ) {
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


/* Print an SyncReceiveEnd event */
void _SyncReceiveEndPrint( char* string, EventCategory category, EventID ID, char* data ) {
	pfloat_t absoluteTimeMS, relativeTimeMS;
	// Retrieve the values from the buffer
	memcpy( &absoluteTimeMS, data, sizeof(pfloat_t) );
	memcpy( &relativeTimeMS, data + sizeof(pfloat_t), sizeof(pfloat_t) );
	// Print to the string
	sprintf( string, "%4.3f:\tSyncReceiveEnd (@%4.3f).\n", absoluteTimeMS, relativeTimeMS );
}


/* Process an SyncReceiveEnd event */
void _SyncReceiveEndProcess( EventCategory category, EventID ID, char* data ) {
}


/*****************************************************************************/


/* Serialize an AsyncSendBegin event */
uint8_t _AsyncSendBeginSerialize( EventCategory category, EventID ID, char* buffer, va_list list ) {
	// Just looking for the absolute clock time
	pfloat_t timeMS = AbsoluteTimeMS();
	// Get the message ID from the list
	uint16_t msgID = (uint16_t)va_arg( list, uint32_t );
	// Copy the value into the buffer
	memcpy( buffer, &timeMS, sizeof(pfloat_t) );
	buffer += sizeof(pfloat_t);
	// Get the in-hyperperiod time
	timeMS = GetTimeMS();
	// Write the in-hyperperiod time
	memcpy( buffer, &timeMS, sizeof(pfloat_t) );
	buffer += sizeof(pfloat_t);
	// Write out the message ID
	memcpy( buffer, &msgID, sizeof(uint16_t) );
	// Return the number of bytes
	return sizeof(pfloat_t) * 2 + sizeof(uint16_t);
}


/* Print an AsyncSendBegin event */
void _AsyncSendBeginPrint( char* string, EventCategory category, EventID ID, char* data ) {
	pfloat_t absoluteTimeMS, relativeTimeMS;
	uint16_t msgID;
	// Retrieve the values from the buffer
	memcpy( &absoluteTimeMS, data, sizeof(pfloat_t) );
	data += sizeof(pfloat_t);
	memcpy( &relativeTimeMS, data, sizeof(pfloat_t) );
	data += sizeof(pfloat_t);
	memcpy( &msgID, data, sizeof(uint16_t) );
	// Print to the string
	sprintf( string, "%4.3f:\tAsync Send Begin for Msg %d (@%4.3f).\n", absoluteTimeMS, msgID, relativeTimeMS );
}


/* Process an AsyncSendBegin event */
void _AsyncSendBeginProcess( EventCategory category, EventID ID, char* data ) {
}


/*****************************************************************************/


/* Serialize an AsyncSendEnd event */
uint8_t _AsyncSendEndSerialize( EventCategory category, EventID ID, char* buffer, va_list list ) {
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


/* Print an AsyncSendEnd event */
void _AsyncSendEndPrint( char* string, EventCategory category, EventID ID, char* data ) {
	pfloat_t absoluteTimeMS, relativeTimeMS;
	// Retrieve the values from the buffer
	memcpy( &absoluteTimeMS, data, sizeof(pfloat_t) );
	memcpy( &relativeTimeMS, data + sizeof(pfloat_t), sizeof(pfloat_t) );
	// Print to the string
	sprintf( string, "%4.3f:\tAsyncSendEnd (@%4.3f).\n", absoluteTimeMS, relativeTimeMS );
}


/* Process an AsyncSendEnd event */
void _AsyncSendEndProcess( EventCategory category, EventID ID, char* data ) {
}


/*****************************************************************************/


/* Serialize an ReceiveBegin event */
uint8_t _ReceiveBeginSerialize( EventCategory category, EventID ID, char* buffer, va_list list ) {
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


/* Print an ReceiveBegin event */
void _ReceiveBeginPrint( char* string, EventCategory category, EventID ID, char* data ) {
	pfloat_t absoluteTimeMS, relativeTimeMS;
	// Retrieve the values from the buffer
	memcpy( &absoluteTimeMS, data, sizeof(pfloat_t) );
	memcpy( &relativeTimeMS, data + sizeof(pfloat_t), sizeof(pfloat_t) );
	// Print to the string
	sprintf( string, "%4.3f:\tReceiveBegin (@%4.3f).\n", absoluteTimeMS, relativeTimeMS );
}


/* Process an ReceiveBegin event */
void _ReceiveBeginProcess( EventCategory category, EventID ID, char* data ) {
}


/*****************************************************************************/


/* Serialize an ReceiveEnd event */
uint8_t _ReceiveEndSerialize( EventCategory category, EventID ID, char* buffer, va_list list ) {
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


/* Print an ReceiveEnd event */
void _ReceiveEndPrint( char* string, EventCategory category, EventID ID, char* data ) {
	pfloat_t absoluteTimeMS, relativeTimeMS;
	// Retrieve the values from the buffer
	memcpy( &absoluteTimeMS, data, sizeof(pfloat_t) );
	memcpy( &relativeTimeMS, data + sizeof(pfloat_t), sizeof(pfloat_t) );
	// Print to the string
	sprintf( string, "%4.3f:\tReceiveEnd (@%4.3f).\n", absoluteTimeMS, relativeTimeMS );
}


/* Process an ReceiveEnd event */
void _ReceiveEndProcess( EventCategory category, EventID ID, char* data ) {
}


/*****************************************************************************/


/* Serialize an SyncReadySend event */
uint8_t _SyncReadySendSerialize( EventCategory category, EventID ID, char* buffer, va_list list ) {
	// Just looking for the absolute time
	pfloat_t absoluteTimeMS = AbsoluteTimeMS();
	// Get the address of the host ID from the list
	char* hostAddress = va_arg( list, char* );
	assert( strlen( hostAddress ) <= 15 );
	// Copy the value into the buffer
	memcpy( buffer, &absoluteTimeMS, sizeof(pfloat_t) );
	buffer += sizeof(pfloat_t);
	// Copy the host address into the buffer
	memcpy( buffer, hostAddress, 15 );
	// Return the number of bytes written into the buffer
	return sizeof(pfloat_t) + 15;
}


/* Print an SyncReadySend event */
void _SyncReadySendPrint( char* string, EventCategory category, EventID ID, char* data ) {
	pfloat_t absoluteTimeMS;
	char hostAddress[15];
	// Retrieve the value from the buffer
	memcpy( &absoluteTimeMS, data, sizeof(pfloat_t) );
	data += sizeof(pfloat_t);
	// Retrieve the host address from buffer
	memcpy( hostAddress, data, 15 );
	// Print to the string
	sprintf( string, "%4.3f: SyncReady Sent from %s (Waiting for SyncAck).\n", absoluteTimeMS, hostAddress );
}


/* Process an SyncReadySend event */
void _SyncReadySendProcess( EventCategory category, EventID ID, char* data ) {
}


/*****************************************************************************/


/* Serialize an SyncReadyReceive event */
uint8_t _SyncReadyReceiveSerialize( EventCategory category, EventID ID, char* buffer, va_list list ) {
	// Just looking for the absolute time
	pfloat_t absoluteTimeMS = AbsoluteTimeMS();
	// Get the address of the sender from the list
	char* senderAddress = va_arg( list, char* );
	assert( strlen( senderAddress ) <= 15 );
	// Copy the value into the buffer
	memcpy( buffer, &absoluteTimeMS, sizeof(pfloat_t) );
	buffer += sizeof(pfloat_t);
	// Copy the sender address into the buffer
	memcpy( buffer, senderAddress, 15 );
	// Return the number of bytes written into the buffer
	return sizeof(pfloat_t) + 15;
}


/* Print an SyncReadyReceive event */
void _SyncReadyReceivePrint( char* string, EventCategory category, EventID ID, char* data ) {
	pfloat_t absoluteTimeMS;
	char senderAddress[15];
	// Retrieve the value from the buffer
	memcpy( &absoluteTimeMS, data, sizeof(pfloat_t) );
	data += sizeof(pfloat_t);
	// Retrieve the sender address from buffer
	memcpy( senderAddress, data, 15 );
	// Print to the string
	sprintf( string, "%4.3f: SyncReady Received from %s.\n", absoluteTimeMS, senderAddress );
}


/* Process an SyncReadyReceive event */
void _SyncReadyReceiveProcess( EventCategory category, EventID ID, char* data ) {
}


/*****************************************************************************/


/* Serialize an SyncAckSend event */
uint8_t _SyncAckSendSerialize( EventCategory category, EventID ID, char* buffer, va_list list ) {
	// Just looking for the absolute time
	pfloat_t absoluteTimeMS = AbsoluteTimeMS();
	// Get the address of the host from the list
	char* hostAddress = va_arg( list, char* );
	assert( strlen( hostAddress ) <= 15 );
	// Copy the value into the buffer
	memcpy( buffer, &absoluteTimeMS, sizeof(pfloat_t) );
	buffer += sizeof(pfloat_t);
	// Copy the host address into the buffer
	memcpy( buffer, hostAddress, 15 );
	// Return the number of bytes written into the buffer
	return sizeof(pfloat_t) + 15;
}


/* Print an SyncAckSend event */
void _SyncAckSendPrint( char* string, EventCategory category, EventID ID, char* data ) {
	pfloat_t absoluteTimeMS;
	char hostAddress[15];
	// Retrieve the value from the buffer
	memcpy( &absoluteTimeMS, data, sizeof(pfloat_t) );
	data += sizeof(pfloat_t);
	// Retrieve the host address from buffer
	memcpy( hostAddress, data, 15 );	
	// Print to the string
	sprintf( string, "%4.3f: SyncAck Sent to %s.\n", absoluteTimeMS, hostAddress );
}


/* Process an SyncAckSend event */
void _SyncAckSendProcess( EventCategory category, EventID ID, char* data ) {
}


/*****************************************************************************/


/* Serialize an SyncAckReceive event */
uint8_t _SyncAckReceiveSerialize( EventCategory category, EventID ID, char* buffer, va_list list ) {
	// Just looking for the absolute time
	pfloat_t absoluteTimeMS = AbsoluteTimeMS();
	// Get the address of the sender from the list
	char* senderAddress = va_arg( list, char* );
	assert( strlen( senderAddress ) <= 15 );
	// Copy the value into the buffer
	memcpy( buffer, &absoluteTimeMS, sizeof(pfloat_t) );
	buffer += sizeof(pfloat_t);
	// Copy the sender address into the buffer
	memcpy( buffer, senderAddress, 15 );
	// Return the number of bytes written into the buffer
	return sizeof(pfloat_t) + 15;
}


/* Print an SyncAckReceive event */
void _SyncAckReceivePrint( char* string, EventCategory category, EventID ID, char* data ) {
	pfloat_t absoluteTimeMS;
	char senderAddress[15];
	// Retrieve the value from the buffer
	memcpy( &absoluteTimeMS, data, sizeof(pfloat_t) );
	data += sizeof(pfloat_t);
	// Retrieve the sender address from buffer
	memcpy( senderAddress, data, 15 );
	// Print to the string
	sprintf( string, "%4.3f: SyncAck Received from %s (Waiting for SyncStart).\n", absoluteTimeMS, senderAddress );
}


/* Process an SyncAckReceive event */
void _SyncAckReceiveProcess( EventCategory category, EventID ID, char* data ) {
}


/*****************************************************************************/


/* Serialize an SyncStartSend event */
uint8_t _SyncStartSendSerialize( EventCategory category, EventID ID, char* buffer, va_list list ) {
	// Just looking for the absolute time
	pfloat_t absoluteTimeMS = AbsoluteTimeMS();
	// Get the address of the host from the list
	char* hostAddress = va_arg( list, char* );
	assert( strlen( hostAddress ) <= 15 );
	// Copy the value into the buffer
	memcpy( buffer, &absoluteTimeMS, sizeof(pfloat_t) );
	buffer += sizeof(pfloat_t);
	// Copy the host address into the buffer
	memcpy( buffer, hostAddress, 15 );
	// Return the number of bytes written into the buffer
	return sizeof(pfloat_t) + 15;
}


/* Print an SyncStartSend event */
void _SyncStartSendPrint( char* string, EventCategory category, EventID ID, char* data ) {
	pfloat_t absoluteTimeMS;
	char hostAddress[15];
	// Retrieve the value from the buffer
	memcpy( &absoluteTimeMS, data, sizeof(pfloat_t) );
	data += sizeof(pfloat_t);
	// Retrieve the host address from buffer
	memcpy( hostAddress, data, 15 );
	// Print to the string
	sprintf( string, "%4.3f: SyncStart Sent from %s.\n", absoluteTimeMS, hostAddress );
}


/* Process an SyncStartSend event */
void _SyncStartSendProcess( EventCategory category, EventID ID, char* data ) {
}


/*****************************************************************************/


/* Serialize an SyncStartReceive event */
uint8_t _SyncStartReceiveSerialize( EventCategory category, EventID ID, char* buffer, va_list list ) {
	// Just looking for the absolute time
	pfloat_t absoluteTimeMS = AbsoluteTimeMS();
	// Get the address of the sender from the list
	char* senderAddress = va_arg( list, char* );
	assert( strlen( senderAddress ) <= 15 );
	// Copy the value into the buffer
	memcpy( buffer, &absoluteTimeMS, sizeof(pfloat_t) );
	buffer += sizeof(pfloat_t);
	// Copy the sender address into the buffer
	memcpy( buffer, senderAddress, 15 );
	// Return the number of bytes written into the buffer
	return sizeof(pfloat_t) + 15;
}


/* Print an SyncStartReceive event */
void _SyncStartReceivePrint( char* string, EventCategory category, EventID ID, char* data ) {
	pfloat_t absoluteTimeMS;
	char senderAddress[15];
	// Retrieve the value from the buffer
	memcpy( &absoluteTimeMS, data, sizeof(pfloat_t) );
	data += sizeof(pfloat_t);
	// Retrieve the sender address from buffer
	memcpy( senderAddress, data, 15 );
	// Print to the string
	sprintf( string, "%4.3f: SyncStart Received from %s.\n", absoluteTimeMS, senderAddress );
}


/* Process an SyncStartReceive event */
void _SyncStartReceiveProcess( EventCategory category, EventID ID, char* data ) {
}


/*****************************************************************************/


/* Serialize an SyncHaltSend event */
uint8_t _SyncHaltSendSerialize( EventCategory category, EventID ID, char* buffer, va_list list ) {
	// Just looking for the absolute time
	pfloat_t absoluteTimeMS = AbsoluteTimeMS();
	// Get the address of the host from the list
	char* hostAddress = va_arg( list, char* );
	assert( strlen( hostAddress ) <= 15 );
	// Copy the value into the buffer
	memcpy( buffer, &absoluteTimeMS, sizeof(pfloat_t) );
	buffer += sizeof(pfloat_t);
	// Copy the host address into the buffer
	memcpy( buffer, hostAddress, 15 );
	// Return the number of bytes written into the buffer
	return sizeof(pfloat_t) + 15;
}


/* Print an SyncHaltSend event */
void _SyncHaltSendPrint( char* string, EventCategory category, EventID ID, char* data ) {
	pfloat_t absoluteTimeMS;
	char hostAddress[15];
	// Retrieve the value from the buffer
	memcpy( &absoluteTimeMS, data, sizeof(pfloat_t) );
	data += sizeof(pfloat_t);
	// Retrieve the host address from buffer
	memcpy( hostAddress, data, 15 );
	// Print to the string
	sprintf( string, "%4.3f: SyncHalt Sent from %s.\n", absoluteTimeMS, hostAddress );
}


/* Process an SyncHaltSend event */
void _SyncHaltSendProcess( EventCategory category, EventID ID, char* data ) {
}


/*****************************************************************************/


/* Serialize an SyncHaltReceive event */
uint8_t _SyncHaltReceiveSerialize( EventCategory category, EventID ID, char* buffer, va_list list ) {
	// Just looking for the absolute time
	pfloat_t absoluteTimeMS = AbsoluteTimeMS();
	// Get the address of the sender from the list
	char* senderAddress = va_arg( list, char* );
	assert( strlen( senderAddress ) <= 15 );
	// Copy the value into the buffer
	memcpy( buffer, &absoluteTimeMS, sizeof(pfloat_t) );
	buffer += sizeof(pfloat_t);
	// Copy the sender address into the buffer
	memcpy( buffer, senderAddress, 15 );
	// Return the number of bytes written into the buffer
	return sizeof(pfloat_t) + 15;
}


/* Print an SyncHaltReceive event */
void _SyncHaltReceivePrint( char* string, EventCategory category, EventID ID, char* data ) {
	pfloat_t absoluteTimeMS;
	char senderAddress[15];
	// Retrieve the value from the buffer
	memcpy( &absoluteTimeMS, data, sizeof(pfloat_t) );
	data += sizeof(pfloat_t);
	// Retrieve the sender address from buffer
	memcpy( senderAddress, data, 15 );
	// Print to the string
	sprintf( string, "%4.3f: SyncHalt Received from %s.\n", absoluteTimeMS, senderAddress );
}


/* Process an SyncHaltReceive event */
void _SyncHaltReceiveProcess( EventCategory category, EventID ID, char* data ) {
}


/*****************************************************************************/


/* Serialize an SyncSuggestOffset event */
uint8_t _SyncSuggestOffsetSerialize( EventCategory category, EventID ID, char* buffer, va_list list ) {
	// Just looking for the absolute time
	pfloat_t timeMS = AbsoluteTimeMS();
	// Copy the value into the buffer
	memcpy( buffer, &timeMS, sizeof(pfloat_t) );
	buffer += sizeof(pfloat_t);
	// Get the offset value from the list
	timeMS = va_arg( list, pfloat_t );
	// Copy the value into the buffer
	memcpy( buffer, &timeMS, sizeof(pfloat_t) );
	// Return the number of bytes written into the buffer
	return sizeof(pfloat_t) * 2;
}


/* Print an SyncSuggestOffset event */
void _SyncSuggestOffsetPrint( char* string, EventCategory category, EventID ID, char* data ) {
	pfloat_t absoluteTimeMS, offsetMS;
	// Retrieve the value from the buffer
	memcpy( &absoluteTimeMS, data, sizeof(pfloat_t) );
	data += sizeof(pfloat_t);
	memcpy( &offsetMS, data, sizeof(pfloat_t) );
	// Print to the string
	sprintf( string, "%4.3f:\t\tSyncSuggestOffset: %4.3f.\n", absoluteTimeMS, offsetMS );
}


/* Process an SyncSuggestOffset event */
void _SyncSuggestOffsetProcess( EventCategory category, EventID ID, char* data ) {
}


/*****************************************************************************/


