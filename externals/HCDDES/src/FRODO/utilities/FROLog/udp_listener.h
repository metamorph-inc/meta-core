/*** Copyright Vanderbilt ***/


#ifndef __FROLOG_UDP_LISTENER_H__
#define __FROLOG_UDP_LISTENER_H__


/*** Included Header Files ***/
#include "FROLog.h"
#include "scheduler.h"
#include "arch/highres_timing.h"
#include "udp.h"


/*** Enums For Error Conditions: ***/


/*****************************************************************************/


/*** Type Definitions ***/

class FLUDPChannel : public FLChannel {
private:
	std::string				_address;
	uint32_t				_port;
	pthread_t				_thread;
	SOCKET					_socket;
	// Hidden Funcs
	FLUDPChannel( );
public:
	FLUDPChannel( FROLogger *logger, const std::string &address, const uint32_t &port );
	~FLUDPChannel();

	virtual void Execute( void );
	virtual void AwaitCompletion( void );
};


/*****************************************************************************/


#endif //__FROLOG_UDP_LISTENER_H__



