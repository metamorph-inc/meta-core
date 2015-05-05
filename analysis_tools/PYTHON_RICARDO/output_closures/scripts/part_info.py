"""
Objects intended to extract and store information about various components.

By changing an import, predefined bundles of variables can be used with the same organization;
see predefined.py in this folder. Helpful for testing without api data.

Organization: specific properties of the spring or
"""
import numpy as np


class Torsion_Bar_Info(object):
    """
    Store information about laminated torsion bars as parsed from api part data dict
    """
    def __init__(self, api_part_dict, part_id):
        # Part ID is passed in as string
        self.part_id = part_id

        self.lam_length = api_part_dict['properties']['laminae_length']
        self.lam_width = api_part_dict['properties']['laminae_width']
        self.lam_thick = api_part_dict['properties']['laminae_thickness']
        self.num_lam = api_part_dict['properties']['number_of_laminae']
        self.shear_modulus = api_part_dict['properties']['shear_modulus']
        self.shear_stress_limit = api_part_dict['properties']['max_shear_stress']
        self.windup_angle = api_part_dict['properties']['installed_preload_angle']

        self.stiffness_coeff = 0.333 - (0.2 / (self.lam_width / self.lam_thick))
        self.tors_stiff = ((self.stiffness_coeff * self.num_lam * self.lam_thick ** 3
                           * self.lam_width * self.shear_modulus) /
                           self.lam_length)


class Spring_Info(object):
    """
    Store information about axial springs as parsed from api part data dict

    Organization of this object will likely change as part connectivity files become available-
    in particular the methods to determine hatch/body attachment points may be reorganized
    """
    def __init__(self, api_part_dict, part_id):
        # Part ID is passed in as string
        self.part_id = part_id

        # Properties derived from api call
        self.shear_modulus = api_part_dict['properties']['shear_modulus']
        self.wire_diameter = api_part_dict['properties']['wire_diameter']
        self.coil_diameter = api_part_dict['properties']['coil_diameter']
        self.number_of_coils = api_part_dict['properties']['number_of_coils']
        self.spring_free_length = api_part_dict['properties']['free_length']

        # Properties derived from geometry of hatch and spring. Will be set after spring is
        # instantiated, based on the hatch it is connected to.
        self.hatch_mt_pt = None
        self.body_mt_pt = None


class Hatch_Info(object):
    """
    Store information about hatches, parsed from API nested dictionary of part info. Also store
    associated springs and torsion bars.
    """
    def __init__(self, api_part_dict, part_id):
        # Part ID is passed in as string
        self.part_id = part_id

        # Placeholders for if spring is found (can assoc spring w/hatch)
        self.spr_ax = {}
        self.spr_tors = {}

        # Set placeholders for data extracted from hatch information. Then get.

        # Total hatch mass. Use as an approximation for the rotating mass.
        self.mass = api_part_dict['model_metrics']['TOTAL_MASS']

        # All these explicit conversions will fail if the xml tag has a weird value
        self.latch_position = api_part_dict['model_metrics']['LATCH_POSITION']
        self.max_ang = api_part_dict['properties']['hinge_max_articulation_angle']

        # Coordinate system treated as origin for hatch calculations.
        self.hinge_csys = api_part_dict['datums']['EXT_HINGE_PIN']['global']

        # Coordinate system marking CG for entire hatch assembly (incl non-rotating parts)
        # Coord transform based on center point = last column of 4x4 matrix
        self.cg_csys = api_part_dict['datums']['CG_CSYS']['global']
        self.cg_pt = self._trans2d(self.cg_csys[:, 3])

        # Store the points where axial springs would potentially be mounted if present: rather
        # than determining from hatch and spring geometry, the expected points are defined as two
        # coordinate systems directly in the hatch cad file.

        ax_spr_mt_hatch = api_part_dict['datums']['EXT_AXIAL_MOUNT_1']['global'][:, 3]
        self.ax_spr_mt_hatch = self._trans2d(ax_spr_mt_hatch)

        ax_spr_mt_body = api_part_dict['datums']['EXT_AXIAL_MOUNT_2']['global'][:, 3]
        self.ax_spr_mt_body = self._trans2d(ax_spr_mt_body)

        ###### Calculated parameters
        # CG Locus radius relative to the hinge center position
        self.r_cg = np.linalg.norm(self.cg_pt)

        # Angle of the hatch when closed
        self.ang_closed = np.arcsin(self.cg_pt[1] / self.r_cg)

        # Angle range. Store as object parameter to guarantee that hatch, spring,
        # and tbar all go over exactly the same angle range
        self.angle_range = self._calc_angle_range()

    def _calc_angle_range(self, ang_incr_deg=1.):
        """
        Get the range (min..max) of possible angles that the hatch can be open.
        """
        # Default increment is 1 degree
        ang_increm = np.pi/180 * ang_incr_deg
        return np.arange(self.ang_closed, self.max_ang + ang_increm, ang_increm)

    def _trans2d(self, pt_vec):
        """
        Transform a 1x4 point vector (for use with 4x4 transformation matrix) to the 2d hatch
        plane assumed for all calculations. Rotates, sets z=0, and returns a 1x3 coordinate vector.
        """
        new_vec = np.dot(np.linalg.inv(self.hinge_csys), pt_vec)
        new_vec[2] = 0.
        return new_vec[:3]
