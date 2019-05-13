from __future__ import absolute_import
from __future__ import print_function
from openmdao.api import Component, AnalysisError
import os
import os.path
import json
import re
import six
import difflib

import smop
import smop.parse
import smop.backend
import smop.node
import numpy
from matlab_wrapper.engine import SMOPEngine, import_mfile


class MatlabWrapper(Component):
    def __init__(self, mFile, start_engine=True, var_file=None):
        super(MatlabWrapper, self).__init__()

        if not os.path.exists(mFile):
            open(mFile)

        self.mFile = os.path.abspath(mFile)

        mfname = os.path.splitext(mFile)[0]
        self.basename = os.path.basename(mfname)

        self.func_list = func_list = import_mfile(mFile)
        fn = [f for f in func_list if type(f) == smop.node.function and f.head.ident.name == self.basename]
        if fn:
            self.bare = False
            fn = fn[0]
            self._input_names = [e.name for e in fn.head.args]
            self._output_names = [e.name for e in fn.head.ret]
            for input in self._input_names:
                self.add_param(input, val=0.0)
            for output in self._output_names:
                self.add_output(output, val=0.0)
        else:
            self.bare = True
            self._input_names = []
            self._output_names = []
            for line in func_list.toplevel_comments or ():
                match = re.search(r'\s*variable\s*:\s*(\S+)\s+(\S+)\s+(\S+)(?:\s+([^=]+)="([^"]*)")*', line)
                if match:
                    name, type_, inout = match.groups()[0:3]
                    args = dict(zip(match.groups()[3::2], match.groups()[4::2]))

                    def map_type(type_, name):
                        ret = {'object': dict,
                               'double': float,
                               'double[]': lambda: numpy.zeros(shape=(1, 1)),
                               'string': six.text_type,
                               'string[]': list,
                               'bool': bool}.get(type_)
                        if ret is None:
                            raise ValueError('Unknown type \'{}\' for variable \'{}\''.format(type_, name))
                        return ret
                    pass_by_obj = True
                    if type_ == 'double':
                        pass_by_obj = False
                    if inout == 'output':
                        self._output_names.append(name)
                        self.add_output(name, val=map_type(type_, name)(), pass_by_obj=pass_by_obj)
                    else:
                        self._input_names.append(name)
                        self.add_param(name, val=map_type(type_, name)(), pass_by_obj=pass_by_obj)
            if not (self._output_names or self._input_names):
                msg = "'{}' does not define a top-level function named '{}' and does not specify 'variable:' in the first comments".format(mFile, self.basename)
                close_matches = difflib.get_close_matches(self.basename, [f.head.ident.name for f in func_list if type(f) == smop.node.function], 2)
                if close_matches:
                    msg += ". Closest function name: {}".format(" or ".join(close_matches))
                raise ValueError(msg)

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

    def solve_nonlinear(self, params, unknowns, resids):
        args = [params[name] for name in self._input_names]

        def set_unknowns(outputs):
            for i, name in enumerate(self._output_names):
                output = outputs[i]
                if isinstance(output, list) and isinstance(self._init_unknowns_dict[name]['val'], numpy.ndarray):
                    output = numpy.array(output)
                try:
                    unknowns[name] = output
                except ValueError as e:
                    six.reraise(ValueError, ValueError('Could not set output {}.{}: {}'.format(self.name, name, e.message)))

        out = six.StringIO()
        err = six.StringIO()

        try:
            outputs = getattr(self.eng, self.basename)(args, nargout=len(self._output_names), bare=self.bare, stdout=out, stderr=err,
                input_names=self._input_names, output_names=self._output_names)
        except AnalysisError as e:
            print('Error in {}: {}'.format(self.name, e.message))
            raise
        # isinstance(numpy.float64(), float) => True
        if len(self._output_names) == 1 and isinstance(outputs, float):
            # MATLAB returns a single float, not an 1x1 float array
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
