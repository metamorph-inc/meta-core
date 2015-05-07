#include "UI.h"
#include "UdmUtil.h"
#include "Boost_Hash_Ext.h"
#include <queue>
#include <boost\unordered_set.hpp>

ModelHandler::ModelHandler(void)
{
}


ModelHandler::~ModelHandler(void)
{
}

void ModelHandler::attachComponent(ModelicaComponent component)
{
	this->component = component;
}

BOOL ModelHandler::generateCompReport()
{
	try
	{
		return this->processComponent();
	} catch(udm_exception e)
	{
		CString toolPrefix;
		toolPrefix.LoadString(LOG_TOOL_PREF);
		Console::Error::writeLine(string(toolPrefix + e.what()));
		return FALSE;
	}

	return TRUE;
}							

CString ModelHandler::extractPortType(Udm::Object portObj)
{
	// whenever the connected port found is derived from DataPort then extract type from the enum attribute of the dstPort
	if(Udm::IsDerivedFrom(portObj.type(), SignalFlow::DataPort::meta))
	{
		return CString(((string) SignalFlow::DataPort::Cast(portObj).DataType()).c_str()).Trim();
	}
	// whenever the connected Port is derived from SF_Port then find the contained TypeRef Port to get the type
	else if(Udm::IsDerivedFrom(portObj.type(), Simulink::SF_Port::meta) ||
			Udm::IsDerivedFrom(portObj.type(), Simulink::SFStateDE::meta) ||
			Udm::IsDerivedFrom(portObj.type(), Simulink::StateDE::meta)
		)
	{
		// find type from the contained type ref
		set<Udm::Object> typerefSet = portObj.GetChildObjects(Simulink::TypeBaseRef::meta);
		// a port can have only a single type
		if(typerefSet.size() == 1)
		{
			Simulink::TypeBaseRef typeBaseRef = Simulink::TypeBaseRef::Cast(*(typerefSet.begin()));
			try
			{
				return CString(((string) Simulink::SF_Matrix::Cast(typeBaseRef.getReferencedObject()).Type()).c_str()).Trim();
			} catch(udm_exception e)
			{
				throw udm_exception(_T("Simulink::TypeBaseRef is currently allowed to refer to objects of only Simulink::SF_Matrix type! @[OBJECT:]") +
									MyUdmUtil::getHyperLinkPath_StdString(typeBaseRef));
			}
		}
		else if(typerefSet.size() > 1)
		{
			throw udm_exception(MyUdmUtil::getHyperLinkPath_StdString(portObj) + _T(" contains more than 1 Simulink::TypeBaseRef type objects. "));
		}
	}
	// the portObj is an InputSignalInterface of a ModelicaComponent
	else if(portObj.type().name() == "InputSignalInterface")
	{
		return CString(((string) InputSignalInterface::Cast(portObj).Class()).c_str()).Trim();
	}
	// the portObj is an OutputSignalInterface of a ModelicaComponent
	else if(portObj.type().name() == "OutputSignalInterface")
	{
		return CString(((string) OutputSignalInterface::Cast(portObj).Class()).c_str()).Trim();
	}
	// the portObj is a ParameterRef of a ModelicaComponent
	else if(portObj.type().name() == "ParameterRef")
	{
		return CString(((string) ParameterRef::Cast(portObj).Class()).c_str()).Trim();
	}

	return CString(_T(""));
}

