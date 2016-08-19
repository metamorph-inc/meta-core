# ./pyxb/bundles/wssplat/raw/soapenc.py
# -*- coding: utf-8 -*-
# PyXB bindings for NM:f939247c4eea3a1bfab0cd637226c9b45b17deb9
# Generated 2013-09-18 10:35:51.163307 by PyXB version 1.2.3
# Namespace http://schemas.xmlsoap.org/soap/encoding/

import pyxb
import pyxb.binding
import pyxb.binding.saxer
import io
import pyxb.utils.utility
import pyxb.utils.domutils
import sys

# Unique identifier for bindings created at the same time
_GenerationUID = pyxb.utils.utility.UniqueIdentifier('urn:uuid:ff80a404-2077-11e3-a9af-c8600024e903')

# Version of PyXB used to generate the bindings
_PyXBVersion = '1.2.3'
# Generated bindings are not compatible across PyXB versions
if pyxb.__version__ != _PyXBVersion:
    raise pyxb.PyXBVersionError(_PyXBVersion)

# Import bindings for namespaces imported into schema
import pyxb.binding.datatypes

# NOTE: All namespace declarations are reserved within the binding
Namespace = pyxb.namespace.NamespaceForURI(u'http://schemas.xmlsoap.org/soap/encoding/', create_if_missing=True)
Namespace.configureCategories(['typeBinding', 'elementBinding'])

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


# Atomic simple type: [anonymous]
class STD_ANON (pyxb.binding.datatypes.boolean):

    """An atomic simple type."""

    _ExpandedName = None
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 46, 3)
    _Documentation = None
STD_ANON._CF_pattern = pyxb.binding.facets.CF_pattern()
STD_ANON._CF_pattern.addPattern(pattern=u'0|1')
STD_ANON._InitializeFacetMap(STD_ANON._CF_pattern)

# Atomic simple type: {http://schemas.xmlsoap.org/soap/encoding/}arrayCoordinate
class arrayCoordinate (pyxb.binding.datatypes.string):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'arrayCoordinate')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 71, 2)
    _Documentation = None
arrayCoordinate._InitializeFacetMap()
Namespace.addCategoryObject('typeBinding', u'arrayCoordinate', arrayCoordinate)

# Atomic simple type: {http://schemas.xmlsoap.org/soap/encoding/}base64
class base64 (pyxb.binding.datatypes.base64Binary):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'base64')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 127, 2)
    _Documentation = None
base64._InitializeFacetMap()
Namespace.addCategoryObject('typeBinding', u'base64', base64)

# Complex type {http://schemas.xmlsoap.org/soap/encoding/}Struct with content type ELEMENT_ONLY
class Struct_ (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {http://schemas.xmlsoap.org/soap/encoding/}Struct with content type ELEMENT_ONLY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'Struct')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 119, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.anyType
    
    # Attribute id uses Python identifier id
    __id = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'id'), 'id', '__httpschemas_xmlsoap_orgsoapencoding_Struct__id', pyxb.binding.datatypes.ID)
    __id._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    __id._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    
    id = property(__id.value, __id.set, None, None)

    
    # Attribute href uses Python identifier href
    __href = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'href'), 'href', '__httpschemas_xmlsoap_orgsoapencoding_Struct__href', pyxb.binding.datatypes.anyURI)
    __href._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    __href._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    
    href = property(__href.value, __href.set, None, None)

    _AttributeWildcard = pyxb.binding.content.Wildcard(process_contents=pyxb.binding.content.Wildcard.PC_lax, namespace_constraint=(pyxb.binding.content.Wildcard.NC_not, u'http://schemas.xmlsoap.org/soap/encoding/'))
    _HasWildcardElement = True
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __id.name() : __id,
        __href.name() : __href
    })
Namespace.addCategoryObject('typeBinding', u'Struct', Struct_)


# Complex type {http://schemas.xmlsoap.org/soap/encoding/}duration with content type SIMPLE
class duration_ (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {http://schemas.xmlsoap.org/soap/encoding/}duration with content type SIMPLE"""
    _TypeDefinition = pyxb.binding.datatypes.duration
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'duration')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 135, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.duration
    
    # Attribute id uses Python identifier id
    __id = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'id'), 'id', '__httpschemas_xmlsoap_orgsoapencoding_duration__id', pyxb.binding.datatypes.ID)
    __id._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    __id._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    
    id = property(__id.value, __id.set, None, None)

    
    # Attribute href uses Python identifier href
    __href = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'href'), 'href', '__httpschemas_xmlsoap_orgsoapencoding_duration__href', pyxb.binding.datatypes.anyURI)
    __href._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    __href._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    
    href = property(__href.value, __href.set, None, None)

    _AttributeWildcard = pyxb.binding.content.Wildcard(process_contents=pyxb.binding.content.Wildcard.PC_lax, namespace_constraint=(pyxb.binding.content.Wildcard.NC_not, u'http://schemas.xmlsoap.org/soap/encoding/'))
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __id.name() : __id,
        __href.name() : __href
    })
Namespace.addCategoryObject('typeBinding', u'duration', duration_)


# Complex type {http://schemas.xmlsoap.org/soap/encoding/}dateTime with content type SIMPLE
class dateTime_ (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {http://schemas.xmlsoap.org/soap/encoding/}dateTime with content type SIMPLE"""
    _TypeDefinition = pyxb.binding.datatypes.dateTime
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'dateTime')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 144, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.dateTime
    
    # Attribute id uses Python identifier id
    __id = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'id'), 'id', '__httpschemas_xmlsoap_orgsoapencoding_dateTime__id', pyxb.binding.datatypes.ID)
    __id._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    __id._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    
    id = property(__id.value, __id.set, None, None)

    
    # Attribute href uses Python identifier href
    __href = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'href'), 'href', '__httpschemas_xmlsoap_orgsoapencoding_dateTime__href', pyxb.binding.datatypes.anyURI)
    __href._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    __href._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    
    href = property(__href.value, __href.set, None, None)

    _AttributeWildcard = pyxb.binding.content.Wildcard(process_contents=pyxb.binding.content.Wildcard.PC_lax, namespace_constraint=(pyxb.binding.content.Wildcard.NC_not, u'http://schemas.xmlsoap.org/soap/encoding/'))
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __id.name() : __id,
        __href.name() : __href
    })
Namespace.addCategoryObject('typeBinding', u'dateTime', dateTime_)


# Complex type {http://schemas.xmlsoap.org/soap/encoding/}NOTATION with content type SIMPLE
class NOTATION_ (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {http://schemas.xmlsoap.org/soap/encoding/}NOTATION with content type SIMPLE"""
    _TypeDefinition = pyxb.binding.datatypes.QName
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'NOTATION')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 155, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.QName
    
    # Attribute id uses Python identifier id
    __id = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'id'), 'id', '__httpschemas_xmlsoap_orgsoapencoding_NOTATION__id', pyxb.binding.datatypes.ID)
    __id._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    __id._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    
    id = property(__id.value, __id.set, None, None)

    
    # Attribute href uses Python identifier href
    __href = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'href'), 'href', '__httpschemas_xmlsoap_orgsoapencoding_NOTATION__href', pyxb.binding.datatypes.anyURI)
    __href._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    __href._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    
    href = property(__href.value, __href.set, None, None)

    _AttributeWildcard = pyxb.binding.content.Wildcard(process_contents=pyxb.binding.content.Wildcard.PC_lax, namespace_constraint=(pyxb.binding.content.Wildcard.NC_not, u'http://schemas.xmlsoap.org/soap/encoding/'))
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __id.name() : __id,
        __href.name() : __href
    })
Namespace.addCategoryObject('typeBinding', u'NOTATION', NOTATION_)


# Complex type {http://schemas.xmlsoap.org/soap/encoding/}time with content type SIMPLE
class time_ (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {http://schemas.xmlsoap.org/soap/encoding/}time with content type SIMPLE"""
    _TypeDefinition = pyxb.binding.datatypes.time
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'time')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 165, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.time
    
    # Attribute id uses Python identifier id
    __id = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'id'), 'id', '__httpschemas_xmlsoap_orgsoapencoding_time__id', pyxb.binding.datatypes.ID)
    __id._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    __id._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    
    id = property(__id.value, __id.set, None, None)

    
    # Attribute href uses Python identifier href
    __href = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'href'), 'href', '__httpschemas_xmlsoap_orgsoapencoding_time__href', pyxb.binding.datatypes.anyURI)
    __href._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    __href._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    
    href = property(__href.value, __href.set, None, None)

    _AttributeWildcard = pyxb.binding.content.Wildcard(process_contents=pyxb.binding.content.Wildcard.PC_lax, namespace_constraint=(pyxb.binding.content.Wildcard.NC_not, u'http://schemas.xmlsoap.org/soap/encoding/'))
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __id.name() : __id,
        __href.name() : __href
    })
Namespace.addCategoryObject('typeBinding', u'time', time_)


