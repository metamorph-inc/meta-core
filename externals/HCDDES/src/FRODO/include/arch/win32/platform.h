/*** Copyright Vanderbilt ***/


#ifndef __FRODO_WIN32_PLATFORM_H__
#define __FRODO_WIN32_PLATFORM_H__


/*** Optional Optimization Global Values ***/
// Scheduler
#define FS_MAXTASKNAMELENGTH				16
#define	FS_MAXSCHEDULABLES					8
#define	FS_MAXPERIODICINSTANCES				16
#define	FS_MAXSPORADICINSTANCES				16
#define FS_MAXSYNCCHANNELS					8
// Generic Peripherals
#define	FP_MAXMESSAGESIZE					256
// UDP Peripherals
#define UDP_MAXCHANNELS						1
#define UDP_MAXSYNCEXPECTATIONS				8
#define UDP_MAXASYNCEXPECTATIONS			8
#define UDP_MAXSYNCTIMES					8
#define UDP_MAXPEERS						8
#define UDP_SYNCWAIT						100.0
#define UDP_SYNCPOLLWAIT					1
#define UDP_ASYNCPRIORITY					128
#define UDP_NAMELENGTH						32
#define UDP_MAXEXPECTINSTANCES				8
// Logger
#define LOGGER_MAXENTRIES					1024 * 1024
#define LOGGER_TEXTLENGTH					128


/*** Included Header Files ***/
#include <Windows.h>


/*****************************************************************************/

// Adapted from stdint.h
typedef double							pfloat_t;
typedef signed char						int8_t;
typedef unsigned char					uint8_t;
typedef signed short					int16_t;
typedef unsigned short					uint16_t;
typedef signed long int					int32_t;
typedef unsigned long int				uint32_t;
typedef signed long long int			int64_t;
typedef unsigned long long int			uint64_t;
typedef int								ssize_t;
typedef int								socklen_t;


// Platform Specific Definitions
#define SCHEDULER_RESOLUTION_MS			0.25
#define SCHEDULER_INITIALIZATION_WAIT	1000.0
#define bzero(b,len)					(memset((b), '\0', (len)), (void) 0)


//typedef sem_t							Semaphore;
typedef HANDLE							Semaphore;
typedef HANDLE							pthread_t;
//#ifndef _WINDOWS_
#define bool							int
#define true							1
#define false							0
#define INLINE							__inline
//#endif


// Platform Specific Definitions
#define SCHEDULER_RESOLUTION_MS			0.25
#define SCHEDULER_INITIALIZATION_WAIT	1000.0
#define bzero(b,len)					(memset((b), '\0', (len)), (void) 0)
#define pthread_exit( value )			ExitThread( 0 )
#define pthread_join( handle, value )	WaitForSingleObject( handle, INFINITE )


/*****************************************************************************/


#endif // __FRODO_WIN32_PLATFORM_H__

