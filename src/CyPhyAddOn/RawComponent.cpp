///////////////////////////////////////////////////////////////////////////
// RawComponent.cpp, the main RAW COM component implementation file
// This is the file (along with its header RawComponent.h)
// that the component implementor is expected to modify in the first place
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#ifndef __INTELLISENSE__
// Meta
#import "libid:0ADEEC71-D83A-11D3-B36B-005004D38590" no_namespace raw_method_prefix("") high_method_prefix("imp_")
// Mga
#import "libid:270B4F86-B17C-11D3-9AD1-00AA00B6FE26" no_namespace raw_method_prefix("") high_method_prefix("imp_")
#else
// IntelliSense has a known issue with the above lines.
//  http://connect.microsoft.com/VisualStudio/feedback/details/533526/vc-2010-intellisense-import-directive-using-libid-does-not-work
#ifdef _DEBUG
// If IntelliSense reports "Cannot open source file", compile then reopen the project
#include "Debug\Meta.tlh"
#include "Debug\Mga.tlh"
#else
#include "Release\Meta.tlh"
#include "Release\Mga.tlh"
#endif
#endif

#include "ComHelp.h"
#include "GMECOM.h"
#include <ComponentConfig.h>
#include "RawComponent.h"
#include <objbase.h>
#include <climits>
#include <algorithm>
#include <iterator>

#define UDM_DYNAMIC_LINKING
#include "CyPhyML.h"
#include "UmlExt.h"

static void UpdateFCORegistry(IMgaFCO* fco, const wchar_t* path, const wchar_t* value)
{
	_bstr_t bPath(path);
	_bstr_t bValue(value);
	_bstr_t old;
	fco->get_RegistryValue(bPath, old.GetAddress());
	if (old.length() == 0 || wcscmp(old, value) != 0)
	{
		fco->put_RegistryValue(bPath, bValue);
	}
}

void RawComponent::createStringSetForMeta_id()
{
	_bstr_t types[] = {
		L"DesignContainer",
		L"Component",
		L"ComponentAssembly",
		L"CyberComponent",
		L"VisualConstraint",
		L"Constraint",
		L"TestComponent",
		L"AlternativeRepresentationContainer",
		L"ComponentRef" };

	std::move(types, types + sizeof(types)/sizeof(types[0]), std::inserter(numericIdfcoKinds, numericIdfcoKinds.end()));

	_bstr_t guidTypes[] = {
		L"Resource",
		L"Connector",
	};
	std::move(guidTypes, guidTypes + sizeof(guidTypes)/sizeof(guidTypes[0]), std::inserter(guidIdfcoKinds, guidIdfcoKinds.end()));
	CyPhyML::Initialize(); // FIXME probably pretty inefficient
	auto ports = Uml::DescendantClasses(CyPhyML::DomainModelPort::meta);
	auto& guidIdfcoKinds = this->guidIdfcoKinds;
	// n.b. this is wrong if there are namespaces
	std::for_each(ports.begin(), ports.end(), [&guidIdfcoKinds](const Uml::Class& class_) { guidIdfcoKinds.insert(_bstr_t((static_cast<std::string>(class_.name())).c_str())); });
	// std::for_each(guidIdfcoKinds.begin(), guidIdfcoKinds.end(), [](const _bstr_t& str) { OutputDebugStringW(str); OutputDebugStringW(L"\n"); });
}

CString RawComponent::getFCOMetaName(IMgaFCO *fco)
{
	CBstr bstr;	
	CComPtr<IMgaMetaFCO> meta;
	COMTHROW(fco->get_Meta(&meta));
	COMTHROW(meta->get_Name(bstr));	
	return CString(bstr);
}

CString RawComponent::getObjectMetaDisplayedName(IMgaObject *obj)
{
	CBstr bstr;
	CComPtr<IMgaMetaBase> meta;
	COMTHROW(obj->get_MetaBase(&meta));
	COMTHROW(meta->get_DisplayedName(bstr));	
	return CString(bstr);
}

bool RawComponent::isDesignElement(IMgaFCO *fco)
{
	CString metaName = getFCOMetaName(fco);
	if(metaName==L"DesignContainer")
		return true;
	if(metaName==L"Component" || metaName==L"CyberComponent" || metaName==L"ComponentAssembly" || metaName==L"ComponentRef")
	{
		CComPtr<IMgaModel> parent_model;
		HRESULT _hr = fco->get_ParentModel(&parent_model);
		if( FAILED(_hr) ) return false;
		if(!parent_model) return false;
		CComQIPtr<IMgaFCO> parent_fco(parent_model);
		CString parent_metaName = getFCOMetaName(parent_fco);
		if(parent_metaName==L"DesignContainer")
			return true;
	}
	else if(metaName==L"VisualConstraint" || metaName==L"Constraint")
		return true;
	return false;
}

