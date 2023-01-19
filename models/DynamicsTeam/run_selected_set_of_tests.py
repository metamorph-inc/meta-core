"""
Generates and executes unit-tests based on the cmd-files in the sub-directories.

"""
import six
__author__ = 'pmeijer'
import os
import xmlrunner
import unittest
import subprocess
import logging
import sys
from logging.handlers import RotatingFileHandler

class ConsoleHandler(logging.StreamHandler):
    """A handler that logs to console in the sensible way.

    StreamHandler can log to *one of* sys.stdout or sys.stderr.

    It is more sensible to log to sys.stdout by default with only error
    (logging.ERROR and above) messages going to sys.stderr. This is how
    ConsoleHandler behaves.
    """

    def __init__(self):
        logging.StreamHandler.__init__(self)
        self.stream = None # reset it; we are not going to use it anyway

    def emit(self, record):
        if record.levelno >= logging.ERROR:
            self.__emit(record, sys.stderr)
        else:
            self.__emit(record, sys.stdout)

    def __emit(self, record, strm):
        self.stream = strm
        logging.StreamHandler.emit(self, record)

    def flush(self):
        # Workaround a bug in logging module
        # See:
        #   http://bugs.python.org/issue6333
        if self.stream and hasattr(self.stream, 'flush') and not self.stream.closed:
            logging.StreamHandler.flush(self)


def instantiate_logger(log_file='debug.log'):
    """
    Creates a logger with two handlers.

    One RotatingFileHandler that writes to the log_file(='debug.log') at level DEBUG.

    One ConsoleHandler that writes to console at level INFO.

    """
    log = logging.getLogger()
    log.setLevel(logging.DEBUG)
    log_formatter = logging.Formatter("%(asctime)s %(levelname)s: %(message)s",
                                      datefmt="%Y-%m-%d %H:%M:%S")

    file_log = RotatingFileHandler(log_file)
    file_log.setFormatter(log_formatter)
    console_log = ConsoleHandler()
    console_log.setFormatter(log_formatter)
    console_log.setLevel(logging.INFO)
    log.addHandler(file_log)
    log.addHandler(console_log)

    return log

cnt_call = 0
cnt_tot = 0
def subprocess_call(command, log):
    """
    Calls the command, if error occurred logging is made of all non-empty returns.
    Reraises the exception putting the formatet message in returncode

    :param command: the command to be executed
    :param log: logger
    """
    global cnt_call
    cnt_call += 1
    print("[{0:3}/{1}]".format(cnt_call, cnt_tot))
    print("About to call : {0}".format(command))
    try:
        return_out = subprocess.check_output(command, stderr=subprocess.STDOUT, shell=True)
        log.info('Subprocess call returned with 0.')
        log.info('console output : \n{0}'.format(return_out))
    except subprocess.CalledProcessError as err:
        print("Failed!")
        log.error(err.cmd)
        log.error(err.output)
        raise err
    print("Success!")
    return return_out


class RunEntryPoints(unittest.TestCase):
    def setUp(self):
        self.root_dir = os.path.dirname(os.path.abspath(__file__))
        self.log = instantiate_logger()
        self.log.info("RootDir : {0}".format(self.root_dir))

    def _execute_cmd_file(self, cmd_file, cmd_dir):
        os.chdir(cmd_dir)
        self.log.info('Switched directory to : {0}'.format(os.getcwd()))
        try:
            subprocess_call(cmd_file, self.log)
            self.assertFalse(os.path.exists('_FAILED.txt'), 'Job failed, generated _FAILD.txt')
        except subprocess.CalledProcessError as err:
            self.assertEqual(err.returncode, 0, 'Subprocess call failed!')
        finally:
            os.chdir(self.root_dir)
            self.log.info('Switched directory back to : {0}'.format(os.getcwd()))

#def test_MSD_PCC_MSD_dymola_CA(self):
#    cmd_dir = r'MSD_PCC\MSD_dymola_CA'
#    cmd_file = 'simulate_dymola.cmd'
#    self._execute_cmd_file(cmd_file, cmd_dir)


