//################################################################################################
//
// Atom bitmap part class (decorator part)
//	AtomBitmapPart.h
//
//################################################################################################

#ifndef __ATOMBITMAPPART_H_
#define __ATOMBITMAPPART_H_


#include "StdAfx.h"
#include "TypeableBitmapPart.h"


namespace DecoratorSDK {

//################################################################################################
//
// CLASS : AtomBitmapPart
//
//################################################################################################

class AtomBitmapPart: public TypeableBitmapPart
{
public:
	AtomBitmapPart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink);
	virtual ~AtomBitmapPart();

// =============== resembles IMgaElementDecorator
public:
	virtual void	InitializeEx				(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart,
												 CComPtr<IMgaFCO>& pFCO, HWND parentWnd, PreferenceMap& preferences);

	virtual void	DrawBackground				(CDC* pDC, Gdiplus::Graphics* gdip);
};

}; // namespace DecoratorSDK

#endif //__ATOMPART_H_
