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

class AnalysisMaterialProperties:
    def __init__(self, in_material_name):
        self.material_name = in_material_name
        #
        self.density_defined = False
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
        self.density = 0.0
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
        self.density_unit = unit_utils.MECHANICAL_UNIT_NOT_DEFINED
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

    def __str__(self):
        return "material_name                                 " + self.material_name \
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
        with _winreg.OpenKey(_winreg.HKEY_LOCAL_MACHINE, r'Software\META', 0, _winreg.KEY_READ | _winreg.KEY_WOW64_32KEY) as key:
            META_PATH = _winreg.QueryValueEx(key, 'META_PATH')[0]

        materialLibInterfacePath = os.path.join(META_PATH, "bin", "Python27", "Lib", "site-packages", "material_library")

        sys.path.insert(0, materialLibInterfacePath)

        from MaterialLibraryInterface import LibraryManager

        path = ctypes.c_wchar_p(chr(0x00) * 256)
        FOLDERID_Documents = ctypes.c_char_p(uuid.UUID("ED4824AF-DCE4-45A8-81E2-FC7965083634").bytes_le)
        ctypes.windll.shell32.SHGetKnownFolderPath(FOLDERID_Documents, 0, None, ctypes.byref(path))
        materialLibPath = os.path.join(path.value, "META Documents", "MaterialLibrary", "material_library.json")

        library_manager = LibraryManager(materialLibPath)

    except Exception as e:
        raise CADExcep.CADError("Error, Function: " + function_name + ", Message: " + \
                                "Failed to locate/open the material library. Locate/open Error:  " + \
                                e.message)


    for material_name in in_material_names_list:
        out_material_lib_props_dict[material_name] = AnalysisMaterialProperties(material_name)
        try:
            material_data = library_manager.materialData(material_name)
        except Exception as e:
            raise CADExcep.CADError("Error, Function: " + function_name + ", Message: " +
                        "Failed to locate material " + material_name + " in material library. Error:  " +
                        e.message)


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