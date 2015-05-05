//################################################################################################
//
// Vector and label composite part class (decorator part)
//	VectorAndLabelPart.h
//
//################################################################################################

#ifndef __VECTORANDLABELPART_H_
#define __VECTORANDLABELPART_H_


#include "StdAfx.h"
#include "ObjectAndTextPart.h"
#include <vector>


namespace DecoratorSDK {

class VectorPart;

//################################################################################################
//
// CLASS : VectorAndLabelPart
//
//################################################################################################

class VectorAndLabelPart: public ObjectAndTextPart
{
public:
	VectorAndLabelPart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink);
	virtual ~VectorAndLabelPart();

public:
	virtual void				AddVectorPart	(VectorPart* part);
	virtual VectorPart*			GetVectorPart	(void) const;
};

}; // namespace DecoratorSDK

#endif //__VECTORANDLABELPART_H_
