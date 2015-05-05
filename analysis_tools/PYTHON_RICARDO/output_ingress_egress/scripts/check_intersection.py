""" Determine the points where manikin and vehicle models overlap"""
import logging

import numpy as np

import data_io
import render_results
from voxel_methods import Vehicle, Component


def load_voxelated_grid(npz_filename):
    """
    Loads a previously determined uniform grid, and gets the in_out matrix that tests for whether
      a point is inside the enclosed area or not.
    """
    d = np.load(npz_filename)
    return d["occupied_voxels"]


def array_parts_in_voxel(vox_pos_i,
                         vox_pos_j,
                         vehicle,
                         manikin):
    """
    For vehicle and manikin with base in xz plane (height along y), returns parts of grids that
    overlap each other when centered at a given point (i,j). Relies on the assumption that
    manikin has smaller dimensions in xz plane than vehicle does.
    """
    assert manikin.shape[0] < vehicle.shape[0], "Vehicle is too narrow to fit manikin"
    assert manikin.shape[1] < vehicle.shape[1], "Vehicle is too narrow to fit manikin"

    manikin_i = manikin.shape[0]
    manikin_j = manikin.shape[1]
    manikin_k = manikin.shape[2]

    half_mani_i = manikin_i // 2
    half_mani_j = manikin_j // 2

    # How much we get at the end of the box depends on whether the manikin dimension is even or odd
    add_to_i = 0 if manikin_i % 2 == 0 else 1
    add_to_j = 0 if manikin_j % 2 == 0 else 1

    vehicle_i = vehicle.shape[0]
    vehicle_j = vehicle.shape[1]

    ########
    # Get manikin and vehicle parts around the center point vox_pos_i, vox_pos_k (in xz plane)
    # On the near side (origin --> vox_i)
    if vox_pos_i - half_mani_i > 0:
        # If end of manikin fits completely in space between origin and voxel...
        mani_start_i = 0  # Then include all of the manikin!
        veh_start_i = vox_pos_i - half_mani_i
    else:
        # Fit whatever of manikin we can to start of vehicle box
        mani_start_i = half_mani_i - vox_pos_i
        veh_start_i = 0  # Manikin overlaps end of box, so include all vehicle from start to here

    if vox_pos_j - half_mani_j > 0:
        mani_start_j = 0
        veh_start_j = vox_pos_j - half_mani_j
    else:
        mani_start_j = half_mani_j - vox_pos_j
        veh_start_j = 0

    # On the far side (vox_i --> origin)
    if vox_pos_i + half_mani_i < vehicle_i:
        # If full manikin fits in space remaining...
        mani_end_i = manikin_i
        veh_end_i = vox_pos_i + half_mani_i + add_to_i
    else:
        # Fit whatever of manikin we can to end of box
        mani_end_i = half_mani_i + (vehicle_i - vox_pos_i)
        veh_end_i = vehicle_i

    if vox_pos_j + half_mani_j < vehicle_j:
        mani_end_j = manikin_j
        veh_end_j = vox_pos_j + half_mani_j + add_to_j
    else:
        mani_end_j = half_mani_j + (vehicle_j - vox_pos_j)
        veh_end_j = vehicle_j

    floor_at_this_point = vehicle.floor[vox_pos_i, vox_pos_j]
    ceil_at_this_point = vehicle.ceiling[vox_pos_i, vox_pos_j] if vehicle.ceiling[vox_pos_i,
                                                                                  vox_pos_j] != 0 \
        else floor_at_this_point

    assert ceil_at_this_point >= floor_at_this_point, "Vehicle roof is below the floor."

    manikin_in_voxel = manikin.occupied_voxels[mani_start_i: mani_end_i,
                                               mani_start_j: mani_end_j,
                                               : manikin_k if manikin_k <= (
                                                ceil_at_this_point - floor_at_this_point)
                                                 else (ceil_at_this_point - floor_at_this_point)]
    # Uses labeled vehicle interior region; assumes that such labels have been produced before
    # this function is used. (see sample __main__ method). Hack: rather than pass in
    # mask_from_voxel to this function, just ignore the getter function and run this directly
    vehicle_in_voxel = vehicle.labels[veh_start_i: veh_end_i,
                                               veh_start_j: veh_end_j,
                                              floor_at_this_point: floor_at_this_point +
                                                                    manikin_k if
                                               (floor_at_this_point + manikin_k) <
                                               ceil_at_this_point else ceil_at_this_point]

    assert vehicle_in_voxel.shape == manikin_in_voxel.shape, \
        "Vehicle and manikin voxel boxes are not same size"
    return vehicle_in_voxel, manikin_in_voxel


