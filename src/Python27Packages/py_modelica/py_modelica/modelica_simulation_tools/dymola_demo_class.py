import os
import time
import shutil
import datetime
import subprocess
import logging

from py_modelica.modelica_simulation_tools.tool_base import ToolBase
from py_modelica.exception_classes import ModelicaInstantiationError


class DymolaDemo(ToolBase):
    """
    This class has not been maintained and might not work as expected.

    """
    tool_name = "DymolaDemo"
    MAX_DYMOLA_STEP_TIME = 300  # max time in sec. waiting for status to get updated with new time

    def __init__(self,
                 model_config,
                 dymola_home=r"C:\Program Files (x86)\Dymola 2013\bin64"):
        """
        Constructor for DymolaDemo
        """

        if os.name == 'nt':
            try:
                dymola_reg, version_nbr = self._query_registry('Dymola 2013')
                self.tool_version_nbr = version_nbr
            except Exception:
                dymola_reg = ""

            dymola_default = r"C:\Program Files (x86)\Dymola 2013"

            # Check if given path exists and contains dymola.exe
            if os.path.exists(os.path.join(dymola_home, "dymola.exe")):
                self.tool_path = dymola_home

            # If not found, look if dymola installed at its default location
            elif os.path.exists(os.path.join(dymola_default, "bin64", "dymola.exe")):
                self.tool_path = os.path.join(dymola_default, "bin64")
            elif os.path.exists(os.path.join(dymola_default, "bin", "dymola.exe")):
                self.tool_path = os.path.join(dymola_default, "bin")

            # If not found, look in the path returned from registry
            elif dymola_reg:
                if os.path.exists(os.path.join(dymola_reg, "bin64", "dymola.exe")):
                    self.tool_path = os.path.join(dymola_reg, "bin64")
                elif os.path.exists(os.path.join(dymola_reg, "bin", "dymola.exe")):
                    self.tool_path = os.path.join(dymola_reg, "bin")

            # If not found in any of these cases raise an exception
            else:
                msg = "Dymola path not found at any of: {0} ; {1} ; {2}".format(
                      dymola_home, dymola_default, dymola_reg)
                raise ModelicaInstantiationError(msg)

            try:
                import _winreg as wr
                key = wr.OpenKey(wr.HKEY_LOCAL_MACHINE, r'software\meta', 0, wr.KEY_READ)
                try:
                    self.MAX_DYMOLA_STEP_TIME = wr.QueryValueEx(key, 'MAX_DYMOLA_STEP_TIME')[0]
                    print 'Found MAX_DYMOLA_STEP_TIME in registry, value was {0}'\
                        .format(self.MAX_DYMOLA_STEP_TIME)
                except WindowsError:
                    print 'MAX_DYMOLA_STEP_TIME not set in registry, using default (5min).'
            except WindowsError:
                print 'META-Tools not installed, using default MAX_DYMOLA_STEP_TIME (5min).'

        self._initialize(model_config)
    # end of __init__

    def _query_registry(self, display_name='Dymola 2013'):
        """
        Queries the registry for the installation path of Dymola.

        """
        import _winreg

        base_key = r'SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall'

        uninstall_key = _winreg.OpenKey(_winreg.HKEY_LOCAL_MACHINE, base_key, 0,
                                        _winreg.KEY_ALL_ACCESS | _winreg.KEY_WOW64_32KEY)

        number_of_keys = _winreg.QueryInfoKey(uninstall_key)[0] # 0 means number of sub_keys

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

    def _generate_mos_script(self, parameters=None):
        """
        Internal function for generating a new mos-script called during,
        compile_model, change_experiment, change_parameter.
        The reason it is called during the two latter is that in DymolaDemo a
        change in experiment or parameters requiers a recompilation of the model.

        """
        log = logging.getLogger()
        log.debug("Entered _generate_mos_script")

        with open(self.mos_file_name, 'wb') as file_out:
            lines = []
            lines.append('// Dymola(demo) script file to run a model')
            lines.append('// name of the file or package,')
            lines.append('// which needs to be loaded')
            lines.append('fileName = "' + self.model_file_name + '"')
            lines.append('// full name in the modelica package')
            lines.append('modelName = "' + self.model_name + '"')
            lines.append('// list of parameters')
            if parameters:
                for par in parameters:
                    lines.append('{0}={1}'.format(par, parameters[par]))
            lines.append('// without .mat extension')
            lines.append('resultFile = modelName')

            lines.append('openModel(fileName)')

            lines.append('simulateModel(modelName, resultFile = resultFile,'
                         'startTime={0},stopTime={1},numberOfIntervals={2},tolerance={3}'
            .format(self.experiment['StartTime'],
                    self.experiment['StopTime'] ,
                    self.experiment['NumberOfIntervals'],
                    self.experiment['Tolerance']))
            if self.experiment.has_key('Solver'):
                lines.append(',method="{0}"'.format(self.experiment['Solver']))
            lines.append(')')
            lines.append('exit()')

            file_out.write("\n".join(lines))
            log.debug("Genrated .mos-script at : {0}".format(self.mos_file_name))

    def compile_model(self):
        """
        In DymolaDemo, the model is compiled during simulate_model.
        This method only generates a .mos script that will be executed
        in simulate_model.

        """
        log = logging.getLogger()
        log.debug("Entered compile_model")
        log.info("In DymolaDemo compilation and simulation are jointly executed in simulate_model.")
        # create a directory for the compiled model
        self.working_dir = os.path.join(os.getcwd(),
            'output',
            self.model_name.split('.')[-1],
            self.tool_name)

        if not os.path.exists(self.working_dir):
            os.makedirs(self.working_dir)

        # generate .mos script to compile and simulate the model

        self.mos_file_name = os.path.join(self.model_name + '.mos')
        self._generate_mos_script()

        self.model_is_compiled = True

        return self.model_is_compiled
    # end of compile_model

    def simulate_model(self):
        """
        Simulates model

        """
        log = logging.getLogger()
        log.debug("Entered simulate_model")

        # call the .mos script
        command = "{0} /nowindow {1}".format(os.path.join(self.tool_path, 'Dymola.exe'),
                                             self.mos_file_name)
        log.debug('About to call: {0}'.format(command))
        t_1 = time.time()
        return_code = subprocess.call(command)
        if return_code != 0:
            log.error('Command faile to execute {0}'.format(command))
            return False
        else:
            log.debug('Call successful!')
        #os.remove(self.mos_file_name)
        t_2 = time.time()
        self.total_time = t_2-t_1

        self.result_mat = self.model_name + '.mat'
        # copy the compiled model into the directory
        files_to_move = ['buildlog.txt',
                         'dsfinal.txt',
                         'dsin.txt',
                         'dslog.txt',
                         'dsmodel.c',
                         'dymosim.exp',
                         'dymosim.lib',
                         'dymosim.exe',
                         'request',
                         'status',
                         self.result_mat]

        for this_file in files_to_move:
            dst_file_name = os.path.join(self.working_dir, this_file)
            if os.path.exists(dst_file_name):
                # remove dst file if it already exists
                os.remove(dst_file_name)

            this_file = os.path.join(self.mo_dir, this_file)
            if os.path.exists(this_file):
                shutil.move(this_file, self.working_dir)
        self.result_mat = os.path.join(self.working_dir, self.result_mat)

        os.chdir(self.root_dir)

        return True
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
        Puts the given experiment data from the parameters into,
        self.experiment dictionary. Then calls _generate_mos_script
        where the data is transfered into a new .most script that can
        be called from simulate_model.

        """

        log = logging.getLogger()
        log.debug("Entered change_experiment")

        self.experiment.clear()
        self.experiment['StartTime'] = start_time
        self.experiment['StopTime'] = stop_time
        self.experiment['NumberOfIntervals'] = n_interval
        self.experiment['Tolerance'] = tolerance

        # dictionary of accepted solvers
        dymola_solvers = ['deabm',
                          'lsode1',
                          'lsode2',
                          'lsodar',
                          'dopri5',
                          'dopri8',
                          'grk4t',
                          'dassl',
                          'odassl',
                          'mexx',
                          'euler',
                          'rkfix2',
                          'rkfix3',
                          'rkfix4']

        if solver in dymola_solvers:
            self.experiment['Solver'] = solver
        else:
            log.warning('Given solver ({0}) was not found, using dassl instead'.format(solver))
            self.experiment['Solver'] = "dassl"

        self._generate_mos_script()

        # generate a new date_time for saving result
        self.date_time = '{0}'.format(datetime.datetime.today())

        log.info("Experiment has been change to: {0}".format(self.experiment))

        return True
    # end of change_experiment

    def change_parameter(self, change_dict):
        """

        """

        log = logging.getLogger()
        log.debug("Entered change_parameter")

        self._generate_mos_script(parameters = change_dict)

        # generate a new date_time for saving result
        self.date_time = '{0}'.format(datetime.datetime.today())

        log.debug("Parameters changed : {0}".format(change_dict))

        return True
    # end of change_parameter




