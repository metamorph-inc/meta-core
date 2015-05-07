__author__ = 'rowens'

# Description:
#    This modules computes the allowable stress properties and populates the class AnalysisMaterialPropertiesAllowables
#    See  compute_allowable_stress_levels
#    for a description of the algorithm for computing the allowables and the list of possible exceptions.
#
#    See ..\..\..\CADAssembler\Python\Test\cad_library\allowable_stress_level_test.py for an example of how to invoke this module.

import cad_exception_classes as CADExcep
import unit_utils


MECHANICAL_INFINITE_CYCLES_INDICATOR = 0
MECHANICAL_INFINITE_CYCLES_THRESHOLD = 500000000  # => means that infinite life fatigue applies
MECHANICAL_NON_FATIGUE_THRESHOLD = 1000  # < means that fatigue is not a factor
# between MECHANICAL_NON_FATIGUE_THRESHOLD and MECHANICAL_INFINITE_CYCLES_THRESHOLD is not yet addressed by
# this file

MECHANICAL_MATERIAL_PROPERTY_SOURCE_SPECIFIED = "Specified"
MECHANICAL_MATERIAL_PROPERTY_SOURCE_DERIVED = "Derived"


class AnalysisMaterialPropertiesAllowables:
    def __init__(self, in_material_name):
        self.material_name = in_material_name
        #
        self.mechanical__strength_tensile_defined = False
        self.mechanical__strength_bearing_defined = False
        self.mechanical__strength_shear_defined = False
        self.mechanical__number_of_cycles_defined = True
        #
        self.mechanical__strength_tensile = 0.0
        self.mechanical__strength_bearing = 0.0
        self.mechanical__strength_shear = 0.0
        self.mechanical__number_of_cycles = 0
        #
        self.mechanical__strength_tensile_unit = unit_utils.MECHANICAL_UNIT_NONE
        self.mechanical__strength_bearing_unit = unit_utils.MECHANICAL_UNIT_NONE
        self.mechanical__strength_shear_unit = unit_utils.MECHANICAL_UNIT_NONE
        self.mechanical__number_of_cycles_unit = unit_utils.MECHANICAL_UNIT_NONE
        #
        self.mechanical__strength_tensile_source = MECHANICAL_MATERIAL_PROPERTY_SOURCE_DERIVED
        self.mechanical__strength_bearing_source = MECHANICAL_MATERIAL_PROPERTY_SOURCE_DERIVED
        self.mechanical__strength_shear_source = MECHANICAL_MATERIAL_PROPERTY_SOURCE_DERIVED
        self.mechanical__number_of_cycles_source = MECHANICAL_MATERIAL_PROPERTY_SOURCE_SPECIFIED

    def __str__(self):
        return "material_name                        " + self.material_name \
        \
        + "\nmechanical__strength_tensile_defined " + str(self.mechanical__strength_tensile_defined) \
        + "\nmechanical__strength_bearing_defined " + str(self.mechanical__strength_bearing_defined) \
        + "\nmechanical__strength_shear_defined   " + str(self.mechanical__strength_shear_defined) \
        + "\nmechanical__number_of_cycles_defined " + str(self.mechanical__number_of_cycles_defined) \
        \
        + "\nmechanical__strength_tensile         " + str(self.mechanical__strength_tensile) \
        + "\nmechanical__strength_bearing         " + str(self.mechanical__strength_bearing) \
        + "\nmechanical__strength_shear           " + str(self.mechanical__strength_shear) \
        + "\nmechanical__number_of_cycles         " + str(self.mechanical__number_of_cycles) \
        \
        + "\nmechanical__strength_tensile_unit    " + str(self.mechanical__strength_tensile_unit) \
        + "\nmechanical__strength_bearing_unit    " + str(self.mechanical__strength_bearing_unit) \
        + "\nmechanical__strength_shear_unit      " + str(self.mechanical__strength_shear_unit) \
        + "\nmechanical__number_of_cycles_unit    " + str(self.mechanical__number_of_cycles_unit) \
        \
        + "\nmechanical__strength_tensile_source  " + str(self.mechanical__strength_tensile_source) \
        + "\nmechanical__strength_bearing_source  " + str(self.mechanical__strength_bearing_source) \
        + "\nmechanical__strength_shear_source    " + str(self.mechanical__strength_shear_source) \
        + "\nmechanical__number_of_cycles_source  " + str(self.mechanical__number_of_cycles_source)


