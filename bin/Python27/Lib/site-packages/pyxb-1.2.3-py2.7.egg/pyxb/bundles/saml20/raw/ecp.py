# ./pyxb/bundles/saml20/raw/ecp.py
# -*- coding: utf-8 -*-
# PyXB bindings for NM:2fcdef609f6a0a75f54ed59ff0ad0ce2350eb6e6
# Generated 2013-09-18 10:35:55.973141 by PyXB version 1.2.3
# Namespace urn:oasis:names:tc:SAML:2.0:profiles:SSO:ecp

import pyxb
import pyxb.binding
import pyxb.binding.saxer
import io
import pyxb.utils.utility
import pyxb.utils.domutils
import sys

# Unique identifier for bindings created at the same time
_GenerationUID = pyxb.utils.utility.UniqueIdentifier('urn:uuid:025e47ee-2078-11e3-89fd-c8600024e903')

# Version of PyXB used to generate the bindings
_PyXBVersion = '1.2.3'
# Generated bindings are not compatible across PyXB versions
if pyxb.__version__ != _PyXBVersion:
    raise pyxb.PyXBVersionError(_PyXBVersion)

# Import bindings for namespaces imported into schema
import pyxb.bundles.saml20.assertion
import pyxb.bundles.wssplat.soap11
import pyxb.binding.datatypes
import pyxb.bundles.saml20.protocol

# NOTE: All namespace declarations are reserved within the binding
Namespace = pyxb.namespace.NamespaceForURI(u'urn:oasis:names:tc:SAML:2.0:profiles:SSO:ecp', create_if_missing=True)
Namespace.configureCategories(['typeBinding', 'elementBinding'])
_Namespace_saml = pyxb.bundles.saml20.assertion.Namespace
_Namespace_saml.configureCategories(['typeBinding', 'elementBinding'])
_Namespace_samlp = pyxb.bundles.saml20.protocol.Namespace
_Namespace_samlp.configureCategories(['typeBinding', 'elementBinding'])
_Namespace_S = pyxb.bundles.wssplat.soap11.Namespace
_Namespace_S.configureCategories(['typeBinding', 'elementBinding'])

def CreateFromDocument (xml_text, default_namespace=None, location_base=None):
    """Parse the given XML and use the document element to create a
    Python instance.

    @param xml_text An XML document.  This should be data (Python 2
    str or Python 3 bytes), or a text (Python 2 unicode or Python 3
    str) in the L{pyxb._InputEncoding} encoding.

    @keyword default_namespace The L{pyxb.Namespace} instance to use as the
    default namespace where there is no default namespace in scope.
    If unspecified or C{None}, the namespace of the module containing
    this function will be used.

    @keyword location_base: An object to be recorded as the base of all
    L{pyxb.utils.utility.Location} instances associated with events and
    objects handled by the parser.  You might pass the URI from which
    the document was obtained.
    """

    if pyxb.XMLStyle_saxer != pyxb._XMLStyle:
        dom = pyxb.utils.domutils.StringToDOM(xml_text)
        return CreateFromDOM(dom.documentElement)
    if default_namespace is None:
        default_namespace = Namespace.fallbackNamespace()
    saxer = pyxb.binding.saxer.make_parser(fallback_namespace=default_namespace, location_base=location_base)
    handler = saxer.getContentHandler()
    xmld = xml_text
    if isinstance(xmld, unicode):
        xmld = xmld.encode(pyxb._InputEncoding)
    saxer.parse(io.BytesIO(xmld))
    instance = handler.rootObject()
    return instance

def CreateFromDOM (node, default_namespace=None):
    """Create a Python instance from the given DOM node.
    The node tag must correspond to an element declaration in this module.

    @deprecated: Forcing use of DOM interface is unnecessary; use L{CreateFromDocument}."""
    if default_namespace is None:
        default_namespace = Namespace.fallbackNamespace()
    return pyxb.binding.basis.element.AnyCreateFromDOM(node, default_namespace)


