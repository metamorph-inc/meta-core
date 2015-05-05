import os
import time
import json
import datetime
import logging
import subprocess
from py_modelica.modelica_simulation_tools.tool_base import ToolBase
from py_modelica.utility_functions import subprocess_call
from py_modelica.exception_classes import ModelicaInstantiationError, ModelicaCompilationError,\
    ModelicaSimulationError


class JModelica(ToolBase):

    tool_name = "JModelica"
    model_statistics = {}
    compile_errors = []
    system_python_path = os.path.abspath("C:\\Python27\\python.exe")
    simulate_failed_file = '_j_simulate_FAILED.txt'
    compile_failed_file = '_j_compile_FAILED.txt'

    def __init__(self, model_config, jm_home="C:\\JModelica.org-1.12"):
        """
        Constructor for JModelica class, called before ToolBase's
        _initialize.

        """
        jm_home = os.path.abspath(jm_home)

        if not jm_home:
            if os.name == 'nt':
                self.tool_path = self._query_registry()
            else:
                msg = 'You need to pass the home dir for JModelica.org when not using Windows.'
                raise ModelicaInstantiationError(msg)
        elif os.path.exists(jm_home):
            self.tool_path = jm_home
        else:
            msg = "Given JModelica home not found at: {0}".format(jm_home)
            raise ModelicaInstantiationError(msg)

        self._initialize(model_config)
    # end of _set_tool_home

    def _query_registry(self):
        """

        Queries the registry for the installation path of JModelica.
        (HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Windows\CurrentVersion\Uninstall\JModelica.org-1.11
        OR HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\JModelica.org-1.12
        - this registry key has the install location of JModelica)
        """
        #TODO: Set this up for JModelica
        import _winreg

        base_key = r'SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall'

        uninstall_key = _winreg.OpenKey(_winreg.HKEY_LOCAL_MACHINE, base_key, 0,
                                        _winreg.KEY_ALL_ACCESS | _winreg.KEY_WOW64_32KEY)

        number_of_keys = _winreg.QueryInfoKey(uninstall_key)[0]  # 0 means number of sub_keys

        product_key = None
        for sub_key_id in range(0, number_of_keys):
            sub_key_name = _winreg.EnumKey(uninstall_key, sub_key_id)
            sub_key = _winreg.OpenKey(uninstall_key, sub_key_name)
            number_of_values = _winreg.QueryInfoKey(sub_key)[1]
            for value_id in range(0, number_of_values):
                value_tuple = _winreg.EnumValue(sub_key, value_id)
                value_name = value_tuple[0]
                value = value_tuple[1]
                if value_name == 'DisplayName' and value == display_name:
                    product_key = sub_key

        if not product_key:
            return "", ""

        install_location = ""
        display_version = ""
        number_of_values = _winreg.QueryInfoKey(product_key)[1]
        for value_id in range(0, number_of_values):
            value_tuple = _winreg.EnumValue(product_key, value_id)
            value_name = value_tuple[0]
            value = value_tuple[1]
            if value_name == "InstallLocation":
                install_location = value
            elif value_name == "DisplayVersion":
                display_version = value

        return install_location, display_version
    # end of _query_registry

    def _setup_libs_env_vars(self):
        """
        Function adds given library paths to environment-variable MODELICAPATH.
        Which is updated in os.environ and returned.

        """
        #TODO: Make sure this works for JModelica too.
        log = logging.getLogger()
        log.debug('Entered _setup_libs_env_vars')

        my_env = os.environ
        lib_paths = ""
        
        cyphy_dir = os.path.abspath(os.getcwd())
        results_dir = os.path.join(cyphy_dir, '..')
        
        self.lib_package_paths.append(results_dir)

        # supports relative paths and adds the package paths if exist
        for lib_path in self.lib_package_paths:
            lib_full_path = os.path.abspath(lib_path)
            if os.path.exists(lib_full_path):
                lib_paths += '"{0}"'.format(lib_full_path)
                lib_paths += os.pathsep
            else:
                print "The library path {0} does not exist, this might lead to errors.".format(lib_full_path)

        if 'MODELICAPATH' in my_env:
            my_env['MODELICAPATH'] += os.pathsep + lib_paths
        else:
            my_env.update({'MODELICAPATH': lib_paths})

        log.info("MODELICAPATH : {0}".format(my_env['MODELICAPATH']))
        log.info("Libraries : {0}".format(self.lib_package_names))

        return my_env
    # end of _setup_libs_env_vars

    def compile_model(self):
        """
        Compile the model

        """

        log = logging.getLogger()
        log.debug("Entered compile_model JModelica")

        # create a .cmd file which will:
        #   1) run %JMODELICAHOME%\\setenv.bat and
        #   2) call a python script to compile the model

        jmodelica_setenv_path = os.path.join(self.tool_path, 'setenv.bat')

        with open(os.path.join('jmodelica_compile.cmd'), 'wb') as file_out:
            lines = [r':: Use System Python (which should have access to JModelica) to compile the model',
                     r'echo off',
                     r'pushd %~dp0',
                     r'call {0}'.format(jmodelica_setenv_path),
                     r'"{0}" ..\scripts\jmodelica_compile.py {1}'.format(self.system_python_path, self.model_name),
                     r'popd']
            file_out.write("\n".join(lines))

        if os.path.isfile(self.compile_failed_file):
            os.remove(self.compile_failed_file)
        if os.path.isfile('j_compile.log'):
            os.remove('j_compile.log')

        t_start = time.time()
        try:
            return_string = subprocess_call(os.path.join('jmodelica_compile.cmd'), log)
            log.debug(return_string)
            if os.path.isfile(self.compile_failed_file):
                raise ModelicaCompilationError("Compilation failed.")
        except subprocess.CalledProcessError as err:
            raise ModelicaCompilationError("Execution of jmodelica_compile.cmd failed.", sp_msg=err.returncode)        
        finally:
            if os.path.isfile('j_compile.log'):
                with open('j_compile.log', 'r') as f_in:
                    log.info('\n'.join(f_in.readlines()))

        self.compilation_time = time.time() - t_start
        return self.model_is_compiled
    # end of compile_model

    def simulate_model(self):
        """
        Simulate model using current settings

        """
        log = logging.getLogger()
        log.debug("Entered simulate_model")
        # create a .cmd file which will:
        #   1) run %JMODELICAHOME%\\setenv.bat and
        #   2) call a python script to simulate the model

        jmodelica_setenv_path = os.path.join(self.tool_path, 'setenv.bat')

        fmu_name = self.model_name.split('.')[-1] + '.fmu'

        with open(os.path.join('jmodelica_simulate.cmd'), 'wb') as file_out:
            lines = [r':: Use System Python (which should have access to JModelica) to compile the model',
                     r'echo off',
                     r'pushd %~dp0',
                     r'call {0}'.format(jmodelica_setenv_path),
                     r'"{0}" ..\scripts\jmodelica_simulate.py {1}'.format(self.system_python_path, fmu_name),
                     r'popd']
            file_out.write("\n".join(lines))
            
        # set up environment such that the libraries are included
        #my_env = os.environ
        #if self.lib_package_paths:
        #    my_env = self._setup_libs_env_vars()
        if os.path.isfile(self.simulate_failed_file):
            os.remove(self.simulate_failed_file)
        if os.path.isfile('j_simulate.log'):
            os.remove('j_simulate.log')
        t_start = time.time()
        try:
            return_string = subprocess_call(os.path.join('jmodelica_simulate.cmd'), log)
            log.debug(return_string)
            self.result_mat = self.model_name.replace('.', '_') + '_result.txt'
            if os.path.isfile(self.simulate_failed_file):
                raise ModelicaSimulationError("Simulation failed.")
        except subprocess.CalledProcessError as err:
            raise ModelicaSimulationError("Execution of jmodelica_simulate.cmd failed.", sp_msg=err.returncode)
        finally:
            if os.path.isfile('j_simulate.log'):
                with open('j_simulate.log', 'r') as f_in:
                    log.info('\n'.join(f_in.readlines()))

        self.simulation_time = time.time() - t_start
        self.total_time = self.compilation_time + self.simulation_time

        return self.model_did_simulate
    # end of simulate_model

    def change_experiment(self,
                          start_time='0',
                          stop_time='1',
                          n_interval='500',
                          tolerance='1e-5',
                          solver='dassl',
                          increment='',
                          max_fixed_step='',
                          output_format='',
                          variable_filter=''):
        """
        Change the default experiment values.

        """

        log = logging.getLogger()
        log.debug("Entered change_experiment")
        raise NotImplementedError('JModelica class does use this method, use model_config instead.')
    # end of change_experiment

    def change_parameter(self, change_dict):
        """
        Generated a json file of the change_dict to j_parameters.json.
        This is consumed by jmodelica_simulate.py python script.
        """
        log = logging.getLogger()
        log.debug("Entered simulate_model")
        parameter_file = 'j_parameters.json'
        if not os.path.basename(os.getcwd()) == 'CyPhy':
            parameter_file = os.path.join('CyPhy', 'j_parameters.json')
        with open(parameter_file, 'wb') as f_out:
            json.dump(change_dict, f_out)

        log.info('Saved parameters to "{0}".'.format(parameter_file))
    # end of change_parameter