#pragma once
#include "resource.h"
#include "CyPhyML.h"
#include <afxcmn.h>

// CDSERootSelDialog dialog

class CDSERootSelDialog : public CDialog
{
	DECLARE_DYNAMIC(CDSERootSelDialog)

public:
	CDSERootSelDialog(CWnd* pParent = NULL);   // standard constructor
	CDSERootSelDialog(CyPhyML::RootFolder &rf, bool runSilent, CWnd* pParent = NULL);   
	virtual ~CDSERootSelDialog();
	Udm::Object GetSelectedDSERoot();
	set<CyPhyML::DesignContainer> GetAllRootDesignContainers();
	virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_DSEROOT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:	
	afx_msg void OnTvnSelchangedDstree(NMHDR *pNMHDR, LRESULT *pResult);
private:
	CTreeCtrl m_dstree;
	map<HTREEITEM, Udm::Object> dseTreeMap;
	Udm::Object dseroot_sel;
	CyPhyML::RootFolder cyphy_rf;
	set<CyPhyML::DesignContainer> root_dcs;
	bool silent_mode;
protected:
	void FillDSETree(CyPhyML::DesignContainer &cyphy_con, HTREEITEM parent); 
	void FillDSETree(CyPhyML::DesignSpace &cyphy_ds, HTREEITEM parent);
};
