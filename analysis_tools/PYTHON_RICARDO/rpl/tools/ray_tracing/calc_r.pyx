# cython: profile=False
# cython: boundscheck=False
# cython: wraparound=False
import cython
cimport cython

import numpy as np
cimport numpy as np

cdef extern from "math.h":
    double sqrt(double x)

DTYPE = np.float64
ctypedef np.float64_t DTYPE_t

ctypedef np.int32_t ITYPE_t

cdef inline bint point_in_out_triangle(double ray_p,
                                       double* ray0,
                                       double* ray_dir,
                                       int n_dir,
                                       ITYPE_t* tri,
                                       double* node_array) except? -1:
    """
    Check if the point where the ray starting at ``ray0`` pointing in ``ray_dir`` intersects the
    plane of triangle ``tri`` is within the triangle boundary.
    """
    cdef bint in_out = False
    cdef unsigned int i, j, x_dir, y_dir
    cdef double xp_i, yp_i, xp_j, yp_j, x, y

    ## Pick which 2D projection to use based on the most significant normal component
    x_dir = (n_dir + 1) % 3
    y_dir = (n_dir + 2) % 3

    ## Calculate the 2D intersection position
    x = ray_dir[x_dir] * ray_p + ray0[x_dir]
    y = ray_dir[y_dir] * ray_p + ray0[y_dir]

    ## Check if point is actually inside the triangle or not
    for i in range(3):
        j = (i - 1) % 3
        xp_i = node_array[tri[<unsigned int> i] * 3 + x_dir]
        yp_i = node_array[tri[<unsigned int> i] * 3 + y_dir]
        xp_j = node_array[tri[<unsigned int> j] * 3 + x_dir]
        yp_j = node_array[tri[<unsigned int> j] * 3 + y_dir]
        if (((yp_i<=y) and (y<yp_j)) or ((yp_j<=y) and (y<yp_i))) and \
           (x < (xp_j - xp_i) * (y -yp_i) / (yp_j - yp_i) + xp_i):
            in_out = not in_out
  
    return in_out


cdef bint inter_ray_tri(double* ray0,
                        double* ray_dir,
                        ITYPE_t* tri,
                        double* node_array,
                        double* normals,
                        unsigned int n_dir) except? -1:
    """
    Check if intersection between ray and triangle occurs.
    """
    cdef double ray_p
    cdef double a, b
    cdef unsigned int tp = tri[0] * 3 # position of first node in node_array
    cdef unsigned int tn = tri[3] * 3 # position of triangle normal in normals
    ## Scalar product of ray direction with triangle normal
    b = normals[tn] * ray_dir[0] + normals[tn + 1] * ray_dir[1] + normals[tn + 2] * ray_dir[2]
    if abs(b) < 1e-7:
        ## ray is parallel to triangle plane
        return False

    ## Where ray start is along triangle normal
    a = -(normals[tn] * (ray0[0] - node_array[tp]) +
          normals[tn + 1] * (ray0[1] - node_array[tp + 1]) +
          normals[tn + 2] * (ray0[2] - node_array[tp + 2]))

    ## Calculate position along ray where intersection occurs
    ray_p = a / b

    if ray_p < 0.0:
        ## Intersection is behind the start point
        return False
        
    return point_in_out_triangle(ray_p, ray0, ray_dir, n_dir, tri, node_array)


cdef double inter_ray_tri_dist(double* ray0,
                               double* ray_dir,
                               ITYPE_t* tri,
                               double* node_array,
                               double* normals,
                               unsigned int n_dir) except? -1:
    """
    Check if intersection between ray and triangle occurs.
    """
    cdef double ray_p
    cdef double a, b
    cdef unsigned int tp = tri[0] * 3 # position of first node in node_array
    cdef unsigned int tn = tri[3] * 3 # position of triangle normal in normals
    cdef bint check

    ## Scalar product of ray direction with triangle normal
    b = normals[tn] * ray_dir[0] + normals[tn + 1] * ray_dir[1] + normals[tn + 2] * ray_dir[2]
    if abs(b) < 1e-7:
        ## ray is parallel to triangle plane
        return -1.0

    ## Where ray start is along triangle normal
    a = -(normals[tn] * (ray0[0] - node_array[tp]) +
          normals[tn + 1] * (ray0[1] - node_array[tp + 1]) +
          normals[tn + 2] * (ray0[2] - node_array[tp + 2]))

    ## Calculate position along ray where intersection occurs
    ray_p = a / b
    if ray_p < 0.0:
        ## Intersection is behind the start point
        return -1.0

    
    check = point_in_out_triangle(ray_p, ray0, ray_dir, n_dir, tri, node_array)
    if check:
        return ray_p
    else:
        return -1.0


