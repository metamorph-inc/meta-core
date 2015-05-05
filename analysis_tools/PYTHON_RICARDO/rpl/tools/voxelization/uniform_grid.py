""" Represent a triangulated surface using a 3D boolean grid"""
import os

import numpy as np
from scipy import ndimage
from test_benches.tools.ray_tracing.bsp_tree_poly import BSP_Element
from test_benches.tools.geometry.load_ascii_stl import open_stl

class BSP_Grid(object):
    def __init__(self, node_array, tris, allocate_step=100000):
        ## Store the triangles with an enumeration so that even when they are subdivided thier
        ## identity is not lost
        self.orig_tris = np.array([[t[0], t[1], t[2], i] for i, t in enumerate(tris)], 
                                  dtype=np.int32)
        ## Have to pad to be able to use numpy arrays while the shapes are allowed to be polys
        padded = [[t[0], t[1], t[2], -1, -1, -1, -1, -1, i] for i, t in enumerate(tris)]
        self.tris = np.array(padded, dtype=np.int32)
        self.allocate_step = allocate_step
        self.node_array = node_array # Reference to the full list of nodes
        self._resize()
        self.next_free = len(node_array)
        self.split_cache = np.zeros(len(self.tris),dtype=np.int32)

    def _resize(self):
        """
        Increase node array size by the allocate_step amount.
        """
        self.array_size = len(self.node_array) + self.allocate_step
        self.node_array = np.concatenate((self.node_array, np.zeros((self.allocate_step, 3))))
        
    def add_node(self, node):
        """
        Adds a new node to the end of the node array (expanding if required) returns the index of 
        the newly added node.
        """
        if self.next_free == self.array_size:
            self._resize()
        self.node_array[self.next_free] = node
        self.next_free += 1
        return self.next_free - 1



voxel_size = 0.05

#stl_file = "50_standing"
stl_file = "50p_hunch8"
#stl_file = "volume_w_seats"
#stl_file = "tom_vision_study_03jun13"

## Load the geometry and scale it to m
if stl_file.startswith("50"):
    manikin = True
    x, z, y, triangles = open_stl(stl_file + ".stl")
else:
    manikin = False
    x, y, z, triangles = open_stl(stl_file + ".stl")
x *= 0.001
y *= 0.001
z *= 0.001
nodes = np.vstack((x, y, z)).T
tris = np.array(triangles)


margin = 0.0 if manikin else 1.0

x_min, x_max = np.min(x) - voxel_size - margin, np.max(x) + voxel_size + margin
y_min, y_max = np.min(y) - voxel_size - margin, np.max(y) + voxel_size + margin
z_min, z_max = np.min(z) - voxel_size - margin, np.max(z) + voxel_size + margin

print x_min, x_max
print y_min, y_max
print z_min, z_max

b_tree = BSP_Grid(nodes, tris)

b_x_root = BSP_Element(b_tree.tris, b_tree)

#x_slices = []

x_coords = np.arange(x_min, x_max + voxel_size, voxel_size) 
y_coords = np.arange(y_min, y_max + voxel_size, voxel_size) 
z_coords = np.arange(z_min, z_max + voxel_size, voxel_size) 


size_i = len(x_coords)
size_j = len(y_coords)
size_k = len(z_coords)

print "num_i", size_i
print "num_j", size_j
print "num_k", size_k

print "sample_points", size_i * size_j * size_k

file_name = "voxels_temp_" + stl_file + "_" + str(voxel_size) + ".npz"

if os.path.exists(file_name):
    d = np.load(file_name)
    in_out = d["in_out"]
