#pragma once

#include "resource.h"
#include "RefacorTypeDef.h"

// CCARefactorDialog dialog
class CCARefactorDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CCARefactorDialog)

public:
	CCARefactorDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCARefactorDialog();

// Dialog Data
	enum { IDD = IDD_CACONVERTER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	RefactorType rt;
	afx_msg void OnBnClickedExradio();
	afx_msg void OnBnClickedDcradio();
	afx_msg void OnBnClickedCaRadio();
	virtual BOOL OnInitDialog();
};
