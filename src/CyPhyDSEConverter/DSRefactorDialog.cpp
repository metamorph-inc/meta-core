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
	ON_BN_CLICKED(IDC_DC_RADIO, &CDSRefactorDialog::OnBnClickedDcRadio)
	ON_BN_CLICKED(IDC_CA_RADIO, &CDSRefactorDialog::OnBnClickedCaRadio)
END_MESSAGE_MAP()


// CDSRefactorDialog message handlers


void CDSRefactorDialog::OnBnClickedDcRadio()
{
	// TODO: Add your control notification handler code here
	m_genCA = false;
}


void CDSRefactorDialog::OnBnClickedCaRadio()
{
	// TODO: Add your control notification handler code here
	m_genCA = true;
}


BOOL CDSRefactorDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	CButton *pRbtn1 = (CButton*)this->GetDlgItem(IDC_DC_RADIO);
	pRbtn1->SetFocus();      
    pRbtn1->SetCheck(true); 
	m_genCA = false;

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
