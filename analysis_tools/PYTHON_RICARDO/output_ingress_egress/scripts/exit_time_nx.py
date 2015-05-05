"""
Using a precalculated map of the best pose that can be adopted at every point in the vehicle,
    determine the total exit time for a soldier moving from a specified starting position to a
    specified end position.

Requires NetworkX for the pathfinding routines.
"""
__author__ = 'ab27'

import logging, os

import matplotlib.pyplot as plt
import numpy as np
from scipy import ndimage
from scipy.spatial.distance import cdist
import networkx as nx

import data_io


## Allowed movement paths by angle
move_i = [0, -1, -1, -1, 0, 1, 1, 1]
move_j = [1, 1, 0, -1, -1, -1, 0, 1]


def a_star_heuristic(u, v):
    """
    Simple cartesian distance heuristic for a* algorithm.
    """
    # Trial: add a small contribution for orientation change, just so that different ories can be
    # distinguished. The effect of orie chage will be weighted 1/10 as much as motion,
    # plus it will never be more than a small integer distance to begin with
    return np.sqrt((u[0] - v[0]) ** 2 + (u[1] - v[1]) ** 2)


def get_nearest_walkable_vox(arb_vox, points_list):
    """
    Given a list of points in the floor, find which one is nearest to the specified arbitrary voxel
    """
    # Work out closest dimension (i or j). Assumes floor has already been calculated!
    if arb_vox in points_list:
        return arb_vox
    else:
        # If given 3-tuples, convert (i,j,orie) to (i,j) for dist search
        p_ij_list = [p[:2] for p in points_list]
        min_dists = cdist(p_ij_list, [arb_vox[:2]], 'cityblock')
        p = points_list[np.argmin(min_dists)]
        return p, np.min(min_dists)


def grid_to_graph_new(scores, settings):
    angles = sorted(scores.keys())
    remap = {k: v for k, v in enumerate(angles)}
    num_angles = len(angles)
    width = scores[angles[0]].shape[0]
    length = scores[angles[0]].shape[1]
    
    ## Get interior points only
    width_int_pts = range(width)[1:-1]
    length_int_pts = range(length)[1:-1]

    ## Make a graph spanning all floor positions at all angles
    walk_grid = nx.grid_graph(dim=[width, length, num_angles])
    
    ## Remove the cartesian edges because we want finer control
    walk_grid.remove_edges_from(walk_grid.edges())

    ## Add in connections that allow movement only in direction faced and angle to either side
    conns = []
    for a in xrange(num_angles):
        ## dirs contains the index of this angle plus the indices of those on either side
        dirs = [t % num_angles for t in [a - 1, a, a + 1]]
        
        ## Loop over every interior floor point
        for w in width_int_pts:
            for l in length_int_pts:
                for d in dirs:
                    ## Use the move_? arrays to determine allowable motion paths
                    conns.append(((w, l, a), (w + move_i[d], l + move_j[d], d)))

    walk_grid.add_edges_from(conns)
    
    ## Remove any nodes that are completely unwalkable (also takes care of edges)
    remove = [n for n in walk_grid.nodes_iter() if scores[remap[n[2]]][n[0], n[1]] == np.inf]            
    walk_grid.remove_nodes_from(remove)
    
    ## Make edge weights equal to the average of the node scores multiplied by distance
    for e in walk_grid.edges():
        n1, n2 = e[:2]
        dist = np.sqrt((n1[0] - n2[0]) ** 2 + (n1[1] - n2[1]) ** 2)

        weight = 0.5 * (scores[remap[n1[2]]][n1[0], n1[1]] + scores[remap[n2[2]]][n2[0], n2[1]])
            
        walk_grid[n1][n2]["weight"] = weight * dist

    return walk_grid


