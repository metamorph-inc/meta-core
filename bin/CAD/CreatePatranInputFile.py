#Revision History
#-------------------------------------------------------
#1/4/2017			Added support for Force_Moment load.
#
import os
import sys
from lxml import etree as letree
import argparse
import logging
import datetime
from cad_library import material_properties
import operator


class CADExcep(Exception):
    """
    Base class for all CAD exceptions
    """

    def __init__(self, message):
        self.message = message


class e_CADModelType():
    part = 0
    assembly = 1

    def __init__(self, Type):
        self.value = Type

    def __str__(self):
        if self.value == e_CADModelType.part:
            return "Part"
        if self.value == e_CADModelType.assembly:
            return "Assembly"

    def __eq__(self, y):
        return self.value == y.value

    def __repr__(self):
        return self.__str__()

#class solidPartMaterialData
#    def __init__(self, in_ComponentInstanceID):
#        self.componentInstanceID = in_ComponentInstanceID
#        #
#        self.materialID = ""
#Solid_ComponentInstanceID_MaterialID

###################################################################################
def get_logger():

    # create logger with 'spam_application'
    logger = logging.getLogger('PatranPCL')
    logger.setLevel(logging.DEBUG)

    # create file handler which logs even debug messages
    if not os.path.isdir('log'):
        os.mkdir('log')

    fh = logging.FileHandler(os.path.join('log', 'CreatePatranInputFile.py.log'),'w')
    fh.setLevel(logging.DEBUG)

    # create console handler with a higher log level
    ch = logging.StreamHandler()
    ch.setLevel(logging.WARNING)

    # create formatter and add it to the handlers
    formatter = logging.Formatter('%(levelname)s - %(message)s')
    fh.setFormatter(formatter)
    ch.setFormatter(formatter)

    # add the handlers to the logger
    logger.addHandler(fh)
    logger.addHandler(ch)

    logger.info("=======================================")
    logger.info("New CreatePatranInputFile.py execution.")
    logger.info("=======================================")
    return logger
###################################################################################
g_logger = get_logger()
###################################################################################
class AnalysisMetaData:
    def __init__(self, in_ComponentInstanceID, in_Name, in_Type, in_MaterialID, in_FEAElementID ):
        self.componentInstanceID = in_ComponentInstanceID
        self.cADModelName = in_Name
        self.cADModelType = in_Type     # e_CADModelType  part/assemlby
        self.materialID = in_MaterialID # Material Name, should be a Material-library material name
        self.fEAElementID = in_FEAElementID

    def __str__(self):
        return "ComponentInstanceID  " + self.componentInstanceID \
        +      "\n   CADModelName         " + self.cADModelName \
        +      "\n   CADModelType         " + str(self.cADModelType) \
        +      "\n   MaterialID           " + self.materialID \
        +      "\n   FEAElementID         " + self.fEAElementID

    def __repr__(self):
        return self.__str__()

###################################################################################
def  retrieve_AnalysisMetaData( in_AnalysisMetaData_path, out_ComponentInsanceID_AnalysisMetaData ):

    function_name = "retrieve_AnalysisMetaData"

    cv_tree = letree.parse(in_AnalysisMetaData_path)

    cv_root = cv_tree.getroot()

    comp_material_string = "Assemblies/Assembly/Component/Component[@Type='PART']"
    g_logger.info("Searching for {} ".format(comp_material_string))
    comp_material_nodes = cv_root.findall(comp_material_string)

    for node in comp_material_nodes:
        comp_ID = node.attrib['ComponentInstanceID']
        name = node.attrib['Name']
        cADModelType_string = node.attrib['Type']
        if ( cADModelType_string.lower() == "part" ):
            cADModelType = e_CADModelType(e_CADModelType.part)
        else:
            cADModelType = e_CADModelType(e_CADModelType.assembly)
        material_ID = node.attrib['MaterialID']
        fEAElementID = node.attrib['FEAElementID']

        # If AnalysisMetaData.xml contains more than one entry (under Assemblies) for a ComponentInstanceID, then this
        # would be a mistake in the CreateAssembly program that created AnalysisMetaData.xml
        # Check here so that if the CreateAssembly is modified erroneously, it will be caught
        if ( comp_ID in out_ComponentInsanceID_AnalysisMetaData):
            raise CADExcep("Error, Function: " + function_name + ", Message: " + comp_ID +
                           " already in out_ComponentInsanceID_AnalysisMetaData. See AnalysisMetaData.xml, " +
                           "A particular ComponentInstanceID should appear only once in this file.")

        out_ComponentInsanceID_AnalysisMetaData[comp_ID] = AnalysisMetaData(comp_ID, name, cADModelType, material_ID, fEAElementID )

###################################################################################
def  log_AnalysisMetaData( in_ComponentInsanceID_AnalysisMetaData ):

    g_logger.info("***************************************************************************")
    g_logger.info("************ BEGIN AnalysisMetaData from  AnalysisMetaData.xml ************")
    for k, v in in_ComponentInsanceID_AnalysisMetaData.items():
        #print(k, v)
        g_logger.info("Key: " + str(k))
        g_logger.info(v)
    g_logger.info("************ END AnalysisMetaData from  AnalysisMetaData.xml **************")
    g_logger.info("***************************************************************************")
###################################################################################
def  check_AnalysisMetaData( in_ComponentInsanceID_AnalysisMetaData ):
    # Need to verify that componentInstanceID and materialID are not null.
    # These two items are needed by this program

    # Note - No need to check if Part or Assembly, retrieve_AnalysisMetaData only retrieves parts

    function_name = "check_AnalysisMetaData"

    for k, v in in_ComponentInsanceID_AnalysisMetaData.items():
        if ( v.componentInstanceID == "" ):
            raise CADExcep("Error, Function: " + function_name + ", Message: " +
                           " a componentInstanceID has a null value in in_ComponentInsanceID_AnalysisMetaData.  " +
                           "See the BEGIN AnalysisMetaData section in the log file for additional information.")
        if (v.materialID == ""):
            raise CADExcep("Error, Function: " + function_name + ", Message: " +
                            " a materialID has a null value in in_ComponentInsanceID_AnalysisMetaData.  " +
                            "See the BEGIN AnalysisMetaData section in the log file for additional information.")

###################################################################################
class Point_3D:
    def __init__(self, in_x, in_y, in_z ):
        self.x = in_x
        self.y = in_y
        self.z = in_z

    def __str__(self):
        return "x: " + str(self.x) + "  y: " + str(self.y) + "  z: " + str(self.z)

    def __repr__(self):
        return self.__str__()

###################################################################################
class ComputedValues:
    def __init__(self, in_MetricID, in_Units, in_Point_3D ):
        self.metricID = in_MetricID
        self.units = in_Units
        self.point_3D = in_Point_3D

    def __str__(self):
        return "MetricID  " + self.metricID + ", Units: " + self.units + \
               "  Point, x: " + str(self.point_3D.x) + ", y: " + str(self.point_3D.y) + ", z: " + str(self.point_3D.z)

    def __repr__(self):
        return self.__str__()

###################################################################################
def retrieve_ComputedValues(in_ComputedValues_path, out_MetricID_ComputedValues):

    function_name = "retrieve_ComputedValues"

    cv_tree = letree.parse(in_ComputedValues_path)
    cv_root = cv_tree.getroot()

    metric_string = "Component/Metrics/Metric[@Type='VECTOR']"
    metric_nodes = cv_root.findall(metric_string)

    for node in metric_nodes:
        metric_id = node.attrib['MetricID']
        units = node.attrib['Units']
        array_value = node.attrib['ArrayValue']
        x_y_z = array_value.split(';')

        # If ComputedValues.xml contains more than one entry for  a particular MetricID, then this
        # would be a mistake in the CreateAssembly program that created ComputedValues.xml
        # Check here so that if the CreateAssembly is modified erroneously, it will be caught
        if ( metric_id in out_MetricID_ComputedValues):
            raise CADExcep("Error, Function: " + function_name + ", Message: " + metric_id +
                           " already in out_MetricID_ComputedValues. See ComputedValues.xml, " +
                           "A particular MetricID should appear only once in this file.")

        out_MetricID_ComputedValues[metric_id] = ComputedValues(metric_id, units,
                                                 Point_3D( x_y_z[0],  x_y_z[1],  x_y_z[2]))


###################################################################################
def log_ComputedValues(in_MetricID_ComputedValues):

    g_logger.info("***************************************************************************")
    g_logger.info("************** BEGIN ComputedValues from  ComputedValues.xml **************")
    for k, v in in_MetricID_ComputedValues.items():
        # print(k, v)
        g_logger.info("Key: " + str(k))
        g_logger.info(v)
    g_logger.info("************** END ComputedValues from  ComputedValues.xml ****************")
    g_logger.info("***************************************************************************")

###################################################################################
# If  CADAssembly.xml has the tag <Elements> then it is a surface model.
# Mixing surfaces and solids is not supported
def is_a_surface_model(in_cADAssembly_root):

    target = "Assembly/CADComponent/CADComponent/Elements"
    elements = in_cADAssembly_root.findall(target)

    if len(elements) == 0:
        return False
    else:
        return True

###################################################################################
def get_surface_model_materials(in_cADAssembly_root, out_MaterialNamesUsedInModel_set):

    target = "Assembly/CADComponent/CADComponent/Elements/Element/ElementContents/MaterialLayup/Layer"
    layers = in_cADAssembly_root.findall(target)

    for layer in layers:
        out_MaterialNamesUsedInModel_set.add(layer.attrib['Material_Name'])

