# .\_iFAB.py
# -*- coding: utf-8 -*-
# PyXB bindings for NM:de45d6eea5b439c583053bcc995a5a07ca6f002d
# Generated 2023-02-15 10:02:24.587000 by PyXB version 1.2.3
# Namespace iFAB [xmlns:iFAB]

import pyxb
import pyxb.binding
import pyxb.binding.saxer
import io
import pyxb.utils.utility
import pyxb.utils.domutils
import sys

# Unique identifier for bindings created at the same time
_GenerationUID = pyxb.utils.utility.UniqueIdentifier('urn:uuid:22e1ec4f-ad4a-11ed-8eff-50e085b81351')

# Version of PyXB used to generate the bindings
_PyXBVersion = '1.2.3'
# Generated bindings are not compatible across PyXB versions
if pyxb.__version__ != _PyXBVersion:
    raise pyxb.PyXBVersionError(_PyXBVersion)

# Import bindings for namespaces imported into schema
import pyxb.binding.datatypes

# NOTE: All namespace declarations are reserved within the binding
Namespace = pyxb.namespace.NamespaceForURI(u'iFAB', create_if_missing=True)
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


# Atomic simple type: {iFAB}baseMaterial
class baseMaterial (pyxb.binding.datatypes.token, pyxb.binding.basis.enumeration_mixin):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'baseMaterial')
    _XSDLocation = pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 101, 2)
    _Documentation = None
baseMaterial._CF_enumeration = pyxb.binding.facets.CF_enumeration(value_datatype=baseMaterial, enum_prefix=None)
baseMaterial.Al = baseMaterial._CF_enumeration.addEnumeration(unicode_value=u'Al', tag=u'Al')
baseMaterial.Plain_Carbon_Steel = baseMaterial._CF_enumeration.addEnumeration(unicode_value=u'Plain Carbon Steel', tag=u'Plain_Carbon_Steel')
baseMaterial._InitializeFacetMap(baseMaterial._CF_enumeration)
Namespace.addCategoryObject('typeBinding', u'baseMaterial', baseMaterial)

# Atomic simple type: {iFAB}fillerMaterial
class fillerMaterial (pyxb.binding.datatypes.string):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'fillerMaterial')
    _XSDLocation = pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 108, 2)
    _Documentation = None
fillerMaterial._InitializeFacetMap()
Namespace.addCategoryObject('typeBinding', u'fillerMaterial', fillerMaterial)

# Atomic simple type: {iFAB}fluxMaterial
class fluxMaterial (pyxb.binding.datatypes.string):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'fluxMaterial')
    _XSDLocation = pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 112, 2)
    _Documentation = None
fluxMaterial._InitializeFacetMap()
Namespace.addCategoryObject('typeBinding', u'fluxMaterial', fluxMaterial)

# Atomic simple type: {iFAB}glue
class glue (pyxb.binding.datatypes.string):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'glue')
    _XSDLocation = pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 116, 2)
    _Documentation = None
glue._InitializeFacetMap()
Namespace.addCategoryObject('typeBinding', u'glue', glue)

# Atomic simple type: {iFAB}fasteningMethod
class fasteningMethod (pyxb.binding.datatypes.token, pyxb.binding.basis.enumeration_mixin):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'fasteningMethod')
    _XSDLocation = pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 120, 2)
    _Documentation = None
fasteningMethod._CF_enumeration = pyxb.binding.facets.CF_enumeration(value_datatype=fasteningMethod, enum_prefix=None)
fasteningMethod.Bolted = fasteningMethod._CF_enumeration.addEnumeration(unicode_value=u'Bolted', tag=u'Bolted')
fasteningMethod.Bolted_blind = fasteningMethod._CF_enumeration.addEnumeration(unicode_value=u'Bolted (blind)', tag=u'Bolted_blind')
fasteningMethod.Machined_Screw = fasteningMethod._CF_enumeration.addEnumeration(unicode_value=u'Machined Screw', tag=u'Machined_Screw')
fasteningMethod.Press_Fit = fasteningMethod._CF_enumeration.addEnumeration(unicode_value=u'Press Fit', tag=u'Press_Fit')
fasteningMethod.Snap_Fit = fasteningMethod._CF_enumeration.addEnumeration(unicode_value=u'Snap Fit', tag=u'Snap_Fit')
fasteningMethod.CrimpClamp_Fit = fasteningMethod._CF_enumeration.addEnumeration(unicode_value=u'Crimp/Clamp Fit', tag=u'CrimpClamp_Fit')
fasteningMethod._InitializeFacetMap(fasteningMethod._CF_enumeration)
Namespace.addCategoryObject('typeBinding', u'fasteningMethod', fasteningMethod)

# Atomic simple type: {iFAB}jointType
class jointType (pyxb.binding.datatypes.token, pyxb.binding.basis.enumeration_mixin):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'jointType')
    _XSDLocation = pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 131, 2)
    _Documentation = None
jointType._CF_enumeration = pyxb.binding.facets.CF_enumeration(value_datatype=jointType, enum_prefix=None)
jointType.Butt = jointType._CF_enumeration.addEnumeration(unicode_value=u'Butt', tag=u'Butt')
jointType.Corner = jointType._CF_enumeration.addEnumeration(unicode_value=u'Corner', tag=u'Corner')
jointType.Edge = jointType._CF_enumeration.addEnumeration(unicode_value=u'Edge', tag=u'Edge')
jointType.Lap = jointType._CF_enumeration.addEnumeration(unicode_value=u'Lap', tag=u'Lap')
jointType.Tee = jointType._CF_enumeration.addEnumeration(unicode_value=u'Tee', tag=u'Tee')
jointType._InitializeFacetMap(jointType._CF_enumeration)
Namespace.addCategoryObject('typeBinding', u'jointType', jointType)

# Atomic simple type: {iFAB}weldType
class weldType (pyxb.binding.datatypes.token, pyxb.binding.basis.enumeration_mixin):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'weldType')
    _XSDLocation = pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 141, 2)
    _Documentation = None
weldType._CF_enumeration = pyxb.binding.facets.CF_enumeration(value_datatype=weldType, enum_prefix=None)
weldType.Seam = weldType._CF_enumeration.addEnumeration(unicode_value=u'Seam', tag=u'Seam')
weldType.Stitch = weldType._CF_enumeration.addEnumeration(unicode_value=u'Stitch', tag=u'Stitch')
weldType.Spot = weldType._CF_enumeration.addEnumeration(unicode_value=u'Spot', tag=u'Spot')
weldType._InitializeFacetMap(weldType._CF_enumeration)
Namespace.addCategoryObject('typeBinding', u'weldType', weldType)

# Atomic simple type: {iFAB}weldPenetration
class weldPenetration (pyxb.binding.datatypes.token, pyxb.binding.basis.enumeration_mixin):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'weldPenetration')
    _XSDLocation = pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 149, 2)
    _Documentation = None
weldPenetration._CF_enumeration = pyxb.binding.facets.CF_enumeration(value_datatype=weldPenetration, enum_prefix=None)
weldPenetration.Full = weldPenetration._CF_enumeration.addEnumeration(unicode_value=u'Full', tag=u'Full')
weldPenetration.Partial = weldPenetration._CF_enumeration.addEnumeration(unicode_value=u'Partial', tag=u'Partial')
weldPenetration._InitializeFacetMap(weldPenetration._CF_enumeration)
Namespace.addCategoryObject('typeBinding', u'weldPenetration', weldPenetration)

# Atomic simple type: {iFAB}inspectionRequirement
class inspectionRequirement (pyxb.binding.datatypes.token, pyxb.binding.basis.enumeration_mixin):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'inspectionRequirement')
    _XSDLocation = pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 156, 2)
    _Documentation = None
inspectionRequirement._CF_enumeration = pyxb.binding.facets.CF_enumeration(value_datatype=inspectionRequirement, enum_prefix=None)
inspectionRequirement.Visual = inspectionRequirement._CF_enumeration.addEnumeration(unicode_value=u'Visual', tag=u'Visual')
inspectionRequirement.X_Ray = inspectionRequirement._CF_enumeration.addEnumeration(unicode_value=u'X-Ray', tag=u'X_Ray')
inspectionRequirement._InitializeFacetMap(inspectionRequirement._CF_enumeration)
Namespace.addCategoryObject('typeBinding', u'inspectionRequirement', inspectionRequirement)

