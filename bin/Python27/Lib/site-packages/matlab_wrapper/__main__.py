import sys
import json
from matlab_wrapper import MatlabWrapper

if __name__ == '__main__':
    # print(repr(sys.argv[1:]))
    c = MatlabWrapper(sys.argv[1], start_engine=False)
    print((json.dumps({'params': c._init_params_dict, 'unknowns': c._init_unknowns_dict})))
