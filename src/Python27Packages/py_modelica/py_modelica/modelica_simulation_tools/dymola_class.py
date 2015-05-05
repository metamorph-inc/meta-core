import os
import time
import shutil
import subprocess
import logging
from scipy.io import savemat
from scipy.io import loadmat

from py_modelica.modelica_simulation_tools.tool_base import ToolBase
from py_modelica.utility_functions import subprocess_call
from py_modelica.exception_classes import ModelicaInstantiationError, ModelicaCompilationError, \
    ModelicaSimulationError


class Dymola(ToolBase):
    """
    Class with methods for simulating the model described by model_config using Dymola.
    """
    tool_name = "Dymola"
    input_mat = ''              # input mat file (parameter values + experiment)
    dymosim = ''                # path to dymosim.exe
    MAX_DYMOLA_STEP_TIME = 300  # max time in sec. waiting for status to get updated with new time

    def __init__(self,
                 model_config,
                 dymola_bin_dir=''):
        """
        Constructor for Dymola Class
        """
        if dymola_bin_dir:
            # This part will not happen from CyPhy.
            self.tool_path = dymola_bin_dir
        elif os.name == 'nt':
            if model_config['experiment']['ToolSelection'] == 'Dymola_2013':
                try:
                    self.tool_path = self._get_dymola_bin_path('Dymola 2013')
                except IOError as err:
                    raise ModelicaInstantiationError(err.message + ' If using CyPhy please consider choosing Dymola' +
                                                     '(latest) in the SolverSettings.')
            elif model_config['experiment']['ToolSelection'] == 'Dymola_2014':
                try:
                    self.tool_path = self._get_dymola_bin_path('Dymola 2014')
                except IOError as err:
                    raise ModelicaInstantiationError(err.message + ' If using CyPhy please consider choosing Dymola' +
                                                     '(latest) in the SolverSettings.')
            else:
                try:
                    self.tool_path = self._get_dymola_bin_path('Dymola 2015')
                except IOError as err:
                    print err.message
                    print 'Looking for Dymola 2014 FD01...'
                    try:
                        self.tool_path = self._get_dymola_bin_path('Dymola 2014 FD01')
                    except IOError as err:
                        print err.message
                        print 'Looking for Dymola 2014...'
                        try:
                            self.tool_path = self._get_dymola_bin_path('Dymola 2014')
                        except IOError as err:
                            print err.message
                            print 'Looking for Dymola 2013...'
                            try:
                                self.tool_path = self._get_dymola_bin_path('Dymola 2013')
                            except IOError as err:
                                print err.message
                                raise ModelicaInstantiationError('Could not find any dymola installation amongst: ' +
                                                                 '2015, 2014 FD01, 2014 and 2013.')
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

    def _get_dymola_bin_path(self, dymola_version='Dymola 2013'):
        """
        Looks for given dymola version in default installation dir and if not found looks for the installation dir
        present in the registry. If all fails, raises an IOError.
        """
        dymola_home = try_get_dymola_bin(os.path.join(r"C:\Program Files (x86)", dymola_version))
        if not dymola_home:
            try:
                print 'Did not find {0} in default location, will look for registry key.'.format(dymola_version)
                dymola_reg, version_nbr = self._query_registry(dymola_version)
                self.tool_version_nbr = version_nbr
                if os.path.isdir(dymola_reg):
                    dymola_home = try_get_dymola_bin(dymola_reg)
                    if not dymola_home:
                        raise IOError('Dymola home at {0} did not contain a bin directory.'.format(dymola_reg))
                else:
                    raise IOError('{0} registry did not contain existing path.'.format(dymola_version))
            except Exception:
                raise IOError('There were problems finding {0} in the registry.'.format(dymola_version))

        return dymola_home

    def _query_registry(self, display_name='Dymola 2013'):
        """
        Queries the registry for the installation path of Dymola.

        """
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

    def _write_mos_script(self, log):
        """
        Writes out .mos-script for translation of the modelica model
        and compilation of generated c-code into executable

        :param log: logger to write to
        """
        log.debug("entered _write_mos_script")

        with open(self.mos_file_name, 'wb') as file_out:
            lines = ['// Dymola script file to run a model',
                     'openModel("{0}")'.format(self.model_file_name),
                     'Advanced.TranslationInCommandLog = true',
                     'translateModel("{0}")'.format(self.model_name),
                     'savelog("translationLog.txt")',
                     'v_nbr = realString(DymolaVersionNumber())',
                     'writeToFile("d_v.txt","", DymolaVersion() + ":" + v_nbr)',
                     'exit()']
            file_out.write("\n".join(lines))
    # end of _write_mos_script

    def _setup_libs_env_vars(self):
        """
        Function adds given library paths to environment-variable MODELICAPATH.
        Which is updated in os.environ and returned.

        """

        log = logging.getLogger()
        log.debug('Entered _setup_libs_env_vars')

        my_env = os.environ
        lib_paths = ""

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

    def _translate_and_compile(self, log, my_env):
        """
        Translate modelica code into c-code and compile that
        code into executable

        """
        log.debug("Entered _translate_and_compile")
        os.chdir(self.mo_dir)

        command = ''
        if os.name == 'nt':
            command = '"{0}" /nowindow {1}'.format(os.path.join(self.tool_path, 'Dymola.exe'),
                                                   self.mos_file_name)
            my_env['PATH'] = my_env['PATH'].replace('"', '') # META-1507 Dymola may hang with a quote in the %PATH%
        elif os.name == 'posix':
            command = 'dymola /nowindow {0}'.format(self.mos_file_name)

        t_stamp = time.time()
        try:
            return_string = subprocess_call(command, log, my_env)
        except subprocess.CalledProcessError as err:
            raise ModelicaCompilationError("Execution of the mos-script failed.", sp_msg=err.returncode)
        self.compilation_time = time.time() - t_stamp

        # setup name for executable
        if os.name == 'nt':
            self.dymosim = 'dymosim.exe'
        elif os.name == 'posix':
            self.dymosim = './dymosim'
            my_env.update({"LD_LIBRARY_PATH": "/opt/dymola/bin/lib"})

        try:
            with open('translationLog.txt', 'r') as f_in:
                translation_log = f_in.read()
        except IOError:
            translation_log = 'Warning, no translationLog.txt!'

        # make sure that dymosim(.exe) does exist
        if not os.path.exists(self.dymosim):
            msg = 'The sub process call with command "{0}" returned with 0, but {1} does not '\
                  'exist.'.format(command, self.dymosim)
            log.error(translation_log)
            raise ModelicaCompilationError(msg, sp_msg=return_string)

        log.info(translation_log)
        self.model_is_compiled = True
        #os.remove(self.mos_file_name)

        # N.B. this is not used and thus not maintained
        if not self.working_dir == self.mo_dir:
            files_to_move = ['buildlog.txt',
                             'dsfinal.txt',
                             'dsin.txt',
                             'dslog.txt',
                             'dsmodel.c',
                             'dymosim.exp',
                             'dymosim.lib',
                             'dymosim.exe',
                             'request',
                             'status']
            for this_file in files_to_move:
                dst_file_name = os.path.join(self.working_dir, this_file)
                if os.path.exists(dst_file_name):
                    # remove dst file if it already exists
                    os.remove(dst_file_name)

                this_file = os.path.join(self.mo_dir, this_file)
                if os.path.exists(this_file):
                    shutil.move(this_file, self.working_dir)

        os.chdir(self.working_dir)
    # end of _translate_and_compile

    def _generate_input_mat_file(self, log, my_env):
        """
        Generates an input file for dymosim(.exe) containing simulation settings

        """

        log.debug("Entered _generate_input_mat_file")

        self.input_mat = 'dsin.mat'

        # dymosim.exe -ib dsin.mat
        command = '{0} -ib {1}'.format(self.dymosim, self.input_mat)
        try:
            subprocess_call(command, log, my_env)
        except subprocess.CalledProcessError as err:
            raise ModelicaCompilationError("Could not generate dsin.mat", sp_msg=err.returncode)

    def compile_model(self):
        """
        Compiles model

        """
        log = logging.getLogger()
        log.debug("Entered compile_model")

        # create a directory for the compiled model
        self.working_dir = os.path.normpath(os.path.join(os.getcwd(), self.output_dir))
        log.debug("Working dir : {0}".format(self.working_dir))
        # create a directory for the compiled model
        if not os.path.exists(self.working_dir):
            os.makedirs(self.working_dir)

        # generate .mos script to compile the model
        if not self.result_mat:
            self.result_mat = '{0}.mat'.format(self.model_name.split('.')[-1])
            log.info('Result mat-file name not given using default : {0}'.format(self.result_mat))
        else:
            log.info('Result mat-file name set to : {0}'.format(self.result_mat))
        self.mos_file_name = os.path.join('dymola_sim.mos')
        self._write_mos_script(log)

        # set up environment such that the libraries are included
        my_env = os.environ
        if self.lib_package_paths:
            my_env = self._setup_libs_env_vars()
                
        # call the .mos script, i.e. translate and compile model into executable
        self._translate_and_compile(log, my_env)

        # get version number
        with open('d_v.txt', 'r') as file_in:
            version = file_in.readlines()[2].split(':')
        os.remove('d_v.txt')
        self.tool_version = version[0]
        if not self.tool_version_nbr:
            self.tool_version_nbr = version[1].strip()

        # generate a .mat input file
        self._generate_input_mat_file(log, my_env)

        # change experiment if such is defined
        if self.experiment:
            self.change_experiment(
                start_time=self.experiment['StartTime'],
                stop_time=self.experiment['StopTime'],
                n_interval=self.experiment['NumberOfIntervals'],
                tolerance=self.experiment['Tolerance'],
                solver=self.experiment['Algorithm'],
                increment=self.experiment['Interval'])
        else:
            self.change_experiment()

        # update parameters if defined
        if 'parameters' in self.model_config:
            self.change_parameter(self.model_config['parameters'])

        #os.chdir(self.root_dir)

        return self.model_is_compiled
    # end of compile_model

    def simulate_model(self):
        """
        Simulate compiled model with default solver dassl

        """
        log = logging.getLogger()
        log.debug("Entered simulate_model")

        # make sure that the model indeed has been compiled
        if not self.model_is_compiled:
            log.error('The model was never compiled!')
            return False

        # change current directory to working directory
        os.chdir(self.working_dir)

        # dymosim(.exe) -s dsin.mat res33.mat
        command = [self.dymosim, '-s', self.input_mat, self.result_mat]

        status_file = 'status'
        if os.name == 'posix':
            status_file = 'status.'

        # run the simulation
        if os.path.exists(status_file):
            os.remove(status_file)
        t_stamp = time.time()

        sim_time = -1.0
        cnt = 0         # Counter to check how long it takes for the system time to be updated
        init_cnt = 0    # Counter to check how long the initial time-step takes
        total_cnt = 0   # Counter to check to total elapsed simulation time

        dymola_stdout = open('dymola_stdout.txt', 'w')
        sim_process = subprocess.Popen(command, stdout=dymola_stdout, stderr=dymola_stdout)
        log.debug('Dymosim sub-process opened.')
        while sim_process.poll() is None:
            time.sleep(1)
            total_cnt += 1
            if total_cnt > self.max_simulation_time:
                sim_process.kill()
                raise ModelicaSimulationError("Dymola simulation up to t={0}, took more than {1:.1f} hours. "
                                              "Simulation killed.".format(sim_time,
                                                                          float(self.max_simulation_time)/3600))
            try:
                with open(status_file, 'r') as f_in:
                    sim_status = f_in.readline()
                    init_cnt = -1
                time_index = sim_status.find('Time=')
                if time_index > 0:
                    new_sim_time = float(sim_status[time_index + 5:].strip())
                    if not new_sim_time > sim_time:
                        cnt += 1
                    else:
                        cnt = 0
                        log.info("Simulating at t={0}".format(new_sim_time))
                    sim_time = new_sim_time
                    if cnt == self.MAX_DYMOLA_STEP_TIME:
                        #return_string, err = sim_process.communicate()
                        sim_process.kill()
                        raise ModelicaSimulationError("Dymola simulation at t={0} was not "
                                                      "progressing for {1} s. Simulation killed."
                                                      .format(sim_time,
                                                              self.MAX_DYMOLA_STEP_TIME))
            except IOError:
                if init_cnt == -1:
                    # it was initialized since status existed at some point
                    log.debug('The status file has existed, but been removed.')
                else:
                    init_cnt += 1

                if init_cnt == self.MAX_DYMOLA_STEP_TIME:
                    sim_process.kill()
                    raise ModelicaSimulationError("Dymola simulation could not progress at "
                                                  "initialization. Simulation was killed after "
                                                  "{0} s.".format(self.MAX_DYMOLA_STEP_TIME))

        self.simulation_time = time.time() - t_stamp
        self.total_time = self.compilation_time + self.simulation_time

        dymola_stdout.close()
        with open('dymola_stdout.txt', 'r') as f_in:
            return_string = '\n'.join(f_in.readlines())
            log.info(return_string)
            if 'Integration terminated successfully' in return_string:
                self.model_did_simulate = True
                log.info('Dymola simulation was successful.')
            else:
                raise ModelicaSimulationError("Simulation aborted by Dymola.", return_string)

        os.chdir(self.root_dir)

        return self.model_did_simulate
    # end of simulate_model

    def change_experiment(self,
                          start_time='0',
                          stop_time='1',
                          n_interval='500',
                          tolerance='1e-5',
                          solver='dassl',
                          increment='0',
                          max_fixed_step='0',
                          output_format='',
                          variable_filter=''):
        """
        Change the default experiment values.

        """
        log = logging.getLogger()
        log.debug("Entered change_experiment")
        os.chdir(self.working_dir)

        data = loadmat(self.input_mat, struct_as_record=False)

        # dictionary of accepted solvers
        dymola_solvers = dict(deabm=1, lsode1=2, lsode2=3, lsodar=4, dopri5=5, dopri8=6, grk4t=7,
                              dassl=8, odassl=9, mexx=10, euler=11, rkfix2=12, rkfix3=13, rkfix4=14,

                              radau=15, esdirk23a=16, esdirk34a=17, esdirk45a=18, dopri45=19,
                              dopri853=21, sdirk34hw=24, cerk23=26, cerk34=27, cerk45=28)

        algorithm_id = 8  # default dassl solver

        if solver in dymola_solvers:
            algorithm_id = dymola_solvers[solver]
        else:
            log.warning('Given solver ({0}) was not found, using dassl instead'.format(solver))

        # StartTime    Time at which integration starts
        #              (and linearization and trimming time)
        if start_time:
            data['experiment'][0] = float(start_time)

        # StopTime     Time at which integration stops
        if stop_time:
            data['experiment'][1] = float(stop_time)

        # Increment    Communication step size, if > 0
        if increment:
            data['experiment'][2] = float(increment)

        # nInterval    Number of communication intervals, if > 0
        if n_interval:
            data['experiment'][3] = int(n_interval)

        # Tolerance    Relative precision of signals for
        #              simulation, linearization and trimming
        if tolerance:
            data['experiment'][4] = float(tolerance)

        # MaxFixedStep Maximum step size of fixed step size
        #              integrators, if > 0.0
        if max_fixed_step:
            data['experiment'][5] = float(max_fixed_step)

        # Algorithm    Integration algorithm as integer (1...28)
        #
        #             | model|       |        | dense | state |
        # Algorithm   | typ  | stiff | order  | output| event |
        # ------------+------+-------+--------+-------+-------+
        #  1 | deabm  |  ode |   no  |  1-12  |  yes  |   no  |
        #  2 | lsode1 |  ode |   no  |  1-12  |  yes  |   no  |
        #  3 | lsode2 |  ode |  yes  |  1-5   |  yes  |   no  |
        #  4 | lsodar |  ode |  both |1-12,1-5|  yes  |  yes  |
        #  5 | dopri5 |  ode |   no  |   5    |   no  |   no  |
        #  6 | dopri8 |  ode |   no  |   8    |   no  |   no  |
        #  7 | grk4t  |  ode |  yes  |   4    |   no  |   no  |
        #  8 | dassl  |  dae |  yes  |  1-5   |  yes  |  yes  |
        #  9 | odassl | hdae |  yes  |  1-5   |  yes  |  yes  |
        # 10 | mexx   | hdae |   no  |  2-24  |   no  |   no  |
        # 11 | euler  |  ode |   no  |   1    |   no  |  yes  |
        # 12 | rkfix2 |  ode |   no  |   2    |   no  |  yes  |
        # 13 | rkfix3 |  ode |   no  |   3    |   no  |  yes  |
        # 14 | rkfix4 |  ode |   no  |   4    |   no  |  yes  |
        #>=14| others |  ode |yes/no |  2-5   |   yes |  yes  |
        # ---+--------+------+-------+--------+-------+-------+
        # euler and rkfix have fixed stepsize
        data['experiment'][6] = algorithm_id

        # make sure the indicator signals and state-derivatives are saved
        data['settings'][2] = 1
        data['settings'][5] = 1
        data['settings'][7] = 1

        # save the modified experiment in the a Dymola compatible format
        savemat(self.input_mat, data, format='4')

        os.chdir(self.root_dir)
    # end of change_experiment

    def change_parameter(self, change_dict):
        """
        Change the parameter values in the model.

        """
        log = logging.getLogger()
        log.debug("Entered change_parameter")
        os.chdir(self.working_dir)

        # create a string array of names
        names = []
        data = loadmat(self.input_mat, struct_as_record=False)
        for name in data['initialName']:
            names.append(str(name).rstrip())

        # overwrite default settings
        for key, value in change_dict.items():
            if key in names:
                # update its value
                data['initialValue'][names.index(key)][1] = value
            else:
                log.warning('{0} was not found'.format(key))

        # save the updated data structure as .mat file
        # Dymola requires MatLab format v4
        savemat(self.input_mat, data, format='4')

        os.chdir(self.root_dir)
    # end of change_parameter


def try_get_dymola_bin(installation_path):
    """
    Attempts to find the dymola executable for given installation dir, where 64-bit has highest priority.
    If not found None will be returned.
    """
    dymola_exe = None
    if os.path.exists(os.path.join(installation_path, "bin64", "dymola.exe")):
        dymola_exe = os.path.join(installation_path, "bin64")
    elif os.path.exists(os.path.join(installation_path, "bin", "dymola.exe")):
        dymola_exe = os.path.join(installation_path, "bin")

    return dymola_exe