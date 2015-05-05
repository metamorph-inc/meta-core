//################################################################################################
//
// Port part class (decorator part)
//	PortPart.cpp
//
//################################################################################################

#include "StdAfx.h"
#include "PortPart.h"

#include "PortBitmapPart.h"
#include "PortLabelPart.h"


namespace Decor {

//################################################################################################
//
// CLASS : PortPart
//
//################################################################################################
//code-changed
PortPart::PortPart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink, const CPoint& ptInner, _bstr_t icon):
	ObjectAndTextPart(pPart, eventSink)
{
	Decor::PortBitmapPart* portBitmapPart = new Decor::PortBitmapPart(this, eventSink, ptInner, icon);
	PortLabelPart* portLabelPart = new PortLabelPart(this, eventSink);
	AddObjectPart(portBitmapPart);
	AddTextPart(portLabelPart);
}

PortPart::~PortPart()
{
}

CPoint PortPart::GetInnerPosition(void) const
{
#ifdef _DEBUG
	PortBitmapPart* portBitmapPart = dynamic_cast<PortBitmapPart*> (GetObjectPart());
#else
	PortBitmapPart* portBitmapPart = static_cast<PortBitmapPart*> (GetObjectPart());
#endif
	ASSERT(portBitmapPart != NULL);
	return portBitmapPart->GetInnerPosition();
}

long PortPart::GetLongest(void) const
{
#ifdef _DEBUG
	PortLabelPart* portLabelPart = dynamic_cast<PortLabelPart*> (GetTextPart());
#else
	PortLabelPart* portLabelPart = static_cast<PortLabelPart*> (GetTextPart());
#endif
	ASSERT(portLabelPart != NULL);
	return portLabelPart->GetLongest();
}

}; // namespace Decor
