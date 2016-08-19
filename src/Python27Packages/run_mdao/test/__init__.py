"""Runs a simple regression test."""
from __future__ import absolute_import

import run_mdao
import glob
import os
import os.path
import shutil
import subprocess
import contextlib

_this_dir = os.path.dirname(os.path.abspath(__file__))


@contextlib.contextmanager
def run_regression(output_filename):
    os.chdir(_this_dir)
    yield
    shutil.copyfile('output.csv', output_filename)
    changed = subprocess.check_output('git diff --name-only'.split() + [output_filename])
    if len(changed) > 0:
        raise Exception(changed)


def test_main():
    tb_json_names = glob.glob(os.path.join(_this_dir, '*/testbench_manifest.json'))
    tb_jsons = [(tb_json_name, open(tb_json_name, 'r').read()) for tb_json_name in tb_json_names]
    try:
        for input_filename in glob.glob(_this_dir + '/mdao_config*json'):
            output_filename = input_filename + '.output.csv'
            with run_regression(output_filename):
                run_mdao.run(input_filename)

        with run_regression(os.path.join(_this_dir, 'single_run.csv')):
            run_mdao.run_one('mdao_config_constant.json', (('designVariable.Naca_Code', 4040), ))

        with run_regression(os.path.join(_this_dir, 'run_failure.csv')):
            run_mdao.run_one('mdao_config_basic_CyPhy.json', (('designVariable.y', 0), ('designVariable.x', 'Ia')))

    finally:
        for tb_json_name, contents in tb_jsons:
            with open(tb_json_name, 'w') as tb_json:
                tb_json.write(contents)

if __name__ == '__main__':
    test_main()
