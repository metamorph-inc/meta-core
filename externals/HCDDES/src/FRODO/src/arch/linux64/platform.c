/*** Included Header Files ***/
#include "arch/linux64/platform.h"
#include "scheduler.h"
#include "highres_timing.h"
#include "logger.h"


/*** Externall Defined Variables and Functions ***/
extern void (*_SchedErrorHandler)(ErrorCode,void*,pfloat_t);
extern bool							_schedExec;
extern Semaphore					*_schedTaskComplete;				


/*****************************************************************************/


#define MAX_SEMAPHORES				64
#define CPU_CORE_TO_USE				0
static Semaphore					_semaphoreArray[MAX_SEMAPHORES];
static uint16_t						_numSemaphores = 0;


/*****************************************************************************/


/* Determine correct threading priority etc. */
bool _CreateThread( pthread_t* thread, void*(*ExecFunc)(void*), void* arg, SchedThreadPriority priority ) {
	int retVal = 0;
	pthread_attr_t thread_attr;
	struct sched_param schedParam;
	// Is this the scheduler thread -- Set to 
	if ( priority == SchedulerThread ) {
		// Set the processor affinity (adopted from MBSHM project APEX_SCHEDULING.cc )
		cpu_set_t mask;
		int policy = -1;
		unsigned int len = sizeof( mask );
		CPU_ZERO( &mask );
		CPU_SET( CPU_CORE_TO_USE, &mask );
		// Check to see if we were successful setting the CPU affinity
		if ( sched_setaffinity( 0, len, &mask ) < 0 ) {
			// Log a non-fatal error
			_SchedErrorHandler( FS_ERR_SETAFFINITYFAIL, NULL, 0.0 );
		}
		// Get the current scheduling info
		retVal = pthread_getschedparam( pthread_self(), &policy, &schedParam );
		// Are we SCHED_FIFO
		if ( policy != SCHED_FIFO ) {
			// Set the process priority ( all the way to max)
			schedParam.sched_priority = sched_get_priority_max( SCHED_FIFO );
			retVal = sched_setscheduler( 0, SCHED_FIFO, &schedParam );
			// Where we able to set SCHED_FIFO?
			if ( retVal == -1 ) {
				// Log a fatal error (will exit program)
				_SchedErrorHandler( FS_ERR_SCHEDFIFOUNAVAILABLE, NULL, 0.0 );
			}
			// Set the priority correctly (one below max)
			schedParam.sched_priority = sched_get_priority_max( SCHED_FIFO ) - 1;
			retVal = pthread_setschedparam( pthread_self(), SCHED_FIFO, &schedParam );
			// Double check that it has been set
			retVal = pthread_getschedparam( pthread_self(), &policy, & schedParam );
			if ( schedParam.sched_priority != sched_get_priority_max( SCHED_FIFO ) - 1 ) {
				// Log a fatal error (will not exit program)
				_SchedErrorHandler( FS_ERR_SCHEDPRIORITYNOTSET, NULL, 0.0 );
			}
		}
		// Lock the memory paging system
		if ( mlockall( MCL_CURRENT | MCL_FUTURE ) == -1 ) {
			printf("mlockall failed.\n");
			exit(-2);
        }
		// Force all pages to be written to RAM (permanently)
		else {
//#define MAX_SAFE_STACK (100 * 1024 * 1024)
//			unsigned char dummy[MAX_SAFE_STACK];
//			memset( &dummy, 0, MAX_SAFE_STACK );
		}
		
	}
	// Otherwise, create the needed thread
	else {
		// Switch on thread priority
		switch( priority ) {
			case ErrorThread:
				schedParam.sched_priority = sched_get_priority_max( SCHED_FIFO );
				break;
			case AsyncReceiveThread:
				schedParam.sched_priority = sched_get_priority_max( SCHED_FIFO ) - 3;
				break;
			case TaskThread:
				schedParam.sched_priority = sched_get_priority_max( SCHED_FIFO ) - 2;
				break;
		}
		// Have the thread have a relatively low priority
		pthread_attr_init( &thread_attr );
		pthread_attr_setschedparam( &thread_attr, &schedParam );
		// Initiate thread ( it will block on the semaphore )
		retVal = pthread_create( thread, &thread_attr, ExecFunc, arg );
		// Make sure the thread has started correctly
		if ( retVal != 0 ) {
			// Signal an error
			_SchedErrorHandler( FS_ERR_PERIODICCREATEFAILURE, NULL, 0.0 );
			// Return bad
			return false;
		}
	}
	// All is good
	return true;
}


/* On Linux the scheduler can preempt, so we must figure something out here!!!
 *	1) The current time is greater than the start time + WCET (no preemption)	*/
pfloat_t _CheckTaskOverrun( SchedInstance *instance ) {
	pfloat_t overrunMS = 0.0;
	// Wait on the semaphore ( should be posted when task completes )
	int retVal = _WaitSemaphore( _schedTaskComplete );
	// Check some values
	assert( instance != NULL );
	// Was the wait successful
	if ( retVal == -1 ) {
		// Try waiting again
		_WaitSemaphore( _schedTaskComplete );
	}
	// Record this as the stop time
	overrunMS = instance->schedulable->execHalt - instance->timevalMS - instance->schedulable->wcetMS;
	// Check Case #1 - no preemption so scheduler returns from NanoSleep late
	if ( overrunMS > SCHEDULER_RESOLUTION_MS ) {
		// Signal an error has occurred
		_SchedErrorHandler( FS_ERR_TASKWCETOVERRUN, instance, overrunMS );
		// Return amount of  - means there was a WCET overrun
		return overrunMS;
	}
	// No WCET overrun here
	return 0.0;
}


/* Since OSX does not support unnamed semaphores and win32 pthreads does not
 * support named semaphores, we have to wrap the create and destroy functions	*/
Semaphore* _CreateSemaphore( char* name, uint32_t initValue ) {
	// Get the next semaphore ( and increment numSemaphores)
	Semaphore* semaphore = _semaphoreArray + _numSemaphores++;
	// Check a couple of values
	assert( name != NULL );
	assert( _numSemaphores < MAX_SEMAPHORES );
	// For windows we must use sem_init()
	sem_init( semaphore, 0, initValue );
	// Return the good semaphore
	return semaphore;
}


/* */
int _PostSemaphore( Semaphore* semaphore ) {
	// Just call to the sem_t function
	return sem_post( semaphore );
}


/* */
int _WaitSemaphore( Semaphore* semaphore ) {
	// Just call to the sem_t function
	return sem_wait( semaphore );
}


/* */
void _DestroySemaphore( char* name, Semaphore *semaphore ) {
	// Check some values
	assert( name != NULL );
	assert( semaphore != NULL );
	// Just close and unlink the semaphore
	sem_destroy( semaphore );
}


/* Handle signals gracefully */
void _HandleSignals( int signal ) {
	// Log the event
	SysEvent( Signals, 0, signal );
	// Switch on type of signal
	switch ( signal ) {
		case SIGINT:
			// All we should have to do is set _schedExec to false
			_schedExec = false;
			break;
	}
}


/*****************************************************************************/

