#if !defined(AFX_STATUSDLG_H__211F60D4_86F0_44B6_9A0A_3468E8CEFC81__INCLUDED_)
#define AFX_STATUSDLG_H__211F60D4_86F0_44B6_9A0A_3468E8CEFC81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatusDlg.h : header file
//

#include <tchar.h>

typedef enum STATUSID {
	SD_INIT,
	SD_PARSE,
	SD_SPS,
	SD_ERS,
	SD_CTS,
	SD_NDS,
	SD_CDS,
	SD_MRS,
	SD_VPS,
	SD_CPS,
	SD_ASS,
	SD_GUI,
	SD_PREP,
	SD_BACK,
	SD_FINIT,
	SD_NULL

} StatusID;


typedef struct STATUSDEFS {
	StatusID id;
	const TCHAR * desc;
	short percent;
} StatusDefinition;

const StatusDefinition Stats[] = 
{
	{SD_INIT,	_T("Initializing"),							0},
	{SD_PARSE,	_T("Parsing XML"),							5},
	{SD_SPS,	_T("Creating spaces"),						10},
	{SD_ERS,	_T("Creating Element Relations"),			30},
	{SD_CTS,	_T("Creating constraints"),					35},
	{SD_NDS,	_T("Creating Natural Domains"),				40},
	{SD_CDS,	_T("Creating Custom Domains"),				45},
	{SD_MRS,	_T("Creating Member Relations"),			50},		
	{SD_VPS,	_T("Creating Variable Properties"),			55},
	{SD_CPS,	_T("Creating Constant Properties"),			60},
	{SD_ASS,	_T("Creating Assignments"),					70},
	{SD_GUI,	_T("Invoking Desert GUI"),					75},
	{SD_PREP,	_T("Prepare output"),						80},
	{SD_BACK,	_T("Writing output"),						90},
	{SD_FINIT,	_T("Done"),									100},
	{SD_NULL,	NULL,									-1}
};

const StatusDefinition * LookUpStatus(StatusID s_id);

/////////////////////////////////////////////////////////////////////////////
// CStatusDlg dialog

#define PBR_RANGE 1000000

class CStatusDlg : public CDialog
{

private:
	unsigned long tick;
	StatusID status;
	bool _silent;


// Construction
public:
	CStatusDlg(CWnd* pParent = NULL, bool silent= false);   // standard constructor
	void SetStatus(StatusID s_id);
	void StepInState(short percentage);


// Dialog Data
	//{{AFX_DATA(CStatusDlg)
	enum { IDD = IDD_DIALOG1 };
	CProgressCtrl	m_prgBar;
	CEdit	m_Status;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStatusDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStatusDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATUSDLG_H__211F60D4_86F0_44B6_9A0A_3468E8CEFC81__INCLUDED_)
