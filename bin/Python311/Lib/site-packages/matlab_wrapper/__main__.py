import sys
import json
from matlab_wrapper import MatlabWrapper
from openmdao.api import FileRef
import numpy

if __name__ == '__main__':
    # print(repr(sys.argv[1:]))
    c = MatlabWrapper(sys.argv[1], start_engine=False)

    def default(obj):
        if isinstance(obj, FileRef):
            return repr(obj)
        if isinstance(obj, numpy.ndarray):
            return repr(obj)
        raise TypeError(repr(obj) + " is not JSON serializable")

    print(json.dumps({'params': c._init_params_dict, 'unknowns': c._init_unknowns_dict}, default=default))
