/*** Copyright Vanderbilt ***/


#ifndef __FRODO_PERIPHERAL_H__
#define __FRODO_PERIPHERAL_H__


#ifdef __cplusplus
extern "C" {
#endif


/*** Included Header Files ***/
#include "arch.h"
#include "scheduler.h"
#include "ports.h"


/*****************************************************************************/


// Critical definition - Largest possible message size
#ifndef	FP_MAXMESSAGESIZE
#define	FP_MAXMESSAGESIZE					256
#endif


/*** enums For Reserved Message IDs: ***/
typedef enum {
	NotAMessage						= 0x0000,
	SyncReady						= 0xFFFC,
	SyncAck							= 0xFFFD,
	SyncStart						= 0xFFFE,
	SyncHalt						= 0xFFFF
} ReservedMessageID;


/*** enums For Periperhal Event Types: ***/
typedef enum {
	AsyncReceive,
	SyncSendBegin,
	SyncSendEnd,
	SyncReceiveBegin,
	SyncReceiveEnd,
	AsyncSendBegin,
	AsyncSendEnd,
	ReceiveBegin,
	ReceiveEnd,
} PeripheralEventType;


/*** enums For Synchronization Event Types: ***/
typedef enum {
	SyncReadySend,
	SyncReadyReceive,
	SyncAckSend,
	SyncAckReceive,
	SyncStartSend,
	SyncStartReceive,
	SyncHaltSend,
	SyncHaltReceive,
	SyncSuggestOffset
} SynchronizationEventTypes;


/*** enums For Expect Direction: ***/
typedef enum {
	ExpectSend,
	ExpectReceive
} ExpectDirection;


/*** Type definitions ***/

typedef struct SyncExpectation_ {
	ExpectDirection					direction;					// Direction for the expectation
	uint16_t						msgID;						// Unique ID for message type
	uint32_t						sizeBytes;					// Number of bytes in message
	uint8_t							numStartTimes;				// Number of start times
	pfloat_t*						startTimesMS;				// Time at which to expect this action to start
	pfloat_t						wcetMS;						// Worst-case duration of message transmission
	PortId_t						portID;						// Associated messaging port
	SchedSchedulable*				task;						// Associated schedulable
} SyncExpectation;


typedef struct AsyncExpectation_ {
	ExpectDirection					direction;					// Direction for the expectation
	uint16_t						msgID;						// Unique ID for the message type
	uint32_t						sizeBytes;					// Number of bytes in message
	pfloat_t						wcetMS;						// Worst-case duration of message transmission
	pfloat_t						timeLowerBoundMS;			// Earliest in hyperperiod message can occur
	pfloat_t						timeUpperBoundMS;			// Latest in hyperperiod message can occur
	PortId_t						portID;						// Associated messaging port
	SchedSchedulable*				task;						// Schedulable to trigger on async
} AsyncExpectation;


typedef struct {
	SyncExpectation*				expect;
	pfloat_t						time;
} ExpectInstance;


/*** Enums For Error Conditions: ***/
#define FP_ERR_SYNCFAIL				0x0100
#define FP_ERR_CHANNELCREATEFAIL	0x0200
#define FP_ERR_CHANNELDESTROYFAIL	0x0400
#define FP_ERR_UNEXPECTEDMSGIDRECV	0x0800
#define FP_ERR_UNEXPECTEDMSGIDSEND	0x1000
#define FP_ERR_WRONGMSGSIZE			0x2000
#define FP_ERR_FAILURETOSEND		0x4000
#define FP_ERR_FAILURETORECEIVE		0x8000


/*****************************************************************************/


/* Initialize the peripherals subsystem
 *	- Arguments: None
 *	- Returns: None												*/
void PeripheralInitialize( void );


/* Sort a list of expectations
 *	- Arguments: Pointer to array of expectations, number in array
 *	- Returns: None												*/
void SortExpectations(
		ExpectInstance* expectInstances,						// Pointer to array of expectations
		uint16_t numExpects										// Number of elements in array
	);


/*****************************************************************************/


#ifdef __cplusplus
}
#endif


#endif //	__FRODO_PERIPHERAL_H__

