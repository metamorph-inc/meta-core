/*** Copyright Vanderbilt ***/


#ifndef __FROLOG_H__
#define __FROLOG_H__


/*** Included Header Files ***/
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <list>
#include <map>
#include <set>
#include <errno.h>
#include <string>
#include <vector>
#include <sys/stat.h>
#include "log_structures.h"
#include "udp_listener.h"


/*** Enums For Error Conditions: ***/


/*****************************************************************************/


/*** Type Definitions ***/


class FROLogger {
private:
	bool								_exec;
	std::vector<FLChannel*>				_channels;
	std::map<uint16_t,FLMessageType>	_messageTypes;
	FROLogger();
	void ParseConfig( const std::string &configFile );
public:
	FROLogger( const std::string &configFile );
	
	void Execute( void );
	void HaltExecution( void )			{ this->_exec = false; }
	bool IsRunning( void )				{ return this->_exec; }
	void ProcessMessage( char* buffer, uint32_t messageSize, char* sender );
};



/*****************************************************************************/


#endif //__FROLOG_H__



