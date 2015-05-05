//################################################################################################
//
// Model complex part class (decorator part)
//	ModelComplexPart.h
//
//################################################################################################

#ifndef __MODELCOMPLEXPART_H_
#define __MODELCOMPLEXPART_H_


#include "StdAfx.h"
#include "TypeableBitmapPart.h"
#include "PortPart.h"


namespace DecoratorSDK {

class PortPartData;

//################################################################################################
//
// CLASS : ModelComplexPart
//
//################################################################################################

class ModelComplexPart: public TypeableBitmapPart
{
protected:
	std::vector<PortPartData*>	m_AllPorts;
	std::vector<PortPart*>		m_LeftPorts;
	std::vector<PortPart*>		m_RightPorts;
	long						m_iMaxPortTextLength;
	COLORREF					m_crPortText;
	bool						m_bPortLabelInside;
	long						m_iLongestPortTextLength;

public:
	ModelComplexPart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink);
	virtual ~ModelComplexPart();

// =============== resembles IMgaElementDecorator
public:
	virtual void			Initialize			(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart,
												 CComPtr<IMgaFCO>& pFCO);
	virtual void			Destroy				(void);
	virtual CString			GetMnemonic			(void) const;
	virtual feature_code	GetFeatures			(void) const;
	virtual void			SetParam			(const CString& strName, VARIANT vValue);
	virtual bool			GetParam			(const CString& strName, VARIANT* pvValue);
	virtual void			SetActive			(bool bIsActive);
	virtual CSize			GetPreferredSize	(void) const;
	virtual void			SetLocation			(const CRect& location);
	virtual CRect			GetLocation			(void) const;
	virtual CRect			GetLabelLocation	(void) const;
	virtual CRect			GetPortLocation		(CComPtr<IMgaFCO>& fco) const;
	virtual bool			GetPorts			(CComPtr<IMgaFCOs>& portFCOs) const;
	virtual void			Draw				(CDC* pDC, Gdiplus::Graphics* gdip);
	virtual void			SaveState			(void);

	virtual void	InitializeEx				(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart,
												 CComPtr<IMgaFCO>& pFCO, HWND parentWnd, PreferenceMap& preferences);
	virtual void	SetSelected					(bool bIsSelected);
	virtual bool	MouseMoved					(UINT nFlags, const CPoint& point, HDC transformHDC);
	virtual bool	MouseLeftButtonDown			(UINT nFlags, const CPoint& point, HDC transformHDC);
	virtual bool	MouseLeftButtonUp			(UINT nFlags, const CPoint& point, HDC transformHDC);
	virtual bool	MouseLeftButtonDoubleClick	(UINT nFlags, const CPoint& point, HDC transformHDC);
	virtual bool	MouseRightButtonDown		(HMENU hCtxMenu, UINT nFlags, const CPoint& point, HDC transformHDC);
	virtual bool	MouseRightButtonUp			(UINT nFlags, const CPoint& point, HDC transformHDC);
	virtual bool	MouseRightButtonDoubleClick	(UINT nFlags, const CPoint& point, HDC transformHDC);
	virtual bool	MouseMiddleButtonDown		(UINT nFlags, const CPoint& point, HDC transformHDC);
	virtual bool	MouseMiddleButtonUp			(UINT nFlags, const CPoint& point, HDC transformHDC);
	virtual bool	MouseMiddleButtonDoubleClick(UINT nFlags, const CPoint& point, HDC transformHDC);
	virtual bool	MouseWheelTurned			(UINT nFlags, short distance, const CPoint& point, HDC transformHDC);
	virtual bool	DragEnter					(DROPEFFECT* dropEffect, COleDataObject* pDataObject, DWORD dwKeyState, const CPoint& point, HDC transformHDC);
	virtual bool	DragOver					(DROPEFFECT* dropEffect, COleDataObject* pDataObject, DWORD dwKeyState, const CPoint& point, HDC transformHDC);
	virtual bool	Drop						(COleDataObject* pDataObject, DROPEFFECT dropEffect, const CPoint& point, HDC transformHDC);
	virtual bool	DropFile					(HDROP p_hDropInfo, const CPoint& point, HDC transformHDC);
	virtual bool	MenuItemSelected			(UINT menuItemId, UINT nFlags, const CPoint& point, HDC transformHDC);
	virtual bool	OperationCanceledByGME		(void);

	virtual void	DrawBackground				(CDC* pDC, Gdiplus::Graphics* gdip);

	virtual void 	LoadPorts					(void);
	virtual void 	OrderPorts					();
	virtual void 	ReOrderConnectedOnlyPorts	();
	virtual void	SetBoxLocation				(const CRect& cRect);
	virtual void	SetReferenced				(bool referenced);
	virtual void	SetParentPart				(PartBase* pPart);
protected:
	PortPart*		GetPort						(CComPtr<IMgaFCO> spFCO) const;
};

}; // namespace DecoratorSDK

#endif //__MODELCOMPLEXPART_H_
