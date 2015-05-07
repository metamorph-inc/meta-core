/*** Copyright Vanderbilt ***/


/*** Included Header Files ***/
#include "scheduler.h"
#include "highres_timing.h"
#include "events.h"


/*****************************************************************************/


// Array size constants
#ifndef FS_MAXTASKNAMELENGTH
#define FS_MAXTASKNAMELENGTH				32							//
#endif
#ifndef FS_MAXSCHEDULABLES
#define FS_MAXSCHEDULABLES					32							//
#endif
#ifndef FS_MAXPERIODICINSTANCES
#define FS_MAXPERIODICINSTANCES				256							//
#endif
#ifndef FS_MAXSPORADICINSTANCES
#define FS_MAXSPORADICINSTANCES				256							//
#endif
#ifndef FS_MAXSYNCCHANNELS
#define FS_MAXSYNCCHANNELS					8							//
#endif


/*****************************************************************************/


/*** Type definition for synchronization channels ***/
typedef struct {
	void* channel;							//
	pfloat_t (*SyncInit)(void*);			//
	void (*SyncShutdown)(void*);			//
} SchedSyncChannel;


/*** Static Variables ***/
static SchedSchedulable			_schedulables[FS_MAXSCHEDULABLES];			// Static array of schedulable tasks
static uint8_t					_numSchedulable = 0;						// Number of schedulable tasks
SchedInstance					*_nextInstance = NULL;						// Pointer to next task instance to execute
static SchedInstance			_periodicInstances[FS_MAXPERIODICINSTANCES];// Static array of periodic task instances
static uint16_t					_nextPeriodic = 0;							// Next periodic task to execute
static uint16_t					_numPeriodic = 0;							// Number of periodic tasks
static pfloat_t(*_sporadicScheduler)(pfloat_t,pfloat_t) = NULL;				// Function pointer for sporadic scheduling
void (*_SchedErrorHandler)(ErrorCode,void*,pfloat_t) = NULL;				// Function pointer for scheduler error handling
static SchedInstance			_sporadicInstances[FS_MAXSPORADICINSTANCES];// Static array of sporadic task instances
static SchedInstance			*_sporadicList = NULL;						// Pointer to head of sporadic task linked-list
static SchedInstance			*_sporadicFree = NULL;						// Pointer to head of sporadic free linked-list
pfloat_t						_hyperperiodMS;								// Global hyperperiod length in MS
static void						*_context;									// Pointer for client defined context data
static Semaphore				*_schedSporadicMutex;						// Semaphore for mutex access to sporadic list
Semaphore						*_schedTaskComplete;						// Semaphore to signal completion of task exec
static bool						_schedHalt = false;							// Flag to signal halt of task execution
static bool						_schedInit = false;							// Flag for scheduler initialization
bool							_schedExec = false;							// Flag for scheduler executing
static SchedSyncChannel			_schedSyncChannels[FS_MAXSYNCCHANNELS];		// Static array of syncronization channels
static uint8_t					_schedNumSyncChannels = 0;					// Number of syncronization channels


/*** Externally Defined Functions and Variables ***/
extern pfloat_t _CheckTaskOverrun( SchedInstance* );
extern void _HandleSignals( int );


/*****************************************************************************/


/* ExecutionStart Handling Functions */
uint8_t _ExecutionStartSerialize( EventCategory category, EventID ID, char* buffer, va_list list );
void _ExecutionStartPrint( char* string, EventCategory category, EventID ID, char* data );
void _ExecutionStartProcess( EventCategory category, EventID ID, char* data );
/* ExecutionEnd Handling Functions */
uint8_t _ExecutionEndSerialize( EventCategory category, EventID ID, char* buffer, va_list list );
void _ExecutionEndPrint( char* string, EventCategory category, EventID ID, char* data );
void _ExecutionEndProcess( EventCategory category, EventID ID, char* data );
/* HyperperiodStart Handling Functions */
uint8_t _HyperperiodStartSerialize( EventCategory category, EventID ID, char* buffer, va_list list );
void _HyperperiodStartPrint( char* string, EventCategory category, EventID ID, char* data );
void _HyperperiodStartProcess( EventCategory category, EventID ID, char* data );
/* HyperperiodEnd Handling Functions */
uint8_t _HyperperiodEndSerialize( EventCategory category, EventID ID, char* buffer, va_list list );
void _HyperperiodEndPrint( char* string, EventCategory category, EventID ID, char* data );
void _HyperperiodEndProcess( EventCategory category, EventID ID, char* data );
/* TaskStart Handling Functions */
uint8_t _TaskStartSerialize( EventCategory category, EventID ID, char* buffer, va_list list );
void _TaskStartPrint( char* string, EventCategory category, EventID ID, char* data );
void _TaskStartProcess( EventCategory category, EventID ID, char* data );
/* TaskEnd Handling Functions */
uint8_t _TaskEndSerialize( EventCategory category, EventID ID, char* buffer, va_list list );
void _TaskEndPrint( char* string, EventCategory category, EventID ID, char* data );
void _TaskEndProcess( EventCategory category, EventID ID, char* data );
/* SporadicJobCreation Handling Functions */
uint8_t _SporadicJobCreationSerialize( EventCategory category, EventID ID, char* buffer, va_list list );
void _SporadicJobCreationPrint( char* string, EventCategory category, EventID ID, char* data );
void _SporadicJobCreationProcess( EventCategory category, EventID ID, char* data );
/* SporadicJobRemoval */
uint8_t _SporadicJobRemovalSerialize( EventCategory category, EventID ID, char* buffer, va_list list );
void _SporadicJobRemovalPrint( char* string, EventCategory category, EventID ID, char* data );
void _SporadicJobRemovalProcess( EventCategory category, EventID ID, char* data );


