# .\_iFAB.py
# -*- coding: utf-8 -*-
# PyXB bindings for NM:de45d6eea5b439c583053bcc995a5a07ca6f002d
# Generated 2023-01-19 12:49:23.710997 by PyXB version 1.2.6 using Python 3.11.1.final.0
# Namespace iFAB [xmlns:iFAB]

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

# NOTE: All namespace declarations are reserved within the binding
Namespace = pyxb.namespace.NamespaceForURI('iFAB', create_if_missing=True)
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


# Atomic simple type: {iFAB}baseMaterial
class baseMaterial (pyxb.binding.datatypes.token, pyxb.binding.basis.enumeration_mixin):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'baseMaterial')
    _XSDLocation = pyxb.utils.utility.Location('AssemblyDetails.xsd', 101, 2)
    _Documentation = None
baseMaterial._CF_enumeration = pyxb.binding.facets.CF_enumeration(value_datatype=baseMaterial, enum_prefix=None)
baseMaterial.Al = baseMaterial._CF_enumeration.addEnumeration(unicode_value='Al', tag='Al')
baseMaterial.Plain_Carbon_Steel = baseMaterial._CF_enumeration.addEnumeration(unicode_value='Plain Carbon Steel', tag='Plain_Carbon_Steel')
baseMaterial._InitializeFacetMap(baseMaterial._CF_enumeration)
Namespace.addCategoryObject('typeBinding', 'baseMaterial', baseMaterial)
_module_typeBindings.baseMaterial = baseMaterial

# Atomic simple type: {iFAB}fillerMaterial
class fillerMaterial (pyxb.binding.datatypes.string):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'fillerMaterial')
    _XSDLocation = pyxb.utils.utility.Location('AssemblyDetails.xsd', 108, 2)
    _Documentation = None
fillerMaterial._InitializeFacetMap()
Namespace.addCategoryObject('typeBinding', 'fillerMaterial', fillerMaterial)
_module_typeBindings.fillerMaterial = fillerMaterial

# Atomic simple type: {iFAB}fluxMaterial
class fluxMaterial (pyxb.binding.datatypes.string):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'fluxMaterial')
    _XSDLocation = pyxb.utils.utility.Location('AssemblyDetails.xsd', 112, 2)
    _Documentation = None
fluxMaterial._InitializeFacetMap()
Namespace.addCategoryObject('typeBinding', 'fluxMaterial', fluxMaterial)
_module_typeBindings.fluxMaterial = fluxMaterial

# Atomic simple type: {iFAB}glue
class glue (pyxb.binding.datatypes.string):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'glue')
    _XSDLocation = pyxb.utils.utility.Location('AssemblyDetails.xsd', 116, 2)
    _Documentation = None
glue._InitializeFacetMap()
Namespace.addCategoryObject('typeBinding', 'glue', glue)
_module_typeBindings.glue = glue

# Atomic simple type: {iFAB}fasteningMethod
class fasteningMethod (pyxb.binding.datatypes.token, pyxb.binding.basis.enumeration_mixin):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'fasteningMethod')
    _XSDLocation = pyxb.utils.utility.Location('AssemblyDetails.xsd', 120, 2)
    _Documentation = None
fasteningMethod._CF_enumeration = pyxb.binding.facets.CF_enumeration(value_datatype=fasteningMethod, enum_prefix=None)
fasteningMethod.Bolted = fasteningMethod._CF_enumeration.addEnumeration(unicode_value='Bolted', tag='Bolted')
fasteningMethod.Bolted_blind = fasteningMethod._CF_enumeration.addEnumeration(unicode_value='Bolted (blind)', tag='Bolted_blind')
fasteningMethod.Machined_Screw = fasteningMethod._CF_enumeration.addEnumeration(unicode_value='Machined Screw', tag='Machined_Screw')
fasteningMethod.Press_Fit = fasteningMethod._CF_enumeration.addEnumeration(unicode_value='Press Fit', tag='Press_Fit')
fasteningMethod.Snap_Fit = fasteningMethod._CF_enumeration.addEnumeration(unicode_value='Snap Fit', tag='Snap_Fit')
fasteningMethod.CrimpClamp_Fit = fasteningMethod._CF_enumeration.addEnumeration(unicode_value='Crimp/Clamp Fit', tag='CrimpClamp_Fit')
fasteningMethod._InitializeFacetMap(fasteningMethod._CF_enumeration)
Namespace.addCategoryObject('typeBinding', 'fasteningMethod', fasteningMethod)
_module_typeBindings.fasteningMethod = fasteningMethod

# Atomic simple type: {iFAB}jointType
class jointType (pyxb.binding.datatypes.token, pyxb.binding.basis.enumeration_mixin):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'jointType')
    _XSDLocation = pyxb.utils.utility.Location('AssemblyDetails.xsd', 131, 2)
    _Documentation = None
jointType._CF_enumeration = pyxb.binding.facets.CF_enumeration(value_datatype=jointType, enum_prefix=None)
jointType.Butt = jointType._CF_enumeration.addEnumeration(unicode_value='Butt', tag='Butt')
jointType.Corner = jointType._CF_enumeration.addEnumeration(unicode_value='Corner', tag='Corner')
jointType.Edge = jointType._CF_enumeration.addEnumeration(unicode_value='Edge', tag='Edge')
jointType.Lap = jointType._CF_enumeration.addEnumeration(unicode_value='Lap', tag='Lap')
jointType.Tee = jointType._CF_enumeration.addEnumeration(unicode_value='Tee', tag='Tee')
jointType._InitializeFacetMap(jointType._CF_enumeration)
Namespace.addCategoryObject('typeBinding', 'jointType', jointType)
_module_typeBindings.jointType = jointType

# Atomic simple type: {iFAB}weldType
class weldType (pyxb.binding.datatypes.token, pyxb.binding.basis.enumeration_mixin):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'weldType')
    _XSDLocation = pyxb.utils.utility.Location('AssemblyDetails.xsd', 141, 2)
    _Documentation = None
weldType._CF_enumeration = pyxb.binding.facets.CF_enumeration(value_datatype=weldType, enum_prefix=None)
weldType.Seam = weldType._CF_enumeration.addEnumeration(unicode_value='Seam', tag='Seam')
weldType.Stitch = weldType._CF_enumeration.addEnumeration(unicode_value='Stitch', tag='Stitch')
weldType.Spot = weldType._CF_enumeration.addEnumeration(unicode_value='Spot', tag='Spot')
weldType._InitializeFacetMap(weldType._CF_enumeration)
Namespace.addCategoryObject('typeBinding', 'weldType', weldType)
_module_typeBindings.weldType = weldType

# Atomic simple type: {iFAB}weldPenetration
class weldPenetration (pyxb.binding.datatypes.token, pyxb.binding.basis.enumeration_mixin):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'weldPenetration')
    _XSDLocation = pyxb.utils.utility.Location('AssemblyDetails.xsd', 149, 2)
    _Documentation = None
weldPenetration._CF_enumeration = pyxb.binding.facets.CF_enumeration(value_datatype=weldPenetration, enum_prefix=None)
weldPenetration.Full = weldPenetration._CF_enumeration.addEnumeration(unicode_value='Full', tag='Full')
weldPenetration.Partial = weldPenetration._CF_enumeration.addEnumeration(unicode_value='Partial', tag='Partial')
weldPenetration._InitializeFacetMap(weldPenetration._CF_enumeration)
Namespace.addCategoryObject('typeBinding', 'weldPenetration', weldPenetration)
_module_typeBindings.weldPenetration = weldPenetration

# Atomic simple type: {iFAB}inspectionRequirement
class inspectionRequirement (pyxb.binding.datatypes.token, pyxb.binding.basis.enumeration_mixin):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'inspectionRequirement')
    _XSDLocation = pyxb.utils.utility.Location('AssemblyDetails.xsd', 156, 2)
    _Documentation = None
inspectionRequirement._CF_enumeration = pyxb.binding.facets.CF_enumeration(value_datatype=inspectionRequirement, enum_prefix=None)
inspectionRequirement.Visual = inspectionRequirement._CF_enumeration.addEnumeration(unicode_value='Visual', tag='Visual')
inspectionRequirement.X_Ray = inspectionRequirement._CF_enumeration.addEnumeration(unicode_value='X-Ray', tag='X_Ray')
inspectionRequirement._InitializeFacetMap(inspectionRequirement._CF_enumeration)
Namespace.addCategoryObject('typeBinding', 'inspectionRequirement', inspectionRequirement)
_module_typeBindings.inspectionRequirement = inspectionRequirement

# Atomic simple type: {iFAB}guid
class guid (pyxb.binding.datatypes.normalizedString):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'guid')
    _XSDLocation = pyxb.utils.utility.Location('common.xsd', 12, 2)
    _Documentation = None
guid._CF_pattern = pyxb.binding.facets.CF_pattern()
guid._CF_pattern.addPattern(pattern='[a-fA-F0-9]{8}-[a-fA-F0-9]{4}-[a-fA-F0-9]{4}-[a-fA-F0-9]{4}-[a-fA-F0-9]{12}')
guid._InitializeFacetMap(guid._CF_pattern)
Namespace.addCategoryObject('typeBinding', 'guid', guid)
_module_typeBindings.guid = guid

# Atomic simple type: {iFAB}nonNegativeDecimal
class nonNegativeDecimal (pyxb.binding.datatypes.decimal):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'nonNegativeDecimal')
    _XSDLocation = pyxb.utils.utility.Location('common.xsd', 18, 2)
    _Documentation = None
nonNegativeDecimal._CF_minInclusive = pyxb.binding.facets.CF_minInclusive(value_datatype=nonNegativeDecimal, value=pyxb.binding.datatypes.decimal('0.0'))
nonNegativeDecimal._InitializeFacetMap(nonNegativeDecimal._CF_minInclusive)
Namespace.addCategoryObject('typeBinding', 'nonNegativeDecimal', nonNegativeDecimal)
_module_typeBindings.nonNegativeDecimal = nonNegativeDecimal

# Atomic simple type: {iFAB}currency
class currency (pyxb.binding.datatypes.normalizedString, pyxb.binding.basis.enumeration_mixin):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'currency')
    _XSDLocation = pyxb.utils.utility.Location('common.xsd', 122, 2)
    _Documentation = None
currency._CF_enumeration = pyxb.binding.facets.CF_enumeration(value_datatype=currency, enum_prefix=None)
currency.USD = currency._CF_enumeration.addEnumeration(unicode_value='USD', tag='USD')
currency._InitializeFacetMap(currency._CF_enumeration)
Namespace.addCategoryObject('typeBinding', 'currency', currency)
_module_typeBindings.currency = currency

# Atomic simple type: {iFAB}timeUnit
class timeUnit (pyxb.binding.datatypes.normalizedString, pyxb.binding.basis.enumeration_mixin):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'timeUnit')
    _XSDLocation = pyxb.utils.utility.Location('common.xsd', 128, 2)
    _Documentation = None