# Complex type {http://schemas.xmlsoap.org/soap/encoding/}date with content type SIMPLE
class date_ (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {http://schemas.xmlsoap.org/soap/encoding/}date with content type SIMPLE"""
    _TypeDefinition = pyxb.binding.datatypes.date
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'date')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 174, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.date
    
    # Attribute id uses Python identifier id
    __id = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'id'), 'id', '__httpschemas_xmlsoap_orgsoapencoding_date__id', pyxb.binding.datatypes.ID)
    __id._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    __id._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    
    id = property(__id.value, __id.set, None, None)

    
    # Attribute href uses Python identifier href
    __href = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'href'), 'href', '__httpschemas_xmlsoap_orgsoapencoding_date__href', pyxb.binding.datatypes.anyURI)
    __href._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    __href._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    
    href = property(__href.value, __href.set, None, None)

    _AttributeWildcard = pyxb.binding.content.Wildcard(process_contents=pyxb.binding.content.Wildcard.PC_lax, namespace_constraint=(pyxb.binding.content.Wildcard.NC_not, u'http://schemas.xmlsoap.org/soap/encoding/'))
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __id.name() : __id,
        __href.name() : __href
    })
Namespace.addCategoryObject('typeBinding', u'date', date_)


# Complex type {http://schemas.xmlsoap.org/soap/encoding/}gYearMonth with content type SIMPLE
class gYearMonth_ (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {http://schemas.xmlsoap.org/soap/encoding/}gYearMonth with content type SIMPLE"""
    _TypeDefinition = pyxb.binding.datatypes.gYearMonth
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'gYearMonth')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 183, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.gYearMonth
    
    # Attribute id uses Python identifier id
    __id = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'id'), 'id', '__httpschemas_xmlsoap_orgsoapencoding_gYearMonth__id', pyxb.binding.datatypes.ID)
    __id._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    __id._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    
    id = property(__id.value, __id.set, None, None)

    
    # Attribute href uses Python identifier href
    __href = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'href'), 'href', '__httpschemas_xmlsoap_orgsoapencoding_gYearMonth__href', pyxb.binding.datatypes.anyURI)
    __href._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    __href._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    
    href = property(__href.value, __href.set, None, None)

    _AttributeWildcard = pyxb.binding.content.Wildcard(process_contents=pyxb.binding.content.Wildcard.PC_lax, namespace_constraint=(pyxb.binding.content.Wildcard.NC_not, u'http://schemas.xmlsoap.org/soap/encoding/'))
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __id.name() : __id,
        __href.name() : __href
    })
Namespace.addCategoryObject('typeBinding', u'gYearMonth', gYearMonth_)


# Complex type {http://schemas.xmlsoap.org/soap/encoding/}gYear with content type SIMPLE
class gYear_ (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {http://schemas.xmlsoap.org/soap/encoding/}gYear with content type SIMPLE"""
    _TypeDefinition = pyxb.binding.datatypes.gYear
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'gYear')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 192, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.gYear
    
    # Attribute id uses Python identifier id
    __id = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'id'), 'id', '__httpschemas_xmlsoap_orgsoapencoding_gYear__id', pyxb.binding.datatypes.ID)
    __id._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    __id._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    
    id = property(__id.value, __id.set, None, None)

    
    # Attribute href uses Python identifier href
    __href = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'href'), 'href', '__httpschemas_xmlsoap_orgsoapencoding_gYear__href', pyxb.binding.datatypes.anyURI)
    __href._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    __href._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    
    href = property(__href.value, __href.set, None, None)

    _AttributeWildcard = pyxb.binding.content.Wildcard(process_contents=pyxb.binding.content.Wildcard.PC_lax, namespace_constraint=(pyxb.binding.content.Wildcard.NC_not, u'http://schemas.xmlsoap.org/soap/encoding/'))
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __id.name() : __id,
        __href.name() : __href
    })
Namespace.addCategoryObject('typeBinding', u'gYear', gYear_)


# Complex type {http://schemas.xmlsoap.org/soap/encoding/}gMonthDay with content type SIMPLE
class gMonthDay_ (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {http://schemas.xmlsoap.org/soap/encoding/}gMonthDay with content type SIMPLE"""
    _TypeDefinition = pyxb.binding.datatypes.gMonthDay
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'gMonthDay')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 201, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.gMonthDay
    
    # Attribute id uses Python identifier id
    __id = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'id'), 'id', '__httpschemas_xmlsoap_orgsoapencoding_gMonthDay__id', pyxb.binding.datatypes.ID)
    __id._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    __id._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    
    id = property(__id.value, __id.set, None, None)

    
    # Attribute href uses Python identifier href
    __href = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'href'), 'href', '__httpschemas_xmlsoap_orgsoapencoding_gMonthDay__href', pyxb.binding.datatypes.anyURI)
    __href._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    __href._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    
    href = property(__href.value, __href.set, None, None)

    _AttributeWildcard = pyxb.binding.content.Wildcard(process_contents=pyxb.binding.content.Wildcard.PC_lax, namespace_constraint=(pyxb.binding.content.Wildcard.NC_not, u'http://schemas.xmlsoap.org/soap/encoding/'))
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __id.name() : __id,
        __href.name() : __href
    })
Namespace.addCategoryObject('typeBinding', u'gMonthDay', gMonthDay_)


# Complex type {http://schemas.xmlsoap.org/soap/encoding/}gDay with content type SIMPLE
class gDay_ (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {http://schemas.xmlsoap.org/soap/encoding/}gDay with content type SIMPLE"""
    _TypeDefinition = pyxb.binding.datatypes.gDay
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'gDay')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 210, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.gDay
    
    # Attribute id uses Python identifier id
    __id = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'id'), 'id', '__httpschemas_xmlsoap_orgsoapencoding_gDay__id', pyxb.binding.datatypes.ID)
    __id._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    __id._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    
    id = property(__id.value, __id.set, None, None)

    
    # Attribute href uses Python identifier href
    __href = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'href'), 'href', '__httpschemas_xmlsoap_orgsoapencoding_gDay__href', pyxb.binding.datatypes.anyURI)
    __href._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    __href._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    
    href = property(__href.value, __href.set, None, None)

    _AttributeWildcard = pyxb.binding.content.Wildcard(process_contents=pyxb.binding.content.Wildcard.PC_lax, namespace_constraint=(pyxb.binding.content.Wildcard.NC_not, u'http://schemas.xmlsoap.org/soap/encoding/'))
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __id.name() : __id,
        __href.name() : __href
    })
Namespace.addCategoryObject('typeBinding', u'gDay', gDay_)


# Complex type {http://schemas.xmlsoap.org/soap/encoding/}gMonth with content type SIMPLE
class gMonth_ (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {http://schemas.xmlsoap.org/soap/encoding/}gMonth with content type SIMPLE"""
    _TypeDefinition = pyxb.binding.datatypes.gMonth
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'gMonth')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 219, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.gMonth
    
    # Attribute id uses Python identifier id
    __id = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'id'), 'id', '__httpschemas_xmlsoap_orgsoapencoding_gMonth__id', pyxb.binding.datatypes.ID)
    __id._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    __id._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    
    id = property(__id.value, __id.set, None, None)

    
    # Attribute href uses Python identifier href
    __href = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'href'), 'href', '__httpschemas_xmlsoap_orgsoapencoding_gMonth__href', pyxb.binding.datatypes.anyURI)
    __href._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    __href._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    
    href = property(__href.value, __href.set, None, None)

    _AttributeWildcard = pyxb.binding.content.Wildcard(process_contents=pyxb.binding.content.Wildcard.PC_lax, namespace_constraint=(pyxb.binding.content.Wildcard.NC_not, u'http://schemas.xmlsoap.org/soap/encoding/'))
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __id.name() : __id,
        __href.name() : __href
    })
Namespace.addCategoryObject('typeBinding', u'gMonth', gMonth_)


# Complex type {http://schemas.xmlsoap.org/soap/encoding/}boolean with content type SIMPLE
class boolean_ (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {http://schemas.xmlsoap.org/soap/encoding/}boolean with content type SIMPLE"""
    _TypeDefinition = pyxb.binding.datatypes.boolean
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'boolean')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 228, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.boolean
    
    # Attribute id uses Python identifier id
    __id = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'id'), 'id', '__httpschemas_xmlsoap_orgsoapencoding_boolean__id', pyxb.binding.datatypes.ID)
    __id._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    __id._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    
    id = property(__id.value, __id.set, None, None)

    
    # Attribute href uses Python identifier href
    __href = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'href'), 'href', '__httpschemas_xmlsoap_orgsoapencoding_boolean__href', pyxb.binding.datatypes.anyURI)
    __href._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    __href._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    
    href = property(__href.value, __href.set, None, None)

    _AttributeWildcard = pyxb.binding.content.Wildcard(process_contents=pyxb.binding.content.Wildcard.PC_lax, namespace_constraint=(pyxb.binding.content.Wildcard.NC_not, u'http://schemas.xmlsoap.org/soap/encoding/'))
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __id.name() : __id,
        __href.name() : __href
    })
Namespace.addCategoryObject('typeBinding', u'boolean', boolean_)


