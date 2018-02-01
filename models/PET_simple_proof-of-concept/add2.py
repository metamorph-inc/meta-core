from __future__ import print_function

from openmdao.api import IndepVarComp, Component, Problem, Group

class Add2(Component):
    """ Evaluates the equation y = x + 2 """

    def __init__(self):
        super(Add2, self).__init__()

        self.add_param('x', val=0.0)

        self.add_output('y', val=0.0)

    def solve_nonlinear(self, params, unknowns, resids):
        unknowns['y'] = params['x'] + 2

if __name__ == "__main__":

    top = Problem()

    root = top.root = Group()

    root.add('p1', IndepVarComp('x', 3.0))
    root.add('a', Add2())

    root.connect('p1.x', 'a.x')

    top.setup()
    top.run()

    print(top['a.y'])
