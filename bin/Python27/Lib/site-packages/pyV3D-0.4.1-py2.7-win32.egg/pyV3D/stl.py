import os
import struct

from numpy import array, float32
import numpy as np

from pyV3D.sender import WV_Sender

class STLGeometryObject(object):
    '''This is an object that follows the IStaticGeometry interface.
    '''

    def __init__(self, filename):
        self.filename = filename
        self.geom_name = os.path.basename(filename)[:-4]
        
    def get_visualization_data(self, wv, *args, **kwargs):
        '''Load a tesselation from a geometry model.
        
        wv: WV_Wrapper instance
            The pyV3D WV_Wrapper object
        '''
        
        # Determine if we are binary or ascii
        binary = False
        with open(self.filename, 'rU') as stl:
            for line in stl:
        
                line = line.strip()
                if not line:
                    continue
                    
                fields = line.split()
                if fields[0] not in ('solid'):
                    binary = True
                
                break
                
        if binary:
            with open(self.filename, 'rb') as stl:
                self._load_binary(wv, stl)
        else:
            with open(self.filename, 'rU') as stl:
                self._load_ascii(wv, stl)
            
    def _load_ascii(self, wv, stl):
        '''Load from ascii STL file.'''

        vertices = []
        normals = []
        nsolid = 0
        
        for line in stl:
        
            line = line.strip()
            if not line:
                continue
            fields = line.split()
            
            if fields[0] in ('solid', 'outer',
                             'endloop', 'endfacet'):
                continue
                
            elif fields[0] == 'facet':
            
                # Replicate normal for each vertex.
                normal = [float(xyz) for xyz in fields[2:]]
                normals.extend(normal)
                normals.extend(normal)
                normals.extend(normal)
                
            elif fields[0] == 'vertex':
                vertices.extend([float(xyz) for xyz in fields[1:]])
            
            # Finish with this solid and prepare for next one.
            elif fields[0] == 'endsolid':
                ntri = len(vertices)/3
                    
                nsolid += 1
                wv.set_face_data(points=np.array(vertices, dtype=np.float32),
                                 tris=np.array(range(1, ntri+1), dtype=np.int32),
                                 normals=np.array(normals, dtype=np.float32), 
                                 bbox=self._get_bbox(ntri/3, vertices),
                                 name="%s_solid%d"%(self.geom_name, nsolid))
                                                 
                normals = []
                vertices = []
                    
    def _load_binary(self, wv, stl):
        '''Load from binary STL file.'''
        
        BINARY_HEADER ="80sI"
        BINARY_FACET = "12fH"
        
        vertices = []
        normals = []
        
        header, ntri = struct.unpack(BINARY_HEADER, stl.read(84))

        def remove_non_ascii(s): 
            return "".join([i for i in s if ord(i)<128])

        header = remove_non_ascii(header)
        
        for i in xrange(0, ntri):
            facet = struct.unpack(BINARY_FACET, stl.read(50))

            normal = [float(xyz) for xyz in facet[0:3]]
            normals.extend(normal)
            normals.extend(normal)
            normals.extend(normal)
            
            vertices.extend([float(xyz) for xyz in facet[3:12]])
            
        wv.set_face_data(points=np.array(vertices, dtype=np.float32),
                         tris=np.array(range(1, 3*ntri+1), dtype=np.int32),
                         colors=None,
                         normals=np.array(normals, dtype=np.float32), 
                         bbox=self._get_bbox(ntri, vertices),
                         name=header)

    def _get_bbox(self, ntri, vertices):
        """Determine the bounding box

        ntri: int
            Number of triangles

        vertices: list of float
            coordinates of vertices of the form [x1, y1, z1, .... x_n, y_n, z_n].
            Length of vertices is ntri*9

        """

        min_x = max_x = vertices[0]
        min_y = max_y = vertices[1]
        min_z = max_z = vertices[2]
        for i in range(ntri*3):
            min_x = min(min_x, vertices[i*3])
            max_x = max(max_x, vertices[i*3])
            min_y = min(min_y, vertices[i*3+1])
            max_y = max(max_y, vertices[i*3+1])
            min_z = min(min_y, vertices[i*3+2])
            max_z = max(max_y, vertices[i*3+2])
            
        return [max_x, max_y, max_z, min_x, min_y, min_z]

                                         

class STLSender(WV_Sender):
    def initialize(self, **kwargs):
        eye    = array([0.0, 0.0, 7.0], dtype=float32)
        center = array([0.0, 0.0, 0.0], dtype=float32)
        up     = array([0.0, 1.0, 0.0], dtype=float32)
        fov   = 30.0
        zNear = 1.0
        zFar  = 10.0

        bias  = 1
        self.wv.createContext(bias, fov, zNear, zFar, eye, center, up)

    @staticmethod
    def supports(obj):
        if isinstance(obj, STLGeometryObject):
            return True
        return isinstance(obj, basestring) and obj.lower().endswith('.stl')

    def geom_from_file(self, fname):
        geom = STLGeometryObject(fname)
        geom.get_visualization_data(self.wv, 
                                    angle=15., relSide=.02, relSag=.001)

    def geom_from_obj(self, obj):
        obj.get_visualization_data(self.wv, 
                                   angle=15., relSide=.02, relSag=.001)
