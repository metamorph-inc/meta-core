// UdmApp.cpp: implementation of the CUdmApp class.
// This file was automatically generated as UdmApp.cpp
// by UDM Interpreter Wizard on Thursday, March 27, 2008 10:37:31
#include "stdafx.h"
#include "UdmApp.h"
#include "UdmConfig.h"
#include "Uml.h"
#include "CyPhyML.h"
#include "UdmUtil.h"
#include "CyphyDSEConverter.h"
#include "UdmConsole.h"

#include "CyPhyElaborate.h"
#include "DSRefactorDialog.h"
#include "CARefactorDialog.h"
#include "CompoundDCRefactorDialog.h"

using namespace CyPhyML;

CString CUdmApp::mgaPath = "";

extern bool Automation;
extern string RefactoredTypeOrAction;
extern wstring ExceptionMessage;


// converts jdlk_jasf__afsdf__Fsda___ => jdlk_jasf
std::string StripComponentNameSuffix(std::string inputName)
{
	auto result = inputName;
	size_t pos = inputName.find("__");
	if (pos != string::npos)
	{
		result = inputName.substr(0, int(pos));
	}
	return result;
}

std::string getFileNameNoExt(const std::string &fullpath)
{
	std::string ret = fullpath;
	size_t pos = ret.rfind("\\");
	if(pos!=string::npos)
		ret = ret.substr(pos+1);
	pos = ret.find(".");
	if(pos!=string::npos)
		ret = ret.substr(0, pos);
	return ret;
}

std::string appendObjLink(const Udm::Object &obj)
{
	if (!obj)
		return "null";
	return ("<A HREF=\"mga:"+ UdmGme::UdmId2GmeId(obj.uniqueId()) + "\">" + UdmUtil::ExtractName(obj) + "</A>");
}

void showUsage()
{
	CString usage(L"CyPhyDSEConverter interpreter cannot be invoked. Please see its usage as below: \r\n");
	usage.Append(L"1. Invoke interpreter inside a ComponentAssembly/TestBench model and convert this model into DesignContainer of compound type.\r\n");
	usage.Append(L"2. Invoke interpreter inside a DesignContainer model, select an object or a group of objects and ");
	usage.Append(L"convert them into a DesignContainer or ComponentAssembly and still keep the port connection.\r\n");
	GMEConsole::Console::Out::writeLine(usage.GetString());
	if(Automation) {
		ExceptionMessage = usage;
		throw;
	}
}

void generateDesignContainerFromCA(CyPhyML::RootFolder &cyphy_rf, CyPhyML::ComponentAssembly &cyphy_ca)
{
		CyPhyDSEConverter converter(cyphy_rf);
		if(!Automation)
			converter.startProgressBar();
		Udm::Object ca_parent = cyphy_ca.parent();
		CyPhyML::DesignContainer newdc;
		if(ca_parent.type()==CyPhyML::DesignContainer::meta)
		{
			converter.initCopyMap();
			CyPhyML::DesignContainer currdc = CyPhyML::DesignContainer::Cast(ca_parent);
			// Try to generate a safe name for downstream tools
			std::string newdcname = "NewDC__"+(std::string)cyphy_ca.name();
			newdc = converter.createNewDesignContainer(currdc, newdcname, false);
			// set the position of the generated container to the position of CA
			newdc.position() = cyphy_ca.position();
			converter.create_CA_DC_copy(cyphy_ca, newdc);
			converter.reconstructAssociations(currdc);
			cyphy_ca.DeleteObject();
		}
		else
		{
			converter.initCopyMap();
			CyPhyML::DesignSpace dsFolder = CyPhyML::DesignSpace::Create(cyphy_rf);
			dsFolder.name() = "NewDS__" + (std::string)cyphy_ca.name();
			newdc = converter.createNewDesignContainer(dsFolder, "NewDC__"+(std::string)cyphy_ca.name());
			converter.createDSFromCA(cyphy_ca);
		}
		if(!Automation)
			converter.endProgressBar();

		std::string addlog("New DesignContainer: "+appendObjLink(newdc)+" is generated for ComponentAssembly: "+appendObjLink(cyphy_ca));
		GMEConsole::Console::Out::writeLine(addlog);
}

