// ConstraintMainDialog.cpp : implementation file
//

#include "StdAfx.h"
#include "ConstraintMainDialog.h"
#include "ConstraintEditDialog.h"
#include "ConstraintAddDialog.h"
#include "DesertConfigDialog.h"
#include "Uml.h"
#include "UmlExt.h"
#include "CyPhyML.h"

#include "DSESelectorDialog.h"

#include "desertdll.h"

#define WTIMES	1.4
// CConstraintMainDialog dialog

IMPLEMENT_DYNAMIC(CConstraintMainDialog, CDialog)

CConstraintMainDialog::CConstraintMainDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CConstraintMainDialog::IDD, pParent), m_gmeWindow(NULL)
{
	m_pToolTip = NULL;
	refresh_needed = true;
	applyAll = false;
	initSize = { 0,0,0,0 };
}

CConstraintMainDialog::CConstraintMainDialog(DesertHelper *deserthelper_ptr, CWnd* pParent /*=NULL*/)
	: CDialog(CConstraintMainDialog::IDD, pParent), dhelper_ptr(deserthelper_ptr)
{
	m_pToolTip = NULL;
	refresh_needed = true;
	applyAll = false;
	initSize = { 0,0,0,0 };
}

CConstraintMainDialog::~CConstraintMainDialog()
{
	if(m_pToolTip) {
		delete m_pToolTip;
	}
}

void CConstraintMainDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CONSTRAINTLIST, m_listctrl);
	DDX_Control(pDX, IDC_EDITBTN, m_btEdit);
	DDX_Control(pDX, IDC_VALIDATEBTN, m_btValidate);
	DDX_Control(pDX, IDC_COMMITBTN, m_btCommit);
	DDX_Control(pDX, IDC_RESTARTBTN, m_btRestart);
	DDX_Control(pDX, IDC_APPLYBTN, m_btApply);
	DDX_Control(pDX, IDC_APPLYALLBTN, m_btApplyAll);
	DDX_Control(pDX, IDC_BACKBTN, m_btBack);
	DDX_Control(pDX, IDC_FORWARDBTN, m_btForward);
	DDX_Control(pDX, IDC_GENCFGBTN, m_btGenCfgs);
	DDX_Control(pDX, IDC_SELECTBTN, m_btSelect);
	DDX_Control(pDX, IDC_FILTERLIST, m_filterlist);
}

BEGIN_MESSAGE_MAP(CConstraintMainDialog, CDialog)
//	ON_BN_CLICKED(IDC_ADDBTN, &CConstraintMainDialog::OnBnClickedAddbtn)
	ON_WM_DESTROY()
	ON_NOTIFY(LVN_GETINFOTIP, IDC_CONSTRAINTLIST, OnInfoTip)
	ON_NOTIFY(LVN_ITEMCHANGING, IDC_CONSTRAINTLIST, OnConstraintListChanging )
	ON_BN_CLICKED(IDC_EDITBTN, &CConstraintMainDialog::OnBnClickedEditbtn)
	ON_BN_CLICKED(IDC_COMMITBTN, &CConstraintMainDialog::OnBnClickedCommitbtn)
	ON_BN_CLICKED(IDC_APPLYBTN, &CConstraintMainDialog::OnBnClickedApplybtn)
	ON_BN_CLICKED(IDC_APPLYALLBTN, &CConstraintMainDialog::OnBnClickedApplyallbtn)
	ON_BN_CLICKED(IDC_VALIDATEBTN, &CConstraintMainDialog::OnBnClickedValidatebtn)
//	ON_BN_CLICKED(IDC_REMOVEBTN, &CConstraintMainDialog::OnBnClickedRemovebtn)
	ON_BN_CLICKED(IDC_BACKBTN, &CConstraintMainDialog::OnBnClickedBackbtn)
	ON_BN_CLICKED(IDC_FORWARDBTN, &CConstraintMainDialog::OnBnClickedForwardbtn)
	ON_BN_CLICKED(IDC_GENCFGBTN, &CConstraintMainDialog::OnBnClickedGencfgbtn)
	ON_BN_CLICKED(IDOK, &CConstraintMainDialog::OnBnClickedOk)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_CONSTRAINTLIST, &CConstraintMainDialog::OnNMCustomdrawConstraintlist)
	ON_BN_CLICKED(IDC_SELECTBTN, &CConstraintMainDialog::OnBnClickedSelectbtn)
	ON_BN_CLICKED(IDC_RESTARTBTN, &CConstraintMainDialog::OnBnClickedRestartbtn)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CONSTRAINTLIST, &CConstraintMainDialog::OnLvnItemchangedConstraintlist)
	ON_NOTIFY(NM_CLICK, IDC_FILTERLIST, &CConstraintMainDialog::OnNMClickFilterlist)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_FILTERLIST, &CConstraintMainDialog::OnNMCustomdrawFilterlist)
	ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()