void RawComponent::traverseFolder(IMgaFolder *fdr)
{
	VARIANT_BOOL isLib;
	fdr->get_IsLibObject(&isLib);
	if(isLib) return;
	CComPtr<IMgaFolders> fdrs;
	fdr->get_ChildFolders(&fdrs);
	MGACOLL_ITERATE(IMgaFolder,fdrs) 
	{	
		traverseFolder(MGACOLL_ITER);
	}
	MGACOLL_ITERATE_END;

	CComPtr<IMgaFCOs> fcos;
	COMTHROW(fdr->get_ChildFCOs(&fcos));
	MGACOLL_ITERATE(IMgaFCO,fcos) 
	{	
		_bstr_t metaName = MGACOLL_ITER->Meta->Name;
		traverseFCO(MGACOLL_ITER, metaName);
	}
	MGACOLL_ITERATE_END;
}

void RawComponent::traverseFCO(IMgaFCO *fco, _bstr_t &metaName)
{
	if(numericIdfcoKinds.find(metaName)==numericIdfcoKinds.end()) return;

	long currid = 0;
	COMTHROW(fco->get_IntAttrByName(CBstrIn(L"ID"), &currid));
	
	if(currid > maxId)
	{
		maxId = currid;
	}
	else if(currid <= 0)
	{
		fcolist.push_back(fco);
	}

	_bstr_t name = fco->Name;
	objtype_enum type = fco->ObjType;
	if(type != OBJTYPE_MODEL)
		return;
	CComPtr<IMgaObjects> objs;
	COMTHROW(fco->get_ChildObjects(&objs));
	MGACOLL_ITERATE(IMgaObject, objs)
	{
		CComQIPtr<IMgaFCO> child_fco(MGACOLL_ITER);
		_bstr_t child_metaName = child_fco->Meta->Name;
		traverseFCO(child_fco, child_metaName);
	}
	MGACOLL_ITERATE_END;
}

// this method is called after all the generic initialization is done
// this should be empty, unless application-specific initialization is needed
STDMETHODIMP RawComponent::Initialize(struct IMgaProject *project) {
	
	maxId = 0;
	ids.clear();
	createStringSetForMeta_id();

	//this->project = project;
	//CComPtr<IMgaTerritory> terr;
	//CComBSTR mslProject = 0;
	//CComVariant hostGuid;
	//COMTHROW(project->CreateTerritory(NULL, &terr));
	//COMTHROW(project->BeginTransaction(terr, TRANSACTION_GENERAL));
	//try {
	//	CComPtr<IMgaFolder> rf;
	//	COMTHROW(project->get_RootFolder(&rf));
	//	
	//	//traverseDesignSpace(rf);
	//	traverseFolder(rf);

	//	for (auto fcolistIt = fcolist.begin(); fcolistIt != fcolist.end(); fcolistIt++)
	//	{
	//		CComPtr<IMgaFCO> fco = *fcolistIt;
	//		if(fco)
	//		{
	//			fco->put_IntAttrByName(CBstrIn("ID"), ++maxId);
	//		}
	//	}
	//	
	//	project->CommitTransaction();
	//} catch(HRESULT hr)	{
	//	ASSERT(hr);
	//	project->AbortTransaction();
	//	return hr;
	//}

	return S_OK;
}

// this is the obsolete component interface
// this present implementation either tries to call InvokeEx, or returns an error;
STDMETHODIMP RawComponent::Invoke(IMgaProject* gme, IMgaFCOs *models, long param) {
#ifdef SUPPORT_OLD_INVOKE
	CComPtr<IMgaFCO> focus;
	CComVariant parval = param;
	return InvokeEx(gme, focus, selected, parvar);
#else
	if(interactive) {
		AfxMessageBox(L"This component does not support the obsolete invoke mechanism");
	}
	return E_MGA_NOT_SUPPORTED;
#endif
}


