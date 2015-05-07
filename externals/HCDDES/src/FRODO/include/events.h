/*** Copyright Vanderbilt ***/


#ifndef __FRODO_EVENTS_H__
#define __FRODO_EVENTS_H__


#ifdef __cplusplus
extern "C" {
#endif


/*** Included Header Files ***/
#include "arch.h"


/*****************************************************************************/

	
/*** enums For Event Categories: ***/
typedef enum {
	NoCategory,
	Message,
	Scheduler,
	Peripheral,
	Syncronization,
	Error,
	UserData,
	Signals,
	UserEvents,
	AllCategories				= 0xff
} EventCategory;

	
/*****************************************************************************/
	
	
/*** enums For final Event destination: ***/
typedef enum {
	ToXML,											// Log to an XMl formatted file
	ToOMNeT,										// Log to
	ToConsole,										// Log to the console
	ToStats,										// Use logging to collect runtime stats
	ToRemoteHost,									// Log to a remote host
	ToNothing										// Turn off logging
} LogDestination;
	

// Typedef for EventID type - really just a 16-bit unsigned int
typedef uint16_t				EventID;


/* Header structure for each SysEvent storage */
typedef struct {
	EventCategory				category;						// Category of event
	EventID						ID;								// Sub-category ID 
	uint8_t						size;							// Size of event-related data
} SysEventHeader;


/*****************************************************************************/


/* Initialize the SysEvent subsystem
 *	- Arguments: Mask of accepted system events, if buffering is on, 
 *		if printing is allowed
 *	- Returns: None															*/
void SysEventInitialize(
		uint8_t					categoryMask,					// OR'ed mask of accepted SysEvent categories
		bool					buffer,							// Allow buffering of system events
		bool					print,							// Allow priting of system events
		bool					overwrite						// Allow memory bool to be overwritten
	);


/* Register a category of events
 *	- Arguments: name of category, unique ID for category, number of types category may have
 *	- Returns: None															*/
void SysEventRegisterCategory(
		char* name,
		EventCategory category,
		uint8_t numRequestedTypes
	);


/* Register a particular type of SysEvent
 *	- Arguments:
 *	- Returns: True if successfuly, false otherwise							*/
void SysEventRegister(
		EventCategory			category,						// Category of the event being registered
		EventID					ID,								// ID of the event being registered
		uint8_t(*SerializeFunc)(EventCategory,EventID,char*,va_list),	// If present, event is serializable
		void(*PrintFunc)(char*,EventCategory,EventID,char*),	// If present, event is printable
		void(*ProcessFunc)(EventCategory,EventID,char*)			// If present, function processes event
	);


/* Set the SysEvent mask
 *	- Arguments: number of items in the mask, variable mask elements
 *	- Returns: None															*/
void SysEventSetMask(
		uint8_t					numValues,						// Number of items in the mask
		...														// Mask values
	);


/* Record an event happening
 *	- Arguments: event category, event ID,
 *	- Returns: None															*/
void SysEvent(
		EventCategory			type,							// Type of data being written
		EventID					ID,								//
		...														// Event-specific list of function parameters
	);


/* Process any records in the datastore pool
 *	- Arguments: type of entries to process (AND a bitfield as a mask
 *	- Returns: None															*/
void SysEventsProcess(
		EventCategory			categoryMask
	);


/* Shutdown the SysEvent subsystem
 *	- Arguments: name of file to save it to (if any)
 *	- Returns: True if successfully shutdown, false otherwise				*/
void SysEventShutdown(
		char* filename
	);
 

/*****************************************************************************/


#ifdef __cplusplus
}
#endif


#endif // __FRODO_EVENTS_H__

