"""
Vehicle geometry (CAD) test bench:

Assess what types of transports the design will fit into.

Also evaluates whether lift eyes and tiedowns can support the expected loads.
"""

import _winreg
import sys
import os

from scipy.spatial import ConvexHull
import numpy as np


def query_analysis_tools():
    """
    Use a registry entry to find where the library tools are.
    """
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

## Add the ricardo python library onto the import path
tools = query_analysis_tools()
sys.path.append(os.path.join(tools['transportability']['InstallLocation']))


import logging

import matplotlib

matplotlib.use('Agg')
import matplotlib.pyplot as plt
from matplotlib.tri import Triangulation
import matplotlib.patches as mpatches
from matplotlib import rcParams
from matplotlib.font_manager import FontProperties
import rpl.tools.api.test_bench_api as tba


parts_of_interest = {"Eye_Welded": {"Datum": ["INT_LOAD_LOCATION"],
                                    "PrimitiveProperty": ["load_rating.x",
                                                          "load_rating.y",
                                                          "load_rating.z"],
                                    "Required": "{4}"},
                     "Ring_Hoist": {"Datum": ["INT_LOAD_LOCATION"],
                                    "PrimitiveProperty": ["load_rating.x",
                                                          "load_rating.y",
                                                          "load_rating.z"],
                                    "Required": "{4}"},
                     "Eye_Bolted": {"Datum": ["INT_LOAD_LOCATION"],
                                    "PrimitiveProperty": ["load_rating.x",
                                                          "load_rating.y",
                                                          "load_rating.z"],
                                    "Required": "{*}"},
                     "D_Ring_Lashing": {"Datum": ["INT_LOAD_LOCATION"],
                                        "PrimitiveProperty": ["load_rating.x",
                                                              "load_rating.y",
                                                              "load_rating.z"],
                                        "Required": "{*}"},
                     "Pintle_Tow": {"Datum": ["INT_LOAD_LOCATION"],
                                    "PrimitiveProperty": ["load_rating.x",
                                                          "load_rating.y",
                                                          "load_rating.z"],
                                    "Required": "{*}"},
                     "Cleat_Mooring": {"Datum": ["INT_LOAD_LOCATION"],
                                       "PrimitiveProperty": ["load_rating.x",
                                                             "load_rating.y",
                                                             "load_rating.z"],
                                       "Required": "{*}"},
                     "Drawbar_Bolted": {"Datum": ["INT_LOAD_LOCATION"],
                                        "PrimitiveProperty": ["load_rating.x",
                                                              "load_rating.y",
                                                              "load_rating.z"],
                                        "Required": "{*}"},
                     "Drawbar_Eyebolt": {"Datum": ["INT_LOAD_LOCATION"],
                                         "PrimitiveProperty": ["load_rating.x",
                                                               "load_rating.y",
                                                               "load_rating.z"],
                                         "Required": "{*}"},
                     "Drawbar_Welded": {"Datum": ["INT_LOAD_LOCATION"],
                                        "PrimitiveProperty": ["load_rating.x",
                                                              "load_rating.y",
                                                              "load_rating.z"],
                                        "Required": "{*}"}}


