#include "stdafx.h"
#include "UdmApp.h"
#include "UdmConfig.h"
#include "Uml.h"
#include "UdmUtil.h"
#include "UI.h"

using namespace std;

/*********************************************************************************/
/* Initialization function. The framework calls it before preparing the backend. */
/* Initialize here the settings in the config global object.					 */
/* Return 0 if successful.														 */
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

2.	The possible values for param (from GME Mga.idl component_startmode_enum):
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
					 std::set<Udm::Object> selectedObjects,	// Selected objects
					 long param)						// Parameters
{	

	// TODO: Your Code Comes Here...
	CString toolPrefix;
	toolPrefix.LoadString(LOG_TOOL_PREF);
	
	Console::Info::writeLine(string(toolPrefix) + 
							 _T("Invoked on [Object:] ") + 
							 MyUdmUtil::getHyperLinkPath_StdString(focusObject) + 
							 _T(", [TYPE]:") + (string) focusObject.type().name());

	if(!(Udm::IsDerivedFrom(focusObject.type(), ModelicaComponent::meta)))
	{
		Console::Error::writeLine(string(toolPrefix) + 
								  _T("must be invoked from within a 'SignalFlowWrapper' \
									 OR 'SimulinkWrapper' type object."));
		return;
	}

	UI tool;
	// Attach the ModelicaComponent Object to tool
	tool.attachComponent(ModelicaComponent::Cast(focusObject));
	// inititate the UI for the tool
	tool.DoModal();
}

