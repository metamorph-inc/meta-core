/*** Copyright Vanderbilt ***/


#ifndef __FRODO_HIGHRES_TIMING_H__
#define __FRODO_HIGHRES_TIMING_H__


#ifdef __cplusplus
extern "C" {
#endif


/*** Included Header Files ***/
#include "arch.h"


/*****************************************************************************/


/* Initialize the high resolution timer system
 *	- Arguments: None
 *	- Return: None															*/
void InitializeTime( void );


/* Zero out the high resolution timer
 * 	- Arguments: The time when the clock should have been zeroed
 *	- Return: None															*/
void ZeroTime(
		pfloat_t desiredMS								// Time when the clock should have been zeroed
	);


/* Get the amount of elapsed time (in ms) since the last call to ZeroTime
 *	- Arguments: None
 *	- Return: Amount of time elapsed (in ms) since last call to ZeroTime	*/
pfloat_t GetTimeMS( void );


/* Get the absolute time from when InitializeTime was called - not affected by
 * calls to ZeroTime
 *	- Arguments: None
 *	- Returns: The absolute amount of time elapsed (in MS) since InitializeTime was called */
pfloat_t AbsoluteTimeMS( void );


/* Let the timing system know that an event occured at a slightly different time than expected
 *	- Arguments: Offset of event (positive means event occured early, negative means event occured late
 *	- Returns: None															*/
void SuggestOffset(
	   pfloat_t offsetMS
	);


/* Sleep until the given time has elapsed
 *	- Arguments: deadline - time to sleep until relative to last called ZeroTime
 *				 tolerance - fudge factor for is NanoSleep is called a bit late
 *	- Return: None															*/
pfloat_t NanoSleep(
		pfloat_t deadlineMS								// Deadline (within hyperperiod) for sleeping
	);


/* Busy wait (not sleep) until the given time has elapsed
 *	- Arguments: duration - time to busywait from right now
 *	- Return: None															*/
pfloat_t BusyWait(
		pfloat_t durationMS								// Relative duration (in ms) to wait
	);

	
/*****************************************************************************/


#ifdef __cplusplus
}
#endif


#endif // __FRODO_HIGHRES_TIMING_H__

