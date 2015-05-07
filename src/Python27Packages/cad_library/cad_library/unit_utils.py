__author__ = 'rowens'

import logging
import cad_library

MECHANICAL_UNIT_STRESS_PA = "Pa"  # Pascals
MECHANICAL_UNIT_STRESS_MPA = "MPa"  # Mega Pascals
MECHANICAL_UNIT_DENSITY_KG_PER_M3 = "kg/m^3"
MECHANICAL_UNIT_NONE = "None"  # Used for ratios (e.g. modulus_elastic )
MECHANICAL_UNIT_NOT_DEFINED = "Not Defined"
MECHANICAL_UNIT_CYCLE = "Cycle"

MECHANICAL_PASCAL_TO_MEGA_PASCAL_CONVERSION_FACTOR = 1.0 / 1000000.0


def convert_Pa_MPa(in_Pa):
    return in_Pa * MECHANICAL_PASCAL_TO_MEGA_PASCAL_CONVERSION_FACTOR


#TEMPORARY: assumes all parts have the same unit length (needs to be modified)
# This will always fail for time being (if CAD units are not mm) because CreateCADAssembly's top level
# assembly data is reported in mm, no matter the units of its containing parts.
def getUnitLength(asminfo):
    distUnits = []
    for child in asminfo.componentsdict.values():
        distUnits.append(child.units.distance)
    if not len(set(distUnits)) == 1:
        cad_library.exitwitherror("Non-consistent unit scale. All parts should "
                                  "have the same distance units.", -1)
    return distUnits[0]


def calcGeoScaleFac(unitLength):
    """ Calculates scaling factor to be applied to each numerical value's units. """
    logger = logging.getLogger()
    logger.info("Calculating geometry scale factor" + '\n')

    # Calculate geometry scale factor (with respect to meters)
    if unitLength == 'millimeter':
        unitScale = 0.001
        unitShort = 'mm'
    elif unitLength == 'inch':
        unitScale = 0.0254
        unitShort = 'in'
    elif unitLength == 'meter':
        unitScale = 1.0
        unitShort = 'm'
    else:
        cad_library.exitwitherror("ValueError: Length unit not supported", -1)

    logger.info("**********************************************************************************" + '\n')
    logger.info("Length unit is " + str(unitShort) + '\n')
    logger.info("Unit scale of " + str(unitScale) + ' will be used during the analysis\n')
    logger.info("**********************************************************************************" + '\n')

    return unitScale, unitShort


# Unit conversion dictionary uses mmNs, more convenient than mmKs
def generateConvDict(unitScale):
    conv = {}                                                            # unit conversion dictionary
    conv.update([['GPa', (10**9)*(unitScale**2)],                        # to N/length^2
                 ['MPa', (10**6)*(unitScale**2)],                        # to N/length^2
                 ['kPa', (10**3)*(unitScale**2)],                        # to N/length^2
                 ['pascal', unitScale**2],                               # to N/length^2
                 ['Pa', unitScale**2],                                   # to N/length^2
                 ['g/cm^3', (10**6)*(unitScale**3)],                     # to (N-s^2/length)/length^3
                 ['kg/m^3', (10**-3)*(unitScale**3)],                    # to (N-s^2/length)/length^3
                 ['kg/mm^3', (10**6)*(unitScale**3)],                    # to (N-s^2/length)/length^3
                 ['N', 1],                                               # to N
                 ['kN', 10**3],                                          # to N
                 ['N', 1],                                               # to N
                 ['N-mm', (10**-3)*(unitScale**-1)],                     # to N-length
                 ['m/s^2', 1],                                           # to length/s^2
                 ['mm/s^2', (10**-3)/unitScale],                         # to length/s^2
                 ['N/A', 1],                                             # dimensionless
                 ['1/C', 1],                                             # to 1/C
                 ['C', 1],                            # to Celsius (temperature values will be converted somewhere else)
                 ['1/K', 1],                                             # to 1/K
                 ['K', 1],                            # to Kelvin (temperature values will be converted somewhere else)
                 ['F', 1],                         # to Fahrenheit (temperature values will be converted somewhere else)
                 ['W/(m*K)', unitScale],                                 # to (N-length/s)/(K-length)
                 ['J/(kg*K)', unitScale**-2],                            # to (N-length)/(K-(N-s^2/length))
                 ['J/(kg-K)', unitScale**-2]])                           # to (N-length)/(K-(N-s^2/length))

    return conv