timeUnit._CF_enumeration = pyxb.binding.facets.CF_enumeration(value_datatype=timeUnit, enum_prefix=None)
timeUnit.week = timeUnit._CF_enumeration.addEnumeration(unicode_value='week', tag='week')
timeUnit.day = timeUnit._CF_enumeration.addEnumeration(unicode_value='day', tag='day')
timeUnit.hr = timeUnit._CF_enumeration.addEnumeration(unicode_value='hr', tag='hr')
timeUnit.min = timeUnit._CF_enumeration.addEnumeration(unicode_value='min', tag='min')
timeUnit.sec = timeUnit._CF_enumeration.addEnumeration(unicode_value='sec', tag='sec')
timeUnit._InitializeFacetMap(timeUnit._CF_enumeration)
Namespace.addCategoryObject('typeBinding', 'timeUnit', timeUnit)
_module_typeBindings.timeUnit = timeUnit

# Atomic simple type: {iFAB}lengthUnit
class lengthUnit (pyxb.binding.datatypes.normalizedString, pyxb.binding.basis.enumeration_mixin):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'lengthUnit')
    _XSDLocation = pyxb.utils.utility.Location('common.xsd', 138, 2)
    _Documentation = None
lengthUnit._CF_enumeration = pyxb.binding.facets.CF_enumeration(value_datatype=lengthUnit, enum_prefix=None)
lengthUnit.mile = lengthUnit._CF_enumeration.addEnumeration(unicode_value='mile', tag='mile')
lengthUnit.yd = lengthUnit._CF_enumeration.addEnumeration(unicode_value='yd', tag='yd')
lengthUnit.ft = lengthUnit._CF_enumeration.addEnumeration(unicode_value='ft', tag='ft')
lengthUnit.in_ = lengthUnit._CF_enumeration.addEnumeration(unicode_value='in', tag='in_')
lengthUnit.km = lengthUnit._CF_enumeration.addEnumeration(unicode_value='km', tag='km')
lengthUnit.m = lengthUnit._CF_enumeration.addEnumeration(unicode_value='m', tag='m')
lengthUnit.cm = lengthUnit._CF_enumeration.addEnumeration(unicode_value='cm', tag='cm')
lengthUnit.mm = lengthUnit._CF_enumeration.addEnumeration(unicode_value='mm', tag='mm')
lengthUnit.um = lengthUnit._CF_enumeration.addEnumeration(unicode_value='um', tag='um')
lengthUnit.nm = lengthUnit._CF_enumeration.addEnumeration(unicode_value='nm', tag='nm')
lengthUnit._InitializeFacetMap(lengthUnit._CF_enumeration)
Namespace.addCategoryObject('typeBinding', 'lengthUnit', lengthUnit)
_module_typeBindings.lengthUnit = lengthUnit

# Atomic simple type: {iFAB}areaUnit
class areaUnit (pyxb.binding.datatypes.normalizedString, pyxb.binding.basis.enumeration_mixin):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'areaUnit')
    _XSDLocation = pyxb.utils.utility.Location('common.xsd', 153, 2)
    _Documentation = None
areaUnit._CF_enumeration = pyxb.binding.facets.CF_enumeration(value_datatype=areaUnit, enum_prefix=None)
areaUnit.in2 = areaUnit._CF_enumeration.addEnumeration(unicode_value='in2', tag='in2')
areaUnit.ft2 = areaUnit._CF_enumeration.addEnumeration(unicode_value='ft2', tag='ft2')
areaUnit.yd2 = areaUnit._CF_enumeration.addEnumeration(unicode_value='yd2', tag='yd2')
areaUnit.acre = areaUnit._CF_enumeration.addEnumeration(unicode_value='acre', tag='acre')
areaUnit.mile2 = areaUnit._CF_enumeration.addEnumeration(unicode_value='mile2', tag='mile2')
areaUnit.mm2 = areaUnit._CF_enumeration.addEnumeration(unicode_value='mm2', tag='mm2')
areaUnit.cm2 = areaUnit._CF_enumeration.addEnumeration(unicode_value='cm2', tag='cm2')
areaUnit.m2 = areaUnit._CF_enumeration.addEnumeration(unicode_value='m2', tag='m2')
areaUnit.km2 = areaUnit._CF_enumeration.addEnumeration(unicode_value='km2', tag='km2')
areaUnit._InitializeFacetMap(areaUnit._CF_enumeration)
Namespace.addCategoryObject('typeBinding', 'areaUnit', areaUnit)
_module_typeBindings.areaUnit = areaUnit

# Atomic simple type: {iFAB}volumeUnit
class volumeUnit (pyxb.binding.datatypes.normalizedString, pyxb.binding.basis.enumeration_mixin):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'volumeUnit')
    _XSDLocation = pyxb.utils.utility.Location('common.xsd', 167, 2)
    _Documentation = None
volumeUnit._CF_enumeration = pyxb.binding.facets.CF_enumeration(value_datatype=volumeUnit, enum_prefix=None)
volumeUnit.in3 = volumeUnit._CF_enumeration.addEnumeration(unicode_value='in3', tag='in3')
volumeUnit.ft3 = volumeUnit._CF_enumeration.addEnumeration(unicode_value='ft3', tag='ft3')
volumeUnit.fl_oz = volumeUnit._CF_enumeration.addEnumeration(unicode_value='fl oz', tag='fl_oz')
volumeUnit.pt = volumeUnit._CF_enumeration.addEnumeration(unicode_value='pt', tag='pt')
volumeUnit.gal = volumeUnit._CF_enumeration.addEnumeration(unicode_value='gal', tag='gal')
volumeUnit.mL = volumeUnit._CF_enumeration.addEnumeration(unicode_value='mL', tag='mL')
volumeUnit.L = volumeUnit._CF_enumeration.addEnumeration(unicode_value='L', tag='L')
volumeUnit.mm3 = volumeUnit._CF_enumeration.addEnumeration(unicode_value='mm3', tag='mm3')
volumeUnit.cm3 = volumeUnit._CF_enumeration.addEnumeration(unicode_value='cm3', tag='cm3')
volumeUnit.m3 = volumeUnit._CF_enumeration.addEnumeration(unicode_value='m3', tag='m3')
volumeUnit._InitializeFacetMap(volumeUnit._CF_enumeration)
Namespace.addCategoryObject('typeBinding', 'volumeUnit', volumeUnit)
_module_typeBindings.volumeUnit = volumeUnit

# Atomic simple type: {iFAB}massUnit
class massUnit (pyxb.binding.datatypes.normalizedString, pyxb.binding.basis.enumeration_mixin):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'massUnit')
    _XSDLocation = pyxb.utils.utility.Location('common.xsd', 182, 2)
    _Documentation = None
massUnit._CF_enumeration = pyxb.binding.facets.CF_enumeration(value_datatype=massUnit, enum_prefix=None)
massUnit.oz = massUnit._CF_enumeration.addEnumeration(unicode_value='oz', tag='oz')
massUnit.lb = massUnit._CF_enumeration.addEnumeration(unicode_value='lb', tag='lb')
massUnit.ton = massUnit._CF_enumeration.addEnumeration(unicode_value='ton', tag='ton')
massUnit.mg = massUnit._CF_enumeration.addEnumeration(unicode_value='mg', tag='mg')
massUnit.g = massUnit._CF_enumeration.addEnumeration(unicode_value='g', tag='g')
massUnit.kg = massUnit._CF_enumeration.addEnumeration(unicode_value='kg', tag='kg')
massUnit.tonne = massUnit._CF_enumeration.addEnumeration(unicode_value='tonne', tag='tonne')
massUnit._InitializeFacetMap(massUnit._CF_enumeration)
Namespace.addCategoryObject('typeBinding', 'massUnit', massUnit)
_module_typeBindings.massUnit = massUnit

# Atomic simple type: {iFAB}forceUnit
class forceUnit (pyxb.binding.datatypes.normalizedString, pyxb.binding.basis.enumeration_mixin):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'forceUnit')
    _XSDLocation = pyxb.utils.utility.Location('common.xsd', 194, 2)
    _Documentation = None
forceUnit._CF_enumeration = pyxb.binding.facets.CF_enumeration(value_datatype=forceUnit, enum_prefix=None)
forceUnit.N = forceUnit._CF_enumeration.addEnumeration(unicode_value='N', tag='N')
forceUnit._InitializeFacetMap(forceUnit._CF_enumeration)
Namespace.addCategoryObject('typeBinding', 'forceUnit', forceUnit)
_module_typeBindings.forceUnit = forceUnit

# Atomic simple type: {iFAB}torqueUnit
class torqueUnit (pyxb.binding.datatypes.normalizedString, pyxb.binding.basis.enumeration_mixin):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'torqueUnit')
    _XSDLocation = pyxb.utils.utility.Location('common.xsd', 200, 2)
    _Documentation = None
torqueUnit._CF_enumeration = pyxb.binding.facets.CF_enumeration(value_datatype=torqueUnit, enum_prefix=None)
torqueUnit.N_m = torqueUnit._CF_enumeration.addEnumeration(unicode_value='N m', tag='N_m')
torqueUnit._InitializeFacetMap(torqueUnit._CF_enumeration)
Namespace.addCategoryObject('typeBinding', 'torqueUnit', torqueUnit)
_module_typeBindings.torqueUnit = torqueUnit

# Atomic simple type: {iFAB}temperatureUnit
class temperatureUnit (pyxb.binding.datatypes.normalizedString, pyxb.binding.basis.enumeration_mixin):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'temperatureUnit')
    _XSDLocation = pyxb.utils.utility.Location('common.xsd', 206, 2)
    _Documentation = None
temperatureUnit._CF_enumeration = pyxb.binding.facets.CF_enumeration(value_datatype=temperatureUnit, enum_prefix=None)
temperatureUnit.F = temperatureUnit._CF_enumeration.addEnumeration(unicode_value='F', tag='F')
temperatureUnit.C = temperatureUnit._CF_enumeration.addEnumeration(unicode_value='C', tag='C')
temperatureUnit._InitializeFacetMap(temperatureUnit._CF_enumeration)
Namespace.addCategoryObject('typeBinding', 'temperatureUnit', temperatureUnit)
_module_typeBindings.temperatureUnit = temperatureUnit

# Atomic simple type: {iFAB}hardnessUnit
class hardnessUnit (pyxb.binding.datatypes.normalizedString, pyxb.binding.basis.enumeration_mixin):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'hardnessUnit')
    _XSDLocation = pyxb.utils.utility.Location('common.xsd', 213, 2)
    _Documentation = None
hardnessUnit._CF_enumeration = pyxb.binding.facets.CF_enumeration(value_datatype=hardnessUnit, enum_prefix=None)
hardnessUnit.bh = hardnessUnit._CF_enumeration.addEnumeration(unicode_value='bh', tag='bh')
hardnessUnit._InitializeFacetMap(hardnessUnit._CF_enumeration)
Namespace.addCategoryObject('typeBinding', 'hardnessUnit', hardnessUnit)
_module_typeBindings.hardnessUnit = hardnessUnit

# Atomic simple type: {iFAB}angleUnit
class angleUnit (pyxb.binding.datatypes.normalizedString, pyxb.binding.basis.enumeration_mixin):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'angleUnit')
    _XSDLocation = pyxb.utils.utility.Location('common.xsd', 219, 2)
    _Documentation = None
