/*** Copyright Vanderbilt ***/


#ifndef __FRODO_UDP_H__
#define __FRODO_UDP_H__


#ifdef __cplusplus
extern "C" {
#endif


/*** Included Header Files ***/
#include "peripheral.h"
// In order to have SOCKET be well defined...
#ifdef WIN32
//#include <Winsock2.h>
#else
#define SOCKET	int
#endif


/*** Enums For Error Conditions: ***/
#define UDP_ERR_BINDSOCKETFAIL				0x0001
#define UDP_ERR_CREATERECVSOCKETFAIL		0x0002
#define UDP_ERR_CREATESENDSOCKETFAIL		0x0004
#define UDP_ERR_CLOSESOCKETFAIL				0x0008
#define UDP_ERR_SELECTFAIL					0x0010
#define UDP_ERR_RECVFROMFAIL				0x0020
#define UDP_ERR_SENDTOFAIL					0x0040
#define UDP_ERR_PORTREADFAIL				0x0080


/*****************************************************************************/


/*** Type Definitions ***/


typedef struct {
	pthread_t						asyncThread;				// Thread for async receive - if needed
	Semaphore*						semaphore;					// Access control semaphore
	SyncExpectation*				activeExpect;				// What is the current expectation
	bool							activeReceive;				// Was the expectation met
	char							localAddress[15];			// Local IP address
	char							remoteAddress[15];			// Remote IP address
	uint16_t						receivePort;				// Port number for receiving
	SOCKET							receiveSocket;				// Socket created to receive
	SOCKET							sendSocket;					// Socket created for sending
	uint8_t							numSyncExpectations;		// Number of sync expectations
	SyncExpectation*				syncExpectations;			// Array of sync expectations
	uint8_t							numAsyncExpectations;		// Number of async expectations
	AsyncExpectation*			asyncExpectations;			// Array of async expectations
	bool							syncChannel;				// Is channel a sync channel
	bool							syncMaster;					// Is node a sync master on channel
	uint8_t							numSyncPeers;				// How many peers on sync channel
	uint8_t							channelID;					// Unique ID of this node on the channel
} UDPChannel;


/*****************************************************************************/


/* Initialize the UDP subsystem (applies to all ports)
 *	- Arguments:
 *	- Returns: true on success, false on failure							*/
bool UDPInitialize( 
		void (*ErrorHandler)(ErrorCode,void*,pfloat_t)		// Function pointer to error handler for UDP channels
	);


/* Shutsdown the peripheral (all channels)
 *	- Arguments: None
 *	- Return (bool): true for successfully shutdown,
 *					 false for failure to properly shutdown					*/
bool UDPShutdown( void );


/* Creates a channel for UDP communication - is initialized with
 * all of the syncronous and asyncronous expectations it could ever possibly use
 *	- Arguments: UDP port number, number of sync and async expectations and arrays for each
 *	- Returns: Pointer to valid channel structure on success, null on failure	*/
UDPChannel* UDPCreateChannel(
		char*					localIPAddress,					// IP address to use for local socket
		char*					remoteIPAddress,				// IP address of other end of channel
		uint16_t				receivePort,					// Port to use for receiving messages
		uint8_t					numSyncExpectations,			// Number of sync expectations
		SyncExpectation*		syncExpectations,				// Array of sync expectations
		uint8_t					numAsyncExpectations,			// Number of async expectations
		AsyncExpectation*	asyncExpectations,				// Array of async expectations
		bool					schedSyncChannel,				// Is this a scheduler sync channel
		bool					syncMaster,						// Is this node the channel master
		uint8_t					numSyncPeers					// Number of hosts on the channel
	);


/* Send a message on a UDP channel asyncronously - non-blocking
 *	- Arguments: Channel to send on, port ID to send
 *	- Returns: true on success, false on failure to create sporadic event	*/
bool UDPAsyncSend(
		UDPChannel*				channel,						// Channel to use for async message
		PortId_t				portID,							// ID of port to send
		pfloat_t				relativeDeadlineMS				// Relative deadline for this send
	);


/*****************************************************************************/


#ifdef __cplusplus
}
#endif


#endif // __FRODO_UDP_H__

