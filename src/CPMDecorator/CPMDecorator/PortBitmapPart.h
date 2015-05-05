//################################################################################################
//
// Port bitmap part decorator class (decorator part)
//	PortBitmapPart.h
//
//################################################################################################

#ifndef __NPORTBITMAPPART_H_
#define __NPORTBITMAPPART_H_


#include "StdAfx.h"
#include "TypeableBitmapPart.h"
using namespace DecoratorSDK;

namespace Decor {

//################################################################################################
//
// CLASS : PortBitmapPart
//
//################################################################################################

class PortBitmapPart: public TypeableBitmapPart
{
protected:
	CPoint					m_ptInner;
	_bstr_t m_icon;

public:
	PortBitmapPart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink, const CPoint& ptInner, _bstr_t icon);
	virtual ~PortBitmapPart();

// =============== resembles IMgaElementDecorator
public:
	virtual CSize	GetPreferredSize			(void) const;
	virtual void	InitializeEx				(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart,
												 CComPtr<IMgaFCO>& pFCO, HWND parentWnd, PreferenceMap& preferences);

	virtual void	DrawBackground				(CDC* pDC, Gdiplus::Graphics* gdip);

	virtual CPoint	GetInnerPosition			(void) const { return m_ptInner; };
};

}; // namespace Decor

#endif //__PORTBITMAPPART_H_
