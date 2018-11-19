from __future__ import absolute_import
import os.path
from openmdao.api import Component, FileRef


class FileProducer(Component):
    def __init__(self, path='', *args, **kwargs):
        super(FileProducer, self).__init__(*args, **kwargs)

        self.add_param('unused', val=0.0)

        self.add_output('outfile', FileRef(os.path.join(path, 'bin.out')), binary=True)

    def solve_nonlinear(self, params, unknowns, resids):
        outfile_ref = unknowns['outfile']

        with outfile_ref.open('wb') as f:
            f.write('sample data'.encode('utf8'))
