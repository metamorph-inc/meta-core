# .\_cad.py
# -*- coding: utf-8 -*-
# PyXB bindings for NM:462c18d5b89050fb1b7f8fca1e535af868009675
# Generated 2017-02-15 15:01:22.001000 by PyXB version 1.2.3
# Namespace cad [xmlns:cad]

import pyxb
import pyxb.binding
import pyxb.binding.saxer
import io
import pyxb.utils.utility
import pyxb.utils.domutils
import sys

# Unique identifier for bindings created at the same time
_GenerationUID = pyxb.utils.utility.UniqueIdentifier('urn:uuid:e7551791-f3c1-11e6-bc8f-2c6e8597a6b6')

# Version of PyXB used to generate the bindings
_PyXBVersion = '1.2.3'
# Generated bindings are not compatible across PyXB versions
if pyxb.__version__ != _PyXBVersion:
    raise pyxb.PyXBVersionError(_PyXBVersion)

# Import bindings for namespaces imported into schema
import avm as _ImportedBinding__avm
import pyxb.binding.datatypes

# NOTE: All namespace declarations are reserved within the binding
Namespace = pyxb.namespace.NamespaceForURI(u'cad', create_if_missing=True)
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
    _XSDLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 74, 10)
    _Documentation = None

    _ItemType = pyxb.binding.datatypes.anyURI
STD_ANON._InitializeFacetMap()

# Atomic simple type: {cad}CustomGeometryInputOperationEnum
class CustomGeometryInputOperationEnum (pyxb.binding.datatypes.string, pyxb.binding.basis.enumeration_mixin):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'CustomGeometryInputOperationEnum')
    _XSDLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 170, 2)
    _Documentation = None
CustomGeometryInputOperationEnum._CF_enumeration = pyxb.binding.facets.CF_enumeration(value_datatype=CustomGeometryInputOperationEnum, enum_prefix=None)
CustomGeometryInputOperationEnum.Union = CustomGeometryInputOperationEnum._CF_enumeration.addEnumeration(unicode_value=u'Union', tag=u'Union')
CustomGeometryInputOperationEnum.Intersection = CustomGeometryInputOperationEnum._CF_enumeration.addEnumeration(unicode_value=u'Intersection', tag=u'Intersection')
CustomGeometryInputOperationEnum.Subtraction = CustomGeometryInputOperationEnum._CF_enumeration.addEnumeration(unicode_value=u'Subtraction', tag=u'Subtraction')
CustomGeometryInputOperationEnum._InitializeFacetMap(CustomGeometryInputOperationEnum._CF_enumeration)
Namespace.addCategoryObject('typeBinding', u'CustomGeometryInputOperationEnum', CustomGeometryInputOperationEnum)

# List simple type: [anonymous]
# superclasses pyxb.binding.datatypes.anySimpleType
class STD_ANON_ (pyxb.binding.basis.STD_list):

    """Simple type that is a list of pyxb.binding.datatypes.anyURI."""

    _ExpandedName = None
    _XSDLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 179, 6)
    _Documentation = None

    _ItemType = pyxb.binding.datatypes.anyURI
STD_ANON_._InitializeFacetMap()

# Atomic simple type: {cad}PartIntersectionEnum
class PartIntersectionEnum (pyxb.binding.datatypes.string, pyxb.binding.basis.enumeration_mixin):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'PartIntersectionEnum')
    _XSDLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 184, 2)
    _Documentation = None
PartIntersectionEnum._CF_enumeration = pyxb.binding.facets.CF_enumeration(value_datatype=PartIntersectionEnum, enum_prefix=None)
PartIntersectionEnum.None_ = PartIntersectionEnum._CF_enumeration.addEnumeration(unicode_value=u'None', tag=u'None_')
PartIntersectionEnum.IntersectionWithAnyParts = PartIntersectionEnum._CF_enumeration.addEnumeration(unicode_value=u'IntersectionWithAnyParts', tag=u'IntersectionWithAnyParts')
PartIntersectionEnum.IntersectionWithReferencedParts = PartIntersectionEnum._CF_enumeration.addEnumeration(unicode_value=u'IntersectionWithReferencedParts', tag=u'IntersectionWithReferencedParts')
PartIntersectionEnum._InitializeFacetMap(PartIntersectionEnum._CF_enumeration)
Namespace.addCategoryObject('typeBinding', u'PartIntersectionEnum', PartIntersectionEnum)

# Atomic simple type: {cad}GeometryQualifierEnum
class GeometryQualifierEnum (pyxb.binding.datatypes.string, pyxb.binding.basis.enumeration_mixin):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'GeometryQualifierEnum')
    _XSDLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 191, 2)
    _Documentation = None
GeometryQualifierEnum._CF_enumeration = pyxb.binding.facets.CF_enumeration(value_datatype=GeometryQualifierEnum, enum_prefix=None)
GeometryQualifierEnum.InteriorAndBoundary = GeometryQualifierEnum._CF_enumeration.addEnumeration(unicode_value=u'InteriorAndBoundary', tag=u'InteriorAndBoundary')
GeometryQualifierEnum.InteriorOnly = GeometryQualifierEnum._CF_enumeration.addEnumeration(unicode_value=u'InteriorOnly', tag=u'InteriorOnly')
GeometryQualifierEnum.BoundaryOnly = GeometryQualifierEnum._CF_enumeration.addEnumeration(unicode_value=u'BoundaryOnly', tag=u'BoundaryOnly')
GeometryQualifierEnum._InitializeFacetMap(GeometryQualifierEnum._CF_enumeration)
Namespace.addCategoryObject('typeBinding', u'GeometryQualifierEnum', GeometryQualifierEnum)

# List simple type: [anonymous]
# superclasses pyxb.binding.datatypes.anySimpleType
class STD_ANON_2 (pyxb.binding.basis.STD_list):

    """Simple type that is a list of pyxb.binding.datatypes.anyURI."""

    _ExpandedName = None
    _XSDLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 209, 6)
    _Documentation = None

    _ItemType = pyxb.binding.datatypes.anyURI
STD_ANON_2._InitializeFacetMap()

# Complex type {cad}CADModel with content type ELEMENT_ONLY
class CADModel_ (_ImportedBinding__avm.DomainModel_):
    """Complex type {cad}CADModel with content type ELEMENT_ONLY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'CADModel')
    _XSDLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 29, 2)
    _ElementMap = _ImportedBinding__avm.DomainModel_._ElementMap.copy()
    _AttributeMap = _ImportedBinding__avm.DomainModel_._AttributeMap.copy()
    # Base type is _ImportedBinding__avm.DomainModel_
    
    # Element Datum uses Python identifier Datum
    __Datum = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'Datum'), 'Datum', '__cad_CADModel__Datum', True, pyxb.utils.utility.Location(u'avm.cad.xsd', 33, 10), )

    
    Datum = property(__Datum.value, __Datum.set, None, None)

    
    # Element Parameter uses Python identifier Parameter
    __Parameter = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'Parameter'), 'Parameter', '__cad_CADModel__Parameter', True, pyxb.utils.utility.Location(u'avm.cad.xsd', 34, 10), )

    
    Parameter = property(__Parameter.value, __Parameter.set, None, None)

    
    # Element ModelMetric uses Python identifier ModelMetric
    __ModelMetric = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'ModelMetric'), 'ModelMetric', '__cad_CADModel__ModelMetric', True, pyxb.utils.utility.Location(u'avm.cad.xsd', 35, 10), )

    
    ModelMetric = property(__ModelMetric.value, __ModelMetric.set, None, None)

    
    # Attribute UsesResource inherited from {avm}DomainModel
    
    # Attribute Author inherited from {avm}DomainModel
    
    # Attribute Notes inherited from {avm}DomainModel
    
    # Attribute XPosition inherited from {avm}DomainModel
    
    # Attribute YPosition inherited from {avm}DomainModel
    
    # Attribute Name inherited from {avm}DomainModel
    _ElementMap.update({
        __Datum.name() : __Datum,
        __Parameter.name() : __Parameter,
        __ModelMetric.name() : __ModelMetric
    })
    _AttributeMap.update({
        
    })
Namespace.addCategoryObject('typeBinding', u'CADModel', CADModel_)


# Complex type {cad}Parameter with content type ELEMENT_ONLY
class Parameter_ (_ImportedBinding__avm.DomainModelParameter_):
    """Complex type {cad}Parameter with content type ELEMENT_ONLY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'Parameter')
    _XSDLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 50, 2)
    _ElementMap = _ImportedBinding__avm.DomainModelParameter_._ElementMap.copy()
    _AttributeMap = _ImportedBinding__avm.DomainModelParameter_._AttributeMap.copy()
    # Base type is _ImportedBinding__avm.DomainModelParameter_
    
    # Element Value uses Python identifier Value
    __Value = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'Value'), 'Value', '__cad_Parameter__Value', False, pyxb.utils.utility.Location(u'avm.cad.xsd', 54, 10), )

    
    Value = property(__Value.value, __Value.set, None, None)

    
    # Attribute Notes inherited from {avm}DomainModelParameter
    
    # Attribute XPosition inherited from {avm}DomainModelParameter
    
    # Attribute YPosition inherited from {avm}DomainModelParameter
    
    # Attribute Name uses Python identifier Name
    __Name = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'Name'), 'Name', '__cad_Parameter__Name', pyxb.binding.datatypes.string, required=True)
    __Name._DeclarationLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 56, 8)
    __Name._UseLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 56, 8)
    
    Name = property(__Name.value, __Name.set, None, None)

    _ElementMap.update({
        __Value.name() : __Value
    })
    _AttributeMap.update({
        __Name.name() : __Name
    })
