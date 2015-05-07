// desertui.cpp : implementation file
//



#include "stdafx.h"



#include "desert.h"
#include "desertui.h"
#include "CSetErrDialog.h"	
#include "core/manager.h"
#include "core/dynamic.h"
#include "AboutClass.h"
#include "core/digraph.h"




#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
#define WIDE2(x) L##x
#define WIDE1(x) WIDE2(x)
static TCHAR WTHIS_FILE[] = WIDE1(__FILE__);
static char THIS_FILE[] = __FILE__;
#endif

#define DO_MY_MODAL

/////////////////////////////////////////////////////////////////////////////
// CDesertUi
CImageList *CDesertUi::theImageList = NULL;

IMPLEMENT_DYNAMIC(CDesertUi, CPropertySheet)

CDesertUi::CDesertUi(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
  ASSERT_EX(false, _T("CDesertUi::CDesertUi"), _T("constructor not implemented"));
}

CDesertUi::CDesertUi(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
  ASSERT_EX(CManager::theInstance, _T("CDesertUi::CDesertUi"), _T("CManager::theInstance IS null"));

  // create the image list
  theImageList = new CImageList;
  theImageList->Create( IDB_IMAGELIST, 16, 1, 0 );

  // create page window for constraints
  constraintWnd = new CDesertList();
  ASSERT_EX(constraintWnd, _T("CDesertUi::CDesertUi"), _T("new failed"));
  AddPage(constraintWnd);

  // create page windows for spaces
  {
    CDynSpaceList& spaces = CManager::theInstance->GetSpaces();
    POSITION pos = spaces.GetHeadPosition();
    while(pos)
    {
      CDesertTree *wnd = new CDesertTree(spaces.GetNext(pos));
      ASSERT_EX(wnd, _T("CDesertUi::CDesertUi"), _T("new failed"));
      treeWnds.AddTail(wnd);
      AddPage(wnd);
    }
  }
  
  // create page windows for domains
  {
    CDynDomainList& domains = CManager::theInstance->GetDomains();
    POSITION pos = domains.GetHeadPosition();
    while(pos)
    {
      CDynDomain *domain = domains.GetNext(pos);
      if (domain->GetType() != typeDynCustomDomain) continue;
      CDesertTree *wnd = new CDesertTree((CDynCustomDomain *)domain);
      ASSERT_EX(wnd, _T("CDesertUi::CDesertUi"), _T("new failed"));
      treeWnds.AddTail(wnd);
      AddPage(wnd);
    }
  }
  menu.LoadMenu(IDR_DESERT_MENU);
}

CDesertUi::~CDesertUi()
{
  delete constraintWnd;
  POSITION pos = treeWnds.GetHeadPosition();
  while(pos) delete treeWnds.GetNext(pos);
  delete theImageList;
}


BEGIN_MESSAGE_MAP(CDesertUi, CPropertySheet)
	//{{AFX_MSG_MAP(CDesertUi)
	ON_COMMAND(ID_APPLY_ALL, OnApplyAll)
	ON_COMMAND(ID_APPLY_CHECKED, OnApplyChecked)
	ON_COMMAND(ID_FILE_EXIT, OnFileExit)
	ON_WM_CLOSE()
	ON_WM_CREATE()
	ON_COMMAND(ID_CONFIGURATIONS_GOBACK, OnConfigurationsGoback)
	ON_COMMAND(ID_CONFIGURATIONS_GOFORWARD, OnConfigurationsGoforward)
	ON_COMMAND(ID_APPLY_IMPLICIT, OnApplyImplicit)
	ON_COMMAND(ID_HELP_ABOUT, OnHelpAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDesertUi message handlers

void CDesertUi::OnApplyAll() 
{
  Apply(false);
}

void CDesertUi::OnApplyChecked() 
{
  Apply(true);
}

void CDesertUi::OnFileExit() 
{
#ifdef DO_MY_MODAL
  EndModalLoop(IDCANCEL);
#else
  EndDialog(IDCANCEL);
#endif
}

static TCHAR tabText[128];

BOOL CDesertUi::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	CDiGraph dummy;
	dummy.linker_force();

  // initialize the titles of the pages
  CTabCtrl *tabs = GetTabControl();
  ASSERT_EX( tabs, _T("CDesertUi::OnInitDialog"), _T("tab control in property sheet is null") );
  int index = 1;
  POSITION pos = treeWnds.GetHeadPosition();
  while(pos)
  {
    CDesertTree *wnd = treeWnds.GetNext(pos);
    TCHAR *n = wnd->GetTitle();
    TC_ITEM tab; tab.mask = TCIF_TEXT; tab.pszText = tabText; tab.cchTextMax = 128;
    if (tabs->GetItem(index, &tab)) { tab.mask = TCIF_TEXT; tab.pszText = n; tabs->SetItem(index, &tab); }
    index++;
  }
	
  UpdateMenu();
  
  return bResult;
}

