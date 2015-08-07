#include "UdmGME.h"

#include <algorithm>
#include <string>

#ifndef __INTELLISENSE__
#import "progid:Mga.MgaFolders" /*no_implementation*/ auto_search no_namespace no_search_namespace
#import "libid:BF48BD03-072D-41A4-95F1-3200310005A1" auto_search
#else
// IntelliSense has a known issue with the above lines.
//  http://connect.microsoft.com/VisualStudio/feedback/details/533526/vc-2010-intellisense-import-directive-using-libid-does-not-work
#ifdef _DEBUG
// If IntelliSense reports "Cannot open source file", compile then reopen the project
#include "Debug\Mga.tlh"
#include "Debug\ReferenceSwitcher.tlh"
#else
#include "Release\Mga.tlh"
#include "Release\ReferenceSwitcher.tlh"
#endif
#endif

// from Mga.idl (FIXME: why isnt this in the typelib?)
const long RELID_BASE_MAX  = 0x7FFFFFF;

#define COMTHROW(x) { HRESULT hr = x; if (FAILED(x)) _com_raise_error(x); }
std::string GetErrorMessage(_com_error& e)
{
	if (e.Description().length())
		return static_cast<const char*>(e.Description());
	return static_cast<const char*>(_bstr_t(e.ErrorMessage()));
}

// Caveats:
// will crash if destination or objectsToCopy is not backed by a Gme object
// throws udm_exception if:
//  objectsToCopy containts a reference with externally-connected refports
//  objectsToCopy and destination are in different DataNetworks
//  there is no open RW transaction
// does nothing if destination is not a model or folder
void CopyGMEObjects(const Udm::Object& destination, const std::set<Udm::Object>& objectsToCopy, IMgaMetaRolesPtr roles, IMgaFCOsPtr& copiedObjects)
{
	try {
		IUnknownPtr pDestination(UdmGme::Udm2Gme(destination), false);
		IMgaFolderPtr folderDestination = pDestination;
		IMgaModelPtr modelDestination = pDestination;

		IMgaFCOsPtr fcosToCopy;
		COMTHROW(fcosToCopy.CreateInstance(L"Mga.MgaFCOs", NULL, CLSCTX_INPROC));

		std::for_each(objectsToCopy.begin(), objectsToCopy.end(), [&](const Udm::Object& o)
		{
			IUnknownPtr punk(UdmGme::Udm2Gme(o), false);
			IMgaFCOPtr fco = punk;
			fcosToCopy->Append(fco);
		});

		if (modelDestination)
			modelDestination->CopyFCOs(fcosToCopy, roles, &copiedObjects);
		if (folderDestination)
			folderDestination->CopyFCOs(fcosToCopy, &copiedObjects);
	} catch (_com_error& e) {
		throw udm_exception(GetErrorMessage(e));
	}
}

// returns: the copy of fcoToCopy
Udm::Object CopyGMEObjects(const Udm::Object& destination, const Udm::Object& fcoToCopy, const std::string& rolename="")
{
	try {
		std::set<Udm::Object> fcosToCopy;
		fcosToCopy.insert(fcoToCopy);

		IUnknownPtr punkDest(UdmGme::Udm2Gme(destination), false);
		IMgaModelPtr modelDestination = punkDest;

		IMgaMetaRolesPtr roles;
		if (modelDestination && rolename != "") {
			COMTHROW(roles.CreateInstance(L"Mga.MgaMetaRoles", NULL, CLSCTX_INPROC));
			IMgaMetaModelPtr metaModel = modelDestination->Meta;
			IMgaMetaRolePtr role = metaModel->RoleByName[_bstr_t(rolename.c_str())];
			if (role == NULL) 
				throw udm_exception("Couldn't find rolename " + rolename + " for parent " + static_cast<const char*>(metaModel->Name));
			roles->Append(role);
		}
		IMgaFCOsPtr copieds;
		CopyGMEObjects(destination, fcosToCopy, roles, copieds);
		IMgaFCOPtr copied;
		copieds->get_Item(1, &copied);
		return static_cast<UdmGme::GmeDataNetwork*>(destination.__impl()->__getdn())->Gme2Udm(copied);
	} catch (_com_error& e) {
		throw udm_exception(GetErrorMessage(e));
	}
}

