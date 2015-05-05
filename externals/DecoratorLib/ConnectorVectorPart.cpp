//################################################################################################
//
// Connector vector part decorator class
//	ConnectorVectorPart.cpp
//
//################################################################################################

#include "StdAfx.h"
#include "ConnectorVectorPart.h"

#include "DecoratorDefs.h"


namespace DecoratorSDK {

//################################################################################################
//
// CLASS : ConnectorVectorPart
//
//################################################################################################

ConnectorVectorPart::ConnectorVectorPart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink,
										 long connectorWidth, long connectorHeight):
	EllipseVectorPart(pPart, eventSink, connectorWidth, connectorHeight)
{
	brushColorVariableName	= PREF_COLOR;
}

ConnectorVectorPart::~ConnectorVectorPart()
{
}

}; // namespace DecoratorSDK
