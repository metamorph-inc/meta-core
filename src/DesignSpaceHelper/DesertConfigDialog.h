#pragma once

#include "resource.h"
#include <afxcmn.h>
#include "DesertHelper.h"
#include <afxwin.h>
#include "CheckHeadCtrl.h"
// CDesertConfigDialog dialog

#include "WndResizer.h"
#include "SortListCtrl.h"

class CDesertConfigDialog : public CDialog
{
	DECLARE_DYNAMIC(CDesertConfigDialog)

public:
	CDesertConfigDialog(CWnd* pParent = NULL);   // standard constructor
	CDesertConfigDialog(DesertHelper *deserthelper_ptr, CWnd* pParent = NULL);
	virtual ~CDesertConfigDialog();

// Dialog Data
	enum { IDD = IDD_CONFIG_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	CSortListCtrl m_cfglist;
	CTreeCtrl m_cfgtree;
	DesertHelper *dhelper_ptr;
	bool useCurrentCyphy;
	map<HTREEITEM, int> cfgTreeMap;
	bool isActiveCfg;
	long cfgSize;

	CWndResizer m_resizer;
public:
	afx_msg void OnBnClickedExportselbtn();
	afx_msg void OnBnClickedExportallbtn();
protected:
	void FillCfgList();
	void FillCfgTree();
	void FillCfgTree(DesertIface::Element &elem, HTREEITEM parent);

public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnLvnItemchangedCfglist(NMHDR *pNMHDR, LRESULT *pResult);

protected:
	struct Color_Font
	{
		COLORREF color;
		LOGFONT  logfont;
	};
	CMap< void*, void*, Color_Font, Color_Font& > m_mapColorFont ;
public:
	CEdit m_cfgsize;
	afx_msg void OnBnClickedClearallbtn();
	afx_msg void OnNMCustomdrawConstraintlist(NMHDR *pNMHDR, LRESULT *pResult);
private:
	std::string cfgSizeInfo;
	BOOL	m_blInited;
	CImageList	m_checkImgList;
//	CCheckHeadCtrl	m_checkHeadCtrl;
	HTREEITEM m_cfgTreeRootItem;
//	BOOL initHeadListCtr();
	int checkedSize;
	void updateSize();
	void updateConfigList(HTREEITEM &item, set<int> &cfgIds, int &cnt, bool isAlt=false);
	set<int> computeConfigList(HTREEITEM &item, set<int> cfgIds);
	void updateConfigList(set<int> &cfgIds, bool check=true);
//	void updateConfigList_1();
	void checkItem(HTREEITEM &item);
	void checkSiblings_check(HTREEITEM &item);
	void uncheckItem(HTREEITEM &item);
	bool noneChecked;

public:
	afx_msg void OnBnClickedCyPhy2MorphMatrix();
	afx_msg void OnBnClickedMorphMatrix2CyPhy();
	afx_msg void OnNMDblclkCfglist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnBeginlabeleditCfglist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnEndlabeleditCfglist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClkCfgList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCloseallbtn();
};