/*Udm::Object CopyGMEObjects(const Udm::Object& destination, const Udm::Object& fcoToCopy, const Uml::CompositionChildRole& rolename=Udm::NULLCHILDROLE) {
	return CopyGMEObjects(destination, fcoToCopy, static_cast<std::string>(rolename.name()));
}*/

void CopyGMEObjects(const Udm::Object& destination, const std::set<Udm::Object>& objectsToCopy) {
	CopyGMEObjects(destination, objectsToCopy, NULL, IMgaFCOsPtr());
}

std::string GetChildRole(const Udm::Object& o)
{
	IMgaFCOPtr fco = IUnknownPtr(UdmGme::Udm2Gme(o), false);
	return static_cast<const char*>(fco->MetaRole->Name);
}

void RedirectReference(Udm::Object& reference, Udm::Object& to)
{
	try
	{	
	ReferenceSwitcher::_ReferenceSwitcherInterpreterPtr p;
	if (FAILED(p.CreateInstance(L"{0CB32575-EB7C-4363-A9E6-4F6F0EC6EC84}", 0, CLSCTX_INPROC)))
		throw udm_exception("Could not create ReferenceSwitcher");
	IMgaReferencePtr ref = IUnknownPtr(UdmGme::Udm2Gme(reference), false);
	IMgaFCOPtr fcoTo = IUnknownPtr(UdmGme::Udm2Gme(to), false);
	p->SwitchReference(fcoTo, ref);
	} catch (_com_error& e) {
		throw udm_exception(GetErrorMessage(e));
	}
}

bool IsDestRefport(const Udm::Object& connection)
{
	try
	{	
	IMgaConnectionPtr conn = IUnknownPtr(UdmGme::Udm2Gme(connection), false);
	IMgaConnPointsPtr connPoints = conn->ConnPoints;
	IMgaFCOsPtr references;
	if (connPoints->GetItem(1)->ConnRole == _bstr_t(L"dst"))
	{
		references = connPoints->GetItem(1)->References;
	} else {
		references = connPoints->GetItem(2)->References;
	}
	return references->Count != 0;
	} catch (_com_error& e) {
		throw udm_exception(GetErrorMessage(e));
	}
}

Udm::Object GetRefport(const Udm::Object& connection, const wchar_t* dstOrSrc)
{
	try
	{	
	IMgaConnectionPtr conn = IUnknownPtr(UdmGme::Udm2Gme(connection), false);
	IMgaConnPointsPtr connPoints = conn->ConnPoints;
	IMgaFCOsPtr references;
    if (connPoints->Count > 0)
    {
        if (connPoints->GetItem(1)->ConnRole == _bstr_t(dstOrSrc))
        {
            references = connPoints->GetItem(1)->References;
        } else {
            if (connPoints->Count > 1) {
                references = connPoints->GetItem(2)->References;
            }
        }
    }
	if (references != nullptr && references->Count != 0)
	{
		return static_cast<UdmGme::GmeDataNetwork*>(connection.__impl()->__getdn())->Gme2Udm(references->GetItem(1));
	}
	return Udm::null;
	} catch (_com_error& e) {
		throw udm_exception(GetErrorMessage(e));
	}
}

Udm::Object GetConnectionEnd(const Udm::Object& connection, const wchar_t* dstOrSrc)
{
	try
	{	
	IMgaConnectionPtr conn = IUnknownPtr(UdmGme::Udm2Gme(connection), false);
	IMgaConnPointsPtr connPoints = conn->ConnPoints;
	IMgaFCOPtr end;
    if (connPoints->Count > 0)
    {
        if (connPoints->GetItem(1)->ConnRole == _bstr_t(dstOrSrc))
        {
            end = connPoints->GetItem(1)->GetTarget();
        } else {
            if (connPoints->Count > 1)
                end = connPoints->GetItem(2)->GetTarget();
        }
    }
	if (end)
	{
		return static_cast<UdmGme::GmeDataNetwork*>(connection.__impl()->__getdn())->Gme2Udm(end);
	}
	return Udm::null;
	} catch (_com_error& e) {
		throw udm_exception(GetErrorMessage(e));
	}
}

