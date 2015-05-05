# ./pyxb/bundles/wssplat/raw/whttp.py
# -*- coding: utf-8 -*-
# PyXB bindings for NM:b6d8cfb74f55bcb68c30cab97a9555b1fa1c0cfa
# Generated 2013-09-18 10:35:51.765753 by PyXB version 1.2.3
# Namespace http://www.w3.org/ns/wsdl/http

import pyxb
import pyxb.binding
import pyxb.binding.saxer
import io
import pyxb.utils.utility
import pyxb.utils.domutils
import sys

# Unique identifier for bindings created at the same time
_GenerationUID = pyxb.utils.utility.UniqueIdentifier('urn:uuid:ffdfabfc-2077-11e3-bf28-c8600024e903')

# Version of PyXB used to generate the bindings
_PyXBVersion = '1.2.3'
# Generated bindings are not compatible across PyXB versions
if pyxb.__version__ != _PyXBVersion:
    raise pyxb.PyXBVersionError(_PyXBVersion)

# Import bindings for namespaces imported into schema
import pyxb.bundles.wssplat.wsdl20
import pyxb.binding.datatypes

# NOTE: All namespace declarations are reserved within the binding
Namespace = pyxb.namespace.NamespaceForURI(u'http://www.w3.org/ns/wsdl/http', create_if_missing=True)
Namespace.configureCategories(['typeBinding', 'elementBinding'])
_Namespace_wsdl = pyxb.bundles.wssplat.wsdl20.Namespace
_Namespace_wsdl.configureCategories(['typeBinding', 'elementBinding'])

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
class STD_ANON (pyxb.binding.datatypes.token, pyxb.binding.basis.enumeration_mixin):

    """An atomic simple type."""

    _ExpandedName = None
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/whttp.xsd', 34, 1)
    _Documentation = None
STD_ANON._CF_enumeration = pyxb.binding.facets.CF_enumeration(value_datatype=STD_ANON, enum_prefix=None)
STD_ANON.any = STD_ANON._CF_enumeration.addEnumeration(unicode_value=u'#any', tag=u'any')
STD_ANON._InitializeFacetMap(STD_ANON._CF_enumeration)

# Atomic simple type: {http://www.w3.org/ns/wsdl/http}queryParameterType
class queryParameterType (pyxb.binding.datatypes.string):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'queryParameterType')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/whttp.xsd', 49, 2)
    _Documentation = None
queryParameterType._CF_pattern = pyxb.binding.facets.CF_pattern()
queryParameterType._CF_pattern.addPattern(pattern=u"[&;a-zA-Z0-9\\-\\._~!$'\\(\\):@/\\?\\*\\+,]{1,1}")
queryParameterType._CF_length = pyxb.binding.facets.CF_length(value=pyxb.binding.datatypes.nonNegativeInteger(1L))
queryParameterType._InitializeFacetMap(queryParameterType._CF_pattern,
   queryParameterType._CF_length)
Namespace.addCategoryObject('typeBinding', u'queryParameterType', queryParameterType)

# Atomic simple type: [anonymous]
class STD_ANON_ (pyxb.binding.datatypes.token, pyxb.binding.basis.enumeration_mixin):

    """An atomic simple type."""

    _ExpandedName = None
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/whttp.xsd', 65, 4)
    _Documentation = None
STD_ANON_._CF_enumeration = pyxb.binding.facets.CF_enumeration(value_datatype=STD_ANON_, enum_prefix=None)
STD_ANON_.basic = STD_ANON_._CF_enumeration.addEnumeration(unicode_value=u'basic', tag=u'basic')
STD_ANON_.digest = STD_ANON_._CF_enumeration.addEnumeration(unicode_value=u'digest', tag=u'digest')
STD_ANON_._InitializeFacetMap(STD_ANON_._CF_enumeration)

# Atomic simple type: {http://www.w3.org/ns/wsdl/http}versionType
class versionType (pyxb.binding.datatypes.string):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'versionType')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/whttp.xsd', 74, 2)
    _Documentation = None
versionType._CF_pattern = pyxb.binding.facets.CF_pattern()
versionType._CF_pattern.addPattern(pattern=u'[0-9]+\\.[0-9]+')
versionType._InitializeFacetMap(versionType._CF_pattern)
Namespace.addCategoryObject('typeBinding', u'versionType', versionType)

# Atomic simple type: {http://www.w3.org/ns/wsdl/http}httpTokenType
class httpTokenType (pyxb.binding.datatypes.string):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'httpTokenType')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/whttp.xsd', 80, 2)
    _Documentation = None
