# .\_adamsCar.py
# -*- coding: utf-8 -*-
# PyXB bindings for NM:d15a81270be312be76c1307050be0cc3df24dc84
# Generated 2023-01-23 15:50:31.164000 by PyXB version 1.2.3
# Namespace adamsCar [xmlns:adamsCar]

import pyxb
import pyxb.binding
import pyxb.binding.saxer
import io
import pyxb.utils.utility
import pyxb.utils.domutils
import sys

# Unique identifier for bindings created at the same time
_GenerationUID = pyxb.utils.utility.UniqueIdentifier('urn:uuid:f4c2140f-9b67-11ed-b37a-415645000030')

# Version of PyXB used to generate the bindings
_PyXBVersion = '1.2.3'
# Generated bindings are not compatible across PyXB versions
if pyxb.__version__ != _PyXBVersion:
    raise pyxb.PyXBVersionError(_PyXBVersion)

# Import bindings for namespaces imported into schema
import pyxb.binding.datatypes
import avm as _ImportedBinding__avm

# NOTE: All namespace declarations are reserved within the binding
Namespace = pyxb.namespace.NamespaceForURI(u'adamsCar', create_if_missing=True)
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


# List simple type: [anonymous]
# superclasses pyxb.binding.datatypes.anySimpleType
class STD_ANON (pyxb.binding.basis.STD_list):

    """Simple type that is a list of pyxb.binding.datatypes.anyURI."""

    _ExpandedName = None
    _XSDLocation = pyxb.utils.utility.Location(u'avm.adamsCar.xsd', 30, 6)
    _Documentation = None

    _ItemType = pyxb.binding.datatypes.anyURI
STD_ANON._InitializeFacetMap()

# List simple type: [anonymous]
# superclasses pyxb.binding.datatypes.anySimpleType
class STD_ANON_ (pyxb.binding.basis.STD_list):

    """Simple type that is a list of pyxb.binding.datatypes.anyURI."""

    _ExpandedName = None
    _XSDLocation = pyxb.utils.utility.Location(u'avm.adamsCar.xsd', 35, 6)
    _Documentation = None

    _ItemType = pyxb.binding.datatypes.anyURI
STD_ANON_._InitializeFacetMap()

# Complex type {adamsCar}AdamsCarModel with content type ELEMENT_ONLY
class AdamsCarModel_ (_ImportedBinding__avm.DomainModel_):
    """Complex type {adamsCar}AdamsCarModel with content type ELEMENT_ONLY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'AdamsCarModel')
    _XSDLocation = pyxb.utils.utility.Location(u'avm.adamsCar.xsd', 7, 2)
    _ElementMap = _ImportedBinding__avm.DomainModel_._ElementMap.copy()
    _AttributeMap = _ImportedBinding__avm.DomainModel_._AttributeMap.copy()
    # Base type is _ImportedBinding__avm.DomainModel_
    
    # Element Parameter uses Python identifier Parameter
    __Parameter = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'Parameter'), 'Parameter', '__adamsCar_AdamsCarModel__Parameter', True, pyxb.utils.utility.Location(u'avm.adamsCar.xsd', 11, 10), )

    
    Parameter = property(__Parameter.value, __Parameter.set, None, None)

    
    # Element FileReference uses Python identifier FileReference
    __FileReference = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'FileReference'), 'FileReference', '__adamsCar_AdamsCarModel__FileReference', True, pyxb.utils.utility.Location(u'avm.adamsCar.xsd', 12, 10), )

    
    FileReference = property(__FileReference.value, __FileReference.set, None, None)

    
    # Attribute UsesResource inherited from {avm}DomainModel
    
    # Attribute Author inherited from {avm}DomainModel
    
    # Attribute Notes inherited from {avm}DomainModel
    
    # Attribute XPosition inherited from {avm}DomainModel
    
    # Attribute YPosition inherited from {avm}DomainModel
    
    # Attribute Name inherited from {avm}DomainModel
    _ElementMap.update({
        __Parameter.name() : __Parameter,
        __FileReference.name() : __FileReference
    })
    _AttributeMap.update({
        
    })
Namespace.addCategoryObject('typeBinding', u'AdamsCarModel', AdamsCarModel_)


# Complex type {adamsCar}Parameter with content type ELEMENT_ONLY
class Parameter_ (_ImportedBinding__avm.DomainModelParameter_):
    """Complex type {adamsCar}Parameter with content type ELEMENT_ONLY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'Parameter')
    _XSDLocation = pyxb.utils.utility.Location(u'avm.adamsCar.xsd', 17, 2)
    _ElementMap = _ImportedBinding__avm.DomainModelParameter_._ElementMap.copy()
    _AttributeMap = _ImportedBinding__avm.DomainModelParameter_._AttributeMap.copy()
    # Base type is _ImportedBinding__avm.DomainModelParameter_
    
    # Element Value uses Python identifier Value
    __Value = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'Value'), 'Value', '__adamsCar_Parameter__Value', False, pyxb.utils.utility.Location(u'avm.adamsCar.xsd', 21, 10), )

    
    Value = property(__Value.value, __Value.set, None, None)

    
    # Attribute ID uses Python identifier ID
    __ID = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'ID'), 'ID', '__adamsCar_Parameter__ID', pyxb.binding.datatypes.ID, required=True)
    __ID._DeclarationLocation = pyxb.utils.utility.Location(u'avm.adamsCar.xsd', 23, 8)
    __ID._UseLocation = pyxb.utils.utility.Location(u'avm.adamsCar.xsd', 23, 8)
    
    ID = property(__ID.value, __ID.set, None, None)

    
    # Attribute Name uses Python identifier Name
    __Name = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'Name'), 'Name', '__adamsCar_Parameter__Name', pyxb.binding.datatypes.string, required=True)
    __Name._DeclarationLocation = pyxb.utils.utility.Location(u'avm.adamsCar.xsd', 24, 8)
    __Name._UseLocation = pyxb.utils.utility.Location(u'avm.adamsCar.xsd', 24, 8)
    
    Name = property(__Name.value, __Name.set, None, None)

    
    # Attribute YPosition inherited from {avm}DomainModelParameter
    
    # Attribute Notes inherited from {avm}DomainModelParameter
    
    # Attribute XPosition inherited from {avm}DomainModelParameter
    _ElementMap.update({
        __Value.name() : __Value
    })
    _AttributeMap.update({
        __ID.name() : __ID,
        __Name.name() : __Name
    })