# Complex type {http://schemas.xmlsoap.org/soap/encoding/}base64Binary with content type SIMPLE
class base64Binary_ (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {http://schemas.xmlsoap.org/soap/encoding/}base64Binary with content type SIMPLE"""
    _TypeDefinition = pyxb.binding.datatypes.base64Binary
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'base64Binary')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 237, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.base64Binary
    
    # Attribute id uses Python identifier id
    __id = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'id'), 'id', '__httpschemas_xmlsoap_orgsoapencoding_base64Binary__id', pyxb.binding.datatypes.ID)
    __id._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    __id._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    
    id = property(__id.value, __id.set, None, None)

    
    # Attribute href uses Python identifier href
    __href = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'href'), 'href', '__httpschemas_xmlsoap_orgsoapencoding_base64Binary__href', pyxb.binding.datatypes.anyURI)
    __href._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    __href._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    
    href = property(__href.value, __href.set, None, None)

    _AttributeWildcard = pyxb.binding.content.Wildcard(process_contents=pyxb.binding.content.Wildcard.PC_lax, namespace_constraint=(pyxb.binding.content.Wildcard.NC_not, u'http://schemas.xmlsoap.org/soap/encoding/'))
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __id.name() : __id,
        __href.name() : __href
    })
Namespace.addCategoryObject('typeBinding', u'base64Binary', base64Binary_)


# Complex type {http://schemas.xmlsoap.org/soap/encoding/}hexBinary with content type SIMPLE
class hexBinary_ (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {http://schemas.xmlsoap.org/soap/encoding/}hexBinary with content type SIMPLE"""
    _TypeDefinition = pyxb.binding.datatypes.hexBinary
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'hexBinary')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 246, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.hexBinary
    
    # Attribute id uses Python identifier id
    __id = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'id'), 'id', '__httpschemas_xmlsoap_orgsoapencoding_hexBinary__id', pyxb.binding.datatypes.ID)
    __id._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    __id._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    
    id = property(__id.value, __id.set, None, None)

    
    # Attribute href uses Python identifier href
    __href = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'href'), 'href', '__httpschemas_xmlsoap_orgsoapencoding_hexBinary__href', pyxb.binding.datatypes.anyURI)
    __href._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    __href._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    
    href = property(__href.value, __href.set, None, None)

    _AttributeWildcard = pyxb.binding.content.Wildcard(process_contents=pyxb.binding.content.Wildcard.PC_lax, namespace_constraint=(pyxb.binding.content.Wildcard.NC_not, u'http://schemas.xmlsoap.org/soap/encoding/'))
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __id.name() : __id,
        __href.name() : __href
    })
Namespace.addCategoryObject('typeBinding', u'hexBinary', hexBinary_)


# Complex type {http://schemas.xmlsoap.org/soap/encoding/}float with content type SIMPLE
class float_ (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {http://schemas.xmlsoap.org/soap/encoding/}float with content type SIMPLE"""
    _TypeDefinition = pyxb.binding.datatypes.float
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'float')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 255, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.float
    
    # Attribute id uses Python identifier id
    __id = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'id'), 'id', '__httpschemas_xmlsoap_orgsoapencoding_float__id', pyxb.binding.datatypes.ID)
    __id._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    __id._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    
    id = property(__id.value, __id.set, None, None)

    
    # Attribute href uses Python identifier href
    __href = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'href'), 'href', '__httpschemas_xmlsoap_orgsoapencoding_float__href', pyxb.binding.datatypes.anyURI)
    __href._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    __href._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    
    href = property(__href.value, __href.set, None, None)

    _AttributeWildcard = pyxb.binding.content.Wildcard(process_contents=pyxb.binding.content.Wildcard.PC_lax, namespace_constraint=(pyxb.binding.content.Wildcard.NC_not, u'http://schemas.xmlsoap.org/soap/encoding/'))
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __id.name() : __id,
        __href.name() : __href
    })
Namespace.addCategoryObject('typeBinding', u'float', float_)


# Complex type {http://schemas.xmlsoap.org/soap/encoding/}double with content type SIMPLE
class double_ (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {http://schemas.xmlsoap.org/soap/encoding/}double with content type SIMPLE"""
    _TypeDefinition = pyxb.binding.datatypes.double
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'double')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 264, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.double
    
    # Attribute id uses Python identifier id
    __id = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'id'), 'id', '__httpschemas_xmlsoap_orgsoapencoding_double__id', pyxb.binding.datatypes.ID)
    __id._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    __id._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    
    id = property(__id.value, __id.set, None, None)

    
    # Attribute href uses Python identifier href
    __href = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'href'), 'href', '__httpschemas_xmlsoap_orgsoapencoding_double__href', pyxb.binding.datatypes.anyURI)
    __href._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    __href._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    
    href = property(__href.value, __href.set, None, None)

    _AttributeWildcard = pyxb.binding.content.Wildcard(process_contents=pyxb.binding.content.Wildcard.PC_lax, namespace_constraint=(pyxb.binding.content.Wildcard.NC_not, u'http://schemas.xmlsoap.org/soap/encoding/'))
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __id.name() : __id,
        __href.name() : __href
    })
Namespace.addCategoryObject('typeBinding', u'double', double_)


# Complex type {http://schemas.xmlsoap.org/soap/encoding/}anyURI with content type SIMPLE
class anyURI_ (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {http://schemas.xmlsoap.org/soap/encoding/}anyURI with content type SIMPLE"""
    _TypeDefinition = pyxb.binding.datatypes.anyURI
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'anyURI')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 273, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.anyURI
    
    # Attribute id uses Python identifier id
    __id = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'id'), 'id', '__httpschemas_xmlsoap_orgsoapencoding_anyURI__id', pyxb.binding.datatypes.ID)
    __id._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    __id._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    
    id = property(__id.value, __id.set, None, None)

    
    # Attribute href uses Python identifier href
    __href = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'href'), 'href', '__httpschemas_xmlsoap_orgsoapencoding_anyURI__href', pyxb.binding.datatypes.anyURI)
    __href._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    __href._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    
    href = property(__href.value, __href.set, None, None)

    _AttributeWildcard = pyxb.binding.content.Wildcard(process_contents=pyxb.binding.content.Wildcard.PC_lax, namespace_constraint=(pyxb.binding.content.Wildcard.NC_not, u'http://schemas.xmlsoap.org/soap/encoding/'))
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __id.name() : __id,
        __href.name() : __href
    })
Namespace.addCategoryObject('typeBinding', u'anyURI', anyURI_)


# Complex type {http://schemas.xmlsoap.org/soap/encoding/}QName with content type SIMPLE
class QName_ (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {http://schemas.xmlsoap.org/soap/encoding/}QName with content type SIMPLE"""
    _TypeDefinition = pyxb.binding.datatypes.QName
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'QName')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 282, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.QName
    
    # Attribute id uses Python identifier id
    __id = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'id'), 'id', '__httpschemas_xmlsoap_orgsoapencoding_QName__id', pyxb.binding.datatypes.ID)
    __id._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    __id._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    
    id = property(__id.value, __id.set, None, None)

    
    # Attribute href uses Python identifier href
    __href = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'href'), 'href', '__httpschemas_xmlsoap_orgsoapencoding_QName__href', pyxb.binding.datatypes.anyURI)
    __href._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    __href._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    
    href = property(__href.value, __href.set, None, None)

    _AttributeWildcard = pyxb.binding.content.Wildcard(process_contents=pyxb.binding.content.Wildcard.PC_lax, namespace_constraint=(pyxb.binding.content.Wildcard.NC_not, u'http://schemas.xmlsoap.org/soap/encoding/'))
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __id.name() : __id,
        __href.name() : __href
    })
Namespace.addCategoryObject('typeBinding', u'QName', QName_)


# Complex type {http://schemas.xmlsoap.org/soap/encoding/}string with content type SIMPLE
class string_ (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {http://schemas.xmlsoap.org/soap/encoding/}string with content type SIMPLE"""
    _TypeDefinition = pyxb.binding.datatypes.string
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'string')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 292, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.string
    
    # Attribute id uses Python identifier id
    __id = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'id'), 'id', '__httpschemas_xmlsoap_orgsoapencoding_string__id', pyxb.binding.datatypes.ID)
    __id._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    __id._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    
    id = property(__id.value, __id.set, None, None)

    
    # Attribute href uses Python identifier href
    __href = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'href'), 'href', '__httpschemas_xmlsoap_orgsoapencoding_string__href', pyxb.binding.datatypes.anyURI)
    __href._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    __href._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    
    href = property(__href.value, __href.set, None, None)

    _AttributeWildcard = pyxb.binding.content.Wildcard(process_contents=pyxb.binding.content.Wildcard.PC_lax, namespace_constraint=(pyxb.binding.content.Wildcard.NC_not, u'http://schemas.xmlsoap.org/soap/encoding/'))
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __id.name() : __id,
        __href.name() : __href
    })
Namespace.addCategoryObject('typeBinding', u'string', string_)


