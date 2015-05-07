/*** Copyright Vanderbilt ***/


/*** Included Header Files ***/
#include "events.h"
#include "arch.h"
#include "highres_timing.h"


/*****************************************************************************/


// Array size constants
#ifndef SYSEVENT_POOLSIZE_B
#define SYSEVENT_POOLSIZE_B				1024 * 1024 * 8						// Default is 8MB
#endif
#ifndef SYSEVENT_CATEGORYNAMELENGTH
#define SYSEVENT_CATEGORYNAMELENGTH		64									// Length of category name
#endif
#ifndef SYSEVENT_TYPECATEGORIES
#define SYSEVENT_TYPECATEGORIES			16									// Only allow 16 categories, for now
#endif
#ifndef SYSEVENT_TYPESPOOLSIZE
#define SYSEVENT_TYPESPOOLSIZE			64									// Default is 32 types (across all categories)
#endif


/*****************************************************************************/


/*** Type definition for SysEvent record ***/
typedef struct {
	uint8_t(*SerializeFunc)(EventCategory,EventID,char*,va_list);			// Func pointer for serializing event
	void(*PrintFunc)(char*,EventCategory,EventID,char*);					// Func pointer for printing event
	void(*ProcessFunc)(EventCategory,EventID,char*);						// Func pointer for processing func
} SysEventType;


/*** Type Category Definition Record ***/
typedef struct {
	char						name[SYSEVENT_CATEGORYNAMELENGTH];			// Textual name of the category
	uint8_t						numTypes;									// Number of defined types (static)
	SysEventType				*typeDefinitions;							// Pointer to start of type definitions
} SysEventCategory;


/*** Type definition for SysEvent master record ***/
typedef struct {
	char						pool[SYSEVENT_POOLSIZE_B];					// Reserved pool memory
	char*						firstRecord;								// Pointer to first record
	char*						nextFree;									// Where is next free record
	uint32_t					recordCount;								// Number of written records
	uint32_t					inBufferCount;								// Record counts for overwrite
	uint32_t					bytesRemaining;								// Number of bytes remaining in pool
	bool						overwrite, buffer, print, overrun;			// Flags for subsystem behavior
	SysEventCategory			categories[SYSEVENT_TYPECATEGORIES];		// Array of type categories
	uint16_t					numRequestedTypes;							// Total number of requested type records
	SysEventType				types[SYSEVENT_TYPESPOOLSIZE];				// Array of type definitions
	uint8_t						categoryMask;								// Mask of accepted categories
	Semaphore*					semaphore;									// Datastore access semaphore
} SysEventMaster;


/*** Static Variables ***/
static SysEventMaster			_seMasterRecord;							// Master record for datastore pool
static bool						_seInit = false;							// Flag for datastore initialization


/*** Externally Defined Variables and Functions ***/
extern bool						_schedExec;									// Is the scheduler executing


/*****************************************************************************/


/* Initialize the SysEvents subsystem */
void SysEventInitialize( uint8_t categoryMask, bool buffer, bool print, bool overwrite ) {
	// Make sure the clock is initialized
	InitializeTime( );
	// Have we already been initialized
	assert( !_schedExec );
	assert( !_seInit );
	// Pool size must be at least 1 Header + Max data size (25gb)
	assert( SYSEVENT_POOLSIZE_B > 256 + 8 );
	// Initialize the global SE master record
	_seMasterRecord.categoryMask = categoryMask;
	_seMasterRecord.firstRecord = _seMasterRecord.pool;
	_seMasterRecord.nextFree = _seMasterRecord.pool;
	_seMasterRecord.recordCount = 0;
	_seMasterRecord.inBufferCount = 0;
	_seMasterRecord.bytesRemaining = SYSEVENT_POOLSIZE_B;
	_seMasterRecord.buffer = buffer;
	_seMasterRecord.print = print;
	_seMasterRecord.overwrite = overwrite;
	_seMasterRecord.overrun = false;
	// Now zero out the entire pool
	memset( _seMasterRecord.pool, 0, SYSEVENT_POOLSIZE_B );
	// Initialize the event-type arrays(zero out the array)
	_seMasterRecord.numRequestedTypes = 0;
	memset( _seMasterRecord.categories, 0, sizeof(SysEventCategory) * SYSEVENT_TYPECATEGORIES );
	memset( _seMasterRecord.types, 0, sizeof(SysEventType) * SYSEVENT_TYPESPOOLSIZE );
	// Initialize the semaphore (init to 1 so that is initially available)
	_seMasterRecord.semaphore = _CreateSemaphore( "_seSemaphore", 1 );
	// Mark as initialized
	_seInit = true;
}


