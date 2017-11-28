"""
Determine the force required to open hatches and ramps
"""
__author__ = 'ab27'

import itertools, logging, os, sys
import _winreg

import numpy as np
import matplotlib.pyplot as plt
from matplotlib.tri import Triangulation
from matplotlib.ticker import NullFormatter

from part_info import Hatch_Info, Spring_Info, Torsion_Bar_Info
# tb_api is imported below, after VU environment setup

####
# Define reused physical constants
g = 9.807  # m/s**2 - ensure that units are always consistent with this!
hatch_types = {'Hatch_Assembly_Cargo', 'Hatch_Assembly_Driver_Commander'}


#######
# Setup for VU test bench environment
def query_analysis_tools():
    analysis_tools_key_name = r'SOFTWARE\META\AnalysisTools'

    analysis_tools_key = _winreg.OpenKey(_winreg.HKEY_LOCAL_MACHINE, analysis_tools_key_name, 0,
                                         _winreg.KEY_READ)

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
sys.path.append(os.path.join(tools['closures']['InstallLocation']))

from rpl.tools.api import test_bench_api as tba


def get_parts_of_interest():
    """
    Specify the parts of interest for fetching data via API
    """
    # Same params are requested for every hatch; avoid duplication
    hatch_prms = {"Property": ['hinge_max_articulation_angle'],
                  "ModelMetric": ['LATCH_POSITION', 'TOTAL_MASS'],
                  "Datum": ['CG_CSYS', 'EXT_HINGE_PIN',
                           'EXT_AXIAL_MOUNT_1',
                           'EXT_AXIAL_MOUNT_2'],
                  "Required": "{*}"}

    poss_parts = {"Hatch_Assembly_Driver_Commander": hatch_prms,
                  "Hatch_Assembly_Cargo": hatch_prms,
                  "Spring_Torsion_Laminated":
                      {"Property": ['installed_preload_angle', 'laminae_thickness',
                                    'laminae_width', 'max_shear_stress', 'number_of_laminae',
                                    'laminae_length', 'shear_modulus'],
                      "Required": "{*}"},
                  "Spring_Axial_Helical":
                      {"Property": ['coil_diameter', 'free_length', 'number_of_coils',
                                    'shear_modulus', 'wire_diameter'],
                      "Required": "{*}"}}
    return poss_parts


class Assembly_Info(object):
    """
    Fetch relevant information about the overall assembly, including:
    - What types of hatches and springs there are
    - How they are connected
    - Relative geometry of parts (spring positions relative to hatch)

    The total torque consists of three components:
    1) the torque due to the hatch mass;
    2) the torque from torsion spring bars/laminated springs (if present), and
    3) the torque contributions from axial helical springs (if present)
    """
    def __init__(self):
        self.parts_found = tba.get_data(get_parts_of_interest())

        self.hatches = {}
        self.get_hatches()

        # Determine the connectivity of parts
        self.conn = self.get_connectivity()

        # Load in spring and tbar data & connect each to the appropriate hatch
        self.get_axial_springs()
        self.get_torsion_bars()

        # After assembly initialization, run sanity check
        assert len(self.hatches) > 0, "No hatches were found in this assembly."

    def get_hatches(self):
        """
        Fetch data, then convert to objects
        """
        for t in hatch_types:
            # Get data for all hatches of this type, then create Hatch objs for all hatches of
            # that type in the vehicle (eg 2 cargo hatches)
            hatches_dict = self.parts_found[t]

            for hatch_name in hatches_dict:
                self.hatches[hatch_name] = Hatch_Info(hatches_dict[hatch_name], hatch_name)

    def get_connectivity(self):
        """
        Extract connectivity between parts: roof hatches connect to springs; ramps connect to
        actuators.

        TESTING PURPOSES ONLY- use manually specified part connectivity from settings file.
        """
        # TODO: Replace with extraction of real connectivity info
        return SETTINGS['connectivity']

    def get_axial_springs(self):
        """
        Get Axial Helical springs associated with each hatch
        """
        springs_dict = self.parts_found["Spring_Axial_Helical"]
        for spr_name in springs_dict:
            spring = Spring_Info(springs_dict[spr_name], spr_name)
            # Get the hatch connected to this spring, then add object to hatch's spr_ax dict.
            conn_hatch = self.hatches.get(self.conn.get(spr_name))
            if conn_hatch is None:
                logging.error("Axial spring {} is not associated with any hatch".format(spr_name))
            else:
                conn_hatch.spr_ax[spr_name] = spring
                spring.hatch_mt_pt = conn_hatch.ax_spr_mt_hatch
                spring.body_mt_pt = conn_hatch.ax_spr_mt_body

    def get_torsion_bars(self):
        """
        Get torsion bars associated with each hatch
        """
        springs_dict = self.parts_found["Spring_Torsion_Laminated"]
        for spr_name in springs_dict:
            spring = Torsion_Bar_Info(springs_dict[spr_name], spr_name)
            # Get the hatch associated with this spring, then add object to hatch's spr_tors dict.
            conn_hatch = self.hatches.get(self.conn.get(spr_name))
            if conn_hatch is None:
                logging.error("Torsion bar {} is not associated with any hatch".format(spr_name))
            else:
                conn_hatch.spr_tors[spr_name] = spring


