# .\_modelica.py
# -*- coding: utf-8 -*-
# PyXB bindings for NM:98f83d5ca0f2d925890a625eebbdfd52bb83d5c7
# Generated 2023-01-19 12:49:23.714021 by PyXB version 1.2.6 using Python 3.11.1.final.0
# Namespace modelica [xmlns:modelica]

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
import pyxb.binding.datatypes
import avm as _ImportedBinding__avm

# NOTE: All namespace declarations are reserved within the binding
Namespace = pyxb.namespace.NamespaceForURI('modelica', create_if_missing=True)
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


# Atomic simple type: {modelica}BoundTypeEnum
class BoundTypeEnum (pyxb.binding.datatypes.string, pyxb.binding.basis.enumeration_mixin):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'BoundTypeEnum')
    _XSDLocation = pyxb.utils.utility.Location('avm.modelica.xsd', 64, 2)
    _Documentation = None
BoundTypeEnum._CF_enumeration = pyxb.binding.facets.CF_enumeration(value_datatype=BoundTypeEnum, enum_prefix=None)
BoundTypeEnum.MustNotExceed = BoundTypeEnum._CF_enumeration.addEnumeration(unicode_value='MustNotExceed', tag='MustNotExceed')
BoundTypeEnum.MustNotMeetOrExceed = BoundTypeEnum._CF_enumeration.addEnumeration(unicode_value='MustNotMeetOrExceed', tag='MustNotMeetOrExceed')
BoundTypeEnum.MustExceed = BoundTypeEnum._CF_enumeration.addEnumeration(unicode_value='MustExceed', tag='MustExceed')
BoundTypeEnum.MustExceedOrEqual = BoundTypeEnum._CF_enumeration.addEnumeration(unicode_value='MustExceedOrEqual', tag='MustExceedOrEqual')
BoundTypeEnum._InitializeFacetMap(BoundTypeEnum._CF_enumeration)
Namespace.addCategoryObject('typeBinding', 'BoundTypeEnum', BoundTypeEnum)
_module_typeBindings.BoundTypeEnum = BoundTypeEnum

# Atomic simple type: {modelica}RedeclareTypeEnum
class RedeclareTypeEnum (pyxb.binding.datatypes.string, pyxb.binding.basis.enumeration_mixin):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'RedeclareTypeEnum')
    _XSDLocation = pyxb.utils.utility.Location('avm.modelica.xsd', 83, 2)
    _Documentation = None
RedeclareTypeEnum._CF_enumeration = pyxb.binding.facets.CF_enumeration(value_datatype=RedeclareTypeEnum, enum_prefix=None)
RedeclareTypeEnum.Package = RedeclareTypeEnum._CF_enumeration.addEnumeration(unicode_value='Package', tag='Package')
RedeclareTypeEnum.Class = RedeclareTypeEnum._CF_enumeration.addEnumeration(unicode_value='Class', tag='Class')
RedeclareTypeEnum.Model = RedeclareTypeEnum._CF_enumeration.addEnumeration(unicode_value='Model', tag='Model')
RedeclareTypeEnum.Record = RedeclareTypeEnum._CF_enumeration.addEnumeration(unicode_value='Record', tag='Record')
RedeclareTypeEnum.Block = RedeclareTypeEnum._CF_enumeration.addEnumeration(unicode_value='Block', tag='Block')
RedeclareTypeEnum.Connector = RedeclareTypeEnum._CF_enumeration.addEnumeration(unicode_value='Connector', tag='Connector')
RedeclareTypeEnum.Function = RedeclareTypeEnum._CF_enumeration.addEnumeration(unicode_value='Function', tag='Function')
RedeclareTypeEnum._InitializeFacetMap(RedeclareTypeEnum._CF_enumeration)
Namespace.addCategoryObject('typeBinding', 'RedeclareTypeEnum', RedeclareTypeEnum)
_module_typeBindings.RedeclareTypeEnum = RedeclareTypeEnum

# Atomic simple type: {modelica}IntervalMethod
class IntervalMethod (pyxb.binding.datatypes.string, pyxb.binding.basis.enumeration_mixin):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'IntervalMethod')
    _XSDLocation = pyxb.utils.utility.Location('avm.modelica.xsd', 109, 2)
    _Documentation = None
IntervalMethod._CF_enumeration = pyxb.binding.facets.CF_enumeration(value_datatype=IntervalMethod, enum_prefix=None)
IntervalMethod.NumberOfIntervals = IntervalMethod._CF_enumeration.addEnumeration(unicode_value='NumberOfIntervals', tag='NumberOfIntervals')
IntervalMethod.IntervalLength = IntervalMethod._CF_enumeration.addEnumeration(unicode_value='IntervalLength', tag='IntervalLength')
IntervalMethod._InitializeFacetMap(IntervalMethod._CF_enumeration)
Namespace.addCategoryObject('typeBinding', 'IntervalMethod', IntervalMethod)
_module_typeBindings.IntervalMethod = IntervalMethod