// This is the main component method for interpereters and plugins. 
// May als be used in case of invokeable addons
STDMETHODIMP RawComponent::InvokeEx( IMgaProject *project,  IMgaFCO *currentobj,  
									IMgaFCOs *selectedobjs,  long param)
{
	COMTRY {
	  if(interactive) {
		CComBSTR projname;
		CComBSTR focusname = L"<nothing>";
		CComPtr<IMgaTerritory> terr;
		COMTHROW(project->CreateTerritory(NULL, &terr, 0));
		COMTHROW(project->BeginTransaction(terr, TRANSACTION_GENERAL));
		try {			
			COMTHROW(project->get_Name(&projname));
			if(currentobj) COMTHROW(currentobj->get_Name(&focusname));
			AfxMessageBox(L"RAW Com Component --- Plugin!!!! Sample (project: " + CString(projname) +
						L", focus: " + CString(focusname));
			COMTHROW(project->CommitTransaction());
		}	catch(...) { project->AbortTransaction(); throw; }
		
	  } 
	} COMCATCH(;);
}

// GME currently does not use this function
// you only need to implement it if other invokation mechanisms are used
STDMETHODIMP RawComponent::ObjectsInvokeEx( IMgaProject *project,  IMgaObject *currentobj,  IMgaObjects *selectedobjs,  long param) {
	if(interactive) {
		AfxMessageBox(L"Tho ObjectsInvoke method is not implemented");
	}
	return E_MGA_NOT_SUPPORTED;
}


// implement application specific parameter-mechanism in these functions:
STDMETHODIMP RawComponent::get_ComponentParameter(BSTR name, VARIANT *pVal) {
	return S_OK;
}

bool turnedon = true;
bool dontAssignGUIDsOnNextTransaction = false;

STDMETHODIMP RawComponent::put_ComponentParameter(BSTR name, VARIANT newVal) {
	if (name != 0 && newVal.vt == VT_BOOL)
	{
		if (wcscmp(name,L"turnedon") == 0)
			turnedon = newVal.boolVal != VARIANT_FALSE;
		else if (wcscmp(name,L"dontassignguidsonnexttransaction") == 0 
					&& newVal.boolVal == VARIANT_TRUE)
		{
			dontAssignGUIDsOnNextTransaction = true;
		}
	}
	return S_OK;
}


#ifdef GME_ADDON

// these two functions are the main 
STDMETHODIMP RawComponent::GlobalEvent(globalevent_enum event) { 
	// If flag set, clear flag and skip body.
	if (event == GLOBALEVENT_COMMIT_TRANSACTION
		&& dontAssignGUIDsOnNextTransaction == true)
	{
		dontAssignGUIDsOnNextTransaction = false;
	}
	// If abort, clear flag
	if (event == GLOBALEVENT_ABORT_TRANSACTION)
	{
		dontAssignGUIDsOnNextTransaction = false;
	}

	if(event == GLOBALEVENT_UNDO) 
	{	//AfxMessageBox("UNDO!!");
	}
	if( event == APPEVENT_XML_IMPORT_BEGIN)
		turnedon = false;
	else if( event == APPEVENT_XML_IMPORT_END)
		turnedon = true;
	else if( event == APPEVENT_LIB_ATTACH_BEGIN)
		turnedon = false;
	else if( event == APPEVENT_LIB_ATTACH_END)
		turnedon = true;
	return S_OK; 
}

