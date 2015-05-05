#pragma once

#include "resource.h"

// CMophyMatrixFileDlg dialog

class CCyPhy2MorphMatrixFileDlg : public CDialog
{
	DECLARE_DYNAMIC(CCyPhy2MorphMatrixFileDlg)

public:
	CCyPhy2MorphMatrixFileDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCyPhy2MorphMatrixFileDlg();

// Dialog Data
	enum { IDD = IDD_CYPHY2MM_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOutputbtn();
	afx_msg void OnEnChangeFilepathedit();
	CString m_filepath;
};
