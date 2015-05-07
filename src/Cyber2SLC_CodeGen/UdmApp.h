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

#include "CyberComposition.h"

#ifdef _USE_DOM
	#include "UdmDOM.h"
#endif

#include "UdmGme.h"
#include "UdmStatic.h"

#include "CGProgressDialog.h"

class CUdmApp  
{

public:
	int Initialize()
	{
		mgaPath = "";
		currPath = "";
		logFile = "";
		outputDir = "";
		master = false;
		badNameObjs.clear();
		checkedSFs.clear();
		return 0;
	}
	void UdmMain(Udm::DataNetwork* p_backend, Udm::Object currentObject, std::set<Udm::Object> selectedObjects, long param);
	CString mgaPath;
	std::string outputDir;
	bool master;

private:
	std::string currPath;
	std::string logFile;

	list<Udm::Object> badNameObjs;
	list<Udm::Object> badPortObjs;
	set<CyberComposition::SimulinkWrapper> checkedSFs;
	set<CyberComposition::Simulink::SFState> checkedSFStates;

	//void processComponent(CyPhyML::Component &com,  bool genmo, std::string comname="");
	bool processSimulinkWrapper(CyberComposition::SimulinkWrapper &sl,  bool genmo, std::string comname="");
	std::string createWrapperFolder(const std::string &foldername);
	void removeWrapperFolder(const std::string &foldername);
//	void processComponentAssembly(const CyPhyML::ComponentAssembly &ca, bool genmo, std::string ca_prefix="");
//	void processTestBench(const CyPhyML::TestBench &tb);
	void checkBadNames(const CyberComposition::Simulink::Block &block);
	void checkBadPorts(const CyberComposition::Simulink::Block &block);
	void checkBadPorts_State_Data( const CyberComposition::Simulink::State &state);
	void checkBadPorts_SFState_Data( const CyberComposition::Simulink::SFState &sf_state);
	void checkSFState(CyberComposition::Simulink::SFState &sfstate);
	void updateInputPort(CyberComposition::Simulink::SFData &sfdata);
	void updateOutputPort(CyberComposition::Simulink::SFData &sfdata);
//	void checkComponent(CyPhyML::Component &com);
	void checkSimulinkWrapper(CyberComposition::SimulinkWrapper &sl);
	//void checkPortNumber(const CyberComposition::Simulink::Subsystem &subsys);
	void checkPortNumber(const CyberComposition::Simulink::Block &block);
};

#endif // !defined(AFX_UDMAPP_H__296A93EB_6DB6_4179_AA0B_A3D799FB3A1C__INCLUDED_)