Namespace.addCategoryObject('typeBinding', u'Parameter', Parameter_)


# Complex type {cad}Metric with content type ELEMENT_ONLY
class Metric_ (_ImportedBinding__avm.DomainModelMetric_):
    """Complex type {cad}Metric with content type ELEMENT_ONLY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'Metric')
    _XSDLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 90, 2)
    _ElementMap = _ImportedBinding__avm.DomainModelMetric_._ElementMap.copy()
    _AttributeMap = _ImportedBinding__avm.DomainModelMetric_._AttributeMap.copy()
    # Base type is _ImportedBinding__avm.DomainModelMetric_
    
    # Element Value (Value) inherited from {avm}DomainModelMetric
    
    # Attribute ID inherited from {avm}DomainModelMetric
    
    # Attribute Notes inherited from {avm}DomainModelMetric
    
    # Attribute XPosition inherited from {avm}DomainModelMetric
    
    # Attribute YPosition inherited from {avm}DomainModelMetric
    
    # Attribute Name uses Python identifier Name
    __Name = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'Name'), 'Name', '__cad_Metric__Name', pyxb.binding.datatypes.string, required=True)
    __Name._DeclarationLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 93, 8)
    __Name._UseLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 93, 8)
    
    Name = property(__Name.value, __Name.set, None, None)

    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __Name.name() : __Name
    })
Namespace.addCategoryObject('typeBinding', u'Metric', Metric_)


# Complex type {cad}Geometry with content type EMPTY
class Geometry_ (_ImportedBinding__avm.AnalysisConstruct_):
    """Complex type {cad}Geometry with content type EMPTY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_EMPTY
    _Abstract = True
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'Geometry')
    _XSDLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 97, 2)
    _ElementMap = _ImportedBinding__avm.AnalysisConstruct_._ElementMap.copy()
    _AttributeMap = _ImportedBinding__avm.AnalysisConstruct_._AttributeMap.copy()
    # Base type is _ImportedBinding__avm.AnalysisConstruct_
    
    # Attribute GeometryQualifier uses Python identifier GeometryQualifier
    __GeometryQualifier = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'GeometryQualifier'), 'GeometryQualifier', '__cad_Geometry__GeometryQualifier', GeometryQualifierEnum)
    __GeometryQualifier._DeclarationLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 100, 8)
    __GeometryQualifier._UseLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 100, 8)
    
    GeometryQualifier = property(__GeometryQualifier.value, __GeometryQualifier.set, None, None)

    
    # Attribute PartIntersectionModifier uses Python identifier PartIntersectionModifier
    __PartIntersectionModifier = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'PartIntersectionModifier'), 'PartIntersectionModifier', '__cad_Geometry__PartIntersectionModifier', PartIntersectionEnum)
    __PartIntersectionModifier._DeclarationLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 101, 8)
    __PartIntersectionModifier._UseLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 101, 8)
    
    PartIntersectionModifier = property(__PartIntersectionModifier.value, __PartIntersectionModifier.set, None, None)

    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __GeometryQualifier.name() : __GeometryQualifier,
        __PartIntersectionModifier.name() : __PartIntersectionModifier
    })
Namespace.addCategoryObject('typeBinding', u'Geometry', Geometry_)


# Complex type {cad}CustomGeometryInput with content type ELEMENT_ONLY
class CustomGeometryInput_ (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {cad}CustomGeometryInput with content type ELEMENT_ONLY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'CustomGeometryInput')
    _XSDLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 164, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.anyType
    
    # Element InputGeometry uses Python identifier InputGeometry
    __InputGeometry = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'InputGeometry'), 'InputGeometry', '__cad_CustomGeometryInput__InputGeometry', False, pyxb.utils.utility.Location(u'avm.cad.xsd', 166, 6), )

    
    InputGeometry = property(__InputGeometry.value, __InputGeometry.set, None, None)

    
    # Attribute Operation uses Python identifier Operation
    __Operation = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'Operation'), 'Operation', '__cad_CustomGeometryInput__Operation', CustomGeometryInputOperationEnum, required=True)
    __Operation._DeclarationLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 168, 4)
    __Operation._UseLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 168, 4)
    
    Operation = property(__Operation.value, __Operation.set, None, None)

    _ElementMap.update({
        __InputGeometry.name() : __InputGeometry
    })
    _AttributeMap.update({
        __Operation.name() : __Operation
    })
Namespace.addCategoryObject('typeBinding', u'CustomGeometryInput', CustomGeometryInput_)


# Complex type {cad}PointReference with content type EMPTY
class PointReference_ (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {cad}PointReference with content type EMPTY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_EMPTY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'PointReference')
    _XSDLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 177, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.anyType
    
    # Attribute ReferredPoint uses Python identifier ReferredPoint
    __ReferredPoint = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'ReferredPoint'), 'ReferredPoint', '__cad_PointReference__ReferredPoint', STD_ANON_)
    __ReferredPoint._DeclarationLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 178, 4)
    __ReferredPoint._UseLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 178, 4)
    
    ReferredPoint = property(__ReferredPoint.value, __ReferredPoint.set, None, None)

    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __ReferredPoint.name() : __ReferredPoint
    })
Namespace.addCategoryObject('typeBinding', u'PointReference', PointReference_)


# Complex type {cad}PlaneReference with content type EMPTY
class PlaneReference_ (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {cad}PlaneReference with content type EMPTY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_EMPTY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'PlaneReference')
    _XSDLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 207, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.anyType
    
    # Attribute ReferredPlane uses Python identifier ReferredPlane
    __ReferredPlane = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'ReferredPlane'), 'ReferredPlane', '__cad_PlaneReference__ReferredPlane', STD_ANON_2)
    __ReferredPlane._DeclarationLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 208, 4)
    __ReferredPlane._UseLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 208, 4)
    
    ReferredPlane = property(__ReferredPlane.value, __ReferredPlane.set, None, None)

    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __ReferredPlane.name() : __ReferredPlane
    })
Namespace.addCategoryObject('typeBinding', u'PlaneReference', PlaneReference_)


# Complex type {cad}GuideDatum with content type EMPTY
class GuideDatum_ (_ImportedBinding__avm.ConnectorFeature_):
    """Complex type {cad}GuideDatum with content type EMPTY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_EMPTY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'GuideDatum')
    _XSDLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 214, 2)
    _ElementMap = _ImportedBinding__avm.ConnectorFeature_._ElementMap.copy()
    _AttributeMap = _ImportedBinding__avm.ConnectorFeature_._AttributeMap.copy()
    # Base type is _ImportedBinding__avm.ConnectorFeature_
    
    # Attribute Datum uses Python identifier Datum
    __Datum = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'Datum'), 'Datum', '__cad_GuideDatum__Datum', pyxb.binding.datatypes.anyURI, required=True)
    __Datum._DeclarationLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 217, 8)
    __Datum._UseLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 217, 8)
    
    Datum = property(__Datum.value, __Datum.set, None, None)

    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __Datum.name() : __Datum
    })
Namespace.addCategoryObject('typeBinding', u'GuideDatum', GuideDatum_)


# Complex type {cad}AssemblyRoot with content type EMPTY
class AssemblyRoot_ (_ImportedBinding__avm.DesignDomainFeature_):
    """Complex type {cad}AssemblyRoot with content type EMPTY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_EMPTY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'AssemblyRoot')
    _XSDLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 221, 2)
    _ElementMap = _ImportedBinding__avm.DesignDomainFeature_._ElementMap.copy()
    _AttributeMap = _ImportedBinding__avm.DesignDomainFeature_._AttributeMap.copy()
    # Base type is _ImportedBinding__avm.DesignDomainFeature_
    
    # Attribute AssemblyRootComponentInstance uses Python identifier AssemblyRootComponentInstance
    __AssemblyRootComponentInstance = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'AssemblyRootComponentInstance'), 'AssemblyRootComponentInstance', '__cad_AssemblyRoot__AssemblyRootComponentInstance', pyxb.binding.datatypes.anyURI, required=True)
    __AssemblyRootComponentInstance._DeclarationLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 224, 8)
    __AssemblyRootComponentInstance._UseLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 224, 8)
    
    AssemblyRootComponentInstance = property(__AssemblyRootComponentInstance.value, __AssemblyRootComponentInstance.set, None, None)

    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __AssemblyRootComponentInstance.name() : __AssemblyRootComponentInstance
    })
Namespace.addCategoryObject('typeBinding', u'AssemblyRoot', AssemblyRoot_)


