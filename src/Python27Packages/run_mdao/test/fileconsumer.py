import os.path
import hashlib
from openmdao.api import Component, FileRef


class FileConsumer(Component):
    def __init__(self, path='', *args, **kwargs):
        super(FileConsumer, self).__init__(*args, **kwargs)

        self.add_param('infile', FileRef(os.path.join(path, 'differently_named_bin.dat')), binary=True)

        self.add_output('checksum', val=0)

    def solve_nonlinear(self, params, unknowns, resids):
        infile_ref = params['infile']

        with infile_ref.open('r') as f:
            unknowns['checksum'] = int(hashlib.md5(f.read()).hexdigest(), 16)
