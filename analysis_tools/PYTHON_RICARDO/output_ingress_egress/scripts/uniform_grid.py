""" Represent a triangulated surface using a 3D boolean grid"""
import logging
import numpy as np

from rpl.tools.ray_tracing.bsp_tree_poly import BSP_Element
from rpl.tools.geometry import geom_utils
import data_io


class BSP_Grid(object):
    def __init__(self, node_array, tris, allocate_step=100000):
        """ 
        Store the triangles with an enumeration so that even when they are subdivided their 
        identity is not lost.
        """
        tri_nums = np.arange(len(tris), dtype=np.int32).reshape((len(tris), 1))
        minus_ones = -np.ones((len(tris), 6), dtype=np.int32)

        self.tris = np.hstack((tris, minus_ones, tri_nums))
        
        self.allocate_step = allocate_step
        self.node_array = node_array  # Reference to the full list of nodes
        self._resize()
        self.next_free = len(node_array)

        self.split_cache = np.zeros(len(self.tris), dtype=np.int32)

    def _resize(self):
        """
        Increase node array size by the allocate_step amount.
        """
        self.array_size = len(self.node_array) + self.allocate_step
        self.node_array = np.concatenate((self.node_array, np.zeros((self.allocate_step, 3))))

    def add_node(self, node):
        """
        Adds a new node to the end of the node array (expanding if required). Returns the index of
        the newly added node.
        """
        if self.next_free == self.array_size:
            self._resize()
        self.node_array[self.next_free] = node
        self.next_free += 1
        return self.next_free - 1

    def prepare_add(self, num_add_nodes):
        """
        Make sure that ``num_add_nodes`` can be added later without needing a resize.
        
        Useful if adding nodes from within cython where resizing is tricky.
        """
        if self.next_free + num_add_nodes >= self.array_size:
            self._resize()
        return self.next_free


def make_grid(veh_surfs, settings):
    """
    Make coordinates of voxelated grid based on overall list of vehicle surfaces
    """
    ## Find overall bounding box
    x_min, x_max = 1e30, -1e30
    y_min, y_max = 1e30, -1e30
    z_min, z_max = 1e30, -1e30

    for key, veh_surf in veh_surfs.items():
        x_min, x_max = min(x_min, np.min(veh_surf["x"])), max(x_max, np.max(veh_surf["x"]))
        y_min, y_max = min(y_min, np.min(veh_surf["y"])), max(y_max, np.max(veh_surf["y"]))
        z_min, z_max = min(z_min, np.min(veh_surf["z"])), max(z_max, np.max(veh_surf["z"]))

    x_min, x_max = x_min - settings["voxel_size"],  x_max + settings["voxel_size"]
    y_min, y_max = y_min - settings["voxel_size"],  y_max + settings["voxel_size"]
    z_min, z_max = z_min - settings["voxel_size"],  z_max + settings["voxel_size"]

    ###########################################
    # Create the uniformly spaced grid points
    x_grid = np.arange(x_min, x_max + settings["voxel_size"], settings["voxel_size"])
    y_grid = np.arange(y_min, y_max + settings["voxel_size"], settings["voxel_size"])
    z_grid = np.arange(z_min, z_max + settings["voxel_size"], settings["voxel_size"])

    return x_grid, y_grid, z_grid


def convert_geom(veh_surf, tr_mat):
    """
    Rotate nodes using provided transformation matrix; convert xyz node dict to nodes array
    """
    veh_surf["nodes"] = np.vstack((veh_surf["x"], veh_surf["y"], veh_surf["z"])).T
    veh_surf['nodes'] = np.dot(veh_surf['nodes'], tr_mat[:3, :3])

    veh_surf["x"] = veh_surf['nodes'][:, 0]
    veh_surf["y"] = veh_surf['nodes'][:, 1]
    veh_surf["z"] = veh_surf['nodes'][:, 2]

    return veh_surf