/* Register a SysEvent category and request a number of type definitions to be held aside */
void SysEventRegisterCategory( char* name, EventCategory category, uint8_t numRequestedTypes ) {
	SysEventCategory newCategory;
	// Check some values
	assert( !_schedExec );
	assert( name != NULL );
	assert( numRequestedTypes < SYSEVENT_TYPESPOOLSIZE - _seMasterRecord.numRequestedTypes );
	// Copy in the name
	memcpy( newCategory.name, name, SYSEVENT_CATEGORYNAMELENGTH );
	// Get the number of requested types
	newCategory.numTypes = numRequestedTypes;
	// Set the types pointer
	newCategory.typeDefinitions = _seMasterRecord.types + _seMasterRecord.numRequestedTypes;
	// Increment the master number of requested types
	_seMasterRecord.numRequestedTypes += numRequestedTypes;
	// Copy the category record into place
	memcpy( _seMasterRecord.categories + category, &newCategory, sizeof(SysEventCategory) );
}


/* Register handlers for a specific SysEvent */
void SysEventRegister( EventCategory category, EventID ID, uint8_t(*SerializeFunc)(EventCategory,EventID,char*,va_list),
	void(*PrintFunc)(char*,EventCategory,EventID,char*), void(*ProcessFunc)(EventCategory,EventID,char*) ) {
	SysEventType newType;
	// Get the correct SysEventCategory
	SysEventCategory* cat = _seMasterRecord.categories + category;
	// Check some values
	assert( !_schedExec );
	assert( cat != NULL );
	assert( ID < cat->numTypes );
	// Set values for new SysEventType struct
	newType.SerializeFunc = SerializeFunc;
	newType.PrintFunc = PrintFunc;
	newType.ProcessFunc = ProcessFunc;
	// Copy the new type into the proper place
	memcpy( &(cat->typeDefinitions[ID]), &newType, sizeof(SysEventType) );
}


/* Set the SysEvent Mask */
void SysEventSetMask( uint8_t numValues, ... ) {
	uint8_t mask = 0;
	uint8_t i = 0;
	// Get the list of arguments
	va_list list;
	va_start( list, numValues );
	// Loop for num times
	for (; i < numValues; i++ ) {
		// Get the mask value from the va_list
		int value = va_arg( list, int );
		// And it into the mask
		mask |= ( 1 << ( value - 1 ) );
	}
	va_end( list );
	// Set the global mask value
	_seMasterRecord.categoryMask = mask;
}