###################################################################################
def get_solid_model_materials(in_ComponentInsanceID_AnalysisMetaData, out_MaterialNamesUsedInModel_set):

    for k, v in in_ComponentInsanceID_AnalysisMetaData.items():
        out_MaterialNamesUsedInModel_set.add(v.materialID)


###################################################################################
def get_top_level_assembly_name(in_CADAssembly_root):

    function_name = "get_assembly_name"

    cad_assembly_string = ".//CADComponent[@Type='ASSEMBLY']"
    cad_assemblies = in_CADAssembly_root.findall(cad_assembly_string)

    # Note - The first CADComponent will be the top-level assembly.
    if len(cad_assemblies) == 0:
        raise CADExcep("Error, Function: " + function_name + ", Message: CAD top-level assembly name not found in CADAssembly.xml.")
    else:
        cad_assembly = cad_assemblies[0]
        return cad_assembly.attrib['Name']

###################################################################################
def write_Files_section(in_OutputModelFile_obj, in_TopLevelAssemblyName):

    in_OutputModelFile_obj.write("\n\nFiles")
    in_OutputModelFile_obj.write("\n    Patran_Model_Name =  Patran_model          # Hard-Coded")
    in_OutputModelFile_obj.write("\n    Patran_Model_Dir =   .\                    # Hard-Coded")
    in_OutputModelFile_obj.write("\n    Geometry_File_Name = " + in_TopLevelAssemblyName + "_asm.x_t")
    in_OutputModelFile_obj.write("\n    Geometry_File_Dir =  ..\..\Parasolid       # Hard-Coded")
    in_OutputModelFile_obj.write("\n    Geometry_File_Type = Parasolid             # Hard-Coded")

###################################################################################
def get_failure_criteria(in_CADAssembly_root):

    function_name = "get_failure_criteria"

    metrics_string = "Assembly/Analyses/FEA/Metrics/Metric"

    # First or default
    metric_list = in_CADAssembly_root.findall(metrics_string)
    comment = '{}'.format(metrics_string)

    failure_criteria_composite = []
    failure_criteria_all = []

    nonCompositeFailureCriteria_found = False

    for metric in metric_list:
        metric_type = metric.attrib.get('MetricType')

        failure_criteria_all.append(metric_type)

        if metric_type in ['TSAI_WU_FAILURE', 'HILL_FAILURE', 'HOFFMAN_FAILURE', 'MAX_FAILURE']:
            failure_criteria_composite.append(metric_type)
        else:
            nonCompositeFailureCriteria_found = True

    if len(failure_criteria_composite) != 0 and nonCompositeFailureCriteria_found:
        failure_criteria_all_string = str(failure_criteria_all)
        raise CADExcep("Error, Function: " + function_name +
                       ",  Conflicting attribute MetricType (Assembly/Analyses/FEA/Metrics/Metric_ in CADAssembly.xml., " +
                       "  Found Types: " + failure_criteria_all_string +
                       ".  For composite failure criteria all MetricTypes must be the same type.")

    if len(failure_criteria_composite) == 0 :
        return 'None'
    elif len(failure_criteria_composite) == 1:
        failure_criterion = failure_criteria_composite[0].replace('_FAILURE', '')
        return failure_criterion
    else:  # there can be several, IF they are identical
        fc_1 = failure_criteria_composite[0]
        for fc in failure_criteria_composite:
            if fc_1 != fc:
                raise CADExcep("Error, Function: " + function_name +
                               ",  Conflicting attribute MetricType (Assembly/Analyses/FEA/Metrics/Metric_ in CADAssembly.xml., " +
                               '{} != {}'.format(fc_1, fc) + ".  For composite failure criteria all MetricTypes must be the same type." )

        # No matter what, return the first one (script will log it and fail if there are multiple)
        failure_criterion = fc_1.replace('_FAILURE', '')

        if failure_criterion == 'MAX':
            failure_criterion = 'MAXIMUM_STRESS'

        return failure_criterion
###################################################################################
class AnalysisData_struct(object):
    configurationID = ""            # e.g. b717e39-2e87-471d-b5f0-add5e25ce04f|1
    sourceModel = "CyPhy Model"     # This is the CyPhy Model, in the future should be Bracket_Solid_Model.mga
    type = ""                       #  101 - Structural, 103 Modal
    solver = ""                     # Nastran  not Patran_Nastran, Patran is assumed since this program builds
                                    # an input file for Patran.  In the future other solvers (e.g. Abaqus) could
                                    # be supported
    instructions = ""               # MESH_AND_SOLVE or MESH_ONLY
    failureCriteria = ""            # ONE OF: Tsai_Wu, Hoffman, Hill, Maximum_Stress
    shellElementType = ""           # Plate4 or Plate8

    def __str__(self):
        return "AnalysisData  " \
               + "\n   configurationID   " + self.configurationID \
               + "\n   sourceModel       " + str(self.sourceModel) \
               + "\n   type              " + self.type \
               + "\n   solver            " + self.solver \
               + "\n   instructions      " + self.instructions \
               + "\n   failureCriteria   " + self.failureCriteria \
               + "\n   shellElementType  " + self.shellElementType

    def __repr__(self):
        return self.__str__()

###################################################################################
def get_AnalysisData(in_CADAssembly_root, out_AnalysisData):

    function_name = "get_AnalysisData"

    assembly_string = "Assembly"
    fea_analysis_string = "Assembly/Analyses/FEA"
    solver_string = fea_analysis_string + "/Solvers/Solver"

    # Get configurationID
    assm_node = in_CADAssembly_root.find(assembly_string)
    out_AnalysisData.configurationID = assm_node.attrib['ConfigurationID']

    # Get type, 101 - Structural, 103 Modal, Note - only 101 currently supported
    fea_node =in_CADAssembly_root.find(fea_analysis_string)
    fea_type = fea_node.attrib['Type']
    if str(fea_type).lower() != "structural":
        raise CADExcep("Error, Function: " + function_name + ", Message: Non supported FEA analysis type, " +
                        "only STRUCTURAL currently supported.  CADAssembly.xml has type of " + fea_type )
    out_AnalysisData.type = "101"

    # Solver
    solver_node = in_CADAssembly_root.find(solver_string)
    solver_type = solver_node.attrib['Type']
    if str(solver_type).lower() != 'patran_nastran':
        raise CADExcep("Error, Function: " + function_name + ", Message: Non Solver type, " +
                        "only PATRAN_NASTRAN currently supported.  CADAssembly.xml has type of " + solver_type )
    out_AnalysisData.solver = "NASTRAN"

    # ShellElementType
    shellElementType = solver_node.attrib['ShellElementType']
    out_AnalysisData.shellElementType = shellElementType

    # Instructions
    mesh_only = fea_node.attrib['MeshOnly']
    out_AnalysisData.instructions = 'MESH_ONLY' if mesh_only == 'true' else 'MESH_AND_SOLVE'

    # failureCriteria
    out_AnalysisData.failureCriteria = get_failure_criteria(in_CADAssembly_root)

###################################################################################
def write_Analysis_section(in_OutputModelFile_obj, in_AnalysisData):
    in_OutputModelFile_obj.write("\n\nAnalysis")
    in_OutputModelFile_obj.write("\n    Configuration_ID = " + in_AnalysisData.configurationID)
    in_OutputModelFile_obj.write("\n    Date =             " + str(datetime.datetime.now()))
    in_OutputModelFile_obj.write("\n    Source_Model =     " + in_AnalysisData.sourceModel)
    in_OutputModelFile_obj.write("\n    Type =             " + in_AnalysisData.type)
    in_OutputModelFile_obj.write("\n    Solver =           " + in_AnalysisData.solver)
    in_OutputModelFile_obj.write("\n    Instructions =     " + in_AnalysisData.instructions)
    if ( in_AnalysisData.failureCriteria.lower() != 'none' ):
        in_OutputModelFile_obj.write("\n    Failure_Criteria = " + in_AnalysisData.failureCriteria)
###################################################################################
def write_SubCase_section(in_OutputModelFile_obj):
    in_OutputModelFile_obj.write("\n\nSubCase")
    in_OutputModelFile_obj.write("\n    ID =            1    # Hard-Coded")
    in_OutputModelFile_obj.write("\n    Constraint_ID = 259  # Hard-Coded")
    in_OutputModelFile_obj.write("\n    Load_ID =       559  # Hard-Coded")
###################################################################################
class MeshParameters_struct(object):
    Comment = "Default values"
    ID = "1"   # One and only one of these
    Max_Global_Length = ""
    Max_Curv_Delta_Div_Edge_Len = ""
    Ratio_Min_Edge_To_Max_Edge = ""
    Match_Face_Proximity_Tol = ""

    def __str__(self):
        return "MeshParameters  " \
               + "\n   Comment                        " + self.Comment \
               + "\n   ID                             " + self.ID \
               + "\n   Max_Global_Length              " + self.Max_Global_Length \
               + "\n   Max_Curv_Delta_Div_Edge_Len    " + self.Max_Curv_Delta_Div_Edge_Len \
               + "\n   Ratio_Min_Edge_To_Max_Edge     " + self.Ratio_Min_Edge_To_Max_Edge \
               + "\n   Match_Face_Proximity_Tol       " + self.Match_Face_Proximity_Tol

    def __repr__(self):
        return self.__str__()
