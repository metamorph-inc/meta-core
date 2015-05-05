#include "stdafx.h"
#include "UdmApp.h"
#include "UdmConfig.h"
#include "Uml.h"
#include "UdmUtil.h"

#include "UdmConsole.h"
#include "CyPhyToolbox.h"

extern bool automated_expand;
extern bool automated_collapse;
extern std::string msg_exception;

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
					 std::set<Udm::Object> selectedObjects,	// Selected objects
					 long param)						// Parameters
{	

	// TODO: Your Code Comes Here...

	try
	{
		if (param == 128)
		{
			automated_expand = true;
			ConsoleMessagesOn = false;
		}
		set<Udm::Object> mySet;
		if (focusObject) {
			mySet.insert(focusObject);
		} else {
			for (set<Udm::Object>::const_iterator i = selectedObjects.begin(); i != selectedObjects.end(); i++)
				mySet.insert(*i);
		}

		bool expand = automated_expand;
		bool collapse = automated_collapse;
		if (!automated_expand && !automated_collapse) {
			int result = IDYES; // collapse is broken MessageBoxA(NULL, "Yes to expand, No to collapse", "Operation", MB_YESNO);
			if (result == IDYES)
				expand = true;
			else if (result == IDNO)
				collapse = true;
		}
		
		CyPhyElaborate cpe;

		for (std::set<Udm::Object>::const_iterator i = mySet.begin(); i != mySet.end(); i++) {
			Udm::Object oi(*i);

			if (oi.type() == ComponentAssembly::meta) {
				if (expand)
				{
					cpe.elaborate( CyPhyML::ComponentAssembly::Cast(oi) );
					for (set<Udm::Object>::const_iterator j = cpe.cr_null_ref_set.begin(); j != cpe.cr_null_ref_set.end(); j++)
						GMEConsole::Console::writeLine("ComponentRef [" + (string)(CyPhyML::ComponentRef::Cast(*j).name()) + "] is null!" , MSG_WARNING);
				}
				else if (collapse)
					; // cpe.collapse( CyPhyML::ComponentAssembly::Cast(oi) );
			} else if (Udm::IsDerivedFrom(focusObject.type(), TestBenchType::meta)) {
				if (expand)
				{
					cpe.elaborate(TestBenchType::Cast(oi));
				}
				else if (collapse)
				{
					; // cpe.collapse(TestBenchType::Cast(oi));
				}
			}
		}
		traceability.Attach(new CyPhyElaborateTraceability(std::unique_ptr<std::map<Udm::Object, Udm::Object> >(std::move(cpe.copiedObjectsToOriginals))));
	}
	catch (udm_exception &exc)
	{
		GMEConsole::Console::writeLine("Udm exception occured! Exception description: " + (std::string)exc.what(), MSG_ERROR);
        msg_exception = exc.what();
        throw exc;
	}
	catch (...)
	{
		GMEConsole::Console::writeLine("Non-Udm exception occured.", MSG_ERROR);
	}
}

extern "C" __declspec(dllexport)
bool __stdcall elaborate(IUnknown* currentObj)
{
	try {
		CComPtr<IMgaFCO> fcoCurrentObj;
		currentObj->QueryInterface(&fcoCurrentObj);
		if (fcoCurrentObj)
		{
			CComPtr<IMgaProject> project;
			if (SUCCEEDED(fcoCurrentObj->get_Project(&project)))
			{
				UdmGme::GmeDataNetwork dn(CyPhyML::diagram);
				dn.OpenExisting(project, Udm::CHANGES_LOST_DEFAULT, true);
				Udm::Object current = dn.Gme2Udm(fcoCurrentObj);

				CyPhyElaborate cpe;
				if (current.type() == ComponentAssembly::meta) {
					cpe.elaborate( CyPhyML::ComponentAssembly::Cast(current) );
					return true;
				} else if (current.type() == TestBench::meta) {
					cpe.elaborate( CyPhyML::TestBench::Cast(current) );
					return true;
				}
			}
		}
	} catch (udm_exception& e) {
		return false;
	}
	return false;
}
