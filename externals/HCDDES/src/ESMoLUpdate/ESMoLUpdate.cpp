#include "stdafx.h"
#include "ESMoLUpdate.h"

#include <set>
#include <map>

typedef std::set< ESMoL::DesignFolder > DesignFolderSet;
typedef std::set< ESMoL::ModelsFolder > ModelsFolderSet;
typedef std::set< ESMoL::Dataflow >     DataflowSet;
typedef std::set< ESMoL::Subsystem >    SubsystemSet;
typedef std::set< ESMoL::SubsystemRef > SubsystemRefSet;
typedef std::set< ESMoL::InputPort >    InputPortSet;
typedef std::set< ESMoL::OutputPort>    OutputPortSet;
typedef std::set< ESMoL::Connector >    ConnectorSet;

template< typename OPPORTTYPE >
struct OppositePortType {
public:
	OPPORTTYPE port;
	ESMoL::MessageRef refParent;
	bool operator <(const OppositePortType & opobj) const 
	{   unsigned long long leftID = refParent.uniqueId();
	    leftID = leftID << 32;
		leftID |= port.uniqueId();
		unsigned long long rightID = opobj.refParent.uniqueId();
		rightID = rightID << 32;
		rightID |= opobj.port.uniqueId();
		
		return leftID < rightID; 
	}
};

typedef OppositePortType< ESMoL::Output > OppositeInputPortType;
typedef OppositePortType< ESMoL::Input > OppositeOutputPortType;

template< typename PORTTYPE >
class ConnectedPorts {
public:
	typedef PORTTYPE Port;
	typedef std::set< Port > PortSet;
	typedef std::map< std::string, ConnectedPorts > MapType;

private:
	/* TODO: Try this -- create a struct that holds the opposite port object
	         and its refport parent.  Substitute this for the type handed back by
			 OppositePort below.  Update the connect methods to create the refport connections,
			 and the assignment to use a field of this structure.  Why wouldn't this work? */

	template< typename PORTTYPE, typename T = void >
	struct PortTypeSpecifics { // FOR AN ESMOL::InputPort
		typedef typename OppositeInputPortType OppositePort;
		typedef typename std::set< OppositePort > OppositePortSet;

		static PortSet getPortSet( ESMoL::Subsystem subsystem ) {
			return subsystem.InputPort_kind_children();
		}
		static ConnectorSet getConnectorSet( Port port ) {
			return port.srcConnector();
		}
		static OppositePort getConnectorEnd( ESMoL::Connector connector ) {
            OppositePort op;
			op.port = connector.srcConnector_end();
			op.refParent = ESMoL::MessageRef::Cast(connector.srcConnector__rp_helper());
			return op;
		}
		static void connect( Port port, OppositePort oppositePort, Udm::Object parent ) {
			ESMoL::Connector connector = ESMoL::Connector::Create( parent );
			connector.srcConnector__rp_helper() = oppositePort.refParent;
			connector.srcConnector_end() = oppositePort.port;
			connector.dstConnector_end() = port;
		}
	};

	template< typename T >
	struct PortTypeSpecifics< ESMoL::OutputPort, T > {
		typedef typename OppositeOutputPortType OppositePort;
		typedef typename std::set< OppositePort > OppositePortSet;

		static PortSet getPortSet( ESMoL::Subsystem subsystem ) {
			return subsystem.OutputPort_kind_children();
		}
		static ConnectorSet getConnectorSet( Port port ) {
			return port.dstConnector();
		}
		static OppositePort getConnectorEnd( ESMoL::Connector connector ) {
			OppositePort op;
			op.port = connector.dstConnector_end();
			op.refParent = ESMoL::MessageRef::Cast(connector.dstConnector__rp_helper());
			return op;
		}
		static void connect( Port port, OppositePort oppositePort, Udm::Object parent ) {
			ESMoL::Connector connector = ESMoL::Connector::Create( parent );
			connector.dstConnector__rp_helper() = oppositePort.refParent;
			connector.dstConnector_end() = oppositePort.port;
			connector.srcConnector_end() = port;
		}
	};

	typedef PortTypeSpecifics< Port > PortTypeSpecificClass;

public:
	typedef typename PortTypeSpecificClass::OppositePort OppositePort;
	typedef typename std::set< OppositePort > OppositePortSet;

private:
	OppositePortSet _oppositePortSet;

public:
	static PortSet getPortSet( ESMoL::SubsystemRef subsystemRef ) {
		ESMoL::Subsystem subsystem = subsystemRef.ref();
		return PortTypeSpecificClass::getPortSet( subsystem );
	}