def inter_ray_tris(np.ndarray[DTYPE_t, ndim=1] ray0,
                   np.ndarray[DTYPE_t, ndim=1] ray_dir,
                   np.ndarray[ITYPE_t, ndim=1] tris,
                   np.ndarray[ITYPE_t, ndim=2] tree_tris,
                   np.ndarray[DTYPE_t, ndim=2] node_array,
                   np.ndarray[DTYPE_t, ndim=2] normals,
                   np.ndarray[ITYPE_t, ndim=1] nor_dir,
                   np.ndarray[DTYPE_t, ndim=1] min_,
                   np.ndarray[DTYPE_t, ndim=1] max_):
    """
    Check ray starting at ``ray0`` pointing in direction ``ray_dir`` against the triangles
    referenced by ``tris``.

    Return the triangle number if hit otherwise -1.

    ``tree_tris`` contains references to the nodes used in each triangle.
    ``node_array`` contains the x, y, z positions of every node.
    ``normals`` contains the n_x, n_y, n_z unit normal vector for every triangle
    ``nor_dir`` contains the direction of the most significant normal component.
    ``min_`` contains the minimum x, y, z of all the triangles in ``tris``.
    ``max_`` contains the maximum x, y, z of all the triangles in ``tris``.
    """
    cdef unsigned int i, tri
    cdef unsigned int num_tri = len(tris)
    ## If the ray starts outside of and points away from the bounding box it can't hit any triangles
    for i in range(3):
        if (ray0[i] < min_[i] and ray_dir[i] < -1e-6) or (ray0[i] > max_[i] and ray_dir[i] > 1e-6):
            return -1

    ## Check each triangle and return immediately if any are intersected
    for i in range(num_tri):
        tri = tris[i]
        if inter_ray_tri(&ray0[0], &ray_dir[0], &tree_tris[tri, 0],
                         &node_array[0, 0], &normals[0, 0], nor_dir[tri]):
            return tri

    ## No triangles hit to get here
    return -1


def inter_ray_tris_all(np.ndarray[DTYPE_t, ndim=1] ray0,
                       np.ndarray[DTYPE_t, ndim=1] ray_dir,
                       np.ndarray[ITYPE_t, ndim=1] tris,
                       np.ndarray[ITYPE_t, ndim=2] tree_tris,
                       np.ndarray[DTYPE_t, ndim=2] node_array,
                       np.ndarray[DTYPE_t, ndim=2] normals,
                       np.ndarray[ITYPE_t, ndim=1] nor_dir,
                       np.ndarray[DTYPE_t, ndim=1] min_,
                       np.ndarray[DTYPE_t, ndim=1] max_,
                       np.ndarray[DTYPE_t, ndim=1] hit_cache):
    """
    Check ray starting at ``ray0`` pointing in direction ``ray_dir`` against the triangles
    referenced by ``tris``.

    Returns the distance from ray0 to where each triangle in ``tris`` is hit.  -1 is returned if the
    triangle is not hit by the ray.
    
    triangle number if hit otherwise -1.

    ``tree_tris`` contains references to the nodes used in each triangle.
    ``node_array`` contains the x, y, z positions of every node.
    ``normals`` contains the n_x, n_y, n_z unit normal vector for every triangle
    ``nor_dir`` contains the direction of the most significant normal component.
    ``min_`` contains the minimum x, y, z of all the triangles in ``tris``.
    ``max_`` contains the maximum x, y, z of all the triangles in ``tris``.
    """
    cdef unsigned int i, tri
    cdef unsigned int num_tri = len(tris)
    ## If the ray starts outside of and points away from the bounding box it can't hit any triangles
    for i in range(3):
        if (ray0[i] < min_[i] and ray_dir[i] < -1e-6) or (ray0[i] > max_[i] and ray_dir[i] > 1e-6):
            for j in range(num_tri):
                hit_cache[j] = -1
            return

    ## Check each triangle and return distance to all intersections (or -1 if not intersected)
    for i in range(num_tri):
        tri = tris[i]
        hit_cache[i] = inter_ray_tri_dist(&ray0[0], 
                                            &ray_dir[0], 
                                            &tree_tris[tri, 0],
                                            &node_array[0, 0], 
                                            &normals[0, 0], 
                                            nor_dir[tri])


