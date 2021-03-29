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
import itertools
import numpy
import six
from collections import defaultdict, Mapping

from run_mdao.csv_recorder import MappingCsvRecorder, CsvRecorder
from run_mdao.enum_mapper import EnumMapper
from run_mdao.drivers import FullFactorialDriver, UniformDriver, LatinHypercubeDriver, OptimizedLatinHypercubeDriver, PredeterminedRunsDriver, CsvDriver
from run_mdao.restart_recorder import RestartRecorder

from openmdao.api import IndepVarComp, Problem, Group, ScipyOptimizer, FileRef, SubProblem, Component
from openmdao.api import profile as openmdao_profile

from openmdao.core.mpi_wrap import MPI

from run_mdao.testbenchcomponent import TestBenchComponent, _get_param_name
from run_mdao.parallel_execution import par_clone_and_config

import testbenchexecutor.progress_service


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
    # don't attempt to memoize components with FileRefs
    if [v for v in itertools.chain(component._init_unknowns_dict.values(), component._init_params_dict.values()) if isinstance(v['val'], FileRef)]:
        return fn

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
            memo[hashable] = {key: unknowns[key] for key in unknowns}

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


def instantiate_component(component, component_name, mdao_config, root, subproblem_output_meta):
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
        tb = TestBenchComponent(component_name, mdao_config, root, subproblem_output_meta)
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


def run(filename, override_driver=None, additional_recorders=(), append_csv=False, profile=False):
    """Run OpenMDAO on an mdao_config."""
    original_dir = os.path.dirname(os.path.abspath(filename))
    if MPI:
        mdao_config = par_clone_and_config(filename)
    else:
        with open(filename, 'r') as mdao_config_json:
            mdao_config = json.loads(mdao_config_json.read())
    with with_problem(mdao_config, original_dir, override_driver, additional_recorders=additional_recorders, append_csv=append_csv, profile=profile) as top:
        top.run()
        return top


def get_desvar_path(designVariable):
    return 'designVariable.{}'.format(designVariable)


