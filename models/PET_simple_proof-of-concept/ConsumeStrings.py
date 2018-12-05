from __future__ import print_function

from openmdao.api import IndepVarComp, Component, Problem, Group, FileRef
from openmdao.core.system import AnalysisError


class ConsumeString(Component):

    def __init__(self):
        super(ConsumeString, self).__init__()

        self.add_param("Str", val=u"example", pass_by_obj=True)
        self.add_param("Arr", val=[], pass_by_obj=True)
        self.add_output("Result", val=True, pass_by_obj=True)

    def solve_nonlinear(self, params, unknowns, resids):
        pass
