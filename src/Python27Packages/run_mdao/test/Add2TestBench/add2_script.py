from __future__ import absolute_import
from __future__ import print_function

import sys
import os
import os.path
import json
import numpy
import win32com.client
import six


def log(s):
    print(s)


def log_formatted(s):
    print(s)


try:
    import CyPhyPython  # will fail if not running under CyPhyPython
    import cgi

    def log(s):
        CyPhyPython.log(cgi.escape(s))

    def log_formatted(s):
        CyPhyPython.log(s)
except ImportError:
    pass


def is_number(s):
    try:
        float(s)
        return True
    except ValueError:
        return False


def start_pdb():
    ''' Starts pdb, the Python debugger, in a console window
    '''
    import ctypes
    ctypes.windll.kernel32.AllocConsole()
    sys.stdout = open('CONOUT$', 'wt')
    sys.stdin = open('CONIN$', 'rt')
    import pdb
    pdb.set_trace()


# Phase 1: called by master interpreter
def invoke(focusObject, rootObject, componentParameters, **kwargs):
    output_dir = componentParameters['output_dir']

    componentParameters['runCommand'] = 'Python.exe ' + os.path.basename(__file__)
    # componentParameters['labels'] = 'Windows'

    with open(__file__, 'r') as this_py:
        with open(os.path.join(output_dir, os.path.basename(__file__)), 'w') as output:
            output.write(this_py.read())


# Phase 2: called by OpenMDAO
if __name__ == '__main__':
    with open('testbench_manifest.json', 'r') as testbench_manifest_json:
        testbench_manifest = json.loads(testbench_manifest_json.read())

    def unwrap(val):
        if isinstance(val, six.string_types):
            return float(val)
        if isinstance(val, list):
            return numpy.array(val)
        return val
    params = dict(((param['Name'], unwrap(param['Value'])) for param in testbench_manifest['Parameters']))

    for metric in testbench_manifest['Metrics']:
        metric['Value'] = params[metric['Name']] + 2
        # metric['Value'] = numpy.ones((2, 2)) * params[metric['Name']] + 2

    for metric in testbench_manifest['Metrics']:
        if isinstance(metric['Value'], numpy.ndarray):
            metric['Value'] = metric['Value'].tolist()

    for metric in testbench_manifest['Metrics']:
        if isinstance(metric['Value'], numpy.ndarray):
            metric['Value'] = metric['Value'].tolist()
    output = json.dumps(testbench_manifest, indent=4)
    with open('testbench_manifest.json', 'w') as testbench_manifest_json:
        testbench_manifest_json.write(output)