###################################################################################
def get_MeshParameters(in_OutputModelFile_obj, out_MeshParameters):

    # Set default values
    out_MeshParameters.ID = "1"  # Only one of theses
    out_MeshParameters.Max_Global_Length = "100.0"
    out_MeshParameters.Max_Curv_Delta_Div_Edge_Len = "0.10"
    out_MeshParameters.Ratio_Min_Edge_To_Max_Edge = "0.20"
    out_MeshParameters.Match_Face_Proximity_Tol = "0.50"

    mesh_param_string = "Assembly/Analyses/FEA/MeshParameters"

    # First or default
    mesh_params_list = in_OutputModelFile_obj.findall(mesh_param_string)
    comment = '{}.'.format(mesh_param_string)

    if len(mesh_params_list) == 0:
        warning_msg = "No MeshParameters node found in CADAssembly.xml; using hard-coded values"
        comment += '  ' + warning_msg
        g_logger.warning(warning_msg)

    if len(mesh_params_list) > 0:

        if len(mesh_params_list) > 1:
            comment += ' Multiple MeshParameters nodes.'

            for mp in mesh_params_list:
                comment += "\n    #_id: {}".format(mp.attrib['_id'])


        mesh_params_node = mesh_params_list[0]
        out_MeshParameters.Comment =  mesh_param_string

        max_global_length = mesh_params_node.attrib['Max_Global_Length']
        max_curv_delta = mesh_params_node.attrib['Max_Curv_Delta_Div_Edge_Len']
        ratio_min_to_max = mesh_params_node.attrib['Ratio_Min_Edge_To_Max_Edge']
        match_face_prox = mesh_params_node.attrib['Match_Face_Proximity_Tol']

        out_MeshParameters.Max_Global_Length = mesh_params_node.attrib['Max_Global_Length']
        out_MeshParameters.Max_Curv_Delta_Div_Edge_Len = mesh_params_node.attrib['Max_Curv_Delta_Div_Edge_Len']
        out_MeshParameters.Ratio_Min_Edge_To_Max_Edge = mesh_params_node.attrib['Ratio_Min_Edge_To_Max_Edge']
        out_MeshParameters.Match_Face_Proximity_Tol = mesh_params_node.attrib['Match_Face_Proximity_Tol']

###################################################################################
def write_MeshParameters_section(in_OutputModelFile_obj, in_MeshParameters):

    in_OutputModelFile_obj.write("\n\nMesh_Parameters")
    in_OutputModelFile_obj.write("\n    # " + in_MeshParameters.Comment)
    in_OutputModelFile_obj.write("\n    ID =                          " + in_MeshParameters.ID)
    in_OutputModelFile_obj.write("\n    Max_Global_Length =           " + in_MeshParameters.Max_Global_Length)
    in_OutputModelFile_obj.write("\n    Max_Curv_Delta_Div_Edge_Len = " + in_MeshParameters.Max_Curv_Delta_Div_Edge_Len)
    in_OutputModelFile_obj.write("\n    Ratio_Min_Edge_To_Max_Edge =  " + in_MeshParameters.Ratio_Min_Edge_To_Max_Edge)
    in_OutputModelFile_obj.write("\n    Match_Face_Proximity_Tol =    " + in_MeshParameters.Match_Face_Proximity_Tol)

###################################################################################
def write_material_data(in_OutputModelFile_obj, in_Defined, in_Name, in_Value, in_Multiplier):

    spacer = "                                                                 "

    overall_len = 24  # This is the length of in_Name plus spaces
    in_Name_len = len(in_Name)
    if in_Name_len < overall_len:
        spacer = spacer[:overall_len-in_Name_len]
    else:
        spacer = ""

    if in_Defined:
        in_OutputModelFile_obj.write("\n    " + in_Name + " = " + spacer + "{}".format(in_Value * in_Multiplier))

###################################################################################
def write_Material_section(in_OutputModelFile_obj, in_MaterialName, in_MaterailID):

    function_name = "get_AnalysisData"

    PA_to_MPa_Multiplier =  1.0 / 1000000.0
    Density_Kg_per_M3_to_Kg_per_mm3_Divisor = 1000000000.0
    M_to_MM_Multiplier = 1000.0

    materials_list = [in_MaterialName]
    material_data = {}

    material_properties.get_props_from_material_library_typed(materials_list, material_data)

    data = material_data.get(in_MaterialName, None)

    if data is None:
        raise CADExcep("Error, Function: " + function_name +
                       ", Message: Material '{}' not found in material library., Material name:" +
                       in_MaterialName )

    in_OutputModelFile_obj.write("\n\nMaterial")
    in_OutputModelFile_obj.write("\n    # Units: Density - kg/mm^3, Elastic_Modulus - MPa, Stress_Limit - MPa")
    in_OutputModelFile_obj.write("\n    ID =                       " + in_MaterailID)
    in_OutputModelFile_obj.write("\n    Name =                     " + in_MaterialName)
    in_OutputModelFile_obj.write("\n    Description =              " + in_MaterialName)
    in_OutputModelFile_obj.write("\n    Density =                  " +
                                        '{}'.format(data.density / Density_Kg_per_M3_to_Kg_per_mm3_Divisor))


    if isinstance(data, material_properties.AnalysisMaterialProperties):

        in_OutputModelFile_obj.write("\n    Tropic_Type =                " + "ISOTROPIC")

        write_material_data(in_OutputModelFile_obj,
                            data.mechanical__modulus_elastic_defined,
                                            "Elastic_Modulus",
                            data.mechanical__modulus_elastic,
                            PA_to_MPa_Multiplier)

        write_material_data(in_OutputModelFile_obj,
                            data.mechanical__ratio_poissons_defined,
                                            "Poissons_Ratio",
                            data.mechanical__ratio_poissons,
                            1.0)

        write_material_data(in_OutputModelFile_obj,
                            data.thermal__coefficient_expansion_linear_defined,
                                          "Therm_Expan_Coef",
                            data.thermal__coefficient_expansion_linear,
                            M_to_MM_Multiplier)

    elif isinstance(data, material_properties.AnalysisMaterialProperties_Orthotropic):

        in_OutputModelFile_obj.write("\n    Tropic_Type =              " + "ORTHOTROPIC")

        in_OutputModelFile_obj.write("\n    Tropic_SubType =           " + "2D  # Hard-Coded")
        in_OutputModelFile_obj.write("\n    Failure_Index =            "  + "1   # Hard-Coded")

        write_material_data(in_OutputModelFile_obj,
                            data.mechanical__modulus_elastic_11_defined,
                                            "Elastic_Modulus_11",
                            data.mechanical__modulus_elastic_11,
                            PA_to_MPa_Multiplier)

        write_material_data(in_OutputModelFile_obj,
                            data.mechanical__modulus_elastic_22_defined,
                                            "Elastic_Modulus_22",
                            data.mechanical__modulus_elastic_22,
                            PA_to_MPa_Multiplier)

        write_material_data(in_OutputModelFile_obj,
                            data.mechanical__modulus_elastic_33_defined,
                                            "Elastic_Modulus_33",
                            data.mechanical__modulus_elastic_33,
                            PA_to_MPa_Multiplier)

        write_material_data(in_OutputModelFile_obj,
                            data.mechanical__modulus_shear_12_defined,
                                            "Shear_Modulus_12",
                            data.mechanical__modulus_shear_12,
                            PA_to_MPa_Multiplier)

        write_material_data(in_OutputModelFile_obj,
                            data.mechanical__modulus_shear_23_defined,
                                           "Shear_Modulus_23",
                            data.mechanical__modulus_shear_23,
                            PA_to_MPa_Multiplier)

        write_material_data(in_OutputModelFile_obj,
                            data.mechanical__modulus_shear_31_defined,
                                            "Shear_Modulus_31",
                            data.mechanical__modulus_shear_31,
                            PA_to_MPa_Multiplier)

        write_material_data(in_OutputModelFile_obj,
                            data.mechanical__ratio_poissons_12_defined,
                                            "Poissons_Ratio_12",
                            data.mechanical__ratio_poissons_12,
                            1.0)

        write_material_data(in_OutputModelFile_obj,
                            data.mechanical__ratio_poissons_23_defined,
                                            "Poissons_Ratio_23",
                            data.mechanical__ratio_poissons_23,
                            1.0)

        write_material_data(in_OutputModelFile_obj,
                            data.mechanical__ratio_poissons_31_defined,
                                            "Poissons_Ratio_31",
                            data.mechanical__ratio_poissons_31,
                            1.0)

        write_material_data(in_OutputModelFile_obj,
                            data.physical__cured_ply_thickness_defined,
                                          "Cured_Ply_Thickness",
                            data.physical__cured_ply_thickness,
                            M_to_MM_Multiplier)

        write_material_data(in_OutputModelFile_obj,
                            data.mechanical_tension_stress_limit_11_defined,
                                           "Tension_Stress_Limit_11",
                            data.mechanical_tension_stress_limit_11,
                            PA_to_MPa_Multiplier)

        write_material_data(in_OutputModelFile_obj,
                            data.mechanical_tension_stress_limit_22_defined,
                                           "Tension_Stress_Limit_22",
                            data.mechanical_tension_stress_limit_22,
                            PA_to_MPa_Multiplier)

        write_material_data(in_OutputModelFile_obj,
                            data.mechanical_tension_stress_limit_33_defined,
                                           "Tension_Stress_Limit_33",
                            data.mechanical_tension_stress_limit_33,
                            PA_to_MPa_Multiplier)

        write_material_data(in_OutputModelFile_obj,
                            data.mechanical_compress_stress_limit_11_defined,
                                           "Compress_Stress_Limit_11",
                            data.mechanical_compress_stress_limit_11,
                            PA_to_MPa_Multiplier)

        write_material_data(in_OutputModelFile_obj,
                            data.mechanical_compress_stress_limit_22_defined,
                                           "Compress_Stress_Limit_22",
                            data.mechanical_compress_stress_limit_22,
                            PA_to_MPa_Multiplier)

        write_material_data(in_OutputModelFile_obj,
                            data.mechanical_compress_stress_limit_33_defined,
                                           "Compress_Stress_Limit_33",
                            data.mechanical_compress_stress_limit_33,
                            PA_to_MPa_Multiplier)

        write_material_data(in_OutputModelFile_obj,
                            data.mechanical_shear_stress_limit_12_defined,
                                           "Shear_Stress_Limit_12",
                            data.mechanical_shear_stress_limit_12,
                            PA_to_MPa_Multiplier)

        write_material_data(in_OutputModelFile_obj,
                            data.mechanical_shear_stress_limit_23_defined,
                                           "Shear_Stress_Limit_23",
                            data.mechanical_shear_stress_limit_23,
                            PA_to_MPa_Multiplier)

        write_material_data(in_OutputModelFile_obj,
                            data.mechanical_shear_stress_limit_31_defined,
                                           "Shear_Stress_Limit_31",
                            data.mechanical_shear_stress_limit_31,
                            PA_to_MPa_Multiplier)

        write_material_data(in_OutputModelFile_obj,
                            data.mechanical_shear_stress_bond_defined,
                                           "Shear_Stress_Bond",
                            data.mechanical_shear_stress_bond,
                            PA_to_MPa_Multiplier)

        write_material_data(in_OutputModelFile_obj,
                            data.mechanical_interactive_strength_12_defined,
                                           "Interactive_Strength_12",
                            data.mechanical_interactive_strength_12,
                            PA_to_MPa_Multiplier)

        write_material_data(in_OutputModelFile_obj,
                            data.mechanical_interactive_strength_23_defined,
                                           "Interactive_Strength_23",
                            data.mechanical_interactive_strength_23,
                            PA_to_MPa_Multiplier)

        write_material_data(in_OutputModelFile_obj,
                            data.mechanical_interactive_strength_31_defined,
                                           "Interactive_Strength_31",
                            data.mechanical_interactive_strength_31,
                            PA_to_MPa_Multiplier)