# Complex type {cad}KinematicJointSpec with content type EMPTY
class KinematicJointSpec_ (_ImportedBinding__avm.ConnectorFeature_):
    """Complex type {cad}KinematicJointSpec with content type EMPTY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_EMPTY
    _Abstract = True
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'KinematicJointSpec')
    _XSDLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 228, 2)
    _ElementMap = _ImportedBinding__avm.ConnectorFeature_._ElementMap.copy()
    _AttributeMap = _ImportedBinding__avm.ConnectorFeature_._AttributeMap.copy()
    # Base type is _ImportedBinding__avm.ConnectorFeature_
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        
    })
Namespace.addCategoryObject('typeBinding', u'KinematicJointSpec', KinematicJointSpec_)


# Complex type {cad}Geometry2D with content type EMPTY
class Geometry2D_ (Geometry_):
    """Complex type {cad}Geometry2D with content type EMPTY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_EMPTY
    _Abstract = True
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'Geometry2D')
    _XSDLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 105, 2)
    _ElementMap = Geometry_._ElementMap.copy()
    _AttributeMap = Geometry_._AttributeMap.copy()
    # Base type is Geometry_
    
    # Attribute GeometryQualifier inherited from {cad}Geometry
    
    # Attribute PartIntersectionModifier inherited from {cad}Geometry
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        
    })
Namespace.addCategoryObject('typeBinding', u'Geometry2D', Geometry2D_)


# Complex type {cad}Geometry3D with content type EMPTY
class Geometry3D_ (Geometry_):
    """Complex type {cad}Geometry3D with content type EMPTY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_EMPTY
    _Abstract = True
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'Geometry3D')
    _XSDLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 110, 2)
    _ElementMap = Geometry_._ElementMap.copy()
    _AttributeMap = Geometry_._AttributeMap.copy()
    # Base type is Geometry_
    
    # Attribute GeometryQualifier inherited from {cad}Geometry
    
    # Attribute PartIntersectionModifier inherited from {cad}Geometry
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        
    })
Namespace.addCategoryObject('typeBinding', u'Geometry3D', Geometry3D_)


# Complex type {cad}CustomGeometry with content type ELEMENT_ONLY
class CustomGeometry_ (Geometry_):
    """Complex type {cad}CustomGeometry with content type ELEMENT_ONLY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'CustomGeometry')
    _XSDLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 155, 2)
    _ElementMap = Geometry_._ElementMap.copy()
    _AttributeMap = Geometry_._AttributeMap.copy()
    # Base type is Geometry_
    
    # Element CustomGeometryInput uses Python identifier CustomGeometryInput
    __CustomGeometryInput = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'CustomGeometryInput'), 'CustomGeometryInput', '__cad_CustomGeometry__CustomGeometryInput', True, pyxb.utils.utility.Location(u'avm.cad.xsd', 159, 10), )

    
    CustomGeometryInput = property(__CustomGeometryInput.value, __CustomGeometryInput.set, None, None)

    
    # Attribute GeometryQualifier inherited from {cad}Geometry
    
    # Attribute PartIntersectionModifier inherited from {cad}Geometry
    _ElementMap.update({
        __CustomGeometryInput.name() : __CustomGeometryInput
    })
    _AttributeMap.update({
        
    })
Namespace.addCategoryObject('typeBinding', u'CustomGeometry', CustomGeometry_)


# Complex type {cad}RevoluteJointSpec with content type ELEMENT_ONLY
class RevoluteJointSpec_ (KinematicJointSpec_):
    """Complex type {cad}RevoluteJointSpec with content type ELEMENT_ONLY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'RevoluteJointSpec')
    _XSDLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 233, 2)
    _ElementMap = KinematicJointSpec_._ElementMap.copy()
    _AttributeMap = KinematicJointSpec_._AttributeMap.copy()
    # Base type is KinematicJointSpec_
    
    # Element MinimumRotation uses Python identifier MinimumRotation
    __MinimumRotation = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'MinimumRotation'), 'MinimumRotation', '__cad_RevoluteJointSpec__MinimumRotation', False, pyxb.utils.utility.Location(u'avm.cad.xsd', 237, 10), )

    
    MinimumRotation = property(__MinimumRotation.value, __MinimumRotation.set, None, None)

    
    # Element DefaultRotation uses Python identifier DefaultRotation
    __DefaultRotation = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'DefaultRotation'), 'DefaultRotation', '__cad_RevoluteJointSpec__DefaultRotation', False, pyxb.utils.utility.Location(u'avm.cad.xsd', 238, 10), )

    
    DefaultRotation = property(__DefaultRotation.value, __DefaultRotation.set, None, None)

    
    # Element MaximumRotation uses Python identifier MaximumRotation
    __MaximumRotation = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'MaximumRotation'), 'MaximumRotation', '__cad_RevoluteJointSpec__MaximumRotation', False, pyxb.utils.utility.Location(u'avm.cad.xsd', 239, 10), )

    
    MaximumRotation = property(__MaximumRotation.value, __MaximumRotation.set, None, None)

    
    # Attribute AlignmentPlane uses Python identifier AlignmentPlane
    __AlignmentPlane = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'AlignmentPlane'), 'AlignmentPlane', '__cad_RevoluteJointSpec__AlignmentPlane', pyxb.binding.datatypes.anyURI, required=True)
    __AlignmentPlane._DeclarationLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 241, 8)
    __AlignmentPlane._UseLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 241, 8)
    
    AlignmentPlane = property(__AlignmentPlane.value, __AlignmentPlane.set, None, None)

    
    # Attribute AlignmentAxis uses Python identifier AlignmentAxis
    __AlignmentAxis = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'AlignmentAxis'), 'AlignmentAxis', '__cad_RevoluteJointSpec__AlignmentAxis', pyxb.binding.datatypes.anyURI, required=True)
    __AlignmentAxis._DeclarationLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 242, 8)
    __AlignmentAxis._UseLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 242, 8)
    
    AlignmentAxis = property(__AlignmentAxis.value, __AlignmentAxis.set, None, None)

    
    # Attribute RotationLimitReference uses Python identifier RotationLimitReference
    __RotationLimitReference = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'RotationLimitReference'), 'RotationLimitReference', '__cad_RevoluteJointSpec__RotationLimitReference', pyxb.binding.datatypes.anyURI)
    __RotationLimitReference._DeclarationLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 243, 8)
    __RotationLimitReference._UseLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 243, 8)
    
    RotationLimitReference = property(__RotationLimitReference.value, __RotationLimitReference.set, None, None)

    _ElementMap.update({
        __MinimumRotation.name() : __MinimumRotation,
        __DefaultRotation.name() : __DefaultRotation,
        __MaximumRotation.name() : __MaximumRotation
    })
    _AttributeMap.update({
        __AlignmentPlane.name() : __AlignmentPlane,
        __AlignmentAxis.name() : __AlignmentAxis,
        __RotationLimitReference.name() : __RotationLimitReference
    })
Namespace.addCategoryObject('typeBinding', u'RevoluteJointSpec', RevoluteJointSpec_)


# Complex type {cad}TranslationalJointSpec with content type ELEMENT_ONLY
class TranslationalJointSpec_ (KinematicJointSpec_):
    """Complex type {cad}TranslationalJointSpec with content type ELEMENT_ONLY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'TranslationalJointSpec')
    _XSDLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 247, 2)
    _ElementMap = KinematicJointSpec_._ElementMap.copy()
    _AttributeMap = KinematicJointSpec_._AttributeMap.copy()
    # Base type is KinematicJointSpec_
    
    # Element MinimumTranslation uses Python identifier MinimumTranslation
    __MinimumTranslation = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'MinimumTranslation'), 'MinimumTranslation', '__cad_TranslationalJointSpec__MinimumTranslation', False, pyxb.utils.utility.Location(u'avm.cad.xsd', 251, 10), )

    
    MinimumTranslation = property(__MinimumTranslation.value, __MinimumTranslation.set, None, None)

    
    # Element DefaultTranslation uses Python identifier DefaultTranslation
    __DefaultTranslation = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'DefaultTranslation'), 'DefaultTranslation', '__cad_TranslationalJointSpec__DefaultTranslation', False, pyxb.utils.utility.Location(u'avm.cad.xsd', 252, 10), )

    
    DefaultTranslation = property(__DefaultTranslation.value, __DefaultTranslation.set, None, None)

    
    # Element MaximumTranslation uses Python identifier MaximumTranslation
    __MaximumTranslation = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'MaximumTranslation'), 'MaximumTranslation', '__cad_TranslationalJointSpec__MaximumTranslation', False, pyxb.utils.utility.Location(u'avm.cad.xsd', 253, 10), )

    
    MaximumTranslation = property(__MaximumTranslation.value, __MaximumTranslation.set, None, None)

    
    # Attribute AlignmentPlane uses Python identifier AlignmentPlane
    __AlignmentPlane = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'AlignmentPlane'), 'AlignmentPlane', '__cad_TranslationalJointSpec__AlignmentPlane', pyxb.binding.datatypes.anyURI, required=True)
    __AlignmentPlane._DeclarationLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 255, 8)
    __AlignmentPlane._UseLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 255, 8)
    
    AlignmentPlane = property(__AlignmentPlane.value, __AlignmentPlane.set, None, None)

    
    # Attribute AlignmentAxis uses Python identifier AlignmentAxis
    __AlignmentAxis = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'AlignmentAxis'), 'AlignmentAxis', '__cad_TranslationalJointSpec__AlignmentAxis', pyxb.binding.datatypes.anyURI, required=True)
    __AlignmentAxis._DeclarationLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 256, 8)
    __AlignmentAxis._UseLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 256, 8)
    
    AlignmentAxis = property(__AlignmentAxis.value, __AlignmentAxis.set, None, None)

    
    # Attribute TranslationLimitReference uses Python identifier TranslationLimitReference
    __TranslationLimitReference = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'TranslationLimitReference'), 'TranslationLimitReference', '__cad_TranslationalJointSpec__TranslationLimitReference', pyxb.binding.datatypes.anyURI)
    __TranslationLimitReference._DeclarationLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 257, 8)
    __TranslationLimitReference._UseLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 257, 8)
    
    TranslationLimitReference = property(__TranslationLimitReference.value, __TranslationLimitReference.set, None, None)

    _ElementMap.update({
        __MinimumTranslation.name() : __MinimumTranslation,
        __DefaultTranslation.name() : __DefaultTranslation,
        __MaximumTranslation.name() : __MaximumTranslation
    })
    _AttributeMap.update({
        __AlignmentPlane.name() : __AlignmentPlane,
        __AlignmentAxis.name() : __AlignmentAxis,
        __TranslationLimitReference.name() : __TranslationLimitReference
    })
