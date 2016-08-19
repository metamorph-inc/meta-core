from __future__ import print_function
from __future__ import absolute_import
from __future__ import division
import os
import os.path
import io
import json
import importlib
import time
import contextlib
import numpy
import six

from run_mdao.csv_recorder import MappingCsvRecorder, CsvRecorder
from run_mdao.enum_mapper import EnumMapper
from run_mdao.drivers import FullFactorialDriver, UniformDriver, LatinHypercubeDriver, OptimizedLatinHypercubeDriver, PredeterminedRunsDriver
from run_mdao.restart_recorder import RestartRecorder

from openmdao.api import IndepVarComp, Problem, Group, ScipyOptimizer

from openmdao.core.mpi_wrap import MPI

from run_mdao.testbenchcomponent import TestBenchComponent, _get_param_name
from run_mdao.parallel_execution import par_clone_and_config


__all__ = ('run', 'run_one', 'with_problem')


def CouchDBRecorder(*args, **kwargs):
    """Lazy load CouchDBRecorder."""
    from couchdb_recorder.couchdb_recorder import CouchDBRecorder
    return CouchDBRecorder(*args, **kwargs)


def FmuWrapper(*args, **kwargs):
    from fmu_wrapper import fmu_wrapper
    return fmu_wrapper.FmuWrapper(*args, **kwargs)

if MPI:
    from openmdao.core.petsc_impl import PetscImpl as impl
else:
    from openmdao.api import BasicImpl as impl

# cache the output of a TestBenchComponent if the computation exceeds this many seconds. Otherwise, save memory by throwing it out
CACHE_THRESHOLD_SECONDS = 5


def _memoize_solve(component, fn):
    from run_mdao.array_hashable import array_hashable
    memo = {}
    component.add_output('_runtime', val=0.0)

    def solve_nonlinear(tb_params, unknowns, resids):
        # FIXME: without dict(), this returns wrong values. why?
        tb_params = dict(tb_params)

        def unwrap_val(val):
            if isinstance(val, numpy.ndarray):
                return array_hashable(val)
            return val
        hashable = tuple((unwrap_val(param) for param in tb_params.values()))
        memoized_unknowns = memo.get(hashable, None)
        if memoized_unknowns:
            # print('cache hit')
            for name, value in six.iteritems(memoized_unknowns):
                unknowns[name] = value
            return
        start = time.time()
        fn(tb_params, unknowns, resids=resids)
        unknowns['_runtime'] = time.time() - start
        if time.time() - start >= CACHE_THRESHOLD_SECONDS:
            # memo[hashable] = {key: (value['val'].val if value.get('pass_by_obj', False) else value['val']) for key, value in six.iteritems(unknowns)}
            memo[hashable] = {key: (value['val']) for key, value in six.iteritems(unknowns)}

    return solve_nonlinear


def run_one(filename, input):
    """Run one iteration with specified inputs."""
    original_dir = os.path.dirname(os.path.abspath(filename))

    class OneInputDriver(PredeterminedRunsDriver):
        def __init__(self, *args, **kwargs):
            super(OneInputDriver, self).__init__(original_dir=original_dir, num_samples=1, *args, **kwargs)
            self.use_restart = False

        def _deserialize_or_create_runlist(self):
            return [input]

    return run(filename, override_driver=OneInputDriver())


def instantiate_component(component, component_name, mdao_config, root):
    component_type = component.get('type', 'TestBenchComponent')
    if component_type == 'IndepVarComp':
        def get_unknown_val(unknown):
            if unknown.get('type') is None:
                return unknown['value']
            return {'double': float,
                    'int': int,
                    'string': six.text_type,
                    'array': numpy.array}[unknown['type']](unknown['value'])

        def get_unknown_meta(unknown):
            ret = {'pass_by_obj': True}
            units = unknown.get('units')
            if units:
                ret['units'] = str(units)
            return ret
        vars = ((name, get_unknown_val(unknown), get_unknown_meta(unknown)) for name, unknown in six.iteritems(component['unknowns']))
        return IndepVarComp(vars)
    elif component_type == 'TestBenchComponent':
        tb = TestBenchComponent(component_name, mdao_config, root)
        # FIXME verify this works properly
        tb.solve_nonlinear = _memoize_solve(tb, tb.solve_nonlinear)

        return tb
    elif component_type == 'EnumMap':
        return EnumMapper(component['details']['config'], param_name=_get_param_name('input'))
    elif component_type == 'FMU':
        return FmuWrapper(component['details']['fmu'])
    else:
        if '.' in component_type:
            mod_name = '.'.join(component_type.split('.')[:-1])
            class_name = component_type.split('.')[-1]
            component_instance = getattr(importlib.import_module(mod_name), class_name)(**component['details'])
        else:
            component_instance = locals()[component_type](**component['details'])
        return component_instance


