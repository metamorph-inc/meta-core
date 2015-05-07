/*** Copyright Vanderbilt ***/


/*** Included Header Files ***/
#include "ports.h"
#include "logger.h"


/*****************************************************************************/


/* INTERNAL: Structure for sampling ports */
typedef struct {
	PortId_t				portID;										// ID of the sampling port
	char					name[FP_MAXPORTNAMESIZE];					// Name of the sampling port
	MessageSize_t			maxMessageSizeB;							// Maximum message size port can hold
	PortDirection_t			direction;									// Direction of port
	SystemTime_t			refreshPeriodQ;								// Maximum age of data before not valid
	Semaphore				*semaphore;									// Access semaphore for port
	char*					memory;										// Pointer to memory buffer
	bool					dirty;										// Ever writen flag
	bool					fresh;										// Freshness flag ( write since last read )
	SystemTime_t			writeTimeQ;									// Time of last write
	MessageSize_t			writeSizeB;									// Size of last write
	PortStatus_t			readStatus;									// Status at last read
} SamplingPort;


/* INTERNAL: Structure for queuing port queue elements */
typedef struct {
	MessageSize_t			sizeB;										// Size of the queued message
	SystemTime_t			expirationTimeQ;							// Timeout of the queued message
} QueuingPortElement;


/* INTERNAL: Structure for queuing ports */
typedef struct {
	PortId_t				portID;										// ID of created queuing port
	char					name[FP_MAXPORTNAMESIZE];					// Name of the port
	MessageSize_t			maxMessageSizeB;							// Maximum size of message
	QueueSize_t				maxQueueSize;								// Maximum number of messages in queue
	PortDirection_t			direction;									// Port direction
	QueuingDiscipline_t		queuingDiscipline;							// Discipline of the queuing port
	Semaphore				*semaphore;									// Access semaphore for port
	bool					fresh;										// Freshness flag ( write since last read )
	QueueSize_t				firstElement;								// Index of first element in the queue
	QueueSize_t				numElements;								// Number of elements in the queue
	QueuingPortElement		*elements;									// Pointer to array of element structure in queue
	char*					memory;										// Pointer to element data
} QueuingPort;


/* INTERNAL: Structure for generic ports */
typedef struct {
	bool					sampling;									// Flag for if the port is sampling or not
	PortId_t				portID;										// Index into either sampling or queuing port array
} GenericPort;


/* INTERNAL: Structure for all port management */
typedef struct {
	char					memoryPool[FP_PORTMEMORYPOOLSIZE];			// Primary memory pool for message storage
	char*					memoryAllocPointer;							// Pointer to start of memory pool free space
	SamplingPort			samplingPorts[FP_MAXSAMPLINGPORTS];			// Array of all sampling ports
	PortId_t				numSamplingPorts;							// Number of sampling ports
	QueuingPort				queuingPorts[FP_MAXQUEUINGPORTS];			// Array of all queuing ports
	PortId_t				numQueuingPorts;							// Number of queuing ports
	PortId_t				numPorts;									// Total number of ports
	GenericPort				ports[FP_MAXSAMPLINGPORTS+FP_MAXQUEUINGPORTS];	// Array of generic ports
} PortManager;


/*** INTERNAL: Static Variables ***/
static PortManager			_portManager;								// Static allocation for the actual port manager
static bool					_portInitialized = false;					// Flag for port manager initialization


/*****************************************************************************/


/* INTERNAL: Initialize the port manager */
void _InitializePortManager( void ) {
	// Make sure we are not already initialized
	if ( _portInitialized ) return;
	// Set up the port manager data structure
	_portManager.memoryAllocPointer = _portManager.memoryPool;
	// Zero the memory pool
	memset( _portManager.memoryPool, 0, FP_PORTMEMORYPOOLSIZE );
	// Clear the number of ports
	_portManager.numSamplingPorts = 0;
	_portManager.numQueuingPorts = 0;
	_portManager.numPorts = 0;
	// Mark the manager as initialized
	_portInitialized = true;
}