// CConstraintMainDialog message handlers
map<int, std::string> CConstraintMainDialog::constraintExprMap;
BOOL CConstraintMainDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_listctrl.InsertColumn(0, _T (""), LVCFMT_LEFT, (int)(WTIMES*m_listctrl.GetStringWidth(_T(""))),0);
	m_listctrl.InsertColumn(1,_T ("Constraint"), LVCFMT_LEFT, (int)(WTIMES*m_listctrl.GetStringWidth(_T("Constraint"))),1);
	m_listctrl.InsertColumn(2, _T("Context"), LVCFMT_LEFT, (int)(WTIMES*m_listctrl.GetStringWidth(_T("Context"))),2);
	m_listctrl.InsertColumn(3, _T("Type"), LVCFMT_LEFT, (int)(WTIMES*m_listctrl.GetStringWidth(_T("Type"))),3);
	m_listctrl.InsertColumn(4, _T("Domain"), LVCFMT_LEFT, (int)(WTIMES*m_listctrl.GetStringWidth(_T("Domains"))),3);
	ListView_SetExtendedListViewStyle(m_listctrl.m_hWnd,LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);
	m_listctrl.SetColumnNum(5);
	m_listctrl.initHeadListCtr(false);

	//Set up the tooltip
	m_pToolTip = new CToolTipCtrl;
	if(!m_pToolTip->Create(this))
	{
	   TRACE("Unable To create ToolTip\n");
	   return TRUE;
	}

	if (!m_pToolTip->AddTool(&m_btEdit,_T("Select a constraint and edit")))
	{
	   TRACE("Unable to add Edit button to the tooltip\n");
	}

	if (!m_pToolTip->AddTool(&m_btValidate,_T("Validate all constraints")))
	{
	   TRACE("Unable to add Validate button to the tooltip\n");
	}

	if (!m_pToolTip->AddTool(&m_btCommit,_T("Save all changes of the constraints")))
	{
	   TRACE("Unable to add Save button to the tooltip\n");
	}

	if (!m_pToolTip->AddTool(&m_btRestart,_T("Start the designSpace exploration without constraint applied")))
	{
	   TRACE("Unable to add Restart button to the tooltip\n");
	}

	if (!m_pToolTip->AddTool(&m_btApply,_T("Apply selected constraints or none to Desert")))
	{
	   TRACE("Unable to add Apply button to the tooltip\n");
	}

	if (!m_pToolTip->AddTool(&m_btApplyAll,_T("Apply all constraints to Desert")))
	{
	   TRACE("Unable to add ApplyAll button to the tooltip\n");
	}

	if (!m_pToolTip->AddTool(&m_btSelect,_T("View and prune/down-select design space")))
	{
	   TRACE("Unable to add View/Select button to the tooltip\n");
	}

	if (!m_pToolTip->AddTool(&m_btBack,_T("Go back configurations")))
	{
	   TRACE("Unable to add Go Back Configurations button to the tooltip\n");
	}

	if (!m_pToolTip->AddTool(&m_btForward,_T("Go forward configurations")))
	{
	   TRACE("Unable to add Go forward configurations button to the tooltip\n");
	}

	if (!m_pToolTip->AddTool(&m_btGenCfgs,_T("Show the configurations")))
	{
	   TRACE("Unable to add Show the configurations button to the tooltip\n");
	}

	m_pToolTip->Activate(TRUE);
	conIdlist.clear();
	dhelper_ptr->applyConstraints(conIdlist, refresh_needed);

	applyAll = false;
	refresh_needed = false;
	applyCounter = 0;
	applyIdx = 0;

	FillList();
	FillSizeBox();
	FillFilter();

	GetDlgItem(IDC_BACKBTN)->EnableWindow(FALSE);
	GetDlgItem(IDC_FORWARDBTN)->EnableWindow(FALSE);

	if (m_gmeWindow != NULL) // gmeWindow could be NULL if this component is invoked outside of GME
	{
		::EnableWindow(m_gmeWindow, FALSE); // call this after you ShowWindow your window

#ifdef _DEBUG
		CString gmeWindowText;
		::GetWindowText(m_gmeWindow, gmeWindowText.GetBuffer(500), 500);
		gmeWindowText.ReleaseBuffer();
		OutputDebugString(gmeWindowText.GetBuffer()); // prints the title of the GME window, e.g. MetaGME - UML
#endif
	}

	//initialize resize anchor
	BOOL bOk = FALSE;
	bOk = m_resizer.Hook(this);
	ASSERT( bOk);

	bOk = m_resizer.SetAnchor(IDC_CONSTRAINTLIST, ANCHOR_ALL | ANCHOR_TOP | ANCHOR_LEFT);
	ASSERT( bOk);

	bOk = m_resizer.SetAnchor(IDC_STATIC1, ANCHOR_RIGHT);
	ASSERT( bOk);
	bOk = m_resizer.SetAnchor(IDC_STATIC2, ANCHOR_RIGHT);
	ASSERT( bOk);
	bOk = m_resizer.SetAnchor(IDC_STATIC3, ANCHOR_RIGHT);
	ASSERT( bOk);
	bOk = m_resizer.SetAnchor(IDC_STATIC4, ANCHOR_RIGHT);
	ASSERT( bOk);

	bOk = m_resizer.SetAnchor(IDC_EDITBTN, ANCHOR_RIGHT);
	ASSERT( bOk);
	bOk = m_resizer.SetAnchor(IDC_VALIDATEBTN, ANCHOR_RIGHT);
	ASSERT( bOk);
	bOk = m_resizer.SetAnchor(IDC_COMMITBTN, ANCHOR_RIGHT);
	ASSERT( bOk);
	bOk = m_resizer.SetAnchor(IDC_RESTARTBTN, ANCHOR_RIGHT);
	ASSERT( bOk);
	bOk = m_resizer.SetAnchor(IDC_APPLYBTN, ANCHOR_RIGHT);
	ASSERT( bOk);
	bOk = m_resizer.SetAnchor(IDC_APPLYALLBTN, ANCHOR_RIGHT);
	ASSERT( bOk);
	bOk = m_resizer.SetAnchor(IDC_BACKBTN, ANCHOR_RIGHT);
	ASSERT( bOk);
	bOk = m_resizer.SetAnchor(IDC_FORWARDBTN, ANCHOR_RIGHT);
	ASSERT( bOk);
	bOk = m_resizer.SetAnchor(IDC_SELECTBTN, ANCHOR_RIGHT);
	ASSERT( bOk);
	bOk = m_resizer.SetAnchor(IDC_GENCFGBTN, ANCHOR_RIGHT);
	ASSERT( bOk);
	bOk = m_resizer.SetAnchor(IDOK, ANCHOR_RIGHT);
	ASSERT( bOk);
	bOk = m_resizer.SetAnchor(IDC_DSSIZEEDIT, ANCHOR_HORIZONTALLY | ANCHOR_BOTTOM );
	ASSERT( bOk);
	bOk = m_resizer.SetAnchor(IDC_FILTERLIST, ANCHOR_HORIZONTALLY | ANCHOR_BOTTOM);
	ASSERT( bOk);
	bOk = m_resizer.SetAnchor(IDC_FILTERGROUP, ANCHOR_HORIZONTALLY | ANCHOR_BOTTOM);
	ASSERT( bOk);
	bOk = m_resizer.SetAnchor(IDC_CONSGROUP, ANCHOR_HORIZONTALLY | ANCHOR_TOP | ANCHOR_BOTTOM);
	ASSERT( bOk);

	GetWindowRect(&initSize);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CConstraintMainDialog::OnDestroy()
{
	if (m_gmeWindow != NULL)
	{
		::EnableWindow(m_gmeWindow, TRUE);
		m_gmeWindow = NULL;
	}
}