# Complex type {http://schemas.xmlsoap.org/soap/encoding/}normalizedString with content type SIMPLE
class normalizedString_ (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {http://schemas.xmlsoap.org/soap/encoding/}normalizedString with content type SIMPLE"""
    _TypeDefinition = pyxb.binding.datatypes.normalizedString
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'normalizedString')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 301, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.normalizedString
    
    # Attribute id uses Python identifier id
    __id = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'id'), 'id', '__httpschemas_xmlsoap_orgsoapencoding_normalizedString__id', pyxb.binding.datatypes.ID)
    __id._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    __id._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    
    id = property(__id.value, __id.set, None, None)

    
    # Attribute href uses Python identifier href
    __href = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'href'), 'href', '__httpschemas_xmlsoap_orgsoapencoding_normalizedString__href', pyxb.binding.datatypes.anyURI)
    __href._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    __href._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    
    href = property(__href.value, __href.set, None, None)

    _AttributeWildcard = pyxb.binding.content.Wildcard(process_contents=pyxb.binding.content.Wildcard.PC_lax, namespace_constraint=(pyxb.binding.content.Wildcard.NC_not, u'http://schemas.xmlsoap.org/soap/encoding/'))
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __id.name() : __id,
        __href.name() : __href
    })
Namespace.addCategoryObject('typeBinding', u'normalizedString', normalizedString_)


# Complex type {http://schemas.xmlsoap.org/soap/encoding/}token with content type SIMPLE
class token_ (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {http://schemas.xmlsoap.org/soap/encoding/}token with content type SIMPLE"""
    _TypeDefinition = pyxb.binding.datatypes.token
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'token')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 310, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.token
    
    # Attribute id uses Python identifier id
    __id = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'id'), 'id', '__httpschemas_xmlsoap_orgsoapencoding_token__id', pyxb.binding.datatypes.ID)
    __id._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    __id._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    
    id = property(__id.value, __id.set, None, None)

    
    # Attribute href uses Python identifier href
    __href = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'href'), 'href', '__httpschemas_xmlsoap_orgsoapencoding_token__href', pyxb.binding.datatypes.anyURI)
    __href._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    __href._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    
    href = property(__href.value, __href.set, None, None)

    _AttributeWildcard = pyxb.binding.content.Wildcard(process_contents=pyxb.binding.content.Wildcard.PC_lax, namespace_constraint=(pyxb.binding.content.Wildcard.NC_not, u'http://schemas.xmlsoap.org/soap/encoding/'))
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __id.name() : __id,
        __href.name() : __href
    })
Namespace.addCategoryObject('typeBinding', u'token', token_)


# Complex type {http://schemas.xmlsoap.org/soap/encoding/}language with content type SIMPLE
class language_ (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {http://schemas.xmlsoap.org/soap/encoding/}language with content type SIMPLE"""
    _TypeDefinition = pyxb.binding.datatypes.language
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'language')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 319, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.language
    
    # Attribute id uses Python identifier id
    __id = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'id'), 'id', '__httpschemas_xmlsoap_orgsoapencoding_language__id', pyxb.binding.datatypes.ID)
    __id._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    __id._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    
    id = property(__id.value, __id.set, None, None)

    
    # Attribute href uses Python identifier href
    __href = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'href'), 'href', '__httpschemas_xmlsoap_orgsoapencoding_language__href', pyxb.binding.datatypes.anyURI)
    __href._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    __href._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    
    href = property(__href.value, __href.set, None, None)

    _AttributeWildcard = pyxb.binding.content.Wildcard(process_contents=pyxb.binding.content.Wildcard.PC_lax, namespace_constraint=(pyxb.binding.content.Wildcard.NC_not, u'http://schemas.xmlsoap.org/soap/encoding/'))
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __id.name() : __id,
        __href.name() : __href
    })
Namespace.addCategoryObject('typeBinding', u'language', language_)


# Complex type {http://schemas.xmlsoap.org/soap/encoding/}Name with content type SIMPLE
class Name_ (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {http://schemas.xmlsoap.org/soap/encoding/}Name with content type SIMPLE"""
    _TypeDefinition = pyxb.binding.datatypes.Name
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'Name')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 328, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.Name
    
    # Attribute id uses Python identifier id
    __id = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'id'), 'id', '__httpschemas_xmlsoap_orgsoapencoding_Name__id', pyxb.binding.datatypes.ID)
    __id._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    __id._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    
    id = property(__id.value, __id.set, None, None)

    
    # Attribute href uses Python identifier href
    __href = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'href'), 'href', '__httpschemas_xmlsoap_orgsoapencoding_Name__href', pyxb.binding.datatypes.anyURI)
    __href._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    __href._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    
    href = property(__href.value, __href.set, None, None)

    _AttributeWildcard = pyxb.binding.content.Wildcard(process_contents=pyxb.binding.content.Wildcard.PC_lax, namespace_constraint=(pyxb.binding.content.Wildcard.NC_not, u'http://schemas.xmlsoap.org/soap/encoding/'))
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __id.name() : __id,
        __href.name() : __href
    })
Namespace.addCategoryObject('typeBinding', u'Name', Name_)


# Complex type {http://schemas.xmlsoap.org/soap/encoding/}NMTOKEN with content type SIMPLE
class NMTOKEN_ (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {http://schemas.xmlsoap.org/soap/encoding/}NMTOKEN with content type SIMPLE"""
    _TypeDefinition = pyxb.binding.datatypes.NMTOKEN
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'NMTOKEN')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 337, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.NMTOKEN
    
    # Attribute id uses Python identifier id
    __id = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'id'), 'id', '__httpschemas_xmlsoap_orgsoapencoding_NMTOKEN__id', pyxb.binding.datatypes.ID)
    __id._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    __id._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    
    id = property(__id.value, __id.set, None, None)

    
    # Attribute href uses Python identifier href
    __href = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'href'), 'href', '__httpschemas_xmlsoap_orgsoapencoding_NMTOKEN__href', pyxb.binding.datatypes.anyURI)
    __href._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    __href._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    
    href = property(__href.value, __href.set, None, None)

    _AttributeWildcard = pyxb.binding.content.Wildcard(process_contents=pyxb.binding.content.Wildcard.PC_lax, namespace_constraint=(pyxb.binding.content.Wildcard.NC_not, u'http://schemas.xmlsoap.org/soap/encoding/'))
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __id.name() : __id,
        __href.name() : __href
    })
Namespace.addCategoryObject('typeBinding', u'NMTOKEN', NMTOKEN_)


# Complex type {http://schemas.xmlsoap.org/soap/encoding/}NCName with content type SIMPLE
class NCName_ (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {http://schemas.xmlsoap.org/soap/encoding/}NCName with content type SIMPLE"""
    _TypeDefinition = pyxb.binding.datatypes.NCName
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'NCName')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 346, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.NCName
    
    # Attribute id uses Python identifier id
    __id = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'id'), 'id', '__httpschemas_xmlsoap_orgsoapencoding_NCName__id', pyxb.binding.datatypes.ID)
    __id._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    __id._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    
    id = property(__id.value, __id.set, None, None)

    
    # Attribute href uses Python identifier href
    __href = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'href'), 'href', '__httpschemas_xmlsoap_orgsoapencoding_NCName__href', pyxb.binding.datatypes.anyURI)
    __href._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    __href._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    
    href = property(__href.value, __href.set, None, None)

    _AttributeWildcard = pyxb.binding.content.Wildcard(process_contents=pyxb.binding.content.Wildcard.PC_lax, namespace_constraint=(pyxb.binding.content.Wildcard.NC_not, u'http://schemas.xmlsoap.org/soap/encoding/'))
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __id.name() : __id,
        __href.name() : __href
    })
Namespace.addCategoryObject('typeBinding', u'NCName', NCName_)


# Complex type {http://schemas.xmlsoap.org/soap/encoding/}NMTOKENS with content type SIMPLE
class NMTOKENS_ (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {http://schemas.xmlsoap.org/soap/encoding/}NMTOKENS with content type SIMPLE"""
    _TypeDefinition = pyxb.binding.datatypes.NMTOKENS
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'NMTOKENS')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 355, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.NMTOKENS
    
    # Attribute id uses Python identifier id
    __id = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'id'), 'id', '__httpschemas_xmlsoap_orgsoapencoding_NMTOKENS__id', pyxb.binding.datatypes.ID)
    __id._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    __id._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    
    id = property(__id.value, __id.set, None, None)

    
    # Attribute href uses Python identifier href
    __href = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'href'), 'href', '__httpschemas_xmlsoap_orgsoapencoding_NMTOKENS__href', pyxb.binding.datatypes.anyURI)
    __href._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    __href._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    
    href = property(__href.value, __href.set, None, None)

    _AttributeWildcard = pyxb.binding.content.Wildcard(process_contents=pyxb.binding.content.Wildcard.PC_lax, namespace_constraint=(pyxb.binding.content.Wildcard.NC_not, u'http://schemas.xmlsoap.org/soap/encoding/'))
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __id.name() : __id,
        __href.name() : __href
    })
Namespace.addCategoryObject('typeBinding', u'NMTOKENS', NMTOKENS_)


