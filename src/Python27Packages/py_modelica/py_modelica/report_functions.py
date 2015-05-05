"""
Functions called after or before simulations of CyPhy-generated modelica models.
If any of these fail, they will throw an IOProcessingError
"""
import os
import json

from py_modelica.mat_file_functions.mat_file_processing import MatFileProcessing

REPORT_FILE = 'testbench_manifest.json'
INVALID_NUMBER_IN_REPORT_FILE = False


def invalid_number(x):
    print 'Found invalid {0} in report-file'.format(x)
    global INVALID_NUMBER_IN_REPORT_FILE
    INVALID_NUMBER_IN_REPORT_FILE = True
    return None


def get_analysis_status(report_file=REPORT_FILE):
    """
    Reads and returns the analysis status from report_file,
    default return is 'FAILED'

    """
    with open(report_file, 'r') as file_in:
        result_json = json.load(file_in)

    analysis_status = 'FAILED'

    if 'Status' in result_json:
        analysis_status = result_json['Status']

    return analysis_status


def update_analysis_status(new_status='', msg='', report_file=REPORT_FILE):
    """
    Updates the Status value in the report_file

    new_status has to be within {'UNEXECUTED', 'OK', 'FAILED'}

    """
    with open(report_file, 'r') as file_in:
        result_json = json.load(file_in)

    if 'Status' in result_json:
        result_json['Status'] = new_status

    if new_status == 'FAILED':
        msg_dic = {'ErrorMessage': msg}
        result_json.update(msg_dic)

    with open(report_file, 'wb') as file_out:
        json.dump(result_json, file_out, indent=4)


def get_parameters_from_report_json(report_file=REPORT_FILE):
    """
    Reads in parameters from report_file
    """

    parameters = {}
    with open(report_file) as file_in:
        result_json = json.load(file_in)

    if 'Parameters' in result_json:
        for param in result_json['Parameters']:
            try:
                parameters.update({str(param['Name']): float(param['Value'])})
            except ValueError:
                # if not float assume it's a boolean, thus string
                parameters.update({str(param['Name']): str(param['Value'])})

    return parameters


## Functions used for PET runs
def update_parameters_in_report_json(params, report_file=REPORT_FILE):
    """
    Updates the parameter values in report_file (json),
    using the params

    """
    with open(report_file, 'r') as f_in:
        result_json = json.load(f_in)

    if 'Parameters' in result_json:
        for paramOld in result_json['Parameters']:
            if paramOld['Name'] in params:
                paramOld['Value'] = str(params[paramOld['Name']])

    with open(report_file,'wb') as file_out:
        json.dump(result_json, file_out, indent=4)


def get_metrics_from_report_json(report_file=REPORT_FILE):
    """
    Reads the values of the metrics from the report_file
    and returns them as a dictionary. (Values are strings.)

    """
    metric_dict = {}
    with open(report_file, 'r') as f_in:
        result_json = json.load(f_in)

    if 'Metrics' in result_json:
        for metric in result_json['Metrics']:
            metric_dict.update({metric['Name']: metric['Value']})

    return metric_dict


def update_metrics_and_parameters(metrics, parameters, report_file=REPORT_FILE):
    """
    Replaces the Metrics and Parameters in the report-file with metrics and
    parameters.

    :param metrics:
    :param parameters:
    :param report_file:
    """

    with open(report_file, 'r') as f_in:
        result_json = json.load(f_in)

    result_json['Parameters'] = parameters
    result_json['Metrics'] = metrics

    with open(report_file, 'wb') as f_out:
        json.dump(result_json, f_out, indent=4)


def copy_metrics_and_parameters(report_file=REPORT_FILE):
    """
    Returns Parameters and Metrics from the report_file.
    :param report_file:
    """

    metrics = {}
    parameters = {}
    with open(report_file) as f_in:
        result_json = json.load(f_in)

    if 'Metrics' in result_json:
        metrics = result_json['Metrics']

    if 'Parameters' in result_json:
        parameters = result_json['Parameters']

    return metrics, parameters


def check_for_invalid_numbers(report_file=REPORT_FILE):
    """
    At the end of an execution make sure all NaN and (-)Inifity
    get converted to None (null) in the report_file. 
    
    Raises ValueError if encountered such values
    """
    with open(report_file, 'r') as f_in:
        result_json = json.load(f_in, parse_constant=invalid_number)

    if INVALID_NUMBER_IN_REPORT_FILE:
        import shutil
        shutil.copy(report_file, 'Has_invalid_numbers_' + report_file)

    with open(report_file, 'wb') as f_out:
        json.dump(result_json, f_out, indent=4)

    if INVALID_NUMBER_IN_REPORT_FILE:
        raise ValueError('Invalid values in {0}'.format(report_file))


