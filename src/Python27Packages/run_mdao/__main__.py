from __future__ import absolute_import
import run_mdao
from run_mdao.drivers import CsvDriver
import argparse
import json
import sys
import os.path
import six
import numpy

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Run a mdao_config.json using OpenMDAO')
    parser.add_argument('filename', nargs='?', default='mdao_config.json')
    parser.add_argument('--one-component', help='component name')
    parser.add_argument('--desvar-input', help='design variable csv input')
    parser.add_argument('--append-csv', action='store_true', help='append to CSV instead of overwriting')
    parser.add_argument('--profile', action='store_true', help='enable OpenMDAO instance-based profiling')

    args = parser.parse_args()

    if args.one_component:
        filename = args.filename
        with open(filename, 'r') as mdao_config_json:
            mdao_config = json.load(mdao_config_json)
        component = mdao_config['components'][args.one_component]
        if component.get('type') == 'IndepVarComp':
            raise Exception('Doesnt make sense to run IndepVarComp')
        with run_mdao.with_problem(mdao_config, os.path.dirname(os.path.abspath(filename))) as top:
            # print repr({name: parameter['value'] for name, parameter in six.iteritems(component.get('parameters', {}))})
            mdao_component = {c.name: c for c in top.root.components()}[args.one_component]
            unknowns = {}
            resids = {}
            mdao_component.solve_nonlinear({run_mdao._get_param_name(name, component.get('type')): parameter['value'] for name, parameter in six.iteritems(component.get('parameters', {}))}, unknowns, resids)
            if component.get('type', 'TestBenchComponent') == 'TestBenchComponent':
                if mdao_component.ret_code != 0:
                    raise Exception('Component execution failed')

            def serialize(val):
                if isinstance(val, numpy.ndarray):
                    return val.tolist()
                return val
            sys.stdout.write(json.dumps({name: serialize(val) for name, val in six.iteritems(unknowns)}))

    else:
        run_kwargs = {}
        if args.desvar_input:
            original_dir = os.path.dirname(os.path.abspath(args.filename))
            run_kwargs['override_driver'] = CsvDriver(original_dir, args.desvar_input)

        run_kwargs['append_csv'] = args.append_csv
        run_kwargs['profile'] = args.profile

        run_mdao.run(args.filename, **run_kwargs)
