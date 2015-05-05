//################################################################################################
//
// Object and text composite part class (decorator part)
//	ObjectAndTextPart.cpp
//
//################################################################################################

#include "StdAfx.h"
#include "ObjectAndTextPart.h"

#include "TextPart.h"
#include "DecoratorExceptions.h"


namespace DecoratorSDK {

//################################################################################################
//
// CLASS : ObjectAndTextPart
//
//################################################################################################

ObjectAndTextPart::ObjectAndTextPart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink):
	CompositePart(pPart, eventSink)
{
}

ObjectAndTextPart::~ObjectAndTextPart()
{
	for (std::vector<PartBase*>::iterator ii = m_compositeParts.begin(); ii != m_compositeParts.end(); ++ii) {
		if ((*ii) != NULL) {
			delete (*ii);
		}
	}
	m_compositeParts.clear();
}

void ObjectAndTextPart::AddObjectPart(PartBase* part)
{
	ASSERT(part != NULL);
	ASSERT(m_compositeParts.size() == 0);

	part->SetParentPart(this);
	m_compositeParts.push_back(part);
}

void ObjectAndTextPart::AddTextPart(TextPart* part)
{
	ASSERT(part != NULL);
	ASSERT(m_compositeParts.size() == 1);

	part->SetParentPart(this);
	m_compositeParts.push_back(part);
}

PartBase* ObjectAndTextPart::GetObjectPart(void) const
{
	ASSERT(m_compositeParts.size() >= 1);
	return m_compositeParts[0];
}

TextPart* ObjectAndTextPart::GetTextPart(void) const
{
	ASSERT(m_compositeParts.size() == 2);
#ifdef _DEBUG
	TextPart* part = dynamic_cast<TextPart*> (m_compositeParts[1]);
	ASSERT(part != NULL);
	return part;
#else
	return static_cast<TextPart*> (m_compositeParts[1]);
#endif
}

}; // namespace DecoratorSDK
