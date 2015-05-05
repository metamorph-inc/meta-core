"""
Determines whether a soldier can exit the vehicle, based on dragging a ball of set radius through
    the vehicle. If no path that can fit the ball exists, the soldier cannot exit.
"""

import logging, os
import numpy as np

from scipy import ndimage
import data_io


## Define labels used for various vehicle parts. Must be kept in sync with the labels used in
# ingress_egress.py (and applied to the voxelated model).
VL_NONE, VL_ANY, VL_HULL, VL_DOORS, VL_HATCHES, VL_MANIKINS = 0, 1, 2, 4, 8, 16


class Exit_Tunnel(object):
    """
    Given a voxelated vehicle model and a set of manikin positions (ijk vox indices), use image
    dilation to locate possible paths to the exit
    """
    def __init__(self, vehicle_obj, mani_pts_ijk, ball_radius_vox=5):
        self.voxel_data = vehicle_obj.occupied_voxels
        self.mani_pts = mani_pts_ijk

        # Get the ball that will be dragged through the object to determine ok-sized paths
        self.ball = None
        self.make_ball_struct(ball_radius_vox)

        # Label interior regions of voxelated grid, then mark mani pts on grid of same size
        self.air_zones = None
        self.mani_grid = None
        self.get_air()

        ### Fine tuning done for arrays regardless of which exit type is considered
        self.mani_grid = ndimage.binary_dilation(self.mani_grid, self.ball)

        ## Find the set of labels that contain manikins.
        self.interior_labels = set(self.air_zones[tuple(v)] for v in self.mani_pts)

        ## Make a new air voxel array for only the air regions containing manikins.
        self.air_inside = np.in1d(self.air_zones, list(self.interior_labels)).reshape(
            self.air_zones.shape)

    def make_ball_struct(self, radius):
        """
        Make a voxelated ball with ``radius`` voxels either side of the center voxel.

        Returns a numpy array of ``shape = (cells, cells, cells)`` where ``cells = radius * 2 + 1``.
        """
        cells = radius * 2 + 1
        ball = np.zeros((cells, cells, cells), dtype=np.bool)
        ball_i = np.where(ball == False)
        for i, j, k in zip(*ball_i):
            i_o = float(abs(i - radius))
            j_o = float(abs(j - radius))
            k_o = float(abs(k - radius))
            if np.sqrt(i_o**2 + j_o**2 + k_o**2) < (radius + 0.5):
                ball[i, j, k] = True

        self.ball = ball

    def get_air(self):
        """
        Locate the region of the interior cabin volume that is air
        """
        air = self.voxel_data == VL_NONE

        ## Label every disconnected air zone.  (Will include air inside and outside vehicle)
        air_zoned, num_zones = ndimage.label(air)

        ## Set troop starting points and create a voxel array to represent them.
        manis = np.zeros_like(air, dtype=np.bool)
        for v in self.mani_pts:
            manis[tuple(v)] = True

        self.mani_grid = manis
        self.air_zones = air_zoned

    def get_exits(self, exit_type="door"):
        """
        Get the location of exit points for each exit of ``exit_type`` (door, hatch, both)
        """

        # Set the correct label to look for
        if exit_type == "door":
            find_label = VL_DOORS
        elif exit_type == "hatch":
            find_label = VL_HATCHES
        elif exit_type == "both":
            find_label = (VL_DOORS | VL_HATCHES)
        else:
            raise Exception("Exit route checker requested unknown exit type: {}".format(exit_type))

        exits = (self.voxel_data & find_label) != 0

        ## Label every separate exit.
        exit_labels, num_exits = ndimage.label(exits)

        ## For each labelled exit store 1 ijk coord contained within it.
        exit_points = []
        for e in xrange(1, num_exits + 1):
            loc_index = np.argmax(exit_labels == e)
            loc_crds = np.unravel_index(loc_index, exit_labels.shape)
            exit_points.append(loc_crds)

            #locs = np.vstack(np.where(exit_labels == e)).T
            #exit_points.append(locs[0])
        exit_points = np.array(exit_points)
        logging.debug("EXIT POINTS of type {}: {}".format(exit_type, exit_points))

        return exits, exit_points

    def find_path_to_exits(self, exit_type="door", debug=False):
        """
        Find whether a path exists between a given position and a given exit,
        """
        # Results placeholder
        exit_results = {tuple(p): False for p in self.mani_pts}

        # Find exit points for exit of specified type
        exits, exit_points = self.get_exits(exit_type=exit_type)
        exits_dilated = ndimage.binary_dilation(exits, self.ball)

        ## Make an "outline" of voxels surrounding the air inside.  This will be "open" where air
        ## inside meets the exits.
        eroded = ndimage.binary_erosion(self.air_inside | exits_dilated)
        outline = self.air_inside & ~eroded

        ## Dilate outline using ball, making a voxel grid that ball.radius away from any surface.
        dil_out = ndimage.binary_dilation(outline, self.ball)

        ## Cut away from air_inside the dilated outline but add back the troop locations.
        air_voxgrid = (self.air_inside & ~dil_out) | self.mani_grid

        ## Finally label a composite of the remaining air inside and the solid exits.
        ## If 2 ijk points have the same label they must be connected.
        air_labels, air_nums = ndimage.label(air_voxgrid | exits)

        for pt in self.mani_pts:
            logging.info("Troop at '{}' can reach exits:".format(pt))
            pt_lab = air_labels[tuple(pt)]
            for ex in exit_points:
                if air_labels[tuple(ex)] == pt_lab:
                    logging.info("\t'{}'".format(ex))
                    exit_results[tuple(pt)] = True

        if debug is True:
            return exit_results, exits, air_voxgrid
        else:
            return exit_results


