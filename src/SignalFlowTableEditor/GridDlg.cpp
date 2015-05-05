// GridDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GridDlg.h"
#include "NewCellTypes/GridCellCombo.h"
#include "NewCellTypes/GridCellComboBool.h"
#include "NewCellTypes/GridCellNumeric.h"
#include "NewCellTypes/GridCellDouble.h"
#include "NewCellTypes/GridCellFCO.h"
#include "NewCellTypes/GridCellMultiLine.h"
#include "NewCellTypes/GridCellAttrName.h"
//#include "TableEditorParser.h"
//#include "TableEditorDumper.h"
#include <queue>
#include "Console.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COLOFFSET 7 //offset until attributes start, to make it easy to add columns (extended name, etc)
#define KIND_COL 0
#define NAME_COL 1
#define ID_COL 2
#define EXTNAME_COL 3
#define INSTANCE_COL 4
#define SRC_COL 5
#define DST_COL 6


/////////////////////////////////////////////////////////////////////////////
// CGridDlg dialog


CGridDlg::CGridDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGridDlg::IDD, pParent)
{
	//{{AF    X_DATA_INIT(CGridDlg)
	m_chkAllKinds = TRUE;
	m_chkAllTypes = TRUE;
	m_chkAtom = FALSE;
	m_chkCon = FALSE;
	m_chkModel = FALSE;
	m_chkRef = FALSE;
	m_chkSet = FALSE;
	//}}AF    X_DATA_INIT

	m_OldSize = CSize(-1,-1);
}

CGridDlg::CGridDlg(IMgaFCOs* selectedObjs, CWnd* pParent /*=NULL*/)
	: CDialog(CGridDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGridDlg)
	m_chkAllKinds = TRUE;
	m_chkAllTypes = TRUE;
	m_chkAtom = FALSE;
	m_chkCon = FALSE;
	m_chkModel = FALSE;
	m_chkRef = FALSE;
	m_chkSet = FALSE;
	//}}AFX_DATA_INIT

	m_OldSize = CSize(-1,-1);

	m_FCOs = selectedObjs;

	

}
	

BOOL CGridDlg::OnInitDialog()
{
	BOOL val = CDialog::OnInitDialog();
	
	CComPtr<IMgaMetaProject> m_MetaProject;
	COMTHROW(m_Project->get_RootMeta(&m_MetaProject));

	
	COMTHROW(m_MetaProject->get_RootFolder(&m_rootMetaFolder));

	GetMetaObjectNames(m_rootMetaFolder);

	OnCheckAllKinds();
	OnChkAllTypes();

	long count;
	COMTHROW(m_FCOs->get_Count(&count));

	if(count>0)
	{
		m_btnAtom.EnableWindow(FALSE);
		m_btnModel.EnableWindow(FALSE);
		m_btnRef.EnableWindow(FALSE);
		m_btnSet.EnableWindow(FALSE);
		m_btnCon.EnableWindow(FALSE);
		m_btnDisp.EnableWindow(FALSE);
		m_btnAllKinds.EnableWindow(FALSE);
		m_btnAllTypes.EnableWindow(FALSE);
		m_lstKind.EnableWindow(FALSE);
		m_stcKind.EnableWindow(FALSE);
		m_stcType.EnableWindow(FALSE);
		m_stcHelp.EnableWindow(FALSE);
		m_stcFilters.EnableWindow(FALSE);

		/*
		m_btnAtom.ShowWindow(FALSE);
		m_btnModel.ShowWindow(FALSE);
		m_btnRef.ShowWindow(FALSE);
		m_btnSet.ShowWindow(FALSE);
		m_btnCon.ShowWindow(FALSE);
		m_btnDisp.ShowWindow(FALSE);
		m_btnAllKinds.ShowWindow(FALSE);
		m_btnAllTypes.ShowWindow(FALSE);
		m_lstKind.ShowWindow(FALSE);
		m_stcKind.ShowWindow(FALSE);
		m_stcType.ShowWindow(FALSE);
		m_stcHelp.ShowWindow(FALSE);
		m_stcFilters.ShowWindow(FALSE);
		*/

		InitGrid();
	}
	else
		OnButtonDisplay();

	return val;

}

