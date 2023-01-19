from rpl.tools.api import test_bench_api
__author__ = 'ab27'

import json, logging, os, sys
import _winreg

import numpy as np
import matplotlib.pyplot as plt

from rpl.tools.api import test_bench_api as tba
import utils.part_info as pinf

import matplotlib.pyplot as plt
from matplotlib.tri import Triangulation
from matplotlib.patches import Wedge

####
# Define reused physical constants
g = 9.807  # m/s**2 - ensure that units are always consistent with this!
hatch_types = {'Hatch_Assembly_Cargo', 'Hatch_Assembly_Driver_Commander'}


#######
# Setup for VU test bench environment
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
# TODO: Uncomment next line when deploying to VU environment
#tools = query_analysis_tools()
#sys.path.append(os.path.join(tools['hatch_force']['InstallLocation']))


def excepthook(*args):
    """
    Ensure that any uncaught exception is logged
    """
    logging.getLogger().error('Uncaught exception: ', exc_info=args)


def get_parts_of_interest(part_type="all"):
    """
    Specify the parts of interest for fetching data via API

    Return separate dictionaries for separate hatch and spring types- allows script to adapt
    based on what components are actually present
    """
    hatch_prms = {"Property": ['hinge_max_articulation_angle'],
                  "ModelMetric": ['LATCH_POSITION', 'TOTAL_MASS'],
                  "Datum": ['CG_CSYS']}
    # TODO: Don't use cg_location; unreliable for parametric hatches. Get position from CG_CSYS
    # for now

    # TODO: uses separate coord system markings for Cmdr vs Cargo hatches for now
    htc_cmdr = hatch_prms.copy()
    htc_cmdr['Datum'] = ['CG_CSYS', 'EXT_SPRING_3']

    htc_cargo = hatch_prms.copy()
    htc_cargo["Datum"] = ['CG_CSYS', 'EXT_SPRING_1']

    poss_parts = {"Hatch_Assembly_Driver_Commander": htc_cmdr,
                  "Hatch_Assembly_Cargo": htc_cargo,
                  "Spring_Torsion_Laminated":
                      {"Property": ['installed_preload_angle', 'laminae_thickness',
                                    'laminae_width', 'max_shear_stress', 'number_of_laminae',
                                    'shear_modulus']},
                  "Spring_Axial_Helical":
                      {"Property": ['coil_diameter', 'free_length', 'number_of_coils',
                                    'shear_modulus', 'wire_diameter']}}

    if part_type == "all":
        return poss_parts
    elif part_type in poss_parts:
        return {part_type: poss_parts[part_type]}
    else:
        # Todo find a better place to raise this exception
        raise Exception("Requested an unrecognized component type in get_parts_of_interest")


class Assembly_Info(object):
    """
    Fetch relevant information about the overall assembly, including:
    - What types of hatches and springs there are
    - How they are connected
    - Relative geometry of parts (spring positions relative to hatch)

    The total torque consists of three components:
    1) the torque due to the hatch mass;
    2) the torque from torsion spring bars/laminated springs (if present), and the torque
        contributions from axial spring inputs
    """
    def __init__(self):
        self.hatches = []
        self.get_hatches()

        # After assembly initialization, run sanity check
        assert len(self.hatches) > 0, "No hatches were found in this assembly."

    def get_hatches(self):
        # Fetch data, then convert to objects
        for t in hatch_types:
            # Get data for all hatches of this type, then create Hatch objs for all hatches of
            # that type in the vehicle (eg 2 cargo hatches)

            # TODO: Note [t] indexing and order of looping; we're only querying one hatch type at
            #  a time (due to chamfer/articulation angle difference)
            hatch_type_data = tba.get_data(get_parts_of_interest(t))[t]

            for hatch_name in hatch_type_data:
                self.hatches.append(pinf.Hatch_Info(hatch_type_data[hatch_name], hatch_name))

    def get_axial_springs(self):
        # TODO: May need to reorganize this in order to get springs per hatch. Func org will
        # depend on connectivity file data org and how api queries.
        pass

    def get_torsion_bars(self):
        pass


def hatch_torque_t30(hatch):
    """
    Determine the torque required to open the hatch by itself (due to hatch mass)
    """
    # TODO: this assumes all calculations are in a certain unit and no conversion is required.
    cg_moment_arm = [hatch.r_cg * np.cos(hatch.ang_closed + ang + phi) for ang in
                     hatch.angle_range]
    t_hatch_mass = hatch.mass * g * np.array(cg_moment_arm)
    return t_hatch_mass