/*****************************************************************************/


/* Swap the data values within two FS_SchedInstances - used in sorting algorithm below */
static INLINE void _SwapInstances( SchedInstance* instA, SchedInstance* instB ) {
	// Store the A values
	SchedSchedulable*	schedulable = instA->schedulable;
	pfloat_t timevalMS = instA->timevalMS;
	// Set A values
	instA->schedulable = instB->schedulable;
	instA->timevalMS = instB->timevalMS;
	// Set B values
	instB->schedulable = schedulable;
	instB->timevalMS = timevalMS;
}


/* Sort an array of FS_SchedInstances according to their start times */
static void _SortInstances( SchedInstance* instances, uint16_t numInst ) {
	uint16_t i = 1, j = 2;
	// Case 1: Zero or one existing instances
	if ( numInst <= 1 ) return;
	// Quick and dirty gnome_sort (see http://en.wikipedia.org/wiki/Gnome_sort)
	while ( i < numInst ) {
		if ( instances[i-1].timevalMS <= instances[i].timevalMS ) {
			i = j++;
		}
		else {
			_SwapInstances( instances + i - 1, instances + i );
			i--;
			if ( i == 0 ) i = j++;
		}
	}
}


/* Create a periodic instance and add it to the array of instances */
static INLINE void _AddPeriodicInstance( SchedSchedulable* schedulable, pfloat_t exectimeMS ) {
	SchedInstance* instance = NULL;
	// Check values
	assert( _periodicInstances != NULL );
	assert( _numPeriodic != FS_MAXPERIODICINSTANCES );
	assert( schedulable != NULL);
	assert( exectimeMS >= 0.0 );
	// Get the next available periodic instance (and increment _numPeriodic)
	instance = _periodicInstances + _numPeriodic++;
	// Set the structure values
	instance->schedulable = schedulable;
	instance->timevalMS = exectimeMS;
}


/* Free a sporadic instance from the linked list	*/
static void _PruneSporadic( void ) {
	SchedInstance *index = _sporadicList, *lag = NULL, *next = NULL;
	// Make sure there are instances to look at
	if ( _sporadicList == NULL ) return;
	// Lock the scheduler
	_WaitSemaphore( _schedSporadicMutex );
	// Look through the instance list
	do {
		// If deadline will be missed free the instance
		if ( index->timevalMS < _hyperperiodMS ) {
			// Signal a task has been pruned
			_SchedErrorHandler( FS_ERR_SPORADICDEADLINEMISS, index->schedulable, GetTimeMS() );
			// Set the next item to be assessed
			next = index->next;
			// Lag == NULL means we are removing first in list
			if ( lag == NULL ) _sporadicList = index->next;
			// Otherwise somewhere not first in list (maybe last)
			else lag->next = index->next;
			// And, add it to the free list
			index->next = _sporadicFree;
			_sporadicFree = index;
			// Move to the next instance
			index = next;
		}
		// Otherwise...
		else {
			// Adjust the deadline of the instance
			index->timevalMS -= _hyperperiodMS;
			// Move to the next instance
			lag = index;
			index = index->next;
		}
	}
	// Continue on through the end of the sporadic list
	while ( index != NULL );
	// Unlock the scheduler
	_PostSemaphore( _schedSporadicMutex );
}


/* Find the sporadic task with the highest priority and fastest
 * max frequency that is valid and fits in the amount of slack time			*/
static pfloat_t _ScheduleRateMonotonic( pfloat_t currentTimeMS, pfloat_t slackTimeMS ) {
	// Start at the beginning of the sporadic list
	SchedInstance *index = _sporadicList, *mark = NULL, *lag = NULL, *markLag = NULL;
	// Are there any sporadic tasks...
	if ( _sporadicList == NULL || slackTimeMS <= 0.0 ) return -1.0;
	// Adjust slacktime for the Platform resolution
	slackTimeMS += SCHEDULER_RESOLUTION_MS;
	// Loop through the remainder of the sporadic list
	do {
		// Check absolute deadline, (WCET vs. slack)
		if ( index->timevalMS > currentTimeMS + index->schedulable->wcetMS &&
			index->schedulable->wcetMS <= slackTimeMS ) {
			// If no current mark, check relative priority and then frequency
			if ( mark == NULL ||
			   ( index->schedulable->priority >= mark->schedulable->priority &&
				 index->schedulable->frequencyHZ > mark->schedulable->frequencyHZ ) ) {
				// Set the mark and the markLag
				markLag = lag;
				mark = index;
			}
		}
		// Move to the next instance
		lag = index;
		index = index->next;
	// Loop until end of list
	} while ( index != NULL );
	// Did we not find a candidate - return -1.0
	if ( mark == NULL ) return -1.0;
	// Found a candidate - mark the task for execution
	_nextInstance = mark;
	// Remove the instance from the list (markLag == NULL means first in list)
	if ( markLag == NULL ) _sporadicList = mark->next;
	// markLag != null means not first in list ( may be last )
	else markLag->next = mark->next;
	// And, add it to the free list
	mark->next = _sporadicFree;
	_sporadicFree = mark;
	// Execute the task in just a little while
	return currentTimeMS + SCHEDULER_RESOLUTION_MS;
}


