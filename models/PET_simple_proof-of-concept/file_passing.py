import sys
import os
import os.path
import json
import numpy


# Phase 1: called by master interpreter
def invoke(focusObject, rootObject, componentParameters, **kwargs):
    output_dir = componentParameters['output_dir']

    componentParameters['runCommand'] = 'Python.exe ' + os.path.basename(__file__)
    # componentParameters['labels'] = 'Windows'

    with open(__file__, 'rb') as this_py:
        with open(os.path.join(output_dir, os.path.basename(__file__)), 'wb') as output:
            output.write(this_py.read())

# Phase 2: called by OpenMDAO
if __name__ == '__main__':
    with open('testbench_manifest.json', 'rb') as testbench_manifest_json:
        testbench_manifest = json.loads(testbench_manifest_json.read())

    params = dict(((param['Name'], float(param['Value'])) for param in testbench_manifest['Parameters']))

    for metric in testbench_manifest['Metrics']:
        metric['Value'] = params[metric['Name']]

    fileInputs = {fileInput['Name']: fileInput['FileName'] for fileInput in testbench_manifest['FileInputs']}
    for fileOutput in testbench_manifest['FileOutputs']:
        with open(fileOutput['FileName'], 'wb') as output:
            with open(fileInputs[fileOutput['Name']], 'rb') as input:
                output.write(input.read())

    output = json.dumps(testbench_manifest, indent=4)
    with open('testbench_manifest.json', 'wb') as testbench_manifest_json:
        testbench_manifest_json.write(output)
