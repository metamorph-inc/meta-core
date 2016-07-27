#include "stdafx.h"
#include "UdmConsole.h"
#include "UdmBase.h"


namespace GMEConsole
{
	CComPtr<IGMEOLEApp> Console::gmeoleapp=0;

	void Console::setupConsole(CComPtr<IMgaProject> project)
	{
		CComPtr<IMgaClient> client;	
		CComQIPtr<IDispatch> pDispatch;
		HRESULT s1 = project->GetClientByName(CComBSTR(L"GME.Application"),&client);

		if ((SUCCEEDED(s1)) && (client != 0))
		{
			HRESULT s2 = client->get_OLEServer(&pDispatch);
			if ((SUCCEEDED(s2)) && (pDispatch != 0))
			{
				// release here in case setupConsole was called twice
				gmeoleapp.Release();
				HRESULT s3 = pDispatch->QueryInterface(&gmeoleapp);
				if ((SUCCEEDED(s3)) && (gmeoleapp != 0))
				{
					// gmeoleapp->ConsoleClear();
					gmeoleapp->put_ConsoleContents(NULL);
				}
				else
				{
					throw udm_exception("GME Console could not be accessed.");
				}
			}
		}
	}

	void Console::SetupConsole(CComPtr<IMgaProject> project)
	{
		setupConsole(project);
	}
}