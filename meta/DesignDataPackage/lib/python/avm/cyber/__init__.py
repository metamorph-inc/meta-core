# .\_cyber.py
# -*- coding: utf-8 -*-
# PyXB bindings for NM:9017347a610d1436c1aaf52764e6578e8fc1a083
# Generated 2014-10-29 10:43:04.550000 by PyXB version 1.2.3
# Namespace cyber [xmlns:cyber]

import pyxb
import pyxb.binding
import pyxb.binding.saxer
import io
import pyxb.utils.utility
import pyxb.utils.domutils
import sys

# Unique identifier for bindings created at the same time
_GenerationUID = pyxb.utils.utility.UniqueIdentifier('urn:uuid:455827f0-5f82-11e4-94dc-20c9d0878c2e')

# Version of PyXB used to generate the bindings
_PyXBVersion = '1.2.3'
# Generated bindings are not compatible across PyXB versions
if pyxb.__version__ != _PyXBVersion:
    raise pyxb.PyXBVersionError(_PyXBVersion)

# Import bindings for namespaces imported into schema
import avm as _ImportedBinding__avm
import pyxb.binding.datatypes
import avm.modelica as _ImportedBinding__modelica

# NOTE: All namespace declarations are reserved within the binding
Namespace = pyxb.namespace.NamespaceForURI(u'cyber', create_if_missing=True)
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


# Atomic simple type: {cyber}ModelType
class ModelType (pyxb.binding.datatypes.string, pyxb.binding.basis.enumeration_mixin):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'ModelType')
    _XSDLocation = pyxb.utils.utility.Location(u'avm.cyber.xsd', 19, 2)
    _Documentation = None
ModelType._CF_enumeration = pyxb.binding.facets.CF_enumeration(value_datatype=ModelType, enum_prefix=None)
ModelType.SignalFlow = ModelType._CF_enumeration.addEnumeration(unicode_value=u'SignalFlow', tag=u'SignalFlow')
ModelType.Simulink = ModelType._CF_enumeration.addEnumeration(unicode_value=u'Simulink', tag=u'Simulink')
ModelType.ESMoL = ModelType._CF_enumeration.addEnumeration(unicode_value=u'ESMoL', tag=u'ESMoL')
ModelType._InitializeFacetMap(ModelType._CF_enumeration)
Namespace.addCategoryObject('typeBinding', u'ModelType', ModelType)