Namespace.addCategoryObject('typeBinding', u'Parameter', Parameter_)


# Complex type {adamsCar}FileReference with content type EMPTY
class FileReference_ (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {adamsCar}FileReference with content type EMPTY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_EMPTY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'FileReference')
    _XSDLocation = pyxb.utils.utility.Location(u'avm.adamsCar.xsd', 28, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.anyType
    
    # Attribute ParameterSwap uses Python identifier ParameterSwap
    __ParameterSwap = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'ParameterSwap'), 'ParameterSwap', '__adamsCar_FileReference__ParameterSwap', STD_ANON)
    __ParameterSwap._DeclarationLocation = pyxb.utils.utility.Location(u'avm.adamsCar.xsd', 29, 4)
    __ParameterSwap._UseLocation = pyxb.utils.utility.Location(u'avm.adamsCar.xsd', 29, 4)
    
    ParameterSwap = property(__ParameterSwap.value, __ParameterSwap.set, None, None)

    
    # Attribute FileReferenceSwap uses Python identifier FileReferenceSwap
    __FileReferenceSwap = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'FileReferenceSwap'), 'FileReferenceSwap', '__adamsCar_FileReference__FileReferenceSwap', STD_ANON_)
    __FileReferenceSwap._DeclarationLocation = pyxb.utils.utility.Location(u'avm.adamsCar.xsd', 34, 4)
    __FileReferenceSwap._UseLocation = pyxb.utils.utility.Location(u'avm.adamsCar.xsd', 34, 4)
    
    FileReferenceSwap = property(__FileReferenceSwap.value, __FileReferenceSwap.set, None, None)

    
    # Attribute FilePath uses Python identifier FilePath
    __FilePath = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'FilePath'), 'FilePath', '__adamsCar_FileReference__FilePath', pyxb.binding.datatypes.string, required=True)
    __FilePath._DeclarationLocation = pyxb.utils.utility.Location(u'avm.adamsCar.xsd', 39, 4)
    __FilePath._UseLocation = pyxb.utils.utility.Location(u'avm.adamsCar.xsd', 39, 4)
    
    FilePath = property(__FilePath.value, __FilePath.set, None, None)

    
    # Attribute ID uses Python identifier ID
    __ID = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'ID'), 'ID', '__adamsCar_FileReference__ID', pyxb.binding.datatypes.ID, required=True)
    __ID._DeclarationLocation = pyxb.utils.utility.Location(u'avm.adamsCar.xsd', 40, 4)
    __ID._UseLocation = pyxb.utils.utility.Location(u'avm.adamsCar.xsd', 40, 4)
    
    ID = property(__ID.value, __ID.set, None, None)

    
    # Attribute Name uses Python identifier Name
    __Name = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'Name'), 'Name', '__adamsCar_FileReference__Name', pyxb.binding.datatypes.string, required=True)
    __Name._DeclarationLocation = pyxb.utils.utility.Location(u'avm.adamsCar.xsd', 41, 4)
    __Name._UseLocation = pyxb.utils.utility.Location(u'avm.adamsCar.xsd', 41, 4)
    
    Name = property(__Name.value, __Name.set, None, None)

    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __ParameterSwap.name() : __ParameterSwap,
        __FileReferenceSwap.name() : __FileReferenceSwap,
        __FilePath.name() : __FilePath,
        __ID.name() : __ID,
        __Name.name() : __Name
    })
