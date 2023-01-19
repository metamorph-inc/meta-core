import logging
from math import sqrt

import numpy as np
from calc_r import inter_ray_tris, inter_ray_tris_all, check_side, calc_min_max, calc_tri_normals, calc_mid
LRG_NEG = -1e30
LRG_POS = 1e30


# It seems that copying arrays (deep copy) is markedly faster than instantiating new ones. So, create a few template arrays
#   to be copied later
xyz_template = np.zeros(3)
empty_array_template = np.array([])


padding = 6
total__pad = padding + 4
padded_array_template = np.empty((0, total__pad))

n_b_to_a = xyz_template.copy()  # np.zeros(3)
n_a_to_b = xyz_template.copy()  # np.zeros(3)


class BSP_Tree(object):
    def __init__(self, node_array, tris, allocate_step=100000, min_tris=1200, min_dim=1):
        ## Store the triangles with an enumeration so that even when they are subdivided thier
        ## identity is not lost
        self.orig_tris = np.array([[t[0], t[1], t[2], i] for i, t in enumerate(tris)], 
                                  dtype=np.int32)
        ## Have to pad to be able to use numpy arrays while the shapes are allowed to be polys
        padded = [[t[0], t[1], t[2]] + [-1] * padding + [i] for i, t in enumerate(tris)]                                 
        self.tris = np.array(padded, dtype=np.int32)
        self.allocate_step = allocate_step
        self.min_tris = min_tris
        self.min_dim = min_dim
        self.node_array = node_array # Reference to the full list of nodes
        self._resize()
        self.next_free = len(node_array)
        self.root = BSP_Element(self.tris, self)
        self.normals = None
        self.nor_dir = None
        self._calc_triangle_normals()
        self.split_cache = np.zeros(len(self.tris),dtype=np.int32)
        self.hit_cache = np.zeros(len(self.tris))
        self.mid_cache = np.empty((10000, 3))

    def _calc_triangle_normals(self):
        self.normals = np.zeros((len(self.orig_tris), 3))
        self.nor_dir = np.zeros(len(self.orig_tris), dtype=np.int32)
        calc_tri_normals(self.orig_tris, self.node_array, self.normals, self.nor_dir)            
        logging.info("Calculated triangle normals")    

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
        
    def prepare_add(self, num_add_nodes):
        """
        Make sure that ``num_add_nodes`` can be added later without needing a resize.
        
        Useful if adding nodes from within cython where resizing is tricky.
        """
        if self.next_free + num_add_nodes >= self.array_size:
            self._resize()
        return self.next_free
        
    def generate_tree(self):
        """
        Generate the tree by fitting the triangles into it.
        """
        tree_list = [self.root]
        i = 0
        while i < len(tree_list):
            elem = tree_list[i]
            split = elem.make_split()
            if split:
                logging.debug("Split {} on {} at {}".format(elem, elem.split_dir, elem.split_pos))
                tree_list.extend(split)
            i += 1
            
        ## Finalize tris array to only contain a list referring to the original triangle numbers
        for elem in tree_list:
            if elem.tris is not None:
                elem.set_tris = {t[-1] for t in elem.tris}
                elem.tris = np.array(list(elem.set_tris), dtype=np.int32)
        self.tris = np.array(self.orig_tris)
        logging.info("MADE {} SPLITS".format(i - 1))
        logging.info("HAVE {} NODES".format(self.next_free))
        logging.info("HAVE {} ELEMS".format(len(tree_list)))
      
    def get_line_intersection(self, start_orig, end_orig):
        """
        Return the index of a triangle hit by the 3D line defined by 3D vectors ``start_orig`` and
        ``end_orig``.  Returns -1 if no triangle is hit.
        
        Note that the triangle hit is the first found by the routine.  It is **not** necessarily the
        first that the line would hit.
        """
        tree_list = [(self.root, start_orig, end_orig)]
        ray_direction = end_orig - start_orig
        length = sqrt(np.dot(ray_direction, ray_direction))
        ray_direction = ray_direction / length   
        
        i = 0
        ## Loop over elements starting at the root, new elements are added until a leaf is reached
        while i < len(tree_list):
            elem, start, end = tree_list[i]
            res, stat = elem.get_line_intersection_any(start, end, ray_direction, i)
            
            if stat == BSP_Element.TRIS:
                if res >= 0:
                    return res
            else:
            
                if stat == BSP_Element.ABOVE:
                    tree_list.append((elem.above, start, end))
                elif stat == BSP_Element.BELOW:
                    tree_list.append((elem.below, start, end))
                    
                elif stat == BSP_Element.BOTH_START_ABOVE:
                    tree_list.append((elem.above, start, res))
                    tree_list.append((elem.below, res, end))
                elif stat == BSP_Element.BOTH_START_BELOW:
                    tree_list.append((elem.below, start, res))
                    tree_list.append((elem.above, res, end))
                elif stat == BSP_Element.BOTH_ORIG_RAY:
                    tree_list.append((elem.below, start, end))
                    tree_list.append((elem.above, start, end))

            i += 1  
        return -1
        
    def get_line_intersection_all(self, start_orig, end_orig):
        """
        Return the index of all thes triangle hit by the 3D line defined by 3D vectors ``start_orig`` and
        ``end_orig``.  Returns an empty list if no triangles are hit.
        
        Also returns the distance to each intersection point measured from ``start_orig``.
        
        Note that the triangle hit is the first found by the routine.  It is **not** necessarily the
        first that the line would hit.
        """
        tree_list = [(self.root, start_orig, end_orig)]
        ray_direction = end_orig - start_orig
        length = sqrt(np.dot(ray_direction, ray_direction))
        ray_direction = ray_direction / length   
        
        i = 0
        ## Loop over elements starting at the root, new elements are added until a leaf is reached
        intersections = []
        while i < len(tree_list):
            elem, start, end = tree_list[i]
            res, stat = elem.get_line_intersections(start, end, ray_direction, i)
            
            if stat == BSP_Element.TRIS:
                intersections.extend(res) 
                    
            else:
