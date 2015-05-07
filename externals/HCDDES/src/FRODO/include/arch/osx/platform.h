/*** Copyright Vanderbilt ***/


#ifndef __FRODO_OSX_PLATFORM_H__
#define __FRODO_OSX_PLATFORM_H__


#ifdef __cplusplus
extern "C" {
#endif

	
/*** Optional Optimization Global Values ***/
// Scheduler
#define FS_MAXTASKNAMELENGTH				32
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
#include <pthread.h>
#include <mach/semaphore.h>
#include <mach/task.h>
#include <mach/mach_init.h>
#include <mach/mach_time.h>
#include <mach/thread_policy.h>
#include <mach/thread_act.h>
#include <sched.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <CoreServices/CoreServices.h>
#include <time.h>
	
	
/*****************************************************************************/


// Platform specific type definitions
typedef	double				pfloat_t;
typedef uint16_t            tick_t;
//#define bool				int
//#define true				1
//#define false				0
#define INLINE				inline
typedef semaphore_t			Semaphore;


// Platform Specific Definitions
#define SCHEDULER_RESOLUTION_MS			0.02
#define SCHEDULER_INITIALIZATION_WAIT	1000.0
#define min(X,Y) ((X) < (Y) ? (X) : (Y))


// Platform Specific Functions
//extern Semaphore* _CreateSemaphore( char*, uint32_t );
//extern int _PostSemaphore( Semaphore* semaphore );
//extern int _WaitSemaphore( Semaphore* semaphore );
//extern void _DestroySemaphore( char*, Semaphore* );
//extern bool _CreateThread( pthread_t*, void*(*)(void*), void*, uint32_t priority );
//#define _InitThread( priority )			_CreateThread( NULL, NULL, NULL, priority )


/*****************************************************************************/
	
	
#ifdef __cplusplus
}
#endif


#endif // __FRODO_OSX_PLATFORM_H__

