__author__ = 'rowens'

# Description:
#    This modules retrieves material properties from the material library and populates AnalysisMaterialProperties
#    with the retrieved values.
#    The material library must be installed on the user's machine per the META installer (e.g. META_x64.msi).
#    The material library is typically installed at C:\Users\Public\Documents\META Documents\MaterialLibrary.
#
#    See material_properties_test.py for an exampled of how to invoke this module.
#    Also, see the contract (shown below in this module) on get_props_from_material_library.
#
#    See ..\..\..\CADAssembler\Python\Test\cad_library\material_properties_test.py for an example of how to invoke this module.

import cad_exception_classes as CADExcep
import _winreg
import os
import sys
import uuid, ctypes

import unit_utils

class MaterialProperties_Base:
    def __init___(self, in_material_name):
        self.material_name = in_material_name
        self.density_defined = False
        self.density = 0.0
        self.density_unit = unit_utils.MECHANICAL_UNIT_NOT_DEFINED


class AnalysisMaterialProperties(MaterialProperties_Base):
    def __init__(self, in_material_name, in_material_data):
        MaterialProperties_Base.__init___(self, in_material_name)
        #

        self.mechanical__modulus_elastic_defined = False
        self.mechanical__ratio_poissons_defined = False
        self.mechanical__strength_tensile_yield_defined = False
        self.mechanical__strength_tensile_ultimate_defined = False
        self.mechanical__strength_shear_defined = False
        self.mechanical__strength_bearing_yield_defined = False
        self.mechanical__strength_bearing_ultimate_defined = False
        self.mechanical__strength_fatigue_defined = False
        self.mechanical__strength_fatigue_cycles_defined = False
        self.thermal__coefficient_expansion_linear_defined = False
        self.thermal__conductivity_defined = False
        self.thermal__capacity_specific_heat_defined = False
        #

        self.mechanical__modulus_elastic = 0.0
        self.mechanical__ratio_poissons = 0.0
        self.mechanical__strength_tensile_yield = 0.0
        self.mechanical__strength_tensile_ultimate = 0.0
        self.mechanical__strength_shear = 0.0
        self.mechanical__strength_bearing_yield = 0.0
        self.mechanical__strength_bearing_ultimate = 0.0
        self.mechanical__strength_fatigue = 0.0
        self.mechanical__strength_fatigue_cycles = 0.0
        self.thermal__coefficient_expansion_linear = 0.0
        self.thermal__conductivity = 0.0
        self.thermal__capacity_specific_heat = 0.0
        #
        #self.density_unit = units_utils.MECHANICAL_UNIT_DENSITY_KG_PER_M3

        self.mechanical__modulus_elastic_unit = unit_utils.MECHANICAL_UNIT_NOT_DEFINED
        self.mechanical__ratio_poissons_unit = unit_utils.MECHANICAL_UNIT_NOT_DEFINED
        self.mechanical__strength_tensile_yield_unit = unit_utils.MECHANICAL_UNIT_NOT_DEFINED
        self.mechanical__strength_tensile_ultimate_unit = unit_utils.MECHANICAL_UNIT_NOT_DEFINED
        self.mechanical__strength_shear_unit = unit_utils.MECHANICAL_UNIT_NOT_DEFINED
        self.mechanical__strength_bearing_yield_unit = unit_utils.MECHANICAL_UNIT_NOT_DEFINED
        self.mechanical__strength_bearing_ultimate_unit = unit_utils.MECHANICAL_UNIT_NOT_DEFINED
        self.mechanical__strength_fatigue_unit = unit_utils.MECHANICAL_UNIT_NOT_DEFINED
        self.mechanical__strength_fatigue_cycles_unit = unit_utils.MECHANICAL_UNIT_NOT_DEFINED
        self.thermal__coefficient_expansion_linear_unit = unit_utils.MECHANICAL_UNIT_NOT_DEFINED
        self.thermal__conductivity_unit = unit_utils.MECHANICAL_UNIT_NOT_DEFINED
        self.thermal__capacity_specific_heat_unit = unit_utils.MECHANICAL_UNIT_NOT_DEFINED

        self.populate_material_props(in_material_data)

    def __str__(self):
        return "material_name                              " + self.material_name \
        + "\ndensity_defined                               " + str(self.density_defined) \
        + "\nmechanical__modulus_elastic_defined           " + str(self.mechanical__modulus_elastic_defined) \
        + "\nmechanical__ratio_poissons_defined            " + str(self.mechanical__ratio_poissons_defined) \
        + "\nmechanical__strength_tensile_yield_defined    " + str(self.mechanical__strength_tensile_yield_defined) \
        + "\nmechanical__strength_tensile_ultimate_defined " + str(self.mechanical__strength_tensile_ultimate_defined) \
        + "\nmechanical__strength_shear_defined            " + str(self.mechanical__strength_shear_defined) \
        + "\nmechanical__strength_bearing_yield_defined    " + str(self.mechanical__strength_bearing_yield_defined) \
        + "\nmechanical__strength_bearing_ultimate_defined " + str(self.mechanical__strength_bearing_ultimate_defined) \
        + "\nmechanical__strength_fatigue_defined          " + str(self.mechanical__strength_fatigue_defined) \
        + "\nmechanical__strength_fatigue_cycles_defined   " + str(self.mechanical__strength_fatigue_cycles_defined) \
        + "\nthermal__coefficient_expansion_linear_defined " + str(self.thermal__coefficient_expansion_linear_defined) \
        + "\nthermal__conductivity_defined                 " + str(self.thermal__conductivity_defined) \
        + "\nthermal__capacity_specific_heat_defined       " + str(self.thermal__capacity_specific_heat_defined) \
        + "\n" \
        + "\ndensity                                       " + str(self.density) \
        + "\nmechanical__modulus_elastic                   " +  str(self.mechanical__modulus_elastic) \
        + "\nmechanical__ratio_poissons                    " +  str(self.mechanical__ratio_poissons) \
        + "\nmechanical__strength_tensile_yield            " +  str(self.mechanical__strength_tensile_yield) \
        + "\nmechanical__strength_tensile_ultimate         " +  str(self.mechanical__strength_tensile_ultimate) \
        + "\nmechanical__strength_shear                    " +  str(self.mechanical__strength_shear) \
        + "\nmechanical__strength_bearing_yield            " +  str(self.mechanical__strength_bearing_yield) \
        + "\nmechanical__strength_bearing_ultimate         " +  str(self.mechanical__strength_bearing_ultimate) \
        + "\nmechanical__strength_fatigue                  " +  str(self.mechanical__strength_fatigue) \
        + "\nmechanical__strength_fatigue_cycles           " +  str(self.mechanical__strength_fatigue_cycles) \
        + "\nthermal__coefficient_expansion_linear         " +  str(self.thermal__coefficient_expansion_linear) \
        + "\nthermal__conductivity                         " + str(self.thermal__conductivity) \
        + "\nthermal__capacity_specific_heat               " + str(self.thermal__capacity_specific_heat) \
        + "\n" \
        + "\ndensity_unit                                  " + str(self.density_unit) \
        + "\nmechanical__modulus_elastic_unit              " + str(self.mechanical__modulus_elastic_unit) \
        + "\nmechanical__ratio_poissons_unit               " + str(self.mechanical__ratio_poissons_unit) \
        + "\nmechanical__strength_tensile_yield_unit       " + str(self.mechanical__strength_tensile_yield_unit) \
        + "\nmechanical__strength_tensile_ultimate_unit    " + str(self.mechanical__strength_tensile_ultimate_unit) \
        + "\nmechanical__strength_shear_unit               " + str(self.mechanical__strength_shear_unit) \
        + "\nmechanical__strength_bearing_yield_unit       " + str(self.mechanical__strength_bearing_yield_unit) \
        + "\nmechanical__strength_bearing_ultimate_unit    " + str(self.mechanical__strength_bearing_ultimate_unit) \
        + "\nmechanical__strength_fatigue_unit             " + str(self.mechanical__strength_fatigue_unit) \
        + "\nmechanical__strength_fatigue_cycles_unit      " + str(self.mechanical__strength_fatigue_cycles_unit) \
        + "\nthermal__coefficient_expansion_linear_unit    " + str(self.thermal__coefficient_expansion_linear_unit) \
        + "\nthermal__conductivity_unit                    " + str(self.thermal__conductivity_unit) \
        + "\nthermal__capacity_specific_heat_unit          " + str(self.thermal__capacity_specific_heat_unit)


    def populate_material_props(self,
                                in_material_data):


        #out_material_lib_props_dict[in_material_name] = AnalysisMaterialProperties(in_material_name)

        # density
        if "density" in in_material_data:
            if (in_material_data["density"]["value"] != "None"):
                self.density = in_material_data["density"]["value"]
                if ("unit" in in_material_data["density"]):
                    self.density_unit = in_material_data["density"]["unit"]
                else:
                    self.density_unit = unit_utils.MECHANICAL_UNIT_NONE
                    self.density_defined = True

        # mechanical__modulus_elastic
        if "mechanical__modulus_elastic" in in_material_data:
            if (in_material_data["mechanical__modulus_elastic"]["value"] != "None"):
                self.mechanical__modulus_elastic = \
                    in_material_data["mechanical__modulus_elastic"]["value"]
                if ("unit" in in_material_data["mechanical__modulus_elastic"]):
                    self.mechanical__modulus_elastic_unit = \
                        in_material_data["mechanical__modulus_elastic"]["unit"]
                else:
                    self.mechanical__modulus_elastic_unit = unit_utils.MECHANICAL_UNIT_NONE
                self.mechanical__modulus_elastic_defined = True

        # mechanical__ratio_poissons
        if "mechanical__ratio_poissons" in in_material_data:
            if (in_material_data["mechanical__ratio_poissons"]["value"] != "None"):
                self.mechanical__ratio_poissons = \
                    in_material_data["mechanical__ratio_poissons"]["value"]
                if ("unit" in in_material_data["mechanical__ratio_poissons"]):
                    self.mechanical__ratio_poissons_unit = \
                        in_material_data["mechanical__ratio_poissons"]["unit"]
                else:
                    self.mechanical__ratio_poissons_unit = unit_utils.MECHANICAL_UNIT_NONE
                self.mechanical__ratio_poissons_defined = True

        # mechanical__strength_tensile_yield
        if "mechanical__strength_tensile_yield" in in_material_data:
            if (in_material_data["mechanical__strength_tensile_yield"]["value"] != "None"):
                self.mechanical__strength_tensile_yield = \
                    in_material_data["mechanical__strength_tensile_yield"]["value"]
                if ("unit" in in_material_data["mechanical__strength_tensile_yield"]):
                    self.mechanical__strength_tensile_yield_unit = \
                        in_material_data["mechanical__strength_tensile_yield"]["unit"]
                else:
                    self.mechanical__strength_tensile_yield_unit = unit_utils.MECHANICAL_UNIT_NONE
                self.mechanical__strength_tensile_yield_defined = True

        # mechanical__strength_tensile_ultimate
        if "mechanical__strength_tensile_ultimate" in in_material_data:
            if (in_material_data["mechanical__strength_tensile_ultimate"]["value"] != "None"):
                self.mechanical__strength_tensile_ultimate = \
                    in_material_data["mechanical__strength_tensile_ultimate"]["value"]
                if ("unit" in in_material_data["mechanical__strength_tensile_ultimate"]):
                    self.mechanical__strength_tensile_ultimate_unit = \
                        in_material_data["mechanical__strength_tensile_ultimate"]["unit"]
                else:
                    self.mechanical__strength_tensile_ultimate_unit = unit_utils.MECHANICAL_UNIT_NONE
                self.mechanical__strength_tensile_ultimate_defined = True

        # mechanical__strength_bearing_yield
        if "mechanical__strength_bearing_yield" in in_material_data:
            if (in_material_data["mechanical__strength_bearing_yield"]["value"] != "None"):
                self.mechanical__strength_bearing_yield = \
                    in_material_data["mechanical__strength_bearing_yield"]["value"]
                if ("unit" in in_material_data["mechanical__strength_bearing_yield"]):
                    self.mechanical__strength_bearing_yield_unit = \
                        in_material_data["mechanical__strength_bearing_yield"]["unit"]
                else:
                    self.mechanical__strength_bearing_yield_unit = unit_utils.MECHANICAL_UNIT_NONE
                self.mechanical__strength_bearing_yield_defined = True

        # mechanical__strength_bearing_ultimate
        if "mechanical__strength_bearing_ultimate" in in_material_data:
            if (in_material_data["mechanical__strength_bearing_ultimate"]["value"] != "None"):
                self.mechanical__strength_bearing_ultimate = \
                    in_material_data["mechanical__strength_bearing_ultimate"]["value"]
                if ("unit" in in_material_data["mechanical__strength_bearing_ultimate"]):
                    self.mechanical__strength_bearing_ultimate_unit = \
                        in_material_data["mechanical__strength_bearing_ultimate"]["unit"]
                else:
                    self.mechanical__strength_bearing_ultimate_unit = unit_utils.MECHANICAL_UNIT_NONE
                self.mechanical__strength_bearing_ultimate_defined = True

        # mechanical__strength_shear
        if "mechanical__strength_shear" in in_material_data:
            if (in_material_data["mechanical__strength_shear"]["value"] != "None"):
                self.mechanical__strength_shear = \
                    in_material_data["mechanical__strength_shear"]["value"]
                if ("unit" in in_material_data["mechanical__strength_shear"]):
                    self.mechanical__strength_shear_unit = \
                        in_material_data["mechanical__strength_shear"]["unit"]
                else:
                    self.mechanical__strength_shear_unit = unit_utils.MECHANICAL_UNIT_NONE
                self.mechanical__strength_shear_defined = True

        # mechanical__strength_fatigue
        if "mechanical__strength_fatigue" in in_material_data:
            if (in_material_data["mechanical__strength_fatigue"]["value"] != "None"):
                self.mechanical__strength_fatigue = \
                    in_material_data["mechanical__strength_fatigue"]["value"]
                if ("unit" in in_material_data["mechanical__strength_fatigue"]):
                    self.mechanical__strength_fatigue_unit = \
                        in_material_data["mechanical__strength_fatigue"]["unit"]
                else:
                    self.mechanical__strength_fatigue_unit = unit_utils.MECHANICAL_UNIT_NONE
                self.mechanical__strength_fatigue_defined = True

        # mechanical__strength_fatigue_cycles
        if "mechanical__strength_fatigue_cycles" in in_material_data:
            if (in_material_data["mechanical__strength_fatigue_cycles"]["value"] != "None"):
                self.mechanical__strength_fatigue_cycles = \
                    in_material_data["mechanical__strength_fatigue_cycles"]["value"]
                if ("unit" in in_material_data["mechanical__strength_fatigue_cycles"]):
                    self.mechanical__strength_fatigue_cycles_unit = \
                        in_material_data["mechanical__strength_fatigue_cycles"]["unit"]
                else:
                    self.mechanical__strength_fatigue_cycles_unit = unit_utils.MECHANICAL_UNIT_CYCLE
                self.mechanical__strength_fatigue_cycles_defined = True

        # thermal__coefficient_expansion_linear
        if "thermal__coefficient_expansion_linear" in in_material_data:
            if (in_material_data["thermal__coefficient_expansion_linear"]["value"] != "None"):
                self.thermal__coefficient_expansion_linear = \
                    in_material_data["thermal__coefficient_expansion_linear"]["value"]
                if ("unit" in in_material_data["thermal__coefficient_expansion_linear"]):
                    self.thermal__coefficient_expansion_linear_unit = \
                        in_material_data["thermal__coefficient_expansion_linear"]["unit"]
                else:
                    self.thermal__coefficient_expansion_linear_unit = unit_utils.MECHANICAL_UNIT_NONE
                self.thermal__coefficient_expansion_linear_defined = True

        # thermal__conductivity
        if "thermal__conductivity" in in_material_data:
            if (in_material_data["thermal__conductivity"]["value"] != "None"):
                self.thermal__conductivity = in_material_data["thermal__conductivity"][
                    "value"]
                if ("unit" in in_material_data["thermal__conductivity"]):
                    self.thermal__conductivity_unit = \
                        in_material_data["thermal__conductivity"]["unit"]
                else:
                    self.thermal__conductivity_unit = unit_utils.MECHANICAL_UNIT_NONE
                self.thermal__conductivity_defined = True

        # thermal__capacity_specific_heat
        if "thermal__capacity_specific_heat" in in_material_data:
            if (in_material_data["thermal__capacity_specific_heat"]["value"] != "None"):
                self.thermal__capacity_specific_heat = \
                    in_material_data["thermal__capacity_specific_heat"]["value"]
                if ("unit" in in_material_data["thermal__capacity_specific_heat"]):
                    self.thermal__capacity_specific_heat_unit = \
                        in_material_data["thermal__capacity_specific_heat"]["unit"]
                else:
                    self.thermal__capacity_specific_heat_unit = unit_utils.MECHANICAL_UNIT_NONE
                self.thermal__capacity_specific_heat_defined = True


