/*** Copyright Vanderbilt ***/


#ifndef __FRODO_LOGGER_H__
#define __FRODO_LOGGER_H__


#ifdef __cplusplus
extern "C" {
#endif


/*** Included Header Files ***/
#include "events.h"


/*****************************************************************************/


/* Iniitalize the logging subsystem
 *	- Arguments: None
 *	- Returns: None															*/
void LogInitialize( void );


#ifndef __DISABLE_LOGGING__
/* Log an event 
 *	- Arguments: type of event and optional data related to event
 *	- Returns: None															*/
void LogMessage(
		char*					message			// Message to be logged
	);
#endif


/*****************************************************************************/


#ifdef __cplusplus
}
#endif


#endif // __FRODO_LOGGER_H__

