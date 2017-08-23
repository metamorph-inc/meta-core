'''
# Name: buffer_float.py
# Company: MetaMorph, Inc.
# Author(s): Joseph Coombe
# Email: jcoombe@metamorphsoftware.com
# Create Date: 8/7/2017
# Edit Date: 8/7/2017

# Inputs:
in  - float input

# Outputs:
out - float output
'''

from __future__ import print_function

from openmdao.api import Component

class buffer(Component):
    def __init__(self):
        super(buffer, self).__init__()
        self.add_param('input', val=0.0)
        
        self.add_output('output', val=0.0)
    
    def solve_nonlinear(self, params, unknowns, resids):
        unknowns['output'] = params['input']