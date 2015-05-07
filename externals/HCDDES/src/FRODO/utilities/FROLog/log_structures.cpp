/*** Included Header Files ***/
#include "log_structures.h"


/*****************************************************************************/


void FLMessageType::AddElement( const std::string &name, const std::string &type, const uint16_t &offset ) {
	// Set up the structure
	FLMessageElement newElement;
	newElement.name = name;
	newElement.type = type;
	newElement.offset = offset;
	// Add the element to the message
	this->_elements.insert( std::make_pair( offset, newElement ) );
}


/* */
FLMessageElement FLMessageType::Element( const unsigned int &index ) {
	if( index >= this->_elements.size() ) { exit(-1); }
	std::map<uint16_t,FLMessageElement>::iterator iter = this->_elements.begin();
	for ( int i = 0; i < index; i++ ) iter++;
	return (*iter).second;
}


/*****************************************************************************/

