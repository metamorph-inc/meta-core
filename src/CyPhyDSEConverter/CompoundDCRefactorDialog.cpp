// CompoundDCRefactorDialog.cpp : implementation file
//

#include "stdafx.h"
#include "CompoundDCRefactorDialog.h"
#include "afxdialogex.h"


// CCompoundDCRefactorDialog dialog

IMPLEMENT_DYNAMIC(CCompoundDCRefactorDialog, CDialogEx)

CCompoundDCRefactorDialog::CCompoundDCRefactorDialog(CWnd* pParent /*=NULL*/)
	: _disableCA(false), CDialogEx(CCompoundDCRefactorDialog::IDD, pParent)
{

}

CCompoundDCRefactorDialog::CCompoundDCRefactorDialog(bool disableCA, CWnd* pParent /*=NULL*/)
	: _disableCA(disableCA), CDialogEx(CCompoundDCRefactorDialog::IDD, pParent)
{

}

CCompoundDCRefactorDialog::~CCompoundDCRefactorDialog()
{
}

void CCompoundDCRefactorDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCompoundDCRefactorDialog, CDialogEx)
	ON_BN_CLICKED(IDC_CA_RADIO, &CCompoundDCRefactorDialog::OnBnClickedCaRadio)
	ON_BN_CLICKED(IDC_DCRADIO, &CCompoundDCRefactorDialog::OnBnClickedDcradio)
	ON_BN_CLICKED(IDC_EXRADIO, &CCompoundDCRefactorDialog::OnBnClickedExradio)
END_MESSAGE_MAP()


// CCompoundDCRefactorDialog message handlers


void CCompoundDCRefactorDialog::OnBnClickedCaRadio()
{
	// TODO: Add your control notification handler code here
	rt = genCA;
}


void CCompoundDCRefactorDialog::OnBnClickedDcradio()
{
	// TODO: Add your control notification handler code here
	rt = genDC;
}


void CCompoundDCRefactorDialog::OnBnClickedExradio()
{
	// TODO: Add your control notification handler code here
	rt = extract;
}


BOOL CCompoundDCRefactorDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	if(_disableCA)
		GetDlgItem(IDC_CA_RADIO)->EnableWindow(FALSE); 
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
