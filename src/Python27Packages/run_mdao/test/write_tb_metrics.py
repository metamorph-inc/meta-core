import os
import json


def load_json_file(filename):
    print("Opening {} ...".format(filename))
    with open(filename) as f_in:
        json_dict = json.load(f_in)
    return json_dict


if __name__ == '__main__':
    execution_dir = os.getcwd()

    # Load testbench_manifest
    testbench_manifest = load_json_file("testbench_manifest.json")

    # TestBench Parameters
    params = {}
    for tb_param in testbench_manifest['Parameters']:
        params[tb_param['Name']] = tb_param['Value']

    # TestBench Metrics
    metrics = {}
    for tb_metric in testbench_manifest['Metrics']:
        metrics[tb_metric['Name']] = tb_metric['Value']

    metrics['FloatMetric'] = 42
    metrics['StringMetric'] = "hello world"
    metrics['UnicodeMetric'] = "abc"
    metrics['FloatMetric_Empty'] = 42
    metrics['StringMetric_Empty'] = "hello world"
    metrics['UnicodeMetric_Empty'] = "abc"

    # Write to testbench_manifest metric
    for tb_metric in testbench_manifest['Metrics']:
        if tb_metric['Name'] in metrics:
            tb_metric['Value'] = metrics[tb_metric['Name']]
    
    # Save updated testbench_manifest.json
    with open("testbench_manifest.json", 'w') as savefile:
        json_str = json.dumps(testbench_manifest, sort_keys=True, indent=2, separators=(',', ': '))
        savefile.write(json_str)
