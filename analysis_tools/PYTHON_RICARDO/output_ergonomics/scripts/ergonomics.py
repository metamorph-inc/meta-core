"""Perform ergonomics tests on assembly """

import logging, operator, os, sys, _winreg

import numpy as np
import matplotlib.pyplot as plt
from matplotlib.tri import Triangulation
from matplotlib.patches import Wedge
from matplotlib.ticker import NullFormatter


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
sys.path.append(os.path.join(tools['ergonomics']['InstallLocation']))

from rpl.tools.ray_tracing.bsp_tree_poly import BSP_Tree, BSP_Element
from rpl.tools.geometry.geom_utils import mul, normalize
import rpl.tools.api.test_bench_api as tba

parts_of_interest = {"Manikin": {"Datum": ["EXT_GRASP_ZONE", "EXT_TOGGLE-BUTTON_ZONE",
                                           "EXT_PUSH-BUTTON_ZONE", "BOOT_LEFT_CSYS",
                                           "BOOT_RIGHT_CSYS", 'EXT_EYE_CSYS_LEFT',
                                           'EXT_EYE_CSYS_RIGHT'],
                                 "Property": ["vehicle_role"],
                                 "Required": "{*}"},
                     "Steering_Wheel": {"Datum": ["EXT_REACH_OBJ_GRASP"],
                                        "Required": "{+}"},
                     "Gear_Select_Panel": {"Datum": ["EXT_REACH_OBJ_PUSH"],
                                           "Required": "{+}"},
                     "Pedal_Accelerator_Electric": {"Datum": ["EXT_FOOT_POINT_PEDAL"],
                                                    "Required": "{*}"},
                     "Pedal_Accelerator_Mechanical": {"Datum": ["EXT_FOOT_POINT_PEDAL"],
                                                      "Required": "{*}"},
                     "Seat_Crew": {"Datum": ["CUSHION_FRONT_CSYS", "SRP_1472"],
                                   "Required": "{*}"},
                     "Seat_Troop": {"Datum": ["CUSHION_FRONT_CSYS", "SRP_1472"],
                                    "Required": "{*}"},
                     # The bench datums are for the 3-soldier instance in library; lack of
                     # arbitrary querying is an API limit. 2 or 4 soldier benches won't work
                     "Seat_Troop_Multi_Occupant": {"Datum": ["CUSHION_FRONT_CSYS_1", "SRP_1472_1",
                                                             "CUSHION_FRONT_CSYS_2", "SRP_1472_2",
                                                             "CUSHION_FRONT_CSYS_3", "SRP_1472_3"],
                                                   "Required": "{*}"},
                     "Pedal_Brake_Hydraulic": {"Datum": ["EXT_FOOT_POINT_PEDAL"],
                                               "Required": "{*}"},
                     "Pedal_Brake_Electric": {"Datum": ["EXT_FOOT_POINT_PEDAL"],
                                              "Required": "{*}"}}

ALL_IF = lambda res: all(res) if res else False  # Return false for empty list of manis or results


def merge_res(res_list):
    """
    Utility function to process ergo results: for a list of result tuples [(bool, dist_dict)],
        merge to product a single result dict and a single result list.

        Used to combine the results from multiple sub-metrics of MIL_STD 1472
    """
    res_bool, res_dict = [], {}
    for r in res_list:
        res_bool.append(r[0])
        res_dict.update(r[1])
    return ALL_IF(res_bool), res_dict


class Manikin(object):
    """ A person inside the vehicle."""

    def __init__(self, manikin, inst):
        """
        Setup the manikin properties using data from the supplied xml file.
        """

        self.name = manikin["name"]
        self.inst = inst
        self.trans_origin = manikin["trans"]
        self.body_rot = np.eye(4)

        ## Reach zones
        self.grasp = manikin["datums"]["EXT_GRASP_ZONE"]["local"]
        self.push = manikin["datums"]["EXT_PUSH-BUTTON_ZONE"]["local"]
        self.toggle = manikin["datums"]["EXT_TOGGLE-BUTTON_ZONE"]["local"]
        self.boot_left = manikin["datums"]["BOOT_LEFT_CSYS"]["local"]
        self.boot_right = manikin["datums"]["BOOT_RIGHT_CSYS"]["local"]
        self.boot_left_veh = manikin["datums"]["BOOT_LEFT_CSYS"]["global"]
        self.boot_right_veh = manikin["datums"]["BOOT_RIGHT_CSYS"]["global"]

        ## Vision properties
        self.left_eye = manikin["datums"]["EXT_EYE_CSYS_LEFT"]["local"]
        self.right_eye = manikin["datums"]["EXT_EYE_CSYS_RIGHT"]["local"]

        ## What kind of manikin is it?
        self.role = manikin["properties"]["vehicle_role"]

        ## Get transform to eyes in vehicle space
        self.left_eye_veh = mul(self.trans_origin, self.left_eye, self.body_rot)
        self.right_eye_veh = mul(self.trans_origin, self.right_eye, self.body_rot)

        self.left_eye_veh_inv = np.linalg.inv(self.left_eye_veh)
        self.right_eye_veh_inv = np.linalg.inv(self.right_eye_veh)

        ## Axis to rotate around to scan horizontally from eye point (in eye csys)
        self.eyes_hor_axis = np.array([0, 1, 0, 1])

        ## This list will store the vision devices this manikin can interact with
        self.vision_devices = []

        ## Determine left and right reach zones
        self.grasp_left = self.grasp.copy()
        self.grasp_left[1, 3] += 0.25

        self.grasp_left_veh = mul(self.trans_origin, self.grasp_left, self.body_rot)
        self.grasp_left_veh_inv = np.linalg.inv(self.grasp_left_veh)

        self.grasp_right = self.grasp.copy()
        self.grasp_right[1, 3] -= 0.25

        self.grasp_right_veh = mul(self.trans_origin, self.grasp_right, self.body_rot)
        self.grasp_right_veh_inv = np.linalg.inv(self.grasp_right_veh)

        self.push_left = self.push.copy()
        self.push_left[1, 3] += 0.25

        self.push_left_veh = mul(self.trans_origin, self.push_left, self.body_rot)
        self.push_left_veh_inv = np.linalg.inv(self.push_left_veh)

        self.push_right = self.push.copy()
        self.push_right[1, 3] -= 0.25

        self.push_right_veh = mul(self.trans_origin, self.push_right, self.body_rot)
        self.push_right_veh_inv = np.linalg.inv(self.push_right_veh)

        self.toggle_left = self.toggle.copy()
        self.toggle_left[1, 3] += 0.25

        self.toggle_left_veh = mul(self.trans_origin, self.toggle_left, self.body_rot)
        self.toggle_left_veh_inv = np.linalg.inv(self.toggle_left_veh)

        self.toggle_right = self.toggle.copy()
        self.toggle_right[1, 3] -= 0.25

        self.toggle_right_veh = mul(self.trans_origin, self.toggle_right, self.body_rot)
        self.toggle_right_veh_inv = np.linalg.inv(self.toggle_right_veh)

        self.boot_left_veh_inv = np.linalg.inv(self.boot_left_veh)
        self.boot_right_veh_inv = np.linalg.inv(self.boot_right_veh)

        self.ergo_devices = []  # Store the devices this manikin can interact with
        self.mil_std_distances = {}  # Store MIL-STD distances for a given manikin

        # Store assigned seat (and info about trans matrices for specific seat slot- eg on benches)
        self.seat = None
        self.seat_front = None
        self.seat_front_inv = None
        self.seat_back = None
        self.seat_back_inv = None


