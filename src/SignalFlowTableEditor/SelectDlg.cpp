// SelectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SelectDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectDlg dialog


CSelectDlg::CSelectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectDlg)
	m_chkAllKinds = TRUE;
	m_chkAllTypes = TRUE;
	m_chkAtom = FALSE;
	m_chkModel = FALSE;
	m_chkRef = FALSE;
	m_chkSet = FALSE;
	m_chkCon = FALSE;
	//}}AFX_DATA_INIT
}


void CSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectDlg)
	DDX_Control(pDX, IDC_CHKCON, m_btnCon);
	DDX_Control(pDX, IDC_CHKSET, m_btnSet);
	DDX_Control(pDX, IDC_CHKREF, m_btnRef);
	DDX_Control(pDX, IDC_CHKMODEL, m_btnModel);
	DDX_Control(pDX, IDC_CHKATOM, m_btnAtom);
	DDX_Control(pDX, IDC_LISTKIND, m_lstKind);
	DDX_Check(pDX, IDC_CHECKALLKINDS, m_chkAllKinds);
	DDX_Check(pDX, IDC_CHKALLTYPES, m_chkAllTypes);
	DDX_Check(pDX, IDC_CHKATOM, m_chkAtom);
	DDX_Check(pDX, IDC_CHKMODEL, m_chkModel);
	DDX_Check(pDX, IDC_CHKREF, m_chkRef);
	DDX_Check(pDX, IDC_CHKSET, m_chkSet);
	DDX_Check(pDX, IDC_CHKCON, m_chkCon);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectDlg, CDialog)
	//{{AFX_MSG_MAP(CSelectDlg)
	ON_BN_CLICKED(IDC_CHECKALLKINDS, OnCheckAllKinds)
	ON_BN_CLICKED(IDC_CHKALLTYPES, OnChkAllTypes)
	ON_BN_CLICKED(IDC_CHKATOM, OnChkAtom)
	ON_BN_CLICKED(IDC_CHKCON, OnChkCon)
	ON_BN_CLICKED(IDC_CHKMODEL, OnChkModel)
	ON_BN_CLICKED(IDC_CHKREF, OnChkRef)
	ON_BN_CLICKED(IDC_CHKSET, OnChkSet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectDlg message handlers

BOOL CSelectDlg::OnInitDialog()
{
	BOOL val = CDialog::OnInitDialog();
	
	CComPtr<IMgaMetaProject> m_MetaProject;
	COMTHROW(m_Project->get_RootMeta(&m_MetaProject));

	
	COMTHROW(m_MetaProject->get_RootFolder(&m_rootMetaFolder));

	GetMetaObjectNames(m_rootMetaFolder);

	OnCheckAllKinds();
	OnChkAllTypes();

	return val;

}


void CSelectDlg::GetMetaObjectNames(IMgaMetaBase *metaBase) //based on GetMetaObjectsR in OCLCommonEx.cpp
{
		CComPtr<IMgaMetaFCOs> p_FCOs;
		CComQIPtr<IMgaMetaModel> p_Model = metaBase;
		if ( p_Model.p )
			COMTHROW( p_Model->get_DefinedFCOs( &p_FCOs ) );
		else {
			CComQIPtr<IMgaMetaFolder> p_Folder = metaBase;
			if ( p_Folder.p ) {
				CComPtr<IMgaMetaFolders> p_Folders;
				COMTHROW( p_Folder->get_DefinedFolders( &p_Folders ) );
				MGACOLL_ITERATE( IMgaMetaFolder, p_Folders ) {
					GetMetaObjectNames( MGACOLL_ITER.p);
				} MGACOLL_ITERATE_END;
				COMTHROW( p_Folder->get_DefinedFCOs( &p_FCOs ) );
			}
		}
		if ( p_FCOs.p ) {
			MGACOLL_ITERATE( IMgaMetaFCO, p_FCOs ) {
				CBstr MetaFCOName;
				COMTHROW(MGACOLL_ITER->get_Name(MetaFCOName));

				if(m_chkAllTypes)
					m_lstKind.AddString(CString(MetaFCOName)); //want a string for each kind
				else
				{ //want a string for each kind of specific types ..
					if(m_chkAtom)
					{
						CComQIPtr<IMgaMetaAtom> qAtom = MGACOLL_ITER;
						if (qAtom.p) m_lstKind.AddString(CString(MetaFCOName));
					}
					if(m_chkModel)
					{
						CComQIPtr<IMgaMetaModel> qModel = MGACOLL_ITER;
						if (qModel.p) m_lstKind.AddString(CString(MetaFCOName));
					}
					if(m_chkRef)
					{
						CComQIPtr<IMgaMetaReference> qRef = MGACOLL_ITER;
						if (qRef.p) m_lstKind.AddString(CString(MetaFCOName));
					}
					if(m_chkSet)
					{
						CComQIPtr<IMgaMetaSet> qSet = MGACOLL_ITER;
						if (qSet.p) m_lstKind.AddString(CString(MetaFCOName));
					}
					if(m_chkCon)
					{
						CComQIPtr<IMgaMetaConnection> qCon = MGACOLL_ITER;
						if (qCon.p) m_lstKind.AddString(CString(MetaFCOName));
					}
				}

				GetMetaObjectNames( MGACOLL_ITER.p);
			} MGACOLL_ITERATE_END;
		}

}

void CSelectDlg::OnOK() //Display the grid
{
	
	if(CWnd::UpdateData(TRUE))
	{

		CGridDlg m_Table;

		int nCount = m_lstKind.GetSelCount();
		CArray<int,int> aryLstKindSel;

		aryLstKindSel.SetSize(nCount);
		m_lstKind.GetSelItems(nCount, aryLstKindSel.GetData());
		
		CString m_kindsFiltered;

		if(m_chkAllKinds) //show all kinds
		{
			m_kindsFiltered = "";
		}
		else //show only what's checked in m_lstKind
		{
			for(int i=0; i<nCount; i++)
			{
				CString tmp;
				m_lstKind.GetText(aryLstKindSel.GetAt(i),tmp);
				m_kindsFiltered = m_kindsFiltered + tmp + " ";
			}
		}

		CString m_ObjType = "";

		if(m_chkAllTypes)
		{
			m_ObjType = "";
		}
		else
		{
			if(m_chkAtom)
				m_ObjType = m_ObjType + "OBJTYPE_ATOM";
			if(m_chkModel)
				m_ObjType = m_ObjType + " " + "OBJTYPE_MODEL";
			if(m_chkRef)
				m_ObjType = m_ObjType + " " + "OBJTYPE_REFERENCE";
			if(m_chkSet)
				m_ObjType = m_ObjType + " " + "OBJTYPE_SET";
			if(m_chkCon)
				m_ObjType = m_ObjType + " " + "OBJTYPE_CONNECTION";
		}
			

		//AfxMessageBox(KindsFiltered);


		CComPtr<IMgaFilter> m_Filter;
		COMTHROW(m_Project->CreateFilter(&m_Filter));
		COMVERIFY(m_Filter->put_ObjType(CComBSTR(m_ObjType)));
		COMVERIFY(m_Filter->put_Kind(CComBSTR(m_kindsFiltered)));	

		m_Table.SetProject(m_Project);
		m_Table.SetFilter(m_Filter);
		if (m_Table.DoModal() == IDOK)
			CDialog::OnOK();
		else
			CDialog::OnCancel();

		//CDialog::OnOK();
	}

	
}

void CSelectDlg::OnCheckAllKinds() 
{
	CWnd::UpdateData(TRUE);
	if(m_chkAllKinds)
	{
		m_lstKind.EnableWindow(FALSE);
	}
	else
	{
		m_lstKind.EnableWindow(TRUE);
	}
	
}

void CSelectDlg::OnChkAllTypes() 
{
	CWnd::UpdateData(TRUE);
	if(m_chkAllTypes)
	{
		m_btnAtom.EnableWindow(FALSE);
		m_btnModel.EnableWindow(FALSE);
		m_btnRef.EnableWindow(FALSE);
		m_btnSet.EnableWindow(FALSE);
		m_btnCon.EnableWindow(FALSE);
	}
	else
	{
		m_btnAtom.EnableWindow(TRUE);
		m_btnModel.EnableWindow(TRUE);
		m_btnRef.EnableWindow(TRUE);
		m_btnSet.EnableWindow(TRUE);
		m_btnCon.EnableWindow(TRUE);
	}
	m_lstKind.ResetContent();
	GetMetaObjectNames(m_rootMetaFolder);
	
}



void CSelectDlg::OnChkAtom() 
{
	CWnd::UpdateData(TRUE);
	m_lstKind.ResetContent();
	GetMetaObjectNames(m_rootMetaFolder);
	
}

void CSelectDlg::OnChkCon() 
{
	CWnd::UpdateData(TRUE);
	m_lstKind.ResetContent();
	GetMetaObjectNames(m_rootMetaFolder);
	
}

void CSelectDlg::OnChkModel() 
{
	CWnd::UpdateData(TRUE);
	m_lstKind.ResetContent();
	GetMetaObjectNames(m_rootMetaFolder);
	
}

void CSelectDlg::OnChkRef() 
{
	CWnd::UpdateData(TRUE);
	m_lstKind.ResetContent();
	GetMetaObjectNames(m_rootMetaFolder);
	
}

void CSelectDlg::OnChkSet() 
{
	CWnd::UpdateData(TRUE);
	m_lstKind.ResetContent();
	GetMetaObjectNames(m_rootMetaFolder);
	
}
