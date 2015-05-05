//################################################################################################
//
// Typeable bitmap part class (decorator part)
//	TypeableBitmapPart.h
//
//################################################################################################

#ifndef __TYPEABLEBITMAPPART_H_
#define __TYPEABLEBITMAPPART_H_


#include "StdAfx.h"
#include "BitmapPart.h"


namespace DecoratorSDK {

//################################################################################################
//
// CLASS : TypeableBitmapPart
//
//################################################################################################

class TypeableBitmapPart: public BitmapPart
{
protected:
	bool				m_bTypeIconEnabled;
	int					m_iTypeInfo;

public:
	TypeableBitmapPart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink);
	virtual ~TypeableBitmapPart();

// =============== resembles IMgaElementDecorator
public:
	virtual void	InitializeEx				(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart,
												 CComPtr<IMgaFCO>& pFCO, HWND parentWnd, PreferenceMap& preferences);

protected:
	virtual void	DrawIcons					(CDC* pDC, Gdiplus::Graphics* gdip);
};

}; // namespace DecoratorSDK

#endif //__TYPEABLEBITMAPPART_H_
