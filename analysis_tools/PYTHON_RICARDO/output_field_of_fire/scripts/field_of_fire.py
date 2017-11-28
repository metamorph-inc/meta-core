"""
Test bench to assess field of fire around vehicle.
"""
from math import pi, ceil, sin, atan2, sqrt
import logging, itertools, sys, os, _winreg

import numpy as np
import matplotlib.pyplot as plt
from scipy.ndimage.morphology import grey_erosion

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
sys.path.append(os.path.join(tools['field_of_fire']['InstallLocation']))

from rpl.tools.ray_tracing.bsp_tree_poly import BSP_Tree
from rpl.tools.geometry.geom_utils import (rotation_about_vector, mul,
                                           get_translation, ray_plane_intersection)
from rpl.tools.api.plot_tools import vehicle_polar_axes
import rpl.tools.api.test_bench_api as tba


## xpath strings for extracting datums and properties from AVM format XML files
datum = r"//Datum[@DatumName = $da]"
prop = r"//Property[@Name = $pr]/Value/ValueExpression/Value"

parts_of_interest = {"Weapon_Station_Remote": {
                         "Datum": [
                             "EXT_WEAPON_MUZZLE",
                             "EXT_TURRET_ELEVATION",
                             "EXT_TURRET_TRAVERSE"
                                  ],
                         "Property":[
                             "angle_max_elevation",
                             "angle_max_depression",
                             "angle_traverse_if_not_continuous_neg_z",
                             "angle_traverse_if_not_continuous_pos_z",
                             "traverse_continuous"
                                    ],
                         "Required" : "{1}"}
                    }


class Weapon_Station(object):
    """ A device that can be used to fire at things. """

    def __init__(self, weapon_name, w_data):
        """
        Setup the weapon station using data from the supplied xml file.
        """
        ## Get weapon properties from the dictionary returned by the api.
        self.name = weapon_name

        ## Datums
        datums = w_data[weapon_name]["datums"]
        self.trans_trav = datums["EXT_TURRET_TRAVERSE"]["global"]
        self.trans_elev = datums["EXT_TURRET_ELEVATION"]["global"]
        self.trans_muzzle = datums["EXT_WEAPON_MUZZLE"]["global"]

        ## Properties
        props = w_data[weapon_name]["properties"]
        self.max_elev = props["angle_max_elevation"]
        self.max_depr = props["angle_max_depression"]
        self.max_trav_neg = props["angle_traverse_if_not_continuous_neg_z"]
        self.max_trav_pos = props["angle_traverse_if_not_continuous_pos_z"]
        self.trav_cont = props["traverse_continuous"]

        ## Need the translation from traverse to elevation point
        self.elev_from_trav = np.dot(np.linalg.inv(self.trans_trav), self.trans_elev)

        ## From muzzle to elevation point
        self.muz_from_elev = np.dot(np.linalg.inv(self.trans_elev), self.trans_muzzle)