# Atomic simple type: {modelica}JobManagerToolSelection
class JobManagerToolSelection (pyxb.binding.datatypes.string, pyxb.binding.basis.enumeration_mixin):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'JobManagerToolSelection')
    _XSDLocation = pyxb.utils.utility.Location('avm.modelica.xsd', 115, 2)
    _Documentation = None
JobManagerToolSelection._CF_enumeration = pyxb.binding.facets.CF_enumeration(value_datatype=JobManagerToolSelection, enum_prefix=None)
JobManagerToolSelection.Dymola_latest = JobManagerToolSelection._CF_enumeration.addEnumeration(unicode_value='Dymola_latest', tag='Dymola_latest')
JobManagerToolSelection.Dymola_2014 = JobManagerToolSelection._CF_enumeration.addEnumeration(unicode_value='Dymola_2014', tag='Dymola_2014')
JobManagerToolSelection.Dymola_2013 = JobManagerToolSelection._CF_enumeration.addEnumeration(unicode_value='Dymola_2013', tag='Dymola_2013')
JobManagerToolSelection.OpenModelica_latest = JobManagerToolSelection._CF_enumeration.addEnumeration(unicode_value='OpenModelica_latest', tag='OpenModelica_latest')
JobManagerToolSelection.JModelica_1_12 = JobManagerToolSelection._CF_enumeration.addEnumeration(unicode_value='JModelica_1_12', tag='JModelica_1_12')
JobManagerToolSelection._InitializeFacetMap(JobManagerToolSelection._CF_enumeration)
Namespace.addCategoryObject('typeBinding', 'JobManagerToolSelection', JobManagerToolSelection)
_module_typeBindings.JobManagerToolSelection = JobManagerToolSelection

# Complex type {modelica}ModelicaModel with content type ELEMENT_ONLY
class ModelicaModel_ (_ImportedBinding__avm.DomainModel_):
    """Complex type {modelica}ModelicaModel with content type ELEMENT_ONLY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'ModelicaModel')
    _XSDLocation = pyxb.utils.utility.Location('avm.modelica.xsd', 11, 2)
    _ElementMap = _ImportedBinding__avm.DomainModel_._ElementMap.copy()
    _AttributeMap = _ImportedBinding__avm.DomainModel_._AttributeMap.copy()
    # Base type is _ImportedBinding__avm.DomainModel_
    
    # Element Parameter uses Python identifier Parameter
    __Parameter = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'Parameter'), 'Parameter', '__modelica_ModelicaModel__Parameter', True, pyxb.utils.utility.Location('avm.modelica.xsd', 15, 10), )

    
    Parameter = property(__Parameter.value, __Parameter.set, None, None)

    
    # Element Connector uses Python identifier Connector
    __Connector = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'Connector'), 'Connector', '__modelica_ModelicaModel__Connector', True, pyxb.utils.utility.Location('avm.modelica.xsd', 16, 10), )

    
    Connector = property(__Connector.value, __Connector.set, None, None)

    
    # Element Metric uses Python identifier Metric
    __Metric = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'Metric'), 'Metric', '__modelica_ModelicaModel__Metric', True, pyxb.utils.utility.Location('avm.modelica.xsd', 17, 10), )

    
    Metric = property(__Metric.value, __Metric.set, None, None)

    
    # Element Limit uses Python identifier Limit
    __Limit = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'Limit'), 'Limit', '__modelica_ModelicaModel__Limit', True, pyxb.utils.utility.Location('avm.modelica.xsd', 18, 10), )

    
    Limit = property(__Limit.value, __Limit.set, None, None)

    
    # Element Redeclare uses Python identifier Redeclare
    __Redeclare = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'Redeclare'), 'Redeclare', '__modelica_ModelicaModel__Redeclare', True, pyxb.utils.utility.Location('avm.modelica.xsd', 19, 10), )

    
    Redeclare = property(__Redeclare.value, __Redeclare.set, None, None)

    
    # Attribute UsesResource inherited from {avm}DomainModel
    
    # Attribute Author inherited from {avm}DomainModel
    
    # Attribute Notes inherited from {avm}DomainModel
    
    # Attribute XPosition inherited from {avm}DomainModel
    
    # Attribute YPosition inherited from {avm}DomainModel
    
    # Attribute Name inherited from {avm}DomainModel
    
    # Attribute Class uses Python identifier Class
    __Class = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, 'Class'), 'Class', '__modelica_ModelicaModel__Class', pyxb.binding.datatypes.string, required=True)
    __Class._DeclarationLocation = pyxb.utils.utility.Location('avm.modelica.xsd', 21, 8)
    __Class._UseLocation = pyxb.utils.utility.Location('avm.modelica.xsd', 21, 8)
    
    Class = property(__Class.value, __Class.set, None, None)

    _ElementMap.update({
        __Parameter.name() : __Parameter,
        __Connector.name() : __Connector,
        __Metric.name() : __Metric,
        __Limit.name() : __Limit,
        __Redeclare.name() : __Redeclare
    })
    _AttributeMap.update({
        __Class.name() : __Class
    })
_module_typeBindings.ModelicaModel_ = ModelicaModel_
Namespace.addCategoryObject('typeBinding', 'ModelicaModel', ModelicaModel_)


# Complex type {modelica}Parameter with content type ELEMENT_ONLY
class Parameter_ (_ImportedBinding__avm.DomainModelParameter_):
    """Complex type {modelica}Parameter with content type ELEMENT_ONLY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'Parameter')
    _XSDLocation = pyxb.utils.utility.Location('avm.modelica.xsd', 37, 2)
    _ElementMap = _ImportedBinding__avm.DomainModelParameter_._ElementMap.copy()
    _AttributeMap = _ImportedBinding__avm.DomainModelParameter_._AttributeMap.copy()
    # Base type is _ImportedBinding__avm.DomainModelParameter_
    
    # Element Value uses Python identifier Value
    __Value = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'Value'), 'Value', '__modelica_Parameter__Value', False, pyxb.utils.utility.Location('avm.modelica.xsd', 41, 10), )

    
    Value = property(__Value.value, __Value.set, None, None)

    
    # Attribute Notes inherited from {avm}DomainModelParameter
    
    # Attribute XPosition inherited from {avm}DomainModelParameter
    
    # Attribute YPosition inherited from {avm}DomainModelParameter
    
    # Attribute Locator uses Python identifier Locator
    __Locator = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, 'Locator'), 'Locator', '__modelica_Parameter__Locator', pyxb.binding.datatypes.string, required=True)
    __Locator._DeclarationLocation = pyxb.utils.utility.Location('avm.modelica.xsd', 43, 8)
    __Locator._UseLocation = pyxb.utils.utility.Location('avm.modelica.xsd', 43, 8)
    
    Locator = property(__Locator.value, __Locator.set, None, None)

    _ElementMap.update({
        __Value.name() : __Value
    })
    _AttributeMap.update({
        __Locator.name() : __Locator
    })