###################################################################################
def output_Point_construct( in_OutputModelFile_obj,
                            in_MetricID_ComputedValues,
                            in_ComponentID,
                            in_Feature_Name,
                            in_MetricID,
                            in_Point_ID):

    function_name = "output_Point_construct"

    in_OutputModelFile_obj.write("\n\nPoint")
    in_OutputModelFile_obj.write("\n    # ComponentID = " + in_ComponentID)
    in_OutputModelFile_obj.write("\n    # DatumName =   " + in_Feature_Name)
    in_OutputModelFile_obj.write("\n    # MetricID =    " + in_MetricID)

    in_OutputModelFile_obj.write("\n    ID =            " + str(in_Point_ID))

    if in_MetricID not in in_MetricID_ComputedValues:
        raise CADExcep("Error, Function: " + function_name + ", Message: Do not have data for " +
                       "MetricID " + in_MetricID + ". " +
                       "ComponentID " + in_ComponentID)

    point = in_MetricID_ComputedValues[in_MetricID]
    in_OutputModelFile_obj.write("\n    x_Cord =        " + point.point_3D.x)
    in_OutputModelFile_obj.write("\n    y_Cord =        " + point.point_3D.y)
    in_OutputModelFile_obj.write("\n    z_Cord =        " + point.point_3D.z)

###################################################################################
class IDCounters(object):
    Surface_Contents_ID = 0
    Surface_ID = 0
    Geometry_ID = 0
    Point_ID = 0
    Material_Layup_2D_ID = 0
    Layer_2D_ID = 0
    Polygon_ID = 0
    Extrusion_ID = 0
    Displacement_ID = 0
    Pin_ID = 0
    Load_ID = 0
    Load_Value_ID = 0
    Constraint_ID = 0
    Solid_ID = 0

###################################################################################
def output_Geometry_constructs( in_OutputModelFile_obj,
                                in_out_ID_Counters,
                                in_geometry_features,
                                in_MetricID_ComputedValues,
                                in_cad_component_id):

    function_name = "output_Geometry_constructs"

    geometry_features_type = in_geometry_features.attrib.get('GeometryType')
    geometry_features_type_lower = str(geometry_features_type).lower()

    primary_geometry_qualifier = in_geometry_features.attrib.get('PrimaryGeometryQualifier')
    primary_geometry_qualifier_lower = str(primary_geometry_qualifier).lower()

    cAD_geometry_qualifier = ""
    if primary_geometry_qualifier_lower == "interiorandboundary":
        cAD_geometry_qualifier = "Interior_and_Boundary"
    elif primary_geometry_qualifier_lower == "boundaryonly":
        cAD_geometry_qualifier = "Boundary_Only"
    elif primary_geometry_qualifier_lower == "interioronly":
        cAD_geometry_qualifier = "Interior_Only"

    if geometry_features_type_lower != "face" \
            and geometry_features_type_lower != "polygon" \
            and geometry_features_type_lower != "extrusion":
        raise CADExcep("Error, Function: " + function_name + ", Message: For a surface model, " +
                       ".Elements/Element/Geometry/Features must have a " +
                       "GeometryType of FACE/POLYGON/EXTRUSION.  GeometryType: " + str(geometry_features_type))

    #print(geometry_features_type)

    if len(in_geometry_features) == 0:
        if in_cad_component_id != "":
            raise CADExcep("Error, Function: " + function_name + ", Message: For a surface model, " +
                           "all CADComponent(s) in CADAssembly.xml must have a Elements/Element/Geometry/Features " +
                           "node(s).  ComponentID " + in_cad_component_id + " does not meet this requirement")
        else:
            raise CADExcep("Error, Function: " + function_name + ", Message: Geometry/Features  not found, " +
                           "probably missing from an analysis load/constraint.")


    created_PointIDs = []
    for geometry_feature in in_geometry_features:
        geometry_feature_MetricID = geometry_feature.attrib.get('MetricID')
        geometry_feature_Name = geometry_feature.attrib.get('Name')
        geometry_feature_ComponentID = geometry_feature.attrib.get('ComponentID')

        in_out_ID_Counters.Point_ID += 1
        created_PointIDs.append(in_out_ID_Counters.Point_ID)
        output_Point_construct(in_OutputModelFile_obj,
                               in_MetricID_ComputedValues,
                               geometry_feature_ComponentID,
                               geometry_feature_Name,
                               geometry_feature_MetricID,
                               in_out_ID_Counters.Point_ID)

        in_out_ID_Counters.Geometry_ID += 1

    if geometry_features_type_lower == "face":
        in_OutputModelFile_obj.write("\n\nGeometry")
        in_OutputModelFile_obj.write("\n    # ComponentID =     " + geometry_feature_ComponentID)
        in_OutputModelFile_obj.write("\n    ID =                " + str(in_out_ID_Counters.Geometry_ID))
        in_OutputModelFile_obj.write("\n    TYPE =              " + "FACE")
        in_OutputModelFile_obj.write("\n    Point_ID =          " + str(in_out_ID_Counters.Point_ID))
    elif geometry_features_type_lower == "polygon":
        in_out_ID_Counters.Polygon_ID += 1
        in_OutputModelFile_obj.write("\n\nPolygon")
        in_OutputModelFile_obj.write("\n    # ComponentID =      " + geometry_feature_ComponentID)
        in_OutputModelFile_obj.write("\n    ID =                 " + str(in_out_ID_Counters.Polygon_ID))
        for ptID in created_PointIDs:
            in_OutputModelFile_obj.write("\n    Point_ID =           " + str(ptID))
        in_OutputModelFile_obj.write("\n    Boundary_Qualifier = " + cAD_geometry_qualifier)
        in_OutputModelFile_obj.write("\n\nGeometry")

        in_OutputModelFile_obj.write("\n    # ComponentID =     " + geometry_feature_ComponentID)
        in_OutputModelFile_obj.write("\n    ID =                " + str(in_out_ID_Counters.Geometry_ID))
        in_OutputModelFile_obj.write("\n    TYPE =              " + "POLYGON")
        in_OutputModelFile_obj.write("\n    Polygon_ID =        " + str(in_out_ID_Counters.Polygon_ID))
    elif geometry_features_type_lower == "extrusion":
        in_out_ID_Counters.Extrusion_ID += 1
        in_OutputModelFile_obj.write("\n\nExtrusion")
        in_OutputModelFile_obj.write("\n    # ComponentID =      " + geometry_feature_ComponentID)
        in_OutputModelFile_obj.write("\n    ID =                 " + str(in_out_ID_Counters.Extrusion_ID))
        for ptID in created_PointIDs[:-1]:
            in_OutputModelFile_obj.write("\n    Polygon_Point_ID =   " + str(ptID))
        in_OutputModelFile_obj.write("\n    Offset_Point_ID =    " +
                                     str(created_PointIDs[len(created_PointIDs) - 1]))

        in_OutputModelFile_obj.write("\n    Boundary_Qualifier = " + cAD_geometry_qualifier)

        in_OutputModelFile_obj.write("\n\nGeometry")
        in_OutputModelFile_obj.write("\n    # ComponentID = " + geometry_feature_ComponentID)
        in_OutputModelFile_obj.write("\n    ID =            " + str(in_out_ID_Counters.Geometry_ID))
        in_OutputModelFile_obj.write("\n    TYPE =          " + "EXTRUSION")
        in_OutputModelFile_obj.write("\n    Extrusion_ID =  " + str(in_out_ID_Counters.Extrusion_ID))