#                if stat == BSP_Element.ABOVE:
#                    tree_list.append((elem.above, start, end))
#                elif stat == BSP_Element.BELOW:
#                    tree_list.append((elem.below, start, end))
#                    
#                elif stat == BSP_Element.BOTH_START_ABOVE:
#                    tree_list.append((elem.above, start, res))
#                    tree_list.append((elem.below, res, end))
#                elif stat == BSP_Element.BOTH_START_BELOW:
#                    tree_list.append((elem.below, start, res))
#                    tree_list.append((elem.above, res, end))
#                elif stat == BSP_Element.BOTH_ORIG_RAY:
#                    tree_list.append((elem.below, start, end))
#                    tree_list.append((elem.above, start, end))
                tree_list.append((elem.below, start, end))
                tree_list.append((elem.above, start, end))

            i += 1  
        print("tests: {}".format(i))
        return set(intersections)
        
        
    def get_root(self):
        return self.root


class BSP_Element(object):
    """
    An element in a 3D cartesian BSP tree.
    
    """
    MISS = -1
    ABOVE = -2
    BELOW = -3
    BOTH_ORIG_RAY = -4
    BOTH_START_BELOW = -5
    BOTH_START_ABOVE = -6
    TRIS = -7

    def __init__(self, tris, tree):
        
        self.tree = tree
        ## Element always created containing geometry
        self.tris = tris # List of any triangles owned by this BSP
        self.set_tris = None
        ## But can be subdivided and have 2 children (and they then own the divided geometry)
        self.split_dir = None
        self.split_pos = None
        self.above = None # Reference to a child BSP_Element on the positive side 
        self.below = None # Reference to a child BSP_Element on the negative side 
        self.min_ = np.zeros(3)
        self.max_ = np.zeros(3)
 
    def get_line_intersection_any(self, local_start, local_end, ray_direction, i):
        """
        Test for an intersection with the line defined by ``local_start`` and ``local_end``.
        
        If a leaf return True for a hit or False for a miss.
        
        If a branch return, ABOVE if line is completely above dividing plane, BELOW if completely 
        below and a if line intersects return a tuple with numpy array containing the intersection 
        point of the line with the plane plus a boolean indicating whether (start-intersection) is 
        below (True) or above (False) the dividing plane..
        """
        if self.tris is None:
            ## No triangles, so check which sides of the split need to be checked further       
            stat = calc_mid(local_start, 
                           local_end, 
                           ray_direction, 
                           self.split_dir, 
                           self.split_pos, 
                           i, 
                           self.tree.mid_cache)
                           