class AnalysisMaterialProperties_Orthotropic(MaterialProperties_Base):
    def __init__(self, in_material_name, in_material_data):
        MaterialProperties_Base.__init___(self, in_material_name)

        #Orthotropic material properties begin here
        self.mechanical__modulus_elastic_11 = 0.0
        self.mechanical__modulus_elastic_11_defined = False
        self.mechanical__modulus_elastic_22 = 0.0
        self.mechanical__modulus_elastic_22_defined = False
        self.mechanical__modulus_elastic_33 = 0.0
        self.mechanical__modulus_elastic_33_defined = False
        self.mechanical__modulus_shear_12 = 0.0
        self.mechanical__modulus_shear_12_defined = False
        self.mechanical__modulus_shear_23 = 0.0
        self.mechanical__modulus_shear_23_defined = False
        self.mechanical__modulus_shear_31 = 0.0
        self.mechanical__modulus_shear_31_defined = False
        self.mechanical__ratio_poissons_12 = 0.0
        self.mechanical__ratio_poissons_12_defined = False
        self.mechanical__ratio_poissons_23 = 0.0
        self.mechanical__ratio_poissons_23_defined = False
        self.mechanical__ratio_poissons_31 = 0.0
        self.mechanical__ratio_poissons_31_defined = False
        self.physical__cured_ply_thickness = 0.0
        self.physical__cured_ply_thickness_defined = False
        # added 8-5-2016
        self.mechanical_tension_stress_limit_11 = 0.0
        self.mechanical_tension_stress_limit_11_defined = False
        self.mechanical_tension_stress_limit_22 = 0.0
        self.mechanical_tension_stress_limit_22_defined = False
        self.mechanical_tension_stress_limit_33 = 0.0
        self.mechanical_tension_stress_limit_33_defined = False
        self.mechanical_compress_stress_limit_11 = 0.0
        self.mechanical_compress_stress_limit_11_defined = False
        self.mechanical_compress_stress_limit_22 = 0.0
        self.mechanical_compress_stress_limit_22_defined = False
        self.mechanical_compress_stress_limit_33 = 0.0
        self.mechanical_compress_stress_limit_33_defined = False
        self.mechanical_shear_stress_limit_12 = 0.0
        self.mechanical_shear_stress_limit_12_defined = False
        self.mechanical_shear_stress_limit_23 = 0.0
        self.mechanical_shear_stress_limit_23_defined = False
        self.mechanical_shear_stress_limit_31 = 0.0
        self.mechanical_shear_stress_limit_31_defined = False
        self.mechanical_shear_stress_bond = 0.0
        self.mechanical_shear_stress_bond_defined = False
        self.mechanical_interactive_strength_12 = 0.0
        self.mechanical_interactive_strength_12_defined = False
        self.mechanical_interactive_strength_23 = 0.0
        self.mechanical_interactive_strength_23_defined = False
        self.mechanical_interactive_strength_31 = 0.0
        self.mechanical_interactive_strength_31_defined = False

        self.populate_material_props(in_material_data)

    def __str__(self):
        return "material_name                              " + self.material_name \
        + "\nmechanical__modulus_elastic_11_defined        " + str(self.mechanical__modulus_elastic_11_defined) \
        + "\nmechanical__modulus_elastic_22_defined        " + str(self.mechanical__modulus_elastic_22_defined) \
        + "\nmechanical__modulus_elastic_33_defined        " + str(self.mechanical__modulus_elastic_33_defined) \
        + "\nmechanical__modulus_shear_12_defined          " + str(self.mechanical__modulus_shear_12_defined) \
        + "\nmechanical__modulus_shear_23_defined          " + str(self.mechanical__modulus_shear_23_defined) \
        + "\nmechanical__modulus_shear_31_defined          " + str(self.mechanical__modulus_shear_31_defined) \
        + "\nmechanical__ratio_poissons_12_defined         " + str(self.mechanical__ratio_poissons_12_defined) \
        + "\nmechanical__ratio_poissons_23_defined         " + str(self.mechanical__ratio_poissons_23_defined) \
        + "\nmechanical__ratio_poissons_31_defined         " + str(self.mechanical__ratio_poissons_31_defined) \
        + "\nphysical__cured_ply_thickness_defined         " + str(self.physical__cured_ply_thickness_defined) \
        + "\ndensity_defined                               " + str(self.density_defined) \
        + "\ndensity                                       " + str(self.density) \
        + "\ndensity_unit                                  " + str(self.density_unit) \
        + "\nmechanical__modulus_elastic_11                " + str(self.mechanical__modulus_elastic_11) \
        + "\nmechanical__modulus_elastic_22                " + str(self.mechanical__modulus_elastic_22) \
        + "\nmechanical__modulus_elastic_33                " + str(self.mechanical__modulus_elastic_33) \
        + "\nmechanical__modulus_shear_12                  " + str(self.mechanical__modulus_shear_12) \
        + "\nmechanical__modulus_shear_23                  " + str(self.mechanical__modulus_shear_23) \
        + "\nmechanical__modulus_shear_31                  " + str(self.mechanical__modulus_shear_31) \
        + "\nmechanical__ratio_poissons_12                 " + str(self.mechanical__ratio_poissons_12) \
        + "\nmechanical__ratio_poissons_23                 " + str(self.mechanical__ratio_poissons_23) \
        + "\nmechanical__ratio_poissons_31                 " + str(self.mechanical__ratio_poissons_31) \
        + "\nphysical__cured_ply_thickness                 " + str(self.physical__cured_ply_thickness) \
        + "\nmechanical_tension_stress_limit_11            " + str(self.mechanical_tension_stress_limit_11) \
        + "\nmechanical_tension_stress_limit_11_defined    " + str(self.mechanical_tension_stress_limit_11_defined) \
        + "\nmechanical_tension_stress_limit_22            " + str(self.mechanical_tension_stress_limit_22) \
        + "\nmechanical_tension_stress_limit_22_defined    " + str(self.mechanical_tension_stress_limit_22_defined) \
        + "\nmechanical_tension_stress_limit_33            " + str(self.mechanical_tension_stress_limit_33) \
        + "\nmechanical_tension_stress_limit_33_defined    " + str(self.mechanical_tension_stress_limit_33_defined) \
        + "\nmechanical_compress_stress_limit_11           " + str(self.mechanical_compress_stress_limit_11) \
        + "\nmechanical_compress_stress_limit_11_defined   " + str(self.mechanical_compress_stress_limit_11_defined) \
        + "\nmechanical_compress_stress_limit_22           " + str(self.mechanical_compress_stress_limit_22) \
        + "\nmechanical_compress_stress_limit_22_defined   " + str(self.mechanical_compress_stress_limit_22_defined) \
        + "\nmechanical_compress_stress_limit_33           " + str(self.mechanical_compress_stress_limit_33) \
        + "\nmechanical_compress_stress_limit_33_defined   " + str(self.mechanical_compress_stress_limit_33_defined) \
        + "\nmechanical_shear_stress_limit_12              " + str(self.mechanical_shear_stress_limit_12) \
        + "\nmechanical_shear_stress_limit_12_defined      " + str(self.mechanical_shear_stress_limit_12_defined) \
        + "\nmechanical_shear_stress_limit_23              " + str(self.mechanical_shear_stress_limit_23) \
        + "\nmechanical_shear_stress_limit_23_defined      " + str(self.mechanical_shear_stress_limit_23_defined) \
        + "\nmechanical_shear_stress_limit_31              " + str(self.mechanical_shear_stress_limit_31) \
        + "\nmechanical_shear_stress_limit_31_defined      " + str(self.mechanical_shear_stress_limit_31_defined) \
        + "\nmechanical_shear_stress_bond                  " + str(self.mechanical_shear_stress_bond) \
        + "\nmechanical_shear_stress_bond_defined          " + str(self.mechanical_shear_stress_bond_defined) \
        + "\nmechanical_interactive_strength_12            " + str(self.mechanical_interactive_strength_12) \
        + "\nmechanical_interactive_strength_12_defined    " + str(self.mechanical_interactive_strength_12_defined) \
        + "\nmechanical_interactive_strength_23            " + str(self.mechanical_interactive_strength_23) \
        + "\nmechanical_interactive_strength_23_defined    " + str(self.mechanical_interactive_strength_23_defined) \
        + "\nmechanical_interactive_strength_31            " + str(self.mechanical_interactive_strength_31) \
        + "\nmechanical_interactive_strength_31_defined    " + str(self.mechanical_interactive_strength_31_defined)


    def populate_material_props(self,
                                in_material_data):

        #out_material_lib_props_dict[in_material_name] = AnalysisMaterialProperties_Orthotropic(in_material_name)

        if "density" in in_material_data:
            if (in_material_data["density"]["value"] != "None"):
                self.density = in_material_data["density"]["value"]
                if ("unit" in in_material_data["density"]):
                    self.density_unit = in_material_data["density"]["unit"]
                else:
                    self.density_unit = unit_utils.MECHANICAL_UNIT_NONE
                self.density_defined = True

        if "mechanical__modulus_elastic_11" in in_material_data:
            if (in_material_data["mechanical__modulus_elastic_11"]["value"] != "None"):
                self.mechanical__modulus_elastic_11 = \
                    in_material_data["mechanical__modulus_elastic_11"]["value"]
                self.mechanical__modulus_elastic_11_defined = True

        if "mechanical__modulus_elastic_22" in in_material_data:
            if (in_material_data["mechanical__modulus_elastic_22"]["value"] != "None"):
                self.mechanical__modulus_elastic_22 = \
                    in_material_data["mechanical__modulus_elastic_22"]["value"]
                self.mechanical__modulus_elastic_22_defined = True

        if "mechanical__modulus_elastic_33" in in_material_data:
            if (in_material_data["mechanical__modulus_elastic_33"]["value"] != "None"):
                self.mechanical__modulus_elastic_33 = \
                    in_material_data["mechanical__modulus_elastic_33"]["value"]
                self.mechanical__modulus_elastic_33_defined = True

        if "mechanical__modulus_shear_12" in in_material_data:
            if (in_material_data["mechanical__modulus_shear_12"]["value"] != "None"):
                self.mechanical__modulus_shear_12 = \
                    in_material_data["mechanical__modulus_shear_12"]["value"]
                self.mechanical__modulus_shear_12_defined = True

        if "mechanical__modulus_shear_23" in in_material_data:
            if (in_material_data["mechanical__modulus_shear_23"]["value"] != "None"):
                self.mechanical__modulus_shear_23 = \
                    in_material_data["mechanical__modulus_shear_23"]["value"]
                self.mechanical__modulus_shear_23_defined = True

        if "mechanical__modulus_shear_31" in in_material_data:
            if (in_material_data["mechanical__modulus_shear_31"]["value"] != "None"):
                self.mechanical__modulus_shear_31 = \
                    in_material_data["mechanical__modulus_shear_31"]["value"]
                self.mechanical__modulus_shear_31_defined = True

        if "mechanical__ratio_poissons_12" in in_material_data:
            if (in_material_data["mechanical__ratio_poissons_12"]["value"] != "None"):
                self.mechanical__ratio_poissons_12 = \
                    in_material_data["mechanical__ratio_poissons_12"]["value"]
                self.mechanical__ratio_poissons_12_defined = True

        if "mechanical__ratio_poissons_23" in in_material_data:
            if (in_material_data["mechanical__ratio_poissons_23"]["value"] != "None"):
                self.mechanical__ratio_poissons_23 = \
                    in_material_data["mechanical__ratio_poissons_23"]["value"]
                self.mechanical__ratio_poissons_23_defined = True

        if "mechanical__ratio_poissons_31" in in_material_data:
            if (in_material_data["mechanical__ratio_poissons_31"]["value"] != "None"):
                self.mechanical__ratio_poissons_31 = \
                    in_material_data["mechanical__ratio_poissons_31"]["value"]
                self.mechanical__ratio_poissons_31_defined = True

        if "density" in in_material_data:
            if (in_material_data["density"]["value"] != "None"):
                self.density = in_material_data["density"]["value"]
                if ("unit" in in_material_data["density"]):
                    self.density_unit = in_material_data["density"]["unit"]
                else:
                    self.density_unit = unit_utils.MECHANICAL_UNIT_NONE
                self.density_defined = True

        if "physical__cured_ply_thickness" in in_material_data:
            if (in_material_data["physical__cured_ply_thickness"]["value"] != "None"):
                self.physical__cured_ply_thickness = \
                    in_material_data["physical__cured_ply_thickness"]["value"]
                self.physical__cured_ply_thickness_defined = True

        # added 8-5-2016
        if "mechanical__Tension_Stress_Limit_11" in in_material_data:
            if (in_material_data["mechanical__Tension_Stress_Limit_11"]["value"] != "None"):
                self.mechanical_tension_stress_limit_11 = \
                    in_material_data["mechanical__Tension_Stress_Limit_11"]["value"]
                self.mechanical_tension_stress_limit_11_defined = True

        if "mechanical__Tension_Stress_Limit_22" in in_material_data:
            if (in_material_data["mechanical__Tension_Stress_Limit_22"]["value"] != "None"):
                self.mechanical_tension_stress_limit_22 = \
                    in_material_data["mechanical__Tension_Stress_Limit_22"]["value"]
                self.mechanical_tension_stress_limit_22_defined = True

        if "mechanical__Tension_Stress_Limit_33" in in_material_data:
            if (in_material_data["mechanical__Tension_Stress_Limit_33"]["value"] != "None"):
                self.mechanical_tension_stress_limit_33 = \
                    in_material_data["mechanical__Tension_Stress_Limit_33"]["value"]
                self.mechanical_tension_stress_limit_33_defined = True

        if "mechanical__Compress_Stress_Limit_11" in in_material_data:
            if (in_material_data["mechanical__Compress_Stress_Limit_11"]["value"] != "None"):
                self.mechanical_compress_stress_limit_11 = \
                    in_material_data["mechanical__Compress_Stress_Limit_11"]["value"]
                self.mechanical_compress_stress_limit_11_defined = True

        if "mechanical__Compress_Stress_Limit_22" in in_material_data:
            if (in_material_data["mechanical__Compress_Stress_Limit_22"]["value"] != "None"):
                self.mechanical_compress_stress_limit_22 = \
                    in_material_data["mechanical__Compress_Stress_Limit_22"]["value"]
                self.mechanical_compress_stress_limit_22_defined = True

        if "mechanical__Compress_Stress_Limit_33" in in_material_data:
            if (in_material_data["mechanical__Compress_Stress_Limit_33"]["value"] != "None"):
                self.mechanical_compress_stress_limit_33 = \
                    in_material_data["mechanical__Compress_Stress_Limit_33"]["value"]
                self.mechanical_compress_stress_limit_33_defined = True

        if "mechanical__Shear_Stress_Limit_12" in in_material_data:
            if (in_material_data["mechanical__Shear_Stress_Limit_12"]["value"] != "None"):
                self.mechanical_shear_stress_limit_12 = \
                    in_material_data["mechanical__Shear_Stress_Limit_12"]["value"]
                self.mechanical_shear_stress_limit_12_defined = True

        if "mechanical__Shear_Stress_Limit_23" in in_material_data:
            if (in_material_data["mechanical__Shear_Stress_Limit_23"]["value"] != "None"):
                self.mechanical_shear_stress_limit_23 = \
                    in_material_data["mechanical__Shear_Stress_Limit_23"]["value"]
                self.mechanical_shear_stress_limit_23_defined = True

        if "mechanical__Shear_Stress_Limit_31" in in_material_data:
            if (in_material_data["mechanical__Shear_Stress_Limit_31"]["value"] != "None"):
                self.mechanical_shear_stress_limit_31 = \
                    in_material_data["mechanical__Shear_Stress_Limit_31"]["value"]
                self.mechanical_shear_stress_limit_31_defined = True

        if "mechanical__Shear_Stress_Bond" in in_material_data:
            if (in_material_data["mechanical__Shear_Stress_Bond"]["value"] != "None"):
                self.mechanical_shear_stress_bond = \
                    in_material_data["mechanical__Shear_Stress_Bond"]["value"]
                self.mechanical_shear_stress_bond_defined = True

        if "mechanical__Interactive_Strength_12" in in_material_data:
            if (in_material_data["mechanical__Interactive_Strength_12"]["value"] != "None"):
                self.mechanical_interactive_strength_12 = \
                    in_material_data["mechanical__Interactive_Strength_12"]["value"]
                self.mechanical_interactive_strength_12_defined = True

        if "mechanical__Interactive_Strength_23" in in_material_data:
            if (in_material_data["mechanical__Interactive_Strength_23"]["value"] != "None"):
                self.mechanical_interactive_strength_23 = \
                    in_material_data["mechanical__Interactive_Strength_23"]["value"]
                self.mechanical_interactive_strength_23_defined = True

        if "mechanical__Interactive_Strength_31" in in_material_data:
            if (in_material_data["mechanical__Interactive_Strength_31"]["value"] != "None"):
                self.mechanical_interactive_strength_31 = \
                    in_material_data["mechanical__Interactive_Strength_31"]["value"]
                self.mechanical_interactive_strength_31_defined = True


            #    Description:
