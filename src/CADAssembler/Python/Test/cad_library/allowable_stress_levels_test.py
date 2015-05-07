__author__ = 'rowens'

import sys
sys.path.append("..\..\..\..\Python27Packages\cad_library\cad_library")
import cad_exception_classes as CADExcep
import allowable_stress_levels
import material_properties

# Create list of material names
material_names_list = []
material_names_list.append("aluminum_alloy_6061_t6")
material_names_list.append("aluminum_alloy_6061_t4")
material_names_list.append("aluminum_qc7")
material_names_list.append("steel_stainless_321")

# Create dictionary for material library properties
material_lib_props_dict = {}  # AnalysisMaterialProperties dictionary, material-name key

# Retrieve material library properties
try:
    material_properties.get_props_from_material_library(material_names_list, material_lib_props_dict)
except CADExcep.CADError as e:
    print e.message

# Print the retrieved data
print "*************** Material Properties ************"
for k in material_lib_props_dict:
    print
    print "key: ", k
    print str(material_lib_props_dict[k])


# Compute and print material allowables
print "*************** Material Allowables ************"
#
# WARNING - The following exception occurs for aluminum_qc7
#            Error, Function: compute_allowable_stress_levels, Message: Material aluminum_qc7 does not have a
#          shear strength defined. Aluminum must have shear strength defined.
#
for k in material_lib_props_dict:
    print
    print "key: ", k
    print str(material_lib_props_dict[k])

    allowables = allowable_stress_levels.AnalysisMaterialPropertiesAllowables(k)
    try:
        allowable_stress_levels.compute_allowable_stress_levels(
                        allowable_stress_levels.MECHANICAL_INFINITE_CYCLES_INDICATOR,
                        material_lib_props_dict[k],
                        allowables)
        print
        print "Allowables - Pa"
        print str(allowables)
        print
        # Convert from PA to MPa
        allowable_stress_levels.convert_allowables_from_Pa_to_MPa(allowables)
        print
        print "Allowables - MPa"
        print str(allowables)
        print

    except CADExcep.CADError as e:
        print e.message

"""
# Manual (i.e. not using the material libary) approach for future testing.
# Requires import units_utils

properties = material_properties.AnalysisMaterialProperties("aluminum_alloy_6061_t6",  units_utils.MECHANICAL_UNIT_STRESS_PA)

properties.mechanical__modulus_elastic = 71000000000.0
properties.mechanical__ratio_poissons =  0.33
properties.mechanical__strength_tensile_yield = 241000000.0
properties.mechanical__strength_tensile_ultimate = 290000000.0
properties.mechanical__strength_shear = 207000000.0
properties.mechanical__strength_bearing_yield = 386000000.0
properties.mechanical__strength_bearing_ultimate = 607000000.0
properties.mechanical__strength_fatigue = 96500000.0
properties.mechanical__strength_fatigue_cycles = 500000000.0

properties.mechanical__modulus_elastic_defined = True
properties.mechanical__ratio_poissons_defined = True
properties.mechanical__strength_tensile_yield_defined = True
properties.mechanical__strength_tensile_ultimate_defined = True
properties.mechanical__strength_shear_defined = True
properties.mechanical__strength_bearing_yield_defined = True
properties.mechanical__strength_bearing_ultimate_defined = True
properties.mechanical__strength_fatigue_defined = True
properties.mechanical__strength_fatigue_cycles_defined = False

properties.mechanical__modulus_elastic_units = "Pa"
properties.mechanical__ratio_poissons_units = units_utils.MECHANICAL_UNIT_NONE
properties.mechanical__strength_tensile_yield_units = "Pa"
properties.mechanical__strength_tensile_ultimate_units = "Pa"
properties.mechanical__strength_shear_units = "Pa"
properties.mechanical__strength_bearing_yield_units = "Pa"
properties.mechanical__strength_bearing_ultimate_units = "Pa"
properties.mechanical__strength_fatigue_units = "Pa"
properties.mechanical__strength_fatigue_cycles_units =  units_utils.MECHANICAL_UNIT_NONE

properties.print_self()

print

allowables = allowable_stress_levels.AnalysisMaterialPropertiesAllowables("aluminum_alloy_6061_t6")


try:
    #compute_allowable_stress_levels(1, properties, allowables)
    allowable_stress_levels.compute_allowable_stress_levels(allowable_stress_levels.MECHANICAL_INFINITE_CYCLES_INDICATOR, properties, allowables)
    print
    allowables.print_self()
except CADExcep.CADError as e:
        print e.message
"""