# Atomic simple type: {iFAB}guid
class guid (pyxb.binding.datatypes.normalizedString):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'guid')
    _XSDLocation = pyxb.utils.utility.Location(u'common.xsd', 12, 2)
    _Documentation = None
guid._CF_pattern = pyxb.binding.facets.CF_pattern()
guid._CF_pattern.addPattern(pattern=u'[a-fA-F0-9]{8}-[a-fA-F0-9]{4}-[a-fA-F0-9]{4}-[a-fA-F0-9]{4}-[a-fA-F0-9]{12}')
guid._InitializeFacetMap(guid._CF_pattern)
Namespace.addCategoryObject('typeBinding', u'guid', guid)

# Atomic simple type: {iFAB}nonNegativeDecimal
class nonNegativeDecimal (pyxb.binding.datatypes.decimal):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'nonNegativeDecimal')
    _XSDLocation = pyxb.utils.utility.Location(u'common.xsd', 18, 2)
    _Documentation = None
nonNegativeDecimal._CF_minInclusive = pyxb.binding.facets.CF_minInclusive(value_datatype=nonNegativeDecimal, value=pyxb.binding.datatypes.decimal(0.0))
nonNegativeDecimal._InitializeFacetMap(nonNegativeDecimal._CF_minInclusive)
Namespace.addCategoryObject('typeBinding', u'nonNegativeDecimal', nonNegativeDecimal)

# Atomic simple type: {iFAB}currency
class currency (pyxb.binding.datatypes.normalizedString, pyxb.binding.basis.enumeration_mixin):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'currency')
    _XSDLocation = pyxb.utils.utility.Location(u'common.xsd', 122, 2)
    _Documentation = None
currency._CF_enumeration = pyxb.binding.facets.CF_enumeration(value_datatype=currency, enum_prefix=None)
currency.USD = currency._CF_enumeration.addEnumeration(unicode_value=u'USD', tag=u'USD')
currency._InitializeFacetMap(currency._CF_enumeration)
Namespace.addCategoryObject('typeBinding', u'currency', currency)

# Atomic simple type: {iFAB}timeUnit
class timeUnit (pyxb.binding.datatypes.normalizedString, pyxb.binding.basis.enumeration_mixin):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'timeUnit')
    _XSDLocation = pyxb.utils.utility.Location(u'common.xsd', 128, 2)
    _Documentation = None
timeUnit._CF_enumeration = pyxb.binding.facets.CF_enumeration(value_datatype=timeUnit, enum_prefix=None)
timeUnit.week = timeUnit._CF_enumeration.addEnumeration(unicode_value=u'week', tag=u'week')
timeUnit.day = timeUnit._CF_enumeration.addEnumeration(unicode_value=u'day', tag=u'day')
timeUnit.hr = timeUnit._CF_enumeration.addEnumeration(unicode_value=u'hr', tag=u'hr')
timeUnit.min = timeUnit._CF_enumeration.addEnumeration(unicode_value=u'min', tag=u'min')
timeUnit.sec = timeUnit._CF_enumeration.addEnumeration(unicode_value=u'sec', tag=u'sec')
timeUnit._InitializeFacetMap(timeUnit._CF_enumeration)
Namespace.addCategoryObject('typeBinding', u'timeUnit', timeUnit)

# Atomic simple type: {iFAB}lengthUnit
class lengthUnit (pyxb.binding.datatypes.normalizedString, pyxb.binding.basis.enumeration_mixin):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'lengthUnit')
    _XSDLocation = pyxb.utils.utility.Location(u'common.xsd', 138, 2)
    _Documentation = None
lengthUnit._CF_enumeration = pyxb.binding.facets.CF_enumeration(value_datatype=lengthUnit, enum_prefix=None)
lengthUnit.mile = lengthUnit._CF_enumeration.addEnumeration(unicode_value=u'mile', tag=u'mile')
lengthUnit.yd = lengthUnit._CF_enumeration.addEnumeration(unicode_value=u'yd', tag=u'yd')
lengthUnit.ft = lengthUnit._CF_enumeration.addEnumeration(unicode_value=u'ft', tag=u'ft')
lengthUnit.in_ = lengthUnit._CF_enumeration.addEnumeration(unicode_value=u'in', tag=u'in_')
lengthUnit.km = lengthUnit._CF_enumeration.addEnumeration(unicode_value=u'km', tag=u'km')
lengthUnit.m = lengthUnit._CF_enumeration.addEnumeration(unicode_value=u'm', tag=u'm')
lengthUnit.cm = lengthUnit._CF_enumeration.addEnumeration(unicode_value=u'cm', tag=u'cm')
lengthUnit.mm = lengthUnit._CF_enumeration.addEnumeration(unicode_value=u'mm', tag=u'mm')
lengthUnit.um = lengthUnit._CF_enumeration.addEnumeration(unicode_value=u'um', tag=u'um')
lengthUnit.nm = lengthUnit._CF_enumeration.addEnumeration(unicode_value=u'nm', tag=u'nm')
lengthUnit._InitializeFacetMap(lengthUnit._CF_enumeration)
Namespace.addCategoryObject('typeBinding', u'lengthUnit', lengthUnit)

# Atomic simple type: {iFAB}areaUnit
class areaUnit (pyxb.binding.datatypes.normalizedString, pyxb.binding.basis.enumeration_mixin):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'areaUnit')
    _XSDLocation = pyxb.utils.utility.Location(u'common.xsd', 153, 2)
    _Documentation = None
areaUnit._CF_enumeration = pyxb.binding.facets.CF_enumeration(value_datatype=areaUnit, enum_prefix=None)
areaUnit.in2 = areaUnit._CF_enumeration.addEnumeration(unicode_value=u'in2', tag=u'in2')
areaUnit.ft2 = areaUnit._CF_enumeration.addEnumeration(unicode_value=u'ft2', tag=u'ft2')
areaUnit.yd2 = areaUnit._CF_enumeration.addEnumeration(unicode_value=u'yd2', tag=u'yd2')
areaUnit.acre = areaUnit._CF_enumeration.addEnumeration(unicode_value=u'acre', tag=u'acre')
areaUnit.mile2 = areaUnit._CF_enumeration.addEnumeration(unicode_value=u'mile2', tag=u'mile2')
areaUnit.mm2 = areaUnit._CF_enumeration.addEnumeration(unicode_value=u'mm2', tag=u'mm2')
areaUnit.cm2 = areaUnit._CF_enumeration.addEnumeration(unicode_value=u'cm2', tag=u'cm2')
areaUnit.m2 = areaUnit._CF_enumeration.addEnumeration(unicode_value=u'm2', tag=u'm2')
areaUnit.km2 = areaUnit._CF_enumeration.addEnumeration(unicode_value=u'km2', tag=u'km2')
areaUnit._InitializeFacetMap(areaUnit._CF_enumeration)
Namespace.addCategoryObject('typeBinding', u'areaUnit', areaUnit)

# Atomic simple type: {iFAB}volumeUnit
class volumeUnit (pyxb.binding.datatypes.normalizedString, pyxb.binding.basis.enumeration_mixin):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'volumeUnit')
    _XSDLocation = pyxb.utils.utility.Location(u'common.xsd', 167, 2)
    _Documentation = None
volumeUnit._CF_enumeration = pyxb.binding.facets.CF_enumeration(value_datatype=volumeUnit, enum_prefix=None)
volumeUnit.in3 = volumeUnit._CF_enumeration.addEnumeration(unicode_value=u'in3', tag=u'in3')
volumeUnit.ft3 = volumeUnit._CF_enumeration.addEnumeration(unicode_value=u'ft3', tag=u'ft3')
volumeUnit.fl_oz = volumeUnit._CF_enumeration.addEnumeration(unicode_value=u'fl oz', tag=u'fl_oz')
volumeUnit.pt = volumeUnit._CF_enumeration.addEnumeration(unicode_value=u'pt', tag=u'pt')
volumeUnit.gal = volumeUnit._CF_enumeration.addEnumeration(unicode_value=u'gal', tag=u'gal')
volumeUnit.mL = volumeUnit._CF_enumeration.addEnumeration(unicode_value=u'mL', tag=u'mL')
volumeUnit.L = volumeUnit._CF_enumeration.addEnumeration(unicode_value=u'L', tag=u'L')
volumeUnit.mm3 = volumeUnit._CF_enumeration.addEnumeration(unicode_value=u'mm3', tag=u'mm3')
volumeUnit.cm3 = volumeUnit._CF_enumeration.addEnumeration(unicode_value=u'cm3', tag=u'cm3')
volumeUnit.m3 = volumeUnit._CF_enumeration.addEnumeration(unicode_value=u'm3', tag=u'm3')
volumeUnit._InitializeFacetMap(volumeUnit._CF_enumeration)
Namespace.addCategoryObject('typeBinding', u'volumeUnit', volumeUnit)