## Functions operating on the results .mat-file
def update_metrics_and_check_limits(mat_file='', model_config='model_config.json', report_file=REPORT_FILE):
    """
    Update metrics in report_file after simulation,

    """

    report_file = os.path.join('..', report_file)  # This file is called from within the CyPhy folder.
    with open(model_config, 'r') as file_in:
        metrics_map = json.load(file_in)['metrics_map']

    limits_dict, variable_filter = read_limits()

    for key, value in metrics_map.iteritems():
        variable_filter.append(value)

    mfp = MatFileProcessing(mat_file, variable_filter)

    # read current summary report, which contains the metrics
    with open(report_file, 'r') as file_in:
        result_json = json.load(file_in)

    if 'Metrics' in result_json:
        for metric in result_json['Metrics']:
            if metric['Name'] in metrics_map:
                metric_value = str(mfp.last_value(metrics_map[metric['Name']]))
                metric['Value'] = metric_value
                print 'Metric {0} :: {1}'.format(metric['Name'], metric_value)
        with open(report_file, 'wb') as file_out:
            json.dump(result_json, file_out, indent=4)

    if limits_dict['LimitChecks']:
        check_limits_and_add_to_report_json(mfp, limits_dict)


## Helper functions
def read_limits():
    """
     Reads in limits and modifies the ModelicaUri to the correct one.

     Returns:
      - the updated limit_dict
      - the filter as a list
    """

    with open('limits.json', 'r') as f_in:
        limit_dict = json.load(f_in)

    # use set to avoid checking for duplicates
    variable_filter = set()
    for limit_item in limit_dict['LimitChecks']:
        variable_filter.add(limit_item['VariableFullPath'])

    variable_filter = list(variable_filter)
    print "Variables for limit-checking : {0}".format(variable_filter)

    return limit_dict, variable_filter


def check_limits_and_add_to_report_json(pp, limit_dict, report_file=REPORT_FILE):
    """
    Checks and and returns with an updated version of limit_dict
    """
    assert isinstance(pp, MatFileProcessing)
    report_file = os.path.join('..', report_file)  # This file is called from within the CyPhy folder.

    for limit_item in limit_dict['LimitChecks']:
        modelica_uri = limit_item['VariableFullPath']
        limit_value = limit_item['Value']
        limit_type = limit_item['Type']

        print "--== {0} ==--".format(modelica_uri)
        print "Type of Limit : {0}".format(limit_type)
        print "Limit : {0} ".format(limit_value)
        if limit_type == 'min':
            limit_exceeded, actual_value = pp.check_min_limit(modelica_uri, limit_value)
            limit_item['LimitExceeded'] = limit_exceeded
            limit_item['ActualValue'] = str(actual_value)

        elif limit_type == 'max':
            limit_exceeded, actual_value = pp.check_max_limit(modelica_uri, limit_value)
            limit_item['LimitExceeded'] = limit_exceeded
            limit_item['ActualValue'] = str(actual_value)

        else:
            limit_exceeded_max, actual_max_value = pp.check_max_limit(modelica_uri, limit_value)
            limit_exceeded_min, actual_min_value = pp.check_min_limit(modelica_uri, -limit_value)

            # determine the actual value depending on which limits were exceeded
            if limit_exceeded_max and limit_exceeded_min:
                if actual_max_value > abs(actual_min_value):
                    actual_value = str(actual_max_value)
                else:
                    actual_value = str(abs(actual_min_value))
            elif limit_exceeded_max:
                actual_value = str(actual_max_value)
            elif limit_exceeded_min:
                actual_value = str(abs(actual_min_value))
            else:
                actual_value = ''

            limit_item['LimitExceeded'] = limit_exceeded_max or limit_exceeded_min
            limit_item['ActualValue'] = actual_value

        limit_item['Value'] = str(limit_value)
        print "Violation : {0}".format(limit_item["LimitExceeded"])

    with open(report_file, 'r') as f_in:
        sum_rep_json = json.load(f_in)

    sum_rep_json['LimitChecks'] = limit_dict['LimitChecks']

    with open(report_file, 'wb') as f_out:
        json.dump(sum_rep_json, f_out, indent=4)
