// DesertConfigDialog.cpp : implementation file
//

#include "StdAfx.h"
#include "DesertConfigDialog.h"
#include "commctrl.h"
#include <fstream>
#include <sstream>
#include "MophyMatrixFileDlg.h"
#include "CyPhy2MorphMatrixFileDlg.h"
// #include "XLAutomation.h"

#define WTIMES	1.6

IMPLEMENT_DYNAMIC(CDesertConfigDialog, CDialog)

CDesertConfigDialog::CDesertConfigDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CDesertConfigDialog::IDD, pParent)
	//, m_outputfdr(_T(""))
	, dhelper_ptr(0)
{
	isActiveCfg = true;
}

CDesertConfigDialog::CDesertConfigDialog(DesertHelper *deserthelper_ptr, CWnd* pParent)
	: CDialog(CDesertConfigDialog::IDD, pParent)
	//, m_outputfdr(_T(""))
	, dhelper_ptr(deserthelper_ptr)
{
	dhelper_ptr->clearCfgId2NameMap();
	dhelper_ptr->createNewConfigurations();
}

CDesertConfigDialog::~CDesertConfigDialog()
{
}

void CDesertConfigDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CFGLIST, m_cfglist);
	DDX_Control(pDX, IDC_CFGTREE, m_cfgtree);
	DDX_Control(pDX, IDC_CFGSIZE, m_cfgsize);
}

BEGIN_MESSAGE_MAP(CDesertConfigDialog, CDialog)
	ON_BN_CLICKED(IDC_EXPORTSELBTN, &CDesertConfigDialog::OnBnClickedExportselbtn)
	ON_BN_CLICKED(IDC_EXPORTALLBTN, &CDesertConfigDialog::OnBnClickedExportallbtn)
	ON_BN_CLICKED(IDCANCEL, &CDesertConfigDialog::OnBnClickedCancel)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CFGLIST, &CDesertConfigDialog::OnLvnItemchangedCfglist)
	ON_BN_CLICKED(IDC_CLEARALLBTN, &CDesertConfigDialog::OnBnClickedClearallbtn)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_CONSTRAINTLIST, &CDesertConfigDialog::OnNMCustomdrawConstraintlist)
	ON_BN_CLICKED(IDC_CYPHY2MORPHMATRIX, &CDesertConfigDialog::OnBnClickedCyPhy2MorphMatrix)
	ON_BN_CLICKED(IDC_MORPHMATRIX2CYPHY, &CDesertConfigDialog::OnBnClickedMorphMatrix2CyPhy)
	// ON_NOTIFY(NM_CLICK, IDC_CFGLIST, &CDesertConfigDialog::OnNMClkCfgList)
	ON_NOTIFY(NM_DBLCLK, IDC_CFGLIST, &CDesertConfigDialog::OnNMDblclkCfglist)
	ON_NOTIFY(LVN_BEGINLABELEDIT, IDC_CFGLIST, &CDesertConfigDialog::OnLvnBeginlabeleditCfglist)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_CFGLIST, &CDesertConfigDialog::OnLvnEndlabeleditCfglist)
	ON_BN_CLICKED(IDC_CLOSEALLBTN, &CDesertConfigDialog::OnBnClickedCloseallbtn)
END_MESSAGE_MAP()


// CDesertConfigDialog message handlers