# Atomic simple type: {iFAB}massUnit
class massUnit (pyxb.binding.datatypes.normalizedString, pyxb.binding.basis.enumeration_mixin):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'massUnit')
    _XSDLocation = pyxb.utils.utility.Location(u'common.xsd', 182, 2)
    _Documentation = None
massUnit._CF_enumeration = pyxb.binding.facets.CF_enumeration(value_datatype=massUnit, enum_prefix=None)
massUnit.oz = massUnit._CF_enumeration.addEnumeration(unicode_value=u'oz', tag=u'oz')
massUnit.lb = massUnit._CF_enumeration.addEnumeration(unicode_value=u'lb', tag=u'lb')
massUnit.ton = massUnit._CF_enumeration.addEnumeration(unicode_value=u'ton', tag=u'ton')
massUnit.mg = massUnit._CF_enumeration.addEnumeration(unicode_value=u'mg', tag=u'mg')
massUnit.g = massUnit._CF_enumeration.addEnumeration(unicode_value=u'g', tag=u'g')
massUnit.kg = massUnit._CF_enumeration.addEnumeration(unicode_value=u'kg', tag=u'kg')
massUnit.tonne = massUnit._CF_enumeration.addEnumeration(unicode_value=u'tonne', tag=u'tonne')
massUnit._InitializeFacetMap(massUnit._CF_enumeration)
Namespace.addCategoryObject('typeBinding', u'massUnit', massUnit)

# Atomic simple type: {iFAB}forceUnit
class forceUnit (pyxb.binding.datatypes.normalizedString, pyxb.binding.basis.enumeration_mixin):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'forceUnit')
    _XSDLocation = pyxb.utils.utility.Location(u'common.xsd', 194, 2)
    _Documentation = None
forceUnit._CF_enumeration = pyxb.binding.facets.CF_enumeration(value_datatype=forceUnit, enum_prefix=None)
forceUnit.N = forceUnit._CF_enumeration.addEnumeration(unicode_value=u'N', tag=u'N')
forceUnit._InitializeFacetMap(forceUnit._CF_enumeration)
Namespace.addCategoryObject('typeBinding', u'forceUnit', forceUnit)

# Atomic simple type: {iFAB}torqueUnit
class torqueUnit (pyxb.binding.datatypes.normalizedString, pyxb.binding.basis.enumeration_mixin):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'torqueUnit')
    _XSDLocation = pyxb.utils.utility.Location(u'common.xsd', 200, 2)
    _Documentation = None
torqueUnit._CF_enumeration = pyxb.binding.facets.CF_enumeration(value_datatype=torqueUnit, enum_prefix=None)
torqueUnit.N_m = torqueUnit._CF_enumeration.addEnumeration(unicode_value=u'N m', tag=u'N_m')
torqueUnit._InitializeFacetMap(torqueUnit._CF_enumeration)
Namespace.addCategoryObject('typeBinding', u'torqueUnit', torqueUnit)

# Atomic simple type: {iFAB}temperatureUnit
class temperatureUnit (pyxb.binding.datatypes.normalizedString, pyxb.binding.basis.enumeration_mixin):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'temperatureUnit')
    _XSDLocation = pyxb.utils.utility.Location(u'common.xsd', 206, 2)
    _Documentation = None
temperatureUnit._CF_enumeration = pyxb.binding.facets.CF_enumeration(value_datatype=temperatureUnit, enum_prefix=None)
temperatureUnit.F = temperatureUnit._CF_enumeration.addEnumeration(unicode_value=u'F', tag=u'F')
temperatureUnit.C = temperatureUnit._CF_enumeration.addEnumeration(unicode_value=u'C', tag=u'C')
temperatureUnit._InitializeFacetMap(temperatureUnit._CF_enumeration)
Namespace.addCategoryObject('typeBinding', u'temperatureUnit', temperatureUnit)

# Atomic simple type: {iFAB}hardnessUnit
class hardnessUnit (pyxb.binding.datatypes.normalizedString, pyxb.binding.basis.enumeration_mixin):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'hardnessUnit')
    _XSDLocation = pyxb.utils.utility.Location(u'common.xsd', 213, 2)
    _Documentation = None
hardnessUnit._CF_enumeration = pyxb.binding.facets.CF_enumeration(value_datatype=hardnessUnit, enum_prefix=None)
hardnessUnit.bh = hardnessUnit._CF_enumeration.addEnumeration(unicode_value=u'bh', tag=u'bh')
hardnessUnit._InitializeFacetMap(hardnessUnit._CF_enumeration)
Namespace.addCategoryObject('typeBinding', u'hardnessUnit', hardnessUnit)

# Atomic simple type: {iFAB}angleUnit
class angleUnit (pyxb.binding.datatypes.normalizedString, pyxb.binding.basis.enumeration_mixin):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'angleUnit')
    _XSDLocation = pyxb.utils.utility.Location(u'common.xsd', 219, 2)
    _Documentation = None
angleUnit._CF_enumeration = pyxb.binding.facets.CF_enumeration(value_datatype=angleUnit, enum_prefix=None)
angleUnit.degrees = angleUnit._CF_enumeration.addEnumeration(unicode_value=u'degrees', tag=u'degrees')
angleUnit.radians = angleUnit._CF_enumeration.addEnumeration(unicode_value=u'radians', tag=u'radians')
angleUnit._InitializeFacetMap(angleUnit._CF_enumeration)
Namespace.addCategoryObject('typeBinding', u'angleUnit', angleUnit)

# Atomic simple type: {iFAB}pressureUnit
class pressureUnit (pyxb.binding.datatypes.normalizedString, pyxb.binding.basis.enumeration_mixin):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'pressureUnit')
    _XSDLocation = pyxb.utils.utility.Location(u'common.xsd', 226, 2)
    _Documentation = None
pressureUnit._CF_enumeration = pyxb.binding.facets.CF_enumeration(value_datatype=pressureUnit, enum_prefix=None)
pressureUnit.Pa = pressureUnit._CF_enumeration.addEnumeration(unicode_value=u'Pa', tag=u'Pa')
pressureUnit.kPa = pressureUnit._CF_enumeration.addEnumeration(unicode_value=u'kPa', tag=u'kPa')
pressureUnit.MPa = pressureUnit._CF_enumeration.addEnumeration(unicode_value=u'MPa', tag=u'MPa')
pressureUnit.GPa = pressureUnit._CF_enumeration.addEnumeration(unicode_value=u'GPa', tag=u'GPa')
pressureUnit.psi = pressureUnit._CF_enumeration.addEnumeration(unicode_value=u'psi', tag=u'psi')
pressureUnit.ksi = pressureUnit._CF_enumeration.addEnumeration(unicode_value=u'ksi', tag=u'ksi')
pressureUnit._InitializeFacetMap(pressureUnit._CF_enumeration)
Namespace.addCategoryObject('typeBinding', u'pressureUnit', pressureUnit)

# Complex type {iFAB}assemblyDetails with content type ELEMENT_ONLY
class assemblyDetails_ (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {iFAB}assemblyDetails with content type ELEMENT_ONLY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'assemblyDetails')
    _XSDLocation = pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 14, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.anyType
    
    # Element assemblyDetail uses Python identifier assemblyDetail
    __assemblyDetail = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'assemblyDetail'), 'assemblyDetail', '__iFAB_assemblyDetails__assemblyDetail', True, pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 16, 6), )

    
    assemblyDetail = property(__assemblyDetail.value, __assemblyDetail.set, None, None)

    _ElementMap.update({
        __assemblyDetail.name() : __assemblyDetail
    })
    _AttributeMap.update({
        
    })
Namespace.addCategoryObject('typeBinding', u'assemblyDetails', assemblyDetails_)


# Complex type {iFAB}incidentalContact with content type EMPTY
class incidentalContact (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {iFAB}incidentalContact with content type EMPTY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_EMPTY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'incidentalContact')
    _XSDLocation = pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 38, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.anyType
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        
    })
Namespace.addCategoryObject('typeBinding', u'incidentalContact', incidentalContact)