###################################################################################
def output_Surface_constructs(in_OutputModelFile_obj,
                              in_out_ID_Counters,
                              in_CADAssembly_root,
                              in_MetricID_ComputedValues,
                              in_materialNames_index,
                              in_FEA_ElementType, # Plate4 or Plate8, will convert to QUAD4 or QUAD8
                              in_Mesh_Parameters_ID):

    # in_MetricID_ComputedValues is a dictionary with the key of metric_ID and the value a point with x, y, x coordinates

    # in_materialNames_index is a dictionary with the key of material-name (same name as in material_library.json) and
    #   a value of the index that would have already been written(by the time this function is called) to
    #   in_OutputModel_file

    """
    Example CADAssembly.xml contents
        <Elements _id="id258">
          <Element ElementType="SURFACE" _id="id226">
            <ElementContents _id="id237">
              <MaterialLayup Position="CENTERED" Offset="0" Direction="NORMAL" _id="id232">
                <Layer ID="1" Material_Name="IM7_8551_7_Carbon_Epoxy" Thickness="0.7" Orientation="0" Drop_Order="3" _id="id233" />
                <Layer ID="2" Material_Name="T300_PR319_Carbon_Epoxy" Thickness="1.1" Orientation="30" Drop_Order="4" _id="id234" />
                <Layer ID="3" Material_Name="IM7_8551_7_Carbon_Epoxy" Thickness="0.7" Orientation="60" Drop_Order="2" _id="id235" />
                <Layer ID="4" Material_Name="T300_PR319_Carbon_Epoxy" Thickness="1.1" Orientation="90" Drop_Order="1" _id="id236" />
              </MaterialLayup>
              <Orientation _id="id227">
                <Geometry _id="id228">
                  <Features FeatureID="id-0065-000008ee" GeometryType="Vector" FeatureInterfaceType="CAD_DATUM" FeatureGeometryType="POINT" PrimaryGeometryQualifier="" SecondaryGeometryQualifier="" _id="id231">
                    <Feature Name="Plate_Pt1" ComponentID="d44a4d66-1560-4634-9d20-c83c063be51d" MetricID="d44a4d66-1560-4634-9d20-c83c063be51d:Plate_Pt1" _id="id229" />
                    <Feature Name="Plate_Pt2" ComponentID="d44a4d66-1560-4634-9d20-c83c063be51d" MetricID="d44a4d66-1560-4634-9d20-c83c063be51d:Plate_Pt2" _id="id230" />
                  </Features>
                </Geometry>
              </Orientation>
            </ElementContents>
            <Geometry _id="id238">
              <Features FeatureID="id-0065-000008ef" GeometryType="FACE" FeatureInterfaceType="CAD_DATUM" FeatureGeometryType="POINT" PrimaryGeometryQualifier="InteriorAndBoundary" SecondaryGeometryQualifier="" _id="id239">
                <Feature Name="Plate_Pt1" ComponentID="d44a4d66-1560-4634-9d20-c83c063be51d" MetricID="d44a4d66-1560-4634-9d20-c83c063be51d:Plate_Pt1" _id="id240" />
              </Features>
            </Geometry>
            <SurfaceNormal Direction="TOWARD" _id="id241">
              <Geometry _id="id244">
                <Features FeatureID="id-0066-00000393" GeometryType="POINT" FeatureInterfaceType="CAD_DATUM" FeatureGeometryType="POINT" PrimaryGeometryQualifier="InteriorAndBoundary" SecondaryGeometryQualifier="" _id="id242">
                  <Feature Name="Plate_Flat_A_Surf_Pt" ComponentID="d44a4d66-1560-4634-9d20-c83c063be51d" MetricID="d44a4d66-1560-4634-9d20-c83c063be51d:Plate_Flat_A_Surf_Pt" _id="id243" />
                </Features>
              </Geometry>
            </SurfaceNormal>
          </Element>
        </Elements>

    Example output:  output written to in_OutputModel_file by this function
        Surface_Contents
            ID = 1  # CreatePatranInputFile.py line: 1075
            Surface_ID = 1  # CreatePatranInputFile.py line: 1076
            Material_Layup_2D_ID = 1  # CreatePatranInputFile.py line: 1077
            Material_Orientation_Vector_Start_Point_ID = 3  # CreatePatranInputFile.py line: 1078
            Material_Orientation_Vector_End_Point_ID = 4  # CreatePatranInputFile.py line: 1079
            Material_Layup_Position = CENTERED
            Material_Layup_Offset_Value = 0
            Material_Layup_Direction = NORMAL

        Surface
            ID = 1
            Component_Instance_ID  = d44a4d66-1560-4634-9d20-c83c063be51d
            Geometry_ID = 3
            Element_Type = QUAD4  # CreatePatranInputFile.py line: 719
            Mesh_Parameters_ID = 1  # CreatePatranInputFile.py line: 630
            Normal_Point_ID = 2
            Normal_Direction = TOWARD

        Geometry
            ID = 1
            Type = FACE  # Hard-Coded in PatranInputTemplate.json
            Point_ID = 1

         Point
            # Comments: (Units=millimeter;MetricID=c90e413a-bbb9-46da-9251-ab2c1e129041:PLATE_FLAT_B_SURF_PT)
            ID = 1
            x_Cord = 20
            y_Cord = 2e+002
            z_Cord = 40

        Material_Layup_2D
            ID = 1
            Layer_2D_ID = 11
            Layer_2D_ID = 12
            Layer_2D_ID = 13
            Layer_2D_ID = 14

        Layer_2D
            ID = 11
            Material_ID = 1  # IM7_8551_7_Carbon_Epoxy
            Thickness = 0.7
            Orientation_Angle = 0
            Drop_Order = 3

        Layer_2D
            ID = 12
            Material_ID = 2  # T300_PR319_Carbon_Epoxy
            Thickness = 1.1
            Orientation_Angle = 30
            Drop_Order = 4

        Layer_2D
            # Comments:
            ID = 13
            Material_ID = 1  # IM7_8551_7_Carbon_Epoxy
            Thickness = 0.7
            Orientation_Angle = 60
            Drop_Order = 2

        Layer_2D
            # Comments:
            ID = 14
            Material_ID = 2  # T300_PR319_Carbon_Epoxy
            Thickness = 1.1
            Orientation_Angle = 90
            Drop_Order = 1

        Material
            ID = 1
            ...

        Material
            ID = 2
            ...

    """

    function_name = "output_Surface_constructs"

    if ( in_FEA_ElementType.lower() != 'plate4' and  in_FEA_ElementType.lower() != 'plate8' ):
        raise CADExcep("Error, Function: " + function_name + ", Message: in_FEA_ElementType " +
                        "can only be Plate4 or Plate8.  in_FEA_ElementType: " + in_FEA_ElementType)

    fEA_ElementType_Nastran_notation = ""

    if in_FEA_ElementType.lower() == 'plate4':
        fEA_ElementType_Nastran_notation = 'QUAD4'
    else:
        fEA_ElementType_Nastran_notation = 'QUAD8'

    cad_component_id = ""

    cad_component_parts_string = ".//CADComponent[@Type='PART']"
    cad_components = in_CADAssembly_root.findall(cad_component_parts_string)


    for cc in cad_components:
        cad_component_id = cc.attrib['ComponentID']

        #print(cad_component_id)

        element_string = ".Elements/Element[@ElementType='SURFACE']"
        element_nodes = cc.findall(element_string)

        if len(element_nodes) == 0:
            raise CADExcep("Error, Function: " + function_name + ", Message: For a surface model, " +
                           "all CADComponent(s) in CADAssembly.xml must have a Elements/Element/ElementContents " +
                           "node(s).  ComponentID " + cad_component_id + " does not have a ElementContents node" )

        for element_node in element_nodes:
            element_contents = element_node.find('.ElementContents')
            if len(element_contents) == 0:
                raise CADExcep("Error, Function: " + function_name + ", Message: For a surface model, " +
                               "all CADComponent(s) in CADAssembly.xml must have a Elements/Element/ElementContents " +
                               "node(s).  ComponentID " + cad_component_id + " does not have a ElementContents node")

            #########
            # Layers
            #########

            layup_node = element_contents.find('.MaterialLayup')

            Material_Layup_Position = layup_node.attrib.get('Position')
            Material_Layup_Offset_Value = layup_node.attrib.get('Offset')
            Material_Layup_Direction = layup_node.attrib.get('Direction')

            if len(layup_node) == 0:
                raise CADExcep("Error, Function: " + function_name + ", Message: There must be at least one, " +
                               "layer node in CADAssembly.xml under " +
                               "Elements/Element/ElementContents/MaterialLayup/. " +
                               "ComponentID " + cad_component_id + " does not meet this requirement.")

            used_LayerIDs = []
            for layer_node in layup_node:
                material_Name = layer_node.attrib.get('Material_Name')
                material_Thickness = layer_node.attrib.get('Thickness')
                material_Orientation = layer_node.attrib.get('Orientation')
                material_Drop_Order = layer_node.attrib.get('Drop_Order')

                if material_Name not in in_materialNames_index:
                    raise CADExcep("Error, Function: " + function_name + ", Message: Material " +
                                   material_Name + " not found in dictionary of materials (in_materialNames_index), " +
                                   "ComponentID " + cad_component_id )

                in_OutputModelFile_obj.write("\n\nLayer_2D")
                in_OutputModelFile_obj.write("\n    # ComponentID =     " + cad_component_id)
                in_out_ID_Counters.Layer_2D_ID += 1
                used_LayerIDs.append(in_out_ID_Counters.Layer_2D_ID)
                in_OutputModelFile_obj.write("\n    ID =                " + str(in_out_ID_Counters.Layer_2D_ID))
                in_OutputModelFile_obj.write("\n    Material_ID =       " + str(in_materialNames_index[material_Name]))
                in_OutputModelFile_obj.write("\n    Thickness =         " + material_Thickness)
                in_OutputModelFile_obj.write("\n    Orientation_Angle = " + material_Orientation)
                in_OutputModelFile_obj.write("\n    Drop_Order =        " + material_Drop_Order)

            in_OutputModelFile_obj.write("\n\nMaterial_Layup_2D")
            in_OutputModelFile_obj.write("\n    # ComponentID =     " + cad_component_id)
            in_out_ID_Counters.Material_Layup_2D_ID += 1
            in_OutputModelFile_obj.write("\n    ID =                " + str(in_out_ID_Counters.Material_Layup_2D_ID))
            for used_LayerID in used_LayerIDs:
                in_OutputModelFile_obj.write("\n    Layer_2D_ID =       " + str(used_LayerID))

            ##########
            # Surface
            ##########

            geometry_features_string = ".Geometry/Features[@FeatureGeometryType='POINT']"
            geometry_features = element_node.find(geometry_features_string)

            output_Geometry_constructs(in_OutputModelFile_obj,
                                       in_out_ID_Counters,
                                       geometry_features,
                                       in_MetricID_ComputedValues,
                                       cad_component_id)


            # Surface Normal Point
            surfaceNormal_string = ".SurfaceNormal"
            surfaceNormal = element_node.find(surfaceNormal_string)
            surfaceNormal_Direction = surfaceNormal.attrib.get('Direction')

            surfaceNormal_feature_string = ".SurfaceNormal/Geometry/Features/Feature"
            surfaceNormal_feature = element_node.find(surfaceNormal_feature_string)
            surfaceNormal_feature_MetricID = surfaceNormal_feature.attrib.get('MetricID')
            surfaceNormal_feature_Name = surfaceNormal_feature.attrib.get('Name')
            surfaceNormaly_feature_ComponentID = surfaceNormal_feature.attrib.get('ComponentID')

            in_out_ID_Counters.Point_ID += 1
            output_Point_construct(in_OutputModelFile_obj,
                                   in_MetricID_ComputedValues,
                                   surfaceNormaly_feature_ComponentID,
                                   surfaceNormal_feature_Name,
                                   surfaceNormal_feature_MetricID,
                                   in_out_ID_Counters.Point_ID)

            in_OutputModelFile_obj.write("\n\nSurface")
            in_out_ID_Counters.Surface_ID += 1
            in_OutputModelFile_obj.write("\n    ID =                     " + str(in_out_ID_Counters.Surface_ID))
            in_OutputModelFile_obj.write("\n    Component_Instance_ID =  " + cad_component_id)
            in_OutputModelFile_obj.write("\n    Geometry_ID =            " + str(in_out_ID_Counters.Geometry_ID))
            in_OutputModelFile_obj.write("\n    Element_Type =           " + fEA_ElementType_Nastran_notation)
            in_OutputModelFile_obj.write("\n    Mesh_Parameters_ID =     " + in_Mesh_Parameters_ID)
            in_OutputModelFile_obj.write("\n    Normal_Point_ID =        " + str(in_out_ID_Counters.Point_ID))
            in_OutputModelFile_obj.write("\n    Normal_Direction =       " + surfaceNormal_Direction)

            # Surface_Contents


            # Surface_Contents - Material_Orientation_Vector_Start/End_Point_ID

            orientation_geometry_features_string = \
                ".ElementContents/Orientation/Geometry/Features[@FeatureGeometryType='POINT']"
            orientation_geometry_features = element_node.find(orientation_geometry_features_string)
            if len(orientation_geometry_features) != 2:
                raise CADExcep("Error, Function: " + function_name + ", Message: For a surface model, " +
                               "all CADComponent(s) in CADAssembly.xml must have two " +
                               "Orientation/Geometry/Features " +
                               "nodes.  ComponentID " + cad_component_id + " does not meet this requirement")

            orientation_feature_MetricID = orientation_geometry_features[0].attrib.get('MetricID')
            orientation_feature_Name = orientation_geometry_features[0].attrib.get('Name')
            orientation_feature_ComponentID = orientation_geometry_features[0].attrib.get('ComponentID')

            in_out_ID_Counters.Point_ID += 1
            Material_Orientation_Vector_Start_Point_ID = in_out_ID_Counters.Point_ID
            output_Point_construct(in_OutputModelFile_obj,
                                   in_MetricID_ComputedValues,
                                   orientation_feature_ComponentID,
                                   orientation_feature_Name,
                                   orientation_feature_MetricID,
                                   in_out_ID_Counters.Point_ID)



            orientation_feature_MetricID = orientation_geometry_features[1].attrib.get('MetricID')
            orientation_feature_Name = orientation_geometry_features[1].attrib.get('Name')
            orientation_feature_ComponentID = orientation_geometry_features[1].attrib.get('ComponentID')

            in_out_ID_Counters.Point_ID += 1
            Material_Orientation_Vector_End_Point_ID = in_out_ID_Counters.Point_ID
            output_Point_construct(in_OutputModelFile_obj,
                                   in_MetricID_ComputedValues,
                                   orientation_feature_ComponentID,
                                   orientation_feature_Name,
                                   orientation_feature_MetricID,
                                   in_out_ID_Counters.Point_ID)

            in_out_ID_Counters.Surface_Contents_ID += 1
            in_OutputModelFile_obj.write("\n\nSurface_Contents")
            in_OutputModelFile_obj.write("\n    # ComponentID =               " + cad_component_id)
            in_OutputModelFile_obj.write("\n    ID =                          " + str(in_out_ID_Counters.Surface_Contents_ID))
            in_OutputModelFile_obj.write("\n    Surface_ID =                  " + str(in_out_ID_Counters.Surface_ID))
            in_OutputModelFile_obj.write("\n    Material_Layup_2D_ID =        " + str(in_out_ID_Counters.Material_Layup_2D_ID))
            in_OutputModelFile_obj.write("\n    Material_Orientation_Vector_Start_Point_ID = " +
                                         str(Material_Orientation_Vector_Start_Point_ID))
            in_OutputModelFile_obj.write("\n    Material_Orientation_Vector_End_Point_ID = " +
                                         str(Material_Orientation_Vector_End_Point_ID))

            in_OutputModelFile_obj.write("\n    Material_Layup_Position =     " + Material_Layup_Position)
            in_OutputModelFile_obj.write("\n    Material_Layup_Offset_Value = " + Material_Layup_Offset_Value)
            in_OutputModelFile_obj.write("\n    Material_Layup_Direction =    " + Material_Layup_Direction)


