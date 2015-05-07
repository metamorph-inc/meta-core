/*** Copyright Vanderbilt ***/


#ifndef __FRODO_LINUX64_PLATFORM_H__
#define __FRODO_LINUX64_PLATFORM_H__


#ifdef __cplusplus
extern "C" {
#endif


/*** Optional Optimization Global Values ***/
// Scheduler
#define FS_MAXTASKNAMELENGTH				16
#define	FS_MAXSCHEDULABLES				8
#define	FS_MAXPERIODICINSTANCES				16
#define	FS_MAXSPORADICINSTANCES				16
#define FS_MAXSYNCCHANNELS				8
// Generic Peripherals
#define	FP_MAXMESSAGESIZE				256
// UDP Peripherals
#define UDP_MAXCHANNELS					1
#define UDP_MAXSYNCEXPECTATIONS				8
#define UDP_MAXASYNCEXPECTATIONS			8
#define UDP_MAXSYNCTIMES				8
#define UDP_MAXPEERS					8
#define UDP_SYNCWAIT					100.0
#define UDP_SYNCPOLLWAIT				1
#define UDP_ASYNCPRIORITY				128
#define UDP_NAMELENGTH					32
#define UDP_MAXEXPECTINSTANCES				8
// Logger
#define LOGGER_MAXENTRIES				1024 * 1024
#define LOGGER_TEXTLENGTH				128

	
/*** Included Header Files ***/
#define _GNU_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>
#include <sched.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <time.h>
#include <signal.h>
#include <math.h>
	
	
/*****************************************************************************/


// Platform specific type definitions
typedef	double				pfloat_t;
#define bool				int
#define true				1
#define false				0
#define INLINE				inline
typedef sem_t				Semaphore;


// Platform Specific Definitions
#define SCHEDULER_RESOLUTION_MS			0.01		// 10us
#define SCHEDULER_INITIALIZATION_WAIT	1000.0
#define min(X,Y) ((X) < (Y) ? (X) : (Y))


// Platform Specific Functions
extern Semaphore* _CreateSemaphore( char*, uint32_t );
extern int _PostSemaphore( Semaphore* semaphore );
extern int _WaitSemaphore( Semaphore* semaphore );
extern void _DestroySemaphore( char*, Semaphore* );
extern bool _CreateThread( pthread_t*, void*(*)(void*), void*, uint32_t priority );
#define _InitThread( priority )			_CreateThread( NULL, NULL, NULL, priority )


/*****************************************************************************/
	
	
#ifdef __cplusplus
}
#endif


#endif // __FRODO_LINUX64_PLATFORM_H__


