// DesertStatusDlg.cpp : implementation file
//

#include "StdAfx.h"
#include "DesertStatusDlg.h"
#include <afxdialogex.h>
#include "DesertStatusDlg.h"
#include "Splash.h"

#include <boost/thread/thread.hpp>
#include "DesertThread.h"
// CDesertStatusDlg dialog

IMPLEMENT_DYNAMIC(CDesertStatusDlg, CDialog)

//CDesertStatusDlg::CDesertStatusDlg(CWnd* pParent /*=NULL*/, bool silent)
//	: CDialog(CDesertStatusDlg::IDD, pParent), _silent( silent), processPos(0), m_notify(0), m_thrd(0),m_finished(true),m_cancel(false)
//{
//	m_maxPrg  = 150;
//}

CDesertStatusDlg::CDesertStatusDlg(DesertIface::DesertSystem &dsystem, 
					               const std::string &constraints, 
								   UdmDesertMap &des_map,
								   DesertUdmMap &inv_des_map,
					               CWnd* pParent, bool silent, long& configCount):
											m_notify(0), 
											m_thrd(0),
											m_ds(dsystem),
											m_constraints(constraints),
											m_des_map(des_map),
											m_inv_des_map(inv_des_map),
											m_stage(1),
											CDialog(CDesertStatusDlg::IDD, pParent), 
											_silent( silent), 
											processPos(0),
											m_finished(true),
											m_cancel(false),
											m_configCount(configCount)
{
	m_maxPrg  = 150;
}

CDesertStatusDlg::CDesertStatusDlg(DesertIfaceBack::DesertBackSystem &dbacksystem, 
								   UdmDesertMap &des_map,
								   DesertUdmMap &inv_des_map,
					 CWnd* pParent, bool silent, long& configCount):
						m_notify(0), 
						m_thrd(0),
						m_dbs(dbacksystem),
						m_des_map(des_map),
						m_inv_des_map(inv_des_map),
						m_stage(2),
						CDialog(CDesertStatusDlg::IDD, pParent), 
						_silent( silent), 
						processPos(0),
						m_finished(true),
						m_cancel(false),
						m_configCount(configCount)
{
	m_maxPrg  = 150;
}

CDesertStatusDlg::~CDesertStatusDlg()
{
}

void CDesertStatusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATUS, m_status);
	DDX_Control(pDX, IDC_PROGRESS1, m_prgBar);
}


BEGIN_MESSAGE_MAP(CDesertStatusDlg, CDialog)
	ON_WM_PAINT()
	ON_MESSAGE(SET_STATUS, OnStatus)
	ON_MESSAGE(SET_PROGRESS, OnProgress)
	ON_MESSAGE(DESERT_FINISHED, OnFinished1)
//	ON_BN_CLICKED(IDCCANCEL, &CDesertStatusDlg::OnBnClickedCcancel)
END_MESSAGE_MAP()


// CDesertStatusDlg message handlers


BOOL CDesertStatusDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	//splash
	CSplashWnd::EnableSplashScreen(_silent);
	if ( !_silent) {
		CSplashWnd::ShowSplashScreen(this);
	}

	m_prgBar.SetRange32(0, PBR_RANGE);

	if(m_ds!=Udm::null && m_stage==1)
	{
		m_finished = false;
		m_notify = new Notify(*this, m_maxPrg);
		m_thrd = new boost::thread(DesertThread(m_ds,m_constraints, m_des_map, m_inv_des_map, m_notify,m_stage, m_configCount));
	}
	else if(m_dbs!=Udm::null && m_stage==2)
	{
		m_finished = false;
		m_notify = new Notify(*this, m_maxPrg);
		m_thrd = new boost::thread(DesertThread(m_dbs,m_des_map, m_inv_des_map, m_notify, m_stage, m_configCount));
	}
	else if(m_ds!=Udm::null && m_dbs!=Udm::null && m_stage==0)
	{
	
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CDesertStatusDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (CSplashWnd::PreTranslateAppMessage(pMsg))
		return TRUE;
	
	return CDialog::PreTranslateMessage(pMsg);
}

#include <iostream> // for std::cout
void CDesertStatusDlg::SetStatus(StatusID s_id)
{
	if ( _silent) {
		std::cout << ".";
	} else {
		status = s_id;
		const StatusDefinition * s_def = LookUpStatus(s_id);
		//m_Status = s_def->desc;
		m_status.SetWindowText(s_def->desc);
		m_status.RedrawWindow();
		m_status.UpdateData();

		tick = (unsigned long int)((float)PBR_RANGE * (float)s_def->percent / 100.00);
		m_prgBar.SetPos(tick);


		this->UpdateData();
		this->UpdateWindow();
		this->RedrawWindow();
		this->ShowWindow(SW_SHOW);
	}
};