void CGridDlg::InitGrid() //really long function to init the grid, in the future, may want to 
{                             //separate out code into subfunctions

	

	CRect rect;
	GetClientRect(rect);
	m_OldSize = CSize(rect.Width(), rect.Height());

	//m_Grid.AutoSize();

	m_Grid.DeleteAllItems();
	m_Grid.SetRowCount(0);
	m_Grid.SetColumnCount(0);

	if(m_FCOs == NULL)//if nothing was selected for the table editor invocation.
		return;




	long count;
	//int ColOffset = 5; //offset until attributes start, to make it easy to add columns (extended name, etc)

	COMTHROW(m_FCOs->get_Count(&count));


	
	//m_Grid.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));
	m_Grid.SetRowCount(count+1); //add one for the header row
	m_Grid.SetColumnCount(COLOFFSET + 2);//room for one attribute, will get updated if we need more
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(1);
	m_Grid.EnableSelection(FALSE);
	//m_Grid.SetEditable(FALSE);
	m_Grid.SetHeaderSort(); 

	m_Grid.SetItemText(0,KIND_COL,"Object Kind");
	m_Grid.SetItemText(0,NAME_COL,"Object Name");
	m_Grid.SetItemText(0,ID_COL,"Object ID");
	m_Grid.SetItemText(0,EXTNAME_COL,"Extended Name");
	m_Grid.SetItemText(0,INSTANCE_COL,"Is Instance");
	m_Grid.SetItemText(0,SRC_COL, "Source");
	m_Grid.SetItemText(0,DST_COL, "Destination");

	m_Grid.SetItemText(0,COLOFFSET,"Attribute");
	m_Grid.SetItemText(0,COLOFFSET+1,"Value");

	

	int ColNum = COLOFFSET;
	int RowNum = 1;

	MGACOLL_ITERATE(IMgaFCO,m_FCOs)
	{
		CBstr Type;
		CBstr Name;

		//Display the Kind Name
		CComPtr<IMgaMetaFCO> cmeta; 
		COMTHROW( MGACOLL_ITER->get_Meta(&cmeta) );
		COMTHROW( cmeta->get_DisplayedName(Type) );
		
		CString kindName = Type;
		m_Grid.SetItemText(RowNum,KIND_COL,kindName);

		//Display the Name
		if(m_Grid.SetCellType(RowNum,NAME_COL, RUNTIME_CLASS(CGridCellFCO)))
		{
			CGridCellFCO *pCellFCO = (CGridCellFCO *)m_Grid.GetCell(RowNum,1);
			pCellFCO->SetFCO(MGACOLL_ITER);
		}
		COMTHROW(MGACOLL_ITER->get_Name(Name));
		CString name_str = Name;
		m_Grid.SetItemText(RowNum,NAME_COL,name_str);
		if(RowNum%2==0)
			m_Grid.GetCell(RowNum,NAME_COL)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));

		//Display the ID
		CBstr ID;
		COMTHROW(MGACOLL_ITER->get_ID(ID));
		CString ObjID = ID;
		m_Grid.SetItemText(RowNum,ID_COL,ObjID);
		m_Grid.SetItemState(RowNum,ID_COL, m_Grid.GetItemState(RowNum,2) | GVIS_READONLY);
		if(RowNum%2==0)
			m_Grid.GetCell(RowNum,ID_COL)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));

		//Display the Extended Name
		CString ExtName;
		BuildExtendedName(MGACOLL_ITER,ExtName);
		m_Grid.SetItemText(RowNum,EXTNAME_COL,ExtName);
		m_Grid.SetItemState(RowNum,EXTNAME_COL, m_Grid.GetItemState(RowNum,2) | GVIS_READONLY);
		if(RowNum%2==0)
			m_Grid.GetCell(RowNum,EXTNAME_COL)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));

		//Display if it's an instance
		short bInstance;
		m_Grid.SetItemState(RowNum,INSTANCE_COL, m_Grid.GetItemState(RowNum,2) | GVIS_READONLY);
		COMTHROW(MGACOLL_ITER->get_IsInstance(&bInstance));
		if(bInstance)
			m_Grid.SetItemText(RowNum,INSTANCE_COL, "true");
		else
			m_Grid.SetItemText(RowNum,INSTANCE_COL, "false");
		if(RowNum%2==0)
			m_Grid.GetCell(RowNum,INSTANCE_COL)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));

		//Display Source and Destination if it's a connection
		m_Grid.SetItemState(RowNum, SRC_COL, m_Grid.GetItemState(RowNum,2) | GVIS_READONLY);
		m_Grid.SetItemState(RowNum, DST_COL, m_Grid.GetItemState(RowNum,2) | GVIS_READONLY);
		CBstr ID1, ID2;	
		objtype_enum tp;
		COMTHROW(MGACOLL_ITER->get_ObjType(&tp));
	
		if(tp==OBJTYPE_CONNECTION) {
			CComPtr<IMgaFCO> src, dst;
			CComPtr<IMgaSimpleConnection> conn;
			COMTHROW(MGACOLL_ITER.QueryInterface(&conn));
			conn->get_Src(&src);
			conn->get_Dst(&dst);
			CString SrcExtName;
			CString DstExtName;
			BuildExtendedName(src,SrcExtName);
			BuildExtendedName(dst,DstExtName);
			m_Grid.SetItemText(RowNum,SRC_COL, SrcExtName); 
			m_Grid.SetItemText(RowNum,DST_COL, DstExtName);
		}
		else {
			m_Grid.SetItemText(RowNum,SRC_COL, "N/A"); 
			m_Grid.SetItemText(RowNum,DST_COL, "N/A"); 
		}
		if(RowNum%2==0) {
			m_Grid.GetCell(RowNum,SRC_COL)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));
			m_Grid.GetCell(RowNum,DST_COL)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));
		}


		//Fill in the attributes
		CComPtr<IMgaAttributes> p_AttributeList;
		COMTHROW(MGACOLL_ITER->get_Attributes(&p_AttributeList));

		long AttribCount; //see if we need more room on the grid, if so, make it
		int ColCount = m_Grid.GetColumnCount();
		COMTHROW(p_AttributeList->get_Count(&AttribCount));
		if(((AttribCount*2)+COLOFFSET) > ColCount)
		{
			m_Grid.SetColumnCount((AttribCount*2)+COLOFFSET);
			for(int i=ColCount; i<((AttribCount*2)+COLOFFSET-1); i++)
			{
				m_Grid.SetItemText(0,i,"Attribute");
				i++;
				m_Grid.SetItemText(0,i,"Value");
				
				//set all the new cells for previous rows read only
				for(int j=1; j<RowNum; j++)
				{
					m_Grid.SetItemState(j,i-1, m_Grid.GetItemState(j,i-1) | GVIS_READONLY);
					m_Grid.SetItemState(j,i, m_Grid.GetItemState(j,i) | GVIS_READONLY);
					if(j%2==0)
					{
						m_Grid.GetCell(j,i-1)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));
						m_Grid.GetCell(j,i)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));
					}
				}

			} 
		}

		MGACOLL_ITERATE(IMgaAttribute,p_AttributeList)
		{
			if(RowNum%2==0)
				m_Grid.GetCell(RowNum,ColNum)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));

			CComPtr<IMgaMetaAttribute> p_MetaAtt;
			COMTHROW(MGACOLL_ITER->get_Meta(&p_MetaAtt));
			
			CBstr AttName;
			CBstr AttDisplayedName;
			COMTHROW(p_MetaAtt->get_Name(AttName));
			COMTHROW(p_MetaAtt->get_DisplayedName(AttDisplayedName));

			//have to do this silly madness to make the displayedname displayed
			//but remember the real name for accessing attributes onEndEdit()
			if (m_Grid.SetCellType(RowNum,ColNum, RUNTIME_CLASS(CGridCellAttrName)))
			{
				CGridCellAttrName *pCellAttrName = (CGridCellAttrName *)m_Grid.GetCell(RowNum,ColNum);
				pCellAttrName->SetRealName(CString(AttName));
			}
			m_Grid.SetItemText(RowNum,ColNum,CString(AttDisplayedName));
			m_Grid.SetItemState(RowNum,ColNum, m_Grid.GetItemState(RowNum,ColNum) | GVIS_READONLY);
			ColNum++;

			if(RowNum%2==0)
				m_Grid.GetCell(RowNum,ColNum)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));

			attval_enum ValType;
			COMTHROW(p_MetaAtt->get_ValueType(&ValType));
			int actval_int;
			double actval_dbl;
			bool actval_b;
			CString actval_str;

			CStringArray options;
			CGridCellCombo *pCell;
			
			CComPtr<IMgaMetaEnumItems> p_MetaEnumItems;

			CBstr bstrS;
			long l;
			VARIANT_BOOL vb;
			
			switch (ValType)
			{

				case ATTVAL_NULL:
					break;
				case ATTVAL_STRING:
					COMTHROW( MGACOLL_ITER->get_StringValue(bstrS)); 
					actval_str = bstrS;
					if(GetMultiLine(p_MetaAtt))
					{
						if (!m_Grid.SetCellType(RowNum,ColNum, RUNTIME_CLASS(CGridCellMultiLine)))
							break;
						m_Grid.GetCell(RowNum,ColNum)->SetFormat(DT_WORDBREAK);
						
						actval_str.Replace("\n","\r\n");
						//actval_str.Replace("\"","'"); // needed in order for attributes like "four" to work ...
					}
					m_Grid.SetItemText(RowNum,ColNum,actval_str);
					break;
				case ATTVAL_INTEGER:
					if(SUCCEEDED(MGACOLL_ITER->get_IntValue(&l)))
					{
						actval_int = l;
						if (!m_Grid.SetCellType(RowNum,ColNum, RUNTIME_CLASS(CGridCellNumeric)))
							break;
						char chNum[ 100 ];
						sprintf( chNum, "%d", actval_int );
						actval_str = chNum;
						//numeric cell still passes in and out as a string, but 
						//adds ES_NUMBER to the edit style, so can only type in numbers
						m_Grid.SetItemText(RowNum,ColNum,actval_str);
					}
					break;
				case ATTVAL_DOUBLE:
					if(SUCCEEDED( MGACOLL_ITER->get_FloatValue(&actval_dbl )))
					{
						//AfxMessageBox("Double: "+AttName);
						if (!m_Grid.SetCellType(RowNum,ColNum, RUNTIME_CLASS(CGridCellDouble)))
							break;
						char chNum[ 100 ];
						sprintf( chNum, "%f", actval_dbl );
						actval_str = chNum;
						m_Grid.SetItemText(RowNum,ColNum,actval_str);
					}
					break;
				case ATTVAL_BOOLEAN:
					if(SUCCEEDED( MGACOLL_ITER->get_BoolValue(&vb) ) )
					{
						actval_b = (vb != 0);
						if (!m_Grid.SetCellType(RowNum, ColNum, RUNTIME_CLASS(CGridCellComboBool)))
							break;

						options.Add(_T("true"));
						options.Add(_T("false"));

						pCell = (CGridCellComboBool*) m_Grid.GetCell(RowNum,ColNum);
						pCell->SetOptions(options);
						//CBS_DROPDOWN, CBS_DROPDOWNLIST, CBS_SIMPLE
						pCell->SetStyle(pCell->GetStyle() | CBS_DROPDOWNLIST);

						if(actval_b)
						{
							m_Grid.SetItemText(RowNum,ColNum, "true");

						}
						else
							m_Grid.SetItemText(RowNum,ColNum,"false");

					}
					break;
				case ATTVAL_REFERENCE:
					//AfxMessageBox("Reference Attribute");
					m_Grid.SetItemText(RowNum,ColNum,"Reference Attribute");
					m_Grid.SetItemState(RowNum,ColNum, m_Grid.GetItemState(RowNum,ColNum) | GVIS_READONLY);
					break;
				case ATTVAL_ENUM:
					//AfxMessageBox("Enum Attribute");

					if (!m_Grid.SetCellType(RowNum, ColNum, RUNTIME_CLASS(CGridCellCombo)))
							break;

					COMTHROW(p_MetaAtt->get_EnumItems(&p_MetaEnumItems));
					MGACOLL_ITERATE(IMgaMetaEnumItem,p_MetaEnumItems)
					{
						
						CBstr EnumItemVal;
						COMTHROW(MGACOLL_ITER->get_Value(EnumItemVal));

						options.Add(EnumItemVal);

					}MGACOLL_ITERATE_END

					pCell = (CGridCellCombo*) m_Grid.GetCell(RowNum,ColNum);
					pCell->SetOptions(options);
					//CBS_DROPDOWN, CBS_DROPDOWNLIST, CBS_SIMPLE
					pCell->SetStyle(pCell->GetStyle() | CBS_DROPDOWNLIST);

					COMTHROW( MGACOLL_ITER->get_StringValue(bstrS)); 
					actval_str = bstrS;
					m_Grid.SetItemText(RowNum,ColNum,actval_str);
					break;
				case ATTVAL_DYNAMIC:
					//AfxMessageBox("Dynamic Attribute");
					m_Grid.SetItemText(RowNum,ColNum,"Dynamic Attribute");
					m_Grid.SetItemState(RowNum,ColNum, m_Grid.GetItemState(RowNum,ColNum) | GVIS_READONLY);
					break;
				default:
					break;
			}

			ColNum++;


		}MGACOLL_ITERATE_END;

		//if there's more columns on this row that won't have attributes,
		//set them to be read only
		for(int k=ColNum; k<m_Grid.GetColumnCount();k++)
		{
			m_Grid.SetItemState(RowNum,k, m_Grid.GetItemState(RowNum,k) | GVIS_READONLY);
			if(RowNum%2==0)
				m_Grid.GetCell(RowNum,k)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));
		}

		ColNum = COLOFFSET;
		RowNum++;

	}MGACOLL_ITERATE_END;	
}