def run(filename, override_driver=None):
    """Run OpenMDAO on an mdao_config."""
    original_dir = os.path.dirname(os.path.abspath(filename))
    if MPI:
        mdao_config = par_clone_and_config(filename)
    else:
        with open(filename, 'r') as mdao_config_json:
            mdao_config = json.loads(mdao_config_json.read())
    with with_problem(mdao_config, original_dir, override_driver) as top:
        top.run()
        return top


@contextlib.contextmanager
def with_problem(mdao_config, original_dir, override_driver=None):
    # TODO: can we support more than one driver
    driver = next(iter(mdao_config['drivers'].values()))

    top = Problem(impl=impl)
    root = top.root = Group()
    recorder = None
    driver_params = {'original_dir': original_dir}
    eval(compile(driver['details'].get('Code', ''), '<driver Code>', 'exec'), globals(), driver_params)

    def get_desvar_path(designVariable):
        return 'designVariable.{}'.format(designVariable)

    if driver['type'] == 'optimizer':
        top.driver = ScipyOptimizer()
        top.driver.options['optimizer'] = str(driver.get('details', {}).get('OptimizationFunction', 'SLSQP'))
    elif driver['type'] == 'parameterStudy':
        drivers = {
            "Uniform": UniformDriver,
            "Full Factorial": FullFactorialDriver,
            "Latin Hypercube": LatinHypercubeDriver,
            "Opt Latin Hypercube": OptimizedLatinHypercubeDriver,
        }
        driver_type = drivers.get(driver['details']['DOEType'])
        if driver_type is None:
            raise Exception('DOEType "{}" is unsupported'.format(driver['details']['DOEType']))
        if override_driver is None:
            top.driver = driver_type(**driver_params)
        else:
            top.driver = override_driver
        seed = getattr(top.driver, 'seed', None)
        if seed is not None:
            print('Using random seed {}'.format(seed))
    elif driver['type'] == 'PCCDriver':
        import PCC.pcc_driver
        driver_params.update(driver['details'])
        top.driver = PCC.pcc_driver.PCCdriver(**driver_params)
    else:
        raise ValueError('Unsupported driver type %s' % driver['type'])

    def add_recorders():
        recorders = []
        design_var_map = {get_desvar_path(designVariable): designVariable for designVariable in driver['designVariables']}
        objective_map = {'{}.{}'.format(objective['source'][0], objective['source'][1]): objective_name for objective_name, objective in six.iteritems(driver['objectives'])}
        constants_map = {}
        for name, constant in (c for c in six.iteritems(mdao_config['components']) if c[1].get('type', 'TestBenchComponent') == 'IndepVarComp'):
            constants_map.update({'{}.{}'.format(name, unknown): unknown for unknown in constant['unknowns']})

        unknowns_map = design_var_map
        unknowns_map.update(objective_map)
        unknowns_map.update(constants_map)
        for recorder in mdao_config.get('recorders', [{'type': 'DriverCsvRecorder', 'filename': 'output.csv'}]):
            if recorder['type'] == 'DriverCsvRecorder':
                mode = 'wb'
                if RestartRecorder.is_restartable(original_dir):
                    mode = 'ab'
                recorder = MappingCsvRecorder({}, unknowns_map, io.open(recorder['filename'], mode))
                if mode == 'ab':
                    recorder._wrote_header = True
            elif recorder['type'] == 'AllCsvRecorder':
                mode = 'wb'
                recorder = CsvRecorder(out=open(recorder['filename'], mode))
            elif recorder['type'] == 'CouchDBRecorder':
                recorder = CouchDBRecorder(recorder.get('url', 'http://localhost:5984/'), recorder['run_id'])
                recorder.options['record_params'] = True
                recorder.options['record_unknowns'] = True
                recorder.options['record_resids'] = False
                recorder.options['includes'] = list(unknowns_map.keys())
            else:
                mod_name = '.'.join(recorder['type'].split('.')[:-1])
                class_name = recorder['type'].split('.')[-1]
                recorder = getattr(importlib.import_module(mod_name), class_name)()

            top.driver.add_recorder(recorder)
        return recorders

    recorders = add_recorders()

    try:
        driver_vars = []
        for var_name, var in six.iteritems(driver['designVariables']):
            if var.get('type', 'double') == 'double':
                default = 0.0
                range_min = var.get('RangeMin')
                range_max = var.get('RangeMax')
                if range_min is not None and range_max is not None:
                    default = range_min + (range_max - range_min) / 2
                driver_vars.append((var_name, default))
            elif var['type'] == 'enum':
                driver_vars.append((var_name, var['items'][0], {"pass_by_obj": True}))
            elif var['type'] == 'int':
                driver_vars.append((var_name, 0))
            else:
                raise ValueError('Unimplemented designVariable type "{}"'.format(var['type']))

        root.add(get_desvar_path('').split('.')[0], IndepVarComp(driver_vars))
        for var_name, var in six.iteritems(driver['designVariables']):
            if var.get('type', 'double') == 'double':
                top.driver.add_desvar(get_desvar_path(var_name), lower=var.get('RangeMin'), upper=var.get('RangeMax'))
            elif var['type'] == 'enum':
                driver_vars.append((var_name, u'', {"pass_by_obj": True}))
                formatted_name = get_desvar_path(var_name)
                top.driver.add_desvar(formatted_name)
                top.driver._desvars[formatted_name]['type'] = var['type']
                top.driver._desvars[formatted_name]['items'] = var['items']
            elif var['type'] == 'int':
                driver_vars.append((var_name, 0.0))
                formatted_name = get_desvar_path(var_name)
                top.driver.add_desvar(formatted_name, lower=var.get('RangeMin'), upper=var.get('RangeMax'))
                top.driver._desvars[formatted_name]['type'] = var['type']
            else:
                raise ValueError('Unimplemented designVariable type "{}"'.format(var['type']))

        def get_sorted_components():
            """Apply Tarjan's algorithm to the Components."""
            visited = {}
            tbs_sorted = []

            def get_ordinal(name):
                ordinal = visited.get(name, -1)
                if ordinal is None:
                    raise ValueError('Loop involving component "{}"'.format(name))
                if ordinal != -1:
                    return ordinal
                component = mdao_config['components'][name]
                visited[name] = None
                ordinal = 0
                for source in (param.get('source') for param in component.get('parameters', {}).values()):
                    if not source:
                        continue
                    if source[0] in mdao_config['drivers']:
                        continue
                    ordinal = max(ordinal, get_ordinal(source[0]) + 1)
                visited[name] = ordinal
                tbs_sorted.append(name)
                return ordinal

            for component_name in mdao_config['components']:
                get_ordinal(component_name)
            return tbs_sorted

        tbs_sorted = get_sorted_components()
        for component_name in tbs_sorted:
            component = mdao_config['components'][component_name]
            mdao_component = instantiate_component(component, component_name, mdao_config, root)
            root.add(component_name, mdao_component)

        for component_name, component in six.iteritems(mdao_config['components']):
            for parameter_name, parameter in six.iteritems(component.get('parameters', {})):
                if parameter.get('source'):
                    source = parameter['source']
                    if source[0] in mdao_config['drivers']:
                        # print('driver{}.{}'.format(source[1], source[1]))
                        root.connect(get_desvar_path(source[1]), '{}.{}'.format(component_name, _get_param_name(parameter_name, component.get('type'))))
                    else:
                        root.connect('{}.{}'.format(source[0], source[1]), '{}.{}'.format(component_name, _get_param_name(parameter_name, component.get('type'))))
                else:
                    pass  # TODO warn or fail?

        if driver['type'] == 'optimizer':
            for objective in six.itervalues(driver['objectives']):
                top.driver.add_objective(str('.'.join(objective['source'])))

        top.setup()
        # from openmdao.devtools.debug import dump_meta
        # dump_meta(top.root)
        yield top
    finally:
        for recorder in recorders:
            recorder.close()
