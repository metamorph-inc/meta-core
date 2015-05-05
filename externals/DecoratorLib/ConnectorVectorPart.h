//################################################################################################
//
// Connector vector part decorator class
//	ConnectorVectorPart.h
//
//################################################################################################

#ifndef __CONNECTORVECTORPART_H_
#define __CONNECTORVECTORPART_H_


#include "StdAfx.h"
#include "EllipseVectorPart.h"


namespace DecoratorSDK {

//################################################################################################
//
// CLASS : ConnectorVectorPart
//
//################################################################################################

class ConnectorVectorPart: public EllipseVectorPart
{
public:
	ConnectorVectorPart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink,
						long connectorWidth, long connectorHeight);
	virtual ~ConnectorVectorPart();
};

}; // namespace DecoratorSDK

#endif //__CONNECTORVECTORPART_H_