def hatch_torque_t30(hatch, grav_angle=0.0):
    """
    Determine the torque required to open the hatch by itself (due to hatch mass)
    """
    # Convert angle to radians
    grav_angle = grav_angle * np.pi / 180.
    cg_moment_arm = [hatch.r_cg * np.cos(hatch.ang_closed + ang + grav_angle)
                     for ang in hatch.angle_range]
    t_hatch_mass = hatch.mass * g * np.array(cg_moment_arm)
    return t_hatch_mass


def spring_torsion_torque_t31(hatch, tors_bar):
    """
    Determine the torque due to the laminated torsion bar

    The ICD for this part also calls for stress limit check
    """
    torque_bar = - tors_bar.tors_stiff * (tors_bar.windup_angle - hatch.angle_range)

    max_stress = ((tors_bar.lam_thick * tors_bar.shear_modulus * tors_bar.windup_angle) /
                  tors_bar.lam_length)
    stress_passed = tors_bar.shear_stress_limit > max_stress

    return torque_bar, stress_passed


def spring_axial_torque_t32(hatch, spring_ax):
    """
    Determine the torque due to a spring (axial helical)
    """

    spring_rate = ((spring_ax.shear_modulus * spring_ax.wire_diameter ** 4) /
                   (8. * spring_ax.coil_diameter ** 3 * spring_ax.number_of_coils))

    # Calculate parameters that are distance dependent etc
    hatch_mount_locus_r = np.linalg.norm(spring_ax.hatch_mt_pt)
    body_mount_locus_r = np.linalg.norm(spring_ax.body_mt_pt)
    spring_mt_angle_closed = np.arcsin(spring_ax.hatch_mt_pt[1] / hatch_mount_locus_r)

    torque_spring = []
    for ang in hatch.angle_range:
        # Get the hatch mount point as the hatch rotates
        hatch_mt_x_pos = hatch_mount_locus_r * np.cos(spring_mt_angle_closed + ang)
        hatch_mt_y_pos = hatch_mount_locus_r * np.sin(spring_mt_angle_closed + ang)
        hatch_mt_cur_pt = np.array([hatch_mt_x_pos, hatch_mt_y_pos, 0])

        spring_length = np.linalg.norm(spring_ax.body_mt_pt - hatch_mt_cur_pt)
        # np.sqrt((spring_ax.body_mt_x - hatch_mt_x_pos) ** 2 +
        #                        (spring_ax.body_mt_y - hatch_mt_y_pos) ** 2)

        lin_spring_force = spring_rate * (spring_length - spring_ax.spring_free_length)

        beta = (np.arcsin(spring_ax.body_mt_pt[1] / body_mount_locus_r) -
                np.arcsin((spring_ax.body_mt_pt[1] - hatch_mt_y_pos) / spring_length))

        spring_moment_arm = body_mount_locus_r * np.sin(beta)

        t_spring = - lin_spring_force * spring_moment_arm
        torque_spring.append(t_spring)

    return np.array(torque_spring)


