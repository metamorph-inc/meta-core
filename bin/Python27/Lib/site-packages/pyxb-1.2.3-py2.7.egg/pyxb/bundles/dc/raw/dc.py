# ./pyxb/bundles/dc/raw/dc.py
# -*- coding: utf-8 -*-
# PyXB bindings for NM:fde543a00b9681daae05bbc5a17f3dce9cfacb0c
# Generated 2013-09-18 10:36:00.387899 by PyXB version 1.2.3
# Namespace http://purl.org/dc/elements/1.1/ [xmlns:dc]

import pyxb
import pyxb.binding
import pyxb.binding.saxer
import io
import pyxb.utils.utility
import pyxb.utils.domutils
import sys

# Unique identifier for bindings created at the same time
_GenerationUID = pyxb.utils.utility.UniqueIdentifier('urn:uuid:0500ac62-2078-11e3-a95e-c8600024e903')

# Version of PyXB used to generate the bindings
_PyXBVersion = '1.2.3'
# Generated bindings are not compatible across PyXB versions
if pyxb.__version__ != _PyXBVersion:
    raise pyxb.PyXBVersionError(_PyXBVersion)

# Import bindings for namespaces imported into schema
import pyxb.binding.xml_
import pyxb.binding.datatypes

# NOTE: All namespace declarations are reserved within the binding
Namespace = pyxb.namespace.NamespaceForURI(u'http://purl.org/dc/elements/1.1/', create_if_missing=True)
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


# Complex type {http://purl.org/dc/elements/1.1/}SimpleLiteral with content type MIXED
class SimpleLiteral (pyxb.binding.basis.complexTypeDefinition):
    """
            This is the default type for all of the DC elements.
            It permits text content only with optional
            xml:lang attribute.
            Text is allowed because mixed="true", but sub-elements
            are disallowed because minOccurs="0" and maxOccurs="0" 
            are on the xs:any tag.

    	    This complexType allows for restriction or extension permitting
            child elements.
    	"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_MIXED
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'SimpleLiteral')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/dc/schemas/dc.xsd', 45, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.anyType
    
    # Attribute {http://www.w3.org/XML/1998/namespace}lang uses Python identifier lang
    __lang = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(pyxb.namespace.XML, 'lang'), 'lang', '__httppurl_orgdcelements1_1_SimpleLiteral_httpwww_w3_orgXML1998namespacelang', pyxb.binding.xml_.STD_ANON_lang)
    __lang._DeclarationLocation = None
    __lang._UseLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/dc/schemas/dc.xsd', 65, 5)
    
    lang = property(__lang.value, __lang.set, None, None)

    _HasWildcardElement = True
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __lang.name() : __lang
    })
Namespace.addCategoryObject('typeBinding', u'SimpleLiteral', SimpleLiteral)


# Complex type {http://purl.org/dc/elements/1.1/}elementContainer with content type ELEMENT_ONLY
class elementContainer (pyxb.binding.basis.complexTypeDefinition):
    """
    		This complexType is included as a convenience for schema authors who need to define a root
    		or container element for all of the DC elements.
    	"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'elementContainer')
    _XSDLocation = pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/dc/schemas/dc.xsd', 104, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.anyType
    
    # Element {http://purl.org/dc/elements/1.1/}any uses Python identifier any
    __any = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(Namespace, u'any'), 'any', '__httppurl_orgdcelements1_1_elementContainer_httppurl_orgdcelements1_1any', True, pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/dc/schemas/dc.xsd', 70, 2), )

    
    any = property(__any.value, __any.set, None, None)

    _ElementMap.update({
        __any.name() : __any
    })
    _AttributeMap.update({
        
    })
Namespace.addCategoryObject('typeBinding', u'elementContainer', elementContainer)


title = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'title'), pyxb.binding.datatypes.anyType, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/dc/schemas/dc.xsd', 72, 2))
Namespace.addCategoryObject('elementBinding', title.name().localName(), title)

creator = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'creator'), pyxb.binding.datatypes.anyType, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/dc/schemas/dc.xsd', 73, 2))
Namespace.addCategoryObject('elementBinding', creator.name().localName(), creator)

subject = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'subject'), pyxb.binding.datatypes.anyType, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/dc/schemas/dc.xsd', 74, 2))
Namespace.addCategoryObject('elementBinding', subject.name().localName(), subject)

description = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'description'), pyxb.binding.datatypes.anyType, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/dc/schemas/dc.xsd', 75, 2))
Namespace.addCategoryObject('elementBinding', description.name().localName(), description)

publisher = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'publisher'), pyxb.binding.datatypes.anyType, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/dc/schemas/dc.xsd', 76, 2))
Namespace.addCategoryObject('elementBinding', publisher.name().localName(), publisher)

contributor = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'contributor'), pyxb.binding.datatypes.anyType, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/dc/schemas/dc.xsd', 77, 2))
Namespace.addCategoryObject('elementBinding', contributor.name().localName(), contributor)

date = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'date'), pyxb.binding.datatypes.anyType, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/dc/schemas/dc.xsd', 78, 2))
Namespace.addCategoryObject('elementBinding', date.name().localName(), date)

type = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'type'), pyxb.binding.datatypes.anyType, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/dc/schemas/dc.xsd', 79, 2))
Namespace.addCategoryObject('elementBinding', type.name().localName(), type)

format = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'format'), pyxb.binding.datatypes.anyType, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/dc/schemas/dc.xsd', 80, 2))
Namespace.addCategoryObject('elementBinding', format.name().localName(), format)

identifier = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'identifier'), pyxb.binding.datatypes.anyType, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/dc/schemas/dc.xsd', 81, 2))
Namespace.addCategoryObject('elementBinding', identifier.name().localName(), identifier)

source = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'source'), pyxb.binding.datatypes.anyType, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/dc/schemas/dc.xsd', 82, 2))
Namespace.addCategoryObject('elementBinding', source.name().localName(), source)

language = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'language'), pyxb.binding.datatypes.anyType, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/dc/schemas/dc.xsd', 83, 2))
Namespace.addCategoryObject('elementBinding', language.name().localName(), language)

relation = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'relation'), pyxb.binding.datatypes.anyType, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/dc/schemas/dc.xsd', 84, 2))
Namespace.addCategoryObject('elementBinding', relation.name().localName(), relation)

coverage = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'coverage'), pyxb.binding.datatypes.anyType, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/dc/schemas/dc.xsd', 85, 2))
Namespace.addCategoryObject('elementBinding', coverage.name().localName(), coverage)

rights = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'rights'), pyxb.binding.datatypes.anyType, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/dc/schemas/dc.xsd', 86, 2))
Namespace.addCategoryObject('elementBinding', rights.name().localName(), rights)

any = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'any'), SimpleLiteral, abstract=pyxb.binding.datatypes.boolean(1), location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/dc/schemas/dc.xsd', 70, 2))
Namespace.addCategoryObject('elementBinding', any.name().localName(), any)



def _BuildAutomaton ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton
    del _BuildAutomaton
    import pyxb.utils.fac as fac

    counters = set()
    cc_0 = fac.CounterCondition(min=0L, max=0L, metadata=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/dc/schemas/dc.xsd', 63, 6))
    counters.add(cc_0)
    states = []
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_0, False))
    symbol = pyxb.binding.content.WildcardUse(pyxb.binding.content.Wildcard(process_contents=pyxb.binding.content.Wildcard.PC_lax, namespace_constraint=pyxb.binding.content.Wildcard.NC_any), pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/dc/schemas/dc.xsd', 63, 6))
    st_0 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_0)
    transitions = []
    transitions.append(fac.Transition(st_0, [
        fac.UpdateInstruction(cc_0, True) ]))
    st_0._set_transitionSet(transitions)
    return fac.Automaton(states, counters, True, containing_state=None)
SimpleLiteral._Automaton = _BuildAutomaton()




elementContainer._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'any'), SimpleLiteral, abstract=pyxb.binding.datatypes.boolean(1), scope=elementContainer, location=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/dc/schemas/dc.xsd', 70, 2)))

def _BuildAutomaton_ ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton_
    del _BuildAutomaton_
    import pyxb.utils.fac as fac

    counters = set()
    cc_0 = fac.CounterCondition(min=0L, max=None, metadata=pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/dc/schemas/dc.xsd', 98, 4))
    counters.add(cc_0)
    states = []
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_0, False))
    symbol = pyxb.binding.content.ElementUse(elementContainer._UseForTag(pyxb.namespace.ExpandedName(Namespace, u'any')), pyxb.utils.utility.Location('/tmp/pyxbdist.FvaD3zy/PyXB-1.2.3/pyxb/bundles/dc/schemas/dc.xsd', 99, 6))
    st_0 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_0)
    transitions = []
    transitions.append(fac.Transition(st_0, [
        fac.UpdateInstruction(cc_0, True) ]))
    st_0._set_transitionSet(transitions)
    return fac.Automaton(states, counters, True, containing_state=None)
elementContainer._Automaton = _BuildAutomaton_()


title._setSubstitutionGroup(any)

creator._setSubstitutionGroup(any)

subject._setSubstitutionGroup(any)

description._setSubstitutionGroup(any)

publisher._setSubstitutionGroup(any)

contributor._setSubstitutionGroup(any)

date._setSubstitutionGroup(any)

type._setSubstitutionGroup(any)

format._setSubstitutionGroup(any)

identifier._setSubstitutionGroup(any)

source._setSubstitutionGroup(any)

language._setSubstitutionGroup(any)

relation._setSubstitutionGroup(any)

coverage._setSubstitutionGroup(any)

rights._setSubstitutionGroup(any)