Namespace.addCategoryObject('typeBinding', u'TranslationalJointSpec', TranslationalJointSpec_)


# Complex type {cad}Circle with content type ELEMENT_ONLY
class Circle_ (Geometry2D_):
    """Complex type {cad}Circle with content type ELEMENT_ONLY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'Circle')
    _XSDLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 115, 2)
    _ElementMap = Geometry2D_._ElementMap.copy()
    _AttributeMap = Geometry2D_._AttributeMap.copy()
    # Base type is Geometry2D_
    
    # Element CircleCenter uses Python identifier CircleCenter
    __CircleCenter = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'CircleCenter'), 'CircleCenter', '__cad_Circle__CircleCenter', False, pyxb.utils.utility.Location(u'avm.cad.xsd', 119, 10), )

    
    CircleCenter = property(__CircleCenter.value, __CircleCenter.set, None, None)

    
    # Element CircleEdge uses Python identifier CircleEdge
    __CircleEdge = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'CircleEdge'), 'CircleEdge', '__cad_Circle__CircleEdge', True, pyxb.utils.utility.Location(u'avm.cad.xsd', 120, 10), )

    
    CircleEdge = property(__CircleEdge.value, __CircleEdge.set, None, None)

    
    # Attribute GeometryQualifier inherited from {cad}Geometry
    
    # Attribute PartIntersectionModifier inherited from {cad}Geometry
    _ElementMap.update({
        __CircleCenter.name() : __CircleCenter,
        __CircleEdge.name() : __CircleEdge
    })
    _AttributeMap.update({
        
    })
Namespace.addCategoryObject('typeBinding', u'Circle', Circle_)


# Complex type {cad}Polygon with content type ELEMENT_ONLY
class Polygon_ (Geometry2D_):
    """Complex type {cad}Polygon with content type ELEMENT_ONLY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'Polygon')
    _XSDLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 125, 2)
    _ElementMap = Geometry2D_._ElementMap.copy()
    _AttributeMap = Geometry2D_._AttributeMap.copy()
    # Base type is Geometry2D_
    
    # Element PolygonPoint uses Python identifier PolygonPoint
    __PolygonPoint = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'PolygonPoint'), 'PolygonPoint', '__cad_Polygon__PolygonPoint', True, pyxb.utils.utility.Location(u'avm.cad.xsd', 129, 10), )

    
    PolygonPoint = property(__PolygonPoint.value, __PolygonPoint.set, None, None)

    
    # Attribute GeometryQualifier inherited from {cad}Geometry
    
    # Attribute PartIntersectionModifier inherited from {cad}Geometry
    _ElementMap.update({
        __PolygonPoint.name() : __PolygonPoint
    })
    _AttributeMap.update({
        
    })
Namespace.addCategoryObject('typeBinding', u'Polygon', Polygon_)


# Complex type {cad}ExtrudedGeometry with content type ELEMENT_ONLY
class ExtrudedGeometry_ (Geometry3D_):
    """Complex type {cad}ExtrudedGeometry with content type ELEMENT_ONLY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'ExtrudedGeometry')
    _XSDLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 134, 2)
    _ElementMap = Geometry3D_._ElementMap.copy()
    _AttributeMap = Geometry3D_._AttributeMap.copy()
    # Base type is Geometry3D_
    
    # Element ExtrusionHeight uses Python identifier ExtrusionHeight
    __ExtrusionHeight = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'ExtrusionHeight'), 'ExtrusionHeight', '__cad_ExtrudedGeometry__ExtrusionHeight', False, pyxb.utils.utility.Location(u'avm.cad.xsd', 138, 10), )

    
    ExtrusionHeight = property(__ExtrusionHeight.value, __ExtrusionHeight.set, None, None)

    
    # Element ExtrusionSurface uses Python identifier ExtrusionSurface
    __ExtrusionSurface = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'ExtrusionSurface'), 'ExtrusionSurface', '__cad_ExtrudedGeometry__ExtrusionSurface', False, pyxb.utils.utility.Location(u'avm.cad.xsd', 139, 10), )

    
    ExtrusionSurface = property(__ExtrusionSurface.value, __ExtrusionSurface.set, None, None)

    
    # Element DirectionReferencePoint uses Python identifier DirectionReferencePoint
    __DirectionReferencePoint = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'DirectionReferencePoint'), 'DirectionReferencePoint', '__cad_ExtrudedGeometry__DirectionReferencePoint', False, pyxb.utils.utility.Location(u'avm.cad.xsd', 140, 10), )

    
    DirectionReferencePoint = property(__DirectionReferencePoint.value, __DirectionReferencePoint.set, None, None)

    
    # Attribute GeometryQualifier inherited from {cad}Geometry
    
    # Attribute PartIntersectionModifier inherited from {cad}Geometry
    _ElementMap.update({
        __ExtrusionHeight.name() : __ExtrusionHeight,
        __ExtrusionSurface.name() : __ExtrusionSurface,
        __DirectionReferencePoint.name() : __DirectionReferencePoint
    })
    _AttributeMap.update({
        
    })
Namespace.addCategoryObject('typeBinding', u'ExtrudedGeometry', ExtrudedGeometry_)


# Complex type {cad}Sphere with content type ELEMENT_ONLY
class Sphere_ (Geometry3D_):
    """Complex type {cad}Sphere with content type ELEMENT_ONLY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'Sphere')
    _XSDLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 145, 2)
    _ElementMap = Geometry3D_._ElementMap.copy()
    _AttributeMap = Geometry3D_._AttributeMap.copy()
    # Base type is Geometry3D_
    
    # Element SphereCenter uses Python identifier SphereCenter
    __SphereCenter = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'SphereCenter'), 'SphereCenter', '__cad_Sphere__SphereCenter', False, pyxb.utils.utility.Location(u'avm.cad.xsd', 149, 10), )

    
    SphereCenter = property(__SphereCenter.value, __SphereCenter.set, None, None)

    
    # Element SphereEdge uses Python identifier SphereEdge
    __SphereEdge = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'SphereEdge'), 'SphereEdge', '__cad_Sphere__SphereEdge', False, pyxb.utils.utility.Location(u'avm.cad.xsd', 150, 10), )

    
    SphereEdge = property(__SphereEdge.value, __SphereEdge.set, None, None)

    
    # Attribute GeometryQualifier inherited from {cad}Geometry
    
    # Attribute PartIntersectionModifier inherited from {cad}Geometry
    _ElementMap.update({
        __SphereCenter.name() : __SphereCenter,
        __SphereEdge.name() : __SphereEdge
    })
    _AttributeMap.update({
        
    })
Namespace.addCategoryObject('typeBinding', u'Sphere', Sphere_)


# Complex type {cad}Surface with content type ELEMENT_ONLY
class Surface_ (Geometry3D_):
    """Complex type {cad}Surface with content type ELEMENT_ONLY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'Surface')
    _XSDLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 198, 2)
    _ElementMap = Geometry3D_._ElementMap.copy()
    _AttributeMap = Geometry3D_._AttributeMap.copy()
    # Base type is Geometry3D_
    
    # Element ReferencePlane uses Python identifier ReferencePlane
    __ReferencePlane = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'ReferencePlane'), 'ReferencePlane', '__cad_Surface__ReferencePlane', False, pyxb.utils.utility.Location(u'avm.cad.xsd', 202, 10), )

    
    ReferencePlane = property(__ReferencePlane.value, __ReferencePlane.set, None, None)

    
    # Attribute GeometryQualifier inherited from {cad}Geometry
    
    # Attribute PartIntersectionModifier inherited from {cad}Geometry
    _ElementMap.update({
        __ReferencePlane.name() : __ReferencePlane
    })
    _AttributeMap.update({
        
    })
Namespace.addCategoryObject('typeBinding', u'Surface', Surface_)


# Complex type {cad}Datum with content type ELEMENT_ONLY
class Datum_ (_ImportedBinding__avm.DomainModelPort_):
    """Complex type {cad}Datum with content type ELEMENT_ONLY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = True
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'Datum')
    _XSDLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 40, 2)
    _ElementMap = _ImportedBinding__avm.DomainModelPort_._ElementMap.copy()
    _AttributeMap = _ImportedBinding__avm.DomainModelPort_._AttributeMap.copy()
    # Base type is _ImportedBinding__avm.DomainModelPort_
    
    # Element DatumMetric uses Python identifier DatumMetric
    __DatumMetric = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'DatumMetric'), 'DatumMetric', '__cad_Datum__DatumMetric', True, pyxb.utils.utility.Location(u'avm.cad.xsd', 44, 10), )

    
    DatumMetric = property(__DatumMetric.value, __DatumMetric.set, None, None)

    
    # Attribute Notes inherited from {avm}Port
    
    # Attribute XPosition inherited from {avm}Port
    
    # Attribute Definition inherited from {avm}Port
    
    # Attribute YPosition inherited from {avm}Port
    
    # Attribute Name inherited from {avm}Port
    
    # Attribute ID inherited from {avm}PortMapTarget
    
    # Attribute PortMap inherited from {avm}PortMapTarget
    
    # Attribute DatumName uses Python identifier DatumName
    __DatumName = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'DatumName'), 'DatumName', '__cad_Datum__DatumName', pyxb.binding.datatypes.string)
    __DatumName._DeclarationLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 46, 8)
    __DatumName._UseLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 46, 8)
    
    DatumName = property(__DatumName.value, __DatumName.set, None, None)

    _ElementMap.update({
        __DatumMetric.name() : __DatumMetric
    })
    _AttributeMap.update({
        __DatumName.name() : __DatumName
    })