void CConstraintMainDialog::FillList()
{
	set<std::string> conset;
	if(!refresh_needed)
		dhelper_ptr->getAppliedConstraintSet(conset);

	m_listctrl.DeleteAllItems();
	constraintExprMap.clear();
	appliedConSet.clear();

	int conSize = dhelper_ptr->getConstraintSize();
	for(int i=0; i<conSize; ++i)
	{
		std::string name, context, expression, ctype, domain;
		if(dhelper_ptr->getConstraint(i, name, context, expression, ctype, domain))
		{
			constraintExprMap[i] = expression;
			//bool constraint_applied = conset.find(name)!=conset.end();
			if(conset.find(name)!=conset.end())
				appliedConSet.insert(i);
			if(invalidConstraintName_set.find(CString(name.c_str()))!=invalidConstraintName_set.end())
				invalidConSet.insert(i);

			FillList(i, name, context,ctype,domain);
		}
		else
		{
			//considering update. This case should not happen!
		}
	}
	m_listctrl.Sort();

}

void CConstraintMainDialog::FillList(int index,const std::string &cons_name, const std::string &cons_context, const std::string &cons_type, const std::string &cons_domain)
{
	int ret, req, is;
	int added_index = m_listctrl.AddItem(index, _T(""), utf82cstring(cons_name), utf82cstring(cons_context), utf82cstring(cons_type), utf82cstring(cons_domain));
	if (appliedConSet.find(index) != appliedConSet.end())
	{
		ListView_SetCheckState(m_listctrl.GetSafeHwnd(), added_index, 1);
	}

	m_listctrl.SetColumnWidth(0,35);

	// fix column width
	req = (int)(WTIMES*m_listctrl.GetStringWidth(utf82cstring(cons_name)));
	is = m_listctrl.GetColumnWidth(1);
	if ( is < req ) m_listctrl.SetColumnWidth(1, req);

	//item.iItem = ret;
	//item.mask = LVIF_TEXT;
	//item.iImage = 0;
	//item.lParam = 0;

	// second column
	//item.iSubItem = 1;
	//item.pszText = (LPSTR)cons_context.c_str();
	//ret = m_listctrl.SetItem(&item);
	//
	// fix column width
	req = (int)(WTIMES*m_listctrl.GetStringWidth(utf82cstring(cons_context)));
	is = m_listctrl.GetColumnWidth(2);
	if ( is < req ) m_listctrl.SetColumnWidth(2, req);

	// third column
	//item.iSubItem = 2;
	//item.pszText = (LPSTR)cons_type.c_str();
	//ret = m_listctrl.SetItem(&item);

	// fix column width
	req = (int)(WTIMES*m_listctrl.GetStringWidth(utf82cstring(cons_type)));
	is = m_listctrl.GetColumnWidth(3);
	if ( is < req ) m_listctrl.SetColumnWidth(3, req);

	// fix column width
	req = (int)(WTIMES*m_listctrl.GetStringWidth(utf82cstring(cons_domain)));
	is = m_listctrl.GetColumnWidth(4);
	if ( is < req ) m_listctrl.SetColumnWidth(4, req);
}

