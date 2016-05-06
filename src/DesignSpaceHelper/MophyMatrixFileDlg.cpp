// MophyMatrixFileDlg.cpp : implementation file
//

#include "StdAfx.h"
#include "MophyMatrixFileDlg.h"
#include <afxdialogex.h>


// CMophyMatrixFileDlg dialog

IMPLEMENT_DYNAMIC(CMophyMatrixFileDlg, CDialog)

CMophyMatrixFileDlg::CMophyMatrixFileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMophyMatrixFileDlg::IDD, pParent)
{

}

CMophyMatrixFileDlg::~CMophyMatrixFileDlg()
{
}

void CMophyMatrixFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMophyMatrixFileDlg, CDialog)
	ON_BN_CLICKED(IDC_OUTPUTBTN, &CMophyMatrixFileDlg::OnBnClickedOutputbtn)
	ON_EN_CHANGE(IDC_FILEPATHEDIT, &CMophyMatrixFileDlg::OnEnChangeFilepathedit)
END_MESSAGE_MAP()


// CMophyMatrixFileDlg message handlers


void CMophyMatrixFileDlg::OnBnClickedOutputbtn()
{
	// TODO: Add your control notification handler code here
	CString m_strFile = m_filepath;
	CFileDialog fileDlg( TRUE, NULL, NULL, OFN_FILEMUSTEXIST, _T("Excel Files (*.csv)|*.csv|All Files (*.*)|*.*||"), this);
	// Call DoModal
	if ( fileDlg.DoModal() == IDOK)
	{
		m_filepath = fileDlg.GetPathName(); // This is your selected file name with path
		GetDlgItem(IDC_FILEPATHEDIT)->SetWindowText(m_filepath);
	}
}


void CMophyMatrixFileDlg::OnEnChangeFilepathedit()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	GetDlgItem(IDC_FILEPATHEDIT)->GetWindowText(m_filepath);
}
