#ifndef __RESULTS_DIALOG_H__
#define __RESULTS_DIALOG_H__


/*** Included Header Files ***/
#include "resource.h"
#include "motivate.h"
#include <afxcmn.h>


/************************************************************************************/


class CResultsDialog : public CDialog {
protected:
	Action									*_action;
	BON::Model								_bonAction;
	CEdit									_scriptEdit;
	CEdit									_resultsEdit;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
//	afx_msg void OnPaint();
//	afx_msg HCURSOR OnQueryDragIcon();

	// Event Handlers
//	virtual void OnOK();
	DECLARE_MESSAGE_MAP()

public:
	// Constructors
	CResultsDialog(CWnd* pParent, BON::Model &action);
	CResultsDialog(CWnd* pParent, Action *action);
	enum { IDD = IDD_RESULTS_DIALOG };
};


/************************************************************************************/


#endif //__RESULTS_DIALOG_H__

