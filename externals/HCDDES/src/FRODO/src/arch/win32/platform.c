/*** Included Header Files ***/
#include "arch/win32/platform.h"
#include "scheduler.h"
#include "highres_timing.h"
#include "logger.h"
#include "windows.h"


/*** Externall Defined Variables and Functions ***/
extern void (*_SchedErrorHandler)(ErrorCode,void*,pfloat_t);	
extern bool							_schedExec;	
extern Semaphore					*_schedTaskComplete;


/*****************************************************************************/


#define MAX_SEMAPHORES				1024
static Semaphore					_semaphoreArray[MAX_SEMAPHORES];
static uint16_t						_numSemaphores = 0;


/*****************************************************************************/


/* Determine correct threading priority etc. */
bool _CreateThread( pthread_t* thread, void*(*ExecFunc)(void*), void* arg, SchedThreadPriority priority ) {
	// Is this the scheduler thread
	if ( priority == SchedulerThread ) {
		// Set the affinity for the processor (proc 0)
		DWORD_PTR oldmask = SetThreadAffinityMask( GetCurrentThread(), 0 );
		// Set the thread class to high priority
		SetPriorityClass( GetCurrentProcess(), REALTIME_PRIORITY_CLASS );
		// Set the thread priority itself to real-time
		SetThreadPriority( GetCurrentProcess(), THREAD_PRIORITY_TIME_CRITICAL );
	}
	// Otherwise, create the needed thread
	else {
		HANDLE handle = CreateThread(
			NULL,									//Choose default security
			0,										//Default stack size
			(LPTHREAD_START_ROUTINE)ExecFunc,		//Routine to execute
			(LPVOID)arg,							//Thread parameter
			0,										//Immediately run the thread
			NULL									//Thread Id
							);
		// Make sure the thread has started correctly
		if ( handle == NULL ) {
			// Signal an error
			_SchedErrorHandler( FS_ERR_PERIODICCREATEFAILURE, NULL, 0.0 );
			// Return bad
			return false;
		}
		// Set the affinity for the processor (proc 0)
		SetThreadAffinityMask( handle, 0 );
		// Switch on thread priority
		switch( priority ) {
			case ErrorThread:
				SetThreadPriority( handle, THREAD_PRIORITY_HIGHEST );
				break;
			case AsyncReceiveThread:
				SetThreadPriority( handle, THREAD_PRIORITY_NORMAL );
				break;
			case TaskThread:
				SetThreadPriority( handle, THREAD_PRIORITY_ABOVE_NORMAL );
				break;
		}
	}
	// All is good
	return true;
}


/* On windows the scheduler can not preempt, so we must wait on _semaphore and
 * check one thing: The current time is greater than the start time + WCET (no preemption)
 * Arguments: SchedInstance of task being checked for overrun
 * Returns: the amount of overrun that instance of the task incurred	*/
pfloat_t _CheckTaskOverrun( SchedInstance *instance ) {
	pfloat_t overrunMS = 0.0;
	assert( instance != NULL );
	// Wait on the semaphore ( should return when task completes )
	/*** Note: Since Win32-pthreads handles Signals in a separate thread, _WaitSemaphore behaviour
			   is very different compared to on linux and osx ***/
	_WaitSemaphore( _schedTaskComplete );
	// Record this as the stop time
	overrunMS = instance->schedulable->execHalt - instance->timevalMS - instance->schedulable->wcetMS;
	// Check Case #1 - no preemption so scheduler returns from NanoSleep late
	if ( overrunMS > SCHEDULER_RESOLUTION_MS ) {
		// Signal an error has occurred
		_SchedErrorHandler( FS_ERR_TASKWCETOVERRUN, instance, 0.0 );
		// Return amount of  - means there was a WCET overrun
		return overrunMS;
	}
	// No WCET overrun here
	return 0.0;
}


/* Since OSX does not support unnamed semaphores and win32 pthreads does not
 * support named semaphores, we have to wrap the create and destroy functions	*/
Semaphore* _CreateSemaphore( char* name, uint32_t initValue ) {
	// Check a couple of values
	assert( name != NULL );
	assert( _numSemaphores < MAX_SEMAPHORES );
	// For windows we must use CreateSemaphore
	_semaphoreArray[_numSemaphores++] = CreateSemaphore( NULL, (LONG)initValue, 1000, NULL );
	// Return the good semaphore
	return _semaphoreArray[_numSemaphores-1];
}


/* Simple wrapper for posting to a semaphore */
int _PostSemaphore( Semaphore* semaphore ) {
	// Just call to the ReleaseSemaphore function
	ReleaseSemaphore( semaphore, 1, NULL );
	return 0;
}


/* Simple wrapper for waiting on a semaphore */
int _WaitSemaphore( Semaphore* semaphore ) {
	// Just call to the WaitForSingleObject function
	WaitForSingleObject( semaphore, INFINITE );
	return 0;
}


/* Close the named (or in Win32 case unnamed ) semaphore */
void _DestroySemaphore( char* name, Semaphore *semaphore ) {
	// Check some values
	assert( name != NULL );
	assert( semaphore != NULL );
	// Just close the semaphore
	CloseHandle( semaphore );
}


/*** Adapted from the curl inet_pton function ***/
int inet_pton(int af, const char *src, void *dst) {
	static const char digits[] = "0123456789";
	int saw_digit, octets, ch;
	unsigned char tmp[4], *tp;
	
	saw_digit = 0;
	octets = 0;
	tp = tmp;
	*tp = 0;
	while ((ch = *src++) != '\0') {
		const char *pch;
		if ((pch = strchr(digits, ch)) != NULL) {
			unsigned int val = *tp * 10 + (unsigned int)(pch - digits);
			if (val > 255) return (0);
			*tp = (unsigned char)val;
			if (! saw_digit) {
				if (++octets > 4) return (0);
				saw_digit = 1;
			}
		} else if (ch == '.' && saw_digit) {
			if (octets == 4) return (0);
			*++tp = 0;
			saw_digit = 0;
		} else return (0);
	}
	if (octets < 4) return (0);
	/* bcopy(tmp, dst, INADDRSZ); */
	memcpy(dst, tmp, 4);
	return (1);
}


/* Handle registered signals */
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

