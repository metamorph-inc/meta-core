/*** Included Header Files ***/
#include "FROLog.h"
#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMDocumentType.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMImplementationLS.hpp>
#include <xercesc/dom/DOMNodeIterator.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMText.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/XMLUni.hpp>


/*****************************************************************************/


/* Parse the XML configuration file */
void FROLogger::ParseConfig( const std::string &configFile ) {
	// Test to see if the file is ok.
	struct stat fileStatus;
	int iretStat = stat( configFile.c_str(), &fileStatus );
	if( iretStat == ENOENT )
		std::cout << "Path file_name does not exist, or path is an empty string.";
	else if( iretStat == ENOTDIR )
		std::cout << "A component of the path is not a directory.";
	else if( iretStat == ELOOP )
		std::cout << "Too many symbolic links encountered while traversing the path.";
	else if( iretStat == EACCES )
		std::cout << "Permission denied.";
	else if( iretStat == ENAMETOOLONG )
		std::cout << "File can not be read\n";
	
	// Configure DOM parser
	xercesc::XMLPlatformUtils::Initialize();
	xercesc::XercesDOMParser *parser = new xercesc::XercesDOMParser;
	parser->setValidationScheme( xercesc::XercesDOMParser::Val_Never );
	parser->setDoNamespaces( false );
	parser->setDoSchema( false );
	parser->setLoadExternalDTD( false );
	// Ready the standard tags
	//	XMLCh* tagFROLog = xercesc::XMLString::transcode("FROLog");
	XMLCh* tagUDPChannel = xercesc::XMLString::transcode("UDPChannel");
	XMLCh* tagMessageType = xercesc::XMLString::transcode("MessageType");
	XMLCh* tagElement = xercesc::XMLString::transcode("Element");
	XMLCh* attrAddress = xercesc::XMLString::transcode("address");
	XMLCh* attrPort = xercesc::XMLString::transcode("port");
	XMLCh* attrType = xercesc::XMLString::transcode("type");
	XMLCh* attrID = xercesc::XMLString::transcode("id");
	XMLCh* attrName = xercesc::XMLString::transcode("name");
	XMLCh* attrOffset = xercesc::XMLString::transcode("offset");
	char *charAddress=NULL, *charPort=NULL, *charType=NULL, *charID=NULL, *charName=NULL, *charOffset=NULL;
	
	// Try to parse the file
	try {
		// Here we go...
		parser->parse( configFile.c_str() );
		// No need to free this pointer - owned by the parent parser object
		xercesc::DOMDocument* xmlDoc = parser->getDocument();
		// Get the top-level element: Name is "FROLog". No attributes for "FROLog"
		xercesc::DOMElement* elementRoot = xmlDoc->getDocumentElement();
		if( !elementRoot ) throw( std::runtime_error( "Empty XML document" ) );
		
		// Parse XML file for tags of interest: "Channel, MessageTyp, etc."
		// Look one level nested within "FROLog". (children of root)
		xercesc::DOMNodeList* children = elementRoot->getChildNodes();
		const XMLSize_t nodeCount = children->getLength();
		// For all nodes, children of "FROLog" in the XML tree.
		for( XMLSize_t i = 0; i < nodeCount; i++ ) {
			// Get the child
			xercesc::DOMNode* currentNode = children->item( i );
			// We only care about valid element children
			if( currentNode->getNodeType() && currentNode->getNodeType() == xercesc::DOMNode::ELEMENT_NODE ) {
				// Found node which is an Element. Re-cast node as element
				xercesc::DOMElement* currentElement = dynamic_cast< xercesc::DOMElement* >( currentNode );
				// Is this a UDP Channel declaration
				if( xercesc::XMLString::equals( currentElement->getTagName(), tagUDPChannel ) ) {
					// Read attributes of element "UDPChannel".
					const XMLCh* xmlchAddress = currentElement->getAttribute( attrAddress );
					charAddress = xercesc::XMLString::transcode( xmlchAddress );
					const XMLCh* xmlchPort = currentElement->getAttribute( attrPort );
					charPort = xercesc::XMLString::transcode( xmlchPort );
					// Call to create a UDP channel object
					FLUDPChannel *udpChannel = new FLUDPChannel( this, charAddress, atoi( charPort ) );
					// Add this channel into the global vector
					this->_channels.push_back( udpChannel );
				}
				// Is this a MessageType declaration
				else if ( xercesc::XMLString::equals( currentElement->getTagName(), tagMessageType ) ) {
					// Read attributes of element "MessageType".
					const XMLCh* xmlchID = currentElement->getAttribute( attrID );
					charID = xercesc::XMLString::transcode( xmlchID );
					uint16_t msgID = atoi( charID );
					const XMLCh* xmlchName = currentElement->getAttribute( attrName );
					charName = xercesc::XMLString::transcode( xmlchName );
					// Call to create a MessageType object
					std::cout << "Creating MessageType (" << charName << ") with ID: " << msgID << ".\n";
					FLMessageType messageType( charName, msgID );
					// Now we need to get all of the element children
					xercesc::DOMNodeList* elementChildren = currentNode->getChildNodes();
					const XMLSize_t elementCount = elementChildren->getLength();
					// Loop through all elements
					for ( XMLSize_t j = 0; j < elementCount; j++ ) {
						// Get the element child
						xercesc::DOMNode* elementNode = elementChildren->item( j );
						// We only care about valid element children
						if( elementNode->getNodeType() && elementNode->getNodeType() == xercesc::DOMNode::ELEMENT_NODE ) {
							// Found node which is an Element. Re-cast node as element
							xercesc::DOMElement* element = dynamic_cast< xercesc::DOMElement* >( elementNode );
							// Make sure this is an "Element" type child
							if ( xercesc::XMLString::equals( element->getTagName(), tagElement ) ) {
								// Get the attributes of the element
								const XMLCh* xmlchName = element->getAttribute( attrName );
								charName = xercesc::XMLString::transcode( xmlchName );
								const XMLCh* xmlchType = element->getAttribute( attrType );
								charType = xercesc::XMLString::transcode( xmlchType );
								const XMLCh* xmlchOffset = element->getAttribute( attrOffset );
								charOffset = xercesc::XMLString::transcode( xmlchOffset );
								uint16_t offset = atoi( charOffset );
								// Now add this element to the parent message type object
								std::cout << "\tElement " << charName << " of type: " << charType << " @ " << offset << ".\n";
								messageType.AddElement( charName, charType, offset );
							}
						}
					}
					// Now add this messageType into the global list
					this->_messageTypes.insert( std::make_pair( msgID, messageType ) );
				}
			}
		}
	}
	// Catch any parsing errors
	catch( xercesc::XMLException& e ) {
		char* message = xercesc::XMLString::transcode( e.getMessage() );
		//		ostringstream errBuf;
		//		errBuf << "Error parsing file: " << message << flush;
		xercesc::XMLString::release( &message );
	}
	// Now clean up
	delete parser;
	if( charAddress ) xercesc::XMLString::release( &charAddress );
	if( charPort ) xercesc::XMLString::release( &charPort );
	if( charType ) xercesc::XMLString::release( &charType );
	if( charID ) xercesc::XMLString::release( &charID );
	if( charName ) xercesc::XMLString::release( &charName );
	if( charOffset ) xercesc::XMLString::release( &charOffset );
	try {
		//		xercesc::XMLString::release( &tagFROLog );
		xercesc::XMLString::release( &tagUDPChannel );
		xercesc::XMLString::release( &tagMessageType );
		xercesc::XMLString::release( &tagElement );
		xercesc::XMLString::release( &attrAddress );
		xercesc::XMLString::release( &attrPort );
		xercesc::XMLString::release( &attrType );
		xercesc::XMLString::release( &attrID );
		xercesc::XMLString::release( &attrName );
		xercesc::XMLString::release( &attrOffset );
	}
	catch( ... ) {
		std::cout << "Unknown exception encountered in TagNamesdtor\n";
	}
}


