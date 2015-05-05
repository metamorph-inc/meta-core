#if !defined(AFX_INPLACEEDITDIALOG_H__0D3F6303_5878_489e_ADB0_FFBF2A4CA609__INCLUDED_)
#define AFX_INPLACEEDITDIALOG_H__0D3F6303_5878_489e_ADB0_FFBF2A4CA609__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InPlaceEditDialog.h : header file
//

#include "stdafx.h"
#include "DecoratorDefs.h"

namespace DecoratorSDK {
	class TextPart;
}

// CInPlaceEditDialog dialog

class CInPlaceEditDialog : public CDialog
{
	DECLARE_DYNAMIC(CInPlaceEditDialog)

public:
	CInPlaceEditDialog();

	virtual ~CInPlaceEditDialog();

	CRichEditCtrl* m_richWnd;
	CString	m_Text;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void MeasureText(CDC* cdc, CSize& minSize, CSize& cSize);
	bool IsAsciiString(const CString& textToValidate);
	bool IsValidString(const CString& textToValidate);
	void SignalBackToGme(void);

	DECLARE_MESSAGE_MAP()
public:
	// Generated message map functions
	//{{AFX_MSG(CInPlaceEditDialog)
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnNcActivate(BOOL bActive);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnRequestResize(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

protected:
	bool					m_bToInit;
	bool					m_bClosed;
	bool					m_bSignaledBack;
	DecoratorSDK::TextPart*	m_parentPart;
	CWnd*					m_intendedParentCWnd;
	bool					m_bPermanentCWnd;
	CWnd*					m_realParentCWnd;
	HDC						m_transformHDC;
	CRect					m_initialRect;
	CRect					m_labelRect;
	CSize					m_minSize;
	CRect					m_boundsLimit;
	CPoint					m_mouseClick;	// Screen coordinates of the mouse click which invoked the in-place edit
	long					m_iFontKey;
	CFont*					m_font;
	bool					m_bInflateToRight;
	bool					m_bMultiLine;
	bool					m_bDlgResult;

public:
	void	SetProperties(const CString& text, DecoratorSDK::TextPart* parentPart, const CRect& labelRect,
						  const CPoint& mouseClick, CWnd* parentCWnd, bool isPermanentCWnd, HDC transformHDC,
						  long iFontKey, bool inflateToRight = true, bool multiLine = false);
	CString GetText() const;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInPlaceEditDialog)
	public:
    virtual void EndDialog(int nResult);
	//}}AFX_VIRTUAL
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPLACEEDITDIALOG_H__0D3F6303_5878_489e_ADB0_FFBF2A4CA609__INCLUDED_)