# Complex type {iFAB}mechanical with content type ELEMENT_ONLY
class mechanical (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {iFAB}mechanical with content type ELEMENT_ONLY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'mechanical')
    _XSDLocation = pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 43, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.anyType
    
    # Element linkingPart uses Python identifier linkingPart
    __linkingPart = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'linkingPart'), 'linkingPart', '__iFAB_mechanical_linkingPart', True, pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 45, 6), )

    
    linkingPart = property(__linkingPart.value, __linkingPart.set, None, None)

    
    # Element notes uses Python identifier notes
    __notes = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'notes'), 'notes', '__iFAB_mechanical_notes', False, pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 46, 6), )

    
    notes = property(__notes.value, __notes.set, None, None)

    
    # Element fasteningMethod uses Python identifier fasteningMethod
    __fasteningMethod = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'fasteningMethod'), 'fasteningMethod', '__iFAB_mechanical_fasteningMethod', False, pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 47, 6), )

    
    fasteningMethod = property(__fasteningMethod.value, __fasteningMethod.set, None, None)

    
    # Element fasteningQuantity uses Python identifier fasteningQuantity
    __fasteningQuantity = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'fasteningQuantity'), 'fasteningQuantity', '__iFAB_mechanical_fasteningQuantity', False, pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 48, 6), )

    
    fasteningQuantity = property(__fasteningQuantity.value, __fasteningQuantity.set, None, None)

    
    # Element torque uses Python identifier torque
    __torque = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'torque'), 'torque', '__iFAB_mechanical_torque', False, pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 49, 6), )

    
    torque = property(__torque.value, __torque.set, None, None)

    
    # Element force uses Python identifier force
    __force = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'force'), 'force', '__iFAB_mechanical_force', False, pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 50, 6), )

    
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
Namespace.addCategoryObject('typeBinding', u'mechanical', mechanical)


# Complex type {iFAB}welded with content type ELEMENT_ONLY
class welded (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {iFAB}welded with content type ELEMENT_ONLY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'welded')
    _XSDLocation = pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 54, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.anyType
    
    # Element linkingPart uses Python identifier linkingPart
    __linkingPart = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'linkingPart'), 'linkingPart', '__iFAB_welded_linkingPart', True, pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 56, 6), )

    
    linkingPart = property(__linkingPart.value, __linkingPart.set, None, None)

    
    # Element notes uses Python identifier notes
    __notes = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'notes'), 'notes', '__iFAB_welded_notes', False, pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 57, 6), )

    
    notes = property(__notes.value, __notes.set, None, None)

    
    # Element length uses Python identifier length
    __length = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'length'), 'length', '__iFAB_welded_length', False, pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 58, 6), )

    
    length = property(__length.value, __length.set, None, None)

    
    # Element jointType uses Python identifier jointType
    __jointType = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'jointType'), 'jointType', '__iFAB_welded_jointType', False, pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 59, 6), )

    
    jointType = property(__jointType.value, __jointType.set, None, None)

    
    # Element weldType uses Python identifier weldType
    __weldType = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'weldType'), 'weldType', '__iFAB_welded_weldType', False, pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 60, 6), )

    
    weldType = property(__weldType.value, __weldType.set, None, None)

    
    # Element weldPenetration uses Python identifier weldPenetration
    __weldPenetration = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'weldPenetration'), 'weldPenetration', '__iFAB_welded_weldPenetration', False, pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 61, 6), )

    
    weldPenetration = property(__weldPenetration.value, __weldPenetration.set, None, None)

    
    # Element twoSided uses Python identifier twoSided
    __twoSided = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'twoSided'), 'twoSided', '__iFAB_welded_twoSided', False, pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 62, 6), )

    
    twoSided = property(__twoSided.value, __twoSided.set, None, None)

    
    # Element inspectionRequirement uses Python identifier inspectionRequirement
    __inspectionRequirement = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'inspectionRequirement'), 'inspectionRequirement', '__iFAB_welded_inspectionRequirement', False, pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 63, 6), )

    
    inspectionRequirement = property(__inspectionRequirement.value, __inspectionRequirement.set, None, None)

    
    # Element part1Thickness uses Python identifier part1Thickness
    __part1Thickness = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'part1Thickness'), 'part1Thickness', '__iFAB_welded_part1Thickness', False, pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 64, 6), )

    
    part1Thickness = property(__part1Thickness.value, __part1Thickness.set, None, None)

    
    # Element part1Material uses Python identifier part1Material
    __part1Material = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'part1Material'), 'part1Material', '__iFAB_welded_part1Material', False, pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 65, 6), )

    
    part1Material = property(__part1Material.value, __part1Material.set, None, None)

    
    # Element part2Thickness uses Python identifier part2Thickness
    __part2Thickness = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'part2Thickness'), 'part2Thickness', '__iFAB_welded_part2Thickness', False, pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 66, 6), )

    
    part2Thickness = property(__part2Thickness.value, __part2Thickness.set, None, None)

    
    # Element part2Material uses Python identifier part2Material
    __part2Material = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'part2Material'), 'part2Material', '__iFAB_welded_part2Material', False, pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 67, 6), )

    
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
Namespace.addCategoryObject('typeBinding', u'welded', welded)


# Complex type {iFAB}soldered with content type ELEMENT_ONLY
class soldered (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {iFAB}soldered with content type ELEMENT_ONLY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'soldered')
    _XSDLocation = pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 71, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.anyType
    
    # Element linkingPart uses Python identifier linkingPart
    __linkingPart = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'linkingPart'), 'linkingPart', '__iFAB_soldered_linkingPart', True, pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 73, 6), )

    
    linkingPart = property(__linkingPart.value, __linkingPart.set, None, None)

    
    # Element notes uses Python identifier notes
    __notes = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'notes'), 'notes', '__iFAB_soldered_notes', False, pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 74, 6), )

    
    notes = property(__notes.value, __notes.set, None, None)

    
    # Element length uses Python identifier length
    __length = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'length'), 'length', '__iFAB_soldered_length', False, pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 75, 6), )

    
    length = property(__length.value, __length.set, None, None)

    
    # Element fillerMaterial uses Python identifier fillerMaterial
    __fillerMaterial = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'fillerMaterial'), 'fillerMaterial', '__iFAB_soldered_fillerMaterial', False, pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 76, 6), )

    
    fillerMaterial = property(__fillerMaterial.value, __fillerMaterial.set, None, None)

    
    # Element fluxMaterial uses Python identifier fluxMaterial
    __fluxMaterial = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'fluxMaterial'), 'fluxMaterial', '__iFAB_soldered_fluxMaterial', False, pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 77, 6), )

    
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
Namespace.addCategoryObject('typeBinding', u'soldered', soldered)


# Complex type {iFAB}brazed with content type ELEMENT_ONLY
class brazed (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {iFAB}brazed with content type ELEMENT_ONLY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'brazed')
    _XSDLocation = pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 81, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.anyType
    
    # Element linkingPart uses Python identifier linkingPart
    __linkingPart = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'linkingPart'), 'linkingPart', '__iFAB_brazed_linkingPart', True, pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 83, 6), )

    
    linkingPart = property(__linkingPart.value, __linkingPart.set, None, None)

    
    # Element notes uses Python identifier notes
    __notes = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'notes'), 'notes', '__iFAB_brazed_notes', False, pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 84, 6), )

    
    notes = property(__notes.value, __notes.set, None, None)

    
    # Element length uses Python identifier length
    __length = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'length'), 'length', '__iFAB_brazed_length', False, pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 85, 6), )

    
    length = property(__length.value, __length.set, None, None)

    
    # Element fillerMaterial uses Python identifier fillerMaterial
    __fillerMaterial = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'fillerMaterial'), 'fillerMaterial', '__iFAB_brazed_fillerMaterial', False, pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 86, 6), )

    
    fillerMaterial = property(__fillerMaterial.value, __fillerMaterial.set, None, None)

    
    # Element fluxMaterial uses Python identifier fluxMaterial
    __fluxMaterial = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'fluxMaterial'), 'fluxMaterial', '__iFAB_brazed_fluxMaterial', False, pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 87, 6), )

    
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
Namespace.addCategoryObject('typeBinding', u'brazed', brazed)


