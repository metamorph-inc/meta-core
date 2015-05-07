/*** Copyright Vanderbilt ***/

#ifndef __FRODO_SERIAL_H__
#define __FRODO_SERIAL_H__


#ifdef __cplusplus
extern "C" {
#endif


/*** Included Header Files ***/
#include "arch.h"
#include "peripheral.h"


/*** Enums For Error Conditions: ***/
#define SERIAL_ERR_OPENPORTFAIL                 0x0001
#define SERIAL_ERR_READPORTFAIL                 0x0002
#define SERIAL_ERR_SENDTOFAIL                   0x0004
#define SERIAL_ERR_BADBAUDRATE                  0x0008
#define SERIAL_ERR_BADDATABITS                  0x0010
#define SERIAL_ERR_CLOSEFAIL                    0x0020
#define SERIAL_ERR_SELECTFAIL			0x0040



/*****************************************************************************/


/*** Type Definitions ***/

typedef struct {

   pthread_t            asyncThread;          // Thread for async receive - if needed
   Semaphore*           semaphore;            // Access control semaphore
   SyncExpectation*		activeExpect;         // What is the current expectation
   bool                 activeReceive;        // Was the expectation met
   char                 serialDevice[32];     // Serial device name
   uint32_t             filedesc;             // Send/receive file descriptor
   uint32_t             baud;                 // Baud rate
   uint16_t             dataBits;             // How many serial data bits?
   bool                 parity;               // Parity bit or no
   uint16_t             stopBits;             // How many stop bits?
   uint8_t              numSyncExpectations;  // Number of sync expectations
   SyncExpectation*		syncExpectations;     // Array of sync expectations
   uint8_t              numAsyncExpectations; // Number of async expectations
   AsyncExpectation*	asyncExpectations;    // Array of async expectations
   bool                 syncChannel;          // Is channel a sync channel
   bool                 syncMaster;           // Is node a sync master on channel
   uint8_t              numSyncPeers;         // How many peers on sync channel
   uint8_t              channelID;            // Unique ID of this node on the channel
} Serial_Channel;


/*****************************************************************************/


/* Initialize the Serial subsystem (applies to all ports)
 *	- Arguments: function pointer to an error handler
 *	- Returns: true on success, false on failure
 */
bool Serial_Initialize( 
   void (*ErrorHandler)(ErrorCode,void*,pfloat_t) // Function pointer to error handler for serial channels
	);


/* Shuts down the peripheral (all channels)
 *	- Arguments: None
 *	- Return (bool): true for successfully shutdown,
 *                       false for failure to properly shutdown
 */
bool Serial_Shutdown( void );


/* Creates a channel for Serial communication - is initialized with
 * all of the syncronous and asyncronous expectations it could ever possibly use
 *   - Arguments: Serial device, baud, number of sync and async expectations and arrays for each
 *   - Returns: Pointer to valid channel structure on success, null on failure	*/
Serial_Channel* Serial_CreateChannel(
   char*				serialDevice,         // Device name string
   uint32_t				baud,                 // Baud rate
   uint16_t				dataBits,             // Data bits in each frame
   bool					parity,               // Parity bit in frame (yes/no)
   uint16_t				stopBits,             // Stop bits at the end of each frame
   uint8_t				numSyncExpectations,  // Number of sync expectations
   SyncExpectation*		syncExpectations,      // Array of sync expectations
   uint8_t              numAsyncExpectations,  // Number of async expectations
   AsyncExpectation*	asyncExpectations,     // Array of async expectations
   bool                 schedSyncChannel,      // Is this a scheduler sync channel
   bool                 syncMaster,            // Is this node the channel master
   uint8_t              numSyncPeers           // Number of hosts on the channel
 );


/* Send a message on a Serial channel asyncronously - non-blocking
 *	- Arguments: Channel to send on, message ID to send
 *	- Returns: true on success, false on failure to create sporadic event	*/
bool Serial_AsyncSend(
   Serial_Channel*   channel,             // Channel to use for async message
   uint16_t          msgID,               // ID of async message to send
   pfloat_t          relativeDeadlineMS   // Relative deadline for this send
 );


/*****************************************************************************/

#ifdef __cplusplus
}
#endif


#endif // __FRODO_SERIAL_H__