def check_side(np.ndarray[ITYPE_t, ndim=1] split_cache,
               np.ndarray[ITYPE_t, ndim=2] polys,
               np.ndarray[DTYPE_t, ndim=2] node_array,
               unsigned int split_dir,
               double split_pos):
    """
    Determine for every polygon in ``polys`` which side of ``split_pos`` in direction ``split_dir``
    it is on.

    Return results in ``split_cache[:len(polys)]`` as 1 for completely below, -1 for above or 0 for
    straddling.
    """
    cdef unsigned int i
    cdef unsigned int num_poly = len(polys)
    cdef int check
    for i in range(num_poly):
        split_cache[i] = check_nodes(&polys[i, 0], split_dir, split_pos, &node_array[0, 0])


cdef inline int check_nodes(ITYPE_t* poly_nodes,
                            unsigned int split_dir,
                            double split_pos,
                            double* node_array) except? -1:
    """
    Return 1 if all the nodes of ``poly_nodes`` are below ``split_pos`` in direction ``split_dir``,
    -1 if all above or 0 for a mixture.
    """
    cdef unsigned int max_node = 0
    cdef unsigned int i
    cdef unsigned int node_num
    cdef double node_pos
    ## Is first node below or above
    cdef int b_i_a = -1 if node_array[poly_nodes[0] * 3 + split_dir]  < split_pos else 1
    ## Find the maximum number of a valid node (i.e. not -1)
    while(max_node < 10 and poly_nodes[max_node] >= 0):
        max_node += 1
    ## Find if all nodes are on the same side, return 0 if they aren't
    for i in range(1, max_node):
        node_num = poly_nodes[i] * 3
        node_pos = node_array[node_num + split_dir]
        if (b_i_a == -1 and node_pos > split_pos) or (b_i_a == 1 and node_pos < split_pos):
            return 0
    ## Return 1 if all below or -1 if all above
    return b_i_a


def calc_min_max(np.ndarray[ITYPE_t, ndim=2] poly_nodes, 
                 np.ndarray[DTYPE_t, ndim=2] node_array,
                 np.ndarray[DTYPE_t, ndim=1] out_min,
                 np.ndarray[DTYPE_t, ndim=1] out_max,
                ):
    """
    Calculate the bounding box containing the ``poly_nodes`` polygons which are defined using nodes
    in ``node_array``.

    Returns a tuple containing (min_x, max_x, min_y, max_y, min_z, max_z)
    """
    cdef unsigned int i
    cdef unsigned int num_poly = len(poly_nodes)
    cdef double[6] min_max
    for i in range(6):
        min_max[i] = -1e30 if i % 2 == 0 else 1e30
        
    ## Delegate to the cdef function as it is faster than using Python buffer access
    c_calc_min_max(&poly_nodes[0, 0], num_poly, &node_array[0, 0], &min_max[0])
    
    for i in range(3):
        out_max[i] = min_max[i * 2]
        out_min[i] = min_max[i * 2 + 1]


cdef inline int c_calc_min_max(ITYPE_t* poly_nodes,
                               unsigned int num_poly,
                               double* node_array,
                               double* min_max) except? -1:
    """
    Calculate ``min_max`` the bounding box containing the ``num_poly`` polygons in ``poly_nodes``.

    ``node_array`` contains the node x, y, z positions used by the polygons.
    """
    cdef unsigned int i, j, poly_i
    cdef double x, y, z
    cdef unsigned int node_num
    for i in range(num_poly):
        poly_i = i * 10
        j = 0
        while(poly_nodes[poly_i + j] >= 0):
            node_num = poly_nodes[poly_i + j] * 3
            x = node_array[node_num]
            y = node_array[node_num + 1]
            z = node_array[node_num + 2]
            min_max[0] = min_max[0] if min_max[0] >= x else x
            min_max[1] = min_max[1] if min_max[1] <= x else x
            min_max[2] = min_max[2] if min_max[2] >= y else y
            min_max[3] = min_max[3] if min_max[3] <= y else y
            min_max[4] = min_max[4] if min_max[4] >= z else z
            min_max[5] = min_max[5] if min_max[5] <= z else z
            j = j + 1
    return 0