	static OppositePortSet getOppositePortSet( Port port, ESMoL::SubsystemRef subsystemRef ) {
		Udm::Object subsystemRefParent = subsystemRef.GetParent();

		OppositePortSet oppositePortSet;
		ConnectorSet connectorSet = PortTypeSpecificClass::getConnectorSet( port );
		for( ConnectorSet::iterator cnsItr = connectorSet.begin() ; cnsItr != connectorSet.end() ; ++cnsItr ) {
			ESMoL::Connector connector = *cnsItr;
			if ( connector.GetParent() != subsystemRefParent ) continue;
			oppositePortSet.insert(  PortTypeSpecificClass::getConnectorEnd( *cnsItr )  );
		}

		return oppositePortSet;
	}

public:
	ConnectedPorts( Port port, ESMoL::SubsystemRef subsystemRef ) {
		_oppositePortSet = getOppositePortSet( port, subsystemRef );
	}

	void connect( Port port, ESMoL::SubsystemRef subsystemRef ) {
		Udm::Object subsystemRefParent = subsystemRef.GetParent();
		for( OppositePortSet::iterator opsItr = _oppositePortSet.begin(); opsItr != _oppositePortSet.end() ; ++opsItr ) {
			PortTypeSpecificClass::connect( port, *opsItr, subsystemRefParent );
		}
	}

};

typedef std::map< std::string, ESMoL::Subsystem > SubsystemNameMap;

// GLOBAL VARIABLES
SubsystemNameMap oldSubsystemNameMap;
SubsystemNameMap newSubsystemNameMap;

void processRootFolder( ESMoL::RootFolder rootFolder );
void processModelsFolderSet( ModelsFolderSet modelsFolderSet );

bool modelsFolderUpdate( ESMoL::ModelsFolder oldModelsFolder, ESMoL::ModelsFolder newModelsFolder );
bool buildOldSubsystemNameMap( ESMoL::ModelsFolder oldModelsFolder );
bool buildOldSubsystemNameMap( ESMoL::Subsystem oldSubsystem );
bool buildNewSubsystemNameMap( ESMoL::ModelsFolder newModelsFolder );
bool buildNewSubsystemNameMap( ESMoL::Subsystem newSubsystem );
bool updateModelsFolders( void );

template< typename PORTTYPE >
void copyPortConnections( ESMoL::SubsystemRef &oldSubsystemRef, ESMoL::SubsystemRef &newSubsystemRef );

template< typename PORTTYPE >
typename ConnectedPorts< PORTTYPE >::MapType getConnectedPortsMap( ESMoL::SubsystemRef &subsystemRef );

void clearMap( void );


void processRootFolder( ESMoL::RootFolder rootFolder ) {

	DesignFolderSet designFolderSet = rootFolder.DesignFolder_children();
	for( DesignFolderSet::iterator dfsItr = designFolderSet.begin() ; dfsItr != designFolderSet.end() ; ++dfsItr ) {

		ModelsFolderSet modelsFolderSet = dfsItr->ModelsFolder_children();
		switch( modelsFolderSet.size() ) {
			case 0:
			case 1: {
				break;
			}
			case 2: {
				processModelsFolderSet( modelsFolderSet );
				break;
			}
			case 3: {
				std::string designFolderName = dfsItr->name();
				AfxMessageBox(
				 "Warning: Design Folder \"" + CString( designFolderName.c_str() ) + "\"\n"
				 "has more than two ModelsFolder folders.  Cannot process.\n\n", MB_OK
				);
				break;
			}
		}

	}
}


void processModelsFolderSet( ModelsFolderSet modelsFolderSet ) {
	ModelsFolderSet::iterator oldModelsFolderItr = modelsFolderSet.begin();
	ModelsFolderSet::iterator newModelsFolderItr = oldModelsFolderItr; ++newModelsFolderItr;

	std::string oldModelsFolderName = oldModelsFolderItr->name();
	std::string newModelsFolderName = newModelsFolderItr->name();

	std::string oldModelsFolderNumber;
	if (  oldModelsFolderName.substr( 0, 3 ) == "New"  ) {

		int mfNumberStart = oldModelsFolderName.size() - 8;
		if ( mfNumberStart >= 4 ) oldModelsFolderNumber = oldModelsFolderName.substr( mfNumberStart );
	}

	std::string newModelsFolderNumber;
	if (  newModelsFolderName.substr( 0, 3 ) == "New"  ) {

		int mfNumberStart = newModelsFolderName.size() - 8;
		if ( mfNumberStart >= 4 ) newModelsFolderNumber = newModelsFolderName.substr( mfNumberStart );
	}

	if ( oldModelsFolderNumber.empty() ) {

		if ( newModelsFolderNumber.empty() ) {
			AfxMessageBox(
			 "Error: Cannot determine which of \"" + CString( oldModelsFolderName.c_str() ) + "\"\n\n"
			 "and \"" + CString( newModelsFolderName.c_str() ) + "\" is the new ModelsFolder\n\n"
			);
			return;
		}

	} else if ( newModelsFolderNumber.empty() ) {

		std::swap( oldModelsFolderItr, newModelsFolderItr );

	} else if (  oldModelsFolderNumber > newModelsFolderNumber ) {
		
		std::swap( oldModelsFolderItr, newModelsFolderItr );

	}

	if (  !modelsFolderUpdate( *oldModelsFolderItr, *newModelsFolderItr )  ) return;

	oldModelsFolderName = oldModelsFolderItr->name();
	newModelsFolderName = newModelsFolderItr->name();

	oldModelsFolderName = std::string(  "Old" + newModelsFolderName.substr( 3 )  );
	newModelsFolderName = newModelsFolderName.substr( 3, newModelsFolderName.size() - 8 - 3 );

	oldModelsFolderItr->name() = oldModelsFolderName;
	newModelsFolderItr->name() = newModelsFolderName;
}


