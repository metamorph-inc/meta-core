# .\_manufacturing.py
# -*- coding: utf-8 -*-
# PyXB bindings for NM:001f63800ae37c355961a3d233bb19365b0ea8cb
# Generated 2023-01-19 12:49:23.714021 by PyXB version 1.2.6 using Python 3.11.1.final.0
# Namespace manufacturing [xmlns:manufacturing]

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
_GenerationUID = pyxb.utils.utility.UniqueIdentifier('urn:uuid:fd91d353-9829-11ed-9fb1-415645000030')

# Version of PyXB used to generate the bindings
_PyXBVersion = '1.2.6'
# Generated bindings are not compatible across PyXB versions
if pyxb.__version__ != _PyXBVersion:
    raise pyxb.PyXBVersionError(_PyXBVersion)

# A holder for module-level binding classes so we can access them from
# inside class definitions where property names may conflict.
_module_typeBindings = pyxb.utils.utility.Object()

# Import bindings for namespaces imported into schema
import avm as _ImportedBinding__avm
import pyxb.binding.datatypes

# NOTE: All namespace declarations are reserved within the binding
Namespace = pyxb.namespace.NamespaceForURI('manufacturing', create_if_missing=True)
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


# Complex type {manufacturing}ManufacturingModel with content type ELEMENT_ONLY
class ManufacturingModel_ (_ImportedBinding__avm.DomainModel_):
    """Complex type {manufacturing}ManufacturingModel with content type ELEMENT_ONLY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'ManufacturingModel')
    _XSDLocation = pyxb.utils.utility.Location('avm.manufacturing.xsd', 7, 2)
    _ElementMap = _ImportedBinding__avm.DomainModel_._ElementMap.copy()
    _AttributeMap = _ImportedBinding__avm.DomainModel_._AttributeMap.copy()
    # Base type is _ImportedBinding__avm.DomainModel_
    
    # Element Parameter uses Python identifier Parameter
    __Parameter = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'Parameter'), 'Parameter', '__manufacturing_ManufacturingModel__Parameter', True, pyxb.utils.utility.Location('avm.manufacturing.xsd', 11, 10), )

    
    Parameter = property(__Parameter.value, __Parameter.set, None, None)

    
    # Element Metric uses Python identifier Metric
    __Metric = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'Metric'), 'Metric', '__manufacturing_ManufacturingModel__Metric', True, pyxb.utils.utility.Location('avm.manufacturing.xsd', 12, 10), )

    
    Metric = property(__Metric.value, __Metric.set, None, None)

    
    # Attribute UsesResource inherited from {avm}DomainModel
    
    # Attribute Author inherited from {avm}DomainModel
    
    # Attribute Notes inherited from {avm}DomainModel
    
    # Attribute XPosition inherited from {avm}DomainModel
    
    # Attribute YPosition inherited from {avm}DomainModel
    
    # Attribute Name inherited from {avm}DomainModel
    _ElementMap.update({
        __Parameter.name() : __Parameter,
        __Metric.name() : __Metric
    })
    _AttributeMap.update({
        
    })
_module_typeBindings.ManufacturingModel_ = ManufacturingModel_
Namespace.addCategoryObject('typeBinding', 'ManufacturingModel', ManufacturingModel_)


# Complex type {manufacturing}Parameter with content type ELEMENT_ONLY
class Parameter_ (_ImportedBinding__avm.DomainModelParameter_):
    """Complex type {manufacturing}Parameter with content type ELEMENT_ONLY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'Parameter')
    _XSDLocation = pyxb.utils.utility.Location('avm.manufacturing.xsd', 17, 2)
    _ElementMap = _ImportedBinding__avm.DomainModelParameter_._ElementMap.copy()
    _AttributeMap = _ImportedBinding__avm.DomainModelParameter_._AttributeMap.copy()
    # Base type is _ImportedBinding__avm.DomainModelParameter_
    
    # Element Value uses Python identifier Value
    __Value = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'Value'), 'Value', '__manufacturing_Parameter__Value', False, pyxb.utils.utility.Location('avm.manufacturing.xsd', 21, 10), )

    
    Value = property(__Value.value, __Value.set, None, None)

    
    # Attribute Notes inherited from {avm}DomainModelParameter
    
    # Attribute XPosition inherited from {avm}DomainModelParameter
    
    # Attribute YPosition inherited from {avm}DomainModelParameter
    
    # Attribute Locator uses Python identifier Locator
    __Locator = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, 'Locator'), 'Locator', '__manufacturing_Parameter__Locator', pyxb.binding.datatypes.string)
    __Locator._DeclarationLocation = pyxb.utils.utility.Location('avm.manufacturing.xsd', 23, 8)
    __Locator._UseLocation = pyxb.utils.utility.Location('avm.manufacturing.xsd', 23, 8)
    
    Locator = property(__Locator.value, __Locator.set, None, None)

    
    # Attribute Name uses Python identifier Name
    __Name = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, 'Name'), 'Name', '__manufacturing_Parameter__Name', pyxb.binding.datatypes.string)
    __Name._DeclarationLocation = pyxb.utils.utility.Location('avm.manufacturing.xsd', 24, 8)
    __Name._UseLocation = pyxb.utils.utility.Location('avm.manufacturing.xsd', 24, 8)
    
    Name = property(__Name.value, __Name.set, None, None)

    _ElementMap.update({
        __Value.name() : __Value
    })
    _AttributeMap.update({
        __Locator.name() : __Locator,
        __Name.name() : __Name
    })
_module_typeBindings.Parameter_ = Parameter_
Namespace.addCategoryObject('typeBinding', 'Parameter', Parameter_)


