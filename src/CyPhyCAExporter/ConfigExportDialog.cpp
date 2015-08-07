// ConfigExportDialog.cpp : implementation file
//

#include "stdafx.h"
#include "CyPhyCAExporter.h"
#include "ConfigExportDialog.h"
#include "afxdialogex.h"


// CConfigExportDialog dialog

IMPLEMENT_DYNAMIC(CConfigExportDialog, CDialog)

CConfigExportDialog::CConfigExportDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigExportDialog::IDD, pParent)
	, m_outputfdr(_T(""))
{

}

CConfigExportDialog::~CConfigExportDialog()
{
}

void CConfigExportDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_OUTPUTEDIT, m_outputfdr);
}


BEGIN_MESSAGE_MAP(CConfigExportDialog, CDialog)
	ON_BN_CLICKED(IDC_CURRENTRADIO, &CConfigExportDialog::OnBnClickedCurrentradio)
	ON_BN_CLICKED(IDC_NEWRADIO, &CConfigExportDialog::OnBnClickedNewradio)
	ON_BN_CLICKED(IDC_OUTPUTBTN, &CConfigExportDialog::OnBnClickedOutputbtn)
	ON_EN_CHANGE(IDC_OUTPUTEDIT, &CConfigExportDialog::OnEnChangeOutputedit)
	ON_BN_CLICKED(IDC_HIERARCHICAL, &CConfigExportDialog::OnBnClickedHierarchical)
	ON_BN_CLICKED(IDC_FLATTEN, &CConfigExportDialog::OnBnClickedFlatten)
END_MESSAGE_MAP()


// CConfigExportDialog message handlers


void CConfigExportDialog::OnBnClickedCurrentradio()
{
	// TODO: Add your control notification handler code here
	m_useCurrent = true;
}


void CConfigExportDialog::OnBnClickedNewradio()
{
	// TODO: Add your control notification handler code here
	m_useCurrent = false;
}

int CALLBACK SetSelProc (HWND hWnd,
                         UINT uMsg,
                         LPARAM lParam,
                         LPARAM lpData)
{
   if (uMsg==BFFM_INITIALIZED)
   {
      ::SendMessage(hWnd, BFFM_SETSELECTION, TRUE, lpData );
   }
   return 0;
}

void CConfigExportDialog::OnBnClickedOutputbtn()
{
	CString m_strFile = m_outputfdr;
	CFolderPickerDialog dlg(m_strFile, 0, NULL, 0); 
	if (dlg.DoModal()==IDOK) 
	{
		m_outputfdr = dlg.GetFolderPath();	
		UpdateData(FALSE);
	} 
}


void CConfigExportDialog::OnEnChangeOutputedit()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	GetDlgItem(IDC_OUTPUTEDIT)->GetWindowText(m_outputfdr);
}


BOOL CConfigExportDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CButton *pRbtn1 = (CButton*)this->GetDlgItem(IDC_CURRENTRADIO);
	pRbtn1->SetFocus();      
    pRbtn1->SetCheck(true); 
	m_useCurrent = true;
	CButton *pRbtn2 = (CButton*)this->GetDlgItem(IDC_HIERARCHICAL);
	pRbtn2->SetFocus();      
    pRbtn2->SetCheck(true); 
	m_flatten = false;

	TCHAR szDirectory[MAX_PATH] = L"";
	::GetCurrentDirectory(sizeof(szDirectory) - 1, szDirectory);
	m_outputfdr = (CString)szDirectory;
	this->GetDlgItem(IDC_OUTPUTEDIT)->SetWindowText(szDirectory);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CConfigExportDialog::OnBnClickedHierarchical()
{
	// TODO: Add your control notification handler code here
	m_flatten = false;
}


void CConfigExportDialog::OnBnClickedFlatten()
{
	// TODO: Add your control notification handler code here
	m_flatten = true;
}