HRESULT CGridDlg::ShowItemsRecursively()
{
	COMTRY {
		std::queue< CComPtr<IMgaFCO> > fcoq;

		CComPtr<IMgaFCOs> recursiveFCOs = NULL;
		COMTHROW(recursiveFCOs.CoCreateInstance(L"Mga.MgaFCOs"));

		long count = 0;
		COMTHROW(this->m_FCOs->get_Count(&count));

		for (int idx = 1; idx <= count; idx++) {
			CComPtr<IMgaFCO> fco;
			COMTHROW(this->m_FCOs->get_Item(idx, &fco));
			fcoq.push(fco);
		}

		while (! fcoq.empty()) {
			CComPtr<IMgaFCO> parent = fcoq.front();
			fcoq.pop();

			COMTHROW(recursiveFCOs->Append(parent));

			objtype_enum type;
			COMTHROW(parent->get_ObjType(&type));
			if (type != OBJTYPE_MODEL && type != OBJTYPE_FOLDER)
				continue;

			CComPtr<IMgaObjects> children = NULL;
			COMTHROW(parent->get_ChildObjects(&children));

			long count = 0;
			COMTHROW(children->get_Count(&count));

			for (int idx = 1; idx <= count; idx++) {
				CComPtr<IMgaObject> child;
				COMTHROW(children->get_Item(idx, &child));

				objtype_enum type = OBJTYPE_NULL;
				COMTHROW(child->get_ObjType(&type));

				switch (type) {
				case OBJTYPE_MODEL:
				case OBJTYPE_ATOM:
				case OBJTYPE_REFERENCE:
				case OBJTYPE_CONNECTION:
				case OBJTYPE_SET:
					CComPtr<IMgaFCO> fco = NULL;
					fco = static_cast<IMgaFCO*>(child.p);
					fcoq.push(fco);
					break;
				}
			}
		}

		this->m_FCOs = recursiveFCOs;
		this->InitGrid();

		return S_OK;
	}
	COMCATCH(;);
}