void generateDesignContainerFromCARef(CyPhyML::RootFolder &cyphy_rf, CyPhyML::ComponentAssembly &cyphy_ca, CyPhyML::ComponentRef &cyphy_comref)
{
		CyPhyDSEConverter converter(cyphy_rf);
		if(!Automation)
			converter.startProgressBar();
		Udm::Object ca_parent = cyphy_ca.parent();
		if(cyphy_comref!=Udm::null)
			ca_parent = cyphy_comref.parent();
		CyPhyML::DesignContainer newdc;
		if(ca_parent.type()==CyPhyML::DesignContainer::meta)
		{
			CyPhyML::DesignContainer currdc = CyPhyML::DesignContainer::Cast(ca_parent);
			// Try to generate a safe name for downstream tools
			std::string newdcname = "NewDC__"+(std::string)cyphy_ca.name();
			newdc = converter.createNewDesignContainer(currdc, newdcname, false);
			// set the position of the generated container to the position of CA
			newdc.position() = cyphy_comref.position();
			converter.create_CA_DC_copy(cyphy_ca, newdc);
			converter.addCopyMapInput(cyphy_comref, newdc);
			converter.reconstructAssociations(currdc);
			cyphy_comref.DeleteObject();
		}
		else
		{
			CyPhyML::DesignSpace dsFolder = CyPhyML::DesignSpace::Create(cyphy_rf);
			dsFolder.name() = "NewDS__" + (std::string)cyphy_ca.name();
			newdc = converter.createNewDesignContainer(dsFolder, "NewDC__"+(std::string)cyphy_ca.name());
			converter.createDSFromCA(cyphy_ca);
		}
		if(!Automation)
			converter.endProgressBar();

		std::string addlog("New DesignContainer: "+appendObjLink(newdc)+" is generated for ComponentAssembly: "+appendObjLink(cyphy_ca));
		GMEConsole::Console::Out::writeLine(addlog);
}

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
	if(focusObject==Udm::null)
	{
		showUsage();	
		return;
	}
    try{
		CyPhyML::RootFolder cyphy_rf = CyPhyML::RootFolder::Cast(p_backend->GetRootObject());
		if(focusObject.type()==CyPhyML::DesignContainer::meta)
			refactor_in_DC(cyphy_rf,focusObject, selectedObjects);  
		else
			refactor_in_noneDC(cyphy_rf,focusObject, selectedObjects);
	}
	catch(udm_exception &exc)
	{
		throw exc;
	}
	catch(...)
	{
		throw;
	}
}

