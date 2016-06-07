// DSESelectorDialog.cpp : implementation file
//

#include "StdAfx.h"
#include "DSESelectorDialog.h"
#include <afxdialogex.h>


// CDSESelectorDialog dialog

IMPLEMENT_DYNAMIC(CDSESelectorDialog, CDialogEx)

CDSESelectorDialog::CDSESelectorDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDSESelectorDialog::IDD, pParent)
	, dhelper_ptr(0)
{

}

CDSESelectorDialog::CDSESelectorDialog(DesertHelper *deserthelper_ptr, CWnd* pParent)
	: CDialogEx(CDSESelectorDialog::IDD, pParent)
	, dhelper_ptr(deserthelper_ptr)
{
	dhelper_ptr->clearCfgId2NameMap();
}

CDSESelectorDialog::~CDSESelectorDialog()
{
}

void CDSESelectorDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DSETREE, m_dsetree);
}


BEGIN_MESSAGE_MAP(CDSESelectorDialog, CDialogEx)
//	ON_NOTIFY(NM_CLICK, IDC_DSETREE, &CDSESelectorDialog::OnNMClickDsetree)
END_MESSAGE_MAP()


// CDSESelectorDialog message handlers


BOOL CDSESelectorDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	FillCfgTree();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDSESelectorDialog::FillCfgTree()
{
	m_dsetree.DeleteAllItems();
	DesertIface::Space space;
	dhelper_ptr->getDesertIfaceSpace(space);

	std::string iname = space.name();
	TVINSERTSTRUCT tvInsert;
	tvInsert.hParent = NULL;
	tvInsert.hInsertAfter = NULL;
	tvInsert.item.mask = TVIF_TEXT;
	tvInsert.item.pszText = _T("DesignSpace");
	m_dseTreeRootItem = m_dsetree.InsertItem(&tvInsert);
	
	set<DesertIface::Element> elems = space.Element_kind_children();
	for(set<DesertIface::Element>::iterator i=elems.begin();i!=elems.end();++i)
	{
		iname = (*i).name();
		HTREEITEM elem_node = m_dsetree.InsertItem(utf82cstring(iname), m_dseTreeRootItem, TVI_LAST);
		FillCfgTree(DesertIface::Element(*i), elem_node);
		m_dsetree.SortChildren(elem_node);
		m_dsetree.Expand(elem_node,TVE_EXPAND);
		dseTreeMap[elem_node]=(*i).externalID();
	}
	m_dsetree.SortChildren(m_dseTreeRootItem);
	m_dsetree.Expand(m_dseTreeRootItem, TVE_EXPAND);
}

void CDSESelectorDialog::FillCfgTree(DesertIface::Element &elem, HTREEITEM parent)
{
	set<DesertIface::Element> elems = elem.Element_kind_children();
	for(set<DesertIface::Element>::iterator i=elems.begin();i!=elems.end();++i)
	{
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
		}
		HTREEITEM elem_node = m_dsetree.InsertItem(utf82cstring(iname.c_str()), parent, TVI_LAST);
		FillCfgTree(DesertIface::Element(*i),elem_node);
		m_dsetree.SortChildren(elem_node);
	//	m_dsetree.Expand(elem_node,TVE_EXPAND);
		dseTreeMap[elem_node]=(*i).externalID();
	}
}

set<int> CDSESelectorDialog::getSelectedElems()
{
	return selectedElems;
}

void CDSESelectorDialog::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	for(map<HTREEITEM, int>::iterator it=dseTreeMap.begin();it!=dseTreeMap.end();++it)
	{
		if(m_dsetree.GetCheck((*it).first))
			selectedElems.insert((*it).second);
	}
	CDialogEx::OnOK();
}