void CGridDlg::GetMetaObjectNames(IMgaMetaBase *metaBase) //based on GetMetaObjectsR in OCLCommonEx.cpp
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
				//need to change this to DisplayedName somehow..
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

void CGridDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGridDlg)
	DDX_Control(pDX, IDC_BTNIMPORT, m_btnImport);
	DDX_Control(pDX, IDC_STATIC_FILTERS, m_stcFilters);
	DDX_Control(pDX, IDC_STATIC_SELECTION, m_stcSelect);
	DDX_Control(pDX, IDC_CHKALLTYPES, m_btnAllTypes);
	DDX_Control(pDX, IDC_CHECKALLKINDS, m_btnAllKinds);
	DDX_Control(pDX, IDC_STATIC_TYPE, m_stcType);
	DDX_Control(pDX, IDC_STATIC_KIND, m_stcKind);
	DDX_Control(pDX, IDC_STATICHELP, m_stcHelp);
	DDX_Control(pDX, IDC_BTNDISP, m_btnDisp);
	DDX_Control(pDX, IDC_CHKSET, m_btnSet);
	DDX_Control(pDX, IDC_CHKREF, m_btnRef);
	DDX_Control(pDX, IDC_CHKMODEL, m_btnModel);
	DDX_Control(pDX, IDC_CHKCON, m_btnCon);
	DDX_Control(pDX, IDC_CHKATOM, m_btnAtom);
	DDX_Control(pDX, IDC_LISTKIND, m_lstKind);
	DDX_Control(pDX, IDC_BTNEXPORT, m_btnExport);
	DDX_Control(pDX, IDCANCEL, m_btnCANCEL);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Check(pDX, IDC_CHECKALLKINDS, m_chkAllKinds);
	DDX_Check(pDX, IDC_CHKALLTYPES, m_chkAllTypes);
	DDX_Check(pDX, IDC_CHKATOM, m_chkAtom);
	DDX_Check(pDX, IDC_CHKCON, m_chkCon);
	DDX_Check(pDX, IDC_CHKMODEL, m_chkModel);
	DDX_Check(pDX, IDC_CHKREF, m_chkRef);
	DDX_Check(pDX, IDC_CHKSET, m_chkSet);
	DDX_Control(pDX, IDC_GRID, m_Grid);             // associate the grid window with a C++ object
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGridDlg, CDialog)
	//{{AFX_MSG_MAP(CGridDlg)
	ON_COMMAND(ID_VIEW_RECURSIVELYSHOWITEMS, OnRecursivelyShowItems)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTNDISP, OnButtonDisplay)
	ON_BN_CLICKED(IDC_CHECKALLKINDS, OnCheckAllKinds)
	ON_BN_CLICKED(IDC_CHKALLTYPES, OnChkAllTypes)
	ON_BN_CLICKED(IDC_CHKATOM, OnChkAtom)
	ON_BN_CLICKED(IDC_CHKCON, OnChkCon)
	ON_BN_CLICKED(IDC_CHKMODEL, OnChkModel)
	ON_BN_CLICKED(IDC_CHKREF, OnChkRef)
	ON_BN_CLICKED(IDC_CHKSET, OnChkSet)
	//}}AFX_MSG_MAP
    ON_NOTIFY(NM_DBLCLK, IDC_GRID, OnGridDblClick)
    ON_NOTIFY(NM_CLICK, IDC_GRID, OnGridClick)
	ON_NOTIFY(NM_RCLICK, IDC_GRID, OnGridRClick)
	ON_NOTIFY(GVN_BEGINLABELEDIT, IDC_GRID, OnGridStartEdit)
	ON_NOTIFY(GVN_ENDLABELEDIT, IDC_GRID, OnGridEndEdit)
	ON_NOTIFY(GVN_SELCHANGING, IDC_GRID, OnGridStartSelChange)
	ON_NOTIFY(GVN_SELCHANGED, IDC_GRID, OnGridEndSelChange)
	ON_NOTIFY(GVN_BEGINDRAG, IDC_GRID, OnGridBeginDrag)
	ON_NOTIFY(NM_RCLICK, IDC_GRID, OnGridRClick)
	ON_NOTIFY(NM_RCLICK, IDC_GRID, OnGridRClick)
	ON_NOTIFY(NM_RCLICK, IDC_GRID, OnGridRClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGridDlg message handlers

void CGridDlg::SetFilter(IMgaFilter *filter)
{
	m_Filter = filter;
}


/* void CGridDlg::Trace(LPCTSTR szFmt, ...)
{    
	CString str;

	// Format the message text
    va_list argptr;
    va_start(argptr, szFmt);
    str.FormatV(szFmt, argptr);
    va_end(argptr);

	str.Replace(_T("\n"), _T("\r\n"));

	CString strWndText;
	m_TraceWnd.GetWindowText(strWndText);
	strWndText += str;
	m_TraceWnd.SetWindowText(strWndText);

//	m_TraceWnd.SetSel(str.GetLength()-1, str.GetLength()-2, FALSE);
	m_TraceWnd.LineScroll(-m_TraceWnd.GetLineCount());
	m_TraceWnd.LineScroll(m_TraceWnd.GetLineCount()-4);
} */





// NM_RCLICK
void CGridDlg::OnGridRClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
//    NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
//    Trace(_T("Right button click on row %d, col %d\n"), pItem->iRow, pItem->iColumn);
}

