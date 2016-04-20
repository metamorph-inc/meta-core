// ConstraintAddDialog.cpp : implementation file
//

#include "StdAfx.h"
#include "ConstraintAddDialog.h"


// CConstraintAddDialog dialog

IMPLEMENT_DYNAMIC(CConstraintAddDialog, CDialog)

CConstraintAddDialog::CConstraintAddDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CConstraintAddDialog::IDD, pParent)
	, m_name(_T(""))
	, m_expression(_T("constraint c(){\r\n\r\n}"))
	, m_editExpression(false)
	, dhelper_ptr(0)
	, m_cyphyObjId(0)
{

}

CConstraintAddDialog::CConstraintAddDialog(DesertHelper *deserthelper_ptr, CWnd* pParent /*=NULL*/)
	: CDialog(CConstraintAddDialog::IDD, pParent)
	, m_name(_T(""))
	, m_expression(_T("constraint c(){\r\n\r\n}"))
	, m_editExpression(false)
	, dhelper_ptr(deserthelper_ptr)
	, m_cyphyObjId(0)
{

}

CConstraintAddDialog::~CConstraintAddDialog()
{

}

void CConstraintAddDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NAMEEDIT, m_name);
	DDX_Text(pDX, IDC_EXPRESSIONEDIT, m_expression);
	DDX_Control(pDX, IDC_FUNCTIONLIST, m_funclist);
	DDX_Control(pDX, IDC_EXPRESSIONEDIT, c_expredit);
	DDX_Control(pDX, IDC_NAMEEDIT, c_nameedit);
	DDX_Control(pDX, IDC_SPACETREE, m_spacetree);
}


BEGIN_MESSAGE_MAP(CConstraintAddDialog, CDialog)
	ON_BN_CLICKED(IDC_INSERTBTN, &CConstraintAddDialog::OnBnClickedInsertbtn)
	ON_EN_CHANGE(IDC_EXPRESSIONEDIT, &CConstraintAddDialog::OnEnChangeExpressionedit)
