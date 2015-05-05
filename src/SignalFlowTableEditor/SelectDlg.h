#if !defined(AFX_SELECTDLG_H__00987B03_D52B_43D3_8ABE_C957EE9DBC0D__INCLUDED_)
#define AFX_SELECTDLG_H__00987B03_D52B_43D3_8ABE_C957EE9DBC0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectDlg.h : header file
//

#include "Resource.h"
#include "GridDlg.h"
#include "ComHelp.h"
#include "GMECOM.h"

/////////////////////////////////////////////////////////////////////////////
// CSelectDlg dialog

class CSelectDlg : public CDialog
{
// Construction
public:
	CSelectDlg(CWnd* pParent = NULL);   // standard constructor

	void SetProject(IMgaProject *proj) {m_Project = proj;}


// Dialog Data
	//{{AFX_DATA(CSelectDlg)
	enum { IDD = IDD_SELECTDLG };
	CButton	m_btnCon;
	CButton	m_btnSet;
	CButton	m_btnRef;
	CButton	m_btnModel;
	CButton	m_btnAtom;
	CListBox	m_lstKind;
	BOOL	m_chkAllKinds;
	BOOL	m_chkAllTypes;
	BOOL	m_chkAtom;
	BOOL	m_chkModel;
	BOOL	m_chkRef;
	BOOL	m_chkSet;
	BOOL	m_chkCon;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	CComPtr<IMgaProject> m_Project;
	CComPtr<IMgaMetaFolder> m_rootMetaFolder;

	void GetMetaObjectNames(IMgaMetaBase *metaBase);


	// Generated message map functions
	//{{AFX_MSG(CSelectDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCheckAllKinds();
	afx_msg void OnChkAllTypes();
	afx_msg void OnChkAtom();
	afx_msg void OnChkCon();
	afx_msg void OnChkModel();
	afx_msg void OnChkRef();
	afx_msg void OnChkSet();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTDLG_H__00987B03_D52B_43D3_8ABE_C957EE9DBC0D__INCLUDED_)
