/*** Copyright Vanderbilt ***/


#ifndef __FRODO_ARCH_H__
#define __FRODO_ARCH_H__


/*** Included Header Files ***/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <math.h>
#include <stdarg.h>
#ifdef WIN32
#include "arch\win32\platform.h"
#endif
#ifdef __APPLE__
#include "arch/osx/platform.h"
#endif
#ifdef __linux__
#include "arch/linux64/platform.h"
#endif

/*****************************************************************************/


// Platform Specific Functions
extern int inet_pton(int, const char*, void*);
extern Semaphore* _CreateSemaphore( char*, uint32_t );
extern int _PostSemaphore( Semaphore* semaphore );
extern int _WaitSemaphore( Semaphore* semaphore );
extern void _DestroySemaphore( char*, Semaphore* );
extern bool _CreateThread( pthread_t*, void*(*)(void*), void*, uint32_t priority );
#define _InitThread( priority )			_CreateThread( NULL, NULL, NULL, priority )


/*****************************************************************************/


#endif // __FRODO_ARCH_H__