bool modelsFolderUpdate( ESMoL::ModelsFolder oldModelsFolder, ESMoL::ModelsFolder newModelsFolder ) {

	bool retval =
	 buildOldSubsystemNameMap( oldModelsFolder ) &&
	 buildNewSubsystemNameMap( newModelsFolder ) &&
	 updateModelsFolders()
	;

	clearMap();
	return retval;
}


bool buildOldSubsystemNameMap( ESMoL::ModelsFolder oldModelsFolder ) {

	set< ESMoL::Dataflow > dataflowSet = oldModelsFolder.Dataflow_children();
	for( set< ESMoL::Dataflow >::iterator dfsItr = dataflowSet.begin() ; dfsItr != dataflowSet.end() ; ++dfsItr ) {

		set< ESMoL::Subsystem > subsystemSet = dfsItr->Subsystem_children();
		for( set< ESMoL::Subsystem >::iterator sssItr = subsystemSet.begin() ; sssItr != subsystemSet.end() ; ++sssItr ) {

			ESMoL::Subsystem currsys = *sssItr;
			if(  !buildOldSubsystemNameMap( currsys )  ) return false;
		}
	}
	return true;
}


bool buildOldSubsystemNameMap( ESMoL::Subsystem oldSubsystem ) {

	SubsystemRefSet subsystemRefSet = oldSubsystem.referedbySubsystemRef();
	if ( !subsystemRefSet.empty() ) {

		string oldSubsystemName = oldSubsystem.name();
		if (  oldSubsystemNameMap.find( oldSubsystemName ) != oldSubsystemNameMap.end()  ) {
			AfxMessageBox(
			 "Error: Each referenced Subsystem object must have unique name.\n\n"
			 "In the old ModelsFolder, there is more than one referenced\n\n"
			 "Subsystem object with the same name:  " + CString( oldSubsystemName.c_str() )
			);
			return false;
		}

		oldSubsystemNameMap[ oldSubsystemName ] = oldSubsystem;
	}

	SubsystemSet subsystemSet = oldSubsystem.Subsystem_kind_children();
	for( SubsystemSet::iterator sssItr = subsystemSet.begin() ; sssItr != subsystemSet.end() ; ++sssItr ) {
		if (  !buildOldSubsystemNameMap( *sssItr )  ) return false;			
	}
	return true;
}


bool buildNewSubsystemNameMap( ESMoL::ModelsFolder newModelsFolder ) {

	DataflowSet dataflowSet = newModelsFolder.Dataflow_children();
	for( DataflowSet::iterator dfsItr = dataflowSet.begin() ; dfsItr != dataflowSet.end() ; ++dfsItr ) {

		SubsystemSet subsystemSet = dfsItr->Subsystem_children();
		for( SubsystemSet::iterator sssItr = subsystemSet.begin() ; sssItr != subsystemSet.end() ; ++sssItr ) {

			ESMoL::Subsystem subsystem = *sssItr;
			if(  !buildNewSubsystemNameMap( subsystem )  ) return false;
		}
	}
	return true;
}