class Field_Of_Fire_Metrics(object):
    """ Calculates field of fire metrics. """
    def __init__(self, settings_f):
        """
        Setup field of fire metrics based on settings read in from file.
        """
        ##pass setting to the test bench api
        settings_dict = tba.load_settings(settings_f)

        ## Optional test bench settings
        self.show_3d = settings_dict.get("show_3d", False)
        horizontal_divs = settings_dict.get("horizontal_divs", 720)
        self.up_direction = settings_dict.get("up_direction", np.array([0, 1.0, 0]))
        self.ground = settings_dict.get("ground", np.array([0, 1.0, 0]))

        ## Set up some constant parameters
        self.hor_sweep = np.linspace(0, 2 * pi, horizontal_divs)

        ## How many rays are considered contiguous - 3degrees of arc
        self.contiguous = int(ceil(3 * horizontal_divs / 360.0))
        self.incr_elev = 2 * pi / 720.0

        ## Get the data for the weapon.
        fof_data = tba.get_data(parts_of_interest)["Weapon_Station_Remote"]

        ## Get a weapon object.  API will enforce there is only one.
        wep_name = fof_data.keys()[0]
        self.weapon = Weapon_Station(wep_name, fof_data)

        ## Specify the classes of objects we want geometry for and load them in.
        class_set = tba.get_all_geom_set() - tba.geom_sets["never_exterior_classes"]
        surface = tba.load_geometry(class_set, single_file=True)

        ## Make a binary space partition to speed up intersection calculations
        self.nodes = np.vstack((surface['x'], surface['y'], surface['z'])).T
        self.tris = surface['tris']
        self.b_tree = BSP_Tree(self.nodes, self.tris)
        self.b_tree.generate_tree()

        ## Set up result storage.
        self.traverse_results = []


    def trace_rays(self):
        """
        Trace the rays to calculate the raw hit points for each device.
        """
        wep = self.weapon
        z_rot = np.array([0, 0, 1, 1])
        tran_traverse = np.eye(4)
        tran_elevation = np.eye(4)
        elev_angle = -wep.max_depr
        for rot in self.hor_sweep:
            logging.info("Scanning angle={}".format(rot))

            ## Rotate the weapon by traverse angle.
            tran_traverse = rotation_about_vector(z_rot, rot, tran_traverse)
            tran_elev_point = mul(wep.trans_trav, tran_traverse, wep.elev_from_trav)
            elev_point = get_translation(tran_elev_point)

            first = True
            good_shot = None
            while True:
                ## Rotate the weapon up or down by elevation angle.
                tran_elevation = rotation_about_vector(z_rot, elev_angle, tran_elevation)
                muz_point = get_translation(mul(tran_elev_point, tran_elevation, wep.muz_from_elev))

                ## Find out where shot would hit ground (or None if it won't hit the ground).
                gnd_hit = ray_plane_intersection(elev_point,
                                                 muz_point,
                                                 self.up_direction,
                                                 self.ground)

                ## Shot either 1) wouldn't hit the ground, 2) hit the ground or 3) hit the vehicle.
                if gnd_hit is not None:
                    ## Test if shot line cleared the vehicle.
                    t = self.b_tree.get_line_intersection(muz_point, gnd_hit)
                    shot = "cleared" if t < 0 else "collide"
                else:
                    ## Shot above the horizon
                    shot = "above_horizon"

                ## On first shot at this traverse angle determine if need to elevate or depress aim.
                if first:
                    first = False
                    if shot == "collide":
                        ## Need to try raising elevation
                        elev_change = self.incr_elev
                    elif shot == "cleared":
                        ## Need to try lowering elevation, but store because this might be the best.
                        elev_change = -self.incr_elev
                        good_shot = (muz_point, gnd_hit)
                    else:
                        elev_change = -self.incr_elev
                else:
                    if elev_change > 0.0:
                        ## elevation was being raised to find clearance
                        if shot == "cleared":
                            ## This is the closest shot possible, store it and stop looking.
                            good_shot = (muz_point, gnd_hit)
                            break
                        elif shot == "above_horizon" or elev_angle > wep.max_elev:
                            ## Missed the ground or exceeded weapon elevation.
                            break
                    else:
                        ## elevation was being lower to find a closer shot
                        if shot == "collide" or elev_angle < -wep.max_depr:
                            ## Must have already stored the best shot previously.
                            break
                        else:
                            ## Store this shot but keep looking.
                            good_shot = (muz_point, gnd_hit)

                elev_angle += elev_change


            if good_shot is not None:
                self.traverse_results.append((rot, good_shot))
            else:
                self.traverse_results.append((rot, None))


    def post_process(self):
        """
        Generate the metrics from the stored raw ray trace results.
        """
        data = self.traverse_results
        ## Check raw traverse results for continguous regions of no ground coverage.
        none_runs = [len(list(grp)) for k, grp in itertools.groupby(data, lambda x:x) if k is None]

        if none_runs and max(none_runs) >= self.contiguous:
            ## Fail because there is a contiguous region larger than allowed
            fwd = "Fail"
            aft = "Fail"
            no_fire_area = "Fail"
        else:
            trav_cen = get_translation(self.weapon.trans_trav)

            angles = np.array([x[0] for x in data if x[1] is not None])
            muz_pts = np.array([x[1][0] for x in data if x[1] is not None])
            gnd_pts = np.array([x[1][1] for x in data if x[1] is not None])

            ## Distance along the ground
            gnd_dist = np.sqrt((trav_cen[0] - gnd_pts[:, 0]) ** 2 +
                               (trav_cen[2] - gnd_pts[:, 2]) ** 2)

            ## Calculate the minimum shoot distance over contiguous regions
            mod_dist = grey_erosion(gnd_dist, self.contiguous, mode="wrap")

            ## Calculate an area metric (A = 0.5 * a.b.sin(c_ang))
            num_tris = len(angles)
            no_fire_area = 0.0
            for i in xrange(num_tris):
                next_ang = (angles[0] + 2 * pi) if i + 1 >= num_tris else angles[i+1]
                c_ang = next_ang - angles[i]
                a = gnd_dist[i]
                b = gnd_dist[(i + 1) % num_tris]
                no_fire_area += 0.5 * a * b * sin(c_ang)

            ## Calculate min forward and aft shoot distance.
            fwd = np.max(mod_dist[np.where((angles >= 3 * pi / 2) | (angles <= pi / 2))])
            aft = np.max(mod_dist[np.where((angles <= 3 * pi / 2) & (angles >= pi / 2))])

            self.post_process_2d(angles, gnd_dist, mod_dist, no_fire_area, fwd, aft)

            if self.show_3d:
                self.post_process_3d(muz_pts, gnd_pts)

        ## Write results to json file and echo to log.
        out = {
                  "min_fire_dist_fore_180" : fwd,
                  "min_fire_dist_aft_180" : aft,
                  "no_fire_area" : no_fire_area
              }
        tba.write_results(out)


    def post_process_2d(self, angles, gnd_dist, mod_dist, area, fwd, aft):
        """
        Save a 2d figure to illustrate the no-fire region and the metrics.
        """
        ## Setup polar plot with textual labels rather than numerical
        ax = vehicle_polar_axes()

        ## Plot a line for the raw data and a filled region for the processed data.
        ax.plot(angles, gnd_dist, color="r", alpha=0.3)
        ax.fill(angles, mod_dist, color="r", alpha=0.5)

        ## Show semi-circles for the fore and aft closest point metrics.
        for offset, val in [(0, fwd), (pi, aft)]:
            phi = np.linspace(offset - pi / 2, offset + pi / 2, 180)
            r = np.ones_like(phi) * val
            ax.fill(phi, r, color="k", alpha=0.1)
            ax.plot(phi, r, color="k", linewidth=2)

        ## Add the metrics text.
        ax.text(-pi / 4, fwd + 2, "{}[m]".format(round(fwd, 1)), va="bottom")
        ax.text(pi + pi / 4, aft + 2, "{}[m]".format(round(aft, 1)), va="top")
        ax.text(0.0, fwd + 10, "No Fire Area = {}[m2]".format(int(area)), ha="center", color="r")

        ## Add the vehicle as a bounding box.
        trav_cen = get_translation(self.weapon.trans_trav)
        l_r = trav_cen[0] - np.min(self.nodes[:, 0]), trav_cen[0] - np.max(self.nodes[:, 0])
        f_b = trav_cen[2] - np.min(self.nodes[:, 2]), trav_cen[2] - np.max(self.nodes[:, 2])

        veh = np.array([(atan2(x, y), sqrt(x * x + y * y)) for x, y in itertools.product(l_r, f_b)])
        ax.fill(veh[[0, 1, 3, 2, 0], 0], veh[[0, 1, 3, 2, 0], 1], color="k", alpha=0.4)

        plt.savefig("field_of_fire_pic.png")


    def post_process_3d(self, muz_pts, gnd_pts):
        """
        Make optional 3D plots,
        """
        ## Import delayed till here so only need mayavi if 3D output is enabled.
        from mayavi import mlab

        ## Plot closest shot lines
        plot_shot_lines(muz_pts, gnd_pts, mlab)

        ## Plot the vehicle as shaded surfaces.
        show_vehicle(self.nodes, self.tris, mlab)
        mlab.show()