# Complex type {iFAB}glued with content type ELEMENT_ONLY
class glued (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {iFAB}glued with content type ELEMENT_ONLY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'glued')
    _XSDLocation = pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 91, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.anyType
    
    # Element linkingPart uses Python identifier linkingPart
    __linkingPart = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'linkingPart'), 'linkingPart', '__iFAB_glued_linkingPart', True, pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 93, 6), )

    
    linkingPart = property(__linkingPart.value, __linkingPart.set, None, None)

    
    # Element notes uses Python identifier notes
    __notes = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'notes'), 'notes', '__iFAB_glued_notes', False, pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 94, 6), )

    
    notes = property(__notes.value, __notes.set, None, None)

    
    # Element length uses Python identifier length
    __length = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'length'), 'length', '__iFAB_glued_length', False, pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 95, 6), )

    
    length = property(__length.value, __length.set, None, None)

    
    # Element volume uses Python identifier volume
    __volume = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'volume'), 'volume', '__iFAB_glued_volume', False, pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 96, 6), )

    
    volume = property(__volume.value, __volume.set, None, None)

    
    # Element material uses Python identifier material
    __material = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'material'), 'material', '__iFAB_glued_material', False, pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 97, 6), )

    
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
Namespace.addCategoryObject('typeBinding', u'glued', glued)


# Complex type {iFAB}assemblyDetail with content type ELEMENT_ONLY
class assemblyDetail (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {iFAB}assemblyDetail with content type ELEMENT_ONLY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'assemblyDetail')
    _XSDLocation = pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 20, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.anyType
    
    # Element name uses Python identifier name
    __name = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'name'), 'name', '__iFAB_assemblyDetail_name', False, pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 22, 6), )

    
    name = property(__name.value, __name.set, None, None)

    
    # Element part1 uses Python identifier part1
    __part1 = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'part1'), 'part1', '__iFAB_assemblyDetail_part1', False, pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 23, 6), )

    
    part1 = property(__part1.value, __part1.set, None, None)

    
    # Element part2 uses Python identifier part2
    __part2 = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'part2'), 'part2', '__iFAB_assemblyDetail_part2', False, pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 24, 6), )

    
    part2 = property(__part2.value, __part2.set, None, None)

    
    # Element description uses Python identifier description
    __description = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'description'), 'description', '__iFAB_assemblyDetail_description', False, pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 25, 6), )

    
    description = property(__description.value, __description.set, None, None)

    
    # Element incidentalContact uses Python identifier incidentalContact
    __incidentalContact = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'incidentalContact'), 'incidentalContact', '__iFAB_assemblyDetail_incidentalContact', True, pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 27, 8), )

    
    incidentalContact = property(__incidentalContact.value, __incidentalContact.set, None, None)

    
    # Element mechanical uses Python identifier mechanical
    __mechanical = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'mechanical'), 'mechanical', '__iFAB_assemblyDetail_mechanical', True, pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 28, 8), )

    
    mechanical = property(__mechanical.value, __mechanical.set, None, None)

    
    # Element welded uses Python identifier welded
    __welded = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'welded'), 'welded', '__iFAB_assemblyDetail_welded', True, pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 29, 8), )

    
    welded = property(__welded.value, __welded.set, None, None)

    
    # Element soldered uses Python identifier soldered
    __soldered = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'soldered'), 'soldered', '__iFAB_assemblyDetail_soldered', True, pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 30, 8), )

    
    soldered = property(__soldered.value, __soldered.set, None, None)

    
    # Element brazed uses Python identifier brazed
    __brazed = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'brazed'), 'brazed', '__iFAB_assemblyDetail_brazed', True, pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 31, 8), )

    
    brazed = property(__brazed.value, __brazed.set, None, None)

    
    # Element glued uses Python identifier glued
    __glued = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, u'glued'), 'glued', '__iFAB_assemblyDetail_glued', True, pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 32, 8), )

    
    glued = property(__glued.value, __glued.set, None, None)

    
    # Attribute {iFAB}id uses Python identifier id
    __id = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(Namespace, u'id'), 'id', '__iFAB_assemblyDetail_iFABid', guid, required=True)
    __id._DeclarationLocation = pyxb.utils.utility.Location(u'common.xsd', 10, 2)
    __id._UseLocation = pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 35, 4)
    
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
Namespace.addCategoryObject('typeBinding', u'assemblyDetail', assemblyDetail)


# Complex type {iFAB}partReference with content type SIMPLE
class partReference (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {iFAB}partReference with content type SIMPLE"""
    _TypeDefinition = pyxb.binding.datatypes.string
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'partReference')
    _XSDLocation = pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 163, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.string
    
    # Attribute {iFAB}id uses Python identifier id
    __id = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(Namespace, u'id'), 'id', '__iFAB_partReference_iFABid', guid, required=True)
    __id._DeclarationLocation = pyxb.utils.utility.Location(u'common.xsd', 10, 2)
    __id._UseLocation = pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 166, 8)
    
    id = property(__id.value, __id.set, None, None)

    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __id.name() : __id
    })
Namespace.addCategoryObject('typeBinding', u'partReference', partReference)


# Complex type {iFAB}duration with content type SIMPLE
class duration (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {iFAB}duration with content type SIMPLE"""
    _TypeDefinition = nonNegativeDecimal
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'duration')
    _XSDLocation = pyxb.utils.utility.Location(u'common.xsd', 26, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is nonNegativeDecimal
    
    # Attribute unit uses Python identifier unit
    __unit = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'unit'), 'unit', '__iFAB_duration_unit', timeUnit, unicode_default=u'day')
    __unit._DeclarationLocation = pyxb.utils.utility.Location(u'common.xsd', 29, 8)
    __unit._UseLocation = pyxb.utils.utility.Location(u'common.xsd', 29, 8)
    
    unit = property(__unit.value, __unit.set, None, None)

    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __unit.name() : __unit
    })
Namespace.addCategoryObject('typeBinding', u'duration', duration)


# Complex type {iFAB}price with content type SIMPLE
class price (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {iFAB}price with content type SIMPLE"""
    _TypeDefinition = nonNegativeDecimal
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'price')
    _XSDLocation = pyxb.utils.utility.Location(u'common.xsd', 34, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is nonNegativeDecimal
    
    # Attribute currency uses Python identifier currency
    __currency = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'currency'), 'currency', '__iFAB_price_currency', currency, unicode_default=u'USD')
    __currency._DeclarationLocation = pyxb.utils.utility.Location(u'common.xsd', 37, 8)
    __currency._UseLocation = pyxb.utils.utility.Location(u'common.xsd', 37, 8)
    
    currency = property(__currency.value, __currency.set, None, None)

    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __currency.name() : __currency
    })
Namespace.addCategoryObject('typeBinding', u'price', price)


# Complex type {iFAB}length with content type SIMPLE
class length (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {iFAB}length with content type SIMPLE"""
    _TypeDefinition = nonNegativeDecimal
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'length')
    _XSDLocation = pyxb.utils.utility.Location(u'common.xsd', 42, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is nonNegativeDecimal
    
    # Attribute unit uses Python identifier unit
    __unit = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'unit'), 'unit', '__iFAB_length_unit', lengthUnit, unicode_default=u'mm')
    __unit._DeclarationLocation = pyxb.utils.utility.Location(u'common.xsd', 45, 8)
    __unit._UseLocation = pyxb.utils.utility.Location(u'common.xsd', 45, 8)
    
    unit = property(__unit.value, __unit.set, None, None)

    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __unit.name() : __unit
    })
Namespace.addCategoryObject('typeBinding', u'length', length)


# Complex type {iFAB}area with content type SIMPLE
class area (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {iFAB}area with content type SIMPLE"""
    _TypeDefinition = nonNegativeDecimal
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'area')
    _XSDLocation = pyxb.utils.utility.Location(u'common.xsd', 50, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is nonNegativeDecimal
    
    # Attribute unit uses Python identifier unit
    __unit = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'unit'), 'unit', '__iFAB_area_unit', areaUnit, unicode_default=u'mm2')
    __unit._DeclarationLocation = pyxb.utils.utility.Location(u'common.xsd', 53, 8)
    __unit._UseLocation = pyxb.utils.utility.Location(u'common.xsd', 53, 8)
    
    unit = property(__unit.value, __unit.set, None, None)

    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __unit.name() : __unit
    })
Namespace.addCategoryObject('typeBinding', u'area', area)


# Complex type {iFAB}volume with content type SIMPLE
class volume (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {iFAB}volume with content type SIMPLE"""
    _TypeDefinition = nonNegativeDecimal
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'volume')
    _XSDLocation = pyxb.utils.utility.Location(u'common.xsd', 58, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is nonNegativeDecimal
    
    # Attribute unit uses Python identifier unit
    __unit = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'unit'), 'unit', '__iFAB_volume_unit', volumeUnit, unicode_default=u'mm3')
    __unit._DeclarationLocation = pyxb.utils.utility.Location(u'common.xsd', 61, 8)
    __unit._UseLocation = pyxb.utils.utility.Location(u'common.xsd', 61, 8)
    
    unit = property(__unit.value, __unit.set, None, None)

    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __unit.name() : __unit
    })
