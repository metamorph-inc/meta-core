#pragma once

#include "resource.h"
//#include "UdmBase.h"
#include <afxcmn.h>

#include "DesertHelper.h"
#include <afxwin.h>
// CConstraintMainDialog dialog
#include "CheckHeadCtrl.h"
#include "SortListCtrl.h"

#include "WndResizer.h"

#define MAX_FILTERS 32   //in desert #define MAX_GENERATIONS 32

class CConstraintMainDialog : public CDialog
{
	DECLARE_DYNAMIC(CConstraintMainDialog)

public:
	CConstraintMainDialog(CWnd* pParent = NULL);   // standard constructor
	CConstraintMainDialog(DesertHelper *deserthelper_ptr, CWnd* pParent = NULL);  
	virtual ~CConstraintMainDialog();

// Dialog Data
	enum { IDD = IDD_MAIN_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnBnClickedAddbtn();
private:
	CString m_cname;
	CString m_cexpression;
	DesertHelper *dhelper_ptr;
	CToolTipCtrl* m_pToolTip;
	
	set<int> conIdlist;
//	list< set<int> > appliedCons;
//	int currConListPosition;
	double dspSize;

	set<CString> invalidConstraintName_set;

public:
	static map<int, std::string> constraintExprMap;
	CSortListCtrl m_listctrl;
	virtual BOOL OnInitDialog();
private:
	void FillList();
	void FillList(int index, const std::string &cons_name, const std::string &cons_context, const std::string &cons_type, const std::string &cons_domain);
	void FillSizeBox();
	void FillFilter();
	afx_msg void OnInfoTip( NMHDR * pNMHDR, LRESULT * pResult );
public:
	afx_msg void OnBnClickedEditbtn();
	afx_msg void OnBnClickedCommitbtn();
	afx_msg void OnBnClickedApplybtn();
	afx_msg void OnBnClickedApplyallbtn();
	afx_msg void OnBnClickedValidatebtn();
//	afx_msg void OnBnClickedRemovebtn();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
private:
	CButton m_btEdit;
	CButton m_btValidate;
	CButton m_btCommit;
	CButton m_btRestart;
	CButton m_btApply;
	CButton m_btApplyAll;

private:
	BOOL	m_blInited;
//	CImageList	m_checkImgList;
//	CCheckHeadCtrl	m_checkHeadCtrl;
//	BOOL initHeadListCtr();
	void generateConfig();
	void update();
	void initConstraints();

	CWndResizer m_resizer;
public:
	afx_msg void OnBnClickedBackbtn();
	afx_msg void OnBnClickedForwardbtn();
	afx_msg void OnBnClickedGencfgbtn();
	CButton m_btBack;
	CButton m_btForward;
	CButton m_btGenCfgs;
	CButton m_btSelect;
	CListCtrl m_filterlist;
	HWND m_gmeWindow;

private:
	bool refresh_needed;
	bool applyAll;
	/*bool constraint_applied;*/
	set<int> appliedConSet;
	set<int> invalidConSet;

public:
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedOk();
	afx_msg void OnNMCustomdrawConstraintlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedSelectbtn();	
	afx_msg void OnBnClickedRestartbtn();
	afx_msg void OnLvnItemchangedConstraintlist(NMHDR *pNMHDR, LRESULT *pResult);
private:
	void setDomainCheck(int nItem, bool check=true);
	void setCheckAll(bool check=true);
	
public:
	afx_msg void OnNMClickFilterlist(NMHDR *pNMHDR, LRESULT *pResult);
private:
	int applyCounter;
	int applyIdx;
	set<int> filters[MAX_FILTERS];
public:
	afx_msg void OnNMCustomdrawFilterlist(NMHDR *pNMHDR, LRESULT *pResult);

	virtual INT_PTR DoModal()
	{
		try
		{
			INT_PTR ret = __super::DoModal();
			return ret;
		}
		catch (...)
		{
			OnDestroy();
			throw;
		}
	}
};