# Complex type {http://schemas.xmlsoap.org/soap/encoding/}ID with content type SIMPLE
class ID_ (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {http://schemas.xmlsoap.org/soap/encoding/}ID with content type SIMPLE"""
    _TypeDefinition = pyxb.binding.datatypes.ID
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'ID')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 364, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.ID
    
    # Attribute id uses Python identifier id
    __id = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'id'), 'id', '__httpschemas_xmlsoap_orgsoapencoding_ID__id', pyxb.binding.datatypes.ID)
    __id._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    __id._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    
    id = property(__id.value, __id.set, None, None)

    
    # Attribute href uses Python identifier href
    __href = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'href'), 'href', '__httpschemas_xmlsoap_orgsoapencoding_ID__href', pyxb.binding.datatypes.anyURI)
    __href._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    __href._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    
    href = property(__href.value, __href.set, None, None)

    _AttributeWildcard = pyxb.binding.content.Wildcard(process_contents=pyxb.binding.content.Wildcard.PC_lax, namespace_constraint=(pyxb.binding.content.Wildcard.NC_not, u'http://schemas.xmlsoap.org/soap/encoding/'))
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __id.name() : __id,
        __href.name() : __href
    })
Namespace.addCategoryObject('typeBinding', u'ID', ID_)


# Complex type {http://schemas.xmlsoap.org/soap/encoding/}IDREF with content type SIMPLE
class IDREF_ (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {http://schemas.xmlsoap.org/soap/encoding/}IDREF with content type SIMPLE"""
    _TypeDefinition = pyxb.binding.datatypes.IDREF
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'IDREF')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 373, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.IDREF
    
    # Attribute id uses Python identifier id
    __id = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'id'), 'id', '__httpschemas_xmlsoap_orgsoapencoding_IDREF__id', pyxb.binding.datatypes.ID)
    __id._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    __id._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    
    id = property(__id.value, __id.set, None, None)

    
    # Attribute href uses Python identifier href
    __href = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'href'), 'href', '__httpschemas_xmlsoap_orgsoapencoding_IDREF__href', pyxb.binding.datatypes.anyURI)
    __href._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    __href._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    
    href = property(__href.value, __href.set, None, None)

    _AttributeWildcard = pyxb.binding.content.Wildcard(process_contents=pyxb.binding.content.Wildcard.PC_lax, namespace_constraint=(pyxb.binding.content.Wildcard.NC_not, u'http://schemas.xmlsoap.org/soap/encoding/'))
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __id.name() : __id,
        __href.name() : __href
    })
Namespace.addCategoryObject('typeBinding', u'IDREF', IDREF_)


# Complex type {http://schemas.xmlsoap.org/soap/encoding/}ENTITY with content type SIMPLE
class ENTITY_ (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {http://schemas.xmlsoap.org/soap/encoding/}ENTITY with content type SIMPLE"""
    _TypeDefinition = pyxb.binding.datatypes.ENTITY
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'ENTITY')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 382, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.ENTITY
    
    # Attribute id uses Python identifier id
    __id = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'id'), 'id', '__httpschemas_xmlsoap_orgsoapencoding_ENTITY__id', pyxb.binding.datatypes.ID)
    __id._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    __id._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    
    id = property(__id.value, __id.set, None, None)

    
    # Attribute href uses Python identifier href
    __href = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'href'), 'href', '__httpschemas_xmlsoap_orgsoapencoding_ENTITY__href', pyxb.binding.datatypes.anyURI)
    __href._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    __href._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    
    href = property(__href.value, __href.set, None, None)

    _AttributeWildcard = pyxb.binding.content.Wildcard(process_contents=pyxb.binding.content.Wildcard.PC_lax, namespace_constraint=(pyxb.binding.content.Wildcard.NC_not, u'http://schemas.xmlsoap.org/soap/encoding/'))
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __id.name() : __id,
        __href.name() : __href
    })
Namespace.addCategoryObject('typeBinding', u'ENTITY', ENTITY_)


# Complex type {http://schemas.xmlsoap.org/soap/encoding/}IDREFS with content type SIMPLE
class IDREFS_ (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {http://schemas.xmlsoap.org/soap/encoding/}IDREFS with content type SIMPLE"""
    _TypeDefinition = pyxb.binding.datatypes.IDREFS
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'IDREFS')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 391, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.IDREFS
    
    # Attribute id uses Python identifier id
    __id = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'id'), 'id', '__httpschemas_xmlsoap_orgsoapencoding_IDREFS__id', pyxb.binding.datatypes.ID)
    __id._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    __id._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    
    id = property(__id.value, __id.set, None, None)

    
    # Attribute href uses Python identifier href
    __href = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'href'), 'href', '__httpschemas_xmlsoap_orgsoapencoding_IDREFS__href', pyxb.binding.datatypes.anyURI)
    __href._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    __href._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    
    href = property(__href.value, __href.set, None, None)

    _AttributeWildcard = pyxb.binding.content.Wildcard(process_contents=pyxb.binding.content.Wildcard.PC_lax, namespace_constraint=(pyxb.binding.content.Wildcard.NC_not, u'http://schemas.xmlsoap.org/soap/encoding/'))
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __id.name() : __id,
        __href.name() : __href
    })
Namespace.addCategoryObject('typeBinding', u'IDREFS', IDREFS_)


# Complex type {http://schemas.xmlsoap.org/soap/encoding/}ENTITIES with content type SIMPLE
class ENTITIES_ (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {http://schemas.xmlsoap.org/soap/encoding/}ENTITIES with content type SIMPLE"""
    _TypeDefinition = pyxb.binding.datatypes.ENTITIES
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'ENTITIES')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 400, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.ENTITIES
    
    # Attribute id uses Python identifier id
    __id = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'id'), 'id', '__httpschemas_xmlsoap_orgsoapencoding_ENTITIES__id', pyxb.binding.datatypes.ID)
    __id._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    __id._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    
    id = property(__id.value, __id.set, None, None)

    
    # Attribute href uses Python identifier href
    __href = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'href'), 'href', '__httpschemas_xmlsoap_orgsoapencoding_ENTITIES__href', pyxb.binding.datatypes.anyURI)
    __href._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    __href._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    
    href = property(__href.value, __href.set, None, None)

    _AttributeWildcard = pyxb.binding.content.Wildcard(process_contents=pyxb.binding.content.Wildcard.PC_lax, namespace_constraint=(pyxb.binding.content.Wildcard.NC_not, u'http://schemas.xmlsoap.org/soap/encoding/'))
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __id.name() : __id,
        __href.name() : __href
    })
Namespace.addCategoryObject('typeBinding', u'ENTITIES', ENTITIES_)


# Complex type {http://schemas.xmlsoap.org/soap/encoding/}decimal with content type SIMPLE
class decimal_ (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {http://schemas.xmlsoap.org/soap/encoding/}decimal with content type SIMPLE"""
    _TypeDefinition = pyxb.binding.datatypes.decimal
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'decimal')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 409, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.decimal
    
    # Attribute id uses Python identifier id
    __id = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'id'), 'id', '__httpschemas_xmlsoap_orgsoapencoding_decimal__id', pyxb.binding.datatypes.ID)
    __id._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    __id._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    
    id = property(__id.value, __id.set, None, None)

    
    # Attribute href uses Python identifier href
    __href = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'href'), 'href', '__httpschemas_xmlsoap_orgsoapencoding_decimal__href', pyxb.binding.datatypes.anyURI)
    __href._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    __href._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    
    href = property(__href.value, __href.set, None, None)

    _AttributeWildcard = pyxb.binding.content.Wildcard(process_contents=pyxb.binding.content.Wildcard.PC_lax, namespace_constraint=(pyxb.binding.content.Wildcard.NC_not, u'http://schemas.xmlsoap.org/soap/encoding/'))
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __id.name() : __id,
        __href.name() : __href
    })
Namespace.addCategoryObject('typeBinding', u'decimal', decimal_)


# Complex type {http://schemas.xmlsoap.org/soap/encoding/}integer with content type SIMPLE
class integer_ (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {http://schemas.xmlsoap.org/soap/encoding/}integer with content type SIMPLE"""
    _TypeDefinition = pyxb.binding.datatypes.integer
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'integer')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 418, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.integer
    
    # Attribute id uses Python identifier id
    __id = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'id'), 'id', '__httpschemas_xmlsoap_orgsoapencoding_integer__id', pyxb.binding.datatypes.ID)
    __id._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    __id._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    
    id = property(__id.value, __id.set, None, None)

    
    # Attribute href uses Python identifier href
    __href = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'href'), 'href', '__httpschemas_xmlsoap_orgsoapencoding_integer__href', pyxb.binding.datatypes.anyURI)
    __href._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    __href._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    
    href = property(__href.value, __href.set, None, None)

    _AttributeWildcard = pyxb.binding.content.Wildcard(process_contents=pyxb.binding.content.Wildcard.PC_lax, namespace_constraint=(pyxb.binding.content.Wildcard.NC_not, u'http://schemas.xmlsoap.org/soap/encoding/'))
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __id.name() : __id,
        __href.name() : __href
    })
Namespace.addCategoryObject('typeBinding', u'integer', integer_)


