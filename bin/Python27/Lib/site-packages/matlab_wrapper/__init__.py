from __future__ import absolute_import
from __future__ import print_function
from openmdao.api import Component
import os
import os.path
import json
import six

import smop
import smop.parse
import smop.backend
import smop.node
from matlab_wrapper.engine import SMOPEngine, import_mfile


class MatlabWrapper(Component):
    def __init__(self, mFile, start_engine=True, var_file=None):
        super(MatlabWrapper, self).__init__()
        # self.var_dict = None
        # self.jsonFile = var_file
        # self.create_json_dict()

        if False:
            for key, value in self.var_dict.items():
                if key == "params":
                    for z in value:
                        # print(repr(z))
                        self.add_param(**z)
                elif key == "unknowns":
                    for z in value:
                        # print z["name"]
                        self.add_output(**z)

        if not os.path.exists(mFile):
            open(mFile)

        self.mFile = os.path.abspath(mFile)

        mfname = os.path.splitext(mFile)[0]
        self.basename = os.path.basename(mfname)

        self.func_list = func_list = import_mfile(mFile)
        fn = [f for f in func_list if type(f) == smop.node.function and f.head.ident.name == self.basename]
        if not fn:
            raise ValueError("Could not find function named '{0}' in file '{1}'".format(self.basename, os.path.basename(self.mFile)))
        fn = fn[0]
        self._input_names = [e.name for e in fn.head.args]
        self._output_names = [e.name for e in fn.head.ret]

        for input in self._input_names:
            self.add_param(input, val=0.0)
        for output in self._output_names:
            self.add_output(output, val=0.0)

        if start_engine:
            from matlab_proxy import get_matlab_engine
            self.eng = get_matlab_engine()
            if self.eng is None:
                self.eng = SMOPEngine()
            self.eng.addpath(os.path.dirname(os.path.abspath(mFile)), nargout=0)

    def __del__(self):
        # TODO
        # if self.eng
        # self.eng.quit()
        pass

    def _coerce_val(self, variable):
        if variable['type'] == 'Bool':
            variable['val'] = variable['val'] == 'True'
        elif variable['type'] == 'Str':
            variable['val'] = six.text_type(variable['val'])
        else:
            variable['val'] = getattr(six.moves.builtins, variable['type'].lower())(variable['val'])

    def create_json_dict(self):
        with open(self.jsonFile) as jsonReader:
            self.var_dict = json.load(jsonReader)
            for vartype in ('params', 'unknowns'):
                for var in self.var_dict.get(vartype, []):
                    self._coerce_val(var)

    def solve_nonlinear(self, params, unknowns, resids):
        args = [params[name] for name in self._input_names]

        def set_unknowns(outputs):
            for i, name in enumerate(self._output_names):
                unknowns[name] = outputs[i]

        out = six.StringIO()
        err = six.StringIO()

        outputs = getattr(self.eng, self.basename)(*args, nargout=len(self._output_names), stdout=out, stderr=err)
        if len(self._output_names) == 1 and not isinstance(self.eng, SMOPEngine):
            # MATLAB returns a single float, not an array
            outputs = [outputs]

        set_unknowns(outputs)


if __name__ == '__main__':
    import argparse

    parser = argparse.ArgumentParser(description='MATLAB OpenMDAO wrapper')
    parser.add_argument('--run', action='store_true')
    parser.add_argument('mfile', nargs=1)

    args = parser.parse_args()

    c = MatlabWrapper(args.mfile[0])
    print((json.dumps({'params': c._init_params_dict, 'unknowns': c._init_unknowns_dict})))

    if args.run:
        from openmdao.api import IndepVarComp, Problem, Group

        top = Problem()

        root = top.root = Group()

        root.add('p1', IndepVarComp('x', 3.0))
        root.add('p2', IndepVarComp('y', -4.0))
        root.add('c', c)

        root.connect('p1.x', 'c.a')
        root.connect('p2.y', 'c.b')
        root.connect('p2.y', 'c.c')

        top.setup()
        top.run()

        print(root.c.unknowns['m'])