def generate_unit_tests_methods(cmd_infos):
    #cmd_infos = []
    #
    #for dir_name, subdirs, files in os.walk('.'):
    #    for file_name in files:
    #        if file_name in cmd_types:
    #            #cmd_infos.append([r'MSD_PCC\MSD_dymola_CA', 'simulate_dymola.cmd'])
    #            cmd_infos.append([dir_name[2:], file_name])
    #            print "Added {0:20} : {1}".format(file_name,dir_name[2:])
    
    lines = []
    lines.append("f_map = {}")
    cnt = 0
    for cmd_info in cmd_infos:
        cnt += 1
        #if cnt > 5:
        #    break
        cmd_dir = cmd_info[0]
        cmd_file = cmd_info[1]
        unit_test_name = cmd_dir.replace(os.path.sep, '_').replace(' ', '_')
        if cmd_file == 'simulate_om.cmd':
            unit_test_name = '{0}_OpenModelica'.format(unit_test_name)
        elif cmd_file == 'simulate_dymola.cmd':
            unit_test_name = '{0}_Dymola'.format(unit_test_name)
        elif cmd_file == 'run_PCC.cmd':
            unit_test_name = '{0}_PCC'.format(unit_test_name)
        elif cmd_file == 'run_parameter_study.cmd':
            unit_test_name = '{0}_ParameterStudy'.format(unit_test_name)
        elif cmd_file == 'run_optimizer.cmd':
            unit_test_name = '{0}_Optimization'.format(unit_test_name)
        lines.append("")
        lines.append("")
        lines.append("def test_{0}(self):".format(unit_test_name))
        lines.append("    cmd_dir = r'{0}'".format(cmd_dir))
        lines.append("    cmd_file = '{0}'".format(cmd_file))
        lines.append("    self._execute_cmd_file(cmd_file, cmd_dir)")
        lines.append("")
        lines.append("f_map['test_{0}'] = test_{0}".format(unit_test_name))
        print('New test created : test_{0}'.format(unit_test_name))

    with open('generated_methods.py', 'wb') as f_out:
        f_out.writelines('\n'.join(lines))

    global cnt_tot
    cnt_tot = cnt
    print("Generated {0} unit_tests".format(cnt))


if __name__ == '__main__':
    import time
    t_start = time.time()
    cmd_infos = []
    cmd_infos.append([r'MSD_PCC\PCC_MSD_dymola_CA', 'run_PCC.cmd'])
    cmd_infos.append([r'MSD_PCC\PCC_MSD_om_CA', 'run_PCC.cmd'])
    cmd_infos.append([r'WaterPump\TestBenches_Test_Delivered_Electric_Pump', 'simulate_dymola.cmd'])
    cmd_infos.append([r'WaterPump\TestBenches_Test_Delivered_Electric_Pump_TransportedFluid', 'simulate_om.cmd'])
    cmd_infos.append([r'MSD_PET\ResponseSurfaceExample', 'run_parameter_study.cmd'])
    cmd_infos.append([r'MSD_PET\COBYLAExample', 'run_optimizer.cmd'])
    cmd_infos.append([r'RISimpleFormula\MinMax_RICircuit_Max', 'simulate_om.cmd'])
    cmd_infos.append([r'RISimpleFormula\MinMax_RICircuit_Min', 'simulate_dymola.cmd'])
    cmd_infos.append([r'DriveLine_v3\FullSpeedForwardMaxSpeed_cfg1', 'run_PCC.cmd'])
    cmd_infos.append([r'DriveLine_v3\Dynamics_FullSpeedForward_cfg1', 'simulate_dymola.cmd'])
    cmd_infos.append([r'RollingWheel\TestBenches_RollingWheelBooleanParameterInTestComponent', 'simulate_dymola.cmd'])
    cmd_infos.append([r'RollingWheel\TestBenches_RollingWheelBooleanParameterInComponent', 'simulate_om.cmd'])

    generate_unit_tests_methods(cmd_infos)
    import generated_methods
    for f_name, f in six.iteritems(generated_methods.f_map):
        setattr(RunEntryPoints, f_name, f)
    print("Running tests... See test_results_test.xml when done...")
    with open('test_results_test.xml', 'w') as f_out:
        f_out.write("<testsuites>")
        suite1 = unittest.TestLoader().loadTestsFromTestCase(RunEntryPoints)
        xmlrunner.XMLTestRunner(f_out).run(suite1)
        f_out.write("</testsuites>")
    print("Execution time : {0:.2f} s.".format(time.time() - t_start))
