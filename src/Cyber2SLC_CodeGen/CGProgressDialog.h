#pragma once

#include "resource.h"
#include "afxwin.h"
#include "afxcmn.h"
// CCGProgressDialog dialog

class CCGProgressDialog : public CDialog
{
	DECLARE_DYNAMIC(CCGProgressDialog)

public:
	CCGProgressDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCGProgressDialog();

// Dialog Data
	enum { IDD = IDD_PROGRESS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	DECLARE_MESSAGE_MAP()
	CProgressCtrl m_progress;
	CEdit m_status;
	UINT m_maxPrg;
	int processPos;
public:
	void SetProgress(const CString &status);
	void OnFinished();
	void SetRange(int range);
	bool m_cancel;
	virtual void OnCancel();
};
CCGProgressDialog * GetCGProgressDlg(CCGProgressDialog * set_dlg);