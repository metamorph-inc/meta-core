// UIPropEditor.cpp : implementation file
//

#include "stdafx.h"
#include "UIPropEditor.h"
#include "UI.h"

// UIPropEditor

IMPLEMENT_DYNAMIC(UIPropEditor, CWnd)

UIPropEditor::UIPropEditor()
{

}

UIPropEditor::~UIPropEditor()
{
}


BEGIN_MESSAGE_MAP(UIPropEditor, CWnd)
//	ON_WM_CLOSE()
END_MESSAGE_MAP()



// UIPropEditor message handlers




BOOL UIPropEditor::initialize(CWnd* pParentWnd, CRect rect)
{
	// initialize Caption
	CString captionText;
	CRect captionRect(rect.left, rect.top, rect.right, rect.top + UI::CAPTION_HEIGHT);	// Caption Box Area
	CRect editorRect(rect.left, captionRect.bottom, rect.right, rect.bottom);		// Display Box Area
	// Contained Property View Bars Rect Areas
	CRect rowPortNameBarRect(0, 0, editorRect.right, BAR_HEIGHT);
	CRect rowPortTypeBarRect(0, BAR_HEIGHT, editorRect.right, 2 * BAR_HEIGHT);
	CRect colPortNameBarRect(0, 2 * BAR_HEIGHT, editorRect.right, 3 * BAR_HEIGHT);
	CRect colPortTypeBarRect(0, 3 * BAR_HEIGHT, editorRect.right, 4 * BAR_HEIGHT);

	// Create Caption as the CStatic
	captionText.LoadString(PROPERTY_EDITOR_CAP_TEXT);
	if(!this->caption.Create(captionText, 
							WS_CHILD | WS_VISIBLE | WS_BORDER, 
							captionRect,
							pParentWnd,
							PROPERTY_EDITOR_CAPTION))
	{
		AfxMessageBox(_T("Failed to Create \"") + captionText + _T("\"'s Caption Box! Closing."));
		return FALSE;
	}
	
	// Register UIPropEditor Class
	WNDCLASS wndcls;
    wndcls.style            = CS_DBLCLKS;
    wndcls.lpfnWndProc      = ::DefWindowProc;
    wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
    wndcls.hInstance        = AfxGetInstanceHandle();
    wndcls.hIcon            = NULL;
    wndcls.hCursor          = LoadCursor(wndcls.hInstance, MAKEINTRESOURCE(IDC_ARROW));
	wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1); 
    wndcls.lpszMenuName     = NULL;
    wndcls.lpszClassName    = _T("MFCUIPropEditor");

    if (!AfxRegisterClass(&wndcls))
    {
		AfxMessageBox(_T("Failed to Register") + CString(wndcls.lpszClassName) + _T(" Closing."));
        return FALSE;
    }

	if(!Create(CString(wndcls.lpszClassName), NULL, WS_CHILD | WS_VISIBLE | WS_DLGFRAME | WS_TABSTOP /* | WS_VSCROLL*/, editorRect, pParentWnd, PROPERTY_EDITOR))
	{
		AfxMessageBox(_T("Failed to Create \"") + captionText + _T("\"'s Display Box! Closing."));
		return FALSE;
	}
	

	if(!this->rowPortNameBar.intialize(this,
									   rowPortNameBarRect,
									   PROPERTY_EDITORBAR_ROW_NAME_CAPTION,
									   PROPERTY_EDITORBAR_ROW_NAME_CAP_TEXT, 
									   PROPERTY_EDITORBAR_ROW_NAME_CONTENTBOX,
									   FALSE,
									   FALSE))
		return FALSE;

	if(!this->rowPortTypeBar.intialize(this,
									   rowPortTypeBarRect,
									   PROPERTY_EDITORBAR_ROW_TYPE_CAPTION,
									   PROPERTY_EDITORBAR_ROW_TYPE_CAP_TEXT, 
									   PROPERTY_EDITORBAR_ROW_TYPE_CONTENTBOX,
									   FALSE,
									   TRUE))
		return FALSE;

	if(!this->colPortNameBar.intialize(this,
									   colPortNameBarRect,
									   PROPERTY_EDITORBAR_COL_NAME_CAPTION,
									   PROPERTY_EDITORBAR_COL_NAME_CAP_TEXT, 
									   PROPERTY_EDITORBAR_COL_NAME_CONTENTBOX,
									   FALSE,
									   FALSE)) 
		return FALSE;

	if(!this->colPortTypeBar.intialize(this,
									   colPortTypeBarRect,
									   PROPERTY_EDITORBAR_COL_TYPE_CAPTION,
									   PROPERTY_EDITORBAR_COL_TYPE_CAP_TEXT, 
									   PROPERTY_EDITORBAR_COL_TYPE_CONTENTBOX,
									   FALSE,
									   TRUE)) 
		return FALSE;

	return TRUE;
}

void UIPropEditor::observe(IMSG* pIMSG)
{
	if(pIMSG->fromCtrlId == MATRIX_GRID_VIEW)
	{
		if(pIMSG->actionType == IMSG::ADD_PORT || pIMSG->actionType == IMSG::ADD_PORT_LINK)
		{
				this->updateSrcPortInfo(&pIMSG->srcPortName, &pIMSG->srcPortType);
				this->updateDstPortInfo(&pIMSG->dstPortName, &pIMSG->dstPortType);
		}
	}
	
	if(pIMSG->fromCtrlId == MODEL_HANDLER)
	{
		switch(pIMSG->actionType)
		{
			case (IMSG::ADD_PORT) :
				// Recieved new port type. add it for DropDown list in the bars
				if(!pIMSG->srcPortType.IsEmpty())
				{
					this->rowPortTypeBar.addDropDownString(pIMSG->srcPortType);
					this->colPortTypeBar.addDropDownString(pIMSG->srcPortType);
				}
				break;
		}
	}
}

void UIPropEditor::updateSrcPortInfo(const CString *name, const CString *type)
{
	// Set rowNameBar
	if(!name->GetLength())
	{
		this->rowPortNameBar.showText(_T(""));
		this->rowPortNameBar.disable();
	}
	else
	{
		this->rowPortNameBar.enable();
		this->rowPortNameBar.showText(*name);
	}
	//Set rowTypeBar
	if(!type->GetLength())
	{
		this->rowPortTypeBar.showText(_T(""));
		this->rowPortTypeBar.disable();
	}
	else
	{
		this->rowPortTypeBar.enable();
		this->rowPortTypeBar.showText(*type);
	}
}

void UIPropEditor::updateDstPortInfo(const CString *name, const CString *type)
{
	// Set colNameBar
	if(!name->GetLength())
	{
		this->colPortNameBar.showText(_T(""));
		this->colPortNameBar.disable();
	}
	else
	{
		this->colPortNameBar.enable();
		this->colPortNameBar.showText(*name);
	}
	//Set colTypeBar
	if(!type->GetLength())
	{
		this->colPortTypeBar.showText(_T(""));
		this->colPortTypeBar.disable();
	}
	else
	{
		this->colPortTypeBar.enable();
		this->colPortTypeBar.showText(*type);
	}
}

