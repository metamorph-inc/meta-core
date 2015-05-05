//################################################################################################
//
// Resize logic common codes class
//	ResizeLogic.h
//
//################################################################################################

#ifndef __RESIZELOGIC_H_
#define __RESIZELOGIC_H_


#include "StdAfx.h"
#include "PartBase.h"
#include "SizeTrackerDialog.h"


namespace DecoratorSDK {

//################################################################################################
//
// CLASS : ResizeLogic
//
//################################################################################################

class ResizeLogic
{
public:
	enum ResizeFeatures {
		Resizeable				= 0x0001,
		Movable					= 0x0002,
		DrawResizeCorner		= 0x0004,
		DrawSelectionRectangle	= 0x0008
	};

protected:
	PartBase*			m_parentPart;
	HWND				m_parentWnd;

	ResizeType			m_resizeState;
	short				m_resizeFeatures;
	CRect				m_targetLocation;

	CRect				m_originalLocation;
	CPoint				m_originalMousePosition;
	HCURSOR				m_originalCursor;
	bool				m_bCursorSaved;
	CSize				m_minSize;

	CSizeTrackerDialog*	sizeTrackerDlg;

public:
	ResizeLogic(PartBase* pPart, HWND parentWnd);
	virtual ~ResizeLogic();

	virtual short	GetResizeFeatures			(void) const;
	virtual void	SetResizeFeatures			(short resizeFeatures);
	virtual CRect	GetResizeTargetLocation		(void) const;
	virtual void	SetResizeTargetLocation		(CRect targetLocation);
	virtual void	SetParentPart				(PartBase* pPart);

// =============== resembles IMgaElementDecorator
	virtual void	Initialize					(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart,
												 CComPtr<IMgaFCO>& pFCO);
	virtual void	Destroy						(void);
	virtual void	Draw						(CDC* pDC, Gdiplus::Graphics* gdip);

	virtual void	InitializeEx				(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart,
												 CComPtr<IMgaFCO>& pFCO, HWND parentWnd, PreferenceMap& preferences);
	virtual bool	MouseMoved					(UINT nFlags, const CPoint& point, HDC transformHDC);
	virtual bool	MouseLeftButtonDown			(UINT nFlags, const CPoint& point, HDC transformHDC);
	virtual bool	MouseLeftButtonUp			(UINT nFlags, const CPoint& point, HDC transformHDC);
	virtual bool	OperationCanceledByGME		(void);

	ResizeType		DeterminePotentialResize	(CPoint cursorPoint) const;
	bool			IsSizeChanged				(void) const;
	CRect			GetOriginalLocation			(void) const;
	void			SetMinimumSize				(CSize minSize);

private:
	void			ChangeCursorForm			(ResizeType resizeType, bool notify = true);
	void			RestoreCursor				(void);
};

}; // namespace DecoratorSDK

#endif //__RESIZELOGIC_H_