angleUnit._CF_enumeration = pyxb.binding.facets.CF_enumeration(value_datatype=angleUnit, enum_prefix=None)
angleUnit.degrees = angleUnit._CF_enumeration.addEnumeration(unicode_value='degrees', tag='degrees')
angleUnit.radians = angleUnit._CF_enumeration.addEnumeration(unicode_value='radians', tag='radians')
angleUnit._InitializeFacetMap(angleUnit._CF_enumeration)
Namespace.addCategoryObject('typeBinding', 'angleUnit', angleUnit)
_module_typeBindings.angleUnit = angleUnit

# Atomic simple type: {iFAB}pressureUnit
class pressureUnit (pyxb.binding.datatypes.normalizedString, pyxb.binding.basis.enumeration_mixin):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'pressureUnit')
    _XSDLocation = pyxb.utils.utility.Location('common.xsd', 226, 2)
    _Documentation = None
pressureUnit._CF_enumeration = pyxb.binding.facets.CF_enumeration(value_datatype=pressureUnit, enum_prefix=None)
pressureUnit.Pa = pressureUnit._CF_enumeration.addEnumeration(unicode_value='Pa', tag='Pa')
pressureUnit.kPa = pressureUnit._CF_enumeration.addEnumeration(unicode_value='kPa', tag='kPa')
pressureUnit.MPa = pressureUnit._CF_enumeration.addEnumeration(unicode_value='MPa', tag='MPa')
pressureUnit.GPa = pressureUnit._CF_enumeration.addEnumeration(unicode_value='GPa', tag='GPa')
pressureUnit.psi = pressureUnit._CF_enumeration.addEnumeration(unicode_value='psi', tag='psi')
pressureUnit.ksi = pressureUnit._CF_enumeration.addEnumeration(unicode_value='ksi', tag='ksi')
pressureUnit._InitializeFacetMap(pressureUnit._CF_enumeration)
Namespace.addCategoryObject('typeBinding', 'pressureUnit', pressureUnit)
_module_typeBindings.pressureUnit = pressureUnit

# Complex type {iFAB}assemblyDetails with content type ELEMENT_ONLY
class assemblyDetails_ (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {iFAB}assemblyDetails with content type ELEMENT_ONLY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'assemblyDetails')
    _XSDLocation = pyxb.utils.utility.Location('AssemblyDetails.xsd', 14, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.anyType
    
    # Element assemblyDetail uses Python identifier assemblyDetail
    __assemblyDetail = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'assemblyDetail'), 'assemblyDetail', '__iFAB_assemblyDetails__assemblyDetail', True, pyxb.utils.utility.Location('AssemblyDetails.xsd', 16, 6), )

    
    assemblyDetail = property(__assemblyDetail.value, __assemblyDetail.set, None, None)

    _ElementMap.update({
        __assemblyDetail.name() : __assemblyDetail
    })
    _AttributeMap.update({
        
    })
_module_typeBindings.assemblyDetails_ = assemblyDetails_
Namespace.addCategoryObject('typeBinding', 'assemblyDetails', assemblyDetails_)


# Complex type {iFAB}incidentalContact with content type EMPTY
class incidentalContact (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {iFAB}incidentalContact with content type EMPTY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_EMPTY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'incidentalContact')
    _XSDLocation = pyxb.utils.utility.Location('AssemblyDetails.xsd', 38, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.anyType
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        
    })
_module_typeBindings.incidentalContact = incidentalContact
Namespace.addCategoryObject('typeBinding', 'incidentalContact', incidentalContact)


# Complex type {iFAB}mechanical with content type ELEMENT_ONLY
class mechanical (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {iFAB}mechanical with content type ELEMENT_ONLY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'mechanical')
    _XSDLocation = pyxb.utils.utility.Location('AssemblyDetails.xsd', 43, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.anyType
    
    # Element linkingPart uses Python identifier linkingPart
    __linkingPart = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'linkingPart'), 'linkingPart', '__iFAB_mechanical_linkingPart', True, pyxb.utils.utility.Location('AssemblyDetails.xsd', 45, 6), )

    
    linkingPart = property(__linkingPart.value, __linkingPart.set, None, None)

    
    # Element notes uses Python identifier notes
    __notes = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'notes'), 'notes', '__iFAB_mechanical_notes', False, pyxb.utils.utility.Location('AssemblyDetails.xsd', 46, 6), )

    
    notes = property(__notes.value, __notes.set, None, None)

    
    # Element fasteningMethod uses Python identifier fasteningMethod
    __fasteningMethod = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'fasteningMethod'), 'fasteningMethod', '__iFAB_mechanical_fasteningMethod', False, pyxb.utils.utility.Location('AssemblyDetails.xsd', 47, 6), )

    
    fasteningMethod = property(__fasteningMethod.value, __fasteningMethod.set, None, None)

    
    # Element fasteningQuantity uses Python identifier fasteningQuantity
    __fasteningQuantity = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'fasteningQuantity'), 'fasteningQuantity', '__iFAB_mechanical_fasteningQuantity', False, pyxb.utils.utility.Location('AssemblyDetails.xsd', 48, 6), )

    
    fasteningQuantity = property(__fasteningQuantity.value, __fasteningQuantity.set, None, None)

    
    # Element torque uses Python identifier torque
    __torque = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'torque'), 'torque', '__iFAB_mechanical_torque', False, pyxb.utils.utility.Location('AssemblyDetails.xsd', 49, 6), )

    
    torque = property(__torque.value, __torque.set, None, None)

    
    # Element force uses Python identifier force
    __force = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'force'), 'force', '__iFAB_mechanical_force', False, pyxb.utils.utility.Location('AssemblyDetails.xsd', 50, 6), )

    
    force = property(__force.value, __force.set, None, None)

    _ElementMap.update({
        __linkingPart.name() : __linkingPart,
        __notes.name() : __notes,
        __fasteningMethod.name() : __fasteningMethod,
        __fasteningQuantity.name() : __fasteningQuantity,
        __torque.name() : __torque,
        __force.name() : __force
    })
    _AttributeMap.update({
        
    })
_module_typeBindings.mechanical = mechanical
Namespace.addCategoryObject('typeBinding', 'mechanical', mechanical)


# Complex type {iFAB}welded with content type ELEMENT_ONLY
class welded (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {iFAB}welded with content type ELEMENT_ONLY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'welded')
    _XSDLocation = pyxb.utils.utility.Location('AssemblyDetails.xsd', 54, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.anyType
    
    # Element linkingPart uses Python identifier linkingPart
    __linkingPart = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'linkingPart'), 'linkingPart', '__iFAB_welded_linkingPart', True, pyxb.utils.utility.Location('AssemblyDetails.xsd', 56, 6), )

    
    linkingPart = property(__linkingPart.value, __linkingPart.set, None, None)

    
    # Element notes uses Python identifier notes
    __notes = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'notes'), 'notes', '__iFAB_welded_notes', False, pyxb.utils.utility.Location('AssemblyDetails.xsd', 57, 6), )

    
    notes = property(__notes.value, __notes.set, None, None)

    
    # Element length uses Python identifier length
    __length = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'length'), 'length', '__iFAB_welded_length', False, pyxb.utils.utility.Location('AssemblyDetails.xsd', 58, 6), )

    
    length = property(__length.value, __length.set, None, None)

    
    # Element jointType uses Python identifier jointType
    __jointType = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'jointType'), 'jointType', '__iFAB_welded_jointType', False, pyxb.utils.utility.Location('AssemblyDetails.xsd', 59, 6), )

    
    jointType = property(__jointType.value, __jointType.set, None, None)

    
    # Element weldType uses Python identifier weldType
    __weldType = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'weldType'), 'weldType', '__iFAB_welded_weldType', False, pyxb.utils.utility.Location('AssemblyDetails.xsd', 60, 6), )

    
    weldType = property(__weldType.value, __weldType.set, None, None)

    
    # Element weldPenetration uses Python identifier weldPenetration
    __weldPenetration = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'weldPenetration'), 'weldPenetration', '__iFAB_welded_weldPenetration', False, pyxb.utils.utility.Location('AssemblyDetails.xsd', 61, 6), )

    
    weldPenetration = property(__weldPenetration.value, __weldPenetration.set, None, None)

    
    # Element twoSided uses Python identifier twoSided
    __twoSided = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'twoSided'), 'twoSided', '__iFAB_welded_twoSided', False, pyxb.utils.utility.Location('AssemblyDetails.xsd', 62, 6), )

    
    twoSided = property(__twoSided.value, __twoSided.set, None, None)

    
    # Element inspectionRequirement uses Python identifier inspectionRequirement
    __inspectionRequirement = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'inspectionRequirement'), 'inspectionRequirement', '__iFAB_welded_inspectionRequirement', False, pyxb.utils.utility.Location('AssemblyDetails.xsd', 63, 6), )

    
    inspectionRequirement = property(__inspectionRequirement.value, __inspectionRequirement.set, None, None)

    
    # Element part1Thickness uses Python identifier part1Thickness
    __part1Thickness = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'part1Thickness'), 'part1Thickness', '__iFAB_welded_part1Thickness', False, pyxb.utils.utility.Location('AssemblyDetails.xsd', 64, 6), )

    
    part1Thickness = property(__part1Thickness.value, __part1Thickness.set, None, None)

    
    # Element part1Material uses Python identifier part1Material
    __part1Material = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'part1Material'), 'part1Material', '__iFAB_welded_part1Material', False, pyxb.utils.utility.Location('AssemblyDetails.xsd', 65, 6), )

    
    part1Material = property(__part1Material.value, __part1Material.set, None, None)

    
    # Element part2Thickness uses Python identifier part2Thickness
    __part2Thickness = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'part2Thickness'), 'part2Thickness', '__iFAB_welded_part2Thickness', False, pyxb.utils.utility.Location('AssemblyDetails.xsd', 66, 6), )

    
    part2Thickness = property(__part2Thickness.value, __part2Thickness.set, None, None)

    
    # Element part2Material uses Python identifier part2Material
    __part2Material = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'part2Material'), 'part2Material', '__iFAB_welded_part2Material', False, pyxb.utils.utility.Location('AssemblyDetails.xsd', 67, 6), )

    
    part2Material = property(__part2Material.value, __part2Material.set, None, None)

    _ElementMap.update({
        __linkingPart.name() : __linkingPart,
        __notes.name() : __notes,
        __length.name() : __length,
        __jointType.name() : __jointType,
        __weldType.name() : __weldType,
        __weldPenetration.name() : __weldPenetration,
        __twoSided.name() : __twoSided,
        __inspectionRequirement.name() : __inspectionRequirement,
        __part1Thickness.name() : __part1Thickness,
        __part1Material.name() : __part1Material,
        __part2Thickness.name() : __part2Thickness,
        __part2Material.name() : __part2Material
    })
    _AttributeMap.update({
        
    })
_module_typeBindings.welded = welded
Namespace.addCategoryObject('typeBinding', 'welded', welded)


