import os
import sys
import logging

ROOT_DIR = os.getcwd()  # make sure to change back to this when exiting

try:
    import py_modelica as pym
    print('Found py_modelica in virtual python environment')
except ImportError as err:
    print(err.message)
    print('Use META virtual python environment')


class TestBenchExecutionError(Exception):
    """
    Class representing 'expected' errors.
    """
    def __init__(self, message):
        super(TestBenchExecutionError, self).__init__(message)
        self.message = message

from optparse import OptionParser

parser = OptionParser()
parser.add_option("-d", "--driver", dest="driver",
                  help="Name of the driver to be executed")
parser.add_option("-v", "--validation_driver", dest="validation_driver",
                  default=None, help="Additional, if any, surrogate validation driver.")


def _exit_on_failure(error_message):
    """ Function for exiting after a failure.
    """
    os.chdir(ROOT_DIR)
    import traceback
    with open('_FAILED.txt', 'wb') as f_out:
        f_out.writelines(error_message)
        f_out.writelines(traceback.format_exc())
        f_out.writelines('See log/PET_debug.log for more info.')
    pym.update_analysis_status('FAILED', error_message)
    log = logging.getLogger()
    log.error(error_message)
    log.error("Exception was raised : {0}".format(traceback.format_exc()))
    sys.exit(1)


def main():
    # Setup the logger.
    if not os.path.isdir('log'):
        os.mkdir('log')
    log = pym.instantiate_logger(os.path.join('log', 'PET_debug.log'))

    # Read and log options from command line.
    (options, args) = parser.parse_args()
    driver_name = options.driver
    validation_driver = options.validation_driver
    log.info('driver : {}, validation_driver[optional] : {}'.format(driver_name, validation_driver))

    # Import driver and exception class.
    driver = __import__(driver_name)

    # Run the driver.
    try:
        driver.main()
        if validation_driver:
            log.info(' ---====== Starting Surrogate Validation ======---')
            __import__(validation_driver).main()
        pym.check_for_invalid_numbers()
        pym.update_analysis_status('OK')
    except TestBenchExecutionError as err:
        _exit_on_failure(err.message)


if __name__ == '__main__':
    try:
        main()
    except Exception as err:
        try:
            _exit_on_failure('{0} : (Not a caught TestBenchExecutionError)'.format(err.message))
        except Exception:
            sys.exit(13)  # Just in case something should fail in the exit function