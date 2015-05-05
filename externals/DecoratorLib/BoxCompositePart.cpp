//################################################################################################
//
// New Box decorator composite part class
//	BoxCompositePart.cpp
//
//################################################################################################

#include "StdAfx.h"
#include "BoxCompositePart.h"

#include "ModelComplexPart.h"
#include "TypeableLabelPart.h"
#include "ReferenceBitmapPart.h"
#include "PortPart.h"
#include "AtomBitmapPart.h"
#include "SetBitmapPart.h"
#include "DecoratorExceptions.h"


namespace DecoratorSDK {

//################################################################################################
//
// CLASS : BoxCompositePart
//
//################################################################################################

BoxCompositePart::BoxCompositePart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink):
	ObjectAndTextPart(pPart, eventSink)
{
}

BoxCompositePart::~BoxCompositePart()
{
}

CRect BoxCompositePart::GetPortLocation(CComPtr<IMgaFCO>& fco) const
{
	CRect portLocation = GetObjectPart()->GetPortLocation(fco);
	if (!portLocation.IsRectEmpty()) {
		// if a reference has an icon specified for itself 
		// then it is not surrounded by a black rectangle.
		// if it doesn't have an icon, then the icon of the
		// referred element is used, and it is surrounded
		// that's why we shift port locations only if
		// the surrounding rectangle is there (borderwidth > 0)
		if (GetObjectPart()->GetBorderWidth(false) > 0 && m_eType == OBJTYPE_REFERENCE)	// HACK
			portLocation.OffsetRect(2, 2);

		return portLocation;
	}

	throw PortNotFoundException();
}

CRect BoxCompositePart::GetLabelLocation(void) const
{
	HRESULT retVal = S_OK;
	// The second part is the label
	std::vector<PartBase*>::const_iterator ii = m_compositeParts.begin();
	 ++ii;
	if (ii != m_compositeParts.end()) {
		try {
			return (*ii)->GetLabelLocation();
		}
		catch(hresult_exception& e) {
			retVal = e.hr;
		}
		catch(DecoratorException& e) {
			retVal = e.GetHResult();
		}
	}

	throw DecoratorException((DecoratorExceptionCode)retVal);
}

// New functions
void BoxCompositePart::InitializeEx(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart, CComPtr<IMgaFCO>& pFCO,
									HWND parentWnd, PreferenceMap& preferences)
{
	HRESULT retVal = S_OK;
	if (pProject && pPart) {
		if (pFCO) {
			COMTHROW(pFCO->get_ObjType(&m_eType));
		} else {
			CComPtr<IMgaMetaRole> spRole;
			COMTHROW(pPart->get_Role(&spRole));

			CComPtr<IMgaMetaFCO> spMetaFCO;
			COMTHROW(spRole->get_Kind(&spMetaFCO));

			COMTHROW(spMetaFCO->get_ObjType(&m_eType));
		}
		switch (m_eType) {
			case OBJTYPE_ATOM: {
					AddObjectPart(new AtomBitmapPart(this, m_eventSink));
					AddTextPart(new TypeableLabelPart(this, m_eventSink));
				}
				break;
			case OBJTYPE_SET: {
					AddObjectPart(new SetBitmapPart(this, m_eventSink));
					AddTextPart(new TypeableLabelPart(this, m_eventSink));
				}
				break;
			case OBJTYPE_MODEL: {
					AddObjectPart(new ModelComplexPart(this, m_eventSink));
					AddTextPart(new TypeableLabelPart(this, m_eventSink));
				}
				break;
			case OBJTYPE_REFERENCE: {
					AddObjectPart(new ReferenceBitmapPart(this, m_eventSink));
					AddTextPart(new TypeableLabelPart(this, m_eventSink));
				}
				break;
		}
	}

	CompositePart::InitializeEx(pProject, pPart, pFCO, parentWnd, preferences);
}

void BoxCompositePart::InitializeEx(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart, CComPtr<IMgaFCO>& pFCO,
									HWND parentWnd)
{
	getFacilities().loadPathes(pProject, true);

	PreferenceMap preferencesMap;
	InitializeEx(pProject, pPart, pFCO, parentWnd, preferencesMap);
}

}; // namespace DecoratorSDK
