from __future__ import print_function

from openmdao.api import Component

class TestComp(Component):
    """ """

    def __init__(self):
        super(TestComp, self).__init__()

        self.add_param('a', val=0.0)
        self.add_param('b', val="abc")
        self.add_param('c', val=u'abc')
        self.add_param('d', val=0.0)
        self.add_param('e', val="abc")
        self.add_param('f', val=u'abc')


    def solve_nonlinear(self, params, unknowns, resids):
        """  """

        print(params['a'])
        print(params['b'])
        print(params['c'])
        print(params['d'])
        print(params['e'])
        print(params['f'])
