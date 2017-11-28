""" Test bench to assess closest visible point in front of vehicle. """
from math import sqrt, atan2, pi, acos
import re
import logging
import itertools

import numpy as np


import sys
import os
import _winreg


def query_analysis_tools():
    """
    Find the location of the Ricardo Python library.
    """
    analysis_tools_key_name = r'SOFTWARE\META\AnalysisTools'

    analysis_tools_key = _winreg.OpenKey(_winreg.HKEY_LOCAL_MACHINE, analysis_tools_key_name, 0,
                                         _winreg.KEY_READ)

    number_of_keys =  _winreg.QueryInfoKey(analysis_tools_key)[0]  # 0 means number of sub_keys

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
sys.path.append(os.path.join(tools['field_of_view']['InstallLocation']))


from rpl.tools.ray_tracing.bsp_tree_poly import BSP_Tree
from rpl.tools.geometry.geom_utils import (rotation_about_vector, translate, mul,
                                           get_translation, normalize)
import rpl.tools.api.test_bench_api as tba

## xpath strings for extracting datums and properties from AVM format XML files
datum = r"//Datum[@DatumName = $da]"
prop = r"//Property[@Name = $pr]/Value/ValueExpression/Value"

parts_of_interest = {'Manikin': {'Datum': ['EXT_EYE_CSYS_LEFT',
                                           'EXT_EYE_CSYS_RIGHT'],
                                 "Property":["vehicle_role"],
                                 "Required" : "{+}"},
                     "Periscope":{"Datum":["EXT_PSCOPE_INNER",
                                           "EXT_PSCOPE_OUTER"],
                                  "Property":["focal_distance",
                                              "field_of_view_angle_left",
                                              "field_of_view_angle_right",
                                              "field_of_view_angle_down",
                                              "field_of_view_angle_up"]}}


class Vision_Device(object):
    """ A device that can be used to view things. """

    def __init__(self, periscope, p_data):
        """
        Setup the vision device using data from the supplied xml file.
        """
        ##get periscope properties from the dictionary returned by the api
        datums = p_data[periscope]["datums"]
        properties = p_data[periscope]["properties"]
        self.name = periscope

        ##datums
        self.trans_glass = datums["EXT_PSCOPE_OUTER"]["global"]
        self.trans_inner_glass = datums["EXT_PSCOPE_INNER"]["global"]

        ##properties
        self.focal_distance = float(properties["focal_distance"])
        self.fv_left = float(properties["field_of_view_angle_left"])
        self.fv_right = float(properties["field_of_view_angle_right"])
        self.fv_down = float(properties["field_of_view_angle_down"])
        self.fv_up = float(properties["field_of_view_angle_up"])

        ## Get transform to focal point in vehicle space and it's inverse
        self.focal_point_trans = np.dot(self.trans_glass,
                                        translate(np.array([0, 0, -self.focal_distance])))
        self.focal_point_trans_inv = np.linalg.inv(self.focal_point_trans)

        ## Axis to rotate around to scan horizontally from focal point
        self.lens_hor_axis = np.array([0, 1, 0, 1])


    def is_ray_within_frustrum(self, h_ang, v_ang):
        """
        Return True if a ray projected at angles ``h_ang`` and ``v_ang`` is within the view frustrum
        of this vision device.
        """
        return h_ang < self.fv_left and h_ang > -self.fv_right and \
               v_ang < self.fv_down and v_ang > -self.fv_up


