from __future__ import absolute_import
from __future__ import print_function
import json
import numpy
import hashlib
import six


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

    x = params['x']
    y = params['y']

    with open(testbench_manifest['FileInputs'][0]['FileName'], 'rb') as input:
        with open(testbench_manifest['FileOutputs'][0]['FileName'], 'wb') as output:
            input_contents = input.read()
            checksum = int(hashlib.md5(input_contents).hexdigest(), 16)

            output.write(input_contents)

    for metric in testbench_manifest['Metrics']:
        metric['Value'] = checksum

    output = json.dumps(testbench_manifest, indent=4)
    with open('testbench_manifest.json', 'w') as testbench_manifest_json:
        testbench_manifest_json.write(output)
