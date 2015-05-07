/*** Included Header Files ***/
#include "udp.h"
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <errno.h>


/*** Externall Defined Variables and Functions ***/
extern void (*_udpErrorHandler)(ErrorCode,void*,pfloat_t);


/*****************************************************************************/


bool _UDPIsBroadcast( char* address ) {
	char *ptr;
	assert( address != NULL );
	// Check the easy cases
	if ( strncmp( "localhost", address, 15) == 0 || strncmp( "127.0.0.1", address, 15 ) == 0 ) return true;
	// Find the final '.'
	ptr = strrchr( address, '.' );
	assert( ptr != NULL );
	// Check if the last tuple is .255 or not
	if ( strncmp( ".255", ptr, 4 ) == 0 ) return true;
	// Nope, not a broadcast address
	return false;
}


void _UDPForceBroadcastSend( UDPChannel* channel ) {
	char* ptr;
	assert( channel );
	// Find the final '.'
	ptr = strrchr( channel->remoteAddress, '.' );
	// Write 255 after it
	strncpy( ptr, ".255", 4 );
	
}


/*****************************************************************************/


bool _UDPCreateSockets( UDPChannel* channel ) {
	int retVal;
	bool broadcast = false;
	struct sockaddr_in service;
	bzero( &service, sizeof(service) );
	assert( channel != NULL );

	// Is this channel a broadcast channel
	if ( _UDPIsBroadcast( channel->localAddress ) || _UDPIsBroadcast( channel->remoteAddress ) ) {
		// Mark the channel as broadcast
		broadcast = true;
		// Make sure sending is strictly broadcast
		_UDPForceBroadcastSend( channel );
	}

	/*** Set up the UDP receiving socket ***/

	// Create the socket object
	channel->receiveSocket = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
	if ( channel->receiveSocket == -1 ) {
		// Signal that an error has occurred
		_udpErrorHandler( UDP_ERR_CREATERECVSOCKETFAIL, channel, 0.0 );
		return false;
	}
	// Allow some options
	retVal = 1;
	setsockopt( channel->receiveSocket, SOL_SOCKET, SO_REUSEADDR, (void*)&retVal, sizeof(retVal) );
	setsockopt( channel->receiveSocket, SOL_SOCKET, SO_REUSEPORT, (void*)&retVal, sizeof(retVal) );
	// Set up the sockaddr structure
	service.sin_family = AF_INET;
	// Check for "localhost" or 127.0.0.1 addresses
	if ( broadcast ) {
		// Use INADDR_ANY for the address
		service.sin_addr.s_addr = htonl( INADDR_ANY );
	}
	// Otherwise, use the requested address
	else {
		service.sin_addr.s_addr = inet_addr( channel->localAddress );
	}
	// Always use the requested port
	service.sin_port = htons( channel->receivePort );
	// Bind the socket.
	retVal = bind( channel->receiveSocket, (struct sockaddr *)&service, sizeof(service) );
	if ( retVal == -1 ) {
		// Signal that an error has occurred
		_udpErrorHandler( UDP_ERR_BINDSOCKETFAIL, channel, 0.0 );
		close( channel->receiveSocket );
		return false;
	}

	/*** Set up the UDP sending socket ***/

	// Create the socket object
	channel->sendSocket = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
	if ( channel->sendSocket == -1 ) {
		// Signal that an error has occurred
		_udpErrorHandler( UDP_ERR_CREATESENDSOCKETFAIL, channel, 0.0 );
		// Make sure to close the receive socket
		close( channel->receiveSocket );
		return false;
	}
	// Allow some options
	if ( broadcast ) {
		retVal = 1;
		setsockopt( channel->sendSocket, SOL_SOCKET, SO_BROADCAST, (void*)&retVal, sizeof(retVal) );
	}
	// Otherwise, all is good
	return true;
}


// Since Winsock is messed, this is a quick workaround
int _CloseSocket( SOCKET s ) {
	// Close the socket
	int retVal = close( s );
	// Was there an error
	if ( retVal != 0 ) {
		//Signal that an error has occured
		_udpErrorHandler( UDP_ERR_CLOSESOCKETFAIL, NULL, 0.0 );
	}
	return retVal;
}


/* Simple wrapper to account for WinSock using WSAGetLastError instead of errno */
int _UDPGetError( void )		{ return errno; }


/*****************************************************************************/



