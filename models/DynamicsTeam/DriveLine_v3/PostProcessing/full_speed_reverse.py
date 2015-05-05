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
        filter.append('road_Wheel_Load_Both_Sides.ModelicaModel_road_Wheel_Load_Both_Sides.vehicleSpeed')

        # loads results with the filtered out variables (and 'time' which is default)
        pp = PostProcess(mat_file_name, filter)
        metrics = {}
        
        
        speed_array = pp.data_array("road_Wheel_Load_Both_Sides.ModelicaModel_road_Wheel_Load_Both_Sides.vehicleSpeed")
        time_array = pp.time_array()
        
        n = len(speed_array)
        acc10kph = -1
        points = range(n)
        for i in points:
            if speed_array[i] < -10:
                acc10kph = time_array[i]
                break
        
        
        max_rev_speed = speed_array[-1]
        
        points.reverse()
        tol = 0.001
        time_to_max = 0
        for i in points:
            if speed_array[i] - max_rev_speed > tol:
                time_to_max = time_array[i]
                break
        
        metrics.update({'MaxReverseSpeed': {'value': -max_rev_speed, 'unit': 'kph'}})
        metrics.update({'ReverseAcc10kph': {'value': acc10kph, 'unit': 's'}})
        metrics.update({'TimeToMaxReverseSpeed': {'value': time_to_max, 'unit': 's'}})

        update_metrics_in_report_json(metrics)
        ## end of postprocessing part

        ## Second limit part
        check_limits_and_add_to_report_json(pp, limit_dict)
        ## end of Second limit part


