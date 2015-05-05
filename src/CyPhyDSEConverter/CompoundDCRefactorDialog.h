#pragma once

#include "resource.h"
#include "RefacorTypeDef.h"
// CCompoundDCRefactorDialog dialog

class CCompoundDCRefactorDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CCompoundDCRefactorDialog)

public:
	CCompoundDCRefactorDialog(CWnd* pParent = NULL);   // standard constructor
	CCompoundDCRefactorDialog(bool disableCA, CWnd* pParent = NULL);
	virtual ~CCompoundDCRefactorDialog();

// Dialog Data
	enum { IDD = IDD_COMPOUNDDCCONVVERTER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedCaRadio();
	afx_msg void OnBnClickedDcradio();
	afx_msg void OnBnClickedExradio();
public:
	RefactorType rt;
protected:
	virtual BOOL OnInitDialog();
	bool _disableCA;
};