//	ON_LBN_SELCHANGE(IDC_FUNCTIONLIST, &CConstraintAddDialog::OnLbnSelchangeFunctionlist)
ON_LBN_DBLCLK(IDC_FUNCTIONLIST, &CConstraintAddDialog::OnLbnDblclkFunctionlist)
ON_EN_SETFOCUS(IDC_EXPRESSIONEDIT, &CConstraintAddDialog::OnEnSetfocusExpressionedit)
ON_EN_SETFOCUS(IDC_NAMEEDIT, &CConstraintAddDialog::OnEnSetfocusNameedit)
ON_NOTIFY(TVN_SELCHANGED, IDC_SPACETREE, &CConstraintAddDialog::OnTvnSelchangedSpacetree)
ON_BN_CLICKED(IDOK, &CConstraintAddDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CConstraintAddDialog message handlers

void CConstraintAddDialog::OnBnClickedInsertbtn()
{
	// TODO: Add your control notification handler code here
	InsertFuncString();
}

BOOL CConstraintAddDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_funclist.AddString(_T("self"));
	m_funclist.AddString(_T("children"));
	m_funclist.AddString(_T("implementedBy"));
	m_funclist.AddString(_T("parent"));
	m_funclist.AddString(_T("project"));
	m_funclist.AddString(_T("Weight"));
	m_funclist.AddString(_T("Volume"));
	m_funclist.AddString(_T("Cost"));
	m_funclist.AddString(_T("Input_Power"));
	m_funclist.AddString(_T("Output_Power"));
	m_funclist.AddString(_T("Range"));
	m_funclist.AddString(_T("V_Cruise"));
	m_funclist.AddString(_T("Material"));
	m_funclist.AddString(_T("project().Materials"));
	
	FillDesignSpaceTree();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CConstraintAddDialog::OnEnChangeExpressionedit()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CConstraintAddDialog::OnLbnDblclkFunctionlist()
{
	// TODO: Add your control notification handler code here
	InsertFuncString();
}

void CConstraintAddDialog::InsertFuncString()
{
	if(!m_editExpression) {
		return;
	}

	CPoint caret = GetCaretPos();
	int curpos = c_expredit.CharFromPos(caret);

	CString selfun;
	int cursel = m_funclist.GetCurSel();
	int len = m_funclist.GetTextLen(cursel);
    m_funclist.GetText(cursel, selfun.GetBuffer(len+2) );
	c_expredit.ReplaceSel(selfun);
	if(selfun!=_T("self") && selfun!=_T("children"))
		c_expredit.ReplaceSel(_T("()"));
	if(selfun==_T("children")|| selfun==_T("project().Materials"))
	{
		c_expredit.ReplaceSel(_T("(\"\")"));
	}
	c_expredit.SetFocus();
}

void CConstraintAddDialog::OnEnSetfocusExpressionedit()
{
	// TODO: Add your control notification handler code here
	m_editExpression = true;
}

void CConstraintAddDialog::OnEnSetfocusNameedit()
{
	// TODO: Add your control notification handler code here
	m_editExpression = false;
}

void CConstraintAddDialog::FillDesignSpaceTree()
{
	m_spacetree.DeleteAllItems();

	ListView_SetExtendedListViewStyle(m_spacetree.m_hWnd, TVS_SHOWSELALWAYS);

	DesertIface::Space space;
	dhelper_ptr->getDesertIfaceSpace(space);

	std::string iname = space.name();
	TVINSERTSTRUCT tvInsert;
	tvInsert.hParent = NULL;
	tvInsert.hInsertAfter = NULL;
	tvInsert.item.mask = TVIF_TEXT;
	tvInsert.item.pszText = _T("DesignSpace");
	HTREEITEM tree_root = m_spacetree.InsertItem(&tvInsert);
	
	set<DesertIface::Element> elems = space.Element_kind_children();
	for(set<DesertIface::Element>::iterator i=elems.begin();i!=elems.end();++i)
	{
		iname = (*i).name();
		std::string elemType = dhelper_ptr->getCyPhyObjType((*i).externalID());
		if(elemType=="Compound" || elemType=="Alternative" || elemType=="Optional")
			iname.append(" ["+elemType+"]");

		HTREEITEM elem_node = m_spacetree.InsertItem(utf82cstring(iname), tree_root, TVI_LAST);
		spaceTreeMap[elem_node]=(*i).externalID();
		if(dhelper_ptr->isFocusObject((*i).externalID()))
		{		
			m_spacetree.SelectItem(elem_node);
			m_spacetree.Expand(elem_node,TVE_EXPAND);
		}
		
		FillDesignSpaceTree(DesertIface::Element(*i), elem_node);
		
		m_spacetree.SortChildren(elem_node);
	}
	m_spacetree.SortChildren(tree_root);
	m_spacetree.Expand(tree_root, TVE_EXPAND);
}

void CConstraintAddDialog::FillDesignSpaceTree(DesertIface::Element &elem, HTREEITEM parent)
{
	set<DesertIface::Element> elems = elem.Element_kind_children();
	for(set<DesertIface::Element>::iterator i=elems.begin();i!=elems.end();++i)
	{
		std::string iname = (*i).name();
		std::string elemType = dhelper_ptr->getCyPhyObjType((*i).externalID());
		if(elemType=="Mandatory" || elemType=="Alternative" || elemType=="Optional")
			iname.append(" ["+elemType+"]");
		HTREEITEM elem_node = m_spacetree.InsertItem(utf82cstring(iname),parent, TVI_LAST);
		spaceTreeMap[elem_node]=(*i).externalID();
		if(dhelper_ptr->isFocusObject((*i).externalID()))
		{
			m_spacetree.SelectItem(elem_node);
			m_spacetree.Expand(elem_node,TVE_EXPAND);
		}
		
		FillDesignSpaceTree(DesertIface::Element(*i),elem_node);
		
		m_spacetree.SortChildren(elem_node);
	}
}
void CConstraintAddDialog::OnTvnSelchangedSpacetree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	HTREEITEM elem_node = m_spacetree.GetSelectedItem();
	m_spacetree.Select( elem_node, TVGN_CARET); 				
	m_spacetree.SelectItem(elem_node);
	m_spacetree.SelectDropTarget( elem_node ); 

	*pResult = 0;
}

void CConstraintAddDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	HTREEITEM hItem = m_spacetree.GetSelectedItem();
	map<HTREEITEM, int>::iterator pos = spaceTreeMap.find(hItem);
	if(pos!=spaceTreeMap.end())
	{
		std::string fobjtype = dhelper_ptr->getCyPhyObjType((*pos).second);			
		if(fobjtype == "NullObj")
		{
			MessageBox(_T("The selected object is null. Please seelct a Compound/Alternative/Optional component."), _T("Constraint Context Error"), MB_ICONSTOP);
			return;
		}
		if(fobjtype != "Mandatory" && fobjtype != "Alternative" && fobjtype != "Optional")
		{
			MessageBox(_T("Constraint can only be added into a Mandatory/Alternative/Optional component."), _T("Constraint Context Error"), MB_ICONSTOP);
			return;
		}
	}
	m_cyphyObjId = (*pos).second;

	GetDlgItem(IDC_NAMEEDIT)->GetWindowText(m_name);

	if(m_name.IsEmpty())
	{
		MessageBox(_T("Please type in constraint name."), _T("Constraint Name Error"), MB_ICONSTOP);
		return;			
	}

	OnOK();
}
