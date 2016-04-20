#pragma once

#include "Resource.h"
#include <afxwin.h>
#include <afxcmn.h>

#include "DesertHelper.h"
// CConstraintAddDialog dialog

class CConstraintAddDialog : public CDialog
{
	DECLARE_DYNAMIC(CConstraintAddDialog)

public:
	CConstraintAddDialog(CWnd* pParent = NULL);   // standard constructor
	CConstraintAddDialog(DesertHelper *deserthelper_ptr, CWnd* pParent = NULL);
	virtual ~CConstraintAddDialog();

// Dialog Data
	enum { IDD = IDD_ADD_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_name;
	CString m_expression;
	CListBox m_funclist;
	int m_cyphyObjId;
	afx_msg void OnBnClickedInsertbtn();
	afx_msg void OnLbnDblclkFunctionlist();
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeExpressionedit();
private:
	CEdit c_expredit;
	CEdit c_nameedit;
	CTreeCtrl m_spacetree;
	DesertHelper *dhelper_ptr;
	bool m_editExpression;
	map<HTREEITEM, int> spaceTreeMap;

	void InsertFuncString();
	void FillDesignSpaceTree();
	void FillDesignSpaceTree(DesertIface::Element &elem, HTREEITEM parent);
public:
	afx_msg void OnEnSetfocusExpressionedit();
	afx_msg void OnEnSetfocusNameedit();
	afx_msg void OnTvnSelchangedSpacetree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
};