ray_style = {"representation" : "mesh", "line_width" : 2.0, "tube_radius" : None, "opacity" : 0.07}
ln_style = {"tube_radius" : None, "line_width" : 2.0}

def plot_shot_lines(muz_pts, gnd_pts,  mlab):
    """
    Plot rays from ``muz_pts`` to ``gnd_pts``.  Plot tubes to show the sweep of each set of points.
    """
    color = (1.0, 0.2, 0.2)

    ## Draw lines representing the muzzle sweep and the ground sweep.
    mlab.plot3d(muz_pts[:, 0], muz_pts[:, 1], muz_pts[:, 2], color=(0.6, 0.2, 0.6), **ln_style)
    mlab.plot3d(gnd_pts[:, 0], gnd_pts[:, 1], gnd_pts[:, 2], color=(0.6, 0.2, 0.6), **ln_style)

    n_p = len(muz_pts)

    hit_t = np.concatenate((muz_pts, gnd_pts))

    ## Create two triangles for each point to make a rectangle
    t1 = np.array([[i, i + n_p, (i + 1) % n_p] for i in xrange(n_p)])
    t2 = np.array([[(i + 1) % n_p, i + n_p, (i + n_p + 1) % (2 * n_p)]  for i in xrange(n_p)])
    tris = np.concatenate((t1, t2))

    ## Add the plot to the scene.
    mlab.triangular_mesh(hit_t[:, 0], hit_t[:, 1], hit_t[:, 2], tris, color=color, **ray_style)


def show_vehicle(nodes, tris, mlab):
    """
    Show the vehicle as a triangulated surface.
    """
    ## Add vehicle to scene
    mlab.triangular_mesh(nodes[:, 0], nodes[:, 1], nodes[:, 2], tris, color=(0.6, 0.5, 0.7))


if __name__ == "__main__":
    ## Executes the field of fire test bench.

    logging.info("\n"+ 50 * "_" + "\ntest bench started\n" + 50 * "_")

    settings_file = r"settings.js"
    fof_metrics = Field_Of_Fire_Metrics(settings_file)
    logging.info("Made fof_metrics")

    logging.info("Tracing rays")
    fof_metrics.trace_rays()

    fof_metrics.post_process()
    logging.info("FoF test bench complete\n"+50*"_")




