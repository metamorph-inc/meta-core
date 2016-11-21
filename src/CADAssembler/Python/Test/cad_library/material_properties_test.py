__author__ = 'rowens'
import sys
sys.path.append("..\..\..\..\Python27Packages\cad_library\cad_library")

#print sys.path

import cad_exception_classes as CADExcep
import material_properties

# Create list of material names
material_names_list = []
material_names_list.append("aluminum_alloy_6061_t6")
material_names_list.append("aluminum_alloy_6061_t4")
material_names_list.append("aluminum_qc7")


# material_names_list.append("aluminum_qc7zzz")
# Create output dictionary
material_lib_props_dict = {}  # AnalysisMaterialProperties dictionary, material-name key

# Retrieve Properties from the material library
try:
    material_properties.get_props_from_material_library(material_names_list, material_lib_props_dict)

    material_names_list.append("Epoxy_C_G_30_70_Mix")
    material_names_list.append("IM7_8551_7_Carbon_Epoxy")
    material_names_list.append("T300_PR319_Carbon_Epoxy")
    material_properties.get_props_from_material_library_typed(material_names_list, material_lib_props_dict)

    # Print the retrieved data
    for k in material_lib_props_dict:
        print
        print "key: ", k
        #material_lib_props_dict[k].print_self()
        print str(material_lib_props_dict[k])

except CADExcep.CADError as e:
    print e.message


