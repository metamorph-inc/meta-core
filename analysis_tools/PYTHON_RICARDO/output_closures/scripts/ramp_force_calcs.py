"""
Placeholder file: translate equations from CAB's "Ramp mechanism_22May.xmcd" workup

Calculates the forces required to open/close a ramp
"""
__author__ = 'ab27'

import logging, os, sys
import _winreg

import numpy as np
import matplotlib.pyplot as plt
from matplotlib.tri import Triangulation
from matplotlib.ticker import NullFormatter

import scipy.optimize as opt
# rpl is imported after environment setup

g = 9.807  # m/s**2 - ensure that units are always consistent with this!

# Known types of component that this tool can operate on
RAMP_TYPES = {"Hatch_Assembly_Rear_Ramp"}
ACTUATOR_TYPES = {"Actuator_Linear_Electrohydraulic", "Actuator_Linear_Electromechanical",
                  "Actuator_Linear_Hydraulic"}
# Pneumatic and rotary_hydraulic class actuators are explicitly not being considered.


#######
# Setup for VU test bench environment
def query_analysis_tools():
    analysis_tools_key_name = r'SOFTWARE\Wow6432Node\META\AnalysisTools'

    analysis_tools_key = _winreg.OpenKey(_winreg.HKEY_LOCAL_MACHINE, analysis_tools_key_name, 0,
                                         _winreg.KEY_READ | _winreg.KEY_WOW64_32KEY)

    number_of_keys = _winreg.QueryInfoKey(analysis_tools_key)[0]  # 0 means number of sub_keys

    info = {}
    for sub_key_id in range(0, number_of_keys):
        sub_key_name = _winreg.EnumKey(analysis_tools_key, sub_key_id)
        sub_key = _winreg.OpenKey(analysis_tools_key, sub_key_name)
        info[sub_key_name] = {}
        number_of_values = _winreg.QueryInfoKey(sub_key)[1]
        for value_id in range(0, number_of_values):
            value_tuple = _winreg.EnumValue(sub_key, value_id)
            value_name = value_tuple[0]
            value = value_tuple[1]
            info[sub_key_name][value_name] = value
    return info


tools = query_analysis_tools()
sys.path.append(os.path.join(tools['closures']['InstallLocation']))

from rpl.tools.api import test_bench_api as tba


def get_parts_of_interest():
    """
    List of parts and properties to query

    Due to differently named DB fields / behavior, each separate actuator has a slightly
    different list of requested fields.
    """
    # TODO: in future, add a way to determine the dist from hinge to ground
    poss_parts = {"Hatch_Assembly_Rear_Ramp":
                      {"Property": ["ramp_angle", "hatch_length", 
                                    "mechanism_position_x1", "mechanism_position_y1",
                                    "mechanism_position_x2", "mechanism_position_y2",
                                    "mechanism_position_x3", "mechanism_position_y3",
                                    "mechanism_position_x4", "mechanism_position_y4",
                                    "mechanism_position_x5", "mechanism_position_y5"],
                       "Datum": ["CG_CSYS", "EXT_HINGE_PIN"],
                       "ModelMetric": ["TOTAL_MASS"],
                       "Required": "{+}"},
                  "Actuator_Linear_Electrohydraulic":
                      {"Property": ["max_stroke_length", "compressed_length",
                                    "max_dynamic_load_retract"],
                       "Required": "{*}"},
                  "Actuator_Linear_Electromechanical":
                      {"Property": ["max_stroke_length", "compressed_length",
                                    "max_dynamic_load"],
                       "Required": "{*}"},
                  "Actuator_Linear_Hydraulic":
                      {"Property": ["bore_diameter", "rod_diameter",
                                    "max_pressure", "actuation_pressure",
                                    "max_stroke_length", "compressed_length",
                                    "max_dynamic_load_retract"],
                       "Required": "{*}"}
                  }
    return poss_parts


