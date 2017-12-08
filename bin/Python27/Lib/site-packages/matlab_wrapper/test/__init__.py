import matlab_proxy
import unittest
from matlab_wrapper import MatlabWrapper
from openmdao.api import FileRef
import numpy
import os.path


class TestMatlabWrapper(unittest.TestCase):

    def test_unknowns(self):
        c = MatlabWrapper(os.path.join(os.path.dirname(os.path.abspath(__file__)), 'stat2.m'))

        def default(obj):
            if isinstance(obj, FileRef):
                return repr(obj)
            if isinstance(obj, numpy.ndarray):
                return repr(obj)
            raise TypeError(repr(obj) + " is not JSON serializable")

        # print(json.dumps({'params': c._init_params_dict, 'unknowns': c._init_unknowns_dict}, default=default))
        self.assertIn('a', c._init_params_dict)
        self.assertIn('m', c._init_unknowns_dict)

    def test_values(self):
        c = MatlabWrapper(os.path.join(os.path.dirname(os.path.abspath(__file__)), 'stat2.m'))
        unknowns = {}
        c.solve_nonlinear({'a': 2.5, 'b': 3.5, 'c': 4.0}, unknowns, {})
        # print(repr(unknowns))
        self.assertEqual(unknowns['m'], 35)


class TestBareMatlabWrapper(unittest.TestCase):

    def test_unknowns(self):
        c = MatlabWrapper(os.path.join(os.path.dirname(os.path.abspath(__file__)), 'bare_file.m'))

        def default(obj):
            if isinstance(obj, FileRef):
                return repr(obj)
            if isinstance(obj, numpy.ndarray):
                return repr(obj)
            raise TypeError(repr(obj) + " is not JSON serializable")

        # import json
        # print(json.dumps({'params': c._init_params_dict, 'unknowns': c._init_unknowns_dict}, default=default))
        self.assertIn('input1', c._init_params_dict)
        self.assertIn('output2', c._init_unknowns_dict)

    def test_values(self):
        c = MatlabWrapper(os.path.join(os.path.dirname(os.path.abspath(__file__)), 'bare_file.m'))
        unknowns = {}
        c.solve_nonlinear({'input1': 2.5, 'input2': 3.5, 'input3': numpy.array([4.5, 9]), 'input4': 'asdf', 'input5': ['asdffa', 'asdff']}, unknowns, {})
        # print(repr(unknowns))
        self.assertEqual(unknowns['output1'], 5)
        self.assertEqual(numpy.ndarray, type(unknowns['output3']))
        self.assertEqual(unknowns['output3'].tolist(), [[9, 18]]) # TODO: Do we want to reshape 1xn arrays to be 1-dimensional?
        self.assertEqual(unknowns['output4'], 'asdf')
        self.assertEqual(unknowns['output5'], ['asdffa', 'asdff'])


class TestMatlabVersion(unittest.TestCase):
    def test_sort(self):
        matlab_versions = ['8.4', 'wat', '10.2', None, '9.3.2', '9.3', '9.3.1', '10.1', '5.4', '5.11', '5.5']
        matlab_versions.sort(cmp=matlab_proxy.matlab_version_cmp)
        self.assertEqual([None, 'wat', '5.4', '5.5', '5.11', '8.4', '9.3', '9.3.1', '9.3.2', '10.1', '10.2'], matlab_versions)

    def test_get_preferred_matlab(self):
        # cannot rely on the output, but must not crash
        print matlab_proxy.get_preferred_matlab()
