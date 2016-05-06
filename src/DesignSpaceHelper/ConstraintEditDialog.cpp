// ConstraintEditDialog.cpp : implementation file
//

#include "StdAfx.h"
#include "ConstraintEditDialog.h"


// CConstraintEditDialog dialog

IMPLEMENT_DYNAMIC(CConstraintEditDialog, CDialog)

CConstraintEditDialog::CConstraintEditDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CConstraintEditDialog::IDD, pParent)
	, m_name(_T(""))
	, m_expression(_T("constraint c(){\r\n\r\n}"))
	, m_editExpression(false)
	, dhelper_ptr(0)
{

}

CConstraintEditDialog::CConstraintEditDialog(CString c_name, CString c_expr, CWnd* pParent /*=NULL*/)
	: CDialog(CConstraintEditDialog::IDD, pParent)
	, m_name(c_name)
	, m_expression(c_expr)
	, m_editExpression(false)
	, dhelper_ptr(0)
{

}

CConstraintEditDialog::CConstraintEditDialog(CString c_name, CString c_expr, DesertHelper *deserthelper_ptr, CWnd* pParent /*=NULL*/)
	: CDialog(CConstraintEditDialog::IDD, pParent)
	, m_name(c_name)
	, m_expression(c_expr)
	, m_editExpression(false)
	, dhelper_ptr(deserthelper_ptr)
{

}
CConstraintEditDialog::~CConstraintEditDialog()
{
}

void CConstraintEditDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NAMEEDIT, m_name);
	DDX_Text(pDX, IDC_EXPRESSIONEDIT, m_expression);
	DDX_Control(pDX, IDC_FUNCTIONLIST, m_funclist);
	DDX_Control(pDX, IDC_EXPRESSIONEDIT, c_expredit);
	DDX_Control(pDX, IDC_NAMEEDIT, c_nameedit);
}


BEGIN_MESSAGE_MAP(CConstraintEditDialog, CDialog)
	ON_BN_CLICKED(IDC_INSERTBTN, &CConstraintEditDialog::OnBnClickedInsertbtn)
	ON_EN_CHANGE(IDC_EXPRESSIONEDIT, &CConstraintEditDialog::OnEnChangeExpressionedit)
//	ON_LBN_SELCHANGE(IDC_FUNCTIONLIST, &CConstraintEditDialog::OnLbnSelchangeFunctionlist)
ON_LBN_DBLCLK(IDC_FUNCTIONLIST, &CConstraintEditDialog::OnLbnDblclkFunctionlist)
ON_EN_SETFOCUS(IDC_EXPRESSIONEDIT, &CConstraintEditDialog::OnEnSetfocusExpressionedit)
ON_EN_SETFOCUS(IDC_NAMEEDIT, &CConstraintEditDialog::OnEnSetfocusNameedit)
ON_BN_CLICKED(IDOK, &CConstraintEditDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CConstraintEditDialog message handlers

void CConstraintEditDialog::OnBnClickedInsertbtn()
{
	// TODO: Add your control notification handler code here
	insertFuncString();
}

BOOL CConstraintEditDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_funclist.AddString(_T("self"));
	m_funclist.AddString(_T("children"));
	m_funclist.AddString(_T("implementedBy"));
	m_funclist.AddString(_T("parent"));
	m_funclist.AddString(_T("project"));
	if(!dhelper_ptr) return TRUE;
	set<std::string> nparams = dhelper_ptr->getNaturalParameters();
	for(set<std::string>::iterator ni=nparams.begin();ni!=nparams.end();++ni)
	{
		m_funclist.AddString(utf82cstring((*ni).c_str()));
	}
	set<std::string> cparams = dhelper_ptr->getCustomParameters();
	for(set<std::string>::iterator ci=cparams.begin();ci!=cparams.end();++ci)
	{
		m_funclist.AddString(utf82cstring((*ci).c_str()));
		tstring param = _T("project().") + utf82cstring(*ci) + _T("s");
		m_funclist.AddString(param.c_str());
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CConstraintEditDialog::OnEnChangeExpressionedit()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

//void CConstraintEditDialog::OnLbnSelchangeFunctionlist()
//{
//	// TODO: Add your control notification handler code here
//}

void CConstraintEditDialog::OnLbnDblclkFunctionlist()
{
	// TODO: Add your control notification handler code here
	insertFuncString();
}

void CConstraintEditDialog::insertFuncString()
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
	if(selfun==_T("children") || selfun==_T("project().Materials"))
	{
		c_expredit.ReplaceSel(_T("(\"\")"));
	}
	c_expredit.SetFocus();
}

void CConstraintEditDialog::OnEnSetfocusExpressionedit()
{
	// TODO: Add your control notification handler code here
	m_editExpression = true;
}

void CConstraintEditDialog::OnEnSetfocusNameedit()
{
	// TODO: Add your control notification handler code here
	m_editExpression = false;
}

void CConstraintEditDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_NAMEEDIT)->GetWindowText(m_name);
	if(m_name.IsEmpty())
	{
		MessageBox(_T("Please type in constraint name."), _T("Constraint Name Error"), MB_ICONSTOP);
		return;			
	}

	OnOK();
}