BOOL CDesertConfigDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	stringstream ss;
	long cfgCount = dhelper_ptr->getRealConfigCount();
	if(cfgCount == -2) {
		cfgSizeInfo = "Design Space Too Large";
	} else if(cfgCount == -1) {
		cfgSizeInfo = "Not calculated yet";
	} else {
		ss << cfgCount;
		cfgSizeInfo = ss.str();
	}
	
	FillCfgList();
	FillCfgTree();

	checkedSize = 0;
	updateSize();

	m_blInited = false;

	//initialize resize anchor
	BOOL bOk = FALSE;
	bOk = m_resizer.Hook(this);
	ASSERT( bOk);

	bOk = m_resizer.SetAnchor(IDC_CFGSIZE, ANCHOR_HORIZONTALLY | ANCHOR_TOP);
	ASSERT( bOk);

	bOk = m_resizer.SetAnchor(IDC_CLEARALLETN, ANCHOR_TOP | ANCHOR_RIGHT);
	ASSERT( bOk);

	bOk = m_resizer.SetAnchor(IDC_CFGLIST, ANCHOR_VERTICALLY);
	ASSERT( bOk);

	bOk = m_resizer.SetAnchor(IDC_CFGTREE, ANCHOR_ALL);
	ASSERT( bOk);

	bOk = m_resizer.SetAnchor(IDC_EXPORTSELBTN, ANCHOR_BOTTOM );
	ASSERT( bOk);

	bOk = m_resizer.SetAnchor(IDC_EXPORTALLBTN, ANCHOR_BOTTOM | ANCHOR_LEFT);
	ASSERT( bOk);

	bOk = m_resizer.SetAnchor(IDCANCEL, ANCHOR_BOTTOM |ANCHOR_RIGHT );
	ASSERT( bOk);

	bOk = m_resizer.SetAnchor(IDC_CLOSEALLBTN, ANCHOR_BOTTOM | ANCHOR_RIGHT );
	ASSERT( bOk);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDesertConfigDialog::OnBnClickedExportselbtn()
{
	// TODO: Add your control notification handler code here
	set<int> ids;
	int cnt = m_cfglist.GetItemCount();
	for(int i=0; i<cnt; ++i)
	{
		if(m_cfglist.GetCheck(i))
			ids.insert(m_cfglist.GetItemID(i));
	}
	dhelper_ptr->exportModel_ex(ids);
}

void CDesertConfigDialog::OnBnClickedExportallbtn()
{
	// TODO: Add your control notification handler code here
	set<int> ids;
	int cnt = m_cfglist.GetItemCount();
	for(int i=0; i<cnt; ++i)
	{
		ids.insert(i);
	}
//	dhelper_ptr->exportModel_ex(ids,useCurrentCyphy,(LPCTSTR)m_outputfdr);
	dhelper_ptr->exportModel_ex(ids);
}

void CDesertConfigDialog::FillCfgList()
{
	m_cfglist.DeleteAllItems();
	m_cfglist.TurnOffToolTip();
	CRect rect;
	m_cfglist.GetClientRect(&rect);
	m_cfglist.InsertColumn(0, _T (""), LVCFMT_LEFT, (int)(WTIMES*m_cfglist.GetStringWidth(_T(" "))),0);
	m_cfglist.InsertColumn(1, _T("Cfg.No."), LVCFMT_LEFT, (int)(WTIMES*m_cfglist.GetStringWidth(_T("Cfg.No."))),1);
	m_cfglist.InsertColumn(2, _T("Cfg.Id."), LVCFMT_LEFT, (int)(WTIMES*m_cfglist.GetStringWidth(_T("Cfg.Id."))),2);
	ListView_SetExtendedListViewStyle(m_cfglist.m_hWnd, LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER);
	// disable label editing
	SetWindowLong(m_cfglist.GetSafeHwnd(), GWL_STYLE, GetWindowLong(m_cfglist.GetSafeHwnd(), GWL_STYLE) & (~LVS_EDITLABELS));

	m_cfglist.SetColumnNum(3);
	m_cfglist.initHeadListCtr();
	m_cfglist.SetColumnWidth(0,35);
	cfgSize = dhelper_ptr->getRealConfigCount();
	for(long i=0; i<cfgSize; ++i)
	{
		std::string name, no, id;
		if(dhelper_ptr->getConfiguration(i,name,no, id))
		{
			int ret, req, is;
			LV_ITEM item;
			item.state = item.stateMask = 0;	

			item.lParam = 0;
			item.iItem = i;
			(void)m_cfglist.AddItem(i, _T(""), utf82cstring(no), utf82cstring(id));
			// fix column width
			req = (int)(WTIMES*m_cfglist.GetStringWidth(utf82cstring(id.c_str())));
			is = m_cfglist.GetColumnWidth(2);
			if ( is < req ) m_cfglist.SetColumnWidth(2, req);
		}
	}
	m_cfglist.Sort();
}