###################################################################################
def output_degree_of_freedom_constraint(in_OutputModelFile_obj, in_XMLElement, in_ConstraintType_string):
    # in_XMLElement
    #    <Rotation Units="deg" x="0" y="0" z="0" _id="id82" />
    #    <Translation Units="mm" x="0" y="0" z="0" _id="id83" />
    #
    # in_ConstraintType_string = "Disp" or "Rot"
    #
    # Examples
    #   x_Disp_State = FIXED
    #   y_Disp_State = FIXED
    #   z_Disp_State = FREE
    #   x_Rot_State = FIXED
    #   y_Rot_State = FIXED
    #   z_Rot_State = FREE
    #	x_Disp_Val = 1.0
    #	y_Disp_Val = 2.0
    #	z_Disp_Val = 3.0
    #	x_Rot_Val = 4.0
    #	y_Rot_Val = 5.0
    #	z_Rot_Val = 6.0

    x = in_XMLElement.attrib.get('x')
    y = in_XMLElement.attrib.get('y')
    z = in_XMLElement.attrib.get('z')

    if x is not None:
        string_lower = str(x).lower()
        if string_lower == "fixed" or string_lower == "free":
            name = "x_" + in_ConstraintType_string + "_State"
        else:
            name = "x_" + in_ConstraintType_string + "_Val"

        in_OutputModelFile_obj.write("\n    " + name + " = " + str(x))

    if y is not None:
        string_lower = str(y).lower()
        if string_lower == "fixed" or string_lower == "free":
            name = "y_" + in_ConstraintType_string + "_State"
        else:
            name = "y_" + in_ConstraintType_string + "_Val"

        in_OutputModelFile_obj.write("\n    " + name + " = " + str(y))

    if z is not None:
        string_lower = str(z).lower()
        if string_lower == "fixed" or string_lower == "free":
            name = "z_" + in_ConstraintType_string + "_State"
        else:
            name = "z_" + in_ConstraintType_string + "_Val"

        in_OutputModelFile_obj.write("\n    " + name + " = " + str(z))