# Complex type {http://schemas.xmlsoap.org/soap/encoding/}nonPositiveInteger with content type SIMPLE
class nonPositiveInteger_ (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {http://schemas.xmlsoap.org/soap/encoding/}nonPositiveInteger with content type SIMPLE"""
    _TypeDefinition = pyxb.binding.datatypes.nonPositiveInteger
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'nonPositiveInteger')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 427, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.nonPositiveInteger
    
    # Attribute id uses Python identifier id
    __id = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'id'), 'id', '__httpschemas_xmlsoap_orgsoapencoding_nonPositiveInteger__id', pyxb.binding.datatypes.ID)
    __id._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    __id._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    
    id = property(__id.value, __id.set, None, None)

    
    # Attribute href uses Python identifier href
    __href = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'href'), 'href', '__httpschemas_xmlsoap_orgsoapencoding_nonPositiveInteger__href', pyxb.binding.datatypes.anyURI)
    __href._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    __href._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    
    href = property(__href.value, __href.set, None, None)

    _AttributeWildcard = pyxb.binding.content.Wildcard(process_contents=pyxb.binding.content.Wildcard.PC_lax, namespace_constraint=(pyxb.binding.content.Wildcard.NC_not, u'http://schemas.xmlsoap.org/soap/encoding/'))
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __id.name() : __id,
        __href.name() : __href
    })
Namespace.addCategoryObject('typeBinding', u'nonPositiveInteger', nonPositiveInteger_)


# Complex type {http://schemas.xmlsoap.org/soap/encoding/}negativeInteger with content type SIMPLE
class negativeInteger_ (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {http://schemas.xmlsoap.org/soap/encoding/}negativeInteger with content type SIMPLE"""
    _TypeDefinition = pyxb.binding.datatypes.negativeInteger
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'negativeInteger')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 436, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.negativeInteger
    
    # Attribute id uses Python identifier id
    __id = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'id'), 'id', '__httpschemas_xmlsoap_orgsoapencoding_negativeInteger__id', pyxb.binding.datatypes.ID)
    __id._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    __id._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    
    id = property(__id.value, __id.set, None, None)

    
    # Attribute href uses Python identifier href
    __href = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'href'), 'href', '__httpschemas_xmlsoap_orgsoapencoding_negativeInteger__href', pyxb.binding.datatypes.anyURI)
    __href._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    __href._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    
    href = property(__href.value, __href.set, None, None)

    _AttributeWildcard = pyxb.binding.content.Wildcard(process_contents=pyxb.binding.content.Wildcard.PC_lax, namespace_constraint=(pyxb.binding.content.Wildcard.NC_not, u'http://schemas.xmlsoap.org/soap/encoding/'))
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __id.name() : __id,
        __href.name() : __href
    })
Namespace.addCategoryObject('typeBinding', u'negativeInteger', negativeInteger_)


# Complex type {http://schemas.xmlsoap.org/soap/encoding/}long with content type SIMPLE
class long_ (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {http://schemas.xmlsoap.org/soap/encoding/}long with content type SIMPLE"""
    _TypeDefinition = pyxb.binding.datatypes.long
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'long')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 445, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.long
    
    # Attribute id uses Python identifier id
    __id = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'id'), 'id', '__httpschemas_xmlsoap_orgsoapencoding_long__id', pyxb.binding.datatypes.ID)
    __id._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    __id._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    
    id = property(__id.value, __id.set, None, None)

    
    # Attribute href uses Python identifier href
    __href = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'href'), 'href', '__httpschemas_xmlsoap_orgsoapencoding_long__href', pyxb.binding.datatypes.anyURI)
    __href._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    __href._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    
    href = property(__href.value, __href.set, None, None)

    _AttributeWildcard = pyxb.binding.content.Wildcard(process_contents=pyxb.binding.content.Wildcard.PC_lax, namespace_constraint=(pyxb.binding.content.Wildcard.NC_not, u'http://schemas.xmlsoap.org/soap/encoding/'))
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __id.name() : __id,
        __href.name() : __href
    })
Namespace.addCategoryObject('typeBinding', u'long', long_)


# Complex type {http://schemas.xmlsoap.org/soap/encoding/}int with content type SIMPLE
class int_ (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {http://schemas.xmlsoap.org/soap/encoding/}int with content type SIMPLE"""
    _TypeDefinition = pyxb.binding.datatypes.int
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'int')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 454, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.int
    
    # Attribute id uses Python identifier id
    __id = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'id'), 'id', '__httpschemas_xmlsoap_orgsoapencoding_int__id', pyxb.binding.datatypes.ID)
    __id._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    __id._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    
    id = property(__id.value, __id.set, None, None)

    
    # Attribute href uses Python identifier href
    __href = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'href'), 'href', '__httpschemas_xmlsoap_orgsoapencoding_int__href', pyxb.binding.datatypes.anyURI)
    __href._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    __href._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    
    href = property(__href.value, __href.set, None, None)

    _AttributeWildcard = pyxb.binding.content.Wildcard(process_contents=pyxb.binding.content.Wildcard.PC_lax, namespace_constraint=(pyxb.binding.content.Wildcard.NC_not, u'http://schemas.xmlsoap.org/soap/encoding/'))
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __id.name() : __id,
        __href.name() : __href
    })
Namespace.addCategoryObject('typeBinding', u'int', int_)


# Complex type {http://schemas.xmlsoap.org/soap/encoding/}short with content type SIMPLE
class short_ (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {http://schemas.xmlsoap.org/soap/encoding/}short with content type SIMPLE"""
    _TypeDefinition = pyxb.binding.datatypes.short
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'short')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 463, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.short
    
    # Attribute id uses Python identifier id
    __id = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'id'), 'id', '__httpschemas_xmlsoap_orgsoapencoding_short__id', pyxb.binding.datatypes.ID)
    __id._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    __id._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    
    id = property(__id.value, __id.set, None, None)

    
    # Attribute href uses Python identifier href
    __href = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'href'), 'href', '__httpschemas_xmlsoap_orgsoapencoding_short__href', pyxb.binding.datatypes.anyURI)
    __href._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    __href._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    
    href = property(__href.value, __href.set, None, None)

    _AttributeWildcard = pyxb.binding.content.Wildcard(process_contents=pyxb.binding.content.Wildcard.PC_lax, namespace_constraint=(pyxb.binding.content.Wildcard.NC_not, u'http://schemas.xmlsoap.org/soap/encoding/'))
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __id.name() : __id,
        __href.name() : __href
    })
Namespace.addCategoryObject('typeBinding', u'short', short_)


# Complex type {http://schemas.xmlsoap.org/soap/encoding/}byte with content type SIMPLE
class byte_ (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {http://schemas.xmlsoap.org/soap/encoding/}byte with content type SIMPLE"""
    _TypeDefinition = pyxb.binding.datatypes.byte
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'byte')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 472, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.byte
    
    # Attribute id uses Python identifier id
    __id = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'id'), 'id', '__httpschemas_xmlsoap_orgsoapencoding_byte__id', pyxb.binding.datatypes.ID)
    __id._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    __id._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    
    id = property(__id.value, __id.set, None, None)

    
    # Attribute href uses Python identifier href
    __href = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'href'), 'href', '__httpschemas_xmlsoap_orgsoapencoding_byte__href', pyxb.binding.datatypes.anyURI)
    __href._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    __href._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    
    href = property(__href.value, __href.set, None, None)

    _AttributeWildcard = pyxb.binding.content.Wildcard(process_contents=pyxb.binding.content.Wildcard.PC_lax, namespace_constraint=(pyxb.binding.content.Wildcard.NC_not, u'http://schemas.xmlsoap.org/soap/encoding/'))
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __id.name() : __id,
        __href.name() : __href
    })
Namespace.addCategoryObject('typeBinding', u'byte', byte_)


# Complex type {http://schemas.xmlsoap.org/soap/encoding/}nonNegativeInteger with content type SIMPLE
class nonNegativeInteger_ (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {http://schemas.xmlsoap.org/soap/encoding/}nonNegativeInteger with content type SIMPLE"""
    _TypeDefinition = pyxb.binding.datatypes.nonNegativeInteger
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'nonNegativeInteger')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 481, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.nonNegativeInteger
    
    # Attribute id uses Python identifier id
    __id = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'id'), 'id', '__httpschemas_xmlsoap_orgsoapencoding_nonNegativeInteger__id', pyxb.binding.datatypes.ID)
    __id._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    __id._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    
    id = property(__id.value, __id.set, None, None)

    
    # Attribute href uses Python identifier href
    __href = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'href'), 'href', '__httpschemas_xmlsoap_orgsoapencoding_nonNegativeInteger__href', pyxb.binding.datatypes.anyURI)
    __href._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    __href._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    
    href = property(__href.value, __href.set, None, None)

    _AttributeWildcard = pyxb.binding.content.Wildcard(process_contents=pyxb.binding.content.Wildcard.PC_lax, namespace_constraint=(pyxb.binding.content.Wildcard.NC_not, u'http://schemas.xmlsoap.org/soap/encoding/'))
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __id.name() : __id,
        __href.name() : __href
    })
Namespace.addCategoryObject('typeBinding', u'nonNegativeInteger', nonNegativeInteger_)