void CConstraintMainDialog::OnConstraintListChanging(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pL = (LPNMLISTVIEW)pNMHDR;
	if (appliedConSet.find(pL->iItem) != appliedConSet.end())
	{
		LPNMLISTVIEW pL = (LPNMLISTVIEW)pNMHDR;
		if ((pL->uOldState & 0x2000) != 0 && (pL->uNewState & 0x1000) != 0)
		{
			*pResult = true;
			return;
		}
	}
	*pResult = false;
}


void CConstraintMainDialog::OnInfoTip(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVGETINFOTIP*pInfoTip = reinterpret_cast<NMLVGETINFOTIP*>(pNMHDR);
	ASSERT(pInfoTip);

	int i = m_listctrl.GetItemID(pInfoTip->iItem);
	map<int, std::string>::iterator pos = constraintExprMap.find(i);
	if(pos!=constraintExprMap.end())
		pInfoTip->pszText = const_cast<LPTSTR>(static_cast<LPCTSTR>(utf82cstring(constraintExprMap[i].c_str())));
}

void CConstraintMainDialog::OnBnClickedEditbtn()
{
	// TODO: Add your control notification handler code here
	int n = m_listctrl.GetItemCount();
	int sel = -1;
	UINT uSelectedCount = m_listctrl.GetSelectedCount();
	if (uSelectedCount > 0)
		sel = m_listctrl.GetNextItem(sel, LVNI_SELECTED);
	else
		for(int i=0; i<n&&sel==-1; ++i)
		{
			if(m_listctrl.GetCheck(i)) sel = i;
		}

	if(sel==-1) {
		return;
	}

	int con_index = m_listctrl.GetItemID(sel);
	CConstraintEditDialog editdlg(m_listctrl.GetItemText(sel,1), (CString)(constraintExprMap[con_index].c_str()),dhelper_ptr);
	if(editdlg.DoModal()==IDOK)
	{
		//update the constraint name and expression
		m_listctrl.SetItemText(sel,1,editdlg.m_name);
		constraintExprMap[con_index] = tstring2utf8(editdlg.m_expression);

		//update the desertIface file
		dhelper_ptr->updateConstraint(con_index, tstring2utf8(editdlg.m_name), tstring2utf8(editdlg.m_expression));
//		m_listctrl.SetCheck(sel);
	}
	//refresh_needed = true;
	initConstraints();
}