/* Find the sporadic task with the highest priority and nearest
 * deadline that is valid and fits in the amount of slack time			*/
static pfloat_t _ScheduleEarliestDeadline( pfloat_t currentTimeMS, pfloat_t slackTimeMS ) {
	// Start at the beginning of the sporadic list
	SchedInstance *index = _sporadicList, *mark = NULL, *lag = NULL, *markLag = NULL;
	// Are there any sporadic tasks...
	if ( _sporadicList == NULL || slackTimeMS <= 0.0 ) return -1.0;
	// Adjust slacktime for the Platform resolution
//	slackTimeMS += SCHEDULER_RESOLUTION_MS;
	// Loop through the remainder of the sporadic list
	do {
		// Check absolute deadline, (WCET vs. slack)
		if ( index->timevalMS > currentTimeMS + index->schedulable->wcetMS &&
			index->schedulable->wcetMS <= slackTimeMS ) {
			// If no current mark, or check relative priority and then deadline
			if ( mark == NULL ||
			   ( index->schedulable->priority >= mark->schedulable->priority &&
				 index->timevalMS < mark->timevalMS ) ) {
				// Set the mark and the markLag
				markLag = lag;
				mark = index;
			}
		}
		// Move to the next instance
		lag = index;
		index = index->next;
	// Loop until end of list
	} while ( index != NULL );
	// Did we not find a candidate - return -1.0
	if ( mark == NULL ) return -1.0;
	// Found a candidate - mark the task for execution
	_nextInstance = mark;
	// Remove the instance from the list (markLag == NULL means first in list)
	if ( markLag == NULL ) _sporadicList = mark->next;
	// markLag != null means not first in list ( may be last )
	else markLag->next = mark->next;
	// And, add it to the free list
	mark->next = _sporadicFree;
	_sporadicFree = mark;
	// Execute the task in just a little while
	return currentTimeMS + SCHEDULER_RESOLUTION_MS;
}


/* Determine which task should execute next - return the time
   when the task should be released	and a pointer to appropriate
   (or NULL) schedInstance
 * Arguments: None
 * Returns: pfloat_t for next event time and setting global _nextInstance variable */
static pfloat_t _GetNextTask( void ) {
	SchedInstance* nextInstance = NULL;
	// First, how much slack time is available
	pfloat_t slackTimeMS = 0.0, currentTimeMS = GetTimeMS();
	// A few checks
	assert( _nextPeriodic <= _numPeriodic );
	// Are there no more periodic tasks?
	if ( _nextPeriodic == _numPeriodic ) {
		// How much time remains in the hyperperiod
		slackTimeMS = _hyperperiodMS - currentTimeMS;
	}
	// Otherwise...
	else {
		// How much time until the next periodic instance
		nextInstance = _periodicInstances + _nextPeriodic;
		slackTimeMS = nextInstance->timevalMS - currentTimeMS;
	}
	// Lock the sporadic scheduler (can't be certain no sporadic are executing)
	_WaitSemaphore( _schedSporadicMutex );
	// Now see if a sporadic task should be executed first (-1 return means no sporadic)
	slackTimeMS = _sporadicScheduler( currentTimeMS, slackTimeMS );
	// Unlock the sporadic scheduler
	_PostSemaphore( _schedSporadicMutex );
	// See if sporadic task has been found - return its execution start time
	if ( slackTimeMS != -1.0 ) return slackTimeMS;
	// Ok, so no sporadic tasks - check for end of hyperperiod
	if ( _nextPeriodic == _numPeriodic ) {
		// No next task, so set _nextInstance to NULL
		_nextInstance = NULL;
		// Signal that end of hyperperiod has been reached
		return _hyperperiodMS;
	}
	// Must have next task instance
	assert( nextInstance != NULL );
	// We will use the next periodic, so advance the index
	_nextPeriodic++;
	// Set the task to be run
	_nextInstance = nextInstance;
	// Return the time to the next task
	return nextInstance->timevalMS;
}


/* Coordinate start of execution using Sync channels
 * Arguments: None
 * Returns: pfloat_t of delay value from last channel called	*/
static pfloat_t _WaitForSync( void ) {
	uint8_t i = 0;
	// Init delayMS to zero incase there are no sync channels
	pfloat_t delayMS = 0.0;
	SchedSyncChannel* channel = NULL;
	// Loop through the syncronization channels
	for ( ; i < _schedNumSyncChannels; i++ ) {
		channel = _schedSyncChannels + i;
		// Call the channel to wait for sync
		delayMS = channel->SyncInit( channel->channel );
	}
	// For now just return the last delay value (should really be an average or something)
	return delayMS;
}


/* Coordinate stop of execution using Sync channels
 * Arguments: None
 * Returns: None	*/
static void _HaltSync( void ) {
	uint8_t i = 0;
	SchedSyncChannel* channel = NULL;
	// Loop through the syncronization channels
	for ( ; i < _schedNumSyncChannels; i++ ) {
		channel = _schedSyncChannels + i;
		// Call the channel to shutdown sync
		channel->SyncShutdown( channel->channel );
	}
}


/* Register a new Sync channel for start/stop coordination
 * Arguments: channel to register, function for channel init, function for channel shutdown
 * Returns: None	*/