# Complex type {http://schemas.xmlsoap.org/soap/encoding/}unsignedLong with content type SIMPLE
class unsignedLong_ (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {http://schemas.xmlsoap.org/soap/encoding/}unsignedLong with content type SIMPLE"""
    _TypeDefinition = pyxb.binding.datatypes.unsignedLong
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'unsignedLong')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 490, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.unsignedLong
    
    # Attribute id uses Python identifier id
    __id = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'id'), 'id', '__httpschemas_xmlsoap_orgsoapencoding_unsignedLong__id', pyxb.binding.datatypes.ID)
    __id._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    __id._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    
    id = property(__id.value, __id.set, None, None)

    
    # Attribute href uses Python identifier href
    __href = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'href'), 'href', '__httpschemas_xmlsoap_orgsoapencoding_unsignedLong__href', pyxb.binding.datatypes.anyURI)
    __href._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    __href._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    
    href = property(__href.value, __href.set, None, None)

    _AttributeWildcard = pyxb.binding.content.Wildcard(process_contents=pyxb.binding.content.Wildcard.PC_lax, namespace_constraint=(pyxb.binding.content.Wildcard.NC_not, u'http://schemas.xmlsoap.org/soap/encoding/'))
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __id.name() : __id,
        __href.name() : __href
    })
Namespace.addCategoryObject('typeBinding', u'unsignedLong', unsignedLong_)


# Complex type {http://schemas.xmlsoap.org/soap/encoding/}unsignedInt with content type SIMPLE
class unsignedInt_ (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {http://schemas.xmlsoap.org/soap/encoding/}unsignedInt with content type SIMPLE"""
    _TypeDefinition = pyxb.binding.datatypes.unsignedInt
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'unsignedInt')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 499, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.unsignedInt
    
    # Attribute id uses Python identifier id
    __id = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'id'), 'id', '__httpschemas_xmlsoap_orgsoapencoding_unsignedInt__id', pyxb.binding.datatypes.ID)
    __id._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    __id._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    
    id = property(__id.value, __id.set, None, None)

    
    # Attribute href uses Python identifier href
    __href = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'href'), 'href', '__httpschemas_xmlsoap_orgsoapencoding_unsignedInt__href', pyxb.binding.datatypes.anyURI)
    __href._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    __href._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    
    href = property(__href.value, __href.set, None, None)

    _AttributeWildcard = pyxb.binding.content.Wildcard(process_contents=pyxb.binding.content.Wildcard.PC_lax, namespace_constraint=(pyxb.binding.content.Wildcard.NC_not, u'http://schemas.xmlsoap.org/soap/encoding/'))
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __id.name() : __id,
        __href.name() : __href
    })
Namespace.addCategoryObject('typeBinding', u'unsignedInt', unsignedInt_)


# Complex type {http://schemas.xmlsoap.org/soap/encoding/}unsignedShort with content type SIMPLE
class unsignedShort_ (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {http://schemas.xmlsoap.org/soap/encoding/}unsignedShort with content type SIMPLE"""
    _TypeDefinition = pyxb.binding.datatypes.unsignedShort
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'unsignedShort')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 508, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.unsignedShort
    
    # Attribute id uses Python identifier id
    __id = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'id'), 'id', '__httpschemas_xmlsoap_orgsoapencoding_unsignedShort__id', pyxb.binding.datatypes.ID)
    __id._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    __id._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    
    id = property(__id.value, __id.set, None, None)

    
    # Attribute href uses Python identifier href
    __href = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'href'), 'href', '__httpschemas_xmlsoap_orgsoapencoding_unsignedShort__href', pyxb.binding.datatypes.anyURI)
    __href._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    __href._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    
    href = property(__href.value, __href.set, None, None)

    _AttributeWildcard = pyxb.binding.content.Wildcard(process_contents=pyxb.binding.content.Wildcard.PC_lax, namespace_constraint=(pyxb.binding.content.Wildcard.NC_not, u'http://schemas.xmlsoap.org/soap/encoding/'))
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __id.name() : __id,
        __href.name() : __href
    })
Namespace.addCategoryObject('typeBinding', u'unsignedShort', unsignedShort_)


# Complex type {http://schemas.xmlsoap.org/soap/encoding/}unsignedByte with content type SIMPLE
class unsignedByte_ (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {http://schemas.xmlsoap.org/soap/encoding/}unsignedByte with content type SIMPLE"""
    _TypeDefinition = pyxb.binding.datatypes.unsignedByte
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'unsignedByte')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 517, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.unsignedByte
    
    # Attribute id uses Python identifier id
    __id = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'id'), 'id', '__httpschemas_xmlsoap_orgsoapencoding_unsignedByte__id', pyxb.binding.datatypes.ID)
    __id._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    __id._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    
    id = property(__id.value, __id.set, None, None)

    
    # Attribute href uses Python identifier href
    __href = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'href'), 'href', '__httpschemas_xmlsoap_orgsoapencoding_unsignedByte__href', pyxb.binding.datatypes.anyURI)
    __href._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    __href._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    
    href = property(__href.value, __href.set, None, None)

    _AttributeWildcard = pyxb.binding.content.Wildcard(process_contents=pyxb.binding.content.Wildcard.PC_lax, namespace_constraint=(pyxb.binding.content.Wildcard.NC_not, u'http://schemas.xmlsoap.org/soap/encoding/'))
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __id.name() : __id,
        __href.name() : __href
    })
Namespace.addCategoryObject('typeBinding', u'unsignedByte', unsignedByte_)


# Complex type {http://schemas.xmlsoap.org/soap/encoding/}positiveInteger with content type SIMPLE
class positiveInteger_ (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {http://schemas.xmlsoap.org/soap/encoding/}positiveInteger with content type SIMPLE"""
    _TypeDefinition = pyxb.binding.datatypes.positiveInteger
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'positiveInteger')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 526, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.positiveInteger
    
    # Attribute id uses Python identifier id
    __id = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'id'), 'id', '__httpschemas_xmlsoap_orgsoapencoding_positiveInteger__id', pyxb.binding.datatypes.ID)
    __id._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    __id._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    
    id = property(__id.value, __id.set, None, None)

    
    # Attribute href uses Python identifier href
    __href = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'href'), 'href', '__httpschemas_xmlsoap_orgsoapencoding_positiveInteger__href', pyxb.binding.datatypes.anyURI)
    __href._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    __href._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    
    href = property(__href.value, __href.set, None, None)

    _AttributeWildcard = pyxb.binding.content.Wildcard(process_contents=pyxb.binding.content.Wildcard.PC_lax, namespace_constraint=(pyxb.binding.content.Wildcard.NC_not, u'http://schemas.xmlsoap.org/soap/encoding/'))
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __id.name() : __id,
        __href.name() : __href
    })
Namespace.addCategoryObject('typeBinding', u'positiveInteger', positiveInteger_)


# Complex type {http://schemas.xmlsoap.org/soap/encoding/}Array with content type ELEMENT_ONLY
class Array_ (pyxb.binding.basis.complexTypeDefinition):
    """
	   'Array' is a complex type for accessors identified by position 
	  """
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'Array')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 96, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.anyType
    
    # Attribute id uses Python identifier id
    __id = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'id'), 'id', '__httpschemas_xmlsoap_orgsoapencoding_Array__id', pyxb.binding.datatypes.ID)
    __id._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    __id._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 62, 4)
    
    id = property(__id.value, __id.set, None, None)

    
    # Attribute href uses Python identifier href
    __href = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'href'), 'href', '__httpschemas_xmlsoap_orgsoapencoding_Array__href', pyxb.binding.datatypes.anyURI)
    __href._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    __href._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 63, 4)
    
    href = property(__href.value, __href.set, None, None)

    
    # Attribute {http://schemas.xmlsoap.org/soap/encoding/}arrayType uses Python identifier arrayType
    __arrayType = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(Namespace, u'arrayType'), 'arrayType', '__httpschemas_xmlsoap_orgsoapencoding_Array__httpschemas_xmlsoap_orgsoapencodingarrayType', pyxb.binding.datatypes.string)
    __arrayType._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 75, 2)
    __arrayType._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 79, 4)
    
    arrayType = property(__arrayType.value, __arrayType.set, None, None)

    
    # Attribute {http://schemas.xmlsoap.org/soap/encoding/}offset uses Python identifier offset
    __offset = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(Namespace, u'offset'), 'offset', '__httpschemas_xmlsoap_orgsoapencoding_Array__httpschemas_xmlsoap_orgsoapencodingoffset', arrayCoordinate)
    __offset._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 76, 2)
    __offset._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 80, 4)
    
    offset = property(__offset.value, __offset.set, None, None)

    _AttributeWildcard = pyxb.binding.content.Wildcard(process_contents=pyxb.binding.content.Wildcard.PC_lax, namespace_constraint=(pyxb.binding.content.Wildcard.NC_not, u'http://schemas.xmlsoap.org/soap/encoding/'))
    _HasWildcardElement = True
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __id.name() : __id,
        __href.name() : __href,
        __arrayType.name() : __arrayType,
        __offset.name() : __offset
    })
Namespace.addCategoryObject('typeBinding', u'Array', Array_)


anyType = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'anyType'), pyxb.binding.datatypes.anyType, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 534, 2))
Namespace.addCategoryObject('elementBinding', anyType.name().localName(), anyType)

Struct = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'Struct'), Struct_, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 111, 2))
Namespace.addCategoryObject('elementBinding', Struct.name().localName(), Struct)

duration = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'duration'), duration_, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 134, 2))
Namespace.addCategoryObject('elementBinding', duration.name().localName(), duration)

