""" Provide geometric transformations and other utilities. """
from math import sin, cos, sqrt

import numpy as np
from lxml import etree

def rotation_about_vector(vec, t, mat=None):
    """
    Return a transform matrix to rotate ``t`` radians around unit vector ``vec``.
    """
    l = vec[0]
    m = vec[1]
    n = vec[2]
    st = sin(t)
    ct = cos(t)
    if mat is not None:
        ## Use the supplied array to hold the result instead of making a new one
        mat[0] = l*l*(1-ct)+ct, m*l*(1-ct)-n*st, n*l*(1-ct)+m*st, 0
        mat[1] = l*m*(1-ct)+n*st, m*m*(1-ct)+ct, n*m*(1-ct)-l*st, 0
        mat[2] = l*n*(1-ct)-m*st, m*n*(1-ct)+l*st, n*n*(1-ct)+ct, 0
        mat[3] = 0,0,0,1
        return mat
    return np.array([[l*l*(1-ct)+ct, m*l*(1-ct)-n*st, n*l*(1-ct)+m*st, 0],
                      [l*m*(1-ct)+n*st, m*m*(1-ct)+ct, n*m*(1-ct)-l*st, 0],
                      [l*n*(1-ct)-m*st, m*n*(1-ct)+l*st, n*n*(1-ct)+ct, 0],
                      [0,0,0,1]])

def translate(vec):
    """
    Return a transform matrix to translate by vector ``vec``.
    """
    l = vec[0]
    m = vec[1]
    n = vec[2]
    return np.array([[1.0, 0, 0, l],
                      [0, 1.0, 0, m],
                      [0, 0, 1.0, n],
                      [0, 0, 0, 1.0]])


origin = np.array([0, 0, 0, 1.0])

def get_translation(*args):
    """
    Return the translation component of a translation matrix or sequence of translation matricies.
    """
    arg_org = list(args)
    arg_org.append(origin)
    return mul(*arg_org)[:3]
    
def mul(*args):
    """
    Return the resultant translation matrix from multiplying a supplied sequence of matricies.
    """
    return reduce(np.dot, args)

pos_x = np.array([1, 0, 0])
pos_y = np.array([0, 1, 0])
pos_z = np.array([0, 0, 1])
                      
def normalize(vec, degen=pos_x):
    """
    Return a new vector representing ``vec`` with a length of 1.
    
    If ``vec`` is degenerate (length < 1e-10) returns ``degen`` instead.
    """
    abs_n_vec = sqrt(np.dot(vec, vec))
    return degen if abs_n_vec < 1e-10 else vec / abs_n_vec
    


def matrix_from_nx_ny_ro(n_x, n_y, r_o):
    """
    Return a transform matrix that moves to r_o and aligns the local x, y with n_x, n_y
    
    Doesn't use n_x and n_y directly because they may not be perfectly perpendicular.  The 
    disadvantage is 2 cross products are needed instead of 1.
    """
    e_x = normalize(n_x, pos_x)
    e_z = normalize(np.cross(e_x, n_y), pos_z)
    
    mat = np.eye(4)
    mat[0, :3] = e_x
    mat[1, :3] = np.cross(e_z, e_x)
    mat[2, :3] = e_z
    
    mat = mat.T
    mat[0, 3] = r_o[0]
    mat[1, 3] = r_o[1]
    mat[2, 3] = r_o[2]
    return mat

def trans_mat_from_xml(elem):
    """
    Take an xml element that represents a Coord_System_Definition and return a trans matrix.
    """
    r_o = np.array([float(f) for f in elem.xpath(r"./*[@name = 'position']//@value")])
    n_x = np.array([float(f) for f in elem.xpath(r"./*[@name = 'local_x']//@value")])
    n_y = np.array([float(f) for f in elem.xpath(r"./*[@name = 'local_y']//@value")])
    return matrix_from_nx_ny_ro(n_x, n_y, r_o)

xpath_val = etree.XPath(r".//*[@Name = $n]/Value/ValueExpression/Value")

def trans_mat_from_avm_xml(elem):
    """
    Take an xml element that represents a Coord_System_Definition and return a trans matrix.
    """
    r_o = np.array([float(f) for f in xpath_val(elem, n="position")[0].text[1:-1].split(",")])
    n_x = np.array([float(f) for f in xpath_val(elem, n="local_x")[0].text[1:-1].split(",")])
    n_y = np.array([float(f) for f in xpath_val(elem, n="local_y")[0].text[1:-1].split(",")])

    return matrix_from_nx_ny_ro(n_x, n_y, r_o)

def ray_sphere_intersection(ray_o, ray_t, cen, rad):
    """
    Calculate the intersection point of a ray with a sphere.
    """
    d = normalize(ray_t - ray_o, pos_x)
    offset = ray_o - cen
    A = np.dot(d, d)
    
    B = np.dot(2 * offset, d)
    
    C = np.dot(offset, offset) - rad * rad
    discr = B * B - 4 * A * C
    if discr < 0.0:
        return None
     
    ## Only ever need the far point      
    t_1 = (-B + sqrt(discr)) / (2 * A)        
    return ray_o + d * t_1
    

def ray_plane_intersection(ray_o, ray_t, plane_normal, point_on_plane):
    """
    Return the intersection point of a ray with a plane.
    
    Returns ``None`` if no intersection is found.
    """
    
    plane_d = -np.dot(plane_normal, point_on_plane)   
    
    V = normalize(ray_t - ray_o, pos_x)
    
    t = -(np.dot(ray_o, plane_normal) + plane_d) / ( np.dot(V, plane_normal))   
    
    if t > 0.0:  
        return ray_o + t * V
    