/* INTERNAL: Check port manager memory pool to see if enough memory is available */
bool _CheckMemoryPoolSpace( const MessageSize_t requestedSizeB ) {
	// Make sure we are initialized
	if ( !_portInitialized ) return false;
	// See if enough space remains
	if ( _portManager.memoryAllocPointer + requestedSizeB >= _portManager.memoryPool + FP_PORTMEMORYPOOLSIZE ) return false;
	// There must be enough space
	return true;
}


/* INTERNAL: Check all sampling and queuing ports to see if name is already in use */
bool _CheckPortNameUniqueness( const PortName_t name, const bool sampling ) {
	PortId_t i = 0;
	// Assert checks on states and inputs
	assert( name != NULL );
	// Make sure we are initialized
	if ( !_portInitialized ) return false;
	// Scan through all sampling ports ( if requested )
	if ( sampling ) {
		for ( ; i < _portManager.numSamplingPorts; i++ ) {
			// Compare the names
			int cmpValue = strncmp( name, _portManager.samplingPorts[i].name, FP_MAXPORTNAMESIZE );
			// If there is no difference, then this name is not unique
			if ( cmpValue == 0 ) return false;
		}
	// Scan through all queuing ports
	} else {
		for ( i = 0 ; i < _portManager.numQueuingPorts; i++ ) {
			// Compare the names
			int cmpValue = strncmp( name, _portManager.queuingPorts[i].name, FP_MAXPORTNAMESIZE );
			// If there is no difference, then this name is not unique
			if ( cmpValue == 0 ) return false;
		}
	}
	// Must be a unique name
	return true;
}


/* INTERNAL: Add a new generic port - returns the generic port ID */
PortId_t _AddPort( bool samplingPort, PortId_t portID ) {
	// Get the new port ID and increment the number of generic ports
	PortId_t newPort = _portManager.numPorts++;
	// Setup the correct generic port struct
	GenericPort *port = _portManager.ports + newPort;
	port->portID = portID;
	port->sampling = samplingPort;
	// Return the ID of the new generic port
	return newPort;
}


/*****************************************************************************/


