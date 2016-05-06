// DesignSpaceSelDialog.cpp : implementation file
//

#include "StdAfx.h"
#include "DesignSpaceSelDialog.h"
#include <afxdialogex.h>


// CDesignSpaceSelDialog dialog

IMPLEMENT_DYNAMIC(CDesignSpaceSelDialog, CDialogEx)

CDesignSpaceSelDialog::CDesignSpaceSelDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDesignSpaceSelDialog::IDD, pParent)
{

}

//CDesignSpaceSelDialog::CDesignSpaceSelDialog(const set<CyPhyML::DesignSpace> &dss, CWnd* pParent /*=NULL*/)
//	: CDialogEx(CDesignSpaceSelDialog::IDD, pParent) 
//{
//	int cnt = 0;
//	for(set<CyPhyML::DesignSpace>::const_iterator i=dss.begin();i!=dss.end();++i)
//	{
//		allDSs[cnt] = *i;
//		cnt++;
//	}
//	sel = dss.size();
//}

CDesignSpaceSelDialog::~CDesignSpaceSelDialog()
{
}

void CDesignSpaceSelDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DSLIST, m_dslist);
}


BEGIN_MESSAGE_MAP(CDesignSpaceSelDialog, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DSLIST, &CDesignSpaceSelDialog::OnLvnItemchangedDslist)
	ON_NOTIFY(LVN_ITEMCHANGING, IDC_DSLIST, &CDesignSpaceSelDialog::OnLvnItemchangingDslist)
END_MESSAGE_MAP()


// CDesignSpaceSelDialog message handlers


BOOL CDesignSpaceSelDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	ListView_SetExtendedListViewStyle(m_dslist.m_hWnd, LVS_EX_CHECKBOXES);
	LV_ITEM item;
	for(map<int, CyPhyML::DesignSpace>::iterator i=allDSs.begin();i!=allDSs.end();++i)
	{
		std::string name = ((*i).second).name();		
		item.iItem = (*i).first;
		item.iSubItem = 0;
		item.mask = LVIF_TEXT;
		item.iImage = LVS_EX_CHECKBOXES ;
		
		item.pszText =(LPSTR)name.c_str();			
		m_dslist.InsertItem(&item);
	}

	item.iItem = allDSs.size();
	item.iSubItem = 0;
	item.mask = LVIF_TEXT;
	item.iImage = LVS_EX_CHECKBOXES ;		
	item.pszText ="all of above";
	m_dslist.InsertItem(&item);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDesignSpaceSelDialog::OnLvnItemchangedDslist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	if(pNMLV->uChanged & LVIF_STATE) // item state has been changed
   {
      switch(pNMLV->uNewState & LVIS_STATEIMAGEMASK)
	  {
	  case INDEXTOSTATEIMAGEMASK(BST_CHECKED + 1): // new state: checked
		{
			sel = pNMLV->iItem;
           // TRACE1("\n Item %d has been checked", pNMLV->iItem);
	//		AfxMessageBox(m_dslist.GetItemText(sel,0));
//			ListView_SetCheckState(GetSafeHwnd(),1,1,LVIS_STATEIMAGEMASK);
			break;
		}
      }
   }

	*pResult = 0;
}


void CDesignSpaceSelDialog::OnLvnItemchangingDslist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here

	*pResult = 0;
}

void CDesignSpaceSelDialog::GetSelectedDesignSpace(CyPhyML::DesignSpace &ds)
{
	map<int, CyPhyML::DesignSpace>::iterator pos = allDSs.find(sel);
	if(pos!=allDSs.end())
		ds = (*pos).second;
}