/* Process the reception of a message */
void FROLogger::ProcessMessage( char* buffer, uint32_t messageSize, char* sender ) {
	uint16_t msgID;
	// Get the message ID
	memcpy( &msgID, buffer, sizeof(uint16_t) );
	// Switch on the message type
	switch ( msgID ) {
		case SyncReady:
			printf( "%s @ %4.3f : SyncReady\n", sender, GetTimeMS() );
			break;
		case SyncAck:
			printf( "%s @ %4.3f : SyncAck\n", sender, GetTimeMS() );
			break;
		case SyncStart:
			printf( "%s @ %4.3f : SyncStart\n", sender, GetTimeMS() );
			printf( "----------------------------------------------------\n" );
			ZeroTime( 0.0 );
			break;
		case SyncHalt:
			printf( "----------------------------------------------------\n" );
			printf( "%s @ %4.3f : SyncHalt\n", sender, GetTimeMS() );
			this->HaltExecution();
			break;
		default:
			// Can we find the message ID in the map
			std::map<uint16_t,FLMessageType>::iterator messageIter = this->_messageTypes.find( msgID );
			if ( messageIter != this->_messageTypes.end() ) {
				// Process each element
				for ( int i = 0; i < (*messageIter).second.NumElement(); i++ ) {
					// Handle the detailed processing into message structures
					FLMessageElement element = (*messageIter).second.Element( i );
					// Is the element a double
					if ( element.type == "double" ) {
						double data;
						memcpy( &data, buffer + sizeof(msgID) + element.offset, sizeof(double) );
						printf( "%s @ %4.3f : %d = %4.3f \n", sender, GetTimeMS(), msgID, data );
					}
					else printf( "Error!!!\n" );
				}

			}
			break;
	}
}



/*****************************************************************************/


FROLogger::FROLogger( const std::string &configFile ) : _exec(true), _channels(), _messageTypes() {
	// Initialize the timing subsystem
	InitializeTime();
	// Parse the config file
	this->ParseConfig( configFile );
}


void FROLogger::Execute( void ) {
	// Set the run flag
	this->_exec = true;
	// Run until SyncHalt or Ctrl-C
	while ( this->_exec ) { }
	// Wait for the execution of all channels to stop
	while ( !this->_channels.empty() ) {
		FLChannel *channel = this->_channels.back();
		// Start waiting
		channel->AwaitCompletion();
		// Delete the channel
		delete channel;
		// Now remove it from the vector
		this->_channels.pop_back();
	}
	// We are done now...
	printf( "Finishing message collection...\n" );	
}


