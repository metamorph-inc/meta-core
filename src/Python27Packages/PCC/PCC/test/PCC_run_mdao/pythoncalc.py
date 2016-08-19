from openmdao.api import Component


class PythonCalc(Component):
    def __init__(self, *args, **kwargs):
        super(PythonCalc, self).__init__(*args, **kwargs)

        self.add_param('f_const', val=0.0)
        self.add_param('mass_start_acc', val=0.0)

        self.add_output('MaximumSpeed', val=0.0)
        self.add_output('MaximumSpeed10', val=0.0)

    def solve_nonlinear(self, params, unknowns, resids):
        unknowns['MaximumSpeed'] = (params['f_const'] + 1) * (params['mass_start_acc'] + 1)
        unknowns['MaximumSpeed10'] = unknowns['MaximumSpeed'] * 10
