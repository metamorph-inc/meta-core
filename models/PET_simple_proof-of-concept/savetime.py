from __future__ import print_function

from openmdao.api import IndepVarComp, Component, Problem, Group
import random
import time

class Savetime(Component):
    """ Saves the current time to time.txt """

    def __init__(self):
        super(Savetime, self).__init__()

        self.add_param('pass_in', val=0.0)
        self.add_output('pass_out', val=0.0)

    def solve_nonlinear(self, params, unknowns, resids):
        
        unknowns['pass_out'] = params['pass_in']
        
        with open('time.txt', 'w') as f_out:
            f_out.write("{:.3f}\n".format(time.time()))


if __name__ == "__main__":

    top = Problem()

    root = top.root = Group()
    
    number = random.random()

    root.add('ivc', IndepVarComp('x', number))
    root.add('st', Savetime())

    root.connect('ivc.x', 'st.pass_in')

    top.setup()
    top.run()

    print(top['st.pass_out'] == number)
    
    time.sleep(1)
    
    with open('time.txt', 'r') as f_in:
        print("{:.3f}".format(time.time()-float(f_in.readline())))
    