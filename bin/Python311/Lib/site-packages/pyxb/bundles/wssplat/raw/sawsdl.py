# ./pyxb/bundles/wssplat/raw/sawsdl.py
# -*- coding: utf-8 -*-
# PyXB bindings for NM:070b3b59f767ac4d0f19e851fc2c1f39e654364d
# Generated 2017-09-03 06:16:49.734741 by PyXB version 1.2.6 using Python 2.7.12.final.0
# Namespace http://www.w3.org/ns/sawsdl

from __future__ import unicode_literals
import pyxb
import pyxb.binding
import pyxb.binding.saxer
import io
import pyxb.utils.utility
import pyxb.utils.domutils
import sys
import pyxb.utils.six as _six
# Unique identifier for bindings created at the same time
_GenerationUID = pyxb.utils.utility.UniqueIdentifier('urn:uuid:616f95f4-9099-11e7-b77e-3497f68b2e96')

# Version of PyXB used to generate the bindings
_PyXBVersion = '1.2.6'
# Generated bindings are not compatible across PyXB versions
if pyxb.__version__ != _PyXBVersion:
    raise pyxb.PyXBVersionError(_PyXBVersion)

# A holder for module-level binding classes so we can access them from
# inside class definitions where property names may conflict.
_module_typeBindings = pyxb.utils.utility.Object()

# Import bindings for namespaces imported into schema
import pyxb.binding.datatypes

# NOTE: All namespace declarations are reserved within the binding
Namespace = pyxb.namespace.NamespaceForURI('http://www.w3.org/ns/sawsdl', create_if_missing=True)
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
        return CreateFromDOM(dom.documentElement, default_namespace=default_namespace)
    if default_namespace is None:
        default_namespace = Namespace.fallbackNamespace()
    saxer = pyxb.binding.saxer.make_parser(fallback_namespace=default_namespace, location_base=location_base)
    handler = saxer.getContentHandler()
    xmld = xml_text
    if isinstance(xmld, _six.text_type):
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


# List simple type: {http://www.w3.org/ns/sawsdl}listOfAnyURI
# superclasses pyxb.binding.datatypes.anySimpleType
class listOfAnyURI (pyxb.binding.basis.STD_list):

    """Simple type that is a list of pyxb.binding.datatypes.anyURI."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'listOfAnyURI')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.WUiBAra/PyXB-1.2.6/pyxb/bundles/wssplat/schemas/sawsdl.xsd', 8, 2)
    _Documentation = None

    _ItemType = pyxb.binding.datatypes.anyURI
listOfAnyURI._InitializeFacetMap()
Namespace.addCategoryObject('typeBinding', 'listOfAnyURI', listOfAnyURI)
_module_typeBindings.listOfAnyURI = listOfAnyURI

# Complex type [anonymous] with content type EMPTY
class CTD_ANON (pyxb.binding.basis.complexTypeDefinition):
    """This element is for use in WSDL 1.1 only. It does not apply to WSDL 2.0 documents.  Use in
        WSDL 2.0 documents is invalid."""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_EMPTY
    _Abstract = False
    _ExpandedName = None
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.WUiBAra/PyXB-1.2.6/pyxb/bundles/wssplat/schemas/sawsdl.xsd', 17, 4)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.anyType
    _AttributeWildcard = pyxb.binding.content.Wildcard(process_contents=pyxb.binding.content.Wildcard.PC_lax, namespace_constraint=pyxb.binding.content.Wildcard.NC_any)
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        
    })
_module_typeBindings.CTD_ANON = CTD_ANON


attrExtensions = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, 'attrExtensions'), CTD_ANON, location=pyxb.utils.utility.Location('/tmp/pyxbdist.WUiBAra/PyXB-1.2.6/pyxb/bundles/wssplat/schemas/sawsdl.xsd', 16, 2))
Namespace.addCategoryObject('elementBinding', attrExtensions.name().localName(), attrExtensions)
