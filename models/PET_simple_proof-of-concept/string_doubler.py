from __future__ import print_function

from openmdao.api import IndepVarComp, Component, Problem, Group

class Doubler(Component):
    def __init__(self):
        super(Doubler, self).__init__()

        self.add_param('x', val=u'')
        self.add_param('y', val=u'')

        self.add_output('x_doubled', val=u'')
        self.add_output('y_doubled', val=u'')

    def solve_nonlinear(self, params, unknowns, resids):
        unknowns['x_doubled'] = params['x'] * 2
        unknowns['y_doubled'] = params['y'] * 2