# TODO: move to part_info with the other component objects
class Actuator_Info(object):
    """
    Stores the object parameters from an actuator. Chooses settings based on type of actuator.
    """
    def __init__(self, api_dict, part_id, act_type):
        self.part_id = part_id
        self.act_type = act_type  # The type of actuator.

        #### Same for all actuators
        self.compressed_length = api_dict['properties']['compressed_length']
        self.max_stroke_length = api_dict['properties']['max_stroke_length']
        # The actual value of max length is != stroke length alone:
        self.max_length = self.compressed_length + self.max_stroke_length

        ### Set these fields to values only if actuator is linear_hydraulic type
        is_hyd = act_type.upper() == "ACTUATOR_LINEAR_HYDRAULIC"
        self.bore_diameter = api_dict['properties']['bore_diameter'] if is_hyd else None
        self.rod_diameter = api_dict['properties']['rod_diameter'] if is_hyd else None
        self.act_pressure = api_dict['properties']['actuation_pressure'] if is_hyd else None
        self.max_pressure = api_dict['properties']['max_pressure'] if is_hyd else None
        
        if is_hyd:
            self.max_load = self.act_force_hydraulic()
        else:
            # The desired property will have different names in different actuators. The dict will
            # only have one of these, so this would return the one that is not None.
            self.max_load = api_dict['properties'].get('max_dynamic_load_retract') or\
                            api_dict['properties'].get('max_dynamic_load')

        assert self.max_load is not None, "Max actuator load unknown for {}".format(part_id)

    def act_force_hydraulic(self):
        """
        For linear hydraulic actuators specifically, calculate the max actuator force for the
        actuator based on the system pressure.  Since capabilities for this type depend on installed
        conditions, can't just use the db value directly.
        """
        area = np.pi * (self.bore_diameter**2 - self.rod_diameter**2) / 4.
        return self.act_pressure * area
        