void _SchedAddSyncChannel( void* channel, pfloat_t (*SyncInit)(void*), void (*SyncShutdown)(void*) ) {
	SchedSyncChannel* syncChannel = _schedSyncChannels + _schedNumSyncChannels++;
	// Check some values
	assert( _schedNumSyncChannels != FS_MAXSYNCCHANNELS );
	assert( channel != NULL );
	assert( SyncInit != NULL );
	// Set up the structure
	syncChannel->channel = channel;
	syncChannel->SyncInit = SyncInit;
	syncChannel->SyncShutdown = SyncShutdown;
}


/*****************************************************************************/


/* Initialize the entire FRODO scheduler */
bool SchedInitialize( SchedSporadicType schedulerType, void (*ErrorHandler)(ErrorCode,void*,pfloat_t),
				   pfloat_t hyperperiodMS, uint8_t maxParallelTasks, void* context	) {
	uint16_t i = 1;
	// Do some value checks
	assert( schedulerType == RateMonotonic || schedulerType == EarliestDeadlineFirst );
	assert( ErrorHandler != NULL );
	assert( hyperperiodMS > 0.0 );
	assert( context != NULL );
	// Register the SysEvent Category and Event handlers
	SysEventRegisterCategory( "Scheduler", Scheduler, 9 );
	SysEventRegister( Scheduler, ExecutionStart, _ExecutionStartSerialize, _ExecutionStartPrint, NULL );
	SysEventRegister( Scheduler, ExecutionEnd, _ExecutionEndSerialize, _ExecutionEndPrint, NULL );
	SysEventRegister( Scheduler, HyperperiodStart, _HyperperiodStartSerialize, _HyperperiodStartPrint, NULL );
//	SysEventRegister( Scheduler, HyperperiodEnd, _HyperperiodEndSerialize, _HyperperiodEndPrint, NULL );
	SysEventRegister( Scheduler, TaskStart, _TaskStartSerialize, _TaskStartPrint, NULL );
//	SysEventRegister( Scheduler, TaskEnd, _TaskEndSerialize, _TaskEndPrint, NULL );
	SysEventRegister( Scheduler, SporadicJobCreation, _SporadicJobCreationSerialize, _SporadicJobCreationPrint, NULL );
	SysEventRegister( Scheduler, SporadicJobRemoval, _SporadicJobRemovalSerialize, _SporadicJobRemovalPrint, NULL );
	// Set the error handler
	_SchedErrorHandler = ErrorHandler;
	// Initialize the list of free sporadic instances
	for ( ; i < FS_MAXSPORADICINSTANCES; i++ ) {
		// Create the linked-list of free instances
		_sporadicInstances[i-1].next = _sporadicInstances + i;
		// Make sure to mark the last as NULL
		_sporadicInstances[i].next = NULL;
	}
	// Set the pointer for the start of the free list
	_sporadicFree = _sporadicInstances;
	// Set the sporadic scheduler
	switch( schedulerType ) {
		case RateMonotonic: _sporadicScheduler = _ScheduleRateMonotonic; break;
		case EarliestDeadlineFirst: _sporadicScheduler = _ScheduleEarliestDeadline; break;
	}
	// Initialize the sporadic mutex semaphore (initially set to allow access to the sporadic scheduler)
	_schedSporadicMutex = _CreateSemaphore( "_schedSporadicSemaphore", 1 );
	// Initialize the task completion semaphore (initially set to halt a wait)
	_schedTaskComplete = _CreateSemaphore( "_schedTaskComplete", 0 );
	// Set the hyperperiod
	_hyperperiodMS = hyperperiodMS;
	// Have the thread have a relatively high priority (only Error thread may be higher)
	_InitThread( SchedulerThread );
	// Setup SIGNAL handler
	(void) signal( SIGINT, _HandleSignals );
	// Record the user defined pointer values
	_context = context;
	// Mark as initialized
	_schedInit = true;
	// All is good for now
	return true;
}


/* Create a periodic task with a set in-hyperperiod execution schedule */
bool SchedCreatePeriodicTask( char* name, void*(*ExecFunc)(void*),	SchedSchedulable** schedulable,
						   pfloat_t wcetMS, uint32_t restrictions, uint8_t instanceCount, pfloat_t* instances ) {
	uint8_t i = 0;
	// Make sure parameters are valid
	assert( _schedInit );
	assert( _numSchedulable != FS_MAXSCHEDULABLES );
	assert( name != NULL );
	assert( ExecFunc != NULL );
	assert( wcetMS >= 0.0f );
	assert( instanceCount >= 1 );
	assert( instances != NULL );
	if ( strlen( name ) >= FS_MAXTASKNAMELENGTH ) {
		printf("%s task name is too long.\n", name );
		exit( 0 );
	}
	// Get pointer to first free schedulable
	*schedulable = _schedulables + _numSchedulable;
	// Increment the number of schedulables
	_numSchedulable++;
	// Set structure values
	strncpy( (*schedulable)->name, name, FS_MAXTASKNAMELENGTH );
	// Make sure the name is terminated
	(*schedulable)->name[FS_MAXTASKNAMELENGTH-1] = '\0';
	(*schedulable)->ExecFunc = ExecFunc;
	(*schedulable)->wcetMS = wcetMS;
	(*schedulable)->restrictions = restrictions;
	// Initialize the task semaphore
	(*schedulable)->semaphore = _CreateSemaphore( name, 0 );
	// Loop through instances...
	for ( ; i < instanceCount; i++ ) {
		// Check that requested exec time + WCET does not exceed hyperperiod
		if ( wcetMS + instances[i] > _hyperperiodMS ) {
			// Log that this has happened
			char buffer[256];
			sprintf( buffer, "Requested periodic WCET (%4.3f) + exec time (%4.3f) is greater than hyperperiod %4.3f.\n",
				wcetMS, instances[i], _hyperperiodMS );
			SysEvent( Error, FS_ERR_EXECTIMEOUTSIDEHYPERPERIOD, buffer );
		}	
		// Otherwise, add instances into the instance list
		else _AddPeriodicInstance( *schedulable, instances[i] );
	}
	// Have the thread have a relatively low priority
	return _CreateThread( &((*schedulable)->thread), ExecFunc, _context, TaskThread );
}


