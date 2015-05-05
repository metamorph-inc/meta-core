#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#include "resource.h"
// CStatusDialog dialog
#define PBR_RANGE 1000000

class CStatusDialog : public CDialog
{
	DECLARE_DYNAMIC(CStatusDialog)

public:
	CStatusDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CStatusDialog();

// Dialog Data
	enum { IDD = IDD_PROGRESS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	CProgressCtrl m_prgBar;
	CEdit m_status;
	UINT m_maxPrg;
	int processPos;
public:
	virtual BOOL OnInitDialog();
	void SetProgress(const CString &status);
	void OnFinished();
	void SetRange(int range);

private:
	HWND m_gmeWindow;

public:
	afx_msg void OnDestroy();
};

CStatusDialog * GetStatusDlg(CStatusDialog * set_dlg);