# Description:
#   This function converts the units in in_out_props from Pa to MPa.
#   Note - the units are case insensitive.  PA == Pa == pA == pa
#
# Pre-Conditions:
#   None
# Post-Condtions:
#   If a property is defined ( e.g.  if (in_out_props.mechanical__strength_tensile_defined):) and the
#   units for that property are not PA
#       raise CADExcep.CADError
#   If no exception
#      Modify in_out_props to have the units of MPa for the defined properties in in_out_props

def convert_allowables_from_Pa_to_MPa(in_out_props):  # AnalysisMaterialPropertiesAllowables

    function_name = "convert_allowables_from_Pa_to_MPa"

    if (in_out_props.mechanical__strength_tensile_defined):
        if (in_out_props.mechanical__strength_tensile_unit.upper() != unit_utils.MECHANICAL_UNIT_STRESS_PA.upper()):
            raise CADExcep.CADError("Error, Function: " + function_name + ", Message: " +
                                    "mechanical__strength_tensile_unit != " + unit_utils.MECHANICAL_UNIT_STRESS_PA)
        in_out_props.mechanical__strength_tensile = unit_utils.convert_Pa_MPa(in_out_props.mechanical__strength_tensile)
        in_out_props.mechanical__strength_tensile_unit = unit_utils.MECHANICAL_UNIT_STRESS_MPA

    if (in_out_props.mechanical__strength_bearing_defined):
        if (in_out_props.mechanical__strength_bearing_unit.upper() != unit_utils.MECHANICAL_UNIT_STRESS_PA.upper()):
            raise CADExcep.CADError("Error, Function: " + function_name + ", Message: " +
                                    "mechanical__strength_bearing != " + unit_utils.MECHANICAL_UNIT_STRESS_PA)
        in_out_props.mechanical__strength_bearing = unit_utils.convert_Pa_MPa(in_out_props.mechanical__strength_bearing)
        in_out_props.mechanical__strength_bearing_unit = unit_utils.MECHANICAL_UNIT_STRESS_MPA

    if (in_out_props.mechanical__strength_shear_defined):
        if (in_out_props.mechanical__strength_shear_unit.upper() != unit_utils.MECHANICAL_UNIT_STRESS_PA.upper()):
            raise CADExcep.CADError("Error, Function: " + function_name + ", Message: " +
                                    "mechanical__strength_shear != " + unit_utils.MECHANICAL_UNIT_STRESS_PA)
        in_out_props.mechanical__strength_shear = unit_utils.convert_Pa_MPa(in_out_props.mechanical__strength_shear)
        in_out_props.mechanical__strength_shear_unit = unit_utils.MECHANICAL_UNIT_STRESS_MPA



def stress_value_computed(in_value_1_defined,
                          in_value_1,
                          in_value_2_defined,
                          in_value_2,
                          in_refernece_value):
    if (in_value_1_defined and in_value_2_defined):
        return (in_refernece_value / in_value_1) * in_value_2
    else:
        return 0.0