Namespace.addCategoryObject('typeBinding', u'volume', volume)


# Complex type {iFAB}weight with content type SIMPLE
class weight (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {iFAB}weight with content type SIMPLE"""
    _TypeDefinition = nonNegativeDecimal
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'weight')
    _XSDLocation = pyxb.utils.utility.Location(u'common.xsd', 66, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is nonNegativeDecimal
    
    # Attribute unit uses Python identifier unit
    __unit = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'unit'), 'unit', '__iFAB_weight_unit', massUnit, unicode_default=u'g')
    __unit._DeclarationLocation = pyxb.utils.utility.Location(u'common.xsd', 69, 8)
    __unit._UseLocation = pyxb.utils.utility.Location(u'common.xsd', 69, 8)
    
    unit = property(__unit.value, __unit.set, None, None)

    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __unit.name() : __unit
    })
Namespace.addCategoryObject('typeBinding', u'weight', weight)


# Complex type {iFAB}force with content type SIMPLE
class force (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {iFAB}force with content type SIMPLE"""
    _TypeDefinition = nonNegativeDecimal
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'force')
    _XSDLocation = pyxb.utils.utility.Location(u'common.xsd', 74, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is nonNegativeDecimal
    
    # Attribute unit uses Python identifier unit
    __unit = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'unit'), 'unit', '__iFAB_force_unit', forceUnit, unicode_default=u'N')
    __unit._DeclarationLocation = pyxb.utils.utility.Location(u'common.xsd', 77, 8)
    __unit._UseLocation = pyxb.utils.utility.Location(u'common.xsd', 77, 8)
    
    unit = property(__unit.value, __unit.set, None, None)

    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __unit.name() : __unit
    })
Namespace.addCategoryObject('typeBinding', u'force', force)


# Complex type {iFAB}torque with content type SIMPLE
class torque (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {iFAB}torque with content type SIMPLE"""
    _TypeDefinition = nonNegativeDecimal
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'torque')
    _XSDLocation = pyxb.utils.utility.Location(u'common.xsd', 82, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is nonNegativeDecimal
    
    # Attribute unit uses Python identifier unit
    __unit = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'unit'), 'unit', '__iFAB_torque_unit', torqueUnit, unicode_default=u'N m')
    __unit._DeclarationLocation = pyxb.utils.utility.Location(u'common.xsd', 85, 8)
    __unit._UseLocation = pyxb.utils.utility.Location(u'common.xsd', 85, 8)
    
    unit = property(__unit.value, __unit.set, None, None)

    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __unit.name() : __unit
    })
Namespace.addCategoryObject('typeBinding', u'torque', torque)


# Complex type {iFAB}hardness with content type SIMPLE
class hardness (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {iFAB}hardness with content type SIMPLE"""
    _TypeDefinition = nonNegativeDecimal
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'hardness')
    _XSDLocation = pyxb.utils.utility.Location(u'common.xsd', 90, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is nonNegativeDecimal
    
    # Attribute unit uses Python identifier unit
    __unit = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'unit'), 'unit', '__iFAB_hardness_unit', hardnessUnit, unicode_default=u'bh')
    __unit._DeclarationLocation = pyxb.utils.utility.Location(u'common.xsd', 93, 8)
    __unit._UseLocation = pyxb.utils.utility.Location(u'common.xsd', 93, 8)
    
    unit = property(__unit.value, __unit.set, None, None)

    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __unit.name() : __unit
    })
Namespace.addCategoryObject('typeBinding', u'hardness', hardness)


# Complex type {iFAB}angle with content type SIMPLE
class angle (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {iFAB}angle with content type SIMPLE"""
    _TypeDefinition = nonNegativeDecimal
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'angle')
    _XSDLocation = pyxb.utils.utility.Location(u'common.xsd', 98, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is nonNegativeDecimal
    
    # Attribute unit uses Python identifier unit
    __unit = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'unit'), 'unit', '__iFAB_angle_unit', angleUnit, unicode_default=u'degrees')
    __unit._DeclarationLocation = pyxb.utils.utility.Location(u'common.xsd', 101, 8)
    __unit._UseLocation = pyxb.utils.utility.Location(u'common.xsd', 101, 8)
    
    unit = property(__unit.value, __unit.set, None, None)

    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __unit.name() : __unit
    })
Namespace.addCategoryObject('typeBinding', u'angle', angle)


# Complex type {iFAB}temperature with content type SIMPLE
class temperature (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {iFAB}temperature with content type SIMPLE"""
    _TypeDefinition = pyxb.binding.datatypes.decimal
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'temperature')
    _XSDLocation = pyxb.utils.utility.Location(u'common.xsd', 106, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.decimal
    
    # Attribute unit uses Python identifier unit
    __unit = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'unit'), 'unit', '__iFAB_temperature_unit', temperatureUnit, unicode_default=u'C')
    __unit._DeclarationLocation = pyxb.utils.utility.Location(u'common.xsd', 109, 8)
    __unit._UseLocation = pyxb.utils.utility.Location(u'common.xsd', 109, 8)
    
    unit = property(__unit.value, __unit.set, None, None)

    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __unit.name() : __unit
    })
Namespace.addCategoryObject('typeBinding', u'temperature', temperature)


# Complex type {iFAB}pressure with content type SIMPLE
class pressure (pyxb.binding.basis.complexTypeDefinition):
    """Complex type {iFAB}pressure with content type SIMPLE"""
    _TypeDefinition = nonNegativeDecimal
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, u'pressure')
    _XSDLocation = pyxb.utils.utility.Location(u'common.xsd', 114, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is nonNegativeDecimal
    
    # Attribute unit uses Python identifier unit
    __unit = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, u'unit'), 'unit', '__iFAB_pressure_unit', pressureUnit, unicode_default=u'Pa')
    __unit._DeclarationLocation = pyxb.utils.utility.Location(u'common.xsd', 117, 8)
    __unit._UseLocation = pyxb.utils.utility.Location(u'common.xsd', 117, 8)
    
    unit = property(__unit.value, __unit.set, None, None)

    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __unit.name() : __unit
    })
Namespace.addCategoryObject('typeBinding', u'pressure', pressure)


assemblyDetails = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, u'assemblyDetails'), assemblyDetails_, location=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 12, 2))
Namespace.addCategoryObject('elementBinding', assemblyDetails.name().localName(), assemblyDetails)



assemblyDetails_._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'assemblyDetail'), assemblyDetail, scope=assemblyDetails_, location=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 16, 6)))

def _BuildAutomaton ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton
    del _BuildAutomaton
    import pyxb.utils.fac as fac

    counters = set()
    cc_0 = fac.CounterCondition(min=0L, max=None, metadata=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 15, 4))
    counters.add(cc_0)
    states = []
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_0, False))
    symbol = pyxb.binding.content.ElementUse(assemblyDetails_._UseForTag(pyxb.namespace.ExpandedName(None, u'assemblyDetail')), pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 16, 6))
    st_0 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_0)
    transitions = []
    transitions.append(fac.Transition(st_0, [
        fac.UpdateInstruction(cc_0, True) ]))
    st_0._set_transitionSet(transitions)
    return fac.Automaton(states, counters, True, containing_state=None)
assemblyDetails_._Automaton = _BuildAutomaton()




mechanical._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'linkingPart'), partReference, scope=mechanical, location=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 45, 6)))

mechanical._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'notes'), pyxb.binding.datatypes.string, scope=mechanical, location=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 46, 6)))

mechanical._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'fasteningMethod'), fasteningMethod, scope=mechanical, location=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 47, 6)))

mechanical._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'fasteningQuantity'), pyxb.binding.datatypes.positiveInteger, scope=mechanical, location=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 48, 6), unicode_default=u'1'))

mechanical._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'torque'), torque, scope=mechanical, location=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 49, 6)))

mechanical._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'force'), force, scope=mechanical, location=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 50, 6)))