_module_typeBindings.Parameter_ = Parameter_
Namespace.addCategoryObject('typeBinding', 'Parameter', Parameter_)


# Complex type {modelica}Metric with content type ELEMENT_ONLY
class Metric_ (_ImportedBinding__avm.DomainModelMetric_):
    """Complex type {modelica}Metric with content type ELEMENT_ONLY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'Metric')
    _XSDLocation = pyxb.utils.utility.Location('avm.modelica.xsd', 47, 2)
    _ElementMap = _ImportedBinding__avm.DomainModelMetric_._ElementMap.copy()
    _AttributeMap = _ImportedBinding__avm.DomainModelMetric_._AttributeMap.copy()
    # Base type is _ImportedBinding__avm.DomainModelMetric_
    
    # Element Value (Value) inherited from {avm}DomainModelMetric
    
    # Attribute ID inherited from {avm}DomainModelMetric
    
    # Attribute Notes inherited from {avm}DomainModelMetric
    
    # Attribute XPosition inherited from {avm}DomainModelMetric
    
    # Attribute YPosition inherited from {avm}DomainModelMetric
    
    # Attribute Locator uses Python identifier Locator
    __Locator = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, 'Locator'), 'Locator', '__modelica_Metric__Locator', pyxb.binding.datatypes.string, required=True)
    __Locator._DeclarationLocation = pyxb.utils.utility.Location('avm.modelica.xsd', 50, 8)
    __Locator._UseLocation = pyxb.utils.utility.Location('avm.modelica.xsd', 50, 8)
    
    Locator = property(__Locator.value, __Locator.set, None, None)

    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __Locator.name() : __Locator
    })
_module_typeBindings.Metric_ = Metric_
Namespace.addCategoryObject('typeBinding', 'Metric', Metric_)


# Complex type {modelica}Limit with content type ELEMENT_ONLY
class Limit_ (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {modelica}Limit with content type ELEMENT_ONLY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'Limit')
    _XSDLocation = pyxb.utils.utility.Location('avm.modelica.xsd', 54, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.anyType
    
    # Element TargetValue uses Python identifier TargetValue
    __TargetValue = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'TargetValue'), 'TargetValue', '__modelica_Limit__TargetValue', False, pyxb.utils.utility.Location('avm.modelica.xsd', 56, 6), )

    
    TargetValue = property(__TargetValue.value, __TargetValue.set, None, None)

    
    # Attribute VariableLocator uses Python identifier VariableLocator
    __VariableLocator = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, 'VariableLocator'), 'VariableLocator', '__modelica_Limit__VariableLocator', pyxb.binding.datatypes.string, required=True)
    __VariableLocator._DeclarationLocation = pyxb.utils.utility.Location('avm.modelica.xsd', 58, 4)
    __VariableLocator._UseLocation = pyxb.utils.utility.Location('avm.modelica.xsd', 58, 4)
    
    VariableLocator = property(__VariableLocator.value, __VariableLocator.set, None, None)

    
    # Attribute BoundType uses Python identifier BoundType
    __BoundType = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, 'BoundType'), 'BoundType', '__modelica_Limit__BoundType', _module_typeBindings.BoundTypeEnum, required=True)
    __BoundType._DeclarationLocation = pyxb.utils.utility.Location('avm.modelica.xsd', 59, 4)
    __BoundType._UseLocation = pyxb.utils.utility.Location('avm.modelica.xsd', 59, 4)
    
    BoundType = property(__BoundType.value, __BoundType.set, None, None)

    
    # Attribute Name uses Python identifier Name
    __Name = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, 'Name'), 'Name', '__modelica_Limit__Name', pyxb.binding.datatypes.string)
    __Name._DeclarationLocation = pyxb.utils.utility.Location('avm.modelica.xsd', 60, 4)
    __Name._UseLocation = pyxb.utils.utility.Location('avm.modelica.xsd', 60, 4)
    
    Name = property(__Name.value, __Name.set, None, None)

    
    # Attribute ToleranceTimeWindow uses Python identifier ToleranceTimeWindow
    __ToleranceTimeWindow = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, 'ToleranceTimeWindow'), 'ToleranceTimeWindow', '__modelica_Limit__ToleranceTimeWindow', pyxb.binding.datatypes.float)
    __ToleranceTimeWindow._DeclarationLocation = pyxb.utils.utility.Location('avm.modelica.xsd', 61, 4)
    __ToleranceTimeWindow._UseLocation = pyxb.utils.utility.Location('avm.modelica.xsd', 61, 4)
    
    ToleranceTimeWindow = property(__ToleranceTimeWindow.value, __ToleranceTimeWindow.set, None, None)

    
    # Attribute Notes uses Python identifier Notes
    __Notes = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, 'Notes'), 'Notes', '__modelica_Limit__Notes', pyxb.binding.datatypes.string)
    __Notes._DeclarationLocation = pyxb.utils.utility.Location('avm.modelica.xsd', 62, 4)
    __Notes._UseLocation = pyxb.utils.utility.Location('avm.modelica.xsd', 62, 4)
    
    Notes = property(__Notes.value, __Notes.set, None, None)

    _ElementMap.update({
        __TargetValue.name() : __TargetValue
    })
    _AttributeMap.update({
        __VariableLocator.name() : __VariableLocator,
        __BoundType.name() : __BoundType,
        __Name.name() : __Name,
        __ToleranceTimeWindow.name() : __ToleranceTimeWindow,
        __Notes.name() : __Notes
    })
_module_typeBindings.Limit_ = Limit_
Namespace.addCategoryObject('typeBinding', 'Limit', Limit_)


# Complex type {modelica}Redeclare with content type ELEMENT_ONLY
class Redeclare_ (_ImportedBinding__avm.DomainModelParameter_):
    """Complex type {modelica}Redeclare with content type ELEMENT_ONLY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'Redeclare')
    _XSDLocation = pyxb.utils.utility.Location('avm.modelica.xsd', 72, 2)
    _ElementMap = _ImportedBinding__avm.DomainModelParameter_._ElementMap.copy()
    _AttributeMap = _ImportedBinding__avm.DomainModelParameter_._AttributeMap.copy()
    # Base type is _ImportedBinding__avm.DomainModelParameter_
    
    # Element Value uses Python identifier Value
    __Value = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'Value'), 'Value', '__modelica_Redeclare__Value', False, pyxb.utils.utility.Location('avm.modelica.xsd', 76, 10), )

    
    Value = property(__Value.value, __Value.set, None, None)

    
    # Attribute Notes inherited from {avm}DomainModelParameter
    
    # Attribute XPosition inherited from {avm}DomainModelParameter
    
    # Attribute YPosition inherited from {avm}DomainModelParameter
    
    # Attribute Locator uses Python identifier Locator
    __Locator = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, 'Locator'), 'Locator', '__modelica_Redeclare__Locator', pyxb.binding.datatypes.string, required=True)
    __Locator._DeclarationLocation = pyxb.utils.utility.Location('avm.modelica.xsd', 78, 8)
    __Locator._UseLocation = pyxb.utils.utility.Location('avm.modelica.xsd', 78, 8)
    
    Locator = property(__Locator.value, __Locator.set, None, None)

    
    # Attribute Type uses Python identifier Type
    __Type = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, 'Type'), 'Type', '__modelica_Redeclare__Type', _module_typeBindings.RedeclareTypeEnum)
    __Type._DeclarationLocation = pyxb.utils.utility.Location('avm.modelica.xsd', 79, 8)
    __Type._UseLocation = pyxb.utils.utility.Location('avm.modelica.xsd', 79, 8)
    
    Type = property(__Type.value, __Type.set, None, None)

    _ElementMap.update({
        __Value.name() : __Value
    })
    _AttributeMap.update({
        __Locator.name() : __Locator,
        __Type.name() : __Type
    })
