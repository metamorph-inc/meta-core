#include "stdafx.h"
#include "UdmApp.h"
#include "UdmConfig.h"
#include "Uml.h"
#include "UdmUtil.h"

#include "runSF_CodeGen.h"

using namespace std;

CString CUdmApp::mgaPath = "";

void showUsage()
{
	CString usage("Cyber2SFC_CodeGen interpreter cannot be invoked. Please launch the interpreter inside a SimulinkWrapper model.\r\n");
	AfxMessageBox(usage,MB_ICONINFORMATION);				
}

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

void CUdmApp::UdmMain(
					 Udm::DataNetwork* p_backend,		// Backend pointer(already open!)
					 Udm::Object focusObject,			// Focus object
					 std::set<Udm::Object> selectedObjects,	// Selected objects
					 long param)						// Parameters
{	
	if(focusObject==Udm::null)
	{
		showUsage();	
		return;
	}

	if(!selectedObjects.empty())
	{
		for(std::set<Udm::Object>::iterator it=selectedObjects.begin();it!=selectedObjects.end();++it)
		{
			Udm::Object selectObj = *it;
			if(Uml::IsDerivedFrom(selectObj.type(), CyberComposition::Simulink::Subsystem::meta))
			{
				CyberComposition::Simulink::Subsystem sys = CyberComposition::Simulink::Subsystem::Cast(selectObj);
				runCyberSF_CodeGen(sys, (LPCTSTR)mgaPath, p_backend);
			}
			//else if(Uml::IsDerivedFrom(selectObj.type(), CyberComposition::SimulinkWrapper::meta))
			//{
			//	CyberComposition::SimulinkWrapper slmodel = CyberComposition::SimulinkWrapper::Cast(selectObj);
			//	runCyberSF_CodeGen(slmodel, (LPCTSTR)mgaPath, p_backend);
			//}
		}
	}
	else if(Uml::IsDerivedFrom(focusObject.type(), CyberComposition::SimulinkWrapper::meta))
	{
		CyberComposition::SimulinkWrapper slmodel = CyberComposition::SimulinkWrapper::Cast(focusObject);
		runCyberSF_CodeGen(slmodel, (LPCTSTR)mgaPath, p_backend);
	}
	else
	{
		showUsage();	
		return;
	}

	//AfxMessageBox("SF_Code has been generated for the SimulinkWrapper model. ",MB_ICONINFORMATION);
	//GMEConsole::Console::Info::writeLine("SF_Code has been generated for the SimulinkWrapper model.");
}