def _BuildAutomaton_ ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton_
    del _BuildAutomaton_
    import pyxb.utils.fac as fac

    counters = set()
    cc_0 = fac.CounterCondition(min=0L, max=None, metadata=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 45, 6))
    counters.add(cc_0)
    cc_1 = fac.CounterCondition(min=0L, max=1, metadata=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 46, 6))
    counters.add(cc_1)
    cc_2 = fac.CounterCondition(min=0L, max=1, metadata=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 48, 6))
    counters.add(cc_2)
    cc_3 = fac.CounterCondition(min=0L, max=1, metadata=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 49, 6))
    counters.add(cc_3)
    cc_4 = fac.CounterCondition(min=0L, max=1, metadata=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 50, 6))
    counters.add(cc_4)
    states = []
    final_update = None
    symbol = pyxb.binding.content.ElementUse(mechanical._UseForTag(pyxb.namespace.ExpandedName(None, u'linkingPart')), pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 45, 6))
    st_0 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_0)
    final_update = None
    symbol = pyxb.binding.content.ElementUse(mechanical._UseForTag(pyxb.namespace.ExpandedName(None, u'notes')), pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 46, 6))
    st_1 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_1)
    final_update = set()
    symbol = pyxb.binding.content.ElementUse(mechanical._UseForTag(pyxb.namespace.ExpandedName(None, u'fasteningMethod')), pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 47, 6))
    st_2 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_2)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_2, False))
    symbol = pyxb.binding.content.ElementUse(mechanical._UseForTag(pyxb.namespace.ExpandedName(None, u'fasteningQuantity')), pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 48, 6))
    st_3 = fac.State(symbol, is_initial=False, final_update=final_update, is_unordered_catenation=False)
    states.append(st_3)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_3, False))
    symbol = pyxb.binding.content.ElementUse(mechanical._UseForTag(pyxb.namespace.ExpandedName(None, u'torque')), pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 49, 6))
    st_4 = fac.State(symbol, is_initial=False, final_update=final_update, is_unordered_catenation=False)
    states.append(st_4)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_4, False))
    symbol = pyxb.binding.content.ElementUse(mechanical._UseForTag(pyxb.namespace.ExpandedName(None, u'force')), pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 50, 6))
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




welded._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'linkingPart'), partReference, scope=welded, location=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 56, 6)))

welded._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'notes'), pyxb.binding.datatypes.string, scope=welded, location=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 57, 6)))

welded._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'length'), length, scope=welded, location=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 58, 6)))

welded._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'jointType'), jointType, scope=welded, location=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 59, 6)))

welded._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'weldType'), weldType, scope=welded, location=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 60, 6)))

welded._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'weldPenetration'), weldPenetration, scope=welded, location=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 61, 6)))

welded._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'twoSided'), pyxb.binding.datatypes.boolean, scope=welded, location=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 62, 6)))

welded._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'inspectionRequirement'), inspectionRequirement, scope=welded, location=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 63, 6)))

welded._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'part1Thickness'), length, scope=welded, location=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 64, 6)))

welded._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'part1Material'), baseMaterial, scope=welded, location=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 65, 6)))

welded._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'part2Thickness'), length, scope=welded, location=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 66, 6)))

welded._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'part2Material'), baseMaterial, scope=welded, location=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 67, 6)))

def _BuildAutomaton_2 ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton_2
    del _BuildAutomaton_2
    import pyxb.utils.fac as fac

    counters = set()
    cc_0 = fac.CounterCondition(min=0L, max=None, metadata=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 56, 6))
    counters.add(cc_0)
    cc_1 = fac.CounterCondition(min=0L, max=1, metadata=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 57, 6))
    counters.add(cc_1)
    states = []
    final_update = None
    symbol = pyxb.binding.content.ElementUse(welded._UseForTag(pyxb.namespace.ExpandedName(None, u'linkingPart')), pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 56, 6))
    st_0 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_0)
    final_update = None
    symbol = pyxb.binding.content.ElementUse(welded._UseForTag(pyxb.namespace.ExpandedName(None, u'notes')), pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 57, 6))
    st_1 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_1)
    final_update = None
    symbol = pyxb.binding.content.ElementUse(welded._UseForTag(pyxb.namespace.ExpandedName(None, u'length')), pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 58, 6))
    st_2 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_2)
    final_update = None
    symbol = pyxb.binding.content.ElementUse(welded._UseForTag(pyxb.namespace.ExpandedName(None, u'jointType')), pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 59, 6))
    st_3 = fac.State(symbol, is_initial=False, final_update=final_update, is_unordered_catenation=False)
    states.append(st_3)
    final_update = None
    symbol = pyxb.binding.content.ElementUse(welded._UseForTag(pyxb.namespace.ExpandedName(None, u'weldType')), pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 60, 6))
    st_4 = fac.State(symbol, is_initial=False, final_update=final_update, is_unordered_catenation=False)
    states.append(st_4)
    final_update = None
    symbol = pyxb.binding.content.ElementUse(welded._UseForTag(pyxb.namespace.ExpandedName(None, u'weldPenetration')), pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 61, 6))
    st_5 = fac.State(symbol, is_initial=False, final_update=final_update, is_unordered_catenation=False)
    states.append(st_5)
    final_update = None
    symbol = pyxb.binding.content.ElementUse(welded._UseForTag(pyxb.namespace.ExpandedName(None, u'twoSided')), pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 62, 6))
    st_6 = fac.State(symbol, is_initial=False, final_update=final_update, is_unordered_catenation=False)
    states.append(st_6)
    final_update = None
    symbol = pyxb.binding.content.ElementUse(welded._UseForTag(pyxb.namespace.ExpandedName(None, u'inspectionRequirement')), pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 63, 6))
    st_7 = fac.State(symbol, is_initial=False, final_update=final_update, is_unordered_catenation=False)
    states.append(st_7)
    final_update = None
    symbol = pyxb.binding.content.ElementUse(welded._UseForTag(pyxb.namespace.ExpandedName(None, u'part1Thickness')), pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 64, 6))
    st_8 = fac.State(symbol, is_initial=False, final_update=final_update, is_unordered_catenation=False)
    states.append(st_8)
    final_update = None
    symbol = pyxb.binding.content.ElementUse(welded._UseForTag(pyxb.namespace.ExpandedName(None, u'part1Material')), pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 65, 6))
    st_9 = fac.State(symbol, is_initial=False, final_update=final_update, is_unordered_catenation=False)
    states.append(st_9)
    final_update = None
    symbol = pyxb.binding.content.ElementUse(welded._UseForTag(pyxb.namespace.ExpandedName(None, u'part2Thickness')), pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 66, 6))
    st_10 = fac.State(symbol, is_initial=False, final_update=final_update, is_unordered_catenation=False)
    states.append(st_10)
    final_update = set()
    symbol = pyxb.binding.content.ElementUse(welded._UseForTag(pyxb.namespace.ExpandedName(None, u'part2Material')), pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 67, 6))
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




soldered._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'linkingPart'), partReference, scope=soldered, location=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 73, 6)))

soldered._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'notes'), pyxb.binding.datatypes.string, scope=soldered, location=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 74, 6)))

soldered._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'length'), length, scope=soldered, location=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 75, 6)))

soldered._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'fillerMaterial'), fillerMaterial, scope=soldered, location=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 76, 6)))

soldered._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'fluxMaterial'), fluxMaterial, scope=soldered, location=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 77, 6)))

def _BuildAutomaton_3 ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton_3
    del _BuildAutomaton_3
    import pyxb.utils.fac as fac

    counters = set()
    cc_0 = fac.CounterCondition(min=0L, max=None, metadata=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 73, 6))
    counters.add(cc_0)
    cc_1 = fac.CounterCondition(min=0L, max=1, metadata=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 74, 6))
    counters.add(cc_1)
    states = []
    final_update = None
    symbol = pyxb.binding.content.ElementUse(soldered._UseForTag(pyxb.namespace.ExpandedName(None, u'linkingPart')), pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 73, 6))
    st_0 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_0)
    final_update = None
    symbol = pyxb.binding.content.ElementUse(soldered._UseForTag(pyxb.namespace.ExpandedName(None, u'notes')), pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 74, 6))
    st_1 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_1)
    final_update = None
    symbol = pyxb.binding.content.ElementUse(soldered._UseForTag(pyxb.namespace.ExpandedName(None, u'length')), pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 75, 6))
    st_2 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_2)
    final_update = None
    symbol = pyxb.binding.content.ElementUse(soldered._UseForTag(pyxb.namespace.ExpandedName(None, u'fillerMaterial')), pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 76, 6))
    st_3 = fac.State(symbol, is_initial=False, final_update=final_update, is_unordered_catenation=False)
    states.append(st_3)
    final_update = set()
    symbol = pyxb.binding.content.ElementUse(soldered._UseForTag(pyxb.namespace.ExpandedName(None, u'fluxMaterial')), pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 77, 6))
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




