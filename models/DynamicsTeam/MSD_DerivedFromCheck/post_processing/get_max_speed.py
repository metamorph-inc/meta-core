import sys
import os
from common import PostProcess, update_metrics_in_report_json, read_limits, check_limits_and_add_to_report_json
import numpy as np

def main():
    mat_file_name = sys.argv[1]
    limits_dict, filter = read_limits()
    
    filter.append('SpeedSensor.v')
    # loads results with the filtered out variables (and 'time' which is default)
    pp = PostProcess(mat_file_name, filter)
    metrics = {}
    # No need to convert values into string that is done in update_metrics_in_report_json
    metrics.update({'MaximumSpeed' : {'value': pp.global_max('SpeedSensor.v'), 'unit': 'm/s'}})
    
    cwd = os.getcwd()
    os.chdir('..')
    update_metrics_in_report_json(metrics)
    check_limits_and_add_to_report_json(pp, limits_dict)
    os.chdir(cwd)

if __name__ == '__main__':
    try:
        main()
    except Exception as err:
        import traceback
        trace = traceback.format_exc()
        print trace
        if os.name == 'nt':
            import win32api
            win32api.TerminateProcess(win32api.GetCurrentProcess(), 1)
        else:
            sys.exit(1)