/* PUBLIC: Create a new sampling port */
void CreateSamplingPort( PortName_t name, MessageSize_t maxMessageSizeB, PortDirection_t direction,
						 SystemTime_t refreshPeriodQ, PortId_t *samplingPortID, ReturnCode_t *returnCode ) {
	SamplingPort* port = NULL;
	// Make sure we are initialized
	_InitializePortManager();
	// Assert checks on states and inputs
	assert( name != NULL );
	assert( samplingPortID != NULL );
	assert( returnCode != NULL );
	// Make sure a sampling port is available
	if ( _portManager.numSamplingPorts >= FP_MAXSAMPLINGPORTS ) {
		// Log the error
		LogMessage( "CreateSamplingPort Error: FP_MAXSAMPLINGPORTS limit exceeded." );
		// Set the return values and exit this function
		*samplingPortID = 0;
		*returnCode = InvalidConfig;
		return;
	}
	// Make sure there is space in the memory pool
	if ( !_CheckMemoryPoolSpace( maxMessageSizeB ) ) {
		// Log the error
		LogMessage( "CreateSamplingPort Error: Not enough memory space available." );
		// Set the return values and exit this function
		*samplingPortID = 0;
		*returnCode = InvalidConfig;
		return;
	}
	// Make sure the name is unique
	if ( !_CheckPortNameUniqueness( name, true ) ) {
		// Log the fact that the port already exists
		LogMessage( "CreateSamplingPort Error: Requested name is already in use." );
		// Set the return values and exit this function
		*samplingPortID = 0;
		*returnCode = NoAction;
		return;
	}
	// Make sure the max message size is not too big
	if ( maxMessageSizeB > FP_MAXSAMPLINGMESSAGESIZE ) {
		// Log the error
		LogMessage( "CreateSamplingPort Error: Requested size exceeds FP_MAXSAMPLINGMESSAGESIZE limit." );
		// Set the return values and exit this function
		*samplingPortID = 0;
		*returnCode = InvalidConfig;
		return;
	}
	// Make sure refresh period is in a valid range
	if ( refreshPeriodQ > FP_MAXSAMPLINGREFRESHPERIOD ) {
		// Log the error
		LogMessage( "CreateSamplingPort Error: Requested refresh period exceeds FP_MAXSAMPLINGREFRESHPERIOD limit." );
		// Set the return values and exit this function
		*samplingPortID = 0;
		*returnCode = InvalidConfig;
		return;
	}

	// Get the next available sampling port
	port = _portManager.samplingPorts + _portManager.numSamplingPorts;
	// Request a new generic port ID
	port->portID = _AddPort( true, _portManager.numSamplingPorts++ );
	// Copy in the port name
	strncpy( port->name, name, FP_MAXPORTNAMESIZE );
	// Set the max message size
	port->maxMessageSizeB = maxMessageSizeB;
	// Set the direction
	port->direction = direction;
	// Set the port refresh period
	port->refreshPeriodQ = refreshPeriodQ;
	// Create the semaphore for the port - semaphore must have value of 1 initially
	port->semaphore = _CreateSemaphore( name, 1 );
	// Set the port memory pointer
	port->memory = _portManager.memoryAllocPointer;
	// Zero the memory
	memset( port->memory, 0, port->maxMessageSizeB );
	// Move the port manager memory pointer
	_portManager.memoryAllocPointer += maxMessageSizeB;
	// Mark the port as clean
	port->dirty = false;
	// Mark the port as not fresh
	port->fresh = false;
	// Zero out the read/write stats
	port->writeTimeQ = 0;
	port->writeSizeB = 0;
	port->readStatus = Invalid;
	// All is good, so return success
	*samplingPortID = port->portID;
	*returnCode = NoError;
}


/* PUBLIC: Write to the sampling port */
void WriteSamplingMessage( PortId_t samplingPortID, MessageAddress_t messageAddress,
						   MessageSize_t lengthB, ReturnCode_t *returnCode ) {
	SamplingPort* port = NULL;
	// Assert checks on states and inputs
	assert( _portInitialized );
	assert( messageAddress != NULL );
	assert( returnCode != NULL );
	// Make sure port is valid
	if ( samplingPortID > _portManager.numSamplingPorts ) {
		// Log the error
		LogMessage( "WriteSamplingPort Error: Port does not exist." );
		// Set the return value and exit this function
		*returnCode = InvalidParameter;
		return;
	}
	// Set the port pointer
	port = _portManager.samplingPorts + samplingPortID;
	// Make sure length does not exceed port message size max
	if ( lengthB > port->maxMessageSizeB ) {
		// Log the error
		LogMessage( "WriteSamplingPort Error: Message size exceeds port message size limit." );
		// Set the return value and exit this function
		*returnCode = InvalidConfig;
		return;
	}
	// Make sure port direction allows writes
	if ( port->direction == Source ) {
		// Log the error
		LogMessage( "WriteSamplingPort Error: Writes not allowed as port is strictly a source." );
		// Set the return value and exit this function
		*returnCode = InvalidMode;
		return;
	}

	// Lock the port
	_WaitSemaphore( port->semaphore );
	// Write to the port memory address
	memcpy( port->memory, messageAddress, lengthB );
	// Mark the port as dirty
	port->dirty = true;
	// Mark the port as fresh
	port->fresh = true;
	// Record the write stats
	port->writeTimeQ = 0;
	port->writeSizeB = lengthB;
	// Success, so return NoError
	*returnCode = NoError;
	// Unlock the port
	_PostSemaphore( port->semaphore );
}


