//################################################################################################
//
// Resizable part class (decorator part)
//	ResizablePart.h
//
//################################################################################################

#ifndef __RESIZABLEPART_H_
#define __RESIZABLEPART_H_


#include "StdAfx.h"
#include "PartBase.h"
#include "ResizeLogic.h"


namespace DecoratorSDK {

class CSetSizeDialog;

//################################################################################################
//
// CLASS : ResizablePart
//
//################################################################################################

class ResizablePart: public PartBase
{
protected:
	ResizeLogic		resizeLogic;
	CSetSizeDialog*	m_setSizeDlg;
	bool			inSizeDlgProcedure;

public:
	ResizablePart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink);
	virtual ~ResizablePart();

	// This is a trick to speed up dynamic_cast
	virtual ResizablePart*	dynamic_cast_ResizablePart(void)	{ return this; }

// =============== resembles IMgaElementDecorator
	virtual void			Initialize			(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart,
												 CComPtr<IMgaFCO>& pFCO);
	virtual void			Destroy				(void);
	virtual feature_code	GetFeatures			(void) const;
	virtual CSize			GetPreferredSize	(void) const;
	virtual void			SetLocation			(const CRect& location);
	virtual CRect			GetLocation			(void) const;
	virtual void			Draw				(CDC* pDC, Gdiplus::Graphics* gdip);

	virtual void	InitializeEx				(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart,
												 CComPtr<IMgaFCO>& pFCO, HWND parentWnd, PreferenceMap& preferences);
	virtual bool	MouseMoved					(UINT nFlags, const CPoint& point, HDC transformHDC);
	virtual bool	MouseLeftButtonDown			(UINT nFlags, const CPoint& point, HDC transformHDC);
	virtual bool	MouseLeftButtonUp			(UINT nFlags, const CPoint& point, HDC transformHDC);
	virtual bool	MouseRightButtonDown		(HMENU hCtxMenu, UINT nFlags, const CPoint& point, HDC transformHDC);
	virtual bool	MenuItemSelected			(UINT menuItemId, UINT nFlags, const CPoint& point, HDC transformHDC);
	virtual bool	OperationCanceledByGME		(void);

	virtual void	WindowResizing				(UINT nFlags, CRect& location);
	virtual void	WindowResizingFinished		(UINT nFlags, CRect& location);
};

}; // namespace DecoratorSDK

#endif //__RESIZABLEPART_H_
