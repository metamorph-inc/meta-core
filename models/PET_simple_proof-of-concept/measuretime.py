from __future__ import print_function

from openmdao.api import IndepVarComp, Component, Problem, Group
import random
import time

class Measuretime(Component):
    """ Calculates the elapsed time since the time in time.txt. """

    def __init__(self):
        super(Measuretime, self).__init__()

        self.add_param('finished', val=0.0)
        self.add_output('time', val=0.0)

    def solve_nonlinear(self, params, unknowns, resids):
        
        try:
            with open('time.txt', 'r') as f_in:
                unknowns['time'] = time.time()-float(f_in.readline())
        except IOError:
            unknowns['time'] = -1.0
            
    
if __name__ == "__main__":

    top = Problem()

    root = top.root = Group()
    
    root.add('ivc', IndepVarComp('x', 0.0))
    root.add('mt', Measuretime())

    root.connect('ivc.x', 'mt.finished')

    top.setup()
    top.run()

    print(top['mt.time'])
    