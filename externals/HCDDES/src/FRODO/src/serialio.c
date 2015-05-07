/*** Copyright Vanderbilt ***/


/*** Included Header Files ***/
#include "serialio.h"
#include "scheduler.h"
#include "arch/highres_timing.h"
#include "logger.h"

#include <termios.h>
#include <stdio.h>
#include <unistd.h>
//#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>

/*****************************************************************************/

/*** Configuration Constants ***/
#ifndef SERIAL_MAXCHANNELS
#define SERIAL_MAXCHANNELS            8    //
#endif
#ifndef SERIAL_MAXSYNCEXPECTATIONS
#define SERIAL_MAXSYNCEXPECTATIONS    8    //
#endif
#ifndef SERIAL_MAXASYNCEXPECTATIONS
#define SERIAL_MAXASYNCEXPECTATIONS   8    //
#endif
#ifndef SERIAL_MAXSYNCTIMES
#define SERIAL_MAXSYNCTIMES           8    //
#endif
#ifndef SERIAL_LOCALADDRESS
#define SERIAL_LOCALADDRESS           !!!Must define local host!!!   //
#endif
#ifndef SERIAL_REMOTEADDRESS
#define SERIAL_REMOTEADDRESS          !!!Must define remote address!! //
#endif
#ifndef SERIAL_MAXPEERS
#define SERIAL_MAXPEERS              8    //
#endif
#ifndef SERIAL_SYNCWAIT
#define SERIAL_SYNCWAIT         1000.0    //
#endif
#ifndef SERIAL_SYNCPOLLWAIT
#define SERIAL_SYNCPOLLWAIT        100    //
#endif
#ifndef SERIAL_ASYNCPRIORITY
#define SERIAL_ASYNCPRIORITY       128    //
#endif
#ifndef SERIAL_NAMELENGTH
#define SERIAL_NAMELENGTH           32    //
#endif
#ifndef SERIAL_RECEIVEFACTOR
#define SERIAL_RECEIVEFACTOR      0.95    //
#endif

/*****************************************************************************/

/*** Static Variables ***/
void (*_serialErrorHandler)(FE_ErrorCode,void*,pfloat_t) = NULL;				// Function pointer for serial error handling
static bool            _serialInit = false;  // Flag for serial initialization
static Serial_Channel  _serialChannels[SERIAL_MAXCHANNELS];  //
static uint8_t         _serialNumChannels = 0;               //
static uint32_t        _serialMaxFD = 0;                     // for select
static Serial_Channel* _newChannel = NULL;                   //
static FP_SyncExpectation*  _newSyncExpect = NULL;           //
static FP_AsyncExpectation* _newAsyncExpect = NULL;          //
static Semaphore*           _serialInitSemaphore;            //
static bool _serialReceivedHalt = false;
  // Are we stopping because we received a SyncHalt?

/*** Externally Defined Variables and Functions ***/
extern pfloat_t            _hyperperiodMS;    // Global hyperperiod in MS
extern void _SchedAddSyncChannel( void*, pfloat_t(*InitFunc)(void*), 
                                  void(*ShutdownFunc)(void*) );
extern bool _SerialCreateConnections( Serial_Channel* );   //
extern int  _SerialGetError( void );                       //
extern bool _schedExec;                    // Is the scheduler executing
extern int  _CloseConnection( Serial_Channel* ); // Close the connection
extern void (*_SchedWaitForSync)( void );  // Scheduler syncronization function

/*****************************************************************************/


/* Swap the data values within two FP_ExpectInstance - used in sorting algorithm below */
static void _SwapExpectInstances( FP_ExpectInstance* instA, FP_ExpectInstance* instB ) {
	// Store the A values
	FP_SyncExpectation*	expect = instA->expect;
	pfloat_t time = instA->time;
	// Set A values
	instA->expect = instB->expect;
	instA->time = instB->time;
	// Set B values
	instB->expect = expect;
	instB->time = time;
}