void CDesertConfigDialog::FillCfgTree()
{
	m_cfgtree.DeleteAllItems();
	DesertIface::Space space;
	dhelper_ptr->getDesertIfaceSpace(space);

	std::string iname = space.name();
	TVINSERTSTRUCT tvInsert;
	tvInsert.hParent = NULL;
	tvInsert.hInsertAfter = NULL;
	tvInsert.item.mask = TVIF_TEXT;
	tvInsert.item.pszText = _T("DesignSpace");
	m_cfgTreeRootItem = m_cfgtree.InsertItem(&tvInsert);
//	m_cfgtree.SetItemState( m_cfgTreeRootItem, TVIS_BOLD, TVIS_BOLD ); //bold
//	m_cfgtree.SetItemState( cfg_root, 0, TVIS_BOLD );  //unbold
	
	set<DesertIface::Element> elems = space.Element_kind_children();
	for(set<DesertIface::Element>::iterator i=elems.begin();i!=elems.end();++i)
	{
		if(!dhelper_ptr->isElementExist((*i).externalID())) continue;
		iname = (*i).name();
		HTREEITEM elem_node = m_cfgtree.InsertItem(utf82cstring(iname), m_cfgTreeRootItem, TVI_LAST);
		FillCfgTree(DesertIface::Element(*i), elem_node);
		m_cfgtree.SortChildren(elem_node);
		m_cfgtree.Expand(elem_node,TVE_EXPAND);
		cfgTreeMap[elem_node]=(*i).externalID();
	}
	m_cfgtree.SortChildren(m_cfgTreeRootItem);
	m_cfgtree.Expand(m_cfgTreeRootItem, TVE_EXPAND);
}

void CDesertConfigDialog::FillCfgTree(DesertIface::Element &elem, HTREEITEM parent)
{
	set<DesertIface::Element> elems = elem.Element_kind_children();
	for(set<DesertIface::Element>::iterator i=elems.begin();i!=elems.end();++i)
	{
		if(!dhelper_ptr->isElementExist((*i).externalID())) continue;
		DesertIface::Element currElem = *i;
		std::string iname = currElem.name();
		if(currElem.decomposition())
			iname+=" [Compound]";
		else
		{
			set<DesertIface::Element> child_elems = currElem.Element_kind_children();
			if(!child_elems.empty())
			{
				if(dhelper_ptr->isOptionalElement(currElem))
					iname+=" [Optional]";
				else
					iname+=" [Alternative]";
			}
			else
			{
				//add number it envolved in configurations [#/total#]
				if(iname=="null")
				{
					int cnt = dhelper_ptr->getElementInConfigsCount(currElem.externalID());
					char buffer[10];
					_itoa(cnt, buffer, 10);
					iname = iname + " [" +(std::string)buffer+"/"+cfgSizeInfo+"]";
				}
				else
				{
					std::string cnt_str = dhelper_ptr->getElementInConfigsString(currElem.externalID());
					iname = iname + " [" +cnt_str+"]";
				}
			}
		}
		HTREEITEM elem_node = m_cfgtree.InsertItem(utf82cstring(iname), parent, TVI_LAST);
		FillCfgTree(DesertIface::Element(*i),elem_node);
		m_cfgtree.SortChildren(elem_node);
		m_cfgtree.Expand(elem_node,TVE_EXPAND);
		cfgTreeMap[elem_node]=(*i).externalID();
	}
	//leaf node
}

void CDesertConfigDialog::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
//	dhelper_ptr->closeDesertIfaceBackDN();
	OnCancel();
}