Namespace.addCategoryObject('typeBinding', u'Datum', Datum_)


# Complex type {cad}Point with content type ELEMENT_ONLY
class Point_ (Datum_):
    """Complex type {cad}Point with content type ELEMENT_ONLY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'Point')
    _XSDLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 60, 2)
    _ElementMap = Datum_._ElementMap.copy()
    _AttributeMap = Datum_._AttributeMap.copy()
    # Base type is Datum_
    
    # Element DatumMetric (DatumMetric) inherited from {cad}Datum
    
    # Attribute Notes inherited from {avm}Port
    
    # Attribute XPosition inherited from {avm}Port
    
    # Attribute Definition inherited from {avm}Port
    
    # Attribute YPosition inherited from {avm}Port
    
    # Attribute Name inherited from {avm}Port
    
    # Attribute ID inherited from {avm}PortMapTarget
    
    # Attribute PortMap inherited from {avm}PortMapTarget
    
    # Attribute DatumName inherited from {cad}Datum
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        
    })
Namespace.addCategoryObject('typeBinding', u'Point', Point_)


# Complex type {cad}Axis with content type ELEMENT_ONLY
class Axis_ (Datum_):
    """Complex type {cad}Axis with content type ELEMENT_ONLY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'Axis')
    _XSDLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 65, 2)
    _ElementMap = Datum_._ElementMap.copy()
    _AttributeMap = Datum_._AttributeMap.copy()
    # Base type is Datum_
    
    # Element DatumMetric (DatumMetric) inherited from {cad}Datum
    
    # Attribute Notes inherited from {avm}Port
    
    # Attribute XPosition inherited from {avm}Port
    
    # Attribute Definition inherited from {avm}Port
    
    # Attribute YPosition inherited from {avm}Port
    
    # Attribute Name inherited from {avm}Port
    
    # Attribute ID inherited from {avm}PortMapTarget
    
    # Attribute PortMap inherited from {avm}PortMapTarget
    
    # Attribute DatumName inherited from {cad}Datum
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        
    })
Namespace.addCategoryObject('typeBinding', u'Axis', Axis_)


# Complex type {cad}Plane with content type ELEMENT_ONLY
class Plane_ (Datum_):
    """Complex type {cad}Plane with content type ELEMENT_ONLY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'Plane')
    _XSDLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 70, 2)
    _ElementMap = Datum_._ElementMap.copy()
    _AttributeMap = Datum_._AttributeMap.copy()
    # Base type is Datum_
    
    # Element DatumMetric (DatumMetric) inherited from {cad}Datum
    
    # Attribute Notes inherited from {avm}Port
    
    # Attribute XPosition inherited from {avm}Port
    
    # Attribute Definition inherited from {avm}Port
    
    # Attribute YPosition inherited from {avm}Port
    
    # Attribute Name inherited from {avm}Port
    
    # Attribute ID inherited from {avm}PortMapTarget
    
    # Attribute PortMap inherited from {avm}PortMapTarget
    
    # Attribute DatumName inherited from {cad}Datum
    
    # Attribute SurfaceReverseMap uses Python identifier SurfaceReverseMap
    __SurfaceReverseMap = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'SurfaceReverseMap'), 'SurfaceReverseMap', '__cad_Plane__SurfaceReverseMap', STD_ANON)
    __SurfaceReverseMap._DeclarationLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 73, 8)
    __SurfaceReverseMap._UseLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 73, 8)
    
    SurfaceReverseMap = property(__SurfaceReverseMap.value, __SurfaceReverseMap.set, None, None)

    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __SurfaceReverseMap.name() : __SurfaceReverseMap
    })
Namespace.addCategoryObject('typeBinding', u'Plane', Plane_)


# Complex type {cad}CoordinateSystem with content type ELEMENT_ONLY
class CoordinateSystem_ (Datum_):
    """Complex type {cad}CoordinateSystem with content type ELEMENT_ONLY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'CoordinateSystem')
    _XSDLocation = pyxb.utils.utility.Location(u'avm.cad.xsd', 81, 2)
    _ElementMap = Datum_._ElementMap.copy()
    _AttributeMap = Datum_._AttributeMap.copy()
    # Base type is Datum_
    
    # Element DatumMetric (DatumMetric) inherited from {cad}Datum
    
    # Element Metric uses Python identifier Metric
    __Metric = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'Metric'), 'Metric', '__cad_CoordinateSystem__Metric', True, pyxb.utils.utility.Location(u'avm.cad.xsd', 85, 10), )

    
    Metric = property(__Metric.value, __Metric.set, None, None)

    
    # Attribute Notes inherited from {avm}Port
    
    # Attribute XPosition inherited from {avm}Port
    
    # Attribute Definition inherited from {avm}Port
    
    # Attribute YPosition inherited from {avm}Port
    
    # Attribute Name inherited from {avm}Port
    
    # Attribute ID inherited from {avm}PortMapTarget
    
    # Attribute PortMap inherited from {avm}PortMapTarget
    
    # Attribute DatumName inherited from {cad}Datum
    _ElementMap.update({
        __Metric.name() : __Metric
    })
    _AttributeMap.update({
        
    })
Namespace.addCategoryObject('typeBinding', u'CoordinateSystem', CoordinateSystem_)


CADModel = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'CADModel'), CADModel_, location=pyxb.utils.utility.Location(u'avm.cad.xsd', 4, 2))
Namespace.addCategoryObject('elementBinding', CADModel.name().localName(), CADModel)

Parameter = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'Parameter'), Parameter_, location=pyxb.utils.utility.Location(u'avm.cad.xsd', 6, 2))
Namespace.addCategoryObject('elementBinding', Parameter.name().localName(), Parameter)

Metric = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'Metric'), Metric_, location=pyxb.utils.utility.Location(u'avm.cad.xsd', 11, 2))
Namespace.addCategoryObject('elementBinding', Metric.name().localName(), Metric)

Geometry = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'Geometry'), Geometry_, location=pyxb.utils.utility.Location(u'avm.cad.xsd', 12, 2))
Namespace.addCategoryObject('elementBinding', Geometry.name().localName(), Geometry)

CustomGeometryInput = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'CustomGeometryInput'), CustomGeometryInput_, location=pyxb.utils.utility.Location(u'avm.cad.xsd', 20, 2))
Namespace.addCategoryObject('elementBinding', CustomGeometryInput.name().localName(), CustomGeometryInput)

PointReference = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'PointReference'), PointReference_, location=pyxb.utils.utility.Location(u'avm.cad.xsd', 21, 2))
Namespace.addCategoryObject('elementBinding', PointReference.name().localName(), PointReference)

PlaneReference = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'PlaneReference'), PlaneReference_, location=pyxb.utils.utility.Location(u'avm.cad.xsd', 23, 2))
Namespace.addCategoryObject('elementBinding', PlaneReference.name().localName(), PlaneReference)

GuideDatum = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'GuideDatum'), GuideDatum_, location=pyxb.utils.utility.Location(u'avm.cad.xsd', 24, 2))
Namespace.addCategoryObject('elementBinding', GuideDatum.name().localName(), GuideDatum)

AssemblyRoot = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'AssemblyRoot'), AssemblyRoot_, location=pyxb.utils.utility.Location(u'avm.cad.xsd', 25, 2))
Namespace.addCategoryObject('elementBinding', AssemblyRoot.name().localName(), AssemblyRoot)

KinematicJointSpec = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'KinematicJointSpec'), KinematicJointSpec_, location=pyxb.utils.utility.Location(u'avm.cad.xsd', 26, 2))
Namespace.addCategoryObject('elementBinding', KinematicJointSpec.name().localName(), KinematicJointSpec)

Geometry2D = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'Geometry2D'), Geometry2D_, location=pyxb.utils.utility.Location(u'avm.cad.xsd', 13, 2))
Namespace.addCategoryObject('elementBinding', Geometry2D.name().localName(), Geometry2D)

