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
#include "GMECOM.h"

void refactor_in_DC(CyPhyML::RootFolder &cyphy_rf, Udm::Object &focusObject, set<Udm::Object> &selectedObjects);
void refactor_in_noneDC(CyPhyML::RootFolder &cyphy_rf,Udm::Object &focusObject, set<Udm::Object> &selectedObjects);
void refactor_single_CA(CyPhyML::RootFolder &cyphy_rf, Udm::Object &focusObject, CyPhyML::ComponentAssembly &cyphy_ca, CyPhyML::ComponentRef &comref);
void refactor_single_DC(CyPhyML::RootFolder &cyphy_rf, Udm::Object &focusObject, CyPhyML::DesignContainer &cyphy_dc);
void convert_into_newDC(CyPhyML::RootFolder &cyphy_rf, Udm::Object &focusObject, set<Udm::Object> &selectedObjts);
void convert_into_newCA(CyPhyML::RootFolder &cyphy_rf, Udm::Object &focusObject, set<Udm::Object> &selectedObjts);

class CUdmApp  
{
public:
	static int Initialize();
	static void UdmMain(Udm::DataNetwork* p_backend,Udm::Object currentObject, set<Udm::Object> selectedObjects, long param);
	static CString mgaPath;
#ifdef _DEBUG
	static string ExtractName(Udm::Object ob);
#endif
};

#endif // !defined(AFX_UDMAPP_H__296A93EB_6DB6_4179_AA0B_A3D799FB3A1C__INCLUDED_)
