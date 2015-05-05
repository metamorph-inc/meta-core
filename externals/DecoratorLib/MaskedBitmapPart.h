//################################################################################################
//
// Masked Bitmap part class (decorator part)
//	MaskedBitmapPart.h
//
//################################################################################################

#ifndef __MASKEDBITMAPPART_H_
#define __MASKEDBITMAPPART_H_


#include "StdAfx.h"
#include "BitmapPart.h"


namespace DecoratorSDK {

//################################################################################################
//
// CLASS : MaskedBitmapPart
//
//################################################################################################

class MaskedBitmapPart: public BitmapPart
{
protected:
	long		m_lBitmapResID;
	COLORREF	m_crTransparent;
	COLORREF	m_crGrayedOut;

public:
	MaskedBitmapPart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink, long bitmapResID,
					 COLORREF transparentColor, COLORREF grayedOutColor);
	virtual ~MaskedBitmapPart();

// =============== resembles IMgaElementDecorator
public:
	virtual void			InitializeEx		(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart,
												 CComPtr<IMgaFCO>& pFCO, HWND parentWnd, PreferenceMap& preferences);

	virtual void			DrawBackground		(CDC* pDC, Gdiplus::Graphics* gdip);
};

}; // namespace DecoratorSDK

#endif //__MASKEDBITMAPPART_H_