void CDesertUi::UpdateMenu()
{
	 //enabling/disabling navigation menu
	ASSERT_EX(CManager::theInstance, _T("CDesertUi::Apply"), _T("Manager is NULL"));
  
	CMenu * pui_menu=GetMenu()->GetSubMenu(2);
	pui_menu->EnableMenuItem(ID_CONFIGURATIONS_GOBACK, CManager::theInstance->IsBackNavigable() ? MF_ENABLED:MF_GRAYED   );
	pui_menu->EnableMenuItem(ID_CONFIGURATIONS_GOFORWARD, CManager::theInstance->IsForwardNavigable() ? MF_ENABLED:MF_GRAYED   );

	pui_menu=GetMenu()->GetSubMenu(1);
	pui_menu->EnableMenuItem(ID_APPLY_IMPLICIT, CManager::theInstance->HasImplicitConstraints() ? MF_ENABLED:MF_GRAYED   );

};


void CDesertUi::UpdateContent()
{
	ASSERT_EX(CManager::theInstance, _T("CDesertUi::UpdateContent"), _T("Invalid manager!"));
  // reset space pointer in tree windows after applying constraints
  POSITION pos, treeWndPos = treeWnds.GetHeadPosition();

  CDynSpaceList& spaces = CManager::theInstance->GetSpaces();
  pos = spaces.GetHeadPosition();
  while(pos) treeWnds.GetNext(treeWndPos)->Reset(spaces.GetNext(pos));

  CDynDomainList& domains = CManager::theInstance->GetDomains();
  pos = domains.GetHeadPosition();
  while(pos)
  {
    CDynDomain *domain = domains.GetNext(pos);
    if (domain->GetType() != typeDynCustomDomain) continue;
    treeWnds.GetNext(treeWndPos)->Reset((CDynCustomDomain *)domain);
  }
};
void
CDesertUi::
Apply(bool checked)
{
  constraintWnd->Apply(checked);

 UpdateMenu();
 UpdateContent();

/*

  // reset space pointer in tree windows after applying constraints
  POSITION pos, treeWndPos = treeWnds.GetHeadPosition();

  CDynSpaceList& spaces = CManager::theInstance->GetSpaces();
  pos = spaces.GetHeadPosition();
  while(pos) treeWnds.GetNext(treeWndPos)->Reset(spaces.GetNext(pos));

  CDynDomainList& domains = CManager::theInstance->GetDomains();
  pos = domains.GetHeadPosition();
  while(pos)
  {
    CDynDomain *domain = domains.GetNext(pos);
    if (domain->GetType() != typeDynCustomDomain) continue;
    treeWnds.GetNext(treeWndPos)->Reset((CDynCustomDomain *)domain);
  }
*/

}

INT_PTR CDesertUi::DoModal() 
{
#ifdef DO_MY_MODAL
  CWnd *parent = AfxGetMainWnd();
  // ASSERT_EX(parent, _T("CDesertUi::DoModal"), _T("parent window is null"));
  if (parent) parent->EnableWindow(FALSE);
  Create(parent);
  int res = RunModalLoop(MLF_NOIDLEMSG);
  if (parent) parent->EnableWindow(TRUE);
  DestroyWindow();
  return res;
#else
  return CPropertySheet::DoModal();
#endif
}

