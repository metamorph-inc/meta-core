//################################################################################################
//
// Object and text composite part class (decorator part)
//	ObjectAndTextPart.h
//
//################################################################################################

#ifndef __OBJECTANDTEXTPART_H_
#define __OBJECTANDTEXTPART_H_


#include "StdAfx.h"
#include "CompositePart.h"
#include <vector>


namespace DecoratorSDK {

class TextPart;

//################################################################################################
//
// CLASS : ObjectAndTextPart
//
//################################################################################################

class ObjectAndTextPart: public CompositePart
{
public:
	ObjectAndTextPart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink);
	virtual ~ObjectAndTextPart();

public:		// new functions
	virtual void			AddObjectPart		(PartBase* part);
	virtual void			AddTextPart			(TextPart* part);
	virtual PartBase*		GetObjectPart		(void) const;
	virtual TextPart*		GetTextPart			(void) const;
};

}; // namespace DecoratorSDK

#endif //__OBJECTANDTEXTPART_H_