// GVN_BEGINLABELEDIT
void CGridDlg::OnGridStartEdit(NMHDR *pNotifyStruct, LRESULT* pResult)
{
//    NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
//    Trace(_T("Start Edit on row %d, col %d\n"), pItem->iRow, pItem->iColumn);
	//*pResult = (m_bRejectEditAttempts)? -1 : 0;
}

// GVN_ENDLABELEDIT
void CGridDlg::OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult)
{
    NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
//    Trace(_T("End Edit on row %d, col %d\n"), pItem->iRow, pItem->iColumn);
	//*pResult = (m_bRejectEditChanges)? -1 : 0;


	long RowNum = pItem->iRow;
	long ColNum = pItem->iColumn;
	long count;

	CComPtr<IMgaFCO> obj;
	CString objNameCellClass = m_Grid.GetCell(RowNum,1)->GetRuntimeClass()->m_lpszClassName;
	if(objNameCellClass == "CGridCellFCO")
	{
		CGridCellFCO *pCellFCO = (CGridCellFCO *)m_Grid.GetCell(RowNum,1);
		obj = pCellFCO->GetFCO();
	}
	else //something is wrong
	{
		ASSERT(objNameCellClass == "CGridCellFCO");
		return;
	}

	if(pItem->iColumn == 1)//Name column
	{
		
		COMTHROW(m_FCOs->get_Count(&count));

		if((RowNum > 0) && (RowNum <= count))
		{

			COMTHROW(obj->put_Name(CComBSTR(m_Grid.GetItemText(RowNum,1))));

		}

	}
	if(pItem->iColumn >COLOFFSET /*ColOffset from above */)//all attributes other than name
	{
		//AfxMessageBox("Trying to edit an attribute");

		//get the name of the attribute
		CString AttName;
		CGridCellAttrName *pCellAttrName = (CGridCellAttrName *)m_Grid.GetCell(RowNum,ColNum-1);
		if(!pCellAttrName) return; //shouldn't happen, need to think of an ASSERT to check
		AttName = pCellAttrName->GetRealName();



		CString ClassName = m_Grid.GetCell(RowNum,ColNum)->GetRuntimeClass()->m_lpszClassName;
		//AfxMessageBox(ClassName);
		if(ClassName == "CGridCellComboBool")
		{
			CGridCellComboBool *pCell = (CGridCellComboBool *)(m_Grid.GetCell(RowNum,ColNum));
			CString value = pCell->GetText();
			if(value == "true")
			{
				BOOL val = TRUE;
				COMTHROW(obj->put_BoolAttrByName(CComBSTR(AttName),val));
			}
			else if(value == "false")
			{
				BOOL val = FALSE;
				COMTHROW(obj->put_BoolAttrByName(CComBSTR(AttName),val));
			}
		}

		if(ClassName == "CGridCellCombo")
		{
			//AfxMessageBox(m_Grid.GetItemText(RowNum,ColNum));
			COMTHROW(obj->put_StrAttrByName(CComBSTR(AttName),CComBSTR(m_Grid.GetItemText(RowNum,ColNum))));
		}

		if(ClassName == "CGridCellNumeric")
		{
			//AfxMessageBox(m_Grid.GetItemText(RowNum,ColNum));
			COMTHROW(obj->put_IntAttrByName(CComBSTR(AttName),atol((m_Grid.GetItemText(RowNum,ColNum)))));
		}

		if(ClassName == "CGridCellDouble")
		{
			//AfxMessageBox(m_Grid.GetItemText(RowNum,ColNum));
			COMTHROW(obj->put_FloatAttrByName(CComBSTR(AttName),atof((m_Grid.GetItemText(RowNum,ColNum)))));
		}

		if(ClassName == "CGridCellMultiLine")
		{
			CString outstring = m_Grid.GetItemText(RowNum,ColNum);
			outstring.Replace("\r\n","\n");

			COMTHROW(obj->put_StrAttrByName(CComBSTR(AttName),CComBSTR(outstring)));
		}

		if(ClassName == "CGridCell")
		{

			COMTHROW(obj->put_StrAttrByName(CComBSTR(AttName),CComBSTR(m_Grid.GetItemText(RowNum,ColNum))));
		}


	}
}