_module_typeBindings.Redeclare_ = Redeclare_
Namespace.addCategoryObject('typeBinding', 'Redeclare', Redeclare_)


# Complex type {modelica}SolverSettings with content type EMPTY
class SolverSettings_ (_ImportedBinding__avm.Settings_):
    """Complex type {modelica}SolverSettings with content type EMPTY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_EMPTY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'SolverSettings')
    _XSDLocation = pyxb.utils.utility.Location('avm.modelica.xsd', 94, 2)
    _ElementMap = _ImportedBinding__avm.Settings_._ElementMap.copy()
    _AttributeMap = _ImportedBinding__avm.Settings_._AttributeMap.copy()
    # Base type is _ImportedBinding__avm.Settings_
    
    # Attribute Solver uses Python identifier Solver
    __Solver = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, 'Solver'), 'Solver', '__modelica_SolverSettings__Solver', pyxb.binding.datatypes.string)
    __Solver._DeclarationLocation = pyxb.utils.utility.Location('avm.modelica.xsd', 97, 8)
    __Solver._UseLocation = pyxb.utils.utility.Location('avm.modelica.xsd', 97, 8)
    
    Solver = property(__Solver.value, __Solver.set, None, None)

    
    # Attribute JobManagerToolSelection uses Python identifier JobManagerToolSelection
    __JobManagerToolSelection = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, 'JobManagerToolSelection'), 'JobManagerToolSelection', '__modelica_SolverSettings__JobManagerToolSelection', _module_typeBindings.JobManagerToolSelection)
    __JobManagerToolSelection._DeclarationLocation = pyxb.utils.utility.Location('avm.modelica.xsd', 98, 8)
    __JobManagerToolSelection._UseLocation = pyxb.utils.utility.Location('avm.modelica.xsd', 98, 8)
    
    JobManagerToolSelection = property(__JobManagerToolSelection.value, __JobManagerToolSelection.set, None, None)

    
    # Attribute StartTime uses Python identifier StartTime
    __StartTime = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, 'StartTime'), 'StartTime', '__modelica_SolverSettings__StartTime', pyxb.binding.datatypes.double)
    __StartTime._DeclarationLocation = pyxb.utils.utility.Location('avm.modelica.xsd', 99, 8)
    __StartTime._UseLocation = pyxb.utils.utility.Location('avm.modelica.xsd', 99, 8)
    
    StartTime = property(__StartTime.value, __StartTime.set, None, None)

    
    # Attribute StopTime uses Python identifier StopTime
    __StopTime = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, 'StopTime'), 'StopTime', '__modelica_SolverSettings__StopTime', pyxb.binding.datatypes.double)
    __StopTime._DeclarationLocation = pyxb.utils.utility.Location('avm.modelica.xsd', 100, 8)
    __StopTime._UseLocation = pyxb.utils.utility.Location('avm.modelica.xsd', 100, 8)
    
    StopTime = property(__StopTime.value, __StopTime.set, None, None)

    
    # Attribute IntervalMethod uses Python identifier IntervalMethod
    __IntervalMethod = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, 'IntervalMethod'), 'IntervalMethod', '__modelica_SolverSettings__IntervalMethod', _module_typeBindings.IntervalMethod, unicode_default='NumberOfIntervals')
    __IntervalMethod._DeclarationLocation = pyxb.utils.utility.Location('avm.modelica.xsd', 101, 8)
    __IntervalMethod._UseLocation = pyxb.utils.utility.Location('avm.modelica.xsd', 101, 8)
    
    IntervalMethod = property(__IntervalMethod.value, __IntervalMethod.set, None, None)

    
    # Attribute NumberOfIntervals uses Python identifier NumberOfIntervals
    __NumberOfIntervals = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, 'NumberOfIntervals'), 'NumberOfIntervals', '__modelica_SolverSettings__NumberOfIntervals', pyxb.binding.datatypes.int)
    __NumberOfIntervals._DeclarationLocation = pyxb.utils.utility.Location('avm.modelica.xsd', 102, 8)
    __NumberOfIntervals._UseLocation = pyxb.utils.utility.Location('avm.modelica.xsd', 102, 8)
    
    NumberOfIntervals = property(__NumberOfIntervals.value, __NumberOfIntervals.set, None, None)

    
    # Attribute IntervalLength uses Python identifier IntervalLength
    __IntervalLength = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, 'IntervalLength'), 'IntervalLength', '__modelica_SolverSettings__IntervalLength', pyxb.binding.datatypes.double)
    __IntervalLength._DeclarationLocation = pyxb.utils.utility.Location('avm.modelica.xsd', 103, 8)
    __IntervalLength._UseLocation = pyxb.utils.utility.Location('avm.modelica.xsd', 103, 8)
    
    IntervalLength = property(__IntervalLength.value, __IntervalLength.set, None, None)

    
    # Attribute ToolSpecificAnnotations uses Python identifier ToolSpecificAnnotations
    __ToolSpecificAnnotations = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, 'ToolSpecificAnnotations'), 'ToolSpecificAnnotations', '__modelica_SolverSettings__ToolSpecificAnnotations', pyxb.binding.datatypes.string)
    __ToolSpecificAnnotations._DeclarationLocation = pyxb.utils.utility.Location('avm.modelica.xsd', 104, 8)
    __ToolSpecificAnnotations._UseLocation = pyxb.utils.utility.Location('avm.modelica.xsd', 104, 8)
    
    ToolSpecificAnnotations = property(__ToolSpecificAnnotations.value, __ToolSpecificAnnotations.set, None, None)

    
    # Attribute Tolerance uses Python identifier Tolerance
    __Tolerance = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, 'Tolerance'), 'Tolerance', '__modelica_SolverSettings__Tolerance', pyxb.binding.datatypes.double)
    __Tolerance._DeclarationLocation = pyxb.utils.utility.Location('avm.modelica.xsd', 105, 8)
    __Tolerance._UseLocation = pyxb.utils.utility.Location('avm.modelica.xsd', 105, 8)
    
    Tolerance = property(__Tolerance.value, __Tolerance.set, None, None)

    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __Solver.name() : __Solver,
        __JobManagerToolSelection.name() : __JobManagerToolSelection,
        __StartTime.name() : __StartTime,
        __StopTime.name() : __StopTime,
        __IntervalMethod.name() : __IntervalMethod,
        __NumberOfIntervals.name() : __NumberOfIntervals,
        __IntervalLength.name() : __IntervalLength,
        __ToolSpecificAnnotations.name() : __ToolSpecificAnnotations,
        __Tolerance.name() : __Tolerance
    })
_module_typeBindings.SolverSettings_ = SolverSettings_
Namespace.addCategoryObject('typeBinding', 'SolverSettings', SolverSettings_)


# Complex type {modelica}Connector with content type ELEMENT_ONLY
class Connector_ (_ImportedBinding__avm.DomainModelPort_):
    """Complex type {modelica}Connector with content type ELEMENT_ONLY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'Connector')
    _XSDLocation = pyxb.utils.utility.Location('avm.modelica.xsd', 25, 2)
    _ElementMap = _ImportedBinding__avm.DomainModelPort_._ElementMap.copy()
    _AttributeMap = _ImportedBinding__avm.DomainModelPort_._AttributeMap.copy()
    # Base type is _ImportedBinding__avm.DomainModelPort_
    
    # Element Parameter uses Python identifier Parameter
    __Parameter = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'Parameter'), 'Parameter', '__modelica_Connector__Parameter', True, pyxb.utils.utility.Location('avm.modelica.xsd', 29, 10), )

    
    Parameter = property(__Parameter.value, __Parameter.set, None, None)

    
    # Element Redeclare uses Python identifier Redeclare
    __Redeclare = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'Redeclare'), 'Redeclare', '__modelica_Connector__Redeclare', True, pyxb.utils.utility.Location('avm.modelica.xsd', 30, 10), )

    
    Redeclare = property(__Redeclare.value, __Redeclare.set, None, None)

    
    # Attribute Notes inherited from {avm}Port
    
    # Attribute XPosition inherited from {avm}Port
    
    # Attribute Definition inherited from {avm}Port
    
    # Attribute YPosition inherited from {avm}Port
    
    # Attribute Name inherited from {avm}Port
    
    # Attribute ID inherited from {avm}PortMapTarget
    
    # Attribute PortMap inherited from {avm}PortMapTarget
    
    # Attribute Class uses Python identifier Class
    __Class = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, 'Class'), 'Class', '__modelica_Connector__Class', pyxb.binding.datatypes.string, required=True)
    __Class._DeclarationLocation = pyxb.utils.utility.Location('avm.modelica.xsd', 32, 8)
    __Class._UseLocation = pyxb.utils.utility.Location('avm.modelica.xsd', 32, 8)
    
    Class = property(__Class.value, __Class.set, None, None)

    
    # Attribute Locator uses Python identifier Locator
    __Locator = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, 'Locator'), 'Locator', '__modelica_Connector__Locator', pyxb.binding.datatypes.string)
    __Locator._DeclarationLocation = pyxb.utils.utility.Location('avm.modelica.xsd', 33, 8)
    __Locator._UseLocation = pyxb.utils.utility.Location('avm.modelica.xsd', 33, 8)
    
    Locator = property(__Locator.value, __Locator.set, None, None)

    _ElementMap.update({
        __Parameter.name() : __Parameter,
        __Redeclare.name() : __Redeclare
    })
    _AttributeMap.update({
        __Class.name() : __Class,
        __Locator.name() : __Locator
    })