# Complex type {urn:oasis:names:tc:SAML:2.0:profiles:SSO:ecp}RequestType with content type ELEMENT_ONLY
class RequestType (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {urn:oasis:names:tc:SAML:2.0:profiles:SSO:ecp}RequestType with content type ELEMENT_ONLY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'RequestType')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/saml20/schemas/ecp.xsd', 30, 4)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.anyType
    
    # Element {urn:oasis:names:tc:SAML:2.0:assertion}Issuer uses Python identifier Issuer
    __Issuer = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(_Namespace_saml, u'Issuer'), 'Issuer', '__urnoasisnamestcSAML2_0profilesSSOecp_RequestType_urnoasisnamestcSAML2_0assertionIssuer', False, pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/saml20/schemas/assertion.xsd', 54, 4), )

    
    Issuer = property(__Issuer.value, __Issuer.set, None, None)

    
    # Element {urn:oasis:names:tc:SAML:2.0:protocol}IDPList uses Python identifier IDPList
    __IDPList = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(_Namespace_samlp, u'IDPList'), 'IDPList', '__urnoasisnamestcSAML2_0profilesSSOecp_RequestType_urnoasisnamestcSAML2_0protocolIDPList', False, pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/saml20/schemas/protocol.xsd', 188, 4), )

    
    IDPList = property(__IDPList.value, __IDPList.set, None, None)

    
    # Attribute {http://schemas.xmlsoap.org/soap/envelope/}mustUnderstand uses Python identifier mustUnderstand
    __mustUnderstand = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(_Namespace_S, u'mustUnderstand'), 'mustUnderstand', '__urnoasisnamestcSAML2_0profilesSSOecp_RequestType_httpschemas_xmlsoap_orgsoapenvelopemustUnderstand', pyxb.bundles.wssplat.soap11.STD_ANON, required=True)
    __mustUnderstand._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soap11.xsd', 75, 2)
    __mustUnderstand._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/saml20/schemas/ecp.xsd', 35, 8)
    
    mustUnderstand = property(__mustUnderstand.value, __mustUnderstand.set, None, None)

    
    # Attribute {http://schemas.xmlsoap.org/soap/envelope/}actor uses Python identifier actor
    __actor = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(_Namespace_S, u'actor'), 'actor', '__urnoasisnamestcSAML2_0profilesSSOecp_RequestType_httpschemas_xmlsoap_orgsoapenvelopeactor', pyxb.binding.datatypes.anyURI, required=True)
    __actor._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soap11.xsd', 82, 2)
    __actor._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/saml20/schemas/ecp.xsd', 36, 8)
    
    actor = property(__actor.value, __actor.set, None, None)

    
    # Attribute ProviderName uses Python identifier ProviderName
    __ProviderName = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'ProviderName'), 'ProviderName', '__urnoasisnamestcSAML2_0profilesSSOecp_RequestType_ProviderName', pyxb.binding.datatypes.string)
    __ProviderName._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/saml20/schemas/ecp.xsd', 37, 8)
    __ProviderName._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/saml20/schemas/ecp.xsd', 37, 8)
    
    ProviderName = property(__ProviderName.value, __ProviderName.set, None, None)

    
    # Attribute IsPassive uses Python identifier IsPassive
    __IsPassive = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'IsPassive'), 'IsPassive', '__urnoasisnamestcSAML2_0profilesSSOecp_RequestType_IsPassive', pyxb.binding.datatypes.boolean)
    __IsPassive._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/saml20/schemas/ecp.xsd', 38, 8)
    __IsPassive._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/saml20/schemas/ecp.xsd', 38, 8)
    
    IsPassive = property(__IsPassive.value, __IsPassive.set, None, None)

    _ElementMap.update({
        __Issuer.name() : __Issuer,
        __IDPList.name() : __IDPList
    })
    _AttributeMap.update({
        __mustUnderstand.name() : __mustUnderstand,
        __actor.name() : __actor,
        __ProviderName.name() : __ProviderName,
        __IsPassive.name() : __IsPassive
    })
Namespace.addCategoryObject('typeBinding', u'RequestType', RequestType)


# Complex type {urn:oasis:names:tc:SAML:2.0:profiles:SSO:ecp}ResponseType with content type EMPTY
class ResponseType (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {urn:oasis:names:tc:SAML:2.0:profiles:SSO:ecp}ResponseType with content type EMPTY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_EMPTY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'ResponseType')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/saml20/schemas/ecp.xsd', 42, 4)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.anyType
    
    # Attribute {http://schemas.xmlsoap.org/soap/envelope/}mustUnderstand uses Python identifier mustUnderstand
    __mustUnderstand = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(_Namespace_S, u'mustUnderstand'), 'mustUnderstand', '__urnoasisnamestcSAML2_0profilesSSOecp_ResponseType_httpschemas_xmlsoap_orgsoapenvelopemustUnderstand', pyxb.bundles.wssplat.soap11.STD_ANON, required=True)
    __mustUnderstand._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soap11.xsd', 75, 2)
    __mustUnderstand._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/saml20/schemas/ecp.xsd', 43, 8)
    
    mustUnderstand = property(__mustUnderstand.value, __mustUnderstand.set, None, None)

    
    # Attribute {http://schemas.xmlsoap.org/soap/envelope/}actor uses Python identifier actor
    __actor = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(_Namespace_S, u'actor'), 'actor', '__urnoasisnamestcSAML2_0profilesSSOecp_ResponseType_httpschemas_xmlsoap_orgsoapenvelopeactor', pyxb.binding.datatypes.anyURI, required=True)
    __actor._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soap11.xsd', 82, 2)
    __actor._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/saml20/schemas/ecp.xsd', 44, 8)
    
    actor = property(__actor.value, __actor.set, None, None)

    
    # Attribute AssertionConsumerServiceURL uses Python identifier AssertionConsumerServiceURL
    __AssertionConsumerServiceURL = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'AssertionConsumerServiceURL'), 'AssertionConsumerServiceURL', '__urnoasisnamestcSAML2_0profilesSSOecp_ResponseType_AssertionConsumerServiceURL', pyxb.binding.datatypes.anyURI, required=True)
    __AssertionConsumerServiceURL._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/saml20/schemas/ecp.xsd', 45, 8)
    __AssertionConsumerServiceURL._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/saml20/schemas/ecp.xsd', 45, 8)
    
    AssertionConsumerServiceURL = property(__AssertionConsumerServiceURL.value, __AssertionConsumerServiceURL.set, None, None)

    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __mustUnderstand.name() : __mustUnderstand,
        __actor.name() : __actor,
        __AssertionConsumerServiceURL.name() : __AssertionConsumerServiceURL
    })