void CDesertConfigDialog::OnLvnItemchangedCfglist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here	
	if( pNMLV->uNewState & LVIS_SELECTED )
	{
		int sel = pNMLV->iItem;
		for(map<HTREEITEM, int>::iterator it=cfgTreeMap.begin();it!=cfgTreeMap.end();++it)
		{
			HTREEITEM node = (*it).first;			
			int elemId = (*it).second;
			if(elemId==0)
			{
				HTREEITEM parent_node = m_cfgtree.GetParentItem(node);
				
				HTREEITEM hNextItem;
				HTREEITEM hChildItem = m_cfgtree.GetChildItem(parent_node);

				bool selected = true;
				bool otherSel = false;
				while (hChildItem != NULL)
				{
					map<HTREEITEM, int>::iterator pos = cfgTreeMap.find(hChildItem);
					if(pos!=cfgTreeMap.end())
					{
						int itemId = (*pos).second;
						if(itemId!=0 && dhelper_ptr->isElementSelected(sel, itemId, true))
						{
							selected = false;
							otherSel = true;
							break;
						}
					}
					hNextItem = m_cfgtree.GetNextItem(hChildItem, TVGN_NEXT);
					hChildItem = hNextItem;
				}	
				if(!otherSel)
				{
					map<HTREEITEM, int>::iterator pos = cfgTreeMap.find(parent_node);
					if(pos!=cfgTreeMap.end())
					{
						int itemId = (*pos).second;
						if(itemId!=0 && dhelper_ptr->isElementSelected(sel, itemId, true))
							selected = true;
						else
							selected = false;
					}
					else
						selected = false;
				}
				  
				if(selected)
					m_cfgtree.SetItemState(node,TVIS_BOLD, TVIS_BOLD); 
				else
					m_cfgtree.SetItemState(node,0, TVIS_BOLD); 
			}
			else
			{
				if(dhelper_ptr->isElementSelected(sel, elemId))
					m_cfgtree.SetItemState(node,TVIS_BOLD, TVIS_BOLD); 
				else
					m_cfgtree.SetItemState(node,0, TVIS_BOLD); 
			}
		}
	}	
		

	int ns = pNMLV->uNewState & LVIS_STATEIMAGEMASK;

	if ((ns & 0x2000) != 0)
		checkedSize++;
	else if ((ns & 0x1000) != 0)
		checkedSize--;

	updateSize();

	*pResult = 0;
}

void CDesertConfigDialog::OnBnClickedClearallbtn()
{
	// TODO: Add your control notification handler code here
	for(map<HTREEITEM, int>::iterator i=cfgTreeMap.begin();i!=cfgTreeMap.end();++i)
	{
		HTREEITEM node = (*i).first;
		m_cfgtree.SetCheck(node,0);
		m_cfgtree.SetItemState(node,0, TVIS_BOLD); 
	}
	HTREEITEM root = m_cfgtree.GetRootItem();
	m_cfgtree.SetCheck(root, 0);

	HDITEM hdItem;
	hdItem.mask = HDI_IMAGE | HDI_FORMAT;
//	VERIFY( m_checkHeadCtrl.GetItem(0, &hdItem) );
	hdItem.iImage = 1;
	hdItem.fmt |= HDF_IMAGE;	
//	VERIFY( m_checkHeadCtrl.SetItem(0, &hdItem) );

	int n=m_cfglist.GetItemCount();
	for(int i=0;i<n;++i)
		m_cfglist.SetCheck(i,0);

	checkedSize = 0;
	updateSize();
}

void CDesertConfigDialog::OnNMCustomdrawConstraintlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
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
		if ( isActiveCfg )
		{
			crText = RGB(0,0,0);
		}
		else
		{
			crText = RGB(190,190, 190);
		}

		// Store the colors back in the NMLVCUSTOMDRAW struct.
		pLVCD->clrText = crText;
	//	pLVCD->clrTextBk = crBkgnd;

		// Tell Windows to paint the control itself.
		*pResult = CDRF_DODEFAULT;
	}
}

void CDesertConfigDialog::checkItem(HTREEITEM &item)
{
	m_cfgtree.SetCheck(item, 1);
	bool isRoot = (item==m_cfgTreeRootItem);
	std::string itemType = dhelper_ptr->getElementType(cfgTreeMap[item]);
	/*map<HTREEITEM, int>::iterator pos = cfgTreeMap.find(item);
	if(pos!=cfgTreeMap.end())
	{
		int itemId = (*pos).second;
		std::string itemType = dhelper_ptr->getElementType(itemId);
	}
	else return;
	*/
	if(itemType.empty() && !isRoot) return;
	
	if(itemType=="Compound" || isRoot)
	{
		if (m_cfgtree.ItemHasChildren(item))
		{
		   HTREEITEM hNextItem;
		   HTREEITEM hChildItem = m_cfgtree.GetChildItem(item);
		   while (hChildItem != NULL)
		   {
			  checkItem(hChildItem);
			  hNextItem = m_cfgtree.GetNextItem(hChildItem, TVGN_NEXT);
			  hChildItem = hNextItem;
		   }
		}
	}
	
//	checkSiblings_check(item);
}

