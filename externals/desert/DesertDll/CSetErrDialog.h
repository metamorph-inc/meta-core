#if !defined(AFX_CSETERRDIALOG_H__CCBA9C9B_BF7C_4D76_BFBD_69E383B4112B__INCLUDED_)
#define AFX_CSETERRDIALOG_H__CCBA9C9B_BF7C_4D76_BFBD_69E383B4112B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CSetErrDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCSetErrDialog dialog

class CCSetErrDialog : public CDialog
{
	bool alive;
// Construction
public:

	CCSetErrDialog(CWnd* pParent = NULL);   // standard constructor
	
	void AddError(const TCHAR * CSet, const TCHAR * c, const TCHAR * context, const TCHAR * err_str, const TCHAR * what);
	bool IsAlive()	{return alive;};
// Dialog Data
	//{{AFX_DATA(CCSetErrDialog)
	enum { IDD = IDD_CERRD };
	CString	m_strErrCts;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCSetErrDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCSetErrDialog)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	int errId;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CSETERRDIALOG_H__CCBA9C9B_BF7C_4D76_BFBD_69E383B4112B__INCLUDED_)