class Human_Interface_Device(object):
    """ A device that can be used by the occupant(s). """

    def __init__(self, dev, new_device):
        """
        Setup the device using data from supplied by the api.
        """
        self.class_name = dev
        for dev_inst in new_device:
            self.name = new_device[dev_inst]["name"]
            self.dev_trans_origin = new_device[dev_inst]["trans"]
            datum_prop = new_device[dev_inst]["datums"].keys()[0]
            if "PUSH" in datum_prop:
                self.device_interface = "push"
            elif "GRASP" in datum_prop:
                self.device_interface = "grasp"
            elif "PEDAL" in datum_prop:
                self.device_interface = "boot"
            else:
                self.device_interface = "toggle"
            self.device_trans = new_device[dev_inst]["datums"][datum_prop]["global"]
            self.device_trans_inv = np.linalg.inv(self.device_trans)


class Seat(object):
    """ A device that can be used by the occupant(s). """

    def __init__(self, seat, inst):
        """
        Setup the seats using data from the api.
        """
        self.names = seat['name']
        self.inst = inst
        self.body_rot = np.array([[1, 0, 0, 0],
                                  [0, 1, 0, 0],
                                  [0, 0, 1, 0],
                                  [0, 0, 0, 1]])

        self.trans_origin = seat['trans']
        try:  # Store as list for consistency, even if only one point
            self.trans_front = [seat["datums"]["CUSHION_FRONT_CSYS"]["global"]]
            self.trans_back = [seat["datums"]["SRP_1472"]["global"]]
        except:  # Some seats have multiple points. 3, in the case of Seat_Troop_Multi_Occupant
            # TODO: Are there benches of other sizes (as custom, non-library parts)?
            self.trans_front = [seat["datums"]["CUSHION_FRONT_CSYS_1"]["global"],
                                seat["datums"]["CUSHION_FRONT_CSYS_2"]["global"],
                                seat["datums"]["CUSHION_FRONT_CSYS_3"]["global"]]
            self.trans_back = [seat["datums"]["SRP_1472_1"]["global"],
                               seat["datums"]["SRP_1472_2"]["global"],
                               seat["datums"]["SRP_1472_3"]["global"]]

        self.trans_front_inv = [np.linalg.inv(csys) for csys in self.trans_front]
        self.trans_back_inv = [np.linalg.inv(csys) for csys in self.trans_back]