bool buildNewSubsystemNameMap( ESMoL::Subsystem newSubsystem ) {

	string newSubsystemName = newSubsystem.name();
	if (  oldSubsystemNameMap.find( newSubsystemName ) != oldSubsystemNameMap.end()  ) {

		if (  newSubsystemNameMap.find( newSubsystemName ) != newSubsystemNameMap.end()  ) {
			AfxMessageBox(
			 "Error: Each to-be-referenced Subsystem-object must have unique name.\n\n"
			 "In the new ModelsFolder, more than one to-be-referenced Subsystem object\n\n"
			 "has the same name:  " + CString( newSubsystemName.c_str() )
			);
			return false;
		}
	
		newSubsystemNameMap[ newSubsystemName ] = newSubsystem;
	}
	
	SubsystemSet subsystemSet = newSubsystem.Subsystem_kind_children();
	for( SubsystemSet::iterator sssItr = subsystemSet.begin() ; sssItr != subsystemSet.end() ; ++sssItr ) {
		if (  !buildNewSubsystemNameMap( *sssItr )  ) return false;
	}
	return true;
}

bool updateModelsFolders( void ) {

	for( SubsystemNameMap::iterator osmItr = oldSubsystemNameMap.begin() ; osmItr != oldSubsystemNameMap.end() ; ++osmItr ) {

		std::string subsystemName = osmItr->first;

		SubsystemNameMap::iterator nsmItr = newSubsystemNameMap.find( subsystemName );
		if ( nsmItr == newSubsystemNameMap.end() ) {
			AfxMessageBox(
			 "Warning:  Referenced Subsystem-object \"" + CString( subsystemName.c_str() ) +
			 "\" in old ModelsFolder\n\n"
			 "has no counterpart in new ModelsFolder"
			);
			continue;
		}

		ESMoL::Subsystem oldSubsystem = osmItr->second;
		ESMoL::Subsystem newSubsystem = nsmItr->second;

		SubsystemRefSet oldSubsystemRefSet = oldSubsystem.referedbySubsystemRef();
		for( SubsystemRefSet::iterator srsItr = oldSubsystemRefSet.begin() ; srsItr != oldSubsystemRefSet.end() ; ++srsItr ) {

			ESMoL::SubsystemRef oldSubsystemRef = *srsItr;
			ESMoL::Component comParent = oldSubsystemRef.Component_parent();
			ESMoL::SubsystemRef newSubsystemRef = ESMoL::SubsystemRef::Create( comParent );
			newSubsystemRef.name() = newSubsystem.name();
			newSubsystemRef.position() = oldSubsystemRef.position();
			newSubsystemRef.ref() = newSubsystem;
			
			copyPortConnections< ESMoL::InputPort >( oldSubsystemRef, newSubsystemRef );
			copyPortConnections< ESMoL::OutputPort >( oldSubsystemRef, newSubsystemRef );

			oldSubsystemRef.DeleteObject();
		}
	}

	return true;
}

template< typename PORTTYPE >
void copyPortConnections( ESMoL::SubsystemRef &oldSubsystemRef, ESMoL::SubsystemRef &newSubsystemRef ) {

	typedef PORTTYPE Port;
	typedef ConnectedPorts< Port > ConnectedPortsClass;

	typedef typename ConnectedPortsClass::MapType ConnectedPortsMap;
	typedef typename ConnectedPortsClass::PortSet PortSet;

	ConnectedPortsMap connectedPortsMap = getConnectedPortsMap< Port >( oldSubsystemRef );

	PortSet portSet = ConnectedPortsClass::getPortSet( newSubsystemRef );
	for( PortSet::iterator ptsItr = portSet.begin() ; ptsItr != portSet.end() ; ++ptsItr ) {
		Port port = *ptsItr;
		std::string portName = port.name();

		ConnectedPortsMap::iterator cpmItr = connectedPortsMap.find( portName );
		if ( cpmItr == connectedPortsMap.end() ) continue;

		ConnectedPortsClass connectedPorts = cpmItr->second;
		connectedPorts.connect( port, newSubsystemRef );
	}
}

template< typename PORTTYPE >
typename ConnectedPorts< PORTTYPE >::MapType getConnectedPortsMap( ESMoL::SubsystemRef &oldSubsystemRef ) {

	typedef PORTTYPE Port;
	typedef ConnectedPorts< Port > ConnectedPortsClass;

	typedef typename ConnectedPortsClass::MapType ConnectedPortsMap;
	typedef typename ConnectedPortsClass::PortSet PortSet;

	ConnectedPortsMap connectedPortsMap;

	PortSet portSet = ConnectedPortsClass::getPortSet( oldSubsystemRef );
	for( PortSet::iterator ptsItr = portSet.begin() ; ptsItr != portSet.end() ; ++ptsItr ) {
		Port port = *ptsItr;
		std::string portName = port.name();
		connectedPortsMap.insert(   std::make_pair(  portName, ConnectedPortsClass( port, oldSubsystemRef )  )   );
	}

	return connectedPortsMap;
}

void clearMap( void ) {
	newSubsystemNameMap.clear();
	oldSubsystemNameMap.clear();
}