class Manikin(object):
    """ A person inside the vehicle. Only interested in where their eyes are pointing."""

    def __init__(self, manikin, m_data):
        """
        Setup the manikin using data from the supplied xml file.
        """
        
             
        
        ##get manikin properties from the dictionary returned by the api
        datums = m_data[manikin]["datums"]
        properties = m_data[manikin]["properties"]
        self.name = m_data[manikin]["name"]

        self.vehicle_role = properties["vehicle_role"]

        ##datums
        self.left_eye = datums["EXT_EYE_CSYS_LEFT"]["local"]
        self.right_eye = datums["EXT_EYE_CSYS_RIGHT"]["local"]

        ## This is the transform to the origin of the component in vehicle space
        trans_origin = m_data[manikin]["trans"]

        eye_rot = np.array([[1, 0, 0, 0],
                            [0, 1, 0, 0],
                            [0, 0, 1, 0],
                            [0, 0, 0, 1]])

        ## Get transform to eyes in vehicle space
        self.left_eye_veh = mul(trans_origin, self.left_eye, eye_rot)
        self.right_eye_veh = mul(trans_origin, self.right_eye, eye_rot)

        self.left_eye_veh_inv = np.linalg.inv(self.left_eye_veh)
        self.right_eye_veh_inv = np.linalg.inv(self.right_eye_veh)

        ## Axis to rotate around to scan horizontally from eye point (in eye csys)
        self.eyes_hor_axis = np.array([0, 1, 0, 1])

        ## This list will store the vision devices this manikin can interact with
        self.vision_devices = []


