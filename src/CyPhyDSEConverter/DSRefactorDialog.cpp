// DSRefactorDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DSRefactorDialog.h"
#include "afxdialogex.h"


// CDSRefactorDialog dialog

IMPLEMENT_DYNAMIC(CDSRefactorDialog, CDialogEx)

CDSRefactorDialog::CDSRefactorDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDSRefactorDialog::IDD, pParent)
{
	m_genCA = false;
}

CDSRefactorDialog::~CDSRefactorDialog()
{
}

void CDSRefactorDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDSRefactorDialog, CDialogEx)
	ON_BN_CLICKED(IDC_COMMAND_DESIGNCONTAINER, &CDSRefactorDialog::OnBnClickedDcRadio)
	ON_BN_CLICKED(IDC_COMMAND_COMPONENTASSEMBLY, &CDSRefactorDialog::OnBnClickedCaRadio)
END_MESSAGE_MAP()


// CDSRefactorDialog message handlers


void CDSRefactorDialog::OnBnClickedDcRadio()
{
	m_genCA = false;
	this->EndDialog(IDOK);
}


void CDSRefactorDialog::OnBnClickedCaRadio()
{
	m_genCA = true;
	this->EndDialog(IDOK);
}


BOOL CDSRefactorDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
