"""
Run test benches related to ingress/egress for a vehicle assembly model specified in settings.
"""
import sys
import os
import _winreg
from scipy.ndimage import measurements as meas


def query_analysis_tools():
    """
    Find the location of the Ricardo Python library.
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

tools = query_analysis_tools()
sys.path.append(os.path.join(tools['ingress_egress']['InstallLocation']))

## Add the scripts folder itself to the python path.
#sys.path.append(os.path.split(__file__)[0])

import logging
import numpy as np

from rpl.tools.api import test_bench_api as tba
from rpl.tools.geometry import geom_utils

import uniform_grid, check_intersection, best_poses_at_position, exit_time_nx, escape_routes_ball
import data_io
from voxel_methods import Vehicle


def get_parts_of_interest():
    """
    Information required for test bench to run
    """
    door_prms = {"Datum": ["EXT_HULL_APERTURE"],
                 "Required": "{*}"}

    parts_of_interest = {"Manikin": {"Datum": ["SWRI_H_POINT", "BOOT_LEFT_CSYS"],
                                     "Property": ["vehicle_role"]},
                         "Hatch_Assembly_Rear_Ramp": door_prms,
                         "Hatch_Assembly_Personnel_Door": door_prms}
    # TODO: ask for litter_open csys. TB should fail if there are no litters in model.
    return parts_of_interest


def get_parts_excluded():
    """
    Lists parts explicitly excluded from geometry. For example, we don't have seated
    manikins in a simulation of troops exiting the vehicle.

    We can still request parameters and datums for these parts.
    """
    return {'Manikin'}


class Assembly_Info(object):
    """
    Perform api calls and make vehicle/door/troop data available
    """
    HULLS = {"Hull_Assembly_Parametric", 'Hull_Assembly_Example_With_Connector'}
    DOORS = {'Hatch_Assembly_Rear_Ramp', 'Hatch_Assembly_Personnel_Door'}
    HATCHES = {'Hatch_Assembly_Driver_Commander', 'Hatch_Assembly_Cargo'}
    MANIKINS = {'Manikin'}
    LITTERS = {'Litter_Open'}

    # Special labels applied to specific types of voxels
    VOXEL_LABELS = {2: HULLS,
                    4: DOORS,
                    8: HATCHES,
                    16: MANIKINS,
                    32: LITTERS}

    def __init__(self, settings_dict):
        self.settings_dict = settings_dict

        self.desired_params = tba.get_data(get_parts_of_interest())

        self._up_vector = None
        self._tr_mat = None
        self._vehicle_stl = None
        self._litter_stl = None

        self._doors_xyz = None

        self.voxel_data = None

    def get_doors_xyz(self):
        """
        Provide the xyz coordinates for each door point found
        """
        # iterate over self.doors and get ext_hull_aperture csys for each point; convert to 3d
        # coord vector
        if self._doors_xyz is None:
            self._doors_xyz = []
            for d in self.DOORS:
                # Get every door in every type
                for k in self.desired_params[d]:
                    # Convert 4x4 matrix to 3d point coord vector (last column of trans matrix)
                    tr_mat = self.desired_params[d][k]['datums']['EXT_HULL_APERTURE']['global']
                    pt = tr_mat[:-1, 3]
                    self._doors_xyz.append(pt)

        assert len(self._doors_xyz) > 0, "No doors found in vehicle; cannot perform egress test"
        return self._doors_xyz

    def get_geometry(self):
        """
        Get relevant geometry for vehicle
        """
        if self._vehicle_stl is None:
            all_geom = tba.get_all_geom_set()
            desired_parts = (all_geom - get_parts_excluded()
                             - tba.geom_sets["never_in_cabin"]
                             - self.LITTERS
                             | self.DOORS)
            self._vehicle_stl = tba.load_geometry(desired_parts,
                                                  single_file=False)
        return self._vehicle_stl

    def get_litter(self):
        """
        Get geometry of just the litter
        """
        if self._litter_stl is None:
            self._litter_stl = tba.load_geometry(self.LITTERS,
                                                 single_file=True)
        return self._litter_stl

    def get_vertical(self):
        """
        Return a vector corresponding to the up vector in the vehicle. Determined from calculated
         metrics file. Normalize it to unit length to be safe.
        """
        if self._up_vector is None:
            try:
                self._up_vector, _, _ = tba.get_up_vector()
                logging.debug("Used up vector based on ComputedValues.xml from GME assembly")
            except ValueError:
                wrn = """
                      WARNING: Requires a known vector pointing up. Hard-coded value is (0,1,0),
                      implying that the up direction lies along y axis.
                      """
                self._up_vector = np.array([0, 1, 0])
                logging.warning(wrn)
        self._up_vector /= np.linalg.norm(self._up_vector)
        return self._up_vector

    def get_manikin_positions(self, datum_name="SWRI_H_POINT", veh_role=None):
        """
        Process the test bench data to extract manikin positions.
        Rotation should be handled by voxel_methods objects, so this outputs orig vehicle coords.

        :param datum_name: Get positions based on a specific datum
        :param veh_role: Get only manikins with a specific vehicle role
        Common vehicle roles to narrow things down:
            ("driver", "gunner", "troop", "troop_commander", "vehicle_commander")
        """
        manikins = self.desired_params['Manikin']
        mani_list = [{'name': k, 
                      'vehicle_role': manikins[k]['properties']['vehicle_role'],
                      datum_name: manikins[k]['datums'][datum_name]['global']}
                     for k in manikins]
        
        # API returns a 4x4 matrix marking a csys. The last column (translation) is manikin pos.
        if veh_role is not None:
            # If vehicle role specified, filter list by role
            return [m[datum_name][:-1, 3] for m in mani_list if m['vehicle_role'] == veh_role]
        else:
            return [m[datum_name][:-1, 3] for m in mani_list]

    def get_vehicle_csys(self):
        """
        Create a mock vehicle coordinate system based on the calculated properties/metrics file

        Represents the rotation that would be required to align vehicle csys with +z for occupied 
        voxel grid. (no translation component)
        """
        if self._tr_mat is None:
            # Rotate around a vector mutually orthogonal to the current and desired "up" vectors
            upvec = self.get_vertical()
            rot_around = np.cross(upvec, np.array([0, 0, 1]))
            rot_ang = -np.arccos(upvec[2])
            self._tr_mat = geom_utils.rotation_about_vector(rot_around, rot_ang)
        return self._tr_mat

    def voxelate_geometry(self):
        """
        Perform voxelization. Debug option may choose to call this only if pre-existing results
        file does not already exist.
        """
        self.voxel_data = uniform_grid.main(self.get_geometry(), 
                                            self.get_vehicle_csys(),
                                            self.VOXEL_LABELS, 
                                            self.settings_dict)
        return self.voxel_data

    def voxelate_litter(self, xg, yg, zg):
        """
        Voxelate the litter separately, on a grid of same dimensions as rest of vehicle
        """
        litter_data = {"x_grid": xg,
                       "y_grid": yg,
                       "z_grid": zg,
                       "vox_size": SETTINGS["voxel_size"],
                       "csys_trans": self.get_vehicle_csys(),
                       "value": None}

        litter_geom = self.get_litter()
        # Crude check: if no litter geom found, would get a dictionary with keys for x,y,z, tris...
        #  but no coordinates in those lists. (Not an empty dict b/c single_file= True)
        has_geom = len(litter_geom['tris']) != 0

        if has_geom:
            litter_geom = uniform_grid.convert_geom(litter_geom, self.get_vehicle_csys())
            litter_data = uniform_grid.find_occupied_voxels(litter_geom, 1, litter_data)

            # Ensures that the voxelated litter grid gets saved alongside other voxelated data
            self.voxel_data['litter'] = litter_data['value']

        return has_geom  # Can be used to detect if geom didn't load correctly


def tb_033(assm, stg, veh):

    ## Assess test bench criteria using pathfinding calculations
    logging.info("Starting pathfinding routines")

    ## Get all troop positions from different roles.
    troop_coords = assm.get_manikin_positions("BOOT_LEFT_CSYS", "troop")
    troop_coords += assm.get_manikin_positions("BOOT_LEFT_CSYS", "troop_commander")

    start_pts_troops_2d = []
    for p in troop_coords:
        # Only troops are considered for egress time; use 3-tuples of (x, y, orie#)
        p = veh.get_vox_from_coord(p)
        # TODO: The specified pose/position is not guaranteed to be walkable and no checks are
        # performed.
        start_pts_troops_2d.append((p[0], p[1], 4))

    assert len(start_pts_troops_2d) > 0, "No troop positions found"
    logging.debug("Troop starting voxels are: {}".format(start_pts_troops_2d))

    door_points = []
    for p in assm.get_doors_xyz():
        p = veh.get_vox_from_coord(p)
        door_points.append((p[0], p[1], 4))

    logging.debug("Door point locations: ")
    logging.debug(door_points)
    #door_points = [(38, 13, 4)]  # Hard-coded pt on ricardo seed model nov 2013- for testing only

    graph, overall_walk_scores = exit_time_nx.load_create_graph(stg)

    logging.info("Created directional weighted graph")

    # Ensure that at least one point (near the walkable area perimeter) is connected to the door.
    graph = exit_time_nx.connect_doors(graph, overall_walk_scores, door_points, stg)

    ## TB033: exit time
    total_exit_pen = exit_time_nx.total_exit_time(graph, start_pts_troops_2d, door_points)

    ## Convert penalty score into a time based on avg normal speed (1m/s) and voxel size
    res = total_exit_pen * stg["voxel_size"] / 1.0

    if stg['show_2d']:
        # Make a 2D plot of the exit paths through vehicle for all troops
        exit_time_nx.plot_paths(graph, start_pts_troops_2d, door_points[0], overall_walk_scores,
                                stg)

    logging.info("Test bench TB033 completed. Worst exit time= {}".format(res))

    return res


def tb_034_035_exit_checks(assm, veh):
    """
    Perform checks for whether certain types of soldiers can reach a hatch and a door.
    """
    ## Get all crew positions from related roles
    crew_coords = assm.get_manikin_positions("BOOT_LEFT_CSYS", "driver")
    crew_coords += assm.get_manikin_positions("BOOT_LEFT_CSYS", "gunner")
    crew_coords += assm.get_manikin_positions("BOOT_LEFT_CSYS", "vehicle_commander")

    # Only crew are considered for emergency egress. Use 3-tuples of 3d voxel indices (i,j,k)
    start_points_crew_3d = [veh.get_vox_from_coord(p) for p in crew_coords]
    assert len(start_points_crew_3d) > 0, "No crew members found"
    logging.debug("Crew starting voxels are: {}".format(start_points_crew_3d))

    ext_chk = escape_routes_ball.main(veh, start_points_crew_3d)
    logging.info("Performed exit availability checks")

    return ext_chk


def tb_034(ext_chk):
    """
    TB034: Emergency egress. Check that every troop has a route to at least one door AND at
        least one hatch
    """
    logging.info("Starting crew exit availability assessment")
    res = all(v.values() for v in ext_chk.values())
    logging.info("Test bench TB034 completed. Egress check status: {}".format(res))
    return res


def tb_035(ext_chk):
    """
    TB035: TB035 checks whether all crew members can be evacuated just through a door. This reuses
      the exit checks performed above- hence why it returned hatch and door info separately.
    """
    logging.info("Starting crew evacuation assessment")
    res = all(v["door"] for v in ext_chk.values())
    logging.info("Test bench TB035 completed. Evac check status: {}".format(res))
    return res


def tb_036(assm, veh, stg):
    """
    Run litter evaluation TB036. Checks whether litter voxels would collide with vehicle voxels.
    """
    logging.info("Starting litter position assessment")

    # The open litter model is enclosed in a hollow box defining the allowed boundaries. We need
    #    the whole volume to be solid for collision checks.
    # When voxelated on the vehicle grid, the litter boundaries will not be the largest region on
    #   a labeled grid. Hence we label, discard anything in the largest labeled region, and then
    # combine the labeled regions (empty air inside hollow box) with the areas occupied by litter
    # itself.
    labels, count = meas.label(assm.voxel_data['litter'] == 0)
    bins = np.bincount(labels.ravel())
    labels[labels == np.argmax(bins)] = 0  # Wipe out any labels assigned to empty space
    labels[labels != 0] = 1  # Label anything left as part of the continuous litter "box"
    occ_litter = labels | assm.voxel_data['litter']

    # Max collision: in testing, one layer of litter colliding with floor was ~11% collisions
    #   (failure). Make allowed collisions for litters (and only litters) more generous.
    litter_collision = (veh.occupied_voxels != 0) & (occ_litter != 0)
    litter_collision_count = np.sum(litter_collision)
    litter_vox_count = np.sum(occ_litter)
    max_coll_allowed = stg['max_collision_pct'] * 1.5 / 100. * litter_vox_count

    res = True if litter_collision_count < max_coll_allowed else False

    logging.info("Litter collisions: {:.2%}".format(float(litter_collision_count) /
                                                    float(litter_vox_count)))
    logging.info("Test bench TB036 completed. Vehicle has room for litter: {}".format(res))
    return res


if __name__ == "__main__":
    #############

    # Initialize API with settings
    SETTINGS = tba.load_settings("settings.js")

    # Value references numpy; can't be read from json
    SETTINGS["unwalkable_value"] = np.inf

    #######
    #  Call test bench api to obtain desired information
    assembly = Assembly_Info(SETTINGS)

    #######
    #  Voxelize the vehicle and return all information
    # Save the voxelated model of the vehicle (sans door and other excluded parts)

    vox_veh_folder = r"voxelated_models/vehicles/{}/{}".format(SETTINGS["run_id"],
                                                               SETTINGS["voxel_size"])

    vox_veh_file = "voxels_{}_vox{}".format(SETTINGS["run_id"], SETTINGS["voxel_size"])

    if SETTINGS["debug"] and os.path.exists(os.path.join(vox_veh_folder, vox_veh_file + '.npz')):
        # Load from pre-existing file if possible (if debugging option is on)
        assembly.voxel_data = data_io.load_array(vox_veh_folder, vox_veh_file, multi_array=True)
        logging.info("Vehicle voxel data loaded from preexisting file {}".format(vox_veh_file))

        valid_litter_found = "litter" in assembly.voxel_data  # Does cached geom incl litter?
    else:
        logging.info('Commencing voxelization of vehicle model')
        assembly.voxelate_geometry()

        # Voxelate Litter_Open model on a grid of same size
        x_pts, y_pts, z_pts = [assembly.voxel_data[k] for k in ("x_grid", "y_grid", "z_grid")]
        valid_litter_found = assembly.voxelate_litter(x_pts, y_pts, z_pts)

        data_io.save_multi_array(vox_veh_folder, vox_veh_file, assembly.voxel_data)
        logging.info("Vehicle successfully voxelated.")

    # All later operations act on pre-voxelated models. Create voxel objects with redundant info.
    vehicle = Vehicle(assembly.voxel_data, vehicle_csys=assembly.get_vehicle_csys())

    # Find exit points (hatches + where rear doorway and floor intersect)
    # The door and floor may never intersect at all; instead, use the intersection between the
    # voxelated arrays, vehicle_interior w/ door.
    vox_veh_cabin = vehicle.get_vox_from_coord(
        assembly.get_manikin_positions(datum_name="SWRI_H_POINT", veh_role="troop")[0])

    vehicle_interior = vehicle.get_labels(mask_from_voxel=vox_veh_cabin)
    # Run floor and ceil finding routines to prepare object for later scripts. Finding floor
    # automatically also gets ceiling.
    vehicle.get_floor(vox_veh_cabin)

    logging.info("Vehicle floor, ceiling, and interior successfully labeled")

    if not SETTINGS['debug']:
        # Skip slow intersection checks if in debug mode
        ## Intersection checks
        for pose_name in SETTINGS["manikins_to_use"]:
            logging.info("Starting intersection checks for {}".format(pose_name))
            check_intersection.main(pose_name, vehicle, settings=SETTINGS)
        logging.info("All intersection checks complete")

        ##  Determine best pose at each position (saves output to files as needed)
        best_poses_at_position.main(SETTINGS)
        logging.info("Calculated best pose of manikin overall at each point in vehicle")

    # TB033: Exit time calculations
    total_exit_time = tb_033(assembly, SETTINGS, vehicle)

    # Exit availability checks: TB034 and 035
    exit_checks = tb_034_035_exit_checks(assembly, vehicle)
    logging.info("Detailed list of exit availability results: ")
    logging.info(exit_checks)

    exit_ok = tb_034(exit_checks)
    evac_ok = tb_035(exit_checks)

    ## TB036: Does litter fit in the assigned spot without collisions
    if valid_litter_found:
        litter_status = tb_036(assembly, vehicle, SETTINGS)
    else:
        # Let remainder of TB fail when no litter is present, but failing metric is meaningless
        logging.error("Litter evaluation could not be run, because no Litter_Open was found")
        litter_status = False

    ### Combine results for final output
    tb_results = {"total_exit_time_tb033": total_exit_time,
                  "egress_overview_passed_tb034": exit_ok,
                  "crew_evac_passed_tb035": evac_ok,
                  "litter_fits_tb036": litter_status}

    # Write JSON file with results
    tba.write_results(tb_results)

    logging.info("All egress test bench calculations complete. Exiting.")