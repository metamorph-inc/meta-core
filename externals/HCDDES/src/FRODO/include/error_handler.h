/*** Copyright Vanderbilt ***/


#ifndef __FRODO_ERROR_HANDLER_H__
#define __FRODO_ERROR_HANDLER_H__


#ifdef __cplusplus
extern "C" {
#endif


/*** Included Header Files ***/
#include "arch.h"


/*****************************************************************************/

/*** Enumerated Value and Type Definitions ***/


/*** Type Definitions ***/
typedef uint32_t					ErrorCode;				// Allow many many codes


/*** enums For Error Categories: ***/
typedef enum {
	ErrScheduler,											// For any scheduler errors
	ErrSerial,												// For any serial related errors
	ErrUDP													// For any UDP related errors
} ErrorCategory;

	
/*****************************************************************************/


/* Initialize the peripherals subsystem
 *	- Arguments: None
 *	- Returns: None												*/
void ErrorHandlerInitialize( void );


/* Scheduler Error Handler */
void ErrSchedulerHandler( ErrorCode errorCode, void* schedulable, pfloat_t atTimeMS );


/* UDP Error Handler */
void ErrUDPHandler( ErrorCode errorCode, void* peripheral, pfloat_t atTimeMS );


/* Serial Error Handler */
void ErrSerialHandler( ErrorCode errorCode, void* peripheral, pfloat_t atTimeMS );

/* */
char* ErrGetErrorCondition( void );


/*****************************************************************************/


#ifdef __cplusplus
}
#endif


#endif // __FRODO_ERROR_HANDLER_H__