class Vision_Metrics(object):
    """ Calculates field of vision metrics. """
    def __init__(self, settings_f):
        """
        Setup vision metrics based on settings read in from file.
        """
        ##pass setting to the test bench api
        settings_dict = tba.load_settings(settings_f)

        ## Optional test bench settings
        self.show_3d = settings_dict.get("show_3d", False)
        horizontal_divs = settings_dict.get("horizontal_divs", 720)
        self.focal_length_multiplier = settings_dict.get("focal_length_multiplier", 1.5)
        self.far_dist = settings_dict.get("far_dist", 20.0)
        self.up_direction = settings_dict.get("up_direction", np.array([0, 1.0, 0]))
        self.fore_direction = settings_dict.get("fore_direction", np.array([0, 0, -1.0]))
        self.ground = settings_dict.get("ground", np.array([0, 1.0, 0]))

        ## Set up some constant parameters
        self.hor_sweep = np.linspace(0, 2 * pi, horizontal_divs)
        self.target_points_horizon = np.zeros(horizontal_divs, np.uint32)


        ## Get a dictionary of parts and properties form the api (periscopes, manikins, screens)
        fov_data = tba.get_data(parts_of_interest)

        ## Get Periscope objects for any that are found
        self.periscopes = [Vision_Device(p, fov_data["Periscope"]) for p in fov_data["Periscope"]]

        ## Get Manikin objects for the vehicle occupants that have vision requirements
        m_data = fov_data["Manikin"]
        
        req_roles = ["driver", "vehicle_commander", "troop_commander"]

        vis_roles = lambda m : m_data[m]["properties"]["vehicle_role"] in req_roles
        self.manikins = [Manikin(m, fov_data["Manikin"]) for m in m_data if vis_roles(m)]

        ## Need exactly one of each
        roles = [m.vehicle_role for m in self.manikins]
        if len(set(roles)) != len(req_roles):
            msg = "Didn't get 1 of each vehicle_role={} instead got={}".format(req_roles, roles)
            raise ValueError(msg)


        ## Specify the classes of objects we want geometry for.
        class_set = tba.get_all_geom_set() - tba.geom_sets["never_exterior_classes"]
        
        ## And load them all as an effective single file
        surface = tba.load_geometry(class_set, single_file=True)

        ## Make a binary space partition to speed up intersection calculations
        self.nodes = np.vstack((surface['x'], surface['y'], surface['z'])).T
        self.tris = surface['tris']
        self.b_tree = BSP_Tree(self.nodes, self.tris)
        self.b_tree.generate_tree()

        ## TODO this won't cope with unusual orientations
        ## determine orientation of the vehicle given the direction up and forward
        fore = np.dot(self.nodes, self.fore_direction).min()
        fore = np.min(surface["z"])
        self.side_direction = np.cross(self.up_direction, self.fore_direction)
        side_array = np.dot(self.nodes, self.side_direction)


        self.z_ground = np.dot(self.ground, self.up_direction)

        center = (side_array.min() + side_array.max())/2.0
        center = (np.min(surface["x"]) + np.max(surface["x"])) * 0.5
        ## Find the vehicle origin (point on ground at front on centerline)
        self.veh_origin = np.array([center, self.z_ground, fore])
        logging.info("Vehicle origin is at {}".format(self.veh_origin))
        self.tran_veh = translate(self.veh_origin)

        self.fore_aft = [None] * len(self.periscopes)
        self.uplook = np.array([-1000.0] * len(self.periscopes))

        self.hit = []

        self.hor_fans = []


    def _is_target_point_visible(self, target_point_trans, c_periscope):
        """
        Return True if the ``target_point_trans`` is visible from the current view device.
        """
        ## Target point in the focal point coord system
        t_pt_foc = get_translation(c_periscope.focal_point_trans_inv, target_point_trans)

        ## Calculate angle to target w.r.t to periscope view axis
        h_ang = atan2(t_pt_foc[0], t_pt_foc[2])
        rad_in_plane = sqrt(t_pt_foc[0] * t_pt_foc[0] + t_pt_foc[2] * t_pt_foc[2])
        v_ang = atan2(-t_pt_foc[1], rad_in_plane)

        ## Transform to project forward from the focal point to be slightly in front of the glass
        lens_dist = translate([0.0,
                               0.0,
                               c_periscope.focal_distance * self.focal_length_multiplier,
                               1.0])

        ## Check if within the periscope view frustrum
        if c_periscope.is_ray_within_frustrum(h_ang, v_ang):
            ## Calculate the lens offset point
            tran_hor = rotation_about_vector(c_periscope.lens_hor_axis, h_ang)
            tran_vert = rotation_about_vector(mul(tran_hor, c_periscope.lens_hor_axis), v_ang)
            lens_point = get_translation(c_periscope.focal_point_trans,
                                         tran_vert,
                                         tran_hor,
                                         lens_dist)

            ## Actually do the intersection here
            t = self.b_tree.get_line_intersection(lens_point, get_translation(target_point_trans))

            if t < 0:
                ## Visible
                return True
        ## Not visible
        return False


    def trace_rays(self):
        """
        Trace the rays to calculate the raw hit points for each device.
        """
        for p, periscope in enumerate(self.periscopes):

            hit = []
            hor_fan = []
            hor_fan.append(get_translation(periscope.trans_glass))

            dist_ver = get_translation(periscope.trans_glass)[1] - self.z_ground

            ## Sweep around the global up direction to find horizontal view percent
            for i, ang_hor in enumerate(self.hor_sweep):
                ## Global transform to target point
                tran_hor_world = rotation_about_vector(self.up_direction, ang_hor)

                target_point_trans =  mul(self.tran_veh,
                                          tran_hor_world,
                                          translate(np.array([self.far_dist, 0, 0])),
                                          translate(np.array([0, dist_ver, 0])))

                if self._is_target_point_visible(target_point_trans, periscope):
                    self.target_points_horizon[i] += 2 ** p
                    hor_fan.append(get_translation(target_point_trans))
                else:
                    hor_fan.append(get_translation(periscope.trans_glass))

            self.hor_fans.append(hor_fan)


            ## Find the highest visible point in front of the vehicle
            max_uplook = 50
            accuracy = 0.001
            self.uplook[p] = 0.0
            tran_hor_world = rotation_about_vector(self.up_direction, pi)
            upper_uplook = max_uplook
            lower_uplook = 0.0

            while (upper_uplook - lower_uplook) > accuracy:
                ## Global transform to target point
                height = (upper_uplook + lower_uplook) * 0.5

                target_point_trans =  mul(self.tran_veh,
                                          tran_hor_world,
                                          translate(np.array([0.0, height, 50.0])))

                if self._is_target_point_visible(target_point_trans, periscope):
                    self.uplook[p] = height
                    lower_uplook = height
                else:
                    upper_uplook = height

            if self.uplook[p] > 0.0:
                hit.extend((get_translation(target_point_trans),
                            get_translation(target_point_trans),
                            get_translation(periscope.trans_glass)))

            ## Find the closest visible ground point fore and aft
            max_radius = 2e6
            self.fore_aft[p] = [max_radius, max_radius]
            for i, rot in enumerate([pi, 0.0]):
                tran_hor_world = rotation_about_vector(self.up_direction, rot)
                upper_radius = max_radius
                lower_radius = 0.0

                while (upper_radius - lower_radius) > accuracy:
                    ## Global transform to target point
                    radius = (upper_radius + lower_radius) * 0.5

                    target_point_trans =  mul(self.tran_veh,
                                              tran_hor_world,
                                              translate(np.array([0, 0, radius])))

                    if self._is_target_point_visible(target_point_trans, periscope):
                        self.fore_aft[p][i] = radius
                        upper_radius = radius
                    else:
                        lower_radius = radius

                if self.fore_aft[p][i] < max_radius * 0.5:
                    hit.extend((get_translation(target_point_trans),
                                get_translation(target_point_trans),
                                get_translation(periscope.trans_glass)))

            self.hit.append(hit)


    def assign_to_manikins(self):
        """
        Assign periscopes to manikins (i.e. who can see what)
        """
        for man in self.manikins:
            for periscope in self.periscopes:
                visible = False
                for eye_inv_trans in (man.left_eye_veh_inv, man.right_eye_veh_inv):

                    mtrx_eye_to_glass = np.dot(eye_inv_trans, periscope.trans_inner_glass)
                    trans_eye_to_glass = mtrx_eye_to_glass[:3, 3]

                    resultant_z_into_glass = np.dot(mtrx_eye_to_glass, np.array([0, 0, -1, 0]))[:3]
                    unit_sight_line = normalize(trans_eye_to_glass)
                    sight_distance = sqrt(np.dot(trans_eye_to_glass, trans_eye_to_glass))
                    dot_prod = np.dot(resultant_z_into_glass, unit_sight_line)
                    theta = acos(dot_prod) * 180.0 / pi

                    if sight_distance < 1.0 and theta < 75.0:
                        visible = True
                        break
                man.vision_devices.append(visible)

            msg = "\nmanikin {} can see these devices:\n".format(man.name)
            for p in man.vision_devices:
                msg += "\t{}\n".format(p)
            logging.info(msg)


    def post_process(self):
        """
        Generate the metrics from the stored raw ray trace results.
        """
        out = {}
        for man in self.manikins:
            vis_mask = sum([2 ** p for p, vis in enumerate(man.vision_devices) if vis])
            horizon_fraction = 0.0
            num_pts = np.shape(self.hor_sweep)[0]
            for i in xrange(num_pts):
                if self.target_points_horizon[i] & vis_mask:
                    horizon_fraction += 1.0 / num_pts

            fore_aft_arr = np.array(self.fore_aft)

            pfx = man.vehicle_role
            if any(man.vision_devices):
                out.update({
                               pfx + "_horizon_percentage" : horizon_fraction,
                               pfx + "_fore_closest_visible_point" :
                                   np.min(fore_aft_arr[np.array(man.vision_devices), 0]),
                               pfx + "_aft_closest_visible_point" :
                                   np.min(fore_aft_arr[np.array(man.vision_devices), 1]),
                               pfx + "_max_uplook" :
                                   np.max(self.uplook[np.array(man.vision_devices)]),
                           })
            else:
                out.update({
                               pfx + "_horizon_percentage" : horizon_fraction,
                               pfx + "_fore_closest_visible_point" : "Fail",
                               pfx + "_aft_closest_visible_point" : "Fail",
                               pfx + "_max_uplook" : "Fail",
                           })

        ## Write results and echo to log.
        tba.write_results(out)

        if self.show_3d:
            self.post_process_3d()


    def post_process_3d(self):
        """
        Make optional 3D plots,
        """
        ## Import delayed till here so only need mayavi if 3D output is enabled.
        from mayavi import mlab

        for man in self.manikins:
            ## The devices this manikin is assigned to (i.e. can see).
            is_dev_viz = man.vision_devices

            ## Accumulate all points this manikin can see through all his assigned devices.
            hits = [h for hit, vis in zip(self.hit, is_dev_viz) for h in hit if vis]

            ## Plot horizon visibility fans and rays for max uplook and closest ground point.
            plots(hits, [hor for hor, v in zip(self.hor_fans, is_dev_viz) if v], mlab)

        ## Plot the vehicle as shaded surfaces.
        show_vehicle(self.veh_origin, self.nodes, self.tris, mlab)
        mlab.show()