@contextlib.contextmanager
def with_problem(mdao_config, original_dir, override_driver=None, additional_recorders=(), is_subproblem=False, append_csv=False, profile=False):
    testbenchexecutor.progress_service.update_progress("Configuring PET...", -1, -1)
    # TODO: can we support more than one driver
    if len(mdao_config['drivers']) == 0:
        driver = None
    else:
        driver = next(iter(mdao_config['drivers'].values()))

    top = Problem(impl=impl)
    root = top.root = Group()
    recorder = None
    driver_params = {'original_dir': original_dir}
    if driver is not None:
        eval(compile(driver['details'].get('Code', ''), '<driver Code>', 'exec'), globals(), driver_params)

    subProblemInputMeta = {}
    subProblemOutputMeta = {}

    if driver is not None:
        if driver['type'] == 'optimizer':
            if driver.get('details', {}).get('OptimizationFunction') == 'Custom':
                class_path = driver['details']['OptimizationClass'].split('.')
                mod = __import__('.'.join(class_path[:-1]), fromlist=[class_path[-1]])
                top.driver = getattr(mod, class_path[-1])()
            else:
                top.driver = ScipyOptimizer()
                top.driver.options['optimizer'] = str(driver.get('details', {}).get('OptimizationFunction', 'SLSQP'))

            opt_settings = driver_params.get("opt_settings", {})

            # Pass through opt_settings dict to OpenMDAO; this allows the
            # user to set options on ScipyOptimizer optimizers that OpenMDAO
            # doesn't explicitly support, like COBYLA's rhobeg
            # (We don't do this in the block above that initializes
            # ScipyOptimizer to allow for other optimizers that also use
            # opt_settings)
            if type(opt_settings) is dict and hasattr(top.driver, "opt_settings") and isinstance(top.driver.opt_settings, Mapping):
                for key, value in six.iteritems(opt_settings):
                    top.driver.opt_settings[key] = value

            for key, value in six.iteritems(driver_params):
                try:
                    top.driver.options[key] = value
                except KeyError:
                    pass  # Ignore options that aren't valid for driver
        elif driver['type'] == 'parameterStudy':
            drivers = {
                "Uniform": UniformDriver,
                "Full Factorial": FullFactorialDriver,
                "Latin Hypercube": LatinHypercubeDriver,
                "Opt Latin Hypercube": OptimizedLatinHypercubeDriver,
                "CSV File": CsvDriver,
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
            driver_params["_run_mdao_subproblem_output_meta"] = subProblemOutputMeta
            top.driver = PCC.pcc_driver.PCCdriver(**driver_params)
        else:
            raise ValueError('Unsupported driver type %s' % driver['type'])

        driver_vars = []
        for var_name, var in six.iteritems(driver['designVariables']):
            if var.get('type', 'double') == 'double':
                default = 0.0
                range_min = var.get('RangeMin')
                range_max = var.get('RangeMax')
                if range_min is not None and range_max is not None:
                    default = range_min + (range_max - range_min) / 2
                driver_vars.append((var_name, default, {}))
            elif var['type'] == 'enum':
                driver_vars.append((var_name, var['items'][0], {"pass_by_obj": True}))
            elif var['type'] == 'int':
                driver_vars.append((var_name, 0, {}))
            else:
                raise ValueError('Unimplemented designVariable type "{}"'.format(var['type']))
            units = var.get('units')
            if units:
                driver_vars[-1][2]['units'] = str(var['units'])

        root.add(get_desvar_path('').split('.')[0], IndepVarComp(driver_vars))
        for var_name, var in six.iteritems(driver['designVariables']):
            if var.get('type', 'double') == 'double':
                top.driver.add_desvar(get_desvar_path(var_name), lower=var.get('RangeMin'), upper=var.get('RangeMax'))
            elif var['type'] == 'enum':
                driver_vars.append((var_name, var['items'][0], {"pass_by_obj": True}))
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

    for subProblemName, subProblemConfig in six.iteritems(mdao_config.get('subProblems', {})):
        subProblemDir = os.path.join(original_dir, subProblemName)

        with with_problem(subProblemConfig, subProblemDir, is_subproblem=True) as (subProblem, inputMeta, outputMeta):
            root.add(subProblemName, subProblem)
            subProblemInputMeta[subProblemName] = inputMeta
            subProblemOutputMeta[subProblemName] = outputMeta

    if is_subproblem:
        subProblemInputs = []
        inputMeta = {}
        for name, problemInput in six.iteritems(mdao_config['problemInputs']):
            if problemInput.get("innerSource"):
                if problemInput["innerSource"][0] in mdao_config['drivers']:
                    path = get_desvar_path(problemInput["innerSource"][1])
                else:
                    path = '{}.{}'.format(problemInput["innerSource"][0], problemInput["innerSource"][1])

                subProblemInputs.append(path)
                inputMeta[name] = path
            else:
                # TODO: How important is it to figure out the correct type here?
                #   We might be able to infer the type from a component that connects to
                #   this ProblemInput, but might have to refer to something outside the
                #   subproblem
                (initial_value, pass_by_obj) = get_problem_input_value(problemInput)

                root.add(name, IndepVarComp(name, initial_value, pass_by_obj=pass_by_obj))
                path = "{0}.{0}".format(name)
                subProblemInputs.append(path)
                inputMeta[name] = path

        # TODO: Handle direct connection between ProblemInput and ProblemOutput (single-element Source)
        # TODO: Pass-through ExecComps to allow direct ProblemInput->ProblemOutput connections to behave
        subProblemOutputs = []
        outputMeta = {}
        for name, source in six.iteritems(mdao_config['problemOutputs']):
            if len(source) == 1:
                if source[0] in mdao_config['problemInputs'] and 'innerSource' in mdao_config['problemInputs'][source[0]]:
                    # Assume inner source is a design variable
                    desvar = driver['designVariables']
                    passByObj = False
                    if desvar.get('type', 'double') == 'double':
                        initialVal = 0.0
                    elif desvar['type'] == 'enum':
                        initialVal = ''
                        # TODO or maybe initialVal = 0.0
                    elif desvar['type'] == 'int':
                        initialVal = 0
                    else:
                        raise ValueError('Unimplemented designVariable type "{}"'.format(desvar['type']))
                else:
                    if source[0] in mdao_config['problemInputs']:
                        (initialVal, passByObj) = get_problem_input_value(mdao_config['problemInputs'][source[0]])
                    else:
                        raise ValueError('Missing ProblemOutput source: {}'.format(source[0]))
                comp_name = "pass_through_{}".format(name)
                comp = PassThroughComponent()
                comp.add_var(name, initialVal)
                root.add(comp_name, comp)
                inputPath = "{}.{}".format(comp_name, name)
                path = "{}.{}_out".format(comp_name, name)
                root.connect(inputMeta[source[0]], inputPath)
            else:
                if source[0] in mdao_config['drivers']:
                    # TODO: If it's legal for this desvar to also point to a ProblemInput,
                    # we need to create a PassThroughComponent just like above
                    this_driver = mdao_config['drivers'][source[0]]

                    if source[1] in this_driver.get('designVariables', {}):
                        path = get_desvar_path(source[1])
                    else:  # Source is an objective, ivar, or constraint; need to get the actual source
                        if source[1] in this_driver.get('objectives', {}):
                            driver_output_type = 'objectives'
                        elif source[1] in this_driver.get('constraints', {}):
                            driver_output_type = 'constraints'
                        elif source[1] in this_driver.get('intermediateVariables', {}):
                            driver_output_type = 'intermediateVariables'
                        else:
                            raise ValueError('Driver output "{}"" not found'.format(source[1]))
                        real_source = this_driver[driver_output_type][source[1]]['source']
                        if real_source[0] in mdao_config['subProblems']:
                            unknown_name = subProblemOutputMeta[real_source[0]][real_source[1]]
                            path = '{}.{}'.format(real_source[0], unknown_name)
                        else:
                            path = '{}.{}'.format(real_source[0], real_source[1])
                elif source[0] in mdao_config['subProblems']:
                    unknown_name = subProblemOutputMeta[source[0]][source[1]]
                    path = '{}.{}'.format(source[0], unknown_name)
                else:
                    path = '{}.{}'.format(source[0], source[1])

            subProblemOutputs.append(path)
            outputMeta[name] = path

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
                if source[0] in mdao_config.get('problemInputs', {}):
                    continue
                if source[0] in mdao_config.get('subProblems', {}):
                    continue
                ordinal = max(ordinal, get_ordinal(source[0]) + 1)
            visited[name] = ordinal
            tbs_sorted.append(name)
            return ordinal

        for component_name in mdao_config['components']:
            get_ordinal(component_name)
        return tbs_sorted

    tbs_sorted = get_sorted_components()
    # TestBenchComponents look at params they're connected to, so create them last
    def is_testbenchcomponent(component_name):
        return mdao_config['components'][component_name].get('type', 'TestBenchComponent') == 'TestBenchComponent'
    tbs_sorted = sorted(tbs_sorted, key=is_testbenchcomponent)
    for component_name in tbs_sorted:
        component = mdao_config['components'][component_name]
        mdao_component = instantiate_component(component, component_name, mdao_config, root, subProblemOutputMeta)
        root.add(component_name, mdao_component)
        # Create directories for output FileRefs
        if is_testbenchcomponent(component_name):
            # Only create subdirectories if an Output FileRef needs it
            for k, v in mdao_component.manifest_fileoutputs.items():
                if os.path.dirname(v['FileName']) != "":  # Note: https://stackoverflow.com/a/8384788 but OpenMETA runs only on Windows anyway
                    mdao_component.create_dirs = True
                    break

    for component_name, component in six.iteritems(mdao_config['components']):
        for parameter_name, parameter in six.iteritems(component.get('parameters', {})):
            if parameter.get('source'):
                source = parameter['source']
                if len(source) == 1 and is_subproblem:
                    # Points to a top-level ProblemInput; look up what design variable it points to and reference that IVC
                    problemInputName = source[0]
                    if problemInputName in inputMeta:
                        root.connect(inputMeta[problemInputName], '{}.{}'.format(component_name, _get_param_name(parameter_name, component.get('type'))))
                    else:
                        # TODO: Warn or fail if name isn't found?
                        print("WARNING: Missing problem input reference")
                else:
                    if source[0] in mdao_config['drivers']:
                        # print('driver{}.{}'.format(source[1], source[1]))
                        root.connect(get_desvar_path(source[1]), '{}.{}'.format(component_name, _get_param_name(parameter_name, component.get('type'))))
                    elif source[0] in mdao_config.get('subProblems', {}):
                        # Map subproblem output name to actual unknown name
                        unknown_name = subProblemOutputMeta[source[0]][source[1]]
                        root.connect('{}.{}'.format(source[0], unknown_name), '{}.{}'.format(component_name, _get_param_name(parameter_name, component.get('type'))))
                    else:
                        root.connect('{}.{}'.format(source[0], source[1]), '{}.{}'.format(component_name, _get_param_name(parameter_name, component.get('type'))))
            else:
                pass  # TODO warn or fail?

    for subProblemName, subProblem in six.iteritems(mdao_config.get('subProblems', {})):
        for problemInputName, problemInput in six.iteritems(subProblem.get('problemInputs', {})):
            if problemInput.get('outerSource'):
                source = problemInput['outerSource']
                subProblemInputPath = subProblemInputMeta[subProblemName][problemInputName]

                if len(source) == 1 and is_subproblem:
                    problemInputName = source[0]
                    if problemInputName in inputMeta:
                        root.connect(inputMeta[problemInputName], '{}.{}'.format(subProblemName, subProblemInputPath))
                    else:
                        # TODO: Warn or fail if name isn't found?
                        print("WARNING: Missing problem input reference")
                else:
                    if source[0] in mdao_config['drivers']:
                        # print('driver{}.{}'.format(source[1], source[1]))
                        root.connect(get_desvar_path(source[1]), '{}.{}'.format(subProblemName, subProblemInputPath))
                    elif source[0] in mdao_config['subProblems']:
                        # Map subproblem output name to actual unknown name
                        unknown_name = subProblemOutputMeta[source[0]][source[1]]
                        root.connect('{}.{}'.format(source[0], unknown_name), '{}.{}'.format(subProblemName, subProblemInputPath))
                    else:
                        root.connect('{}.{}'.format(source[0], source[1]), '{}.{}'.format(subProblemName, subProblemInputPath))
            else:
                print("Failed to connect")
                pass  # TODO warn or fail?

    # TODO: Make sure objectives/constraints coming from subproblems and
    # from ProblemInputs/ProblemOutputs are handled correctly
    if driver is not None and driver['type'] == 'optimizer':
        for objective in six.itervalues(driver['objectives']):
            if objective["source"][0] in mdao_config.get('subProblems', {}):
                unknown_name = subProblemOutputMeta[objective["source"][0]][objective["source"][1]]
                top.driver.add_objective("{}.{}".format(objective["source"][0], unknown_name))
            else:
                top.driver.add_objective(str('.'.join(objective['source'])))

        for constraint in six.itervalues(driver['constraints']):
            if constraint['source'][0] in mdao_config['drivers']:
                # References the driver; need to get the path to the design var
                constraintPath = get_desvar_path(constraint['source'][1])
            elif constraint['source'][0] in mdao_config.get('subProblems', {}):
                unknown_name = subProblemOutputMeta[objective.source[0]][objective.source[1]]
                constraintPath = "{}.{}".format(objective.source[0], unknown_name)
            else:
                constraintPath = str('.'.join(constraint['source']))

            if 'RangeMin' in constraint and 'RangeMax' in constraint:
                top.driver.add_constraint(constraintPath, lower=constraint['RangeMin'], upper=constraint['RangeMax'])
            elif 'RangeMin' in constraint and 'RangeMax' not in constraint:
                top.driver.add_constraint(constraintPath, lower=constraint['RangeMin'])
            elif 'RangeMin' not in constraint and 'RangeMax' in constraint:
                top.driver.add_constraint(constraintPath, upper=constraint['RangeMax'])
            else:
                pass  # TODO: No min or max provided with constraint; warn or fail here?

    def add_recorders():
        recorders = []

        unknowns_map = defaultdict(list)
        def update_unknowns(key, val):
            unknowns_map[key].append(val)

        for designVariable in driver['designVariables']:
            update_unknowns(get_desvar_path(designVariable), designVariable)

        for objective_name, objective in six.iteritems(driver['objectives']):
            update_unknowns('{}.{}'.format(objective['source'][0], objective['source'][1]), objective_name)

        for intermediate_var_name, intermediate_var in six.iteritems(driver.get('intermediateVariables', {})):
            update_unknowns('{}.{}'.format(intermediate_var['source'][0], intermediate_var['source'][1]), intermediate_var_name)

        for name, constant in (c for c in six.iteritems(mdao_config['components']) if c[1].get('type', 'TestBenchComponent') == 'IndepVarComp'):
            for unknown in constant['unknowns']:
                update_unknowns('{}.{}'.format(name, unknown), unknown)

        for constraint_name, constraint in six.iteritems(driver.get('constraints', {})):
            if constraint['source'][0] not in mdao_config['drivers']: # All constraints that don't point back to design variables
                update_unknowns('{}.{}'.format(constraint['source'][0], constraint['source'][1]), constraint_name)

        new_unknowns_map = defaultdict(list)
        # Locate/fix any unknowns that point to subproblem outputs
        for unknown_path, unknown_names in six.iteritems(unknowns_map):
            for unknown_name in unknown_names:
                split_path = unknown_path.split('.')
                if split_path[0] in subProblemOutputMeta:
                    split_path[1] = subProblemOutputMeta[split_path[0]][split_path[1]]
                    new_path = '.'.join(split_path)
                    new_unknowns_map[new_path].append(unknown_name)
                else:
                    new_unknowns_map[unknown_path].append(unknown_name)

        unknowns_map = new_unknowns_map

        for recorder in mdao_config.get('recorders', [{'type': 'DriverCsvRecorder', 'filename': 'output.csv'}]):
            if recorder['type'] == 'DriverCsvRecorder':
                mode = 'w'
                exists = os.path.isfile(recorder['filename'])
                if RestartRecorder.is_restartable(original_dir) or append_csv:
                    mode = 'a'
                if six.PY2:
                    mode += 'b'
                    open_kwargs = {}
                else:
                    open_kwargs = {'newline': ''}
                if "class" in recorder:
                    mod_name = '.'.join(recorder['class'].split('.')[:-1])
                    class_name = recorder['class'].split('.')[-1]
                    recorder_class = getattr(importlib.import_module(mod_name), class_name)
                else:
                    recorder_class = MappingCsvRecorder
                recorder = recorder_class({}, unknowns_map, io.open(recorder['filename'], mode, **open_kwargs), include_id=recorder.get('include_id', False))
                if (append_csv and exists) or mode == 'ab':
                    recorder._wrote_header = True
            elif recorder['type'] == 'AllCsvRecorder':
                mode = 'w'
                if six.PY2:
                    mode += 'b'
                    open_kwargs = {}
                else:
                    open_kwargs = {'newline': ''}
                recorder = CsvRecorder(out=io.open(recorder['filename'], mode, **open_kwargs))
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
            recorders.append(recorder)
        return recorders

    if is_subproblem:
        recorders = []

        # print("SubProblemInputs:", subProblemInputs)
        # print("SubProblemOutputs:", subProblemOutputs)

        # print("InputMeta:", inputMeta)
        # print("OutputMeta:", outputMeta)

        top.setup()

        subProblem = SubProblem(top, params=subProblemInputs, unknowns=subProblemOutputs)

        yield (subProblem, inputMeta, outputMeta)
    else:
        if driver:
            recorders = add_recorders()
        else:
            recorders = []
        for recorder in additional_recorders:
            recorders.append(recorder)
            top.driver.add_recorder(recorder)

        if profile:
            openmdao_profile.setup(top)
            openmdao_profile.start()

        try:
            top.setup()
            # from openmdao.devtools.debug import dump_meta
            # dump_meta(top.root)

            # for subproblemName in six.iterkeys(mdao_config['subProblems']):
            #     subProblem = top.root.find_subsystem(subproblemName)
            #     subProblem._problem.root.dump(verbose=True)

            # top.root.dump(verbose=True)
            yield top
        finally:
            for recorder in recorders:
                recorder.close()


class PassThroughComponent(Component):
    '''
    OpenMDAO component which passes through its inputs to its
    outputs, unmodified.
    '''

    def __init__(self):
        super(PassThroughComponent, self).__init__()

    def add_var(self, *args, **kwargs):
        self.add_param(*args, **kwargs)
        argsList = list(args)
        argsList[0] = argsList[0] + "_out"  # Need to suffix name to avoid name collision
        self.add_output(*argsList, **kwargs)

    def solve_nonlinear(self, params, unknowns, resids):
        for name in six.iterkeys(params):
            unknowns[name + "_out"] = params[name]


# For a given problem_input dict, gets the initial value (as a decoded Python object) and
# pass_by_obj flag (returned as a tuple)
def get_problem_input_value(problem_input):
    initial_value = 0.0
    pass_by_obj = False

    if "value" in problem_input:
        initial_value_str = problem_input["value"]
        initial_value = eval(initial_value_str, globals())

    if "pass_by_obj" in problem_input:
        pass_by_obj = problem_input["pass_by_obj"]

    return (initial_value, pass_by_obj)
