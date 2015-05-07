#ifndef __INTERPRETERWIN_H__
#define __INTERPRETERWIN_H__


/*** Included Header Files ***/
#include "resource.h"
#include "motivate.h"
#include <afxcmn.h>


/************************************************************************************/


class CInterpreterDialog : public CDialog {
protected:
	HICON									_hIcon;
	BON::Project							_project;
	CComboBox								_processCombo;
	CComboBox								_initActionCombo;
	CListCtrl								_actionList;
	CButton									_errorHaltCheck;
	CButton									_saveScriptCheck;
	CButton									_saveOnlyCheck;
	CEdit									_scriptPathEdit;
	CButton									_scriptPathButton;
	CStatic									_scriptPathLabel;
	CImageList								_actionListImages;
	std::map<std::string,BON::Model>		_processes;
	BON::Model								_currentProcess;
	std::map<std::string,BON::Model>		_actions;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	void ResetState(void);

	// Event Handlers
	virtual void OnOK();
	void OnSaveScriptCheck(void);
	void OnScriptPathButton(void);
	void OnProcessComboChange(void);
	void OnInitActionComboChange(void);
	void OnActionItemSelection(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()

public:
	// Constructor
	CInterpreterDialog(CWnd* pParent, BON::Project &project);
	enum { IDD = IDD_MAIN_DIALOG };
};


/************************************************************************************/


#endif //__INTERPRETERWIN_H__

