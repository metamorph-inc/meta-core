#if !defined(AFX_SETSIZEDIALOG_H__8348FAF6_E992_41da_91EC_628E36C4CC89__INCLUDED_)
#define AFX_SETSIZEDIALOG_H__8348FAF6_E992_41da_91EC_628E36C4CC89__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetSizeDialog.h : header file
//

#include "stdafx.h"
#include "DecoratorDefs.h"

namespace DecoratorSDK {

class ResizablePart;

enum SetSizeDialogSize {
	SetSizeDialogWidth		= 118 + 64,
	SetSizeDialogHeight		= 58 + 72,
	SetSizeDialogMargin		= 10
};

enum SetSizeControls {
	WidthEditCtrlId			= 1000,
	HeightEditCtrlId		= 1001,
	WidthTextCtrlId			= 1002,
	HeightTextCtrlId		= 1003
};

// CSetSizeDialog dialog

class CSetSizeDialog : public CDialog
{
	DECLARE_DYNAMIC(CSetSizeDialog)

public:
	CSetSizeDialog();

	virtual ~CSetSizeDialog();

	CEdit* m_widthEditWnd;
	CEdit* m_heightEditWnd;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	// Generated message map functions
	//{{AFX_MSG(CSetSizeDialog)
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnNcActivate(BOOL bActive);
//	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	//}}AFX_MSG

protected:
	bool							m_bInited;
	bool							m_bClosed;
	DecoratorSDK::ResizablePart*	m_parentPart;
	CWnd*							m_intendedParentCWnd;
	bool							m_bPermanentCWnd;
	CWnd*							m_realParentCWnd;
	HDC								m_transformHDC;
	CRect							m_boundsLimit;
	CPoint							m_mouseClick;	// Screen coordinates of the mouse click which invoked the in-place edit
	CRect							m_initialRect;
	bool							m_bDlgResult;
	ResizeType						m_resizeType;
	UINT							m_nFlags;
	long							m_lWidth;
	long							m_lHeight;

	CPoint	GetAdjustedPosition(const CPoint& pt);
	bool	GetSizeAndIsValid(void);

public:
	void	SetParameters(ResizablePart* parentPart, const CRect& initialRect, const CPoint& mouseClick,
						  ResizeType resizeType, CWnd* parentCWnd, bool isPermanentCWnd, HDC transformHDC, UINT nFlags);
	void	AdjustPositionAndValues(const CPoint& point, const CRect& location);
	bool	GetDlgResult(void) const;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetSizeDialog)
	public:
    virtual void EndDialog(int nResult);
	//}}AFX_VIRTUAL
};

}

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETSIZEDIALOG_H__8348FAF6_E992_41da_91EC_628E36C4CC89__INCLUDED_)