_module_typeBindings.Connector_ = Connector_
Namespace.addCategoryObject('typeBinding', 'Connector', Connector_)


ModelicaModel = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, 'ModelicaModel'), ModelicaModel_, location=pyxb.utils.utility.Location('avm.modelica.xsd', 4, 2))
Namespace.addCategoryObject('elementBinding', ModelicaModel.name().localName(), ModelicaModel)

Parameter = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, 'Parameter'), Parameter_, location=pyxb.utils.utility.Location('avm.modelica.xsd', 6, 2))
Namespace.addCategoryObject('elementBinding', Parameter.name().localName(), Parameter)

Metric = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, 'Metric'), Metric_, location=pyxb.utils.utility.Location('avm.modelica.xsd', 7, 2))
Namespace.addCategoryObject('elementBinding', Metric.name().localName(), Metric)

Limit = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, 'Limit'), Limit_, location=pyxb.utils.utility.Location('avm.modelica.xsd', 8, 2))
Namespace.addCategoryObject('elementBinding', Limit.name().localName(), Limit)

Redeclare = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, 'Redeclare'), Redeclare_, location=pyxb.utils.utility.Location('avm.modelica.xsd', 9, 2))
Namespace.addCategoryObject('elementBinding', Redeclare.name().localName(), Redeclare)