# Complex type {iFAB}soldered with content type ELEMENT_ONLY
class soldered (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {iFAB}soldered with content type ELEMENT_ONLY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'soldered')
    _XSDLocation = pyxb.utils.utility.Location('AssemblyDetails.xsd', 71, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.anyType
    
    # Element linkingPart uses Python identifier linkingPart
    __linkingPart = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'linkingPart'), 'linkingPart', '__iFAB_soldered_linkingPart', True, pyxb.utils.utility.Location('AssemblyDetails.xsd', 73, 6), )

    
    linkingPart = property(__linkingPart.value, __linkingPart.set, None, None)

    
    # Element notes uses Python identifier notes
    __notes = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'notes'), 'notes', '__iFAB_soldered_notes', False, pyxb.utils.utility.Location('AssemblyDetails.xsd', 74, 6), )

    
    notes = property(__notes.value, __notes.set, None, None)

    
    # Element length uses Python identifier length
    __length = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'length'), 'length', '__iFAB_soldered_length', False, pyxb.utils.utility.Location('AssemblyDetails.xsd', 75, 6), )

    
    length = property(__length.value, __length.set, None, None)

    
    # Element fillerMaterial uses Python identifier fillerMaterial
    __fillerMaterial = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'fillerMaterial'), 'fillerMaterial', '__iFAB_soldered_fillerMaterial', False, pyxb.utils.utility.Location('AssemblyDetails.xsd', 76, 6), )

    
    fillerMaterial = property(__fillerMaterial.value, __fillerMaterial.set, None, None)

    
    # Element fluxMaterial uses Python identifier fluxMaterial
    __fluxMaterial = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'fluxMaterial'), 'fluxMaterial', '__iFAB_soldered_fluxMaterial', False, pyxb.utils.utility.Location('AssemblyDetails.xsd', 77, 6), )

    
    fluxMaterial = property(__fluxMaterial.value, __fluxMaterial.set, None, None)

    _ElementMap.update({
        __linkingPart.name() : __linkingPart,
        __notes.name() : __notes,
        __length.name() : __length,
        __fillerMaterial.name() : __fillerMaterial,
        __fluxMaterial.name() : __fluxMaterial
    })
    _AttributeMap.update({
        
    })
_module_typeBindings.soldered = soldered
Namespace.addCategoryObject('typeBinding', 'soldered', soldered)


# Complex type {iFAB}brazed with content type ELEMENT_ONLY
class brazed (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {iFAB}brazed with content type ELEMENT_ONLY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'brazed')
    _XSDLocation = pyxb.utils.utility.Location('AssemblyDetails.xsd', 81, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.anyType
    
    # Element linkingPart uses Python identifier linkingPart
    __linkingPart = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'linkingPart'), 'linkingPart', '__iFAB_brazed_linkingPart', True, pyxb.utils.utility.Location('AssemblyDetails.xsd', 83, 6), )

    
    linkingPart = property(__linkingPart.value, __linkingPart.set, None, None)

    
    # Element notes uses Python identifier notes
    __notes = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'notes'), 'notes', '__iFAB_brazed_notes', False, pyxb.utils.utility.Location('AssemblyDetails.xsd', 84, 6), )

    
    notes = property(__notes.value, __notes.set, None, None)

    
    # Element length uses Python identifier length
    __length = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'length'), 'length', '__iFAB_brazed_length', False, pyxb.utils.utility.Location('AssemblyDetails.xsd', 85, 6), )

    
    length = property(__length.value, __length.set, None, None)

    
    # Element fillerMaterial uses Python identifier fillerMaterial
    __fillerMaterial = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'fillerMaterial'), 'fillerMaterial', '__iFAB_brazed_fillerMaterial', False, pyxb.utils.utility.Location('AssemblyDetails.xsd', 86, 6), )

    
    fillerMaterial = property(__fillerMaterial.value, __fillerMaterial.set, None, None)

    
    # Element fluxMaterial uses Python identifier fluxMaterial
    __fluxMaterial = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'fluxMaterial'), 'fluxMaterial', '__iFAB_brazed_fluxMaterial', False, pyxb.utils.utility.Location('AssemblyDetails.xsd', 87, 6), )

    
    fluxMaterial = property(__fluxMaterial.value, __fluxMaterial.set, None, None)

    _ElementMap.update({
        __linkingPart.name() : __linkingPart,
        __notes.name() : __notes,
        __length.name() : __length,
        __fillerMaterial.name() : __fillerMaterial,
        __fluxMaterial.name() : __fluxMaterial
    })
    _AttributeMap.update({
        
    })
_module_typeBindings.brazed = brazed
Namespace.addCategoryObject('typeBinding', 'brazed', brazed)


# Complex type {iFAB}glued with content type ELEMENT_ONLY
class glued (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {iFAB}glued with content type ELEMENT_ONLY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'glued')
    _XSDLocation = pyxb.utils.utility.Location('AssemblyDetails.xsd', 91, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.anyType
    
    # Element linkingPart uses Python identifier linkingPart
    __linkingPart = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'linkingPart'), 'linkingPart', '__iFAB_glued_linkingPart', True, pyxb.utils.utility.Location('AssemblyDetails.xsd', 93, 6), )

    
    linkingPart = property(__linkingPart.value, __linkingPart.set, None, None)

    
    # Element notes uses Python identifier notes
    __notes = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'notes'), 'notes', '__iFAB_glued_notes', False, pyxb.utils.utility.Location('AssemblyDetails.xsd', 94, 6), )

    
    notes = property(__notes.value, __notes.set, None, None)

    
    # Element length uses Python identifier length
    __length = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'length'), 'length', '__iFAB_glued_length', False, pyxb.utils.utility.Location('AssemblyDetails.xsd', 95, 6), )

    
    length = property(__length.value, __length.set, None, None)

    
    # Element volume uses Python identifier volume
    __volume = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'volume'), 'volume', '__iFAB_glued_volume', False, pyxb.utils.utility.Location('AssemblyDetails.xsd', 96, 6), )

    
    volume = property(__volume.value, __volume.set, None, None)

    
    # Element material uses Python identifier material
    __material = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'material'), 'material', '__iFAB_glued_material', False, pyxb.utils.utility.Location('AssemblyDetails.xsd', 97, 6), )

    
    material = property(__material.value, __material.set, None, None)

    _ElementMap.update({
        __linkingPart.name() : __linkingPart,
        __notes.name() : __notes,
        __length.name() : __length,
        __volume.name() : __volume,
        __material.name() : __material
    })
    _AttributeMap.update({
        
    })
_module_typeBindings.glued = glued
Namespace.addCategoryObject('typeBinding', 'glued', glued)


# Complex type {iFAB}assemblyDetail with content type ELEMENT_ONLY
class assemblyDetail (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {iFAB}assemblyDetail with content type ELEMENT_ONLY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'assemblyDetail')
    _XSDLocation = pyxb.utils.utility.Location('AssemblyDetails.xsd', 20, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.anyType
    
    # Element name uses Python identifier name
    __name = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'name'), 'name', '__iFAB_assemblyDetail_name', False, pyxb.utils.utility.Location('AssemblyDetails.xsd', 22, 6), )

    
    name = property(__name.value, __name.set, None, None)

    
    # Element part1 uses Python identifier part1
    __part1 = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'part1'), 'part1', '__iFAB_assemblyDetail_part1', False, pyxb.utils.utility.Location('AssemblyDetails.xsd', 23, 6), )

    
    part1 = property(__part1.value, __part1.set, None, None)

    
    # Element part2 uses Python identifier part2
    __part2 = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'part2'), 'part2', '__iFAB_assemblyDetail_part2', False, pyxb.utils.utility.Location('AssemblyDetails.xsd', 24, 6), )

    
    part2 = property(__part2.value, __part2.set, None, None)

    
    # Element description uses Python identifier description
    __description = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'description'), 'description', '__iFAB_assemblyDetail_description', False, pyxb.utils.utility.Location('AssemblyDetails.xsd', 25, 6), )

    
    description = property(__description.value, __description.set, None, None)

    
    # Element incidentalContact uses Python identifier incidentalContact
    __incidentalContact = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'incidentalContact'), 'incidentalContact', '__iFAB_assemblyDetail_incidentalContact', True, pyxb.utils.utility.Location('AssemblyDetails.xsd', 27, 8), )

    
    incidentalContact = property(__incidentalContact.value, __incidentalContact.set, None, None)

    
    # Element mechanical uses Python identifier mechanical
    __mechanical = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'mechanical'), 'mechanical', '__iFAB_assemblyDetail_mechanical', True, pyxb.utils.utility.Location('AssemblyDetails.xsd', 28, 8), )

    
    mechanical = property(__mechanical.value, __mechanical.set, None, None)

    
    # Element welded uses Python identifier welded
    __welded = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'welded'), 'welded', '__iFAB_assemblyDetail_welded', True, pyxb.utils.utility.Location('AssemblyDetails.xsd', 29, 8), )

    
    welded = property(__welded.value, __welded.set, None, None)

    
    # Element soldered uses Python identifier soldered
    __soldered = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'soldered'), 'soldered', '__iFAB_assemblyDetail_soldered', True, pyxb.utils.utility.Location('AssemblyDetails.xsd', 30, 8), )

    
    soldered = property(__soldered.value, __soldered.set, None, None)

    
    # Element brazed uses Python identifier brazed
    __brazed = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'brazed'), 'brazed', '__iFAB_assemblyDetail_brazed', True, pyxb.utils.utility.Location('AssemblyDetails.xsd', 31, 8), )

    
    brazed = property(__brazed.value, __brazed.set, None, None)

    
    # Element glued uses Python identifier glued
    __glued = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'glued'), 'glued', '__iFAB_assemblyDetail_glued', True, pyxb.utils.utility.Location('AssemblyDetails.xsd', 32, 8), )

    
    glued = property(__glued.value, __glued.set, None, None)

    
    # Attribute {iFAB}id uses Python identifier id
    __id = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(Namespace, 'id'), 'id', '__iFAB_assemblyDetail_iFABid', _module_typeBindings.guid, required=True)
    __id._DeclarationLocation = pyxb.utils.utility.Location('common.xsd', 10, 2)
    __id._UseLocation = pyxb.utils.utility.Location('AssemblyDetails.xsd', 35, 4)
    
    id = property(__id.value, __id.set, None, None)

    _ElementMap.update({
        __name.name() : __name,
        __part1.name() : __part1,
        __part2.name() : __part2,
        __description.name() : __description,
        __incidentalContact.name() : __incidentalContact,
        __mechanical.name() : __mechanical,
        __welded.name() : __welded,
        __soldered.name() : __soldered,
        __brazed.name() : __brazed,
        __glued.name() : __glued
    })
    _AttributeMap.update({
        __id.name() : __id
    })
_module_typeBindings.assemblyDetail = assemblyDetail
Namespace.addCategoryObject('typeBinding', 'assemblyDetail', assemblyDetail)