Namespace.addCategoryObject('typeBinding', u'ResponseType', ResponseType)


# Complex type {urn:oasis:names:tc:SAML:2.0:profiles:SSO:ecp}RelayStateType with content type SIMPLE
class RelayStateType (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {urn:oasis:names:tc:SAML:2.0:profiles:SSO:ecp}RelayStateType with content type SIMPLE"""
    _TypeDefinition = pyxb.binding.datatypes.string
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'RelayStateType')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/saml20/schemas/ecp.xsd', 49, 4)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.string
    
    # Attribute {http://schemas.xmlsoap.org/soap/envelope/}mustUnderstand uses Python identifier mustUnderstand
    __mustUnderstand = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(_Namespace_S, u'mustUnderstand'), 'mustUnderstand', '__urnoasisnamestcSAML2_0profilesSSOecp_RelayStateType_httpschemas_xmlsoap_orgsoapenvelopemustUnderstand', pyxb.bundles.wssplat.soap11.STD_ANON, required=True)
    __mustUnderstand._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soap11.xsd', 75, 2)
    __mustUnderstand._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/saml20/schemas/ecp.xsd', 52, 16)
    
    mustUnderstand = property(__mustUnderstand.value, __mustUnderstand.set, None, None)

    
    # Attribute {http://schemas.xmlsoap.org/soap/envelope/}actor uses Python identifier actor
    __actor = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(_Namespace_S, u'actor'), 'actor', '__urnoasisnamestcSAML2_0profilesSSOecp_RelayStateType_httpschemas_xmlsoap_orgsoapenvelopeactor', pyxb.binding.datatypes.anyURI, required=True)
    __actor._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soap11.xsd', 82, 2)
    __actor._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/saml20/schemas/ecp.xsd', 53, 16)
    
    actor = property(__actor.value, __actor.set, None, None)

    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __mustUnderstand.name() : __mustUnderstand,
        __actor.name() : __actor
    })
Namespace.addCategoryObject('typeBinding', u'RelayStateType', RelayStateType)


Request = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'Request'), RequestType, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/saml20/schemas/ecp.xsd', 29, 4))
Namespace.addCategoryObject('elementBinding', Request.name().localName(), Request)

Response = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'Response'), ResponseType, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/saml20/schemas/ecp.xsd', 41, 4))
Namespace.addCategoryObject('elementBinding', Response.name().localName(), Response)

RelayState = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'RelayState'), RelayStateType, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/saml20/schemas/ecp.xsd', 48, 4))
Namespace.addCategoryObject('elementBinding', RelayState.name().localName(), RelayState)



RequestType._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(_Namespace_saml, u'Issuer'), pyxb.bundles.saml20.assertion.NameIDType, scope=RequestType, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/saml20/schemas/assertion.xsd', 54, 4)))

RequestType._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(_Namespace_samlp, u'IDPList'), pyxb.bundles.saml20.protocol.IDPListType, scope=RequestType, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/saml20/schemas/protocol.xsd', 188, 4)))

def _BuildAutomaton ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton
    del _BuildAutomaton
    import pyxb.utils.fac as fac

    counters = set()
    cc_0 = fac.CounterCondition(min=0L, max=1, metadata=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/saml20/schemas/ecp.xsd', 33, 12))
    counters.add(cc_0)
    states = []
    final_update = set()
    symbol = pyxb.binding.content.ElementUse(RequestType._UseForTag(pyxb.namespace.ExpandedName(_Namespace_saml, u'Issuer')), pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/saml20/schemas/ecp.xsd', 32, 12))
    st_0 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_0)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_0, False))
    symbol = pyxb.binding.content.ElementUse(RequestType._UseForTag(pyxb.namespace.ExpandedName(_Namespace_samlp, u'IDPList')), pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/saml20/schemas/ecp.xsd', 33, 12))
    st_1 = fac.State(symbol, is_initial=False, final_update=final_update, is_unordered_catenation=False)
    states.append(st_1)
    transitions = []
    transitions.append(fac.Transition(st_1, [
         ]))
    st_0._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_1, [
        fac.UpdateInstruction(cc_0, True) ]))
    st_1._set_transitionSet(transitions)
    return fac.Automaton(states, counters, False, containing_state=None)
RequestType._Automaton = _BuildAutomaton()

