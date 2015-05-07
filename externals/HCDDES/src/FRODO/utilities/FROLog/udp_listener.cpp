/*** Included Header Files ***/
#include "udp_listener.h"


/*****************************************************************************/


/* Thread entry point */
void* _UDPThreadExec( void* chnl ) {
	// Cast to a channel
	FLUDPChannel* channel = (FLUDPChannel*)chnl;
	// Wait for the signal to start
	channel->Execute();
	// Return NULL
	return NULL;
}


/*****************************************************************************/



/* Create the listening socket */
FLUDPChannel::FLUDPChannel( FROLogger *logger, const std::string &address, const uint32_t &port ) : ::FLChannel(logger),
	_address(address), _port(port), _thread(NULL), _socket(0) {
	int retVal;
	struct sockaddr_in service;
	bzero( &service, sizeof(service) );
	// Create the socket object
	this->_socket = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
	if ( this->_socket == -1 ) {
		// Signal that an error has occurred
		printf( "Error: Not able to create socket.\n" );
		return;
	}
	// Allow some options
	retVal = 1;
	setsockopt( this->_socket, SOL_SOCKET, SO_REUSEADDR, (void*)&retVal, sizeof(retVal) );
	setsockopt( this->_socket, SOL_SOCKET, SO_REUSEPORT, (void*)&retVal, sizeof(retVal) );
	// Set up the sockaddr structure
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = htonl( INADDR_ANY );
	// Always use the requested port
	service.sin_port = htons( port );
	// Bind the socket.
	retVal = bind( this->_socket, (struct sockaddr *)&service, sizeof(service) );
	if ( retVal == -1 ) {
		// Signal that an error has occurred
		std::cout <<  "Error: Not able to bind to socket (" << address << ":" << port << ").\n";
		close( this->_socket );
		return;
	}
	// Announce the start of this channel
	std::cout << "Creating UDP Channel (" << this->_address << ":" << this->_port << ").\n";
	// Need to create pthread here
	pthread_create( &this->_thread, NULL, _UDPThreadExec, this );
}


/* */
void FLUDPChannel::Execute( void ) {
	char buffer[ FP_MAXMESSAGESIZE + sizeof(uint16_t) ];
	struct sockaddr_in cliaddr;
	socklen_t len = sizeof( cliaddr );
	int retVal;
	struct timeval timeout = { 0, 100 };
	fd_set readSocketSet;

	std::cout << "Starting UDP message collection (" << this->_address << ":" << this->_port << ").\n";
	// Wait for the global run flag
	while ( !this->_logger->IsRunning() ) { }
	// Loop until death
	do {
		// Look for a read
		FD_SET( this->_socket, &readSocketSet );
		retVal = select( this->_socket + 1, &readSocketSet, NULL, NULL, &timeout );
		// Is there a message
		if ( retVal > 0 ) {
			// Read the data from the socket
			retVal = recvfrom( this->_socket, buffer, FP_MAXMESSAGESIZE + sizeof(uint16_t),
							  0, (struct sockaddr*)&cliaddr, &len );
			// Process the message
			this->_logger->ProcessMessage( buffer, retVal, inet_ntoa( cliaddr.sin_addr ) );
		}
		// Nothing else for now
	} while ( this->_logger->IsRunning() );
	// Shut it down
	std::cout << "Stopping UDP message collection (" << this->_address << ":" << this->_port << ").\n";
	// Close the receiving socket
	close( this->_socket );
	// Exit the thread
	pthread_exit( NULL );
}


void FLUDPChannel::AwaitCompletion( void ) {
	// Just wait on the join
	pthread_join( this->_thread, NULL );
}


/*****************************************************************************/