#            if stat < self.BOTH_ORIG_RAY:
#                return (self.tree.mid_cache[i], stat)
#            else:
#                return (local_start, stat)
            return (local_start, stat)    
        else:
            ## Has triangles - test for an intersection with the line and return True on first hit
            return (inter_ray_tris(local_start, 
                                   ray_direction, 
                                   self.tris, 
                                   self.tree.tris, 
                                   self.tree.node_array, 
                                   self.tree.normals, 
                                   self.tree.nor_dir, 
                                   self.min_, 
                                   self.max_),
                    self.TRIS)

    def get_line_intersections(self, local_start, local_end, ray_direction, i):
        """
        Test for an intersection with the line defined by ``local_start`` and ``local_end``.
        
        If a leaf return True for a hit or False for a miss.
        
        If a branch return, ABOVE if line is completely above dividing plane, BELOW if completely 
        below and a if line intersects return a tuple with numpy array containing the intersection 
        point of the line with the plane plus a boolean indicating whether (start-intersection) is 
        below (True) or above (False) the dividing plane..
        """
        if self.tris is None:
            ## No triangles, so check which sides of the split need to be checked further       
            stat = calc_mid(local_start, 
                           local_end, 
                           ray_direction, 
                           self.split_dir, 
                           self.split_pos, 
                           i, 
                           self.tree.mid_cache)
            if stat < self.BOTH_ORIG_RAY:
                return (self.tree.mid_cache[i], stat)
            else:
                return (local_start, stat)
                
        else:
            ## Has triangles - test for an intersection with the line and return True on first hit
#            print "Intersecting"
            inter_ray_tris_all(local_start, 
                                   ray_direction, 
                                   self.tris, 
                                   self.tree.tris, 
                                   self.tree.node_array, 
                                   self.tree.normals, 
                                   self.tree.nor_dir, 
                                   self.min_, 
                                   self.max_,
                                   self.tree.hit_cache)
            hit_tris = [(t, self.tree.hit_cache[n])  for n, t in enumerate(self.tris) if self.tree.hit_cache[n] >= 0.0]  
#            print "hit_tris: {}".format(hit_tris)                     
            return hit_tris, self.TRIS


    def make_split(self):
        """
        If successful return 2 new BSP_Element objects else return None.
        """

        dim = self.choose_split_dir()
        if len(self.tris) < self.tree.min_tris or dim < self.tree.min_dim:
            return
        below, above, inter = self.split_triangle_lists()
        self.tris = None # Don't need to keep triangles in this BSP_Element any more
        n_above, n_below = self.cut_intersected_tris(inter)

        self.above = BSP_Element(np.concatenate((above, n_above)), self.tree)
        self.below = BSP_Element(np.concatenate((below, n_below)), self.tree)

  
        return self.above, self.below


    def split_at(self, split_dir, split_pos):
        self.split_dir = split_dir
        self.split_pos = split_pos
        below, above, inter = self.split_triangle_lists()
        if len(inter) == 0:
            return (BSP_Element(above.copy(), self.tree),
                    BSP_Element(below.copy(), self.tree))
            
        n_above, n_below = self.cut_intersected_tris(inter)

        if n_above.shape[0] == 0:
            n_above = n_above.reshape((0, total__pad))
        if n_below.shape[0] == 0:
            n_below = n_below.reshape((0, total__pad))

        return (BSP_Element(np.concatenate((above, n_above)), self.tree),
                BSP_Element(np.concatenate((below, n_below)), self.tree))
        

    
    def choose_split_dir(self):
        """
        Choose the cartesian split direction based on the longest dimension.  Sets  
        ``self.split_dir`` and ``self.split_pos``.
        
        Returns the size of the longest dimension for use in determining if element is worth 
        splitting.
        """
        ## Use a Cython call to get the enclosing cuboid
#        x_max, x_min, y_max, y_min, z_max, z_min = calc_min_max(self.tris,
#                                                                self.tree.node_array,
#                                                                self.min_,
#                                                                self.max_                                                                
#                                                                )
        calc_min_max(self.tris,
                                                                self.tree.node_array,
                                                                self.min_,
                                                                self.max_                                                                
                                                                )
        