def load_create_graph(settings):
    """
    Load previously calculated arrays of walkability scores and create graph structure based on
    node connectivity
    """
    result_folder = os.path.join("results", settings["run_id"], str(settings["voxel_size"]))
    file_name = "{}_vox{}_best_scores".format(settings["run_id"], settings["voxel_size"])
                                              
    overall_walkability_scores = data_io.load_array(result_folder, file_name)

    file_name = "{}_vox{}_best_scores_by_orientation".format(settings["run_id"],
                                                             settings["voxel_size"])
    scores_by_orie = data_io.load_array(result_folder,
                                        file_name,
                                        multi_array=True)

    logging.debug("# nodes in unfiltered list: {}".format(overall_walkability_scores.size))

    connected_points_graph = grid_to_graph_new(scores_by_orie, settings)

    return connected_points_graph, overall_walkability_scores


def connect_doors(graph, floor_2d, door_points_list, settings):
    """
    Given a 2d grid of **walkable** floor points and a list of door point tuples (2d crds + orie),
        connect the door point to a single vehicle perimeter point (per orientation, if multiple
        orientations at that 2d position are near the door AND walkable)
    """
    # Extract floor perimeter.

    floor_mask = floor_2d != settings['unwalkable_value']
    # Graph excludes edges, so we need to erode past the perimeter to find door_adj pts in graph
    # Effectively, we need the single row *inside* the perimeter
    interior_small = ndimage.binary_erosion(floor_mask, iterations=4)
    interior_large = ndimage.binary_erosion(floor_mask, iterations=3)

    floor_perim_inside = interior_large & ~interior_small

    # Get list of (i,j) points where door and cabin intersect. Connect each door to whichever
    # exit spot is closest.
    exit_spots = zip(*np.where(floor_perim_inside))

    # Find the walkable floor point (on vehicle perimeter) closest to each provided door
    for p in door_points_list:
        if p not in graph:
            graph.add_node(p)

        door_adj_node, dist_to_door = get_nearest_walkable_vox(p, exit_spots)
        for i in xrange(len(settings['manikin_orientations'])):
            # Add orientation to the ij indices to create graph node coord
            door_adj_node = (door_adj_node[0], door_adj_node[1], i)
            logging.debug("Connecting door point {} to floor point {}".format(p, door_adj_node))

            graph.add_edge(door_adj_node, p, weight=dist_to_door)
            graph.add_edge(p, door_adj_node, weight=dist_to_door)

    if settings['debug']:
        # Visualize the walkable area perimeter, door points, and marked intersection points
        # together
        plt.imshow(floor_perim_inside.astype(np.int8), cmap="PuBuGn")
        ax = plt.gca()
        ax.autoscale(False)
        dpl = zip(*door_points_list)
        plt.plot(dpl[1], dpl[0], color='b', marker='o')  # Plot each marked door point

        save_folder = os.path.join("results", settings["run_id"], str(settings["voxel_size"]))
        plt.savefig(os.path.join(save_folder, "door_perimeter.png"))
    return graph


def total_exit_time(graph, start_points_list, exit_points_list):
    """
    Calculates the total exit time as the longest time it takes any soldier to reach the exit
    Satisfies TB033
    """
    ## Initialize to impossible value
    max_exit_time = -1.0

    for p_i in start_points_list:
        assert p_i in graph, "Specified start point not walkable"
        for p_e in exit_points_list:
            assert p_e in graph, "Specified exit point not walkable"

            try:
                dist_to_exit = nx.astar_path_length(graph,
                                                    source=p_i,
                                                    target=p_e,
                                                    weight='weight',
                                                    heuristic=a_star_heuristic)
                max_exit_time = max(max_exit_time, dist_to_exit)
            except (KeyError, nx.NetworkXNoPath):
                # If points are in graph but no path exists, log a message
                logging.info("No path found between voxel indices {} and {}".format(p_i, p_e))
                
    if max_exit_time == -1.0:
        raise ValueError("No exit paths were found")
        
    return max_exit_time


