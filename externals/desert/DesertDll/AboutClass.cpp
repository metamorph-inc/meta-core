// AboutClass.cpp : implementation file
//

#include "stdafx.h"
#include "desert.h"
#include "AboutClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
#define WIDE2(x) L##x
#define WIDE1(x) WIDE2(x)
static TCHAR THIS_FILE[] = WIDE1(__FILE__);
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutClass dialog


CAboutClass::CAboutClass(CWnd* pParent /*=NULL*/)
	: CDialog(CAboutClass::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAboutClass)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAboutClass::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutClass)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAboutClass, CDialog)
	//{{AFX_MSG_MAP(CAboutClass)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAboutClass message handlers

void CAboutClass::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}