STDMETHODIMP RawComponent::ObjectEvent(IMgaObject * obj, unsigned long eventmask, VARIANT v) 
{
	VARIANT_BOOL isLibObject;
	COMTHROW(obj->get_IsLibObject(&isLibObject));
	COMTRY {
		if(eventmask & OBJEVENT_CREATED && !(eventmask & OBJEVENT_DESTROYED) && isLibObject == VARIANT_FALSE)
		{
			objtype_enum type;
			COMTHROW(obj->get_ObjType(&type));

			CComBSTR bstrName;
			COMTHROW(obj->get_Name(&bstrName));
			
			CComQIPtr<IMgaObject> comObj(obj);
			CString objectDisplayedName = getObjectMetaDisplayedName(comObj);

			CString name(bstrName.m_str);
			if (name == objectDisplayedName || name == L"New" + objectDisplayedName) {
				name.Replace(L" ", L"");
				COMTHROW(obj->put_Name(_bstr_t(name)));
			}

			if(type != OBJTYPE_FOLDER)	
			{
				CComQIPtr<IMgaFCO> fco(obj);
				_bstr_t fcoKind = fco->Meta->Name;

				if(numericIdfcoKinds.find(fcoKind) != numericIdfcoKinds.end())// && isDesignElement(fco)) || fcoType == L"ComponentAssembly")
				{	
					if(turnedon)
					{
						COMTHROW(fco->put_IntAttrByName(_bstr_t(L"ID"), ++maxId));

						if ( dontAssignGUIDsOnNextTransaction == false &&
							 (wcscmp(fcoKind, L"Component") == 0 || wcscmp(fcoKind, L"ComponentRef") == 0 ) )
						{
							// Populate InstanceGUID field with the object's GUID
							_bstr_t guid = fco->imp_GetGuidDisp();
							_bstr_t noBraces(SysAllocStringLen(static_cast<const OLECHAR*>(guid) + 1, guid.length() - 2), false);
							COMTHROW(fco->put_StrAttrByName(_bstr_t(L"InstanceGUID"), noBraces));
						}
					}
				}
				else if(guidIdfcoKinds.find(fcoKind) != guidIdfcoKinds.end())
				{	
					if(turnedon && dontAssignGUIDsOnNextTransaction == false)
					{
						_bstr_t fcoGuid;
						COMTHROW(fco->GetGuidDisp(fcoGuid.GetAddress()));
						std::wstring guidIdBraces = std::wstring(L"id-") + static_cast<const wchar_t*>(fcoGuid);
						std::wstring guidId;
						std::copy_if(guidIdBraces.begin(), guidIdBraces.end(), std::back_inserter(guidId), [](const std::wstring::value_type c) { return c != L'{' && c != L'}'; });
						fco->StrAttrByName[_bstr_t(L"ID")] = guidId.c_str();
					}
				}
				else if(wcscmp(fcoKind, L"And_operator") == 0 || wcscmp(fcoKind, L"Or_operator") == 0 || wcscmp(fcoKind, L"DesignEntityRef") == 0)
				{
					CComQIPtr<IMgaModel> vcon;
					fco->get_ParentModel(&vcon);
					makeDirtyConfigurations_by_constraint(vcon);
				}
			}
		}
		else if((eventmask & OBJEVENT_ATTR) && turnedon && isLibObject == VARIANT_FALSE && v.vt != VT_EMPTY)
		{
			objtype_enum type;
			COMVERIFY(obj->get_ObjType(&type));
			if(type != OBJTYPE_FOLDER)	
			{
				CComQIPtr<IMgaFCO> fco(obj);
				CString metaName = getFCOMetaName(fco);
				if(metaName == "Constraint")
				{
					CComQIPtr<IMgaFCOs> fcos;
					fco->get_ReferencedBy(&fcos);
					MGACOLL_ITERATE(IMgaFCO, fcos)
					{
						CComQIPtr<IMgaModel> cfgs;
						MGACOLL_ITER->get_ParentModel(&cfgs);
						cfgs->put_BoolAttrByName((CBstrIn)"isDirty",true);
					}
					MGACOLL_ITERATE_END;		
				}
				else if(metaName == "DesignContainer" || metaName=="Component" || metaName=="ComponentAssembly"||metaName=="ComponentRef")
				{
					//if(isDesignElement(fco))
					//{
					//	makeDirtyConfigurations_by_component(fco);
					//}
					if(metaName == "DesignContainer" )
					{
						CBstr ctype;
						if ( ! SUCCEEDED( fco->get_StrAttrByName((CBstrIn)"ContainerType", ctype) ))
							return S_OK;
						CString conType(ctype);
						if (conType=="Compound")
							UpdateFCORegistry(fco, L"icon", L"mandatory_ds.png");
						else if(conType=="Alternative")
							UpdateFCORegistry(fco, L"icon", L"alternative_ds.png");
						else
							UpdateFCORegistry(fco, L"icon", L"optional_ds.png");
					}
				}
				else if(metaName=="Configurations")
				{
					VARIANT_BOOL isDirty;
					if ( ! SUCCEEDED( fco->get_BoolAttrByName((CBstrIn)"isDirty", &isDirty) ) )
						return S_OK;
					if(isDirty)
					{
						fco->put_RegistryValue((CBstrIn)"icon", (CBstrIn)"CfgsDirty.png");
					}
					else 
					{
						fco->put_RegistryValue((CBstrIn)"icon", (CBstrIn)"CfgsClean.png");
					}
				
					VARIANT_BOOL isArchived;
					if ( ! SUCCEEDED( fco->get_BoolAttrByName((CBstrIn)"isArchived", &isArchived) ) )
						return S_OK;
					if(isArchived)
					{
						fco->put_RegistryValue((CBstrIn)"icon", (CBstrIn)"CfgsArchived.png");
					}
				}
				else if(metaName=="Property" || metaName=="Parameter")//check the parent type
				{
					CComQIPtr<IMgaModel> parent;
					fco->get_ParentModel(&parent);
					if(parent)
					{
						CString pName = getFCOMetaName(parent);
						if(pName=="Component")
							if(isDesignElement(parent))
								makeDirtyConfigurations_by_component(parent);
					}
				}
			}
		}
		else if(eventmask & OBJEVENT_PRE_DESTROYED)
		{
			objtype_enum type;
			COMVERIFY(obj->get_ObjType(&type));
			if(type != OBJTYPE_FOLDER)	
			{
				CComQIPtr<IMgaFCO> fco(obj);
				CString metaName = getFCOMetaName(fco);
				if(metaName == "Constraint" || metaName == "VisualConstraint")
				{
					makeDirtyConfigurations_by_constraint(fco);
				}
				else if(metaName == "Configurations" || metaName == "ComponentAssembly")
				{
					CComQIPtr<IMgaFCOs> fcos;
					fco->get_ReferencedBy(&fcos);
					MGACOLL_ITERATE(IMgaFCO, fcos)
					{
						MGACOLL_ITER->DestroyObject();
					}
					MGACOLL_ITERATE_END;
				}
				else if(isDesignElement(fco))
					makeDirtyConfigurations_by_component(fco);
			}
		}
		else if(((eventmask & OBJEVENT_CONNECTED) || (eventmask & OBJEVENT_DISCONNECTED)) && isLibObject == VARIANT_FALSE)
		{
			CComQIPtr<IMgaFCO> fco(obj);		
			CString fcoType = getFCOMetaName(fco);
			if(fcoType == "And_operator" || fcoType == "Or_operator" || fcoType == "DesignEntityRef")
			{
				CComQIPtr<IMgaModel> vcon;
				fco->get_ParentModel(&vcon);
				makeDirtyConfigurations_by_constraint(vcon);
			}
		}

		if ((eventmask & OBJEVENT_ATTR) && turnedon && isLibObject == VARIANT_FALSE)
		{
			CComQIPtr<IMgaFCO> fco(obj);
			if (fco)
			{
				CString metaName = getFCOMetaName(fco);
				if (metaName == "Parameter")
				{
					if (fco->StrAttrByName[L"Range"].length() == 0)
					{
						fco->imp_ClearAttrByName(L"Range");
					}
				}
			}
		}

	} COMCATCH(;)
}