#        get_props_from_material_library retrieves material properties from the material library.
#        in_material_names_list must contain one or more material names (e.g. aluminum_alloy_6061_t6,
#        aluminum_alloy_6061_t4, aluminum_qc7, steel_stainless_321).
#        out_material_lib_props_dict is a dictionary with the key being the material name and the contents being
#         AnalysisMaterialProperties objects.
#
#    Pre-Conditions:
#        1) The material library must be installed on the user's machine per the META installer (e.g. META_x64.msi).
#           The material library is typically installed at C:\Users\Public\Documents\META Documents\MaterialLibrary;
#           however, the library could be installed at other locations and this function would locate it as long as it
#           was installed via the META installer.
#        2) out_material_lib_props_dict should be empty prior to calling this function.
#
#    Post-Conditions:
#        If an error occurs, this function would raise CADExcep.CADError
#        Possible causes of errors include:
#            Could not locate material library
#            Material name does not exists in the material library
#        If no exceptions/errors, return a populated out_material_lib_props_dict
#
#        Note - If a particular material property was not found, then those values are not populated within the
#                AnalysisMaterialProperties class.  It is the responsibility of the consumer of
#                to check if the needed properties are defined in AnalysisMaterialProperties.  Check the
#                pertinent _defined property (e.g. self.density_defined) to determine if a property was found
#                in the material library.
def get_props_from_material_library(in_material_names_list,
                                    out_material_lib_props_dict):  # AnalysisMaterialProperties dictionary

    function_name = "get_props_from_material_library"

    # Open the material library


    try:
        library_manager = get_material_library_manager()


    except Exception as e:
        raise CADExcep.CADError("Error, Function: " + function_name + ", Message: " + \
                            "Failed to locate/open the material library. Locate/open Error:  " + \
                            e.message)
    for material_name in in_material_names_list:
        try:
            material_data = library_manager.materialData(material_name)
        except Exception as e:
            raise CADExcep.CADError("Error, Function: " + function_name + ", Message: " +
                        "Failed to locate material " + material_name + " in material library. Error:  " +
                        e.message)

        out_material_lib_props_dict[material_name] = AnalysisMaterialProperties(material_name, material_data)

        """
        #replace zzz with the material property (e.g. replace zzz mechanical__ratio_poissons)
        # zzz
        if "zzz" in material_data:
            if ( material_data["zzz"]["value"]!="None" ):
                out_material_lib_props_dict[material_name].zzz = material_data["zzz"]["value"]
                if ("unit" in material_data["zzz"]):
                    out_material_lib_props_dict[material_name].zzz_unit = material_data["zzz"]["unit"]
                else:
                    out_material_lib_props_dict[material_name].zzz_unit = unit_utils.MECHANICAL_UNIT_NONE
                out_material_lib_props_dict[material_name].zzz_defined = True
        """

        # density
        if "density" in material_data:
            if (material_data["density"]["value"]!="None"):
                out_material_lib_props_dict[material_name].density = material_data["density"]["value"]
                if ("unit" in material_data["density"]):
                    out_material_lib_props_dict[material_name].density_unit = material_data["density"]["unit"]
                else:
                    out_material_lib_props_dict[material_name].density_unit = unit_utils.MECHANICAL_UNIT_NONE
                out_material_lib_props_dict[material_name].density_defined = True


        # mechanical__modulus_elastic
        if "mechanical__modulus_elastic" in material_data:
            if (material_data["mechanical__modulus_elastic"]["value"]!="None"):
                out_material_lib_props_dict[material_name].mechanical__modulus_elastic = material_data["mechanical__modulus_elastic"]["value"]
                if ("unit" in material_data["mechanical__modulus_elastic"]):
                    out_material_lib_props_dict[material_name].mechanical__modulus_elastic_unit = material_data["mechanical__modulus_elastic"]["unit"]
                else:
                    out_material_lib_props_dict[material_name].mechanical__modulus_elastic_unit = unit_utils.MECHANICAL_UNIT_NONE
                out_material_lib_props_dict[material_name].mechanical__modulus_elastic_defined = True
        

        # mechanical__ratio_poissons
        if "mechanical__ratio_poissons" in material_data:
            if (material_data["mechanical__ratio_poissons"]["value"]!="None"):
                out_material_lib_props_dict[material_name].mechanical__ratio_poissons = material_data["mechanical__ratio_poissons"]["value"]
                if ("unit" in material_data["mechanical__ratio_poissons"]):
                    out_material_lib_props_dict[material_name].mechanical__ratio_poissons_unit = material_data["mechanical__ratio_poissons"]["unit"]
                else:
                    out_material_lib_props_dict[material_name].mechanical__ratio_poissons_unit = unit_utils.MECHANICAL_UNIT_NONE
                out_material_lib_props_dict[material_name].mechanical__ratio_poissons_defined = True

        # mechanical__strength_tensile_yield
        if "mechanical__strength_tensile_yield" in material_data:
            if (material_data["mechanical__strength_tensile_yield"]["value"]!="None"):
                out_material_lib_props_dict[material_name].mechanical__strength_tensile_yield = material_data["mechanical__strength_tensile_yield"]["value"]
                if ("unit" in material_data["mechanical__strength_tensile_yield"]):
                    out_material_lib_props_dict[material_name].mechanical__strength_tensile_yield_unit = material_data["mechanical__strength_tensile_yield"]["unit"]
                else:
                    out_material_lib_props_dict[material_name].mechanical__strength_tensile_yield_unit = unit_utils.MECHANICAL_UNIT_NONE
                out_material_lib_props_dict[material_name].mechanical__strength_tensile_yield_defined = True

        # mechanical__strength_tensile_ultimate
        if "mechanical__strength_tensile_ultimate" in material_data:
            if (material_data["mechanical__strength_tensile_ultimate"]["value"]!="None"):
                out_material_lib_props_dict[material_name].mechanical__strength_tensile_ultimate = material_data["mechanical__strength_tensile_ultimate"]["value"]
                if ("unit" in material_data["mechanical__strength_tensile_ultimate"]):
                    out_material_lib_props_dict[material_name].mechanical__strength_tensile_ultimate_unit = material_data["mechanical__strength_tensile_ultimate"]["unit"]
                else:
                    out_material_lib_props_dict[material_name].mechanical__strength_tensile_ultimate_unit = unit_utils.MECHANICAL_UNIT_NONE
                out_material_lib_props_dict[material_name].mechanical__strength_tensile_ultimate_defined = True

        # mechanical__strength_bearing_yield
        if "mechanical__strength_bearing_yield" in material_data:
            if (material_data["mechanical__strength_bearing_yield"]["value"]!="None"):
                out_material_lib_props_dict[material_name].mechanical__strength_bearing_yield = material_data["mechanical__strength_bearing_yield"]["value"]
                if ("unit" in material_data["mechanical__strength_bearing_yield"]):
                    out_material_lib_props_dict[material_name].mechanical__strength_bearing_yield_unit = material_data["mechanical__strength_bearing_yield"]["unit"]
                else:
                    out_material_lib_props_dict[material_name].mechanical__strength_bearing_yield_unit = unit_utils.MECHANICAL_UNIT_NONE
                out_material_lib_props_dict[material_name].mechanical__strength_bearing_yield_defined = True
            
            
        # mechanical__strength_bearing_ultimate
        if "mechanical__strength_bearing_ultimate" in material_data:
            if (material_data["mechanical__strength_bearing_ultimate"]["value"]!="None"):
                out_material_lib_props_dict[material_name].mechanical__strength_bearing_ultimate = material_data["mechanical__strength_bearing_ultimate"]["value"]
                if ("unit" in material_data["mechanical__strength_bearing_ultimate"]):
                    out_material_lib_props_dict[material_name].mechanical__strength_bearing_ultimate_unit = material_data["mechanical__strength_bearing_ultimate"]["unit"]
                else:
                    out_material_lib_props_dict[material_name].mechanical__strength_bearing_ultimate_unit = unit_utils.MECHANICAL_UNIT_NONE
                out_material_lib_props_dict[material_name].mechanical__strength_bearing_ultimate_defined = True
            
        # mechanical__strength_shear
        if "mechanical__strength_shear" in material_data:
            if (material_data["mechanical__strength_shear"]["value"]!="None"):
                out_material_lib_props_dict[material_name].mechanical__strength_shear = material_data["mechanical__strength_shear"]["value"]
                if ("unit" in material_data["mechanical__strength_shear"]):
                    out_material_lib_props_dict[material_name].mechanical__strength_shear_unit = material_data["mechanical__strength_shear"]["unit"]
                else:
                    out_material_lib_props_dict[material_name].mechanical__strength_shear_unit = unit_utils.MECHANICAL_UNIT_NONE
                out_material_lib_props_dict[material_name].mechanical__strength_shear_defined = True
            
        # mechanical__strength_fatigue
        if "mechanical__strength_fatigue" in material_data:
            if (material_data["mechanical__strength_fatigue"]["value"]!="None"):
                out_material_lib_props_dict[material_name].mechanical__strength_fatigue = material_data["mechanical__strength_fatigue"]["value"]
                if ("unit" in material_data["mechanical__strength_fatigue"]):
                    out_material_lib_props_dict[material_name].mechanical__strength_fatigue_unit = material_data["mechanical__strength_fatigue"]["unit"]
                else:
                    out_material_lib_props_dict[material_name].mechanical__strength_fatigue_unit = unit_utils.MECHANICAL_UNIT_NONE
                out_material_lib_props_dict[material_name].mechanical__strength_fatigue_defined = True

        # mechanical__strength_fatigue_cycles
        if "mechanical__strength_fatigue_cycles" in material_data:
            if (material_data["mechanical__strength_fatigue_cycles"]["value"]!="None"):
                out_material_lib_props_dict[material_name].mechanical__strength_fatigue_cycles = material_data["mechanical__strength_fatigue_cycles"]["value"]
                if ("unit" in material_data["mechanical__strength_fatigue_cycles"]):
                    out_material_lib_props_dict[material_name].mechanical__strength_fatigue_cycles_unit = material_data["mechanical__strength_fatigue_cycles"]["unit"]
                else:
                    out_material_lib_props_dict[material_name].mechanical__strength_fatigue_cycles_unit = unit_utils.MECHANICAL_UNIT_CYCLE
                out_material_lib_props_dict[material_name].mechanical__strength_fatigue_cycles_defined = True

        # thermal__coefficient_expansion_linear
        if "thermal__coefficient_expansion_linear" in material_data:
            if (material_data["thermal__coefficient_expansion_linear"]["value"]!="None"):
                out_material_lib_props_dict[material_name].thermal__coefficient_expansion_linear = material_data["thermal__coefficient_expansion_linear"]["value"]
                if ("unit" in material_data["thermal__coefficient_expansion_linear"]):
                    out_material_lib_props_dict[material_name].thermal__coefficient_expansion_linear_unit = material_data["thermal__coefficient_expansion_linear"]["unit"]
                else:
                    out_material_lib_props_dict[material_name].thermal__coefficient_expansion_linear_unit = unit_utils.MECHANICAL_UNIT_NONE
                out_material_lib_props_dict[material_name].thermal__coefficient_expansion_linear_defined = True

        # thermal__conductivity
        if "thermal__conductivity" in material_data:
            if (material_data["thermal__conductivity"]["value"]!="None"):
                out_material_lib_props_dict[material_name].thermal__conductivity = material_data["thermal__conductivity"]["value"]
                if ("unit" in material_data["thermal__conductivity"]):
                    out_material_lib_props_dict[material_name].thermal__conductivity_unit = material_data["thermal__conductivity"]["unit"]
                else:
                    out_material_lib_props_dict[material_name].thermal__conductivity_unit = unit_utils.MECHANICAL_UNIT_NONE
                out_material_lib_props_dict[material_name].thermal__conductivity_defined = True

        # thermal__capacity_specific_heat
        if "thermal__capacity_specific_heat" in material_data:
            if (material_data["thermal__capacity_specific_heat"]["value"]!="None"):
                out_material_lib_props_dict[material_name].thermal__capacity_specific_heat = material_data["thermal__capacity_specific_heat"]["value"]
                if ("unit" in material_data["thermal__capacity_specific_heat"]):
                    out_material_lib_props_dict[material_name].thermal__capacity_specific_heat_unit = material_data["thermal__capacity_specific_heat"]["unit"]
                else:
                    out_material_lib_props_dict[material_name].thermal__capacity_specific_heat_unit = unit_utils.MECHANICAL_UNIT_NONE
                out_material_lib_props_dict[material_name].thermal__capacity_specific_heat_defined = True

        """
        print material_data["mechanical__ratio_poissons"]["value"]
        print material_data["mechanical__strength_tensile_yield"]["value"]
        print material_data["mechanical__strength_tensile_ultimate"]["value"]
        print material_data["mechanical__strength_shear"]["value"]
        print material_data["mechanical__strength_bearing_yield"]["value"]
        print material_data["mechanical__strength_bearing_ultimate"]["value"]
        print material_data["mechanical__strength_fatigue"]["value"]
        print material_data["mechanical__strength_fatigue_cycles"]["value"]
        """





