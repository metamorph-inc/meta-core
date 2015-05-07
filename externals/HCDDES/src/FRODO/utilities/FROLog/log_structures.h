/*** Copyright Vanderbilt ***/


#ifndef __FROLOG_LOG_STRUCTURES_H__
#define __FROLOG_LOG_STRUCTURES_H__


/*** Included Header Files ***/
#include <map>
#include <string>
#include "main.h"
	
	
/*** Enums For Error Conditions: ***/
	
	
/*****************************************************************************/
	
	
/*** Type Definitions ***/
class FROLogger;

class FLChannel {
private:
	FLChannel();
protected:
	FROLogger*				_logger;
public:
	FLChannel( FROLogger *logger ) : _logger(logger) { }
	virtual void Execute( void )=0;
	virtual void AwaitCompletion( void )=0;
};


typedef struct {
	std::string				name;
	std::string				type;
	uint16_t				offset;
} FLMessageElement;


class FLMessageType {
private:
	uint16_t								_id;
	std::string								_name;
	std::map<uint16_t,FLMessageElement>		_elements;
	// Private Methods
	FLMessageType();
public:
	FLMessageType( const std::string &name, const uint16_t &id ) : _name(name), _id(id) { }
	
	void AddElement( const std::string &name, const std::string &type, const uint16_t &offset );
	unsigned int NumElement( void )	{ return this->_elements.size(); }
	FLMessageElement Element( const unsigned int &index );
	
};


/*****************************************************************************/


#endif //__FROLOG_LOG_STRUCTURES_H__

