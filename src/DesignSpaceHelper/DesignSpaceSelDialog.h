#pragma once

#include "resource.h"
#include "CyPhyML.h"
#include <afxcmn.h>
// CDesignSpaceSelDialog dialog

class CDesignSpaceSelDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CDesignSpaceSelDialog)

public:
	CDesignSpaceSelDialog(CWnd* pParent = NULL);   // standard constructor
//	CDesignSpaceSelDialog(const set<CyPhyML::DesignSpace> &dss, CWnd* pParent = NULL);
	virtual ~CDesignSpaceSelDialog();
//	void GetSelectedDesignSpace(CyPhyML::DesignSpace &ds);

// Dialog Data
	enum { IDD = IDD_DSSEL_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	int sel;
	map<int, CyPhyML::DesignSpace> allDSs;
public:
	CListCtrl m_dslist;
	virtual BOOL OnInitDialog();
	afx_msg void OnLvnItemchangedDslist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangingDslist(NMHDR *pNMHDR, LRESULT *pResult);
};
