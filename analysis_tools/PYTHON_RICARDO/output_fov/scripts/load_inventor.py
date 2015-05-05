# -*- coding: utf-8 -*-
"""
Created on Mon Aug 12 16:56:42 2013

@author: tgb
"""
from cStringIO import StringIO
import re

import numpy as np



def read_iv_file(f_name):
    """
    Return a list of parts from the inventor file called ``f_name``.
    
    Each entry in the part list is a tuple ``(x, y, z, tris, edges)``.  ``x``, ``y`` and ``z`` are 
    1D numpy arrays containing the node positions, ``tris`` is numpy array of triples indexing 
    which nodes make the triangles and ``edges`` contains which nodes make the sharp edges in the 
    part.
    """
    with open(f_name, "r") as iv_file:
        
        point_sets = []
        n_str = None
        p_str = None
        in_points = False
        in_normals = False
        for line in iv_file:
            if in_points:
                if "]" in line:
                    in_points = False
                    p_str.write(line[:line.find("]")])
                    point_sets.append(_get_points_tris(p_str.getvalue(), 
                                                       n_str.getvalue()))

                else:
                    p_str.write(line)
            elif in_normals:
                if "]" in line:
                    in_normals = False
                    n_str.write(line[:line.find("]")])
                else:
                    n_str.write(line)
            else:               
                if re.search(r"point\s+\[", line):
                    p_str = StringIO()
                    in_points = True
                    p_str.write(line[line.find("[") + 1:])
                if re.search(r"vector\s+\[", line):
                    n_str = StringIO()
                    in_normals = True
                    n_str.write(line[line.find("[") + 1:])

    total_verts = total_tris = 0
    t_x = np.empty((0,), dtype=np.float32)
    t_y = np.empty((0,), dtype=np.float32)
    t_z = np.empty((0,), dtype=np.float32)
    t_tris = np.zeros((0, 3), dtype=np.uint32)
    for x, y, z, triangles in point_sets:
        total_verts += len(x)
        total_tris += len(triangles)
        
        t_tris = np.concatenate((t_tris, triangles + len(t_x)))

        t_x = np.concatenate((t_x, x))
        t_y = np.concatenate((t_y, y))
        t_z = np.concatenate((t_z, z)) 

    return {"x" : t_x, "y" : t_y, "z" : t_z, "tris" : t_tris}
     
    
        
def _get_points_tris(point_set, norm_sets):
    """
    Merge points together (unless ``merge_nodes == False``) and build triangle and edge lists.

    Returns x, y, z, tris, edges
    """
    point_set = re.sub(r",\n", " ", point_set).split()
    point_set = np.array([float(x) for x in point_set], dtype=np.float32)       
    point_set.shape = (point_set.size / 3, 3)    

    tris = np.arange(point_set.size / 3, dtype=np.uint32)            
    tris.shape = (tris.size / 3, 3)   
    

    return (point_set[:, 0], point_set[:, 1], point_set[:, 2], tris)




