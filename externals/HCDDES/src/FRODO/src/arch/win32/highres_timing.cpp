/*** Included Header Files ***/
#include <Windows.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include "scheduler.h"


#ifdef __cplusplus
extern "C" {
#endif


/*****************************************************************************/


#define HRT_MAXOFFSETS						8
#define HRT_OFFSETGAIN						0.5


// Static Variables
static bool									_hrtInitialized = false;
static bool									_hrtFirstHyperperiod = true;
static LARGE_INTEGER						_startAT = {0,0};
static LARGE_INTEGER						_initAT = {0,0};
static LARGE_INTEGER						_frequencyMS = {0,0};
static double								_hrtOffsets[HRT_MAXOFFSETS];
static uint8_t								_hrtNumOffsets = 0;
static double								_hrtGain = 1.00;


/*** Externall Defined Variables and Functions ***/
extern SchedSchedulable						*_nextTask;
extern void (*_SchedErrorHandler)(ErrorCode,void*,pfloat_t);	


/*****************************************************************************/


/* Initialize the high resolution timer system */
void InitializeTime( void ) {
	// Get the frequency - quit if there is an error
	assert( ::QueryPerformanceFrequency( &_frequencyMS ) != 0 );
	// Adjust frequency to milliseconds
	_frequencyMS.QuadPart /= 1000;
	// Also, make sure Windows time resolution is set the finest possible
	TIMECAPS timeCaps;
	timeGetDevCaps( &timeCaps, sizeof(TIMECAPS) );
	timeBeginPeriod( timeCaps.wPeriodMin );
	// Initialize the init and start times
	::QueryPerformanceCounter( &_startAT );
	_initAT = _startAT;
	// Set the initialization flag
	_hrtInitialized = true;
	// Also set the first hyperperiod flag
	_hrtFirstHyperperiod = true;
}


/* Reset the high resolution timer */
void ZeroTime( double desiredMS ) {
	// Make sure clock has been initialized
	assert( _hrtInitialized );
	// Is this the first hyperperiod
	if ( _hrtFirstHyperperiod ) {
		// Just set the startAT clock
		::QueryPerformanceCounter( &_startAT );
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
			// Look out for div by zero
			offsetAvgMS /= (double)_hrtNumOffsets;
//			printf( "Average offset: %4.3f.\n", offsetAvgMS );
			// Set the HRT Gain factor
			desiredMS -= ( offsetAvgMS * HRT_OFFSETGAIN );
			// Clear the offsets
			_hrtNumOffsets = 0;
		}
	}
	// Just increment Start clock by desiredMS
	LONGLONG diffNS = (LONGLONG)( desiredMS * _frequencyMS.QuadPart );
	_startAT.QuadPart += (LONGLONG)diffNS;
}


/* Get the amount of time (in ms) since the timer was last reset */
double GetTimeMS( void ) {
	LARGE_INTEGER stop;
	// Make sure clock has been initialized
	assert( _hrtInitialized );
	::QueryPerformanceCounter( &stop );
	// Determine how much time has elapsed (take the Gain into account)
	double time = (double)(stop.QuadPart - _startAT.QuadPart) / (double)_frequencyMS.QuadPart * _hrtGain;
	// Convert to float and return
	return time;
}


/* Return the time (in ms) since the last call to InitializeTime() */
double AbsoluteTimeMS( void ) {
	LARGE_INTEGER stop;
	// Make sure clock has been initialized
	assert( _hrtInitialized );
	::QueryPerformanceCounter( &stop );
	double time = (double)(stop.QuadPart - _initAT.QuadPart) / (double)_frequencyMS.QuadPart;
	// Convert to float and return
	return time;
}


/* Let the timing system know that an event occured at a slightly different time than expected */
void SuggestOffset( double deltaMS ) {
	// Make sure the is room for an additional offset
	if ( _hrtNumOffsets == HRT_MAXOFFSETS - 1 ) return;
	// Add the offset to the list
	_hrtOffsets[_hrtNumOffsets++] = deltaMS;
}


/* Sleep until the specified amount of time has elapsed (measured using _startAT) */
double NanoSleep( double deadlineMS ) {
	// Make sure clock has been initialized
	assert( _hrtInitialized );
	assert( deadlineMS >= 0.0 );
	// Get the current time and determine sleep duration
	double currentTimeMS = GetTimeMS();
	// Make sure we are not calling this too late
	if ( currentTimeMS > deadlineMS + SCHEDULER_RESOLUTION_MS ) {
		// Signal an error has occured
		_SchedErrorHandler( FS_ERR_SLEEPDEADLINEMISS, NULL, deadlineMS );
		return currentTimeMS;
	}
	// Try sleeping that long - will not be accurate ( make sure differnce is positive )
	DWORD sleepDuration = DWORD( floor( abs( deadlineMS - currentTimeMS ) ) );
	DWORD retVal = SleepEx( sleepDuration, TRUE );
	// See what the time is now and busy wait until the time is up
	do {
		currentTimeMS = GetTimeMS();
	} while ( currentTimeMS < deadlineMS );
	// Return the current time
	return currentTimeMS;
}


/* Occupy this thread until the desired amount of time has elapsed */
double BusyWait( double durationMS ) {
	double stopTimeMS = GetTimeMS() + durationMS;
	double timeMS;
	// Make sure clock has been initialized
	assert( _hrtInitialized );
	// Loop until desired duration has been reached
	do {
		// Get the current time
		timeMS = GetTimeMS();
	// Loop until timeMS is reached
	} while ( timeMS < stopTimeMS );
	// Return the current time
	return timeMS;
}


/*****************************************************************************/


#ifdef __cplusplus
}
#endif

