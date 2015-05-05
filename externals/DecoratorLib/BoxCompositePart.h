//################################################################################################
//
// New Box decorator composite part class
//	BoxCompositePart.h
//
//################################################################################################

#ifndef __BOXCOMPOSITEPART_H_
#define __BOXCOMPOSITEPART_H_


#include "StdAfx.h"
#include "ObjectAndTextPart.h"


namespace DecoratorSDK {

//################################################################################################
//
// CLASS : BoxCompositePart
//
//################################################################################################

class BoxCompositePart: public ObjectAndTextPart
{
public:
	BoxCompositePart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink);
	virtual ~BoxCompositePart();

// =============== resembles IMgaElementDecorator
public:
	virtual CRect	GetPortLocation				(CComPtr<IMgaFCO>& fco) const;
	virtual CRect	GetLabelLocation			(void) const;

	virtual void	InitializeEx				(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart,
												 CComPtr<IMgaFCO>& pFCO, HWND parentWnd, PreferenceMap& preferences);
	virtual void	InitializeEx				(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart,
												 CComPtr<IMgaFCO>& pFCO, HWND parentWnd);
};

}; // namespace DecoratorSDK

#endif //__BOXCOMPOSITEPART_H_
