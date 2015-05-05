// CARefactorDialog.cpp : implementation file
//

#include "stdafx.h"
#include "CARefactorDialog.h"
#include "afxdialogex.h"


// CCARefactorDialog dialog

IMPLEMENT_DYNAMIC(CCARefactorDialog, CDialogEx)

CCARefactorDialog::CCARefactorDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCARefactorDialog::IDD, pParent)
{

}

CCARefactorDialog::~CCARefactorDialog()
{
}

void CCARefactorDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCARefactorDialog, CDialogEx)
	ON_BN_CLICKED(IDC_EXRADIO, &CCARefactorDialog::OnBnClickedExradio)
	ON_BN_CLICKED(IDC_DCRADIO, &CCARefactorDialog::OnBnClickedDcradio)
	ON_BN_CLICKED(IDC_CA_RADIO, &CCARefactorDialog::OnBnClickedCaRadio)
END_MESSAGE_MAP()


// CCARefactorDialog message handlers


void CCARefactorDialog::OnBnClickedExradio()
{
	// TODO: Add your control notification handler code here
	rt = extract;
}


void CCARefactorDialog::OnBnClickedDcradio()
{
	// TODO: Add your control notification handler code here
	rt = genDC;
}


void CCARefactorDialog::OnBnClickedCaRadio()
{
	// TODO: Add your control notification handler code here
	rt = genCA;
}


BOOL CCARefactorDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	CButton *pRbtn1 = (CButton*)this->GetDlgItem(IDC_EXRADIO);
	pRbtn1->SetFocus();      
    pRbtn1->SetCheck(true); 
	rt = extract;
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