class Vehicle_Metrics(object):
    """ Calculates vehicle metrics. """

    def __init__(self, settings):
        """
        Setup vehicle metrics based on settings read in from file.
        """
        ## Essential test bench settings
        self.output_json_file = settings["output_json_file"]

        ##testbench specific settings
        transport_dimensions = settings["transport_dimensions"]
        self.up_direction = settings.get("up_direction", np.array([0, 1.0, 0]))
        self.fore_direction = settings.get("fore_direction", np.array([0, 0, 1.0]))
        self.ground = settings.get("ground", np.array([0, 1.0, 0]))
        self.curb_mass = tba.get_veh_mass()
        self.cent_grav = tba.get_veh_cg()

        try:
            self.up_direction, self.ground_plane, _ = tba.get_up_vector()
        except:
            self.up_direction = settings.get("up_direction", np.array([0, 1.0, 0]))

        trans_data = tba.get_data(parts_of_interest)

        ## Define dictionaries for each lifting and tie down class
        self.eye_weld = {}
        self.eye_bolt = {}
        self.ring_hoist = {}
        self.d_ring = {}
        self.dbar_weld = {}
        self.dbar_bolt = {}
        self.dbar_eye = {}
        self.pintle = {}
        self.cleat = {}

        self.eye_weld = trans_data["Eye_Welded"]  # TODO: does this crash on empty list?
        self.eye_bolt = trans_data["Eye_Bolted"]
        self.ring_hoist = trans_data["Ring_Hoist"]
        self.d_ring = trans_data["D_Ring_Lashing"]
        self.pintle = trans_data["Pintle_Tow"]
        self.cleat = trans_data["Cleat_Mooring"]
        self.dbar_weld = trans_data["Drawbar_Welded"]
        self.dbar_bolt = trans_data["Drawbar_Bolted"]
        self.dbar_eye = trans_data["Drawbar_Eyebolt"]

        ## Define set of classes to get geometry for
        all_class_set = tba.get_all_geom_set()
        ## Class sets and geometry for approach angles
        class_set = all_class_set - tba.geom_sets["never_exterior_classes"] - \
                    {"Roadwheel",
                     "Trailing_Arm_Hydropneumatic_Rh",
                     "Trailing_Arm_Hydropneumatic_Lh",
                     "Track",
                     "Sprocket_And_Carrier_Drive",
                     "Wheel_Idler"}
        surface = tba.load_geometry(class_set, single_file=True)

        self.surface_coords = np.vstack([surface["x"], surface["y"], surface["z"]]).T

        wheel_set = {"Roadwheel", "Sprocket_And_Carrier_Drive", "Wheel_Idler"}
        wheels = tba.load_geometry(wheel_set, single_file=True)

        track_set = {"Track"}
        track = tba.load_geometry(track_set, single_file=True)

        ## Class set and geometry for container fitting
        trans_class_set = all_class_set
        trans_geom = tba.load_geometry(trans_class_set, single_file=True)

        ## Class sets and geometry for lift eyes and tie_downs
        lift_set = {"Eye_Welded"}
        tie_down_set = {"Ring_Hoist"}

        lift_geom = tba.load_geometry(lift_set, single_file=True)
        tie_down_geom = tba.load_geometry(tie_down_set, single_file=True)

        self.trans_dict = vehicle_fit(surface,
                                      self.up_direction,
                                      self.fore_direction,
                                      transport_dimensions)

        self.trans_dict["Lifting_Metrics"] = self.lifting_metrics()
        self.trans_dict["Tie_Down_Metrics"] = self.tie_down_metrics()

        self.trans_dict["Approach_Angles"] = approach_angles(surface, wheels, track)

        pintle_count = len(self.pintle)

        cleat_count = len(self.cleat)

        tow_count = len(self.eye_bolt) + len(self.eye_weld) + len(self.dbar_bolt) + \
                    len(self.dbar_eye) + len(self.dbar_weld) + len(self.pintle)
        lift_count = len(self.eye_bolt) + len(self.eye_weld)

        self.trans_dict["Count_for_components_in_towing_class"] = tow_count
        self.trans_dict["Count_for_components_in_lifting_class"] = lift_count
        self.trans_dict["Count_for_components_in_pintle_class"] = pintle_count
        self.trans_dict["Count_for_components_in_mooring_class"] = cleat_count

        plot_vehicle(trans_geom, transport_dimensions)

        plot_lift_tie_down(trans_geom, lift_geom, tie_down_geom)

        tba.write_results(self.trans_dict)

    def lifting_metrics(self):
        """calculate loads and angles while lifting"""

        gross_wt = np.float(self.curb_mass) * 9.8
        ## create a list of the coords for the center of gravity
        cent_grav = self.cent_grav

        lift_eye = []
        load_ratings = []
        lift_metrics = {}

        ## Lift_eye instances
        eye_insts = []
        for lift in self.eye_weld:
            eye_insts.append(lift)

        ## Create a list of the lift eyes' locations
        for eye, eye_props in self.eye_weld.items():
            lift_eye.append(eye_props["datums"]["INT_LOAD_LOCATION"]["global"][:3, 3])
            try:
                load_ratings.append([float(eye_props["properties"]["load_rating.x"]),
                                     float(eye_props["properties"]["load_rating.y"]),
                                     float(eye_props["properties"]["load_rating.z"])])
            except ValueError as err:
                msg = "Primitive load ratings for {} are not numbers.".format(eye)
                msg += "Must have numbers to compute lifting metrics. Error {}".format(err)
                logging.error(msg)
                lift_metrics[eye] = "Failed: no_load_ratings"

        ### Get arrays for lat, long and height for center of grav and lift eyes
        side_direction = np.cross(self.up_direction, self.fore_direction)

        cent_grav_long = np.dot(cent_grav, self.fore_direction)
        cent_grav_lat = np.dot(side_direction, cent_grav)
        cent_grav_height = np.dot(self.up_direction, cent_grav)

        lift_eye_long = np.dot(lift_eye, self.fore_direction)
        lift_eye_lat = np.dot(lift_eye, side_direction)
        lift_eye_height = np.dot(lift_eye, self.up_direction)

        long_dist = list(abs(lift_eye_long - cent_grav_long))
        lat_dist = list(abs(lift_eye_lat - cent_grav_lat))
        height = list(abs(lift_eye_height - cent_grav_height))

        long_dist.sort()
        lat_dist.sort()
        height.sort()

        long_dist_fr = (long_dist[0] + long_dist[1]) / 2.0
        long_dist_r = (long_dist[2] + long_dist[3]) / 2.0
        height_fr = (height[0] + height[1]) / 2.0
        height_r = (height[2] + height[3]) / 2.0

        ## Determine b, the angle of the plane of the provisions with respect to the
        ##    horizontal, and long_xy.
        delta = abs(height_r - height_fr)
        long_dist_fr2r = long_dist_fr + long_dist_r
        beta = np.arctan(float(delta) / long_dist_fr2r)
        long_xy = float(long_dist_fr2r) / np.cos(beta)

        ## Determine horiz_dist_level and self.sling_len (these are constant for all slings)
        lat_dist_ab = (lat_dist[0] + lat_dist[1]) / 2.0
        lat_dist_cd = (lat_dist[2] + lat_dist[3]) / 2.0
        Ly = (np.power(lat_dist_ab, 2) - np.power(lat_dist_cd, 2) +
              np.power(long_xy, 2)) / (2 * long_xy)

        Lx = long_xy - Ly
        horiz_dist_level = np.sqrt(np.power(lat_dist_ab, 2) + np.power(Lx, 2))

        ## self.sling_len is set to 45 degrees to determine the sling length for a
        ##   single apex sling assembly.
        sling_ang_vert = 45. * (np.pi / 180.)
        sling_len = horiz_dist_level / np.cos(sling_ang_vert)

        ## If sling_len is shorter than 3.568 meters(12 ft), the sling length for an equal
        ##  length single apex sling assembly is set to 3.568 meters. This is most
        ##  likely the shortest size of slings that will be available in the field to lift an item.
        if sling_len < 3.568:
            sling_len = 3.568

        horiz_plane_dist = np.power((np.square([long_dist_fr, long_dist_fr,
                                                long_dist_r, long_dist_r]) +
                                     np.square(lat_dist)), 0.5)

        horiz_dist_lift = horiz_plane_dist / np.cos(beta)

        hl_sq = np.square(horiz_dist_lift)  # matrix with all elements of horiz_dist_lift squared
        sling_len_sq = np.power(sling_len, 2)  # matrix of the sling length squared
        k = np.sqrt(sling_len_sq + hl_sq[0] - 2 * sling_len * horiz_dist_lift[0] *
                    np.cos(sling_len))

        ## Determine vert_ang, the angle of the slings with the vertical when the
        ## equipment is lifted.

        k_sq = np.power(k, 2)
        vert_ang = [np.arccos((sling_len_sq + k_sq - h_sq) / (2 * sling_len * k)) for h_sq in hl_sq]

        vert_force = []

        denom = long_dist_r + long_dist_fr
        lat_dist_denom = [(lat_dist[0] + lat_dist[1]), (lat_dist[2] + lat_dist[3])]

        vert_force.append(((long_dist_r / denom) * (lat_dist[1] / lat_dist_denom[0])) * gross_wt)
        vert_force.append(((long_dist_r / denom) * (lat_dist[0] / lat_dist_denom[0])) * gross_wt)
        vert_force.append(((long_dist_fr / denom) * (lat_dist[3] / lat_dist_denom[1])) * gross_wt)
        vert_force.append(((long_dist_fr / denom) * (lat_dist[2] / lat_dist_denom[1])) * gross_wt)

        vert_force = np.array(vert_force)

        ## Determine x,y,z components of resulting forces on eyes.
        ## For eye A, Va_sub is the force component on the horizontal
        ## plane along ha, alpha is the angle between ha and long_dist_fr
        lifting_load = np.ones((4, 3))

        ## eye a
        lifting_load[0][0] = vert_force[0] * np.tan(vert_ang[0]) * np.sin(
            np.arccos(long_dist_fr / horiz_plane_dist[0]))
        lifting_load[0][1] = vert_force[0] * np.tan(vert_ang[0]) * long_dist_fr / horiz_plane_dist[
            0]
        lifting_load[0][2] = vert_force[0]

        ## eye b
        lifting_load[1][0] = vert_force[1] * np.tan(vert_ang[0]) * np.sin(
            np.pi + np.arccos(long_dist_fr / horiz_plane_dist[1]))
        lifting_load[1][1] = vert_force[1] * np.tan(vert_ang[0]) * long_dist_fr / horiz_plane_dist[
            1]
        lifting_load[1][2] = vert_force[1]

        ## and for eye C, Vc_sub is the force component on the horizontal plane
        ##along hc, rho is the angle between hc and long_dist_r.
        ## eye c
        lifting_load[2][0] = vert_force[2] * np.tan(vert_ang[2]) * np.sin(
            np.pi + np.arctan(lat_dist[2] / long_dist_r))
        lifting_load[2][1] = vert_force[2] * np.tan(vert_ang[2]) * np.cos(
            np.pi + np.arctan(lat_dist[2] / long_dist_r))
        lifting_load[2][2] = vert_force[2]

        ## and for eye d
        lifting_load[3][0] = vert_force[3] * np.tan(vert_ang[3]) * np.sin(
            np.arctan(lat_dist[3] / long_dist_r))
        lifting_load[3][1] = vert_force[3] * np.tan(vert_ang[3]) * np.cos(
            np.pi + np.arctan(lat_dist[3] / long_dist_r))
        lifting_load[3][2] = vert_force[3]

        lifting_load_newtons = abs(lifting_load)

        for eye_num, load in enumerate(load_ratings):
            lift_key = eye_insts[eye_num]
            load_msg = " Calculated loads: x: {0:.2f}, y: {1:.2f}, z: {2:.2f}. ".format(
                lifting_load_newtons[eye_num][0], lifting_load_newtons[eye_num][1],
                lifting_load_newtons[eye_num][2])

            load_msg += "load ratings: x: {0:.2f}, y: {1:.2f}, z: {2:.2f}.".format(
                load[0], load[1], load[2])

            if np.any(lifting_load_newtons[eye_num] > load):
                lift_metrics[lift_key] = "Fail"
                msg = "The calculated loads for lift eye '{}', exceed the \
                        load_ratings.".format(lift_key)
                msg += load_msg
                logging.warning(msg)
            else:
                msg = "The calculated loads for lift eye '{}' meet the \
                        load_ratings.".format(lift_key)
                msg += load_msg
                logging.info(msg)
                lift_metrics[lift_key] = "Pass"

        return lift_metrics

    def tie_down_metrics(self):
        """calculate loads for tie down"""

        gross_wt = np.float(self.curb_mass) * 9.8
        ## create a list of the coords for the center of gravity
        cent_grav = self.cent_grav

        tie_down_list = []
        load_ratings = []
        tie_down_metrics = {}

        tie_down_instances = []
        for tie in self.ring_hoist:
            tie_down_instances.append(tie)

        ## Create a list of the lift eyes' locations
        for inst_name, inst_data in self.ring_hoist.iteritems():
            tie_down_list.append(inst_data["datums"]["INT_LOAD_LOCATION"]["global"][:3, 3])
            try:
                load_ratings.append([float(inst_data["properties"]["load_rating.x"]),
                                     float(inst_data["properties"]["load_rating.y"]),
                                     float(inst_data["properties"]["load_rating.z"])])
            except ValueError as err:
                msg = "Load ratings for {} are not numeric. ".format(inst_name)
                msg += "Must have numbers to compute lifting metrics. Error {}".format(err)
                logging.error(msg)
                tie_down_metrics[inst_name] = "Failed: no_load_ratings"

        load_ratings = np.vstack(load_ratings)
        tie_down_list = np.vstack(tie_down_list)

        ### Get arrays for lat, long and height for center of grav and lift eyes
        side_direction = np.cross(self.up_direction, self.fore_direction)

        cent_grav_long = np.dot(cent_grav, self.fore_direction)
        cent_grav_lat = np.dot(side_direction, cent_grav)

        tie_long = np.dot(tie_down_list, self.fore_direction)
        tie_lat = np.dot(tie_down_list, side_direction)

        lat_dist = np.array(abs(cent_grav_lat - tie_lat))
        lon_dist = np.array(abs(cent_grav_long - tie_long))

        #The longitudinal inertia force acting through the CG
        lon_force = 4. * gross_wt

        # Aft: design_load[2] & design_load[3], Fore: design_load[0] & design_load[1]
        dl_1 = lat_dist * lon_force

        design_lon_load = np.array([dl_1[1] / (lat_dist[0] + lat_dist[1]),
                                    dl_1[0] / (lat_dist[0] + lat_dist[1]),
                                    dl_1[3] / (lat_dist[2] + lat_dist[3]),
                                    dl_1[2] / (lat_dist[2] + lat_dist[3])],
                                   dtype=np.float)

        vert_force = gross_wt  # The vertical inertia force acting through the CG

        # Design load limit in vertical
        design_vert_load = np.array([(lat_dist[1] / (lat_dist[0] + lat_dist[1])) *
                                     (lon_dist[2] / (lon_dist[0] + lon_dist[2])) * vert_force,
                                     (lat_dist[0] / (lat_dist[0] + lat_dist[1])) *
                                     (lon_dist[3] / (lon_dist[1] + lon_dist[3])) * vert_force,
                                     (lat_dist[3] / (lat_dist[2] + lat_dist[3])) *
                                     (lon_dist[0] / (lon_dist[0] + lon_dist[2])) * vert_force,
                                     (lat_dist[2] / (lat_dist[2] + lat_dist[3])) *
                                     (lon_dist[1] / (lon_dist[1] + lon_dist[3])) * vert_force],
                                    dtype=np.float)

        lat_force = 1.5 * gross_wt  ## The lateral inertia force acting through the CG

        # Design load limit in lateral
        design_lat_load = np.array([lon_dist[2] * lat_force / (lon_dist[0] + lon_dist[2]),
                                    lon_dist[3] * lat_force / (lon_dist[1] + lon_dist[3]),
                                    lon_dist[0] * lat_force / (lon_dist[0] + lon_dist[2]),
                                    lon_dist[1] * lat_force / (lon_dist[1] + lon_dist[3])])

        lat_load_ratings = np.dot(load_ratings, side_direction)
        lon_load_ratings = np.dot(load_ratings, self.fore_direction)
        vert_load_ratings = np.dot(load_ratings, self.up_direction)

        vert_test = vert_load_ratings - design_vert_load
        lat_test = lat_load_ratings - design_lat_load
        long_test = lon_load_ratings - design_lon_load

        test_case = np.vstack([long_test, lat_test, vert_test]).T

        for test_num, test in enumerate(test_case):
            tie_key = tie_down_instances[test_num]
            load_msg = "Calculated loads: lat: {}, long: {}, vert: {}".format(
                design_lat_load[test_num],
                design_lon_load[test_num],
                design_vert_load[test_num])
            load_msg += " Load ratings: lat: {}, long: {}, vert: {}".format(
                lat_load_ratings[test_num],
                lon_load_ratings[test_num],
                vert_load_ratings[test_num])
            if np.any(test < 0):
                tie_down_metrics[tie_key] = "Fail"
                msg = "The calculated load for tie_down {} exceeded the load rating.".format(
                    tie_key)
                msg += load_msg
                logging.warning(msg)
            else:
                msg = "The calculated load for tie_down {} are within the load rating.".format(
                    tie_key)
                msg += load_msg
                logging.info(msg)
                tie_down_metrics[tie_key] = "Pass"

        return tie_down_metrics


