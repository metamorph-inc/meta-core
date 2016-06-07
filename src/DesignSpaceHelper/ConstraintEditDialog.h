#pragma once

#include "Resource.h"
#include <afxwin.h>
#include "DesertHelper.h"
// CConstraintEditDialog dialog

class CConstraintEditDialog : public CDialog
{
	DECLARE_DYNAMIC(CConstraintEditDialog)

public:
	CConstraintEditDialog(CWnd* pParent = NULL);   // standard constructor
	CConstraintEditDialog(CString c_name, CString c_expr, CWnd* pParent = NULL); 
	CConstraintEditDialog(CString c_name, CString c_expr, DesertHelper *deserthelper_ptr, CWnd* pParent = NULL);
	virtual ~CConstraintEditDialog();

// Dialog Data
	enum { IDD = IDD_EDIT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_name;
	CString m_expression;
	CListBox m_funclist;
	afx_msg void OnBnClickedInsertbtn();
	afx_msg void OnLbnDblclkFunctionlist();
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeExpressionedit();
//	afx_msg void OnLbnSelchangeFunctionlist();
private:
	CEdit c_expredit;
	CEdit c_nameedit;
	void insertFuncString();
	bool m_editExpression;
	DesertHelper *dhelper_ptr;
public:
	afx_msg void OnEnSetfocusExpressionedit();
	afx_msg void OnEnSetfocusNameedit();
	afx_msg void OnBnClickedOk();
};
