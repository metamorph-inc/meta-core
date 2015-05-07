/*** Copyright Vanderbilt ***/


#ifndef __FRODO_PORTS_H__
#define __FRODO_PORTS_H__


#ifdef __cplusplus
extern "C" {
#endif


/*** Included Header Files ***/
#include "arch.h"


/*****************************************************************************/


// Configuration
#ifndef FP_PORTMEMORYPOOLSIZE
#define FP_PORTMEMORYPOOLSIZE				1048576
#endif
#ifndef	FP_MAXPORTNAMESIZE
#define	FP_MAXPORTNAMESIZE					256
#endif
#ifndef FP_MAXSAMPLINGPORTS
#define FP_MAXSAMPLINGPORTS					256
#endif
#ifndef FP_MAXSAMPLINGMESSAGESIZE
#define FP_MAXSAMPLINGMESSAGESIZE			1024
#endif
#ifndef FP_MAXSAMPLINGREFRESHPERIOD
#define FP_MAXSAMPLINGREFRESHPERIOD			16
#endif
#ifndef FP_MAXQUEUINGPORTS
#define FP_MAXQUEUINGPORTS					32
#endif
#ifndef FP_MAXQUEUINGMESSAGESIZE
#define FP_MAXQUEUINGMESSAGESIZE			1024
#endif
#ifndef FP_MAXQUEUESIZE
#define FP_MAXQUEUESIZE						32
#endif


/*** enums For Port Methods ***/
typedef enum {
	Source							= 0,
	Destination						= 1,
	Bidirectional					= 2
} PortDirection_t;


typedef enum {
	InvalidConfig					= 0,
	NoAction						= 1,
	InvalidMode						= 2,
	NoError							= 3,
	InvalidParameter				= 4
} ReturnCode_t;


typedef enum {
	Valid							= 0,
	Invalid							= 1
} Validity_t, PortStatus_t;


// APEX Type Definitions
typedef char*						PortName_t;
typedef uint16_t					PortId_t;
typedef uint32_t					MessageSize_t;
typedef uint16_t					QueueSize_t;
typedef uint32_t					SystemTime_t;
typedef void*						MessageAddress_t;
typedef uint8_t						QueuingDiscipline_t;


/*** Type definitions ***/
// None


/*** Enums For Error Conditions: ***/
// None


/*****************************************************************************/


/* Create a sampling port */
void CreateSamplingPort(
		PortName_t					name,							// Name of this sampling port
		MessageSize_t				maxMessageSizeB,				// Maximum size of message
		PortDirection_t				direction,						// Port direction
		SystemTime_t				refreshPeriodQ,					// Refresh period
		PortId_t					*samplingPortID,				// ID of created sampling port
		ReturnCode_t				*returnCode						// Function return code
	);


/* Write a message to a sampling port */
void WriteSamplingMessage(
		PortId_t					samplingPortID,					// ID of sampling port
		MessageAddress_t			messageAddress,					// Address of data to be written
		MessageSize_t				lengthB,						// Number of bytes to be written
		ReturnCode_t				*returnCode						// Function return code
	);


/* Read a message from a sampling port */
void ReadSamplingMessage(
		PortId_t					samplingPortID,					// ID of sampling port
		MessageAddress_t			messageAddress,					// Address into which data will be placed
		MessageSize_t				*lengthB,						// Number of bytes that were read
		Validity_t					*validity,						// Validity of the read message
		ReturnCode_t				*returnCode						// Function return code
	);


/* Get the id of a sampling port from the name */
void GetSamplingPortID(
		PortName_t					name,							// Name of requested sampling port
		PortId_t					*samplingPortID,				// ID of requested sampling port
		ReturnCode_t				*returnCode						// Function return code
	);


/* Get the current status of a sampling port */
void GetSamplingPortStatus(
		PortId_t					samplingPortID,					// ID of sampling port
		PortStatus_t				*samplingPortStatus,			// Status of the sampling port
		ReturnCode_t				*returnCode						// Function return code
	);


/*****************************************************************************/


/* Create a new queuing port */
void CreateQueuingPort(
		PortName_t					name,							// Name of this sampling port
		MessageSize_t				maxMessageSizeB,				// Maximum size of message
		QueueSize_t					maxQueueSize,					// Maximum number of messages in queue
		PortDirection_t				direction,						// Port direction
		QueuingDiscipline_t			queuingDiscipline,				//
		PortId_t					*queuingPortID,					// ID of created queuing port
		ReturnCode_t				*returnCode						// Function return code
	);


/* Send a message via a queuing port */
void SendQueuingMessage(
		PortId_t					queuingPortID,					// ID of requested queuing port
		MessageAddress_t			messageAddress,					// Address of data to be written
		MessageSize_t				lengthB,						// Number of bytes to be written
		SystemTime_t				timeoutQ,						// How long is the message valid
		ReturnCode_t				*returnCode						// Function return code
	);


/* Receive a message via a queuing port */
void ReceiveQueuingMessage(
		PortId_t					queuingPortID,					// ID of requested queuing port
		SystemTime_t				timeoutQ,						//
		MessageAddress_t			messageAddress,					// Address into which data will be placed
		MessageSize_t				*lengthB,						// Number of bytes that were read
		ReturnCode_t				*returnCode						// Function return code
	);


/* Get the id of a queuing port from its name */
void GetQueuingPortID(
		PortName_t					name,							// Name of requested queuing port
		PortId_t					*queuingPortID,					// ID of requested queuing port
		ReturnCode_t				*returnCode						// Function return code
	);


/* Get the current status of a queuing port */
void GetQueuingPortStatus(
		PortId_t					queuingPortID,					// ID of sampling port
		PortStatus_t				*queuePortStatus,				// The status of the queuing port
		ReturnCode_t				*returnCode						// Function return code
	);


/*****************************************************************************/


/* Write a message via a generic port */
void WriteMessage(
		PortId_t					portID,							// ID of requested port
		MessageAddress_t			messageAddress,					// Address of data to be written
		MessageSize_t				lengthB,						// Number of bytes to be written
		SystemTime_t				timeoutQ,						// How long is the message valid
		ReturnCode_t				*returnCode						// Function return code
	);


/* Read a message via a generic port */
void ReadMessage(
		PortId_t					portID,							// ID of requested queuing port
		MessageAddress_t			messageAddress,					// Address into which data will be placed
		MessageSize_t				*lengthB,						// Number of bytes that were read
		Validity_t					*validity,						// Validity of the read message
		ReturnCode_t				*returnCode						// Function return code
	);


/*****************************************************************************/


#ifdef __cplusplus
}
#endif


#endif //	__FRODO_PORTS_H__