def plt_geom(hatch_geom):
    """
    Plot the 2D cross section of a hatch, showing:
    - Side view of hatch
    - Marked CG point in 2D projection
    - Hinge position, marked
    """
    # Start with geometry fetching
    # May need to merge xyz vectors into node array first, in case we do any coord transformations
    pad_vec = np.ones(hatch_geom['x'].shape)
    hatch_geom['nodes'] = np.vstack((hatch_geom['x'], hatch_geom['y'],
                                hatch_geom['z'], pad_vec)).T
    plt.figure()
    tr = Triangulation(hatch_geom['nodes'][:,0], this_geom['nodes'][:,1],
                              triangles=this_geom['tris'])
    plt.tricontourf(tr, tr.x*0.0); plt.gca().set_aspect('equal')
    plt.show()




if __name__ == "__main__":
    #############
    # User specified settings from separate file
    with open(r"settings.js", "r") as f:
        SETTINGS = json.load(f)

    ## Set up logging
    logging.info("\n"+50*"_"+'\nTest bench started\n'+50*"_")
    # Uncaught exceptions and assertions should write error to log
    sys.excepthook = excepthook

    # Initialize API with settings
    tba.load_settings(SETTINGS)
    logging.debug("Data loader API initialized")

    phi = 0.0
    assembly = Assembly_Info()

    geo = {}
    for t in hatch_types:
        geo.update(tba.load_geometry({t}))

    show3d = False
    if show3d:
        from mayavi import mlab
        from itertools import cycle
        hatch_colors = cycle([(.8, .4, .4),
                              (.4, .8, .4),
                              (.4, .4, .8),
                              (.8, .8, .4)])

    for h in assembly.hatches:
        hatch_torques = hatch_torque_t30(h)
        this_geom = geo[h.part_id]
        pad_vec = np.ones(this_geom['x'].shape)

        this_geom['nodes'] = np.vstack((this_geom['x'],this_geom['y'],
                                        this_geom['z'], pad_vec)).T

        # Coordinates should align with hinge csys
        transformed_crds = np.dot(this_geom['nodes'], h.hinge_csys)

        if show3d:
            # Plot cg pt projected onto desired plane in blue
            mlab.points3d(*h.cg_pt, scale_factor=0.1, color=(0, 0, 1))
            # Plot CG point (in 3d) from cg_csys in green
            mlab.points3d(*h.cg_csys[:-1, 3], scale_factor=0.1, color=(0, 1, 0))
            # Plot hinge csys center pt (in 3d) in white
            mlab.points3d(*h.hinge_csys[:-1, 3], scale_factor=0.1)

            # Plot hatch geom in an appropriate color
            h_geo = geo[h.part_id]
            mlab.triangular_mesh(h_geo['x'], h_geo['y'], h_geo['z'], h_geo['tris'],
                                 color=hatch_colors.next())

            # Plot lines corresponding to at very least the xy plane that we're projecting cg
            # points onto
            pln_csys = h.hinge_csys
            pln_orig = pln_csys[:-1, 3]
            v_x = np.dot(pln_csys[:3, :3], np.array([1, 0, 0]))
            v_y = np.dot(pln_csys[:3, :3], np.array([0, 1, 0]))
            v_z = np.dot(pln_csys[:3, :3], np.array([0, 0, 1]))
            # vec = orig--> orig + vec
            x_line_end = pln_orig + v_x
            y_line_end = pln_orig + v_y
            z_line_end = pln_orig + v_z

            mlab.plot3d(*zip(pln_orig, x_line_end), line_width=2, color=(1, 0, 0))
            mlab.plot3d(*zip(pln_orig, y_line_end), line_width=2, color=(0, 1, 0))
            mlab.plot3d(*zip(pln_orig, z_line_end), line_width=2, color=(0, 0, 1))
        else:
        # Plot hatch opening force (max)
            plt.figure()
            # TODO: plot visualization should be relative to the marked hinge coord system
            tr = Triangulation(-this_geom['x'], this_geom['y'], triangles=this_geom['tris'])
            # Plot filled contours, and mult tr.x by 0 because that makes everything be 1 color.
            plt.tricontourf(tr, tr.x*0.0)
            plt.gca().set_aspect('equal')

            # Superimpose interesting hatch points on this figure
            # Want the coords in 2d, so...
            # Imperfect HACK in case the z coord isn't the one == 0

            # Plot the CG point overlaid on the plane. Marker is blue circle
            print("cg pt", h.cg_pt[:2])
            plt.plot(-h.cg_pt[0], h.cg_pt[1], marker='o', color='b')
            # Plot the hinge center pt. Marker is red triangle
            print("hcsys all", h.hinge_csys[:-1, 3])
            plt.plot(-h.hinge_csys[0, 3], h.hinge_csys[1, 3], marker='^', color='r')

    if show3d:
        mlab.show()
    else:
        plt.show()