/* PUBLIC: Read from a Sampling port */
void ReadSamplingMessage( PortId_t samplingPortID, MessageAddress_t messageAddress,
						  MessageSize_t *lengthB, Validity_t *validity, ReturnCode_t *returnCode ) {
	SamplingPort* port = NULL;
	// Assert checks on states and inputs
	assert( _portInitialized );
	assert( messageAddress != NULL );
	assert( returnCode != NULL );
	// Make sure port is valid
	if ( samplingPortID > _portManager.numSamplingPorts ) {
		// Log the error
		LogMessage( "WriteSamplingPort Error: Port does not exist." );
		// Set the return values and exit this function
		*returnCode = InvalidParameter;
		return;
	}
	// Set the port pointer
	port = _portManager.samplingPorts + samplingPortID;
	// Lock the port
	_WaitSemaphore( port->semaphore );
	// Make sure port direction allows reads
	if ( port->direction == Destination ) {
		// Log the error
		LogMessage( "WriteSamplingPort Error: Writes not allowed as port is strictly a destination." );
		// Mark the read status
		port->readStatus = Invalid;
		// Set the return values and exit this function
		if ( validity != NULL) *validity = port->readStatus;
		*returnCode = InvalidMode;
	}
	// See if the port is not dirty (never been writen to)
	else if ( !port->dirty ) {
		// Mark the read status
		port->readStatus = Invalid;
		// Set return values (Validity = false, RC = NoAction)
		if ( validity != NULL) *validity = port->readStatus;
		*returnCode = NoAction;
	}
	// See if the memory is valid (based on time stamps)
	else if ( false ) {
		// Mark the read status
		port->readStatus = Invalid;
		// Set the return values (Validity = false, RC = NoError)
		if ( validity != NULL) *validity = port->readStatus;
		// No error
		*returnCode = NoError;
	}
	// Otherwise...
	else {
		// Copy the message to the output buffer
		memcpy( messageAddress, port->memory, port->writeSizeB );
		// Mark the read status
		port->readStatus = Valid;
		// Mark the port as not fresh
		port->fresh = false;
		// Set the number of bytes in the message and the validity
		if ( lengthB != NULL ) *lengthB = port->writeSizeB;
		if ( validity != NULL) *validity = port->readStatus;
		// Success, so return NoError
		*returnCode = NoError;
	}
	// Unlock the port
	_PostSemaphore( port->semaphore );
}


/* PUBLIC: Given a name, get the sampling port ID */
void GetSamplingPortID( PortName_t name, PortId_t *samplingPortID, ReturnCode_t *returnCode ) {
	PortId_t i = 0;
	// Assert checks on states and inputs
	assert( _portInitialized );
	assert( name != NULL );
	assert( samplingPortID != NULL );
	assert( returnCode != NULL );
	// Loop through all sampling ports
	for ( ; i < _portManager.numSamplingPorts; i++ ) {
		// Get pointer to port
		SamplingPort* port = _portManager.samplingPorts + i;
		// See if the name matches
		if ( strncmp( name, port->name, FP_MAXPORTNAMESIZE ) == 0 ) {
			// Match found - return correct values
			*samplingPortID = port->portID;
			*returnCode = NoError;
			// No need to keep looking - return
			return;
		}
	}
	// No match found, log the error
	LogMessage( "GetSamplingPortID Error: Port with given name does not exist." );
	// Return error condition
	*samplingPortID = 0;
	*returnCode = InvalidConfig;
}