// GVN_SELCHANGING
void CGridDlg::OnGridStartSelChange(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
//    NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
//    Trace(_T("Start Selection Change on row %d, col %d (%d Selected)\n"), 
//		pItem->iRow, pItem->iColumn, m_Grid.GetSelectedCount());
}

// GVN_SELCHANGED
void CGridDlg::OnGridEndSelChange(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
//    NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
//    Trace(_T("End Selection Change on row %d, col %d (%d Selected)\n"), 
//		pItem->iRow, pItem->iColumn, m_Grid.GetSelectedCount());
}

// GVN_BEGINDRAG
void CGridDlg::OnGridBeginDrag(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
//    NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
//    Trace(_T("Begin Drag row %d, col %d\n"), pItem->iRow, pItem->iColumn);
}

void CGridDlg::OnGridDblClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
//   NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
//    Trace(_T("Double Clicked on row %d, col %d\n"), pItem->iRow, pItem->iColumn);
}

void CGridDlg::OnGridClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{

	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

	if(pItem->iRow == 0) //redo the background colors if sorting
	{
		int MaxRows = m_Grid.GetRowCount();
		int MaxCols = m_Grid.GetColumnCount();
		for (int i=1; i<MaxRows; i++)
		{
			for (int j=1; j<MaxCols; j++)
			{
				if(i%2==0)
					m_Grid.GetCell(i,j)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));
				else
					m_Grid.GetCell(i,j)->SetBackClr(RGB(0xFF, 0xFF, 0xFF));
			}

		}
		Invalidate();
	}
	

