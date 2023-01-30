from matlab_wrapper import MatlabWrapper
from openmdao.api import FileRef
import numpy
import unittest
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


if __name__ == '__main__':
    try:
        unittest.main()
    except SystemExit as e:
        import gc
        gc.collect()
        import sys
        # sys.stdin.readline()
        sys.exit(e.args[0])

# import stat2
# x = numpy.array([12.7, 45.4, 98.9, 26.6, 53.1])
# values = _marray(_dtype(x), _size(x), x)
# (ave, stdev) = stat2(values)

# assert(ave._a[0][0] == 47.339999999999996)
# assert(stdev._a[0][0] == 29.412419145660223)

# ave = 47.3400
# stdev =   29.4124