def vehicle_fit(surface, up_direction, fore_direction, transport_dimensions):
    """
    Determines if the vehicle can fit into specified dimensions for transport.
    """
    ## Get the vehicle extent in the 3 cartesian directions
    ## determine orientation of the vehicle given the direction up and forward
    coord_array = np.vstack((surface['x'], surface['y'], surface['z'])).T
    side_direction = np.cross(up_direction, fore_direction)

    width = np.ptp(np.dot(coord_array, side_direction))
    height = np.ptp(np.dot(coord_array, up_direction))
    length = np.ptp(np.dot(coord_array, fore_direction))
    #print width, height, length

    ## Store the calculated vehicle dimensions for info only (not an output metric)
    results = {"_vehicle_calculated_dimension": {"vehicle_length[m]": length,
                                                 "vehicle_width[m]": width,
                                                 "vehicle_height[m]": height}}

    ## Check each transport option in turn and write True for any that can fit the vehicle
    trans_compat = results["Transportation_Compatibility"] = {}
    for transport, size in transport_dimensions.items():
        if size["max_length"] < length or size["max_width"] < width or size["max_height"] < height:
            trans_compat[transport] = False
        else:
            trans_compat[transport] = True

    return results


def plot_vehicle(surface, transport_dimensions):
    """
    Plot vehicle geometry silhouette along with transport dimensions as a visual check.
    """
    logging.info("Plotting vehicle with transport outline boxes")

    plt.subplot(2, 2, 4)
    plt.gca().set_aspect('equal', anchor="NW")
    plt.gca().grid(True)
    plt.title("Side")
    plt.gca().axes.get_yaxis().set_visible(False)

    start_point = [np.min(surface['z']), np.min(surface['y'])]
    plot_transport_dimensions(start_point, transport_dimensions, "max_length", "max_height")

    ## Plot Vehicle
    triangulation = Triangulation(surface['z'], surface['y'], surface['tris'])
    plt.tricontourf(triangulation, triangulation.x * 0.0, colors="k", alpha=0.6)

    ##Top View
    plt.subplot(2, 2, 2)
    plt.gca().set_aspect('equal', anchor="SW")
    plt.gca().grid(True)
    plt.title("Top View")
    plt.gca().axes.get_yaxis().set_visible(False)

    start_point = [np.min(surface['z']), np.min(surface['x'])]
    plot_transport_dimensions(start_point, transport_dimensions, "max_length", "max_width")

    ## Plot Vehicle
    triangulation = Triangulation(surface['z'], surface['x'], surface['tris'])
    plt.tricontourf(triangulation, triangulation.x * 0.0, colors="k", alpha=0.6)

    ## Front View
    plt.subplot(2, 2, 3)
    plt.gca().set_aspect('equal')
    plt.gca().grid(True)
    plt.title("Front View")

    start_point = [np.min(surface['x']), np.min(surface['y'])]
    plot_transport_dimensions(start_point, transport_dimensions, "max_width", "max_height")

    ## Plot Vehicle
    triangulation = Triangulation(surface['x'], surface['y'], surface['tris'])
    plt.tricontourf(triangulation, triangulation.x * 0.0, colors="k", alpha=0.6)

    plt.tight_layout()

    fontP = FontProperties()
    fontP.set_size('small')
    plt.legend(bbox_to_anchor=(0.05, 1.4), loc=3, borderaxespad=0.0, prop=fontP)

    img_file_name = "Transportability_Container_fitting.png"
    current_dir = os.getcwd()
    logging.info("Plotting Vehicle: writing image to file {} in {}.".format(
        img_file_name, current_dir))
    plt.suptitle("Transportability Container fitting", horizontalalignment='right',
                 verticalalignment='top')
    plt.savefig(img_file_name, dpi=100)

    plt.clf()


