#if !defined(AFX_ABOUTCLASS_H__7EC6CB24_C286_46F0_80D5_6F07EDB1A682__INCLUDED_)
#define AFX_ABOUTCLASS_H__7EC6CB24_C286_46F0_80D5_6F07EDB1A682__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AboutClass.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAboutClass dialog

class CAboutClass : public CDialog
{
// Construction
public:
	CAboutClass(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAboutClass)
	enum { IDD = IDD_ABOUT_DLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutClass)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAboutClass)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ABOUTCLASS_H__7EC6CB24_C286_46F0_80D5_6F07EDB1A682__INCLUDED_)