/* Create a sporadic task - invocations will be created dynamically */
bool SchedCreateSporadicTask( char* name, void*(*ExecFunc)(void*), SchedSchedulable** schedulable,
				   pfloat_t wcetMS, uint32_t restrictions, uint8_t priority, uint16_t frequencyHZ ) {
	// Make sure parameters are valid
	assert( _schedInit );
	assert( _numSchedulable != FS_MAXSCHEDULABLES );
	assert( name != NULL );
	assert( ExecFunc != NULL );
	assert( wcetMS >= 0.0f );
	// Get pointer to first free schedulable (and increment _numSchedulable)
	*schedulable = _schedulables + _numSchedulable++;
	// Set structure values
	strncpy( (*schedulable)->name, name, FS_MAXTASKNAMELENGTH );
	// Make sure the name is terminated
	(*schedulable)->name[FS_MAXTASKNAMELENGTH-1] = '\0';
	(*schedulable)->ExecFunc = ExecFunc;
	(*schedulable)->wcetMS = wcetMS;
	(*schedulable)->restrictions = restrictions;
	(*schedulable)->priority = priority;
	(*schedulable)->frequencyHZ = frequencyHZ;
	// Initialize the task semaphore
	(*schedulable)->semaphore = _CreateSemaphore( name, 0 );
	// Have the thread have a relatively low priority
	return _CreateThread( &((*schedulable)->thread), ExecFunc, _context, TaskThread );
}


/* Create an instance of a sporadic task */
bool SchedCreateJob( SchedSchedulable* schedulable, pfloat_t relativeDeadlineMS ) {
	SchedInstance* instance;
	// Make sure parameters are valid
	assert( _schedInit );
	assert( _sporadicFree != NULL );
	assert( schedulable != NULL );
	assert( relativeDeadlineMS >= 0.0 );
	// Lock the scheduler
	_WaitSemaphore( _schedSporadicMutex );
	// Get the first available free sporadic instance
	instance = _sporadicFree;
	// Set the free list head to the next free
	_sporadicFree = _sporadicFree->next;
	// Add the allocated instance to the sporadic list
	instance->next = _sporadicList;
	_sporadicList = instance;
	// Set the structure values
	instance->schedulable = schedulable;
	// Make sure to adjust deadline to be relative to start of hyperperiod
	instance->timevalMS = GetTimeMS() + relativeDeadlineMS;
	// Record the event
	SysEvent( Scheduler, SporadicJobCreation, schedulable );
	// Unlock the scheduler
	_PostSemaphore( _schedSporadicMutex );
	// All is good
	return true;
}


/* Execute the scheduler for X hyperperiods */
bool SchedExecute( uint32_t numHyperperiods ) {
	pfloat_t taskStartTimeMS, desiredMS;
	uint32_t hyperperiod = 0;
	bool hyperperiodStart = true;
	// Check some values
	assert( _schedInit );
	// Sort the periodic instances array according to exectime
	_SortInstances( _periodicInstances, _numPeriodic );
	// Mark the start of execution
	_schedExec = true;
	// Syncronize with other nodes and prepare to start execution
	desiredMS = _WaitForSync( );
	// Log the start of execution
	SysEvent( Scheduler, ExecutionStart );
	// Cycle forever, or until _schedExec is set to false
	while ( _schedExec ) {
		// Is this the start of a hyperperiod
		if ( hyperperiodStart ) {
			// Log the start of the hyperperiod
			SysEvent( Scheduler, HyperperiodStart );
			// Clear the start flag
			hyperperiodStart = false;
			// Clear the clock (first time desired == Sync delay, then hyperperiod for the remainder)
			ZeroTime( desiredMS );
			// Set desiredMS to hyperperiodMS (see previous comment)
			desiredMS = _hyperperiodMS;
			// Find the next task
			taskStartTimeMS = _GetNextTask();
		}
		// Otherwise, execute the ready task
		else {
			// If there is a next task ready to run
			if ( _nextInstance != NULL ) {
				// Set the schedulable instance
				_nextInstance->schedulable->execInst = _nextInstance;
				// Signal the task to run, only takes a post to the task semaphore			
				_PostSemaphore( _nextInstance->schedulable->semaphore );
				// Check for WCET overrun
				_CheckTaskOverrun( _nextInstance );
			}
			// Find the next task
			taskStartTimeMS = _GetNextTask();
			// Double check for end of hyperperiod
			if ( taskStartTimeMS == _hyperperiodMS ) {
				// Prune out unreachable sporadic instances
				_PruneSporadic();
				// Increment the hyperperiod count
				hyperperiod++;
				// Check for number of hyperperiods ending the execution
				if ( hyperperiod == numHyperperiods && numHyperperiods != 999999 ) _schedExec = false;
				// Mark the start of a hyperperiod
				hyperperiodStart = true;
				// Reset the periodic counter
				_nextPeriodic = 0;
				// Log the start of the hyperperiod
				SysEvent( Scheduler, HyperperiodEnd );
			}
		}
		// Sleep until the next known event (task execution)
		NanoSleep( taskStartTimeMS );
	}
	// All is good, a failure would result in exit() or call to error handler
	SysEvent( Scheduler, ExecutionEnd );
	// Inform other nodes about end of execution
	_HaltSync( );
	// We are all done
	return true;
}


