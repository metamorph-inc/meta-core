""" OpenMDAO component for playing files."""

from __future__ import print_function
import math
from openmdao.api import IndepVarComp, Component, Problem, Group, FileRef

import random
import string
from shutil import copyfile
import os
import glob

class file_player(Component):

    def __init__(self):
        super(file_player, self).__init__()
        self.add_param('Number', val=0.0)
        self.add_output('File', FileRef('file.txt'), binary=True, pass_by_obj=True)

    def solve_nonlinear(self, params, unknowns, resids):
        with open('file.txt', 'w') as fout:
            fout.write("{}\n".format(repr(params['Number'])))

if __name__ == "__main__":

    top = Problem()
    root = top.root = Group()
    root.add('IndepVar', IndepVarComp('x', 1.0))
    root.add('FilePlayer', file_player())
    root.connect('IndepVar.x', 'FilePlayer.Number')
    top.setup()
    top.run()
    print(top['FilePlayer.File'])