###################################################################################
def output_Constraints_constructs(in_OutputModelFile_obj,
                                  in_out_ID_Counters,
                                  in_CADAssembly_root,
                                  in_MetricID_ComputedValues,
                                  in_SubcaseID):
    function_name = "output_Constraints_constructs"

    analysis_constraints_string = "Assembly/Analyses/FEA/AnalysisConstraints/AnalysisConstraint"
    analysis_constraints = in_CADAssembly_root.findall(analysis_constraints_string)

    for analysis_constraint_node in analysis_constraints:
        ##############################
        # Output geometry information
        ##############################
        geometry_features_string = ".Geometry/Features[@FeatureGeometryType='POINT']"
        geometry_features = analysis_constraint_node.find(geometry_features_string)

        output_Geometry_constructs(in_OutputModelFile_obj,
                                   in_out_ID_Counters,
                                   geometry_features,
                                   in_MetricID_ComputedValues,
                                   "")
        ###############
        # Displacement
        ###############
        displacement_string = ".Displacement"
        displacement = analysis_constraint_node.find(displacement_string)

        if displacement is not None:
            in_out_ID_Counters.Displacement_ID += 1
            in_OutputModelFile_obj.write("\n\nDisplacement")
            in_OutputModelFile_obj.write("\n    ID =        " + str(in_out_ID_Counters.Displacement_ID))

            rotation_string = ".Rotation"
            rotation = displacement.find(rotation_string)
            output_degree_of_freedom_constraint(in_OutputModelFile_obj, rotation, "Rot")

            rotation_string = ".Translation"
            rotation = displacement.find(rotation_string)
            output_degree_of_freedom_constraint(in_OutputModelFile_obj, rotation, "Disp")
        else:
            raise CADExcep("Error, Function: " + function_name +
                           ", Message: only Displacement constraints are currently supported.  " +
                           "CADAssembly.xml Assembly/Analyses/FEA/AnalysisConstraints")

        in_out_ID_Counters.Constraint_ID += 1
        in_OutputModelFile_obj.write("\n\nConstraint")
        in_OutputModelFile_obj.write("\n    ID =              " + str(in_out_ID_Counters.Constraint_ID))
        in_OutputModelFile_obj.write("\n    Type =            " + "DISPLACEMENT")
        in_OutputModelFile_obj.write("\n    SubCase_ID =      " + in_SubcaseID)
        in_OutputModelFile_obj.write("\n    Geometry_ID =     " + str(in_out_ID_Counters.Geometry_ID))
        in_OutputModelFile_obj.write("\n    Displacement_ID = " + str(in_out_ID_Counters.Displacement_ID))

###################################################################################
def output_Loads_constructs(in_OutputModelFile_obj,
                                      in_out_ID_Counters,
                                      in_CADAssembly_root,
                                      in_MetricID_ComputedValues,
                                      in_SubcaseID):
    function_name = "output_Constraints_constructs"

    analysis_load_string = "Assembly/Analyses/FEA/Loads/Load"
    analysis_loads = in_CADAssembly_root.findall(analysis_load_string)

    for analysis_load_node in analysis_loads:
        ##############################
        # Output geometry information
        ##############################
        geometry_features_string = ".Geometry/Features[@FeatureGeometryType='POINT']"
        geometry_features = analysis_load_node.find(geometry_features_string)

        output_Geometry_constructs(in_OutputModelFile_obj,
                                   in_out_ID_Counters,
                                   geometry_features,
                                   in_MetricID_ComputedValues,
                                   "")



        ###########
        # Pressure
        ###########
        pressure_string = ".Pressure"
        pressure = analysis_load_node.find(pressure_string)

        forceMoment_string = ".ForceMoment"
        forceMoment = analysis_load_node.find(forceMoment_string)

        if pressure is None and forceMoment is None:
            raise CADExcep("Error, Function: " + function_name +
                           ", Message: only Pressure and Force loads are currently supported.  " +
                           "CADAssembly.xml Assembly/Analyses/FEA/Loads")

        loadtype = ""

        if pressure is not None:
            in_out_ID_Counters.Load_Value_ID += 1
            in_OutputModelFile_obj.write("\n\nLoad_Value")
            in_OutputModelFile_obj.write("\n    ID =           " + str(in_out_ID_Counters.Load_Value_ID))
            pressure_value = pressure.attrib.get('Value')
            in_OutputModelFile_obj.write("\n    Scalar_Value = " + str(pressure_value))
            loadtype = "PRESSURE"

        else:
            # Must be force
            in_out_ID_Counters.Load_Value_ID += 1
            in_OutputModelFile_obj.write("\n\nLoad_Value")
            in_OutputModelFile_obj.write("\n    ID =           " + str(in_out_ID_Counters.Load_Value_ID))

            force_string = ".Force"
            force = forceMoment.find(force_string)

            moment_string = ".Moment"
            moment = forceMoment.find(moment_string)

            if force is None and moment is None:
                raise CADExcep("Error, Function: " + function_name +
                               ", Message: There must be a Force or Moment tag under ForceMoment.  " +
                               "CADAssembly.xml Assembly/Analyses/FEA/Loads")

            if force is None:
                x_Force_Value = 0.0
                y_Force_Value = 0.0
                z_Force_Value = 0.0
            else:
                x_Force_Value = force.attrib.get('x')
                y_Force_Value = force.attrib.get('y')
                z_Force_Value = force.attrib.get('z')

            if moment is None:
                x_Moment_Value = 0.0
                y_Moment_Value = 0.0
                z_Moment_Value = 0.0
            else:
                x_Moment_Value = moment.attrib.get('x')
                y_Moment_Value = moment.attrib.get('y')
                z_Moment_Value = moment.attrib.get('z')

            in_OutputModelFile_obj.write("\n    x_Value = " + str(x_Force_Value))
            in_OutputModelFile_obj.write("\n    y_Value = " + str(y_Force_Value))
            in_OutputModelFile_obj.write("\n    z_Value = " + str(z_Force_Value))
            
            in_OutputModelFile_obj.write("\n    x11_Value = " + str(x_Moment_Value))
            in_OutputModelFile_obj.write("\n    y22_Value = " + str(y_Moment_Value))
            in_OutputModelFile_obj.write("\n    z33_Value = " + str(z_Moment_Value))
            loadtype = "FORCE_MOMENT"


        in_out_ID_Counters.Load_ID += 1
        in_OutputModelFile_obj.write("\n\nLoad")
        in_OutputModelFile_obj.write("\n    ID =              " + str(in_out_ID_Counters.Load_ID))
        in_OutputModelFile_obj.write("\n    Type =            " + loadtype)
        in_OutputModelFile_obj.write("\n    SubCase_ID =      " + in_SubcaseID)
        in_OutputModelFile_obj.write("\n    Geometry_ID =     " + str(in_out_ID_Counters.Geometry_ID))
        in_OutputModelFile_obj.write("\n    Load_Value_ID =   " + str(in_out_ID_Counters.Load_Value_ID))

###################################################################################
def output_Solid_constructs(in_OutputModelFile_obj,
                            in_out_ID_Counters,
                            in_ComponentInsanceID_AnalysisMetaData,
                            in_materialNames_index,
                            in_Mesh_Parameters_ID):  # Mesh_Paramters_ID is always 1 for now

    function_name = "output_Solid_constructs"

    # We need to sort based on FEAElementID (e.g. "PSOLID_2", "PSOLID_1").  This is because
    # PSOLID_1/2/3 is the order that the parts are added into the Creo model and is also
    # the order that they will appear in the Parsolid file. They must also appear in
    # the output file of this program in that order.  There is no guarantee that the order
    # in AnalysisMetaData.xml is correct.

    componentInsanceID_AnalysisMetaData_sorted = sorted(in_ComponentInsanceID_AnalysisMetaData.values(),
                                                        key=operator.attrgetter('fEAElementID'))
    """ Old Approach, not sorted
    for k, v in in_ComponentInsanceID_AnalysisMetaData.items():
        #print v.cADModelType
        if v.cADModelType.value == e_CADModelType.part:

            material_Name = v.materialID
            if material_Name not in in_materialNames_index:
                raise CADExcep("Error, Function: " + function_name + ", Message: Material " +
                               material_Name + " not found in dictionary of materials (in_materialNames_index), " +
                               "ComponentID " + k)

            in_out_ID_Counters.Solid_ID += 1
            in_OutputModelFile_obj.write("\n\nSolid")
            in_OutputModelFile_obj.write("\n    ID =                    " + str(in_out_ID_Counters.Solid_ID))
            in_OutputModelFile_obj.write("\n    Component_Instance_ID = " + k)
            in_OutputModelFile_obj.write("\n    Material_ID =           " + str(in_materialNames_index[v.materialID]))
            in_OutputModelFile_obj.write("\n    Element_Type  =         " + "TETRA10  # Hard-Coded")
            in_OutputModelFile_obj.write("\n    Mesh_Parameters_ID =    " + in_Mesh_Parameters_ID)
    """

    for itr in componentInsanceID_AnalysisMetaData_sorted:
        # print v.cADModelType
        # Note - The following check for a part is not  necessary because only parts in in the list.
        if  itr.cADModelType.value == e_CADModelType.part:

            material_Name = itr.materialID
            if material_Name not in in_materialNames_index:
                raise CADExcep("Error, Function: " + function_name + ", Message: Material " +
                               material_Name + " not found in dictionary of materials (in_materialNames_index), " +
                               "ComponentID " + itr.componentInstanceID)

            in_out_ID_Counters.Solid_ID += 1
            in_OutputModelFile_obj.write("\n\nSolid")
            in_OutputModelFile_obj.write("\n    ID =                    " + str(in_out_ID_Counters.Solid_ID))
            in_OutputModelFile_obj.write("\n    Component_Instance_ID = " + itr.componentInstanceID)
            in_OutputModelFile_obj.write("\n    Material_ID =           " + str(in_materialNames_index[itr.materialID]))
            in_OutputModelFile_obj.write("\n    Element_Type  =         " + "TETRA10  # Hard-Coded")
            in_OutputModelFile_obj.write("\n    Mesh_Parameters_ID =    " + in_Mesh_Parameters_ID)

