from __future__ import print_function

from openmdao.api import IndepVarComp, Component, Problem, Group

class SampleAnalysis(Component):
    icon_path = r"componentref.jpg"
    def __init__(self, config1, config2, opt1=None, opt2=0.0):
        super(SampleAnalysis, self).__init__()

        self.config1 = config1
        self.config2 = config2
        self.opt1 = opt1 or 0.0
        self.opt2 = opt2 or 0.0
        self.add_param('x', val=0.0)

        self.add_output('output', shape=1)

    def solve_nonlinear(self, params, unknowns, resids):
        x = params['x']

        unknowns['output'] = x * (self.config1 + self.opt1)
