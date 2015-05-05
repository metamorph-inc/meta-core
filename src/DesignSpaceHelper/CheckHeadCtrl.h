#if !defined(AFX_CHECKHEADCTRL_H__044A05D0_60CE_40D9_A04E_6AB0BC88C1E9__INCLUDED_)
#define AFX_CHECKHEADCTRL_H__044A05D0_60CE_40D9_A04E_6AB0BC88C1E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CheckHeadCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCheckHeadCtrl window

class CCheckHeadCtrl : public CHeaderCtrl
{
// Construction
public:
	CCheckHeadCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCheckHeadCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCheckHeadCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCheckHeadCtrl)
	afx_msg void OnItemClicked(NMHDR* pNMHDR, LRESULT* pResult);
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHECKHEADCTRL_H__044A05D0_60CE_40D9_A04E_6AB0BC88C1E9__INCLUDED_)
