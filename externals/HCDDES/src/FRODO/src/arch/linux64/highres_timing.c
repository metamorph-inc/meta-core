/*** Included Header Files ***/
#include "scheduler.h"
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <time.h>


/*****************************************************************************/


#define HRT_MAXOFFSETS					8
#define HRT_OFFSETGAIN					0.5


// Static Variables
static bool								_hrtInitialized = false;
static struct timespec					_startAT;
static struct timespec					_initAT;
static bool								_hrtFirstHyperperiod = true;
static double							_hrtOffsets[HRT_MAXOFFSETS];
static uint8_t							_hrtNumOffsets = 0;
static double							_hrtGain = 1.00;


/*** Externally Defined Variables and Functions ***/
extern SchedInstance					*_nextInstance;
extern void (*_SchedErrorHandler)(ErrorCode,void*,pfloat_t);


/*****************************************************************************/


/* Quick and dirty timespec normalization function */
#define USEC_PER_SEC		1000000
#define NSEC_PER_SEC		1000000000
static inline void tsnorm(struct timespec *ts) {
	while (ts->tv_nsec >= NSEC_PER_SEC) {
		ts->tv_nsec -= NSEC_PER_SEC;
		ts->tv_sec++;
	}
	while (ts->tv_nsec < 0 ) {
		ts->tv_nsec += NSEC_PER_SEC;
		ts->tv_sec--;
	}
}


/*****************************************************************************/


/* Initialize the high resolution timer system */
void InitializeTime( void ){
	// Set the initial time
	clock_gettime( CLOCK_REALTIME, &_initAT );
	// Set the start time
	_startAT = _initAT;
	// Set the initialization flag
	_hrtInitialized = true;
	// Also set the first hyperperiod flag
	_hrtFirstHyperperiod = true;
}


/* Get the amount of elapsed time (in ms) since the last call to ZeroTime */
double GetTimeMS( void ) {
	struct timespec currentTimeAT;
	// Make sure the clock has been initialized
	assert( _hrtInitialized );
	// Get the current time
	clock_gettime( CLOCK_REALTIME, &currentTimeAT );
	// Get difference between current time and startAT
	long differenceNS = currentTimeAT.tv_nsec - _startAT.tv_nsec;
	double differenceSec = difftime( currentTimeAT.tv_sec, _startAT.tv_sec );
	// Convert difference to double and MS
	double differenceMS = differenceSec * 1000.0 + ((double)differenceNS) / 1000000.0;
	return differenceMS;
}


/* Zero out the high resolution timer */
double ZeroTime( double desiredMS ) {
	// Make sure the clock has been initialized
	assert( _hrtInitialized );
        // Is this the first hyperperiod
	if ( _hrtFirstHyperperiod ) {
		// Just zero the _startAT clock
		clock_gettime( CLOCK_REALTIME, &_startAT );
		// Mark as no longer first hyperperiod
		_hrtFirstHyperperiod = false;
	}
	// Otherwise...
	else {
		// Are there offsets to work with (avoid div by zero)
		if ( _hrtNumOffsets != 0 ) {
			// What is the average offset
			uint8_t i = 0;
			double offsetAvgMS = 0.0;
			for ( ; i < _hrtNumOffsets; i++ ) offsetAvgMS += _hrtOffsets[i];
			// Lok out for div by zero
			offsetAvgMS /= (double)_hrtNumOffsets;
			// Set teh HRT Gain factor
			desiredMS -= ( offsetAvgMS * HRT_OFFSETGAIN );
			// Clear the offsets
			_hrtNumOffsets = 0;
		}
	}
	// Just increment Start clock by desiredMS
	_startAT.tv_nsec += (long)(desiredMS * 1000000.0 );
	// Normalize the nanosec portion of the time (>1s of ns adds 1 to s and truncs ns)
	tsnorm( &_startAT );
}


/* Get the absolute time from when InitializeTime was called - not affected by */
double AbsoluteTimeMS( void ) {
	struct timespec currentTimeAT;
	// Make sure the timer has been initialized
	assert( _hrtInitialized );
	// Get the current time
	clock_gettime( CLOCK_REALTIME, &currentTimeAT );
	// Get difference between current time and startAT
	long differenceNS = currentTimeAT.tv_nsec - _initAT.tv_nsec;
	double differenceSec = difftime( currentTimeAT.tv_sec, _initAT.tv_sec );//time_t differenceSec = currentTimeAT.tv_sec - _initAT.tv_sec;
	// Convert difference to double and MS
	double differenceMS = differenceSec * 1000.0 + ((double)differenceNS) / 1000000.0;
	return differenceMS;
}


/* Let the timing system know that an event occured at a slightly different time than expected */
void SuggestOffset( double deltaMS ) {
	// Make sure there is room for an additional offset
	if ( _hrtNumOffsets == HRT_MAXOFFSETS - 1 ) return;
	// Add the offset to the list
	_hrtOffsets[_hrtNumOffsets++] = deltaMS;
}


/* Sleep until the given time has elapsed from the start of the hyperperiod */
double NanoSleep( double deadlineMS ) {
	int retVal = 0;
	struct timespec timeVal, currentTimeAT;
	// Get the current time
	clock_gettime( CLOCK_REALTIME, &currentTimeAT );
	// Get difference between current time and startAT (start of hyperperiod)
	long differenceNS = currentTimeAT.tv_nsec - _startAT.tv_nsec;
	double differenceSec = difftime( currentTimeAT.tv_sec, _startAT.tv_sec );
	// Convert difference to long and NS
	long currentNS = (long)(differenceSec * 1000000000.0) + differenceNS;
	long deadlineNS = deadlineMS * 1000000;
	long toleranceNS = SCHEDULER_RESOLUTION_MS * 1000000;
	// Make sure clock has been initialized
	assert( _hrtInitialized );
	assert( deadlineMS >= 0.0 );
	// Make sure we are not calling this too late
	if ( currentNS > deadlineNS + toleranceNS ){
		double currentTimeMS = (double)currentNS / 1000000.0;
		// Signal that an error has occured
		_SchedErrorHandler( FS_ERR_SLEEPDEADLINEMISS, _nextInstance, deadlineMS );
		return currentTimeMS;
	}
	// Sleep until the deadline is reached
	timeVal.tv_sec = _startAT.tv_sec;
	timeVal.tv_nsec = _startAT.tv_nsec + deadlineNS;
	// Must normalize timeVal
	tsnorm( &timeVal );
	// Call the actual clock_nanosleep function (with absolute time value, not relative)
	do {
		retVal = clock_nanosleep( CLOCK_REALTIME, TIMER_ABSTIME, &timeVal, NULL );
	} while ( retVal != 0 );
	// Return the current time
	return GetTimeMS();
}


/* Busy wait (not sleep) until the given time has elapsed */
double  BusyWait( double durationMS ) {
	double stopTimeMS = GetTimeMS() + durationMS;
	double time;
	assert( _hrtInitialized );
	// Repeat until duration is reached
	do {
		time = GetTimeMS();
	} while ( time < stopTimeMS );
	return time;

}


/*****************************************************************************/

