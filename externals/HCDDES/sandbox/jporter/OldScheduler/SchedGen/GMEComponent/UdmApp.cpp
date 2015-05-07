// UdmApp.cpp: implementation of the CUdmApp class.
// This file was automatically generated as UdmApp.cpp
// by UDM Interpreter Wizard on Wednesday, August 13, 2008 15:55:00



#include "stdafx.h"
#include "afxdlgs.h"
#include "UdmApp.h"
#include "UdmConfig.h"
#include "Uml.h"

#include "ESMoL.h"
#include "schedResults.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <set>

using namespace ESMoL;


/*********************************************************************************/
/* Initialization function. The framework calls it before preparing the backend. */
/* Initialize here the settings in the config global object.					  */
/* Return 0 if successful.														  */
/*********************************************************************************/
int CUdmApp::Initialize()
{

	// TODO: Your initialization code comes here...
	return 0;
}



/* 
Remarks to CUdmApp::UdmMain(...):
0.	The p_backend points to an already open backend, and the framework 
	closes it automatically. DO NOT OPEN OR CLOSE IT!
	To commit changes use p_backend->CommitEditSequence().
	To abort changes use p_backend->AbortEditSequence().
	To save changes to a different file use p_backend->SaveAs() or p_backend->CloseAs().

1.	Focus is the currently open model.

2.	The possible values for param (from GME DecoratorLib.h component_startmode_enum):
	GME_MAIN_START			=   0,
	GME_BROWSER_START		=   1,
	GME_CONTEXT_START		=   2,
	GME_EMBEDDED_START		=   3,
	GME_MENU_START			=  16,
	GME_BGCONTEXT_START		=  18,
	GME_ICON_START			=  32,
	METAMODEL_CHECK_SYNTAX	= 101

 3. The framework catches all the exceptions and reports the error in a message box,
	clean up and close the transactions aborting the changes. You can override this 
	behavior by catching udm_exception. Use udm_exception::what() to form an error 
	message.
*/

/***********************************************/
/* Main entry point for Udm-based Interpreter  */
/***********************************************/