void CDesertUi::OnClose() 
{
#ifdef DO_MY_MODAL
  EndModalLoop(IDCANCEL);
#else
  CPropertySheet::OnClose();
#endif
}

int CDesertUi::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
  if (CPropertySheet::OnCreate(lpCreateStruct) == -1)
    return -1;

  SetMenu(&menu);
  menu.Detach();

  CRect rect;
  GetWindowRect(rect);
  rect.InflateRect(0, 0, 0, 20);
  MoveWindow(rect);
	
  return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CDesertList property page
static TCHAR* columnHeadings[] = {
  _T("Constraint"),
  _T("Category"),
  _T("Context"),
  _T("Expression"),
  0
};

#define IDX_CHECKBOX_NOCHECK 	7       // this numbers depend on the imagelist - tbd skn
#define IDX_CHECKBOX_CHECK 	8       // this value depends on the imagelist
#define WTIMES	1.6

IMPLEMENT_DYNCREATE(CDesertList, CPropertyPage)

CDesertList::CDesertList() : CPropertyPage(CDesertList::IDD)
{
	//{{AFX_DATA_INIT(CDesertList)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CDesertList::~CDesertList()
{
}

void CDesertList::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDesertList)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDesertList, CPropertyPage)
	//{{AFX_MSG_MAP(CDesertList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, OnItemchangedList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDesertList message handlers

BOOL CDesertList::OnInitDialog() 
{
  CPropertyPage::OnInitDialog();
  clockTime = 0;
  FillList();
  FillSizeBox();
  return TRUE;  // return TRUE unless you set the focus to a control
}

void
CDesertList::
FillSizeBox()
{
	CEdit *box = (CEdit *)GetDlgItem(IDC_DSP_REP_SZ);
	ASSERT_EX( box, _T("CDesertDialog::FillSizeBox"), _T("the ctrl is null"));
	ASSERT_EX( CManager::theInstance, _T("CDesertList::FillSizeBox"), _T("manager not instantiated") );
	long dummy = 0;                       // we want the clocktime from the previous apply
	
	//make sure we invoke here the GetSizeInfo only when it's needed
	//(NOT after Apply()...)
	try{
		actualCfgs = CManager::theInstance->CalcRealNoOfConfigurations();
		if (CManager::theInstance->ui_refresh_needed)
		{		
  			CManager::theInstance->GetSizeInfo(dspSize, repSize, dummy);	
			CManager::theInstance->ui_refresh_needed = false;
		}
	}catch(CDesertException *e)
	{
		return;
	}
	CString str;
	if(actualCfgs == -2) {
		str.Format(_T("Design Space (Encoded): %3.2e        EncodingLength: %6d      Time (ms): %3d     No. of Cfgs: Design Space Too Large"), dspSize, repSize, clockTime);
	} else {
		str.Format(_T("Design Space (Encoded): %3.2e        EncodingLength: %6d      Time (ms): %3d     No. of Cfgs: %7d"), dspSize, repSize, clockTime,actualCfgs);
	}
	box->SetWindowText(str);
}


void
CDesertList::
FillSizeBox1()
{
  CEdit *box = (CEdit *)GetDlgItem(IDC_DSP_REP_SZ);
  ASSERT_EX( box, _T("CDesertDialog::FillSizeBox1"), _T("the ctrl is null"));
  CString str; str.Format(_T("Design Space: %3.1e        Representation: %6d      Time (ms): %3d"), dspSize, repSize, clockTime);
  box->SetWindowText(str);
}


void
CDesertList::
FillList()
{
  CListCtrl *ctrl = (CListCtrl *)GetDlgItem(IDC_LIST);
  ASSERT_EX( ctrl, _T("CDesertDialog::OnInitDialog"), _T("the ctrl is null"));
  // set the image list
  if (CDesertUi::theImageList) ctrl->SetImageList(CDesertUi::theImageList, LVSIL_SMALL);
  // set the column headings
  for (int i=0; columnHeadings[i]; i++)
  {
    int ret = ctrl->InsertColumn(
      i,                                // column index
      columnHeadings[i],                  // column heading
      LVCFMT_LEFT,                      // column text format
      (int)(WTIMES*ctrl->GetStringWidth(columnHeadings[i])), // column width
      i                                 // the subitem asssociated with the column
      );
    ASSERT_EX( ret>=0, _T("CDesertList::FillList"), _T("InsertColumn in list failed") );
  }
  // fill the list
  FillList(ctrl);
}