brazed._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'linkingPart'), partReference, scope=brazed, location=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 83, 6)))

brazed._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'notes'), pyxb.binding.datatypes.string, scope=brazed, location=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 84, 6)))

brazed._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'length'), length, scope=brazed, location=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 85, 6)))

brazed._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'fillerMaterial'), fillerMaterial, scope=brazed, location=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 86, 6)))

brazed._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'fluxMaterial'), fluxMaterial, scope=brazed, location=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 87, 6)))

def _BuildAutomaton_4 ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton_4
    del _BuildAutomaton_4
    import pyxb.utils.fac as fac

    counters = set()
    cc_0 = fac.CounterCondition(min=0L, max=None, metadata=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 83, 6))
    counters.add(cc_0)
    cc_1 = fac.CounterCondition(min=0L, max=1, metadata=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 84, 6))
    counters.add(cc_1)
    states = []
    final_update = None
    symbol = pyxb.binding.content.ElementUse(brazed._UseForTag(pyxb.namespace.ExpandedName(None, u'linkingPart')), pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 83, 6))
    st_0 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_0)
    final_update = None
    symbol = pyxb.binding.content.ElementUse(brazed._UseForTag(pyxb.namespace.ExpandedName(None, u'notes')), pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 84, 6))
    st_1 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_1)
    final_update = None
    symbol = pyxb.binding.content.ElementUse(brazed._UseForTag(pyxb.namespace.ExpandedName(None, u'length')), pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 85, 6))
    st_2 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_2)
    final_update = None
    symbol = pyxb.binding.content.ElementUse(brazed._UseForTag(pyxb.namespace.ExpandedName(None, u'fillerMaterial')), pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 86, 6))
    st_3 = fac.State(symbol, is_initial=False, final_update=final_update, is_unordered_catenation=False)
    states.append(st_3)
    final_update = set()
    symbol = pyxb.binding.content.ElementUse(brazed._UseForTag(pyxb.namespace.ExpandedName(None, u'fluxMaterial')), pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 87, 6))
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




glued._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'linkingPart'), partReference, scope=glued, location=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 93, 6)))

glued._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'notes'), pyxb.binding.datatypes.string, scope=glued, location=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 94, 6)))

glued._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'length'), length, scope=glued, location=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 95, 6)))

glued._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'volume'), volume, scope=glued, location=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 96, 6)))

glued._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'material'), glue, scope=glued, location=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 97, 6)))

def _BuildAutomaton_5 ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton_5
    del _BuildAutomaton_5
    import pyxb.utils.fac as fac

    counters = set()
    cc_0 = fac.CounterCondition(min=0L, max=None, metadata=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 93, 6))
    counters.add(cc_0)
    cc_1 = fac.CounterCondition(min=0L, max=1, metadata=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 94, 6))
    counters.add(cc_1)
    states = []
    final_update = None
    symbol = pyxb.binding.content.ElementUse(glued._UseForTag(pyxb.namespace.ExpandedName(None, u'linkingPart')), pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 93, 6))
    st_0 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_0)
    final_update = None
    symbol = pyxb.binding.content.ElementUse(glued._UseForTag(pyxb.namespace.ExpandedName(None, u'notes')), pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 94, 6))
    st_1 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_1)
    final_update = None
    symbol = pyxb.binding.content.ElementUse(glued._UseForTag(pyxb.namespace.ExpandedName(None, u'length')), pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 95, 6))
    st_2 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_2)
    final_update = None
    symbol = pyxb.binding.content.ElementUse(glued._UseForTag(pyxb.namespace.ExpandedName(None, u'volume')), pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 96, 6))
    st_3 = fac.State(symbol, is_initial=False, final_update=final_update, is_unordered_catenation=False)
    states.append(st_3)
    final_update = set()
    symbol = pyxb.binding.content.ElementUse(glued._UseForTag(pyxb.namespace.ExpandedName(None, u'material')), pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 97, 6))
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




assemblyDetail._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'name'), pyxb.binding.datatypes.string, scope=assemblyDetail, location=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 22, 6)))

assemblyDetail._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'part1'), partReference, scope=assemblyDetail, location=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 23, 6)))

assemblyDetail._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'part2'), partReference, scope=assemblyDetail, location=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 24, 6)))

assemblyDetail._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'description'), pyxb.binding.datatypes.string, scope=assemblyDetail, location=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 25, 6)))

assemblyDetail._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'incidentalContact'), incidentalContact, scope=assemblyDetail, location=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 27, 8)))

assemblyDetail._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'mechanical'), mechanical, scope=assemblyDetail, location=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 28, 8)))

assemblyDetail._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'welded'), welded, scope=assemblyDetail, location=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 29, 8)))

assemblyDetail._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'soldered'), soldered, scope=assemblyDetail, location=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 30, 8)))

assemblyDetail._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'brazed'), brazed, scope=assemblyDetail, location=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 31, 8)))

assemblyDetail._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, u'glued'), glued, scope=assemblyDetail, location=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 32, 8)))

def _BuildAutomaton_6 ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton_6
    del _BuildAutomaton_6
    import pyxb.utils.fac as fac

    counters = set()
    cc_0 = fac.CounterCondition(min=0L, max=1, metadata=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 22, 6))
    counters.add(cc_0)
    cc_1 = fac.CounterCondition(min=0L, max=1, metadata=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 25, 6))
    counters.add(cc_1)
    cc_2 = fac.CounterCondition(min=0L, max=None, metadata=pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 26, 6))
    counters.add(cc_2)
    states = []
    final_update = None
    symbol = pyxb.binding.content.ElementUse(assemblyDetail._UseForTag(pyxb.namespace.ExpandedName(None, u'name')), pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 22, 6))
    st_0 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_0)
    final_update = None
    symbol = pyxb.binding.content.ElementUse(assemblyDetail._UseForTag(pyxb.namespace.ExpandedName(None, u'part1')), pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 23, 6))
    st_1 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_1)
    final_update = set()
    symbol = pyxb.binding.content.ElementUse(assemblyDetail._UseForTag(pyxb.namespace.ExpandedName(None, u'part2')), pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 24, 6))
    st_2 = fac.State(symbol, is_initial=False, final_update=final_update, is_unordered_catenation=False)
    states.append(st_2)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_1, False))
    symbol = pyxb.binding.content.ElementUse(assemblyDetail._UseForTag(pyxb.namespace.ExpandedName(None, u'description')), pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 25, 6))
    st_3 = fac.State(symbol, is_initial=False, final_update=final_update, is_unordered_catenation=False)
    states.append(st_3)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_2, False))
    symbol = pyxb.binding.content.ElementUse(assemblyDetail._UseForTag(pyxb.namespace.ExpandedName(None, u'incidentalContact')), pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 27, 8))
    st_4 = fac.State(symbol, is_initial=False, final_update=final_update, is_unordered_catenation=False)
    states.append(st_4)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_2, False))
    symbol = pyxb.binding.content.ElementUse(assemblyDetail._UseForTag(pyxb.namespace.ExpandedName(None, u'mechanical')), pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 28, 8))
    st_5 = fac.State(symbol, is_initial=False, final_update=final_update, is_unordered_catenation=False)
    states.append(st_5)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_2, False))
    symbol = pyxb.binding.content.ElementUse(assemblyDetail._UseForTag(pyxb.namespace.ExpandedName(None, u'welded')), pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 29, 8))
    st_6 = fac.State(symbol, is_initial=False, final_update=final_update, is_unordered_catenation=False)
    states.append(st_6)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_2, False))
    symbol = pyxb.binding.content.ElementUse(assemblyDetail._UseForTag(pyxb.namespace.ExpandedName(None, u'soldered')), pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 30, 8))
    st_7 = fac.State(symbol, is_initial=False, final_update=final_update, is_unordered_catenation=False)
    states.append(st_7)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_2, False))
    symbol = pyxb.binding.content.ElementUse(assemblyDetail._UseForTag(pyxb.namespace.ExpandedName(None, u'brazed')), pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 31, 8))
    st_8 = fac.State(symbol, is_initial=False, final_update=final_update, is_unordered_catenation=False)
    states.append(st_8)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_2, False))
    symbol = pyxb.binding.content.ElementUse(assemblyDetail._UseForTag(pyxb.namespace.ExpandedName(None, u'glued')), pyxb.utils.utility.Location(u'AssemblyDetails.xsd', 32, 8))
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

