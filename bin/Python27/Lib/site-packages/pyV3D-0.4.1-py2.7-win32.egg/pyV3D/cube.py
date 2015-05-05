
from numpy import array, float32, int32

from pyV3D.sender import WV_Sender

class CubeGeometry(object):
    # cube  (2 x 2 x height)
    #    v6----- v5
    #   /|      /|
    #  v1------v0|
    #  | |     | |
    #  | |v7---|-|v4
    #  |/      |/
    #  v2------v3
    #
    def __init__(self):
        # vertex coords arrays
        self.vertices = array([
               [ 1, 1, 1,  -1, 1, 1,  -1,-1, 1,   1,-1, 1],  # v0-v1-v2-v3 front
               [ 1, 1, 1,   1,-1, 1,   1,-1,-1,   1, 1,-1],  # v0-v3-v4-v5 right
               [ 1, 1, 1,   1, 1,-1,  -1, 1,-1,  -1, 1, 1],  # v0-v5-v6-v1 top
               [-1, 1, 1,  -1, 1,-1,  -1,-1,-1,  -1,-1, 1],  # v1-v6-v7-v2 left
               [-1,-1,-1,   1,-1,-1,   1,-1, 1,  -1,-1, 1],  # v7-v4-v3-v2 bottom
               [ 1,-1,-1,  -1,-1,-1,  -1, 1,-1,   1, 1,-1],  # v4-v7-v6-v5 back
        ], dtype=float32)

        # color array.  one color per face in this case
        self.colors = array([
               [0, 0, 255],    # v0-v1-v2-v3
               [255, 0, 0],    # v0-v3-v4-v5
               [0, 255, 0],    # v0-v5-v6-v1
               [255, 255, 0],  # v1-v6-v7-v2
               [255, 0, 255],  # v7-v4-v3-v2
               [0, 255, 255],  # v4-v7-v6-v5
        ], dtype=float32)

        # index array
        # each face has 2 triangles
        self.triangles = array([0, 1, 2,   0, 2, 3], dtype=int32)

        # bounding box
        self.bbox = [-1.,-1.,-1.,1.,1.,1.]

    def get_visualization_data(self, wv):
        '''Fills the given WV_Wrapper object with data for faces,
        edges, colors, etc.
        
        wv: WV_Wrapper object 
        '''
        for i in range(6):  # 6 faces
            wv.set_face_data(points=self.vertices[i], 
                             tris=self.triangles, 
                             colors=self.colors[i],
                             bbox=self.bbox,
                             name="Face %d"%(i+1))

            wv.set_edge_data(points=self.vertices[i],
                             bbox=self.bbox,
                             name="Edge %d"%(i+1))


class CubeSender(WV_Sender):

    def initialize(self, **kwargs):
        eye    = array([0.0, 0.0, 7.0], dtype=float32)
        center = array([0.0, 0.0, 0.0], dtype=float32)
        up     = array([0.0, 1.0, 0.0], dtype=float32)
        fov   = 30.0
        zNear = 1.0
        zFar  = 10.0

        bias = 0
        self.wv.createContext(bias, fov, zNear, zFar, eye, center, up)

    @staticmethod
    def supports(obj):
        return isinstance(obj, CubeGeometry)

    def geom_from_obj(self, obj):
        if isinstance(obj, CubeGeometry):
            obj.get_visualization_data(self.wv)