def get_props_from_material_library_typed(in_material_names_list,
                                          out_material_lib_props_dict):  # AnalysisMaterialProperties dictionary

    function_name = "get_props_from_material_library_typed"

    try:
        library_manager = get_material_library_manager()


    except Exception as e:
        raise CADExcep.CADError("Error, Function: " + function_name + ", Message: " + \
                                "Failed to locate/open the material library. Locate/open Error:  " + \
                                e.message)

    for material_name in in_material_names_list:
        #out_material_lib_props_dict[material_name] = AnalysisMaterialProperties(material_name)
        try:
            material_data = library_manager.materialData(material_name)
        except Exception as e:
            raise CADExcep.CADError("Error, Function: " + function_name + ", Message: " + \
                                        "Failed to locate material " + material_name + " in material library. Error:  " + \
                                        e.message)

        if ("mechanical__modulus_elastic_11" in material_data and
            "mechanical__modulus_elastic_22" in material_data and
            "mechanical__ratio_poissons_12" in material_data and
            "mechanical__ratio_poissons_23" in material_data):

            # orthotropic material
            out_material_lib_props_dict[material_name] = AnalysisMaterialProperties_Orthotropic(material_name, material_data)
        else:      #isotropic
            out_material_lib_props_dict[material_name] = AnalysisMaterialProperties(material_name, material_data)