void refactor_in_DC(CyPhyML::RootFolder &cyphy_rf, Udm::Object &focusObject, set<Udm::Object> &selectedObjects)
{
	//interpreter invoked inside DesignContainer
	if(focusObject.isInstance())
	{
		CString msg("The current DesignContainer is instance itself and it cannot be modified.");

		GMEConsole::Console::Out::writeLine(msg.GetString());

		if(Automation) {
			ExceptionMessage = msg;
			throw;
		}

		return;
	}

	if(selectedObjects.empty())  
	{
		showUsage();	
		return;
	}
	else if(selectedObjects.size()==1)
	{
		Udm::Object selectedObj = *(selectedObjects.begin());
		
		if(selectedObj.type()==CyPhyML::DesignContainer::meta)
		{
			CyPhyML::DesignContainer dc = CyPhyML::DesignContainer::Cast(selectedObj);
			return refactor_single_DC(cyphy_rf, focusObject, dc);
		}

		CyPhyML::ComponentAssembly ca_obj;
		CyPhyML::Component com_obj;
		CyPhyML::ComponentRef comref_obj;
		if(selectedObj.type()==CyPhyML::ComponentAssembly::meta)
			ca_obj = CyPhyML::ComponentAssembly::Cast(selectedObj);
		else if(selectedObj.type()==CyPhyML::Component::meta)
			com_obj = CyPhyML::Component::Cast(selectedObj);
		else if(selectedObj.type()==CyPhyML::ComponentRef::meta)
		{
			comref_obj = CyPhyML::ComponentRef::Cast(selectedObj);
			CyPhyML::DesignElement referred = comref_obj.ref();
			if(referred.type()==CyPhyML::ComponentAssembly::meta)
				ca_obj = CyPhyML::ComponentAssembly::Cast(referred);
			else if(referred.type()==CyPhyML::Component::meta)
				com_obj = CyPhyML::Component::Cast(referred);
			else 
				return;
		}
		else 
		{
			showUsage();	
			return;
		}
		if(ca_obj!=Udm::null)
		{
			 return refactor_single_CA(cyphy_rf, focusObject, ca_obj, comref_obj);
		}
		else if(com_obj!=Udm::null)
		{
			if(Automation) {
				if(RefactoredTypeOrAction == "ComponentAssembly")
					convert_into_newCA(cyphy_rf, focusObject, selectedObjects);
				else if(RefactoredTypeOrAction == "DesignContainer")
					convert_into_newDC(cyphy_rf, focusObject, selectedObjects);
				else {
					udm_exception exc("Invalid refactor action selected");
					throw exc;
				}
				return;

			} else {
				CDSRefactorDialog dlg;
				if (dlg.DoModal() == IDOK)
				{
					if(dlg.m_genCA)
						convert_into_newCA(cyphy_rf, focusObject, selectedObjects);
					else
						convert_into_newDC(cyphy_rf, focusObject, selectedObjects);
				}
				else 
					return;
			}
		}
		else 
			return;
	}
	else //more than 1
	{
		bool genCA = false;
		for(auto it=selectedObjects.begin();it!=selectedObjects.end();++it)
		{
			Udm::Object currObj = *it;
			if(Uml::IsDerivedFrom(currObj.type(), CyPhyML::Component::meta) 
				|| Uml::IsDerivedFrom(currObj.type(), CyPhyML::ComponentAssembly::meta)
				|| Uml::IsDerivedFrom(currObj.type(), CyPhyML::ComponentRef::meta))
			{
				genCA = true;
				break;
			}
		}
		if(!genCA)
			return convert_into_newDC(cyphy_rf, focusObject, selectedObjects);

		if(Automation) {
				if(RefactoredTypeOrAction == "ComponentAssembly")
					convert_into_newCA(cyphy_rf, focusObject, selectedObjects);
				else if(RefactoredTypeOrAction == "DesignContainer")
					convert_into_newDC(cyphy_rf, focusObject, selectedObjects);
				else {
					udm_exception exc("Invalid refactor action selected");
					throw exc;
				}
				return;

		} else {
			CDSRefactorDialog dlg;
			if (dlg.DoModal() == IDOK)
			{
				if(dlg.m_genCA)
					return convert_into_newCA(cyphy_rf, focusObject, selectedObjects);
				else
					return convert_into_newDC(cyphy_rf, focusObject, selectedObjects);
			}
			else 
				return;
			}
	}
}

void refactor_in_noneDC(CyPhyML::RootFolder &cyphy_rf, Udm::Object &focusObject, set<Udm::Object> &selectedObjects)
{
	if(selectedObjects.empty())
	{
		if(focusObject.type()==CyPhyML::ComponentAssembly::meta)
		{
			//CA0
			CyPhyML::ComponentAssembly cyphy_ca = CyPhyML::ComponentAssembly::Cast(focusObject);
			return generateDesignContainerFromCA(cyphy_rf, cyphy_ca);
		}
		else
		{
			showUsage();	
			return;
		}
	}
	else if(selectedObjects.size()==1)
	{
		Udm::Object selectedObj = *(selectedObjects.begin());
		CyPhyML::ComponentAssembly ca_obj;
		CyPhyML::Component com_obj;
		CyPhyML::ComponentRef comref_obj;
		if(selectedObj.type()==CyPhyML::ComponentAssembly::meta)
			ca_obj = CyPhyML::ComponentAssembly::Cast(selectedObj);
		else if(selectedObj.type()==CyPhyML::Component::meta)
			com_obj = CyPhyML::Component::Cast(selectedObj);
		else if(selectedObj.type()==CyPhyML::ComponentRef::meta)
		{
			comref_obj = CyPhyML::ComponentRef::Cast(selectedObj);
			CyPhyML::DesignElement referred = comref_obj.ref();
			if(referred.type()==CyPhyML::ComponentAssembly::meta)
				ca_obj = CyPhyML::ComponentAssembly::Cast(referred);
			else if(referred.type()==CyPhyML::Component::meta)
				com_obj = CyPhyML::Component::Cast(referred);
			else 
				return;
		}
		else 
		{
			showUsage();	
			return;
		}
		if(ca_obj!=Udm::null)
		{
			 return refactor_single_CA(cyphy_rf, focusObject, ca_obj, comref_obj);
		}
		else if(com_obj!=Udm::null)
		{
			return convert_into_newCA(cyphy_rf, focusObject, selectedObjects);
		}
		else 
			return;
	}
	else //more than 1
	{
		return convert_into_newCA(cyphy_rf, focusObject, selectedObjects);
	}
}

