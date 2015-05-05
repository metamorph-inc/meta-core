""" 
Read and write geometry files.  Handles .stl(ascii & binary), .vtk, .iv and a custom binary format 
serialized as compressed numpy arrays (.npz). 
"""
import struct
import os
import functools
import collections
import contextlib

import numpy as np

from load_inventor import read_iv_file

def open_geom(filename, **kwargs):
    """
    Return geometry information contained in ``filename``.  File type is determined from extension 
    and can be any of ``[".stl", ".iv", ".npz"]``.
    
    At a minimum the returned dictionary will have keys ``"x", "y", "z" and "tris"`` representing 
    the nodes and triangles contained``filename``.  If the file type encodes other information there
    may be additional keys.
    """
    ext = os.path.splitext(filename)[-1]
    jump_table = {".stl" : open_stl,
                  ".iv" : read_iv_file,
                  ".npz" : open_npz}
    
    
              
    return jump_table[ext](filename, **kwargs)
    
def open_stl(filename):
    """
    Return a dictionary of numpy arrays with keys ``"x", "y", "z" and "tris"`` representing the 
    nodes and triangles contained in .stl file ``filename``.
   
    Tries to open as a binary stl first and then ascii if that fails.
    
    Note that there is no connectivity between triangles in an stl file.  Every triangle has its 
    own 3 unique nodes.  
    """
    try:
        ## Try as a binary file first
        num_triangles, nodes = _open_binary_stl(filename)
    except ValueError:
        ## Maybe it is in ascii format
        num_triangles, nodes = _open_ascii_stl(filename)
    
    tri_conn = np.arange(num_triangles * 3, dtype=np.int32).reshape((num_triangles, 3))

    return {"x" : nodes[:, 0], "y" : nodes[:, 1], "z" : nodes[:, 2], "tris" : tri_conn}    
    
    
def _open_binary_stl(filename):
    """
    Assuming binary format, return number of triangles and node positions from file ``filename``.
    """    
    with open(filename, "rb") as stl_file:   
        ## Skip the 80 byte header
        stl_file.read(80)
        
        ## Find out how many triangles to expect
        num_tris = struct.unpack("i", stl_file.read(4))[0]   
        
        ## Check expected binary file size based on triangle count against actual file length
        if 84 + num_tris * 50 != os.stat(filename).st_size:
            raise ValueError("STL binary read failed.  File is either ascii format or corrupt")
            
        ## Each tri is 12 floats [nx, ny, nz, x0, y0, z0, x2, y2, z1, x2, y2, z2] + 2 bytes packing
        data = np.fromfile(stl_file, dtype=np.dtype("(12)f4, a2"), count=num_tris)
        
    ## Discard the tri normal and reshape to group into triples
    return num_tris, data["f0"][:, 3:].reshape((num_tris * 3, 3))


def _open_ascii_stl(filename):
    """
    Assuming ascii format, return number of triangles and node positions from file ``filename``.
    """
    ## Make a shortened sig function for use in the comprehension below
    get_nodes = functools.partial(np.fromstring, dtype=np.float32, sep=" ")
    
    with open(filename, "r") as stl_file:
        ## load lines like "vertex 2.375058e+03 1.860550e+03 4.254500e+03"
        nodes = [get_nodes(line.split("x")[-1]) for line in stl_file if "vertex" in line]
   
    ## Number of triangles is always 1/3rd the number of nodes for an stl file
    return len(nodes) / 3, np.array(nodes)


def open_npz(filename, extras="all"):
    """
    Return a dictionary of numpy arrays with keys ``"x", "y", "z" and "tris"`` representing the 
    nodes and triangles contained in .npz file ``filename``.
   
    If other arrays were serialized to the npz file they will be returned only if asked for by 
    including their name in the iterable ``extras``.  If ``extras == "all"`` (default) then 
    everything in the file is returned
    """
    with contextlib.closing(np.load(filename)) as npz_f:
        if extras == "all":
            return {k : npz_f[k] for k in npz_f}
        elif isinstance(extras, collections.Iterable):
            ## Silently remove anything asked for that is not available
            return {k : npz_f[k] for k in ["x", "y", "z", "tris"] + extras if k in npz_f}    
        else:
            return {k : npz_f[k] for k in ["x", "y", "z", "tris"]}  

    
def save_binary_stl(filename, surf):
    """
    Save ``surf`` as a binary stl file to ``filename``.
    """        
    x, y, z, tris = [surf[k] for k in ["x", "y", "z", "tris"]]
    
    with open(filename, "wb") as stl_out:
        ## Write out an 80 byte header
        stl_out.write(struct.pack("x" * 80))
        
        ## Number of triangles
        stl_out.write(struct.pack("i", len(tris)))
        
        ## Each triangle has a normal (not important -> written as zeros) and 3 nodes v0, v1, v2
        for v0, v1, v2 in tris:
            stl_out.write(struct.pack("12fxx", 0.0, 0.0, 0.0, 
                                               x[v0], y[v0], z[v0], 
                                               x[v1], y[v1], z[v1], 
                                               x[v2], y[v2], z[v2]))
  
    
if __name__ == "__main__":
    srf = open_geom(r"C:\Users\Robert\Downloads\h170_cat_c9_600hp.iv")
    save_binary_stl(r"C:\Users\Robert\Downloads\h170_cat_c9_600hp.stl", srf)
    