/* Signal that the schedulable task has started executing */
void SchedSignalExecution( SchedSchedulable* schedulable ) {
	// Check some values
	assert( schedulable != NULL );
	// Wait on the semaphore to execute
	_WaitSemaphore( schedulable->semaphore );
	// Check to exit
	if ( _schedHalt ) {
		// Close and unlink the thread semaphore
		_DestroySemaphore( schedulable->name, schedulable->semaphore );
		// Exit the function and end the thread
		pthread_exit( NULL );
	}
	// Set the time at the start of execution
	schedulable->execInit = GetTimeMS();
	// Log the event (pass the schedInstance - can get to schedulable from there)
	SysEvent( Scheduler, TaskStart, schedulable );
}


/* Signal that the schedulable task has finished executing */
void SchedSignalCompletion( SchedSchedulable* schedulable ) {
	// Check some values
	assert( _schedInit );
	assert( schedulable != NULL );
	// Set the time at the end of execution
	schedulable->execHalt = GetTimeMS();
	// Log the event
	SysEvent( Scheduler, TaskEnd, schedulable );
	// The only thing to do is post to the scheduler semaphore
	_PostSemaphore( _schedTaskComplete );
}


/* Shutdown the scheduler and all task threads */
bool SchedShutdown( void ) {
	uint16_t i = 0;
	// If not initialized, nothing to do
	if ( !_schedInit ) return true;
	// Signal all tasks to stop by setting _schedHalt to true
	_schedHalt = true;
	// Loop through all schedulables
	for ( ; i < _numSchedulable; i++ ) {
		// Signal tasks to "run" - they should kill themselves when they see _semaphore == 2
		_PostSemaphore( _schedulables[i].semaphore );
		// Wait for tasks to kill its thread
		pthread_join( _schedulables[i].thread, NULL );
	}
	// Check to see if any sporadic jobs still exist
	if ( _sporadicList != NULL ) {
		// Log the pruning of these jobs
		SchedInstance* index = _sporadicList;
		do {
			SysEvent( Scheduler, SporadicJobRemoval, index );
			// Move to the next instance
			index = index->next;
		} while ( index != NULL );
	}
	// Destroy the two global scheduler semaphores
	_DestroySemaphore( "_schedSporadicMutex", _schedSporadicMutex );
	_DestroySemaphore( "_schedTaskComplete", _schedTaskComplete );
	// All done
	return _schedHalt;
}


/*****************************************************************************/


/* Serialize an ExecutionStart event */
uint8_t _ExecutionStartSerialize( EventCategory category, EventID ID, char* buffer, va_list list ) {
	// Just looking for the absolute time
	pfloat_t absoluteTimeMS = AbsoluteTimeMS() ;
	// Copy the value into the buffer
	memcpy( buffer, &absoluteTimeMS, sizeof(pfloat_t) );
	// Return the number of bytes written into the buffer
	return sizeof(pfloat_t);
}


/* Print an ExecutionStart event */
void _ExecutionStartPrint( char* string, EventCategory category, EventID ID, char* data ) {
	pfloat_t absoluteTimeMS;
	// Retrieve the value from the buffer
	memcpy( &absoluteTimeMS, data, sizeof(pfloat_t) );
	// Print to the string
	sprintf( string, "%4.3f: Beginning execution.\n", absoluteTimeMS );
}


/* Process an ExecutionStart event */
void _ExecutionStartProcess( EventCategory category, EventID ID, char* data ) {
}


/*****************************************************************************/


/* Serialize an ExecutionEnd event */
uint8_t _ExecutionEndSerialize( EventCategory category, EventID ID, char* buffer, va_list list ) {
	// Just looking for the absolute time
	pfloat_t absoluteTimeMS = AbsoluteTimeMS() ;
	// Copy the value into the buffer
	memcpy( buffer, &absoluteTimeMS, sizeof(pfloat_t) );
	// Return the number of bytes written into the buffer
	return sizeof(pfloat_t);
}


/* Print an ExecutionEnd event */
void _ExecutionEndPrint( char* string, EventCategory category, EventID ID, char* data ) {
	pfloat_t absoluteTimeMS;
	// Retrieve the value from the buffer
	memcpy( &absoluteTimeMS, data, sizeof(pfloat_t) );
	// Print to the string
	sprintf( string, "%4.3f: Ending execution.\n", absoluteTimeMS );
}


/* Process an ExecutionEnd event */
void _ExecutionEndProcess( EventCategory category, EventID ID, char* data ) {
}


/*****************************************************************************/


/* Serialize an HyperperiodStart event */
uint8_t _HyperperiodStartSerialize( EventCategory category, EventID ID, char* buffer, va_list list ) {
	// Just looking for the absolute clock time
	pfloat_t timeMS = AbsoluteTimeMS() ;
	// Copy the value into the buffer
	memcpy( buffer, &timeMS, sizeof(pfloat_t) );
	// Get the in-hyperperiod time
	timeMS = GetTimeMS();
	// Write the in-hyperperiod time
	memcpy( buffer + sizeof(pfloat_t), &timeMS, sizeof(pfloat_t) );
	// Return the number of bytes written into the buffer
	return sizeof(pfloat_t) * 2;
}