def plot_lift_tie_down(surface, lift_geom, tie_down_geom):
    """
    Plot vehicle geometry silhouette along with lift eye and tie down locations.
    """
    logging.info("Plotting vehicle with lift eye and tie down locations")

    try:
        ## Side view
        plt.subplot(2, 2, 4)
        plt.gca().set_aspect('equal', anchor="NW")
        plt.gca().grid(True)
        plt.title("Side")
        plt.gca().axes.get_yaxis().set_visible(False)

        ## Plot Vehicle
        triangulation = Triangulation(surface['z'], surface['y'], surface['tris'])
        plt.tricontourf(triangulation, triangulation.x * 0.0, colors="k", alpha=0.3)

        ## Plot lift eyes
        lift_triangulation = Triangulation(lift_geom['z'], lift_geom['y'], lift_geom['tris'])
        plt.tricontourf(lift_triangulation, lift_triangulation.x * 0.0, colors="g", alpha=0.6)

        ## Plot tie downs
        tie_triangulation = Triangulation(tie_down_geom['z'], tie_down_geom['y'],
                                          tie_down_geom['tris'])
        plt.tricontourf(tie_triangulation, tie_triangulation.x * 0.0, colors="b", alpha=0.6)

        ##Top View
        plt.subplot(2, 2, 2)
        plt.gca().set_aspect('equal', anchor="SW")
        plt.gca().grid(True)
        plt.title("Top View")
        plt.gca().axes.get_yaxis().set_visible(False)

        ## Plot Vehicle
        triangulation = Triangulation(surface['z'], surface['x'], surface['tris'])
        plt.tricontourf(triangulation, triangulation.x * 0.0, colors="k", alpha=0.3)

        ## Plot lift eyes
        lift_triangulation = Triangulation(lift_geom['z'], lift_geom['x'], lift_geom['tris'])
        plt.tricontourf(lift_triangulation, lift_triangulation.x * 0.0, colors="g", alpha=0.6)

        ## Plot tie downs
        tie_triangulation = Triangulation(tie_down_geom['z'], tie_down_geom['x'],
                                          tie_down_geom['tris'])
        plt.tricontourf(tie_triangulation, tie_triangulation.x * 0.0, colors="b", alpha=0.6)

        ## Front View
        plt.subplot(2, 2, 3)
        plt.gca().set_aspect('equal')
        plt.gca().grid(True)
        plt.title("Front View")

        ## Plot Vehicle
        triangulation = Triangulation(surface['x'], surface['y'], surface['tris'])
        plt.tricontourf(triangulation, triangulation.x * 0.0, colors="k", alpha=0.3)

        ## Plot lift eyes
        lift_triangulation = Triangulation(lift_geom['x'], lift_geom['y'], lift_geom['tris'])
        plt.tricontourf(lift_triangulation, lift_triangulation.x * 0.0, colors="g", alpha=0.6)

        ## Plot tie downs
        tie_triangulation = Triangulation(tie_down_geom['x'], tie_down_geom['y'],
                                          tie_down_geom['tris'])
        plt.tricontourf(tie_triangulation, tie_triangulation.x * 0.0, colors="b", alpha=0.6)

        plt.tight_layout()

        font_prop = FontProperties()
        font_prop.set_size('small')
        plt.legend(bbox_to_anchor=(0.05, 1.4), loc=3, borderaxespad=0.0, prop=font_prop)

        img_file_name = "Lift_eye_tie_down_locations.png"
        current_dir = os.getcwd()
        logging.info("Plotting Vehicle: writing image to file {} in {}.".format(
            img_file_name, current_dir))
        plt.suptitle("Lift eye and tie down locations", horizontalalignment='right',
                     verticalalignment='top')
        plt.savefig(img_file_name, dpi=100)

        plt.clf()
    except:
        logging.warning("Could not plot vehicle with lift eye and tie down locations")