###################################################################################
def main():
    function_name = "main"

    g_logger.info('Timestamp: {:%Y-%m-%d %H:%M:%S}'.format(datetime.datetime.now()))
    g_logger.info("Begin of Main")

    ################################################
    # Invocation of this script from CADJobDriver
    ###############################################
    #cpif_name = 'CreatePatranInputFile.py'
    #cpif_path_py = os.path.join(self.meta_bin_cad, cpif_name)
    #python_command = " \"{}\" {} {} {} {}".format(
    #    cpif_path_py,
    #    '-cadassembly ../../CADAssembly.xml',
    #    '-cadassembly_metrics ../AnalysisMetaData.xml',
    #    '-computedvalues ../../ComputedValues.xml',
    #    '-copyxmltext False'
    #)

    errorOccurred = False
    try:
        parser = argparse.ArgumentParser(description="reads in CADAssembly.xml, AnalysisMetaData.xml, "
                                                     "and ComputedValues.xml and creates a .pcl script for Patran")
        parser.add_argument('-cadassembly',
                            default="CADAssembly.xml",
                            help="CADAssembly.xml filename")
        parser.add_argument('-analysismetaData',
                            default="AnalysisMetaData.xml",
                            help="AnalysisMetaData.xml filename")
        parser.add_argument('-computedvalues',
                            default="ComputedValues.xml",
                            help="ComputedValues.xml filename")

        args = parser.parse_args()

        cADAssembly_path = args.cadassembly
        analysisMetaData_path = args.analysismetaData
        computedValues_path = args.computedvalues

        g_logger.info("Input Arguments:")
        g_logger.info("  " + cADAssembly_path)
        g_logger.info("  " + analysisMetaData_path)
        g_logger.info("  " + computedValues_path)

        ##########################
        # Verify xml Files Exist
        ##########################
        if not os.path.exists(cADAssembly_path):
            raise CADExcep("Error, Function: " + function_name + ", Message: " + cADAssembly_path + " not found.")

        if not os.path.exists(analysisMetaData_path):
            raise CADExcep("Error, Function: " + function_name + ", Message: " + analysisMetaData_path + " not found.")

        if not os.path.exists(computedValues_path):
            raise CADExcep("Error, Function: " + function_name + ", Message: " + computedValues_path + " not found.")

        #############################
        # Read AnalysisMetaData.xml
        #############################
        componentInstanceID_AnalysisMetaData = {}
        retrieve_AnalysisMetaData(analysisMetaData_path, componentInstanceID_AnalysisMetaData )
        log_AnalysisMetaData(componentInstanceID_AnalysisMetaData)
        check_AnalysisMetaData(componentInstanceID_AnalysisMetaData)

        ######################################################################
        # Read ComputedValues.xml   Note - This contains the point coordinates
        ######################################################################
        metricID_ComputedValues = {}
        retrieve_ComputedValues(computedValues_path, metricID_ComputedValues)
        g_logger.info("")
        log_ComputedValues(metricID_ComputedValues)

        #########################
        # Open cADAssembly_tree
        #########################
        cADAssembly_tree = letree.parse(cADAssembly_path)
        cADAssembly_root = cADAssembly_tree.getroot()

        ##########################################
        # Determine if a Surface or Solids Model
        ##########################################
        # Note - A mixture of surfaces and Solids is NOT supported
        surface_model_bool = is_a_surface_model( cADAssembly_root )

        if surface_model_bool:
            g_logger.info("Model type: Surface Model")
        else:
            g_logger.info("Model type: Solid Model")

        ##########################################
        # ??? need to verify that if a surface model all components have elements
        ##########################################
        # zzzzzzzzzzzzz

        ##########################################
        # Get material names used in model
        ##########################################
        materialNamesUsedInModel_set = set()
        if surface_model_bool:
            get_surface_model_materials(cADAssembly_root, materialNamesUsedInModel_set)
        else:
            get_solid_model_materials(componentInstanceID_AnalysisMetaData, materialNamesUsedInModel_set)

        g_logger.info("Materials used in model:")
        for matrl in materialNamesUsedInModel_set:
            g_logger.info("   " + matrl)

        materialNames_index = {}
        index = 1
        for matrl in materialNamesUsedInModel_set:
            materialNames_index[matrl] = index
            index += 1

        g_logger.info("Materials to Index:")
        for k, v in materialNames_index.items():
            g_logger.info("   " + k + "    " + str(v))

        ##############################
        # Get Top-Level Assembly Name
        ##############################
        topLevelAssemblyName = get_top_level_assembly_name(cADAssembly_root)
        g_logger.info("Top-level assembly name: " + topLevelAssemblyName )

        #####################################
        # Open CreatePatranModelInput.txt
        #####################################
        outputModelFile_name = "CreatePatranModelInput.txt"
        outputModelFile_obj = open(outputModelFile_name,'w')
        outputModelFile_obj.write( '# Timestamp: {:%Y-%m-%d %H:%M:%S}'.format(datetime.datetime.now()))
        g_logger.info("File " + outputModelFile_name + " opened for writing.")

        ######################
        # Write Files section
        ######################
        g_logger.info("Calling   write_Files_section")
        write_Files_section(outputModelFile_obj, topLevelAssemblyName)
        g_logger.info("Completed write_Files_section")

        ######################################
        # Retrieve and Write Analysis section
        ######################################
        analysisData = AnalysisData_struct()
        get_AnalysisData(cADAssembly_root, analysisData)
        #print(analysisData)
        g_logger.info("Calling   write_Analysis_section")
        write_Analysis_section(outputModelFile_obj, analysisData)
        g_logger.info("Completed write_Analysis_section")

        ########################
        # Write SubCase section
        ########################
        g_logger.info("Calling   write_SubCase_section")
        write_SubCase_section(outputModelFile_obj)
        g_logger.info("Completed write_SubCase_section")

        #############################################
        # Retrieve and write Mesh_Parameters section
        #############################################
        meshParameters = MeshParameters_struct()
        get_MeshParameters(cADAssembly_root, meshParameters)
        #print meshParameters
        g_logger.info("Calling   write_MeshParameters_section")
        write_MeshParameters_section(outputModelFile_obj, meshParameters)
        g_logger.info("Completed write_MeshParameters_section")
        ##########################
        # Write Material sections
        ##########################
        g_logger.info("Calling   write_Material_section")
        for k, v in materialNames_index.items():
            # k - material name in material library, v - integer of material ID, which will be written to outputModelFile_obj
            write_Material_section(outputModelFile_obj, k, str(v))
        g_logger.info("Completed write_Material_section")


        ###############################
        # Write Surface/Solid Sections
        ###############################
        id_counters = IDCounters

        if surface_model_bool:
            g_logger.info("Calling   output_Surface_constructs")
            output_Surface_constructs(outputModelFile_obj,
                                      id_counters,
                                      cADAssembly_root,
                                      metricID_ComputedValues,
                                      materialNames_index,
                                      analysisData.shellElementType,  # Plate4 or Plate8
                                      "1")  # Mesh_Paramters_ID is always 1 for now
            g_logger.info("Completed   output_Surface_constructs")
        else:
            g_logger.info("Calling   output_Solid_constructs")
            output_Solid_constructs(outputModelFile_obj,
                                    id_counters,
                                    componentInstanceID_AnalysisMetaData,
                                    materialNames_index,
                                    "1")  # Mesh_Paramters_ID is always 1 for now
            g_logger.info("Completed   output_Solid_constructs")


        #####################
        # Output Constraints
        #####################
        g_logger.info("Calling   output_Constraints_constructs")
        output_Constraints_constructs(outputModelFile_obj,
                                  id_counters,
                                  cADAssembly_root,
                                  metricID_ComputedValues,
                                  "1"    ) # subcase_ID is always 1 for now
        g_logger.info("Completed output_Constraints_constructs")

        ################
        # Output Loads
        ################
        g_logger.info("Calling   output_Loads_constructs")
        output_Loads_constructs(outputModelFile_obj,
                                  id_counters,
                                  cADAssembly_root,
                                  metricID_ComputedValues,
                                  "1"    ) # subcase_ID is always 1 for now
        g_logger.info("Completed output_Loads_constructs")

        outputModelFile_obj.close()

        g_logger.info("")
        g_logger.info("CreatePatranInputFile.py ran successfully.    " +
                      'Timestamp: {:%Y-%m-%d %H:%M:%S}'.format(datetime.datetime.now()))

    except Exception as e:
        g_logger.error(e.message)
        #print("Error" + e.message)
        failed_File = open('_FAILED.txt', 'a')
        failed_File.write("Error" + e.message)
        failed_File.write("  See .\log\CreatePatranInputFile.py.log for more information.")
        failed_File.close()
        failed_File = open('..\..\_FAILED.txt', 'a')
        failed_File.write("Error" + e.message)
        failed_File.write("  See .Analysis\Patran_Nastran\log\CreatePatranInputFile.py.log for more information.")
        failed_File.close()
        errorOccurred = True

    if errorOccurred:
        return -1
    else:
        succeeded_File = open('_SUCCEEDED_CreatePatranModelInput.txt', 'w')
        succeeded_File.write("CreatePatranModelInput.py succeeded.  See .\log\CreatePatranInputFile.py.log for more information")
        succeeded_File.close()
        return -0



if __name__ == '__main__':

    main()