/* Print an HyperperiodStart event */
void _HyperperiodStartPrint( char* string, EventCategory category, EventID ID, char* data ) {
	pfloat_t absoluteTimeMS, relativeTimeMS;
	// Retrieve the values from the buffer
	memcpy( &absoluteTimeMS, data, sizeof(pfloat_t) );
	memcpy( &relativeTimeMS, data + sizeof(pfloat_t), sizeof(pfloat_t) );
	// Print to the string
	sprintf( string, "%4.3f: HyperPeriod Commencing (@%4.3f) ---\n", absoluteTimeMS, relativeTimeMS );
}


/* Process an HyperperiodStart event */
void _HyperperiodStartProcess( EventCategory category, EventID ID, char* data ) {
}


/*****************************************************************************/


/* Serialize an HyperperiodEnd event */
uint8_t _HyperperiodEndSerialize( EventCategory category, EventID ID, char* buffer, va_list list ) {
	// Just looking for the absolute clock time
	pfloat_t timeMS = AbsoluteTimeMS() ;
	// Copy the value into the buffer
	memcpy( buffer, &timeMS, sizeof(pfloat_t) );
	// Get the in-hyperperiod time
	timeMS = GetTimeMS();
	// Write the in-hyperperiod time
	memcpy( buffer + sizeof(pfloat_t), &timeMS, sizeof(pfloat_t) );
	// Return the number of bytes written into the buffer
	return sizeof(pfloat_t) * 2;
}


/* Print an HyperperiodEnd event */
void _HyperperiodEndPrint( char* string, EventCategory category, EventID ID, char* data ) {
	pfloat_t absoluteTimeMS, relativeTimeMS;
	// Retrieve the value from the buffer
	memcpy( &absoluteTimeMS, data, sizeof(pfloat_t) );
	memcpy( &relativeTimeMS, data + sizeof(pfloat_t), sizeof(pfloat_t) );
	// Print to the string
	sprintf( string, "%4.3f: HyperPeriod Concluding ---\n", absoluteTimeMS, relativeTimeMS );
}


/* Process an HyperperiodEnd event */
void _HyperperiodEndProcess( EventCategory category, EventID ID, char* data ) {
}


/*****************************************************************************/


/* Serialize an TaskStart event */
uint8_t _TaskStartSerialize( EventCategory category, EventID ID, char* buffer, va_list list ) {
	// Just looking for the absolute clock time
	pfloat_t absoluteTimeMS = AbsoluteTimeMS();
	// Get the scheduler from the va_list
	SchedSchedulable *schedulable = va_arg( list, SchedSchedulable* );
	SchedInstance *schedInstance = schedulable->execInst;
	// Copy the values into the buffer
	memcpy( buffer, &absoluteTimeMS, sizeof(pfloat_t) );
	buffer += sizeof(pfloat_t);
	memcpy( buffer, &schedulable, sizeof(SchedSchedulable*) );
	buffer += sizeof(SchedSchedulable*);
	memcpy( buffer, &(schedulable->execInit), sizeof(pfloat_t) );
	buffer += sizeof(pfloat_t);
	memcpy( buffer, &(schedInstance->timevalMS), sizeof(pfloat_t) );
	// Return the number of bytes written into the buffer
	return sizeof(pfloat_t) * 3 + sizeof(SchedSchedulable*);
}


/* Print an TaskStart event */
void _TaskStartPrint( char* string, EventCategory category, EventID ID, char* data ) {
	pfloat_t absoluteTimeMS, execStartMS, expectStartMS;
	SchedSchedulable *schedulable;
	// Retrieve the values from the buffer
	memcpy( &absoluteTimeMS, data, sizeof(pfloat_t) );
	data += sizeof(pfloat_t);
	memcpy( &schedulable, data, sizeof(SchedSchedulable*) );
	data += sizeof(SchedSchedulable*);
	memcpy( &execStartMS, data, sizeof(pfloat_t) );
	data += sizeof(pfloat_t);
	memcpy( &expectStartMS, data, sizeof(pfloat_t) );
	// Print to the string
	sprintf( string, "%4.3f:\t%s Commencing (%4.3f vs %4.3f = %4.3f).\n",
			absoluteTimeMS, schedulable->name, execStartMS, expectStartMS, execStartMS - expectStartMS );
}


/* Process an TaskStart event */
void _TaskStartProcess( EventCategory category, EventID ID, char* data ) {
}


/*****************************************************************************/


/* Serialize an TaskEnd event */
uint8_t _TaskEndSerialize( EventCategory category, EventID ID, char* buffer, va_list list ) {
	// Just looking for the absolute clock time
	pfloat_t timeMS = AbsoluteTimeMS();
	// Get the scheduler from the va_list
	SchedSchedulable *schedulable = va_arg( list, SchedSchedulable* );
	SchedInstance *schedInstance = schedulable->execInst;
	// Copy the values into the buffer
	memcpy( buffer, &timeMS, sizeof(pfloat_t) );
	buffer += sizeof(pfloat_t);
	memcpy( buffer, &schedulable, sizeof(SchedSchedulable*) );
	buffer += sizeof(SchedSchedulable*);
	memcpy( buffer, &(schedulable->execHalt), sizeof(pfloat_t) );
	buffer += sizeof(pfloat_t);
	timeMS = schedInstance->timevalMS + schedulable->wcetMS;
	memcpy( buffer, &timeMS, sizeof(pfloat_t) );
	// Return the number of bytes written into the buffer
	return sizeof(pfloat_t) * 3 + sizeof(SchedSchedulable*);
}