/* PUBLIC: Get the status of the sampling port */
void GetSamplingPortStatus( PortId_t samplingPortID, PortStatus_t *samplingPortStatus, ReturnCode_t *returnCode ) {
	SamplingPort* port = NULL;
	// Assert checks on states and inputs
	assert( _portInitialized );
	assert( samplingPortStatus != NULL );
	assert( returnCode != NULL );
	// Make sure port is valid
	if ( samplingPortID > _portManager.numSamplingPorts ) {
		// Log the error
		LogMessage( "GetSamplingPortStatus Error: Port does not exist." );
		// Set the return values and exit this function
		*samplingPortStatus = Invalid;
		*returnCode = InvalidParameter;
		return;
	}
	// Set the port pointer
	port = _portManager.samplingPorts + samplingPortID;
	// Lock the port
	_WaitSemaphore( port->semaphore );
	// Return the validity of the last message read
	*samplingPortStatus = port->readStatus;
	*returnCode = NoError;
	// Unlock the port
	_PostSemaphore( port->semaphore );
}


/*****************************************************************************/


/* PUBLIC: Create a queuing port */
void CreateQueuingPort( PortName_t name, MessageSize_t maxMessageSizeB, QueueSize_t maxQueueSize,
						PortDirection_t direction, QueuingDiscipline_t queuingDiscipline,
						PortId_t *queuingPortID, ReturnCode_t *returnCode ) {
	QueuingPort* port = NULL;
	// Make sure we are initialized
	_InitializePortManager();
	// Assert checks on states and inputs
	assert( name != NULL );
	assert( queuingPortID != NULL );
	assert( returnCode != NULL );
	// Make sure a queuing port is available
	if ( _portManager.numQueuingPorts >= FP_MAXQUEUINGPORTS ) {
		// Log the error
		LogMessage( "CreateQueuingPort Error: FP_MAXQUEUINGPORTS limit exceeded." );
		// Set the return values and exit this function
		*queuingPortID = 0;
		*returnCode = InvalidConfig;
		return;
	}
	// Make sure there is space in the memory pool
	if ( !_CheckMemoryPoolSpace( maxMessageSizeB * maxQueueSize + sizeof(QueuingPortElement) * maxQueueSize ) ) {
		// Log the error
		LogMessage( "CreateQueuingPort Error: Not enough memory space available." );
		// Set the return values and exit this function
		*queuingPortID = 0;
		*returnCode = InvalidConfig;
		return;
	}
	// Make sure the name is unique
	if ( !_CheckPortNameUniqueness( name, false ) ) {
		// Log the fact that the port already exists
		LogMessage( "CreateQueuingPort Error: Requested name is already in use." );
		// Set the return values and exit this function
		*queuingPortID = 0;
		*returnCode = NoAction;
		return;
	}
	// Make sure the max message size is not too big
	if ( maxMessageSizeB > FP_MAXQUEUINGMESSAGESIZE ) {
		// Log the error
		LogMessage( "CreateQueuingPort Error: Requested message size exceeds FP_MAXQUEUINGMESSAGESIZE limit." );
		// Set the return values and exit this function
		*queuingPortID = 0;
		*returnCode = InvalidConfig;
		return;
	}
	// Make sure the max queue size is not too big
	if ( maxQueueSize > FP_MAXQUEUESIZE ) {
		// Log the error
		LogMessage( "CreateQueuingPort Error: Requested queue size exceeds FP_MAXQUEUESIZE limit." );
		// Set the return values and exit this function
		*queuingPortID = 0;
		*returnCode = InvalidConfig;
		return;
	}

	// Get the next available queuing port
	port = _portManager.queuingPorts + _portManager.numQueuingPorts;
	// Request a new generic port ID
	port->portID = _AddPort( false, _portManager.numQueuingPorts++ );
	// Copy in the port name
	strncpy( port->name, name, FP_MAXPORTNAMESIZE );
	// Set the max message size
	port->maxMessageSizeB = maxMessageSizeB;
	// Set the max queue size
	port->maxQueueSize = maxQueueSize;
	// Set the direction
	port->direction = direction;
	// Set the queuing discipline
	port->queuingDiscipline = queuingDiscipline;
	// Create the semaphore for the port - semaphore must have value of 1 initially
	port->semaphore = _CreateSemaphore( name, 1 );
	// Zero out the first index and number of elements
	port->firstElement = 0;
	port->numElements = 0;
	// Set the port element structures pointer
	port->elements = (QueuingPortElement*)_portManager.memoryAllocPointer;
	_portManager.memoryAllocPointer += sizeof(QueuingPortElement) * maxQueueSize;
	// Set the port element data pointer
	port->memory = (void*)_portManager.memoryAllocPointer;
	_portManager.memoryAllocPointer += maxMessageSizeB * maxQueueSize;
	// All is good, so return success
	*queuingPortID = port->portID;
	*returnCode = NoError;
}