BOOL ModelHandler::processComponent()
{
	dynamic_cast<UI*>(this->pUI)->printNormalLog(_T("Processing: ") + 
					  CString(this->component.getPath2().c_str()) +
					  _T(", [TYPE:] ") +
					  CString(((string) this->component.type().name()).c_str()));

	// Perform BFS
	queue<Udm::Object>					portQueue;
	boost::unordered_set<Udm::Object>	processedPortSet;
	boost::unordered_set<string>		processedPortNameSet;

	// Find the ModelicaComponent's contained InputSignalInterface and push in queue
	// while  also notifying the observers of ModelHandler the Port's info
	set<InputSignalInterface>	inPortSet = this->component.InputSignalInterface_kind_children();
	for each(InputSignalInterface inPort in inPortSet)
	{
		portQueue.push(inPort);
		// Notify the new port info to the observers
		this->notifyNewPortInfo(inPort.getPath2().c_str(), this->extractPortType(inPort));
	}

	//	ports may be contained in the BusPortInterface
	set<BusInterface>	busInterfaceSet	=	this->component.BusInterface_kind_children();
	for each(BusInterface bi in busInterfaceSet)
	{
		set<InputSignalInterface>	biInPortSet = bi.InputSignalInterface_kind_children();
		for each(InputSignalInterface inPort in biInPortSet)
		{
			portQueue.push(inPort);
			// Notify the new port info to the observers
			this->notifyNewPortInfo(inPort.getPath2().c_str(), this->extractPortType(inPort));
		}
	}

	// start the while loop for BFS search to find chains of input connections down the hierarchy of the component
	while(portQueue.size() > 0)
	{
		// Get the front Port
		Udm::Object startPort = portQueue.front();
		// Get srcport name
		string startPortName = startPort.getPath2();
		// remove the front port from the queue
		portQueue.pop();
		// check if the name of the port has already appeared before for some other processed port
		if(processedPortNameSet.find(startPortName) != processedPortNameSet.end())
		{
			throw udm_exception(_T("Multiple Incoming Connections or Duplicacy for Port: ") + MyUdmUtil::getHyperLinkPath_StdString(startPort));
			return FALSE;
		}
		// insert the front ports name in the processed port name list
		processedPortNameSet.insert(startPortName);
		// push it in the processed port set list
		processedPortSet.insert(startPort);
		// collect connected ports
		multiset<Udm::Object> endPortSet = startPort.GetAdjacentObjects();
		for each(Udm::Object endPort in endPortSet)
		{
			// if the endPort is not already traversed port then Notify observers its info and push it in queue
			if(processedPortSet.find(endPort) == processedPortSet.end())
			{
				portQueue.push(endPort);
				// Notify the new port info to the observers
				this->notifyNewPortInfo(endPort.getPath2().c_str(), this->extractPortType(endPort));
				// Notify the connection info between startPort and endPort
				this->notifyNewLinkInfo(startPortName.c_str(), endPort.getPath2().c_str());
			}
			
		}
	}

	// Find the ModelicaComponent's contained OutputSignalInterface Ports and push in queue
	// while  also notifying the observers of ModelHandler the Port's info
	set<OutputSignalInterface>	outPortSet = this->component.OutputSignalInterface_kind_children();
	for each(OutputSignalInterface outPort in outPortSet)
	{
		portQueue.push(outPort);
		// Notify the new port info to the observers
		this->notifyNewPortInfo(outPort.getPath2().c_str(), this->extractPortType(outPort));
	}

	//	ports may be contained in the BusPortInterface
	for each(BusInterface bi in busInterfaceSet)
	{
		set<OutputSignalInterface>	biOutPortSet = bi.OutputSignalInterface_kind_children();
		for each(OutputSignalInterface outPort in biOutPortSet)
		{
			portQueue.push(outPort);
			// Notify the new port info to the observers
			this->notifyNewPortInfo(outPort.getPath2().c_str(), this->extractPortType(outPort));
		}
	}

	// start the while loop for BFS search to find chains of output connections down the hierarchy of the component
	while(portQueue.size() > 0)
	{
		// Get the front Port
		Udm::Object endPort = portQueue.front();
		// Get srcport name
		string endPortName = endPort.getPath2();
		// remove the front port from the queue
		portQueue.pop();
		// check if the name of the port has already appeared before for some other processed port
		if(processedPortNameSet.find(endPortName) != processedPortNameSet.end())
		{
			throw udm_exception(_T("Multiple Incoming Connections or Duplicacy for Port: ") + MyUdmUtil::getHyperLinkPath_StdString(endPort));
			return FALSE;
		}
		// insert the front ports name in the processed port name list
		processedPortNameSet.insert(endPortName);
		// push it in the processed port set list
		processedPortSet.insert(endPort);
		// collect connected ports
		multiset<Udm::Object> startPortSet = endPort.GetAdjacentObjects();
		for each(Udm::Object startPort in startPortSet)
		{
			// if the startPort is not already traversed port then Notify observers its info and push it in queue
			if(processedPortSet.find(startPort) == processedPortSet.end())
			{
				portQueue.push(startPort);
				// Notify the new port info to the observers
				this->notifyNewPortInfo(startPort.getPath2().c_str(), this->extractPortType(startPort));
				// Notify the connection info between startPort and start Port
				this->notifyNewLinkInfo(startPort.getPath2().c_str(), endPortName.c_str());
			}
		}
	}

	// Find the ModelicaComponent's contained ParameterRef and push in queue
	// while  also notifying the observers of ModelHandler the Port's info
	set<ParameterRef>	paramrefPortSet = this->component.ParameterRef_kind_children();
	for each(ParameterRef paramrefPort in paramrefPortSet)
	{
		BOOL isNullRef = FALSE;
		BOOL hasConnections = FALSE;
		// Notify the new ParameterRef info to the observers
		this->notifyNewPortInfo(paramrefPort.getPath2().c_str(), this->extractPortType(paramrefPort));

		if(paramrefPort.getReferencedObject() == Udm::null)
			isNullRef = TRUE;
		if( (paramrefPort.GetAdjacentObjects(ParameterBase::meta).size() > 1 && !isNullRef) ||
			(paramrefPort.GetAdjacentObjects(ParameterBase::meta).size() > 0 && isNullRef) )
			hasConnections = TRUE;
		// ParameterRef cannot be referenced to and directly connected to other ports at the same time
		if(!isNullRef && hasConnections)
		{
			throw udm_exception(_T("ParameterRef can only be directly connected to other ports if \
									it is a Null Reference (not referencing to any object)! @[Object:] ") + 
									MyUdmUtil::getHyperLinkPath_StdString(paramrefPort));
			return FALSE;
		}
		// if the ParameterRef is not null then notify the observers regarding the connection info
		// considering the reference as a virtual connection/link
		// and also notify the observers regarding the connected port info
		if(!isNullRef)
		{
			ParameterBase referencedParam = ParameterBase::Cast(paramrefPort.getReferencedObject());
			this->notifyNewPortInfo(referencedParam.getPath2().c_str(), this->extractPortType(referencedParam));
			this->notifyNewLinkInfo(paramrefPort.getPath2().c_str(), referencedParam.getPath2().c_str());
		}
		// else push in the queue to find the directly connected ports
		else
		portQueue.push(paramrefPort);
	}

	// start the while loop for BFS search to find chains of input connections down the hierarchy of the component
	while(portQueue.size() > 0)
	{
		// Get the front Port
		Udm::Object startPort = portQueue.front();
		// Get srcport name
		string startPortName = startPort.getPath2();
		// remove the front port from the queue
		portQueue.pop();
		// check if the name of the port has already appeared before for some other processed port
		if(processedPortNameSet.find(startPortName) != processedPortNameSet.end())
		{
			throw udm_exception(_T("Multiple Incoming Connections or Duplicacy for Port: ") + MyUdmUtil::getHyperLinkPath_StdString(startPort));
			return FALSE;
		}
		// insert the front ports name in the processed port name list
		processedPortNameSet.insert(startPortName);
		// push it in the processed port set list
		processedPortSet.insert(startPort);
		// collect connected ports
		multiset<Udm::Object> endPortSet = startPort.GetAdjacentObjects();
		for each(Udm::Object endPort in endPortSet)
		{
			// if the endPort is not already traversed port then Notify observers its info and push it in queue
			if(processedPortSet.find(endPort) == processedPortSet.end())
			{
				portQueue.push(endPort);
				// Notify the new port info to the observers
				this->notifyNewPortInfo(endPort.getPath2().c_str(), this->extractPortType(endPort));
				// Notify the connection info between startPort and endPort
				this->notifyNewLinkInfo(startPortName.c_str(), endPort.getPath2().c_str());
			}
			
		}
	}

	return TRUE;
}

void ModelHandler::notifyNewPortInfo(LPCTSTR name, LPCTSTR type)
{
	IMSG iMsg;
	iMsg.fromCtrlId = MODEL_HANDLER;
	iMsg.actionType = IMSG::ADD_PORT;
	iMsg.srcPortName = name;
	iMsg.srcPortType = type;
	this->notify(&iMsg);
}

void ModelHandler::notifyNewLinkInfo(LPCTSTR srcPortName, LPCTSTR dstPortName)
{
	IMSG iMsg;
	iMsg.fromCtrlId = MODEL_HANDLER;
	iMsg.actionType = IMSG::ADD_PORT_LINK;
	iMsg.srcPortName = srcPortName;
	iMsg.dstPortName = dstPortName;
	this->notify(&iMsg);
}


//void ModelHandler::modifyPortInfo(LPCTSTR portName){}
//void ModelHandler::observe(IMSG* pIMsg){}
