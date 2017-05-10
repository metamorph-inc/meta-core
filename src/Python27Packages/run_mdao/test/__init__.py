"""Runs a simple regression test."""
from __future__ import absolute_import

import run_mdao
import glob
import os
import os.path
import shutil
import subprocess
import unittest
import contextlib
import csv
import hashlib

_this_dir = os.path.dirname(os.path.abspath(__file__))


@contextlib.contextmanager
def run_regression(output_filename):
    os.chdir(_this_dir)
    yield
    shutil.copyfile('output.csv', output_filename)
    changed = subprocess.check_output('git diff --name-only'.split() + [output_filename])
    if len(changed) > 0:
        raise Exception(changed)


class RegressionTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        tb_json_names = glob.glob(os.path.join(_this_dir, '*/testbench_manifest.json'))
        cls.tb_jsons = [(tb_json_name, open(tb_json_name, 'r').read()) for tb_json_name in tb_json_names]
        artifacts_dir = os.path.join(_this_dir, 'artifacts')
        if os.path.exists(artifacts_dir):
            shutil.rmtree(artifacts_dir)

    @classmethod
    def tearDownClass(cls):
        for tb_json_name, contents in cls.tb_jsons:
            with open(tb_json_name, 'w') as tb_json:
                tb_json.write(contents)

    def _test_mdao_config(self, input_filename, output_filename):
        with run_regression(output_filename):
            run_mdao.run(input_filename)

    def test_single_run(self):
        with run_regression(os.path.join(_this_dir, 'single_run.csv')):
            run_mdao.run_one('mdao_config_constant.json', (('designVariable.Naca_Code', 4040), ))

    def test_run_failure(self):
        with run_regression(os.path.join(_this_dir, 'run_failure.csv')):
            run_mdao.run_one('mdao_config_basic_CyPhy.json', (('designVariable.y', 0), ('designVariable.x', 'Ia')))

    def test_mdao_config_artifacts(self):
        os.chdir(_this_dir)
        run_mdao.run('mdao_config_artifacts.json')
        with open('output.csv') as output:
            csvreader = iter(csv.reader(output))
            headers = next(csvreader)
            for row in csvreader:
                guid = row[headers.index('GUID')]
                self.assertTrue(os.path.isfile(os.path.join('artifacts', guid, 'bin.out')))

    def test_mdao_config_file_testbench(self):
        os.chdir(_this_dir)
        run_mdao.run('mdao_config_file_testbench.json')
        with open('output.csv') as output:
            csvreader = iter(csv.reader(output))
            headers = next(csvreader)
            for row in csvreader:
                checksum_file = row[headers.index('checksum')]
                checksum_expected = str(int(hashlib.md5('sample data').hexdigest(), 16))
                self.assertEqual(checksum_file, checksum_expected)


for input_filename in glob.glob(_this_dir + '/mdao_config*json'):
    output_filename = input_filename + '.output.csv'

    def _add_test(input_filename, output_filename):
        def test(self):
            self._test_mdao_config(input_filename, output_filename)

        name = 'test_' + os.path.splitext(os.path.basename(input_filename))[0]
        if getattr(RegressionTest, name, None) is None:
            test.__name__ = name
            setattr(RegressionTest, name, test)

    _add_test(input_filename, output_filename)


if __name__ == '__main__':
    unittest.main()