httpTokenType._CF_pattern = pyxb.binding.facets.CF_pattern()
httpTokenType._CF_pattern.addPattern(pattern=u"[!#-'*+\\-.0-9A-Z^-z|~]+")
httpTokenType._InitializeFacetMap(httpTokenType._CF_pattern)
Namespace.addCategoryObject('typeBinding', u'httpTokenType', httpTokenType)

# Union simple type: [anonymous]
# superclasses pyxb.binding.datatypes.anySimpleType
class STD_ANON_2 (pyxb.binding.basis.STD_union):

    """Simple type that is a union of pyxb.binding.datatypes.int, STD_ANON."""

    _ExpandedName = None
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/whttp.xsd', 32, 4)
    _Documentation = None

    _MemberTypes = ( pyxb.binding.datatypes.int, STD_ANON, )
STD_ANON_2._CF_enumeration = pyxb.binding.facets.CF_enumeration(value_datatype=STD_ANON_2)
STD_ANON_2._CF_pattern = pyxb.binding.facets.CF_pattern()
STD_ANON_2.any = u'#any'                          # originally STD_ANON.any
STD_ANON_2._InitializeFacetMap(STD_ANON_2._CF_enumeration,
   STD_ANON_2._CF_pattern)

# Complex type [anonymous] with content type ELEMENT_ONLY
class CTD_ANON (pyxb.bundles.wssplat.wsdl20.ExtensibleDocumentedType):
    """Complex type [anonymous] with content type ELEMENT_ONLY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = False
    _ExpandedName = None
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/whttp.xsd', 87, 4)
    _ElementMap = pyxb.bundles.wssplat.wsdl20.ExtensibleDocumentedType._ElementMap.copy()
    _AttributeMap = pyxb.bundles.wssplat.wsdl20.ExtensibleDocumentedType._AttributeMap.copy()
    # Base type is pyxb.bundles.wssplat.wsdl20.ExtensibleDocumentedType
    
    # Element documentation ({http://www.w3.org/ns/wsdl}documentation) inherited from {http://www.w3.org/ns/wsdl}DocumentedType
    
    # Attribute name uses Python identifier name
    __name = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'name'), 'name', '__httpwww_w3_orgnswsdlhttp_CTD_ANON_name', httpTokenType, required=True)
    __name._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/whttp.xsd', 90, 3)
    __name._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/whttp.xsd', 90, 3)
    
    name = property(__name.value, __name.set, None, None)

    
    # Attribute type uses Python identifier type
    __type = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'type'), 'type', '__httpwww_w3_orgnswsdlhttp_CTD_ANON_type', pyxb.binding.datatypes.QName, required=True)
    __type._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/whttp.xsd', 91, 3)
    __type._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/whttp.xsd', 91, 3)
    
    type = property(__type.value, __type.set, None, None)

    
    # Attribute required uses Python identifier required
    __required = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'required'), 'required', '__httpwww_w3_orgnswsdlhttp_CTD_ANON_required', pyxb.binding.datatypes.boolean)
    __required._DeclarationLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/whttp.xsd', 92, 3)
    __required._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/whttp.xsd', 92, 3)
    
    required = property(__required.value, __required.set, None, None)

    _AttributeWildcard = pyxb.binding.content.Wildcard(process_contents=pyxb.binding.content.Wildcard.PC_lax, namespace_constraint=(pyxb.binding.content.Wildcard.NC_not, u'http://www.w3.org/ns/wsdl'))
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __name.name() : __name,
        __type.name() : __type,
        __required.name() : __required
    })



header = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'header'), CTD_ANON, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/whttp.xsd', 86, 2))
Namespace.addCategoryObject('elementBinding', header.name().localName(), header)



def _BuildAutomaton ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton
    del _BuildAutomaton
    import pyxb.utils.fac as fac

    counters = set()
    cc_0 = fac.CounterCondition(min=0L, max=None, metadata=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/wsdl20.xsd', 37, 6))
    counters.add(cc_0)
    states = []
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_0, False))
    symbol = pyxb.binding.content.ElementUse(CTD_ANON._UseForTag(pyxb.namespace.ExpandedName(_Namespace_wsdl, u'documentation')), pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/wssplat/schemas/wsdl20.xsd', 37, 6))
    st_0 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_0)
    transitions = []
    transitions.append(fac.Transition(st_0, [
        fac.UpdateInstruction(cc_0, True) ]))
    st_0._set_transitionSet(transitions)
    return fac.Automaton(states, counters, True, containing_state=None)
CTD_ANON._Automaton = _BuildAutomaton()