void
CDesertList::
FillList(CListCtrl *list)
{
  list->DeleteAllItems();
  ASSERT_EX( CManager::theInstance, _T("CDesertList::FillList"), _T("manager not instantiated") );
  CDynConstraintSetList& consets = CManager::theInstance->GetConstraintSets();
  POSITION pos = consets.GetHeadPosition();
  while(pos)
  {
    CDynConstraintSet* set = consets.GetNext(pos);
    FillList(list, set);
  }
}

void
CDesertList::
FillList(CListCtrl *list, CDynConstraintSet *set)
{
  CDynConstraintList& constraints = set->GetConstraints();
  POSITION pos = constraints.GetHeadPosition();
  while(pos)
  {
    CDynConstraint* cons = constraints.GetNext(pos);
	CConstraint * static_cons =(CConstraint *) cons->GetCore();

	//fill only not-implicit constraints
	if (!static_cons->IsImplicit() && static_cons->IsValid())
		FillList(list, cons, (const TCHAR *)*set);
  }
}

void
CDesertList::
FillList(CListCtrl *list, CDynConstraint *cons, const TCHAR *category)
{
  int ret, req, is;
  LV_ITEM item;
  item.state = item.stateMask = 0;
  item.iItem = item.iSubItem = 0;

  item.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
  item.iImage = IDX_CHECKBOX_NOCHECK;
  item.lParam = (LPARAM)(long)cons;

  // first column is the name of the constraint
  item.pszText = (LPTSTR)(const TCHAR *)*cons;
  ret = list->InsertItem(&item);
  ASSERT_EX( ret >= 0, _T("CDesertList::FillList"), _T("InsertItem failed") );

  // fix column width
  req = (int)(WTIMES*list->GetStringWidth(item.pszText));
  is = list->GetColumnWidth(0);
  if ( is < req ) list->SetColumnWidth(0, req);

  item.iItem = ret;
  item.mask = LVIF_TEXT;
  item.iImage = 0;
  item.lParam = 0;

  // second column is the category of the set
  item.iSubItem = 1;
  item.pszText = (LPTSTR)category;
  ret = list->SetItem(&item);
  ASSERT_EX( ret >= 0, _T("CDesertList::FillList"), _T("SetItem failed") );
  
  // fix column width
  req = (int)(WTIMES*list->GetStringWidth(item.pszText));
  is = list->GetColumnWidth(1);
  if ( is < req ) list->SetColumnWidth(1, req);

  CDynElement *ctx = cons->GetContext();
  ASSERT_EX( ctx, _T("CDesertList::FillList"), _T("constraint context is NULL") );

  // third column is the context of the constraint
  item.iSubItem = 2;
  item.pszText = (LPTSTR)(const TCHAR *)*ctx;
  ret = list->SetItem(&item);
  ASSERT_EX( ret >= 0, _T("CDesertList::FillList"), _T("SetItem failed") );

  // fix column width
  req = (int)(WTIMES*list->GetStringWidth(item.pszText));
  is = list->GetColumnWidth(2);
  if ( is < req ) list->SetColumnWidth(2, req);

  CString& expr = cons->GetText();

  // forth column is the expression of the constraint
  item.iSubItem = 3;
  item.pszText = (LPTSTR)(const TCHAR *)expr;
  ret = list->SetItem(&item);
  ASSERT_EX( ret >= 0, _T("CDesertList::FillList"), _T("SetItem failed") );

  // fix column width
  req = (int)(WTIMES*list->GetStringWidth(item.pszText));
  is = list->GetColumnWidth(3);
  if ( is < req ) list->SetColumnWidth(3, req);
}

