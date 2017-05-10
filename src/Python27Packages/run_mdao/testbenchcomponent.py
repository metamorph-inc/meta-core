from __future__ import print_function
from __future__ import absolute_import

import os
import os.path
import sys
import json
import subprocess
import six
import numpy
from run_mdao.drivers import AnalysisError
from openmdao.api import Component, FileRef


def _get_param_name(param_name, component_type=None):
    """OpenMDAO won't let us have a parameter and output of the same name..."""
    if component_type is not None and component_type not in ('IndepVarComp', 'TestBenchComponent', 'EnumMap'):
        return param_name
    return 'param_{}'.format(param_name)


class TestBenchComponent(Component):
    def __init__(self, name, mdao_config, root):
        super(TestBenchComponent, self).__init__()
        self.name = name
        self.mdao_config = mdao_config
        self.__directory = mdao_config['components'][name]['details']['directory']
        self.original_testbench_manifest = self._read_testbench_manifest()
        self.manifest_params = {param['Name']: param for param in self.original_testbench_manifest['Parameters']}
        self.manifest_fileinputs = {param['Name']: param for param in self.original_testbench_manifest.get('FileInputs', [])}
        self.manifest_metrics = {param['Name']: param for param in self.original_testbench_manifest['Metrics']}
        self.manifest_fileoutputs = {param['Name']: param for param in self.original_testbench_manifest.get('FileOutputs', {})}

        self.fd_options['force_fd'] = True

        def get_meta(param):
            units = param.get('units')
            if units:
                return {'units': str(units)}
            else:
                return {}
        for param_name, param in six.iteritems(mdao_config['components'][name].get('parameters', {})):
            pass_by_obj = source_is_not_driver = param.get('source', [''])[0] not in mdao_config['drivers']
            val = 0.0
            manifest_fileinput = self.manifest_fileinputs.get(param_name)
            if manifest_fileinput is not None:
                val = FileRef(os.path.join(self.__directory, manifest_fileinput.get('FileName', param_name)))
                self.add_param(_get_param_name(param_name), val=val, binary=True)
                continue
            elif source_is_not_driver and 'source' in param:
                source_component = {c.name: c for c in root.components()}[param['source'][0]]
                val = source_component._init_unknowns_dict[param['source'][-1]]['val']
                pass_by_obj = source_component._init_unknowns_dict[param['source'][-1]].get('pass_by_obj', False)
            elif 'source' in param:
                source_type = mdao_config['drivers'][param['source'][0]]['designVariables'][param['source'][-1]].get('type')
                if source_type == 'enum':
                    val = mdao_config['drivers'][param['source'][0]]['designVariables'][param['source'][-1]]['items'][0]
                    pass_by_obj = True
                elif source_type == "int":
                    val = 0
            else:
                manifest_param = self.manifest_params.get(param_name)
                if manifest_param is not None:
                    val = manifest_param['Value']
                    pass_by_obj = True
                else:
                    raise ValueError('Could not find parameter or input file named {} in testbench_manifest.json'.format(param_name))

            self.add_param(_get_param_name(param_name), val=val, pass_by_obj=pass_by_obj, **get_meta(param))

        for metric_name, metric in six.iteritems(mdao_config['components'][name].get('unknowns', {})):
            manifest_metric = self.manifest_metrics.get(metric_name)
            if manifest_metric is not None:
                pass_by_obj = True
                for driver in mdao_config['drivers'].values():
                    if driver.get('type') != 'optimizer':
                        continue
                    for objective in driver['objectives'].values():
                        if objective['source'][0] == name and objective['source'][1] == metric_name:
                            pass_by_obj = False
                self.add_output(metric_name, val=0.0, pass_by_obj=pass_by_obj, **get_meta(metric))
            else:
                manifest_fileoutput = self.manifest_fileoutputs.get(metric_name)
                if manifest_fileoutput is None:
                    raise ValueError(metric_name)
                self.add_output(metric_name, val=FileRef(os.path.join(self.__directory, manifest_fileoutput.get('FileName', metric_name))), binary=True)

        self.add_output('_ret_code', val=0, pass_by_obj=True)

    def _read_testbench_manifest(self):
        with open(os.path.join(self.__directory, 'testbench_manifest.json'), 'r') as testbench_manifest_json:
            return json.loads(testbench_manifest_json.read())

    def _write_testbench_manifest(self, testbench_manifest):
        # print(repr(testbench_manifest))
        output = json.dumps(testbench_manifest, indent=4)
        with open(os.path.join(self.__directory, 'testbench_manifest.json'), 'w') as testbench_manifest_json:
            testbench_manifest_json.write(output)

    def _run_testbench(self):
        return subprocess.call([sys.executable, '-m', 'testbenchexecutor', '--detailed-errors', 'testbench_manifest.json'], cwd=self.__directory)

    def solve_nonlinear(self, params, unknowns, resids):
        # FIXME: without dict(), this returns wrong values. why?
        for param_name, val in six.iteritems(dict(params)):
            param_name = param_name[len(_get_param_name('')):]
            manifest_param = self.manifest_params.get(param_name)
            if manifest_param:
                # val = param_value['val']
                # if param_value.get('pass_by_obj', True):
                #     val = val.val
                if isinstance(val, numpy.ndarray):
                    # manifest_param['Value'] = list((numpy.asscalar(v) for v in param_value['val']))
                    if len(val) == 1:
                        manifest_param['Value'] = val[0]  # FIXME seems we always get an ndarray from the DOE. Why?
                    else:
                        manifest_param['Value'] = val.tolist()
                else:
                    # manifest_param['Value'] = numpy.asscalar(param_value['val'].val)
                    manifest_param['Value'] = val
            else:
                manifest_fileinput = self.manifest_fileinputs.get(param_name)
                if manifest_fileinput is None:
                    raise ValueError('Could not find parameter or input file named {} in testbench_manifest.json'.format(param_name))

        self._write_testbench_manifest(self.original_testbench_manifest)

        self.ret_code = unknowns['_ret_code'] = self._run_testbench()
        if self.ret_code != 0:
            raise AnalysisError('testbenchexecutor failed with exit code {}'.format(self.ret_code))

        manifest_metrics = {param['Name']: param for param in self._read_testbench_manifest()['Metrics']}

        for metric_name in self.mdao_config['components'][self.name].get('unknowns', {}):
            testbench_metric = manifest_metrics.get(metric_name)
            if testbench_metric is not None:
                value = testbench_metric['Value']
                if isinstance(value, list):
                    unknowns[metric_name] = numpy.array(value)
                else:
                    unknowns[metric_name] = value
            else:
                manifest_fileoutput = self.manifest_fileoutputs.get(metric_name)
                if manifest_fileoutput is None:
                    raise ValueError('Could not find metric "{}" in {}/testbench_manifest.json'.format(metric_name, self.__directory))

    def jacobian(self, params, unknowns, resids):
        raise Exception('unsupported')
