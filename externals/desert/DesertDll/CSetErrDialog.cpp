// CSetErrDialog.cpp : implementation file
//

#include "stdafx.h"
#include "desert.h"
#include "CSetErrDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
#define WIDE2(x) L##x
#define WIDE1(x) WIDE2(x)
static TCHAR THIS_FILE[] = WIDE1(__FILE__);
#endif

/////////////////////////////////////////////////////////////////////////////
// CCSetErrDialog dialog


CCSetErrDialog::CCSetErrDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CCSetErrDialog::IDD, pParent)
{
	alive = false;
	//{{AFX_DATA_INIT(CCSetErrDialog)
	m_strErrCts = _T("");
	//}}AFX_DATA_INIT
	errId = 1;
}


void CCSetErrDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCSetErrDialog)
	DDX_Text(pDX, IDC_CERR, m_strErrCts);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCSetErrDialog, CDialog)
	//{{AFX_MSG_MAP(CCSetErrDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCSetErrDialog message handlers

void CCSetErrDialog::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CCSetErrDialog::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL CCSetErrDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();


	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCSetErrDialog::AddError(const TCHAR * CSet, const TCHAR * c, const TCHAR * context, const TCHAR * err_str, const TCHAR * what) 
{
//	int i = 1;
	CString formatter;
	if (c)
	{
		alive = true;
		
		formatter.Format(_T("---Errored Constraint No: %d----------\r\n"), errId);
		m_strErrCts += formatter;

		if (CSet)
		{
			formatter.Format(_T("Constraint Set: %s\r\n"), CSet);
			m_strErrCts += formatter;
		}

		if (c)
		{
			formatter.Format(_T("Constraint: %s\r\n"), c);
			m_strErrCts += formatter;
		}

		if (context)
		{
			formatter.Format(_T("Context: %s\r\n"), context);
			m_strErrCts += formatter;
		}

		if (err_str)
		{
			formatter.Format(_T("Context Error: %s\r\n"), err_str);
			m_strErrCts += formatter;
		}

		if (what)
		{
			formatter.Format(_T("Exception: %s\r\n"), what);
			m_strErrCts += formatter;
		}

		errId++;
	}
}


