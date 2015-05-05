import os
import datetime
import logging
from py_modelica.exception_classes import ModelicaInstantiationError

from abc import ABCMeta, abstractmethod


class ToolBase:
    __metaclass__ = ABCMeta

    tool_name = ''
    tool_version = ''
    tool_version_nbr = ''

    model_config = None

    date_time = ''

    ## instance variables
    tool_path = ''              # path to the bin folder of the tool

    model_file_name = ''        # file that needs to be loaded
    model_name = ''             # name of the model in the loaded packages

    msl_version = ''            # version of Modelica Standard Library

    mos_file_name = ''          # modelica script files for compiling the model

    result_mat = ''             # contains the latest simulation results
    base_result_mat = ''        # contains the expected simulation results

    working_dir = ''            # contains the temporary files and executables
    root_dir = ''
    mo_dir = ''                 # contains the modelica file, (package or model)

    output_dir = ''             # relative or absolute

    variable_filter = []        # list of names of variables to save/load to/from mat-file

    experiment = {}             # dictionary with StartTime, StopTime, Tolerance,
                                # NumberOfIntervals, Interval and Algorithm.

    model_is_compiled = False   # flag for telling if the model was compiled
    model_did_simulate = False  # flag for telling if the model has been simulated

    lib_package_paths = []      # paths to additional packages
    lib_package_names = []      # names of additional packages

    max_simulation_time = 43200 # (=12h) time threshold before simulation is aborted

    ## Variables with execution statistics
    compilation_time = -1
    translation_time = -1
    make_time = -1
    simulation_time = -1
    total_time = -1

    def _initialize(self,
                    model_config):
        """
        Creates a new instance of a modelica simulation.

        dictionary : model_config

            Mandatory Keys : 'model_name' (str), 'model_file_name' (str)

            Optional Keys : 'MSL_version' (str), 'variable_filter' ([str]),
                            'result_file' (str), 'experiment' ({str})
        """

        print ' --- =====  See debug.log for error/debug messages ===== --- \n'
        print ' in {0}'.format(os.getcwd())

        # create a logger, (will only be written to if no other logger defined 'higher' up)
        logging.basicConfig(filename="debug.log",
                            format="%(asctime)s %(levelname)s: %(message)s",
                            datefmt="%Y-%m-%d %H:%M:%S")

        log = logging.getLogger()
        # always use highest level of debugging
        log.setLevel(logging.DEBUG)

        log.debug(" --- ==== ******************************* ==== ---")
        log.info(" --- ==== ******* New Run Started ******* ==== ---")
        self.date_time = '{0}'.format(datetime.datetime.today())
        log.debug(" --- ==== * {0} ** ==== ---".format(self.date_time))
        log.debug(" --- ==== ******************************* ==== ---")
        log.debug("Entered _initialize")
        log.info("tool_name : {0}".format(self.tool_name))
        log.info("tool_path : {0}".format(self.tool_path))
        self.root_dir = os.getcwd()

        self.model_config = model_config

        # Mandatory keys in dictionary
        try:
            model_file_name = self.model_config['model_file_name']
            if model_file_name == "":
                self.model_file_name = ""
                log.info("No model_file name given, assumes model is in Modelica Standard Library")
            else:
                self.model_file_name = os.path.normpath(os.path.join(os.getcwd(), model_file_name))
                self.mo_dir = os.path.dirname(self.model_file_name)
                log.info("mo_dir : {}".format(self.mo_dir))
                log.info("model_file_name : {0}".format(self.model_file_name))

            model_name = self.model_config['model_name']
            if model_name == "":
                base_name = os.path.basename(model_file_name)
                self.model_name = os.path.splitext(base_name)[0]
                log.info("No model_name given, uses model_file_name without .mo")
            else:
                self.model_name = model_name
                log.info("model_name : {0}".format(self.model_name))
        except KeyError as err:
            raise ModelicaInstantiationError("Mandatory key missing in model_config : {0}".format(err.message))

        # optional keys in dictionary
        if 'MSL_version' in model_config:
            self.msl_version = self.model_config['MSL_version']
        else:
            self.msl_version = "3.2"
        log.info("msl_version : {0}".format(self.msl_version))

        if 'experiment' in model_config:
            self.experiment = dict(
                StartTime=model_config['experiment']['StartTime'],
                StopTime=model_config['experiment']['StopTime'],
                NumberOfIntervals=model_config['experiment']['NumberOfIntervals'],
                Tolerance=model_config['experiment']['Tolerance'])
            # Algorithm
            if 'Algorithm' in model_config['experiment']:
                if self.tool_name.startswith('Dymola'):
                    self.experiment.update({'Algorithm':
                                            self.model_config['experiment']['Algorithm']['Dymola']})
                elif self.tool_name == 'OpenModelica':
                    self.experiment.update({'Algorithm':
                                            self.model_config['experiment']['Algorithm']['OpenModelica']})
                elif self.tool_name == 'JModelica':
                    self.experiment.update({'Algorithm':
                                            self.model_config['experiment']['Algorithm']['JModelica']})
            else:  # py_modelica 12.09
                self.experiment.update({'Algorithm': 'dassl'})
            # Interval
            if 'IntervalMethod' in model_config['experiment']:
                if model_config['experiment']['IntervalMethod'] == 'Interval':
                    self.experiment.update({"NumberOfIntervals": "0"})
                    self.experiment.update({"Interval": model_config['experiment']['Interval']})
                else:
                    self.experiment.update({"NumberOfIntervals":
                                            model_config['experiment']['NumberOfIntervals']})
                    self.experiment.update({"Interval": "0"})
            else:  # py_modelica 12.09
                self.experiment.update({"NumberOfIntervals":
                                        model_config['experiment']['NumberOfIntervals']})
                self.experiment.update({"Interval": "0"})
        else:
            self.experiment = dict(StartTime='0',
                                   StopTime='1',
                                   NumberOfIntervals='500',
                                   Interval='0',
                                   Tolerance='1e-5',
                                   Algorithm='dassl')
            log.info("No experiment data given, default values will be used...")
        log.info("Experiment settings : {0}".format(self.experiment))

        if 'lib_package_paths' in model_config:
            for lib_path in self.model_config['lib_package_paths']:
                if lib_path:
                    self.lib_package_paths.append(str(lib_path))

        if 'lib_package_names' in model_config:
            for lib_name in self.model_config['lib_package_names']:
                if lib_name:
                    self.lib_package_names.append(lib_name)

        if os.name == 'nt':
            try:
                import _winreg as wr
                key = wr.OpenKey(wr.HKEY_LOCAL_MACHINE, r'software\meta', 0, wr.KEY_READ)
                try:
                    self.max_simulation_time = wr.QueryValueEx(key, 'MAX_SIMULATION_TIME')[0]
                    print 'Found MAX_SIMULATION_TIME in registry, value was {0} (={1:.1f} h).'\
                        .format(self.max_simulation_time, float(self.max_simulation_time)/3600)
                except WindowsError:
                    print 'MAX_SIMULATION_TIME not set in registry, using default {0} (={1:.1f} h).'\
                        .format(self.max_simulation_time, float(self.max_simulation_time)/3600)
            except WindowsError:
                print 'META-Tools not installed, using default max_simulation_time at {0} (={1:.1f} h).'\
                    .format(self.max_simulation_time, float(self.max_simulation_time)/3600)

    # end of __initialize__

    @abstractmethod
    def compile_model(self):
        return bool

    @abstractmethod
    def simulate_model(self):
        return bool

    @abstractmethod
    def change_experiment(self,
                          start_time='0',
                          stop_time='1',
                          increment='',
                          n_interval='500',
                          tolerance='1e-5',
                          max_fixed_step='',
                          solver='dassl',
                          output_format='',
                          variable_filter=''):
        return bool

    @abstractmethod
    def change_parameter(self, change_dict):
        return bool