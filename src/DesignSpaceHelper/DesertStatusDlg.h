#pragma once

#include "resource.h"
#include "afxwin.h"
#include "afxcmn.h"
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
	const char * desc;
	short percent;
} StatusDefinition;

const StatusDefinition Stats[] = 
{
	{SD_INIT,	"Initializing",							0},
	{SD_PARSE,	"Parsing XML",							5},
	{SD_SPS,	"Creating spaces",						10},
	{SD_ERS,	"Creating Element Relations",			30},
	{SD_CTS,	"Creating constraints",					35},
	{SD_NDS,	"Creating Natural Domains",				40},
	{SD_CDS,	"Creating Custom Domains",				45},
	{SD_MRS,	"Creating Member Relations",			50},		
	{SD_VPS,	"Creating Variable Properties",			55},
	{SD_CPS,	"Creating Constant Properties",			60},
	{SD_ASS,	"Creating Assignments",					70},
	{SD_PREAPPLY,	"Verify, Analysis constraints",		72},
	{SD_APPLY,	"Apply constraints",					74},
	{SD_GUI,	"Invoking Desert GUI",					75},
	{SD_PREP,	"Prepare output",						80},
	{SD_BACK,	"Writing output",						90},
	{SD_FINIT,	"Done",									100},
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

namespace boost
{
  class thread;
}

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
	boost::thread*  m_thrd; 
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
	void SetStatus(const char *info, int percent);
	unsigned long StepInState(short percentage);
	unsigned long StepInState(short percentage, const char *desc);

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
	afx_msg void OnPaint();
//	afx_msg void OnBnClickedCcancel();
	bool m_cancel;
	bool m_fatal;
	CString m_invalidConstraint;
};

CDesertStatusDlg * GetStatusDlg(CDesertStatusDlg * set_dlg);