dateTime = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'dateTime'), dateTime_, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 143, 2))
Namespace.addCategoryObject('elementBinding', dateTime.name().localName(), dateTime)

NOTATION = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'NOTATION'), NOTATION_, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 154, 2))
Namespace.addCategoryObject('elementBinding', NOTATION.name().localName(), NOTATION)

time = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'time'), time_, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 164, 2))
Namespace.addCategoryObject('elementBinding', time.name().localName(), time)

date = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'date'), date_, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 173, 2))
Namespace.addCategoryObject('elementBinding', date.name().localName(), date)

gYearMonth = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'gYearMonth'), gYearMonth_, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 182, 2))
Namespace.addCategoryObject('elementBinding', gYearMonth.name().localName(), gYearMonth)

gYear = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'gYear'), gYear_, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 191, 2))
Namespace.addCategoryObject('elementBinding', gYear.name().localName(), gYear)

gMonthDay = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'gMonthDay'), gMonthDay_, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 200, 2))
Namespace.addCategoryObject('elementBinding', gMonthDay.name().localName(), gMonthDay)

gDay = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'gDay'), gDay_, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 209, 2))
Namespace.addCategoryObject('elementBinding', gDay.name().localName(), gDay)

gMonth = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'gMonth'), gMonth_, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 218, 2))
Namespace.addCategoryObject('elementBinding', gMonth.name().localName(), gMonth)

boolean = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'boolean'), boolean_, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 227, 2))
Namespace.addCategoryObject('elementBinding', boolean.name().localName(), boolean)

base64Binary = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'base64Binary'), base64Binary_, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 236, 2))
Namespace.addCategoryObject('elementBinding', base64Binary.name().localName(), base64Binary)

hexBinary = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'hexBinary'), hexBinary_, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 245, 2))
Namespace.addCategoryObject('elementBinding', hexBinary.name().localName(), hexBinary)

float = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'float'), float_, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 254, 2))
Namespace.addCategoryObject('elementBinding', float.name().localName(), float)

double = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'double'), double_, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 263, 2))
Namespace.addCategoryObject('elementBinding', double.name().localName(), double)

anyURI = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'anyURI'), anyURI_, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 272, 2))
Namespace.addCategoryObject('elementBinding', anyURI.name().localName(), anyURI)

QName = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'QName'), QName_, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 281, 2))
Namespace.addCategoryObject('elementBinding', QName.name().localName(), QName)

string = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'string'), string_, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 291, 2))
Namespace.addCategoryObject('elementBinding', string.name().localName(), string)

normalizedString = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'normalizedString'), normalizedString_, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 300, 2))
Namespace.addCategoryObject('elementBinding', normalizedString.name().localName(), normalizedString)

token = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'token'), token_, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 309, 2))
Namespace.addCategoryObject('elementBinding', token.name().localName(), token)

language = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'language'), language_, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 318, 2))
Namespace.addCategoryObject('elementBinding', language.name().localName(), language)

Name = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'Name'), Name_, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 327, 2))
Namespace.addCategoryObject('elementBinding', Name.name().localName(), Name)

NMTOKEN = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'NMTOKEN'), NMTOKEN_, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 336, 2))
Namespace.addCategoryObject('elementBinding', NMTOKEN.name().localName(), NMTOKEN)

NCName = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'NCName'), NCName_, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 345, 2))
Namespace.addCategoryObject('elementBinding', NCName.name().localName(), NCName)

NMTOKENS = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'NMTOKENS'), NMTOKENS_, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 354, 2))
Namespace.addCategoryObject('elementBinding', NMTOKENS.name().localName(), NMTOKENS)

ID = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'ID'), ID_, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 363, 2))
Namespace.addCategoryObject('elementBinding', ID.name().localName(), ID)

IDREF = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'IDREF'), IDREF_, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 372, 2))
Namespace.addCategoryObject('elementBinding', IDREF.name().localName(), IDREF)

ENTITY = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'ENTITY'), ENTITY_, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 381, 2))
Namespace.addCategoryObject('elementBinding', ENTITY.name().localName(), ENTITY)

IDREFS = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'IDREFS'), IDREFS_, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 390, 2))
Namespace.addCategoryObject('elementBinding', IDREFS.name().localName(), IDREFS)

ENTITIES = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'ENTITIES'), ENTITIES_, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 399, 2))
Namespace.addCategoryObject('elementBinding', ENTITIES.name().localName(), ENTITIES)

decimal = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'decimal'), decimal_, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 408, 2))
Namespace.addCategoryObject('elementBinding', decimal.name().localName(), decimal)

integer = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'integer'), integer_, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 417, 2))
Namespace.addCategoryObject('elementBinding', integer.name().localName(), integer)

nonPositiveInteger = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'nonPositiveInteger'), nonPositiveInteger_, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 426, 2))
Namespace.addCategoryObject('elementBinding', nonPositiveInteger.name().localName(), nonPositiveInteger)

negativeInteger = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'negativeInteger'), negativeInteger_, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 435, 2))
Namespace.addCategoryObject('elementBinding', negativeInteger.name().localName(), negativeInteger)

long = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'long'), long_, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 444, 2))
Namespace.addCategoryObject('elementBinding', long.name().localName(), long)

int = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'int'), int_, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 453, 2))
Namespace.addCategoryObject('elementBinding', int.name().localName(), int)

short = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'short'), short_, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 462, 2))
Namespace.addCategoryObject('elementBinding', short.name().localName(), short)

byte = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'byte'), byte_, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 471, 2))
Namespace.addCategoryObject('elementBinding', byte.name().localName(), byte)

nonNegativeInteger = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'nonNegativeInteger'), nonNegativeInteger_, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 480, 2))
Namespace.addCategoryObject('elementBinding', nonNegativeInteger.name().localName(), nonNegativeInteger)

unsignedLong = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'unsignedLong'), unsignedLong_, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 489, 2))
Namespace.addCategoryObject('elementBinding', unsignedLong.name().localName(), unsignedLong)

unsignedInt = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'unsignedInt'), unsignedInt_, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 498, 2))
Namespace.addCategoryObject('elementBinding', unsignedInt.name().localName(), unsignedInt)

unsignedShort = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'unsignedShort'), unsignedShort_, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 507, 2))
Namespace.addCategoryObject('elementBinding', unsignedShort.name().localName(), unsignedShort)

unsignedByte = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'unsignedByte'), unsignedByte_, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 516, 2))
Namespace.addCategoryObject('elementBinding', unsignedByte.name().localName(), unsignedByte)

positiveInteger = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'positiveInteger'), positiveInteger_, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 525, 2))
Namespace.addCategoryObject('elementBinding', positiveInteger.name().localName(), positiveInteger)

Array = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'Array'), Array_, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 95, 2))
Namespace.addCategoryObject('elementBinding', Array.name().localName(), Array)



def _BuildAutomaton ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton
    del _BuildAutomaton
    import pyxb.utils.fac as fac

    counters = set()
    cc_0 = fac.CounterCondition(min=0L, max=1, metadata=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 120, 4))
    counters.add(cc_0)
    cc_1 = fac.CounterCondition(min=0L, max=None, metadata=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 115, 6))
    counters.add(cc_1)
    states = []
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_0, False))
    final_update.add(fac.UpdateInstruction(cc_1, False))
    symbol = pyxb.binding.content.WildcardUse(pyxb.binding.content.Wildcard(process_contents=pyxb.binding.content.Wildcard.PC_lax, namespace_constraint=pyxb.binding.content.Wildcard.NC_any), pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 115, 6))
    st_0 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_0)
    transitions = []
    transitions.append(fac.Transition(st_0, [
        fac.UpdateInstruction(cc_0, True),
        fac.UpdateInstruction(cc_1, False) ]))
    transitions.append(fac.Transition(st_0, [
        fac.UpdateInstruction(cc_1, True) ]))
    st_0._set_transitionSet(transitions)
    return fac.Automaton(states, counters, True, containing_state=None)
Struct_._Automaton = _BuildAutomaton()




def _BuildAutomaton_ ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton_
    del _BuildAutomaton_
    import pyxb.utils.fac as fac

    counters = set()
    cc_0 = fac.CounterCondition(min=0L, max=1, metadata=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 102, 4))
    counters.add(cc_0)
    cc_1 = fac.CounterCondition(min=0L, max=None, metadata=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 91, 6))
    counters.add(cc_1)
    states = []
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_0, False))
    final_update.add(fac.UpdateInstruction(cc_1, False))
    symbol = pyxb.binding.content.WildcardUse(pyxb.binding.content.Wildcard(process_contents=pyxb.binding.content.Wildcard.PC_lax, namespace_constraint=pyxb.binding.content.Wildcard.NC_any), pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/soapenc.xsd', 91, 6))
    st_0 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_0)
    transitions = []
    transitions.append(fac.Transition(st_0, [
        fac.UpdateInstruction(cc_0, True),
        fac.UpdateInstruction(cc_1, False) ]))
    transitions.append(fac.Transition(st_0, [
        fac.UpdateInstruction(cc_1, True) ]))
    st_0._set_transitionSet(transitions)
    return fac.Automaton(states, counters, True, containing_state=None)
Array_._Automaton = _BuildAutomaton_()