def plt_res_graphs(hatch_obj, torques_dict, hatch_geom):
    """
    Plot 2D line graphs of the results- individual torque contributions and net force
    """
    # Set locations of 3 subplots on square using grid manager
    left, width = 0.1, 0.4
    bottom, height = 0.1, 0.4
    bottom_h = left_h = left + width + 0.04

    top_left = [left, bottom_h, width * 2., height]
    bottom_left = [left, bottom, width, height]
    bottom_right = [left_h, bottom, width, height]

    # Plots should cycle through a list of possible plot symbols
    line_styles = itertools.cycle(["--", ":", "-.", "-"])

    fig = plt.figure()
    fig.suptitle("Results for {}".format(hatch_obj.part_id.title()))

    # # TORQUE components
    ax1 = plt.axes(top_left)
    plt.title('Torque')
    for k in sorted(torques_dict):
        plt.plot(hatch_obj.angle_range * 180. / np.pi, torques_dict[k],
                 label=k, linestyle=line_styles.next())

    plt.plot(hatch_obj.angle_range * 180. / np.pi, torque_net, label="Net")
    plt.xlabel('Hatch opening angle (degrees)')
    plt.ylabel('Torque (J)')
    plt.autoscale(enable=True, tight=True)

    # Intelligently position plot legend for top left plot
    box = ax1.get_position()
    ax1.set_position([box.x0, box.y0, box.width * 0.5, box.height * 0.8])

    # Put a legend to the right of the current axis
    ax1.legend(loc='center left', bbox_to_anchor=(1, 0.5))

    ## HATCH FORCE
    ax2 = plt.axes(bottom_left)
    plt.title('Net force applied at handle')
    plt.plot(hatch_obj.angle_range * 180. / np.pi, hatch_force_net, label="Net hatch force")
    plt.xlabel('Hatch opening angle (degrees)')
    plt.ylabel('Force (N)')
    plt.autoscale(enable=True, tight=True)
    box = ax2.get_position()
    ax2.set_position([box.x0, box.y0, box.width, box.height * 0.8])

    ## Plot the 2D cross section side view of a hatch, showing marked CG point and hinge position
    # Convert geometry to the hinge coordinate system. Create 4 column node dict
    ax3 = plt.axes(bottom_right)
    nullfmt = NullFormatter()
    ax3.xaxis.set_major_formatter(nullfmt)
    ax3.yaxis.set_major_formatter(nullfmt)

    plt.title("Side view")
    hatch_geom['nodes'] = np.vstack((hatch_geom['x'],
                                     hatch_geom['y'],
                                     hatch_geom['z'],
                                     np.ones(hatch_geom['x'].size))).T

    nodes_rot = np.dot(np.linalg.inv(h.hinge_csys), hatch_geom['nodes'].T).T

    ### Make plot of hatch geometry
    tr = Triangulation(nodes_rot[:, 0],
                       nodes_rot[:, 1], triangles=hatch_geom['tris'])
    # Plot filled contours, and mult tr.x by 0 because that makes everything be same color.
    plt.tricontourf(tr, tr.x * 0.0, colors=(.5, .5, .5))
    plt.gca().set_aspect('equal')

    # Plot the hinge center pt. Marker is red triangle
    plt.scatter(0., 0., marker='^', color='r', label="Hinge loc.")
    # Plot cg point on hatch
    plt.scatter(h.cg_pt[0], h.cg_pt[1], marker='o', color='b', label="CG point")

    ## Plot the CG point position at the min and max force values.
    new_pt = lambda ang: [hatch_obj.r_cg * np.cos(ang), hatch_obj.r_cg * np.sin(ang)]
    min_force_ang = hatch_obj.angle_range[np.argmin(hatch_force_net)]
    max_force_ang = hatch_obj.angle_range[np.argmax(hatch_force_net)]

    plt.scatter(*new_pt(min_force_ang), marker='_', s=100, color='g',
                label="CG @ min force")
    plt.scatter(*new_pt(max_force_ang), marker='+', s=100, color='g',
                label="CG @ max force")

    # Put a legend below the view
    ax3.legend(loc='upper center', bbox_to_anchor=(0.5, -0.05),
               ncol=2, columnspacing=1, fontsize=10, scatterpoints=1)

    # Save results
    fn = "{}_results_{}.png".format(SETTINGS['design_name'], hatch_obj.part_id)
    plt.savefig(fn)
    logging.info("SAVED plot of results for {} to: {}".format(hatch_obj.part_id, fn))