else:

    
    in_out = np.empty((size_i - 1, size_j - 1, size_k -1), dtype=np.bool)
    
    #print tgb
    for i, x_pos in enumerate(x_coords[1:]):
        print "Slice {} of {}".format(i, size_i)
    #    print "\n\n-----------------------------------------------------------------------------\n\n"
        b_above_x, b_below_x = b_x_root.split_at(0, x_pos)
        b_y_root = b_below_x
        for j, y_pos in enumerate(y_coords[1:]):
    #        print ""
            b_above_y, b_below_y = b_y_root.split_at(1, y_pos)
            b_z_root = b_below_y
            for k, z_pos in enumerate(z_coords[1:]):
                b_above_z, b_below_z = b_z_root.split_at(2, z_pos)
        #        print "Y", y_pos, len(b_below_y.tris)
                in_out[i, j, k] = len(b_below_z.tris) == 0
    
                    
                b_z_root = b_above_z
            b_y_root = b_above_y
        b_x_root = b_above_x
    
    
    np.savez(file_name, in_out=in_out, x_coords=x_coords, y_coords=y_coords, z_coords=z_coords)



labels, count = ndimage.measurements.label(in_out)

label_corner = labels[-1, -1, -1]

## Find floor

make_single_solid = manikin

if make_single_solid:
    
    in_out = labels != label_corner  
    labels, count = ndimage.measurements.label(in_out)
    label_corner = labels[-1, -1, -1]

#    in_out = ndimage.binary_erosion(in_out, iterations=1)

#exclude = labels != label_corner
#
#in_out &= exclude

#in_out = ndimage.binary_opening(in_out, iterations=4)

#labels, count = ndimage.measurements.label(in_out)
#label_corner = labels[-1, -1, -1]

for obj_num in range(count + 1):
    print obj_num, np.sum(labels == obj_num)


show_3d = True
                       

if show_3d:
    from itertools import cycle
    
    from mayavi import mlab
    bright_colors = cycle([(0.5, 1.0, 1.0),
                           (1.0, 0.5, 1.0),
                           (1.0, 1.0, 0.5),
                           (1.0, 0.75, 0.75),
                           (0.75, 1.0, 0.75),
                           (0.75, 0.75, 1.0),
                           (0.83, 0.83, 0.83)])
#    for obj_num in range(1, 3):
#    for obj_num in range(0, count + 1):
    if manikin:
        low, high = 1, 2
        mode = "cube"
    else:
        low, high = 0, count + 1
        mode = "2dvertex"                       
                           
    for obj_num in range(low, high):
        if obj_num == label_corner:
            continue
        color = (0.5, 0.5, 1.0) if obj_num == 0 else next(bright_colors)
        hit = []
        floor = []
        print obj_num, np.sum(labels == obj_num)
        for i, x_pos in enumerate(x_coords[1:]):
            print "Slice {} of {}".format(i, len(x_coords))
            for j, y_pos in enumerate(y_coords[1:]):
                for k, z_pos in enumerate(z_coords[1:]):
                    if labels[i, j, k] == obj_num:

                        hit.append((x_pos, y_pos, z_pos))
                        if obj_num != 0 and not manikin and labels[i, j - 1, k] == 0:
                            ## Calculate height above floor
                            floor.append((x_pos, y_coords[1:][j - 1], z_pos))
        hit_t = np.array(hit).transpose()                
        mlab.points3d(hit_t[0], hit_t[1], hit_t[2], color=color,
                      scale_mode="none", scale_factor=voxel_size, mode=mode)
                
        if not manikin and floor:
            floor_t = np.array(floor).transpose() 
            mlab.points3d(floor_t[0], floor_t[1], floor_t[2], color=(0.4, 0.4, 0.4),
                          scale_mode="none", scale_factor=voxel_size, mode="cube")


    ref_position = [(x_coords[len(x_coords) / 2], y_coords[0], z_coords[len(z_coords) / 2])]

    for q in range(8):
        print q, (1 & q), (2 & q), (4 & q)
        ref_position.append((x_coords[(len(x_coords) - 1) * (1 & q)], 
                             y_coords[(len(y_coords) - 1) * (2 & q) / 2],
                             z_coords[(len(z_coords) - 1) * (4 & q) / 4]))
        

    ref_pos_t = np.array(ref_position).transpose()
           
    mlab.points3d(ref_pos_t[0], ref_pos_t[1], ref_pos_t[2], color=(1.0, 0.2, 0.2),
                  scale_mode="none", scale_factor=voxel_size, mode="cube")


    mlab.show()
