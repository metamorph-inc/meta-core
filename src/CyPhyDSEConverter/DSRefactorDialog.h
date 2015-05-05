#pragma once

#include "resource.h"
// CDSRefactorDialog dialog

class CDSRefactorDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CDSRefactorDialog)

public:
	CDSRefactorDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDSRefactorDialog();

// Dialog Data
	enum { IDD = IDD_DSEREFACTOR_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	bool m_genCA;
	afx_msg void OnBnClickedDcRadio();
	afx_msg void OnBnClickedCaRadio();
	virtual BOOL OnInitDialog();
};
