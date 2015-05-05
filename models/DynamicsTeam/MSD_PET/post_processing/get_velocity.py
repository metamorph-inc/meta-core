import sys
import os
from common import PostProcess, update_metrics_in_report_json, read_limits, check_limits_and_add_to_report_json

if __name__ == '__main__':
    if len(sys.argv) > 1:
        try:
            mat_file_name = sys.argv[1]
            if not os.path.exists(mat_file_name):
                print 'Given result file does not exist: {0}'.format(sys.argv[1])
                os._exit(3)
            
            limits_dict, filter = read_limits()
            
            filter.append('SpeedSensor.v')
            # loads results with the filtered out variables (and 'time' which is default)
            pp = PostProcess(mat_file_name, filter)

            metrics = {}
            # No need to convert values into string that is done in update_metrics_in_report_json
            metrics.update({'Velocity' : {'value': pp.global_abs_max('SpeedSensor.v'), 'unit': 'm/s'}})
            
            cwd = os.getcwd()
            os.chdir('..')      
            update_metrics_in_report_json(metrics)
            
            check_limits_and_add_to_report_json(pp, limits_dict)
            os.chdir(cwd)
        except Exception as err:
            print err.message
            if os.name == 'nt':
                import win32api
                win32api.TerminateProcess(win32api.GetCurrentProcess(), 1)
            else:
                sys.exit(1)