void CDesertList::OnItemchangedList(NMHDR* hdr, LRESULT* result) 
{
  NM_LISTVIEW *lv = (NM_LISTVIEW *)hdr;
  UINT flag = lv->uChanged;
  if (flag & LVIF_STATE)
  {
    UINT newState = lv->uNewState;
    if (newState & LVIS_SELECTED)       // item selected 
	{
	  // flip image
	  CListCtrl *ctrl = (CListCtrl *)GetDlgItem(IDC_LIST);
	  LV_ITEM item;
	  memset(&item, 0, sizeof(LV_ITEM));
	  item.iItem = lv->iItem;
	  item.mask = LVIF_IMAGE;
	  ctrl->GetItem( &item );
	  if (item.iImage == IDX_CHECKBOX_CHECK) item.iImage = IDX_CHECKBOX_NOCHECK;
	  else item.iImage = IDX_CHECKBOX_CHECK;
	  ctrl->SetItem( &item );
	}
  }
  *result = 0;
}


void CDesertList::
ApplyImpl()
{
	ASSERT_EX( CManager::theInstance, _T("CDesertList::Apply"), _T("manager not instantiated") );
	
	CManager::theInstance->GenerateNextHierarchy();
	//create a new constraintset
	CDynConstraintSet *set = new CDynConstraintSet(0);
	set->RemoveAll();

	//fill in 

	CDynConstraintSetList & setlist = CManager::theInstance->GetConstraintSets();
	POSITION sl_pos = setlist.GetHeadPosition();
	while (sl_pos)
	{
		CDynConstraintSet * setlist_i = setlist.GetNext(sl_pos);
		CDynConstraintList& list = setlist_i->GetConstraints();
		POSITION pos1 = list.GetHeadPosition();
		while(pos1)
		{
			
			CDynConstraint *cur = list.GetNext(pos1);
			CConstraint * static_cur = (CConstraint *)cur->GetCore();

			//apply only implicit constraints here
			if (static_cur->IsImplicit())
			{
				set->InsertConstraint(cur);
				cur->SetApplied();
			}
		}
	}

	//actually apply
	try{
		CManager::theInstance->GetSizeInfo(dspSize, repSize, clockTime, set);
		CManager::theInstance->GenerateNextHierarchy();
		FillList();
		FillSizeBox();
	}
	catch(CDesertException *e)
	{
		e->ReportError();
		set->RemoveAll();
		delete set;
		return;
	}

	set->RemoveAll();
	delete set;
};

void
CDesertList::
Apply(bool checked)
{
  // make sure to update the tree pages once you apply constraints
  ASSERT_EX( CManager::theInstance, _T("CDesertList::Apply"), _T("manager not instantiated") );

  CListCtrl *ctrl = (CListCtrl *)GetDlgItem(IDC_LIST);
  ASSERT_EX( ctrl, _T("CDesertList::Apply"), _T("the ctrl is null"));

  // actually apply
  if(Apply(ctrl, checked))
  {
	  FillSizeBox();
  }
	FillList(ctrl);
}