class Ergonomics(object):
    """ Calculates field of vision metrics. """
    SEAT_CL = {"Seat_Crew", "Seat_Troop", "Seat_Troop_Multi_Occupant"}
    PEDAL_BRAKE_CL = {"Pedal_Brake_Electric", "Pedal_Brake_Hydraulic"}
    PEDAL_ACC_CL = {"Pedal_Accelerator_Electric", "Pedal_Accelerator_Mechanical"}

    def __init__(self, settings_dict):
        ## Essential settings
        self.output_json_file = settings_dict["output_json_file"]

        ## Settings needed for the testbench
        self.output_json_file = settings_dict["output_json_file"]
        self.mil_std = settings_dict["mil_std"]
        # TODO: Auto-determined up dir with hard-coded fore/side/ceil-finder dirs could lead to
        #  weird inconsistencies, so at least hard-code everything consistently for now
        #self.up_direction, _, _ = tba.get_up_vector()
        self.up_direction = np.array([0, 1.0, 0])

        self.fore_direction = settings_dict.get("fore_direction", np.array([0, 0, 1.0]))
        self.mil_std_dict = {}

        ## Get desired properties and datums for interested classes
        self.ergo_data = tba.get_data(parts_of_interest)

        self.man_data = self.ergo_data["Manikin"]

        ## Flatten related / interchangeable component classes dict {class:inst} --> {inst: data}
        self.seat_data = {v1: v2 for k, v in self.ergo_data.iteritems()
                          for v1, v2 in v.iteritems() if k in self.SEAT_CL}
        self.pedals_brake = {v1: v2 for k, v in self.ergo_data.iteritems()
                             for v1, v2 in v.iteritems() if k in self.PEDAL_BRAKE_CL}
        self.pedals_acc = {v1: v2 for k, v in self.ergo_data.iteritems()
                           for v1, v2 in v.iteritems() if k in self.PEDAL_ACC_CL}

        if len(self.pedals_acc) != 1 or len(self.pedals_brake) != 1:
            logging.warning("Incorrect total number of pedals found")
            logging.warning("There are {} brakes and {} accelerators!".format(
                len(self.pedals_brake), len(self.pedals_acc)))

        ## Isolate manikins by role. Some roles are expected to have only 1 member, but...
        self.occupants = [Manikin(self.man_data[man], man) for man in self.man_data]

        ## create device instance list. Why only one of each?
        self.dev_inst_list = [self.ergo_data[dev].keys()[0] for dev in
                              self.ergo_data if dev != "Manikin" and dev not in self.SEAT_CL]

        self.seats = [Seat(self.seat_data[s], s) for s in self.seat_data]
        self.devices = [Human_Interface_Device(d, self.ergo_data[d]) for d in
                        self.ergo_data if d != "Manikin" and d not in self.SEAT_CL]

        ## Get geometry for desired classes
        class_set = parts_of_interest.keys()

        ## Get geometry of individual manikins and components
        self.crew_geom = {}
        self.device_geom = {}

        self.surf_indiv = tba.load_geometry(class_set)

        for geom_inst, geom in self.surf_indiv.iteritems():
            # surf_indiv_items is dict keyed on instance names. Get mani inst names?
            if geom_inst in [i.inst for i in self.occupants]:  # Kludgy
                self.crew_geom[geom_inst] = geom
            if geom_inst in self.dev_inst_list:
                self.device_geom[geom_inst] = geom

        # Geometry used to raytrace ceiling/floor around seat. Some floor panels get custom class
        #  names, so get broad set of geom to be safe. Don't intersect seat or manikins.
        class_set = tba.get_all_geom_set() - tba.geom_sets["never_in_cabin"] - \
                    self.SEAT_CL - {"Manikin"}

        self.surface = tba.load_geometry(class_set, single_file=True)

        ## Make a binary space partition to speed up intersection calculations
        self.nodes = np.vstack((self.surface['x'], self.surface['y'], self.surface['z'])).T

        self.tris = self.surface['tris']
        self.b_tree = BSP_Tree(self.nodes, self.tris)
        self.b_tree.generate_tree()

        self.bsp_t = BSP_Element(self.nodes, self.tris)

        ## Determine which way is to the side
        self.side_direction = np.cross(self.up_direction, self.fore_direction)

        coord_array = np.vstack((self.surface['x'], self.surface['y'], self.surface['z'])).T

        ## Get list of points in the up direction and determine the highest point
        up_array = np.dot(coord_array, self.up_direction)
        self.top = up_array.max()

    def mil_std_check(self):
        """
        Determine whether manikins in each vehicle role pass or fail checks overall
        Distance information is stored in each individual manikin instance as the tests are run
        """
        ## Tests return (bool, dist_dict) tuples. We only combine the boolean part here.
        res_driver = [self._mil_1472_driver(mani)[0] for mani in self.occupants
                      if mani.role == "driver"]
        res_gunner = [self._mil_1472_common(mani)[0] for mani in self.occupants
                      if mani.role == "gunner"]
        res_veh_cmdr = [self._mil_1472_common(mani)[0] for mani in self.occupants
                        if mani.role == "vehicle_commander"]
        res_troop_cmdr = [self._mil_1472_common(mani)[0] for mani in self.occupants
                          if mani.role == "troop_commander"]
        res_troops = [self._mil_1472_common(mani)[0] for mani in self.occupants
                      if mani.role == "troop"]

        return {"Driver_Ergonomics": ALL_IF(res_driver),
                "Gunner_Ergonomics": ALL_IF(res_gunner),
                "Vehicle_Cmdr_Ergonomics": ALL_IF(res_veh_cmdr),
                "Troop_Cmdr_Ergonomics": ALL_IF(res_troop_cmdr),
                "Troop_Ergonomics": ALL_IF(res_troops)}

    def _get_trans(self, class_name, inst_name):
        """Get device transformation matrix- factors out commonly repeated code"""
        return self.ergo_data[class_name][inst_name]["trans"]

    def _assign_seat(self, mani):
        """
        Assign the seat closest to this manikin
        """
        seat_distances = []
        for seat in self.seats:
            dist_to_slot = []
            for slot in seat.trans_back_inv:  # Benches hold mult troops
                mat_seat_to_driver = np.dot(slot, mani.trans_origin)
                trans_seat_to_driver = mat_seat_to_driver[:3, 3]
                dist_to_slot.append(np.linalg.norm(trans_seat_to_driver))
            best_slot_num = np.argmin(dist_to_slot)
            seat_distances.append((seat, best_slot_num, dist_to_slot[best_slot_num]))

        closest_seat = min(seat_distances, key=lambda x: x[2])

        if closest_seat[2] <= 0.5:
            closest_seat_inst = closest_seat[0]
            closest_slot = closest_seat[1]
            logging.info("Mani {} {} was assigned to seat {}".format(mani.role, mani.inst,
                                                                     closest_seat_inst.inst))
            mani.seat = closest_seat_inst  # Assign to mani for access elsewhere
            mani.seat_front = closest_seat_inst.trans_front[closest_slot]
            mani.seat_front_inv = closest_seat_inst.trans_front_inv[closest_slot]
            mani.seat_back = closest_seat_inst.trans_back[closest_slot]
            mani.seat_back_inv = closest_seat_inst.trans_back_inv[closest_slot]

            return closest_seat_inst
        return None  # Safety in case veh model has no seats, or none that are near mani

    def _mil_1472_common(self, mani):
        """
        For a specified manikin, perform the metric checks shared amongst all roles. The
        final result is based on whether it passes results 1 and 5
        """
        logging.info("Performing common tests for manikin {} {}".format(mani.role, mani.inst))

        if mani.seat is None and self._assign_seat(mani) is None:
            #  Safety: One metric will fail, so total=False
            logging.warning("No seat was found near manikin {}".format(mani.inst))
            return False

        res = [self._mil_1472_01_SRP_to_roof(mani),
               self._mil_1472_05_seat_height(mani)]

        res_bool, res_distances = merge_res(res)
        seat_distances = {"srp": mani.seat_back, "cushion_front": mani.seat_front}
        mani.mil_std_distances.update(res_distances)  # Assoc distances with mani for plotting
        mani.mil_std_distances.update(seat_distances)
        return res_bool, res_distances

    def _mil_1472_driver(self, mani):
        """
        For drivers, perform all available mil spec checks (1-10)

        Technically this will iterate over all drivers, steering wheels, and pedals etc. But if
        there is more than one of those controls per vehicle, the design may have other issues.
        """
        logging.info("Performing driver tests for manikin {} {}".format(mani.role, mani.inst))

        if mani.seat is None and self._assign_seat(mani) is None:
            #  Safety: One metric will fail, so total=False
            logging.warning("No seat was found near manikin {}".format(mani.inst))
            return False

        if not self.pedals_acc or not self.pedals_brake:
            logging.warning("Could not assess driver metrics because assembly is missing pedals")
            logging.info("{} accelerator pedals and {} brake pedals found".format(
                len(self.pedals_acc), len(self.pedals_brake)))
            return False

        res = [self._mil_1472_common(mani),
               self._mil_1472_02_seat_to_steering_wheel(mani),
               self._mil_1472_03_seat_to_dash(mani),
               self._mil_1472_04_seat_to_under_steering_wheel(mani),
               self._mil_1472_06_seat_to_accel(mani),  # TODO: implement metric 7?
               self._mil_1472_08_brake_to_under_steering_wheel(),
               self._mil_1472_09_driver_center_to_brake_right(mani),
               self._mil_1472_10_brake_right_to_accel_right()]
        res_bool, res_distances = merge_res(res)
        mani.mil_std_distances.update(res_distances)  # Assoc distances with mani for plotting
        return res_bool, res_distances

    ###########
    # FUNCTIONS TO DO SEPARATE MIL-STD checks
    def _mil_1472_01_SRP_to_roof(self, mani):
        """
        True or False, the distance from the seat reference point is within the allowed range
        """
        dim_name = "01_SRP_to_roof"
        min_dist, max_dist = get_mil_std_limits(dim_name, self.mil_std)

        ## Test dimension 01 from MIL-STD
        srp_origin = mani.seat_back[:3, 3]

        ## Assign beginning point above the seat ref point (assumes fixed up dir, +y)
        ceiling_dist = 2.0
        roof_point = srp_origin.copy()
        roof_point[1] += ceiling_dist

        ## Find ceiling by shooting a ray from the srp to a point above the roof
        start_orin = np.array(mani.seat_back[:3, 3])
        stop_orin = np.array(roof_point)
        t = self.b_tree.get_line_intersection_all(start_orin, stop_orin)
        tris_dist = list(t)

        ## Find smallest distance from start point to the tris the ray intersected
        for tri in tris_dist:
            if ceiling_dist > tri[1] > 0.2:
                ceiling_dist = tri[1]

        dev_distance = ceiling_dist
        logging.info("{} distance: {}".format(dim_name, dev_distance))
        res = dim_check(dev_distance, min_dist, max_dist)
        logging.info("{} : {}".format(dim_name, res))

        mil_std_dist = {dim_name: dev_distance}
        return res, mil_std_dist

    def _mil_1472_02_seat_to_steering_wheel(self, mani):
        """
        True or False, the distance between the seat and the steering wheel is
        within  allowed range
        """
        class_name = "Steering_Wheel"
        dim_name = "02_seat_to_steering_wheel"
        min_dist, max_dist = get_mil_std_limits(dim_name, self.mil_std)

        seat_inv = mani.seat_back_inv

        scores = []
        for inst_name in self.ergo_data[class_name]:
            dev_trans = self._get_trans(class_name, inst_name)
            wheel_coord = dev_trans[:3, 3]
            wheel_up_coord = np.dot(self.up_direction, wheel_coord)
            wheel_fore_coord = np.dot(self.fore_direction, wheel_coord)
            wheel_ang_vert = np.arctan(wheel_fore_coord / wheel_up_coord)
            wheel_ang_radius = np.pi / 2 - wheel_ang_vert
            wheel_radius = 0.2
            wheel_fore = wheel_radius * np.sin(wheel_ang_radius)

            mat_seat_to_device = np.dot(seat_inv, dev_trans)
            trans_seat_to_device = mat_seat_to_device[:3, 3]
            dev_distance = abs(np.dot(self.fore_direction,
                                      trans_seat_to_device)) + wheel_fore
            logging.info("{} distance: {}".format(dim_name, dev_distance))
            dim_score = dim_check(dev_distance, min_dist, max_dist)
            scores.append(dim_score)

        mil_std_dist = {dim_name: dev_distance,  # For plot, assume 1 score
                        "steer_wheel_loc": dev_trans[:3,3],
                        "steer_wheel_edge_len": wheel_fore}
        logging.info("{} : {}".format(dim_name, scores))
        return ALL_IF(scores), mil_std_dist

    def _mil_1472_03_seat_to_dash(self, mani):
        """
        True or False, the distance between the front of the seat and the dash is within the
        allowed minimum and maximum
        """
        class_name = "Gear_Select_Panel"
        dim_name = "03_seat_to_dash"

        seat_inv = mani.seat_back[:3, 3]
        min_dist, max_dist = get_mil_std_limits(dim_name, self.mil_std)

        scores = []
        for inst_name in self.ergo_data[class_name]:
            dev_trans = self._get_trans(class_name, inst_name)[:3, 3]
            dev_distance = abs(seat_inv[2] - dev_trans[2])
            logging.info("{} distance: {}".format(dim_name, dev_distance))
            dim_score = dim_check(dev_distance, min_dist, max_dist)
            scores.append(dim_score)

        mil_std_dist = {dim_name: dev_distance}  # For plot, assume 1 score
        logging.info("{} : {}".format(dim_name, scores))
        return ALL_IF(scores), mil_std_dist

    def _mil_1472_04_seat_to_under_steering_wheel(self, mani):
        """
        True or False, the height between the seat front and the underside of the steering wheel
         is within the allowed minimum and maximum.
        """
        class_name = "Steering_Wheel"
        dim_name = "04_seat_to_under_steering_wheel"
        min_dist, max_dist = get_mil_std_limits(dim_name, self.mil_std)

        scores = []
        for inst_name in self.ergo_data[class_name]:
            dev_trans = self._get_trans(class_name, inst_name)

            wheel_coord = dev_trans[:3, 3]
            wheel_up_coord = np.dot(self.up_direction, wheel_coord)
            wheel_fore_coord = np.dot(self.fore_direction, wheel_coord)
            wheel_ang_vert = np.arctan(wheel_fore_coord / wheel_up_coord)
            wheel_ang_radius = np.pi / 2 - wheel_ang_vert
            wheel_radius = 0.2
            wheel_bottom = wheel_radius * np.cos(wheel_ang_radius)

            dev_dist = abs(dev_trans[:3, 3] - mani.seat_back[:3, 3]) - wheel_bottom
            dev_distance = np.dot(self.up_direction, dev_dist)
            logging.info("{} distance: {}".format(dim_name, dev_distance))
            scores.append(dim_check(dev_distance, min_dist, max_dist))

        mil_std_dist = {dim_name: dev_distance,  # For plot, assume 1 score
                        "steer_wheel_edge_ht": wheel_bottom}
        logging.info("{} : {}".format(dim_name, scores))
        return ALL_IF(scores), mil_std_dist

    def _mil_1472_05_seat_height(self, mani):
        """
        True or False, the height of the seat pan from the floor is within the allowed range
        """
        dim_name = "05_seat_height"

        srp_origin = mani.seat_front[:3, 3]

        ## Assign beginning point below the seat ref point (assumes fixed up dir, +y)
        floor_dist = 4.0  # Very tall or off center vehicles might have problems
        past_floor_pt = srp_origin.copy()
        past_floor_pt[1] -= floor_dist

        ## Find floor by shooting a ray from the srp to a point below the floor
        start_orin = np.array(mani.seat_front[:3, 3])
        stop_orin = np.array(past_floor_pt)
        t = self.b_tree.get_line_intersection_all(start_orin, stop_orin)
        t = self.b_tree.get_line_intersection_all(start_orin, stop_orin)
        tris_dist = list(t)

        for tri in tris_dist:  # Find nearest geom pt above/below the seat
            if floor_dist > tri[1] > 0.2:
                floor_dist = tri[1]

        # We care about floor point as direct downward distance... new coord is dist from srp
        past_floor_pt[1] = srp_origin[1] - floor_dist

        min_dist, max_dist = get_mil_std_limits(dim_name, self.mil_std)
        dev_distance = floor_dist
        logging.info("{} distance: {}".format(dim_name, dev_distance))

        res = dim_check(dev_distance, min_dist, max_dist)
        logging.info("{} : {}".format(dim_name, res))
        mil_std_dist = {dim_name: dev_distance,
                        "floor": past_floor_pt}
        return res, mil_std_dist

    def _mil_1472_06_seat_to_accel(self, mani):
        """
        Check if distance between the seat front and the accelerator pedal is within
            the allowed range
        """
        dim_name = "06_seat_to_accel"
        seat_inv = mani.seat_back_inv
        min_dist, max_dist = get_mil_std_limits(dim_name, self.mil_std)

        scores = []
        for inst_name in self.pedals_acc:
            dev_trans = self.pedals_acc[inst_name]['trans']
            mat_seat_to_device = np.dot(seat_inv, dev_trans)
            trans_seat_to_device = mat_seat_to_device[:3, 3]
            dev_distance = abs(np.dot(self.fore_direction, trans_seat_to_device))
            logging.info("{} distance: {}".format(dim_name, dev_distance))
            scores.append(dim_check(dev_distance, min_dist, max_dist))

        mil_std_dist = {dim_name: dev_distance,  # For plot, assume 1 score
                        "accel_loc": dev_trans[:3, 3]}  # For plotting assume there's only 1 pedal
        logging.info("{} : {}".format(dim_name, scores))
        return ALL_IF(scores), mil_std_dist

    def _mil_1472_08_brake_to_under_steering_wheel(self):
        """
        True or False, the distance between the normal of the brake pedal and the underside of
         the steering wheel is within the allowed minimum and maximum
        """
        dim_name = "08_brake_to_under_steering_wheel"
        min_dist, max_dist = get_mil_std_limits(dim_name, self.mil_std)

        steering_wheel_name = self.ergo_data["Steering_Wheel"].keys()[0]
        steering_wheel_trans = self._get_trans("Steering_Wheel", steering_wheel_name)

        scores = []
        for brake_name in self.pedals_brake:
            brake_trans = self.pedals_brake[brake_name]['trans']
            second_dev_inv = np.linalg.inv(brake_trans)
            mat_dev_to_device = np.dot(second_dev_inv, steering_wheel_trans)
            trans_dev_to_device = mat_dev_to_device[:3, 3]
            dev_distance_up = abs(np.dot(self.up_direction,
                                         trans_dev_to_device))
            dev_distance_fore = abs(np.dot(self.fore_direction,
                                           trans_dev_to_device))

            dev_distance = np.sqrt(np.dot(dev_distance_up, dev_distance_fore))
            logging.info("{} distance: {}".format(dim_name, dev_distance))
            scores.append(dim_check(dev_distance, min_dist, max_dist))
        mil_std_dist = {dim_name: dev_distance}  # For plot, assume 1 score
        logging.info("{} : {}".format(dim_name, scores))
        return ALL_IF(scores), mil_std_dist

    def _mil_1472_09_driver_center_to_brake_right(self, mani):
        """
        True or False, The lateral distance between the driver's center and the right side of the
         brake pedal is within allowed minimum and maximum
        """
        dim_name = "09_driver_center_to_brake_right"
        seat_inv = mani.seat_back_inv
        min_dist, max_dist = get_mil_std_limits(dim_name, self.mil_std)

        scores = []
        for brake_name in self.pedals_brake:
            brake_trans = self.pedals_brake[brake_name]['trans']
            mat_seat_to_device = np.dot(seat_inv, brake_trans)
            trans_seat_to_device = mat_seat_to_device[:3, 3]
            dev_distance = abs(np.dot(self.fore_direction, trans_seat_to_device))
            logging.info("{} distance: {}".format(dim_name, dev_distance))

            scores.append(dim_check(dev_distance, min_dist, max_dist))
        mil_std_dist = {dim_name: dev_distance,  # For plot, assume 1 score
                        "brake_loc": brake_trans[:3, 3]}  # Outer check verifies (at least) 1 brake
        logging.info("{} : {}".format(dim_name, scores))
        return ALL_IF(scores), mil_std_dist

    def _mil_1472_10_brake_right_to_accel_right(self):
        """
        True or False, the distance between the right side of the brake pedal and the right side
         of the accelerator is within allowed range
        """
        dim_name = "10_brake_right_to_accel_right"

        min_dist, max_dist = get_mil_std_limits(dim_name, self.mil_std)
        scores = []

        for brake_name in self.pedals_brake:
            brake_trans = self.pedals_brake[brake_name]['trans']
            for acc_name in self.pedals_acc:
                acc_trans = self.pedals_acc[acc_name]['trans']
                second_dev_inv = np.linalg.inv(acc_trans)
                mat_dev_to_device = np.dot(second_dev_inv, brake_trans)
                trans_dev_to_device = mat_dev_to_device[:3, 3]
                dev_distance = abs(np.dot(self.side_direction,
                                          trans_dev_to_device))
                logging.info("{} distance: {}".format(dim_name, dev_distance))
                dim_score = dim_check(dev_distance, min_dist, max_dist)
                scores.append(dim_score)
        mil_std_dist = {dim_name: dev_distance}  # For plot, assume 1 score
        logging.info("{} : {}".format(dim_name, scores))
        return ALL_IF(scores), mil_std_dist
    #  END SEPARATE MIL STD FUNCS
    ####################

    def assign_to_manikins(self):
        """
        Assign devices to manikins (i.e. who can reach what)
        """
        interface = ["grasp", "push", "toggle", "boot"]

        for man in self.occupants:
            for dev in self.devices:
                dev_interface = dev.device_interface
                if dev_interface != "none" and dev_interface != "seat":
                    visible = [dev.class_name, False]
                    for inter in interface:
                        if inter in dev_interface:
                            if inter == "grasp":
                                man_inv = man.grasp_left_veh_inv
                                man_inv_rt = man.grasp_right_veh_inv
                                veh_trans = dev.device_trans
                                max_dist = 0.45
                            elif inter == "push":
                                man_inv = man.push_left_veh_inv
                                man_inv_rt = man.push_right_veh_inv
                                veh_trans = dev.device_trans
                                max_dist = 0.68
                            elif inter == "toggle":
                                man_inv = man.toggle_left_veh_inv
                                man_inv_rt = man.toggle_right_veh_inv
                                veh_trans = dev.device_trans
                                max_dist = 0.55
                            elif inter == "boot":
                                man_inv = man.boot_left_veh_inv
                                man_inv_rt = man.boot_right_veh_inv
                                veh_trans = dev.device_trans
                                max_dist = 0.1
                            else:
                                raise ValueError("Unrecognized interface type specified")

                            mat_human_to_device = np.dot(man_inv, veh_trans)
                            trans_human_to_device = mat_human_to_device[:3, 3]
                            reach_distance = np.sqrt(np.dot(trans_human_to_device,
                                                            trans_human_to_device))
                            # TODO: why convert theta to deg and immediately back to rad for use??
                            theta = np.arccos(trans_human_to_device[2] / reach_distance) * \
                                    (180. / np.pi)
                            res_z_line = reach_distance * np.sin(theta * np.pi / 180)
                            phi = np.arccos(trans_human_to_device[0] / res_z_line) * (180. / np.pi)

                            ###right hand zones
                            mat_human_to_device_rt = np.dot(man_inv_rt, veh_trans)
                            trans_human_to_device_rt = mat_human_to_device_rt[:3, 3]
                            reach_distance_rt = np.sqrt(np.dot(trans_human_to_device_rt,
                                                               trans_human_to_device_rt))
                            theta_rt = np.arccos(trans_human_to_device[2] / reach_distance) * \
                                       180. / np.pi
                            res_z_line_rt = reach_distance * np.sin(theta * np.pi / 180)
                            phi_rt = np.arccos(trans_human_to_device[0] / res_z_line_rt) * \
                                     180. / np.pi

                            if reach_distance <= max_dist and theta <= 180.0 and \
                                            phi <= 180.0 or reach_distance_rt <= max_dist and \
                                            theta_rt <= 180.0 and phi_rt <= 180.0:
                                visible = [dev.class_name, True]
                                break
                    man.ergo_devices.append(visible)

            msg = "\nManikin {} can use these devices:\n".format(man.name)
            for p in man.ergo_devices:
                msg += "\t{}\n".format(p)
            logging.info(msg)

    def manikin_visible_devices(self):
        """
        Test to see what each manikin can see
        """
        for man in self.occupants:
            for device in self.devices:
                visible = [device.class_name, False]
                for eye_name, eye_inv_trans in (("left", man.left_eye_veh_inv),
                                                ("right", man.right_eye_veh_inv)):

                    mtrx_eye_to_dev = np.dot(eye_inv_trans, device.dev_trans_origin)
                    trans_eye_to_dev = mtrx_eye_to_dev[:3, 3]

                    resultant_z_to_dev = np.dot(mtrx_eye_to_dev, np.array([0, 0, -1, 0]))[:3]
                    unit_sight_line = normalize(trans_eye_to_dev)
                    sight_distance = np.sqrt(np.dot(trans_eye_to_dev, trans_eye_to_dev))
                    dot_prod = np.dot(resultant_z_to_dev, unit_sight_line)
                    theta = np.arccos(dot_prod) * 180.0 / np.pi

                    if sight_distance < 2.0 and theta < 75.0:
                        visible = [device.class_name, True]
                        break
                man.vision_devices.append(visible)

            msg = "\nmanikin {} can see these devices:\n".format(man.name)
            for p in man.vision_devices:
                msg += "\t{}\n".format(p)
            logging.info(msg)

    def post_process(self, mil_std_results):  # TODO: Reorg how output will be handled/ written
        """
        Generate the metrics from the stored raw ray trace results.
        """
        metrics_out = {"Reach Evaluation of Interior Components": {},
                       "Visible Evaluation of Interior Components": {},
                       "MIL-STD-1472": mil_std_results}

        # Only pass this metric if all sub-tests passed (result !=False)
        for man in self.occupants:
            metrics_out["Reach Evaluation of Interior Components"][man.role] = {}
            metrics_out["Visible Evaluation of Interior Components"][man.role] = {}
            for dev in man.ergo_devices:
                metrics_out["Reach Evaluation of Interior Components"][man.role][dev[0]] = dev[1]
            for dev in man.vision_devices:
                metrics_out["Visible Evaluation of Interior Components"][
                    man.role][dev[0]] = dev[1]

        for mani in self.occupants:
            if mani.role == "driver":  # Plot grasp zones, etc
                self.plot_manikins(mani)
            # Plot mil std metrics (as relevant) for every separate mani
            if mani.seat is not None:
                seat_geom = self.surf_indiv[mani.seat.inst]
                plot_mil_std(seat_geom, mani)

        tba.write_results(metrics_out)

    def plot_zones(self, view, mani_obj):
        """
        plot the each of the zones for each manikin
        """
        if view == "Side View":
            cen1_index = 2
            cen2_index = 1
            theta1, theta2 = 60, 270
            theta3, theta4 = theta1, theta2
            theta1_boot, theta2_boot = 180, 0
            cent_mul = 1
        elif view == "Top View":
            cen1_index = 2
            cen2_index = 0
            theta1, theta2 = 90, 300
            theta3, theta4 = 60, 270
            theta1_boot, theta2_boot = 0, 360
            cent_mul = 1
        elif view == "Front View":
            cen1_index = 0
            cen2_index = 1
            theta1, theta2 = 0, 360
            theta3, theta4 = theta1, theta2
            theta1_boot, theta2_boot = 180, 0
            ## Multiplier for center x coordinates. Front view need the x axis reversed
            cent_mul = -1
        else:
            raise ValueError('Unrecognized view requested')

        role = mani_obj.role
        erg_dev = mani_obj.ergo_devices
        ## Grasp zone
        radius_grasp = .45
        grasp_left = mani_obj.grasp_left_veh[:3, 3]
        center_left = (grasp_left[cen1_index] * cent_mul, grasp_left[cen2_index])
        grasp_right = mani_obj.grasp_right_veh[:3, 3]
        center_right = (grasp_right[cen1_index] * cent_mul, grasp_right[cen2_index])

        plot_wedge(center_left, radius_grasp, theta1, theta2, "g")
        plot_wedge(center_right, radius_grasp, theta3, theta4, "g")

        ## Push Zone
        radius = 0.68
        push_left = mani_obj.push_left_veh[:3, 3]
        center_left = (push_left[cen1_index] * cent_mul, push_left[cen2_index])
        push_right = mani_obj.push_right_veh[:3, 3]
        center_right = (push_right[cen1_index] * cent_mul, push_right[cen2_index])

        plot_wedge(center_left, radius, theta1, theta2, "b")
        plot_wedge(center_right, radius, theta3, theta4, "b")

        ## Toggle zone
        radius = 0.55
        toggle_left = mani_obj.toggle_left_veh[:3, 3]
        center_left = (toggle_left[cen1_index] * cent_mul, toggle_left[cen2_index])
        toggle_right = mani_obj.toggle_right_veh[:3, 3]
        center_right = (toggle_right[cen1_index] * cent_mul, toggle_right[cen2_index])

        plot_wedge(center_left, radius, theta1, theta2, "c")
        plot_wedge(center_right, radius, theta3, theta4, "c")

        ## Boot zones
        radius_boot = 0.1
        boot_left = mani_obj.boot_left_veh[:3, 3]
        center_left = (boot_left[cen1_index] * cent_mul, boot_left[cen2_index])
        boot_right = mani_obj.boot_right_veh[:3, 3]
        center_right = (boot_right[cen1_index] * cent_mul, boot_right[cen2_index])

        plot_wedge(center_left, radius_boot, theta1_boot, theta2_boot, "b")
        plot_wedge(center_right, radius_boot, theta1_boot, theta2_boot, "b")

        return role, erg_dev  # TODO: These two things don't really need to be returned

    def plot_manikins(self, mani_obj):
        """
        Plot manikin push, grasp, and toggle zones
        """
        plt.clf()
        nullfmt = NullFormatter()

        left, width = 0.1, 0.4
        bottom, height = 0.1, 0.4
        bottom_h = left_h = left + width + .05

        rect_side = [left_h, bottom, width, height]
        rect_top = [left_h, bottom_h, width, height]
        rect_front = [left, bottom, width, height]

        plt.axes(rect_side).yaxis.set_major_formatter(nullfmt)
        plt.axes(rect_side).grid(True)
        plt.axes(rect_side).set_aspect('equal', anchor='SW')
        view = "Side View"
        axis1, axis2 = 'z', 'y'

        veh_role, erg_devices = ergo_metrics.plot_zones(view, mani_obj)

        geom = self.crew_geom[mani_obj.inst]
        triangulation = Triangulation(geom[axis1], geom[axis2], geom['tris'])
        plt.tricontourf(triangulation, triangulation.x * 0.0, colors="k", alpha=0.6)

        for dev_inst, geom in self.device_geom.items():
            for ergo_dev in erg_devices:
                if ergo_dev[0] in dev_inst.title() and ergo_dev[1] is True:
                    triangulation = Triangulation(geom[axis1], geom[axis2], geom['tris'])
                    plt.tricontourf(triangulation, triangulation.x * 0.0, colors="y", alpha=1.0)

        plt.axes(rect_top)
        plt.axes(rect_top).xaxis.set_major_formatter(nullfmt)
        plt.axes(rect_top).grid(True)
        plt.axes(rect_top).set_aspect('equal', anchor='SW')
        view = "Top View"
        axis1, axis2 = 'z', 'x'

        veh_role, erg_devices = ergo_metrics.plot_zones(view, mani_obj)

        geom = self.crew_geom[mani_obj.inst]
        triangulation = Triangulation(geom[axis1], geom[axis2], geom['tris'])
        plt.tricontourf(triangulation, triangulation.x * 0.0, colors="k", alpha=0.6)

        for dev_inst, geom in self.device_geom.items():
            for ergo_dev in erg_devices:
                if ergo_dev[0] in dev_inst.title() and ergo_dev[1] is True:
                    triangulation = Triangulation(geom[axis1], geom[axis2], geom['tris'])
                    plt.tricontourf(triangulation, triangulation.x * 0.0, colors="y", alpha=1.0)
        xmin, xmax, ymin, ymax = plt.axis()

        plt.xlim(xmin, 5.25)
        plt.axes(rect_front)
        plt.axes(rect_front).grid(True)
        plt.axes(rect_front).set_aspect('equal')
        view = "Front View"
        axis1, axis2 = 'x', 'y'

        veh_role, erg_devices = ergo_metrics.plot_zones(view, mani_obj)

        geom = self.crew_geom[mani_obj.inst]
        triangulation = Triangulation(geom[axis1] * -1.0, geom[axis2], geom['tris'])
        plt.tricontourf(triangulation, triangulation.x * 0.0, colors="k", alpha=0.6)

        for dev_inst, geom in self.device_geom.items():
            for ergo_dev in erg_devices:
                if ergo_dev[0] in dev_inst.title() and ergo_dev[1] is True:
                    triangulation = Triangulation(geom[axis1] * -1.0, geom[axis2], geom['tris'])
                    plt.tricontourf(triangulation, triangulation.x * 0.0, colors="y", alpha=1.0)

        plt.suptitle(veh_role.title() + " ergonomic zones", horizontalalignment='right',
                     verticalalignment='top')

        img_file_name = veh_role + "_ergonomic_zones.png"  # Assumes just one driver
        current_dir = os.getcwd()
        logging.info("Plotting Manikin zones: writing image to file {} in {}".format(
            img_file_name, current_dir))
        plt.savefig(img_file_name, dpi=100)
        plt.clf()


