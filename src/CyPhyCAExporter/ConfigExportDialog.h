#pragma once

#include "resource.h"
// CConfigExportDialog dialog

class CConfigExportDialog : public CDialog
{
	DECLARE_DYNAMIC(CConfigExportDialog)

public:
	CConfigExportDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CConfigExportDialog();

// Dialog Data
	enum { IDD = IDD_EXPORT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	bool m_useCurrent;
	bool m_flatten;
	CString m_outputfdr;
	afx_msg void OnBnClickedCurrentradio();
	afx_msg void OnBnClickedNewradio();
	afx_msg void OnBnClickedOutputbtn();
	afx_msg void OnEnChangeOutputedit();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedHierarchical();
	afx_msg void OnBnClickedFlatten();
};
