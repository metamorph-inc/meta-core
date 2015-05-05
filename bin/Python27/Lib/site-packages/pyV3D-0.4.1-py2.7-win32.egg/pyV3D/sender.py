

class Sender(object):
    def send(self, obj, first=False):
        raise NotImplementedError('send')


class WV_Sender(Sender):
    def __init__(self, wv, **kwargs):
        self.wv = wv
        self.initialize(**kwargs)

    def initialize(self, **kwargs):
        pass

    def send(self, obj, first=False):
        if not first:
            self.wv.clear()  # clear out old GPrim data

        if isinstance(obj, basestring): # assume it's a filename
            self.geom_from_file(obj)
        else:
            self.geom_from_obj(obj)
        self.wv.send(first)

    def on_close(self):
        self.wv = None

    def geom_from_file(self, fname):
        raise NotImplementedError("geom_from_file")
    
    def geom_from_obj(self, obj):
        raise NotImplementedError("geom_from_obj")

