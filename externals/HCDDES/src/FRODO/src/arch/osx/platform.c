/*** Included Header Files ***/
#include "arch/osx/platform.h"
#include "scheduler.h"
#include "highres_timing.h"
#include "logger.h"


/*** Externall Defined Variables and Functions ***/
extern void (*_SchedErrorHandler)(ErrorCode,void*,pfloat_t);
extern bool								_schedExec;
extern Semaphore						*_schedTaskComplete;


/*****************************************************************************/


#define MAX_SEMAPHORES					16
static semaphore_t						_semaphoreArray[MAX_SEMAPHORES];
static uint16_t							_numSemaphores = 0;


/*****************************************************************************/


/* Determine correct threading priority etc. */
bool _CreateThread( pthread_t* thread, void*(*ExecFunc)(void*), void* arg, SchedThreadPriority priority ) {
	int retVal;
	pthread_attr_t thread_attr;
	struct sched_param schedParam;
	// Is this the scheduler thread
	if ( priority == SchedulerThread ) {
		// Try setting overall process to most favorable scheduling
		setpriority( PRIO_PROCESS, 0, -20 );
		// Setup the scheduler as a "real-time" thread
		struct thread_time_constraint_policy ttcPolicy;
		// Indicate that there is no natural periodicity
		ttcPolicy.period = 0;
		// But we expect up to 100us of computation from each invokation of the scheduler
		ttcPolicy.computation = 100000L;
		// Must not be more than 200us of computation
		ttcPolicy.constraint = 200000L;
		// Don't allow anything to preempt the scheduler - never!!!
		ttcPolicy.preemptible = 0;
		// Try to enforce the real-time policy
		if ( (retVal = thread_policy_set( mach_thread_self(),
								   THREAD_TIME_CONSTRAINT_POLICY, (thread_policy_t)&ttcPolicy,
								   THREAD_TIME_CONSTRAINT_POLICY_COUNT ) ) != KERN_SUCCESS ) {
			fprintf(stderr, "set_realtime() failed.\n");
			return 0;
		}
		// Just set the thread priority ( one notch below max)
//		schedParam.sched_priority = sched_get_priority_max( SCHED_OTHER ) - 1;
//		pthread_setschedparam( pthread_self(), SCHED_OTHER, &schedParam );
	}
	// Otherwise, create the needed thread
	else {
		// Switch on thread priority
		switch( priority ) {
			case ErrorThread:
				schedParam.sched_priority = sched_get_priority_max( SCHED_OTHER );
				break;
			case AsyncReceiveThread:
				schedParam.sched_priority = sched_get_priority_max( SCHED_OTHER ) - 3;
				break;
			case TaskThread:
				schedParam.sched_priority = sched_get_priority_max( SCHED_OTHER ) - 2;
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


/* On OSX the scheduler can not preempt, so we must wait on _semaphore and
 * check one thing:
 *	1) The current time is greater than the start time + WCET (no preemption)	*/
pfloat_t _CheckTaskOverrun( SchedInstance *instance ) {
	pfloat_t overrunMS;
	// Wait on the semaphore ( should be posted when task completes )
	int retVal = _WaitSemaphore( _schedTaskComplete );
	// Was the wait successful... -1 means wait interrupted by SIGNAL
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
	// Get the next available semaphore
	semaphore_t* semaphore = _semaphoreArray + _numSemaphores++;
	kern_return_t retVal;
	// Check a couple of values
	assert( name != NULL );
	assert( _numSemaphores < MAX_SEMAPHORES );
	// For OS X we utilize mach semaphores which are supposed to be better than posix sem_t
	retVal = semaphore_create( mach_task_self(), semaphore, SYNC_POLICY_FIFO, initValue);
	// Check for valid creation
	assert( retVal == KERN_SUCCESS );
	// Return the good semaphore
	return semaphore;
}


/* Simple wrapper for posting to a semaphore */
int _PostSemaphore( Semaphore* semaphore ) {
	// Use the mach call to post
	kern_return_t retVal = semaphore_signal( *semaphore );
	if ( retVal != KERN_SUCCESS ) assert( false );
	return 0;
}


/* Simple wrapper for waiting on a semaphore */
int _WaitSemaphore( Semaphore* semaphore ) {
	// Use the mach call to wait
	kern_return_t retVal = semaphore_wait( *semaphore );
	if ( retVal != KERN_SUCCESS ) assert( false );
	return 0;
}


/* Simple wrapper for destroying a semaphore */
void _DestroySemaphore( char* name, Semaphore *semaphore ) {
	kern_return_t retVal;
	// Check some values
	assert( name != NULL );
	assert( semaphore != NULL );
	// Just destroy the semaphore (Mach call)
	retVal = semaphore_destroy( mach_task_self(), *semaphore );
	// Check for success
	assert( retVal == KERN_SUCCESS );
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

