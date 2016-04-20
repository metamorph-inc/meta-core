// CyPhy2MorphMatrixFileDlg.cpp : implementation file
//

#include "StdAfx.h"
#include "CyPhy2MorphMatrixFileDlg.h"
#include <afxdialogex.h>


// CCyPhy2MorphMatrixFileDlg dialog

IMPLEMENT_DYNAMIC(CCyPhy2MorphMatrixFileDlg, CDialog)

CCyPhy2MorphMatrixFileDlg::CCyPhy2MorphMatrixFileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCyPhy2MorphMatrixFileDlg::IDD, pParent)
{

}

CCyPhy2MorphMatrixFileDlg::~CCyPhy2MorphMatrixFileDlg()
{
}

void CCyPhy2MorphMatrixFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCyPhy2MorphMatrixFileDlg, CDialog)
	ON_BN_CLICKED(IDC_OUTPUTBTN, &CCyPhy2MorphMatrixFileDlg::OnBnClickedOutputbtn)
	ON_EN_CHANGE(IDC_FILEPATHEDIT, &CCyPhy2MorphMatrixFileDlg::OnEnChangeFilepathedit)
END_MESSAGE_MAP()


// CCyPhy2MorphMatrixFileDlg message handlers


void CCyPhy2MorphMatrixFileDlg::OnBnClickedOutputbtn()
{
	// TODO: Add your control notification handler code here
	CString m_strFile = m_filepath;
	CFileDialog fileDlg( TRUE, NULL, NULL, OFN_FILEMUSTEXIST, _T("Excel Files (*.xlsm)|*.xlsm|All Files (*.*)|*.*||"), this);
	// Call DoModal
	if ( fileDlg.DoModal() == IDOK)
	{
		m_filepath = fileDlg.GetPathName(); // This is your selected file name with path
		GetDlgItem(IDC_FILEPATHEDIT_C2M)->SetWindowText(m_filepath);
	}
}


void CCyPhy2MorphMatrixFileDlg::OnEnChangeFilepathedit()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	GetDlgItem(IDC_FILEPATHEDIT_C2M)->GetWindowText(m_filepath);
}
