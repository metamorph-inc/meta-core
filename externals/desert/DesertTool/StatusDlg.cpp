// StatusDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DesertTool.h"
#include "StatusDlg.h"

#include "Splash.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatusDlg dialog


CStatusDlg::CStatusDlg(CWnd* pParent /*=NULL*/, bool silent)
	: CDialog(CStatusDlg::IDD, pParent), _silent( silent)
{
	//{{AFX_DATA_INIT(CStatusDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CStatusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatusDlg)
	DDX_Control(pDX, IDC_PROGRESS1, m_prgBar);
	DDX_Control(pDX, IDC_EDIT1, m_Status);
	//}}AFX_DATA_MAP
}
#include <iostream> // for std::cout
void CStatusDlg::SetStatus(StatusID s_id)
{
	status = s_id;
	const StatusDefinition * s_def = LookUpStatus(s_id);
	tick = (unsigned long int)((float)PBR_RANGE * (float)s_def->percent / 100.00);
	if ( _silent) {
		std::cout << ".";
	} else {
		//m_Status = s_def->desc;
		m_Status.SetWindowText(s_def->desc);
		m_Status.RedrawWindow();
		m_Status.UpdateData();

		m_prgBar.SetPos(tick);


		this->UpdateData();
		this->UpdateWindow();
		this->RedrawWindow();
		this->ShowWindow(SW_SHOW);
	}
};

unsigned long CStatusDlg::StepInState(short p)
{
	ASSERT( status != SD_FINIT);
	ASSERT( p <= 100 && p >= 0);


	const StatusDefinition * s_def = LookUpStatus((StatusID)((int)status + 1));
	unsigned long end_tick = 
		(unsigned long int)((float)PBR_RANGE * (float)s_def->percent / 100.00);
	unsigned long new_tick = tick + \
		(unsigned int) ((float)(end_tick - tick) * (float)p / 100.00);

	m_prgBar.SetPos(new_tick);
	m_prgBar.UpdateWindow();

	return new_tick;
};

CStatusDlg * GetStatusDlg(CStatusDlg * set)
{
	static CStatusDlg * csdlg;
	
	if (set) csdlg = set;
	ASSERT(csdlg != NULL);
	return csdlg;

};




const StatusDefinition * LookUpStatus(StatusID s_id)
{
	const StatusDefinition * std = Stats;
	while (std->id != SD_NULL)
	{
		if (std->id == s_id) break;
		std++;
	}

	ASSERT(std != NULL);

	return std;
}


BEGIN_MESSAGE_MAP(CStatusDlg, CDialog)
	//{{AFX_MSG_MAP(CStatusDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatusDlg message handlers

BOOL CStatusDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//splash
	CSplashWnd::EnableSplashScreen(_silent);
	if ( !_silent) {
		CSplashWnd::ShowSplashScreen(this);
	}


	// TODO: Add extra initialization here
	m_prgBar.SetRange32(0, PBR_RANGE);
	
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CStatusDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (CSplashWnd::PreTranslateAppMessage(pMsg))
           return TRUE;
	
	return CDialog::PreTranslateMessage(pMsg);
}
