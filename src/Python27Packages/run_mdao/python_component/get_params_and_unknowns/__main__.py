from __future__ import absolute_import
from __future__ import print_function
import sys
import json
import run_mdao.python_component
import run_mdao.python_component.get_params_and_unknowns


def main(python_filename):
    c = run_mdao.python_component.PythonComponent(python_filename)
    return json.dumps({'params': c._init_params_dict, 'unknowns': c._init_unknowns_dict},
                      default=run_mdao.python_component.get_params_and_unknowns.json_default)


if __name__ == '__main__':
    print(main(sys.argv[1]))