void CConstraintMainDialog::OnBnClickedCommitbtn()
{
	// TODO: Add your control notification handler code here
	dhelper_ptr->updateCyPhy();
}

void CConstraintMainDialog::OnBnClickedApplybtn()
{
	// TODO: Add your control notification handler code here
	if(refresh_needed) {
		dhelper_ptr->closeDesertIfaceBackDN();
	}
	std::string conslist="";
	int cnt = m_listctrl.GetItemCount();
	//set<int> conIdlist;
	conIdlist.clear();
	for(int i=0; i<cnt; ++i)
	{
		if(!m_listctrl.GetCheck(i)) continue;
		conIdlist.insert(m_listctrl.GetItemID(i));
	}
	if(conIdlist.empty()) return;
	try{
		dhelper_ptr->applyConstraints(conIdlist, refresh_needed);
//		appliedCons.push_back(conIdlist);
	//	currConListPosition++;
		applyAll = false;
		refresh_needed = false;
	}catch(udm_exception &exc)
	{
		AfxMessageBox(CString(exc.what()));
	}
	catch(CDesertException *e)
	{
		invalidConstraintName_set.insert(e->GetConstraintName());
		e->Delete();
	}
//	FillSizeBox();
	applyCounter++;
	applyCounter %= MAX_FILTERS;
	applyIdx++;
	filters[applyCounter].clear();
	int nFilters = m_filterlist.GetItemCount();
	for(int ifilter = 0;ifilter<nFilters;ifilter++)
	{
		if(m_filterlist.GetCheck(ifilter))
			filters[applyCounter].insert(ifilter);
	}
	update();
}

void CConstraintMainDialog::OnBnClickedApplyallbtn()
{
	// TODO: Add your control notification handler code here
	conIdlist.clear();
	if(refresh_needed)
	{
		dhelper_ptr->closeDesertIfaceBackDN();
		dhelper_ptr->applyConstraints(conIdlist, true);
		// dspSize = dhelper_ptr->getDesignSpaceSize();
		refresh_needed = false;
	}	//for the purpose to get the correct dspSize

	int cnt = m_listctrl.GetItemCount();
	for(int i=0; i<cnt; ++i)
	{
		conIdlist.insert(m_listctrl.GetItemID(i));
	}
//	dhelper_ptr->applyAllConstraints(refresh_needed);
	try{
	dhelper_ptr->applyConstraints(conIdlist, refresh_needed);
	}catch(udm_exception &exc)
	{
//		AfxMessageBox(CString(exc.what()));
	//	initConstraints();
		if(dhelper_ptr->isBackNavigable()) {
			dhelper_ptr->goBack();
		} else {
			initConstraints();
	}
		FillList();
		return;
	}
	catch(CDesertException *e)
	{
		invalidConstraintName_set.insert(e->GetConstraintName());
		e->Delete();
		if(dhelper_ptr->isBackNavigable()) {
			dhelper_ptr->goBack();
		} else {
			initConstraints();
	}
		FillList();
		return;
	}
//	appliedCons.push_back(conIdlist);
//	currConListPosition++;

	// dspSize = dhelper_ptr->getDesignSpaceSize();
	dspSize = dhelper_ptr->getRealConfigCount();
	applyAll = true;

	applyCounter++;
	applyCounter %= MAX_FILTERS; // FIXME this won't crash, but is it correct?
	applyIdx++;
	filters[applyCounter].clear();
	int nFilters = m_filterlist.GetItemCount();
	for(int ifilter = 0;ifilter<nFilters;ifilter++)
	{
		if(m_filterlist.GetCheck(ifilter))
			filters[applyCounter].insert(ifilter);
	}
	update();
}

void CConstraintMainDialog::OnBnClickedValidatebtn()
{
	// TODO: Add your control notification handler code here
	if(dhelper_ptr->checkConstraints())
		AfxMessageBox(_T("The constraints are valid."));
	initConstraints();
	FillSizeBox();
}