void CDesertConfigDialog::checkSiblings_check(HTREEITEM &item)
{
	if(item==m_cfgTreeRootItem) return;
	HTREEITEM hparentItem = m_cfgtree.GetParentItem(item);
	std::string parentitemType = dhelper_ptr->getElementType(cfgTreeMap[hparentItem]);

	if(parentitemType=="Compound") return;

	HTREEITEM hItem = m_cfgtree.GetPrevSiblingItem(item);
	while (hItem != NULL)
	{
		uncheckItem(hItem);
		hItem = m_cfgtree.GetPrevSiblingItem(hItem);
	}

	hItem = m_cfgtree.GetNextSiblingItem(item);
	while (hItem != NULL)
	{
		uncheckItem(hItem);
		hItem = m_cfgtree.GetNextSiblingItem(hItem);
	}
}

void CDesertConfigDialog::uncheckItem(HTREEITEM &item)
{
	m_cfgtree.SetCheck(item, 0);
	if (m_cfgtree.ItemHasChildren(item))
	{
		HTREEITEM hNextItem;
		HTREEITEM hChildItem = m_cfgtree.GetChildItem(item);
		while (hChildItem != NULL)
		{
			uncheckItem(hChildItem);
			hNextItem = m_cfgtree.GetNextItem(hChildItem, TVGN_NEXT);
			hChildItem = hNextItem;
		}
	}
}

void CDesertConfigDialog::updateConfigList(HTREEITEM &item, set<int> &cfgIds, int &cnt, bool isAlt)
{
	//update from the current Item here:
	if(m_cfgtree.GetCheck(item))
	{
		noneChecked = true;
		if(item != m_cfgTreeRootItem)
		{
			set<int> curr_ids = dhelper_ptr->getConfigurationsfromElement(cfgTreeMap[item]);
			if(!isAlt || (isAlt && cnt==0))
			{
				set<int> tmp_ids;
				for(set<int>::iterator it=cfgIds.begin();it!=cfgIds.end();++it)
				{
					if(curr_ids.find((*it))!=curr_ids.end())
						tmp_ids.insert((*it));
				}
				cfgIds = tmp_ids;
			}
			else
				cfgIds.insert(curr_ids.begin(), curr_ids.end());
		}
		cnt++;
	}

	//recursive update here:
	if(!m_cfgtree.ItemHasChildren(item)) return;

	std::string itemType = dhelper_ptr->getElementType(cfgTreeMap[item]);
	HTREEITEM hNextItem;
	HTREEITEM hChildItem = m_cfgtree.GetChildItem(item);
	int itemCnt = 0;
	while (hChildItem != NULL)
	{
		if(itemType=="Alternative" || itemType=="Optional")
			updateConfigList(hChildItem, cfgIds, itemCnt, true);
		else
			updateConfigList(hChildItem, cfgIds, itemCnt);
		hNextItem = m_cfgtree.GetNextItem(hChildItem, TVGN_NEXT);
		hChildItem = hNextItem;
	}
}

set<int> CDesertConfigDialog::computeConfigList(HTREEITEM &item, set<int> cfgIds)
{
	set<int> ret_ids;
	if(m_cfgtree.GetCheck(item) && item!=m_cfgTreeRootItem)
	{
		noneChecked = true;
		set<int> curr_ids = dhelper_ptr->getConfigurationsfromElement(cfgTreeMap[item]);

		for(set<int>::iterator it=curr_ids.begin();it!=curr_ids.end();++it)
		{
			if(cfgIds.find((*it))!=cfgIds.end())
				ret_ids.insert((*it));
		}
	}
	else
		ret_ids = cfgIds;

	if(!m_cfgtree.ItemHasChildren(item)) return ret_ids;
		
	std::string itemType = dhelper_ptr->getElementType(cfgTreeMap[item]);
	HTREEITEM hNextItem;
	HTREEITEM hChildItem = m_cfgtree.GetChildItem(item);
	if(itemType=="Alternative" || itemType=="Optional")
	{
		set<int> tmp_ids = ret_ids;
		ret_ids.clear();
		bool changed = false;
		while (hChildItem != NULL)
		{
			set<int> child_ids = computeConfigList(hChildItem, tmp_ids);
			if(child_ids!=tmp_ids || m_cfgtree.GetCheck(hChildItem))
			{
				ret_ids.insert(child_ids.begin(), child_ids.end());
				changed = true;
			}
			hNextItem = m_cfgtree.GetNextItem(hChildItem, TVGN_NEXT);
			hChildItem = hNextItem;
		}
		if(!changed)
			ret_ids = tmp_ids;
	}
	else
	{
		while (hChildItem != NULL)
		{
			ret_ids = computeConfigList(hChildItem, ret_ids);
			hNextItem = m_cfgtree.GetNextItem(hChildItem, TVGN_NEXT);
			hChildItem = hNextItem;
		}
	}
		
	return ret_ids;
}