/* PUBLIC: Write a message into a queuing port */
void SendQueuingMessage( PortId_t queuingPortID, MessageAddress_t messageAddress, MessageSize_t lengthB,
						 SystemTime_t timeoutQ, ReturnCode_t *returnCode ) {
	QueuingPort* port = NULL;
	QueuingPortElement *element = NULL;
	QueueSize_t index = 0;
	// Assert checks on states and inputs
	assert( _portInitialized );
	assert( messageAddress != NULL );
	assert( returnCode != NULL );
	// Make sure port is valid
	if ( queuingPortID > _portManager.numQueuingPorts ) {
		// Log the error
		LogMessage( "SendQueuingMessage Error: Port does not exist." );
		// Set the return value and exit this function
		*returnCode = InvalidParameter;
		return;
	}
	// Set the port pointer
	port = _portManager.queuingPorts + queuingPortID;
	// Check timeout value
	if ( false ) {
		// Log the error
		LogMessage( "SendQueuingMessage Error: Timeout value out of range." );
		// Set the return value and exit this function
		*returnCode = InvalidParameter;
		return;
	}
	// Make sure length does not exceed port message size max
	if ( lengthB > port->maxMessageSizeB ) {
		// Log the error
		LogMessage( "SendQueuingMessage Error: Message size exceeds port message size limit." );
		// Set the return value and exit this function
		*returnCode = InvalidConfig;
		return;
	}
	// Make sure port direction allows writes
	if ( port->direction == Source ) {
		// Log the error
		LogMessage( "SendQueuingMessage Error: Writes not allowed as port is strictly a source." );
		// Set the return value and exit this function
		*returnCode = InvalidMode;
		return;
	}

	// Lock the port
	_WaitSemaphore( port->semaphore );
	// Determine the new element index
	index = ( port->firstElement + port->numElements ) % port->maxQueueSize;
	// Get the element structure
	element = port->elements + index;
	// Set up the element structure
	element->sizeB = lengthB;
	element->expirationTimeQ = timeoutQ;
	// Write to the element memory address
	memcpy( port->memory + port->maxMessageSizeB * index, messageAddress, lengthB );
	// Mark the port as fresh
	port->fresh = true;
	// Increment the element counter
	port->numElements++;
	// Success, so return NoError
	*returnCode = NoError;
	// Unlock the port
	_PostSemaphore( port->semaphore );
}