colors = itertools.cycle([(1.0, 0.2, 0.2), (0.2, 0.2, 1.0), (0.2, 1.0, 0.2)])
ray_style = {"representation" : "mesh", "line_width" : 1.0, "tube_radius" : None}
fan_style = {"opacity" : 0.1}
pt_style = {"mode" : "2dvertex", "scale_mode" : "none", "scale_factor" : 1.2}


def plots(hit, hor_fans, mlab):
    """
    Plot rays from focal point to ``hit`` points.  Plot triangulated surface for each fan in
    ``hor_fans``.
    """
    color = next(colors)

    if hit:
        ## Use triangles to show mesh rays from the focal point to the hit point.
        hit_t = np.array(hit).transpose()
        tris = np.arange(hit_t.shape[1])
        tris.shape = (len(tris) / 3, 3)

        ## Add the plot to the scene.
        mlab.triangular_mesh(hit_t[0], hit_t[1], hit_t[2], tris, color=color, **ray_style)

    for i, hor_fan in enumerate(hor_fans):

        ## Generate semi transparent fans to highlight the horizontal feed of view.
        hf_t = np.array(hor_fan)

        ## Build the triangle connectivity to show the fans
        tris = np.zeros((hf_t.shape[0] - 1) * 3)
        for n in range(0, len(tris) / 3):
            tris[n * 3 + 1] = n
            tris[n * 3 + 2] = n + 1
        tris.shape = (len(tris) / 3, 3)

        ## Add the plot to the scene.
        mlab.triangular_mesh(hf_t[:, 0], hf_t[:, 1], hf_t[:, 2], tris, color=color, **fan_style)

        ## Generate points at the exterior of the fans to help highlight them.
        h_l = hf_t[1:-1]
        vec = h_l - hf_t[0]

        ## Make the points highlighting the arcs appear at different radii.
        h_l = hf_t[0] + vec * (1.0 + i / 8.0 + color[2] / 4)

        ## Add the plot to the scene.
        mlab.points3d(h_l[:, 0], h_l[:, 1], h_l[:, 2], color=color, **pt_style)


def show_vehicle(veh_origin, nodes, tris, mlab):
    """
    Show the vehicle as a triangulated surface and its origin as a sphere.
    """
    mlab.points3d([veh_origin[0]], [veh_origin[1]], [veh_origin[2]], [1.0],
                  color=(1.0, 0.5, 0.8), scale_mode="none", scale_factor=0.2)

    ## Add vehicle to scene
    mlab.triangular_mesh(nodes[:, 0], nodes[:, 1], nodes[:, 2], tris, color=(0.6, 0.5, 0.7))


if __name__ == "__main__":
    ## Executes the closest view test bench.

    logging.info("\n"+ 50 * "_" + "\ntest bench started\n" + 50 * "_")

    settings_file = r"settings.js"
    vis_metrics = Vision_Metrics(settings_file)
    logging.info("Made vis_metrics")

    vis_metrics.assign_to_manikins()
    logging.info("Assigned to metrics")

    vis_metrics.trace_rays()
    logging.info("Tracing rays")

    vis_metrics.post_process()
    logging.info("FoV test bench complete\n"+50*"_")