//    Trace(_T("Clicked on row %d, col %d\n"), pItem->iRow, pItem->iColumn);
}


// BuildExtendedName functions copied and paste out of GME Search
// recursively build a path of names from the rootFCO to "named" used in the original call
void CGridDlg::BuildExtendedName(IMgaFCO *named, CString &extName)
{
	
	if (named != NULL)
	{
		CComPtr<IMgaModel> parent;
		COMTHROW( named->get_ParentModel(&parent));
		if (parent != NULL)
		{
			BuildExtendedName(parent, extName);
			extName += " : ";
		}
		else //parent is a Folder
		{
			CComPtr<IMgaFolder> parentFolder = NULL;
			COMTHROW( named->get_ParentFolder(&parentFolder));
			BuildExtendedName(parentFolder, extName);
			extName += " : ";
		}
		CBstr bstr;
		COMTHROW( named->get_Name(bstr));
		extName += bstr;
	}

}

// Get the parent folders' names, too
void CGridDlg::BuildExtendedName(IMgaFolder *named, CString &extName)
{
	if (named != NULL)
	{
		CComPtr<IMgaFolder> parent;
		COMTHROW( named->get_ParentFolder(&parent));
		if (parent != NULL)
		{
			BuildExtendedName(parent, extName);
			extName += " : ";
		}
		CBstr bstr;
		COMTHROW( named->get_Name(bstr));
		extName += bstr;
	}
}