void CUdmApp::UdmMain(
					 Udm::DataNetwork* p_backend,		// Backend pointer(already open!)
					 Udm::Object focusObject,			// Focus object
					 set<Udm::Object> selectedObjects,	// Selected objects
					 long param)						// Parameters
{	
	// Select a file
	CFileDialog * dlgRsltFile = new CFileDialog(true);

	CString rsltFileName;
	dlgRsltFile->GetOFN().lpstrFile = rsltFileName.GetBuffer(8000);
	dlgRsltFile->GetOFN().nMaxFile = 8000;
	dlgRsltFile->GetOFN().lpstrTitle = "Select result file to open:";
	dlgRsltFile->GetOFN().lpstrFilter = "Result Files (*.rslt)\0*.rslt\0All files (*.*)\0*.*\0\0";

	INT_PTR nResult = dlgRsltFile->DoModal();
	rsltFileName.ReleaseBuffer();

	if ( nResult == IDCANCEL )
		return;

	string rsltPath( rsltFileName );
	string rsltFname( rsltPath.substr( rsltPath.rfind( "\\" )+1 ));
	//AfxMessageBox( rsltFname.c_str());	

	CFileDialog * dlgIDFile = new CFileDialog(true);

	CString idFileName;
	dlgIDFile->GetOFN().lpstrFile = idFileName.GetBuffer(8000);
	dlgIDFile->GetOFN().nMaxFile = 8000;
	dlgRsltFile->GetOFN().lpstrTitle = "Select matching ID file to open:";
	dlgRsltFile->GetOFN().lpstrFilter = "ID Files (*.ids)|*.ids|All files (*.*)|*.*||";

	nResult = dlgIDFile->DoModal();
	idFileName.ReleaseBuffer();

	if ( nResult == IDCANCEL )
		return;

	string idPath( idFileName );
	string idFname( idPath.substr( idPath.rfind( "\\" )+1 ));
	//AfxMessageBox(idFname.c_str());

	SchedKeeper sk( rsltPath, idPath );
	sk.Dump();

	// Select a design folder (for now, use the first one found )
	Udm::Object rootObject=p_backend->GetRootObject();
	RootFolder rf = RootFolder::Cast( rootObject );
	std::set< DesignFolder > dfs = rf.DesignFolder_kind_children();

	if ( dfs.size() < 1 )
	{
		std::string msg( "No DesignFolder found in model." );
		AfxMessageBox( msg.c_str() );
		return;
	}

	DesignFolder df( *dfs.begin() );

	// Check consistency:  Design Folder vs. Results model (show warnings)

	// Do you want to proceed?  If no, return
	// AfxMessageBox( text, MB_YESNO ) returns int (IDYES, IDNO)

	// Iterate down and clear all of the TT schedule fields in the Design Folder
	set< Deployment > dps = df.Deployment_kind_children();
	for ( set< Deployment >::iterator pDp = dps.begin(); pDp != dps.end(); pDp++ )
	{
		set< TTTask > tasks = (*pDp).TTTask_kind_children();
		for (set< TTTask >::iterator ptask = tasks.begin(); ptask != tasks.end(); ptask++ )
		{
			(*ptask).TTSchedule() = std::string("");

			set< TTMessage > msgs = (*ptask).TTMessage_kind_children();
			for ( set< TTMessage >::iterator pmsg = msgs.begin(); pmsg != msgs.end(); pmsg++ )
			{
				(*pmsg).TTSchedule() = std::string("");
			}

		}
	}

	set< HardwareUnit > pfs = df.HardwareUnit_kind_children();
	for ( set< HardwareUnit >::iterator phwu = pfs.begin(); phwu != pfs.end(); phwu++ )
	{
		ProcessHardwareUnit( *phwu );
	}

	// Write in values...
	// First, the buses... (this is not quite right, since it does _all_ of the buses)
	for ( set< HardwareUnit >::iterator phwu = pfs.begin(); phwu != pfs.end(); phwu++ )
	{
		ProcessHardwareUnit( *phwu, sk.GetHyperperiod() );
	}

	// Then tasks and messages
	// Get the resource name, and the task name
	// TODO: flip the interface around -- get the Node/Bus names from the models,
	// TODO: also get associated Tasks and Messages.  Hmmm... this could be hard
	// TODO:  without crosslinks.  Then look up task/message start lists from a map in 
	// TODO:  the keeper class.

	// We can simply look up the objects by ID
	// Then get the associated schedule and fill it in.
	// We cheated, in that we knew that uniqueID_type was a long
	// to avoid class tangling.
	std::vector< std::string > namelist;
	sk.GetExecNames( namelist );
	//AfxMessageBox( "0" );
	for( std::vector< std::string >::iterator pName = namelist.begin();
		 pName != namelist.end(); pName++ )
	{
		long id = sk.GetExecId( *pName );
		Udm::Object obj = p_backend->ObjectById( id );

		// Two possibilities
		if ( obj )
		{
			try 
			{
				ESMoL::TTTask t = TTTask::Cast( obj );
				if ( t )
				{
					t.TTSchedule() = sk.GetSchedule( *pName );
				}
			}
			catch (...) { }

			try
			{
				ESMoL::TTMessage m = TTMessage::Cast( obj );
				if ( m )
				{
					m.TTSchedule() = sk.GetSchedule( *pName );
				}
			}
			catch (...) { }
		}
	}
	//AfxMessageBox( "9" );
}

// Recursively handle hardware units
void CUdmApp::ProcessHardwareUnit( HardwareUnit & hwu, std::string val )
{
	// Find and zero TTBus hyperperiods
	set< TTBus > buses = hwu.TTBus_kind_children();
	for ( set< TTBus >::iterator pbus = buses.begin(); pbus != buses.end(); pbus++ )
	{
		(*pbus).Hyperperiod() = val;
	}

	// Then get lower-level hardware units and process them
	set< HardwareUnit > hwus = hwu.HardwareUnit_kind_children();
	for ( set< HardwareUnit >::iterator phwu = hwus.begin(); phwu != hwus.end(); phwu++ )
	{
		ProcessHardwareUnit( *phwu, val );
	}
}


#ifdef _DEBUG
/*****************************************************/
/* Debug time helper function. If the object has an  */
/* attribute called "name", this function retrieves  */
/* it to help you to find it in the model during the */
/* application development.	Usually every GME Object */
/* has a "name" attribute. If an object happens not	 */
/* to have it,function returns <no name specified>.  */
/*****************************************************/
string CUdmApp::ExtractName(Udm::Object ob)
{
	Uml::Class cls= ob.type();				
	set<Uml::Attribute> attrs=cls.attributes();		
	
	// Adding parent attributes
	set<Uml::Attribute> aattrs=Uml::AncestorAttributes(cls);
	attrs.insert(aattrs.begin(),aattrs.end());

	for(set<Uml::Attribute>::iterator ai = attrs.begin();ai != attrs.end(); ai++) 
	{
		if(string(ai->type())=="String")
		{
			string str=ai->name();
			if(str=="name")
			{
				string value=ob.getStringAttr(*ai);
				if(value.empty())value="<empty string>";
				return value;
			}			
		}				
	}	
	return string("<no name specified>");
}
#endif