# Complex type {iFAB}partReference with content type SIMPLE
class partReference (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {iFAB}partReference with content type SIMPLE"""
    _TypeDefinition = pyxb.binding.datatypes.string
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'partReference')
    _XSDLocation = pyxb.utils.utility.Location('AssemblyDetails.xsd', 163, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.string
    
    # Attribute {iFAB}id uses Python identifier id
    __id = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(Namespace, 'id'), 'id', '__iFAB_partReference_iFABid', _module_typeBindings.guid, required=True)
    __id._DeclarationLocation = pyxb.utils.utility.Location('common.xsd', 10, 2)
    __id._UseLocation = pyxb.utils.utility.Location('AssemblyDetails.xsd', 166, 8)
    
    id = property(__id.value, __id.set, None, None)

    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __id.name() : __id
    })
_module_typeBindings.partReference = partReference
Namespace.addCategoryObject('typeBinding', 'partReference', partReference)


# Complex type {iFAB}duration with content type SIMPLE
class duration (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {iFAB}duration with content type SIMPLE"""
    _TypeDefinition = nonNegativeDecimal
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'duration')
    _XSDLocation = pyxb.utils.utility.Location('common.xsd', 26, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is nonNegativeDecimal
    
    # Attribute unit uses Python identifier unit
    __unit = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, 'unit'), 'unit', '__iFAB_duration_unit', _module_typeBindings.timeUnit, unicode_default='day')
    __unit._DeclarationLocation = pyxb.utils.utility.Location('common.xsd', 29, 8)
    __unit._UseLocation = pyxb.utils.utility.Location('common.xsd', 29, 8)
    
    unit = property(__unit.value, __unit.set, None, None)

    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __unit.name() : __unit
    })
_module_typeBindings.duration = duration
Namespace.addCategoryObject('typeBinding', 'duration', duration)


# Complex type {iFAB}price with content type SIMPLE
class price (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {iFAB}price with content type SIMPLE"""
    _TypeDefinition = nonNegativeDecimal
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'price')
    _XSDLocation = pyxb.utils.utility.Location('common.xsd', 34, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is nonNegativeDecimal
    
    # Attribute currency uses Python identifier currency
    __currency = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, 'currency'), 'currency', '__iFAB_price_currency', _module_typeBindings.currency, unicode_default='USD')
    __currency._DeclarationLocation = pyxb.utils.utility.Location('common.xsd', 37, 8)
    __currency._UseLocation = pyxb.utils.utility.Location('common.xsd', 37, 8)
    
    currency = property(__currency.value, __currency.set, None, None)

    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __currency.name() : __currency
    })
_module_typeBindings.price = price
Namespace.addCategoryObject('typeBinding', 'price', price)


# Complex type {iFAB}length with content type SIMPLE
class length (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {iFAB}length with content type SIMPLE"""
    _TypeDefinition = nonNegativeDecimal
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'length')
    _XSDLocation = pyxb.utils.utility.Location('common.xsd', 42, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is nonNegativeDecimal
    
    # Attribute unit uses Python identifier unit
    __unit = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, 'unit'), 'unit', '__iFAB_length_unit', _module_typeBindings.lengthUnit, unicode_default='mm')
    __unit._DeclarationLocation = pyxb.utils.utility.Location('common.xsd', 45, 8)
    __unit._UseLocation = pyxb.utils.utility.Location('common.xsd', 45, 8)
    
    unit = property(__unit.value, __unit.set, None, None)

    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __unit.name() : __unit
    })
_module_typeBindings.length = length
Namespace.addCategoryObject('typeBinding', 'length', length)


# Complex type {iFAB}area with content type SIMPLE
class area (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {iFAB}area with content type SIMPLE"""
    _TypeDefinition = nonNegativeDecimal
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'area')
    _XSDLocation = pyxb.utils.utility.Location('common.xsd', 50, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is nonNegativeDecimal
    
    # Attribute unit uses Python identifier unit
    __unit = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, 'unit'), 'unit', '__iFAB_area_unit', _module_typeBindings.areaUnit, unicode_default='mm2')
    __unit._DeclarationLocation = pyxb.utils.utility.Location('common.xsd', 53, 8)
    __unit._UseLocation = pyxb.utils.utility.Location('common.xsd', 53, 8)
    
    unit = property(__unit.value, __unit.set, None, None)

    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __unit.name() : __unit
    })
_module_typeBindings.area = area
Namespace.addCategoryObject('typeBinding', 'area', area)


# Complex type {iFAB}volume with content type SIMPLE
class volume (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {iFAB}volume with content type SIMPLE"""
    _TypeDefinition = nonNegativeDecimal
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'volume')
    _XSDLocation = pyxb.utils.utility.Location('common.xsd', 58, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is nonNegativeDecimal
    
    # Attribute unit uses Python identifier unit
    __unit = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, 'unit'), 'unit', '__iFAB_volume_unit', _module_typeBindings.volumeUnit, unicode_default='mm3')
    __unit._DeclarationLocation = pyxb.utils.utility.Location('common.xsd', 61, 8)
    __unit._UseLocation = pyxb.utils.utility.Location('common.xsd', 61, 8)
    
    unit = property(__unit.value, __unit.set, None, None)

    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __unit.name() : __unit
    })
_module_typeBindings.volume = volume
Namespace.addCategoryObject('typeBinding', 'volume', volume)


# Complex type {iFAB}weight with content type SIMPLE
class weight (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {iFAB}weight with content type SIMPLE"""
    _TypeDefinition = nonNegativeDecimal
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'weight')
    _XSDLocation = pyxb.utils.utility.Location('common.xsd', 66, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is nonNegativeDecimal
    
    # Attribute unit uses Python identifier unit
    __unit = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, 'unit'), 'unit', '__iFAB_weight_unit', _module_typeBindings.massUnit, unicode_default='g')
    __unit._DeclarationLocation = pyxb.utils.utility.Location('common.xsd', 69, 8)
    __unit._UseLocation = pyxb.utils.utility.Location('common.xsd', 69, 8)
    
    unit = property(__unit.value, __unit.set, None, None)

    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __unit.name() : __unit
    })
_module_typeBindings.weight = weight
Namespace.addCategoryObject('typeBinding', 'weight', weight)


# Complex type {iFAB}force with content type SIMPLE
class force (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {iFAB}force with content type SIMPLE"""
    _TypeDefinition = nonNegativeDecimal
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'force')
    _XSDLocation = pyxb.utils.utility.Location('common.xsd', 74, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is nonNegativeDecimal
    
    # Attribute unit uses Python identifier unit
    __unit = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, 'unit'), 'unit', '__iFAB_force_unit', _module_typeBindings.forceUnit, unicode_default='N')
    __unit._DeclarationLocation = pyxb.utils.utility.Location('common.xsd', 77, 8)
    __unit._UseLocation = pyxb.utils.utility.Location('common.xsd', 77, 8)
    
    unit = property(__unit.value, __unit.set, None, None)

    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __unit.name() : __unit
    })
_module_typeBindings.force = force
Namespace.addCategoryObject('typeBinding', 'force', force)


# Complex type {iFAB}torque with content type SIMPLE
class torque (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {iFAB}torque with content type SIMPLE"""
    _TypeDefinition = nonNegativeDecimal
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'torque')
    _XSDLocation = pyxb.utils.utility.Location('common.xsd', 82, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is nonNegativeDecimal
    
    # Attribute unit uses Python identifier unit
    __unit = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, 'unit'), 'unit', '__iFAB_torque_unit', _module_typeBindings.torqueUnit, unicode_default='N m')
    __unit._DeclarationLocation = pyxb.utils.utility.Location('common.xsd', 85, 8)
    __unit._UseLocation = pyxb.utils.utility.Location('common.xsd', 85, 8)
    
    unit = property(__unit.value, __unit.set, None, None)

    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __unit.name() : __unit
    })
_module_typeBindings.torque = torque
Namespace.addCategoryObject('typeBinding', 'torque', torque)


# Complex type {iFAB}hardness with content type SIMPLE
class hardness (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {iFAB}hardness with content type SIMPLE"""
    _TypeDefinition = nonNegativeDecimal
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'hardness')
    _XSDLocation = pyxb.utils.utility.Location('common.xsd', 90, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is nonNegativeDecimal
    
    # Attribute unit uses Python identifier unit
    __unit = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, 'unit'), 'unit', '__iFAB_hardness_unit', _module_typeBindings.hardnessUnit, unicode_default='bh')
    __unit._DeclarationLocation = pyxb.utils.utility.Location('common.xsd', 93, 8)
    __unit._UseLocation = pyxb.utils.utility.Location('common.xsd', 93, 8)
    
    unit = property(__unit.value, __unit.set, None, None)

    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __unit.name() : __unit
    })
_module_typeBindings.hardness = hardness
Namespace.addCategoryObject('typeBinding', 'hardness', hardness)


# Complex type {iFAB}angle with content type SIMPLE
class angle (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {iFAB}angle with content type SIMPLE"""
    _TypeDefinition = nonNegativeDecimal
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'angle')
    _XSDLocation = pyxb.utils.utility.Location('common.xsd', 98, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is nonNegativeDecimal
    
    # Attribute unit uses Python identifier unit
    __unit = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, 'unit'), 'unit', '__iFAB_angle_unit', _module_typeBindings.angleUnit, unicode_default='degrees')
    __unit._DeclarationLocation = pyxb.utils.utility.Location('common.xsd', 101, 8)
    __unit._UseLocation = pyxb.utils.utility.Location('common.xsd', 101, 8)
    
    unit = property(__unit.value, __unit.set, None, None)

    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __unit.name() : __unit
    })
_module_typeBindings.angle = angle
Namespace.addCategoryObject('typeBinding', 'angle', angle)


# Complex type {iFAB}temperature with content type SIMPLE
class temperature (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {iFAB}temperature with content type SIMPLE"""
    _TypeDefinition = pyxb.binding.datatypes.decimal
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'temperature')
    _XSDLocation = pyxb.utils.utility.Location('common.xsd', 106, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.decimal
    
    # Attribute unit uses Python identifier unit
    __unit = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, 'unit'), 'unit', '__iFAB_temperature_unit', _module_typeBindings.temperatureUnit, unicode_default='C')
    __unit._DeclarationLocation = pyxb.utils.utility.Location('common.xsd', 109, 8)
    __unit._UseLocation = pyxb.utils.utility.Location('common.xsd', 109, 8)
    
    unit = property(__unit.value, __unit.set, None, None)

    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __unit.name() : __unit
    })
_module_typeBindings.temperature = temperature
Namespace.addCategoryObject('typeBinding', 'temperature', temperature)