Namespace.addCategoryObject('typeBinding', u'FileReference', FileReference_)


AdamsCarModel = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'AdamsCarModel'), AdamsCarModel_, location=pyxb.utils.utility.Location(u'avm.adamsCar.xsd', 4, 2))
Namespace.addCategoryObject('elementBinding', AdamsCarModel.name().localName(), AdamsCarModel)

Parameter = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'Parameter'), Parameter_, location=pyxb.utils.utility.Location(u'avm.adamsCar.xsd', 5, 2))
Namespace.addCategoryObject('elementBinding', Parameter.name().localName(), Parameter)

FileReference = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'FileReference'), FileReference_, location=pyxb.utils.utility.Location(u'avm.adamsCar.xsd', 6, 2))
Namespace.addCategoryObject('elementBinding', FileReference.name().localName(), FileReference)



AdamsCarModel_._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'Parameter'), Parameter_, scope=AdamsCarModel_, location=pyxb.utils.utility.Location(u'avm.adamsCar.xsd', 11, 10)))

AdamsCarModel_._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'FileReference'), FileReference_, scope=AdamsCarModel_, location=pyxb.utils.utility.Location(u'avm.adamsCar.xsd', 12, 10)))

def _BuildAutomaton ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton
    del _BuildAutomaton
    import pyxb.utils.fac as fac

    counters = set()
    cc_0 = fac.CounterCondition(min=0L, max=None, metadata=pyxb.utils.utility.Location(u'avm.adamsCar.xsd', 11, 10))
    counters.add(cc_0)
    states = []
    final_update = None
    symbol = pyxb.binding.content.ElementUse(AdamsCarModel_._UseForTag(pyxb.namespace.ExpandedName(None, u'Parameter')), pyxb.utils.utility.Location(u'avm.adamsCar.xsd', 11, 10))
    st_0 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_0)
    final_update = set()
    symbol = pyxb.binding.content.ElementUse(AdamsCarModel_._UseForTag(pyxb.namespace.ExpandedName(None, u'FileReference')), pyxb.utils.utility.Location(u'avm.adamsCar.xsd', 12, 10))
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
         ]))
    st_1._set_transitionSet(transitions)
    return fac.Automaton(states, counters, False, containing_state=None)
AdamsCarModel_._Automaton = _BuildAutomaton()




Parameter_._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'Value'), _ImportedBinding__avm.Value_, scope=Parameter_, location=pyxb.utils.utility.Location(u'avm.adamsCar.xsd', 21, 10)))

def _BuildAutomaton_ ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton_
    del _BuildAutomaton_
    import pyxb.utils.fac as fac

    counters = set()
    cc_0 = fac.CounterCondition(min=0L, max=1, metadata=pyxb.utils.utility.Location(u'avm.adamsCar.xsd', 21, 10))
    counters.add(cc_0)
    states = []
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_0, False))
    symbol = pyxb.binding.content.ElementUse(Parameter_._UseForTag(pyxb.namespace.ExpandedName(None, u'Value')), pyxb.utils.utility.Location(u'avm.adamsCar.xsd', 21, 10))
    st_0 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_0)
    transitions = []
    transitions.append(fac.Transition(st_0, [
        fac.UpdateInstruction(cc_0, True) ]))
    st_0._set_transitionSet(transitions)
    return fac.Automaton(states, counters, True, containing_state=None)
Parameter_._Automaton = _BuildAutomaton_()

