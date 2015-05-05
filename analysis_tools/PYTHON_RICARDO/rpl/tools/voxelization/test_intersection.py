""" Represent a triangulated surface using a 3D boolean grid"""
import numpy as np
from scipy import ndimage
import matplotlib.pyplot as plt
import matplotlib.cm as cm


voxel_size = 0.05

#stl_file = "50_standing"
#stl_file = "50p_hunch8"
stl_file = "volume_w_seats"

file_name = "voxels_temp_" + stl_file + "_" + str(voxel_size) + ".npz"

d = np.load(file_name)
in_out = d["in_out"]

#stl_file = "50_standing"
stl_file = "50p_hunch8"
file_name = "voxels_temp_" + stl_file + "_" + str(voxel_size) + ".npz"

in_out_manikin_base = np.load(file_name)["in_out"]



labels, count = ndimage.measurements.label(in_out)

label_corner = labels[-1, -1, -1]


exclude = labels != label_corner

in_out &= exclude

#in_out = ndimage.binary_opening(in_out, iterations=4)

labels, count = ndimage.measurements.label(in_out)
label_corner = labels[-1, -1, -1]

floor = np.ones((in_out.shape[0], in_out.shape[2]), dtype=np.uint16) * (in_out.shape[1] - in_out_manikin_base.shape[1])
#floor = np.ones((in_out.shape[0], in_out.shape[2]), dtype=np.uint16) * (in_out.shape[1])



for obj_num in range(count + 1):
    print obj_num, np.sum(labels == obj_num)
    if obj_num == label_corner:
        continue
    print obj_num, np.sum(labels == obj_num)
    for i in xrange(in_out.shape[0]):
        print "Slice {} of {}".format(i, in_out.shape[0])
        for j in xrange(in_out.shape[1]):
            for k in xrange(in_out.shape[2]):
                if labels[i, j, k] == obj_num:
                    if labels[i, j - 1, k] == 0:
                        floor[i, k] = min(floor[i, k], j)



full = in_out_manikin_base.shape[1]

manikin_heights = [full, full - full // 10, full - full // 4] 
manikin_heights = [full] 

print manikin_heights

v_min = None
v_max = None

for config, manikin_height in enumerate(manikin_heights):
    in_out_manikin = in_out_manikin_base[:, :manikin_height, :]
#    in_out_manikin = in_out_manikin_base[:, :, :]
    labels, count = ndimage.measurements.label(in_out_manikin)
    label_corner = labels[-1, -1, -1]
    
    in_out_manikin = labels != label_corner
    
    manikin_volume = np.sum(in_out_manikin)
    
    manikin_i = in_out_manikin.shape[0]
    half_i = manikin_i // 2
    manikin_j = in_out_manikin.shape[1]
    manikin_k = in_out_manikin.shape[2]
    half_k = manikin_k // 2
    
    
    print in_out.shape
    print in_out_manikin.shape
    
    intersection_checks = 0
    
    results = np.empty((in_out.shape[0] - manikin_i, in_out.shape[2] - manikin_k), dtype=np.float)
    
    for i in xrange(in_out.shape[0] - manikin_i):
        for k in xrange(in_out.shape[2] - manikin_k):
            results[i, k] = np.sum(in_out_manikin & in_out[i : i + manikin_i, 
                                                           floor[i + half_i, k + half_k] : floor[i + half_i, k + half_k] + manikin_j,
                                                           k : k + manikin_k])
            intersection_checks += 1
    
    print "Intersection checks", intersection_checks                                        
     
    
#    results = results / manikin_volume
    results = (manikin_volume - results) / manikin_volume

    
    if v_min is None:
        v_min = np.min(results)
        v_max = np.max(results)
    
    plt.subplot(1, len(manikin_heights), config)
    plt.imshow(results, cmap=cm.copper_r, vmin=v_min, vmax=v_max, norm=None)

#plt.subplot(122)
#plt.imshow(floor, cmap=cm.copper_r)

plt.show()



                                             
#size_check = 50
#
#block_check = np.ones((size_check, size_check, size_check), dtype=np.bool)
#
#
#for i in xrange(in_out.shape[0] - size_check):
#    for j in xrange(in_out.shape[1] - size_check):
#        print i, j, np.sum(block_check & in_out[i : i + size_check, 
#                                                j : j + size_check,
#                                                :size_check])
#                                                
                                                
                                                