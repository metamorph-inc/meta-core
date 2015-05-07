/*** Included Header Files ***/
#include "scheduler.h"



/*****************************************************************************/


/*** HRT Definitions ***/
#define HRT_MAXOFFSETS					8
#define HRT_OFFSETGAIN					0.5


/*** Static Variables ***/
static bool								_hrtInitialized = false;
static AbsoluteTime						_startAT;
static AbsoluteTime						_initAT;
static bool								_hrtFirstHyperperiod = true;
static double							_hrtOffsets[HRT_MAXOFFSETS];
static uint8_t							_hrtNumOffsets = 0;
static double							_hrtGain = 1.00;


/*** Externally Defined Variables and Functions ***/
extern SchedInstance					*_nextInstance;
extern void (*_SchedErrorHandler)(ErrorCode,void*,pfloat_t);


/*****************************************************************************/


/* Initialize the high resolution timer system */
void InitializeTime( void ) {
	// Make sure that timebase numer and denom are both 1
	mach_timebase_info_data_t timeBaseInfo;
	mach_timebase_info( &timeBaseInfo );
	assert( timeBaseInfo.denom == 1 );
	assert( timeBaseInfo.numer == 1 );
	// Get the current absolute time
	_initAT = UpTime();
	// Also set the start time
	_startAT = _initAT;
	// Set the initialization flag
	_hrtInitialized = true;
	// Also set the first hyperperiod flag
	_hrtFirstHyperperiod = true;
}


/* Get the amount of elapsed time (in ms) since the last call to ZeroTime */
double GetTimeMS( void ) {
	// Get the current time
	AbsoluteTime currentTimeAT = UpTime();
	// Make sure the clock has been initialized
	assert( _hrtInitialized );
	// Find the different between currentTime and _start
	Nanoseconds differenceNS = AbsoluteDeltaToNanoseconds( currentTimeAT, _startAT );
	long nanoseconds = UnsignedWideToUInt64( differenceNS );
	double milliseconds = (double)nanoseconds / 1000000.0 * _hrtGain;
	// Convert difference to MS
	return milliseconds;
}


/* Zero out the high resolution timer */
void ZeroTime( double desiredMS ) {
	// Make sure the clock has been initialized
	assert( _hrtInitialized );
	// Is this the first hyperperiod
	if ( _hrtFirstHyperperiod ) {
		// Just zero the _startAT clock
		_startAT = UpTime();
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
	Nanoseconds	desiredNS = UInt64ToUnsignedWide( desiredMS * 1000000.0 );
	_startAT = AddNanosecondsToAbsolute( desiredNS, _startAT );
}


/* Get the amount of MS elapsed since the last InitializeTime call */
double AbsoluteTimeMS( void ) {
	// Get the current time
	AbsoluteTime currentTimeAT = UpTime();
	// Make sure the clock has been initialized
	assert( _hrtInitialized );
	// Find the different between currentTime and _start
	Nanoseconds differenceNS = AbsoluteDeltaToNanoseconds( currentTimeAT, _initAT );
	long nanoseconds = UnsignedWideToUInt64( differenceNS );
	double milliseconds = (double)nanoseconds * 0.0000010;
	// Convert difference to MS
	return milliseconds;
}


/* Let the timing system know that an event occured at a slightly different time than expected */
void SuggestOffset( double deltaMS ) {
	// Make sure there is room for an additional offset
	if ( _hrtNumOffsets == HRT_MAXOFFSETS - 1 ) return;
	// Add the offset to the list
	_hrtOffsets[_hrtNumOffsets++] = deltaMS;
}


/* Sleep until the given time has elapsed */
double NanoSleep( double deadlineMS ) {
	int retVal = 0.0;
	struct timespec timeVal;
	// Get the current absolute time
	AbsoluteTime currentTimeAT = UpTime();
	// Find the different between currentTime and _start (the clock time thus far)
	Nanoseconds diffNS = AbsoluteDeltaToNanoseconds( currentTimeAT, _startAT );
	long currentNS = UnsignedWideToUInt64( diffNS );
	long deadlineNS = deadlineMS * 1000000;
	long toleranceNS = SCHEDULER_RESOLUTION_MS * 1000000;
	// Make sure clock has been initialized
	assert( _hrtInitialized );
	assert( deadlineMS >= 0.0 );
	// Make sure we are not calling this too late
	if ( currentNS > deadlineNS + toleranceNS ) {
		double currentTimeMS = (double)currentNS / 1000000.0;
		// Signal that an error has occurred
		_SchedErrorHandler( FS_ERR_SLEEPDEADLINEMISS, _nextInstance, deadlineMS );
		return currentTimeMS;
	}
	// Sleep until the deadline is reached
	timeVal.tv_sec = 0;
	timeVal.tv_nsec = deadlineNS - currentNS;	
	retVal = nanosleep( &timeVal, NULL );
	// Return the current time
	return GetTimeMS();
}


/* Busy wait (not sleep) until the given time has elapsed */
double BusyWait( double durationMS ) {
	double stopTimeMS = GetTimeMS() + durationMS;
	double timeMS;
	assert ( _hrtInitialized );
	// Repeat until duration is reached
	do {
		timeMS = GetTimeMS();
	} while ( timeMS < stopTimeMS );
	return timeMS;
}


/*****************************************************************************/