def plot_transport_dimensions(start_point, transport_dimensions, dim1, dim2):
    """
    Plots the bounding boxes for the different types of transport containers.
    """
    colors = ["b", "g", "r", "c", "m", "y", "#FFCC00"]
    color_index = 0
    for transport, dims in transport_dimensions.items():
        rect_label = str(transport)
        rec = mpatches.Rectangle(start_point,
                                 dims[dim1],
                                 dims[dim2],
                                 alpha=0.3,
                                 edgecolor=colors[color_index],
                                 facecolor="none",
                                 linewidth=2.0,
                                 label=rect_label)
        plt.gca().add_patch(rec)
        color_index += 1

    box = plt.gca().get_position()
    plt.gca().set_position([box.x0, box.y0, box.width * 0.7, box.height])
    font_prop = FontProperties()
    font_prop.set_size('small')


def approach_angles(surface, wheel_geom, track_geom):
    """
    Calculates approach angles
    """
    logging.info("Calculating approach angles")
    approach = {}

    x = surface['z']
    y = surface['y']

    tris = surface['tris']

    wheel_points = np.array((wheel_geom["z"], wheel_geom["y"])).T

    points = np.array((x, y))
    points = points.T

    body_points = points.shape[0]

    points = np.vstack((points, wheel_points))

    hull = ConvexHull(points)

    ## turn track into a convex hull
    track_points = np.vstack([track_geom["z"], track_geom["y"]]).T

    try:
        track_outline = ConvexHull(track_points)
    except ValueError as err:
        logging.error(
            "Could not not convert the track into a convex hull. Error description {}".format(err))
        track_outline = 0
    point_on_hull = set()

    approaches = []
    under_wheels = []
    body_lines = []
    approach_lines = []
    for simp in hull.simplices:
        point_on_hull.add(simp[0])
        point_on_hull.add(simp[1])

        if simp[0] >= body_points and simp[1] >= body_points:
            under_wheels.append([[points[simp[0], 0], points[simp[1], 0]],
                                 [points[simp[0], 1], points[simp[1], 1]], ])

        elif simp[0] >= body_points or simp[1] >= body_points:
            approach_lines.append([[points[simp[0], 0], points[simp[1], 0]],
                                   [points[simp[0], 1], points[simp[1], 1]]])

            approaches.append(simp)

        else:
            body_lines.append([[points[simp[0], 0], points[simp[1], 0]],
                               [points[simp[0], 1], points[simp[1], 1]]])

    ## Calculate approach angles and ground clearance.
    if len(approaches) == 2:
        ## Determine which approach is fore and which is aft
        min_0 = min(hull.points[approaches[0][0], 0], hull.points[approaches[0][1], 0])
        min_1 = min(hull.points[approaches[0][1], 0], hull.points[approaches[1][1], 0])
        if min_0 < min_1:
            front = approaches[0]
            aft = approaches[1]
        else:
            front = approaches[1]
            aft = approaches[0]

        front_ang = np.arctan(abs(hull.points[front[0], 1] - hull.points[front[1], 1])
                              / abs(hull.points[front[0], 0] - hull.points[front[1], 0]))
        approach["Front_approach_angle_[deg]"] = front_ang * (180. / 3.14159)
        aft_ang = np.arctan(abs(hull.points[aft[0], 1] - hull.points[aft[1], 1]) /
                            abs(hull.points[aft[0], 0] - hull.points[aft[1], 0]))
        approach["Aft_approach_angle_[deg]"] = aft_ang * (180. / 3.14159)
        ground_clearance = min(y) - min(points[:, 1])
        approach["Ground_clearance_[m]"] = ground_clearance

    plot_approach_angles(x, y, tris, under_wheels, body_lines, approach_lines,
                         track_points, track_outline, wheel_geom)
    return approach