void CDesertList::UpdateContent()
{
  ASSERT_EX( CManager::theInstance, _T("CDesertList::Apply"), _T("manager not instantiated") );

  CListCtrl *ctrl = (CListCtrl *)GetDlgItem(IDC_LIST);
  ASSERT_EX( ctrl, _T("CDesertList::UpdateContent"), _T("the ctrl is null"));
  // refill ctrl
  FillList(ctrl);
  FillSizeBox();

}
bool
CDesertList::
Apply(CListCtrl *ctrl, bool checked)
{
  // create a constraint set & fill it with checked constraints from the list ctrl
  // invoke prune method of core manager with this set as an argument
  
	CManager::theInstance->GenerateNextHierarchy();
 
  CDynConstraintSet *set = new CDynConstraintSet(0);
  set->RemoveAll();

  int count = ctrl->GetItemCount();
  LV_ITEM item;
  memset(&item, 0, sizeof(LV_ITEM));
  item.mask = checked ? LVIF_PARAM | LVIF_IMAGE : LVIF_PARAM;

 CDynConstraintSetList & setlist = CManager::theInstance->GetConstraintSets();
 CStringList cNames; 
 for (int i=0; i<count; i++)
  {
    item.iItem = i;
    if (!ctrl->GetItem(&item)) continue;
    CDynConstraint *cons = (CDynConstraint *)item.lParam;
    if (checked && item.iImage != IDX_CHECKBOX_CHECK) continue;
 //   cons->SetApplied();
 //   set->InsertConstraint(cons);
	const CString& nm  = cons->GetName();
	cNames.AddTail(nm);
  }

 if(!cNames.IsEmpty())
 {
	POSITION sl_pos = setlist.GetHeadPosition();
	while (sl_pos)
	{
		CDynConstraintSet * setlist_i = setlist.GetNext(sl_pos);
		CDynConstraintList& list = setlist_i->GetConstraints();
		POSITION pos1 = list.GetHeadPosition();
		while(pos1)
		{
			CDynConstraint *cur = list.GetNext(pos1);
			const CString& nm  = cur->GetName();

			if (cNames.Find(nm))
			{
				cur->SetApplied();
				set->InsertConstraint(cur);
			}
		}
	}
 }
  // prune & generate next hierarchy
  try{
  CManager::theInstance->GetSizeInfo(dspSize, repSize, clockTime, set);
  CManager::theInstance->GenerateNextHierarchy();
  }
  catch(CDesertException *e)
{
		e->ReportError();
	set->RemoveAll();
	delete set;
	return false;
  }
  set->RemoveAll();
  delete set;
  return true;
}

/////////////////////////////////////////////////////////////////////////////
// CDesertTree property page
static int imageIndex[] = {             // this values depend on the image list
  5,
  3,
  4
};

IMPLEMENT_DYNCREATE(CDesertTree, CPropertyPage)

CDesertTree::CDesertTree() : CPropertyPage(CDesertTree::IDD)
{
  ASSERT_EX(false, _T("CDesertTree::CDesertTree"), _T("this constructor not implemented"));
	//{{AFX_DATA_INIT(CDesertTree)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CDesertTree::CDesertTree(CDynSpace *space) : CPropertyPage(CDesertTree::IDD)
{
  coreType = coreSpace;
  coreObj.space = space;
  title = NULL;
}

CDesertTree::CDesertTree(CDynCustomDomain *domain) : CPropertyPage(CDesertTree::IDD)
{
  coreType = coreDomain;
  coreObj.domain = domain;
  title = NULL;
}

CDesertTree::~CDesertTree()
{
  if (title) delete title;
}

void CDesertTree::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDesertTree)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDesertTree, CPropertyPage)
	//{{AFX_MSG_MAP(CDesertTree)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDesertTree message handlers

BOOL CDesertTree::OnInitDialog() 
{
  CPropertyPage::OnInitDialog();
  CTreeCtrl *tree = (CTreeCtrl *)GetDlgItem(IDC_TREE);
  ASSERT_EX( tree, _T("CDesertTree::OnInitDialog"), _T("tree is NULL"));
  // set the imagelist
  if (CDesertUi::theImageList) tree->SetImageList(CDesertUi::theImageList, LVSIL_SMALL);
	
  return TRUE;  // return TRUE unless you set the focus to a control
}

BOOL CDesertTree::OnSetActive() 
{
  FillTree();
  FillBox();
  return CPropertyPage::OnSetActive();
}

void
CDesertTree::
FillBox()
{
  CEdit *box = (CEdit *)GetDlgItem(IDC_SIZE_BOX);
  ASSERT_EX( box, _T("CDesertTree::FillBox"), _T("box is NULL"));
  CString str;
  switch(coreType)
  {
  case coreSpace:
    ASSERT_EX(coreObj.space, _T("CDesertTree::FillBox"), _T("space is NULL"));
    str.Format(_T("Size of the space = %2.2e"), coreObj.space->ComputeSize());
    break;
      
  case coreDomain:
    ASSERT_EX(coreObj.domain, _T("CDesertTree::FillBox"), _T("domain is NULL"));
    str.Format(_T("Size of the space = %3d"), coreObj.domain->GetSize());
    break;
  }
  box->SetWindowText(str);
}