void CDesertConfigDialog::updateConfigList(set<int> &cfgIds, bool check)
{
	int cnt = m_cfglist.GetItemCount();
	bool isclear = true;
	for(int i=0;i<cnt;++i)
	{
		if(m_cfglist.GetCheck(i))
		{
			isclear = false;
			break;
		}
	}
	for(int i=0;i<cnt;++i)
	{
		//get the real row position
		for(int j=0; j<cnt;++j)
		{
			if(m_cfglist.GetItemID(j)==i)
			{
				if(cfgIds.find(i)!=cfgIds.end())
				{
					if(check)
						m_cfglist.SetCheck(j,1);
					else
					if(isclear)
						m_cfglist.SetCheck(j,1);
					else
						if(m_cfglist.GetCheck(j))
							m_cfglist.SetCheck(j,1);
						else
							m_cfglist.SetCheck(j,0);
				}
				else
					m_cfglist.SetCheck(j,0);
			}
		}
	}
}

//void CDesertConfigDialog::updateConfigList_1()
//{
//	set<int> checkedCfgIds;
//	bool first = true;
//	for(map<HTREEITEM, int>::iterator pos=cfgTreeMap.begin();pos!=cfgTreeMap.end();++pos)
//	{
//		HTREEITEM node = (*pos).first;
//		if(!m_cfgtree.GetCheck(node)) continue;
//		set<int> cfgids = dhelper_ptr->getConfigurationsfromElement(cfgTreeMap[node]);
//		if(first)
//		{
//			checkedCfgIds = cfgids;
//			first = false;
//		}
//		else
//		{
//			set<int> tmp;
//			for(set<int>::iterator it=checkedCfgIds.begin();it!=checkedCfgIds.end();++it)
//			{
//				if(cfgids.find((*it))!=cfgids.end())
//					tmp.insert((*it));
//			}
//			checkedCfgIds = tmp;
//		}
//	}
//	
//	int cnt = m_cfglist.GetItemCount();
//	for(int i=0;i<cnt;++i)
//	{
//		if(checkedCfgIds.find(i)!=checkedCfgIds.end())
//			m_cfglist.SetCheck(i,1);
//		else
//			m_cfglist.SetCheck(i,0);
//	}
//}

void CDesertConfigDialog::OnBnClickedCyPhy2MorphMatrix()
{
	set<int> ids;
	int cnt = m_cfglist.GetItemCount();
	for(int i=0; i<cnt; ++i)
	{
		if(m_cfglist.GetCheck(i)) {
			ids.insert(m_cfglist.GetItemID(i));
		}
	}
	dhelper_ptr->exportModel_ForMorphMatrix(ids);

	IDispatch *exceldispatch = NULL;

	try {

		// CXLAutomation XL;
		// XL.OpenExcelFile(mmFileDlg.m_filepath);

		//CCyPhy2MorphMatrixFileDlg mmFileDlg;
		
		//if(mmFileDlg.DoModal()==IDOK)
		//{
			::CoInitialize(NULL);

			if( exceldispatch == NULL)
			{
				CLSID clsid;
				CLSIDFromProgID(L"Excel.Application", &clsid);

				HRESULT hRes = CoCreateInstance(clsid, NULL, CLSCTX_LOCAL_SERVER, 
								   IID_IDispatch, (void**)&exceldispatch);
				if(FAILED(hRes))
				{
					MessageBox(_T("Failed to create Excel Application Object"));
					return;
				}

				DISPID dispidVisible;
				OLECHAR *propertyName = L"Visible";
				hRes = exceldispatch->GetIDsOfNames(IID_NULL, &propertyName, 1, LOCALE_SYSTEM_DEFAULT, &dispidVisible);
         
				if(FAILED(hRes))
				{
					MessageBox(_T("Failed to access Excel Application Methods"));
					return;
				}
         
				unsigned returnArg;
				VARIANT varTrue;
				DISPID rgDispidNamedArgs[1];
				rgDispidNamedArgs[0] = DISPID_PROPERTYPUT;
				DISPPARAMS params = { &varTrue, rgDispidNamedArgs, 1, 1 };

				varTrue.vt = VT_BOOL;
				varTrue.boolVal = VARIANT_TRUE;
         
				hRes = exceldispatch->Invoke(dispidVisible, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYPUT, &params, 0, 0, &returnArg);
         
				if(FAILED(hRes))
				{
					MessageBox(_T("Failed to access Excel Application Methods"));
					exceldispatch->Release();
					return;
				}

 			}
		//}
	} catch (...) {
		// TODO: Show error dialog
		exceldispatch->Release();
	}
}

