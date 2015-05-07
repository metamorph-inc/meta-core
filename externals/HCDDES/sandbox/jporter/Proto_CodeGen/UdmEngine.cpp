/*
 *	Copyright (c) Vanderbilt University, 2006
 *	ALL RIGHTS RESERVED
 *
 *	Vanderbilt University disclaims all warranties with regard to this
 *	software, including all implied warranties of merchantability
 *	and fitness.  In no event shall Vanderbilt University be liable for
 *	any special, indirect or consequential damages or any damages
 *	whatsoever resulting from loss of use, data or profits, whether
 *	in an action of contract, negligence or other tortious action,
 *	arising out of or in connection with the use or performance of
 *	this software.
 *
 *  FILE NAME:	UdmEngine.cpp 
 *	SYNOPSIS:	Implementation of class "UdmEngine", for setting up a Udm
 *				file to accept the translation of an SL/SF mdl file.
 *  AUTHOR:		Harmon Nine - hnine@isis.vanderbilt.edu
 *  CREATED:	07/20/06
 *  LAST UPDATE: 
 */
#include "UdmEngine.hpp"


void UdmEngine::createNewDataNetwork( const std::string &udmFileName ) {

	std::string extension( "xml" );
	std::string::size_type dotPos = udmFileName.rfind( '.' );
	if ( dotPos != std::string::npos ) {
		extension = udmFileName.substr( dotPos + 1 );
		if ( extension != "mga" ) extension = "xml";
	}

	std::string metainfo = "ESMoL";

	if ( extension == "xml" ) metainfo += ".xsd";

	_smartDataNetwork.CreateNew( udmFileName.c_str(), metainfo.c_str(), ESMoL::RootFolder::meta );

}


void UdmEngine::closeWithUpdate( void ) {
	_smartDataNetwork.CloseWithUpdate();
}