Geometry3D = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'Geometry3D'), Geometry3D_, location=pyxb.utils.utility.Location(u'avm.cad.xsd', 14, 2))
Namespace.addCategoryObject('elementBinding', Geometry3D.name().localName(), Geometry3D)

CustomGeometry = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'CustomGeometry'), CustomGeometry_, location=pyxb.utils.utility.Location(u'avm.cad.xsd', 19, 2))
Namespace.addCategoryObject('elementBinding', CustomGeometry.name().localName(), CustomGeometry)

RevoluteJointSpec = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'RevoluteJointSpec'), RevoluteJointSpec_, location=pyxb.utils.utility.Location(u'avm.cad.xsd', 27, 2))
Namespace.addCategoryObject('elementBinding', RevoluteJointSpec.name().localName(), RevoluteJointSpec)

TranslationalJointSpec = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'TranslationalJointSpec'), TranslationalJointSpec_, location=pyxb.utils.utility.Location(u'avm.cad.xsd', 28, 2))
Namespace.addCategoryObject('elementBinding', TranslationalJointSpec.name().localName(), TranslationalJointSpec)

Circle = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'Circle'), Circle_, location=pyxb.utils.utility.Location(u'avm.cad.xsd', 15, 2))
Namespace.addCategoryObject('elementBinding', Circle.name().localName(), Circle)

Polygon = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'Polygon'), Polygon_, location=pyxb.utils.utility.Location(u'avm.cad.xsd', 16, 2))
Namespace.addCategoryObject('elementBinding', Polygon.name().localName(), Polygon)

ExtrudedGeometry = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'ExtrudedGeometry'), ExtrudedGeometry_, location=pyxb.utils.utility.Location(u'avm.cad.xsd', 17, 2))
Namespace.addCategoryObject('elementBinding', ExtrudedGeometry.name().localName(), ExtrudedGeometry)

Sphere = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'Sphere'), Sphere_, location=pyxb.utils.utility.Location(u'avm.cad.xsd', 18, 2))
Namespace.addCategoryObject('elementBinding', Sphere.name().localName(), Sphere)

Surface = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'Surface'), Surface_, location=pyxb.utils.utility.Location(u'avm.cad.xsd', 22, 2))
Namespace.addCategoryObject('elementBinding', Surface.name().localName(), Surface)

Datum = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'Datum'), Datum_, location=pyxb.utils.utility.Location(u'avm.cad.xsd', 5, 2))
Namespace.addCategoryObject('elementBinding', Datum.name().localName(), Datum)

Point = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'Point'), Point_, location=pyxb.utils.utility.Location(u'avm.cad.xsd', 7, 2))
Namespace.addCategoryObject('elementBinding', Point.name().localName(), Point)

Axis = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'Axis'), Axis_, location=pyxb.utils.utility.Location(u'avm.cad.xsd', 8, 2))
Namespace.addCategoryObject('elementBinding', Axis.name().localName(), Axis)

Plane = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'Plane'), Plane_, location=pyxb.utils.utility.Location(u'avm.cad.xsd', 9, 2))
Namespace.addCategoryObject('elementBinding', Plane.name().localName(), Plane)

CoordinateSystem = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'CoordinateSystem'), CoordinateSystem_, location=pyxb.utils.utility.Location(u'avm.cad.xsd', 10, 2))
Namespace.addCategoryObject('elementBinding', CoordinateSystem.name().localName(), CoordinateSystem)



CADModel_._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'Datum'), Datum_, scope=CADModel_, location=pyxb.utils.utility.Location(u'avm.cad.xsd', 33, 10)))

CADModel_._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'Parameter'), Parameter_, scope=CADModel_, location=pyxb.utils.utility.Location(u'avm.cad.xsd', 34, 10)))

CADModel_._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'ModelMetric'), Metric_, scope=CADModel_, location=pyxb.utils.utility.Location(u'avm.cad.xsd', 35, 10)))

def _BuildAutomaton ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton
    del _BuildAutomaton
    import pyxb.utils.fac as fac

    counters = set()
    cc_0 = fac.CounterCondition(min=0L, max=None, metadata=pyxb.utils.utility.Location(u'avm.cad.xsd', 33, 10))
    counters.add(cc_0)
    cc_1 = fac.CounterCondition(min=0L, max=None, metadata=pyxb.utils.utility.Location(u'avm.cad.xsd', 34, 10))
    counters.add(cc_1)
    cc_2 = fac.CounterCondition(min=0L, max=None, metadata=pyxb.utils.utility.Location(u'avm.cad.xsd', 35, 10))
    counters.add(cc_2)
    states = []
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_0, False))
    symbol = pyxb.binding.content.ElementUse(CADModel_._UseForTag(pyxb.namespace.ExpandedName(None, u'Datum')), pyxb.utils.utility.Location(u'avm.cad.xsd', 33, 10))
    st_0 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_0)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_1, False))
    symbol = pyxb.binding.content.ElementUse(CADModel_._UseForTag(pyxb.namespace.ExpandedName(None, u'Parameter')), pyxb.utils.utility.Location(u'avm.cad.xsd', 34, 10))
    st_1 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_1)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_2, False))
    symbol = pyxb.binding.content.ElementUse(CADModel_._UseForTag(pyxb.namespace.ExpandedName(None, u'ModelMetric')), pyxb.utils.utility.Location(u'avm.cad.xsd', 35, 10))
    st_2 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_2)
    transitions = []
    transitions.append(fac.Transition(st_0, [
        fac.UpdateInstruction(cc_0, True) ]))
    transitions.append(fac.Transition(st_1, [
        fac.UpdateInstruction(cc_0, False) ]))
    transitions.append(fac.Transition(st_2, [
        fac.UpdateInstruction(cc_0, False) ]))
    st_0._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_1, [
        fac.UpdateInstruction(cc_1, True) ]))
    transitions.append(fac.Transition(st_2, [
        fac.UpdateInstruction(cc_1, False) ]))
    st_1._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_2, [
        fac.UpdateInstruction(cc_2, True) ]))
    st_2._set_transitionSet(transitions)
    return fac.Automaton(states, counters, True, containing_state=None)
CADModel_._Automaton = _BuildAutomaton()




Parameter_._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'Value'), _ImportedBinding__avm.Value_, scope=Parameter_, location=pyxb.utils.utility.Location(u'avm.cad.xsd', 54, 10)))

def _BuildAutomaton_ ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton_
    del _BuildAutomaton_
    import pyxb.utils.fac as fac

    counters = set()
    cc_0 = fac.CounterCondition(min=0L, max=1, metadata=pyxb.utils.utility.Location(u'avm.cad.xsd', 54, 10))
    counters.add(cc_0)
    states = []
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_0, False))
    symbol = pyxb.binding.content.ElementUse(Parameter_._UseForTag(pyxb.namespace.ExpandedName(None, u'Value')), pyxb.utils.utility.Location(u'avm.cad.xsd', 54, 10))
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
    cc_0 = fac.CounterCondition(min=0L, max=1, metadata=pyxb.utils.utility.Location(u'avm.xsd', 265, 6))
    counters.add(cc_0)
    states = []
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_0, False))
    symbol = pyxb.binding.content.ElementUse(Metric_._UseForTag(pyxb.namespace.ExpandedName(None, u'Value')), pyxb.utils.utility.Location(u'avm.xsd', 265, 6))
    st_0 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_0)
    transitions = []
    transitions.append(fac.Transition(st_0, [
        fac.UpdateInstruction(cc_0, True) ]))
    st_0._set_transitionSet(transitions)
    return fac.Automaton(states, counters, True, containing_state=None)
Metric_._Automaton = _BuildAutomaton_2()




CustomGeometryInput_._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'InputGeometry'), Geometry_, scope=CustomGeometryInput_, location=pyxb.utils.utility.Location(u'avm.cad.xsd', 166, 6)))

def _BuildAutomaton_3 ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton_3
    del _BuildAutomaton_3
    import pyxb.utils.fac as fac

    counters = set()
    states = []
    final_update = set()
    symbol = pyxb.binding.content.ElementUse(CustomGeometryInput_._UseForTag(pyxb.namespace.ExpandedName(None, u'InputGeometry')), pyxb.utils.utility.Location(u'avm.cad.xsd', 166, 6))
    st_0 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_0)
    transitions = []
    st_0._set_transitionSet(transitions)
    return fac.Automaton(states, counters, False, containing_state=None)
CustomGeometryInput_._Automaton = _BuildAutomaton_3()




CustomGeometry_._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'CustomGeometryInput'), CustomGeometryInput_, scope=CustomGeometry_, location=pyxb.utils.utility.Location(u'avm.cad.xsd', 159, 10)))

def _BuildAutomaton_4 ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton_4
    del _BuildAutomaton_4
    import pyxb.utils.fac as fac

    counters = set()
    cc_0 = fac.CounterCondition(min=2L, max=None, metadata=pyxb.utils.utility.Location(u'avm.cad.xsd', 159, 10))
    counters.add(cc_0)
    states = []
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_0, False))
    symbol = pyxb.binding.content.ElementUse(CustomGeometry_._UseForTag(pyxb.namespace.ExpandedName(None, u'CustomGeometryInput')), pyxb.utils.utility.Location(u'avm.cad.xsd', 159, 10))
    st_0 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_0)
    transitions = []
    transitions.append(fac.Transition(st_0, [
        fac.UpdateInstruction(cc_0, True) ]))
    st_0._set_transitionSet(transitions)
    return fac.Automaton(states, counters, False, containing_state=None)