#        self.min_ = np.array([x_min, y_min, z_min])        
#        self.max_ = np.array([x_max, y_max, z_max])        

        splits = (self.max_ + self.min_) * 0.5
        dims = self.max_ - self.min_
        
        self.split_dir = dims.argmax()
        self.split_pos = splits[self.split_dir]

        return dims[self.split_dir]


    def split_triangle_lists(self):
        """
        Split the triangles into those that are completely below ``split_pos``, completely above and
        those that are intersected.
        """
        if len(self.tris) == 0:
            return (self.tris, self.tris, self.tris)
                    
        s_c = self.tree.split_cache

        ## Modifies value of s_c (split_cache) to mark triangle locations.
        check_side(s_c, self.tris, self.tree.node_array, self.split_dir, self.split_pos)
        
        ## Use the split_cache to mark where the triangles should go
        below = self.tris[s_c[:len(self.tris)] == -1]        
        inter = self.tris[s_c[:len(self.tris)] == 0]        
        above = self.tris[s_c[:len(self.tris)] == 1]        
        return below, above, inter


    def cut_intersected_tris(self, inter_tris):
        """
        Cut the intersected triangles into new triangles.  Make the new nodes required.
        
        Every triangle will be split into 3 new triangles and need 2 new nodes.
        """
        num_inter_tris = len(inter_tris)
        self.tree.prepare_add(2 * num_inter_tris)
        new_tris_above = -np.ones((num_inter_tris, 10), dtype=np.int32)
        new_tris_below = -np.ones((num_inter_tris, 10), dtype=np.int32)
        for i, tri in enumerate(inter_tris):
            # The tris array is padded (up to 9 vertices, plus the original triangle # tacked onto the end).

            # xyz node coordinates
            s = self.tree.node_array[tri[tri[:-1] != -1]]
            num_nodes = len(s)

            # Decides whether the split position for a given node is below or above the split position
            stat = s[:, self.split_dir] < self.split_pos

            a_to_b, b_to_a = -1, -1
            ## Find the edge that goes from below (True) to above (False)
            # At the end gives the node values where that node, but not the next, meets the given criteria.
            for n in xrange(num_nodes):
                if stat[n] and (not stat[(n + 1) % num_nodes]):
                    b_to_a = n

                if (not stat[n]) and stat[(n + 1) % num_nodes]:
                    a_to_b = n

            ## Make the 2 new nodes
            m_b_to_a = (s[b_to_a, self.split_dir] - self.split_pos) / (s[b_to_a, self.split_dir] - s[(b_to_a + 1) % num_nodes, self.split_dir])

            n_b_to_a[:] = (self.tree.node_array[tri[b_to_a]] * (1 - m_b_to_a) + self.tree.node_array[tri[(b_to_a + 1) % num_nodes]] * m_b_to_a)
            
            n_b_to_a[self.split_dir] = self.split_pos
            n1 = self.tree.add_node(n_b_to_a)

            m_a_to_b = (s[a_to_b, self.split_dir] - self.split_pos) / (s[a_to_b, self.split_dir] - s[(a_to_b + 1) % num_nodes, self.split_dir])
            
            n_a_to_b[:] = (self.tree.node_array[tri[a_to_b]] * (1 - m_a_to_b) + self.tree.node_array[tri[(a_to_b + 1) % num_nodes]] * m_a_to_b)
            
            n_a_to_b[self.split_dir] = self.split_pos
            self.tree.add_node(n_a_to_b)

#            make_polys(i, new_tris_above, new_tris_below, tri, a_to_b, b_to_a, n1, num_nodes)
            ## make the 2 new polys
            new_tris_above[i, 9] = tri[-1]
            new_tris_below[i, 9] = tri[-1]
        
            if a_to_b >= b_to_a:
                ## The above poly is the simpler one in this case
                poly_above = [n1] + tri[b_to_a + 1:a_to_b + 1].tolist() + [n1 + 1]
                
                poly_below = ([n1 + 1] + tri[a_to_b + 1:num_nodes].tolist() + tri[:b_to_a + 1].tolist() + [n1])
        
            else:            
                poly_above = ([n1] + tri[b_to_a + 1:num_nodes].tolist() + tri[:a_to_b + 1].tolist() + [n1 + 1])   
                              
                poly_below = [n1 + 1] + tri[a_to_b + 1:b_to_a + 1].tolist() + [n1]
                
            new_tris_above[i, :len(poly_above)] = poly_above    
            new_tris_below[i, :len(poly_below)] = poly_below    

        return   new_tris_above,   new_tris_below       