void refactor_single_DC(CyPhyML::RootFolder &cyphy_rf, Udm::Object &focusObject, CyPhyML::DesignContainer &cyphy_dc)
{
	set<Udm::Object> selectedObjects;
	selectedObjects.insert(cyphy_dc);
	set<Udm::Object> dc_objs = cyphy_dc.GetChildObjects();
	if(dc_objs.empty())
	{
		//if the to-be-refactored DC is empty, then put it into a new Alternative Container
		return convert_into_newDC(cyphy_rf, focusObject, selectedObjects);
	}

	//for the to-be-refactored DC is not empty:
	std::string dc_type = cyphy_dc.ContainerType();
	if(dc_type!="Compound")
	{
		//if it is not compound type, then put it into a new Alternative Container
		convert_into_newDC(cyphy_rf, focusObject, selectedObjects);
	}
	else
	{
		//if it is compound type, check whether it contains DesignContainer inside
		//if there is no designContainer, user could choose to convert it into a new CA
		//otherwise, user could choose to extract the elements of designcontainer or put
		//it into a new Alternative DesignContainer
		set<CyPhyML::DesignContainer> dcs = cyphy_dc.DesignContainer_kind_children();

		RefactorType refactorType;

		if(Automation) {
			if(RefactoredTypeOrAction == "ComponentAssembly")
				refactorType = genCA;
			else if(RefactoredTypeOrAction == "DesignContainer")
				refactorType = genDC;
			else {
				refactorType = extract;
			}
		} else {
			CCompoundDCRefactorDialog dlg(!dcs.empty());//if there is designcontainer inside, then disable the generate CA radio button
			if(dlg.DoModal()==IDCANCEL)
				return;

			refactorType = dlg.rt;
		}

		switch(refactorType)
		{
		case genDC:
				return convert_into_newDC(cyphy_rf, focusObject, selectedObjects);
		case genCA:
			{
				CyPhyML::ComponentAssembly new_ca = CyPhyML::ComponentAssembly::Create(CyPhyML::DesignContainer::Cast(focusObject));
				new_ca.name() = "NewCA__"+(std::string)cyphy_dc.name();
				new_ca.position() = cyphy_dc.position();
				CyPhyDSEConverter converter(cyphy_rf);
				converter.startProgressBar();
				converter.gatherConnections_tobe_reconstruce(cyphy_dc, focusObject);
				converter.create_CA_DC_copy(cyphy_dc, new_ca);
				converter.reconstructExtractAssociations(focusObject);
				cyphy_dc.DeleteObject();					
				converter.endProgressBar();
			}
			return;
		case extract:
			{
				CyPhyDSEConverter converter(cyphy_rf);
				converter.startProgressBar();
				converter.gatherConnections_tobe_reconstruce(cyphy_dc, focusObject);
				converter.create_CA_DC_copy(cyphy_dc, focusObject);
				converter.reconstructExtractAssociations(focusObject);
				cyphy_dc.DeleteObject();					
				converter.endProgressBar();
			}
			return;
		}
	}
}