if __name__ == "__main__":
    ## Set up logging
    logging.info("\n" + 50 * "_" + '\nTest bench started\n' + 50 * "_")

    # Initialize API with user-specified settings from separate file
    SETTINGS = tba.load_settings("settings.js")
    logging.debug("Data loader API initialized")

    assembly = Assembly_Info()
    if SETTINGS['show_2d']:
        # Load geom in order to make the "marked points on hatch side view" images. Only load
        #  if we will actually be rendering.
        geo = {}
        for t in hatch_types:
            geo.update(tba.load_geometry({t}))

    # Results will be a dictionary listing min and max force required to open each separate hatch
    all_results = {}
    # Individual hatch metrics stored in sub-dictionary
    all_results["Metrics"] = {}
    all_stress_ok = True

    for h_name in assembly.hatches:
        h = assembly.hatches[h_name]
        logging.debug("Parameters for hatch {}:".format(h.part_id))
        logging.debug("  cg x={}, cg y={}".format(h.cg_pt[0], h.cg_pt[1]))
        logging.debug("  mass={}".format(h.mass))
        logging.debug("  latch position={}".format(h.latch_position))
        logging.debug("  max artic angle={}".format(h.max_ang * 180. / np.pi))
        logging.debug("  ang when closed={}".format(h.ang_closed * 180. / np.pi))
        logging.debug("  has {} axial springs".format(len(h.spr_ax)))
        logging.debug("  has {} torsion bars".format(len(h.spr_tors)))
        logging.debug("  Axial spring mount to body {} and hatch {}".format(h.ax_spr_mt_body,
                                                                            h.ax_spr_mt_hatch))

        # There will always be a hatch
        # Gravity angle spread
        torques = {'Mass': hatch_torque_t30(h)}

        # Calc torque for the optional spring components, if they are present on this hatch
        snum = 1
        for spring_name in h.spr_ax:
            spr = h.spr_ax[spring_name]
            resname = "Spring {}".format(snum)
            # Increment the unique spring # for plotting
            snum += 1
            # Calculate torques and store- one dict entry per helical spring
            logging.debug("{} for {} has the following parameters:".format(spring_name, h.part_id))
            logging.debug(vars(spr))
            torques[resname] = spring_axial_torque_t32(h, h.spr_ax[spring_name])


        # Report whether or not ALL tbars pass stress test, so store list
        snum = 1
        tbar_stresses = []
        for tbar_name in h.spr_tors:
            tbar = h.spr_tors[tbar_name]
            resname = "Tors. bar {}".format(snum)
            # Increment the unique spring # for plotting
            snum += 1
            # Calculate torques and store- one dict entry per torsion bar
            logging.debug("{} for {} has the following parameters:".format(
                          tbar_name, h.part_id))
            logging.debug(vars(tbar))
            torques[resname], tbar_stress_ok = spring_torsion_torque_t31(h, tbar)
            tbar_stresses.append(tbar_stress_ok)
            if not tbar_stress_ok:
                # Remember whether any tbar ever failed due to too much stress
                all_stress_ok = False

        # Add together all torque contributions present
        torque_net = sum(torques[k] for k in torques)

        # Net hatch force (applied at handle)
        hatch_force_net = torque_net / h.latch_position

        # Results dict for this hatch
        hatch_res = {"MAX HATCH FORCE (N)": np.max(hatch_force_net),
                     "MIN HATCH FORCE (N)": np.min(hatch_force_net)}
        if h.spr_tors:
            # Results dict includes whether all tbars were under their max stress
            hatch_res["TORSION BAR STRESS OK"] = all(tbar_stresses)

        logging.info("Results calculated for hatch: {}".format(h.part_id))
        logging.info(hatch_res)

        all_results["Metrics"][h.part_id] = hatch_res

        if SETTINGS['show_2d']:
            ## After running hatch, create results images for debugging
            this_geom = geo[h.part_id]
            plt_res_graphs(h, torques, this_geom)

    ## View all plots
    #plt.show()

    # Add final status of all tests to output
    all_results["RequirementsMet"] = all_stress_ok
    all_results["TestBench"] = "Closures"
    all_results["DesignName"] = SETTINGS["design_name"]

    #############
    ## Save output data
    tba.write_results(all_results)

    logging.info("Done with test bench calculations. Exiting.")
