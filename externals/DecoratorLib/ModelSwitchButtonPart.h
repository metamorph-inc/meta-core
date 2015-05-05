//################################################################################################
//
// Model switch button part class (decorator part)
//	ModelSwitchButtonPart.h
//
//################################################################################################

#ifndef __MODELSWITCHBUTTONPART_H_
#define __MODELSWITCHBUTTONPART_H_


#include "StdAfx.h"
#include "PartBase.h"
#include "MaskedBitmapPart.h"


namespace DecoratorSDK {

//################################################################################################
//
// CLASS : ModelSwitchButtonPart
//
//################################################################################################

class ModelSwitchButtonPart: public MaskedBitmapPart
{
protected:
	HCURSOR				m_originalCursor;
	bool				m_bCursorSaved;

public:
	ModelSwitchButtonPart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink);
	virtual ~ModelSwitchButtonPart();

// =============== resembles IMgaElementDecorator
public:
	virtual void	InitializeEx				(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart,
												 CComPtr<IMgaFCO>& pFCO, HWND parentWnd, PreferenceMap& preferences);
	virtual bool	MouseMoved					(UINT nFlags, const CPoint& point, HDC transformHDC);
	virtual bool	MouseLeftButtonDown			(UINT nFlags, const CPoint& point, HDC transformHDC);
	virtual bool	MouseLeftButtonUp			(UINT nFlags, const CPoint& point, HDC transformHDC);
	virtual bool	MouseLeftButtonDoubleClick	(UINT nFlags, const CPoint& point, HDC transformHDC);
	virtual bool	MouseRightButtonDown		(HMENU hCtxMenu, UINT nFlags, const CPoint& point, HDC transformHDC);
	virtual bool	MenuItemSelected			(UINT menuItemId, UINT nFlags, const CPoint& point, HDC transformHDC);

protected:
	void			InitiateModelExpandStateSwitch(void);
};

}; // namespace DecoratorSDK

#endif //__MODELSWITCHBUTTONPART_H_