BOOL CConstraintMainDialog::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
     if (NULL != m_pToolTip)
            m_pToolTip->RelayEvent(pMsg);

	return CDialog::PreTranslateMessage(pMsg);
}

void CConstraintMainDialog::OnBnClickedBackbtn()
{
	dhelper_ptr->goBack();
	applyIdx--;
	//FillSizeBox();
	update();
}

void CConstraintMainDialog::OnBnClickedForwardbtn()
{
	dhelper_ptr->goForward();
	applyIdx++;
	update();
}

void CConstraintMainDialog::OnBnClickedGencfgbtn()
{
	// TODO: Add your control notification handler code here
	generateConfig();
	update();
}

void CConstraintMainDialog::FillSizeBox()
{
	dspSize = dhelper_ptr->getRealConfigCount();
	// dspSize = dhelper_ptr->getDesignSpaceSize();
	TCHAR buffer[65];
	_i64tot(dspSize, buffer, 10);
	tstring str = _T("Current no. of configurations: ");
	if(dspSize == -1) {
		str += _T("Not calculated yet");
	} else if(dspSize == DESIGN_SPACE_TOO_LARGE) {
		str += _T("Design Space Too Large");
	} else {
		str += (tstring) buffer;
	}
//	dhelper_ptr->getDesignSpaceSize(dspSize, repSize);
//	CString str; str.Format("Design Space (Encoded): %3.2e        EncodingLength: %6d", dspSize, repSize);
	CEdit *box = (CEdit *)GetDlgItem(IDC_DSSIZEEDIT);
	box->SetWindowText(str.c_str());
}

void CConstraintMainDialog::update()
{
	dhelper_ptr->isBackNavigable()&&applyIdx > 0 ? GetDlgItem(IDC_BACKBTN)->EnableWindow(true) : GetDlgItem(IDC_BACKBTN)->EnableWindow(FALSE);
	dhelper_ptr->isForwardNavigable()&&applyIdx < applyCounter ? GetDlgItem(IDC_FORWARDBTN)->EnableWindow(true) : GetDlgItem(IDC_FORWARDBTN)->EnableWindow(FALSE);
	FillList();
	FillSizeBox();
	FillFilter();
}

void CConstraintMainDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	/*if(!dhelper_ptr->isCfgModelsNull() )
	{
		if(AfxMessageBox("Do you want to save the exported configurations?", MB_ICONQUESTION | MB_YESNO)==IDNO)
		{
			dhelper_ptr->removeCfgs();
		}
	}*/
	for (int i=0; i<MAX_FILTERS; i++)
	{
		  filters[i].clear();
	}

	CDialog::OnOK();
}

void CConstraintMainDialog::generateConfig()
{
	if(dspSize == DESIGN_SPACE_TOO_LARGE)
	{
		if(IDNO==AfxMessageBox(_T("The configurations cannot be shown due to the too large design space.\nPlease apply constraints or use View/Select for further pruning/down-selecting.\n\nDo you want to proceed?"),  MB_ICONQUESTION |MB_YESNO |MB_DEFBUTTON2))
			return;
	}

	if(dhelper_ptr->isLastDesertFinit_2_fail())
	{
		if(applyAll) {
			dhelper_ptr->applyAllConstraints(true);
		} else {
			dhelper_ptr->applyConstraints(conIdlist, true);
		}
	}

	int ret = dhelper_ptr->runDesertFinit_2();
	if(ret<=0) {
		if(ret==0)
			AfxMessageBox(_T("There are no configurations generated."));
		set<int> emptyListOfConstraints;
		dhelper_ptr->applyConstraints(emptyListOfConstraints, true);
		FillSizeBox();
		update();
	}
	else
	{
		CDesertConfigDialog cfgdlg(dhelper_ptr, this);

		if(cfgdlg.DoModal()==100)
		{
			dhelper_ptr->removeCfgsIfEmpty();
			CDialog::OnOK();
		}
		else
		{
			dhelper_ptr->removeCfgsIfEmpty();
			dhelper_ptr->closeDesertIfaceBackDN();
			initConstraints();
		}

	}
}

