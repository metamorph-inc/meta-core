import sys
import os
from common import PostProcess, update_metrics_in_report_json
from common import read_limits, check_limits_and_add_to_report_json


if __name__ == '__main__':
    if len(sys.argv) > 1:
        try:
            mat_file_name = sys.argv[1]
            ## First limit part
            limit_dict, filter = read_limits()

            ## Post processing part
            filter.append('road_Wheel_Load_Both_Sides.vehicleSpeed')
            filter.append('road_Wheel_Load_Both_Sides.Accel_20kph')

            # loads results with the filtered out variables (and 'time' which is default)
            pp = PostProcess(mat_file_name, filter)

            metrics = {}
            metrics.update({'VehicleSpeed': {'value': pp.global_abs_max("road_Wheel_Load_Both_Sides.vehicleSpeed"), 'unit': 'kph'}})
            metrics.update({'Acc20kph': {'value': pp.last_value('road_Wheel_Load_Both_Sides.Accel_20kph'), 'unit': 's'}})

            cwd = os.getcwd()
            os.chdir('..')    
            update_metrics_in_report_json(metrics)
            check_limits_and_add_to_report_json(pp, limit_dict)
            os.chdir(cwd)
        except Exception as err:
            print err.message
            if os.name == 'nt':
                import win32api
                win32api.TerminateProcess(win32api.GetCurrentProcess(), 1)
            else:
                sys.exit(8)