"""
 General Rules:
    1) If in_NumberOfCycles == MECHANICAL_INFINITE_CYCLES_INDICATOR assume infinite number of cycles
    2) Consider 500,000,000 cycles to be infinite cycles
    3) Consider less than 1,000 cycles to be equivalent to 1 cycle for determining the allowable stress levels.
    3) Aluminum must have in_AnalysisMaterialProperties.fatigueStrength and fatigueNumberOfCycles set.
    4) For Steel, if in_AnalysisMaterialProperties.fatigueStrength is not set, assume that it is
       50% of in_AnalysisMaterialProperties.tensileUltimateStrength
    5) TBD For cycle values between 1,000 and 500,000,000, we need a graph of X-Number Cycles, and Y-Stress.  These
       are usually log base 10 graphs.

    Material        Fatigue   Number      Tensile                        Bearing                       Shear
                    Given     Cycles      Strength                       Strength                      Strength
    -----------     --------  -------     --------------                 -----------                   ---------------
    Steel/Aluminum  Yes       N < 1000    tensileYieldStrength           bearingYieldStrength          shearStrength
    Stee/Aluminum   Yes       1000 < N    fatigueStrength                SF * bearingUltimateStrength  SF * shearStrength
    Steel           No        1000 < N    .5 * tensileUltimateStrength   SF  * bearingYieldStrength    SF  * shearStrength
    Aluminum        No        (Not Allowed, throw exception)

    SF - Scale Factor = fatigueStrength / tensileUltimateStrength;

    CADExcep.CADError will be raised if:
        1) in_props material other than aluminum or steel.  Other materials are not supported at this time.
        2) in_props unit are not consistent (e.g. PA for stress, not mixtures of Pa and MPa) for all defined properties.
        3) in_props.mechanical__strength_tensile_yield not defined
        4) in_props.mechanical__strength_tensile_ultimate not defined
        5) for aluminum in_props.mechanical__strength_shear not defined
        6) for aluminum in_props.mechanical__strength_fatigue not defined
        7) in_num_cycles != MECHANICAL_INFINITE_CYCLES_INDICATOR and \
           MECHANICAL_NON_FATIGUE_THRESHOLD < in_num_cycles < MECHANICAL_INFINITE_CYCLES_THRESHOLD
           In other words, only non-infinite or infininte cycles are supported.  The middle portion would require
           a fatigue analysis, which is not currently supported.

     If a property is defined in in_props, then the unit must be consistent; otherwise an exception would be raised.
"""
def compute_allowable_stress_levels(in_num_cycles,
                                    in_props,     #_AnalysisMaterialProperties
                                    out_allow):  #_AnalysisMaterialPropertiesAllowables

    function_name = "compute_allowable_stress_levels"

    #####################
    # zero out values
    #####################
    out_allow.material_name = ""
    out_allow.mechanical__strength_tensile = 0.0
    out_allow.mechanical__strength_bearing = 0.0
    out_allow.mechanical__strength_shear = 0.0
    out_allow.mechanical__strength_tensile_defined = False
    out_allow.mechanical__strength_bearing_defined = False
    out_allow.mechanical__strength_shear_defined = False


    #########################################
    # Get type of material /steel or aluminum
    #########################################
    steel_material = False
    aluminum_material = False

    if (in_props.material_name.lower().find("steel") != -1):
        steel_material = True
    elif in_props.material_name.lower().find("aluminum") != -1:
        aluminum_material = True

    ###############################################
    # Only Aluminum and Steel Currently Supported
    ###############################################
    if not steel_material and not aluminum_material:
        raise CADExcep.CADError("Error, Function: " + function_name + ", Message: " +
                                "Function was passed " + in_props.material_name + ".  Currently only " +
                                "Steel and Aluminum are supported. Plastic and Ceramic will be supported " +
                                "in future releases.")

    #######################################################
    # Fatigue strength must be defined for Aluminum
    #######################################################
    if aluminum_material and not in_props.mechanical__strength_fatigue_defined:
        raise CADExcep.CADError("Error, Function: " + function_name + ", Message: " +
                                "Material " +  in_props.material_name + " does not have a fatigue strength defined. " +
                                "Aluminum must have fatigue strength defined.")

    ########################################
    # Tensile yield strength must be given
    ########################################
    if not in_props.mechanical__strength_tensile_yield_defined:
        raise CADExcep.CADError("Error, Function: " + function_name + ", Message: " +
                                "Material " +  in_props.material_name +
                                " does not have a tensile yield strength defined. " +
                                "All materials must have tensile yield strength defined.")

    ########################################
    # Tensile ultimate strength must be given
    ########################################
    if not in_props.mechanical__strength_tensile_ultimate_defined:
        raise CADExcep.CADError("Error, Function: " + function_name + ", Message: " +
                                "Material " +  in_props.material_name +
                                " does not have a tensile ultimate strength defined. " +
                                "All materials must have tensile ultimate strength defined.")

    ############################################
    # Shear Strength must be given for Aluminum
    ############################################
    if aluminum_material and not in_props.mechanical__strength_shear_defined:
        raise CADExcep.CADError("Error, Function: " + function_name + ", Message: " +
                                "Material " +  in_props.material_name +
                                " does not have a shear strength defined. " +
                                "Aluminum must have shear strength defined.")

    ##############################
    # Set Material Name and Type
    ##############################
    out_allow.material_name = in_props.material_name

    ##############################
    # Set Number of cycles
    ##############################
    out_allow.mechanical__number_of_cycles = in_num_cycles

    ################################################
    # Determine Unit, Stress Unit must be the same
    ################################################
    # The unit for stress should be consistent and typically PA
    # At this point tensile yield and tensile ultimate must be defined

    if len(in_props.mechanical__strength_tensile_yield_unit) == 0:
        raise CADExcep.CADError("Error, Function: " + function_name + ", Message: " +
                        "Tensile yield strength unit not defined for " +
                        in_props.material_name + ".")

    if in_props.mechanical__strength_tensile_ultimate_unit == 0:
        raise CADExcep.CADError("Error, Function: " + function_name + ", Message: " +
                        "Tensile yield strength unit not defined for " +
                        in_props.material_name + ".")

    ################################################
    # Verify consistent unit
    ################################################
    overall_unit = in_props.mechanical__strength_tensile_yield_unit
    overall_unit_upper_case = overall_unit.upper()

    not_consistent_unit_msg = "not consistent with mechanical__strength_tensile_yield_unit(" + \
                        in_props.mechanical__strength_tensile_yield_unit + \
                        ").  Note - unit are case insensitive."

    # tensile_ultimate
    if overall_unit_upper_case != in_props.mechanical__strength_tensile_ultimate_unit.upper():
        raise CADExcep.CADError("Error, Function: " + function_name + ", Message: " +
                        "mechanical__strength_tensile_ultimate_unit unit (" +
                        in_props.mechanical__strength_tensile_ultimate_unit + ") " + not_consistent_unit_msg)

    # bearing_yield
    if in_props.mechanical__strength_bearing_yield_defined and \
        overall_unit_upper_case != in_props.mechanical__strength_bearing_yield_unit.upper():
        raise CADExcep.CADError("Error, Function: " + function_name + ", Message: " +
                        "mechanical__strength_bearing_yield_unit unit (" +
                        in_props.mechanical__strength_bearing_yield_unit + ") " + not_consistent_unit_msg)

    # bearing_ultimate
    if in_props.mechanical__strength_bearing_ultimate_defined and \
        overall_unit_upper_case != in_props.mechanical__strength_bearing_ultimate_unit.upper():
        raise CADExcep.CADError("Error, Function: " + function_name + ", Message: " +
                        "mechanical__strength_bearing_ultimate_unit unit (" +
                        in_props.mechanical__strength_bearing_ultimate_unit + ") " + not_consistent_unit_msg)

    # shear
    if in_props.mechanical__strength_shear_defined and \
        overall_unit_upper_case != in_props.mechanical__strength_shear_unit.upper():
        raise CADExcep.CADError("Error, Function: " + function_name + ", Message: " +
                        "mechanical__strength_shear_unit unit (" +
                        in_props.mechanical__strength_shear_unit + ") " + not_consistent_unit_msg)

    # fatigue
    if in_props.mechanical__strength_fatigue_defined and \
        overall_unit_upper_case != in_props.mechanical__strength_fatigue_unit.upper():
        raise CADExcep.CADError("Error, Function: " + function_name + ", Message: " +
                        "mechanical__strength_fatigue_unit unit (" +
                        in_props.mechanical__strength_fatigue_unit + ") " + not_consistent_unit_msg)

    # modulus_elastic
    if in_props.mechanical__modulus_elastic_defined and \
        overall_unit_upper_case != in_props.mechanical__modulus_elastic_unit.upper():
        raise CADExcep.CADError("Error, Function: " + function_name + ", Message: " +
                        "mechanical__modulus_elastic_unit unit (" +
                        in_props.mechanical__strength_fatigue_unit + ") " + not_consistent_unit_msg)


    ###############################
    # Compute temp_shear_strength
    ###############################
    if in_props.mechanical__strength_shear_defined:
        temp_shear_strength = in_props.mechanical__strength_shear
        out_allow.mechanical__strength_shear_source = MECHANICAL_MATERIAL_PROPERTY_SOURCE_SPECIFIED
    else:
        # http://www.roymech.co.uk/Useful_Tables/Matter/shear_tensile.htm
        # http://www.roymech.co.uk/Useful_Tables/Fatigue/Fatigue.html
        # Case for shear strength is not defined, use rule of thumb
        # This would more closely align with the shear yield strength
        temp_shear_strength = 0.58 * in_props.mechanical__strength_tensile_yield
        out_allow.mechanical__strength_shear_source = MECHANICAL_MATERIAL_PROPERTY_SOURCE_DERIVED

    ######################
    # No Fatigue Case
    ######################
    if in_num_cycles != MECHANICAL_INFINITE_CYCLES_INDICATOR and in_num_cycles <= MECHANICAL_NON_FATIGUE_THRESHOLD:
        out_allow.mechanical__strength_tensile = in_props.mechanical__strength_tensile_yield
        out_allow.mechanical__strength_tensile_source = MECHANICAL_MATERIAL_PROPERTY_SOURCE_SPECIFIED
        out_allow.mechanical__strength_shear = temp_shear_strength
        if (in_props.mechanical__strength_bearing_yield_defined):
            out_allow.mechanical__strength_bearing = in_props.mechanical__strength_bearing_yield
            out_allow.mechanical__strength_bearing_source = MECHANICAL_MATERIAL_PROPERTY_SOURCE_SPECIFIED
        else:
            out_allow.mechanical__strength_bearing = in_props.mechanical__strength_tensile_yield
            out_allow.mechanical__strength_bearing_source = MECHANICAL_MATERIAL_PROPERTY_SOURCE_DERIVED
    else:
        #####################################
        # Fatigue Case - Steel and Aluminum
        #####################################
        # // Note fatigue strength would be defined for aluminum as checked above, maybe not for steel.
        if in_props.mechanical__strength_fatigue_defined:
            temp_fatigue_strength = in_props.mechanical__strength_fatigue
            out_allow.mechanical__strength_tensile_source = MECHANICAL_MATERIAL_PROPERTY_SOURCE_SPECIFIED
        else:
            temp_fatigue_strength = .5 * in_props.mechanical__strength_tensile_ultimate
            out_allow.mechanical__strength_tensile_source = MECHANICAL_MATERIAL_PROPERTY_SOURCE_DERIVED

        #############
        # Tensile
        #############
        out_allow.mechanical__strength_tensile = temp_fatigue_strength

        #############
        # Bearing
        #############
        stress_computed = stress_value_computed(in_props.mechanical__strength_tensile_ultimate_defined,
                                                in_props.mechanical__strength_tensile_ultimate,
                                                in_props.mechanical__strength_bearing_ultimate_defined,
                                                in_props.mechanical__strength_bearing_ultimate,
                                                temp_fatigue_strength)

        if stress_computed == 0.0:
            stress_computed = stress_value_computed(in_props.mechanical__strength_tensile_yield_defined,
                                                in_props.mechanical__strength_tensile_yield,
                                                in_props.mechanical__strength_bearing_yield_defined,
                                                in_props.mechanical__strength_bearing_yield,
                                                temp_fatigue_strength)

        if stress_computed == 0.0:
            out_allow.mechanical__strength_bearing = temp_fatigue_strength
        else:
            out_allow.mechanical__strength_bearing = stress_computed

        out_allow.mechanical__strength_bearing_source = MECHANICAL_MATERIAL_PROPERTY_SOURCE_DERIVED

        ############
        # Shear
        ############
        stress_computed = stress_value_computed(in_props.mechanical__strength_tensile_ultimate_defined,
                                                in_props.mechanical__strength_tensile_ultimate,
                                                True,
                                                temp_shear_strength,
                                                temp_fatigue_strength)
        if stress_computed == 0.0:
            stress_computed = stress_value_computed(in_props.mechanical__strength_tensile_yield_defined,
                                                    in_props.mechanical__strength_tensile_yield,
                                                    True,
                                                    temp_shear_strength,
                                                    temp_fatigue_strength)

        if stress_computed == 0.0:
            out_allow.mechanical__strength_shear = .5 * in_props.mechanical__strength_shear
        else:
            out_allow.mechanical__strength_shear = stress_computed

        out_allow.mechanical__strength_shear_source = MECHANICAL_MATERIAL_PROPERTY_SOURCE_DERIVED

    ###############################################
    # At this point, the strength properties
    # should be set.  It would be a programming
    # error if this was not the case. Throw
    # and exception if the strength properties
    # are not set.
    ###############################################

    if  out_allow.mechanical__strength_tensile == 0.0 or \
        out_allow.mechanical__strength_bearing == 0.0 or \
        out_allow.mechanical__strength_shear == 0.0:
        raise CADExcep.CADError("Error, Function: " + function_name + ", Message: " +
                    "failed to compute the all of the strength material properties.  Computed properties are: " +
                    "Tensile Strength: " +  str(out_allow.mechanical__strength_tensile) +
                    ", Bearing Strength: " + str(out_allow.mechanical__strength_bearing) +
                    ", Shear Strength: " + str(out_allow.mechanical__strength_shear))

    out_allow.mechanical__strength_tensile_defined = True
    out_allow.mechanical__strength_bearing_defined = True
    out_allow.mechanical__strength_shear_defined = True
    out_allow.mechanical__strength_tensile_unit = overall_unit
    out_allow.mechanical__strength_bearing_unit = overall_unit
    out_allow.mechanical__strength_shear_unit = overall_unit