SolverSettings = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, 'SolverSettings'), SolverSettings_, location=pyxb.utils.utility.Location('avm.modelica.xsd', 10, 2))
Namespace.addCategoryObject('elementBinding', SolverSettings.name().localName(), SolverSettings)

Connector = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, 'Connector'), Connector_, location=pyxb.utils.utility.Location('avm.modelica.xsd', 5, 2))
Namespace.addCategoryObject('elementBinding', Connector.name().localName(), Connector)



ModelicaModel_._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'Parameter'), Parameter_, scope=ModelicaModel_, location=pyxb.utils.utility.Location('avm.modelica.xsd', 15, 10)))

ModelicaModel_._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'Connector'), Connector_, scope=ModelicaModel_, location=pyxb.utils.utility.Location('avm.modelica.xsd', 16, 10)))

ModelicaModel_._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'Metric'), Metric_, scope=ModelicaModel_, location=pyxb.utils.utility.Location('avm.modelica.xsd', 17, 10)))

ModelicaModel_._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'Limit'), Limit_, scope=ModelicaModel_, location=pyxb.utils.utility.Location('avm.modelica.xsd', 18, 10)))

ModelicaModel_._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'Redeclare'), Redeclare_, scope=ModelicaModel_, location=pyxb.utils.utility.Location('avm.modelica.xsd', 19, 10)))