void RawComponent::makeDirtyConfigurations_by_constraint(IMgaFCO *fco)
{
	CComQIPtr<IMgaFCOs> fcos;
	fco->get_ReferencedBy(&fcos);
	MGACOLL_ITERATE(IMgaFCO, fcos)
	{
		CComQIPtr<IMgaModel> cfgs;
		MGACOLL_ITER->get_ParentModel(&cfgs);
		cfgs->put_BoolAttrByName((CBstrIn)"isDirty",true);
	}
	MGACOLL_ITERATE_END;
}

void RawComponent::makeDirtyConfigurations_by_component(IMgaFCO *fco)
{	
	CComQIPtr<IMgaModel> parent;
	if ( ! SUCCEEDED( fco->get_ParentModel(&parent) ) )
		return;
	if(!parent) return;
	CString metaName = getFCOMetaName(parent);
	if(metaName=="DesignContainer")
	{
		CComPtr<IMgaObjects> objs;
		COMTHROW(parent->get_ChildObjects(&objs));
		MGACOLL_ITERATE(IMgaObject, objs)
		{
			CComQIPtr<IMgaFCO> child_fco(MGACOLL_ITER);
			CString child_metaName = getFCOMetaName(child_fco);
			if(child_metaName=="Configurations")
			{
				child_fco->put_BoolAttrByName((CBstrIn)"isDirty",true);
				child_fco->put_RegistryValue((CBstrIn)"icon", (CBstrIn)"CfgsDirty.png");
			}
		}
		MGACOLL_ITERATE_END;

		makeDirtyConfigurations_by_component(parent);
	}
	else if(metaName=="ComponentAssembly")
	{
		makeDirtyConfigurations_by_component(parent);
	}
}

#endif