def convert_material_properties_given_conversion_dict(materialDict, conv):
    noUnits = [MECHANICAL_UNIT_NONE, MECHANICAL_UNIT_NOT_DEFINED]
    for k in materialDict.keys():
        material = materialDict[k]
        if material.density_defined:
            if any([material.density_unit == u for u in noUnits]):
                units = 1.0
            else:
                units = conv[material.density_unit]
            material.density *= units
        if material.mechanical__modulus_elastic_defined:
            if any([material.mechanical__modulus_elastic_unit == u for u in noUnits]):
                units = 1.0
            else:
                units = conv[material.mechanical__modulus_elastic_unit]
            material.mechanical__modulus_elastic *= units
        if material.mechanical__ratio_poissons_defined:
            if any([material.mechanical__ratio_poissons_unit == u for u in noUnits]):
                units = 1.0
            else:
                units = conv[material.mechanical__ratio_poissons_unit]
            material.mechanical__modulus_elastic *= units
        if material.mechanical__strength_tensile_yield_defined:
            if any([material.mechanical__strength_tensile_yield_unit == u for u in noUnits]):
                units = 1.0
            else:
                units = conv[material.mechanical__strength_tensile_yield_unit]
            material.mechanical__strength_tensile_yield *= units
        if material.mechanical__strength_tensile_ultimate_defined:
            if any([material.mechanical__strength_tensile_ultimate_unit == u for u in noUnits]):
                units = 1.0
            else:
                units = conv[material.mechanical__strength_tensile_ultimate_unit]
            material.mechanical__strength_tensile_ultimate *= units
        if material.mechanical__strength_bearing_yield_defined:
            if any([material.mechanical__strength_bearing_yield_unit == u for u in noUnits]):
                units = 1.0
            else:
                units = conv[material.mechanical__strength_bearing_yield_unit]
            material.mechanical__strength_bearing_yield *= units
        if material.mechanical__strength_bearing_ultimate_defined:
            if any([material.mechanical__strength_bearing_ultimate_unit == u for u in noUnits]):
                units = 1.0
            else:
                units = conv[material.mechanical__strength_bearing_ultimate_unit]
            material.mechanical__strength_bearing_ultimate *= units
        if material.mechanical__strength_shear_defined:
            if any([material.mechanical__strength_shear_unit == u for u in noUnits]):
                units = 1.0
            else:
                units = conv[material.mechanical__strength_shear_unit]
            material.mechanical__strength_shear *= units
        if material.mechanical__strength_fatigue_defined:
            if any([material.mechanical__strength_fatigue_unit == u for u in noUnits]):
                units = 1.0
            else:
                units = conv[material.mechanical__strength_fatigue_unit]
            material.mechanical__strength_fatigue *= units
        if material.thermal__coefficient_expansion_linear_defined:
            if any([material.thermal__coefficient_expansion_linear_unit == u for u in noUnits]):
                units = 1.0
            else:
                units = conv[material.thermal__coefficient_expansion_linear_unit]
            material.thermal__coefficient_expansion_linear *= units
        if material.thermal__conductivity_defined:
            if any([material.thermal__conductivity_unit == u for u in noUnits]):
                units = 1.0
            else:
                units = conv[material.thermal__conductivity_unit]
            material.thermal__conductivity *= units
        if material.thermal__capacity_specific_heat_defined:
            if any([material.thermal__capacity_specific_heat_unit == u for u in noUnits]):
                units = 1.0
            else:
                units = conv[material.thermal__capacity_specific_heat_unit]
            material.thermal__capacity_specific_heat *= units