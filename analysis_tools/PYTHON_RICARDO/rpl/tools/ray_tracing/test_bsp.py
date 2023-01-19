import numpy as np
from math import sqrt
import time
from rpl.tools.geometry.surface_io import open_geom

from bsp_tree_poly import BSP_Tree
import logging
## Get rid of any existing handlers (e.g. if running interactively in ipython)
logging.getLogger().handlers = []

## Main logging goes to a file
logging.basicConfig(filename="test_bench.log",
                    filemode="w",
                    format="%(levelname)s %(asctime)s %(message)s", 
                    level=logging.DEBUG)
                    
## Also log INFO and above to the console
console = logging.StreamHandler()
console.setLevel(logging.INFO)
console.setFormatter(logging.Formatter("%(asctime)s %(message)s"))
logging.getLogger("").addHandler(console) 

start_load = time.time()
surf = open_geom("h170_engine_comp_ign_diesel_prt.stl")
x = surf["x"] / 1000.0
y = surf["y"] / 1000.0
z = surf["z"] / 1000.0



print("X", np.min(x), np.max(x))
print("Y", np.min(y), np.max(y))
print("Z", np.min(z), np.max(z))
nodes = np.vstack((x, y, z)).T

b_tree = BSP_Tree(nodes, surf["tris"], min_tris=200, min_dim=0.5)
b_tree.generate_tree()

start_pt = np.array([-0.25, 0.5, -1.5])
end_pt = np.array([-0.05, 0.5, 0.5])
vector = (end_pt - start_pt)

print(vector)

unit_vector = vector / sqrt(np.dot(vector, vector))

print(unit_vector)

i_points = b_tree.get_line_intersection_all(start_pt, end_pt)

print(i_points)

print(b_tree.get_line_intersection(start_pt, end_pt))

points = []
points.append(start_pt)
points.append(end_pt)

for i_p in i_points:
    d = i_p[1]
    points.append(start_pt + unit_vector * d)


points = np.array(points)



print(points)

#view_model(surf)

from mayavi import mlab

tris = surf["tris"]

scene = mlab.figure(size=(800, 800))

scene.scene.background = (0.0, 0.0, 0.0)
    

s = mlab.triangular_mesh(x, y, z, tris, color=(1, 1, 1), representation="wireframe", line_width=1.5, opacity=0.1)
mlab.axes(s)



q = mlab.points3d(points[:, 0],
             points[:, 1],
             points[:, 2],
             color=(1.0, 0.0, 0.0),
             scale_mode="none", scale_factor=0.01,
             mode='cube', opacity=1.0)


    
scene.scene.disable_render = False

mlab.show()