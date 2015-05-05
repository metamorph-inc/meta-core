"""
Utilities helpful for handling the vehicle model: find floor/ceiling, convert coordinates to
voxel grid points, label regions (and based on a specific point), etc.
"""
import logging
import numpy as np
from scipy.ndimage import measurements as meas


class Component(object):
    """Object to store information about a specific vehicle component, manikin, etc."""
    def __init__(self, voxel_data_dict, vehicle_csys=None, voxel_size=None):
        """
        :param voxel_data_dict: Output of voxelization routine, usually read in from file.
        :param voxel_size: The spacing between adjacent voxels.
        :param vehicle_csys: The transform matrix to go to the vehicle csys; usually ignored
        """
        # Allow occupied voxels to be a string (file location) or numpy array (data)

        self.voxel_size = voxel_size or voxel_data_dict['vox_size']
        self.vehicle_csys = vehicle_csys if vehicle_csys is not None else np.eye(4)

        # Hack so that we can use old manikins where voxels are stored as occupied_voxels
        try:
            self.occupied_voxels = voxel_data_dict["value"]
        except KeyError:
            self.occupied_voxels = voxel_data_dict["occupied_voxels"]

        self.shape = self.occupied_voxels.shape

        self.labels = None
        self.labels_binary = None

        try:
            # Load in x,y,z grid if available. Otherwise can create arbitrary grid on demand.
            self.x_grid = voxel_data_dict["x_grid"]
            self.y_grid = voxel_data_dict["y_grid"]
            self.z_grid = voxel_data_dict["z_grid"]
        except KeyError:
            logging.debug("No predefined gridpoints.")
            self.x_grid = None
            self.y_grid = None
            self.z_grid = None

    def _make_labeled_regions(self, mask_from_voxel=None):
        """
        Label regions of EMPTY space in the voxelated grid. Can be used for binarization to find
        vehicle interior (as well as floor and ceiling, based on vehicle interior)

        Optionally creates true/false mask based on the ijk voxel indices of a selected point.
        """
        # Labeled image is converted to boolean, because ndimage thinks 0=background

        self.labels, _ = meas.label((self.occupied_voxels == 0))
        # Store whether labels are a binary mask (desired region/not), or a list of indiv regions
        self.labels_binary = mask_from_voxel
        if mask_from_voxel is not None:
            # (sans a buffer region, this method of empty space detection may cause problems)
            label_desired = self.labels[mask_from_voxel]
            self.labels = (self.labels == label_desired)
            self.labels_binary = mask_from_voxel
        return self.labels

    def _make_coord_grid(self, csys=None):
        """
        Originating at the given coord system, provide a coordinate grid to match voxel numbers
        (giving xyz positions of voxel[i,j,k]). (so doesn't give range such as -x...+x)

        :param csys: Defaults to origin; else 4x4 transformation matrix representing the
        coordinate system in which to rotate the coordinates and make the grid
        """
        if csys is None:
            csys = np.identity(4)

        origin = csys[:-1, 3]
        self.x_grid = np.arange(origin[0] * self.voxel_size,
                                origin[0] * self.voxel_size + self.shape[0] * self.voxel_size,
                                self.voxel_size)
        self.y_grid = np.arange(origin[1] * self.voxel_size,
                                origin[1] * self.voxel_size + self.shape[1] * self.voxel_size,
                                self.voxel_size)
        self.z_grid = np.arange(origin[2] * self.voxel_size,
                                origin[2] * self.voxel_size + self.shape[2] * self.voxel_size,
                                self.voxel_size)

        return self.x_grid, self.y_grid, self.z_grid

    def get_vox_from_coord(self, coord_vec):
        """
        Turn xyz coordinate vector (or list) into tuple of the ijk voxel #s (array indices in
        occupied_voxels).
        """
        assert len(coord_vec) == 3, "Specified coordinate vector of incorrect size"

        # Correct for diff in alignment between vehicle coords and occ voxel grid
        coord_vec = np.dot(np.array(coord_vec), self.vehicle_csys[:3, :3])
        x_pos = (i for i, c in enumerate(self.x_grid) if coord_vec[0] <= c)
        y_pos = (i for i, c in enumerate(self.y_grid) if coord_vec[1] <= c)
        z_pos = (i for i, c in enumerate(self.z_grid) if coord_vec[2] <= c)

        return x_pos.next(), y_pos.next(), z_pos.next()

    def get_labels(self, mask_from_voxel=None):
        """
        Return or create labels as needed
        """
        if self.labels is not None and self.labels_binary == mask_from_voxel:
            return self.labels
        # If the labels don't exist or don't match what we want, regenerate
        return self._make_labeled_regions(mask_from_voxel=mask_from_voxel)

    def get_coord_grid(self):
        """
        Return or create the coordinates along a voxelated grid, as needed
        """
        if self.x_grid is not None and self.y_grid is not None and self.z_grid is not None:
            return self.x_grid, self.y_grid, self.z_grid
        else:
            return self._make_coord_grid()


class Vehicle(Component):
    """
    Add specific methods for finding floor and ceiling
    """
    def __init__(self, voxel_data_dict, vehicle_csys=None, voxel_size=None):
        """
        Call constructor of superclass and then add on two more empty parameters
        """
        super(Vehicle, self).__init__(voxel_data_dict,
                                      vehicle_csys=vehicle_csys,
                                      voxel_size=voxel_size)
        self.floor = None
        self.ceiling = None

    def _make_floor_ceil(self, cabin_voxel):
        """
        Alternate method of ceiling detection: get the label in a region containing troops (
        assumed to be the cabin interior volume), then find min and max points where that label
        is found, everywhere in the vehicle. Floor and ceiling are the endpoints of the longest
        continuous gap between floor and ceiling.

        :param cabin_voxel: 3-tuple containing the ijk indices of a voxel known to be cabin-
            determine this from the position of a troop manikin in the vehicle model
        """
        # Default value = bottom of vehicle box. Easy to spot meaningless ceiling points.

        labels = self.get_labels(mask_from_voxel=cabin_voxel)

        self.ceiling = np.zeros((labels.shape[0], labels.shape[1]), dtype=np.int16)
        self.floor = np.zeros((labels.shape[0], labels.shape[1]), dtype=np.int16)
        for i in xrange(labels.shape[0]):
            for j in xrange(labels.shape[1]):
                labs, isl = meas.label(labels[i, j, :])
                if isl == 0:
                    continue
                slices = meas.find_objects(labs)
                lrgst = np.argmax(np.array([sli[0].stop - sli[0].start for sli in slices]))
                
                self.floor[i, j] = slices[lrgst][0].start - 1
                self.ceiling[i, j] = slices[lrgst][0].stop
        # Hack: postprocess so that floor and ceiling arrays have the default values assumed
        # by rest of test bench
        self.floor[self.floor == -1] = 0
        self.ceiling[self.ceiling == labels.shape[2]] = 0

    def get_floor(self, cabin_voxel=None):
        """
        Return vehicle floor. Calculate if necessary.
        """
        if self.floor is None:
            self._make_floor_ceil(cabin_voxel)
        return self.floor

    def get_ceil(self, cabin_voxel=None):
        """
        Return vehicle ceiling. Calculate for the first time if necessary.
        """
        if self.ceiling is None:
            self._make_floor_ceil(cabin_voxel)    
        return self.ceiling
