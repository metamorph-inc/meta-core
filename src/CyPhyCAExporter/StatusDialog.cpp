// StatusDialog.cpp : implementation file
//

#include "stdafx.h"
#include "StatusDialog.h"


// CStatusDialog dialog

IMPLEMENT_DYNAMIC(CStatusDialog, CDialog)

CStatusDialog::CStatusDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CStatusDialog::IDD, pParent), m_gmeWindow(NULL),
	  processPos(0)
{
	m_maxPrg  = 100;
}

CStatusDialog::~CStatusDialog()
{
}

void CStatusDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_prgBar);
	DDX_Control(pDX, IDC_EDIT1, m_status);
}


BEGIN_MESSAGE_MAP(CStatusDialog, CDialog)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

BOOL CStatusDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	//splash

	m_gmeWindow = ::GetActiveWindow();
	if (m_gmeWindow != NULL) // gmeWindow could be NULL if this component is invoked outside of GME
	{
		::EnableWindow(m_gmeWindow, FALSE); // call this after you ShowWindow your window

#ifdef _DEBUG
		CString gmeWindowText;
		::GetWindowText(m_gmeWindow, gmeWindowText.GetBuffer(500), 500);
		gmeWindowText.ReleaseBuffer();
		OutputDebugString(gmeWindowText.GetBuffer()); // prints the title of the GME window, e.g. MetaGME - UML
#endif
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CStatusDialog::SetProgress(const CString &status)
{
	if(processPos > m_maxPrg) processPos = 0;
	m_prgBar.SetRange( 0, m_maxPrg);
	m_prgBar.SetPos(processPos);

	processPos += 10;

	m_status.SetWindowText(status);

	this->UpdateData();
	this->UpdateWindow();
	this->RedrawWindow();
	this->ShowWindow(SW_SHOW);
}

void CStatusDialog::OnDestroy()
{
	if (m_gmeWindow != NULL)
	{
		::EnableWindow(m_gmeWindow, TRUE);
	}
}

void CStatusDialog::OnFinished()
{
	m_prgBar.SetRange( 0, m_maxPrg);
	while(processPos < m_maxPrg)
	{
		//Sleep(20);
		m_prgBar.StepIt();
		processPos += 5;
	}
	CDialog::OnCancel();
}

void CStatusDialog::SetRange(int range)
{
	if(range > m_maxPrg)
		m_maxPrg = range;
};