/* PUBLIC: Receive a message on a queuing port */
void ReceiveQueuingMessage( PortId_t queuingPortID, SystemTime_t timeoutQ, MessageAddress_t messageAddress,
						    MessageSize_t *lengthB, ReturnCode_t *returnCode ) {
	QueuingPort* port = NULL;
	QueuingPortElement *element = NULL;
	// Assert checks on states and inputs
	assert( _portInitialized );
	assert( messageAddress != NULL );
	assert( lengthB != NULL );
	assert( returnCode != NULL );
	// Make sure port is valid
	if ( queuingPortID > _portManager.numQueuingPorts ) {
		// Log the error
		LogMessage( "ReceiveQueuingMessage Error: Port does not exist." );
		// Set the return value and exit this function
		*returnCode = InvalidParameter;
		return;
	}
	// Set the port pointer
	port = _portManager.queuingPorts + queuingPortID;
	// Check timeout value
	if ( false ) {
		// Log the error
		LogMessage( "ReceiveQueuingMessage Error: Timeout value out of range." );
		// Set the return value and exit this function
		*returnCode = InvalidParameter;
		return;
	}
	// Make sure port direction allows reads
	if ( port->direction == Destination ) {
		// Log the error
		LogMessage( "ReceiveQueuingMessage Error: Reads not allowed as port is strictly a destination." );
		// Set the return value and exit this function
		*returnCode = InvalidMode;
		return;
	}

	// Lock the port
	_WaitSemaphore( port->semaphore );
	// Get the element structure
	element = port->elements + port->firstElement;
	// Get the length of the message
	*lengthB = element->sizeB;
	// Read the element into the memory address
	memcpy( messageAddress, port->memory + port->maxMessageSizeB * port->firstElement, element->sizeB );
	// Mark the port as not fresh
	port->fresh = false;
	// Increment the first element and counter
	port->firstElement = ( port->firstElement + 1 ) % port->maxQueueSize;
	port->numElements--;
	// Success, so return NoError
	*returnCode = NoError;
	// Unlock the port
	_PostSemaphore( port->semaphore );
}


/* PUBLIC: Retrieve a queuing port ID from a name */
void GetQueuingPortID( PortName_t name, PortId_t *queuingPortID, ReturnCode_t *returnCode ) {
	PortId_t i = 0;
	// Assert checks on states and inputs
	assert( _portInitialized );
	assert( name != NULL );
	assert( queuingPortID != NULL );
	assert( returnCode != NULL );
	// Loop through all queuing ports
	for ( ; i < _portManager.numSamplingPorts; i++ ) {
		// Get pointer to port
		QueuingPort* port = _portManager.queuingPorts + i;
		// See if the name matches
		if ( strncmp( name, port->name, FP_MAXPORTNAMESIZE ) == 0 ) {
			// Match found - return correct values
			*queuingPortID = port->portID;
			*returnCode = NoError;
			// No need to keep looking - return
			return;
		}
	}
	// No match found, log the error
	LogMessage( "GetQueuingPortID Error: Port with given name does not exist." );
	// Return error condition
	*queuingPortID = 0;
	*returnCode = InvalidConfig;
}


/* PUBLIC: Get the status of the queuing port */
void GetQueuingPortStatus( PortId_t queuingPortID, PortStatus_t *queuePortStatus, ReturnCode_t *returnCode ) {
}


/*****************************************************************************/


/* PUBLIC: Send a message via a generic port */
void WriteMessage( PortId_t portID, MessageAddress_t messageAddress, MessageSize_t lengthB, SystemTime_t timeoutQ, ReturnCode_t *returnCode ) {
	// Lookup if generic port is sampling or queuing
	GenericPort *port = _portManager.ports + portID;
	// If this is a sampling port
	if ( port->sampling ) {
		// Call to the sampling port ( with the correct sampling port ID )
		WriteSamplingMessage( port->portID, messageAddress, lengthB, returnCode );
	}
	// Must be a queuing port
	else {
		// Call to the queuing port ( with the correct queuing port ID )
		SendQueuingMessage( port->portID, messageAddress, lengthB, timeoutQ, returnCode );
	}
}


/* PUBLIC: Receive a message via a generic port */
void ReadMessage( PortId_t portID, MessageAddress_t messageAddress, MessageSize_t *lengthB, Validity_t *validity, ReturnCode_t *returnCode	) {
	// Lookup if generic port is sampling or queuing
	GenericPort *port = _portManager.ports + portID;
	// If this is a sampling port
	if ( port->sampling ) {
		// Call to the sampling port ( with the correct sampling port ID )
		ReadSamplingMessage( port->portID, messageAddress, lengthB, validity, returnCode );
	}
	// Must be a queuing port
	else {
		// Call to the queuing port ( with the correct queuing port ID )
		ReceiveQueuingMessage(	port->portID, 0, messageAddress, lengthB, returnCode	);
	}
}


/*****************************************************************************/