# TODO: move to part_info with the other component objects
class Rear_Ramp(object):
    """
    Contains all parameters and values necessary for determining ramp force and actuator length
    """
    def __init__(self, api_part_dict, part_id, ground_slope_pct=30.):
        self.part_id = part_id
        self.actuator = None  # Placeholder for assigning an actuator object

        self.hinge_csys = None

        # The ground slope angle can be determined from the % slope that the vehicle sits on
        # http://en.wikipedia.org/wiki/Grade_%28slope%29#Equations
        self.ground_slope_ang = np.arctan(ground_slope_pct / 100.)
        # TODO: Note that sign is important in the above: as the angle is defined, +30 = easier to
        # open, -30% slope = harder to open

        # Geometry "hard point coords (ramp closed position)"
        # or are converted to be so. (in vehicle csys)
        self.pt1 = None
        self.pt2 = None
        self.pt3 = None
        self.pt4 = None
        self.pt5 = None

        self.installed_angle = None  # Ramp installed angle, closed

        self.length = None  # Ramp length

        self.height_to_ground = None  # Dist from hinge center to ground
        self.mass = None

        ### Initialize values
        if api_part_dict is None and part_id is None:
            self.testing_data()
        else:
            self.extract_data(api_part_dict)

        ########### Calculated parameters
        # Point locus radii
        self.r1 = np.linalg.norm(self.pt1)
        self.r2 = np.linalg.norm(self.pt3 - self.pt2)
        self.r4 = np.linalg.norm(self.pt4 - self.pt3)

        self.linkage_length = np.linalg.norm(self.pt1 - self.pt2)

        self.r1_ang_closed = np.arccos(self.pt1[0] / self.r1)
        self.r2_ang_closed = np.arccos((self.pt2[0] - self.pt3[0]) / self.r2)
        self.r4_ang_closed = np.arccos((self.pt4[0] - self.pt3[0]) / self.r4)

        self.act_a3 = np.linalg.norm(self.pt5-self.pt3)

        self.length_cg = self.length / 2.

        # The vehicle spec calls for the ramp to always be able to touch a point 15cm lower
        # than the ground. Hardcode this parameter.
        ramp_ang_at_grd_contact = np.arcsin((self.height_to_ground - 15. * 0.01) / self.length)
        self.max_open_angle = self.installed_angle - ramp_ang_at_grd_contact

        self.angle_range = self._calc_angle_range()

    def extract_data(self, api_part_dict):
        """
        Extract data from api get_data dictionary for this part
        """
        self.pt1 = np.array([api_part_dict['properties']['mechanism_position_x1'],
                             api_part_dict['properties']['mechanism_position_y1']])
                            
        self.pt2 = np.array([api_part_dict['properties']['mechanism_position_x2'],
                             api_part_dict['properties']['mechanism_position_y2']])
                             
        self.pt3 = np.array([api_part_dict['properties']['mechanism_position_x3'],
                             api_part_dict['properties']['mechanism_position_y3']])
                             
        self.pt4 = np.array([api_part_dict['properties']['mechanism_position_x4'],
                             api_part_dict['properties']['mechanism_position_y4']])

        self.pt5 = np.array([api_part_dict['properties']['mechanism_position_x5'],
                             api_part_dict['properties']['mechanism_position_y5']])
                       
        self.installed_angle = api_part_dict['properties']['ramp_angle']
        self.length = api_part_dict['properties']['length']

        # Todo: dist from hinge to ground is hardcoded for now in SETTINGS.
        self.height_to_ground = None ### Distance from hinge to ground
        self.mass = api_part_dict['model_metrics']['TOTAL_MASS']

        ### These properties aren't used- all points are already rel to hinge position
        ## Coordinate system treated as origin for calculations
        # self.hinge_csys = api_part_dict['datums']['EXT_HINGE_PIN']['global']

        # Coordinate system marking CG for entire ramp assembly (incl non-rotating parts)
        # self.cg_csys = api_part_dict['datums']['CG_CSYS']['global']

    def testing_data(self):
        """
        If called, sets all object properties to default bundle of values (from mathcad sheet)
        for testing
        """
        # Geometry "hard point coords (ramp closed position)"
        self.pt1 = np.array([-91.1, 275.3]) * .001
        self.pt2 = np.array([-596.6, -13.1]) * .001
        self.pt3 = np.array([-188.7, -193.4]) * .001
        self.pt4 = np.array([-408.7, -124.2]) * .001
        self.pt5 = np.array([-842., -175.]) * .001

        self.installed_angle = 76. * np.pi / 180.  # Ramp installed angle, closed

        #self.open_angle = 20. * np.pi / 180. #None  # Ramp open angle (what is the meaning of this
        # fixed value?) Deprecate so that we can do things as function of open angle later

        self.length = 1767. * .001  # Ramp length

        self.height_to_ground = -681.6 * .001
        self.mass = 300.

        # Dummy in case any coord transforms get called
        self.hinge_csys = np.eye(4)

    @staticmethod
    def p2_solver_func(variables, pt3=None, r2=None, x1=None, y1=None, linkage_length=None):
        """
        Solves for the position of point 2 when hatch is open; provides func in form needed for
        fsolve.
        """

        # Accepts initial guess values as args to the function. The order on the kw args matter,
        #  because fsolve just passes in a tuple and treats kwargs as positional args
        x2, y2 = variables
        # Assumes that certain required variables were defined in whatever function is calling this.
        # A hack, but scipy.optimize syntax seems to expect list of vars to be optimized
        #x3, y3, r2, x1, y1, linkage_length = args
        eqn1 = (x2 - pt3[0])**2 + (y2-pt3[1])**2 - r2**2
        eqn2 = (x2 - x1)**2 + (y2-y1)**2 - linkage_length**2
        return eqn1, eqn2

    ### End of ramp coordinates
    def angle_open(self, ang):
        return self.installed_angle - ang

    def end_position(self, ang):
        x = self.length * np.cos(self.angle_open(ang))
        y = self.length * np.sin(self.angle_open(ang))
        return np.array([x, y])

    def r1_ang_open(self, ang):
        return self.r1_ang_closed - ang

    def p1_crds_open(self, ang):
        # Position of point r1
        x = self.r1 * np.cos(self.r1_ang_open(ang))
        y = self.r1 * np.sin(self.r1_ang_open(ang))
        return np.array([x, y])

    def p2_crds_open(self, ang):
        """
        Solve for point 2 coordinates (open). Pass in x & y guess values- 2 eqns, 2 unknown
        """
        # values- 2 eqns solves for 2 unknowns
        guess_vals = self.pt2[:2]
        r1o_x, r1o_y = self.p1_crds_open(ang)
        sln = opt.fsolve(self.p2_solver_func, guess_vals,
                         args=(self.pt3, self.r2, r1o_x, r1o_y, self.linkage_length))
        # TODO: add error handling in case solution doesn't converge, which might occur if
        # params are invalid somehow.
        return sln

    def r2_ang_open(self, ang):
        x2_ang = self.p2_crds_open(ang)[0]
        return np.arccos((x2_ang - self.pt3[0]) / self.r2)

    def r4_ang_open(self, ang):
        return self.r2_ang_open(ang) + (self.r4_ang_closed - self.r2_ang_closed)

    def p4_crds_open(self, ang):
        x = self.pt3[0] + self.r4 * np.cos(self.r4_ang_open(ang))
        y = self.pt3[1] + self.r4 * np.sin(self.r4_ang_open(ang))
        return np.array([x, y])

    ######
    # Actuator calculations
    def act_length(self, ang):
        return np.linalg.norm(self.pt5 - self.p4_crds_open(ang))

    ### Actuator force calcs
    def act_a1(self, ang):
        p2 = self.p2_crds_open(ang)
        return np.linalg.norm(p2)

    def act_d1(self, ang):
        return np.sqrt(self.r1**2 - ((self.r1**2 - self.act_a1(ang)**2 + self.linkage_length**2) /
                                     (2. * self.linkage_length)
                                     )**2)

    def act_a2(self, ang):
        return np.linalg.norm(self.p1_crds_open(ang) - self.pt3)

    def act_d2(self, ang):
        return np.sqrt(self.act_a2(ang)**2 -
                       ((self.act_a2(ang)**2 - self.r2**2 + self.linkage_length**2) /
                        (2. * self.linkage_length)
                        )**2)

    def act_d3(self, ang):
        return np.sqrt(self.r4**2 - ((self.r4**2 - self.act_a3**2 + self.act_length(ang)**2) /
                                     (2*self.linkage_length)
                                     )**2)

    ## Ramp cg position
    def pos_ramp_cg(self, ang):
        return self.end_position(ang) / 2.

    # Ramp mass moment arm, adjusted for longitudinal group slope angle
    def d_cg(self, ang):
        return self.length_cg * np.sin(np.pi / 2. - self.angle_open(ang) + self.ground_slope_ang)

    def linkage_force(self, ang):
        return (self.mass * g * self.d_cg(ang)) / self.act_d1(ang)

    def act_force(self, ang):
        return (self.linkage_force(ang) * self.act_d2(ang)) / self.act_d3(ang)

    def _calc_angle_range(self, ang_incr_deg=1.):
        # All possible ramp open angles through full range of motion
        ang_incr = ang_incr_deg * np.pi / 180.
        return np.arange(self.installed_angle, self.max_open_angle + ang_incr, ang_incr)

    def _trans2d(self, pt_vec):
        """
        Transform a 1x4 point vector (for use with 4x4 transformation matrix) to the 2d hatch
        plane assumed for all calculations. Rotates, sets z=0, and returns a 1x3 coordinate vector.

        Not currently used in this object, but might be needed in future for CG calcs
        """
        new_vec = np.dot(np.linalg.inv(self.hinge_csys), pt_vec)
        new_vec[2] = 0.
        return new_vec[:3]