# Complex type {iFAB}pressure with content type SIMPLE
class pressure (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {iFAB}pressure with content type SIMPLE"""
    _TypeDefinition = nonNegativeDecimal
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'pressure')
    _XSDLocation = pyxb.utils.utility.Location('common.xsd', 114, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is nonNegativeDecimal
    
    # Attribute unit uses Python identifier unit
    __unit = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, 'unit'), 'unit', '__iFAB_pressure_unit', _module_typeBindings.pressureUnit, unicode_default='Pa')
    __unit._DeclarationLocation = pyxb.utils.utility.Location('common.xsd', 117, 8)
    __unit._UseLocation = pyxb.utils.utility.Location('common.xsd', 117, 8)
    
    unit = property(__unit.value, __unit.set, None, None)

    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __unit.name() : __unit
    })
_module_typeBindings.pressure = pressure
Namespace.addCategoryObject('typeBinding', 'pressure', pressure)


assemblyDetails = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, 'assemblyDetails'), assemblyDetails_, location=pyxb.utils.utility.Location('AssemblyDetails.xsd', 12, 2))
Namespace.addCategoryObject('elementBinding', assemblyDetails.name().localName(), assemblyDetails)



assemblyDetails_._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'assemblyDetail'), assemblyDetail, scope=assemblyDetails_, location=pyxb.utils.utility.Location('AssemblyDetails.xsd', 16, 6)))

def _BuildAutomaton ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton
    del _BuildAutomaton
    import pyxb.utils.fac as fac

    counters = set()
    cc_0 = fac.CounterCondition(min=0, max=None, metadata=pyxb.utils.utility.Location('AssemblyDetails.xsd', 15, 4))
    counters.add(cc_0)
    states = []
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_0, False))
    symbol = pyxb.binding.content.ElementUse(assemblyDetails_._UseForTag(pyxb.namespace.ExpandedName(None, 'assemblyDetail')), pyxb.utils.utility.Location('AssemblyDetails.xsd', 16, 6))
    st_0 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_0)
    transitions = []
    transitions.append(fac.Transition(st_0, [
        fac.UpdateInstruction(cc_0, True) ]))
    st_0._set_transitionSet(transitions)
    return fac.Automaton(states, counters, True, containing_state=None)
assemblyDetails_._Automaton = _BuildAutomaton()




mechanical._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'linkingPart'), partReference, scope=mechanical, location=pyxb.utils.utility.Location('AssemblyDetails.xsd', 45, 6)))

mechanical._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'notes'), pyxb.binding.datatypes.string, scope=mechanical, location=pyxb.utils.utility.Location('AssemblyDetails.xsd', 46, 6)))

mechanical._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'fasteningMethod'), fasteningMethod, scope=mechanical, location=pyxb.utils.utility.Location('AssemblyDetails.xsd', 47, 6)))

mechanical._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'fasteningQuantity'), pyxb.binding.datatypes.positiveInteger, scope=mechanical, location=pyxb.utils.utility.Location('AssemblyDetails.xsd', 48, 6), unicode_default='1'))

mechanical._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'torque'), torque, scope=mechanical, location=pyxb.utils.utility.Location('AssemblyDetails.xsd', 49, 6)))

mechanical._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'force'), force, scope=mechanical, location=pyxb.utils.utility.Location('AssemblyDetails.xsd', 50, 6)))

def _BuildAutomaton_ ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton_
    del _BuildAutomaton_
    import pyxb.utils.fac as fac

    counters = set()
    cc_0 = fac.CounterCondition(min=0, max=None, metadata=pyxb.utils.utility.Location('AssemblyDetails.xsd', 45, 6))
    counters.add(cc_0)
    cc_1 = fac.CounterCondition(min=0, max=1, metadata=pyxb.utils.utility.Location('AssemblyDetails.xsd', 46, 6))
    counters.add(cc_1)
    cc_2 = fac.CounterCondition(min=0, max=1, metadata=pyxb.utils.utility.Location('AssemblyDetails.xsd', 48, 6))
    counters.add(cc_2)
    cc_3 = fac.CounterCondition(min=0, max=1, metadata=pyxb.utils.utility.Location('AssemblyDetails.xsd', 49, 6))
    counters.add(cc_3)
    cc_4 = fac.CounterCondition(min=0, max=1, metadata=pyxb.utils.utility.Location('AssemblyDetails.xsd', 50, 6))
    counters.add(cc_4)
    states = []
    final_update = None
    symbol = pyxb.binding.content.ElementUse(mechanical._UseForTag(pyxb.namespace.ExpandedName(None, 'linkingPart')), pyxb.utils.utility.Location('AssemblyDetails.xsd', 45, 6))
    st_0 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_0)
    final_update = None
    symbol = pyxb.binding.content.ElementUse(mechanical._UseForTag(pyxb.namespace.ExpandedName(None, 'notes')), pyxb.utils.utility.Location('AssemblyDetails.xsd', 46, 6))
    st_1 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_1)
    final_update = set()
    symbol = pyxb.binding.content.ElementUse(mechanical._UseForTag(pyxb.namespace.ExpandedName(None, 'fasteningMethod')), pyxb.utils.utility.Location('AssemblyDetails.xsd', 47, 6))
    st_2 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_2)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_2, False))
    symbol = pyxb.binding.content.ElementUse(mechanical._UseForTag(pyxb.namespace.ExpandedName(None, 'fasteningQuantity')), pyxb.utils.utility.Location('AssemblyDetails.xsd', 48, 6))
    st_3 = fac.State(symbol, is_initial=False, final_update=final_update, is_unordered_catenation=False)
    states.append(st_3)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_3, False))
    symbol = pyxb.binding.content.ElementUse(mechanical._UseForTag(pyxb.namespace.ExpandedName(None, 'torque')), pyxb.utils.utility.Location('AssemblyDetails.xsd', 49, 6))
    st_4 = fac.State(symbol, is_initial=False, final_update=final_update, is_unordered_catenation=False)
    states.append(st_4)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_4, False))
    symbol = pyxb.binding.content.ElementUse(mechanical._UseForTag(pyxb.namespace.ExpandedName(None, 'force')), pyxb.utils.utility.Location('AssemblyDetails.xsd', 50, 6))
    st_5 = fac.State(symbol, is_initial=False, final_update=final_update, is_unordered_catenation=False)
    states.append(st_5)
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
    transitions.append(fac.Transition(st_3, [
         ]))
    transitions.append(fac.Transition(st_4, [
         ]))
    transitions.append(fac.Transition(st_5, [
         ]))
    st_2._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_3, [
        fac.UpdateInstruction(cc_2, True) ]))
    transitions.append(fac.Transition(st_4, [
        fac.UpdateInstruction(cc_2, False) ]))
    transitions.append(fac.Transition(st_5, [
        fac.UpdateInstruction(cc_2, False) ]))
    st_3._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_4, [
        fac.UpdateInstruction(cc_3, True) ]))
    transitions.append(fac.Transition(st_5, [
        fac.UpdateInstruction(cc_3, False) ]))
    st_4._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_5, [
        fac.UpdateInstruction(cc_4, True) ]))
    st_5._set_transitionSet(transitions)
    return fac.Automaton(states, counters, False, containing_state=None)
mechanical._Automaton = _BuildAutomaton_()




welded._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'linkingPart'), partReference, scope=welded, location=pyxb.utils.utility.Location('AssemblyDetails.xsd', 56, 6)))

welded._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'notes'), pyxb.binding.datatypes.string, scope=welded, location=pyxb.utils.utility.Location('AssemblyDetails.xsd', 57, 6)))

welded._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'length'), length, scope=welded, location=pyxb.utils.utility.Location('AssemblyDetails.xsd', 58, 6)))

welded._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'jointType'), jointType, scope=welded, location=pyxb.utils.utility.Location('AssemblyDetails.xsd', 59, 6)))

welded._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'weldType'), weldType, scope=welded, location=pyxb.utils.utility.Location('AssemblyDetails.xsd', 60, 6)))

welded._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'weldPenetration'), weldPenetration, scope=welded, location=pyxb.utils.utility.Location('AssemblyDetails.xsd', 61, 6)))

welded._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'twoSided'), pyxb.binding.datatypes.boolean, scope=welded, location=pyxb.utils.utility.Location('AssemblyDetails.xsd', 62, 6)))

welded._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'inspectionRequirement'), inspectionRequirement, scope=welded, location=pyxb.utils.utility.Location('AssemblyDetails.xsd', 63, 6)))

welded._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'part1Thickness'), length, scope=welded, location=pyxb.utils.utility.Location('AssemblyDetails.xsd', 64, 6)))

welded._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'part1Material'), baseMaterial, scope=welded, location=pyxb.utils.utility.Location('AssemblyDetails.xsd', 65, 6)))

welded._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'part2Thickness'), length, scope=welded, location=pyxb.utils.utility.Location('AssemblyDetails.xsd', 66, 6)))

welded._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'part2Material'), baseMaterial, scope=welded, location=pyxb.utils.utility.Location('AssemblyDetails.xsd', 67, 6)))

def _BuildAutomaton_2 ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton_2
    del _BuildAutomaton_2
    import pyxb.utils.fac as fac

    counters = set()
    cc_0 = fac.CounterCondition(min=0, max=None, metadata=pyxb.utils.utility.Location('AssemblyDetails.xsd', 56, 6))
    counters.add(cc_0)
    cc_1 = fac.CounterCondition(min=0, max=1, metadata=pyxb.utils.utility.Location('AssemblyDetails.xsd', 57, 6))
    counters.add(cc_1)
    states = []
    final_update = None
    symbol = pyxb.binding.content.ElementUse(welded._UseForTag(pyxb.namespace.ExpandedName(None, 'linkingPart')), pyxb.utils.utility.Location('AssemblyDetails.xsd', 56, 6))
    st_0 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_0)
    final_update = None
    symbol = pyxb.binding.content.ElementUse(welded._UseForTag(pyxb.namespace.ExpandedName(None, 'notes')), pyxb.utils.utility.Location('AssemblyDetails.xsd', 57, 6))
    st_1 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_1)
    final_update = None
    symbol = pyxb.binding.content.ElementUse(welded._UseForTag(pyxb.namespace.ExpandedName(None, 'length')), pyxb.utils.utility.Location('AssemblyDetails.xsd', 58, 6))
    st_2 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_2)
    final_update = None
    symbol = pyxb.binding.content.ElementUse(welded._UseForTag(pyxb.namespace.ExpandedName(None, 'jointType')), pyxb.utils.utility.Location('AssemblyDetails.xsd', 59, 6))
    st_3 = fac.State(symbol, is_initial=False, final_update=final_update, is_unordered_catenation=False)
    states.append(st_3)
    final_update = None
    symbol = pyxb.binding.content.ElementUse(welded._UseForTag(pyxb.namespace.ExpandedName(None, 'weldType')), pyxb.utils.utility.Location('AssemblyDetails.xsd', 60, 6))
    st_4 = fac.State(symbol, is_initial=False, final_update=final_update, is_unordered_catenation=False)
    states.append(st_4)
    final_update = None
    symbol = pyxb.binding.content.ElementUse(welded._UseForTag(pyxb.namespace.ExpandedName(None, 'weldPenetration')), pyxb.utils.utility.Location('AssemblyDetails.xsd', 61, 6))
    st_5 = fac.State(symbol, is_initial=False, final_update=final_update, is_unordered_catenation=False)
    states.append(st_5)
    final_update = None
    symbol = pyxb.binding.content.ElementUse(welded._UseForTag(pyxb.namespace.ExpandedName(None, 'twoSided')), pyxb.utils.utility.Location('AssemblyDetails.xsd', 62, 6))
    st_6 = fac.State(symbol, is_initial=False, final_update=final_update, is_unordered_catenation=False)
    states.append(st_6)
    final_update = None
    symbol = pyxb.binding.content.ElementUse(welded._UseForTag(pyxb.namespace.ExpandedName(None, 'inspectionRequirement')), pyxb.utils.utility.Location('AssemblyDetails.xsd', 63, 6))
    st_7 = fac.State(symbol, is_initial=False, final_update=final_update, is_unordered_catenation=False)
    states.append(st_7)
    final_update = None
    symbol = pyxb.binding.content.ElementUse(welded._UseForTag(pyxb.namespace.ExpandedName(None, 'part1Thickness')), pyxb.utils.utility.Location('AssemblyDetails.xsd', 64, 6))
    st_8 = fac.State(symbol, is_initial=False, final_update=final_update, is_unordered_catenation=False)
    states.append(st_8)
    final_update = None
    symbol = pyxb.binding.content.ElementUse(welded._UseForTag(pyxb.namespace.ExpandedName(None, 'part1Material')), pyxb.utils.utility.Location('AssemblyDetails.xsd', 65, 6))
    st_9 = fac.State(symbol, is_initial=False, final_update=final_update, is_unordered_catenation=False)
    states.append(st_9)
    final_update = None
    symbol = pyxb.binding.content.ElementUse(welded._UseForTag(pyxb.namespace.ExpandedName(None, 'part2Thickness')), pyxb.utils.utility.Location('AssemblyDetails.xsd', 66, 6))
    st_10 = fac.State(symbol, is_initial=False, final_update=final_update, is_unordered_catenation=False)
    states.append(st_10)
    final_update = set()
    symbol = pyxb.binding.content.ElementUse(welded._UseForTag(pyxb.namespace.ExpandedName(None, 'part2Material')), pyxb.utils.utility.Location('AssemblyDetails.xsd', 67, 6))
    st_11 = fac.State(symbol, is_initial=False, final_update=final_update, is_unordered_catenation=False)
    states.append(st_11)
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
    transitions.append(fac.Transition(st_3, [
         ]))
    st_2._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_4, [
         ]))
    st_3._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_5, [
         ]))
    st_4._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_6, [
         ]))
    st_5._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_7, [
         ]))
    st_6._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_8, [
         ]))
    st_7._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_9, [
         ]))
    st_8._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_10, [
         ]))
    st_9._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_11, [
         ]))
    st_10._set_transitionSet(transitions)
    transitions = []
    st_11._set_transitionSet(transitions)
    return fac.Automaton(states, counters, False, containing_state=None)
welded._Automaton = _BuildAutomaton_2()




soldered._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'linkingPart'), partReference, scope=soldered, location=pyxb.utils.utility.Location('AssemblyDetails.xsd', 73, 6)))

soldered._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'notes'), pyxb.binding.datatypes.string, scope=soldered, location=pyxb.utils.utility.Location('AssemblyDetails.xsd', 74, 6)))

soldered._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'length'), length, scope=soldered, location=pyxb.utils.utility.Location('AssemblyDetails.xsd', 75, 6)))

soldered._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'fillerMaterial'), fillerMaterial, scope=soldered, location=pyxb.utils.utility.Location('AssemblyDetails.xsd', 76, 6)))

soldered._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'fluxMaterial'), fluxMaterial, scope=soldered, location=pyxb.utils.utility.Location('AssemblyDetails.xsd', 77, 6)))

def _BuildAutomaton_3 ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton_3
    del _BuildAutomaton_3
    import pyxb.utils.fac as fac

    counters = set()
    cc_0 = fac.CounterCondition(min=0, max=None, metadata=pyxb.utils.utility.Location('AssemblyDetails.xsd', 73, 6))
    counters.add(cc_0)
    cc_1 = fac.CounterCondition(min=0, max=1, metadata=pyxb.utils.utility.Location('AssemblyDetails.xsd', 74, 6))
    counters.add(cc_1)
    states = []
    final_update = None
    symbol = pyxb.binding.content.ElementUse(soldered._UseForTag(pyxb.namespace.ExpandedName(None, 'linkingPart')), pyxb.utils.utility.Location('AssemblyDetails.xsd', 73, 6))
    st_0 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_0)
    final_update = None
    symbol = pyxb.binding.content.ElementUse(soldered._UseForTag(pyxb.namespace.ExpandedName(None, 'notes')), pyxb.utils.utility.Location('AssemblyDetails.xsd', 74, 6))
    st_1 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_1)
    final_update = None
    symbol = pyxb.binding.content.ElementUse(soldered._UseForTag(pyxb.namespace.ExpandedName(None, 'length')), pyxb.utils.utility.Location('AssemblyDetails.xsd', 75, 6))
    st_2 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_2)
    final_update = None
    symbol = pyxb.binding.content.ElementUse(soldered._UseForTag(pyxb.namespace.ExpandedName(None, 'fillerMaterial')), pyxb.utils.utility.Location('AssemblyDetails.xsd', 76, 6))
    st_3 = fac.State(symbol, is_initial=False, final_update=final_update, is_unordered_catenation=False)
    states.append(st_3)
    final_update = set()
    symbol = pyxb.binding.content.ElementUse(soldered._UseForTag(pyxb.namespace.ExpandedName(None, 'fluxMaterial')), pyxb.utils.utility.Location('AssemblyDetails.xsd', 77, 6))
    st_4 = fac.State(symbol, is_initial=False, final_update=final_update, is_unordered_catenation=False)
    states.append(st_4)
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
    transitions.append(fac.Transition(st_3, [
         ]))
    st_2._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_4, [
         ]))
    st_3._set_transitionSet(transitions)
    transitions = []
    st_4._set_transitionSet(transitions)
    return fac.Automaton(states, counters, False, containing_state=None)
soldered._Automaton = _BuildAutomaton_3()




brazed._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'linkingPart'), partReference, scope=brazed, location=pyxb.utils.utility.Location('AssemblyDetails.xsd', 83, 6)))

brazed._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'notes'), pyxb.binding.datatypes.string, scope=brazed, location=pyxb.utils.utility.Location('AssemblyDetails.xsd', 84, 6)))

brazed._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'length'), length, scope=brazed, location=pyxb.utils.utility.Location('AssemblyDetails.xsd', 85, 6)))

brazed._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'fillerMaterial'), fillerMaterial, scope=brazed, location=pyxb.utils.utility.Location('AssemblyDetails.xsd', 86, 6)))

brazed._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'fluxMaterial'), fluxMaterial, scope=brazed, location=pyxb.utils.utility.Location('AssemblyDetails.xsd', 87, 6)))

def _BuildAutomaton_4 ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton_4
    del _BuildAutomaton_4
    import pyxb.utils.fac as fac

    counters = set()
    cc_0 = fac.CounterCondition(min=0, max=None, metadata=pyxb.utils.utility.Location('AssemblyDetails.xsd', 83, 6))
    counters.add(cc_0)
    cc_1 = fac.CounterCondition(min=0, max=1, metadata=pyxb.utils.utility.Location('AssemblyDetails.xsd', 84, 6))
    counters.add(cc_1)
    states = []
    final_update = None
    symbol = pyxb.binding.content.ElementUse(brazed._UseForTag(pyxb.namespace.ExpandedName(None, 'linkingPart')), pyxb.utils.utility.Location('AssemblyDetails.xsd', 83, 6))
    st_0 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_0)
    final_update = None
    symbol = pyxb.binding.content.ElementUse(brazed._UseForTag(pyxb.namespace.ExpandedName(None, 'notes')), pyxb.utils.utility.Location('AssemblyDetails.xsd', 84, 6))
    st_1 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_1)
    final_update = None
    symbol = pyxb.binding.content.ElementUse(brazed._UseForTag(pyxb.namespace.ExpandedName(None, 'length')), pyxb.utils.utility.Location('AssemblyDetails.xsd', 85, 6))
    st_2 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_2)
    final_update = None
    symbol = pyxb.binding.content.ElementUse(brazed._UseForTag(pyxb.namespace.ExpandedName(None, 'fillerMaterial')), pyxb.utils.utility.Location('AssemblyDetails.xsd', 86, 6))
    st_3 = fac.State(symbol, is_initial=False, final_update=final_update, is_unordered_catenation=False)
    states.append(st_3)
    final_update = set()
    symbol = pyxb.binding.content.ElementUse(brazed._UseForTag(pyxb.namespace.ExpandedName(None, 'fluxMaterial')), pyxb.utils.utility.Location('AssemblyDetails.xsd', 87, 6))
    st_4 = fac.State(symbol, is_initial=False, final_update=final_update, is_unordered_catenation=False)
    states.append(st_4)
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
    transitions.append(fac.Transition(st_3, [
         ]))
    st_2._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_4, [
         ]))
    st_3._set_transitionSet(transitions)
    transitions = []
    st_4._set_transitionSet(transitions)
    return fac.Automaton(states, counters, False, containing_state=None)
brazed._Automaton = _BuildAutomaton_4()




glued._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'linkingPart'), partReference, scope=glued, location=pyxb.utils.utility.Location('AssemblyDetails.xsd', 93, 6)))

glued._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'notes'), pyxb.binding.datatypes.string, scope=glued, location=pyxb.utils.utility.Location('AssemblyDetails.xsd', 94, 6)))

glued._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'length'), length, scope=glued, location=pyxb.utils.utility.Location('AssemblyDetails.xsd', 95, 6)))

glued._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'volume'), volume, scope=glued, location=pyxb.utils.utility.Location('AssemblyDetails.xsd', 96, 6)))

glued._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'material'), glue, scope=glued, location=pyxb.utils.utility.Location('AssemblyDetails.xsd', 97, 6)))

def _BuildAutomaton_5 ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton_5
    del _BuildAutomaton_5
    import pyxb.utils.fac as fac

    counters = set()
    cc_0 = fac.CounterCondition(min=0, max=None, metadata=pyxb.utils.utility.Location('AssemblyDetails.xsd', 93, 6))
    counters.add(cc_0)
    cc_1 = fac.CounterCondition(min=0, max=1, metadata=pyxb.utils.utility.Location('AssemblyDetails.xsd', 94, 6))
    counters.add(cc_1)
    states = []
    final_update = None
    symbol = pyxb.binding.content.ElementUse(glued._UseForTag(pyxb.namespace.ExpandedName(None, 'linkingPart')), pyxb.utils.utility.Location('AssemblyDetails.xsd', 93, 6))
    st_0 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_0)
    final_update = None
    symbol = pyxb.binding.content.ElementUse(glued._UseForTag(pyxb.namespace.ExpandedName(None, 'notes')), pyxb.utils.utility.Location('AssemblyDetails.xsd', 94, 6))
    st_1 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_1)
    final_update = None
    symbol = pyxb.binding.content.ElementUse(glued._UseForTag(pyxb.namespace.ExpandedName(None, 'length')), pyxb.utils.utility.Location('AssemblyDetails.xsd', 95, 6))
    st_2 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_2)
    final_update = None
    symbol = pyxb.binding.content.ElementUse(glued._UseForTag(pyxb.namespace.ExpandedName(None, 'volume')), pyxb.utils.utility.Location('AssemblyDetails.xsd', 96, 6))
    st_3 = fac.State(symbol, is_initial=False, final_update=final_update, is_unordered_catenation=False)
    states.append(st_3)
    final_update = set()
    symbol = pyxb.binding.content.ElementUse(glued._UseForTag(pyxb.namespace.ExpandedName(None, 'material')), pyxb.utils.utility.Location('AssemblyDetails.xsd', 97, 6))
    st_4 = fac.State(symbol, is_initial=False, final_update=final_update, is_unordered_catenation=False)
    states.append(st_4)
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
    transitions.append(fac.Transition(st_3, [
         ]))
    st_2._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_4, [
         ]))
    st_3._set_transitionSet(transitions)
    transitions = []
    st_4._set_transitionSet(transitions)
    return fac.Automaton(states, counters, False, containing_state=None)
glued._Automaton = _BuildAutomaton_5()




assemblyDetail._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'name'), pyxb.binding.datatypes.string, scope=assemblyDetail, location=pyxb.utils.utility.Location('AssemblyDetails.xsd', 22, 6)))

assemblyDetail._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'part1'), partReference, scope=assemblyDetail, location=pyxb.utils.utility.Location('AssemblyDetails.xsd', 23, 6)))

assemblyDetail._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'part2'), partReference, scope=assemblyDetail, location=pyxb.utils.utility.Location('AssemblyDetails.xsd', 24, 6)))

assemblyDetail._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'description'), pyxb.binding.datatypes.string, scope=assemblyDetail, location=pyxb.utils.utility.Location('AssemblyDetails.xsd', 25, 6)))

assemblyDetail._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'incidentalContact'), incidentalContact, scope=assemblyDetail, location=pyxb.utils.utility.Location('AssemblyDetails.xsd', 27, 8)))

assemblyDetail._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'mechanical'), mechanical, scope=assemblyDetail, location=pyxb.utils.utility.Location('AssemblyDetails.xsd', 28, 8)))

assemblyDetail._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'welded'), welded, scope=assemblyDetail, location=pyxb.utils.utility.Location('AssemblyDetails.xsd', 29, 8)))

assemblyDetail._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'soldered'), soldered, scope=assemblyDetail, location=pyxb.utils.utility.Location('AssemblyDetails.xsd', 30, 8)))

assemblyDetail._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'brazed'), brazed, scope=assemblyDetail, location=pyxb.utils.utility.Location('AssemblyDetails.xsd', 31, 8)))

assemblyDetail._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'glued'), glued, scope=assemblyDetail, location=pyxb.utils.utility.Location('AssemblyDetails.xsd', 32, 8)))

def _BuildAutomaton_6 ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton_6
    del _BuildAutomaton_6
    import pyxb.utils.fac as fac

    counters = set()
    cc_0 = fac.CounterCondition(min=0, max=1, metadata=pyxb.utils.utility.Location('AssemblyDetails.xsd', 22, 6))
    counters.add(cc_0)
    cc_1 = fac.CounterCondition(min=0, max=1, metadata=pyxb.utils.utility.Location('AssemblyDetails.xsd', 25, 6))
    counters.add(cc_1)
    cc_2 = fac.CounterCondition(min=0, max=None, metadata=pyxb.utils.utility.Location('AssemblyDetails.xsd', 26, 6))
    counters.add(cc_2)
    states = []
    final_update = None
    symbol = pyxb.binding.content.ElementUse(assemblyDetail._UseForTag(pyxb.namespace.ExpandedName(None, 'name')), pyxb.utils.utility.Location('AssemblyDetails.xsd', 22, 6))
    st_0 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_0)
    final_update = None
    symbol = pyxb.binding.content.ElementUse(assemblyDetail._UseForTag(pyxb.namespace.ExpandedName(None, 'part1')), pyxb.utils.utility.Location('AssemblyDetails.xsd', 23, 6))
    st_1 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_1)
    final_update = set()
    symbol = pyxb.binding.content.ElementUse(assemblyDetail._UseForTag(pyxb.namespace.ExpandedName(None, 'part2')), pyxb.utils.utility.Location('AssemblyDetails.xsd', 24, 6))
    st_2 = fac.State(symbol, is_initial=False, final_update=final_update, is_unordered_catenation=False)
    states.append(st_2)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_1, False))
    symbol = pyxb.binding.content.ElementUse(assemblyDetail._UseForTag(pyxb.namespace.ExpandedName(None, 'description')), pyxb.utils.utility.Location('AssemblyDetails.xsd', 25, 6))
    st_3 = fac.State(symbol, is_initial=False, final_update=final_update, is_unordered_catenation=False)
    states.append(st_3)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_2, False))
    symbol = pyxb.binding.content.ElementUse(assemblyDetail._UseForTag(pyxb.namespace.ExpandedName(None, 'incidentalContact')), pyxb.utils.utility.Location('AssemblyDetails.xsd', 27, 8))
    st_4 = fac.State(symbol, is_initial=False, final_update=final_update, is_unordered_catenation=False)
    states.append(st_4)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_2, False))
    symbol = pyxb.binding.content.ElementUse(assemblyDetail._UseForTag(pyxb.namespace.ExpandedName(None, 'mechanical')), pyxb.utils.utility.Location('AssemblyDetails.xsd', 28, 8))
    st_5 = fac.State(symbol, is_initial=False, final_update=final_update, is_unordered_catenation=False)
    states.append(st_5)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_2, False))
    symbol = pyxb.binding.content.ElementUse(assemblyDetail._UseForTag(pyxb.namespace.ExpandedName(None, 'welded')), pyxb.utils.utility.Location('AssemblyDetails.xsd', 29, 8))
    st_6 = fac.State(symbol, is_initial=False, final_update=final_update, is_unordered_catenation=False)
    states.append(st_6)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_2, False))
    symbol = pyxb.binding.content.ElementUse(assemblyDetail._UseForTag(pyxb.namespace.ExpandedName(None, 'soldered')), pyxb.utils.utility.Location('AssemblyDetails.xsd', 30, 8))
    st_7 = fac.State(symbol, is_initial=False, final_update=final_update, is_unordered_catenation=False)
    states.append(st_7)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_2, False))
    symbol = pyxb.binding.content.ElementUse(assemblyDetail._UseForTag(pyxb.namespace.ExpandedName(None, 'brazed')), pyxb.utils.utility.Location('AssemblyDetails.xsd', 31, 8))
    st_8 = fac.State(symbol, is_initial=False, final_update=final_update, is_unordered_catenation=False)
    states.append(st_8)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_2, False))
    symbol = pyxb.binding.content.ElementUse(assemblyDetail._UseForTag(pyxb.namespace.ExpandedName(None, 'glued')), pyxb.utils.utility.Location('AssemblyDetails.xsd', 32, 8))
    st_9 = fac.State(symbol, is_initial=False, final_update=final_update, is_unordered_catenation=False)
    states.append(st_9)
    transitions = []
    transitions.append(fac.Transition(st_0, [
        fac.UpdateInstruction(cc_0, True) ]))
    transitions.append(fac.Transition(st_1, [
        fac.UpdateInstruction(cc_0, False) ]))
    st_0._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_2, [
         ]))
    st_1._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_3, [
         ]))
    transitions.append(fac.Transition(st_4, [
         ]))
    transitions.append(fac.Transition(st_5, [
         ]))
    transitions.append(fac.Transition(st_6, [
         ]))
    transitions.append(fac.Transition(st_7, [
         ]))
    transitions.append(fac.Transition(st_8, [
         ]))
    transitions.append(fac.Transition(st_9, [
         ]))
    st_2._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_3, [
        fac.UpdateInstruction(cc_1, True) ]))
    transitions.append(fac.Transition(st_4, [
        fac.UpdateInstruction(cc_1, False) ]))
    transitions.append(fac.Transition(st_5, [
        fac.UpdateInstruction(cc_1, False) ]))
    transitions.append(fac.Transition(st_6, [
        fac.UpdateInstruction(cc_1, False) ]))
    transitions.append(fac.Transition(st_7, [
        fac.UpdateInstruction(cc_1, False) ]))
    transitions.append(fac.Transition(st_8, [
        fac.UpdateInstruction(cc_1, False) ]))
    transitions.append(fac.Transition(st_9, [
        fac.UpdateInstruction(cc_1, False) ]))
    st_3._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_4, [
        fac.UpdateInstruction(cc_2, True) ]))
    transitions.append(fac.Transition(st_5, [
        fac.UpdateInstruction(cc_2, True) ]))
    transitions.append(fac.Transition(st_6, [
        fac.UpdateInstruction(cc_2, True) ]))
    transitions.append(fac.Transition(st_7, [
        fac.UpdateInstruction(cc_2, True) ]))
    transitions.append(fac.Transition(st_8, [
        fac.UpdateInstruction(cc_2, True) ]))
    transitions.append(fac.Transition(st_9, [
        fac.UpdateInstruction(cc_2, True) ]))
    st_4._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_4, [
        fac.UpdateInstruction(cc_2, True) ]))
    transitions.append(fac.Transition(st_5, [
        fac.UpdateInstruction(cc_2, True) ]))
    transitions.append(fac.Transition(st_6, [
        fac.UpdateInstruction(cc_2, True) ]))
    transitions.append(fac.Transition(st_7, [
        fac.UpdateInstruction(cc_2, True) ]))
    transitions.append(fac.Transition(st_8, [
        fac.UpdateInstruction(cc_2, True) ]))
    transitions.append(fac.Transition(st_9, [
        fac.UpdateInstruction(cc_2, True) ]))
    st_5._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_4, [
        fac.UpdateInstruction(cc_2, True) ]))
    transitions.append(fac.Transition(st_5, [
        fac.UpdateInstruction(cc_2, True) ]))
    transitions.append(fac.Transition(st_6, [
        fac.UpdateInstruction(cc_2, True) ]))
    transitions.append(fac.Transition(st_7, [
        fac.UpdateInstruction(cc_2, True) ]))
    transitions.append(fac.Transition(st_8, [
        fac.UpdateInstruction(cc_2, True) ]))
    transitions.append(fac.Transition(st_9, [
        fac.UpdateInstruction(cc_2, True) ]))
    st_6._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_4, [
        fac.UpdateInstruction(cc_2, True) ]))
    transitions.append(fac.Transition(st_5, [
        fac.UpdateInstruction(cc_2, True) ]))
    transitions.append(fac.Transition(st_6, [
        fac.UpdateInstruction(cc_2, True) ]))
    transitions.append(fac.Transition(st_7, [
        fac.UpdateInstruction(cc_2, True) ]))
    transitions.append(fac.Transition(st_8, [
        fac.UpdateInstruction(cc_2, True) ]))
    transitions.append(fac.Transition(st_9, [
        fac.UpdateInstruction(cc_2, True) ]))
    st_7._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_4, [
        fac.UpdateInstruction(cc_2, True) ]))
    transitions.append(fac.Transition(st_5, [
        fac.UpdateInstruction(cc_2, True) ]))
    transitions.append(fac.Transition(st_6, [
        fac.UpdateInstruction(cc_2, True) ]))
    transitions.append(fac.Transition(st_7, [
        fac.UpdateInstruction(cc_2, True) ]))
    transitions.append(fac.Transition(st_8, [
        fac.UpdateInstruction(cc_2, True) ]))
    transitions.append(fac.Transition(st_9, [
        fac.UpdateInstruction(cc_2, True) ]))
    st_8._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_4, [
        fac.UpdateInstruction(cc_2, True) ]))
    transitions.append(fac.Transition(st_5, [
        fac.UpdateInstruction(cc_2, True) ]))
    transitions.append(fac.Transition(st_6, [
        fac.UpdateInstruction(cc_2, True) ]))
    transitions.append(fac.Transition(st_7, [
        fac.UpdateInstruction(cc_2, True) ]))
    transitions.append(fac.Transition(st_8, [
        fac.UpdateInstruction(cc_2, True) ]))
    transitions.append(fac.Transition(st_9, [
        fac.UpdateInstruction(cc_2, True) ]))
    st_9._set_transitionSet(transitions)
    return fac.Automaton(states, counters, False, containing_state=None)
assemblyDetail._Automaton = _BuildAutomaton_6()

