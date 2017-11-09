// UdmApp.h: interface for the CUdmApp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UDMAPP_H__296A93EB_6DB6_4179_AA0B_A3D799FB3A1C__INCLUDED_)
#define AFX_UDMAPP_H__296A93EB_6DB6_4179_AA0B_A3D799FB3A1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Udm includes
#include "UdmBase.h"
#include "Uml.h"
#include "UmlExt.h"

#ifdef _USE_DOM
	#include "UdmDOM.h"
#endif

#include "UdmGme.h"
#include "UdmStatic.h"


class CUdmApp  
{

public:
	string projectDir;
	string projectDir_SOT;
	bool &Automation;
	int Initialize();
	void UdmMain(Udm::DataNetwork* p_backend, Udm::Object currentObject, std::set<Udm::Object> selectedObjects, long param);
	std::vector<std::string> numericLeafNodes;

	bool &DoNotGeneratePostProcessing;
	string &OutputDir;
	CUdmApp(bool &Automation, bool &DoNotGeneratePostProcessing, string &OutputDir) : Automation(Automation), DoNotGeneratePostProcessing(DoNotGeneratePostProcessing), OutputDir(OutputDir)
	{
	}

private:
	void GeneratePostProcessingPython(const Udm::Object &, string);
	static void CopyDirectory(string src, string dest);
};

#endif // !defined(AFX_UDMAPP_H__296A93EB_6DB6_4179_AA0B_A3D799FB3A1C__INCLUDED_)
