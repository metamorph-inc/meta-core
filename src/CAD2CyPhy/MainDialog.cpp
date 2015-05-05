// MainDialog.cpp : implementation file
//

#include "MainDialog.h"
#include "afxdialogex.h"
#include "afxdlgs.h"

// MainDialog dialog

IMPLEMENT_DYNAMIC(MainDialog, CDialog)

MainDialog::MainDialog(CWnd* pParent /*=NULL*/)
	: CDialog(MainDialog::IDD, pParent),
	m_metricsFile(_T(""))
{

}

MainDialog::~MainDialog()
{
}

void MainDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FILE  , m_metricsFile);
}


BEGIN_MESSAGE_MAP(MainDialog, CDialog)
	ON_BN_CLICKED(IDC_OK_BUTTON, &MainDialog::OnBnClickedOkButton)
	ON_BN_CLICKED(IDC_CANCEL_BUTTON, &MainDialog::OnBnClickedCancelButton)
	ON_BN_CLICKED(IDC_FILE_BUTTON, &MainDialog::OnBnClickedFileButton)
END_MESSAGE_MAP()


// MainDialog message handlers


void MainDialog::OnBnClickedOkButton()
{
	// TODO: Add your control notification handler code here
	this->UpdateData(1);
	
	if(m_metricsFile.IsEmpty())
		AfxMessageBox("You must specify the output directory.", MB_ICONINFORMATION );
	else
		this->OnOK();
}


void MainDialog::OnBnClickedCancelButton()
{
	// TODO: Add your control notification handler code here
	this->OnCancel();
}


void MainDialog::OnBnClickedFileButton()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	CFileDialog outputLoc = CFileDialog(1);
	int dialogReturn(outputLoc.DoModal());
	if (dialogReturn == IDOK) 
	{
		this->m_metricsFile = outputLoc.GetPathName();
		UpdateData(false);
	}
}