def main(vehicle_obj, mani_pts_ijk, ball_radius_vox=5, debug=False):
    """
    Find whether a path exists between mani position and exits of ``exit_type`` (can be "door",
    "hatch", or "both")

    Returns additional arrays (for 3d viewing) if ``debug`` is True
    """
    # Keep track of whether each starting point has paths to hatch and door. Track separately
    # for compatibility with TB034 + TB035
    all_exit_checks = {tuple(p): {"door": False, "hatch": False}
                       for p in mani_pts_ijk}

    tunnel_checker = Exit_Tunnel(vehicle_obj, mani_pts_ijk, ball_radius_vox=ball_radius_vox)

    ### Do exit calculations for specific types of exit
    can_reach_door = tunnel_checker.find_path_to_exits(exit_type="door")
    can_reach_hatch = tunnel_checker.find_path_to_exits(exit_type="hatch")

    # Store overall results
    for k in all_exit_checks:
        all_exit_checks[k]["door"] = can_reach_door[k]
        all_exit_checks[k]["hatch"] = can_reach_hatch[k]

    if debug is True:
        # Return results suitable for rendering (showing ability to reach both doors)
        can_reach_both, exits, air_vox = tunnel_checker.find_path_to_exits(exit_type="both",
                                                                           debug=True)
        return all_exit_checks, exits, air_vox
    else:
        return all_exit_checks

if __name__ == "__main__":
    import json
    from voxel_methods import Vehicle

    ################
    # Setup
    # Load the settings file directly; no api functionality is needed for this script
    with open(r"settings.js", "r") as f:
        SETTINGS = json.load(f)

    ## Print log INFO and above to the console
    logging.basicConfig(filename="debug_escape_routes_ball.log",
                        filemode="w",
                        format="%(levelname)s %(asctime)s %(message)s",
                        level=logging.DEBUG)

    console = logging.StreamHandler()
    console.setLevel(logging.INFO)
    console.setFormatter(logging.Formatter("%(asctime)s %(message)s"))
    logging.getLogger("").addHandler(console)

    ##########
    # Load the pre-voxelated vehicle model for testing
    b_fldr = r"voxelated_models/vehicles"
    vox_size_str = str(SETTINGS["voxel_size"])
    vox_folder = os.path.join(b_fldr, SETTINGS["run_id"], vox_size_str)
    vox_veh_file = "voxels_{}_vox{}".format(SETTINGS["run_id"], vox_size_str)
    voxel_dat = data_io.load_array(vox_folder, vox_veh_file, True)
    vehicle = Vehicle(voxel_dat)

    ### Hardcoded dummy troop positions that may be meaningless. Uses 3D voxel indices. These
    # values are for testing only, based on the Ricardo master assembly 11/11/2013.
    crew_points = np.array([[40, 40, 25], [40, 80, 25], [20, 145, 40]])

    # Perform exit checks and return combined results
    can_exit, exit_grid, air_inside = main(vehicle, crew_points, debug=True)

    # Print exit status: do all points pass exit check?
    exit_ok = all(v.values() for v in can_exit.values())

    if SETTINGS["debug"]:
        ## Render a 3D image, for debugging purposes only
        from mayavi import mlab

        class Voxel_Plotter(object):
            """
            Plotting class (for use with matlab, visualization/debug purposes only)
            """
            def __init__(self, voxel_data):
                self.voxel_data = voxel_data

            def __call__(self, grid, color, opacity=1.0):

                if grid.dtype == np.bool:
                    xo, yo, zo = np.where(grid)
                else:
                    xo, yo, zo = grid[:, 0], grid[:, 1], grid[:, 2]
                mlab.points3d(self.voxel_data["x_grid"][xo],
                              self.voxel_data["y_grid"][yo],
                              self.voxel_data["z_grid"][zo],
                              color=color,
                              scale_mode="none",
                              scale_factor=self.voxel_data["vox_size"],
                              mode='cube',
                              opacity=opacity)

        # Make an outline of the air_inside for quicker visualization.
        ero_vis = ndimage.binary_erosion(air_inside)
        air_inside = air_inside & ~ero_vis

        scene = mlab.figure(size=(400, 400))
        scene.scene.background = (0.2, 0.2, 0.2)

        vox_plotter = Voxel_Plotter(voxel_dat)

        vox_plotter(air_inside, (0.6, 0.6, 0.8), 0.1)
        vox_plotter(exit_grid, (1.0, 0.0, 0.0))
        vox_plotter(crew_points, (0.3, 1.0, 0.3))

        mlab.show()