void
CDesertTree::
FillTree()
{
  CTreeCtrl *tree = (CTreeCtrl *)GetDlgItem(IDC_TREE);
  ASSERT_EX( tree, _T("CDesertTree::OnInitDialog"), _T("tree is NULL"));
  tree->DeleteAllItems();

  switch(coreType)
  {
  case coreSpace:
    ASSERT_EX(coreObj.space, _T("CDesertTree::FillTree"), _T("space is NULL"));
    {
      CDynElementList& elements = coreObj.space->GetRootElements();
      FillTree(tree, elements);
    }
    break;
      
  case coreDomain:
    ASSERT_EX(coreObj.domain, _T("CDesertTree::FillTree"), _T("domain is NULL"));
    {
    //  CDynElementList& elements = coreObj.domain->GetElements();
	  CDynElementList& elements = coreObj.domain->GetRootElements();
      FillTree(tree, elements);
    }
    break;
  }
}
//this function's last param has the def. value TVI_ROOT

void
CDesertTree::
FillTree(CTreeCtrl *tree, CDynElementList& elements, HTREEITEM item)
{
  POSITION pos = elements.GetHeadPosition();
  while(pos)
  {
    CDynElement *element = elements.GetNext(pos);
    int image = imageIndex[((CDecomposition)*element)];
    CDynElementList& children = element->GetChildren();
    HTREEITEM parent = tree->InsertItem(
      TVIF_TEXT | TVIF_IMAGE | TVIF_STATE,
      (LPCTSTR)*element,
      image, image,
      TVIS_EXPANDED, TVIS_EXPANDED,
      0,
      item,
      TVI_LAST
      );
    FillTree(tree, children, parent);
  }
}

void
CDesertTree::
Reset(CDynSpace *space)
{
  ASSERT_EX(coreType == coreSpace, _T("CDesertTree::Reset"), _T("can not change type midway"));
  coreObj.space = space;
  // if the page is active update it
}

void
CDesertTree::
Reset(CDynCustomDomain *domain)
{
  ASSERT_EX(coreType == coreDomain, _T("CDesertTree::Reset"), _T("can not change type midway"));
  coreObj.domain = domain;
  // if the page is active update it
}

TCHAR *
CDesertTree::
GetTitle()
{
  if (title) return title;
  const TCHAR *ret = 0;
  switch(coreType)
  {
  case coreSpace:
    ASSERT_EX(coreObj.space, _T("CDesertTree::FillTree"), _T("space is NULL"));
    ret = (const TCHAR *)*(coreObj.space);
    break;
      
  case coreDomain:
    ASSERT_EX(coreObj.domain, _T("CDesertTree::FillTree"), _T("domain is NULL"));
    ret = (const TCHAR *)*(coreObj.domain);
    break;
  }
  return ret ? title = _tcsdup(ret) : NULL;
}


void CDesertUi::OnConfigurationsGoback() 
{
	// TODO: Add your command handler code here
	
	ASSERT_EX(CManager::theInstance,_T("CDesertUi::OnConfigurationsGoBack"), _T("Manager is not present"));
	ASSERT_EX(CManager::theInstance->IsBackNavigable(),_T("CDesertUi::OnConfigurationsGoBack"),
		_T("Manager is not backnavigable!"));
	CManager::theInstance->NavigateBack();
	constraintWnd->UpdateContent();
	UpdateMenu();
	UpdateContent();
}

void CDesertUi::OnConfigurationsGoforward() 
{
	// TODO: Add your command handler code here
	ASSERT_EX(CManager::theInstance,_T("CDesertUi::OnConfigurationsGoForward"), _T("Manager is not present"));
	ASSERT_EX(CManager::theInstance->IsForwardNavigable(),_T("CDesertUi::OnConfigurationsGoForward"),
		_T("Manager is not forward!"));
	CManager::theInstance->NavigateForward();
	constraintWnd->UpdateContent();
	UpdateMenu();
	UpdateContent();
}

void CDesertUi::OnApplyImplicit() 
{
	//apply implicit constraints
	constraintWnd->ApplyImpl();
	UpdateMenu();
	UpdateContent();
}

void CDesertUi::OnHelpAbout() 
{
	// TODO: Add your command handler code here
	CAboutClass cab;
	cab.DoModal();
}