def _BuildAutomaton ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton
    del _BuildAutomaton
    import pyxb.utils.fac as fac

    counters = set()
    cc_0 = fac.CounterCondition(min=0, max=None, metadata=pyxb.utils.utility.Location('avm.modelica.xsd', 15, 10))
    counters.add(cc_0)
    cc_1 = fac.CounterCondition(min=0, max=None, metadata=pyxb.utils.utility.Location('avm.modelica.xsd', 16, 10))
    counters.add(cc_1)
    cc_2 = fac.CounterCondition(min=0, max=None, metadata=pyxb.utils.utility.Location('avm.modelica.xsd', 17, 10))
    counters.add(cc_2)
    cc_3 = fac.CounterCondition(min=0, max=None, metadata=pyxb.utils.utility.Location('avm.modelica.xsd', 18, 10))
    counters.add(cc_3)
    cc_4 = fac.CounterCondition(min=0, max=None, metadata=pyxb.utils.utility.Location('avm.modelica.xsd', 19, 10))
    counters.add(cc_4)
    states = []
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_0, False))
    symbol = pyxb.binding.content.ElementUse(ModelicaModel_._UseForTag(pyxb.namespace.ExpandedName(None, 'Parameter')), pyxb.utils.utility.Location('avm.modelica.xsd', 15, 10))
    st_0 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_0)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_1, False))
    symbol = pyxb.binding.content.ElementUse(ModelicaModel_._UseForTag(pyxb.namespace.ExpandedName(None, 'Connector')), pyxb.utils.utility.Location('avm.modelica.xsd', 16, 10))
    st_1 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_1)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_2, False))
    symbol = pyxb.binding.content.ElementUse(ModelicaModel_._UseForTag(pyxb.namespace.ExpandedName(None, 'Metric')), pyxb.utils.utility.Location('avm.modelica.xsd', 17, 10))
    st_2 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_2)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_3, False))
    symbol = pyxb.binding.content.ElementUse(ModelicaModel_._UseForTag(pyxb.namespace.ExpandedName(None, 'Limit')), pyxb.utils.utility.Location('avm.modelica.xsd', 18, 10))
    st_3 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_3)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_4, False))
    symbol = pyxb.binding.content.ElementUse(ModelicaModel_._UseForTag(pyxb.namespace.ExpandedName(None, 'Redeclare')), pyxb.utils.utility.Location('avm.modelica.xsd', 19, 10))
    st_4 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_4)
    transitions = []
    transitions.append(fac.Transition(st_0, [
        fac.UpdateInstruction(cc_0, True) ]))
    transitions.append(fac.Transition(st_1, [
        fac.UpdateInstruction(cc_0, False) ]))
    transitions.append(fac.Transition(st_2, [
        fac.UpdateInstruction(cc_0, False) ]))
    transitions.append(fac.Transition(st_3, [
        fac.UpdateInstruction(cc_0, False) ]))
    transitions.append(fac.Transition(st_4, [
        fac.UpdateInstruction(cc_0, False) ]))
    st_0._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_1, [
        fac.UpdateInstruction(cc_1, True) ]))
    transitions.append(fac.Transition(st_2, [
        fac.UpdateInstruction(cc_1, False) ]))
    transitions.append(fac.Transition(st_3, [
        fac.UpdateInstruction(cc_1, False) ]))
    transitions.append(fac.Transition(st_4, [
        fac.UpdateInstruction(cc_1, False) ]))
    st_1._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_2, [
        fac.UpdateInstruction(cc_2, True) ]))
    transitions.append(fac.Transition(st_3, [
        fac.UpdateInstruction(cc_2, False) ]))
    transitions.append(fac.Transition(st_4, [
        fac.UpdateInstruction(cc_2, False) ]))
    st_2._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_3, [
        fac.UpdateInstruction(cc_3, True) ]))
    transitions.append(fac.Transition(st_4, [
        fac.UpdateInstruction(cc_3, False) ]))
    st_3._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_4, [
        fac.UpdateInstruction(cc_4, True) ]))
    st_4._set_transitionSet(transitions)
    return fac.Automaton(states, counters, True, containing_state=None)