void refactor_single_CA(CyPhyML::RootFolder &cyphy_rf, Udm::Object &focusObject, CyPhyML::ComponentAssembly &cyphy_ca, CyPhyML::ComponentRef &cyphy_comref)
{
	set<Udm::Object> ca_objs = cyphy_ca.GetChildObjects();
	if(ca_objs.empty())
	{
		//no action for the empty ca
		std::string msg="The ComponentAssembly: <"+(std::string)cyphy_ca.name()+"> is empty.";

		GMEConsole::Console::Out::writeLine(msg);

		if(Automation) {
			ExceptionMessage = CString(msg.c_str());
			throw;
		}

		return;
	}

	RefactorType refactorType;
	if(Automation) {
		if(RefactoredTypeOrAction == "ComponentAssembly")
			refactorType = genCA;
		else if(RefactoredTypeOrAction == "DesignContainer")
			refactorType = genDC;
		else {
			refactorType = extract;
		}

	} else {
		CCARefactorDialog dlg;
		if(dlg.DoModal()==IDCANCEL)
			return;

		refactorType = dlg.rt;
	}


	// Make sure that extract is not chosen on a CA reference
	if(refactorType == extract && cyphy_comref != Udm::null)
	{
		std::string msg="Extract cannot be used on a ComponentAssembly reference model.";
		GMEConsole::Console::Out::writeLine(msg);

		if(Automation) {
			ExceptionMessage = CString(msg.c_str());
			throw;
		}

		return;
	}


	switch(refactorType)
	{
	case genDC:
		{
			if(cyphy_comref == Udm::null)
				generateDesignContainerFromCA(cyphy_rf, cyphy_ca);
			else
				generateDesignContainerFromCARef(cyphy_rf, cyphy_ca, cyphy_comref);
		}
		break;

	case genCA:
		{
			CyPhyDSEConverter converter(cyphy_rf);
			converter.startProgressBar();
			converter.initCopyMap(focusObject);
			std::string newcaname;
			if(cyphy_comref==Udm::null)
				//newcaname = "NewCA__"+StripComponentNameSuffix(UdmUtil::ExtractName(cyphy_ca));
				newcaname = "NewCA__"+UdmUtil::ExtractName(cyphy_ca);
			else
				//newcaname = "NewCA__"+StripComponentNameSuffix(UdmUtil::ExtractName(cyphy_comref));
				newcaname = "NewCA__"+UdmUtil::ExtractName(cyphy_comref);
			CyPhyML::ComponentAssembly newca = converter.createNewCA(focusObject, newcaname);							
			newca.position() = cyphy_comref==Udm::null?cyphy_ca.position():cyphy_comref.position();
			if(cyphy_comref==Udm::null)
				converter.createNewObject(cyphy_ca);
			else
				converter.createNewObject(cyphy_comref);
			converter.reconstructAssociations(focusObject);
			converter.deleteConvertedObjs();
			converter.endProgressBar();
			
			std::string addlog("New ComponentAssembly: "+ appendObjLink(newca)+" is generated.");
			GMEConsole::Console::Out::writeLine(addlog);
		}
		break;

	case extract:
		{
			CyPhyDSEConverter converter(cyphy_rf);
			converter.startProgressBar();
			converter.initCopyMap(focusObject);
			converter.gatherConnections_tobe_reconstruce(cyphy_ca, focusObject);
			converter.create_CA_DC_copy(cyphy_ca, focusObject);
			converter.reconstructExtractAssociations(focusObject);
			if(cyphy_comref==Udm::null)
				cyphy_ca.DeleteObject();
			else
				cyphy_comref.DeleteObject();
			converter.endProgressBar();
		}
		break;
	}
}