/* Print an TaskEnd event */
void _TaskEndPrint( char* string, EventCategory category, EventID ID, char* data ) {
	pfloat_t absoluteTimeMS, execStartMS, expectStartMS;
	SchedSchedulable *schedulable;
	// Retrieve the values from the buffer
	memcpy( &absoluteTimeMS, data, sizeof(pfloat_t) );
	data += sizeof(pfloat_t);
	memcpy( &schedulable, data, sizeof(SchedSchedulable*) );
	data += sizeof(SchedSchedulable*);
	memcpy( &execStartMS, data, sizeof(pfloat_t) );
	data += sizeof(pfloat_t);
	memcpy( &expectStartMS, data, sizeof(pfloat_t) );
	// Print to the string
	sprintf( string, "%4.3f:\t%s Concluding (%4.3f vs %4.3f : %4.3f).\n",
			absoluteTimeMS, schedulable->name, execStartMS, expectStartMS, execStartMS - expectStartMS );
}


/* Process an TaskEnd event */
void _TaskEndProcess( EventCategory category, EventID ID, char* data ) {
}


/*****************************************************************************/


/* Serialize an SporadicJobCreation event */
uint8_t _SporadicJobCreationSerialize( EventCategory category, EventID ID, char* buffer, va_list list ) {
	// Just looking for the absolute clock time
	pfloat_t timeMS = AbsoluteTimeMS();
	// Get the scheduler from the va_list
	SchedSchedulable *schedulable = va_arg( list, SchedSchedulable* );
	// Copy the values into the buffer
	memcpy( buffer, &timeMS, sizeof(pfloat_t) );
	buffer += sizeof(pfloat_t);
	memcpy( buffer, &schedulable, sizeof(SchedSchedulable*) );
	buffer += sizeof(SchedSchedulable*);	// Get the in-hyperperiod time
	timeMS = GetTimeMS();
	// Write the in-hyperperiod time
	memcpy( buffer, &timeMS, sizeof(pfloat_t) );
	// Return the number of bytes written into the buffer
	return sizeof(pfloat_t) * 2 + sizeof(SchedSchedulable*);
}


/* Print an SporadicJobCreation event */
void _SporadicJobCreationPrint( char* string, EventCategory category, EventID ID, char* data ) {
	pfloat_t absoluteTimeMS, intervalTimeMS;
	SchedSchedulable *schedulable;
	// Retrieve the values from the buffer
	memcpy( &absoluteTimeMS, data, sizeof(pfloat_t) );
	data += sizeof(pfloat_t);
	memcpy( &schedulable, data, sizeof(SchedSchedulable*) );
	data += sizeof(SchedSchedulable*);
	memcpy( &intervalTimeMS, data, sizeof(pfloat_t) );
	// Print to the string
	sprintf( string, "%4.3f:\t%s Sporadic Job Created (@%4.3f).\n", absoluteTimeMS, schedulable->name, intervalTimeMS );
}


/* Process an SporadicJobCreation event */
void _SporadicJobCreationProcess( EventCategory category, EventID ID, char* data ) {
}


/*****************************************************************************/


/* Serialize an SporadicJobRemoval event */
uint8_t _SporadicJobRemovalSerialize( EventCategory category, EventID ID, char* buffer, va_list list ) {
	// Just looking for the absolute clock time
	pfloat_t timeMS = AbsoluteTimeMS();
	// Get the scheduler from the va_list
	SchedSchedulable *schedulable = va_arg( list, SchedSchedulable* );
	// Copy the values into the buffer
	memcpy( buffer, &timeMS, sizeof(pfloat_t) );
	buffer += sizeof(pfloat_t);
	memcpy( buffer, &schedulable, sizeof(SchedSchedulable*) );
	buffer += sizeof(SchedSchedulable*);	// Get the in-hyperperiod time
	timeMS = GetTimeMS();
	// Write the in-hyperperiod time
	memcpy( buffer + sizeof(pfloat_t), &timeMS, sizeof(pfloat_t) );
	// Return the number of bytes written into the buffer
	return sizeof(pfloat_t) * 2 + sizeof(SchedSchedulable*);
}


/* Print an SporadicJobRemoval event */
void _SporadicJobRemovalPrint( char* string, EventCategory category, EventID ID, char* data ) {
	pfloat_t absoluteTimeMS, intervalTimeMS;
	SchedSchedulable *schedulable;
	// Retrieve the values from the buffer
	memcpy( &absoluteTimeMS, data, sizeof(pfloat_t) );
	data += sizeof(pfloat_t);
	memcpy( &schedulable, data, sizeof(SchedSchedulable*) );
	data += sizeof(SchedSchedulable*);
	memcpy( &intervalTimeMS, data, sizeof(pfloat_t) );
	// Print to the string
	sprintf( string, "%4.3f:\t%s Sporadic Job Pruned (@%4.3f).\n", absoluteTimeMS, schedulable->name, intervalTimeMS );
}


/* Process an SporadicJobRemoval event */
void _SporadicJobRemovalProcess( EventCategory category, EventID ID, char* data ) {
}


/*****************************************************************************/