static IMgaFCOsPtr GetRefChain(IMgaFCO* fco)
{
	try {
	IMgaFCOsPtr srcrefs;
	COMTHROW(srcrefs.CreateInstance(L"Mga.MgaFCOs", NULL, CLSCTX_INPROC));

	IMgaReferencePtr ref = fco;
	while (srcrefs && ref)
	{
		srcrefs->Append(ref);
		IMgaFCOPtr referred = ref->Referred;
		if (referred)
		{
			ref.Release();
			ref = referred;
		}
	}
	return srcrefs;
	} catch (_com_error& e) {
		throw udm_exception(GetErrorMessage(e));
	}
}

Udm::Object createComposition(const Uml::Class type,const Udm::Object& src,const Udm::Object& dst,const Udm::Object& parent,const Udm::Object& srcRefParent,const Udm::Object& dstRefParent, const char* compositionRole=NULL)
{
	try {
	IMgaModelPtr gparent = IUnknownPtr(UdmGme::Udm2Gme(parent), false);
	IMgaFCOPtr gsrc = IUnknownPtr(UdmGme::Udm2Gme(src), false);
	IMgaFCOPtr gdst = IUnknownPtr(UdmGme::Udm2Gme(dst), false);
	IMgaFCOPtr gsrcRP;
	if (srcRefParent)
		gsrcRP = IUnknownPtr(UdmGme::Udm2Gme(srcRefParent), false);
	IMgaFCOPtr gdstRP;
	if (dstRefParent)
		gdstRP = IUnknownPtr(UdmGme::Udm2Gme(dstRefParent), false);
	IMgaMetaRolePtr role;
	IMgaMetaModelPtr metaModel = gparent->Meta;
	if (compositionRole)
	{
		role = metaModel->RoleByName[_bstr_t(compositionRole)];
		if (role == NULL) 
			throw udm_exception(std::string("Couldn't find rolename ") + compositionRole + " for parent " + static_cast<const char*>(metaModel->Name));
	}
	else
	{
		role = metaModel->RoleByName[static_cast<std::string>(type.name()).c_str()];
		if (role == NULL) 
			throw udm_exception(std::string("Couldn't find rolename for child kind ") + static_cast<std::string>(type.name()) + " for parent " + static_cast<const char*>(metaModel->Name));
	}

	IMgaFCOPtr gconn = gparent->CreateSimpleConn(role, gsrc, gdst, srcRefParent ? GetRefChain(gsrcRP) : NULL, dstRefParent ? GetRefChain(gdstRP) : NULL);
	return static_cast<UdmGme::GmeDataNetwork*>(parent.__impl()->__getdn())->Gme2Udm(gconn);
	} catch (_com_error& e) {
		throw udm_exception(GetErrorMessage(e));
	}
}

Udm::Object changeConnection(const wchar_t* dstOrSrc, const Udm::Object& connection, const Udm::Object& endpoint, Udm::Object refportParent)
{
    try {
    IMgaSimpleConnectionPtr gconn = IUnknownPtr(UdmGme::Udm2Gme(connection), false);
    IMgaFCOPtr gendpoint = IUnknownPtr(UdmGme::Udm2Gme(endpoint), false);
    IMgaFCOPtr grefportParent;
    if (refportParent)
        grefportParent = IUnknownPtr(UdmGme::Udm2Gme(refportParent), false);

    if (wcscmp(dstOrSrc, L"dst"))
    {
        gconn->SetDst(grefportParent ? GetRefChain(grefportParent) : NULL, gendpoint);
    }
    else
    {
        gconn->SetSrc(grefportParent ? GetRefChain(grefportParent) : NULL, gendpoint);
    }

 	} catch (_com_error& e) {
		throw udm_exception(GetErrorMessage(e));
	}
    return connection;
}

