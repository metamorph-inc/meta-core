// -*-C++-*-
#if !defined(AFX_DESERTUI_H__856CAB61_31A6_11D3_8298_00E098803415__INCLUDED_)
#define AFX_DESERTUI_H__856CAB61_31A6_11D3_8298_00E098803415__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// desertui.h : header file
//

#include "core/coredefs.h"

class CDesertUi;
class CDesertList;
class CDesertTree;

typedef CTypedPtrList<CPtrList, CDesertTree *> CDesertTreeList;

/////////////////////////////////////////////////////////////////////////////
// CDesertUi

class CDesertUi : public CPropertySheet
{
	DECLARE_DYNAMIC(CDesertUi)

// Construction
public:
	CDesertUi(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CDesertUi(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	void UpdateMenu();
	void UpdateContent();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDesertUi)
	public:
	virtual BOOL OnInitDialog();
	virtual INT_PTR DoModal();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDesertUi();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDesertUi)
	afx_msg void OnApplyAll();
	afx_msg void OnApplyChecked();
	afx_msg void OnFileExit();
	afx_msg void OnClose();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnConfigurationsGoback();
	afx_msg void OnConfigurationsGoforward();
	afx_msg void OnApplyImplicit();
	afx_msg void OnHelpAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP();

private:
  CDesertList *constraintWnd;
  CDesertTreeList treeWnds;
  CMenu menu;
  void Apply(bool checked);

public:
  static CImageList *theImageList;
};

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CDesertList dialog

class CDesertList : public CPropertyPage
{
	DECLARE_DYNCREATE(CDesertList)

// Construction
public:
	CDesertList();
	~CDesertList();

// Dialog Data
	//{{AFX_DATA(CDesertList)
	enum { IDD = IDD_LIST_DIALOG };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDesertList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDesertList)
	afx_msg void OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP();

private:
  double dspSize;
  long repSize;
  long clockTime;
  long actualCfgs;

private:
  void FillSizeBox();
  void FillSizeBox1();
  void FillList();
  void FillList(CListCtrl *ctrl);
  void FillList(CListCtrl *ctrl, CDynConstraintSet *set);
  void FillList(CListCtrl *ctrl, CDynConstraint *cons, const TCHAR *category);
  bool Apply(CListCtrl *ctrl, bool checked); // actually apply
  
public:
  void Apply(bool checked);				// apply the checked constraints
  void ApplyImpl();						// apply implicit constraints
  void UpdateContent();
};
/////////////////////////////////////////////////////////////////////////////
// CDesertTree dialog

class CDesertTree : public CPropertyPage
{
	DECLARE_DYNCREATE(CDesertTree)

// Construction
public:
	CDesertTree();
	~CDesertTree();

// Dialog Data
	//{{AFX_DATA(CDesertTree)
	enum { IDD = IDD_TREE_DIALOG };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDesertTree)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDesertTree)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP();

public:
  CDesertTree(CDynSpace *);
  CDesertTree(CDynCustomDomain *);
  void Reset(CDynSpace *);
  void Reset(CDynCustomDomain *);
  TCHAR *GetTitle();

private:
  enum { coreSpace, coreDomain } coreType;
  union {
    CDynSpace *space;
    CDynCustomDomain *domain;
  } coreObj;
  TCHAR *title;
  void FillTree();
  void FillBox();
  void FillTree(CTreeCtrl *tree, CDynElementList& elements, HTREEITEM item=TVI_ROOT);
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DESERTUI_H__856CAB61_31A6_11D3_8298_00E098803415__INCLUDED_)
