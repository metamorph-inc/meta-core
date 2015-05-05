//################################################################################################
//
// Port bitmap part decorator class (decorator part)
//	PortBitmapPart.h
//
//################################################################################################

#ifndef __PORTBITMAPPART_H_
#define __PORTBITMAPPART_H_


#include "StdAfx.h"
#include "TypeableBitmapPart.h"


namespace DecoratorSDK {

//################################################################################################
//
// CLASS : PortBitmapPart
//
//################################################################################################

class PortBitmapPart: public TypeableBitmapPart
{
protected:
	CPoint					m_ptInner;

public:
	PortBitmapPart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink, const CPoint& ptInner);
	virtual ~PortBitmapPart();

// =============== resembles IMgaElementDecorator
public:
	virtual CSize	GetPreferredSize			(void) const;
	virtual void	InitializeEx				(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart,
												 CComPtr<IMgaFCO>& pFCO, HWND parentWnd, PreferenceMap& preferences);

	virtual void	DrawBackground				(CDC* pDC, Gdiplus::Graphics* gdip);

	virtual CPoint	GetInnerPosition			(void) const { return m_ptInner; };
};

}; // namespace DecoratorSDK

#endif //__PORTBITMAPPART_H_
