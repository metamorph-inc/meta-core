//################################################################################################
//
// Set bitmap part class (decorator part)
//	SetBitmapPart.h
//
//################################################################################################

#ifndef __SETBITMAPPART_H_
#define __SETBITMAPPART_H_


#include "StdAfx.h"
#include "TypeableBitmapPart.h"


namespace DecoratorSDK {

//################################################################################################
//
// CLASS : SetBitmapPart
//
//################################################################################################

class SetBitmapPart: public TypeableBitmapPart
{
public:
	SetBitmapPart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink);
	virtual ~SetBitmapPart();

// =============== resembles IMgaElementDecorator
public:
	virtual void	InitializeEx				(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart,
												 CComPtr<IMgaFCO>& pFCO, HWND parentWnd, PreferenceMap& preferences);

	virtual void	DrawBackground				(CDC* pDC, Gdiplus::Graphics* gdip);
};

}; // namespace DecoratorSDK

#endif //__SETBITMAPPART_H_