void CGridDlg::OnSize(UINT nType, int cx, int cy) 
{
	if(!(::IsWindow(m_hWnd)))
		return;
	CDialog::OnSize(nType, cx, cy);
	
	if (cx <= 1 || cy <= 1 ) 
        return;

	CSize Translate(cx - m_OldSize.cx, cy - m_OldSize.cy);


	//Expand or contract the Grid
	CRect rect;
	m_Grid.GetWindowRect(rect);

	ScreenToClient(rect);

	if (  ((rect.top >= 7 && Translate.cy > 0) || rect.Height() > 20) &&
		((rect.left >= 7 && Translate.cx > 0) || rect.Width() > 20)   )
		m_Grid.MoveWindow(rect.left, rect.top, 
					 rect.Width()+Translate.cx, 
					rect.Height()+Translate.cy, FALSE);
	else
		m_Grid.MoveWindow(rect.left+Translate.cx, rect.top+Translate.cy, 
						rect.Width(), rect.Height(), FALSE);


	//Just move the buttons down
	MoveWndDown(&m_btnOK, Translate.cy);

	MoveWndDown(&m_btnCANCEL, Translate.cy);

	MoveWndDown(&m_btnExport, Translate.cy);

	MoveWndDown(&m_btnDisp, Translate.cy);

	MoveWndDown(&m_btnAllKinds, Translate.cy);

	MoveWndDown(&m_btnAllTypes, Translate.cy);

	MoveWndDown(&m_btnAtom, Translate.cy);

	MoveWndDown(&m_btnModel, Translate.cy);

	MoveWndDown(&m_btnRef, Translate.cy);

	MoveWndDown(&m_btnCon, Translate.cy);

	MoveWndDown(&m_btnSet, Translate.cy);

	MoveWndDown(&m_stcKind, Translate.cy);

	MoveWndDown(&m_stcType, Translate.cy);

	MoveWndDown(&m_stcHelp, Translate.cy);

	MoveWndDown(&m_lstKind, Translate.cy);

	MoveWndDown(&m_stcSelect, Translate.cy);

	MoveWndDown(&m_stcFilters, Translate.cy);

	MoveWndDown(&m_btnImport, Translate.cy);

	Invalidate();

	m_OldSize = CSize(cx,cy);

	
	
}

void CGridDlg::MoveWndDown(CWnd *wnd, int offset)
{
 CRect rect;
	wnd->GetWindowRect(rect);
	ScreenToClient(rect);
	wnd->MoveWindow(rect.left, rect.top+offset, rect.Width(), rect.Height(), FALSE);
}

void CGridDlg::OnRecursivelyShowItems()
{
	ShowItemsRecursively();
}

	
BOOL CGridDlg::GetMultiLine(CComPtr<IMgaMetaAttribute> p_Meta) 
{
	//determine if this is a multiline attribute
	int multiline;
	CComBSTR attrPref;
	CComBSTR attrPrefPath = "multiLine";
	COMTHROW(p_Meta->get_RegistryValue(attrPrefPath, &attrPref));
	CString lines(attrPref);
	multiline = atoi(lines); //returns 0 if it can't convert to int
	return multiline > 1;
}

void CGridDlg::OnButtonDisplay() 
{
	if(CWnd::UpdateData(TRUE))
	{

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

		m_Filter = NULL;

		COMTHROW(m_Project->CreateFilter(&m_Filter));
		COMVERIFY(m_Filter->put_ObjType(CComBSTR(m_ObjType)));
		COMVERIFY(m_Filter->put_Kind(CComBSTR(m_kindsFiltered)));
		
		m_FCOs = NULL;
	
		COMVERIFY(m_Project->AllFCOs(m_Filter, &m_FCOs));


		InitGrid();
	}	
}

void CGridDlg::OnCheckAllKinds() 
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

void CGridDlg::OnChkAllTypes() 
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



void CGridDlg::OnChkAtom() 
{
	CWnd::UpdateData(TRUE);
	m_lstKind.ResetContent();
	GetMetaObjectNames(m_rootMetaFolder);
	
}

void CGridDlg::OnChkCon() 
{
	CWnd::UpdateData(TRUE);
	m_lstKind.ResetContent();
	GetMetaObjectNames(m_rootMetaFolder);
	
}

void CGridDlg::OnChkModel() 
{
	CWnd::UpdateData(TRUE);
	m_lstKind.ResetContent();
	GetMetaObjectNames(m_rootMetaFolder);
	
}

void CGridDlg::OnChkRef() 
{
	CWnd::UpdateData(TRUE);
	m_lstKind.ResetContent();
	GetMetaObjectNames(m_rootMetaFolder);
	
}

void CGridDlg::OnChkSet() 
{
	CWnd::UpdateData(TRUE);
	m_lstKind.ResetContent();
	GetMetaObjectNames(m_rootMetaFolder);
	
}


