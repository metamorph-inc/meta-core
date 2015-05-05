import sys
import os
from common import PostProcess, update_metrics_in_report_json
from common import read_limits, check_limits_and_add_to_report_json
import numpy as np

if __name__ == '__main__':
    if len(sys.argv) > 1:
        mat_file_name = sys.argv[1]
        if not os.path.exists(mat_file_name):
            print 'Given result file does not exist: {0}'.format(sys.argv[1])
            os._exit(3)

        ## First limit part
        limit_dict, filter = read_limits()
        ## End of first limit part
        
        ## Post processing part
        filter.append('road_Wheel_Load_Both_Sides.vehicleSpeed')
        filter.append('road_Wheel_Load_Both_Sides.Accel_20kph')
        filter.append('design_v1.EngineHeatPort.T')

        # loads results with the filtered out variables (and 'time' which is default)
        pp = PostProcess(mat_file_name, filter)
        metrics = {}
        
        metrics.update({'MaxVehicleSpeed': {'value': pp.global_abs_max("road_Wheel_Load_Both_Sides.vehicleSpeed"), 'unit': 'kph'}})
        metrics.update({'Acc20kph': {'value': pp.last_value('road_Wheel_Load_Both_Sides.Accel_20kph'), 'unit': 's'}})
        metrics.update({'EngineTemperature': {'value': pp.last_value('design_v1.EngineHeatPort.T') - 273.15, 'unit': 'C'}})

        cwd = os.getcwd()
        os.chdir('..')        
        update_metrics_in_report_json(metrics)
        ## end of postprocessing part

        ## Second limit part
        check_limits_and_add_to_report_json(pp, limit_dict)
        ## end of Second limit part

        os.chdir(cwd)  
