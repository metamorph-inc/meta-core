import sys
import os
from common import PostProcess, update_metrics_in_report_json
from common import read_limits, check_limits_and_add_to_report_json
import numpy as np

if __name__ == '__main__':
    try:
        mat_file_name = sys.argv[1]
        if not os.path.exists(mat_file_name):
            print 'Given result file does not exist: {0}'.format(sys.argv[1])
            os._exit(3)

        ## First limit part
        limit_dict, filter = read_limits()
        ## End of first limit part

        
        ## Post processing part
        filter.append('PositionSensor.s')
        filter.append('SpeedSensor.v')
        filter.append('AccSensor.a')

        # loads results with the filtered out variables (and 'time' which is default)
        pp = PostProcess(mat_file_name, filter)
        metrics = {}
        time_array = pp.time_array()
        velocity_array = pp.data_array('SpeedSensor.v')
        acceleration_array = pp.data_array('AccSensor.a')
        position = pp.last_value('PositionSensor.s')

	
		
        zero = pp.find_zero(velocity_array,acceleration_array,time_array)
        if zero != -1:
			time_to_zero = zero
			stopped_moving = True
        else:
            time_to_zero = 10000    
            stopped_moving = False

        
        metrics.update({'Time_to_Zero': {'value': time_to_zero, 'unit': 's'}})
        metrics.update({'Stopped_Moving': {'value': stopped_moving, 'unit': 'T/F'}})
        metrics.update({'Final_Position': {'value': abs(position),'unit': 'm'}})

        cwd = os.getcwd()
        os.chdir('..')        
        update_metrics_in_report_json(metrics)
        ## end of postprocessing part

        ## Second limit part
        ## check_limits_and_add_to_report_json(pp, limit_dict)
        ## end of Second limit part

        os.chdir(cwd)
    except Exception as err:
        print err.message
        if os.name == 'nt':
            import win32api
            win32api.TerminateProcess(win32api.GetCurrentProcess(), 1)
        else:
            sys.exit(1)