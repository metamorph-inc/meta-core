/*** Included Header Files ***/
#include "results_dialog.h"
#include "motivate_process.h"
#include "action.h"


/************************************************************************************/


CResultsDialog::CResultsDialog( CWnd* pParent, BON::Model &action )
	: CDialog(CResultsDialog::IDD, pParent), _action(NULL), _bonAction(action) {
	//{{AFX_DATA_INIT(CMainDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


CResultsDialog::CResultsDialog( CWnd* pParent, Action *action )
	: CDialog(CResultsDialog::IDD, pParent), _action(action), _bonAction() {
	//{{AFX_DATA_INIT(CMainDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Make sure we have a valid bonAction
	if (action) this->_bonAction = action->Model();
}


void CResultsDialog::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainDlg)
	DDX_Control(pDX, IDC_SCRIPT_EDIT, this->_scriptEdit);
	DDX_Control(pDX, IDC_RESULTS_EDIT, this->_resultsEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CResultsDialog, CDialog)
	//{{AFX_MSG_MAP(CMainDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
//	ON_BN_CLICKED(IDC_SCRIPTPATH_BUTTON, OnScriptPathButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CMainDlg message handlers

BOOL CResultsDialog::OnInitDialog() {
	CDialog::OnInitDialog( );

	// Set the name of this window
	this->SetWindowText( this->_bonAction->getName().c_str() );

	// Do we have an Action
	if (this->_action) {
		// Set the script text
		this->_scriptEdit.SetWindowText( this->_action->ExecutedString().c_str() );
		// Set the results text
		this->_resultsEdit.SetWindowText( this->_action->Results().c_str() );
	}
	// Just have a BON::Model
	else {
		// Get the template text
		std::string templateStr = this->_bonAction->getAttribute( "TemplateCode" )->getValue();
		// Set the script edit
		this->_scriptEdit.SetWindowText( templateStr.c_str() );
		// Clear the results text
		this->_resultsEdit.SetWindowText( "" );
	}
	// Return TRUE  unless you set the focus to a control
	return TRUE;
}


void CResultsDialog::OnSysCommand(UINT nID, LPARAM lParam) {
	CDialog::OnSysCommand(nID, lParam);
}


/************************************************************************************/