def intersection_check(vehicle, manikin, settings):
    """
    Determines which points in the vehicle are ok for the manikin to stand in. Returns an array
        indicating bad places to stand. (100% = totally unwalkable)
    """
    collisions = np.ones((vehicle.shape[0], vehicle.shape[1]),
                         dtype=np.float32)

    #manikin.get_labels(mask_from_voxel=(-1, -1, -1))
    #vehicle.make_labeled_regions(binary_mask=True)

    # Find how many voxels are assigned to the manikin by adding up all the "true" pixels.
    #   Will be used later for % collision checks.
    manikin_volume = np.sum(manikin.occupied_voxels)

    assert manikin_volume < np.product(vehicle.shape),\
        "Manikin volume > vehicle volume"

    # Allow buffer (rounded up) to let soldier fit box by tilting head.
    num_voxel_height_tolerance = np.ceil(0.075 / settings["voxel_size"])
    manikin_height = manikin.shape[2]

    floor = vehicle.get_floor()
    ceiling = vehicle.get_ceil()

    # Anywhere that floor != top of vehicle box (max y dimension)
    voxels_with_floor = zip(*np.where(floor != vehicle.shape[2]))
    # We only perform intersection checks when there is floor available to stand on.
    for p in voxels_with_floor:
        avail_height = max(ceiling[p] - floor[p], 0)

        if manikin_height - avail_height > num_voxel_height_tolerance:
            # If manikin is taller than allowed height by more than a buffer zone, it doesn't fit.
            continue

        # If manikin fits, do intersection checks. Marks voxel true if both the result and
        #   manikin arrays have triangles there.
        # This rasters over the plane of the vehicle
        vehicle_in_voxel, manikin_in_voxel = array_parts_in_voxel(p[0],
                                                                  p[1],
                                                                  vehicle,
                                                                  manikin)

        collisions[p] = 1. - float(np.sum(manikin_in_voxel & vehicle_in_voxel)) / float(
            manikin_volume)

    collisions *= 100

    return collisions


def save_results(fldr_name, f_name, data_array, save_plots=False):
    """
    Saves figure. Basename should be the model name without voxel or angle info appended.
    """
    data_io.save_array(fldr_name, f_name, data_array)
    if save_plots:
        render_results.save_heatmap(fldr_name, f_name, data_array, label="% collisions with walls")


def main(manikin_name, vehicle_obj, settings):
    """
    Perform all tasks related to intersection check between the specified vehicle and manikin
    """
    for ang in settings["manikin_orientations"]:
        ang %= 360.
        manikin_folder = r"voxelated_models\manikins\{}\{}".format(
            manikin_name, settings["voxel_size"])
        manikin_filename = "voxels_{}_vox{}_ang{}".format(manikin_name, settings["voxel_size"],
                                                          ang)
        logging.debug("Processing intersection between {} and {}".format(settings["run_id"],
                                                                        manikin_name))

        mani_data = data_io.load_array(manikin_folder, manikin_filename, multi_array=True)
        manikin = Component(mani_data,
                            voxel_size=settings["voxel_size"])

        ################################################
        # Process manikin
        collisions = intersection_check(vehicle_obj, manikin, settings)

        basename = "{}_{}".format(settings["run_id"], manikin_name)

        result_folder = "results/{}/{}".format(basename, settings["voxel_size"])
        file_name = "{}_vox{}_ang{}".format(basename, settings["voxel_size"], ang)

        save_results(result_folder, file_name, collisions, save_plots=settings["show_2d"])

if __name__ == "__main__":
    #############
    # User specified settings from separate file
    from rpl.tools.api import test_bench_api as tba
    SETTINGS = tba.load_settings("settings.js")

    # Load in vehicle occupied voxels file
    vox_veh_folder = r"voxelated_models/vehicles/{}/{}".format(SETTINGS["run_id"],
                                                               SETTINGS["voxel_size"])
    vox_veh_file = "voxels_{}_vox{}".format(SETTINGS["run_id"], SETTINGS["voxel_size"])
    veh_vox = data_io.load_array(vox_veh_folder, vox_veh_file, multi_array=True)

    vehicle = Vehicle(veh_vox, voxel_size=SETTINGS["voxel_size"])
    # Get floor and ceiling at a dummy position that may or may not work; used for testing code

    #### Arbitrary: use a point in the exact center of the vehicle
    #vox_veh_cabin = (vehicle.shape[0] // 2, vehicle.shape[1] // 2, vehicle.shape[2] // 2)

    # Point chosen for the EFV tester model
    vox_veh_cabin = (140,30,10)

    vehicle.get_labels(vox_veh_cabin)
    vehicle.get_floor(vox_veh_cabin)
    vehicle.get_ceil(vox_veh_cabin)

    for pose_name in SETTINGS["manikins_to_use"]:
        logging.debug("Checking intersection of '{}'".format(pose_name))
        main(pose_name, vehicle, settings=SETTINGS)