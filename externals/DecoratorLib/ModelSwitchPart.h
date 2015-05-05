//################################################################################################
//
// Model switch part class (decorator part)
//	ModelSwitchPart.h
//
//################################################################################################

#ifndef __MODELSWITCHPART_H_
#define __MODELSWITCHPART_H_


#include "StdAfx.h"
#include "TypeableBitmapPart.h"
#include "PortPart.h"

namespace DecoratorSDK {

//################################################################################################
//
// CLASS : ModelSwitchPart
//
//################################################################################################

class ModelSwitchPart: public CompositePart
{
public:
	ModelSwitchPart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink);
	virtual ~ModelSwitchPart();

// =============== resembles IMgaElementDecorator
public:
	virtual void	SetLocation					(const CRect& location);
	virtual void	InitializeEx				(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart,
												 CComPtr<IMgaFCO>& pFCO, HWND parentWnd, PreferenceMap& preferences);

	virtual bool	MouseMoved					(UINT nFlags, const CPoint& point, HDC transformHDC);	// default (no) implementation
	virtual bool	MouseLeftButtonDown			(UINT nFlags, const CPoint& point, HDC transformHDC);	// default (no) implementation
	virtual bool	MouseLeftButtonUp			(UINT nFlags, const CPoint& point, HDC transformHDC);	// default (no) implementation
	virtual bool	MouseLeftButtonDoubleClick	(UINT nFlags, const CPoint& point, HDC transformHDC);	// default (no) implementation
	virtual bool	MouseRightButtonDown		(HMENU hCtxMenu, UINT nFlags, const CPoint& point, HDC transformHDC);	// default (no) implementation
	virtual bool	MenuItemSelected			(UINT menuItemId, UINT nFlags, const CPoint& point, HDC transformHDC);	// default (no) implementation
	virtual void	SetBoxLocation				(const CRect& cRect);

protected:
	void			SwapParts					(void);
};

}; // namespace DecoratorSDK

#endif //__MODELSWITCHPART_H_
