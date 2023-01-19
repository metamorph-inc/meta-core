 
#!/usr/bin/python
# ---------------------------------------------------
# Auto generated from simulate.tt
# ---------------------------------------------------
import os
import sys
import json
import logging
import subprocess
import time

ROOT_DIR = os.getcwd()  # make sure to change back to this when exiting

try:
    import py_modelica as pym
    print('Found py_modelica in virtual python environment')
except ImportError as err:
    print(err.message)
    print('Use META virtual python environment')

from optparse import OptionParser

parser = OptionParser()
parser.add_option("-t", "--tool", dest="tool",
                  help="Name of the tool like Dymola OpenModelica",
                  default="Dymola")

def _exit_on_failure(error_message):
    """ Function for exiting after a failure.
    """
    os.chdir(ROOT_DIR)
    print(error_message)
    import traceback
    the_trace = traceback.format_exc()
    #traceback.print_exc(file=sys.stderr)
    with open('_FAILED.txt', 'wb') as f_out:
        f_out.writelines(error_message)
        f_out.writelines(traceback.format_exc())
    pym.update_analysis_status('FAILED', error_message, 'testbench_manifest.json')
    log = logging.getLogger()
    log.error(error_message)
    log.error("Exception was raised : {0}".format(traceback.format_exc()))
    sys.exit(1)

def init_and_compile_test_bench(tool_name='Dymola',
                                result_mat_file='',
                                dymola_home='',
                                model_config_file='model_config.json'):
    """
    Instantiates a new tool for simulation,
    compiles and then returns with the tool. 

    Raises ModelicaError if instantiation or
    compilation fails.
    """

    with open(model_config_file) as file_model:
        model_config = json.load(file_model)
    result_mat_file = model_config['result_file']
    del model_config['result_file']

    # Instantiate Modelica-Tool
    if tool_name == 'OpenModelica':
        modelica_tool = pym.OpenModelica(model_config)
        modelica_tool.short_name = result_mat_file
    elif tool_name == 'Dymola':
        dymola_home = r"C:\Program Files (x86)\Dymola 2014\bin64"
        if not os.path.isdir(dymola_home):
            dymola_home = r"C:\Program Files (x86)\Dymola 2013\bin64"
        if not os.path.isdir(dymola_home):
            raise pym.ModelicaError('Dymola installation not found!')
        modelica_tool = pym.Dymola(model_config, dymola_home)
        modelica_tool.result_mat = '{0}.mat'.format(result_mat_file) 

    # Compile
    modelica_tool.compile_model()

    return modelica_tool

def execute_test_bench(modelica_tool, report_file):
    """
    Reads in parameters from sumary.testresults.json and simulates the
    test bench. When done, writes metrics to sumary.testresults.json

    Can raise ModelicaError
    """
    log = logging.getLogger()
    # Read in parameters        
    parameters = pym.get_parameters_from_report_json(report_file)
    log.info('Parameters : {0}'.format(parameters))
    if parameters:
        modelica_tool.change_parameter(parameters)

    # Simulate model
    modelica_tool.simulate_model()
    t_1 = time.time()

    # TODO: uncomment the line once META-1226 is complete
    ## pym.update_metrics_and_check_limits(modelica_tool.result_mat, 'model_config.json')

    t_mat = time.time() - t_1

    # Run PostProcessing scripts
    try:
        t_post = pym.run_post_scripts(modelica_tool.result_mat, log)
    except subprocess.CalledProcessError as err:
        raise pym.ModelicaError('Post-script execution returned with error.', 
                                err.returncode)

    pym.update_analysis_status(new_status="OK", report_file=report_file)
    
    return t_mat, t_post    

def main():
    if not os.path.isdir('log'):
        os.mkdir('log')
    log = pym.instantiate_logger(os.path.join('log', 'dynamics_debug.log'))
    (options, args) = parser.parse_args()
    tool_name = options.tool

    # Set up paths
    this_file_path = os.path.dirname(os.path.abspath(__file__))
    os.chdir(os.path.join(this_file_path, '..', 'CyPhy'))
    print('current dir {0}'.format(os.getcwd()))
    report_file = os.path.join('..', 'testbench_manifest.json')

    try:
        # Read which tool to use and mat-filename from argument	
        modelica_tool = init_and_compile_test_bench(tool_name)
        t_mat, t_post = execute_test_bench(modelica_tool, report_file)
        # write statistics
        pym.write_out_tool_statistics(modelica_tool, t_mat, t_post, 
                                      os.path.join('..', 'tool_stat.json'))
    except pym.ModelicaError as err:
        _exit_on_failure('{0} : {1}'.format(err.message, err.sp_msg))

if __name__ == '__main__':
    try:
        main()
    except Exception as err:
        try:
            _exit_on_failure('{0} : {1}'.format(err.message, '(Not a caught ModelicaError)'))
        except Exception:
            sys.exit(13)  # Just in case something should fail in the exit function
