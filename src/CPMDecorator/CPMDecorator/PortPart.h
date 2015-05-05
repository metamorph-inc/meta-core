//################################################################################################
//
// Port part class (decorator part)
//	PortPart.h
//
//################################################################################################

#ifndef __NPORTPART_H_
#define __NPORTPART_H_


#include "StdAfx.h"
#include "ObjectAndTextPart.h"
using namespace DecoratorSDK;

namespace Decor {

//################################################################################################
//
// CLASS : PortPart
//
//################################################################################################

class PortPart: public ObjectAndTextPart
{
public:
	PortPart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink, const CPoint& ptInner, _bstr_t icon);//code-changed
	virtual ~PortPart();

public:
	virtual CPoint	GetInnerPosition			(void) const;
	virtual long	GetLongest					(void) const;
};

}; // namespace Decor

#endif //__PORTPART_H_
