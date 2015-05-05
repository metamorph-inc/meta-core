#pragma once

#include "resource.h"
#include <string>
#include "afxcmn.h"
#include "afxwin.h"
#include "StdAfx.h"

// MainDialog dialog

class MainDialog : public CDialog
{
	DECLARE_DYNAMIC(MainDialog)

public:
	MainDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~MainDialog();

// Dialog Data
	enum { IDD = IDD_MAIN_DLG };

	CString m_metricsFile;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOkButton();
	afx_msg void OnBnClickedCancelButton();
	afx_msg void OnBnClickedFileButton();

public:
	std::string GetMetricsFile() {return (std::string)m_metricsFile;}
};