# Complex type {manufacturing}Metric with content type ELEMENT_ONLY
class Metric_ (_ImportedBinding__avm.DomainModelMetric_):
    """Complex type {manufacturing}Metric with content type ELEMENT_ONLY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'Metric')
    _XSDLocation = pyxb.utils.utility.Location('avm.manufacturing.xsd', 28, 2)
    _ElementMap = _ImportedBinding__avm.DomainModelMetric_._ElementMap.copy()
    _AttributeMap = _ImportedBinding__avm.DomainModelMetric_._AttributeMap.copy()
    # Base type is _ImportedBinding__avm.DomainModelMetric_
    
    # Element Value (Value) inherited from {avm}DomainModelMetric
    
    # Attribute ID inherited from {avm}DomainModelMetric
    
    # Attribute Notes inherited from {avm}DomainModelMetric
    
    # Attribute XPosition inherited from {avm}DomainModelMetric
    
    # Attribute YPosition inherited from {avm}DomainModelMetric
    
    # Attribute Name uses Python identifier Name
    __Name = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, 'Name'), 'Name', '__manufacturing_Metric__Name', pyxb.binding.datatypes.string, required=True)
    __Name._DeclarationLocation = pyxb.utils.utility.Location('avm.manufacturing.xsd', 31, 8)
    __Name._UseLocation = pyxb.utils.utility.Location('avm.manufacturing.xsd', 31, 8)
    
    Name = property(__Name.value, __Name.set, None, None)

    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __Name.name() : __Name
    })
_module_typeBindings.Metric_ = Metric_
Namespace.addCategoryObject('typeBinding', 'Metric', Metric_)


ManufacturingModel = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, 'ManufacturingModel'), ManufacturingModel_, location=pyxb.utils.utility.Location('avm.manufacturing.xsd', 4, 2))
Namespace.addCategoryObject('elementBinding', ManufacturingModel.name().localName(), ManufacturingModel)

Parameter = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, 'Parameter'), Parameter_, location=pyxb.utils.utility.Location('avm.manufacturing.xsd', 5, 2))
Namespace.addCategoryObject('elementBinding', Parameter.name().localName(), Parameter)

Metric = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, 'Metric'), Metric_, location=pyxb.utils.utility.Location('avm.manufacturing.xsd', 6, 2))
Namespace.addCategoryObject('elementBinding', Metric.name().localName(), Metric)



ManufacturingModel_._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'Parameter'), Parameter_, scope=ManufacturingModel_, location=pyxb.utils.utility.Location('avm.manufacturing.xsd', 11, 10)))

ManufacturingModel_._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'Metric'), Metric_, scope=ManufacturingModel_, location=pyxb.utils.utility.Location('avm.manufacturing.xsd', 12, 10)))

def _BuildAutomaton ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton
    del _BuildAutomaton
    import pyxb.utils.fac as fac

    counters = set()
    cc_0 = fac.CounterCondition(min=0, max=None, metadata=pyxb.utils.utility.Location('avm.manufacturing.xsd', 11, 10))
    counters.add(cc_0)
    cc_1 = fac.CounterCondition(min=0, max=None, metadata=pyxb.utils.utility.Location('avm.manufacturing.xsd', 12, 10))
    counters.add(cc_1)
    states = []
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_0, False))
    symbol = pyxb.binding.content.ElementUse(ManufacturingModel_._UseForTag(pyxb.namespace.ExpandedName(None, 'Parameter')), pyxb.utils.utility.Location('avm.manufacturing.xsd', 11, 10))
    st_0 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_0)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_1, False))
    symbol = pyxb.binding.content.ElementUse(ManufacturingModel_._UseForTag(pyxb.namespace.ExpandedName(None, 'Metric')), pyxb.utils.utility.Location('avm.manufacturing.xsd', 12, 10))
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
ManufacturingModel_._Automaton = _BuildAutomaton()




Parameter_._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'Value'), _ImportedBinding__avm.Value_, scope=Parameter_, location=pyxb.utils.utility.Location('avm.manufacturing.xsd', 21, 10)))

def _BuildAutomaton_ ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton_
    del _BuildAutomaton_
    import pyxb.utils.fac as fac

    counters = set()
    cc_0 = fac.CounterCondition(min=0, max=1, metadata=pyxb.utils.utility.Location('avm.manufacturing.xsd', 21, 10))
    counters.add(cc_0)
    states = []
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_0, False))
    symbol = pyxb.binding.content.ElementUse(Parameter_._UseForTag(pyxb.namespace.ExpandedName(None, 'Value')), pyxb.utils.utility.Location('avm.manufacturing.xsd', 21, 10))
    st_0 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_0)
    transitions = []
    transitions.append(fac.Transition(st_0, [
        fac.UpdateInstruction(cc_0, True) ]))
    st_0._set_transitionSet(transitions)
    return fac.Automaton(states, counters, True, containing_state=None)
Parameter_._Automaton = _BuildAutomaton_()




def _BuildAutomaton_2 ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton_2
    del _BuildAutomaton_2
    import pyxb.utils.fac as fac

    counters = set()
    cc_0 = fac.CounterCondition(min=0, max=1, metadata=pyxb.utils.utility.Location('avm.xsd', 265, 6))
    counters.add(cc_0)
    states = []
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_0, False))
    symbol = pyxb.binding.content.ElementUse(Metric_._UseForTag(pyxb.namespace.ExpandedName(None, 'Value')), pyxb.utils.utility.Location('avm.xsd', 265, 6))
    st_0 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_0)
    transitions = []
    transitions.append(fac.Transition(st_0, [
        fac.UpdateInstruction(cc_0, True) ]))
    st_0._set_transitionSet(transitions)
    return fac.Automaton(states, counters, True, containing_state=None)
Metric_._Automaton = _BuildAutomaton_2()