void SwitchConnections(Udm::Object fcoFrom, Udm::Object fcoTo, Udm::Object throughRefport, Udm::Object parent)
{
try {
    IMgaFCOPtr gfcoFrom = IUnknownPtr(UdmGme::Udm2Gme(fcoFrom), false);
    IMgaFCOPtr gfcoTo = IUnknownPtr(UdmGme::Udm2Gme(fcoTo), false);
	IMgaReferencePtr gthroughRefport;
    if (throughRefport)
        gthroughRefport = IUnknownPtr(UdmGme::Udm2Gme(throughRefport), false);
    IMgaFCOPtr gparent = IUnknownPtr(UdmGme::Udm2Gme(parent), false);

	IMgaConnPointsPtr connPoints;
	if (gthroughRefport)
		connPoints = gthroughRefport->UsedByConns;
	else
		connPoints = gfcoFrom->GetPartOfConns();
    for (int i = 1; i <= connPoints->Count; i++)
    {
        IMgaConnPointPtr connPoint = connPoints->GetItem(i);
        IMgaSimpleConnectionPtr conn = connPoint->Owner;

		if (gthroughRefport && connPoint->Target->ID != gfcoFrom->ID)
			continue;
		if (gthroughRefport && connPoint->References->GetItem(1)->ID != gthroughRefport->ID)
			continue;
        if (!gthroughRefport && (connPoint->Owner->ParentModel->ID != gparent->ID))
            continue;
		/*
		_bstr_t rp;
		if (gthroughRefport != nullptr)
			rp = gthroughRefport->AbsPath;
		_bstr_t from = gfcoFrom->AbsPath;
		_bstr_t to = gfcoTo->AbsPath;
		_bstr_t connsrc = conn->Src->AbsPath;
		_bstr_t conndst = conn->Dst->AbsPath;
		_bstr_t conn_ = conn->AbsPath;
		*/

        if (wcscmp(L"src", connPoint->ConnRole) == 0)
        {
            conn->SetSrc(NULL, gfcoTo);
        }
        else
        {
            conn->SetDst(NULL, gfcoTo);
        }
    }
    
} catch (_com_error& e) {
	throw udm_exception(GetErrorMessage(e));
}
}


void SwitchRefportConnections(Udm::Object referenceFrom, Udm::Object referenceTo, std::map<Udm::Object, Udm::Object>& portMap, UdmGme::GmeDataNetwork& dn)
{
try {
    IMgaReferencePtr gReferenceFrom = IUnknownPtr(UdmGme::Udm2Gme(referenceFrom), false);
    IMgaReferencePtr gReferenceTo = IUnknownPtr(UdmGme::Udm2Gme(referenceTo), false);
	IMgaFCOsPtr gReferenceToFCOs;
	COMTHROW(gReferenceToFCOs.CreateInstance(L"Mga.MgaFCOs", NULL, CLSCTX_INPROC));
	IMgaModelPtr gToRefModel = nullptr;
	IMgaReferencePtr chain = gReferenceTo;
	while (chain)
	{
		gReferenceToFCOs->Append(chain);
		IMgaFCOPtr referred = chain->Referred;
		chain = nullptr;
		chain = referred;
		gToRefModel = referred;
	}

	IMgaConnPointsPtr connPoints = gReferenceFrom->UsedByConns;
    for (int i = 1; i <= connPoints->Count; i++)
    {
        IMgaConnPointPtr connPoint = connPoints->GetItem(i);
        IMgaSimpleConnectionPtr conn = connPoint->Owner;

		if (connPoint->References->GetItem(1)->ID != gReferenceFrom->ID)
			continue;
		Udm::Object mappedTarget;
		auto portMapIt = portMap.find(dn.Gme2Udm(connPoint->Target));
		while (portMapIt != portMap.end())
		{
			mappedTarget = portMapIt->second;
			portMapIt = portMap.find(portMapIt->second);
		}
		IMgaFCOPtr target;
		if (mappedTarget)
		{
			target = IUnknownPtr(UdmGme::Udm2Gme(mappedTarget), false);
		}
		//bool d1 = false, d2 = false;
		if (!mappedTarget && gToRefModel)
		{
			long relID = connPoint->Target->RelID;
			bool derived = gToRefModel->IsInstance || gToRefModel->ArcheType != nullptr;
			if (derived)
			{
				relID = relID | (RELID_BASE_MAX+1);
			}
			else
			{
				relID = relID & ~(RELID_BASE_MAX+1);
			}

			target = gToRefModel->ChildObjectByRelID[relID];
		//	if (target != nullptr && wcscmp(target->Name, connPoint->Target->Name) != 0)
		//		DebugBreak();
		}
		if (!target)
		{
			throw udm_exception(std::string("Bug: no mapped object for port ") 
				+ static_cast<const char *>(connPoint->Target->AbsPath));
		}
		// _bstr_t d1 = target->AbsPath;
		// _bstr_t d2 = target->Name;
		// _bstr_t d3 = gReferenceToFCOs->GetItem(1)->AbsPath;
		// _bstr_t d4 = gReferenceToFCOs->GetItem(1)->Name;
		// _bstr_t d5 = IMgaReferencePtr(gReferenceToFCOs->GetItem(1))->Referred->AbsPath;
		// _bstr_t d5 = IMgaReferencePtr(gReferenceToFCOs->GetItem(1))->Referred->AbsPath;
        if (wcscmp(L"src", connPoint->ConnRole) == 0)
        {
            conn->SetSrc(gReferenceToFCOs, target);
        }
        else
        {
            conn->SetDst(gReferenceToFCOs, target);
        }
    }
    
} catch (_com_error& e) {
	throw udm_exception(GetErrorMessage(e));
}
}


