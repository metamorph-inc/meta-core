from __future__ import print_function
import collections

from openmdao.api import IndepVarComp, Component, Problem, Group, FileRef
import numpy as np


class Adder(Component):
    def __init__(self):
        super(Adder, self).__init__()

        self.add_param("a", val=0.0)
        self.add_param("b", val=0.0)
        self.add_output("x", val=0.0)

    def solve_nonlinear(self, params, unknowns, resids):
        unknowns["x"] = params["a"] + params["b"]

def main():
    top = Problem()

    root = top.root = Group()

    root.add('p', Adder())

    top.setup()
    top.run()

if __name__ == "__main__":
    main()
