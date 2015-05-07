/*** Included Header Files ***/
#include "InterpreterWin.h"
#include "motivate_process.h"
#include "action.h"
#include "results_dialog.h"


/************************************************************************************/


CInterpreterDialog::CInterpreterDialog( CWnd* pParent, BON::Project &project )
	: CDialog(CInterpreterDialog::IDD, pParent), _hIcon(), _project(project),
	_processes(), _currentProcess(), _actions() {
	//{{AFX_DATA_INIT(CMainDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CInterpreterDialog::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainDlg)
	DDX_Control(pDX, IDC_PROCESS_COMBO, this->_processCombo);
	DDX_Control(pDX, IDC_INITACTION_COMBO, this->_initActionCombo);
	DDX_Control(pDX, IDC_ACTION_LIST, this->_actionList);
	DDX_Control(pDX, IDC_ERRORHALT_CHECK, this->_errorHaltCheck);
	DDX_Control(pDX, IDC_SAVESCRIPT_CHECK, this->_saveScriptCheck);
	DDX_Control(pDX, IDC_SAVEONLY_CHECK, this->_saveOnlyCheck);
	DDX_Control(pDX, IDC_SCRIPTPATH_EDIT, this->_scriptPathEdit);
	DDX_Control(pDX, IDC_SCRIPTPATH_BUTTON, this->_scriptPathButton);
	DDX_Control(pDX, IDC_SCRIPTPATH_LABEL, this->_scriptPathLabel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInterpreterDialog, CDialog)
	//{{AFX_MSG_MAP(CMainDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_DBLCLK, IDC_ACTION_LIST, OnActionItemSelection)
	ON_CBN_SELCHANGE(IDC_PROCESS_COMBO, OnProcessComboChange)
	ON_CBN_SELCHANGE(IDC_INITACTION_COMBO, OnInitActionComboChange)
	ON_BN_CLICKED(IDC_SAVESCRIPT_CHECK, OnSaveScriptCheck)
	ON_BN_CLICKED(IDC_SCRIPTPATH_BUTTON, OnScriptPathButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CMainDlg message handlers

BOOL CInterpreterDialog::OnInitDialog() {
	CDialog::OnInitDialog( );
	// Make sure Halt on Error is checked
	this->_errorHaltCheck.SetCheck(BST_CHECKED);
	// Get the list of processes
	this->_processes = Motivate::MapProcesses( this->_project );
	std::vector< std::string > processVector = Motivate::OrderProcesses( this->_processes );

	// Loop through all items in process list and add to process combo
	std::vector< std::string >::iterator procIter = processVector.begin();
	for (; procIter != processVector.end(); procIter++) {
		this->_processCombo.AddString( (*procIter).c_str() );
	}
	// Set the selected process in the combo box
	this->_processCombo.SetCurSel(0);

	// Format the ListControl
	LVCOLUMN lvColumn;
	this->_actionList.SetExtendedStyle( this->_actionList.GetStyle() | LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT );
	this->_actionList.InsertColumn( 0, "Exec", LVCFMT_CENTER, 40 );
	// Add the Action Name column
	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 315;
	lvColumn.pszText = "Action Name";
	this->_actionList.InsertColumn(1, &lvColumn);
	// Add the Status column
	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_CENTER;
	lvColumn.cx = 50;
	lvColumn.pszText = "Status";
	this->_actionList.InsertColumn( 2, &lvColumn );
	// Create the image list icons
	this->_actionListImages.Create( IDB_ACTIONLISTIMGS, 16, 1, RGB( 255, 255, 245 ) );
	this->_actionList.SetImageList( &(this->_actionListImages), LVSIL_SMALL);

	// Notify the initial action combo to build itself
	this->OnProcessComboChange();
	// Set the focus to the process combo box
	this->_processCombo.SetFocus();

	// Return TRUE  unless you set the focus to a control
	return FALSE;
}


void CInterpreterDialog::OnSysCommand(UINT nID, LPARAM lParam) {
	CDialog::OnSysCommand(nID, lParam);
}


// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
void CInterpreterDialog::OnPaint() {
	if (IsIconic()) {
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, this->_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}


// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CInterpreterDialog::OnQueryDragIcon() {
	return (HCURSOR) this->_hIcon;
}


void CInterpreterDialog::ResetState(void) {
	// First, try to reset the Conductor
	Conductor::Reset();
	// Clear all of the CListCtrl items
	for (int i=0; i<this->_actionList.GetItemCount(); i++) {
		// Set the item status to blank
		this->_actionList.SetItemText( i, 2, "" );
		// Set the icon to blank
		LVITEM lvItem;
		lvItem.mask = LVIF_IMAGE | LVIF_TEXT;
		lvItem.iItem = i;
		lvItem.iSubItem = 0;
		lvItem.pszText = "";
		lvItem.iImage = 3;
		// Update the visuals with the action status
		this->_actionList.SetItem( &lvItem );
	}
}


void CInterpreterDialog::OnSaveScriptCheck(void) {
	// If box is checked
	if ( this->_saveScriptCheck.GetCheck() == 1 ) {
		// Enable items
		this->_saveOnlyCheck.EnableWindow(TRUE);
		this->_scriptPathEdit.EnableWindow(TRUE);
		this->_scriptPathLabel.EnableWindow(TRUE);
		this->_scriptPathButton.EnableWindow(TRUE);
	}
	// If box is unchecked
	else {
		// Disable items
		this->_saveOnlyCheck.EnableWindow(FALSE);
		this->_scriptPathEdit.EnableWindow(FALSE);
		this->_scriptPathLabel.EnableWindow(FALSE);
		this->_scriptPathButton.EnableWindow(FALSE);
	}
}


void CInterpreterDialog::OnScriptPathButton(void) {
}


void CInterpreterDialog::OnProcessComboChange() {
	// Clear the initActionComboBox and the action map
	this->_initActionCombo.ResetContent();
	this->_actions.clear();

	// Get the currently selected process
	int nIndex = this->_processCombo.GetCurSel();
    CString strCBText;
    this->_processCombo.GetLBText( nIndex, strCBText );
	std::string procName = strCBText;

	// Build list of all actions in the process
	std::map< std::string,BON::Model >::iterator procMapIter = this->_processes.find( procName );
	// Quick error check
	if (procMapIter == this->_processes.end()) {
		Console::Out::WriteLine( "Error finding selected process..." );
	}
	// All is good, so get the list of actions for this process
	else {
		this->_currentProcess = (*procMapIter).second;
		// Get the list of children actions
		this->_actions = Motivate::MapActions( this->_currentProcess );
		// Sort the list of children actions
		std::vector< std::string > actionVector = Motivate::OrderActions( this->_actions );
		// Loop through all actions
		std::vector< std::string >::iterator actionIter = actionVector.begin();
		for (; actionIter != actionVector.end(); actionIter++) {
			// Add actions to combo box
			this->_initActionCombo.AddString( (*actionIter).c_str() );
		}
	}

	// Set the initial action in the combo box
	this->_initActionCombo.SetCurSel(0);
	// Notify the initial action combo to build itself
	this->OnInitActionComboChange();
}


void CInterpreterDialog::OnInitActionComboChange(void) {
	// Clear the list control
	this->_actionList.DeleteAllItems();

	// Get the currently selected initial action
	int nIndex = this->_initActionCombo.GetCurSel();
    CString strCBText;
    this->_initActionCombo.GetLBText( nIndex, strCBText );
	std::string actionName = strCBText;
	std::map< std::string,BON::Model >::iterator actionMapIter = this->_actions.find( actionName );
	// Quick error check
	if (actionMapIter == this->_actions.end()) {
		Console::Out::WriteLine( "Error finding selected initial action..." );
	}

	// Build the ordered list of actions
	std::vector< BON::Model > actionVec;
	// Recursively find all children actions
	Motivate::RecurseActions( (*actionMapIter).second, actionVec );

	// Load the proper list of actions
	std::vector< BON::Model >::reverse_iterator actionVecIter = actionVec.rbegin();
	for (; actionVecIter < actionVec.rend(); ++actionVecIter) {
		// Set up the item to be inserted
		LVITEM lvItem;
		lvItem.mask = LVIF_IMAGE | LVIF_TEXT;
		lvItem.iItem = 0;
		lvItem.iSubItem = 0;
		lvItem.pszText = "";
		lvItem.iImage = 3;
		// Insert the item
		nIndex = this->_actionList.InsertItem( &lvItem );
		// Set all of the column values
		this->_actionList.SetItemText( nIndex, 1, (*actionVecIter)->getName().c_str() );
		this->_actionList.SetItemText( nIndex, 2, "" );
		this->_actionList.SetCheck( nIndex, true );
	}
}


void CInterpreterDialog::OnActionItemSelection(NMHDR *pNMHDR, LRESULT *pResult) {
	// Figure out which item was double clicked
	int nItem = -1;
	for (int i=0; i<this->_actionList.GetSelectedCount(); i++) {
		// Get the next selected item
		nItem = this->_actionList.GetNextItem( nItem, LVNI_SELECTED );
	}
	// Make sure something has been selected
	if (nItem == -1) return;
	// Get name of the action
	std::string actionName = this->_actionList.GetItemText( nItem, 1);
	// Is there a current process (i.e. has the process been run?)
	Process* process = Conductor::CurrentProcess();
	if ( process ) {
		// Find action in current process
		Action* action = process->FindAction( actionName );
		// See if the action has been run
		if ( action ) {
			// Open the results dialog
			CResultsDialog dialog( this, action );
			int nResponse = (int)dialog.DoModal();
			// Exit this method
			return;
		}
	}
	// Find BON::Action - it must not have been run yet
	std::map< std::string,BON::Model >::iterator actionIter = this->_actions.find( actionName );
	// Make sure you have found it
	if ( actionIter == this->_actions.end() ) return;
	BON::Model action = (*actionIter).second;
	// Open the results dialog
	CResultsDialog dialog( this, action );
	int nResponse = (int)dialog.DoModal();
}


void CInterpreterDialog::OnOK() {
	// Reset everything before we start
	this->ResetState();

	// Get brake on failure value
	bool stopOnError = this->_errorHaltCheck.GetCheck() == 1;
	// Build list of checked actions
	int numActions = this->_actionList.GetItemCount();
	std::vector< BON::Model > actions;
	for (int i=0; i < numActions; i++) {
		// If the item is checked ...
		if ( this->_actionList.GetCheck(i) == TRUE ) {
			// Get the name of the action
			std::string actionName = this->_actionList.GetItemText( i, 1 );
			// Look it up in the map
			std::map< std::string,BON::Model >::iterator actionIter = this->_actions.find( actionName );
			// Insert the model into the execution list
			actions.push_back( (*actionIter).second );
		}
	}
	// Clear the console
	Console::Clear();
	Action *currentAction;
	bool continueFlag = true;
	// Prepare and execute the actions
	Conductor::Prepare( this->_currentProcess, actions );
	do {
		// Execute an action
		currentAction = Conductor::ExecuteStep();
		// See if action was successful
		if ( currentAction ) {
			// Output the results to the console
			if (true) Console::Out::WriteLine( currentAction->Results().c_str() );
			// Find the list control item number
			int itemCount = 0;
			for( int i=0; i<this->_actionList.GetItemCount(); i++) {
				std::string itemText = this->_actionList.GetItemText( i, 1);
				// See if the strings match
				if ( itemText.compare( currentAction->Name() ) == 0 ) itemCount = i;
			}
			// Get the action status
			LVITEM lvItem;
			lvItem.mask = LVIF_IMAGE | LVIF_TEXT;
			lvItem.iItem = itemCount;
			lvItem.iSubItem = 0;
			lvItem.pszText = "";
			if ( currentAction->Status() == Success ) {
				lvItem.iImage = 1;
				this->_actionList.SetItemText( itemCount, 2, "Success" );
			}
			else if ( currentAction->Status() == Failure ){
				lvItem.iImage = 0;
				this->_actionList.SetItemText( itemCount, 2, "Failure" );
			}
			else lvItem.iImage = 3;
			// Update the visuals with the action status
			this->_actionList.SetItem( &lvItem );
			// Advance the item count
			itemCount++;
		}
		// Determine the continue flag
		if ( !currentAction ) continueFlag = false;
		else if (( stopOnError ) && ( currentAction->Status() == Failure )) continueFlag = false;
	} while ( continueFlag );
	// Nothing else to do for now
}


/************************************************************************************/

