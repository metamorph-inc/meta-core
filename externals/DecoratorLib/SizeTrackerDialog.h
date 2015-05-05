#if !defined(AFX_SIZETRACKERDIALOG_H__83F5D328_52B2_4c8b_8239_3BE9FF315196__INCLUDED_)
#define AFX_SIZETRACKERDIALOG_H__83F5D328_52B2_4c8b_8239_3BE9FF315196__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SizeTrackerDialog.h : header file
//

#include "stdafx.h"
#include "DecoratorDefs.h"

namespace DecoratorSDK {

enum SizeTrackerSize {
	SizeTrackerWidth		= 86,
	SizeTrackerHeight		= 16,
	SizeTrackerMargin		= 10
};

// CSizeTrackerDialog dialog

class CSizeTrackerDialog : public CDialog
{
	DECLARE_DYNAMIC(CSizeTrackerDialog)

public:
	CSizeTrackerDialog();

	virtual ~CSizeTrackerDialog();

	CStatic* m_staticWnd;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy(void);

	DECLARE_MESSAGE_MAP()
public:
	// Generated message map functions
	//{{AFX_MSG(CSizeTrackerDialog)
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

protected:
	CWnd*					m_intendedParentCWnd;
	bool					m_bPermanentCWnd;
	CWnd*					m_realParentCWnd;
	HDC						m_transformHDC;
	CRect					m_boundsLimit;
	CPoint					m_mouseClick;	// Screen coordinates of the mouse click which invoked the in-place edit
	CRect					m_initialRect;
	ResizeType				m_resizeType;

	CString	GetStrToDisplay(const CRect& location);
	CPoint	GetAdjustedPosition(const CPoint& pt);

public:
	void	SetParameters(const CRect& initialRect, const CPoint& mouseClick, ResizeType resizeType,
						  CWnd* parentCWnd, bool isPermanentCWnd, HDC transformHDC);
	void	AdjustPositionAndText(const CPoint& point, const CRect& location);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSizeTrackerDialog)
	public:
	//}}AFX_VIRTUAL
};

}

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIZETRACKERDIALOG_H__83F5D328_52B2_4c8b_8239_3BE9FF315196__INCLUDED_)
