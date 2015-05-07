// UI.cpp : implementation file
//

#include "stdafx.h"
#include "afxdialogex.h"
#include "UI.h"
#include "MyColors.h"
#include <safeint.h>

// UI dialog

IMPLEMENT_DYNAMIC(UI, CDialog)

UI::UI(CWnd* pParent /*=NULL*/)
	: CDialog(UI::IDD, pParent)
{

}

UI::~UI()
{
}

void UI::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btnOK);
}


BEGIN_MESSAGE_MAP(UI, CDialog)
	ON_WM_CTLCOLOR()
	ON_MESSAGE(DM_GETDEFID, OnGetDefID)
END_MESSAGE_MAP()


// UI message handlers


BOOL UI::OnInitDialog()
{
	CDialog::OnInitDialog();

	this->ShowWindow(SW_MAXIMIZE);

	// TODO:  Add extra initialization here
	CRect	dlgRect;
	CString capText;
	// Get the maximized dlg windows area
	GetClientRect(&dlgRect);

	// Set footer, header
	this->HEADER_END = dlgRect.top + 0.05 * dlgRect.Height();
	this->FOOTER_START = dlgRect.bottom - 0.2 * dlgRect.Height();

	//Move the OK button in the maximized window
	CRect btnRect(this->MARGIN_SIZE + 0.95 * dlgRect.Width(), this->FOOTER_START + this->MARGIN_SIZE, dlgRect.right - this->MARGIN_SIZE, dlgRect.bottom - this->MARGIN_SIZE);
	m_btnOK.MoveWindow(&btnRect);
	
	// Create output Console Caption
	capText.LoadString(CONSOLE_CAP_TEXT);
	// Create output console 
	CRect outConsoleRect(dlgRect.left + MARGIN_SIZE, FOOTER_START + MARGIN_SIZE, btnRect.left - MARGIN_SIZE, dlgRect.bottom - MARGIN_SIZE);
	if(!outputConsole.initialize(capText, OUTPUT_CONSOLE_CAPTION, outConsoleRect, this, LB_CONSOLE)) 
	{	
		this->EndDialog(IDOK);
		return TRUE;
	}

	// Create the Matrix Grid view
	CRect gridRect(dlgRect.left + MARGIN_SIZE, HEADER_END + MARGIN_SIZE, dlgRect.right - MARGIN_SIZE, HEADER_END + (FOOTER_START - HEADER_END) * 0.7);
	if(!matrixView.Create(gridRect, this, MATRIX_GRID_VIEW))
	{
		AfxMessageBox(_T("Failed to Create Grid Matrix View! Closing."));
		this->EndDialog(IDOK);
		return TRUE;
	}

	// Create the property viewer
	CRect propViewRect(dlgRect.left + MARGIN_SIZE, gridRect.bottom + MARGIN_SIZE , dlgRect.right - MARGIN_SIZE, FOOTER_START - MARGIN_SIZE);
	if(!propViewer.initialize(this, propViewRect))
	{	
		this->EndDialog(IDOK);
		return TRUE;
	}

	// Attach relevant observers to relevant objects
	matrixView.attachObserver(&propViewer);		// to show the clicked/selected matrix cell's info in the prop editor
	modelHandler.attachObserver(&matrixView);	// to fill in the port info in the grid
	modelHandler.attachObserver(&propViewer);	// to insert new port data type for the drop down lists in editor bars

	// if the modelHandler is able to traverse the info about the comp successfully then display info in the grid
	if(modelHandler.generateCompReport())
		matrixView.showMatrix();
	else
	{
		AfxMessageBox(_T("Error occurred while processing the selected GME Component!"));
		this->EndDialog(IDOK);
		return TRUE;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

// for CListBox
void UI::writeConsole(CString str, UINT itemType)
{
	INT				errCode;						// err code for CListBox::AddString
	CString			outStr = _T("");				// final concatenated string
	CString			errPrefix;						// Error Statement Prefix
	CString			norPrefix;
	CString			warPrefix;
	CString			newDate = CTime::GetCurrentTime().Format("%#x");
	CString			curTime = CTime::GetCurrentTime().Format("%I:%M:%S  ");

	// if date has changed then print it
	if(curDate.Compare(newDate))
	{
		outputConsole.listBox.AddString(curTime + "------------- " + newDate + " -------------");
		curDate = newDate;
	}

	// Load prefixes from the String table
	errPrefix.LoadString(LOG_ERR_PREF);
	norPrefix.LoadString(LOG_NOR_PREF);
	warPrefix.LoadString(LOG_WAR_PREF);

	// Attach error code to the log statement
	if(itemType == UIConsole::ERROR_ITEM_CONSOLE) outStr += curTime + errPrefix + str;
	else if(itemType == UIConsole::NORMAL_ITEM_CONSOLE) outStr += curTime + norPrefix + str;
	else if(itemType == UIConsole::WARNING_ITEM_CONSOLE) outStr += curTime + warPrefix + str;
	
	if(this->outputConsole.listBox.GetSafeHwnd() == NULL)
	{
		AfxMessageBox(_T("Could not print the message: \"") + outStr + _T("\" on the Output Console!"));
		return;
	}

	// Add string to the console
	errCode = outputConsole.listBox.AddString(outStr);

	// Handle error from above call
	if(errCode == LB_ERR || errCode == LB_ERRSPACE)
		AfxMessageBox(_T("Could not print the message: \"") + outStr + _T("\" on the Output Console!"));
}

void UI::printErrorLog(CString str)
{
	this->writeConsole(str, UIConsole::ERROR_ITEM_CONSOLE);
}

void UI::printNormalLog(CString str)
{
	this->writeConsole(str, UIConsole::NORMAL_ITEM_CONSOLE);
}

void UI::printWarningLog(CString str)
{
	this->writeConsole(str, UIConsole::WARNING_ITEM_CONSOLE);
}

vector<CString> UI::getCompleteLog()
{
	vector<CString> strArray;
	for(INT i=0; i < this->outputConsole.listBox.GetCount(); ++i)
	{
		CString itemText = _T("");
		this->outputConsole.listBox.GetText(i, itemText);
		strArray.push_back(itemText);
	}

	return strArray;
}


HBRUSH UI::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	if(nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetTextColor(MY_BLACK);
		pDC->SetBkColor(MY_GRAY);

		return CreateSolidBrush(MY_GRAY);
	}
	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

// Refer to http://msdn.microsoft.com/en-us/magazine/cc301409.aspx
LRESULT UI::OnGetDefID(WPARAM wp, LPARAM lp) 
{
    return MAKELONG(0,DC_HASDEFID); 
} 
 
void UI::attachComponent(ModelicaComponent component)
{
	// Attch UI to modelhandler for it to be able to print on the UIConsole
	this->modelHandler.attachUI(this);

	this->modelHandler.attachComponent(component);
}


BOOL UI::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	CString toolPrefix;
	toolPrefix.LoadString(LOG_TOOL_PREF);

	Console::Info::writeLine(string(toolPrefix + _T("Last run's console log:")));
	vector<CString> msgArray = this->getCompleteLog();
	for each(CString msg in msgArray)
		Console::Out::writeLine(string(msg));
	Console::Info::writeLine(string(toolPrefix + _T("Completed Execution.")));

	return CDialog::DestroyWindow();
}