void CDesertStatusDlg::SetStatus(const TCHAR *desc, int percent)
{
	if(m_cancel) {
		return;
	}

	if ( _silent) {
		std::cout << ".";
	} else {
		m_status.SetWindowText(desc);
		m_status.RedrawWindow();
		m_status.UpdateData();

		tick = (unsigned long int)((float)PBR_RANGE * (float)percent / 100.00);
		m_prgBar.SetPos(tick);


		this->UpdateData();
		this->UpdateWindow();
		this->RedrawWindow();
		this->ShowWindow(SW_SHOW);
	}
}

unsigned long CDesertStatusDlg::StepInState(short p)
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

unsigned long CDesertStatusDlg::StepInState(short p, const TCHAR *desc)
{
	ASSERT( status != SD_FINIT);
	ASSERT( p <= 100 && p >= 0);

	const StatusDefinition * s_def = LookUpStatus((StatusID)((int)status + 1));
	unsigned long end_tick = 
		(unsigned long int)((float)PBR_RANGE * (float)s_def->percent / 100.00);
	unsigned long new_tick = tick + \
		(unsigned int) ((float)(end_tick - tick) * (float)p / 100.00);
	m_status.SetWindowText(desc);
	m_status.RedrawWindow();
		m_status.UpdateData();
	m_prgBar.SetPos(new_tick);
	m_prgBar.UpdateWindow();

	return new_tick;
};

CDesertStatusDlg * GetStatusDlg(CDesertStatusDlg * set)
{
	static CDesertStatusDlg * csdlg = NULL;
	
	if (set != 0) {
		csdlg = set;
	}
	ASSERT(csdlg != NULL);
	return csdlg;

};

const StatusDefinition * LookUpStatus(StatusID s_id)
{
	const StatusDefinition * std = Stats;
	while (std->id != SD_NULL)
	{
		if (std->id == s_id) {
			break;
		}
		std++;
	}

	ASSERT(std != NULL);

	return std;
}

void CDesertStatusDlg::SetProgress(const CString &status)
{
	if(processPos > m_maxPrg) processPos = 0;
	m_prgBar.SetRange( 0, m_maxPrg);
	m_prgBar.SetPos(processPos);

	processPos += 5;

	m_status.SetWindowText(status);

	this->UpdateData();
	this->UpdateWindow();
	this->RedrawWindow();
	this->ShowWindow(SW_SHOW);
}

void CDesertStatusDlg::SetRange(int range)
{
	if(range > m_maxPrg)
		m_maxPrg = range;
};


void CDesertStatusDlg::OnFinished()
{
	m_prgBar.SetRange( 0, m_maxPrg);
	while(processPos < m_maxPrg)
	{
		Sleep(20);
		m_prgBar.StepIt();
		processPos += 5;
	}
	CDialog::OnCancel();
}

LRESULT CDesertStatusDlg::OnFinished1(WPARAM wp, LPARAM lp)
{
	if(m_notify)
		if(!m_notify->m_quit || !m_notify->m_cancel)  //If there is any exception, do not show the progress any more
		{
			m_prgBar.SetRange( 0, m_maxPrg);
			while(processPos < m_maxPrg)
			{
				Sleep(50);
				m_prgBar.StepIt();
				processPos += 5;
			}
		}
		

	if (m_thrd )
	{
		m_fatal = m_notify->m_fail;
		m_notify->m_quit = true;
		m_invalidConstraint = m_notify->m_invalidConstraint;
		m_thrd->join();	
	}	

	CDialog::OnCancel();	
	return 0;
}

LRESULT CDesertStatusDlg::OnStatus(WPARAM wp, LPARAM lp)
{
	if ( _silent) {
		std::cout << ".";
	} else {
		m_status.SetWindowText((LPCTSTR)wp);
		m_status.RedrawWindow();
		m_status.UpdateData();

		unsigned long tick = (unsigned long int)((float)PBR_RANGE * lp / 100.00);
		m_prgBar.SetPos(tick);


		this->UpdateData();
		this->UpdateWindow();
		this->RedrawWindow();
		this->ShowWindow(SW_SHOW);
	}
	
	return 0;
}

LRESULT CDesertStatusDlg::OnProgress(WPARAM wp, LPARAM lp)
{
	SetProgress((LPCTSTR)wp);
	return 0;
}

//void CDesertStatusDlg::OnBnClickedCcancel()
//{
//	// TODO: Add your control notification handler code here
//	if (m_thrd )
//	{
//		m_notify->m_quit = true;
//		m_notify->m_cancel = true;
//		m_cancel = true;
//		m_thrd->join();
//	}
//	//m_notify->finished();
//
//	CDialog::OnCancel();
//}

void CDesertStatusDlg::OnCancel()
{
	// TODO: Add your control notification handler code here
	if (m_thrd )
	{
		m_notify->m_quit = true;
		m_notify->m_cancel = true;	
		m_fatal = m_notify->m_fail;
		m_invalidConstraint = m_notify->m_invalidConstraint;
		m_thrd->join();
	}
	//m_notify->finished();
	m_cancel = true;
	CDialog::OnCancel();
}

void CDesertStatusDlg::PostNcDestroy() 
{
	delete m_thrd, m_thrd = 0;
	delete m_notify, m_notify = 0;

	CDialog::PostNcDestroy();
}