def plot_wedge(center, radius, theta1, theta2, color):
    """ Using the center from each zone plots a wedge to represent the zone"""
    zone_wedge = Wedge(center, radius, theta1, theta2, color=color, alpha=0.2)
    plt.gca().add_patch(zone_wedge)


def plot_mil_std(seat_geom, mani):
    """ plot the seat with mil_std dimensions"""
    mil_std = mani.mil_std_distances

    ## General plotting settings
    fig = plt.figure(figsize=(25, 10))
    ax = fig.add_subplot(1, 1, 1)
    ax.set_aspect('equal')
    ax.grid(True)
    axis1, axis2 = 'z', 'y'
    ## Points on seat that the MIL-STD was measured from
    cushion_edge = mil_std["cushion_front"][:3, 3]
    seat_srp = mil_std["srp"][:3, 3]
    seat_floor = mil_std["floor"]
    seat_height = seat_floor[1] + (cushion_edge[1] - seat_floor[1])  # TODO Simplify

    ## Generate text for text box
    leg_text = ""
    ## Sort mil_std
    sorted_mil_std = sorted(mil_std.iteritems(), key=operator.itemgetter(0))
    for std in sorted_mil_std:
        if std[0] not in {"cushion_front", "srp", "floor", "accel_loc", "steer_wheel_edge_ht",
                          "steer_wheel_edge_len", "steer_wheel_loc", "brake_loc"}:
            leg_text += str(std[0]) + ": " + str(std[1])[:4] + " m\n"
    ## text box properties
    props = dict(boxstyle='round', facecolor='white', alpha=1.0)
    ## Plot text box
    ax.text(1.03, .99, leg_text, ha="left", va="top", bbox=props, transform=ax.transAxes)

    ## Plot seat
    triangulation = Triangulation(seat_geom[axis1], seat_geom[axis2], seat_geom['tris'])
    plt.tricontourf(triangulation, triangulation.x * 0.0, colors="k", alpha=0.6)

    ## plot MIL-STD dimensions
    arrow_style_dict = dict(facecolor='b', arrowstyle='<->')

    ## MIL-STD 01
    if "01_SRP_to_roof" in mil_std:
        arrow_start_y = seat_srp[1]
        arrow_end_y = arrow_start_y + mil_std["01_SRP_to_roof"]
        arrow_start_x = seat_srp[2]
        arrow_end_x = seat_srp[2]
        text_pos_x = arrow_start_x + 0.01
        text_pos_y = arrow_start_y + 0.75 * (arrow_end_y - arrow_start_y)
        plt.annotate('', xy=(arrow_start_x, arrow_start_y),
                     xytext=(arrow_end_x, arrow_end_y), arrowprops=arrow_style_dict)
        plt.text(text_pos_x, text_pos_y, '01')

        ymin, ymax = plt.ylim()
        plt.ylim(ymin, max(ymax, arrow_end_y))

    ## MIL-STD 02
    if "02_seat_to_steering_wheel" in mil_std:
        arrow_start_x = seat_srp[2] - mil_std["02_seat_to_steering_wheel"]
        arrow_end_x = seat_srp[2]
        arrow_start_y = seat_height + 0.2
        arrow_end_y = seat_height + 0.2
        text_pos_x = arrow_start_x + 0.1 * (arrow_end_x - arrow_start_x) + 0.01
        text_pos_y = arrow_end_y + 0.01
        plt.annotate('', xy=(arrow_start_x, arrow_start_y),
                     xytext=(arrow_end_x, arrow_end_y), arrowprops=arrow_style_dict)
        plt.text(text_pos_x, text_pos_y, '02')

    ## MIL-STD 03
    if "03_seat_to_dash" in mil_std:
        arrow_start_x = seat_srp[2] - mil_std["03_seat_to_dash"]
        arrow_end_x = seat_srp[2]
        arrow_start_y = seat_height
        arrow_end_y = seat_height
        text_pos_x = arrow_start_x + 0.1
        text_pos_y = arrow_start_y + 0.01
        plt.annotate('', xy=(arrow_start_x, arrow_start_y),
                     xytext=(arrow_end_x, arrow_end_y), arrowprops=arrow_style_dict)
        plt.text(text_pos_x, text_pos_y, '03')

        xmin, xmax = plt.xlim()
        plt.xlim(min(xmin, arrow_start_x) - 0.01, xmax)

    ## MIL-STD 04
    if "04_seat_to_under_steering_wheel" in mil_std:
        arrow_start_y = seat_height
        arrow_end_y = arrow_start_y + mil_std["04_seat_to_under_steering_wheel"]
        arrow_start_x = cushion_edge[2]
        arrow_end_x = cushion_edge[2]
        text_pos_x = cushion_edge[2] + 0.01
        text_pos_y = arrow_start_y + (arrow_end_y - arrow_start_y) / 2 - 0.01
        plt.annotate('', xy=(arrow_start_x, arrow_start_y),
                     xytext=(arrow_end_x, arrow_end_y), arrowprops=arrow_style_dict)
        plt.text(text_pos_x, text_pos_y, '04')

    ## MIL-STD 05
    if "05_seat_height" in mil_std:
        arrow_start_x = cushion_edge[2]
        arrow_end_x = arrow_start_x
        arrow_start_y = seat_floor[1]
        arrow_end_y = arrow_start_y + mil_std["05_seat_height"]
        text_pos_x = cushion_edge[2] + 0.01
        text_pos_y = seat_floor[1] + mil_std["05_seat_height"] / 2 - 0.01
        plt.annotate('', xy=(arrow_start_x, arrow_start_y),
                     xytext=(arrow_end_x, arrow_end_y), arrowprops=arrow_style_dict)
        plt.text(text_pos_x, text_pos_y, '05')

        ymin, ymax = plt.ylim()
        plt.ylim(min(ymin, arrow_start_y), ymax)

    ## MIL-STD 06
    if "06_seat_to_accel" in mil_std:
        arrow_start_x = cushion_edge[2] - 0.45
        arrow_end_x = cushion_edge[2]
        arrow_start_y = seat_floor[1]
        arrow_end_y = arrow_start_y
        text_pos_x = arrow_start_x + 0.1
        text_pos_y = arrow_start_y + 0.01
        plt.annotate('', xy=(arrow_start_x, arrow_start_y),
                     xytext=(arrow_end_x, arrow_end_y), arrowprops=arrow_style_dict)
        plt.text(text_pos_x, text_pos_y, '06')

        xmin, xmax = plt.xlim()
        plt.xlim(min(xmin, arrow_start_x) - 0.01, xmax)

    ## MIL-STD 07
    if "07_steering_clearance" in mil_std:
        arrow_end_x = cushion_edge[2]
        arrow_start_x = arrow_end_x - mil_std["06_seat_to_accel"]
        arrow_start_y = seat_floor[1]
        arrow_end_y = arrow_start_y
        text_pos_x = arrow_start_x + (arrow_end_x - arrow_start_x) / 2
        text_pos_y = seat_floor[1] + .01
        plt.annotate('', xy=(arrow_start_x, arrow_start_y),
                     xytext=(arrow_end_x, arrow_end_y), arrowprops=arrow_style_dict)
        plt.text(text_pos_x, text_pos_y, '07')

    ## MIL-STD 08
    if "08_brake_to_under_steering_wheel" in mil_std:
        steer_wheel_loc = mil_std["steer_wheel_loc"]
        arrow_end_x = steer_wheel_loc[2] + mil_std["steer_wheel_edge_len"]
        arrow_end_y = steer_wheel_loc[1] - mil_std["steer_wheel_edge_ht"]
        arrow_start_x = mil_std["brake_loc"][2]
        arrow_start_y = mil_std["brake_loc"][1]
        text_pos_x = arrow_start_x + (arrow_end_x - arrow_start_x) / 2
        text_pos_y = arrow_start_y + (arrow_end_y - arrow_start_y) / 2
        plt.annotate('', xy=(arrow_start_x, arrow_start_y),
                     xytext=(arrow_end_x, arrow_end_y), arrowprops=arrow_style_dict)
        plt.text(text_pos_x, text_pos_y, '08')

    ## MIL-STD 09
    if "09_driver_center_to_brake_right" in mil_std:
        arrow_end_x = seat_srp[2]
        arrow_start_x = arrow_end_x - mil_std["09_driver_center_to_brake_right"]
        arrow_start_y = seat_srp[1]
        arrow_end_y = arrow_start_y
        text_pos_x = arrow_start_x + (arrow_end_x - arrow_start_x) / 2
        text_pos_y = arrow_start_y + .01
        plt.annotate('', xy=(arrow_start_x, arrow_start_y),
                     xytext=(arrow_end_x, arrow_end_y), arrowprops=arrow_style_dict)
        plt.text(text_pos_x, text_pos_y, '09')

    ## MIL-STD 10
    if "10_brake_right_to_accel_right" in mil_std:
        arrow_end_x = cushion_edge[2]
        arrow_start_x = arrow_end_x - mil_std["06_seat_to_accel"]
        arrow_start_y = seat_floor[1]
        arrow_end_y = arrow_start_y
        text_pos_x = arrow_start_x + (arrow_end_x - arrow_start_x) / 2
        text_pos_y = seat_floor[1] + .01
        plt.annotate('', xy=(arrow_start_x, arrow_start_y),
                     xytext=(arrow_end_x, arrow_end_y), arrowprops=arrow_style_dict)
        plt.text(text_pos_x, text_pos_y, '10')

    plt.suptitle("MIL-STD 1472 for {}".format(mani.role))
    img_file_name = "MIL-STD 1472_mani_{}.png".format(mani.inst)
    current_dir = os.getcwd()
    logging.info("Plotting MIL-STD-1472: writing image to file {} in {}".format(
        img_file_name, current_dir))
    plt.savefig(img_file_name, dpi=100)


def get_mil_std_limits(name, mil_std_dict):
    """ Returns the min and max values for an specific dimension check """
    std_min = mil_std_dict[name]['min']
    std_max = mil_std_dict[name]['max']
    return std_min, std_max


def dim_check(dev_dist, std_min, std_max):
    """ Boolean: does value fall within allowed range? """
    return std_min <= dev_dist <= std_max


if __name__ == "__main__":
    # Executes the ergonomics test bench.

    logging.info("\n" + 50 * "_" + "\ntest bench started\n" + 50 * "_")

    settings_file = r"settings.js"
    settings_dict = tba.load_settings(settings_file)
    ergo_metrics = Ergonomics(settings_dict)
    mil_std_results = ergo_metrics.mil_std_check()
    ergo_metrics.assign_to_manikins()
    ergo_metrics.manikin_visible_devices()
    ergo_metrics.post_process(mil_std_results)
    logging.info("Test bench completed\n" + 50 * "_")