def plot_paths(graph, start_pts_list, exit_pt, ows, settings):
    """
    Provides an option to do pathfinding and plot results. Primarily useful for debugging,
    """

    # This should by definition be impossible, since the floor @ exit exists
    assert exit_pt in graph, "Specified exit point not in walkable path"

    plt.imshow(ows, cmap="gray_r")
    ax = plt.gca()
    ax.autoscale(False)

    for p_start in start_pts_list:
        msg = "Specified starting point is not walkable; ignore soldier @ {} "
        assert p_start in graph, msg.format(p_start)

        path_to_exit = nx.astar_path(graph,
                                     source=p_start,
                                     target=exit_pt,
                                     weight='weight',
                                     heuristic=a_star_heuristic)
        # Don't plot the final exit point; that's for internal bookkeeping. We care more about
        # what edge the soldier stepped on right before exiting.
        coord_lists = zip(*path_to_exit)
        
        ax.plot(coord_lists[1], coord_lists[0], linewidth=3, alpha=0.3)
        ax.plot(exit_pt[1], exit_pt[0], marker="^", color='r')

    save_folder = os.path.join("results", settings["run_id"], str(settings["voxel_size"]))
    plt.savefig(os.path.join(save_folder, "troop_exit_paths.png"))

if __name__ == "__main__":
    # User specified settings from separate file
    from rpl.tools.api import test_bench_api as tba
    SETTINGS = tba.load_settings("settings.js")

    ## Set up logging
    logging.info("\n"+50*"_"+'\ntest bench started\n'+50*"_")

    # Value references numpy; can't be read from json
    SETTINGS["unwalkable_value"] = np.inf

    # Load in vehicle occupied voxels file in order to locate the door points
    vox_veh_folder = r"voxelated_models/vehicles/{}/{}".format(SETTINGS["run_id"],
                                                               SETTINGS["voxel_size"])
    vox_veh_file = "voxels_{}_vox{}".format(SETTINGS["run_id"], SETTINGS["voxel_size"])
    veh_vox = data_io.load_array(vox_veh_folder, vox_veh_file, multi_array=True)

    #### Standalone egress testing mode requires a few dummy points that will depend on the
    # assembly chosen. These apply to the Ricardo master assembly as of Nov 11, 2013.
    vox_veh_cabin = (29, 63, 20)  # Dummy position; must label floor regions to find door-adj

    ## Point chosen for the EFV tester model
    #vox_veh_cabin = (140,30,10)

    #### Some sample output.
    # Gridpoints for 5cm voxel spacings. One point from cabin (exit exists), and one point for
    # driver ( can not reach door)
    start_points = [(29, 63, 4), (46, 45, 4), (29, 30, 4), (46, 67, 4), (29, 74, 4), (46, 34, 4),
                    (29, 52, 4), (46, 78, 4), (46, 23, 4), (29, 96, 4), (29, 85, 4), (46, 56, 4),
                    (46, 12, 4), (29, 19, 4), (29, 41, 4), (46, 89, 4)]

    # Define a single point of exit from the vehicle; for the Ricardo seed assembly Nov 2013
    door_points = [(38, 6, 4)]

    ## Start points on the EFV tester model (last digit is orientation)
    #start_points = [(40, 12, 4)]
    #door_points = [(145, 30, 4)]

    ##############
    # Assess test bench criteria using pathfinding calculations
    graph, overall_walkability_scores = load_create_graph(SETTINGS)

    ### Connect doors
    graph = connect_doors(graph, overall_walkability_scores, door_points, SETTINGS)

    # TB033: exit time
    tot_exit_time = total_exit_time(graph, start_points, door_points)

    # Apply same "final exit time" calculation used in the main egress test bench code.
    tot_exit_time *= SETTINGS["voxel_size"] / 1.0

    tb_results = {"total_exit_time": tot_exit_time}
    print "Results?"
    print tb_results

    plot_paths(graph, [start_points[0]], door_points[0], overall_walkability_scores, SETTINGS)