class Assembly_Info(object):
    """
    Extract information about the assembly
    """
    def __init__(self):
        # get doors, get actuators. Initialize objects and connect.
        # Manually specified vertical distance from hinge center to ground in SETTINGS[
        # 'dist_to_ground'. According to mayavi, heights are 1.76m and 1.07 m, respectively.
        self.parts_found = tba.get_data(get_parts_of_interest())

        # Determine the connectivity of parts
        self.conn = self.get_connectivity()

        self.ramps = {}
        self.get_ramps()

        # Load in actuator data & connect each to the appropriate ramp
        self.get_actuators()

        # After assembly initialization, run sanity check
        assert len(self.ramps) > 0, "No ramps found in this assembly"

    def get_ramps(self):
        """
        Fetch data, then convert to objects
        """
        for t in RAMP_TYPES:
            # Get data for all ramps of this type, then create objs for all ramps of
            # that type in the vehicle
            ramps_dict = self.parts_found[t]

            for ramp_name in ramps_dict:
                ramp_obj = Rear_Ramp(ramps_dict[ramp_name], ramp_name)
                # TODO: Until ground info is ready, hardcode distance to ground
                ramp_obj.height_to_ground = SETTINGS['dist_to_ground'][ramp_name]
                self.ramps[ramp_name] = ramp_obj

    def get_connectivity(self):
        """
        Extract connectivity between parts: roof hatches connect to springs; ramps connect to
        actuators.

        TESTING PURPOSES ONLY- use manually specified part connectivity from settings file.
        """
        # TODO: Replace with extraction of real connectivity info
        return SETTINGS['connectivity']

    def get_actuators(self):
        """
        Get actuators associated with each ramp
        """
        for act_type in ACTUATOR_TYPES:
            acts_dict = self.parts_found[act_type]
            for act_name in acts_dict:
                act = Actuator_Info(acts_dict[act_name], act_name, act_type)
                # Get the hatch connected to this spring, then add object to hatch's spr_ax dict.
                conn_ramp = self.ramps.get(self.conn.get(act_name))
                if conn_ramp is None:
                    logging.error("Actuator {} is not associated with any ramp".format(act_name))
                else:
                    conn_ramp.actuator = act


###
# Test bench equation code from TB111 spec
def ramp_requirements_t31(ramp_obj):
    """
    Determine the actuator forces and lengths that would be required to open this hatch
    """
    forces_req = [ramp_obj.act_force(a) for a in ramp_obj.angle_range]
    lengths_req = [ramp_obj.act_length(a) for a in ramp_obj.angle_range]
    # should return max force requirement
    return forces_req, lengths_req