Udm::Object getReferredOrNull(Udm::Object& refOrAnything)
{
try {
    IMgaReferencePtr ref = IUnknownPtr(UdmGme::Udm2Gme(refOrAnything), false);
    if (ref)
    {
        if (ref->Referred)
            return static_cast<UdmGme::GmeDataNetwork*>(refOrAnything.__impl()->__getdn())->Gme2Udm(ref->Referred);
    }
    return Udm::null;
} catch (_com_error& e) {
	throw udm_exception(GetErrorMessage(e));
}
}


void SwitchConnections(Udm::Object context, const map<Udm::Object, Udm::Object>& old_to_new_map)
{
try {
    IMgaModelPtr gContext = IUnknownPtr(UdmGme::Udm2Gme(context), false);
	for (int i = 1; i <= gContext->ChildFCOs->Count; i++)
	{
		IMgaSimpleConnectionPtr conn = gContext->ChildFCOs->GetItem(i);
		if (conn == nullptr)
			continue;
		
		Udm::Object srcParent = static_cast<UdmGme::GmeDataNetwork*>(context.__impl()->__getdn())->Gme2Udm(conn->Src->ParentModel);
		auto srcEntry = old_to_new_map.find(srcParent);
		if (srcEntry != old_to_new_map.end())
		{
			IMgaReferencePtr ref = IUnknownPtr(UdmGme::Udm2Gme(srcEntry->second), false);
			IMgaFCOsPtr refs;
			refs.CreateInstance(L"Mga.MgaFCOs");
			refs->Append(ref);
			if (conn->Src->IsInstance)
				conn->SetSrc(refs, conn->Src->DerivedFrom);
			else
			{
				std::wstring relpath = std::wstring(conn->Src->AbsPath).substr(conn->Src->ParentModel->AbsPath.length());
				IMgaFCOPtr src = ref->Referred->ObjectByPath[_bstr_t(relpath.c_str())];
				conn->SetSrc(refs, src);
			}
		}

		Udm::Object dstParent = static_cast<UdmGme::GmeDataNetwork*>(context.__impl()->__getdn())->Gme2Udm(conn->Dst->ParentModel);
		auto dstEntry = old_to_new_map.find(dstParent);
		if (dstEntry != old_to_new_map.end())
		{
			IMgaReferencePtr ref = IUnknownPtr(UdmGme::Udm2Gme(dstEntry->second), false);
			IMgaFCOsPtr refs;
			refs.CreateInstance(L"Mga.MgaFCOs");
			refs->Append(ref);
			if (conn->Dst->IsInstance)
				conn->SetDst(refs, conn->Dst->ArcheType);
			else
			{
				std::wstring relpath = std::wstring(conn->Dst->AbsPath).substr(conn->Dst->ParentModel->AbsPath.length());
				IMgaFCOPtr dst = ref->Referred->ObjectByPath[_bstr_t(relpath.c_str())];
				conn->SetDst(refs, dst);
			}
		}
	}    
} catch (_com_error& e) {
	throw udm_exception(GetErrorMessage(e));
}
}

void DetachFromArchetype(Udm::Object derived)
{
try {
    IMgaModelPtr gDerived = IUnknownPtr(UdmGme::Udm2Gme(derived), false);
	gDerived->DetachFromArcheType();
} catch (_com_error& e) {
	throw udm_exception(GetErrorMessage(e));
}
}