def plot_approach_angles(x, y, tris, under_wheels, body_lines, approach_lines,
                         track_points, track_outline, wheel_geom):
    """
    Plot approach angles and a bounding box around the vehicle.
    """
    plt.subplot2grid((3, 2), (0, 0))
    plt.subplot(1, 1, 1)
    rcParams['figure.figsize'] = 50, 50
    plt.gca().set_aspect('equal')
    plt.gca().grid(True)

    ## Plot body of the vehicle
    triangulation = Triangulation(x, y, tris)
    plt.triplot(triangulation, linewidth=0.5, alpha=0.7)

    ## Plot wheels
    if wheel_geom:
        wheel_triangulation = Triangulation(wheel_geom["z"], wheel_geom["y"],
                                            wheel_geom["tris"])
        plt.triplot(wheel_triangulation, linewidth=0.5, alpha=0.7)

    ## Plot Tracks
    if track_outline != 0:
        plot_tracks(track_points, track_outline)

    ## plot under_wheel lines
    for line in under_wheels:
        plt.plot(line[0], line[1], "m", linewidth=4, alpha=0.2)

    ## plot approach_lines
    for a_line in approach_lines:
        plt.plot(a_line[0], a_line[1], "g", linewidth=4, alpha=0.2)

    ## plot body lines
    for b_line in body_lines:
        plt.plot(b_line[0], b_line[1], "r", linewidth=4, alpha=0.2)

    plt.suptitle("Approach angles", horizontalalignment='right', verticalalignment='top')

    img_file_name = "Approach_angles.png"
    current_dir = os.getcwd()
    logging.info("Plotting approach angles: writing image to file {} in {}.".format(
        img_file_name, current_dir))

    plt.savefig(img_file_name, dpi=100)

    logging.info("Finished plotting approach angles")

    plt.clf()


def plot_tracks(track_points, track_outline):
    """
    plots the convex hull created by the track geometry
    """
    for simplex in track_outline.simplices:
        plt.plot(track_points[simplex, 0], track_points[simplex, 1], 'k-', linewidth=0.5, alpha=0.7)


if __name__ == "__main__":
    ## Executes the transportability test bench.

    logging.info("\n" + 50 * "_" + "\ntest bench started\n" + 50 * "_")

    settings_file = r"settings.js"
    settings_dict = tba.load_settings(settings_file)
    vehicle_metrics = Vehicle_Metrics(settings_dict)
    logging.info("Test bench completed\n" + 50 * "_")