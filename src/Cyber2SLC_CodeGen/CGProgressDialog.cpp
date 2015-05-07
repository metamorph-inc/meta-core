// CGProgressDialog.cpp : implementation file
//

#include "stdafx.h"
#include "CGProgressDialog.h"
#include "afxdialogex.h"


// CCGProgressDialog dialog

IMPLEMENT_DYNAMIC(CCGProgressDialog, CDialog)

CCGProgressDialog::CCGProgressDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CCGProgressDialog::IDD, pParent),
	  processPos(0)
{
	m_maxPrg  = 100;
	m_cancel = false;
}

CCGProgressDialog::~CCGProgressDialog()
{
}

void CCGProgressDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	DDX_Control(pDX, IDC_STATUS_EDIT, m_status);
}


BEGIN_MESSAGE_MAP(CCGProgressDialog, CDialog)

END_MESSAGE_MAP()


// CCGProgressDialog message handlers
void CCGProgressDialog::PostNcDestroy() 
{
	CDialog::PostNcDestroy();
	delete this; 
}

void CCGProgressDialog::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	m_cancel = true;
	CDialog::OnCancel();
}

void CCGProgressDialog::SetProgress(const CString &status)
{
	if( processPos > int(m_maxPrg)) processPos = 0;
	m_progress.SetRange( 0, m_maxPrg);
	m_progress.SetPos(processPos);

	processPos += 10;

	m_status.SetWindowText(status);

	this->UpdateData();
	this->UpdateWindow();
	this->RedrawWindow();
	this->ShowWindow(SW_SHOW);
}

void CCGProgressDialog::OnFinished()
{
	m_progress.SetRange( 0, (short) m_maxPrg);
	while(processPos < int(m_maxPrg))
	{
		Sleep(20);
		m_progress.StepIt();
		processPos += 5;
	}
	CDialog::OnCancel();
}

void CCGProgressDialog::SetRange(int range)
{
	if(range > int(m_maxPrg))
		m_maxPrg = range;
};

CCGProgressDialog * GetCGProgressDlg(CCGProgressDialog * set_dlg)
{
	static CCGProgressDialog * progressdlg;
	
	if (set_dlg) progressdlg = set_dlg;
	ASSERT(progressdlg != NULL);
	return progressdlg;
};