void CDesertConfigDialog::OnBnClickedMorphMatrix2CyPhy()
{
	CMophyMatrixFileDlg mmFileDlg;
	set<long> cfg_ids;
	if(mmFileDlg.DoModal()==IDOK)
	{
		ifstream mmFile;
		mmFile.open(mmFileDlg.m_filepath);
		char id[100];
		if (mmFile.is_open()) 
		{
			while (!mmFile.eof()) 
			{
				mmFile >> id;
				cfg_ids.insert(atol(id));
			}
		}
		mmFile.close();

		long cfgSize = dhelper_ptr->getRealConfigCount();
		for(int i=0; i<cfgSize; ++i)
		{
			m_cfglist.SetCheck(i, 0);
			std::string id;
			if(dhelper_ptr->getConfigurationUniqueId(i, id))
			{
				long configurationId = atol(id.c_str());
				set<long>::iterator itCfg = cfg_ids.find(configurationId);
				if(itCfg != cfg_ids.end()) {
					m_cfglist.SetCheck(i, 1);
				}
			}
		}
	}
}

void CDesertConfigDialog::updateSize()
{
	TCHAR cnt_buff[32];
	_itot_s(checkedSize, cnt_buff, 10);
	CString info = _T("Total Configurations: \t");
	info += utf82cstring(cfgSizeInfo) + _T("\r\nChecked Configurations: \t") + cnt_buff;
	m_cfgsize.SetWindowText(info);
}


void CDesertConfigDialog::OnNMDblclkCfglist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here

	int n = pNMItemActivate->iItem;
	if(n>=0)
	{
		CEdit* pEdit =m_cfglist.EditLabel(n); 
		ASSERT(pEdit != NULL); 
	}
	*pResult = 0;
}


void CDesertConfigDialog::OnLvnBeginlabeleditCfglist(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CDesertConfigDialog::OnLvnEndlabeleditCfglist(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: Add your control notification handler code here
	 if(NULL != pDispInfo->item.pszText)
   {
      //... let's put it in the list! 
		// AfxMessageBox(CString("In OnLvnEndlabeleditCfglist, pszText is NOT null"));
	   m_cfglist.SetItemText(pDispInfo->item.iItem, 2, pDispInfo->item.pszText);
	   dhelper_ptr->updateCfgId2NameMap(pDispInfo->item.iItem+1, tstring2utf8(pDispInfo->item.pszText));
   } else {
	   // AfxMessageBox(CString("In OnLvnEndlabeleditCfglist, pszText is NULL"));
	   // m_cfglist.SetItemText(pDispInfo->item.iItem, 2, pDispInfo->item.pszText);
	   // dhelper_ptr->updateCfgId2NameMap(pDispInfo->item.iItem+1, pDispInfo->item.pszText);
	 }

	*pResult = 0;
}




void CDesertConfigDialog::OnNMClkCfgList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// AfxMessageBox(CString("OnNMClkCfgList called"));
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here

	int n = pNMItemActivate->iItem;
	std::stringstream msgSS;
	msgSS << "n = " << n;
	std::string msgS = msgSS.str();
	CString cstringMsg(msgS.c_str());
	// AfxMessageBox(cstringMsg);
	if(n>=0)
	{
		CEdit* pEdit = m_cfglist.EditLabel(n);
		if(pEdit == NULL) {
			// AfxMessageBox("pEdit = NULL");
		}
		else {
			// AfxMessageBox("pEdit is NOT null");
		}
		ASSERT(pEdit != NULL); 
	}
	*pResult = 0;
}


void CDesertConfigDialog::OnBnClickedCloseallbtn()
{
	// TODO: Add your control notification handler code here
	EndDialog(100);
	return;
}
