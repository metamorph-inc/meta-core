#pragma once

#include "resource.h"

// CMophyMatrixFileDlg dialog

class CMophyMatrixFileDlg : public CDialog
{
	DECLARE_DYNAMIC(CMophyMatrixFileDlg)

public:
	CMophyMatrixFileDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMophyMatrixFileDlg();

// Dialog Data
	enum { IDD = IDD_MM2CYPHY_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOutputbtn();
	afx_msg void OnEnChangeFilepathedit();
	CString m_filepath;
};
