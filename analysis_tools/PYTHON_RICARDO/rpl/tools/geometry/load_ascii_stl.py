import numpy as np
import logging

import random

def open_stl(filename):
    count = 0
    with open(filename) as f:
        for line in f:
            count += 1
    logging.info("number of lines {}".format(count))
    tri_count = (count - 2) / 7
    logging.info("number of triangles {}".format(tri_count))
    vert_count = tri_count * 3
    logging.info("number of vertices {}".format(vert_count))
    
    x = np.zeros((tri_count, 3))
    y = np.zeros((tri_count, 3))
    z = np.zeros((tri_count, 3))
    i = 0
    j = 0
    with open(filename) as f:
        for line in f:
            
            if "vertex" in line:
                tokens = line.split()
                x[i][j] = float(tokens[1])
                y[i][j] = float(tokens[2])
                z[i][j] = float(tokens[3])
                j += 1
                if j == 3:
                    j = 0
                    i += 1


    ratio = tri_count / 100000
    
    if ratio >= 2:
        x = x[::ratio,:]
        y = y[::ratio,:]
        z = z[::ratio,:]

        tri_count = tri_count / ratio 

    triangles = [None] * tri_count
    for i in xrange(tri_count):
        v = i * 3
        triangles[i] = (v, v+1, v+2)
        
    
    
    return x.flatten(), y.flatten(), z.flatten() , triangles  
    
if __name__ == "__main__":
#    x, y, z, triangles = open_stl("STL_INSTANCES\\50_cat_3446170_3_d.stl")
    x, y, z, triangles = open_stl("20_allison_x4560_1_e.stl")
    
    from mayavi import mlab
    s = mlab.triangular_mesh(x, y, z, triangles, color=(random.random() / 2 + 0.5,
                                                        random.random() / 2 + 0.5,
                                                        random.random() / 2 + 0.5
                                                        ))
    mlab.show()