void CConstraintMainDialog::OnNMCustomdrawConstraintlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );

	// Take the default processing unless we set this to something else below.
	*pResult = CDRF_DODEFAULT;

	// First thing - check the draw stage. If it's the control's prepaint
	// stage, then tell Windows we want messages for every item.

	if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
	{
	// This is the notification message for an item. We'll request
	// notifications before each subitem's prepaint stage.

		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
	else if ( (CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage )
	{
		COLORREF crText;
		int nItem = pLVCD->nmcd.dwItemSpec;
		if( invalidConSet.find(m_listctrl.GetItemID(nItem))!= invalidConSet.end() )
			crText = RGB(255,0,0); //red
		else if ( appliedConSet.find(m_listctrl.GetItemID(nItem))!= appliedConSet.end() )
			crText = RGB(190,190,190);  //grey
		else
			crText = RGB(0,0,0);  //black

		// Store the colors back in the NMLVCUSTOMDRAW struct.
		pLVCD->clrText = crText;
		*pResult = CDRF_DODEFAULT;
	}
}


void CConstraintMainDialog::OnBnClickedSelectbtn()
{
	// TODO: Add your control notification handler code here
	CDSESelectorDialog seldlg(dhelper_ptr);
	if(seldlg.DoModal()==IDCANCEL) return;

	set<int> selectedElems = seldlg.getSelectedElems();
	dhelper_ptr->generateSelectionConstraints(selectedElems);
	initConstraints();
}


void CConstraintMainDialog::OnBnClickedRestartbtn()
{
	// TODO: Add your control notification handler code here
	//conIdlist.clear();
	initConstraints();

	int nFilter = m_filterlist.GetItemCount();
	for(int i=0;i<nFilter; i++)
		m_filterlist.SetCheck(i,0);
}

void CConstraintMainDialog::initConstraints()
{
	conIdlist.clear();
	dhelper_ptr->applyConstraints(conIdlist, true);
	applyCounter = 0;
	applyIdx = 0;
	for (int i=0; i<MAX_FILTERS; i++)
	{
	  filters[i].clear();
	}

	//FillSizeBox();
	update();
	applyAll = false;
	refresh_needed = false;
}


void CConstraintMainDialog::OnLvnItemchangedConstraintlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CConstraintMainDialog::FillFilter()
{
	m_filterlist.DeleteAllItems();
	m_filterlist.SetExtendedStyle(LVS_EX_CHECKBOXES |LVS_ALIGNTOP);

	POINT pt, pt0;
	m_filterlist.InsertItem(0,_T("All"));
	m_filterlist.GetItemPosition(0, &pt0);
	pt = pt0;

	int nItem = 1;
	tstring preStr = _T("All");
	set<std::string> domains = dhelper_ptr->getDomainSet();
	if(domains.find("Visual Constraints")!=domains.end())
	{
		m_filterlist.InsertItem(nItem,_T("Visual Constraints"));
		pt.x = pt.x+ (int)(m_filterlist.GetStringWidth(preStr.c_str()))+50;
		m_filterlist.SetItemPosition(nItem, pt);
		nItem++;
		domains.erase("Visual Constraints");
		preStr = _T("Visual Constraints");
	}

	if(domains.find("Property Constraints")!=domains.end())
	{
		m_filterlist.InsertItem(nItem,_T("Property Constraints"));
		pt.x = pt.x+ (int)(m_filterlist.GetStringWidth(preStr.c_str()))+50;
		m_filterlist.SetItemPosition(nItem, pt);
		nItem++;
		domains.erase("Property Constraints");
		preStr = _T("Property Constraints");
	}

	if(domains.find("Parameter Constraints")!=domains.end())
	{
		m_filterlist.InsertItem(nItem,_T("Parameter Constraints"));
		pt.x = pt.x+ (int)(m_filterlist.GetStringWidth(preStr.c_str()))+50;
		m_filterlist.SetItemPosition(nItem, pt);
		nItem++;
		domains.erase("Parameter Constraints");
		preStr = _T("Parameter Constraints");
	}

	if(domains.find("OCL")!=domains.end())
	{
		m_filterlist.InsertItem(nItem,_T("OCL"));
		pt.x = pt.x+ (int)(m_filterlist.GetStringWidth(preStr.c_str()))+50;
		m_filterlist.SetItemPosition(nItem, pt);
		nItem++;
		domains.erase("OCL");
		preStr = _T("");
	}

	pt.y += 20;
	//for each row, put 5 domain filter checkboxes
	int row = 0;
	for(auto it=domains.begin();it!=domains.end(); ++it)
	{
		if(row==5)
		{
			pt.y += 20;
			row = 0;
		}
		std::string domain = *it;
		m_filterlist.InsertItem(nItem, utf82cstring(domain));
		if(row==0)
			pt.x = pt0.x;
		else
			pt.x = pt.x+ (int)(m_filterlist.GetStringWidth(preStr.c_str()))+50;
		m_filterlist.SetItemPosition(nItem, pt);
		nItem++;
		preStr = utf82cstring(domain);
		row++;
	}
	int nFilters = m_filterlist.GetItemCount();
	for(int i=0;i<nFilters;i++)
	{
		if(filters[applyIdx%MAX_FILTERS].find(i)!=filters[applyIdx%MAX_FILTERS].end())
			m_filterlist.SetCheck(i);
		else
			m_filterlist.SetCheck(i,0);
	}
}

void CConstraintMainDialog::setDomainCheck(int nItem, bool check)
{
	int nFilter = m_filterlist.GetItemCount();
	int nCons = m_listctrl.GetItemCount();

	if(!check) {
		m_filterlist.SetCheck(0,0);
	} else
	{
		bool allChecked=true;
		for(int i=1;i<nFilter; i++)
		{
			if(i==nItem) {
				continue;
			}
			if(!m_filterlist.GetCheck(i))
			{
				allChecked = false;
				break;
			}
		}
		if(allChecked)
		{
			m_filterlist.SetCheck(0);
			for(int i=0;i<nCons; i++)
				m_listctrl.SetCheck(i);
		}
	}

	for(int j=0; j<nCons; j++)
	{
		m_listctrl.SetCheck(j,0);
	}
	for(int i=1; i<nFilter; i++)
	{
		if(i!=nItem && !m_filterlist.GetCheck(i))
			continue;
		else if(i==nItem && m_filterlist.GetCheck(i))
			continue;

		std::string domain = tstring2utf8(m_filterlist.GetItemText(i, 0));
		set<int> conlist = dhelper_ptr->getDesertConstraintlist(domain);
		for(int j=0; j<nCons; j++)
		{
			int realId = m_listctrl.GetItemID(j);
			if(conlist.find(realId)!=conlist.end())
			{
				m_listctrl.SetCheck(j);
			}
		}
	}
}

void CConstraintMainDialog::setCheckAll(bool check)
{
	int nFilter = m_filterlist.GetItemCount();
	int i;
	for(i=1;i<nFilter; i++)
		m_filterlist.SetCheck(i,check);

	int nCons = m_listctrl.GetItemCount();
	for(i=0;i<nCons; i++)
		m_listctrl.SetCheck(i,check);
}


void CConstraintMainDialog::OnNMClickFilterlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	LVHITTESTINFO hitinfo;

     //Copy click point
     hitinfo.pt = pNMLV->ptAction;

     //Make the hit test...
     int nItem = m_filterlist.HitTest(&hitinfo);

     if(hitinfo.flags != LVHT_ONITEMSTATEICON)
          return; // Didn't click on an icon

	std::string domain = tstring2utf8(m_filterlist.GetItemText(nItem, 0));
	bool bchecked;
	if(m_filterlist.GetItemState(nItem,LVIS_SELECTED) != LVIS_SELECTED) {
		bchecked = m_filterlist.GetCheck(nItem);
		bchecked = !bchecked;
	}

	if(nItem==0) {
		setCheckAll(bchecked);
	} else {
		setDomainCheck(nItem, bchecked);
	}
}


void CConstraintMainDialog::OnNMCustomdrawFilterlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );

	// Take the default processing unless we set this to something else below.
	*pResult = CDRF_DODEFAULT;

	// First thing - check the draw stage. If it's the control's prepaint
	// stage, then tell Windows we want messages for every item.

	if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		COLORREF crText;
		int nItem = pLVCD->nmcd.dwItemSpec;
		if(filters[applyIdx%MAX_FILTERS].find(nItem)!=filters[applyIdx%MAX_FILTERS].end())
			crText = RGB(190,190,190);  //grey
		else
			crText = RGB(0,0,0);  //black

		// Store the colors back in the NMLVCUSTOMDRAW struct.
		pLVCD->clrText = crText;
		*pResult = CDRF_DODEFAULT;
	}
}

void CConstraintMainDialog::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	__super::OnGetMinMaxInfo(lpMMI);
	if (initSize.right != 0) {
		lpMMI->ptMinTrackSize.x = initSize.right - initSize.left;
		lpMMI->ptMinTrackSize.y = initSize.bottom - initSize.top;
	}
}
