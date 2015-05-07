/*** Included Header Files ***/
#include "udp.h"
#include "scheduler.h"
#include "highres_timing.h"


/*** Externall Defined Variables and Functions ***/
extern bool							_schedExec;
extern void (*_udpErrorHandler)(ErrorCode,void*,pfloat_t);


/*****************************************************************************/


bool _UDPIsBroadcast( char* address ) {
	char *ptr = NULL;
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


/* Primary thread for a specific UDP channel */
bool _UDPCreateSockets( UDPChannel *channel ) {
	int retVal;
	bool broadcast = false;
    WSADATA wsaData;
    WORD wVersionRequested = MAKEWORD(2, 2);
	struct sockaddr_in service;
	bzero( &service, sizeof(service) );
	assert( channel != NULL );

	// Initialize the sockets infrastructure
	retVal = WSAStartup( wVersionRequested, &wsaData );
	// Check for WSA errors
    if ( retVal != 0 ) {
        /* Tell the user that we could not find a usable */
        /* Winsock DLL.                                  */
        printf( "WSAStartup failed with error: %ld\n", WSAGetLastError() );
        return false;
	}

	// Is this channel a broadcast channel
	if ( _UDPIsBroadcast( channel->localAddress ) || _UDPIsBroadcast( channel->remoteAddress ) ) {
		// Mark the channel as broadcast
		broadcast = true;
		// Make sure sending is strictly broadcast
		_UDPForceBroadcastSend( channel );
	}

	/*** Set up the UDP receiving socket ***/

	// Set up the UDP socket
	channel->receiveSocket = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
	if ( channel->receiveSocket == INVALID_SOCKET ) {
		// Signal that an error has occured
		_udpErrorHandler( UDP_ERR_CREATERECVSOCKETFAIL, channel, 0.0 );
		return false;
	}
	// Allow some options
	retVal = 1;
	setsockopt( channel->receiveSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&retVal, sizeof(retVal) );
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
	service.sin_port = htons( channel->receivePort );
	// Bind the socket.
	retVal = bind( channel->receiveSocket, (SOCKADDR*) &service, sizeof(service) );
	if ( retVal == SOCKET_ERROR ) {
		// What is the error
		retVal = WSAGetLastError();
		// Close the socket
		closesocket( channel->receiveSocket );
		// Signal that an error has occured
		_udpErrorHandler( UDP_ERR_BINDSOCKETFAIL, channel, 0.0 );
		return false;
	}

	/*** Set up the UDP sending socket ***/

	// Set up the UDP socket
	channel->sendSocket = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
	if ( channel->sendSocket == INVALID_SOCKET ) {
		// Close the receiving socket
		closesocket( channel->receiveSocket );
		// Signal that an error has occured
		_udpErrorHandler( UDP_ERR_CREATESENDSOCKETFAIL, channel, 0.0 );
		return false;
	}
	// Allow some options
	if ( broadcast ) {
		// Set broadcast to true
		retVal = 1;
		setsockopt( channel->sendSocket, SOL_SOCKET, SO_BROADCAST, (void*)&retVal, sizeof(retVal) );
	}
	// Otherwise, all is good
	return true;
}


// Adjust for using socket-standard close instead of WinSock closesocket
int _CloseSocket( SOCKET s )	{ return closesocket( s ); }


/* Since Win32/WinSock does not use Errno, must wrap WSAGetLastError() */
int _UDPGetError( void ) {
	// Just get and return the last WSA error
	return WSAGetLastError();
}

/*****************************************************************************/