def find_occupied_voxels(surf, surf_mask, voxel_data):
    """
    Voxels with any triangle from ``surf`` are considered occupied and or'ed with ``group_mask``.
    
    If the supplied ``occupied_voxels`` is None a voxel array is created and returned.
    """
    nodes = surf["nodes"]
    tris = surf["tris"]
    
    x_pts, y_pts, z_pts = [voxel_data[k] for k in ("x_grid", "y_grid", "z_grid")]
    vox_size = voxel_data["vox_size"]
    ## Find the local extents of this part
    min_x, max_x = np.min(surf["x"]) - vox_size, np.max(surf["x"]) + vox_size
    min_y, max_y = np.min(surf["y"]) - vox_size, np.max(surf["y"]) + vox_size
    min_z, max_z = np.min(surf["z"]) - vox_size, np.max(surf["z"]) + vox_size
    
    b_tree = BSP_Grid(nodes, tris)

    # Create BSP tree elements- we're not using a tree, but we are using some of the functions
    b_x_root = BSP_Element(b_tree.tris, b_tree)

    size_i, size_j, size_k = len(x_pts), len(y_pts), len(z_pts)

    ## Create the occupied voxels if none were supplied
    if voxel_data["value"] is None:
        voxel_data["value"] = np.zeros((size_i - 1, size_j - 1, size_k - 1), dtype=np.uint32)

    occupied_voxels = voxel_data["value"]
    
    ## The [1:] is because to make n voxels in a given direction we need n-1 splits
    for i, x_pos in enumerate(x_pts[1:]):
        if x_pos < min_x: continue
        if x_pos > max_x: break
        b_above_x, b_below_x = b_x_root.split_at(0, x_pos)
        b_y_root = b_below_x
        for j, y_pos in enumerate(y_pts[1:]):
            if b_y_root is None:
                break
            if y_pos < min_y: continue
            if y_pos > max_y: break
            b_above_y, b_below_y = b_y_root.split_at(1, y_pos)
            b_z_root = b_below_y
            for k, z_pos in enumerate(z_pts[1:]):
                if b_z_root is None:
                    break
                if z_pos < min_z: continue
                if z_pos > max_z: break
                b_above_z, b_below_z = b_z_root.split_at(2, z_pos)
                
                if not (b_below_z and (len(b_below_z.tris) == 0)):
                    ## There is at least part of triangle here so mark as occupied
                    occupied_voxels[i, j, k] |= surf_mask

                b_z_root = b_above_z
            b_y_root = b_above_y
        b_x_root = b_above_x

    return voxel_data


#############
# Main code
def main(vehicle_comp_coords, tr_mat, voxel_masks, settings):
    """
    Perform voxelization for all vehicle geometries in a list of parts. Combine on a uniform grid.
    """
    for key, veh_surf in vehicle_comp_coords.items():
        # Convert coordinates and find overall best bounding box
        veh_surf = convert_geom(veh_surf, tr_mat)

    x_grid, y_grid, z_grid = make_grid(vehicle_comp_coords, settings)

    voxel_data = {"x_grid": x_grid,
                  "y_grid": y_grid,
                  "z_grid": z_grid,
                  "vox_size": settings["voxel_size"],
                  "csys_trans": tr_mat,
                  "value": None}

    for key, veh_surf in vehicle_comp_coords.items():
        # Build up the voxel_data
        logging.debug("Sampling component: {}".format(key))
        ## Default mask is 1 for anything not in an identified set
        surf_mask = 1
        for mask, geo_set in voxel_masks.items():
            if veh_surf['part_class'] in geo_set:
                surf_mask |= mask

        voxel_data = find_occupied_voxels(veh_surf, surf_mask, voxel_data)

    return voxel_data