def passes_tests(ramp_obj, forces_req, lengths_req):
    """
    Determine whether the actuator attached to this ramp meets all requirements
    """
    force_ok = ramp_obj.actuator.max_load >= max(forces_req)
    length_ok = ramp_obj.actuator.max_length >= max(lengths_req)
    return force_ok, length_ok


def plt_ramp(rr):
    """
    Plotting commands for visualizing the rear ramp
    """
    #hatch_geom['nodes'] = np.vstack((hatch_geom['x'],
    #                                 hatch_geom['y'],
    #                                 hatch_geom['z'],
    #                                 np.ones(hatch_geom['x'].size))).T
    #
    #nodes_rot = np.dot(np.linalg.inv(h.hinge_csys), hatch_geom['nodes'].T).T

    ### Make plot of hatch geometry
    #tr = Triangulation(nodes_rot[:, 0],
    #                   nodes_rot[:, 1], triangles=hatch_geom['tris'])
    ## Plot filled contours, and mult tr.x by 0 because that makes everything be same color.
    #plt.tricontourf(tr, tr.x * 0.0, colors=(.5, .5, .5))

    origin = np.zeros(2)
    #  Points when closed don't make for a great visual sans accompanying overlaid hatch geom
    # ramp_points = zip(rr.pt1, rr.pt2, rr.pt3, rr.pt4, rr.pt5, origin)
    samp_ang = 20. * np.pi / 180.
    # Ramp points at a sample open position, to match mathcad representation
    ramp_points = zip(rr.end_position(samp_ang),
                      origin,
                      rr.p1_crds_open(samp_ang),
                      rr.p2_crds_open(samp_ang),
                      rr.pt3,
                      rr.p4_crds_open(samp_ang),
                      rr.pt5)
    plt.figure()
    plt.title("Side view")

    plt.plot(*ramp_points)
    plt.gca().set_aspect('equal')
    fn = "{}_ramp_coords_{}.png".format(SETTINGS['design_name'], rr.part_id)
    plt.savefig(fn)


def plt_ramp_results(rr, act_forces, act_lengths):
    """
    Plot 2D line graphs of results
    """
    plt.figure(figsize=(8, 3))
    plt.subplot(1, 2, 1)
    plt.title("Actuator forces")
    plt.plot(rr.angle_range * 180. / np.pi, act_forces, label="Actuator forces")

    plt.subplot(1, 2, 2)
    plt.title("Actuator lengths")
    plt.plot(rr.angle_range * 180. / np.pi, act_lengths, label="Actuator lengths")
    fn = "{}_act_lengths_{}.png".format(SETTINGS['design_name'], rr.part_id)
    plt.savefig(fn)


if __name__ == "__main__":
    ## Set up logging
    logging.info("\n" + 50 * "_" + '\nTest bench started\n' + 50 * "_")

    # Initialize API with user-specified settings from separate file
    SETTINGS = tba.load_settings("settings.js")
    logging.debug("Data loader API initialized")

    # Store results for all ramps overall
    all_results = {}

    # Actual test bench calculations
    all_results = {"Metrics": {}}
    all_tests_ok = True

    ## Testing data
    rr = Rear_Ramp(None, None)
    logging.debug("Rear ramp {} has the following parameters:".format(rr.part_id))
    logging.debug(vars(rr))

    logging.debug("Actuator {} for ramp {} has the following parameters:".format(
        rr.actuator.part_id, rr.part_id))

    act_forces, act_lengths = ramp_requirements_t31(rr)
    force_ok, length_ok = passes_tests(rr, act_forces, act_lengths)
    if not (force_ok and length_ok):
        all_tests_ok = False

    all_results["Metrics"][rr.part_id] = {"Max act. force": max(act_forces),
                                          "Max act. length": max(act_lengths),
                                          "Act force req met": force_ok,
                                          "Act length req met": length_ok}


    if SETTINGS['show_2d']:
        ## Load in geometry for plotting
        #geo = {}
        #for t in ramp_types:
        #    geo.update(tba.load_geometry({t}))

        # Plot 2d line graphs of results
        plt_ramp_results(rr, act_forces, act_lengths)
        plt_ramp(rr)

        # Show the 5 hatch mount points + center position on the plot
        #this_geom = geo[rr.part_id]
        #plt_ramp(rr, this_geom)

        ## Show all plots when done
        #plt.show()

    # Update all results dict with overall info about run
    all_results["RequirementsMet"] = all_tests_ok
    all_results["TestBench"] = "Closures"
    all_results["DesignName"] = SETTINGS["design_name"]

    tba.write_results(all_results)