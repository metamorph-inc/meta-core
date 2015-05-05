"""
Find the manikin pose that can most comfortably stand at each point in a vehicle
    Requires having already run uniform_grid.py --> test_intersection.py to voxelate and
    intersect the desired manikin and vehicle models. Assumes the result folders are arranged as
    specified by test_intersection.py.
"""
__author__ = 'ab27'

import numpy as np

import render_results
import data_io


def load_fit_of_pose_in_vehicle(basename, settings):
    """
    Load in intersection check results for a given pose in the vehicle
    :param basename: The manikin + vehicle name pair
    :param settings:
    :return: Dict of {angle: collision array} pairs for every orientation of mani in vehicle
    """
    result_folder = "results/{}/{}".format(basename, settings["voxel_size"])

    collision_arrays_dict = {}
    for ang in settings["manikin_orientations"]:
        data_file_name = "{}_vox{}_ang{}".format(basename, settings["voxel_size"], ang)
        collision_arrays_dict[ang] = data_io.load_array(result_folder, data_file_name)

    # Also combine results and store overall best fit (aka "does pose ever fit in this point?")
    collision_arrays_dict["overall"] = find_best_scored_pose(
        [collision_arrays_dict[k] for k in collision_arrays_dict])

    return collision_arrays_dict


def mask_results(result_array, value_if_fits, settings):
    """
     Determines if each position in one array is walkable. If so, assigns the specified float
     value to that position. This is useful when combining results later: get a set of scores for
     each pose, then stack those score matrices.
    :param result_array:
    :param value_if_fits:
    """
    result_array = result_array.copy()
    result_array[result_array <= settings["max_collision_pct"]] = value_if_fits
    result_array[result_array > settings["max_collision_pct"]] = settings["unwalkable_value"]

    return result_array


def find_best_scored_pose(results_list):
    """
    Stack the "best possible fit for each pose" arrays together. Determine the overall best
    scoring (lowest value) pose.
    """
    stacked_results = np.array(results_list)
    return np.min(stacked_results, axis=0)


def score_from_list(walkability_by_pose, scores_for_poses, settings, orientation="overall",
                    score_key='score'):
    """
    From the original set of walkability scores for all manikin poses, determine the best score
    for every point. (using either pose #s or scores as desired)
    """
    results_masked_with_pose_ids = [mask_results(walkability_by_pose[k][orientation],
                                    scores_for_poses[k][score_key], settings)
                                    for k in walkability_by_pose]

    return find_best_scored_pose(results_masked_with_pose_ids)


def save_results(data_array, result_folder, save_basename, plot=False, plot_options=None):
    """
    Save (and plot) results as appropriate
    """
    plot_options = plot_options or {}

    data_io.save_array(result_folder,
                       save_basename,
                       data_array)
    if plot:
        render_results.save_heatmap(result_folder,
                                    save_basename,
                                    data_array,
                                    **plot_options)


def main(settings):
    """
    Determine the best pose of the manikin for each position and orientation.
    """
    basename = "{}_{}"

    # Get one matrix per pose, representing the best possible fit of that pose at a given point
    # in the vehicle
    walkability_by_pose_name = {manikin_name: load_fit_of_pose_in_vehicle(
        basename.format(settings["run_id"], manikin_name),
        settings)
        for manikin_name in settings["manikin_poses"]}
    # TODO: number of best pose may be superfluous. Can probably disable saving this info.
    best_pose_number_by_point = score_from_list(walkability_by_pose_name,
                                                settings["manikin_poses"],
                                                settings,
                                                score_key='num',
                                                orientation="overall")
    best_score_by_point = score_from_list(walkability_by_pose_name,
                                          settings["manikin_poses"],
                                          settings,
                                          score_key='score',
                                          orientation="overall")

    best_pose_for_orientation = {}
    best_score_for_orientation = {}
    for ang in settings["manikin_orientations"]:
        best_pose_for_orientation[str(ang)] = score_from_list(walkability_by_pose_name,
                                                              settings["manikin_poses"],
                                                              settings,
                                                              score_key='num',
                                                              orientation=ang)
        best_score_for_orientation[str(ang)] = score_from_list(walkability_by_pose_name,
                                                               settings["manikin_poses"],
                                                               settings,
                                                               score_key='score',
                                                               orientation=ang)

    # Save results
    result_folder = "results/{}/{}".format(settings["run_id"], settings["voxel_size"])

    saved_file_basename = "{}_vox{}_best_scores".format(
        settings["run_id"], settings["voxel_size"])

    save_results(best_score_by_point,
                 result_folder,
                 saved_file_basename,
                 plot=settings["show_2d"],
                 plot_options={"label": "Walkability scores",
                               "tick_labels": sorted([v['score'] for k, v
                                                      in settings["manikin_poses"].items()])})

    saved_file_basename = "{}_vox{}_best_poses".format(
        settings["run_id"], settings["voxel_size"])
    save_results(best_pose_number_by_point,
                 result_folder,
                 saved_file_basename,
                 plot=settings["show_2d"],
                 plot_options={"label": "Best pose at point",
                               "tick_labels": sorted([v['num'] for k, v in
                                                      settings["manikin_poses"].items()])})

    # Also save the best scores per orientation
    data_io.save_multi_array(result_folder,
                             "{}_vox{}_best_poses_by_orientation".format(settings["run_id"],
                                                                         settings["voxel_size"]),
                             best_pose_for_orientation)
    data_io.save_multi_array(result_folder,
                             "{}_vox{}_best_scores_by_orientation".format(settings["run_id"],
                                                                          settings["voxel_size"]),
                             best_score_for_orientation)

    if settings["show_2d"]:
        # After saving score data to one big npz file, make the individual plots to match
        for ang in settings["manikin_orientations"]:
            saved_file_basename = "{}_vox{}_best_scores_ang{}".format(settings["run_id"],
                                                                      settings["voxel_size"], ang)
            render_results.save_heatmap(result_folder,
                                        saved_file_basename,
                                        best_score_for_orientation[str(ang)],
                                        label="Best score for orientation {}".format(ang),
                                        tick_labels=sorted([v['score']
                                                for k, v in settings["manikin_poses"].items()]))

if __name__ == "__main__":
    #############
    # User specified settings from separate file
    import json
    with open(r"settings.js", "r") as f:
        SETTINGS = json.load(f)

    # Value references numpy; can't be read from json
    SETTINGS["unwalkable_value"] = np.inf

    main(SETTINGS)