# Complex type {cyber}CyberModel with content type ELEMENT_ONLY
class CyberModel_ (_ImportedBinding__avm.DomainModel_):
    """Complex type {cyber}CyberModel with content type ELEMENT_ONLY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'CyberModel')
    _XSDLocation = pyxb.utils.utility.Location(u'avm.cyber.xsd', 6, 2)
    _ElementMap = _ImportedBinding__avm.DomainModel_._ElementMap.copy()
    _AttributeMap = _ImportedBinding__avm.DomainModel_._AttributeMap.copy()
    # Base type is _ImportedBinding__avm.DomainModel_
    
    # Element Connector uses Python identifier Connector
    __Connector = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'Connector'), 'Connector', '__cyber_CyberModel__Connector', True, pyxb.utils.utility.Location(u'avm.cyber.xsd', 10, 10), )

    
    Connector = property(__Connector.value, __Connector.set, None, None)

    
    # Element Parameter uses Python identifier Parameter
    __Parameter = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'Parameter'), 'Parameter', '__cyber_CyberModel__Parameter', True, pyxb.utils.utility.Location(u'avm.cyber.xsd', 11, 10), )

    
    Parameter = property(__Parameter.value, __Parameter.set, None, None)

    
    # Attribute UsesResource inherited from {avm}DomainModel
    
    # Attribute Author inherited from {avm}DomainModel
    
    # Attribute Notes inherited from {avm}DomainModel
    
    # Attribute XPosition inherited from {avm}DomainModel
    
    # Attribute YPosition inherited from {avm}DomainModel
    
    # Attribute Name inherited from {avm}DomainModel
    
    # Attribute Type uses Python identifier Type
    __Type = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'Type'), 'Type', '__cyber_CyberModel__Type', ModelType, required=True)
    __Type._DeclarationLocation = pyxb.utils.utility.Location(u'avm.cyber.xsd', 13, 8)
    __Type._UseLocation = pyxb.utils.utility.Location(u'avm.cyber.xsd', 13, 8)
    
    Type = property(__Type.value, __Type.set, None, None)

    
    # Attribute Class uses Python identifier Class
    __Class = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'Class'), 'Class', '__cyber_CyberModel__Class', pyxb.binding.datatypes.string, required=True)
    __Class._DeclarationLocation = pyxb.utils.utility.Location(u'avm.cyber.xsd', 14, 8)
    __Class._UseLocation = pyxb.utils.utility.Location(u'avm.cyber.xsd', 14, 8)
    
    Class = property(__Class.value, __Class.set, None, None)

    
    # Attribute Locator uses Python identifier Locator
    __Locator = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'Locator'), 'Locator', '__cyber_CyberModel__Locator', pyxb.binding.datatypes.string, required=True)
    __Locator._DeclarationLocation = pyxb.utils.utility.Location(u'avm.cyber.xsd', 15, 8)
    __Locator._UseLocation = pyxb.utils.utility.Location(u'avm.cyber.xsd', 15, 8)
    
    Locator = property(__Locator.value, __Locator.set, None, None)

    _ElementMap.update({
        __Connector.name() : __Connector,
        __Parameter.name() : __Parameter
    })
    _AttributeMap.update({
        __Type.name() : __Type,
        __Class.name() : __Class,
        __Locator.name() : __Locator
    })
Namespace.addCategoryObject('typeBinding', u'CyberModel', CyberModel_)


CyberModel = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'CyberModel'), CyberModel_, location=pyxb.utils.utility.Location(u'avm.cyber.xsd', 5, 2))
Namespace.addCategoryObject('elementBinding', CyberModel.name().localName(), CyberModel)



CyberModel_._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'Connector'), _ImportedBinding__modelica.Connector_, scope=CyberModel_, location=pyxb.utils.utility.Location(u'avm.cyber.xsd', 10, 10)))

CyberModel_._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'Parameter'), _ImportedBinding__modelica.Parameter_, scope=CyberModel_, location=pyxb.utils.utility.Location(u'avm.cyber.xsd', 11, 10)))

def _BuildAutomaton ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton
    del _BuildAutomaton
    import pyxb.utils.fac as fac

    counters = set()
    cc_0 = fac.CounterCondition(min=0L, max=None, metadata=pyxb.utils.utility.Location(u'avm.cyber.xsd', 10, 10))
    counters.add(cc_0)
    cc_1 = fac.CounterCondition(min=0L, max=None, metadata=pyxb.utils.utility.Location(u'avm.cyber.xsd', 11, 10))
    counters.add(cc_1)
    states = []
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_0, False))
    symbol = pyxb.binding.content.ElementUse(CyberModel_._UseForTag(pyxb.namespace.ExpandedName(None, u'Connector')), pyxb.utils.utility.Location(u'avm.cyber.xsd', 10, 10))
    st_0 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_0)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_1, False))
    symbol = pyxb.binding.content.ElementUse(CyberModel_._UseForTag(pyxb.namespace.ExpandedName(None, u'Parameter')), pyxb.utils.utility.Location(u'avm.cyber.xsd', 11, 10))
    st_1 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_1)
    transitions = []
    transitions.append(fac.Transition(st_0, [
        fac.UpdateInstruction(cc_0, True) ]))
    transitions.append(fac.Transition(st_1, [
        fac.UpdateInstruction(cc_0, False) ]))
    st_0._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_1, [
        fac.UpdateInstruction(cc_1, True) ]))
    st_1._set_transitionSet(transitions)
    return fac.Automaton(states, counters, True, containing_state=None)
CyberModel_._Automaton = _BuildAutomaton()