def calc_tri_normals(np.ndarray[ITYPE_t, ndim=2] tris,
                     np.ndarray[DTYPE_t, ndim=2] node_array,
                     np.ndarray[DTYPE_t, ndim=2] normals,
                     np.ndarray[ITYPE_t, ndim=1] nor_dir):
    """
    Calculate ``normals`` unit normal vectors and ``nor_dir`` the most significant component of the
    normal vector for all triangles in ``tris`` with nodes in ``node_array``.
    """
    ## Delegate to the cdef function as it is faster than using Python buffer access
    cdef unsigned int num_tri = len(tris)
    c_calc_tri_normals(&tris[0, 0], num_tri, &node_array[0, 0], &normals[0, 0], &nor_dir[0])


cdef int c_calc_tri_normals(ITYPE_t* tri_nodes,
                            unsigned int num_tri,
                            double* node_array,
                            double* normals,
                            ITYPE_t* nor_dir) except? -1:
    """
    Calculate ``normals`` unit normal vectors and ``nor_dir`` the most significant component of the
    normal vector for all triangles in ``tris`` with nodes in ``node_array``.
    """
    cdef unsigned int i, j, nor_i, tri_i
    cdef double length
    cdef double[3] a, b, p0, p1, p2, n
    for i in range(num_tri):
        nor_i = i * 3
        tri_i = i * 4
        ## Get the node positions and then the vectors of two triangle edges
        for j in range(3):
            p0[j] = node_array[tri_nodes[tri_i] * 3 + j]
            p1[j] = node_array[tri_nodes[tri_i + 1] * 3 + j]
            p2[j] = node_array[tri_nodes[tri_i + 2] * 3 + j]
            a[j] = p1[j] - p0[j]
            b[j] = p2[j] - p1[j]

        ## Use cross product to get normal vector
        normals[nor_i] = a[1] * b[2] - a[2] * b[1]
        normals[nor_i + 1] = a[2] * b[0] - a[0] * b[2]
        normals[nor_i + 2] = a[0] * b[1] - a[1] * b[0]

        ## Normalize to get the unit normal vector
        length = sqrt(normals[nor_i] * normals[nor_i] +
                      normals[nor_i + 1] * normals[nor_i + 1] +
                      normals[nor_i + 2] * normals[nor_i + 2])
                      
                      
        if length < 1e-10:
#            print "BAD_TRI", i
            length = 1e10

        normals[nor_i] = normals[nor_i] / length
        normals[nor_i + 1] = normals[nor_i + 1] / length
        normals[nor_i + 2] = normals[nor_i + 2] / length

        ## Pre-calculate and store which is the most significant component of the normal
        if abs(normals[nor_i]) > abs(normals[nor_i + 1]):
            nor_dir[i] = 0 if abs(normals[nor_i]) > abs(normals[nor_i + 2]) else 2
        else:
            nor_dir[i] = 1 if abs(normals[nor_i + 1]) > abs(normals[nor_i + 2]) else 2
    return 0


def calc_mid(np.ndarray[DTYPE_t, ndim=1] start,
             np.ndarray[DTYPE_t, ndim=1] end,
             np.ndarray[DTYPE_t, ndim=1] ray_dir,
             unsigned int split_dir,
             double split_pos,
             unsigned int mid_index,
             np.ndarray[DTYPE_t, ndim=2] mids):
    """
    Calculate the mid point and place it in mids and return an integer representing the split type.
    """
    return c_calc_mid(&start[0], &end[0], &ray_dir[0], split_dir, split_pos, mid_index, &mids[0, 0])

cdef inline int c_calc_mid(double* start,
                   double* end,
                   double* ray_dir,
                   unsigned int split_dir,
                   double split_pos,
                   unsigned int mid_index,
                   double* mids) except? -1:
    cdef unsigned int i
    cdef double m
    if start[split_dir] < split_pos and ray_dir[split_dir] < 0:
        return -3
    if end[split_dir] > split_pos and ray_dir[split_dir] > 0:
        return -2
    if end[split_dir] == start[split_dir]:
        return -4
        
    m = (split_pos - start[split_dir]) / (end[split_dir] - start[split_dir])
    m = max(min(m, 1.0), 0.0)  
    for i in range(3):
        mids[mid_index * 3 + i] = start[i] * (1 - m) + end[i] * m
    mids[mid_index * 3 + split_dir] = split_pos            
      
    if ray_dir[split_dir] > 1e-6:
        return -5
    if ray_dir[split_dir] < -1e-6:
        return -6
    return -4          
                       