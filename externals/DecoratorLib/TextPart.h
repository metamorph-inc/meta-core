//################################################################################################
//
// Text part class (decorator part)
//	TextPart.h
//
//################################################################################################

#ifndef __TEXTPART_H_
#define __TEXTPART_H_


#include "StdAfx.h"
#include "PartBase.h"
#include "ResizeLogic.h"


namespace DecoratorSDK {

//################################################################################################
//
// CLASS : TextPart
//
//################################################################################################

class TextPart: public PartBase
{
protected:
	// Members
	ResizeLogic					resizeLogic;
	CRect						location;

	HCURSOR						m_originalCursor;
	bool						m_bCursorSaved;

	CString						m_strText;
	ELocation					m_eTextLocation;
	bool						m_bTextEnabled;
	bool						m_bTextEditable;
	bool						m_bMultiLine;
	long						m_iTextWrapCount;
	COLORREF					m_crText;
	int							m_iFontKey;
	int							m_iMaxTextLength;

public:
	TextPart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink);
	virtual ~TextPart();

// =============== resembles IMgaElementDecorator
	virtual void			Initialize			(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart,
												 CComPtr<IMgaFCO>& pFCO);
	virtual void			Destroy				(void);
	virtual feature_code	GetFeatures			(void) const;
	virtual CSize			GetPreferredSize	(void) const;
	virtual CRect			GetLabelLocation	(void) const;

	virtual void	InitializeEx				(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart,
												 CComPtr<IMgaFCO>& pFCO, HWND parentWnd, PreferenceMap& preferences);
	virtual bool	MouseMoved					(UINT nFlags, const CPoint& point, HDC transformHDC);
	virtual bool	MouseLeftButtonUp			(UINT nFlags, const CPoint& point, HDC transformHDC);
	virtual bool	MouseLeftButtonDoubleClick	(UINT nFlags, const CPoint& point, HDC transformHDC);
	virtual bool	MouseRightButtonDown		(HMENU hCtxMenu, UINT nFlags, const CPoint& point, HDC transformHDC);
	virtual bool	MenuItemSelected			(UINT menuItemId, UINT nFlags, const CPoint& point, HDC transformHDC);
	virtual bool	OperationCanceledByGME		(void);

	virtual long	GetLongest					(void) const;

	virtual void	SetText						(const CString& text) { m_strText = text; };
	virtual CPoint	GetTextPosition				(CDC* pDC, Gdiplus::Graphics* gdip) const = 0;
	virtual void	SetTextRelYPosition			(long relYPosition) = 0;
	virtual CRect	GetTextLocation				(CDC* pDC, Gdiplus::Graphics* gdip) const = 0;
	virtual void	ExecuteOperation			(const CString& newString) = 0;

protected:
	virtual CRect	GetTextLocation				(void) const;
	virtual bool	HandleTextEditOperation		(bool isDoubleClick, const CPoint& point, HDC transformHDC);
};

}; // namespace DecoratorSDK

#endif //__TEXTPART_H_
