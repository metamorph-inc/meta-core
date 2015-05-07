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
*  FILE NAME:	UdmEngine.hpp 
*	SYNOPSIS:	Definition of class "UdmEngine", for setting up a Udm
*				file to accept the translation of an SL/SF mdl file.
*  AUTHOR:		Harmon Nine - hnine@isis.vanderbilt.edu
*  CREATED:	07/20/06
*  LAST UPDATE: 
*/
#ifndef _ISIS_UDMENGINE
#define _ISIS_UDMENGINE

#include "ESMoL.h"

class UdmEngine {

private:
	Udm::SmartDataNetwork _smartDataNetwork;

	void createNewDataNetwork( const std::string &udmFileName );

	void openExisting( const std::string &udmFileName ) {
		_smartDataNetwork.OpenExisting( udmFileName );
	}

	void closeWithUpdate( void );

	Udm::Object getRootObject( void ) { return _smartDataNetwork.GetRootObject(); }


	UdmEngine( void ) : _smartDataNetwork( ESMoL::diagram ) { }

	~UdmEngine( void ) { closeWithUpdate(); }

	static UdmEngine &getUdmEngine( void ) {
		static UdmEngine udmEngine;
		return udmEngine;
	}


public:

	static void open( const std::string &udmFileName ) {
		getUdmEngine().openExisting( udmFileName );
	}

	static void close( void ) {
		getUdmEngine().closeWithUpdate();
	}

	static ESMoL::RootFolder &getRootFolder( void ) {
		static ESMoL::RootFolder rootFolder(  ESMoL::RootFolder::Cast( getUdmEngine().getRootObject() )  );

		return rootFolder;
	}

	typedef std::vector< ESMoL::Dataflow > DataflowVector;

	class DataflowInit {
	private:
		ESMoL::Dataflow _dataflow;

	public:
		DataflowInit( void ) {
			DataflowVector dataflowVector = getRootFolder().Dataflow_children();
			_dataflow = *dataflowVector.begin();
		}

		ESMoL::Dataflow &getDataflow( void ) { return _dataflow; }

	};


	static ESMoL::Dataflow &getDataflow( void ) {

		static DataflowInit dataflowInit;
		return dataflowInit.getDataflow();
	}

	typedef std::vector< ESMoL::Types > TypesVector;

private:
	class TypesInit {
	private:
		ESMoL::Types _types;

	public:
		TypesInit( void ) {
			TypesVector typesVector = getRootFolder().Types_children();
			if ( typesVector.empty() ) {
				_types = ESMoL::Types::Create( getRootFolder() );

				_types.name() = "Types";
			} else {
				_types = *typesVector.begin();
			}
		}
		ESMoL::Types &getTypes( void ) { return _types; }

	};

public:

	static ESMoL::Types &getTypes( void ) {

		static TypesInit typesInit;
		return typesInit.getTypes();
	}


};

#endif