if __name__ == "__main__":
    from rpl.tools.api import test_bench_api as tb_api
    SETTINGS = tb_api.load_settings("settings.js")

    DOORS = {'Hatch_Assembly_Rear_Ramp', 'Hatch_Assembly_Personnel_Door'}
    HATCHES = {'Hatch_Assembly_Driver_Commander', 'Hatch_Assembly_Cargo'}
    HULLS = {"Hull_Assembly_Parametric", 'Hull_Assembly_Example_With_Connector'}
    MANIKINS = {"Manikin"}
    # Special labels applied to specific types of voxels
    VOXEL_LABELS = {2: HULLS,
                    4: DOORS,
                    8: HATCHES,
                    16: MANIKINS}

    vehicle_surfs = tb_api.load_geometry(tb_api.get_all_geom_set() - MANIKINS, single_file=False)

    # Modify node coords so object aligns with cartesian axes of occ voxel grid, +z=up
    # Vector to rotate around is cross product of current z axis and sfc normal
    veh_up = np.array([0., 1., 0.])
    rot_around = np.cross(veh_up, np.array([0, 0, 1]))
    rot_ang = -np.arccos(veh_up[2])
    tr_mat = geom_utils.rotation_about_vector(rot_around, rot_ang)

#    voxel_data = main(vehicle_surfs, tr_mat, VOXEL_LABELS, SETTINGS)
    vox_veh_folder = r"voxelated_models/vehicles/{}/{}".format(SETTINGS["run_id"],
                                                               SETTINGS["voxel_size"])

    vox_veh_file = "voxels_{}_vox{}_hacked".format(SETTINGS["run_id"],
                                                   SETTINGS["voxel_size"])

    try:
        voxel_data = data_io.load_array(vox_veh_folder, vox_veh_file, True)
    except:
        voxel_data = main(vehicle_surfs, tr_mat, VOXEL_LABELS, SETTINGS)
    
    from mayavi import mlab
    xo, yo, zo = np.where(voxel_data["value"] == 1)
    plot_vehicle = mlab.points3d(voxel_data["x_grid"][xo],
                                 voxel_data["y_grid"][yo],
                                 voxel_data["z_grid"][zo],
                                 color=(0.9, 0.9, 0.9),
                                 scale_mode="none", scale_factor=voxel_data["vox_size"],
                                 mode='cube', opacity=1)

    xo, yo, zo = np.where(voxel_data["value"] & 2)
    plot_vehicle = mlab.points3d(voxel_data["x_grid"][xo],
                                 voxel_data["y_grid"][yo],
                                 voxel_data["z_grid"][zo],
                                 color=(1, 1, 1),
                                 scale_mode="none", scale_factor=voxel_data["vox_size"],
                                 mode='cube', opacity=0.05)

    xo, yo, zo = np.where(voxel_data["value"] & 4)
    plot_vehicle = mlab.points3d(voxel_data["x_grid"][xo],
                                 voxel_data["y_grid"][yo],
                                 voxel_data["z_grid"][zo],
                                 color=(1.0, 0.5, 0.5),
                                 scale_mode="none", scale_factor=voxel_data["vox_size"],
                                 mode='cube', opacity=1)

    xo, yo, zo = np.where(voxel_data["value"] & 8)
    plot_vehicle = mlab.points3d(voxel_data["x_grid"][xo],
                                 voxel_data["y_grid"][yo],
                                 voxel_data["z_grid"][zo],
                                 color=(0.6, 0.6, 1.0),
                                 scale_mode="none", scale_factor=voxel_data["vox_size"],
                                 mode='cube', opacity=1)

    # No manikins included, no need to plot them
    # xo, yo, zo = np.where(voxel_data["value"] & 16)
    # plot_vehicle = mlab.points3d(voxel_data["x_grid"][xo],
    #                              voxel_data["y_grid"][yo],
    #                              voxel_data["z_grid"][zo],
    #                              color=(0.5, 1.0, 0.8),
    #                              scale_mode="none", scale_factor=voxel_data["vox_size"],
    #                              mode='cube', opacity=1.0)
    mlab.show()

    # Save the voxelated model of the vehicle (sans door and other excluded parts)
    data_io.save_multi_array(vox_veh_folder, vox_veh_file, voxel_data)