void convert_into_newDC(CyPhyML::RootFolder &cyphy_rf, Udm::Object &focusObject, set<Udm::Object> &selectedObjects)
{
	if(focusObject.type()!=CyPhyML::DesignContainer::meta)
		return;

	//get first selected DesignEntity type object for position setting
	Udm::Object firstObj;
	for(set<Udm::Object>::iterator it=selectedObjects.begin();it!=selectedObjects.end();++it)
	{
		if(Uml::IsDerivedFrom((*it).type(), CyPhyML::DesignEntity::meta))
		{
			firstObj = *it;
			break;
		}
	}

	if(firstObj==Udm::null)
	{
		showUsage();
		return;
	}

	CyPhyDSEConverter converter(cyphy_rf);
	if(!Automation)
		converter.startProgressBar();
			
	converter.initCopyMap(focusObject);
	
	std::string newdcname;
	if(selectedObjects.size()==1)
		//newdcname = "NewDC__"+StripComponentNameSuffix(UdmUtil::ExtractName(firstObj));
		newdcname = "NewDC__"+UdmUtil::ExtractName(firstObj);
	else
		newdcname = "NewDC";
	
	CyPhyML::DesignContainer newdc;
	CyPhyML::DesignContainer currdc = CyPhyML::DesignContainer::Cast(focusObject);
	if(selectedObjects.size()==1)
		newdc = converter.createNewDesignContainer(currdc, newdcname, firstObj.type()!=CyPhyML::ComponentAssembly::meta);
	else
		newdc = converter.createNewDesignContainer(currdc, newdcname, false);

	newdc.position() = CyPhyML::DesignEntity::Cast(firstObj).position();
	std::string	newlink = "New DesignContainer: "+ appendObjLink(newdc);
	set<Udm::Object>::iterator i;
	for(i=selectedObjects.begin();i!=selectedObjects.end();++i)
	{
		Udm::Object currObj = *i;
		converter.createNewObject(currObj);
	}

	for(i=selectedObjects.begin();i!=selectedObjects.end();++i)
	{
		Udm::Object currObj = *i;
		if(Uml::IsDerivedFrom(currObj.type(), CyPhyML::DesignElement::meta))
			converter.reconstructReference(CyPhyML::DesignElement::Cast(currObj));
	}
	
	//reconstructReference
	converter.reconstructAssociations(focusObject);
	converter.deleteConvertedObjs();
	if(!Automation)
		converter.endProgressBar();
			
	std::string addlog(newlink +" is generated.");
	GMEConsole::Console::Out::writeLine(addlog);
}

void convert_into_newCA(CyPhyML::RootFolder &cyphy_rf, Udm::Object &focusObject, set<Udm::Object> &selectedObjects)
{
	//get first selected DesignEntity type object for position setting
	Udm::Object firstObj;
	for(set<Udm::Object>::iterator it=selectedObjects.begin();it!=selectedObjects.end();++it)
	{
		if(Uml::IsDerivedFrom((*it).type(), CyPhyML::DesignEntity::meta))
		{
			firstObj = *it;
			break;
		}
	}

	if(firstObj==Udm::null)
	{
		showUsage();
		return;
	}

	CyPhyDSEConverter converter(cyphy_rf);
	if(!Automation)
		converter.startProgressBar();
			
	converter.initCopyMap(focusObject);
	
	std::string newcaname;
	if(selectedObjects.size()==1)
		//newcaname = "NewCA__"+StripComponentNameSuffix(UdmUtil::ExtractName(firstObj));
		newcaname = "NewCA__"+UdmUtil::ExtractName(firstObj);
	else
		newcaname = "NewCA";
				
	CyPhyML::ComponentAssembly newca = converter.createNewCA(focusObject, newcaname);
	newca.position() = CyPhyML::DesignEntity::Cast(firstObj).position();
	std::string newlink = "New ComponentAssembly: "+ appendObjLink(newca);

	set<Udm::Object>::iterator i;
	for(i=selectedObjects.begin();i!=selectedObjects.end();++i)
	{
		Udm::Object currObj = *i;
		if(Uml::IsDerivedFrom(currObj.type(), CyPhyML::Component::meta) 
		|| Uml::IsDerivedFrom(currObj.type(), CyPhyML::ComponentAssembly::meta)
		|| Uml::IsDerivedFrom(currObj.type(), CyPhyML::ComponentRef::meta))
		converter.createNewObject(currObj);
	}

	for(i=selectedObjects.begin();i!=selectedObjects.end();++i)
	{
		Udm::Object currObj = *i;
		if(Uml::IsDerivedFrom(currObj.type(), CyPhyML::DesignElement::meta))
			converter.reconstructReference(CyPhyML::DesignElement::Cast(currObj));
	}
	
	//reconstructReference
	converter.reconstructAssociations(focusObject);
	converter.deleteConvertedObjs();
	if(!Automation)
		converter.endProgressBar();
			
	std::string addlog(newlink +" is generated.");
	GMEConsole::Console::Out::writeLine(addlog);
}