//################################################################################################
//
// Vector and label composite part class (decorator part)
//	VectorAndLabelPart.cpp
//
//################################################################################################

#include "StdAfx.h"
#include "VectorAndLabelPart.h"
#include "VectorPart.h"
#include "LabelPart.h"
#include "DecoratorExceptions.h"


namespace DecoratorSDK {

//################################################################################################
//
// CLASS : VectorAndLabelPart
//
//################################################################################################

VectorAndLabelPart::VectorAndLabelPart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink):
	ObjectAndTextPart(pPart, eventSink)
{
}

VectorAndLabelPart::~VectorAndLabelPart()
{
	for (std::vector<PartBase*>::iterator ii = m_compositeParts.begin(); ii != m_compositeParts.end(); ++ii) {
		if ((*ii) != NULL) {
			delete (*ii);
		}
	}
	m_compositeParts.clear();
}

void VectorAndLabelPart::AddVectorPart(VectorPart* part)
{
	AddObjectPart(part);
}

VectorPart* VectorAndLabelPart::GetVectorPart(void) const
{
	ASSERT(m_compositeParts.size() >= 1);
#ifdef _DEBUG
	VectorPart* part = dynamic_cast<VectorPart*> (GetObjectPart());
	ASSERT(part != NULL);
	return part;
#else
	return static_cast<VectorPart*> (GetObjectPart());
#endif
}

}; // namespace DecoratorSDK