# Open the material library
def get_material_library_manager():
    function_name = "get_material_library_manager"

    try:
        with _winreg.OpenKey(_winreg.HKEY_LOCAL_MACHINE, r'Software\META', 0,
                             _winreg.KEY_READ | _winreg.KEY_WOW64_32KEY) as key:
            META_PATH = _winreg.QueryValueEx(key, 'META_PATH')[0]

        materialLibInterfacePath = os.path.join(META_PATH, "bin", "Python27", "Lib", "site-packages",
                                                "material_library")

        sys.path.insert(0, materialLibInterfacePath)

        from MaterialLibraryInterface import LibraryManager

        path = ctypes.c_wchar_p(chr(0x00) * 256)
        FOLDERID_Documents = ctypes.c_char_p(uuid.UUID("ED4824AF-DCE4-45A8-81E2-FC7965083634").bytes_le)
        ctypes.windll.shell32.SHGetKnownFolderPath(FOLDERID_Documents, 0, None, ctypes.byref(path))
        materialLibPath = os.path.join(path.value, "META Documents", "MaterialLibrary", "material_library.json")

        library_manager = LibraryManager(materialLibPath)
        return library_manager

    except Exception as e:
        raise CADExcep.CADError("Error, Function: " + function_name + ", Message: " + \
                                "Failed to locate/open the material library. Locate/open Error:  " + \
                                e.message)