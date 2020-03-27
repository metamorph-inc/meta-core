#pragma once

#include "resource.h"
#include <afxwin.h>
#include <afxcmn.h>
#include "DesertIface.h"
#include "DesertIfaceBack.h"
#include "DesMap.h"
#include "DesBackMap.h"
// CDesertStatusDlg dialog

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
	SD_PREAPPLY,
	SD_APPLY,
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
	{SD_PREAPPLY,	_T("Verify, Analysis constraints"),		72},
	{SD_APPLY,	_T("Apply constraints"),					74},
	{SD_GUI,	_T("Invoking Desert GUI"),					75},
	{SD_PREP,	_T("Prepare output"),						80},
	{SD_BACK,	_T("Writing output"),						90},
	{SD_FINIT,	_T("Done"),									100},
	{SD_NULL,	NULL,									-1}
};

const StatusDefinition * LookUpStatus(StatusID s_id);

/////////////////////////////////////////////////////////////////////////////
// CDesertStatusDlg dialog

#define PBR_RANGE 1000000
#define SET_STATUS (WM_USER+1)
#define SET_PROGRESS (WM_USER+2)
#define DESERT_FINISHED (WM_USER+3)

class Notify;

class DesertThread;

class CDesertStatusDlg : public CDialog
{
	DECLARE_DYNAMIC(CDesertStatusDlg)

private:
	unsigned long tick;
	StatusID status;
	bool _silent;
	UINT m_maxPrg;
	int processPos;

	//for DesertThread
	Notify* m_notify;
	HANDLE m_thrd;
	std::unique_ptr<DesertThread> threadData;
	bool m_finished;
	DesertIface::DesertSystem m_ds;
	DesertIfaceBack::DesertBackSystem m_dbs;
	std::string m_constraints;
	int m_stage;
	UdmDesertMap &m_des_map;
	DesertUdmMap &m_inv_des_map;

	long& m_configCount;

public:
	LRESULT OnStatus(WPARAM wp, LPARAM lp);
	LRESULT OnProgress(WPARAM wp, LPARAM lp);
	LRESULT OnFinished1(WPARAM wp, LPARAM lp);
	
	//CDesertStatusDlg(CWnd* pParent = NULL, bool silent= false);   // standard constructor
	CDesertStatusDlg(DesertIface::DesertSystem &dsystem, 
					 const std::string &constraints,
					 UdmDesertMap &des_map,
					 DesertUdmMap &inv_des_map,
					 CWnd* pParent, bool silent, long& configCount);
	CDesertStatusDlg(DesertIfaceBack::DesertBackSystem &dbacksystem,
					 UdmDesertMap &des_map,
					 DesertUdmMap &inv_des_map,
					 CWnd* pParent, bool silent, long& configCount);
	virtual ~CDesertStatusDlg();
	void SetStatus(StatusID s_id);
	void SetStatus(const TCHAR *desc, int percent);
	unsigned long StepInState(short p);
	unsigned long StepInState(short p, const TCHAR *desc);

// Dialog Data
	enum { IDD = IDD_DESERTSTATUSDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_status;
	CProgressCtrl m_prgBar;
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void SetProgress(const CString &status);
	void OnFinished();
	void SetRange(int range);
//	afx_msg void OnBnClickedCcancel();
	bool m_cancel;
	bool m_fatal;
	CString m_invalidConstraint;
};
