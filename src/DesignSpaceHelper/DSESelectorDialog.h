#pragma once

#include "resource.h"
#include "DesertHelper.h"
#include <afxcmn.h>

// CDSESelectorDialog dialog

class CDSESelectorDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CDSESelectorDialog)

public:
	CDSESelectorDialog(CWnd* pParent = NULL);   // standard constructor
	CDSESelectorDialog(DesertHelper *deserthelper_ptr, CWnd* pParent = NULL);
	virtual ~CDSESelectorDialog();

// Dialog Data
	enum { IDD = IDD_DSESELECTOR_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	
private:
	CTreeCtrl m_dsetree;
	DesertHelper *dhelper_ptr;
	HTREEITEM m_dseTreeRootItem;
	map<HTREEITEM, int> dseTreeMap;
	set<int> selectedElems;

	void FillCfgTree();
	void FillCfgTree(DesertIface::Element &elem, HTREEITEM parent);
public:
	set<int> getSelectedElems();
	virtual void OnOK();
};