/* Sort an array of Serial_ExpectInstances according to their start times */
static void _SerialSortExpectations( FP_ExpectInstance* expectInstances, 
                                     uint16_t numExpects ) {
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

/* Function to make sure that there are no pending messages in the receive buffer */
static uint16_t _SerialClearBuffer( Serial_Channel* channel ) {
	size_t retVal;
	char buffer[ FP_MAXMESSAGESIZE + sizeof(uint16_t) ];
	struct timeval waitTime = { 0, SERIAL_SYNCPOLLWAIT };
	bool bufferEmpty = true;
	fd_set rset;
	uint16_t pruneCount = 0;
	//FD_ZERO( &rset );
	// Keep trying to receive until buffer is empty
	do {
		// Always start with the buffer being empty
		bufferEmpty = true;
		// Try to receive
		FD_SET( channel->filedesc, &rset ); 
		// Wait to try to receive
		retVal = select( channel->filedesc + 1, &rset, NULL, NULL, &waitTime ); 
		// Check for running status
		if ( !_schedExec ) return 0;
		// Did we receive
		if ( FD_ISSET( channel->filedesc, &rset ) ) {
			// Actually receive the message
			retVal = read( channel->filedesc, buffer,
				FP_MAXMESSAGESIZE + sizeof(uint16_t) );

			// Make sure there was not a receiving error
			if ( retVal == -1 ) {
				// Error case
				LogEvent( Error, SERIAL_ERR_READPORTFAIL, 
                                        channel );
				// Set the run flag to false
				_schedExec = false;
				// Exit the function
				return 0;
			}
			// There is something in the buffer
			bufferEmpty = false;
			pruneCount++;
		} 
	// Check if the buffer is empty
	} while ( !bufferEmpty );

        tcflush( channel->filedesc, TCIOFLUSH ); // Make sure
	// Return how many messages were pruned
	return pruneCount;
}

/* Look through existing port to ensure port not already in use */
static bool _CheckPort( uint32_t port ) {
	uint8_t i = 0;
	/* @todo: do we need this for serial io? 
	for ( ; i < _udpNumChannels; i++ ) {
		// Check to see if port == in-use port
		if ( _udpChannels[i].receivePort == port ) return false;
	} */
	// All is good
	return true;
}

/* Primary thread for a specific serial channel */
void* _SerialAsyncReceiveExec( void *chnl ) {
	int retVal = 0, i;
	pfloat_t timeMS = 0.0, arrivalStartMS, arrivalStopMS, deltaMS;
	uint16_t receiveID;
	struct timeval timeout = { 0, 0 };
	fd_set read_set;
	char buffer[ FP_MAXMESSAGESIZE + sizeof(uint16_t) ];
	// Cast to a valid channel object
	Serial_Channel* channel = (Serial_Channel*)chnl;
	// Set up array for ordered SyncExpects
	FP_ExpectInstance orderedExpects[SERIAL_MAXEXPECTINSTANCES];
	uint16_t numOrderedExpects = 0;
	// Check some values
	assert( chnl != NULL );
	assert( channel->semaphore != NULL );
	// Loop through all sync expects ( only actually need the receive expects )
	for ( ; retVal < channel->numSyncExpectations; retVal++ ) {
		// Is this expectation for receiving...
		if ( channel->syncExpectations[retVal].direction == ExpectReceive ) {
			// Loop through all times for this expectation
			for ( i = 0; i < channel->syncExpectations[retVal].numStartTimes; i++ ) {
				// Add this start time and pointer into the ordered array
				orderedExpects[numOrderedExpects].expect = &(channel->syncExpectations[retVal]);
				orderedExpects[numOrderedExpects].time = channel->syncExpectations[retVal].startTimesMS[i];
				// Increment the number or ordered expects
				numOrderedExpects++;
			}
		}
	}
	// Sort the expects to get ordered expects
	_SerialSortExpectations( orderedExpects, numOrderedExpects );
	// Set up socket sets
	FD_ZERO( &read_set );
	// Signal that thread initialization is complete
	_PostSemaphore( _serialInitSemaphore );
	// Wait until the scheduler is running and syncronization is done
	_WaitSemaphore( channel->semaphore );
	// Must check the run flag
	if ( !_schedExec ) { pthread_exit( NULL ) ; return NULL; }
	// Post back to the semaphore so others can access the channel
	_PostSemaphore( channel->semaphore );
	
	/*** Entry primary run loop ***/
	do {
		// Loop until a valid message is found (retVal == 1)
		do {
			// How long should be wait on select
			timeout.tv_usec = (long)( ( _hyperperiodMS - timeMS ) * 990.0 );
			if ( timeout.tv_usec < 1000 ) timeout.tv_usec = 10;
			// Look for a read
			FD_SET( channel->filedesc, &read_set );
			retVal = select( channel->filedesc + 1, &read_set, NULL, NULL, &timeout );
			// Is there an error
			if ( retVal == -1 ) {
				// Log the error
				LogEvent( Error, SERIAL_ERR_SELECTFAIL, channel );
				// Mark as no longer running
				_schedExec = false;
			}
			if ( !_schedExec ) {
				// Exit the thread
				pthread_exit( NULL );
			}
			// Determine how long we can look for an async receive
			timeMS = GetTimeMS();
		} while ( retVal != 1 );

		// Lock the channel semaphore
		_WaitSemaphore( channel->semaphore );
		// Log the start of the receive
		LogEvent( Peripheral, ReceiveBegin, channel );
		// Get the time of receive completion
		arrivalStartMS = GetTimeMS();
		// Read the data from the socket
		retVal = read( channel->filedesc, buffer, 
			FP_MAXMESSAGESIZE + sizeof(uint16_t) );
		// Get the time of receive completion
		arrivalStopMS = GetTimeMS();
		// Check for _schedExec
		if ( !_schedExec || retVal == -1 ){
			// Unlock the channel semaphore
			_PostSemaphore( channel->semaphore );
			// Exit the thread
			pthread_exit( NULL );
		}
		// Log the end of the receive
		LogEvent( Peripheral, ReceiveEnd, channel );
		// Get the message ID
		memcpy( &receiveID, buffer, sizeof(uint16_t) );
		// Is this a halt message
		if ( receiveID == SyncHalt ) {
			// Set _schedExec to false
			_schedExec = false;
			_serialReceivedHalt = true;
			// Unlock the channel semaphore
			_PostSemaphore( channel->semaphore );
			pthread_exit( NULL );
		}

		// Is channel->activeExpect set?
		if ( channel->activeExpect != NULL ) {
			// Validate message ID and size
			if ( receiveID != channel->activeExpect->msgID ) {
				// Signal that an error has occured
				_serialErrorHandler( FP_ERR_UNEXPECTEDMSGIDRECV, channel, arrivalStopMS );
			}
			else if ( retVal - sizeof(receiveID) != channel->activeExpect->sizeBytes ) {
				// Signal that an error has occured
				_serialErrorHandler( FP_ERR_WRONGMSGSIZE, channel, arrivalStopMS );
			}
			// Otherwise, we are good to write the message to the port
			else {
				// Write the message to the port
				ReturnCode_t returnCode;
				WriteSamplingMessage( channel->activeExpect->portID, buffer + sizeof(receiveID), channel->activeExpect->sizeBytes, &returnCode );
				// Lock the expect semaphore
//				_WaitSemaphore( channel->activeExpect->semaphore );
				// Copy message into buffer
//				memcpy( channel->activeExpect->buffer, buffer + sizeof(receiveID), channel->activeExpect->sizeBytes );
				// Set the receive flag
				channel->activeReceive = true;
				// Unlock the expect semaphore
//				_PostSemaphore( channel->activeExpect->semaphore );
				// Determine deltaMS to the middle ( see fraction ) of the receive window
				deltaMS = channel->activeExpect->task->execInit + ( channel->activeExpect->wcetMS - arrivalStopMS - arrivalStartMS ) * 0.5;
				// Suggest the desired clock offset and log it
				sprintf( buffer, "%4.3f", deltaMS );
				LogEvent( Syncronization, SyncSuggestOffset, buffer);
				SuggestOffset( deltaMS );
			}
		}
		// Otherwise, are we just too early or late ( watch out for zero case )
		else if ( numOrderedExpects != 0 ) {
			FP_ExpectInstance *expect = orderedExpects;
			pfloat_t distance = _hyperperiodMS;
			bool decline = false;
			// Find the correct lower and upper bounds expectations
			for ( retVal = 0; retVal < numOrderedExpects; retVal++ ) {
				// Find the correct lower-bounds distance ( make sure to check message ID )
				if ( fabs( orderedExpects[retVal].time - arrivalStopMS ) < distance &&
					 receiveID == orderedExpects[retVal].expect->msgID ) {
					// Set the expect to this one
					expect = orderedExpects + retVal;
					// Record the new closest distance
					distance = fabs( orderedExpects[retVal].time - arrivalStopMS );
					// Mark as still declining
					decline = true;
				}
				// Find the correct upper-bounds distance  make sure to check message ID )
				if ( fabs( arrivalStopMS - orderedExpects[retVal].time - orderedExpects[retVal].expect->wcetMS ) < distance &&
					 receiveID == orderedExpects[retVal].expect->msgID ) {
					// Set the expect to this one
					expect = orderedExpects + retVal;
					// Record the new closest distance
					distance = fabs( arrivalStopMS - orderedExpects[retVal].time - orderedExpects[retVal].expect->wcetMS );
					// Mark as still declining
					decline = true;
				}
				// Have we found the closest point
				if ( !decline ) continue;
				// Nope, not yet
				else decline = false;
			}
			// Set delta for the middle ( see fraction ) of the receive window
			deltaMS = expect->time + ( expect->expect->wcetMS - arrivalStopMS - arrivalStartMS ) * 0.5;
			// Suggest the desired clock offset and log it
			sprintf( buffer, "%4.3f", deltaMS );
			LogEvent( Syncronization, SyncSuggestOffset, buffer);
			SuggestOffset( deltaMS );
		}
		// Must be an async expect
		else {
			// Find in list of async expects
			FP_AsyncExpectation *expect = NULL;
			for ( i = 0; i < channel->numAsyncExpectations; i++ ) {
				// Make sure we expect a receive with this MsgID
				if ( channel->asyncExpectations[i].msgID &&
					 channel->asyncExpectations[i].direction == ExpectReceive ) {
					// Set expect to this one
					expect = channel->asyncExpectations + i;
					continue;
				}
			}
			// Did we find it
			if ( expect != NULL ) {
				// Lock the expect semaphore
//				_WaitSemaphore( expect->semaphore );
				// Copy message into buffer
//				memcpy( channel->activeExpect->buffer, buffer + sizeof(receiveID), channel->activeExpect->sizeBytes );
				// Do we need to trigger a sporadic task
				if ( expect->task != NULL ) FS_CreateJob( expect->task, arrivalStopMS + 10.0 );
				// Unlock the expect semaphore
//				_PostSemaphore( channel->activeExpect->semaphore );
				// Log the message receipt
				LogEvent( Peripheral, AsyncReceive, channel );
			}
			// Otherwise, this is a truely unexpected message
			else {
				// Signal that an error has occured
				_serialErrorHandler( FP_ERR_UNEXPECTEDMSGIDRECV, channel, arrivalStopMS );

			}
		}
		// Unlock the channel semaphore
		_PostSemaphore( channel->semaphore );
	// Continue until run-flag is off
	} while ( true );
	// Never going to get here
	return NULL;
}

/* Thread for asyncronous send expectations of the UDP channel */
static void* _SerialAsyncSend( void *cntx ) {
	ReturnCode_t returnCode;
	MessageSize_t size;
	// For this task, cntx is meaningless, instead look to the global variable
	Serial_Channel * channel = (Serial_Channel * ) _newChannel;
	FP_AsyncExpectation* expect = _newAsyncExpect;
	FS_Schedulable* schedulable = expect->task;
	ssize_t	sendSize = 0;
	size_t messageSize = expect->sizeBytes + sizeof(uint16_t);
	// @todo: struct sockaddr_in servaddr;
	char buffer[ FP_MAXMESSAGESIZE + sizeof(uint16_t) ];
	memcpy( buffer, &(expect->msgID), sizeof(uint16_t) );
	/* @todo: Set up the receiver's address information
	bzero( &servaddr, sizeof(servaddr) );
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons( channel->receivePort );
	inet_pton( AF_INET, channel->remoteAddress, &servaddr.sin_addr ); */
	// Signal that thread initialization is complete
	_PostSemaphore( _serialInitSemaphore );

	// Go into the sending infinite loop? - only the scheduler can force exit
	while ( true ) {
		// Wait to execute
		FS_SignalExecution( schedulable );
		// Lock channel semaphore
		_WaitSemaphore( channel->semaphore );
		// Read from the port and place into channel outbound buffer
		ReadSamplingMessage( expect->portID, buffer + sizeof(uint16_t), &size, NULL, &returnCode );
		// Log the start of sending
		LogEvent( Peripheral, AsyncSendBegin, expect );
		// Try to send the message
		// @todo sendSize = sendto( channel->sendSocket, buffer, messageSize,
		//				  0, (struct sockaddr*)(&servaddr), sizeof(servaddr) );
		// Log the end of sending
		LogEvent( Peripheral, AsyncSendEnd, expect );
		// Make sure there was not a sending error
		if ( sendSize == -1 ) {
			// Error case
			_serialErrorHandler( SERIAL_ERR_SENDTOFAIL, channel, GetTimeMS() );
		}
		// Check for completeness (right # of bytes sent)
		if ( sendSize != messageSize ) {
			// Manage the error
			_serialErrorHandler( FP_ERR_WRONGMSGSIZE, channel, GetTimeMS() );
		}
		// Unlock channel semaphore
		_PostSemaphore( channel->semaphore );
		// Signal that we are done with this execution
		FS_SignalCompletion( schedulable );
	}
	// Must return something - never will get here
	return NULL;
}

/* Function to handle syncronous receive */
static void* _SerialSyncReceive( Serial_Channel* channel, 
                                 FP_SyncExpectation* expect ) {
	FS_Schedulable* schedulable = expect->task;
	pfloat_t deadlineMS;
	// Enter endless run-loop
	while ( true ) {
		// Wait to execute
		FS_SignalExecution( schedulable );
		// Lock channel semaphore
		_WaitSemaphore( channel->semaphore );
		// Set sync expect
		channel->activeExpect = expect;
		// Clear the receive flag
		channel->activeReceive = false;
		// Unlock channel semaphore
		_PostSemaphore( channel->semaphore );
		// Sleep until close to the end of WCET
		deadlineMS = schedulable->execInit 
                     + ( schedulable->wcetMS * SERIAL_RECEIVEFACTOR );
		NanoSleep( deadlineMS, SCHEDULER_RESOLUTION_MS );
		// Lock the channel semaphore
		_WaitSemaphore( channel->semaphore );
		// Clear the sync expect
		channel->activeExpect = NULL;
		// Check if message was not received
		if ( !channel->activeReceive ) {
			// Handle that a message was not received in time
			_serialErrorHandler( FP_ERR_FAILURETORECEIVE, channel, deadlineMS );
		}
		// Unlock channel semaphore
		_PostSemaphore( channel->semaphore );
		// Signal that we are done with this execution
		FS_SignalCompletion( schedulable );
	}
	// Will never get here, but have to return something
	return NULL;
}

/* Function to handle syncronous sending */
static void* _SerialSyncSend( Serial_Channel* channel, 
                              FP_SyncExpectation* expect ) {
	ReturnCode_t returnCode;
	MessageSize_t size;
	FS_Schedulable* schedulable = expect->task;
	ssize_t	sendSize = 0;
	size_t messageSize = expect->sizeBytes + sizeof(uint16_t);
	// @todo: struct sockaddr_in servaddr;
	char buffer[ FP_MAXMESSAGESIZE + sizeof(uint16_t) ];
	memcpy( buffer, &(expect->msgID), sizeof(uint16_t) );
	/* @todo: Set up the receiver's address information
	bzero( &servaddr, sizeof(servaddr) );
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons( channel->receivePort );
	inet_pton( AF_INET, channel->remoteAddress, &servaddr.sin_addr ); */
	while ( true ) {
		// Wait to execute
		FS_SignalExecution( schedulable );
		// Read from the port and place into channel outbound buffer
		ReadSamplingMessage( expect->portID, buffer + sizeof(uint16_t), &size, NULL, &returnCode );
		// Make sure read succeded
		if ( returnCode != NoError ) {
			// Handle that a message was not read from the port
			_serialErrorHandler( SERIAL_ERR_READPORTFAIL, channel, GetTimeMS() );
		}
		// Log the start of sending
		LogEvent( Peripheral, SyncSendBegin, expect );
		// Try to send the message
		sendSize = sendto( channel->sendSocket, buffer, messageSize,
						  0, (struct sockaddr*)(&servaddr), sizeof(servaddr) );
		// Log the end of sending
		LogEvent( Peripheral, SyncSendEnd, expect );
		// Make sure there was not a sending error
		if ( sendSize == -1 ) {
			// Handle that a message was not received in time
			_serialErrorHandler( SERIAL_ERR_SENDTOFAIL, channel, GetTimeMS() );
		}
		// Check for completeness (right # of bytes sent)
		if ( sendSize != messageSize ) {
			// Manage the error
			_serialErrorHandler( FP_ERR_WRONGMSGSIZE, channel, GetTimeMS() );
		}
		// Signal that we are done with this execution
		FS_SignalCompletion( schedulable );
	}
	// Will never get here, but have to return something
	return NULL;
}

/* Thread for syncronous expectations of the serial channel */
static void* _SerialSyncExec( void *cntx ) {
	// For this task, cntx is meaningless, instead look to the global variable
	Serial_Channel * channel = (Serial_Channel *) _newChannel;
	FP_SyncExpectation* expect = _newSyncExpect;
	// Signal that thread initialization is complete
	_PostSemaphore( _udpInitSemaphore );
	// Go into the receving infinite loop? - only the scheduler can force exit
	if ( expect->direction == ExpectReceive ) 
                _SerialSyncReceive( channel, expect );
	// Otherwise, we are in the sending loop
	else _SerialSyncSend( channel, expect );
	// Must return something - never will get here
	return NULL;
}

/* Master role in initialization syncronization */
pfloat_t _SerialSyncInitMaster( void* chnl ) {
	// Cast to a UDP_Channel
	Serial_Channel * channel = (Serial_Channel *)chnl;
	// Set up many other variables
	char buffer[ FP_MAXMESSAGESIZE + sizeof(uint16_t) ];
	size_t retVal;
	struct timeval waitTime = { 0, SERIAL_SYNCPOLLWAIT };
	uint16_t msgID, pruneCount;
	fd_set rset;
	bool syncStart = false;
	bool newSlave;
	pfloat_t startMS = AbsoluteTimeMS();
	// @todo: struct sockaddr_in cliaddr;
	// @todo: socklen_t len = sizeof( cliaddr );
	char slaves[SERIAL_MAXPEERS][15];
	uint8_t numSlaves = 0;
	uint8_t i;
	// Prepare the sending port
	/* @todo: struct sockaddr_in servaddr;
	bzero( &servaddr, sizeof(servaddr) );
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons( channel->receivePort );
	inet_pton( AF_INET, channel->remoteAddress, &servaddr.sin_addr );
	// Prepare the fd set
	FD_ZERO( &rset ); */
	// Make sure we have a channel
	assert( chnl != NULL );
	assert( channel->syncChannel );

	// Clear the receive socket buffer
	pruneCount = _SerialClearBuffer( channel );
	// Start the synchronization
	sprintf( buffer, "Synchronizing Channel as Master (waiting for SyncReady:%d).\n", pruneCount );
	LogMessage( buffer );
	// Check to see if this is only node
	if ( channel->numSyncPeers == 1 ) {
		// Start the asyncReceive thread running
		_PostSemaphore( channel->semaphore );
		// Log the sending of SyncStart
		LogEvent( Syncronization, SyncStartSend, channel );
		// Return 0.0 - since we are master there is no clock adjustment
		return 0.0;	
	}
	// Loop until all SyncReady message have been received
	do {
		// Try to receive for a bit...
		// @todo: FD_SET( channel->receiveSocket, &rset );
		// Wait to try to receive
		// @todo: retVal = select( channel->receiveSocket + 1, &rset, NULL, NULL, &waitTime );
		// Check for running status
		if ( !_schedExec ) return 0.0;
		// Did we receive
		/* @todo: fix this up
                if ( FD_ISSET( channel->receiveSocket, &rset ) ) {
			// Actually receive the message
			retVal = recvfrom( channel->receiveSocket, buffer, FP_MAXMESSAGESIZE + sizeof(uint16_t),
								0, (struct sockaddr*)&cliaddr, &len );
			// Make sure there was not a receiving error
			if ( retVal == -1 ) {
				// Manage the error
				_udpErrorHandler( FP_ERR_SYNCFAIL, channel, GetTimeMS() );
				// Exit the function
				return 0.0;
			}
			// Get the message ID
			memcpy( &msgID, buffer, sizeof(msgID) );
			// Is this a SyncReady message
			if ( msgID == SyncReady ) {
				// Log the receiving of SyncReady
				LogEvent( Syncronization, SyncReadyReceive, inet_ntoa( cliaddr.sin_addr ) );
				// Is this a new SyncReady or one we already know about
				newSlave = true;
				for ( i = 0; i < numSlaves; i++ ) {
					// Compare the IP addresses
					retVal = strncmp( slaves[i], inet_ntoa( cliaddr.sin_addr ), 15 );
					// If no difference, this slave has been seen before
					if ( retVal == 0 ) newSlave = false;
				}
				// If slave is new - i.e. first SyncReady message
				if ( newSlave ) {
					// Add to the list ( and increment numSlaves )
					strncpy( slaves[numSlaves++], inet_ntoa( cliaddr.sin_addr ), 15 );
					// Send back SyncAck message
					msgID = SyncAck;
					memcpy( buffer, &msgID, sizeof(msgID) );
					// Add the channel sequence number this is in response to
					memcpy( buffer + sizeof(msgID), &numSlaves, sizeof(numSlaves) );
					// Add in the destination to disambiguate the ACK
					memcpy( buffer + sizeof(msgID) + sizeof(numSlaves), inet_ntoa( cliaddr.sin_addr ), 15 );
					// Send out the SyncReady message
					retVal = sendto( channel->sendSocket, buffer, sizeof(msgID) + sizeof(numSlaves) + 15,
									0, (struct sockaddr*)(&servaddr), sizeof(servaddr) );
					// Make sure there was not a sending error
					if ( retVal == -1 ) {
						// Manage the error
						_udpErrorHandler( UDP_ERR_SENDTOFAIL, channel, GetTimeMS() );
						// Set the run flag to false
						_schedExec = false;
						// Exit the function
						return 0.0;
					}
					// Log the sending of SyncAck
					LogEvent( Syncronization, SyncAckSend, inet_ntoa( cliaddr.sin_addr ) );
				}
				// Are all slaves present ( -1 because self does not count )
				if ( numSlaves == channel->numSyncPeers - 1 ) syncStart = true;
			}
			// Error case - nobody should be sending anything but SyncReady 
			else {
				// For now, just do some debugging
				sprintf( buffer, "Should not have received a message with type: %d during sync as master.\n", msgID );
				LogEvent( Error, FP_ERR_UNEXPECTEDMSGIDRECV, buffer );
		 	}
		} */ 
	// Continue until syncStart is ready to be sent
	} while ( !syncStart );

	// Wait for a short bit (to clear the lines)
	BusyWait( SERIAL_SYNCWAIT );
	// Clear the receive socket buffer
	pruneCount = _SerialClearBuffer( channel );
	// Send out SyncStart message
	msgID = SyncStart;
	memcpy( buffer, &msgID, sizeof(msgID) );
	// Send out the SyncReady message
	retVal = sendto( channel->sendSocket, buffer, sizeof(msgID), 0, (struct sockaddr*)(&servaddr), sizeof(servaddr) );
	// Make sure there was not a sending error
	if ( retVal == -1 ) {
		// Manage the error
		_serialErrorHandler( SERIAL_ERR_SENDTOFAIL, channel, GetTimeMS() );
		// Set the run flag to false
		_schedExec = false;
		// Exit the function
		return 0.0;
	}
	// Absorb the local broadcast of the SyncStart
	recvfrom( channel->receiveSocket, buffer, FP_MAXMESSAGESIZE + sizeof(uint16_t), 0, NULL, NULL );
	// Start the asyncReceive thread running
	_PostSemaphore( channel->semaphore );
	// Log the sending of SyncStart
	LogEvent( Syncronization, SyncStartSend, channel );
	// Log how much time sync took
	sprintf( buffer, "Syncronized Channel in %4.3fms (%d).\n", AbsoluteTimeMS() - startMS, pruneCount );
	LogMessage( buffer );
	// Return 0.0 - since we are master there is no clock adjustment
	return 0.0;
}

/* Handle the initial synchronization in the Slave role */
pfloat_t _SerialSyncInitSlave( void* chnl ) {
	// Cast to a Serial_Channel
	Serial_Channel* channel = (Serial_Channel*)chnl;
	// Set up lots of other local variables
	uint16_t msgID;
	char buffer[ FP_MAXMESSAGESIZE + sizeof(uint16_t) ];
	bool syncStartReceived = false, syncAckReceived = false;
	size_t retVal;
	pfloat_t deadlineMS, timeMS, startMS = AbsoluteTimeMS();
	struct timeval waitTime = { 0, SERIAL_SYNCPOLLWAIT };
	/* @todo: struct sockaddr_in cliaddr;
	socklen_t len = sizeof( cliaddr );
	fd_set rset; */
	pfloat_t readyTimeMS, ackTimeMS, deltaMS;
	/* @todo: Prepare the sending port
	struct sockaddr_in servaddr;
	bzero( &servaddr, sizeof(servaddr) );
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons( channel->receivePort );
	inet_pton( AF_INET, channel->remoteAddress, &servaddr.sin_addr );
	// Prepare the fd set
	FD_ZERO( &rset ); */
	// Make sure we have a channel
	assert( chnl != NULL );
	assert( channel->syncChannel );

	// Start the syncronization
	LogMessage( "Synchronizing Channel as Slave.\n" );
	deadlineMS = AbsoluteTimeMS();
	// Loop until SyncAck is received or schedExec is false
	do {
		// Prep the outbound message
		msgID = SyncReady;
		// Set the message type - SyncReady
		memcpy( buffer, &msgID, sizeof(msgID) );
		// Send out the SyncReady message
		retVal = sendto( channel->sendSocket, buffer, sizeof(msgID), 0, (struct sockaddr*)(&servaddr), sizeof(servaddr) );
		// Capture the time sending ended
		readyTimeMS = AbsoluteTimeMS();
		// Check the transmission for errors
		if ( retVal == -1 ) {
			// Manage the error
			_serialErrorHandler( SERIAL_ERR_SENDTOFAIL, channel, GetTimeMS() );
			// Set the run flag to false
			_schedExec = false;
			// Exit the function
			return 0.0;
		}
		// Log the sending of SyncReady
		LogEvent( Syncronization, SyncReadySend, channel );

		// Try for up to 1000ms to receive a SyncAck message - ignore other messages
		deadlineMS += 1000.0;
		do {
			/* @todo: Wait to try to receive
			FD_SET( channel->receiveSocket, &rset );
			retVal = select( channel->receiveSocket + 1, &rset, NULL, NULL, &waitTime ); */
			// Check for running status
			if ( !_schedExec ) return 0.0;
			// Did we receive or just get a signal
			if ( /* @todo: FD_ISSET( channel->receiveSocket, &rset ) */ ) {
				// Actually receive the message
				retVal = recvfrom( channel->receiveSocket, buffer, FP_MAXMESSAGESIZE + sizeof(uint16_t),
									0, (struct sockaddr*)&cliaddr, &len );
				// Capture the time ack was done receiving
				ackTimeMS = AbsoluteTimeMS();
				// Check for running status
				if ( !_schedExec ) return 0.0;
				// Get just msgID - for now that is all we care about
				memcpy( &msgID, buffer, sizeof(msgID) );
				// Is it the right type of message
				if ( msgID == SyncAck ) {
					// Get the channel sequence number
					memcpy( &channel->channelID, buffer + sizeof(msgID), sizeof(channel->channelID) );
					// See if ACK is intended for this node
					if ( strncmp( buffer+sizeof(msgID)+sizeof(channel->channelID), channel->localAddress, 15 ) == 0 ) {
						// Set the SyncAck flag to true
						syncAckReceived = true;
						// Log the reception of SyncAck
						LogEvent( Syncronization, SyncAckReceive, inet_ntoa( cliaddr.sin_addr ) );
					}
				}
			}
			// How much time has elapsed
			timeMS = AbsoluteTimeMS();
		} while ( timeMS < deadlineMS && !syncAckReceived );
	// ... until SyncStart message is received - or scheduler is not executing
	} while ( !syncAckReceived );

	// Figure out a rough one-way transmission delay total
	deltaMS = ( readyTimeMS - ackTimeMS ) * 0.5;
	deltaMS = min( deltaMS, 0.5 );

	// Now must wait for SyncStart, SyncHalt or a local signal
	do {
		// @todo: FD_SET( channel->receiveSocket, &rset );
		// Wait to try to receive
		// @todo: retVal = select( channel->receiveSocket + 1, &rset, NULL, NULL, &waitTime );
		// Double check the schedExec flag
		if ( !_schedExec ) return 0.0;
		// Did we receive or just get a signal
		if ( /* @todo: FD_ISSET( channel->receiveSocket, &rset ) */ ) {
			// Actually receive the message
			retVal = recvfrom( channel->receiveSocket, buffer, FP_MAXMESSAGESIZE + sizeof(uint16_t),
								0, (struct sockaddr*)&cliaddr, &len );
			// Double check the schedExec flag
			if ( !_schedExec ) return 0.0;
			memcpy( &msgID, buffer, sizeof(msgID) );
			// Is it the SyncStart message
			if ( msgID == SyncStart ) {
				// Set the SyncAck flag to true
				syncStartReceived = true;
				// Log the reception of SyncStart
				LogEvent( Syncronization, SyncStartReceive, inet_ntoa( cliaddr.sin_addr ) );
			}
			// Or is it the SyncHalt message
			else if ( msgID == SyncHalt ) {
				// Log that this has been received
				LogEvent( Syncronization, SyncHaltReceive, inet_ntoa( cliaddr.sin_addr ) );
				// Set the schedExec flag to false
				_schedExec = false;
				return 0.0;
			}
			// Otherwise, I can ignore this message
		}
	// Wait until the SyncStart message is received
	} while ( !syncStartReceived );
	// Start the asyncReceive thread running
	_PostSemaphore( channel->semaphore );
	// Log how much time sync took
	sprintf( buffer, "Syncronized Channel(%d) in %4.3fms with delay delta of %4.3fms.\n",
			 channel->channelID, AbsoluteTimeMS() - startMS, -deltaMS );
	LogMessage( buffer );
	// Return the clock adjustment value
	return deltaMS;
}

/* Handle a nicely synchronized shutdown */
void _SerialSyncShutdown( void* chnl ) {
	// Cast to a Serial_Channel
	Serial_Channel* channel = (Serial_Channel*)chnl;
	char buffer[ FP_MAXMESSAGESIZE + sizeof(uint16_t) ];
	uint16_t msgID = SyncHalt;
	int retVal;
	/* @todo: Prepare the sending port
	struct sockaddr_in servaddr;
	bzero( &servaddr, sizeof(servaddr) );
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons( channel->receivePort );
	inet_pton( AF_INET, channel->remoteAddress, &servaddr.sin_addr ); */
	// Did we receive a SyncHalt, or are we the first
	if ( _serialReceivedHalt ) return;
	// Just send out a SyncHalt message
	memcpy( buffer, &msgID, sizeof(msgID) );
	// Send out the SyncHalt message
	retVal = sendto( channel->sendSocket, buffer, sizeof(msgID), 0, (struct sockaddr*)(&servaddr), sizeof(servaddr) );
	// Make sure there was not a sending error
	if ( retVal == -1 ) {
		// Manage the error
		_serialErrorHandler( SERIAL_ERR_SENDTOFAIL, channel, GetTimeMS() );
		// Exit the function
		return;
	}
	// Log the sending of SyncStart
	LogEvent( Syncronization, SyncHaltSend, channel );
}

/*****************************************************************************/


/* Initialize the serial subsystem (all channels) */
bool Serial_Initialize( void (*ErrorHandler)(FE_ErrorCode,void*,pfloat_t) ) {

        // Static initializers (for broken compilers)
        _serialInit = false;
        _serialNumChannels = 0;
        _serialMaxFD = 0;
        _newChannel = NULL;
        _newSyncExpect = NULL;
        _newAsyncExpect = NULL;
        _serialReceivedHalt = false;

	// Set the error handling function
	_serialErrorHandler = ErrorHandler;
	// Initialize the thread semaphores
	_serialInitSemaphore = _CreateSemaphore( "_serialInitSemaphore", 0 ); 

	// Set the global init flag
	_serialInit = true;
	// Return success
	return true;
}

/* Shuts down the serial subsystem (all channels) */
bool Serial_Shutdown( void ) {
	uint8_t i = 0;
	// See if ever initialized
	if ( !_serialInit ) return true;
	// Go through all channels
	for ( ; i < _serialNumChannels; i++ ) {
		// Try signaling the channel semaphore - just in case
		_PostSemaphore( _serialChannels[i].semaphore );
		// Wait on the asyncReceive thread
		pthread_join( _serialChannels[i].asyncThread, NULL );
		/* @todo: Close the send and receive sockets
		_CloseSocket( _serialChannels[i].receiveSocket );
		_CloseSocket( _serialChannels[i].sendSocket ); */
	}
	// Destroy the thread semaphores
	_DestroySemaphore( "_serialInitSemaphore", _serialInitSemaphore );
	// Set the global init flag
	_serialInit = false;
	// Return success
	return true;
}

/* Create a serial channel with a given set of expectations */
Serial_Channel* Serial_CreateChannel( char* serialPort, uint32_t baud, 
                                      uint16_t dataBits, bool parity,
                                      uint16_t stopBits,
                                      uint8_t numSyncExpectations, 
                                      FP_SyncExpectation* syncExpectations,
                                      uint8_t numAsyncExpectations, 
                                      FP_AsyncExpectation* asyncExpectations,
                                      bool schedSyncChannel, 
                                      bool syncMaster, 
                                      uint8_t numSyncPeers ) {
	uint8_t i = 0;
	char channelName[SERIAL_NAMELENGTH];
	char taskName[SERIAL_NAMELENGTH];
	// Check some values
	assert( _serialInit );
	assert( !_schedExec );
	assert( _serialNumChannels <= SERIAL_MAXCHANNELS );
	// @todo: assert( localIPAddress != NULL );
	// @todo: assert( remoteIPAddress != NULL );
	assert( numSyncExpectations <= SERIAL_MAXSYNCEXPECTATIONS );
	assert( numAsyncExpectations <= SERIAL_MAXASYNCEXPECTATIONS );
	assert( numSyncExpectations == 0 || syncExpectations != NULL );
	assert( numAsyncExpectations == 0 || asyncExpectations != NULL );
	// Make sure port is not already in use
	assert( _CheckPort( receivePort ) );
	// Determine port name
	sprintf( channelName, "Serial_%d", receivePort );
	// Set up the new channel structure
	_newChannel = _serialChannels + _serialNumChannels;
	if ( schedSyncChannel ) {
		// Init value is 0 because initial Sync will happen before asyncReceive takes over	
		_newChannel->semaphore = _CreateSemaphore( channelName, 0 );
	}
	// Otherwise, allow the async thread to start polling immediately
	else {
		_newChannel->semaphore = _CreateSemaphore( channelName, 1 );
	}
	strcpy( _newChannel->serialDevice, serialPort ); 
        _newChannel->baud = baud;
        _newChannel->dataBits = dataBits;
        _newChannel->parity = parity;
        _newChannel->stopBits = stopBits;
	_newChannel->numSyncExpectations = numSyncExpectations;
	_newChannel->syncExpectations = syncExpectations;
	_newChannel->numAsyncExpectations = numAsyncExpectations;
	_newChannel->asyncExpectations = asyncExpectations;
	_newChannel->syncChannel = schedSyncChannel;
	_newChannel->syncMaster = syncMaster;
	_newChannel->numSyncPeers = numSyncPeers;
	_newChannel->activeExpect = NULL;
	_newChannel->activeReceive = false;
	_newChannel->channelID = 255;
	_serialNumChannels++;
	// Try to create the receiving and sending sockets
	if ( !_SerialCreateConnections( _newChannel ) ) {
		// Fatal error - not able to continue
		exit( -1 );
	}

        // Update the max file descriptor for select
        if ( (_newChannel->filedesc + 1) > _serialMaxFD )
               _serialMaxFD = _newChannel->filedesc + 1;

	// If there are synchronous expectations
	for ( ; i < numSyncExpectations; i++ ) {
		char directionDesignator;
		// Get the expectation pointer
		_newSyncExpect = syncExpectations + i;
		// Check the properties of this expectation
		assert( _newSyncExpect->msgID != 0 && _newSyncExpect->msgID < 65500 );
		assert( _newSyncExpect->numStartTimes != 0 );
		assert( _newSyncExpect->startTimesMS != NULL );
		assert( _newSyncExpect->wcetMS > 0.0 );
		// Generate the task name and see if we are sending or receiving
		if ( _newSyncExpect->direction == ExpectReceive ) directionDesignator = 'r';
		else directionDesignator = 's';
		sprintf( taskName, "%s_%c%d", channelName, directionDesignator, _newSyncExpect->msgID );
		// Create a periodic task
		FS_CreatePeriodicTask( taskName, 
                                       _SerialSyncExec, 
                                       &_newSyncExpect->task, 
                                       _newSyncExpect->wcetMS,
                                       NoRestriction, 
                                       _newSyncExpect->numStartTimes, 
                                       _newSyncExpect->startTimesMS );
		// Wait for sync thread to be fully initialized
		_WaitSemaphore( _serialInitSemaphore );
	}
	// Are there any asyncronous expectations
	for (i = 0; i < numAsyncExpectations; i++ ) {
		// Get the expectation pointer
		_newAsyncExpect = asyncExpectations + i;
		// Check the properties of this expectation
		assert( _newAsyncExpect->msgID != 0 && _newAsyncExpect->msgID < 65500 );
		assert( _newAsyncExpect->wcetMS > 0.0 );
		assert( _newAsyncExpect->timeLowerBoundMS >= 0.0 );
		assert( _newAsyncExpect->timeUpperBoundMS >= 0.0 );
		// For sending tasks only
		if ( _newAsyncExpect->direction == ExpectSend ) {
			// Generate the task name
			sprintf( taskName, "%s_s%d", channelName, _newAsyncExpect->msgID );
			// Create a sporadic task
			FS_CreateSporadicTask( taskName, 
                                               _SerialAsyncSend, 
                                               &_newAsyncExpect->task,
                                               _newAsyncExpect->wcetMS, 
                                               NoRestriction, 
                                               SERIAL_ASYNCPRIORITY, 9999 );
			// Wait for primary thread to be fully initialized
			_WaitSemaphore( _serialInitSemaphore );
		}
	}
	// Is this a syncronization channel
	if ( schedSyncChannel ) {
		// Is this the master - use the right function
		if ( syncMaster ) _SchedAddSyncChannel( _newChannel, _SerialSyncInitMaster, _SerialSyncShutdown );
		// Must be the slave
		else _SchedAddSyncChannel( _newChannel, _SerialSyncInitSlave, _SerialSyncShutdown );
	}
	// Set up the asyncronous receive thread
	_CreateThread( &(_newChannel->asyncThread), _SerialAsyncReceiveExec, _newChannel, AsyncReceiveThread );
	// Wait for the async thread initialization to complete
	_WaitSemaphore( _serialInitSemaphore );
	// Return the new channel
	return _newChannel;
}

/* Create a job for the sporadic task associated with msgID */
bool Serial_AsyncSend( Serial_Channel* channel, 
                       uint16_t msgID, 
                       pfloat_t relativeDeadlineMS ) {
	FP_AsyncExpectation* expectation = NULL;
	uint8_t i = 0;
	assert( channel != NULL );
	assert( relativeDeadlineMS > 0.0 );
	// Find the correct schedulable by looking through all async expectations
	for ( ; i < channel->numAsyncExpectations; i++ ) {
		// If this expect is both send and matches msgID
		if ( channel->asyncExpectations[i].direction == ExpectSend &&
			channel->asyncExpectations[i].msgID == msgID ) {
			// Have we already found an expectation - this would be a bad thing
			assert( expectation == NULL );
			// Set the expectation
			expectation = channel->asyncExpectations + i;
		}
	}
	// Did we not find it - this is a bad thing
	assert( expectation != NULL );
	// Otherwise, create a job for this task
	FS_CreateJob( expectation->task, relativeDeadlineMS );
	// Create a job for this schedulable
	return true;
}

// Primary thread for all of the serial channels
void * _SerialInputThread( void * nothing ) {

	int index;
	fd_set readfs;
        while ( !_serialReceivedHalt )
	{
                for ( index = 0; index < _serialNumChannels; index++ )
		{
			FD_SET(_serialChannels[index].filedesc, &readfs );
		}

		select( _serialMaxFD, &readfs, NULL, NULL, NULL );

                for ( index = 0; index < _serialNumChannels; index++ )
		{
			if ( FD_ISSET( _serialChannels[index].filedesc, &readfs ) )
			{
				
			}
		}
                
	}
}

/*****************************************************************************/