/* Dispatch a system event */
void SysEvent( EventCategory category, EventID ID, ... ) {
	// Make sure we want this category of event
	uint8_t catMask = 1 << ( category - 1 );
	if ( ( catMask & _seMasterRecord.categoryMask) == 0 ) return;
	// Otherwise, try to process it
	else {
		// Get the correct SysEventCategory
		SysEventCategory* cat = _seMasterRecord.categories + category;
		// Get the correct SysEventType
		SysEventType* type = cat->typeDefinitions + ID;
		// Is there one there
		if ( type->SerializeFunc != NULL ) {
			uint8_t size;
			// Set up buffer space
			char buffer[256];
			// Get the list of arguments
			va_list list;
			va_start( list, ID );
			// Call the function
			size = type->SerializeFunc( category, ID, buffer, list );
			// End the list
			va_end( list );
			// Are we buffering
			if ( _seMasterRecord.buffer ) {
				// Create a new header
				SysEventHeader header = { category, ID, size };
				// Lock the SysEvent Master Record
				_WaitSemaphore( _seMasterRecord.semaphore );

				// See if we are going to overrun the pool
				if ( _seMasterRecord.bytesRemaining < (uint32_t)size + (uint32_t)sizeof(SysEventHeader) ) {
					// Ok, make sure the overrun flag is set
					_seMasterRecord.overrun = true;
					// Clear the rest of the pool
					memset( _seMasterRecord.nextFree, 0, _seMasterRecord.bytesRemaining );
					// Clear the number of bytes remaining
					_seMasterRecord.bytesRemaining = SYSEVENT_POOLSIZE_B;
					// Start writing at the beginning again
					_seMasterRecord.nextFree = _seMasterRecord.pool;
				}

				// If we are overwriting, erase n records to make space
				if ( _seMasterRecord.overrun ) {
					// How much space is already free
					uint32_t free = _seMasterRecord.firstRecord - _seMasterRecord.nextFree;
					// Free as much room as needed
					while ( free < size + sizeof(SysEventHeader) ) {
						// How much is being freed
						uint32_t freeSize = sizeof(SysEventHeader) + ((SysEventHeader*)_seMasterRecord.firstRecord)->size;
						// Adjust firstRecord
						_seMasterRecord.firstRecord += freeSize;
						// Are there any more records before the end
						if ( _seMasterRecord.firstRecord == _seMasterRecord.pool + SYSEVENT_POOLSIZE_B ||
							((SysEventHeader*)_seMasterRecord.firstRecord)->category == NoCategory ) {
							// Adjust the firstRecord
							_seMasterRecord.firstRecord = _seMasterRecord.pool;
							
						}
						// Add freeSize to free list
						free += freeSize;
						// Reduce the in buffer count
						_seMasterRecord.inBufferCount--;
					}
				}

				// Copy the header into the pool
				memcpy( _seMasterRecord.nextFree, &header, sizeof(SysEventHeader) );
				// Increment nextFree and decrement bytesRemaining
				_seMasterRecord.nextFree += sizeof(SysEventHeader);
				_seMasterRecord.bytesRemaining -= sizeof(SysEventHeader);
				// Copy in the data
				memcpy( _seMasterRecord.nextFree, buffer, size );
				// Increment nextFree and decrement bytesRemaining
				_seMasterRecord.nextFree += size;
				_seMasterRecord.bytesRemaining -= size;

				// Increment the record count
				_seMasterRecord.recordCount++;
				// Increment the in buffer count
				_seMasterRecord.inBufferCount++;

				// Unlock the SysEvent Master Record
				_PostSemaphore( _seMasterRecord.semaphore );
			}
			// Otherwise, just print them
			else {
				// Is there a print function
				if ( type->PrintFunc != NULL ) {
					char printBuffer[256];
					// Print to the buffer
					type->PrintFunc( printBuffer, category, ID, buffer );
					// Print it to the console
					printf("%s", printBuffer);
				}
			}
		}
	}
}


/* Process masked SysEvents in the memory buffer */
void SysEventsProcess( EventCategory categoryMask ) {
}


/* Shutdown the SysEvent subsystem */
void SysEventShutdown( char* filename ) {
	SysEventHeader *header;
	char overrun[6];
	uint32_t printCount = 0;
	// Did we buffer - if not, exit
	if ( !_seMasterRecord.buffer || _seMasterRecord.recordCount == 0 ) return;
	// Show some basic info
	if ( _seMasterRecord.overrun ) strcpy( overrun, "True" );
	else strcpy( overrun, "False" );
	printf("-- SysEvents (%u Total, %u Buffered, Overrun: %s, Free: %4.1fKB) --\n",
		   _seMasterRecord.recordCount, _seMasterRecord.inBufferCount, overrun, _seMasterRecord.bytesRemaining / 1024.0 );
	// Ok, start at the beginning
	header = (SysEventHeader*)_seMasterRecord.firstRecord;
	// Loop until a termination condition is met
	while ( printCount < _seMasterRecord.inBufferCount ) {
		char *buffer;
		SysEventCategory* cat;
		SysEventType* type;
		assert ( header->category != NoCategory );
		// Get the correct SysEventCategory
		cat = _seMasterRecord.categories + header->category;
		// Get the correct SysEventType
		type = cat->typeDefinitions + header->ID;
		// Extract the data
		buffer = (char*)(header) + sizeof(SysEventHeader);
		// Is there a print function
		if ( type->PrintFunc != NULL ) {
			char printBuffer[256];
			// Print to the buffer
			type->PrintFunc( printBuffer, header->category, header->ID, buffer );
			// Print it to the console
			printf("%s", printBuffer);
		}
		// Advance the buffer pointer
		header = (SysEventHeader*)(buffer + header->size);
		// Did we overrun
		if ( _seMasterRecord.overrun && header->category == NoCategory ) {
			// Start back at the beginning
			header = (SysEventHeader*)_seMasterRecord.pool;
			
		}
		// Increment the print count
		printCount++;
	}
	// Destroy the master semaphore
	_DestroySemaphore( "_seSemaphore", _seMasterRecord.semaphore );
	// Reset the init flag
	_seInit = false;
}


/*****************************************************************************/