CustomGeometry_._Automaton = _BuildAutomaton_4()




RevoluteJointSpec_._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'MinimumRotation'), _ImportedBinding__avm.Value_, scope=RevoluteJointSpec_, location=pyxb.utils.utility.Location(u'avm.cad.xsd', 237, 10)))

RevoluteJointSpec_._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'DefaultRotation'), _ImportedBinding__avm.Value_, scope=RevoluteJointSpec_, location=pyxb.utils.utility.Location(u'avm.cad.xsd', 238, 10)))

RevoluteJointSpec_._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'MaximumRotation'), _ImportedBinding__avm.Value_, scope=RevoluteJointSpec_, location=pyxb.utils.utility.Location(u'avm.cad.xsd', 239, 10)))

def _BuildAutomaton_5 ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton_5
    del _BuildAutomaton_5
    import pyxb.utils.fac as fac

    counters = set()
    cc_0 = fac.CounterCondition(min=0L, max=1, metadata=pyxb.utils.utility.Location(u'avm.cad.xsd', 237, 10))
    counters.add(cc_0)
    cc_1 = fac.CounterCondition(min=0L, max=1, metadata=pyxb.utils.utility.Location(u'avm.cad.xsd', 238, 10))
    counters.add(cc_1)
    cc_2 = fac.CounterCondition(min=0L, max=1, metadata=pyxb.utils.utility.Location(u'avm.cad.xsd', 239, 10))
    counters.add(cc_2)
    states = []
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_0, False))
    symbol = pyxb.binding.content.ElementUse(RevoluteJointSpec_._UseForTag(pyxb.namespace.ExpandedName(None, u'MinimumRotation')), pyxb.utils.utility.Location(u'avm.cad.xsd', 237, 10))
    st_0 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_0)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_1, False))
    symbol = pyxb.binding.content.ElementUse(RevoluteJointSpec_._UseForTag(pyxb.namespace.ExpandedName(None, u'DefaultRotation')), pyxb.utils.utility.Location(u'avm.cad.xsd', 238, 10))
    st_1 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_1)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_2, False))
    symbol = pyxb.binding.content.ElementUse(RevoluteJointSpec_._UseForTag(pyxb.namespace.ExpandedName(None, u'MaximumRotation')), pyxb.utils.utility.Location(u'avm.cad.xsd', 239, 10))
    st_2 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_2)
    transitions = []
    transitions.append(fac.Transition(st_0, [
        fac.UpdateInstruction(cc_0, True) ]))
    transitions.append(fac.Transition(st_1, [
        fac.UpdateInstruction(cc_0, False) ]))
    transitions.append(fac.Transition(st_2, [
        fac.UpdateInstruction(cc_0, False) ]))
    st_0._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_1, [
        fac.UpdateInstruction(cc_1, True) ]))
    transitions.append(fac.Transition(st_2, [
        fac.UpdateInstruction(cc_1, False) ]))
    st_1._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_2, [
        fac.UpdateInstruction(cc_2, True) ]))
    st_2._set_transitionSet(transitions)
    return fac.Automaton(states, counters, True, containing_state=None)
RevoluteJointSpec_._Automaton = _BuildAutomaton_5()




TranslationalJointSpec_._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'MinimumTranslation'), _ImportedBinding__avm.Value_, scope=TranslationalJointSpec_, location=pyxb.utils.utility.Location(u'avm.cad.xsd', 251, 10)))

TranslationalJointSpec_._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'DefaultTranslation'), _ImportedBinding__avm.Value_, scope=TranslationalJointSpec_, location=pyxb.utils.utility.Location(u'avm.cad.xsd', 252, 10)))

TranslationalJointSpec_._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'MaximumTranslation'), _ImportedBinding__avm.Value_, scope=TranslationalJointSpec_, location=pyxb.utils.utility.Location(u'avm.cad.xsd', 253, 10)))

def _BuildAutomaton_6 ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton_6
    del _BuildAutomaton_6
    import pyxb.utils.fac as fac

    counters = set()
    cc_0 = fac.CounterCondition(min=0L, max=1, metadata=pyxb.utils.utility.Location(u'avm.cad.xsd', 251, 10))
    counters.add(cc_0)
    cc_1 = fac.CounterCondition(min=0L, max=1, metadata=pyxb.utils.utility.Location(u'avm.cad.xsd', 252, 10))
    counters.add(cc_1)
    cc_2 = fac.CounterCondition(min=0L, max=1, metadata=pyxb.utils.utility.Location(u'avm.cad.xsd', 253, 10))
    counters.add(cc_2)
    states = []
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_0, False))
    symbol = pyxb.binding.content.ElementUse(TranslationalJointSpec_._UseForTag(pyxb.namespace.ExpandedName(None, u'MinimumTranslation')), pyxb.utils.utility.Location(u'avm.cad.xsd', 251, 10))
    st_0 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_0)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_1, False))
    symbol = pyxb.binding.content.ElementUse(TranslationalJointSpec_._UseForTag(pyxb.namespace.ExpandedName(None, u'DefaultTranslation')), pyxb.utils.utility.Location(u'avm.cad.xsd', 252, 10))
    st_1 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_1)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_2, False))
    symbol = pyxb.binding.content.ElementUse(TranslationalJointSpec_._UseForTag(pyxb.namespace.ExpandedName(None, u'MaximumTranslation')), pyxb.utils.utility.Location(u'avm.cad.xsd', 253, 10))
    st_2 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_2)
    transitions = []
    transitions.append(fac.Transition(st_0, [
        fac.UpdateInstruction(cc_0, True) ]))
    transitions.append(fac.Transition(st_1, [
        fac.UpdateInstruction(cc_0, False) ]))
    transitions.append(fac.Transition(st_2, [
        fac.UpdateInstruction(cc_0, False) ]))
    st_0._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_1, [
        fac.UpdateInstruction(cc_1, True) ]))
    transitions.append(fac.Transition(st_2, [
        fac.UpdateInstruction(cc_1, False) ]))
    st_1._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_2, [
        fac.UpdateInstruction(cc_2, True) ]))
    st_2._set_transitionSet(transitions)
    return fac.Automaton(states, counters, True, containing_state=None)
TranslationalJointSpec_._Automaton = _BuildAutomaton_6()




Circle_._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'CircleCenter'), PointReference_, scope=Circle_, location=pyxb.utils.utility.Location(u'avm.cad.xsd', 119, 10)))

Circle_._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'CircleEdge'), PointReference_, scope=Circle_, location=pyxb.utils.utility.Location(u'avm.cad.xsd', 120, 10)))

def _BuildAutomaton_7 ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton_7
    del _BuildAutomaton_7
    import pyxb.utils.fac as fac

    counters = set()
    cc_0 = fac.CounterCondition(min=2L, max=2L, metadata=pyxb.utils.utility.Location(u'avm.cad.xsd', 120, 10))
    counters.add(cc_0)
    states = []
    final_update = None
    symbol = pyxb.binding.content.ElementUse(Circle_._UseForTag(pyxb.namespace.ExpandedName(None, u'CircleCenter')), pyxb.utils.utility.Location(u'avm.cad.xsd', 119, 10))
    st_0 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_0)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_0, False))
    symbol = pyxb.binding.content.ElementUse(Circle_._UseForTag(pyxb.namespace.ExpandedName(None, u'CircleEdge')), pyxb.utils.utility.Location(u'avm.cad.xsd', 120, 10))
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
Circle_._Automaton = _BuildAutomaton_7()




Polygon_._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'PolygonPoint'), PointReference_, scope=Polygon_, location=pyxb.utils.utility.Location(u'avm.cad.xsd', 129, 10)))

def _BuildAutomaton_8 ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton_8
    del _BuildAutomaton_8
    import pyxb.utils.fac as fac

    counters = set()
    cc_0 = fac.CounterCondition(min=3L, max=None, metadata=pyxb.utils.utility.Location(u'avm.cad.xsd', 129, 10))
    counters.add(cc_0)
    states = []
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_0, False))
    symbol = pyxb.binding.content.ElementUse(Polygon_._UseForTag(pyxb.namespace.ExpandedName(None, u'PolygonPoint')), pyxb.utils.utility.Location(u'avm.cad.xsd', 129, 10))
    st_0 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_0)
    transitions = []
    transitions.append(fac.Transition(st_0, [
        fac.UpdateInstruction(cc_0, True) ]))
    st_0._set_transitionSet(transitions)
    return fac.Automaton(states, counters, False, containing_state=None)
Polygon_._Automaton = _BuildAutomaton_8()




ExtrudedGeometry_._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'ExtrusionHeight'), PointReference_, scope=ExtrudedGeometry_, location=pyxb.utils.utility.Location(u'avm.cad.xsd', 138, 10)))

ExtrudedGeometry_._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'ExtrusionSurface'), Geometry2D_, scope=ExtrudedGeometry_, location=pyxb.utils.utility.Location(u'avm.cad.xsd', 139, 10)))

