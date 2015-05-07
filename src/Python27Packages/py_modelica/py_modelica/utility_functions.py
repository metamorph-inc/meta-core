import os
import sys
import json
import subprocess
import logging
import time
from logging.handlers import RotatingFileHandler
from optparse import OptionParser


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


def flush_handlers():
    """
    Flushes the loggers to get console printouts during execution.
    """

    log = logging.getLogger()
    for handler in log.handlers:
        handler.flush()


def run_post_scripts(result_mat, log):
    """
    Runs all python scripts the PostProcessing-dir as subprocess calls with
    the path of the result_mat file as argument.
    """

    pp_dir = os.path.join(os.getcwd(), "PostProcessing")
    python_exec = sys.executable
    t_post = 0
    if os.path.exists(pp_dir):
        t_1 = time.time()
        for pp_file in os.listdir(pp_dir):
            # META-3580
            if pp_file.endswith(".py") and not pp_file == '__init__.py':
                post_script_cmd = '"{0}" "{1}" "{2}"'\
                    .format(python_exec, os.path.join(os.path.basename(pp_dir), pp_file), result_mat)
                log.info("Running post process script.. {0}".format(pp_file))
                subprocess_call(post_script_cmd, log)
                log.info("Post processing finished!")
        t_post = time.time() - t_1

    return t_post


def write_out_tool_statistics(tool, t_mat, t_post, json_file='tool_stat.json'):
    """
    Writes out statistics about the run to json_file
    """
    tool_stat = {"PCC_times": {},
                 "tool_name": tool.tool_name,
                 "version": tool.tool_version,
                 "version_number": tool.tool_version_nbr}

    mat_file = tool.result_mat
    size = 0
    if os.path.exists(mat_file):
        size = int(os.path.getsize(mat_file))
    tool_stat["mat_file_size"] = size
    try:
        mo_file = tool.model_name.split('.')[1] + ".mo"
    except IndexError:
        mo_file = tool.model_name + ".mo"

    size = 0
    if os.path.exists(mo_file):
        size = int(os.path.getsize(mo_file))
    tool_stat["mo_file_size"] = size

    tool_stat["exec_times"] = {
        "compilation_time": tool.compilation_time,
        "translation_time": tool.translation_time,
        "make_time": tool.make_time,
        "simulation_time": tool.simulation_time,
        "total_time": tool.total_time,
        "load_mat_time": t_mat,
        "post_scripts_time": t_post}

    with open(json_file, 'wb') as f_out:
        json.dump(tool_stat, f_out, indent=4)


def subprocess_call(command, log, my_env=None):
    """
    Calls the command, if error occurred logging is made of all non-empty returns.
    Reraises the exception putting the formatet message in returncode

    :param command: the command to be executed
    :param log: logger
    :param my_env: dictionary of environment-variables, None -> uses the default windows
    """
    log.info("About to call : {0}".format(command))
    try:
        if my_env:
            return_out = subprocess.check_output(command, stderr=subprocess.STDOUT, env=my_env, shell=True)
        else:
            return_out = subprocess.check_output(command, stderr=subprocess.STDOUT, shell=True)
        log.info('Subprocess call returned with 0.')
        log.info('console output : \n{0}'.format(return_out))
    except subprocess.CalledProcessError as err:
        msg = "Subprocess call failed!"
        msg += "\n  command       : {0}".format(err.cmd)
        msg += "\n  return-code   : {0}".format(err.returncode)
        if err.output:
            msg += "\n  console output: \n\n{0}".format(err.output)
        if err.message:
            msg += "\n  error message : {0}".format(err.message)
        log.error(msg)
        raise subprocess.CalledProcessError(returncode=msg, cmd=err.cmd)

    return return_out


# unused stuff
def get_simscript_opts_and_args():
    """
    Deals with optional arguments passed to simscript.py
    """
    usage = "usage: %prog [options] tool_name"
    parser = OptionParser(usage=usage)
    parser.add_option("-s", "--stand_alone", action="store_true", default=False,
        help='If True -> report_file, filter_file, limits_file all set to "". [default: False]')
    parser.add_option("-m", "--result_mat_file", default="",
        help='Name without .mat extension for output result .mat-file (OpenModelica will append _res to this name)')
    parser.add_option("-c", "--model_config_file",
        help='Name of model_config .json file [default: %default]', default="model_config.json")
    parser.add_option("-d", "--debug_file", default="debug.log",
        help='Name of debug log file [default: %default]')
    parser.add_option("-r","--report_file", default="testbench_manifest.json",
        help='Name of report .json file. If set to "" no report file will be used. [default: %default]')
    parser.add_option("-f","--filter_file", default="filter.json",
        help='Name of filter .json file. If set to "" no filter will not be looked for. [default: %default]')
    parser.add_option("-l","--limits_file", default="limits.json",
        help='Name of limits .json file. If set to "" limits will not be checked. [default: %default]')

    (opts, args) = parser.parse_args()
    if opts.stand_alone:
        opts.report_file = ""
        opts.filter_file = ""
        opts.limits_file = ""

    return opts, args