ModelicaModel_._Automaton = _BuildAutomaton()




Parameter_._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'Value'), _ImportedBinding__avm.Value_, scope=Parameter_, location=pyxb.utils.utility.Location('avm.modelica.xsd', 41, 10)))

def _BuildAutomaton_ ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton_
    del _BuildAutomaton_
    import pyxb.utils.fac as fac

    counters = set()
    cc_0 = fac.CounterCondition(min=0, max=1, metadata=pyxb.utils.utility.Location('avm.modelica.xsd', 41, 10))
    counters.add(cc_0)
    states = []
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_0, False))
    symbol = pyxb.binding.content.ElementUse(Parameter_._UseForTag(pyxb.namespace.ExpandedName(None, 'Value')), pyxb.utils.utility.Location('avm.modelica.xsd', 41, 10))
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




Limit_._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'TargetValue'), _ImportedBinding__avm.Value_, scope=Limit_, location=pyxb.utils.utility.Location('avm.modelica.xsd', 56, 6)))

def _BuildAutomaton_3 ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton_3
    del _BuildAutomaton_3
    import pyxb.utils.fac as fac

    counters = set()
    states = []
    final_update = set()
    symbol = pyxb.binding.content.ElementUse(Limit_._UseForTag(pyxb.namespace.ExpandedName(None, 'TargetValue')), pyxb.utils.utility.Location('avm.modelica.xsd', 56, 6))
    st_0 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_0)
    transitions = []
    st_0._set_transitionSet(transitions)
    return fac.Automaton(states, counters, False, containing_state=None)
Limit_._Automaton = _BuildAutomaton_3()




Redeclare_._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'Value'), _ImportedBinding__avm.Value_, scope=Redeclare_, location=pyxb.utils.utility.Location('avm.modelica.xsd', 76, 10)))

def _BuildAutomaton_4 ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton_4
    del _BuildAutomaton_4
    import pyxb.utils.fac as fac

    counters = set()
    cc_0 = fac.CounterCondition(min=0, max=1, metadata=pyxb.utils.utility.Location('avm.modelica.xsd', 76, 10))
    counters.add(cc_0)
    states = []
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_0, False))
    symbol = pyxb.binding.content.ElementUse(Redeclare_._UseForTag(pyxb.namespace.ExpandedName(None, 'Value')), pyxb.utils.utility.Location('avm.modelica.xsd', 76, 10))
    st_0 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_0)
    transitions = []
    transitions.append(fac.Transition(st_0, [
        fac.UpdateInstruction(cc_0, True) ]))
    st_0._set_transitionSet(transitions)
    return fac.Automaton(states, counters, True, containing_state=None)
Redeclare_._Automaton = _BuildAutomaton_4()




Connector_._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'Parameter'), Parameter_, scope=Connector_, location=pyxb.utils.utility.Location('avm.modelica.xsd', 29, 10)))

Connector_._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'Redeclare'), Redeclare_, scope=Connector_, location=pyxb.utils.utility.Location('avm.modelica.xsd', 30, 10)))

def _BuildAutomaton_5 ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton_5
    del _BuildAutomaton_5
    import pyxb.utils.fac as fac

    counters = set()
    cc_0 = fac.CounterCondition(min=0, max=None, metadata=pyxb.utils.utility.Location('avm.modelica.xsd', 29, 10))
    counters.add(cc_0)
    cc_1 = fac.CounterCondition(min=0, max=None, metadata=pyxb.utils.utility.Location('avm.modelica.xsd', 30, 10))
    counters.add(cc_1)
    states = []
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_0, False))
    symbol = pyxb.binding.content.ElementUse(Connector_._UseForTag(pyxb.namespace.ExpandedName(None, 'Parameter')), pyxb.utils.utility.Location('avm.modelica.xsd', 29, 10))
    st_0 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_0)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_1, False))
    symbol = pyxb.binding.content.ElementUse(Connector_._UseForTag(pyxb.namespace.ExpandedName(None, 'Redeclare')), pyxb.utils.utility.Location('avm.modelica.xsd', 30, 10))
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
Connector_._Automaton = _BuildAutomaton_5()