ExtrudedGeometry_._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'DirectionReferencePoint'), PointReference_, scope=ExtrudedGeometry_, location=pyxb.utils.utility.Location(u'avm.cad.xsd', 140, 10)))

def _BuildAutomaton_9 ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton_9
    del _BuildAutomaton_9
    import pyxb.utils.fac as fac

    counters = set()
    cc_0 = fac.CounterCondition(min=0L, max=1, metadata=pyxb.utils.utility.Location(u'avm.cad.xsd', 140, 10))
    counters.add(cc_0)
    states = []
    final_update = None
    symbol = pyxb.binding.content.ElementUse(ExtrudedGeometry_._UseForTag(pyxb.namespace.ExpandedName(None, u'ExtrusionHeight')), pyxb.utils.utility.Location(u'avm.cad.xsd', 138, 10))
    st_0 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_0)
    final_update = set()
    symbol = pyxb.binding.content.ElementUse(ExtrudedGeometry_._UseForTag(pyxb.namespace.ExpandedName(None, u'ExtrusionSurface')), pyxb.utils.utility.Location(u'avm.cad.xsd', 139, 10))
    st_1 = fac.State(symbol, is_initial=False, final_update=final_update, is_unordered_catenation=False)
    states.append(st_1)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_0, False))
    symbol = pyxb.binding.content.ElementUse(ExtrudedGeometry_._UseForTag(pyxb.namespace.ExpandedName(None, u'DirectionReferencePoint')), pyxb.utils.utility.Location(u'avm.cad.xsd', 140, 10))
    st_2 = fac.State(symbol, is_initial=False, final_update=final_update, is_unordered_catenation=False)
    states.append(st_2)
    transitions = []
    transitions.append(fac.Transition(st_1, [
         ]))
    st_0._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_2, [
         ]))
    st_1._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_2, [
        fac.UpdateInstruction(cc_0, True) ]))
    st_2._set_transitionSet(transitions)
    return fac.Automaton(states, counters, False, containing_state=None)
ExtrudedGeometry_._Automaton = _BuildAutomaton_9()




Sphere_._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'SphereCenter'), PointReference_, scope=Sphere_, location=pyxb.utils.utility.Location(u'avm.cad.xsd', 149, 10)))

Sphere_._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'SphereEdge'), PointReference_, scope=Sphere_, location=pyxb.utils.utility.Location(u'avm.cad.xsd', 150, 10)))

def _BuildAutomaton_10 ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton_10
    del _BuildAutomaton_10
    import pyxb.utils.fac as fac

    counters = set()
    states = []
    final_update = None
    symbol = pyxb.binding.content.ElementUse(Sphere_._UseForTag(pyxb.namespace.ExpandedName(None, u'SphereCenter')), pyxb.utils.utility.Location(u'avm.cad.xsd', 149, 10))
    st_0 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_0)
    final_update = set()
    symbol = pyxb.binding.content.ElementUse(Sphere_._UseForTag(pyxb.namespace.ExpandedName(None, u'SphereEdge')), pyxb.utils.utility.Location(u'avm.cad.xsd', 150, 10))
    st_1 = fac.State(symbol, is_initial=False, final_update=final_update, is_unordered_catenation=False)
    states.append(st_1)
    transitions = []
    transitions.append(fac.Transition(st_1, [
         ]))
    st_0._set_transitionSet(transitions)
    transitions = []
    st_1._set_transitionSet(transitions)
    return fac.Automaton(states, counters, False, containing_state=None)
Sphere_._Automaton = _BuildAutomaton_10()




Surface_._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'ReferencePlane'), PlaneReference_, scope=Surface_, location=pyxb.utils.utility.Location(u'avm.cad.xsd', 202, 10)))

def _BuildAutomaton_11 ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton_11
    del _BuildAutomaton_11
    import pyxb.utils.fac as fac

    counters = set()
    states = []
    final_update = set()
    symbol = pyxb.binding.content.ElementUse(Surface_._UseForTag(pyxb.namespace.ExpandedName(None, u'ReferencePlane')), pyxb.utils.utility.Location(u'avm.cad.xsd', 202, 10))
    st_0 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_0)
    transitions = []
    st_0._set_transitionSet(transitions)
    return fac.Automaton(states, counters, False, containing_state=None)
Surface_._Automaton = _BuildAutomaton_11()




Datum_._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'DatumMetric'), Metric_, scope=Datum_, location=pyxb.utils.utility.Location(u'avm.cad.xsd', 44, 10)))

def _BuildAutomaton_12 ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton_12
    del _BuildAutomaton_12
    import pyxb.utils.fac as fac

    counters = set()
    cc_0 = fac.CounterCondition(min=0L, max=None, metadata=pyxb.utils.utility.Location(u'avm.cad.xsd', 44, 10))
    counters.add(cc_0)
    states = []
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_0, False))
    symbol = pyxb.binding.content.ElementUse(Datum_._UseForTag(pyxb.namespace.ExpandedName(None, u'DatumMetric')), pyxb.utils.utility.Location(u'avm.cad.xsd', 44, 10))
    st_0 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_0)
    transitions = []
    transitions.append(fac.Transition(st_0, [
        fac.UpdateInstruction(cc_0, True) ]))
    st_0._set_transitionSet(transitions)
    return fac.Automaton(states, counters, True, containing_state=None)
Datum_._Automaton = _BuildAutomaton_12()




def _BuildAutomaton_13 ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton_13
    del _BuildAutomaton_13
    import pyxb.utils.fac as fac

    counters = set()
    cc_0 = fac.CounterCondition(min=0L, max=None, metadata=pyxb.utils.utility.Location(u'avm.cad.xsd', 44, 10))
    counters.add(cc_0)
    states = []
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_0, False))
    symbol = pyxb.binding.content.ElementUse(Point_._UseForTag(pyxb.namespace.ExpandedName(None, u'DatumMetric')), pyxb.utils.utility.Location(u'avm.cad.xsd', 44, 10))
    st_0 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_0)
    transitions = []
    transitions.append(fac.Transition(st_0, [
        fac.UpdateInstruction(cc_0, True) ]))
    st_0._set_transitionSet(transitions)
    return fac.Automaton(states, counters, True, containing_state=None)
Point_._Automaton = _BuildAutomaton_13()




def _BuildAutomaton_14 ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton_14
    del _BuildAutomaton_14
    import pyxb.utils.fac as fac

    counters = set()
    cc_0 = fac.CounterCondition(min=0L, max=None, metadata=pyxb.utils.utility.Location(u'avm.cad.xsd', 44, 10))
    counters.add(cc_0)
    states = []
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_0, False))
    symbol = pyxb.binding.content.ElementUse(Axis_._UseForTag(pyxb.namespace.ExpandedName(None, u'DatumMetric')), pyxb.utils.utility.Location(u'avm.cad.xsd', 44, 10))
    st_0 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_0)
    transitions = []
    transitions.append(fac.Transition(st_0, [
        fac.UpdateInstruction(cc_0, True) ]))
    st_0._set_transitionSet(transitions)
    return fac.Automaton(states, counters, True, containing_state=None)
Axis_._Automaton = _BuildAutomaton_14()




def _BuildAutomaton_15 ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton_15
    del _BuildAutomaton_15
    import pyxb.utils.fac as fac

    counters = set()
    cc_0 = fac.CounterCondition(min=0L, max=None, metadata=pyxb.utils.utility.Location(u'avm.cad.xsd', 44, 10))
    counters.add(cc_0)
    states = []
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_0, False))
    symbol = pyxb.binding.content.ElementUse(Plane_._UseForTag(pyxb.namespace.ExpandedName(None, u'DatumMetric')), pyxb.utils.utility.Location(u'avm.cad.xsd', 44, 10))
    st_0 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_0)
    transitions = []
    transitions.append(fac.Transition(st_0, [
        fac.UpdateInstruction(cc_0, True) ]))
    st_0._set_transitionSet(transitions)
    return fac.Automaton(states, counters, True, containing_state=None)
Plane_._Automaton = _BuildAutomaton_15()




CoordinateSystem_._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'Metric'), Metric_, scope=CoordinateSystem_, location=pyxb.utils.utility.Location(u'avm.cad.xsd', 85, 10)))

def _BuildAutomaton_16 ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton_16
    del _BuildAutomaton_16
    import pyxb.utils.fac as fac

    counters = set()
    cc_0 = fac.CounterCondition(min=0L, max=None, metadata=pyxb.utils.utility.Location(u'avm.cad.xsd', 44, 10))
    counters.add(cc_0)
    cc_1 = fac.CounterCondition(min=0L, max=None, metadata=pyxb.utils.utility.Location(u'avm.cad.xsd', 85, 10))
    counters.add(cc_1)
    states = []
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_0, False))
    symbol = pyxb.binding.content.ElementUse(CoordinateSystem_._UseForTag(pyxb.namespace.ExpandedName(None, u'DatumMetric')), pyxb.utils.utility.Location(u'avm.cad.xsd', 44, 10))
    st_0 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_0)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_1, False))
    symbol = pyxb.binding.content.ElementUse(CoordinateSystem_._UseForTag(pyxb.namespace.ExpandedName(None, u'Metric')), pyxb.utils.utility.Location(u'avm.cad.xsd', 85, 10))
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
CoordinateSystem_._Automaton = _BuildAutomaton_16()

