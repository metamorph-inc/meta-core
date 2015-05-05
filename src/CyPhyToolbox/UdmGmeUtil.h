#pragma once

#include "UdmBase.h"

// Caveats:
// will crash if destination or objectsToCopy is not backed by a Gme object
// throws _com_error if:
//  objectsToCopy containts a reference with externally-connected refports
//  objectsToCopy and destination are in different DataNetworks
//  there is no open RW transaction
// does nothing if destination is not a model or folder
void CopyGMEObjects(const Udm::Object& destination, const std::set<Udm::Object>& objectsToCopy);
// returns: the copy of fcoToCopy
Udm::Object CopyGMEObjects(const Udm::Object& destination, const Udm::Object& fcoToCopy, const std::string& rolename="");
//Udm::Object CopyGMEObjects(const Udm::Object& destination, const Udm::Object& fcoToCopy, const Uml::CompositionChildRole& rolename=Udm::NULLCHILDROLE);

std::string GetChildRole(const Udm::Object& o);

void RedirectReference(Udm::Object& reference, Udm::Object& to);

Udm::Object GetConnectionEnd(const Udm::Object& connection, const wchar_t* dstOrSrc);

bool IsDestRefport(const Udm::Object& connection);
Udm::Object GetRefport(const Udm::Object& connection, const wchar_t* dstOrSrc);

Udm::Object createComposition(const Uml::Class type,const Udm::Object& src,const Udm::Object& dst,const Udm::Object& parent,const Udm::Object& srcRefParent,const Udm::Object& dstRefParent, const char* compositionRole=NULL);

Udm::Object changeConnection(const wchar_t* dstOrSrc, const Udm::Object& connection, const Udm::Object& endpoint, Udm::Object refportParent);

void SwitchConnections(Udm::Object fcoFrom, Udm::Object fcoTo, Udm::Object throughRefport, Udm::Object parent);
void SwitchConnections(Udm::Object connectionContainer, const map<Udm::Object, Udm::Object>& old_to_new_map);
void SwitchRefportConnections(Udm::Object referenceFrom, Udm::Object referenceTo, std::map<Udm::Object, Udm::Object>& portMap, UdmGme::GmeDataNetwork& dn);

Udm::Object getReferredOrNull(Udm::Object& refOrAnything);

void DetachFromArchetype(Udm::Object derived);
